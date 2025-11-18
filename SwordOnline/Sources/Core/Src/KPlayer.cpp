
#include "KCore.h"
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER 0x02A1
#endif
#include "KEngine.h"
#ifndef _SERVER
#include "../../Headers/IClient.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"

#endif

#ifdef _SERVER
	 #include "MSSQL/KB_MSSQL.h"
#endif

#include "KNpc.h"
#include "KObj.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KPlayer.h"
#include "LuaFuns.h"
#include "KSortScript.h"
#include "KScriptValueSet.h"
#include "KObjSet.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KSubWorldSet.h"
#include "KFile.h"
#include "GameDataDef.h"
#include "KBuySell.h"
//#include "MyAssert.h"
#include "MsgGenreDef.h"
#include "KItemSet.h"
#include "KTaskFuns.h"
#include "Text.h"

#ifdef _SERVER
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
#else
#include "CoreShell.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "ImgRef.h"
#include "Text.h"
#endif

extern int LuaGetNpcTalk(Lua_State *L);

#define		defPLAYER_LOGIN_TIMEOUT			5 * 18
#define		defPLAYER_SAVE_TIMEOUT			30 * 18//30 * 20 fix by kinnox;

#define		PLAYER_LEVEL_1_EXP				48
#define		PLAYER_LEVEL_ADD_ATTRIBUTE		5
#define		PLAYER_LEVEL_ADD_SKILL			1
#define		PLAYER_SHARE_EXP_DISTANCE		1024 //768 fix by kinnox;
#define		PLAYER_TEAM_EXP_ADD				50

#define		MAX_APPLY_TEAM_TIME				500

#define		BASE_WALK_SPEED					5
#define		BASE_RUN_SPEED					10

#define		BASE_FIRE_RESIST_MAX			75
#define		BASE_COLD_RESIST_MAX			75
#define		BASE_POISON_RESIST_MAX			75
#define		BASE_LIGHT_RESIST_MAX			75
#define		BASE_PHYSICS_RESIST_MAX			75

#define		BASE_ATTACK_SPEED				0
#define		BASE_CAST_SPEED					0
#define		BASE_VISION_RADIUS				120
#define		BASE_HIT_RECOVER				6

#define		TOWN_PORTAL_TIME				1800

#ifndef _SERVER

extern IClientCallback* l_pDataChangedNotifyFunc;
#endif

KPlayer	Player[MAX_PLAYER];
int		g_nLastNetMsgLoop;


//	

KPlayer::KPlayer()
{
	Release();
}


//	

KPlayer::~KPlayer()
{
	Release();
}


//	

void	KPlayer::Release()
{
#ifdef _SERVER
	m_nLixianStatus = 0; //Lixian by kinnox;
	m_nLixianTime = 0 ; //Lixian by kinnox;
#endif
#ifndef _SERVER
	m_RunStatus = 0;
	m_dwNextLevelLeadExp = 0;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nSendMoveFrames = defMAX_PLAYER_SEND_MOVE_FRAME;
	m_MouseDown[0] = FALSE;
	m_MouseDown[1] = FALSE;
	m_bDebugMode	= FALSE;
	m_cAI.Release(); //AutoAI by kinnox;
	ZeroMemory(m_szExpAddNo, sizeof(m_szExpAddNo));
	m_btExpAddTime	= 0;
	m_btExpAddAlpha	= 0;
	m_bHideNpc	= FALSE;//AutoAI by kinnox;
	m_bHidePlayer = FALSE;//AutoAI by kinnox;
	m_bHideAnimation = FALSE;//AutoAI by kinnox;	
	m_ItemList.RemoveAll();// them by kinnox 24/03/2024;
#endif
	m_dwID = 0;
	m_nIndex = 0;
	m_nNetConnectIdx = -1;
	m_cMenuState.Release();
	m_cTrade.Release();
	m_BuyInfo.Clear();// them by kinnox 24/03/2024;
	m_cShop.Release();
	m_cChat.Release();// them by kinnox 24/03/2024;

	m_nAttributePoint = 0;
	m_nSkillPoint = 0;
	
	m_nStrength = 0;
	m_nDexterity = 0;
	m_nVitality = 0;
	m_nEngergy = 0;
	m_nLucky = 0;
	m_nCurStrength = 0;
	m_nCurDexterity = 0;
	m_nCurVitality = 0;
	m_nCurEngergy = 0;
	m_nCurLucky = 0;
	m_btChatSpecialChannel = 0;
	
	m_nExp = 0;
	m_nNextLevelExp = PLAYER_LEVEL_1_EXP;
	m_bExchangeServer = FALSE;
	m_btRepositoryCount = 0;
	m_btExpandtoryCount = 0;//ExpandBox by kinnox;
	m_nLockPlayer	= 0;//LockPlayer by kinnox;
	m_dwLeadExp = 0;
	m_dwLeadLevel = 1;
	
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_bWaitingPlayerFeedBack = false;	
	//
	m_bActiveAuto	= FALSE;	// AutoAI by kinnox;	
	//
	m_bLockPlayer 	= FALSE; //LockPlayer by kinnox;
	m_bPass 	= FALSE; //LockPlayer by kinnox;
	//
#ifdef _SERVER
	m_bUseReviveIdWhenLogin = 0;
	m_dwDeathScriptId		= 0;
	m_dwLogoutScriptId		= 0;
	
	m_sLoginRevivalPos.m_nSubWorldID = 0;
	m_sLoginRevivalPos.m_nMpsX = 0;
	m_sLoginRevivalPos.m_nMpsY = 0;
	
	m_sDeathRevivalPos.m_nSubWorldID = 0;
	m_sDeathRevivalPos.m_nMpsX = 0;
	m_sDeathRevivalPos.m_nMpsY = 0;
	
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
	
	m_pLastScriptCacheNode = NULL;
	m_dwLoginTime			= -1;
	//	m_uLastPingTime			= -1;
	m_bFinishLoading = FALSE;
	m_uMustSave = SAVE_IDLE;
	m_bIsQuiting = FALSE;
	m_bSleepMode = FALSE;
	m_nLastNetOperationTime = 0;
	m_nPrePayMoney = 0;
	m_nForbiddenFlag = 0;
	m_nExtPoint = 0;
	m_nExtPoint2 = 0;
	m_nChangeExtPoint = 0;

	
	m_bLoginDelaySync	= true;
	m_bForbidTownPortal	= FALSE;
	m_wTitleId			= 0;
	m_cTabFile.Clear();
	m_nLastDialogIdx	= 0;
	
	memset(&m_nArrayPacket, 0,sizeof(m_nArrayPacket));
	// thang Chat GPT no keu giai phong cai bo nho nay by kinnox 24/03/2024;
	memset(&m_szTaskAnswerFun,0,sizeof(m_szTaskAnswerFun));
	ZeroMemory(&m_SaveBuffer,sizeof(m_SaveBuffer));
	memset(&m_nTimeScriptId, 0,sizeof(m_nTimeScriptId));
	memset(&m_AccoutName, 0,sizeof(m_AccoutName));
	//
	// m_cBaseLog.Clear();
	// m_cItemLog.Clear();
	// m_cScriptLog.Clear();
	m_dwGiveBoxId		= 0;//GiveBox by kinnox;
	m_dwRewardId 		= 0;//Reward by kinnox;
	m_dwTimeBoxId = 0;//TimeBox by kinnox;	
	m_ImagePlayer = 0;//Player Avatar by kinnox;
	//

#endif
	
	m_dwTimeHorse		= 0;
	m_nFortunePoint		= 0;
	
}


//

void	KPlayer::SetPlayerIndex(int nNo)
{
	if (nNo < 0)
		m_nPlayerIndex = 0;
	else
		m_nPlayerIndex = nNo;
}


//

int		KPlayer::GetPlayerIndex()
{
	return m_nPlayerIndex;
}


//	

void	KPlayer::Active()
{
#ifdef _SERVER
	//moi them
	if (m_nLixianStatus == 1)
	{
		/* moi them cho nay tuc la khi player uy thac thi khong check netconnect idex*/
	}
	else if (m_nNetConnectIdx == -1 || m_bExchangeServer)
		return;
	
	if( IsLoginDelaySync() )
	{
		if(g_SubWorldSet.GetGameTime() - m_dwLoginTime > defMAX_LOGIN_SCRIPT_LOAD_TIME)
		{
			ExecuteScript(PLAYER_LOGIN_SCRIPT_FILE, "main", 0);	
			if (m_nLockPlayer > 0)//LockPlayer by kinnox;
			{
				m_bLockPlayer = TRUE;
				S2C_LOCKPLAYER Cmd;
				Cmd.ProtocolType = s2c_lockplayer;
				Cmd.m_bLockPlayer = TRUE;
				Cmd.m_bPass = TRUE; 
				if(g_pServer && this->m_nNetConnectIdx != -1)
					g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
			}			
		}
		return;
	}
	this->SendCurNormalSyncData();
	
	this->m_cChat.TimerAdd();
	
	this->m_cPK.Active();
	//AutoAI by kinnox;
	if (g_SubWorldSet.GetGameTime()% 18 == m_nPlayerIndex % 18)
	{
		if (this->m_cTask.GetSaveVal(TV_ACTIVE_AUTO) > 0 && m_bActiveAuto == FALSE)
		{
			SetAutoFlag(TRUE);
		}
	}
	//
	// if (Npc[m_nIndex].m_FightMode == 0 && m_bSleepMode == FALSE && g_SubWorldSet.GetGameTime() - m_nLastNetOperationTime > defMAX_SLEEP_TIME)
	// {
		// NPC_SLEEP_SYNC	SleepSync;
		// SleepSync.ProtocolType = s2c_npcsleepmode;
		// SleepSync.bSleep = 1;
		// SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		// Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
		// m_bSleepMode = TRUE;
	// }
#else
	if ( !m_cTeam.m_nFlag )
	{
		if (m_cTeam.m_nApplyCaptainID > 0)
		{
			if ( m_cTeam.m_dwApplyTimer == 0 )
			{
				m_cTeam.m_nApplyCaptainID = 0;
				
			}
			else
			{
				m_cTeam.m_dwApplyTimer--;
				if ( !NpcSet.SearchID(m_cTeam.m_nApplyCaptainID) )
				{
					m_cTeam.m_nApplyCaptainID = 0;
					m_cTeam.m_dwApplyTimer = 0;
				}
			}
		}
	}
	else if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
	{
		for (int i = 0; i < MAX_TEAM_APPLY_LIST; i++)
		{
			if (m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
			{
				if (m_cTeam.m_sApplyList[i].m_dwTimer == 0)
				{
					m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
					m_cTeam.UpdateInterface();
				}
				else
				{
					m_cTeam.m_sApplyList[i].m_dwTimer--;
					if ( !Npc[this->m_nIndex].SearchAroundID(m_cTeam.m_sApplyList[i].m_dwNpcID) )
					{
						m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
						m_cTeam.m_sApplyList[i].m_dwTimer = 0;
						m_cTeam.UpdateInterface();
					}
				}
			}
		}
	}
	
	m_nSendMoveFrames++;
	
	this->m_cPK.Active();
	if (Npc[m_nIndex].m_bOpenShop <= 0)
		this->m_cAI.Active(); //AutoAI by kinnox;
#endif
}



//	

void	KPlayer::ProcessMsg(KWorldMsgNode *lpMsg)
{
	switch (lpMsg->m_dwMsgType)
	{
	case GWM_PLAYER_SKILL:
		break;
	case GWM_PLAYER_RUNTO:
		break;
	case GWM_PLAYER_WALKTO:
		break;
	case GWM_PLAYER_JUMPTO:
		break;
	default:
		break;
	}	
}

#ifndef _SERVER

//	

void KPlayer::ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
		if (wParam & MK_LBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		else if (wParam & MK_RBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		else
			OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
		
	case WM_LBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_left);
		break;
		
	case WM_LBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam), 
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		break;
		
	case WM_RBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_right);
		break;	
		
	case WM_RBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		break;
	}
}



//	

void KPlayer::OnButtonDown(int x,int y, int Key, MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = TRUE;
	
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	ProcessMouse(x, y, Key, nButton);
}



//	

void KPlayer::OnButtonMove(int x,int y,int Key, MOUSE_BUTTON nButton)
{
	if (m_MouseDown[(int)nButton])
	{
		ProcessMouse(x, y, Key, nButton);
	}
}



//	

void KPlayer::OnButtonUp(int x,int y,MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = 0;
}



//	

void KPlayer::OnMouseMove(int x,int y)
{
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	if (m_nPeapleIdx)
	{	
		
		if(Npc[m_nPeapleIdx].m_HideState.nTime > 0)
		{
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL); 
			return;
		}
		

		if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (NpcSet.GetRelation(m_nIndex, m_nPeapleIdx) == relation_enemy)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_FIGHT);
		
		else if(Npc[m_nPeapleIdx].m_bOpenShop)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		
		else
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
		
	}
	else if (m_nObjectIdx)
	{
		if (Object[m_nObjectIdx].m_nKind == Obj_Kind_MapObj)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Item || Object[m_nObjectIdx].m_nKind == Obj_Kind_Money || Object[m_nObjectIdx].m_nKind == Obj_Kind_Task)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_PICK);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Prop)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_USE);
	}
	else
		CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
}



//	

void KPlayer::ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton)
{
	if(CheckTrading())
		return;
	
	if(Npc[m_nIndex].m_bOpenShop)
		return;
	
	
	if (m_ItemList.m_Hand > 0)
	{
		if (nButton == button_left)
		{
			ThrowAwayItem();
			return;
		}
	}
	
	int nX = x;
	int nY = y;
	int nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
		
	if (Npc[m_nIndex].IsCanInput())
	{
		int nIdx = 0;
		
		if (nButton == button_right)
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nRightSkillID);
		}
		else
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
		}
		Npc[m_nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	if ((Key & MK_SHIFT) || (nButton == button_right))
	{
		if (Npc[m_nIndex].m_ActiveSkillID > 0)
		{
			ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID, 1);
			if (!pISkill) 
                return;
			
			if (pISkill->IsAura())
				return;
			int nAttackRange = pISkill->GetAttackRadius();
			
			int nTargetIdx = 0;
					
			if (pISkill->IsTargetAlly())
			{
				FindSelectNpc(x, y, relation_ally);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetEnemy())
			{
				FindSelectNpc(x, y, relation_enemy);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetObj())
			{
				FindSelectObject(x, y);
				if (m_nObjectIdx)
				{
					nTargetIdx = m_nObjectIdx;
				}
			}
			if (pISkill->IsTargetOnly() && !nTargetIdx)
            {
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
			
			if (m_nIndex == nTargetIdx)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
			
			if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_ActiveSkillID, SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
				||
				(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType() , pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
				)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return ;
			}
			
			if (!nTargetIdx)
			{
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, nX, nY);
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, nX, nY);
			}
			else
			{
				if (pISkill->IsTargetOnly())
				{
					int distance = NpcSet.GetDistance(m_nIndex , nTargetIdx);
					if (distance > pISkill->GetAttackRadius())
					{
						m_nPeapleIdx = nTargetIdx;
						return ;
					}
				}
				if (m_nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles) 
					return ;
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nTargetIdx);
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID);
			}
		}
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	
	if (Key == 0 )
	{
		int nRelation = NpcSet.GetRelation(m_nIndex, m_nPeapleIdx);
		if(nRelation == relation_enemy || nRelation == relation_dialog)
		{
			Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
		}
		
		if (m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
		{
			m_nPickObjectIdx = m_nObjectIdx;
			Npc[m_nIndex].m_nObjectIdx = m_nPickObjectIdx;
			if (!m_RunStatus)
			{
				Npc[m_nIndex].SendCommand(do_walk, nX, nY);
				SendClientCmdWalk(nX, nY);
			}
			else
			{
				Npc[m_nIndex].SendCommand(do_run, nX, nY);
				SendClientCmdRun(nX, nY);
			}
			m_nSendMoveFrames = 0;
		}
		return;
	}
}



//	

void KPlayer::Walk(int nDir, int nSpeed)
{
	int	nMapX = Npc[m_nIndex].m_MapX;
	int nMapY = Npc[m_nIndex].m_MapY;
	int	nOffX = Npc[m_nIndex].m_OffX;
	int	nOffY = Npc[m_nIndex].m_OffY;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nX, nY;
	
	SubWorld[nSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &nX, &nY);
	SubWorld[nSubWorld].GetMps(&nX, &nY, nSpeed * 2, nDir);
	
	if (m_RunStatus)
	{
		Npc[m_nIndex].SendCommand(do_run, nX, nY);
		//
		if (CheckTrading())
			return;
		if(Npc[m_nIndex].m_bOpenShop)
			return;
		
		SendClientCmdRun(nX, nY);
	}
	else
	{
		Npc[m_nIndex].SendCommand(do_walk, nX, nY);
		//
		if (CheckTrading())
			return;
		if(Npc[m_nIndex].m_bOpenShop)
			return;
		
		SendClientCmdWalk(nX, nY);
	}
}


//	

void KPlayer::TurnLeft()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir > 8)
		Npc[m_nIndex].m_Dir -= 8;
	else
		Npc[m_nIndex].m_Dir = MAX_NPC_DIR - 1;
}


//	

void KPlayer::TurnRight()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR - 9)
		Npc[m_nIndex].m_Dir += 8;
	else
		Npc[m_nIndex].m_Dir = 0;
}


//	

void KPlayer::TurnBack()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR / 2)
		Npc[m_nIndex].m_Dir += MAX_NPC_DIR / 2;
	else
		Npc[m_nIndex].m_Dir -= MAX_NPC_DIR / 2;
}


//	

void KPlayer::FindSelectNpc(int x, int y, int nRelation)
{
	int	nNpcIdx = 0;
	
	nNpcIdx = NpcSet.SearchNpcAt(x, y, nRelation, 40);
	
	if (Npc[nNpcIdx].m_HideState.nTime > 0 && NpcSet.GetRelation(m_nIndex, nNpcIdx) == relation_enemy)
		m_nPeapleIdx = 0;
		
	if (nNpcIdx)
	{
		m_nPeapleIdx = nNpcIdx;
	}
	else
	{
		m_nPeapleIdx = 0;
	}
}


//	

void KPlayer::FindSelectObject(int x, int y)
{
	int	nObjIdx = 0;
	
	nObjIdx = ObjSet.SearchObjAt(x, y, 40);
	if (nObjIdx)
		m_nObjectIdx = nObjIdx;
	else
		m_nObjectIdx = 0;
}


//	

int	KPlayer::NetCommandPlayerTalk(BYTE * pProtocol)
{
	/*	DWORD	dwNpcID;
	int		nSize;
	dwNpcID = *(DWORD *) &pProtocol[1];
	nSize = pProtocol[5];
	int nIdx = NpcSet.SearchID(dwNpcID);

	  if (ConformIdx(nIdx))
	  {
			strcpy(Npc[nIdx].m_szChatBuffer, (char *)(pProtocol+6));
			Npc[nIdx].m_nCurChatTime = NPC_SHOW_CHAT_TIME_LENGTH;
			}
			
			  return (7 + nSize);	// need test
		*/
	return 0;
}


//	

BOOL KPlayer::ConformIdx(int nIdx)
{	
	if(nIdx == m_nIndex || nIdx == 0)
		return FALSE;
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL KPlayer::IsWaitingRemove()
{
	if (!m_dwID)
		return FALSE;
	return m_bIsQuiting;
}


//	

void KPlayer::WaitForRemove()
{
	m_bIsQuiting = TRUE;
}


//	

BOOL KPlayer::IsLoginTimeOut()
{
	if (m_nNetConnectIdx != -1)
		return FALSE;
	
	if (!m_dwID)
		return FALSE;
	
	if (-1 != m_dwLoginTime &&
		g_SubWorldSet.GetGameTime() - m_dwLoginTime > defPLAYER_LOGIN_TIMEOUT)
	{
		return TRUE;
	}
	
	return FALSE;
}


void KPlayer::LoginTimeOut()
{
	m_cChat.OffLine(m_dwID);
	
	m_pStatusLoadPlayerInfo = NULL;
	
	Release();
}


//	

BOOL KPlayer::Save()
{
	if (m_nIndex <= 0 && m_dwID == 0)
		return FALSE;
	
	TRoleData* pData = (TRoleData *)m_SaveBuffer;
	
	if (UpdateDBPlayerInfo((BYTE *)pData) == -1)
		return FALSE;

	_ASSERT(pData->dwDataLen < sizeof(m_SaveBuffer));
	if (pData->dwDataLen <= 0)
		return FALSE;

	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	return TRUE;
}


//	

BOOL KPlayer::CanSave()
{
	if (m_nNetConnectIdx == -1)
		return FALSE;
	
	if (m_nIndex <= 0)
		return FALSE;
	
	if (m_bExchangeServer)
		return FALSE;
	
	if (m_bIsQuiting)
		return FALSE;
	
	if (CheckTrading())
	{
		return FALSE;
	}

	
	if (this->m_cShop.GetState())
		return FALSE;
	
	
	if (m_uMustSave == SAVE_DOING && g_SubWorldSet.GetGameTime() - m_ulLastSaveTime > defPLAYER_SAVE_TIMEOUT)
		return TRUE;
	
	if (m_uMustSave != SAVE_IDLE)
		return FALSE;
	
	return TRUE;
}


//	

BOOL KPlayer::SendSyncData(int &nStep, unsigned int &nParam)
{	
	BOOL bRet = FALSE;
	switch(nStep)
	{
	case STEP_BASE_INFO:
		{	
			bRet = SubWorld[Npc[m_nIndex].m_SubWorldIndex].SendSyncData(m_nIndex, m_nNetConnectIdx);
			if (!bRet)
			{
				break;
			}
			bRet = Npc[m_nIndex].SendSyncData(m_nNetConnectIdx);
			if (!bRet)
			{
				break;
			}

			CURPLAYER_SYNC	sSync;	
			sSync.ProtocolType = (BYTE)s2c_synccurplayer;
			sSync.m_dwID = Npc[m_nIndex].m_dwID;
			sSync.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
			sSync.m_btSex = Npc[m_nIndex].m_nSex;
			sSync.m_btKind = Npc[m_nIndex].m_Kind;
			sSync.m_btSeries = Npc[m_nIndex].m_Series;
			sSync.m_wLifeMax = Npc[m_nIndex].m_LifeMax;
			sSync.m_wStaminaMax = Npc[m_nIndex].m_StaminaMax;
			sSync.m_wManaMax = Npc[m_nIndex].m_ManaMax;
			sSync.m_HeadImage = Npc[m_nIndex].m_HeadImage;
			sSync.m_wAttributePoint = this->m_nAttributePoint;
			sSync.m_wSkillPoint = this->m_nSkillPoint;
			sSync.m_wStrength = this->m_nStrength;
			sSync.m_wDexterity = this->m_nDexterity;
			sSync.m_wVitality = this->m_nVitality;
			sSync.m_wEngergy = this->m_nEngergy;
			sSync.m_wLucky = this->m_nLucky;
			sSync.m_nExp = this->m_nExp;
			sSync.m_dwLeadExp = this->m_dwLeadExp;
			sSync.m_btCurFaction = this->m_cFaction.m_nCurFaction;
			sSync.m_btFirstFaction = this->m_cFaction.m_nFirstAddFaction;
			sSync.m_nFactionAddTimes = this->m_cFaction.m_nAddTimes;
			sSync.m_wWorldStat = (WORD)m_nWorldStat;
			sSync.m_wSectStat = (WORD)m_nSectStat;
			sSync.m_nMoney1 = this->m_ItemList.GetMoney(room_equipment);
			sSync.m_nMoney2 = this->m_ItemList.GetMoney(room_repository);
			//
			sSync.m_nBronzeCoin = this->GetBronzeCoin();
			sSync.m_nLockBronzeCoin = this->GetLockBronzeCoin();
			sSync.m_btRepositoryCount = this->m_btRepositoryCount;
			sSync.m_nLockPlayer = this->m_nLockPlayer;//LockPlayer by kinnox;
			sSync.m_btExpandtoryCount = this->m_btExpandtoryCount;//Expandbox by kinnox;	
			sSync.m_btImagePlayer = this->m_ImagePlayer; //Player Avatar by kinnox;		
			//
			if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_SYNC))))
			{
				bRet = TRUE;
			}
			else
			{
				printf("player Packing sync data failed...\n");
				bRet = FALSE;
				break;
			}
		}
		break;
	case STEP_FIGHT_SKILL_LIST:	
		{
			bRet = SendSyncData_Skill();
		}
		break;
	case STEP_LIFE_SKILL_LIST:
		bRet = TRUE;
		break;
	case STEP_ITEM_LIST:
		bRet = TRUE;
		break;
	case STEP_TASK_LIST:
		bRet = TRUE;
		break;
	case STEP_SYNC_END:
		bRet = TRUE;
		break;
	}
	if (!bRet)
	{
		printf("Load PlayerInfo Step %d Failed...\n", nStep);
	}
	return bRet;
}


//	

BOOL KPlayer::SendSyncData_Skill()
{
	SKILL_SEND_ALL_SYNC		sSkill;
	
	sSkill.ProtocolType = s2c_synccurplayerskill;
	sSkill.m_wProtocolLong = 2;
	int i;
	int j;
	for (i = 0,j = 0; i < MAX_NPCSKILL; i++)
	{
		sSkill.m_sAllSkill[i].SkillId = 0;
		sSkill.m_sAllSkill[i].SkillLevel = 0;
		if( Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId <= 0)
			continue;
		sSkill.m_sAllSkill[j].SkillId = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId;
		sSkill.m_sAllSkill[j].SkillLevel = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillLevel;
		j++;
	}
	sSkill.m_wProtocolLong += sizeof(SKILL_SEND_ALL_SYNC_DATA) * j;
	
	if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sSkill.m_wProtocolLong + 1)))
	{
		return TRUE;
	}
	else
	{
		printf("player Packing skill sync data failed...\n");
		return FALSE;
	}
}


//	

void	KPlayer::SendCurNormalSyncData()
{
	CURPLAYER_NORMAL_SYNC	sSync;
	
	sSync.ProtocolType	= s2c_synccurplayernormal;
	sSync.m_ushLife		= (USHORT)Npc[m_nIndex].m_CurrentLife;
	sSync.m_ushStamina	= (USHORT)Npc[m_nIndex].m_CurrentStamina;
	sSync.m_ushMana		= (USHORT)Npc[m_nIndex].m_CurrentMana;
	if ( !m_cTeam.m_nFlag )
	{
		sSync.m_btTeamData = 0;
	}
	else
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
			sSync.m_btTeamData = 0x03;
		else
			sSync.m_btTeamData = 0x01;
	}	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_NORMAL_SYNC));
}


//	

void KPlayer::QuitGame(int nType)
{
	Save();
}


//	

void KPlayer::BuyItem(BYTE* pProtocol)
{
	PLAYER_BUY_ITEM_COMMAND* pCommand = (PLAYER_BUY_ITEM_COMMAND *)pProtocol;
	BuySell.Buy(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, pCommand->m_BuyIdx, pCommand->m_Place, pCommand->m_X, pCommand->m_Y);
}


//	

void KPlayer::SellItem(BYTE* pProtocol)
{
	PLAYER_SELL_ITEM_COMMAND* pCommand = (PLAYER_SELL_ITEM_COMMAND *)pProtocol;
	BuySell.Sell(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, m_ItemList.SearchID(pCommand->m_ID));
}

#endif

#ifdef _SERVER

//	

void KPlayer::GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sLoginRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sLoginRevivalPos.m_nMpsX;
	*lpnMpsY = m_sLoginRevivalPos.m_nMpsY;
}


//	

void KPlayer::GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sDeathRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sDeathRevivalPos.m_nMpsX;
	*lpnMpsY = m_sDeathRevivalPos.m_nMpsY;
}
#endif

#ifdef _SERVER

void	KPlayer::SetRevivalPos(int nSubWorld, int nReviveId)
{
	if (nSubWorld >= 0) 
	{
		m_sLoginRevivalPos.m_nSubWorldID = nSubWorld;
	}
	else
	{
		m_sLoginRevivalPos.m_nSubWorldID = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	}
	
	POINT Pos;
	
	g_SubWorldSet.GetRevivalPosFromId(m_sLoginRevivalPos.m_nSubWorldID, nReviveId, &Pos);
	m_sLoginRevivalPos.m_ReviveID = nReviveId;
	m_sLoginRevivalPos.m_nMpsX = Pos.x;
	m_sLoginRevivalPos.m_nMpsY = Pos.y;
	
	m_sDeathRevivalPos = m_sLoginRevivalPos;
}
#endif

#ifdef _SERVER

void	KPlayer::S2CSendTeamInfo(BYTE* pProtocol)
{
	PLAYER_APPLY_TEAM_INFO	*pApplyTeamInfo = (PLAYER_APPLY_TEAM_INFO*)pProtocol;
	
	if (pApplyTeamInfo->m_dwTarNpcID == Npc[m_nIndex].m_dwID)
	{
		SendSelfTeamInfo();
		return;
	}
	
	PLAYER_SEND_TEAM_INFO	sTeamInfo;
	PLAYER_APPLY_TEAM_INFO_FALSE	sTeamInfoFalse;

	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		goto SendFalse;
	
SendMsg:
	KTeam	*pTeam;
	if ( !Player[nPlayer].m_cTeam.m_nFlag )
		goto SendFalse;
	pTeam = &g_Team[Player[nPlayer].m_cTeam.m_nID];
	if (pTeam->m_nCaptain < 0 || !pTeam->IsOpen())
		goto SendFalse;
	
	sTeamInfo.ProtocolType = (BYTE)s2c_teaminfo;
	sTeamInfo.m_nCaptain = Npc[Player[pTeam->m_nCaptain].m_nIndex].m_dwID;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pTeam->m_nMember[i] < 0)
			sTeamInfo.m_nMember[i] = 0;
		else
			sTeamInfo.m_nMember[i] = Npc[Player[pTeam->m_nMember[i]].m_nIndex].m_dwID;
	}

	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfo, sizeof(PLAYER_SEND_TEAM_INFO));
	return;
	
SendFalse:
	sTeamInfoFalse.ProtocolType = s2c_teamapplyinfofalse;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfoFalse, sizeof(PLAYER_APPLY_TEAM_INFO_FALSE));
	return;
}
#endif

#ifdef _SERVER

//	

void	KPlayer::SendSelfTeamInfo()
{
	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
	{
		int		nNpcIndex;
		PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
		sSelfInfo.ProtocolType = s2c_teamselfinfo;
		sSelfInfo.nTeamServerID = m_cTeam.m_nID;
		sSelfInfo.m_dwLeadExp = m_dwLeadExp;
		sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;
		
		nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
		sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
		sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
		sSelfInfo.m_TP[0] = SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_SubWorldID; // S? d?ng m_TP cho SubWorldID
		sSelfInfo.m_HP[0] = Npc[nNpcIndex].m_CurrentLife; // Lay HP th�nh vi�n
		sSelfInfo.m_MHP[0] = Npc[nNpcIndex].m_CurrentLifeMax;
		sSelfInfo.m_SM[0] = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_cFaction.m_nCurFaction;
		strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);

		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
			{
				nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
				sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
				sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
				sSelfInfo.m_TP[i + 1] = SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_SubWorldID;
				sSelfInfo.m_HP[i + 1] = Npc[nNpcIndex].m_CurrentLife; // Lay HP th�nh vi�n
				sSelfInfo.m_MHP[i + 1] = Npc[nNpcIndex].m_CurrentLifeMax; // Lay HP th�nh vi�n
				sSelfInfo.m_SM[i + 1] = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_cFaction.m_nCurFaction;
				strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
			}
			else
			{
				sSelfInfo.m_dwNpcID[i + 1] = 0;
				sSelfInfo.m_btLevel[i + 1] = 0;
				sSelfInfo.m_TP[i + 1] = 0;
				sSelfInfo.m_HP[i + 1] = 0; //thay sau
				sSelfInfo.m_MHP[i + 1] = 0; //thay sau
				sSelfInfo.m_SM[i + 1] = 0; //thay sau
			}
		}
	
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	}
	else
	{
		PLAYER_LEAVE_TEAM	sLeaveTeam;
		sLeaveTeam.ProtocolType = s2c_teamleave;
		sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
	}
}
#endif

#ifdef _SERVER
//	
BOOL	KPlayer::CreateTeam(BYTE* pProtocol)
{
	return m_cTeam.CreateTeam(this->m_nPlayerIndex, (PLAYER_APPLY_CREATE_TEAM*)pProtocol);
}
#endif

#ifdef _SERVER
//	
BOOL	KPlayer::SetTeamState(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return FALSE;
	
	if (this->m_cShop.GetState())
		return FALSE;
	
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN || m_cTeam.m_nID < 0)
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pProtocol;
	if (pTeamState->m_btOpenClose == 0)
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	else
	{
		if (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN)
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
	
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::S2CSendAddTeamInfo(BYTE* pProtocol)
{
	if (!m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pAddTeam->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pAddTeam->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		return FALSE;
	
SendMsg:
	if (Npc[Player[nPlayer].m_nIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return FALSE;
	m_cTeam.m_nApplyCaptainID = pAddTeam->m_dwTarNpcID;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = s2c_teamgetapply;
	sAddTeam.m_dwTarNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	return TRUE;
}
#endif

#ifdef _SERVER

//	
/*
BOOL	KPlayer::AddTeamMember(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag ||
		m_cTeam.m_nFigure != TEAM_CAPTAIN ||
		!g_Team[m_cTeam.m_nID].IsOpen() ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= MAX_TEAM_MEMBER ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= g_Team[m_cTeam.m_nID].CalcCaptainPower())
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	int		nPlayer, i;
	PLAYER_ACCEPT_TEAM_MEMBER	*pAccept = (PLAYER_ACCEPT_TEAM_MEMBER*)pProtocol;
	nPlayer = FindAroundPlayer(pAccept->m_dwNpcID);
	if (nPlayer < 0)
		return FALSE;

	if (!Player[nPlayer].m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TARGET_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	if (Player[nPlayer].m_cTeam.m_nFlag)
		return FALSE;
	if (Player[nPlayer].m_cTeam.m_nApplyCaptainID != Npc[this->m_nIndex].m_dwID)
		return FALSE;
	
	if ( !g_Team[m_cTeam.m_nID].AddMember(nPlayer) )
		return FALSE;
	Player[nPlayer].m_cTeam.Release();
	Player[nPlayer].m_cTeam.m_nFlag = 1;
	Player[nPlayer].m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[nPlayer].m_cTeam.m_nID = m_cTeam.m_nID;
	Npc[Player[nPlayer].m_nIndex].SetCurrentCamp(Npc[m_nIndex].m_Camp);
	
	PLAYER_TEAM_ADD_MEMBER	sAddMem;
	sAddMem.ProtocolType = s2c_teamaddmember;
	sAddMem.m_dwNpcID = Npc[Player[nPlayer].m_nIndex].m_dwID;
	sAddMem.m_btLevel = (DWORD)Npc[Player[nPlayer].m_nIndex].m_Level;
	sAddMem.m_TP = SubWorld[Npc[Player[nPlayer].m_nIndex].m_SubWorldIndex].m_SubWorldID; // Th�m SubWorldID
	sAddMem.m_SM = Player[nPlayer].m_cFaction.m_nCurFaction;
	sAddMem.m_HP = Npc[Player[nPlayer].m_nIndex].m_CurrentLife;
	sAddMem.m_MHP = Npc[Player[nPlayer].m_nIndex].m_CurrentLifeMax;
	strcpy(sAddMem.m_szName, Npc[Player[nPlayer].m_nIndex].Name);
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0 || g_Team[m_cTeam.m_nID].m_nMember[i] == nPlayer)
			continue;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	}
	
	int		nNpcIndex;
	PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
	sSelfInfo.ProtocolType = s2c_teamselfinfo;
	sSelfInfo.nTeamServerID = m_cTeam.m_nID;
	sSelfInfo.m_dwLeadExp = Player[nPlayer].m_dwLeadExp;
	sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;

	nNpcIndex = m_nIndex;
	sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
	sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
	sSelfInfo.m_TP[0] = SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_SubWorldID;
	sSelfInfo.m_SM[0] = Player[nNpcIndex].m_cFaction.m_nCurFaction;
	sSelfInfo.m_HP[0] = Npc[nNpcIndex].m_CurrentLife;
	sSelfInfo.m_MHP[0] = Npc[nNpcIndex].m_CurrentLifeMax;
	strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);

	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
		{
			nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
			sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
			sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
			sSelfInfo.m_TP[i + 1] = SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_SubWorldID;
			sSelfInfo.m_SM[i + 1] = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_cFaction.m_nCurFaction;
			sSelfInfo.m_HP[i + 1] = Npc[nNpcIndex].m_CurrentLife;
			sSelfInfo.m_MHP[i + 1] = Npc[nNpcIndex].m_CurrentLifeMax;
			strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
		}
		else
		{
			sSelfInfo.m_dwNpcID[i + 1] = 0;
			sSelfInfo.m_btLevel[i + 1] = 0;
			sSelfInfo.m_TP[i + 1] = 0;
			sSelfInfo.m_HP[i + 1] = 0;
			sSelfInfo.m_HP[i + 1] = 0;
			sSelfInfo.m_MHP[i + 1] = 0;
		}
	}

	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_SELF_ADD;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
	sMsg.m_lpBuf = 0;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	return TRUE;
}*/
//Kenny fix Party 
BOOL KPlayer::AddTeamMember(BYTE* pProtocol)
{
    // captain-side checks
    if ( !m_cTeam.m_nFlag ||
         m_cTeam.m_nFigure != TEAM_CAPTAIN ||
         !g_Team[m_cTeam.m_nID].IsOpen() ||
         g_Team[m_cTeam.m_nID].m_nMemNum >= MAX_TEAM_MEMBER ||
         g_Team[m_cTeam.m_nID].m_nMemNum >= g_Team[m_cTeam.m_nID].CalcCaptainPower() )
    {
        SendSelfTeamInfo();
        return FALSE;
    }

    int i;
    PLAYER_ACCEPT_TEAM_MEMBER* pAccept = (PLAYER_ACCEPT_TEAM_MEMBER*)pProtocol;

    // find the joining player by NPC id around me
    int nPlayer = FindAroundPlayer(pAccept->m_dwNpcID);
    if (nPlayer < 0)
        return FALSE;

    // joining player guards
    if (!Player[nPlayer].m_cTeam.m_bCanTeamFlag)
    {
        SHOW_MSG_SYNC sMsg;
        sMsg.ProtocolType = s2c_msgshow;
        sMsg.m_wMsgID     = enumMSG_ID_TARGET_CANNOT_ADD_TEAM;
        sMsg.m_wLength    = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
        g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
        return FALSE;
    }
    if (Player[nPlayer].m_cTeam.m_nFlag) // already in a team
        return FALSE;

    // must have applied to THIS captain (compare to my NPC id)
    if (Player[nPlayer].m_cTeam.m_nApplyCaptainID != Npc[this->m_nIndex].m_dwID)
        return FALSE;

    // add to team
    if (!g_Team[m_cTeam.m_nID].AddMember(nPlayer))
        return FALSE;

    // init team state for the joining player
    Player[nPlayer].m_cTeam.Release();
    Player[nPlayer].m_cTeam.m_nFlag   = 1;
    Player[nPlayer].m_cTeam.m_nFigure = TEAM_MEMBER;
    Player[nPlayer].m_cTeam.m_nID     = m_cTeam.m_nID;

    // align camp
    Npc[Player[nPlayer].m_nIndex].SetCurrentCamp(Npc[m_nIndex].m_Camp);

    // notify captain (me) about the new member
    {
        PLAYER_TEAM_ADD_MEMBER sAddMem;
        sAddMem.ProtocolType = s2c_teamaddmember;
        sAddMem.m_dwNpcID    = Npc[Player[nPlayer].m_nIndex].m_dwID;
        sAddMem.m_btLevel    = (DWORD)Npc[Player[nPlayer].m_nIndex].m_Level;
        sAddMem.m_TP         = SubWorld[Npc[Player[nPlayer].m_nIndex].m_SubWorldIndex].m_SubWorldID; // SubWorldID
        sAddMem.m_SM         = Player[nPlayer].m_cFaction.m_nCurFaction;
        sAddMem.m_HP         = Npc[Player[nPlayer].m_nIndex].m_CurrentLife;
        sAddMem.m_MHP        = Npc[Player[nPlayer].m_nIndex].m_CurrentLifeMax;
        strcpy(sAddMem.m_szName, Npc[Player[nPlayer].m_nIndex].Name);

        // to captain (me)
        g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));

        // to other existing members
        for (i = 0; i < MAX_TEAM_MEMBER; i++)
        {
            int nMem = g_Team[m_cTeam.m_nID].m_nMember[i];
            if (nMem < 0 || nMem == nPlayer)
                continue;
            g_pServer->PackDataToClient(Player[nMem].m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
        }
    }

    // send full self team info to the JOINING player
    {
        PLAYER_SEND_SELF_TEAM_INFO sSelfInfo;
        memset(&sSelfInfo, 0, sizeof(sSelfInfo));

        sSelfInfo.ProtocolType  = s2c_teamselfinfo;
        sSelfInfo.nTeamServerID = m_cTeam.m_nID;

        // captain index (PLAYER index), NOT NPC index
        int nCaptainPlayerIdx = g_Team[m_cTeam.m_nID].m_nCaptain;
        if (nCaptainPlayerIdx <= 0)
            nCaptainPlayerIdx = nPlayer; // fallback, should not happen

        int nCaptainNpcIdx = Player[nCaptainPlayerIdx].m_nIndex;

        // lead exp should be of captain (consistent with SendSelfTeamInfo)
        sSelfInfo.m_dwLeadExp = Player[nCaptainPlayerIdx].m_dwLeadExp;

        sSelfInfo.m_btState   = g_Team[m_cTeam.m_nID].m_nState;

        // slot 0: captain
        sSelfInfo.m_dwNpcID[0]  = Npc[nCaptainNpcIdx].m_dwID;
        sSelfInfo.m_btLevel[0]  = (DWORD)Npc[nCaptainNpcIdx].m_Level;
        sSelfInfo.m_TP[0]       = SubWorld[Npc[nCaptainNpcIdx].m_SubWorldIndex].m_SubWorldID;
        sSelfInfo.m_SM[0]       = Player[nCaptainPlayerIdx].m_cFaction.m_nCurFaction; // *** FIXED ***
        sSelfInfo.m_HP[0]       = Npc[nCaptainNpcIdx].m_CurrentLife;
        sSelfInfo.m_MHP[0]      = Npc[nCaptainNpcIdx].m_CurrentLifeMax;
        strcpy(sSelfInfo.m_szNpcName[0], Npc[nCaptainNpcIdx].Name);

        // slots 1..MAX_TEAM_MEMBER: members
        for (i = 0; i < MAX_TEAM_MEMBER; i++)
        {
            int nMemPlayerIdx = g_Team[m_cTeam.m_nID].m_nMember[i];
            int slot = i + 1;

            if (nMemPlayerIdx >= 0)
            {
                int nMemNpcIdx = Player[nMemPlayerIdx].m_nIndex;

                sSelfInfo.m_dwNpcID[    slot] = Npc[nMemNpcIdx].m_dwID;
                sSelfInfo.m_btLevel[    slot] = (DWORD)Npc[nMemNpcIdx].m_Level;
                sSelfInfo.m_TP[         slot] = SubWorld[Npc[nMemNpcIdx].m_SubWorldIndex].m_SubWorldID;
                sSelfInfo.m_SM[         slot] = Player[nMemPlayerIdx].m_cFaction.m_nCurFaction;
                sSelfInfo.m_HP[         slot] = Npc[nMemNpcIdx].m_CurrentLife;
                sSelfInfo.m_MHP[        slot] = Npc[nMemNpcIdx].m_CurrentLifeMax;
                strcpy(sSelfInfo.m_szNpcName[slot], Npc[nMemNpcIdx].Name);
            }
            else
            {
                sSelfInfo.m_dwNpcID[slot] = 0;
                sSelfInfo.m_btLevel[slot] = 0;
                sSelfInfo.m_TP[slot]      = 0;
                sSelfInfo.m_SM[slot]      = 0;
                sSelfInfo.m_HP[slot]      = 0;
                sSelfInfo.m_MHP[slot]     = 0;
                sSelfInfo.m_szNpcName[slot][0] = '\0';
            }
        }

        // send to the new member only
        g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
    }

    // notify the new member
    {
        SHOW_MSG_SYNC sMsg;
        sMsg.ProtocolType = s2c_msgshow;
        sMsg.m_wMsgID     = enumMSG_ID_TEAM_SELF_ADD;
        sMsg.m_wLength    = sizeof(SHOW_MSG_SYNC) - 1;
        sMsg.m_lpBuf      = 0;
        g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
    }

    return TRUE;
}

#endif

#ifdef _SERVER

//	

void	KPlayer::LeaveTeam(BYTE* pProtocol)
{
	if (!m_cTeam.m_nFlag)
		return;

	if (this->m_cTeam.m_nFigure == TEAM_CAPTAIN && g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_LEAVE;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (void*)Npc[this->m_nIndex].m_dwID;
		
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			{
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			}
		}
		sMsg.m_lpBuf = 0;
	}
	
	g_Team[m_cTeam.m_nID].DeleteMember(GetPlayerIndex());
}
#endif

#ifdef _SERVER

//
void KPlayer::TeamKickOne(BYTE* pProtocol)
{
    if (!m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
        return;

    if (m_cTeam.m_nID < 0)
        return;

    PLAYER_TEAM_KICK_MEMBER* pKickOne = (PLAYER_TEAM_KICK_MEMBER*)pProtocol;
    int nMemNo = g_Team[m_cTeam.m_nID].FindMemberID(pKickOne->m_dwNpcID);
    if (nMemNo < 0)
        return;

    int nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[nMemNo];
    if (nPlayerNo <= 0)
        return;

    g_Team[m_cTeam.m_nID].DeleteMember(nPlayerNo);

    SHOW_MSG_SYNC sMsg;

    const char* szName = Npc[Player[nPlayerNo].m_nIndex].Name;
    int nLength = (int)strlen(szName);
    const int NAME_MAX = 31; 
    if (nLength > NAME_MAX) nLength = NAME_MAX;

    sMsg.ProtocolType = s2c_msgshow;
    sMsg.m_wMsgID     = enumMSG_ID_TEAM_KICK_One;
    sMsg.m_wLength    = (WORD)(sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength);
    sMsg.m_lpBuf      = new BYTE[sMsg.m_wLength + 1];
    memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
    memcpy((char*)sMsg.m_lpBuf + (sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID)), szName, nLength);

    ((char*)sMsg.m_lpBuf)[sMsg.m_wLength] = '\0';

    g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
    if (g_Team[m_cTeam.m_nID].m_nCaptain > 0)
        g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);

    for (int i = 0; i < MAX_TEAM_MEMBER; ++i)
    {
        int memIdx = g_Team[m_cTeam.m_nID].m_nMember[i];
        if (memIdx > 0)
        {
            g_pServer->PackDataToClient(
                Player[memIdx].m_nNetConnectIdx,
                sMsg.m_lpBuf, sMsg.m_wLength + 1
            );
        }
    }

    sMsg.Release();
}
//End fix team	
/*
void	KPlayer::TeamKickOne(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	int		nMemNo;
	PLAYER_TEAM_KICK_MEMBER	*pKickOne = (PLAYER_TEAM_KICK_MEMBER*)pProtocol;
	nMemNo = g_Team[m_cTeam.m_nID].FindMemberID(pKickOne->m_dwNpcID);
	if (nMemNo < 0)
		return;
	int		nPlayerNo;
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[nMemNo];
	g_Team[m_cTeam.m_nID].DeleteMember(nPlayerNo);
	
	int		nLength;
	SHOW_MSG_SYNC	sMsg;
	
	nLength = strlen(Npc[Player[nPlayerNo].m_nIndex].Name);
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_KICK_One;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength;
	sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
	
	memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
	memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), Npc[Player[nPlayerNo].m_nIndex].Name, nLength);
	
	g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
	sMsg.Release();
}*/ 
	
#endif

#ifdef _SERVER

//	

void	KPlayer::TeamChangeCaptain(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	int		i, nMember, nPlayerNo, nIsOpen = 0;
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_APPLY_TEAM_CHANGE_CAPTAIN*)pProtocol;
	nMember = g_Team[m_cTeam.m_nID].FindMemberID(pChange->m_dwNpcID);
	if (nMember < 0)
		return;
	if (Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_nIndex].m_Camp == camp_begin && Npc[this->m_nIndex].m_Camp != camp_begin)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].m_nMemNum > PlayerSet.m_cLeadExp.GetMemNumFromLevel(Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_dwLeadLevel))
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
		nIsOpen = 1;
	}
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nCaptain;
	g_Team[m_cTeam.m_nID].m_nCaptain = g_Team[m_cTeam.m_nID].m_nMember[nMember];
	g_Team[m_cTeam.m_nID].m_nMember[nMember] = nPlayerNo;
	m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	
	Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].RestoreCurrentCamp();
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		Npc[Player[nPlayerNo].m_nIndex].SetCurrentCamp(Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Camp);
	}
	
	PLAYER_TEAM_CHANGE_CAPTAIN	sChangeCaptain;
	sChangeCaptain.ProtocolType = s2c_teamchangecaptain;
	sChangeCaptain.m_dwCaptainID = pChange->m_dwNpcID;
	sChangeCaptain.m_dwMemberID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	}
	
	if (nIsOpen)
	{
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::TeamDismiss(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_LEAVE_TEAM	sLeaveTeam;
	int		i;
	SHOW_MSG_SYNC	sMsg;
	
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_DISMISS;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	sLeaveTeam.ProtocolType = s2c_teamleave;
	sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));

	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		
		sLeaveTeam.m_dwNpcID = Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
		Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_cTeam.m_nFlag = 0;
		Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].RestoreCurrentCamp();;
	}
	g_Team[m_cTeam.m_nID].Release();
	m_cTeam.m_nFlag = 0;
	Npc[m_nIndex].RestoreCurrentCamp();
}
#endif

#ifdef _SERVER

//	

void	KPlayer::SetPK(BYTE* pProtocol)
{
	PLAYER_SET_PK	*pPK = (PLAYER_SET_PK*)pProtocol;
	
	if (pPK->m_bPKFlag)
	{
		Npc[m_nIndex].SetCurrentCamp(camp_free);
	}
	else
	{
		Npc[m_nIndex].RestoreCurrentCamp();
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::SendFactionData(BYTE* pProtocol)
{
	SendFactionData();
}
#endif

#ifdef _SERVER

//	

void	KPlayer::SendFactionData()
{
	PLAYER_FACTION_DATA	sData;
	sData.ProtocolType = s2c_playerfactiondata;
	sData.m_btCamp = Npc[m_nIndex].m_Camp;
	sData.m_btCurFaction = m_cFaction.m_nCurFaction;
	sData.m_btFirstFaction = m_cFaction.m_nFirstAddFaction;
	sData.m_nAddTimes = m_cFaction.m_nAddTimes;
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sData, sizeof(PLAYER_FACTION_DATA));
}
#endif

#ifdef _SERVER

//	

void KPlayer::GetAboutPos(KMapPos *pMapPos)
{
	if (m_nIndex <= 0)
		return;
	
	if (Npc[m_nIndex].m_SubWorldIndex < 0)
		return;
	
	POINT Pos;
	int nX, nY;
	Npc[m_nIndex].GetMpsPos(&nX, &nY);
	Pos.x = nX;
	Pos.y = nY;
	
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetFreeObjPos(Pos);
	
	pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(
		Pos.x, 
		Pos.y, 
		&pMapPos->nRegion, 
		&pMapPos->nMapX, 
		&pMapPos->nMapY, 
		&pMapPos->nOffX, 
		&pMapPos->nOffY);
}

/*
void	KPlayer::GetAboutPos(KMapPos *pMapPos)
{
POINT	Pos[8] = 
{
{0, 32}, {-32, 32}, {-32, 0}, {-32, -32},
{0, -32}, {32, -32}, {32, 0}, {32, 32},
};

  int nMpsX, nMpsY, nTmpX, nTmpY;
  int nR, nMapX, nMapY, nOffX, nOffY;
  Npc[m_nIndex].GetMpsPos(&nMpsX, &nMpsY);
  
	for (int i = 0; i < 8; i++)
	{
	nTmpX = nMpsX + Pos[i].x;
	nTmpY = nMpsY + Pos[i].y;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetBarrier(nTmpX, nTmpY))
	continue;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(nTmpX, nTmpY, &nR, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nR == -1)
	continue;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nR].GetRef(nMapX, nMapY, obj_object))
	continue;
	else
	break;
	}
	
	  if (i == 8)
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = Npc[m_nIndex].m_RegionIndex;
	  pMapPos->nMapX = Npc[m_nIndex].m_MapX;
	  pMapPos->nMapY = Npc[m_nIndex].m_MapY;
	  pMapPos->nOffX = Npc[m_nIndex].m_OffX;
	  pMapPos->nOffY = Npc[m_nIndex].m_OffY;
	  }
	  else
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = nR;
	  pMapPos->nMapX = nMapX;
	  pMapPos->nMapY = nMapY;
	  pMapPos->nOffX = nOffX;
	  pMapPos->nOffY = nOffY;
	  }
}*/
#endif


#ifdef _SERVER

//

int		KPlayer::FindAroundPlayer(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return -1;
	
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(dwNpcID);
	if ( nPlayer >= 0)
		return nPlayer;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(dwNpcID);
		if (nPlayer >= 0)
			return nPlayer;
	}
	
	return -1;
}


//	

BOOL	KPlayer::CheckPlayerAround(int nPlayerIdx)
{
	if (nPlayerIdx <= 0)
		return FALSE;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].CheckPlayerIn(nPlayerIdx))
		return TRUE;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].CheckPlayerIn(nPlayerIdx))
			return TRUE;
	}
	
	return FALSE;
}


//

int		KPlayer::FindAroundNpc(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return 0;
	
	int		nNpc, nRegionNo, i;
	nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].SearchNpc(dwNpcID);
	if ( nNpc > 0)
		return nNpc;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].SearchNpc(dwNpcID);
		if (nNpc > 0)
			return nNpc;
	}
	
	return 0;
}
#endif


//

void	KPlayer::ResetBaseAttribute(char cSeries)
{
	if (cSeries < series_metal || cSeries > series_earth)
		return;

	m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[cSeries];
	m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[cSeries];
	m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[cSeries];
	m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[cSeries];
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;

	Npc[m_nIndex].m_LifeMax =((Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series)) +
		(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
	
	Npc[m_nIndex].m_ManaMax =(Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series) + 
			(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * m_nCurEngergy));
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
	
	Npc[m_nIndex].m_StaminaMax =(Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_nSex) + 
		(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;

	m_nAttributePoint = (Npc[m_nIndex].m_Level - 1) * 5;
	UpdataCurData();
#ifdef _SERVER
	PLAYER_ATTRIBUTE_SYNC	sSync;
	sSync.ProtocolType = s2c_playersyncattribute;
	sSync.m_btAttribute = ATTRIBUTE_RESET_ALL;
	sSync.m_nBasePoint = cSeries;
	sSync.m_nCurPoint = 0;
	sSync.m_nLeavePoint = m_nAttributePoint;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	RefeshPlayer(TRUE);
#endif
}


//

//void	KPlayer::CalcCurStrength()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurStrength = m_nStrength;
//}


//	

//void	KPlayer::CalcCurDexterity()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurDexterity = m_nDexterity;
//}


//	

//void	KPlayer::CalcCurVitality()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurVitality = m_nVitality;
//}


//	

//void	KPlayer::CalcCurEngergy()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurEngergy = m_nEngergy;
//}


//	

void	KPlayer::CalcCurLucky()
{
	m_nCurLucky = m_nLucky;
}

#ifdef _SERVER

//	

void	KPlayer::AddBaseStrength(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nStrength += nData;
		m_nAttributePoint -= nData;
		m_nCurStrength += nData;
		
		UpdataCurData();
		SetNpcPhysicsDamage();

		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 0;
		sSync.m_nBasePoint = m_nStrength;
		sSync.m_nCurPoint = m_nCurStrength;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::AddBaseDexterity(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nDexterity += nData;
		m_nAttributePoint -= nData;
		m_nCurDexterity += nData;
		
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 1;
		sSync.m_nBasePoint = m_nDexterity;
		sSync.m_nCurPoint = m_nCurDexterity;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::AddBaseVitality(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nVitality += nData;
		m_nAttributePoint -= nData;
		m_nCurVitality += nData;
		
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 2;
		sSync.m_nBasePoint = m_nVitality;
		sSync.m_nCurPoint = m_nCurVitality;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::AddBaseEngergy(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nEngergy += nData;
		m_nAttributePoint -= nData;
		m_nCurEngergy += nData;
		
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 3;
		sSync.m_nBasePoint = m_nEngergy;
		sSync.m_nCurPoint = m_nCurEngergy;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif


//

void	KPlayer::ChangeCurStrength(int nData)
{
	m_nCurStrength += nData;
	SetNpcPhysicsDamage();
}


//

void	KPlayer::ChangeCurDexterity(int nData)
{
	m_nCurDexterity += nData;
	
	int nRating = nData * 4;
	Npc[m_nIndex].m_CurrentAttackRating += nRating;
	
	int nDefence = nData / 4;
	Npc[m_nIndex].m_CurrentDefend += nDefence;
	
	SetNpcPhysicsDamage();
	
}



//

void	KPlayer::ChangeCurVitality(int nData)
{

	m_nCurVitality += nData;
	
	Npc[m_nIndex].AddCurLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
	Npc[m_nIndex].AddCurStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
	
}


//

void	KPlayer::ChangeCurEngergy(int nData)
{
	m_nCurEngergy += nData;
	
	Npc[m_nIndex].AddCurManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
	
}


//	

void	KPlayer::AddBaseLucky(int nData)
{
	m_nLucky += nData;
	CalcCurLucky();
}


//

void	KPlayer::SetNpcPhysicsDamage()
{
	int		nMinDamage, nMaxDamage;
	m_ItemList.GetWeaponDamage(&nMinDamage, &nMaxDamage);
	
	if (m_ItemList.GetWeaponType() == equip_meleeweapon)
	{
		nMinDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else if (m_ItemList.GetWeaponType() == equip_rangeweapon)
	{
		nMinDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else
	{
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
}


//

void	KPlayer::SetNpcAttackRating()
{
	int		nRating;
	
	nRating = m_nDexterity * 4 - 28;
	Npc[m_nIndex].SetBaseAttackRating(nRating);
}


//	

void	KPlayer::SetNpcDefence()
{
	int		nDefence;
	
	nDefence = m_nDexterity / 4;
	Npc[m_nIndex].SetBaseDefence(nDefence);
}

/*
#ifdef _SERVER

//	

void	KPlayer::SetNpcWalkSpeed()
{
int		nSpeed;
nSpeed = BASE_WALK_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseWalkSpeed(nSpeed);
}
#endif
*/

/*
#ifdef _SERVER

//	

void	KPlayer::SetNpcRunSpeed()
{
int		nSpeed;
nSpeed = BASE_RUN_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseRunSpeed(nSpeed);
}
#endif
*/


//

void	KPlayer::AddExp(float nExp, int nTarLevel,BOOL bCheck)//chia exp team by kinnox;
{
#ifdef _SERVER
	CONST KNPC_EXP_WORLD*		pNpcExp;
	pNpcExp = SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetNpcExp(); 
	if(pNpcExp->byMinL > 0)
	{
		if(Npc[m_nIndex].m_Level <= pNpcExp->byMaxL)
		{ 
			nExp *= pNpcExp->nExp*3; //nhan doi kinh nghiem o day by kinnox; hien tai dang x3; -- chi can build o server;			
		}
		else 
		{
			nExp *= (pNpcExp->nExp/5);
		} 
	}
#endif
	
	
#ifndef _SERVER
	AddSelfExp(nExp, nTarLevel);
#endif
	
#ifdef _SERVER
	if (!m_cTeam.m_nFlag)
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}
	
	int		i, j, nShareFlag[MAX_TEAM_MEMBER + 1], nRegion, nTotalLevel = 0, nTotalPlayer = 0;
	memset(nShareFlag, 0, sizeof(nShareFlag));
	int		nMpsX, nMpsY, nNpcIdx, nSelfSubIdx, nSelfMpsX, nSelfMpsY;
	
	nSelfSubIdx = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[nSelfSubIdx].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nSelfMpsX,
		&nSelfMpsY);
	
	nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
	if (nSelfSubIdx == Npc[nNpcIdx].m_SubWorldIndex)
	{
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) <= 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
		{
			nTotalLevel += Npc[nNpcIdx].m_Level;
			nShareFlag[0] = 1;
			nTotalPlayer++;
			if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
				goto SharePlace;
		}
	}

	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0)
			continue;
		nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
		if (nSelfSubIdx != Npc[nNpcIdx].m_SubWorldIndex)
			continue;
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) > 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
			continue;
		nTotalLevel += Npc[nNpcIdx].m_Level;
		nShareFlag[i + 1] = 1;
		nTotalPlayer++;
		if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
			goto SharePlace;
	}
	
SharePlace:
	_ASSERT(nTotalPlayer >= 1);
	_ASSERT(nTotalLevel >= 1);
	if (nTotalPlayer < 1 || nTotalLevel < 1)
		return;
	if (nTotalPlayer == 1)
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}
	// fix exp team by kinnox;
	int		k, nGetExp;
	k = 100 + (nTotalPlayer-1) * 15;
	
	int ShareExp;
	nTotalPlayer = g_Team[m_cTeam.m_nID].m_nMemNum + 1;
	if (nTotalPlayer == 2)
		ShareExp = 80;
	else if (nTotalPlayer == 3)
		ShareExp = 70;
	else if (nTotalPlayer == 4)
		ShareExp = 60;
	else if (nTotalPlayer == 5)
		ShareExp = 55;
	else if (nTotalPlayer == 6)
		ShareExp = 55;
	else if (nTotalPlayer == 7)
		ShareExp = 50;
	else if (nTotalPlayer == 8)
		ShareExp = 50;
	else
		ShareExp = 60;
	if (nShareFlag[0] == 1)
	{
		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;
		if (m_nPlayerIndex != g_Team[m_cTeam.m_nID].m_nCaptain || bCheck)
		{
			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * ShareExp / 100, nTarLevel);
		}
		else
		{
			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp , nTarLevel);
		}
		//int		nLeadExp = nCalExp * k * PlayerSet.m_cLevelAdd.GetLeadExpShare(Npc[m_nIndex].m_Series) * (nTotalLevel - Npc[m_nIndex].m_Level) / (nTotalLevel * 10000);
		int	nLeadExp = ((nExp * k) / 10000) * PlayerSet.m_cLevelAdd.GetLeadExpShare(Npc[m_nIndex].m_Series) * (nTotalLevel - Npc[m_nIndex].m_Level) / nTotalLevel;		//
		Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddLeadExp(nLeadExp);
	}
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (nShareFlag[i + 1] != 1)
			continue;
		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;
		if (m_nPlayerIndex != g_Team[m_cTeam.m_nID].m_nMember[i] || bCheck)
		{
			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * ShareExp / 100, nTarLevel);
		}
		else
		{
			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp, nTarLevel);
		}
	}
	//end code*/
	
#endif
}


//

void	KPlayer::AddSelfExp(int nExp, int nTarLevel)
{
	if (nExp <= 0)
		return;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;

	int		nSubLevel, nGetExp;
	nSubLevel = Npc[m_nIndex].m_Level - nTarLevel;
	if(nTarLevel > 80)
	{	
		if(Npc[m_nIndex].m_Level >= 90)
		{
			nGetExp = nExp;
		}
		else
		{
			if (nSubLevel <= 5)
			{
				nGetExp = nExp;
			}
			else if (nSubLevel <= 15)
			{
				nGetExp = nExp * (295 - 19 * nSubLevel) / 200;
			}
			else
			{
				nGetExp = nExp / 20;
			}
		}
	}
	else
	{
		if (nSubLevel <= 5)
		{
			nGetExp = nExp;
		}
		else if (nSubLevel <= 15)
		{
			nGetExp = nExp * (295 - 19 * nSubLevel) / 200;
		}
		else
		{
			nGetExp = nExp / 20;
		}
	}

	if (nGetExp <= 0)
		nGetExp = 1;
#ifdef _SERVER
	if(Npc[m_nIndex].m_SkillList.IsAddExp(Npc[m_nIndex].m_SkillList.GetSkillId120()))
	{
		int nEnhanceSkillExp = (int)(nGetExp / 1500.0 + 0.5);
		Npc[m_nIndex].m_SkillList.AddExpSkill120(nEnhanceSkillExp);
	}
#endif
	if(Npc[m_nIndex].m_CurrentExpEnhance > 0)
	{
		nGetExp += nGetExp * (Npc[m_nIndex].m_CurrentExpEnhance / 100.0);
	}
	
	m_nExp += nGetExp;

#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)
	{
		LevelUp();
	}
	else
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}



//	

void	KPlayer::DirectAddExp(int nExp)
{
	if (nExp > 0 && Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	
	m_nExp += nExp;
	if (m_nExp < 0)
	{
		if (-m_nExp > m_nNextLevelExp)
			m_nExp = -m_nNextLevelExp;
	}
#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)	
	{
		LevelUp();
	}
	else								
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}


//	

void	KPlayer::LevelUp()
{
	m_nExp = 0;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	Npc[m_nIndex].m_Level++;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	m_nAttributePoint += PLAYER_LEVEL_ADD_ATTRIBUTE;
	m_nSkillPoint += PLAYER_LEVEL_ADD_SKILL;
	
	LevelAddBaseLifeMax();
	LevelAddBaseStaminaMax();
	LevelAddBaseManaMax();
	
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;

	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
#ifdef _SERVER
	PLAYER_LEVEL_UP_SYNC	sLevelUp;
	sLevelUp.ProtocolType		= s2c_playerlevelup;
	sLevelUp.m_btLevel			= (DWORD)Npc[m_nIndex].m_Level;
	sLevelUp.m_nExp				= m_nExp;
	sLevelUp.m_nAttributePoint	= m_nAttributePoint;
	sLevelUp.m_nSkillPoint		= m_nSkillPoint;
	sLevelUp.m_nBaseLifeMax		= Npc[m_nIndex].m_LifeMax;
	sLevelUp.m_nBaseStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	sLevelUp.m_nBaseManaMax		= Npc[m_nIndex].m_ManaMax;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevelUp, sizeof(PLAYER_LEVEL_UP_SYNC));

	if ( m_cTeam.m_nFlag )
	{
		PLAYER_TEAMMATE_LEVEL_SYNC	sLevel;
		sLevel.ProtocolType = s2c_teammatelevel;
		sLevel.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
		sLevel.m_dwTeammateID = Npc[m_nIndex].m_dwID;

		if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0 && g_Team[m_cTeam.m_nID].m_nMember[i] != GetPlayerIndex())
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		}
	}
#endif
}


//	

void	KPlayer::UpdataCurData()
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;
	
	Npc[m_nIndex].m_CurrentLifeMax		= Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentManaMax		= Npc[m_nIndex].m_ManaMax;
	Npc[m_nIndex].m_CurrentStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentAttackRating	= Npc[m_nIndex].m_AttackRating;
	Npc[m_nIndex].m_CurrentAttackSpeed	= BASE_ATTACK_SPEED;
	Npc[m_nIndex].m_CurrentCastSpeed	= BASE_CAST_SPEED;
	ZeroMemory(&Npc[m_nIndex].m_CurrentColdDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentColdEnhance	= 0;
	Npc[m_nIndex].m_CurrentColdResist	= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentDamage2Mana	= 0;
	Npc[m_nIndex].m_CurrentDamageReduce	= 0;
	Npc[m_nIndex].m_CurrentDeadlyStrike	= 0;
	Npc[m_nIndex].m_CurrentDefend		= Npc[m_nIndex].m_Defend;
	Npc[m_nIndex].m_CurrentElementDamageReduce = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentFireDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentFireEnhance	= 0;
	Npc[m_nIndex].m_CurrentFireResist	= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentHandEnhance	= 0;
	Npc[m_nIndex].m_CurrentExpEnhance	= 0;		
	Npc[m_nIndex].m_CurrentHitRecover	= Npc[m_nIndex].m_HitRecover;
	Npc[m_nIndex].m_CurrentJumpSpeed	= Npc[m_nIndex].m_JumpSpeed;
	Npc[m_nIndex].m_CurrentKnockBack	= 0;
	Npc[m_nIndex].m_CurrentLifeReplenish	= Npc[m_nIndex].m_LifeReplenish;
	Npc[m_nIndex].m_CurrentLifeStolen	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentLightDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentLightEnhance	= 0;
	Npc[m_nIndex].m_CurrentLightResist	= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_CurrentLightResistMax = Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentManaPerEnemy	= 0;
	Npc[m_nIndex].m_CurrentManaReplenish	= Npc[m_nIndex].m_ManaReplenish;
	Npc[m_nIndex].m_CurrentManaStolen	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRetPercent = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMeleeEnhance, sizeof(Npc[m_nIndex].m_CurrentMeleeEnhance));
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	Npc[m_nIndex].m_CurrentPiercePercent	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentPoisonDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentPoisonEnhance	= 0;
	Npc[m_nIndex].m_CurrentPoisonResist	= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentRangeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentRangeDmgRetPercent	= 0;
	Npc[m_nIndex].m_CurrentRangeEnhance	= 0;
	Npc[m_nIndex].m_CurrentRunSpeed	= Npc[m_nIndex].m_RunSpeed;
	Npc[m_nIndex].m_CurrentSlowMissle	= 0;
	Npc[m_nIndex].m_CurrentStaminaGain	= Npc[m_nIndex].m_StaminaGain;
	Npc[m_nIndex].m_CurrentStaminaLoss	= Npc[m_nIndex].m_StaminaLoss;
	Npc[m_nIndex].m_CurrentStaminaStolen	= 0;
	Npc[m_nIndex].m_CurrentVisionRadius	= Npc[m_nIndex].m_VisionRadius;
	Npc[m_nIndex].m_CurrentWalkSpeed	= Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_CurrentAddPhysicsDamage = 0;
	m_nCurStrength						= m_nStrength;
	m_nCurDexterity						= m_nDexterity;
	m_nCurVitality						= m_nVitality;
	m_nCurEngergy						= m_nEngergy;
	m_nCurLucky							= m_nLucky;
	
	Npc[m_nIndex].m_CurrentFreezeTimeReducePercent	= 0;
	Npc[m_nIndex].m_CurrentPoisonTimeReducePercent	= 0;
	Npc[m_nIndex].m_CurrentStunTimeReducePercent		= 0;
	ZeroMemory(&Npc[m_nIndex].m_MagicPhysicsDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicColdDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicFireDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicLightDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicPoisonDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_ManaShield			= 0;
	Npc[m_nIndex].m_DynamicMagicShield	= 0;
	Npc[m_nIndex].m_CurrentReturnResist	= 0;
	Npc[m_nIndex].m_PoisonDamageReturn	= 0;
	Npc[m_nIndex].m_ReturnSkill			= 0;
	ZeroMemory(&Npc[m_nIndex].m_AutoReplySkill, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_LifeReplenishPercent	= 0;
	Npc[m_nIndex].m_IgnoreSkill				= 0;
	ZeroMemory(&Npc[m_nIndex].m_AutoAttackSkill, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_AppendSkill, sizeof(Npc[m_nIndex].m_AppendSkill));
	ZeroMemory(&Npc[m_nIndex].m_AutoDeathSkill, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_AutoRescueSkill, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_StaticMagicShield	= 0;

	UpdateSkillPointAdd();
	
	ReCalcEquip();
	ReCalcState();
}


//	

void KPlayer::ReCalcEquip()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	int nIdx = 0;
	int nActive = 0;
	for (int i = 0; i < itempart_num; i++)
	{
		nIdx = m_ItemList.m_EquipItem[i];
		if (!nIdx)
			continue;
		
		if (i == itempart_horse)
		{
			if (!Npc[m_nIndex].m_bRideHorse) 
					continue;
		}
		
		nActive = m_ItemList.GetEquipEnhance(i);
		if (m_ItemList.m_bActiveSet)
		{
			nActive = 3;
		}
		Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], nActive);
	}
}


//	

void KPlayer::ReCalcState()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	Npc[m_nIndex].ReCalcStateEffect();
}


//	

void	KPlayer::AddLeadExp(int nExp)
{
	if (m_dwLeadLevel >= MAX_LEAD_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;
	m_dwLeadExp += nExp;
	if (m_dwLeadExp >= (DWORD)PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel))
	{
		m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp, m_dwLeadLevel);
#ifndef _SERVER
		m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
#endif
	}
	
#ifdef _SERVER
	PLAYER_LEAD_EXP_SYNC	sLeadExp;
	sLeadExp.ProtocolType = s2c_playersyncleadexp;
	sLeadExp.m_dwLeadExp = m_dwLeadExp;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeadExp, sizeof(PLAYER_LEAD_EXP_SYNC));
#endif
	
}


//	

void KPlayer::LevelAddBaseLifeMax()
{
	Npc[m_nIndex].m_LifeMax += PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
}


//	

void KPlayer::LevelAddBaseStaminaMax()
{
	Npc[m_nIndex].m_StaminaMax += PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_nSex);
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
}


//	

void KPlayer::LevelAddBaseManaMax()
{
	Npc[m_nIndex].m_ManaMax += PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
}


//	

void	KPlayer::ChangePlayerCamp(int nCamp)
{
	if (nCamp < camp_begin || nCamp >= camp_num)
		return;
	Npc[m_nIndex].m_Camp = (NPCCAMP)nCamp;
}

#ifndef _SERVER

//	

void	KPlayer::ApplyTeamInfo(DWORD dwNpcID)
{
	if (dwNpcID <= 0)
		return;
	PLAYER_APPLY_TEAM_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_teamapplyinfo;
	/*sApplyInfo.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sApplyInfo.dwTimePacker = GetTickCount();*/
	sApplyInfo.dwID = 0;			
	sApplyInfo.dwTimePacker = 0;
	sApplyInfo.m_dwTarNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(PLAYER_APPLY_TEAM_INFO));
}
#endif

#ifndef _SERVER

//

void	KPlayer::ApplySelfTeamInfo()
{
	ApplyTeamInfo(Npc[m_nIndex].m_dwID);
}
#endif

#ifndef _SERVER

//

BOOL	KPlayer::ApplyCreateTeam()
{
	return m_cTeam.ApplyCreate();
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayer::ApplyTeamOpenClose(BOOL bFlag)
{
	if ( !m_cTeam.m_nFlag )					
		return FALSE;
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return FALSE;
	if (this->CheckTrading())
		return FALSE;
	
	if (Npc[m_nIndex].m_bOpenShop)
		return FALSE;
	
	
	if (bFlag)
	{
		if (PlayerSet.m_cLeadExp.GetMemNumFromLevel(m_dwLeadLevel) <= g_Team[0].m_nMemNum)
			return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	sTeamState;
	sTeamState.ProtocolType		= c2s_teamapplyopenclose;
	sTeamState.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sTeamState.dwTimePacker = GetTickCount();
	sTeamState.m_btOpenClose	= bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamState, sizeof(PLAYER_TEAM_OPEN_CLOSE));
	return TRUE;
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ApplyAddTeam(int nNpcIndex)
{	
	
	if (Npc[m_nIndex].m_bOpenShop)
		return;
	

	if (this->CheckTrading())
		return;
	
	if (m_cTeam.m_nFlag)				
		return;
	
	if (Npc[nNpcIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return;

	if (Npc[nNpcIndex].GetMenuState() != PLAYER_MENU_STATE_TEAMOPEN)
		return;
	
	m_cTeam.m_nApplyCaptainID = Npc[nNpcIndex].m_dwID;
	m_cTeam.m_dwApplyTimer = MAX_APPLY_TEAM_TIME;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType	= c2s_teamapplyadd;
	sAddTeam.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sAddTeam.dwTimePacker = GetTickCount();
	sAddTeam.m_dwTarNpcID	= Npc[nNpcIndex].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD_SELF_MSG, Npc[nNpcIndex].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::AcceptTeamMember(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	if (dwNpcID == 0)
		return;
	
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.UpdateInterface();
			break;
		}
	}

	PLAYER_ACCEPT_TEAM_MEMBER	sAcceptTeam;
	sAcceptTeam.ProtocolType	= c2s_teamacceptmember;
	sAcceptTeam.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sAcceptTeam.dwTimePacker = GetTickCount();
	sAcceptTeam.m_dwNpcID		= dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAcceptTeam, sizeof(PLAYER_ACCEPT_TEAM_MEMBER));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TeamDropApplyOne(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return;

	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.m_sApplyList[i].m_dwTimer = 0;
			m_cTeam.m_sApplyList[i].m_nLevel = 0;
			m_cTeam.m_sApplyList[i].m_szName[0] = 0;
			break;
		}
	}
}
#endif

#ifndef _SERVER

//	

void	KPlayer::LeaveTeam()
{
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType		= c2s_teamapplyleave;
	sLeaveTeam.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sLeaveTeam.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer(&sLeaveTeam, sizeof(PLAYER_APPLY_LEAVE_TEAM));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TeamKickMember(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_TEAM_KICK_MEMBER	sKickOne;
	sKickOne.ProtocolType	= c2s_teamapplykickmember;
	sKickOne.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sKickOne.dwTimePacker = GetTickCount();
	sKickOne.m_dwNpcID		= dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sKickOne, sizeof(PLAYER_TEAM_KICK_MEMBER));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ApplyTeamChangeCaptain(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	sTeamChange;
	sTeamChange.ProtocolType	= c2s_teamapplychangecaptain;
	sTeamChange.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sTeamChange.dwTimePacker = GetTickCount();
	sTeamChange.m_dwNpcID		= dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamChange, sizeof(PLAYER_APPLY_TEAM_CHANGE_CAPTAIN));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ApplyTeamDismiss()
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_DISMISS	sTeamDismiss;
	sTeamDismiss.ProtocolType	= c2s_teamapplydismiss;
	sTeamDismiss.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sTeamDismiss.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamDismiss, sizeof(PLAYER_APPLY_TEAM_DISMISS));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ApplySetPK(BOOL bPK)
{
	PLAYER_SET_PK	sPK;
	sPK.ProtocolType	= c2s_playerapplysetpk;
	sPK.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sPK.dwTimePacker = GetTickCount();
	sPK.m_bPKFlag		= bPK;
	if (g_pClient)
		g_pClient->SendPackToServer(&sPK, sizeof(PLAYER_SET_PK));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ApplyFactionData()
{
	PLAYER_APPLY_FACTION_DATA	sFaction;
	sFaction.ProtocolType	= c2s_playerapplyfactiondata;
	sFaction.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sFaction.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer(&sFaction, sizeof(PLAYER_APPLY_FACTION_DATA));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::SendChat(KUiMsgParam *pMsg, char *lpszSentence)
{
	if (!lpszSentence || !pMsg || pMsg->nMsgLength <= 0)
		return;
	int nLength = pMsg->nMsgLength;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	
	if (pMsg->eGenre == MSG_G_CHAT)
	{
		Npc[m_nIndex].SetChatInfo(pMsg->szName, lpszSentence, nLength);
		
		m_cChat.SendSentence(pMsg, Npc[m_nIndex].m_szChatBuffer);
	}
	else if (pMsg->eGenre == MSG_G_CMD)
	{
	}
}
#endif

#ifndef _SERVER

//

void	KPlayer::ApplyAddBaseAttribute(int nAttribute, int nNo)
{
	if (nAttribute < 0 || nAttribute > 3)
		return;

	if (nNo <= 0 || nNo >= 255)
		return;

	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	sAdd;

	sAdd.ProtocolType	= c2s_playeraddbaseattribute;
	sAdd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sAdd.dwTimePacker = GetTickCount();
	sAdd.m_btAttribute	= nAttribute;
	sAdd.m_btAddNo = nNo;		//
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(PLAYER_ADD_BASE_ATTRIBUTE_COMMAND));
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayer::ApplyAddSkillLevel(int nSkillID, int nAddPoint)
{
	if ( !Npc[m_nIndex].m_SkillList.FindSame(nSkillID) )
		return FALSE;
	if (m_nSkillPoint < nAddPoint || nAddPoint + Npc[m_nIndex].m_SkillList.GetLevel(nSkillID) > MAX_SKILLLEVEL)
		return FALSE;
	
	PLAYER_ADD_SKILL_POINT_COMMAND	sSkill;

	sSkill.ProtocolType		= c2s_playerapplyaddskillpoint;
	sSkill.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sSkill.dwTimePacker = GetTickCount();
	sSkill.m_nSkillID		= nSkillID;
	sSkill.m_nAddPoint		= nAddPoint;
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sSkill, sizeof(PLAYER_ADD_SKILL_POINT_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayer::ApplyUseItem(int nItemID, ItemPos SrcPos)
{
	if (this->CheckTrading())
		return FALSE;
	
	if (Npc[m_nIndex].m_bOpenShop)
		return FALSE;
	
	int nRet = m_ItemList.UseItem(nItemID,SrcPos.nPlace);
	//g_DebugLog("nRes = :%d",nRet);
	if (nRet == 0)
		return FALSE;
	

	if (nRet == REQUEST_EAT_MEDICINE)
	{
		PLAYER_EAT_ITEM_COMMAND	sEat;
		sEat.ProtocolType = c2s_playereatitem;
		sEat.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sEat.dwTimePacker = GetTickCount();
		sEat.m_nItemID = nItemID;
		sEat.m_btPlace = SrcPos.nPlace;
		sEat.m_btX = SrcPos.nX;
		sEat.m_btY = SrcPos.nY;
		if (g_pClient)
			g_pClient->SendPackToServer(&sEat, sizeof(PLAYER_EAT_ITEM_COMMAND));
	}
	else if (nRet == REQUEST_EQUIP_ITEM)
	{
		if (!m_ItemList.IsLockOperation() && !CheckTrading())
		{
			ItemPos DesPos;
			DesPos.nPlace = DesPos.nX = DesPos.nY = -1;
			DesPos.nPlace = pos_equip;
			DesPos.nX = m_ItemList.SearchPlaceEquip(nItemID,DesPos.nPlace);

			if (DesPos.nPlace < 0)
			{
				return FALSE;
			}

			MoveItem(SrcPos, SrcPos);

			PLAYER_MOVE_ITEM_COMMAND	sMove2;
			sMove2.ProtocolType = c2s_playermoveitem;
			sMove2.m_btDownPos = DesPos.nPlace;
			sMove2.dwTimePacker = GetTickCount();
			sMove2.m_btDownX = DesPos.nX;
			sMove2.m_btDownY = DesPos.nY;
			sMove2.m_btUpPos = DesPos.nPlace;
			sMove2.m_btUpX = DesPos.nX;
			sMove2.m_btUpY = DesPos.nY;
			if (g_pClient)
				g_pClient->SendPackToServer(&sMove2, sizeof(PLAYER_MOVE_ITEM_COMMAND));


			PLAYER_MOVE_ITEM_COMMAND	sMove;
			sMove.ProtocolType = c2s_playermoveitem;
			sMove.m_btDownPos = SrcPos.nPlace;
			sMove.dwTimePacker = GetTickCount();
			sMove.m_btDownX = SrcPos.nX;
			sMove.m_btDownY = SrcPos.nY;
			sMove.m_btUpPos = SrcPos.nPlace;
			sMove.m_btUpX = SrcPos.nX;
			sMove.m_btUpY = SrcPos.nY;
			if (g_pClient)
				g_pClient->SendPackToServer(&sMove, sizeof(PLAYER_MOVE_ITEM_COMMAND));
		}
		
	}
	else if(REQUEST_EQUIP_ITEM_TO_EQUIPROOM)
	{
		if (!m_ItemList.IsLockOperation() && !CheckTrading())
		{
			ItemPos DesPos;
			DesPos.nPlace = DesPos.nX = DesPos.nY = -1;
			DesPos.nPlace = pos_equiproom;
			SrcPos.nX = m_ItemList.SearchPlaceEquip(nItemID,DesPos.nPlace);		

			if (DesPos.nPlace < 0)
			{
				return FALSE;
			}

			MoveItem(SrcPos, SrcPos);	

			if (m_ItemList.SearchPosition((int)Item[nItemID].GetWidth(), (int)Item[nItemID].GetHeight(), &DesPos))
			{
				PLAYER_MOVE_ITEM_COMMAND	sMove2;
				sMove2.ProtocolType = c2s_playermoveitem;
				sMove2.m_btDownPos = DesPos.nPlace;
				sMove2.dwTimePacker = GetTickCount();
				sMove2.m_btDownX = DesPos.nX;
				sMove2.m_btDownY = DesPos.nY;
				sMove2.m_btUpPos = DesPos.nPlace;
				sMove2.m_btUpX = DesPos.nX;
				sMove2.m_btUpY = DesPos.nY;
				if (g_pClient)
					g_pClient->SendPackToServer(&sMove2, sizeof(PLAYER_MOVE_ITEM_COMMAND));
			}	
		}
	}
	return TRUE;
}
#endif

#ifndef _SERVER

//	

void	KPlayer::PickUpObj(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	
	if (Npc[m_nIndex].m_bOpenShop)
		return;
	
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Item && Object[nObjIndex].m_nKind != Obj_Kind_Money)
		return;
	
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
	{
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sPickUp.dwTimePacker = GetTickCount();
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = 0;
		sPickUp.m_btPosX = 0;
		sPickUp.m_btPosY = 0;
	}
	else
	{
		ItemPos	sItemPos;
		if ( FALSE == m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
		{
			KSystemMessage	sMsg;
			
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.eType = SMT_NORMAL;
			sMsg.byParamSize = 0;
			sMsg.byPriority = 0;
			
			strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			return;
		}
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sPickUp.dwTimePacker = GetTickCount();
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;
		sPickUp.m_btPosY = sItemPos.nY;
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ObjMouseClick(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	
	if (Npc[m_nIndex].m_bOpenShop)
		return;
	
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Box && Object[nObjIndex].m_nKind != Obj_Kind_Prop && Object[nObjIndex].m_nKind != Obj_Kind_Task)
		return;
	SendObjMouseClick(Object[nObjIndex].m_nID, SubWorld[0].m_Region[Object[nObjIndex].m_nRegionIdx].m_RegionID);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::MoveItem(ItemPos DownPos, ItemPos UpPos)
{	
	if(Npc[m_nIndex].m_bOpenShop)
		return;
	//
	if(DownPos.nPlace == pos_traderoom)
	{
		if( !CheckTrading() )
			return;
		if (this->m_cTrade.m_bTradeLock)
			return;
		const int nHandIdx = m_ItemList.Hand();
		if( (nHandIdx > 0) && (nHandIdx < MAX_ITEM) )
		{
			if(Item[nHandIdx].GetBindState() > 0)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_NOT_TRADE_ITEM);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return;
			}
			//Add By ShinT
			if(Item[nHandIdx].GetGenre() == 4)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_NOT_TRADE_ITEM);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return;
			}
			//end
			if( (Item[nHandIdx].GetGenre() == item_task) )
			{
				if(Item[nHandIdx].GetParticular() == 0)
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return;
				}
			}
		}
	}
	//
	if( DownPos.nPlace == pos_immediacy )
	{	
		if( CheckTrading() )
			return;
		const int nHandIdx = m_ItemList.Hand();
		if( (nHandIdx > 0) && (nHandIdx < MAX_ITEM) )
		{	
			if(Item[nHandIdx].GetWidth() != 1 ||
				Item[nHandIdx].GetHeight() != 1)
				return;

			if(	(Item[nHandIdx].GetGenre() != item_medicine) &&
				(Item[nHandIdx].GetGenre() != item_script) )
				return;
			
			if(Item[nHandIdx].GetGenre() == item_script)
			{
				if(Item[nHandIdx].GetMaxStackCount() > 0)
					return;
			}
			
			if( m_ItemList.CheckSameDetailType(room_immediacy, Item[nHandIdx].GetGenre(), Item[nHandIdx].GetDetailType()) )
			{
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return;
			}
		}
	}
	if(DownPos.nPlace == pos_equip)
	{	
		if( CheckTrading() )
			return;
	}
	//
	if(DownPos.nPlace >= pos_repositoryroom && DownPos.nPlace <= pos_expandtoryroom1)//Expandbox by kinnox;
	{
		if( CheckTrading() )
			return;
	}
	SendClientCmdMoveItem(&DownPos, &UpPos);
	//g_DebugLog("Send Move Item CLIENT -> SERVER");
}
#endif

#ifndef _SERVER

//	

int	KPlayer::ThrowAwayItem()
{	
	if( CheckTrading() )
		return 0;
	
	if( Npc[m_nIndex].m_bOpenShop )
		return 0;
	if( !m_ItemList.Hand() )
		return 0;

	if(Item[m_ItemList.Hand()].GetBindState() > 0)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0; 
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}
	if(Item[m_ItemList.Hand()].GetDetailType() == ItemSet.m_nCoinID)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0; 
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;	
	}
	//AddBy ShinT
	if(Item[m_ItemList.Hand()].GetGenre() == 4)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0; 
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}
	//End
	//
	if(Item[m_ItemList.Hand()].GetGenre() == item_goldequip)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}
	if(Item[m_ItemList.Hand()].GetGenre() == item_purpleequip)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}
	
	//
	PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	sThrow.ProtocolType = c2s_playerthrowawayitem;
	sThrow.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sThrow.dwTimePacker = GetTickCount();
	sThrow.dwIDItem = Item[m_ItemList.m_Hand].GetID();
	sThrow.bAutoPick = FALSE;
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));

	return 1;
}


//	

int	KPlayer::AutoThrowAwayItem(int nIndexItem)
{	
	if (this->CheckTrading())
		return 0;
	
	if (Npc[m_nIndex].m_bOpenShop)
		return 0;
	

	if(Item[nIndexItem].GetBindState() > 0)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}

	if(Item[nIndexItem].GetGenre() == item_goldequip)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMT_NORMAL;
		Msg.eType = SMCT_NONE;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		sprintf(Msg.szMessage, MSG_NOT_THROW_ITEM);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return 0;
	}
	
	
	PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	
	sThrow.ProtocolType = c2s_playerthrowawayitem;
	sThrow.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sThrow.dwTimePacker = GetTickCount();
	sThrow.dwIDItem = Item[nIndexItem].GetID();
	sThrow.bAutoPick = TRUE;


	if (g_pClient)
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));

	return 1;
}

#endif

#ifndef _SERVER

//	

void	KPlayer::ChatAddFriend(int nPlayerIdx)
{
	CHAT_ADD_FRIEND_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_chataddfriend;
	sAdd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sAdd.dwTimePacker = GetTickCount();
	sAdd.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(CHAT_ADD_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::ChatRefuseFriend(int nPlayerIdx)
{
	CHAT_REFUSE_FRIEND_COMMAND	sRefuse;
	sRefuse.ProtocolType = c2s_chatrefusefriend;
	sRefuse.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sRefuse.dwTimePacker = GetTickCount();
	sRefuse.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sRefuse, sizeof(CHAT_REFUSE_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TradeApplyOpen(char *lpszSentence, int nLength)
{	
	if (Npc[m_nIndex].m_bOpenShop)
		return;

	if (this->CheckTrading())
		return;

	if (m_bLockPlayer)//LockPlayer by kinnox;
		return;		
		
	TRADE_APPLY_OPEN_COMMAND	sOpen;
	sOpen.ProtocolType			= c2s_tradeapplystateopen;
	sOpen.dwID					= Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sOpen.dwTimePacker			= GetTickCount();
	sOpen.m_bReserve			= 31;

	if(g_pClient)
		g_pClient->SendPackToServer(&sOpen, sizeof(TRADE_APPLY_OPEN_COMMAND));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TradeApplyClose()
{	
	
	if(this->CheckTrading())
		return;
	
	TRADE_APPLY_CLOSE_COMMAND	sApply;
	sApply.ProtocolType		= c2s_tradeapplystateclose;
	sApply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sApply.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TRADE_APPLY_CLOSE_COMMAND));
}
#endif

#ifndef _SERVER
//	
void	KPlayer::TradeApplyStart(int nNpcIdx)
{	
	
	if(Npc[nNpcIdx].m_bOpenShop)
		return;
	if(this->CheckTrading())
		return;

	if (this->m_bLockPlayer)//LockPlayer by kinnox;
		return;			

	if (nNpcIdx < 0 || nNpcIdx >= MAX_NPC)
		return;

	TRADE_APPLY_START_COMMAND	sStart;
	sStart.ProtocolType		= c2s_tradeapplystart;
	sStart.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sStart.dwTimePacker = GetTickCount();
	sStart.m_dwID			= Npc[nNpcIdx].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sStart, sizeof(TRADE_APPLY_START_COMMAND));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TRADE_SEND_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER
//	
BOOL	KPlayer::TradeMoveMoney(int nMoney)
{
	if (!CheckTrading())
		return FALSE;
	//
	if (this->m_cTrade.m_bTradeLock)
		return FALSE;
	
	if (nMoney < 0 || nMoney > m_ItemList.GetEquipmentMoney() + m_ItemList.GetTradeMoney())//fix trade money by kinnox;
		return FALSE;
	
	TRADE_MOVE_MONEY_COMMAND	sMoney;
	
	sMoney.ProtocolType		= c2s_trademovemoney;
	sMoney.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sMoney.dwTimePacker = GetTickCount();
	sMoney.m_nMoney			= nMoney;
	if (g_pClient)
		g_pClient->SendPackToServer(&sMoney, sizeof(TRADE_MOVE_MONEY_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TradeDecision(int nDecision)
{	

	if(Npc[m_nIndex].m_bOpenShop)
		return;

	if (!CheckTrading())
		return;

	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType		= c2s_tradedecision;
	sDecision.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sDecision.dwTimePacker = GetTickCount();
	if (nDecision == 1)
	{
		sDecision.m_btDecision = 1;
	}
	else if (nDecision == 0)
	{
		sDecision.m_btDecision = 0;
	}
	else if (nDecision == 2)
	{
		sDecision.m_btDecision = 4;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER



void	KPlayer::TradeApplyLock(BOOL bLockOrNot)
{	
	if (Npc[m_nIndex].m_bOpenShop)
		return;

	if (!this->CheckTrading())
		return;

	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	sDecision.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sDecision.dwTimePacker = GetTickCount();
	if (bLockOrNot)
	{
		sDecision.m_btDecision = 2;
	}
	else
	{
		sDecision.m_btDecision = 3;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cApplyAddTeam(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	int nNpcNo = NpcSet.SearchID(pAddTeam->m_dwTarNpcID);
	if (nNpcNo == 0)
		return;
	
	int i, nFreeListNo;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == pAddTeam->m_dwTarNpcID)
		{
			m_cTeam.m_sApplyList[i].m_nMHP = Npc[nNpcNo].m_CurrentLifeMax;
			m_cTeam.m_sApplyList[i].m_nHP = Npc[nNpcNo].m_CurrentLife;
			m_cTeam.m_sApplyList[i].m_nLevel = Npc[nNpcNo].m_Level;
			m_cTeam.m_sApplyList[i].m_dwTimer = MAX_APPLY_TEAM_TIME;
			strcpy(m_cTeam.m_sApplyList[i].m_szName, Npc[nNpcNo].Name);
			m_cTeam.UpdateInterface();
			return;
		}
	}

	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == 0)
		{
			nFreeListNo = i;
			break;
		}
	}
	if (i >= MAX_TEAM_APPLY_LIST)
		return;
	
	m_cTeam.m_sApplyList[nFreeListNo].m_dwNpcID = pAddTeam->m_dwTarNpcID;
	m_cTeam.m_sApplyList[nFreeListNo].m_nLevel = Npc[nNpcNo].m_Level;
	m_cTeam.m_sApplyList[nFreeListNo].m_nHP = Npc[nNpcNo].m_CurrentLife;
	m_cTeam.m_sApplyList[nFreeListNo].m_nMHP = Npc[nNpcNo].m_CurrentLifeMax;
	m_cTeam.m_sApplyList[nFreeListNo].m_dwTimer = MAX_APPLY_TEAM_TIME;
	strcpy(m_cTeam.m_sApplyList[nFreeListNo].m_szName, Npc[nNpcNo].Name);
	
	m_cTeam.UpdateInterface();
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD, Npc[nNpcNo].Name);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_UI_TEAM_APPLY;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);

	KUiPlayerItem	player;
	strcpy(player.Name, Npc[nNpcNo].Name);
	player.nIndex = 0;
	player.uId = pAddTeam->m_dwTarNpcID;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&player);
	
	return;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::AddFaction(char *lpszFactionName)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, lpszFactionName) )
		return FALSE;
	
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	Npc[m_nIndex].SetCurrentCamp(m_cFaction.GetGurFactionCamp());
	
	
	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::AddFaction(int nFactionID)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, nFactionID) )
		return FALSE;
	
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	Npc[m_nIndex].SetCurrentCamp(m_cFaction.GetGurFactionCamp());
	

	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::LeaveCurFaction()
{
	m_cFaction.LeaveFaction();
	
	Npc[m_nIndex].SetCamp(camp_free);
	
	PLAYER_LEAVE_FACTION	sLeave;
	sLeave.ProtocolType = s2c_playerleavefaction;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeave, sizeof(PLAYER_LEAVE_FACTION));
	
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::CurFactionOpenSkill(int nLevel)
{
	if ( !m_cFaction.OpenCurSkillLevel(nLevel, &Npc[m_nIndex].m_SkillList) )
		return FALSE;
	
	PLAYER_FACTION_SKILL_LEVEL	sLevel;
	sLevel.ProtocolType = s2c_playerfactionskilllevel;
	sLevel.m_btCurFactionID = m_cFaction.m_nCurFaction;
	sLevel.m_btLevel = nLevel;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_FACTION_SKILL_LEVEL));
	
	return TRUE;
}
#endif

#ifdef _SERVER

//	

void	KPlayer::TaskClearTempVal()
{
	this->m_cTask.ClearTempVar();
}
#endif




int		KPlayer::TaskGetSaveVal(int nNo)
{
	return this->m_cTask.GetSaveVal(nNo);
}




void	KPlayer::TaskSetSaveVal(int nNo, BOOL bFlag)
{	

	if(nNo == TV_TRANSLIFE)
	{	
		if(bFlag < 0)
			bFlag = 0;

		if(bFlag > MAX_TRANSLIFE_VALUE)
			bFlag = MAX_TRANSLIFE_VALUE;

	}

	if(nNo == TV_REPUTE)
	{	
		if(bFlag > MAX_REUPTE_VALUE)
			bFlag = MAX_REUPTE_VALUE;

		if(bFlag < -MAX_REUPTE_VALUE)
			bFlag = -MAX_REUPTE_VALUE;
	}

	if(nNo == TV_FUYUAN)
	{	
		if(bFlag > MAX_FUYUAN_VALUE)
			bFlag = MAX_FUYUAN_VALUE;

		if(bFlag < -MAX_FUYUAN_VALUE)
			bFlag = -MAX_FUYUAN_VALUE;
	}

	this->m_cTask.SetSaveVal(nNo, bFlag);

#ifdef _SERVER
	TASK_VALUE_SYNC		sValue;
	sValue.ProtocolType		= s2c_taskvaluesync;
	sValue.m_bClear			= FALSE;
	sValue.m_nId			= (USHORT)nNo;
	sValue.m_bValue			= bFlag;

	if(g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sValue, sizeof(TASK_VALUE_SYNC));

#endif
}




int		KPlayer::TaskGetClearVal(int nNo)
{	
	return this->m_cTask.GetClearVal(nNo);;
}




void	KPlayer::TaskSetClearVal(int nNo, int nVal)
{
	this->m_cTask.SetClearVal(nNo, nVal);

#ifdef _SERVER
	TASK_VALUE_SYNC		sValue;
	sValue.ProtocolType		= s2c_taskvaluesync;
	sValue.m_bClear			= TRUE;
	sValue.m_nId			= (USHORT)nNo;
	sValue.m_bValue			= (BOOL)nVal;

	if(g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sValue, sizeof(TASK_VALUE_SYNC));
#endif

}




void KPlayer::CheckRideHorse(BOOL bFlagRide)
{
    int ItemActiveIdx = m_ItemList.GetEquipment(itempart_horse);
    if(ItemActiveIdx <= 0)
        return;
    if(bFlagRide)
    {

		Npc[m_nIndex].m_bRideHorse = FALSE;
		Item[ItemActiveIdx].RemoveMagicAttribFromNPC(&Npc[m_nIndex], 3);
    }
    else
    { 

		 Npc[m_nIndex].m_bRideHorse = TRUE;
		 Item[ItemActiveIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], 3);
    }
#ifdef _SERVER
	PLAYER_RIDE_SYNC	sValue;
	sValue.ProtocolType		= s2c_playerridesync;
	sValue.m_bRideFlag		= bFlagRide;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(PLAYER_RIDE_SYNC));
#endif
}


//	

void	KPlayer::SetLevel(BYTE btLevel)
{
	if(btLevel == Npc[m_nIndex].m_Level)
		return;

	if (Npc[m_nIndex].m_Level > MAX_LEVEL)
		return;

	if(btLevel < 1 || btLevel > MAX_LEVEL)
		return ;

	m_nExp = 0;
	
	
	Npc[m_nIndex].m_Level = btLevel;
	
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[Npc[m_nIndex].m_Series];
	m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[Npc[m_nIndex].m_Series];
	m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[Npc[m_nIndex].m_Series];
	m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[Npc[m_nIndex].m_Series];
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;

	Npc[m_nIndex].m_LifeMax = (UINT)((btLevel * PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series)) +
		(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
	
	
	Npc[m_nIndex].m_ManaMax = (int)(btLevel * PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series) + 
			(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * m_nCurEngergy));
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
	
	Npc[m_nIndex].m_StaminaMax = (int)(btLevel * PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_nSex) + 
		(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
	
#ifdef _SERVER
	Npc[m_nIndex].m_SkillList.ClearSkillLevel(true);
#endif
	m_nAttributePoint = (btLevel - 1) * 5;
	m_nSkillPoint = btLevel - 1;
	
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
#ifdef _SERVER
	PLAYER_LEVEL_UP_SYNC	sLevelUp;
	sLevelUp.ProtocolType		= s2c_playerlevelup;
	sLevelUp.m_btLevel			= (DWORD)Npc[m_nIndex].m_Level;
	sLevelUp.m_nExp				= -2;
	sLevelUp.m_nAttributePoint	= m_nAttributePoint;
	sLevelUp.m_nSkillPoint		= m_nSkillPoint;
	sLevelUp.m_nBaseLifeMax		= Npc[m_nIndex].m_LifeMax;
	sLevelUp.m_nBaseStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	sLevelUp.m_nBaseManaMax		= Npc[m_nIndex].m_ManaMax;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevelUp, sizeof(PLAYER_LEVEL_UP_SYNC));
#endif
}


//	

void KPlayer::RefeshPlayer(bool bClear /*= false*/)
{	
	if(bClear)
	{
		Npc[m_nIndex].ClearStateSkillEffect();
		Npc[m_nIndex].ClearNormalState();
	}
	else
	{
		UpdataCurData();
	}
#ifdef _SERVER
	PLAYER_REFRESH_SYNC		sRefresh;
	sRefresh.ProtocolType	= s2c_refeshplayersync;
	sRefresh.m_bClear		= bClear;

	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sRefresh, sizeof(PLAYER_REFRESH_SYNC));
#endif
}


//	

void KPlayer::AddAllSkillPoint(int nAdd)
{
	for(int i = 1; i < MAX_NPCSKILL; i++)
	{	
		if(Npc[m_nIndex].m_SkillList.m_Skills[i].SkillLevel <= 0)
			continue;

		AddSkillPoint(Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId, nAdd);
	}
}


//	

void KPlayer::AddSkillPoint(int SkillID, int nAdd)
{	
	if(SkillID == 0)
		return;

	int i = 0;
	if(SkillID < 0)
		SkillID = -SkillID;

	i = Npc[m_nIndex].m_SkillList.FindSame(SkillID);
	if(i >= MAX_NPCSKILL)
		return;
	
	if(i <= 0)
	{	
		if(nAdd > 0)
		{	
			ISkill * pSkill =  g_SkillManager.GetSkill(SkillID, nAdd);
			if (!pSkill) 
				return;
			
			if (pSkill->IsExpSkill())
				return;

			Npc[m_nIndex].m_SkillList.Add(SkillID, nAdd, true);
		}
		return;
	}
	else
	{	

		if(Npc[m_nIndex].m_SkillList.m_Skills[i].SkillExpand == true)
		{	
			if(nAdd < 0)
			{
				Npc[m_nIndex].m_SkillList.Remove(SkillID);
			}
			return;
		}

	}
	
	if(nAdd < 0)
	{	
		if(Npc[m_nIndex].m_SkillList.m_Skills[i].PointAdd <= 0)
			return;
	}

	if(Npc[m_nIndex].m_SkillList.m_Skills[i].CurrentSkillLevel > 0)
	{
		Npc[m_nIndex].m_SkillList.IncreaseLevel(i, nAdd, TRUE);
	}
}


//	

void KPlayer::UpdateSkillPointAdd()
{	
	for(int i = 1; i < MAX_NPCSKILL; i++)
	{	
		if(Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(i) <= 0)
			continue;
		Npc[m_nIndex].m_SkillList.ResetSkillPointAdd(i);
	}
}

#ifndef _SERVER

//	

void	KPlayer::SetDefaultImmedSkill()
{
	int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
	int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
		
	if (nDetailType == 0)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
	}
	else if (nDetailType == 1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
	}
	else if (nDetailType == -1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
	}
}


//	

void	KPlayer::SetLeftSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nLeftSkillID = nSkillID;
	
	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nLeftSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -1);
}


//	

void	KPlayer::SetRightSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nRightSkillID = nSkillID;

	ISkill * pOrdinSkill = g_SkillManager.GetSkill(nSkillID, 1);
	if (!pOrdinSkill) 
        return;
	
	if (pOrdinSkill->IsAura())
	{
		Npc[m_nIndex].SetAuraSkill(nSkillID);
	}
	else
	{
		Npc[m_nIndex].SetAuraSkill(0);
	}
	
	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nRightSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -2);
}


//	

void KPlayer::UpdateWeaponSkill()
{
	if (m_nLeftSkillID > 0)
	{
		ISkill * pISkill = g_SkillManager.GetSkill(m_nLeftSkillID, 1);
		if (!pISkill) 
            return;
		
		//
		if (pISkill->GetSkillAttrib() < 100)
		{
			SetLeftSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
		
		
	}
	if (m_nRightSkillID > 0)
	{
		ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(m_nRightSkillID, 1);
		if (!pISkill) 
            return;
		
		//
		if (pISkill->GetSkillAttrib() < 100)
		{
			SetRightSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
		
	}
}
#endif

#ifndef _SERVER

//	

void	KPlayer::SetChatCurChannel(int nChannelNo)
{
	m_cChat.SetCurChannel(nChannelNo);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::TeamInviteAdd(DWORD dwNpcID)
{
	m_cTeam.InviteAdd(dwNpcID);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ServerSendChat(BYTE* pProtocol)
{
	m_cChat.ServerSendChat(m_nPlayerIndex, pProtocol);
}
#endif

#ifdef _SERVER

//

void	KPlayer::AddBaseAttribute(BYTE* pProtocol)
{
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	*pAdd = (PLAYER_ADD_BASE_ATTRIBUTE_COMMAND*)pProtocol;
	switch (pAdd->m_btAttribute)
	{
		case ATTRIBUTE_STRENGTH:
			AddBaseStrength(pAdd->m_btAddNo);
			break;
		case ATTRIBUTE_DEXTERITY:
			AddBaseDexterity(pAdd->m_btAddNo);
			break;
		case ATTRIBUTE_VITALITY:
			AddBaseVitality(pAdd->m_btAddNo);
			break;
		case ATTRIBUTE_ENGERGY:
			AddBaseEngergy(pAdd->m_btAddNo);
			break;
	}
}
#endif

#define MAX_ORDINSKILL_LEVEL_ALWAYS  20
#ifdef _SERVER

//	

void	KPlayer::AddSkillPoint(BYTE* pProtocol)
{
	PLAYER_ADD_SKILL_POINT_COMMAND	*pAdd = (PLAYER_ADD_SKILL_POINT_COMMAND*)pProtocol;
	
	int		nSkillIndex, nSkillLevel;
	
	int	nPointAdd = 0;
	

	nSkillIndex = Npc[m_nIndex].m_SkillList.FindSame(pAdd->m_nSkillID);
	if (nSkillIndex == 0)
		return;

	nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
	
	nPointAdd	= Npc[m_nIndex].m_SkillList.GetSkillIdxPointAdd(nSkillIndex);
	
	
	
	if (m_nSkillPoint >= pAdd->m_nAddPoint && nSkillIndex )
	{
		ISkill * pSkill = NULL;
		if (nSkillLevel == 0)
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, 1);
		}
		else
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, nSkillLevel);
		}
		
		if (pSkill)
		{	
			if(pSkill->IsExpSkill())
			{
				char szMsg[128];
				sprintf(szMsg, "<color=green>[ %s ]<color> c�n ph�i tu luy�n m�i th�ng c�p ���c.", pSkill->GetSkillName());
				KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "K� n�ng", (char *)szMsg, strlen(szMsg));
				return;
			}

			if (pSkill->GetSkillLevelUpScriptId())
			{
				ExecuteScript(pSkill->GetSkillLevelUpScriptId(), "main", pAdd->m_nAddPoint);
				nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
			}
			else
			{
				switch(pSkill->GetSkillStyle())
				{
				case SKILL_SS_Missles:
				case SKILL_SS_Melee:
				case SKILL_SS_InitiativeNpcState:	
				case SKILL_SS_PassivityNpcState:				
					{	
						int nWantToBeLevel = nSkillLevel + pAdd->m_nAddPoint;
						
						BYTE btMaxLevel = g_SkillManager.GetSkillMaxLevel(pAdd->m_nSkillID) + TaskGetSaveVal(TV_TRANSLIFE);
						
						
						if (nWantToBeLevel <= btMaxLevel && nWantToBeLevel <= Npc[m_nIndex].m_Level - ((KSkill*)pSkill)->GetSkillReqLevel() + 1)
						{
							if (!Npc[m_nIndex].m_SkillList.IncreaseLevel(nSkillIndex, pAdd->m_nAddPoint) )
								return;
							m_nSkillPoint -= pAdd->m_nAddPoint;
							nSkillLevel += pAdd->m_nAddPoint;
						}
						else
						{
							if (nWantToBeLevel >=  btMaxLevel)
							{
								char szMsg[128];
								sprintf(szMsg, "<color=green>[ %s ]<color> �� ��t c�p t�i �a kh�ng th� l�nh ng� th�m.", pSkill->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "K� n�ng", (char *) szMsg, strlen(szMsg) );
							}
							else 
							{
								char szMsg[128];
								sprintf(szMsg, "C�p �� tu luy�n <color=gold>%d c�p<color> m�i c� th� t�ng k� n�ng <color=green>[ %s ]<color>.", ((KSkill *)pSkill)->GetSkillReqLevel() -1 +  pAdd->m_nAddPoint + nSkillLevel,  ((KSkill*)pSkill)->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "K� n�ng", (char *) szMsg, strlen(szMsg) );
							}
							
						}
					}
					break;
					
				case SKILL_SS_Thief:
					{
						return;
					}break;
					
				}
				
			}
			
			PLAYER_SKILL_LEVEL_SYNC	sSkill;
			sSkill.ProtocolType = s2c_playerskilllevel;
			sSkill.m_nSkillID = pAdd->m_nSkillID;
			sSkill.m_nSkillLevel = nSkillLevel;
			sSkill.m_nLeavePoint = m_nSkillPoint;
			
			sSkill.m_btPointAdd	 = nPointAdd;
			sSkill.m_bExpand	 = false;
			
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));

			
			this->RefeshPlayer(TRUE);
			
		}
	}
	
	
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::ServerPickUpItem(BYTE* pProtocol)
{
	PLAYER_PICKUP_ITEM_COMMAND	*pPickUp = (PLAYER_PICKUP_ITEM_COMMAND*)pProtocol;
	
	int		nObjIndex, nNpcX, nNpcY, nObjX, nObjY;
	nObjIndex = ObjSet.FindID(pPickUp->m_nObjID);
	if (nObjIndex == 0)
		return FALSE;
	/*
	if(Object[nObjIndex].m_nKind == Obj_Kind_Item)
	{	
		const int nItemIdx = Object[nObjIndex].m_nItemDataID;
		if( (Item[nItemIdx].GetGenre() == item_script) && (Item[nItemIdx].GetParticular() == 1) )
		{	
			g_pMainScript = (KLuaScript*)g_GetScript(Item[nItemIdx].GetScriptID());
			if(g_pMainScript)
			{	
				int nTopIndex = 0;
				g_pMainScript->SafeCallBegin(&nTopIndex);
				g_pMainScript->GetGlobalName("IsPickable");
				nTopIndex = Lua_GetTopIndex(g_pMainScript->m_LuaState);
				if(Lua_GetValueType(g_pMainScript->m_LuaState, nTopIndex) == 5)
				{
					if(g_pMainScript->CallFunction("IsPickable", 1, "d", nItemIdx) == 1)
					{
						Object[nObjIndex].m_nBelong = -1;
					}
					else
					{
						g_pMainScript->SafeCallEnd(nTopIndex);
						return FALSE;
					}
				}
				g_pMainScript->SafeCallEnd(nTopIndex);
			}
			

		}
	}*/
	
	//
	if (Object[nObjIndex].m_nBelong != -1)
	{
		if (!m_cTeam.m_nFlag)
		{
			if (Object[nObjIndex].m_nBelong != m_nPlayerIndex)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
				else
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				return FALSE;
			}
		}
		else
		{
			if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
			{
				if (Object[nObjIndex].m_nBelong != m_nPlayerIndex &&
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else if (Object[nObjIndex].m_nKind == Obj_Kind_Item)
			{
				if( (Object[nObjIndex].m_nItemDataID <= 0) || (Object[nObjIndex].m_nItemDataID >= MAX_ITEM) )
					return FALSE;

				if (((Item[Object[nObjIndex].m_nItemDataID].GetGenre() == item_task || Item[Object[nObjIndex].m_nItemDataID].GetGenre() == item_script) && 
					Object[nObjIndex].m_nBelong != m_nPlayerIndex) ||
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
	}

	if (Object[nObjIndex].m_nSubWorldID != Npc[m_nIndex].m_SubWorldIndex)
		return FALSE;
	SubWorld[Object[nObjIndex].m_nSubWorldID].Map2Mps(
		Object[nObjIndex].m_nRegionIdx,
		Object[nObjIndex].m_nMapX,
		Object[nObjIndex].m_nMapY,
		Object[nObjIndex].m_nOffX,
		Object[nObjIndex].m_nOffY,
		&nObjX,
		&nObjY);
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nNpcX,
		&nNpcY);
		
	//cho phep nhat item tu xa by kinnox;
	// if ( PLAYER_PICKUP_SERVER_DISTANCE < (nNpcX - nObjX) * (nNpcX - nObjX) + (nNpcY - nObjY) * (nNpcY - nObjY))
	// {
		// SHOW_MSG_SYNC	sMsg;
		// sMsg.ProtocolType = s2c_msgshow;
		// sMsg.m_wMsgID = enumMSG_ID_OBJ_TOO_FAR;
		// sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		// g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		// return FALSE;
	// }
	
	switch (Object[nObjIndex].m_nKind)
	{
	case Obj_Kind_Item:				
		{	
			const int nGameIdx = Object[nObjIndex].m_nItemDataID;
			if( (nGameIdx <= 0) || (nGameIdx >= MAX_ITEM) )
			{	
				Object[nObjIndex].m_nItemDataID = 0;
				Object[nObjIndex].m_nItemWidth = 0;
				Object[nObjIndex].m_nItemHeight = 0;
				Object[nObjIndex].Remove(FALSE);
				return FALSE;
			}
			
			int nListIdx = 0;
			if( !Item[nGameIdx].CanStack() || (pPickUp->m_btPosType == pos_hand) )
				nListIdx = m_ItemList.Add(nGameIdx, pPickUp->m_btPosType, pPickUp->m_btPosX, pPickUp->m_btPosY);
			else
				nListIdx = m_ItemList.Add_AutoStack(room_equipment, nGameIdx, pPickUp->m_btPosX, pPickUp->m_btPosY);

			if( (nListIdx <= 0) || (nListIdx >= MAX_PLAYER_ITEM) )
				return FALSE;

			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_GET_ITEM;
			sMsg.m_lpBuf = (LPVOID)Item[nGameIdx].GetID();
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_lpBuf = 0;
			if (Object[nObjIndex].GetObjPickExecute()) //executeitem by kinnox;
			{

				m_ItemList.EatMecidine(m_ItemList.m_Items[nListIdx].nIdx); 
				m_ItemList.RemoveByIndex(m_ItemList.m_Items[nListIdx].nIdx,0);
			}

			//			
			Object[nObjIndex].m_nItemDataID = 0;
			Object[nObjIndex].m_nItemWidth = 0;
			Object[nObjIndex].m_nItemHeight = 0;
			Object[nObjIndex].m_bPickExecute = FALSE;//executeitem by kinnox;
			Object[nObjIndex].Remove(FALSE);
			//
			if(Item[nGameIdx].GetGenre() != item_medicine)
			{
				char szData[128];
				//
				Item[nGameIdx].GetItemBackupInfo(szData);
				//
//				SaveLog(2, szData, "PICKUP ITEM", Item[nGameIdx].GetName());
			}
			g_pMainScript = (KLuaScript*)g_GetScript(Item[nGameIdx].GetScriptID());
			if(g_pMainScript)
			{	
				int nTopIndex = 0;
				g_pMainScript->SafeCallBegin(&nTopIndex);
				g_pMainScript->GetGlobalName("PickUp");
				nTopIndex = Lua_GetTopIndex(g_pMainScript->m_LuaState);
				if(Lua_GetValueType(g_pMainScript->m_LuaState, nTopIndex) == 5)
				{
					if(g_pMainScript->CallFunction("PickUp", 1, "dd", nGameIdx,this->m_nPlayerIndex) == 1)
					{
						Player[this->m_nPlayerIndex].m_ItemList.Remove(nGameIdx);
					}
					else
					{
						g_pMainScript->SafeCallEnd(nTopIndex);
						return FALSE;
					}
				}
				g_pMainScript->SafeCallEnd(nTopIndex);
			}		
		}
		break;
	case Obj_Kind_Money:
		if ( !Earn(Object[nObjIndex].m_nMoneyNum) )
			return FALSE;
		Object[nObjIndex].SyncRemove(TRUE);
		if (Object[nObjIndex].m_nRegionIdx >= 0)
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
		ObjSet.Remove(nObjIndex);
		break;
	}
	
	return TRUE;
}
#endif

#ifdef _SERVER

//

void	KPlayer::EatItem(BYTE* pProtocol)
{
	PLAYER_EAT_ITEM_COMMAND	*pEat = (PLAYER_EAT_ITEM_COMMAND*)pProtocol;
	
	m_ItemList.EatMecidine(pEat->m_btPlace, pEat->m_btX, pEat->m_btY);
}
#endif

#ifdef _SERVER

//

//void	KPlayer::UseItem(BYTE* pProtocol)
//{
//	PLAYER_USE_ITEM_COMMAND	*pItem = (PLAYER_USE_ITEM_COMMAND*)pProtocol;
//}
#endif

#ifdef _SERVER

//	

void	KPlayer::ServerMoveItem(BYTE* pProtocol)
{	
	if( m_cShop.GetState() )
		return;

	PLAYER_MOVE_ITEM_COMMAND	*pMove = (PLAYER_MOVE_ITEM_COMMAND*)pProtocol;
	ItemPos		DownPos, UpPos;
	
	DownPos.nPlace = pMove->m_btDownPos;
	DownPos.nX = pMove->m_btDownX;
	DownPos.nY = pMove->m_btDownY;
	UpPos.nPlace = pMove->m_btUpPos;
	UpPos.nX = pMove->m_btUpX;
	UpPos.nY = pMove->m_btUpY;
	//
	if(DownPos.nPlace == pos_traderoom)
	{	
		if( !CheckTrading() )
			return;

		if (this->m_cTrade.m_bTradeLock)
			return;

		const int nHandIdx = m_ItemList.Hand();
		if( (nHandIdx > 0) && (nHandIdx < MAX_ITEM) )
		{	
			if(Item[nHandIdx].GetBindState() > 0)
				return;

			if( (Item[nHandIdx].GetGenre() == item_task) )
			{
				if(Item[nHandIdx].GetParticular() == 0)
					return;
			}
		}

	}
	//
	if( DownPos.nPlace == pos_immediacy )
	{	
		if( CheckTrading() )
			return;
	
		const int nHandIdx = m_ItemList.Hand();
		if( (nHandIdx > 0) && (nHandIdx < MAX_ITEM) )
		{	
			if(Item[nHandIdx].GetWidth() != 1 ||
				Item[nHandIdx].GetHeight() != 1)
				return;

			if(	(Item[nHandIdx].GetGenre() != item_medicine) &&
				(Item[nHandIdx].GetGenre() != item_script) )
				return;
			
			if(Item[nHandIdx].GetGenre() == item_script)
			{
				if(Item[nHandIdx].GetMaxStackCount() > 0)
					return;
			}
			if(m_ItemList.CheckSameDetailType(room_immediacy, Item[nHandIdx].GetGenre(), Item[nHandIdx].GetDetailType()))
				return;
		}
	}
	if(DownPos.nPlace == pos_equip)
	{	
		if( CheckTrading() )
			return;
	}
	if(DownPos.nPlace >= pos_repositoryroom && DownPos.nPlace <= pos_expandtoryroom1)//Expandbox by kinnox;
	{
		if( CheckTrading() )
			return;
	}
	m_ItemList.ExchangeItem(&DownPos, &UpPos);
	
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &byFinished, sizeof(BYTE));
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ServerThrowAwayItem(BYTE* pProtocol)
{	
	PLAYER_THROW_AWAY_ITEM_COMMAND	*pThrow = (PLAYER_THROW_AWAY_ITEM_COMMAND*)pProtocol;
	KMapPos		sMapPos;
	

	/*
	int nIdx = ItemSet.SearchID(pThrow->dwIDItem);
	int nItemIdx = 0;
	
	if (pThrow->bAutoPick == FALSE)
	{ 
		if (!this->m_ItemList.Hand())
			return;

		nItemIdx = m_ItemList.m_Hand;
	
		if (nIdx != nItemIdx)
			return; 
	}
	else 
	{
		nItemIdx = nIdx;
		if (!m_ItemList.FindSame(nIdx))
			return;
	}
	*/
	//if (!this->m_ItemList.Hand())
	//	return;
	//nItemIdx = m_ItemList.m_Hand;

	if (!this->m_ItemList.Hand())
	return;

	int nItemIdx = 0;
	nItemIdx = m_ItemList.m_Hand;



	
	if(Item[nItemIdx].GetBindState() > 0)
		return;
	if(Item[nItemIdx].GetGoldId() > 0)
		return;
	

	GetAboutPos(&sMapPos);
	if (m_ItemList.Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_dwItemID = Item[nItemIdx].GetID();
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		//
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

		if(Item[nItemIdx].GetGenre() != item_medicine)
		{
			char szData[128];
			//
			Item[nItemIdx].GetItemBackupInfo(szData);
			//
//			SaveLog(2, szData, "THROWN ITEM", Item[nItemIdx].GetName());
		}

		//
		sInfo.m_nColorID = 0;
		//AutoAI by kinnox;
		sInfo.m_nGenre = Item[nItemIdx].GetGenre();
		sInfo.m_nDetailType = Item[nItemIdx].GetDetailType();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_dwNpcId = Npc[m_nIndex].m_dwID;
		//end code;
		nObj = ObjSet.Add(Item[nItemIdx].GetObjID(), sMapPos, sInfo);
		if (nObj == -1)
		{
			ItemSet.Remove(nItemIdx);
		}
		else
		{
			if (Item[nItemIdx].GetGenre() == item_task ||
				Item[nItemIdx].GetGenre() == item_script)
			{
				Object[nObj].SetEntireBelong(this->m_nPlayerIndex);
			}
			else
			{
				Object[nObj].SetItemBelong(-1);
			}
		}
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatSetTakeChannel(BYTE* pProtocol)
{
	CHAT_SET_CHANNEL_COMMAND	*pChannel = (CHAT_SET_CHANNEL_COMMAND*)pProtocol;
	this->m_cChat.SetTakeChannel(pChannel->m_dwTakeChannel);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatTransmitApplyAddFriend(BYTE* pProtocol)
{
	CHAT_APPLY_ADD_FRIEND_COMMAND	*pAdd = (CHAT_APPLY_ADD_FRIEND_COMMAND*)pProtocol;
	int	nPlayerIdx = FindAroundPlayer(pAdd->m_dwTargetNpcID);
	if (nPlayerIdx < 0)
		return;
	
	m_cChat.m_nAddFriendList[m_cChat.m_nListPos++] = nPlayerIdx;
	m_cChat.m_nListPos %= CHAT_APPLY_ADD_FRIEND_LIST;
	
	CHAT_APPLY_ADD_FRIEND_SYNC	sAdd;
	sAdd.ProtocolType = s2c_chatapplyaddfriend;
	sAdd.m_nSrcPlayerIdx = m_nPlayerIndex;
	strcpy(sAdd.m_szSourceName, Npc[m_nIndex].Name);
	memset(sAdd.m_szInfo, 0, sizeof(sAdd.m_szInfo));
	memcpy(sAdd.m_szInfo, pAdd->m_szInfo, pAdd->m_wLength + 1 + sizeof(pAdd->m_szInfo) - sizeof(CHAT_APPLY_ADD_FRIEND_COMMAND));
	//Kenny Fix Name
	/*CHAT_APPLY_ADD_FRIEND_SYNC sAdd;
	sAdd.ProtocolType   = s2c_chatapplyaddfriend;
	sAdd.m_nSrcPlayerIdx = m_nPlayerIndex;

	_snprintf(sAdd.m_szSourceName, sizeof(sAdd.m_szSourceName), "%s", Npc[m_nIndex].Name);
	sAdd.m_szSourceName[sizeof(sAdd.m_szSourceName)-1] = '\0';

	memset(sAdd.m_szInfo, 0, sizeof(sAdd.m_szInfo));
	// Sao ch�p an to�n t? buffer client (KH�NG tin m_wLength)
	int copyLen = (int)sizeof(pAdd->m_szInfo);
	if (copyLen > (int)sizeof(sAdd.m_szInfo) - 1) copyLen = (int)sizeof(sAdd.m_szInfo) - 1;
	memcpy(sAdd.m_szInfo, pAdd->m_szInfo, copyLen);
	sAdd.m_szInfo[copyLen] = '\0';*/
	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sAdd, sizeof(CHAT_APPLY_ADD_FRIEND_SYNC));
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::ChatAddFriend(BYTE* pProtocol)
{
	CHAT_ADD_FRIEND_COMMAND	*pAdd = (CHAT_ADD_FRIEND_COMMAND*)pProtocol;
	
	if (pAdd->m_nTargetPlayerIdx < 0 || pAdd->m_nTargetPlayerIdx >= MAX_PLAYER)
		goto AddFalse;
	if ( !Player[pAdd->m_nTargetPlayerIdx].m_nIndex )
		goto AddFalse;
	int		i;
	for (i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pAdd->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
			break;
	}
	if (i >= CHAT_APPLY_ADD_FRIEND_LIST)
		goto AddFalse;
	
	m_cChat.AddFriendData(m_nPlayerIndex, pAdd->m_nTargetPlayerIdx);
	Player[pAdd->m_nTargetPlayerIdx].m_cChat.AddFriendData(pAdd->m_nTargetPlayerIdx, m_nPlayerIndex);
	
	return TRUE;
	
AddFalse:
	CHAT_ADD_FRIEND_FAIL_SYNC	sFail;
	sFail.ProtocolType = s2c_chataddfriendfail;
	sFail.m_nTargetPlayerIdx = pAdd->m_nTargetPlayerIdx;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFail, sizeof(CHAT_ADD_FRIEND_FAIL_SYNC));
	
	return FALSE;
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatRefuseFriend(BYTE* pProtocol)
{
	CHAT_REFUSE_FRIEND_COMMAND	*pRefuse = (CHAT_REFUSE_FRIEND_COMMAND*)pProtocol;
	
	if (pRefuse->m_nTargetPlayerIdx < 0 || pRefuse->m_nTargetPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[pRefuse->m_nTargetPlayerIdx].m_nIndex == 0)
		return;
	for (int i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pRefuse->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
		{
			CHAT_REFUSE_FRIEND_SYNC	sRefuse;
			sRefuse.ProtocolType = s2c_chatrefusefriend;
			g_StrCpy(sRefuse.m_szName, Npc[m_nIndex].Name);
			sRefuse.m_wLength = sizeof(CHAT_REFUSE_FRIEND_SYNC) - 1 - sizeof(sRefuse.m_szName) + strlen(sRefuse.m_szName);
			
			g_pServer->PackDataToClient(Player[pRefuse->m_nTargetPlayerIdx].m_nNetConnectIdx, (BYTE*)&sRefuse, sRefuse.m_wLength + 1);
			return;
		}
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatResendAllFriend(BYTE* pProtocol)
{
	if ( m_cChat.m_nSyncState != 1)
		return;
	m_cChat.m_nSyncState = 2;
	m_cChat.m_pStateSendPos = (CChatFriend*)m_cChat.m_cFriendTeam[0].m_cEveryOne.GetHead();
	if ( !m_cChat.m_pStateSendPos )
		m_cChat.m_nSyncState = 0;
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatSendOneFriendData(BYTE* pProtocol)
{
	CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	*pApply = (CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND*)pProtocol;
	
	m_cChat.ResendOneFriendData(pApply->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatDeleteFriend(BYTE* pProtocol)
{
	CHAT_DELETE_FRIEND_COMMAND	*pDelete = (CHAT_DELETE_FRIEND_COMMAND*)pProtocol;
	
	m_cChat.DeleteFriend(pDelete->m_dwID, m_dwID);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ChatRedeleteFriend(BYTE* pProtocol)
{
	CHAT_REDELETE_FRIEND_COMMAND	*pDelete = (CHAT_REDELETE_FRIEND_COMMAND*)pProtocol;
	
	this->m_cChat.RedeleteFriend(pDelete->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::TradeApplyOpen(BYTE* pProtocol)
{	
	if( m_cTrade.GetForbitTrade() )
		return;
	if ( CheckTrading() )
		return;
	if ( m_cShop.GetState() )
		return;

	TRADE_APPLY_OPEN_COMMAND* pApply = (TRADE_APPLY_OPEN_COMMAND*)pProtocol;
	if(pApply->m_bReserve != 31)
		return;
	
	switch (m_cMenuState.m_nState)
	{
		case PLAYER_MENU_STATE_NORMAL:
		case PLAYER_MENU_STATE_TRADEOPEN:
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN);
			break;
		case PLAYER_MENU_STATE_TEAMOPEN:
			if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
				g_Team[m_cTeam.m_nID].SetTeamClose();
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN);
			break;
		default:
			break;
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::TradeApplyClose(BYTE* pProtocol)
{	
	
	if(this->CheckTrading())
		return;
	
	m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
}
#endif

#ifdef _SERVER

//	

void KPlayer::TradeApplyStart(BYTE* pProtocol)
{	
	if( m_cTrade.GetForbitTrade() )
		return;
	if ( m_cShop.GetState() )
		return;
	if ( CheckTrading() )
		return;

	TRADE_APPLY_START_COMMAND	*pStart = (TRADE_APPLY_START_COMMAND*)pProtocol;
	
	int		nPlayer;
	nPlayer = FindAroundPlayer(pStart->m_dwID);
	if (nPlayer < 0)
		return;
	
	if( Player[nPlayer].m_cTrade.GetForbitTrade() )
		return;
	if (Player[nPlayer].m_cShop.GetState())
		return;
	if (Player[nPlayer].CheckTrading())
		return;

	if (Player[nPlayer].m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
		return;

	this->m_cTrade.m_nApplyIdx = nPlayer;
	
	TRADE_APPLY_START_SYNC	sApply;
	sApply.ProtocolType = s2c_tradeapplystart;
	sApply.m_nDestIdx = this->m_nPlayerIndex;
	sApply.m_dwNpcId = Npc[m_nIndex].m_dwID;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sApply, sizeof(TRADE_APPLY_START_SYNC));
}
#endif

#ifdef _SERVER

//	

void KPlayer::TradeMoveMoney(BYTE* pProtocol)
{	
	//
	if( !CheckTrading() )
		return;
	if ( m_cTrade.m_bTradeLock )
		return;
	
	
	int		nMoney;
	TRADE_MOVE_MONEY_COMMAND	*pMoney = (TRADE_MOVE_MONEY_COMMAND*)pProtocol;
	if (pMoney->m_nMoney < 0 || pMoney->m_nMoney > m_ItemList.GetEquipmentMoney() + m_ItemList.GetTradeMoney()) //fix trade money by kinnox;
		nMoney = 0;
	else
		nMoney = pMoney->m_nMoney;
	m_ItemList.TradeMoveMoney(nMoney);
}
#endif

#ifdef _SERVER

//	

void	KPlayer::TradeDecision(BYTE* pProtocol)
{	
	
	if( m_cShop.GetState() )
		return;
	if ( !CheckTrading() )
		return;
	if( Player[m_cTrade.m_nTradeDest].m_cShop.GetState() )
		return;
	if( !Player[m_cTrade.m_nTradeDest].CheckTrading() )
		return;
	
	
	TRADE_DECISION_COMMAND	*pDecision = (TRADE_DECISION_COMMAND*)pProtocol;
	

	if (pDecision->m_btDecision == 2)
	{	
		//
		if (this->m_cTrade.m_bTradeLock == FALSE)
		{	
			this->m_cTrade.m_bTradeLock = TRUE;
			this->m_cTrade.m_bTradeState = FALSE;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeState = FALSE;
		}
		
		
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 3)
	{	
		//
		if (this->m_cTrade.m_bTradeLock == TRUE)
		{	
			this->m_cTrade.m_bTradeLock = FALSE;
			this->m_cTrade.m_bTradeState = FALSE;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeLock = FALSE;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeState = FALSE;
		}
		
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 4)
	{	
		//
		if (m_cTrade.m_bTradeLock == TRUE && Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeLock == TRUE)
		{
			m_cTrade.m_bTradeState = FALSE;
			SyncTradeState();
		}
		
		return;
	}
	else if (pDecision->m_btDecision == 0)
	{
		int	nDestIdx;
		nDestIdx = this->m_cTrade.m_nTradeDest;
		this->m_ItemList.RecoverTrade();
		this->m_ItemList.ClearRoom(room_trade);
		this->m_ItemList.ClearRoom(room_trade1);
		this->m_cTrade.Release();
		Player[nDestIdx].m_ItemList.RecoverTrade();
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade);
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade1);
		Player[nDestIdx].m_cTrade.Release();
		
		TRADE_DECISION_SYNC	sSync;
		sSync.ProtocolType = s2c_tradedecision;
		sSync.m_btDecision = 0;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		
		this->m_cMenuState.RestoreBackupState(m_nPlayerIndex);
		
		Player[nDestIdx].m_cMenuState.RestoreBackupState(nDestIdx);
		
		return;
	}
	else if (pDecision->m_btDecision == 1)
	{	
		//
		if (this->m_cTrade.m_bTradeLock != TRUE ||
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeLock != TRUE)
		{
			SyncTradeState();
			return;
		}
		else if (Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeState == FALSE)
		{
			m_cTrade.m_bTradeState = TRUE;
			
			SyncTradeState();
			
			return;
		}
		
		else
		{
			int	nSM1, nSM2, nDM1, nDM2;
			int	nDestIdx = m_cTrade.m_nTradeDest;
			nSM1 = m_ItemList.GetMoney(room_equipment);
			nSM2 = m_ItemList.GetMoney(room_trade);
			nDM1 = Player[nDestIdx].m_ItemList.GetMoney(room_equipment);
			nDM2 = Player[nDestIdx].m_ItemList.GetMoney(room_trade);
			if (nSM2 < 0 || nSM1 + nDM2 - nSM2 < 0)
			{
				this->m_ItemList.TradeMoveMoney(0);
				return;
			}
			if (nDM2 < 0 || nDM1 + nSM2 - nDM2 < 0)
			{
				Player[nDestIdx].m_ItemList.TradeMoveMoney(0);
				return;
			}

			int		nSelfCanGet, nDestCanGet;
			
			Player[nDestIdx].m_ItemList.GetTradeRoomItemInfo();
			nSelfCanGet = m_ItemList.TradeCheckCanPlace();
			ItemSet.BackItemInfo();
			
			this->m_ItemList.GetTradeRoomItemInfo();
			nDestCanGet = Player[nDestIdx].m_ItemList.TradeCheckCanPlace();
			
			if (!nSelfCanGet || !nDestCanGet)
			{
				if (!nSelfCanGet)
				{
					this->m_cTrade.m_bTradeState = FALSE;
					
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				if (!nDestCanGet)
				{
					Player[nDestIdx].m_cTrade.m_bTradeState = FALSE;
					
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				SyncTradeState();
				return;
			}
	
			TRADE_DECISION_SYNC	sSync;
			sSync.ProtocolType = s2c_tradedecision;
			sSync.m_btDecision = 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
			Player[nDestIdx].m_cMenuState.SetState(nDestIdx, PLAYER_MENU_STATE_NORMAL);
			this->m_cTrade.Release();
			Player[nDestIdx].m_cTrade.Release();
			
			if (nSM2 <= nDM2)
			{
				this->Earn(nDM2 - nSM2);
				Player[nDestIdx].Pay(nDM2 - nSM2);
			}
			else
			{
				this->Pay(nSM2 - nDM2);
				Player[nDestIdx].Earn(nSM2 - nDM2);
			}
			
			int		nPos;
			nPos = 0;
			while (1)
			{	
				//
				if (nPos >= MAX_TRADE_ITEM)
					break;
				
				if (!ItemSet.m_psBackItemInfo[nPos].m_nIdx)
					break;

				Player[nDestIdx].m_ItemList.Remove(ItemSet.m_psBackItemInfo[nPos].m_nIdx);

				m_ItemList.Add(
					ItemSet.m_psBackItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psBackItemInfo[nPos].m_nX,
					ItemSet.m_psBackItemInfo[nPos].m_nY);
				nPos++;
			}

			nPos = 0;
			while (1)
			{	
				//
				if (nPos >= MAX_TRADE_ITEM)
					break;
				

				if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
					break;

				this->m_ItemList.Remove(ItemSet.m_psItemInfo[nPos].m_nIdx);

				Player[nDestIdx].m_ItemList.Add(
					ItemSet.m_psItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psItemInfo[nPos].m_nX,
					ItemSet.m_psItemInfo[nPos].m_nY);
				nPos++;
			}
			
			this->m_uMustSave = SAVE_REQUEST;
			Save();
			Player[nDestIdx].m_uMustSave = SAVE_REQUEST;
			Player[nDestIdx].Save();
			
			return;
		}
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::c2sTradeReplyStart(BYTE* pProtocol)
{	
	if( m_cTrade.GetForbitTrade() )
		return;
	if (this->m_cShop.GetState())
		return;

	if (this->CheckTrading())
		return;

	if (this->m_bLockPlayer)//LockPlayer by kinnox;
		return;
		
	if (!pProtocol)
		return;

	TRADE_REPLY_START_COMMAND *pReply = (TRADE_REPLY_START_COMMAND*)pProtocol;
	
	int	nPlayerIdx = FindAroundPlayer(pReply->m_dwNpcID);		//
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[nPlayerIdx].m_nIndex <= 0)
		return;

	if(Player[nPlayerIdx].CheckTrading())
		return;
		
	if(Player[nPlayerIdx].m_bLockPlayer)//LockPlayer by kinnox;
		return;
		
	if(Player[nPlayerIdx].m_cShop.GetState())
		return;

	if (Player[nPlayerIdx].m_cTrade.m_nApplyIdx != this->m_nPlayerIndex)
		return;
	
	if (pReply->m_bDecision == 0)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TRADE_REFUSE_APPLY;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	
	if (this->m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
	{
		return;
	}
	
	if (Player[nPlayerIdx].m_cMenuState.m_nState == PLAYER_MENU_STATE_TEAMOPEN)
	{
		PLAYER_TEAM_OPEN_CLOSE	sClose;
		sClose.m_btOpenClose = 0;
		Player[nPlayerIdx].SetTeamState((BYTE*)&sClose);
	}

	if (Npc[m_nIndex].m_Doing == do_sit)
	{
		Npc[m_nIndex].SendCommand(do_stand);
	}
	if (Npc[Player[nPlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[nPlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	
	this->m_ItemList.StartTrade();
	this->m_cTrade.Release();
	this->m_cTrade.StartTrade(nPlayerIdx);
	this->m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[Player[nPlayerIdx].m_nIndex].m_dwID);
	
	Player[nPlayerIdx].m_ItemList.StartTrade();
	Player[nPlayerIdx].m_cTrade.Release();
	Player[nPlayerIdx].m_cTrade.StartTrade(m_nPlayerIndex);
	Player[nPlayerIdx].m_cMenuState.SetState(nPlayerIdx, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[m_nIndex].m_dwID);
}
#endif


//	

#ifdef _SERVER
void	KPlayer::SyncTradeState()
{
	if (!CheckTrading())
		return;
	
	//
	TRADE_STATE_SYNC	sSync;
	sSync.ProtocolType = s2c_tradepressoksync;
	sSync.m_btSelfLock = this->m_cTrade.m_bTradeLock;
	sSync.m_btDestLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeLock;
	sSync.m_btSelfOk = m_cTrade.m_bTradeState;
	sSync.m_btDestOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeState;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
	
	sSync.m_btSelfLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeLock;
	sSync.m_btDestLock = m_cTrade.m_bTradeLock;
	sSync.m_btSelfOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_bTradeState;
	sSync.m_btDestOk = m_cTrade.m_bTradeState;
	g_pServer->PackDataToClient(Player[m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
	
}
#endif


//	

#ifdef _SERVER
void	KPlayer::SendEquipItemInfo(int nTargetPlayer)
{
	if (nTargetPlayer <= 0 || nTargetPlayer >= MAX_PLAYER || Player[nTargetPlayer].m_nIndex <= 0)
		return;
	
	int				nIdx, i, j;
	SViewItemInfo	*pInfo;
	VIEW_EQUIP_SYNC	sView;
	
	sView.ProtocolType = s2c_viewequip;
	sView.m_dwNpcID = Npc[m_nIndex].m_dwID;
	sView.m_bAvatar = m_ImagePlayer; //Player Avatar by kinnox;
	for (i = 0; i < itempart_num; i++)
	{
		nIdx = m_ItemList.m_EquipItem[i];
		pInfo = &sView.m_sInfo[i];
		if (nIdx <= 0)
		{
			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		pInfo->m_ID				= Item[nIdx].GetID();
		pInfo->m_Genre			= Item[nIdx].GetGenre();
		pInfo->m_Detail			= Item[nIdx].GetDetailType();
		pInfo->m_Series			= Item[nIdx].GetSeries();
		pInfo->m_Level			= Item[nIdx].GetLevel();
		pInfo->m_Luck			= Item[nIdx].m_GeneratorParam.nLuck;
		pInfo->m_RandomSeed		= Item[nIdx].m_GeneratorParam.dwRandomSeed;
		pInfo->m_Version		= Item[nIdx].m_GeneratorParam.nVersion;
		for (j = 0; j < 6; j++)
			pInfo->m_MagicLevel[j] = (BYTE)Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];
		pInfo->m_Record			= Item[nIdx].GetRecord();
	}
	
	g_pServer->PackDataToClient(Player[nTargetPlayer].m_nNetConnectIdx, (BYTE*)&sView, sizeof(sView));
}
#endif

#ifdef _SERVER

//	

void KPlayer::UseTownPortal()
{	
	//
	if (m_nPlayerIndex <= 0 || m_nPlayerIndex >= MAX_PLAYER)
		return;

	if(m_bForbidTownPortal)
	{	
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Nh�c nh�", MSG_FORBID_TOWN_PORTAL, strlen(MSG_FORBID_TOWN_PORTAL));
		return;
	}

	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;

	if (Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD)
		return;
	
	//fix disconect client;
	

	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
	{
		return;
	}
	
	if (!Npc[m_nIndex].m_FightMode)
	{
		return;
	}
	
	Npc[m_nIndex].SendCommand(do_stand);
	
	m_sPortalPos.m_nSubWorldId = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	Npc[m_nIndex].GetMpsPos(&m_sPortalPos.m_nMpsX, &m_sPortalPos.m_nMpsY);
	m_sPortalPos.m_nTime = TOWN_PORTAL_TIME;
	
	Npc[m_nIndex].ChangeWorld(m_sDeathRevivalPos.m_nSubWorldID, m_sDeathRevivalPos.m_nMpsX, m_sDeathRevivalPos.m_nMpsY);
	Npc[m_nIndex].SetFightMode(0);
}


//	

void KPlayer::BackToTownPortal(int nIdSubWorld)
{
	//
	if (m_nPlayerIndex <= 0 || m_nPlayerIndex >= MAX_PLAYER)
		return;

	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;
	
	if (CheckTrading())
		return;
	
	if (this->m_cShop.GetState())
		return;
	if (nIdSubWorld <= 0)
	{
		if (m_sPortalPos.m_nTime <= 0)
		return;

		if (Npc[m_nIndex].ChangeWorld(m_sPortalPos.m_nSubWorldId, m_sPortalPos.m_nMpsX, m_sPortalPos.m_nMpsY))
		{//fix disconect GS khi player lam cung mot luc;
			m_sPortalPos.m_nSubWorldId = 0;
			m_sPortalPos.m_nTime = 0;
			m_sPortalPos.m_nMpsX = 0;
			m_sPortalPos.m_nMpsY = 0;
			Npc[m_nIndex].SetFightMode(TRUE);
		}
	}
	else
	{
		if (m_sPortalPos.m_nSubWorldId > 0 || m_sPortalPos.m_nTime > 0 )
		{ //fix disconect GS khi player lam cung mot luc;
			return;
		}
		else
		{
			int nX,nY;
			if (nIdSubWorld == 75)
			{
				nX = 53504;
				nY = 107040;
			}
			else if (nIdSubWorld == 224)
			{	
				nX = 54144;
				nY = 100800;
			}
			else if (nIdSubWorld == 320)
			{	
				nX = 36704;
				nY = 100320;
			}
			else if (nIdSubWorld == 225)
			{	
				nX = 50784;
				nY = 102368;
			}
			else if (nIdSubWorld == 226)
			{	
				nX = 54560;
				nY = 104000;
			}
			else if (nIdSubWorld == 227)
			{	
				nX = 50976;
				nY = 103296;
			}
			else if (nIdSubWorld == 322)
			{	
				nX = 50848;
				nY = 101184;
			}
			else if (nIdSubWorld == 321)
			{	
				nX = 30912;
				nY = 73888;
			}
			else if (nIdSubWorld == 336)
			{	
				nX = 35872;
				nY = 101984;
			}
			else if (nIdSubWorld == 340)
			{	
				nX = 59328;
				nY = 109760;
			}								
			Npc[m_nIndex].ChangeWorld(nIdSubWorld, nX, nY);
			m_sPortalPos.m_nSubWorldId = 0;
			m_sPortalPos.m_nTime = 0;
			m_sPortalPos.m_nMpsX = 0;
			m_sPortalPos.m_nMpsY = 0;
			Npc[m_nIndex].SetFightMode(TRUE);
		}
	}
}

void KPlayer::ClearPortal()
{
	if (m_nPlayerIndex <= 0 || m_nPlayerIndex >= MAX_PLAYER)
		return;

	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
}

#endif

#ifdef _SERVER

//	

void KPlayer::Revive(int nType)
{
	int	nSubWorldID = 0;
	int nMpsX = 0, nMpsY = 0;

	NPC_REVIVE_SYNC	Sync;
	Sync.ProtocolType = s2c_playerrevive;
	Sync.ID = Npc[m_nIndex].m_dwID;
	Sync.Type = nType;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (const void*)&Sync, sizeof(NPC_REVIVE_SYNC));
	Npc[m_nIndex].BroadCastRevive(nType);

	switch(nType)
	{
	case REMOTE_REVIVE_TYPE:
		GetDeathRevivalPos(&nSubWorldID, &nMpsX, &nMpsY);
		Npc[m_nIndex].ChangeWorld(nSubWorldID, nMpsX, nMpsY);
		Npc[m_nIndex].m_FightMode = 0;
		break;
	case LOCAL_REVIVE_TYPE:
		Npc[m_nIndex].m_FightMode = 1;
		break;
	default:
		break;
	}
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;	
	Npc[m_nIndex].SendCommand(do_revive);

	ExecuteScript(PLAYER_REVIVE_SCRIPT_FILE, "main", 0);
	
}


//	

void KPlayer::RestoreLiveData()
{
	Npc[m_nIndex].RestoreNpcBaseInfo();
}


//	

BOOL KPlayer::Pay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;
	
	return m_ItemList.CostMoney(nMoney);
}


//	

BOOL KPlayer::PrePay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (Pay(nMoney))
	{
		m_nPrePayMoney = nMoney;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//	

BOOL KPlayer::Earn(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	return m_ItemList.AddMoney(room_equipment, nMoney);
}


//	

void	KPlayer::LaunchPlayer()
{
	int nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int nRegion = Npc[m_nIndex].m_RegionIndex;
	int nX = Npc[m_nIndex].m_MapX;
	int nY = Npc[m_nIndex].m_MapY;
	int nOffX = Npc[m_nIndex].m_OffX;
	int nOffY = Npc[m_nIndex].m_OffY;
	SubWorld[nSubWorld].AddPlayer(nRegion, m_nPlayerIndex);
	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	if (SubWorld[nSubWorld].m_Region[nRegion].GetBarrierMin(nX, nY, nOffX, nOffY, FALSE) != Obstacle_NULL)
	{
		Npc[m_nIndex].ChangeWorld(m_sLoginRevivalPos.m_nSubWorldID, m_sLoginRevivalPos.m_nMpsX, m_sLoginRevivalPos.m_nMpsY);
	}
	

	// Auto assign default title if player doesn't have one
	if(m_wTitleId == 0)
	{
		m_wTitleId = 1; // Default title: "Binh sĩ" (TitleId=1)
	}

	if(m_wTitleId > 0)
	{
		this->SetTitle(m_wTitleId, true);
	}
	
}
#endif

#ifdef _SERVER

//	

void KPlayer::StopMove()
{
	BYTE	sStopInfo = s2c_stopmove;
	if(g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE *)&sStopInfo, sizeof(BYTE));
}
#endif


//	

BOOL 	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) 
		return FALSE;
	return ExecuteScript(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}




BOOL 	KPlayer::ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam)
{	
	try
	{	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
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
			m_bWaitingPlayerFeedBack = false;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


	

BOOL	KPlayer::ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams)
{
	try
	{	
		bool bExecuteScriptMistake = false;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if ( (!szParams) || !szParams[0]) 
			{
				if(pScript->CallFunction(szFunName, 0, ""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if(pScript->CallFunction(szFunName, 0, "sd", szParams, 0))
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


//	

BOOL	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, char *  szParams)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScript(dwScriptId, szFunName, szParams);
}


BOOL	KPlayer::ExecuteScript2(char * ScriptFileName, char * szFunName, int nParam1, int nParam2, BOOL bSelectUI /*= TRUE*/)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript2(g_FileName2Id(ScriptFileName), szFunName, nParam1, nParam2);	
}


BOOL	KPlayer::ExecuteScript2(DWORD dwScriptId,  char* szFunName, int nParam1, int nParam2, BOOL bSelectUI /*= TRUE*/)
{	
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript* pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if(pScript)
		{	
			if(bSelectUI)
				Npc[m_nIndex].m_ActionScriptID = dwScriptId;

			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName,0, "dd", nParam1, nParam2)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if(bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			if(bSelectUI)
				Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		if(bSelectUI)
			Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


BOOL	KPlayer::ExecuteScript2(char* ScriptFileName, char* szFunName, int nParam, char* szParams, BOOL bSelectUI /*= TRUE*/)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript2(g_FileName2Id(ScriptFileName), szFunName, nParam, szParams);	
}

BOOL	KPlayer::ExecuteScript2(DWORD dwScriptId, char* szFunName, int nParam, char*  szParams, BOOL bSelectUI /*= TRUE*/)
{	
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if(pScript)
		{	
			if(bSelectUI)
				Npc[m_nIndex].m_ActionScriptID = dwScriptId;

			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName, 0, "ds", nParam, szParams)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			if(bSelectUI)
				Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		if(bSelectUI)
			Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


//	

void	KPlayer::DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo)
{
	if(!pUIInfo) 
		return;
	//
	if(pUIInfo->m_bParam2 > 0)
	{
#ifdef _SERVER
		pUIInfo->ProtocolType = (BYTE)s2c_scriptaction;
		pUIInfo->m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + pUIInfo->m_nBufferLen - 1;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)pUIInfo, pUIInfo->m_wProtocolLong + 1 );
#else
	}
	
	else
	{
		OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pUIInfo);
#endif
	}
	
}


//	

void	KPlayer::ProcessPlayerSelectFromUI(BYTE* pProtocol)	
{	
	PLAYER_SELECTUI_COMMAND * pSelUI = (PLAYER_SELECTUI_COMMAND*) pProtocol;
	
	if(!pSelUI)
		return;
	if(!pSelUI->m_btUIId)
		return;
	if(pSelUI->nSelectIndex < 0)
		return;
	
	m_bWaitingPlayerFeedBack = false;
	
	if (m_nAvailableAnswerNum > pSelUI->nSelectIndex)
	{
		if (m_szTaskAnswerFun[pSelUI->nSelectIndex][0])
		{
			if (m_nIndex)
			{	
				//
				char szParam[32];
				szParam[0] = 0;
				if(!pSelUI->m_szParam || !pSelUI->m_szParam[0])
				{
					char* pParam = NULL;
					pParam = strstr(m_szTaskAnswerFun[pSelUI->nSelectIndex], "#");
					if(pParam)
					{
						g_StrCpyLen(szParam, pParam + 1, sizeof(szParam));
						*pParam = 0;
					}
				}
				else
				{	
					sprintf(szParam, "%s", pSelUI->m_szParam);
				}
				
				if(szParam[0])
					ExecuteScript2(Npc[m_nIndex].m_ActionScriptID, m_szTaskAnswerFun[pSelUI->nSelectIndex], pSelUI->nSelectIndex, szParam);
				else
					ExecuteScript(Npc[m_nIndex].m_ActionScriptID, m_szTaskAnswerFun[pSelUI->nSelectIndex], pSelUI->nSelectIndex);
				
				
			}
		}
	}
}

#ifndef _SERVER

//	

void	KPlayer::OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo)			
{
	if (!pSelectUI) 
		return;
	
	
	pSelectUI->m_btUIId		= (BYTE)eUIInfo;
	

	switch(eUIInfo)
	{
	case UI_SELECTDIALOG:
		{	
			if (g_bUISelIntelActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				pSelectUI->dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
				pSelectUI->dwTimePacker = GetTickCount();
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);	
			}
		}
		break;
	case UI_TALKDIALOG:
		{
			if (g_bUISpeakActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				pSelectUI->dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
				pSelectUI->dwTimePacker = GetTickCount();
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);	
			}
			
		}break;
	
	case UI_ASKFORNUMBER:
		{	
			pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
			pSelectUI->dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			pSelectUI->dwTimePacker = GetTickCount();
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
		}
		break;
	case UI_ASKFORSTRING:
		{	
			pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
			pSelectUI->dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			pSelectUI->dwTimePacker = GetTickCount();
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
		}
		break;
	case UI_CHANGETAXDIALOG:
		{	
			pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
			pSelectUI->dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			pSelectUI->dwTimePacker = GetTickCount();
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
		}
		break;
	
	}
}
#endif


//	

void	KPlayer::ChatFriendOnLine(DWORD dwID, int nFriendIdx)
{
	if (nFriendIdx <= 0)
		return;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		if (m_cChat.m_cFriendTeam[i].m_nFriendNo == 0)
			continue;
		CChatFriend	*pFriend;
		pFriend = (CChatFriend*)m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == dwID)
			{
				pFriend->m_nPlayerIdx = nFriendIdx;
#ifdef _SERVER
				CHAT_FRIEND_ONLINE_SYNC	sFriend;
				sFriend.ProtocolType = s2c_chatfriendonline;
				sFriend.m_dwID = dwID;
				sFriend.m_nPlayerIdx = nFriendIdx;
				g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFriend, sizeof(CHAT_FRIEND_ONLINE_SYNC));
#else
				KUiPlayerItem	sPlayer;
				strcpy(sPlayer.Name, pFriend->m_szName);
				sPlayer.uId = dwID;
				sPlayer.nIndex = nFriendIdx;
				sPlayer.nData = CHAT_S_ONLINE;
				CoreDataChanged(GDCNI_CHAT_FRIEND_STATUS, (unsigned int)&sPlayer, i);
				
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
#endif
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}
#ifdef _SERVER
	Player[nFriendIdx].m_cChat.DeleteFriendData(m_dwID, nFriendIdx);
#endif
}


//	

void	KPlayer::GetFactionName(char *lpszName, int nSize)
{
	this->m_cFaction.GetCurFactionName(lpszName);
}

#ifdef _SERVER

//	

void	KPlayer::S2CExecuteScript(char * ScriptName, char * szParam = NULL)
{
	if (!ScriptName || (!ScriptName[0])) return; 
	
	PLAYER_SCRIPTACTION_SYNC  ScriptAction;
	ScriptAction.m_nOperateType = SCRIPTACTION_EXESCRIPT;
	ScriptAction.ProtocolType = s2c_scriptaction;
	char * script = NULL;
	if (szParam == NULL || szParam[0] == 0) 
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 1 ;
		strcpy(ScriptAction.m_pContent, ScriptName);
	}
	else
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 2 + strlen(szParam);
		sprintf(ScriptAction.m_pContent, "%s|%s", ScriptName, szParam);
	}
	ScriptAction.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen - 1;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&ScriptAction, sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen);	
	
}
#endif

#ifndef _SERVER

//	

void	KPlayer::OnScriptAction(PLAYER_SCRIPTACTION_SYNC * pMsg)
{
	PLAYER_SCRIPTACTION_SYNC * pScriptAction = (PLAYER_SCRIPTACTION_SYNC *)pMsg;
	char szString[1000];
	
	switch(pScriptAction->m_nOperateType)
	{
	case SCRIPTACTION_UISHOW:
		{
			switch(pScriptAction->m_bUIId)
			{
			case UI_SELECTDIALOG:
				{
					KUiQuestionAndAnswer	*pQuest = NULL;
					if (pScriptAction->m_nBufferLen <= 0) 
						break;

					
					BOOL bShowImage = FALSE;
					if(pScriptAction->m_bParam2 == 2)
					{
						bShowImage = TRUE;
					}
					
					if(!bShowImage)
					{
						if (pScriptAction->m_bOptionNum <= 0)
							pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer));
						else
							pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer) * (pScriptAction->m_bOptionNum - 1));
					}
					else
					{
						if (pScriptAction->m_bOptionNum <= 0)
							pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer));
						else
							pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer) * (pScriptAction->m_bOptionNum));
					}
					
					
					char strContent[1024];
					char * pAnswer = NULL;
					pQuest->AnswerCount = 0;

					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
						pAnswer = strstr(strContent, "|");
						if (!pAnswer)
						{
							pScriptAction->m_bOptionNum = 0;
							pQuest->AnswerCount = 0;
						}
						else
							*pAnswer++ = 0;
						
						g_StrCpyLen(pQuest->Question, strContent, sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
					}
					else 
					{
						g_StrCpyLen(pQuest->Question, g_GetStringRes(*(int *)pScriptAction->m_pContent, szString, 1000), sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
						
						g_StrCpyLen(strContent, pScriptAction->m_pContent + sizeof(int), pScriptAction->m_nBufferLen - sizeof(int) + 1);
						pAnswer = strContent + 1;
					}
					
					
					char * pNewAnswer = NULL;
					if(bShowImage)
					{
						pNewAnswer = strstr(pAnswer, "|");
						if(pNewAnswer)
						{
							*pNewAnswer = 0;
							if(pScriptAction->m_bOptionNum <= 0)
							{
								strcpy(pQuest->Answer[0].AnswerText, pAnswer);
								pQuest->Answer[0].AnswerLen = -1;
								pAnswer = pNewAnswer + 1;
							}
							else
							{
								strcpy(pQuest->Answer[pScriptAction->m_bOptionNum].AnswerText, pAnswer);
								pQuest->Answer[pScriptAction->m_bOptionNum].AnswerLen = -1;
								pAnswer = pNewAnswer + 1;
							}
						}
						else
						{
							if(pScriptAction->m_bOptionNum <= 0)
							{
								strcpy(pQuest->Answer[0].AnswerText, pAnswer);
								pQuest->Answer[0].AnswerLen = -1;
							}
							else
							{
								strcpy(pQuest->Answer[pScriptAction->m_bOptionNum].AnswerText, pAnswer);
								pQuest->Answer[pScriptAction->m_bOptionNum].AnswerLen = -1;
							}
						}
					}
					
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						pNewAnswer = strstr(pAnswer, "|");
						if (pNewAnswer)
						{	
							*pNewAnswer = 0;
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pQuest->AnswerCount = i + 1;
							break;
						}
					}
					
					g_bUISelIntelActiveWithServer = pScriptAction->m_bParam2;
					g_bUISelLastSelCount = pQuest->AnswerCount;
					CoreDataChanged(GDCNI_QUESTION_CHOOSE,(unsigned int) pQuest, bShowImage);

					free(pQuest);
					pQuest = NULL;

				}
				break;	
			case UI_TALKDIALOG: 
				{
					BOOL bUsingSpeakId = pScriptAction->m_bParam1;
					int	 nSentenceCount = pScriptAction->m_bOptionNum;
					if (nSentenceCount <= 0) return ;
					KUiInformationParam *pSpeakList  = new KUiInformationParam[nSentenceCount];
					memset(pSpeakList, 0, sizeof(KUiInformationParam) * nSentenceCount);
					
					char * pAnswer = new char [pScriptAction->m_nBufferLen + 1];
					char * pBackupAnswer = pAnswer;
					g_StrCpyLen(pAnswer, pScriptAction->m_pContent, pScriptAction->m_nBufferLen + 1);
					
					int nCount = 0;
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
								
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString, sizeof(szString)));
							}
							
							
							if (i < pScriptAction->m_bOptionNum - 1)
								strcpy(pSpeakList[i].sConfirmText, "����");
							else 
							{
								strcpy(pSpeakList[i].sConfirmText, "���");
								if (pScriptAction->m_nParam == 1)						
									pSpeakList[i].bNeedConfirmNotify = TRUE;
								
							}
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString ,sizeof(szString)));
							}
							
							strcpy(pSpeakList[i].sConfirmText, "���");
							
							if (pScriptAction->m_nParam == 1)						
								pSpeakList[i].bNeedConfirmNotify = TRUE;
							
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							nCount++;
							break;
						}
						nCount ++;
					}
					if (pBackupAnswer)
					{
						delete []pBackupAnswer;
						pBackupAnswer = NULL;
					}
					g_bUISpeakActiveWithServer = pScriptAction->m_bParam2;
					CoreDataChanged(GDCNI_SPEAK_WORDS,(unsigned int)pSpeakList, nCount);
					if (pSpeakList)
					{
						delete pSpeakList;	
						pSpeakList = NULL;
					}
				}
				break;
			case UI_NOTEINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					KMissionRecord Record;

					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(Record.sContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) - 1);
						Record.uValue = *(unsigned int*) (pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int));
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, Record.sContent ,sizeof(Record.sContent));
						Record.uValue = *(unsigned int* ) (Record.sContent + sizeof(int));
					}
					
					
					Record.nContentLen = TEncodeText(Record.sContent, strlen(Record.sContent));
					
					CoreDataChanged(GDCNI_MISSION_RECORD, (unsigned long)&Record, 0);
					
				}
				break;
			case UI_MSGINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					
					char strContent[1024];

					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
					}
					
					KSystemMessage	sMsg;
					sMsg.eType = SMT_PLAYER;
					sMsg.byConfirmType = SMCT_MSG_BOX;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					g_StrCpyLen(sMsg.szMessage, strContent, sizeof(sMsg.szMessage));
					
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);					
				}break;	
			case UI_NEWSINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					switch(pScriptAction->m_bOptionNum)
					{
					case NEWSMESSAGE_NORMAL:
						{
							
							KNewsMessage News;
							News.nType = NEWSMESSAGE_NORMAL;
							
							char strContent[1024];
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));

							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, 0);
						}
						break;
					case NEWSMESSAGE_COUNTING:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;

							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							
							memset(&systime, 0, sizeof(SYSTEMTIME));
							systime.wSecond = *(int *)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int)));
							
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;
					case NEWSMESSAGE_TIMEEND:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							systime = *(SYSTEMTIME*)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;
					
					}
					
				}break;
			case UI_NEWSINFOEX:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					switch(pScriptAction->m_bOptionNum) //OPtionNum??????News??
					{
					case NEWSMESSAGE_NORMAL:
						{
							
							KNewsMessage News;
							News.nType = NEWSMESSAGE_NORMAL;
							
							char strContent[1024];
							//???????
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));

							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_NEWS_MESSAGE_EX, (unsigned int)&News, 0);
						}
						break;
						//STRING|STRINGID + TIME(INT)
					case NEWSMESSAGE_COUNTING:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//???????
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							
							memset(&systime, 0, sizeof(SYSTEMTIME));
							systime.wSecond = *(int *)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int)));
							
							CoreDataChanged(GDCNI_NEWS_MESSAGE_EX, (unsigned int)&News, (unsigned int)&systime);
							
						}break;

					case NEWSMESSAGE_TIMEEND:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//???????
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							systime = *(SYSTEMTIME*)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE_EX, (unsigned int)&News, (unsigned int)&systime);
							
						}break;
					
					}
					
				}
				break;
			case UI_PLAYMUSIC:
				{
					char szMusicFile[MAX_PATH];
					memcpy(szMusicFile, pScriptAction->m_pContent, pScriptAction->m_nBufferLen);
					szMusicFile[pScriptAction->m_nBufferLen] = 0;
					g_SubWorldSet.m_cMusic.ScriptPlay(szMusicFile);
				}break;
			
			case UI_OPENTONGUI:				
				{
					Player[CLIENT_PLAYER_INDEX].m_cTong.OpenCreateInterface();
					break;
				}
				
			case UI_ASKFORNUMBER:
				{	
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					
					KUiAskClientNumber *pAsk = NULL;
					pAsk = (KUiAskClientNumber *)malloc(sizeof(KUiAskClientNumber));
					if(!pAsk)
						break;
					
					char strContent[1024];
					g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);

					g_StrCpyLen(pAsk->Title, strContent, sizeof(pAsk->Title));
					pAsk->MinValue = pScriptAction->m_bParam1;
					pAsk->MaxValue = pScriptAction->m_nParam;
					
					g_bUISelLastSelCount = 1;
					CoreDataChanged(GDCNI_GET_NUMBER_UI,(unsigned int)pAsk, NULL);
					free(pAsk);
					pAsk = NULL;
				}
				break;
			case UI_ASKFORSTRING:
				{	
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					
					KUiAskClientString *pAsk = NULL;
					pAsk = (KUiAskClientString *)malloc(sizeof(KUiAskClientString));
					if(!pAsk)
						break;
					
					char strContent[1024];
					char *pInit = NULL;

					g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
					pInit = strstr(strContent, "|");
					if(pInit)
					{
						*pInit++ = 0;
					}

					g_StrCpyLen(pAsk->Title, strContent, sizeof(pAsk->Title));
					g_StrCpyLen(pAsk->Init, pInit, sizeof(pAsk->Init));
					pAsk->MinLen	= pScriptAction->m_bParam1;
					pAsk->MaxLen	= (BYTE)pScriptAction->m_nParam;
					
					g_bUISelLastSelCount = 1;
					CoreDataChanged(GDCNI_GET_STRING_UI,(unsigned int)pAsk, NULL);
					free(pAsk);
					pAsk = NULL;
				}
				break;
			
			}
	} break;
	case SCRIPTACTION_EXESCRIPT:
		{
			if (pScriptAction->m_nBufferLen <= 0 ) 
				break;
			char szScriptInfo[1000];
			g_StrCpyLen(szScriptInfo, pScriptAction->m_pContent,pScriptAction->m_nBufferLen + 1);

			char* pDivPos = strstr(szScriptInfo, "|");
			if(pDivPos)	
				*pDivPos++ = 0; 
			/*
			if (pDivPos)
				ExecuteScript(szScriptInfo, "OnCall", pDivPos);
			else
				ExecuteScript(szScriptInfo, "OnCall", "");
			*/
			
			KLuaScript* pScript = new KLuaScript;

			pScript->Init();
			pScript->RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
			pScript->Load(szScriptInfo);
			
			if(pScript)
			{
				int nTopIndex = 0, nParam = 0;
				pScript->SafeCallBegin(&nTopIndex);
				pScript->CallFunction("OnCall", 0, "ds", nParam, pDivPos);
				pScript->SafeCallEnd(nTopIndex);
				delete pScript;
			}
			pScript = NULL;
		}
		break;
	}
	
	
}
#endif

#ifdef _SERVER

//	

int KPlayer::AddTempTaskValue(void* pData)
{
	memcpy(m_cTask.nClear, pData, sizeof(int) * MAX_TEMP_TASK);
	return 1;
}


//	

void	KPlayer::DialogNpc(BYTE * pProtocol)
{
	PLAYER_DIALOG_NPC_COMMAND * pDialogNpc = (PLAYER_DIALOG_NPC_COMMAND*) pProtocol;
	int nIdx = 0;
	if (pDialogNpc->nNpcId < 0) 
		return;
	nIdx = FindAroundNpc(pDialogNpc->nNpcId);
	if (nIdx > 0) 
	{
		int distance = NpcSet.GetDistance(nIdx, m_nIndex);
		if ((Npc[nIdx].m_Kind == kind_dialoger) ||
			(NpcSet.GetRelation(m_nIndex, nIdx) == relation_none))
		{
			if (distance <= Npc[nIdx].m_DialogRadius * 2)
			{
				if(Npc[nIdx].ActionScript[0])
				{	
					m_nLastDialogIdx = nIdx;		
					ExecuteScript(Npc[nIdx].m_ActionScriptID, "main", nIdx);		//
				}
			}
		}
	}
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cTradeChangeState(BYTE* pMsg)
{
	TRADE_CHANGE_STATE_SYNC	*pTrade = (TRADE_CHANGE_STATE_SYNC*)pMsg;
	
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:
	case PLAYER_MENU_STATE_TRADEOPEN:
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:
		g_Team[0].SetTeamClose();
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TRADING:
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.m_nState = PLAYER_MENU_STATE_TRADING;
			m_cMenuState.m_nTradeDest = pTrade->m_dwNpcID;
			m_cMenuState.m_nTradeState = 0;
			m_cMenuState.m_nTradeDestState = 0;
			this->m_ItemList.RecoverTrade();
			this->m_ItemList.BackupTrade();
			this->m_ItemList.ClearRoom(room_trade);
			this->m_ItemList.ClearRoom(room_trade1);
			
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	}
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cLevelUp(BYTE* pMsg)
{
	PLAYER_LEVEL_UP_SYNC	*pLevel = (PLAYER_LEVEL_UP_SYNC*)pMsg;

	if ((DWORD)Npc[m_nIndex].m_Level < (DWORD)pLevel->m_btLevel && m_nExp < PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level))
	{
		/*KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_GET_EXP, PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level) - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);*/

		
		SetExpAddNo(PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level) - m_nExp);
		
	}
	
	m_nExp = 0;
	Npc[m_nIndex].m_Level = (DWORD)pLevel->m_btLevel;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	if (pLevel->m_nAttributePoint > m_nAttributePoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_ATTRIBUTE_POINT, pLevel->m_nAttributePoint - m_nAttributePoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_ATTRIBUTE;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nAttributePoint = pLevel->m_nAttributePoint;
	
	if (m_nSkillPoint < pLevel->m_nSkillPoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_SKILL_POINT, pLevel->m_nSkillPoint - m_nSkillPoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_SKILLS;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nSkillPoint = pLevel->m_nSkillPoint;
	
	Npc[m_nIndex].m_LifeMax = pLevel->m_nBaseLifeMax;
	Npc[m_nIndex].m_StaminaMax = pLevel->m_nBaseStaminaMax;
	Npc[m_nIndex].m_ManaMax = pLevel->m_nBaseManaMax;
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
	
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
	if (this->m_cTeam.m_nFlag)
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
		{
			g_Team[0].m_nMemLevel[0] = Npc[m_nIndex].m_Level;
		}
		else
		{
			for (int i = 0; i < MAX_TEAM_MEMBER; i++)
			{
				if ((DWORD)g_Team[0].m_nMember[i] == Npc[m_nIndex].m_dwID)
				{
					g_Team[0].m_nMemLevel[i] = Npc[m_nIndex].m_Level;
					break;
				}
			}
		}
	}
	
	KSystemMessage Msg;
				
	Msg.byConfirmType = SMCT_CLICK;
	Msg.byParamSize = 0;
	Msg.byPriority = 1;
	Msg.eType = SMT_PLAYER;
	sprintf(Msg.szMessage, MSG_LEVEL_UP, Npc[m_nIndex].m_Level);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	
	Npc[this->m_nIndex].SetInstantSpr(enumINSTANT_STATE_LEVELUP);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cGetCurAttribute(BYTE* pMsg)
{
	PLAYER_ATTRIBUTE_SYNC	*pAttribute = (PLAYER_ATTRIBUTE_SYNC*)pMsg;
	m_nAttributePoint = pAttribute->m_nLeavePoint;
	int nData;
	switch (pAttribute->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		nData = pAttribute->m_nBasePoint - m_nStrength;
		m_nStrength = pAttribute->m_nBasePoint;
		m_nCurStrength = pAttribute->m_nCurPoint;
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_DEXTERITY:
		nData = pAttribute->m_nBasePoint - m_nDexterity;
		m_nDexterity = pAttribute->m_nBasePoint;
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_VITALITY:
		nData = pAttribute->m_nBasePoint - m_nVitality;
		m_nVitality = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	case ATTRIBUTE_ENGERGY:
		nData = pAttribute->m_nBasePoint - m_nEngergy;
		m_nEngergy = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	case ATTRIBUTE_RESET_ALL:
		int cSeries = pAttribute->m_nBasePoint;
		m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[cSeries];
		m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[cSeries];
		m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[cSeries];
		m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[cSeries];
		
		m_nCurStrength = m_nStrength;
		m_nCurDexterity = m_nDexterity;
		m_nCurVitality = m_nVitality;
		m_nCurEngergy = m_nEngergy;

		Npc[m_nIndex].m_LifeMax =((Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series)) +
			(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
		Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
		
		Npc[m_nIndex].m_ManaMax =(Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series) + 
				(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * m_nCurEngergy));
		Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
		
		Npc[m_nIndex].m_StaminaMax =(Npc[m_nIndex].m_Level*PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_nSex) + 
			(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * m_nCurVitality));
		Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;

		m_nAttributePoint = (Npc[m_nIndex].m_Level - 1) * 5;
		UpdataCurData();
		break;
	}
	CoreDataChanged(GDCNI_PLAYER_RT_ATTRIBUTE, 0, 0);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cSetExp(int nExp)
{
	if (nExp > m_nExp)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_GET_EXP, nExp - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

		
	}
	else if (nExp < m_nExp)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_DEC_EXP, m_nExp - nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	
	
	SetExpAddNo(nExp - m_nExp);
	

	this->m_nExp = nExp;
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cSyncMoney(BYTE* pMsg) 
{
	PLAYER_MONEY_SYNC	*pMoney = (PLAYER_MONEY_SYNC*)pMsg;
	
	if ( CheckTrading() )
	{
		if (pMoney->m_nMoney1 >= pMoney->m_nMoney3)
		{
			m_ItemList.SetMoney(pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney2, pMoney->m_nMoney3);
		}
		else
		{
			m_ItemList.SetMoney(0, pMoney->m_nMoney2 + pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney3);
		}
		//
		m_cTrade.m_bTradeState = FALSE;
		m_cTrade.m_bTradeDestState = FALSE;
		
		m_cTrade.m_nBackEquipMoney = pMoney->m_nMoney1;
		m_cTrade.m_nBackRepositoryMoney = pMoney->m_nMoney2;
	}
	else
	{
		int nMoney1 = m_ItemList.GetMoney(room_equipment);
		if (pMoney->m_nMoney1 - nMoney1 > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_EARN_MONEY, pMoney->m_nMoney1 - nMoney1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		m_ItemList.SetMoney(pMoney->m_nMoney1, pMoney->m_nMoney2, pMoney->m_nMoney3);
	}
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cTradeApplyStart(BYTE* pMsg)
{
	if (!pMsg)
		return;
	TRADE_APPLY_START_SYNC	*pApply = (TRADE_APPLY_START_SYNC*)pMsg;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchID(pApply->m_dwNpcId);
	if (nNpcIdx == 0)
		return;
	
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;
	
	strcpy(sPlayer.Name, Npc[nNpcIdx].Name);
	sPlayer.nIndex = pApply->m_nDestIdx;
	sPlayer.uId = pApply->m_dwNpcId;
	sPlayer.nData = 0;
	
	sprintf(sMsg.szMessage, MSG_TRADE_GET_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_SYSTEM;
	sMsg.byConfirmType = SMCT_UI_TRADE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cTradeMoneySync(BYTE* pMsg)
{
	TRADE_MONEY_SYNC	*pMoney = (TRADE_MONEY_SYNC*)pMsg;
	
	m_ItemList.SetRoomMoney(room_trade1, pMoney->m_nMoney);
	
	KUiObjAtRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = pMoney->m_nMoney;
	CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (unsigned int)&sMoney, 0);
}
#endif

#ifndef _SERVER

//	

void	KPlayer::s2cTradeDecision(BYTE* pMsg)
{
	TRADE_DECISION_SYNC	*pSync = (TRADE_DECISION_SYNC*)pMsg;
	if (pSync->m_btDecision == 1)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_SUCCESS, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		this->m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
	else if (pSync->m_btDecision == 0)
	{
		if ( !CheckTrading() )
			return;
		
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_FAIL, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		m_ItemList.RecoverTrade();
		m_ItemList.SetMoney(m_cTrade.m_nBackEquipMoney, m_cTrade.m_nBackRepositoryMoney, 0);
		m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		CoreDataChanged(GDCNI_CONTAINER_OBJECT_CHANGED, UOC_ITEM_TAKE_WITH, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
}
#endif
//Kenny fix skill chuyen map
#ifndef _SERVER
int KPlayer::GetRightSkillID() const
{
    return m_nRightSkillID;
}

int KPlayer::GetRightSkillLevel() const
{
    return m_nRightSkillLevel;
}


void KPlayer::ForceReapplyRightAura()
{
    int id = m_nRightSkillID;
    int lv = m_nRightSkillLevel;
    if (id <= 0) return;

    // ch? �p g?i l?i n?u l� skill aura (tr�nh spam)
    KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(id, (lv > 0 ? lv : 1));
    if (pSkill && pSkill->IsAura())
    {
        // �p kh�c ID d? SetRightSkill() g?i c2s_changeauraskill cho GS m?i
        m_nRightSkillID = 0;
        SetRightSkill(id);
        // kh�i ph?c level hi?n th? local
        m_nRightSkillLevel = lv;
    }
}
#endif
#ifndef _SERVER

void KPlayer::SyncCurPlayer(BYTE* pMsg)
{
    // --- GI? L?I SKILL TAY TR�I / TAY PH?I TRU?C KHI B? RESET ---
    int keepLeftID    = m_nLeftSkillID;
    int keepLeftLV    = m_nLeftSkillLevel;
    int keepRightID   = m_nRightSkillID;
    int keepRightLV   = m_nRightSkillLevel;

    m_ItemList.RemoveAll();
    CURPLAYER_SYNC* PlaySync = (CURPLAYER_SYNC *)pMsg;
    this->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);
    this->m_dwID = g_FileName2Id(Npc[m_nIndex].Name); 
    Npc[m_nIndex].m_Kind = kind_player;
    Npc[m_nIndex].m_Level = (DWORD)PlaySync->m_btLevel;
    Npc[m_nIndex].m_nSex = PlaySync->m_btSex;
    Npc[m_nIndex].m_Series  = PlaySync->m_btSeries;
    Npc[m_nIndex].SetPlayerIdx(CLIENT_PLAYER_INDEX);

    m_nAttributePoint = PlaySync->m_wAttributePoint;
    m_nSkillPoint     = PlaySync->m_wSkillPoint;
    m_nStrength       = PlaySync->m_wStrength;
    m_nDexterity      = PlaySync->m_wDexterity;
    m_nVitality       = PlaySync->m_wVitality;
    m_nEngergy        = PlaySync->m_wEngergy;
    m_nLucky          = PlaySync->m_wLucky;
    m_nCurStrength    = m_nStrength;
    m_nCurDexterity   = m_nDexterity;
    m_nCurVitality    = m_nVitality;
    m_nCurEngergy     = m_nEngergy;
    m_nCurLucky       = m_nLucky;

    SetFirstDamage();
    SetBaseAttackRating();
    SetBaseDefence();

    Npc[m_nIndex].m_ActionScriptID = 0;
    Npc[m_nIndex].m_TrapScriptID   = 0;

    m_nExp             = PlaySync->m_nExp;
    m_nNextLevelExp    = PlayerSet.m_cLevelAdd.GetLevelExp(PlaySync->m_btLevel);
    m_dwLeadExp        = PlaySync->m_dwLeadExp;
    m_dwLeadLevel      = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp);
    m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);

    m_cFaction.m_nCurFaction     = (char)PlaySync->m_btCurFaction;
    m_cFaction.m_nFirstAddFaction= (char)PlaySync->m_btFirstFaction;
    m_cFaction.m_nAddTimes       = PlaySync->m_nFactionAddTimes;
    m_nWorldStat                 = (int)PlaySync->m_wWorldStat;
    m_nSectStat                  = (int)PlaySync->m_wSectStat;

    m_ItemList.Init(CLIENT_PLAYER_INDEX);
    m_ItemList.SetMoney(PlaySync->m_nMoney1, PlaySync->m_nMoney2, 0);
    this->SetBronzeCoin(PlaySync->m_nBronzeCoin);
    this->SetLockBronzeCoin(PlaySync->m_nLockBronzeCoin);

    m_btRepositoryCount = PlaySync->m_btRepositoryCount;
    m_nLockPlayer       = PlaySync->m_nLockPlayer;
    m_btExpandtoryCount = PlaySync->m_btExpandtoryCount;
    m_ImagePlayer       = PlaySync->m_btImagePlayer;

    Npc[m_nIndex].m_LifeMax     = PlaySync->m_wLifeMax;
    Npc[m_nIndex].m_ManaMax     = PlaySync->m_wManaMax;
    Npc[m_nIndex].m_StaminaMax  = PlaySync->m_wStaminaMax;
    Npc[m_nIndex].m_LifeReplenish = PLAYER_LIFE_REPLENISH;
    Npc[m_nIndex].m_ManaReplenish = PLAYER_MANA_REPLENISH;
    Npc[m_nIndex].m_StaminaGain   = PLAYER_STAMINA_GAIN;
    Npc[m_nIndex].m_StaminaLoss   = PLAYER_STAMINA_LOSS;

    SetBaseResistData();
    SetBaseSpeedAndRadius();
    Npc[m_nIndex].RestoreNpcBaseInfo();

    m_BuyInfo.Clear();
    m_cMenuState.Release();
    m_cChat.Release();
    memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
    m_nAvailableAnswerNum = 0;
    Npc[m_nIndex].m_Experience = 0;
    memset(Npc[m_nIndex].m_szChatBuffer, 0, sizeof(Npc[m_nIndex].m_szChatBuffer));
    Npc[m_nIndex].m_nCurChatTime = 0;

    m_RunStatus = 1;
    m_nPeapleIdx  = 0;
    m_nObjectIdx  = 0;
    m_MouseDown[0]= 0;
    m_MouseDown[1]= 0;
    Npc[m_nIndex].m_SyncSignal = 0;

    // --- KH�I PH?C SKILL ---
    if (keepLeftID > 0) {
        m_nLeftSkillID    = keepLeftID;
        m_nLeftSkillLevel = keepLeftLV;
        SetLeftSkill(keepLeftID);
    } else {
        // tu? �: SetDefaultImmedSkill();
    }

    if (keepRightID > 0) {
        KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(keepRightID, keepRightLV);
        if (pSkill && pSkill->IsAura()) {
            m_nRightSkillID = 0;
            SetRightSkill(keepRightID);
            m_nRightSkillLevel = keepRightLV;
			Npc[m_nIndex].m_ActiveAuraID = keepRightID;
        } else {
            m_nRightSkillID    = keepRightID;
            m_nRightSkillLevel = keepRightLV;
        }
    }
}

#endif

//	

BOOL	KPlayer::CheckTrading()
{
	return (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADING);
}


//	

void	KPlayer::SetFirstDamage()
{
	int nDamageBase;

	nDamageBase = m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	
	Npc[m_nIndex].m_PhysicsDamage.nValue[0] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[2] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2] = 0;

	
	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[0] = 0;
	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[2] = 0;
	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[2] = 0;

	Npc[m_nIndex].m_AutoReplySkill.nValue[0] = 0;
	Npc[m_nIndex].m_AutoReplySkill.nValue[1] = 0;
	Npc[m_nIndex].m_AutoReplySkill.nValue[2] = 0;

	Npc[m_nIndex].m_AutoAttackSkill.nValue[0] = 0;
	Npc[m_nIndex].m_AutoAttackSkill.nValue[1] = 0;
	Npc[m_nIndex].m_AutoAttackSkill.nValue[2] = 0;

	Npc[m_nIndex].m_AutoDeathSkill.nValue[0] = 0;
	Npc[m_nIndex].m_AutoDeathSkill.nValue[1] = 0;
	Npc[m_nIndex].m_AutoDeathSkill.nValue[2] = 0;

	Npc[m_nIndex].m_AutoRescueSkill.nValue[0] = 0;
	Npc[m_nIndex].m_AutoRescueSkill.nValue[1] = 0;
	Npc[m_nIndex].m_AutoRescueSkill.nValue[2] = 0;
	
	
}


//	

void	KPlayer::SetBaseAttackRating()
{
	Npc[m_nIndex].m_AttackRating = m_nDexterity * 4 - 28;
}


//	

void	KPlayer::SetBaseDefence()
{
	Npc[m_nIndex].m_Defend = m_nDexterity >> 2;
}


//	

void	KPlayer::SetBaseResistData()
{
	Npc[m_nIndex].m_FireResist			= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_ColdResist			= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PoisonResist		= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_LightResist			= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PhysicsResist		= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_FireResistMax		= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax		= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax		= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax		= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax	= BASE_PHYSICS_RESIST_MAX;
}


//	

void	KPlayer::SetBaseSpeedAndRadius()
{
	Npc[m_nIndex].m_WalkSpeed = BASE_WALK_SPEED;
	Npc[m_nIndex].m_RunSpeed = BASE_RUN_SPEED;
	Npc[m_nIndex].m_AttackSpeed = BASE_ATTACK_SPEED;
	Npc[m_nIndex].m_CastSpeed = BASE_CAST_SPEED;
	Npc[m_nIndex].m_VisionRadius = BASE_VISION_RADIUS;
	Npc[m_nIndex].m_HitRecover = BASE_HIT_RECOVER;
}

#ifndef _SERVER

//	

void KPlayer::DialogNpc(int nIndex)
{
	if (nIndex > 0 && Npc[nIndex].m_Index > 0)
	{
		if (Npc[nIndex].ActionScript[0])
		{
			ExecuteScript(Npc[nIndex].m_ActionScriptID,"main","");
		}
		else
		{
			PLAYER_DIALOG_NPC_COMMAND DialogNpcCmd;

			DialogNpcCmd.ProtocolType	= c2s_dialognpc;
			DialogNpcCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			DialogNpcCmd.dwTimePacker = GetTickCount();
			DialogNpcCmd.nNpcId			= Npc[nIndex].m_dwID;
			if (g_pClient)
				g_pClient->SendPackToServer(&DialogNpcCmd, sizeof(PLAYER_DIALOG_NPC_COMMAND));
			
		}
	}
}
#endif

#ifndef _SERVER

//	

void KPlayer::CheckObject(int nIdx)
{
	switch(Object[nIdx].m_nKind)
	{
		case Obj_Kind_Item:
		case Obj_Kind_Money:
			PickUpObj(nIdx);
			Npc[m_nIndex].m_nObjectIdx = 0;
			break;
		case Obj_Kind_Box:
		case Obj_Kind_Door:
		case Obj_Kind_Trap:
		case Obj_Kind_Prop:
		case Obj_Kind_Task:
			this->ObjMouseClick(nIdx);
			Npc[m_nIndex].m_nObjectIdx = 0;
			break;
		default:
			break;
	}
	m_nObjectIdx = 0;
	m_nPickObjectIdx = 0;
}
#endif

#ifndef _SERVER



BOOL KPlayer::ShopApplyOpen()
{
	if(this->CheckTrading())
		return FALSE;
	if(Npc[m_nIndex].m_FightMode)
		return FALSE;
	if (this->m_bLockPlayer)//LockPlayer by kinnox;
		return FALSE;		
	if(Npc[m_nIndex].m_bOpenShop)
		return FALSE;
	if(!Npc[m_nIndex].m_szShopAdv[0])
		return FALSE;
	if(Npc[m_nIndex].m_bRideHorse)
		return FALSE;
	if(strcmp(SubWorld[0].m_szMapType, "City") &&
		strcmp(SubWorld[0].m_szMapType, "Capital"))
		return FALSE;

	/*
	if(this->TaskGetSaveVal(TV_OPENSHOP) <= 0)
	{
		KSystemMessage Msg;	
		Msg.byConfirmType = SMCT_NONE;
		Msg.eType = SMT_NORMAL;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		strcpy(Msg.szMessage, MSG_TIME_OPEN_SHOP);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
		return FALSE;
	}
	*/

	if(Npc[m_nIndex].m_Doing != do_sit)
	{
		Npc[m_nIndex].SendCommand(do_sit);
		SendClientCmdSit(TRUE);
	}

	PLAYER_SHOP_COMMAND OpenShop;
	OpenShop.ProtocolType	= c2s_playershop;
	OpenShop.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	OpenShop.dwTimePacker = GetTickCount();
	OpenShop.m_bOpen		= TRUE;

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&OpenShop, sizeof(PLAYER_SHOP_COMMAND));

	return TRUE;
}
#endif

#ifndef _SERVER



BOOL KPlayer::ShopApplyClose()
{
	if(!Npc[m_nIndex].m_bOpenShop)
		return FALSE;
	
	if(Npc[m_nIndex].m_Doing == do_sit)
	{
		Npc[m_nIndex].SendCommand(do_stand);
		SendClientCmdSit(FALSE);
	}

	PLAYER_SHOP_COMMAND OpenShop;
	OpenShop.ProtocolType	= c2s_playershop;
	OpenShop.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	OpenShop.dwTimePacker = GetTickCount();
	OpenShop.m_bOpen		= FALSE;

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&OpenShop, sizeof(PLAYER_SHOP_COMMAND));
	return TRUE;
}
#endif

#ifndef _SERVER
// --
//
// --
void KPlayer::DrawSelectInfo()
{
	if (m_nIndex <= 0)
		return;
	
	if (m_nPeapleIdx)
	{
		if (Npc[m_nPeapleIdx].m_Kind == kind_player)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
				Npc[m_nPeapleIdx].PaintRankInfo(Npc[m_nPeapleIdx].GetNpcPate());
			}
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}		
		return;
	}
	if (m_nObjectIdx)
	{
		if (!ObjSet.CheckShowName())
			Object[m_nObjectIdx].DrawInfo();
		return;
	}
}
#endif

#ifndef _SERVER

//	

void KPlayer::GetEchoDamage(int* nMin, int* nMax, int* nAr, int nType)
{
	int nSkillId;
	*nMin = 0;
	*nMax = 0;
	
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC 
		|| nType < 0 || nType > 1 
		|| NULL == nMin || NULL == nMax)
		return;
	
	nSkillId = (0 == nType)?m_nLeftSkillID:m_nRightSkillID;
	int nLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);
	int nPointAdd = Npc[m_nIndex].m_SkillList.GetPointAdd(nSkillId);		
	
	if (nSkillId <= 0 ||nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel + nPointAdd);		//
	if (!pOrdinSkill) 
        return ;

	switch(pOrdinSkill->GetSkillStyle())
	{
		case SKILL_SS_Missles:
		case SKILL_SS_Melee:
		case SKILL_SS_InitiativeNpcState:
		case SKILL_SS_PassivityNpcState:
			{
			}
			break;
		default:
			return;
	}
	
	BOOL	bIsPhysical = pOrdinSkill->IsPhysical();

	// lay % ho tro skill
	int nSkillEnhance = 0;
	if(nSkillId > 0 && nSkillId < MAX_SKILL)
	{
		nSkillEnhance = Npc[m_nIndex].m_SkillList.GetEnhanceSkills(nSkillId);
	}

	pMagicData = pOrdinSkill->GetDamageAttribs();
	
	if (!pMagicData)
		return;
	
	
	
	int nMinNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[0] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	int nMaxNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[2] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	// Skip attackrating
	*nAr = Npc[m_nIndex].m_CurrentAttackRating;
	if(nType == 0)
	{
		if (magic_attackrating_p == pMagicData->nAttribType)
		{
			*nAr += Npc[m_nIndex].m_AttackRating * pMagicData->nValue[0] / 100;
		}
	}
	// Skip ignoredefense
	pMagicData++;
	// Calc physics damage
	pMagicData++;
	if (magic_physicsenhance_p == pMagicData->nAttribType)
	{
		*nMin += nMinNpcDamage * (100 + pMagicData->nValue[0]) / 100;
		*nMax += nMaxNpcDamage * (100 + pMagicData->nValue[0]) / 100;
		*nMin += nMinNpcDamage * (pMagicData->nValue[0] * nSkillEnhance / 100) / 100;
		*nMax += nMaxNpcDamage * (pMagicData->nValue[0] * nSkillEnhance / 100) / 100;
		
		int nEnhance;
		if (equip_meleeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentMeleeEnhance[m_ItemList.GetWeaponParticular()];
		}
		else if (equip_rangeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentRangeEnhance;
		}
		else 
		{
			nEnhance = Npc[m_nIndex].m_CurrentHandEnhance;
		}
		*nMin += nMinNpcDamage * nEnhance / 100;
		*nMax += nMaxNpcDamage * nEnhance / 100;
	}
	else if (magic_physicsdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0] * (100 + nSkillEnhance) / 100;
		*nMax += pMagicData->nValue[2] * (100 + nSkillEnhance) / 100;
		// Luc danh vat ly noi
		if (!bIsPhysical)
		{
			*nMin += Npc[m_nIndex].m_MagicPhysicsDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_MagicPhysicsDamage.nValue[2];
		}
	}
	pMagicData++;
	// Tinh luc danh bang
	if (magic_colddamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0] * (100 + nSkillEnhance) / 100;
		*nMax += pMagicData->nValue[2] * (100 + nSkillEnhance) / 100;
		// Luc danh bang noi
		if (!bIsPhysical)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2];
		}
	}
	// Luc danh bang ngoai
	if (bIsPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentColdDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentColdDamage.nValue[2];
	}
	// Tinh luc danh hoa
	pMagicData++;
	if (magic_firedamage_v == pMagicData->nAttribType)
	{
		/* Fix by kinnox;
		*nMin += pMagicData->nValue[0] * (100 + nSkillEnhance) / 100;
		*nMax += pMagicData->nValue[2] * (100 + nSkillEnhance) / 100;
		// Luc danh hoa noi
		if (!bIsPhysical)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2];
		}*/

		*nMin += pMagicData->nValue[0]*(100+nSkillEnhance)/100 + pMagicData->nValue[0]*(100+nSkillEnhance)/100 * Npc[m_nIndex].m_CurrentFireEnhance/100;
		*nMax += pMagicData->nValue[2]*(100+nSkillEnhance)/100 + pMagicData->nValue[2]*(100+nSkillEnhance)/100 * Npc[m_nIndex].m_CurrentFireEnhance/100;
		if (!bIsPhysical)
	     {
			 *nMin += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0] + Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0] * Npc[m_nIndex].m_CurrentFireEnhance/100;
			 *nMax += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2] + Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2] * Npc[m_nIndex].m_CurrentFireEnhance/100;
	     }

	}
	// Luc danh hoa ngoai
	if (bIsPhysical)
	{
		/* Fix by kinnox;			
		*nMin += Npc[m_nIndex].m_CurrentFireDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentFireDamage.nValue[2];*/
		*nMin += Npc[m_nIndex].m_CurrentFireDamage.nValue[0] + Npc[m_nIndex].m_CurrentFireDamage.nValue[0] * Npc[m_nIndex].m_CurrentFireEnhance/100;
		*nMax += Npc[m_nIndex].m_CurrentFireDamage.nValue[2] + Npc[m_nIndex].m_CurrentFireDamage.nValue[2] * Npc[m_nIndex].m_CurrentFireEnhance/100;
	}
	// Tinh luc danh loi
	pMagicData++;
	if (magic_lightingdamage_v == pMagicData->nAttribType)
	{
		/* Fix by kinnox;	
		*nMin += pMagicData->nValue[0] * (100 + nSkillEnhance) / 100;
		*nMax += pMagicData->nValue[2] * (100 + nSkillEnhance) / 100;
		// Luc danh loi noi
		if (!bIsPhysical)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2];
		}*/
		*nMin += pMagicData->nValue[0]*(100+nSkillEnhance)/100 + (pMagicData->nValue[2]*(100+nSkillEnhance)/100 - pMagicData->nValue[0]*(100+nSkillEnhance)/100) * Npc[m_nIndex].m_CurrentLightEnhance / 100;
		*nMax += pMagicData->nValue[2]*(100+nSkillEnhance)/100;

		if (!bIsPhysical)
	     {
		 *nMin += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0];
		 *nMax += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2];
	     }
	}

	// Luc danh loi ngoai
	if (bIsPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentLightDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentLightDamage.nValue[2];
	}
	// Tinh luc danh doc
	pMagicData++;
	if (magic_poisondamage_v == pMagicData->nAttribType)
	{
		if (pMagicData->nValue[2] > 0)
		{
			int nPoisonDamage = (pMagicData->nValue[0] * pMagicData->nValue[1] / pMagicData->nValue[2]);
			nPoisonDamage = nPoisonDamage * (100 + nSkillEnhance) / 100;
			*nMin += nPoisonDamage;
			*nMax += nPoisonDamage;
		}
		// Luc danh doc noi
		if (!bIsPhysical)
		{
			if (Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[2] > 0)
			{
				int nPoisonDamage = (Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0]
					* Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[1] 
					/ Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[2]);
				*nMin += nPoisonDamage;
				*nMax += nPoisonDamage;
			}
		}
	}
	// Luc danh doc ngoai
	if (bIsPhysical)
	{
		if (Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2] > 0)
		{
			int nPoisonDamage = (Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0]
				* Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1] 
				/ Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2]);
			*nMin += nPoisonDamage;
			*nMax += nPoisonDamage;
		}
	}
	pMagicData++;
	// Calc magic damage
	if (magic_magicdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];
	}
}
#endif


//	

void KPlayer::SetNpcDamageAttrib()
{
	SetNpcPhysicsDamage();
}

#ifdef _SERVER

//	

void KPlayer::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!m_dwID || m_nNetConnectIdx == -1)
		return;
	
	m_sExchangePos.m_dwMapID = dwMapID;
	m_sExchangePos.m_nX = nX;
	m_sExchangePos.m_nY = nY;
	m_bExchangeServer = TRUE;
}
#endif

/*
#ifdef _SERVER

//	

void KPlayer::UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode)
{
	TRoleData* pData = (TRoleData *)&m_SaveBuffer;
	pData->BaseInfo.cFightMode = nFightMode;
	pData->BaseInfo.cUseRevive = 0;
	pData->BaseInfo.ientergameid = dwSubWorldID;
	pData->BaseInfo.ientergamex = nX;
	pData->BaseInfo.ientergamey = nY;
}
#endif
*/

#ifdef _SERVER

//	

BOOL KPlayer::IsExchangingServer()
{
	return m_bExchangeServer;
}
#endif

#ifdef _SERVER

//	

void KPlayer::SetLastNetOperationTime(int nTime)
{
	if (m_nNetConnectIdx == -1 || m_dwID == 0 || m_nIndex <= 0)
		return;
	
	m_nLastNetOperationTime = nTime;
	if (m_bSleepMode)
	{
		m_bSleepMode = FALSE;
		NPC_SLEEP_SYNC	SleepSync;
		SleepSync.ProtocolType = s2c_npcsleepmode;
		SleepSync.bSleep = m_bSleepMode;
		SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
	}
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::CreateTong(int nCamp, char *lpszTongName)
{
	return this->m_cTong.Create(nCamp, lpszTongName);
}
#endif

#ifdef _SERVER

//	

void KPlayer::RepairItem(DWORD dwItemID)
{
	int nIdx = m_ItemList.SearchID(dwItemID);
	int nCost = Item[nIdx].GetRepairPrice();

	if (!nCost)
		return;

	int nMaxDur = Item[nIdx].GetMaxDurability();
	if (Pay(nCost) && m_nNetConnectIdx >= 0)
	{
		
		Item[nIdx].SetDurability(nMaxDur);
		
		ITEM_CHANGE_INFO sChange;
		sChange.ProtocolType	= s2c_itemchangeinfosync;
		sChange.m_btType		= 4; 
		sChange.m_dwItemID		= dwItemID;
		sChange.m_uChange		= nMaxDur;
		if (g_pServer)
			g_pServer->PackDataToClient(m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
	}
}
#endif

#ifdef _SERVER

//	

void KPlayer::SetExtPoint(int nPoint, int nChangePoint)
{
	m_nExtPoint = nPoint;
	m_nChangeExtPoint = nChangePoint;
}


//	

int KPlayer::GetExtPoint()
{
	return m_nExtPoint;
}


//	

BOOL KPlayer::PayExtPoint(int nPoint)
{
	if (m_nExtPoint < nPoint)
		return FALSE;

	m_nExtPoint -= nPoint;
	m_nChangeExtPoint += nPoint;
	return TRUE;
}


//	

int KPlayer::GetExtPointChanged()
{
	return m_nChangeExtPoint;
}


//	

void KPlayer::SetExtPoint2(int nPoint)
{
	m_nExtPoint2 = nPoint;
}

#endif

#ifdef _SERVER

//	

void KPlayer::SyncBronzeCoin()
{

	TASK_VALUE_SYNC		sValue;
	sValue.ProtocolType		= s2c_taskvaluesync;
	sValue.m_bClear			= 2;
	sValue.m_nId			= 0;
	sValue.m_bValue			= m_nBronzeCoin;

	if(g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sValue, sizeof(TASK_VALUE_SYNC));

}
#endif

// --
//
// --
void KPlayer::SetBronzeCoin(const int nCoin)
{
#ifdef _SERVER
	if( !MSSQL_SetExtPoint(this->m_AccoutName, nCoin) )
		return;
	
	m_nBronzeCoin = nCoin;
	SyncBronzeCoin();
#endif

#ifndef _SERVER
	m_nBronzeCoin = nCoin;
#endif

}

// --
//
// --
int KPlayer::GetBronzeCoin()
{
#ifdef _SERVER
	int nPoint = 0;
	if( MSSQL_GetExtPoint(this->m_AccoutName, &nPoint) )
		m_nBronzeCoin = nPoint;
	else
		m_nBronzeCoin = 0;
#endif

	return m_nBronzeCoin;
}

#ifdef _SERVER

//	

BOOL KPlayer::PayBronzeCoin(const int nCoin)
{
	if(m_nBronzeCoin < nCoin)
		return FALSE;
	
	if( MSSQL_PayExtPoint(this->m_AccoutName, nCoin) )
	{
		GetBronzeCoin();
		SyncBronzeCoin();
		return TRUE;
	}
	return FALSE;	
}
#endif

#ifdef _SERVER

//	

void KPlayer::SyncLockBronzeCoin()
{

	TASK_VALUE_SYNC		sValue;
	sValue.ProtocolType		= s2c_taskvaluesync;
	sValue.m_bClear			= 3;
	sValue.m_nId			= 0;
	sValue.m_bValue			= m_nLockBronzeCoin;

	if(g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sValue, sizeof(TASK_VALUE_SYNC));

}
#endif

// --
//
// --
void KPlayer::SetLockBronzeCoin(const int nCoin)
{
#ifdef _SERVER
	if( !MSSQL_SetExtPoint2(this->m_AccoutName, nCoin) )
		return;
	
	m_nLockBronzeCoin = nCoin;
	SyncLockBronzeCoin();
#endif

#ifndef _SERVER
	m_nLockBronzeCoin = nCoin;
#endif

}

// --
//
// --
int KPlayer::GetLockBronzeCoin()
{
 #ifdef _SERVER
	 int nPoint = 0;
	 if( MSSQL_GetExtPoint2(this->m_AccoutName, &nPoint) )
		 m_nLockBronzeCoin = nPoint;
	 else
		m_nLockBronzeCoin = 0;
 #endif

	return m_nLockBronzeCoin;
}

#ifdef _SERVER

//	

BOOL KPlayer::PayLockBronzeCoin(const int nCoin)
{
	if(m_nLockBronzeCoin < nCoin)
		return FALSE;
	
	if( MSSQL_PayExtPoint2(this->m_AccoutName, nCoin) )
	{
		GetLockBronzeCoin();
		SyncLockBronzeCoin();
		return TRUE;
	}
	return FALSE;	
}
#endif

#ifdef _SERVER

//

void KPlayer::ProcessEnhance()
{
	if(this->m_cTask.GetSaveVal(TV_ENHANCE_EXP) > 0)
	{
		this->m_cTask.SetSaveVal(TV_ENHANCE_EXP, this->m_cTask.GetSaveVal(TV_ENHANCE_EXP) - 1);
	}

	if(this->m_cTask.GetSaveVal(TV_ENHANCE_SKILL) > 0)
	{
		this->m_cTask.SetSaveVal(TV_ENHANCE_SKILL, this->m_cTask.GetSaveVal(TV_ENHANCE_SKILL) - 1);
	}
	
	if(this->m_cTask.GetSaveVal(TV_QUEHOATUU) > 0)
	{
		this->m_cTask.SetSaveVal(TV_QUEHOATUU, this->m_cTask.GetSaveVal(TV_QUEHOATUU) - 1);
	}

	

	if(this->m_cTask.GetSaveVal(TV_THIENSOWNBAOLO) > 0)
	{
		this->m_cTask.SetSaveVal(TV_THIENSOWNBAOLO, this->m_cTask.GetSaveVal(TV_THIENSOWNBAOLO) - 1);
	}


 	if (this->m_nLixianStatus) //Lixian by kinnox;
	{
		  int nLeftTime = this->m_cTask.GetSaveVal(TV_LIXIANEXPRESS); //Lixian by kinnox;

		  if (nLeftTime <= 0)
		  {
		  	this->m_cTask.SetSaveVal(TV_LIXIANEXPRESS,0);
		  	return;
		  }
		  else
		  {
			this->m_cTask.SetSaveVal(TV_LIXIANEXPRESS,nLeftTime - 1);
			//#pragma omp atomic, uy thac kinh nghiem theo time;
			this->m_nLixianTime++;
			// lon hon 120 giay ( 2phut ) + 1 lan exp; [ cho nay chua hop ly bua nao sua o day]
			if (m_nLixianTime >= 120*18 && !Npc[m_nIndex].m_FightMode)
			{
				this->m_nLixianTime = 0;
				float nEnhanceExp = Npc[m_nIndex].m_Level/100;
				int nGetEnhanceExp = GetRandomNumber(50000,100000);
				DirectAddExp((int)(nGetEnhanceExp + nGetEnhanceExp*nEnhanceExp));
				this->m_uMustSave = SAVE_REQUEST;
				Save();
			}			
		  }
	 }

	/*
	if(this->m_cTask.GetSaveVal(TV_OPENSHOP) > 0)
	{	
		if(m_cShop.GetState())
		{
			this->TaskSetSaveVal(TV_OPENSHOP, this->m_cTask.GetSaveVal(TV_OPENSHOP) - 1);
		}
	}
	else
	{
		if(m_cShop.GetState())
		{
			m_cShop.SetState(FALSE);
		}
	}
	*/
}

void KPlayer::ProcessRemoveEnhance()
{	
	if(this->m_cTask.GetSaveVal(TV_QUEHOATUU) > 0)
	{
		this->m_cTask.SetSaveVal(TV_QUEHOATUU, 0);
	}
	
	if (this->m_cTask.GetSaveVal(TV_THIENSOWNBAOLO) > 0)
	{
		this->m_cTask.SetSaveVal(TV_THIENSOWNBAOLO, 0);
	}
}
#endif

#ifdef _SERVER

//	

void	KPlayer::ExeScriptButton(BYTE* pProtocol)
{	
		EXE_SCRIPT_COMMAND*	 pExe = (EXE_SCRIPT_COMMAND*)pProtocol;

	if(!pExe)
		return;
	if(!pExe->m_btExeId)
		return;
	//Lixian by kinnox;
	BOOL nRet = FALSE;
	int nMapLimited = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (!Npc[m_nIndex].m_FightMode && (nMapLimited == 11 || nMapLimited == 1 ||
		nMapLimited == 37 || nMapLimited == 78 || nMapLimited == 176 ||
		nMapLimited == 162 || nMapLimited == 80 || nMapLimited == 53) ) //mess errror by kinnox;		
	{
		nRet = TRUE;
	}	
	switch(pExe->m_btExeId)
	{
	case 1:
		{	
			if(!pExe->m_szContent || !pExe->m_szContent[0])
				break;
				
			if (!nRet)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_OFFLINE_MARKET;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				break;	
			}
			
			char szScriptFile[64];
			char* pId = NULL;
			pId = strstr(pExe->m_szContent, "|");
			if(pId)
			{
				*pId++ = 0;
			}
			if( strcmp(pExe->m_szContent, "OpenMarket") )
				break;
			
			g_GameSettingFile.GetString("SHOP", "ShopScript", "", szScriptFile, sizeof(szScriptFile));
			this->ExecuteScript(szScriptFile, pExe->m_szContent, pId);
		}
		break;
	case 2:
		{
			if(!pExe->m_szContent || !pExe->m_szContent[0])
				break;
			

			char szScriptFile[64];
			char* pId = NULL;
			pId = strstr(pExe->m_szContent, "|");
			if(pId)
			{
				*pId++ = 0;
			}
			if( strcmp(pExe->m_szContent, "OnBind") &&
				strcmp(pExe->m_szContent, "OnUnBind") )
				break;

			DWORD dwID = atol(pId);
			int nItemIdx = ItemSet.SearchID(dwID);
			g_GameSettingFile.GetString("BINDITEM", "Script", "", szScriptFile, sizeof(szScriptFile));
			this->ExecuteScript(szScriptFile, pExe->m_szContent, nItemIdx);
		}
		break;
	case 3:
		{	
			if(!pExe->m_szContent || !pExe->m_szContent[0])
				break;		
				
			if (!nRet)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_OFFLINE_MARKET;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				break;	
			}	
				
			char szScriptFile[64];
			char* pId = NULL;
			pId = strstr(pExe->m_szContent, "|");
			if(pId)
			{
				*pId++ = 0;
			}
			if( strcmp(pExe->m_szContent, "Offline") )
				break;
			StopMove();
			g_GameSettingFile.GetString("OFFLINE", "Script", "", szScriptFile, sizeof(szScriptFile));
			this->ExecuteScript(szScriptFile, pExe->m_szContent, pId);
		}
		break;
	case 4: 
		{
			if(!pExe->m_szContent || !pExe->m_szContent[0])
				break;		
				
			if (!nRet)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_OFFLINE_MARKET;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				break;	
			}	
				
			char szScriptFile[64];
			char* pId = NULL;
			pId = strstr(pExe->m_szContent, "|");
			if(pId)
			{
				*pId++ = 0;
			}
			if( strcmp(pExe->m_szContent, "ExeTremble") )
				break;
			StopMove();
			g_GameSettingFile.GetString("TREMBLEITEM", "Script", "", szScriptFile, sizeof(szScriptFile));
			this->ExecuteScript(szScriptFile, pExe->m_szContent, pId);
		}
		break;
	default:
		break;
	}
}
#endif

#ifdef _SERVER

//

void KPlayer::BreakItem(const DWORD dwItemID, const int nNumber)
{	
	if( this->CheckTrading() )
		return;

	if( this->m_cShop.GetState() )
		return;
	
	if(m_ItemList.Hand())
		return;

	int nItemIdx = m_ItemList.SearchID(dwItemID);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return;

	if( !Item[nItemIdx].CanStack() )
		return;
	
	int nStackCount = Item[nItemIdx].GetDurability();
	if( (nNumber <= 0) || (nNumber >= nStackCount) )
		return;

	int nTotalValue = Item[nItemIdx].GetStackCount() - nNumber;	
	Item[nItemIdx].SetDurability(nTotalValue);

	ITEM_CHANGE_INFO sChange;
	sChange.ProtocolType	= s2c_itemchangeinfosync;
	sChange.m_btType		= 1;
	sChange.m_dwItemID		= Item[nItemIdx].GetID();
	sChange.m_uChange		= (UINT)Item[nItemIdx].GetDurability();
	if (g_pServer)
		g_pServer->PackDataToClient(this->m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));

	const int nGameIdx = ItemSet.Add(&Item[nItemIdx]);
	if( (nGameIdx <= 0) || (nGameIdx >= MAX_ITEM) )
		return;

	Item[nGameIdx].SetStackCount(nNumber);

	m_ItemList.Add(nGameIdx, pos_hand, 0 ,0);
}
#endif

// --
//
// --
void	KPlayer::HideMask(BOOL bHide)
{
	if(bHide)
	{	
		if(!m_ItemList.m_bMaskLock)
		{
			Npc[m_nIndex].m_wMaskType = 0;
		}
	}
	else
	{	
		if(!m_ItemList.m_bMaskLock)
		{
			int nMaskIdx = m_ItemList.GetEquipment(itempart_mask);
			Npc[m_nIndex].m_wMaskType = (WORD)Item[nMaskIdx].GetBaseMagic();
		}
	}
#ifndef _SERVER
	HIDE_MASK_COMMAND	Command;
	Command.ProtocolType = c2s_hidemask;
	Command.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Command.dwTimePacker = GetTickCount();
	Command.m_bHide = bHide;		

	if (g_pClient)
		g_pClient->SendPackToServer(&Command, sizeof(HIDE_MASK_COMMAND));
#endif
}


#ifdef _SERVER

//	

void	KPlayer::SendShopItemInfo(int nTargetPlayer, int nDataIndex)
{
	if (nTargetPlayer <= 0 || nTargetPlayer >= MAX_PLAYER || Player[nTargetPlayer].m_nIndex <= 0)
		return;
	if(Player[nTargetPlayer].m_bLockPlayer)//LockPlayer by kinnox;
		return;	
	int nIdx = 0, i = 0, j = 0, k = 0, nCount = 0;
	SViewShopItemInfo	*pInfo;

	VIEW_SHOP_SYNC	sView;
	sView.ProtocolType		= s2c_viewshop;
	sView.m_dwNpcID			= Npc[m_nIndex].m_dwID;
	
	memset(&sView.m_sInfo, 0, sizeof(SViewShopItemInfo) * 6);

	for (i = nDataIndex; i < MAX_PLAYER_ITEM; i++)
	{		
		if(m_ItemList.m_Items[i].nPlace != pos_equiproom)
			continue;

		nIdx = m_ItemList.m_Items[i].nIdx;
		if (nIdx <= 0 || nIdx >= MAX_ITEM)
			continue;
		if(Item[nIdx].GetPlayerShopPrice() == 0)
			continue;
		
		pInfo = &sView.m_sInfo[k];
		
		if (nIdx <= 0)
		{
			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		pInfo->m_ID				= Item[nIdx].GetID();
		pInfo->m_Genre			= Item[nIdx].GetGenre();
		pInfo->m_Detail			= Item[nIdx].GetDetailType();
		pInfo->m_Series			= Item[nIdx].GetSeries();
		pInfo->m_Level			= Item[nIdx].GetLevel();
		pInfo->m_Record			= Item[nIdx].GetRecord();
		pInfo->m_Luck			= Item[nIdx].GetGeneratorParam()->nLuck;
		pInfo->m_RandomSeed		= Item[nIdx].GetGeneratorParam()->dwRandomSeed;
		pInfo->m_Version		= Item[nIdx].GetGeneratorParam()->nVersion;
		//them SendStackNum fix by player shop by kinnox;
		pInfo->m_StackNum		= Item[nIdx].GetStackCount();
		//
		for (j = 0; j < 6; j++) 
			pInfo->m_MagicLevel[j] = (BYTE)Item[nIdx].GetGeneratorParam()->nGeneratorLevel[j]; 
		
		pInfo->m_Durability		= Item[nIdx].GetDurability();
		pInfo->m_ExpiredTime	= Item[nIdx].GetTime();
		pInfo->m_StorePrice		= Item[nIdx].GetPlayerShopPrice();
		pInfo->m_X				= m_ItemList.m_Items[i].nX;
		pInfo->m_Y				= m_ItemList.m_Items[i].nY;
		

		k++;

		if(k >= defMAX_SHOPITEM_PER_UPDATE)
		{	
			sView.m_nDataIndex = i + 1;
			break;
		}
	}
	
	if(nDataIndex == 0)
	{	
		i = m_ItemList.m_UseIdx.GetNext(0);
		while(i)
		{	
			j = i;
			i = m_ItemList.m_UseIdx.GetNext(i);
			if(m_ItemList.m_Items[j].nPlace != pos_equiproom)
				continue;
			if(Item[m_ItemList.m_Items[j].nIdx].GetPlayerShopPrice() == 0)
				continue;
			
			nCount++;	
		}
		sView.m_btCount = nCount;
	}
	else
	{
		sView.m_btCount = 0;
	}
	if(sView.m_sInfo[0].m_ID == 0)
		m_cShop.SetAdvText(" (B�n h�t) ");

	g_pServer->PackDataToClient(Player[nTargetPlayer].m_nNetConnectIdx, (BYTE*)&sView, sizeof(sView));
}
#endif

#ifdef _SERVER

//	

void	KPlayer::BuyPlayerShop(BYTE* pProtocol)
{	
	BUY_PLAYER_SHOP_COMMAND* pBuy = (BUY_PLAYER_SHOP_COMMAND*)pProtocol;
	
	int nDestPlayer = FindAroundPlayer(pBuy->m_dwNpcID);
	if(nDestPlayer <= 0 || nDestPlayer >= MAX_PLAYER)
		return;

	if(Player[nDestPlayer].CheckTrading())
		return;
	if(!Player[nDestPlayer].m_cShop.GetState())
		return;
	
	int nIdx = ItemSet.SearchID(pBuy->m_dwItemID);
	if(nIdx <= 0 || nIdx >= MAX_ITEM)
		return;
	
	int nPrice = Item[nIdx].GetPlayerShopPrice();
	if(this->m_ItemList.GetEquipmentMoney() < nPrice)
		return;
	
	BYTE btWidth = Item[nIdx].GetWidth();
	BYTE btHeight = Item[nIdx].GetHeight();
	ItemPos	Pos;
	if (!this->m_ItemList.SearchPosition((int)btWidth, (int)btHeight, &Pos))
		return;
	if (Pos.nPlace != pos_equiproom)
		return;
	if(!Pay(nPrice))
		return;


	Item[nIdx].SetPlayerShopPrice(0);
	Player[nDestPlayer].Earn(nPrice);
	Player[nDestPlayer].m_ItemList.Remove(nIdx);

	this->m_ItemList.Add(nIdx, Pos.nPlace, Pos.nX, Pos.nY);

	this->m_uMustSave = SAVE_REQUEST;
	Save();
	Player[nDestPlayer].m_uMustSave = SAVE_REQUEST;
	Player[nDestPlayer].Save();

	ITEM_CHANGE_INFO sChange;
	sChange.ProtocolType	= s2c_itemchangeinfosync;
	sChange.m_btType		= 6;
	sChange.m_dwItemID		= pBuy->m_dwItemID;
	sChange.m_uChange		= 0;
	if (g_pServer)
		g_pServer->PackDataToClient(this->m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
}
#endif

#ifdef _SERVER

//

void	KPlayer::RestoreTask()
{
	if(PlayerSet.m_nResetTaskCount <= 0)
		return;
	if(!PlayerSet.m_pResetTask)
		return;
	short i;
	for(i = 0; i < PlayerSet.m_nResetTaskCount; i++)
	{	
		if(PlayerSet.m_pResetTask[i].m_bSync)
		{
			this->TaskSetSaveVal(PlayerSet.m_pResetTask[i].m_nID, 0);
		}
		else
		{
			this->m_cTask.SetSaveVal(PlayerSet.m_pResetTask[i].m_nID, 0);
		}
	}
}
#endif



#ifdef _SERVER

//

BOOL 	KPlayer::ExecuteScriptRet(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) 
		return FALSE;
	return ExecuteScriptRet(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}
#endif

#ifdef _SERVER

//

BOOL 	KPlayer::ExecuteScriptRet(DWORD dwScriptId,  char * szFunName, int nParam)
{	
	try
	{	
		bool bExecuteScriptMistake = true;
		BOOL  bRet = 0;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{	
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);

			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if(pScript->CallFunction(szFunName, 1, "d", nParam))
			{
				bExecuteScriptMistake = false;
			} 
			bRet = (BOOL)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			pScript->SafeCallEnd(nTopIndex);
			
		}
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return bRet;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]\n", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::ExecuteScriptRet(char * ScriptFileName, char * szFunName, char *  szParams)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScriptRet(dwScriptId, szFunName, szParams);
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayer::ExecuteScriptRet(DWORD dwScriptId, char * szFunName, char *  szParams)
{
	try
	{	
		bool bExecuteScriptMistake = false;
		BOOL  bRet = 0;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			int nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);
			if ( (!szParams) || !szParams[0]) 
			{
				if(pScript->CallFunction(szFunName, 0, ""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if(pScript->CallFunction(szFunName, 0, "sd", szParams, 0))
				{
					bExecuteScriptMistake = false;
				}
			}
			bRet = (BOOL)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return bRet;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}
#endif





#ifdef _SERVER

//	

void	KPlayer::SetTitle(WORD wTitleId, bool bLogin /*= false*/)
{	
	int nRow, nValue1;
	char szString[32];
	sprintf(szString, "%d", wTitleId);
	nRow = g_TitleTabSetting.FindRow(szString, 1);
	if(nRow > 0)
	{
		g_TitleTabSetting.GetInteger(nRow, 3, 0, &nValue1);
		Npc[m_nIndex].SetTitleEffect(nValue1);
	}
	else
	{
		Npc[m_nIndex].SetTitleEffect(0);
	}
	if(!bLogin)
	{
		if(m_wTitleId == wTitleId)
			return;
	}
	int i, nValue2;
	
	if(!bLogin)
	{
		// Xoa hieu ung danh hieu cu
		sprintf(szString, "%d", m_wTitleId);
		nRow = g_TitleTabSetting.FindRow(szString, 1);
		if(nRow > 0)
		{
			for(i = 0; i < 5; i++)
			{
				sprintf(szString, "ExtSkill%d", i + 1);
				g_TitleTabSetting.GetInteger(nRow, szString, 0, &nValue1);
				if(nValue1 > 0)
				{
					if(Npc[m_nIndex].m_SkillList.FindSame(nValue1))
					{
						Npc[m_nIndex].m_SkillList.Remove(nValue1);
						PLAYER_SKILL_REMOVE_SYNC sRemove;
						sRemove.ProtocolType = s2c_playerskillremove;
						sRemove.m_nSkillID = nValue1;
						g_pServer->PackDataToClient(m_nNetConnectIdx, &sRemove, sizeof(PLAYER_SKILL_REMOVE_SYNC));
						RefeshPlayer(TRUE);
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	// Them hieu ung danh hieu moi
	sprintf(szString, "%d", wTitleId);
	nRow = g_TitleTabSetting.FindRow(szString, 1);
	if(wTitleId > 0)
	{
		if(nRow > 0)
		{	
			for(i = 0; i < 5; i++)
			{
				sprintf(szString, "ExtSkill%d", i + 1);
				g_TitleTabSetting.GetInteger(nRow, szString, 0, &nValue1);
				sprintf(szString, "ExtSkillLevel%d", i + 1);
				g_TitleTabSetting.GetInteger(nRow, szString, 0, &nValue2);
				if(nValue1 > 0 && nValue2 > 0)
				{
					if(Npc[m_nIndex].m_SkillList.Add(nValue1, nValue2, true))
					{	
						PLAYER_SKILL_LEVEL_SYNC NewSkill;
						NewSkill.ProtocolType	= s2c_playerskilllevel;
						NewSkill.m_nSkillID		= nValue1;
						NewSkill.m_nSkillLevel	= nValue2;
						NewSkill.m_nLeavePoint	= m_nSkillPoint;
						NewSkill.m_btPointAdd	= 0;
						NewSkill.m_bExpand		= true;
						g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
						RefeshPlayer(TRUE);
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	if(!bLogin)
		m_wTitleId = wTitleId;
}
#endif

#ifndef _SERVER
#define defMAX_SHOW_EXP_TIME			120

//

void	KPlayer::SetExpAddNo(int nNo)
{
	
	if(nNo == 0)
		return;
	
	if(nNo > 0)
		sprintf(m_szExpAddNo, "a%d", nNo);
	else
		sprintf(m_szExpAddNo, "d%d", -nNo);
	m_btExpAddTime	= defMAX_SHOW_EXP_TIME;
	m_btExpAddAlpha	= 255;
	
}
#endif



#ifdef _SERVER

//

int KPlayer::GetTaxShopItem(int nPrice)
{	
	int nRet = 0;
	if(nPrice <= 0)
		return nRet;

	int nSubWorldIdx = Npc[m_nIndex].m_SubWorldIndex;
	if(nSubWorldIdx < 0 || nSubWorldIdx >= MAX_SUBWORLD)
		return nRet;
	
	nRet = nPrice;
	

	return nRet;
}
#endif

#ifndef _SERVER

//

void KPlayer::ClientSystemMsg(const char* lpszName, const char* lpszMsg)
{
	l_pDataChangedNotifyFunc->ChannelMessageArrival(0, (char*)lpszName, lpszMsg, strlen(lpszMsg), TRUE);
}
#endif

#ifdef _SERVER
BOOL KPlayer::AddAntiPaket(DWORD idPaket, DWORD timePaket)
{
	int i = 0;
	for(i = 0; i < MAX_PACKER; i++){

		if (m_nArrayPacket[i][0] == 0) break;

		if (m_nArrayPacket[i][0] == idPaket){
			if (m_nArrayPacket[i][1] == timePaket) return FALSE;
			else break;

		}
	}

	if (i == MAX_PACKER - 1){
		memset(&m_nArrayPacket, 0,sizeof(m_nArrayPacket));
		i =0;
	}

	m_nArrayPacket[i][0] = idPaket;
	m_nArrayPacket[i][1] = timePaket;

	return TRUE;
}

//

/*void KPlayer::InitLog()
{
	char szPathName[MAX_PATH];
	char szPathPlayer[80];
	//
	GetCurrentDirectory(MAX_PATH, szPathName);
	sprintf(szPathPlayer, "\\server_log");
	strcat(szPathName, szPathPlayer);
	CreateDirectory(szPathName, NULL);
	//
	sprintf(szPathPlayer, "\\%s", m_AccoutName);
	strcat(szPathName, szPathPlayer);
	CreateDirectory(szPathName, NULL);
	//
	sprintf(szPathPlayer, "\\%u", m_dwID);
	strcat(szPathName, szPathPlayer);
	CreateDirectory(szPathName, NULL);
	//
	sprintf(szPathName, "\\server_log\\%s\\%u\\common", m_AccoutName, m_dwID);
	m_cBaseLog.InitWithDate(szPathName,"log", TRUE);
	//
	sprintf(szPathName, "\\server_log\\%s\\%u\\item", m_AccoutName, m_dwID);
	m_cItemLog.InitWithDate(szPathName,"log", TRUE);
	//
	sprintf(szPathName, "\\server_log\\%s\\%u\\script", m_AccoutName, m_dwID);
	m_cScriptLog.InitWithDate(szPathName,"log", TRUE);
}*/


//

void KPlayer::SaveLog(int nType, const char* lpszData, const char* lpszDesc, const char* lpszValue)
{
	// char szLog[512];
	// sprintf(szLog, "[%s] %s [%s] %s\n", GetPlayerName(), lpszDesc, lpszValue, lpszData);
	// switch(nType)
	// {
	// case 1:
		// m_cBaseLog.puts_t(szLog);
		// break;
	// case 2:
		// m_cItemLog.puts_t(szLog);
		// break;
	// case 3:
		// m_cScriptLog.puts_t(szLog);
		// break;
	// default:
		// break;
	// }
}
#endif

// Auto by kinnox;
void KPlayer::SetAutoFlag(BOOL nbAuto)
{		
	m_bActiveAuto = nbAuto;
#ifdef _SERVER
	S2CPLAYER_REQUEST_AUTO Cmd;
	Cmd.ProtocolType = s2c_autoplay;
	Cmd.m_bAuto = 1;
	Cmd.m_bActive = nbAuto;
	this->m_cTask.SetSaveVal(TV_ACTIVE_AUTO, nbAuto);
	if(g_pServer && this->m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2CPLAYER_REQUEST_AUTO));
#endif
}

#ifdef _SERVER
void KPlayer::LockPlayerInfo(BYTE bType,int nNumber1, int nNumber2)
{
	switch(bType)
	{
		case CREATE_PW:
			if (m_nLockPlayer != 0)
			{
				break;
			}

			else
			{
				m_nLockPlayer = nNumber1;
				Player[m_nPlayerIndex].m_cTask.SetSaveVal(TV_PWBOX, m_nLockPlayer);
				m_bLockPlayer = FALSE;		
				S2C_LOCKPLAYER Cmd;
				Cmd.ProtocolType = s2c_lockplayer;
				Cmd.m_bLockPlayer = m_bLockPlayer;
				Cmd.m_bPass = TRUE;
				if(g_pServer && this->m_nNetConnectIdx != -1)
					g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_CREATE_LOCK_SUCCESS;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);	
			}
		break;
		case LOGIN_PW:
			if (m_nLockPlayer == 0 || m_nLockPlayer != nNumber1 || nNumber1 > 999999 || nNumber2 > 999999)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_UN_LOCK_FAIL;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);	
				break;
			}
			else
			{	
				m_bLockPlayer = FALSE;
				S2C_LOCKPLAYER Cmd;
				Cmd.ProtocolType = s2c_lockplayer;
				Cmd.m_bLockPlayer = m_bLockPlayer;
				Cmd.m_bPass = TRUE;
				if(g_pServer && this->m_nNetConnectIdx != -1)
					g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_UN_LOCK_SUCCESS;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);		
			};
		break;
		case CHANGE_PW:
			if (m_nLockPlayer == 0 || m_nLockPlayer != nNumber1 || m_bLockPlayer || nNumber1 > 999999 || nNumber2 > 999999)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_CHANGE_LOCK_FAIL;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);	
				break;
			}
			else
			{	
				m_nLockPlayer = nNumber2;
				Player[m_nPlayerIndex].m_cTask.SetSaveVal(TV_PWBOX, m_nLockPlayer);				
				m_bLockPlayer = FALSE;
				S2C_LOCKPLAYER Cmd;
				Cmd.ProtocolType = s2c_lockplayer;
				Cmd.m_bLockPlayer = m_bLockPlayer;
				Cmd.m_bPass = TRUE;
				if(g_pServer && this->m_nNetConnectIdx != -1)
					g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_CHANGE_LOCK_SUCCESS;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);			
			};
		break; 
		case LOCK_PW:
		{	if (m_nLockPlayer == 0)
				break;					
			m_bLockPlayer = nNumber1;
			S2C_LOCKPLAYER Cmd;
			Cmd.ProtocolType = s2c_lockplayer;
			Cmd.m_bLockPlayer = m_bLockPlayer;
			Cmd.m_bPass = TRUE;
			if(g_pServer && this->m_nNetConnectIdx != -1)
				g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_ACTIVE_LOCK;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);		
		}
		break;
		case DEL_PW:
		{	
			m_nLockPlayer = 0;
			m_bLockPlayer = FALSE;
			S2C_LOCKPLAYER Cmd;
			Cmd.ProtocolType = s2c_lockplayer;
			Cmd.m_bLockPlayer = FALSE;
			Cmd.m_bPass = FALSE;
			if(g_pServer && this->m_nNetConnectIdx != -1)
				g_pServer->PackDataToClient(this->m_nNetConnectIdx, &Cmd, sizeof(S2C_LOCKPLAYER));
		}
		break;
		// default:
		// break;
	}
}
#endif


//load 3 storebox by kinnox;
void KPlayer::SetStoreBoxNum(BYTE nNum)
{
	m_btRepositoryCount = nNum;
#ifdef _SERVER
	REPOSIPLAYER_SYNC	sSync;	
	sSync.ProtocolType = s2c_syncreposiplayer;
	sSync.m_nNum = nNum;
	if (g_pServer)
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(REPOSIPLAYER_SYNC));
#endif
}
//end code

//Expandbox by kinnox;
void KPlayer::SetExpandBoxNum(BYTE nNum)
{
	m_btExpandtoryCount = nNum;
#ifdef _SERVER
	EXPANDPLAYER_SYNC	sSync;	
	sSync.ProtocolType = s2c_syncexpandplayer;
	sSync.m_nNum = nNum;
	if (g_pServer)
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(EXPANDPLAYER_SYNC));
#endif
}

#ifdef _SERVER
void KPlayer::SetNumImgAvatar(int ID)//Player Avatar by kinnox;
{
	if(ID>0 && ID < 28)
		m_ImagePlayer=ID;
}
void KPlayer::GiveBoxCmd(DWORD dwID, int nX, int nY)//GiveBox by kinnox; //TrembleItem by kinnox;
{
	if (CheckTrading())
		return;

	int nIndex = m_ItemList.SearchID(dwID);
		
	if (nIndex <= 0)
		return ;
	
	m_ItemList.Remove(nIndex);
	
	int		x, y;
	if (m_ItemList.CheckCanPlaceInEquipment(nX, nY, &x, &y))
	{
		m_ItemList.Add(nIndex, pos_equiproom, x, y);
	}		
	else
	{
		int	nIdx = m_ItemList.Hand();
		if (nIdx)
		{
			m_ItemList.Remove(nIdx);		
			KMapPos sMapPos;
			KObjItemInfo	sInfo;
			char			szNameTemp[80];
			GetAboutPos(&sMapPos);
				
			sInfo.m_nItemID = nIdx;
			sInfo.m_dwItemID = Item[nIdx].GetID();
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			char szTemp[FILE_NAME_LENGTH];
			sprintf(szTemp, "%s x%d", Item[nIdx].GetName(), Item[nIdx].GetStackCount());
			strcpy(sInfo.m_szName, szTemp);
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code
				
			int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
			if (nObj == -1)
			{
				ItemSet.Remove(nIdx);
			}
			else
			{
				if (Item[nIdx].GetGenre() == item_task)
					Object[nObj].SetEntireBelong(m_nPlayerIndex);
				else
					Object[nObj].SetItemBelong(m_nPlayerIndex);
			}
		}
		m_ItemList.Add(nIndex, pos_hand, 0, 0);
	}
}

#endif
