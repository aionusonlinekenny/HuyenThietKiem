
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiMarket.h"
#include "UiShoppingCart.h"
#include "UiItem.h"
#include "UiTradeConfirmWnd.h"
#include "UiStatus.h"
#include "UiMiniMap.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiInformation.h"
#include <crtdbg.h>
#include <io.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI		"UiShoppingCart.ini"

KUiShoppingCart* KUiShoppingCart::m_pSelf = NULL;

KUiShoppingCart::KUiShoppingCart()
{	
	m_nJbCoin		= 0;
	m_nTotalCost	= 0;
}


KUiShoppingCart* KUiShoppingCart::GetIfVisible()
{
	return m_pSelf;
}

KUiShoppingCart* KUiShoppingCart::OpenWindow(KWndWindow* pRequester, KUiObjAtContRegion* pObj, 
											 KUiItemBuySelInfo* pPriceInfo, bool bShow)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiShoppingCart;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{	
		if(bShow == false)
		{	
			m_pSelf->UpdateData(pRequester, pObj, pPriceInfo);
		}
		m_pSelf->BringToTop();
		m_pSelf->Show();
		Wnd_SetExclusive(m_pSelf);
	}
	return m_pSelf;
}

void KUiShoppingCart::CloseWindow(bool bDestroy /*= false*/)
{
	if (m_pSelf)
	{	
		Wnd_ReleaseExclusive(m_pSelf);
		if(bDestroy)
		{	
			m_pSelf->m_GoodsPage.Clear();
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			m_pSelf->Hide();
		}
	}
}

void KUiShoppingCart::Initialize()
{
	AddChild(&m_PrePaidBtn);
	AddChild(&m_ConfirmBuyBtn);
	AddChild(&m_ScrollBar);
	AddChild(&m_Title);
	AddChild(&m_TotalCostTitle);
	AddChild(&m_TotalSaveTitle);
	AddChild(&m_OwnTitle);
	AddChild(&m_TotalCostValue);
	AddChild(&m_TotalSaveValue);
	AddChild(&m_OwnValue);
	AddChild(&m_CloseBtn);
	AddChild(&m_TitleHeader);

	AddChild(&m_GoodsPageBtn);
	m_GoodsPage.Initialize();
	AddPage(&m_GoodsPage, &m_GoodsPageBtn);
	
	Wnd_AddWindow(this);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

void KUiShoppingCart::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{		
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\"SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_PrePaidBtn.Init(&Ini, "PrePaid");
			m_pSelf->m_ConfirmBuyBtn.Init(&Ini, "ConfirmBuy");
			m_pSelf->m_ScrollBar.Init(&Ini, "Scroll");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_TitleHeader.Init(&Ini, "TitleHeader");
			m_pSelf->m_TotalCostTitle.Init(&Ini, "TotalCostTitleText");
			m_pSelf->m_TotalSaveTitle.Init(&Ini, "TotalSaveTitleText");
			m_pSelf->m_OwnTitle.Init(&Ini, "OwnTitleText");
			m_pSelf->m_TotalCostValue.Init(&Ini, "TotalCostValueText");
			m_pSelf->m_TotalSaveValue.Init(&Ini, "TotalSaveValueText");
			m_pSelf->m_OwnValue.Init(&Ini, "OwnValueText");
			m_pSelf->m_CloseBtn.Init(&Ini,"CloseBtn");

			m_pSelf->m_TotalSaveValue.SetJbCoinText(0);
			m_pSelf->m_OwnValue.SetJbCoinText(m_pSelf->m_nJbCoin);

			m_pSelf->m_GoodsPageBtn.Hide();
		}
	}
}

int	KUiShoppingCart::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_PrePaidBtn)
		{
			KUiMarket::OnClickPrePaid();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBuyBtn)
		{
			m_GoodsPage.OnBuyItem();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{
			CloseWindow();
		}
		break;
	default:
		return KWndPageSet::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiShoppingCart::UpdateValue(int nCost, int nValue)
{	
	m_pSelf->m_nTotalCost = nCost;
	m_pSelf->m_TotalCostValue.SetJbCoinText(m_pSelf->m_nTotalCost);
	m_pSelf->m_TotalSaveValue.SetJbCoinText(nValue);

	if(m_pSelf->m_nJbCoin < m_pSelf->m_nTotalCost)
		m_pSelf->m_ConfirmBuyBtn.Enable(false);
	else
		m_pSelf->m_ConfirmBuyBtn.Enable(true);
}

// --
//
// --
void KUiShoppingCart::UpdateData(KWndWindow* pRequester, KUiObjAtContRegion* pObj,
								KUiItemBuySelInfo* pPriceInfo)
{	
	if(!pRequester || !pObj || !pPriceInfo)
		return;

	KUiDraggedObject Obj;

	Obj.uGenre	= pObj->Obj.uGenre;
	Obj.uId		= pObj->Obj.uId;
	Obj.DataX	= pObj->Region.h;
	Obj.DataY	= pObj->Region.v;
	Obj.DataW	= pObj->Region.Width;
	Obj.DataH	= pObj->Region.Height;
	
	if(pPriceInfo->btPriceType == 1)
		m_pSelf->m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_JBCOIN, 0, 0);
	else if(pPriceInfo->btPriceType == 5)
		m_pSelf->m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_LOCKCOIN, 0, 0);

	m_pSelf->m_OwnValue.SetJbCoinText(m_pSelf->m_nJbCoin);

	m_nTotalCost += pPriceInfo->nPrice;
	
	m_GoodsPage.Clear();
	m_GoodsPage.UpdateData(pRequester, &Obj, pPriceInfo);

	if(m_nJbCoin < m_nTotalCost)
		m_ConfirmBuyBtn.Enable(false);
	else
		m_ConfirmBuyBtn.Enable(true);
}

KUiShoppingCartGoods::KUiShoppingCartGoods()
{	
	m_nJbCoin					= 0;
	m_PriceInfo.szItemName[0]	= 0;
	m_PriceInfo.nPrice			= 0;
	m_PriceInfo.btDisCountPrice = 0;
}

void KUiShoppingCartGoods::Initialize()
{	
	AddChild(&m_ItemsBox);
	AddChild(&m_DelItemBtn);
	AddChild(&m_Name);
	AddChild(&m_TotalPrice);
	AddChild(&m_AddCountBtn);
	AddChild(&m_DelCountBtn);
	AddChild(&m_CountEdit);
	
	m_ItemsBox.SetContainerId((int)UOC_NPC_SHOP);
	Wnd_AddWindow(this);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

// --
//
// --
void KUiShoppingCartGoods::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);

	if (Ini.Load(Buff))
	{	
		Init(&Ini, "GoodsInfo");
		m_ItemsBox.Init(&Ini, "GoodsInfo_ItemBox");
		m_DelItemBtn.Init(&Ini, "GoodsInfo_DelItem");
		m_Name.Init(&Ini, "GoodsInfo_GoodsName");
		m_TotalPrice.Init(&Ini, "GoodsInfo_TotalPrice");
		m_AddCountBtn.Init(&Ini, "GoodsInfo_AddCount");
		m_DelCountBtn.Init(&Ini, "GoodsInfo_DelCount");
		m_CountEdit.Init(&Ini, "GoodsInfo_CountEdit");
		
		m_ItemsBox.EnablePickPut(false);
		
		if(m_PriceInfo.btPriceType == 1)
			m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_JBCOIN, 0, 0);
		else if(m_PriceInfo.btPriceType == 5)
			m_nJbCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_LOCKCOIN, 0, 0);
	}
}

int KUiShoppingCartGoods::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{	
		case WND_N_BUTTON_CLICK:
			if (uParam == (unsigned int)(KWndWindow*)&m_DelItemBtn)
			{	
				m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, (unsigned int)0, 0);
				KUiShoppingCart::CloseWindow(true);
			}
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddCountBtn)
			{
				OnAddCount();
			}
			else if (uParam == (unsigned int)(KWndWindow*)&m_DelCountBtn)
			{
				OnDelCount();
			}
			break;
		case WND_N_EDIT_CHANGE:
			OnCheckInput();
		break;
		default:
			nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiShoppingCartGoods::OnAddCount()
{
	int nNumber = m_CountEdit.GetIntNumber();
	nNumber++;
	if (nNumber < 1)
	{
		nNumber = 1;
	}
	else if (nNumber > MAX_BUY_NUMBER)
	{
		nNumber = MAX_BUY_NUMBER;
	}
	m_CountEdit.SetIntText(nNumber);
	m_TotalPrice.SetJbCoinText(m_PriceInfo.nPrice * nNumber);
	KUiShoppingCart::UpdateValue(m_PriceInfo.nPrice * nNumber, m_PriceInfo.btDisCountPrice * nNumber);
}

void KUiShoppingCartGoods::OnDelCount()
{
	int nNumber = m_CountEdit.GetIntNumber();
	nNumber--;
	if (nNumber < 1)
	{
		nNumber = 1;
	}
	else if (nNumber > MAX_BUY_NUMBER)
	{
		nNumber = MAX_BUY_NUMBER;
	}
	m_CountEdit.SetIntText(nNumber);
	m_TotalPrice.SetJbCoinText(m_PriceInfo.nPrice * nNumber);
	KUiShoppingCart::UpdateValue(m_PriceInfo.nPrice * nNumber, m_PriceInfo.btDisCountPrice * nNumber);
}


void KUiShoppingCartGoods::OnCheckInput()
{
	int nNumber = m_CountEdit.GetIntNumber();
	if (nNumber < 0)
	{
		nNumber = 0;
	}
	else if (nNumber > MAX_BUY_NUMBER)
	{
		nNumber = MAX_BUY_NUMBER;
	}
	char	szBuff1[16], szBuff2[16];
	_itoa(nNumber, szBuff1, 10);
	m_CountEdit.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_CountEdit.SetIntText(nNumber);
	
	m_TotalPrice.SetJbCoinText(m_PriceInfo.nPrice * nNumber);
	KUiShoppingCart::UpdateValue(m_PriceInfo.nPrice * nNumber, m_PriceInfo.btDisCountPrice * nNumber);
}


void KUiShoppingCartGoods::OnBuyItem()
{	
	OnCheckInput();
	int nNumber = m_CountEdit.GetIntNumber();
	m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, (unsigned int)nNumber, 0);
	KUiShoppingCart::CloseWindow(true);
}

// --
//
// --
void KUiShoppingCartGoods::Clear()
{	
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	Hide();
	m_ItemsBox.Clear();
	m_ItemsBox.Hide();
}

// --
//
// --
void KUiShoppingCartGoods::UpdateData(KWndWindow* pRequester, KUiDraggedObject* pObj,
									  KUiItemBuySelInfo* pPriceInfo)
{	
	if(!pRequester)
		return;

	if(!pObj)
		return;

	m_pRequester	= pRequester;
	m_PriceInfo		= *pPriceInfo;
	m_ItemsBox.AddObject(pObj, 1);
	m_ItemsBox.Show();
	
	m_Name.SetText(m_PriceInfo.szItemName);
	m_TotalPrice.SetJbCoinText(m_PriceInfo.nPrice);
	m_CountEdit.SetIntText(1);
	Show();
}

// --
//
// --
bool KUiShoppingCartGoods::HaveObject()
{	
	if(m_ItemsBox.IsVisible())
		return true;

	return false;
}