#include "KCore.h"
/*#include "KPlayerPartner.h"
#include "KPlayer.h"
#include "KNpcSet.h"
#include "KNpc.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif

#ifdef _SERVER
void KPlayerPartner::Init(KPlayer* pPlayer)
{
	m_nPartnerIdx = 0;
	m_bPartnerCallOut = FALSE;
	m_pPlayer = pPlayer;
}

int KPlayerPartner::Add(int nNpcTemplateId, int nLevel, char* szName)
{
	if (nNpcTemplateId < 0) 
		return FALSE;
	
	if (nLevel < 0 ) 
		return FALSE;
	
	int	nNpcIdxInfo = MAKELONG(nLevel, nNpcTemplateId);
	
	int nSubWorld, nMpsX, nMpsY;
	nSubWorld = Npc[m_pPlayer->m_nIndex].m_SubWorldIndex;
	Npc[m_pPlayer->m_nIndex].GetMpsPos(&nMpsX, &nMpsY);
	
	int nNpcIdx = NpcSet.Add(0, nNpcIdxInfo, nSubWorld, nMpsX, nMpsY);
	
	if (nNpcIdx > 0)
	{
		RemoveCurPartner();

		if (szName[0])
			strcpy(Npc[nNpcIdx].Name, szName);
		Npc[nNpcIdx].m_Series = series_num;
		Npc[nNpcIdx].m_Kind = kind_partner;

		m_nPartnerIdx = nNpcIdx;
		m_bPartnerCallOut = TRUE;
	}

	return nNpcIdx;
}

int KPlayerPartner::Add(BYTE * pPartnerBuffer)
{
	if (!pPartnerBuffer) 
		return 0;

	TPartnerInfo * pPartnerData = (TPartnerInfo *) pPartnerBuffer;
	
	if (pPartnerData->nNpcTemplateId < 0 || pPartnerData->nLevel < 0 ) 
		return 0;
	
	int	nNpcIdxInfo = MAKELONG(pPartnerData->nLevel, pPartnerData->nNpcTemplateId);
	
	int nSubWorld, nMpsX, nMpsY;
	nSubWorld = Npc[m_pPlayer->m_nIndex].m_SubWorldIndex;
	Npc[m_pPlayer->m_nIndex].GetMpsPos(&nMpsX, &nMpsY);
	
	int nNpcIdx = NpcSet.Add(0,nNpcIdxInfo, nSubWorld, nMpsX, nMpsY);
	
	if (nNpcIdx > 0)
	{
		RemoveCurPartner();

		if (pPartnerData->szName[0])
			strcpy(Npc[nNpcIdx].Name, pPartnerData->szName);

		Npc[nNpcIdx].m_Series = series_num;
		Npc[nNpcIdx].m_Kind = kind_partner;

		m_nPartnerIdx = nNpcIdx;
		m_bPartnerCallOut = pPartnerData->bCallOut;

		if (FALSE == m_bPartnerCallOut)
		{
			if (Npc[m_nPartnerIdx].m_SubWorldIndex >= 0 && Npc[m_nPartnerIdx].m_RegionIndex >= 0)
			{
				SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].m_Region[Npc[m_nPartnerIdx].m_RegionIndex].DecRef(Npc[m_nPartnerIdx].m_MapX, Npc[m_nPartnerIdx].m_MapY, obj_npc);
			}
			
			NPC_REMOVE_SYNC	RemoveSync;
			RemoveSync.ProtocolType = s2c_npcremove;
			RemoveSync.ID = Npc[m_nPartnerIdx].m_dwID;
			RemoveSync.bInMap = TRUE;
			Npc[m_nPartnerIdx].SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));

			SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].NpcChangeRegion(Npc[m_nPartnerIdx].m_RegionIndex, VOID_REGION, Npc[m_nPartnerIdx].m_Index);

			Npc[m_nPartnerIdx].m_RegionIndex = VOID_REGION;
		}
	}

	return nNpcIdx;
}

BOOL KPlayerPartner::Activate()
{
	if (!m_nPartnerIdx || !m_bPartnerCallOut)
		return FALSE;
	
	if (IsOwnerOutOfRange(500000))
	{
		int nOwnerSubWorldID, nOwnerMpsX, nOwnerMpsY;

		nOwnerSubWorldID = SubWorld[Npc[m_pPlayer->m_nIndex].m_SubWorldIndex].m_SubWorldID;
		Npc[m_pPlayer->m_nIndex].GetMpsPos(&nOwnerMpsX, &nOwnerMpsY);

		Npc[m_nPartnerIdx].ChangeWorld(nOwnerSubWorldID, nOwnerMpsX, nOwnerMpsY);

		return TRUE;
	}

	RunToOwner();
	return TRUE;
}

BOOL KPlayerPartner::IsOwnerOutOfRange(int nRange)
{
	if (!m_nPartnerIdx || !m_bPartnerCallOut)
		return FALSE;

	// 和主人在不同地图 或 和主人在同一地图距离太远
	int	nDisSquare = NpcSet.GetDistanceSquare(Npc[m_nPartnerIdx].m_Index, m_pPlayer->m_nIndex);
	if (nDisSquare < 0 || nDisSquare > nRange)
		return TRUE;

	return FALSE;
}

void KPlayerPartner::RunToOwner()
{
	if (!m_nPartnerIdx || !m_bPartnerCallOut)
		return;

	if (IsOwnerOutOfRange(20000))
	{
		int nOwnerX, nOwnerY, nPartnerX, nPartnerY, nXGo, nYGo;

		Npc[m_pPlayer->m_nIndex].GetMpsPos(&nOwnerX, &nOwnerY);
		Npc[m_nPartnerIdx].GetMpsPos(&nPartnerX, &nPartnerY);

		nXGo = nOwnerX + ((nOwnerX - nPartnerX) > 0 ? -1 : 1) * 50;
		nYGo = nOwnerY + ((nOwnerY - nPartnerY) > 0 ? -1 : 1) * 50;

		Npc[m_nPartnerIdx].SendCommand(do_walk, nXGo, nYGo);
	}
}

void KPlayerPartner::RemoveCurPartner()
{
	if (!m_nPartnerIdx || !m_bPartnerCallOut)
		return;

	if (m_nPartnerIdx)
	{
		if (Npc[m_nPartnerIdx].m_RegionIndex >= 0)
		{
			SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].m_Region[Npc[m_nPartnerIdx].m_RegionIndex].RemoveNpc(m_nPartnerIdx);
			SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].m_Region[Npc[m_nPartnerIdx].m_RegionIndex].DecRef(Npc[m_nPartnerIdx].m_MapX, Npc[m_nPartnerIdx].m_MapY, obj_npc);
		}
		NpcSet.Remove(m_nPartnerIdx);
	}
}

void KPlayerPartner::CallOut()
{
	if (m_bPartnerCallOut)
		return;

	int nOldSubWorldIdx = Npc[m_nPartnerIdx].m_SubWorldIndex;
	int nOldRegionIdx = Npc[m_nPartnerIdx].m_RegionIndex;
	int nOldMapX = Npc[m_nPartnerIdx].m_MapX;
	int nOldMapY = Npc[m_nPartnerIdx].m_MapY;
	int nNewSubWorldIdx = Npc[m_pPlayer->m_nIndex].m_SubWorldIndex;
	int nNewRegionIdx = Npc[m_pPlayer->m_nIndex].m_RegionIndex;
	int nNewMapX = Npc[m_pPlayer->m_nIndex].m_MapX;
	int nNewMapY = Npc[m_pPlayer->m_nIndex].m_MapY;
	int nNewMapZ = Npc[m_pPlayer->m_nIndex].m_MapZ;

	if (nOldSubWorldIdx >= 0 && nOldRegionIdx >= 0)
	{
		SubWorld[nOldSubWorldIdx].m_Region[nOldRegionIdx].DecRef(nOldMapX, nOldMapY, obj_npc);
	}
	
	if (nNewSubWorldIdx >= 0 && nNewRegionIdx >= 0)
	{
		Npc[m_nPartnerIdx].m_MapX = nNewMapX;
		Npc[m_nPartnerIdx].m_MapY = nNewMapY;
		Npc[m_nPartnerIdx].m_MapZ = nNewMapZ;
		SubWorld[nNewSubWorldIdx].m_Region[nNewRegionIdx].AddRef(nNewMapX, nNewMapY, obj_npc);
	}

	SubWorld[nNewSubWorldIdx].NpcChangeRegion(nOldRegionIdx, nNewRegionIdx, Npc[m_nPartnerIdx].m_Index);
	Npc[m_nPartnerIdx].m_SubWorldIndex = nNewSubWorldIdx;

	m_bPartnerCallOut = TRUE;
}

void KPlayerPartner::UnCallOut()
{
	if (!m_bPartnerCallOut)
		return;

	if (Npc[m_nPartnerIdx].m_SubWorldIndex >= 0 && Npc[m_nPartnerIdx].m_RegionIndex >= 0)
	{
		SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].m_Region[Npc[m_nPartnerIdx].m_RegionIndex].DecRef(Npc[m_nPartnerIdx].m_MapX, Npc[m_nPartnerIdx].m_MapY, obj_npc);
	}
	
	NPC_REMOVE_SYNC	RemoveSync;
	RemoveSync.ProtocolType = s2c_npcremove;
	RemoveSync.ID = Npc[m_nPartnerIdx].m_dwID;
	RemoveSync.bInMap = TRUE;
	Npc[m_nPartnerIdx].SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));

	SubWorld[Npc[m_nPartnerIdx].m_SubWorldIndex].NpcChangeRegion(Npc[m_nPartnerIdx].m_RegionIndex, VOID_REGION, Npc[m_nPartnerIdx].m_Index);

	Npc[m_nPartnerIdx].m_RegionIndex = VOID_REGION;

	m_bPartnerCallOut = FALSE;
}

int KPlayerPartner::UpdateDBInfo(BYTE * pPartnerBuffer)
{
	if (!pPartnerBuffer)
		return -1;
	
	if (!m_pPlayer || !m_nPartnerIdx)
		return 0;

	TPartnerInfo * pPartnerData = (TPartnerInfo *) pPartnerBuffer;

	strcpy(pPartnerData->szName, Npc[m_nPartnerIdx].Name);
	pPartnerData->nNpcTemplateId = Npc[m_nPartnerIdx].m_NpcSettingIdx;
	pPartnerData->nLevel = Npc[m_nPartnerIdx].m_Level;
	pPartnerData->bCallOut = m_bPartnerCallOut;

	if (m_pPlayer->IsWaitingRemove())
		RemoveCurPartner();
	
	return 1;
}
#endif*/