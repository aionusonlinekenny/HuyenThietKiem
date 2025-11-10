
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../elem/wndlabeledbutton.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

struct KUiObjAtRegion;

class KUiTrade : protected KWndShowAnimate
{
public:
	static KUiTrade*	OpenWindow(KUiPlayerItem* pOther);
	static KUiTrade*	GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);

	void				OnChangedTakewithItem(KUiObjAtRegion* pObj, int bAdd);
	void				OnOppositeChangedItem(KUiObjAtRegion* pObj, int bAdd);
	void				OnSelfChangedItem(KUiObjAtRegion* pObj, int bAdd);
	void				UpdateOperData();

	void				UpdateTradeEnd(int bTraded);

private:
	KUiTrade();
	~KUiTrade();
	void	Initialize();						
	void	LoadScheme(KIniFile* pIni);			
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
					
	void	Breathe();
	void	OnTrade();
	void	OnOk(int bChecked);
	void	OnCancelTrade();
	void	OnAdjustMoney(bool bAdd);
	void	OnAdjustedMoney();
	void	OnModififyMoney();
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	UpdateTakewithItems();

private:
	static KUiTrade*	m_pSelf;
private:
	KWndText32			m_TakewithMoney;
	KWndObjectMatrix	m_TakewithItemsBox;

	KWndObjectMatrix	m_SelfItemsBox;		
	KWndEdit32			m_SelfMoney;		
	KWndLabeledButton			m_Ok;				
	KWndLabeledButton			m_Cancel;			
	KWndLabeledButton			m_Trade;			
	KWndButton			m_AddMoney;			
	KWndButton			m_ReduceMoney;		
	int					m_nSelfTradeMoney;

	int					m_bAdjustingMoney;
	int					m_nTakewithMoney;

	KWndText32			m_OtherName;		
	KWndObjectMatrix	m_OtherItemsBox;	
	KWndText32			m_OtherMoney;		
	KWndText32			m_Info;				
	KUiPlayerItem		m_OtherData;

	unsigned int		m_uWaitTradeMsgColor;
	unsigned int		m_uOtherLockMsgColor;
	unsigned int		m_uOtherNotLockMsgColor;
};
