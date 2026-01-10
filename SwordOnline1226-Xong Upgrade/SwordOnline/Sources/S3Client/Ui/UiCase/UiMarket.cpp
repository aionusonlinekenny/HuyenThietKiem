
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

#define SCHEME_INI		"UiMarket.ini"

KUiMarket* KUiMarket::m_pSelf = NULL;


KUiMarket::KUiMarket()
{
	m_pObjsList				= NULL;
	m_nObjCount				= 0;
	m_nPageCount			= 0;
}


KUiMarket* KUiMarket::GetIfVisible()
{
	return m_pSelf;
}

KUiMarket* KUiMarket::OpenWindow()
{	
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMarket;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{	
		g_UiBase.SetStatus(UIS_S_TRADE_NPC);
		m_pSelf->InitMarket();
		m_pSelf->UpdateData();
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

void KUiMarket::CloseWindow()
{
	if (m_pSelf)
	{	
		KUiItem::OnNpcTradeMode(false);
		g_UiBase.SetStatus(UIS_S_IDLE);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiMarket::Initialize()
{
	AddChild(&m_PrePaidBtn);
	AddChild(&m_ShoppingCartBtn);
	char i = 0;
	for(i = 0; i < defMAX_SELL_TYPE_BTN; i++)
	{
		AddChild(&m_SellTypeBtn[i]);
	}
	AddChild(&m_PreBtn);
	AddChild(&m_NextBtn);
	AddChild(&m_PageInfo);
	AddChild(&m_Title);
	for(i = 0; i < defMAX_GOODS_PER_PAGE; i++)
	{
		AddChild(&m_GoodsBtn[i]);
		m_Goods[i].Initialize();
		AddPage(&m_Goods[i], &m_GoodsBtn[i]);
	}
	AddChild(&m_CloseBtn);
	//
	AddChild(&m_LockCoinTitle);
	AddChild(&m_LockCoinTxt);
	
	Wnd_AddWindow(this);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

void KUiMarket::LoadScheme(const char* pScheme)
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
			m_pSelf->m_ShoppingCartBtn.Init(&Ini,"ShoppingCart");
			char i = 0;
			for(i = 0; i < defMAX_SELL_TYPE_BTN; i++)
			{
				m_pSelf->m_SellTypeBtn[i].Init(&Ini,"SellType");
				m_pSelf->m_SellTypeBtn[i].Hide();
			}
			m_pSelf->m_PreBtn.Init(&Ini,"LeftBtn");
			m_pSelf->m_NextBtn.Init(&Ini,"RightBtn");
			m_pSelf->m_PageInfo.Init(&Ini,"PageInfo");
			m_pSelf->m_Title.Init(&Ini,"Title");
			m_pSelf->m_CloseBtn.Init(&Ini,"CloseBtn");
			//
			m_pSelf->m_LockCoinTitle.Init(&Ini, "LockCoinTitle");
			m_pSelf->m_LockCoinTxt.Init(&Ini, "LockCoinTxt");

			m_pSelf->m_PreBtn.Enable(false);
			m_pSelf->m_NextBtn.Enable(false);

			int nLeft, nTop, nWidth, nHeigh, nX, nY, nDiff1, nDiff2;
			Ini.GetInteger2("MarketGoods", "Start", &nLeft, &nTop);
			Ini.GetInteger2("MarketGoods", "Diff", &nDiff1, &nDiff2);
			m_pSelf->m_Goods[0].GetSize(&nWidth, &nHeigh);
			nX = nLeft;
			nY = nTop;
			for(i = 0; i < defMAX_GOODS_PER_PAGE; i++)
			{	
				if(i % 3 == 0 && i != 0)
				{	
					nX = nLeft;
					nY += nHeigh + nDiff2;
				}
				else
				{	
					if(i != 0)
						nX += nWidth + nDiff1;
				}
				m_pSelf->m_Goods[i].SetPosition(nX, nY);
				m_pSelf->m_Goods[i].Show();
				m_pSelf->m_GoodsBtn[i].Hide();
			}
		}
	}
}

void KUiMarket::CancelTrade()
{
	CloseWindow();
	KUiShoppingCart::CloseWindow(true);
}

int	KUiMarket::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_PrePaidBtn)
		{
			OnClickPrePaid();
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_ShoppingCartBtn)
		{
			KUiShoppingCart::OpenWindow(NULL, NULL, NULL, true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{
			CancelTrade();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_PreBtn)
		{
			SetPage(m_nCurrentPage - 1);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_NextBtn)
		{
			SetPage(m_nCurrentPage + 1);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[0])
		{
			OnClickSellType(0);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[1])
		{
			OnClickSellType(1);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[2])
		{
			OnClickSellType(2);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[3])
		{
			OnClickSellType(3);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[4])
		{
			OnClickSellType(4);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[5])
		{
			OnClickSellType(5);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[6])
		{
			OnClickSellType(6);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_SellTypeBtn[7])
		{
			OnClickSellType(7);
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_ESCAPE)
		{
			if (g_UiBase.GetStatus() == UIS_S_TRADE_NPC)
			{
				CloseWindow();
			}
			else
			{
				g_UiBase.SetStatus(UIS_S_TRADE_NPC);
			}
		}
		break;
	default:
		return KWndPageSet::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

// --
//
// --
void KUiMarket::Breathe()
{	
	char i = 0;
	for(i = 0; i < defMAX_GOODS_PER_PAGE; i++)
	{	
		m_pSelf->m_Goods[i].Breathe();
	}
	
	KUiShoppingCart* pShopping = KUiShoppingCart::GetIfVisible();
	m_ShoppingCartBtn.Enable(pShopping != NULL);

	m_nLockCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_LOCKCOIN, 0, 0);
	m_LockCoinTxt.SetJbCoinText(m_nLockCoin);
}

// --
//
// --
void KUiMarket::InitMarket()
{	
	if(!g_pCoreShell)
		return;

	int nCount = g_pCoreShell->GetGameData(GDI_MARKET_COUNT, 0, 0);
	if(nCount <= 0)
		return;

	char szMarketName[32];	
	int nWidth, nHeight, nLeft, nTop, nX, nY;
	m_SellTypeBtn[0].GetPosition(&nLeft, &nTop);
	m_SellTypeBtn[0].GetSize(&nWidth, &nHeight);
	nX = nLeft;
	nY = nTop;
	for(int i = 0; i < nCount; i++)
	{	
		g_pCoreShell->GetGameData(GDI_MARKET_SELLNAME, (unsigned int)&szMarketName, i);
		
		if(i != 0)
		{
			nX += nWidth;
		}
		m_SellTypeBtn[i].SetLabel((char*)szMarketName);
		m_SellTypeBtn[i].SetPosition(nX, nY);
		m_SellTypeBtn[i].Show();	
	}
	m_SellTypeBtn[0].CheckButton(true);
}

void KUiMarket::Clear()
{	
	for(int i = 0; i < defMAX_GOODS_PER_PAGE; i++)
	{
		m_Goods[i].Clear();
	}
	m_nObjCount  = 0;
	m_nPageCount = 0;
	if (m_pObjsList)
	{
		free(m_pObjsList);
		m_pObjsList = NULL;
	}
}

// --
//
// --
void KUiMarket::UpdateData()
{	
	m_nLockCoin = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_LOCKCOIN, 0, 0);
	m_LockCoinTxt.SetJbCoinText(m_nLockCoin);

	Clear();
	m_nObjCount = g_pCoreShell->GetGameData(GDI_TRADE_NPC_ITEM, 0, 0);
	if (m_nObjCount == 0)
		return;
	if (m_pObjsList = (KUiObjAtContRegion*)malloc(sizeof(KUiObjAtContRegion) * m_nObjCount))
	{
		g_pCoreShell->GetGameData(GDI_TRADE_MARKET_ITEM, (unsigned int)m_pObjsList, m_nObjCount);
		m_nPageCount = m_pObjsList[m_nObjCount - 1].nContainer + 1;
		SetPage(0);
		m_PreBtn.Enable(m_nPageCount > 1);
		m_NextBtn.Enable(m_nPageCount > 1);
	}
	else
	{
		m_nObjCount = 0;
	}
}

void KUiMarket::SetPage(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nPageCount && m_pObjsList)
	{	
		int i, j;
		for(j = 0; j < defMAX_GOODS_PER_PAGE; j++)
		{
			m_Goods[j].Clear();
		}
		for (i = 0; i < m_nObjCount; i++)
		{	
			if (m_pObjsList[i].nContainer == nIndex)
			{	
				for(j = 0; j < defMAX_GOODS_PER_PAGE; j++)
				{
					if(m_Goods[j].HaveObject())
						continue;

					m_Goods[j].UpdateData((KUiDraggedObject*)&m_pObjsList[i]);
					break;
				}
			}
		}
		m_nCurrentPage = nIndex;
		m_PageInfo.Set2IntText(m_nCurrentPage + 1, m_nPageCount, '/');
	}
}

void KUiMarket::OnClickPrePaid()
{	
	TCHAR szDefaultBrowserFolder[64];
    TCHAR szDefaultBrowserName[16] = TEXT("chrome.exe");
    TCHAR szDefaultWebsite[] = TEXT("http://chuaphattrientinhnangnay.com"); 
    TCHAR szOperation[] = TEXT("open");
    
	sprintf(szDefaultBrowserFolder, TEXT("C:\\Program Files (x86)\\Google\\Chrome\\Application\\"));
	char cRec = _access(szDefaultBrowserFolder, 00);
	if(cRec == -1)
	{
		sprintf(szDefaultBrowserFolder, TEXT("C:\\Program Files\\Google\\Chrome\\Application\\"));
		cRec = _access(szDefaultBrowserFolder, 00);
		if(cRec == -1)
		{
			sprintf(szDefaultBrowserFolder, TEXT("C:\\Program Files (x86)\\Mozilla Firefox"));
			sprintf(szDefaultBrowserName, TEXT("firefox.exe"));
			cRec = _access(szDefaultBrowserFolder, 00);
			if(cRec == -1)
			{
				sprintf(szDefaultBrowserFolder, TEXT("C:\\Program Files\\Mozilla Firefox"));
				cRec = _access(szDefaultBrowserFolder, 00);
				if(cRec == -1)
				{
					sprintf(szDefaultBrowserFolder, TEXT("C:\\Program Files\\Internet Explorer"));
					sprintf(szDefaultBrowserName, TEXT("iexplore.exe"));
					cRec = _access(szDefaultBrowserFolder, 00);
					if(cRec == -1)
					{
						MessageBox(NULL, "Trinh Duyet Web Khong Duoc Tim Thay", "NN Team", MB_OK | MB_ICONERROR);
						return;
					}
				}
			}
		}
		
	}
    HINSTANCE hBrowserInstance = ShellExecute (NULL, 
                                    szOperation, 
                                    szDefaultBrowserName, 
                                    szDefaultWebsite,
                                    szDefaultBrowserFolder,
                                    SW_SHOWDEFAULT);

}

void KUiMarket::OnClickSellType(int nIndex)
{	
	if(nIndex < 0)
		return;

	if(m_SellTypeBtn[nIndex].IsButtonChecked() == FALSE)
	{	
		m_SellTypeBtn[nIndex].CheckButton(true);
		return;
	}

	int nCount = g_pCoreShell->GetGameData(GDI_MARKET_COUNT, 0, 0);
	if(nCount <= 0)
		return;

	if(nIndex >= nCount)
		return;

	for(int i = 0; i < nCount; i++)
	{	
		if(i == nIndex)
			continue;

		m_SellTypeBtn[i].CheckButton(false);
	}

	char	szFunc[32];
	sprintf(szFunc, "OpenMarket|%d", g_pCoreShell->GetGameData(GDI_MARKET_SELLID, nIndex, 0));
	g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 1);

	KUiShoppingCart::CloseWindow(true);
}

KUiMarketGoods::KUiMarketGoods()
{	
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
	m_cBuyNumber		= -1;
	m_btDelayTime		= 0;
}

void KUiMarketGoods::Initialize()
{	
	AddChild(&m_DisCount);
	AddChild(&m_ItemsBox);
	AddChild(&m_BuyBtn);
	AddChild(&m_Name);
	AddChild(&m_OriginalPrice);
	AddChild(&m_Price);
	AddChild(&m_OriginalPrice_Number);
	AddChild(&m_Price_Number);
	
	AddChild(&m_NewArrival);
	
	m_ItemsBox.SetContainerId((int)UOC_NPC_SHOP);
	Wnd_AddWindow(this);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

// --
//
// --
void KUiMarketGoods::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);

	if (Ini.Load(Buff))
	{	
		Init(&Ini, "MarketGoods");
		m_ItemsBox.Init(&Ini, "MarketGoods_ItemBox");
		m_BuyBtn.Init(&Ini, "MarketGoods_Buy");
		m_Name.Init(&Ini, "MarketGoods_GoodsName");
		m_OriginalPrice.Init(&Ini, "MarketGoods_OriginalPrice");
		m_Price.Init(&Ini, "MarketGoods_Price");
		m_OriginalPrice_Number.Init(&Ini, "MarketGoods_OriginalPrice_Number");
		m_Price_Number.Init(&Ini, "MarketGoods_Price_Number");
		m_DisCount.Init(&Ini, "MarketGoods_DisCount");
		m_NewArrival.Init(&Ini, "MarketGoods_imgNewArrival");
		
		m_ItemsBox.EnablePickPut(false);
		Clear();
	}
}

int KUiMarketGoods::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{	
		case WND_N_BUTTON_CLICK:
			if (uParam == (unsigned int)(KWndWindow*)&m_BuyBtn)
			{
				OnBuyItem();
			}
			break;
		case WND_M_OTHER_WORK_RESULT:
			OnBuyItem((char)uParam);
			break;
		default:
			nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiMarketGoods::OnBuyItem()
{	
	if(!g_pCoreShell)
		return;

	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE,
		(unsigned int)(&m_ItemInfo), (int)(&Price)))
	{
		
		KUiShoppingCart::OpenWindow(this, &m_ItemInfo, &Price, false);
	}
}

void KUiMarketGoods::OnBuyItem(char cBuyNumber)
{
	if (g_pCoreShell == NULL)
		return;
	
	if(m_ItemInfo.Obj.uGenre == CGOG_NOTHING)
		return;

	m_cBuyNumber = cBuyNumber;
}

// --
//
// --
void KUiMarketGoods::Breathe()
{
	if(m_NewArrival.IsVisible())
	{
		m_NewArrival.NextFrame();
	}

	if(m_cBuyNumber > 0 && m_cBuyNumber <= MAX_BUY_NUMBER)
	{	
		if(!m_BuyBtn.IsDisable())
			m_BuyBtn.Enable(false);
		if(m_btDelayTime % 3 == 0)
		{	
			if(g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
				(unsigned int)(&m_ItemInfo), 0))
			{
				m_cBuyNumber--;
			}
			else
			{
				m_cBuyNumber = 0;
			}
			m_btDelayTime = 0;
		}
		m_btDelayTime++;
	}
	else if(m_cBuyNumber == 0)
	{
		if(m_BuyBtn.IsDisable())
			m_BuyBtn.Enable(true);
	}
}

// --
//
// --
void KUiMarketGoods::Clear()
{	
	m_cBuyNumber = -1;
	m_ItemsBox.Clear();
	m_ItemsBox.Hide();
	m_BuyBtn.Enable(false);
	m_Name.Hide();
	m_OriginalPrice.SetText("Gi¸ gèc:");
	m_OriginalPrice.Hide();
	m_Price.SetText("Gi¸ thùc:");
	m_Price.Hide();
	m_OriginalPrice_Number.Hide();
	m_Price_Number.Hide();
	m_DisCount.Hide();
	m_NewArrival.Hide();
}

// --
//
// --
void KUiMarketGoods::UpdateData(KUiDraggedObject* pObj)
{	
	if(!pObj)
		return;

	if(!g_pCoreShell)
		return;

	m_ItemsBox.AddObject(pObj, 1);
	m_ItemsBox.Show();
	m_BuyBtn.Enable(true);
	
	m_ItemInfo.Obj.uGenre = pObj->uGenre;
	m_ItemInfo.Obj.uId = pObj->uId;
	m_ItemInfo.Region.h = pObj->DataX;
	m_ItemInfo.Region.v = pObj->DataY;
	m_ItemInfo.Region.Width = pObj->DataW;
	m_ItemInfo.Region.Height = pObj->DataH;
	m_ItemInfo.eContainer = UOC_NPC_SHOP;
	
	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE,
		(unsigned int)(&m_ItemInfo), (int)(&Price)))
	{
		m_Name.SetText(Price.szItemName);
		m_Name.Show();
		m_OriginalPrice.Show();
		m_Price.Show();
		m_OriginalPrice_Number.SetJbCoinText(Price.nPrice + Price.btDisCountPrice);
		m_Price_Number.SetJbCoinText(Price.nPrice);
		m_OriginalPrice_Number.Show();
		m_Price_Number.Show();
		if(Price.btDisCountPrice > 0)
			m_DisCount.Show();
		if(Price.bHotSell)
			m_NewArrival.Show();

	}
}

// --
//
// --
bool KUiMarketGoods::HaveObject()
{
	if(m_ItemsBox.IsVisible())
		return true;

	return false;
}