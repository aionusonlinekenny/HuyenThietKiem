
#ifndef KObj_H
#define KObj_H

#ifdef	_SERVER
#define	MAX_OBJECT	20000 //20000
#else
#define	MAX_OBJECT	256
#endif

#include "KPolygon.h"

#ifndef _SERVER
#include "KSprite.h"
#include "KWavSound.h"
#include "KSprControl.h"
#endif

#include "KIniFile.h"
#include "GameDataDef.h"
#ifndef	_SERVER
#include "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#define		OBJ_BAR_WIDTH		3								
#define		OBJ_BAR_HEIGHT		5								
#define		OBJ_BAR_SIZE		(OBJ_BAR_WIDTH*OBJ_BAR_HEIGHT)

#define		OBJ_BOX_STATE_CLOSE		0		
#define		OBJ_BOX_STATE_OPEN		1		
#define		OBJ_DOOR_STATE_CLOSE	0		
#define		OBJ_DOOR_STATE_OPEN		1		
#define		OBJ_PROP_STATE_DISPLAY	0		
#define		OBJ_PROP_STATE_HIDE		1		
#define		OBJ_TRAP_STATE_STOP		0		
#define		OBJ_TRAP_STATE_ACTIVE	1		
#define		OBJ_TRAP_STATE_ACTING	2

#define		OBJ_BELONG_TIME		600

enum	Obj_Bar_Type			
{
	Obj_Bar_Empty = 0,			
	Obj_Bar_Full,				
	Obj_Bar_LT,					
	Obj_Bar_RT,					
	Obj_Bar_LB,					
	Obj_Bar_RB,					
	Obj_Bar_Type_Num,			
};

typedef struct SOBJ_LIGHT_DATA
{
	int			m_nRadius;			
	int			m_nRed;				
	int			m_nGreen;			
	int			m_nBlue;			
	int			m_nAlpha;			
	int			m_nReflectType;	
	SOBJ_LIGHT_DATA() {Release();};
	void	Release() {	m_nRadius = 0; m_nRed = 0; m_nGreen = 0; m_nBlue = 0; m_nAlpha = 0; m_nReflectType = 0; };
} OBJ_LIGHT;		

enum	ObjTrap_Kind
{
	ObjTrap_Kind_Cycle_Image_Static = 0,
	ObjTrap_Kind_Cycle_Image_Dir,
	ObjTrap_Kind_Auto_Image_Static,
	ObjTrap_Kind_Auto_Image_Dir,
	ObjTrap_Kind_Auto_Image_Target,
	ObjTrap_Kind_Auto_Delay_Static,
	ObjTrap_Kind_Auto_Delay_Dir,
	ObjTrap_Kind_Auto_Delay_Target,
};

typedef struct OBJ_SKILL_DATA
{
	int		m_nKind;			
	int		m_nCamp;			
	int		m_nRange;			
	int		m_nCastTime;		
	int		m_nID;				
	int		m_nLevel;			
	int		m_nTarX;			
	int		m_nTarY;			
	OBJ_SKILL_DATA() {Release();};
	void	Release() {m_nKind = 0; m_nCamp = camp_animal; m_nRange = 0; m_nCastTime = 0; m_nID = 0; m_nLevel = 0; m_nTarX = 0; m_nTarY = 0;};
} OBJ_SKILL;		

// --

#ifdef _SERVER
class ServerImage
{
public:
	int			m_nTotalFrame;						
	int			m_nCurFrame;						
	int			m_nTotalDir;						
	int			m_nCurDir;							
	DWORD		m_dwTimer;							
	DWORD		m_dwInterval;						

protected:
	int			m_nDirFrames;						

public:
	ServerImage();									
	void		Release();							
	void		SetTotalFrame(int nTotalFrame);		
	void		SetCurFrame(int nCurFrame);			
	void		SetTotalDir(int nTotalDir);			
	BOOL		SetCurDir(int nDir);				
	void		SetInterVal(DWORD dwInterval);		
	int			GetOneDirFrames();					

	BOOL		GetNextFrame(BOOL bLoop = TRUE);	
	BOOL		GetPrevFrame(BOOL bLoop = TRUE);	
	BOOL		CheckEnd();							
	void		SetDirStart();						
	void		SetDirEnd();						
	BOOL		SetCurDir64(int nDir);		
	int			GetCurDirFrameNo();
};
#endif
#ifdef TOOLVERSION
class CORE_API KObj
#else
class KObj
#endif
{
	friend class KObjSet;
public:
	int			m_nID;
	int			m_nDataID;
	int			m_nKind;
	int			m_nIndex;
	int			m_nBelongRegion;

	int			m_nSubWorldID;			
	int			m_nRegionIdx;			
	int			m_nMapX;				 
	int			m_nMapY;			
	int			m_nOffX;				
	int			m_nOffY;				
	int			m_nDir;	

	int			m_nState;				
	int			m_nLifeTime;			
	int			m_nBornTime;			
	int			m_nWaitTime;		
	OBJ_SKILL	m_cSkill;

	char		m_szName[FILE_NAME_LENGTH];		
	DWORD		m_dwScriptID;			
	int			m_nColorID;			

	int			m_nItemDataID;			
	int			m_nItemWidth;			
	int			m_nItemHeight;			
	int			m_nMoneyNum;
	
	DWORD		m_dwItemID;
	//AutoAI by kinnox;
	int 		m_nGenre;
	int 		m_nDetailType;
	DWORD		m_dwNpcId;
	//end code;
#ifdef _SERVER
	BOOL		m_bPickExecute;
	int			m_nBelong;
	int			m_nBelongTime;
	ServerImage m_cImage;				
#endif

#ifndef _SERVER
	char		m_szImageName[80];		
	char		m_szSoundName[80];		
	KSprControl	m_cImage;				

	char		m_szImageDropName[80];	
	KSprControl	m_cImageDrop;			
	int			m_nDropState;			

	KRUImage	m_Image;
	unsigned int	m_SceneID;

	int			m_nLayer;				
	int			m_nHeight;				
	OBJ_LIGHT	m_sObjLight;			
	DWORD		m_dwNameColor;	
	BOOL		m_bAutoCheck;			// Kiem tra vat pham co nhat duoc hay ko [Auto In Game]
	BOOL		m_bAutoLag;		
#endif

	KPolygon	Polygon;				
	BYTE		m_btBar[OBJ_BAR_SIZE];	

private:
	DWORD		m_dwTimer;				

#ifndef _SERVER
	KCacheNode*	m_pSoundNode;			
	KWavSound*	m_pWave;				

	BOOL		m_bDrawFlag;
#endif

public:
	KObj();
	void		GetMpsPos(int *pX, int *pY);
	void		Release();
	void		Activate();
//	void		Save(KIniFile *IniFile, LPSTR Section);
//	void		Load(int nObjIndex, int nSubWorldID, KIniFile *IniFile, LPSTR Section);

	void		ExecScript(int nPlayerIdx);
	void		CastSkill(int nDir);
	void		CastSkill(int nXpos, int nYpos);

	void		SetIndex(int nIndex);
	void		SetWorldID(int nID);			
	void		SetLifeTime(int nLifeTime);
	BOOL		SetDir(int n64Dir);
	void		SetState(int nState, int nPlaySoundFlag = 0);
	void		SetImageDir(int nDir);
	void		Remove(BOOL bSoundFlag);
	void		SetScriptFile(char *lpszScriptFile);
	int			GetKind();
	int 		GetDistanceSquare(int nNpcIndex);//AutoAI by kinnox;
#ifdef _SERVER
	int			FindEnemy();
	void		TrapAct();
	void		SetItemDataID(int nItemDataID);
	BOOL		SyncAdd(int nClient);
	void		SyncState();
	void		SyncDir();
	void		SyncRemove(BOOL bSoundFlag);
	int			GetItemDataID();
	void		SetItemBelong(int nPlayerIdx);
	void		SetEntireBelong(int nPlayerIdx);
	//executeitem by kinnox;
	void		SetObjPickExecute(BOOL bFlag) {m_bPickExecute = (BOOL)bFlag;};
	BOOL		GetObjPickExecute() {return m_bPickExecute;};
	//
#endif

#ifndef _SERVER
	void		Draw();
	void		DrawInfo();
	void		DrawBorder();
#endif
	void		PolygonChangeToBar(
					KPolygon Polygon,		
					int nGridWidth,			
					int nGridHeight,		
					int nTableWidth,		
					int nTableHeight,		
					BYTE *lpbBarTable);

private:
	void		BoxOpen();
	void		BoxClose();
	void		DoorOpen();
	void		DoorClose();
#ifndef _SERVER
	void		PlayLoopSound();
	void		PlayRandSound();
	int			GetSoundPan();
	int			GetSoundVolume();
	void		PlaySound();
#endif
};

extern	KObj	Object[MAX_OBJECT];
// --
#endif

