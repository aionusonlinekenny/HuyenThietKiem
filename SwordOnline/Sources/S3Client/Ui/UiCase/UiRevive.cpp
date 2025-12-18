
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiRevive.h"
#include "UiInformation.h"
#include "UiSysMsgCentre.h"
#include "../UiShell.h"
#include "../Elem/UiImage.h"
#include "../UiBase.h"
#include "../../../core/src/CoreShell.h"
#include "../../../Engine/Src/Text.h"
#include <crtdbg.h>

extern iCoreShell* g_pCoreShell;

#define	MSG_WAIT_TO_REVIVE	"B¹n ®· bÞ träng th­¬ng. Tù ®éng vÒ thµnh sau <color=red>%d<color> gi©y..."
#define MSG_NO_COINT		"B¹n kh«ng cã ®ñ <color=green>5<color> xu kh«ng thÓ håi sinh t¹i chç. Tù ®éng vÒ thµnh sau <color=red>%d<color> gi©y..."
#define REVIVE_INTERVAL		5000

#define	REVIVE_BTN_LABEL	"VÒ thµnh"
#define	REBORN_BTN_LABEL	"Håi sinh(5 xu)"

KReviveWnd*	KReviveWnd::m_pSelf = NULL;

bool KReviveWnd::LaunchRevive()
{
	_ASSERT(m_pSelf == NULL);
	if (m_pSelf == NULL)
		m_pSelf = new KReviveWnd;
	if (m_pSelf)
	{
		Wnd_AddWindow(m_pSelf, WL_TOPMOST);
		m_pSelf->FirstRevive();
	}
	return (m_pSelf != NULL);
}

void KReviveWnd::FirstRevive()
{	
	m_bWaitToRevive = true;
	m_uWaitReviveTime = IR_GetCurrentTime();
	m_bReborn = true;
}

void KReviveWnd::CloseWindow(BOOL bTimeout)
{	
	if(bTimeout)
	{
		g_UiInformation.Close();
	}
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KReviveWnd::Revive()
{	
	if(g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE, 0, REMOTE_REVIVE_TYPE);
	}
	CloseWindow();
}

void KReviveWnd::Reborn()
{	
/*	if(g_pCoreShell)
	{	
		int nExtPoint = 0;

		nExtPoint = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_EXTPOINT, 0, 0);

		if(nExtPoint >= COST_LOCAL_REVIVE)
		{	
			g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE, 0, LOCAL_REVIVE_TYPE);
			CloseWindow();
		}
		else
		{	
			m_bReborn = false;
		}
	}*/
	
}

void KReviveWnd::TimeoutRevive()
{	
	
	if(g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE, 0, REMOTE_REVIVE_TYPE);
	}
	CloseWindow(true);
}

void KReviveWnd::Breathe()
{
	char	szInfo[256];
	int		nInfoLen;
	if (m_bWaitToRevive)
	{
		unsigned int uRemain;
		uRemain = IR_GetRemainTime(REVIVE_INTERVAL, m_uWaitReviveTime);
		if (uRemain > 0)
		{	
			if(m_bReborn)
			{
				nInfoLen = sprintf(szInfo, MSG_WAIT_TO_REVIVE, (uRemain + 999) / 1000);
				nInfoLen = TEncodeText(szInfo, nInfoLen);
				g_UiInformation.Show(szInfo, REVIVE_BTN_LABEL, NULL, this, 0, nInfoLen);
			}
			else
			{
				nInfoLen = sprintf(szInfo, MSG_NO_COINT, (uRemain + 999) / 1000);
				nInfoLen = TEncodeText(szInfo, nInfoLen);
				g_UiInformation.Show(szInfo, REVIVE_BTN_LABEL, NULL, this, 0, nInfoLen);
			}
		}
		else
		{
			TimeoutRevive();
		}
			
	}
}

int KReviveWnd::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_M_OTHER_WORK_RESULT)
	{
		if (nParam == 1)
		{	
			Reborn();
		}
		else
		{
			Revive();	
		}
	}
	return 0;
}
