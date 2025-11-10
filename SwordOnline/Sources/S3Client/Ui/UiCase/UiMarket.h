#pragma once

#include "../Elem/WndPage.h"
#include "../Elem/WndLabeledButton.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define		defMAX_SELL_TYPE_BTN		7

struct KUiObjAtContRegion;

//===========================================================================

class KUiMarketGoods : public KWndPage
{
public:
	KUiMarketGoods();
	void	Initialize();
	void	Breathe();
	void	Clear();
	void	UpdateData(KUiDraggedObject* pObj);
	bool	HaveObject();
private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnBuyItem();
	void	OnBuyItem(char cBuyNumber);
private:
	KWndObjectMatrix		m_ItemsBox;
	KWndLabeledButton		m_BuyBtn;
	KWndText32				m_Name;
	KWndText32				m_OriginalPrice;
	KWndText32				m_Price;
	KWndText32				m_OriginalPrice_Number;
	KWndText32				m_Price_Number;
	KWndImage				m_DisCount;
	KWndImage				m_NewArrival;
private:
	KUiObjAtContRegion		m_ItemInfo;
	char					m_cBuyNumber;
	BYTE					m_btDelayTime;
};

//===========================================================================

class KUiMarket : public KWndPageSet
{
public:
	static KUiMarket*		OpenWindow();
	static KUiMarket*		GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
	static void			CancelTrade();
	void				UpdateData();
	static	void		OnClickPrePaid();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KUiMarket();
	~KUiMarket() {}
	void	Initialize();
	void	Breathe();
	void	InitMarket();
	
	void	OnClickSellType(int nIndex);
	void	Clear();
	void	SetPage(int nIndex);
private:
	static KUiMarket*		m_pSelf;

private:
	KWndLabeledButton	m_PrePaidBtn;
	KWndLabeledButton	m_ShoppingCartBtn;
	KWndLabeledButton	m_SellTypeBtn[defMAX_SELL_TYPE_BTN];
	KWndLabeledButton	m_PreBtn;
	KWndLabeledButton	m_NextBtn;
	KWndText32			m_PageInfo;
	KWndText32			m_Title;
	KWndButton			m_GoodsBtn[defMAX_GOODS_PER_PAGE];
	KUiMarketGoods		m_Goods[defMAX_GOODS_PER_PAGE];
	KWndButton			m_CloseBtn;
	//
	KWndText32			m_LockCoinTitle;
	KWndText32			m_LockCoinTxt;

private:
	KUiObjAtContRegion*	m_pObjsList;
	int					m_nObjCount;
	int					m_nPageCount;
	int					m_nCurrentPage;
	//
	int					m_nLockCoin;
};