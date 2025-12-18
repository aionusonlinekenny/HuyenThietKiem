#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTradeConfirmWnd.h"
#include "UiPlayerShop.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTradeConfirmWnd.ini"

KUiTradeConfirm* KUiTradeConfirm::m_pSelf = NULL;


KUiTradeConfirm* KUiTradeConfirm::OpenWindow(KWndWindow* pRequester, KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction, int btNumber /*= 1*/,
			int btMaxNumber /*= 999*/)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTradeConfirm;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_BuyImg.Hide();
		m_pSelf->m_SaleImg.Hide();
		m_pSelf->m_RepairImg.Hide();
		m_pSelf->m_BreakImg.Hide();
		if (eAction == TCA_SALE)
		{
			m_pSelf->m_SaleImg.Show();
			m_pSelf->m_BuyNumber.Enable(false);
			m_pSelf->m_IncreaseBtn.Enable(false);
			m_pSelf->m_DecreaseBtn.Enable(false);
			m_pSelf->m_OkBtn.Enable(true);
			m_pSelf->m_Money.SetTextColor(m_pSelf->m_uNormalPriceColor);
		}
		else if(eAction == TCA_BREAK)
		{
			m_pSelf->m_BreakImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
			m_pSelf->m_Money.SetTextColor(m_pSelf->m_uNormalPriceColor);
			m_pSelf->m_BuyNumber.Enable(true);
			m_pSelf->m_IncreaseBtn.Enable(true);
			m_pSelf->m_DecreaseBtn.Enable(true);	
		}
		else
		{
			if (eAction == TCA_BUY)
			{
				m_pSelf->m_BuyImg.Show();
				if(KUiPlayerShop::GetIfVisible() != NULL)
				{
					m_pSelf->m_BuyNumber.Enable(false);
					m_pSelf->m_IncreaseBtn.Enable(false);
					m_pSelf->m_DecreaseBtn.Enable(false);
				}
			}
			else
			{	
				m_pSelf->m_RepairImg.Show();
				m_pSelf->m_BuyNumber.Enable(false);
				m_pSelf->m_IncreaseBtn.Enable(false);
				m_pSelf->m_DecreaseBtn.Enable(false);
			}
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
		}
		m_pSelf->m_btMaxNumber	= btMaxNumber;
		m_pSelf->m_pRequester	= pRequester;
		m_pSelf->m_ItemInfo		= *pObj;
		m_pSelf->m_PriceInfo	= *pPriceInfo;
		m_pSelf->m_BuyNumber.SetIntText(btNumber);
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiTradeConfirm::KUiTradeConfirm()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

KUiTradeConfirm* KUiTradeConfirm::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiTradeConfirm::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

void KUiTradeConfirm::Show()
{
	m_ItemName.SetText(m_PriceInfo.szItemName);
	//
	if (m_BuyImg.IsVisible() || m_SaleImg.IsVisible() || m_RepairImg.IsVisible())
	{	
		m_Money.SetMoneyText(m_PriceInfo.nPrice);
	}
	else if(m_BreakImg.IsVisible())
	{
		m_Money.SetStackText(m_PriceInfo.nPrice);
	}
	
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

void KUiTradeConfirm::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

int KUiTradeConfirm::Initialize()
{
	AddChild(&m_ItemName);
	AddChild(&m_Money);
	AddChild(&m_BuyImg);
	AddChild(&m_SaleImg);
	AddChild(&m_RepairImg);
	
	AddChild(&m_BreakImg);
	AddChild(&m_BuyNumber);
	AddChild(&m_IncreaseBtn);
	AddChild(&m_DecreaseBtn);
	
	AddChild(&m_Title);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

void KUiTradeConfirm::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemName.Init(&Ini, "ItemName");

		m_Money.Init(&Ini, "Price");			
		Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
		m_uNormalPriceColor = GetColor(Buff);
		Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
		m_uNotEnoughMoneyPriceColor = GetColor(Buff);

		m_BuyImg.Init(&Ini, "BuyImg");
		m_SaleImg.Init(&Ini, "SaleImg");
		m_RepairImg.Init(&Ini, "RepairImg");
		
		m_BuyNumber.Init(&Ini, "BuyNumber");
		m_IncreaseBtn.Init(&Ini, "Increase");
		m_DecreaseBtn.Init(&Ini, "Decrease");
		
		m_Title.Init(&Ini,	"Title");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_Money.SetMoneyText(m_PriceInfo.nPrice);
	}
}
int KUiTradeConfirm::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			if (m_BuyImg.IsVisible())
				OnBuy();
			else if (m_SaleImg.IsVisible())
				OnSale();
			
			else if(m_BreakImg.IsVisible())
				OnBreak();
			
			else
				OnRepair();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		{
			OnCancel();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_IncreaseBtn)
		{
			OnIncrease();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_DecreaseBtn)
		{
			OnDecrease();
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			if (m_SaleImg.IsVisible())
			{
				OnSale();
			}
			else if (m_BuyImg.IsVisible())
			{
				if (m_BuyImg.IsDisable())
					OnCancel();
				else
					OnBuy();
			}
			
			else if(m_BreakImg.IsVisible())
			{
				OnBreak();
			}
			
			else if (m_RepairImg.IsDisable())
			{
				OnCancel();	
			}
			else
			{
				OnRepair();
			}
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiTradeConfirm::OnBuy()
{	
	//
	OnCheckInput();
	int nNumber = m_BuyNumber.GetIntNumber();
	m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, (unsigned int)&m_ItemInfo, nNumber);
	
	CloseWindow(false);
}

void KUiTradeConfirm::OnSale()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

void KUiTradeConfirm::OnRepair()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

void KUiTradeConfirm::OnBreak()
{	
	OnCheckInput();
	int nNumber = m_BuyNumber.GetIntNumber();
	if(nNumber <= 0)
	{
		CloseWindow(false);
		return;
	}
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_ITEM_BREAK,
			(unsigned int)(&m_ItemInfo), nNumber);
	}
	CloseWindow(false);
}

void KUiTradeConfirm::OnIncrease()
{
	int nNumber = m_BuyNumber.GetIntNumber();
	nNumber++;
	if (nNumber < 1)
	{
		nNumber = 1;
	}
	else if (nNumber > m_btMaxNumber)
	{
		nNumber = m_btMaxNumber;
	}
	m_BuyNumber.SetIntText(nNumber);
	//
	if (m_BuyImg.IsVisible() || m_SaleImg.IsVisible() || m_RepairImg.IsVisible())
	{	
		m_Money.SetMoneyText(m_PriceInfo.nPrice * nNumber);
	}
	else if(m_BreakImg.IsVisible())
	{
		m_Money.SetStackText(m_PriceInfo.nPrice);
	}
	
}

void KUiTradeConfirm::OnDecrease()
{
	int nNumber = m_BuyNumber.GetIntNumber();
	nNumber--;
	if (nNumber < 1)
	{
		nNumber = 1;
	}
	else if (nNumber > m_btMaxNumber)
	{
		nNumber = m_btMaxNumber;
	}
	m_BuyNumber.SetIntText(nNumber);
	//
	if (m_BuyImg.IsVisible() || m_SaleImg.IsVisible() || m_RepairImg.IsVisible())
	{	
		m_Money.SetMoneyText(m_PriceInfo.nPrice * nNumber);
	}
	else if(m_BreakImg.IsVisible())
	{
		m_Money.SetStackText(m_PriceInfo.nPrice);
	}
	
}

void KUiTradeConfirm::OnCheckInput()
{
	int nNumber = m_BuyNumber.GetIntNumber();
	if (nNumber < 0)
	{
		nNumber = 0;
	}
	else if (nNumber > m_btMaxNumber)
	{
		nNumber = m_btMaxNumber;
	}
	char	szBuff1[16], szBuff2[16];
	_itoa(nNumber, szBuff1, 10);
	m_BuyNumber.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_BuyNumber.SetIntText(nNumber);
	//
	if (m_BuyImg.IsVisible() || m_SaleImg.IsVisible() || m_RepairImg.IsVisible())
	{	
		m_Money.SetMoneyText(m_PriceInfo.nPrice * nNumber);
	}
	else if(m_BreakImg.IsVisible())
	{
		m_Money.SetStackText(m_PriceInfo.nPrice);
	}
	
	
}

void KUiTradeConfirm::OnCancel()
{
	CloseWindow(false);
}
