
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiShell.h"
#include "UiInit.h"
#include "UiLoginBg.h"
#include "UiConnectInfo.h"
#include "UiSelServer.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "../../Login/Login.h"
#include <crtdbg.h>

#define	SCHEME_INI_SELSERV				"UiSelServer.ini"
#define	SERVER_LIST_FILE				"\\Settings\\ServerList.ini"

KUiSelServer* KUiSelServer::m_pSelf = NULL;


KUiSelServer::KUiSelServer()
{	
	m_pRegionList		= NULL;
	m_pServList			= NULL;
	m_szLoginBg[0]		= 0;
	m_bSelRegion		= true;
	m_nRegionIndex		= 0;
}

KUiSelServer::~KUiSelServer()
{
	m_RegionList.SetContent(NULL, 0, 0, 0);
	m_ServerList.SetContent(NULL, 0, 0, 0);
	if (m_pRegionList)
	{	
		free(m_pRegionList);
		m_pRegionList = NULL;
	}
	if (m_pServList)
	{	
		free(m_pServList);
		m_pServList = NULL;
	}
}

KUiSelServer* KUiSelServer::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiSelServer;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
		KUiLoginBackGround::SetConfig(m_pSelf->m_szLoginBg);
		m_pSelf->GetList();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiSelServer::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			m_pSelf->Hide();
		}
	}
}

void KUiSelServer::Initialize()
{
	AddChild(&m_RegionList);
	AddChild(&m_ScrollBar);
	AddChild(&m_ServerList);
	AddChild(&m_NameBigger);
	AddChild(&m_Login);
	AddChild(&m_Cancel);

	m_RegionList.SetScrollbar(&m_ScrollBar);
	m_ScrollBar.Hide();

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiSelServer::GetList()
{	
	if (m_pRegionList)
	{
		free(m_pRegionList);
		m_pRegionList = NULL;
	}

	if (m_pServList)
	{
		free(m_pServList);
		m_pServList = NULL;
	}

	int nCount, nSel;
	if (m_bSelRegion)
	{	
		m_pRegionList = g_LoginLogic.GetServerRegionList(nCount, m_nRegionIndex);
		if (m_pRegionList)
		{
			m_RegionList.SetContent((BYTE*)m_pRegionList, nCount, sizeof(KLoginServer), 0);
			m_RegionList.SetCurSel(m_nRegionIndex);
			
			m_pServList = g_LoginLogic.GetServerList(m_nRegionIndex, nCount, nSel);
			m_ServerList.SetContent((BYTE*)m_pServList, nCount, sizeof(KLoginServer), 0);
			m_ServerList.SetCurSel(nSel);
		}
	}
	else
	{	
		g_LoginLogic.LoadLoginChoice();
		
		m_pServList = g_LoginLogic.GetServerList(m_nRegionIndex, nCount, nSel);
		if (m_pServList)
		{	
			m_ServerList.SetContent((BYTE*)m_pServList, nCount, sizeof(KLoginServer), 0);
			m_ServerList.SetCurSel(nSel);
		}

		m_pRegionList = g_LoginLogic.GetServerRegionList(nCount, m_nRegionIndex);
		if (m_pRegionList)
		{
			m_RegionList.SetContent((BYTE*)m_pRegionList, nCount, sizeof(KLoginServer), 0);
			m_RegionList.SetCurSel(m_nRegionIndex);
		}
	}
}

void KUiSelServer::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SELSERV);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_RegionList     .Init(&Ini, "RegionList");
		m_ScrollBar.Init(&Ini, "Scroll");
		m_ServerList.Init(&Ini, "ServerList");
		m_NameBigger.Init(&Ini, "NameBigger");
		m_Login    .Init(&Ini, "Login");
		m_Cancel   .Init(&Ini, "Cancel");

		Ini.GetString("Main", "LoginBg", "", m_szLoginBg, sizeof(m_szLoginBg));

		short nWidth, nHeight;
		Ini.GetString("SelItemImage", "Image", "", Buff, sizeof(Buff));
		Ini.GetShort("SelItemImage", "Width", 0, &nWidth);
		Ini.GetShort("SelItemImage", "Height", 0, &nHeight);
		m_RegionList.SetImage(Buff, nWidth, nHeight); 
		m_ServerList.SetImage(Buff, nWidth, nHeight); 
	}
}

int KUiSelServer::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndButton*)(KWndWindow*)uParam);
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(m_bSelRegion)
		{
			m_RegionList.SetTopItemIndex(nParam);
		}
		else
		{
			m_ServerList.SetTopItemIndex(nParam);
		}
		break;
	case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	case WND_N_LIST_ITEM_SEL:
		if (nParam >= 0 && uParam == (unsigned int)(KWndWindow*)&m_RegionList)
			OnSelRegion(nParam);
		break;
	case WND_N_LIST_ITEM_D_CLICK:
		if (nParam >= 0 && uParam == (unsigned int)(KWndWindow*)&m_ServerList)
			OnLogin();
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

int KUiSelServer::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	int nSelRegion, nSelServer;
	switch (uKey)
	{
	case VK_RETURN:
		if (m_Cancel.IsButtonActive())
			OnCancel();
		else
			OnLogin();
		break;
	case VK_ESCAPE:
		OnCancel();
		break;
	case VK_LEFT:
		if(m_bSelRegion == false)
		{
			m_bSelRegion = true;
		}
		nSelRegion = m_RegionList.GetCurSel();
		if(nSelRegion < 0)
		{
			m_RegionList.SetCurSel(0);
		}
		else
		{
			m_RegionList.SetCurSel(nSelRegion);
		}
		m_ScrollBar.SetScrollPos(m_RegionList.GetTopItemIndex());
		
		break;
	case VK_RIGHT:
		if(m_bSelRegion == true)
		{
			m_bSelRegion = false;
		}
		nSelServer = m_ServerList.GetCurSel();
		if(nSelServer < 0)
		{
			m_ServerList.SetCurSel(0);
		}
		else
		{
			m_ServerList.SetCurSel(nSelServer);
		}
		m_ScrollBar.SetScrollPos(m_ServerList.GetTopItemIndex());	
		break;
	case VK_UP:
		if(m_bSelRegion)
		{	
			nSelRegion = m_RegionList.GetCurSel();
			if (nSelRegion  > 0)
			{
				m_RegionList.SetCurSel(nSelRegion - 1);
			}
			else if (nSelRegion < 0)
			{
				m_RegionList.SetCurSel(0);
			}
			m_ScrollBar.SetScrollPos(m_RegionList.GetTopItemIndex());
		}
		else 
		{	
			nSelServer = m_ServerList.GetCurSel();
			if (nSelServer  > 0)
			{
				m_ServerList.SetCurSel(nSelServer - 1);
			}
			else if (nSelServer < 0)
			{
				m_ServerList.SetCurSel(0);
			}
			m_ScrollBar.SetScrollPos(m_ServerList.GetTopItemIndex());
		}
		nRet = 1;
		break;
	case VK_DOWN:
		if(m_bSelRegion)
		{	
			nSelRegion = m_RegionList.GetCurSel();
			if (nSelRegion < m_RegionList.GetCount() - 1)
			{
				m_RegionList.SetCurSel(nSelRegion + 1);
			
			}
			else if (nSelRegion < 0)
			{
				m_RegionList.SetCurSel(0);
			}
			m_ScrollBar.SetScrollPos(m_RegionList.GetTopItemIndex());
		}
		else
		{	
			nSelServer = m_ServerList.GetCurSel();
			if (nSelServer < m_ServerList.GetCount() - 1)
			{
				m_ServerList.SetCurSel(nSelServer + 1);
			}
			else if (nSelServer < 0)
			{
				m_ServerList.SetCurSel(0);
			}
			m_ScrollBar.SetScrollPos(m_ServerList.GetTopItemIndex());
		}
		nRet = 1;
		break;
	default:
		nRet = 0;
		break;
	}

	return nRet;
}

void KUiSelServer::OnClickButton(KWndButton* pBtn)
{
	if (pBtn == &m_Login)
	{
		OnLogin();
	}
	else if(pBtn == &m_Cancel)
	{
		OnCancel();
	}
}

void KUiSelServer::OnSelRegion(int nSel)
{	
	KIniFile	Ini;
	if (Ini.Load(SERVER_LIST_FILE))
	{
		char	szKey[32], szBuffer[32];
		sprintf(szKey, "Region_%d", nSel);
		Ini.GetString("List", szKey, "", szBuffer, sizeof(szBuffer));
		if (szBuffer[0])
		{
			m_NameBigger.SetText(szBuffer);
		}
	}

	if(nSel == m_nRegionIndex)
		return;
	
	if (m_pRegionList)
	{	
		if(nSel > 0)
		{
			if (m_bSelRegion)
			{	
				m_bSelRegion = false;	
			}
			m_nRegionIndex = nSel;
			GetList();
			m_bSelRegion = true;
		}
	}
}

void KUiSelServer::OnLogin()
{
	int nSelServer = m_ServerList.GetCurSel();
	
	if (m_pServList && nSelServer >= 0)
	{

	    g_LoginLogic.SetAccountServer(m_pServList[nSelServer]);
		g_LoginLogic.CreateConnection(m_pServList[nSelServer].Address);
		KUiConnectInfo::OpenWindow(CI_MI_CONNECTING, LL_S_WAIT_INPUT_ACCOUNT);
		
		g_LoginLogic.SaveLoginChoice();
		
		CloseWindow(true);
		
	}
}

void KUiSelServer::OnCancel()
{
	CloseWindow(true);
	KUiInit::OpenWindow(false, false);
}
