
#include "KWin32.h"
#include "KIniFile.h"
#include "../UiBase.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiTongInformation.h"
#include "../UiSoundSetting.h"

#define	SCHEME_INI	"UiTongInformation.ini"

KUiTongInformation* KUiTongInformation::ms_pSelf = NULL;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongInformation::KUiTongInformation()
{
	m_pCallerWnd		= NULL;
	m_uCallerParam		= 0;
	m_nData				= 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongInformation* KUiTongInformation::OpenWindow(const char* pInformation, int nData,
											KWndWindow* pCallerWnd /*= NULL*/, unsigned int uParam /*= 0*/)
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTongInformation;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->BringToTop();
		ms_pSelf->Show(pInformation, nData, pCallerWnd, uParam);
	}
	return ms_pSelf;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongInformation* KUiTongInformation::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInformation::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{	
		Wnd_ReleaseExclusive((KWndWindow*)ms_pSelf);
		ms_pSelf->Hide();
		if (bDestory)
		{
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongInformation::Initialize()
{
	AddChild(&m_Information);
	AddChild(&m_FirstBtn);
	AddChild(&m_SecondBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongInformation::LoadScheme(const char* pScheme)
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
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongInformation::Show(const char* pInformation, int nData,
							  KWndWindow* pCallerWnd /*= 0*/, unsigned int uParam /*= 0*/)
{
	if (pInformation)
	{	
		m_nData = nData;
		m_pCallerWnd = pCallerWnd;
		m_uCallerParam = uParam;
		m_Information.SetText(pInformation);

		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		BringToTop();
		KWndShowAnimate::Show();
		Wnd_SetExclusive((KWndWindow*)this);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongInformation::OnClickBtn(int nBtnIndex)
{	
	if(nBtnIndex == 0)
	{
		if (m_pCallerWnd)
		{
			m_pCallerWnd->WndProc(WND_M_OTHER_WORK_RESULT, m_uCallerParam, m_nData);
			m_pCallerWnd	= NULL;
			m_nData			= 0;
		}
	}
	CloseWindow();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int KUiTongInformation::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_FirstBtn)
			OnClickBtn(0);
		else if (uParam == (unsigned int)(KWndWindow*)&m_SecondBtn)
			OnClickBtn(1);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}