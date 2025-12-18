
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiInformation3.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI	"UiInformation3.ini"

KUiInformation3	g_UiInformation3;


void UIMessageBox3(const char* pMsg,
				  KWndWindow* pCaller /*= 0*/,
				  const char* pszFirstBtnText /*= "§ång ý"*/,
				  const char* pszSecondBtnText /*= 0*/,	
				  const char* pszThirdBtnText /*= "Xem nhÑ "*/,
				  unsigned int uParam /*= 0*/)
{
	g_UiInformation3.Show(pMsg, pszFirstBtnText, pszSecondBtnText, pszThirdBtnText, pCaller, uParam);
}


void UiCloseMessageBox3()
{
	g_UiInformation3.Close();
}

KUiInformation3::KUiInformation3()
{
	m_pCallerWnd			= NULL;
}


void KUiInformation3::Initialize()
{
	m_pCallerWnd = NULL;
	AddChild(&m_Information);
	AddChild(&m_FirstBtn);
	AddChild(&m_SecondBtn);
	AddChild(&m_ThirdBtn);
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiInformation3::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_Information .Init(&Ini, "Info");
		m_FirstBtn .Init(&Ini, "FirstBtn");
		m_SecondBtn.Init(&Ini, "SecondBtn");
		m_ThirdBtn.Init(&Ini, "ThirdBtn");
	}
}


void KUiInformation3::Show(const char* pInformation,
				const char* pszFirstBtnText,
				const char* pszSecondBtnText /*= 0*/,
				const char* pszThirdBtnText /*= "Xem nhÑ "*/,
				KWndWindow* pCallerWnd /*= 0*/,
				unsigned int uParam /*= 0*/,
                int nInformationLen /*= -1*/)
{
	if (pInformation)
	{	
		m_pCallerWnd = pCallerWnd;
		m_uCallerParam = uParam;
		m_Information.SetText(pInformation, nInformationLen);

		if (pszSecondBtnText && pszFirstBtnText == NULL)
		{
			pszFirstBtnText = pszSecondBtnText;
			pszSecondBtnText = NULL;
		}

		if (pszFirstBtnText)
		{
			m_FirstBtn.SetLabel(pszFirstBtnText);
			m_FirstBtn.Show();
		}
		else
		{
			m_FirstBtn.Hide();
		}

		if (pszSecondBtnText)
		{	
			m_SecondBtn.SetLabel(pszSecondBtnText);
			m_SecondBtn.Show();
		}
		else
		{
			m_SecondBtn.Hide();
		}

		if (pszThirdBtnText)
		{	
			m_ThirdBtn.SetLabel(pszThirdBtnText);
			m_ThirdBtn.Show();
		}
		else
		{
			m_ThirdBtn.Hide();
		}
		BringToTop();
		KWndShowAnimate::Show();
		Wnd_SetExclusive((KWndWindow*)this);
	}
}

void KUiInformation3::Hide(int nBtnIndex)
{
	if (m_pCallerWnd)
	{
		m_pCallerWnd->WndProc(WND_M_OTHER_WORK_RESULT,
			m_uCallerParam, WND_OPER_RESULT(nBtnIndex));
		m_pCallerWnd = NULL;
	}
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndShowAnimate::Hide();
}

void KUiInformation3::Close()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndShowAnimate::Hide();
}

int KUiInformation3::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_FirstBtn)
			Hide(0);
		else if (uParam == (unsigned int)(KWndWindow*)&m_SecondBtn)
			Hide(1);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ThirdBtn)
			Hide(2);
		break;
	case WM_KEYDOWN:
		if (uParam  == VK_RETURN || uParam == VK_SPACE || uParam == VK_ESCAPE)
		{
			if (!m_SecondBtn.IsVisible())
			{
				Hide(m_FirstBtn.IsVisible() ? 0 : -1);
				nRet = true;
			}
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}
