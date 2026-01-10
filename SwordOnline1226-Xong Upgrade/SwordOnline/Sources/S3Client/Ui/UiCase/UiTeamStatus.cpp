
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiTeamStatus.h"
#include "UiInformation.h"
#include "UiPlayerBar.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/CoreShell.h"
#include "../UiBase.h"
#include "KEngine.h"
#include "CoreShell.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI		"UiTeamStatus.ini"

KUiTeamStatus* KUiTeamStatus::m_pSelf = NULL;


KUiTeamStatus::KUiTeamStatus()
{
	memset(&m_Info, 0, sizeof(KUiPlayerTeam));
	m_pPlayersListMem = NULL;
}

// --
//
// --
KUiTeamStatus* KUiTeamStatus::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	else
		return NULL;
}


KUiTeamStatus* KUiTeamStatus::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTeamStatus;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiTeamStatus::CloseWindow(bool bDestroy /*= true*/)
{
	if (m_pSelf)
	{	
		m_pSelf->Clear(true);
		if(bDestroy)
		{	
			m_pSelf->Clear();
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			m_pSelf->Hide();
		}
	}	
}


void KUiTeamStatus::Initialize()
{	
	for(int i = 0; i < TEAM_NUM_MEMBER; i++)
	{
		AddChild(&m_TeamMem[i]);
	}
	AddChild(&m_TeamMember);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


void KUiTeamStatus::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		short nWidth, nHeight, nIconCount;
		Ini.GetShort("Main", "IconWidth", 0, &nWidth);
		Ini.GetShort("Main", "IconHeight", 0, &nHeight);
		Ini.GetShort("Main", "IconCount", 0, &nIconCount);
		for(int i = 0; i < TEAM_NUM_MEMBER; i++)
		{
			sprintf(Buff, "Team_%d", i);
			m_TeamMem[i].Init(&Ini, Buff);
			m_TeamMem[i].SetPosition(0, nHeight * i);
			m_TeamMem[i].SetSize(nWidth, nHeight);
			m_TeamMem[i].Hide();
		}
		this->SetSize(nWidth, nHeight * nIconCount);

		m_TeamMember.Init(&Ini, "MemberList");

	}	
}


void KUiTeamStatus::Breathe()
{
	Clear(1);
	UpdateData(NULL);
}

// --
//
// --
void KUiTeamStatus::Clear(bool bData)
{
	m_TeamMember.SetContent(0, 1, 0, 0);
	for(int i = 0; i < TEAM_NUM_MEMBER; i++)
	{
		m_TeamMem[i].Hide();
	}

	if (m_pPlayersListMem)
	{
		free(m_pPlayersListMem);
		m_pPlayersListMem = NULL;
	}
}
// --
//
// --
void KUiTeamStatus::UpdateData(KUiPlayerTeam* pInfo)
{	
	if (pInfo)
		m_Info = *pInfo;
	else if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (unsigned int)&m_Info, 0))
	{
		memset(&m_Info, 0, sizeof(m_Info));
	}


	if (m_Info.cNumMember > 0)
	{
		m_pPlayersListMem = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * (m_Info.cNumMember));
		if (m_pPlayersListMem)
		{
			int nCount = g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST,
				(unsigned int)m_pPlayersListMem, m_Info.cNumMember);
			_ASSERT(nCount == m_Info.cNumMember);
			m_TeamMember.SetContent((unsigned char*)m_pPlayersListMem, m_Info.cNumMember, sizeof(KUiPlayerItem),
				((char*)&m_pPlayersListMem->Name - (char*)m_pPlayersListMem));

			for(int i = 0; i < nCount; i++)
			{
				m_TeamMem[i].Show();

				int nCount = g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST,
				(unsigned int)m_pPlayersListMem, m_Info.cNumMember);
			}
			
		}
		else
		{
			m_Info.cNumMember = 0;
		}
	}
		
}

// --
//
// --
void KUiTeamStatus::UpdateNewState(int nCount)
{
	Clear(true);
}

// --
//
// --
void KUiTeamStatus::UpdateTime()
{

}

// --
//
// --
void KUiTeamStatus::UpdateToolTip(int nSlot, bool bBuff /*= true*/)
{	

}


int KUiTeamStatus::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{	
		case WND_M_MOUSE_ENTER:
			{
			
			}
			break;
		default:
			nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);	
	}
	return nRet;
}


// --
//
// --
void KUiTeamStatus::ConvertInfo(char* pszRet, int nTime, bool bFull /*= false*/)
{	
	
}

