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

class KUiNotification : protected KWndShowAnimate
{
public:
	static KUiNotification*	OpenWindow(const char* pszTitle, const char* pszInitString);
	static KUiNotification*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);	
private:
	KUiNotification();
	~KUiNotification() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	//void	OnOk();
private:
	static KUiNotification*	m_pSelf;
	KWndText32			m_Title;
	KWndMessageListBox	m_ContentList;

	KWndLabeledButton	m_CancelBtn;
	char 				szFunc[32];			
};

