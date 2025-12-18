
#pragma once

#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiInformation3 : protected KWndShowAnimate
{
public:
	void	Initialize();							
	void	LoadScheme(const char* pScheme);		
	void	Show(const char* pInformation,
				const char* pszFirstBtnText = "§ång ý",
				const char* pszSecondBtnText = 0,
				const char* pszThirdBtnText = "Xem nhÑ ",
				KWndWindow* pCallerWnd = 0,
				unsigned int uParam = 0,
                int nInformationLen = -1);			
	void	Close();							
	KUiInformation3();

private:
	void	Hide(int nBtnIndex);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KWndText256			m_Information;
	KWndLabeledButton	m_FirstBtn, m_SecondBtn, m_ThirdBtn;
	KWndWindow*			m_pCallerWnd;
	unsigned int		m_uCallerParam;
};

void UIMessageBox3(const char* pMsg, KWndWindow* pCaller = 0, 
				  const char* pszFirstBtnText = "§ång ý",
				  const char* pszSecondBtnText = 0,
				  const char* pszThirdBtnText = "Xem nhÑ ",
				  unsigned int uParam = 0);
void UiCloseMessageBox3();

extern KUiInformation3	g_UiInformation3;