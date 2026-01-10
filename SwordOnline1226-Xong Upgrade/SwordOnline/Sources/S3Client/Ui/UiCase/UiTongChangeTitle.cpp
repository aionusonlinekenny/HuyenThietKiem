
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTongChangeTitle.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTongChangeTitle.ini"

KUiTongChangeTitle* KUiTongChangeTitle::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongChangeTitle::KUiTongChangeTitle()
{
	m_pRequester = NULL;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongChangeTitle* KUiTongChangeTitle::OpenWindow(KWndWindow* pRequester, unsigned int uParam,
 													int nMinLen, int nMaxLen)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTongChangeTitle;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nMinLen = nMinLen;
		m_pSelf->m_nMaxLen = nMaxLen;
		m_pSelf->m_pRequester = pRequester;
		m_pSelf->m_uRequesterParam = uParam;
		m_pSelf->m_TitleEdit.ClearText();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::CloseWindow(bool bDestroy)
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

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongChangeTitle*	KUiTongChangeTitle::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_TitleEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::Initialize()
{
	AddChild(&m_TitleEdit);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_TitleEdit.Init(&Ini, "TitleInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int KUiTongChangeTitle::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
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
			uParam == (unsigned int)(KWndWindow*)&m_TitleEdit)
		{
			OnOk();
			return 1;
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::OnOk()
{
	char	szString[32];
	int nLen = m_TitleEdit.GetText(szString, sizeof(szString), true);

	if (nLen >= m_nMinLen && nLen <= m_nMaxLen)
	{
		if (m_pRequester)
		{
			m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, (int)&szString);
		}
		else
		{
			g_pCoreShell->OperationRequest(GOI_ASK_CLIENT_STRING_CHOOSE, (unsigned int)szString, 0);
		}
		CloseWindow(true);
	}
	else
	{
		UIMessageBox(((nLen < m_nMinLen) ?
			"NhËp chuçi qu¸ Ýt!" : "NhËp chuçi qu¸ dµi!"));
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongChangeTitle::OnCancel()
{
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, 0);
	CloseWindow(true);
}
