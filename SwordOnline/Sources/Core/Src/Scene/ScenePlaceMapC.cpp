// ***************************************************************************************
// 地图的类定义实现
// Copyright : Kingsoft 2003
// Author    : wooy(wu yue)
// CreateTime: 2003-7-8
// ***************************************************************************************
#include "KCore.h"
#include "ScenePlaceMapC.h"
#include "crtdbg.h"
#include "../KPlayer.h"
#include "../ImgRef.h"
#include "../GameDataDef.h"
#include "../CoreShell.h"
#include "KScenePlaceRegionC.h"
#include "SceneDataDef.h"
#include    "../../Engine/Src/KSG_StringProcess.h"
#include "KScenePlaceC.h"//PaintFindPos by kinnox;
extern KScenePlaceC        g_ScenePlace;//PaintFindPos by kinnox;
#define	PLACE_MAP_FILE_NAME_APPEND		"24.jpg"
#define	PLACE_MAP_SAVE_SECTION			"MAIN"
#define	MAP_SETTING_FILE				"\\Ui\\Setting.ini"
#define FLAG_ICON						"\\Spr\\Ui3\\小地图\\地图小旗帜.spr"
#define	PLACE_MAP_SYMBOL_SIZE			5//PaintFindPos by kinnox;
#define	PLACE_MAP_FLAG_FILE_PATH		"\\Spr\\Ui3\\UiMiniMap\\FlagImage.spr"//PaintFindPos by kinnox;
#define	PLACE_MAP_FOLLOW_FILE_PATH		"\\Spr\\Ui3\\UiMiniMap\\FollowImage.spr"// find way by kinnox;
#define	PLACE_MAP_FOLLOW_FILE_PATH_1	"\\Spr\\Ui3\\UiMiniMap\\FollowImage2.spr"// find way by kinnox;
#define	WORLD_MAP_TRAFFIC_FILE			"\\Settings\\maps\\MapTraffic.ini"
#define	RIGHT_BOTTOM_NO_LIMIT			0x7fffffff
#define	MAX_NUM_PARTNER					8
#define	MAX_NUM_CHARACTER				40
// --
//
// --
KScenePlaceMapC::KScenePlaceMapC()
{
	m_bHavePicMap = false;
	m_bInited = false;
	nFindDesX = 0;
	nFindDesY = 0;
	m_szEntireMapFile[0] = 0;
	m_EntireMapLTPosition.x = m_EntireMapLTPosition.y = 0;
	m_FocusPosition.x = m_FocusPosition.y = 0;
	m_FocusLimit.left = m_FocusLimit.right = m_FocusLimit.top = m_FocusLimit.bottom = 0;
	m_PicLoadedLTPosition.x = m_PicLoadedLTPosition.y = 0;
	m_Size.cx = m_Size.cy = 0;
	m_MapCoverArea.left = m_MapCoverArea.right = m_MapCoverArea.top = m_MapCoverArea.bottom = 0;
	m_PaintCell.left = m_PaintCell.right = m_PaintCell.top = m_PaintCell.bottom = 0;
	m_uMapShowElems = SCENE_PLACE_MAP_ELEM_NONE;
	memset(&m_ElemsList, 0, sizeof(m_ElemsList));
	m_pEntireMap = NULL;
	//
	m_pPartnerPos		= NULL;
	m_pFootSpot			= NULL;
	m_nLineDestX		= 0;
	m_nLineDestX		= 0;
	m_nCountTraffic		= 0;
	m_MapTraffic		= NULL;
	m_pPointTraffic		= NULL;
	m_pContentTraffic	= NULL;
	m_bLine = false;//PaintFindPos by kinnox;
	m_FlagPos.x = m_FlagPos.y = -123456; //PaintFindPos by kinnox;
}

// --
//
// --
KScenePlaceMapC::~KScenePlaceMapC()
{
	Terminate();
	//
	if(m_pFootSpot)
	{
		delete [] m_pFootSpot;
		m_pFootSpot = NULL;
	}
	if(m_pPartnerPos)
	{
		delete [] m_pPartnerPos;
		m_pPartnerPos = NULL;
	}

	m_nCountTraffic		= 0;
	if (m_MapTraffic)
	{
		delete [] m_MapTraffic;
		m_MapTraffic = NULL;
	}
	if(m_pPointTraffic)
	{
		delete [] m_pPointTraffic;
		m_pPointTraffic = NULL;
	}
	if(m_pContentTraffic)
	{
		delete [] m_pContentTraffic;
		m_pContentTraffic = NULL;
	}
	
}

// --
//initialization
// --
bool KScenePlaceMapC::Initialize()
{
	if (m_bInited == false && g_pRepresent)
	{
		int nCount = MAP_CELL_MAX_RANGE * MAP_CELL_MAX_RANGE;
		int	nIndex, i;
		MAP_CELL* pCell = &m_ElemsList[0][0];
		for (nIndex = i = 0; nIndex < nCount && i < 0x03335688; i++)
		{
			sprintf(pCell->szImageName, "*PlaceMap_%d*", i);
			pCell->uImageId = g_pRepresent->CreateImage(
				pCell->szImageName, MAP_CELL_MAP_WIDTH, MAP_CELL_MAP_HEIGHT, ISI_T_BITMAP16);
			if (pCell->uImageId)
			{
				pCell++;
				nIndex++;
			}
		}
		m_bInited = true;

		KIniFile	ColorSetting;
		if (ColorSetting.Load(MAP_SETTING_FILE))
		{
			ColorSetting.GetString("Map", "SelfColor", "", m_szSelfSpr, sizeof(m_szSelfSpr));
			ColorSetting.GetString("Map", "TeammateColor", "", m_szTeammateSpr, sizeof(m_szTeammateSpr));
			ColorSetting.GetString("Map", "PlayerColor", "", m_szPlayerSpr, sizeof(m_szPlayerSpr));
			ColorSetting.GetString("Map", "FightNpcColor", "", m_szFightNpcSpr, sizeof(m_szFightNpcSpr));
			ColorSetting.GetString("Map", "NormalNpcColor", "", m_szNormalNpcSpr, sizeof(m_szNormalNpcSpr));
			ColorSetting.GetString("Map", "TongColor", "", m_szTongSpr, sizeof(m_szTongSpr));
		}
	}

	return m_bInited;
}

// --
//
// --
void KScenePlaceMapC::Terminate()
{
	if (m_bInited && g_pRepresent)
	{
		for (int v = 0; v < MAP_CELL_MAX_RANGE; v++)
		{
			for (int h = 0; h < MAP_CELL_MAX_RANGE; h++)
			{
				if (m_ElemsList[v][h].uImageId)
				{
					g_pRepresent->FreeImage(m_ElemsList[v][h].szImageName);
					m_ElemsList[v][h].uImageId = 0;
				}
				m_ElemsList[v][h].szImageName[0] = 0;
			}
		}
	}
}

// --
//
// --
void KScenePlaceMapC::SetShowElemsFlag(unsigned int uShowElemsFlag)
{
	m_uMapShowElems = uShowElemsFlag;
	if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
		SetFocusPosition(m_FocusPosition.x, m_FocusPosition.y, true);
}
//read settings
BOOL KScenePlaceMapC::Load(KIniFile* pSetting, const char* pszScenePlaceRootPath)
{
	if (Initialize() == false)
		return FALSE;
	Free();
	m_bHavePicMap = false;
	BOOL m_bFlag = FALSE;
	if (pSetting && pszScenePlaceRootPath)
	{
		sprintf(m_szEntireMapFile, "%s"PLACE_MAP_FILE_NAME_APPEND, pszScenePlaceRootPath);
		m_bHavePicMap = g_FileExists(m_szEntireMapFile);
		if (m_bHavePicMap)
		{
			m_EntireMapLTPosition.x = -1;
			pSetting->GetInteger2(PLACE_MAP_SAVE_SECTION, "MapLTRegionIndex",
				(int*)&m_EntireMapLTPosition.x, (int*)m_EntireMapLTPosition.y);
			if (m_EntireMapLTPosition.x == -1)
			{
				RECT	rc;
				pSetting->GetRect(PLACE_MAP_SAVE_SECTION, "rect", &rc);
				m_EntireMapLTPosition.x = rc.left;
				m_EntireMapLTPosition.y = rc.top;
				// find way by kinnox;
				m_cLittleMap.Init(rc.left, rc.top, rc.right, rc.bottom);
				int		i, j;
				int		x, y;
				x = rc.left;
				y = rc.top;
				for (i = x; i < rc.right; i++)
				{
					for (j = y; j < rc.bottom; j++)
					{
						KRegion::LoadLittleMapData(i, j, SubWorld[0].m_szMapPath, m_cLittleMap.GetBarrierBuf(i, j));
						m_cLittleMap.SetHaveLoad(i, j);
					}
				}
				//
			}
			if (m_EntireMapLTPosition.x != -1)
			{
				m_EntireMapLTPosition.x *= KScenePlaceRegionC::RWPP_AREGION_WIDTH;
				m_EntireMapLTPosition.y *= KScenePlaceRegionC::RWPP_AREGION_HEIGHT;

				m_PicLoadedLTPosition.x = 0;
				m_PicLoadedLTPosition.y = 0;

				m_FocusLimit.left = m_EntireMapLTPosition.x + (m_Size.cx * MAP_SCALE_H / 2);
				m_FocusLimit.top  = m_EntireMapLTPosition.y + (m_Size.cy * MAP_SCALE_V / 2);
				m_FocusLimit.bottom = m_FocusLimit.right = RIGHT_BOTTOM_NO_LIMIT;

				m_bHavePicMap	= true;
				m_pPartnerPos	= new POINT[MAX_NUM_PARTNER];
				m_pFootSpot		= new KRUImage[MAX_NUM_CHARACTER];
				m_bFlag = TRUE;
			}
			else
			{
				m_bHavePicMap = false;
				m_bFlag = FALSE;
			}
		}
	}
	return m_bFlag;
}

//设置场景地图的焦点
void KScenePlaceMapC::SetFocusPosition(int nX, int nY, bool bChangedRegion)
{
	if (nX < m_FocusLimit.left)
		m_FocusPosition.x = m_FocusLimit.left;
	else if (nX > m_FocusLimit.right)
		m_FocusPosition.x = m_FocusLimit.right;
	else
		m_FocusPosition.x = nX;
	if (nY < m_FocusLimit.top)
		m_FocusPosition.y = m_FocusLimit.top;
	else if (nY > m_FocusLimit.bottom)
		m_FocusPosition.y = m_FocusLimit.bottom;
	else
		m_FocusPosition.y = nY;

	m_MapCoverArea.left = m_FocusPosition.x / MAP_SCALE_H - m_Size.cx / 2;
	m_MapCoverArea.top  = m_FocusPosition.y / MAP_SCALE_V - m_Size.cy / 2;
	m_MapCoverArea.right  = m_MapCoverArea.left + m_Size.cx;
	m_MapCoverArea.bottom = m_MapCoverArea.top  + m_Size.cy;

	if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
	{
		if (bChangedRegion && m_bHavePicMap)
		{
			POINT	CellIndex;
			CellIndex.x = (m_FocusPosition.x - m_PicLoadedLTPosition.x) / MAP_CELL_SCENE_WIDTH;
			CellIndex.y = (m_FocusPosition.y - m_PicLoadedLTPosition.y) / MAP_CELL_SCENE_HEIGHT;
			if (CellIndex.x != MAP_CELL_CENTRE_INDEX || //MAP_CELL_FOCUS_INDEX_MIN || CellIndex.x > MAP_CELL_FOCUS_INDEX_MAX ||
				CellIndex.y != MAP_CELL_CENTRE_INDEX)	//MAP_CELL_FOCUS_INDEX_MIN || CellIndex.y > MAP_CELL_FOCUS_INDEX_MAX)
			{
				m_PicLoadedLTPosition.x += (CellIndex.x - MAP_CELL_CENTRE_INDEX) * MAP_CELL_SCENE_WIDTH;
				m_PicLoadedLTPosition.y += (CellIndex.y - MAP_CELL_CENTRE_INDEX) * MAP_CELL_SCENE_HEIGHT;
				FillCellsPicInfo();
			}
		}
		CalcPicLayout();
	}
}

void KScenePlaceMapC::FillCellsPicInfo()
{
	if (m_pEntireMap == NULL)
	{
		if (m_bHavePicMap)
		{
			KBitmapDataBuffInfo	Info;
			short *pBuff = (short*)g_pRepresent->GetBitmapDataBuffer(
				m_ElemsList[0][0].szImageName, &Info);
			if (pBuff)
			{
				g_pRepresent->ReleaseBitmapDataBuffer(
						m_ElemsList[0][0].szImageName, pBuff);
				unsigned int uMask16 = -1;
				if (Info.eFormat == BDBF_16BIT_555)
					uMask16 = RGB_555;
				else if (Info.eFormat == BDBF_16BIT_565)
					uMask16 = RGB_565;
				if (uMask16 >= 0)
					m_pEntireMap = get_jpg_image(m_szEntireMapFile, uMask16);
			}			
		}
		if (m_pEntireMap)
		{
			m_FocusLimit.right = m_FocusLimit.left + (m_pEntireMap->nWidth - m_Size.cx)* MAP_SCALE_H;
			m_FocusLimit.bottom = m_FocusLimit.top + (m_pEntireMap->nHeight - m_Size.cy)* MAP_SCALE_V;
			if (m_FocusLimit.right < m_FocusLimit.left)
				m_FocusLimit.right = m_FocusLimit.left;
			if (m_FocusLimit.bottom < m_FocusLimit.top)
				m_FocusLimit.bottom = m_FocusLimit.top;
		}
		else
		{
			m_bHavePicMap = false;
			CoreDataChanged(GDCNI_REFRESH_MINIMAP, FALSE, 0);//fix minimaps by kinnox;
			return;
		}
	}

	int	nStartX = (m_PicLoadedLTPosition.x - m_EntireMapLTPosition.x) / MAP_SCALE_H;
	int nStartY = (m_PicLoadedLTPosition.y - m_EntireMapLTPosition.y) / MAP_SCALE_V;

	SIZE	PicEntireSize;
	PicEntireSize.cx = m_pEntireMap->nWidth;
	PicEntireSize.cy = m_pEntireMap->nHeight;
	POINT	DestPos;
	
	int	h, v, x, y, nFromX, nFromY, nToX, nToY;
	for (v = 0, y = nStartY; v < MAP_CELL_MAX_RANGE; v++, y += MAP_CELL_MAP_HEIGHT)
	{
		nToY = y + MAP_CELL_MAP_HEIGHT;
		bool bCleared = false;
		if (y < 0 || nToY > PicEntireSize.cy)
		{
			for (h = 0; h < MAP_CELL_MAX_RANGE; h++)
			{
				g_pRepresent->ClearImageData(m_ElemsList[v][h].szImageName,
					m_ElemsList[v][h].uImageId, m_ElemsList[v][h].sISPosition);
			}
			bCleared = true;
		}
		if (y < PicEntireSize.cy && nToY > 0)
		{
			if (y >= 0)
			{
				nFromY = y;
				DestPos.y = 0;
			}
			else
			{
				nFromY = 0;
				DestPos.y = -y;
			}

			if (nToY > PicEntireSize.cy)
				nToY = PicEntireSize.cy;

			for (h = 0, x = nStartX; h < MAP_CELL_MAX_RANGE; h++, x += MAP_CELL_MAP_WIDTH)
			{
				nToX = x + MAP_CELL_MAP_WIDTH;
				if (bCleared == false && (x < 0 || nToX > PicEntireSize.cx))
				{//横向没有全部落在图内
					g_pRepresent->ClearImageData(m_ElemsList[v][h].szImageName,
						m_ElemsList[v][h].uImageId, m_ElemsList[v][h].sISPosition);
				}

				if (x < PicEntireSize.cx && nToX > 0)
				{
					if (x >= 0)
					{
						nFromX = x;
						DestPos.x = 0;
					}
					else
					{
						nFromX = 0;
						DestPos.x = -x;
					}
					if (nToX > PicEntireSize.cx)
						nToX = PicEntireSize.cx;
					
					short *pBuf = (short*)g_pRepresent->GetBitmapDataBuffer(
						m_ElemsList[v][h].szImageName, NULL);
					if (pBuf)
					{
						short* pEntire = (short*)(&m_pEntireMap->Data) +
							nFromY * PicEntireSize.cx + nFromX;
						short* pDest = pBuf + MAP_CELL_MAP_WIDTH * DestPos.y + DestPos.x;
						for (int i = nFromY; i < nToY; i++)
						{
							memcpy(pDest, pEntire, (nToX - nFromX) * 2);
							pEntire += PicEntireSize.cx;
							pDest += MAP_CELL_MAP_WIDTH;
						}
						g_pRepresent->ReleaseBitmapDataBuffer(
							m_ElemsList[v][h].szImageName, pBuf);
					}
				}
			}
		}
	}
}

void KScenePlaceMapC::CalcPicLayout()
{
	POINT	ShowLTPos;

	ShowLTPos.x = m_MapCoverArea.left - m_PicLoadedLTPosition.x / MAP_SCALE_H ;
	ShowLTPos.y = m_MapCoverArea.top  - m_PicLoadedLTPosition.y / MAP_SCALE_V ;
//	ShowLTPos.x = (m_FocusPosition.x - m_PicLoadedLTPosition.x) / MAP_SCALE_H - m_Size.cx / 2;
//	ShowLTPos.y = (m_FocusPosition.y - m_PicLoadedLTPosition.y) / MAP_SCALE_V - m_Size.cy / 2;

	m_PaintCell.left = ShowLTPos.x / MAP_CELL_MAP_WIDTH;
	m_PaintCell.top = ShowLTPos.y / MAP_CELL_MAP_HEIGHT;

	m_FirstCellSkipWidth.cx = ShowLTPos.x - m_PaintCell.left * MAP_CELL_MAP_WIDTH;
	m_FirstCellSkipWidth.cy = ShowLTPos.y - m_PaintCell.top * MAP_CELL_MAP_HEIGHT;

	int nTemp = m_Size.cx + m_FirstCellSkipWidth.cx + MAP_CELL_MAP_WIDTH - 1;
	m_PaintCell.right = nTemp / MAP_CELL_MAP_WIDTH;
	m_LastCellSkipHeight.cx = nTemp - MAP_CELL_MAP_WIDTH * m_PaintCell.right;
	m_PaintCell.right += m_PaintCell.left;

	nTemp = m_Size.cy + m_FirstCellSkipWidth.cy + MAP_CELL_MAP_HEIGHT - 1;
	m_PaintCell.bottom = nTemp / MAP_CELL_MAP_HEIGHT;
	m_LastCellSkipHeight.cy = nTemp - MAP_CELL_MAP_HEIGHT * m_PaintCell.bottom;
	m_PaintCell.bottom += m_PaintCell.top;
}

// --
//
// --
void KScenePlaceMapC::Free()
{
	if (m_pEntireMap)
	{
		release_image(m_pEntireMap);
		m_pEntireMap = NULL;
	}
}

// --
//
// --
void KScenePlaceMapC::PaintMap(int nX, int nY)
{	
	m_MapPos.left = nX; m_MapPos.top = nY;//PaintFindPos by kinnox;
    m_MapPos.right = m_MapPos.left + m_Size.cx; m_MapPos.bottom = m_MapPos.top + m_Size.cy; //PaintFindPos by kinnox;	
	if (m_bHavePicMap && g_pRepresent)
	{	
		//----draw thumbnail----
		if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
			PaintMapPic(nX, nY);
		PaintMapPointNpc(nX, nY);
		//---Draw teammate positions----
		PaintCharacters(nX, nY);
		//---draw yourself position----
		int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (nNpcIdx >= 0 && Npc[nNpcIdx].m_RegionIndex >= 0)
		{
			int nNpcX = LOWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_H +
				Npc[nNpcIdx].m_MapX * 2;
			int nNpcY = HIWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_V +
				Npc[nNpcIdx].m_MapY;

			if (nNpcX >= m_MapCoverArea.left && nNpcX < m_MapCoverArea.right &&
				nNpcY >= m_MapCoverArea.top  && nNpcY < m_MapCoverArea.bottom)
			{
				KRUImage	FootSpot;
				FootSpot.nType = ISI_T_SPR;
				FootSpot.Color.Color_b.a = 255;
				FootSpot.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				FootSpot.nISPosition = IMAGE_IS_POSITION_INIT;
				FootSpot.bRenderFlag = 0;
				FootSpot.nFrame = 0;
				FootSpot.uImage = 0;
				strcpy(FootSpot.szImage, m_szSelfSpr);
				FootSpot.oPosition.nZ = FootSpot.oEndPos.nZ = 0;
				FootSpot.oPosition.nX = nX + nNpcX - m_MapCoverArea.left - 4;
				FootSpot.oPosition.nY = nY + nNpcY - m_MapCoverArea.top  - 4;
				FootSpot.oPosition.nZ = FootSpot.oEndPos.nZ = 0;
				g_pRepresent->DrawPrimitives(1, &FootSpot, RU_T_IMAGE, true);
			}
			//PaintFindPos by kinnox;
				PaintFindPos(nX + m_FlagPos.x - m_MapCoverArea.left - 1,
                        nY + m_FlagPos.y - m_MapCoverArea.top  - 1,
                        nX + nNpcX - m_MapCoverArea.left - 2,
                        nY + nNpcY - m_MapCoverArea.top  - 2);
			
			
			if (g_ScenePlace.nPaintFindPos)
			{	
				//----draw auto pathing----
				if (nNpcX >= m_MapCoverArea.left && nNpcX < m_MapCoverArea.right &&
					nNpcY >= m_MapCoverArea.top  && nNpcY < m_MapCoverArea.bottom && m_bLine)
				{
				if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
					PaintAutoPath(nX, nY); // find way by kinnox;
				}

			}
			else
			{
				for (int i = 0; i < Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.size(); ++i) 
				{
					if (i % 3 == 0) 
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
					}
				}
			}

		}
		
	}
}

// find way by kinnox;
void KScenePlaceMapC::PaintAutoPath(int nX, int nY) 
{
	for (int i = 0; i < Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.size(); ++i) 
	{
		if (i % 3 == 0) 
		{ // Check if the index is a multiple of 3
			const FindPathNode& node = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind[i];
			
			//Images
			/*int nDesX = nX + node.x / 16 - m_MapCoverArea.left - 1;
			int nDesY = nY + node.y / 32 - m_MapCoverArea.top - 12;*/
			
			//Rect
			int nDesX = nX + node.x / 16 - m_MapCoverArea.left - 1;
			int nDesY = nY + node.y / 32 - m_MapCoverArea.top - 1;

		 	if (nDesX < m_MapPos.left)
            {
                    nDesX = m_MapPos.left;
            }
            if (nDesY < m_MapPos.top)
            {
                    nDesY = m_MapPos.top;
            }
            if (nDesX > m_MapPos.right)
            {
                    nDesX = m_MapPos.right;
            }
            if (nDesY > m_MapPos.bottom)
            {
                    nDesY = m_MapPos.bottom;
            }

            /*KRUImage Image;
            Image.nType = ISI_T_SPR;
            Image.Color.Color_b.a = 255;
            Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
            Image.uImage = 0;
            Image.nFrame = 0;
            Image.nISPosition = IMAGE_IS_POSITION_INIT;
            Image.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
            SYSTEMTIME systm;
			GetLocalTime(&systm);
           	strcpy(Image.szImage, (systm.wSecond % 2) ? PLACE_MAP_FOLLOW_FILE_PATH : PLACE_MAP_FOLLOW_FILE_PATH_1);
	        Image.oPosition.nX =  nDesX;
	        Image.oPosition.nY =  nDesY;
	        Image.oPosition.nZ = 0;
	        g_pRepresent->DrawPrimitives(1, &Image, RU_T_IMAGE, 1);*/

            unsigned int	Color_dws,Color_dws1;
            Color_dws = 0xff0bd92d;
            Color_dws1 = 0x6aff30;

	        KRUShadow FootSpot;
	        SYSTEMTIME systm;
			GetLocalTime(&systm);
           	(systm.wSecond % 2) ? Color_dws = 0xffff00 : Color_dws = 0x6aff30;
			FootSpot.Color.Color_dw = Color_dws;


			FootSpot.oPosition.nX = nDesX;
			FootSpot.oPosition.nY = nDesY;
			FootSpot.oEndPos.nX = FootSpot.oPosition.nX + 2;
			FootSpot.oEndPos.nY = FootSpot.oPosition.nY + 2;
			g_pRepresent->DrawPrimitives(1, &FootSpot, RU_T_SHADOW, true);

            // Debug information
			// g_DebugLog("FootSpot.oPosition.nX: %d, FootSpot.oPosition.nY: %d\n\
	        //                    nX: %d, nY: %d\n\
	        //                    node.x: %d, node.y: %d\n", FootSpot.oPosition.nX, FootSpot.oPosition.nY, nX, nY, node.x / 16, node.y / 32);
		}
 	}
}
//
// --
//
// --
void KScenePlaceMapC::PaintCharacters(int nX, int nY)
{	
	bool bCharacters = (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_CHARACTER) != 0;
	bool bPartners = (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PARTNER) != 0;
	if (bCharacters == false || bPartners == false)
		return;

	if(!m_pPartnerPos)
		return;
	if(!m_pFootSpot)
		return;
	BOOL		nIsInTeam		= Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag;
	BYTE		btNumPartner	= 0;
	BYTE		btNumSpot		= 0;

	int nNpcIdx = 0;
	while (nNpcIdx = NpcSet.GetNextIdx(nNpcIdx))
	{
		if (Npc[nNpcIdx].m_RegionIndex == -1)
			continue;
		
		int nNpcX = LOWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_H +
						Npc[nNpcIdx].m_MapX * 2;
		int nNpcY = HIWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_V +
						Npc[nNpcIdx].m_MapY;
		if (nNpcX < m_MapCoverArea.left || nNpcX >= m_MapCoverArea.right ||
			nNpcY < m_MapCoverArea.top  || nNpcY >= m_MapCoverArea.bottom)
		{
			continue;
		}

		bool bValidNpc = false; 
		BYTE PointType;

		if (Npc[nNpcIdx].m_Kind == kind_normal && bCharacters)
		{
			bValidNpc = true;
			//strcpy(szSprColor, m_szFightNpcSpr);
			PointType = 4;
		}
		else if (Npc[nNpcIdx].m_Kind == kind_dialoger && bCharacters)
		{
			bValidNpc = true;
			//strcpy(szSprColor, m_szNormalNpcSpr);
			PointType = 5;
		}
		else if (Npc[nNpcIdx].m_Kind == kind_player && nNpcIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
		{
			if (bPartners &&
				nIsInTeam &&
				((DWORD)g_Team[0].m_nCaptain == Npc[nNpcIdx].m_dwID ||
					g_Team[0].FindMemberID(Npc[nNpcIdx].m_dwID) >= 0) &&
				btNumPartner < MAX_NUM_PARTNER)	
			{
				m_pPartnerPos[btNumPartner].x = nNpcX;
				m_pPartnerPos[btNumPartner].y = nNpcY;
				btNumPartner++;
			}
			else if (bCharacters)
			{	
				//
				if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_BATTLE_GROUP) > 0)
				{
					if(Npc[nNpcIdx].m_CurrentCamp == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp)
					{
						bValidNpc = true;
						PointType = 2;
					}
					else
					{
						bValidNpc = true;
						PointType = 3;
					}
				}
				else if((Npc[nNpcIdx].m_szTongName[0] || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szTongName[0]) && 
						!strcmp(Npc[nNpcIdx].m_szTongName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szTongName))
				{
						bValidNpc = true;
						PointType = 6;
				}
				else
				{
					bValidNpc = true;
					PointType = 3;
				}
				
			
				//
				if(Npc[nNpcIdx].m_HideState.nValue[0] < 0 && Npc[nNpcIdx].m_HideState.nTime > 0)
				{
					bValidNpc = false;
				}
				else if(NpcSet.GetRelation(nNpcIdx,Player[CLIENT_PLAYER_INDEX].m_nIndex) == relation_enemy && Npc[nNpcIdx].m_HideState.nTime > 0)
				{
					bValidNpc = false;
				}
				
			}
		}
		if (bValidNpc)
		{	
			m_pFootSpot[btNumSpot].nType = ISI_T_SPR;
			m_pFootSpot[btNumSpot].Color.Color_b.a = 255;
			m_pFootSpot[btNumSpot].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_pFootSpot[btNumSpot].nISPosition = IMAGE_IS_POSITION_INIT;
			m_pFootSpot[btNumSpot].bRenderFlag = 0;
			m_pFootSpot[btNumSpot].nFrame = 0;
			m_pFootSpot[btNumSpot].uImage = 0;
			switch(PointType)
			{
			case 2:
				strcpy(m_pFootSpot[btNumSpot].szImage, m_szTeammateSpr);
				break;
			case 3:
				strcpy(m_pFootSpot[btNumSpot].szImage, m_szPlayerSpr);
				break;
			case 4:
				strcpy(m_pFootSpot[btNumSpot].szImage, m_szFightNpcSpr);
				break;
			case 5:
				strcpy(m_pFootSpot[btNumSpot].szImage, m_szNormalNpcSpr);
				break;
			case 6:
				strcpy(m_pFootSpot[btNumSpot].szImage, m_szTongSpr);
				break;
			}
			m_pFootSpot[btNumSpot].oPosition.nZ = m_pFootSpot[btNumSpot].oEndPos.nZ = 0;
			m_pFootSpot[btNumSpot].oPosition.nX = nX + nNpcX - m_MapCoverArea.left - 4;
			m_pFootSpot[btNumSpot].oPosition.nY = nY + nNpcY - m_MapCoverArea.top  - 4;
			btNumSpot++;
			if (btNumSpot == MAX_NUM_CHARACTER)
			{
				g_pRepresent->DrawPrimitives(MAX_NUM_CHARACTER, &m_pFootSpot[0], RU_T_IMAGE, true);
				btNumSpot = 0;
			}


		}
	}

	if (btNumSpot)
	{
		g_pRepresent->DrawPrimitives(btNumSpot, &m_pFootSpot[0], RU_T_IMAGE, true);
		btNumSpot = 0;
	}
	if (btNumPartner)
	{
		_ASSERT(btNumPartner <= MAX_NUM_CHARACTER);
		for (int i = 0; i < btNumPartner; i++)
		{	
			m_pFootSpot[i].nType = ISI_T_SPR;
			m_pFootSpot[i].Color.Color_b.a = 255;
			m_pFootSpot[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_pFootSpot[i].nISPosition = IMAGE_IS_POSITION_INIT;
			m_pFootSpot[i].bRenderFlag = 0;
			m_pFootSpot[i].nFrame = 0;
			m_pFootSpot[i].uImage = 0;
			strcpy(m_pFootSpot[i].szImage, m_szTeammateSpr);
			m_pFootSpot[i].oPosition.nX = nX + m_pPartnerPos[i].x - m_MapCoverArea.left - 4;
			m_pFootSpot[i].oPosition.nY = nY + m_pPartnerPos[i].y - m_MapCoverArea.top  - 4;
			m_pFootSpot[i].oPosition.nZ = m_pFootSpot[i].oEndPos.nZ = 0;
		}
		g_pRepresent->DrawPrimitives(i, &m_pFootSpot[0], RU_T_IMAGE, true);
	}
}


// --
//
// --
void KScenePlaceMapC::PaintMapPic(int nX, int nY)
{
	//_ASSERT(g_pRepresent);

	KRUImagePart	Img;
	Img.bRenderFlag = 0;
	Img.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
	Img.Color.Color_dw = 0;
	Img.nFrame = 0;
	Img.nType = ISI_T_BITMAP16;
	Img.oPosition.nY = nY;

	for (int v = m_PaintCell.top; v < m_PaintCell.bottom; v++)
	{
		if (v != m_PaintCell.top)
			Img.oImgLTPos.nY = 0;
		else
			Img.oImgLTPos.nY = m_FirstCellSkipWidth.cy;
		if (v != m_PaintCell.bottom - 1)
			Img.oImgRBPos.nY = MAP_CELL_MAP_HEIGHT;
		else
			Img.oImgRBPos.nY = m_LastCellSkipHeight.cy;

		Img.oPosition.nX = nX;
		for (int h = m_PaintCell.left; h < m_PaintCell.right; h++)
		{
			if (h != m_PaintCell.left)
				Img.oImgLTPos.nX = 0;
			else
				Img.oImgLTPos.nX = m_FirstCellSkipWidth.cx;
			if (h != m_PaintCell.right - 1)
				Img.oImgRBPos.nX = MAP_CELL_MAP_WIDTH;
			else
				Img.oImgRBPos.nX = m_LastCellSkipHeight.cx;

			Img.nISPosition = m_ElemsList[v][h].sISPosition;
			strcpy(Img.szImage, m_ElemsList[v][h].szImageName);
			Img.uImage = m_ElemsList[v][h].uImageId;
			g_pRepresent->DrawPrimitives(1, &Img, RU_T_IMAGE_PART, true);
			m_ElemsList[v][h].sISPosition = Img.nISPosition;
			m_ElemsList[v][h].uImageId = Img.uImage;

			Img.oPosition.nX += Img.oImgRBPos.nX - Img.oImgLTPos.nX;
		}
		Img.oPosition.nY += Img.oImgRBPos.nY - Img.oImgLTPos.nY;
	};
}

//设置小地图的大小（单位：像素点）
void KScenePlaceMapC::SetSize(int cx, int cy)
{
	if (m_Size.cx != cx || m_Size.cy != cy)
	{
		if (cx > MAP_MAX_SUPPORT_WIDTH)
			cx = MAP_MAX_SUPPORT_WIDTH;
		else if (cx < 0)
			cx = 0;
		if (cy > MAP_MAX_SUPPORT_HEIGHT)
			cy = MAP_MAX_SUPPORT_HEIGHT;
		else if (cy < 0)
			cy = 0;
		int nDLimit = (cx - m_Size.cx) * MAP_SCALE_H / 2;
		m_FocusLimit.left += nDLimit;
		if (m_FocusLimit.right != RIGHT_BOTTOM_NO_LIMIT)
		{
			m_FocusLimit.right -= nDLimit;
			if (m_FocusLimit.right < m_FocusLimit.left)
				m_FocusLimit.right = m_FocusLimit.left;
		}
		nDLimit = (cy - m_Size.cy) * MAP_SCALE_V / 2;
		m_FocusLimit.top += nDLimit;
		if (m_FocusLimit.bottom != RIGHT_BOTTOM_NO_LIMIT)
		{
			m_FocusLimit.bottom -= nDLimit;
			if (m_FocusLimit.bottom < m_FocusLimit.top)
				m_FocusLimit.bottom = m_FocusLimit.top;
		}

		m_Size.cx = cx;
		m_Size.cy = cy;
		SetFocusPosition(m_FocusPosition.x, m_FocusPosition.y, true);
	}
}




// --
//
// --
int KScenePlaceMapC::GetMapRect(RECT* pRect)
{
	if (pRect)
	{
		pRect->left = m_EntireMapLTPosition.x;
		pRect->top  = m_EntireMapLTPosition.y;
		if (m_pEntireMap)
		{
			pRect->right = m_EntireMapLTPosition.x + m_pEntireMap->nWidth * MAP_SCALE_H;
			pRect->bottom = m_EntireMapLTPosition.y + m_pEntireMap->nHeight * MAP_SCALE_V;
		}
		else
		{
			pRect->right = m_EntireMapLTPosition.x;
			pRect->bottom = m_EntireMapLTPosition.y;
		}
	}
	return m_bHavePicMap;
}


// --
//PaintFindPos by kinnox;
// --
void KScenePlaceMapC::PaintFindPos(int nX, int nY, int nNpcPosX, int nNpcPosY)
{
        if (!g_ScenePlace.nPaintFindPos)
                return;
        int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
        int nNpcX = LOWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_H + Npc[nNpcIdx].m_MapX * 2;
        int nNpcY = HIWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_V + Npc[nNpcIdx].m_MapY;
        BOOL bPaintFlag = TRUE;
        if (m_bLine &&
                m_FlagPos.x - nNpcX > -PLACE_MAP_SYMBOL_SIZE &&
                m_FlagPos.x - nNpcX < PLACE_MAP_SYMBOL_SIZE &&
                m_FlagPos.y - nNpcY > -PLACE_MAP_SYMBOL_SIZE &&
                m_FlagPos.y - nNpcY < PLACE_MAP_SYMBOL_SIZE)
                g_ScenePlace.nPaintFindPos = FALSE;

        if (nNpcIdx && Npc[nNpcIdx].m_RegionIndex >= 0)
        {
                if (nX < m_MapPos.left)
                {
                        nX = m_MapPos.left;
                        bPaintFlag = FALSE;
                }
                if (nY < m_MapPos.top)
                {
                        nY = m_MapPos.top;
                        bPaintFlag = FALSE;
                }
                if (nX > m_MapPos.right)
                {
                        nX = m_MapPos.right;
                        bPaintFlag = FALSE;
                }
                if (nY > m_MapPos.bottom)
                {
                        nY = m_MapPos.bottom;
                        bPaintFlag = FALSE;
                }
                if (nNpcPosX < m_MapPos.left)
                        nNpcPosX = m_MapPos.left;
                if (nNpcPosY < m_MapPos.top)
                        nNpcPosY = m_MapPos.top;
                if (nNpcPosX > m_MapPos.right)
                        nNpcPosX = m_MapPos.right;
                if (nNpcPosY > m_MapPos.bottom)
                        nNpcPosY = m_MapPos.bottom;
                                
                if (bPaintFlag)
                {
                      
                        KRUImage Image;
                        Image.nType = ISI_T_SPR;
                        Image.Color.Color_b.a = 255;
                        Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
                        Image.uImage = 0;
                        Image.nFrame = 0;
                        Image.nISPosition = IMAGE_IS_POSITION_INIT;
                        Image.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
                        strcpy(Image.szImage, PLACE_MAP_FLAG_FILE_PATH);
                        Image.oPosition.nX = nX;
                        Image.oPosition.nY = nY - 12;
                        Image.oPosition.nZ = 0;
                        g_pRepresent->DrawPrimitives(1, &Image, RU_T_IMAGE, 1);
                    	
                }

                if (m_bLine)
                {
                        KRULine Line;
                        Line.oPosition.nX = nX;
                        Line.oPosition.nY = nY;
                        Line.Color.Color_dw = 0xffff0e0e;
                        Line.oPosition.nZ = Line.oEndPos.nZ = 0;
                        Line.oEndPos.nX = nNpcPosX - 2;
                        Line.oEndPos.nY = nNpcPosY - 2;
                       // g_pRepresent->DrawPrimitives(1, &Line, RU_T_LINE, 1);
                        char MapPos[80];
                        DWORD dwColor;
						FindPathNode current, target;
						INT nCurX, nCurY;
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nCurX, &nCurY);
						current.x = nCurX;// Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffX + m_EntireMapLTPosition.x;// / 16 - m_MapPos.left + m_MapCoverArea.left + 1;
						current.y = nCurY;// Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffY + m_EntireMapLTPosition.y;// / 32 - m_MapPos.top + m_MapCoverArea.top + 1;
						target.x = m_FlagPos.x * 16;
						target.y = m_FlagPos.y * 32;
						if (g_JXPathFinder.FindPath(current.x, current.y, target.x, target.y))
						{	
							dwColor = 0xffff00;
                       		sprintf(MapPos,"%d,%d", m_FlagPos.x/16, m_FlagPos.y/16);
                    	}
                    	else
                    	{
                    		dwColor = 0xfffe1200;
                    		sprintf(MapPos,"Vn v藅 c秐");
                    	}

                        BYTE nFontSize = 12;
                        BYTE nFontWidth = nFontSize / 2;
                        int nNamePosLeftB = Line.oPosition.nX;
                        int nNamePosLeftE = Line.oPosition.nX + (g_StrLen(MapPos) * nFontWidth) + nFontWidth;;
                        int nNamePosY = Line.oPosition.nY - 5;
                        if (nNamePosLeftE > m_MapPos.right)
                                nNamePosLeftB -= nNamePosLeftE - m_MapPos.right;
                        if (nNamePosY - 15 < m_MapPos.top)
                                nNamePosY = m_MapPos.top + 15;
                        if (nNamePosY + nFontSize > m_MapPos.bottom)
                                nNamePosY -= nNamePosY + nFontSize - m_MapPos.bottom;
                        g_pRepresent->OutputText(nFontSize, MapPos, KRF_ZERO_END, nNamePosLeftB, nNamePosY, dwColor);                                                  
                }

        }
}
// --
//
// --
void KScenePlaceMapC::CalcFlagPos(int nX, int nY, bool bCalc, bool bLine)
{
        if(nX == 0 || nY == 0)
        {
                g_ScenePlace.nPaintFindPos = FALSE;
                return;
        }
        int nDesX, nDesY;
        if (bCalc)
        {
                nDesX = nX - m_MapPos.left + m_MapCoverArea.left + 1;
                nDesY = nY - m_MapPos.top + m_MapCoverArea.top + 1;
                if ( nDesX < m_MapCoverArea.left ||
                        nDesY < m_MapCoverArea.top ||
                        nDesX > m_MapCoverArea.right ||
                        nDesY > m_MapCoverArea.bottom)
                {
                        return;
                }
        }
        else
        {
                nDesX = nX * MAP_SCALE_H + 8;
                nDesY = nY * MAP_SCALE_H + 8;
        }
        m_FlagPos.x = nDesX;
        m_FlagPos.y = nDesY;  
        m_bLine = bLine;

        if(m_bLine)// find way by kinnox;
       	{
       		AutoRunTo(nDesX, nDesY);   
       	}
}
// --
//
// --

// load yellow point npc
void KScenePlaceMapC::PaintMapPointNpc(int nX, int nY)
{
	KIniFile	Ini;
	if (!Ini.Load(WORLD_MAP_TRAFFIC_FILE))
		return;

	int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	int nSubWorldID = SubWorld[Npc[nNpcIdx].m_SubWorldIndex].m_SubWorldID;
	int	nNamePosX, nNamePosY, nSprPosX, nSprPosY;
	
	char szBuffer[32], szBuffer1[32], szBuffer2[32], szMapId[32], szBuffer3[32], szBuffer4[32], szBuffer5[32];			
	int nCount;
	
	sprintf(szMapId, "[%d]", nSubWorldID);	
	Ini.GetInteger(szMapId, "Count", 0, &nCount);			
	for(int i = -1; i < nCount; i++)
	{
		sprintf(szBuffer3, "%d_SprPos", i);
		Ini.GetInteger2(szMapId, szBuffer3, &nSprPosX, &nSprPosY);			
		sprintf(szBuffer, "%d_NamePos", i);
		Ini.GetInteger2(szMapId, szBuffer, &nNamePosX, &nNamePosY);
		sprintf(szBuffer1, "%d_NpcName", i);
		Ini.GetString(szMapId, szBuffer1,"", szBuffer2, sizeof(szBuffer2));			
		sprintf(szBuffer4, "%d_SprLink", i);
		Ini.GetString(szMapId, szBuffer4,"", szBuffer5, sizeof(szBuffer5));		
		while (nNpcIdx = NpcSet.GetNextIdx(nNpcIdx))
		{
			if (Npc[nNpcIdx].m_RegionIndex == -1)
				continue;
			KRUImage	RUSymbol;
			RUSymbol.nType = ISI_T_SPR;
			RUSymbol.Color.Color_b.a = 255;
			RUSymbol.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			RUSymbol.uImage = 0;
			RUSymbol.nISPosition = IMAGE_IS_POSITION_INIT;
			RUSymbol.bRenderFlag = 0;
			RUSymbol.nFrame = 0;
			RUSymbol.oPosition.nX = nX + nSprPosX * 16 - m_MapCoverArea.left + 10;
			RUSymbol.oPosition.nY = nY + nSprPosY * 16 - m_MapCoverArea.top + 2;
			RUSymbol.oPosition.nZ = 1;
			strcpy(RUSymbol.szImage, szBuffer5);
					
			int PosSymbolLeft, PosSymbolRight, PosSymbolBottom;
			
			PosSymbolLeft = nSprPosX * 16 - (g_StrLen(szBuffer2) * 2) + nNamePosX;
			PosSymbolRight = nSprPosX * 16 + nNamePosX;
			PosSymbolBottom = nSprPosY * 16 + 6 - nNamePosY;
			
			
			if (PosSymbolLeft < m_MapCoverArea.left || 
				PosSymbolRight >= m_MapCoverArea.right || 
				PosSymbolBottom >= m_MapCoverArea.bottom)
				continue;					
			g_pRepresent->DrawPrimitives(1, &RUSymbol, RU_T_IMAGE, true);
			g_pRepresent->OutputText(12, szBuffer2, KRF_ZERO_END, RUSymbol.oPosition.nX - 
				(g_StrLen(szBuffer2) * 2), RUSymbol.oPosition.nY - 15, 0xffffff00);
		}
	}
	Ini.Clear();
}

// find way by kinnox;
BOOL KScenePlaceMapC::AutoRunTo(int nX, int nY)
 {
	FindPathNode current, target;
	INT nCurX, nCurY;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nCurX, &nCurY);
	current.x = nCurX;// Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffX + m_EntireMapLTPosition.x;// / 16 - m_MapPos.left + m_MapCoverArea.left + 1;
	current.y = nCurY;// Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffY + m_EntireMapLTPosition.y;// / 32 - m_MapPos.top + m_MapCoverArea.top + 1;
	target.x = nX * 16;
	target.y = nY * 32;

	//g_DebugLog("Current %d, %d - Target %d, %d\n", current.x, current.y, target.x, target.y);

	if (g_JXPathFinder.FindPath(current.x, current.y, target.x, target.y)) {
		//g_DebugLog("JXPathFinder found path!\n");
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.clear();
		//g_DebugLog("g_JXPathFinder.clear");
	}
	else {
		//g_DebugLog("JXPathFinder 404!\n");
		return false;
	}
	g_JXPathFinder.GetPath(current.x, current.y, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind);
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.insert(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin(), current);
	//g_DebugLog("g_JXPathFinder.m_PathFind.insert");
	/*for (const auto& node : Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind) 
	{
		//g_DebugLog("x: %d, y: %d\n", node.x, node.y);
	}*/
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
	//g_DebugLog("g_JXPathFinder.m_PathFind.erase");
 	return true;
 }

BYTE* KScenePlaceMapC::GetbtBarrier(int X, int Y)
{
	return m_cLittleMap.GetBarrierBuf(X, Y);
}

BOOL KScenePlaceMapC::JxPathFinderClear()
{

		if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
		{	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
			m_bLine = FALSE;
			g_ScenePlace.nPaintFindPos = FALSE;
			return TRUE;
		}

	return FALSE;
}

//