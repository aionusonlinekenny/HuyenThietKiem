
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiESCDlg.h"
#include "UiSelPlayer.h"
#include "UiInit.h"
#include "UiOptions.h"
#include "UiItem.h"
#include "UiSkills.h"
#include "UiStatus.h"
#include "UiConnectInfo.h"
#include "../UiShell.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "UiHelper2.h"
#include "UiTaskNote.h"
#include "../../../Engine/src/KDebug.h"
extern iCoreShell*	g_pCoreShell;

#define	SCHEME_INI_INIT 	"UiESCDlg.ini"

KUiESCDlg* KUiESCDlg::m_pSelf = NULL;

KUiESCDlg* KUiESCDlg::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiESCDlg;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->Show();
		m_pSelf->BringToTop();
	}
	return m_pSelf;
}


KUiESCDlg::~KUiESCDlg()
{
	Hide();
}


void KUiESCDlg::CloseWindow(bool bDestroy)
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


KUiESCDlg*	KUiESCDlg::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


void KUiESCDlg::Show()
{
	KWndShowAnimate::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}


void KUiESCDlg::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndShowAnimate::Hide();
}

KUiESCDlg::KUiESCDlg()
{	
	TimeLixian = 0;
}

int KUiESCDlg::Initialize()
{
	AddChild(&m_ExitGameBtn);
	AddChild(&m_HelpBtn);
	AddChild(&m_OptionsBtn);
	AddChild(&m_OfflineBtn);
	AddChild(&m_ContinueGameBtn);
	

	Wnd_AddWindow(this, WL_TOPMOST);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}


void KUiESCDlg::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_INIT);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ExitGameBtn.Init(&Ini, "ExitGame");
		m_HelpBtn.Init(&Ini, "GameHelp");
		m_OptionsBtn.Init(&Ini, "Options");
		m_OfflineBtn.Init(&Ini, "Offline");
		m_ContinueGameBtn.Init(&Ini, "ContinueGame");
	}	
}

int KUiESCDlg::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndWindow*)uParam);
		break;
	case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

int KUiESCDlg::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	KWndButton* pActive = NULL;
	KWndButton* pToActive = NULL;
	if (uKey == VK_RETURN)
	{
		if (pActive = GetActiveBtn())
		{
			OnClickButton(pActive);
		}
	}
	else if (uKey == VK_ESCAPE)
	{
		CloseWindow(false);
	}
	else if (uKey == VK_UP)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_ExitGameBtn)
			pToActive = &m_ContinueGameBtn;
		else if (pActive == &m_ContinueGameBtn)
			pToActive = &m_OfflineBtn;
		else if (pActive == &m_OfflineBtn)
			pToActive = &m_OptionsBtn;
		else if (pActive == &m_OptionsBtn)
			pToActive = &m_HelpBtn;
		else if (pActive == &m_HelpBtn) 
			pToActive = &m_ExitGameBtn;		
		else
			pToActive = &m_ExitGameBtn;
	}
	else if (uKey == VK_DOWN)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_ExitGameBtn)
			pToActive = &m_HelpBtn;
		else if (pActive == &m_HelpBtn)
			pToActive = &m_OptionsBtn;
		else if (pActive == &m_OptionsBtn)
			pToActive = &m_OfflineBtn;
		else if (pActive == &m_OfflineBtn)
			pToActive = &m_ContinueGameBtn;
		else if (pActive == &m_ContinueGameBtn)
			pToActive = &m_ExitGameBtn;
		else
			pToActive = &m_ExitGameBtn;
	}
	else
		nRet = 0;
	if (pToActive)
	{
		POINT		Pos;
		SIZE		Size;
		pToActive->GetAbsolutePos((int*)&Pos.x, (int*)&Pos.y);
		pToActive->GetSize((int*)&Size.cx, (int*)&Size.cy);
		Pos.x += Size.cx / 2;
		Pos.y += Size.cy / 2;
		Wnd_SetCursorPos((int)Pos.x, (int)Pos.y);
	}
	return nRet;
}

void KUiESCDlg::OnClickButton(KWndWindow* pWnd)
{
	if (pWnd == (KWndWindow*)&m_ContinueGameBtn)
		Hide();
	else if (pWnd == (KWndWindow*)&m_OptionsBtn)
	{
		Hide();
		KUiOptions::OpenWindow();
	}
	else if (pWnd == (KWndWindow*)&m_ExitGameBtn)
	{
		Hide();
		g_UiBase.SavePrivateConfig();
		g_pCoreShell->OperationRequest(GOI_EXIT_GAME, 0, 0);		
		g_LoginLogic.ReturnToIdle();
		UiEndGame();
		KUiInit::OpenWindow(true, false);
	}
	else if (pWnd == (KWndWindow*)&m_HelpBtn)
	{
		Hide();
		KUiHelper2::OpenWindow(true);
	}
	else if (pWnd == (KWndWindow*)&m_OfflineBtn)
	{
		char	szFunc[32];
		// Lixian by kinnox;
		// hang chuan VNG.
		if (g_pCoreShell->GetLixian())
		{
			sprintf(szFunc, "Offline");
			g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 3);
			
		}	
	}
}

void KUiESCDlg::LixianOut()
{
	m_pSelf->TimeLixian = GetTickCount();
}

void KUiESCDlg::Breathe()//Lixian by kinnox;
{
	if (TimeLixian > 0)
	{
		if (GetTickCount() - TimeLixian > 100)
		{
			TimeLixian = 0 ;
			m_pSelf->Hide();
			g_UiBase.SavePrivateConfig();
			g_pCoreShell->OperationRequest(GOI_EXIT_GAME, 0, 0);
			g_LoginLogic.ReturnToIdleLixian();	
			UiPostQuitMsg();
			UiEndGame();
			KUiInit::OpenWindow(true, false);

		}
	}
}
KWndButton*	KUiESCDlg::GetActiveBtn()
{
	KWndButton* pBtn = NULL;
	if (m_ContinueGameBtn.IsButtonActive())
		pBtn = &m_ContinueGameBtn;
	else if (m_OptionsBtn.IsButtonActive())
		pBtn = &m_OptionsBtn;
	else if (m_HelpBtn.IsButtonActive())
		pBtn = &m_HelpBtn;
	else if (m_OfflineBtn.IsButtonActive())
		pBtn = &m_OfflineBtn;
	else if (m_ExitGameBtn.IsButtonActive())
		pBtn = &m_ExitGameBtn;

	return pBtn;
}
