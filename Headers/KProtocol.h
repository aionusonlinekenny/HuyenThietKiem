#ifndef KPROTOCOL_H
#define KPROTOCOL_H

#ifndef __linux
#ifdef _STANDALONE
#include "GameDataDef.h"
#else
#include "../Sources/Core/src/GameDataDef.h"
#endif
#else
#include "GameDataDef.h"
#include <string.h>
#endif

#include "KProtocolDef.h"
#include "KRelayProtocol.h"

#pragma pack(push, enter_protocol)
#pragma	pack(1)

#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))
#define	MAX_PROTOCOL_NUM	256


typedef struct
{
	BYTE	ProtocolType;
	BYTE	WalkSpeed;
	BYTE	RunSpeed;
	short	AttackSpeed;
	short	CastSpeed;
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	WORD	MaskType;
	short	MantleType;
	BYTE	MantleLevel;			  
	BYTE	RankID;
	char	PKValue;
	short	WorldStat;
	short	Repute;
	short	Fuyuan;
	char	TransLife;
	BYTE	OpenShop;
	char	ShopAdv[16];
	char	TongName[defTONG_NAME_MAX_LENGTH + 2];
	char	TongTitle[defTONG_TITLE_MAX_LENGTH + 2];
	WORD	TitleID;
	DWORD	ID;
	BYTE	m_btSomeFlag;
	BYTE	ImagePlayer; //Player Avatar by kinnox;
	//
} PLAYER_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	WalkSpeed;
	BYTE	RunSpeed;
	short	AttackSpeed;
	short	CastSpeed;
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	WORD	MaskType;
	short	MantleType;
	BYTE	MantleLevel;			  
	BYTE	RankID;
	char	PKValue;
	short	WorldStat;
	short	Repute;
	short	Fuyuan;
	char	TransLife;
	BYTE	OpenShop;
	char	ShopAdv[16];
	char	TongName[defTONG_NAME_MAX_LENGTH + 2];
	char	TongTitle[defTONG_TITLE_MAX_LENGTH + 2];
	WORD	TitleID;
	BYTE	m_btSomeFlag;
	BYTE	ImagePlayer;//Player Avatar by kinnox;
	//	
} PLAYER_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	Camp;				
	BYTE	CurrentCamp;		
	BYTE	m_bySeries;			
	BYTE	LifePerCent;		
	BYTE	m_btMenuState;		
	BYTE	m_Doing;			
	BYTE	m_btKind;			
	DWORD	MapX;				
	DWORD	MapY;				
	DWORD	ID;					
	int		NpcSettingIdx;	
	WORD	NpcEnchant;			
	char	m_szName[32];		
} NPC_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	DWORD	MapX;
	DWORD	MapY;
	BYTE	Camp;
	BYTE	LifePerCent;
	BYTE	Doing;
	BYTE	State;
	char	HideValue;
	//
	int		m_CurrentLife;
	int		m_CurrentLifeMax;
	int		m_CurrentMana;
	int		m_CurrentManaMax;

	int		m_CurrentWalkSpeed;
	int		m_CurrentRunSpeed;
	int		m_ASpeed;
	int		m_CSpeed;
	//them by kinnox;
} NPC_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwNpcID;
	DWORD	m_dwMapX;
	DWORD	m_dwMapY;
	WORD	m_wOffX;
	WORD	m_wOffY;
} NPC_PLAYER_TYPE_NORMAL_SYNC;
                 
typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
} NPC_REMOVE_SYNC, NPC_SIT_SYNC, NPC_DEATH_SYNC, NPC_REQUEST_FAIL;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	ID;
} NPC_REQUEST_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nMpsX;
	int		nMpsY;
	BOOL	bRandMove;
} NPC_WALK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Type;
} NPC_REVIVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwID;
	int		m_nMpsX;
	int		m_nMpsY;
} NPC_JUMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwID;
	int		m_nMpsX;
	int		m_nMpsY;
	short	m_nLenght;
} NPC_GLIDE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_RUN_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nFrames;
	int		nX;
	int		nY;
} NPC_HURT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCURCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		nSkillID;
	int		nSkillLevel;
	int		nMpsX;
	int		nMpsY;
	BOOL	m_bAuto;
} NPC_SKILL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		nSkillID;
	int		nMpsX;
	int		nMpsY;
} NPC_SKILL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		nMpsX;
	int		nMpsY;
} NPC_WALK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} NPC_REVIVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		nMpsX;
	int		nMpsY;
} NPC_RUN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nShopIndex;
} SALE_BOX_SYNC;

typedef struct 
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		nNpcId;
} PLAYER_DIALOG_NPC_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nExp;
} PLAYER_EXP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} PLAYER_APPLY_CREATE_TEAM;	

struct PLAYER_SEND_CREATE_TEAM_SUCCESS
{
	BYTE	ProtocolType;		
	DWORD	nTeamServerID;		
	PLAYER_SEND_CREATE_TEAM_SUCCESS() {nTeamServerID = -1;}
};

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_btErrorID;
} PLAYER_SEND_CREATE_TEAM_FALSE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwTarNpcID;
} PLAYER_APPLY_TEAM_INFO;

typedef struct
{
	BYTE	ProtocolType;		
} PLAYER_APPLY_TEAM_INFO_FALSE;	

typedef struct PLAYER_SEND_TEAM_INFO_DATA
{
	BYTE	ProtocolType;		
	int		m_nCaptain;
	int		m_nMember[MAX_TEAM_MEMBER];	
	DWORD	nTeamServerID;
	PLAYER_SEND_TEAM_INFO_DATA() {nTeamServerID = -1;};
} PLAYER_SEND_TEAM_INFO;

typedef struct PLAYER_SEND_SELF_TEAM_INFO_DATA
{
	BYTE	ProtocolType;							
	BYTE	m_btState;								
	DWORD	m_dwNpcID[MAX_TEAM_MEMBER + 1];		
	char	m_szNpcName[MAX_TEAM_MEMBER + 1][32];
	DWORD	nTeamServerID;
	DWORD	m_dwLeadExp;
	BYTE	m_btLevel[MAX_TEAM_MEMBER + 1];
	int 	m_HP[MAX_TEAM_MEMBER + 1];
	int 	m_MHP[MAX_TEAM_MEMBER + 1];
	int 	m_SM[MAX_TEAM_MEMBER + 1];
	int 	m_TP[MAX_TEAM_MEMBER + 1];
	int 	m_SubWorldID[MAX_TEAM_MEMBER + 1];
	PLAYER_SEND_SELF_TEAM_INFO_DATA() {memset(m_szNpcName, 0, 32 * (MAX_TEAM_MEMBER + 1)); nTeamServerID = -1;};
} PLAYER_SEND_SELF_TEAM_INFO;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btOpenClose;		
} PLAYER_TEAM_OPEN_CLOSE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwTarNpcID;
} PLAYER_APPLY_ADD_TEAM;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;	
} PLAYER_ACCEPT_TEAM_MEMBER;		

typedef struct PLAYER_TEAM_ADD_MEMBER_DATA
{
	BYTE	ProtocolType;		
	BYTE	m_btLevel;			
	DWORD	m_dwNpcID;		
	char	m_szName[32];	
	int 	m_SM;
	int 	m_HP;
	int 	m_MHP;
	int 	m_TP;
	int 	m_SubWorldID;     // Th阭 bien SubWorldID
	PLAYER_TEAM_ADD_MEMBER_DATA() {memset(m_szName, 0, 32);};
} PLAYER_TEAM_ADD_MEMBER;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} PLAYER_APPLY_LEAVE_TEAM;

typedef struct
{
	BYTE	ProtocolType;		
	DWORD	m_dwNpcID;			
} PLAYER_LEAVE_TEAM;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
} PLAYER_TEAM_KICK_MEMBER;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
} PLAYER_APPLY_TEAM_CHANGE_CAPTAIN;	

typedef struct
{
	BYTE	ProtocolType;		
	DWORD	m_dwCaptainID;
	DWORD	m_dwMemberID;
} PLAYER_TEAM_CHANGE_CAPTAIN;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} PLAYER_APPLY_TEAM_DISMISS;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BOOL	m_bPKFlag;
} PLAYER_SET_PK;	

typedef struct
{
	BYTE	ProtocolType;			
	BYTE	m_btCamp;				
	BYTE	m_btCurFaction;			
	BYTE	m_btFirstFaction;		
	int		m_nAddTimes;			
} PLAYER_FACTION_DATA;				

typedef struct
{
	BYTE	ProtocolType;			
} PLAYER_LEAVE_FACTION;				

typedef struct
{
	BYTE	ProtocolType;			
	BYTE	m_btCurFactionID;	
	BYTE	m_btLevel;				
} PLAYER_FACTION_SKILL_LEVEL;		

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} PLAYER_APPLY_FACTION_DATA;

typedef struct PLAYER_SEND_CHAT_DATA_COMMAND
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	WORD	m_wLength;
	BYTE	m_btCurChannel;		
	BYTE	m_btType;
	BYTE	m_btChatPrefixLen;
	WORD	m_wSentenceLen;
	DWORD	m_dwTargetID;
	int		m_nTargetIdx;
	char	m_szSentence[MAX_SENTENCE_LENGTH + CHAT_MSG_PREFIX_MAX_LEN];	
	PLAYER_SEND_CHAT_DATA_COMMAND() {memset(m_szSentence, 0, sizeof(m_szSentence));};
} PLAYER_SEND_CHAT_COMMAND;

typedef struct PLAYER_SEND_CHAT_DATA_SYNC
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btCurChannel;
	BYTE	m_btNameLen;
	BYTE	m_btChatPrefixLen;
	WORD	m_wSentenceLen;
	DWORD	m_dwSourceID;
	char	m_szSentence[32 + CHAT_MSG_PREFIX_MAX_LEN + MAX_SENTENCE_LENGTH];
	PLAYER_SEND_CHAT_DATA_SYNC() { memset(m_szSentence, 0, sizeof(m_szSentence)); };
} PLAYER_SEND_CHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	int		m_nID;
	int		m_nDataID;
	int		m_nXpos;
	int		m_nYpos;
	int		m_nMoneyNum;
	int		m_nItemID;
	DWORD	m_dwItemID;
	BYTE	m_btDir;
	BYTE	m_btItemWidth;
	WORD	m_wCurFrame;
	BYTE	m_btItemHeight;
	BYTE	m_btColorID;
	BYTE	m_btFlag;
	//AutoAI by kinnox;
	int		m_nGenre;
	int		m_nDetailType;
	WORD	m_dwNpcId;
	//end code;
	char	m_szName[FILE_NAME_LENGTH];
} OBJ_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btState;
	int		m_nID;
} OBJ_SYNC_STATE;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btDir;
	int		m_nID;
} OBJ_SYNC_DIR;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	BYTE	m_btSoundFlag;
} OBJ_SYNC_REMOVE;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	int		m_nTarX;
	int		m_nTarY;
} OBJ_SYNC_TRAP_ACT;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nID;
} OBJ_CLIENT_SYNC_ADD;

typedef struct
{
	BYTE	ProtocolType;		
	DWORD	m_dwLeadExp;		
} PLAYER_LEAD_EXP_SYNC;

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_btLevel;			
	int		m_nExp;
	int		m_nAttributePoint;	
	int		m_nSkillPoint;		
	int		m_nBaseLifeMax;		
	int		m_nBaseStaminaMax;	
	int		m_nBaseManaMax;		
} PLAYER_LEVEL_UP_SYNC;			

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btLevel;			
	DWORD	m_dwTeammateID;	
} PLAYER_TEAMMATE_LEVEL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btAttribute;
	BYTE	m_btAddNo;
} PLAYER_ADD_BASE_ATTRIBUTE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nSkillID;
	int		m_nAddPoint;
} PLAYER_ADD_SKILL_POINT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_btAttribute;
	int		m_nBasePoint;		
	int		m_nCurPoint;	
	int		m_nLeavePoint;		
} PLAYER_ATTRIBUTE_SYNC;		

typedef struct
{
	BYTE	ProtocolType;		
	int		m_nSkillID;			
	int		m_nSkillLevel;
	int		m_nLeavePoint;
	BYTE	m_btPointAdd;	
	bool	m_bExpand;
} PLAYER_SKILL_LEVEL_SYNC;	
	
typedef struct
{
	BYTE	ProtocolType;		
	int		m_nSkillID;
} PLAYER_SKILL_REMOVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btPlace;			
	BYTE	m_btX;				
	BYTE	m_btY;				
	int		m_nItemID;			
} PLAYER_EAT_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nObjID;
	BYTE	m_btPosType;
	BYTE	m_btPosX;
	BYTE	m_btPosY;
} PLAYER_PICKUP_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		
	DWORD	m_ID;				
	char	m_Genre;			
	short	m_Detail;	
	BYTE	m_Level;
	char	m_Series;			
	BYTE	m_Place;			
	BYTE	m_X;				
	BYTE	m_Y;
	int		m_Luck;
	BYTE	m_MagicLevel[6];	
	WORD	m_Version;			
	WORD	m_Durability;		
	DWORD	m_RandomSeed;
	WORD	m_Record;
	DWORD	m_BindState;
	DWORD	m_ExpiredTime;
	int		m_ShopPrice;
} ITEM_SYNC;

typedef struct
{
	BYTE	ProtocolType;		
	int		m_ID;				
} ITEM_REMOVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_ID;
} PLAYER_SELL_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_BuyIdx;
	BYTE	m_Place;
	BYTE	m_X;
	BYTE	m_Y;
} PLAYER_BUY_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nMoney1;
	int		m_nMoney2;
	int		m_nMoney3;
} PLAYER_MONEY_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btDownPos;
	BYTE	m_btDownX;
	BYTE	m_btDownY;
	BYTE	m_btUpPos;
	BYTE	m_btUpX;
	BYTE	m_btUpY;
} PLAYER_MOVE_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btDownPos;
	BYTE	m_btDownX;
	BYTE	m_btDownY;
	BYTE	m_btUpPos;
	BYTE	m_btUpX;
	BYTE	m_btUpY;
} PLAYER_MOVE_ITEM_SYNC;

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_btSrcPos;
	BYTE	m_btSrcX;
	BYTE	m_btSrcY;
	BYTE	m_btDestPos;
	BYTE	m_btDestX;
	BYTE	m_btDestY;
} ITEM_AUTO_MOVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	dwIDItem;
	BOOL	bAutoPick;
} PLAYER_THROW_AWAY_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	USHORT	m_ushLife;
	USHORT	m_ushStamina;
	USHORT	m_ushMana;
	BYTE	m_btTeamData;
} CURPLAYER_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;		
	DWORD	m_dwID;				
	BYTE	m_btLevel;			
	BYTE	m_btSex;			
	BYTE	m_btKind;			
	BYTE	m_btSeries;			
	WORD	m_wLifeMax;			
	WORD	m_wStaminaMax;		
	WORD	m_wManaMax;			
	int		m_HeadImage;

	WORD	m_wAttributePoint;	
	WORD	m_wSkillPoint;		
	WORD	m_wStrength;		
	WORD	m_wDexterity;		
	WORD	m_wVitality;		
	WORD	m_wEngergy;			
	WORD	m_wLucky;			
	UINT	m_nExp;			
	DWORD	m_dwLeadExp;

	BYTE	m_btCurFaction;		
	BYTE	m_btFirstFaction;	
	int		m_nFactionAddTimes;	
	
	WORD	m_wWorldStat;		
	WORD	m_wSectStat;		

	int		m_nMoney1;
	int		m_nMoney2;

	int		m_nBronzeCoin;
	int		m_nLockBronzeCoin;
	BYTE	m_btRepositoryCount;
	int		m_nLockPlayer; //LockPlayer by kinnox;
	BYTE	m_btExpandtoryCount;//expandbox by kinnox;	
	BYTE	m_btImagePlayer;//Player Avatar by kinnox;
	//
} CURPLAYER_SYNC;

#define MAX_SCIRPTACTION_BUFFERNUM 1024

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wProtocolLong;
	BYTE	m_nOperateType;	
	BYTE	m_bUIId, m_bOptionNum, m_bParam1, m_bParam2;
	int		m_nParam;
	int		m_nBufferLen;
	char	m_pContent[MAX_SCIRPTACTION_BUFFERNUM];
} PLAYER_SCRIPTACTION_SYNC;

typedef struct
{
	WORD	SkillId;
	BYTE	SkillLevel;
} SKILL_SEND_ALL_SYNC_DATA;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wProtocolLong;
	SKILL_SEND_ALL_SYNC_DATA	m_sAllSkill[80];
} SKILL_SEND_ALL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	WeatherID;
} SYNC_WEATHER;

typedef struct defWORLD_SYNC
{
	BYTE	ProtocolType;
	int		SubWorld;
	int		Region;
	BYTE	Weather;
	DWORD	Frame;
} WORLD_SYNC;

typedef struct 
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btUIId;
	BYTE	nSelectIndex;
	char	m_szParam[32];

}PLAYER_SELECTUI_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwTakeChannel;	
} CHAT_SET_CHANNEL_COMMAND;				

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	WORD	m_wLength;
	DWORD	m_dwTargetNpcID;
	char	m_szInfo[MAX_SENTENCE_LENGTH];
} CHAT_APPLY_ADD_FRIEND_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nSrcPlayerIdx;
	char	m_szSourceName[32];	
	char	m_szInfo[MAX_SENTENCE_LENGTH];
} CHAT_APPLY_ADD_FRIEND_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
	int				m_nTargetPlayerIdx;	// 被接受player idx
} CHAT_ADD_FRIEND_COMMAND;				// 添加某玩家为聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
	int				m_nTargetPlayerIdx;	// 被拒绝player idx
} CHAT_REFUSE_FRIEND_COMMAND;			// 拒绝添加某玩家为聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 新添加好友的 id
	int				m_nIdx;				// 新添加好友在 player 数组中的位置
	char			m_szName[32];		// 新添加好友的名字
} CHAT_ADD_FRIEND_SYNC;					// 通知客户端成功添加一个聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	char			m_szName[32];		// 拒绝者名字
} CHAT_REFUSE_FRIEND_SYNC;				// 通知客户端添加聊天好友的申请被拒绝

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	int				m_nTargetPlayerIdx;	// 出错 player idx (一般可能是此player下线或者换服务器了)
} CHAT_ADD_FRIEND_FAIL_SYNC;			// 通知客户端添加聊天好友失败

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
} VIEW_EQUIP_COMMAND;


/*typedef struct
{
	BYTE			ProtocolType;
	int				m_nSelect; 
} DB_PLAYERSELECT_COMMAND;*/

/*
 * { Add by liupeng 2003.05.10
 *
 * #pragma pack( push, 1 )
*/

/*
 * Nonstandard extension used : zero-sized array in struct/union
 */
#pragma warning(disable: 4200)

#define KSG_PASSWORD_MAX_SIZE   64

typedef struct tagKSG_PASSWORD
{
    char szPassword[KSG_PASSWORD_MAX_SIZE];
} KSG_PASSWORD;

#define _NAME_LEN	32

struct tagProtoHeader
{
	BYTE	cProtocol;
};

struct tagResult : public tagProtoHeader
{
	BYTE	cResult;
};

struct tagDBSelPlayer : public tagProtoHeader
{
	char	szRoleName[_NAME_LEN];
};

struct tagDBDelPlayer : public tagProtoHeader
{
	char	        szAccountName[_NAME_LEN];
    KSG_PASSWORD    Password;
	char	        szRoleName[_NAME_LEN];
};

struct tagNewDelRoleResponse : public tagDBSelPlayer
{
	char	bSucceeded;
};

struct tagDBSyncPlayerInfo : public tagProtoHeader
{
	size_t	dataLength;
	char	szData[0];
};


#define	AP_WARNING_ALL_PLAYER_QUIT			1
#define	AP_NOTIFY_GAMESERVER_SAFECLOSE		2
#define	AP_NOTIFY_ALL_PLAYER				3
#define	MAX_GATEWAYBROADCAST_LEN	260
struct tagGatewayBroadCast : public tagProtoHeader
{
	UINT	uCmdType;
	char	szData[MAX_GATEWAYBROADCAST_LEN];
};

/*
 * 2003.05.22
 * s2c_syncgamesvr_roleinfo_cipher
*/
struct tagGuidableInfo : public tagProtoHeader
{
	GUID guid;
	BYTE bCheckOff;
	size_t	datalength;
	char	szData[0];

};

/*
 * c2s_permitplayerlogin
 */
struct tagPermitPlayerLogin : public tagProtoHeader
{
	GUID guid;

	BYTE szRoleName[_NAME_LEN];

	/*
	 * Succeeded : true
	 * Failed	 : false
	 */
	bool bPermit;
};

struct tagPermitPlayerExchange
{
	BYTE cProtocol;
	GUID guid;
	DWORD dwIp;
	WORD wPort;
	bool bPermit;
};
/*
 * c2s_notifyplayerlogin
 */
struct tagNotifyPlayerLogin : public tagPermitPlayerLogin
{
	UINT			nIPAddr;
	unsigned short	nPort;
};

/*
 * s2c_querymapinfo
 */
struct tagQueryMapInfo : public tagProtoHeader
{	
};

/*
 * s2c_querygameserverinfo
 */
struct tagQueryGameSvrInfo : public tagProtoHeader
{	
};

/*
 * s2c_notifysvrip
 */
struct tagNotifySvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;

	DWORD	dwSvrIP;
};

/*
 * s2c_notifyplayerexchange
 */
struct tagNotifyPlayerExchange : public tagProtoHeader
{
	GUID			guid;
	UINT			nIPAddr;
	unsigned short	nPort;
};

/*
 * c2s_requestsvrip
 */

/*
 * BYTE	cIPType
 */
#define INTRANER_IP	0
#define INTERNET_IP 1

struct tagRequestSvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;
};

/*
 * c2c_notifyexchange
 */
struct tagSearchWay : public tagProtoHeader
{
	int		lnID;
	int		nIndex;
	DWORD	dwPlayerID;
};

/*
 * c2s_updatemapinfo
 */
struct tagUpdateMapID : public tagProtoHeader
{
	/*
	 * For example : Are your clear older information when it 
	 *		update local informatin
	 */
	BYTE cReserve;

	BYTE cMapCount;

	short szMapID[0];	// C4200 warning
};

/*
 * c2s_updategameserverinfo
 */
struct tagGameSvrInfo : public tagProtoHeader
{
	UINT			nIPAddr_Intraner;
	UINT			nIPAddr_Internet;

	unsigned short	nPort;
	WORD			wCapability;
};

/*
 * s2c_identitymapping
 */
struct tagIdentityMapping : public tagGameSvrInfo
{
	GUID guid;
};

/*
 * c2s_logiclogin
 * s2c_gmgateway2relaysvr
 * s2c_gmnotify
 */
struct tagLogicLogin : public tagProtoHeader
{
	GUID guid;
};

/*
 * s2c_logiclogout
 */
struct tagLogicLogout : public tagProtoHeader
{
	BYTE szRoleName[_NAME_LEN];
};

/*
 * c2s_registeraccount
 */
struct tagRegisterAccount : public tagProtoHeader
{
	BYTE szAccountName[_NAME_LEN];
};

/*
 * c2s_entergame
 */
struct tagEnterGame : public tagProtoHeader
{
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	BYTE szAccountName[_NAME_LEN];
};

struct tagEnterGame2 : public EXTEND_HEADER
{
	char szAccountName[_NAME_LEN];
	char szCharacterName[_NAME_LEN];
	DWORD	dwNameID;
	unsigned long	lnID;
};

/*
 * c2s_leavegame
 */

/*
 * BYTE cCmdType
 */
#define NORMAL_LEAVEGAME	0x0		// lock account
#define HOLDACC_LEAVEGAME	0x1A	// clear resource but don't to unlock account 

struct tagLeaveGame : public tagProtoHeader
{
	BYTE cCmdType;
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	char szAccountName[_NAME_LEN];
};

struct tagLeaveGame2 : public EXTEND_HEADER
{
	BYTE cCmdType;
	char szAccountName[_NAME_LEN];
};
/*
*  c2s_registerfamily
*/
struct tagRegisterFamily : public tagProtoHeader
{
	BYTE bRegister;		//1 is Register, 0 is unRegister
	BYTE nFamily;
	BYTE RelayMethod;
};

/*
 * c2s_gmsvr2gateway_saverole
 */
struct tagGS2GWSaveRole : public tagProtoHeader
{
	size_t	datalength;
	BYTE	szData[0];
};

/*
 * #pragma pack( pop )
 *
 * } End of the struct define
 */

typedef struct
{
	char	szName[32];
	BYTE	Sex;
	BYTE	Series;
	BYTE	Level;
	DWORD	dwTongId;
} RoleBaseInfo/* client */, S3DBI_RoleBaseInfo /* server */;

typedef struct
{
	BYTE				ProtocolType;
	RoleBaseInfo		m_RoleList[MAX_PLAYER_IN_ACCOUNT];
} ROLE_LIST_SYNC;

struct TProcessData
{
	unsigned char	nProtoId;
	size_t			nDataLen;
	unsigned long	ulIdentity;
	bool			bLeave;
	int				nSaveType;//Lixian by kinnox;	
	char			pDataBuffer[1];
};

struct tagRoleEnterGame
{
	BYTE			ProtocolType;
	bool			bLock;
	char			Name[_NAME_LEN];
};

struct NEW_PLAYER_COMMAND
{
	BYTE			m_btRoleNo;			
	BYTE			m_btSeries;			
	unsigned short	m_NativePlaceId;	
	char			m_szName[32];		
};


typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
} CHAT_LOGIN_FRIEND_NONAME_SYNC;		// 玩家登录时发送玩家聊天好友数据（不带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	WORD			m_wLength;
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
	char			m_szName[32];		// 好友名字
} CHAT_LOGIN_FRIEND_NAME_SYNC;			// 玩家登录时发送玩家聊天好友数据（带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
} CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
	DWORD			m_dwID;				// 好友 id	
} CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND;	// 申请得到玩家某个聊天好友的完整数据

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
	char			m_szName[32];		// 好友名字
} CHAT_ONE_FRIEND_DATA_SYNC;			// 发送玩家某一个聊天好友数据（带名字）

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 好友 id
	int				m_nPlayerIdx;		// 好友 player index
} CHAT_FRIEND_ONLINE_SYNC;				// 通知客户端有好友上线

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
	DWORD			m_dwID;				// 被删除id
} CHAT_DELETE_FRIEND_COMMAND;			// 删除某个聊天好友

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			m_dwID;				// 删除id
} CHAT_DELETE_FRIEND_SYNC;				// 被某个聊天好友删除

typedef struct
{
	BYTE			ProtocolType;		// 协议类型
	DWORD			dwID;
	DWORD			dwTimePacker;
	DWORD			m_dwID;				// 被删除id
} CHAT_REDELETE_FRIEND_COMMAND;			// 删除某个聊天好友

typedef struct 
{
	BYTE			ProtocolType;
	BYTE			m_LogoutType;
} LOGOUT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			szAccName[32];
} LOGIN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		
	DWORD			m_dwID;	
} CHAT_FRIEND_OFFLINE_SYNC;	

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bReserve;
} TRADE_APPLY_OPEN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} TRADE_APPLY_CLOSE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		
	BYTE			m_btState;
	DWORD			m_dwNpcID;
} TRADE_CHANGE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	DWORD			m_dwID;
	BYTE			m_btState;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} NPC_SET_MENU_STATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwID;
} TRADE_APPLY_START_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		
	int		m_nDestIdx;
	DWORD	m_dwNpcId;
} TRADE_APPLY_START_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bDecision;
	DWORD	m_dwNpcID;
} TRADE_REPLY_START_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nMoney;
} TRADE_MOVE_MONEY_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nMoney;
} TRADE_MONEY_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btDecision;
} TRADE_DECISION_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_btDecision;
} TRADE_DECISION_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_byDir;
	DWORD	m_dwMoney;
} STORE_MONEY_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		
	WORD	m_wLength;			
	BYTE	m_btError;		
	char	m_szName[32];
} CHAT_SCREENSINGLE_ERROR_SYNC;

typedef struct 
{
	BYTE			ProtocolType;		
	BYTE			m_btStateInfo[MAX_NPC_RECORDER_STATE];
	DWORD			m_ID;				
} NPC_SYNC_STATEINFO;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
} TEAM_INVITE_ADD_COMMAND;

typedef struct
{
	BYTE	ProtocolType;	
	WORD	m_wLength;
	int		m_nIdx;
	char	m_szName[32];
} TEAM_INVITE_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nAuraSkill;
} SKILL_CHANGEAURASKILL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btResult;
	int		m_nIndex;
} TEAM_REPLY_INVITE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btSelfLock;
	BYTE	m_btDestLock;
	BYTE	m_btSelfOk;
	BYTE	m_btDestOk;
} TRADE_STATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	DWORD	m_dwSkillID;		
	int		m_nLevel;
	int		m_nTime;
	BOOL	m_bRemove;
	KMagicAttrib	m_MagicAttrib[MAX_SKILL_STATE];
} STATE_EFFECT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwTime;
} PING_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwReplyServerTime;
	DWORD	m_dwClientTime;
} PING_CLIENTREPLY_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btSitFlag;
} NPC_SIT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		nMpsX;
	int		nMpsY;
} NPC_JUMP_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_dwRegionID;
	int		m_nObjID;
} OBJ_MOUSE_CLICK_SYNC;

typedef struct tagSHOW_MSG_SYNC
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	WORD	m_wMsgID;
	LPVOID	m_lpBuf;
	tagSHOW_MSG_SYNC() {m_lpBuf = NULL;};
	~tagSHOW_MSG_SYNC() {Release();}
	void	Release() {if (m_lpBuf) delete []m_lpBuf; m_lpBuf = NULL;}
} SHOW_MSG_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btFlag;
} PK_APPLY_NORMAL_FLAG_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_NORMAL_FLAG_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
} PK_APPLY_ENMITY_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	DWORD	m_dwNpcID;
	char	m_szName[32];
} PK_ENMITY_STATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	DWORD	m_dwNpcID;
	char	m_szName[32];
} PK_EXERCISE_STATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	char	m_cPKValue;
} PK_VALUE_SYNC;

typedef struct
{
	//
	DWORD	m_ID;				
	char	m_Genre;			
	short	m_Detail;	
	BYTE	m_Level;
	char	m_Series;
	int		m_Luck;
	BYTE	m_MagicLevel[6];	
	WORD	m_Version;		
	DWORD	m_RandomSeed;
	WORD	m_Record;

} SViewItemInfo;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwNpcID;
	SViewItemInfo	m_sInfo[itempart_num];
	BYTE			m_bAvatar;//Player Avatar by kinnox;
	//
} VIEW_EQUIP_SYNC;

typedef struct
{
	char	Name[20];
	int		nValue;
	BYTE	bySort;
	BYTE	TransLife;
}TRoleList;

typedef struct
{
	TRoleList MoneyStat[10];			
	TRoleList LevelStat[10];			
	TRoleList KillerStat[10];			
	
	TRoleList MoneyStatBySect[11][10];	
	TRoleList LevelStatBySect[11][10];	

	int SectPlayerNum[11];				
	int SectMoneyMost[11];				
	int SectLevelMost[11];				
}  TGAME_STAT_DATA;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	bSleep;
	DWORD	NpcID;
} NPC_SLEEP_SYNC;


typedef struct
{
	BYTE		ProtocolType;
	DWORD		dwLadderID;
	TRoleList	StatData[10];
} LADDER_DATA;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	int		nCount;
	DWORD	dwLadderID[0];
} LADDER_LIST;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	dwLadderID;
} LADDER_QUERY;


///////////////////

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwTimePacker;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	WORD	sentlen;
} CHAT_SOMEONECHAT_CMD, CHAT_SOMEONECHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwTimePacker;
	WORD	wSize;
	DWORD	packageID;
	BYTE	filter;
	DWORD	channelid;
	BYTE	cost;
	WORD	sentlen;
} CHAT_CHANNELCHAT_CMD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	DWORD	channelid;
	WORD	sentlen;
} CHAT_CHANNELCHAT_SYNC;

enum {codeSucc, codeFail, codeStore};
typedef struct
{
	BYTE	ProtocolType;
	DWORD	packageID;
	BYTE	code;
} CHAT_FEEDBACK;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
} CHAT_EVERYONE;
//Add By ShinT
typedef struct
{
	BYTE	ProtocolType;
	BYTE	ActionType;
	char	szText[512];
	char	szFunc[128];
	int		nValue1;
	int		nValue2;
	int		nValue3;
} S2C_PLAYER_ACTION;

//EndAdd

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
	BYTE	byHasIdentify;
	WORD	wPlayerCount;
} CHAT_GROUPMAN;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nameid;
	unsigned long lnID;
	WORD	wChatLength;
} CHAT_SPECMAN;


enum { tgtcls_team, tgtcls_fac, tgtcls_tong, tgtcls_scrn, tgtcls_bc, tgtcls_battle};
typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nFromIP;
	DWORD	nFromRelayID;
	DWORD	channelid;
	BYTE	TargetCls;
	DWORD	TargetID;
	WORD	routeDateLength;
} CHAT_RELEGATE;


///////////////////////////////////////

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} S2C_TONG_HEAD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} STONG_PROTOCOL_HEAD;

typedef struct
{
	int		m_nPlayerIdx;
	int		m_nCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_APPLY_CREATE;

typedef struct
{
	int		m_nPlayerIdx;
	DWORD	m_dwNpcID;
} STONG_SERVER_TO_CORE_APPLY_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	DWORD	m_dwNameID;
	BYTE	m_bGender;
} STONG_SERVER_TO_CORE_CHECK_ADD_CONDITION;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CREATE_SUCCESS;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	DWORD	m_dwNameID;
} STONG_SERVER_TO_CORE_REFUSE_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nInfoID;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_GET_INFO;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szMasterName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitleName[defTONG_TITLE_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_ADD_SUCCESS;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_BE_INSTATED;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_BE_KICKED;

typedef struct
{
	int		m_nPlayerIdx;
	BOOL	m_bSuccessFlag;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_LEAVE;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CHANGE_AS;

typedef struct
{
	DWORD	m_dwTongNameID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CHANGE_MASTER;


typedef struct
{
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	BYTE	m_btNewCamp;
} STONG_SERVER_TO_CORE_CHANGE_CAMP;

typedef struct
{
	DWORD	m_dwTongNameID;
	BYTE	m_bGender;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CHANGE_MEMBER_TITLE;

typedef struct
{
	int		m_nPlayerIdx;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_CHANGE_ONE_TITLE;

typedef struct
{	
	DWORD	m_dwTongNameID;
	int		m_nMoney;
} STONG_SERVER_TO_CORE_SAVE_MONEY_FAIL;

typedef struct
{	
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
} STONG_SERVER_TO_CORE_SAVE_MONEY;

typedef struct
{	
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
} STONG_SERVER_TO_CORE_GET_MONEY;
// End add

typedef struct
{	
	DWORD	m_dwMoney;
	BYTE	m_nFlag;
	BYTE	m_nCamp;
	BYTE	m_nFigure;
	BYTE	m_nPos;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szMaster[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_LOGIN;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_bGender;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];
} TONG_APPLY_CREATE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
} TONG_APPLY_ADD_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_INSTATE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btFailId;
} TONG_CREATE_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];
} TONG_CREATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szMaster[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_Add_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} TONG_APPLY_DISMISS_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
} TONG_DISMISS_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;
	DWORD	m_dwNameID;
	BYTE	m_btFlag;
} TONG_ACCEPT_MEMBER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btInfoID;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
	char	m_szBuf[defTONG_NAME_MAX_LENGTH + defPLAYER_NAME_MAX_LENGTH + 2];
} TONG_APPLY_INFO_COMMAND;

typedef struct
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_ONE_LEADER_INFO;

typedef struct
{
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_ONE_MEMBER_INFO;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_bStatus[defTONG_MAX_DIRECTOR];
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	TONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR];
} TONG_HEAD_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	BYTE	m_bStatus[defTONG_ONE_PAGE_MAX_NUM];
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	TONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MANAGER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	BYTE	m_bStatus[defTONG_ONE_PAGE_MAX_NUM];
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	TONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MEMBER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btJoinFlag;
	BYTE	m_btFigure;
	BYTE	m_btCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szMaster[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_SELF_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_INSTATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_KICK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_KICK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_LEAVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_CHANGE_MASTER_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int		m_nPrice;
	BYTE	m_btNewCamp;
} TONG_CHANGE_CAMP_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_bGender;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
} TONG_CHANGE_MEMBER_TITLE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_bFigureFlag;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
} TONG_CHANGE_ONE_TITLE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_SAVE_MONEY_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_APPLY_GET_MONEY_COMMAND;

// End add

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
} TONG_CHANGE_MASTER_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwNpcID;
	WORD	m_wGoldFlag;
} NPC_GOLD_CHANGE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
	int		nChange;
} ITEM_DURABILITY_CHANGE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	dwItemID;
} ITEM_REPAIR;

typedef struct
{
	BYTE	ProtocolType;
	BOOL	m_bClear;
	USHORT	m_nId;
	BOOL	m_bValue;
} TASK_VALUE_SYNC;

typedef struct tagPLAYER_COMMAND
{
	BYTE			ProtocolType;
	int				m_nMsgID;
	int				m_nValue;
} VALUE_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwID;
	int		m_nPosX;
	int		m_nPosY;
	BYTE	m_btForce;
	int		m_nOffX;
	int		m_nOffY;
} FIND_PATH_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nMpsX;
	int		m_nMpsY;
} PLAYER_POS_COMMAND;

typedef struct
{
	BYTE	ProtocolType;	
	DWORD	m_dwNpcID;
	int		m_nMpsX;
	int		m_nMpsY;
} PLAYER_POS_SYNC;

typedef struct 
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BOOL	m_bRideFlag;
} PLAYER_RIDE_COMMAND;

typedef struct 
{
	BYTE	ProtocolType;
	BYTE	m_bRideFlag;
} PLAYER_RIDE_SYNC;

typedef struct
{
	BYTE	ProtocolType;	
	DWORD	m_dwNpcID;
	int		m_nBloodNo;
} BLOOD_SYNC;

typedef struct
{
	BYTE	ProtocolType;	
	bool	m_bClear;
} PLAYER_REFRESH_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btExeId;
	char	m_szContent[80];
} EXE_SCRIPT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btType;
	DWORD	m_dwItemID;
	UINT	m_uChange;
} ITEM_CHANGE_INFO;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwItemID;
	int		m_btNum; // fix gioi han max number tach item by kinnox;
} ITEM_BREAK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;	
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_btType;
	int		m_nPwd;
	int		m_nPwdConfirm;
} PASSWORD_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_bFlag;
} NPC_MASK_LOCK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BOOL	m_bHide;
} HIDE_MASK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwItemID;
	int		m_nPrice;
} MARK_PRICE_ITEM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	char	m_szAdvText[16];
} ADV_TEXT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bOpen;
} PLAYER_SHOP_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
	int		m_nDataIndex;
} VIEW_SHOP_COMMAND;

typedef struct
{
	/*
	DWORD	m_dwID;				
	BYTE	m_btGenre;			
	short	m_nDetail;			
	BYTE	m_btParticur;		
	BYTE	m_btSeries;			
	BYTE	m_btLevel;			
	BYTE	m_btLuck;			
	BYTE	m_btMagicLevel[6];	
	WORD	m_wVersion;			
	DWORD	m_dwRandomSeed;
	short	m_nGoldId;
	BYTE	m_btStackNum;
	KTime	m_ExpiredTime;
	int		m_nIdx;
	int		m_nShopPrice;
	BYTE	m_btX;
	BYTE	m_btY;
	*/
	DWORD	m_ID;				
	char	m_Genre;			
	short	m_Detail;	
	char	m_Series;
	BYTE	m_Level;
	DWORD	m_Luck;
	BYTE	m_MagicLevel[6];	
	WORD	m_Version;			
	WORD	m_Durability;		
	DWORD	m_RandomSeed;
	WORD	m_Record;
	DWORD	m_ExpiredTime;
	int		m_StorePrice;
	BYTE	m_X;
	BYTE	m_Y;
	//them SendStackNum fix by player shop by kinnox;
	int		m_StackNum;
	//
} SViewShopItemInfo;

typedef struct
{
	BYTE				ProtocolType;
	DWORD				m_dwNpcID;
	BYTE				m_btCount;
	short				m_nDataIndex;
	SViewShopItemInfo	m_sInfo[defMAX_SHOPITEM_PER_UPDATE];
} VIEW_SHOP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD	m_dwNpcID;
	DWORD	m_dwItemID;
} BUY_PLAYER_SHOP_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nSubWorldID;
	BYTE	m_btDataCount;
} BATTLE_REPORT_INFO_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	int		m_nMpsX;
	int		m_nMpsY;
} NPC_RANDMOVE_SYNC;


typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btUIId;
} CLOSE_UI_SYNC;

typedef struct//AutoAI by kinnox;
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bAuto;
	BYTE	m_bActive;
} S2CPLAYER_REQUEST_AUTO;

typedef struct //AutoAI by kinnox;
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bAuto;
	BYTE	m_bActive;
} C2SPLAYER_REQUEST_AUTO;

typedef struct//AutoAI by kinnox;
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	DWORD   nIdSubWorld;
}C2SPLAYER_AI_BACKTOTOWN;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nPW;			
}CREPW_BOX,LOGINPW_BOX;//LockPlayer by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_bLockPlayer;			
	BOOL	m_bPass;			
}S2C_LOCKPLAYER;//LockPlayer by kinnox;

typedef struct
{
	BYTE	ProtocolType;	
	DWORD	dwID;
	DWORD	dwTimePacker;
	BYTE	m_bLockPlayer;	
}C2S_LOCKPLAYER;//LockPlayer by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nPW;			
	int		m_nPWNew;			
}CHAPW_BOX;//LockPlayer by kinnox;

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_nNum;		
}REPOSIPLAYER_SYNC;//load 3 storebox by kinnox; S2C

typedef struct
{
	BYTE	ProtocolType;		
	BYTE	m_nNum;		
}EXPANDPLAYER_SYNC;//Expandbox by kinnox;

typedef struct
{ 
	BYTE ProtocolType;
	BYTE	nType;
	char	Value[32];
	char	Value1[256];
	char	Value2[64];
} S2C_GIVE_BOX;//GiveBox by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	char	szFunc[32];	
}GIVEBOX_SCRIPT;//GiveBox by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dw_ID;
	DWORD	dwTimePacker;
	DWORD	dwID;
	int		nX;
	int		nY;
}GIVEBOX_CMD;//GiveBox by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	char	szFunc[32];	
}REWARD_SCRIPT;//Reward by kinnox;

typedef struct
{ 
	BYTE ProtocolType;
	char	Value[64];
	char	Value1[32];
	char	Value2[32];
	char	Value3[32];
} S2C_REWARD_BOX;//Reward by kinnox;

typedef struct
{ 
	BYTE ProtocolType;
	char	Value[32];
	int		Value1;
	int		nIndexNpc;
	char	Value2[64];
} S2C_TIME_BOX;//TimeBox by kinnox;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	char	szFunc[32];	
	int		nIndexNpc;	
}C2S_TIME_BOX;//TimeBox by kinnox;

typedef struct
{ 
	BYTE	ProtocolType;
	BYTE	nType;
	int		nValue;
} S2C_PLAYER_POINT_SYNC;

//Report Battle by kinnox;
typedef struct
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

} PLAYER_DATA_TOP_TK_NEW;

typedef struct
{
	BYTE			ProtocolType;
    PLAYER_DATA_TOP_TK_NEW			m_DataTop;

} PLAYER_SEND_TOP_TK_NEW;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
} OPEN_TOPBATTLE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwID;
	DWORD	dwTimePacker;
	int		m_nID;
} AVATAR_PLAYER;//Player Avatar by kinnox;


typedef struct
{ 
	BYTE ProtocolType;
	char	Value[32];
	char	Value1[256];
} S2C_NOTIFICATION;//Notification by kinnox;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			dwID;
	DWORD			dwTimePacker;
	DWORD			nID;
	int				nX;
	int				nY;
} RECOVERY_BOX_CMD;//TrembleItem by kinnox;

void SendClientCmdSell(int nID);
void SendClientCmdBuy(int nBuyIdx, int nPlace, int nX, int nY);
void SendClientCmdRun(int nX, int nY);
void SendClientCmdWalk(int nX, int nY);
void SendClientCmdSkill(int nSkillID, int nX, int nY);
//void SendClientCmdPing();
void SendClientCmdSit(int nSitFlag);
void SendClientCmdMoveItem(void* pDownPos, void* pUpPos);
void SendClientCmdQueryLadder(DWORD	dwLadderID);
void SendClientCmdRequestNpc(int nID);
void SendClientCmdJump(int nX, int nY);
void SendClientCmdStoreMoney(int nDir, int nMoney);
//void SendClientCmdRevive(int nReviveType);
void SendClientCmdRevive();
void SendObjMouseClick(int nObjID, DWORD dwRegionID);
void SendClientCmdRepair(DWORD dwID);
void SendClientCmdRide(BOOL bFlagRide);
void SendClientCmdExeScript(int nType, const char* pszContent);
void SendClientCmdBreakItem(DWORD dwID, int btNum); // fix gioi han max number tach item by kinnox;
void SendClientCmdMarkPriceItem(DWORD dwItemID, int nPrice);
void SendClientCmdMakeAdvText(const char* lpszText);
void SendClientCmdBuyPlayerShop(DWORD dwItemID);
void SendClientCmdAutoPlay(BOOL nbAuto, BOOL nbActive);//AutoAI by kinnox;
void SendClientCreatPWCmd(int nPW);//LockPlayer by kinnox;
void SendClientLoginPWCmd(int nPW);//LockPlayer by kinnox;
void SendClientChaPWCmd(int nPW,int nPWNew);//LockPlayer by kinnox;
void SendClientLockPWCmd(BOOL bLock);//LockPlayer by kinnox;
void SendClientGiveBoxScript(char*szFunc);//GiveBox by kinnox;
void SendClientGiveBoxCmd(DWORD dwID, int nX, int nY);//GiveBox by kinnox;
void SendClientReWardScript(char*szFunc);//Reward by kinnox;
void SendClientCmdTimeBox(char*szFunc,int nIndexNpc);//TimeBox by kinnox;
void SendClientCmdOpenTopTKNew(); //Report Battle by kinnox;	
void SendClientCPSetImageCmd(int ID);//Player Avatar by kinnox;
void SendClientRecoveryBox(DWORD nID, int nX, int nY);//TrembleItem by kinnox;
extern	int	g_nProtocolSize[MAX_PROTOCOL_NUM];
#pragma pack(pop, enter_protocol)
#endif
