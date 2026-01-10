
#include "KCore.h"
#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif
#include "KSkills.h"
#include "KMissle.h"
#include "KMissleSet.h"
#include "KNpc.h"
#include "math.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KMath.h"
#include "KEngine.h"
#include "KTabFile.h"
#include "KTabFileCtrl.h"
#include "KMissleMagicAttribsData.h"
#include "KPlayer.h"
#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include "imgref.h"
#include "KMagicDesc.h"
#include "KOption.h"

#include "LuaFuns.h"

#endif

#define	 NPCINDEXOFOBJECT 0
const char * g_MagicID2String(int nAttrib);
extern  const KScript * g_GetScript(DWORD dwScriptId);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*!*****************************************************************************
// Function		: KSkill::KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::KSkill()
{
	m_nFlySkillId =  m_nCollideSkillId = m_nVanishedSkillId = 0;
	
    // add by FreewayChen in 2003.6.6
    m_nImmediateAttribsNum = m_nStateAttribsNum = m_nMissleAttribsNum = m_nDamageAttribsNum = m_nEnhanceSkillsNum = 0;
	m_nSkillCostType = attrib_mana;
    m_nWaitTime = 0;
	m_nEquiptLimited = 0;
	m_bDoHurt = 1; //fix by kinnox; // = 1
	//AutoAI by kinnox;
	m_bSkillLifeReplenish = FALSE;
	//end code;
#ifndef _SERVER
	m_szSkillDesc[0] = 0;
	m_szManPreCastSoundFile[0] = 0;
	m_szFMPreCastSoundFile[0] = 0;
	
	m_szSkillDescEx[0] = 0;
	m_bBuffEffect = 0;//AutoAI by kinnox;
#else
	m_dwSkillLevelUpScriptID = 0;
	m_dwSkillLevelDataScriptId = 0;
#endif	
}
/*!*****************************************************************************
// Function		: KSkill::~KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::~KSkill()
{
	
}

/*!*****************************************************************************
// Function		: KSkill::Param2PCoordinate
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nParam3
// Argumant		: int *npPX
// Argumant		: int *npPY
// Comments		:
// Author		: RomanDou
*****************************************************************************/
inline int	KSkill::Param2PCoordinate(int nLauncher, int nParam1, int nParam2 , int *npPX, int *npPY, eSkillLauncherType eLauncherType)  const 
{
	
	int nRegionId, nDesMapX, nDesMapY ;
	int nTargetId = -1;
	if (eLauncherType == SKILL_SLT_Obj) 
		return 0;
	
	switch(nParam1)
	{
	case -1:
		nTargetId		= nParam2;
		nRegionId		= Npc[nParam2].m_RegionIndex;
		nDesMapX		= Npc[nParam2].m_MapX;
		nDesMapY		= Npc[nParam2].m_MapY;
		
		if (eLauncherType == SKILL_SLT_Npc)
			SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(nRegionId, nDesMapX , nDesMapY, Npc[nParam2].m_OffX , Npc[nParam2].m_OffY, npPX, npPY);
		else if(eLauncherType == SKILL_SLT_Obj)
			SubWorld[Object[nLauncher].m_nSubWorldID].Map2Mps(nRegionId, nDesMapX, nDesMapY, Object[nParam2].m_nOffX , Object[nParam2].m_nOffY, npPX, npPY);
		else;
		break;
		
	case -2:
		
		break;
	default:
		*npPX = nParam1;
		*npPY = nParam2;
		break;
	}
	
	if (*npPX < 0 || *npPY < 0)	
	{
#ifdef _DEBUG
		//g_DebugLog("Param2PCoordinateº¯Êý»ñµÃ²ÎÊýÎ¥·¨£¡nParam1 ,nParam2 [%d,%d], nPX,nPY", nParam1, nParam2, *npPX, * npPY);
#endif
	}
	
	return nTargetId;
}

//Kenny Fix Skill
BOOL KSkill::CanCastSkill(int nLauncher, int &nParam1, int &nParam2)  const 
{
	if(m_bTargetSelf && nParam1 != -1) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
		return TRUE;
	}
	else
	{
		if (m_bTargetOnly && nParam1 != -1) 
			return FALSE;
		
		if (nParam1 == -1)
		{
			if ( nParam2 <= 0 || nParam2 >= MAX_NPC) return FALSE;
			NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
			
			if (m_bTargetEnemy)
			{
				if (Relation & relation_enemy) goto relationisvalid;
				//if (Npc[nParam2].m_Kind == kind_normal) goto relationisvalid;
				
			}
			
			if (m_bTargetAlly)
			{
				if (Relation & relation_ally) goto relationisvalid;
			}
			
			if (m_bTargetSelf)
			{
				if (Relation & relation_self) goto relationisvalid;
			}
			return FALSE;
		}
		
	}
	
relationisvalid:
	if (Npc[nLauncher].IsPlayer())
	{	
		//
		if (GetSkillAttrib() < 100)
		{
			int nWeapoinSkill = Npc[nLauncher].GetCurActiveWeaponSkill();
			if ((DWORD)nWeapoinSkill != m_nId)
			{
				return FALSE;
			}
		}
		

		if (-2 != m_nEquiptLimited)
		{

#ifdef _SERVER
			int nPlayerIdx		= Npc[nLauncher].GetPlayerIdx();
#else
			int nPlayerIdx		= CLIENT_PLAYER_INDEX;
#endif
			int nDetailType		= Player[nPlayerIdx].m_ItemList.GetWeaponType();
			int nParticularType = Player[nPlayerIdx].m_ItemList.GetWeaponParticular();
			
			if (nDetailType == 0)
			{	
				
				if(nParticularType > 5)
					nParticularType = -1;
				
			}
			else if (nDetailType == 1)
			{
				nParticularType += MAX_MELEEWEAPON_PARTICULARTYPE_NUM;
			}
			else if (nDetailType == -1)
			{
				nParticularType = -1;
			}
			
			if (nParticularType != m_nEquiptLimited)
				return FALSE;
		}
		
		if (m_nHorseLimited)
		{
			switch(m_nHorseLimited)
			{
			case 1:
				{
					if (Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			case 2:
				{
					if (!Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			default:
				return FALSE;
			}
		}
		
		if((m_bTargetOnly) && nParam1 == -1)
		{
			//
			int nDistance = NpcSet.GetDistance(nLauncher, nParam2);
			if(nDistance > GetAttackRadius()) 
				return FALSE;
			
		}
		/*else
		{
		if (nParam1 < 0 || nParam2 < 0) return FALSE;
		
		  #ifndef _SERVER
		  int nLauncherPX = 0, nLauncherPY = 0;
		  Npc[nLauncher].GetMpsPos(&nLauncherPX, &nLauncherPY);
		  int ndistance = g_GetDistance(nLauncherPX, nLauncherPY, nParam1, nParam2);
		  if (ndistance > GetAttackRadius()) return FALSE;
		  #endif
		  }
		*/
	}
	return TRUE;
}


//

BOOL KSkill::Cast(int nLauncher, int nParam1, int nParam2, int nWaitTime, eSkillLauncherType eLauncherType, BOOL bRemove /*= FALSE*/)  const 
{
	if(nLauncher < 0)
	{
		return FALSE; 
	}

	switch(eLauncherType)
	{
	case SKILL_SLT_Npc:
		{
			if(nLauncher >= MAX_NPC) 
				return FALSE;
			if(Npc[nLauncher].m_dwID < 0) 
				return FALSE;
					
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;

				if ((Npc[nParam2].m_Index <= 0) || 
						Npc[nLauncher].m_SubWorldIndex != Npc[nParam2].m_SubWorldIndex)
					return FALSE;
			}
		}
		break;
	case SKILL_SLT_Obj:
		{
			return FALSE;
			if (MAX_OBJECT <= nLauncher) 
				return FALSE;
			if (Object[nLauncher].m_nDataID < 0) 
				return FALSE;
		}
		break;
	case SKILL_SLT_Missle:
		{
			if (nLauncher >= MAX_MISSLE) 
				return FALSE;
			
			if (Missle[nLauncher].m_nMissleId < 0) 
				return FALSE;
			
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;
				
				if ((Npc[nParam2].m_Index <= 0) ||  Missle[nLauncher].m_nSubWorldId != Npc[nParam2].m_SubWorldIndex)
					return FALSE;
			}

		}
		break;
	default:
		return FALSE;
	}

	if(nParam1 < 0 && nParam2 < 0 ) 
		return FALSE;

	if(nWaitTime < 0 ) 
	{
		nWaitTime = 0;
	}
	
	switch(m_eSkillStyle)
	{
		
	case SKILL_SS_Missles:
		{
			CastMissles(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
		}
		break;	
	case SKILL_SS_Melee:
		{
			
		}
		break;	
	case SKILL_SS_InitiativeNpcState:
		{
			CastInitiativeSkill(nLauncher, nParam1, nParam2, nWaitTime);
		}
		break;
		
	case SKILL_SS_PassivityNpcState:	
		{
			CastPassivitySkill(nLauncher, nParam1, nParam2, nWaitTime, bRemove);		//
		}
		break;
		
	case SKILL_SS_CreateNpc:	 
		{
			
		}
		break;
		
	case SKILL_SS_BuildPoison:			
		{
			
		}
		break;
		
	case SKILL_SS_AddPoison:			
		{
			
		}
		break;
		
	case SKILL_SS_GetObjDirectly:			
		{
			
		}
		break;
		
	case SKILL_SS_StrideObstacle:		
		{
			
		}
		break;
		
	case SKILL_SS_BodyToObject:		
		{
			
		}
		break;
		
	case SKILL_SS_Mining:				
		{
			
		}
		break;
		
	case SKILL_SS_RepairWeapon:		
		{
			
		}
		break;
		
	case SKILL_SS_Capture:				 
		{
			
		}
		break;
	}
	
	if(m_bStartEvent && m_nStartSkillId > 0 && m_nEventSkillLevel > 0)
	{
		KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_nStartSkillId, m_nEventSkillLevel);
		if (!pOrdinSkill) 
            return FALSE;
		
        pOrdinSkill->Cast(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
	}
	
	return TRUE;	  
}

/*!*****************************************************************************
// Function		: KSkill::Vanish
// Purpose		: ×Óµ¯ÉúÃü½áÊøÊ±»Øµ÷
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Vanish(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_VanishEvent, pMissle);
}

BOOL KSkill::OnMissleEvent(unsigned short usEvent, KMissle * pMissle)  const 
{
	if (!pMissle) 
        return FALSE;

	int nLauncherIdx = pMissle->m_nLauncher;
	
    if (
		pMissle->m_nMissleId <= 0 
		|| pMissle->m_nMissleId >= MAX_MISSLE 
		|| nLauncherIdx <= 0
		|| nLauncherIdx >= MAX_NPC
		|| Npc[nLauncherIdx].m_Index <= 0
		)
        return FALSE;

	
	if (
		(!Npc[nLauncherIdx].IsMatch(pMissle->m_dwLauncherId)) 
		|| Npc[nLauncherIdx].m_SubWorldIndex != pMissle->m_nSubWorldId
		|| Npc[nLauncherIdx].m_RegionIndex < 0
		)
	{
		return FALSE;
	}
	
	int nEventSkillId = 0;
	int nEventSkillLevel = 0;
	switch(usEvent)
	{
	case Missle_FlyEvent:
		if (!m_bFlyingEvent || m_nFlySkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nFlySkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_StartEvent:
		if (!m_bStartEvent || m_nStartSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nStartSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_VanishEvent:
		if (!m_bVanishedEvent || m_nVanishedSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nVanishedSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_CollideEvent:
		if (!m_bCollideEvent || m_nCollideSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nCollideSkillId;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
	default:
		return FALSE;
	}
		
	int nDesPX = 0, nDesPY = 0;
	
	if (m_bByMissle)
	{
		pMissle->GetMpsPos(&nDesPX, &nDesPY);
	}
	else
	{
		Npc[nLauncherIdx].GetMpsPos(&nDesPX, &nDesPY);
	}
	
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nEventSkillId, nEventSkillLevel + m_btShowEvent);		//
	if (!pOrdinSkill) 
        return FALSE;
	
	BOOL bRetCode = FALSE;
	
    if (m_bByMissle)    //When Event
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{
			bRetCode = pOrdinSkill->CastMissles(pMissle->m_nMissleId, nDesPX, nDesPY, 0, SKILL_SLT_Missle);
		}
	}
	else
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{   
            bRetCode = pOrdinSkill->CastMissles(nLauncherIdx, nDesPX, nDesPY, 0, SKILL_SLT_Npc);
		}
	}
	
	return bRetCode;
}

/*!*****************************************************************************
// Function		: KSkill::FlyEvent
// Purpose		: 
// Return		: void 
// Argumant		: int nMissleId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KSkill::FlyEvent(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_FlyEvent, pMissle);
}

/*!*****************************************************************************
// Function		: KSkill::Collidsion
// Purpose		: ×Óµ¯±»×²Ê±»Øµ÷
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Collidsion(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_CollideEvent, pMissle);
}


//

BOOL	KSkill::CastMissles(int nLauncher, int nParam1, int nParam2, int nWaitTime  , eSkillLauncherType eLauncherType )  const 
{
	int nRegionId		=	0;
	int	nDesMapX		=	0;
	int nDesMapY		=	0;
	int nDesOffX		=	0;
	int nDesOffY		=	0;
	int nSrcOffX		=	0;
	int nSrcOffY		=	0;
	int nSrcPX			=	0;
	int nSrcPY			=	0;
	int nDesPX			=	0;
	int nDesPY			=	0;
	int nDistance		=	0;
	int nDir			=	0;
	int nDirIndex		=	0;
	int nTargetId		=	-1;
	int nRefPX			=	0;
	int nRefPY			=	0;
	TOrdinSkillParam	SkillParam ;
	SkillParam.eLauncherType = SKILL_SLT_Npc;
	SkillParam.nParent = 0;
	SkillParam.eParentType = (eSkillLauncherType)0;
	SkillParam.nWaitTime = nWaitTime;
	SkillParam.nTargetId = 0;
	if (nLauncher <= 0) 
		return FALSE;
	
	switch(m_eMisslesForm)
	{
	case SKILL_MF_Wall:				
		{
			if (nParam1 == SKILL_SPT_Direction) 
				return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{	
					nTargetId = Param2PCoordinate(nLauncher, nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);
					if (Npc[nLauncher].m_SubWorldIndex < 0) 
					{
						return FALSE;
					}
					
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					nDir = nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;		
					CastWall(&SkillParam , nDir, nDesPX, nDesPY, nSrcPX, nSrcPY);		//
				}	
				break;
			case SKILL_SLT_Obj:
				{
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					
					SubWorld[Missle[nLauncher].m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					int nDir = pMissle->m_nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.nParent = SKILL_SLT_Missle;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastWall(&SkillParam,  nDir, nRefPX, nRefPY, nRefPX, nRefPY);		//
				}
				break;
			}
		}break;
		
		
	case SKILL_MF_Line:
		{
			if(nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{	
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) 
							return FALSE;

						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						CastLine(&SkillParam, nDir, nSrcPX, nSrcPY, nSrcPX, nSrcPY);		//
						
					}
					break;
				case SKILL_SLT_Obj:
					{
					}
					break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastWall(&SkillParam, nDir,  nRefPX, nRefPY, nRefPX, nRefPY);		//
					}
					break;
				}
				
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line || g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Parabola) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		
								return FALSE;

							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							
							//
							if(nDistance == 0) 
							{
								nDistance = 1;
							}
							
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;
							int		nCos = (nXLength << 10) / nDistance;
							
							//
							if (abs(nSin) > 1024) 
							{
								nSin = 0;
								nCos = 0;
							}

							if (abs(nCos) > 1024) 
							{
								nSin = 0;
								nCos = 0;
							}
							
							
							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
						{
							CastLine(&SkillParam, nDir, nSrcPX, nSrcPY, nDesPX, nDesPY);		//
						}
					}
					break;
				case SKILL_SLT_Obj:
					{
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) 
							return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastLine(&SkillParam,  pMissle->m_nDir,  nRefPX, nRefPY, nRefPX, nRefPY);		//
					}
					break;
				}
			}
		}
		break;
	case	SKILL_MF_Spread:					
		{
			
			if (nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						CastSpread(&SkillParam, nDir, nSrcPX,nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastSpread(&SkillParam, nDir,  nRefPX, nRefPY);
					}break;
				}
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY, SKILL_SLT_Npc);		
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		
								return FALSE ;

							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							//
							if(nDistance == 0) 
							{
								nDistance = 1;
							}
							
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;
							int		nCos = (nXLength << 10) / nDistance;

							//
							if (abs(nSin) > 1024) 
							{
								nSin = 0;
								nCos = 0;
							}

							if (abs(nCos) > 1024) 
							{	
								nSin = 0;
								nCos = 0;
							}
							

							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
						{
							CastSpread(&SkillParam, nDir, nSrcPX, nSrcPY);
						}
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if(!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) 
							return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastSpread(&SkillParam ,pMissle->m_nDir,  nRefPX, nRefPY);
					}
					break;
				}
			}
			
		}break;
		
		
		//ÒÔµ±Ç°µãÎªÔ²µã²úÉú¶à¸öÎ§ÈÅµÄ×Óµ¯
		//·Ö³ÉÁ½ÖÖÇé¿ö£¬Ò»ÖÖÎªÒÔÔ­µØÎªÔ­ÐÄ·¢³ö£¬ÁíÒ»ÖÖÎªÒÔÄ¿±êµãÎªÔ­ÐÄ·¢³ö
		// Êý×Ö²ÎÊýÒ»±íÊ¾ ÊÇ·ñÎªÔ­µØ·¢³ö
		
	case	SKILL_MF_Circle:				//Ô²ÐÎ	¶à¸ö×Óµ¯Î§³ÉÒ»¸öÈ¦
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY, eLauncherType);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					
					if (m_nValue1 == 0)
						CastCircle(&SkillParam, nDir, nSrcPX, nSrcPY);
					else
						CastCircle(&SkillParam, nDir, nDesPX, nDesPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastCircle(&SkillParam, pMissle->m_nDir,  nRefPX, nRefPY);
				}break;
			}
			
		}break;
		
	case	SKILL_MF_Random:				//Ëæ»ú	¶à¸ö×Óµ¯Ëæ»úÅÅ·Å
		{
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					
				}break;
			}
		}
		break;
		
	case SKILL_MF_AtTarget:
		{
			if (nParam1 == SKILL_SPT_Direction) 
				return FALSE;	
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam, nDir, nDesPX, nDesPY, nSrcPX, nSrcPY);		//
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) 
						return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY, nRefPX, nRefPY);		//
				}
				break;
			}
		}break;
		
	case SKILL_MF_AtFirer:
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam,  nDir, nSrcPX, nSrcPY, nDesPX, nDesPY);		//
				}
				break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam , pMissle->m_nDir, nRefPX, nRefPY, nRefPX, nRefPY);		//
				}
				break;
			}
			
		}break;
		
	case	SKILL_MF_Zone:
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam, nDir, nSrcPX, nSrcPY, nDesPX, nDesPY);		//
				}
				break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY, nRefPX, nRefPY);		//
				}
				break;
			}
		}break;
	}
	return TRUE;
}

// --
//
// --
int KSkill::CastZone(TOrdinSkillParam * pSkillParam , int nDir, int nRefPX, int nRefPY, int nDestX, int nDestY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) 
		return 0;
	int nCastMissleNum	= 0;
	int nBeginPX ;
	int nBeginPY ;
	if (m_nChildSkillNum == 1)
	{
		nBeginPX = nRefPX;
		nBeginPY = nRefPY;
	}
	else 
	{
		nBeginPX		= nRefPX - m_nChildSkillNum * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth / 2;
		nBeginPY		= nRefPY - m_nChildSkillNum * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight / 2;
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	for (int i = 0; i < m_nChildSkillNum; i ++)
		for (int j = 0; j < m_nChildSkillNum; j ++)
		{
			if (m_bBaseSkill)
			{
				int nMissleIndex ;
				int nSubWorldId ; 
				
				nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
				
				if (m_nValue1 == 32)
					if ( ((i - m_nChildSkillNum / 2) * (i - m_nChildSkillNum / 2) + (j - m_nChildSkillNum / 2) * (j - m_nChildSkillNum / 2)) > (m_nChildSkillNum * m_nChildSkillNum / 4))			continue;
					
					
					if (nSubWorldId < 0)	goto exit;
					int nDesSubX = nBeginPX + j * SubWorld[nSubWorldId].m_nCellWidth;
					int nDesSubY = nBeginPY +  i * SubWorld[nSubWorldId].m_nCellHeight;
					nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX , nDesSubY);
					
					if (nMissleIndex < 0)	continue;
					
					Missle[nMissleIndex].m_nDir				= nDir;
					Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
					CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
					Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
					if (pSkillParam->nTargetId > 0)
						Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
					Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
					Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
					Missle[nMissleIndex].m_nLauncher		= nLauncher;
					Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
					
					if (pSkillParam->nParent)
						Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
					else 
						Missle[nMissleIndex].m_nParentMissleIndex = 0;
					
					Missle[nMissleIndex].m_nSkillId			= m_nId;
					Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j);
					Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
					Missle[nMissleIndex].m_nRefPX			= nDesSubX;
					Missle[nMissleIndex].m_nRefPY			= nDesSubY;
					
					if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack || 
						Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
					{
						int		nSin = 0;
						int		nCos = 0;
						int		nDistance = 0;
						if (nDestX != nRefPX && nDestY != nRefPY)
						{	
							nDistance = g_GetDistance(nRefPX, nRefPY, nDestX, nDestY);
							if(nDistance == 0) 
							{
								nDistance = 1;
							}
							int		nYLength = 0;
							int		nXLength = 0;
							nYLength = nDestY - nRefPY;
							nXLength = nDestX - nRefPX;
								
							nSin = (nYLength << 10) / nDistance;
							nCos = (nXLength << 10) / nDistance;
							if (abs(nSin) > 1024)
							{
								nSin = 0;
								nCos = 0;		
							}
							if (abs(nCos) > 1024)
							{
								nSin = 0;
								nCos = 0;
							}
						}
						if (nSin != 0 && nCos != 0)
						{
							Missle[nMissleIndex].m_nXFactor = nCos;
							Missle[nMissleIndex].m_nYFactor = nSin;
						}
						else
						{
							Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
							Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
						}
					}
#ifdef _SERVER
					Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
					nCastMissleNum ++;
			}
			else
			{
				_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
				if (pOrdinSkill) 
				{
					if (!pSkillParam->nParent)
						nCastMissleNum += pOrdinSkill->Cast(nLauncher, nBeginPX + j * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth , nBeginPY +  i * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), eLauncherType);
					else 
						nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nLauncher, nBeginPX + j * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth , nBeginPY +  i * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), pSkillParam->eLauncherType);
				}
			}
			
		}
exit:	
#ifdef _SERVER
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
			{
				delete pNewMagicAttribsData;
				pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
			}
#endif
			return nCastMissleNum;
}

// --
//
// -- 
int KSkill::CastLine(TOrdinSkillParam *pSkillParam, int nDir, int nRefPX, int nRefPY, int nDestX, int nDestY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	int nMSDistanceEach = m_nValue1;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	int nNum = 0;
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nRefPX + ((nMSDistanceEach * (i + 1) * g_DirCos(nDirIndex, MaxMissleDir) )>>10);
		nDesSubY	= nRefPY + ((nMSDistanceEach * (i + 1) * g_DirSin(nDirIndex, MaxMissleDir) )>>10);
	
		if(nDesSubX < 0 || nDesSubY < 0) 	
			continue;
		
		if(m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;

			if (nSubWorldId < 0 || nSubWorldId >= MAX_SUBWORLD)	
				goto exit;

			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0 || nMissleIndex >= MAX_MISSLE)	
				continue;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			if (pSkillParam->nTargetId > 0)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack  || 
				Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow)
			{
				int		nSin = 0;
				int		nCos = 0;
				int		nDistance = 0;
				if (nDestX != nRefPX && nDestY != nRefPY)
				{	
					nDistance = g_GetDistance(nRefPX, nRefPY, nDestX, nDestY);
					//
					if(nDistance == 0) 
					{
						nDistance = 1;
					}
					
					int	nYLength = nDestY - nRefPY;
					int	nXLength = nDestX - nRefPX;
					nSin = (nYLength << 10) / nDistance;
					nCos = (nXLength << 10) / nDistance;
					if (abs(nSin) > 1024)
					{
						nSin = 0;
						nCos = 0;		
					}
					if (abs(nCos) > 1024)
					{
						nSin = 0;
						nCos = 0;
					}
				}
				if (nSin != 0 && nCos != 0)
				{
					Missle[nMissleIndex].m_nXFactor = nCos;
					Missle[nMissleIndex].m_nYFactor = nSin;
				}
				else
				{
					Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
					Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
				}
			}			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
				
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
		{
			delete pNewMagicAttribsData;
			pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
		}
#endif
		return nCastMissleNum;
}

int KSkill::CastExtractiveLineMissle(TOrdinSkillParam* pSkillParam,  int nDir,int nSrcX, int nSrcY, int nXOffset, int nYOffset, int nDesX, int nDesY)  const 
{
	_ASSERT(pSkillParam);
	
	int nLauncher = pSkillParam->nLauncher;
	if (pSkillParam->eLauncherType != SKILL_SLT_Npc) 
		return 0;	
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
		
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER

	if (m_bBaseSkill)
	{
		int nMissleIndex ;
		int nSubWorldId ; 
		
		nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
		if (nSubWorldId < 0 || nSubWorldId >= MAX_SUBWORLD)	
			goto exit;

		nMissleIndex = MissleSet.Add(nSubWorldId, nSrcX, nSrcY);
		if (nMissleIndex < 0 || nMissleIndex >= MAX_MISSLE)	
			goto exit;
		
		Missle[nMissleIndex].m_nDir				= nDir;
		Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
		CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
		
		if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
		{
			int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
			int nTime = nLength / Missle[nMissleIndex].m_nSpeed;
			Missle[nMissleIndex].m_nHeightSpeed	= Missle[nMissleIndex].m_nZAcceleration * (nTime - 1) / 2;
			
		}
		
		Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
		if (pSkillParam->nTargetId > 0)
			Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
		Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
		Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
		Missle[nMissleIndex].m_nLauncher		= nLauncher;
		Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
	
		if (pSkillParam->nParent)
			Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
		else 
			Missle[nMissleIndex].m_nParentMissleIndex = 0;
		
		Missle[nMissleIndex].m_nSkillId			= m_nId;
		Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(0);
		Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
		Missle[nMissleIndex].m_nRefPX			= nSrcX;
		Missle[nMissleIndex].m_nRefPY			= nSrcY;
		Missle[nMissleIndex].m_bNeedReclaim		= TRUE;

		int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
		Missle[nMissleIndex].m_nFirstReclaimTime	= nLength / Missle[nMissleIndex].m_nSpeed + Missle[nMissleIndex].m_nStartLifeTime;
		Missle[nMissleIndex].m_nEndReclaimTime		= Missle[nMissleIndex].m_nFirstReclaimTime + SubWorld[nSubWorldId].m_nCellWidth / Missle[nMissleIndex].m_nSpeed + 2;
		
		if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
		{	
			//
			if (nXOffset != 0 && nYOffset != 0)
			{
				Missle[nMissleIndex].m_nXFactor = nXOffset;
				Missle[nMissleIndex].m_nYFactor = nYOffset;
			}
			else
			{
			
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
			}
			
		}
		
		
#ifdef _SERVER
		Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
		
		nCastMissleNum ++;
	}
	else
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
		if (pOrdinSkill) 
		{
			if (!pSkillParam->nParent)
				nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eLauncherType);
			else
				nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eParentType);
			
		}
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
		{
			delete pNewMagicAttribsData;
			pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
		}
#endif
		
		return nCastMissleNum;
		
}

// --

// --
int KSkill::CastWall(TOrdinSkillParam * pSkillParam,  int nDir , int nRefPX , int nRefPY, int nSrcX, int nSrcY)  const  
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;

	int nMSDistanceEach = m_nValue1;
	int nCurMSDistance	= -1 * nMSDistanceEach * m_nChildSkillNum / 2;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif
	
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		if(m_nValue2)
		{
			nDesSubX	= nSrcX + ((nCurMSDistance * g_DirCos(nDirIndex, MaxMissleDir)) >>10);
			nDesSubY	= nSrcY + ((nCurMSDistance * g_DirSin(nDirIndex, MaxMissleDir)) >>10);
		}
		else
		{
			nDesSubX	= nRefPX + ((nCurMSDistance * g_DirCos(nDirIndex, MaxMissleDir)) >>10);
			nDesSubY	= nRefPY + ((nCurMSDistance * g_DirSin(nDirIndex, MaxMissleDir)) >>10);
		}
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	
			{
				goto exit;
			}
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0)	
			{
				continue;
			}

			if (m_nValue2)
			{
				int nDirTemp = nDir - MaxMissleDir / 4;
				if (nDirTemp < 0) nDirTemp += MaxMissleDir;
				Missle[nMissleIndex].m_nDir				= nDirTemp;
				Missle[nMissleIndex].m_nDirIndex = g_Dir2DirIndex(nDirTemp, 64);

			}
			else
			{
				Missle[nMissleIndex].m_nDir				= nDir;
				Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			}
			
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			
			if (pSkillParam->nTargetId)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
#ifdef _DEBUG			
			//g_DebugLog("[Target] %d", Missle[nMissleIndex].m_dwFollowNpcID);
#endif

			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack ||
				Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				int		nSin = 0;
				int		nCos = 0;
				int		nDistance = 0;
				if (nSrcX != nRefPX && nSrcY != nRefPY)
				{	
					nDistance = g_GetDistance(nSrcX, nSrcY, nRefPX, nRefPY);
					if(nDistance == 0) 
					{
						nDistance = 1;
					}
					int		nYLength = nRefPY - nSrcY;
					int		nXLength = nRefPX - nSrcX;
					nSin = (nYLength << 10) / nDistance;
					nCos = (nXLength << 10) / nDistance;
					if (abs(nSin) > 1024)
					{
						nSin = 0;
						nCos = 0;		
					}
					if (abs(nCos) > 1024)
					{
						nSin = 0;
						nCos = 0;
					}
				}
				if (nSin != 0 && nCos != 0)
				{
					Missle[nMissleIndex].m_nXFactor = nCos;
					Missle[nMissleIndex].m_nYFactor = nSin;
				}
				else
				{
					Missle[nMissleIndex].m_nXFactor = g_DirCos(Missle[nMissleIndex].m_nDir, MaxMissleDir);
					Missle[nMissleIndex].m_nYFactor = g_DirSin(Missle[nMissleIndex].m_nDir, MaxMissleDir);
				}
			}
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime +  GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
		nCurMSDistance += nMSDistanceEach;
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
		{
			delete pNewMagicAttribsData;
			pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
		}
#endif
		return nCastMissleNum;
}

// --

// --
int		KSkill::CastCircle(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType  eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;	
	int nDesSubPX	= 0;
	int nDesSubPY	= 0;
	int nFirstStep	= m_nValue2;
	int nCurSubDir	= 0;
	int nDirPerNum  = 	MaxMissleDir / m_nChildSkillNum  ;
	int nCastMissleNum = 0;
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		int nCurSubDir	= nDir + nDirPerNum * i ;
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB	= g_DirSin(nCurSubDir, MaxMissleDir);
		int nCosAB	= g_DirCos(nCurSubDir, MaxMissleDir);
		
		nDesSubPX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubPY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		
		
		if (nDesSubPX < 0 || nDesSubPY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubPX, nDesSubPY);
			
			if (nMissleIndex < 0)	
			{
				continue;
			}
			
			Missle[nMissleIndex].m_nDir			= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex	= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			if (pSkillParam->nTargetId > 0)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubPX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubPY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack || 
				Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Follow || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nCurSubDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nCurSubDir, MaxMissleDir);
			}			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
			
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
		{	
			delete pNewMagicAttribsData;
			pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
		}
#endif
		
		return nCastMissleNum;
}

// --

// --
int		KSkill::CastSpread(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nDesSubMapX		= 0;
	int nDesSubMapY		= 0;
	int nFirstStep		= m_nValue2;
	int nCurMSRadius	= m_nChildSkillNum / 2 ; 
	int nCurSubDir		= 0;
	int	nCastMissleNum  = 0;
	int nTargetId		= -1;
		
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif
	
	int nDesSubX = 0;
	int nDesSubY = 0;
	int nXFactor = 0;
	int nYFactor = 0;
	nTargetId = pSkillParam->nTargetId;

	if(nTargetId > 0)
	{
		int nDistance = 0;
		int nDesX, nDesY;
		if (Npc[nTargetId].m_Index > 0 && Npc[nTargetId].m_SubWorldIndex >= 0) 
			SubWorld[Npc[nTargetId].m_SubWorldIndex].Map2Mps(Npc[nTargetId].m_RegionIndex, Npc[nTargetId].m_MapX, Npc[nTargetId].m_MapY, Npc[nTargetId].m_OffX, Npc[nTargetId].m_OffY, &nDesX, &nDesY);
		
		
		nDistance = g_GetDistance(nRefPX, nRefPY, nDesX, nDesY);
		if(nDistance == 0)
			nDistance = 1;

		int nSin = 0;
		int nCos = 0;
		int	nYLength = nDesY - nRefPY;
		int	nXLength = nDesX - nRefPX;
		nSin = (nYLength << 10) / nDistance;
		nCos = (nXLength << 10) / nDistance;

		if (abs(nSin) > 1024)
		{
			nSin = 0;
			nCos = 0;		
		}
		if (abs(nCos) > 1024)
		{
			nSin = 0;
			nCos = 0;
		}
		
		if(nSin != 0 && nCos != 0)
		{
			nXFactor = nCos;
			nYFactor = nSin;
		}
		else
		{
			nXFactor = ((nDesX - nRefPX) << 10) / nDistance;
			nYFactor = ((nDesY - nRefPY) << 10) / nDistance;
		}
		
		
		nDesSubX = nRefPX + ((nXFactor * nFirstStep) >> 10);
		nDesSubY = nRefPY + ((nYFactor * nFirstStep) >> 10);
		
		if (nDesSubX < 0  || nDesSubY < 0 ) 
			return 0;
	}
	
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		int nDSubDir	= m_nValue1 * nCurMSRadius; 
		nCurSubDir		= nDir - m_nValue1 * nCurMSRadius;
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB;
		int nCosAB;
		
		if (nTargetId > 0)
		{
			nDSubDir	+= 48;
			if (nDSubDir >= MaxMissleDir)
				nDSubDir -= MaxMissleDir;

			nSinAB = (nYFactor * g_DirCos(nDSubDir, MaxMissleDir) - nXFactor * g_DirSin(nDSubDir, MaxMissleDir)) >> 10;
			nCosAB = (nXFactor * g_DirCos(nDSubDir, MaxMissleDir) + nYFactor * g_DirSin(nDSubDir , MaxMissleDir)) >> 10;
		}
		else
		{
			nSinAB = g_DirSin(nCurSubDir, MaxMissleDir);
			nCosAB = g_DirCos(nCurSubDir, MaxMissleDir);
		}
		
		nDesSubX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	
			continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nXFactor			= nCosAB;
			Missle[nMissleIndex].m_nYFactor			= nSinAB;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum +=  pOrdinSkill->Cast(nLauncher,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum +=  pOrdinSkill->Cast(pSkillParam->nParent,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType); 
			}
		}
		
		nCurMSRadius-- ;
	}
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
		{
			delete pNewMagicAttribsData;
			pNewMagicAttribsData = NULL; //them code 16/09/2023 by kinnox;
		}
#endif
		
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::GetChildSkillNum
// Purpose		: ÓÉÓÚ¿ÉÄÜÄ³Ð©¼¼ÄÜÖÐ£¬Ëæ×Å¼¼ÄÜµÈ¼¶µÄÉý¼¶£¬×Óµ¯µÄÊýÄ¿Ò²»áÒò´ËÔö¼Ó£¬ËùÒÔÍ¨¹ý¸Ãº¯Êý»ñµÃÊµ¼ÊµÄ×Ó¼¼ÄÜÊýÄ¿
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: RomanDou
*****************************************************************************/
int 	KSkill::GetChildSkillNum(int nLevel)  const 
{
	return m_nChildSkillNum;
};
/*!*****************************************************************************
// Function		: KSkill::CreateMissle
// Purpose		: ÉèÖÃ×Óµ¯µÄ»ù±¾Êý¾Ý£¬ÒÔ¼°¸Ã¼¼ÄÜ¸ÃµÈ¼¶ÏÂµÄ¶Ô×Óµ¯ÐÅÏ¢µÄ±ä¶¯Êý¾Ý
//					ÉèÖÃÓÃÓÚÊýÖµ¼ÆËãµÄÖ¸Õë
// Return		: 
// Argumant		: int nChildSkillId
// Argumant		: int nMissleIndex
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::CreateMissle(int nLauncher, int nChildSkillId, int nMissleIndex)  const 
{
	_ASSERT(nChildSkillId > 0 && nChildSkillId < MAX_MISSLESTYLE && nMissleIndex > 0);
	
	if (nLauncher <= 0) 
	{
		return ;
	}
	
	KMissle * pMissle = &Missle[nMissleIndex];
	
	g_MisslesLib[nChildSkillId] = *pMissle;//¸´ÖÆ¿½±´¶ÔÏó
	
	pMissle->m_nLevel			= m_ulLevel;
	pMissle->m_bCollideEvent	= m_bCollideEvent;
	pMissle->m_bVanishedEvent   = m_bVanishedEvent;
	pMissle->m_bStartEvent		= m_bStartEvent;
	pMissle->m_bFlyEvent		= m_bFlyingEvent;
	pMissle->m_nFlyEventTime	= m_nFlyEventTime;
	pMissle->m_nMissleId		= nMissleIndex;
	pMissle->m_bClientSend      = m_bClientSend;
	pMissle->m_bMustBeHit		= m_bMustBeHit;
	pMissle->m_bIsMelee			= m_bIsMelee;
	pMissle->m_bByMissle		= m_bByMissle;
	pMissle->m_bTargetSelf		= (m_bTargetSelf == 1);
	pMissle->m_nInteruptTypeWhenMove = m_nInteruptTypeWhenMove;
	pMissle->m_bHeelAtParent	= m_bHeelAtParent;
	pMissle->m_bUseAttackRating	= m_bUseAttackRate;
	pMissle->m_bDoHurt			= m_bDoHurt;
	
	//if (pMissle->m_nInteruptTypeWhenMove)
	{
		Npc[nLauncher].GetMpsPos(&pMissle->m_nLauncherSrcPX, &pMissle->m_nLauncherSrcPY);
	}
	pMissle->m_eRelation = m_eRelation;

	
#ifndef _SERVER
	pMissle->m_MissleRes.m_bNeedShadow   = m_bNeedShadow;
	pMissle->m_MissleRes.m_nMaxShadowNum = m_nMaxShadowNum;
	pMissle->m_MissleRes.m_nMissleId	 = nMissleIndex;

	if (!pMissle->m_MissleRes.Init()) 
	{
#ifdef _DEBUG
		//g_DebugLog("CAN NOT LOAD %s!!!", __FILE__);
#endif
	}
#else
	
	pMissle->m_btRandomParam	= (BYTE)g_Random(100);
	
#endif
	
	pMissle->DoWait();
	
	for (int i = 0  ; i < m_nMissleAttribsNum; i ++)
	{
		switch (m_MissleAttribs[i].nAttribType)
		{
		case magic_missle_movekind_v:
			{
				pMissle->m_eMoveKind	= (eMissleMoveKind) m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_speed_v:	
			{
				pMissle->m_nSpeed		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_lifetime_v:
			{
				pMissle->m_nLifeTime	= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_height_v:	
			{
				pMissle->m_nHeight		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_damagerange_v:
			{
				pMissle->m_nDamageRange = m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_radius_v:	
			{
			}break;
		
		case magic_missle_missrate:
			{
				pMissle->m_btMissRate	= m_MissleAttribs[i].nValue[0];
			}break;
		case magic_missle_hitcount:
			{
				pMissle->m_btHitCount	= m_MissleAttribs[i].nValue[0];
			}break;
		
		}
	}
	
	if (m_bIsMelee)
		pMissle->m_nLifeTime = Npc[nLauncher].ModifyMissleLifeTime(pMissle->m_nLifeTime);
	else
	{
		pMissle->m_nSpeed = Npc[nLauncher].ModifyMissleSpeed(pMissle->m_nSpeed);
		pMissle->m_bCollideVanish = Npc[nLauncher].ModifyMissleCollsion(pMissle->m_bCollideVanish);
	}
	
}


/*!*****************************************************************************
// Function		: KSkill::GetInfoFromTabFile
// Purpose		: ´ÓTabFileÖÐ»ñµÃ¸Ã¼¼ÄÜµÄ³£¹æÊôÐÔ
// Return		: 
// Argumant		: int nCol
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::GetInfoFromTabFile(int nRow)
{
	KITabFile * pITabFile = &g_OrdinSkillsSetting;
	return GetInfoFromTabFile(&g_OrdinSkillsSetting, nRow);
}


BOOL	KSkill::GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow)
{
	if (!pSkillsSettingFile || nRow < 0) 
		return FALSE;
	pSkillsSettingFile->GetString(nRow, "SkillName",		"", m_szName, sizeof(m_szName) ,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillId",			0, (int *)&m_nId,TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "Attrib",		0, (int*)&m_nSkillAttrib, TRUE);
	
	
	int nReqLevel = 0;
	pSkillsSettingFile->GetInteger(nRow, "ReqLevel",		0, (int *)&nReqLevel, TRUE);
	m_usReqLevel = (unsigned short)nReqLevel;


	pSkillsSettingFile->GetInteger(nRow, "EqtLimit",		-2, (int *)&m_nEquiptLimited, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HorseLimit",		0, (int *)&m_nHorseLimited, TRUE);

	pSkillsSettingFile->GetInteger(nRow, "DoHurt",			1, (int *)&m_bDoHurt);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillNum",	0, &m_nChildSkillNum,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MisslesForm",		0, (int *)&m_eMisslesForm, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharClass",		0, &m_nCharClass, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillStyle",		0, (int *)&m_eSkillStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharAnimId",		0, (int *)&m_nCharActionId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsPhysical",		0, &m_bIsPhysical, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsAura",			0, &m_bIsAura, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsUseAR",			0, &m_bUseAttackRate, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetOnly",		0, &m_bTargetOnly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetEnemy",		0, &m_bTargetEnemy, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetAlly",		0, &m_bTargetAlly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetObj",		0, &m_bTargetObj, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "BaseSkill",		0, &m_bBaseSkill, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ByMissle",		0, &m_bByMissle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillId",	0, &m_nChildSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEvent",		0, &m_bFlyingEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartEvent",		0, &m_bStartEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollideEvent",	0, &m_bCollideEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedEvent",	0, &m_bVanishedEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlySkillId",		0, &m_nFlySkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartSkillId",	0, &m_nStartSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedSkillId",	0, &m_nVanishedSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollidSkillId",	0, &m_nCollideSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillCostType",	0, (int *)&m_nSkillCostType, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CostValue",		0, &m_nCost, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TimePerCast",		0, &m_nMinTimePerCast, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TimePerCast",		0, &m_nMinTimePerCastOnHorse, TRUE);		
	pSkillsSettingFile->GetInteger(nRow, "Param1",			0, &m_nValue1, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param2",			0, &m_nValue2, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillLevel", 0, &m_nChildSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "EventSkillLevel", 0, &m_nEventSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsMelee",			0, &m_bIsMelee, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEventTime",	0, &m_nFlyEventTime, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerate",	0, (int *)&m_eMisslesGenerateStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerateData",0, &m_nMisslesGenerateData, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MaxShadowNum",	0, &m_nMaxShadowNum, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "AttackRadius",	50, &m_nAttackRadius, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "WaitTime",		0, &m_nWaitTime, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ClientSend",		0, &m_bClientSend, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetSelf",		0, &m_bTargetSelf, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StopWhenMove",	0, &m_nInteruptTypeWhenMove, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HeelAtParent",  0, (int *)&m_bHeelAtParent, TRUE );
	
	pSkillsSettingFile->GetInteger(nRow, "StateSpecialId",  0, &m_nStateSpecialId, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "StatePriority",  0, (int*)&m_btStatePriority, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ShowEvent", 0, (int*)&m_btShowEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsExpSkill", 0, (int*)&m_bIsExpSkill, TRUE);
	

	m_eRelation = 0;
	if (m_bTargetEnemy)
		m_eRelation |= relation_enemy;
	
	if (m_bTargetAlly)
		m_eRelation |= relation_ally;
	
	if (m_bTargetSelf)
		m_eRelation |= relation_self;
	
#ifndef _SERVER
	pSkillsSettingFile->GetString(nRow, "SkillDesc", "", m_szSkillDesc, 100);
	pSkillsSettingFile->GetInteger(nRow, "NeedShadow",		0, &m_bNeedShadow, TRUE);
	pSkillsSettingFile->GetString(nRow, "SkillIcon","\\spr\\skill\\Í¼±ê\\Í¨ÓÃ.spr",	m_szSkillIcon, 80);
	if (!m_szSkillIcon[0])	strcpy(m_szSkillIcon, "\\spr\\skill\\Í¼±ê\\Í¨ÓÃ.spr");
	pSkillsSettingFile->GetInteger(nRow, "LRSkill",		0, (int*)&m_eLRSkillInfo);
	pSkillsSettingFile->GetString(nRow, "PreCastSpr", "", m_szPreCastEffectFile, 100);
	pSkillsSettingFile->GetString(nRow, "ManCastSnd","", m_szManPreCastSoundFile, 100);
	pSkillsSettingFile->GetString(nRow, "FMCastSnd", "", m_szFMPreCastSoundFile, 100);
	pSkillsSettingFile->GetInteger(nRow, "BuffEffect",	0, &m_bBuffEffect, TRUE);//AutoAI by kinnox;
#else
	char szLevelScript[MAX_PATH];

	pSkillsSettingFile->GetString(nRow, "LvlSetScript", "", szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelDataScriptId = g_FileName2Id(szLevelScript);
	}
	
	pSkillsSettingFile->GetString(nRow, "LevelUpScript","",szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelUpScriptID = g_FileName2Id(szLevelScript);
	}
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::LoadSkillLevelData
// Purpose		: ¶Á±í»ñµÃµ±Ç°µÈ¼¶ÏÂµ±Ç°¼¼ÄÜµÄ¼¼ÄÜ¡¢×Óµ¯¡¢Åö×²ÊýÖµÓ°Ïì
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: Romandou
****************************************************************************/
void		KSkill::LoadSkillLevelData(unsigned long  nLevel /* =0*/, int nParam)
{
	m_nMissleAttribsNum = 0;
	m_nDamageAttribsNum = 0;
	m_nImmediateAttribsNum = 0;
	m_nStateAttribsNum	= 0;
	
	char szSettingNameValue[100];
	char szSettingDataValue[100];

	ZeroMemory(szSettingNameValue, sizeof(szSettingNameValue));
	ZeroMemory(szSettingDataValue, sizeof(szSettingDataValue));
	char szResult[1024];
	int nRowId = nParam;
	if (nRowId < 2) 
		return ;
	KLuaScript * pScript = NULL;
#ifndef _SERVER
	char szSettingScriptName[MAX_PATH];

	g_OrdinSkillsSetting.GetString(nRowId,  "LvlSetScript", "", szSettingScriptName, MAX_PATH );
	if (!szSettingScriptName[0]) 
		return;

	g_SetFilePath("\\");

	KLuaScript Script;
	Script.Init();
	Script.RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
	if(!Script.Load(szSettingScriptName)) 
	{
//#ifdef _DEBUG
		//g_DebugLog("[Skill] CAN NOT LOAD LEVEL DATA SCRIPT %s", szSettingScriptName);
//#endif
		return;
	}
	pScript  = &Script;
#else
	if(!m_dwSkillLevelDataScriptId) 
	{
		//printf("[Skill] CAN NOT LOAD LEVEL DATA SCRIPT %s\n", GetSkillName());
		return ;
	}
	
	pScript = (KLuaScript*)g_GetScript(m_dwSkillLevelDataScriptId);
	
	if(!pScript)
	{
		//printf("[Skill] CAN NOT LOAD LEVEL DATA SCRIPT %s\n", GetSkillName());
		return ;
	}
#endif
	
	

	int nSafeIndex = 1;
	pScript->SafeCallBegin(&nSafeIndex);
	//
	int i = 0;
	char szSettingName[40];
	char szSettingData[40]; 
	for(i = 0;  i  < MAXSKILLLEVELSETTINGNUM; i++)
	{
		sprintf(szSettingName, "LvlSetting%d", i + 1);
		sprintf(szSettingData, "LvlData%d", i + 1);
		
		g_OrdinSkillsSetting.GetString(nRowId, szSettingName, "", szSettingNameValue, 100);
		g_OrdinSkillsSetting.GetString(nRowId, szSettingData, "", szSettingDataValue, 100);
		
		
		if (szSettingNameValue[0] == 0 	|| szSettingDataValue[0] == '0')
		{
			continue;
		}
		
		pScript->CallFunction("GetSkillLevelData", 1, "ssd", szSettingNameValue, szSettingDataValue, nLevel);
		const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
		if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			int nResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			sprintf(szResult, "%d", nResult);

		}
		else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			strcpy(szResult , (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)));
		}
		else
		{
			break;
		}
		ParseString2MagicAttrib(nLevel, szSettingNameValue, szResult);
		
	}
	pScript->SafeCallEnd(nSafeIndex);
}

#ifdef _SERVER
//When nLauncher == 0 , means neednt  AppendSkillEffect;
KMissleMagicAttribsData* KSkill::CreateMissleMagicAttribsData(int nLauncher)  const 
{
	if (nLauncher < 0 || m_bClientSend) return NULL; 
	
	KMissleMagicAttribsData* pMissleMagicAttribsData = new KMissleMagicAttribsData;
	
	pMissleMagicAttribsData->m_pStateMagicAttribs = (KMagicAttrib *)m_StateAttribs; 
	pMissleMagicAttribsData->m_nStateMagicAttribsNum = m_nStateAttribsNum;
	
	pMissleMagicAttribsData->m_pImmediateAttribs = (KMagicAttrib *)m_ImmediateAttribs;
	pMissleMagicAttribsData->m_nImmediateMagicAttribsNum = m_nImmediateAttribsNum;
	
	KMagicAttrib * pDamageAttribs =  new KMagicAttrib[MAX_MISSLE_DAMAGEATTRIB];
	pMissleMagicAttribsData->m_nDamageMagicAttribsNum = m_nDamageAttribsNum;
	
	
	pMissleMagicAttribsData->m_pEnhanceSkills = (KMagicAttrib *)m_EnhanceSkills;
    pMissleMagicAttribsData->m_nEnhanceSkillsNum = m_nEnhanceSkillsNum; 
	
	if (nLauncher)
	{
		Npc[nLauncher].AppendSkillEffect(m_nId, m_bIsPhysical, m_bIsMelee, (KMagicAttrib *)m_DamageAttribs, pDamageAttribs);
	}
	else
	{
		memcpy(pDamageAttribs, (KMagicAttrib *)m_DamageAttribs, sizeof(m_DamageAttribs));
	}
	
	pMissleMagicAttribsData->m_pDamageMagicAttribs = pDamageAttribs;
	return pMissleMagicAttribsData;
}
#endif
/*!*****************************************************************************
// Function		: KSkill::SetMissleGenerateTime
// Purpose		: »ñµÃµ±Ç°µÄ×Óµ¯µÄÊµ¼Ê²úÉúÊ±¼ä
// Return		: void 
// Argumant		: Missle * pMissle
// Argumant		: int nNo
// Comments		:
// Author		: RomanDou
*****************************************************************************/
unsigned int KSkill::GetMissleGenerateTime(int nNo) const 
{
	
	switch(m_eMisslesGenerateStyle)
	{
	case SKILL_MGS_NULL:
		{
			return m_nWaitTime;
		}break;
		
	case SKILL_MGS_SAMETIME:
		{
			return  m_nWaitTime + m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_ORDER:		
		{
			return  m_nWaitTime + nNo * m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_RANDONORDER:	
		{
			if (g_Random(2) == 1) 
				return m_nWaitTime + nNo * m_nMisslesGenerateData + g_Random(m_nMisslesGenerateData);
			else 
				return m_nWaitTime + nNo * m_nMisslesGenerateData  - g_Random(m_nMisslesGenerateData / 2);
		}break;
		
	case SKILL_MGS_RANDONSAME:	
		{
			return  m_nWaitTime + g_Random(m_nMisslesGenerateData);
		}break;
		
	case SKILL_MGS_CENTEREXTENDLINE:
		{
			if (m_nChildSkillNum <= 1) return m_nWaitTime;
			int nCenter = m_nChildSkillNum / 2	;
			return m_nWaitTime + abs(nNo - nCenter) * m_nMisslesGenerateData ;
		}
	}
	return m_nWaitTime;
}

int KSkill::GetSkillIdFromName(char * szSkillName)  
{
	//	
	if (!szSkillName || !szSkillName[0]) 
        return -1;
	
	for (int i = 0; i < MAX_SKILL; i ++)
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(i, 1);
		if (pOrdinSkill) 
		{
			if (!strcmp(pOrdinSkill->m_szName, szSkillName))
            {
                return i;
            }
		}
	}
	return -1;
	
}




BOOL KSkill::CastInitiativeSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime)  const 
{
	
#ifdef _SERVER
	if (nParam1 != -1 && m_bTargetSelf) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
		CastInitiativeSkill(nLauncher, nParam2);
	}
	else
	{	
		
		if (nParam1 != -1 || nParam2 <= 0 || nParam2 >= MAX_NPC) 
			return FALSE;
		
	
		NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
		
		if (m_bTargetEnemy)
		{
			if (Relation & relation_enemy)
			//Kenny Fix skill
			CastInitiativeSkill(nLauncher, nParam2);
			//else if (Npc[nParam2].m_Kind == kind_normal)
			//CastInitiativeSkill(nLauncher, nParam2);
		}
		
		if (m_bTargetAlly)
		{
			if (Relation & relation_ally) 
				CastInitiativeSkill(nLauncher, nParam2);
		}
		
		if (m_bTargetSelf)
		{
			if (Relation & relation_self) 
				CastInitiativeSkill(nLauncher, nParam2);
		}
		return FALSE;
	}		
#endif
	return TRUE;
}

#ifdef _SERVER

/*

void KSkill::CastInitiativeSkill(int nLauncher, int nParam) const
{
	KMissleMagicAttribsData * pAttribsData = CreateMissleMagicAttribsData(nLauncher);
	if(pAttribsData) 
	{
		if(Npc[nParam].ReceiveDamage(nLauncher, m_bIsMelee, pAttribsData->m_pDamageMagicAttribs, m_bUseAttackRate, m_bDoHurt))
		{		
			if(pAttribsData->m_nStateMagicAttribsNum > 0)
			{
				Npc[nParam].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, pAttribsData->m_pStateMagicAttribs, pAttribsData->m_nStateMagicAttribsNum, pAttribsData->m_pStateMagicAttribs[0].nValue[1]);
			}
			
			else
			{	
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nId, m_ulLevel);
				BYTE btStatePriority = pOrdinSkill->GetStatePriority();
				switch(btStatePriority)
				{	
					case 1:
					case 2:
						break;
					case 3:	
						break;
					case 4:
						Npc[nParam].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, 0, 0, 18);
						break;
					default:
						break;
				}
			}
			
			if(pAttribsData->m_nImmediateMagicAttribsNum > 0)
				Npc[nParam].SetImmediatelySkillEffect(nLauncher, pAttribsData->m_pImmediateAttribs, pAttribsData->m_nImmediateMagicAttribsNum);
		}
		return;
	}
	if(pAttribsData->DelRef() == 0)
	{
		delete pAttribsData;
		pAttribsData = NULL; //them code 16/09/2023 by kinnox;
	}
}*/
//Kenny Fix Skill
void KSkill::CastInitiativeSkill(int nLauncher, int nParam) const
{
    KMissleMagicAttribsData* pAttribsData = CreateMissleMagicAttribsData(nLauncher);
    if (!pAttribsData) return;

    BOOL bHit = Npc[nParam].ReceiveDamage(
        nLauncher,
        m_bIsMelee,
        pAttribsData->m_pDamageMagicAttribs,
        m_bUseAttackRate,
        m_bDoHurt
    );
/*
    if (bHit || pAttribsData->m_nDamageMagicAttribsNum == 0)
    {
		printf("[CastInit] id=%d, DoHurt=%d, DmgNum=%d, StateNum=%d, bHit=%d, tgtKind=%d\n",
       m_nId, m_bDoHurt,
       pAttribsData->m_nDamageMagicAttribsNum,
       pAttribsData->m_nStateMagicAttribsNum,
       bHit, Npc[nParam].m_Kind);
        if (pAttribsData->m_nStateMagicAttribsNum > 0)
        {
            Npc[nParam].SetStateSkillEffect(
                nLauncher, m_nId, m_ulLevel,
                pAttribsData->m_pStateMagicAttribs,
                pAttribsData->m_nStateMagicAttribsNum,
                pAttribsData->m_pStateMagicAttribs[0].nValue[1]
            );
        }*/
		if (bHit || pAttribsData->m_nDamageMagicAttribsNum == 0) {
			if (pAttribsData->m_nStateMagicAttribsNum > 0) {
				int stateId = pAttribsData->m_pStateMagicAttribs[0].nValue[0];
				int stateTime = pAttribsData->m_pStateMagicAttribs[0].nValue[1];

				//printf("[StateApply-Pre] skill=%d -> npc=%d, stateId=%d, time=%d, tgtKind=%d\n", m_nId, nParam, stateId, stateTime, Npc[nParam].m_Kind);

				Npc[nParam].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel,
					   pAttribsData->m_pStateMagicAttribs,
					   pAttribsData->m_nStateMagicAttribsNum,
					   stateTime);
			}
        else
        {
            KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_nId, m_ulLevel);
            BYTE btStatePriority = pOrdinSkill->GetStatePriority();
            if (btStatePriority == 4)
            {
                Npc[nParam].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, 0, 0, 18);
            }
        }

        if (pAttribsData->m_nImmediateMagicAttribsNum > 0)
        {
            Npc[nParam].SetImmediatelySkillEffect(
                nLauncher,
                pAttribsData->m_pImmediateAttribs,
                pAttribsData->m_nImmediateMagicAttribsNum
            );
        }
    }

    if (pAttribsData->DelRef() == 0)
    {
        delete pAttribsData;
        pAttribsData = NULL; // thêm code 16/09/2023 by kinnox
    }
}


#endif

/*!*****************************************************************************
// Function		: KSkill::CastPassivitySkill
// Purpose		: ±»¶¯
// Return		: BOOL 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL KSkill::CastPassivitySkill(int nLauncher, int nParam1, int nParam2, int nWaitTime, BOOL bRemove /*= FALSE*/)  const 
{
#ifdef _SERVER
	KMissleMagicAttribsData * pAttribsData = (KMissleMagicAttribsData*)m_StateAttribs;
	if (m_nStateAttribsNum > 0)
	{
		Npc[nLauncher].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, (KMagicAttrib *)m_StateAttribs, m_nStateAttribsNum, -1, bRemove);
	}
	
	else
	{	
		KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nId, m_ulLevel);
		BYTE btStatePriority = pOrdinSkill->GetStatePriority();
		switch(btStatePriority)
		{	
			case 1:
			case 2:	
			case 3:
				break;
			case 4:
				Npc[nLauncher].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, 0, 0, -1, bRemove);
				break;
			default:
				break;
		}
	}
	
#endif
	return TRUE;
}

#ifdef _SERVER
// --
//
// --
BOOL KSkill::CastStateSkill( int nLauncher, int nParam1, int nParam2, int nWaitTime, BOOL bRemove /*= FALSE*/) const
{

	KMissleMagicAttribsData * pAttribsData = (KMissleMagicAttribsData*)m_StateAttribs;
	if (m_nStateAttribsNum > 0)
	{
		Npc[nLauncher].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, (KMagicAttrib *)m_StateAttribs, m_nStateAttribsNum, nWaitTime, bRemove);
	}
	return TRUE;
}
#endif

/*!*****************************************************************************
// Function		: KSkill::ParseString2MagicAttrib
// Purpose		: ½âÎöÍ¨¹ý½Å±¾ÔËËã»ñµÃµÄ¼¼ÄÜÊý¾Ý
// Return		: 
// Argumant		: char * szMagicAttribName
// Argumant		: char * szValue
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::ParseString2MagicAttrib(unsigned long ulLevel, char * szMagicAttribName, char * szValue)  
{
	int nValue1 = 0;
	int nValue2 = 0;
	int nValue3 = 0;
    const char *pcszTemp = NULL;
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) 
		return FALSE;
	
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, MagicAttrib2String(i)))
		{
            pcszTemp = szValue;
            nValue1 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue2 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue3 = KSG_StringGetInt(&pcszTemp, 0);


			if (i > magic_missle_begin && i < magic_missle_end)
			{
				m_MissleAttribs[m_nMissleAttribsNum].nAttribType = i;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[0] = nValue1;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[1] = nValue2;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[2] = nValue3;
				m_nMissleAttribsNum ++;
				return TRUE;
			}
			if (i > magic_skill_begin && i < magic_skill_end)
			{
				switch(i)
				{
				case magic_skill_cost_v:				
					{
						m_nCost = nValue1;
					}
					break;
					
				case magic_skill_costtype_v:
					{
						m_nSkillCostType = (NPCATTRIB)nValue1;
					}break;
					
				case magic_skill_mintimepercast_v: 		
					{
						m_nMinTimePerCast = nValue1;
					}break;
					
				case magic_skill_misslenum_v:
					{
						m_nChildSkillNum = nValue1;
					}break;
					
				case magic_skill_misslesform_v:
					{
						m_eMisslesForm = (eMisslesForm) nValue1;
					}break;
				case magic_skill_param1_v:
					{
						m_nValue1 = nValue1;
					}
					break;
				case magic_skill_param2_v:	
					{
						m_nValue2 = nValue2;
					}
					break;
					
				case magic_skill_startevent:
					{
						m_bStartEvent = nValue1;
						m_nStartSkillId = nValue3;

					}
					break;
				case magic_skill_flyevent:
					{
						m_bFlyingEvent = nValue1;
						m_nFlyEventTime = nValue2;
						m_nFlySkillId = nValue3;

					}
					break;
				case magic_skill_collideevent:
					{
						m_bCollideEvent = nValue1;
						m_nCollideSkillId = nValue3;

					}
					break;
				case magic_skill_vanishedevent:
					{
						m_bVanishedEvent = nValue1;
						m_nVanishedSkillId = nValue3;

					}
					break;
					
				case magic_skill_eventskilllevel:
					{
						m_nEventSkillLevel = nValue1;
					}
					break;
				}
				return TRUE;
			}
			
			if (i > magic_damage_begin && i < magic_damage_end)
			{
				switch(i)
				{
				case magic_attackrating_v:
				case magic_attackrating_p:
					m_DamageAttribs[0].nAttribType = i;
					m_DamageAttribs[0].nValue[0] = nValue1;
					m_DamageAttribs[0].nValue[1] = nValue2;
					m_DamageAttribs[0].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_ignoredefense_p:
					m_DamageAttribs[1].nAttribType = i;
					m_DamageAttribs[1].nValue[0] = nValue1;
					m_DamageAttribs[1].nValue[1] = nValue2;
					m_DamageAttribs[1].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_physicsdamage_v:
				case magic_physicsenhance_p:
					m_DamageAttribs[2].nAttribType = i;
					m_DamageAttribs[2].nValue[0] = nValue1;
					m_DamageAttribs[2].nValue[1] = nValue2;
					m_DamageAttribs[2].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_colddamage_v:
				case magic_coldenhance_p:
					m_DamageAttribs[3].nAttribType = i;
					m_DamageAttribs[3].nValue[0] = nValue1;
					m_DamageAttribs[3].nValue[1] = nValue2;
					m_DamageAttribs[3].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_firedamage_v:
				case magic_fireenhance_p:
					m_DamageAttribs[4].nAttribType = i;
					m_DamageAttribs[4].nValue[0] = nValue1;
					m_DamageAttribs[4].nValue[1] = nValue2;
					m_DamageAttribs[4].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_lightingdamage_v:
				case magic_lightingenhance_p:
					m_DamageAttribs[5].nAttribType = i;
					m_DamageAttribs[5].nValue[0] = nValue1;
					m_DamageAttribs[5].nValue[1] = nValue2;
					m_DamageAttribs[5].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_poisondamage_v:
				case magic_poisonenhance_p:
					m_DamageAttribs[6].nAttribType = i;
					m_DamageAttribs[6].nValue[0] = nValue1;
					m_DamageAttribs[6].nValue[1] = nValue2;
					m_DamageAttribs[6].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_magicdamage_v:
				case magic_magicenhance_p:
					m_DamageAttribs[7].nAttribType = i;
					m_DamageAttribs[7].nValue[0] = nValue1;
					m_DamageAttribs[7].nValue[1] = nValue2;
					m_DamageAttribs[7].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
					
				case magic_seriesdamage_p:
					m_DamageAttribs[8].nAttribType = i;
					m_DamageAttribs[8].nValue[0] = nValue1;
					m_DamageAttribs[8].nValue[1] = nValue2;
					m_DamageAttribs[8].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_deadlystrike_p:
				case magic_deadlystrikeenhance_p:
					m_DamageAttribs[9].nAttribType = i;
					m_DamageAttribs[9].nValue[0] = nValue1;
					m_DamageAttribs[9].nValue[1] = nValue2;
					m_DamageAttribs[9].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_fatallystrike_p:
				case magic_fatallystrikeenhance_p:
					m_DamageAttribs[10].nAttribType = i;
					m_DamageAttribs[10].nValue[0] = nValue1;
					m_DamageAttribs[10].nValue[1] = nValue2;
					m_DamageAttribs[10].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_stun_p:
					m_DamageAttribs[11].nAttribType = i;
					m_DamageAttribs[11].nValue[0] = nValue1;
					m_DamageAttribs[11].nValue[1] = nValue2;
					m_DamageAttribs[11].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_steallife_p:
				case magic_steallifeenhance_p:
					m_DamageAttribs[12].nAttribType = i;
					m_DamageAttribs[12].nValue[0] = nValue1;
					m_DamageAttribs[12].nValue[1] = nValue2;
					m_DamageAttribs[12].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_stealmana_p:
				case magic_stealmanaenhance_p:
					m_DamageAttribs[13].nAttribType = i;
					m_DamageAttribs[13].nValue[0] = nValue1;
					m_DamageAttribs[13].nValue[1] = nValue2;
					m_DamageAttribs[13].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_stealstamina_p:
				case magic_stealstaminaenhance_p:
					m_DamageAttribs[14].nAttribType = i;
					m_DamageAttribs[14].nValue[0] = nValue1;
					m_DamageAttribs[14].nValue[1] = nValue2;
					m_DamageAttribs[14].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_skill_skillexp_v:
					m_DamageAttribs[15].nAttribType = i;
					m_DamageAttribs[15].nValue[0] = nValue1;
					m_DamageAttribs[15].nValue[1] = nValue2;
					m_DamageAttribs[15].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
					
				}
				return TRUE;
			}
			//AutoAI by kinnox;
			switch (i)
			{
				case magic_lifereplenish_v:
					m_bSkillLifeReplenish = TRUE;
					break;
			}
			//end code			
			if(i == magic_skill_mintimepercastonhorse_v)
			{
				m_nMinTimePerCastOnHorse = nValue1;
				return TRUE;
			}

			if(i == magic_skill_attackradius)
			{	
				m_nAttackRadius = nValue1;
				return TRUE;
			}

			if(i == magic_skill_showevent)
			{	
				m_btShowEvent = (BYTE)nValue1;
				return TRUE;
			}

			if ( i >= magic_addskilldamage1 && i <= magic_addskilldamage6)
			{
				switch(i)
				{
				case magic_addskilldamage1:
					m_EnhanceSkills[0].nAttribType = i;
					m_EnhanceSkills[0].nValue[0] = nValue1;
					m_EnhanceSkills[0].nValue[1] = nValue2;
					m_EnhanceSkills[0].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 1;
					break;
				case magic_addskilldamage2:
					m_EnhanceSkills[1].nAttribType = i;
					m_EnhanceSkills[1].nValue[0] = nValue1;
					m_EnhanceSkills[1].nValue[1] = nValue2;
					m_EnhanceSkills[1].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 2;
					break;
				case magic_addskilldamage3:
					m_EnhanceSkills[2].nAttribType = i;
					m_EnhanceSkills[2].nValue[0] = nValue1;
					m_EnhanceSkills[2].nValue[1] = nValue2;
					m_EnhanceSkills[2].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 3;
					break;
				case magic_addskilldamage4:
					m_EnhanceSkills[3].nAttribType = i;
					m_EnhanceSkills[3].nValue[0] = nValue1;
					m_EnhanceSkills[3].nValue[1] = nValue2;
					m_EnhanceSkills[3].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 4;
					break;
				case magic_addskilldamage5:
					m_EnhanceSkills[4].nAttribType = i;
					m_EnhanceSkills[4].nValue[0] = nValue1;
					m_EnhanceSkills[4].nValue[1] = nValue2;
					m_EnhanceSkills[4].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 5;
					break;
				case magic_addskilldamage6:
					m_EnhanceSkills[5].nAttribType = i;
					m_EnhanceSkills[5].nValue[0] = nValue1;
					m_EnhanceSkills[5].nValue[1] = nValue2;
					m_EnhanceSkills[5].nValue[2] = nValue3;
					m_nEnhanceSkillsNum = 6;
					break;
				}
				return TRUE;
			}
#ifndef _SERVER			
			if (i == magic_skill_desc)
			{
				sprintf(m_szSkillDescEx, szValue);
				return TRUE;
			}
#endif		
			if(i == magic_autoattackskill)
			{
				m_nDamageAttribsNum++;
			}
			
			if (nValue2 == 0) 
			{
				m_ImmediateAttribs[m_nImmediateAttribsNum].nAttribType = i;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[0] = nValue1;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[1] = nValue2;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[2] = nValue3;
				m_nImmediateAttribsNum ++;
				return TRUE;
				
			}
			else
			{
				m_StateAttribs[m_nStateAttribsNum].nAttribType = i;
				m_StateAttribs[m_nStateAttribsNum].nValue[0] = nValue1;
				m_StateAttribs[m_nStateAttribsNum].nValue[1] = nValue2;
				m_StateAttribs[m_nStateAttribsNum].nValue[2] = nValue3;
				m_nStateAttribsNum ++;
				return TRUE;
			}
			
		}
	}
	return FALSE;
}

const char * KSkill::MagicAttrib2String(int MagicAttrib)  const 
{
	return 	g_MagicID2String(MagicAttrib);
}

#ifndef _SERVER
// --

// --
void	KSkill::DrawSkillIcon(int nSkillID, int x, int y, int Width, int Height)  
{

	if (!m_szSkillIcon[0]) 
		return ;
	
	m_RUIconImage.nType = ISI_T_SPR;
	m_RUIconImage.Color.Color_b.a = 255;
	m_RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUIconImage.uImage = 0;
	m_RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUIconImage.bRenderFlag = 0;
	strcpy(m_RUIconImage.szImage, m_szSkillIcon);
	m_RUIconImage.oPosition.nX = x;
	m_RUIconImage.oPosition.nY = y;
	m_RUIconImage.oPosition.nZ = 0;
	m_RUIconImage.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUIconImage, RU_T_IMAGE, 1);

	if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextCastTime(nSkillID) > SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_dwCurrentTime)
	{
		DWORD dwMaxTime = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetMaxCastTime(nSkillID);
		if (dwMaxTime <= 0)
			return;

		DWORD dwTime = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextCastTime(nSkillID) - SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_dwCurrentTime;
		float nPercent = float(dwTime * 1.0 / dwMaxTime);
		nPercent *= 100;
		
		m_RUIconShadow.Color.Color_b.a = 20;
		m_RUIconShadow.Color.Color_dw = 0x0f000000;
		m_RUIconShadow.oPosition.nX = x;
		m_RUIconShadow.oPosition.nY = y;
		m_RUIconShadow.oEndPos.nX = x + Width;
		m_RUIconShadow.oEndPos.nY = y + Height;
		m_RUIconShadow.oEndPos.nZ = m_RUIconShadow.oPosition.nZ = 0;
		g_pRepresent->DrawPrimitives(1, &m_RUIconShadow, RU_T_SHADOW, 1);

		m_RUIconShadow.Color.Color_b.a = 10;
		m_RUIconShadow.Color.Color_dw = 0x0ffff000;
		m_RUIconShadow.oPosition.nX = x;
		m_RUIconShadow.oPosition.nY = y + Height - (Height * nPercent) / 100;
		m_RUIconShadow.oEndPos.nX = x + Width;
		m_RUIconShadow.oEndPos.nY = y + Height;
		m_RUIconShadow.oEndPos.nZ = m_RUIconShadow.oPosition.nZ = 0;
		g_pRepresent->DrawPrimitives(1, &m_RUIconShadow, RU_T_SHADOW, 1);
	}
}

// --
//
// --
void	KSkill::GetDesc(unsigned long ulSkillId, unsigned long ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc)
{
	if(!pszMsg) 
		return;
	if(nOwnerIndex <= 0 || nOwnerIndex >= MAX_PLAYER)	
		return;
	strcpy(pszMsg,"");
	char szTemp[300];
	char szKey[16];
	
	KSkill * pTempSkill = NULL;
	KSkill * pCurSkill = NULL;
	KSkill * pNextSkill = NULL;
	
	if(ulCurLevel == 0)
	{
		pNextSkill = (KSkill*)g_SkillManager.GetSkill(ulSkillId, 1);
		pTempSkill = pNextSkill;
	}
	else
	{	
		pCurSkill = (KSkill*) g_SkillManager.GetSkill(ulSkillId, ulCurLevel);
		pNextSkill = (KSkill*) g_SkillManager.GetSkill(ulSkillId, ulCurLevel + 1);
		pTempSkill = pCurSkill;
	}
	
	if(pTempSkill == NULL)
		return;
	
	strcat(pszMsg, "<color=Yellow>");
	strcat(pszMsg, pTempSkill->m_szName);
	strcat(pszMsg, "\n<bclr=Black><color>");
	
	
	if(ulCurLevel == 0)
	{	
		if(Npc[Player[nOwnerIndex].m_nIndex].m_Level < pNextSkill->GetSkillReqLevel())
		{
			strcat(pszMsg, "<color=red>");
		}
		sprintf(szTemp, "§¼ng cÊp yªu cÇu: %d", pNextSkill->GetSkillReqLevel());
		strcat(pszMsg, szTemp);
		if(Npc[Player[nOwnerIndex].m_nIndex].m_Level < pNextSkill->GetSkillReqLevel())
		{
			strcat(pszMsg, "<color>");
		}
		strcat(pszMsg, "\n");
	}
	
	
	
#ifdef SHOW_SKILL_MORE_INFO
	switch(pTempSkill->m_eSkillStyle)
	{
		case SKILL_SS_Missles:
			{
				strcat(pszMsg, "MisslesForm-");
				szTemp[0] = 0;
				switch(pTempSkill->m_eMisslesForm)
				{
					case SKILL_MF_Wall:
						{
							sprintf(szTemp, "Wall: %d\n", pTempSkill->m_nChildSkillNum);
						}
						break;	
					case SKILL_MF_Line:
						{
							sprintf(szTemp, "Line: %d\n", pTempSkill->m_nChildSkillNum);
						}
						break;
					case SKILL_MF_Spread:
						{
							sprintf(szTemp, "Spread: %d\n", pTempSkill->m_nChildSkillNum);
						}
						break;
					case SKILL_MF_Circle:
						{
							sprintf(szTemp, "Circle: %d\n", pTempSkill->m_nChildSkillNum);
						}
						break;
					case SKILL_MF_Random:
						break;
					case SKILL_MF_Zone:
						{
							if (pTempSkill->m_nValue1)
								sprintf(szTemp, "Zone_1: %d\n", pTempSkill->m_nChildSkillNum);
							else 
								sprintf(szTemp, "Zone: %d\n", pTempSkill->m_nChildSkillNum);
							
						}
						break;
					case SKILL_MF_AtTarget:
						{
							if (pTempSkill->m_nValue1)
								sprintf(szTemp, "AtTarget_1: %d\n", pTempSkill->m_nChildSkillNum);
							else 
								sprintf(szTemp, "AtTarget: %d\n", pTempSkill->m_nChildSkillNum);
						}
						break;
					case SKILL_MF_AtFirer:
						{
							if (pTempSkill->m_nValue1)
								sprintf(szTemp, "AtFirer_1: %d\n", pTempSkill->m_nChildSkillNum);
							else 
								sprintf(szTemp, "AtFirer: %d\n", pTempSkill->m_nChildSkillNum);
							
						}
						break;
					default:
						break;
				}
				strcat(pszMsg, szTemp);
				
			}
			break;
			
		case SKILL_SS_Melee:
			{
				strcat(pszMsg, "SkillStyle: Melee\n");
			}break;
		case SKILL_SS_InitiativeNpcState:
			{
				strcat(pszMsg, "Initiative State\n");			
				if (pTempSkill->m_StateAttribs[0].nValue[1] > 0)
				{
					sprintf (szTemp, "State Time: %d\n" ,pTempSkill->m_StateAttribs[0].nValue[1]);
					strcat(pszMsg,szTemp);
				}
			}
			break;
		case SKILL_SS_PassivityNpcState:
			{
				strcat(pszMsg, "Passivity State\n");
			}
			break;			
		case SKILL_SS_CreateNpc:
		case SKILL_SS_BuildPoison:
		case SKILL_SS_AddPoison:
		case SKILL_SS_GetObjDirectly:
		case SKILL_SS_StrideObstacle:
		case SKILL_SS_BodyToObject:
		case SKILL_SS_Mining:
		case SKILL_SS_RepairWeapon:
		case SKILL_SS_Capture:
		default:
			break;
	}

	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bRangeDamage) 
		strcat(pszMsg, "RangeDamage\n");
	
	switch(g_MisslesLib[pTempSkill->m_nChildSkillId].m_eMoveKind)
	{
		case MISSLE_MMK_Stand:
			{
				strcat(pszMsg, "MoveKind: Stand\n");
			}
			break;
		case MISSLE_MMK_Line:
			{
				strcat(pszMsg, "MoveKind: Line\n");
			}
			break;
		case MISSLE_MMK_Random:
			break;
		case MISSLE_MMK_Circle:
			{
				strcat(pszMsg, "MoveKind: Circle\n");
			}
			break;
		case MISSLE_MMK_Helix:
			{
				strcat(pszMsg, "MoveKind: Helix\n");
			}
			break;
		case MISSLE_MMK_Follow:
			{
				strcat(pszMsg, "MoveKind: Follow\n");
			}
			break;
		case MISSLE_MMK_Motion:
			{
				strcat(pszMsg, "MoveKind: Motion\n");
			}
			break;
		case MISSLE_MMK_Parabola:
			{
				strcat(pszMsg, "MoveKind: Parabola\n");
			}
			break;						
		case MISSLE_MMK_SingleLine:
			{
				strcat(pszMsg, "MoveKind: SingleLine\n");
			}
			break;
		default:
			break;
	}
	
	if (!g_MisslesLib[pTempSkill->m_nChildSkillId].m_bCollideVanish)
		strcat(pszMsg, "CollideVanish ");
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange > 1) 
	{
		sprintf(szTemp, "DamageRange: %d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack)
	{
		sprintf(szTemp, "KnockBack: %d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bAutoExplode)
	{
		strcat(pszMsg, "AutoExplode");
	}
	
	if (pTempSkill->m_bIsAura) 
		strcat(pszMsg, "Aura ");
	if (pTempSkill->m_bIsPhysical) 
		strcat(pszMsg, "Physical ");
	if (pTempSkill->m_bIsMelee) 
		strcat(pszMsg, "Melee ");
	if (pTempSkill->m_bTargetOnly) 
		strcat(pszMsg, "TargetOnly ");
	if (pTempSkill->m_bTargetAlly) 
		strcat(pszMsg, "TargetAlly ");
	if (pTempSkill->m_bTargetEnemy) 
		strcat(pszMsg, "TargetEnemy ");
	if (pTempSkill->m_bTargetObj)	  
		strcat(pszMsg, "TargetObj ");
	if (pTempSkill->m_bTargetSelf) 
		strcat(pszMsg, "TargetSelf ");
	if (pTempSkill->m_bUseAttackRate) 
		strcat(pszMsg, "UseAttackRate ");
#endif
	
	strcat (pszMsg, "\n");
	strcat(pszMsg, pTempSkill->m_szSkillDesc);
	
	
	strcat(pszMsg, "\n\n");
	short nSkillAttrib = pTempSkill->GetSkillAttrib();
	if(nSkillAttrib > 100)
	{	
		itoa(nSkillAttrib, szKey, 10);
		g_GameSettingFile.GetString("SkillAttrib", szKey, "", szTemp, sizeof(szTemp));
		if(szTemp[0])
		{
			strcat(pszMsg, szTemp);
			strcat(pszMsg, "\n\n");
		}
	}
	
	
	//
	if (pTempSkill->GetSkillAttrib() >= 100)
	{	
		int nPointAdd = Npc[Player[nOwnerIndex].m_nIndex].m_SkillList.GetPointAdd(ulSkillId);
		if(nPointAdd > 0)
		{
			sprintf(szTemp, "§¼ng cÊp hiÖn t¹i:%d (%d + <color=Blue>%d<color>)", ulCurLevel, ulCurLevel - nPointAdd, nPointAdd);
		}
		else
		{
			sprintf(szTemp, "§¼ng cÊp hiÖn t¹i: %d", ulCurLevel);	
		}
		BYTE btMaxLevel = (BYTE)(g_SkillManager.GetSkillMaxLevel(ulSkillId) + Npc[Player[nOwnerIndex].m_nIndex].m_cTransLife);
		if((ulCurLevel - nPointAdd) >= btMaxLevel)
		{
			strcat(szTemp, " (<color=red>CÊp cao nhÊt<color>)");
		}
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "\n");
	}
	
	
	int i = 0;
	
	if (pCurSkill)
	{	
		pCurSkill->GetDescAboutLevel(pszMsg, ulSkillId, ulCurLevel);
		
		if(pCurSkill->GetStatePriority() <= 0)
		{
			KSkill * pOrdinSkill = NULL;
			BYTE btFloor = 2;
			if(pCurSkill->m_bFlyingEvent)
			{
				pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pCurSkill->GetFlySkillId(), pCurSkill->GetEventSkillLevel());
				if(pOrdinSkill)
				{	
					sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pCurSkill->GetEventSkillLevel());
					strcat(pszMsg, "\n");
					strcat(pszMsg, szTemp);
					pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, nOwnerIndex);
					btFloor++;
				}
			}
			if(pCurSkill->m_bStartEvent)
			{
				pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pCurSkill->GetStartSkillId(), pCurSkill->GetEventSkillLevel());
				if(pOrdinSkill)
				{	
					sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pCurSkill->GetEventSkillLevel());
					strcat(pszMsg, "\n");
					strcat(pszMsg, szTemp);
					pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, nOwnerIndex);
					btFloor++;
				}
			}
			if(pCurSkill->m_bCollideEvent)
			{
				pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pCurSkill->GetCollideSkillId(), pCurSkill->GetEventSkillLevel());
				if(pOrdinSkill)
				{	
					sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pCurSkill->GetEventSkillLevel());
					strcat(pszMsg, "\n");
					strcat(pszMsg, szTemp);
					pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, nOwnerIndex);
					btFloor++;
				}
			}
			if(pCurSkill->m_bVanishedEvent)
			{
				pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pCurSkill->GetVanishedSkillId(), pCurSkill->GetEventSkillLevel());
				if(pOrdinSkill)
				{	
					sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pCurSkill->GetEventSkillLevel());
					strcat(pszMsg, "\n");
					strcat(pszMsg, szTemp);
					pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, nOwnerIndex);
					btFloor++;
				}
			}
		}
		
	}
	
	if (pTempSkill->m_nHorseLimited)
	{
		switch(pTempSkill->m_nHorseLimited)
		{
			case 1:
				{	
					strcat(pszMsg, "\n");
					strcat(pszMsg, "\n");
					strcat(pszMsg, "Kh«ng thÓ sö dông kü n¨ng khi trªn ngùa\n");
				}
				break;
			case 2:
				{	
					strcat(pszMsg, "\n");
					strcat(pszMsg, "\n");
					strcat(pszMsg, "Kü n¨ng sö dông trªn ngùa\n");
				}
				break;
			default:
				return ;
		}
	}
	
	
	short nEqLimited = pTempSkill->m_nEquiptLimited;
	strcat(pszMsg,"\n");
	strcat(pszMsg,"H¹n chÕ vò khÝ: ");
	if(nEqLimited >= 0)
	{
		itoa(nEqLimited, szKey, 10);
		g_GameSettingFile.GetString("WeaponLimit", szKey, "", szTemp, sizeof(szTemp));
	}
	else if(nEqLimited == -2)
	{
		g_GameSettingFile.GetString("WeaponLimit", "F2", "", szTemp, sizeof(szTemp));
	}
	else if(nEqLimited == -1)
	{
		g_GameSettingFile.GetString("WeaponLimit", "F1", "", szTemp, sizeof(szTemp));
	}
	strcat(pszMsg, szTemp);
	strcat(pszMsg,"\n");
	

	//
	if (pNextSkill->GetSkillAttrib() >= 100)
	{
		if (bGetNextLevelDesc)
		{
			if(pNextSkill)
			{
				strcat(pszMsg, "\n<color=Red>§¼ng cÊp tiÕp theo\n");
				pNextSkill->GetDescAboutLevel(pszMsg, ulSkillId, ulCurLevel + 1);
			
				
				if(pNextSkill->GetStatePriority() <= 0)
				{
					KSkill * pOrdinSkill = NULL;
					BYTE btFloor = 2;
					if(pNextSkill->m_bFlyingEvent)
					{
						pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pNextSkill->GetFlySkillId(), pNextSkill->GetEventSkillLevel());
						if(pOrdinSkill)
						{	
							sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pNextSkill->GetEventSkillLevel());
							strcat(pszMsg, "\n");
							strcat(pszMsg, szTemp);
							pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, -1);
							btFloor++;
						}
					}
					if(pNextSkill->m_bStartEvent)
					{
						pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pNextSkill->GetStartSkillId(), pNextSkill->GetEventSkillLevel());
						if(pOrdinSkill)
						{	
							sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pNextSkill->GetEventSkillLevel());
							strcat(pszMsg, "\n");
							strcat(pszMsg, szTemp);
							pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, -1);
							btFloor++;
						}
					}
					if(pNextSkill->m_bCollideEvent)
					{
						pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pNextSkill->GetCollideSkillId(), pNextSkill->GetEventSkillLevel());
						if(pOrdinSkill && pNextSkill->GetEventSkillLevel() != 1)
						{	
							sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pNextSkill->GetEventSkillLevel());
							strcat(pszMsg, "\n");
							strcat(pszMsg, szTemp);
							pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, -1);
							btFloor++;
						}
					}
					if(pNextSkill->m_bVanishedEvent)
					{
						pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pNextSkill->GetVanishedSkillId(), pNextSkill->GetEventSkillLevel());
						if(pOrdinSkill && pOrdinSkill->GetEventSkillLevel() == 0)
						{	
							sprintf(szTemp, "<color=blue>TÇng thø %d:<color> <color=yellow>[ %s ]<color> <color=blue>[CÊp %d]<color>", btFloor, pOrdinSkill->GetSkillName(), pNextSkill->GetEventSkillLevel());
							strcat(pszMsg, "\n");
							strcat(pszMsg, szTemp);
							pOrdinSkill->GetDescAboutEventSkillLevel(pszMsg, -1);
						}
					}
				}
				
			}
		}
	}
	
}

// --
//
// --
void KSkill::GetDescAboutLevel(char * pszMsg, int nSkillId, int nSkillLevel)
{
	
	char szTemp[100];
	int nGetCost = GetSkillCost(NULL);

	if (nGetCost)
	{
		switch(m_nSkillCostType)
		{
			case attrib_mana:
				
				sprintf(szTemp, "Néi lùc tiªu hao: %d\n", nGetCost);
				strcat(pszMsg,szTemp);
				break;
			case attrib_stamina:
				sprintf(szTemp, "ThÓ lùc tiªu hao: %d\n", nGetCost);
				strcat(pszMsg,szTemp);
				break;
			case attrib_life:
				sprintf(szTemp, "Sinh lùc tiªu hao: %d\n", nGetCost);
				strcat(pszMsg,szTemp);
				break;
		}
	}
	
	int nGetAttackRadius = GetAttackRadius();
	if (nGetAttackRadius)
	{
		sprintf(szTemp,"Ph¹m vi: %d\n", nGetAttackRadius);
		strcat(pszMsg,szTemp);
	}

	for (int i  = 0; i < m_nImmediateAttribsNum; i ++)
	{
		if (!m_ImmediateAttribs[i].nAttribType) 
			continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_ImmediateAttribs[i]);
		if (!pszInfo) 
			continue;
		strcat(pszMsg, pszInfo);
	}

	KMagicAttrib *DamageAttribs = m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType) 
			continue;
		if ((DamageAttribs + i)->nAttribType == magic_skill_skillexp_v)
		{	
			float ExpAdd = 0;
			int nPointAdd = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetPointAdd(nSkillId);
			if(nPointAdd > 0)
			{
				nSkillLevel -= nPointAdd;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nSkillLevel);
				KMagicAttrib *DamageAttribs1 = pOrdinSkill->GetDamageAttribs();
				for (int j  = 0; j < MAX_MISSLE_DAMAGEATTRIB; j ++)
				{
					if (!(DamageAttribs1 + j)->nAttribType) continue;
					if ((DamageAttribs1 + j)->nAttribType == magic_skill_skillexp_v)
					{
						ExpAdd = (float)(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal((DamageAttribs1 + j)->nValue[2])*100.0)/(DamageAttribs1 + j)->nValue[0];
						break;
					}
				}
			}
			else 
			{
				ExpAdd = (float)(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal((DamageAttribs + i)->nValue[2])*100.0)/(DamageAttribs + i)->nValue[0];
			}
			char pszInfo[80] ;
			sprintf(pszInfo, "Møc ®é luyÖn <color=water>%0.2f%%<color>",ExpAdd);
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "\n");
		}
		else
		{
			if (!m_szSkillDescEx[0])
			{
				char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
				if (!pszInfo) continue;
				strcat(pszMsg, pszInfo);
				strcat(pszMsg, "\n");
			}
		}
	}
	
	if (!m_szSkillDescEx[0])
	{
		for (i  = 0; i < m_nStateAttribsNum; i ++)
		{
			if (!m_StateAttribs[i].nAttribType) 
				continue;
			char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_StateAttribs[i]);
			if (!pszInfo) continue;
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "\n");
		}
	}

	if (m_szSkillDescEx[0])
	{
		strcat(pszMsg, m_szSkillDescEx);
		strcat(pszMsg, "\n");
	}

	
	for ( i  = 0; i < m_nEnhanceSkillsNum; i ++)
	{
		if (!m_EnhanceSkills[i].nAttribType) 
			continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_EnhanceSkills[i]);
		if (!pszInfo) 
			continue;
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}

	int nPercent = 0;
	nPercent = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetEnhanceSkills(nSkillId);
	if(nPercent > 0)
	{	
		sprintf(szTemp, "NhËn hç trî tõ kü n¨ng kh¸c: <color=gold>%d%%<color>", nPercent);
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "\n");
	}
	
}

// --

// --
void KSkill::GetDescAboutEventSkillLevel(char * pszMsg, int nIndex)
{	
	int i;
	for (i  = 0; i < m_nImmediateAttribsNum; i ++)
	{
		if (!m_ImmediateAttribs[i].nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_ImmediateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, "\n");
		strcat(pszMsg, pszInfo);
	}
	
	KMagicAttrib *DamageAttribs = m_DamageAttribs;
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType) continue;
		if((DamageAttribs + i)->nAttribType == magic_seriesdamage_p) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, "\n");
		strcat(pszMsg, pszInfo);
	}

	for (i  = 0; i < m_nStateAttribsNum; i ++)
	{
		if (!m_StateAttribs[i].nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_StateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, "\n");
		strcat(pszMsg, pszInfo);
	}
}

void KSkill::PlayPreCastSound(BOOL bIsFeMale, int nX, int nY)  const 
{
	char * pSoundFile = NULL;
	
	if (!bIsFeMale)
		pSoundFile = (char *)m_szManPreCastSoundFile;
	else 
		pSoundFile = (char *)m_szFMPreCastSoundFile;
	
	int		nCenterX = 0, nCenterY = 0, nCenterZ = 0;
	
	g_ScenePlace.GetFocusPosition(nCenterX, nCenterY, nCenterZ);
	KCacheNode * pSoundNode = NULL;
	pSoundNode = (KCacheNode*) g_SoundCache.GetNode(pSoundFile, (KCacheNode*)pSoundNode);
	KWavSound * pWave = (KWavSound*)pSoundNode->m_lpData;
	if (pWave)
	{
		pWave->Play((nX - nCenterX) * 5, (10000 - (abs(nX - nCenterX) + abs(nY - nCenterY))) * Option.GetSndVolume() / 100 - 10000, 0);
	}
}
#endif
