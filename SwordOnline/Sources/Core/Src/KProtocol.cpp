#include "KCore.h"

#include "KEngine.h"
#include "KProtocol.h"
#include "KPlayer.h"
#include "KItemList.h"
#include "KLadder.h"

int	g_nProtocolSize[MAX_PROTOCOL_NUM] = 
{
#ifndef _SERVER				// �ͻ��˽��յ��ķ��������ͻ��˵�Э�鳤��
	-1,							// s2c_login,
	-1,							// s2c_logout,
	sizeof(BYTE),				// s2c_syncend,
	sizeof(CURPLAYER_SYNC),		// s2c_synccurplayer,
	-1,							// s2c_synccurplayerskill
	sizeof(CURPLAYER_NORMAL_SYNC),// s2c_synccurplayernormal
	-1,							// s2c_newplayer,
	-1,							// s2c_removeplayer,
	sizeof(WORLD_SYNC),			// s2c_syncworld,
	sizeof(PLAYER_SYNC),		// s2c_syncplayer,
	sizeof(PLAYER_NORMAL_SYNC),	// s2c_syncplayermin,
	-1,	//sizeof(NPC_SYNC),			// s2c_syncnpc,
	sizeof(NPC_NORMAL_SYNC),	// s2c_syncnpcmin,
	sizeof(NPC_PLAYER_TYPE_NORMAL_SYNC),	// s2c_syncnpcminplayer,

	-1,//sizeof(OBJ_ADD_SYNC),	// s2c_objadd,
	sizeof(OBJ_SYNC_STATE),		// s2c_syncobjstate,
	sizeof(OBJ_SYNC_DIR),		// s2c_syncobjdir,
	sizeof(OBJ_SYNC_REMOVE),	// s2c_objremove,
	sizeof(OBJ_SYNC_TRAP_ACT),	// s2c_objTrapAct,

	sizeof(NPC_REMOVE_SYNC),	// s2c_npcremove,
	sizeof(NPC_WALK_SYNC),		// s2c_npcwalk,
	sizeof(NPC_RUN_SYNC),		// s2c_npcrun,
	-1,							// s2c_npcattack,
	-1,							// s2c_npcmagic,
	sizeof(NPC_JUMP_SYNC),		// s2c_npcjump,
	sizeof(NPC_GLIDE_SYNC),		// s2c_npcglide,
	-1,							// s2c_npctalk,
	sizeof(NPC_HURT_SYNC),		// s2c_npchurt,
	sizeof(NPC_DEATH_SYNC),		// s2c_npcdeath,
	sizeof(NPC_CHGCURCAMP_SYNC),// s2c_npcchgcurcamp,
	sizeof(NPC_CHGCAMP_SYNC),	// s2c_npcchgcamp,
	sizeof(NPC_SKILL_SYNC),		// s2c_skillcast,
	-1,							// s2c_playertalk,
	sizeof(PLAYER_EXP_SYNC),	// s2c_playerexp,

	sizeof(PLAYER_SEND_TEAM_INFO),			// s2c_teaminfo,
	sizeof(PLAYER_SEND_SELF_TEAM_INFO),		// s2c_teamselfinfo,
	sizeof(PLAYER_APPLY_TEAM_INFO_FALSE),	// s2c_teamapplyinfofalse,
	sizeof(PLAYER_SEND_CREATE_TEAM_SUCCESS),// s2c_teamcreatesuccess,
	sizeof(PLAYER_SEND_CREATE_TEAM_FALSE),	// s2c_teamcreatefalse,
	sizeof(PLAYER_TEAM_OPEN_CLOSE),			// s2c_teamopenclose,
	sizeof(PLAYER_APPLY_ADD_TEAM),			// s2c_teamgetapply,
	sizeof(PLAYER_TEAM_ADD_MEMBER),			// s2c_teamaddmember,
	sizeof(PLAYER_LEAVE_TEAM),				// s2c_teamleave,
	sizeof(PLAYER_TEAM_CHANGE_CAPTAIN),		// s2c_teamchangecaptain,
	sizeof(PLAYER_FACTION_DATA),			// s2c_playerfactiondata,
	sizeof(PLAYER_LEAVE_FACTION),			// s2c_playerleavefaction,
	sizeof(PLAYER_FACTION_SKILL_LEVEL),		// s2c_playerfactionskilllevel,
	-1,//sizeof(PLAYER_SEND_CHAT_SYNC),			// s2c_playersendchat,
	sizeof(PLAYER_LEAD_EXP_SYNC),			// s2c_playersyncleadexp
	sizeof(PLAYER_LEVEL_UP_SYNC),			// s2c_playerlevelup
	sizeof(PLAYER_TEAMMATE_LEVEL_SYNC),		// s2c_teammatelevel
	sizeof(PLAYER_ATTRIBUTE_SYNC),			// s2c_playersyncattribute
	sizeof(PLAYER_SKILL_LEVEL_SYNC),		// s2c_playerskilllevel
	sizeof(PLAYER_SKILL_REMOVE_SYNC),		// s2c_playerskillremove
	sizeof(ITEM_SYNC),						// s2c_syncitem
	sizeof(ITEM_REMOVE_SYNC),				// s2c_removeitem
	sizeof(PLAYER_MONEY_SYNC),				// s2c_syncmoney
	sizeof(PLAYER_MOVE_ITEM_SYNC),			// s2c_playermoveitem
	-1,										// s2c_playershowui
	sizeof(CHAT_APPLY_ADD_FRIEND_SYNC),		// s2c_chatapplyaddfriend
	sizeof(CHAT_ADD_FRIEND_SYNC),			// s2c_chataddfriend
	-1,//sizeof(CHAT_REFUSE_FRIEND_SYNC),		// s2c_chatrefusefriend
	sizeof(CHAT_ADD_FRIEND_FAIL_SYNC),		// s2c_chataddfriendfail
	sizeof(CHAT_LOGIN_FRIEND_NONAME_SYNC),	// s2c_chatloginfriendnoname
	-1,//sizeof(CHAT_LOGIN_FRIEND_NAME_SYNC),	// s2c_chatloginfriendname
	sizeof(CHAT_ONE_FRIEND_DATA_SYNC),		// s2c_chatonefrienddata
	sizeof(CHAT_FRIEND_ONLINE_SYNC),		// s2c_chatfriendinline
	sizeof(CHAT_DELETE_FRIEND_SYNC),		// s2c_chatdeletefriend
	sizeof(CHAT_FRIEND_OFFLINE_SYNC),		// s2c_chatfriendoffline
	sizeof(ROLE_LIST_SYNC),					// s2c_syncrolelist
	sizeof(TRADE_CHANGE_STATE_SYNC),		// s2c_tradechangestate
	-1, // NPC_SET_MENU_STATE_SYNC			   s2c_npcsetmenustate
	sizeof(TRADE_MONEY_SYNC),				// s2c_trademoneysync
	sizeof(TRADE_DECISION_SYNC),			// s2c_tradedecision
	-1, // sizeof(CHAT_SCREENSINGLE_ERROR_SYNC)s2c_chatscreensingleerror
	sizeof(NPC_SYNC_STATEINFO),				// s2c_syncnpcstate,
	-1,	// sizeof(TEAM_INVITE_ADD_SYNC)		   s2c_teaminviteadd
	sizeof(TRADE_STATE_SYNC),				// s2c_tradepressoksync
	sizeof(PING_COMMAND),					// s2c_ping
	sizeof(NPC_SIT_SYNC),					// s2c_npcsit
	sizeof(SALE_BOX_SYNC),					// s2c_opensalebox
	sizeof(NPC_SKILL_SYNC),					// s2cDirectlyCastSkill
	-1,										// s2c_msgshow
	-1,										// s2c_syncstateeffect
	sizeof(BYTE),							// s2c_openstorebox
	sizeof(NPC_REVIVE_SYNC),				// s2c_playerrevive
	//sizeof(S2C_SHOW_LIEN_TRAM),				//s2c_show_lien_tram
	sizeof(NPC_REQUEST_FAIL),				// s2c_requestnpcfail
	sizeof(TRADE_APPLY_START_SYNC),			// s2c_tradeapplystart
	sizeof(tagNewDelRoleResponse),			// s2c_rolenewdelresponse
	sizeof(ITEM_AUTO_MOVE_SYNC),			// s2c_ItemAutoMove
	sizeof(BYTE),							// s2c_itemexchangefinish
	sizeof(SYNC_WEATHER),					// s2c_changeweather
	sizeof(PK_NORMAL_FLAG_SYNC),			// s2c_pksyncnormalflag
	-1,//sizeof(PK_ENMITY_STATE_SYNC),		// s2c_pksyncenmitystate
	-1,//sizeof(PK_EXERCISE_STATE_SYNC),	// s2c_pksyncexercisestate
	sizeof(PK_VALUE_SYNC),					// s2c_pksyncpkvalue
	sizeof(NPC_SLEEP_SYNC),					// s2c_npcsleepmode
	sizeof(VIEW_EQUIP_SYNC),				// s2c_viewequip
	sizeof(LADDER_DATA),					// s2c_ladderresult
	-1,										// s2c_ladderlist
	sizeof(TONG_CREATE_SYNC),				// s2c_tongcreate
	sizeof(PING_COMMAND),					// s2c_replyclientping
	sizeof(NPC_GOLD_CHANGE_SYNC),			// s2c_npcgoldchange
	sizeof(TASK_VALUE_SYNC),				// s2c_taskvaluesync
	sizeof(FIND_PATH_SYNC),					// s2c_findpathsync
	sizeof(BYTE),							// s2c_stopmove
	sizeof(PLAYER_POS_SYNC),				// s2c_playerpos
	sizeof(PLAYER_RIDE_SYNC),				// s2c_playerridesync
	sizeof(BLOOD_SYNC),						// s2c_bloodsync,
	sizeof(PLAYER_REFRESH_SYNC),			// s2c_refeshplayersync
	sizeof(ITEM_CHANGE_INFO),				// s2c_itemchangeinfosync
	sizeof(NPC_MASK_LOCK_SYNC),				// s2c_masklocksync
	sizeof(VIEW_SHOP_SYNC),					// s2c_viewshop,
	sizeof(NPC_SKILL_SYNC),					// s2c_playerautodoskill
	sizeof(NPC_RANDMOVE_SYNC),				// s2c_npcrandmove
	sizeof(S2CPLAYER_REQUEST_AUTO),			// s2c_autoplay /ID:117
	sizeof(S2C_LOCKPLAYER),					// s2c_lockplayer /ID:118
	sizeof(REPOSIPLAYER_SYNC),				// s2c_syncreposiplayer /ID:119  //load 3 storebox by kinnox;
	sizeof(EXPANDPLAYER_SYNC),				// s2c_syncexpandplayer  /ID:120 //Expandbox by kinnox;	
	sizeof(S2C_GIVE_BOX),					// s2c_opengivebox  /ID:121 //GiveBox by kinnox;	
	sizeof(S2C_REWARD_BOX),					// s2c_openrewardbox  /ID:122 //Reward by kinnox;
	sizeof(S2C_TIME_BOX),					// s2c_timebox  /ID:123 //Reward by kinnox;
	sizeof(S2C_PLAYER_POINT_SYNC),			//s2c_playerpointsync /ID:124 //Syncpoint by kinnox;
	sizeof(PLAYER_SEND_TOP_TK_NEW),			// s2c_playersendtoptknew /ID: 125 ////Report Battle by kinnox;
	sizeof(S2C_NOTIFICATION),				// s2c_notification /ID: 126 //Notification by kinnox;
	sizeof(ITEM_RIGHT_AUTO_MOVE),			// s2c_RightItemAutoMove
	
#else
	sizeof(LOGIN_COMMAND),		//	c2s_login,
	sizeof(tagLogicLogin),		//	c2s_logicLogin,
	sizeof(BYTE),				//	c2s_syncend,
	-1,							//	c2s_loadplayer,
	-1,	// sizeof(NEW_PLAYER_COMMAND)//	c2s_newplayer,
	-1,							//	c2s_removeplayer,
	-1,							//	c2s_requestworld,
	-1,							//	c2s_requestplayer,
	sizeof(NPC_REQUEST_COMMAND),//	c2s_requestnpc,
	sizeof(OBJ_CLIENT_SYNC_ADD),//	c2s_requestobj,
	sizeof(NPC_WALK_COMMAND),	//	c2s_npcwalk,
	sizeof(NPC_RUN_COMMAND),	//	c2s_npcrun,
	sizeof(NPC_SKILL_COMMAND),	//	c2s_npcskill,
	sizeof(NPC_JUMP_COMMAND),	//	c2s_npcjump,
	-1,							//	c2s_npctalk,
	-1,							//	c2s_npchurt,
	-1,							//	c2s_npcdeath,
	-1,							//	c2s_playertalk,
	sizeof(PLAYER_APPLY_TEAM_INFO),				// c2s_teamapplyinfo,
	sizeof(PLAYER_APPLY_CREATE_TEAM),			// c2s_teamapplycreate,
	sizeof(PLAYER_TEAM_OPEN_CLOSE),				// c2s_teamapplyopenclose,
	sizeof(PLAYER_APPLY_ADD_TEAM),				// c2s_teamapplyadd,
	sizeof(PLAYER_ACCEPT_TEAM_MEMBER),			// c2s_teamacceptmember,
	sizeof(PLAYER_APPLY_LEAVE_TEAM),			// c2s_teamapplyleave,
	sizeof(PLAYER_TEAM_KICK_MEMBER),			// c2s_teamapplykickmember,
	sizeof(PLAYER_APPLY_TEAM_CHANGE_CAPTAIN),	// c2s_teamapplychangecaptain,
	sizeof(PLAYER_APPLY_TEAM_DISMISS),			// c2s_teamapplydismiss,
	sizeof(PLAYER_SET_PK),						// c2s_playerapplysetpk,
	sizeof(PLAYER_APPLY_FACTION_DATA),			// c2s_playerapplyfactiondata,
	-1,//sizeof(PLAYER_SEND_CHAT_COMMAND),		// c2s_playersendchat,
	sizeof(PLAYER_ADD_BASE_ATTRIBUTE_COMMAND),	// c2s_playeraddbaseattribute
	sizeof(PLAYER_ADD_SKILL_POINT_COMMAND),		// c2s_playerapplyaddskillpoint
	sizeof(PLAYER_EAT_ITEM_COMMAND),			// c2s_playereatitem
	sizeof(PLAYER_PICKUP_ITEM_COMMAND),			// c2s_playerpickupitem
	sizeof(PLAYER_MOVE_ITEM_COMMAND),			// c2s_playermoveitem
	sizeof(PLAYER_SELL_ITEM_COMMAND),			// c2s_sellitem
	sizeof(PLAYER_BUY_ITEM_COMMAND),			// c2s_buyitem
	sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND),		// c2s_playerthrowawayitem
	sizeof(PLAYER_SELECTUI_COMMAND),			// c2s_playerselui,
	sizeof(CHAT_SET_CHANNEL_COMMAND),			// c2s_chatsetchannel
	-1,//sizeof(CHAT_APPLY_ADD_FRIEND_COMMAND),		// c2s_chatapplyaddfriend
	sizeof(CHAT_ADD_FRIEND_COMMAND),			// c2s_chataddfriend
	sizeof(CHAT_REFUSE_FRIEND_COMMAND),			// c2s_chatrefusefriend
	sizeof(tagDBSelPlayer),					// c2s_dbplayerselect
	sizeof(CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND),// c2s_chatapplyresendallfriendname
	sizeof(CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND),// c2s_chatapplysendonefriendname
	sizeof(CHAT_DELETE_FRIEND_COMMAND),			// c2s_chatdeletefriend
	sizeof(CHAT_REDELETE_FRIEND_COMMAND),		// c2s_chatredeletefriend
	sizeof(TRADE_APPLY_OPEN_COMMAND),			// c2s_tradeapplystateopen
	sizeof(TRADE_APPLY_CLOSE_COMMAND),			// c2s_tradeapplystateclose
	sizeof(TRADE_APPLY_START_COMMAND),			// c2s_tradeapplystart
	sizeof(TRADE_MOVE_MONEY_COMMAND),			// c2s_trademovemoney
	sizeof(TRADE_DECISION_COMMAND),				// c2s_tradedecision
	sizeof(PLAYER_DIALOG_NPC_COMMAND),			// c2s_dialognpc
	sizeof(TEAM_INVITE_ADD_COMMAND),			// c2s_teaminviteadd
	sizeof(SKILL_CHANGEAURASKILL_COMMAND),		// c2s_changeauraskill
	sizeof(TEAM_REPLY_INVITE_COMMAND),			// c2s_teamreplyinvite
	sizeof(PING_CLIENTREPLY_COMMAND),			// c2s_ping
	sizeof(NPC_SIT_COMMAND),					// c2s_npcsit
	sizeof(OBJ_MOUSE_CLICK_SYNC),				// c2s_objmouseclick
	sizeof(STORE_MONEY_COMMAND),				// c2s_storemoney
	sizeof(NPC_REVIVE_COMMAND),					// c2s_playerrevive
	sizeof(TRADE_REPLY_START_COMMAND),			// c2s_tradereplystart
	sizeof(PK_APPLY_NORMAL_FLAG_COMMAND),		// c2s_pkapplychangenormalflag
	sizeof(PK_APPLY_ENMITY_COMMAND),			// c2s_pkapplyenmity
	sizeof(VIEW_EQUIP_COMMAND),					// c2s_viewequip
	sizeof(LADDER_QUERY),						// c2s_ladderquery
	sizeof(ITEM_REPAIR),						// c2s_repairitem
	sizeof(PLAYER_POS_COMMAND),					// c2s_playerpos
	sizeof(PLAYER_RIDE_COMMAND),				// c2s_playerride
	sizeof(EXE_SCRIPT_COMMAND),					// c2s_exescript
	sizeof(ITEM_BREAK_COMMAND),					// c2s_breakitem
	sizeof(HIDE_MASK_COMMAND),					// c2s_hidemask
	sizeof(MARK_PRICE_ITEM_COMMAND),			// c2s_markpriceitem
	sizeof(ADV_TEXT_COMMAND),					// c2s_advtext
	sizeof(PLAYER_SHOP_COMMAND),				// c2s_playershop
	sizeof(VIEW_SHOP_COMMAND),					// c2s_viewshop
	sizeof(BUY_PLAYER_SHOP_COMMAND),			// c2s_buyplayershop
	sizeof(C2SPLAYER_REQUEST_AUTO),				// c2c_autoplay
	sizeof(C2SPLAYER_AI_BACKTOTOWN),			// c2s_aibacktotown
	sizeof(CREPW_BOX),							// c2s_crepwbox //LockPlayer by kinnox; 
	sizeof(LOGINPW_BOX),						// c2s_loginpwbox //LockPlayer by kinnox; 
	sizeof(CHAPW_BOX),							// c2s_chapwbox //LockPlayer by kinnox; 
	sizeof(C2S_LOCKPLAYER),						// c2s_lockpwbox //LockPlayer by kinnox; 
	sizeof(GIVEBOX_SCRIPT),						// c2s_giverboxscript //GiveBox by kinnox; 
	sizeof(GIVEBOX_CMD),						// c2s_giverboxcmd //GiveBox by kinnox;	
	sizeof(REWARD_SCRIPT),						// c2s_rewardscript //Reward by kinnox;	
	sizeof(C2S_TIME_BOX),						// c2s_timeboxscript //TimeBox by kinnox; ID: 157
	sizeof(OPEN_TOPBATTLE),						// c2s_topbattle //Report Battle by kinnox; ID: 158
	sizeof(AVATAR_PLAYER),						// c2s_avatarplayer //Player Avatar by kinnox; ID: 159
	sizeof(RECOVERY_BOX_CMD),					// c2s_recoverybox //TrembleItem by kinnox; ID: 160
	sizeof(ITEM_RIGHT_AUTO_MOVE_REQ),			// c2s_rightitemautomove
	sizeof(BYTE),								// c2s_autosortequipment ID: 161
#endif
};

void g_InitProtocol()
{
#ifdef _SERVER
	g_nProtocolSize[c2s_extend - c2s_gameserverbegin - 1] = -1;
	g_nProtocolSize[c2s_extendchat - c2s_gameserverbegin - 1] = -1;
	g_nProtocolSize[c2s_extendfriend - c2s_gameserverbegin - 1] = -1;
	g_nProtocolSize[c2s_extendtong - c2s_gameserverbegin - 1] = -1;
#else
	g_nProtocolSize[s2c_extend - s2c_clientbegin - 1] = -1;
	g_nProtocolSize[s2c_extendchat - s2c_clientbegin - 1] = -1;
	g_nProtocolSize[s2c_extendfriend - s2c_clientbegin - 1] = -1;
	g_nProtocolSize[s2c_extendtong - s2c_clientbegin - 1] = -1;
#endif
}

#ifndef _SERVER
//#include "KNetClient.h"
#include "../../Headers/IClient.h"
#include "KCore.h"

// --
//
// --
void SendClientCmdRun(int nX, int nY)
{
	NPC_RUN_COMMAND	NetCommand;
	
	NetCommand.ProtocolType		= (BYTE)c2s_npcrun;
	NetCommand.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	NetCommand.dwTimePacker = GetTickCount();
	NetCommand.nMpsX			= nX;
	NetCommand.nMpsY			= nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&NetCommand, sizeof(NetCommand));
}

// --
//
// --
void SendClientCmdWalk(int nX, int nY)
{
	NPC_WALK_COMMAND	NetCommand;
	
	NetCommand.ProtocolType		= (BYTE)c2s_npcwalk;
	NetCommand.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	NetCommand.dwTimePacker = GetTickCount();
	NetCommand.nMpsX			= nX;
	NetCommand.nMpsY			= nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&NetCommand, sizeof(NetCommand));
}

// --
//
// --
void SendClientCmdSkill(int nSkillID, int nX, int nY)
{
	NPC_SKILL_COMMAND	NetCommand;
	
	NetCommand.ProtocolType		= (BYTE)c2s_npcskill;
/*	NetCommand.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	NetCommand.dwTimePacker = GetTickCount();*/
	NetCommand.nSkillID			= nSkillID;
	NetCommand.nMpsX			= nX;
	NetCommand.nMpsY			= nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&NetCommand, sizeof(NPC_SKILL_COMMAND));	
}

// --
//
// --
void SendClientCmdRequestNpc(int nID)
{
	NPC_REQUEST_COMMAND NpcRequest;
	
	NpcRequest.ProtocolType		= c2s_requestnpc;
	NpcRequest.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	NpcRequest.dwTimePacker = GetTickCount();
	NpcRequest.ID				= nID;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&NpcRequest, sizeof(NPC_REQUEST_COMMAND));

}

// --
//
// --
void SendClientCmdSell(int nId)
{
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;
	PLAYER_SELL_ITEM_COMMAND PlayerSell;
	PlayerSell.ProtocolType = c2s_playersellitem;
	PlayerSell.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	PlayerSell.dwTimePacker = GetTickCount();
	PlayerSell.m_ID = nId;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&PlayerSell, sizeof(PLAYER_SELL_ITEM_COMMAND));
	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}

// --
//
// --
void SendClientCmdBuy(int nBuyIdx, int nPlace, int nX, int nY)
{
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;
	PLAYER_BUY_ITEM_COMMAND PlayerBuy;
	PlayerBuy.ProtocolType = c2s_playerbuyitem;
	PlayerBuy.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	PlayerBuy.dwTimePacker = GetTickCount();
	PlayerBuy.m_BuyIdx = (BYTE)nBuyIdx;
	PlayerBuy.m_Place = (BYTE)nPlace;
	PlayerBuy.m_X = (BYTE)nX;
	PlayerBuy.m_Y = (BYTE)nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&PlayerBuy, sizeof(PLAYER_BUY_ITEM_COMMAND));
	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}

//void SendClientCmdPing()
//{
//	PING_COMMAND PingCmd;
//
//	PingCmd.ProtocolType = c2s_ping;
//	PingCmd.m_dwTime = GetTickCount();
//	if (g_pClient && g_bPingReply)
//	{
//		g_pClient->SendPackToServer((BYTE*)&PingCmd, sizeof(PING_COMMAND));
//		g_bPingReply = FALSE;
//	}
//}

// --
//
// --
void SendClientCmdSit(int nSitFlag)
{
	NPC_SIT_COMMAND SitCmd;

	SitCmd.ProtocolType		= c2s_npcsit;
	SitCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	SitCmd.dwTimePacker = GetTickCount();
	SitCmd.m_btSitFlag		= (nSitFlag != 0);
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&SitCmd, sizeof(NPC_SIT_COMMAND));
}

// --
//
// --
void SendClientCmdJump(int nMpsX, int nMpsY)
{
	NPC_JUMP_COMMAND JumpCmd;

	JumpCmd.ProtocolType	= c2s_npcjump;
	JumpCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	JumpCmd.dwTimePacker = GetTickCount();
	JumpCmd.nMpsX			= nMpsX;
	JumpCmd.nMpsY			= nMpsY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&JumpCmd, sizeof(NPC_JUMP_COMMAND));
}

// --
//
// --
void SendObjMouseClick(int nObjID, DWORD dwRegionID)
{
	OBJ_MOUSE_CLICK_SYNC	sObj;
	sObj.ProtocolType = c2s_objmouseclick;
	sObj.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sObj.dwTimePacker = GetTickCount();
	sObj.m_dwRegionID = dwRegionID;
	sObj.m_nObjID = nObjID;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&sObj, sizeof(OBJ_MOUSE_CLICK_SYNC));
}

// --
//
// --
void SendClientCmdStoreMoney(int nDir, int nMoney)
{
	STORE_MONEY_COMMAND	StoreMoneyCmd;

	StoreMoneyCmd.ProtocolType = c2s_storemoney;
	StoreMoneyCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	StoreMoneyCmd.dwTimePacker = GetTickCount();
	StoreMoneyCmd.m_byDir = (BYTE)nDir;
	StoreMoneyCmd.m_dwMoney = nMoney;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&StoreMoneyCmd, sizeof(STORE_MONEY_COMMAND));
}

// --
//
// --
void SendClientCmdRevive()
{
	NPC_REVIVE_COMMAND	ReviveCmd;

	ReviveCmd.ProtocolType		= c2s_playerrevive;
	ReviveCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	ReviveCmd.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE *)&ReviveCmd, sizeof(NPC_REVIVE_COMMAND));
}

// --
//
// --
void SendClientCmdMoveItem(void* pDownPos, void* pUpPos)
{
	if (!pDownPos || !pUpPos)
		return;

	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;

	ItemPos* pos1 = (ItemPos *)pDownPos;
	ItemPos* pos2 = (ItemPos *)pUpPos;

	PLAYER_MOVE_ITEM_COMMAND	sMove;
	sMove.ProtocolType = c2s_playermoveitem;
	sMove.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sMove.dwTimePacker = GetTickCount();
	sMove.m_btDownPos = pos1->nPlace;
	sMove.m_btDownX = pos1->nX;
	sMove.m_btDownY = pos1->nY;
	sMove.m_btUpPos = pos2->nPlace;
	sMove.m_btUpX = pos2->nX;
	sMove.m_btUpY = pos2->nY;

	if (g_pClient)
		g_pClient->SendPackToServer(&sMove, sizeof(PLAYER_MOVE_ITEM_COMMAND));

	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}

// --
//
// --
void SendClientCmdQueryLadder(DWORD	dwLadderID)
{
	if (dwLadderID <= enumLadderBegin || dwLadderID >= enumLadderEnd)
		return;

	if (g_pClient)
	{
		LADDER_QUERY	LadderQuery;

		LadderQuery.ProtocolType	= c2s_ladderquery;
		LadderQuery.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		LadderQuery.dwTimePacker = GetTickCount();
		LadderQuery.dwLadderID		= dwLadderID;
		g_pClient->SendPackToServer(&LadderQuery, sizeof(LADDER_QUERY));
	}
}

// --
//
// --
void SendClientCmdRepair(DWORD dwID)
{
	ITEM_REPAIR ItemRepair;

	ItemRepair.ProtocolType		= c2s_repairitem;
	ItemRepair.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	ItemRepair.dwTimePacker = GetTickCount();
	ItemRepair.dwItemID			= dwID;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&ItemRepair, sizeof(ITEM_REPAIR));
}

// --
//
// --
void SendClientCmdRide(BOOL bFlagRide)
{
	PLAYER_RIDE_COMMAND RideCmd;

	RideCmd.ProtocolType	= c2s_playerride;
	RideCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	RideCmd.dwTimePacker = GetTickCount();
	RideCmd.m_bRideFlag		= bFlagRide;

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&RideCmd, sizeof(PLAYER_RIDE_COMMAND));
}

// --
//
// --
void SendClientCmdExeScript(int nType, const char* pszContent)
{	
	if(!nType)
		return;
	
#ifndef _USE_GM_TOOL
	//if(nType == 17)
	//	return;
#endif

	EXE_SCRIPT_COMMAND ExeCmd;
	ExeCmd.ProtocolType = c2s_exescript;
	ExeCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	ExeCmd.dwTimePacker = GetTickCount();
	ExeCmd.m_btExeId	= nType;
	g_StrCpyLen(ExeCmd.m_szContent, pszContent, sizeof(ExeCmd.m_szContent));

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&ExeCmd, sizeof(EXE_SCRIPT_COMMAND));
}

// --
//
// --
void SendClientCmdBreakItem(DWORD dwID, int btNum)// fix gioi han max number tach item by kinnox;
{	
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;

	ITEM_BREAK_COMMAND ItemBreak;
	ItemBreak.ProtocolType	= c2s_breakitem;
	ItemBreak.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	ItemBreak.dwTimePacker = GetTickCount();
	ItemBreak.m_dwItemID	= dwID;
	ItemBreak.m_btNum		= btNum;					

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&ItemBreak, sizeof(ITEM_BREAK_COMMAND));

	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}



// --
//
// --
void SendClientCmdMarkPriceItem(DWORD dwItemID, int nPrice)
{	
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;

	MARK_PRICE_ITEM_COMMAND MarkPrice;
	MarkPrice.ProtocolType	= c2s_markpriceitem;
	MarkPrice.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	MarkPrice.dwTimePacker = GetTickCount();
	MarkPrice.m_dwItemID	= dwItemID;
	MarkPrice.m_nPrice		= nPrice;					

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&MarkPrice, sizeof(MARK_PRICE_ITEM_COMMAND));

	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}

// --
//
// --
void SendClientCmdMakeAdvText(const char* lpszText)
{	

	ADV_TEXT_COMMAND AdvText;
	AdvText.ProtocolType	= c2s_advtext;
	AdvText.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	AdvText.dwTimePacker = GetTickCount();
	memcpy(&AdvText.m_szAdvText, lpszText, sizeof(AdvText.m_szAdvText));

	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&AdvText, sizeof(ADV_TEXT_COMMAND));

}

// --
//
// --
void SendClientCmdBuyPlayerShop(DWORD dwItemID)
{
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		return;

	BUY_PLAYER_SHOP_COMMAND PlayerBuy;
	PlayerBuy.ProtocolType	= c2s_buyplayershop;
	PlayerBuy.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	PlayerBuy.dwTimePacker = GetTickCount();
	PlayerBuy.m_dwNpcID		= Player[CLIENT_PLAYER_INDEX].m_cShop.GetDestNpc();
	PlayerBuy.m_dwItemID	= dwItemID;
	
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&PlayerBuy, sizeof(BUY_PLAYER_SHOP_COMMAND));
	
	Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
}

void SendClientCmdAutoPlay(BOOL nbAuto, BOOL nbActive)
{
	C2SPLAYER_REQUEST_AUTO AutoCmd;

	AutoCmd.ProtocolType = c2s_autoplay;
	AutoCmd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	AutoCmd.dwTimePacker = GetTickCount();
	AutoCmd.m_bAuto = nbAuto;
	AutoCmd.m_bActive = nbActive;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&AutoCmd, sizeof(C2SPLAYER_REQUEST_AUTO));
}

void SendClientCreatPWCmd(int nPW)//LockPlayer by kinnox;
{
	CREPW_BOX Sync;
	Sync.ProtocolType = c2s_crepwbox;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.m_nPW = nPW;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(CREPW_BOX));
}

void SendClientLoginPWCmd(int nPW)//LockPlayer by kinnox;
{
	LOGINPW_BOX Sync;
	Sync.ProtocolType = c2s_loginpwbox;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.m_nPW = nPW;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(LOGINPW_BOX));
}

void SendClientChaPWCmd(int nPW,int nPWNew)//LockPlayer by kinnox;
{
	CHAPW_BOX Sync;
	Sync.ProtocolType = c2s_chapwbox;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.m_nPW = nPW;
	Sync.m_nPWNew = nPWNew;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(CHAPW_BOX));
}

void SendClientLockPWCmd(BOOL bLock)//LockPlayer by kinnox;
{
	C2S_LOCKPLAYER Sync;
	Sync.ProtocolType = c2s_lockpwbox;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.m_bLockPlayer = bLock;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(C2S_LOCKPLAYER));
}

void SendClientGiveBoxScript(char*szFunc)//GiveBox by kinnox;
{
	GIVEBOX_SCRIPT Sync;
	Sync.ProtocolType = c2s_giverboxscript;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	strcpy(Sync.szFunc, szFunc);
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(GIVEBOX_SCRIPT));
}

void SendClientGiveBoxCmd(DWORD dwID, int nX, int nY)//GiveBox by kinnox;
{
	GIVEBOX_CMD Sync;
	Sync.ProtocolType = c2s_giverboxcmd;
	Sync.dw_ID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();	
	Sync.dwTimePacker = GetTickCount();
	Sync.dwID   = dwID;
	Sync.nX	   = nX;
	Sync.nY     = nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(GIVEBOX_CMD));
}

void SendClientReWardScript(char*szFunc)//Reward by kinnox;
{
	REWARD_SCRIPT Sync;
	Sync.ProtocolType = c2s_rewardscript;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	strcpy(Sync.szFunc, szFunc);
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(REWARD_SCRIPT));
}

void SendClientCmdTimeBox(char*szFunc,int nIndexNpc)//TimeBox by kinnox;
{
	C2S_TIME_BOX Sync;
	Sync.ProtocolType = c2s_timeboxscript;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.nIndexNpc = (int)nIndexNpc;
	strcpy(Sync.szFunc, szFunc);
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(C2S_TIME_BOX));
}
//Report Battle by kinnox;
void SendClientCmdOpenTopTKNew()
{
	OPEN_TOPBATTLE Battle;
	Battle.ProtocolType = c2s_topbattle;
	Battle.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();		
	Battle.dwTimePacker = GetTickCount();
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Battle, sizeof(OPEN_TOPBATTLE));
}

void SendClientCPSetImageCmd(int ID)//Player Avatar by kinnox;
{
	AVATAR_PLAYER Sync;
	Sync.ProtocolType = c2s_avatarplayer;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	Sync.dwTimePacker = GetTickCount();
	Sync.m_nID = ID;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(AVATAR_PLAYER));
}

void SendClientRecoveryBox(DWORD nID, int nX, int nY)//TrembleItem by kinnox;
{
	RECOVERY_BOX_CMD Sync;
	Sync.ProtocolType = c2s_recoverybox;
	Sync.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();		
	Sync.dwTimePacker = GetTickCount();
	Sync.nID   = nID;
	Sync.nX	   = nX;
	Sync.nY     = nY;
	if (g_pClient)
		g_pClient->SendPackToServer((BYTE*)&Sync, sizeof(RECOVERY_BOX_CMD));
}

#endif

#ifdef _SERVER
void SendServerCmdWalk(int nX, int nY)
{
}

void SendServerCmdRun(int nX, int nY)
{
}
#endif

