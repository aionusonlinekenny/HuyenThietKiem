#ifndef KNpcH
#define KNpcH
#include <vector> // find way by kinnox;


class ISkill;
#include "KCore.h"
#include "KSkillList.h"
#include "KMagicAttrib.h"
#include "GameDataDef.h"
#include "KNpcFindPath.h"
#include "KNpcDeathCalcExp.h"
#include "KIndexNode.h"
#include "KNpcGold.h"

#ifdef _SERVER
#include "KTaskFuns.h"
#endif


class KSkill;
#ifndef _SERVER
#include "KNpcRes.h"
#include "../KJXPathFinder.h" // find way by kinnox;
#endif
#ifdef	_SERVER
#define	MAX_NPC			65536//65536		//
#else
#define	MAX_NPC			512			//
#endif
#define MAX_NPCSTYLE	2500

#define		MAX_AI_PARAM					11
#define		MAX_NPC_USE_SKILL				4

#define		STATE_FREEZE	0x01
#define		STATE_POISON	0x02
#define		STATE_STUN		0x04		//

#define		STATE_POISON2MANA		0x08
#define		STATE_HIDE				0x10
#define		STATE_RANDMOVE			0x20
#define		STATE_IGNORENEGATIVE	0x40


enum NPCATTRIB
{
	attrib_mana,
	attrib_stamina,
	attrib_life,
	attrib_maxmana,
	attrib_maxstamina,
	attrib_maxlife,
};

enum NPCCMD
{
	do_none,		
	do_stand,		
	do_walk,		
	do_run,			
	do_jump,
	do_glide,		
	do_skill,		
	do_magic,		
	do_attack,		
	do_sit,			
	do_hurt,		
	do_death,		
	do_defense,		
	do_idle,		
	do_specialskill,
	do_special1,	
	do_special2,	
	do_special3,	
	do_special4,	
	do_runattack,
	do_manyattack,
	do_jumpattack,
	do_revive,
};

enum CLIENTACTION
{
	cdo_fightstand,
	cdo_stand,
	cdo_stand1,
	cdo_fightwalk,
	cdo_walk,
	cdo_fightrun,
	cdo_run,
	cdo_hurt,
	cdo_death,
	cdo_attack,
	cdo_attack1,
	cdo_magic,
	cdo_sit,
	cdo_jump,
	cdo_none,   
	cdo_count,
};

enum DAMAGE_TYPE
{
	damage_physics = 0,		
	damage_fire,			
	damage_cold,			
	damage_light,			
	damage_poison,			
	damage_magic,			
	damage_num,				
};

enum	enumDEATH_MODE
{
	enumDEATH_MODE_NPC_KILL = 0,		
	enumDEATH_MODE_PLAYER_NO_PUNISH,	
	enumDEATH_MODE_PLAYER_PUNISH,		
	enumDEATH_MODE_PKBATTLE_PUNISH,		
	enumDEATH_MODE_NUM,
};

typedef struct
{
	NPCCMD		CmdKind;		
	int			Param_X;		
	int			Param_Y;		
	int			Param_Z;		
} NPC_COMMAND;

typedef struct
{
	int		nTotalFrame;
	int		nCurrentFrame;
} DOING_FRAME;

struct KState
{
	int	nMagicAttrib;
	int	nValue[2];
	int	nTime;
};

struct	KSyncPos
{
	DWORD	m_dwRegionID;
	int		m_nMapX;
	int		m_nMapY;
	int		m_nOffX;
	int		m_nOffY;
	int		m_nDoing;
};
//
struct Coordinates {
    int x;
    int y;
};
//
class KStateNode : public KNode
{
public:
	int				m_SkillID;					
	int				m_Level;					
	int				m_LeftTime;					
	KMagicAttrib	m_State[MAX_SKILL_STATE];	
	int				m_StateGraphics;			
};

#ifndef _SERVER
struct	KClientNpcID
{
	DWORD	m_dwRegionID;
	int		m_nNo;
};
#endif

#ifdef _SERVER
struct KItemDropRate
{
	struct	KItemParam
	{
		int		nGenre;
		int		nDetailType;
		int		nParticulType;
		int		nRate;
	};
	int			nCount;
	int			nMagicRate;
	int			nMaxRandRate;
	int			nMoneyRate;
	int			nMoneyScale;
	int			nMinItemLevelScale;
	int			nMaxItemLevelScale;
	int			nMinItemLevel;
	int			nMaxItemLevel;
	KItemParam*	pItemParam;
};
#endif

class KNpc
{
	friend class KNpcSet;
public:
	DWORD				m_dwID;					
	int					m_Index;				
	KIndexNode			m_Node;
	BYTE				m_Level;				
	BYTE				m_Kind;
	
	BYTE				m_CustomKind;
	
	BYTE				m_Series;				
	int					m_Height;
	BYTE				m_btRankId;
	BYTE				m_ImagePlayer; //Player Avatar by kinnox;
	//
	USHORT				m_nStature;
	BYTE				m_btStateInfo[MAX_NPC_RECORDER_STATE]; 	
			
	KNpcGold			m_cGold;

	BOOL				m_bHaveLoadedFromTemplate;
	
	KState				m_PoisonState;			
	KState				m_FreezeState;						
	KState				m_StunState;			
	KState				m_LifeState;			
	KState				m_ManaState;			
	KState				m_DrunkState;

	
	KState				m_Poison2DecManaState;
	KState				m_HideState;
	KState				m_RandMoveState;
	KState				m_IgnoreNegativeState;
	

	KList				m_StateSkillList;		
	BYTE				m_Camp;					
	BYTE				m_CurrentCamp;
	
	BYTE				m_CustomCamp;
	
	NPCCMD				m_Doing;				
	CLIENTACTION		m_ClientDoing;			
	DOING_FRAME			m_Frames;				
	KSkillList			m_SkillList;			
	int					m_SubWorldIndex;		
	int					m_RegionIndex;			
	USHORT				m_ActiveSkillID;		

	USHORT				m_ActiveAuraID;

	int					m_CurrentExperience;
	UINT				m_CurrentLife;
	UINT				m_CurrentLifeMax;
	int					m_CurrentLifeReplenish;
	int					m_CurrentMana;
	int					m_CurrentManaMax;
	short				m_CurrentManaReplenish;
	int					m_CurrentStamina;
	int					m_CurrentStaminaMax;
	short				m_CurrentStaminaGain;
	BYTE				m_CurrentStaminaLoss;
	KMagicAttrib		m_PhysicsDamage;
	KMagicAttrib		m_CurrentFireDamage;
	KMagicAttrib		m_CurrentColdDamage;
	KMagicAttrib		m_CurrentLightDamage;
	KMagicAttrib		m_CurrentPoisonDamage;
	
	KMagicAttrib		m_MagicPhysicsDamage;
	KMagicAttrib		m_CurrentMagicFireDamage;
	KMagicAttrib		m_CurrentMagicColdDamage;
	KMagicAttrib		m_CurrentMagicLightDamage;
	KMagicAttrib		m_CurrentMagicPoisonDamage;
	
	short				m_CurrentAttackRating;
	short				m_CurrentDefend;
	
	char				m_ManaShield;
	short				m_DynamicMagicShield;		
	short				m_CurrentReturnResist;
	

	short				m_CurrentFireResist;	
	short				m_CurrentColdResist;	
	short				m_CurrentPoisonResist;	
	short				m_CurrentLightResist;	
	short				m_CurrentPhysicsResist;	
	short				m_CurrentFireResistMax;		
	short				m_CurrentColdResistMax;		
	short				m_CurrentPoisonResistMax;	
	short				m_CurrentLightResistMax;	
	short				m_CurrentPhysicsResistMax;
	

	float				m_CurrentWalkSpeed;		
	float				m_CurrentRunSpeed;		
	short				m_CurrentJumpSpeed;		
	short				m_CurrentJumpFrame;		
	short				m_CurrentAttackSpeed;	
	short				m_CurrentCastSpeed;		
	short				m_CurrentVisionRadius;	
	short				m_CurrentAttackRadius;	
	short				m_CurrentActiveRadius;	
	short				m_CurrentHitRecover;	
	BYTE				m_CurrentTreasure;		

	short				m_CurrentMeleeDmgRetPercent;	
	short				m_CurrentMeleeDmgRet;			
	short				m_CurrentRangeDmgRetPercent;	
	short				m_CurrentRangeDmgRet;			
	BOOL				m_CurrentSlowMissle;			

	int					m_CurrentDamageReduce;			
	int					m_CurrentElementDamageReduce;	

	short				m_CurrentDamage2Mana;			
	int					m_CurrentManaPerEnemy;			
	char				m_CurrentLifeStolen;			
	char				m_CurrentManaStolen;			
	char				m_CurrentStaminaStolen;			
	short				m_CurrentKnockBack;				
	short				m_CurrentDeadlyStrike;			
	int					m_CurrentBlindEnemy;			
	short				m_CurrentPiercePercent;			
	short				m_CurrentFreezeTimeReducePercent;	
	short				m_CurrentPoisonTimeReducePercent;	
	short				m_CurrentStunTimeReducePercent;		

	short				m_CurrentFireEnhance;			
	short				m_CurrentColdEnhance;			
	short				m_CurrentPoisonEnhance;			
	short				m_CurrentLightEnhance;			
	short				m_CurrentAddPhysicsDamage;		
	short				m_CurrentMeleeEnhance[MAX_MELEE_WEAPON];	
	short				m_CurrentRangeEnhance;			
	short				m_CurrentHandEnhance;
	short				m_CurrentExpEnhance;		
	
	short				m_PoisonDamageReturn;
	short				m_ReturnSkill;
	KMagicAttrib		m_AutoReplySkill;
	short				m_LifeReplenishPercent;
	short				m_IgnoreSkill;
	KMagicAttrib		m_AutoAttackSkill;
	WORD				m_AppendSkill[5];
	KMagicAttrib		m_AutoDeathSkill;
	KMagicAttrib		m_AutoRescueSkill;
	WORD				m_StaticMagicShield;
	

	int					m_Dir;							
	int					m_RedLum;						
	int					m_GreenLum;
	int					m_BlueLum;
	int					m_MapX, m_MapY, m_MapZ;			
	int					m_OffX, m_OffY;					
	int					m_DesX, m_DesY;					
	int					m_SkillParam1, m_SkillParam2;
	int					m_OriginX, m_OriginY;	
	int					m_NextAITime;
	BYTE				m_AIMAXTime;
	int					m_HelmType;					
	int					m_ArmorType;				
	int					m_WeaponType;				
	int					m_HorseType;				
	BOOL				m_bRideHorse;
	BYTE				m_nReducedDamge;
	
	WORD				m_wMaskType;
	WORD				m_wMaskMark;
	BYTE				m_byMantleLevel;
	short				m_MantleType;
	

	char				Name[32];				
	int					m_nSex;					
	int					m_NpcSettingIdx;		
	int					m_CorpseSettingIdx;		
	char				ActionScript[80];		
	DWORD				m_ActionScriptID;		
	DWORD				m_TrapScriptID;
	
	DWORD				m_TimerScriptID;
	

	int					m_nPeopleIdx;			
	int					m_nLastDamageIdx;		
	int					m_nLastPoisonDamageIdx;	
	int					m_nObjectIdx;

	int					m_Experience;			
	UINT				m_LifeMax;				
	int					m_LifeReplenish;		
	int					m_ManaMax;				
	int					m_ManaReplenish;		
	int					m_StaminaMax;			
	int					m_StaminaGain;			
	int					m_StaminaLoss;			
	int					m_AttackRating;			
	int					m_Defend;				
	int					m_FireResist;			
	int					m_ColdResist;			
	int					m_PoisonResist;			
	int					m_LightResist;			
	int					m_PhysicsResist;		
	int					m_FireResistMax;		
	int					m_ColdResistMax;		
	int					m_PoisonResistMax;		
	int					m_LightResistMax;		
	int					m_PhysicsResistMax;		
	int					m_WalkSpeed;			
	int					m_RunSpeed;				
	int					m_JumpSpeed;			
	int					m_AttackSpeed;			
	int					m_CastSpeed;			
	int					m_VisionRadius;			
	int					m_DialogRadius;			
	int					m_ActiveRadius;			
	int					m_HitRecover;			
	int					m_Treasure;				
	BOOL				m_bClientOnly;			

	int					m_nCurrentMeleeSkill;	
	int					m_nCurrentMeleeTime;	
	
	int					m_AiMode;				
	int					m_AiParam[MAX_AI_PARAM];
	int					m_AiAddLifeTime;

	int					m_HeadImage;

	int					m_FightMode;
	int					m_OldFightMode;
	BOOL				m_bExchangeServer;
	BOOL				m_bWaitingPlayerFeedBack;
#ifdef _SERVER
	int					m_AiSkillRadiusLoadFlag;
	KNpcDeathCalcExp	m_cDeathCalcExp;		
	KItemDropRate*		m_pDropRate;
	int					m_nCurPKPunishState;
	//BOOL				HasTeamStateChanged();
	char				CustomName[32];
	int					m_nParam[500];
	int					m_nTotalReceiveDamage;
	bool				m_bRemoveWhenDeath;
	int 				m_nImmortal;
	void				MaintainAura();
#endif

#ifndef	_SERVER
	int					m_SyncSignal;			
	KClientNpcID		m_sClientNpcID;			
	DWORD				m_dwRegionID;			
	KSyncPos			m_sSyncPos;
	BYTE				m_btPKFlag;	
	char				m_szChatBuffer[MAX_SENTENCE_LENGTH];
	int					m_nChatContentLen;
	int					m_nChatNumLine;
	int					m_nChatFontWidth;
	unsigned int		m_nCurChatTime;
	int					m_nSleepFlag;
	int					m_nHurtHeight;
	int					m_nHurtDesX;
	int					m_nHurtDesY;

	int					m_nTongFlag;
	
	short				m_nWorldStat;
	BOOL				m_bOpenShop;
	char				m_szShopAdv[16];
	short				m_nSecMoveBlurNo;
	short				m_nSecMoveBlurTime;
	char				m_cPKValue;
	short				m_nRepute;
	short				m_nFuyuan;
	char				m_cTransLife;
	char				m_szTongName[defTONG_NAME_MAX_LENGTH + 2];
	char				m_szTongTitle[defTONG_TITLE_MAX_LENGTH + 2];
	WORD				m_wTitleId;
	BOOL				m_bWarning;//AutoAI by kinnox;	
	std::vector<FindPathNode> m_PathFind;// find way by kinnox;
	FindPathNode		m_AutoMoveTemp;	// find way by kinnox;
	bool				m_bEditPos;
#endif
private:
	int					m_LoopFrames;			
	int					m_nPlayerIdx;
	int					m_DeathFrame;			
	int					m_StandFrame;
	int					m_HurtFrame;
	int					m_AttackFrame;
	int					m_CastFrame;
	int					m_WalkFrame;
	int					m_RunFrame;
	int					m_StandFrame1;
	int					m_ReviveFrame;			
	int					m_SitFrame;
	int					m_JumpFrame;
	int					m_JumpFirstSpeed;
	NPC_COMMAND			m_Command;				
	BOOL				m_ProcessAI;			
	BOOL				m_ProcessState;
	int					m_XFactor;
	int					m_YFactor;
	int					m_JumpStep;
	int					m_JumpDir;
	int					m_SpecialSkillStep;		
	NPC_COMMAND			m_SpecialSkillCommand;
	KNpcFindPath		m_PathFinder;
	BOOL				m_bActivateFlag;

#ifdef _SERVER
	BYTE				m_bGold;
	KTimerTaskFun		m_TimerTask;
	// DEBUG: Track sitting regeneration per NPC instance (not static!)
	int					m_nSitRegenTicks;
	int					m_nSitRegenLastLog;						
#endif

#ifndef	_SERVER
	int					m_ResDir;
	KNpcRes				m_DataRes;

	BYTE				m_btBloodAlpha[5];
	BYTE				m_btBloodTime[5];
	char				m_szBloodNo[5][16];
	
	BYTE				m_btTimeFrame[3];
	BYTE				m_btFrameSpr[3];
#endif

private:
	BOOL				WaitForFrame();
	BOOL				IsReachFrame(int nPercent);
	void				DoStand();
	void				OnStand();
	void				DoRevive();
	void				OnRevive();
	void				DoWait();
	void				OnWait();
	void				DoWalk();
	void				OnWalk();
	void				DoRun();
	void				OnRun(BOOL bAddSpeed = FALSE);		//
	void				DoSkill(int nX, int nY);
	void				UnDoSkill();		
	int					DoOrdinSkill(KSkill * pSkill, int nX, int nY);
	void				OnSkill();
	void				DoAttack();
	void				DoMagic();
	void				DoJump();
	BOOL				OnJump();
	void				DoSit();
	void				OnSit();
	void				DoHurt(int nHurtFrames = 0, int nX = 0, int nY = 0);
	void				OnHurt();
	void				DoDeath(int nMode = 0,int nAttacker = 0);	//chia exp team by kinnox;
	void				OnDeath();
	void				DoDefense();
	void				OnDefense();
	void				DoIdle();
	void				OnIdle();
	BOOL				DoManyAttack();
	void				OnManyAttack();
	BOOL				DoBlurAttack();
	BOOL				DoJumpAttack();
	BOOL				OnJumpAttack();
	BOOL				DoRunAttack();
	void				OnRunAttack();
	BOOL				CastMeleeSkill(KSkill * pSkill);
	void				OnPlayerTalk();
	void				DoSpecial1();
	void				OnSpecial1();
	void				DoSpecial2();
	void				OnSpecial2();
	void				DoSpecial3();
	void				OnSpecial3();
	void				DoSpecial4();
	void				OnSpecial4();
	
	BOOL				DoGlide(short nLenght);
	void				OnGlide();
	void				GlideTo(int nMpsX, int nMpsY, short nLenght);
	
	void				Goto(int nMpsX, int nMpsY);
	void				RunTo(int nMpsX, int nMpsY);
	void				JumpTo(int nMpsX, int nMpsY);
	void				ProcMove(int nSpeed);
	void				ProcJump(int nSpeed);
	BOOL				NewPath(int nMpsX, int nMpsY);
	BOOL				NewJump(int nMpsX, int nMpsY);
	int					NewGlide(int nMpsX, int nMpsY, int nMaxLenght);
	BOOL				CheckHitTarget(int nAR, int nDf, int nIngore = 0);
	
	void				AddStateInfo(int nID);
	void				AddStateSkillList(KStateNode * pNewNode);
	
#ifdef _SERVER
	void				LoseSingleItem(int nBelongPlayer);
	void				LoseMoney(int nBelongPlayer);
	void				PlayerDeadCreateMoneyObj(int nMoneyNum);	
	void				UpdateNpcStateInfo();
	//
	void				AddPoisonState(int nLauncher, int nDamage, int nTime, int nCycle);
#endif

#ifndef _SERVER
	void				HurtAutoMove();
	void				AutoFixXY(); //kinnox	
	void				AddStateSkillListClient(KStateNode * pNewNode);
	void				RemoveStateSkillList(BYTE* pNpcState);
	
#endif

public:
	friend class KNpcAttribModify;
	friend class KThiefSkill;
	KNpc();
	void				SetActiveFlag(BOOL bFlag) { m_bActivateFlag = bFlag; };
	void				DoPlayerTalk(char *);
	void				CheckTrap();
	void				Init();
	void				Remove();
	void				Activate();
	BOOL				IsPlayer();
	BOOL				IsAlive();		
	void				SetFightMode(BOOL bFightMode);
	void				TurnTo(int nIdx);
	void				SendCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				SendCommandRandMove(int x = 0, int y = 0, int z = 0);		
	void				ProcCommand(int nAI);
	ISkill* 			GetActiveSkill();
	BOOL				ProcessState();
	void				ProcStatus();
	void				ModifyAttrib(int nAttacker, void* pData);
	int					GetSkillLevel(int nSkillId);
	void				SetId(DWORD	dwID)	{ m_dwID = dwID;};
	BOOL				IsMatch(DWORD dwID)	{ return dwID == m_dwID; };	
	BOOL				Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast = FALSE);		
	void				SelfDamage(int nDamage);						
	void				Load(int nNpcSettingIdx, int nLevel, int nSeries);		//
	void				GetMpsPos(int * pPosX, int *pPosY);
	BOOL				SetActiveSkill(int nSkillIdx);
	void				SetAuraSkill(int nSkillID);
	void				SetCamp(int nCamp);
	void				SetCurrentCamp(int nCamp);
	void				RestoreCurrentCamp();
	void				SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime = -1, BOOL bRemove = FALSE);	//
	void				ClearStateSkillEffect();
	void				ReCalcStateEffect();
	void				ClearNormalState();
	void				SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum);
	void				AppendSkillEffect(DWORD uId, BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData);
	int					ModifyMissleLifeTime(int nLifeTime);
	int					ModifyMissleSpeed(int nSpeed);
	BOOL				ModifyMissleCollsion(BOOL bCollsion);
	void				RestoreNpcBaseInfo(); //Set Current_Data ;
	void				RestoreState();
	void				ClearNpcState();
	BOOL				SetPlayerIdx(int nIdx);
	void				DialogNpc(int nIndex);
	void				Revive();
	void				AddBaseLifeMax(int nLife);
	void				AddCurLifeMax(int nLife);
	void				AddBaseStaminaMax(int nStamina);
	void				AddCurStaminaMax(int nStamina);
	void				AddBaseManaMax(int nMana);
	void				AddCurManaMax(int nMana);
//	void				ResetLifeReplenish();		
//	void				CalcCurLifeMax();			
//	void				CalcCurStaminaMax();		
//	void				CalcCurManaMax();			
	void				CalcCurLifeReplenish();		
	void				SetSeries(int nSeries);
//	int					GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam);
//	int					SkillString2Id(char * szSkillString);
	void				GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel, int nSeries);		//
//	void				InitNpcLevelData(KTabFile * pTabFile, int nNpcTemplateId, KLuaScript * pLevelScript, int nLevel);
//	void				InitNpcBaseData(int nNpcTemplateId);
	void				SetPhysicsDamage(int nMinDamage, int nMaxDamage);	
	void				SetBaseAttackRating(int nAttackRating);					
	void				SetBaseDefence(int nDefence);							
//	void				SetBaseWalkSpeed(int nSpeed);							
//	void				SetBaseRunSpeed(int nSpeed);							
	int					GetCurActiveWeaponSkill();
	void				LoadDataFromTemplate(int nNpcTemplateId, int nLevel, int nSeries);		//
	
	inline int			GetMapX() const {	return m_MapX;	};
	inline int			GetMapY() const {	return m_MapY;	};
	inline int			GetMapZ() const {	return m_MapZ;	};
	inline int			GetOffX() const {	return m_OffX;	};
	inline int			GetOffY() const {	return m_OffY;	};
	void				MoveNpc(int nRegion, int nMapX, int nMapY, int nOffX, int nOffY, bool bFix = false);

	bool				SetPos(int nX, int nY); // moi fix

#ifndef _SERVER	
	BOOL				FindStateSkill(int nID);//AutoAI by kinnox;

#endif
	void				GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel, int nSeries);
	void				LoadFrameFromTemplate(int nNpcTemplateId, int nLevel, int nSeries);
	void				ReSetRes(BOOL bMask);
//fix script nil value by kinnox;
	BOOL				ExecuteScript(char * ScriptFileName, char * szFunName, int nParam = 0);		// string - num
	BOOL				ExecuteScript(char * ScriptFileName, char * szFunName, char * szParams);	// string - char
	BOOL				ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams);		// id - char
	BOOL				ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam);				// id - num

	BOOL				ExecuteScript2(char * ScriptFileName, char * szFunName, int nParam1 = 0, int nParam2 = 0);
	BOOL				ExecuteScript2(char * ScriptFileName, char * szFunName, char * szParams1, char * szParams2);
	BOOL				ExecuteScript2(DWORD dwScriptId, char * szFunName, char *  szParams1, char *  szParams2);
	BOOL				ExecuteScript2(DWORD dwScriptId,  char * szFunName, int nParam1, int nParam2);
//

	void				AutoDoSkill(int nSkillId, int nLevel, int nX, int nY);
	void				RandMove(int nX, int nY, BOOL bFlag = TRUE);

	void				SetTitleEffect(int SpeicalGraphicId);
	

#ifdef	_SERVER
	void				ExecuteRevive();
	BOOL				SendSyncData(int nClient);						
	void				NormalSync();									
	void				BroadCastState();
	void				BroadCastRevive(int nType);
	//
	inline int			GetPlayerIdx()
	{
		if (m_Kind != kind_player)
			return 0;
		return m_nPlayerIdx;
	};
	
	int					CalcDamage(int nAttacker, int nMin, int nMax, int nSeriesDamage, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bDoHurt = TRUE, 
									BOOL bReturn = FALSE, BOOL bFatally = FALSE, int nLifeStole = 0, int nManaStole = 0, int nStaminaStole = 0, int nIdSkills = 0);
	BOOL				ReceiveDamage(int nLauncher, BOOL bIsMelee, void *pData, BOOL bUseAR, BOOL bDoHurt, int nDameXG = 1);
	void				DeathPunish(int nMode, int nBelongPlayer);
	void				RestoreLiveData();								

	int					ChangeWorld(DWORD dwSubWorldID, int nX, int nY);	
	void				TobeExchangeServer(DWORD dwMapID, int nX, int nY);
	void				RestoreLife(){m_CurrentLife = m_CurrentLifeMax;	};
	void				RestoreMana(){m_CurrentMana = m_CurrentManaMax;};
	void				RestoreStamina(){m_CurrentStamina = m_CurrentStaminaMax; };
	void				SendDataToNearRegion(void* pBuffer, DWORD dwSize);
	int					DeathCalcPKValue(int nKiller);					

	BOOL				CheckPlayerAround(int nPlayerIdx);
	
	void				SendBloodNo(DWORD dwID, int nBloodNo);
//
// Ham tra ve mot Level ngau nhien dua vao phan tram truyen vao cac muc Level
	int 				getRandomLevel(const double percentages[], int numLevels);
// Ham kiem tra xem mot trang bi co cot level bang 10 hay khong
	int 				IsMagicAttribLuckyLevel(int nLucky);
//
	void				DropRateItem(int nCount, const char* pszFileName, int nUnknow, int nItemLevel, 
								int nItemSeries, int nBelongIdx);

	void				DropObj(int nBelongPlayer, int nObjId, char* lpScript, int nRandRate, int nMaxRandRate);

	void				SetTimer(DWORD nTime, int nTimeTaskId);
	void				CloseTimer();
	inline DWORD		GetTimerId() const { return m_TimerTask.GetTaskId(); };
	inline DWORD		GetTimerRestTime() const { return m_TimerTask.GetRestTime(); };
	void				SaveTimerTask();
	void				LoadTimerTask();

	BOOL				ExecuteTimerScript(char * ScriptFileName, char * szFunName, int nParam = 0);
	BOOL				ExecuteTimerScript(DWORD dwScriptId,  char * szFunName, int nParam);
	void				BroadCastPosition();
	
#endif

#ifndef _SERVER
	void				SetSleepMode(BOOL bSleep) { m_nSleepFlag = bSleep; m_DataRes.SetSleepState(bSleep);};
	void				SetNpcState(BYTE* pNpcState);
	void				RemoveRes();
	void				ProcNetCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				Paint();
	int					PaintInfo(int nHeightOffset, bool bSelect, int nFontSize = 12, DWORD dwBorderColor = 0xff000000);
	void				PaintNpcAuto(int nHeightOffset, bool bSelect, int nFontSize = 12, DWORD dwBorderColor = 0xff000000);
	
	void				PaintSeriesNpc(int nFontSize, int nHeightOff);
	void				PaintHorseState(int nFontSize, int nHeightOff);
	void				PaintRankInfo(int nHeightOffset, int nFontSize = 12, DWORD	dwBorderColor = 0xff000000);
	void				PaintTransLifeInfo(int nFontSize, int nHeightOff);
	int					PaintMantle(int nHeightOff, int nFontSize, int nMpsX, int nMpsY);
	void				PaintShopInfo(int nFontSize, int nHeightOff);
	
	int					PaintChat(int nHeightOffset);
	int					SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength);
	int					SetWarning(BOOL bWarning);//AutoAI by kinnox;
	int					PaintWarning(int nHeightOffset, bool bSelect);//AutoAI by kinnox;
	int					PaintLife(int nHeightOffset, bool bSelect);
	int					PaintMana(int nHeightOffset);
	void				DrawBorder();
	int					DrawMenuState(int n);
	BOOL				IsCanInput() { return m_ProcessAI; };
	void				SetMenuState(int nState, char *lpszSentence = NULL, int nLength = 0);	
	int					GetMenuState();				
	DWORD				SearchAroundID(DWORD dwID);
	void				SetSpecialSpr(char *lpszSprName);
	void				SetInstantSpr(int nNo);
	int					GetNormalNpcStandDir(int nFrame);
	KNpcRes*			GetNpcRes(){return &m_DataRes;};

	int GetNpcPate();
	int GetNpcPatePeopleInfo();

	void				ClearBlood(int nNo);
	void				SetBlood(int nNo);
	int					PaintBlood(int nHeightOffset);
	
	void				EditPos(bool bFlag);
	void				SendClientPos2Server();
	void				ExecuteStand();
	
	VOID				ClientGotoPos(INT nX, INT nY, INT nMode = 0);
#endif
	
};
#ifndef TOOLVERSION
extern KNpc Npc[MAX_NPC];
#else
extern CORE_API KNpc Npc[MAX_NPC];
#endif

#endif

