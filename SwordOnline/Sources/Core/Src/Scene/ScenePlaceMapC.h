// ***************************************************************************************
// 场景地图的小地图
// Copyright : Kingsoft 2003
// Author    : wooy(wu yue)
// CreateTime: 2003-7-8
// // ---
// 考虑效率因素，决定障碍小地图显示的范围要包含在场景地图架载的范围之中。场景模块架载地图的范围是
// 7*7=49个区域，障碍小地图的最大显示范围对应为6*6=36个区域。当小地图的缩小比率为16分之一的情况
// 下，则对应的小地图显示图形的大小就限定在不大于192*192像素。
// ***************************************************************************************
#include "../../Represent/iRepresent/iRepresentShell.h"

#ifndef _SERVER
#include "../KLittleMap.h" // find way by kinnox;
#endif

#pragma once
class KScenePlaceMapC
{
public:
	enum SCENE_PLACE_MAP_WORK_PARAM_PUB
	{
        MAP_SCALE_H = 16,
		MAP_SCALE_V = 32,
	};
public:
	KScenePlaceMapC();
	~KScenePlaceMapC();
	void	Terminate();

	BOOL Load(KIniFile* pSetting, const char* pszScenePlaceRootPath);

	void Free();

	void SetFocusPosition(int nX, int nY, bool bChangedRegion);
	void SetShowElemsFlag(unsigned int uShowElemsFlag);
	void SetSize(int cx, int cy);
	void PaintMap(int nx, int ny);
	int	GetMapRect(RECT* pRect);
	//PaintFindPos by kinnox;
	void PaintFindPos(int nX, int nY, int nNpcPosX, int nNpcPosY);
    void CalcFlagPos(int nX, int nY, bool bCalc, bool bLine);
    POINT        m_FlagPos;
    BOOL AutoRunTo(int nX, int nY);// find way by kinnox;
    BYTE* GetbtBarrier(int X, int Y);// find way by kinnox;
    BOOL  JxPathFinderClear();// find way by kinnox;
	//end
#ifndef _SERVER
	KLittleMap 		m_cLittleMap;
#endif

private:
	bool	Initialize();
	void	FillCellsPicInfo();
	void	PaintMapPic(int nX, int nY);
	void	PaintCharacters(int nX, int nY);
	void	PaintAutoPath(int nX, int nY);// find way by kinnox;
	void	PaintMapPointNpc(int nX, int nY);
	void	CalcPicLayout();
private:
	enum SCENE_PLACE_MAP_WORK_PARAM
	{
		MAP_CELL_MAP_WIDTH = 128,
		MAP_CELL_MAP_HEIGHT = 128,
		MAP_CELL_SCENE_WIDTH = 2048,
		MAP_CELL_SCENE_HEIGHT = 4096,
		MAP_CELL_NUM_REGION_H = 4,		
		MAP_CELL_NUM_REGION_V = 4,		

 		MAP_CELL_MAX_RANGE = 5,			
		MAP_CELL_CENTRE_INDEX = 2,

//		MAP_CELL_FOCUS_INDEX_MIN = 2,	
//		MAP_CELL_FOCUS_INDEX_MAX = 2,	

		MAP_MAX_SUPPORT_WIDTH  = (MAP_CELL_MAX_RANGE - 1) * MAP_CELL_MAP_WIDTH,		
		MAP_MAX_SUPPORT_HEIGHT = (MAP_CELL_MAX_RANGE - 1) * MAP_CELL_MAP_HEIGHT,	

		MAP_A_REGION_NUM_MAP_PIXEL_H = 32,	
		MAP_A_REGION_NUM_MAP_PIXEL_V = 32,	
	};

	struct	MAP_CELL
	{
		 char			szImageName[32];
		 unsigned int	uImageId;
		 short			sISPosition;
		 short			sReserved;
	};

	struct MAP_TRAFFIC
	{
		BYTE	btType;
		short	nPointX;
		short	nPointY;
		char	szContent[80];

	};


private:
	int		m_bHavePicMap;
	bool	m_bInited;
	int		nFindDesX;					
	int		nFindDesY;
	#ifndef _SERVER
	//PaintFindPos by kinnox;
	RECT        m_MapPos;
    bool        m_bLine;
	//
	char	m_szEntireMapFile[128];
	KSGImageContent*	m_pEntireMap;
	#endif
	POINT	m_EntireMapLTPosition;

	POINT	m_FocusPosition;
	POINT	m_PicLoadedLTPosition;
	RECT	m_FocusLimit;

	SIZE			m_Size;
	unsigned int	m_uMapShowElems;
	MAP_CELL		m_ElemsList[MAP_CELL_MAX_RANGE][MAP_CELL_MAX_RANGE];

	RECT		m_MapCoverArea;
	RECT		m_PaintCell;
	SIZE		m_FirstCellSkipWidth;
	SIZE		m_LastCellSkipHeight;
	
	POINT*				m_pPartnerPos;
	KRUImage*			m_pFootSpot;

	short				m_nLineDestX;
	short				m_nLineDestY;
	short				m_nCountTraffic;
	MAP_TRAFFIC*		m_MapTraffic;
	KRUImage*			m_pPointTraffic;
	KOutputTextParam*	m_pContentTraffic;

	char			m_szSelfSpr[80];
	char			m_szTeammateSpr[80];
	char			m_szPlayerSpr[80];
	char			m_szFightNpcSpr[80];
	char			m_szNormalNpcSpr[80];
	char			m_szTongSpr[80];
};
