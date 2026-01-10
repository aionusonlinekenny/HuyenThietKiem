
#pragma once

#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiInformation : protected KWndShowAnimate
{
public:
	void	Initialize();							
	void	LoadScheme(const char* pScheme);		
	void	Show(const char* pInformation,
				const char* pszFirstBtnText = "§ãng",
				const char* pszSecondBtnText = 0,
				KWndWindow* pCallerWnd = 0,
				unsigned int uParam = 0,
                int nInformationLen = -1);			
	void	Close();								
	KUiInformation();

private:
	void	Hide(int nBtnIndex);					
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KWndText256			m_Information;
	KWndLabeledButton	m_FirstBtn, m_SecondBtn;
	KWndWindow*			m_pCallerWnd;
	unsigned int		m_uCallerParam;
	int					m_nOrigFirstBtnXPos;
	int					m_nCentreBtnXPos;

    int                 m_nTipModeFlagFlag;
};

void UIMessageBox(const char* pMsg, KWndWindow* pCaller = 0, 
				  const char* pszFirstBtnText = "§ãng",
				  const char* pszSecondBtnText = 0,
				  unsigned int uParam = 0);
void UiCloseMessageBox();

extern KUiInformation	g_UiInformation;