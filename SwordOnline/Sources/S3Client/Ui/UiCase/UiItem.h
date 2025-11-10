
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/wndlabeledbutton.h"

struct KUiObjAtRegion;

class KUiItem : public KWndShowAnimate
{
public:
	static KUiItem* OpenWindow();
	static KUiItem* GetIfVisible();
	static void		CloseWindow(bool bDestroy);
	static void		LoadScheme(const char* pScheme);
	static void		OnNpcTradeMode(bool bTrue);
	void			UpdateItem(KUiObjAtRegion* pItem, int bAdd);
	void			Breathe();									
	static void		MoveItemToRepository(KUiDraggedObject* pItem);
	static void 	MoveItemToInventory(KUiDraggedObject* pItem);
private:
	KUiItem() {}
	~KUiItem() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateData();
	void	OnClickItem(KUiDraggedObject* pItem, bool bDoImmed);
	void	OnRepairItem(KUiDraggedObject* pItem);
	void	OnBindItem(KUiDraggedObject* pItem);
	void	OnUnBindItem(KUiDraggedObject* pItem);
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	OnGetMoney(int nMoney);
	void	OnClickMarkPriceBtn();
	void	OnMarkPriceItem(KUiDraggedObject* pItem);
	void	OnClickMakeAdvBtn();
	void	OnClickMakeStallBtn();
private:
	static KUiItem*		m_pSelf;
private:
	int					m_nMoney;
	int					m_nJbCoin;
	KWndImage			m_TitleIcon;
	KWndText80			m_Money;
	KWndImage			m_MoneyIcon;		
	KWndText80			m_JbCoin;
	KWndImage			m_JbCoinIcon;
	KWndLabeledButton			m_MakeAdvBtn;
	KWndLabeledButton			m_MarkPriceBtn;
	KWndLabeledButton			m_MakeStallBtn;
	KWndLabeledButton			m_GetMoneyBtn;
	KWndButton			m_CloseBtn;
	KWndObjectMatrix	m_ItemBox;
	KWndLabeledButton			m_OpenStatusPadBtn;
};