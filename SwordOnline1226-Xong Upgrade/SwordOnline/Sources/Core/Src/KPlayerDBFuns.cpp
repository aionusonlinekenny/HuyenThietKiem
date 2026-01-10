
#include "KCore.h"
#ifdef _SERVER
#include "KEngine.h"
#include "KSubWorldSet.h"
#include "KSubWorld.h"
#include "KPlayer.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
#include "KNpc.h"
#include "KItem.h"
#include "KItemList.h"
#include "KItemGenerator.h"
#include "KItemSet.h"
#include "KNpcSet.h"
#include "KPlayerSet.h"
#include "KItemChangeRes.h"
#include <time.h>
//#include "MyAssert.H"
#include "KTaskFuns.h"

KList g_DBMsgList;

// --
//
// --
int KPlayer::AddDBPlayer(char * szPlayerName, int sex, DWORD * pdwID)
{
	return FALSE;
}

// --
//
// --
int KPlayer::LoadDBPlayerInfo(BYTE *pPlayerInfo, int &nStep, unsigned int &nParam)
{
	_ASSERT(pPlayerInfo);
	int nRet = 0;
	int nRetValue = 0;
	
	switch(nStep)
	{
	case STEP_BASE_INFO:
		m_pCurStatusOffset = pPlayerInfo;
		
		if ((nRet = LoadPlayerBaseInfo(pPlayerInfo, m_pCurStatusOffset, nParam)) == 1)
		{
			nRetValue = SendSyncData(nStep, nParam);
			nStep++; 
			nParam = 0;
		}
		else 
		{
			if (nRet == -1)
			{
				nStep++;
				nParam = 0;
				return 0;
			}
			else
			{
				nRetValue = SendSyncData(nStep, nParam);
			}
		}
		break;
	case STEP_FIGHT_SKILL_LIST:
		if ((nRet = LoadPlayerFightSkillList(pPlayerInfo, m_pCurStatusOffset, nParam)) == 1)
		{
			nRetValue = SendSyncData(nStep, nParam);
			nStep++;
			nParam = 0;
		}
		else
		{
			if (nRet == -1)
			{
				nStep++;
				nParam = 0;
				return 0;
			}
			else
			{
				nRetValue = SendSyncData(nStep, nParam);
			}
		}
		break;
	case STEP_LIFE_SKILL_LIST:
		if ((nRet = LoadPlayerLifeSkilllList(pPlayerInfo, m_pCurStatusOffset, nParam)) == 1)
		{
			nRetValue = SendSyncData(nStep, nParam);
			nStep++;
			nParam = 0;
		}
		else
		{
			if (nRet == -1)
			{
				nStep ++;
				nParam = 0;
				return 0;
			}
			else
			{
				nRetValue = SendSyncData(nStep, nParam);
			}
		}
		
		break;
		
	case STEP_TASK_LIST:
		if ((nRet = LoadPlayerTaskList(pPlayerInfo, m_pCurStatusOffset, nParam)) == 1)
		{
			
			int i = 0;
			for (i  = 0; i < TASKVALUE_MAXWAYPOINT_COUNT; i ++)
			{
				int nWayPoint = m_cTask.nSave[TASKVALUE_SAVEWAYPOINT_BEGIN + i];
				if (nWayPoint)
				{
					KIndexNode * pNewNode = new KIndexNode;
					pNewNode->m_nIndex = nWayPoint;
					m_PlayerWayPointList.AddTail(pNewNode);
				}
			}
			
			for (i  = 0; i < TASKVALUE_MAXSTATION_COUNT / 2; i ++)
			{
				DWORD Stations = 0;
				if (Stations = (DWORD) m_cTask.GetSaveVal(TASKVALUE_SAVESTATION_BEGIN + i))
				{
					int nStation1 = (int) HIWORD(Stations);
					int nStation2 = (int) LOWORD(Stations);
					
					if (nStation1)
					{
						KIndexNode * pNewNode = new KIndexNode;
						pNewNode->m_nIndex = nStation1;
						m_PlayerStationList.AddTail(pNewNode);
					}
					
					if (nStation2)
					{
						KIndexNode * pNewNode = new KIndexNode;
						pNewNode->m_nIndex = nStation2;
						m_PlayerStationList.AddTail(pNewNode);
					}
				}
			}
			//
			Npc[m_nIndex].LoadTimerTask();
			
			nRetValue = SendSyncData(nStep, nParam);
			nStep++;
			nParam = 0;
		}
		else
		{
			if (nRet == -1)
			{
				nStep ++;
				nParam = 0;
				return 0;
			}
			else
			{
				nRetValue = SendSyncData(nStep, nParam);
			}
		}
		break;
	case STEP_ITEM_LIST:
		if ((nRet = LoadPlayerItemList(pPlayerInfo, m_pCurStatusOffset, nParam)) == 1)
		{
			nRetValue = SendSyncData(nStep, nParam);
			nStep++;
			nParam = 0;
		}
		else
		{
			if (nRet == -1)
			{
				nStep ++;
				nParam = 0;
				return 0;
			}
			else
			{
				nRetValue = SendSyncData(nStep, nParam);
			}
		}
		break;
	default:
		nStep = STEP_SYNC_END;
		break;
	}
	return nRetValue;
}

// --
//
// --
int KPlayer::UpdateDBPlayerInfo(BYTE* pPlayerInfo)
{
	if (!pPlayerInfo)
		return -1;

	SavePlayerBaseInfo(pPlayerInfo);

	SavePlayerFightSkillList(pPlayerInfo);

	SavePlayerLifeSkilllList(pPlayerInfo);

	SavePlayerTaskList(pPlayerInfo);

	SavePlayerItemList(pPlayerInfo);

//	SavePlayerFriendList(pPlayerInfo);

	return 1;	
}

// --
//
// --
int	KPlayer::LoadPlayerBaseInfo(BYTE  * pRoleBuffer, BYTE * &pCurData, unsigned int &nParam)
{
	if (!pRoleBuffer)
	{
		KASSERT(pRoleBuffer);
		return -1;
	}
	
	TRoleData * pRoleData = (TRoleData*)pRoleBuffer;
	
	if (nParam != 0) return -1;
	
	int		nSex;
	int		nLevel;	
#define PLAYER_MALE_NPCTEMPLATEID		-1
#define PLAYER_FEMALE_NPCTEMPLATEID		-2


	nLevel = pRoleData->BaseInfo.ifightlevel;
	nSex = pRoleData->BaseInfo.bSex;
	if (nSex)
		nSex = MAKELONG(nLevel, PLAYER_FEMALE_NPCTEMPLATEID);
	else
		nSex = MAKELONG(nLevel, PLAYER_MALE_NPCTEMPLATEID);

	m_sLoginRevivalPos.m_nSubWorldID = pRoleData->BaseInfo.irevivalid;
	m_sLoginRevivalPos.m_ReviveID = pRoleData->BaseInfo.irevivalx;
	
label_retry:
	POINT Pos;
	g_SubWorldSet.GetRevivalPosFromId(m_sLoginRevivalPos.m_nSubWorldID,
		m_sLoginRevivalPos.m_ReviveID,
		&Pos);
	
	m_sLoginRevivalPos.m_nMpsX = Pos.x;
	m_sLoginRevivalPos.m_nMpsY = Pos.y;
		
	PLAYER_REVIVAL_POS	tempPos;

	if (pRoleData->BaseInfo.cUseRevive)
	{
		tempPos.m_nSubWorldID = m_sLoginRevivalPos.m_nSubWorldID;
		tempPos.m_ReviveID = m_sLoginRevivalPos.m_ReviveID;
		tempPos.m_nMpsX = m_sLoginRevivalPos.m_nMpsX;
		tempPos.m_nMpsY = m_sLoginRevivalPos.m_nMpsY;
	}
	else
	{
		tempPos.m_nSubWorldID = pRoleData->BaseInfo.ientergameid;
		tempPos.m_nMpsX = pRoleData->BaseInfo.ientergamex;
		tempPos.m_nMpsY = pRoleData->BaseInfo.ientergamey;
	}

	if (tempPos.m_nSubWorldID == 242)
	{
		m_nIndex = NpcSet.Add(pRoleData->BaseInfo.ifiveprop, nSex, 
			g_SubWorldSet.SearchWorld(242), 51616, 102304);	
	}
	else
	{
		m_nIndex = NpcSet.Add(pRoleData->BaseInfo.ifiveprop, nSex, 
			g_SubWorldSet.SearchWorld(tempPos.m_nSubWorldID),
			tempPos.m_nMpsX,
			tempPos.m_nMpsY);
	}

	if(m_nIndex <= 0) 
	{
		if (pRoleData->BaseInfo.cUseRevive)
		{
			return -1;
		}
		else
		{
			pRoleData->BaseInfo.cUseRevive = 1;
			goto label_retry;
		}
	}

	m_sDeathRevivalPos = m_sLoginRevivalPos;

	KNpc* pNpc = &Npc[m_nIndex];
	pNpc->m_Kind = kind_player;
	pNpc->SetPlayerIdx(m_nPlayerIndex);
	pNpc->m_Level = nLevel;

	strcpy(pNpc->Name, pRoleData->BaseInfo.szName);
	m_nForbiddenFlag = pRoleData->BaseInfo.nForbiddenFlag;
	
	m_nAttributePoint = pRoleData->BaseInfo.ileftprop;
	m_nSkillPoint = pRoleData->BaseInfo.ileftfight;

	m_nStrength		= pRoleData->BaseInfo.ipower;
	m_nDexterity	= pRoleData->BaseInfo.iagility;
	m_nVitality		= pRoleData->BaseInfo.iouter;
	m_nEngergy		= pRoleData->BaseInfo.iinside;
	m_nLucky		= pRoleData->BaseInfo.iluck;
	m_cTong.Clear();
	m_cTong.DBSetTongNameID(pRoleData->BaseInfo.dwTongID);

	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	m_nCurLucky = m_nLucky;
	this->SetFirstDamage();
	this->SetBaseAttackRating();
	this->SetBaseDefence();

	m_nExp			= pRoleData->BaseInfo.fightexp;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(pNpc->m_Level);
	m_dwLeadLevel	= pRoleData->BaseInfo.ileadlevel;
	m_dwLeadExp		= pRoleData->BaseInfo.ileadexp;

	m_cFaction.m_nCurFaction = (char)pRoleData->BaseInfo.nSect;
	m_cFaction.m_nFirstAddFaction = (char)pRoleData->BaseInfo.nFirstSect;
	m_cFaction.m_nAddTimes			= pRoleData->BaseInfo.ijoincount;
	pNpc->m_btRankId				= pRoleData->BaseInfo.isectrole;
	m_nWorldStat					= pRoleData->BaseInfo.nWorldStat;
	m_nSectStat						= pRoleData->BaseInfo.nSectStat;
	
	m_wTitleId						= pRoleData->BaseInfo.wtitleid;
	
	m_btRepositoryCount				= pRoleData->BaseInfo.iexboxrole;
	m_nLockPlayer					= pRoleData->BaseInfo.lockboxrole;//LockPlayer by kinnox;
	m_btExpandtoryCount				= pRoleData->BaseInfo.iexpandboxrole; //Expandbox by kinnox;	
	m_ImagePlayer   				= pRoleData->BaseInfo.ihelmres;//Player Avatar by kinnox;

	int nCashMoney = 0;
	int nSaveMoney = 0;
	this->m_ItemList.Init(GetPlayerIndex());// them 17/09/2023 by kinnox; khong biet dung hay sai;
	nCashMoney		= pRoleData->BaseInfo.imoney;
	nSaveMoney		= pRoleData->BaseInfo.isavemoney;
	m_ItemList.SetMoney(nCashMoney, nSaveMoney,0);

	pNpc->m_Series	= pRoleData->BaseInfo.ifiveprop;
	pNpc->m_Camp	= pRoleData->BaseInfo.iteam;
	
	pNpc->m_nSex	= pRoleData->BaseInfo.bSex;

	pNpc->m_LifeMax	= pRoleData->BaseInfo.imaxlife;
	pNpc->m_StaminaMax = pRoleData->BaseInfo.imaxstamina;
	pNpc->m_ManaMax = pRoleData->BaseInfo.imaxinner;

	pNpc->m_LifeReplenish = PLAYER_LIFE_REPLENISH;
	pNpc->m_ManaReplenish = PLAYER_MANA_REPLENISH;
	pNpc->m_StaminaGain = PLAYER_STAMINA_GAIN;
	pNpc->m_StaminaLoss = PLAYER_STAMINA_LOSS;

	this->SetBaseResistData();
	SetBaseSpeedAndRadius();
	pNpc->RestoreNpcBaseInfo();
	
	pNpc->m_CurrentLife = pRoleData->BaseInfo.icurlife;
	pNpc->m_CurrentMana = pRoleData->BaseInfo.icurinner;
	pNpc->m_CurrentStamina = pRoleData->BaseInfo.icurstamina;

	m_cPK.SetNormalPKState(pRoleData->BaseInfo.cPkStatus);
	m_cPK.SetPKValue(pRoleData->BaseInfo.ipkvalue);

	m_BuyInfo.Clear();
	m_cMenuState.Release();
	m_cChat.Release();
	m_cTeam.Release();
	m_cTeam.SetCanTeamFlag(m_nPlayerIndex, TRUE);
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
	m_nAvailableAnswerNum = 0;
	Npc[m_nIndex].m_ActionScriptID = 0;
	Npc[m_nIndex].m_TrapScriptID = 0;
	m_nViewEquipTime = 0;

	pNpc->m_Experience = 0;

	pNpc->m_WeaponType = g_ItemChangeRes.GetWeaponRes(0, -1);
	pNpc->m_ArmorType = g_ItemChangeRes.GetArmorRes(-1);
	pNpc->m_HelmType = g_ItemChangeRes.GetHelmRes(-1);
	pNpc->m_HorseType = g_ItemChangeRes.GetHorseRes(-1);
	pNpc->m_bRideHorse = FALSE;
	nParam = 1;
	pCurData = (BYTE *)&pRoleData->pBuffer;
	pNpc->m_FightMode = pRoleData->BaseInfo.cFightMode;
	if (pRoleData->BaseInfo.cIsExchange)
	{
	}
	//
//	this->InitLog(); fix bo by kinnox;
	return 1;
}
// --
//
// --
int	KPlayer::LoadPlayerItemList(BYTE * pRoleBuffer , BYTE* &pItemBuffer, unsigned int &nParam)
{
	KASSERT(pRoleBuffer);
	
	int nItemCount = ((TRoleData *)pRoleBuffer)->nItemCount;
	TDBItemData* pItemData = (TDBItemData *)pItemBuffer;
	
	if(nItemCount == 0) 
		return 1;

	if(nParam != 0)
	{
		if(nParam >= nItemCount )
			return -1;
	}

	int nLocal = 0;
	int nItemX = 0;
	int nItemY = 0;
	int nBegin = nParam;
	int nEnd = nParam + DBLOADPERTIME_ITEM;
	
	if(nEnd > nItemCount)
		nEnd = nItemCount;

	nParam = nEnd;		
	
	KItem NewItem;	
	int i;
	for(i = nBegin ; i < nEnd; i ++)
	{	
		ZeroMemory(&NewItem, sizeof(KItem));
		
		NewItem.m_CommonAttrib.cGenre			= (char)pItemData->igenre;
		NewItem.m_CommonAttrib.nDetailType		= (short)pItemData->idetailtype;
		NewItem.m_CommonAttrib.bLevel			= (BYTE)pItemData->ilevel;
		NewItem.m_CommonAttrib.cSeries			= (char)pItemData->iseries;
		NewItem.m_wRecord						= (WORD)pItemData->irecord;
		
		nLocal									= pItemData->ilocal;
		nItemX									= pItemData->ix;
		nItemY									= pItemData->iy;

		//	
		NewItem.m_GeneratorParam.dwRandomSeed		= pItemData->irandseed;
		NewItem.m_GeneratorParam.nGeneratorLevel[0] = pItemData->imagiclevel1;
		NewItem.m_GeneratorParam.nGeneratorLevel[1] = pItemData->imagiclevel2;
		NewItem.m_GeneratorParam.nGeneratorLevel[2] = pItemData->imagiclevel3;
		NewItem.m_GeneratorParam.nGeneratorLevel[3] = pItemData->imagiclevel4;
		NewItem.m_GeneratorParam.nGeneratorLevel[4] = pItemData->imagiclevel5;
		NewItem.m_GeneratorParam.nGeneratorLevel[5] = pItemData->imagiclevel6;
		NewItem.m_GeneratorParam.nLuck				= pItemData->ilucky;
		NewItem.m_GeneratorParam.nVersion			= pItemData->iversion;
		
		BOOL bGetEquiptResult = 0;
		switch(pItemData->igenre)
		{
			case item_equip:
				bGetEquiptResult = ItemGen.Gen_ExistEquipment(
						NewItem.m_wRecord,
						NewItem.m_CommonAttrib.nDetailType,
						NewItem.m_CommonAttrib.cSeries,					
						NewItem.m_GeneratorParam.nGeneratorLevel, 
						NewItem.m_GeneratorParam.nLuck,
						NewItem.m_GeneratorParam.nVersion,
						&NewItem);
				break;
			case item_purpleequip:
				bGetEquiptResult = ItemGen.Gen_ExistPurpleEquipment(
						NewItem.m_wRecord,
						NewItem.m_CommonAttrib.nDetailType,
						NewItem.m_CommonAttrib.cSeries,
						NewItem.m_GeneratorParam.nGeneratorLevel, 
						NewItem.m_GeneratorParam.nLuck,
						NewItem.m_GeneratorParam.nVersion,
						&NewItem);
				break;
			case item_goldequip:
				bGetEquiptResult = ItemGen.Gen_GoldEquipment(NewItem.m_wRecord, NewItem.m_CommonAttrib.cSeries, &NewItem);
				break;
			case item_platinaequip:
				break;
			case item_brokenequip:
				bGetEquiptResult = ItemGen.Gen_BrokenEquipment(
						NewItem.m_wRecord,
						NewItem.m_CommonAttrib.nDetailType,
						-1,
						NewItem.m_CommonAttrib.cSeries,
						0,
						NewItem.m_GeneratorParam.nGeneratorLevel, 
						NewItem.m_GeneratorParam.nLuck,
						NewItem.m_GeneratorParam.nVersion,
						FALSE,
						&NewItem);
				break;
			case item_medicine:			
				bGetEquiptResult = ItemGen.Gen_ExistMedicine(NewItem.m_wRecord, &NewItem);
				break;
			case item_task:				
				bGetEquiptResult = ItemGen.Gen_ExistQuest(NewItem.m_wRecord, &NewItem);
				break;
			case item_script:
				bGetEquiptResult = ItemGen.Gen_ExistScript(NewItem.m_wRecord, &NewItem);
				break;
			case item_mine:
				bGetEquiptResult = ItemGen.Gen_ExistMine(NewItem.m_wRecord, NewItem.m_CommonAttrib.bLevel, NewItem.m_CommonAttrib.cSeries, &NewItem);
				break;
			default:
				break;
		}

		if(pItemData->idurability)
			NewItem.SetDurability(pItemData->idurability);
		
		if(pItemData->iexpiredtime)
		{
			NewItem.SetTime(pItemData->iexpiredtime);
		}
		if(pItemData->ibindstate)
		{
			NewItem.SetBindState(pItemData->ibindstate);
			if( NewItem.CheckVaildTime(TRUE) == FALSE )
				NewItem.SetBindState(0);
			
		}
		if(pItemData->ishopprice)
			NewItem.SetPlayerShopPrice(pItemData->ishopprice);

		pItemData++;
		
		if( NewItem.GetTime() )
			if( NewItem.CheckVaildTime() == FALSE )
				continue;

		const int nGameIndex = ItemSet.Add(&NewItem);
		if( (nGameIndex <= 0) || (nGameIndex >= MAX_ITEM) ) 
			continue ;
		m_ItemList.Add(nGameIndex, nLocal, nItemX, nItemY);		
	}

	pItemBuffer	= (BYTE*)pItemData;

	if(nParam >= nItemCount)
		return 1;
	else 
		return 0;
}

// --
//
// --
int	KPlayer::LoadPlayerFightSkillList(BYTE * pRoleBuffer, BYTE * &pFightBuffer, unsigned int& nParam)
{
	KASSERT(pRoleBuffer);
	int nSkillCount = 0;
	char szSkillId[100];
	char szSkillLevel[100];
	nSkillCount = ((TRoleData*)(pRoleBuffer))->nFightSkillCount;
	
	if (nSkillCount == 0)	
		return 1;
	
	if (nParam >= nSkillCount )	
		return -1;
	
	int nBegin	= nParam;
	int nEnd	= nBegin + DBLOADPERTIME_SKILL;
	if (nEnd > nSkillCount) 	
		nEnd = nSkillCount;
	nParam = nEnd;
	TDBSkillData * pSkillData = NULL;
	if (nBegin == 0)
	{
		pSkillData = (TDBSkillData *)((BYTE*)pRoleBuffer + ((TRoleData*)pRoleBuffer)->dwFSkillOffset);
	}
	else
	{
		pSkillData = (TDBSkillData*)pFightBuffer;
	}

	for (int i = nBegin ; i < nEnd; i ++, pSkillData ++ )
	{
		int nSkillId = 0;
		int nSkillLevel = 0;
		nSkillId = pSkillData->m_nSkillId;
		nSkillLevel = pSkillData->m_nSkillLevel;
		Npc[m_nIndex].m_SkillList.Add(nSkillId, nSkillLevel);
	}

	pFightBuffer = (BYTE*) pSkillData;

	if (nParam >= nSkillCount) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

int	KPlayer::LoadPlayerLifeSkilllList(BYTE * pRoleBuffer, BYTE * &pFriendBuffer, unsigned int& nParam)
{
	KASSERT(pRoleBuffer);
	int nSkillCount = 0;
	char szSkillId[100];
	char szSkillLevel[100];
	nSkillCount = ((TRoleData*)(pRoleBuffer))->nLiveSkillCount;
	
	if (nSkillCount == 0)
		return 1;

	if (nParam >= nSkillCount )		
		return -1;

	int nBegin	= nParam;
	int nEnd	= nBegin + DBLOADPERTIME_SKILL;
	if (nEnd > nSkillCount) 	nEnd = nSkillCount;
	nParam = nEnd;

	TDBSkillData * pSkillData = (TDBSkillData*)pFriendBuffer;
	for (int i = nBegin ; i < nEnd; i ++, pSkillData ++ )
	{
		int nSkillId = 0;
		int nSkillLevel = 0;
		nSkillId = pSkillData->m_nSkillId;
		nSkillLevel = pSkillData->m_nSkillLevel;
		Npc[m_nIndex].m_SkillList.Add(nSkillId, nSkillLevel);
	}
	pFriendBuffer = (BYTE*) pSkillData;

	if (nParam >= nSkillCount) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}

}

// --
//
// --
int	KPlayer::LoadPlayerFriendList(BYTE * pRoleBuffer, BYTE * &pFriendBuffer, unsigned int &nParam)
{
	KASSERT(pRoleBuffer);

	if (nParam == 1)
		return 0;
	if (nParam == 2)
		return 1;

	int		nFriendCount;
	DWORD	dwID;
	char	szBuffer[32], szName[32];
	nFriendCount = ((TRoleData*)pRoleBuffer)->nFriendCount;
	if (nFriendCount == 0)
	{
		nParam = 2;
		return 1;
	}
	char * pCurFriend = (char *)pFriendBuffer;
	for (int i = 0; i < nFriendCount; i++)
	{
		
		strcpy(szName, (char *)pFriendBuffer);
		if (!szName[0])
			continue;
		dwID = g_FileName2Id(szName);
		m_cChat.DataBaseAddOne(this->m_nPlayerIndex, dwID, szName);
		pFriendBuffer += (strlen(szName) + 1);
	}
	this->m_cChat.StartSendFriendData();

	nParam = 1;
	return 0;
}

// --
//
// --
int	KPlayer::LoadPlayerTaskList(BYTE * pRoleBuffer, BYTE * &pTaskBuffer, unsigned int& nParam)
{
	KASSERT(pRoleBuffer);
	while(m_PlayerStationList.GetHead())
	{
		KIndexNode * pNode = (KIndexNode * ) m_PlayerStationList.GetHead();
		m_PlayerStationList.RemoveHead();
		delete pNode;
		pNode = NULL; //them code 16/09/2023 by kinnox;
	}
	while(m_PlayerWayPointList.GetHead())
	{
		KIndexNode * pNode = (KIndexNode * ) m_PlayerWayPointList.GetHead();
		m_PlayerWayPointList.RemoveHead();
		delete pNode;
		pNode = NULL; //them code 16/09/2023 by kinnox;
	}

	if(nParam == 0)	m_cTask.Release();

	int nTaskCount = 0;
	int nTaskId = 0;
	int nTaskDegee = 0;
	char szTaskIDKey[100];
	char szTaskValueKey[100];
	nTaskCount = ((TRoleData* )pRoleBuffer)->nTaskCount;

	if (nTaskCount == 0) 
		return 1;
	if (nParam >= nTaskCount ) 
		return -1;

	int nBegin	= nParam;
	int nEnd	= nBegin + DBLOADPERTIME_TASK;
	if (nEnd > nTaskCount) 
		nEnd = nTaskCount;
	nParam = nEnd;
	TDBTaskData * pTaskData = (TDBTaskData*) pTaskBuffer;
	int i, j;
	for(i = nBegin; i < nEnd; i ++ , pTaskData ++)
	{
		nTaskId = pTaskData->m_nTaskId;
		nTaskDegee = pTaskData->m_nTaskValue;
		if(nTaskId >= MAX_TASK) 
			continue;

		m_cTask.SetSaveVal(nTaskId, nTaskDegee);

		if(PlayerSet.m_nSyncTaskCount > 0)
		{
			bool bMatch = false;
			for(j = 0; j < PlayerSet.m_nSyncTaskCount; j++)
			{
				if(nTaskId == PlayerSet.m_pSyncTask[j].m_nID)
				{
					bMatch = true;
					break;
				}
			}
			if(bMatch)
				this->TaskSetSaveVal(nTaskId, nTaskDegee);
		}
	}
	pTaskBuffer = (BYTE*) pTaskData;
	if(nParam >= nTaskCount) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

// --
//
// --
int	KPlayer::SavePlayerBaseInfo(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);
	if (m_nIndex <= 0) return -1;

	KNpc * pNpc = &Npc[m_nIndex];
	TRoleData * pRoleData = (TRoleData*)pRoleBuffer;
	memset(pRoleData, 0, sizeof(TRoleData));
	pRoleData->bBaseNeedUpdate = 1;
	strcpy(pRoleData->BaseInfo.szName, m_PlayerName);
	if (m_AccoutName[0])
		strcpy(pRoleData->BaseInfo.caccname, m_AccoutName);
	pRoleData->BaseInfo.nForbiddenFlag = m_nForbiddenFlag;
	pRoleData->BaseInfo.ileftprop = m_nAttributePoint;
	pRoleData->BaseInfo.ileftfight = m_nSkillPoint;
	pRoleData->BaseInfo.ipower = m_nStrength;
	pRoleData->BaseInfo.iagility = m_nDexterity;
	pRoleData->BaseInfo.iouter = m_nVitality;
	pRoleData->BaseInfo.iinside	=  m_nEngergy;
	pRoleData->BaseInfo.iluck = m_nLucky;
	pRoleData->BaseInfo.dwTongID = m_cTong.GetTongNameID();

	pRoleData->BaseInfo.fightexp = m_nExp;
	pRoleData->BaseInfo.ifightlevel = pNpc->m_Level;
	
	pRoleData->BaseInfo.ileadlevel = m_dwLeadLevel;
	pRoleData->BaseInfo.ileadexp =	m_dwLeadExp;

	pRoleData->BaseInfo.nSect =		m_cFaction.m_nCurFaction;
	pRoleData->BaseInfo.nFirstSect = 	m_cFaction.m_nFirstAddFaction;
	pRoleData->BaseInfo.ijoincount = m_cFaction.m_nAddTimes;
	pRoleData->BaseInfo.isectrole = pNpc->m_btRankId;
	pRoleData->BaseInfo.nWorldStat	= m_nWorldStat;
	pRoleData->BaseInfo.nSectStat	= m_nSectStat;
	pRoleData->BaseInfo.iexboxrole = m_btRepositoryCount;
	pRoleData->BaseInfo.lockboxrole = m_nLockPlayer;//LockPlayer by kinnox;
	pRoleData->BaseInfo.iexpandboxrole = m_btExpandtoryCount;//Expandbox by kinnox;
	pRoleData->BaseInfo.ihelmres =  m_ImagePlayer;//Player Avatar by kinnox;
	//	
	int nCashMoney = 0;
	int nSaveMoney = 0;
	
	nCashMoney  = m_ItemList.GetMoney(room_equipment);
	nSaveMoney	= m_ItemList.GetMoney(room_repository);
	pRoleData->BaseInfo.imoney			= nCashMoney;
	pRoleData->BaseInfo.isavemoney		= nSaveMoney;
	pRoleData->BaseInfo.ifiveprop		= pNpc->m_Series;
	pRoleData->BaseInfo.iteam			= pNpc->m_Camp;
	pRoleData->BaseInfo.bSex			= pNpc->m_nSex;
	pRoleData->BaseInfo.imaxlife		= pNpc->m_LifeMax;
	pRoleData->BaseInfo.imaxstamina		= pNpc->m_StaminaMax;
	pRoleData->BaseInfo.imaxinner		= pNpc->m_ManaMax;
	pRoleData->BaseInfo.icurlife		= pNpc->m_CurrentLife;
	pRoleData->BaseInfo.icurinner		= pNpc->m_CurrentMana;
	pRoleData->BaseInfo.icurstamina		= pNpc->m_CurrentStamina;

	pRoleData->BaseInfo.irevivalid = 	m_sLoginRevivalPos.m_nSubWorldID;
	pRoleData->BaseInfo.irevivalx = 	m_sLoginRevivalPos.m_ReviveID;
	pRoleData->BaseInfo.irevivaly = 	0;

	if (m_bExchangeServer)
	{
		pRoleData->BaseInfo.cUseRevive = 0;
		pRoleData->BaseInfo.ientergameid = m_sExchangePos.m_dwMapID;
		pRoleData->BaseInfo.ientergamex = m_sExchangePos.m_nX;
		pRoleData->BaseInfo.ientergamey = m_sExchangePos.m_nY;
		pRoleData->BaseInfo.cFightMode = (BYTE)pNpc->m_FightMode;
	}
	else if (pNpc->m_SubWorldIndex >= 0 && pNpc->m_RegionIndex >= 0 && pNpc->m_Doing != do_death && pNpc->m_Doing != do_revive)
	{
		pRoleData->BaseInfo.cUseRevive = m_bUseReviveIdWhenLogin;
		pRoleData->BaseInfo.ientergameid = SubWorld[pNpc->m_SubWorldIndex].m_SubWorldID;
		pNpc->GetMpsPos(&pRoleData->BaseInfo.ientergamex, &pRoleData->BaseInfo.ientergamey);
		pRoleData->BaseInfo.cFightMode = (BYTE)pNpc->m_FightMode;
	}
	else
	{
		pRoleData->BaseInfo.cUseRevive = 1;
		pRoleData->BaseInfo.cFightMode = 0;
		if (pNpc->m_Doing == do_death || pNpc->m_Doing == do_revive)
		{
			pRoleData->BaseInfo.icurlife = pNpc->m_LifeMax;
			pRoleData->BaseInfo.icurinner = pNpc->m_ManaMax;
			pRoleData->BaseInfo.icurstamina = pNpc->m_StaminaMax;
		}
	}

	pRoleData->BaseInfo.cPkStatus = (BYTE)m_cPK.GetNormalPKState();
	pRoleData->BaseInfo.ipkvalue = m_cPK.GetPKValue();
	pRoleData->dwFSkillOffset = (BYTE * )pRoleData->pBuffer - (BYTE *)pRoleBuffer;
	
	pRoleData->BaseInfo.btTransLife		= (BYTE)this->TaskGetSaveVal(TV_TRANSLIFE);
	pRoleData->BaseInfo.wtitleid		= this->m_wTitleId;
	
	return 1;
	
}

// --
//
// --
int	KPlayer::SavePlayerItemList(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);

	TRoleData* pRoleData = (TRoleData*)pRoleBuffer;
	TDBItemData* pItemData = (TDBItemData*)((BYTE*)pRoleData + pRoleData->dwItemOffset);

	int nIdx = 0, nItemIndex = 0, nItemCount = 0;
	while(1)
	{
		nIdx = m_ItemList.m_UseIdx.GetNext(nIdx);
		if(nIdx == 0 )
			break;
		nItemIndex = m_ItemList.m_Items[nIdx].nIdx;
		
		pItemData->igenre =  Item[nItemIndex].GetGenre();
		pItemData->idetailtype =  Item[nItemIndex].GetDetailType();
		pItemData->ilevel =  Item[nItemIndex].GetLevel();
		pItemData->iseries =  Item[nItemIndex].GetSeries();
		pItemData->ilocal =  m_ItemList.m_Items[nIdx].nPlace;
		pItemData->ix =  m_ItemList.m_Items[nIdx].nX;
		pItemData->iy =  m_ItemList.m_Items[nIdx].nY;
		pItemData->iversion = Item[nItemIndex].GetGeneratorParam()->nVersion;
		pItemData->irandseed = Item[nItemIndex].GetGeneratorParam()->dwRandomSeed;
		pItemData->imagiclevel1 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[0];
		pItemData->imagiclevel2 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[1];
		pItemData->imagiclevel3 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[2];
		pItemData->imagiclevel4 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[3];
		pItemData->imagiclevel5 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[4];
		pItemData->imagiclevel6 =  Item[nItemIndex].GetGeneratorParam()->nGeneratorLevel[5];
		pItemData->ilucky = 	 Item[nItemIndex].GetGeneratorParam()->nLuck;
		pItemData->idurability = Item[nItemIndex].GetDurability();
		//
		pItemData->irecord			= Item[nItemIndex].GetRecord();
		//
		pItemData->iparam1			= Item[nItemIndex].GetParam1();
		//
		pItemData->ibindstate		= Item[nItemIndex].GetBindState();
		//
		pItemData->iexpiredtime		= Item[nItemIndex].GetTime();
		pItemData->ishopprice		= Item[nItemIndex].GetPlayerShopPrice();
		
		pItemData++;
		nItemCount++;
	}
	
	pRoleData->nItemCount = nItemCount;
	
	pRoleData->dwFriendOffset = (BYTE*)pItemData - (BYTE*)pRoleData;
	pRoleData->dwDataLen = (BYTE*)pItemData - (BYTE*)pRoleBuffer;
	return 1;
}

// --
//
// --
int	KPlayer::SavePlayerFightSkillList(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);
	if (m_nIndex <= 0) 
		return FALSE;
	TRoleData * pRoleData = (TRoleData *)pRoleBuffer;
	TDBSkillData * pSkillData = (TDBSkillData *) (pRoleBuffer + pRoleData->dwFSkillOffset);
	int nCount = Npc[m_nIndex].m_SkillList.UpdateDBSkillList((BYTE *)pSkillData);
	if (nCount > 0)
	{
		pRoleData->nFightSkillCount = nCount;
		pRoleData->dwLSkillOffset = (BYTE*)pSkillData - pRoleBuffer + sizeof(TDBSkillData) * nCount;
	}
	else
	{
		pRoleData->nFightSkillCount  = 0;
		pRoleData->dwLSkillOffset = (BYTE*)pSkillData - pRoleBuffer;
	}
	return 1;
}

// --
//
// --
int	KPlayer::SavePlayerLifeSkilllList(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);
	TRoleData * pRoleData = (TRoleData *)pRoleBuffer;
	TDBSkillData * pSkillData = (TDBSkillData *) (pRoleBuffer + pRoleData->dwLSkillOffset);
	pRoleData->dwTaskOffset = (BYTE*)pSkillData - pRoleBuffer;
	return 1;
}

// --
//
// --
int	KPlayer::SavePlayerFriendList(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);

	int		nFriendNo;
	char	szBuffer[32];
	CChatFriend	*pFriend;
	TRoleData * pRoleData = (TRoleData *)pRoleBuffer;

	nFriendNo = m_cChat.m_cFriendTeam[0].m_nFriendNo;
	char * pCurFriend = (char *)((BYTE *)pRoleData + pRoleData->dwFriendOffset);
	int nTotalFriendNo  = 0;
	pFriend = (CChatFriend*)m_cChat.m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pFriend)
	{
		nTotalFriendNo++;
		strcpy(pCurFriend, pFriend->m_szName);
		pCurFriend += strlen(pFriend->m_szName) + 1;
		pFriend = (CChatFriend*)pFriend->GetNext();
	}
	pRoleData->nFriendCount = nTotalFriendNo;
	_ASSERT(nTotalFriendNo == nFriendNo);
	pRoleData->dwDataLen = (BYTE*)pCurFriend - (BYTE*)pRoleBuffer;
	return 1;
}

// --
//
// --
int	KPlayer::SavePlayerTaskList(BYTE * pRoleBuffer)
{
	_ASSERT(pRoleBuffer);
	TRoleData * pRoleData = (TRoleData *) pRoleBuffer;
	int nTaskCount = 0;
	int nTaskId = 0;
	int nTaskDegee = 0;
	char szTaskIDKey[100];
	char szTaskValueKey[100];

	KIndexNode * pNode = (KIndexNode*)m_PlayerStationList.GetHead();
	int n = 0;
	memset(&m_cTask.nSave[TASKVALUE_SAVESTATION_BEGIN], 0, (TASKVALUE_MAXSTATION_COUNT / 2) * sizeof(int));
	memset(&m_cTask.nSave[TASKVALUE_SAVEWAYPOINT_BEGIN], 0, TASKVALUE_MAXWAYPOINT_COUNT * sizeof(int));

	while(pNode)
	{
		if (n >= TASKVALUE_MAXSTATION_COUNT) break;
		DWORD dwValue = m_cTask.nSave[TASKVALUE_SAVESTATION_BEGIN + n / 2];
		DWORD OrValue = pNode->m_nIndex ;
		OrValue = OrValue << ((n % 2) * 16);
		dwValue = dwValue | OrValue;
		m_cTask.nSave[TASKVALUE_SAVESTATION_BEGIN + n / 2] = dwValue;
		n++;
		pNode = (KIndexNode*)pNode->GetNext();
	}
	
	n = 0;
	pNode = (KIndexNode*) m_PlayerWayPointList.GetHead();
	while(pNode)
	{
		if (n >= TASKVALUE_MAXWAYPOINT_COUNT) break;
		if (pNode->m_nIndex == 0) continue;
		m_cTask.nSave[TASKVALUE_SAVEWAYPOINT_BEGIN + n] = pNode->m_nIndex;
		n++;
		pNode = (KIndexNode*)pNode->GetNext();
	}
	//
	Npc[m_nIndex].SaveTimerTask();
	

	TDBTaskData * pTaskData = (TDBTaskData *)(pRoleBuffer + pRoleData->dwTaskOffset);
	for (int i = 0; i < MAX_TASK; i ++)
	{
		if (!m_cTask.nSave[i]) 
			continue;
		
		pTaskData->m_nTaskId = i;
		pTaskData->m_nTaskValue = m_cTask.GetSaveVal(i);
		nTaskCount ++;
		pTaskData ++;
	}
		
	pRoleData->nTaskCount = nTaskCount;
	pRoleData->dwItemOffset = (BYTE *)pTaskData - pRoleBuffer;
	return 1;
}
#endif

