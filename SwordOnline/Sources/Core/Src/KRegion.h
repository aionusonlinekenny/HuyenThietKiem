#ifndef	KRegionH
#define	KRegionH

// --
#include "KEngine.h"
#include "KWorldMsg.h"
// --
#ifdef _SERVER
#define	MAX_BROADCAST_COUNT		100
#else
#define	MAX_REGION		9
#endif

#define	REGION_GRID_WIDTH	16
#define	REGION_GRID_HEIGHT	32

enum MOVE_OBJ_KIND
{
	obj_npc,
	obj_object,
	obj_missle,
};
#ifndef TOOLVERSION
class KRegion
#else
class CORE_API KRegion
#endif
{
	friend class	KSubWorld;
public:
	int			m_nIndex;
	int			m_RegionID;
	KList		m_NpcList;
	KList		m_ObjList;
	KList		m_MissleList;
	KList		m_PlayerList;
	int			m_nConnectRegion[8];
	int			m_nConRegionID[8];
	int			m_nRegionX;
	int			m_nRegionY;
	int			m_nWidth;
	int			m_nHeight;
private:
#ifdef _SERVER
	long		m_Obstacle[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];
	DWORD		m_dwTrap[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];
#endif
	int			m_nNpcSyncCounter;
	int			m_nObjSyncCounter;
	int			m_nActive;
	BYTE*		m_pNpcRef; //Thay doi tu BYTE -> INT by kinnox;
	BYTE*		m_pObjRef; //Thay doi tu BYTE -> INT by kinnox;
	BYTE*		m_pMslRef; //Thay doi tu BYTE -> INT by kinnox;
public:
	KRegion();
	~KRegion();
	BOOL		Init(int nWidth, int nHeight);
	BOOL		Load(int nX, int nY);
#ifdef _SERVER
	BOOL		LoadObject(int nSubWorld, int nX, int nY);
	BOOL		LoadServerObstacle(KPakFile *pFile, DWORD dwDataSize);
	BOOL		LoadServerTrap(KPakFile *pFile, DWORD dwDataSize);
	BOOL		LoadServerNpc(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);
	BOOL		LoadServerObj(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);
#endif

#ifndef _SERVER
	BOOL		LoadObject(int nSubWorld, int nX, int nY, char *lpszPath);
	BOOL		LoadClientNpc(KPakFile *pFile, DWORD dwDataSize);
	BOOL		LoadClientObj(KPakFile *pFile, DWORD dwDataSize);
	static BOOL		LoadLittleMapData(int nX, int nY, char *lpszPath, BYTE *lpbtObstacle);
#endif
	void		Close();
	void		Activate();
	BYTE		GetBarrier(int MapX, int MapY, int nDx, int nDy);	//	µØÍ¼¸ß¶È
	BYTE		GetBarrierMin(int nGridX, int nGridY, int nOffX, int nOffY, BOOL bCheckNpc);
	DWORD		GetTrap(int MapX, int MapY);
	inline BOOL		IsActive()
	{
#ifdef _SERVER
		return m_nActive;
#else
		return TRUE;
#endif
	};
	int			GetRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	BOOL		AddRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	BOOL		DecRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	int			FindNpc(int nMapX, int nMapY, int nNpcIdx, int nRelation);
	int			FindEquip(int nMapX, int nMapY);
	int			FindObject(int nMapX, int nMapY,bool bAutoFind = FALSE);
	int			FindObject(int nObjID);
	void*		GetObjNode(int nIdx);
	int			SearchNpc(DWORD dwNpcID);
	int			SearchNpcName(const char* szName); //AutoAI by kinnox;
#ifdef _SERVER
	void		SendSyncData(int nClient);
	void		BroadCast(const void *pBuffer, DWORD dwSize, int &nMaxCount, int nOX, int nOY);		//
	int			FindPlayer(DWORD dwId);
	BOOL		CheckPlayerIn(int nPlayerIdx);
	void		AddTrap(int nMapX, int nMapY, DWORD dwScriptId);
	void		RemoveTrap();
#endif
#ifndef _SERVER
	void		Paint();
#endif
	void		AddNpc(int nIdx);
	void		RemoveNpc(int nIdx);
	void		AddMissle(int nIdx);
	void		RemoveMissle(int nIdx);
	void		AddObj(int nIdx);
	void		RemoveObj(int nIdx);
	BOOL		AddPlayer(int nIdx);
	BOOL		RemovePlayer(int nIdx);
};

#endif
