
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiFindPos.h"
#include "UiMiniMap.h"//PaintFindPos by kinnox;
#include "UiInformation.h"						  
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
							   

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiFindPos.ini"

KUiFindPos* KUiFindPos::m_pSelf = NULL;

KUiFindPos* KUiFindPos::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiFindPos;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->Show();
		m_pSelf->BringToTop();
		Wnd_SetFocusWnd(&m_pSelf->m_PosX);
	}
	return m_pSelf;
}

void KUiFindPos::CloseWindow()
{
	if (m_pSelf)
	{
			m_pSelf->Destroy();
			m_pSelf = NULL;

	}
}


KUiFindPos*	KUiFindPos::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


void KUiFindPos::UpdateData()
{	
	KUiSceneTimeInfo	pInfo;
	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&pInfo, 0);
	if (m_pSelf && g_pCoreShell)
	{
		m_pSelf->m_PosX.SetIntText(pInfo.nScenePos0 / 8);
		m_pSelf->m_PosY.SetIntText(pInfo.nScenePos1 / 8);
	}
}


int KUiFindPos::Initialize()
{	
	AddChild(&m_Title);
	AddChild(&m_Symbols);
	AddChild(&m_PosX);
	AddChild(&m_PosY);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	g_pCoreShell->SetPaintFindPos(FALSE);//PaintFindPos by kinnox;
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}


void KUiFindPos::LoadScheme(const char* pScheme)
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
			m_pSelf->m_Symbols.Init(&Ini, "Symbols");
			m_pSelf->m_PosX.Init(&Ini, "Input_X");
			m_pSelf->m_PosY.Init(&Ini, "Input_Y");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");

			m_pSelf->m_Title.SetText("NhËp täa ®é ®Ých ®Õn");
			m_pSelf->m_Symbols.SetText("/");
		}
	}	
}


int KUiFindPos::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		{
			g_pCoreShell->SetPaintFindPos(FALSE);//PaintFindPos by kinnox;
			OnCancel();
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		else if (uParam == VK_TAB)
		{
			if (Wnd_GetFocusWnd() == (KWndWindow*)&m_PosX)
				Wnd_SetFocusWnd(&m_PosY);
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_PosX || uParam == (unsigned int)(KWndWindow*)&m_PosY)
		{
			OnOk();
			nRet = 1;
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}


void KUiFindPos::OnOk()
{
	if(!g_pCoreShell)
	{
		CloseWindow();
		return;
	}

	OnCheckInput();

	// Get input coordinates (in Tâm units)
	int nDestX = m_PosX.GetIntNumber();
	int nDestY = m_PosY.GetIntNumber();


	// Validate: coordinates must be positive


	if(nDestX <= 0 || nDestY <= 0)
	{
		UIMessageBox("Vui lßng nhËp täa ®é hîp lÖ!", this);
		return;
	}

	// Use the same mechanism as flag/marker on minimap
	// This will automatically handle pathfinding and obstacles
	KUiMiniMap::SetValueFindPos(nDestX, nDestY);
	g_pCoreShell->AutoMove();

	CloseWindow();
}


void KUiFindPos::OnCheckInput()
{
	int nXpos = m_PosX.GetIntNumber();
	char	szBuff1[16], szBuff2[16];
	
	if (nXpos < 0)
		nXpos = 0;
	
	_itoa(nXpos, szBuff1, 10);
	m_PosX.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_PosX.SetIntText(nXpos);

	int nYpos = m_PosY.GetIntNumber();
	if (nYpos < 0)
		nYpos = 0;
	_itoa(nYpos, szBuff1, 10);
	m_PosY.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_PosY.SetIntText(nYpos);
}


void KUiFindPos::OnCancel()
{
	CloseWindow();
}