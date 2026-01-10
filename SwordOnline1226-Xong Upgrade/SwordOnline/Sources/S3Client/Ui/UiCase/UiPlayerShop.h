
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndText.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

struct KUiObjAtContRegion;

class KUiPlayerShop : protected KWndShowAnimate
{
public:
	static KUiPlayerShop*		OpenWindow(KUiPlayerItem* pDest);
	static KUiPlayerShop*		GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
	static void			CancelTrade();
	static void			UpdateGoods();	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KUiPlayerShop();
	~KUiPlayerShop() {}
	void	Clear();
	void	OnBuyItem(KUiDraggedObject* pItem);
	void	OnBuyItem(unsigned int uParam);
	void	Initialize();
	void	Breathe();
private:
	static KUiPlayerShop*		m_pSelf;
private:
	KUiObjAtContRegion*	m_pObjsList;
	int					m_nObjCount;
	KWndObjectMatrix	m_ItemsBox;
	KWndText32			m_OwnerName;
	KWndButton			m_CloseBtn;

private:
	KUiPlayerItem m_Dest;
};