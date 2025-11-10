
#ifndef KSkillsH
#define KSkillsH

#include "KMissle.h"
#include "KWorldMsg.h"
#include "KNpc.h"
#include "KNode.h"
#include "KMissleMagicAttribsData.h"
#include "Skill.h"
#define			MAX_MISSLE_DAMAGEATTRIB		17		//
#define			MAX_ENHANCE_SKILL			6		

typedef  KSkill KFightSkill;
class KMissle;
class KSkill;

enum
{
	Missle_StartEvent = 1,
	Missle_FlyEvent,
	Missle_CollideEvent,
	Missle_VanishEvent,
};

#ifdef TOOLVERSION
class CORE_API KSkill  :public ISkill
#else
class KSkill :public ISkill
#endif
{
private:
	
public:
	unsigned long		m_ulLevel;

#ifdef _SERVER
	DWORD				m_dwSkillLevelUpScriptID;	
	DWORD				m_dwSkillLevelDataScriptId; 
#endif

#ifndef _SERVER
	char				m_szSkillIcon[80];
	char				m_szPreCastEffectFile[100];
	char				m_szManPreCastSoundFile[100];
	char				m_szFMPreCastSoundFile[100];
	eSkillLRInfo		m_eLRSkillInfo;			
	BOOL				m_bBuffEffect;	//AutoAI by kinnox;
#endif
	unsigned short		m_usReqLevel;
private:
	int					m_nCharClass;			
	eSKillStyle			m_eSkillStyle;			
	int					m_nSkillTime;			
	BOOL				m_bClientSend;			
	BOOL				m_bHaveLoad;			
	int					m_nInteruptTypeWhenMove;
	BOOL				m_bHeelAtParent;

	CLIENTACTION		m_nCharActionId;		
	int					m_nWaitTime;			
	BOOL				m_bIsAura;				
	BOOL				m_bIsPassivity;			
	BOOL				m_bIsMelee;				
	BOOL				m_bUseAttackRate;		
	BOOL				m_bTargetOnly;			
	BOOL				m_bTargetEnemy;			
	BOOL				m_bTargetAlly;			
	BOOL				m_bTargetObj;			
	BOOL				m_bTargetSelf;			
	int					m_eRelation;
	char				m_szName[32];			
	DWORD				m_nId;					
	BOOL				m_bBaseSkill;			
	BOOL				m_bByMissle;			
	BOOL				m_bIsPhysical;			
	int					m_nCost;				
	NPCATTRIB			m_nSkillCostType;		
	int					m_nMinTimePerCast;
	int					m_nMinTimePerCastOnHorse;		
	int					m_nChildSkillNum;			
	eMisslesForm		m_eMisslesForm;			
	int					m_nValue1;				
	int					m_nValue2;				
	int					m_nEventSkillLevel;
	KMagicAttrib		m_MissleAttribs[MAXSKILLLEVELSETTINGNUM];
	int					m_nMissleAttribsNum;
	KMagicAttrib		m_DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];
	int					m_nDamageAttribsNum;
	KMagicAttrib		m_ImmediateAttribs[MAXSKILLLEVELSETTINGNUM];
	int					m_nImmediateAttribsNum;
	KMagicAttrib		m_StateAttribs[MAXSKILLLEVELSETTINGNUM];
	int					m_nStateAttribsNum;
	eMissleFollowKind   m_eMissleFollowKind;	
	int					m_nFollowKindIndex;		
	DWORD				m_dwFollowKindID;
	int					m_nChildSkillId;		
	int					m_nChildSkillLevel;
	BOOL				m_bFlyingEvent;			
	BOOL				m_bStartEvent;			
	BOOL				m_bCollideEvent;		
	BOOL				m_bVanishedEvent;		
	int					m_nFlySkillId;			
	int					m_nFlyEventTime;		
	int					m_nStartSkillId;		
	int					m_nVanishedSkillId;		
	int					m_nCollideSkillId;		
	BOOL				m_bMustBeHit;			
	eMisslesGenerateStyle m_eMisslesGenerateStyle;	
	int					m_nMisslesGenerateData;	
	int					m_nMaxShadowNum;		
	BOOL				m_bNeedShadow;
	int					m_nAttackRadius;
	int					m_nStateSpecialId;
	BYTE				m_btStatePriority;		
	int					m_nMaxTimes;
	int					m_nEquiptLimited;
	int					m_nHorseLimited;
	BOOL				m_bDoHurt;
	
	short				m_nSkillAttrib;
	KMagicAttrib		m_EnhanceSkills[MAX_ENHANCE_SKILL];
	int					m_nEnhanceSkillsNum;
	BYTE				m_btShowEvent;
	BOOL				m_bIsExpSkill;
	//AutoAI by kinnox;
	BOOL				m_bSkillLifeReplenish;
	//end code;	

public:
	KSkill();
	virtual ~KSkill();				
	BOOL				OnMissleEvent(unsigned short usEvent, KMissle * pMissle) const;
	BOOL				IsTargetOnly()const{return m_bTargetOnly;};
	BOOL				IsTargetSelf()const{return m_bTargetSelf;};
	BOOL				IsTargetEnemy()const{return m_bTargetEnemy;};
	BOOL				IsTargetAlly()const{return m_bTargetAlly;};
	BOOL				IsTargetObj()const{return m_bTargetObj;};
	BOOL				IsNeedShadow()const{	return m_bNeedShadow;};
	int					GetMaxTimes()const{  return m_nMaxTimes;};
	static	int			GetSkillIdFromName(char * szSkillName);
	unsigned short		GetSkillReqLevel()const{	return m_usReqLevel;};
	
	int					GetParam1()const{	return m_nValue1;};
	//AutoAI by kinnox;
	BOOL				IsSkillLifeReplenish() const {return m_bSkillLifeReplenish;};
	//end code;	

#ifdef _SERVER
	unsigned long		GetSkillLevelUpScriptId()const{return m_dwSkillLevelUpScriptID;};
	KMissleMagicAttribsData* CreateMissleMagicAttribsData(int nLauncher) const;
	
	BOOL				CastStateSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime, BOOL bRemove = FALSE) const ;		
	
#endif

#ifndef _SERVER
	eSkillLRInfo		GetSkillLRInfo() const{ return m_eLRSkillInfo;	};
	const char *		GetPreCastEffectFile() const{	 return m_szPreCastEffectFile;};
	const char *		GetPreCastSoundFile(BOOL bIsFeMale) const{ return bIsFeMale? m_szFMPreCastSoundFile: m_szManPreCastSoundFile;};
	
	int					GetFlySkillId() const { return m_nFlySkillId;} 
	int					GetStartSkillId() const { return m_nStartSkillId;} 
	int					GetVanishedSkillId() const { return m_nVanishedSkillId;} 
	int					GetCollideSkillId() const { return m_nCollideSkillId;} 
	int					GetEventSkillLevel() const { return m_nEventSkillLevel;}

	BOOL				GetHorseLimit() const { return m_nHorseLimited; }
	BOOL				IsBuffEffect() const { return m_bBuffEffect;};//AutoAI by kinnox;		
#endif

	const char * GetSkillName(){return m_szName;};
	int					GetSkillStyle(){ return m_eSkillStyle;};
	BOOL				GetInfoFromTabFile(int nCol);
	BOOL				GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow);
	BOOL				Load(int nCol);
	BOOL				Cast(int nLauncher,   int nParam1, int nParam2, int nWaitTime = 0, eSkillLauncherType eLauncherType = SKILL_SLT_Npc, BOOL bRemove = FALSE) const;		//
	int					GetChildSkillNum(int nLevel)const;
	int					GetAttackRadius() const{	return m_nAttackRadius;};
	int					CalculateDamage()const;
	void				LoadSkillLevelData(unsigned long  ulLevel , int nParam);
	friend	class		KMissle;
	int					GetSkillCost(void *)const{return m_nCost;};
	NPCATTRIB			GetSkillCostType()const{return m_nSkillCostType;};
	BOOL				GetItemLimit(int)const;
	CLIENTACTION		GetActionType()const{return m_nCharActionId;};
	int					GetDelayPerCast()const{return m_nMinTimePerCast;	};
	int					GetDelayPerCastOnHorse() const { return m_nMinTimePerCastOnHorse; };		
	unsigned int		GetMissleGenerateTime(int nNo) const ;		
	int					GetChildSkillNum() const {return m_nChildSkillNum;};
	int					GetChildSkillId() const {return m_nChildSkillId;};
	int					GetChildSkillLevel() const {return m_nChildSkillLevel;};
	
	int					GetSkillId() {return m_nId;};
	void				SetSkillId(int nId) {m_nId = nId;};		
	void				SetSkillLevel(unsigned long ulLevel){m_ulLevel = ulLevel;};

	BOOL				IsAura() const { return m_bIsAura;};
	BOOL				IsPhysical() const {return m_bIsPhysical;};
	int					GetMeleeType() const { return m_eMisslesForm;};
	int					GetStateSpecailId() const {return m_nStateSpecialId;		};
	BOOL				CanCastSkill  (int nLauncher, int &nParam1, int &nParam2)  const ;
	void				PlayPreCastSound  (BOOL bIsFeMale , int nX, int nY) const ;
	KMagicAttrib*		GetDamageAttribs()  { return m_DamageAttribs;};	
	
	short				GetSkillAttrib() const { return m_nSkillAttrib; };
	BYTE				GetStatePriority() const {return  m_btStatePriority; };
	KMagicAttrib*		GetEnhanceSkills()  { return m_EnhanceSkills;};	
	int					GetEnhanceSkillsNum()  { return m_nEnhanceSkillsNum;};
	inline BOOL			IsExpSkill() const { return m_bIsExpSkill;};
	

private:
	void				Vanish(KMissle *) const;
	void				FlyEvent(KMissle *)  const ;
	void				Collidsion(KMissle *) const ;
	BOOL				CastMissles(int nLauncher,	int nParam1, int nParam2, int nWaitTime = 0 , eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const;
	BOOL				CastInitiativeSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime) const;
	BOOL				CastPassivitySkill(int nLauncher, int nParam1, int nParam2, int nWaitTime, BOOL bRemove = FALSE) const ;		//
	
	int					CastWall(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY, int nSrcX, int nSrcY) const ;		//
	int					CastLine(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY, int nDestX, int nDestY) const ;		//
	int					CastSpread(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastCircle(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastZone(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY, int nDestX, int nDestY) const ;		//
	int					CastExtractiveLineMissle(TOrdinSkillParam * pSkillParam, int nDir, int nSrcX,  int nSrcY, int nXOffset, int nYOffset, int nDesX, int nDesY) const ;
	int					Param2PCoordinate(int nLauncher, int nParam1, int nParam2 , int *npPX, int *npPY, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const ;
	void				CreateMissle(int nLauncher, int ChildSkillId,  int nMissleIndex) const ;	
	BOOL				ParseString2MagicAttrib(unsigned long ulLevel, char * szMagicAttribName, char * szValue) ;
	inline const char *		MagicAttrib2String(int MagicAttrib) const ;

#ifdef _SERVER
	
	void				CastInitiativeSkill(int nLauncher, int nParam) const;
	
#endif

public:

#ifndef _SERVER
	void				DrawSkillIcon(int nSkillID, int x, int y, int Width, int Height);		//
	KRUImage			m_RUIconImage;
	KRUShadow			m_RUIconShadow;
	char				m_szSkillDesc[300];
	
	char				m_szSkillDescEx[1024];
	
	static void			GetDesc(unsigned long ulSkillId, unsigned long ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc);
	void				GetDescAboutLevel(char * pszMsg, int nSkillId, int nSkillLevel);
	
	void				GetDescAboutEventSkillLevel(char * pszMsg, int nIndex);
	
#endif
};

#define MAX_MELEEWEAPON_PARTICULARTYPE_NUM 100
#define MAX_RANGEWEAPON_PARTICULARTYPE_NUM 100
extern int		g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
extern int		g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];	
extern int		g_nHandSkill;

#endif
