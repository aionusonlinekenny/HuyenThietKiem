#ifndef KPLAYERDEF_H
#define KPLAYERDEF_H
//// --
#define		DBLOADPERTIME_ITEM		10
#define		DBLOADPERTIME_SKILL		10
#define		DBLOADPERTIME_FRIEND	10
#define		DBLOADPERTIME_TASK		10
//// --
#define		SCRIPT_PLAYERID			"PlayerId"
#define		SCRIPT_PLAYERINDEX		"PlayerIndex"
#define		SCRIPT_OBJINDEX			"ObjIndex"
#define		SCRIPT_SUBWORLDINDEX	"SubWorld"
#define		SCRIPT_SUBWORLDID		"SubWorldID"
//fix script nil value by kinnox;
#define SCRIPT_NPCINDEX				"NpcIndex"
#define SCRIPT_NPCID				"NpcId"	
//
//// --
#ifdef _SERVER
#define		MAX_PLAYER				1200
#else
#define		MAX_PLAYER				2
#endif
//// --
#define		CLIENT_PLAYER_INDEX		1
//// --
#define		MAX_TEAM_APPLY_LIST		32
// --
#define	QUEST_BEFORE	0x0000
#define	QUEST_START		0x0001
#define	QUEST_FINISHED	0xffff
// --
#ifndef _SERVER
enum MOUSE_BUTTON
{
	button_left  = 0,
	button_right,
};
#endif

enum SCRIPTACTION  
{
	SCRIPTACTION_UISHOW,			//显示某个UI界面
	SCRIPTACTION_EXESCRIPT,			//执行脚本
};
/*
enum UISEL_DATATYPE
{
	UISEL_nInfo_sOption,		//主信息以数字标识，选项为字符串
	UISEL_nInfo_sOption,		//主信息以数字标识，选项为数字标识	
	
	UISEL_sInfo_nOption,		
	UISEL_sInfo_sOption,
};
*/
enum
{
	ATTRIBUTE_STRENGTH = 0,
	ATTRIBUTE_DEXTERITY,
	ATTRIBUTE_VITALITY,
	ATTRIBUTE_ENGERGY,
	ATTRIBUTE_RESET_ALL,
};

enum	// 队伍创建失败原因
{
	Team_Create_Error_InTeam = 0,	// 已经在队伍中
	Team_Create_Error_SameName,		// 同名错误
	Team_Create_Error_Name,			// 队伍名字非法
	Team_Create_Error_TeamFull,		// 队伍数量太多，容纳不下
	Team_Create_Error_CannotCreate,	// 当前处于不能组队状态
};

enum TEAM_FIGURE
{
	TEAM_CAPTAIN,
	TEAM_MEMBER,
};

enum SYNC_STEP
{
	STEP_BASE_INFO = 0,
	STEP_FIGHT_SKILL_LIST,
	STEP_LIFE_SKILL_LIST,
	STEP_TASK_LIST,
	STEP_ITEM_LIST,
//	STEP_FRIEND_LIST,
	STEP_SYNC_END
};

enum SAVE_STATUS
{
	SAVE_IDLE = 0,
	SAVE_REQUEST,
	SAVE_DOING,
};
#endif
