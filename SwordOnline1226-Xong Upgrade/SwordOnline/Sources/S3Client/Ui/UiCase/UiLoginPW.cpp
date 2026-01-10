/*****************************************************************************************
//	ΩÁ√Ê// --¥¢ŒÔœ‰ΩÁ√Ê
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	03.06.2021
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiLoginPW.h"
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


#define SCHEME_INI	"UiLoginPW.ini"

KUiLoginPW* KUiLoginPW::m_pSelf = NULL;


KUiLoginPW* KUiLoginPW::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiLoginPW* KUiLoginPW::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiLoginPW;
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


void KUiLoginPW::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -
// π¶ƒ‹	: ≥ı ºªØ
// -
void KUiLoginPW::Initialize()
{

	AddChild(&m_Input);
	AddChild(&m_InputBtn);
	
	AddChild(&m_ConfirmBtn);
	AddChild(&m_CloseBtn);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

// -
// π¶ƒ‹	: ‘ÿ»ÎΩÁ√Ê∑Ω∞∏
// -
void KUiLoginPW::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Input.Init(&Ini, "Input");
		m_pSelf->m_InputBtn.Init(&Ini, "InputBtn");
		m_pSelf->m_ConfirmBtn.Init(&Ini, "ConfirmBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
	}
}

// -
// π¶ƒ‹	: ¥∞ø⁄∫Ø ˝
// -
int KUiLoginPW::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_InputBtn)
		{
			KUiKeyBoard::OpenWindow(this,WAIT_PW_INPUT);
			m_Input.SetText("");
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if(uParam == WAIT_PW_INPUT && nParam)
		{	
			m_nPW1 = nParam;
			if (m_nPW1 == 0)
				m_Input.SetText("");
			else
				m_Input.SetText("******");
			
		}
		break;	
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiLoginPW::OnOk()
{
	if (OnCheckInput() == 1)
		return;
	g_pCoreShell->OperationRequest(GOI_PLAYER_LOGIN_LOCK_PWD,m_nPW1,0);
	if (g_pCoreShell)
	g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, 0, 0);

	KUiKeyBoard::CloseWindow();
	CloseWindow();
}

int KUiLoginPW::OnCheckInput()
{
	int nRet = 0;
	char	szBuff1[16];
	_itoa(m_nPW1, szBuff1, 10);
	if (strlen(szBuff1) < 6)
	{
		UIMessageBox(" MÀt kh»u c∏c hπ nhÀp ph∂i c„ ÆÒ 6 k› t˘ sË           **Y™u c«u mÀt kh»u sË Æ«u ti™n ph∂i kh∏c sË '0' !",this,"Tho∏t");
		return 1;
	}
	else if (strlen(szBuff1) <= 0)
	{
		UIMessageBox(" Vui lﬂng nhÀp mÀt m∑ b∂o v÷!",this,"Tho∏t");
		return 1;
	}
	else
	{
		return nRet;
	}
}
