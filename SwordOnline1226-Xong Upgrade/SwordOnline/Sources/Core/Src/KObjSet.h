
#ifndef	KObjSetH
#define	KObjSetH

#include "KObj.h"
#include "KLinkArray.h"

#define		MAX_OBJ_NAME_COLOR		16

typedef struct
{
	char	m_szCopyRight[31];
	BYTE	m_btVersion;
	DWORD	m_dwNum;
} KObjMapDataHead;

typedef struct
{
	int		m_nTemplateID;
	int		m_nState;
	int		m_nDir;
	int		m_nPos[3];
	DWORD	m_dwScriptLength;
	char	m_szScript[80];
} KObjMapData;

typedef struct
{
	int		m_nItemID;
	DWORD	m_dwItemID;
	int		m_nMoneyNum;
	int		m_nItemWidth;
	int		m_nItemHeight;
	int		m_nColorID;
	//AutoAI by kinnox;
	int		m_nGenre;
	int		m_nDetailType;
	int		m_nMovieFlag;
	int		m_nSoundFlag;
	DWORD	m_dwNpcId;
	//endcode
	char	m_szName[FILE_NAME_LENGTH];
} KObjItemInfo;

class KObjSet
{
public:
	KTabFile	m_cTabFile;
	KTabFile	m_cMoneyFile;
	int			m_nObjID;
#ifndef _SERVER
	DWORD		m_dwNameColor[MAX_OBJ_NAME_COLOR];
	int			m_nShowNameFlag;
#endif
private:
	KLinkArray	m_UseIdx;
	KLinkArray	m_FreeIdx;
public:
	KObjSet();
	~KObjSet();
	BOOL	Init();
	int		GetID();
	void	Remove(int nIdx);
#ifdef _SERVER
	int		Add(int nDataID, KMapPos MapPos, KObjItemInfo sItemInfo);

	int		Add(int nDataID, int nSubWorld, int nMpsX, int nMpsY, int nDir, int nState, char* szScript);

	int		AddData(int nDataID, KMapPos MapPos, int nMoneyNum = 0, int nItemID = 0, int nItemWidth = 0, int nItemHeight = 0);

	int		AddMoneyObj(KMapPos MapPos, int nMoneyNum);

	BOOL	ServerLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld);

	BOOL	ServerLoadRegionObj(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);

	BOOL	SetObjItem(int nObjIndex, int nItemID, int nItemWidth, int nItemHeight);

	BOOL	ServerAdd(const int nDataID, const int nSubWorld, const int nMpsX, const int nMpsY, const int nDir, const int nState, const char* pScriptFile);

	BOOL	SyncAdd(int nID, int nClient);
	
	int		GetObjNumber(){ return m_UseIdx.GetCount();};
	
#endif

#ifndef _SERVER
	int		AddData(int nDataID, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX, int nOffY);
	int		ClientAdd(int nID, int nDataID, int nState, int nDir, int nCurFrame, int nXpos, int nYpos, KObjItemInfo sInfo);
	BOOL	ClientLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld, int nRegion);
	BOOL	ClientLoadRegionObj(KPakFile *pFile, DWORD dwDataSize);
	void	RemoveIfClientOnly(int nIdx);
	int		SearchObjAt(int nX, int nY, int nRange);
	DWORD	GetNameColor(int nColorID);
	void	SetShowNameFlag(BOOL bFlag);
	BOOL	CheckShowName();
	int		GetDistanceMpx(int nRx1, int nRy1, int nRx2, int nRy2);
	int		AutoGetObjNear(int nX1, int nY1, int nDistance, BOOL bPickItem = FALSE, BOOL bPickItemBlue = FALSE, BOOL bPickEvent = FALSE, BOOL bPickMoney = FALSE, BOOL bPickAll = FALSE);
#endif
	int		FindID(int nID);
	int		FindName(char *lpszObjName);
private:
	int		FindFree();
	int		CheckClientKind(int nKind);
	int		GetDataIDKind(int nDataID);
#ifdef _SERVER
	int		GetMoneyDataId(int nMoney);
#endif
};

extern	KObjSet	ObjSet;
extern	char	g_szObjKind[Obj_Kind_Num][32];

#endif






