#ifndef __SKILLDEF_H__
#define __SKILLDEF_H__

#define MAX_SKILL  2000
#define MAX_SKILLLEVEL 64
#define MaxMissleDir	64
#define MAXSKILLLEVELSETTINGNUM	20		//
#define MAX_MISSLESTYLE  450
#define MISSLE_MIN_COLLISION_ZHEIGHT 0
#define MISSLE_MAX_COLLISION_ZHEIGHT 20	

// --
// MoveKind 运动类型
// --
enum eMissleMoveKind
{
	MISSLE_MMK_Stand,							
	MISSLE_MMK_Line,							
	MISSLE_MMK_Random,						
	MISSLE_MMK_Circle,							
	MISSLE_MMK_Helix,						
	MISSLE_MMK_Follow,							
	MISSLE_MMK_Motion,							
	MISSLE_MMK_Parabola,						
	MISSLE_MMK_SingleLine,						
	MISSLE_MMK_RollBack = 100,					
	MISSLE_MMK_Toss		,						
};

// --
// FollowKind 跟随类型	(主要是针对原地、环行与螺旋线飞行有意义)
// --
enum eMissleFollowKind
{
	MISSLE_MFK_None,							//	不跟随任何物件
	MISSLE_MFK_NPC,								//	跟随NPC或玩家
	MISSLE_MFK_Missle,							//	跟随子弹
};

#define	MAX_MISSLE_STATUS 4
enum eMissleStatus
{
	MS_DoWait,
	MS_DoFly,
	MS_DoVanish,
	MS_DoCollision,
};


enum eSkillLRInfo
{
	BothSkill,          //左右键皆可
	leftOnlySkill,		//左键
	RightOnlySkill,		//右键
	NoneSkill,			//都不可
};

//Skill.h

//技能发送者的类型
enum eGameActorType
{
	Actor_Npc,
	Actor_Obj,
	Actor_Missle,
	Actor_Sound,
	Actor_None,
};
enum eSkillLauncherType
{
	SKILL_SLT_Npc = 0,
	SKILL_SLT_Obj ,
	SKILL_SLT_Missle,
};


#ifndef _SERVER

struct	TOrginSkill
{
	int		nNpcIndex;				//	Npc的index
	DWORD	nSkillId;				//	发送的skillid
};

#endif


enum eSkillParamType
{
	SKILL_SPT_TargetIndex	= -1,
	SKILL_SPT_Direction		= -2,
};

//技能的类型
enum eSKillStyle
{
	SKILL_SS_Missles = 0,			//	子弹类		本技能用于发送子弹类
		SKILL_SS_Melee,
		SKILL_SS_InitiativeNpcState,	//	主动类		本技能用于改变当前Npc的主动状态
		SKILL_SS_PassivityNpcState,		//	被动类		本技能用于改变Npc的被动状态
		SKILL_SS_CreateNpc,				//	产生Npc类	本技能用于生成一个新的Npc
		SKILL_SS_BuildPoison,			//	炼毒类		本技能用于炼毒
		SKILL_SS_AddPoison,				//	加毒类		本技能用于给武器加毒性
		SKILL_SS_GetObjDirectly,		//	取物类		本技能用于隔空取物
		SKILL_SS_StrideObstacle ,		//	跨越类		本技能用于跨越障碍
		SKILL_SS_BodyToObject,			//	变物类		本技能用于将尸体变成宝箱
		SKILL_SS_Mining,				//	采矿类		本技能用于采矿随机生成矿石
		SKILL_SS_RepairWeapon,			//	修复类		本技能用于修复装备
		SKILL_SS_Capture,				//	捕捉类		本技能用于捕捉动物Npc
		SKILL_SS_Thief,					//	偷窃类
};


enum eMisslesForm
{
		SKILL_MF_Wall	= 0,			
		SKILL_MF_Line,					
		SKILL_MF_Spread,					
		SKILL_MF_Circle,				
		SKILL_MF_Random,				
		SKILL_MF_Zone,					
		SKILL_MF_AtTarget,				
		SKILL_MF_AtFirer,				
		SKILL_MF_COUNT,
};

enum eMeleeForm
{
	Melee_AttackWithBlur = SKILL_MF_COUNT,
	Melee_Jump,
	Melee_JumpAndAttack,
	Melee_RunAndAttack,
	Melee_ManyAttack,
	
	Melee_Move,
	
};


enum eSKillCostType
{
	SKILL_SCT_MANA		= 1,
		SKILL_SCT_LIFE		= 2,
		SKILL_SCT_STAMINA	= 8,
		SKILL_SCT_MONEY		= 16,
};

enum eMisslesGenerateStyle
{
	SKILL_MGS_NULL		= 0,
		SKILL_MGS_SAMETIME	,    //同时
		SKILL_MGS_ORDER		,	 //按顺序
		SKILL_MGS_RANDONORDER,
		SKILL_MGS_RANDONSAME,
		SKILL_MGS_CENTEREXTENDLINE,  //由中间向两周扩散
};

typedef struct 
{
	int dx;
	int dy;
}TCollisionOffset;

typedef struct 
{
	int nRegion;
	int nMapX;
	int nMapY;
}
TMisslePos;

typedef struct 
{
	TCollisionOffset m_Offset [4];
}
TCollisionMatrix;
extern TCollisionMatrix g_CollisionMatrix[64];


typedef struct 
{
	int nLauncher;	
	DWORD dwLauncherID;			
	eSkillLauncherType eLauncherType; //发送者，一般为Npc


	int nParent;
	eSkillLauncherType eParentType;	  //母	 
	DWORD dwParentID;

	int nParam1;
	int nParam2;
	int nWaitTime;
	int nTargetId;
	DWORD dwTargetNpcID;
}
TOrdinSkillParam, * LPOrdinSkillParam;

#endif //__SKILLDEF_H__
