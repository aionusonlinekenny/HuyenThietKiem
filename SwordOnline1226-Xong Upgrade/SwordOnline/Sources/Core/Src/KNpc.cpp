#include "KCore.h"
//#include <crtdbg.h>
#include "KNpcAI.h"
#include "KSkills.h"
#include "KObj.h"
#include "KObjSet.h"
#include "KMath.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "GameDataDef.h"
#include "KSubWorldSet.h"
#include "KRegion.h"
#include "KNpcTemplate.h"
#include "KItemSet.h"
#include "KSortScript.h"
#include "KPlayerSet.h"
#ifdef _SERVER
#include "KSkillManager.h"
#include <iostream>
#include "time.h"
#else
#include "../../Headers/IClient.h"
#include "CoreShell.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "ImgRef.h"
#include "Text.h"
#include "LuaFuns.h"
#endif
#include "KNpcAttribModify.h"
#include "CoreUseNameDef.h"
#include "KSubWorld.h"
#include "Scene/ObstacleDef.h"
#include "KThiefSkill.h"
#ifdef _STANDALONE
#include "KThiefSkill.cpp"
#endif

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

extern KLuaScript		*g_pNpcLevelScript;

#define	ATTACKACTION_EFFECT_PERCENT		60
#define	MIN_JUMP_RANGE					0
#define	ACCELERATION_OF_GRAVITY			10

#define		SHOW_CHAT_WIDTH				24
#define		SHOW_CHAT_COLOR				0xffffffff
#define		SHOW_BLOOD_COLOR			0x00ff0000
#define		defMAX_SHOW_BLOOD_TIME		100
#define		defSHOW_BLOOD_MOVE_SPEED	0.5

#define		SHOW_LIFE_WIDTH				38
#define		SHOW_LIFE_HEIGHT			4

#define		SHOW_SPACE_HEIGHT			5
// --
#define	GAME_UPDATE_TIME		10
#define	GAME_SYNC_LOSS			100
#define	STAMINA_RECOVER_SCALE	4
#define	REGIONWIDTH			REGION_CELL_WIDTH
#define	REGIONHEIGHT		REGION_CELL_HEIGHT

#define	CELLWIDTH			(REGION_CELL_SIZE_X << 10)
#define	CELLHEIGHT			(REGION_CELL_SIZE_Y << 10)
#define	CURREGION			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex]

#define	LEFTREGIONIDX		CURREGION.m_nConnectRegion[2]
#define	RIGHTREGIONIDX		CURREGION.m_nConnectRegion[6]
#define	UPREGIONIDX			CURREGION.m_nConnectRegion[4]
#define	DOWNREGIONIDX		CURREGION.m_nConnectRegion[0]
#define	LEFTUPREGIONIDX		CURREGION.m_nConnectRegion[3]
#define	LEFTDOWNREGIONIDX	CURREGION.m_nConnectRegion[1]
#define	RIGHTUPREGIONIDX	CURREGION.m_nConnectRegion[5]
#define	RIGHTDOWNREGIONIDX	CURREGION.m_nConnectRegion[7]

#define	LEFTREGION			SubWorld[m_SubWorldIndex].m_Region[LEFTREGIONIDX]
#define	RIGHTREGION			SubWorld[m_SubWorldIndex].m_Region[RIGHTREGIONIDX]
#define	UPREGION			SubWorld[m_SubWorldIndex].m_Region[UPREGIONIDX]
#define	DOWNREGION			SubWorld[m_SubWorldIndex].m_Region[DOWNREGIONIDX]
#define	LEFTUPREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTUPREGIONIDX]
#define	LEFTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTDOWNREGIONIDX]
#define	RIGHTUPREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTUPREGIONIDX]
#define	RIGHTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTDOWNREGIONIDX]

#define	CONREGION(x)		SubWorld[m_SubWorldIndex].m_Region[CURREGION.m_nConnectRegion[x]]
#define	CONREGIONIDX(x)		CURREGION.m_nConnectRegion[x]
#define BROADCAST_REGION(pBuff,uSize,uMaxCount)	 if(m_SubWorldIndex >= 0 && m_SubWorldIndex < MAX_SUBWORLD && SubWorld[m_SubWorldIndex].m_SubWorldID != -1) SubWorld[m_SubWorldIndex].BroadCastRegion((pBuff), (uSize), (uMaxCount), m_RegionIndex, m_MapX, m_MapY);
#define POS_EDITION_PERCENT				40
#define	STUN_STATE_SPR					"\\spr\\skill\\²¹³ä\\mag_spe_Ñ£ÔÎ.spr"
#define defSEC_MOVE_BLUR_TIME			72

KNpc	Npc[MAX_NPC];

KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL][MAX_SERIES];		//


// --
//
// --
KNpc::KNpc()
{
#ifdef _SERVER
	m_AiSkillRadiusLoadFlag = 0;
#endif
	Init();
}

// --
//
// --
void KNpc::Init()
{
	memset(m_btStateInfo, 0, sizeof(m_btStateInfo));//moi them by kinnox;
	m_dwID					= 0;
	m_Index					= 0;
	m_nPlayerIdx			= 0;
	m_ProcessAI				= 1;
	m_Kind					= kind_normal;
	m_CustomKind			= kind_dialoger;
	m_Series				= series_metal;
	m_Camp					= camp_free;
	m_CurrentCamp			= camp_free;
	m_CustomCamp			= camp_free;
	m_Doing					= do_stand;
	m_Height				= 0;
	m_Frames.nCurrentFrame	= 0;
	m_Frames.nTotalFrame	= 0;
	m_SubWorldIndex			= 0;
	m_RegionIndex			= -1;
	m_Experience			= 0;
	m_ActiveSkillID			= 0;
	m_SkillParam1			= 0;
	m_SkillParam2			= 0;
	m_nReducedDamge			= 0;

	m_cGold.Release();

	//them chu khong biet no hoat dong hieu qua khong? by kinnox 24/03/2024;
	ZeroMemory(&m_Command, sizeof(m_Command));
	m_SkillList.Clear();
	memset(&m_PhysicsDamage, 0, sizeof(m_PhysicsDamage));
	memset(&m_CurrentPoisonDamage, 0, sizeof(m_CurrentPoisonDamage));
	memset(&m_MagicPhysicsDamage, 0, sizeof(m_MagicPhysicsDamage));
	memset(&m_LifeState, 0, sizeof(m_LifeState));
	memset(&m_ManaState, 0, sizeof(m_ManaState));
	//
#ifndef _SERVER
	m_nChatContentLen		= 0;
	m_nCurChatTime			= 0;
	m_nChatNumLine			= 0;
	m_nChatFontWidth		= 0;
	m_nStature				= 0;
#else
	m_nImmortal				= 0;
#endif

	m_CurrentLife = 100;			
	m_CurrentLifeMax = 100;		
	m_CurrentLifeReplenish = 0;	
	m_CurrentMana = 100;			
	m_CurrentManaMax = 100;		
	m_CurrentManaReplenish = 0;	
	m_CurrentStamina = 100;		
	m_CurrentStaminaMax = 100;	
	m_CurrentStaminaGain = 0;	
	m_CurrentStaminaLoss = 0;	
	m_CurrentAttackRating = 100;	
	m_CurrentDefend = 10;		
	m_CurrentWalkSpeed = 5;		
	m_CurrentRunSpeed = 10;		
	m_CurrentJumpSpeed = 12;	
	m_CurrentJumpFrame = 40;	
	m_CurrentAttackSpeed = 0;	
	m_CurrentCastSpeed = 0;		
	m_CurrentVisionRadius = 40;	
	m_CurrentAttackRadius = 30;	
	m_CurrentHitRecover = 0;	
	m_CurrentAddPhysicsDamage = 0;	
	
	m_Dir = 0;					
	m_JumpStep = 0;
	m_JumpDir = 0;			
	m_MapZ = 0;					
	m_HelmType = 1;				
	m_ArmorType = 1;			
	m_WeaponType = 1;			
	m_HorseType = -1;			
	m_bRideHorse = FALSE;
	m_wMaskType 				= 0;		
	m_wMaskMark 				= 0;
	m_MantleType				= -1;
	m_byMantleLevel				= 0;
	ZeroMemory(Name, sizeof(Name));		
	m_NpcSettingIdx = 0;		
	m_CorpseSettingIdx = 0;
	ZeroMemory(ActionScript,32);
	m_ActionScriptID			= 0;
	m_TrapScriptID				= 0;
	m_TimerScriptID				= 0;

	m_btRankId					= 0;
	m_LifeMax					= 100;
	m_LifeReplenish				= 0;
	m_ManaMax					= 100;
	m_ManaReplenish				= 0;		
	m_StaminaMax				= 100;		
	m_StaminaGain				= 0;		
	m_StaminaLoss				= 0;		
	m_AttackRating				= 100;		
	m_Defend					= 10;		
	m_WalkSpeed					= 6;		
	m_RunSpeed					= 10;		
	m_JumpSpeed					= 12;		
	m_AttackSpeed				= 0;		
	m_CastSpeed					= 0;		
	m_VisionRadius				= 40;		
	m_DialogRadius				= 124;		
	m_HitRecover				= 12;		
	m_nPeopleIdx				= 0;

	m_LoopFrames				= 0;
	m_WalkFrame					= 12;
	m_RunFrame					= 15;
	m_StandFrame				= 15;
	m_DeathFrame				= 15;
	m_HurtFrame					= 10;
	m_AttackFrame				= 20;
	m_CastFrame					= 20;
	m_SitFrame					= 15;
	m_JumpFrame					= 40;
	m_AIMAXTime					= 25;
	m_NextAITime				= 0;
	m_ProcessState				= 1;
	m_ReviveFrame				= 100;
#ifdef _SERVER
	m_nSitRegenTicks			= 0;
	m_nSitRegenLastLog			= 0;
#endif	  
	m_bExchangeServer			= FALSE;
	m_bActivateFlag				= FALSE;
	m_FightMode					= 0;
	m_OldFightMode				= 0;
#ifdef _SERVER
	m_pDropRate = NULL;
	ZeroMemory(CustomName, sizeof(CustomName));
	memset(m_nParam, 0, sizeof(int) * 5);
	m_nTotalReceiveDamage		= 0;
	m_bRemoveWhenDeath			= false;
	m_TimerTask.SetOwner(this);
#endif

#ifndef _SERVER
	m_SyncSignal				= 0;
	m_sClientNpcID.m_dwRegionID	= 0;
	m_sClientNpcID.m_nNo		= -1;
	m_ResDir					= 0;
	m_btPKFlag					= 0;
	m_nSleepFlag				= 0;
	memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));

	memset(m_btBloodAlpha, 0, sizeof(m_btBloodAlpha));
	memset(m_btBloodTime, 0, sizeof(m_btBloodTime));
	memset(m_szBloodNo, 0, sizeof(m_szBloodNo));

	m_nTongFlag					= 0;
	m_nWorldStat				= 0;
	m_bOpenShop					= FALSE;
	ZeroMemory(m_szShopAdv, sizeof(m_szShopAdv));
	m_bEditPos					= FALSE;
	memset(m_btTimeFrame, 0, sizeof(BYTE) * 3);
	memset(m_btFrameSpr	, 0, sizeof(BYTE) * 3);
	m_nSecMoveBlurNo			= 0;
	m_nSecMoveBlurTime			= -1;
	m_cPKValue					= 0;
	m_nRepute					= 0;
	m_nFuyuan					= 0;
	m_cTransLife				= 0;
	m_ImagePlayer				= 0; //Player Avatar by kinnox;	
	ZeroMemory(m_szTongName, sizeof(m_szTongName));
	ZeroMemory(m_szTongTitle, sizeof(m_szTongTitle));
	m_wTitleId					= 0;
	m_bWarning					= FALSE;//AutoAI by kinnox;;
#endif

	m_nLastPoisonDamageIdx = 0;
	m_nLastDamageIdx = 0;
	m_bHaveLoadedFromTemplate = FALSE;
	m_bClientOnly = FALSE;
	m_bWaitingPlayerFeedBack	= 0;	//
}


//

ISkill* KNpc::GetActiveSkill()
{
	_ASSERT(m_ActiveSkillID < MAX_SKILL);
	int nCurLevel = m_SkillList.GetCurrentLevel(m_ActiveSkillID);
	int nPointAdd = m_SkillList.GetPointAdd(m_ActiveSkillID);
	if (nCurLevel > 0)
		return g_SkillManager.GetSkill(m_ActiveSkillID, nCurLevel + nPointAdd);
	else 
		return NULL;
};


//

void KNpc::SetCurrentCamp(int nCamp)
{
	m_CurrentCamp = nCamp;

#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};	
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;

	int	nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}


//

void KNpc::SetCamp(int nCamp)
{
	m_Camp = nCamp;

#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCAMP_SYNC	NetCommand;

	NetCommand.ProtocolType = (BYTE)s2c_npcchgcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_Camp;

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}


//

void KNpc::RestoreCurrentCamp()
{
	m_CurrentCamp = m_Camp;

#ifdef _SERVER
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

#define		NPC_SHOW_CHAT_TIME		15000
int		IR_IsTimePassed(unsigned int uInterval, unsigned int& uLastTimer);


//

void KNpc::Activate()
{
	if (!m_Index)
	{
		return;
	}

	if (m_bExchangeServer)
	{
		return;
	}

	if (m_bActivateFlag)
	{
		m_bActivateFlag = FALSE;
		return;
	}

	m_LoopFrames++;
#ifdef _SERVER
	if( IsPlayer() )
	{
		if( Player[m_nPlayerIdx].IsLoginDelaySync() )
			return;

		SYSTEMTIME systm;
		GetLocalTime(&systm);
		if (m_nImmortal > 0)
		{
			if (m_nImmortal == systm.wSecond)
				m_nImmortal = 0;
		}
		//PK 10 move to prison
        if(g_SubWorldSet.GetGameTime()% 18 == m_nPlayerIdx % 18)
        {
                if (SubWorld[m_SubWorldIndex].m_SubWorldID != 208 && Player[m_nPlayerIdx].m_cPK.GetPKValue() >= 10)
                {
                        ChangeWorld(208,1585*32,3210*32);
                }
        }
       //
	}
	m_TimerTask.Activate(g_NpcTimerCallBackFun);
#endif
//Kenny fix Skill
	if (m_ProcessState)
{
    if (ProcessState() && (m_Command.CmdKind != do_skill || m_Command.Param_X != 90))
    return;
   /* {
#ifdef _SERVER
        if (!(m_LoopFrames % GAME_UPDATE_TIME))
            MaintainAura();
#endif
        return;
    }*/
}
	if (m_ProcessAI)
	{
		NpcAI.Activate(m_Index);
	}

	ProcCommand(m_ProcessAI);
	ProcStatus();	
	if(m_Kind == kind_player)
	{
		if (m_wMaskType > 0 && m_wMaskMark && m_wMaskMark != m_wMaskType)
		{
			ReSetRes(FALSE);
			m_wMaskMark = 0;
		}
		else if(m_wMaskType > 0 && !m_wMaskMark)
		{
			ReSetRes(TRUE);
			m_wMaskMark = m_wMaskType;
		}
		else if (m_wMaskType == 0 && m_wMaskMark)
		{
			ReSetRes(FALSE);
			m_wMaskMark = 0;
		}
	}

#ifdef _SERVER
	this->m_cDeathCalcExp.Active();
#endif

#ifndef _SERVER
	HurtAutoMove();	
	
	if (m_RegionIndex == -1)
		return;

	int		nMpsX, nMpsY;
	
	if(m_Kind == kind_player)
	{
		if (m_wMaskMark > 0)
		{
			// FIX: Validate mask ID before calling GetFrameCopyFromTemplate
			// This code runs EVERY FRAME - if mask ID invalid, will crash repeatedly
			// Bug: No validation before accessing g_pNpcTemplate[m_wMaskType][1][0]
			if (m_wMaskType > 0 && m_wMaskType < MAX_NPCSTYLE)
			{
				GetFrameCopyFromTemplate(m_wMaskType, 1, 0);
			}
			else
			{
				// Invalid mask - clear it to prevent repeated crashes
				m_wMaskType = 0;
				m_wMaskMark = 0;
			}
		}
	}
	AutoFixXY();//kinnox
	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
	m_DataRes.SetMantle(m_MantleType);
	m_DataRes.SetState(&m_StateSkillList, &g_NpcResList);

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index)
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, TRUE);
	}
	else
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, FALSE);
	}
	
	// Di hinh 
	if(m_nSecMoveBlurTime > 0)
	{	
		if(m_nSecMoveBlurTime < 63)
		{
			if(m_nSecMoveBlurTime % 4 != 0)
			{
				for(short i = 0; i < m_nSecMoveBlurNo; i ++)
				{
					m_DataRes.RemoveBlurSecMove(i);
				}
			}
		}
		m_nSecMoveBlurTime--;
	}
	if(m_nSecMoveBlurTime == 0)
	{
		m_DataRes.SetBlur(FALSE);
		m_nSecMoveBlurNo = 0;
		m_nSecMoveBlurTime = -1;	
	}
	// End 

	if (m_Kind == kind_bird || m_Kind == kind_mouse)
	{
		m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}

	if (m_nChatContentLen > 0)
	{
		if (IR_GetCurrentTime() - m_nCurChatTime > NPC_SHOW_CHAT_TIME)
		{
			m_nChatContentLen = 0;
			m_nChatNumLine = 0;
			m_nChatFontWidth = 0;
			m_nCurChatTime = 0;
		}
	}

#endif
}

//fix
#ifndef _SERVER
void	KNpc::AutoFixXY()
{
	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex ].IsPlayer())
	return;
	
		if ((m_sSyncPos.m_nDoing == do_stand
			|| m_sSyncPos.m_nDoing == do_magic
			|| m_sSyncPos.m_nDoing == do_attack
			|| m_sSyncPos.m_nDoing == do_runattack
			|| m_sSyncPos.m_nDoing == do_manyattack
			|| m_sSyncPos.m_nDoing == do_jumpattack
			) 
			&& (m_Doing == do_run || m_Doing == do_walk))
		{	
			int	nRegionIdx;

				if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
				{
					SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
					m_MapX = m_sSyncPos.m_nMapX;
					m_MapY = m_sSyncPos.m_nMapY;
					m_OffX = m_sSyncPos.m_nOffX;
					m_OffY = m_sSyncPos.m_nOffY;
					memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
					SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
				}
				else
				{
					nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
					if (nRegionIdx < 0)
						return;
					SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
					SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
					m_RegionIndex = nRegionIdx;
					m_dwRegionID = m_sSyncPos.m_dwRegionID;
					m_MapX = m_sSyncPos.m_nMapX;
					m_MapY = m_sSyncPos.m_nMapY;
					m_OffX = m_sSyncPos.m_nOffX;
					m_OffY = m_sSyncPos.m_nOffY;
					memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
				}
		}
}
#endif
//

void KNpc::ProcStatus()
{
	if (m_bExchangeServer)
		return;

	switch(m_Doing)
	{
	case do_stand:
		OnStand();
		break;
	case do_run:
		OnRun();
		break;
	case do_walk:
		OnWalk();
		break;
	case do_attack:
	case do_magic:
		OnSkill();
		break;
	case do_sit:
		OnSit();
		break;
	case do_jump:
		OnJump();
		break;
	case do_hurt:
		OnHurt();
		break;
	case do_revive:
		OnRevive();
		break;
	case do_death:
		OnDeath();
		break;
	case do_defense:
		OnDefense();
		break;
	case do_special1:
		OnSpecial1();
		break;
	case do_special2:
		OnSpecial2();
		break;
	case do_special3:
		OnSpecial3();
		break;
	case do_special4:
		OnSpecial4();
		break;
	case do_manyattack:
		OnManyAttack();
		break;
	case do_runattack:
		OnRunAttack();
		break;
	case do_jumpattack:
		OnJumpAttack();
		break;
	case do_idle:
		OnIdle();
	default:
		break;
	}
}


//

void KNpc::ProcCommand(int nAI)
{
	if (m_Command.CmdKind == do_none || m_bExchangeServer)
		return;

	if(nAI)
	{
#ifdef _SERVER
		// FIX: CH? block AI auto-fight commands (du?i theo mob), KH?NG block user click chu?t
		// User click chu?t ? m_nPeopleIdx == 0 v? m_nObjectIdx == 0
		// Auto-fight ? m_nPeopleIdx > 0 ho?c m_nObjectIdx > 0
		if (m_Doing == do_sit && IsPlayer()) {
			if (m_Command.CmdKind == do_walk || m_Command.CmdKind == do_run || m_Command.CmdKind == do_jump) {
				// CH? block n?u dang du?i theo target (auto-fight)
				if (m_nPeopleIdx > 0 || m_nObjectIdx > 0) {
					char szMsg[256];
					sprintf(szMsg, "[PROCCOMMAND-BLOCKED] Player %s AI cmd=%d BLOCKED (auto-fight) tick=%d PeopleIdx=%d ObjIdx=%d",
						Name, m_Command.CmdKind, m_nSitRegenTicks, m_nPeopleIdx, m_nObjectIdx);
					g_DebugLog(szMsg);
					m_Command.CmdKind = do_none;
					return;
				}
				// KHÔNG block n?u không du?i ai (user click chu?t) ? cho phép d?ng d?y và di chuy?n
			}
		}
#endif
		if(m_RegionIndex < 0)
			return;
		switch (m_Command.CmdKind)
		{
		case do_stand:
			DoStand();
			break;
		case do_walk:
			Goto(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_run:
			RunTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_jump:
			JumpTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_skill:
			if (int nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))
			{
				SetActiveSkill(nSkillIdx);
				DoSkill(m_Command.Param_Y, m_Command.Param_Z);
			}
		else
			{
				DoStand();
#ifndef _SERVER
				SendClientPos2Server();// them by kinnox 27/03/2024;
#endif
			}
			break;
		case do_sit:
			DoSit();
			break;
		case do_defense:
			DoDefense();
			break;
		case do_idle:
			DoIdle();
			break;
		case do_hurt:
			DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
		case do_revive:
			DoStand(); 
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		}
	}
	else
	{
		switch(m_Command.CmdKind)
		{
		case do_hurt:
			if(m_RegionIndex >= 0)
				DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
		case do_revive:
			DoStand(); 
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		case do_glide:
			GlideTo(m_Command.Param_X, m_Command.Param_Y, (short)m_Command.Param_Z);
			break;
		default:
			break;
		}
	}
	m_Command.CmdKind = do_none;
}


//

BOOL KNpc::ProcessState()
{
	int nRet = FALSE;
	if (m_RegionIndex < 0)
		return FALSE;
#ifdef _SERVER
	if (m_IgnoreNegativeState.nTime > 0)
	{	
		m_PoisonState.nTime = 0;
		m_FreezeState.nTime = 0;
		m_StunState.nTime = 0;
		m_Poison2DecManaState.nTime = 0;
		m_RandMoveState.nTime = 0;
		m_IgnoreNegativeState.nTime--;
	}
	
	if(IsPlayer())
	{
		Player[this->m_nPlayerIdx].ProcessEnhance();
	}

	if (!(m_LoopFrames % GAME_UPDATE_TIME))
	{
		UpdateNpcStateInfo();
		BroadCastState();
	}
	if(!(m_LoopFrames % 5))
	{
		// Broadcast position if currently moving
		if (m_Doing == do_run || m_Doing == do_walk)
		{
			BroadCastPosition();
		}
		if(!IsPlayer() && m_nTotalReceiveDamage > 0)
		{
			SendBloodNo(m_dwID, m_nTotalReceiveDamage);
			m_nTotalReceiveDamage = 0;
		}
	}
#endif
	if (!(m_LoopFrames % GAME_UPDATE_TIME))
	{
#ifdef _SERVER
		
		// FIX: Sitting regeneration - works for all characters
		if (m_Doing == do_sit)
		{
			m_nSitRegenTicks++;
			
			// Life regeneration: 0.3% of max per second, boosted by LifeReplenishPercent		
			int nLifeAdd = (m_CurrentLifeMax * 3 * (100 + m_LifeReplenishPercent)) / 100000;
			if (nLifeAdd <= 0)
				nLifeAdd = 1;

			m_CurrentLife += nLifeAdd;
			if (m_CurrentLife > m_CurrentLifeMax)
				m_CurrentLife = m_CurrentLifeMax;

			// Mana regeneration: 0.3% of max per second
			int nManaAdd = m_CurrentManaMax * 3 / 1000;
			if (nManaAdd <= 0)
				nManaAdd = 1;

			m_CurrentMana += nManaAdd;
			if (m_CurrentMana > m_CurrentManaMax)
				m_CurrentMana = m_CurrentManaMax;

			// Stamina regeneration: 2x normal rate when sitting
			
			int nStaminaAdd = m_CurrentStaminaGain * 2;
			if (nStaminaAdd <= 0)
				nStaminaAdd = 1; // Works even without faction

				m_CurrentStamina += nStaminaAdd;
			if (m_CurrentStamina > m_CurrentStaminaMax)
				m_CurrentStamina = m_CurrentStaminaMax;
			
		}
		else
		{
			
			m_nSitRegenTicks = 0;
			m_nSitRegenLastLog = 0;
		}
		if(m_CurrentLifeReplenish < 0)
		{
			if(m_CurrentLife < (-m_CurrentLifeReplenish))
				m_CurrentLife = 0;
			else
				m_CurrentLife += m_CurrentLifeReplenish;
		}
		else
			m_CurrentLife += m_CurrentLifeReplenish;
	
		if (m_CurrentLife > m_CurrentLifeMax)
			m_CurrentLife = m_CurrentLifeMax;
		
		if(m_CurrentManaReplenish < 0)
		{
			if(m_CurrentMana < (-m_CurrentManaReplenish))
				m_CurrentMana = 0;
			else
				m_CurrentMana += m_CurrentManaReplenish;
		}
		else
			m_CurrentMana += m_CurrentManaReplenish;
		if (m_CurrentMana > m_CurrentManaMax)
			m_CurrentMana = m_CurrentManaMax;
		

		// Stamina regeneration for non-sitting states (sitting handled above with 2x bonus)
		if (m_Doing == do_run)
			m_CurrentStamina += m_CurrentStaminaGain / STAMINA_RECOVER_SCALE;
		else if (m_Doing != do_sit)
			m_CurrentStamina += m_CurrentStaminaGain;

		if (m_CurrentStamina > m_CurrentStaminaMax)
			m_CurrentStamina = m_CurrentStaminaMax;
		else if(m_CurrentStamina <= 0)
			m_CurrentStamina = 0;
#endif

		if (m_ActiveAuraID)
		{
			int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
			int nPointAdd = m_SkillList.GetPointAdd(m_ActiveAuraID);
			if (nLevel > 0)
			{
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				_ASSERT(m_ActiveAuraID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
#ifdef _SERVER
				NPC_SKILL_SYNC SkillCmd;
				SkillCmd.ID = this->m_dwID;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel + nPointAdd);
				if (pOrdinSkill)
                {
					SkillCmd.nSkillID = pOrdinSkill->GetChildSkillId();
                }
				else
                {
					SkillCmd.nSkillID = 0;
                }

				SkillCmd.nSkillLevel = nLevel + nPointAdd;	
				SkillCmd.nMpsX = -1;
				SkillCmd.nMpsY = m_dwID;
				SkillCmd.ProtocolType = s2c_castskilldirectly;
				SkillCmd.m_bAuto	= FALSE;
				POINT	POff[8] = 
				{
					{0, 32},
					{-16, 32},
					{-16, 0},
					{-16, -32},
					{0, -32},
					{16, -32},
					{16, 0},
					{16, 32},
				};
				
				int nMaxCount = MAX_BROADCAST_COUNT;
				CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
				int i;
				for (i = 0; i < 8; i++)
				{
					if (CONREGIONIDX(i) == -1)
						continue;
					CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
				}
#endif				
				KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel + nPointAdd);
				int nChildSkillId = 0;
				if (pOrdinSkill1)
				{
					nChildSkillId = pOrdinSkill1->GetChildSkillId();
                    
					KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel + nPointAdd);
					if (pOrdinSkill2)
                    {
						pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY);
                    }
				}
			}
		}
	//Kenny fix skill
		/*#ifdef _SERVER
		if (!(m_LoopFrames % GAME_UPDATE_TIME))
			MaintainAura();
		#endif
		*/
		if (m_AppendSkill[0])
		{
			int nMpsX, nMpsY;
			SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
			
			int i = 0;
			for (i = 0; i < 5; i++)
			{
				if (m_ActiveAuraID != 712) 
					continue;

				if (m_AppendSkill[i])
				{	
					int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
					int nPointAdd = m_SkillList.GetPointAdd(m_ActiveAuraID);
					nLevel += nPointAdd;

					if (m_SkillList.GetCurrentLevel(m_AppendSkill[i]) < nLevel)
						nLevel = m_SkillList.GetCurrentLevel(m_AppendSkill[i]);

					if (nLevel > 0)
					{
						_ASSERT(m_AppendSkill[i] < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	#ifdef _SERVER
						NPC_SKILL_SYNC SkillCmd;
						SkillCmd.ID = this->m_dwID;

						KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_AppendSkill[i], nLevel + nPointAdd);
						if (pOrdinSkill)
						{
							SkillCmd.nSkillID = pOrdinSkill->GetChildSkillId();
						}
						else
						{
							SkillCmd.nSkillID = 0;
						}
						
						SkillCmd.nSkillLevel = nLevel + nPointAdd;
						SkillCmd.nMpsX = -1;
						SkillCmd.nMpsY = m_dwID;
						SkillCmd.ProtocolType = s2c_castskilldirectly;
						SkillCmd.m_bAuto	= FALSE;
						
						POINT	POff[8] = 
						{
							{0, 32},
							{-16, 32},
							{-16, 0},
							{-16, -32},
							{0, -32},
							{16, -32},
							{16, 0},
							{16, 32},
						};
						 
						int nMaxCount = MAX_BROADCAST_COUNT;
						CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
						int j;
						for (j = 0; j < 8; j++)
						{
							if (CONREGIONIDX(j) == -1)
								continue;
							CONREGION(j).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
						}					
#endif
						KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_AppendSkill[i], nLevel + nPointAdd);
						int nChildSkillId = 0;
						if (pOrdinSkill1)
						{
							nChildSkillId = pOrdinSkill1->GetChildSkillId();
							
							KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel + nPointAdd);
							if (pOrdinSkill2)
							{
								pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY);
							}
						}
					}
				}
			}
		}
	}
#ifdef _SERVER	
	if (m_PoisonState.nTime > 0)
	{
		if( (NpcSet.GetRelation(m_nLastPoisonDamageIdx, this->m_Index) & relation_enemy) && (this->m_CurrentLife > 0) )
		{

			m_PoisonState.nTime--;
			if (m_PoisonState.nValue[1] == 0)
			{
				m_PoisonState.nValue[1] = 1;
			}
			if (!(m_PoisonState.nTime % m_PoisonState.nValue[1]))
			{	
				int nPoisonDamage = 0;
				nPoisonDamage = CalcDamage(m_nLastPoisonDamageIdx, m_PoisonState.nValue[0], m_PoisonState.nValue[0], 0, damage_poison, FALSE, FALSE, TRUE);

				if(nPoisonDamage > 0 && !IsPlayer())
				{
					Npc[m_nLastPoisonDamageIdx].SendBloodNo(m_dwID, nPoisonDamage);
				}
			}
		}
		else
		{
			this->m_PoisonState.nTime = 0;
		}
	}

	if (m_FreezeState.nTime > 0)
	{	
		m_FreezeState.nTime--;
		if (m_FreezeState.nTime & 1)
		{
			nRet = TRUE;
		}
	}
	if (m_StunState.nTime > 0)
	{
		m_StunState.nTime--;
		nRet = TRUE;
	}

	if (m_LifeState.nTime > 0)
	{
		m_LifeState.nTime--;
		//if (!(m_LifeState.nTime % GAME_UPDATE_TIME)) // loi len mau nhanh
		if ((m_LifeState.nTime % GAME_UPDATE_TIME) == 5)
		{
			m_CurrentLife += m_LifeState.nValue[0] + (m_LifeState.nValue[0] * m_LifeReplenishPercent / 100);
			if (m_CurrentLife > m_CurrentLifeMax)
			{
				m_CurrentLife = m_CurrentLifeMax;
			}
			else if(m_CurrentLife <= 0)
			{
				m_CurrentLife = 0;
			}
			if(m_LifeState.nValue[0] < 0)
			{
				if(m_CurrentLife == 0)
				{
					m_LifeState.nTime = 0;
				}
			}
		}
	}
	if (m_ManaState.nTime > 0)
	{
		m_ManaState.nTime--;
		//if (!(m_ManaState.nTime % GAME_UPDATE_TIME)) // loi len mana nhanh
		if ((m_ManaState.nTime % GAME_UPDATE_TIME) == 5)
		{
			m_CurrentMana += m_ManaState.nValue[0];
			if (m_CurrentMana > m_CurrentManaMax)
			{
				m_CurrentMana = m_CurrentManaMax;
			}
			if(m_ManaState.nValue[0] < 0)
			{
				if(m_CurrentMana < 0)
				{
					m_CurrentMana = 0;
					m_ManaState.nTime = 0;
				}
			}
		}
	}
	if (m_DrunkState.nTime > 0)
	{
		m_DrunkState.nTime--;
	}
	if (m_Poison2DecManaState.nTime > 0)
	{	
		m_Poison2DecManaState.nTime--;
		if (!(m_Poison2DecManaState.nTime % GAME_UPDATE_TIME))
		{
			m_CurrentMana -= m_CurrentMana * m_Poison2DecManaState.nValue[0] / 100;
			if (m_CurrentMana < 0)
				m_CurrentMana = 0;
		}
	}

	if (m_HideState.nTime > 0)
	{
		m_HideState.nTime--;
	}

	if (m_RandMoveState.nTime > 0)
	{
		m_RandMoveState.nTime--;
		if (!(m_RandMoveState.nTime % GAME_UPDATE_TIME))
		{
			int nRaX = ::GetRandomNumber(0,100);
			int nRaY = ::GetRandomNumber(0,100); 
			int nPosX = 0; 
			int nPosY = 0; 
			int nIsX = ::GetRandomNumber(0,3);
			int nIsY = ::GetRandomNumber(0,3);
			if (nIsX > 1)
				nRaX = - nRaX;
			if (nIsY > 1)
				nRaY = - nRaY;
			GetMpsPos(&nPosX, &nPosY);	
			RandMove(nPosX + nRaX , nPosY + nRaY);
		}
	} 
#endif

#ifndef _SERVER
	bool bAdjustColorId = false;
	if (m_FreezeState.nTime > 0)
	{
		if (SubWorld[0].m_dwCurrentTime & 1)
			nRet = TRUE;
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_freeze);
		bAdjustColorId = true;
	}
	
	if (m_StunState.nTime > 0)
	{
		m_DataRes.SetSpecialSpr(STUN_STATE_SPR);
		nRet = TRUE;
		bAdjustColorId = true;
	}

	if (m_PoisonState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_poison);
		bAdjustColorId = true;
	}

	if (!bAdjustColorId)
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_physics);
#endif
	
	KSkill * pOrdinSkill = NULL;
	KStateNode* pNode = NULL;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	//fix by kinnox;
	KStateNode* pTempNode = NULL;
	//moi them by kinnox kiem tra pNode;
	while(pNode)
	{	
		//KStateNode* pTempNode = pNode; // bo doan nay;
		pTempNode = pNode;//them
		pNode = (KStateNode *)pNode->GetNext();
		
		if(m_IgnoreNegativeState.nTime > 0)
		{
			pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(pTempNode->m_SkillID, pTempNode->m_Level);
			if (pOrdinSkill)
			{	
				if(pOrdinSkill->IsTargetEnemy())
				{
					if(pOrdinSkill->GetChildSkillId() == BUFF_SKILL_MISSLE_ID_1 || 
						pOrdinSkill->GetChildSkillId() == BUFF_SKILL_MISSLE_ID_2)
					{
						pTempNode->m_LeftTime = 0 ;
					}
				}
			}
		}

		if (pTempNode->m_LeftTime == -1)
			continue;

		if (pTempNode->m_LeftTime == 0)
		{
			int i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
			delete pTempNode;
#ifdef _SERVER
			UpdateNpcStateInfo();
			BroadCastState();
#endif
			pTempNode = NULL;
			continue;
		}
		else
			pTempNode->m_LeftTime--;
	}
	return nRet;

}


//

void KNpc::DoDeath(int nMode/* = 0*/,int nAttacker) //chia exp team by kinnox;
{

	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	if (IsPlayer() && !m_FightMode)
	{
		m_CurrentLife = 1;
		return;
	}

#ifndef _SERVER
	if (this->m_Kind == kind_normal)
		this->SetBlood(this->m_CurrentLife);
#endif

	m_Doing = do_death;
	m_ProcessAI	= 0;
	m_ProcessState = 0;
	m_Frames.nTotalFrame = m_DeathFrame;
	m_Frames.nCurrentFrame = 0;
	
	m_Height = 0;

#ifdef _SERVER	
	int nPlayer = 0;
	//chia exp team by kinnox;
	if (this->m_Kind != kind_player && nAttacker > 0 && nAttacker < MAX_NPC)
	{
		nPlayer = m_cDeathCalcExp.CalcExp(nAttacker);
	}
	//end

	DeathPunish(nMode, nPlayer);

	NPC_DEATH_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcdeath;
	NetCommand.ID			= m_dwID;
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
	m_ClientDoing = cdo_death;
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx == m_Index)
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
	}
#endif
	if (IsPlayer())
	{
		#ifdef _SERVER
		if (!Npc[nAttacker].IsPlayer())
		{
			if (Player[m_nPlayerIdx].m_cTeam.m_nFlag && Player[m_nPlayerIdx].m_cTeam.m_nID >= 0)
			{
			PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
			sLeaveTeam.ProtocolType = c2s_teamapplyleave;
			Player[m_nPlayerIdx].LeaveTeam((BYTE*)&sLeaveTeam);
			}
		}
		#endif
	}
}


//

void KNpc::OnDeath()
{
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
#ifndef _SERVER
		int		nTempX, nTempY;
		KObjItemInfo	sInfo;

		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nTempX, &nTempY);
		sInfo.m_nItemID = 0;
		sInfo.m_dwItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		//AutoAI by kinnox;
		sInfo.m_nGenre = 0;
		sInfo.m_nDetailType = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_dwNpcId = 0;
		//end code;
		sInfo.m_szName[0] = 0;
		ObjSet.ClientAdd(0, m_CorpseSettingIdx, 0, m_Dir, 0, nTempX, nTempY, sInfo);
		m_ProcessAI = 1;
#else
		// them by kinnox 
		if (IsPlayer() && Npc[m_nLastDamageIdx].IsPlayer())
        {
                char szMessageDeath[128];
                if (m_nPlayerIdx > 0)
                {
                        sprintf(szMessageDeath,"§¹i hiÖp ®· bÞ %s ®¸nh träng th­¬ng",Npc[m_nLastDamageIdx].Name);
                        KPlayerChat::SendSystemInfo(1, m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, szMessageDeath,strlen(szMessageDeath) );
                        if (Player[m_nPlayerIdx].m_cTeam.m_nFlag && Player[m_nPlayerIdx].m_cTeam.m_nID >= 0)
                        {
                                PLAYER_APPLY_LEAVE_TEAM        sLeaveTeam;
                                sLeaveTeam.ProtocolType = c2s_teamapplyleave;
                                Player[m_nPlayerIdx].LeaveTeam((BYTE*)&sLeaveTeam);
                        }
                }
                if (Npc[m_nLastDamageIdx].m_nPlayerIdx > 0)
                {
                sprintf(szMessageDeath,"§¹i hiÖp ®· ®¸nh träng th­¬ng %s",Name);
                KPlayerChat::SendSystemInfo(1, Npc[m_nLastDamageIdx].m_nPlayerIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, szMessageDeath, strlen(szMessageDeath) );
                }         
        }
       //end

		if (IsPlayer())
		{
			
			if (Player[m_nPlayerIdx].m_dwDeathScriptId)
			{
				if(Npc[m_nLastDamageIdx].IsPlayer())
				{
					Player[m_nPlayerIdx].ExecuteScript2(Player[m_nPlayerIdx].m_dwDeathScriptId, "OnDeath", m_nPlayerIdx, Npc[m_nLastDamageIdx].m_nPlayerIdx);
				}
				else
				{
					Player[m_nPlayerIdx].ExecuteScript(Player[m_nPlayerIdx].m_dwDeathScriptId, "OnDeath", m_nPlayerIdx);
				}
			}
			Player[m_nPlayerIdx].ProcessRemoveEnhance();//Xoa nhan doi by kinnox 30/04/2024;
		}
		else if (Npc[m_nLastDamageIdx].IsPlayer())
		{
			int nIdx = Npc[m_nLastDamageIdx].m_nPlayerIdx;
			if (nIdx)
			{
				if (m_ActionScriptID)
				{
					ExecuteScript2(m_ActionScriptID, "OnDeath", m_Index, m_nLastDamageIdx);		 // dang lam
				}
			}
		}

		/*if (!IsPlayer())
		{
			if (m_bRemoveWhenDeath)
			{
				if (m_RegionIndex > 0)
				{
					SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);
					SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
					NpcSet.Remove(m_Index);
				}
				return;
			}
		}*/
#endif
		if (!IsPlayer())
		{
			if(m_cGold.GetGoldType() > 0)
				this->m_cGold.RecoverBackData();
			
		}

		if (m_Kind != kind_partner)
		{
			DoRevive();
#ifndef _SERVER
			if (m_Kind != kind_player)
			{
				SubWorld[0].m_WorldMessage.Send(GWM_NPC_DEL, m_Index);
				return;
			}
#endif		
		}
		else
		{

		}
	}
	else
	{

	}
}


//

void KNpc::DoDefense()
{
	m_ProcessAI = 0;
}


//

void KNpc::OnDefense()
{
}


//

void KNpc::DoIdle()
{
	if (m_Doing == do_idle)
		return;
	m_Doing = do_idle;
}


//

void KNpc::OnIdle()
{
}


//

void KNpc::DoHurt(int nHurtFrames /*= 0*/, int nX /*= 0*/, int nY /*= 0*/) 
{
#ifdef _SERVER
	if (m_nImmortal > 0) 
		return;
#endif
	if (m_Doing == do_hurt || m_Doing == do_death || m_Doing == do_revive || m_Doing == do_runattack)
	return;

	//_ASSERT(m_RegionIndex >= 0);
#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	if (m_RegionIndex < 0)
		return;
		
	if (m_IgnoreNegativeState.nValue[1] > 1)
		return;

#ifdef _SERVER

#define	MIN_HURT_PERCENT	50	
	if (g_RandPercent(MIN_HURT_PERCENT + m_CurrentHitRecover * (100 - MIN_HURT_PERCENT) / 100))
	{
		return;
	}
#endif
	m_Doing = do_hurt;
	m_ProcessAI	= 0;

#ifdef _SERVER
	m_Frames.nTotalFrame = m_HurtFrame * (100 - m_CurrentHitRecover) / 100;
#else
	m_ClientDoing = cdo_hurt;
	m_Frames.nTotalFrame = m_HurtFrame * (100 - m_CurrentHitRecover) / 100;;
	m_nHurtDesX = nX;
	m_nHurtDesY = nY;
	if (m_Height > 0)
	{
		m_nHurtHeight = m_Height;
	}
	else
	{
		m_nHurtHeight = 0;
	}
#endif
	if (m_Frames.nTotalFrame == 0)
		m_Frames.nTotalFrame = 1;
	m_Frames.nCurrentFrame = 0;

#ifdef _SERVER
	NPC_HURT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npchurt;
	NetCommand.ID = m_dwID;
	NetCommand.nFrames = m_Frames.nTotalFrame;
	GetMpsPos(&NetCommand.nX, &NetCommand.nY);

POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}


//

void KNpc::OnHurt()
{
	if (m_RegionIndex < 0)
	{
		//g_DebugLog("[error]%s Region Index < 0 when hurt", Name);
		return;
	}
	int nX, nY;
	GetMpsPos(&nX, &nY);
#ifdef _SERVER
	m_Height = 0;
#endif
#ifndef _SERVER
	m_Height = m_nHurtHeight * (m_Frames.nTotalFrame - m_Frames.nCurrentFrame - 1) / m_Frames.nTotalFrame;
	nX = nX + (m_nHurtDesX - nX) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;
	nY = nY + (m_nHurtDesY - nY) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;

	int nOldRegion = m_RegionIndex;
	CURREGION.DecRef(m_MapX, m_MapY, obj_npc);
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	nRegion = -1;
	nMapX = nMapY = nOffX = nOffY = 0;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
	{
		 // fix remove region by kinnox;
		int nCheckIndexNpcinRegion = SubWorld[0].m_Region[nOldRegion].SearchNpc(m_dwID);
		if (nCheckIndexNpcinRegion> 0)
		{
			if(nCheckIndexNpcinRegion == m_Index && m_Doing != do_death && m_Doing != do_hurt && m_Doing != do_glide && m_Doing != do_revive) 
			{	
				SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
				m_dwRegionID = 0;
			}
		}
		//end code
	}
	else if (nOldRegion != nRegion)
	{
		m_RegionIndex = nRegion;
		m_MapX = nMapX;
		m_MapY = nMapY;
		m_OffX = nOffX;
		m_OffY = nOffY;
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index); 
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	if (nRegion >= 0)
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
#endif

	if (WaitForFrame())
	{
		//g_DebugLog("[DEATH]On Hurt Finished");
		DoStand();
		m_ProcessAI = 1;
	}
}


//

void KNpc::DoSpecial1()
{
	DoBlurAttack();
}


//

void KNpc::OnSpecial1()
{
	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
#ifndef _SERVER
	DoStand();
	SendClientPos2Server();// them by kinnox 27/03/2024;
#endif
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (pSkill)
		{
			int nChildSkill = pSkill->GetChildSkillId();
			int nChildSkillLevel = pSkill->m_ulLevel;
			
			if (nChildSkill > 0)
			{
				KSkill * pChildSkill = (KSkill*)g_SkillManager.GetSkill(nChildSkill, nChildSkillLevel);
				if (pChildSkill)
				{
					pChildSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
		}

		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
}


//

void KNpc::DoSpecial2()
{
}


//

void KNpc::OnSpecial2()
{

	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		ISkill * pSkill = GetActiveSkill();
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Thief:
			{
				( (KThiefSkill*)pSkill )->OnSkill(this);
			}
			break;
		} 
		
		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
	
}


//

void KNpc::DoSpecial3()
{
}


//

void KNpc::OnSpecial3()
{
}


//

void KNpc::DoSpecial4()
{
}


//

void KNpc::OnSpecial4()
{
}


//

BOOL KNpc::DoGlide(short nLenght)
{
#ifdef _SERVER
	if (m_nImmortal > 0) 
		return FALSE;
#endif
	if(nLenght <= 0)
		return FALSE;

	if(m_RegionIndex < 0)
		return FALSE;
	
	if(m_Doing == do_glide)
		return FALSE;
	
	m_Doing = do_glide;
	m_Dir = m_JumpDir;
	m_ProcessAI	= 0;

#ifndef _SERVER
	int nSrcX;
	int nSrcY;
	SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nSrcX, &nSrcY);
#endif

	int nRegion, nMapX, nMapY, nOffX, nOffY;
	//
	SubWorld[m_SubWorldIndex].Mps2Map(m_DesX, m_DesY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	//
	if(nOffX < 0)
	{
		nMapX--;
		nOffX += CELLWIDTH;
	}
	else if(nOffX > CELLWIDTH)
	{
		nMapX++;
		nOffX -= CELLWIDTH;
	}
	//
	if(nOffY < 0)
	{
		nMapY--;
		nOffY += CELLHEIGHT;
	}
	else if(nOffY > CELLHEIGHT)
	{
		nMapY++;
		nOffY -= CELLHEIGHT;
	}
	//
	bool bEc = false;
	//
	if(nMapX < 0 && nMapY < 0)
	{
		nMapX += REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = LEFTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY < 0)
	{
		nMapX -= REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = RIGHTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX < 0 && nMapY >= REGIONHEIGHT)
	{
		nMapX += REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = LEFTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY >= REGIONHEIGHT  )
	{
		nMapX -= REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = RIGHTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(bEc)
	{	
		if(nRegion >= 0) 
		{
			if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
			{
				DoStand();
				m_ProcessAI	= 1;
	#ifndef _SERVER
				if( IsPlayer() )
				{
					SendClientPos2Server();
				}
				if(m_bEditPos)
				{
					m_bEditPos = false;
				}
	#else
				 if( IsPlayer() )
				 {	
					 Player[m_nPlayerIdx].StopMove();
					 BroadCastPosition();
				 }
	#endif
				 return FALSE;
			}
#ifdef _SERVER
			if( IsPlayer() )
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
#endif
			MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);	
		}
		
	}
	else // bEc = false
	{
		if(nMapX < 0)
		{
			nMapX += REGIONWIDTH;
			nRegion = LEFTREGIONIDX;
		}
		else if(nMapX >= REGIONWIDTH)
		{
			nMapX -= REGIONWIDTH;
			nRegion = RIGHTREGIONIDX;
		}
		//
		if(nRegion >= 0)
		{
			if(nMapY < 0)
			{
				nMapY += REGIONHEIGHT;
				nRegion = UPREGIONIDX;
			}
			else if(nMapY >= REGIONHEIGHT)
			{
				nMapY -= REGIONHEIGHT;
				nRegion = DOWNREGIONIDX;
			}
			if(nRegion >= 0) 
			{
				if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
				{
					DoStand();
					m_ProcessAI	= 1;
#ifndef _SERVER
					if( IsPlayer() )
					{
						SendClientPos2Server();
					}
					if(m_bEditPos)
					{
						m_bEditPos = false;
					}
#else
					 if( IsPlayer() )
					 {	
						 Player[m_nPlayerIdx].StopMove();
						 BroadCastPosition();
					 }
#endif				
					 return FALSE;
				}
#ifdef _SERVER
				if( IsPlayer() )
				{
					Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
				}
#endif
				MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);
			}
			
		}
	}
#ifndef _SERVER
	// Start Blur
	m_DataRes.SetBlur(TRUE);
	m_nSecMoveBlurNo = nLenght / 10;
	short nRatioX = (m_DesX - nSrcX) / m_nSecMoveBlurNo;
	short nRatioY = (m_DesY - nSrcY) / m_nSecMoveBlurNo;
	int nTempX = nSrcX;
	int nTempY = nSrcY;
	short i;
	for(i = 0; i < m_nSecMoveBlurNo; i++)
	{
		m_DataRes.SetBlurSecMove(m_Index, nTempX, nTempY, m_Height);
		nTempX += nRatioX;
		nTempY += nRatioY;
	}
	m_nSecMoveBlurTime = defSEC_MOVE_BLUR_TIME; 
	// End Blur
#endif
#ifdef _SERVER
	NPC_GLIDE_SYNC	NetCommand;
	NetCommand.ProtocolType = s2c_npcglide;
	NetCommand.m_dwID		= m_dwID;
	NetCommand.m_nMpsX		= m_DesX;
	NetCommand.m_nMpsY		= m_DesY;
	NetCommand.m_nLenght	= nLenght;
		
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
	DoStand();
	m_ProcessAI	= 1;
	return FALSE;
}


//

void KNpc::OnGlide()
{
}


//

void KNpc::GlideTo(int nMpsX, int nMpsY, short nLenght)
{
	if(NewGlide(nMpsX, nMpsY, nLenght))
	{
		DoGlide(nLenght);
	}
	else
	{
		RunTo(nMpsX, nMpsY);
	}
}


//

void KNpc::DoStand()
{
	m_Frames.nTotalFrame = m_StandFrame;
	if (m_Doing == do_stand)
	{
		return;
	}
	else
	{
		m_Doing = do_stand;
	#ifdef _SERVER
			// Reset sit tick counter khi dung day
			m_nSitRegenTicks = 0;
	#endif
		m_Frames.nCurrentFrame = 0;
		GetMpsPos(&m_DesX, &m_DesY);
#ifndef _SERVER
		if (m_FightMode)
			m_ClientDoing = cdo_fightstand;
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
		m_DataRes.StopSound();
#endif
	}
}


//

void KNpc::OnStand()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightstand;
		}
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
#endif
	}
}


//

void KNpc::DoRevive()
{
	if (m_RegionIndex < 0)
		return;
	
	if (m_Doing == do_revive)
	{
		return;
	}
	else
	{
#ifdef _SERVER
		if ( m_bRemoveWhenDeath == TRUE )
		{
			SubWorld[m_SubWorldIndex].m_WorldMessage.Send(GWM_NPC_DEL, m_Index, m_dwID);
			m_bRemoveWhenDeath = FALSE;
			return ;
		}	
#endif
		m_Doing = do_revive;
		m_ProcessAI = 0;
		m_ProcessState = 0;
	
		m_nPeopleIdx = 0;                    // theo sau
		m_nObjectIdx = 0;                    // obj theo
		m_nLastDamageIdx = 0;                // player lastdame

		ClearStateSkillEffect();
		ClearNormalState();

#ifdef _SERVER
		if(IsPlayer())
			return;
		this->m_nTotalReceiveDamage = 0;
		m_Frames.nTotalFrame = m_ReviveFrame;
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		SubWorld[m_SubWorldIndex].NpcChangeRegion(m_RegionIndex, VOID_REGION, m_Index);
		m_Frames.nCurrentFrame = 0;		
#else
		if(IsPlayer())
		{	
			CoreDataChanged(GDCNI_WAIT_TO_REVIVE, NULL, NULL);
		}
		m_Frames.nTotalFrame = m_DeathFrame;
		m_ClientDoing = cdo_death;
#endif
	}
}


//

void KNpc::OnRevive()
{
#ifdef _SERVER
	if (!IsPlayer() && WaitForFrame())
	{
		Revive();
	}
#else
	m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
#endif
}


//

void KNpc::DoRun()
{
	_ASSERT(m_RegionIndex >= 0);	
	if (m_CurrentRunSpeed)
		m_Frames.nTotalFrame = (m_RunFrame * m_RunSpeed) / m_CurrentRunSpeed;
	else
		m_Frames.nTotalFrame = m_RunFrame;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightrun;
	}
	else
	{
		m_ClientDoing = cdo_run;
	}
#endif
	if (IsPlayer())
	{
		if(Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2 && this->m_FightMode == 1)
		{
			if (!Cost(attrib_stamina, m_CurrentStaminaLoss))
			{
				DoWalk();
				return;
			}
		}
	}

#ifdef _SERVER

	NPC_RUN_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcrun;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
	if (m_Doing == do_run)
	{
		return;
	}
	m_Doing = do_run;

	m_Frames.nCurrentFrame = 0;
}

// --
//
// --
void KNpc::OnRun(BOOL bAddSpeed /*= FALSE*/)
{	
	if(m_Doing == do_hurt || m_Doing == do_death || m_Doing == do_revive)
		DoStand();

	WaitForFrame();
	
	if(bAddSpeed)
	{	
		ProcMove(REGION_CELL_SIZE_X - 1);
		CheckTrap();
	}
	else
	{
		if(m_Doing == do_runattack)
		{	
			m_CurrentRunSpeed += 65;
			ProcMove(m_CurrentRunSpeed);
			m_CurrentRunSpeed -= 65;
		}
		else
		{
			ProcMove(m_CurrentRunSpeed);
		}
	}
}


//

void KNpc::DoSit()
{
	_ASSERT(m_RegionIndex >= 0);

	if (m_Doing == do_sit)
	{
		return;
	}

	m_Doing = do_sit;
	
#ifdef _SERVER	
	NPC_SIT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcsit;
	NetCommand.ID = m_dwID;
		
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};

	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
		m_ClientDoing = cdo_sit;
#endif

	m_Frames.nTotalFrame = m_SitFrame;
	m_Frames.nCurrentFrame = 0;
}

//

void KNpc::OnSit()
{
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
	}
}
//	
void KNpc::DoSkill(int nX, int nY)
{
	_ASSERT(m_RegionIndex >= 0);

	if(m_Doing == do_skill)
		return;
	if( IsPlayer() )
	{
		if(!m_FightMode)
		{
			UnDoSkill();
			return;
		}
#ifdef _SERVER
		Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeAttack);
#endif
	}

	if( !m_SkillList.CanCast(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime) )
	{
		UnDoSkill();
		return;
	}

	ISkill* pSkill = GetActiveSkill();
	if(!pSkill)
	{
		UnDoSkill();
		return;
	}
	
	if( !pSkill->CanCastSkill(m_Index, nX, nY) )
	{
		UnDoSkill();
		return;
	}

	if(m_Kind == kind_player)
	{
		if( !Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this)) )
		{
			UnDoSkill();
			return;
		}
	}
	eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
	
#ifdef _SERVER
	NPC_SKILL_SYNC	NetCommand;
	
	NetCommand.ProtocolType = (BYTE)s2c_skillcast;
	NetCommand.ID			= m_dwID;
	NetCommand.nSkillID		= m_ActiveSkillID;
	NetCommand.nSkillLevel	= m_SkillList.GetCurrentLevel(m_ActiveSkillID) + m_SkillList.GetPointAdd(m_ActiveSkillID);
	
	if(nY <= 0) 
	{
		DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
		return;
	}
	
	NetCommand.nMpsX = nX;
	if(nX == -1) 
	{
		NetCommand.nMpsY = Npc[nY].m_dwID;
		if(NetCommand.nMpsY == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return;
	}
	else
	{
		NetCommand.nMpsY = nY;
	}
	NetCommand.m_bAuto	= FALSE;
	m_SkillParam1 = nX;
	m_SkillParam2 = nY;
	m_DesX = nX;
	m_DesY = nY;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	m_HideState.nTime = 0;
#endif
	if(eStyle == SKILL_SS_Missles 
		|| eStyle == SKILL_SS_Melee 
		|| eStyle == SKILL_SS_InitiativeNpcState 
		|| eStyle == SKILL_SS_PassivityNpcState)
	{
		DoOrdinSkill((KSkill *) pSkill, nX, nY);
	}
	else
	{
		switch(eStyle)
		{
		case SKILL_SS_Thief:
			{
				((KThiefSkill*)pSkill)->DoSkill(this, nX, nY);

			}
			break;
		default:
			{
				// UnDoSkill(); kinnox bo
			}
			break;
		}
	}
} 


//

void KNpc::UnDoSkill()
{
	m_nPeopleIdx = 0;
	m_nObjectIdx = 0;
	DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
}


//	

int KNpc::DoOrdinSkill(KSkill * pSkill, int nX, int nY)
{
	_ASSERT(pSkill);

#ifndef _SERVER		
	m_DataRes.StopSound();
	int x, y, tx, ty;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	
	if (nY < 0)
		return 0;
	
	if (nX < 0)
	{
		if (nX != -1) 
			return 0;
		
		if (nY >= MAX_NPC || Npc[nY].m_dwID == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return 0;
		Npc[nY].GetMpsPos(&tx, &ty);
	}
	else
	{
		tx = nX;
		ty = nY;
	}
	
	m_SkillParam1 = nX;
	m_SkillParam2 = nY;
	m_DesX = nX;
	m_DesY = nY;
	
	m_Dir = g_GetDirIndex(x, y, tx, ty);
	if (pSkill->GetPreCastEffectFile()[0])
		m_DataRes.SetSpecialSpr((char *)pSkill->GetPreCastEffectFile());
	
	if (IsPlayer())
		pSkill->PlayPreCastSound(m_nSex,x, y);
	
	if (pSkill->IsNeedShadow())		
		m_DataRes.SetBlur(TRUE);
	else
		m_DataRes.SetBlur(FALSE);
#endif
	
	CLIENTACTION ClientDoing = pSkill->GetActionType();
	
#ifndef _SERVER
	if (ClientDoing >= cdo_count) 
		m_ClientDoing = cdo_magic;
	else if (ClientDoing != cdo_none)
		m_ClientDoing = ClientDoing;
#endif
	if(pSkill->GetSkillStyle() == SKILL_SS_Melee)
	{
		if(CastMeleeSkill(pSkill) == FALSE)
		{
			m_nPeopleIdx = 0;
			m_nObjectIdx = 0;
			m_ProcessAI = 1;
			DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
			return 1;
		}
	}
	else if( pSkill->IsPhysical() )
	{
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
		{	// m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed); kinnox fix toc do danh tren 65%;
			int nTotalFrame = m_AttackFrame * MAX_PERCENT / (m_CurrentAttackSpeed + MAX_PERCENT);
			m_Frames.nTotalFrame = nTotalFrame - nTotalFrame % 2;
			if (m_Frames.nTotalFrame <= 0)
				m_Frames.nTotalFrame = 1;
		}
#ifndef _SERVER
		if (g_Random(3))
			m_ClientDoing = cdo_attack;
		else 
			m_ClientDoing = cdo_attack1;
#endif
		m_Doing = do_attack;
	}
	else
	{
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
		{
			// m_Frames.nTotalFrame = m_CastFrame * 100 / (m_CurrentCastSpeed + 100); kinnox fix toc do danh tren 65%;
			int nTotalFrame = m_CastFrame * MAX_PERCENT / (m_CurrentCastSpeed + MAX_PERCENT);
			m_Frames.nTotalFrame = nTotalFrame - nTotalFrame % 2;
			if (m_Frames.nTotalFrame <= 0)
				m_Frames.nTotalFrame = 1;
		}
		m_Doing  = do_magic;
	}
	
	m_ProcessAI = 0;
	m_Frames.nCurrentFrame = 0;
	return 1;
}


//

void KNpc::DoAttack()
{
	if(m_Doing == do_attack)
		return;

#ifndef _SERVER
	if(g_Random(2) == 1)
	{
		m_ClientDoing = cdo_attack;
	}
	else
	{
		m_ClientDoing = cdo_attack1;
	}
#endif

	m_ProcessAI = 0;
	m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_attack;
}


//

BOOL	KNpc::CastMeleeSkill(KSkill * pSkill)
{
	BOOL bSuceess = FALSE;
	_ASSERT(pSkill);
	
	switch(pSkill->GetMeleeType())
	{
	case Melee_AttackWithBlur:
		{
			bSuceess = DoBlurAttack();
		}
		break;
	case Melee_Jump:
		{
			if (NewJump(m_DesX, m_DesY))
			{
				DoJump();
				bSuceess = TRUE;
			}
			
		}
		break;
	case Melee_JumpAndAttack:
		{
			if (m_DesX < 0 && m_DesY > 0) 
			{
				int x, y;
				SubWorld[m_SubWorldIndex].Map2Mps
					(
					Npc[m_DesY].m_RegionIndex,
					Npc[m_DesY].m_MapX, 
					Npc[m_DesY].m_MapY, 
					Npc[m_DesY].m_OffX, 
					Npc[m_DesY].m_OffY, 
					&x,
					&y
					);
				
				m_DesX = x + 1;
				m_DesY = y;
			}

			if (NewJump(m_DesX, m_DesY))
			{
				DoJumpAttack();
				bSuceess = TRUE;
			}

		}
		break;
	case Melee_RunAndAttack:
		{
			bSuceess = DoRunAttack();
		}
		break;
	case Melee_ManyAttack:
		{
			bSuceess = DoManyAttack();
		}
		break;
	case Melee_Move:
		{	
			int nLenght = NewGlide(m_DesX, m_DesY, pSkill->GetParam1());
			if(nLenght > 0)
			{
				
				if(m_bRideHorse)
				{
					if(pSkill->GetDelayPerCastOnHorse() > 0)
					{
						m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCastOnHorse());
						m_SkillList.SetMaxCastTime(m_ActiveSkillID, pSkill->GetDelayPerCastOnHorse());
					}
				}
				else
				{	
					if(pSkill->GetDelayPerCast() > 0)
					{
						m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());
						m_SkillList.SetMaxCastTime(m_ActiveSkillID, pSkill->GetDelayPerCast());
					}
				}				
				bSuceess = DoGlide((short)nLenght);
			}
		}
		break;
	default:
		m_ProcessAI = 1;
		break;
	}
	return bSuceess;

}


//

BOOL KNpc::DoBlurAttack()
{
	if (m_Doing == do_special1)
		return FALSE;
	
	KSkill * pSkill = (KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	_ASSERT(pSkill->GetSkillStyle() == SKILL_SS_Melee);

#ifndef _SERVER
		m_ClientDoing = pSkill->GetActionType();
		m_DataRes.SetBlur(TRUE);
#endif

	m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_special1;
	return TRUE;
}

void KNpc::DoMagic()
{
	if (m_Doing == do_magic)
		return;

	m_ProcessAI = 0;
#ifndef _SERVER
	m_ClientDoing = cdo_magic;
#endif
	m_Frames.nTotalFrame = m_CastFrame * 100 / (m_CurrentCastSpeed + 100);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_magic;
}


//

void KNpc::OnSkill()
{
	KSkill* pSkill = NULL;
	if(WaitForFrame() && m_Frames.nTotalFrame != 0)
	{
		DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif	
		m_ProcessAI = 1;	
	}
	else if(IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif

		if(m_DesX == -1) 
		{
			if(m_DesY <= 0) 
			{
				if(m_Frames.nTotalFrame == 0)
				{
					m_ProcessAI = 1;
				}
				return;
			}
			
			if(Npc[m_DesY].m_RegionIndex < 0) 
			{
				if(m_Frames.nTotalFrame == 0)
				{
					m_ProcessAI = 1;
				}
				return;
			}
		}
		
		
		pSkill = (KSkill*)GetActiveSkill();
		if(pSkill)
		{
			pSkill->Cast(m_Index, m_DesX, m_DesY);
			if(pSkill->GetDelayPerCast() > 0)
			{
				m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());
				m_SkillList.SetMaxCastTime(m_ActiveSkillID, pSkill->GetDelayPerCast());
			}
		}
	}
}


//

void KNpc::JumpTo(int nMpsX, int nMpsY)
{

	// FIX: N?u dang ng?i, ph?i DoStand() TRU?C khi DoJump()
	// Ð? g?i packet stand v? client, tránh desync
	if (m_Doing == do_sit && IsPlayer()) {
		DoStand();
	}

	if (NewJump(nMpsX, nMpsY))
		DoJump();
	else
	{
		RunTo(nMpsX, nMpsY);
	}
}


//

void KNpc::RunTo(int nMpsX, int nMpsY)
{

	// FIX: N?u dang ng?i, ph?i DoStand() TRU?C khi DoRun()
	// Ð? g?i packet stand v? client, tránh desync
	if (m_Doing == do_sit && IsPlayer()) {
		DoStand();
	}

	if(NewPath(nMpsX, nMpsY))
		DoRun();
}


//

void KNpc::Goto(int nMpsX, int nMpsY)
{

	
	if (m_Doing == do_sit && IsPlayer()) {
		DoStand();
	}

	if (NewPath(nMpsX, nMpsY))
		DoWalk();
}


//

void KNpc::DoWalk()
{
	_ASSERT(m_RegionIndex >= 0);

	if (m_CurrentWalkSpeed)
		m_Frames.nTotalFrame = (m_WalkFrame * m_WalkSpeed) / m_CurrentWalkSpeed + 1;
	else
		m_Frames.nTotalFrame = m_WalkFrame;
#ifdef _SERVER
	NPC_WALK_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcwalk;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;
	
	if(m_RandMoveState.nTime > 0)
	{
		NetCommand.bRandMove = TRUE;
	}
	else
	{
		NetCommand.bRandMove = FALSE;
	}

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}	
#endif

	if (m_Doing == do_walk)
	{
		return;
	}
	m_Doing = do_walk;
	m_Frames.nCurrentFrame = 0;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightwalk;
	}
	else
	{
		m_ClientDoing = cdo_walk;
	}
#endif
	
}


//

void KNpc::DoPlayerTalk(char * szTalk)
{
#ifdef _SERVER
	_ASSERT(m_RegionIndex >= 0);
	int nTalkLen = strlen(szTalk);
	if (!nTalkLen) return;
	BYTE * pNetCommand = new  BYTE[nTalkLen + 6 + 1];
	pNetCommand[0] = (BYTE)s2c_playertalk;
	*(DWORD *)(pNetCommand + 1) = m_dwID;
	pNetCommand[5] = nTalkLen;
	strcpy((char*)(pNetCommand + 6), szTalk);
	pNetCommand[nTalkLen + 6 ] = '\0';

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	if (pNetCommand)
	{
		delete [] pNetCommand;
		pNetCommand = NULL; //them code 16/09/2023 by kinnox;
	}
#endif

}


//

void KNpc::OnPlayerTalk()
{

}

void KNpc::OnWalk()
{
#ifndef	_SERVER

#endif
	WaitForFrame();
	ProcMove(m_CurrentWalkSpeed);
}


//

int	KNpc::GetSkillLevel(int nSkillId)
{
	int nIndex = m_SkillList.FindSame(nSkillId);
	if (nIndex)
	{
		return m_SkillList.m_Skills[nIndex].SkillLevel;
	}
	else
	{
		return 0;
	}
}


//

void KNpc::ModifyAttrib(int nAttacker, void* pData)
{
	if (pData != NULL)
		g_NpcAttribModify.ModifyAttrib(this, pData);
}

#ifdef _SERVER

//

int KNpc::CalcDamage(int nAttacker, int nMin, int nMax, int nSeriesDamage, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bDoHurt /* TRUE */, BOOL bReturn /* = FALSE */, 
					  BOOL bFatally /*= FALSE*/, int nLifeStole /*= 0*/, int nManaStole /*= 0*/, int nStaminaStole /*= 0*/, int nIdSkills /*= 0*/)
{
	if (!IsAlive() || m_RegionIndex < 0 || (IsPlayer() && m_FightMode == 0))
		return 0;


	if (m_nImmortal > 0 || Npc[nAttacker].m_nImmortal > 0)
		return 0;
	// Tinh damage chi tu
	if (bFatally)
	{	
		int nFatallyDamage = (int)m_CurrentLife / 10;
		m_CurrentLife -= nFatallyDamage;
		return nFatallyDamage;
	}

	if (nMin + nMax <= 0)
		return 0;
	int	nRes = 0;
	int nDamageRange = nMax - nMin;
	int nDamage = 0;
	if (nDamageRange < 0) 
	{
		nDamage = nMax + g_Random(-nDamageRange);
	}
	else
	{
		nDamage = nMin + g_Random(nMax - nMin);
	}
	// Tinh % ngu hanh tuong khac
	int nSeriesPercent = 0;
	switch(Npc[nAttacker].m_Series)
	{
		case 0:
			switch(m_Series)
			{	
				case 1:
					nSeriesPercent -= nSeriesDamage;
					break;
				default:
					break;
			}
			break;
		case 1:
			switch(m_Series)
			{	
				case 4:
					nSeriesPercent -= nSeriesDamage;
					break;
				default:
					break;
			}
			break;
		case 2:
			switch(m_Series)
			{	
				case 3:
					nSeriesPercent -= nSeriesDamage;
					break;
				default:
					break;
			}
			break;
		case 3:
			switch(m_Series)
			{	
				case 0:
					nSeriesPercent -= nSeriesDamage;
					break;
				default:
					break;
			}
			break;
		case 4:
			switch(m_Series)
			{	
				case 2:
					nSeriesPercent -= nSeriesDamage;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	// Tinh khang ngu hanh
	nRes = nSeriesPercent;
	// Tinh khang theo loai damage
	switch(nType)
	{
	case damage_physics:
		nRes += m_CurrentPhysicsResist;
		if (nRes > m_CurrentPhysicsResistMax)
		{
			nRes = m_CurrentPhysicsResistMax;
		}
		else if(nRes < -m_CurrentPhysicsResistMax)
		{
			nRes = -m_CurrentPhysicsResistMax;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		if (bIsMelee)
		{
			nMax = m_CurrentMeleeDmgRetPercent;
		}
		else
		{
			nMax = m_CurrentRangeDmgRetPercent;
		}
		break;
	case damage_cold:
		nRes += m_CurrentColdResist;
		if (nRes > m_CurrentColdResistMax)
		{
			nRes = m_CurrentColdResistMax;
		}
		else if(nRes < -m_CurrentColdResistMax)
		{
			nRes = -m_CurrentColdResistMax;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_fire:
		nRes += m_CurrentFireResist;
		if (nRes > m_CurrentFireResistMax)
		{
			nRes = m_CurrentFireResistMax;
		}
		else if(nRes < -m_CurrentFireResistMax)
		{
			nRes = -m_CurrentFireResistMax;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}

		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_light:
		nRes += m_CurrentLightResist;
		if (nRes > m_CurrentLightResistMax)
		{
			nRes = m_CurrentLightResistMax;
		}
		else if(nRes < -m_CurrentLightResistMax)
		{
			nRes = -m_CurrentLightResistMax;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}
		
		nMax = m_CurrentRangeDmgRetPercent;
		break;
	case damage_poison:
		nRes += m_CurrentPoisonResist;
		if (nRes > m_CurrentPoisonResistMax)
		{
			nRes = m_CurrentPoisonResistMax;
		}
		else if(nRes < -m_CurrentPoisonResistMax)
		{
			nRes = -m_CurrentPoisonResistMax;
		}
		if (nRes > MAX_RESIST)
		{
			nRes = MAX_RESIST;
		}
		
		nMax = m_CurrentRangeDmgRetPercent;
		m_nLastPoisonDamageIdx = nAttacker;
		break;
	case damage_magic:
		nRes = 0;
		break;
	default:
		nRes = 0;
		break;
	}

	if (!nDamage)
		return 0;


	nDamage = nDamage * (100 - nRes) / 100;
	// giam sat thuong cua player vao npc
	if (this->m_nReducedDamge > 0 && Npc[nAttacker].m_Kind == kind_player && this->m_Kind != kind_player)
	{
		if (this->m_nReducedDamge > 100)
			this->m_nReducedDamge = 99;
		nDamage -= (nDamage * m_nReducedDamge) / 100;
	}

	// Tinh luc sat thuong khi PK giua 2 player
	if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind == kind_player && bReturn == FALSE)
	{
		if (nIdSkills > 0)
		{
			if (nIdSkills == 336)// Bang Tung vo anh
			{
				nDamage = (nDamage*8) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 357)// Phi Long Tai Thien
			{
				nDamage = (nDamage) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 359)// Thien Ha Vo Cau
			{
				nDamage = (nDamage*3) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 322)// Pha Thien Tram
			{
				nDamage = (nDamage*10) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 323)// truy tinh truc nguyet
			{
				nDamage = (nDamage*10) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 325)// truy phong quyet
			{
				nDamage = (nDamage*10) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 328)// Tam Nga Te Tuyet
			{
				nDamage = (nDamage) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 372)// Ngao Tuyet Tieu Phong
			{
				nDamage = (nDamage*5) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 365)// Thien Dia Vo Cuc
			{
				nDamage = (nDamage*3) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 368)// Nhan Kiem
			{
				nDamage = (nDamage*5) * NpcSet.m_nPKDamageRate / 100;
			}
			else if (nIdSkills == 361)// Van Long Kich
			{
				nDamage = (nDamage*3) * NpcSet.m_nPKDamageRate / 100;
			}
			else
				nDamage = nDamage/2 * NpcSet.m_nPKDamageRate / 100;
		}
		else
		{
			nDamage = nDamage/2 * NpcSet.m_nPKDamageRate / 100;
		}
	}
	
	// Giam sat thuong Npc xuong
	if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind != kind_player && bReturn == FALSE)
	{	
		if (Npc[nAttacker].m_cGold.GetGoldType() != 1)
		{
			nDamage = (nDamage * 50) / 100;
		}
	}

	////////////////////DIEU CHINH SAT THUONG CAC HE
	// Tang sat thuong cua he kim 40%
	if (Npc[nAttacker].m_Series == 0 && Npc[nAttacker].m_Kind == kind_player)
	{
		nDamage = nDamage + (nDamage * 40) / 100; 
	}
	// Tang sat thuong cua he moc 
	if (Npc[nAttacker].m_Series == 1 && Npc[nAttacker].m_Kind == kind_player)
	{
		nDamage = nDamage*2;
	}
	// Giam sat thuong cua he thuy 60%
	if (Npc[nAttacker].m_Series == 2 && Npc[nAttacker].m_Kind == kind_player)
	{
		nDamage = (nDamage * 60) / 100; 
	}
	// Tang sat thuong cua he hoa 60%
	if (Npc[nAttacker].m_Series == 3 && Npc[nAttacker].m_Kind == kind_player)
	{
		nDamage = nDamage + (nDamage * 60) / 100; 
	}
	// Tang sat thuong cua he tho 50%
	if (Npc[nAttacker].m_Series == 4 && Npc[nAttacker].m_Kind == kind_player)
	{
		nDamage = nDamage + (nDamage * 50) / 100; 
	}

	// Tinh ty le hut
	if (m_CurrentLife > 0) 
	{	
		if(!IsPlayer())
		{		
			int nStole = 0;
			// Hut sinh luc
			if(nDamage > this->m_CurrentLifeMax)
			{
				nStole = m_CurrentLifeMax * nLifeStole / 100;
			}
			else
			{
				nStole = nDamage * nLifeStole / 100;
			}
			
			if ((Npc[nAttacker].m_CurrentLifeMax - Npc[nAttacker].m_CurrentLife) > nStole)
			{	
				Npc[nAttacker].m_CurrentLife += nStole;
			}
			else
			{
				Npc[nAttacker].m_CurrentLife = Npc[nAttacker].m_CurrentLifeMax;
			}
			// Hut noi luc
			if(nDamage > this->m_CurrentLifeMax)
			{
				nStole = m_CurrentLifeMax * nManaStole / 100;
			}
			else
			{
				nStole = nDamage * nManaStole / 100;
			}

			if ((Npc[nAttacker].m_CurrentManaMax - Npc[nAttacker].m_CurrentMana) > nStole)
			{
				Npc[nAttacker].m_CurrentMana += nStole;
			}
			else
			{
				Npc[nAttacker].m_CurrentMana = Npc[nAttacker].m_CurrentManaMax;
			}
			// Hut the luc
			if(nDamage > this->m_CurrentLifeMax)
			{
				nStole = m_CurrentLifeMax * nStaminaStole / 100;
			}
			else
			{
				nStole = nDamage * nStaminaStole / 100;
			}

			if ((Npc[nAttacker].m_CurrentStaminaMax - Npc[nAttacker].m_CurrentStamina) > nStole)
			{
				Npc[nAttacker].m_CurrentStamina += nStole;
			}
			else
			{
				Npc[nAttacker].m_CurrentStamina = Npc[nAttacker].m_CurrentStaminaMax;
			}
		}
	}
	// Tinh phan don sat thuong
	if (nAttacker > 0 && bReturn == FALSE)
	{
		if (bIsMelee)
		{	
			if (nType != damage_poison)
			{
				nMin = m_CurrentMeleeDmgRet;
				nMin += nDamage * nMax / 100;
				

				if(nMin > 0)
				{
					// Noi luc ho than phan don %
					if (Npc[nAttacker].m_ManaShield > 0)
					{	
						int nManaDamageA = nMin * Npc[nAttacker].m_ManaShield / 100;
						if (nManaDamageA < Npc[nAttacker].m_CurrentMana)
						{
							Npc[nAttacker].m_CurrentMana -= nManaDamageA;
							nMin -= nManaDamageA;
						}
						else
						{
							nMin -= Npc[nAttacker].m_CurrentMana;
							Npc[nAttacker].m_CurrentMana = 0;       
						}
					}
					
					// Khang phan don sat thuong
					if(Npc[nAttacker].m_CurrentReturnResist > 0)
					{	
						int nResistDamage = nMin * Npc[nAttacker].m_CurrentReturnResist / 100;
						nMin -= nResistDamage;
					}

					int nReturnDamage = 0;
					nReturnDamage = Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, 0, damage_magic, FALSE, FALSE, TRUE);
					if(nReturnDamage > 0 && !Npc[nAttacker].IsPlayer())
					{
						SendBloodNo(Npc[nAttacker].m_dwID, nReturnDamage);
					}
				}
			}
		
		}
		// Phan don tam xa
		else
		{	
			if (nType != damage_poison)
			{	
				nMin = m_CurrentRangeDmgRet;
				nMin += nDamage * nMax / 100;
				
				if(nMin > 0)
				{
					// Noi luc ho than %
					if (Npc[nAttacker].m_ManaShield > 0)
					{	
					
						int nManaDamageA = nMin * Npc[nAttacker].m_ManaShield / 100;
						if (nManaDamageA < Npc[nAttacker].m_CurrentMana)
						{
							Npc[nAttacker].m_CurrentMana -= nManaDamageA;
							nMin -= nManaDamageA;
						}
						else
						{
							nMin -= Npc[nAttacker].m_CurrentMana;
							Npc[nAttacker].m_CurrentMana = 0;       
						}
					}
					// Khang phan don sat thuong
					if(Npc[nAttacker].m_CurrentReturnResist > 0)
					{	
						int nResistDamage = nMin * Npc[nAttacker].m_CurrentReturnResist / 100;
						nMin -= nResistDamage;
					}

					int nReturnDamage = 0;
					nReturnDamage = Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, 0, damage_magic, FALSE, FALSE, TRUE);
					if(nReturnDamage > 0 && !Npc[nAttacker].IsPlayer())
					{
						SendBloodNo(Npc[nAttacker].m_dwID, nReturnDamage);
					}
				}
			}	
		}
	}
	
	// Phan don doc sat
	if (nAttacker > 0 && m_PoisonDamageReturn > 0)
	{	
		if (nType == damage_poison)
		{
			nMin = 0;
			nMin += nDamage * m_PoisonDamageReturn / 100;

			int nReturnDamage = 0;
			nReturnDamage = Npc[nAttacker].CalcDamage(m_Index, nMin, nMin, 0, damage_magic, FALSE, FALSE, TRUE);
			if(nReturnDamage > 0 && !Npc[nAttacker].IsPlayer())
			{
				SendBloodNo(Npc[nAttacker].m_dwID, nReturnDamage);
			}
		}
	}

	// Noi luc ho than %
	int nManaDamage = 0;
	if (m_ManaShield > 0)
	{	
		nManaDamage = nDamage * m_ManaShield / 100;
		m_CurrentMana -= nManaDamage;
		if (m_CurrentMana < 0)
		{
			nDamage -= m_CurrentMana; 
			m_CurrentMana = 0;
		}
		else
		{
			nDamage -= nManaDamage;
		}
	}

	// Noi luc ho than diem
	if(m_DynamicMagicShield > 0)
	{	
		int nStartDamage = 0, nMagicShield = 0;

		nStartDamage = nDamage * 40 / 100;

		nMagicShield = m_DynamicMagicShield / 5;
		if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind == kind_player && bReturn == FALSE)
			nMagicShield = nMagicShield * NpcSet.m_nPKDamageRate / 100;
		
		if(nMagicShield > nStartDamage) 
			nMagicShield = nStartDamage;
		
		if (m_StaticMagicShield > 0)
			nMagicShield = m_CurrentManaMax * m_StaticMagicShield / 100;

		if(nDamage > nMagicShield)
		{
			nDamage -= nMagicShield;
		}
		else
		{
			nDamage = 0;
		}
	}
	
	m_nLastDamageIdx = nAttacker;

	// Tinh Exp thong qua damage
	int nCalcExpDamage = 0;
	if(Npc[nAttacker].m_Kind == kind_player && Npc[nAttacker].m_nPlayerIdx > 0)
	{	
		if(this->m_Kind == kind_normal)
		{
			if(nDamage >= (int)m_CurrentLife)
				nCalcExpDamage = (int)m_CurrentLife;
			else
				nCalcExpDamage = nDamage;
			this->m_cDeathCalcExp.AddDamage(Npc[nAttacker].m_nPlayerIdx, nCalcExpDamage);
		}
		if(this->m_Kind == kind_player)
		{
			if(Player[m_nPlayerIdx].TaskGetSaveVal(TV_CALCDAMAGE) > 0)
			{
				nCalcExpDamage = nDamage + nManaDamage;
				this->m_cDeathCalcExp.AddDamage(Npc[nAttacker].m_nPlayerIdx, nCalcExpDamage);
			}
		}
	}

	if(nDamage > (int)m_CurrentLife)
		nDamage = (int)m_CurrentLife;

	m_CurrentLife -= nDamage;

	if (nDamage > 0)
	{
		m_CurrentMana += m_CurrentDamage2Mana * nDamage / 100;
		if (m_CurrentMana > m_CurrentManaMax)
		{
			m_CurrentMana = m_CurrentManaMax;
		}
		if (bDoHurt) // boom lag vi tri ASSERT thong bao lien tuc;
			DoHurt();
	}

	if (m_CurrentLife <= 0)
	{	
		if (m_AutoDeathSkill.nValue[0] > 0)
		{	
			WORD wSkillId = m_AutoDeathSkill.nValue[0] / 256;
			BYTE btSkillLevel = m_AutoDeathSkill.nValue[0] % 256;
			AutoDoSkill(wSkillId, btSkillLevel, -1, m_Index);
		}
		int nMode = DeathCalcPKValue(nAttacker);
		DoDeath(nMode,nAttacker);

		if (m_Kind == kind_player)
			Player[m_nPlayerIdx].m_cPK.CloseAll();
	}
	return nDamage;
}
#endif

#ifdef _SERVER

//

BOOL KNpc::ReceiveDamage(int nLauncher, BOOL bIsMelee, void *pData, BOOL bUseAR, BOOL bDoHurt, int nDameXG)
{
	if (nLauncher <= 0 || nLauncher >= MAX_NPC)
              	return FALSE;
	if (m_Doing == do_death || m_Doing == do_revive)
		return FALSE;
	if (Npc[nLauncher].m_Doing == do_death || Npc[nLauncher].m_Doing == do_revive)
                return FALSE;
	if (!m_Index || !Npc[nLauncher].m_Index)
		return FALSE;

	if (!pData)
		return FALSE;
	/*if (IsPlayer()) {
    NPC_RELATION rel = NpcSet.GetRelation(nLauncher, m_Index);
    if (rel & relation_enemy) {
        if (!m_FightMode)
            SetFightMode(TRUE);
		}
	}*/

	KMagicAttrib *pTemp = NULL;

	pTemp = (KMagicAttrib *)pData;
	
	if (!pTemp)
		return FALSE;

	// Chinh xac
	int nAr = pTemp->nValue[0];
	pTemp++;
	// Bo qua ne tranh
	int nIgnoreAr = pTemp->nValue[0];
	pTemp++;
	
	if (bUseAR)
	{
		if (!CheckHitTarget(nAr, m_CurrentDefend, nIgnoreAr))
		{	
			if(!IsPlayer())
			{
				Npc[nLauncher].SendBloodNo(m_dwID, 999999999);
			}
			return FALSE;
		}
	}

	//fix tvb
	int nDameXGTT = 0;
	int nDohurdXGTT = 0;

	if (nDameXG == 1)
	{
		nDameXGTT = 300;
		nDohurdXGTT = 60;
	}
	else if (nDameXG == 2)
	{
		nDameXGTT = 250;
		nDohurdXGTT = 50;
	}
	else if (nDameXG == 3)
	{
		nDameXGTT = 250;
		nDohurdXGTT = 20;
	}
	else if (nDameXG == 4)
	{
		nDameXGTT = 250;
		nDohurdXGTT = 30;
	}
	else if (nDameXG == 5) // duong mon bao vu
	{
		nDameXGTT = 320;
	}
	else if(nDameXG == 9)// Vu Da Le Hoa
	{
		nDameXGTT = ::GetRandomNumber(300,550);
	}
	
	//
	UINT nLife = m_CurrentLife;

	int pTemp_PhysicDamage[3] = { 0, 0, 0 }; // Luc danh vat ly
	int pTemp_ColdDamage[3] = { 0, 0, 0 }; // Luc danh bang
	int pTemp_FireDamage[3] = { 0, 0, 0 }; // Luc danh hoa
	int pTemp_LightDamage[3] = { 0, 0, 0 }; // Luc danh loi
	int pTemp_PoisonDamage[3] = { 0, 0, 0 }; // Luc danh doc
	int pTemp_MagicDamage[3] = { 0, 0, 0 }; // Luc danh ...
	int pTemp_SeriesDamage[3] = { 0, 0, 0 }; // Ngu hanh tuong khac
	int pTemp_DeadlyStrike[3] = { 0, 0, 0 }; // Chi mang
	int pTemp_FatallyStrike[3] = { 0, 0, 0 }; // Chi tu
	int pTemp_Stun[3] = { 0, 0, 0 }; // Lam choang
	int pTemp_StoleLife[3] = { 0, 0, 0 };// Hut sinh luc
	int pTemp_StoleMana[3] = { 0, 0, 0 }; // Hut noi luc
	int pTemp_StoleStamina[3] = { 0, 0, 0 }; // Hut the luc
	int pTemp_SkillExp[3] = { 0, 0, 0 }; // Luyen ky nang
	int pTemp_AutoAttackSkill[3] = { 0, 0, 0 }; // Tu dong thuc thi ky nang

	int* pTempArray[] = {
	    pTemp_PhysicDamage,
	    pTemp_ColdDamage,
	    pTemp_FireDamage,
	    pTemp_LightDamage,
	    pTemp_PoisonDamage,
	    pTemp_MagicDamage,
	    pTemp_SeriesDamage,
	    pTemp_DeadlyStrike,
	    pTemp_FatallyStrike,
	    pTemp_Stun,
	    pTemp_StoleLife,
	    pTemp_StoleMana,
	    pTemp_StoleStamina,
	    pTemp_SkillExp,
	    pTemp_AutoAttackSkill
	};

	for (int i = 0; i < sizeof(pTempArray) / sizeof(pTempArray[0]); ++i) {
	    pTempArray[i][0] = pTemp->nValue[0];
	    pTempArray[i][1] = pTemp->nValue[1];
	    pTempArray[i][2] = pTemp->nValue[2];
	    ++pTemp;
	}
		
	// Tinh luyen ky nang

	if (Npc[nLauncher].IsPlayer() && pTemp_SkillExp[2] > 0)
	{
		if (!IsPlayer() || (IsPlayer() && m_FightMode))
		{	
			if(Npc[nLauncher].m_SkillList.IsAddExp(pTemp_SkillExp[1]))
			{
				int nMaxLevel = MAX_LEVEL_SKILL_EXP;
				if(Player[Npc[nLauncher].m_nPlayerIdx].TaskGetSaveVal(TV_TRANSLIFE) > 0)
				{
					nMaxLevel += Player[Npc[nLauncher].m_nPlayerIdx].TaskGetSaveVal(TV_TRANSLIFE);
				}
			
				if (Npc[nLauncher].m_SkillList.GetLevel(pTemp_SkillExp[1]) < nMaxLevel)
				{	
					int	nExpSkill = Player[Npc[nLauncher].m_nPlayerIdx].TaskGetSaveVal(pTemp_SkillExp[2]);
					if(Player[Npc[nLauncher].m_nPlayerIdx].TaskGetSaveVal(TV_ENHANCE_SKILL) > 0)
					{
						nExpSkill += 4;
					}
					nExpSkill += 2;

					Player[Npc[nLauncher].m_nPlayerIdx].TaskSetSaveVal(pTemp_SkillExp[2], nExpSkill);
					if (Player[Npc[nLauncher].m_nPlayerIdx].TaskGetSaveVal(pTemp_SkillExp[2]) >= pTemp_SkillExp[0])
					{
						Player[Npc[nLauncher].m_nPlayerIdx].TaskSetSaveVal(pTemp_SkillExp[2],0);
						Npc[nLauncher].m_SkillList.IncreaseLevel(Npc[nLauncher].m_SkillList.GetSkillByIndex(pTemp_SkillExp[1]),1);
						PLAYER_SKILL_LEVEL_SYNC NewSkill;
						NewSkill.ProtocolType = s2c_playerskilllevel;
						NewSkill.m_nSkillID = pTemp_SkillExp[1];
						NewSkill.m_nSkillLevel = Npc[nLauncher].m_SkillList.GetLevel(pTemp_SkillExp[1]);
						NewSkill.m_nLeavePoint = Player[Npc[nLauncher].m_nPlayerIdx].m_nSkillPoint;
						NewSkill.m_btPointAdd	= Npc[nLauncher].m_SkillList.GetPointAdd(pTemp_SkillExp[1]);
						NewSkill.m_bExpand		= false;
						g_pServer->PackDataToClient(Player[Npc[nLauncher].m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
					}
					
				}
			}
		}
	}
	// Tinh ty le chi mang
	BOOL bDeadly = FALSE;

	if (g_RandPercent(pTemp_DeadlyStrike[0]))
	{	
		bDeadly = TRUE;
	}
	// End
	
	//fix by kinnox;
	if (m_IgnoreNegativeState.nTime > 0)
		bDoHurt = FALSE;

	//
	// Tinh luc danh vat ly
	if(pTemp_PhysicDamage[0] > 0)
	{	
		if(bDeadly)
		{
			pTemp_PhysicDamage[0] *= 2;
			pTemp_PhysicDamage[2] *= 2;
		}
		int nID =0;
		if (nDameXG == 2)//pha thien tram
			nID = 322;
		else if (nDameXG == 3)//truy tinh truc nguyet
			nID = 323;
		else if (nDameXG == 4)//truy phong quyet
			nID = 325;
		if (nID == 0)
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_PhysicDamage[0] * (100 + nDameXGTT)/100, pTemp_PhysicDamage[2] * (100 + nDameXGTT)/100, pTemp_SeriesDamage[0], damage_physics, bIsMelee,1, FALSE, FALSE, pTemp_StoleLife[0], pTemp_StoleMana[0], pTemp_StoleStamina[0]);
		else
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_PhysicDamage[0] * (100 + nDameXGTT)/100, pTemp_PhysicDamage[2] * (100 + nDameXGTT)/100, pTemp_SeriesDamage[0], damage_physics, bIsMelee,1, FALSE, FALSE, pTemp_StoleLife[0], pTemp_StoleMana[0], pTemp_StoleStamina[0],nID);
	}
	// End

	// Tinh luc danh bang
	if(pTemp_ColdDamage[0] > 0)
	{
		//them by kinnox;
		if(bDeadly)
		{
			pTemp_ColdDamage[0] *= 2;
			pTemp_ColdDamage[2] *= 2;
		}
		//
		if (nDameXG == 6) // Bang Tung vo anh
		{
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_ColdDamage[0], pTemp_ColdDamage[2], pTemp_SeriesDamage[0], damage_cold, bIsMelee, 0,0,0,0,0,0,336);
		}
		else if(nDameXG == 9)// Vu Da Le Hoa
		{
			int xDamage = ::GetRandomNumber(5,20);
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_ColdDamage[0]*xDamage, pTemp_ColdDamage[2]*xDamage, pTemp_SeriesDamage[0], damage_cold, bIsMelee, 0);
		}
		else if (nDameXG == 10)// Tam Nga Te Tuyet
		{
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_ColdDamage[0], pTemp_ColdDamage[2], pTemp_SeriesDamage[0], damage_cold, bIsMelee, 0,0,0,0,0,0,328);
		}
		else
		m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_ColdDamage[0], pTemp_ColdDamage[2], pTemp_SeriesDamage[0], damage_cold, bIsMelee, 0);
		// Tinh thoi gian lam cham
		if (m_FreezeState.nTime <= 0 && m_IgnoreNegativeState.nValue[0] <= 1)
		{
			if(pTemp_ColdDamage[1] > 0)
			{
				if(m_CurrentFreezeTimeReducePercent > 77)
					m_CurrentFreezeTimeReducePercent = 77;
				m_FreezeState.nTime = pTemp_ColdDamage[1] * (100 - m_CurrentFreezeTimeReducePercent) / 100;
			}
		}
		// End
	}
	// End 

	// Tinh luc danh hoa
	if(pTemp_FireDamage[0] > 0)
	{
		//them by kinnox;
		if(bDeadly)
		{
			pTemp_FireDamage[0] *= 2;
			pTemp_FireDamage[2] *= 2;
		}
		//
		int nID =0;
		if (nDameXG == 7) // Phi Long Tai Thien
			nID = 357;
		else if (nDameXG == 8) // Thien Ha Vo Cau
			nID = 359;
		else if (nDameXG == 14) // van Long kich
		{	
			nID = 361;
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_FireDamage[0], pTemp_FireDamage[2], pTemp_SeriesDamage[0], damage_fire, bIsMelee, 1,0,0,0,0,0,nID);
		}
		if (nID == 0)
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_FireDamage[0], pTemp_FireDamage[2], pTemp_SeriesDamage[0], damage_fire, bIsMelee, 0);
		else if (nID != 361)
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_FireDamage[0], pTemp_FireDamage[2], pTemp_SeriesDamage[0], damage_fire, bIsMelee, 0,0,0,0,0,0,nID);
	}
	// End 

	// Tinh luc danh loi
	if(pTemp_LightDamage[0] > 0)
	{
		//them by kinnox;
		if(bDeadly)
		{
			pTemp_LightDamage[0] *= 2;
			pTemp_LightDamage[2] *= 2;
		}
		//
		int nID = 0;
		if (nDameXG == 11)// Ngao Tuyet Tieu Phong
			nID = 372;
		else if (nDameXG == 12)// Thien Dia Vo Cuc
			nID = 365;
		else if (nDameXG == 13)// Nhan Kiem Hop Nhat
		{
			nID = 368;
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_LightDamage[0], pTemp_LightDamage[2], pTemp_SeriesDamage[0], damage_light, bIsMelee, 1,0,0,0,0,0,nID);
		}

		if (nID == 0)
		m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_LightDamage[0], pTemp_LightDamage[2], pTemp_SeriesDamage[0], damage_light, bIsMelee, 0);
		else if(nID != 368)
		m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_LightDamage[0], pTemp_LightDamage[2], pTemp_SeriesDamage[0], damage_light, bIsMelee, 0,0,0,0,0,0,nID);
		
	}
	// End

	// Tinh luc danh doc Kenny Fix
	if (pTemp_PoisonDamage[0] > 0)
	{
		if (bDeadly)
		{
			pTemp_PoisonDamage[0] *= 2;
			pTemp_PoisonDamage[0] *= 2;
		}
		if (m_IgnoreNegativeState.nValue[0] <= 1)
		{
			const int baseDur = pTemp_PoisonDamage[2];
			int reduce = m_CurrentPoisonTimeReducePercent;
			if (reduce >  200) reduce = 200;
			if (reduce < -200) reduce = -200;
			int effDur = baseDur;
			if (reduce > 0) {
				effDur = baseDur * (100 - reduce) / 100;
				if (effDur < 1) effDur = 1;
			} else if (reduce < 0) {
				effDur = baseDur * (100 - reduce) / 100;
				if (effDur < 1) effDur = 1;
			}
			this->AddPoisonState(nLauncher, pTemp_PoisonDamage[0], effDur, pTemp_PoisonDamage[2]);
		}
		bDoHurt = TRUE;
		m_nTotalReceiveDamage += CalcDamage(nLauncher,
			pTemp_PoisonDamage[0], pTemp_PoisonDamage[0],
			pTemp_SeriesDamage[0], damage_poison, bIsMelee, bDoHurt);
	}

	// End

	// Tinh luc danh ...
	if(pTemp_MagicDamage[0] > 0)
	{
		//them by kinnox;
		if(bDeadly)
		{
			pTemp_MagicDamage[0] *= 2;
			pTemp_MagicDamage[2] *= 2;
		}
		//
		m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_MagicDamage[0], pTemp_MagicDamage[2], pTemp_SeriesDamage[0], damage_magic, bIsMelee, 0);
	}
	// End

	// Tinh ty le chi tu
	if(pTemp_FatallyStrike[0] > 0)
	{
		if (g_RandPercent(pTemp_FatallyStrike[0]))
		{
			m_nTotalReceiveDamage += CalcDamage(nLauncher, pTemp_MagicDamage[0], pTemp_MagicDamage[2], pTemp_SeriesDamage[0], damage_magic, bIsMelee, bDoHurt, FALSE, TRUE);
		}
	}
	// End 

	// Tinh lam choang
	if(pTemp_Stun[0] > 0 && m_IgnoreNegativeState.nValue[0] <= 1)
	{	
		/*if (g_RandPercent(pTemp_Stun[0]))
		{
			int ReduceStun = 0;
			if (m_CurrentStunTimeReducePercent < 0)
			{
				ReduceStun = m_CurrentStunTimeReducePercent;
			}
			else if (m_CurrentStunTimeReducePercent < 10)
			{
				ReduceStun = 0;
			}
			else if (m_CurrentStunTimeReducePercent < 20)
			{
				ReduceStun = 10;
			}
			else if (m_CurrentStunTimeReducePercent < 30)
			{
				ReduceStun = 20;
			}
			else if (m_CurrentStunTimeReducePercent < 40)
			{
				ReduceStun = 30;
			}
			else if (m_CurrentStunTimeReducePercent < 50)
			{
				ReduceStun = 40;
			}
			else if (m_CurrentStunTimeReducePercent < 60)
			{
				ReduceStun = 50;
			}
			else if (m_CurrentStunTimeReducePercent < 70)
			{
				ReduceStun = 60;
			}
			else if (m_CurrentStunTimeReducePercent < 80)
			{
				ReduceStun = 65;
			}
			else if (m_CurrentStunTimeReducePercent < 90)
			{
				ReduceStun = 70;
			}
			else if (m_CurrentStunTimeReducePercent < 100)
			{
				ReduceStun = 75;
			}
			else if (m_CurrentStunTimeReducePercent < 110)
			{
				ReduceStun = 80;
			}
			else if (m_CurrentStunTimeReducePercent < 120)
			{
				ReduceStun = 82;
			}
			else if (m_CurrentStunTimeReducePercent < 130)
			{
				ReduceStun = 84;
			}
			else if (m_CurrentStunTimeReducePercent < 140)
			{
				ReduceStun = 86;
			}
			else if (m_CurrentStunTimeReducePercent < 150)
			{
				ReduceStun = 88;
			}
			else if (m_CurrentStunTimeReducePercent < 160)
			{
				ReduceStun = 90;
			}
			else if (m_CurrentStunTimeReducePercent < 170)
			{
				ReduceStun = 91;
			}
			else if (m_CurrentStunTimeReducePercent < 180)
			{
				ReduceStun = 92;
			}
			else if (m_CurrentStunTimeReducePercent < 190)
			{
				ReduceStun = 93;
			}
			else if (m_CurrentStunTimeReducePercent < 200)
			{
				ReduceStun = 94;
			}
			else
			{
				ReduceStun = 95;
			}

			m_StunState.nTime = pTemp_Stun[1] * (100 - ReduceStun) / 100;

		}*/
		int ReduceStun = 0;

		if (m_CurrentStunTimeReducePercent <= 200)
		{
			ReduceStun = (m_CurrentStunTimeReducePercent/20)*10;
		}
		else
		{
			ReduceStun = 200;
		}

		if (pTemp_Stun[0] > 0)
		{
			if (g_RandPercent(pTemp_Stun[0]))
			{
				m_StunState.nTime = pTemp_Stun[1] * (100 - ReduceStun) / 100;
			}
		}		
	}
	// End
	
	// Tu dong phan lai skill
	if (nLauncher != m_Index && m_AutoReplySkill.nValue[2] > 0 && 
		Npc[nLauncher].m_CurrentCamp != Npc[m_Index].m_CurrentCamp && 
		this->m_Kind == kind_player)	
	{
		BYTE btLimitRand = m_AutoReplySkill.nValue[2] % 256;
		WORD wSkillId = m_AutoReplySkill.nValue[0] / 256;
		BYTE btSkillLevel = m_AutoReplySkill.nValue[0] % 256;
		if (::GetRandomNumber(0,100) < btLimitRand)
		{	
			AutoDoSkill(wSkillId, btSkillLevel, -1, m_Index);
		}
	}
	// End
	// Tu dong thuc thi ky nang
	if (nLauncher != m_Index && pTemp_AutoAttackSkill[2] > 0 && (pTemp_PoisonDamage[0] > 0 || pTemp_FireDamage[0] > 0))
	{	
		if(m_IgnoreNegativeState.nValue[0] <= 1)
		{
			BYTE btLimitRand = pTemp_AutoAttackSkill[2] % 256;
			WORD wSkillId = pTemp_AutoAttackSkill[0] / 256;
			BYTE btSkillLevel = pTemp_AutoAttackSkill[0] % 256;
			if (::GetRandomNumber(0,100) < btLimitRand)
			{	
				Npc[nLauncher].AutoDoSkill(wSkillId, btSkillLevel, -1, m_Index);
			}
		}
	}
	// End
	// Thuc thi ky nang khi sinh luc < 25%
	if(m_AutoRescueSkill.nValue[0] > 0)
	{	
		int nLifeCheck = m_CurrentLifeMax * 25 / 100;
		if(m_CurrentLife < nLifeCheck)
		{	
			BYTE btLimitRand = m_AutoRescueSkill.nValue[2] % 256;
			WORD wSkillId = m_AutoRescueSkill.nValue[0] / 256;
			BYTE btSkillLevel = m_AutoRescueSkill.nValue[0] % 256;
			if(::GetRandomNumber(0,100) < btLimitRand)
			{
				AutoDoSkill(wSkillId, btSkillLevel, -1, m_Index);
			}
		}
	}	
	// End

	m_nPeopleIdx = nLauncher;

	if (IsPlayer() && (m_CurrentLife - nLife < 0))
	{
		if (m_nPlayerIdx > 0)
		{
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeDefend);
		}
	}
	if(IsPlayer())
		m_nTotalReceiveDamage = 0;

	memset(pTemp_PhysicDamage,	0, sizeof(pTemp_PhysicDamage));
	memset(pTemp_ColdDamage,	0, sizeof(pTemp_ColdDamage));
	memset(pTemp_FireDamage,	0, sizeof(pTemp_FireDamage));
	memset(pTemp_LightDamage,	0, sizeof(pTemp_LightDamage));
	memset(pTemp_PoisonDamage,	0, sizeof(pTemp_PoisonDamage));
	memset(pTemp_MagicDamage,	0, sizeof(pTemp_MagicDamage));
	memset(pTemp_SeriesDamage,	0, sizeof(pTemp_SeriesDamage));
	memset(pTemp_DeadlyStrike,	0, sizeof(pTemp_DeadlyStrike));
	memset(pTemp_FatallyStrike,	0, sizeof(pTemp_FatallyStrike));
	memset(pTemp_Stun,		0, sizeof(pTemp_Stun));
	memset(pTemp_StoleLife,		0, sizeof(pTemp_StoleLife));
	memset(pTemp_StoleMana,		0, sizeof(pTemp_StoleMana));
	memset(pTemp_StoleStamina,	0, sizeof(pTemp_StoleStamina));
	memset(pTemp_SkillExp,		0, sizeof(pTemp_SkillExp));
	memset(pTemp_AutoAttackSkill, 	0, sizeof(pTemp_AutoAttackSkill));
	return TRUE;
}
#endif


//

void KNpc::SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum)
{
	if (!pData || !nDataNum)
		return;

	KMagicAttrib*	pTemp = (KMagicAttrib *)pData;
	_ASSERT(nDataNum <= MAX_SKILL_STATE);
	for (int i = 0; i < nDataNum; i++)
	{
		ModifyAttrib(nLauncher, pTemp);
		pTemp++;
	}
}


//

void KNpc::AppendSkillEffect(DWORD uId, BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData)
{	
	if (!pSrcData || !pDesData) {
	    return;
	}

	if (uId <= 0 || uId >= MAX_SKILL) {
    	return;
	}
	
	// Lay % ma skill dc ho tro
	int nSkillEnhance = 0;
	if(uId > 0 && uId < MAX_SKILL)
	{
		nSkillEnhance = m_SkillList.GetEnhanceSkills(uId);
	}

	int nMinDamage = m_PhysicsDamage.nValue[0] + m_CurrentAddPhysicsDamage;
	int	nMaxDamage = m_PhysicsDamage.nValue[2] + m_CurrentAddPhysicsDamage;

	KMagicAttrib* pTemp = (KMagicAttrib *)pSrcData;
	KMagicAttrib* pDes = (KMagicAttrib *)pDesData;

	// Tinh do chinh xac
	if (pTemp->nAttribType == magic_attackrating_p)
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating + m_AttackRating * pTemp->nValue[0] / 100;
	}
	else
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating;
	}
	// Tinh bo qua ne tranh
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_ignoredefense_p)
	{
		pDes->nAttribType = magic_ignoredefense_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Tinh luc danh vat ly
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_physicsenhance_p)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = nMinDamage * (100 + pTemp->nValue[0]) / 100;
		pDes->nValue[2] = nMaxDamage * (100 + pTemp->nValue[0]) / 100;
		if (IsPlayer())
		{
			if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_meleeweapon)
			{
				pDes->nValue[0] += nMinDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
			}
			else if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_rangeweapon)
			{
				pDes->nValue[0] += nMinDamage * m_CurrentRangeEnhance / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentRangeEnhance / 100;
			}
			else
			{
				pDes->nValue[0] += nMinDamage * m_CurrentHandEnhance / 100;
				pDes->nValue[2] += nMaxDamage * m_CurrentHandEnhance / 100;
			}
		}
	}
	else if (pTemp->nAttribType == magic_physicsdamage_v)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = pTemp->nValue[0] * (100 + nSkillEnhance) / 100;
		pDes->nValue[2] = pTemp->nValue[2] * (100 + nSkillEnhance) / 100;
		// Luc danh vat ly noi cong
		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_MagicPhysicsDamage.nValue[0];
			pDes->nValue[2] += m_MagicPhysicsDamage.nValue[2];
		}
	}
	
	// Tinh luc danh bang
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_colddamage_v)
	{
		pDes->nAttribType = magic_colddamage_v;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = this->m_CurrentColdEnhance + pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
		
		if(nSkillEnhance > 0)
		{
			pDes->nValue[0] += pTemp->nValue[0] * nSkillEnhance / 100;
			pDes->nValue[2] += pTemp->nValue[2] * nSkillEnhance / 100;
		}

		// Luc danh bang noi cong
		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_CurrentMagicColdDamage.nValue[0];
			pDes->nValue[2] += m_CurrentMagicColdDamage.nValue[2];
	   }
	}
	//Luc danh bang ngoai cong
	if (bIsPhysical)
	{	
		const int nTempMin = pDes->nValue[0] + m_CurrentColdDamage.nValue[0];
		pDes->nValue[0] = nTempMin;

		if(nTempMin)
		{
			int nTempTime = this->m_CurrentColdEnhance + this->m_CurrentColdDamage.nValue[1];
			if(nTempTime < pDes->nValue[1])
				nTempTime = pDes->nValue[1];
			pDes->nValue[1] = nTempTime;
		}
		pDes->nValue[2] += this->m_CurrentColdDamage.nValue[2];
		//g_DebugLog("Bang sat: %d",pDes->nValue[2]);
	}
	// Tinh luc danh hoa
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_firedamage_v)
	{
		pDes->nAttribType = magic_firedamage_v;
		// pDes->nValue[0] = pTemp->nValue[0] * (100 + nSkillEnhance) / 100;
		// pDes->nValue[2] = pTemp->nValue[2] * (100 + nSkillEnhance) / 100;
		pDes->nValue[0] = pTemp->nValue[0]*(100+nSkillEnhance)/100 + pTemp->nValue[0]*(100+nSkillEnhance)/100 * m_CurrentFireEnhance/100;
		pDes->nValue[2] = pTemp->nValue[2]*(100+nSkillEnhance)/100 + pTemp->nValue[2]*(100+nSkillEnhance)/100 * m_CurrentFireEnhance/100;

		// Luc danh hoa noi cong
		if (!bIsPhysical)
		{
			/*pDes->nValue[0] += m_CurrentMagicFireDamage.nValue[0];
			pDes->nValue[2] += m_CurrentMagicFireDamage.nValue[2];*/
		    pDes->nValue[0] += m_CurrentMagicFireDamage.nValue[0] + m_CurrentMagicFireDamage.nValue[0] * m_CurrentFireEnhance /100;
      		pDes->nValue[2] += m_CurrentMagicFireDamage.nValue[2] + m_CurrentMagicFireDamage.nValue[2] * m_CurrentFireEnhance /100;
		}
	}
	// Luc danh hoa ngoai cong
	if (bIsPhysical)
	{
/*		pDes->nValue[0] += m_CurrentFireDamage.nValue[0];
		pDes->nValue[2] += m_CurrentFireDamage.nValue[2];*/
		pDes->nValue[0] += m_CurrentFireDamage.nValue[0] + m_CurrentFireDamage.nValue[0] * m_CurrentFireEnhance /100;
		pDes->nValue[2] += m_CurrentFireDamage.nValue[2] + m_CurrentFireDamage.nValue[2] * m_CurrentFireEnhance /100;	
	}
	// Tinh luc danh loi
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_lightingdamage_v)
	{
		pDes->nAttribType = magic_lightingdamage_v;
/*		pDes->nValue[0] = pTemp->nValue[0] + (pTemp->nValue[2] - pTemp->nValue[0]) * m_CurrentLightEnhance / 100;
		pDes->nValue[0] = pDes->nValue[0] * (100 + nSkillEnhance) / 100;
		pDes->nValue[2] = pTemp->nValue[2] * (100 + nSkillEnhance) / 100;*/
		pDes->nValue[0] = pTemp->nValue[0] *(100 + nSkillEnhance)/100 + (pTemp->nValue[2]*(100+nSkillEnhance)/100 - pTemp->nValue[0]*(100+nSkillEnhance)/100) * m_CurrentLightEnhance / 100;
		pDes->nValue[2] = pTemp->nValue[2] *(100 + nSkillEnhance) / 100;
		// Luc danh loi noi cong
		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_CurrentMagicLightDamage.nValue[0];
			pDes->nValue[2] += m_CurrentMagicLightDamage.nValue[2];
		}
	}
	// Luc danh loi ngoai cong
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentLightDamage.nValue[0];
		pDes->nValue[2] += m_CurrentLightDamage.nValue[2];
	}
	// Tinh luc danh doc
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_poisondamage_v)
	{
		pDes->nAttribType = magic_poisondamage_v;

		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2]; // nhim fix;
		if(pDes->nValue[2] <= 0)
			pDes->nValue[2] = 1;
		
		if(nSkillEnhance > 0)
		{
			pDes->nValue[0] += pDes->nValue[0] * nSkillEnhance / 100;
		}
		// Luc danh doc noi cong
		if (!bIsPhysical)
		{
			if(m_CurrentMagicPoisonDamage.nValue[0] > 0)
			{
				KMagicAttrib cAttrib;
				cAttrib.nAttribType = magic_poisondamage_v;
				cAttrib.nValue[0] = m_CurrentMagicPoisonDamage.nValue[0];
				cAttrib.nValue[1] = m_CurrentMagicPoisonDamage.nValue[1];
				if(cAttrib.nValue[1] <= 0)
					cAttrib.nValue[1] = 60;
				cAttrib.nValue[2] = m_CurrentMagicPoisonDamage.nValue[2];
				if(cAttrib.nValue[2] <= 0)
					cAttrib.nValue[2] = 1;

				 g_NpcAttribModify.MixPoisonDamage(pDes, &cAttrib);
			}
		}
	}
	if (bIsPhysical)
	{
		if(m_CurrentPoisonDamage.nValue[0] > 0)
		{
			g_NpcAttribModify.MixPoisonDamage(pDes, &this->m_CurrentPoisonDamage);
		}
	}
	// Tinh luc danh ngu hanh
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_magicdamage_v)
	{
		pDes->nAttribType = magic_magicdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]*(100+ nSkillEnhance)/100; //fix dame ngu hanh by kinnox;
		pDes->nValue[2] = pTemp->nValue[2]*(100+ nSkillEnhance)/100;  //fix dame ngu hanh by kinnox;
	}
	// Tinh ngu hanh tuong khac
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_seriesdamage_p)
	{
		pDes->nAttribType = magic_seriesdamage_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	// Tinh luc danh chi mang
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_deadlystrike_p)
	{
		pDes->nAttribType = magic_deadlystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Chi mang o ngoai cong
	if (bIsPhysical)
	{
		pDes->nValue[0] = pDes->nValue[0] + (pDes->nValue[0] * m_CurrentDeadlyStrike / 100);
	}
	// Tinh luc danh chi tu
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_fatallystrike_p)
	{
		pDes->nAttribType = magic_fatallystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Tinh lam choang
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stun_p)
	{
		pDes->nAttribType = magic_stun_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	// Tinh hut sinh luc
	pTemp++;
	pDes++;
	
	if (pTemp->nAttribType == magic_steallife_p)
	{
		pDes->nAttribType = magic_steallife_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Hut sinh luc o ngoai cong
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentLifeStolen;
	}
	// Tinh hut noi luc
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stealmana_p)
	{
		pDes->nAttribType = magic_stealmana_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Hut noi luc o ngoai cong
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentManaStolen;
	}
	// Tinh hut the luc
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stealstamina_p)
	{
		pDes->nAttribType = magic_stealstamina_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	// Hut noi luc o ngoai cong
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentStaminaStolen;
	}
	// Tinh luyen ky nang
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_skill_skillexp_v)
	{	
		int nPointAdd = m_SkillList.GetPointAdd(uId);
		if(nPointAdd > 0)
		{
			int nLevel = m_SkillList.GetCurrentLevel(uId);
			if(nLevel <= 0)
				nLevel = 1;

			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(uId, nLevel);
			KMagicAttrib *DamageAttribs = pOrdinSkill->GetDamageAttribs();
			for (int i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
			{
				if (!(DamageAttribs + i)->nAttribType) continue;
				if ((DamageAttribs + i)->nAttribType == magic_skill_skillexp_v)
				{
					pDes->nAttribType = magic_skill_skillexp_v;
					pDes->nValue[0] = (DamageAttribs + i)->nValue[0];
					pDes->nValue[1] = (DamageAttribs + i)->nValue[1];
					pDes->nValue[2] = (DamageAttribs + i)->nValue[2];
					break;
				}
			}
		}
		else
		{
			pDes->nAttribType = magic_skill_skillexp_v;
			pDes->nValue[0] = pTemp->nValue[0];
			pDes->nValue[1] = pTemp->nValue[1];
			pDes->nValue[2] = pTemp->nValue[2];
		}
	}
	// Tu dong xuat skill
	pTemp++;
	pDes++;
	if(m_AutoAttackSkill.nValue[2] > 0)
	{
		pDes->nAttribType = magic_autoattackskill;
		pDes->nValue[0] = m_AutoAttackSkill.nValue[0];
		pDes->nValue[1] = m_AutoAttackSkill.nValue[1];
		pDes->nValue[2] = m_AutoAttackSkill.nValue[2];
	}
}

// --
//
// --
void KNpc::ProcMove(int nSpeed)
{
	if(m_Doing != do_walk && m_Doing != do_run && m_Doing != do_hurt && m_Doing != do_runattack && m_Doing != do_manyattack)
		return;

	if(nSpeed <= 0)
		return;

#ifndef _SERVER
	if(m_bEditPos)
	{
		nSpeed = nSpeed * (100 + POS_EDITION_PERCENT) / 100;
	}

	if(m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		_ASSERT(0);
		DoStand(); //kinnox5
		if( IsPlayer() )
		SendClientPos2Server();
		return;
	}
#else
	_ASSERT(m_RegionIndex >= 0);
	if(m_RegionIndex < 0)
		return;
#endif

	if(nSpeed >= REGION_CELL_SIZE_X)
	{
		nSpeed = REGION_CELL_SIZE_X - 1;
	}

	int x, y;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, 0, 0, &x, &y);
	x = (x << 10) + m_OffX;
	y = (y << 10) + m_OffY;
	
	int nStopOK = 0;
	int nRet = m_PathFinder.GetDir(x, y, m_Dir, m_DesX, m_DesY, nSpeed, &m_Dir, &nStopOK);

#ifndef _SERVER
	if(nRet == 1)
	{
		x = g_DirCos(m_Dir, 64) * nSpeed;
		y = g_DirSin(m_Dir, 64) * nSpeed;

		if(x >= ((REGION_CELL_SIZE_X) << 10))
		{
			x = (((REGION_CELL_SIZE_X) -1) << 10);
		}
		if(y >= ((REGION_CELL_SIZE_Y) << 10))
		{
			y = (((REGION_CELL_SIZE_Y) -1) << 10);
		}
	}
	else if(nRet == 0)
	{
		DoStand(); //kinnox6
		if( IsPlayer() )
		{
			SendClientPos2Server();
		}
		if(m_bEditPos)
		{
			m_bEditPos = false;
		}
		return;
	}
	else if(nRet == -1)
	{
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		m_RegionIndex = -1;
		return;
	}
	else
	{	
		return;
	}
#endif
#ifdef _SERVER
	if(nRet == 1)
	{
		x = g_DirCos(m_Dir, 64) * nSpeed;
		y = g_DirSin(m_Dir, 64) * nSpeed;

		if (x >= ((REGION_CELL_SIZE_X) << 10))
		{
			x = (((REGION_CELL_SIZE_X) -1) << 10);
		}
		if (y >= ((REGION_CELL_SIZE_Y) << 10))
		{
			y = (((REGION_CELL_SIZE_Y) -1) << 10);
		}
	}
	else
	{
		DoStand(); //kinnox7 moi xoa
		if(nStopOK != 1)
		{
			if( !IsPlayer() )
			{
				int nMpsX, nMpsY;
				GetMpsPos(&nMpsX, &nMpsY);

				NPC_RUN_SYNC	NetCommand;
				NetCommand.ProtocolType = s2c_npcrun;
				NetCommand.ID			= m_dwID;
				NetCommand.nMpsX		= nMpsX;
				NetCommand.nMpsY		= nMpsY;

				int nMaxCount = MAX_BROADCAST_COUNT;
				BROADCAST_REGION(&NetCommand, sizeof(NetCommand), nMaxCount);
			}
		}
		else
		{
           		 if( IsPlayer() )
				BroadCastPosition();
		}
		return;
	}
#endif
	//
	int nRegion = m_RegionIndex;
	int nMapX = m_MapX;
	int nMapY = m_MapY;
	int nOffX = m_OffX + x;
	int nOffY = m_OffY + y;
	//
	if(nOffX < 0)
	{
		nMapX--;
		nOffX += CELLWIDTH;
	}
	else if(nOffX > CELLWIDTH)
	{
		nMapX++;
		nOffX -= CELLWIDTH;
	}
	//
	if(nOffY < 0)
	{
		nMapY--;
		nOffY += CELLHEIGHT;
	}
	else if(nOffY > CELLHEIGHT)
	{
		nMapY++;
		nOffY -= CELLHEIGHT;
	}
	//
	bool bEc = false;
	//
	if(nMapX < 0 && nMapY < 0)
	{
		nMapX += REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = LEFTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY < 0)
	{
		nMapX -= REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = RIGHTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX < 0 && nMapY >= REGIONHEIGHT)
	{
		nMapX += REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = LEFTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY >= REGIONHEIGHT  )
	{
		nMapX -= REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = RIGHTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(bEc)
	{	
		if(nRegion >= 0) 
		{
			if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
			{
				DoStand(); //kinnox8 moi xoa
	#ifndef _SERVER
				if( IsPlayer() )
				{
					DoStand(); //kinnox9
					SendClientPos2Server();
				}
				if(m_bEditPos)
				{
					m_bEditPos = false;
				}
	#else
				 if( IsPlayer() )
				 {	
					 Player[m_nPlayerIdx].StopMove();// dung lai khi tong vao barrier, xoa by kinnox;
					 BroadCastPosition();
				 }
	#endif
				 return;
			}
#ifdef _SERVER
			if( IsPlayer() )
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
#endif
			MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);	
		}
		
	}
	else // bEc = false
	{
		if(nMapX < 0)
		{
			nMapX += REGIONWIDTH;
			nRegion = LEFTREGIONIDX;
		}
		else if(nMapX >= REGIONWIDTH)
		{
			nMapX -= REGIONWIDTH;
			nRegion = RIGHTREGIONIDX;
		}
		//
		if(nRegion >= 0)
		{
			if(nMapY < 0)
			{
				nMapY += REGIONHEIGHT;
				nRegion = UPREGIONIDX;
			}
			else if(nMapY >= REGIONHEIGHT)
			{
				nMapY -= REGIONHEIGHT;
				nRegion = DOWNREGIONIDX;
			}
			if(nRegion >= 0) 
			{
				if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
				{
					DoStand();//moi xoa //kinnox9
#ifndef _SERVER
					if( IsPlayer() )
					{
						DoStand(); //kinnox10
						SendClientPos2Server();
					}
					if(m_bEditPos)
					{
						m_bEditPos = false;
					}
#else
					 if( IsPlayer() )
					 {	
						 Player[m_nPlayerIdx].StopMove(); //dung lai khi tong vao barrier, xoa by kinnox;
						 BroadCastPosition();
					 }
#endif				
					 return;
				}
#ifdef _SERVER
				if( IsPlayer() )
				{
					Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
				}
#endif
				MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);
			}
			
		}
	}
}
//




void KNpc::ProcJump(int nSpeed)
{		
	if(m_Doing != do_jump && m_Doing != do_jumpattack)
		return;

	if(nSpeed <= 0)
		return;

	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if(nSpeed >= REGION_CELL_SIZE_X)
	{
		nSpeed = REGION_CELL_SIZE_X - 1;
	}

	m_Height = (m_JumpFirstSpeed * m_Frames.nCurrentFrame - ACCELERATION_OF_GRAVITY * m_Frames.nCurrentFrame * m_Frames.nCurrentFrame / 2) / 8;
	if (m_Height < 0)
		m_Height = 0;

	int nRegion = m_RegionIndex;
	int nMapX = m_MapX;
	int nMapY = m_MapY;
	int nOffX = m_OffX + g_DirCos(m_JumpDir, 64) * nSpeed;
	int nOffY = m_OffY + g_DirSin(m_JumpDir, 64) * nSpeed;
	//
	if(nOffX < 0)
	{
		nMapX--;
		nOffX += CELLWIDTH;
	}
	else if(nOffX > CELLWIDTH)
	{
		nMapX++;
		nOffX -= CELLWIDTH;
	}
	//
	if(nOffY < 0)
	{
		nMapY--;
		nOffY += CELLHEIGHT;
	}
	else if(nOffY > CELLHEIGHT)
	{
		nMapY++;
		nOffY -= CELLHEIGHT;
	}
	//
	bool bEc = false;
	//
	if(nMapX < 0 && nMapY < 0)
	{
		nMapX += REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = LEFTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY < 0)
	{
		nMapX -= REGIONWIDTH;
		nMapY += REGIONHEIGHT;
		nRegion = RIGHTUPREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX < 0 && nMapY >= REGIONHEIGHT)
	{
		nMapX += REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = LEFTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(nMapX >= REGIONWIDTH && nMapY >= REGIONHEIGHT  )
	{
		nMapX -= REGIONWIDTH;
		nMapY -= REGIONHEIGHT;
		nRegion = RIGHTDOWNREGIONIDX;
		bEc = true;
	}
	//
	if(bEc)
	{	
		if(nRegion >= 0) 
		{
			// if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
			// {
				// DoStand();
				// return;
			// }
			MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);	
		}
		
	}
	else // bEc = false
	{
		if(nMapX < 0)
		{
			nMapX += REGIONWIDTH;
			nRegion = LEFTREGIONIDX;
		}
		else if(nMapX >= REGIONWIDTH)
		{
			nMapX -= REGIONWIDTH;
			nRegion = RIGHTREGIONIDX;
		}
		//
		if(nRegion >= 0)
		{
			if(nMapY < 0)
			{
				nMapY += REGIONHEIGHT;
				nRegion = UPREGIONIDX;
			}
			else if(nMapY >= REGIONHEIGHT)
			{
				nMapY -= REGIONHEIGHT;
				nRegion = DOWNREGIONIDX;
			}
			if(nRegion >= 0) 
			{
				// if(SubWorld[m_SubWorldIndex].TestBarrierMin(nRegion, nMapX, nMapY, nOffX, nOffY, 0, 0) > 0)
				// {
					// DoStand();
					// return;
				// }
				MoveNpc(nRegion, nMapX, nMapY, nOffX, nOffY);
			}
			
		}
	}
}


//

void KNpc::SendCommand(NPCCMD cmd,int x,int y, int z)
{

	if (m_RandMoveState.nTime > 0 )
		return;
#ifdef _SERVER
	if (IsPlayer())
	{
		if(m_Command.CmdKind == do_death)
			return;
	}
#else
	if (!IsPlayer() && m_bEditPos)
	{
		int nCurX;
		int nCurY;
		GetMpsPos(&nCurX, &nCurY);
		
		int nOldDx = m_DesX-nCurX;
		int nOldDy = m_DesY-nCurY;
  
		int nNewDx = x - nCurX;
		int nNewDy = y - nCurY;

		if (cmd == do_run && nOldDx * nNewDx + nOldDy * nNewDy <= 0)
		{
			EditPos(FALSE);
		}
		else if (cmd == do_walk && nOldDx * nNewDx + nOldDy * nNewDy <= 0)
		{
			EditPos(FALSE);
		}
	}
#endif
	m_Command.CmdKind = cmd;
	m_Command.Param_X = x;
	m_Command.Param_Y = y;
	m_Command.Param_Z = z;
}


//

void KNpc::SendCommandRandMove(int x,int y, int z)
{
#ifndef _SERVER
	if (!IsPlayer() && m_bEditPos)
	{
		int nCurX;
		int nCurY;
		GetMpsPos(&nCurX, &nCurY);
		
		int nOldDx = m_DesX-nCurX;
		int nOldDy = m_DesY-nCurY;
  
		int nNewDx = x - nCurX;
		int nNewDy = y - nCurY;

		if (nOldDx * nNewDx + nOldDy * nNewDy <= 0)
		{
			EditPos(FALSE);
		}
		else if (nOldDx * nNewDx + nOldDy * nNewDy <= 0)
		{
			EditPos(FALSE);
		}
	}
#endif

	m_Command.CmdKind = do_walk;
	m_Command.Param_X = x;
	m_Command.Param_Y = y;
	m_Command.Param_Z = z;
}


//

BOOL KNpc::NewPath(int nMpsX, int nMpsY)
{
	m_DesX = nMpsX;
	m_DesY = nMpsY;
	return TRUE;
}


//

BOOL KNpc::NewJump(int nMpsX, int nMpsY)
{
	if (m_CurrentJumpSpeed <= 0)
		return FALSE;
	
	int nX, nY;
	GetMpsPos(&nX, &nY);

	if (nX == nMpsX && nY == nMpsY)
		return FALSE;

	int nDir = g_GetDirIndex(nX, nY, nMpsX, nMpsY);
	int	nMaxLength = m_CurrentJumpSpeed * m_CurrentJumpFrame;
	int	nWantLength = g_GetDistance(nX, nY, nMpsX, nMpsY);
	int	nSin = g_DirSin(nDir, 64);
	int	nCos = g_DirCos(nDir, 64);

	if(nWantLength > nMaxLength)
	{
		m_DesX = nX + ((nMaxLength * nCos) >> 10);
		m_DesY = nY + ((nMaxLength * nSin) >> 10);
		nWantLength = nMaxLength;
	}
	else if(nWantLength <= MIN_JUMP_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return FALSE;
	}

	m_JumpStep = nWantLength / m_CurrentJumpSpeed;

	int nTestX = 0;
	int nTestY = 0;
	int nSuccessStep = 0;

	for (int i = 1; i < m_JumpStep + 1; i++)
	{
		nTestX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
		nTestY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
		int nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
		if (Obstacle_NULL == nBarrier)
		{
			nSuccessStep = i;
		}
		if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier)
		{
			if (nSuccessStep <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * nSuccessStep) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * nSuccessStep) >> 10);
			m_JumpStep = nSuccessStep;
			break;
		}
		int	nTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);
		if (nTrap)
		{
			if (i <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
			m_JumpStep = i;
			break;
		}
	}
	m_JumpDir = nDir;
	return TRUE;
}


//

int KNpc::NewGlide(int nMpsX, int nMpsY, int nMaxLength)
{	
	if(m_CurrentJumpSpeed <= 0)
		return 0;
	
	int nX, nY;
	GetMpsPos(&nX, &nY);

	if (nX == nMpsX && nY == nMpsY)
		return 0;

	int nDir = g_GetDirIndex(nX, nY, nMpsX, nMpsY);
	int	nWantLength = g_GetDistance(nX, nY, nMpsX, nMpsY);
	int	nSin = g_DirSin(nDir, 64);
	int	nCos = g_DirCos(nDir, 64);

	if(nWantLength > nMaxLength)
	{
		m_DesX = nX + ((nMaxLength * nCos) >> 10);
		m_DesY = nY + ((nMaxLength * nSin) >> 10);
		nWantLength = nMaxLength;
	}
	else if(nWantLength <= MIN_JUMP_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return 0;
	}

	m_JumpStep = nWantLength / m_CurrentJumpSpeed;

	int nTestX = 0;
	int nTestY = 0;
	int nSuccessStep = 0;

	for (int i = 1; i < m_JumpStep + 1; i++)
	{
		nTestX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
		nTestY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
		int nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
		if (Obstacle_NULL == nBarrier)
		{
			nSuccessStep = i;
		}
		if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier)
		{
			if (nSuccessStep <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return 0;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * nSuccessStep) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * nSuccessStep) >> 10);
			m_JumpStep = nSuccessStep;
			break;
		}
		int	nTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);
		if (nTrap)
		{
			if (i <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return 0;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
			m_JumpStep = i;
			break;
		}
	}
	m_JumpDir = nDir;
	return nWantLength;
}


//

void KNpc::SelfDamage(int nDamage)
{
	m_CurrentLife -= nDamage;
	if (m_CurrentLife <= 0)
	{
		m_CurrentLife = 1;
	}
}


//

BOOL KNpc::Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast)
{
	if (!IsPlayer())
		return TRUE;

	int *pSource = NULL;

	switch(nType)
	{
	case attrib_mana:
		pSource = &m_CurrentMana;
		break;
	case attrib_life:
		pSource = (int*)&m_CurrentLife;
		break;
	case attrib_stamina:
		pSource = &m_CurrentStamina;
		break;
	default:
		break;
	}

	if (pSource)
	{
		if (*pSource < nCost)
		{
#ifndef _SERVER
			KSystemMessage Msg;

			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 1;
			Msg.eType = SMT_NORMAL;
			switch(nType)
			{
			case attrib_mana:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_MANA, sizeof(Msg.szMessage));
				break;
			case attrib_life:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_LIFE, sizeof(Msg.szMessage));
				break;
			case attrib_stamina:
				g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_STAMINA, sizeof(Msg.szMessage));
				break;
			default:
				break;
			}
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
#endif
			return FALSE;
		}
		else
		{
			if (!bOnlyCheckCanCast)
				*pSource -= nCost;
			return TRUE;
		}
	}
	return FALSE;
}


//

void KNpc::DoJump()
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_jump)
		return;
	
	m_Doing = do_jump;
	m_Dir = m_JumpDir;
	m_ProcessAI	= 0;
	m_JumpFirstSpeed = ACCELERATION_OF_GRAVITY * (m_JumpStep - 1) / 2 ;
#ifdef _SERVER
	NPC_JUMP_SYNC	NetCommand;
	NetCommand.ProtocolType = s2c_npcjump;
	NetCommand.m_dwID		= m_dwID;
	NetCommand.m_nMpsX		= m_DesX;
	NetCommand.m_nMpsY		= m_DesY;
		
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	for (int i= 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
	

#ifndef _SERVER
	m_ClientDoing = cdo_jump;
#endif

	m_Frames.nTotalFrame = m_JumpStep;
	m_Frames.nCurrentFrame = 0;	
}


//

BOOL KNpc::OnJump()
{
	if (m_Doing == do_death || m_Doing == do_revive)
        {
                DoStand();
                m_ProcessAI = 1;
                return FALSE;
        }
	ProcJump(m_CurrentJumpSpeed);
	if (WaitForFrame())
	{
		DoStand();
		m_ProcessAI	= 1;
		return FALSE;
	}
	return TRUE;
}


//

BOOL KNpc::WaitForFrame()
{
	m_Frames.nCurrentFrame++;
	if(m_Frames.nCurrentFrame < m_Frames.nTotalFrame)
	{
		return FALSE;
	}
	m_Frames.nCurrentFrame = 0;
	return TRUE;
}


//

BOOL KNpc::IsReachFrame(int nPercent)
{
	if (m_Frames.nCurrentFrame == m_Frames.nTotalFrame * nPercent / 100)
	{
		return TRUE;
	}
	return FALSE;
}


//

void KNpc::Load(int nNpcSettingIdx, int nLevel, int nSeries)
{
	m_PathFinder.Init(m_Index);
	if (nLevel <= 0) 
	{
		nLevel = 1;
	}
	
	if(nSeries < series_metal || nSeries >= series_num)
	{
		nSeries = series_metal;
	}

#ifndef _SERVER
	char	szNpcTypeName[32];
#endif
	if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID || nNpcSettingIdx == PLAYER_FEMALE_NPCTEMPLATEID)
	{
		m_NpcSettingIdx = nNpcSettingIdx;
		m_Level = nLevel;
#ifndef _SERVER
		if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "MainMan");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "MainLady");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	= NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetNpcCopyFromTemplate(nNpcSettingIdx, nLevel, nSeries);

#ifndef _SERVER	
		g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(2, "CharacterName", "", szNpcTypeName, sizeof(szNpcTypeName));
		}
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIMode", 12, &m_AiMode);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam1", 12, &m_AiParam[0]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam2", 12, &m_AiParam[1]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam3", 12, &m_AiParam[2]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam4", 12, &m_AiParam[3]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam5", 12, &m_AiParam[4]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam6", 12, &m_AiParam[5]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam7", 12, &m_AiParam[6]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ActiveRadius", 12, &m_ActiveRadius);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ClientOnly", 0, &m_bClientOnly);
		if (m_AiMode == 11 || m_AiMode == 12 || m_AiMode == 17)
			m_AiParam[6] = m_AiMode;
#endif
	}
#ifndef _SERVER
	m_DataRes.Init(szNpcTypeName, &g_NpcResList);
	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
	m_DataRes.SetMantle(m_MantleType);
#endif
	m_CurrentCamp = m_Camp;
}

void KNpc::GetMpsPos(int *pPosX, int *pPosY)
{
#ifdef _SERVER
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#else
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#endif
}


//

BOOL	KNpc::SetActiveSkill(int nSkillIdx)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;

	if (!(m_SkillList.m_Skills[nSkillIdx].SkillId && m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel))
		return FALSE;

	m_ActiveSkillID = m_SkillList.m_Skills[nSkillIdx].SkillId;
	int nLevel = m_SkillList.m_Skills[nSkillIdx].SkillLevel;
	_ASSERT(m_ActiveSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL && nLevel > 0);
	
	ISkill * pISkill =  g_SkillManager.GetSkill(m_ActiveSkillID, nLevel);
	if (pISkill)
    {
		m_CurrentAttackRadius = pISkill->GetAttackRadius();
    }
	return TRUE;
}


//

void KNpc::SetAuraSkill(int nSkillID)
{
	int nCurLevel = 0;
	int nPointAdd = 0;
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL) 
    {
        nSkillID = 0;
    }
	else
	{
		nCurLevel = m_SkillList.GetCurrentLevel(nSkillID);
		nPointAdd = m_SkillList.GetPointAdd(nSkillID);
		if (nCurLevel <= 0) 
        {
            nSkillID = 0;
        }
		else
		{
			_ASSERT(nSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL);
			
			KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nCurLevel + nPointAdd);
            if (!pOrdinSkill || !pOrdinSkill->IsAura())
			{
				nSkillID  = 0;
			}
		}
	}
	m_ActiveAuraID = nSkillID;

#ifdef _SERVER
	if (m_ActiveAuraID)
	{
		int nStateSpecialId = 0;
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID, nCurLevel + nPointAdd);
		
		if (pOrdinSkill)
        {
			nStateSpecialId = pOrdinSkill->GetStateSpecailId();
        }
		
		if (nStateSpecialId) 
		{
			AddStateInfo(nStateSpecialId);
		}
		UpdateNpcStateInfo();
		BroadCastState();
	}
	else
	{
		UpdateNpcStateInfo();
		BroadCastState();
	}
#endif

#ifndef _SERVER
	SKILL_CHANGEAURASKILL_COMMAND ChangeAuraMsg;
	ChangeAuraMsg.ProtocolType		= c2s_changeauraskill;
	ChangeAuraMsg.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	ChangeAuraMsg.dwTimePacker = GetTickCount();
	ChangeAuraMsg.m_nAuraSkill		= m_ActiveAuraID;
	if (g_pClient)
		g_pClient->SendPackToServer(&ChangeAuraMsg, sizeof(SKILL_CHANGEAURASKILL_COMMAND));
#endif

}


//

BOOL KNpc::SetPlayerIdx(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_PLAYER)
		return FALSE;

	if (m_Kind != kind_player)
		return FALSE;

	m_nPlayerIdx = nIdx;
	return TRUE;
}

#ifdef _SERVER

//

BOOL KNpc::SendSyncData(int nClient)
{
	BOOL	bRet = FALSE;
	NPC_SYNC	NpcSync;

	NpcSync.ProtocolType		= (BYTE)s2c_syncnpc;
	NpcSync.m_btKind			= (BYTE)m_Kind;
	NpcSync.Camp				= (BYTE)m_Camp;
	NpcSync.CurrentCamp			= (BYTE)m_CurrentCamp;
	NpcSync.m_bySeries			= (BYTE)m_Series;
	NpcSync.m_Doing				= (BYTE)m_Doing;
	if (m_CurrentLifeMax > 0)
		NpcSync.LifePerCent		= (BYTE)((m_CurrentLife << 7) / m_CurrentLifeMax);
	else
		NpcSync.LifePerCent		= 0;

	if (this->IsPlayer())
		NpcSync.m_btMenuState	= (BYTE)Player[this->m_nPlayerIdx].m_cMenuState.m_nState;
	else
		NpcSync.m_btMenuState	= 0;

	GetMpsPos((int *)&NpcSync.MapX, (int *)&NpcSync.MapY);
	NpcSync.ID					= m_dwID;
	NpcSync.NpcSettingIdx		= MAKELONG(m_Level, m_NpcSettingIdx);
	NpcSync.NpcEnchant			= (WORD)this->m_cGold.GetGoldType();

	strcpy(NpcSync.m_szName, Name);
	NpcSync.m_wLength			= sizeof(NPC_SYNC) - 1 - sizeof(NpcSync.m_szName) + strlen(NpcSync.m_szName);

	if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&NpcSync, NpcSync.m_wLength + 1)))
	{
		bRet = TRUE;
	}
	else
	{
		printf("Packing sync data failed...\n");
		return FALSE;
	}
#ifdef _DEBUG
	//g_DebugLog("[Sync]%d:%s<%d> request to %d. size:%d", SubWorld[m_SubWorldIndex].m_dwCurrentTime, Name, m_Kind, nClient, NpcSync.m_wLength + 1);
#endif
	BroadCastState(); // moi them by kinnox 20/07/2023;
	if( IsPlayer() )
	{
		PLAYER_SYNC	PlayerSync;

		PlayerSync.ProtocolType		= (BYTE)s2c_syncplayer;
		PlayerSync.ID				= m_dwID;
		PlayerSync.ArmorType		= (BYTE)m_ArmorType;
		PlayerSync.AttackSpeed		= (BYTE)m_CurrentAttackSpeed;
		PlayerSync.CastSpeed		= (BYTE)m_CurrentCastSpeed;
		PlayerSync.HelmType			= (BYTE)m_HelmType;
		if(m_bRideHorse) 
		{
			PlayerSync.HorseType        = (BYTE)m_HorseType;
		}
		else
		{
			PlayerSync.HorseType        = -1;
		}
		PlayerSync.MaskType			= m_wMaskType;
		PlayerSync.MantleType		= m_MantleType;
		PlayerSync.MantleLevel		= m_byMantleLevel;									
		PlayerSync.RunSpeed			= m_CurrentRunSpeed;
		PlayerSync.WalkSpeed		= m_CurrentWalkSpeed;
		PlayerSync.WeaponType		= (BYTE)m_WeaponType;
		PlayerSync.RankID			= (BYTE)m_btRankId;
		PlayerSync.PKValue			= Player[m_nPlayerIdx].m_cPK.GetPKValue();
		PlayerSync.WorldStat		= Player[m_nPlayerIdx].m_nWorldStat;
		PlayerSync.Repute			= Player[m_nPlayerIdx].TaskGetSaveVal(TV_REPUTE);
		PlayerSync.Fuyuan			= Player[m_nPlayerIdx].TaskGetSaveVal(TV_FUYUAN);
		PlayerSync.TransLife		= (BYTE)Player[m_nPlayerIdx].TaskGetSaveVal(TV_TRANSLIFE);
		PlayerSync.OpenShop			= (BYTE)Player[m_nPlayerIdx].m_cShop.GetState();
		PlayerSync.ImagePlayer		= (BYTE)Player[m_nPlayerIdx].m_ImagePlayer;//Player Avatar by kinnox;	
		sprintf(PlayerSync.ShopAdv, Player[m_nPlayerIdx].m_cShop.GetAdv());
		Player[m_nPlayerIdx].m_cTong.GetTongName((char*)PlayerSync.TongName);
		Player[m_nPlayerIdx].m_cTong.GetTongTitle((char*)PlayerSync.TongTitle);
		PlayerSync.TitleID			= Player[m_nPlayerIdx].GetTitle();
		PlayerSync.m_btSomeFlag		= 0;
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 1)
			PlayerSync.m_btSomeFlag |= 0x01;
		if (Npc[Player[m_nPlayerIdx].m_nIndex].m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x02;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x04;
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
			PlayerSync.m_btSomeFlag |= 0x10;

		if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&PlayerSync, sizeof(PLAYER_SYNC))))
		{
			//printf("Packing player sync data ok...\n");
			bRet = TRUE;
		}
		else
		{
			printf("Packing player sync data failed...\n");
			return FALSE;
		}
	}
	return bRet;
}


/*
BOOL KNpc::HasTeamStateChanged()
{
    TeamMemberState currentState;

    
    currentState.nMemberCount = g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].GetMemberCount();

    for (int i = 0; i < currentState.nMemberCount; i++)
    {
        
        int memberIdx = g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].m_nMember[i];
        if (memberIdx >= 0)
        {
            currentState.m_HP[i] = Player[memberIdx].m_HP;
            currentState.m_TP[i] = Player[memberIdx].m_TP;
        }
        else
        {
            currentState.m_HP[i] = 0;
            currentState.m_TP[i] = 0;
        }
    }

   
    if (memcmp(&currentState, &lastTeamState, sizeof(TeamMemberState)) != 0)
    {
        
        lastTeamState = currentState;
        return true;
    }

    return false; 
}
*/
void KNpc::NormalSync()
{
	if(m_Index <= 0 || m_Index >= MAX_NPC)
		return;

	if(m_RegionIndex < 0)
		return;

	if( !IsAlive() )
		return;

	NPC_NORMAL_SYNC NpcSync;
	int	nMpsX, nMpsY;

	GetMpsPos(&nMpsX, &nMpsY);

	NpcSync.ProtocolType = (BYTE)s2c_syncnpcmin;
	NpcSync.ID = m_dwID;

	NpcSync.MapX = nMpsX;
	NpcSync.MapY = nMpsY;

	NpcSync.Camp = (BYTE)m_CurrentCamp;
	NpcSync.State = 0;
	//them by kinnox;
    NpcSync.m_CurrentLifeMax			= (int)m_CurrentLifeMax;
    if (m_CurrentLifeMax > 0)
            NpcSync.m_CurrentLife		= (int)m_CurrentLife;
    else
            NpcSync.m_CurrentLife		= 0;
    NpcSync.m_CurrentManaMax			= (int)m_CurrentManaMax;
    if (m_CurrentLifeMax > 0)
            NpcSync.m_CurrentMana		= (int)m_CurrentMana;
    else
            NpcSync.m_CurrentMana		= 0;
    NpcSync.m_CurrentWalkSpeed			= (BYTE)m_CurrentWalkSpeed;
    NpcSync.m_CurrentRunSpeed			= (BYTE)m_CurrentRunSpeed;
    NpcSync.m_ASpeed					= (int)m_CurrentAttackSpeed;
    NpcSync.m_CSpeed                    = (int)m_CurrentCastSpeed;
        //
	if (m_FreezeState.nTime > 0)
		NpcSync.State |= STATE_FREEZE;
	if (m_PoisonState.nTime > 0)
		NpcSync.State |= STATE_POISON;
	if (m_StunState.nTime > 0)
		NpcSync.State |= STATE_STUN;
	if (m_Poison2DecManaState.nTime > 0)
		NpcSync.State |= STATE_POISON2MANA;
	if (m_HideState.nTime > 0)
	{
		NpcSync.State |= STATE_HIDE;
		NpcSync.HideValue = m_HideState.nValue[0];
	}
	if (m_RandMoveState.nTime > 0)
		NpcSync.State |= STATE_RANDMOVE;
	if (m_IgnoreNegativeState.nTime > 0)
		NpcSync.State |= STATE_IGNORENEGATIVE;
	NpcSync.Doing = (BYTE)m_Doing;
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_PLAYER;//MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		_ASSERT(m_SubWorldIndex >= 0 && nConRegion >= 0);
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}

	if( IsPlayer() )
	{
		if (Player[m_nPlayerIdx].m_cTeam.m_nFlag && g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].IsOpen())
		{
			Player[m_nPlayerIdx].SendSelfTeamInfo();
		}
		PLAYER_NORMAL_SYNC PlayerSync;

		PlayerSync.ProtocolType	= (BYTE)s2c_syncplayermin;
		PlayerSync.ID			= m_dwID;
		PlayerSync.AttackSpeed	= m_CurrentAttackSpeed;
		PlayerSync.CastSpeed	= m_CurrentCastSpeed;
		PlayerSync.RunSpeed		= (BYTE)m_CurrentRunSpeed;
		PlayerSync.WalkSpeed	= (BYTE)m_CurrentWalkSpeed;
		PlayerSync.HelmType		= (BYTE)m_HelmType;
		PlayerSync.ArmorType	= (BYTE)m_ArmorType;
		PlayerSync.WeaponType	= (BYTE)m_WeaponType;
		if(m_bRideHorse) 
		{
			PlayerSync.HorseType = (BYTE)m_HorseType;
		}
		else
		{
			PlayerSync.HorseType = -1;
		}
		PlayerSync.MaskType			= m_wMaskType;
		PlayerSync.MantleType		= m_MantleType;
		PlayerSync.MantleLevel		= m_byMantleLevel;
		PlayerSync.RankID			= (BYTE)m_btRankId;
		PlayerSync.PKValue			= Player[m_nPlayerIdx].m_cPK.GetPKValue();
		PlayerSync.WorldStat		= Player[m_nPlayerIdx].m_nWorldStat;
		PlayerSync.Repute			= Player[m_nPlayerIdx].TaskGetSaveVal(TV_REPUTE);
		PlayerSync.Fuyuan			= Player[m_nPlayerIdx].TaskGetSaveVal(TV_FUYUAN);
		PlayerSync.TransLife		= (BYTE)Player[m_nPlayerIdx].TaskGetSaveVal(TV_TRANSLIFE);
		PlayerSync.OpenShop			= (BYTE)Player[m_nPlayerIdx].m_cShop.GetState();
		PlayerSync.ImagePlayer		= (BYTE)Player[m_nPlayerIdx].m_ImagePlayer; //Player Avatar by kinnox;
		sprintf(PlayerSync.ShopAdv, Player[m_nPlayerIdx].m_cShop.GetAdv());
		Player[m_nPlayerIdx].m_cTong.GetTongName((char*)PlayerSync.TongName);
		Player[m_nPlayerIdx].m_cTong.GetTongTitle((char*)PlayerSync.TongTitle);
		PlayerSync.TitleID			= Player[m_nPlayerIdx].GetTitle();

		PlayerSync.m_btSomeFlag	= 0;
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 1)
			PlayerSync.m_btSomeFlag |= 0x01;
		if (m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x02;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x04;
		if (Player[m_nPlayerIdx].m_cTong.GetOpenFlag())
			PlayerSync.m_btSomeFlag |= 0x08;
		if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
			PlayerSync.m_btSomeFlag |= 0x10;
		/*int nMaxCount = MAX_BROADCAST_COUNT;
		BROADCAST_REGION(&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount);*/
		//moi them by kinnox 20/07/2023;
		int nMaxCount = MAX_PLAYER;//MAX_BROADCAST_COUNT;
		CURREGION.BroadCast(&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
		for (j = 0; j < 8; j++)
		{
			int nConRegion = CURREGION.m_nConnectRegion[j];
			if (nConRegion == -1)
				continue;
			SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
		}
		//

		NPC_PLAYER_TYPE_NORMAL_SYNC	sSync;
		sSync.ProtocolType = s2c_syncnpcminplayer;
		sSync.m_dwNpcID = m_dwID;
		sSync.m_dwMapX = nMpsX;
		sSync.m_dwMapY = nMpsY;
		sSync.m_wOffX = m_OffX;
		sSync.m_wOffY = m_OffY;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(sSync));
	}
}


//

void KNpc::BroadCastRevive(int nType)
{
	//kenny
	if (!IsPlayer())
		return;

	if (m_RegionIndex < 0)
		return;

	NPC_REVIVE_SYNC	NpcReviveSync;
	NpcReviveSync.ProtocolType = s2c_playerrevive;
	NpcReviveSync.ID = m_dwID;
	NpcReviveSync.Type = (BYTE)nType;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX, m_MapY);
	int j;
	for (j = 0; j < 8; j++)
	{
		int nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}
}
#endif

// --
// 
// --
bool KNpc::SetPos(int nX, int nY)
{
	if (m_SubWorldIndex < 0)
	{
		_ASSERT(0);
		return 0;
	}
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion < 0)
	{
		return 0;
	}

	int nOldRegion = m_RegionIndex;
	if (m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
#ifdef _SERVER
		NPC_REMOVE_SYNC	RemoveSync;
		RemoveSync.ProtocolType = s2c_npcremove;
		RemoveSync.ID = m_dwID;
		SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
#endif
	}

	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;

	if (nOldRegion != nRegion)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
		if (IsPlayer())
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx);
#else
		SubWorld[m_SubWorldIndex].NpcChangeRegion(
			SubWorld[m_SubWorldIndex].m_Region[nOldRegion].m_RegionID,
			SubWorld[m_SubWorldIndex].m_Region[nRegion].m_RegionID, m_Index);
#endif
	}
	SubWorld[m_SubWorldIndex].m_Region[nRegion].AddRef(m_MapX, m_MapY, obj_npc);

	m_Height = 0;
#ifdef _SERVER
	FIND_PATH_SYNC	Sync;
	Sync.ProtocolType = s2c_findpathsync;
	Sync.m_dwID = m_dwID;
	Sync.m_nPosX = nX;
	Sync.m_nPosY = nY;
	Sync.m_nOffX = m_OffX;
	Sync.m_nOffY = m_OffY;
	g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&Sync, sizeof(Sync));
	// Only call DoStand() if not sitting to avoid interrupting regeneration
	if (m_Doing != do_sit) {
		DoStand(); //kinnox13
	}
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
	m_ProcessAI = 1;
	m_ProcessState = 1;
#endif

	return 1;
}

// --
//
// --
void KNpc::MoveNpc(int nRegion, int nMapX, int nMapY, int nOffX, int nOffY, bool bFix /*= false*/)
{
	
/*	if(!m_bClientOnly && m_RegionIndex >= 0)
	{
		CURREGION.DecRef(m_MapX, m_MapY, obj_npc);
	}
	
	if (nRegion == -1)
	{
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		return;
	}
	int nOldRegion = m_RegionIndex;
	if(m_RegionIndex != nRegion)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
		if (IsPlayer())
		{
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx); // kha nang bi loi line 1414
		}
#else	
		if(bFix)
		{
			SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
		}
		else
		{
			if(nOldRegion >= 0)
			{
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
			}
			else
			{
				SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);		
			}
		}
		m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;

#endif
	}
	
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	
	if(!m_bClientOnly && m_RegionIndex >= 0) 
	{
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	}
*/
	if ( !m_bClientOnly && m_RegionIndex >= 0)
	{
		CURREGION.DecRef(m_MapX, m_MapY, obj_npc);
	}
#ifndef _SERVER
	if ( m_CurrentLife == 0 && m_Kind == kind_normal )
	{
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		return;
	}
#endif
	int nOldRegion = m_RegionIndex;
	if (m_RegionIndex != nRegion)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
		if (IsPlayer())
		{
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx);
			if (m_nPlayerIdx > 0)
			{
				//Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
		}
#else
		if (nRegion >= 0)
		{		
			if (nOldRegion >= 0)
			{
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
			}
			else
			{
				SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);		
			}
			m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		}
		else
		{
			if (m_RegionIndex >= 0)
			{
				SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
				SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
				m_RegionIndex = -1;
				m_dwRegionID = -1;
			}
		}
#endif
	}
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_OffX = nOffX;
	m_OffY = nOffY;
	
	if ( !m_bClientOnly && m_RegionIndex >= 0) 
	{
		CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	}
}

#ifndef _SERVER
#include "scene/KScenePlaceC.h"


//

int KNpc::PaintInfo(int nHeightOffset, bool bSelect, int nFontSize, DWORD dwBorderColor)
{
	int /*nX, nY,*/ nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	DWORD	dwColor;
	int nHeightOff = nHeightOffset + nFontSize + 1;
	
	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return 0;
	}
	else if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player ||
		 m_Kind == kind_partner) && m_HideState.nTime > 0
	)
	{
		return 0;
	}
	if (m_Kind == kind_player)
	{
		switch(m_CurrentCamp)
		{
		case camp_begin:
			dwColor = 0xffffffff;
			break;
		case camp_justice:
			dwColor = 0xff000000 | (255 << 16) | (168 << 8) | 94;
			break;
		case camp_evil:
			dwColor = 0xff000000 | (255 << 16) | (146 << 8) | 255;
			break;
		case camp_balance:
			dwColor = 0xff000000 | (85 << 16) | (255 << 8) | 145;
			break;
		case camp_free:
			dwColor = 0xff000000 | (255 << 16);
			break;
		case camp_animal:
			dwColor = 0xff000000 | 255 << 16 | 0 << 8 | 204;
			break;
		case camp_event:
			dwColor = 0xff000000 | 109 << 16 | 207 << 8 | 246;
			break;
		default:
			dwColor = 0xffff00ff;
			break;
		}
		
		char	szString[128];
		strcpy(szString, Name);
		if (m_FreezeState.nTime || m_PoisonState.nTime || m_StunState.nTime || m_RandMoveState.nTime)
		{
			strcat(szString, "(");
			if (m_FreezeState.nTime)
				strcat(szString, "B¨ng");
			if (m_PoisonState.nTime)
				strcat(szString, "§éc");
			if (m_StunState.nTime)
				strcat(szString, "Mª");
			if (m_RandMoveState.nTime)   
				strcat(szString, "Lo¹n");
			strcat(szString, ")");
		}
// Calculate name position - shift right if has mantle icon
		int nNameWidth = nFontSize * g_StrLen(szString) / 4;
		int nXX = nMpsX - nNameWidth + ((m_byMantleLevel > 0 && m_byMantleLevel <= MAX_ITEM_LEVEL) ? 40 : 0);

		// Draw mantle icon before name (if equipped)
		if(m_byMantleLevel > 0 && m_byMantleLevel <= MAX_ITEM_LEVEL)
		{
			PaintMantle(nHeightOff, nFontSize, nXX, nMpsY);
		}

		// Draw name								  
		g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nXX, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
		if(m_wMaskType > 0)
		{
			PaintHorseState(nFontSize, nHeightOff);
		}


		if(m_cTransLife > 0)
		{
			PaintTransLifeInfo(nFontSize, nHeightOff);
		}

		nHeightOffset += nFontSize + 1;

	}
	else if (m_Kind == kind_dialoger && IsAlive())
	{
		dwColor = 0xffffffff;
		g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
		nHeightOffset += nFontSize + 1;
	}
	else if (bSelect)
	{	
		int nType = this->m_cGold.GetGoldType();
		switch(nType)
		{	
			case 1:
					dwColor = 0xffae00c3;  
					break;	
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:	
			case 12:
				dwColor = 0xff000000 | (243 << 16) | (194 << 8) | 90;
				break;
			case 13:
			case 14:
			case 15:
			case 16:
				dwColor = 0xff000000 | (100 << 16) | (100 << 8) | 255;
				break;
			default:
				dwColor = 0xffffffff;
				break;
		}
#ifdef _DEBUG
		char szDebuger[80];
		sprintf(szDebuger, "%s[%d][%d][%d]", Name, m_Level, m_CurrentLife, m_Experience * 2);
		g_pRepresent->OutputText(nFontSize, szDebuger, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szDebuger) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
		

#else
		g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
#endif
		PaintSeriesNpc(nFontSize, nHeightOff);
		nHeightOffset += nFontSize + 1;
	}
	
//#ifdef SWORDONLINE_SHOW_DBUG_INFO
#ifdef _DEBUG
	//if(Player[CLIENT_PLAYER_INDEX].m_bDebugMode)
	//{
		char szNameID[50];
		sprintf(szNameID,"[%d]-[%d]", m_dwID, m_Level);
		g_pRepresent->OutputText(12, szNameID, KRF_ZERO_END, nMpsX, nMpsY + 20, 0xfff0fff0, 0, m_Height, dwBorderColor);
		
		if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index)
		{
			char	szMsg[256];
			int nCount[9];
			for (int i = 0; i < 9; i++)
				nCount[i] = 0;
			if (LEFTUPREGIONIDX >= 0)
				nCount[0] = LEFTUPREGION.m_NpcList.GetNodeCount();
			if (UPREGIONIDX >= 0)
				nCount[1] = UPREGION.m_NpcList.GetNodeCount();
			if (RIGHTUPREGIONIDX >= 0)
				nCount[2] = RIGHTUPREGION.m_NpcList.GetNodeCount();
			if (LEFTREGIONIDX >= 0)
				nCount[3] = LEFTREGION.m_NpcList.GetNodeCount();
			if (m_RegionIndex >= 0)
				nCount[4] = CURREGION.m_NpcList.GetNodeCount();
			if (RIGHTREGIONIDX >= 0)
				nCount[5] = RIGHTREGION.m_NpcList.GetNodeCount();
			if (LEFTDOWNREGIONIDX >= 0)
				nCount[6] = LEFTDOWNREGION.m_NpcList.GetNodeCount();
			if (DOWNREGIONIDX >= 0)
				nCount[7] = DOWNREGION.m_NpcList.GetNodeCount();
			if (RIGHTDOWNREGIONIDX >= 0)
				nCount[8] = RIGHTDOWNREGION.m_NpcList.GetNodeCount();
			
			int nPosX, nPosY;
			GetMpsPos(&nPosX, &nPosY);
			sprintf(szMsg,
				"NpcID:%d  Life:%d\nRegionIndex:%d Pos:%d,%d\nPlayerNumber:%d\n"
				"NpcNumber:\n%02d,%02d,%02d\n%02d,%02d,%02d\n%02d,%02d,%02d",
				m_dwID,
				m_CurrentLife,			
				m_RegionIndex,
				m_MapX,
				m_MapY,
				CURREGION.m_PlayerList.GetNodeCount(),
				nCount[0], nCount[1], nCount[2],
				nCount[3], nCount[4], nCount[5],
				nCount[6], nCount[7], nCount[8]			
				);
			
			g_pRepresent->OutputText(12, szMsg, -1, 320, 40, 0xffffffff);

		}
	//}
#endif

	return nHeightOffset;
}

//
// Draw mantle icon before player name
//

int KNpc::PaintMantle(int nHeightOff, int nFontSize, int nMpsX, int nMpsY)

{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	// Load mantle icon sprite from npcbobo.ini [FortuneRank] section
	char szKey[16];
	sprintf(szKey, "Spr_%d", m_byMantleLevel);
	g_StateIniSetting.GetString("FortuneRank", szKey, "", RUIconImage.szImage, sizeof(RUIconImage.szImage));
	// Check if sprite path is valid
	if(RUIconImage.szImage[0] == 0)
	{
		return nMpsX; // No sprite to draw, return original position
	}
	KImageParam	Param;
	g_pRepresent->GetImageParam(RUIconImage.szImage, &Param, ISI_T_SPR);
	RUIconImage.oPosition.nX = nMpsX - Param.nWidth;
	RUIconImage.oPosition.nY = nMpsY - Param.nHeight/2 - 4;
	RUIconImage.oPosition.nZ = nHeightOff;
	RUIconImage.nFrame = g_SubWorldSet.GetGameTime() % Param.nNumFrames;
	g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	return RUIconImage.oPosition.nX;
}
//
void KNpc::PaintNpcAuto(int nHeightOffset, bool bSelect, int nFontSize, DWORD dwBorderColor)
{
	int nMpsX, nMpsY;
	DWORD dwColor;
	int nHeightOff = nHeightOffset; //+ nFontSize + 1;	

	GetMpsPos(&nMpsX, &nMpsY);
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto && (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index))
	{
		char szString[64];
		dwColor = 0x53acff;
		int nDesX = Player[CLIENT_PLAYER_INDEX].m_cAI.nDestXPaint;
		int nDesY = Player[CLIENT_PLAYER_INDEX].m_cAI.nDestYPaint;
		if (Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint <= 0 && m_FightMode && m_Doing != do_stand)
		{
			sprintf(szString, "[%s]", "T×m ®èi t­îng...");
		}
		else
		{
			sprintf(szString, "[%s]", "§ang tù ®éng");
		}
		
		if (Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint > 0)
		{
			if (nDesX > 0 && nDesY > 0)
			{
				sprintf(szString, "[%s - T®:%d/%d]",Npc[Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint].Name,nDesX, nDesY);
				Player[CLIENT_PLAYER_INDEX].m_cAI.nDestXPaint = 0;
				Player[CLIENT_PLAYER_INDEX].m_cAI.nDestYPaint = 0;
				Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint = 0;
			}
			else
			{
				sprintf(szString, "[%s]", Npc[Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint].Name);
				Player[CLIENT_PLAYER_INDEX].m_cAI.nNpcIDPaint = 0;
			}
		}
		else if (nDesX > 0 && nDesY > 0)
		{
			sprintf(szString, "[Täa ®é:%d/%d]",nDesX, nDesY);
		}
		
		if (strlen(szString) > 0)
		{
			g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szString) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize + 1, dwBorderColor);				
		}
	}
}
//

void	KNpc::PaintSeriesNpc(int nFontSize, int nHeightOff) 
{	
	KRUImage	m_RUSeriesImg;
	char szKey[10];
	int nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	
	itoa(m_Series, szKey, 10);
	g_StateIniSetting.GetString("Series", szKey, "", m_RUSeriesImg.szImage, sizeof(m_RUSeriesImg.szImage));
	
    m_RUSeriesImg.nType = ISI_T_SPR;
    m_RUSeriesImg.Color.Color_b.a = 255;
    m_RUSeriesImg.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
    m_RUSeriesImg.uImage = 0;
    m_RUSeriesImg.nISPosition = IMAGE_IS_POSITION_INIT;
    m_RUSeriesImg.bRenderFlag = 1;
    m_RUSeriesImg.oPosition.nX = nMpsX + nFontSize * g_StrLen(Name) / 4 + 5;
    m_RUSeriesImg.oPosition.nY = nMpsY + 4; 
    m_RUSeriesImg.oPosition.nZ = nHeightOff;
    m_RUSeriesImg.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUSeriesImg, RU_T_IMAGE, FALSE);  
}

//

void KNpc::PaintHorseState(int nFontSize, int nHeightOff)
{	
	if(!m_bRideHorse && m_Doing != do_sit)
		return;

	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return;
	}
	
	KRUImage	m_RUActionImg;
	int nMpsX, nMpsY;
	GetMpsPos(&nMpsX,&nMpsY);

	if (m_bRideHorse)
	{
		g_StateIniSetting.GetString("Actions", "0", "", m_RUActionImg.szImage, sizeof(m_RUActionImg.szImage));
	}
	else if (m_Doing == do_sit)
	{
		g_StateIniSetting.GetString("Actions", "1", "", m_RUActionImg.szImage, sizeof(m_RUActionImg.szImage));
	}
	
	m_RUActionImg.nType = ISI_T_SPR;
	m_RUActionImg.Color.Color_b.a = 255;
	m_RUActionImg.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUActionImg.uImage = 0;
	m_RUActionImg.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUActionImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	m_RUActionImg.oPosition.nX = nMpsX + nFontSize * 2;
	m_RUActionImg.oPosition.nY = nMpsY + nHeightOff / 6;
	m_RUActionImg.oPosition.nZ = nHeightOff;
	m_RUActionImg.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUActionImg, RU_T_IMAGE, FALSE);
}


//

void KNpc::PaintRankInfo(int nHeightOffset, int nFontSize, DWORD dwBorderColor)
{	
	if(!m_szTongName && !m_szTongName[0] && !m_wTitleId && !m_btRankId)
		return;

	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return;
	}

	int nMpsX, nMpsY;
	DWORD dwColor;
	int nHeightOff = nHeightOffset + nFontSize + 1;
	GetMpsPos(&nMpsX, &nMpsY);
	
	switch(m_CurrentCamp)
	{
		case camp_begin:
			dwColor = 0xffffffff;
			break;
		case camp_justice:
			dwColor = 0xff000000 | (255 << 16) | (168 << 8) | 94;
			break;
		case camp_evil:
			dwColor = 0xff000000 | (255 << 16) | (146 << 8) | 255;
			break;
		case camp_balance:
			dwColor = 0xff000000 | (85 << 16) | (255 << 8) | 145;
			break;
		case camp_free:
			dwColor = 0xff000000 | (255 << 16);
			break;
		default:
			dwColor = 0xffff00ff;
			break;
	}

	if(m_szTongName[0])
	{	
		char szTong[32];
		sprintf(szTong, "%s-%s", m_szTongName, m_szTongTitle);
		g_pRepresent->OutputText(nFontSize, szTong, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szTong) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize, dwBorderColor);
		nHeightOff += nFontSize + 1;
	}

	if(m_btRankId)
	{
		dwColor = 0xff01e098;
		char szString[64];
		sprintf(szString, "%d", m_btRankId);
		int nRow = g_TitleTabSetting.FindRow(szString, 1);
		g_TitleTabSetting.GetString(nRow, 1, "", szString, sizeof(szString)); 
		g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szString) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize + 1, dwBorderColor);	
		nHeightOff += nFontSize + 1;
	}

	if(m_wTitleId) 
	{	
		dwColor = 0 << 24 | 255 << 16 | 0 << 8 | 204;
		char szString[64];
		sprintf(szString, "%d", m_wTitleId);
		int nRow = g_TitleTabSetting.FindRow(szString, 1);
		g_TitleTabSetting.GetString(nRow, 1, "", szString, sizeof(szString));
		g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szString) / 4, nMpsY, dwColor, 0, nHeightOff + nFontSize + 1, dwBorderColor);				
		nHeightOff += nFontSize + 1;
	}

	if (m_Kind == kind_player && Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
			PaintNpcAuto(nHeightOff,FALSE,nFontSize,dwBorderColor);

	
}



//

void KNpc::PaintTransLifeInfo(int nFontSize, int nHeightOff)
{	
	if(!m_cTransLife)
		return;

	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return;
	}
	
	KRUImage	m_RUTransLifeImg;
	int nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	
	char szKey[8];
	sprintf(szKey, "%d", m_cTransLife);
	g_StateIniSetting.GetString("TransLife", szKey, "", m_RUTransLifeImg.szImage, sizeof(m_RUTransLifeImg.szImage));

	m_RUTransLifeImg.nType = ISI_T_SPR;
	m_RUTransLifeImg.Color.Color_b.a = 255;
	m_RUTransLifeImg.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUTransLifeImg.uImage = 0;
	m_RUTransLifeImg.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUTransLifeImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	m_RUTransLifeImg.oPosition.nX = nMpsX - nFontSize * g_StrLen(Name) / 4 - 2;
	
	m_RUTransLifeImg.oPosition.nY = nMpsY + 10;
	m_RUTransLifeImg.oPosition.nZ = nHeightOff;
	m_btTimeFrame[1]++;
	if(!(m_btTimeFrame[1] % GAME_UPDATE_TIME)) 
	{	
		m_btTimeFrame[1] = 0;
		m_btFrameSpr[1]++;
		
		if(m_btFrameSpr[1] >= m_DataRes.GetSprNumFrames(m_RUTransLifeImg.szImage, m_RUTransLifeImg.nType))
		{
			m_btFrameSpr[1] = 0;
		}
		
	}
	m_RUTransLifeImg.nFrame = m_btFrameSpr[1];
	g_pRepresent->DrawPrimitives(1, &m_RUTransLifeImg, RU_T_IMAGE, FALSE);
}


//

void KNpc::PaintShopInfo(int nFontSize, int nHeightOff)
{	
	if(!m_bOpenShop)
		return;
	
	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return;
	}
	
	KRUImage	m_RUShopImg;
	int nMpsX, nMpsY;
	int nLeft = nFontSize * g_StrLen(m_szShopAdv) / 4;
	GetMpsPos(&nMpsX, &nMpsY);
	
	g_StateIniSetting.GetString("PlayerShop", "Left", "", m_RUShopImg.szImage, sizeof(m_RUShopImg.szImage));
	m_RUShopImg.oPosition.nX = nMpsX - nLeft - 18;
	if(m_wMaskType > 0)
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2 - nHeightOff);
	else
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2);

	m_RUShopImg.nType = ISI_T_SPR;
	m_RUShopImg.Color.Color_b.a = 255;
	m_RUShopImg.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUShopImg.uImage = 0;
	m_RUShopImg.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUShopImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	m_RUShopImg.oPosition.nZ = nHeightOff;
	m_RUShopImg.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUShopImg, RU_T_IMAGE, FALSE);
	
	g_StateIniSetting.GetString("PlayerShop", "Mid", "", m_RUShopImg.szImage, sizeof(m_RUShopImg.szImage));
	if(m_wMaskType > 0)
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2 - nHeightOff);
	else
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2);

	m_RUShopImg.uImage = 0;
	for(int i = 0; i < nLeft * 2; i++)
	{	
		m_RUShopImg.oPosition.nX = nMpsX - nLeft + i;
		g_pRepresent->DrawPrimitives(1, &m_RUShopImg, RU_T_IMAGE, FALSE);
	}

	g_StateIniSetting.GetString("PlayerShop", "Right", "", m_RUShopImg.szImage, sizeof(m_RUShopImg.szImage));
	m_RUShopImg.oPosition.nX = nMpsX + nLeft;
	if(m_wMaskType > 0)
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2 - nHeightOff);
	else
		m_RUShopImg.oPosition.nY = nMpsY - (nHeightOff * 2);

	m_RUShopImg.uImage = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUShopImg, RU_T_IMAGE, FALSE);

	if(m_wMaskType > 0)
		g_pRepresent->OutputText(nFontSize, m_szShopAdv, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szShopAdv) / 4, nMpsY + nFontSize + 4, 255 << 16 | 217 << 8 | 78, 0, nHeightOff + nFontSize * 5);
	else
		g_pRepresent->OutputText(nFontSize, m_szShopAdv, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szShopAdv) / 4, nMpsY, 255 << 16 | 217 << 8 | 78, 0, nHeightOff + nFontSize * 5);

}




int	KNpc::PaintChat(int nHeightOffset)
{	
	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return nHeightOffset;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return nHeightOffset;
	}
	if (m_Kind != kind_player)
		return nHeightOffset;
	if (m_nChatContentLen <= 0)
		return nHeightOffset;
	if (m_nChatNumLine <= 0)
		return nHeightOffset;

	int nFontSize = 12;
	int					nWidth, nHeight;
	int					nMpsX, nMpsY;
	KOutputTextParam	sParam;

	sParam.BorderColor = 0;
	sParam.nNumLine = m_nChatNumLine;

	nWidth = m_nChatFontWidth * nFontSize / 2;
	nHeight = sParam.nNumLine * (nFontSize + 1);

	nWidth += 6;
	nHeight += 5;


	GetMpsPos(&nMpsX, &nMpsY);
	sParam.nX = nMpsX - nWidth / 2;
	sParam.nY = nMpsY;
	sParam.nZ = nHeightOffset + nHeight;
	sParam.Color = SHOW_CHAT_COLOR;
	sParam.nSkipLine = 0;
	sParam.nVertAlign = 0;	
	sParam.bPicPackInSingleLine = true;
	g_pRepresent->OutputRichText(nFontSize, &sParam, m_szChatBuffer, m_nChatContentLen, nWidth);

	return sParam.nZ;
}

#include "../../Engine/Src/Text.h"

//

int	KNpc::SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength)
{
	int nFontSize = 12;

	char szChatBuffer[MAX_SENTENCE_LENGTH];

	memset(szChatBuffer, 0, sizeof(szChatBuffer));

	if (nMsgLength)
	{
		int nOffset = 0;
		if (pMsgBuff[0] != KTC_TAB)
		{
			szChatBuffer[nOffset] = (char)KTC_COLOR;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0x00;
			nOffset++;
			strncpy(szChatBuffer + nOffset, Name, 32);
			nOffset += strlen(Name);
			szChatBuffer[nOffset] = ':';
			nOffset++;
			szChatBuffer[nOffset] = (char)KTC_COLOR_RESTORE;
			nOffset++;
		}
		else
		{
			pMsgBuff ++;
			nMsgLength--;
		}
		
		if (nMsgLength)
		{
			int i = 0;
			int j = 0;
			int nCount = 0;
			BOOL bOk = FALSE;
			unsigned int uMsgLen1 = 0;
			char* pszCheck1 = NULL;
			char* pszCheck2 = NULL;

			pszCheck1 = (char*)pMsgBuff;
			while(i < nMsgLength)
			{
				if(*pszCheck1 == '(')
				{
					pszCheck2 = (char*)(pMsgBuff + i);
					nCount = 0;
					j = i;
					while(j < nMsgLength)
					{
						if(*pszCheck2 == ')' && nCount == 17)
						{
								bOk = TRUE;
								break;
						}
						if(*pszCheck2 == '•')
						{
							nCount++;
						}
						pszCheck2++;
						j++;
					}
				}
				pszCheck1++;
				if(bOk) 
				{
					uMsgLen1 = i; 
					break;
				}
				i++;
			}
			if(bOk)
			{
				if(uMsgLen1)
				{
					memcpy(szChatBuffer + nOffset, pMsgBuff, uMsgLen1);
					nOffset += uMsgLen1;
					szChatBuffer[nOffset] = 0;
				} 
				else 
				{
					nOffset++;
				}
			}
			else
			{
				memcpy(szChatBuffer + nOffset, pMsgBuff, nMsgLength);
				nOffset += nMsgLength;
			}

			memset(m_szChatBuffer, 0, sizeof(m_szChatBuffer));
			m_nChatContentLen = MAX_SENTENCE_LENGTH;
			TGetLimitLenEncodedString(szChatBuffer, nOffset, nFontSize, SHOW_CHAT_WIDTH,
				m_szChatBuffer, m_nChatContentLen, 2, true);

			m_nChatNumLine = TGetEncodedTextLineCount(m_szChatBuffer, m_nChatContentLen, SHOW_CHAT_WIDTH, m_nChatFontWidth, nFontSize, 0, 0, true);
			if (m_nChatNumLine >= 2)
				m_nChatNumLine = 2;
			m_nCurChatTime = IR_GetCurrentTime();
			return true;
		}
	}
	return false;
}

//
//AutoAI by kinnox;
int	KNpc::SetWarning(BOOL bWarning)
{
	m_bWarning = bWarning;
	return m_bWarning;
}
//
int	KNpc::PaintWarning(int nHeightOffset, bool bSelect)
{
	BOOL nRet = false;
	int	nMpsXs, nMpsYs;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMpsXs, &nMpsYs);
	
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bWarning) 
	{
		if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player ||
		 m_Kind == kind_partner)
		)
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				if (!Npc[m_Index].m_bRideHorse)
				nMpsYs = nMpsYs-(SHOW_LIFE_HEIGHT+147);
				if (Npc[m_Index].m_bRideHorse)
				nMpsYs = nMpsYs-(SHOW_LIFE_HEIGHT+80);
			}
			else if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				if (!Npc[m_Index].m_bRideHorse)
				nMpsYs = nMpsYs-(SHOW_LIFE_HEIGHT+90);
				if (Npc[m_Index].m_bRideHorse)
				nMpsYs = nMpsYs-(SHOW_LIFE_HEIGHT+24);
			}
			g_pRepresent->OutputText(12,"Cã cao thñ mai phôc h·y thËn träng!", KRF_ZERO_END, (nMpsXs - SHOW_LIFE_WIDTH * 2.5),nMpsYs, 0x810000, 0, nHeightOffset, 0);
		}
		return true;
	}
	return nRet;
}
//end code;
//
//

int	KNpc::PaintLife(int nHeightOffset, bool bSelect)
{	
	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return nHeightOffset;
	}
	else if (m_HideState.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
	{
		return nHeightOffset;
	}
	if (!bSelect &&
		(m_Kind != kind_player &&
		m_Kind != kind_partner)
		)
		return nHeightOffset;

	if (m_CurrentLifeMax <= 0)
		return nHeightOffset;

	if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player ||
		 m_Kind == kind_partner)
		)
		return nHeightOffset;

	int	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	int nWid = SHOW_LIFE_WIDTH;
	int nHei = SHOW_LIFE_HEIGHT;
	KRUShadow	Blood;
	int nX = m_CurrentLife * 100 / m_CurrentLifeMax;

	if (nX >= 50)
	{
		Blood.Color.Color_b.r = 0;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else if (nX >= 25)
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 0;
	}

	if(Npc[m_Index].m_btPKFlag == 2 || 
			Player[Npc[m_Index].m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING)
	{
		Blood.Color.Color_b.r = 204;
		Blood.Color.Color_b.g = 51;
		Blood.Color.Color_b.b = 153;
	}

	Blood.Color.Color_b.a = 0;
	Blood.oPosition.nX = nMpsX - nWid / 2;
	Blood.oPosition.nY = nMpsY;
	Blood.oPosition.nZ = nHeightOffset + nHei;
	Blood.oEndPos.nX = Blood.oPosition.nX + nWid * nX / 100;
	Blood.oEndPos.nY = nMpsY;
	Blood.oEndPos.nZ = nHeightOffset;
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);

	Blood.Color.Color_b.r = 128;
	Blood.Color.Color_b.g = 128;
	Blood.Color.Color_b.b = 128;
	Blood.oPosition.nX = Blood.oEndPos.nX;
	Blood.oEndPos.nX = nMpsX + nWid / 2;
	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
	
	return nHeightOffset + nHei;
}


//

int	KNpc::PaintMana(int nHeightOffset)
{
	if (m_Kind != kind_player &&
		m_Kind != kind_partner)
		return nHeightOffset;

	if (m_CurrentManaMax <= 0)
		return nHeightOffset;

	int	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	int nWid = 38;
	int nHei = 3;
	KRUShadow	Mana;
	int nX = m_CurrentMana * 100 / m_CurrentManaMax;
	
	Mana.Color.Color_b.r = 100;
	Mana.Color.Color_b.g = 125;
	Mana.Color.Color_b.b = 255;
	Mana.Color.Color_b.a = 0;
	Mana.oPosition.nX = nMpsX - nWid / 2;
	Mana.oPosition.nY = nMpsY;
	Mana.oPosition.nZ = nHeightOffset + nHei;
	Mana.oEndPos.nX = Mana.oPosition.nX + nWid * nX / 100;
	Mana.oEndPos.nY = nMpsY;
	Mana.oEndPos.nZ = nHeightOffset;
	g_pRepresent->DrawPrimitives(1, &Mana, RU_T_SHADOW, FALSE);

	Mana.Color.Color_b.r = 0;
	Mana.Color.Color_b.g = 0;
	Mana.Color.Color_b.b = 0;
	Mana.Color.Color_b.a = 20;
	Mana.oPosition.nX = Mana.oEndPos.nX;
	Mana.oEndPos.nX = nMpsX + nWid / 2;
	g_pRepresent->DrawPrimitives(1, &Mana, RU_T_SHADOW, FALSE);

	KRURect Rect;
	Rect.Color.Color_dw	= 0xff000000;
	Rect.oPosition.nX = nMpsX - nWid / 2;
	Rect.oPosition.nY = nMpsY;
	Rect.oPosition.nZ = nHeightOffset + nHei;
	Rect.oEndPos.nX = Rect.oPosition.nX + nWid;
	Rect.oEndPos.nY = nMpsY;
	Rect.oEndPos.nZ = nHeightOffset;
	g_pRepresent->DrawPrimitives(1, &Rect, RU_T_RECT, FALSE);

	return nHeightOffset + nHei;
}


//

void KNpc::Paint()
{	
	if(m_HideState.nValue[0] < 0 && m_HideState.nTime > 0)
	{
		if(m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			return;
	}
	else if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player ||
		 m_Kind == kind_partner) && m_HideState.nTime > 0
		 )
	{
		return;
	}

	if (m_ResDir != m_Dir)
	{
		int nDirOff = m_Dir - m_ResDir;
		if (nDirOff > 32)
			nDirOff -= 64;
		else if (nDirOff < - 32)
			nDirOff += 64;
		m_ResDir += nDirOff / 2;
		if (m_ResDir >= 64)
			m_ResDir -= 64;
		if (m_ResDir < 0)
			m_ResDir += 64;
	}

	if (Player[CLIENT_PLAYER_INDEX].m_bHideNpc == TRUE && Player[CLIENT_PLAYER_INDEX].m_bHidePlayer == FALSE)
	{            
		if ( m_Kind == kind_normal ||  m_Kind == kind_dialoger)
		{
			if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
			}      
			else
			{
				m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
			}
		}
		else
		{
			m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
		}
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_bHidePlayer == TRUE && Player[CLIENT_PLAYER_INDEX].m_bHideNpc == FALSE)
	{            
		if ( m_Kind == kind_player)
		{
			if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
			}      
			else
			{
				m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
			}
		}	
		else
		{
			m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
		}		
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_bHideNpc == TRUE && Player[CLIENT_PLAYER_INDEX].m_bHidePlayer == TRUE)
	{
		if ( m_Kind == kind_normal ||  m_Kind == kind_dialoger || m_Kind == kind_player )
		{
			if (m_Kind == kind_player)
			{
				if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
				{
				}  
				else
				{
					m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
				}
			}
		}	
		else
		{
			m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
		}
	}	
	else
	{
		m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame);
	}

	int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	DrawMenuState(nHeight);
}
#endif


void	KNpc::AddBaseLifeMax(int nLife)
{
	m_LifeMax += nLife;
	m_CurrentLifeMax = m_LifeMax;
}


void	KNpc::AddCurLifeMax(int nLife)
{
	m_CurrentLifeMax += nLife;
}


void	KNpc::AddBaseStaminaMax(int nStamina)
{
	m_StaminaMax += nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}


void	KNpc::AddCurStaminaMax(int nStamina)
{
	m_CurrentStaminaMax += nStamina;
}

void	KNpc::AddBaseManaMax(int nMana)
{
	m_ManaMax += nMana;
	m_CurrentManaMax = m_ManaMax;
}

void	KNpc::AddCurManaMax(int nMana)
{
	m_CurrentManaMax += nMana;
}

/*
-
//	
-
void	KNpc::ResetLifeReplenish()
{
	m_LifeReplenish = (m_Level + 5) / 6;
	m_CurrentLifeReplenish = m_LifeReplenish;
}
*/

/*
-
//	
-
void	KNpc::CalcCurLifeMax()
{
}
*/

/*
-
//	
-
void	KNpc::CalcCurStaminaMax()
{
	m_CurrentStaminaMax = m_StaminaMax;		// »¹ÐèÒª¼ÓÉÏ ×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©µÈµÄÓ°Ïì
}
*/

/*
-
//	
-
void	KNpc::CalcCurManaMax()
{
	m_CurrentManaMax = m_ManaMax;			// »¹ÐèÒª¼ÓÉÏ ×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©µÈµÄÓ°Ïì
}
*/

void	KNpc::CalcCurLifeReplenish()
{
	m_CurrentLifeReplenish = m_LifeReplenish;
}


void	KNpc::Remove()
{
	Init();
#ifndef _SERVER
	RemoveRes();
#endif
}

#ifndef _SERVER
void	KNpc::RemoveRes()
{
	m_DataRes.Remove(m_Index);
}
#endif

void	KNpc::SetSeries(int nSeries)
{
	if (nSeries >= series_metal && nSeries < series_num)
		m_Series = nSeries;
	else
		m_Series = series_metal;
}

//Goc chua fix loi skill

//void KNpc::SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime/* = -1*/, BOOL bRemove /*= FALSE*/)
/*{
	if (nLevel <= 0|| nSkillID <= 0) return ;
	_ASSERT(nDataNum < MAX_SKILL_STATE);
	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;
#ifdef _SERVER
	if (IsPlayer())//moi them by kinnox;
	{
		STATE_EFFECT_SYNC	Sync;
		Sync.ProtocolType	= s2c_syncstateeffect;
		Sync.m_dwSkillID	= nSkillID;
		Sync.m_nLevel		= nLevel;
		Sync.m_nTime		= nTime;
		Sync.m_bRemove		= bRemove;
		memcpy(Sync.m_MagicAttrib, pData, sizeof(KMagicAttrib) * nDataNum);
		Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * (MAX_SKILL_STATE - nDataNum) - 1;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
	}
#endif
	KStateNode* pNode = NULL;
	KMagicAttrib* pTemp = NULL;
	
	if(bRemove)
	{
		pNode = (KStateNode *)m_StateSkillList.GetHead();
		while(pNode)
		{
			if (pNode->m_SkillID == nSkillID)
			{
				for (int i = 0; i < nDataNum; i++)
				{
					ModifyAttrib(nLauncher, &pNode->m_State[i]);
					pTemp++;
				}
				_ASSERT(pNode != NULL);
				pNode->Remove();
				delete pNode;
#ifdef _SERVER
				UpdateNpcStateInfo();
				BroadCastState();
#endif
				pNode = NULL;
				return;
			}
			pNode = (KStateNode *)pNode->GetNext();
		}
	}
	else
	{
		pNode = (KStateNode *)m_StateSkillList.GetHead();
		while(pNode)
		{
			if (pNode->m_SkillID == nSkillID)
			{
				if (pNode->m_Level == nLevel)
				{
					pNode->m_LeftTime = nTime;
				}
				else if (pNode->m_Level < nLevel)
				{
					pTemp = (KMagicAttrib *)pData;
					for (int i = 0; i < nDataNum; i++)
					{
						ModifyAttrib(nLauncher, &pNode->m_State[i]);
						ModifyAttrib(nLauncher, pTemp);
						pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
						pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
						pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
						pTemp++;
					}
				}
				return;
			}
			pNode = (KStateNode *)pNode->GetNext();
		}

		pNode = new KStateNode;
		pNode->m_SkillID = nSkillID;
		pNode->m_Level = nLevel;
		pNode->m_LeftTime = nTime;
		_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
		
		KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);
		if (pOrdinSkill)
			pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();
		else
			pNode->m_StateGraphics = 0;

	#ifdef _SERVER	
		if (pNode->m_StateGraphics) 
		{	
			AddStateInfo(pNode->m_StateGraphics);
			UpdateNpcStateInfo();
			BroadCastState();	
		}
	#else
		if (IsPlayer() && pNode->m_StateGraphics) 
		{	
			AddStateInfo(pNode->m_StateGraphics);
		}
	#endif
		pTemp = (KMagicAttrib *)pData;
		
		for (int i = 0; i < nDataNum; i++)
		{			
			ModifyAttrib(nLauncher, pTemp);
			pNode->m_State[i].nAttribType = pTemp->nAttribType;
			pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
			pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
			pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
			pTemp++;
		}
		AddStateSkillList(pNode);
	}
}*/

void KNpc::SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime/* = -1*/, BOOL bRemove /*= FALSE*/)
{
    if (nLevel <= 0 || nSkillID <= 0) return;
    _ASSERT(nDataNum < MAX_SKILL_STATE);
    if (nDataNum >= MAX_SKILL_STATE) nDataNum = MAX_SKILL_STATE;


    KMagicAttrib* pDbg = (KMagicAttrib*)pData;
    int dbgStateId = (pDbg && nDataNum > 0) ? pDbg[0].nValue[0] : -1;
    int dbgStateT  = (pDbg && nDataNum > 0) ? pDbg[0].nValue[1] : nTime;


#ifdef _SERVER
    /*
    if (IsPlayer())
    {
        STATE_EFFECT_SYNC Sync;
        Sync.ProtocolType = s2c_syncstateeffect;
        Sync.m_dwSkillID  = nSkillID;
        Sync.m_nLevel     = nLevel;
        Sync.m_nTime      = nTime;
        Sync.m_bRemove    = bRemove;
        memcpy(Sync.m_MagicAttrib, pData, sizeof(KMagicAttrib) * nDataNum);
        Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * (MAX_SKILL_STATE - nDataNum) - 1;
        g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
    }*/
	STATE_EFFECT_SYNC Sync;
	Sync.ProtocolType = s2c_syncstateeffect;
	Sync.m_dwNpcId = m_dwID; 
	Sync.m_dwSkillID  = nSkillID;
	Sync.m_nLevel     = nLevel;
	Sync.m_nTime      = nTime;
	Sync.m_bRemove    = bRemove;
	memcpy(Sync.m_MagicAttrib, pData, sizeof(KMagicAttrib) * nDataNum);
	Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * (MAX_SKILL_STATE - nDataNum) - 1;

	if (IsPlayer())
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
	else
		{
		BroadCastState(); 
		}

#endif

    KStateNode*    pNode = NULL;
    KMagicAttrib*  pTemp = NULL;

    // ---------------- REMOVE STATE ----------------
    if (bRemove)
    {
        pNode = (KStateNode*)m_StateSkillList.GetHead();
        while (pNode)
        {
            if (pNode->m_SkillID == nSkillID)
            {
                // G? tác d?ng (d?o l?i thu?c tính)
                for (int i = 0; i < nDataNum; i++)
                {
                    ModifyAttrib(nLauncher, &pNode->m_State[i]);
                }
                _ASSERT(pNode != NULL);
                pNode->Remove();
                delete pNode;

#ifdef _SERVER
                UpdateNpcStateInfo();
                BroadCastState();
                //printf("[StateApply-Remove] skill=%d lvl=%d (broadcast)\n",  nSkillID, nLevel);
#endif
                return;
            }
            pNode = (KStateNode*)pNode->GetNext();
        }
        return; // không có node d? remove
    }

    // ---------------- ADD / REFRESH / UPGRADE ----------------
    // Tìm state cùng SkillID dã t?n t?i
    pNode = (KStateNode*)m_StateSkillList.GetHead();
    while (pNode)
    {
        if (pNode->m_SkillID == nSkillID)
        {
            // REFRESH (cùng level) -> gia h?n th?i gian
            if (pNode->m_Level == nLevel)
            {
                pNode->m_LeftTime = nTime;
#ifdef _SERVER
                UpdateNpcStateInfo();
                BroadCastState();
                //printf("[StateApply-Refresh]  skill=%d lvl=%d time=%d (broadcast)\n", nSkillID, nLevel, nTime);
#endif
            }
            // UPGRADE (level tang) -> g? cu + áp m?i
            else if (pNode->m_Level < nLevel)
            {
#ifdef _SERVER
                //printf("[StateApply-Upgrade]   skill=%d fromLvl=%d -> toLvl=%d\n", nSkillID, pNode->m_Level, nLevel);
#endif
                pTemp = (KMagicAttrib*)pData;
                for (int i = 0; i < nDataNum; i++)
                {
                    // g? tác d?ng cu
                    ModifyAttrib(nLauncher, &pNode->m_State[i]);
                    // áp tác d?ng m?i
                    ModifyAttrib(nLauncher, pTemp);
                    // luu giá tr? d?o d?u d? ti?n g? sau này
                    pNode->m_State[i].nAttribType = pTemp->nAttribType;
                    pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
                    pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
                    pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
                    pTemp++;
                }
                pNode->m_Level    = nLevel;
                pNode->m_LeftTime = nTime;
#ifdef _SERVER
                UpdateNpcStateInfo();
                BroadCastState();
                //printf("[StateApply-Upgrade-OK] skill=%d lvl=%d time=%d (broadcast)\n", nSkillID, nLevel, nTime);
#endif
            }
            return;
        }
        pNode = (KStateNode*)pNode->GetNext();
    }

    // ADD m?i
    pNode = new KStateNode;
    pNode->m_SkillID  = nSkillID;
    pNode->m_Level    = nLevel;
    pNode->m_LeftTime = nTime;
    _ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);

#ifdef _SERVER
    //printf("[StateApply-Add] (kind=%d) skill=%d lvl=%d time=%d\n", m_Kind, nSkillID, nLevel, nTime);
#endif

    KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillID, nLevel);
    if (pOrdinSkill)
        pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();
    else
        pNode->m_StateGraphics = 0;

#ifdef _SERVER
    // N?u có StateSpecialId -> thêm icon r?i broadcast
    if (pNode->m_StateGraphics)
    {
        AddStateInfo(pNode->m_StateGraphics);
        UpdateNpcStateInfo();
        BroadCastState();
        //printf("[StateApply-AddGfx] skill=%d gfx=%d (broadcast)\n", nSkillID, pNode->m_StateGraphics);
    }
    else
    {
        
        UpdateNpcStateInfo();
        BroadCastState();
        //printf("[StateApply-AddNoGfx] skill=%d (broadcast fallback)\n", nSkillID);
    }
#else
    
    /* Kenny
    if ((IsPlayer() || m_Kind == kind_normal) && pNode->m_StateGraphics)
    {
        AddStateInfo(pNode->m_StateGraphics);
    }*/
	if ((IsPlayer() || m_Kind == kind_normal) && pNode->m_StateGraphics)
	{
		AddStateInfo(pNode->m_StateGraphics);
	}

#endif

   
    pTemp = (KMagicAttrib*)pData;
    for (int i = 0; i < nDataNum; i++)
    {
        ModifyAttrib(nLauncher, pTemp);
        pNode->m_State[i].nAttribType = pTemp->nAttribType;
        pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
        pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
        pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
        pTemp++;
    }

    AddStateSkillList(pNode);
}
//

BOOL KNpc::ModifyMissleCollsion(BOOL bCollsion)
{
	if (bCollsion)
		return TRUE;

	if (g_RandPercent(m_CurrentPiercePercent))
		return TRUE;
	else
		return FALSE;
}


//

int KNpc::ModifyMissleLifeTime(int nLifeTime)
{
	if (IsPlayer())
	{
		return nLifeTime;
	}
	else
	{
		return nLifeTime;
	}
}

int	KNpc::ModifyMissleSpeed(int nSpeed)
{
	if (m_CurrentSlowMissle)
	{
		return nSpeed / 2;
	}
	return nSpeed;
}


//

BOOL KNpc::DoManyAttack()
{
	m_ProcessAI = 0;
	
	KSkill * pSkill =(KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	if (pSkill->GetChildSkillNum() <= m_SpecialSkillStep) 		
        goto ExitManyAttack;
#ifndef _SERVER
        m_DataRes.SetBlur(TRUE);
#endif
	
	m_Frames.nTotalFrame = pSkill->GetMissleGenerateTime(m_SpecialSkillStep);
	
	int x, y;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	
#ifndef _SERVER
	if (m_nPlayerIdx > 0)
		pSkill->PlayPreCastSound(m_nSex, x ,y);
	if (g_Random(2))
		m_ClientDoing = cdo_attack;
	else 
		m_ClientDoing = cdo_attack1;
#endif

	
	m_Doing = do_manyattack;
	
	m_Frames.nCurrentFrame = 0;

	return TRUE;

ExitManyAttack:

#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
	m_ProcessAI = 1;
	m_SpecialSkillStep = 0;

	return TRUE;
}


//

void KNpc::OnManyAttack()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;

		int nPhySkillId =  pSkill->GetChildSkillId();

		if (nPhySkillId > 0)
		{
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
			if (pOrdinSkill)
            {
				pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
            }
		}
		m_SpecialSkillStep ++;
		DoManyAttack();

	}	
}

// --
//
// --
BOOL	KNpc::DoRunAttack()
{
	m_ProcessAI = 0;
	switch(m_SpecialSkillStep)
	{
	case 0:
		m_Frames.nTotalFrame = REGION_CELL_SIZE_X / 4;
		m_ProcessAI = 0;
		
#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);

		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightrun;
		}
		else
		{
			m_ClientDoing = cdo_run;
		}
#endif
		
		if (m_DesX < 0 && m_DesY > 0) 
		{
			int x, y;
			SubWorld[m_SubWorldIndex].Map2Mps
				(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX, 
				Npc[m_DesY].m_MapY, 
				Npc[m_DesY].m_OffX, 
				Npc[m_DesY].m_OffY, 
				&x,
				&y
				);

		m_DesX = x;
		m_DesY = y;
		}

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 1:
#ifndef _SERVER
		if (g_Random(2))	
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;

		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif	
		m_Frames.nTotalFrame = 0;
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;
	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif		
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;
		
	return TRUE;
}
void KNpc::OnRunAttack()
{
	if(m_Doing == do_death || m_Doing == do_revive)
	{	
		DoStand();
#ifndef _SERVER
	if (IsPlayer())
	{
		DoStand();
		SendClientPos2Server();// them by kinnox 27/03/2024;
	}
#endif
	}
	if(m_SpecialSkillStep == 0)
	{
		OnRun(TRUE);
		KSkill* pSkill = (KSkill*)GetActiveSkill();
		if(!pSkill) 
            return ;
		
        if(m_Doing == do_stand || (DWORD)m_nCurrentMeleeTime > pSkill->GetMissleGenerateTime(0)) 
		{
			m_SpecialSkillStep++;
			m_nCurrentMeleeTime = 0;

			DoRunAttack();
		}
		else
		{
			m_nCurrentMeleeTime ++;
		}

		m_ProcessAI = 0;
	}
	else if(m_SpecialSkillStep == 1)
	{
		if( IsReachFrame(ATTACKACTION_EFFECT_PERCENT) )
		{
			KSkill* pSkill = (KSkill*)GetActiveSkill();
			if(!pSkill) 
                return ;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();
			if(nCurPhySkillId > 0)
			{
				KSkill* pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if(pOrdinSkill)
                {
				    pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
					if(pSkill->GetDelayPerCast() > 0)
					{
						m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());
						m_SkillList.SetMaxCastTime(m_ActiveSkillID, pSkill->GetDelayPerCast());
                
					}
				}
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}


//

BOOL KNpc::DoJumpAttack()
{
	m_ProcessAI = 0;
	
	switch(m_SpecialSkillStep)
	{
	case 0:
		DoJump();

#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);
		m_ClientDoing = cdo_jump;
#endif
		m_Doing = do_jumpattack;
		break;
		
	case 1:
#ifndef _SERVER
		if (g_Random(2))	
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;
		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_jumpattack;
		break;
		
	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}
	
	m_Frames.nCurrentFrame = 0;
	
	return TRUE;
	
}


//

BOOL KNpc::OnJumpAttack()
{
	if (m_SpecialSkillStep == 0)
	{
		if (!OnJump())
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;
			DoJumpAttack();
		}
		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;	
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill * pSkill =(KSkill*) GetActiveSkill();
			if (!pSkill) 
                return FALSE;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
	}
	return TRUE;
}


//

BOOL KNpc::CheckHitTarget(int nAR, int nDf, int nIngore/* = 0*/)
{
	int nDefense = nDf * (100 - nIngore) / 100;
	int nPercent = 0;
	if ((nAR + nDefense) == 0) 
		nPercent = 50;
	else
		nPercent = nAR * 100 / (nAR + nDefense);

	if (nPercent > MAX_HIT_PERCENT)
		nPercent = MAX_HIT_PERCENT;

	if (nPercent < MIN_HIT_PERCENT)
		nPercent = MIN_HIT_PERCENT;

	BOOL bRet = g_RandPercent(nPercent);
#ifdef _DEBUG
	//g_DebugLog("[ÊýÖµ]AttackRating %d : Defense %d: RandomPercent (%d, %d)", nAR, nDf, nPercent, bRet);
#endif
	return bRet;
}

// --
//
// --
/*
void KNpc::AddStateInfo(int nID)
{	
	// moi them by kinnox 03/08/2023;
	if (nID < 0 || nID >= MAX_NPC_RECORDER_STATE)
		return;
	//


	BOOL bEC = FALSE;
	int i = 0;
	for (i = 0; i < MAX_NPC_RECORDER_STATE; i++)
	{
		if (m_btStateInfo[i] == nID)
		{
			bEC = TRUE;
			break;
		}
	}
	if (bEC == TRUE)
		return;
	for (i = 0; i < MAX_NPC_RECORDER_STATE; i++)
	{
		if (m_btStateInfo[i] == 0)
			break;
	}
	if (i == MAX_NPC_RECORDER_STATE)
		i = 0;

	m_btStateInfo[i] = nID;
}
*/

//Kenny fix skill
void KNpc::AddStateInfo(int nID)
{
    if (nID <= 0 || nID > MAX_STATE_EFFECT_ID)
        return;
    for (int i = 0; i < MAX_NPC_RECORDER_STATE; ++i)
        if (m_btStateInfo[i] == nID)
            return;
    i = 0;
    for (; i < MAX_NPC_RECORDER_STATE; ++i)
        if (m_btStateInfo[i] == 0)
            break;

    if (i == MAX_NPC_RECORDER_STATE)
        i = 0;

    m_btStateInfo[i] = (BYTE)nID;
}


// --
//
// --
void KNpc::AddStateSkillList(KStateNode * pNewNode)
{
	BOOL bFound = FALSE;
	KStateNode	*pNode = NULL;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_SkillID == pNewNode->m_SkillID)
		{
			bFound = TRUE;
			break;	
		}
		pNode = (KStateNode*)pNode->GetNext();
	}

	if (bFound)
		return;
	m_StateSkillList.AddHead(pNewNode);
}

#ifndef _SERVER
// --
//
// --
void KNpc::AddStateSkillListClient(KStateNode * pNewNode)
{
	BOOL bFound = FALSE;
	KStateNode	*pNode = NULL;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_StateGraphics == pNewNode->m_StateGraphics)
		{
			bFound = TRUE;
			break;	
		}
		pNode = (KStateNode*)pNode->GetNext();
	}

	if (bFound)
		return;
	pNewNode->m_LeftTime = -1;
	//pNewNode->m_SkillID = 100;
	m_StateSkillList.AddHead(pNewNode);
}

// --
//
// --
void KNpc::RemoveStateSkillList(BYTE* pNpcState)
{
	int i = 0;
	BOOL bFound = FALSE;
	KStateNode	*pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	KStateNode * pTempNode = NULL;
	
	while (pNode)
	{
		pTempNode = pNode;
		pNode = (KStateNode *)pNode->GetNext();
		if (pTempNode->m_StateGraphics > 0)
		{
			for (i = 0; i < MAX_NPC_RECORDER_STATE; i++)
			{
				if ((*(pNpcState + i) != 0) && (*(pNpcState + i) == pTempNode->m_StateGraphics))
				{
					bFound = TRUE;
				}
			}
		} 
		if (!bFound)
		{
			pTempNode->Remove();
			delete pTempNode;
			pTempNode = NULL;
		}
		bFound = FALSE;
	}/*
	//fix ngon ram client
    KStateNode * pNode = (KStateNode*)m_StateSkillList.GetHead();
    KStateNode * pTempNode = NULL;
    while(pNode)
    {
            pTempNode = pNode;
            pNode = (KStateNode*) pNode->GetNext();
            pTempNode->Remove();
            delete pTempNode;
    }*/
    return;
}
#endif


//

void KNpc::GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel, int nSeries)
{
	// FIX: Add upper bounds checking to prevent array overflow crash
	if (nNpcTemplateId < 0 || nNpcTemplateId >= MAX_NPCSTYLE || nLevel < 1 || nLevel >= MAX_NPC_LEVEL)
		return;

	if(nSeries < series_metal || nSeries >= series_num)
		return;

	if(g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries])
	{
		LoadDataFromTemplate(nNpcTemplateId, nLevel, nSeries);
	}
	else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0][nSeries])
		{
			g_pNpcTemplate[nNpcTemplateId][0][nSeries] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->InitNpcBaseData(nNpcTemplateId);
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_bHaveLoadedFromTemplate = TRUE;
		}
		KLuaScript * pLevelScript = NULL;		

#ifdef _SERVER
			pLevelScript = (KLuaScript*)g_GetScript(
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_dwLevelSettingScript
			);
		
		if (pLevelScript == NULL)
			pLevelScript = g_pNpcLevelScript;
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			LevelScript.Init();
			LevelScript.RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_szLevelSettingScript))
			{
				pLevelScript = g_pNpcLevelScript;
			}
			else
			{
				pLevelScript = &LevelScript;
			}
		}
#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries] = new KNpcTemplate;
		*g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries] = *g_pNpcTemplate[nNpcTemplateId][0][nSeries];
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->m_nLevel = nLevel;
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->m_Series = nSeries;
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->InitNpcLevelData(&g_NpcKindFile, nNpcTemplateId, pLevelScript, nLevel, nSeries);
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->m_bHaveLoadedFromTemplate = TRUE;
		LoadDataFromTemplate(nNpcTemplateId, nLevel, nSeries);
	}
}


//

void	KNpc::LoadDataFromTemplate(int nNpcTemplateId, int nLevel, int nSeries)
{
	if (nNpcTemplateId < 0 )
		return ;
	
	KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries];

	strcpy(Name,pNpcTemp->Name);
	//
	if(m_CustomKind == kind_normal)
	{
		m_Kind = m_CustomKind;
	}
	else
	{
		m_Kind = pNpcTemp->m_Kind;
	}

	if(m_CustomCamp == camp_animal)
	{
		m_Camp = m_CustomCamp;
	}
	else
	{
		m_Camp = pNpcTemp->m_Camp;
	}
	
	m_Series	= pNpcTemp->m_Series;
	m_HeadImage =	pNpcTemp->m_HeadImage;
	m_bClientOnly = pNpcTemp->m_bClientOnly;
	m_CorpseSettingIdx =	pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame =	pNpcTemp->m_DeathFrame;
	m_WalkFrame =	pNpcTemp->m_WalkFrame;
	m_RunFrame =	pNpcTemp->m_RunFrame;
	m_HurtFrame =	pNpcTemp->m_HurtFrame;
	m_WalkSpeed =	pNpcTemp->m_WalkSpeed;
	m_AttackFrame =	pNpcTemp->m_AttackFrame;
	m_CastFrame =	pNpcTemp->m_CastFrame;
	m_RunSpeed =	pNpcTemp->m_RunSpeed;
	m_StandFrame =  pNpcTemp->m_StandFrame;
	m_StandFrame1 = pNpcTemp->m_StandFrame1;
	m_NpcSettingIdx = pNpcTemp->m_NpcSettingIdx;
	m_nStature		= pNpcTemp->m_nStature;

#ifdef _SERVER	
	m_Treasure		= pNpcTemp->m_Treasure;
	m_SkillList		= pNpcTemp->m_SkillList;
	m_AiMode		= pNpcTemp->m_AiMode;
	m_AiAddLifeTime	= 0;
	m_pDropRate		= pNpcTemp->m_pItemDropRate;

	if (!m_AiSkillRadiusLoadFlag)
	{
		m_AiSkillRadiusLoadFlag = 1;
		int i;
		for (i = 0; i < MAX_AI_PARAM - 1; i ++)
			m_AiParam[i] =	pNpcTemp->m_AiParam[i];

		int		nMaxRadius = 0, nTempRadius;
		KSkill	*pSkill;
		for (i = 1; i < MAX_NPC_USE_SKILL + 1; i++)
		{
			pSkill = (KSkill*)g_SkillManager.GetSkill(m_SkillList.m_Skills[i].SkillId, m_SkillList.m_Skills[i].SkillLevel);
			if (!pSkill)
				continue;
			nTempRadius = pSkill->GetAttackRadius();
			if (nTempRadius > nMaxRadius)
				nMaxRadius = nTempRadius;
		}
		m_AiParam[MAX_AI_PARAM - 1] = nMaxRadius * nMaxRadius;
	}

	m_FireResistMax			= pNpcTemp->m_FireResistMax;
	m_ColdResistMax			= pNpcTemp->m_ColdResistMax;
	m_LightResistMax		= pNpcTemp->m_LightResistMax;
	m_PoisonResistMax		= pNpcTemp->m_PoisonResistMax;
	m_PhysicsResistMax		= pNpcTemp->m_PhysicsResistMax;
	m_ActiveRadius			= pNpcTemp->m_ActiveRadius;
	m_VisionRadius			= pNpcTemp->m_VisionRadius;
	m_AIMAXTime				= pNpcTemp->m_AIMAXTime;
	m_HitRecover			= pNpcTemp->m_HitRecover;
	m_ReviveFrame			= pNpcTemp->m_ReviveFrame;
	m_Experience			= pNpcTemp->m_Experience;
	m_CurrentExperience		= m_Experience;
	m_LifeMax				= pNpcTemp->m_LifeMax;
	m_LifeReplenish			= pNpcTemp->m_LifeReplenish;
	m_AttackRating			= pNpcTemp->m_AttackRating;
	m_Defend				= pNpcTemp->m_Defend;
	m_PhysicsDamage			= pNpcTemp->m_PhysicsDamage;
	m_RedLum				= pNpcTemp->m_RedLum;
	m_GreenLum				= pNpcTemp->m_GreenLum;
	m_BlueLum				= pNpcTemp->m_BlueLum;
	m_FireResist			= pNpcTemp->m_FireResist;
	m_ColdResist			= pNpcTemp->m_ColdResist;
	m_LightResist			= pNpcTemp->m_LightResist;
	m_PoisonResist			= pNpcTemp->m_PoisonResist;
	m_PhysicsResist			= pNpcTemp->m_PhysicsResist;
	
	if(CustomName[0])
	{
		strcpy(Name, CustomName);
	}
	
#else
	m_LifeMax				= pNpcTemp->m_LifeMax;
	m_ArmorType				= pNpcTemp->m_ArmorType;
	m_HelmType				= pNpcTemp->m_HelmType;
	m_WeaponType			= pNpcTemp->m_WeaponType;
	m_HorseType				= pNpcTemp->m_HorseType;
	m_bRideHorse			= pNpcTemp->m_bRideHorse;
	strcpy(ActionScript, pNpcTemp->ActionScript);
#endif
	m_Level = pNpcTemp->m_nLevel;
	RestoreNpcBaseInfo();
	
#ifdef _SERVER
	if(m_SkillList.m_Skills[5].SkillId)
	{
		KSkill	*pSkill;
		pSkill = (KSkill*)g_SkillManager.GetSkill(m_SkillList.m_Skills[5].SkillId, m_SkillList.m_Skills[5].SkillLevel);
		if (pSkill)
		{
			pSkill->CastStateSkill(m_Index, 0, 0,-1);
		}
	}
	if(m_SkillList.m_Skills[6].SkillId)
	{
		KSkill	*pSkill;
		pSkill = (KSkill*)g_SkillManager.GetSkill(m_SkillList.m_Skills[6].SkillId, m_SkillList.m_Skills[6].SkillLevel);
		if (pSkill)
		{
			pSkill->CastStateSkill(m_Index, 0, 0, -1);
		}
	}

	if(SubWorld[m_SubWorldIndex].m_nAutoGoldenNpc > 0)
	{	
		m_cGold.RandChangeGold(SubWorld[m_SubWorldIndex].m_nAutoGoldenNpc, SubWorld[m_SubWorldIndex].m_btGoldenType);
	}

	// Exp Maps
	CONST KNPC_EXP_WORLD*		pNpcExp;
	pNpcExp = SubWorld[m_SubWorldIndex].GetNpcExp();

	if(pNpcExp->nExp > 0)
	{
		m_Experience = pNpcExp->nExp;
	}
#endif
	
}

// --
//
// --
void	KNpc::SetPhysicsDamage(int nMinDamage, int nMaxDamage)
{
	m_PhysicsDamage.nValue[0] = nMinDamage;
	m_PhysicsDamage.nValue[2] = nMaxDamage;
}

// --
//
// --
void	KNpc::SetBaseAttackRating(int nAttackRating)
{
	m_AttackRating = nAttackRating;
	m_CurrentAttackRating = m_AttackRating;
}

void	KNpc::SetBaseDefence(int nDefence)
{
	m_Defend = nDefence;
	m_CurrentDefend = m_Defend;
}

#ifdef _SERVER
void KNpc::DeathPunish(int nMode, int nBelongPlayer)
{
#define	LOSE_EXP_SCALE		10

	if (IsPlayer())
	{
		if (nMode == enumDEATH_MODE_NPC_KILL)
		{	
			//
			int nSubExp;
			if (m_Level <= 10)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 2) / 100;
			else if (m_Level <= 50)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 3) / 100;
			else if (m_Level <= 100)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 2) / 100;
			else if (m_Level <= 120)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 1) / 100;
			else if (m_Level <= 150)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 1) / 200;
			else
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level) * 1) / 300;

			if (Player[m_nPlayerIdx].m_nExp >= nSubExp)
				Player[m_nPlayerIdx].DirectAddExp( -nSubExp );
			else
				Player[m_nPlayerIdx].DirectAddExp( -Player[m_nPlayerIdx].m_nExp );
			

			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() / 2;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;

				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}
		}
		else if (nMode == enumDEATH_MODE_PLAYER_NO_PUNISH)
		{
			return;
		}
		else if (nMode == enumDEATH_MODE_PKBATTLE_PUNISH)
		{
			return;
		}
		else
		{
			char	cPKValue;
			cPKValue = Player[this->m_nPlayerIdx].m_cPK.GetPKValue();
			if (cPKValue < 0)
				cPKValue = 0;
			if (cPKValue > MAX_DEATH_PUNISH_PK_VALUE)
				cPKValue = MAX_DEATH_PUNISH_PK_VALUE;
			
			int nLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level);
			nLevelExp = (int)(nLevelExp * (PlayerSet.m_sPKPunishParam[cPKValue].m_nExp / 100.0));
			Player[m_nPlayerIdx].DirectAddExp(-nLevelExp);
			

			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() * PlayerSet.m_sPKPunishParam[cPKValue].m_nMoney / 100;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;
				
				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}

			Player[m_nPlayerIdx].m_ItemList.AutoLoseItemFromEquipmentRoom(PlayerSet.m_sPKPunishParam[cPKValue].m_nItem);

			if (g_Random(100) < PlayerSet.m_sPKPunishParam[cPKValue].m_nEquip)
			{
				Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}

			Player[m_nPlayerIdx].m_cPK.AddPKValue((char)NpcSet.m_nBeKilledAddPKValue);
			if (m_nLastDamageIdx)
			{
				if (Npc[m_nLastDamageIdx].IsPlayer())
				{
					int nLastDamagePlayerIdx = Npc[m_nLastDamageIdx].GetPlayerIdx();
					if(Player[nLastDamagePlayerIdx].m_cPK.GetNormalPKState() == 2 ||
						Player[nLastDamagePlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING)
					{
						KPlayerChat::MakeEnemy(Name, Npc[m_nLastDamageIdx].Name);
					}
				}
				
			}
		}
	}
	else if (nBelongPlayer > 0 && m_pDropRate)
	{
		for (int i = 0; i < m_CurrentTreasure; i++)
		{
			if (g_RandPercent(m_pDropRate->nMoneyRate))
			{
				LoseMoney(nBelongPlayer);
			}
			else
			{
				LoseSingleItem(nBelongPlayer);
			}
		}
	}
}

void	KNpc::PlayerDeadCreateMoneyObj(int nMoneyNum)
{
	int		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoneyNum);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		Object[nObjIdx].SetItemBelong(-1);
	}
}

void KNpc::LoseMoney(int nBelongPlayer)
{
	int nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	int nMoney = m_CurrentExperience * m_pDropRate->nMoneyScale / 100;
	if (nMoney <= 0)
		return;
	
	short nMoneyRate = PlayerSet.GetMoneyRate();
	nMoney = nMoney * nMoneyRate / 100;
	
	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoney);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		if (nBelongPlayer > 0)
			Object[nObjIdx].SetItemBelong(nBelongPlayer);
		else
			Object[nObjIdx].SetItemBelong(-1);
	}

}


void KNpc::LoseSingleItem(int nBelongPlayer)
{
	if (!m_pDropRate)
		return;

	if (m_pDropRate->nMaxItemLevelScale <= 0 || m_pDropRate->nMinItemLevelScale <= 0)
		return;

	int nRand = g_Random(m_pDropRate->nMaxRandRate);
	int nCheckRand = 0;

	int i;
	for (i = 0; i < m_pDropRate->nCount; i++)
	{
		if (nRand >= nCheckRand && nRand < nCheckRand + m_pDropRate->pItemParam[i].nRate)
		{
			break;
		}
		nCheckRand += m_pDropRate->pItemParam[i].nRate;
	}

	if (i == m_pDropRate->nCount)
		return;

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel, pnMagicLevel[6], nLevelMagic;
	int nMinItemLevel, nMaxItemLevel;
	
	nGenre = m_pDropRate->pItemParam[i].nGenre;
	nDetail = m_pDropRate->pItemParam[i].nDetailType;
	nParticular = m_pDropRate->pItemParam[i].nParticulType;
	nSeries = m_Series;
	nMinItemLevel = m_pDropRate->nMinItemLevel;
	nMaxItemLevel = m_pDropRate->nMaxItemLevel;
	int nModifyLuck = 0;
	nLuck = Player[nBelongPlayer].m_nCurLucky + nModifyLuck;
	nLevel = g_Random(nMaxItemLevel - nMinItemLevel + 1) + nMinItemLevel;
	if(nLevel < 1)
		nLevel = 1;
	else if (nLevel > 10)
		nLevel = 10;

	nLevelMagic = g_Random(m_pDropRate->nMaxItemLevelScale - m_pDropRate->nMinItemLevelScale + 1) + m_pDropRate->nMinItemLevelScale;
	int nItemMagicNum = 3 + g_Random(4);

	BOOL	bSkip = FALSE;
	int j;
	for (j = 0; j < 6; j++)
	{
		if (!bSkip)
		{
			if (g_Random(m_pDropRate->nMagicRate))
			{
				pnMagicLevel[j] = nLevelMagic;
				
			}
			else
			{
				pnMagicLevel[j] = 0;
				bSkip = TRUE;
			}
		}
		else
		{
			pnMagicLevel[j] = 0;
		}
	}
// Debug: Check pnMagicLevel before creating item
	printf("[NPC_DROP_DEBUG] Before ItemSet.Add | Genre:%d Level:%d Luck:%d | MagicLevel:[%d,%d,%d,%d,%d,%d]\n",
		nGenre, nLevel, nLuck,
		pnMagicLevel[0], pnMagicLevel[1], pnMagicLevel[2],
		pnMagicLevel[3], pnMagicLevel[4], pnMagicLevel[5]);
		
	const int nItemIdx = ItemSet.Add(nGenre, nSeries, nLevel, 0, nLuck, nDetail, nParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion());

	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return;
	
	// Debug: Check magic attributes right after item creation
	printf("[NPC_DROP_DEBUG] Item created: %s | Genre:%d | Attrib:[%d,%d,%d,%d,%d,%d]\n",
		Item[nItemIdx].GetName(),
		Item[nItemIdx].GetGenre(),
		Item[nItemIdx].GetAttribType(0),
		Item[nItemIdx].GetAttribType(1),
		Item[nItemIdx].GetAttribType(2),
		Item[nItemIdx].GetAttribType(3),
		Item[nItemIdx].GetAttribType(4),
		Item[nItemIdx].GetAttribType(5));
		
	int		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	KObjItemInfo sInfo;
	sInfo.m_nItemID = nItemIdx;
	sInfo.m_dwItemID = Item[nItemIdx].GetID();
	sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	if(Item[nItemIdx].GetGenre() == item_task ||
		Item[nItemIdx].GetGenre() == item_script)
	{
		char szTemp[FILE_NAME_LENGTH];
		if(Item[nItemIdx].GetMaxStackCount() > 0)
		{	
			sprintf(szTemp, "%s x%d", Item[nItemIdx].GetName(), Item[nItemIdx].GetStackCount());
			strcpy(sInfo.m_szName, szTemp);
		}
		else
		{
			sprintf(szTemp, "%s", Item[nItemIdx].GetName());
			strcpy(sInfo.m_szName, szTemp);
		}
	}
	else
	{
		strcpy(sInfo.m_szName, Item[nItemIdx].GetName());
	}
	sInfo.m_nColorID = 0;
	//AutoAI by kinnox;
	sInfo.m_nGenre = Item[nItemIdx].GetGenre();
	sInfo.m_nDetailType = Item[nItemIdx].GetDetailType();
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
	sInfo.m_dwNpcId = 0;
	//end code;
	nObj = ObjSet.Add(Item[nItemIdx].GetObjID(), Pos, sInfo);
	if (nObj == -1)
	{
		ItemSet.Remove(nItemIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
			Object[nObj].SetItemBelong(nBelongPlayer);
		else
			Object[nObj].SetItemBelong(-1);
	}
}


//	

void KNpc::Revive()
{	
#ifdef _SERVER
	int nOldSeries = m_Series;
	if(SubWorld[m_SubWorldIndex].m_bNpcSeriesAuto)
	{
		m_Series = ::GetRandomNumber(0,4);
	}

	if(nOldSeries != m_Series)
	{
		char	szName[32];
		DWORD	dwKind;
		int	nCamp;

		strcpy(szName, Name);
		dwKind = m_Kind;
		nCamp = m_CurrentCamp;

		Load(m_NpcSettingIdx, m_Level, m_Series);
		
		m_Kind = dwKind;
		m_Camp = nCamp;
		m_CurrentCamp = nCamp;
		
		if(m_pDropRate == NULL && SubWorld[m_SubWorldIndex].m_pNormalNpcDropRate)
		{	
			m_pDropRate = SubWorld[m_SubWorldIndex].m_pNormalNpcDropRate;
		}

		if (!szName)
			return ;
		g_StrCpyLen(Name, szName, sizeof(Name));
	}
	else
	{
		int nCamp = m_CurrentCamp;
		RestoreNpcBaseInfo();
		m_CurrentCamp = nCamp; 
	}
#endif


	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(m_OriginX, m_OriginY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	if (m_RegionIndex < 0)
		return;
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);

#ifdef _SERVER
	SubWorld[m_SubWorldIndex].NpcChangeRegion(VOID_REGION, nRegion, m_Index);
	//them by kinnox;
	if (!IsPlayer())
	{	
		if (m_ActionScriptID)
			NpcSet.ExecuteScript(m_Index,m_ActionScriptID, "OnRevive", m_Index); 
	}
	//end code;
#else
	SubWorld[0].NpcChangeRegion(VOID_REGION, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
	m_DataRes.SetBlur(FALSE);
#endif

	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	m_AiAddLifeTime = 0;
}

void KNpc::RestoreLiveData()
{

}
#endif

#ifdef	_SERVER

//

void	KNpc::SendDataToNearRegion(void* pBuffer, DWORD dwSize)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX, m_MapY);
	for (int i= 0; i < 8; i++)
	{
		if (SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i] < 0)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i]].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
}
#endif

#ifdef	_SERVER

//

int		KNpc::DeathCalcPKValue(int nKiller)
{
	if (nKiller <= 0 || nKiller >= MAX_NPC)
		return enumDEATH_MODE_NPC_KILL;
	
	if (m_nCurPKPunishState == enumDEATH_MODE_PKBATTLE_PUNISH)
		return enumDEATH_MODE_PKBATTLE_PUNISH;

	if (this->m_Kind != kind_player || Npc[nKiller].m_Kind != kind_player || !m_FightMode)
		return enumDEATH_MODE_NPC_KILL;

	if (Player[m_nPlayerIdx].m_cPK.GetExercisePKAim() == Npc[nKiller].m_nPlayerIdx)
		return enumDEATH_MODE_PLAYER_NO_PUNISH;

	if (Player[m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
		Player[m_nPlayerIdx].m_cPK.GetEnmityPKAim() == Npc[nKiller].m_nPlayerIdx)
	{
		if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.IsEnmityPKLauncher())
			Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nEnmityAddPKValue);
		return enumDEATH_MODE_PLAYER_PUNISH;
	}
	if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() != 2 && Player[Npc[nKiller].m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
	{
		Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_cKillPartnerPKAddPKValue);					
	}
	if (m_Level <= 50 && Npc[nKiller].m_Level * 2 >= m_Level * 3)
	{
		if ( !Player[m_nPlayerIdx].m_cPK.GetNormalPKState() )
		{
			if (Npc[nKiller].m_CurrentCamp == camp_free)
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nKillerPKFactionAddPKValue);
			else
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nFactionPKFactionAddPKValue);
		}
		return enumDEATH_MODE_PLAYER_PUNISH;
	}

	return enumDEATH_MODE_PLAYER_PUNISH;
}
#endif

#ifdef	_SERVER

//

BOOL	KNpc::CheckPlayerAround(int nPlayerIdx)
{
	if (nPlayerIdx <= 0 || m_RegionIndex < 0)
		return FALSE;
	if (SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].CheckPlayerIn(nPlayerIdx))
		return TRUE;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		if (SubWorld[m_SubWorldIndex].m_Region[nRegionNo].CheckPlayerIn(nPlayerIdx))
			return TRUE;
	}
	return FALSE;
}
#endif

#ifdef	_SERVER

//

void KNpc::SendBloodNo(DWORD dwID, int nBloodNo)
{
	int nNpcIndex = NpcSet.SearchID(dwID);
	if(nBloodNo > Npc[nNpcIndex].m_CurrentLifeMax && nBloodNo != 999999999)
	{
		nBloodNo = Npc[nNpcIndex].m_CurrentLifeMax;
	}

	BLOOD_SYNC Sync;
	Sync.ProtocolType	= s2c_bloodsync;
	Sync.m_dwNpcID		= dwID;
	Sync.m_nBloodNo		= nBloodNo;
	
	int nMaxCount = MAX_PLAYER;
	BROADCAST_REGION(&Sync, sizeof(Sync), nMaxCount);
}
#endif

#ifdef _SERVER
// --
//Ham tra ve mot Level ngau nhien dua vao phan tram truyen vao cac moc Level
int KNpc::getRandomLevel(const double percentages[], int numLevels) {
 	int totalPercentage = 0;
 	int i = 0;
    for ( i = 0; i < numLevels; ++i) {
        totalPercentage += percentages[i];
    }

    int randomNum = rand() % totalPercentage + 1;
    int cumulativePercentage = 0;

    for (i = 0; i < numLevels; ++i) {
        cumulativePercentage += percentages[i];
        if (randomNum <= cumulativePercentage) {
            return i;
        }
    }

    // Neu co loi, tra ve Level cuoi cung
    return numLevels - 1;
}

// Ham kiem tra xem mot trang bi co cot level bang 10 hay khong
int KNpc::IsMagicAttribLuckyLevel(int nLucky) 
{
	if (nLucky < 0) //Neu may man be hon 0 thi trang bi cap do trang bi roi ra bang 1;
	{
		printf("[KNpc] Loi nLucky < 0");
		return 1;
	}

	// Khai bao cac moc level va phan tram tuong ung
    const int numLevels = 10; //MAX LEVEL
    int i = 0;
    //		LEVEL                  1    2    3     4     5     6     7     8      9    10
    double percentages[numLevels] = {10.0, 20.0, 40.0, 20.0, 10.0, 00.0, 00.0, 0.0, 0.0, 0.0};
 	
    if (nLucky <= 3)
    {
    //		LEVEL                       1      2     3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {10.0, 20.0, 40.0, 20.0, 10.0, 00.0, 00.0, 0.0, 0.0, 0.0};
    }
    else if(nLucky >= 4 && nLucky <= 7)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {5.0, 20.0, 40.0, 20.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if(nLucky >= 8 && nLucky <= 11)
    {
     //		LEVEL                       1    2     3     4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 10.0, 30.0, 35.0, 15.0, 10.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if(nLucky >= 12 && nLucky <= 15)
    {
     //		LEVEL                       1    2    3     4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 5.0, 20.0, 35.0, 20.0, 15.0, 5.0, 0.0, 0.0, 0.0};
    }
    else if(nLucky >= 16 && nLucky <= 19)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 9.0, 20.0, 20.0, 25.0, 25.0, 1.0, 0.0, 0.0};
    }
    else if(nLucky >= 20 && nLucky <= 23)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 5.0, 15.0, 20.0, 20.0, 20.0, 19.0, 1.0, 0.0};
    }   
    else if(nLucky >= 24 && nLucky <= 27)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 15.0, 25.0, 30.0, 15.0, 10.0, 5.0, 0.0};
    }
    else if(nLucky >= 28 && nLucky <= 31)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 5.0, 15.0, 20.0, 20.0, 20.0, 19.0, 1.0};
    }
    else if(nLucky >= 32 && nLucky <= 35)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 5.0, 20.0, 30.0, 20.0, 22.0, 3.0};
    }    
    else if(nLucky >= 36 && nLucky <= 39)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 5.0, 25.0, 20.0, 20.0, 25.0, 5.0};
    }
    else if(nLucky >= 40 && nLucky <= 43)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 30.0, 25.0, 25.0, 10.0};
    } 
    else if(nLucky >= 44 && nLucky <= 47)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 25.0, 30.0, 27.0, 13.0};
    }
    else if(nLucky >= 48 && nLucky <= 51)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0, 30.0, 30.0, 15.0};
    }
    else if(nLucky >= 52 && nLucky <= 55)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 33.0, 30.0, 17.0};
    }
    else if(nLucky >= 56 && nLucky <= 59)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 35.0, 30.0, 20.0};
    }
    else if(nLucky >= 60)
    {
    //		LEVEL                       1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 30.0, 35.0, 25.0};
    }

    // Kiem tra tong phan tram co bang 100 hay khong
    double totalPercentage = 0.0;
    for (i = 0; i < numLevels; ++i) {
        totalPercentage += percentages[i];
    }
    if (totalPercentage != 100.0) {
        printf("[DROPRATE]Loi Level Item May Man: Tong phan tram khong bang 100!");
        return 1;
    }

	// Seed thoi gian hien tai da co gia tri ngau nhien khac nhau moi lan chay
	srand( (unsigned long)time(NULL) );

    // Lay ket qua ngau nhien dua vao phan tram
    int randomLevel = getRandomLevel(percentages, numLevels);

    // In ket qua ra man hinh
    //g_DebugLog("[DROPRATE]Ket qua ngau nhien: Level %d - Lucky: %d",randomLevel + 1,nLucky);
    return (randomLevel + 1);
}
// --
void KNpc::DropRateItem(int nCount, const char* pszFileName, int nUnknow, int nItemLevel, 
						   int nItemSeries, int nBelongIdx)
{	
	if(nCount <= 0)
		return;

	if(nBelongIdx <= 0 || nBelongIdx >= MAX_PLAYER)
		return;
	
	if(!pszFileName || !pszFileName[0])
		return;
	
	KItemDropRate* m_pItemDropRate = NULL;
	m_pItemDropRate = KNpcTemplate::UpdateDropRate((char*)pszFileName);
	if(!m_pItemDropRate)
		return;
	

	int nRand = 0;
	int nCheckRand = 0;
	int i, j, k;
	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel, pnMagicLevel[6], nMagicLevel;
	BOOL bSkip;
	int nIdx;
	int		nX, nY;
	//
	Coordinates equipmentCoordinates[57] = {
		{0,0}, {32,0}, {-32,0}, {0,32}, {0,-32}, 
		{32,32}, {32,-32}, {-32,32}, {-32, 32},  
		{64,0}, {-64,0}, {0,64}, {0,-64}, 
		{64,64}, {64,-64}, {-64,64}, {-64, 64},
		{96,0}, {-96,0}, {0,96}, {0,-96}, 
		{96,96}, {96,-96}, {-96,96}, {-96, 96},
		{128,0}, {-128,0}, {0,128}, {0,-128}, 
		{128,128}, {128,-128}, {-128,128}, {-128, 128},
		{160,0}, {-160,0}, {0,160}, {0,-160}, 
		{160,160}, {160,-160}, {-160,160}, {-160, 160},		
		{192,0}, {-192,0}, {0,192}, {0,-192}, 
		{192,192}, {192,-192}, {-192,192}, {-192, 192},	
		{224,0}, {-224,0}, {0,224}, {0,-224}, 
		{224,224}, {224,-224}, {-224,224}, {-224, 224},	
    };
    int equipmentNumber;
	//
	POINT	ptLocal;
	KMapPos	Pos;
	int nObj;
	KObjItemInfo sInfo;
	j = 0;
	GetMpsPos(&nX, &nY);
	while(j < nCount)
	{
		nRand = g_Random(m_pItemDropRate->nMaxRandRate);
		for (i = 0; i < m_pItemDropRate->nCount; i++)
		{
			if (nRand >= nCheckRand && nRand < nCheckRand + m_pItemDropRate->pItemParam[i].nRate)
			{
				break;
			}
			nCheckRand += m_pItemDropRate->pItemParam[i].nRate;
		}

		if (i == m_pItemDropRate->nCount)
			continue;

		nGenre = m_pItemDropRate->pItemParam[i].nGenre;
		nDetail = m_pItemDropRate->pItemParam[i].nDetailType;
		nParticular = m_pItemDropRate->pItemParam[i].nParticulType;
		nSeries = nItemSeries;
		nLuck = Player[nBelongIdx].m_nCurLucky;

		/*//tinh toan cap do trang bi roi ra by kinnox;
		nItemLevel = IsMagicAttribLuckyLevel(nLuck);
		////g_DebugLog("DEBUG: LEVEL O DAY : %d LUCKY: %d ",nItemLevel,nLuck);
		nLevel = nItemLevel;*/
		// Tinh toan do drop cho quai Kenny Fix
		if (nItemLevel > 0) {
		    nLevel = nItemLevel;    
		}
		else {
		    // Neu file ini co Min/Max thi dung no
		    if (m_pItemDropRate->nMinItemLevel > 0 && m_pItemDropRate->nMaxItemLevel > 0) {
		        nLevel = g_Random(m_pItemDropRate->nMaxItemLevel - m_pItemDropRate->nMinItemLevel + 1) 
		                 + m_pItemDropRate->nMinItemLevel;
		    } else {
		        nLevel = IsMagicAttribLuckyLevel(nLuck);
		    }
		}
		nMagicLevel = g_Random(m_pItemDropRate->nMaxItemLevelScale - m_pItemDropRate->nMinItemLevelScale) + m_pItemDropRate->nMinItemLevelScale;
		
		bSkip = FALSE;
		for (k = 0; k < 6; k++)
		{
			if (!bSkip)
			{
				 if (g_Random(m_pItemDropRate->nMagicRate))
				 {
					 pnMagicLevel[k] = nMagicLevel;
				 }
				 else
				 {
					 pnMagicLevel[k] = 0;
					 bSkip = TRUE;
				 }
			}
			else
			{
				pnMagicLevel[k] = 0;
			}
		}

		nIdx = ItemSet.Add(nGenre, nSeries, nLevel, 0, nLuck, nDetail, nParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion());
		
		if (nIdx <= 0 || nIdx >= MAX_ITEM)
			return;
		//drop theo toa do cho truoc toi da la 57 cap toa do by kinnox;
		equipmentNumber = j;
		if (equipmentNumber >= 0 && equipmentNumber <= 57) 
		{
	        Coordinates newCoordinates = {
	            nX + equipmentCoordinates[equipmentNumber].x,
	            nY + equipmentCoordinates[equipmentNumber].y
	        };   
	        ptLocal.x = newCoordinates.x;
			ptLocal.y = newCoordinates.y;
	    } 
	    else 
	    {
	        printf("[ERROR] Vuot qua 57 Item Droprate [ITEM DROP: %d] \n",j);
	        ptLocal.x = nX;
			ptLocal.y = nY;
	    }
	    //

		SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
		Pos.nSubWorld = m_SubWorldIndex;
		SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
			&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
			&Pos.nOffX, &Pos.nOffY);

		sInfo.m_nItemID = nIdx;
		sInfo.m_dwItemID = Item[nIdx].GetID();
		sInfo.m_nItemWidth = Item[nIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		//
		if(Item[nIdx].GetGenre() == item_task ||
			Item[nIdx].GetGenre() == item_script)
		{
			char szTemp[FILE_NAME_LENGTH];
			if(Item[nIdx].GetMaxStackCount() > 0)
			{	
				sprintf(szTemp, "%s x%d", Item[nIdx].GetName(), Item[nIdx].GetStackCount());
				strcpy(sInfo.m_szName, szTemp);
			}
			else
			{
				sprintf(szTemp, "%s", Item[nIdx].GetName());
				strcpy(sInfo.m_szName, szTemp);
			}
		}
		else
		{
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
		}
		
		sInfo.m_nColorID = 0;
		//AutoAI by kinnox;
		sInfo.m_nGenre = Item[nIdx].GetGenre();
		sInfo.m_nDetailType = Item[nIdx].GetDetailType();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_dwNpcId = 0;
		//
		nObj = ObjSet.Add(Item[nIdx].GetObjID(), Pos, sInfo);
		if (nObj == -1)
		{
			ItemSet.Remove(nIdx);
		}
		else
		{
			if (nBelongIdx > 0)
				Object[nObj].SetItemBelong(nBelongIdx);
			else
				Object[nObj].SetItemBelong(-1);
		}
		j++;
	}
}

//	

void	KNpc::DropObj(int nBelongPlayer, int nObjId, char* lpScript, int nRandRate, int nMaxRandRate)
{
	int nRand = g_Random(nMaxRandRate);
	
	if(nRand > nRandRate)
		return;

	int		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	int nObj = ObjSet.Add(nObjId, m_SubWorldIndex, ptLocal.x, ptLocal.y, 32, 1, lpScript);
	
	if(Object[nObj].GetKind() != Obj_Kind_Task)
		ObjSet.Remove(nObj);

	if (nBelongPlayer > 0)
		Object[nObj].SetItemBelong(nBelongPlayer);
	else
		Object[nObj].SetItemBelong(-1);
}
#endif

#ifndef _SERVER

//	

void	KNpc::SetMenuState(int nState, char *lpszSentence, int nLength)
{
	this->m_DataRes.SetMenuState(nState, lpszSentence, nLength);
}
#endif

#ifndef _SERVER

//	

int		KNpc::GetMenuState()
{
	return this->m_DataRes.GetMenuState();
}
#endif

#ifndef _SERVER

//	¹¦ÄÜ£º²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨ ID µÄ npc

DWORD	KNpc::SearchAroundID(DWORD dwID)
{
	int		nIdx, nRegionNo;
	nIdx = SubWorld[0].m_Region[m_RegionIndex].SearchNpc(dwID);
	if (nIdx)
		return nIdx;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[0].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nIdx = SubWorld[0].m_Region[nRegionNo].SearchNpc(dwID);
		if (nIdx)
			return nIdx;
	}
	return 0;
}
#endif

#ifndef _SERVER

//	¹¦ÄÜ£ºÉè¶¨ÌØÊâµÄÖ»²¥·ÅÒ»±éµÄËæÉísprÎÄ¼þ

void	KNpc::SetSpecialSpr(char *lpszSprName)
{
	m_DataRes.SetSpecialSpr(lpszSprName);
}
#endif

#ifndef _SERVER

//	¹¦ÄÜ£ºÉè¶¨Ë²¼äÌØÐ§

void	KNpc::SetInstantSpr(int nNo)
{
	char	szName[FILE_NAME_LENGTH];
	szName[0] = 0;
	NpcSet.m_cInstantSpecial.GetSprName(nNo, szName, sizeof(szName));
	if (szName[0])
		this->SetSpecialSpr(szName);
}
#endif

#ifndef _SERVER
int		KNpc::GetNormalNpcStandDir(int nFrame)
{
	return m_DataRes.GetNormalNpcStandDir(nFrame);
}
#endif

#ifdef _SERVER

//

void	KNpc::UpdateNpcStateInfo()
{
	int i = 0;
	memset(m_btStateInfo, 0 ,sizeof(BYTE) * MAX_NPC_RECORDER_STATE);
	KStateNode *pNode = NULL;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	if (m_ActiveAuraID)
	{
		int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
		int nPointAdd = m_SkillList.GetPointAdd(m_ActiveAuraID);		
		if (nLevel > 0)
		{
			int nSpecialID = 0;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel + nPointAdd);		//
			if (pOrdinSkill)
            {	
				
				nSpecialID = pOrdinSkill->GetStateSpecailId();
				AddStateInfo(nSpecialID);
				
            }
		}
	}

	while ( pNode && i < MAX_NPC_RECORDER_STATE)
	{	
		
		if (pNode->m_StateGraphics > 0)
		{
			AddStateInfo(pNode->m_StateGraphics);
		}
		
		pNode = (KStateNode*)pNode->GetNext();
	}

}




void	KNpc::BroadCastState()
{
	if (m_RegionIndex < 0)
		return;
	
	//
	NPC_SYNC_STATEINFO StateInfo;
	StateInfo.ProtocolType	= (BYTE)s2c_syncnpcstate;
	StateInfo.m_ID			= m_dwID;
	int i;
	for (i = 0; i < MAX_NPC_RECORDER_STATE; i++)
		StateInfo.m_btStateInfo[i] = m_btStateInfo[i];

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX, m_MapY);
	for (i= 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	
}
#endif

#ifdef _SERVER

//

void KNpc::AddPoisonState(int nLauncher, int nDamage, int nTime, int nCycle)
{	
	if(!nDamage || !nTime)
	{
	return;
	}
	if (m_PoisonState.nTime == 0)
	{
		m_PoisonState.nTime		= nTime;
		m_PoisonState.nValue[0]	= nDamage;
		m_PoisonState.nValue[1]	= nCycle;
	}
	else
	{
		int d, t, c;
		d	= m_PoisonState.nValue[0];
		t	= m_PoisonState.nTime;
		c	= m_PoisonState.nValue[1];
		if (c > 0 && nCycle > 0 && d > 0 && nDamage > 0) 
		{
			m_PoisonState.nTime	= (int)((1.0 * t * d * nCycle + 1.0 * nTime * nDamage * c) /(1.0 * d * nCycle + 1.0 * nDamage * c));
			
			int nDamageMax	= 123456;//123456;
			if( IsPlayer() )
			   nDamageMax = 60; //550 phe 5doc

			int nAddDamage;
			if (nDamageMax > 0)
			{
				nAddDamage = nDamageMax * nCycle * 2 - d * nCycle / c; //khuc nay11
				if (nDamage < nAddDamage)
					nAddDamage = nDamage;
			}					
			else
			{
				nAddDamage = nDamage;
			}
			if (nAddDamage > 0)
			{
				m_PoisonState.nValue[0]	= ((c + nCycle) * d / c + (c + nCycle) * nAddDamage / nCycle) / 2;
				m_PoisonState.nValue[1]	= (c + nCycle) / 2;
			}
		}
	}
	m_nLastPoisonDamageIdx	= nLauncher;
}
#endif

#ifndef _SERVER
void	KNpc::SetNpcState(BYTE* pNpcState)
{
	RemoveStateSkillList(pNpcState);		
	if (!pNpcState)
		return ;
	
	for (int i = 0; i < MAX_NPC_RECORDER_STATE; i++)
	{
		if (*(pNpcState + i) != 0)
		{
			KStateNode * pNewNode = new KStateNode;
			pNewNode->m_StateGraphics = *(pNpcState + i);
			AddStateSkillListClient(pNewNode);		
		}
	}
}
#endif
void	KNpc::ClearNpcState()
{
	KStateNode * pNode = (KStateNode*)m_StateSkillList.GetHead();
	KStateNode * pTempNode = NULL;
	while(pNode)
	{
		pTempNode = pNode;
		pNode = (KStateNode*) pNode->GetNext();
		pTempNode->Remove();
		delete pTempNode;
		pTempNode = NULL;
	}
	return;
}


void	KNpc::RestoreNpcBaseInfo()
{

	m_CurrentCamp = m_Camp;
	m_ActiveSkillID = 0;
	m_ActiveAuraID = 0;

	m_nPeopleIdx = 0;
	m_nLastDamageIdx = 0;
	m_nLastPoisonDamageIdx = 0;
	m_nObjectIdx = 0;

	m_CurrentLife			= m_LifeMax;
	m_CurrentLifeMax		= m_LifeMax;
	m_CurrentLifeReplenish	= m_LifeReplenish;
	m_CurrentMana			= m_ManaMax;
	m_CurrentManaMax		= m_ManaMax;
	m_CurrentManaReplenish	= m_ManaReplenish;
	m_CurrentStamina		= m_StaminaMax;
	m_CurrentStaminaMax		= m_StaminaMax;
	m_CurrentStaminaGain	= m_StaminaGain;
	m_CurrentStaminaLoss	= m_StaminaLoss;

	memset(&m_CurrentFireDamage, 0, sizeof(m_CurrentFireDamage));
	memset(&m_CurrentColdDamage, 0, sizeof(m_CurrentColdDamage));
	memset(&m_CurrentLightDamage, 0, sizeof(m_CurrentLightDamage));
	memset(&m_CurrentPoisonDamage, 0, sizeof(m_CurrentPoisonDamage));
	
	memset(&m_MagicPhysicsDamage, 0, sizeof(m_MagicPhysicsDamage));
	memset(&m_CurrentMagicFireDamage, 0, sizeof(m_CurrentMagicFireDamage));
	memset(&m_CurrentMagicColdDamage, 0, sizeof(m_CurrentMagicColdDamage));
	memset(&m_CurrentMagicLightDamage, 0, sizeof(m_CurrentMagicLightDamage));
	memset(&m_CurrentMagicPoisonDamage, 0, sizeof(m_CurrentMagicPoisonDamage));
	

	m_CurrentAttackRating	= m_AttackRating;
	m_CurrentDefend			= m_Defend;

	m_ManaShield			= 0;			
	m_DynamicMagicShield	= 0;			
	m_CurrentReturnResist	= 0;			

	m_CurrentFireResist		= m_FireResist;
	m_CurrentColdResist		= m_ColdResist;
	//fix dame ngu doc vao boss va quai 
	if (!IsPlayer())
	{
		if(m_cGold.GetGoldType() > 0)
		{	
			m_CurrentPoisonResist	 = 1;	
			m_CurrentPoisonResistMax = 1;
		}
		else
		{
			m_CurrentPoisonResist	= m_PoisonResist;
			m_CurrentPoisonResistMax = m_PoisonResistMax;
		}
	}
	else
	{
		m_CurrentPoisonResist	= m_PoisonResist;
		m_CurrentPoisonResistMax = m_PoisonResistMax;
	}

	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;
	m_CurrentFireResistMax	= m_FireResistMax;
	m_CurrentColdResistMax	= m_ColdResistMax;

	
	m_CurrentLightResistMax	= m_LightResistMax;
	m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
	

	m_CurrentWalkSpeed		= m_WalkSpeed;
	m_CurrentRunSpeed		= m_RunSpeed;
	m_CurrentAttackSpeed	= m_AttackSpeed;
	m_CurrentCastSpeed		= m_CastSpeed;
	m_CurrentVisionRadius	= m_VisionRadius;
	m_CurrentActiveRadius	= m_ActiveRadius;
	m_CurrentHitRecover		= m_HitRecover;
	m_CurrentTreasure		= m_Treasure;

	m_CurrentDamage2Mana	= 0;
	m_CurrentManaPerEnemy	= 0;
	m_CurrentLifeStolen		= 0;
	m_CurrentManaStolen		= 0;
	m_CurrentStaminaStolen	= 0;
	m_CurrentKnockBack		= 0;	
	m_CurrentDeadlyStrike	= 0;
	m_CurrentBlindEnemy		= 0;
	m_CurrentPiercePercent	= 0;
	m_CurrentFreezeTimeReducePercent	= 0;
	m_CurrentPoisonTimeReducePercent	= 0;
	m_CurrentStunTimeReducePercent		= 0;
	m_CurrentFireEnhance	= 0;
	m_CurrentColdEnhance	= 0;
	m_CurrentPoisonEnhance	= 0;
	m_CurrentLightEnhance	= 0;
	m_CurrentRangeEnhance	= 0;
	m_CurrentHandEnhance	= 0;
	m_CurrentExpEnhance		= 0;		
	ZeroMemory(m_CurrentMeleeEnhance, sizeof(m_CurrentMeleeEnhance));
	
	m_CurrentMeleeDmgRet		= 0;
	m_CurrentMeleeDmgRetPercent	= 0;
	m_CurrentRangeDmgRet		= 0;
	m_CurrentRangeDmgRetPercent = 0;
	m_PoisonDamageReturn		= 0;
	m_ReturnSkill				= 0;
	memset(&m_AutoReplySkill, 0, sizeof(m_AutoReplySkill));
	m_LifeReplenishPercent		= 0;
	m_IgnoreSkill				= 0;
	memset(&m_AutoAttackSkill, 0, sizeof(m_AutoAttackSkill));
	ZeroMemory(m_AppendSkill,sizeof(m_AppendSkill));
	memset(&m_AutoDeathSkill, 0, sizeof(m_AutoDeathSkill));
	memset(&m_AutoRescueSkill, 0, sizeof(m_AutoRescueSkill));
	m_StaticMagicShield		= 0;
	
	ClearStateSkillEffect();
	ClearNormalState();

}

#ifndef _SERVER
void KNpc::DrawBorder()
{
	if (m_Index <= 0)
		return;

	m_DataRes.DrawBorder();
}

int KNpc::DrawMenuState(int n)
{
	if (m_Index <= 0)
		return n;

	return m_DataRes.DrawMenuState(n);
}

#endif

#ifdef _SERVER
int KNpc::ChangeWorld(DWORD dwSubWorldID, int nX, int nY)
{
    int nTargetSubWorld = g_SubWorldSet.SearchWorld(dwSubWorldID);

    if (-1 == nTargetSubWorld && IsPlayer())
    {
        if (m_SubWorldIndex >= 0 && m_RegionIndex >= 0) {
            SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);
            SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);
        }

        Player[m_nPlayerIdx].m_nPrePayMoney = 0;
        TobeExchangeServer(dwSubWorldID, nX, nY);
        g_pServer->ShutdownClient(Player[m_nPlayerIdx].m_nNetConnectIdx);

        return 1;   // ÐèÒª¼ÓÇÐ»»·þÎñÆ÷µÄ´¦Àí -- spe
    }

	if( !IsPlayer() )
		return false;

/*	int nTargetSubWorld = g_SubWorldSet.SearchWorld(dwSubWorldID);
	if(nTargetSubWorld == -1)
	{
		/*if (m_SubWorldIndex >= 0)
			SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);

		TobeExchangeServer(dwSubWorldID, nX, nY);
		
		if(IsPlayer())
		{
			if(g_pServer)
				g_pServer->ShutdownClient(Player[m_nPlayerIdx].m_nNetConnectIdx);
		}
		//moi them
		//Kenny Kiem Tra neu dung Multi GS bi loi thi doi lai
		if (m_SubWorldIndex >= 0)
		SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);
		TobeExchangeServer(dwSubWorldID, nX, nY);
		//g_DebugLog("[Map]World%d haven't been loaded!", dwSubWorldID);
		//end
		return false;
	}
*/
	Player[m_nPlayerIdx].m_nPrePayMoney = 0;

	if(nTargetSubWorld == m_SubWorldIndex)
	{
		return SetPos(nX, nY);
	}
	
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[nTargetSubWorld].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if(nRegion < 0)
		return false;
	
	if(m_SubWorldIndex >= 0)
		SubWorld[m_SubWorldIndex].m_MissionArray.RemovePlayer(m_nPlayerIdx);
	
	if(m_SubWorldIndex >= 0 && m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
		NPC_REMOVE_SYNC	RemoveSync;
		RemoveSync.ProtocolType = s2c_npcremove;
		RemoveSync.ID = m_dwID;
		SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
		
	}

	int nSourceSubWorld = m_SubWorldIndex;
	int nSourceRegion = m_RegionIndex;

	m_SubWorldIndex = nTargetSubWorld;
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	SubWorld[nTargetSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &m_OriginX, &m_OriginY);//moi them by kinnox;
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddNpc(m_Index);	
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddRef(m_MapX, m_MapY, obj_npc);
	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;		

	if( IsPlayer() )
	{
		SubWorld[nTargetSubWorld].SendSyncData(m_Index, Player[m_nPlayerIdx].m_nNetConnectIdx);
		SubWorld[nSourceSubWorld].RemovePlayer(nSourceRegion, m_nPlayerIdx);
		SubWorld[nTargetSubWorld].AddPlayer(nRegion, m_nPlayerIdx);
		SYSTEMTIME systm;
		GetLocalTime(&systm);
		if (!m_FightMode)
		{
			if (systm.wSecond < 55)
				m_nImmortal = systm.wSecond + 5;
			else 
				m_nImmortal = systm.wSecond - 55;
		}
	}
	return true;
}
#endif

#ifdef _SERVER
void KNpc::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!IsPlayer())
	{
		return;
	}

	m_OldFightMode = m_FightMode;
	m_bExchangeServer = TRUE;
	if (m_nPlayerIdx > 0 && m_nPlayerIdx <= MAX_PLAYER)
	{
		Player[m_nPlayerIdx].TobeExchangeServer(dwMapID, nX, nY);
	}
}
#endif

BOOL KNpc::IsPlayer()
{
#ifdef _SERVER
	return m_Kind == kind_player;
#else
	return m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex;
#endif
}

//// ---
//
//// ---
BOOL KNpc::IsAlive()
{
	if(m_Doing == do_death)
	{
		return FALSE;
	}

	if(m_Doing == do_revive)
	{
		return FALSE;
	}
	
	return TRUE;
}

//// ---
//
//// ---
void KNpc::ClearStateSkillEffect()
{
	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	KStateNode* pTempNode = NULL; // sua by kinnox;
	while(pNode)
	{
		pTempNode = pNode;
		pNode = (KStateNode *)pNode->GetNext();

		if (pTempNode->m_LeftTime == -1)
			continue;

		if (pTempNode->m_LeftTime > 0)
		{
			for (int i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
			delete pTempNode;
#ifdef _SERVER
			UpdateNpcStateInfo();
			BroadCastState();
#endif
			pTempNode = NULL;
			continue;
		}
	}
	return;
}

//// ---
//
//// ---
void KNpc::ClearNormalState()
{
	ZeroMemory(&m_PoisonState, sizeof(m_PoisonState));
	ZeroMemory(&m_FreezeState, sizeof(m_FreezeState));
	ZeroMemory(&m_StunState, sizeof(m_StunState));
	ZeroMemory(&m_LifeState, sizeof(m_LifeState));
	ZeroMemory(&m_ManaState, sizeof(m_ManaState));
	ZeroMemory(&m_DrunkState, sizeof(m_DrunkState));
	
	ZeroMemory(&m_Poison2DecManaState, sizeof(m_Poison2DecManaState));
	ZeroMemory(&m_HideState, sizeof(m_HideState));
	ZeroMemory(&m_RandMoveState, sizeof(m_RandMoveState));
	ZeroMemory(&m_IgnoreNegativeState, sizeof(m_IgnoreNegativeState));
	
}

void KNpc::CheckTrap()
{
	if (m_Kind != kind_player)
		return;
	
	if (m_Index <= 0)
		return;

	if (m_SubWorldIndex < 0 || m_RegionIndex < 0)
		return;

	DWORD	dwTrap = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].GetTrap(m_MapX, m_MapY);
	if (m_TrapScriptID == dwTrap)
	{
		return;
	}
	else
	{
		m_TrapScriptID = dwTrap;
	}

	if (!m_TrapScriptID)
	{
		return;
	}

	Player[m_nPlayerIdx].ExecuteScript(m_TrapScriptID, "main", 0);
}

void KNpc::SetFightMode(BOOL bFightMode)
{
#ifdef _DEBUG
	//g_DebugLog("[DEATH]SetMode:%d", bFightMode);
#endif
	m_FightMode = bFightMode;
	m_PoisonState.nTime = 0; //moi them by kinnox 20/07/2023;

#ifdef _SERVER
	if (this->m_Kind == kind_player)
		Player[this->m_nPlayerIdx].m_cPK.CloseAll();
#endif
}

void KNpc::TurnTo(int nIdx)
{
	if (!Npc[nIdx].m_Index || !m_Index)
		return;

	int nX1, nY1, nX2, nY2;

	GetMpsPos(&nX1, &nY1);
	Npc[nIdx].GetMpsPos(&nX2, &nY2);

	m_Dir = g_GetDirIndex(nX1, nY1, nX2, nY2);
}

void KNpc::ReCalcStateEffect()
{
	KStateNode* pNode = NULL;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_LeftTime != 0)	// °üÀ¨±»¶¯(-1)ºÍÖ÷¶¯(>0)
		{
			int i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pNode->m_State[i].nAttribType)
				{
					KMagicAttrib	MagicAttrib;
					MagicAttrib.nAttribType = pNode->m_State[i].nAttribType;
					MagicAttrib.nValue[0] = -pNode->m_State[i].nValue[0];
					MagicAttrib.nValue[1] = -pNode->m_State[i].nValue[1];
					MagicAttrib.nValue[2] = -pNode->m_State[i].nValue[2];
					ModifyAttrib(m_Index, &MagicAttrib);
				}
			}
		}
		pNode = (KStateNode *)pNode->GetNext();
	}
}

#ifndef _SERVER
extern KTabFile g_ClientWeaponSkillTabFile;
#endif

int		KNpc::GetCurActiveWeaponSkill()
{
	int nSkillId = 0;
	if (IsPlayer())
	{
		
		int nDetailType = Player[m_nPlayerIdx].m_ItemList.GetWeaponType();
		int nParticularType = Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular();
		
		//½üÉíÎäÆ÷
		if (nDetailType == 0)
		{
			nSkillId = g_nMeleeWeaponSkill[nParticularType];
		}//Ô¶³ÌÎäÆ÷
		else if (nDetailType == 1)
		{
			nSkillId = g_nRangeWeaponSkill[nParticularType];
		}//¿ÕÊÖ
		else if (nDetailType == -1)
		{
			nSkillId = g_nHandSkill;
		}
	}
	else
	{
#ifdef _SERVER
		//Real Npc
		return 0;
#else
		if (m_Kind == kind_player) // No Local Player
		{
			g_ClientWeaponSkillTabFile.GetInteger(m_WeaponType + 1, "SkillId", 0, &nSkillId);
		}
		else						//Real Npc
		{
			return 0;//
		}
#endif
	}
	return nSkillId;
}

#ifndef _SERVER
void	KNpc::HurtAutoMove()
{
	if (this->m_Doing != do_hurt)
	return;
	
	if (m_sSyncPos.m_nDoing != do_hurt && m_sSyncPos.m_nDoing != do_stand)
		return;
	int	nFrames, nRegionIdx;
	nFrames = m_Frames.nTotalFrame - m_Frames.nCurrentFrame;
	if (nFrames <= 1)
	{
		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
	}
	else
	{
		nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
		if (nRegionIdx < 0)
			return;
		int		nNpcX, nNpcY, nSyncX, nSyncY;
		int		nNewX, nNewY, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Map2Mps(m_RegionIndex, 
			m_MapX, m_MapY,
			m_OffX, m_OffY,
			&nNpcX, &nNpcY);
		SubWorld[0].Map2Mps(nRegionIdx, 
			m_sSyncPos.m_nMapX, m_sSyncPos.m_nMapY,
			m_sSyncPos.m_nOffX, m_sSyncPos.m_nOffY,
			&nSyncX, &nSyncY);
		nNewX = nNpcX + (nSyncX - nNpcX) / nFrames;
		nNewY = nNpcY + (nSyncY - nNpcY) / nFrames;
		SubWorld[0].Mps2Map(nNewX, nNewY, &nRegionIdx, &nMapX, &nMapY, &nOffX, &nOffY);
		_ASSERT(nRegionIdx >= 0);
		if (nRegionIdx < 0)
			return;
		if (nRegionIdx != m_RegionIndex)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
		}
		else
		{
			SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
			SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
		}
	}
}

#endif

#ifndef _SERVER
void KNpc::ProcNetCommand(NPCCMD cmd, int x /* = 0 */, int y /* = 0 */, int z /* = 0 */)
{
	switch (cmd)
	{
	case do_death:
		DoDeath();
		break;
	case do_hurt:
		DoHurt(x, y, z);
		break;
	case do_revive:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
		SetInstantSpr(enumINSTANT_STATE_REVIVE);
		break;
	case do_stand:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
	default:
		break;
	}
}
#endif

#ifndef _SERVER
// --
//
// --
void	KNpc::ClearBlood(int i)
{
	m_btBloodAlpha[i]	= 0;
	m_btBloodTime[i]	= 0;
	m_szBloodNo[i][0]	= 0;
}
#endif

#ifndef _SERVER
// --
//
// --
void	KNpc::SetBlood(int nNo)
{
	if (nNo <= 0)
		return;

	int i;
	for (i = 0;i < 5;i++)
	{
		if (!m_szBloodNo[i][0])
			break;
	}
	if (i == 5) 
		i = 0;

	m_btBloodAlpha[i]		= 255;
	m_btBloodTime[i]		= defMAX_SHOW_BLOOD_TIME;

	if(nNo != 999999999)
	{
		sprintf(m_szBloodNo[i], "%d", nNo);
	}
	else
	{
		//sprintf(m_szBloodNo[i], "NÐ ");
	}
}
#endif

#ifndef _SERVER
// --
//
// --
int	KNpc::PaintBlood(int nHeightOffset)
{	
	int i;
	for (i = 0;i < 5;i++)
	{
		if (!m_szBloodNo[i][0])
			continue;
		
		int	nHeightOff = (int)(nHeightOffset + (defMAX_SHOW_BLOOD_TIME - m_btBloodTime[i]) * defSHOW_BLOOD_MOVE_SPEED);
		int nFontSize = 16;
		DWORD	dwColor = SHOW_BLOOD_COLOR | (m_btBloodAlpha[i] << 24);
		int		nMpsX, nMpsY;
		GetMpsPos(&nMpsX, &nMpsY);
		g_pRepresent->OutputText(nFontSize, m_szBloodNo[i], KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szBloodNo[i]) / 4, nMpsY, dwColor, 0, nHeightOff, 0xff000000);
		
		m_btBloodTime[i]--;
		if (m_btBloodTime[i] <= 0)
		{
			ClearBlood(i);
				continue;
		}
		m_btBloodAlpha[i]++;
		if (m_btBloodAlpha[i] > 31)
			m_btBloodAlpha[i] = 31;
	}

	return nHeightOffset;
}
#endif

#ifndef _SERVER
//// ---

//// ---
void KNpc::EditPos(bool bFlag)
{
	if(bFlag)
	{
		if(m_bEditPos == false)
		{
			m_bEditPos = bFlag;
		}
	}
	else
	{
		if(m_bEditPos == true)
		{
			m_bEditPos = bFlag;
		}
	}
}

//// ---

//// ---
void KNpc::SendClientPos2Server()
{
	if(m_Doing == do_revive || m_Doing == cdo_death)
		return;

	PLAYER_POS_COMMAND    NetCommand;
	NetCommand.ProtocolType	= c2s_playerpos;
	NetCommand.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	NetCommand.dwTimePacker = GetTickCount();
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, GetMapX(), GetMapY(), GetOffX(), GetOffY(), &NetCommand.m_nMpsX, &NetCommand.m_nMpsY);	
	if(g_pClient)
	{
		g_pClient->SendPackToServer(&NetCommand, sizeof(NetCommand));
	}
}
#endif

#ifdef _SERVER
//// ---

//// ---
void KNpc::BroadCastPosition()
{
	if (IsPlayer())
	{
		if (m_nImmortal)
		return;
	}
	PLAYER_POS_SYNC	PosInfo;

	PosInfo.ProtocolType	= s2c_playerpos;
	PosInfo.m_dwNpcID		= m_dwID;
	GetMpsPos(&PosInfo.m_nMpsX, &PosInfo.m_nMpsY);
	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&PosInfo, sizeof(PosInfo), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&PosInfo, sizeof(PosInfo), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (PLAYER_POS_SYNC*)&PosInfo, sizeof(PosInfo));	
}
#endif

#ifndef _SERVER
int	KNpc::GetNpcPate()
{
	int nHeight = m_Height + m_nStature;
	if (m_Kind == kind_player)
	{
		if (m_nSex)
			nHeight += 84;
		else
			nHeight += 84;
		
		if (!m_wMaskType)
		{
			if (m_Doing == do_sit && MulDiv(10, m_Frames.nCurrentFrame, m_Frames.nTotalFrame) >= 8)
				nHeight -= MulDiv(30, m_Frames.nCurrentFrame, m_Frames.nTotalFrame);
			
			if (m_bRideHorse)
				nHeight += 38;
		}
		else
		{
			nHeight += 10;
		}
	}

	return nHeight;
}
#endif

#ifndef _SERVER
int	KNpc::GetNpcPatePeopleInfo()
{
	int nFontSize = 12;
	if (m_nChatContentLen > 0 && m_nChatNumLine > 0)
		return m_nChatNumLine * (nFontSize + 1);

	int nHeight = 0;
	if (NpcSet.CheckShowLife())
	{
		if (m_Kind == kind_player ||
			m_Kind == kind_partner)
		{
			if (m_CurrentLifeMax > 0 &&
				(relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
				)
				nHeight += SHOW_LIFE_HEIGHT;
		}
	}
	if (NpcSet.CheckShowName())
	{
		if (nHeight != 0)
			nHeight += SHOW_SPACE_HEIGHT;

		if (m_Kind == kind_player || m_Kind == kind_dialoger)
			nHeight += nFontSize + 1;
	}
	return nHeight;
}
#endif

//AutoAI by kinnox;
#ifndef _SERVER
BOOL KNpc::FindStateSkill(int nID)
{
	if (nID == 0)
		return FALSE;
	KStateNode* pNode = NULL;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_SkillID == nID)
		{
			return TRUE;
		}
		pNode = (KStateNode *)pNode->GetNext();
	}
	return FALSE;
}
#endif
//	
//	

void KNpc::GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel, int nSeries)
{
// FIX: Add bounds checking to prevent array overflow crash with mask items
	// Bug: Mask items can have ID >= MAX_NPCSTYLE (2500), causing array overflow
	// when accessing g_pNpcTemplate[nNpcTemplateId]
	// Symptoms: Game crashes when hovering over mask item or equipping it
	if (nNpcTemplateId < 0 || nNpcTemplateId >= MAX_NPCSTYLE || nLevel < 1 || nLevel >= MAX_NPC_LEVEL || nSeries < 0 || nSeries >= MAX_SERIES)
		return ;

	if(g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries])
		LoadFrameFromTemplate(nNpcTemplateId, nLevel, nSeries);
	else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0][nSeries])
		{
			g_pNpcTemplate[nNpcTemplateId][0][nSeries] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->InitNpcBaseData(nNpcTemplateId);
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_bHaveLoadedFromTemplate = TRUE;
		}
		KLuaScript * pLevelScript = NULL;		
		
#ifdef _SERVER
			pLevelScript = (KLuaScript*)g_GetScript(
			g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_dwLevelSettingScript
			);
		
		if (pLevelScript == NULL)
			pLevelScript = g_pNpcLevelScript;
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			LevelScript.Init();
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0][nSeries]->m_szLevelSettingScript))
			{
				pLevelScript = g_pNpcLevelScript;
			}
			else
			{
				pLevelScript = &LevelScript;
			}
		}
#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries] = new KNpcTemplate;
		*g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries] = *g_pNpcTemplate[nNpcTemplateId][0][nSeries];
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->m_nLevel = nLevel;
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->InitNpcLevelData(&g_NpcKindFile, nNpcTemplateId, pLevelScript, nLevel, nSeries);
		g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries]->m_bHaveLoadedFromTemplate = TRUE;
		LoadFrameFromTemplate(nNpcTemplateId, nLevel, nSeries);
	}
}


//	

void	KNpc::LoadFrameFromTemplate(int nNpcTemplateId, int nLevel, int nSeries)
{
	if (nNpcTemplateId < 0 )
		return;
	
	KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel][nSeries];

	m_HeadImage			= pNpcTemp->m_HeadImage;
	m_CorpseSettingIdx	= pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame		= pNpcTemp->m_DeathFrame;
	m_WalkFrame			= pNpcTemp->m_WalkFrame;
	m_RunFrame			= pNpcTemp->m_RunFrame;
	m_HurtFrame			= pNpcTemp->m_HurtFrame;
	m_StandFrame		= pNpcTemp->m_StandFrame;
	m_StandFrame1		= pNpcTemp->m_StandFrame1;
#ifndef _SERVER
	m_ArmorType			= pNpcTemp->m_ArmorType;
	m_HelmType			= pNpcTemp->m_HelmType;
	m_WeaponType		= pNpcTemp->m_WeaponType;
#endif
}


//	

void	KNpc::ReSetRes(BOOL bMask)
{
	
#ifndef _SERVER
	char	szNpcTypeName[32];
#endif
	if(!bMask)
	{
#ifndef _SERVER
		if (m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "MainMan");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "MainLady");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	= NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetFrameCopyFromTemplate(m_wMaskType, 1, 0);
#ifndef _SERVER
		g_NpcSetting.GetString(m_wMaskType + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(2, "CharacterName", "", szNpcTypeName, sizeof(szNpcTypeName));
		}
#endif
	}

#ifndef _SERVER
	RemoveRes();
	m_DataRes.Init(szNpcTypeName, &g_NpcResList);
	m_DataRes.SetAction(m_ClientDoing);
	m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
	
	m_DataRes.SetMantle(m_MantleType);
	
#endif
}


//	

void	KNpc::AutoDoSkill(int nSkillId, int nLevel, int nX, int nY)
{	
	if(m_AutoReplySkill.nValue[2] > 0)
	{	
		if(nSkillId == 20)
		{
			if(!m_SkillList.CanCast(nSkillId, SubWorld[m_SubWorldIndex].m_dwCurrentTime))
				return;
		}
	}

	if(m_AutoAttackSkill.nValue[2] > 0)
	{	
		if(nSkillId == 719)
		{
			if(!m_SkillList.CanCast(nSkillId - 8, SubWorld[m_SubWorldIndex].m_dwCurrentTime))
				return;
		}
		else if(nSkillId == 720)
		{
			if(!m_SkillList.CanCast(nSkillId - 6, SubWorld[m_SubWorldIndex].m_dwCurrentTime))
				return;
		}
	}

	if(m_AutoRescueSkill.nValue[0] > 0)
	{
		if(nSkillId == 721)
		{
			if(!m_SkillList.CanCast(nSkillId - 4, SubWorld[m_SubWorldIndex].m_dwCurrentTime))
				return;
		}
	}

	KSkill * pSkill = (KSkill *)g_SkillManager.GetSkill(nSkillId, nLevel);
	if(!pSkill)
		return;
	
	eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();

#ifdef _SERVER
	NPC_SKILL_SYNC	NetCommand;
	
	NetCommand.ProtocolType = s2c_skillcast;
	NetCommand.ID = m_dwID;
	NetCommand.nSkillID = nSkillId;
	NetCommand.nSkillLevel = nLevel;

	if (nY <= 0 ) 
	{
		return;
	}
	
	NetCommand.nMpsX = nX;
	if (nX == -1)
	{
		NetCommand.nMpsY = Npc[nY].m_dwID;
		if (0 == NetCommand.nMpsY || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return;
	}
	else
	{
		NetCommand.nMpsY = nY;
	}

	
	NetCommand.m_bAuto	= TRUE;
	

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	NetCommand.ProtocolType = s2c_playerautodoskill;
	g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (NPC_SKILL_SYNC*)&NetCommand, sizeof(NetCommand));	

#endif
	if (eStyle == SKILL_SS_Missles 
		|| eStyle == SKILL_SS_Melee 
		|| eStyle == SKILL_SS_InitiativeNpcState 
		|| eStyle == SKILL_SS_PassivityNpcState)
	{
#ifndef _SERVER
		m_DataRes.StopSound();
		int x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		
		if (nY < 0)
			return;
		
		if (nX < 0)
		{
			if (nX != -1) 
				return;
			
			if (nY >= MAX_NPC || Npc[nY].m_dwID == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
				return;
			Npc[nY].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = nX;
			ty = nY;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
		if (pSkill->GetPreCastEffectFile()[0])
			m_DataRes.SetSpecialSpr((char *)pSkill->GetPreCastEffectFile());
		
		if (IsPlayer())
			pSkill->PlayPreCastSound(m_nSex,x, y);
		
		if (pSkill->IsNeedShadow())		
			m_DataRes.SetBlur(TRUE);
		else
			m_DataRes.SetBlur(FALSE);
#endif
		if (pSkill->GetSkillStyle() == SKILL_SS_Melee)
		{
			CastMeleeSkill(pSkill);
		}
		else
		{	
			if (m_AutoReplySkill.nValue[2] > 0)
			{	
				pSkill->Cast(m_Index, nX, nY);
			
				USHORT nDelay = 0;
				nDelay = m_AutoReplySkill.nValue[2] / 256;
				
				if(nSkillId == 20)
				{
					m_SkillList.SetNextCastTime(nSkillId, SubWorld[m_SubWorldIndex].m_dwCurrentTime + nDelay);
					m_SkillList.SetMaxCastTime(nSkillId, nDelay);
				}
				return;
			}

			if (m_AutoAttackSkill.nValue[2] > 0)
			{	
				pSkill->Cast(m_Index, nX, nY);
								
				USHORT nDelay = 0;
				nDelay = m_AutoAttackSkill.nValue[2] / 256;
				
				if(nSkillId == 719)
				{
					m_SkillList.SetNextCastTime(nSkillId - 8, SubWorld[m_SubWorldIndex].m_dwCurrentTime + nDelay);
					m_SkillList.SetMaxCastTime(nSkillId - 8, nDelay);
				}
				else if(nSkillId == 720)
				{
					m_SkillList.SetNextCastTime(nSkillId - 6, SubWorld[m_SubWorldIndex].m_dwCurrentTime + nDelay);
					m_SkillList.SetMaxCastTime(nSkillId - 6, nDelay);
				}
				return;
			}

			if (m_AutoRescueSkill.nValue[2] > 0)
			{	
				pSkill->Cast(m_Index, nX, nY);
				
				WORD wDelay = 0;
				wDelay = m_AutoRescueSkill.nValue[2] / 256;
				
				if(nSkillId == 721)
				{
					m_SkillList.SetNextCastTime(nSkillId - 4, SubWorld[m_SubWorldIndex].m_dwCurrentTime + wDelay);
					m_SkillList.SetMaxCastTime(nSkillId - 4, wDelay);
				} 
				return;
			}
			pSkill->Cast(m_Index, nX, nY);
		}
	}
}

//-
// Di chuyen ngau nhien
//-
void	KNpc::RandMove(int nX, int nY, BOOL bFlag)
{
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;

	if (bFlag)
	{
		int nPlayerX, nPlayerY;
		int nDestX, nDestY, dX, dY;
		GetMpsPos(&nPlayerX, &nPlayerY);
		dX = nPlayerX & 0x1F; 
		dY = nPlayerY & 0x1F; 
		nDestX = nX + 0x10 - dX;
		nDestY = nY + 0x10 - dY;

		if (m_Kind == kind_player)
		{
			SendCommandRandMove(nDestX, nDestY);
#ifdef _SERVER
			NPC_RANDMOVE_SYNC Sync;
			Sync.ProtocolType = s2c_npcrandmove;
			Sync.m_nMpsX = nDestX;
			Sync.m_nMpsY = nDestY;
			POINT	POff[8] = 
			{
				{0, 32},
				{-16, 32},
				{-16, 0},
				{-16, -32},
				{0, -32},
				{16, -32},
				{16, 0},
				{16, 32},
			};

			int nMaxCount = MAX_BROADCAST_COUNT;
			CURREGION.BroadCast(&Sync, sizeof(NPC_RANDMOVE_SYNC), nMaxCount, m_MapX, m_MapY);
			int i;
			for (i = 0; i < 8; i++)
			{
				if (CONREGIONIDX(i) == -1)
					continue;
				CONREGION(i).BroadCast(&Sync, sizeof(NPC_RANDMOVE_SYNC), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
			}
			g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (NPC_RANDMOVE_SYNC*)&Sync, sizeof(Sync));
#endif
		}
		else
		{
			SendCommandRandMove(nDestX, nDestY);
		}
	}
}

#ifdef _SERVER
//-
//
//-
void	KNpc::ExecuteRevive()
{	
	if(IsAlive())
	{
		DoDeath();
	}
	DoRevive();
}
#endif

#ifndef _SERVER
//-
//
//-
void	KNpc::ExecuteStand()
{	
	DoStand();	
	m_ProcessAI = 1;
	m_ProcessState = 1;
	
}
#endif

//-
//
//-
void KNpc::SetTitleEffect(int SpeicalGraphicId)
{		
#ifdef _SERVER
	if (IsPlayer())
	{
		STATE_EFFECT_SYNC	Sync;
		Sync.ProtocolType	= s2c_syncstateeffect;
		Sync.m_dwSkillID	= SpeicalGraphicId;
		Sync.m_nLevel		= -1;
		Sync.m_nTime		= -1;
		Sync.m_bRemove		= FALSE;		
		memset(Sync.m_MagicAttrib, 0, sizeof(KMagicAttrib));
		Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * MAX_SKILL_STATE - 1;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
	}
#endif
	KStateNode* pNode = NULL;
	KStateNode* pTempNode = NULL;
		
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{	
		pTempNode = (KStateNode *)pNode->GetNext();
		if(pNode->m_SkillID == -1)
		{
			pNode->Remove();
			delete pNode;
#ifdef _SERVER
			UpdateNpcStateInfo();
			BroadCastState();
#endif
			pNode = NULL;
		}
		pNode = pTempNode;
	}
	delete pTempNode;
	pTempNode = NULL;
	if(SpeicalGraphicId <= 0)
		return;

	pNode = new KStateNode;
	pNode->m_SkillID = -1;
	pNode->m_Level = -1;
	pNode->m_LeftTime = -1;
	pNode->m_StateGraphics = SpeicalGraphicId;

#ifdef _SERVER	
	if (SpeicalGraphicId) 
	{	
		
		AddStateInfo(SpeicalGraphicId);
		UpdateNpcStateInfo();
		
		BroadCastState();	
	}
#else
	if (IsPlayer() && SpeicalGraphicId) 
	{	
		
		AddStateInfo(SpeicalGraphicId);
		
	}
#endif
	AddStateSkillList(pNode);
}

#ifdef _SERVER

//	

void KNpc::SetTimer(DWORD nTime, int nTimerTaskId)					
{
	if (!nTime || !nTimerTaskId) 
		return;
	m_TimerTask.SetTimer(nTime, nTimerTaskId);
}
#endif

#ifdef _SERVER

//	

void KNpc::CloseTimer()							
{
	m_TimerTask.CloseTimer();
}
#endif

#ifdef _SERVER

//	

void KNpc::SaveTimerTask()							
{
	m_TimerTask.SaveTask(m_nPlayerIdx);
}
#endif

#ifdef _SERVER

//	

void KNpc::LoadTimerTask()							
{
	m_TimerTask.LoadTask(m_nPlayerIdx);
}
#endif

#ifdef _SERVER



BOOL KNpc::ExecuteTimerScript(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) 
		return FALSE;
	return ExecuteTimerScript(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}
#endif

#ifdef _SERVER



BOOL KNpc::ExecuteTimerScript(DWORD dwScriptId,  char * szFunName, int nParam)
{	
	try
	{	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			m_TimerScriptID = dwScriptId;
			if(IsPlayer())
				Lua_PushNumber(pScript->m_LuaState, m_nPlayerIdx);
			else
				Lua_PushNumber(pScript->m_LuaState, 0);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			if(IsPlayer())
				Lua_PushNumber(pScript->m_LuaState, Player[m_nPlayerIdx].m_dwID);
			else
				Lua_PushNumber(pScript->m_LuaState, 0);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if(pScript->CallFunction(szFunName, 1, "d", nParam))
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
			
		}
		if (bExecuteScriptMistake)
		{
			if(IsPlayer())
				Player[m_nPlayerIdx].m_bWaitingPlayerFeedBack = false;
			m_TimerScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		if(IsPlayer())
			Player[m_nPlayerIdx].m_bWaitingPlayerFeedBack = false;
		m_TimerScriptID = 0;
		return FALSE;
	}
	return TRUE;
}
#endif


#ifndef _SERVER
VOID KNpc::ClientGotoPos(INT nX, INT nY, INT nMode)
{
	INT nOrgX, nOrgY;
	GetMpsPos(&nOrgX, &nOrgY);

	if ((nMode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) || nMode == 2)
	{
		if (m_Doing == do_run)
		{
			m_DesX = nX;
			m_DesY = nY;
			SendClientCmdRun(m_DesX, m_DesY);
		}
		else
		{
			SendClientCmdRun(nX, nY);
		}
		SendCommand(do_run, nX, nY);
	}
	else
	{
		if (m_Doing == do_walk)
		{
			m_DesX = nX;
			m_DesY = nY;
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			SendClientCmdWalk(nX, nY);
		}
		SendCommand(do_walk, nX, nY);
	}
}
#endif


BOOL KNpc::ExecuteScript(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}

BOOL KNpc::ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam)
{
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_Index);
			pScript->SetGlobalName(SCRIPT_NPCINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_NPCID);
			
			Lua_PushNumber(pScript->m_LuaState, m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName,0, "d", nParam)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("[KNPC] Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams)
{
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;
		
		if (pScript)
		{
			m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_Index);
			pScript->SetGlobalName(SCRIPT_NPCINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_NPCID);
			
			Lua_PushNumber(pScript->m_LuaState, m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			pScript->SafeCallBegin(&nTopIndex);
			
			if ( (!szParams) || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName, 0,""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (pScript->CallFunction(szFunName,0, "sd", szParams,0)) 
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("[KNPC] Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


BOOL KNpc::ExecuteScript(char * ScriptFileName, char * szFunName, char *  szParams)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScript(dwScriptId, szFunName, szParams);
}


BOOL	KNpc::ExecuteScript2(DWORD dwScriptId, char * szFunName, char *  szParams1, char *  szParams2)
{
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;
		
		if (pScript)
		{
			m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_Index);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_NPCID);
			
			Lua_PushNumber(pScript->m_LuaState, m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			pScript->SafeCallBegin(&nTopIndex);
			
			if ( ((!szParams1) || !szParams1[0]) && ((!szParams2) || !szParams2[0])) 
			{
				if (pScript->CallFunction(szFunName, 0,""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (pScript->CallFunction(szFunName, 0, "ssd", szParams1, szParams2, 0)) 
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("[KNPC] Exception Have Caught When Execute Script[%u]!!!!!\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL	KNpc::ExecuteScript2(DWORD dwScriptId,  char * szFunName, int nParam1, int nParam2)
{
	try
	{
		bool bExecuteScriptMistake = true;
		int nTopIndex = 0;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_Index);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_NPCID);
			
			Lua_PushNumber(pScript->m_LuaState, m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName, 0, "dd", nParam1, nParam2)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("[KNPC] Exception Have Caught When Execute Script[%u]!!!!!\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL	KNpc::ExecuteScript2(char * ScriptFileName, char * szFunName, int nParam1, int nParam2)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript2(g_FileName2Id(ScriptFileName), szFunName, nParam1, nParam2);	
}

BOOL	KNpc::ExecuteScript2(char * ScriptFileName, char * szFunName, char *  szParams1, char *  szParams2)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0])
		return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScript2(dwScriptId, szFunName, szParams1, szParams2);
}

#ifdef _SERVER
void KNpc::MaintainAura()
{
    if (!m_ActiveAuraID) return;

    int nLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);
    int nPointAdd = m_SkillList.GetPointAdd(m_ActiveAuraID);
    if (nLevel <= 0) return;

    int nMpsX, nMpsY;
    SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);

    KSkill* pAura = (KSkill*)g_SkillManager.GetSkill(m_ActiveAuraID, nLevel + nPointAdd);
    if (!pAura) return;

    int nChildSkillId = pAura->GetChildSkillId();
    KSkill* pChild = (KSkill*)g_SkillManager.GetSkill(nChildSkillId, nLevel + nPointAdd);
    if (!pChild) return;

    pChild->Cast(m_Index, nMpsX, nMpsY);
}
#endif