
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiConnectInfo.h"
#include "UiLoginBg.h"
#include "UiInit.h"
#include "UiLogin.h"
#include "UiSelPlayer.h"
#include "UiMsgCentrePad.h"
#include "UiNewPlayer.h"
#include "UiInformation.h"
#include "UiSelServer.h"
#include "Text.h"
#include "../UiShell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../ENGINE/Src/KSG_MD5_String.h"
extern iRepresentShell*	g_pRepresentShell;

#include <crtdbg.h>

#define	SCHEME_INI				"UiConnectInfo.ini"

KUiConnectInfo* KUiConnectInfo::m_pSelf = NULL;


KUiConnectInfo::KUiConnectInfo()
{
	m_nDesireLoginStatus = LL_S_IDLE;

	m_szMessage[0] = 0;
	m_nMsgLen = 0;
	m_szLoginBg[0] = 0;
	m_nColor2CharacterStartIndex = 0;
	m_nColor2CharacterEndIndex = 0;
	m_nFont = 12;
	m_nParam = 0;
}

KUiConnectInfo* KUiConnectInfo::OpenWindow(LOGIN_BG_INFO_MSG_INDEX eIndex,
										int eDesireLoginStatus,
										int nParam /*= 0*/)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiConnectInfo;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		_ASSERT((m_pSelf->m_nDesireLoginStatus >= (int)LL_S_IDLE  &&
					m_pSelf->m_nDesireLoginStatus <= (int)LL_S_IN_GAME) ||
				(m_pSelf->m_nDesireLoginStatus >= (int)CI_NS_INIT_WND &&
					m_pSelf->m_nDesireLoginStatus <= (int)CI_NS_EXIT_PROGRAM));
		m_pSelf->m_nDesireLoginStatus = eDesireLoginStatus;
		m_pSelf->m_nParam = nParam;
		m_pSelf->SetInfoMsg(eIndex);
		KUiLoginBackGround::SetConfig(m_pSelf->m_szLoginBg);
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiConnectInfo::CloseWindow(bool bDestroy)
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

void KUiConnectInfo::Initialize()
{
	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 128);
	m_pSelf->LoadScheme(Scheme);
	
	Wnd_AddWindow(this, WL_NORMAL);
}


void KUiConnectInfo::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "RuningImgBg");
		m_ConfirmBtn.Init(&Ini, "ConfirmBtn");
		m_InputPwdWnd.Init(&Ini, "Password");
		m_DelRoleBtn.Init(&Ini, "DelRole");
		m_DelRoleBgImg.Init(&Ini, "DelRoleBgImg");
		m_CancelDelRoleBtn.Init(&Ini, "CancelDelRole");

		Ini.GetString("Message", "MsgColor", "0,0,0", Buff, sizeof(Buff));
		m_uMsgColor = GetColor(Buff);
		Ini.GetString("Message", "MsgBorderColor", "0,0,0", Buff, sizeof(Buff));
		m_uMsgBorderColor = GetColor(Buff);
		Ini.GetString("Message", "MsgColor2", "0,0,0", Buff, sizeof(Buff));
		m_uMsgColor2=  GetColor(Buff);
		Ini.GetString("Message", "MsgBorderColor2", "0,0,0", Buff, sizeof(Buff));
		m_uMsgBorderColor2=  GetColor(Buff);
		Ini.GetInteger("Message", "ColorChangeInterval", 0, (int*)&m_uMsgColorChangeInterval);
		Ini.GetInteger("Message", "Font", 12, &m_nFont);

		int	x, y, w, h;
		Ini.GetInteger2("Message", "Pos", &x, &y);
		Ini.GetInteger2("Message", "Size", &w, &h);
		m_nTextCentreX = x + w / 2;
		m_nTextCentreY = y;

		Ini.GetString("RuningImgBg", "LoginBg", "", m_szLoginBg, sizeof(m_szLoginBg));
	}
}

void KUiConnectInfo::OnClickConfirmBtn()
{
	switch(m_nDesireLoginStatus)
	{
	case CI_NS_INIT_WND:
		g_LoginLogic.ReturnToIdle();
		Hide();
		KUiInit::OpenWindow(false, false);
		break;
	case CI_NS_SEL_SERVER:
		g_LoginLogic.ReturnToIdle();
		Hide();
		KUiSelServer::OpenWindow();
		break;
	case CI_NS_LOGIN_WND:
		Hide();
		KUiLogin::OpenWindow();
		break;
	case CI_NS_SEL_ROLE_WND:
		Hide();
		KUiSelPlayer::OpenWindow();
		break;
	case CI_NS_NEW_ROLE_WND:
		Hide();
		KUiNewPlayer::OpenWindow(m_nParam);
		break;
	case CI_NS_CONFIRM_DEL_ROLE:
		SetInfoMsg(CI_MI_TO_DEL_ROLE);
		break;
	case CI_NS_EXIT_PROGRAM:
		Hide();
        UiPostQuitMsg();
		break;
	default:
		Hide();
		g_LoginLogic.ReturnToIdle();
		KUiSelServer::OpenWindow();
		break;
	}
}

int KUiConnectInfo::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBtn)
			OnClickConfirmBtn();
		else if (uParam == (unsigned int)(KWndWindow*)&m_DelRoleBtn)
			OnDelRole();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelDelRoleBtn)
			OnCancelDelRole();
		break;
	case WM_KEYDOWN:
		if (m_ConfirmBtn.GetParent() && (uParam == VK_RETURN || uParam == VK_ESCAPE))
			OnClickConfirmBtn();
		else if (m_DelRoleBtn.GetParent() && uParam == VK_RETURN)
			OnDelRole();
		else if (m_CancelDelRoleBtn.GetParent() && uParam == VK_ESCAPE)
			OnCancelDelRole();
		return 1;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}


bool KUiConnectInfo::OnDelRole()
{
	KSG_PASSWORD SupperPassword;
    char	szSupperPassword[KSG_PASSWORD_MAX_SIZE];

	if (m_InputPwdWnd.GetText(szSupperPassword, sizeof(szSupperPassword), false))
	{

        #ifdef SWORDONLINE_USE_MD5_PASSWORD

        KSG_StringToMD5String(SupperPassword.szPassword, szSupperPassword);

        #else

        #pragma message (KSG_ATTENTION("Add Password to MD5 string"))
        strncpy(SupperPassword.szPassword, szSupperPassword, sizeof(SupperPassword.szPassword));
        SupperPassword.szPassword[sizeof(SupperPassword.szPassword) - 1] = '\0';

        #endif


		m_InputPwdWnd.ClearText(true);
		
        if (g_LoginLogic.DeleteRole(m_nParam, SupperPassword))
		{
			SetInfoMsg(CI_MI_DELETING_ROLE);
			m_nDesireLoginStatus = LL_S_ROLE_LIST_READY;
		}
		else
		{
			SetInfoMsg(CI_MI_CONNECT_FAILED);
			m_nDesireLoginStatus = CI_NS_INIT_WND;
		}
		memset(&szSupperPassword, 0, sizeof(szSupperPassword));	
		memset(&SupperPassword, 0, sizeof(SupperPassword));	
	}
	else
	{
		SetInfoMsg(CI_MI_ERROR_CONFIRM_INPUT);
		m_nDesireLoginStatus = CI_NS_CONFIRM_DEL_ROLE;
	}
	return false;
}


void KUiConnectInfo::OnCancelDelRole()
{
	m_InputPwdWnd.ClearText(true);
	Hide();
	KUiSelPlayer::OpenWindow();
}


void KUiConnectInfo::SetInfoMsg(LOGIN_BG_INFO_MSG_INDEX eIndex)
{
	m_DelRoleBgImg.LeaveAlone();
	m_DelRoleBtn.LeaveAlone();
	m_CancelDelRoleBtn.LeaveAlone();
	m_ConfirmBtn.LeaveAlone();
	m_InputPwdWnd.LeaveAlone();

	m_szMessage[0] = 0;
	m_nMsgLen = 0;
	m_nColor2CharacterStartIndex = 0;
	m_nColor2CharacterEndIndex = 0;

	KIniFile*	pIni = g_UiBase.GetCommConfigFile();
	if (pIni)
	{
		char	szKey[8];
		_itoa(eIndex, szKey, 10);
		pIni->GetString("InfoString", szKey, "", m_szMessage, sizeof(m_szMessage));
										  

		// Convert ~ to \n for multiline support
		for (int i = 0; m_szMessage[i] != '\0'; i++)
		{
			if (m_szMessage[i] == '~')
				m_szMessage[i] = '\n';
		}

		m_nMsgLen = strlen(m_szMessage);
		if (m_nMsgLen)
		{
			m_uLastMsgColorChanageTime = IR_GetCurrentTime();
			m_nColor2CharacterEndIndex =
				TSplitString(m_szMessage, m_nColor2CharacterStartIndex + 1, false);
		}
	}

	if (eIndex == CI_MI_TO_DEL_ROLE)	//13=???????
	{
		AddChild(&m_DelRoleBgImg);
		AddChild(&m_InputPwdWnd);
		AddChild(&m_DelRoleBtn);
		AddChild(&m_CancelDelRoleBtn);
		Wnd_SetFocusWnd(&m_InputPwdWnd);
	}
	else if (eIndex != CI_MI_ENTERING_GAME)
	{
		AddChild(&m_ConfirmBtn);
	}
}


void KUiConnectInfo::GotoNextStep()
{
	LOGIN_LOGIC_STATUS eStatus = g_LoginLogic.GetStatus();
	switch(eStatus)
	{
	case LL_S_IDLE:
		Hide();
		KUiSelServer::OpenWindow();
		break;
	case LL_S_WAIT_INPUT_ACCOUNT:
		Hide();
		KUiLogin::OpenWindow();
		break;
	case LL_S_ACCOUNT_CONFIRMING:
		SetInfoMsg(CI_MI_CONNECTING);
		break;
	case LL_S_WAIT_ROLE_LIST:		
		SetInfoMsg(CI_MI_GETTING_ROLE_DATA);
		break;
	case LL_S_ROLE_LIST_READY:		
		Hide();
		KUiSelPlayer::OpenWindow();
		break;
	case LL_S_CREATING_ROLE:		
		SetInfoMsg(CI_MI_CREATING_ROLE);
		break;
	case LL_S_DELETING_ROLE:		
		SetInfoMsg(CI_MI_DELETING_ROLE);
		break;
	case LL_S_WAIT_EXTPOINT:
		SetInfoMsg(CI_MI_ACCOUNT_EXTPOINT);
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:	
		SetInfoMsg(CI_MI_CONNECTING);
		break;
	case LL_S_ENTERING_GAME:		
		SetInfoMsg(CI_MI_ENTERING_GAME);
		break;
	case LL_S_IN_GAME:
		UiStartGame();
		break;
	}
}


void KUiConnectInfo::Breathe()
{
	if (m_nDesireLoginStatus >= CI_NS_INIT_WND)
		return;

	LOGIN_LOGIC_STATUS eStatus = g_LoginLogic.GetStatus();
	LOGIN_LOGIC_RESULT_INFO eResult = g_LoginLogic.GetResult();
	if (eResult == LL_R_NOTHING)
	{
		if (eStatus == m_nDesireLoginStatus)
			GotoNextStep();
		return;
	}

	switch(eResult)
	{
	case LL_R_CONNECT_FAILED:
		SetInfoMsg(CI_MI_CONNECT_FAILED);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_CONNECT_SERV_BUSY:
		SetInfoMsg(CI_MI_CONNECT_SERV_BUSY);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_CONNECT_TIMEOUT:
		SetInfoMsg(CI_MI_CONNECT_TIMEOUT);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_ACCOUNT_PWD_ERROR:
		SetInfoMsg(CI_MI_ACCOUNT_PWD_ERROR);
		m_nDesireLoginStatus = CI_NS_LOGIN_WND;
		break;
	case LL_R_ACCOUNT_LOCKED:
		SetInfoMsg(CI_MI_ACCOUNT_LOCKED);
		m_nDesireLoginStatus = CI_NS_LOGIN_WND;
		break;
	case LL_R_ACCOUNT_FREEZE:
		SetInfoMsg(CI_MI_ACCOUNT_FREEZE);
		m_nDesireLoginStatus = CI_NS_LOGIN_WND;
		break;
	case LL_R_INVALID_ROLENAME:
		SetInfoMsg(CI_MI_ERROR_ROLE_NAME);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_SERVER_SHUTDOWN:		
		SetInfoMsg(CI_MI_SVRDOWN);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_INVALID_PASSWORD:
		SetInfoMsg(CI_MI_INVALID_PASSWORD);
		m_nDesireLoginStatus = CI_NS_CONFIRM_DEL_ROLE;
		break;
	case LL_R_INVALID_PROTOCOLVERSION:
		SetInfoMsg(CI_MI_INVALID_PROTOCOLVERSION);
		m_nDesireLoginStatus = CI_NS_INIT_WND;
		break;
	case LL_R_ACCOUNT_CONFIRM_SUCCESS:
		SetInfoMsg(CI_MI_GETTING_ROLE_DATA);
		break;
	case LL_R_CREATE_ROLE_SUCCESS:
		SetInfoMsg(CI_MI_ENTERING_GAME);
		break;
	case LL_R_ACCOUNT_NOT_ENOUGH_POINT:
		SetInfoMsg(CI_MI_NOT_ENOUGH_ACCOUNT_POINT);
		m_nDesireLoginStatus = CI_NS_LOGIN_WND;
		break;
	case LL_R_LOGIN_TO_GAMESERVER:
		KUiMsgCentrePad::OpenWindow();
		KUiMsgCentrePad::CloseWindow(false);
		break;
	case LL_R_DEL_ROLE_HAVE_TONG:
		SetInfoMsg(CI_MI_DEL_ROLE_HAVE_TONG);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_CONNECT_SERV_LOCKED:
		SetInfoMsg(CI_MI_CONNECT_SERV_LOCKED);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_CONNECT_SERV_DENIED:
		SetInfoMsg(CI_MI_CONNECT_SERV_DENIED);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	case LL_R_CONNECT_SERV_BANED:
		SetInfoMsg(CI_MI_CONNECT_SERV_BANED);
		m_nDesireLoginStatus = CI_NS_SEL_SERVER;
		break;
	}
}


void KUiConnectInfo::PaintWindow()
{
	KWndImage::PaintWindow();
	if (m_nMsgLen && g_pRepresentShell)
	{
		// Update animation
		if (IR_IsTimePassed(m_uMsgColorChangeInterval, m_uLastMsgColorChanageTime))
		{
			if (m_nColor2CharacterEndIndex != m_nMsgLen)
				m_nColor2CharacterStartIndex = m_nColor2CharacterEndIndex;
			else
				m_nColor2CharacterStartIndex = 0;
			m_nColor2CharacterEndIndex =
					TSplitString(m_szMessage, m_nColor2CharacterStartIndex + 1, false);
		}

		// Build line info for multiline rendering
		struct LineInfo { int start; int end; };
		LineInfo lines[16];
		int nLineCount = 0;
		int nTempStart = 0;

		for (int i = 0; i <= m_nMsgLen && nLineCount < 16; i++)
		{
			if (i == m_nMsgLen || m_szMessage[i] == '\n')
			{
				lines[nLineCount].start = nTempStart;
				lines[nLineCount].end = i;
				nLineCount++;
				nTempStart = i + 1;
			}
		}

		// Render text with animation for each line
		int nBaseX = m_nAbsoluteLeft + m_nTextCentreX;
		int nBaseY = m_nAbsoluteTop + m_nTextCentreY;
		char szSegment[256];

		for (int iLine = 0; iLine < nLineCount; iLine++)
		{
			int nLineStart = lines[iLine].start;
			int nLineEnd = lines[iLine].end;
			int nLineLen = nLineEnd - nLineStart;

			if (nLineLen <= 0) continue;

			int nY = nBaseY + iLine * (m_nFont + 2);
			int nLineX = nBaseX - nLineLen * m_nFont / 4;  // Center this line

			// Render three color segments for this line
			// Segment 1: Before animation (color1)
			if (m_nColor2CharacterStartIndex > nLineStart)
			{
				int nSegStart = nLineStart;
				int nSegEnd = (m_nColor2CharacterStartIndex < nLineEnd) ? m_nColor2CharacterStartIndex : nLineEnd;
				int nSegLen = nSegEnd - nSegStart;

				if (nSegLen > 0)
				{
					memcpy(szSegment, &m_szMessage[nSegStart], nSegLen);
					szSegment[nSegLen] = '\0';

					int nSegX = nLineX + (nSegStart - nLineStart) * m_nFont / 2;
					g_pRepresentShell->OutputText(m_nFont, szSegment, nSegLen,
						nSegX, nY, m_uMsgColor, 0, TEXT_IN_SINGLE_PLANE_COORD, m_uMsgBorderColor);
				}
			}

			// Segment 2: Animation (color2)
			if (m_nColor2CharacterStartIndex < nLineEnd && m_nColor2CharacterEndIndex > nLineStart)
			{
				int nSegStart = (m_nColor2CharacterStartIndex > nLineStart) ? m_nColor2CharacterStartIndex : nLineStart;
				int nSegEnd = (m_nColor2CharacterEndIndex < nLineEnd) ? m_nColor2CharacterEndIndex : nLineEnd;
				int nSegLen = nSegEnd - nSegStart;

				if (nSegLen > 0)
				{
					memcpy(szSegment, &m_szMessage[nSegStart], nSegLen);
					szSegment[nSegLen] = '\0';

					int nSegX = nLineX + (nSegStart - nLineStart) * m_nFont / 2;
					g_pRepresentShell->OutputText(m_nFont, szSegment, nSegLen,
						nSegX, nY, m_uMsgColor2, 0, TEXT_IN_SINGLE_PLANE_COORD, m_uMsgBorderColor2);
				}
			}

			// Segment 3: After animation (color1)
			if (m_nColor2CharacterEndIndex < nLineEnd)
			{
				int nSegStart = (m_nColor2CharacterEndIndex > nLineStart) ? m_nColor2CharacterEndIndex : nLineStart;
				int nSegEnd = nLineEnd;
				int nSegLen = nSegEnd - nSegStart;

				if (nSegLen > 0)
				{
					memcpy(szSegment, &m_szMessage[nSegStart], nSegLen);
					szSegment[nSegLen] = '\0';

					int nSegX = nLineX + (nSegStart - nLineStart) * m_nFont / 2;
					g_pRepresentShell->OutputText(m_nFont, szSegment, nSegLen,
						nSegX, nY, m_uMsgColor, 0, TEXT_IN_SINGLE_PLANE_COORD, m_uMsgBorderColor);
				}
			}
		}
	}
}