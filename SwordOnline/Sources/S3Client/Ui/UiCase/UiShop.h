
#pragma once
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndText.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define		MAX_BUY_NUMBER						60

struct KUiObjAtContRegion;

class KUiShop : protected KWndShowAnimate
{
public:
	static KUiShop*		OpenWindow();
	static KUiShop*		GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
	static void			CancelTrade();
	void				UpdateData();	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KUiShop();
	~KUiShop() {}
	void	SetPage(int nIndex);
	void	Clear();
	void	OnClickButton(KWndButton* pWnd, int bCheck);
	void	OnBuyItem(KUiDraggedObject* pItem, bool bDoImmed);
	void	OnBuyItem(unsigned int uParam, BYTE btParam);
	void	Initialize();
	void	Breathe();
private:
	static KUiShop*		m_pSelf;
private:
	KUiObjAtContRegion*	m_pObjsList;
	int					m_nObjCount;
	int					m_nPageCount;
	int					m_nCurrentPage;
	KWndObjectMatrix	m_ItemsBox;
	KWndLabeledButton			m_BuyBtn;
	KWndLabeledButton			m_SellBtn;
	KWndLabeledButton			m_RepairBtn;
	KWndButton			m_PreBtn;
	KWndButton			m_NextBtn;
	KWndButton			m_CloseBtn;
	KWndText32			m_Text;

private:
	
	KUiObjAtContRegion	m_ItemInfo;
	BYTE				m_btBuyNumber;
	BYTE				m_btDelayTime;
	
};