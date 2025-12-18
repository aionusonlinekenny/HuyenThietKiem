#pragma once
#include "../Elem/WndButton.h"
#include "../Elem/WndShadow.h"
#include "../Elem/WndPureTextBtn.h"
#include "../../Core/Src/GameDataDef.h"
#include "../../../Core/KJXPathFinder.h"
enum MINIMAP_MODE
{
	MINIMAP_M_BRIEF_NOT_PIC = -1,	
	MINIMAP_M_BRIEF_PIC = 0,			
	MINIMAP_M_BRIEF_PIC_BROWSE,
	MINIMAP_M_WORLD_MAP,
	MINIMAP_M_CAVE_MAP,
				
	MINIMAP_M_NONE,

	MINIMAP_M_COUNT,
};

struct KUiSceneTimeInfo;

class KUiMiniMap : public KWndImage
{
public:
	static KUiMiniMap*	OpenWindow();					
	static KUiMiniMap*	GetIfVisible();					
	static void			CloseWindow();
	static bool			IsHavePicMap();

	static void			SetMode(MINIMAP_MODE eMode);
	static void			LoadScheme(const char* pScheme);
	static void			UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo);
	static BOOL 		bCheckFlag;
	static KUiMiniMap*	GetSelf() {return ms_pSelf;}

	void	Show();										
	void	Hide();
	
private:
	KUiMiniMap();
	~KUiMiniMap() {}
	void	Initialize();
	void	PaintWindow();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadScheme(KIniFile* pIni);
	void	SetStatus(bool bSmall);
	void    MapScroll(int nbScrollScene);
	void	StopScrollMap();
	void	MapMoveBack();
	virtual void	Breathe();
public:
	static void	SetValueFindPos(int nMpsX, int nMpsY);
private:
	static KUiMiniMap*	ms_pSelf;

private:
	unsigned int	m_uShowElem;
	POINT			m_MapPos;
	SIZE			m_MapSize;
	KWndButton		m_SwitchBtn;
	KWndText32		m_SceneName;
	KWndPureTextBtn		m_ScenePos;
	KWndShadow		m_Shadow;
	KWndButton		m_BtnFlag;
	KWndButton		m_Market;
	KWndButton		m_WorldMapBtn;
	KWndButton		m_CaveMapBtn;
	//
	KWndText32		m_TongName;
	KWndText32		m_TongMaster;
	KWndText32		m_ExchangeTax;
	KWndText32		m_PriceParam;

	POINT           m_OldPos;
	unsigned int	m_uLastScrollTime;						  
	short           m_nOffsetX;
	short           m_nOffsetY;
	int				m_nIsStopUpdate;
	int				nPosXCurrent;
	int				nPosYCurrent;
	BOOL			m_bCalcPosMode;
	bool			m_bFlagged;
	char			m_szFlagImage[80];

	bool			m_bHavePicMap;
};

void	MapToggleStatus();
void	MapSetMode(MINIMAP_MODE eMode);
MINIMAP_MODE MapGetMode();