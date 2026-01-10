
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGetNumber.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiGetNumber.ini"

KUiGetNumber* KUiGetNumber::m_pSelf = NULL;


KUiGetNumber::KUiGetNumber()
{	
	m_nMaxNumber	= 0;
	m_pRequester	= NULL;
}


KUiGetNumber* KUiGetNumber::OpenWindow(const char* pszTitle,
					int nNumber, int nMaxNumber,	
					KWndWindow* pRequester, unsigned int uParam)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGetNumber;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nMaxNumber	= nMaxNumber;
		m_pSelf->m_pRequester	= pRequester;
		m_pSelf->m_uRequesterParam = uParam;
		if (pszTitle)
			m_pSelf->m_Title.SetText(pszTitle);
		else
			m_pSelf->m_Title.SetText("");
		m_pSelf->m_NumberEdit.SetIntText(nNumber);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiGetNumber::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

KUiGetNumber*	KUiGetNumber::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


void KUiGetNumber::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_NumberEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}


void KUiGetNumber::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}


void KUiGetNumber::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_NumberEdit);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


void KUiGetNumber::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_NumberEdit.Init(&Ini, "NumberInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}
}

int KUiGetNumber::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_NumberEdit)
		{
			OnOk();
			return 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}


void KUiGetNumber::OnOk()
{	
	OnCheckInput();

	int nNumber = m_NumberEdit.GetIntNumber();
	if (nNumber >= 0 && nNumber <= m_nMaxNumber)
	{	
		if (m_pRequester)
		{
			m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, nNumber);
		}
		else
		{
			g_pCoreShell->OperationRequest(GOI_ASK_CLIENT_NUMBER_CHOOSE, nNumber, 0);
		}
		CloseWindow(true);
	}
	else
	{
		UIMessageBox("Gi¸ trÞ nhËp vµo kh«ng ®óng!");
	}
}

void KUiGetNumber::OnCancel()
{
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, 0);
	CloseWindow(true);
}


void KUiGetNumber::OnCheckInput()
{
	int nNumber = m_NumberEdit.GetIntNumber();
	if (nNumber < 0)
		nNumber = 0;
	else if (nNumber > m_nMaxNumber)
		nNumber = m_nMaxNumber;
	char	szBuff1[16], szBuff2[16];
	_itoa(nNumber, szBuff1, 10);
	m_NumberEdit.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_NumberEdit.SetIntText(nNumber);
}
