#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include <crtdbg.h>
#include "UiWorldMap.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../core/src/CoreShell.h"
#include "../../../core/src/GameDataDef.h"

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;

KUiWorldMap* KUiWorldMap::m_pSelf = NULL;

#define		SCHEME_INI_WORLD		"UiWorldMap.ini"
#define		WORLD_MAP_INFO_FILE		"\\Settings\\MapList.ini"

// --
//
// --
void MapToggleStatus();

// --
//
// --
KUiWorldMap::KUiWorldMap()
{
	memset(&m_nLeft, 0, sizeof(short) * defMAX_UI_CITY);
	memset(&m_nTop, 0, sizeof(short) * defMAX_UI_CITY);
}
// --
//
// --
KUiWorldMap* KUiWorldMap::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiWorldMap;
		if (m_pSelf)
			m_pSelf->Initialize();
	}

	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->Show();
		m_pSelf->BringToTop();
		Wnd_SetExclusive(m_pSelf);
	}
	return m_pSelf;
}

// --
//
// --
void KUiWorldMap::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_ReleaseExclusive(m_pSelf);
		m_pSelf->Destroy();
		m_pSelf = NULL;
		MapToggleStatus();
	}
}

// --
//
// --
KUiWorldMap* KUiWorldMap::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	else
		return NULL;
}

// --
//
// --
void KUiWorldMap::Initialize()
{
	AddChild(&m_Country);
	AddChild(&m_City);
	AddChild(&m_Capital);
	AddChild(&m_Cave);
	AddChild(&m_Field);

	char szBuffer[128];
	g_UiBase.GetCurSchemePath(szBuffer, sizeof(szBuffer));
	strcat(szBuffer, "\\"SCHEME_INI_WORLD);
	KIniFile	Ini;
	if (Ini.Load(szBuffer))
	{
		Init(&Ini, "WorldMap");
		m_Country.Init(&Ini, "Country");
		m_City.Init(&Ini, "City");
		m_Capital.Init(&Ini, "Capital");
		m_Cave.Init(&Ini, "Cave");
		m_Field.Init(&Ini, "Field");


		Ini.GetShort2("fengxiang", "BtnStartPos", &m_nLeft[0], &m_nTop[0]);
		Ini.GetShort2("bianjing", "BtnStartPos", &m_nLeft[1], &m_nTop[1]);
		Ini.GetShort2("yangzhou", "BtnStartPos", &m_nLeft[2], &m_nTop[2]);
		Ini.GetShort2("linan", "BtnStartPos", &m_nLeft[3], &m_nTop[3]);
		Ini.GetShort2("xiangyang", "BtnStartPos", &m_nLeft[4], &m_nTop[4]);
		Ini.GetShort2("chengdu", "BtnStartPos", &m_nLeft[5], &m_nTop[5]);
		Ini.GetShort2("dali", "BtnStartPos", &m_nLeft[6], &m_nTop[6]);
	}

	Wnd_AddWindow(this, WL_TOPMOST);
	return;
}

// --
//
// --
int KUiWorldMap::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;

	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_KEYDOWN:
		CloseWindow();
		nResult = true;
		break;
	case WM_MOUSEMOVE:
		{

		}
		break;
	default:
		nResult = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nResult;
}

// --
//
// --
void KUiWorldMap::UpdateData()
{
	m_Country.Hide();
	m_City.Hide();
	m_Capital.Hide();
	m_Cave.Hide();
	m_Field.Hide();
	

	if (g_pCoreShell)
	{
		KIniFile	Ini;
		if (Ini.Load(WORLD_MAP_INFO_FILE))
		{
			char	szBuffer[128], szType[16];
			int nAreaX = -1, nAreaY = 0;
			KUiSceneTimeInfo Info;
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0);
			sprintf(szBuffer, "%d_MapPos", Info.nSceneId);
			Ini.GetInteger2("List", szBuffer, &nAreaX, &nAreaY);
			sprintf(szBuffer, "%d_MapType", Info.nSceneId);
			Ini.GetString("List", szBuffer, "", szType, sizeof(szType));
			if (nAreaX != -1)
			{
				int nWidth, nHeight;
				if(!strcmp(szType,"Country"))
				{
					m_Country.GetSize(&nWidth,  &nHeight);
					m_Country.SetPosition(nAreaX + nWidth - 8,
						nAreaY + nHeight - 2);
					m_Country.Show();
				}
				if(!strcmp(szType,"City"))
				{
					m_City.GetSize(&nWidth,  &nHeight);
					m_City.SetPosition(nAreaX + nWidth - 8,
						nAreaY + nHeight - 2);
					m_City.Show();
				}
				if(!strcmp(szType,"Capital"))
				{
					m_Capital.GetSize(&nWidth,  &nHeight);
					m_Capital.SetPosition(nAreaX + nWidth - 8,
						nAreaY + nHeight - 2);
					m_Capital.Show();
				}
				if(!strcmp(szType,"Cave"))
				{
					m_Cave.GetSize(&nWidth,  &nHeight);
					m_Cave.SetPosition(nAreaX + nWidth - 8,
						nAreaY + nHeight - 2);
					m_Cave.Show();
				}
				if(!strcmp(szType,"Field"))
				{
					m_Field.GetSize(&nWidth,  &nHeight);
					m_Field.SetPosition(nAreaX + nWidth - 8,
						nAreaY + nHeight - 2);
					m_Field.Show();
				}
			}
		}
	}
}



// --
//
// --
void KUiWorldMap::Breathe()
{
	if(m_Country.IsVisible())
		m_Country.NextFrame();
	if(m_City.IsVisible())
		m_City.NextFrame();
	if(m_Capital.IsVisible())
		m_Capital.NextFrame();
	if(m_Cave.IsVisible())
		m_Cave.NextFrame();
	if(m_Field.IsVisible())
		m_Field.NextFrame();
}
