/*****************************************************************************************
//	½çÃæ// --´¢ÎïÏä½çÃæ
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	03.06.2021
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiChaPW.h"
#include "UiKeyBoard.h"
#include "../../../core/src/coreshell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiInformation.h"
#include <crtdbg.h>
#include "KEngine.h"

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;


#define SCHEME_INI	"UiChaPW.ini"

KUiChaPW* KUiChaPW::m_pSelf = NULL;


KUiChaPW* KUiChaPW::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiChaPW* KUiChaPW::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiChaPW;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(false);

		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiChaPW::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -
// ¹¦ÄÜ	: ³õÊ¼»¯
// -
void KUiChaPW::Initialize()
{
	AddChild(&m_InputOld);
	AddChild(&m_Input);
	AddChild(&m_ReInput);
	AddChild(&m_InputOldBtn);
	AddChild(&m_InputBtn);
	AddChild(&m_ReInputBtn);
	
	AddChild(&m_ConfirmBtn);
	AddChild(&m_CloseBtn);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

// -
// ¹¦ÄÜ	: ÔØÈë½çÃæ·½°¸
// -
void KUiChaPW::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_InputOld.Init(&Ini, "InputOld");
		m_pSelf->m_InputOldBtn.Init(&Ini, "InputOldBtn");
		m_pSelf->m_Input.Init(&Ini, "Input");
		m_pSelf->m_InputBtn.Init(&Ini, "InputBtn");
		m_pSelf->m_ReInput.Init(&Ini, "ReInput");
		m_pSelf->m_ReInputBtn.Init(&Ini, "ReInputBtn");
		m_pSelf->m_ConfirmBtn.Init(&Ini, "ConfirmBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
	}
}

// -
// ¹¦ÄÜ	: ´°¿Úº¯Êý
// -
int KUiChaPW::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_InputOldBtn)
		{
			KUiKeyBoard::OpenWindow(this,WAIT_PW_OLDPASS);
			m_InputOld.SetText("");
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_InputBtn)
		{
			KUiKeyBoard::OpenWindow(this,WAIT_PW_INPUT);
			m_Input.SetText("");
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ReInputBtn)
		{
			KUiKeyBoard::OpenWindow(this,WAIT_PW_REINPUT);
			m_ReInput.SetText("");
		}	
		break;
	case WND_M_OTHER_WORK_RESULT:
		if(uParam == WAIT_PW_OLDPASS && nParam)
		{	
			m_nPWOld = nParam;
			if (m_nPWOld == 0)
				m_InputOld.SetText("");
			else
				m_InputOld.SetText("******");
			
		}
		else if(uParam == WAIT_PW_INPUT && nParam)
		{	
			m_nPW1 = nParam;
			if (m_nPW1 == 0)
				m_Input.SetText("");
			else
				m_Input.SetText("******");
			
		}
		else if(uParam == WAIT_PW_REINPUT && nParam)
		{
			m_nPW2 = nParam;
			if (m_nPW2 == 0)
				m_ReInput.SetText("");
			else
				m_ReInput.SetText("******");
		}
		break;	
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiChaPW::OnOk()
{
	if (OnCheckInput() == 1)
		return;
	g_pCoreShell->OperationRequest(GOI_PLAYER_CHANGE_PWD,m_nPWOld,m_nPW1);
	KUiKeyBoard::CloseWindow();
	CloseWindow();
}

int KUiChaPW::OnCheckInput()
{
	int nRet = 0;
	char	szBuff1[16], szBuff2[16], szBuff3[16];
	_itoa(m_nPW1, szBuff1, 10);
	_itoa(m_nPW2, szBuff2, 10);
	_itoa(m_nPWOld, szBuff3, 10);
	if (m_nPW1 != m_nPW2)
	{
		UIMessageBox(" MËt khÈu c¸c h¹ nhËp kh«ng trïng khíp nhau!",this,"Tho¸t");
		return 1;
	}
	else if (strlen(szBuff1) < 6 || strlen(szBuff2) < 6 || strlen(szBuff3) < 6 )
	{
		UIMessageBox(" MËt khÈu c¸c h¹ nhËp ph¶i cã ®ñ 6 kÝ tù sè           **Yªu cÇu mËt khÈu sè ®Çu tiªn ph¶i kh¸c sè '0' !",this,"Tho¸t");
		return 1;
	}
	else if (strlen(szBuff1) <= 0 || strlen(szBuff2) <= 0 || strlen(szBuff3) < 0)
	{
		UIMessageBox(" Vui lßng nhËp mËt m· b¶o vÖ!",this,"Tho¸t");
		return 1;
	}
	else
	{
		return nRet;
	}
}
