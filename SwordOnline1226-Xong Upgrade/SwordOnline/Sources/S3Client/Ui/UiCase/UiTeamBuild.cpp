
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiInit.h"
#include "UiInformation.h"
#include "UiSelServer.h"
#include "UiTeamBuild.h"
#include "Text.h"
#include "../UiShell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../ENGINE/Src/KSG_MD5_String.h"
extern iRepresentShell*	g_pRepresentShell;

#include <crtdbg.h>

#define	SCHEME_INI				"UiTeamBuild.ini"

KUiTeamBuild* KUiTeamBuild::m_pSelf = NULL;


KUiTeamBuild::KUiTeamBuild()
{
}

KUiTeamBuild* KUiTeamBuild::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTeamBuild;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiTeamBuild::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
		{
			m_pSelf->Hide();
		}
		else
		{
			Wnd_ReleaseExclusive(m_pSelf);
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

void KUiTeamBuild::Initialize()
{
	AddChild(&m_WarningBg);
	AddChild(&m_ConfirmBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_TextWarning);
	AddChild(&m_TextTeam);
	
	
	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 128);
	m_pSelf->LoadScheme(Scheme);
	Wnd_AddWindow(this, WL_NORMAL);
}


void KUiTeamBuild::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_WarningBg.Init(&Ini, "WarningBg");
		m_ConfirmBtn.Init(&Ini, "ConfirmBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_TextWarning.Init(&Ini, "TextWarning");
		m_TextWarning.SetText("C¶nh b¸o: Trß ch¬i kh«ng ph¶i lµ phiªn b¶n chÝnh thøc ®­îc ph¸t hµnh bëi VINAGAME company.\n§©y lµ phiªn b¶n t¸i hiÖn l¹i kh«ng nh»m môc ®Ých kinh doanh.\nXin vui lßng c©n nh¾c khi tham gia!");
		m_TextTeam.Init(&Ini, "TextTeam");
		m_TextTeam.SetText("Th«ng tin phiªn b¶n: S¬n Hµ X· T¾c\nPh¸t hµnh: NN Team\nPhiªn b¶n: 1.0\nWebsite: NNteam.com");
	}
}

void KUiTeamBuild::OnClickConfirmBtn()
{
	Hide();
	g_LoginLogic.ReturnToIdle();
	KUiSelServer::OpenWindow();
}

void KUiTeamBuild::OnClickCancelBtn()
{
	Hide();
	g_LoginLogic.ReturnToIdle();
	KUiInit::OpenWindow(true,false);
}

int KUiTeamBuild::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBtn)
			OnClickConfirmBtn();
		else if(uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnClickCancelBtn();
		break;
	case WM_KEYDOWN:
		if (m_ConfirmBtn.GetParent() && (uParam == VK_RETURN))
			OnClickConfirmBtn();
		else if (m_ConfirmBtn.GetParent() && (uParam == VK_ESCAPE))
			OnClickCancelBtn();
		return 1;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}

