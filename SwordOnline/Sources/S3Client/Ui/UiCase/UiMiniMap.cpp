#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiMiniMap.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "UiFindPos.h"
#include "UiWorldMap.h"
#include "UiCaveMap.h"
#include "UiShop.h"
#include "UiMarket.h"
#include "UiGetString.h"
#include "UiGetNumber.h"
#include "UiTongSetTax.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../Elem/TextPic.h"
#include "KEngine.h"
#include <time.h>

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*			g_pCoreShell;
extern IInlinePicEngineSink *g_pIInlinePicSink;

#define	SCHEME_INI_NOPIC	"UiMiniMapNotpic.ini"
#define	SCHEME_INI_SMALL	"UiMiniMap_Small.ini"
#define	SCHEME_INI_BIG		"UiMiniMap_Big.ini"

#define MAP_RECOVER_TIME		3000
#define	NOT_DRAGING_MAP			-1
#define	WAIT_TO_BE_SET_BACK		-2
#define	IS_DRAGING_MAP			(m_OldPos.x != NOT_DRAGING_MAP)
#define	IS_WAIT_TO_SET_BACK		(m_OldPos.x == WAIT_TO_BE_SET_BACK)

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	s_ModeName[MINIMAP_M_COUNT] = 
{
	"B¶n ®å nhá", 
	"B¶n ®å lín",
	"B¶n ®å thÕ giíi",
	"B¶n ®å s¬n ®éng",
	" §ãng b¶n ®å "
};

KUiMiniMap*			KUiMiniMap::ms_pSelf = NULL;
static MINIMAP_MODE	s_eMapMode = MINIMAP_M_NONE;
static MINIMAP_MODE	s_eMapOldMode = MINIMAP_M_BRIEF_PIC;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiMiniMap::KUiMiniMap()
{
	m_OldPos.x		= NOT_DRAGING_MAP;
	m_bFlagged		= false;
	memset(&m_szFlagImage, 0, sizeof(m_szFlagImage));
	m_bHavePicMap	= true;
			 
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiMiniMap* KUiMiniMap::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiMiniMap;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->Show();
		ms_pSelf->BringToTop();
	}
	return ms_pSelf;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiMiniMap* KUiMiniMap::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::Initialize()
{
	AddChild(&m_Shadow);
	AddChild(&m_SceneName);
	AddChild(&m_SwitchBtn);
	AddChild(&m_WorldMapBtn);
	AddChild(&m_CaveMapBtn);
	AddChild(&m_BtnFlag);
	AddChild(&m_ScenePos);
	m_nIsStopUpdate = 0;
	//
	AddChild(&m_TongName);
	AddChild(&m_TongMaster);
	AddChild(&m_ExchangeTax);
	AddChild(&m_PriceParam);
	AddChild(&m_Market);

	m_Style &= ~WND_S_VISIBLE;

	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_NORMAL);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::CloseWindow()
{
	if (ms_pSelf)
	{
		ms_pSelf->StopScrollMap();
		ms_pSelf->MapMoveBack();
		if (g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING, SCENE_PLACE_MAP_ELEM_NONE, 0);
		g_pCoreShell->SetPaintFindPos(FALSE);
		ms_pSelf->Destroy();
		ms_pSelf = NULL;
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool KUiMiniMap::IsHavePicMap()
{
	return ms_pSelf->m_bHavePicMap;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::LoadScheme(const char* pScheme)
{
	if (ms_pSelf)
	{
		char		szBuf[128];
		KIniFile	Ini;
		if (s_eMapMode == MINIMAP_M_BRIEF_NOT_PIC)
			sprintf(szBuf, "%s\\%s", pScheme, SCHEME_INI_NOPIC);
		else
			sprintf(szBuf, "%s\\%s", pScheme,
			s_eMapMode == MINIMAP_M_BRIEF_PIC ? SCHEME_INI_SMALL : SCHEME_INI_BIG);
		if (Ini.Load(szBuf))
		{
			ms_pSelf->LoadScheme(&Ini);
			ms_pSelf->m_nIsStopUpdate = 0;			
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::LoadScheme(KIniFile* pIni)
{
	if (pIni == NULL)
		return;
	Init(pIni, "MiniMap");
	m_Shadow.Init(pIni, "NameShadow");
	m_SceneName.Init(pIni, "SceneName");
	m_ScenePos.Init(pIni, "ScenePos");
	m_SwitchBtn.Init(pIni, "SwitchBtn");
	m_WorldMapBtn.Init(pIni, "WorldMapBtn");
	m_CaveMapBtn.Init(pIni, "CaveMapBtn");
	m_BtnFlag.Init(pIni, "BtnFlag");
	//
	m_TongName.Init(pIni, "TongName");
	m_TongMaster.Init(pIni,"TongMaster");
	m_ExchangeTax.Init(pIni, "ExchangeTax");
	m_PriceParam.Init(pIni, "PriceParam");
	m_Market.Init(pIni, "Market");

	pIni->GetInteger("MapRect", "Left",   0, (int*)&m_MapPos.x);
	pIni->GetInteger("MapRect", "Top",    0, (int*)&m_MapPos.y);
	pIni->GetInteger("MapRect", "Width",  0, (int*)&m_MapSize.cx);
	pIni->GetInteger("MapRect", "Height", 0, (int*)&m_MapSize.cy);
	pIni->GetString("BtnFlag", "FlagImage", "", m_szFlagImage, sizeof(m_szFlagImage));

	if (IsVisible() && g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING,
			SCENE_PLACE_MAP_ELEM_PIC | SCENE_PLACE_MAP_ELEM_CHARACTER | SCENE_PLACE_MAP_ELEM_PARTNER,
			(m_MapSize.cx | (m_MapSize.cy << 16)));
	}


	if(s_eMapMode == MINIMAP_M_BRIEF_PIC_BROWSE)
	{	
		m_TongName.Show();
		m_TongMaster.Show();
		m_ExchangeTax.Show();
		m_PriceParam.Show();	
	}
	else
	{
		m_TongName.Hide();
		m_TongMaster.Hide();
		m_ExchangeTax.Hide();
		m_PriceParam.Hide();
	}
	
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int KUiMiniMap::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_ScenePos)
		{
			if (g_pCoreShell->GetPaintFindPos())
			{
				g_pCoreShell->SetPaintFindPos(FALSE);
				m_bCalcPosMode = FALSE;
			}
			else 
			{
				if(KUiFindPos::GetIfVisible() == NULL)
				{
					KUiFindPos::OpenWindow();
				}
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnFlag)
		{
			g_pCoreShell->SetPaintFindPos(FALSE);
			m_bCalcPosMode = FALSE;
			m_bFlagged = false;
			if (g_pCoreShell->GetPaintFindPos())
			{
				g_pCoreShell->SetPaintFindPos(FALSE);
				m_bCalcPosMode = FALSE;
				m_bFlagged = false;
			}
			else
			{
				g_pCoreShell->SetPaintFindPos(TRUE);
				m_bCalcPosMode = TRUE;
				m_bFlagged = true;
			}		
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Market)
		{	
			if(KUiShop::GetIfVisible() != NULL)
				break;
			if(KUiGetString::GetIfVisible() != NULL)
				break;
			if(KUiGetNumber::GetIfVisible() != NULL)
				break;
			if(KUiTongSetTax::GetIfVisible() != NULL)
				break;
			if(g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0))
				break;
			char	szFunc[32];
			sprintf(szFunc, "OpenMarket|%d", g_pCoreShell->GetGameData(GDI_MARKET_SELLID, 0, 0));
			g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 1);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SwitchBtn)
			MapSetMode(s_eMapMode == MINIMAP_M_BRIEF_PIC ? MINIMAP_M_BRIEF_PIC_BROWSE : MINIMAP_M_BRIEF_PIC);
		else if (uParam == (unsigned int)(KWndWindow*)&m_WorldMapBtn)
			MapSetMode(MINIMAP_M_WORLD_MAP);
		else if (uParam == (unsigned int)(KWndWindow*)&m_CaveMapBtn)
			MapSetMode(MINIMAP_M_CAVE_MAP);
		break;
	case WM_LBUTTONDOWN:
		if(m_bFlagged)
		{
			m_bFlagged = false;
		}
		// break;
		if (m_bCalcPosMode && g_pCoreShell->GetPaintFindPos())
		{
			// Wnd_SetCapture(this);	
			Wnd_GetCursorPos((int *)&m_OldPos.x, (int *)&m_OldPos.y);
			g_pCoreShell->SetPaintFindPos(TRUE);
			m_bCalcPosMode = FALSE;
			g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, m_OldPos.x, m_OldPos.y, true, true);
		}
		else
		{
			//Wnd_SetCapture(this);
			Wnd_GetCursorPos((int *)&m_OldPos.x, (int *)&m_OldPos.y);		
		}
		break;
	case WM_LBUTTONUP:
		if (IS_DRAGING_MAP && ((uParam & MK_CONTROL) == 0) && g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		StopScrollMap();
		break;
	case WND_N_CHILD_MOVE:
		if (uParam == (unsigned int)(KWndWindow*)&m_SceneName ||
			uParam == (unsigned int)(KWndWindow*)&m_ScenePos ||
			uParam == (unsigned int)(KWndWindow*)&m_Shadow)
		{
			KWndWindow* pWnd = (KWndWindow*)uParam;
			int nMoveOffsetX = (short)(nParam & 0xffff);
			int nMoveOffsetY = (short)((nParam >> 16) & 0xffff);

			int x, y;
			pWnd->GetPosition(&x, &y);
			x -= nMoveOffsetX;
			y -= nMoveOffsetY;
			pWnd->SetPosition(x, y);
			GetPosition(&x, &y);
			x += nMoveOffsetX;
			y += nMoveOffsetY;
			SetPosition(x, y);
		}
		break;
	case WM_RBUTTONDOWN:
		Wnd_SetCapture(this);
		Wnd_GetCursorPos((int*)&m_OldPos.x, (int*)&m_OldPos.y);
		break;
	case WM_MOUSEMOVE:
		if(IS_DRAGING_MAP && (uParam & MK_RBUTTON))
		{
			int nCursorX, nCursorY;
			m_nIsStopUpdate = 1;
			Wnd_GetCursorPos(&nCursorX, &nCursorY);
			m_nOffsetX = (short)(m_OldPos.x - nCursorX);
			m_nOffsetY = (short)(m_OldPos.y - nCursorY);
			m_OldPos.x   =  nCursorX;
			m_OldPos.y   =  nCursorY;
			MapScroll(uParam & MK_CONTROL);
		}
		else if (IS_DRAGING_MAP && (uParam & MK_LBUTTON))
		{
	
		}			
		else
		{
			Wnd_TransmitInputToGameSpace(uMsg, uParam, nParam);
		}
		break;
	case WM_RBUTTONUP:
		if (IS_DRAGING_MAP && ((uParam & MK_CONTROL) == 0) && g_pCoreShell)
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		StopScrollMap();
		break;
	default:
		nRet = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::StopScrollMap()
{
	if (IS_DRAGING_MAP)
	{
		m_uLastScrollTime = IR_GetCurrentTime();
		Wnd_ReleaseCapture();
		m_OldPos.x = WAIT_TO_BE_SET_BACK;
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::MapMoveBack()
{
	m_OldPos.x = NOT_DRAGING_MAP;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	if (g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_FOCUS_OFFSET, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::SetValueFindPos(int nMpsX, int nMpsY)
{
	if (nMpsX && nMpsY)
	{
		if (ms_pSelf->m_bCalcPosMode)
			ms_pSelf->m_bCalcPosMode = FALSE;
		g_pCoreShell->SetPaintFindPos(TRUE);
		g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, nMpsX, nMpsY, false, true);
	}
	else
		return;
}
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::PaintWindow()
{
	KWndImage::PaintWindow();
	if(!g_pCoreShell)
		return;
		int nX = m_MapPos.x + m_nAbsoluteLeft;
		int nY = m_MapPos.y + m_nAbsoluteTop;
	if(m_bHavePicMap)
	{
		
		g_pCoreShell->SceneMapOperation(GSMOI_PAINT_SCENE_MAP, nX, nY);

		if (g_pRepresentShell)
		{
			KRURect	rect;
			rect.Color.Color_dw = 0xff000000;
			rect.oEndPos.nX = rect.oPosition.nX = nX - 1;
			rect.oEndPos.nY = rect.oPosition.nY = nY - 1;
			rect.oEndPos.nZ = rect.oPosition.nZ = 0;
			rect.oEndPos.nX += m_MapSize.cx;
			rect.oEndPos.nY += m_MapSize.cy;
			g_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);
		}
	}

	/*if(m_bFlagged)
	{
		int nCursorX, nCursorY;
		Wnd_GetCursorPos(&nCursorX, &nCursorY);
		
		if(nCursorX > nX && nCursorY < nY + m_MapSize.cy)
		{
			KRUImage RUIconImage;
			RUIconImage.nType = ISI_T_SPR;
			RUIconImage.Color.Color_b.a = 255;
			RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			RUIconImage.uImage = 0;
			RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
			RUIconImage.bRenderFlag = 0;
			strcpy(RUIconImage.szImage, m_szFlagImage);
			RUIconImage.oPosition.nX = nCursorX;
			RUIconImage.oPosition.nY = nCursorY - 10;
			RUIconImage.oPosition.nZ = 0;
			RUIconImage.nFrame = 0;
			g_pRepresentShell->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, true);
		}
	}*/
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::Breathe()
{
	if (IS_WAIT_TO_SET_BACK && g_pCoreShell &&
		IR_IsTimePassed(MAP_RECOVER_TIME, m_uLastScrollTime))
	{
		MapMoveBack();
	}
	int nCursorX, nCursorY;
	if (m_bCalcPosMode)
	{
		Wnd_GetCursorPos(&nCursorX, &nCursorY);
		g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, nCursorX, nCursorY, true, false);
	}

	if (!m_bFlagged && g_pCoreShell && g_pCoreShell->GetPaintFindPos())
	{
		g_pCoreShell->AutoMove();
	}
}



//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo)
{
	if (ms_pSelf && pInfo)
	{
		/*ms_pSelf->m_SceneName.SetText(pInfo->szSceneName);
		char szBuffer[16];
		sprintf(szBuffer, "%d/%d T×m", pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8);
		ms_pSelf->m_ScenePos.SetText(szBuffer);*/
		ms_pSelf->m_SceneName.SetText(pInfo->szSceneName);
		if (!ms_pSelf->m_nIsStopUpdate)
		{
			char sScenePos[32];
			if (ms_pSelf->m_bCalcPosMode && g_pCoreShell->GetPaintFindPos())
			{
				KUiSceneCalcPos	Spot;
				g_pCoreShell->SceneMapOperation(GSMOI_SCENE_CALCFLAG, (unsigned int)&Spot, 0);
				sprintf(sScenePos, "%d/%d T×m", pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8);
			}
			else
				sprintf(sScenePos, "%d/%d T×m", pInfo->nScenePos0 / 8, pInfo->nScenePos1 / 8);
			ms_pSelf->m_ScenePos.SetText(sScenePos);
			ms_pSelf->nPosXCurrent = pInfo->nScenePos0 / 8;
			ms_pSelf->nPosYCurrent = pInfo->nScenePos1 / 8;
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::Show()
{
	KWndImage::Show();
	if (g_pCoreShell)
	{
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING,
			SCENE_PLACE_MAP_ELEM_PIC | SCENE_PLACE_MAP_ELEM_CHARACTER | SCENE_PLACE_MAP_ELEM_PARTNER,
			(ms_pSelf->m_MapSize.cx | (ms_pSelf->m_MapSize.cy << 16)));	

		KSceneMapInfo MapInfo;
		if(!g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_INFO, (unsigned int)&MapInfo, 0))
		{ 
			m_bHavePicMap = false;
			ms_pSelf->SetSize(m_MapSize.cx, 14);
			m_ScenePos.Hide();
			m_SwitchBtn.Hide();
			m_BtnFlag.Hide();
			m_CaveMapBtn.Hide();
			m_WorldMapBtn.SetPosition(115,0);

		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::Hide()
{
	KWndImage::Hide();
	if (g_pCoreShell)
		g_pCoreShell->SceneMapOperation(GSMOI_IS_SCENE_MAP_SHOWING, SCENE_PLACE_MAP_ELEM_NONE, 0);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiMiniMap::MapScroll(int nbScrollScene)
{	
	if(!g_pCoreShell)
		return;
	KSceneMapInfo MapInfo;
    if(g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_INFO, (unsigned int)&MapInfo, 0))
	{
		int nX = MapInfo.nOrigFocusH + MapInfo.nFocusOffsetH + MapInfo.nScallH * m_nOffsetX;
		int nY = MapInfo.nOrigFocusV + MapInfo.nFocusOffsetV + MapInfo.nScallV * m_nOffsetY;
		if (nX < MapInfo.nFocusMinH)
			nX = MapInfo.nFocusMinH; 
		else if (nX > MapInfo.nFocusMaxH)
			nX = MapInfo.nFocusMaxH;
		if (nY < MapInfo.nFocusMinV)
			nY = MapInfo.nFocusMinV;
		else if (nY > MapInfo.nFocusMaxV)
			nY = MapInfo.nFocusMaxV;

		nX -= MapInfo.nOrigFocusH;
		nY -= MapInfo.nOrigFocusV;
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_FOLLOW_WITH_MAP, 0, 0);
		g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_FOCUS_OFFSET, (unsigned int)nX, nY);

	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void MapToggleStatus()
{	
	if(KUiMiniMap::GetIfVisible())
	{
		if(!KUiMiniMap::IsHavePicMap())
			return;
	}
	if (s_eMapMode == MINIMAP_M_NONE || s_eMapMode == MINIMAP_M_WORLD_MAP || s_eMapMode == MINIMAP_M_CAVE_MAP)
		MapSetMode(MINIMAP_M_BRIEF_PIC);
	else if(s_eMapMode == MINIMAP_M_BRIEF_PIC)
		MapSetMode(MINIMAP_M_BRIEF_PIC_BROWSE);
	/*else if(s_eMapMode == MINIMAP_M_BRIEF_PIC_BROWSE)
		MapSetMode(MINIMAP_M_NONE);*/
	else if (s_eMapMode == MINIMAP_M_BRIEF_NOT_PIC)
	{

	}
	else
		MapSetMode(MINIMAP_M_NONE);
}
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
MINIMAP_MODE MapGetMode()
{
	return s_eMapMode;
}
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void MapSetMode(MINIMAP_MODE eMode)
{
	if (s_eMapMode == MINIMAP_M_BRIEF_NOT_PIC && eMode != MINIMAP_M_BRIEF_PIC && eMode < MINIMAP_M_WORLD_MAP)
		return;
	if ((s_eMapMode == MINIMAP_M_WORLD_MAP || s_eMapMode == MINIMAP_M_CAVE_MAP) && s_eMapOldMode == MINIMAP_M_BRIEF_NOT_PIC)
		eMode = MINIMAP_M_BRIEF_NOT_PIC;

/*	if (eMode == s_eMapMode &&
		(eMode < MINIMAP_M_NONE || eMode > MINIMAP_M_CAVE_MAP))
		return;

	if (s_eMapMode != MINIMAP_M_WORLD_MAP && s_eMapMode != MINIMAP_M_CAVE_MAP)
		s_eMapOldMode = s_eMapMode;*/
	if (eMode == s_eMapMode && (eMode < MINIMAP_M_NONE || eMode > MINIMAP_M_WORLD_MAP || eMode > MINIMAP_M_CAVE_MAP))
		return;
	if (s_eMapMode != MINIMAP_M_WORLD_MAP)
		s_eMapOldMode = s_eMapMode;

	switch(s_eMapMode = eMode)
	{
		case MINIMAP_M_NONE:
			KUiMiniMap::CloseWindow();
			KUiWorldMap::CloseWindow();
			KUiCaveMap::CloseWindow();
			break;
		case MINIMAP_M_BRIEF_NOT_PIC:
		case MINIMAP_M_BRIEF_PIC:
		case MINIMAP_M_BRIEF_PIC_BROWSE:
			KUiWorldMap::CloseWindow();
			KUiCaveMap::CloseWindow();
			if(KUiMiniMap::GetIfVisible() == NULL)
			{
				
				KUiMiniMap::OpenWindow();
			}
			else
			{	
				char Scheme[128];
				g_UiBase.GetCurSchemePath(Scheme, 256);
				KUiMiniMap::LoadScheme(Scheme);
			}
			break;
		case MINIMAP_M_WORLD_MAP:		
			KUiMiniMap::CloseWindow();
			KUiCaveMap::CloseWindow();
			KUiWorldMap::OpenWindow();
			break;
		case MINIMAP_M_CAVE_MAP:
			if(!KUiMiniMap::IsHavePicMap())
				break;
			KUiMiniMap::CloseWindow();
			KUiWorldMap::CloseWindow();
			KUiCaveMap::OpenWindow();
			break;
	}
}
