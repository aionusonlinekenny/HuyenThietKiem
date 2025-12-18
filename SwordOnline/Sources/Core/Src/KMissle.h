//////////////////////////////////////////////////////////////////////
// KMissle.h: interface for the KMissle class.
//////////////////////////////////////////////////////////////////////
#ifndef	KMissleH
#define KMissleH

#ifdef  _SERVER
#define MAX_MISSLE  200000
#else
#define MAX_MISSLE 500
#endif
#include "KCore.h"
#include "SkillDef.h"
#include "KObj.h"
#ifndef _SERVER
#include "KMissleRes.h"
#endif

#include "KNode.h"
#include "KITabFile.h"
#include "KSkills.h"
#include "KNpcSet.h"
#include "Scene/ObstacleDef.h"
#include "KSubWorld.h"
#include "KIndexNode.h"
#include "GameDataDef.h"
#ifdef _SERVER
#include "KMissleMagicAttribsData.h"
#endif
enum 
{
	Interupt_None,
		Interupt_EndNewMissleWhenMove,
		Interupt_EndOldMissleLifeWhenMove,
};

#ifndef _SERVER
struct TMissleForShow
{
	int nPX ;
	int nPY ;
	int nPZ ;
	int nNpcIndex;
	int nLauncherIndex;
};
#endif

#ifndef TOOLVERSION
class KMissle
#else
class CORE_API KMissle  
#endif
{
	friend class	KSkill;
private:
public:
	KIndexNode			m_Node;
	char				m_szMissleName[30];		
	int					m_nAction;				
	BOOL				m_bIsSlow;				
	BOOL				m_bClientSend;			
	BOOL				m_bRemoving;			
	BOOL				m_bIsMelee;				
	eMissleMoveKind		m_eMoveKind;
	eMissleFollowKind	m_eFollowKind;			
	int					m_nHeight;
	int					m_nHeightSpeed;
	int					m_nLifeTime;
	int					m_nSpeed;
	int					m_nSkillId;
	BOOL				m_bRangeDamage;	
	int					m_eRelation;
	BOOL				m_bAutoExplode;
	BOOL				m_bTargetSelf;
	BOOL				m_bBaseSkill;
	BOOL				m_bByMissle;
	INT					m_nInteruptTypeWhenMove;
	BOOL				m_bHeelAtParent;
	int					m_nLauncherSrcPX;		
	int					m_nLauncherSrcPY;
	int					m_nCollideRange;
	int					m_nDamageRange;
	BOOL				m_bCollideVanish;
	BOOL				m_bCollideFriend;
	BOOL				m_bCanSlow;
	int					m_nKnockBack;
	int					m_nStunTime;
	
	BOOL				m_bFlyEvent;
	int					m_nFlyEventTime;
	BOOL				m_bSubEvent;
	BOOL				m_bStartEvent;
	BOOL				m_bCollideEvent;
	BOOL				m_bVanishedEvent;
	
	unsigned long		m_ulDamageInterval;
	BOOL				m_bMustBeHit;
	int					m_nCurrentLife;
	int					m_nStartLifeTime;
	int					m_nCollideOrVanishTime; 
	int					m_nCurrentMapX;
	int					m_nCurrentMapY;
	int					m_nCurrentMapZ;
	int					m_nXOffset;
	int					m_nYOffset;
	int					m_nRefPX;
	int					m_nRefPY;
	
	int					m_nDesMapX;
	int					m_nDesMapY;				
	int					m_nDesRegion;
	BOOL				m_bNeedReclaim;	
	TMisslePos			m_NeedReclaimPos[4];
	BOOL				m_bDoHurt;				
	int					m_nXFactor;
	int					m_nYFactor;
	int					m_nLevel;				//	技能等级
	
	int					m_nFollowNpcIdx;		
	DWORD				m_dwFollowNpcID;		
	
	int					m_nLauncher;			//	发射者在NpcSet中的Index
	DWORD				m_dwLauncherId;			//	发射者的唯一ID
	int					m_nParentMissleIndex;   // if 0 then means parent is npclauncher	
	int					m_nCurrentSpeed;		//	当前速度（可能被减速）
	int					m_nZAcceleration;		//	Z轴的加速度
	eMissleStatus		m_eMissleStatus;		//	子弹当前的状态
	int					m_nMissleId;			//	子弹在Missle中的ID
	int					m_nSubWorldId;			//	子世界ID
	int					m_nRegionId;			//	区域ID
	int					m_nMaxDamage;			//	最大伤害
	int					m_nElementType;			//	元素伤害类型
	int					m_nMaxElementDamage;	//	最大元素伤害(首次)
	int					m_nElementTime;			//	元素持续时间
	int					m_nElementInterval;		//	元素间隔时间
	int					m_nElementPerDamage;	//	元素伤害时，每次发作所受的伤害值
	int					m_nParam1;				//	参数一
	int					m_nParam2;				//	参数二
	int					m_nParam3;				//	参数三
	int					m_nFirstReclaimTime;
	int					m_nEndReclaimTime;
	int					m_nTempParam1;			//	运行期使用的参数
	int					m_nTempParam2;
	int					m_nDirIndex;			
	int					m_nDir;					
	int					m_nAngle;				
	DWORD				m_dwBornTime;			
	BOOL				m_bUseAttackRating;	
	BYTE				m_btMissRate;
	BYTE				m_btHitCount;
	int					m_nLastDoCollisionIdx;// fix skills con lon dao	
#ifdef _SERVER
	unsigned long		m_ulNextCalDamageTime;
	BYTE				m_btRandomParam;
#endif

#ifndef _SERVER
	BOOL				m_bMultiShow;			//	子弹有两个显示
	BOOL				m_bFollowNpcWhenCollid; //	爆炸效果跟随被击中的人物
	int					m_btRedLum;
	int					m_btGreenLum;
	int					m_btBlueLum;
	unsigned short		m_usLightRadius;
	KMissleRes			m_MissleRes;			//	子弹的资源
	unsigned int		m_SceneID;
#endif
	
	
private:
	BOOL				Init( int nLauncher, int nMissleId, int nXFactor, int nYFactor, int nLevel);
	void				OnVanish();//即将消失
	void				OnCollision();//碰撞
	void				OnFly();//飞行过程中
	void				OnWait();
	void				DoWait();
	void				DoFly();
	BOOL				PrePareFly();
	void				DoVanish();
	void				DoCollision();
	int					CheckNearestCollision();
	int					FindNearestCollision(int nLauncher, int nRelation);
	friend				class KMissleSet;
	void				Release();
	int					CheckCollision();//检测是否碰撞// 1表示正常碰撞到物体，0表示未碰撞到任何物体, -1表示落地
	BOOL				CheckBeyondRegion(int nDOffsetX, int nDOffsetY);//检测是否越界   //FALSE表示越到一个无效的位置，TRUE表示OK
	int					GetDir(int dx,int dy);
	KMissle&			operator=(KMissle& Missle);
	DWORD				GetCurrentSubWorldTime();
	BOOL				ProcessDamage(int nNpcId);
	int					ProcessCollision();//处理碰撞
	int					ProcessCollision(int nLauncherIdx, int nRegionId, int nMapX, int nMapY, int nRange , int eRelation);

	inline void	ZAxisMove()
	{
		if (m_nZAcceleration)
		{
			m_nHeight += m_nHeightSpeed;
			if (m_nHeight < 0) m_nHeight = 0;
			m_nHeightSpeed -= m_nZAcceleration;
			m_nCurrentMapZ = m_nHeight >> 10;
		}
	}
	//TRUE表示遇到障碍，FALSE表示未遇到，一切正常
	inline BOOL TestBarrier()
	{
		int nBarrierKind = SubWorld[m_nSubWorldId].TestBarrier(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, 0, 0);
		if (nBarrierKind == Obstacle_Normal || nBarrierKind == Obstacle_Jump)
		{
			
			return TRUE;
		}
		return FALSE;
	}
	
public:	
	KMissle();
	virtual ~KMissle();
	BOOL				GetInfoFromTabFile(int nMissleId);
	BOOL				GetInfoFromTabFile(KITabFile * pTabFile, int nMissleId);
	void				GetMpsPos(int *pPosX, int *pPosY);
	int					Activate();
static	BOOL			GetOffsetAxis(int nSubWorld, int nSrcRegionId, int nSrcMapX, int nSrcMapY,
							int nOffsetMapX, int nOffsetMapY, 
							int &nDesRegionId, int &nDesMapX, int &nDesMapY);
	
#ifdef _SERVER
	KMissleMagicAttribsData * m_pMagicAttribsData;
	int					SetMagicAttribsData(KMissleMagicAttribsData * pData)
	{
		m_pMagicAttribsData = pData;
		if (pData)
			return pData->AddRef();
		else 
			return 0;
	};
#endif

#ifndef _SERVER
	void				Paint();
	void				GetLightInfo(KLightInfo * pLightInfo);
	BOOL				CreateSpecialEffect(eMissleStatus eStatus,  int nPX, int nPY, int nPZ, int nNpcIndex = 0);
	static BOOL				CreateMissleForShow(char * szMovie, char * szSprInfo, char * szSound, TMissleForShow * pShowParam);
#endif //_SERVER
	
};


extern CORE_API KMissle			Missle[MAX_MISSLE];
extern CORE_API KMissle			g_MisslesLib[MAX_MISSLESTYLE];//Base 1
#endif
