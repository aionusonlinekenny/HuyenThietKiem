/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	07.06.2023
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiNotification.h"
#include "UiInformation.h"
#include "UiFaceSelector.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../Engine/src/KDebug.h"
#include "../../Engine/Src/Text.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiNotification.ini"


KUiNotification* KUiNotification::m_pSelf = NULL;

KUiNotification::KUiNotification()
{

}

//--------------------------------------------------------------------------
//	Open
//--------------------------------------------------------------------------
KUiNotification* KUiNotification::OpenWindow(const char* pszTitle, const char* pszInitString)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiNotification;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		
		if (pszTitle)
			m_pSelf->m_Title.SetText(pszTitle);

		m_pSelf->m_ContentList.Clear();

		char * strMessage = (char *)pszInitString;

		int nLen = strlen(strMessage);
		char Buffer[256];
		nLen = KUiFaceSelector::ConvertFaceText(Buffer, strMessage, nLen);
		nLen = TEncodeText(Buffer, nLen);
		if (nLen)
		{	
			m_pSelf->m_ContentList.AddOneMessage(Buffer, nLen);		
		}	
				
		m_pSelf->BringToTop();
		m_pSelf->Show();
		//Wnd_GameSpaceHandleInput(false);			
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	Close
//--------------------------------------------------------------------------
void KUiNotification::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);		
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
			
		m_pSelf->OnCancel();			
	
	}
}

KUiNotification*	KUiNotification::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	Khoi tao
//--------------------------------------------------------------------------
void KUiNotification::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_ContentList);

	//AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	Load
//--------------------------------------------------------------------------
void KUiNotification::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_ContentList.Init(&Ini, "ContentList");
			//m_pSelf->m_OkBtn.Init(&Ini, "Assemble");
			m_pSelf->m_CancelBtn.Init(&Ini, "Close");
			
		}
	}
}

//--------------------------------------------------------------------------
//	Su kien
//--------------------------------------------------------------------------
int KUiNotification::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		/*if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else*/ if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			CloseWindow(true);
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			//OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			CloseWindow(true);
			nRet = 1;
		}
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}
//--------------------------------------------------------------------------
//	Confirm
//--------------------------------------------------------------------------
/*void KUiNotification::OnOk()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_GIVEBOX_SCRIPT, (unsigned int)m_pSelf->szFunc,0);
	}
}*/

//--------------------------------------------------------------------------
//	Cancel
//--------------------------------------------------------------------------
void KUiNotification::OnCancel()
{
		CloseWindow(true);
}



