//Author: AutoAI by kinnox
//Date: 29/04/2021
#include	"KCore.h"
#include	"KNpc.h"
#include	"KNpcFindPath.h"
#include	"KNpcGold.h"
#include	"KPlayer.h"
#include	"KBuySell.h"
#include 	"ImgRef.h"
#include	"KPlayerDef.h"
#include	"KPlayerAI.h"
#include	"KSubWorldSet.h"
#include 	"KMath.h"

#ifndef _SERVER
#include	"CoreShell.h"
#include	"KPlayerPK.h"

// External functions
extern void SendClientCmdRide(BOOL bFlagRide);
// Note: TIME_RIDE is defined in GameDataDef.h (currently 1000ms = 1 second)

int KEquipmentArray::FindSame(int i)
{
	int nIdx = 0;
	while(1)
	{
		nIdx = m_Link.GetNext(nIdx);
		if (!nIdx)
			break;

		if (nIdx == i)
			return nIdx;
	}
	return 0;
}

void KPlayerAI::Init()
{
	Release();
}
void KPlayerAI::Release()
{
	m_nStartAIX			=	0; 
	m_nStartAIY			=	0; 
	m_bPriorityUseMouse 	= FALSE;
	// m_nTimePriorityUseMouse = 0;
	m_RadiusAuto 			= 0;
	m_DistanceAuto 			= 0;
	m_PosXAuto 				= 0;
	m_PosYAuto 				= 0;
	m_bActacker 			= FALSE;
	m_bAttackPeople			= FALSE;
	m_Actacker 				= 0;
	m_Count_Acttack_Lag 	= 0;
	m_nLifeLag			    = 0;
	m_nTimeRunLag 			= 0;
	m_nTimerMove			= 0;
	m_bAttackAround 		= FALSE;
	m_bAroundBtn 			= FALSE;
	m_bFollowPeople 		= FALSE;
	m_FollowPeopleName[0] 	= 0;
	m_FollowPeopleIdx 		= 0;
	m_nRadiusFollow			= 0;
	m_nLeaderCurrentTarget	= 0;
	m_nLeaderTargetLostTime	= 0;	
	m_bAutoAttack 			= TRUE;	
	m_bFollowAttack			= FALSE;
	m_SpaceBar 				= FALSE;
	m_HoldSpaceBar 			= FALSE;
	m_bAttackNpc 			= FALSE;
	m_bRiderHorse 			= 0;
	m_nDefense 				= 0;
	m_nFightBoss			= 0;
	m_MoveStep 				= 0;
	m_MoveCount				= 0;
	m_AutoMove 				= FALSE;
//
	m_EatLife 				= FALSE;
	m_EatMana 				= FALSE;
	m_LifeTimeUse 			= 0;
	m_ManaTimeUse 			= 0;
	m_LifeAutoV 			= 0;
	m_ManaAutoV 			= 0;
	m_LifeCountDown			= 0;
	m_ManaCountDown 		= 0;
	m_PortalCountDown 		= 0;
	m_TPLife 				= FALSE;
	m_TPLifeV 				= 0;
	m_TPMana 				= FALSE;
	m_TPManaV 				= 0;
	m_TPNotMedicineBlood 	= FALSE;
	m_TPNotMedicineMana 	= FALSE;
	m_TPHightMoney 			= FALSE;
	m_TPHightMoneyV 		= 0;
	m_btDurabilityItem 		= FALSE;
	m_btDurabilityValue 	= 0;
	m_bTPNotEquipment		= FALSE;
	m_nTPNotEquipmentV		= 0;
	m_AutoAntiPoison 		= FALSE;
	m_AntiPoisonCountDown 	= 0;
	m_AutoTTL				= FALSE;
	m_AutoBoodBag			= FALSE;
	m_bRepairEQuip			= FALSE;
	m_bSaveMoney			= FALSE;
	m_nMoney				= 0;
	m_AutoLifeReplenish 	= FALSE;
	m_AutoLifeReplenishP 	= 0;
	m_EatFullBox			= FALSE;
	m_EatFullBoxCountDown	= 0;
	m_nWarningPK			= FALSE;
	m_nNextItem 			= itempart_head;
	m_RepairTime			= 0;
//
	m_nLeftSkillActiving	= 0;
	m_nOldRightHandActiving	= 0;

	m_bAutoParty			= FALSE;
	m_bAutoInvite 			= FALSE;
	m_bAutoInviteList 		= FALSE;
	m_bAutoAccept 			= FALSE;
	m_bAutoAcceptAll 		= FALSE;
	m_bAutoAcceptWithName	= FALSE;

	m_bPickItem 			= FALSE;
	m_btPickUpKind 			= enumPickUpEarn;
	m_SaveRAP 				= FALSE;
	m_bObject 				= FALSE;
	m_nObject 				= 0;
	m_bFilterEquipment 		= FALSE;
	m_bPiceItem				= FALSE;
	m_nPiceItem				= 0;
	m_bLevelItem			= FALSE;
	m_nLevelItem			= 0;	
	m_BackToTownPor			= FALSE;
	m_nTimeSetBack			= 0;
	m_nTimeBacking			= 0;
	m_nTimeMove				= 0;
	bPlayerSellItem			= FALSE;
	bPlayerMoveItem			= FALSE;
	bCheckFilter			= FALSE;
	bCheckSellItem			= FALSE;
	m_nTimeSkip				= 0;
	nTickAttack				= 0;
	m_MoveRevese			= 0;
	b_MoveRevese			= FALSE;
	m_BackMapTrain			= 0;
	nNpcIDPaint				= 0;
	nDestXPaint				= 0;
	nDestYPaint				= 0;
	memset(m_ArrayStateSkill,0,sizeof(m_ArrayStateSkill));
	memset(m_AuraSkill,0,sizeof(m_AuraSkill));
	memset(m_nLeftSkillSet,0,sizeof(m_nLeftSkillSet));
	memset(m_nRightHandSkillSet,0,sizeof(m_nRightHandSkillSet));
	memset(m_MoveMps, 0, sizeof(m_MoveMps));
	memset(m_FilterMagic, 0, sizeof(m_FilterMagic));
	ClearArrayNpcLag();
	ClearArrayTimeNpcLag();
	ClearArrayInvitePlayer();
	ClearArrayTimeInvitePlayer();	
	ClearArrayObjectNeast();
	ClearArrayObjectLag();
	ClearArrayTimeObjectLag();
	m_sListEquipment.m_Link.Init(MAX_EQUIPMENT_ITEM);

}

void KPlayerAI::ResetAuto()
{
	m_nStartAIX			=	0;	
	m_nStartAIY			=	0; 
	m_nTimeSkip			= 	0;		
	m_nTimeBacking		= 	0;
	m_nTimeBackToMapTrain =	0;
	nNpcIDPaint			= 0;
	nDestXPaint			= 0;
	nDestYPaint			= 0;
	for (int i = MAX_EQUIPMENT_ITEM - 1; i > 0 ; i--)
	{
		if (m_sListEquipment.FindSame(i))
			m_sListEquipment.m_Link.Remove(i);
	}	
	ClearArrayNpcLag();
	ClearArrayTimeNpcLag();
	ClearArrayObjectNeast();
	ClearArrayObjectLag();
	ClearArrayTimeObjectLag();
}

void KPlayerAI::SendAroundAuto(BOOL bUp)
{
	if (m_bAroundBtn)
	{
		if (bUp)
		{
			
			int nDesX = 0, nDesY = 0;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nDesX, &nDesY);
			int nDesTempX = nDesX/8/32;
			int nDesTempY = nDesY/16/32;
			int nDesTempXAuto = m_PosXAuto/8/32;
			int nDesTempYAuto = m_PosYAuto/16/32;
			if (nDesTempX != nDesTempXAuto && 
				nDesTempY != nDesTempYAuto)
			{
				m_PosXAuto = nDesX;
				m_PosYAuto = nDesY;
			}
		}
		m_bAttackAround = bUp;
	}
	else
	{
		m_bAttackAround = 0;
	}
	
}

void KPlayerAI::Active()
{
	int index = 0;
	int	iObject = 0;
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop > 0)
		return;
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
	{ 
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&m_nStartAIX,&m_nStartAIY); // save coordinates player when start auto
		///Fight and NotFight
		PlayerSwitchAura(); // Change Auto with time changed

		/*if (PlayerEatAItem()) // Eat Blood, mana,antiposi, townportant, x2,....
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
				return;
		}*/
		PlayerEatAItem(); // Eat Blood, mana,antiposi, townportant, x2,....

		if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() != 2)
			EatFullBox();//Eat Blood or mana if full box 

		RepairEquip();// Repair Equip or item
		if (m_bFilterEquipment) //Filter Item Equipment
		{
			if (g_SubWorldSet.GetGameTime() % 16 == 0)
			PlayerFilterEquip();
		}
		// 
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode) // trang thai chien dau
		{
			if (g_SubWorldSet.GetGameTime() % 4 == 0)
				CheckPlayerRider(); // check rider on Horse 

			//if (g_SubWorldSet.GetGameTime() % 4 == 0)
				AutoBuffSkillState();//buff skills;

			PlayerBuffWhenManaSmall();//change skills when mana small;
			PlayerActiveFightHand(); // Set Right Hand Skill;
			if (AutoBuffEmi() == TRUE)
			return;
			m_nTimeBacking = 0; // Reset Time Back To TownPort;				
			m_nTimeBackToMapTrain = 0; // Reset Time Back To Map Train;				
			if (m_nWarningPK)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetWarning(1);	
			else if(!m_nWarningPK)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetWarning(0);
			int i = 0;
			int nCountNpcLag = 0;
			int nCountObjectLag = 0;
			for (i = 0; i < defMAX_ARRAY_AUTO; i++)
			{
				if (m_ArrayNpcLag[i] > 0)
				{
					nCountNpcLag++;
					if (Npc[m_ArrayNpcLag[i]].m_Kind == kind_player)
					{
						if (GetTickCount() - m_ArrayTimeNpcLag[i] > defAUTO_TIME_RESET_LAG/20)
						{
							m_ArrayNpcLag[i] = 0;
							m_ArrayTimeNpcLag[i] = 0;
							// m_nCountTimerBuff = 0;
						}
					}
					else
					{
						if (GetTickCount() - m_ArrayTimeNpcLag[i] > defAUTO_TIME_RESET_LAG)
						{
							m_ArrayNpcLag[i] = 0;
							m_ArrayTimeNpcLag[i] = 0;
							// m_nCountTimerBuff = 0;
						}
					}
				}
				
				if (m_ArrayObjectLag[i] > 0)
				{
					nCountObjectLag++;
					if (GetTickCount() - m_ArrayTimeObjectLag[i] > defAUTO_TIME_RESET_LAG)
					{
						m_ArrayObjectLag[i] = 0;
						m_ArrayTimeObjectLag[i] = 0;
					}
				}		
		
			}
			if (nCountNpcLag >= defMAX_ARRAY_AUTO)
			{
				ClearArrayNpcLag();
				ClearArrayTimeNpcLag();
			}

			if (nCountObjectLag >= defMAX_ARRAY_AUTO)
			{
				ClearArrayObjectLag();
				ClearArrayTimeObjectLag();
			}

			if (m_bAutoParty && m_bAutoInvite)
			{
				if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				{
					Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();
					ClearArrayInvitePlayer();
					ClearArrayTimeInvitePlayer();
				}
				else
					InviteParty();
			}

			if (m_bFollowPeople == TRUE && m_FollowPeopleName[0])
			{
				int	nRegionNo, i, j;

				// LAYER 1: Search in current region
				m_FollowPeopleIdx = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].SearchNpcName(m_FollowPeopleName);

				// LAYER 2: Search in 8 adjacent regions (1st layer neighbors)
				if(m_FollowPeopleIdx <= 0)
				{
					for (i = 0; i < 8; i++)
					{
						nRegionNo = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].m_nConnectRegion[i];
						if ( nRegionNo < 0)
							continue;
						m_FollowPeopleIdx = SubWorld[0].m_Region[nRegionNo].SearchNpcName(m_FollowPeopleName);
						if (m_FollowPeopleIdx > 0)
							break;
					}
				}

				// LAYER 3: Search in 2nd layer neighbors (regions adjacent to 1st layer)
				// This expands search area to ~24 additional regions
				if(m_FollowPeopleIdx <= 0)
				{
					for (i = 0; i < 8; i++)
					{
						int nFirstLayerRegion = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].m_nConnectRegion[i];
						if (nFirstLayerRegion < 0)
							continue;

						// Search in neighbors of this 1st layer region
						for (j = 0; j < 8; j++)
						{
							nRegionNo = SubWorld[0].m_Region[nFirstLayerRegion].m_nConnectRegion[j];
							if (nRegionNo < 0)
								continue;
							m_FollowPeopleIdx = SubWorld[0].m_Region[nRegionNo].SearchNpcName(m_FollowPeopleName);
							if (m_FollowPeopleIdx > 0)
								break;
						}
						if (m_FollowPeopleIdx > 0)
							break;
					}
				}

				if(m_FollowPeopleIdx <= 0)
				{
					m_FollowPeopleIdx = NpcSet.SearchName(m_FollowPeopleName);
				}

				if (m_FollowPeopleIdx)
				{
					// FIX: Track leader's current attack target for coordinated combat with grace period
					// When following, follower should attack the same target as leader
					if (Npc[m_FollowPeopleIdx].m_nPeopleIdx > 0 &&
						Npc[m_FollowPeopleIdx].m_Index > 0)
					{
						// Leader is attacking someone - update our target tracking
						int newTarget = Npc[m_FollowPeopleIdx].m_nPeopleIdx;
						if (m_nLeaderCurrentTarget != newTarget)
						{
							// Leader switched to a new target
							m_nLeaderCurrentTarget = newTarget;
							m_nLeaderTargetLostTime = 0;  // Reset grace period timer
						}
					}
					else if (m_nLeaderCurrentTarget > 0)
					{
						// Leader not attacking - clear target tracking
						// Leader stopped attacking - start grace period
						// Don't clear target immediately, give leader time to pick next target
						if (m_nLeaderTargetLostTime == 0)
						{
							m_nLeaderTargetLostTime = GetTickCount();
						}
						else
						{
							// Check if grace period expired (2 seconds)
							unsigned int elapsed = GetTickCount() - m_nLeaderTargetLostTime;
							if (elapsed > 4000)  // 2 second grace period
							{
								// Grace period expired, leader really has no target now
								m_nLeaderCurrentTarget = 0;
								m_nLeaderTargetLostTime = 0;
							}
						}
					}
					int distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, m_FollowPeopleIdx);

					// AUTO RIDE HORSE FEATURE: Auto mount when target is too far
					// Note: Dismounting is handled by other auto-play logic (some classes can attack while riding)
					if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_attack &&
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_magic &&
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_skill)
					{
						// If target moves beyond half radius → mount horse to catch up early
						// Example: radius=500 → mount at distance>250 (not 750!) to catch up in time
						if (distance > m_nRadiusFollow / 2)  // 0.5x radius (half)
						{
							if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
							{
								if (GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= TIME_RIDE)
								{
									Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse = GetTickCount();
									SendClientCmdRide(FALSE);  // FALSE = mount horse
								}
							}
						}
					}

					if (distance >= m_nRadiusFollow)
					{
						// FIX: Don't reset combat if attacking leader's target
						// Check if we're currently attacking leader's target
						BOOL isAttackingLeaderTarget = (m_nLeaderCurrentTarget > 0 &&
						                                 m_Actacker == m_nLeaderCurrentTarget &&
						                                 m_bActacker == TRUE);

						// If attacking leader's target, allow combat to continue even when far from leader
						// This prevents breaking combat coordination when chasing enemies
						if (!isAttackingLeaderTarget)
						{
						// Not attacking leader's target, move back to leader
							int nX, nY;
							Npc[m_FollowPeopleIdx].GetMpsPos(&nX,&nY);
							if (!m_bPriorityUseMouse)
								MoveTo(nX, nY);
							m_Actacker = 0;
							m_bActacker = FALSE;
							m_nLifeLag = 0;
							m_nTimeRunLag = 0;
							m_nTimeSkip = 0;
							m_Count_Acttack_Lag = 0;
							m_nObject = 0;
							m_bObject = FALSE;
							return;
						}
						// Else: Continue attacking leader's target even if far from leader
					}
				}

			}

			//////////FOllow pick object
			/*if (((m_bActacker == FALSE || m_Actacker == 0) && (m_bObject == FALSE || m_nObject == 0)) && 
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nObjectIdx == 0)*/
			if ((m_bObject == FALSE || m_nObject == 0) && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nObjectIdx == 0)
			{
				iObject = FindNearObject2Array();
				if (iObject > 0)
				{
					BOOL _flagLag = FALSE;
					for (i = 0; i < defMAX_ARRAY_AUTO; i++)
					{
						if (m_ArrayObjectLag[i] == iObject)
						{
							_flagLag = TRUE;
							break;
						}
					} 
				
					if (_flagLag)
					{
						iObject = 0; 
						m_nObject = 0;
						m_bObject = FALSE;
					}
					else
					{
						ItemPos	sItemPos;
						if(Object[iObject].m_nKind == Obj_Kind_Money || Object[iObject].m_nKind == Obj_Kind_Task)
						{
							m_bObject = TRUE;
							m_nObject = iObject;
						}
						else if (Object[iObject].m_nKind == Obj_Kind_Item)
						{
							if (Object[iObject].m_nGenre == item_task || Object[iObject].m_nGenre == item_script)
							{
								m_bObject = TRUE;
								m_nObject = iObject;
							}
							else
							{
								if (FALSE == Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(Object[iObject].m_nItemWidth, 
									Object[iObject].m_nItemHeight, &sItemPos) || sItemPos.nPlace != pos_equiproom)
								{
									m_bObject = FALSE;
									m_nObject = 0;
								}
								else
								{
									m_bObject = TRUE;
									m_nObject = iObject;
								}
							}	
						}
							
					}
				}
			}
			if (m_nObject > 0)
			{
				if (PlayerFollowObject(m_nObject))
					return;
			}
			m_bObject = FALSE;
			m_nObject = 0;	
			/////////


			if (m_bActacker == FALSE || m_Actacker == 0 || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_skill)
			{
				memset(m_ArrayNpcNeast,0,sizeof(m_ArrayNpcNeast));
				index = FindNearNpc2Array(relation_enemy);
				//Anti Lag Pos Attack Around by kinnox;
				/*int nMpsX,nMpsY;
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMpsX,&nMpsY);
				if (nMpsX == m_PosXAuto && nMpsY == m_PosYAuto)
					index = 0;*/
				
				// Anti Lag Pos Attack Around by kinnox (nới điều kiện)
				int nMpsX, nMpsY;
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMpsX, &nMpsY);

				// Cho phép đứng gần PosAuto trong bán kính ~2 cell (mps/32)
				const int CELL = 32;
				const int TOL_CELL = 2; // 2 cell
				int dxAuto = (nMpsX / CELL) - (m_PosXAuto / CELL);
				int dyAuto = (nMpsY / CELL) - (m_PosYAuto / CELL);
				if (dxAuto*dxAuto + dyAuto*dyAuto <= TOL_CELL*TOL_CELL)
				{
					// KHÔNG ép index = 0 nữa; để AI vẫn có thể pick target gần
					// index = 0;  // xoá dòng này
				}
				//
				if (index > 0)
				{
					BOOL _flagLag = FALSE;
					for (i = 0; i < defMAX_ARRAY_AUTO; i++)
					{
						if (m_ArrayNpcLag[i] == index)
						{
							_flagLag = TRUE;
							break;
						}
					}				
					if (_flagLag)
					{
						m_Actacker = 0;
						m_bActacker = FALSE;
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
						index = 0;
						if (m_AutoMove)
						{
							if (m_bPriorityUseMouse)
							return;
							 PlayerMoveMps();
						}
					}
					else
					{
						m_Actacker = index;
						m_bActacker = TRUE;
					}
				}
			}
			else
			{
				index = m_Actacker;
			}
			if (index > 0 && m_bAutoAttack == TRUE)
				{				
					PlayerFollowActack(index);
				}
			else
				m_bActacker = FALSE;
				if ( m_AutoMove && index == 0)
				{
					if (m_bPriorityUseMouse)
						return;
					PlayerMoveMps();	
				}
				

				
			}
			else // trang thai phi chien dau
			{
				m_nTimeSkip = 0;
				BackToMap();//Come back to maps train;	
				SaveMoney(); //Save Money when live country; 
				// delay code by kinnox;
				if (g_SubWorldSet.GetGameTime() % 16 == 0) 
				{			
					if (bPlayerMoveItem && bCheckFilter == FALSE)
					{
						if (bPlayerSellItem)
							bCheckSellItem = TRUE;
						
							MoveItemToBox();
							for (int i = MAX_EQUIPMENT_ITEM - 1; i > 0 ; i--)
							{
								m_sListEquipment.m_Link.Remove(i);
							}	
						
						if (bPlayerSellItem)
							bCheckSellItem = FALSE;
					}
				}
				if (bPlayerSellItem && bCheckSellItem == FALSE)
				{
					if (g_SubWorldSet.GetGameTime()% 16 == 0 ) // delay code by kinnox;
					CoreDataChanged(GDCNI_UPDATE_PLAYERSELLITEM, 0,0);
				}
				BackMapTrain();//Quay lai maps train sau khi chet;
				
			}
		
	}
	else
	{
		ResetAuto();
	}

}

int KPlayerAI::FindNearNpc2Array(int nRelation)
{
	int nRet = 0;
	// FIX: Priority 1 - If following someone, attack their target first (coordinated combat)
	// This ensures follower focuses on leader's target instead of randomly finding nearest NPC
	if (m_bFollowPeople && m_FollowPeopleIdx > 0 && m_nLeaderCurrentTarget > 0)
	{
		// Validate leader's target is still valid and attackable
		if (!IsNotValidNpc(m_nLeaderCurrentTarget) &&
		    Npc[m_nLeaderCurrentTarget].m_Index > 0 &&
		    Npc[m_nLeaderCurrentTarget].m_dwID > 0)
		{
			// Check if target is within reasonable range (2x follow radius)
			int distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, m_nLeaderCurrentTarget);
			if (distance <= m_nRadiusFollow * 2)
			{
				// Attack same target as leader (coordinated combat)
				// Reset grace period since target is still valid
				m_nLeaderTargetLostTime = 0;
				return m_nLeaderCurrentTarget;
			}
		}
		// Leader's target invalid/dead -> check if in grace period
		if (m_nLeaderTargetLostTime == 0)
		{
			// Just lost target, start grace period
			m_nLeaderTargetLostTime = GetTickCount();
		}
		unsigned int elapsed = GetTickCount() - m_nLeaderTargetLostTime;
		if (elapsed < 4000)  // Still in grace period (2 seconds)
		{
			// Don't find new target yet, wait for leader to pick next target
			// Return 0 to stay near leader
			return 0;
		}
		// Grace period expired, clear target and find new one below
		m_nLeaderCurrentTarget = 0;
		m_nLeaderTargetLostTime = 0;
	}
	// Original logic: Find nearest NPC when not following or leader has no target
	if (AutoAddNpc2Array(nRelation))
	{
		int distance = 0 ;
		int distanceMin = 0 ;
		BOOL _fg = FALSE;
		int i = 0;
		int j = 0;
		for (i=0; i < defMAX_ARRAY_AUTO; i++)
		{
			if (m_ArrayNpcNeast[i] != 0)
			{
				BOOL _findLag = FALSE;
				for (j = 0; j < defMAX_ARRAY_AUTO; j++)
				{
					if (m_ArrayNpcNeast[i] == m_ArrayNpcLag[j])
					{
						_findLag = TRUE;
						break;
					}
				}
				if (_findLag == TRUE)
					continue;	
				
				if (IsNotValidNpc(m_ArrayNpcNeast[i]))		
					continue;
				distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, m_ArrayNpcNeast[i]);
				if (FALSE == _fg)
				{
					distanceMin = distance;
					_fg = TRUE;
					nRet = m_ArrayNpcNeast[i];
				}
				if (distance <= distanceMin)
				{
					distanceMin = distance;
					nRet = m_ArrayNpcNeast[i];
				}
			}
		}
	}

	if (nRet == 0 && m_bAutoAttack == TRUE)
	{
		// FIX: When following leader and no target, stick to leader instead of wandering
		// This keeps follower ready to attack leader's next target
		if (m_bFollowPeople && m_FollowPeopleIdx > 0)
		{
			// Move closer to leader when idle (no target)
			int leaderDistance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, m_FollowPeopleIdx);
			if (leaderDistance > m_nRadiusFollow / 3)  // If not very close to leader
			{
				int nX, nY;
				Npc[m_FollowPeopleIdx].GetMpsPos(&nX, &nY);
				if (!m_bPriorityUseMouse)
					MoveTo(nX, nY);  // Move to leader's position
			}
		}
		else if (m_AutoMove)
		{
			PlayerMoveMps();  // Original behavior when not following
		}
		AutoReturn();
		m_Actacker = 0;
		m_bActacker = FALSE;
		// FIX: Don't clear player's target when following leader and in grace period
		// This prevents losing track of combat when waiting for leader's next target
		BOOL isInGracePeriod = (m_bFollowPeople && m_FollowPeopleIdx > 0 &&
		                         m_nLeaderTargetLostTime > 0 &&
		                         (GetTickCount() - m_nLeaderTargetLostTime) < 4000);
		if (!isInGracePeriod)
		{
			// Only clear target if not in grace period
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
		}
		// m_nTimeRunLag = 0;
		// m_Count_Acttack_Lag = 0;
	}
	return nRet;
}

/*BOOL KPlayerAI::AutoAddNpc2Array(int nRelation)
{
	int nRangeX = 0;
	int	nRangeY = 0;
	int	nSubWorldIdx = 0;
	int	nRegion = 0;
	int	nMapX = 0;
	int	nMapY = 0;

	nRangeX = m_RadiusAuto <= 100 ? 100 : m_RadiusAuto;
	nRangeY = m_RadiusAuto <= 100 ? 100 : m_RadiusAuto;

	nSubWorldIdx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex;
	nRegion = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex;
	nMapX = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapX;
	nMapY = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapY;
	int	nRet;
	int	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorldIdx].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorldIdx].m_nCellHeight;	

	for (int j = 0; j < defMAX_ARRAY_AUTO; j++)
	{
		m_ArrayNpcNeast[j] = 0;
	}

	for (int i = 0; i < nRangeX; i++)
	{
		for (int j = 0; j < nRangeY; j++)
		{
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
		
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, Player[CLIENT_PLAYER_INDEX].m_nIndex, nRelation);

			if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;

			if (nRet > 0)
			{				
				DefensePlayer(nRet);	
				int k = 0;
				if (m_bAttackPeople && Npc[nRet].m_Kind == kind_player)
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (Npc[nRet].m_Kind == kind_normal)
							m_ArrayNpcNeast[k] = 0;
					}

					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				else
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				if(k == (defMAX_ARRAY_AUTO))
					return TRUE;
			}
			
			nRMx = nMapX - i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
		
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, Player[CLIENT_PLAYER_INDEX].m_nIndex, nRelation);

			if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
			{
				DefensePlayer(nRet);	
				int k = 0;
				if (m_bAttackPeople && Npc[nRet].m_Kind == kind_player)
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (Npc[nRet].m_Kind == kind_normal)
							m_ArrayNpcNeast[k] = 0;
					}

					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				else
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				if(k == (defMAX_ARRAY_AUTO))
					return TRUE;
			}
			
			nRMx = nMapX - i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
		
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, Player[CLIENT_PLAYER_INDEX].m_nIndex, nRelation);
			
			if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
			{	
				DefensePlayer(nRet);	
				int k = 0;
				if (m_bAttackPeople && Npc[nRet].m_Kind == kind_player)
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (Npc[nRet].m_Kind == kind_normal)
							m_ArrayNpcNeast[k] = 0;
					}

					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				else
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				if(k == (defMAX_ARRAY_AUTO))
					return TRUE;
			}
			
			nRMx = nMapX + i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, Player[CLIENT_PLAYER_INDEX].m_nIndex, nRelation);
			
			if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
			{
				DefensePlayer(nRet);	
				int k = 0;
				if (m_bAttackPeople && Npc[nRet].m_Kind == kind_player)
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (Npc[nRet].m_Kind == kind_normal)
							m_ArrayNpcNeast[k] = 0;
					}

					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				else
				{
					for (k=0; k < (defMAX_ARRAY_AUTO); k++)
					{
						if (m_ArrayNpcNeast[k] == 0)
						{
							m_ArrayNpcNeast[k] = nRet;
							break;
						}
					}
				}
				if(k == (defMAX_ARRAY_AUTO))
					return TRUE;
			}
		}
	}
	return TRUE;
}*/
BOOL KPlayerAI::AutoAddNpc2Array(int nRelation)
{
    int nRangeX = 0;
    int nRangeY = 0;
    int nSubWorldIdx = 0;
    int nRegion = 0;
    int nMapX = 0;
    int nMapY = 0;

    // Ban kinh toi thieu 100
    nRangeX = (m_RadiusAuto <= 100) ? 100 : m_RadiusAuto;
    nRangeY = (m_RadiusAuto <= 100) ? 100 : m_RadiusAuto;

    // Lay thong tin nguoi choi hien tai
    int nPlayerNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
    nSubWorldIdx = Npc[nPlayerNpcIdx].m_SubWorldIndex;
    nRegion      = Npc[nPlayerNpcIdx].m_RegionIndex;
    nMapX        = Npc[nPlayerNpcIdx].m_MapX;
    nMapY        = Npc[nPlayerNpcIdx].m_MapY;

    int nRet;
    int nRMx, nRMy, nSearchRegion;

    // Chuyen ban kinh ra don vi cell (bao ve chia 0)
    if (SubWorld[nSubWorldIdx].m_nCellWidth  > 0) nRangeX = nRangeX / SubWorld[nSubWorldIdx].m_nCellWidth;
    if (SubWorld[nSubWorldIdx].m_nCellHeight > 0) nRangeY = nRangeY / SubWorld[nSubWorldIdx].m_nCellHeight;

    // Reset danh sach ung vien
    int ii;
    for (ii = 0; ii < defMAX_ARRAY_AUTO; ++ii)
        m_ArrayNpcNeast[ii] = 0;

    // ---- helper cuc bo bang vong lap (thu cong, khong lambda) ----
    // Kiem tra mang da day
    #define CHECK_ARRAY_FULL_AND_RETURN()             \
        do {                                          \
            BOOL bFull = TRUE;                        \
            int __f;                                  \
            for (__f = 0; __f < defMAX_ARRAY_AUTO; ++__f) { \
                if (m_ArrayNpcNeast[__f] == 0) {      \
                    bFull = FALSE;                    \
                    break;                            \
                }                                     \
            }                                         \
            if (bFull) return TRUE;                   \
        } while (0)

    // Kiem tra da ton tai id trong mang chua
    #define ARRAY_CONTAINS(_id, _outHas)              \
        do {                                          \
            _outHas = FALSE;                          \
            int __t;                                  \
            for (__t = 0; __t < defMAX_ARRAY_AUTO; ++__t) { \
                if (m_ArrayNpcNeast[__t] == (_id)) {  \
                    _outHas = TRUE;                   \
                    break;                            \
                }                                     \
            }                                         \
        } while (0)

    // Clear het quai thuong ra khoi danh sach (uu tien Player khi PvP)
    #define CLEAR_NORMALS_FOR_PVP_PRIORITY()          \
        do {                                          \
            int __c;                                  \
            for (__c = 0; __c < defMAX_ARRAY_AUTO; ++__c) { \
                int __id = m_ArrayNpcNeast[__c];      \
                if (__id > 0 && Npc[__id].m_Kind == kind_normal) \
                    m_ArrayNpcNeast[__c] = 0;         \
            }                                         \
        } while (0)

    // Thu push 1 candidate vao danh sach (uu tien Player neu PvP), tranh trung ID
    #define TRY_PUSH_CANDIDATE(_cand)                                 \
        do {                                                          \
            if ((_cand) <= 0) break;                                  \
            if (IsNotValidNpc((_cand))) break;                        \
            BOOL __has; ARRAY_CONTAINS((_cand), __has);               \
            if (__has) break;                                         \
                                                                      \
            if (m_bAttackPeople && Npc[(_cand)].m_Kind == kind_player) { \
                CLEAR_NORMALS_FOR_PVP_PRIORITY();                     \
                int __k;                                              \
                for (__k = 0; __k < defMAX_ARRAY_AUTO; ++__k) {       \
                    if (m_ArrayNpcNeast[__k] == 0) {                  \
                        m_ArrayNpcNeast[__k] = (_cand);               \
                        break;                                        \
                    }                                                 \
                }                                                     \
            } else {                                                  \
                if (Npc[(_cand)].m_Kind == kind_normal) {             \
                    if (!m_bAttackNpc) break;                         \
                    int __k;                                          \
                    for (__k = 0; __k < defMAX_ARRAY_AUTO; ++__k) {   \
                        if (m_ArrayNpcNeast[__k] == 0) {              \
                            m_ArrayNpcNeast[__k] = (_cand);           \
                            break;                                    \
                        }                                             \
                    }                                                 \
                } else if (Npc[(_cand)].m_Kind == kind_player) {      \
                    if (!m_bAttackPeople) break;                      \
                    int __k;                                          \
                    for (__k = 0; __k < defMAX_ARRAY_AUTO; ++__k) {   \
                        if (m_ArrayNpcNeast[__k] == 0) {              \
                            m_ArrayNpcNeast[__k] = (_cand);           \
                            break;                                    \
                        }                                             \
                    }                                                 \
                }                                                     \
            }                                                         \
        } while (0)

    // ---- quet 4 goc phan tu i,j (hinh tron: i*i + j*j <= nRangeX*nRangeX) ----
    int i, j;
    for (i = 0; i < nRangeX; ++i)
    {
        for (j = 0; j < nRangeY; ++j)
        {
            if ((i * i + j * j) > nRangeX * nRangeX)
                continue;

            // ===================== (+i, +j) =====================
            nRMx = nMapX + i;
            nRMy = nMapY + j;
            nSearchRegion = nRegion;

            if (nRMx < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
                nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
                nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            if (nSearchRegion != -1)
            {
                if (nRMy < 0)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
                    nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
                else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
                    nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
            }
            if (nSearchRegion != -1)
            {
                nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nPlayerNpcIdx, nRelation);

                if (nRet > 0)
                {
                    if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
                        nRet = 0;
                }
                if (nRet > 0)
                {
                    DefensePlayer(nRet);
                    TRY_PUSH_CANDIDATE(nRet);
                    CHECK_ARRAY_FULL_AND_RETURN();
                }
            }

            // ===================== (-i, +j) =====================
            nRMx = nMapX - i;
            nRMy = nMapY + j;
            nSearchRegion = nRegion;

            if (nRMx < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
                nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
                nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            if (nSearchRegion != -1)
            {
                if (nRMy < 0)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
                    nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
                else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
                    nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
            }
            if (nSearchRegion != -1)
            {
                nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nPlayerNpcIdx, nRelation);

                if (nRet > 0)
                {
                    if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
                        nRet = 0;
                }
                if (nRet > 0)
                {
                    DefensePlayer(nRet);
                    TRY_PUSH_CANDIDATE(nRet);
                    CHECK_ARRAY_FULL_AND_RETURN();
                }
            }

            // ===================== (-i, -j) =====================
            nRMx = nMapX - i;
            nRMy = nMapY - j;
            nSearchRegion = nRegion;

            if (nRMx < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
                nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
                nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            if (nSearchRegion != -1)
            {
                if (nRMy < 0)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
                    nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
                else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
				{
					nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
					nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
				}
            }
            if (nSearchRegion != -1)
            {
                nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nPlayerNpcIdx, nRelation);

                if (nRet > 0)
                {
                    if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
                        nRet = 0;
                }
                if (nRet > 0)
                {
                    DefensePlayer(nRet);
                    TRY_PUSH_CANDIDATE(nRet);
                    CHECK_ARRAY_FULL_AND_RETURN();
                }
            }

            // ===================== (+i, -j) =====================
            nRMx = nMapX + i;
            nRMy = nMapY - j;
            nSearchRegion = nRegion;

            if (nRMx < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
                nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
                nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            if (nSearchRegion != -1)
            {
                if (nRMy < 0)
                {
                    nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
                    nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
                }
                else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
				{
					nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
					nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
				}
            }
            if (nSearchRegion != -1)
            {
                nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nPlayerNpcIdx, nRelation);

                if (nRet > 0)
                {
                    if (Npc[nRet].m_Doing == do_death || Npc[nRet].m_Doing == do_revive || Npc[nRet].m_HideState.nTime > 0)
                        nRet = 0;
                }
                if (nRet > 0)
                {
                    DefensePlayer(nRet);
                    TRY_PUSH_CANDIDATE(nRet);
                    CHECK_ARRAY_FULL_AND_RETURN();
                }
            }
        }
    }

    return TRUE;
}



void KPlayerAI::PlayerFollowActack(int i)
{
	if (IsNotValidNpc(i))
		return;
	 if ((m_SpaceBar && !m_HoldSpaceBar) || m_bPriorityUseMouse)
		{ 
			m_nLifeLag = 0;
			m_Actacker = 0;
			m_nTimeRunLag = 0;
			m_nTimeSkip = 0;
			m_Count_Acttack_Lag = 0;
			m_bActacker = FALSE;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
			return;
		}
	// BuffAmulet(i); //Buff bua;
	int _i = 0; 
	int nMapX, nMapY, k, h;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMapX, &nMapY);
	Npc[i].GetMpsPos(&k, &h);	
	if (m_bAttackAround)
	AutoReturn();//Attack Around by kinnox;
		
	//Distance between m_MoveMps[m_MoveStep][1] && Npc[Monster]
	/*if (m_AutoMove)
	{
		if ((k/32 - m_MoveMps[m_MoveStep][1]/32) * (k/32 - m_MoveMps[m_MoveStep][1]/32) + (h/32 - m_MoveMps[m_MoveStep][2]/32) * (h/32 - m_MoveMps[m_MoveStep][2]/32) > 400)
		{
			PlayerMoveMps();//Move Coordinates;
			return;
		}
	}*/

	if (Npc[i].m_RegionIndex > 0)
	{
		if (m_NpcFind.CheckBarrier(h,k) != 0)
		{
			m_nLifeLag = 0;
			m_Actacker = 0;
			m_nTimeRunLag = 0;
			m_nTimeSkip = 0;
			m_Count_Acttack_Lag = 0;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
			m_bActacker = FALSE;
		}
	}


	if ( Npc[i].m_RegionIndex < 0  || !SubWorld[0].TestBarrier(h,k) )
	{
		m_nLifeLag = 0;
		m_Actacker = 0;
		m_nTimeRunLag = 0;
		m_nTimeSkip = 0;
		m_Count_Acttack_Lag = 0;
		m_bActacker = FALSE;
		return;
	}
	
	if ((unsigned)m_nLifeLag == Npc[i].m_CurrentLife)
	{
		m_nTimeRunLag++;
		m_Count_Acttack_Lag++;
	}
	else
	{
		m_nTimeRunLag = 0;
		m_Count_Acttack_Lag = 0;
	}
	m_nTimeSkip++; //Thoi gian bo qua
	int nAddTime = 1;
	if (Npc[i].m_Kind == kind_player)
	{
		nAddTime = 1;
	}
	else 
	{
		if (Npc[i].m_cGold.GetGoldType() >= 1)
			nAddTime = 100000;
		else
			nAddTime = 1;
	}
	
	
	if (Npc[i].m_Kind == kind_player)
	{
		if (m_Count_Acttack_Lag >= defAUTO_COUNT_LAG/2  || m_nTimeRunLag >= defAUTO_TIME_LAG/2 || m_nTimeSkip >= defAUTO_TIME_SKIP*nAddTime)
		{
			for (int j=0; j < defMAX_ARRAY_AUTO; j++)
			{
				if (m_ArrayNpcLag[j] == 0)
				{
					m_ArrayNpcLag[j] = i;
					m_ArrayTimeNpcLag[j] = GetTickCount();
					m_Actacker = 0;
					m_bActacker = FALSE;
					m_nLifeLag = 0;
					m_nTimeRunLag = 0;
					m_Count_Acttack_Lag = 0;
					m_nTimeSkip = 0;
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
					return;
				}
			}
		}
	}
	else
	{
		if (m_Count_Acttack_Lag >= defAUTO_COUNT_LAG*nAddTime || m_nTimeRunLag >= defAUTO_TIME_LAG * 2*nAddTime || m_nTimeSkip >= defAUTO_TIME_SKIP*nAddTime)
		{
			for (int j=0; j < defMAX_ARRAY_AUTO; j++)
			{
				if (m_ArrayNpcLag[j] == 0)
				{
					m_ArrayNpcLag[j] = i;
					m_ArrayTimeNpcLag[j] = GetTickCount();
					m_Actacker = 0;
					m_bActacker = FALSE;
					m_nLifeLag = 0;
					m_nTimeRunLag = 0;
					m_Count_Acttack_Lag = 0;
					m_nTimeSkip = 0;
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
					return;
				}
			}
		}
	
	}
	m_nTimeRunLag++;
	PaintActionAuto(1,i,0,0);//Dang danh quai
	int nIdx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Player[CLIENT_PLAYER_INDEX].GetLeftSkill());
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].IsCanInput() && 
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActiveSkillID != nIdx)
		{
			if (m_nRightHandSkillSet[0] > 0)
			{
				if ( m_nRightHandSkillSet[0] != Player[CLIENT_PLAYER_INDEX].GetRightSkill())
					m_nOldRightHandActiving = Player[CLIENT_PLAYER_INDEX].GetRightSkill();
				
				if (m_nOldRightHandActiving > 0 && m_nRightHandSkillSet[0] > 0)
				{
						nIdx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Player[CLIENT_PLAYER_INDEX].GetRightSkill());
					if (m_nRightHandSkillSet[0] != Player[CLIENT_PLAYER_INDEX].GetRightSkill() || m_nOldRightHandActiving != Player[CLIENT_PLAYER_INDEX].GetRightSkill() )
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(m_nRightHandSkillSet[0]);
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetActiveSkill(nIdx);
						
					}		
				}
			}
			else			
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetActiveSkill(nIdx);
		
		}
		int distanceattack = 0;
 		if (m_bFightDistance)
 		{
 			if (m_DistanceAuto <= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentAttackRadius)
 			{
 				distanceattack = m_DistanceAuto;
 			}
 			else
 			{
 				distanceattack = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentAttackRadius;
 			}
 		}
 		else
 		{
 			distanceattack = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentAttackRadius;
 		}

	int distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, i);

	if (distance <= distanceattack)
	{
		KSkill * pSkill = (KSkill *) g_SkillManager.GetSkill(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActiveSkillID, 1);
		if (pSkill)
		{
			if (pSkill->GetHorseLimit())
			{
				switch(pSkill->GetHorseLimit())
				{
				case 1:
					{
						if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
						{
							if(GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= TIME_RIDE)
								SendClientCmdRide(TRUE);
							return;
						}
					}
					break;
				case 2:
					{
						if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
						{
							if(GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= TIME_RIDE)
								SendClientCmdRide(FALSE);
							return;
						}
					}
					break;
				}
			}
			m_nLifeLag = Npc[i].m_CurrentLife ;
			
			const int ATTACK_DELAY_PLAYER = 30; // 30ms delay for attacking players
    		const int ATTACK_DELAY_NPC = 100;   // 100ms delay for attacking NPCs

		    if (nTickAttack == 0 && (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_run || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_walk))
		    {
		        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing = do_stand;

		        // Determine the appropriate client doing based on fight mode and randomization
		        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
		            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_fightstand;
		        else if (g_Random(6) != 1)
		            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_stand;
		        else
		            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_stand1;

		        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
		        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
		        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
		        nTickAttack = GetTickCount();
		    }
		    else
		    {
		        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_skill)
		            return;

		        // Check if the target NPC is normal
		        if (Npc[i].m_Kind != kind_player)
		        {
		            if (GetTickCount() - nTickAttack < ATTACK_DELAY_NPC)
		            {
		                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
		                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
		                return;
		            }
		        }
		        else // The target NPC is a player
		        {
		            if (GetTickCount() - nTickAttack < ATTACK_DELAY_PLAYER)
		            {
		                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
		                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
		                return;
		            }
		        }
		        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_stand || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_run)
		        {
			        // Reset the nTickAttack and send attack commands
			        nTickAttack = 0;
	//		        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
			        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_skill, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActiveSkillID, -1, i);
			        SendClientCmdSkill(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActiveSkillID, -1, Npc[i].m_dwID);
		    	}
		    }
		}
		return;

	}
	else
	{
		int nDesX, nDesY;
		Npc[i].GetMpsPos(&nDesX, &nDesY);

		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_run || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_skill)
			return;	
		if (m_bFollowAttack == TRUE)
			MoveTo(nDesX, nDesY);
		return;
	}	
	m_Actacker = 0;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
	m_bActacker = FALSE;
}

BOOL KPlayerAI::AutoReturn()
{
	int nMapX, nMapY;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMapX, &nMapY);
	if ( (m_SpaceBar && !m_HoldSpaceBar)  || m_bPriorityUseMouse)
		return FALSE;
	if (m_bAttackAround) // attack around;
	{
			int LimitedY = m_RadiusAuto;
			if (LimitedY > 2048)
				LimitedY = 2048;
			else 
				LimitedY= LimitedY+512;
			
			if (((nMapX/32 - m_PosXAuto/32)*(nMapX/32 - m_PosXAuto/32) > m_RadiusAuto) || ((nMapY/32 - m_PosYAuto/32)*(nMapY/32 - m_PosYAuto/32) > LimitedY)) 
			{
				m_nLifeLag = 0; 
				m_Actacker = 0;
				m_nTimeRunLag = 0;
				m_nTimeSkip = 0;
				m_Count_Acttack_Lag = 0;
				m_bActacker = FALSE;
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
				/*Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_run, m_PosXAuto, m_PosYAuto);			
				SendClientCmdRun(m_PosXAuto, m_PosYAuto);*/
				Player[CLIENT_PLAYER_INDEX].m_cAutoMove.AutoMoveTo(m_MoveMps[m_MoveStep][1],m_MoveMps[m_MoveStep][2]);
				return TRUE;
			}
	}
	return FALSE;
}

BOOL KPlayerAI::PlayerMoveMps()
{
	if (!m_AutoMove) // Chay Toa Do
	{
		b_MoveRevese = FALSE;
		m_MoveStep = 0;
		return FALSE;
	}
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_run)
		return FALSE;

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == fight_active)
	{
		// FIX: Loop from start when reaching end, instead of ping-pong
		if (m_MoveStep >= m_MoveCount || m_MoveStep < 0)
		{
			m_MoveStep = 0;  // Reset to start for continuous loop
			b_MoveRevese = FALSE;
		}

		// Skip invalid coordinates or wrong map
		if (!m_MoveMps[m_MoveStep][0] ||
			(m_MoveMps[m_MoveStep][0] != SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID))
		{
			m_MoveStep++;
			return FALSE;
		}

		int nMapX, nMapY;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMapX, &nMapY);
		//Distance between m_MoveMps[m_MoveStep][1] && Npc[PLAYER]
		// FIX: Check if player is far from current target coordinate
		if (((nMapX/32 - m_MoveMps[m_MoveStep][1]/32)*(nMapX/32 - m_MoveMps[m_MoveStep][1]/32) > 64) ||
			((nMapY/32 - m_MoveMps[m_MoveStep][2]/32)*(nMapY/32 - m_MoveMps[m_MoveStep][2]/32) > 64))
		{
			// Player is far from target, move to it
			m_nLifeLag = 0;
			m_Actacker = 0;
			m_nTimeRunLag = 0;
			m_nTimeSkip = 0;
			m_Count_Acttack_Lag = 0;
			m_bActacker = FALSE;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;

			Player[CLIENT_PLAYER_INDEX].m_cAutoMove.AutoMoveTo(m_MoveMps[m_MoveStep][1],m_MoveMps[m_MoveStep][2]);
			PaintActionAuto(2,0,m_MoveMps[m_MoveStep][1]/256,m_MoveMps[m_MoveStep][2]/512);//Di chuyen den quai
		}
		else
		{
			// Reached current coordinate, move to next in sequence
			m_MoveStep++;
		}

	}
	return FALSE;
}

void KPlayerAI::MoveTo(int nX, int nY)
{
	int nPlayerX, nPlayerY;
	int nDestX, nDestY, dX, dY;

	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_run)
			return;
		PaintActionAuto(2,0,nX/256,nY/512);//Di chuyen den quai
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nPlayerX, &nPlayerY);
		dX = nPlayerX & 0x1F;
		dY = nPlayerY & 0x1F;
		nDestX = nX + 0x10 - dX; // Anti-LAG
		nDestY = nY + 0x10 - dY; // Anti-LAG	
		if (!Player[CLIENT_PLAYER_INDEX].m_RunStatus)
		{
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_walk, nDestX, nDestY);
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_run, nDestX, nDestY);
			SendClientCmdRun(nX, nY);	
		}
		//Player[CLIENT_PLAYER_INDEX].m_cAutoMove.AutoMoveTo(nDestX,nDestY);
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}

BOOL KPlayerAI::IsNotValidNpc(int nIndex)
{
	if (nIndex <= 0 || 
		Npc[nIndex].m_SubWorldIndex != Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex || 
		Npc[nIndex].m_RegionIndex < 0 || 
	   (Npc[nIndex].m_Kind == kind_player && (/*FALSE ==*/ !m_bAttackPeople || 
	    Npc[nIndex].m_FightMode == fight_none || m_bFollowPeople)) || 
	   (Npc[nIndex].m_Kind == kind_normal && (/*FALSE ==*/ !m_bAttackNpc || 
		 (m_nFightBoss == 1 && Npc[nIndex].m_cGold.GetGoldType() > 0 ) ||//Skip all boss
		 (m_nFightBoss == 2 && Npc[nIndex].m_cGold.GetGoldType() > 12 ) ||//Skip blue boss
		 (m_nFightBoss == 3 && Npc[nIndex].m_cGold.GetGoldType() > 1  && Npc[nIndex].m_cGold.GetGoldType() < 13) ||//Skip gold boss
		 (m_nFightBoss == 4 && Npc[nIndex].m_cGold.GetGoldType() > 0  && Npc[nIndex].m_cGold.GetGoldType() < 2) )) || //Skip violet boss		
		Npc[nIndex].m_CurrentLife < 0 || 
		Npc[nIndex].m_CurrentLifeMax < 0 || 
		Npc[nIndex].m_HideState.nTime > 0 || 
		!Npc[nIndex].IsAlive())

	{
		m_Actacker = 0;
		m_bActacker = FALSE;
		m_nLifeLag = 0;
		// m_nTimeRunLag = 0; //block
		m_Count_Acttack_Lag = 0;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
		return TRUE;
	}

	return FALSE;
}

BOOL KPlayerAI::DefensePlayer(int nIndex)
{
	if (m_nDefense == 1)
	{
		if(Npc[nIndex].m_Kind == kind_player)
		{	
			ItemPos    Pos;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy)
			{
				if (pItem)
				{
					if ( Item[pItem->nIdx].GetGenre() == item_script && (Item[pItem->nIdx].GetDetailType() == 18 || Item[pItem->nIdx].GetDetailType() == 19))
					{
						Pos.nPlace = pItem->nPlace;
						Pos.nX = pItem->nX;
						Pos.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
						return FALSE;
					}
				}
				while(pItem)
				{
					pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
					if (pItem)
					{
						if (Item[pItem->nIdx].GetGenre() == item_script && (Item[pItem->nIdx].GetDetailType() == 18 || Item[pItem->nIdx].GetDetailType() == 19))
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return FALSE;
						}
					}
				}
				return FALSE;
			}
		}
		return FALSE;
	}
	return TRUE;
}

void KPlayerAI::CheckPlayerRider()
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_attack ||
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_magic ||
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_skill)
	{
		return;
	}
	
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
	{
		if (m_bRiderHorse == 1)
		{
			if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				if(GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= TIME_RIDE)
					SendClientCmdRide(FALSE);
				return;
			}
		}
		else if (m_bRiderHorse == 2)
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				if(GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= TIME_RIDE)
					SendClientCmdRide(TRUE);
				return;
			}
		}
		else
		{
		}
		return;
	}
}

BOOL KPlayerAI::AutoBuffSkillState()
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].IsCanInput())
	{
		for (int i = 0; i < defMAX_ARRAY_STATESKILL; i ++)
		{
			if (m_ArrayStateSkill[i])
			{
				ISkill * pSkill = g_SkillManager.GetSkill(m_ArrayStateSkill[i], 1);

				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.CanCast(m_ArrayStateSkill[i], SubWorld[0].m_dwCurrentTime) && 
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this), TRUE) && 
					!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].FindStateSkill(m_ArrayStateSkill[i]))
				{
					if (g_SubWorldSet.GetGameTime() % 4 == 0)
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_skill, m_ArrayStateSkill[i], -1, Player[CLIENT_PLAYER_INDEX].m_nIndex);
						SendClientCmdSkill(m_ArrayStateSkill[i], -1, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID);
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL KPlayerAI::AutoBuffEmi()
{
	BOOL nFlag = FALSE;
	if (m_AutoLifeReplenish)
	{
		int nX0,nY0;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nX0,&nY0);
		int nX1,nY1;
		int nSkillLifeReplenishID = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSkillLifeReplenish();
		ISkill * pSkill = g_SkillManager.GetSkill(nSkillLifeReplenishID, 1);
		if (nSkillLifeReplenishID && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this), TRUE))
		{
			// Truong hop 1: buff cho ban than 
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_skill || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_death)
				return FALSE;
			int distance = 0;
			if ( Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife < (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax * (unsigned int)m_AutoLifeReplenishP/MAX_PERCENT) )
			{
				
				if (m_bFollowPeople && m_FollowPeopleIdx)
				{
					Npc[m_FollowPeopleIdx].GetMpsPos(&nX1,&nY1);
					if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < (120*120))
					{
						nFlag = TRUE;
					}
					else
					{
						nFlag = FALSE;
					}
				}
				else
				{
					nFlag = TRUE;
				}	
				
			}
			// Truong hop 2: buff cho captain
			if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag > 0)
			{
				int nIdxCaption = NpcSet.SearchID(g_Team[0].m_nCaptain);
				if ( Npc[nIdxCaption].m_CurrentLife <  (Npc[nIdxCaption].m_CurrentLifeMax * (unsigned int)m_AutoLifeReplenishP/MAX_PERCENT) )
				{
					nFlag = FALSE;
					Npc[nIdxCaption].GetMpsPos(&nX1,&nY1);
					if  ((((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) > (120*120)) && (nIdxCaption || Player[CLIENT_PLAYER_INDEX].m_nIndex))
					{
						nFlag = FALSE;
					}
					else
					{
						nFlag = TRUE;
					}				
				}
				// Truong hop 3: buff cho thanh vien
				for (int i = 0; i < MAX_TEAM_MEMBER; i++)
				{
					if (g_Team[0].m_nMember[i] < 0)
						continue;
					int nIdxMember= NpcSet.SearchID(g_Team[0].m_nMember[i]);	 
					if ( Npc[nIdxMember].m_CurrentLife < Npc[nIdxMember].m_CurrentLifeMax * (unsigned int)m_AutoLifeReplenishP / MAX_PERCENT )		
					{
						Npc[nIdxMember].GetMpsPos(&nX1,&nY1);
						if  ((((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) > (120*120)) && (nIdxCaption || Player[CLIENT_PLAYER_INDEX].m_nIndex))
						{
							nFlag = FALSE;
						}
						else
						{
							nFlag = TRUE;
						}

					}
				}
			}
			if (nFlag)
			{
				m_Actacker = 0;
				m_bActacker = FALSE;
				m_nLifeLag = 0;
				m_nTimeRunLag = 0;
				m_nTimeSkip = 0;
				m_Count_Acttack_Lag = 0;
				m_nObject = 0;
				m_bObject = FALSE;
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_skill, nSkillLifeReplenishID, -1, Player[CLIENT_PLAYER_INDEX].m_nIndex);		
				SendClientCmdSkill(nSkillLifeReplenishID, -1, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID);	
				return nFlag;
			}
		}
	}
	return nFlag;
}

void KPlayerAI::PlayerSwitchAura()
{
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
	{
		if (m_AuraSkill[0] && m_AuraSkill[1])
		{
			SYSTEMTIME systm;
			GetLocalTime(&systm);
			Player[CLIENT_PLAYER_INDEX].SetRightSkill((systm.wSecond % 2) ? m_AuraSkill[0] : m_AuraSkill[1]);
		}
	}
}

void KPlayerAI::PlayerBuffWhenManaSmall()
{
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
	{
		if ( m_nLeftSkillSet[0] != Player[CLIENT_PLAYER_INDEX].GetLeftSkill())
			m_nLeftSkillActiving = Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
		
		if (m_nLeftSkillActiving > 0 && m_nLeftSkillSet[0] > 0)
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana < (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax *30/100))
			{
					if (m_nLeftSkillSet[0] == Player[CLIENT_PLAYER_INDEX].GetLeftSkill())
						return;
					Player[CLIENT_PLAYER_INDEX].SetLeftSkill(m_nLeftSkillSet[0]);
			}
			else
			{
				if (m_nLeftSkillActiving == Player[CLIENT_PLAYER_INDEX].GetLeftSkill())
						return;
				Player[CLIENT_PLAYER_INDEX].SetLeftSkill(m_nLeftSkillActiving);
			}
		}
	}
	else
	{
		if (m_nLeftSkillActiving != 0)
			m_nLeftSkillActiving = 0;
	}
}

void KPlayerAI::PlayerActiveFightHand()
{
	if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
	{
		if ( m_nRightHandSkillSet[0] != Player[CLIENT_PLAYER_INDEX].GetRightSkill())
			m_nOldRightHandActiving = Player[CLIENT_PLAYER_INDEX].GetRightSkill();
		
		if (m_nOldRightHandActiving > 0 && m_nRightHandSkillSet[0] > 0)
		{
			int nIdx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Player[CLIENT_PLAYER_INDEX].GetRightSkill());
			if (m_nRightHandSkillSet[0] != Player[CLIENT_PLAYER_INDEX].GetRightSkill() || m_nOldRightHandActiving != Player[CLIENT_PLAYER_INDEX].GetRightSkill() )
			{
				Player[CLIENT_PLAYER_INDEX].SetRightSkill(m_nRightHandSkillSet[0]);
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetActiveSkill(nIdx);
				
			}	
			return;			
		}
	}
	else
	{
		if (m_nOldRightHandActiving != 0)
			m_nOldRightHandActiving = 0;
	}
}

void KPlayerAI::InviteParty()
{
	for (int i=0; i < defMAX_LIST_PARTY; i++)
	{
		if (m_nArrayTimeInvitePlayer[i] > 0)
		{
			m_nArrayTimeInvitePlayer[i]--;
			if (m_nArrayTimeInvitePlayer[i] == 0)
				m_nArrayInvitePlayer[i] = 0;
		}
	}		

	if (g_Team[0].m_nMemNum < g_Team[0].CalcCaptainPower())
	{
		int nCount = NpcSet.GetAroundPlayerForTeamInvite(NULL, 0);
		if(nCount)
		{
			KUiPlayerItem*	m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nCount);
			NpcSet.GetAroundPlayerForTeamInvite(m_pNearbyPlayersList, nCount);
			for (i = 0; i < nCount; i++)
			{
				if (m_bAutoInviteList && !CheckPTList(m_pNearbyPlayersList[i].Name))
					continue;
				if (FindSameInvitePlayer(m_pNearbyPlayersList[i].uId))
					continue;
				int j = FindFreeInvitePlayer();
				if (j>=0)
				{
					Player[CLIENT_PLAYER_INDEX].TeamInviteAdd(m_pNearbyPlayersList[i].uId);
					m_nArrayInvitePlayer[j] = m_pNearbyPlayersList[i].uId;
					m_nArrayTimeInvitePlayer[j] = defINTERVAL_AUTO;
				}
			}
			free (m_pNearbyPlayersList);
		}
	}
}

BOOL KPlayerAI::CheckPTList(const char *Name)
{
	if (!Name || !Name[0])
		return TRUE;
		
	for (int i = 0; i < defMAX_LIST_PARTY; i ++)
	{
		if (m_szPartyList[i][0] && strcmp(m_szPartyList[i], Name) == 0)
			return TRUE;
	}
	return FALSE;
}

BOOL KPlayerAI::IsRAPEquip(int btDetail)
{
	if (btDetail == equip_ring || 
		btDetail == equip_amulet || 
		btDetail == equip_pendant)
		return TRUE;
	return FALSE;
}

BOOL KPlayerAI::IsEquipSatisfyCondition(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return FALSE;
		
	int i, j;
	for (i = 0; i < defMAX_AUTO_FILTERL; i++)
	{
		if (m_FilterMagic[i][0])
		{
			for (j = 0; j < MAX_ITEM_MAGICATTRIB; j++)
			{
				if (Item[nIdx].m_aryMagicAttrib[j].nAttribType == m_FilterMagic[i][0] && 
					Item[nIdx].m_aryMagicAttrib[j].nValue[0] >= m_FilterMagic[i][1])
					return TRUE;
			}
		}
	}
	return FALSE;
}

void KPlayerAI::PlayerFilterEquip()
{
	/*if (Player[CLIENT_PLAYER_INDEX].GetStatusBox() > 0)
		return;	*/
	int nHand = Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
	if (nHand)
	{
		PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
		if (pItem)
		{
			if (Item[nHand].GetGenre() != item_goldequip && Item[nHand].GetGenre() == item_equip &&
				Item[nHand].GetDetailType() < equip_detailnum) 
			{
				BOOL bFilter = TRUE;				
					if (Item[nHand].GetGenre() == item_goldequip)
					bFilter = FALSE;
					else if (Item[nHand].GetBindState() > 0)
						bFilter = FALSE;
					else if ( m_bPiceItem && Item[nHand].GetPrice() >= m_nPiceItem)
						bFilter = FALSE;
					else if ( m_bLevelItem && Item[nHand].GetLevel() >= m_nLevelItem)
						bFilter = FALSE;	
					else if (Item[nHand].GetGenre() == item_equip && 
							(Item[nHand].GetDetailType() == equip_horse ||
							 Item[nHand].GetDetailType() == equip_mask))
						bFilter = FALSE;
					else if (m_SaveRAP && IsRAPEquip(Item[nHand].GetDetailType()))
						bFilter = FALSE;
					else
					{
						if (IsEquipSatisfyCondition(nHand))
							bFilter = FALSE;
					}		
					if (bFilter == FALSE)
						bCheckFilter = FALSE;			
				if (bFilter)
				{						
					Player[CLIENT_PLAYER_INDEX].AutoThrowAwayItem(nHand);
					bCheckFilter = TRUE;			
					return;
				}
			}
		}
	}
	else
	{
		ItemPos	P;
		PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
		if (pItem)
		{
			if (pItem->nPlace == pos_equiproom && 
				Item[pItem->nIdx].GetGenre() == item_equip && 
				Item[pItem->nIdx].GetDetailType() < equip_detailnum)
			{
				BOOL bFilter = TRUE;
				if (Item[pItem->nIdx].GetGenre() == item_goldequip)
					bFilter = FALSE;
				else if (Item[pItem->nIdx].GetBindState() > 0)
					bFilter = FALSE;
				else if ( m_bPiceItem && Item[pItem->nIdx].GetPrice() >= m_nPiceItem)
					bFilter = FALSE;
				else if ( m_bLevelItem && Item[pItem->nIdx].GetLevel() >= m_nLevelItem)
					bFilter = FALSE;	
				else if (Item[pItem->nIdx].GetGenre() == item_equip && 
						(Item[pItem->nIdx].GetDetailType() == equip_horse ||
						 Item[pItem->nIdx].GetDetailType() == equip_mask))
					bFilter = FALSE;
				else if (m_SaveRAP && IsRAPEquip(Item[pItem->nIdx].GetDetailType()))
					bFilter = FALSE;
				else
				{
					if (IsEquipSatisfyCondition(pItem->nIdx))
						bFilter = FALSE;
				}				
				if (bFilter == FALSE)
					bCheckFilter = FALSE;	
			
				if (bFilter)
				{					
					P.nPlace = pItem->nPlace;
					P.nX = pItem->nX;
					P.nY = pItem->nY;
					Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
					bCheckFilter = TRUE;	
					return;
				}
			}
		}
		int n = 0;
		while(pItem) 
		{ 
			pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
			n++;
			if (pItem)
			{
				if (pItem->nPlace == pos_equiproom && 
					Item[pItem->nIdx].GetGenre() == item_equip && 
					Item[pItem->nIdx].GetDetailType() < equip_detailnum)
				{
					BOOL bFilter = TRUE;
					if (Item[pItem->nIdx].GetGenre() == item_goldequip)
						bFilter = FALSE;
					else if (Item[pItem->nIdx].GetBindState() > 0)
						bFilter = FALSE;
					else if ( m_bPiceItem && Item[pItem->nIdx].GetPrice() >= m_nPiceItem)
						bFilter = FALSE;
					else if ( m_bLevelItem && Item[pItem->nIdx].GetLevel() >= m_nLevelItem)
						bFilter = FALSE;	
					else if (Item[pItem->nIdx].GetGenre() == item_equip && 
						(Item[pItem->nIdx].GetDetailType() >= equip_horse &&
						 Item[pItem->nIdx].GetDetailType() <= equip_mask))
						bFilter = FALSE;
					else if (m_SaveRAP && IsRAPEquip(Item[pItem->nIdx].GetDetailType()))
						bFilter = FALSE;							
					else
					{
						if (IsEquipSatisfyCondition(pItem->nIdx))
							bFilter = FALSE;
					}		
					
					if (bFilter == FALSE)
						bCheckFilter = FALSE;	
					
					if (bFilter)
					{												
						P.nPlace = pItem->nPlace;
						P.nX = pItem->nX;
						P.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
						bCheckFilter = TRUE;	
						return;
					}
				}
			}
		}
	}
}

BOOL KPlayerAI::FindObjectNearPlayer()
{
	int nRangeX = 0;
	int	nRangeY = 0;
	int	nSubWorldIdx = 0;
	int	nRegion = 0;
	int	nMapX = 0;
	int	nMapY = 0;

	nRangeX = m_RadiusAuto <= 100 ? 100 : m_RadiusAuto;
	nRangeY = m_RadiusAuto <= 100 ? 100 : m_RadiusAuto;
	
	nSubWorldIdx = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex;
	nRegion = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex;
	nMapX = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapX;
	nMapY = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapY;

	int	nRet;
	int	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorldIdx].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorldIdx].m_nCellHeight;	

	ClearArrayObjectNeast();

	for (int i = 0; i < nRangeX; i++)
	{
		for (int j = 0; j < nRangeY; j++)
		{
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
				
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindObject(nRMx, nRMy, true);
			if (nRet > 0)
			{	
				for (int k=0; k < defMAX_ARRAY_AUTO; k++)
				{
					if (m_ArrayObjectNeast[k] == 0)
					{
						m_ArrayObjectNeast[k] = nRet;
						break;
					}
					if(k == defMAX_ARRAY_AUTO)
						return TRUE;
				}
			}
			nRMx = nMapX - i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;		
			
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindObject(nRMx, nRMy, true);
			if (nRet > 0)
			{	
				for (int k=0; k < defMAX_ARRAY_AUTO; k++)
				{
					if (m_ArrayObjectNeast[k] == 0)
					{
						m_ArrayObjectNeast[k] = nRet;
						break;
					}
					if(k == defMAX_ARRAY_AUTO)
						return TRUE;
				}
			}
			
			nRMx = nMapX - i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindObject(nRMx, nRMy, true);
			if (nRet > 0)
			{	
				for (int k=0; k < defMAX_ARRAY_AUTO; k++)
				{
					if (m_ArrayObjectNeast[k] == 0)
					{
						m_ArrayObjectNeast[k] = nRet;
						break;
					}
					if(k == defMAX_ARRAY_AUTO)
						return TRUE;
				}
			}

			nRMx = nMapX + i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;			
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			
			nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindObject(nRMx, nRMy, true);
			if (nRet > 0)
			{	
				for (int k=0; k < defMAX_ARRAY_AUTO; k++)
				{
					if (m_ArrayObjectNeast[k] == 0)
					{
						m_ArrayObjectNeast[k] = nRet;
						break;
					}
					if(k == defMAX_ARRAY_AUTO)
						return TRUE;
				}
			}
		}
	}
	return TRUE;
}

int KPlayerAI::FindNearObject2Array()
{
	int nRet = 0 ;
	int m_ArrayObjectMoney[100];
	int m_ArrayObjectTask[100];
	int m_ArrayObjectItem[100];
	memset(m_ArrayObjectMoney,0,sizeof(m_ArrayObjectMoney));
	memset(m_ArrayObjectTask,0,sizeof(m_ArrayObjectTask));
	memset(m_ArrayObjectItem,0,sizeof(m_ArrayObjectItem));
	int nCountMoney = 0;
	int nCountTask  = 0;
	int nCountItem  = 0;
	int nCountPick  = 0;
	int p = 0;
	if (FindObjectNearPlayer())
	{
		int distance = 0 ;
		int distanceMin = 0 ;
		BOOL _fg = FALSE;

		for (p = 0; p < defMAX_ARRAY_AUTO; p++)
		{
			if (Object[m_ArrayObjectNeast[p]].m_nKind == Obj_Kind_Money)
			{
				m_ArrayObjectMoney[nCountMoney] = m_ArrayObjectNeast[p];
				nCountMoney++;
				continue;
			}
			else if (Object[m_ArrayObjectNeast[p]].m_nKind == Obj_Kind_Item)
			{
				if (Object[m_ArrayObjectNeast[p]].m_nGenre == item_task || Object[m_ArrayObjectNeast[p]].m_nGenre == item_script)
				{
					m_ArrayObjectTask[nCountTask] = m_ArrayObjectNeast[p];
					nCountTask++;
					continue;
				}
				else
				{
					m_ArrayObjectItem[nCountTask] = m_ArrayObjectNeast[p];
					nCountItem++;
					continue;
				}
			}
		}

		for (p = 0; p < defMAX_ARRAY_AUTO; p++)
		{
			m_ArrayObjectNeast[p] = 0;
		}

		for (p = 0; p < defMAX_ARRAY_AUTO; p++)
		{
			if (m_ArrayObjectMoney[p] != 0)
			{
				m_ArrayObjectNeast[nCountPick] = m_ArrayObjectMoney[p];
				nCountPick++;
			}
		}

		for ( p = 0; p < defMAX_ARRAY_AUTO; p++)
		{
			if (m_ArrayObjectTask[p] != 0)
			{
				m_ArrayObjectNeast[nCountPick] = m_ArrayObjectTask[p];
				nCountPick++;
			}
		}
		
		for ( p = 0; p < defMAX_ARRAY_AUTO; p++)
		{
			if (m_ArrayObjectItem[p] != 0)
			{
				m_ArrayObjectNeast[nCountPick] = m_ArrayObjectItem[p];
				nCountPick++;
			}
		}

		for (int i= 0; i < defMAX_ARRAY_AUTO; i++)
		{			
			if (m_ArrayObjectNeast[i] != 0)
			{
				BOOL _findLag = FALSE;
				for (int j=0; j < defMAX_ARRAY_AUTO; j++)
				{		
					if (Object[m_ArrayObjectNeast[i]].m_nID == m_ArrayObjectLag[j])
					{
						_findLag = TRUE;
						break;
					}
				}
				if (_findLag == TRUE)
					continue;
				if (IsNotValidObject(m_ArrayObjectNeast[i]))
					continue;								
				distance = Object[m_ArrayObjectNeast[i]].GetDistanceSquare(Player[CLIENT_PLAYER_INDEX].m_nIndex);
				if (FALSE == _fg)
				{
					distanceMin = distance ;
					_fg = TRUE;
					nRet = m_ArrayObjectNeast[i];
				}
				if (distance <= distanceMin)
				{
					distanceMin = distance ;
					nRet = m_ArrayObjectNeast[i];
				}
			}
		}
	}

	if (nRet == 0)
	{
		m_nObject = 0;
		m_bObject = FALSE;
		// m_nTimeRunLag = 0;
	}
	ClearArrayObjectNeast();
	return nRet;
}

BOOL KPlayerAI::IsNotValidObject(int nObject)
{		
	if (!Player[CLIENT_PLAYER_INDEX].m_bActiveAuto && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == fight_none)
		return TRUE;
//	if (m_nObject == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nObjectIdx)
//		return FALSE;

	if (nObject > 0 && nObject < MAX_OBJECT)
	{
		
		if (Object[nObject].m_dwNpcId != 0 || // vat pham nguoi khac vut ra
			Object[nObject].m_dwNpcId == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)	// vat pham minh vut ra
			return TRUE;
	/*	if (Object[nObject].m_dwNpcId == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)	// vat pham minh vut ra
			return TRUE;*/

		if (m_bPickItem)
		{
//			int nX1, nY1, nX2, nY2;
//			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nX1, &nY1);		// khong duoi theo vat pham
//			Object[nObject].GetMpsPos(&nX2, &nY2);

//			if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) > PLAYER_PICKUP_CLIENT_DISTANCE * PLAYER_PICKUP_CLIENT_DISTANCE)
//				return FALSE;
			if (Object[nObject].m_nKind == Obj_Kind_Money)
			{
				if (m_btPickUpKind >= enumPickUpEarn)
					return FALSE;
			}
			else if (Object[nObject].m_nKind == Obj_Kind_Item)
			{
				if (Object[nObject].m_nGenre == item_equip && 
				   (Object[nObject].m_nDetailType == equip_horse || 
					Object[nObject].m_nDetailType == equip_mask))
					return FALSE;			
					
				if (m_btPickUpKind == enumPickEventItem)
				{
					if (Object[nObject].m_nGenre != item_equip)
						return FALSE;			
				}
				else if (m_btPickUpKind == enumPickItemColor)
				{
					if (Object[nObject].m_nColorID > 0)
						return FALSE;
				}
				else if (m_btPickUpKind == enumPickUpAll)
				{
					return FALSE;				
				}
			}
		}
	}
	return TRUE;
}

BOOL KPlayerAI::PlayerFollowObject(int nObject) //dang lam
{
	if (IsNotValidObject(nObject))
	{
		m_nObject = 0;
		return FALSE;
	}

	if (m_SpaceBar)
		return FALSE;
		
	int nX1, nY1, nX2, nY2;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nX1, &nY1);
	Object[nObject].GetMpsPos(&nX2, &nY2);
	
	if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) < PLAYER_PICKUP_CLIENT_DISTANCE * PLAYER_PICKUP_CLIENT_DISTANCE)
	{
		Player[CLIENT_PLAYER_INDEX].CheckObject(nObject);		
		for (int i=0; i<defMAX_ARRAY_AUTO; i++)
		{
			if (m_ArrayObjectLag[i] == 0)
			{
				m_ArrayObjectLag[i] = Object[m_nObject].m_nID;
				m_ArrayTimeObjectLag[i] = GetTickCount();
				break;
			}
		}
		m_bObject = FALSE;
		m_nObject = 0;
		m_nTimeRunLag = 0;
//		ClearArrayObjectLag();
		return FALSE;
	}
	else
	{
		if (m_bPriorityUseMouse == FALSE)
		{		
			MoveTo(nX2, nY2);
			m_nTimeRunLag++;
			Player[CLIENT_PLAYER_INDEX].CheckObject(nObject);
			if (m_nTimeRunLag >= defAUTO_TIME_LAG * 2)
			{ 
				for (int i=0; i < defMAX_ARRAY_AUTO; i++)
				{
					if (m_ArrayObjectLag[i] == 0)
					{
						m_ArrayObjectLag[i] = Object[nObject].m_nID;
						m_ArrayTimeObjectLag[i] = GetTickCount();
						m_bObject = FALSE;
						m_nObject = 0;
						nObject = 0;
						break;
					}
				}
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			m_bObject = FALSE;
			m_nObject = 0;
			m_nTimeRunLag = 0;
		}
	}
	return FALSE;
}

BOOL KPlayerAI::PlayerEatAItem()
{	
	bool bTPFlag = false;

	if (m_EatLife && (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife <= (unsigned)m_LifeAutoV || 
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife*(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax/100) <= (unsigned)m_LifeAutoP))
	{

		if (IR_GetCurrentTime() - m_LifeCountDown > m_LifeTimeUse)
		{
			if (PlayerUseItem(Blood))		
				m_LifeCountDown = IR_GetCurrentTime();
		}
	}
	if (m_EatMana && (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana <= m_ManaAutoV || 
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana*(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax/100) <= m_LifeAutoP))
	{
		if (IR_GetCurrentTime() - m_ManaCountDown > m_ManaTimeUse)
		{
			if (PlayerUseItem(Mana))
				m_ManaCountDown = IR_GetCurrentTime();
		}
	}
	if (m_AutoAntiPoison && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PoisonState.nTime)
	{
		if (IR_GetCurrentTime() - m_AntiPoisonCountDown > defMAX_EAT_SPACETIME)
		{
			if (PlayerUseItem(AntiPoison))
				m_AntiPoisonCountDown = IR_GetCurrentTime();
		}
	}				
	
	if (m_AutoTTL && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentExpEnhance <= 0)
	{
		if (IR_GetCurrentTime() - m_TTLCountDown > defMAX_EAT_SPACETIME)
		{
			if (PlayerUseItem(EnchaseExp))
				m_TTLCountDown = IR_GetCurrentTime();
		}
	}

	if (m_AutoBoodBag && 
	   !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 0) &&
	   !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 1) &&
	   !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 2) /*||
	   !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 4)*/ )
	{
		int nMapID = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
		if (nMapID == 325 || nMapID == 380)
		{

		}
		else
		{
		if (IR_GetCurrentTime() - m_BoodBagCountDown > defMAX_EAT_SPACETIME)
		{
			if (PlayerUseItem(MecidineBag))
				m_BoodBagCountDown = IR_GetCurrentTime();
		}
		}
	}
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		if ((m_TPLife && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife <= (unsigned)m_TPLifeV) || 
			(m_TPMana && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana <= m_TPManaV) ||
			(m_TPNotMedicineBlood && !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 0) && 
			!Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 2) /*&& 
			!Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 4)*/) || 
			(m_TPNotMedicineMana  && !Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 1) && 
			!Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 2) /*&& 
			!Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 4)*/) || 
			(m_TPHightMoney && Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() > 10000 && 
			Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() / 10000 > m_TPHightMoneyV) || 
			(m_btDurabilityItem && Player[CLIENT_PLAYER_INDEX].m_ItemList.HaveDamageItem(m_btDurabilityValue))
			)
		{
			bTPFlag = true;							
		}
		if (m_bTPNotEquipment)
		{
			switch (m_nTPNotEquipmentV)
			{
				case enumRoomNotEnough1:
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(1,1) == FALSE)
						bTPFlag = true;
					break;
				case enumRoomNotEnough2:
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(1,2) == FALSE && 
						Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(2,1) == FALSE)
						bTPFlag = true;
					break;		
				case enumRoomNotEnough4:
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(2,2) == FALSE)
						bTPFlag = true;
					break;
				case enumRoomNotEnough6:
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(2,3) == FALSE)
						bTPFlag = true;	
					break;						
			}
		}

		if (bTPFlag && IR_GetCurrentTime() - m_PortalCountDown > defMAX_EAT_SPACETIME)
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_death || 
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_revive || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_hurt) // fix lag frame khi ve thanh by kinnox;
			{
				/* not content */
			}
			else
			{
				if (PlayerUseItem(TownPortal))
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing = do_stand;
				        // Determine the appropriate client doing based on fight mode and randomization
				        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
				            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_fightstand;
				        else if (g_Random(6) != 1)
				            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_stand;
				        else
				            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ClientDoing = cdo_stand1;

				        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
//				        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
				        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;//moi them by kinnox 20/07/2023;
						m_PortalCountDown = IR_GetCurrentTime();
					}
			}

		}
	}
	return bTPFlag;
}

BOOL KPlayerAI::PlayerUseItem(int type)
{
	ItemPos    Pos;
	PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
	//ThrowItem on hand when Use Item;
	int nHand = Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
	if (nHand)
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
			Player[CLIENT_PLAYER_INDEX].ThrowAwayItem();	
	}
	//end
	
	
	if (pItem)
	{
		if (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy)
		{
			switch(type)
			{
			case Blood:
				if (Item[pItem->nIdx].GetGenre() == item_medicine)
				{
					for (int i = 0; i < 6; i++)
					{
						if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_lifepotion_v)
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
				}
				break;
			case Mana:
				if (Item[pItem->nIdx].GetGenre() == item_medicine)
				{
					for (int i = 0; i < 6; i++)
					{
						if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_manapotion_v)
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
				}
				break;
			case TownPortal:
				if (Item[pItem->nIdx].GetGenre() == 7)
				{
					if (Item[pItem->nIdx].GetDetailType() == 18 || 
						Item[pItem->nIdx].GetDetailType() == 19)
					{
						Pos.nPlace = pItem->nPlace;
						Pos.nX = pItem->nX;
						Pos.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
						return TRUE;
					}
				}
				break;		
			case AntiPoison:
				if (Item[pItem->nIdx].GetGenre() == item_medicine)
				{
					for (int i = 0; i < 6; i++)
					{
						if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_poisondamagereduce_v)
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
				}
				break;
			case EnchaseExp:
				if (Item[pItem->nIdx].GetGenre() == 7)
				{
					if (Item[pItem->nIdx].GetDetailType() == 3 || 
						Item[pItem->nIdx].GetDetailType() == 4)
					{
						Pos.nPlace = pItem->nPlace;
						Pos.nX = pItem->nX;
						Pos.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
						return TRUE;
					}
				}
				break;
			case MecidineBag:
				if (Item[pItem->nIdx].GetGenre() == 7)
				{
					if (Item[pItem->nIdx].GetDetailType() == 70)
					{
						if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() != 0)
							return FALSE;
						Pos.nPlace = pItem->nPlace;
						Pos.nX = pItem->nX;
						Pos.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
						return TRUE;
					}
				}
				break;
			}
		}
	}
	
	while(pItem) 
	{ 
		pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
		if (pItem)
		{
			if (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_builditem)
			{
				
				switch(type)
				{
				case Blood:
					if (Item[pItem->nIdx].GetGenre() == item_medicine)
					{
						for (int i = 0; i < 6; i++)
						{
							if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_lifepotion_v)
							{
								Pos.nPlace = pItem->nPlace;
								Pos.nX = pItem->nX;
								Pos.nY = pItem->nY;
								Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
								return TRUE;
							}
						}
					}
					break;
				case Mana:
					if (Item[pItem->nIdx].GetGenre() == item_medicine)
					{
						for (int i = 0; i < 6; i++)
						{
							if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_manapotion_v)
							{
								Pos.nPlace = pItem->nPlace;
								Pos.nX = pItem->nX;
								Pos.nY = pItem->nY;
								Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
								return TRUE;
							}
						}
					}
					break;	
				case TownPortal:
					if (Item[pItem->nIdx].GetGenre() == 7)
					{
						if (Item[pItem->nIdx].GetDetailType() == 18 || 
							Item[pItem->nIdx].GetDetailType() == 19)
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
					break;	
				case AntiPoison:
					if (Item[pItem->nIdx].GetGenre() == item_medicine)
					{
						for (int i = 0; i < 6; i++)
						{
							if (Item[pItem->nIdx].GetBaseMagicAttribType(i) == magic_poisondamagereduce_v)
							{
								Pos.nPlace = pItem->nPlace;
								Pos.nX = pItem->nX;
								Pos.nY = pItem->nY;
								Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
								return TRUE;
							}
						}
					}
					break;		
				case EnchaseExp:
					if (Item[pItem->nIdx].GetGenre() == 7)
					{
						if (Item[pItem->nIdx].GetDetailType() == 3 || 
							Item[pItem->nIdx].GetDetailType() == 4)
						{
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
					break;	
				case MecidineBag:
					if (Item[pItem->nIdx].GetGenre() == 7)
					{
						if (Item[pItem->nIdx].GetDetailType() == 70)
						{
							if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() != 0)
							return FALSE;
							Pos.nPlace = pItem->nPlace;
							Pos.nX = pItem->nX;
							Pos.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos);
							return TRUE;
						}
					}
					break;
				}
			}
		}
	}
	return FALSE;	
}

void KPlayerAI::EatFullBox()
{
	if (m_EatFullBox)
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
		{
			BOOL bTPFlag = false;
			BOOL bTPFlag1 = false;
			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchEquipment(3,3) == FALSE)
			{
				
				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 0) || 
					Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 2) /*||
					Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 4)*/)
						bTPFlag = true;
				else
				{
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 1) || 
					Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 2) /*|| 
					Player[CLIENT_PLAYER_INDEX].m_ItemList.CountCommonItem(item_medicine, 4)*/)
						bTPFlag1 = true;	
				}
				
			}
			
			if (bTPFlag && IR_GetCurrentTime() - m_EatFullBoxCountDown > defMAX_EAT_SPACETIME)
			{
				if (PlayerUseItem(Blood))
					m_EatFullBoxCountDown = IR_GetCurrentTime();
			}
			
			if (bTPFlag1 && IR_GetCurrentTime() - m_EatFullBoxCountDown > defMAX_EAT_SPACETIME)
			{
				if (PlayerUseItem(Mana))
					m_EatFullBoxCountDown = IR_GetCurrentTime();
			}
		}
	}		
}

BOOL KPlayerAI::RepairEquip()
{
	int nMapID = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{	
		if (nMapID == 242 || nMapID == 342)	
		{		
			m_RepairTime = 0;
			Player[CLIENT_PLAYER_INDEX].m_ItemList.HaveDamageItem(99);
		}
		else
		{
			m_RepairTime = 0;
			return TRUE; 
		}
	}
	if (!m_bRepairEQuip)
	{
		return TRUE;
	}
	if (nMapID != 242 && nMapID != 342)
	{
		if (!m_btDurabilityItem)
		{
			m_nNextItem = itempart_head;
			return TRUE;
		}		
	}
	if (IR_GetCurrentTime() - m_RepairTime > 2000)
	{
		int nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.m_EquipItem[m_nNextItem];
		if (nIdx)
		{
			if (Item[nIdx].GetRepairPrice())
			{
				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < Item[nIdx].GetRepairPrice())
				{	
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetRepositoryMoney() > Item[nIdx].GetRepairPrice())
					{
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(room_repository, room_equipment,Player[CLIENT_PLAYER_INDEX].m_ItemList.GetRepositoryMoney());
						SendClientCmdRepair(Item[nIdx].GetID());
						m_nNextItem = m_nNextItem + 1;
					}
					else
						m_nNextItem = m_nNextItem + 1;  

				}
				else
				{
					SendClientCmdRepair(Item[nIdx].GetID());
					m_nNextItem = m_nNextItem + 1;
				}		
			}
			else
				m_nNextItem = m_nNextItem + 1;
		}
		else
			m_nNextItem = m_nNextItem + 1;
		
		if (m_nNextItem == itempart_cuff)	// 5 loai can sua
		{
			m_nNextItem = itempart_head;
		}
		m_RepairTime = IR_GetCurrentTime();	
		return TRUE;
	}	
	return FALSE;
}

BOOL KPlayerAI::SaveMoney()
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode )
		return TRUE;
	if (m_bSaveMoney)
	{
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() > m_nMoney)
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(room_equipment, room_repository, Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney());
		return TRUE;
	}
	return FALSE;
}

void KPlayerAI::BackToMap()
{
	int nMapID = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (nMapID == 337 ||  nMapID == 338 || nMapID == 339 || 
		nMapID == 464 ||  nMapID == 465 || nMapID == 466 || 
		nMapID == 467 ||  nMapID == 468 || nMapID == 469 || 
		nMapID == 470 ||  nMapID == 380 || nMapID == 242 || nMapID == 208 || nMapID == 209 || nMapID == 372 || nMapID == 396 || nMapID == 397 || nMapID == 401 || nMapID == 403 || nMapID == 394)
	{
		m_nTimeBacking = 0;
		return;
	}
	
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading() || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop )
	{
		m_nTimeBacking = 0;
		return;
	}

	
	if (!m_BackToTownPor || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		m_nTimeBacking = 0;
		return;
	}

	if (m_nTimeSetBack < 10)
		m_nTimeSetBack = 10;
		
	if (m_nTimeSetBack > 0)
	{
		if (Npc[CLIENT_PLAYER_INDEX].m_Doing == do_death || 
		Npc[CLIENT_PLAYER_INDEX].m_Doing == do_revive || Npc[CLIENT_PLAYER_INDEX].m_Doing == do_hurt) // fix lag frame khi ve thanh by kinnox;
		return;
			
		if (m_nTimeBacking <= 0)
		{
			m_nTimeBacking = g_SubWorldSet.GetGameTime() + m_nTimeSetBack*20;
		}
		else
		{
			if (g_SubWorldSet.GetGameTime() - m_nTimeBacking >= 0)
			{
				C2SPLAYER_AI_BACKTOTOWN AutoCmd;
				AutoCmd.ProtocolType = c2s_aibacktotown;	
				AutoCmd.nIdSubWorld = 0;	
				AutoCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();	
				AutoCmd.dwTimePacker = GetTickCount();
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)&AutoCmd, sizeof(C2SPLAYER_AI_BACKTOTOWN));
				m_nTimeBacking = 0;
			}
		}
	}	
}

void KPlayerAI::BackMapTrain()
{
	int nMapID = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (nMapID == 337 ||  nMapID == 338 || nMapID == 339 || 
		nMapID == 464 ||  nMapID == 465 || nMapID == 466 || 
		nMapID == 467 ||  nMapID == 468 || nMapID == 469 || 
		nMapID == 470 ||  nMapID == 380 || nMapID == 242 || nMapID == 208 || nMapID == 209 || nMapID == 372 || nMapID == 396 || nMapID == 397 || nMapID == 401 || nMapID == 403 || nMapID == 394)
	{
		m_nTimeBackToMapTrain = 0;
		return;
	}
	
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading() || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop )
	{
		m_nTimeBackToMapTrain = 0;
		return;
	}

	if (Npc[CLIENT_PLAYER_INDEX].m_Doing == do_death || 
	Npc[CLIENT_PLAYER_INDEX].m_Doing == do_revive || Npc[CLIENT_PLAYER_INDEX].m_Doing == do_hurt) // fix lag frame khi ve thanh by kinnox;
	return;
		
	if (m_BackMapTrain <= 0 || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		m_nTimeBackToMapTrain = 0;
		return;
	}
	int nIDSubWorld = 0;
	if (m_BackMapTrain > 0)
	{
		if (m_BackMapTrain == 1)
		{
			nIDSubWorld = 75;
		}
		else if (m_BackMapTrain == 2)
		{
			nIDSubWorld = 224;
		}
		else if (m_BackMapTrain == 3)
		{
			nIDSubWorld = 320;
		}
		else if (m_BackMapTrain == 4)
		{
			nIDSubWorld = 225;
		}
		else if (m_BackMapTrain == 5)
		{
			nIDSubWorld = 226;
		}	
		else if (m_BackMapTrain == 6)
		{
			nIDSubWorld = 227;
		}	
		else if (m_BackMapTrain == 7)
		{
			nIDSubWorld = 322;
		}
		else if (m_BackMapTrain == 8)
		{
			nIDSubWorld = 321;
		}	
		else if (m_BackMapTrain == 9)
		{
			nIDSubWorld = 336;
		}
		else if (m_BackMapTrain == 10)
		{
			nIDSubWorld = 340;
		}
	}

	if (m_nTimeBackToMapTrain <= 0)
	{
		m_nTimeBackToMapTrain = g_SubWorldSet.GetGameTime() + 30*20; // 30 giay len maps train
	}
	else
	{
		if (g_SubWorldSet.GetGameTime() - m_nTimeBackToMapTrain >= 0)
		{
			m_nTimeBackToMapTrain = 0;
			C2SPLAYER_AI_BACKTOTOWN AutoCmd;
			AutoCmd.ProtocolType = c2s_aibacktotown;		
			AutoCmd.nIdSubWorld = nIDSubWorld;
			AutoCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();	
			AutoCmd.dwTimePacker = GetTickCount();
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&AutoCmd, sizeof(C2SPLAYER_AI_BACKTOTOWN));
		}
	}
}


void KPlayerAI::MoveItemToBox()
{
	int nHand = Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
	if (nHand)
	{
		ItemPos	P;
		if ( FALSE == Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &P) )		
		{
			return;
		}		
		POINT pPt;
		ItemPos newP;
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt))
		{
			newP.nPlace = pos_repositoryroom;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository1].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt) && Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount >= 1)
		{
			newP.nPlace = pos_repositoryroom1;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository2].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt) && Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount >= 2)
		{
			newP.nPlace = pos_repositoryroom2;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository3].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt) && Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount >= 3)
		{
			newP.nPlace = pos_repositoryroom3;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository4].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt) && Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount >= 4)
		{
			newP.nPlace = pos_repositoryroom4;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else if (Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_repository5].FindRoom(Item[nHand].GetWidth(), Item[nHand].GetHeight(), &pPt) && Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount >= 5)
		{
			newP.nPlace = pos_repositoryroom5;
			newP.nX = (int)pPt.x;
			newP.nY = (int)pPt.y;
			Player[CLIENT_PLAYER_INDEX].MoveItem(newP, newP);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
			if (!m_sListEquipment.FindSame(nHand))
			{
				m_sListEquipment.m_Link.Insert(nHand);
			}
			return;
		}	
	}
	else
	{
		ItemPos	P;
		PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
		
		if (pItem)
		{
			if (pItem->nPlace == pos_equiproom)
			{
				if (!m_sListEquipment.FindSame(pItem->nIdx))
				{
					if (Item[pItem->nIdx].GetGenre() == item_equip && m_SaveRAP && IsRAPEquip(Item[pItem->nIdx].GetDetailType()))
					{
						if (Item[pItem->nIdx].GetGenre() == item_goldequip)
						return;
						
						P.nPlace = pItem->nPlace;
						P.nX = pItem->nX;
						P.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
						return;
					}					
					else if ( (Item[pItem->nIdx].GetGenre() == item_equip && IsEquipSatisfyCondition(pItem->nIdx)))
					{
						if (Item[pItem->nIdx].GetGenre() == item_goldequip)
							return;

						P.nPlace = pItem->nPlace;
						P.nX = pItem->nX;
						P.nY = pItem->nY;
						Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
						return;
					}
				}
			}
		}

		while(pItem) 
		{ 
			pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
			if (pItem)
			{
				if (pItem->nPlace == pos_equiproom)
				{
					if (!m_sListEquipment.FindSame(pItem->nIdx))
					{			
						if (Item[pItem->nIdx].GetGenre() == item_equip && m_SaveRAP && IsRAPEquip(Item[pItem->nIdx].GetDetailType()))
						{
							if (Item[pItem->nIdx].GetGenre() == item_goldequip)
							return;
							
							P.nPlace = pItem->nPlace;
							P.nX = pItem->nX;
							P.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
							return;
						}
						else if ( (Item[pItem->nIdx].GetGenre() == item_equip && IsEquipSatisfyCondition(pItem->nIdx)))
						{
							if (Item[pItem->nIdx].GetGenre() == item_goldequip)
							return;
							
							P.nPlace = pItem->nPlace;
							P.nX = pItem->nX;
							P.nY = pItem->nY;
							Player[CLIENT_PLAYER_INDEX].MoveItem(P, P);
							return;
						}
					}
				}
			}
		}
	}

}

void KPlayerAI::PaintActionAuto(int nType,int nNpcID,int nX,int nY)
{
		if (nType == 1 && nNpcID > 0)
		{
			nNpcIDPaint = nNpcID;
		}
		else if(nType == 2 && nX > 0 && nY > 0)
		{
			nDestXPaint = nX;
			nDestYPaint = nY;
		}
}


#endif