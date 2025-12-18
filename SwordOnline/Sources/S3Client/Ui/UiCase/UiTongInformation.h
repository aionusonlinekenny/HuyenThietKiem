
#pragma once

#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiTongInformation : protected KWndShowAnimate
{
public:
	static	KUiTongInformation* OpenWindow(const char* pInformation, int nData, 
											KWndWindow* pCallerWnd = NULL, unsigned int uParam = 0);
	static	KUiTongInformation* GetIfVisible();
	static void   CloseWindow(bool bDestory = false); 				

private:
	KUiTongInformation();
	void	Initialize();
	void	LoadScheme(const char* pScheme);
	void	Show(const char* pInformation, int nData, 
					KWndWindow* pCallerWnd = NULL, unsigned int uParam = 0);
	void	OnClickBtn(int nBtnIndex);					
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	static        KUiTongInformation *ms_pSelf;

private:
	KWndText256			m_Information;
	KWndLabeledButton	m_FirstBtn, m_SecondBtn;

private:
	KWndWindow*			m_pCallerWnd;
	unsigned int		m_uCallerParam;
	int 				m_nData;
};
