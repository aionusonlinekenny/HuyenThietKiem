#ifndef GAMEDATADEF_H
#define GAMEDATADEF_H

#include "CoreObjGenreDef.h"
#include "CoreUseNameDef.h"

#define		ITEM_VERSION						1

#define		MAX_TEAM_MEMBER						7
#define		MAX_SENTENCE_LENGTH					512
#define 	NORMAL_FONTSIZE						12
#define		FILE_NAME_LENGTH					80
#define		PLAYER_PICKUP_CLIENT_DISTANCE		128//63
#define		defMAX_EXEC_OBJ_SCRIPT_DISTANCE		200
#define		defMAX_PLAYER_SEND_MOVE_FRAME		2
#define		PLAYER_PICKUP_SERVER_DISTANCE		40000
#define		MAX_INT								0x7fffffff

#define		MAX_PERCENT							100
#define		ROLE_NO								2
#define		PLAYER_MALE_NPCTEMPLATEID			-1
#define		PLAYER_FEMALE_NPCTEMPLATEID			-2

#define		PLAYER_SHARE_EXP_DISTANCE			1024 //768 fix by kinnox;
//// --
#define		MAX_MSG_LENGTH						128
#define		MAX_CHATITEM_INFO_LENGTH			80
#define		defPLAYER_NAME_MAX_LENGTH			16
//// --
#define REGION_PIXEL_WIDTH						512
#define REGION_PIXEL_HEIGHT						1024
#define REGION_CELL_SIZE_X						32
#define REGION_CELL_SIZE_Y						32
#define REGION_CELL_WIDTH	(REGION_PIXEL_WIDTH/REGION_CELL_SIZE_X)
#define REGION_CELL_HEIGHT	(REGION_PIXEL_HEIGHT/REGION_CELL_SIZE_Y)
//// --
#define		MAX_DEATH_PUNISH_PK_VALUE			10
#define		MAX_TRANSLIFE_VALUE					5
#define		MAX_ITEM_LEVEL						10 //Kenny Nguyen
#define		MAX_REUPTE_VALUE					10000
#define		MAX_FUYUAN_VALUE					10000
//// --
#define		TV_BATTLE_GROUP						7
#define		TV_LIXIANEXPRESS					2024
#define		TV_LOCKSTATE						2023
#define		TV_TRANSLIFE						2022
#define		TV_REPUTE							2021
#define		TV_FUYUAN							2020
#ifdef _SERVER
#define		TV_THIENSOWNBAOLO					2019
#define		TV_QUEHOATUU						2018
#define		TV_PWBOX							2017
#define		TV_ACTIVE_AUTO						2016
#define		TV_ENHANCE_EXP						2015
#define		TV_ENHANCE_SKILL					2014
#define		TV_CALCDAMAGE						2012
#endif
#define		TV_SJPOINT							2013
#define		TV_FPOINT							2011
//// --
#ifndef _SERVER
#define		defMAX_GOODS_PER_PAGE				15
#endif
#define		defMAX_SHOPITEM_PER_UPDATE			6
//// --
#ifdef _SERVER
#define		GMS_EXPRATE							49
#endif
//// --
#ifdef _SERVER
#define	defMAX_LOGIN_SCRIPT_LOAD_TIME			18
#define	defMAX_SLEEP_TIME						3600
#define	defMAX_TIMER_SERVER_LOOP				1080
#endif
//// --
#define		MAX_SERIES							5
//// --
#define	BUFF_SKILL_MISSLE_ID_1					20
#define	BUFF_SKILL_MISSLE_ID_2					273
//// --
#define	BUY_SELL_SCALE		4
//// --
#define		MAX_TIME_COUNTID			32
//// --

//// --
//AutoAI by kinnox;
//// -- 
#define		defMAX_ARRAY_AUTO				50
#define		defAUTO_TIME_LAG				50
#define		defAUTO_COUNT_LAG				75 //150
#define		defAUTO_TIME_RESET_LAG			10000 //7000
#define		defAUTO_TIME_SKIP				300 //7000
#define		TIME_RIDE						5000  // 5 seconds cooldown for riding/dismounting
#define		defMAX_ARRAY_STATESKILL			3
#define		defMAX_ARRAY_AURA				2
#define		defMAX_ARRAY_OPTION				3
#define		defMAX_AUTO_MOVEMPSL			15
#define		defMAX_EAT_SPACETIME			1500
#define		defMAX_LIST_PARTY				15
#define		defINTERVAL_AUTO				250
#define		MAX_ITEM_MAGICATTRIB			6
#define		defMAX_AUTO_FILTERL				40
#define		defMAX_ARRAY_FIGHT				10
//TrembleItem by kinnox;
#define		MAX_PART_BUILD						9		// Max so part item kham nam
enum UI_BUILDITEM_POSITION
{
	UIEP_BUILDITEM1 = 0,
	UIEP_BUILDITEM2 = 1,
	UIEP_BUILDITEM3 = 2,
	UIEP_BUILDITEM4 = 3,
	UIEP_BUILDITEM5 = 4,
	UIEP_BUILDITEM6 = 5,
	UIEP_BUILDITEM7 = 6,
	UIEP_BUILDITEM8 = 7,
	UIEP_BUILDITEM9 = 8,	
};

enum BUILD_PART
{
	itempart_build1 = 0,
	itempart_build2,
	itempart_build3,
	itempart_build4,
	itempart_build5,
	itempart_build6,
	itempart_build7,
	itempart_build8,
	itempart_build9,
	buildpart_num,
};

enum SPECIALITEM_TASK
{
	SP_CHANKHUYET	= 0,//bua chu' chua lam
	SP_CHANTUYET	= 1,//bua chu' chua lam
	SP_TUTHUYTINH 	= 17,
	SP_LUCTHUYTINH 	= 18,
	SP_LAMTHUYTINH 	= 16,

};
//
enum LockPlayerInfo //LockPlayer by kinnox;
{
	CREATE_PW,
	LOGIN_PW,
	CHANGE_PW,
	LOCK_PW,
	DEL_PW,
	LOCK_NUM,
};

enum PickUpItem
{
	enumPickUpEarn,
	enumPickEventItem,
	enumPickItemColor,
	enumPickUpAll,
	enumPickNum,
};

enum
{
	enumRoomNotEnough1,
	enumRoomNotEnough2,
	enumRoomNotEnough4,
	enumRoomNotEnough6,
};	

enum ITEMGENRE
{
	item_equip = 0,
	item_purpleequip,
	item_goldequip,
	item_platinaequip,
	item_brokenequip,
	item_medicine,
	item_task,
	item_script,
	item_mine,	
	item_number,			
};

enum ITEM_PART
{
	itempart_head = 0,	
	itempart_body,		
	itempart_belt,		
	itempart_weapon,	
	itempart_foot,
	itempart_cuff,
	itempart_amulet,
	itempart_ring1,
	itempart_ring2,
	itempart_pendant,
	itempart_horse,
	itempart_mask,
	itempart_mantle,
	itempart_signet,
	itempart_shipin,
	itempart_num,
};

//==================================
//
//==================================
enum EQUIPDETAILTYPE
{
	equip_meleeweapon = 0,
	equip_rangeweapon,
	equip_armor,
	equip_ring,
	equip_amulet,
	equip_boots,
	equip_belt,
	equip_helm,
	equip_cuff,
	equip_pendant,
	equip_horse,
	equip_mask,
	equip_mantle,
	equip_signet,
	equip_shipin,
	equip_detailnum,
};

typedef struct
{
	int		nIdx;
	int		nPlace;
	int		nX;
	int		nY;
} PlayerItem;

enum INVENTORY_ROOM
{
	room_equipment = 0,
	room_repository,
	//load 3 storebox by kinnox;
	room_repository1,
	room_repository2,
	room_repository3,
	room_repository4,
	room_repository5,
	room_expandtory1,//Expandbox by kinnox;
	room_givebox,//GiveBox by kinnox;	
	//end code	
	room_trade,			
	room_tradeback,		
	room_trade1,		
	room_immediacy,
	room_builditem,		//TrembleItem by kinnox;
	room_num,			
};

enum ITEM_POSITION
{
	pos_hand = 1,		
	pos_equip,			
	pos_equiproom,		
	pos_repositoryroom,	
	//load 3 storebox by kinnox;
	pos_repositoryroom1,	
	pos_repositoryroom2,	
	pos_repositoryroom3,	
	pos_repositoryroom4,	
	pos_repositoryroom5,
	pos_expandtoryroom1,//Expandbox by kinnox;
	pos_givebox,//GiveBox by kinnox;	
	//	
	pos_traderoom,		
	pos_trade1,			
	pos_immediacy,
	pos_builditem,	//ID 15	//TrembleItem by kinnox;
};

//load 3 storebox by kinnox;
enum REPOSI_BOX
{
	REPOSITORY_ONE,
	REPOSITORY_TWO,
	REPOSITORY_THREE,
	REPOSITORY_FOUR,
	REPOSITORY_FIVE,
	REPOSITORY_NUM,
};
//end code
enum EXPAND_BOX//ExPandBox by kinnox;
{
	EXPANDTORY_ONE,
	EXPANDTORY_NUM,
};
#define		MAX_HAND_ITEM				1
#define		EQUIPMENT_ROOM_WIDTH		6
#define		EQUIPMENT_ROOM_HEIGHT		10
#define		MAX_EQUIPMENT_ITEM			(EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT)

#define		REPOSITORY_ROOM_WIDTH		6
#define		REPOSITORY_ROOM_HEIGHT		10
#define		MAX_REPOSITORY_ITEM			(REPOSITORY_ROOM_WIDTH * REPOSITORY_ROOM_HEIGHT)


#define		TRADE_ROOM_WIDTH			10
#define		TRADE_ROOM_HEIGHT			4
#define		MAX_TRADE_ITEM				(TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
#define		MAX_TRADE1_ITEM				MAX_TRADE_ITEM
#define		IMMEDIACY_ROOM_WIDTH		9
#define		IMMEDIACY_ROOM_HEIGHT		1
#define		MAX_IMMEDIACY_ITEM			(IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)
#define		EXPANDTORY_ROOM_WIDTH		6	//Expandbox by kinnox;
#define		EXPANDTORY_ROOM_HEIGHT		10	//Expandbox by kinnox;
#define		MAX_EXPANDTORY_ITEM			(EXPANDTORY_ROOM_WIDTH * EXPANDTORY_ROOM_HEIGHT)//Expandbox by kinnox;
#define		GIVEBOX_ROOM_WIDTH			6//GiveBox by kinnox;
#define		GIVEBOX_ROOM_HEIGHT			4//GiveBox by kinnox;
#define		MAX_GIVEBOX_ITEM			(GIVEBOX_ROOM_WIDTH * GIVEBOX_ROOM_HEIGHT)//GiveBox by kinnox;
#define		MAX_PLAYER_ITEM_RESERVED	32
#define		MAX_PLAYER_ITEM				(MAX_GIVEBOX_ITEM + MAX_EXPANDTORY_ITEM + MAX_EQUIPMENT_ITEM + (MAX_REPOSITORY_ITEM*6) + MAX_TRADE_ITEM + MAX_TRADE1_ITEM + MAX_IMMEDIACY_ITEM + itempart_num + MAX_HAND_ITEM + MAX_PLAYER_ITEM_RESERVED )


#define		REMOTE_REVIVE_TYPE			0
#define		LOCAL_REVIVE_TYPE			1

#define		MAX_MELEE_WEAPON			6
#define		MAX_RANGE_WEAPON			3
#define		MAX_ARMOR					14
#define		MAX_HELM					14
#define		MAX_RING					1
#define		MAX_BELT					2
#define		MAX_PENDANT					2
#define		MAX_AMULET					2
#define		MAX_CUFF					2
#define		MAX_BOOT					4
#define		MAX_HORSE					6

#define		MAX_NPC_TYPE	300
#define		MAX_NPC_LEVEL	100


#define		MAX_NPC_DIR		64
#define		MAX_WEAPON		MAX_MELEE_WEAPON + MAX_RANGE_WEAPON
#define		MAX_SKILL_STATE				24
#define		MAX_NPC_HEIGHT	128
#define		MAX_RESIST		95
#define		MAX_HIT_PERCENT	95
#define		MIN_HIT_PERCENT	5
#define		MAX_NPC_RECORDER_STATE		16
//Kenny Test tra ve nhu cu 09 27 25
#define     MAX_STATE_EFFECT_ID         200


#define		PLAYER_MOVE_DO_NOT_MANAGE_DISTANCE	5

#define	NORMAL_NPC_PART_NO		5

#ifndef _SERVER
#define		C_REGION_X(x)	(LOWORD(SubWorld[0].m_Region[ (x) ].m_RegionID))
#define		C_REGION_Y(y)	(HIWORD(SubWorld[0].m_Region[ (y) ].m_RegionID))
#endif

enum
{
	CHAT_S_STOP = 0,						// ????????
	CHAT_S_SCREEN,							// ?????L???????
	CHAT_S_SINGLE,							// ????????????????
	CHAT_S_TEAM,							// ?????????????
	CHAT_S_NUM,								// ????????????
};

enum PLAYER_INSTANT_STATE
{
	enumINSTANT_STATE_LEVELUP = 0,
	enumINSTANT_STATE_REVIVE,
	enumINSTANT_STATE_CREATE_TEAM,
	enumINSTANT_STATE_LOGIN,
	enumINSTANT_STATE_NUM,
};

enum CHAT_STATUS
{
	CHAT_S_ONLINE = 0,		//????
	CHAT_S_BUSY,			//æµ
	CHAT_S_HIDE,			//????
	CHAT_S_LEAVE,			//??
	CHAT_S_DISCONNECT,		//????
};

// ?????ö???????????(by zroc)
enum OBJ_ATTRIBYTE_TYPE
{
	series_metal = 0,		//	???
	series_wood,			//	l?
	series_water,			//	??
	series_fire,			//	???
	series_earth,			//	???
	series_num,
};

enum OBJ_GENDER
{
	OBJ_G_MALE	= 0,	//????????
	OBJ_G_FEMALE,		//??g?U??
};

enum NPCCAMP
{
	camp_begin,				
	camp_justice,			
	camp_evil,				
	camp_balance,			
	camp_free,				
	camp_animal,			
	camp_event,				
	camp_num,				
};

enum NPCFIGHT 
{
	fight_none = 0,
	fight_active,
	fight_num,
};

enum ITEM_IN_ENVIRO_PROP
{
	IIEP_NORMAL = 0,
	IIEP_NOT_USEABLE,
	IIEP_SPECIAL,
	IIEP_PRICED,
};

#define	GOD_MAX_OBJ_TITLE_LEN	1536
#define	GOD_MAX_OBJ_PROP_LEN	516
#define	GOD_MAX_OBJ_DESC_LEN	4096

//==================================
//
//==================================
struct KGameObjDesc
{
	char	szTitle[GOD_MAX_OBJ_TITLE_LEN];
	char	szProp[GOD_MAX_OBJ_PROP_LEN];
	char	szDesc[GOD_MAX_OBJ_DESC_LEN];
};

//==================================
//	
//==================================
struct KUiAnswer
{
	char	AnswerText[256];	
	int		AnswerLen;			
};

//==================================
//	
//==================================
struct KUiQuestionAndAnswer
{
	char		Question[512];	
	int			QuestionLen;	
	int			AnswerCount;	
	KUiAnswer	Answer[1];		
};

//==================================
//	
//==================================
struct KUiAskClient
{
	char	Title[16];
};

//==================================
//	
//==================================
struct KUiAskClientNumber : public KUiAskClient
{
	int		MinValue;
	int		MaxValue;
};

//==================================
//	
//==================================
struct KUiAskClientString : public KUiAskClient
{
	char	Init[16];
	BYTE	MinLen;
	BYTE	MaxLen;
};

//==================================
//	
//==================================
struct KUiGameObject
{
	unsigned int uGenre;	
	unsigned int uId;		
};

//==================================
//
//==================================
struct KUiStateList
{
	short	nSkillID;
	int		nValue; 
	int		nTime;		
};
//==================================
//
//==================================
struct KUiRegion
{
	int		h;
	int		v;
	int		Width;
	int		Height;
};
//==================================
//
//==================================
enum UIOBJECT_CONTAINER
{
	UOC_IN_HAND	= 1,		
	UOC_GAMESPACE,			
	UOC_IMMEDIA_ITEM,		
	UOC_IMMEDIA_SKILL,
	UOC_ITEM_TAKE_WITH,		
	UOC_TO_BE_TRADE,		
	UOC_OTHER_TO_BE_TRADE,	
	UOC_EQUIPTMENT,			
	UOC_NPC_SHOP,			
	UOC_STORE_BOX,
	//load 3 storebox by kinnox;
	UOC_STORE_BOX1,
	UOC_STORE_BOX2,
	UOC_STORE_BOX3,
	UOC_STORE_BOX4,
	UOC_STORE_BOX5,
	UOC_EXPAND_BOX1,//Expandbox by kinnox;
	UOC_GIVE_BOX,//GiveBox by kinnox;	
	//endcode;	
	UOC_SKILL_LIST,			
	UOC_SKILL_TREE,
	UOC_BUILD_ITEM,//TrembleItem by kinnox;
};

//==================================
//
//==================================
enum UI_TRADE_OPER_DATA
{
	UTOD_IS_WILLING,
	UTOD_IS_LOCKED,
	UTOD_IS_TRADING,
	UTOD_IS_OTHER_LOCKED,
};

//==================================
//
//==================================
struct KUiItemBuySelInfo
{
	char			szItemName[64];
	int				nPrice;
	BYTE			btDisCountPrice;
	BYTE			btPriceType;
	BOOL			bHotSell;
	int				btStack;
	
};
//==================================
//TimeBox by kinnox;
//==================================
struct KUiTimeBoxInfo           
{
	char	szTitle[32];
	int		nTime;
	int		nIndexNpc;
	char	szAction[64];
};
//==================================
//GiveBox by kinnox;
//==================================
struct KUiGiveBoxStruc           
{
	char	szTitle[32];
	char	szInitString[256];
	char	szAction1[64];
};
//==================================
//Notification by kinnox;
//==================================
struct KUiNotifiStruc          
{
	char	szTitle[32];
	char	szInitString[256];
};
//==================================
//Reward by kinnox;
//==================================
struct KUiReWardBox           
{
	char	szTitle[64];
	char	szFunction1[32];
	char	szFunction2[32];
	char	szFunction3[32];
};
//==================================
//
//==================================
struct KUiObjAtRegion
{
	KUiGameObject	Obj;
	KUiRegion		Region;
};
struct KUiObjAtContRegion : public KUiObjAtRegion
{
	union
	{
		UIOBJECT_CONTAINER	eContainer; 
		int					nContainer;
	};
};
//==================================
//
//==================================
struct KTime
{
	BYTE btHour;
	BYTE btMin;
	BYTE btDay;
	BYTE btMonth;
	WORD wYear;
};

//==================================
//
//==================================
struct KUiChatItemParam
{	
	BYTE	m_ColorID;
	char	m_Genre;		 
	short	m_DetailType;	
	WORD	m_Record;
	char	m_Series;	
	BYTE	m_Level;
	int		m_Luck;
	BYTE	m_MagicLevel[6];
	DWORD	m_RandomSeed;
	WORD	m_Version;
	BYTE	m_StackCount;
	DWORD	m_BindState;
	DWORD	m_ExpiredTime;
};
//==================================
//
//==================================
struct KUiChatItemDesc
{
	char	szItemDesc[GOD_MAX_OBJ_TITLE_LEN];
	char	szImageIcon[MAX_PATH];
	int		nWidth;
	int		nHeight;
};

struct KUiMsgParam
{
	unsigned char	eGenre;
	unsigned char	cChatPrefixLen;
	unsigned short	nMsgLength;
	char			szName[32];
#define	CHAT_MSG_PREFIX_MAX_LEN	16
	unsigned char	cChatPrefix[CHAT_MSG_PREFIX_MAX_LEN];
};

struct KUiInformationParam
{
	char	sInformation[512];
	char	sConfirmText[64];
	short	nInforLen;
	bool	bNeedConfirmNotify;
	bool	bReserved;
};

enum PLAYER_ACTION_LIST
{
	PA_NONE = 0,	//?????
	PA_RUN  = 0x01,	//??
	PA_SIT  = 0x02,	//????
	PA_RIDE = 0x04,	//??????
};

//==================================
//	?????????
//==================================
enum SYS_MESSAGE_TYPE
{
	SMT_NORMAL = 0,	//???µ????????
	SMT_SYSTEM,		//???????????
	SMT_PLAYER,		//??????
	SMT_TEAM,		//??????
	SMT_FRIEND,		//??????????
	SMT_MISSION,	//???????
	SMT_CLIQUE,		//???????
};

//==================================
//	???????????
//==================================
enum SYS_MESSAGE_CONFIRM_TYPE
{
	SMCT_NONE,				//?????????????????????????????
	SMCT_CLICK,				//????????????????
	SMCT_MSG_BOX,			//??????????????
	SMCT_UI_ATTRIBUTE,		//?????????
	SMCT_UI_SKILLS,			//?????????
	SMCT_UI_ATTRIBUTE_SKILLS,//??????????????
	SMCT_UI_TEAM_INVITE,	//???????????????????,
	//						pParamBuf ???h??KUiPlayerItem??????????????????(???)
	SMCT_UI_TEAM_APPLY,		//???????????????????,
	//						pParamBuf ???h??KUiPlayerItem??????????????????
	SMCT_UI_TEAM,			//????????????
	SMCT_UI_INTERVIEW,		//????????????,
	//						pParamBuf ???h??KUiPlayerItem????????????????????l???
	SMCT_UI_FRIEND_INVITE,	//????????????????????
	//						pParamBuf ???h??KUiPlayerItem???????????????????????????
	SMCT_UI_TRADE,			//????????????????,
	//						pParamBuf ???h??KUiPlayerItem???????????????????????????
	SMCT_DISCONNECT,		
	SMCT_UI_TONG_JOIN_APPLY,
};

//==================================
//
//==================================
struct KSystemMessage
{
	char			szMessage[MAX_MSG_LENGTH];	
	unsigned int	uReservedForUi;
	unsigned char	eType;
	unsigned char	byConfirmType;
	unsigned char	byPriority;	
	unsigned char	byParamSize;
};

//==================================
//	
//==================================
struct KUiChatChannel
{
	int			 nChannelNo;
	unsigned int uChannelId;
	union
	{
		int		 nChannelIndex;
		int		 nIsSubscibed;
	};
	char		 cTitle[32];
};

//==================================
//	????????h??????????
//==================================
struct KUiChatGroupInfo
{
	char	szTitle[32];	//?????????
	int		nNumFriend;		//???????????
};

//==================================
//	
//==================================
struct KUiChatMessage
{
	unsigned int uColor;
	short	nContentLen;
	char	szContent[256];
};

//==================================
//
//==================================
struct KUiPlayerBaseInfo
{
	char	Name[32];
	char	Title[32];
	int		nCurFaction;
	short	nRankInWorld;
	BYTE	btCurBattleTeam;
	DWORD	dwCurTong;	
	DWORD	dwTongMasterID;
};

//==================================
//	
//==================================
struct KUiPlayerRuntimeInfo
{
	int		nLifeFull;			
	int		nLife;				
	int		nManaFull;			
	int		nMana;				
	int		nStaminaFull;		
	int		nStamina;						
	int		nExperienceFull;	
	int		nExperience;		
	int		nCurLevelExperience;

	unsigned char	byActionDisable;
	unsigned char	byAction;
};

//==================================
//	
//==================================
enum UI_PLAYER_ATTRIBUTE
{
	UIPA_STRENGTH = 0,			
	UIPA_DEXTERITY,				
	UIPA_VITALITY,				
	UIPA_ENERGY,				
};

//==================================
//	
//==================================
struct KUiPlayerAttribute
{
	int		nMoney;				
	int		nLevel;				
	char	StatusDesc[16];		

	int		nBARemainPoint;
	int		nStrength;			
	int		nDexterity;			
	int		nVitality;			
	int		nEnergy;			

	int		nKillMAX;			
	int		nKillMIN;			
	int		nRightKillMax;		
	int		nRightKillMin;		

	int		nAttack;			
	int		nDefence;			
	int		nMoveSpeed;	
	short	nAttackSpeed;
	short	nCastSpeed;

	int		nPhyDef;			
	int		nCoolDef;			
	int		nLightDef;			
	int		nFireDef;			
	int		nPoisonDef;

	char	cPKValue;
	short	nRepute;
	short	nFuyuan;
	char	cTransLife;
};

//==================================
//	
//==================================
struct KUiPlayerImmedItemSkill
{
	KUiGameObject	ImmediaItem[9];	
	KUiGameObject	IMmediaSkill[2];
};

//==================================
//	
//==================================
enum UI_EQUIPMENT_POSITION
{
	UIEP_HEAD			= 0,		
	UIEP_HAND			= 1,		
	UIEP_NECK			= 2,		
	UIEP_FINESSE		= 3,	
	UIEP_BODY			= 4,		
	UIEP_WAIST			= 5,		
	UIEP_FINGER1		= 6,	
	UIEP_FINGER2		= 7,	
	UIEP_WAIST_DECOR	= 8,
	UIEP_FOOT			= 9,		
	UIEP_HORSE			= 10,
	UIEP_MASK			= 11,
	UIEP_MANTLE			= 12,
	UIEP_SIGNET			= 13,
	UIEP_SHIPIN			= 14,
};

//==================================
//
//==================================
enum UI_TREMBLE_POSITION
{
	UIEP_BLUEITEM	= 0,		
	UIEP_GEMLEVEL	= 1,		
	UIEP_GEMSPIRIT	= 2,		
	UIEP_GEMMETAL	= 3,	
	UIEP_GEMWOOD	= 4,		
	UIEP_GEMWATER	= 5,		
	UIEP_GEMFIRE	= 6,	
	UIEP_GEMEARTH	= 7,	
};
//==================================
//
//==================================
enum UI_FOUNDRY_POSITION
{
	UIFP_BOX1		= 0,		
	UIFP_BOX2		= 1,		
	UIFP_BOX3		= 2,
	UIFP_BOX4		= 3,
};
//==================================
//	
//==================================
struct KUiPlayerLiveSkillBase
{
	int		nRemainPoint;			
	int		nLiveExperience;		
	int		nLiveExperienceFull;
};

//==================================
//	
//==================================
struct KUiSkillData : public KUiGameObject
{
	union
	{
		int		nLevel;
		int		nData;
	};
	char	cPointAdd;
};

//==================================
//	
//==================================
struct KUiExecScriptInfo
{	
	char	m_szFunc[32];
	int		m_nParam;	
};

//==================================
//	
//==================================
#define	PLAYER_TEAM_MAX_MEMBER	8

//==================================
//	
//==================================
struct KUiPlayerLeaderShip
{
	int		nLeaderShipLevel;
	int		nLeaderShipExperience;
	int		nLeaderShipExperienceFull;
};

//==================================
//
//==================================
struct KUiPlayerItem
{
	char			Name[32];
	unsigned int	uId;
	int				nIndex;
	int				nData;
	int				nAvatarID;//Player Avatar by kinnox;	
	int				m_Level;
    int				m_SM;
    int				m_TP;
    int				m_HP;
    int				m_MHP;
};

//==================================
//
//==================================
struct KUiTeamItem
{
	KUiPlayerItem	Leader;
};

//==================================
//
//==================================
struct KUiPlayerTeam
{
	bool			bTeamLeader;
	char			cNumMember;
	char			cNumTojoin;
	bool			bOpened;
	int				nTeamServerID;
	int				nCaptainPower;
	int 			nNpcId;
};

//==================================
//
//==================================
#ifndef MAGICATTRIB
#define MAGICATTRIB
struct KMagicAttrib
{
	int				nAttribType;
	int				nValue[3];
	short			nMin;		// Min value of attribute (TamLTM custom)
	short			nMax;		// Max value of attribute (TamLTM custom)
	KMagicAttrib(){nValue[0] = nValue[1] = nValue[2] = nAttribType = nMin = nMax = 0;};
};
#else
struct KMagicAttrib;
#endif

//UiMiniBuff by kinnox;
struct KStateControl
{
	int				nSkillId;
	int				nLeftTime;
};

struct KMapPos
{
	int		nSubWorld;
	int		nRegion;
	int		nMapX;
	int		nMapY;
	int		nOffX;
	int		nOffY;
};

//==================================
//
//==================================
enum OPTIONS_LIST
{
	OPTION_PERSPECTIVE,
	OPTION_DYNALIGHT,
	OPTION_MUSIC_VALUE,
	OPTION_SOUND_VALUE,
	OPTION_BRIGHTNESS,
	OPTION_WEATHER,
};

//==================================
//	
//==================================
struct KUiSceneTimeInfo
{
	char	szSceneName[32];
	int		nSceneId;				
	int		nScenePos0;				
	int		nScenePos1;				
	int		nGameSpaceTime;
};
//==================================
//PaintFindPos by kinnox;
//==================================
struct KUiSceneCalcPos
{
	int		nScenePos0;				
	int		nScenePos1;				
};
//==================================
//
//==================================
struct KPosition3
{
	int nX;
	int nY;
	int nZ;
};

struct KLightInfo
{
	KPosition3 oPosition;
	DWORD dwColor;
	long  nRadius;
};


enum SCENE_PLACE_MAP_ELEM
{ 
	SCENE_PLACE_MAP_ELEM_NONE		= 0x00,
	SCENE_PLACE_MAP_ELEM_PIC		= 0x01,
	SCENE_PLACE_MAP_ELEM_CHARACTER	= 0x02,
	SCENE_PLACE_MAP_ELEM_PARTNER	= 0x04,
};

struct KSceneMapInfo
{
	int	nScallH;
	int nScallV;
	int	nFocusMinH;
	int nFocusMinV;
	int nFocusMaxH;
	int nFocusMaxV;
	int nOrigFocusH;
	int nOrigFocusV;
	int nFocusOffsetH;
	int nFocusOffsetV;
};

enum NPC_RELATION
{
	relation_none	= 1,
	relation_self	= 2,
	relation_ally	= 4,
	relation_enemy	= 8,
	relation_dialog	= 16,
	relation_all	= relation_none | relation_ally | relation_enemy | relation_self | relation_dialog,	
	relation_num,
};

enum NPCKIND
{
	kind_normal = 0	,		
	kind_player,
	kind_partner,
	kind_dialoger,
	kind_bird,
	kind_mouse,
    kind_num
};

enum
{
	Obj_Kind_MapObj = 0,
	Obj_Kind_Body,
	Obj_Kind_Box,
	Obj_Kind_Item,
	Obj_Kind_Money,
	Obj_Kind_LoopSound,
	Obj_Kind_RandSound,
	Obj_Kind_Light,
	Obj_Kind_Door,
	Obj_Kind_Trap,
	Obj_Kind_Prop,
	Obj_Kind_Task,
	Obj_Kind_Num,
};

enum PLAYER_BRIEF_PROP
{
	PBP_LEVEL = 1,
	PBP_FACTION,
	PBP_CLIQUE,
};

enum NEWS_MESSAGE_TYPE
{
	NEWSMESSAGE_NORMAL,
						
	NEWSMESSAGE_COUNTING,
							
	NEWSMESSAGE_TIMEEND,
						
};

#define MAX_MESSAGE_LENGTH 512

struct KNewsMessage
{
	int		nType;
	char	sMsg[MAX_MESSAGE_LENGTH];
	int		nMsgLen;
};

struct KRankIndex
{
	bool			bValueAppened;
	bool			bSortFlag;
	unsigned short	usIndexId;
};

#define MAX_RANK_MESSAGE_STRING_LENGTH 128

struct KRankMessage
{
	char szMsg[MAX_RANK_MESSAGE_STRING_LENGTH];
	unsigned short		usMsgLen;				
	short				cSortFlag;			
	int					nValueAppend;	
	BYTE				btTransLife;
};

struct KMissionRecord
{
	char			sContent[256];
	int				nContentLen;
	unsigned int	uValue;
};


#define		defTONG_MAX_DIRECTOR				7
#define		defTONG_MAX_MANAGER					56
#define		defTONG_ONE_PAGE_MAX_NUM			7

#define		defTONG_STR_LENGTH					32

#define		defTONG_NAME_MAX_LENGTH				8
#define		defTONG_TITLE_MAX_LENGTH			10

enum TONG_MEMBER_FIGURE
{
	enumTONG_FIGURE_MEMBER,				
	enumTONG_FIGURE_MANAGER,			
	enumTONG_FIGURE_DIRECTOR,			
	enumTONG_FIGURE_MASTER,			
	enumTONG_FIGURE_NUM,
};

enum
{
	enumTONG_APPLY_INFO_ID_SELF,		
	enumTONG_APPLY_INFO_ID_MASTER,		
	enumTONG_APPLY_INFO_ID_DIRECTOR,	
	enumTONG_APPLY_INFO_ID_MANAGER,		
	enumTONG_APPLY_INFO_ID_MEMBER,	
	enumTONG_APPLY_INFO_ID_ONE,		
	enumTONG_APPLY_INFO_ID_TONG_HEAD,
	enumTONG_APPLY_INFO_ID_NUM,
};

//=============================
//
//=============================
enum
{
	enumTONG_CREATE_ERROR_ID1,
	enumTONG_CREATE_ERROR_ID2,	
	enumTONG_CREATE_ERROR_ID3,	
	enumTONG_CREATE_ERROR_ID4,	
	enumTONG_CREATE_ERROR_ID5,	
	enumTONG_CREATE_ERROR_ID6,	
	enumTONG_CREATE_ERROR_ID7,		
	enumTONG_CREATE_ERROR_ID8,	
	enumTONG_CREATE_ERROR_ID9,
	enumTONG_CREATE_ERROR_ID10,
	enumTONG_CREATE_ERROR_ID11,
	enumTONG_CREATE_ERROR_ID12,
	enumTONG_CREATE_ERROR_ID13,
	enumTONG_CREATE_ERROR_ID14,
	enumTONG_CREATE_ERROR_ID15,
	enumTONG_CREATE_ERROR_ID16,
};

//=========================
//
//=========================
struct KTongInfo
{	
	BYTE  nFaction;
	BYTE  btLevel;			
	int   nMoney;
	int	  nMemberCount;         
	int   nManagerCount;        
	int   nDirectorCount;       
	int   nFigure;
	char  szName[defTONG_NAME_MAX_LENGTH + 1];		
	char  szMasterName[defPLAYER_NAME_MAX_LENGTH + 1];	
};

enum TONG_ACTION_TYPE
{
	TONG_ACTION_KICKOUT,       
	TONG_ACTION_ASSIGN,        
	TONG_ACTION_DEMISE,        
	TONG_ACTION_LEAVE,
	TONG_ACTION_MALE_TITLE,
	TONG_ACTION_FEMALE_TITLE,
	TONG_ACTION_TITLE,
	TONG_ACTION_SAVE_MONEY,
	TONG_ACTION_GET_MONEY,
	TONG_ACTION_SND_MONEY,
	TONG_ACTION_RECRUIT,       
	TONG_ACTION_APPLY,         
};

struct KTongOperationParam
{
	TONG_ACTION_TYPE	eOper;
	int                 nData[4];
	char				szPassword[32];
};

//============================================
//
//============================================
struct KTongMemberItem : public KUiPlayerItem
{	 
	char	szAgname[defTONG_TITLE_MAX_LENGTH + 1];
	bool	bStatus;
};

struct KUiPlayerRelationWithOther : KUiPlayerItem
{
	int		nRelation;
	int		nParam;
};

struct KUiGameObjectWithName
{
	char			szName[32];
	char			szString[32];
	int				nData;
	int				nParam;
	unsigned int 	uParam;
};
//===========================================
//Report Battle by kinnox;
//===========================================
struct KUiTopTKNewData
{
	BYTE			m_SoluongSong;
	BYTE			m_SoluongJin;
	BYTE			m_ThoiGian;
	WORD			m_NguoiChoi;
	WORD			m_TNguoiChoi;
	WORD			m_NPC;
	WORD			m_TNPC;
	WORD			m_LienTramHT;
	WORD			m_TLienTramHT;
	WORD			m_BauVat;
	WORD			m_TBauVat;
	WORD			m_TichLuy;
	WORD			m_TuVong;
	WORD			m_LienTram;

	char			m_szTop1Name[32];
	BYTE			m_Top1Phe;
	WORD			m_Top1TichLuy;
	WORD			m_Top1NguoiChoi;
	WORD			m_Top1NPC;
	WORD			m_Top1TuVong;
	WORD			m_Top1LienTram;
	WORD			m_Top1BaoVat;

	char			m_szTop2Name[32];
	BYTE			m_Top2Phe;
	WORD			m_Top2TichLuy;
	WORD			m_Top2NguoiChoi;
	WORD			m_Top2NPC;
	WORD			m_Top2TuVong;
	WORD			m_Top2LienTram;
	WORD			m_Top2BaoVat;

	char			m_szTop3Name[32];
	BYTE			m_Top3Phe;
	WORD			m_Top3TichLuy;
	WORD			m_Top3NguoiChoi;
	WORD			m_Top3NPC;
	WORD			m_Top3TuVong;
	WORD			m_Top3LienTram;
	WORD			m_Top3BaoVat;

	char			m_szTop4Name[32];
	BYTE			m_Top4Phe;
	WORD			m_Top4TichLuy;
	WORD			m_Top4NguoiChoi;
	WORD			m_Top4NPC;
	WORD			m_Top4TuVong;
	WORD			m_Top4LienTram;
	WORD			m_Top4BaoVat;

	char			m_szTop5Name[32];
	BYTE			m_Top5Phe;
	WORD			m_Top5TichLuy;
	WORD			m_Top5NguoiChoi;
	WORD			m_Top5NPC;
	WORD			m_Top5TuVong;
	WORD			m_Top5LienTram;
	WORD			m_Top5BaoVat;

	char			m_szTop6Name[32];
	BYTE			m_Top6Phe;
	WORD			m_Top6TichLuy;
	WORD			m_Top6NguoiChoi;
	WORD			m_Top6NPC;
	WORD			m_Top6TuVong;
	WORD			m_Top6LienTram;
	WORD			m_Top6BaoVat;

	char			m_szTop7Name[32];
	BYTE			m_Top7Phe;
	WORD			m_Top7TichLuy;
	WORD			m_Top7NguoiChoi;
	WORD			m_Top7NPC;
	WORD			m_Top7TuVong;
	WORD			m_Top7LienTram;
	WORD			m_Top7BaoVat;

	char			m_szTop8Name[32];
	BYTE			m_Top8Phe;
	WORD			m_Top8TichLuy;
	WORD			m_Top8NguoiChoi;
	WORD			m_Top8NPC;
	WORD			m_Top8TuVong;
	WORD			m_Top8LienTram;
	WORD			m_Top8BaoVat;

	char			m_szTop9Name[32];
	BYTE			m_Top9Phe;
	WORD			m_Top9TichLuy;
	WORD			m_Top9NguoiChoi;
	WORD			m_Top9NPC;
	WORD			m_Top9TuVong;
	WORD			m_Top9LienTram;
	WORD			m_Top9BaoVat;

	char			m_szTop10Name[32];
	BYTE			m_Top10Phe;
	WORD			m_Top10TichLuy;
	WORD			m_Top10NguoiChoi;
	WORD			m_Top10NPC;
	WORD			m_Top10TuVong;
	WORD			m_Top10LienTram;
	WORD			m_Top10BaoVat;
};
#define		MIN_TONG_MONEY_VALUE		10000



//=========================================================================================





#define	MAP_TYPE_CITY							"City"
#define	MAP_TYPE_CAPITAL						"Capital"


#define		defMAX_CITY							7
#define		defMAX_TAX_VALUE					20


//=========================================================================================
#ifdef _SERVER
struct KTaskObject
{
	short	m_nID;
	bool	m_bSync;
};
#endif

//=========================================================================================
typedef struct
{
	int		m_nX;
	int		m_nY;
} TOADO;

struct KUiSkillItem
{
	char	Name[32];
	USHORT	Index;	
};


#ifdef _SERVER
#include <string>
typedef struct tag_DATABASEINFO
{
	std::string strServer;
	std::string strDataBase;
	std::string strUser;
	std::string strPassword;
}_DATABASEINFO, *_LPDATABASEINFO;
#endif

#endif