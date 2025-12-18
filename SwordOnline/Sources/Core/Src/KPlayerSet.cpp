
#include "KCore.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "GameDataDef.h"
#include "KProtocolProcess.h"
#ifdef _SERVER
#include "KSubWorldSet.h"
#include "CoreServerShell.h"
#include "KLicense.h"
#include <time.h>
#endif
#include "KPlayerSet.h"
#include "Text.h"
#include "CoreUseNameDef.h"

#define		PLAYER_FIRST_LUCKY				0

KPlayerSet PlayerSet;

#ifdef _SERVER
KLicense	g_PlayerLicense;
#endif

// --
//
// --
KPlayerSet::KPlayerSet()
{
#ifdef _SERVER
	m_ulNextSaveTime	= 0;
	m_ulDelayTimePerSave		= 0;
	m_ulMaxSaveTimePerPlayer	= 0;
	
	m_pResetTask		= NULL;
	m_nResetTaskCount	= 0;
	m_pSyncTask			= NULL;
	m_nSyncTaskCount	= 0;
#endif


#ifndef _SERVER
	memset(m_szFortuneRankPic, 0, sizeof(m_szFortuneRankPic));	
#endif
}

// --

// --
KPlayerSet::~KPlayerSet()
{
#ifdef _SERVER
	if(m_pResetTask)
		delete [] m_pResetTask;
	m_pResetTask = NULL;
	if(m_pSyncTask)
		delete [] m_pSyncTask;
	m_pSyncTask = NULL;
#endif
}

// --
//
// --
BOOL	KPlayerSet::Init()
{
	int i;
#ifdef _SERVER
	m_nNumPlayer = 0;
	m_ulNextSaveTime = 0;
	m_ulMaxSaveTimePerPlayer = 10 * 60 * 20;
	m_ulDelayTimePerSave = m_ulMaxSaveTimePerPlayer / MAX_PLAYER;
	
	m_bLock			= false;
	m_nExpRate		= 100;
	m_nMoneyRate	= 100;
	
#endif

	m_FreeIdx.Init(MAX_PLAYER);
	m_UseIdx.Init(MAX_PLAYER);

	for (i = MAX_PLAYER - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
	}

	if ( !m_cLevelAdd.Init() )
		return FALSE;
	if ( !m_cLeadExp.Init() )
		return FALSE;
	for (i = 0; i < MAX_PLAYER; i++)
	{
		Player[i].Release();
		Player[i].SetPlayerIndex(i);
#ifdef _SERVER
		Player[i].m_cPK.Init(i);
#endif
		Player[i].m_cTong.Init(i);
		Player[i].m_ItemList.Init(i);
		Player[i].m_Node.m_nIndex = i;
	}

	if ( !m_cNewPlayerAttribute.Init() )
		return FALSE;

#ifdef _SERVER
	KTabFile	cPKParam;

	memset(m_sPKPunishParam, 0, sizeof(m_sPKPunishParam));
	if (!cPKParam.Load(defPK_PUNISH_FILE))
		return FALSE;
	for (i = 0; i < MAX_DEATH_PUNISH_PK_VALUE + 1; i++)
	{
		cPKParam.GetInteger(i + 2, 2, 1, &m_sPKPunishParam[i].m_nExp);
		cPKParam.GetInteger(i + 2, 3, 1, &m_sPKPunishParam[i].m_nMoney);
		cPKParam.GetInteger(i + 2, 4, 1, &m_sPKPunishParam[i].m_nItem);
		cPKParam.GetInteger(i + 2, 5, 1, &m_sPKPunishParam[i].m_nEquip);
	}
#endif
	KIniFile	cTongFile;
	if (cTongFile.Load(defPLAYER_TONG_PARAM_FILE))
	{
		cTongFile.GetInteger("TongCreate", "Level", 60, &m_sTongParam.m_nLevel);
		cTongFile.GetInteger("TongCreate", "LeadLevel", 10, &m_sTongParam.m_nLeadLevel);
		cTongFile.GetInteger("TongCreate", "Money", 1000000, &m_sTongParam.m_nMoney);
		cTongFile.GetShort("TongCreate", "Item", 1, &m_sTongParam.m_nItemIdx);
		cTongFile.GetShort("TongCreate", "Repute", 300, &m_sTongParam.m_nRepute);

		cTongFile.GetInteger("TongChange", "Price", 1000000, &m_sTongParam.m_nChangePrice);
		cTongFile.GetInteger("TongLeave", "Money", 1000000, &m_sTongParam.m_nLeaveMoney);
	}
	else
	{
		m_sTongParam.m_nLevel		= 60;
		m_sTongParam.m_nLeadLevel	= 10;
		m_sTongParam.m_nMoney		= 1000000;
		m_sTongParam.m_nItemIdx		= 1;
		m_sTongParam.m_nRepute		= 300;
		m_sTongParam.m_nChangePrice	= 1000000;
		m_sTongParam.m_nLeaveMoney	= 1000000;
	}
#ifndef _SERVER
	// Load FortuneRank sprites for mantle display
	char Buff[32];
	for (i = 0; i < MAX_ITEM_LEVEL + 1; i++)
	{
		sprintf(Buff, "Spr_%d", i);
		g_GameSettingFile.GetString("FortuneRank", Buff, "", m_szFortuneRankPic[i], sizeof(m_szFortuneRankPic[i]));
	}
#endif

#ifdef _SERVER
	g_GameSettingFile.GetShort("ServerConfig", "ExpRate", 100, &m_nExpRate);
	g_GameSettingFile.GetShort("ServerConfig", "MoneyRate", 100, &m_nMoneyRate);
	
	g_GameSettingFile.GetShort("SyncTask", "Count", 0, &m_nSyncTaskCount);
	if(m_nSyncTaskCount > 0)
	{	
		m_pSyncTask = new KTaskObject[m_nSyncTaskCount];
		memset(m_pSyncTask, 0, sizeof(KTaskObject) * m_nSyncTaskCount);

		char szBuff[16];
		for(i = 0; i < m_nSyncTaskCount; i++)
		{	
			sprintf(szBuff, "%d", i + 1);
			g_GameSettingFile.GetShort("SyncTask", szBuff, 0, &m_pSyncTask[i].m_nID);
		}
	}

	g_GameSettingFile.GetShort("ResetTask", "Count", 0, &m_nResetTaskCount);
	if(m_nResetTaskCount > 0)
	{	
		m_pResetTask = new KTaskObject[m_nResetTaskCount];
		memset(m_pResetTask, 0, sizeof(KTaskObject) * m_nResetTaskCount);

		char szBuff[16];
		for(i = 0; i < m_nResetTaskCount; i++)
		{	
			sprintf(szBuff, "%d_No", i + 1);
			g_GameSettingFile.GetShort("ResetTask", szBuff, 0, &m_pResetTask[i].m_nID);
			sprintf(szBuff, "%d_Type", i + 1);
			g_GameSettingFile.GetShort("ResetTask", szBuff, 0, (short*)&m_pResetTask[i].m_bSync);
		}
	}
#endif
	return TRUE;
}

#ifdef _SERVER
// --
//
// --
void	KPlayerSet::Activate()
{
}
#endif

// --
//
// --
int	KPlayerSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

// --
//
// --
int KPlayerSet::FindSame(DWORD dwID)
{
	int nUseIdx = 0;

	nUseIdx = m_UseIdx.GetNext(0);
	while(nUseIdx)
	{
		if (Player[nUseIdx].m_dwID == dwID)
			return nUseIdx;
		nUseIdx = m_UseIdx.GetNext(nUseIdx);
	}
	return 0;
}

// --
//
// --
int		KPlayerSet::GetFirstPlayer()
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return m_nListCurIdx;
}

// --
//
// --
int		KPlayerSet::GetNextPlayer()
{
	if ( !m_nListCurIdx )
		return 0;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return m_nListCurIdx;
}

#ifdef _SERVER
// --
//
// --
int KPlayerSet::Add(LPSTR szPlayerID, void* pGuid)
{
	if (!pGuid || !szPlayerID || !szPlayerID[0])
		return 0;

	const int i = FindFree();
	if(i)
	{	
		DWORD dwID = g_FileName2Id(szPlayerID);

		Player[i].m_dwID = dwID;
		Player[i].m_nNetConnectIdx = -1;
		Player[i].m_dwLoginTime = g_SubWorldSet.GetGameTime();
		memcpy(&Player[i].m_Guid, pGuid, sizeof(GUID));
		Player[i].SetPlayerIndex(i);
		//
		m_FreeIdx.Remove(i);
		m_UseIdx.Insert(i);
		m_nNumPlayer++;
		//
		return i;
	}
	return 0;
}

// --
//
// --
int	KPlayerSet::Broadcasting(char* pMessage, int nLen)
{
	if ( !pMessage || nLen <= 0 || nLen >= MAX_SENTENCE_LENGTH)
		return 0;
	if (!g_pServer)
		return 0;
	g_pServer->PreparePackSink();
	KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, pMessage, nLen);
	g_pServer->SendPackToClient(-1);
	return 1;
}

// --
//
// --
void KPlayerSet::PrepareRemove(int nIndex)
{
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	

#ifdef _DEBUG
	//g_DebugLog("SERVER:Player[%s] has been removed!", Npc[Player[nIndex].m_nIndex].Name);
#endif

	// them by kinnox
	if (Player[nIndex].m_dwLogoutScriptId)
	{
		Player[nIndex].ExecuteScript(Player[nIndex].m_dwLogoutScriptId, "OnLogout", nIndex); 

	}
	int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
	if (nSubWorld >= 0)
	{
		SubWorld[nSubWorld].m_MissionArray.RemovePlayer(nIndex);
	} 
	//end code;

	Player[nIndex].m_cChat.OffLine(Player[nIndex].m_dwID);

	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[nIndex].LeaveTeam((BYTE*)&sLeaveTeam);

	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[nIndex].TradeDecision((BYTE*)&sTrade);

	Player[nIndex].m_cPK.CloseAll();
	Player[nIndex].WaitForRemove();

	Player[nIndex].ExecuteScript(PLAYER_LOGOUT_SCRIPT_FILE, "main", 0);
}

// --
//
// --
void KPlayerSet::PrepareLoginFailed(int nIndex)
{
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	Player[nIndex].m_cChat.OffLine(Player[nIndex].m_dwID);
	
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[nIndex].LeaveTeam((BYTE*)&sLeaveTeam);

	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[nIndex].TradeDecision((BYTE*)&sTrade);
	
	Player[nIndex].m_cPK.CloseAll();
	Player[nIndex].m_cTong.Clear();
	Player[nIndex].m_nNetConnectIdx = -1;
	Player[nIndex].m_dwLoginTime = 0;
}

// --
//
// --
void KPlayerSet::PrepareExchange(int i)
{
	if (!Player[i].m_bExchangeServer || !Npc[Player[i].m_nIndex].m_bExchangeServer)
		return;

	Player[i].m_cPK.CloseAll();

	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[i].LeaveTeam((BYTE*)&sLeaveTeam);

	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[i].TradeDecision((BYTE*)&sTrade);

	Player[i].Save();
}

// --
//
// --
void KPlayerSet::RemoveQuiting(int nIndex)
{

	if( (Player[nIndex].m_nNetConnectIdx == -1) || (Player[nIndex].m_dwID == 0) )
	{
		return;
	}
	if (Player[nIndex].IsWaitingRemove())
	{	
		if (Player[nIndex].m_nIndex > 0)
		{
			int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
			int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
			
			if (nSubWorld >= 0 && nRegion >= 0)
			{
				SubWorld[nSubWorld].RemovePlayer(nRegion, nIndex);
				SubWorld[nSubWorld].m_MissionArray.RemovePlayer(nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
			}	
			NpcSet.Remove(Player[nIndex].m_nIndex);
		}
		//
		char szData[128];

		sprintf(szData,"So Tien:%d - So xu:%d - Sè l­îng Item:%d/%d", Player[nIndex].m_ItemList.GetMoney(room_equipment) + Player[nIndex].m_ItemList.GetMoney(room_repository), Player[nIndex].GetExtPoint(), Player[nIndex].m_ItemList.GetItemCount(), MAX_PLAYER_ITEM);
		//
//		Player[nIndex].SaveLog(1, szData, "LOG_OUT_GAME", "");

		//
		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_ItemList.Release();
		Player[nIndex].m_cTong.Clear();
		Player[nIndex].m_cTask.Release();
		//
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].Release();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer--;
	}
}	

// --
//
// --
void KPlayerSet::RemoveLoginTimeOut(int nIndex)
{
	if( Player[nIndex].IsLoginTimeOut() )
	{
		if (Player[nIndex].m_nIndex > 0)
		{
			int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
			int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
		
			if (nSubWorld >= 0 && nRegion >= 0)
			{
				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
			}
			NpcSet.Remove(Player[nIndex].m_nIndex);
		}		
		//
		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_ItemList.Release();
		Player[nIndex].m_cTong.Clear();
		Player[nIndex].m_cTask.Release();
		//
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].LoginTimeOut();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer--;
	}
}

// --
//
// --
void KPlayerSet::RemoveExchanging(int nIndex)
{

	if (Player[nIndex].m_nNetConnectIdx == -1)
	{
		return;
	}

	if (Player[nIndex].IsExchangingServer())
	{
		int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
		int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
		
		if (nSubWorld >= 0 && nRegion >= 0)
		{
			SubWorld[nSubWorld].RemovePlayer(nRegion, nIndex);
			SubWorld[nSubWorld].m_MissionArray.RemovePlayer(nIndex);
			SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
			SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
		}
		
		if (Player[nIndex].m_nIndex > 0)
			NpcSet.Remove(Player[nIndex].m_nIndex);
		
		//
		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_ItemList.Release();
		Player[nIndex].m_cTong.Clear();
		Player[nIndex].m_cTask.Release();
		//
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].Release();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer--;
	}
}
#endif


#ifdef _SERVER
// --
//
// --
void KPlayerSet::ProcessClientMessage(int nIndex, const char* pChar, int nSize)
{
	if (nIndex <= 0 && nIndex >= MAX_PLAYER)
		return;

	int i = Player[nIndex].m_nNetConnectIdx;

	if (i >= 0)
	{
		if (Player[nIndex].m_dwID && !Player[nIndex].m_bExchangeServer)
			g_ProtocolProcess.ProcessNetMsg(nIndex, (BYTE*)pChar);
	}
}
#endif

#ifdef _SERVER
// --
//
// --
BOOL	KPlayerSet::GetPlayerName(int nIndex, char* szName)
{
	int i = nIndex;

	if (!szName)
		return FALSE;

	if (i <= 0 || i >= MAX_PLAYER)
	{
		szName[0] = 0;
		return FALSE;
	}
	strcpy(szName, Player[i].m_PlayerName);
	return TRUE;
}
#endif

#ifdef _SERVER
// --
//
// --
BOOL	KPlayerSet::GetPlayerAccount(int i, char* szName)
{
	if (!szName)
		return FALSE;

	if (i <= 0 || i >= MAX_PLAYER)
	{
		szName[0] = 0;
		return FALSE;
	}

	strcpy(szName, Player[i].m_AccoutName);
	return TRUE;
}
#endif


#ifdef _SERVER
// --
//
// --
int		KPlayerSet::AttachPlayer(const unsigned long lnID, GUID* pGuid)
{
	if (lnID >= MAX_PLAYER || NULL == pGuid)
		return 0;

	int nUseIdx = m_UseIdx.GetPrev(0);
	while(nUseIdx)
	{
		if (Player[nUseIdx].m_nNetConnectIdx == -1)
		{
			if (0 == memcmp(&Player[nUseIdx].m_Guid, pGuid, sizeof(GUID)))
			{
				Player[nUseIdx].m_nNetConnectIdx = lnID;
				Player[nUseIdx].m_ulLastSaveTime = g_SubWorldSet.m_nLoopRate;
				// FIX: Reset FightMode when re-attaching player to same GameServer
				// Bug: When player logout then login again to SAME GS, the old Npc object
				// is reused but m_FightMode is NOT reset, staying at 0 (cannot attack).
				// This causes Nga My and other classes to be unable to attack NPCs.
				if (Player[nUseIdx].m_nIndex > 0)
				{
					Npc[Player[nUseIdx].m_nIndex].SetFightMode(TRUE);
				}
				return nUseIdx;
			}
		}
		nUseIdx = m_UseIdx.GetPrev(nUseIdx);
	}
	return 0;
}
#endif

#ifdef _SERVER
// --
//
// --
int		KPlayerSet::GetPlayerIndexByGuid(GUID* pGuid)
{
	int nUseIdx = m_UseIdx.GetNext(0);
	while(nUseIdx)
	{
		if (0 == memcmp(&Player[nUseIdx].m_Guid, pGuid, sizeof(GUID)))
		{
			if (Player[nUseIdx].m_nNetConnectIdx != -1)
			{
				return nUseIdx;
			}
			else
			{
				return 0;
			}
		}
		nUseIdx = m_UseIdx.GetNext(nUseIdx);
	}
	return 0;
}
#endif

#ifdef _SERVER
// --

// --
void KPlayerSet::AutoSave()
{
	unsigned long uTime = g_SubWorldSet.GetGameTime();
	if (uTime >= m_ulNextSaveTime)
	{
		int nUseIdx = m_UseIdx.GetNext(0);
		while(nUseIdx)
		{
			if (Player[nUseIdx].CanSave() && uTime - Player[nUseIdx].m_ulLastSaveTime >= m_ulMaxSaveTimePerPlayer)
			{	
				if (Player[nUseIdx].Save())
				{	
					Player[nUseIdx].m_uMustSave = SAVE_REQUEST;
					m_ulNextSaveTime += m_ulDelayTimePerSave;
				}
				break;
			}
			nUseIdx = m_UseIdx.GetNext(nUseIdx);
		}
	}
}
#endif


#ifdef _SERVER
// --
//
// --
int		KPlayerSet::GetFirstPlayerEx()
{
	return m_UseIdx.GetNext(0);
}
#endif

#ifdef _SERVER
// --
//
// --
int		KPlayerSet::GetNextPlayerEx(int nCurIdx)
{
	if (!nCurIdx)
		return 0;

	return m_UseIdx.GetNext(nCurIdx);
}
#endif



// --
//	
// --
KLevelAdd::KLevelAdd()
{
	memset(m_nLevelExp, 0, sizeof(int) * MAX_LEVEL);
	memset(m_nLifePerLevel, 0, sizeof(int) * series_num);
	memset(m_nManaPerLevel, 0, sizeof(int) * series_num);
	memset(m_nStaminaMalePerLevel, 0, sizeof(int) * series_num);		
	memset(m_nStaminaFemalePerLevel, 0, sizeof(int) * series_num);		
	memset(m_nLifePerVitality, 0, sizeof(int) * series_num);
	memset(m_nStaminaPerVitality, 0, sizeof(int) * series_num);
	memset(m_nManaPerEnergy, 0, sizeof(int) * series_num);
}

// --
//	
// --
BOOL	KLevelAdd::Init()
{
	int			i;
	KTabFile	LevelExp;
	if ( !LevelExp.Load(PLAYER_LEVEL_EXP_FILE) )
		return FALSE;
	for (i = 0; i < MAX_LEVEL; i++)
	{
		LevelExp.GetInteger(i + 2, 2, 0, &m_nLevelExp[i]);
	}

	KTabFile	LevelAdd;
	if ( !LevelAdd.Load(PLAYER_LEVEL_ADD_FILE) )
		return FALSE;
	for (i = 0; i < series_num; i++)
	{
		LevelAdd.GetInteger(i + 2, 2, 0, &m_nLifePerLevel[i]);
		LevelAdd.GetInteger(i + 2, 3, 0, &m_nStaminaMalePerLevel[i]);
		LevelAdd.GetInteger(i + 2, 4, 0, &m_nStaminaFemalePerLevel[i]);
		LevelAdd.GetInteger(i + 2, 5, 0, &m_nManaPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 6, 0, &m_nLifePerVitality[i]);
		LevelAdd.GetInteger(i + 2, 7, 0, &m_nStaminaPerVitality[i]);
		LevelAdd.GetInteger(i + 2, 8, 0, &m_nManaPerEnergy[i]);
		LevelAdd.GetInteger(i + 2, 9, 0, &m_nLeadExpShare[i]);
		LevelAdd.GetInteger(i + 2, 10, 0, &m_nFireResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 11, 0, &m_nColdResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 12, 0, &m_nPoisonResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 13, 0, &m_nLightResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 14, 0, &m_nPhysicsResistPerLevel[i]);
	}

	return TRUE;
}

// --
//
// --
int	KLevelAdd::GetLevelExp(BYTE btLevel)
{	
	if (btLevel < 1 || btLevel > MAX_LEVEL)
		return 0;

	return m_nLevelExp[btLevel - 1];;
}

// --
//	
// --
int		KLevelAdd::GetLifePerLevel(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLifePerLevel[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetStaminaPerLevel(int nSeries, BOOL bSex)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if(!bSex)
	{
		return m_nStaminaMalePerLevel[nSeries];
	}
	else
	{
		return m_nStaminaFemalePerLevel[nSeries];
	}
}

// --
//	
// --
int		KLevelAdd::GetManaPerLevel(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nManaPerLevel[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetLifePerVitality(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLifePerVitality[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetStaminaPerVitality(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nStaminaPerVitality[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetManaPerEnergy(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nManaPerEnergy[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetLeadExpShare(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLeadExpShare[nSeries];
}

// --
//	
// --
int		KLevelAdd::GetFireResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel > MAX_LEVEL)
		return 0;
	return (m_nFireResistPerLevel[nSeries] * nLevel / 100);
}

// --
//	
// --
int		KLevelAdd::GetColdResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel > MAX_LEVEL)
		return 0;
	return (m_nColdResistPerLevel[nSeries] * nLevel / 100);
}

// --
//	
// --
int		KLevelAdd::GetPoisonResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel > MAX_LEVEL)
		return 0;
	return (m_nPoisonResistPerLevel[nSeries] * nLevel / 100);
}

// --
//	
// --
int		KLevelAdd::GetLightResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel > MAX_LEVEL)
		return 0;
	return (m_nLightResistPerLevel[nSeries] * nLevel / 100);
}

// --
//	
// --
int		KLevelAdd::GetPhysicsResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel > MAX_LEVEL)
		return 0;
	return (m_nPhysicsResistPerLevel[nSeries] * nLevel / 100);
}

// --
//	
// --
KTeamLeadExp::KTeamLeadExp()
{
	for (int i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		m_sPerLevel[i].m_dwExp = 0;
		m_sPerLevel[i].m_dwMemNum = 1;
	}
}

// --
//	
// --
BOOL	KTeamLeadExp::Init()
{
	int			i;
	KTabFile	LevelExp;

	if ( !LevelExp.Load(PLAYER_LEVEL_LEAD_EXP_FILE) )
		return FALSE;
	for (i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		LevelExp.GetInteger(i + 2, 2, 0, (int*)&m_sPerLevel[i].m_dwExp);
		LevelExp.GetInteger(i + 2, 3, 1, (int*)&m_sPerLevel[i].m_dwMemNum);
	}

	return TRUE;
}

// --
//	
// --
int		KTeamLeadExp::GetLevel(DWORD dwExp, int nCurLeadLevel)
{
	if (dwExp <= 0)
		return 1;
	if (nCurLeadLevel > 0)
	{
		if (nCurLeadLevel >= MAX_LEAD_LEVEL)
			return MAX_LEAD_LEVEL;
		if (dwExp < m_sPerLevel[nCurLeadLevel - 1].m_dwExp)
			return nCurLeadLevel;
		if (dwExp < m_sPerLevel[nCurLeadLevel].m_dwExp)
			return nCurLeadLevel + 1;
	}
	for (int i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		if (dwExp < m_sPerLevel[i].m_dwExp)
		{
			return i + 1;
		}
	}
	return MAX_LEAD_LEVEL;
}

// --
//	
// --
int		KTeamLeadExp::GetMemNumFromExp(DWORD dwExp)
{
	int nGetLevel;
	nGetLevel = GetLevel(dwExp);
	return m_sPerLevel[nGetLevel - 1].m_dwMemNum;
}

// --
//	
// --
int		KTeamLeadExp::GetMemNumFromLevel(int nLevel)
{
	if (1 <= nLevel && nLevel <= MAX_LEAD_LEVEL)
		return m_sPerLevel[nLevel - 1].m_dwMemNum;
	return 1;
}

// --
//	
// --
int		KTeamLeadExp::GetLevelExp(int nLevel)
{
	if (1 <= nLevel && nLevel <= MAX_LEAD_LEVEL)
		return this->m_sPerLevel[nLevel - 1].m_dwExp;
	return 0;
}

// --
//
// --
KNewPlayerAttribute::KNewPlayerAttribute()
{
	memset(m_nStrength, 0, sizeof(m_nStrength));
	memset(m_nDexterity, 0, sizeof(m_nDexterity));
	memset(m_nVitality, 0, sizeof(m_nVitality));
	memset(m_nEngergy, 0, sizeof(m_nEngergy));
	memset(m_nLucky, 0, sizeof(m_nLucky));
}

// --
//
// --
BOOL	KNewPlayerAttribute::Init()
{
	char		szSeries[5][16] = {"metal", "wood", "water", "fire", "earth"};
	KIniFile	AttributeFile;

	if ( !AttributeFile.Load(BASE_ATTRIBUTE_FILE_NAME) )
		return FALSE;

	for (int i = 0; i < series_num; i++)
	{
		AttributeFile.GetInteger(szSeries[i], "Strength", 0, &m_nStrength[i]);
		AttributeFile.GetInteger(szSeries[i], "Dexterity", 0, &m_nDexterity[i]);
		AttributeFile.GetInteger(szSeries[i], "Vitality", 0, &m_nVitality[i]);
		AttributeFile.GetInteger(szSeries[i], "Engergy", 0, &m_nEngergy[i]);
		AttributeFile.GetInteger(szSeries[i], "Lucky", PLAYER_FIRST_LUCKY, &m_nLucky[i]);
	}

	return TRUE;
}
