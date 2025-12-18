// -------------------------------------------------------------------------
//	文件名		：	KTongProtocol.h
//	创建者		：	谢茂培 (Hsie)
//	创建时间	：	2003-08-13 15:12:19
//	功能描述	：	
//
// -------------------------------------------------------------------------
#ifndef __KTONGPROTOCOL_H__
#define __KTONGPROTOCOL_H__

#pragma pack(push, 1)

#define		defTONG_PROTOCOL_SERVER_NUM		255
#define		defTONG_PROTOCOL_CLIENT_NUM		255



//---------------------------- tong protocol ----------------------------
enum 
{
	enumC2S_TONG_CREATE = 0,			
	enumC2S_TONG_ADD_MEMBER,			
	enumC2S_TONG_GET_HEAD_INFO,			
	enumC2S_TONG_GET_MANAGER_INFO,		
	enumC2S_TONG_GET_MEMBER_INFO,		
	enumC2S_TONG_INSTATE,				
	enumC2S_TONG_KICK,					
	enumC2S_TONG_LEAVE,					
	enumC2S_TONG_CHANGE_MASTER,			
	enumC2S_TONG_ACCEPT_MASTER,
	enumC2S_TONG_CHANGE_CAMP,
	enumC2S_TONG_CHANGE_MEMBER_TITLE,
	enumC2S_TONG_CHANGE_ONE_TITLE,
	enumC2S_TONG_SAVE_MONEY,
	enumC2S_TONG_GET_MONEY,
	enumC2S_TONG_GET_LOGIN_DATA,
	
	enumC2S_TONG_NUM,					// 数量
};

enum
{
	enumS2C_TONG_CREATE_SUCCESS = 0,		
	enumS2C_TONG_CREATE_FAIL,				
	enumS2C_TONG_ADD_MEMBER_SUCCESS,		
	enumS2C_TONG_ADD_MEMBER_FAIL,			
	enumS2C_TONG_HEAD_INFO,					
	enumS2C_TONG_MANAGER_INFO,				
	enumS2C_TONG_MEMBER_INFO,				
	enumS2C_TONG_BE_INSTATED,				
	enumS2C_TONG_INSTATE,					
	enumS2C_TONG_KICK,						
	enumS2C_TONG_BE_KICKED,					
	enumS2C_TONG_LEAVE,						
	enumS2C_TONG_CHECK_CHANGE_MASTER_POWER,	
	enumS2C_TONG_CHANGE_MASTER_FAIL,		
	enumS2C_TONG_CHANGE_AS,					
	enumS2C_TONG_CHANGE_MASTER,				
	enumS2C_TONG_CHANGE_CAMP,
	enumS2C_TONG_CHANGE_MEMBER_TITLE,
	enumS2C_TONG_CHANGE_ONE_TITLE,
	enumS2C_TONG_SAVE_MONEY_FAIL,
	enumS2C_TONG_SAVE_MONEY,
	enumS2C_TONG_GET_MONEY,
	enumS2C_TONG_LOGIN_DATA,				

	enumS2C_TONG_NUM,					
};
//-------------------------- tong protocol end --------------------------

//friend protocol
enum 
{
	friend_c2c_askaddfriend,	//请求加为好友
	friend_c2c_repaddfriend,	//同意/拒绝加为好友
	friend_c2s_groupfriend,		//将好友分组
	friend_c2s_erasefriend,		//删除好友

	friend_c2s_asksyncfriendlist,	//请求同步好友列表
	friend_s2c_repsyncfriendlist,	//同步好友列表

	friend_s2c_friendstate,		//好友状态通知

	friend_c2s_associate,		//GS到Relay，自动组合2个人（有方向）
	friend_c2s_associatebevy,	//GS到Relay，自动组合n个人
	friend_s2c_syncassociate,	//Relay到Client，通知组合
};

//extend protocol
enum
{
	extend_s2c_passtosomeone,
	extend_s2c_passtobevy,
};



/////////////////////////////////////////////////////////////////
//friend struct

const int _GROUP_NAME_LEN = _NAME_LEN * 2;


struct ASK_ADDFRIEND_CMD : EXTEND_HEADER
{
	BYTE pckgid;
	char dstrole[_NAME_LEN];
};
struct ASK_ADDFRIEND_SYNC : EXTEND_HEADER
{
	BYTE pckgid;
	char srcrole[_NAME_LEN];
};

enum {answerAgree, answerDisagree, answerUnable};
struct REP_ADDFRIEND_CMD : EXTEND_HEADER
{
	BYTE pckgid;
	char dstrole[_NAME_LEN];
	BYTE answer;	//agree/disagree/unable
};
struct REP_ADDFRIEND_SYNC : EXTEND_HEADER
{
	BYTE pckgid;
	char srcrole[_NAME_LEN];
	BYTE answer;	//agree/disagree/unable
};



//used by GROUP_FRIEND & REP_SYNCFRIENDLIST
enum {specOver = 0x00, specGroup = 0x01, specRole = 0x02};

struct GROUP_FRIEND : EXTEND_HEADER
{
	//format: char seq
	//specGroup标记组，其后接该组好友列表，以\0间隔，specRole标记角色名
	//最后以双\0结束
};


struct ERASE_FRIEND : EXTEND_HEADER
{
	char friendrole[_NAME_LEN];
};


struct ASK_SYNCFRIENDLIST : EXTEND_HEADER
{
	BYTE pckgid;
	BYTE full;
};


struct REP_SYNCFRIENDLIST : EXTEND_HEADER
{
	BYTE pckgid;
	//format: char seq (same as GROUP_FRIEND)
	//specGroup标记组，其后接该组好友列表，以\0间隔，specRole标记角色名
	//最后以双\0结束
};


enum {stateOffline, stateOnline};

struct FRIEND_STATE : EXTEND_HEADER
{
	BYTE state;
	//format: char seq, \0间隔，双\0结束
};

struct FRIEND_ASSOCIATE : EXTEND_HEADER
{
	BYTE bidir;
	//format: string * 3
	//组名
	//角色名 * 2
};

struct FRIEND_ASSOCIATEBEVY : EXTEND_HEADER
{
	//format: char seq, \0间隔，双\0结束
	//组名
	//角色名列表
};

struct FRIEND_SYNCASSOCIATE : EXTEND_HEADER
{
	//format: char seq, \0间隔，双\0结束
	//组名
	//角色名列表
};


/////////////////////////////////////////////////////////////////
//extend struct

struct EXTEND_PASSTOSOMEONE : EXTEND_HEADER
{
	DWORD			nameid;
	unsigned long	lnID;
	WORD			datasize;
};

struct EXTEND_PASSTOBEVY : EXTEND_HEADER
{
	WORD	datasize;
	WORD	playercount;
	//data
	//tagPlusSrcInfo vector
};

//----------------------------- tong struct -----------------------------

struct STONG_CREATE_COMMAND : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BYTE	m_bGender;
	BYTE	m_btCamp;
	BYTE	m_btTongNameLength;
	BYTE	m_btPlayerNameLength;
	char	m_szBuffer[defTONG_NAME_MAX_LENGTH + defPLAYER_NAME_MAX_LENGTH + 2];
};

struct STONG_CREATE_SUCCESS_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BYTE	m_btCamp;
	BYTE	m_btTongNameLength;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
};

struct STONG_CREATE_FAIL_SYNC : EXTEND_HEADER
{
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BYTE	m_btFailID;
};

struct STONG_ADD_MEMBER_COMMAND : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BOOL	m_bGender;
	BYTE	m_btTongNameLength;
	BYTE	m_btPlayerNameLength;
	char	m_szBuffer[defTONG_NAME_MAX_LENGTH + defPLAYER_NAME_MAX_LENGTH + 2];
};

struct STONG_ADD_MEMBER_SUCCESS_SYNC : EXTEND_HEADER
{
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BYTE	m_btCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szMasterName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitleName[defTONG_TITLE_MAX_LENGTH + 1];
};

struct STONG_ADD_MEMBER_FAIL_SYNC : EXTEND_HEADER
{
	DWORD	m_dwPlayerNameID;
	DWORD	m_dwParam;
	BYTE	m_btFailID;
};

struct STONG_GET_TONG_HEAD_INFO_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwNpcID;
	DWORD	m_dwTongNameID;
};

struct STONG_GET_MANAGER_INFO_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
};

struct STONG_GET_MEMBER_INFO_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
};

struct STONG_ONE_LEADER_INFO
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

//================================
//
//================================
struct STONG_ONE_MEMBER_INFO
{	
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_HEAD_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwParam;
	DWORD	m_dwNpcID;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	STONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR];
};

struct STONG_MANAGER_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwParam;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStartNo;
	BYTE	m_btCurNum;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	STONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
};

struct STONG_MEMBER_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	DWORD	m_dwParam;
	DWORD	m_dwMoney;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStartNo;
	BYTE	m_btCurNum;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	STONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
};

struct STONG_INSTATE_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_BE_INSTATED_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;			
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_BE_KICKED_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;			
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_INSTATE_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_KICK_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_KICK_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_LEAVE_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_LEAVE_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	BYTE	m_btSuccessFlag;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHANGE_MASTER_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHECK_GET_MASTER_POWER_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHANGE_MASTER_FAIL_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHANGE_AS_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_ACCEPT_MASTER_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	BYTE	m_btAcceptFalg;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHANGE_MASTER_SYNC : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_CHANGE_CAMP_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	int		m_nPrice;
	BYTE	m_btNewCamp;	
};

struct STONG_CHANGE_CAMP_SYNC : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	BYTE	m_btNewCamp;	
};

struct STONG_CHANGE_MEMBER_TITLE_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	BYTE	m_bGender;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];		
};

struct STONG_CHANGE_MEMBER_TITLE_SYNC : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	BYTE	m_bGender;
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
};

struct STONG_CHANGE_ONE_TITLE_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	BYTE	m_bFigureFlag;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];	
};

struct STONG_CHANGE_ONE_TITLE_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
};

struct STONG_SAVE_MONEY_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_SAVE_MONEY_FAIL_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	int		m_nMoney;
};

struct STONG_SAVE_MONEY_SYNC : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
};

struct STONG_GET_MONEY_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_GET_MONEY_SYNC : EXTEND_HEADER
{	
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	int		m_nMoney;
};
// End add

struct STONG_GET_LOGIN_DATA_COMMAND : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwTongNameID;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

struct STONG_LOGIN_DATA_SYNC : EXTEND_HEADER
{
	DWORD	m_dwParam;
	DWORD	m_dwMoney;
	BYTE	m_btFlag;
	BYTE	m_btCamp;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
	char	m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char	m_szMaster[defPLAYER_NAME_MAX_LENGTH + 1];
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};


//--------------------------- tong struct end ---------------------------


#pragma pack(pop)

#endif // __KTONGPROTOCOL_H__
