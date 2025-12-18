#include "KWin32.h"

#include "Windows.h"
#include "../../core/src/coreshell.h"
#include "../../core/src/GameDataDef.h"
#include "../../core/src/CoreObjGenreDef.h"
#include "Windows.h"
#include "KEngine.h"
#include "Elem/Wnds.h"
#include "Elem/MouseHover.h"
#include "UiCase/UiPlayerBar.h"
#include "UiCase/UiStatus.h"
#include "UiCase/UiTrade.h"
#include "UiCase/UiSkills.h"
#include "UiCase/UiItem.h"
#include "UiCase/UiShop.h"
#include "UiCase/UiMsgSel.h"
#include "UiCase/UiMsgSel2.h"
#include "UiCase/UiMsgCentrePad.h"
#include "UiCase/UiInformation2.h"
#include "UiCase/UiSysMsgCentre.h"
#include "UiCase/UiTeamManage.h"
#include "UiCase/UiSelPlayerNearby.h"
#include "UiCase/UiChatCentre.h"
#include "UiCase/UiStoreBox.h"
#include "UiCase/UiTaskNote.h"
#include "UiCase/UiNewsMessage.h"
#include "UiCase/UiNewsMessageEx.h"
#include "UiCase/UiStrengthRank.h"
#include "UiCase/UiTongManager.h"
#include "UiCase/UiTongCreateSheet.h"
#include "UiCase/UiMarket.h"
#include "UiCase/UiRevive.h"
#include "UiCase/UiPlayerShop.h"
#include "UiCase/UiAutoPlay.h" //AutoAI by kinnox;
#include "UiCase/UiExpandItem.h"//ExPandBox by kinnox;
#include "UiCase/UiGiveBox.h" //GiveBox by kinnox;
#include "UiCase/UiReWard.h"//Reward by kinnox;
#include "UiCase/UiTimeBox.h"//TimeBox by kinnox;
#include "UiCase/UiTopTKNew.h"//Report Battle by kinnox;
#include "UiCase/UiNotification.h"//Notification by kinnox;
#include "UiCase/UiTrembleItem.h"//TrembleItem by kinnox;
#include "../Login/Login.h"
#include "UiCase/UiParadeItem.h"
#include "UiCase/UiGetNumber.h"
#include "UiCase/UiGetString.h"
#include "UiCase/UiTongSetTax.h"
#include "UiCase/UiMiniMap.h"
#include "UiCase/UiESCDlg.h"//Lixian by kinnox;
#include "KTongProtocol.h"

#include "../S3Client.h"
#include "UiShell.h"

#include "../../Engine/Src/Text.h"

bool UiCloseWndsInGame(bool bAll);

extern iCoreShell*		g_pCoreShell;

void GameWorldTips(unsigned int uParam, int nParam);


void CoreDataChangedCallback(unsigned int uDataId, unsigned int uParam, int nParam)
{
	KUiTrade* pTradeBar = NULL;
	switch(uDataId)
	{
	case GDCNI_HOLD_OBJECT:
		Wnd_DragFinished();
		if (uParam && ((KUiObjAtRegion*)uParam)->Obj.uGenre != CGOG_NOTHING)
		{
			KUiDraggedObject Obj;
			Obj.uGenre = ((KUiObjAtRegion*)uParam)->Obj.uGenre;
			Obj.uId = ((KUiObjAtRegion*)uParam)->Obj.uId;
			Obj.DataW = ((KUiObjAtRegion*)uParam)->Region.Width;
			Obj.DataH = ((KUiObjAtRegion*)uParam)->Region.Height;			
			Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
		}
		break;
	case GDCNI_PLAYER_BASE_INFO:
		{	
			KUiStatus* pBar = KUiStatus::GetIfVisible();
			if (pBar)
				pBar->UpdateBaseData();
			
			KUiPlayerBaseInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
			if (Info.nCurFaction >= 0)
				KUiMsgCentrePad::QueryAllChannel();
			else
			{
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Faction);
			}

			if (Info.dwCurTong != 0)
			{
				KUiMsgCentrePad::QueryAllChannel();
			}
			else
			{
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Tong);
			}
			
			if(Info.btCurBattleTeam > 0)
			{
				KUiMsgCentrePad::QueryAllChannel();
			}
			else
			{
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Song);
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Jin);
			}
		}
		break;
	case GDCNI_PLAYER_RT_ATTRIBUTE:
		{
			KUiStatus* pBar = KUiStatus::GetIfVisible();
			if (pBar)
				pBar->UpdateData();
		}
		break;
	case GDCNI_PLAYER_IMMED_ITEMSKILL:
		if (uParam)
		{
			KUiGameObject* pObj = (KUiGameObject*)uParam;
			KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
			if (pBar)
			{
				if (nParam >= 0)
					pBar->UpdateItem(nParam, pObj->uGenre, pObj->uId);
				else
					pBar->UpdateSkill(-nParam - 1, pObj->uGenre, pObj->uId);
			}
		}
		break;
	case GDCNI_OBJECT_CHANGED:
		if (uParam)
		{
			KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
			if (pObject->eContainer == UOC_ITEM_TAKE_WITH)
			{
				pTradeBar = KUiTrade::GetIfVisible();
				if (pTradeBar)
				{
					pTradeBar->OnChangedTakewithItem((KUiObjAtRegion*)uParam, nParam);
				}
				else
				{
					KUiItem* pItemsBar = KUiItem::GetIfVisible();
					if (pItemsBar)
						pItemsBar->UpdateItem((KUiObjAtRegion*)uParam, nParam);
				}
			}
			else if (pObject->eContainer == UOC_EQUIPTMENT)
			{	
				KUiStatus* pEquips = KUiStatus::GetIfVisible();
				if (pEquips)
					pEquips->UpdateEquip((KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(-1, (KUiObjAtRegion*)uParam, nParam);
			}
			//load 3 storebox by kinnox;
			else if (pObject->eContainer == UOC_STORE_BOX1)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(REPOSITORY_ONE, (KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX2)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(REPOSITORY_TWO, (KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX3)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(REPOSITORY_THREE, (KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX4)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(REPOSITORY_FOUR, (KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_STORE_BOX5)
			{
				KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
				if (pStoreBox)
					pStoreBox->UpdateItem(REPOSITORY_FIVE, (KUiObjAtRegion*)uParam, nParam);
			}
			else if (pObject->eContainer == UOC_EXPAND_BOX1)//Expandbox by kinnox;
			{
				KUiExpandItem* pExpandBox = KUiExpandItem::GetIfVisible();
				if (pExpandBox)
					pExpandBox->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}	
			//GiveBox by kinnox;
			else if (pObject->eContainer == UOC_GIVE_BOX)
			{
				KUiGiveBox* pGiveBoxItem = KUiGiveBox::GetIfVisible();
				if (pGiveBoxItem)
					pGiveBoxItem->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}		
			//end code			
			else if (pObject->eContainer == UOC_TO_BE_TRADE)
			{
				KUiTrade* pTrade = KUiTrade::GetIfVisible();
				if (pTrade)
					pTrade->OnSelfChangedItem(pObject, nParam);
			}
			else if (pObject->eContainer == UOC_IN_HAND)
			{
				Wnd_DragFinished();
				if (pObject->Obj.uGenre != CGOG_NOTHING)
				{
					KUiDraggedObject Obj;
					Obj.uGenre = pObject->Obj.uGenre;
					Obj.uId = pObject->Obj.uId;
					Obj.DataW = pObject->Region.Width;
					Obj.DataH = pObject->Region.Height;			
					Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
				}
			}
			else if (pObject->eContainer == UOC_IMMEDIA_ITEM)
			{
				KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
				if (pBar)
				{
					if (nParam)
						pBar->UpdateItem(pObject->Region.h, pObject->Obj.uGenre, pObject->Obj.uId);
					else
						pBar->UpdateItem(pObject->Region.h, CGOG_NOTHING, 0);
				}
			}
			else if (pObject->eContainer == UOC_IMMEDIA_SKILL)
			{
				KUiPlayerBar* pBar = KUiPlayerBar::GetIfVisible();
				if (pBar)
				{
					if (nParam)
						pBar->UpdateSkill(pObject->Region.h, pObject->Obj.uGenre, pObject->Obj.uId);
					else
						pBar->UpdateSkill(pObject->Region.h, CGOG_NOTHING, 0);
				}
			}
			else if (pObject->eContainer == UOC_BUILD_ITEM)//TrembleItem by kinnox;
			{
				KUiTrembleItem* pTremble = KUiTrembleItem::GetIfVisible();
				if (pTremble)
					pTremble->UpdateItem((KUiObjAtRegion*)uParam, nParam);
			}
		}
		break;
	case GDCNI_FIGHT_SKILL_POINT:	
		{
			KUiSkills* pPad = KUiSkills::GetIfVisible();
			if (pPad)
			{
				pPad->UpdateFightRemainPoint(nParam);
				// Also refresh all skill data when item bonuses change
				pPad->UpdateFightSkillData();
			}
		}
		break;
	case GDCNI_SKILL_CHANGE:			
		if (uParam)
		{
			KUiSkills::UpdateSkill((KUiSkillData*)uParam, nParam);
		}
		break;
	case GDCNI_TRADE_START:
		if (uParam)
		{
			UiCloseWndsInGame(false);
			KUiTrade::OpenWindow((KUiPlayerItem*)uParam);
		}
		break;
	case GDCNI_TRADE_DESIRE_ITEM:	
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->OnOppositeChangedItem((KUiObjAtRegion*)uParam, nParam);
		break;
	case GDCNI_TRADE_OPER_DATA:		
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->UpdateOperData();
		break;
	case GDCNI_TRADE_END:			
		pTradeBar = KUiTrade::GetIfVisible();
		if (pTradeBar)
			pTradeBar->UpdateTradeEnd(nParam);
		break;
	case GDCNI_NPC_TRADE:
		if (nParam)
			KUiShop::OpenWindow();
		else
			KUiShop::CloseWindow();
		break;
	case GDCNI_QUESTION_CHOOSE:
		{
			if(!nParam)
				KUiMsgSel::OpenWindow((KUiQuestionAndAnswer*)uParam);
			else
				KUiMsgSel2::OpenWindow((KUiQuestionAndAnswer*)uParam);
		}
		break;
	case GDCNI_GAME_START:
		{
			g_LoginLogic.NotifyToStartGame();
			Wnd_GameSpaceHandleInput(true);
			KUiMsgCentrePad::ReleaseActivateChannelAll();
			KUiMsgCentrePad::QueryAllChannel();
		}
		break;
	case GDCNI_SPEAK_WORDS:			
		if (uParam && nParam)
			g_UiInformation2.SpeakWords((KUiInformationParam*)uParam, nParam);
		break;
	case GDCNI_CHAT_GROUP:
		KUiChatCentre::UpdateData(UICC_U_ALL, 0, 0);
		break;
	case GDCNI_CHAT_FRIEND:		
		KUiChatCentre::UpdateData(UICC_U_GROUP, 0, nParam);
		break;
	case GDCNI_CHAT_FRIEND_STATUS:	
		KUiChatCentre::UpdateData(UICC_U_FRIEND, uParam, nParam);
		break;
	case GDCNI_TEAM:
		{
			KUiTeamManage* pPad = KUiTeamManage::GetIfVisible();
			if (pPad)
				pPad->UpdateData((KUiPlayerTeam*)uParam);
			if (uParam)
			{
				KUiTeamManageBar::OpenWindow();
				KUiMsgCentrePad::QueryAllChannel();
			}	
			else
				KUiMsgCentrePad::CloseSelfChannel(KUiMsgCentrePad::ch_Team);
		}
		break;
	case GDCNI_SWITCH_CURSOR:
		Wnd_SwitchCursor(nParam);
		break;
	case GDCNI_SYSTEM_MESSAGE:
		if (uParam)
			KUiSysMsgCentre::AMessageArrival((KSystemMessage*)uParam, (void*)nParam);
		break;
	case GDCNI_OPEN_STORE_BOX:
		KUiStoreBox::OpenWindow((int)uParam);
		break;
	case GDCNI_SWITCHING_SCENEPLACE:
		break;
	case GDCNI_MISSION_RECORD:
		if (uParam)
		{
			KMissionRecord* pRecord = (KMissionRecord*)uParam;
			KUiTaskNote::WakeUp(pRecord->sContent, pRecord->nContentLen, pRecord->uValue);
		}
		break;
	case GDCNI_PK_SETTING:
		break;
	case GDCNI_VIEW_PLAYERITEM:
		KUiParadeItem::OpenWindow((KUiPlayerItem*)uParam);
		break;
	case GDCNI_NEWS_MESSAGE:		
		if (uParam)
			KUiNewsMessage::MessageArrival((KNewsMessage*)uParam, (SYSTEMTIME*)nParam);
		break;
	case GDCNI_NEWS_MESSAGE_EX:		
		if (uParam)
			KUiNewsMessageEx::MessageArrival((KNewsMessage*)uParam, (SYSTEMTIME*)nParam);
		break;
	case GDCNII_RANK_INDEX_LIST_ARRIVE:
		KUiStrengthRank::OpenWindow();
		KUiStrengthRank::NewIndexArrive(uParam, (KRankIndex *)nParam);
		break;
	case GDCNII_RANK_INFORMATION_ARRIVE:
		KUiStrengthRank::NewRankArrive(uParam, (KRankMessage *)nParam);
		break;
	case GDCNI_TONG_INFO:
		KUiTongManager::TongInfoArrive((KUiPlayerRelationWithOther *)uParam, (KTongInfo *)nParam);
		break;
	case GDCNI_TONG_MEMBER_LIST:
		KUiTongManager::NewDataArrive((KUiGameObjectWithName *)uParam, (KTongMemberItem *)nParam);
		break;
	case GDCNI_TONG_ACTION_RESULT:
		KUiTongManager::ResponseResult((KUiGameObjectWithName *)uParam, nParam);
		break;
	case GDCNI_OPEN_TONG_CREATE_SHEET:
		if(uParam)
			KUiTongCreateSheet::OpenWindow(NULL);
		else
			KUiTongCreateSheet::CloseWindow();
		break;
	case GDCNI_MARKET_TRADE:
		{	
			KUiMarket* pMarket = KUiMarket::GetIfVisible();
			if(pMarket)
			{
				pMarket->UpdateData();
			}
			else
			{
				KUiMarket::OpenWindow();
			}
		}
		break;
	case GDCNI_WAIT_TO_REVIVE:
		{
			KReviveWnd::LaunchRevive();
		}
		break;
	case GDCNI_VIEW_PLAYERSHOP:
		if(uParam)
		{
			KUiPlayerShop::OpenWindow((KUiPlayerItem*)uParam);
		}
		else
		{	
			if(KUiPlayerShop::GetIfVisible() != NULL)
				KUiPlayerShop::UpdateGoods();
		}
		break;
	case GDCNI_GET_NUMBER_UI:
		if(uParam)
		{
			KUiAskClientNumber* pAsk = (KUiAskClientNumber*)uParam;
			KUiGetNumber::OpenWindow(pAsk->Title, pAsk->MinValue, pAsk->MaxValue, NULL, 0);
		}
		break;
	case GDCNI_GET_STRING_UI:
		if(uParam)
		{
			KUiAskClientString* pAsk = (KUiAskClientString*)uParam;
			KUiGetString::OpenWindow(pAsk->Title, pAsk->Init, NULL, 0, pAsk->MinLen, pAsk->MaxLen);
		}
		break;
	case GDCNI_REFRESH_MINIMAP:
		{	
			if (uParam > 0 && uParam != 325)
				MapSetMode(MINIMAP_M_BRIEF_PIC);
			else if (uParam == 325)
				MapSetMode(MINIMAP_M_NONE);
			else
				MapSetMode(MINIMAP_M_BRIEF_NOT_PIC);
			break;
		}
		break;
	case GDCNI_SAYDIALOG_END:
		{
			try
			{
				KUiMsgSel::CloseWindow(false);
				KUiMsgSel2::CloseWindow(false);
			}
			catch(...){}
		}
		break;
	case GDCNI_BUYSELL_END:
		{
			try
			{
				if(KUiShop::GetIfVisible()) 
					KUiShop::CloseWindow();
				if(KUiItem::GetIfVisible())
					KUiItem::CloseWindow(false);
				if (KUiStatus::GetIfVisible())
				KUiStatus::CloseWindow(false);
			}
			catch(...){}
		}
		break;
	case GDCNI_UPDATE_AUTO://AutoAI by kinnox;
		{
			KUiAutoPlay::UpdateData();
		}
		break;
	case GDCNI_UPDATE_PLAYERSELLITEM://AutoAI by kinnox;
		{
			KUiSupportPlayer::UpdateClientDaTa();
		}
		break;
	case GDCNI_UPDATE_STOREBOX://LockPlayer by kinnox;
		{
			try
			{
				if (KUiStoreBox::GetIfVisible())
					KUiStoreBox::UpdateClientData();
				if (KUiStatus::GetIfVisible())
					KUiStatus::UpdateClientDaTa();
			}
			catch(...){}
		}
		break;	
	case GDCNI_OPEN_GIVE_BOX://GiveBox by kinnox;
		{
			KUiGiveBoxStruc* pInfo = (KUiGiveBoxStruc*)uParam;
			KUiGiveBox::OpenWindow(pInfo->szTitle, pInfo->szInitString, pInfo->szAction1);
		}
		break;
	case GDCNI_END_GIVE_BOX://GiveBox by kinnox;
		if (KUiGiveBox::GetIfVisible())
			KUiGiveBox::CloseWindow(false);
		break;
	case GDCNI_OPEN_REWARD_BOX://Reward by kinnox;
		{
			KUiReWardBox* pInfo = (KUiReWardBox*)uParam;
			KUiReWard::OpenWindow(pInfo->szTitle, pInfo->szFunction1, pInfo->szFunction2, pInfo->szFunction3);
		}
		break;	
	case GDCNI_OPEN_TIME_BOX://TimeBox by kinnox;
		{
			KUiTimeBoxInfo* pInfo = (KUiTimeBoxInfo*)uParam;
			KUiTimeBox::OpenWindow(pInfo->szTitle, pInfo->nTime, pInfo->szAction,pInfo->nIndexNpc);
		}
		break;		
 	case GDCNI_SEND_TOP_TK_NEW_ITEM://Report Battle By Kinnox;
	   if (!KUiTopTKNew::GetIfVisible())
	   {
		KUiTopTKNew::OpenWindow();
	   }

	   if (KUiTopTKNew::GetIfVisible())
	   {
		KUiTopTKNew::UpdateDataTop((KUiTopTKNewData*)uParam);
	   }
	break;		
	case GDCNI_LIXIAN_RESULT://Lixian by Kinnox;
		try
		{
		
			KUiESCDlg::LixianOut();

		}
		catch(...){}
	break;		
	case GDCNI_OPEN_NOTIFICATION://Notification by kinnox;
		{
			KUiNotifiStruc* pInfo = (KUiNotifiStruc*)uParam;
			KUiNotification::OpenWindow(pInfo->szTitle, pInfo->szInitString);
		}
		break;
	case GDCNI_OPEN_TREMBLE://TrembleItem by Kinnox;
		{
			KUiTrembleItem::OpenWindow();
		}
		break;	
	case GDCNI_EXIT_GAME:
		UiEndGame();
		UiPostQuitMsg();
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////

void KClientCallback::CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam)
{
	CoreDataChangedCallback(uDataId, uParam, nParam);
}

typedef std::map<std::string, std::string> BLACKLIST;
BLACKLIST g_BlackListUserNames;

#define BLACKLIST_UNITNAME	 "Sæ ®en"

struct BlacklistNotify : public AddinNotify
{
	virtual int	RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends);
	virtual int	MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends);
	virtual int	DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends);
	virtual int	DeleteUnitMember(const char* Unit, const char* Name);
	virtual int	MoveUnitMember(const char* Unit, const char* Name, const char* Group);
	void	SendNotifyGroupFriend(const char* Unit, const std::string& group, const STRINGLIST& friends);
	void	SendNotifyDeleteFriend(const char* Unit, const char* Name);
};

BlacklistNotify s_LNotify;

void ClearBlackList()
{
	g_BlackListUserNames.clear();
}

void CreateBlackListUnit()
{
	ClearBlackList();
	KUiChatCentre::AddAddinUnit(BLACKLIST_UNITNAME, &s_LNotify);
}

void AddBlackList(const char* strName, const char* strGroup)
{
	if (strName && strName[0] != 0 && strGroup)
	{
		g_BlackListUserNames[strName] = strGroup;

		if (g_BlackListUserNames.size() > 0)
		{
			int nUnit = KUiChatCentre::FindUnitIndex(BLACKLIST_UNITNAME);
			if (nUnit >= 0)
			{
				KUiChatCentre::AddFriendInfo(nUnit, (char*)strName, (char*)strGroup);
			}
		}
	}
}

void RemoveBlackList(char* strName)
{
	if (strName && strName[0] != 0)
	{
		BLACKLIST::iterator i = g_BlackListUserNames.find(strName);
		if (i != g_BlackListUserNames.end())
		{
			g_BlackListUserNames.erase(i);

			int nUnit = KUiChatCentre::FindUnitIndex(BLACKLIST_UNITNAME);
			if (nUnit >= 0)
			{
				KUiChatCentre::DeleteFriend(nUnit, strName, true);
			}
		}
	}
}

bool IsInBlackName(char* strName)
{
	if (strName && strName[0] != 0)
	{
		return g_BlackListUserNames.find(strName) != g_BlackListUserNames.end();
	}

	return false;
}

int	SaveBlackListPrivateSetting(KIniFile* pFile, LPCSTR lpSection, int nStart)
{
	int i = nStart;
	char szKey[10];
	char szLine[256];
	BLACKLIST::iterator iU = g_BlackListUserNames.begin();
	while (iU != g_BlackListUserNames.end())
	{
		sprintf(szKey, "%d", i);
		sprintf(szLine, "AddBlackList(\"%s\", \"%s\")", iU->first.c_str(), iU->second.c_str());
		pFile->WriteString(lpSection, szKey, szLine);
		i++;
		iU++;
	}
	return i;
}

int	BlacklistNotify::RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends)
{
	AddinNotify::RenameUnitGroup(Unit, Name, NewName, friends);

	if (NewName && NewName[0])
		SendNotifyGroupFriend(Unit, std::string(NewName), friends);

	return 0;
}

int	BlacklistNotify::MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends)
{
	AddinNotify::MoveUnitGroup(Unit, Name, Name2, friends);

	if (Name2 && Name2[0])
		SendNotifyGroupFriend(Unit, std::string(Name2), friends);

	return 0;
}

int	BlacklistNotify::DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends)
{
	AddinNotify::DeleteUnitGroup(Unit, Name, friends);

	if (Name && Name[0])
		SendNotifyGroupFriend(Unit, "", friends);

	return 0;
}

int	BlacklistNotify::DeleteUnitMember(const char* Unit, const char* Name)
{
	AddinNotify::DeleteUnitMember(Unit, Name);

	SendNotifyDeleteFriend(Unit, Name);

	return 0;
}

int	BlacklistNotify::MoveUnitMember(const char* Unit, const char* Name, const char* Group)
{
	AddinNotify::MoveUnitMember(Unit, Name, Group);

	if (Name && Name[0])
	{
		STRINGLIST friends;
		friends.push_back(Name);
		SendNotifyGroupFriend(Unit, Group, friends);
	}

	return 0;
}

void BlacklistNotify::SendNotifyGroupFriend(const char* Unit, const std::string& group, const STRINGLIST& friends)
{
	if (strcmp(Unit, BLACKLIST_UNITNAME) == 0)
	{
		STRINGLIST::const_iterator i = friends.begin();
		while (i != friends.end())
		{
			g_BlackListUserNames[(*i)] = group;
			i++;
		}
	}
}

void BlacklistNotify::SendNotifyDeleteFriend(const char* Unit, const char* Name)
{
	if (strcmp(Unit, BLACKLIST_UNITNAME) == 0)
	{
		RemoveBlackList((char*)Name);
	}
}

///////////////////////////////////////////////////////////////////////////////

void KClientCallback::ChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc)
{
	if (!bSucc)
	{
		char szInfo[256];
		int n = sprintf(szInfo, "Xin thø lçi! TÇn sè vÉn ch­a më, kh«ng thÓ chuyÓn tin tøc!");
		KUiMsgCentrePad::SystemMessageArrival(szInfo, n);
		return;
	}

	int nIndex = -1;
	if (nChannelID == -1)
	{
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		for (int n = 0; n < nChannelDataCount; n++)
		{
			if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_GM))
			{
				nIndex = n;
				break;
			}
		}
		if (nIndex < 0)
			return;

		nChannelID = KUiMsgCentrePad::GetChannelID(nIndex);
		if (nChannelID == -1)
			return;
	}
	else
	{
		nIndex = KUiMsgCentrePad::GetChannelIndex(nChannelID);
		if (nIndex < 0)
			return;

		if (IsInBlackName(szSendName))
			return;
	}
	char pChatItemName[80];
	char pChatItemInfo[MAX_CHATITEM_INFO_LENGTH];

	ZeroMemory(pChatItemName, sizeof(pChatItemName));
	ZeroMemory(pChatItemInfo, sizeof(pChatItemInfo));
	
	KUiPlayerBar::EncodeChatItemInfo(pMsgBuff, nMsgLength, pChatItemName, pChatItemInfo);

	if(pChatItemName[0])
	{
		KUiMsgCentrePad::NewChannelMessageArrival(nChannelID, szSendName, pMsgBuff, nMsgLength, pChatItemName, pChatItemInfo);
	}
	else
	{
		KUiMsgCentrePad::NewChannelMessageArrival(nChannelID, szSendName, pMsgBuff, nMsgLength);
	}

	if (KUiMsgCentrePad::GetChannelSubscribe(nIndex) &&
		KUiMsgCentrePad::IsChannelType(nIndex, KUiMsgCentrePad::ch_Screen))
	{
		KUiPlayerItem SelectPlayer;
		int nKind = -1;
		if (g_pCoreShell->FindSpecialNPC(szSendName, &SelectPlayer, nKind) && nKind == kind_player)
		{
			g_pCoreShell->ChatSpecialPlayer(&SelectPlayer, pMsgBuff, nMsgLength);
		}
	}
}

void KClientCallback::MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc)
{
	if (!bSucc)
	{
		char szInfo[256];
		int n = sprintf(szInfo, "§¹i hiÖp %s hiÖn t¹i kh«ng cã trªn m¹ng!", szSendName);
		KUiMsgCentrePad::SystemMessageArrival(szInfo, n);
		return;
	}

	if (IsInBlackName(szSourceName))
		return;

	char pChatItemName[64];
	char pChatItemInfo[128];

	ZeroMemory(pChatItemName, sizeof(pChatItemName));
	ZeroMemory(pChatItemInfo, sizeof(pChatItemInfo));
	
	KUiPlayerBar::EncodeChatItemInfo(pMsgBuff,nMsgLength, pChatItemName, pChatItemInfo);
	
	if(pChatItemName[0])
	{
		KUiMsgCentrePad::NewMSNMessageArrival(szSourceName, szSendName, pMsgBuff, nMsgLength, pChatItemName, pChatItemInfo);
	}
	else
	{
		KUiMsgCentrePad::NewMSNMessageArrival(szSourceName, szSendName, pMsgBuff, nMsgLength);
	}
}

void KClientCallback::NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost)
{
	KUiMsgCentrePad::OpenChannel(ChannelName, channelid, cost);
}

void KClientCallback::FriendInvite(char* roleName)
{
	if (roleName && roleName[0] != 0)
	{
		KSystemMessage	sMsg;
		KUiPlayerItem	sPlayer;

		strcpy(sPlayer.Name, roleName);
		sPlayer.nIndex = -1;
		sPlayer.uId = 0;
		sPlayer.nData = 0;

		sprintf(sMsg.szMessage, MSG_CHAT_GET_FRIEND_APPLY, roleName);
		sMsg.eType = SMT_FRIEND;
		sMsg.byConfirmType = SMCT_UI_FRIEND_INVITE;
		sMsg.byPriority = 3;
		sMsg.byParamSize = sizeof(KUiPlayerItem);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
	}
}

void KClientCallback::AddFriend(char* roleName, BYTE answer)
{
	if (roleName && roleName[0] != 0)
	{
		if (answer == answerAgree)
		{
			KSystemMessage	sMsg;
			KUiPlayerItem	sPlayer;

			memset(&sPlayer, 0, sizeof(KUiPlayerItem));
			strcpy(sPlayer.Name, roleName);
			sPlayer.uId = 0;
			sPlayer.nIndex = -1;

			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_SUCCESS, roleName);
			sMsg.eType = SMT_FRIEND;
			sMsg.byConfirmType = SMCT_UI_INTERVIEW;
			sMsg.byPriority = 2;
			sMsg.byParamSize = sizeof(KUiPlayerItem);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);

			int nUnit = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnit >= 0)
			{
				KUiChatCentre::DeleteFriend(nUnit, roleName, false);
			}

			KUiChatCentre::AddFriendInfo(0, roleName, "");
			KUiChatCentre::FriendStatus(0, roleName, stateOnline);
		}
		else if (answer == answerDisagree)
		{

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_REFUSE_FRIEND, roleName);
			sMsg.eType = SMT_FRIEND;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		else if (answer == answerUnable)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_FAIL, roleName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
}

void KClientCallback::FriendStatus(char* roleName, BYTE state)
{
	if (roleName && roleName[0] != 0)
	{
		int nUnit = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
		if (nUnit >= 0 && KUiChatCentre::FriendStatus(nUnit, roleName, state))
		{
			KSystemMessage	sMsg;
			if (state == stateOnline)
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, roleName);
			else if (state == stateOffline)
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_OFFLINE, roleName);
			else
				return;

			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
}

void KClientCallback::FriendInfo(char* roleName, char* unitName, char* groupName, BYTE state)
{
	if (roleName && roleName[0] != 0 && unitName && groupName)
	{
		int nUnit = 0;
		if (unitName[0])
			nUnit = KUiChatCentre::FindUnitIndex(unitName);
		if (nUnit >= 0)
		{
			int nUnitOld = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnitOld >= 0 && nUnitOld != nUnit)
			{
				KUiChatCentre::DeleteFriend(nUnitOld, roleName, false);
			}

			KUiChatCentre::AddFriendInfo(nUnit, roleName, groupName);
			KUiChatCentre::FriendStatus(nUnit, roleName, state);
		}
	}
}

void KClientCallback::AddPeople(char* unitName, char* roleName)
{
	if (roleName && roleName[0] != 0 && unitName)
	{
		int nUnit = KUiChatCentre::FindUnitIndex(unitName);
		if (nUnit >= 0)
		{
			int nUnitOld = KUiChatCentre::FindUnitIndexByRoleNameAtServerUnit(roleName);
			if (nUnitOld >= 0 && nUnitOld != nUnit)
			{
				KUiChatCentre::DeleteFriend(nUnitOld, roleName, false);
			}
			
			KUiChatCentre::AddFriendInfo(nUnit, roleName, "");
			KUiChatCentre::FriendStatus(nUnit, roleName, stateOnline);
		}
	}
}

#define LEVEL_TIPS_INI "\\Ui\\ÔÓ»â.ini"
void LevelTips(int nNewLevel)
{
	char szMsg[256], szBuf[16];
	KIniFile Ini;
	int nMsgLen;

	if(Ini.Load(LEVEL_TIPS_INI))
	{
		Ini.GetString("LevelUpTips", _itoa(nNewLevel, szBuf, 10), "", szMsg, sizeof(szMsg));

		nMsgLen = TEncodeText(szMsg, strlen(szMsg));

		if(nMsgLen)
		{
		    KUiMsgCentrePad::SystemMessageArrival(szMsg, nMsgLen);
		}
	}
}

void FactionTips(int nbJoin)
{
	char szMsg[256] = "";
	KIniFile Ini;
	int nMsgLen;

	if(Ini.Load(LEVEL_TIPS_INI))
	{
		if (nbJoin)
			Ini.GetString("FactionTips", "Join", "", szMsg, sizeof(szMsg));

		nMsgLen = TEncodeText(szMsg, strlen(szMsg));

		if(nMsgLen)
		{
		    KUiMsgCentrePad::SystemMessageArrival(szMsg, nMsgLen);
		}
	}
}

void GameWorldTips(unsigned int uParam, int nParam)
{
	if(uParam == PBP_LEVEL)
		LevelTips(nParam);
	else if(uParam == PBP_FACTION)
		FactionTips(nParam);
}
