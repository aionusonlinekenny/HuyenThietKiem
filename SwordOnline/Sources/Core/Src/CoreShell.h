#pragma once
#include "../KJXPathFinder.h" // find way by kinnox;
//=========================================================
//
//=========================================================
enum GAMEDATA_INDEX
{
	GDI_GAME_OBJ_DESC = 1,
	
	GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO,

	GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO,
	
	GDI_PLAYER_BASE_INFO,

	GDI_PLAYER_RT_INFO,

	GDI_PLAYER_RT_ATTRIBUTE,

	GDI_PLAYER_IMMED_ITEMSKILL,	

	GDI_PLAYER_HOLD_MONEY,

	GDI_PLAYER_IS_MALE,	

	GDI_ITEM_TAKEN_WITH,

	GDI_ITEM_IN_STORE_BOX,

	GDI_EQUIPMENT,

	GDI_TRADE_NPC_ITEM,	

	GDI_TRADE_ITEM_PRICE,

	GDI_REPAIR_ITEM_PRICE,

	GDI_TRADE_OPER_DATA,

	GDI_LIVE_SKILL_BASE,

	GDI_LIVE_SKILLS,

	GDI_FIGHT_SKILL_POINT,

	GDI_FIGHT_SKILLS,

	GDI_LEFT_ENABLE_SKILLS,	

	GDI_RIGHT_ENABLE_SKILLS,

	GDI_NEARBY_PLAYER_LIST,	

	GDI_NEARBY_IDLE_PLAYER_LIST,

	GDI_PLAYER_LEADERSHIP,

	GDI_ITEM_IN_ENVIRO_PROP,
	
	GDI_CHAT_SEND_CHANNEL_LIST,

	GDI_CHAT_CURRENT_SEND_CHANNEL,

	GDI_PK_SETTING,	

	GDI_SHOW_PLAYERS_NAME,

	GDI_SHOW_PLAYERS_LIFE,

	GDI_SHOW_PLAYERS_MANA,
	
	GDI_PARADE_EQUIPMENT,
	
	GDI_IMMEDIATEITEM_NUM,

	GDI_SHOW_OBJ_NAME,

	GDI_ITEM_NAME,
	
	GDI_ITEM_GENRE,

	GDI_ITEM_DETAIL,

	GDI_MASK_IMAGE,

	GDI_CHAT_ITEM_INFO,

	GDI_CHAT_ITEM_NAME,

	GDI_CHAT_ITEM_DESC,

	GDI_MARKET_COUNT,

	GDI_MARKET_SELLID,
	
	GDI_MARKET_SELLNAME,

	GDI_TRADE_MARKET_ITEM,

	GDI_ITEM_BREAK_INFO,

	GDI_PLAYER_FIGHT_MODE,

	GDI_PLAYER_HOLD_JBCOIN,

	GDI_PLAYER_HOLD_LOCKCOIN,

	GDI_PLAYER_OPEN_SHOP,

	GDI_PLAYER_SHOP_ADV,

	GDI_PLAYER_RIDE_HORSE,

	GDI_MAP_TYPE,

	GDI_PLAYER_SHOP_ITEM,

	GDI_PLAYER_SHOP_ITEM_COUNT,	

	GDI_ITEM_IN_STORE_BOX1,//load 3 storebox by kinnox;
	
	GDI_ITEM_IN_STORE_BOX2,//load 3 storebox by kinnox;
	
	GDI_ITEM_IN_STORE_BOX3,//load 3 storebox by kinnox;
	
	GDI_ITEM_IN_STORE_BOX4,//load 3 storebox by kinnox;
	
	GDI_ITEM_IN_STORE_BOX5,//load 3 storebox by kinnox;	
	
	GDI_EXPANDBOX_NUM,//Expandbox by kinnox;

	GDI_ITEM_IN_EXPAND_BOX1,//Expandbox by kinnox;
	
	GDI_GIVEBOX_ITEM,//GiveBox by kinnox;

	GDI_NPC_STATE_SKILL,//UiMiniBuff by kinnox;	

	GDI_AVATAR_IMAGE, //Player Avatar by kinnox;

	GDI_BUILD_ITEM,//TrembleItem by kinnox;

	GDI_EXBOX_NUM,
		
	GDI_GAME_OBJ_DESC_INCLUDE_MOUSEHOVER


};

//=========================================================
// 
//=========================================================
enum GAMEDATA_CHANGED_NOTIFY_INDEX
{
	GDCNI_HOLD_OBJECT = 1,

	GDCNI_PLAYER_BASE_INFO,

	GDCNI_PLAYER_RT_ATTRIBUTE,

	GDCNI_PLAYER_IMMED_ITEMSKILL,

	GDCNI_OBJECT_CHANGED,
	
	GDCNI_CONTAINER_OBJECT_CHANGED,

	GDCNI_FIGHT_SKILL_POINT,

	GDCNI_SKILL_CHANGE,
	
	GDCNI_TEAM,

	GDCNI_TEAM_NEARBY_LIST,

	GDCNI_TRADE_START,

	GDCNI_TRADE_DESIRE_ITEM,

	GDCNI_TRADE_OPER_DATA,

	GDCNI_TRADE_END,

	GDCNI_NPC_TRADE,

	GDCNI_GAME_START,
	
	GDCNI_QUESTION_CHOOSE,

	GDCNI_SPEAK_WORDS,

	GDCNI_MSG_ARRIVAL,

	GDCNI_SEND_CHAT_CHANNEL,

	GDCNI_CHAT_GROUP,

	GDCNI_CHAT_FRIEND,

	GDCNI_CHAT_FRIEND_STATUS,
	
	GDCNI_CHAT_MESSAGE,

	GDCNI_SYSTEM_MESSAGE,

	GDCNI_NEWS_MESSAGE,	

	GDCNI_NEWS_MESSAGE_EX,

	GDCNI_SWITCH_CURSOR,

	GDCNI_OPEN_STORE_BOX,
	
	GDCNI_SWITCHING_SCENEPLACE,	

	GDCNI_MISSION_RECORD,

	GDCNI_PK_SETTING,

	GDCNI_VIEW_PLAYERITEM,

	GDCNII_RANK_INDEX_LIST_ARRIVE,

	GDCNII_RANK_INFORMATION_ARRIVE,

	GDCNI_TONG_INFO,

	GDCNI_TONG_MEMBER_LIST,

	GDCNI_TONG_ACTION_RESULT,

	GDCNI_OPEN_TONG_CREATE_SHEET,

	GDCNI_MARKET_TRADE,

	GDCNI_WAIT_TO_REVIVE,

	GDCNI_VIEW_PLAYERSHOP,

	GDCNI_GET_NUMBER_UI,

	GDCNI_GET_STRING_UI,

	GDCNI_REFRESH_MINIMAP,

	GDCNI_SAYDIALOG_END,

	GDCNI_BUYSELL_END,

	GDCNI_UPDATE_AUTO,//AutoAI by kinnox;
	
	GDCNI_UPDATE_PLAYERSELLITEM,//AutoAI by kinnox;
	
	GDCNI_UPDATE_STOREBOX, //LockPlayer by kinnox;
	
	GDCNI_OPEN_GIVE_BOX, //GiveBox by kinnox;
	
	GDCNI_END_GIVE_BOX,//GiveBox by kinnox;	

	GDCNI_OPEN_REWARD_BOX,//Reward by kinnox;
	
	GDCNI_OPEN_TIME_BOX,//TimeBox by kinnox;
	
	GDCNI_SEND_TOP_TK_NEW_ITEM,//Report Battle by kinnox;
	
	GDCNI_LIXIAN_RESULT,//Lixian by kinnox;
	
	GDCNI_OPEN_NOTIFICATION,//Notification by kinnox;

	GDCNI_OPEN_TREMBLE,//TrembleItem by kinnox;

	GDCNI_EXIT_GAME,
	

};

enum GAMEDEBUGCONTROL
{
	DEBUG_SHOWINFO = 1,
	DEBUG_SHOWOBSTACLE,
};

//=========================================================
// 
//=========================================================
enum GAMEOPERATION_INDEX
{
	GOI_EXIT_GAME = 1,

	GOI_SWITCH_OBJECT,

	GOI_MONEY_INOUT_STORE_BOX,

	GOI_PLAYER_ACTION,

	GOI_PLAYER_RENASCENCE,

	GOI_INFORMATION_CONFIRM_NOTIFY,	

	GOI_QUESTION_CHOOSE,

	GOI_USE_ITEM,

	GOI_SET_IMMDIA_SKILL,

	GOI_TONE_UP_SKILL,

	GOI_TONE_UP_ATTRIBUTE,

	GOI_TRADE_INVITE_RESPONSE,

	GOI_TRADE_DESIRE_ITEM,

	GOI_TRADE_WILLING,

	GOI_TRADE_LOCK,

	GOI_TRADE,
	
	GOI_TRADE_CANCEL,

	GOI_TRADE_NPC_BUY,

	GOI_TRADE_NPC_SELL,

	GOI_TRADE_NPC_REPAIR,
	
	GOI_SEND_MSG,
	
	GOI_SET_SEND_CHAT_CHANNEL,
	
	GOI_CHAT_FRIEND_ADD,

	GOI_CHAT_WORDS_TO_FRIEND,

	GOI_CHAT_FRIEND_INVITE,

	GOI_OPTION_SETTING,

	GOI_PLAY_SOUND,

	GOI_PK_SETTING,

	GOI_REVENGE_SOMEONE,

	GOI_SHOW_PLAYERS_NAME,

	GOI_SHOW_PLAYERS_LIFE,

	GOI_SHOW_PLAYERS_MANA,

	GOI_GAMESPACE_DISCONNECTED,
	
	GOI_VIEW_PLAYERITEM,

	GOI_VIEW_PLAYERITEM_END,

	GOI_FOLLOW_SOMEONE,

	GOI_QUERY_RANK_INFORMATION,

	GOI_SHOW_OBJ_NAME,

	GOI_EXESCRIPT_BUTTON,

	GOI_PLAYER_ITEM_BREAK,

	GOI_PLAYER_BIND_ITEM,

	GOI_HIDE_MASK,

	GOI_MARK_PRICE_ITEM,

	GOI_MAKE_ADV_TEXT,

	GOI_SHOP_WILLING,

	GOI_VIEW_PLAYERSHOP,

	GOI_VIEW_PLAYERSHOP_END,

	GOI_BUY_PLAYER_SHOP,

	GOI_ASK_CLIENT_NUMBER_CHOOSE,

	GOI_ASK_CLIENT_STRING_CHOOSE,

	GOI_CHANGE_TAX_CHOOSE,
	
	GOI_PLAYER_CREATE_PWD,//LockPlayer by kinnox;
	
	GOI_PLAYER_CHANGE_PWD,//LockPlayer by kinnox;
	
	GOI_PLAYER_LOGIN_LOCK_PWD,//LockPlayer by kinnox;
	
	GOI_PLAYER_LOCK_PWD,//LockPlayer by kinnox;

	GOI_GIVEBOX_SCRIPT,//GiveBox by kinnox;
	
	GOI_GIVEBOX_COMMAND,//GiveBox by kinnox;	
	
	GOI_REWARD_CMD_SCRIPT,//Reward by kinnox;
	
	GOI_TIME_BOX,//TimeBox by kinnox;	
	
	GOI_OPEN_TOP_TK_NEW,//Report Battle by kinnox;	

	GOI_SET_IMAGE_PLAYER,//Player Avatar by kinnox;

	GOI_RECOVERY_BOX_COMMAND,//TrembleItem by kinnox;
};

//=========================================================
// 
//=========================================================
enum GAME_SCENE_MAP_OPERATION_INDEX
{
	GSMOI_SCENE_TIME_INFO,

	GSMOI_SCENE_MAP_INFO,

	GSMOI_IS_SCENE_MAP_SHOWING,

	GSMOI_PAINT_SCENE_MAP,

	GSMOI_SCENE_MAP_FOCUS_OFFSET,

	GSMOI_SCENE_FOLLOW_WITH_MAP,

	GSMOI_PAINT_LINE,

	GSMOI_SCENE_FIND_POS,//PaintFindPos by kinnox;

	GSMOI_SCENE_CALCFLAG,//PaintFindPos by kinnox;

	GSMOI_PAINT_SCENE_FIND_POS,//PaintFindPos by kinnox;
};

//=========================================================
// 
//=========================================================
enum GAME_TONG_OPERATION_INDEX
{
	GTOI_TONG_CREATE,

	GTOI_TONG_IS_RECRUIT,

	GTOI_TONG_RECRUIT,

	GTOI_TONG_ACTION,

	GTOI_TONG_JOIN_REPLY,

	GTOI_REQUEST_PLAYER_TONG,

	GTOI_REQUEST_TONG_DATA,
	
	GTOI_TONG_CHANGE_CAMP,

	GTOI_TONG_CHANGE_TITLE,

	GTOI_TONG_CHANGE_MONEY,

};

//=========================================================
//
//=========================================================
enum GAME_TEAM_OPERATION_INDEX
{
	TEAM_OI_GD_INFO,

	TEAM_OI_GD_MEMBER_LIST,

	TEAM_OI_GD_REFUSE_INVITE_STATUS,

	TEAM_OI_COLLECT_NEARBY_LIST,

	TEAM_OI_APPLY,

	TEAM_OI_CREATE,

	TEAM_OI_APPOINT,

	TEAM_OI_INVITE,	

	TEAM_OI_KICK,

	TEAM_OI_LEAVE,

	TEAM_OI_CLOSE,

	TEAM_OI_REFUSE_INVITE,

	TEAM_OI_APPLY_RESPONSE,	

	TEAM_OI_INVITE_RESPONSE,
};
//===========================
//AutoAI by kinnox;
//===========================
enum GAME_PLAYERAI_OPERATION_INDEX
{
	GPI_SWITCH_ACTIVE,
	GPI_F_VINCINITY,
	GPI_F_VINCINITY_NUM,
	GPI_F_RADIUS_NUM,
	GPI_F_FOLLOW_TAGET,
	GPI_F_NORMAL,
	GPI_F_PEOPLE,
	GPI_F_SPACE,
	GPI_F_HOLDSPACE,
	GPI_F_MOUSE,
	GPI_F_SUPPORT_SKILLID,
	GPI_F_AURA_SKILLID,
	GPI_F_LEFT_SKILL,
	GPI_F_RIGHTHAND_SKILL,
	GPI_F_RIDER,
	GPI_F_BUFF_AMULET,
	GPI_F_DEFENSE,
	GPI_F_FIGHTBOSS,
	GPI_M_FOLLOW_PEOPLE,
	GPI_M_FOLLOW_PEOPLE_NAME,
	GPI_M_FOLLOW_PEOPLE_RADIUS,
	GPI_M_AROUND,
	GPI_M_AROUND_NUM,
	GPI_M_MOVEPOS,
	GAUTO_AUTO_MOVEMPSID,
	GAUTO_AUTO_MOVEMPSX,	
	GAUTO_AUTO_MOVEMPSY,	
	GPI_R_LIFE,
	GPI_R_EATLIFE,
	GPI_R_MANA,
	GPI_R_EATMANA,
	GPI_R_BTOWNLIF,
	GPI_R_BTOWNMAN,
	GPI_R_TOWLIFV,
	GPI_R_TOWMANV,
	GPI_R_NOBLOOD,
	GPI_R_NOMANA,
	GPI_R_NOGMON,
	GPI_R_NOGMONV,
	GPI_R_DURABI,
	GPI_R_DURABIV,
	GPI_R_NOROOMCK,
	GPI_R_NOROOM,
	GPI_R_ANTIPOISON,
	GPI_R_USETTL,
	GPI_R_MEDIBAG,
	GPI_R_REPAIREQCK,
	GPI_R_SAVEMONEY,
	GPI_R_EMEIBUFF,
	GPI_R_EMEIBUFFNUM,
	GPI_R_EATFULLBOX,
	GPI_R_WARNINGPK,
	GPI_O_PARTYCK,
	GPI_O_PARTYINVITE,
	GPI_O_PARTYLIST,
	GPI_O_PARTYLISTNAME,
	GPI_O_PARTYACCEPT,
	GPI_O_PARTYACCEPT_ALL,
	GPI_O_PARTYACCEPT_WITH_NAME,
	GPI_O_OPTIMAL_NPC,
	GPI_O_OPTIMAL_PLAYER,
	GPI_O_OPTIMAL_ANI,
	GPI_S_BACKTOMAPCK,
	GPI_S_BACKTOMAPEDIT,
	GPI_S_WITHDRAWAL,
	GPI_S_WITHDRAWALEDIT,
	GPI_S_BUYBLOOD_NUM,
	GPI_S_BUYBLOODCK,
	GPI_S_BUYBLOOD_ID,
	GPI_S_BUYMANA_NUM,
	GPI_S_BUYMANACK,
	GPI_S_BUYMANA_ID,
	GPI_S_BUYANTIPOSI_NUM,
	GPI_S_BUYANTIPOSICK,
	GPI_S_BUYANTIPOSI_ID,
	GPI_S_BUYTOWNPORCK,
	GPI_S_BACKTOWAYPOINTCK,
	GPI_S_BACKTOPWAYPOINT_ID,
	GPI_S_BACKTOWAYMAPCK,
	GPI_S_BACKTOPWAYMAP_ID,
	GPI_S_SORTEQUIPMENT_ID,
	GPI_P_BPICKUP,
	GPI_P_PICKUPK,
	GPI_P_FILTEQUIP,
	GPI_P_SAVERAP,
	GPI_P_PRICEITEMCK,
	GPI_P_PRICEITEM,
	GPI_S_LEVELITEMCK,
	GPI_S_LEVELITEM,
	GDI_S_SELL_ITEMCK,
	GDI_S_SELL_ITEM,
	GDI_S_MOVE_ITEMCK,
	GPI_S_MAP_TRAIN,

};
//===========================
//
//===========================
enum GAME_SYSTEM_MESSAGES
{	
	GSM_FIGHT_MODE_ON,

	GSM_FEATURE_BUIDING,

	GSM_PLAYER_RIDEHORSE,

	GSM_MAP_TYPE_ERROR,

	GSM_COMPOUND_ERROR,

	GSM_FORGE_ERROR,
};




//===========================


struct IClientCallback
{
	virtual void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	virtual void ChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc) = 0;
	virtual void MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc) = 0;
	virtual void NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost) = 0;
	virtual void FriendInvite(char* roleName) = 0;
	virtual void AddFriend(char* roleName, BYTE answer) = 0;
	virtual void FriendStatus(char* roleName, BYTE state) = 0;
	virtual void FriendInfo(char* roleName, char* unitName, char* groupname, BYTE state) = 0;
	virtual void AddPeople(char* unitName, char* roleName) = 0;
};

struct _declspec (novtable) iCoreShell
{
	virtual	int	 GetProtocolSize(BYTE byProtocol) = 0;
	virtual int	 Debug(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	virtual int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	virtual void ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam) = 0;
	virtual int FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind) = 0;
	virtual int FindSpecialNPC(char* Name, void* pReturn, int& nKind) = 0;
	virtual int FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind) = 0;
	virtual int ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength) = 0;
	virtual void ApplyAddTeam(void* pPlayer) = 0;
	virtual void TradeApplyStart(void* pPlayer) = 0;
	virtual int UseSkill(int x, int y, int nSkillID, BOOL bFlag = FALSE) = 0;
	virtual int LockSomeoneUseSkill(int nTargetIndex, int nSkillID) = 0;
	virtual int LockSomeoneAction(int nTargetIndex) = 0;
	virtual int LockObjectAction(int nTargetIndex) = 0;
	virtual void GotoWhere(int x, int y, int mode) = 0;
	virtual void GotoWhereDirect(int x, int y, int mode) = 0;	//mode 0 is auto, 1 is walk, 2 is run // find way by kinnox;
	virtual BOOL AutoMove() = 0;// find way by kinnox;
	virtual void Goto(int nDir, int mode) = 0;
	virtual void Turn(int nDir) = 0;
	virtual int ThrowAwayItem() = 0;
	virtual int GetNPCRelation(int nIndex) = 0;
	virtual BOOL GetNPCShopState(int nIndex) = 0;
	virtual int	SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	virtual int	TongOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	virtual int TeamOperation(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	virtual int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	virtual void DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam) = 0;
	virtual void DrawGameSpace() = 0;
	virtual DWORD GetPing() = 0;
	virtual int	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc) = 0;
	virtual void NetMsgCallbackFunc(void* pMsgData) = 0;
	virtual void SetRepresentShell(struct iRepresentShell* pRepresent) = 0;
	virtual void SetMusicInterface(void* pMusicInterface) = 0;
	virtual void SetRepresentAreaSize(int nWidth, int nHeight) = 0;
	virtual int  Breathe() = 0;
	virtual void Release() = 0;
	virtual void SetClient(LPVOID pClient) = 0;
	virtual void SendNewDataToServer(void* pData, int nLength) = 0;
	virtual int	 SystemMessages(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	virtual BOOL CanGetSkill(int nSkillId) = 0;
	virtual int		PAIOperation(unsigned int uOper, unsigned int uParam, int nParam, int zParam) = 0;//AutoAI by kinnox;
	virtual	BOOL 	GetAutoFlag();//Active AutoAI by kinnox;
	virtual BOOL	GetSpaceFlag() = 0;//CheckSpace AutoAI by kinnox;
	virtual int		FindSkillInfo(int nType, int nIndex) = 0;//AutoAI by kinnox;
	virtual	void	GetSkillName(int nSkillId, char* szSkillName) = 0;//AutoAI by kinnox;
	virtual void	SetSortItem(int nType, int nValue, int nPos) = 0;//AutoAI by kinnox;
	virtual	void	SetMoveMap(int nType, int nPos, int nValue) = 0;//AutoAI by kinnox;
	virtual BOOL	CheckMouse() = 0;// fix kiem tra chuot by kinnox	
	virtual int		GetGenreItem(unsigned int uItemId, unsigned int uGenre = 0) = 0; // fix xoa nen item questkey by kinnox	
	virtual POINT	GetStatusLockPlayer() = 0; //LockPlayer by kinnox;	
	virtual int		GetLixian() = 0;//Lixian by kinnox;
	virtual	BOOL	GetPaintFindPos() = 0;//PaintFindPos by kinnox;
	virtual	void	SetPaintFindPos(BOOL nIndex) = 0;//PaintFindPos by kinnox;
	virtual int		SceneMapFindPosOperation(unsigned int uOper, unsigned int uParam, int nParam, bool bCalc = false, bool bLine = false) = 0;//PaintFindPos by kinnox;
	virtual int GetDetailItem(unsigned int uId ) = 0;
	virtual int GetParticularItem(unsigned int uId ) = 0;
	virtual int GetLevelItem(unsigned int uId ) = 0;
	virtual int GetSeriesItem(unsigned int uId ) = 0;
	virtual int GetNumStack(unsigned int uId ) = 0;
	virtual int CheckPositionBarrier(int nMapX, int nMapY) = 0; // Check if position has barrier - returns 0 if no barrier
};

#ifndef CORE_EXPORTS
	extern "C" iCoreShell* CoreGetShell();
#else
	void	CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam);
#endif
