
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiReconnect.h"
#include "UiInformation.h"
#include "UiSysMsgCentre.h"
#include "../UiShell.h"
#include "../UiCase/UiInit.h"
#include "../Elem/UiImage.h"
#include "../UiBase.h"
#include "../../Login/Login.h"
#include "../../../Engine/Src/Text.h"
#include <crtdbg.h>
#include "../../../core/src/coreshell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#define	MSG_RECONNECTING				"§ang thùc hiÖn kÕt nèi l¹i lÇn thø <color=red>%d<color> \n%s..."
#define	MSG_WAIT_TO_RECONNECT			"KÕt nèi l¹i lÇn thø <color=red>%d<color> sau %d gi©y!"
#define	MSG_WAIT_NEXT_GROUP_RECONNECT	"Sau %d gi©y tiÕn hµnh t¸i kÕt nèi!"
#define	MSG_HIDE_RECONNECT_MSG_ID		"21"

#define RECONNECT_GROUP_INTERVAL		60000
#define RECONNECT_INTERVAL				10000
#define INVISIBLE_RECONNECT_TIMES		100
#define INVISIBLE_RECONNECT_INTERVAL	20000
#define	RECONNECT_QUIT_BTN_LABEL		"Tho¸t"
#define	RECONNECT_START_AT_ONCE			"KÕt nèi"


KReconnectWnd*	KReconnectWnd::m_pSelf = NULL;
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;
// --
//
// --
bool KReconnectWnd::LaunchReconnect(int nMaxTimes)
{
	_ASSERT(m_pSelf == NULL);
	if (m_pSelf == NULL)
		m_pSelf = new KReconnectWnd;
	if (m_pSelf)
	{
		m_pSelf->m_bStop = false;
		Wnd_AddWindow(m_pSelf, WL_TOPMOST);
		if (nMaxTimes >= 1)
			m_pSelf->m_nMaxReconnectTimes = nMaxTimes;
		else
			m_pSelf->m_nMaxReconnectTimes = 1;
		m_pSelf->FirstReconnect();
	}
	else
	{
		Exit(true);
	}
	return (m_pSelf != NULL);
}

// --
//
// --
bool KReconnectWnd::IsReconnecttingGoingOn()
{
	return (m_pSelf != NULL);
}

// --
//
// --
void KReconnectWnd::FirstReconnect()
{
	m_nReconnectTimes = 0;
	m_bWaitToReconnect = true;
	m_uWaitStartTime = IR_GetCurrentTime();
	m_uToWaitTime = rand () % INVISIBLE_RECONNECT_INTERVAL + 1000;

	KIniFile* pConfig = g_UiBase.GetCommConfigFile();
	m_nHideTimes = INVISIBLE_RECONNECT_TIMES;
	if (pConfig)
	{
		pConfig->GetInteger("Main", "AutoReconnectHideTimes", 3, &m_nHideTimes);
		if (m_nHideTimes < 0)
			m_nHideTimes = 0;
	}
}

// --
//
// --
bool KReconnectWnd::StartReconnect()
{
	if (m_nHideTimes > 0 ||
		(++m_nReconnectTimes) < m_nMaxReconnectTimes)
	{
		if (m_nHideTimes > 0)
		{
			KSystemMessage	Msg;
			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 0;
			Msg.eType = SMT_NORMAL;
			Msg.uReservedForUi = 0;

			KIniFile*	pIni = g_UiBase.GetCommConfigFile();
			if (pIni)
			{
				if (pIni->GetString("InfoString", MSG_HIDE_RECONNECT_MSG_ID,
					"", Msg.szMessage, sizeof(Msg.szMessage)))
				{
					KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
				}
				g_UiBase.CloseCommConfigFile();
			}
		}

		g_LoginLogic.ReturnToIdle();
		g_LoginLogic.AutoLogin();
		m_bWaitToReconnect = false;
	}
	else
	{
		m_nReconnectTimes = 0;
		m_pSelf->m_bWaitToReconnect = true;
		m_pSelf->m_uWaitStartTime = IR_GetCurrentTime();
		m_pSelf->m_uToWaitTime = RECONNECT_GROUP_INTERVAL;
	}
	return true;
}

// --
//
// --
void KReconnectWnd::Exit(bool bQuitGame)
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
	if (bQuitGame)
	{	
		
		g_UiBase.SavePrivateConfig();
		
		g_LoginLogic.ReturnToIdle();
		UiEndGame();
		KUiInit::OpenWindow(true, false);
	}
}

int m_nChangeWorldReconnectTimes = 0;
unsigned int m_uLastReconnectTime         = 500; 
unsigned int m_uLastReconnectTick         = 0;

// --
//
// --
/*
void KReconnectWnd::Breathe()
{
	if (m_bStop)
		return;
	char	szInfo[256];
	int		nInfoLen;
	if (m_bWaitToReconnect)
	{
		unsigned int uRemain;
		if (uRemain = IR_GetRemainTime(m_uToWaitTime, m_uWaitStartTime))
		{
			if (m_nHideTimes == 0)
			{
				if (uRemain < RECONNECT_INTERVAL)
				{
					nInfoLen = sprintf(szInfo, MSG_WAIT_TO_RECONNECT,
						m_nReconnectTimes + 1, (uRemain + 999)/ 1000);
				}
				else
				{
					nInfoLen = sprintf(szInfo, MSG_WAIT_NEXT_GROUP_RECONNECT,
						(uRemain + 999) / 1000);
				}			
				nInfoLen = TEncodeText(szInfo, nInfoLen);
				g_UiInformation.Show(szInfo, RECONNECT_QUIT_BTN_LABEL, RECONNECT_START_AT_ONCE, this, 0, nInfoLen);
			}
		}
		else
		{
			StartReconnect();
		}
		return;
	}

	LOGIN_LOGIC_STATUS eStatus = g_LoginLogic.GetStatus();
	const char* pReconnectMsg = NULL;
	switch (eStatus)
	{
	case LL_S_IN_GAME:
		g_UiInformation.Close();
		UiResumeGame();
		Exit(false);
		break;
	case LL_S_IDLE:
		{
			LOGIN_LOGIC_RESULT_INFO eResult= g_LoginLogic.GetResult();
			if (eResult == LL_R_ACCOUNT_NOT_ENOUGH_POINT)
			{
				m_bStop = true;
			}
			else
			{
				m_bWaitToReconnect = true;
				m_uWaitStartTime = IR_GetCurrentTime();
				if (m_nHideTimes > 0)
				{
					m_nHideTimes--;
					m_uToWaitTime = INVISIBLE_RECONNECT_INTERVAL;
				}
				else
				{
					m_uToWaitTime = RECONNECT_INTERVAL;
					pReconnectMsg = "KÕt nèi l¹i thÊt b¹i.";
				}
			}
		}
		break;
	case LL_S_ACCOUNT_CONFIRMING:
		pReconnectMsg = "§ang x¸c thùc tµi kho¶n vµ mËt khÈu";
		break;
	case LL_S_WAIT_ROLE_LIST:
		pReconnectMsg = "§ang lÊy th«ng tin nh©n vËt";
		break;
	
	case LL_S_WAIT_EXTPOINT:
		pReconnectMsg = "§ang lÊy ÷ liÖu";
		break;
	
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:
		pReconnectMsg = "§ang kÕt nèi m¸y chñ";
		break;
	case LL_S_ENTERING_GAME:
		pReconnectMsg = "§ang ®¨ng nhËp vµo trß ch¬i";
		break;
	}

	if (m_bStop)
	{
		pReconnectMsg = "Tµi kho¶n ®· hÕt h¹n, vui lßng gia h¹n tríc khi tiÕp tôc trß ch¬i!";
		nInfoLen = strlen(pReconnectMsg);
		g_UiInformation.Show(pReconnectMsg, RECONNECT_QUIT_BTN_LABEL, NULL, this, 0, nInfoLen);
	}
	else if (pReconnectMsg && m_nHideTimes == 0)
	{
		nInfoLen = sprintf(szInfo, MSG_RECONNECTING,
				m_nReconnectTimes + 1, pReconnectMsg);
		nInfoLen = TEncodeText(szInfo, nInfoLen);
		g_UiInformation.Show(szInfo, RECONNECT_QUIT_BTN_LABEL, NULL, this, 0, nInfoLen);
	}
}

// --
//
// --
int KReconnectWnd::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_M_OTHER_WORK_RESULT)
	{
		if (nParam == 1)
		{
			StartReconnect();
			Breathe();
		}
		else
			Exit(true);
	}
	return 0;
}
*/

void KReconnectWnd::Breathe()
{
    if (m_bStop)
        return;

    char szInfo[256];
    int  nInfoLen;

    if (m_bWaitToReconnect)
    {
        /*
        if (m_nChangeWorldReconnectTimes < 3)
        {
            unsigned int now = IR_GetCurrentTime();
            if (now - m_uLastReconnectTick >= m_uLastReconnectTime)
            {
                StartReconnect();
                m_uLastReconnectTick = now;
                m_nChangeWorldReconnectTimes++;
                return; 
            }
        }*/
		if (m_nChangeWorldReconnectTimes < 3) 
		{ 
        StartReconnect();
        m_nChangeWorldReconnectTimes++;
        return;
		}

        unsigned int uRemain;
        if (uRemain = IR_GetRemainTime(m_uToWaitTime, m_uWaitStartTime))
        {
            if (m_nHideTimes == 0)
            {
                if (uRemain < RECONNECT_INTERVAL)
                {
                    nInfoLen = sprintf(szInfo, MSG_WAIT_TO_RECONNECT,
                                       m_nReconnectTimes + 1, (uRemain + 999) / 1000);
                }
                else
                {
                    nInfoLen = sprintf(szInfo, MSG_WAIT_NEXT_GROUP_RECONNECT,
                                       (uRemain + 999) / 1000);
                }
                nInfoLen = TEncodeText(szInfo, nInfoLen);
                g_UiInformation.Show(szInfo, RECONNECT_QUIT_BTN_LABEL, RECONNECT_START_AT_ONCE, this, 0, nInfoLen);
            }
        }
        else
        {
            StartReconnect();
        }
        return;
    }

    LOGIN_LOGIC_STATUS eStatus = g_LoginLogic.GetStatus();
    const char* pReconnectMsg = NULL;

    switch (eStatus)
    {
    case LL_S_IN_GAME:
        m_nChangeWorldReconnectTimes = 0;
        g_UiInformation.Close();
		if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, 0, 0);
        UiResumeGame();
        Exit(false);
        break;

    case LL_S_IDLE:
    {
        LOGIN_LOGIC_RESULT_INFO eResult = g_LoginLogic.GetResult();

        if (eResult == LL_R_ACCOUNT_NOT_ENOUGH_POINT)
        {
            m_bStop = true;
        }
        else
        {
            m_bWaitToReconnect = true;
            m_uWaitStartTime   = IR_GetCurrentTime();
            if (m_nHideTimes > 0)
            {
                m_nHideTimes--;
                m_uToWaitTime = INVISIBLE_RECONNECT_INTERVAL;
            }
            else
            {
                m_uToWaitTime  = RECONNECT_INTERVAL;
                pReconnectMsg  = "KÕt nèi l¹i thÊt b¹i.";
            }
        }
        break;
    }

    case LL_S_ACCOUNT_CONFIRMING:
        pReconnectMsg = "§ang x¸c thùc tµi kho¶n vµ mËt khÈu";
        break;

    case LL_S_WAIT_ROLE_LIST:
        pReconnectMsg = "§ang lÊy th«ng tin nh©n vËt";
        break;

    case LL_S_WAIT_EXTPOINT:
        pReconnectMsg = "§ang lÊy d÷ liÖu";
        break;

    case LL_S_WAIT_TO_LOGIN_GAMESERVER:
        pReconnectMsg = "§ang kÕt nèi m¸y chñ";
        break;

    case LL_S_ENTERING_GAME:
        pReconnectMsg = "§ang ®¨ng nhËp vµo trß ch¬i";
        break;
    }

    if (m_bStop)
    {
        pReconnectMsg = "Tµi kho¶n ®· hÕt h¹n, vui lßng gia h¹n tríc khi tiÕp tôc trß ch¬i!";
        nInfoLen = (int)strlen(pReconnectMsg);
        g_UiInformation.Show(pReconnectMsg, RECONNECT_QUIT_BTN_LABEL, NULL, this, 0, nInfoLen);
    }
    else if (pReconnectMsg && m_nHideTimes == 0)
    {
        nInfoLen = sprintf(szInfo, MSG_RECONNECTING,
                           m_nReconnectTimes + 1, pReconnectMsg);
        nInfoLen = TEncodeText(szInfo, nInfoLen);
        g_UiInformation.Show(szInfo, RECONNECT_QUIT_BTN_LABEL, NULL, this, 0, nInfoLen);
    }
}
int KReconnectWnd::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
    if (uMsg == WND_M_OTHER_WORK_RESULT)
    {
        if (nParam == 1)
        {
            StartReconnect();
            Breathe();
        }
        else
        {
            Exit(true);
        }
    }
    return 0;
}
