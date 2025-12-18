
#pragma once

#include "../Elem/WndPage.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndEdit.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define		MAX_BUY_NUMBER						60

struct KUiObjAtContRegion;

//===========================================================================

class KUiShoppingCartGoods : public KWndPage
{
public:
	KUiShoppingCartGoods();
	void	Initialize();
	void	Clear();
	void	UpdateData(KWndWindow* pRequester, KUiDraggedObject* pObj, 
						KUiItemBuySelInfo* pPriceInfo);
	void	OnBuyItem();
	bool	HaveObject();
private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnAddCount();
	void	OnDelCount();
	void	OnCheckInput();
private:
	KWndObjectMatrix		m_ItemsBox;
	KWndLabeledButton		m_DelItemBtn;
	KWndText32				m_Name;
	KWndText32				m_TotalPrice;
	KWndButton				m_AddCountBtn;
	KWndButton				m_DelCountBtn;
	KWndEdit32				m_CountEdit;
private:
	int						m_nJbCoin;
	KWndWindow*				m_pRequester;
	KUiItemBuySelInfo		m_PriceInfo;
};

//===========================================================================

class KUiShoppingCart : public KWndPageSet
{
public:
	static KUiShoppingCart*		OpenWindow(KWndWindow* pRequester, KUiObjAtContRegion* pObj,
											KUiItemBuySelInfo* pPriceInfo, bool bShow);
	static KUiShoppingCart*		GetIfVisible();
	static void			CloseWindow(bool bDestroy = false);
	static void			LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static void			UpdateValue(int nCost, int nValue);
private:
	KUiShoppingCart();
	~KUiShoppingCart() {}
	void	Initialize();
	void	UpdateData(KWndWindow* pRequester, KUiObjAtContRegion* pObj,
						KUiItemBuySelInfo* pPriceInfo);	
private:
	static KUiShoppingCart*		m_pSelf;
private:
	KWndLabeledButton	m_PrePaidBtn;
	KWndLabeledButton	m_ConfirmBuyBtn;
	KWndScrollBar		m_ScrollBar;
	KWndText80			m_Title;
	KWndText32			m_TitleHeader;
	KWndText32			m_TotalCostTitle;
	KWndText32			m_TotalSaveTitle;
	KWndText32			m_OwnTitle;
	KWndText32			m_TotalCostValue;
	KWndText32			m_TotalSaveValue;
	KWndText32			m_OwnValue;
	KWndButton			m_CloseBtn;
	
	KWndButton			m_GoodsPageBtn;
	KUiShoppingCartGoods	m_GoodsPage;
private:
	int					m_nJbCoin;
	int					m_nTotalCost;
};