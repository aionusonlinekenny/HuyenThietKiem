#ifndef __UiTradeConfirm_H__
#define __UiTradeConfirm_H__

#include "../elem/wndlabeledbutton.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../../../core/src/gamedatadef.h"

enum TRADE_CONFIRM_ACTION
{
	TCA_BUY,
	TCA_SALE,
	TCA_REPAIR,
	TCA_BREAK,
};

class KUiTradeConfirm : protected KWndImage
{
public:
	static KUiTradeConfirm*	OpenWindow(KWndWindow* pRequester, KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction, int btNumber = 1, 
			int btMaxNumber = 999);	
	static KUiTradeConfirm*	GetIfVisible();				
	static void			CloseWindow(bool bDestroy);		
private:
	KUiTradeConfirm();
	~KUiTradeConfirm() {}
	int		Initialize();								
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnBuy();
	void	OnSale();
	void	OnRepair();
	void	OnBreak();
	void	OnIncrease();
	void	OnDecrease();
	void	OnCheckInput();
private:
	static KUiTradeConfirm*	m_pSelf;
	KWndText32			m_ItemName;
	KWndText32			m_Money;
	KWndImage			m_BuyImg, m_SaleImg, m_RepairImg, m_BreakImg;
	KWndEdit32			m_BuyNumber;
	KWndButton			m_IncreaseBtn;
	KWndButton			m_DecreaseBtn;
	KWndText32			m_Title;
	KWndLabeledButton			m_OkBtn;
	KWndLabeledButton			m_CancelBtn;
	KUiObjAtContRegion	m_ItemInfo;
	KUiItemBuySelInfo	m_PriceInfo;
	unsigned int		m_uNormalPriceColor;
	unsigned int		m_uNotEnoughMoneyPriceColor;

private:
	KWndWindow*			m_pRequester;
	int					m_btMaxNumber; // fix gioi han max number tach item by kinnox;
};


#endif