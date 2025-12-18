
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiMakeAdv.h"
#include "UiInformation.h"
#include "UiItem.h"
#include "UiMiniMap.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiMakeAdv.ini"

KUiMakeAdv* KUiMakeAdv::m_pSelf = NULL;

KUiMakeAdv::KUiMakeAdv()
{

}


KUiMakeAdv* KUiMakeAdv::OpenWindow(const char* pszInitString, int nMinLen /*= 4*/, int nMaxLen /*= 16*/)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMakeAdv;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nMinLen = nMinLen;
		m_pSelf->m_nMaxLen = nMaxLen;
		if (pszInitString)
			m_pSelf->m_AdvEditor.SetText(pszInitString);
		else
			m_pSelf->m_AdvEditor.ClearText();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiMakeAdv::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}


KUiMakeAdv*	KUiMakeAdv::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiMakeAdv::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_AdvEditor);
	Wnd_SetExclusive((KWndWindow*)this);
}


void KUiMakeAdv::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}


void KUiMakeAdv::Initialize()
{
	AddChild(&m_AdvEditor);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_Title);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


void KUiMakeAdv::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_AdvEditor.Init(&Ini, "AdvEditor");
			m_pSelf->m_OkBtn.Init(&Ini, "BtnOK");
			m_pSelf->m_CancelBtn.Init(&Ini, "BtnCancel");
			m_pSelf->m_Title.Init(&Ini, "Title");
		}
	}
}


int KUiMakeAdv::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
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
			uParam == (unsigned int)(KWndWindow*)&m_AdvEditor)
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


void KUiMakeAdv::OnOk()
{
	char	szString[32];

	int nLen = m_AdvEditor.GetText(szString, sizeof(szString), true);
	if (nLen >= m_nMinLen && nLen <= m_nMaxLen)
	{	
		g_pCoreShell->OperationRequest(GOI_MAKE_ADV_TEXT, (unsigned int)szString, 0);
		CloseWindow(true);
	}
	else
	{
		UIMessageBox(((nLen < m_nMinLen) ?
			"Chuçi nhËp vµo qu¸ ng¾n!" : "Chuçi nhËp vao qu¸ dµi!"));
	}
}

void KUiMakeAdv::OnCancel()
{
	CloseWindow(true);
}
