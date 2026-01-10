#ifndef	KWorldH
#define	KWorldH

#ifdef _SERVER
#define	MAX_SUBWORLD	62
#else
#define	MAX_SUBWORLD	1
#endif
#define	VOID_REGION		-2
//-
#include "KEngine.h"
#include "KRegion.h"
#include "KWeatherMgr.h"

#ifdef _SERVER
#include "KMission.h"
#include "KMissionArray.h"
#define MAX_SUBWORLD_MISSIONCOUNT 10
#define MAX_GLOBAL_MISSIONCOUNT 50
typedef KMissionArray <KMission , MAX_TIMER_PERMISSION> KSubWorldMissionArray;
typedef KMissionArray <KMission , MAX_GLOBAL_MISSIONCOUNT> KGlobalMissionArray;
extern KGlobalMissionArray g_GlobalMissionArray;
#endif

//-

struct KNPC_EXP_WORLD
{
	INT		nExp;
	BYTE	byMinL;
	BYTE	byMaxL;
};

#ifndef TOOLVERSION
class KSubWorld
#else

class CORE_API KSubWorld
#endif
{
public:	
	int			m_nIndex;
	int			m_SubWorldID;
#ifdef _SERVER
	KSubWorldMissionArray m_MissionArray;
	
	BOOL		m_bNpcSeriesAuto;
	int			m_nAutoGoldenNpc;
	BYTE		m_btGoldenType;
//	char		m_szGoldenDropRate[MAX_PATH];
//	char		m_szNormalDropRate[MAX_PATH];
	char		m_szMapInfo[MAX_PATH];
	char		m_szWorldScript[32];

	KItemDropRate*		m_pBossDropRate;
	KItemDropRate*		m_pNormalNpcDropRate;

	KTabFile	m_cTabFile;
	KIniFile	m_cIniFile;
	
#endif
	
	char		m_szMapType[32];
	
	KRegion*	m_Region;
#ifndef _SERVER
	int			m_ClientRegionIdx[MAX_REGION];
	char		m_szMapPath[FILE_NAME_LENGTH];
#endif
	int			m_nWorldRegionWidth;		
	int			m_nWorldRegionHeight;			
	int			m_nTotalRegion;					
	int			m_nRegionWidth;					
	int			m_nRegionHeight;				
	int			m_nCellWidth;					
	int			m_nCellHeight;					
	int			m_nRegionBeginX;				
	int			m_nRegionBeginY;
	int			m_nWeather;						
	DWORD		m_dwCurrentTime;				
	KWorldMsg	m_WorldMessage;					
	KList		m_NoneRegionNpcList;			

#ifdef _SERVER
	KWeatherMgr *m_pWeatherMgr;
	KNPC_EXP_WORLD		m_sNpcExp;
#endif

private:
#ifdef _SERVER
	DWORD		m_dwTaxMoney;
#endif

public:
	KSubWorld();
	~KSubWorld();
	void		Activate();
	void		GetFreeObjPos(POINT& pos);
	BOOL		CanPutObj(POINT pos);
	void		ObjChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nObjIdx);
	void		MissleChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nObjIdx);
	void		AddPlayer(int nRegion, int nIdx);
	void		RemovePlayer(int nRegion, int nIdx);
	void		Close();
	int			GetDistance(int nRx1, int nRy1, int nRx2, int nRy2);						
	void		Map2Mps(int nR, int nX, int nY, int nDx, int nDy, int *nRx, int *nRy);		
	static void Map2Mps(int nRx, int nRy, int nX, int nY, int nDx, int nDy, int *pnX, int *pnY);		
	void		Mps2Map(int Rx, int Ry, int * nR, int * nX, int * nY, int *nDx, int * nDy);	
	void		GetMps(int *nX, int *nY, int nSpeed, int nDir, int nMaxDir = 64);			
	BYTE		TestBarrier(int nMpsX, int nMpsY);
	BYTE		TestBarrier(int nRegion, int nMapX, int nMapY, int nDx, int nDy, int nChangeX, int nChangeY);	
	BYTE		TestBarrierMin(int nRegion, int nMapX, int nMapY, int nDx, int nDy, int nChangeX, int nChangeY);	
	BYTE		GetBarrier(int nMpsX, int nMpsY);											
	DWORD		GetTrap(int nMpsX, int nMpsY);
	void		MessageLoop();
	int			FindRegion(int RegionID);													
	int			FindFreeRegion(int nX = 0, int nY = 0);

#ifdef _SERVER
	
	int			CountAllNpc();
	void		RemoveAllObj();
	void		RemoveAllTrap();
	
	void		BroadCast(const char* pBuffer, size_t uSize);
	void		BroadCastRegion(const void* pBuffer, size_t uSize, int &nMaxCount, int nRegionIndex, int nOX, int nOY);		
	BOOL		LoadMap(int nIdx);
	void		LoadObject(char* szPath, char* szFile);
	void		NpcChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nNpcIdx);
	void		PlayerChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nObjIdx);
	BOOL		SendSyncData(int nIdx, int nClient);
	int			GetRegionIndex(int nRegionID);
	
	void		AddTrap(int nMpsX, int nMpsY, DWORD dwScriptId);
	BOOL		ExecuteScript(char * ScriptFileName, char * szFunName, int nParam1 = 0, int nParam2 = 0);
	BOOL		ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam1, int nParam2);
	CONST KNPC_EXP_WORLD*		GetNpcExp() const { return &m_sNpcExp; }
	
#endif
#ifndef _SERVER
	BOOL		LoadMap(int nIdx, int nRegion);
	void		NpcChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nNpcIdx);
	void		Paint();
	void		Mps2Screen(int *Rx, int *Ry);
	void		Screen2Mps(int *Rx, int *Ry);
#endif
private:
	void		LoadTrap();
	void		ProcessMsg(KWorldMsgNode *pMsg);
#ifdef _SERVER

#else
	void		LoadCell();
#endif

};

#ifndef TOOLVERSION
extern KSubWorld	SubWorld[MAX_SUBWORLD];
#else 
extern CORE_API KSubWorld	SubWorld[MAX_SUBWORLD];
#endif
#endif
