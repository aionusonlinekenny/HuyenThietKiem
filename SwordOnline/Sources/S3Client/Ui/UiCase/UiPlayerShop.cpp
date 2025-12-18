
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiPlayerShop.h"
#include "UiItem.h"
#include "UiTradeConfirmWnd.h"
#include "UiMiniMap.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiInformation.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI		"UiPlayerShop.ini"

KUiPlayerShop* KUiPlayerShop::m_pSelf = NULL;

KUiPlayerShop::KUiPlayerShop()
{
	m_pObjsList				= NULL;
	m_nObjCount				= 0;
}

KUiPlayerShop* KUiPlayerShop::GetIfVisible()
{
	return m_pSelf;
}

KUiPlayerShop* KUiPlayerShop::OpenWindow(KUiPlayerItem* pDest)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPlayerShop;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{	
		m_pSelf->m_Dest = *pDest;
		m_pSelf->m_OwnerName.SetText(m_pSelf->m_Dest.Name);
		g_UiBase.SetStatus(UIS_S_PLAYER_SHOP);
		m_pSelf->UpdateGoods();
		m_pSelf->BringToTop();
		m_pSelf->Show();

		if (KUiItem::GetIfVisible() == NULL)
		{
			KUiItem::OpenWindow();
		}
		KUiItem::OnNpcTradeMode(true);
	}
	return m_pSelf;
}

void KUiPlayerShop::CloseWindow()
{
	if (m_pSelf)
	{	
		KUiItem::OnNpcTradeMode(false);
		KUiTradeConfirm::CloseWindow(true);
		g_UiBase.SetStatus(UIS_S_IDLE);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiPlayerShop::Breathe()
{
	if(!g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, m_Dest.nIndex, 0) ||
		!g_pCoreShell->GetGameData(GDI_PLAYER_SHOP_ITEM_COUNT, 0, 0))
	{
		CancelTrade();
	}
}

void KUiPlayerShop::Initialize()
{
	AddChild(&m_ItemsBox);
	AddChild(&m_OwnerName);
	AddChild(&m_CloseBtn);
	
	m_ItemsBox.SetContainerId((int)UOC_NPC_SHOP);
	Wnd_AddWindow(this);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

void KUiPlayerShop::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{		
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\"SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			m_pSelf->KWndShowAnimate::Init(&Ini, "Main");
			m_pSelf->m_ItemsBox.Init(&Ini, "ItemBox");
			m_pSelf->m_OwnerName.Init(&Ini,"Owner");
			m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");

			m_pSelf->m_ItemsBox.EnablePickPut(false);
		}
	}
}

void KUiPlayerShop::CancelTrade()
{	
	if(!g_pCoreShell)
		return;
	g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERSHOP_END, 0, 0);
	CloseWindow();
}

int	KUiPlayerShop::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{
			CancelTrade();
		}
		break;
	case WND_N_LEFT_CLICK_ITEM:
		OnBuyItem((KUiDraggedObject*)uParam);
		break;
	case WND_M_OTHER_WORK_RESULT:
		OnBuyItem(uParam);
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiPlayerShop::OnBuyItem(KUiDraggedObject* pItem)
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
	Obj.eContainer = UOC_NPC_SHOP;

	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE,
		(unsigned int)(&Obj), (int)(&Price)))
	{
		KUiTradeConfirm::OpenWindow(this, &Obj, &Price, TCA_BUY);
	}
}

void KUiPlayerShop::OnBuyItem(unsigned int uParam)
{	
	g_pCoreShell->OperationRequest(GOI_BUY_PLAYER_SHOP,	uParam, 0);
}

void KUiPlayerShop::UpdateGoods()
{	
	m_pSelf->Clear();
	m_pSelf->m_nObjCount = g_pCoreShell->GetGameData(GDI_PLAYER_SHOP_ITEM, 0, 0);
	if (m_pSelf->m_nObjCount == 0)
		return;

	if (m_pSelf->m_pObjsList = (KUiObjAtContRegion*)malloc(sizeof(KUiObjAtContRegion) * m_pSelf->m_nObjCount))
	{
		g_pCoreShell->GetGameData(GDI_PLAYER_SHOP_ITEM, (unsigned int)m_pSelf->m_pObjsList, m_pSelf->m_nObjCount);
		for (int i = 0; i < m_pSelf->m_nObjCount; i++)
		{
			m_pSelf->m_ItemsBox.AddObject((KUiDraggedObject*)&m_pSelf->m_pObjsList[i], 1);
		}
	}
	else
		m_pSelf->m_nObjCount = 0;
}

void KUiPlayerShop::Clear()
{	
	m_ItemsBox.Clear();
	m_nObjCount  = 0;
	if (m_pObjsList)
	{
		free(m_pObjsList);
		m_pObjsList = NULL;
	}
}
