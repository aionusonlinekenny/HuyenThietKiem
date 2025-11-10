/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	07.06.2021
*****************************************************************************************/
#pragma once

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndText.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndLabeledButton.h"

struct KUiObjAtRegion;

class KUiGiveBox : protected KWndImage
{
public:
	static KUiGiveBox*	OpenWindow(const char* pszTitle, const char* pszInitString,
					char* pszFunc);
	static KUiGiveBox*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);	
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);
	void	UpdateData();	
private:
	KUiGiveBox();
	~KUiGiveBox() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
private:
	static KUiGiveBox*	m_pSelf;
	KWndText32			m_Title;
	KWndMessageListBox	m_ContentList;
	KWndScrollBar		m_ContentScroll;	
	KWndObjectMatrix	m_ItemBox;
	KWndLabeledButton	m_OkBtn;
	KWndLabeledButton	m_CancelBtn;
	char 				szFunc[32];			
};

