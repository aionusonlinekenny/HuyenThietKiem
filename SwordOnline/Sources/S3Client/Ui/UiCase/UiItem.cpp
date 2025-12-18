
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "uitrade.h"
#include "uiitem.h"
#include "UiGetMoney.h"
#include "UiStoreBox.h"
#include "UiTradeConfirmWnd.h"
#include "UiShop.h"
#include "UiSysMsgCentre.h"
#include "UiPlayerBar.h"
#include "UiMarket.h"
#include "UiStatus.h"
#include "UiMarkPrice.h"
#include "UiMakeAdv.h"
#include "UiMiniMap.h"
#include "UiPlayerShop.h"
#include "UiMarket.h"
#include "UiGetString.h"
#include "UiGetNumber.h"
#include "UiTongSetTax.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiBase.h"
#include "../ShortcutKey.h"
#include <crtdbg.h>
#include "../UiSoundSetting.h"
#include "KEngine.h"

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiItem.ini"
#define	SELL_MESSAGES	"Ên gi÷ <color=green>Shift + chuét ph¶i<color> vµo vËt phÈm, lËp tøc b¸n ®­îc vËt phÈm."

KUiItem* KUiItem::m_pSelf = NULL;


enum WAIT_OTHER_WND_OPER_PARAM
{
	UIITEM_WAIT_GETMONEY,
};

KUiItem* KUiItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiItem* KUiItem::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiItem::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		KUiShop::CancelTrade();
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}


void KUiItem::Initialize()
{	
	AddChild(&m_TitleIcon);
	AddChild(&m_Money);
	AddChild(&m_MoneyIcon);
	AddChild(&m_MakeAdvBtn);
	AddChild(&m_MarkPriceBtn);
	AddChild(&m_MakeStallBtn);
	AddChild(&m_GetMoneyBtn);
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);
	AddChild(&m_OpenStatusPadBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_ItemBox.SetContainerId((int)UOC_ITEM_TAKE_WITH);
	m_nMoney = 0;
	Wnd_AddWindow(this);
}


void KUiItem::Breathe()
{
	m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	m_Money.SetMoneyText(m_nMoney);
	//m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_JBCOIN, 0, 0);
	//m_JbCoin.SetJbCoinText(m_nJbCoin);
	m_MakeStallBtn.CheckButton(g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0));
}


void KUiItem::OnNpcTradeMode(bool bTrue)
{
	if (m_pSelf)
		m_pSelf->m_ItemBox.EnablePickPut(!bTrue);
}


void KUiItem::UpdateData()
{
	m_ItemBox.Clear();

	m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	m_Money.SetMoneyText(m_nMoney);
	//m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_JBCOIN, 0, 0);
	//m_JbCoin.SetJbCoinText(m_nJbCoin);

	KUiObjAtRegion* pObjs = NULL;
	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0);
	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (unsigned int)pObjs, nCount);
		for (int i = 0; i < nCount; i++)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId = pObjs[i].Obj.uId;
			no.DataX = pObjs[i].Region.h;
			no.DataY = pObjs[i].Region.v;
			no.DataW = pObjs[i].Region.Width;
			no.DataH = pObjs[i].Region.Height;
			m_ItemBox.AddObject(&no, 1);
		}
		free(pObjs);
		pObjs = NULL;
	}
}

void KUiItem::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId = pItem->Obj.uId;
		Obj.DataX = pItem->Region.h;
		Obj.DataY = pItem->Region.v;
		Obj.DataW = pItem->Region.Width;
		Obj.DataH = pItem->Region.Height;
		if (bAdd)
			m_ItemBox.AddObject(&Obj, 1);
		else
			m_ItemBox.RemoveObject(&Obj);
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
	}
	else
		UpdateData();
}


void KUiItem::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_TitleIcon.Init(&Ini, "TitleIcon");
		m_pSelf->m_Money.Init(&Ini, "Money");
		m_pSelf->m_MoneyIcon.Init(&Ini, "MoneyIcon");
//		m_pSelf->m_JbCoin.Init(&Ini, "JbCoin");
//		m_pSelf->m_JbCoinIcon.Init(&Ini, "JbCoinIcon");
		m_pSelf->m_MakeAdvBtn.Init(&Ini, "MakeAdvBtn");
		m_pSelf->m_MarkPriceBtn.Init(&Ini, "MarkPriceBtn");
		m_pSelf->m_MakeStallBtn.Init(&Ini, "MakeStallBtn");
		m_pSelf->m_GetMoneyBtn.Init(&Ini, "GetMoneyBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_OpenStatusPadBtn.Init(&Ini, "OpenStatus");
		m_pSelf->m_ItemBox.EnableTracePutPos(true);
	}
}


void KUiItem::OnClickItem(KUiDraggedObject* pItem, bool bDoImmed)
{
	if (pItem == NULL || g_pCoreShell == NULL)
		return;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.h = pItem->DataX;
	Obj.Region.v = pItem->DataY;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.eContainer = UOC_ITEM_TAKE_WITH;

	if (bDoImmed == false)
	{	
		if(KUiMarket::GetIfVisible() != NULL)
			return;
		
		KUiItemBuySelInfo	Price = { 0 };
		if (g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE,
			(unsigned int)(&Obj), (int)(&Price)))
		{
			KUiTradeConfirm::OpenWindow(this, &Obj, &Price, TCA_SALE);
		}
	}
	else
	{
		UISYS_STATUS eStatus = g_UiBase.GetStatus();
		if (eStatus == UIS_S_TRADE_SALE || eStatus == UIS_S_TRADE_NPC)
		{	
			if(KUiMarket::GetIfVisible() != NULL)
				return;
			
			if(eStatus == UIS_S_TRADE_NPC)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)
				{
					g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
						(unsigned int)(&Obj), 0);
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_NONE;
					Msg.byParamSize = 0;
					Msg.byPriority = 0;
					Msg.eType = SMT_NORMAL;
					Msg.uReservedForUi = 0;
					sprintf(Msg.szMessage, SELL_MESSAGES);
					KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
				}
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
						(unsigned int)(&Obj), 0);
			}
		}
		else if (g_UiBase.IsOperationEnable(UIS_O_USE_ITEM))
		{	
			if(GetKeyState(VK_CONTROL) & 0x8000)
			{	
				KUiItemBuySelInfo	Price = { 0 };
				if (g_pCoreShell->GetGameData(GDI_ITEM_BREAK_INFO,
					(unsigned int)(&Obj), (int)(&Price)))
				{
					KUiTradeConfirm::OpenWindow(this, &Obj, &Price, TCA_BREAK, 1, Price.nPrice - 1);
				}
				return;
			}
			else
			{
				if(KUiGetString::GetIfVisible() != NULL)
					return;
				if(KUiGetNumber::GetIfVisible() != NULL)
					return;
				if(KUiTongSetTax::GetIfVisible() != NULL)
					return;
				g_pCoreShell->OperationRequest(GOI_USE_ITEM,
					(unsigned int)(&Obj), UOC_ITEM_TAKE_WITH);
			}
		}
	}
}

void KUiItem::OnRepairItem(KUiDraggedObject* pItem)
{
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.h = pItem->DataX;
	Obj.Region.v = pItem->DataY;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.eContainer = UOC_ITEM_TAKE_WITH;

	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_REPAIR_ITEM_PRICE,
		(unsigned int)(&Obj), (int)(&Price)))
	{
		KUiTradeConfirm::OpenWindow(this, &Obj, &Price, TCA_REPAIR);
	}
}


void KUiItem::OnBindItem(KUiDraggedObject* pItem)
{	
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.Region.h = 0;
	Obj.eContainer = UOC_EQUIPTMENT;
	
	char szFunc[16];
	sprintf(szFunc, "OnBind");
	if(g_pCoreShell->OperationRequest(GOI_PLAYER_BIND_ITEM, (unsigned int)(&Obj), (int)szFunc))
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		KUiStatus::UpdateButton(); 
	}
}


void KUiItem::OnUnBindItem(KUiDraggedObject* pItem)
{	
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.Region.h = 0;
	Obj.eContainer = UOC_EQUIPTMENT;
	
	char szFunc[16];
	sprintf(szFunc, "OnUnBind");
	if(g_pCoreShell->OperationRequest(GOI_PLAYER_BIND_ITEM, (unsigned int)(&Obj), (int)szFunc))
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		KUiStatus::UpdateButton();
	}
}

int KUiItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_LEFT_CLICK_ITEM:
		if (g_UiBase.GetStatus() == UIS_S_TRADE_SALE)
			OnClickItem((KUiDraggedObject*)uParam, true);
		else if (g_UiBase.GetStatus() == UIS_S_TRADE_NPC)
			OnClickItem((KUiDraggedObject*)uParam, false);
		else if (g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
			OnRepairItem((KUiDraggedObject*)uParam);
		else if (g_UiBase.GetStatus() == UIS_S_BIND_ITEM)
			OnBindItem((KUiDraggedObject*)uParam);
		else if (g_UiBase.GetStatus() == UIS_S_UNBIND_ITEM)
			OnUnBindItem((KUiDraggedObject*)uParam);
		else if (g_UiBase.GetStatus() == UIS_S_MARKPRICE_ITEM)
			OnMarkPriceItem((KUiDraggedObject*)uParam);
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		OnClickItem((KUiDraggedObject*)uParam, true);
		break;
	case WND_N_ITEM_PICKDROP:
		OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{	
			if(g_UiBase.GetStatus() != UIS_S_MARKPRICE_ITEM &&
				!g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0))
			{
				Hide();
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OpenStatusPadBtn)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_STATUS);
		else if (uParam == (unsigned int)(KWndWindow*)&m_GetMoneyBtn)
		{
			if (KUiStoreBox::GetIfVisible())
				KUiGetMoney::OpenWindow(0, m_nMoney, this, UIITEM_WAIT_GETMONEY, &m_Money);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_MakeAdvBtn)
		{
			OnClickMakeAdvBtn();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_MarkPriceBtn)
		{
			OnClickMarkPriceBtn();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_MakeStallBtn)
		{
			OnClickMakeStallBtn();
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (uParam == UIITEM_WAIT_GETMONEY)
			OnGetMoney(nParam);
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiItem::OnGetMoney(int nMoney)
{	
	if (nMoney > 0 && KUiStoreBox::GetIfVisible())
	{
		g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,
			true, nMoney);
	}
}


void KUiItem::OnClickMarkPriceBtn()
{	
	if(m_MakeStallBtn.IsButtonChecked())
		return;
	if(KUiPlayerShop::GetIfVisible() != NULL)
		return;
	if (g_pCoreShell == NULL)
		return;

	if(g_pCoreShell->GetGameData(GDI_PLAYER_FIGHT_MODE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_FIGHT_MODE_ON, 0, 0);
		return;
	}

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus == UIS_S_TRADE_REPAIR)
	{	
		return;
	}
	else if (eStatus == UIS_S_TRADE_SALE)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_NPC)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_BUY)
	{
		return;
	}

	if(eStatus == UIS_S_MARKPRICE_ITEM)
	{	
		OnNpcTradeMode(false);
		g_UiBase.SetStatus(UIS_S_IDLE);
		if(m_MarkPriceBtn.IsButtonChecked())
			m_MarkPriceBtn.CheckButton(false);
	}
	else
	{	
		OnNpcTradeMode(true);
		g_UiBase.SetStatus(UIS_S_MARKPRICE_ITEM);
		if(!m_MarkPriceBtn.IsButtonChecked())
			m_MarkPriceBtn.CheckButton(true);
	}
}

void KUiItem::OnClickMakeAdvBtn()
{
	if (g_pCoreShell == NULL)
		return;

	if(g_pCoreShell->GetGameData(GDI_PLAYER_FIGHT_MODE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_FIGHT_MODE_ON, 0, 0);
		return;
	}

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus != UIS_S_IDLE)
	{	
		return;
	}

	KUiMakeAdv::OpenWindow();
}


void KUiItem::OnClickMakeStallBtn()
{	
	if (g_pCoreShell == NULL)
		return;
	
	if(KUiMarket::GetIfVisible() != NULL)
	{
		KUiMarket::CloseWindow();
	}

	if(KUiPlayerShop::GetIfVisible() != NULL)
		return;


	if(g_pCoreShell->GetGameData(GDI_PLAYER_FIGHT_MODE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_FIGHT_MODE_ON, 0, 0);
		return;
	}

	if(g_pCoreShell->GetGameData(GDI_PLAYER_RIDE_HORSE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_PLAYER_RIDEHORSE, 0, 0);
		return;
	}

	if(!g_pCoreShell->GetGameData(GDI_MAP_TYPE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_MAP_TYPE_ERROR, 0, 0);
		return;
	}

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus != UIS_S_IDLE &&
		eStatus != UIS_S_PLAYER_SHOP)
	{	
		return;
	}
	
	BOOL bOpen = g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0);

	if(!g_pCoreShell->GetGameData(GDI_PLAYER_SHOP_ADV, 0, 0))
	{	
		KUiMakeAdv::OpenWindow();
		return;
	}

	if(g_pCoreShell->OperationRequest(GOI_SHOP_WILLING, !bOpen, 0))
	{	
		OnNpcTradeMode((bool)!bOpen);
		if(!bOpen)
			g_UiBase.SetStatus(UIS_S_PLAYER_SHOP);
		else
			g_UiBase.SetStatus(UIS_S_IDLE);
	}
}

void KUiItem::OnMarkPriceItem(KUiDraggedObject* pItem)
{
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.h = pItem->DataX;
	Obj.Region.v = pItem->DataY;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.eContainer = UOC_ITEM_TAKE_WITH;

	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE,
		(unsigned int)(&Obj), (int)(&Price)))
	{
		KUiMarkPrice::OpenWindow(&Obj, &Price);
	}
}


void KUiItem::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (!g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) && 
		!g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM))
		return;
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);		
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(
			Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY;
		Pick.eContainer = UOC_ITEM_TAKE_WITH;
		if (eStatus == UIS_S_TRADE_REPAIR)
		{	
			return;
		}
		else if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_TRADE_NPC)
		{
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}
		if(GetKeyState(VK_CONTROL) & 0x8000)
		{
			KUiChatItemParam pInfo = { 0 };

			if (g_pCoreShell->GetGameData(GDI_CHAT_ITEM_INFO,
				(unsigned int)&Pick, (unsigned int)&pInfo))
			{
				KUiPlayerBar::SetChatItemInfo(pInfo, Pick.Obj.uId);
			}
			return;
		}
	}

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = pDropPos->h;
		Drop.Region.v = pDropPos->v;
		Drop.eContainer = UOC_ITEM_TAKE_WITH;	
	}
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}
//Kenny Click Phai
void KUiItem::MoveItemToRepository(KUiDraggedObject* pItem)
{
    if (!pItem || !KUiStoreBox::GetIfVisible())
        return;

    KUiObjAtContRegion Pick;
    Pick.Obj.uGenre     = pItem->uGenre;
    Pick.Obj.uId        = pItem->uId;
    Pick.Region.Width   = pItem->DataW;
    Pick.Region.Height  = pItem->DataH;
    Pick.Region.h       = pItem->DataX;
    Pick.Region.v       = pItem->DataY;
    Pick.eContainer     = UOC_ITEM_TAKE_WITH;

    KUiObjAtContRegion Drop = Pick;
    Drop.Obj.uGenre = Pick.Obj.uGenre;
    Drop.Obj.uId    = Pick.Obj.uId;
    Drop.Region.h   = -1;
    Drop.Region.v   = -1;

    KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
    if (pStoreBox)
    {
        int containerID = pStoreBox->GetCurrentContainerID();
        int curPage = pStoreBox->GetCurrentPage();
        int numOpened = g_pCoreShell->GetGameData(GDI_EXBOX_NUM, NULL, NULL);

        if (curPage >= numOpened)
        {
			KSystemMessage Msg;
			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 0;
			Msg.eType = SMT_NORMAL;
			Msg.uReservedForUi = 0;
			strcpy(Msg.szMessage, "B¹n ch­a më r­¬ng chøa nµy !");
			KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
			return;
        }

        if (containerID < UOC_STORE_BOX || containerID > UOC_STORE_BOX5)
        {
            KSystemMessage Msg;
            Msg.byConfirmType = SMCT_NONE;
            Msg.byParamSize = 0;
            Msg.byPriority = 0;
            Msg.eType = SMT_NORMAL;
            Msg.uReservedForUi = 0;
            strcpy(Msg.szMessage, "R­¬ng ®ang më kh«ng cßn chç trèng!");
            KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
            return;
        }

        Drop.eContainer = static_cast<UIOBJECT_CONTAINER>(containerID);
    }

    //g_DebugLog("[CLIENT] MoveItemToRepository: uId=%d, Pos=(%d,%d), Size=(%d x %d), SrcCont=%d, DestCont=%d", pItem->uId, Pick.Region.h, Pick.Region.v, Pick.Region.Width, Pick.Region.Height, Pick.eContainer, Drop.eContainer);

    g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
}

void KUiItem::MoveItemToInventory(KUiDraggedObject* pItem)
{
    if (!pItem)
        return;

    KUiObjAtContRegion Pick, Drop;
    Pick.Obj.uGenre = pItem->uGenre;
    Pick.Obj.uId = pItem->uId;
    Pick.Region.Width = pItem->DataW;
    Pick.Region.Height = pItem->DataH;
    Pick.Region.h = pItem->DataX;
    Pick.Region.v = pItem->DataY;

    // container hi?n t?i xác ??nh theo t?ng page m?
    KUiStoreBox* pStoreBox = KUiStoreBox::GetIfVisible();
    if (!pStoreBox)
    {
        KSystemMessage Msg;
        Msg.byConfirmType = SMCT_NONE;
        Msg.byParamSize = 0;
        Msg.byPriority = 0;
        Msg.eType = SMT_NORMAL;
        Msg.uReservedForUi = 0;
        strcpy(Msg.szMessage, "B¹n ch­a më r­¬ng chøa nµy !");
        KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
        return;
    }

    int containerID = pStoreBox->GetCurrentContainerID();
    if (containerID < UOC_STORE_BOX || containerID > UOC_STORE_BOX5)
    {
        KSystemMessage Msg;
        Msg.byConfirmType = SMCT_NONE;
        Msg.byParamSize = 0;
        Msg.byPriority = 0;
        Msg.eType = SMT_NORMAL;
        Msg.uReservedForUi = 0;
        strcpy(Msg.szMessage, "R­¬ng ®ang më kh«ng hîp lÖ !");
        KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
        return;
    }

    Pick.eContainer = static_cast<UIOBJECT_CONTAINER>(containerID);

    Drop = Pick;
    Drop.Region.h = -1;
    Drop.Region.v = -1;
    Drop.eContainer = UOC_ITEM_TAKE_WITH;

    //g_DebugLog("[CLIENT] MoveItemToInventory: uId=%d, SrcCont=%d, DestCont=%d", pItem->uId, Pick.eContainer, Drop.eContainer);

    g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
}


