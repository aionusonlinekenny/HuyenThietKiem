//#include "kstdafx.h"
#ifndef S3DBINTERFACE_H
#define S3DBINTERFACE_H
#define IN
#define OUT
#include <map>
#include <string>
#include <vector>
#include "KProtocol.h"
//#define SAVE_ITEM_MAGICATTRIB_TO_DB

class KTabFileCtrl;
class KIniFile;

#pragma pack( push, 1 )

enum ROLE_LOG
{
	log_create_role = 1,
	log_begin_game,
	log_save_game,
};

enum 
{
	create_role,
	save_role,
};

enum
{
	must_norole,
	must_haverole,
	nolimited,
};

struct TIntDBValue
{
	BYTE btUnitType;
};

struct TRoleBaseInfo
{
	DWORD	dwId;
	char	szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	szMateName[defPLAYER_NAME_MAX_LENGTH + 1];
	bool	bSex;		
	char	caccname[32];
	BYTE	nFirstSect;
	BYTE	nSect;
	bool	cFightMode;
	BYTE	cUseRevive;
	BYTE	cIsExchange;
	BYTE	cPkStatus;
	WORD	ijoincount;
	BYTE	isectrole;
	WORD	wtitleid;
	WORD	irevivalid;
	int 	irevivalx;
	int 	irevivaly;
	WORD	ientergameid;
	int		ientergamex;
	int		ientergamey;
	UINT	isavemoney;
	UINT	imoney;
	BYTE	ifiveprop;
	BYTE	iteam;
	BYTE	ifightlevel;
	UINT	fightexp;
	BYTE	ileadlevel;
	int		ileadexp;
	int		iliveexp;
	WORD	ipower;
	WORD	iagility;
	WORD	iouter;
	WORD	iinside;
	BYTE	iluck;
	int		imaxlife;
	int		imaxstamina;
	int		imaxinner;
	int		icurlife;
	int		icurstamina;
	int		icurinner;
	BYTE	ipkvalue;
	WORD	ileftprop;
	WORD	ileftfight;
	WORD	ileftlife;
	WORD	nSectStat;
	WORD	nWorldStat;
	WORD	nKillPeopleNumber;
	bool	nForbiddenFlag;
	DWORD	dwTongID;
	BYTE	btTransLife;
	BYTE	iexboxrole;
	int		lockboxrole;//LockPlayer by kinnox;
	BYTE	iexpandboxrole;//Expandbox by kinnox;
	short	ihelmres;//Player Avatar by kinnox;
};

struct TRoleData
{
	TRoleBaseInfo BaseInfo;
	DWORD	dwVersion;
	bool	bBaseNeedUpdate;
	short	nFightSkillCount;
	short	nLiveSkillCount;
	BYTE	nTaskCount;
	int		nPartnerCount;
	short	nItemCount;
	short	nFriendCount;
	int		dwTaskOffset;
	int		dwLSkillOffset;
	int		dwFSkillOffset;
	int		dwItemOffset;
	int		dwFriendOffset;
	int		dwDataLen;
	BYTE	pBuffer[1];
};

struct TDBTaskData
{
	WORD	m_nTaskId;
	int		m_nTaskValue;
};

//===========================
//
//===========================
struct TDBItemData
{
	int	igenre;
	int	ilocal;
	int	ix;
	int	iy;
	int	iseries;
	int	ilevel;
	int	iversion;
	int	imagiclevel1;
	int	imagiclevel2;
	int	imagiclevel3;
	int	imagiclevel4;
	int	imagiclevel5;
	int	imagiclevel6;
	int	ilucky;
	//
	int	idetailtype;
	int	irecord;
	int	idurability;
	//
	int		iparam1;
	int		ishopprice;
	DWORD	irandseed;
	DWORD	ibindstate;
	DWORD	iexpiredtime;
};

//===========================
//
//===========================
struct TDBFriendsData
{
	short FriendCount;
	void * pBuffer;
	short Offset[1];
};

//===========================
// Modify by lzlsky301
//===========================
struct TDBSkillData
{
	short	m_nSkillId;
	BYTE	m_nSkillLevel;
};

enum 
{
/*==============================================================*/
/* Table: EquipBaseInfo                                         */
/*==============================================================*/
   EQUIPBASEINFO_IID                                             ,
   EQUIPBASEINFO_IMAINID                                        ,
   EQUIPBASEINFO_ICLASS                                      ,
   EQUIPBASEINFO_IVALUE1                        ,
   EQUIPBASEINFO_IVALUE2                        ,
   EQUIPBASEINFO_IVALUE3                        ,
/*==============================================================*/
/* TABLE: EQUIPEFFICINFO_                                        */
/*==============================================================*/
   EQUIPEFFICINFO_IID                                             ,
   EQUIPEFFICINFO_IMAINID                                        ,
   EQUIPEFFICINFO_ICLASS                                      ,
   EQUIPEFFICINFO_IVALUE1                        ,
   EQUIPEFFICINFO_IVALUE2                        ,
   EQUIPEFFICINFO_IVALUE3                        ,

/*==============================================================*/
/* TABLE: EQUIPREQUIREINFO_                                     */
/*==============================================================*/
   EQUIPREQUIREINFO_IID                                             ,
   EQUIPREQUIREINFO_IMAINID                                        ,
   EQUIPREQUIREINFO_ICLASS                                      ,
   EQUIPREQUIREINFO_IVALUE1                        ,
   EQUIPREQUIREINFO_IVALUE2                        ,
   EQUIPREQUIREINFO_IVALUE3                        ,
/*==============================================================*/
/* TABLE: EQUIPMENTS                                            */
/*==============================================================*/
   EQUIPMENTS_IID                                             ,
   EQUIPMENTS_CUSERCODE                                         ,
   EQUIPMENTS_IEQUIPCLASSCODE                                   ,
   EQUIPMENTS_ILOCAL                                      ,
   EQUIPMENTS_IX                                            ,
   EQUIPMENTS_IY                                            ,
   EQUIPMENTS_IEQUIPCODE                                     ,
   EQUIPMENTS_IDETAILTYPE                    ,
   EQUIPMENTS_IPARTICULARTYPE                ,
   EQUIPMENTS_ILEVEL                         ,
   EQUIPMENTS_ISERIES                        ,
   EQUIPMENTS_IEQUIPVERSION                  ,
   EQUIPMENTS_IRANDSEED                      ,
   EQUIPMENTS_IPARAM2                        ,
   EQUIPMENTS_IPARAM3                        ,
   EQUIPMENTS_IPARAM5                        ,
   EQUIPMENTS_IPARAM4                        ,
   EQUIPMENTS_IPARAM6                        ,
   EQUIPMENTS_IPARAM1                        ,
   EQUIPMENTS_ILUCKY                         ,
 
/*==============================================================*/
/* TABLE: FIGHTSKILL                                            */
/*==============================================================*/
   FIGHTSKILL_IID                                             ,
   FIGHTSKILL_CUSERCODE                                         ,
   FIGHTSKILL_IFIGHTSKILL                                 ,
   FIGHTSKILL_IFIGHTSKILLLEVEL                            ,

/*==============================================================*/
/* TABLE: FRIENDLIST_                                           */
/*==============================================================*/
   FRIENDLIST_IID                                             ,
   FRIENDLIST_CUSERCODE                                         ,
   FRIENDLIST_CFRIENDCODE                                       ,


/*==============================================================*/
/* TABLE: LIFESKILL                                             */
/*==============================================================*/

   LIFESKILL_IID                                             ,
   LIFESKILL_CUSERCODE                                         ,
   LIFESKILL_ILIFESKILL                                  ,
   LIFESKILL_ILIFESKILLLEVEL                             ,
/*==============================================================*/
/* TABLE: ROLEINFO                                             */
/*==============================================================*/
   ROLEINFO_IID                                             ,
   ROLEINFO_CUSERCODE                                         ,
   ROLEINFO_BSEX                                            ,
   ROLEINFO_CALIAS                         ,
   ROLEINFO_IFIRSTSECT                     ,
   ROLEINFO_ISECT                          ,
   ROLEINFO_IJOINCOUNT                     ,
   ROLEINFO_ISECTROLE                      ,
   ROLEINFO_IGROUPCODE                     ,
   ROLEINFO_IGROUPROLE                     ,
   ROLEINFO_IREVIVALID                     ,
   ROLEINFO_CPARTNERCODE                   ,
   ROLEINFO_ISAVEMONEY                                     ,
   ROLEINFO_IMONEY                                         ,
   ROLEINFO_IFIVEPROP                      ,
   ROLEINFO_ITEAM                          ,
   ROLEINFO_IFIGHTLEVEL                    ,
   ROLEINFO_FIGHTEXP                      ,
   ROLEINFO_ILEADLEVEL                     ,
   ROLEINFO_ILEADEXP                       ,
   ROLEINFO_ILIVEEXP                       ,
   ROLEINFO_IPOWER                         ,
   ROLEINFO_IAGILITY                       ,
   ROLEINFO_IOUTER                         ,
   ROLEINFO_IINSIDE                        ,
   ROLEINFO_ILUCK                          ,
   ROLEINFO_IMAXLIFE                       ,
   ROLEINFO_IMAXSTAMINA                    ,
   ROLEINFO_IMAXINNER                      ,
   ROLEINFO_ILEFTPROP                      ,
   ROLEINFO_ILEFTFIGHT                     ,
   ROLEINFO_ILEFTLIFE                      ,
   ROLEINFO_CACCNAME                       ,
   ROLEINFO_IFINISHGAME                    ,
   ROLEINFO_DLOGINTIME                     ,
   ROLEINFO_DBEGINDATE                     ,
   ROLEINFO_DENDDATE                       ,
   ROLEINFO_IREVIVALX                      ,
   ROLEINFO_IREVIVALY                      ,
   ROLEINFO_IARMORRES                      ,
   ROLEINFO_IWEAPONRES                     ,
   ROLEINFO_IHELMRES                       ,

/*==============================================================*/
/* TABLE: TASKLIST                                             */
/*==============================================================*/
   TASKLIST_IID                                             ,
   TASKLIST_CUSERCODE                                         ,
   TASKLIST_ITASKCODE                                   ,
   TASKLIST_IDEGREE                                     ,

};



//	Ini文件各字段定义---------------------------------------------------
#define SECTION_ROLE		"ROLE"	

#define SECTION_ITEMLIST	"ITEMS"	
#define SECTION_ITEM		"ITEM" // + id
#define KEY_COUNT			"COUNT"
#define KEY_BASELIST		"BLISTS"
#define KEY_EFFICLIST		"ELISTS"
#define KEY_REQUIRELIST		"RLISTS"

#define SECTION_BASEINFO	"IB" //IB1_2
#define SECTION_EFFICEINFO	"IE"
#define SECTION_REQUIREINFO	"IR"


#define SECTION_FRIENDLIST	"FRIENDS"
#define KEY_FRIEND			"F" // + id

#define SECTION_TASKLIST	"TASKS"
#define KEY_TASKID			"T" // + id
#define KEY_TASKVALUE		"V" // + id

#define SECTION_FIGHTSKILLLIST		"FSKILLS"
#define KEY_FIGHTSKILL		"S"
#define KEY_FIGHTSKILLLEVEL	"L"

#define SECTION_LIFESKILLLIST		"LSKILLS"
#define KEY_LIFESKILL				"S" // + id
#define	KEY_LIFESKILLLEVEL			"L" // + id
//------------------------------------------------------------------------------
#pragma warning (disable: 4512)
#pragma warning (disable: 4786)

#ifndef __linux
#ifdef S3DBINTERFACE_EXPORTS
#define S3DBI_API __declspec(dllexport)
#else
#define S3DBI_API __declspec(dllimport)
#endif
#else
#define S3DBI_API
#endif

//改回使用core\kprotocol.h里的S3DBI_RoleBaseInfo
/*struct S3DBI_RoleBaseInfo
{
	char szRoleName[32];
	int	 nSex;
	int	 nHelmType;
	int	 nArmorType;
	int	 nWeaponType;
	int	 nLevel;
};*/


struct S3DBI_DATAS
{
	char * strNames;
	char * strValues;
	int	   nDataNum;
};


enum  S3DBI_TABLENAME
{
	TABLE_ACCOUNT,
	TABLE_CARD,
	TABLE_EQUIPBASEINFO,
	TABLE_EQUIPEFFICINFO,
	TABLE_EQUIPMENT,
	TABLE_EQUIPREQUIREINFO,
	TABLE_FIGHTSKILL,
	TABLE_FRIEND,
	TABLE_LIFESKILL,
	TABLE_LIST,
	TABLE_ROLE,
	TABLE_TASK,
	TABLE_TASK_LIST,
};

enum S3DBI_LISTNAME
{
	LIST_TASK,
	LIST_FRIEND,
	LIST_FIGHT,
	LIST_LIFESKILL,
	LIST_EQUIPMENT,
	LIST_EQUIPMENTREQINFO,
	LIST_EQUIPMENTBASEINFO,
	LIST_EQUIPMENTEFFICINFO,
};
/*
extern DBTableKeyMap  g_RoleTableKeyMap;

typedef std::map<std::string, int>	DBTableKeyMap;

typedef std::map<int, std::string>	DBStringMap;
typedef std::map<int, int>			DBIntegerMap;
*/
/*struct TDBTaskData
{
	int m_nTaskId;
	int	m_nTaskValue;
};


struct TDBTableData
{
	DBStringMap			m_StringDatas;
	DBIntegerMap		m_IntegerDatas;
};
struct TDBEquiptData;
typedef std::vector<KMagicAttrib>		TMagicAttribArray;
typedef std::vector<TDBEquiptData>		TDBEquiptArray;
typedef std::vector<TDBSkillData>		TDBSkillArray;
typedef std::vector<std::string>					TDBFriendArray;
typedef std::vector<TDBTaskData>			TDBTaskArray;

extern DBTableKeyMap g_TableIntegerKeyMap;
extern DBTableKeyMap g_TableStringKeyMap;
struct TDBEquiptData : TDBTableData
{	
	TMagicAttribArray	m_mapRequestInfo;
	TMagicAttribArray	m_mapBaseInfo;
	TMagicAttribArray	m_mapEfficInfo;
};

struct TDBPlayerData :TDBTableData
{
	TDBEquiptArray		m_mapEquipt;
	TDBSkillArray		m_mapFightSkill;
	TDBSkillArray		m_mapLiveSkill;
	TDBFriendArray		m_mapFriend;
	TDBTaskArray		m_mapTask;
};

*/
#define S3_DB_TABLENAME  S3DBI_TABLENAME
S3DBI_API int S3DBI_InitDBInterface();			//初始化数据库引擎
S3DBI_API void S3DBI_ReleaseDBInterface();		//释放数据库引擎

//获得角色的信息并以Ini文件的格式放置
//S3DBI_API int	S3DBI_GetRoleInfo(IN char * strUser, IN OUT KIniFile * pIniFile);
S3DBI_API void *	S3DBI_GetRoleInfo(IN OUT BYTE * pRoleBuffer, IN char * strUser, IN OUT int &nBufLen);

//保存角色的信息，如果数据库不存在该玩家，则增加该玩家
//bAutoInsertWhenNoExistUser 设为TRUE时表示，如果需要保存的该玩家在数据库中并不存在则自动加入到数据库中，FALSE则不增加直接返回错误
//注意INI文件只须存放将需要改动的数据，不需改动的数据将自动保存原状。
S3DBI_API int	S3DBI_SaveRoleInfo(IN BYTE * pRoleBuffer, IN char * strUser, BOOL bAutoInsertWhenNoExistUser);
S3DBI_API void S3DBI_ShowDebugInfo(IN BYTE * pRoleBuffer, char * pShowString);

S3DBI_API BOOL S3DBI_IsRoleNameExisted(char * strUser);
S3DBI_API void S3DBI_SaveRoleLogData(const char * strUser, ROLE_LOG eLogType, time_t logtime = 0);
S3DBI_API time_t S3DBI_GetDataBaseSysTime();

//  eLogType标识日志的类型
//	log_create_role , 记录角色建立的时间
//	log_begin_game,   记录角色本次游戏登入的时间
//	log_save_game,	  记录角色本次游戏过程中上一次存档的时间
//  数据库保存存档的时间的目的是为了

S3DBI_API int S3DBI_GetRoleListOfAccount(char * szAccountName, S3DBI_RoleBaseInfo * RoleBaseList, int nMaxCount);

//删除角色
S3DBI_API int	S3DBI_DeleteRole(char * strUser);




S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  char * strUser);
S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  int iid);


//S3DBI_API int	S3DBI_GetListData(S3DBI_LISTNAME listname, void * pList, int i , ColumnAndValue &cav);
S3DBI_API int	S3DBI_GetCountOfList(S3DBI_LISTNAME listname, void * pList);
//S3DBI_API int	S3DBI_UpdateDataAtList(S3DBI_LISTNAME listname, void * pList, int i , ColumnAndValue &cav);
S3DBI_API int	S3DBI_DeleteDataAtList(S3DBI_LISTNAME listname, void * pList, int i );
//S3DBI_API int	S3DBI_InsertDataAtList(S3DBI_LISTNAME listname, void * pList, ColumnAndValue cav);
S3DBI_API void	S3DBI_ReleaseListBuffer(S3DBI_LISTNAME listname, void * pList);

S3DBI_API int	S3DBI_ClearList(S3DBI_LISTNAME listname, void * pList);

//S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue *pcav);
//S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, char * strUser , ColumnAndValue * pCav);
//S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue *pcav);
//S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, char * strUser, ColumnAndValue *pcav);
S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, int nId);
S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, char * strUser);
//S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename, int nId, ColumnAndValue cav);
//S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename, char * strUser, ColumnAndValue cav);


S3DBI_API int	S3DBI_GetListData(S3DBI_LISTNAME listname, void * pList, int i , KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_DeleteDataAtList(S3DBI_LISTNAME listname, void * pList, int i );
S3DBI_API int	S3DBI_InsertDataAtList(S3DBI_LISTNAME listname, void * pList, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl * pTabFile);
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl *pTabFile);
S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename,  KTabFileCtrl *pTabFile);

#pragma pack( pop )

#endif
