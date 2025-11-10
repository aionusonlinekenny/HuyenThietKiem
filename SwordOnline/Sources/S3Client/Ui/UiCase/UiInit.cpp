// -
//	文件名		：	UiInit.cpp
//	创建者		：	彭建波
//	创建时间	：	2002-9-10 11:27:04
//	功能描述	：	初始界面
//
// -
#include "KWin32.h"
#include "KIniFile.h"
#include "KFilePath.h"
#include "KMusic.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../Elem/SpecialFuncs.h"
#include "../UiBase.h"
#include "../UiShell.h"
#include "UiInit.h"
#include "UiLoginBg.h"
#include "UiOptions.h"
#include "UiConnectInfo.h"
#include "UiSelServer.h"
#include "UiTeamBuild.h"
#include "../UiSoundSetting.h"

extern KMusic*		g_pMusic;

#define	SCHEME_INI_INIT 	"UiInit.ini"
#define	LAUNCH_GAME_INI		"\\Ui\\LaunchGame.ini"


KUiInit* KUiInit::m_pSelf = NULL;


KUiInit* KUiInit::OpenWindow(bool bStartMusic, bool bJustLaunched)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiInit;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		if (bJustLaunched)
		{
			UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
			KUiLoginBackGround::OpenWindow(m_pSelf->m_szLoginBg);
			m_pSelf->Show();
			if (bStartMusic)
				PlayTitleMusic();
		}
		else
		{
			UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
			KUiLoginBackGround::OpenWindow(m_pSelf->m_szLoginBg);
			m_pSelf->Show();
			if (bStartMusic)
				PlayTitleMusic();
		}
		KUiOptions::LoadSetting(bJustLaunched, true);
	}
	return m_pSelf;
}


void KUiInit::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_ShowCursor(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}	
}

void KUiInit::PlayTitleMusic()
{
	char	szMusic[128] = "";
	KIniFile	Ini;
	if (Ini.Load(LAUNCH_GAME_INI))
	{
		int	nCount = 0;
		Ini.GetInteger("JustLaunched", "TitleMusicCount", 0, &nCount);
		if (nCount > 0)
		{
			char	szKey[16];
			sprintf(szKey, "TitleMusic_%d", rand() % nCount);
			Ini.GetString("JustLaunched", szKey, "", szMusic, sizeof(szMusic));
			if (szMusic[0])
			{
				g_pMusic->Stop();
				g_pMusic->Open((char*)szMusic);
				g_pMusic->Play(true);
			}
		}
	}
}

void KUiInit::StopTitleMusic()
{
	if (g_pMusic)
	{
		g_pMusic->Stop();
		g_pMusic->Close();
	}
}

void KUiInit::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_INIT);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_EnterGameBorder.	Init(&Ini, "EnterGameBorder");
		m_GameConfigBorder.	Init(&Ini, "GameConfigBorder");
		m_OpenRepBorder.	Init(&Ini, "OpenRepBorder");
		m_ExitGameBorder.		Init(&Ini, "ExitGameBorder");

		m_EnterGame.	Init(&Ini, "EnterGame");
		m_GameConfig.	Init(&Ini, "GameConfig");
		m_OpenRep.	Init(&Ini, "OpenRep");
		m_ExitGame.		Init(&Ini, "ExitGame");

		Ini.GetString("Main", "LoginBg", "", m_szLoginBg, sizeof(m_szLoginBg));
	}	
}


void KUiInit::Initialize()
{
	AddChild(&m_EnterGameBorder);
	AddChild(&m_GameConfigBorder);
	AddChild(&m_OpenRepBorder);
	AddChild(&m_ExitGameBorder);

	AddChild(&m_EnterGame);
	AddChild(&m_GameConfig);
	AddChild(&m_OpenRep);
	AddChild(&m_ExitGame);
	
	m_szLoginBg[0] = 0;
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}


int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndButton*)(KWndWindow*)uParam);
		break;
	case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	case WND_M_OTHER_WORK_RESULT:
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);	
	}
	return nRet;
}

int KUiInit::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	KWndButton* pActive = NULL;
	KWndButton* pToActive = NULL;
	if (uKey == VK_RETURN)
	{
		if (pActive = GetActiveBtn())
			OnClickButton(pActive);
	}
	else if (uKey == VK_UP)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_EnterGame)	
			pToActive = &m_ExitGame;
		else if (pActive == &m_ExitGame)
			pToActive = &m_OpenRep;
		else if (pActive == &m_OpenRep)
			pToActive = &m_GameConfig;
		else if (pActive == &m_GameConfig)
			pToActive = &m_EnterGame;
		else
			pToActive = &m_ExitGame;
	}
	else if (uKey == VK_DOWN)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_EnterGame)
			pToActive = &m_GameConfig;
		else if (pActive == &m_GameConfig)
			pToActive = &m_OpenRep;
		else if (pActive == &m_OpenRep)
			pToActive = &m_ExitGame;
		else
			pToActive = &m_EnterGame;
	}
	else
		nRet = 0;
	if (pToActive)
		pToActive->SetCursorAbove();
	return nRet;
}

void KUiInit::OnClickButton(KWndButton* pWnd)
{
	if (pWnd == &m_EnterGame)
	{
		//if (KUiSelServer::OpenWindow())
		if (KUiTeamBuild::OpenWindow())
			CloseWindow();
	}
	else if (pWnd == &m_GameConfig)
	{
		if (KUiOptions::OpenWindow(this))
			Hide();
	}
	else if (pWnd == &m_OpenRep)
	{
		ShellExecute(NULL,"explore",GetSrcPicPath(),NULL,NULL,SW_SHOWNORMAL);
	}
	else if (pWnd == &m_ExitGame)
	{
		CloseWindow();
		UiPostQuitMsg();
	}
}

KWndButton*	KUiInit::GetActiveBtn()
{
	KWndButton* pBtn = NULL;
	if (m_EnterGame.IsButtonActive())
		pBtn = &m_EnterGame;
	else if (m_GameConfig.IsButtonActive())
		pBtn = &m_GameConfig;	
	else if (m_OpenRep.IsButtonActive())
		pBtn = &m_OpenRep;	
	else if (m_ExitGame.IsButtonActive())
		pBtn = &m_ExitGame;		

	return pBtn;
}

void KUiInit::ShowCompleted()
{
    m_EnterGame.SetCursorAbove();
}
