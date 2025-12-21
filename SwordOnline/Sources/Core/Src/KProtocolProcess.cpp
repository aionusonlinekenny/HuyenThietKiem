#include "KEngine.h"
#include "KCore.h"
#ifndef _SERVER
#include "../../Headers/IClient.h"
#include "CoreShell.h"
#include "KViewItem.h"
#endif
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KPlayerFaction.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "LuaFuns.h"
#include "KProtocolProcess.h"
#include "KSkills.h"
#include "KItemSet.h"
#include "KBuySell.h"
#include "KSubWorldSet.h"
//#include "MyAssert.h"
#include "Scene/ObstacleDef.h"
#include "KMath.h"
#ifndef _SERVER
#include "Scene\KScenePlaceC.h"
#include "../KJXPathFinder.h" // find way by kinnox;
extern KJXPathFinder g_JXPathFinder; // find way by kinnox;
#endif
#include "KTongProtocol.h"
#include "KLadder.h"
#include <stdio.h>
//#define WAIGUA_ZROC

KProtocolProcess g_ProtocolProcess;
KProtocolProcess::KProtocolProcess()
{
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
#ifndef _SERVER
	ProcessFunc[s2c_login] = NULL;
	ProcessFunc[s2c_logout] = NULL;
	ProcessFunc[s2c_syncend] = &KProtocolProcess::SyncEnd;
	ProcessFunc[s2c_synccurplayer] = &KProtocolProcess::SyncCurPlayer;
	ProcessFunc[s2c_synccurplayerskill] = &KProtocolProcess::s2cSyncAllSkill;
	ProcessFunc[s2c_synccurplayernormal] = &KProtocolProcess::SyncCurNormalData;
	ProcessFunc[s2c_newplayer] = NULL;
	ProcessFunc[s2c_removeplayer] = NULL;
	ProcessFunc[s2c_syncworld] = &KProtocolProcess::SyncWorld;
	ProcessFunc[s2c_syncplayer] = &KProtocolProcess::SyncPlayer;
	ProcessFunc[s2c_syncplayermin] = &KProtocolProcess::SyncPlayerMin;
	ProcessFunc[s2c_syncnpc] = &KProtocolProcess::SyncNpc;
	ProcessFunc[s2c_syncnpcmin] = &KProtocolProcess::SyncNpcMin;
	ProcessFunc[s2c_syncnpcminplayer] = &KProtocolProcess::SyncNpcMinPlayer;
	ProcessFunc[s2c_objadd] = &KProtocolProcess::SyncObjectAdd;
	ProcessFunc[s2c_syncobjstate] = &KProtocolProcess::SyncObjectState;
	ProcessFunc[s2c_syncobjdir] = &KProtocolProcess::SyncObjectDir;
	ProcessFunc[s2c_objremove] = &KProtocolProcess::SyncObjectRemove;
	ProcessFunc[s2c_objTrapAct] = &KProtocolProcess::SyncObjectTrap;
	ProcessFunc[s2c_npcremove] = &KProtocolProcess::NetCommandRemoveNpc;
	ProcessFunc[s2c_npcwalk] = &KProtocolProcess::NetCommandWalk;
	ProcessFunc[s2c_npcrun] = &KProtocolProcess::NetCommandRun;
	ProcessFunc[s2c_npcattack] = NULL;
	ProcessFunc[s2c_npcmagic] = NULL;
	ProcessFunc[s2c_npcjump] = &KProtocolProcess::NetCommandJump;
	ProcessFunc[s2c_npcglide] = &KProtocolProcess::NetCommandGlide;
	ProcessFunc[s2c_npctalk] = NULL;
	ProcessFunc[s2c_npchurt] = &KProtocolProcess::NetCommandHurt;
	ProcessFunc[s2c_npcdeath] = &KProtocolProcess::NetCommandDeath;
	ProcessFunc[s2c_npcchgcurcamp] = &KProtocolProcess::NetCommandChgCurCamp;
	ProcessFunc[s2c_npcchgcamp] = &KProtocolProcess::NetCommandChgCamp;
	ProcessFunc[s2c_skillcast] = &KProtocolProcess::NetCommandSkill;
	ProcessFunc[s2c_playertalk] = NULL;
	ProcessFunc[s2c_playerexp] = &KProtocolProcess::s2cPlayerExp;
	ProcessFunc[s2c_teaminfo] = &KProtocolProcess::s2cShowTeamInfo;
	ProcessFunc[s2c_teamselfinfo] = &KProtocolProcess::s2cUpdataSelfTeamInfo;
	ProcessFunc[s2c_teamapplyinfofalse] = &KProtocolProcess::s2cApplyTeamInfoFalse;
	ProcessFunc[s2c_teamcreatesuccess] = &KProtocolProcess::s2cCreateTeam;
	ProcessFunc[s2c_teamcreatefalse] = &KProtocolProcess::s2cApplyCreateTeamFalse;
	ProcessFunc[s2c_teamopenclose] = &KProtocolProcess::s2cSetTeamState;
	ProcessFunc[s2c_teamgetapply] = &KProtocolProcess::s2cApplyAddTeam;
	ProcessFunc[s2c_teamaddmember] = &KProtocolProcess::s2cTeamAddMember;
	ProcessFunc[s2c_teamleave] = &KProtocolProcess::s2cLeaveTeam;
	ProcessFunc[s2c_teamchangecaptain] = &KProtocolProcess::s2cTeamChangeCaptain;
	ProcessFunc[s2c_playerfactiondata] = &KProtocolProcess::s2cSetFactionData;
	ProcessFunc[s2c_playerleavefaction] = &KProtocolProcess::s2cLeaveFaction;
	ProcessFunc[s2c_playerfactionskilllevel] = &KProtocolProcess::s2cFactionSkillOpen;
	ProcessFunc[s2c_playersendchat] = &KProtocolProcess::s2cGetChat;
	ProcessFunc[s2c_playersyncleadexp] = &KProtocolProcess::s2cGetLeadExp;
	ProcessFunc[s2c_playerlevelup] = &KProtocolProcess::s2cLevelUp;
	ProcessFunc[s2c_teammatelevel] = &KProtocolProcess::s2cGetTeammateLevel;
	ProcessFunc[s2c_playersyncattribute] = &KProtocolProcess::s2cGetCurAttribute;
	ProcessFunc[s2c_playerskilllevel] = &KProtocolProcess::s2cGetSkillLevel;
	ProcessFunc[s2c_playerskillremove] = &KProtocolProcess::s2cGetSkillRemove;
	ProcessFunc[s2c_syncitem] = &KProtocolProcess::s2cSyncItem;
	ProcessFunc[s2c_removeitem] = &KProtocolProcess::s2cRemoveItem;
	ProcessFunc[s2c_syncmoney] = &KProtocolProcess::s2cSyncMoney;
	ProcessFunc[s2c_playermoveitem] = &KProtocolProcess::s2cMoveItem;
	ProcessFunc[s2c_scriptaction] = &KProtocolProcess::SyncScriptAction;
	ProcessFunc[s2c_chatapplyaddfriend] = &KProtocolProcess::s2cChatGetApplyAddFriend;
	ProcessFunc[s2c_chataddfriend] = &KProtocolProcess::s2cChatAddFriend;
	ProcessFunc[s2c_chatrefusefriend] = &KProtocolProcess::s2cChatBeRefusedAddFriend;
	ProcessFunc[s2c_chataddfriendfail] = &KProtocolProcess::s2cChatAddFriendFail;
	ProcessFunc[s2c_chatloginfriendnoname] = &KProtocolProcess::s2cChatLoginFriendNoName;
	ProcessFunc[s2c_chatloginfriendname] = &KProtocolProcess::s2cChatLoginFriendName;
	ProcessFunc[s2c_chatonefrienddata] = &KProtocolProcess::s2cChatAddOneFriend;
	ProcessFunc[s2c_chatfriendonline] = &KProtocolProcess::s2cChatFriendOnline;
	ProcessFunc[s2c_chatdeletefriend] = &KProtocolProcess::s2cChatDeleteFriend;
	ProcessFunc[s2c_chatfriendoffline] = &KProtocolProcess::s2cChatFriendOffLine;
	ProcessFunc[s2c_syncrolelist] = &KProtocolProcess::s2cSyncRoleList;
	ProcessFunc[s2c_tradechangestate] = &KProtocolProcess::s2cTradeChangeState;
	ProcessFunc[s2c_npcsetmenustate] = &KProtocolProcess::s2cNpcSetMenuState;
	ProcessFunc[s2c_trademoneysync] = &KProtocolProcess::s2cTradeMoneySync;
	ProcessFunc[s2c_tradedecision] = &KProtocolProcess::s2cTradeDecision;
	ProcessFunc[s2c_chatscreensingleerror] = &KProtocolProcess::s2cChatScreenSingleError;
	ProcessFunc[s2c_syncnpcstate] = &KProtocolProcess::s2cUpdateNpcState;
	ProcessFunc[s2c_teaminviteadd] = &KProtocolProcess::s2cTeamInviteAdd;
	ProcessFunc[s2c_tradepressoksync] = &KProtocolProcess::s2cTradePressOkSync;
	ProcessFunc[s2c_ping] = &KProtocolProcess::s2cPing;
	ProcessFunc[s2c_npcsit] = &KProtocolProcess::NetCommandSit;
	ProcessFunc[s2c_opensalebox] = &KProtocolProcess::OpenSaleBox;
	ProcessFunc[s2c_castskilldirectly] = &KProtocolProcess::s2cDirectlyCastSkill;
	ProcessFunc[s2c_msgshow] = &KProtocolProcess::s2cShowMsg;
	ProcessFunc[s2c_syncstateeffect] = &KProtocolProcess::SyncStateEffect;
	ProcessFunc[s2c_openstorebox] = &KProtocolProcess::OpenStoreBox;
	ProcessFunc[s2c_playerrevive] = &KProtocolProcess::PlayerRevive;
	ProcessFunc[s2c_requestnpcfail] = &KProtocolProcess::RequestNpcFail;
	ProcessFunc[s2c_tradeapplystart] = &KProtocolProcess::s2cTradeApplyStart;
	ProcessFunc[s2c_rolenewdelresponse] = NULL;
	ProcessFunc[s2c_ItemAutoMove] = &KProtocolProcess::s2cItemAutoMove;
	ProcessFunc[s2c_itemexchangefinish] = &KProtocolProcess::FinishedItemExchange;
	ProcessFunc[s2c_changeweather] = &KProtocolProcess::s2cChangeWeather;
	ProcessFunc[s2c_pksyncnormalflag] = &KProtocolProcess::s2cPKSyncNormalFlag;
	ProcessFunc[s2c_pksyncenmitystate] = &KProtocolProcess::s2cPKSyncEnmityState;
	ProcessFunc[s2c_pksyncexercisestate] = &KProtocolProcess::s2cPKSyncExerciseState;
	ProcessFunc[s2c_pksyncpkvalue] = &KProtocolProcess::s2cPKValueSync;
	ProcessFunc[s2c_npcsleepmode] = &KProtocolProcess::NpcSleepSync;
	ProcessFunc[s2c_viewequip] = &KProtocolProcess::s2cViewEquip;
	ProcessFunc[s2c_ladderresult] = &KProtocolProcess::LadderResult;
	ProcessFunc[s2c_ladderlist] = &KProtocolProcess::LadderList;
	ProcessFunc[s2c_tongcreate] = &KProtocolProcess::s2cTongCreate;
	ProcessFunc[s2c_replyclientping] = &KProtocolProcess::ServerReplyClientPing;
	ProcessFunc[s2c_npcgoldchange] = &KProtocolProcess::s2cNpcGoldChange;
	
	ProcessFunc[s2c_taskvaluesync] = &KProtocolProcess::TaskValueSync;
	ProcessFunc[s2c_findpathsync] = &KProtocolProcess::s2cFindPathSync;
	ProcessFunc[s2c_stopmove] = &KProtocolProcess::PlayerStopMove;
	ProcessFunc[s2c_playerpos] = &KProtocolProcess::s2cPlayerPos;
	ProcessFunc[s2c_playerridesync] = &KProtocolProcess::SyncPlayerRide;
	ProcessFunc[s2c_bloodsync] = &KProtocolProcess::NetCommandBloodNo;
	ProcessFunc[s2c_refeshplayersync] = &KProtocolProcess::s2cPlayerRefesh;
	ProcessFunc[s2c_itemchangeinfosync] = &KProtocolProcess::ItemChangeInfo;
	ProcessFunc[s2c_masklocksync] = &KProtocolProcess::s2cMaskLock;
	ProcessFunc[s2c_viewshop] = &KProtocolProcess::s2cViewShop;
	ProcessFunc[s2c_playerautodoskill] = &KProtocolProcess::s2cPlayerAutoDoSkill;
	ProcessFunc[s2c_npcrandmove] = &KProtocolProcess::NetCommandRandMove;
	ProcessFunc[s2c_autoplay] = &KProtocolProcess::s2cAutoPlaySync;//Auto by kinnox;	
	ProcessFunc[s2c_lockplayer] = &KProtocolProcess::s2cLockPlayerSync;//LockPlayer by kinnox;	
	ProcessFunc[s2c_syncreposiplayer] = &KProtocolProcess::s2cReposiPlayer;//load 3 storebox by kinnox;
	ProcessFunc[s2c_syncexpandplayer] = &KProtocolProcess::s2cExpandboxPlayer;//Expandbox by kinnox;
	ProcessFunc[s2c_opengivebox] = &KProtocolProcess::s2cGiveBox;//GiveBox by kinnox;	
	ProcessFunc[s2c_openrewardbox] = &KProtocolProcess::s2cReWardBox;//Reward by kinnox;	
	ProcessFunc[s2c_timebox] = &KProtocolProcess::s2cTimeBox;//TimeBox by kinnox;
	ProcessFunc[s2c_playerpointsync] = &KProtocolProcess::s2cPlayerPointSync;//Sync point by kinnox;
	ProcessFunc[s2c_playersendtoptknew] = &KProtocolProcess::PlayerSendTopTKNew;//Report Battle by kinnox;
	ProcessFunc[s2c_notification] = &KProtocolProcess::s2cNotification;//Notification by kinnox;
	//ProcessFunc[s2c_show_lien_tram] = &KProtocolProcess::ShowLienTram; //KennyLienTram
	ProcessFunc[s2c_extend] = &KProtocolProcess::s2cExtend;
	ProcessFunc[s2c_extendchat] = &KProtocolProcess::s2cExtendChat;
	ProcessFunc[s2c_extendfriend] = &KProtocolProcess::s2cExtendFriend;
	ProcessFunc[s2c_extendtong] = &KProtocolProcess::s2cExtendTong;
	ProcessFunc[s2c_rightitemautomove] = &KProtocolProcess::s2cRightItemAutoMove;

#else
	ProcessFunc[c2s_login] = NULL;
	ProcessFunc[c2s_logiclogin] = NULL;
	ProcessFunc[c2s_syncend] = NULL;
	ProcessFunc[c2s_loadplayer] = NULL;
	ProcessFunc[c2s_newplayer] = NULL;
	ProcessFunc[c2s_removeplayer] = &KProtocolProcess::RemoveRole;
	ProcessFunc[c2s_requestworld] = NULL;
	ProcessFunc[c2s_requestplayer] = NULL;
	ProcessFunc[c2s_requestnpc] = &KProtocolProcess::NpcRequestCommand;
	ProcessFunc[c2s_requestobj] = &KProtocolProcess::ObjRequestCommand;
	ProcessFunc[c2s_npcwalk] = &KProtocolProcess::NpcWalkCommand;
	ProcessFunc[c2s_npcrun] = &KProtocolProcess::NpcRunCommand;
	ProcessFunc[c2s_npcskill] = &KProtocolProcess::NpcSkillCommand;
	ProcessFunc[c2s_npcjump] = &KProtocolProcess::NpcJumpCommand;
	ProcessFunc[c2s_npctalk] = &KProtocolProcess::NpcTalkCommand;
	ProcessFunc[c2s_npchurt] = NULL;
	ProcessFunc[c2s_npcdeath] = NULL;
	ProcessFunc[c2s_playertalk] = &KProtocolProcess::PlayerTalkCommand;
	ProcessFunc[c2s_teamapplyinfo] = &KProtocolProcess::PlayerApplyTeamInfo;
	ProcessFunc[c2s_teamapplycreate] = &KProtocolProcess::PlayerApplyCreateTeam;
	ProcessFunc[c2s_teamapplyopenclose] = &KProtocolProcess::PlayerApplyTeamOpenClose;
	ProcessFunc[c2s_teamapplyadd] = &KProtocolProcess::PlayerApplyAddTeam;
	ProcessFunc[c2s_teamacceptmember] = &KProtocolProcess::PlayerAcceptTeamMember;
	ProcessFunc[c2s_teamapplyleave] = &KProtocolProcess::PlayerApplyLeaveTeam;
	ProcessFunc[c2s_teamapplykickmember] = &KProtocolProcess::PlayerApplyTeamKickMember;
	ProcessFunc[c2s_teamapplychangecaptain] = &KProtocolProcess::PlayerApplyTeamChangeCaptain;
	ProcessFunc[c2s_teamapplydismiss] = &KProtocolProcess::PlayerApplyTeamDismiss;
	ProcessFunc[c2s_playerapplysetpk] = &KProtocolProcess::PlayerApplySetPK;
	ProcessFunc[c2s_playerapplyfactiondata] = &KProtocolProcess::PlayerApplyFactionData;
	ProcessFunc[c2s_playersendchat] = &KProtocolProcess::PlayerSendChat;
	ProcessFunc[c2s_playeraddbaseattribute] = &KProtocolProcess::PlayerAddBaseAttribute;
	ProcessFunc[c2s_playerapplyaddskillpoint] = &KProtocolProcess::PlayerApplyAddSkillPoint;
	ProcessFunc[c2s_playereatitem] = &KProtocolProcess::PlayerEatItem;
	ProcessFunc[c2s_playerpickupitem] = &KProtocolProcess::PlayerPickUpItem;
	ProcessFunc[c2s_playermoveitem] = &KProtocolProcess::PlayerMoveItem;
	ProcessFunc[c2s_playersellitem] = &KProtocolProcess::PlayerSellItem;
	ProcessFunc[c2s_playerbuyitem] = &KProtocolProcess::PlayerBuyItem;
	ProcessFunc[c2s_playerthrowawayitem] = &KProtocolProcess::PlayerDropItem;
	ProcessFunc[c2s_playerselui] = &KProtocolProcess::PlayerSelUI;
	ProcessFunc[c2s_chatsetchannel] = &KProtocolProcess::ChatSetChannel;
	ProcessFunc[c2s_chatapplyaddfriend] = &KProtocolProcess::ChatApplyAddFriend;
	ProcessFunc[c2s_chataddfriend] = &KProtocolProcess::ChatAddFriend;
	ProcessFunc[c2s_chatrefusefriend] = &KProtocolProcess::ChatRefuseFriend;
	ProcessFunc[c2s_dbplayerselect] = NULL;
	ProcessFunc[c2s_chatapplyresendallfriendname] = &KProtocolProcess::ChatApplyReSendAllFriendName;
	ProcessFunc[c2s_chatapplysendonefriendname] = &KProtocolProcess::ChatApplySendOneFriendName;
	ProcessFunc[c2s_chatdeletefriend] = &KProtocolProcess::ChatDeleteFriend;
	ProcessFunc[c2s_chatredeletefriend] = &KProtocolProcess::ChatReDeleteFriend;
	ProcessFunc[c2s_tradeapplystateopen] = &KProtocolProcess::TradeApplyOpen;
	ProcessFunc[c2s_tradeapplystateclose] = &KProtocolProcess::TradeApplyClose;
	ProcessFunc[c2s_tradeapplystart] = &KProtocolProcess::TradeApplyStart;
	ProcessFunc[c2s_trademovemoney] = &KProtocolProcess::TradeMoveMoney;
	ProcessFunc[c2s_tradedecision] = &KProtocolProcess::TradeDecision;
	ProcessFunc[c2s_dialognpc]	= &KProtocolProcess::DialogNpc;
	ProcessFunc[c2s_teaminviteadd]	= &KProtocolProcess::TeamInviteAdd;
	ProcessFunc[c2s_changeauraskill] = &KProtocolProcess::ChangeAuraSkill;
	ProcessFunc[c2s_teamreplyinvite] = &KProtocolProcess::TeamReplyInvite;
	ProcessFunc[c2s_ping] = NULL;//ReplyPing;
	ProcessFunc[c2s_npcsit] = &KProtocolProcess::NpcSitCommand;
	ProcessFunc[c2s_objmouseclick] = &KProtocolProcess::ObjMouseClick;
	ProcessFunc[c2s_storemoney] = &KProtocolProcess::StoreMoneyCommand;
	ProcessFunc[c2s_playerrevive] = &KProtocolProcess::NpcReviveCommand;
	ProcessFunc[c2s_tradereplystart] = &KProtocolProcess::c2sTradeReplyStart;
	ProcessFunc[c2s_pkapplychangenormalflag] = &KProtocolProcess::c2sPKApplyChangeNormalFlag;
	ProcessFunc[c2s_pkapplyenmity] = &KProtocolProcess::c2sPKApplyEnmity;
	ProcessFunc[c2s_viewequip] = &KProtocolProcess::c2sViewEquip;
	ProcessFunc[c2s_ladderquery] = &KProtocolProcess::LadderQuery;
	ProcessFunc[c2s_repairitem] = &KProtocolProcess::ItemRepair;
	
	ProcessFunc[c2s_playerpos] = &KProtocolProcess::c2sPosCommand;
	ProcessFunc[c2s_playerride] = &KProtocolProcess::PlayerRideCommand;
	ProcessFunc[c2s_exescript] = &KProtocolProcess::ExeScriptCommand;
	ProcessFunc[c2s_breakitem] = &KProtocolProcess::ItemBreakCommand;
	ProcessFunc[c2s_hidemask] = &KProtocolProcess::HideMaskCommand;
	ProcessFunc[c2s_markpriceitem] = &KProtocolProcess::MarkPriceItemCommand;
	ProcessFunc[c2s_advtext] = &KProtocolProcess::MakeAdvTextCommand;
	ProcessFunc[c2s_playershop] = &KProtocolProcess::PlayerApplyShopState;
	ProcessFunc[c2s_viewshop] = &KProtocolProcess::ViewShopCommand;
	ProcessFunc[c2s_buyplayershop] = &KProtocolProcess::BuyPlayerShopCommand;
	ProcessFunc[c2s_autoplay] = &KProtocolProcess::c2sSetAutoSync;//Auto by kinnox;
	ProcessFunc[c2s_aibacktotown] = &KProtocolProcess::c2sBackToTown;//Auto by kinnox;
	ProcessFunc[c2s_crepwbox] = &KProtocolProcess::c2sCrePwBox;//LockPlayer by kinnox;
	ProcessFunc[c2s_loginpwbox] = &KProtocolProcess::c2sLoginPwBox;//LockPlayer by kinnox;
	ProcessFunc[c2s_chapwbox] = &KProtocolProcess::c2sChaPwBox;//LockPlayer by kinnox;
	ProcessFunc[c2s_lockpwbox] = &KProtocolProcess::c2sLockPwBox;//LockPlayer by kinnox;
	ProcessFunc[c2s_giverboxscript] = &KProtocolProcess::c2sGiveBoxScript;//GiveBox by kinnox;
	ProcessFunc[c2s_giverboxcmd] = &KProtocolProcess::c2sGiveBoxCmd;//GiveBox by kinnox;
	ProcessFunc[c2s_rewardscript] = &KProtocolProcess::c2sReWardScript;//Reward by kinnox;
	ProcessFunc[c2s_timeboxscript] = &KProtocolProcess::c2sTimeBoxScript;//TimeBox by kinnox;	
	ProcessFunc[c2s_topbattle] = &KProtocolProcess::c2sTopBattle;//Report Battle by kinnox;
	ProcessFunc[c2s_avatarplayer] = &KProtocolProcess::c2sAvatarPlayer; //Player Avatar by kinnox;	
	ProcessFunc[c2s_recoverybox] = &KProtocolProcess::RecoveryBoxCmd; //TrembleItem by kinnox;
	ProcessFunc[c2s_rightitemautomove] = &KProtocolProcess::PlayerRightAutoMove;//AutoRightClick
	ProcessFunc[c2s_autosortequipment] = &KProtocolProcess::PlayerAutoSortEquipment;
#endif
}

KProtocolProcess::~KProtocolProcess()
{
}

#ifndef _SERVER

void KProtocolProcess::ProcessNetMsg(BYTE* pMsg)
{
	if (!pMsg || pMsg[0] <= s2c_clientbegin || pMsg[0] >= s2c_end || ProcessFunc[pMsg[0]] == NULL)
	{
		return;
	}

	if (ProcessFunc[pMsg[0]])
		(this->*ProcessFunc[pMsg[0]])(pMsg);
}
#else



void KProtocolProcess::ProcessNetMsg(int nIndex, BYTE* pMsg)
{
	_ASSERT(pMsg && pMsg[0] > c2s_gameserverbegin && pMsg[0] < c2s_end);
	BYTE	byProtocol = pMsg[0];  // <-- THI?U D�NG N�Y!
	_ASSERT(nIndex > 0 && nIndex < MAX_PLAYER);

	if (   (pMsg[0] >= c2s_requestnpc && pMsg[0] <= c2s_buyplayershop)
		&& (pMsg[0] != c2s_npctalk)
		&& (pMsg[0] != c2s_npchurt)
		&& (pMsg[0] != c2s_npcdeath)
		&& (pMsg[0] != c2s_playertalk)
		&& (pMsg[0] != c2s_dbplayerselect)
		&& (pMsg[0] != c2s_requestobj)
		&& (pMsg[0] != c2s_requestnpc)
		&& (pMsg[0] != c2s_ping)
		)
	{
		DWORD	dwID = *(DWORD *)&pMsg[1];
		DWORD	dwTimePacker = *(DWORD *)&pMsg[5];
		if (dwID == Player[nIndex].GetPlayerID() && dwID > 0 && dwTimePacker > 0)
		{
			if (!Player[nIndex].AddAntiPaket(pMsg[0], dwTimePacker)) 
				return;
		}
	}
	if (ProcessFunc[byProtocol])
	{
		(this->*ProcessFunc[byProtocol])(nIndex, pMsg);
		Player[nIndex].SetLastNetOperationTime(g_SubWorldSet.GetGameTime());
		
	}

}
#endif

#ifndef _SERVER
extern IClientCallback* l_pDataChangedNotifyFunc;
//Lien Tram Kenny
/*void KProtocolProcess::ShowLienTram(BYTE* pMsg)
{
    const S2C_SHOW_LIEN_TRAM* pkt = (const S2C_SHOW_LIEN_TRAM*)pMsg;
    CoreDataChanged(GDCNI_SHOW_LIEN_TRAM, pkt->nValue, NULL);
}*/
void KProtocolProcess::NpcSleepSync(BYTE* pMsg)
{
	NPC_SLEEP_SYNC*	pSync = (NPC_SLEEP_SYNC *)pMsg;
	DWORD	dwNpcId = pSync->NpcID;

	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].SetSleepMode((BOOL)pSync->bSleep);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::s2cPing(BYTE* pMsg)
{
//	DWORD	dwTimer = GetTickCount();
//	PING_COMMAND* PingCmd = (PING_COMMAND *)pMsg;
//
//	dwTimer -= PingCmd->m_dwTime;
//	dwTimer >>= 1;
//	g_SubWorldSet.SetPing(dwTimer);
//	g_bPingReply = TRUE;

	DWORD	dwTimer = GetTickCount();
	PING_COMMAND*	PingCmd = (PING_COMMAND *)pMsg;
	PING_CLIENTREPLY_COMMAND	pcc;
	pcc.ProtocolType = c2s_ping;
	pcc.m_dwReplyServerTime = PingCmd->m_dwTime;
	pcc.m_dwClientTime = dwTimer;
	// DEBUG: Log PING receipt and PONG send attempt
	printf("[CLIENT-PING] Received PING with time=%u, sending PONG reply...\n", PingCmd->m_dwTime);
	if (g_pClient)
	{
		int sendResult = g_pClient->SendPackToServer(&pcc, sizeof(PING_CLIENTREPLY_COMMAND));
		printf("[CLIENT-PONG] SendPackToServer result=%d (0=success)\n", sendResult);
	}
	else
	{
		printf("[CLIENT-PONG-ERROR] g_pClient is NULL, cannot send PONG!\n");
	}
	//g_SubWorldSet.SetPing(PingCmd->m_dwTime);
}

void KProtocolProcess::ServerReplyClientPing(BYTE* pMsg)
{
	DWORD	dwTimer = GetTickCount();
	PING_COMMAND* pPc = (PING_COMMAND *)pMsg;

	dwTimer -= pPc->m_dwTime;
	dwTimer >>= 1;
	g_SubWorldSet.SetPing(dwTimer);
}

void KProtocolProcess::s2cPlayerExp(BYTE* pMsg)
{
	PLAYER_EXP_SYNC	*pExp = (PLAYER_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].s2cSetExp(pExp->m_nExp);
}

void	KProtocolProcess::s2cChatScreenSingleError(BYTE* pMsg)
{
	CHAT_SCREENSINGLE_ERROR_SYNC	*pError = (CHAT_SCREENSINGLE_ERROR_SYNC*)pMsg;
	
	// ֪ͨ������Ļ���Ĳ��ɹ�
	char	szName[32];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pError->m_szName, pError->m_wLength + 1 + sizeof(pError->m_szName) - sizeof(CHAT_SCREENSINGLE_ERROR_SYNC));

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_CHAT_TAR_REFUSE_SINGLE_TALK, szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}

void KProtocolProcess::NetCommandChgCamp(BYTE* pMsg)
{
	DWORD	dwNpcId;

	dwNpcId = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_Camp = (int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandChgCurCamp(BYTE* pMsg)
{
	DWORD	dwNpcId;

	dwNpcId = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_CurrentCamp = (int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandDeath(BYTE* pMsg)
{
	DWORD	dwNpcID;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcID);

	if (nIdx > 0)
	{
		Npc[nIdx].ProcNetCommand(do_death);
		Npc[nIdx].m_CurrentLife = 0;
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
#ifdef _DEBUG
		//g_DebugLog("[Death]Net command comes");
#endif
	}
}


//

void KProtocolProcess::NetCommandJump(BYTE* pMsg)
{
	NPC_JUMP_SYNC* pNetCommandJump = (NPC_JUMP_SYNC *)pMsg;

	if(!pNetCommandJump)
		return;

	DWORD dwNpcId = pNetCommandJump->m_dwID;
	int nIdx = NpcSet.SearchID(dwNpcId);
	
	if(Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{	
		Npc[nIdx].SendCommand(do_jump, pNetCommandJump->m_nMpsX, pNetCommandJump->m_nMpsY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}




void KProtocolProcess::NetCommandGlide(BYTE* pMsg)
{
	NPC_GLIDE_SYNC* pNetCommandGlide = (NPC_GLIDE_SYNC*)pMsg;

	if(!pNetCommandGlide)
		return;

	DWORD dwNpcId = pNetCommandGlide->m_dwID;
	int nIdx = NpcSet.SearchID(dwNpcId);
	
	if(Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{	
		Npc[nIdx].SendCommand(do_glide, pNetCommandGlide->m_nMpsX, pNetCommandGlide->m_nMpsY, (int)pNetCommandGlide->m_nLenght);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}


//

void KProtocolProcess::NetCommandHurt(BYTE* pMsg)
{
	NPC_HURT_SYNC*	pSync = (NPC_HURT_SYNC *)pMsg;
	
	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0)
	{
		//Npc[nIdx].SendCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].ProcNetCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandRemoveNpc(BYTE* pMsg)
{
	DWORD	dwNpcID;
	dwNpcID = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			// zroc change
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
//			SubWorld[Npc[nIdx].m_SubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
		}
		NpcSet.Remove(nIdx);
	}
}

void KProtocolProcess::NetCommandRun(BYTE* pMsg)
{
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	MapX = *(int *)&pMsg[5];
	MapY = *(int *)&pMsg[9];
	int nIdx = NpcSet.SearchID(dwNpcID);
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendCommand(do_run, MapX, MapY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandSit(BYTE* pMsg)
{
	NPC_SIT_SYNC*	pSitSync;

	pSitSync = (NPC_SIT_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pSitSync->ID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendCommand(do_sit);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::OpenSaleBox(BYTE* pMsg)
{
	SALE_BOX_SYNC* pSale = (SALE_BOX_SYNC *)pMsg;

	BuySell.OpenSale(pSale->nShopIndex);
}

void KProtocolProcess::OpenStoreBox(BYTE* pMsg)
{
	CoreDataChanged(GDCNI_OPEN_STORE_BOX, -1, NULL);
}

void KProtocolProcess::PlayerRevive(BYTE* pMsg)
{
	NPC_REVIVE_SYNC* pSync = (NPC_REVIVE_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0)
	{
		if (!Npc[nIdx].IsPlayer() && pSync->Type == REMOTE_REVIVE_TYPE)
		{
			SubWorld[0].m_WorldMessage.Send(GWM_NPC_DEL, nIdx);
			return;
		}
		else
		{
			Npc[nIdx].ProcNetCommand(do_revive);
		}
	}
}

void KProtocolProcess::RequestNpcFail(BYTE* pMsg)
{
	NPC_REQUEST_FAIL* pNpcSync = (NPC_REQUEST_FAIL *)pMsg;

	if (NpcSet.IsNpcRequestExist(pNpcSync->ID))
		NpcSet.RemoveNpcRequest(pNpcSync->ID);	
}

void KProtocolProcess::NetCommandSkill(BYTE* pMsg)
{
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel;
	int		MapX, MapY;
	
	BOOL bAuto;
	
	
	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	
	bAuto = *(BOOL *)&pMsg[21];
	
	
	if (MapY < 0)
		return ;

	if (MapX < 0)
	{
		if (MapX != -1)
			return;
	
		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)
			return;

		if (Npc[MapY].m_RegionIndex < 0)
			return;

	}
	
	int nIdx = NpcSet.SearchID(dwNpcID);
	
	if (nIdx <= 0) 
		return;

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{	
		//
		if(bAuto)
		{
			Npc[nIdx].AutoDoSkill(nSkillID, nSkillLevel, MapX, MapY);
		}
		else
		{
			Npc[nIdx].m_SkillList.SetSkillLevel(nSkillID, nSkillLevel);
			Npc[nIdx].SendCommand(do_skill, nSkillID, MapX, MapY);
		}
		
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandWalk(BYTE* pMsg)
{
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	BOOL	bRandMove;		
	
	dwNpcID = *(DWORD *)&pMsg[1];
	MapX = *(int *)&pMsg[5];
	MapY = *(int *)&pMsg[9];
	bRandMove = *(BOOL *)&pMsg[13];		
	int nIdx = NpcSet.SearchID(dwNpcID);
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{	
		//
		if(bRandMove)
		{
			Npc[nIdx].SendCommandRandMove(MapX, MapY);
		}
		else
		{
			Npc[nIdx].SendCommand(do_walk, MapX, MapY);
		}
		
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}


//	���ܣ��յ�������֪ͨ���鴴��ʧ��

void KProtocolProcess::s2cApplyCreateTeamFalse(BYTE* pMsg)
{
	PLAYER_SEND_CREATE_TEAM_FALSE *pCreateFalse = (PLAYER_SEND_CREATE_TEAM_FALSE*)pMsg;
	KSystemMessage	sMsg;

	switch (pCreateFalse->m_btErrorID)
	{
	// �Ѿ��ڶ����У�˵���ͻ��˶��������д����������»�ö�������
	case Team_Create_Error_InTeam:
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		break;

	// ��ǰ���ڲ������״̬
	case Team_Create_Error_CannotCreate:
		sprintf(sMsg.szMessage, MSG_TEAM_CANNOT_CREATE);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		break;


	default:
		sprintf(sMsg.szMessage, MSG_TEAM_CREATE_FAIL);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		break;
	}
}


//	���ܣ��յ�������֪ͨ��ѯĳ������������ʧ��

void KProtocolProcess::s2cApplyTeamInfoFalse(BYTE* pMsg)
{
	// ������ʾ�����ѯʧ��(not end)
}


//	���ܣ�����һ������

void KProtocolProcess::s2cChatAddFriend(BYTE* pMsg)
{
	CHAT_ADD_FRIEND_SYNC	*pFriend = (CHAT_ADD_FRIEND_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nIdx, pFriend->m_szName, 0);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	memset(&sPlayer, 0, sizeof(KUiPlayerItem));
	strcpy(sPlayer.Name, pFriend->m_szName);
	sPlayer.uId = pFriend->m_dwID;
	sPlayer.nIndex = pFriend->m_nIdx;

	sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_SUCCESS, pFriend->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_INTERVIEW;
	sMsg.byPriority = 2;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}


//	���ܣ����Ӻ���ʧ��

void KProtocolProcess::s2cChatAddFriendFail(BYTE* pMsg)
{
	CHAT_ADD_FRIEND_FAIL_SYNC	*pFail = (CHAT_ADD_FRIEND_FAIL_SYNC*)pMsg;

	char	szName[32];
	CChatApplyListNode	*pList;
	pList = (CChatApplyListNode*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.GetHead();
	while (pList)
	{
		if (pList->m_nPlayerIdx == pFail->m_nTargetPlayerIdx)
		{
			g_StrCpy(szName, pList->m_szName);

			// ֪ͨ��������ĳ��Ϊ���Ѳ���ʧ��
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_FAIL, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			return;
		}
		pList = (CChatApplyListNode*)pList->GetNext();
	}	
}


//	���ܣ��յ����������������ĳ��������ѵ����ݣ������֣�

void KProtocolProcess::s2cChatAddOneFriend(BYTE* pMsg)
{
	CHAT_ONE_FRIEND_DATA_SYNC	*pData = (CHAT_ONE_FRIEND_DATA_SYNC*)pMsg;
	int		nTeamNo;

	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pData->m_szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pData->m_dwID, pData->m_nPlayerIdx, pData->m_szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	// ֪ͨ����õ��³�Ա not end
}


//	���ܣ��������Ҿܾ�����

void KProtocolProcess::s2cChatBeRefusedAddFriend(BYTE* pMsg)
{
	CHAT_REFUSE_FRIEND_SYNC	*pRefuse = (CHAT_REFUSE_FRIEND_SYNC*)pMsg;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pRefuse->m_szName, pRefuse->m_wLength + 1 + sizeof(pRefuse->m_szName) - sizeof(CHAT_REFUSE_FRIEND_SYNC));

	// ֪ͨ����pRefuse->m_szName�ܾ������Ľ�������
	KSystemMessage	sMsg;

	sprintf(sMsg.szMessage, MSG_CHAT_REFUSE_FRIEND, szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}


//	���ܣ��õ�֪ͨ��������

void KProtocolProcess::s2cChatFriendOnline(BYTE* pMsg)
{
	CHAT_FRIEND_ONLINE_SYNC	*pFriend = (CHAT_FRIEND_ONLINE_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].ChatFriendOnLine(pFriend->m_dwID, pFriend->m_nPlayerIdx);
}


// �յ�������ת���ı�������������ѵ�����

void KProtocolProcess::s2cChatGetApplyAddFriend(BYTE* pMsg)
{
	CHAT_APPLY_ADD_FRIEND_SYNC	*pApply = (CHAT_APPLY_ADD_FRIEND_SYNC*)pMsg;

	if ( Player[CLIENT_PLAYER_INDEX].m_cChat.CheckIsFriend(pApply->m_nSrcPlayerIdx) )
		return;

	CChatApplyListNode	*pNode = NULL;
	pNode = new CChatApplyListNode;
	if (pNode == NULL)
		return;
	pNode->m_nPlayerIdx = pApply->m_nSrcPlayerIdx;
	strcpy(pNode->m_szName, pApply->m_szSourceName);
	strcpy(pNode->m_szInfo, pApply->m_szInfo);

	Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.AddTail(pNode);

	// ֪ͨ�����������������������
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	strcpy(sPlayer.Name, pApply->m_szSourceName);
	sPlayer.nIndex = pApply->m_nSrcPlayerIdx;
	sPlayer.uId = 0;
	sPlayer.nData = 0;

	sprintf(sMsg.szMessage, MSG_CHAT_GET_FRIEND_APPLY, pNode->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_FRIEND_INVITE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}


//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ������֣�

void KProtocolProcess::s2cChatLoginFriendName(BYTE* pMsg)
{
	CHAT_LOGIN_FRIEND_NAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NAME_SYNC*)pMsg;
	int		nTeamNo;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pFriend->m_szName, pFriend->m_wLength + 1 + sizeof(pFriend->m_szName) - sizeof(CHAT_LOGIN_FRIEND_NAME_SYNC));
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
}


//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ��������֣�

void KProtocolProcess::s2cChatLoginFriendNoName(BYTE* pMsg)
{
	CHAT_LOGIN_FRIEND_NONAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NONAME_SYNC*)pMsg;
	char	szName[32];
	int		nTeamNo;

	szName[0] = 0;
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, NULL);
	if (nTeamNo < 0)
	{
		if (Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag == 0)		// �������·������к������ݣ�������
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;

			CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplyresendallfriendname;
			sApply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			sApply.dwTimePacker = GetTickCount();
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND));
		}
		else												// �������·�������������ݣ�������
		{
			CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplysendonefriendname;
			sApply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			sApply.dwTimePacker = GetTickCount();
			sApply.m_dwID = pFriend->m_dwID;

			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND));
		}
		return;
	}
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;

}


//	���ܣ��յ�������֪ͨ����һ֧����

void KProtocolProcess::s2cCreateTeam(BYTE* pMsg)
{
	PLAYER_SEND_CREATE_TEAM_SUCCESS	*pCreateSuccess = (PLAYER_SEND_CREATE_TEAM_SUCCESS*)pMsg;

	g_Team[0].CreateTeam(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level, pCreateSuccess->nTeamServerID, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Series, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].RestoreCurrentCamp();

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_CREATE);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

	sprintf(sMsg.szMessage, MSG_TEAM_CREATE);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_CREATE_TEAM);
}


//	���ܣ��յ�������֪ͨ���ŵ�ǰ���ɼ���ĳ�ȼ�

void KProtocolProcess::s2cFactionSkillOpen(BYTE* pMsg)
{
	PLAYER_FACTION_SKILL_LEVEL	*pSkill = (PLAYER_FACTION_SKILL_LEVEL*)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction < 0 || Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction != pSkill->m_btCurFactionID)
	{
		// �������������Ϣ
		Player[CLIENT_PLAYER_INDEX].ApplyFactionData();
		return;
	}

//	for (int i = 0; i < FACTIONS_PRR_SERIES; i++)
//	{
//		if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nID == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction)
//		{
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel = pSkill->m_btLevel;
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//			break;
//		}
//	}
}


//	���ܣ��յ�������������������Ϣ

void KProtocolProcess::s2cGetChat(BYTE* pMsg)
{
	PLAYER_SEND_CHAT_SYNC	*pChat = (PLAYER_SEND_CHAT_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_cChat.GetChat(pChat);
}


//	���ܣ��յ��������������ĵ�ǰĳ���Եĵ���

void KProtocolProcess::s2cGetCurAttribute(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cGetCurAttribute(pMsg);
}


//	���ܣ��յ�������������ͳ��������ֵ

void KProtocolProcess::s2cGetLeadExp(BYTE* pMsg)
{
	DWORD	dwLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	PLAYER_LEAD_EXP_SYNC	*pLeadExp = (PLAYER_LEAD_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pLeadExp->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp, Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);

	if (dwLevel < Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_LEADER_LEVEL_UP, Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
		sMsg.eType = SMT_PLAYER;
		sMsg.byConfirmType = SMCT_CLICK;
		sMsg.byPriority = 1;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}


//	

void KProtocolProcess::s2cGetSkillLevel(BYTE* pMsg)
{
	int		nSkillIndex;
	PLAYER_SKILL_LEVEL_SYNC	*pSkill = (PLAYER_SKILL_LEVEL_SYNC*)pMsg;
	if (pSkill->m_nSkillLevel > MAX_SKILLLEVEL)
		return;

	nSkillIndex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(pSkill->m_nSkillID);
	Player[CLIENT_PLAYER_INDEX].m_nSkillPoint = pSkill->m_nLeavePoint;
	if (!nSkillIndex)
	{
		if (pSkill->m_nSkillLevel == 0)
		{
			if ( Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, 0, pSkill->m_bExpand) == 0 )
				return;
		}
		else
		{
			if ( Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, pSkill->m_nSkillLevel, pSkill->m_bExpand) == 0 )
				return;
		}
	}
	else
	{	
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetLevel(nSkillIndex, pSkill->m_nSkillLevel);
	}

	CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[CLIENT_PLAYER_INDEX].m_nSkillPoint);
	KUiSkillData SkillData;
	SkillData.uGenre = CGOG_SKILL_FIGHT;
	SkillData.uId = pSkill->m_nSkillID;
	SkillData.nLevel = pSkill->m_nSkillLevel;
	
	SkillData.cPointAdd = pSkill->m_btPointAdd;
	
	int nSkillPosition = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillPosition(pSkill->m_nSkillID);
	if(nSkillPosition < 0) 
		return;
	CoreDataChanged(GDCNI_SKILL_CHANGE, (unsigned int)&SkillData, nSkillPosition);
}


//	

void KProtocolProcess::s2cGetSkillRemove(BYTE* pMsg)
{
	PLAYER_SKILL_REMOVE_SYNC	*pSkill = (PLAYER_SKILL_REMOVE_SYNC*)pMsg;

	int		nSkillIndex = 0;
	nSkillIndex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(pSkill->m_nSkillID);
	
	if (!nSkillIndex)
	{
		return;
	}
	else
	{	
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Remove(pSkill->m_nSkillID);
	}
}


//	

void KProtocolProcess::s2cGetTeammateLevel(BYTE* pMsg)
{
	PLAYER_TEAMMATE_LEVEL_SYNC	*pLevel = (PLAYER_TEAMMATE_LEVEL_SYNC*)pMsg;
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	if ((DWORD)g_Team[0].m_nCaptain == pLevel->m_dwTeammateID)
	{
		g_Team[0].m_nMemLevel[0] = (DWORD)pLevel->m_btLevel;
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if ((DWORD)g_Team[0].m_nMember[i] == pLevel->m_dwTeammateID)
		{
			g_Team[0].m_nMemLevel[i + 1] = (DWORD)pLevel->m_btLevel;
			break;
		}
	}
}


//	���ܣ��յ�������֪ͨ�뿪����

void KProtocolProcess::s2cLeaveFaction(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cFaction.LeaveFaction();

	// �ı���Ӫ
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(camp_free);

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}


//	���ܣ��յ�������֪ͨ�г�Ա�뿪(�����Լ��뿪)

void KProtocolProcess::s2cLeaveTeam(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
		return;
	}

	PLAYER_LEAVE_TEAM	*pLeaveTeam = (PLAYER_LEAVE_TEAM*)pMsg;

	// �Լ��뿪
	if (pLeaveTeam->m_dwNpcID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
		g_Team[0].Release();
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TEAMOPEN)
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetMenuState(PLAYER_MENU_STATE_NORMAL);
	}
	// �����뿪
	else
	{
		g_Team[0].DeleteMember(pLeaveTeam->m_dwNpcID);
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	return;
}


//	���ܣ��յ�������֪ͨ����

void KProtocolProcess::s2cLevelUp(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cLevelUp(pMsg);
}

void KProtocolProcess::s2cMoveItem(BYTE* pMsg)
{
	PLAYER_MOVE_ITEM_SYNC	*pMove = (PLAYER_MOVE_ITEM_SYNC*)pMsg;

	ItemPos		DownPos, UpPos;
	DownPos.nPlace = pMove->m_btDownPos;
	DownPos.nX = pMove->m_btDownX;
	DownPos.nY = pMove->m_btDownY;
	UpPos.nPlace = pMove->m_btUpPos;
	UpPos.nX = pMove->m_btUpX;
	UpPos.nY = pMove->m_btUpY;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeItem(&DownPos, &UpPos);
}

/* Tam Thoi Code nay ok cho chuyen item
void KProtocolProcess::s2cRightItemAutoMove(BYTE* pMsg)
{
    ITEM_RIGHT_AUTO_MOVE* p = (ITEM_RIGHT_AUTO_MOVE*)pMsg;

    ItemPos src, dest;
    src.nPlace = p->SrcPlace; src.nX = p->SrcX; src.nY = p->SrcY;
    dest.nPlace = p->DestPlace; dest.nX = p->DestX; dest.nY = p->DestY;

    // ??ng b? UI/Client state theo server
    Player[CLIENT_PLAYER_INDEX].m_ItemList.HandleAutoMoveFromServer(src, dest, p->ItemID);

    // Th�ng b�o UI (tu? ch?n)
    if (p->ItemID > 0 && p->ItemID < MAX_ITEM && Item[p->ItemID].GetID() > 0)
    {
        const char* szName = Item[p->ItemID].GetName();
        if (szName && szName[0])
        {
            KSystemMessage msg;
            msg.eType = SMT_NORMAL;
            msg.byConfirmType = SMCT_NONE;
            msg.byPriority = 0;
            msg.byParamSize = 0;
            sprintf(msg.szMessage, "�� chuy�n %s th�nh c�ng", szName);
            CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&msg, 0);
        }
    }
}*/
void KProtocolProcess::GetPlaceName(int place, char* out, size_t outsz)
{
    if (!out || outsz == 0)
        return;

    strcpy(out, "r��ng ch�a ��");

    if (place == pos_equiproom)
    {
        strcpy(out, "h�nh trang");
    }
    else if (place == pos_immediacy)
    {
        strcpy(out, "� nhanh");
    }
    else if (place == pos_expandtoryroom1)
    {
        strcpy(out, "t�i m� r�ng");
    }
    else if (place >= pos_repositoryroom && place <= pos_repositoryroom5)
    {
        int page = (place - pos_repositoryroom) + 1; // 1..5
        sprintf(out, "r��ng ch�a �� (trang %d)", page);
    }

}

//Kenny Lam right click
void KProtocolProcess::s2cRightItemAutoMove(BYTE* pMsg)
{
    ITEM_RIGHT_AUTO_MOVE* p = (ITEM_RIGHT_AUTO_MOVE*)pMsg;

    ItemPos src, dest;
    src.nPlace = p->SrcPlace;  src.nX = p->SrcX;  src.nY = p->SrcY;
    dest.nPlace = p->DestPlace; dest.nX = p->DestX; dest.nY = p->DestY;
    int nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(p->ItemID);
    // HandleAutoMoveFromServer theo (SrcX,SrcY)
    if (nIdx <= 0)
        nIdx = 0;

    Player[CLIENT_PLAYER_INDEX].m_ItemList.HandleAutoMoveFromServer(src, dest, nIdx);

    const char* szName = NULL;
    int itemIdx = (nIdx > 0) ? nIdx
                             : Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(p->ItemID);

    if (itemIdx > 0 && itemIdx < MAX_ITEM && Item[itemIdx].GetID() == p->ItemID)
        szName = Item[itemIdx].GetName();

    
    if ((!szName || !szName[0]))
    {
        int destRoom = Player[CLIENT_PLAYER_INDEX].m_ItemList.PositionToRoom(dest.nPlace);
        if (destRoom >= 0)
        {
            int guessIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.FindItemInRoom(
                               (INVENTORY_ROOM)destRoom, dest.nX, dest.nY);
            if (guessIdx > 0 && guessIdx < MAX_ITEM && Item[guessIdx].GetID() == p->ItemID)
            {
                itemIdx = guessIdx;
                szName = Item[guessIdx].GetName();
            }
        }
    }

    if (szName && szName[0])
    {
        char whereBuf[64];
        GetPlaceName(dest.nPlace, whereBuf, sizeof(whereBuf));

        KSystemMessage msg;
        msg.eType         = SMT_NORMAL;
        msg.byConfirmType = SMCT_NONE;
        msg.byPriority    = 0;
        msg.byParamSize   = 0;

        sprintf(msg.szMessage, "�� chuy�n <color=yellow>%s<color> v�o %s th�nh c�ng", szName, whereBuf);
        CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&msg, 0);
    }
}

void KProtocolProcess::s2cRemoveItem(BYTE* pMsg)
{
	ITEM_REMOVE_SYNC	*pRemove = (ITEM_REMOVE_SYNC*)pMsg;

	int		nIdx;
	nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(pRemove->m_ID);
	if (nIdx > 0)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.Remove(nIdx);
		Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	}

	
}

//	���ܣ��յ�������������������Ϣ

void KProtocolProcess::s2cSetFactionData(BYTE* pMsg)
{
	PLAYER_FACTION_DATA	*pData = (PLAYER_FACTION_DATA*)pMsg;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(pData->m_btCamp);
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction = (char)pData->m_btCurFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nFirstAddFaction = (char)pData->m_btFirstFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nAddTimes = pData->m_nAddTimes;

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel = pData->m_btNo1SkillOpenLevel;
//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel = pData->m_btNo2SkillOpenLevel;
//	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nID)
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//	else
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
}


//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)

void KProtocolProcess::s2cSetTeamState(BYTE* pMsg)
{
	// �ͻ����������������������
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	if (pTeamState->m_btOpenClose)
	{
		g_Team[0].SetTeamOpen();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_OPEN);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else
	{
		g_Team[0].SetTeamClose();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CLOSE);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}


//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)

void	KProtocolProcess::s2cApplyAddTeam(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cApplyAddTeam(pMsg);
}


//	���ܣ��յ�������������ĳ�����������������ʾ֮

void KProtocolProcess::s2cShowTeamInfo(BYTE* pMsg)
{
}


void KProtocolProcess::s2cSyncItem(BYTE* pMsg)
{
	ITEM_SYNC* pItemSync = (ITEM_SYNC*)pMsg;

	int i, pnMagicParam[6];
	for(i = 0; i < 6; i++)
		pnMagicParam[i] = pItemSync->m_MagicLevel[i];

	const int nIndex = ItemSet.AddExist(
		pItemSync->m_Genre,
		pItemSync->m_Series,
		pItemSync->m_Level,
		pItemSync->m_Record,
		pItemSync->m_Luck,
		pItemSync->m_Detail,
		pnMagicParam,
		pItemSync->m_Version,
		pItemSync->m_RandomSeed);
	
	if( (nIndex <= 0) || (nIndex >= MAX_ITEM) )
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
		return;
	}

	Item[nIndex].SetID(pItemSync->m_ID);
	Item[nIndex].SetDurability((short)pItemSync->m_Durability);
	//
	if(pItemSync->m_BindState > 2000000000)
		Item[nIndex].SetBindState(pItemSync->m_BindState, TRUE);
	else
		Item[nIndex].SetBindState(pItemSync->m_BindState);

	Item[nIndex].SetTime(pItemSync->m_ExpiredTime);
	//
	Item[nIndex].SetPlayerShopPrice(pItemSync->m_ShopPrice);

	Player[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pItemSync->m_Place, pItemSync->m_X, pItemSync->m_Y);
	//
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}


//	���ܣ��յ���������������ͬ��money����Ϣ

void KProtocolProcess::s2cSyncMoney(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cSyncMoney(pMsg);
}

void KProtocolProcess::s2cSyncRoleList(BYTE* pMsg)
{
	ROLE_LIST_SYNC	*pSync = (ROLE_LIST_SYNC *)pMsg;
	// TODO: ͨ��CORESHELL�������棿
}


//	���ܣ��յ�������֪ͨ�����˼������

void KProtocolProcess::s2cTeamAddMember(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag )
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_ADD_MEMBER	*pAddMem = (PLAYER_TEAM_ADD_MEMBER*)pMsg;
	int		nMemNum;

	nMemNum = g_Team[0].FindFree();
	if (nMemNum < 0)
		return;
	g_Team[0].m_nMember[nMemNum] = pAddMem->m_dwNpcID;
	g_Team[0].m_nMemLevel[nMemNum + 1] = (DWORD)pAddMem->m_btLevel;
	g_Team[0].m_nMemTP[nMemNum + 1] = pAddMem->m_TP; ////KEnny
	g_Team[0].m_nMemSM[nMemNum + 1] = pAddMem->m_SM; ////KEnny
	g_Team[0].m_nMemHP[nMemNum + 1] = pAddMem->m_HP; ////KEnny
	g_Team[0].m_nMemMHP[nMemNum + 1] = pAddMem->m_MHP; ////KEnny
	strcpy(g_Team[0].m_szMemName[nMemNum + 1], pAddMem->m_szName);
	g_Team[0].m_nMemNum++;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(pAddMem->m_dwNpcID);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_ADD_MEMBER, pAddMem->m_szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}


//	���ܣ��յ�������֪ͨ�����ӳ�

void KProtocolProcess::s2cTeamChangeCaptain(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}
	int		nLevel, nMemNo, nHP, nMHP, nSM, nTP;
	char	szName[32];
	PLAYER_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_TEAM_CHANGE_CAPTAIN*)pMsg;

	nMemNo = g_Team[0].FindMemberID(pChange->m_dwCaptainID);
	// ��ǰ������������
	if ((DWORD)g_Team[0].m_nCaptain != pChange->m_dwMemberID || nMemNo < 0)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	// �Լ�������Ϊ�ӳ�
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_SELF, g_Team[0].m_szMemName[0]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else	// ���˱�����Ϊ�ӳ�
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN, g_Team[0].m_szMemName[nMemNo + 1]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}

	// ������Ա
	nLevel = g_Team[0].m_nMemLevel[0];
	nHP = g_Team[0].m_nMemHP[0];
	nMHP = g_Team[0].m_nMemMHP[0];
	nSM = g_Team[0].m_nMemSM[0];
	nTP = g_Team[0].m_nMemTP[0];
	strcpy(szName, g_Team[0].m_szMemName[0]);
	g_Team[0].m_nCaptain = pChange->m_dwCaptainID;
	g_Team[0].m_nMemLevel[0] = g_Team[0].m_nMemLevel[nMemNo + 1];
	g_Team[0].m_nMemTP[0] = g_Team[0].m_nMemTP[nMemNo + 1];
	g_Team[0].m_nMemSM[0] = g_Team[0].m_nMemSM[nMemNo + 1];
	g_Team[0].m_nMemHP[0] = g_Team[0].m_nMemHP[nMemNo + 1];
	g_Team[0].m_nMemMHP[0] = g_Team[0].m_nMemMHP[nMemNo + 1];
	strcpy(g_Team[0].m_szMemName[0], g_Team[0].m_szMemName[nMemNo + 1]);
	g_Team[0].m_nMember[nMemNo] = pChange->m_dwMemberID;
	g_Team[0].m_nMemLevel[nMemNo + 1] = nLevel;
	g_Team[0].m_nMemTP[nMemNo + 1] = nTP;
	g_Team[0].m_nMemHP[nMemNo + 1] = nHP;
	g_Team[0].m_nMemMHP[nMemNo + 1] = nMHP;
	g_Team[0].m_nMemSM[nMemNo + 1] = nSM;
	strcpy(g_Team[0].m_szMemName[nMemNo + 1], szName);

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}
	else if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwMemberID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
}


//	���ܣ��յ��������������Լ��������������������Ӧ��Ϣ

void KProtocolProcess::s2cUpdataSelfTeamInfo(BYTE* pMsg)
{
	int		i;
	PLAYER_SEND_SELF_TEAM_INFO	*pSelfInfo = (PLAYER_SEND_SELF_TEAM_INFO*)pMsg;

	// ���ݰ�����
	if (pSelfInfo->m_dwNpcID[0] == 0)
		return;

	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pSelfInfo->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	if (Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp == 0)
		Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pSelfInfo->m_dwNpcID[0])	// �ӳ�
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	}
	else													// ��Ա
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].Release();
	}
	g_Team[0].m_nCaptain = pSelfInfo->m_dwNpcID[0];
	if (pSelfInfo->m_btState == 0)
		g_Team[0].SetTeamClose();
	else
		g_Team[0].SetTeamOpen();
	g_Team[0].m_nTeamServerID = pSelfInfo->nTeamServerID;
	g_Team[0].m_nMemTP[0] = (UINT)pSelfInfo->m_TP[0];
	g_Team[0].m_nMemLevel[0] = (DWORD)pSelfInfo->m_btLevel[0];
	strcpy(g_Team[0].m_szMemName[0], pSelfInfo->m_szNpcName[0]);
	g_Team[0].m_nMemSM[0] = (UINT)pSelfInfo->m_SM[0];//thay kenny
	g_Team[0].m_nMemHP[0] = (UINT)pSelfInfo->m_HP[0];//thay kenny
	g_Team[0].m_nMemMHP[0] = (UINT)pSelfInfo->m_MHP[0];//thay kenny
	g_Team[0].m_nMemNum = 0;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pSelfInfo->m_dwNpcID[i + 1] > 0)
		{
			g_Team[0].m_nMember[i] = pSelfInfo->m_dwNpcID[i + 1];
			g_Team[0].m_nMemLevel[i + 1] = (DWORD)pSelfInfo->m_btLevel[i + 1];
			strcpy(g_Team[0].m_szMemName[i + 1], pSelfInfo->m_szNpcName[i + 1]);
			g_Team[0].m_nMemTP[i + 1]  = (UINT)pSelfInfo->m_TP[i + 1] ;
			g_Team[0].m_nMemSM[i + 1]  = (UINT)pSelfInfo->m_SM[i + 1] ;
			g_Team[0].m_nMemHP[i + 1]  = (UINT)pSelfInfo->m_HP[i + 1] ;
			g_Team[0].m_nMemMHP[i + 1]  = (UINT)pSelfInfo->m_MHP[i + 1] ;
			g_Team[0].m_nMemNum++;
		}
		else
		{
			g_Team[0].m_nMember[i] = -1;
			g_Team[0].m_nMemLevel[i + 1] = 0;
			g_Team[0].m_szMemName[i + 1][0] = 0;
			g_Team[0].m_nMemSM[i + 1]  = 0;
			g_Team[0].m_nMemTP[i + 1]  = 0;
			g_Team[0].m_nMemHP[i + 1]  = 0;
			g_Team[0].m_nMemMHP[i + 1]  = 0;
		}
	}
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
}




void KProtocolProcess::SyncCurNormalData(BYTE* pMsg)
{
	CURPLAYER_NORMAL_SYNC	*pSync = (CURPLAYER_NORMAL_SYNC*)pMsg;
	if (pSync->m_ushLife > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = (UINT)pSync->m_ushLife;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = 0;
	if (pSync->m_ushStamina > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = (int)pSync->m_ushStamina;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = 0;

	if (pSync->m_ushMana > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = (int)pSync->m_ushMana;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = 0;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	switch (pSync->m_btTeamData)
	{
	case 0x00:
		if ( Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag )
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x03:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x01:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_MEMBER)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	}
}

void KProtocolProcess::SyncCurPlayer(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].SyncCurPlayer(pMsg);
}




void KProtocolProcess::SyncNpc(BYTE* pMsg)
{
	NPC_SYNC* NpcSync = (NPC_SYNC*)pMsg;

	int nNewRegion, nNewMapX, nNewMapY, nNewOffX, nNewOffY;
	SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nNewRegion, &nNewMapX, &nNewMapY, &nNewOffX, &nNewOffY);

	if(nNewRegion == -1)
		return;

	int nIdx = NpcSet.SearchID(NpcSync->ID);
	if( (nIdx <= 0) || (nIdx >= MAX_NPC) )
	{	
		nIdx = NpcSet.Add(NpcSync->m_bySeries, NpcSync->NpcSettingIdx, 0, NpcSync->MapX, NpcSync->MapY);
		Npc[nIdx].m_dwID = NpcSync->ID;
		Npc[nIdx].m_Kind = NpcSync->m_btKind;
		Npc[nIdx].m_Height = 0;

		if( NpcSet.IsNpcRequestExist(NpcSync->ID) )
			NpcSet.RemoveNpcRequest(NpcSync->ID);
	}
	else
	{	
		if(Npc[nIdx].m_RegionIndex >= 0)
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);

		Npc[nIdx].m_MapX = nNewMapX;
		Npc[nIdx].m_MapY = nNewMapY;
		Npc[nIdx].m_OffX = nNewOffX;
		Npc[nIdx].m_OffY = nNewOffY;

		Npc[nIdx].m_NpcSettingIdx = (short)HIWORD(NpcSync->NpcSettingIdx);
		Npc[nIdx].m_Level = LOWORD(NpcSync->NpcSettingIdx);

		if(Npc[nIdx].m_RegionIndex >= 0)
		{
			if(Npc[nIdx].m_RegionIndex != nNewRegion)
			{
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nNewRegion].m_RegionID, nIdx);
				Npc[nIdx].m_RegionIndex = nNewRegion;
			}
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
		}
		else
		{	
			SubWorld[0].m_Region[nNewRegion].AddNpc(nIdx);
			Npc[nIdx].m_RegionIndex = nNewRegion;
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
		}
	}
	Npc[nIdx].m_Camp			= (NPCCAMP)NpcSync->Camp;
	Npc[nIdx].m_CurrentCamp		= (NPCCAMP)NpcSync->CurrentCamp;
	Npc[nIdx].m_Series			= NpcSync->m_bySeries;
	Npc[nIdx].m_cGold.SetGoldCurrentType(NpcSync->NpcEnchant);

	if (NpcSync->LifePerCent <= 128)
		Npc[nIdx].m_CurrentLife	= (Npc[nIdx].m_CurrentLifeMax * NpcSync->LifePerCent) >> 7 ;
	else
		Npc[nIdx].m_CurrentLife	= 0;
	
	if (Npc[nIdx].m_Doing != do_death || Npc[nIdx].m_Doing != do_revive)
		Npc[nIdx].SendCommand((NPCCMD)NpcSync->m_Doing, NpcSync->MapX, NpcSync->MapY);

	Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	Npc[nIdx].SetMenuState(NpcSync->m_btMenuState);

	memset(Npc[nIdx].Name, 0, sizeof(Npc[nIdx].Name));
	memcpy(Npc[nIdx].Name, NpcSync->m_szName, NpcSync->m_wLength - (sizeof(NPC_SYNC) - 1 - sizeof(NpcSync->m_szName)));
}




void KProtocolProcess::SyncNpcMin(BYTE* pMsg)
{
NPC_NORMAL_SYNC* NpcSync = (NPC_NORMAL_SYNC*)pMsg;
	
	if( (NpcSync->Doing == do_revive) || (NpcSync->Doing == do_death) )
		return;

	const int nIdx = NpcSet.SearchID(NpcSync->ID);
	if( (nIdx <= 0) || (nIdx >= MAX_NPC) )
	{
		if( !NpcSet.IsNpcRequestExist(NpcSync->ID) )
		{
			SendClientCmdRequestNpc(NpcSync->ID);
			NpcSet.InsertNpcRequest(NpcSync->ID);
		}
	}
	else
	{	
		if(nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
		{	
			int nNewRegion, nNewMapX, nNewMapY, nNewOffX, nNewOffY;
			SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nNewRegion, &nNewMapX, &nNewMapY, &nNewOffX, &nNewOffY);
			
			if(Npc[nIdx].m_RegionIndex == -1)	
			{
				if(nNewRegion == -1)
					return;
				
				Npc[nIdx].m_MapX = nNewMapX;
				Npc[nIdx].m_MapY = nNewMapY;
				Npc[nIdx].m_OffX = nNewOffX;
				Npc[nIdx].m_OffY = nNewOffY;

				Npc[nIdx].m_RegionIndex = nNewRegion;
		
				SubWorld[0].m_Region[nNewRegion].AddNpc(nIdx);
				Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nNewRegion].m_RegionID;
				SubWorld[0].m_Region[nNewRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
				
				if(NpcSync->Doing == do_stand)
					Npc[nIdx].ProcNetCommand(do_stand);
			}
			else if(Npc[nIdx].m_RegionIndex != nNewRegion)
			{	
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);

				Npc[nIdx].m_MapX = nNewMapX;
				Npc[nIdx].m_MapY = nNewMapY;
				Npc[nIdx].m_OffX = nNewOffX;
				Npc[nIdx].m_OffY = nNewOffY;
				Npc[nIdx].m_RegionIndex = nNewRegion;
				
				if(nNewRegion >= 0)
				{
					SubWorld[0].m_Region[nNewRegion].AddNpc(nIdx);
					Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nNewRegion].m_RegionID;
					SubWorld[0].m_Region[nNewRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
				}

			}
			/*int	nOldLife = Npc[nIdx].m_CurrentLife;
			if (NpcSync->LifePerCent <= 128)
			{
				Npc[nIdx].m_CurrentLife = (Npc[nIdx].m_CurrentLifeMax * NpcSync->LifePerCent) >> 7;
			}
			else
			{
				Npc[nIdx].m_CurrentLife = 0;
			}*/
			Npc[nIdx].m_CurrentCamp = NpcSync->Camp;
			int	nOldLife = Npc[nIdx].m_CurrentLife;

			Npc[nIdx].m_CurrentLifeMax	= NpcSync->m_CurrentLifeMax;
			Npc[nIdx].m_CurrentLife		= NpcSync->m_CurrentLife;

			if (Npc[nIdx].m_Kind == kind_normal)
				Npc[nIdx].SetBlood(nOldLife - Npc[nIdx].m_CurrentLife);

			Npc[nIdx].m_CurrentMana			= NpcSync->m_CurrentMana;
			Npc[nIdx].m_CurrentManaMax		= NpcSync->m_CurrentManaMax;
		}
		Npc[nIdx].m_CurrentWalkSpeed	= NpcSync->m_CurrentWalkSpeed;
		Npc[nIdx].m_CurrentRunSpeed		= NpcSync->m_CurrentRunSpeed;
		Npc[nIdx].m_CurrentAttackSpeed	= NpcSync->m_ASpeed;
		Npc[nIdx].m_CurrentCastSpeed	= NpcSync->m_CSpeed;

		
		Npc[nIdx].m_FreezeState.nTime = NpcSync->State & STATE_FREEZE;
		Npc[nIdx].m_PoisonState.nTime = NpcSync->State & STATE_POISON;
		Npc[nIdx].m_StunState.nTime = NpcSync->State & STATE_STUN;
		
		Npc[nIdx].m_Poison2DecManaState.nTime = NpcSync->State & STATE_POISON2MANA;
		Npc[nIdx].m_HideState.nTime = NpcSync->State & STATE_HIDE;
		Npc[nIdx].m_HideState.nValue[0] = NpcSync->HideValue;
		Npc[nIdx].m_RandMoveState.nTime = NpcSync->State & STATE_RANDMOVE;
		Npc[nIdx].m_IgnoreNegativeState.nTime = NpcSync->State & STATE_IGNORENEGATIVE;
		

		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}


//	

void KProtocolProcess::SyncNpcMinPlayer(BYTE* pMsg)
{	
	NPC_PLAYER_TYPE_NORMAL_SYNC	*pSync = (NPC_PLAYER_TYPE_NORMAL_SYNC*)pMsg;

	const int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return;

	if (pSync->m_dwNpcID != Npc[nNpcIdx].m_dwID)
		return;

	int nNewRegion, nNewMapX, nNewMapY, nNewOffX, nNewOffY;
	SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nNewRegion, &nNewMapX, &nNewMapY, &nNewOffX, &nNewOffY);
	
	if(Npc[nNpcIdx].m_RegionIndex == -1)
	{
		if( nNewRegion < 0 )
			return;
		
		Npc[nNpcIdx].m_RegionIndex = nNewRegion;
		Npc[nNpcIdx].m_dwRegionID = SubWorld[0].m_Region[nNewRegion].m_RegionID;

		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nNewRegion].m_RegionID, nNpcIdx);
		SubWorld[0].m_Region[nNewRegion].AddRef(nNewMapX, nNewMapY, obj_npc);

		Npc[nNpcIdx].m_MapX = nNewMapX;
		Npc[nNpcIdx].m_MapY = nNewMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;

		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		return;
	}

	if(nNewRegion == -1)
	{	
		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].DecRef(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, obj_npc);

		int nRegionX = pSync->m_dwMapX / (SubWorld[0].m_nCellWidth * SubWorld[0].m_nRegionWidth);
		int nRegionY = pSync->m_dwMapY / (SubWorld[0].m_nCellHeight * SubWorld[0].m_nRegionHeight);
		
		DWORD	dwRegionID = MAKELONG(nRegionX, nRegionY);
		SubWorld[0].LoadMap(SubWorld[0].m_SubWorldID, dwRegionID);

		nNewRegion = SubWorld[0].FindRegion(dwRegionID);

		Npc[nNpcIdx].m_RegionIndex = nNewRegion;
		Npc[nNpcIdx].m_dwRegionID = dwRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nNewRegion].m_RegionID, nNpcIdx);
		SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nNewRegion, &nNewMapX, &nNewMapY, &nNewOffX, &nNewOffY);
		Npc[nNpcIdx].m_MapX = nNewMapX;
		Npc[nNpcIdx].m_MapY = nNewMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;
		
		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].AddRef(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, obj_npc);
		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}


//	

void KProtocolProcess::SyncObjectAdd(BYTE* pMsg)
{
	OBJ_ADD_SYNC	*pObjSyncAdd = (OBJ_ADD_SYNC*)pMsg;
	int				nObjIndex;
	KObjItemInfo	sInfo;

	nObjIndex = ObjSet.FindID(pObjSyncAdd->m_nID);
	if (nObjIndex > 0)
		return;

	sInfo.m_nItemID = pObjSyncAdd->m_nItemID;
	sInfo.m_dwItemID = pObjSyncAdd->m_dwItemID;
	sInfo.m_nItemWidth = pObjSyncAdd->m_btItemWidth;
	sInfo.m_nItemHeight = pObjSyncAdd->m_btItemHeight;
	sInfo.m_nMoneyNum = pObjSyncAdd->m_nMoneyNum;
	sInfo.m_nColorID = pObjSyncAdd->m_btColorID;
	//AutoAI by kinnox
	sInfo.m_nGenre = pObjSyncAdd->m_nGenre;
	sInfo.m_nDetailType = pObjSyncAdd->m_nDetailType;
	sInfo.m_dwNpcId = pObjSyncAdd->m_dwNpcId;
	//end code
	sInfo.m_nMovieFlag = ((pObjSyncAdd->m_btFlag & 0x02) > 0 ? 1 : 0);
	sInfo.m_nSoundFlag = ((pObjSyncAdd->m_btFlag & 0x01) > 0 ? 1 : 0);
	memset(sInfo.m_szName, 0, sizeof(sInfo.m_szName));
	memcpy(sInfo.m_szName, pObjSyncAdd->m_szName, pObjSyncAdd->m_wLength + 1 + sizeof(pObjSyncAdd->m_szName) - sizeof(OBJ_ADD_SYNC));

	nObjIndex = ObjSet.ClientAdd(
		pObjSyncAdd->m_nID,
		pObjSyncAdd->m_nDataID,
		pObjSyncAdd->m_btState,
		pObjSyncAdd->m_btDir,
		pObjSyncAdd->m_wCurFrame,
		pObjSyncAdd->m_nXpos,
		pObjSyncAdd->m_nYpos,
		sInfo);
#ifdef WAIGUA_ZROC
	if (nObjIndex <= 0)
		return;
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
	{
//		sPickUp.ProtocolType = c2s_playerpickupitem;
//		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
//		sPickUp.m_btPosType = 0;
//		sPickUp.m_btPosX = 0;
//		sPickUp.m_btPosY = 0;
//		if (g_pClient)
//			g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
	}
	else if (Object[nObjIndex].m_nKind == Obj_Kind_Item)
	{
		ItemPos	sItemPos;
		if ( FALSE == Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
			return;
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sPickUp.dwTimePacker = GetTickCount();
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;
		sPickUp.m_btPosY = sItemPos.nY;
		if (g_pClient)
			g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
	}
#endif
}


//	

void KProtocolProcess::SyncObjectDir(BYTE* pMsg)
{
	OBJ_SYNC_DIR	*pObjSyncDir = (OBJ_SYNC_DIR*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncDir->m_nID);
	if (nObjIndex <= 0)
	{
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sObjClientSyncAdd.dwTimePacker = GetTickCount();
		sObjClientSyncAdd.m_nID = pObjSyncDir->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{
		Object[nObjIndex].SetDir(pObjSyncDir->m_btDir);
	}
}


//	���ܣ��յ���������Ϣɾ��ĳ��obj

void KProtocolProcess::SyncObjectRemove(BYTE* pMsg)
{
	OBJ_SYNC_REMOVE	*pObjSyncRemove = (OBJ_SYNC_REMOVE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncRemove->m_nID);
	if (nObjIndex > 0)
	{	// ɾ��
		Object[nObjIndex].Remove(pObjSyncRemove->m_btSoundFlag);
	}
}


//	���ܣ��յ���������Ϣ����ĳ��obj״̬

void KProtocolProcess::SyncObjectState(BYTE* pMsg)
{
	OBJ_SYNC_STATE	*pObjSyncState = (OBJ_SYNC_STATE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncState->m_nID);
	if (nObjIndex <= 0)
	{
		// �����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sObjClientSyncAdd.dwTimePacker = GetTickCount();
		sObjClientSyncAdd.m_nID = pObjSyncState->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{	// ͬ��״̬
		if (Object[nObjIndex].m_nRegionIdx == -1)
		{
			int	nRegion;
			nRegion = SubWorld[0].FindRegion(Object[nObjIndex].m_nBelongRegion);
			if (nRegion >= 0)
			{
				Object[nObjIndex].m_nRegionIdx = nRegion;
				SubWorld[0].m_Region[nRegion].AddObj(nObjIndex);
			}
		}
		Object[nObjIndex].SetState(pObjSyncState->m_btState);
	}
}


//

void KProtocolProcess::SyncObjectTrap(BYTE* pMsg)
{
	OBJ_SYNC_TRAP_ACT *pObjTrapSync = (OBJ_SYNC_TRAP_ACT*)pMsg;
	int		nObjIndex;
	nObjIndex = ObjSet.FindID(pObjTrapSync->m_nID);
	if (nObjIndex <= 0)
	{
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
		sObjClientSyncAdd.dwTimePacker = GetTickCount();
		sObjClientSyncAdd.m_nID = pObjTrapSync->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{
		Object[nObjIndex].m_nState = OBJ_TRAP_STATE_ACTING;
		Object[nObjIndex].m_cImage.SetDirStart();
		Object[nObjIndex].m_cSkill.m_nTarX = pObjTrapSync->m_nTarX;
		Object[nObjIndex].m_cSkill.m_nTarY = pObjTrapSync->m_nTarY;
	}
}

void KProtocolProcess::SyncPlayer(BYTE* pMsg)
{
	PLAYER_SYNC*	pPlaySync = (PLAYER_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);

	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	//
	Npc[nIdx].m_CurrentAttackSpeed	= pPlaySync->AttackSpeed;
	Npc[nIdx].m_CurrentCastSpeed	= pPlaySync->CastSpeed;
	
	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_HorseType			= (char)pPlaySync->HorseType;
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;
	//
	Npc[nIdx].m_wMaskType			= pPlaySync->MaskType;
	Npc[nIdx].m_MantleType			= pPlaySync->MantleType;
	Npc[nIdx].m_byMantleLevel		= pPlaySync->MantleLevel;
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	
	Npc[nIdx].m_cPKValue			= pPlaySync->PKValue;
	Npc[nIdx].m_nWorldStat			= pPlaySync->WorldStat;
	Npc[nIdx].m_nRepute				= pPlaySync->Repute;
	Npc[nIdx].m_nFuyuan				= pPlaySync->Fuyuan;	
	Npc[nIdx].m_cTransLife			= pPlaySync->TransLife;	
	Npc[nIdx].m_bOpenShop			= pPlaySync->OpenShop;
	sprintf(Npc[nIdx].m_szShopAdv, pPlaySync->ShopAdv);
	sprintf(Npc[nIdx].m_szTongName, pPlaySync->TongName);
	sprintf(Npc[nIdx].m_szTongTitle, pPlaySync->TongTitle);
	Npc[nIdx].m_wTitleId			= pPlaySync->TitleID;
	//Player Avatar by kinnox;
	Npc[nIdx].m_ImagePlayer			= pPlaySync->ImagePlayer;
	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		Player[CLIENT_PLAYER_INDEX].m_ImagePlayer		= pPlaySync->ImagePlayer;
	}
	//end code
	//
	if (Npc[nIdx].m_HorseType >= 0) 
	{
		Npc[nIdx].m_bRideHorse = TRUE;
	}
	else
	{
		Npc[nIdx].m_bRideHorse = FALSE;
	}
	
	//
	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_btPKFlag		= 1;
	else if(pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_btPKFlag		= 2;
	else
		Npc[nIdx].m_btPKFlag		= 0;
	
	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;
	else
		Npc[nIdx].m_FightMode		= 0;
	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);
	
}

void KProtocolProcess::SyncPlayerMin(BYTE* pMsg)
{
	PLAYER_NORMAL_SYNC* pPlaySync = (PLAYER_NORMAL_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);

	//
	Npc[nIdx].m_CurrentAttackSpeed	= pPlaySync->AttackSpeed;
	Npc[nIdx].m_CurrentCastSpeed	= pPlaySync->CastSpeed;
	
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;
	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	Npc[nIdx].m_HorseType			= (char)pPlaySync->HorseType;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;
	//
	Npc[nIdx].m_wMaskType			= pPlaySync->MaskType;
	Npc[nIdx].m_MantleType			= pPlaySync->MantleType;
	Npc[nIdx].m_byMantleLevel		= pPlaySync->MantleLevel;
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	
	Npc[nIdx].m_cPKValue			= pPlaySync->PKValue;
	Npc[nIdx].m_nWorldStat			= pPlaySync->WorldStat;
	Npc[nIdx].m_nRepute				= pPlaySync->Repute;
	Npc[nIdx].m_nFuyuan				= pPlaySync->Fuyuan;	
	Npc[nIdx].m_cTransLife			= pPlaySync->TransLife;	
	Npc[nIdx].m_bOpenShop			= pPlaySync->OpenShop;
	sprintf(Npc[nIdx].m_szShopAdv, pPlaySync->ShopAdv);
	sprintf(Npc[nIdx].m_szTongName, pPlaySync->TongName);
	sprintf(Npc[nIdx].m_szTongTitle, pPlaySync->TongTitle);
	Npc[nIdx].m_wTitleId			= pPlaySync->TitleID;
	//Player Avatar by kinnox;
	Npc[nIdx].m_ImagePlayer			= pPlaySync->ImagePlayer;
	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		Player[CLIENT_PLAYER_INDEX].m_ImagePlayer		= pPlaySync->ImagePlayer;
	}

	//end code;	
	//
	if (Npc[nIdx].m_HorseType >= 0) 
	{
		Npc[nIdx].m_bRideHorse = TRUE;
	}
	else
	{
		Npc[nIdx].m_bRideHorse = FALSE;
	}
	

	//
	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_btPKFlag		= 1;
	else if(pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_btPKFlag		= 2;
	else
		Npc[nIdx].m_btPKFlag		= 0;
	
	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;
	else
		Npc[nIdx].m_FightMode		= 0;
	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);
	if (pPlaySync->m_btSomeFlag & 0x08)
		Npc[nIdx].m_nTongFlag		= 1;
	else
		Npc[nIdx].m_nTongFlag		= 0;
}

void KProtocolProcess::SyncScriptAction(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pMsg);
}

void KProtocolProcess::SyncWorld(BYTE* pMsg)
{
	WORLD_SYNC* WorldSync = (WORLD_SYNC *)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].ExecuteStand();
	if (SubWorld[0].m_SubWorldID != WorldSync->SubWorld)
	{
		SubWorld[0].LoadMap(WorldSync->SubWorld, WorldSync->Region);
		g_JXPathFinder.Init(g_ScenePlace.GetLittleMap()->GetRect(), g_ScenePlace.GetKScenePlaceMapC()); // find way by kinnox;
	}
		
	if (SubWorld[0].m_dwCurrentTime > WorldSync->Frame)
	{
	
	}
	else if (SubWorld[0].m_dwCurrentTime < WorldSync->Frame)
	{
	
	}
	
	SubWorld[0].m_dwCurrentTime = WorldSync->Frame;

	SubWorld[0].m_nWeather = WorldSync->Weather;
	g_ScenePlace.ChangeWeather(WorldSync->Weather);

	

	//CoreDataChanged(GDCNI_REFRESH_MINIMAP, NULL, NULL);
	
}


//	���ܣ��յ���������Ϣĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ

void	KProtocolProcess::s2cChatDeleteFriend(BYTE* pMsg)
{
	CHAT_DELETE_FRIEND_SYNC	*pDelete = (CHAT_DELETE_FRIEND_SYNC*)pMsg;

	CChatFriend	*pFriend;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		pFriend = (CChatFriend*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == pDelete->m_dwID)
			{
				pFriend->Remove();
				Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_nFriendNo--;

				// ֪ͨ���� ĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ
//				CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
				CoreDataChanged(GDCNI_CHAT_FRIEND, 0, i);

				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_DELETED_FRIEND, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

				delete pFriend;
				pFriend = NULL; //them code 16/09/2023 by kinnox;
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}
}


//	���ܣ��յ�������֪ͨĳ�����������

void	KProtocolProcess::s2cChatFriendOffLine(BYTE* pMsg)
{
	CHAT_FRIEND_OFFLINE_SYNC	*pSync = (CHAT_FRIEND_OFFLINE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cChat.GetMsgOffLine(pSync->m_dwID);
}
//Kenny fix chuyen map mat skill phai
/*
void	KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)
{
	SKILL_SEND_ALL_SYNC	* pSync = (SKILL_SEND_ALL_SYNC*) pMsg;
	int nSkillCount = (pSync->m_wProtocolLong - 2) / sizeof(SKILL_SEND_ALL_SYNC_DATA);
	int nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	KSkill * pOrdinSkill = NULL;
	Npc[nNpcIndex].m_SkillList.Clear();
	for (int i = 0; i < nSkillCount; i ++)
	{
		if (pSync->m_sAllSkill[i].SkillId)
		{
			
//			pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pSync->m_sAllSkill[i].SkillId, pSync->m_sAllSkill[i].SkillLevel);
//			if (!pOrdinSkill) 
//              continue;

			Npc[nNpcIndex].m_SkillList.Add(
				pSync->m_sAllSkill[i].SkillId,
				pSync->m_sAllSkill[i].SkillLevel
            );
		}
	}

}*/

void KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)
{
    SKILL_SEND_ALL_SYNC * pSync = (SKILL_SEND_ALL_SYNC*) pMsg;
    int nSkillCount = (pSync->m_wProtocolLong - 2) / sizeof(SKILL_SEND_ALL_SYNC_DATA);
    int nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
    KSkill * pOrdinSkill = NULL;
    Npc[nNpcIndex].m_SkillList.Clear();
    for (int i = 0; i < nSkillCount; i++)
    {
        if (pSync->m_sAllSkill[i].SkillId)
        {
            Npc[nNpcIndex].m_SkillList.Add(
                pSync->m_sAllSkill[i].SkillId,
                pSync->m_sAllSkill[i].SkillLevel
            );
        }
    }

     // 2) Re-apply equipment bonuses to restore +skill from items
    Player[CLIENT_PLAYER_INDEX].UpdataCurData();
    // 3) Notify UI to refresh skill display
#ifndef _SERVER
    CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[CLIENT_PLAYER_INDEX].m_nSkillPoint);
#endif
    // 4) Re-apply aura/right skill
#ifndef _SERVER
    Player[CLIENT_PLAYER_INDEX].ForceReapplyRightAura();
	int rightId = Player[CLIENT_PLAYER_INDEX].GetRightSkillID();
	if (rightId > 0) {
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ActiveAuraID = rightId;
	}

#endif
}
void KProtocolProcess::SyncEnd(BYTE* pMsg)
{
	BYTE	SyncEnd = (BYTE)c2s_syncend;
	if (g_pClient)
		g_pClient->SendPackToServer(&SyncEnd, sizeof(BYTE));
	Player[CLIENT_PLAYER_INDEX].SetDefaultImmedSkill();
#ifdef _DEBUG
	//g_DebugLog("[TRACE]SyncEnd to Server");
#endif
	CoreDataChanged(GDCNI_GAME_START, 0, 0);	
//	g_bPingReply = TRUE;
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTeamName(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTakeChannelInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_LOGIN);
	g_SubWorldSet.m_cMusic.Start(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);
	Player[CLIENT_PLAYER_INDEX].m_cPK.Init();


}

void	KProtocolProcess::s2cTradeChangeState(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeChangeState(pMsg);
}

void	KProtocolProcess::s2cNpcSetMenuState(BYTE* pMsg)
{
	NPC_SET_MENU_STATE_SYNC	*pState = (NPC_SET_MENU_STATE_SYNC*)pMsg;
	int		nNpc;
	if (pState->m_dwID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{
		nNpc = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (pState->m_btState == PLAYER_MENU_STATE_TRADEOPEN)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_STATE_OPEN);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		else if (pState->m_btState == PLAYER_MENU_STATE_NORMAL)
		{
			if (Npc[nNpc].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_TRADE_STATE_CLOSE);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
	}
	else
	{
		nNpc = NpcSet.SearchID(pState->m_dwID);
		if (nNpc == 0)
			return;
	}
	if (pState->m_wLength > sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence))
		Npc[nNpc].SetMenuState(pState->m_btState, pState->m_szSentence, pState->m_wLength - (sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence)));
	else
		Npc[nNpc].SetMenuState(pState->m_btState);
}

void	KProtocolProcess::s2cTradeMoneySync(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeMoneySync(pMsg);
}

void	KProtocolProcess::s2cTradeDecision(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeDecision(pMsg);
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

void	KProtocolProcess::s2cUpdateNpcState(BYTE * pMsg)
{
	NPC_SYNC_STATEINFO * pInfo = (NPC_SYNC_STATEINFO*) pMsg;
	int nIndex = NpcSet.SearchID(pInfo->m_ID);	
	
	if (nIndex > 0 && !Npc[nIndex].IsPlayer() && Npc[nIndex].m_RegionIndex >= 0)
	{
		Npc[nIndex].SetNpcState(pInfo->m_btStateInfo);
	}
}

//Kenny Fix Skill
/*
void KProtocolProcess::s2cUpdateNpcState(BYTE* pMsg)
{
    NPC_SYNC_STATEINFO* pInfo = (NPC_SYNC_STATEINFO*)pMsg;

    int nIndex = NpcSet.SearchID(pInfo->m_ID);
    if (nIndex <= 0)
        return;

    if (Npc[nIndex].m_RegionIndex < 0)
        return;

    if (Npc[nIndex].IsPlayer())
    {
        if (nIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
            return;
    }

    if (memcmp(Npc[nIndex].m_btStateInfo, pInfo->m_btStateInfo, MAX_NPC_RECORDER_STATE) == 0)
        return;
    memcpy(Npc[nIndex].m_btStateInfo, pInfo->m_btStateInfo, MAX_NPC_RECORDER_STATE);
    Npc[nIndex].SetNpcState(Npc[nIndex].m_btStateInfo);
}
*/

void	KProtocolProcess::s2cTeamInviteAdd(BYTE * pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReceiveInvite((TEAM_INVITE_ADD_SYNC*)pMsg);
}

void	KProtocolProcess::s2cTradePressOkSync(BYTE * pMsg)
{
	TRADE_STATE_SYNC	*pPress = (TRADE_STATE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTrade.SetTradeState(pPress->m_btSelfLock, pPress->m_btDestLock, pPress->m_btSelfOk, pPress->m_btDestOk);
}

void	KProtocolProcess::s2cDirectlyCastSkill(BYTE * pMsg)
{
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel;
	DWORD	MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	
	if (MapX == -1)
	{
		if (MapY < 0 ) return;
		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)	return;
		if (Npc[MapY].m_RegionIndex < 0)
			return;
		int nX, nY;
		nX = Npc[MapY].m_MapX;
		nY = Npc[MapY].m_MapY;
		SubWorld[0].Map2Mps(Npc[MapY].m_RegionIndex, nX, nY, 0, 0, (int *)&MapX, (int *)&MapY);
	}
	
	int nIdx = NpcSet.SearchID(dwNpcID);
	
///	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		_ASSERT (nSkillID > 0 && nSkillLevel > 0);
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID, nSkillLevel);
		if (!pOrdinSkill) 
            return ;
		
        pOrdinSkill->Cast(nIdx, MapX, MapY);
	}

}

void	KProtocolProcess::s2cShowMsg(BYTE *pMsg)
{
	if (!pMsg)
		return;
	SHOW_MSG_SYNC	*pShowMsg = (SHOW_MSG_SYNC*)pMsg;

	switch (pShowMsg->m_wMsgID)
	{
	case enumMSG_ID_TEAM_KICK_One:
		{
			char	szName[32];
			KSystemMessage	sMsg;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));
			if (strcmp(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName) == 0)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_BE_KICKEN);
				sMsg.eType = SMT_TEAM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_KICK_ONE, szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
			}
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_DISMISS:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_DISMISS_CAPTAIN);
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_DISMISS_MEMBER, g_Team[0].m_szMemName[0]);
			}
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_LEAVE:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == dwID)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_LEAVE_SELF_MSG, g_Team[0].m_szMemName[0]);
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_LEAVE, "����");
				for (int i = 0; i < MAX_TEAM_MEMBER; i++)
				{
					if ((DWORD)g_Team[0].m_nMember[i] == dwID)
					{
						sprintf(sMsg.szMessage, MSG_TEAM_LEAVE, g_Team[0].m_szMemName[i + 1]);
						break;
					}
				}
			}
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_REFUSE_INVITE:
		{
			char	szName[32];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_REFUSE_INVITE, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_SELF_ADD:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_SELF_ADD, g_Team[0].m_szMemName[0]);
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_UI_TEAM;
			sMsg.byPriority = 3;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL2, g_Team[0].m_szMemName[nMember + 1]);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL3);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_OBJ_CANNOT_PICKUP:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_OBJ_CANNOT_PICKUP, 
															strlen(MSG_OBJ_CANNOT_PICKUP), TRUE);
		}
		break;
	case enumMSG_ID_OBJ_TOO_FAR:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_OBJ_TOO_FAR, 
															strlen(MSG_OBJ_TOO_FAR), TRUE);
		}
		break;
	case enumMSG_ID_DEC_MONEY:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_DEC_MONEY, (int)pShowMsg->m_lpBuf);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_SELF_ROOM_FULL:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_TRADE_SELF_ROOM_FULL, 
															strlen(MSG_TRADE_SELF_ROOM_FULL), TRUE);
		}
		break;
	case enumMSG_ID_TRADE_DEST_ROOM_FULL:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_DEST_ROOM_FULL, Player[CLIENT_PLAYER_INDEX].m_cTrade.m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_REFUSE_APPLY:
		{
			int	nIdx = NpcSet.SearchID(*((DWORD*)&pShowMsg->m_lpBuf));
			if (nIdx <= 0)
				return;
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_REFUSE_APPLY, Npc[nIdx].Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_TASK_ITEM:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_TRADE_TASK_ITEM, 
															strlen(MSG_TRADE_TASK_ITEM), TRUE);
		}
		break;
	case enumMSG_ID_ITEM_DAMAGED:
		{
			int nItemID = (int)pShowMsg->m_lpBuf;
			int nIdx = ItemSet.SearchID(nItemID);
			if (!nIdx)
				break;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_ITEM_DAMAGED, Item[nIdx].GetName());
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_GET_ITEM:
		{
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			
			int nItemIdx = ItemSet.SearchID(dwID);
			if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
				break;

			char	szName[128];
			KSystemMessage	sMsg;

			strcpy(szName, Item[nItemIdx].GetName());

			if (strlen(szName) >= sizeof(sMsg.szMessage) - strlen(MSG_ADD_ITEM))
				break;
			
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_ADD_ITEM, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_MONEY_CANNOT_PICKUP:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_MONEY_CANNOT_PICKUP, 
															strlen(MSG_MONEY_CANNOT_PICKUP), TRUE);
		}
		break;
	case enumMSG_ID_CANNOT_ADD_TEAM:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "T� ��i", MSG_TEAM_CANNOT_CREATE, 
															strlen(MSG_TEAM_CANNOT_CREATE), TRUE);
		}
		break;
	case enumMSG_ID_TARGET_CANNOT_ADD_TEAM:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "T� ��i", MSG_TEAM_TARGET_CANNOT_ADD_TEAM, 
															strlen(MSG_TEAM_TARGET_CANNOT_ADD_TEAM), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_1:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_1, 
															strlen(MSG_PK_ERROR_1), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_2:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_2, 
															strlen(MSG_PK_ERROR_2), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_3:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_3, 
															strlen(MSG_PK_ERROR_3), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_4:
		{	
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_4, 
															strlen(MSG_PK_ERROR_4), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_5:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_5, 
															strlen(MSG_PK_ERROR_5), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_6:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_6, 
															strlen(MSG_PK_ERROR_6), TRUE);
		}
		break;
	case enumMSG_ID_PK_ERROR_7:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "PK", MSG_PK_ERROR_7, 
															strlen(MSG_PK_ERROR_7), TRUE);

		}
		break;
	case enumMSG_ID_DEATH_LOSE_ITEM:
		{
			char	szName[32];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_DEATH_LOSE_ITEM, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_REFUSE_ADD:
		{
			char	szName[32];

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_REFUSE_ADD, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_BE_KICK:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Bang h�i", MSG_TONG_BE_KICKED, 
															strlen(MSG_TONG_BE_KICKED), TRUE);
			
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_KICKOUT;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_SUCCESS:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Bang h�i", MSG_TONG_LEAVE_SUCCESS, 
															strlen(MSG_TONG_LEAVE_SUCCESS), TRUE);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_FAIL:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Bang h�i", MSG_TONG_LEAVE_FAIL, 
															strlen(MSG_TONG_LEAVE_FAIL), TRUE);
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MASTER:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Bang h�i", MSG_TONG_CHANGE_AS_MASTER, 
															strlen(MSG_TONG_CHANGE_AS_MASTER), TRUE);


			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_szMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MEMBER:
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Bang h�i", MSG_TONG_CHANGE_AS_MEMBER, 
															strlen(MSG_TONG_CHANGE_AS_MEMBER), TRUE);

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_szMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;
	case enumMSG_ID_CREATE_LOCK_SUCCESS://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_CREATE_LOCK_SUCCESS, 
															strlen(MSG_CREATE_LOCK_SUCCESS), TRUE);
		}
		break;
	case enumMSG_ID_UN_LOCK_SUCCESS://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_UN_LOCK_SUCCESS, 
															strlen(MSG_UN_LOCK_SUCCESS), TRUE);
		}
		break;
	case enumMSG_ID_UN_LOCK_FAIL://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_UN_LOCK_FAIL, 
															strlen(MSG_UN_LOCK_FAIL), TRUE);
		}
		break;
	case enumMSG_ID_CHANGE_LOCK_SUCCESS://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_CHANGE_LOCK_SUCCESS, 
															strlen(MSG_CHANGE_LOCK_SUCCESS), TRUE);
		}
		break;
	case enumMSG_ID_CHANGE_LOCK_FAIL://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_CHANGE_LOCK_FAIL, 
															strlen(MSG_CHANGE_LOCK_FAIL), TRUE);
		}
		break;
	case enumMSG_ID_ACTIVE_LOCK://LockPlayer by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "Kh�a nh�n v�t", MSG_ACTIVE_LOCK, 
															strlen(MSG_ACTIVE_LOCK), TRUE);
		}
		break;
	case enumMSG_ID_OFFLINE_MARKET://Lixian by kinnox;
		{
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H� th�ng", MSG_OFFLINE_MARKET, 
															strlen(MSG_OFFLINE_MARKET), TRUE);
		}
		break;
	case enumMSG_ID_LIXIAN: //Lixian by kinnox;
		{
			if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szShopAdv[0])
			{
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
				SendClientCmdSit(TRUE);
			}
			CoreDataChanged(GDCNI_LIXIAN_RESULT,0, 0);
		}
		break;
	case enumMSG_ID_TREMBLE://TrembleItem by kinnox;
		{
			CoreDataChanged(GDCNI_OPEN_TREMBLE, 0, 0);
		}
		break;
	case enumMSG_ID_UPGRADE_ATTRIB://UpgradeAttrib - Upgrade equipment attributes
		{
			CoreDataChanged(GDCNI_OPEN_UPGRADE_ATTRIB, 0, 0);
		}
		break;
	default:
		break;
	}
}
// kenny fix skill 
/*
void	KProtocolProcess::SyncStateEffect(BYTE* pMsg)
{
	STATE_EFFECT_SYNC*	pSync = (STATE_EFFECT_SYNC *)pMsg;
	
	if(pSync->m_nLevel > 0)
	{
		int nDataNum = MAX_SKILL_STATE - (sizeof(STATE_EFFECT_SYNC) - pSync->m_wLength) / sizeof(KMagicAttrib);
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetStateSkillEffect(Player[CLIENT_PLAYER_INDEX].m_nIndex, pSync->m_dwSkillID, pSync->m_nLevel, 
																	pSync->m_MagicAttrib, nDataNum, pSync->m_nTime, pSync->m_bRemove);		//
	}
	else
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetTitleEffect(pSync->m_dwSkillID);
	}
}*/

void KProtocolProcess::SyncStateEffect(BYTE* pMsg)
{
    STATE_EFFECT_SYNC* pSync = (STATE_EFFECT_SYNC*)pMsg;

    if (pSync->m_nLevel <= 0)
    {
        KNpc* pNpc = NpcSet.SearchNpcById(pSync->m_dwNpcId);
        if (pNpc)
            pNpc->SetTitleEffect(pSync->m_dwSkillID);
        return;
    }

    int nDataNum = MAX_SKILL_STATE -
        (sizeof(STATE_EFFECT_SYNC) - pSync->m_wLength) / sizeof(KMagicAttrib);

    KNpc* pNpc = NpcSet.SearchNpcById(pSync->m_dwNpcId);
    if (!pNpc) return;

    pNpc->SetStateSkillEffect(
        Player[CLIENT_PLAYER_INDEX].m_nIndex,   
        pSync->m_dwSkillID,
        pSync->m_nLevel,
        pSync->m_MagicAttrib,
        nDataNum,
        pSync->m_nTime,
        pSync->m_bRemove);
}

void	KProtocolProcess::s2cTradeApplyStart(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeApplyStart(pMsg);
}

void	KProtocolProcess::s2cItemAutoMove(BYTE* pMsg)
{
	ITEM_AUTO_MOVE_SYNC	*pSync = (ITEM_AUTO_MOVE_SYNC*)pMsg;
	ItemPos	sSrc, sDest;
	sSrc.nPlace = pSync->m_btSrcPos;
	sSrc.nX = pSync->m_btSrcX;
	sSrc.nY = pSync->m_btSrcY;
	sDest.nPlace = pSync->m_btDestPos;
	sDest.nX = pSync->m_btDestX;
	sDest.nY = pSync->m_btDestY;
	
	Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoMoveItem(sSrc, sDest);
}

void KProtocolProcess::FinishedItemExchange(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

void KProtocolProcess::s2cExtend(BYTE* pMsg)
{
	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pMsg + sizeof(tagExtendProtoHeader));

	if (pHeader->ProtocolFamily == pf_playercommunity)
	{
		if (pHeader->ProtocolID == playercomm_s2c_notifychannelid)
		{
			PLAYERCOMM_NOTIFYCHANNELID* pNChann = (PLAYERCOMM_NOTIFYCHANNELID*)pHeader;

			l_pDataChangedNotifyFunc->NotifyChannelID(pNChann->channel, pNChann->channelid, pNChann->cost);
		}
	}
}

void KProtocolProcess::s2cExtendChat(BYTE* pMsg)
{
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;
	BYTE protocol = *(BYTE*)(pExPckg);

	if (protocol == chat_someonechat)
	{
		CHAT_SOMEONECHAT_SYNC* pCscSync = (CHAT_SOMEONECHAT_SYNC*)pExPckg;

		l_pDataChangedNotifyFunc->MSNMessageArrival(
			pCscSync->someone, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
			(const char*)(pCscSync + 1), pCscSync->sentlen, true);
	}
	else if (protocol == chat_channelchat)
	{
		CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)pExPckg;
	
		l_pDataChangedNotifyFunc->ChannelMessageArrival(
			pCccSync->channelid, pCccSync->someone,
			(const char*)(pCccSync + 1), pCccSync->sentlen, true);
	}
	else if (protocol == chat_feedback)
	{
		////X
		CHAT_FEEDBACK* pCfb = (CHAT_FEEDBACK*)pExPckg;
		DWORD* pChannelid = (DWORD*)(pCfb + 1);

		if (*pChannelid == -1)
		{//someone
			char* pDstName = (char*)(pChannelid + 1);
			BYTE* pSentlen = (BYTE*)(pDstName + _NAME_LEN);
			void* pSent = pSentlen + 1;

			l_pDataChangedNotifyFunc->MSNMessageArrival(
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pDstName,
				(const char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		else
		{//channel
			BYTE* pSentlen = (BYTE*)(pChannelid + 1);
			void* pSent = pSentlen + 1;

			l_pDataChangedNotifyFunc->ChannelMessageArrival(
				*pChannelid, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
				(const char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		////X
	}
}


static BOOL sParseUGName(const std::string& name, std::string* pUnit, std::string* pGroup)
{
	static const char char_split = '\n';

	size_t pos = name.find(char_split);
	if (pos == name.npos)
	{
		if (pUnit)
			pUnit->resize(0);
		if (pGroup)
			pGroup->assign(name);
	}
	else
	{
		std::string::const_iterator itSplit = name.begin() + pos;

		if (pUnit)
			pUnit->assign(name.begin(), itSplit);
		if (pGroup)
			pGroup->assign(itSplit + 1, name.end());
	}

	return TRUE;
}

void KProtocolProcess::s2cExtendFriend(BYTE* pMsg)
{
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;

	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pExPckg);

	if (pHeader->ProtocolFamily == pf_tong)
	{
		_ASSERT(FALSE);
		//tong message
		//...
	}
	else if (pHeader->ProtocolFamily == pf_friend)
	{
		if (pHeader->ProtocolID == friend_c2c_askaddfriend)
		{
			ASK_ADDFRIEND_SYNC* pAafSync = (ASK_ADDFRIEND_SYNC*)pHeader;
			l_pDataChangedNotifyFunc->FriendInvite(pAafSync->srcrole);
		}
		else if (pHeader->ProtocolID == friend_c2c_repaddfriend)
		{
			REP_ADDFRIEND_SYNC* pRafSync = (REP_ADDFRIEND_SYNC*)pHeader;
			l_pDataChangedNotifyFunc->AddFriend(pRafSync->srcrole, pRafSync->answer);
		}
		else if (pHeader->ProtocolID == friend_s2c_repsyncfriendlist)
		{
			REP_SYNCFRIENDLIST* pRsfl = (REP_SYNCFRIENDLIST*)pHeader;
			char* pGroupTag = (char*)(pRsfl + 1);
			if (*pGroupTag != specGroup)
				goto on_error;
			{{
			char* pGroup = (char*)(pGroupTag + 1);
on_newgroup:
			std::string theUnit, theGroup;
			sParseUGName(std::string(pGroup), &theUnit, &theGroup);

			for (char* pRoleTag = pGroup + strlen(pGroup) + 1; ; )
			{
				if (*pRoleTag == specOver)
					goto on_over;
				else if (*pRoleTag == specGroup)
				{
					pGroup = pRoleTag + 1;
					goto on_newgroup;
				}
				else if (*pRoleTag == specRole)
				{
					char* pState = pRoleTag + 1;
					char* pRole = pState + 1;
					l_pDataChangedNotifyFunc->FriendInfo(pRole, (char*)theUnit.c_str(), (char*)theGroup.c_str(), (BYTE)*pState);
					pRoleTag = pRole + strlen(pRole) + 1;
					continue;
				}
				else
					goto on_error;
			}
			}}
on_error:
			_ASSERT(FALSE);
on_over:
			0;
		}
		else if (pHeader->ProtocolID == friend_s2c_friendstate)
		{
			FRIEND_STATE* pFs = (FRIEND_STATE*)pHeader;
			for (char* pRole = (char*)(pFs + 1); *pRole; pRole += strlen(pRole) + 1)
				l_pDataChangedNotifyFunc->FriendStatus(pRole, pFs->state);
		}
		else if (pHeader->ProtocolID == friend_s2c_syncassociate)
		{
			FRIEND_SYNCASSOCIATE* pFsa = (FRIEND_SYNCASSOCIATE*)pHeader;

			char* szGroup = (char*)(pFsa + 1);
			std::string group(szGroup);

			std::string theUnit, theGroup;
			sParseUGName(group, &theUnit, &theGroup);
			_ASSERT(theGroup.empty());

			for (char* szRole = szGroup + group.size() + 1; *szRole; szRole += strlen(szRole) + 1)
				l_pDataChangedNotifyFunc->AddPeople((char*)theUnit.c_str(), szRole);
		}
	}
}


void KProtocolProcess::s2cExtendTong(BYTE* pMsg)
{
	S2C_TONG_HEAD	*pHead = (S2C_TONG_HEAD*)pMsg;
	switch (pHead->m_btMsgId)
	{
	case enumTONG_SYNC_ID_CREATE_FAIL:
		{
			TONG_CREATE_FAIL_SYNC *pFail = (TONG_CREATE_FAIL_SYNC*)pMsg;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, "L�p bang h�i th�t b�i.");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			switch (pFail->m_btFailId)
			{
			case enumTONG_CREATE_ERROR_ID1:
				break;
			case enumTONG_CREATE_ERROR_ID2:	
				break;
			case enumTONG_CREATE_ERROR_ID3:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR09);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID4:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR02);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID5:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR03);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID6:	
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR04);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID7:		
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR05);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID8:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR10);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID9:		
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR08);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID10:
				break;
			case enumTONG_CREATE_ERROR_ID11:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID12:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID13:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID14:
				break;
			case enumTONG_CREATE_ERROR_ID15:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR12);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID16:
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR14);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
		}
		break;
	case enumTONG_SYNC_ID_TRANSFER_ADD_APPLY:
		{
			TONG_APPLY_ADD_SYNC	*pApply = (TONG_APPLY_ADD_SYNC*)pMsg;
			char	szName[32];
			DWORD	dwNameID;
			int		nPlayerIdx;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pApply->m_szName, pApply->m_wLength + 1 + sizeof(pApply->m_szName) - sizeof(TONG_APPLY_ADD_SYNC));
			dwNameID = g_FileName2Id(szName);
			nPlayerIdx = pApply->m_nPlayerIdx;

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, "%s mu�n xin v�o bang h�i!", szName);
			sMsg.eType = SMT_CLIQUE;
			sMsg.byConfirmType = SMCT_UI_TONG_JOIN_APPLY;
			sMsg.byPriority = 3;
			sMsg.byParamSize = sizeof(KUiPlayerItem);
			
			KUiPlayerItem	player;
			strcpy(player.Name, szName);
			player.nIndex = pApply->m_nPlayerIdx;
			player.uId = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&player);

		}
		break;
	case enumTONG_SYNC_ID_ADD:
		{
			TONG_Add_SYNC	*pAdd = (TONG_Add_SYNC*)pMsg;
			char	szName[32], szTitle[32], szMaster[32];

			memcpy(szName, pAdd->m_szTongName,sizeof(szName));
			memcpy(szTitle, pAdd->m_szTitle, sizeof(szTitle));
			memcpy(szMaster, pAdd->m_szMaster, sizeof(szMaster));

			Player[CLIENT_PLAYER_INDEX].m_cTong.AddTong(pAdd->m_btCamp, szName, szTitle, szMaster);

			KUiGameObjectWithName	sUi;
			memset(&sUi, 0, sizeof(sUi));
			strcpy(sUi.szName, szName);
			sUi.nData = TONG_ACTION_APPLY;
			strcpy(sUi.szString, szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;

	case enumTONG_SYNC_ID_HEAD_INFO:
		{
			TONG_HEAD_INFO_SYNC	*pInfo = (TONG_HEAD_INFO_SYNC*)pMsg;

			int nIdx = NpcSet.SearchID(pInfo->m_dwNpcID);
			if (nIdx <= 0)
				break;

			KUiPlayerRelationWithOther	sUi;
			sUi.nIndex = nIdx;
			sUi.uId = Npc[nIdx].m_dwID;
			if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
				sUi.nRelation = Player[CLIENT_PLAYER_INDEX].m_cTong.GetFigure();
			else
				sUi.nRelation = -1;
			sUi.nData = 0;
			sUi.nParam = 0;
			strcpy(sUi.Name, Npc[nIdx].Name);

			KTongInfo	sInfo;
			memset(&sInfo, 0, sizeof(sInfo));
			sInfo.nFaction	= pInfo->m_btCamp;
			sInfo.btLevel	= pInfo->m_btLevel;
			sInfo.nMemberCount = (int)pInfo->m_dwMemberNum;
			sInfo.nManagerCount = (int)pInfo->m_btManagerNum;
			sInfo.nDirectorCount = (int)pInfo->m_btDirectorNum + 1;
			sInfo.nMoney = pInfo->m_dwMoney;
			strcpy(sInfo.szMasterName, pInfo->m_sMember[0].m_szName);
			strcpy(sInfo.szName, pInfo->m_szTongName);

			CoreDataChanged(GDCNI_TONG_INFO, (unsigned int)&sUi, (unsigned int)&sInfo);

			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_DIRECTOR;
			sObj.nParam = 0;
			sObj.uParam = pInfo->m_btDirectorNum + 1;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_MAX_DIRECTOR + 1];		//
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btDirectorNum + 1; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_DIRECTOR;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
				
				if(pInfo->m_bStatus[i])
				{
					sItem[i].bStatus = true;
				}
				else
				{
					sItem[i].bStatus = false;
				}
				
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_SELF_INFO:
		{
			TONG_SELF_INFO_SYNC	*pInfo = (TONG_SELF_INFO_SYNC*)pMsg;
			Player[CLIENT_PLAYER_INDEX].m_cTong.SetSelfInfo(pInfo);
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;
	case enumTONG_SYNC_ID_MANAGER_INFO:
		{
			TONG_MANAGER_INFO_SYNC	*pInfo = (TONG_MANAGER_INFO_SYNC*)pMsg;

			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_MANAGER;
			sObj.nParam = pInfo->m_btStateNo;
			sObj.uParam = pInfo->m_btCurNum;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MANAGER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
				
				if(pInfo->m_bStatus[i])
				{
					sItem[i].bStatus = true;
				}
				else
				{
					sItem[i].bStatus = false;
				}
				
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_MEMBER_INFO:
		{
			TONG_MEMBER_INFO_SYNC	*pInfo = (TONG_MEMBER_INFO_SYNC*)pMsg;

			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_MEMBER;
			sObj.nParam = pInfo->m_btStateNo;
			sObj.uParam = pInfo->m_btCurNum;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MEMBER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
				if(pInfo->m_bStatus[i])
				{
					sItem[i].bStatus = true;
				}
				else
				{
					sItem[i].bStatus = false;
				}
				
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_INSTATE:
		{
			TONG_INSTATE_SYNC	*pInstate = (TONG_INSTATE_SYNC*)pMsg;
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pInstate->m_szName);
			sUi.nData = TONG_ACTION_ASSIGN;
			sUi.nParam = pInstate->m_btNewFigure;
			sUi.uParam = pInstate->m_btOldFigure;
			strcpy(sUi.szString, pInstate->m_szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, pInstate->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_KICK:
		{
			TONG_KICK_SYNC	*pKick = (TONG_KICK_SYNC*)pMsg;

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pKick->m_szName);
			sUi.nData = TONG_ACTION_KICKOUT;
			sUi.nParam = pKick->m_btFigure;
			sUi.uParam = pKick->m_btPos;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, pKick->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_CHANGE_MASTER_FAIL:
		{
			TONG_CHANGE_MASTER_FAIL_SYNC	*pFail = (TONG_CHANGE_MASTER_FAIL_SYNC*)pMsg;

			switch (pFail->m_btFailID)
			{
			case 0:		// �Է�������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��Է������ߣ�");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;
			case 1:		// �Է�����������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��Է�����������");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;
			case 2:		// ���ֲ��ԣ�
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��������޴��ˣ�");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}


void KProtocolProcess::s2cChangeWeather(BYTE* pMsg)
{
	g_ScenePlace.ChangeWeather(((SYNC_WEATHER*)pMsg)->WeatherID);
}

void	KProtocolProcess::s2cPKSyncNormalFlag(BYTE* pMsg)
{
	PK_NORMAL_FLAG_SYNC	*pFlag = (PK_NORMAL_FLAG_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetNormalPKState(pFlag->m_btFlag);
}

void	KProtocolProcess::s2cPKSyncEnmityState(BYTE* pMsg)
{
	PK_ENMITY_STATE_SYNC	*pState = (PK_ENMITY_STATE_SYNC*)pMsg;
	char	szName[32];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_ENMITY_STATE_SYNC));
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetEnmityPKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKSyncExerciseState(BYTE* pMsg)
{
	PK_EXERCISE_STATE_SYNC	*pState = (PK_EXERCISE_STATE_SYNC*)pMsg;
	char	szName[32];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_EXERCISE_STATE_SYNC));
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetExercisePKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKValueSync(BYTE* pMsg)
{
	PK_VALUE_SYNC	*pValue = (PK_VALUE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetPKValue(pValue->m_cPKValue);
}

void	KProtocolProcess::s2cViewEquip(BYTE* pMsg)
{
	g_cViewItem.GetData(pMsg);
}

void	KProtocolProcess::s2cTongCreate(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cTong.Create((TONG_CREATE_SYNC*)pMsg);
}

void	KProtocolProcess::s2cNpcGoldChange(BYTE* pMsg)
{
	NPC_GOLD_CHANGE_SYNC	*pSync = (NPC_GOLD_CHANGE_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->m_dwNpcID);

	if (nIdx && Npc[nIdx].m_Kind == kind_normal)
	{
		Npc[nIdx].m_cGold.SetGoldCurrentType((int)pSync->m_wGoldFlag);
	}
}

// --

// --
void	KProtocolProcess::TaskValueSync(BYTE* pMsg)
{
	TASK_VALUE_SYNC* pValue = (TASK_VALUE_SYNC*)pMsg;
	
	switch(pValue->m_bClear)
	{
	case 0:
		Player[CLIENT_PLAYER_INDEX].TaskSetSaveVal(pValue->m_nId, pValue->m_bValue);
		break;
	case 1:
		Player[CLIENT_PLAYER_INDEX].TaskSetClearVal(pValue->m_nId, pValue->m_bValue);
		break;
	case 2:
		// Player[CLIENT_PLAYER_INDEX].SetBronzeCoin(pValue->m_bValue);
		break;
	case 3:
		// Player[CLIENT_PLAYER_INDEX].SetLockBronzeCoin(pValue->m_bValue);
		break;
	default:
		break;
	}
}
	
//-

//-
void    KProtocolProcess::s2cFindPathSync(BYTE * pMsg)
{
	FIND_PATH_SYNC* pSyncData = (FIND_PATH_SYNC*)pMsg;
	
	int nNpcIdx = NpcSet.SearchID(pSyncData->m_dwID);
	if(nNpcIdx > 0)
	{
		Npc[nNpcIdx].SetPos(pSyncData->m_nPosX, pSyncData->m_nPosY);
		// Kh�ng g?i l?nh do_stand n?u dang ng?i - d? kh�ng l�m gi�n do?n h?i m�u
		if (Npc[nNpcIdx].m_Doing != do_sit) {
			Npc[nNpcIdx].SendCommand(do_stand);
		}
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
	/*FIND_PATH_SYNC *pSync = (FIND_PATH_SYNC*)pMsg;
	
	int nIdx = NpcSet.SearchID(pSync->m_dwID);
	if(nIdx > 0)
	{
		int nRegion, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Mps2Map(pSync->m_nPosX, pSync->m_nPosY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);

		int nRegionX = pSync->m_nPosX / (SubWorld[0].m_nCellWidth * SubWorld[0].m_nRegionWidth);
		int nRegionY = pSync->m_nPosY / (SubWorld[0].m_nCellHeight * SubWorld[0].m_nRegionHeight);
		
		DWORD	dwRegionID = MAKELONG(nRegionX, nRegionY);
		SubWorld[0].LoadMap(SubWorld[0].m_SubWorldID, dwRegionID);

		nRegion = SubWorld[0].FindRegion(dwRegionID);
		_ASSERT(nRegion >= 0);
		Npc[nIdx].m_RegionIndex = nRegion;
		Npc[nIdx].m_dwRegionID = dwRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, nIdx);

		SubWorld[0].Mps2Map(pSync->m_nPosX, pSync->m_nPosY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
		Npc[nIdx].m_MapX = nMapX;
		Npc[nIdx].m_MapY = nMapY;
		Npc[nIdx].m_OffX = pSync->m_nOffX;
		Npc[nIdx].m_OffY = pSync->m_nOffY;
		
		SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);

		Npc[nIdx].SendCommand(do_stand);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}*/
}

//-

//-
void    KProtocolProcess::PlayerStopMove(BYTE * pMsg)
{ 
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit) 
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
	}
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendClientPos2Server();
}

//-

//-
void    KProtocolProcess::s2cPlayerPos(BYTE * pMsg)
{
	PLAYER_POS_SYNC* pSync = (PLAYER_POS_SYNC*)pMsg;
	if(!pSync)
		return;

	int nIndex = NpcSet.SearchID(pSync->m_dwNpcID);
	if(nIndex <= 0 || nIndex >= MAX_NPC)
		return;
	if(Npc[nIndex].m_dwID <= 0)
		return;	
	if(nIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
		return;			
	Npc[nIndex].SendCommand(do_run, pSync->m_nMpsX, pSync->m_nMpsY);
	Npc[nIndex].EditPos(true);
}

//-

//-
void	KProtocolProcess::SyncPlayerRide(BYTE* pMsg)
{
	PLAYER_RIDE_SYNC	*pValue = (PLAYER_RIDE_SYNC*)pMsg;
	if(!pValue->m_bRideFlag)
	{
		if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			return;
	}
	else
	{
		if(!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			return;
	}
	Player[CLIENT_PLAYER_INDEX].CheckRideHorse(pValue->m_bRideFlag);
}

//-

//-
void    KProtocolProcess::NetCommandBloodNo(BYTE * pMsg)
{
		BLOOD_SYNC*	pCommand = (BLOOD_SYNC*)pMsg;
		
		int nIndex = NpcSet.SearchID(pCommand->m_dwNpcID);
		if (nIndex && nIndex != Player[CLIENT_PLAYER_INDEX].m_nIndex && Npc[nIndex].m_dwID > 0)
		{	
			Npc[nIndex].SetBlood(pCommand->m_nBloodNo);
		}
}

//-

//-
void	KProtocolProcess::s2cPlayerRefesh(BYTE* pMsg)
{	
	PLAYER_REFRESH_SYNC* pRefresh = (PLAYER_REFRESH_SYNC*)pMsg;
	if(!pRefresh)
		return;

	Player[CLIENT_PLAYER_INDEX].RefeshPlayer(pRefresh->m_bClear);
}

//-

//-
void	KProtocolProcess::ItemChangeInfo(BYTE* pMsg)
{
	ITEM_CHANGE_INFO	*pICI = (ITEM_CHANGE_INFO *)pMsg;

	int nIdx = ItemSet.SearchID(pICI->m_dwItemID);
	
	if (nIdx)
	{	
		switch(pICI->m_btType)
		{
		case 0:
			{
				Item[nIdx].SetDurability(Item[nIdx].GetDurability() - pICI->m_uChange);

				if(Item[nIdx].GetDurability() <= 3)
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, MSG_ITEM_NEARLY_DAMAGED, Item[nIdx].GetName());
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
			}
			break;
		case 1:
			Item[nIdx].SetStackCount(pICI->m_uChange);
			Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			break;
		case 2:
			Item[nIdx].SetBindState(pICI->m_uChange);
			Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			break;
		case 4:
			Item[nIdx].SetDurability(pICI->m_uChange);
			break;
		case 5:
			Item[nIdx].SetPlayerShopPrice(pICI->m_uChange);
			Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			break;
		case 6:
			Player[CLIENT_PLAYER_INDEX].m_cShop.UpdateData();
			Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			break;
		case 7:
			break;
		default:
			break;
		}
	}
}

//-

//-
void KProtocolProcess::s2cMaskLock(BYTE* pMsg)
{
	NPC_MASK_LOCK_SYNC *pInfo = (NPC_MASK_LOCK_SYNC *)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_ItemList.SetMaskLock(pInfo->m_bFlag);
}

//-

//-
void KProtocolProcess::s2cViewShop(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cShop.GetData(pMsg);
}



//-

//-
void KProtocolProcess::s2cPlayerAutoDoSkill(BYTE* pMsg)
{
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel;
	int		MapX, MapY;
	
	NPC_SKILL_SYNC	*pAuto = (NPC_SKILL_SYNC*)pMsg;

	dwNpcID = pAuto->ID;
	nSkillID = pAuto->nSkillID;
	nSkillLevel = pAuto->nSkillLevel;
	MapX = pAuto->nMpsX;
	MapY = pAuto->nMpsY;
	
	if (MapY < 0)
		return ;
	if (MapX < 0)
	{
		if (MapX != -1)
			return;
	
		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)
			return;

		if (Npc[MapY].m_RegionIndex < 0)
			return;

	}
	
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].AutoDoSkill(nSkillID, nSkillLevel, MapX, MapY);
}

//-

//-
void KProtocolProcess::NetCommandRandMove(BYTE* pMsg)
{
	NPC_RANDMOVE_SYNC	*pCommand = (NPC_RANDMOVE_SYNC*)pMsg;
	int nMapX, nMapY;
	nMapX = pCommand->m_nMpsX;
	nMapY = pCommand->m_nMpsY;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommandRandMove(nMapX, nMapY);
}
//-
//
//-
void	KProtocolProcess::s2cAutoPlaySync(BYTE* pMsg)
{
	S2CPLAYER_REQUEST_AUTO	*pInfo = (S2CPLAYER_REQUEST_AUTO*)pMsg;
	BYTE btAuto = pInfo->m_bAuto;
	BOOL bActive = pInfo->m_bActive;
	if (btAuto == 1)
	{
		if (bActive)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, "<color=green>T� ��ng<color=yellow> k�ch ho�t <pic=136>!");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		else
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, "<color=green>T� ��ng<color=yellow> t�m d�ng <pic=137>!");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);		
		}
		Player[CLIENT_PLAYER_INDEX].SetAutoFlag((BOOL)bActive);
		CoreDataChanged(GDCNI_UPDATE_AUTO, NULL, NULL);
	}
}

void	KProtocolProcess::s2cLockPlayerSync(BYTE* pMsg)//LockPlayer by kinnox;
{
	S2C_LOCKPLAYER	*pSync = (S2C_LOCKPLAYER*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_bLockPlayer = pSync->m_bLockPlayer;
	Player[CLIENT_PLAYER_INDEX].m_bPass = pSync->m_bPass;
		CoreDataChanged(GDCNI_UPDATE_STOREBOX,NULL,NULL);
}
//-
//load 3 storebox by kinnox;
//-
void KProtocolProcess::s2cReposiPlayer(BYTE* pMsg)
{
	REPOSIPLAYER_SYNC	*pCommand = (REPOSIPLAYER_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].SetStoreBoxNum((BYTE)pCommand->m_nNum);
}
//-
//Expandbox by kinnox;
//-
void KProtocolProcess::s2cExpandboxPlayer(BYTE* pMsg)
{
	EXPANDPLAYER_SYNC	*pCommand = (EXPANDPLAYER_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].SetExpandBoxNum((BYTE)pCommand->m_nNum);
}
//-
//GiveBox by kinnox;
//-
void KProtocolProcess::s2cGiveBox(BYTE* pMsg)
{
	S2C_GIVE_BOX	*GiveBoxCmd = (S2C_GIVE_BOX *)pMsg;
	switch(GiveBoxCmd->nType)
	{
	case 1:
		{
			KUiGiveBoxStruc	pInfo;
			strcpy(pInfo.szTitle, GiveBoxCmd->Value); 
			strcpy(pInfo.szInitString, GiveBoxCmd->Value1); 
			strcpy(pInfo.szAction1, GiveBoxCmd->Value2); 
			CoreDataChanged(GDCNI_OPEN_GIVE_BOX, (unsigned int)&pInfo, NULL);
		}
		break;
	case 2:
		CoreDataChanged(GDCNI_END_GIVE_BOX, NULL, NULL);
		break;
	default:
		break;
	}
}

//-
//
//-
void KProtocolProcess::s2cReWardBox(BYTE* pMsg)
{
	S2C_REWARD_BOX	*ReWardBoxCmd = (S2C_REWARD_BOX *)pMsg;
	KUiReWardBox	pInfo;
	strcpy(pInfo.szTitle, ReWardBoxCmd->Value); 
	strcpy(pInfo.szFunction1, ReWardBoxCmd->Value1); 
	strcpy(pInfo.szFunction2, ReWardBoxCmd->Value2); 
	strcpy(pInfo.szFunction3, ReWardBoxCmd->Value3); 
	CoreDataChanged(GDCNI_OPEN_REWARD_BOX, (unsigned int)&pInfo, 0);
}
//-
//
//-
void KProtocolProcess::s2cTimeBox(BYTE* pMsg)
{
	S2C_TIME_BOX	*TimeBoxCmd = (S2C_TIME_BOX *)pMsg;
	KUiTimeBoxInfo	pInfo;
	strcpy(pInfo.szTitle, TimeBoxCmd->Value); 
	pInfo.nTime = TimeBoxCmd->Value1;
	strcpy(pInfo.szAction, TimeBoxCmd->Value2); 
	pInfo.nIndexNpc = TimeBoxCmd->nIndexNpc;
	CoreDataChanged(GDCNI_OPEN_TIME_BOX, (unsigned int)&pInfo, 0);
}
//-
//
//-
void KProtocolProcess::s2cPlayerPointSync(BYTE* pMsg)//Syncpoint by kinnox;
{
	S2C_PLAYER_POINT_SYNC	*pInfo = (S2C_PLAYER_POINT_SYNC *)pMsg;

	switch (pInfo->nType)
	{
	case 0:
		Player[CLIENT_PLAYER_INDEX].m_nAttributePoint += pInfo->nValue;
		break;
	case 1:
		Player[CLIENT_PLAYER_INDEX].m_nSkillPoint += pInfo->nValue;
		break;
	default:
		break;
	}
}
//-
//
//-
void KProtocolProcess::PlayerSendTopTKNew(BYTE* pMsg)
{
	PLAYER_SEND_TOP_TK_NEW* pSync = (PLAYER_SEND_TOP_TK_NEW*)pMsg;
	KUiTopTKNewData pDataTop;
	pDataTop.m_SoluongSong = pSync->m_DataTop.m_SoluongSong;
	pDataTop.m_SoluongJin = pSync->m_DataTop.m_SoluongJin;
	pDataTop.m_ThoiGian = pSync->m_DataTop.m_ThoiGian;
	pDataTop.m_NguoiChoi = pSync->m_DataTop.m_NguoiChoi;
	pDataTop.m_TNguoiChoi = pSync->m_DataTop.m_TNguoiChoi;
	pDataTop.m_NPC = pSync->m_DataTop.m_NPC;
	pDataTop.m_TNPC = pSync->m_DataTop.m_TNPC;
	pDataTop.m_LienTramHT = pSync->m_DataTop.m_LienTramHT;
	pDataTop.m_TLienTramHT = pSync->m_DataTop.m_TLienTramHT;
	pDataTop.m_BauVat = pSync->m_DataTop.m_BauVat;
	pDataTop.m_TBauVat = pSync->m_DataTop.m_TBauVat;
	pDataTop.m_TichLuy = pSync->m_DataTop.m_TichLuy;
	pDataTop.m_TuVong = pSync->m_DataTop.m_TuVong;
	pDataTop.m_LienTram = pSync->m_DataTop.m_LienTram;

	g_StrCpyLen(pDataTop.m_szTop1Name, pSync->m_DataTop.m_szTop1Name, sizeof(pDataTop.m_szTop1Name));
	pDataTop.m_Top1Phe = pSync->m_DataTop.m_Top1Phe;
	pDataTop.m_Top1TichLuy = pSync->m_DataTop.m_Top1TichLuy;
	pDataTop.m_Top1NguoiChoi = pSync->m_DataTop.m_Top1NguoiChoi;
	pDataTop.m_Top1NPC = pSync->m_DataTop.m_Top1NPC;
	pDataTop.m_Top1TuVong = pSync->m_DataTop.m_Top1TuVong;
	pDataTop.m_Top1LienTram = pSync->m_DataTop.m_Top1LienTram;
	pDataTop.m_Top1BaoVat = pSync->m_DataTop.m_Top1BaoVat;

	g_StrCpyLen(pDataTop.m_szTop2Name, pSync->m_DataTop.m_szTop2Name, sizeof(pDataTop.m_szTop2Name));
	pDataTop.m_Top2Phe = pSync->m_DataTop.m_Top2Phe;
	pDataTop.m_Top2TichLuy = pSync->m_DataTop.m_Top2TichLuy;
	pDataTop.m_Top2NguoiChoi = pSync->m_DataTop.m_Top2NguoiChoi;
	pDataTop.m_Top2NPC = pSync->m_DataTop.m_Top2NPC;
	pDataTop.m_Top2TuVong = pSync->m_DataTop.m_Top2TuVong;
	pDataTop.m_Top2LienTram = pSync->m_DataTop.m_Top2LienTram;
	pDataTop.m_Top2BaoVat = pSync->m_DataTop.m_Top2BaoVat;

	g_StrCpyLen(pDataTop.m_szTop3Name, pSync->m_DataTop.m_szTop3Name, sizeof(pDataTop.m_szTop3Name));
	pDataTop.m_Top3Phe = pSync->m_DataTop.m_Top3Phe;
	pDataTop.m_Top3TichLuy = pSync->m_DataTop.m_Top3TichLuy;
	pDataTop.m_Top3NguoiChoi = pSync->m_DataTop.m_Top3NguoiChoi;
	pDataTop.m_Top3NPC = pSync->m_DataTop.m_Top3NPC;
	pDataTop.m_Top3TuVong = pSync->m_DataTop.m_Top3TuVong;
	pDataTop.m_Top3LienTram = pSync->m_DataTop.m_Top3LienTram;
	pDataTop.m_Top3BaoVat = pSync->m_DataTop.m_Top3BaoVat;

	g_StrCpyLen(pDataTop.m_szTop4Name, pSync->m_DataTop.m_szTop4Name, sizeof(pDataTop.m_szTop4Name));
	pDataTop.m_Top4Phe = pSync->m_DataTop.m_Top4Phe;
	pDataTop.m_Top4TichLuy = pSync->m_DataTop.m_Top4TichLuy;
	pDataTop.m_Top4NguoiChoi = pSync->m_DataTop.m_Top4NguoiChoi;
	pDataTop.m_Top4NPC = pSync->m_DataTop.m_Top4NPC;
	pDataTop.m_Top4TuVong = pSync->m_DataTop.m_Top4TuVong;
	pDataTop.m_Top4LienTram = pSync->m_DataTop.m_Top4LienTram;
	pDataTop.m_Top4BaoVat = pSync->m_DataTop.m_Top4BaoVat;

	g_StrCpyLen(pDataTop.m_szTop5Name, pSync->m_DataTop.m_szTop5Name, sizeof(pDataTop.m_szTop5Name));
	pDataTop.m_Top5Phe = pSync->m_DataTop.m_Top5Phe;
	pDataTop.m_Top5TichLuy = pSync->m_DataTop.m_Top5TichLuy;
	pDataTop.m_Top5NguoiChoi = pSync->m_DataTop.m_Top5NguoiChoi;
	pDataTop.m_Top5NPC = pSync->m_DataTop.m_Top5NPC;
	pDataTop.m_Top5TuVong = pSync->m_DataTop.m_Top5TuVong;
	pDataTop.m_Top5LienTram = pSync->m_DataTop.m_Top5LienTram;
	pDataTop.m_Top5BaoVat = pSync->m_DataTop.m_Top5BaoVat;

	g_StrCpyLen(pDataTop.m_szTop6Name, pSync->m_DataTop.m_szTop6Name, sizeof(pDataTop.m_szTop6Name));
	pDataTop.m_Top6Phe = pSync->m_DataTop.m_Top6Phe;
	pDataTop.m_Top6TichLuy = pSync->m_DataTop.m_Top6TichLuy;
	pDataTop.m_Top6NguoiChoi = pSync->m_DataTop.m_Top6NguoiChoi;
	pDataTop.m_Top6NPC = pSync->m_DataTop.m_Top6NPC;
	pDataTop.m_Top6TuVong = pSync->m_DataTop.m_Top6TuVong;
	pDataTop.m_Top6LienTram = pSync->m_DataTop.m_Top6LienTram;
	pDataTop.m_Top6BaoVat = pSync->m_DataTop.m_Top6BaoVat;

	g_StrCpyLen(pDataTop.m_szTop7Name, pSync->m_DataTop.m_szTop7Name, sizeof(pDataTop.m_szTop7Name));
	pDataTop.m_Top7Phe = pSync->m_DataTop.m_Top7Phe;
	pDataTop.m_Top7TichLuy = pSync->m_DataTop.m_Top7TichLuy;
	pDataTop.m_Top7NguoiChoi = pSync->m_DataTop.m_Top7NguoiChoi;
	pDataTop.m_Top7NPC = pSync->m_DataTop.m_Top7NPC;
	pDataTop.m_Top7TuVong = pSync->m_DataTop.m_Top7TuVong;
	pDataTop.m_Top7LienTram = pSync->m_DataTop.m_Top7LienTram;
	pDataTop.m_Top7BaoVat = pSync->m_DataTop.m_Top7BaoVat;

	g_StrCpyLen(pDataTop.m_szTop8Name, pSync->m_DataTop.m_szTop8Name, sizeof(pDataTop.m_szTop8Name));
	pDataTop.m_Top8Phe = pSync->m_DataTop.m_Top8Phe;
	pDataTop.m_Top8TichLuy = pSync->m_DataTop.m_Top8TichLuy;
	pDataTop.m_Top8NguoiChoi = pSync->m_DataTop.m_Top8NguoiChoi;
	pDataTop.m_Top8NPC = pSync->m_DataTop.m_Top8NPC;
	pDataTop.m_Top8TuVong = pSync->m_DataTop.m_Top8TuVong;
	pDataTop.m_Top8LienTram = pSync->m_DataTop.m_Top8LienTram;
	pDataTop.m_Top8BaoVat = pSync->m_DataTop.m_Top8BaoVat;

	g_StrCpyLen(pDataTop.m_szTop9Name, pSync->m_DataTop.m_szTop9Name, sizeof(pDataTop.m_szTop9Name));
	pDataTop.m_Top9Phe = pSync->m_DataTop.m_Top9Phe;
	pDataTop.m_Top9TichLuy = pSync->m_DataTop.m_Top9TichLuy;
	pDataTop.m_Top9NguoiChoi = pSync->m_DataTop.m_Top9NguoiChoi;
	pDataTop.m_Top9NPC = pSync->m_DataTop.m_Top9NPC;
	pDataTop.m_Top9TuVong = pSync->m_DataTop.m_Top9TuVong;
	pDataTop.m_Top9LienTram = pSync->m_DataTop.m_Top9LienTram;
	pDataTop.m_Top9BaoVat = pSync->m_DataTop.m_Top9BaoVat;

	g_StrCpyLen(pDataTop.m_szTop10Name, pSync->m_DataTop.m_szTop10Name, sizeof(pDataTop.m_szTop10Name));
	pDataTop.m_Top10Phe = pSync->m_DataTop.m_Top10Phe;
	pDataTop.m_Top10TichLuy = pSync->m_DataTop.m_Top10TichLuy;
	pDataTop.m_Top10NguoiChoi = pSync->m_DataTop.m_Top10NguoiChoi;
	pDataTop.m_Top10NPC = pSync->m_DataTop.m_Top10NPC;
	pDataTop.m_Top10TuVong = pSync->m_DataTop.m_Top10TuVong;
	pDataTop.m_Top10LienTram = pSync->m_DataTop.m_Top10LienTram;
	pDataTop.m_Top10BaoVat = pSync->m_DataTop.m_Top10BaoVat;
	CoreDataChanged(GDCNI_SEND_TOP_TK_NEW_ITEM, (unsigned int)&pDataTop,0);
}
//-
//
//-
//-
//Notification by kinnox;
//-
void KProtocolProcess::s2cNotification(BYTE* pMsg)
{
	S2C_NOTIFICATION	*NotificationCmd = (S2C_NOTIFICATION *)pMsg;

	KUiNotifiStruc	pInfo;
	strcpy(pInfo.szTitle, NotificationCmd->Value); 
	strcpy(pInfo.szInitString, NotificationCmd->Value1); 
	CoreDataChanged(GDCNI_OPEN_NOTIFICATION, (unsigned int)&pInfo, NULL);
}
//-
//
//-
void KProtocolProcess::LadderList(BYTE* pMsg)
{
	LADDER_LIST*	pList = (LADDER_LIST *)pMsg;
	KRankIndex		*pLadderListIndex = NULL;
	
	if (pList->nCount > 0 && pList->nCount < enumLadderEnd)
	{
		pLadderListIndex = new KRankIndex[pList->nCount];
	}
	if (pLadderListIndex)
	{
		for (int i = 0; i < pList->nCount; i++)
		{
			pLadderListIndex[i].usIndexId = pList->dwLadderID[i];
			pLadderListIndex[i].bValueAppened = true;
			pLadderListIndex[i].bSortFlag = true;
		}
		CoreDataChanged(GDCNII_RANK_INDEX_LIST_ARRIVE, pList->nCount, (int)pLadderListIndex);
		delete [] pLadderListIndex;
		pLadderListIndex = NULL;
	}
}

void KProtocolProcess::LadderResult(BYTE* pMsg)
{
	LADDER_DATA*	pLadderData = (LADDER_DATA *)pMsg;
	KRankMessage*	pLadderMessage = NULL;

	pLadderMessage = new KRankMessage[10];
	if (pLadderMessage)
	{
		for (int i = 0; i < 10; i++)
		{
			pLadderMessage[i].usMsgLen = strlen(pLadderData->StatData[i].Name);
			strcpy(pLadderMessage[i].szMsg, pLadderData->StatData[i].Name);
			pLadderMessage[i].nValueAppend = pLadderData->StatData[i].nValue;
			pLadderMessage[i].cSortFlag = (char)pLadderData->StatData[i].bySort;
			pLadderMessage[i].btTransLife		= pLadderData->StatData[i].TransLife;
		}
		unsigned int uParam = 10 | (((WORD)pLadderData->dwLadderID) << 16);
		CoreDataChanged(GDCNII_RANK_INFORMATION_ARRIVE, uParam, (int)pLadderMessage);
		delete [] pLadderMessage;
		pLadderMessage = NULL;
	}
}
#else

void KProtocolProcess::RemoveRole(int nIndex, BYTE * pProtocol)
{

}

void KProtocolProcess::NpcRequestCommand(int nIndex, BYTE* pProtocol)
{
	NPC_REQUEST_COMMAND *pNpcRequestSync = (NPC_REQUEST_COMMAND *)pProtocol;
	NpcSet.SyncNpc(pNpcRequestSync->ID, Player[nIndex].m_nNetConnectIdx);
}


//	���ܣ��ͻ�����������������ĳ��obj����

void KProtocolProcess::ObjRequestCommand(int nIndex, BYTE* pProtocol)
{
	OBJ_CLIENT_SYNC_ADD	*pObjClientSyncAdd = (OBJ_CLIENT_SYNC_ADD*)pProtocol;
	ObjSet.SyncAdd(pObjClientSyncAdd->m_nID, Player[nIndex].m_nNetConnectIdx);
}

void KProtocolProcess::NpcWalkCommand(int nIndex, BYTE* pProtocol)
{
	NPC_WALK_COMMAND* pNetCommand = (NPC_WALK_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	if (ParamX < 0)
	{
		ParamX = 0;
	}
	if (ParamY < 0)
	{
		ParamY = 0;
	}

	int nNpcIdx = Player[nIndex].m_nIndex;

	// FIX: Khi dang ngoi va nhan walk command -> dung day va di chuyen
	// Client side da fix khong gui background sync khi ngoi
	// Nen day chi la intentional movement (user click)
	if (Npc[nNpcIdx].m_Doing == do_sit)
	{
	// Set truc tiep state va reset tick - khong dung SendCommand vi se bi ghi de
		Npc[nNpcIdx].m_Doing = do_stand;

	}

	Npc[nNpcIdx].SendCommand(do_walk, ParamX, ParamY);
}

void KProtocolProcess::NpcRunCommand(int nIndex, BYTE* pProtocol)
{
	NPC_RUN_COMMAND* pNetCommand = (NPC_RUN_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	if (ParamX < 0)
	{
		ParamX = 0;
	}
	if (ParamY < 0)
	{
		ParamY = 0;
	}

	int nNpcIdx = Player[nIndex].m_nIndex;

// FIX: Khi dang ngoi va nhan run command -> dung day va chay
	// Client side da fix khong gui background sync khi ngoi
	// Nen day chi la intentional movement (user click)
	if (Npc[nNpcIdx].m_Doing == do_sit)
	{
		// Set truc tiep state va reset tick - khong dung SendCommand vi se bi ghi de
		Npc[nNpcIdx].m_Doing = do_stand;

	}

	Npc[nNpcIdx].SendCommand(do_run, ParamX, ParamY);
	}
void KProtocolProcess::NpcSkillCommand(int nIndex, BYTE* pProtocol)
{
NPC_SKILL_COMMAND* pNetCommand = (NPC_SKILL_COMMAND *)pProtocol;
int ParamX = pNetCommand->nSkillID;
int ParamY = pNetCommand->nMpsX;
int ParamZ = pNetCommand->nMpsY;
// �����Ϸ��Լ��
	if (ParamX <= 0 || ParamX > MAX_SKILL )
	{
		return ;
	}

	if (ParamZ < 0) 
		return;

	if (ParamY < 0)
	{
		if (ParamY != -1) 
			return;

		int nNpcIndex = Player[nIndex].FindAroundNpc((DWORD)ParamZ);
		if (nNpcIndex > 0)
		{
			// FIX: Server-side distance validation to prevent desync exploits
			// This ensures player is actually in range before allowing skill
			int nSkillLevel = Npc[Player[nIndex].m_nIndex].m_SkillList.GetLevel(ParamX);
			if (nSkillLevel <= 0)
				nSkillLevel = 1;  // Use level 1 if skill not learned yet
			ISkill* pSkill = g_SkillManager.GetSkill(ParamX, nSkillLevel);
			if (pSkill)
			{
				int nDistance = KNpcSet::GetDistance(Player[nIndex].m_nIndex, nNpcIndex);
				int nAttackRadius = pSkill->GetAttackRadius();
				// Reject skill if target is out of range (with 10% tolerance for lag)
				if (nAttackRadius > 0 && nDistance > nAttackRadius * 1.1)
				{
					// Target too far - reject skill to prevent desync damage
					return;
				}
			}
			Npc[Player[nIndex].m_nIndex].SendCommand(do_skill, ParamX, ParamY, nNpcIndex);
		}
	}
	else
		Npc[Player[nIndex].m_nIndex].SendCommand(do_skill, ParamX, ParamY, ParamZ);
}

void KProtocolProcess::NpcJumpCommand(int nIndex, BYTE* pProtocol)
{
	NPC_JUMP_COMMAND* pNetCommand = (NPC_JUMP_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	Npc[Player[nIndex].m_nIndex].SendCommand(do_jump, ParamX, ParamY);
}

void KProtocolProcess::NpcTalkCommand(int nIndex, BYTE* pProtocol)
{
}

void KProtocolProcess::PlayerTalkCommand(int nIndex, BYTE* pProtocol)
{
	Npc[Player[nIndex].m_nIndex].DoPlayerTalk((char *)pProtocol + 1);
}

void KProtocolProcess::PlayerApplyTeamInfo(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].S2CSendTeamInfo(pProtocol);
}

void KProtocolProcess::PlayerApplyCreateTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].CreateTeam(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamOpenClose(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SetTeamState(pProtocol);
}

void KProtocolProcess::PlayerApplyAddTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].S2CSendAddTeamInfo(pProtocol);
}

void KProtocolProcess::PlayerAcceptTeamMember(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddTeamMember(pProtocol);
}

void KProtocolProcess::PlayerApplyLeaveTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].LeaveTeam(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamKickMember(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamKickOne(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamChangeCaptain(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamChangeCaptain(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamDismiss(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamDismiss(pProtocol);
}

void KProtocolProcess::PlayerApplySetPK(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SetPK(pProtocol);
}

void KProtocolProcess::PlayerApplyFactionData(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SendFactionData(pProtocol);
}

void KProtocolProcess::PlayerSendChat(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ServerSendChat(pProtocol);
}

void KProtocolProcess::PlayerAddBaseAttribute(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddBaseAttribute(pProtocol);
}

void KProtocolProcess::PlayerApplyAddSkillPoint(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddSkillPoint(pProtocol);
}

void KProtocolProcess::PlayerEatItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;
	Player[nIndex].EatItem(pProtocol);
}

void KProtocolProcess::PlayerPickUpItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;
	Player[nIndex].ServerPickUpItem(pProtocol);
		
}

void KProtocolProcess::PlayerRightAutoMove(int nIndex, BYTE* pProtocol)
{
    ITEM_AUTO_MOVE_SYNC* p = (ITEM_AUTO_MOVE_SYNC*)pProtocol;

    if (nIndex <= 0 || nIndex >= MAX_PLAYER)
        return;

    KItemList& itemList = Player[nIndex].m_ItemList;

    int src = p->m_btSrcPos;
    int dst = p->m_btDestPos;

    BOOL bResult = FALSE;

    if (src == pos_equiproom && (dst >= pos_repositoryroom && dst <= pos_repositoryroom5))
    {
        bResult = itemList.AutoMoveToRepository(0, p->m_btSrcX, p->m_btSrcY, p->m_btSrcPos, p->m_btDestPos);
    }
    else if ((src >= pos_repositoryroom && src <= pos_repositoryroom5) && dst == pos_equiproom)
    {
        bResult = itemList.AutoMoveToInventory(0, p->m_btSrcX, p->m_btSrcY, p->m_btSrcPos, p->m_btDestPos);
    }

    if (!bResult)
    {
        //g_DebugLog("[SERVER] PlayerRightAutoMove FAILED: player=%d Src=(%d,%d,%d) Dest=%d",nIndex, p->m_btSrcPos, p->m_btSrcX, p->m_btSrcY, p->m_btDestPos);
    }
}

void KProtocolProcess::PlayerAutoSortEquipment(int nIndex, BYTE* pProtocol)
{
    

    if (nIndex <= 0 || nIndex >= MAX_PLAYER)
        return;

    // Collect all items from equiproom
    struct SortItem {
        int nIdx;
        int nWidth;
        int nHeight;
        int nArea;
    };
    SortItem sortedItems[MAX_EQUIPMENT_ITEM];
    int nItemCount = 0;

    // Gather all items in equipment room and remove them temporarily
    PlayerItem* pItem = Player[nIndex].m_ItemList.GetFirstItem();
    while (pItem && nItemCount < MAX_EQUIPMENT_ITEM)
    {
        if (pItem->nIdx > 0 && pItem->nPlace == pos_equiproom)
        {
            sortedItems[nItemCount].nIdx = pItem->nIdx;
            sortedItems[nItemCount].nWidth = Item[pItem->nIdx].GetWidth();
            sortedItems[nItemCount].nHeight = Item[pItem->nIdx].GetHeight();
            sortedItems[nItemCount].nArea = sortedItems[nItemCount].nWidth * sortedItems[nItemCount].nHeight;
            nItemCount++;
        }
        pItem = Player[nIndex].m_ItemList.GetNextItem();
    }

    // Remove all items from ItemList and inventory grid
    for (int i = 0; i < nItemCount; i++)
    {
        Player[nIndex].m_ItemList.Remove(sortedItems[i].nIdx);
    }

    // Sort by area (smaller items first: 1x1, then 2x2, 2x3, etc.)
    for (i = 0; i < nItemCount - 1; i++)
    {
        for (int j = i + 1; j < nItemCount; j++)
        {
            if (sortedItems[j].nArea < sortedItems[i].nArea)
            {
                SortItem temp = sortedItems[i];
                sortedItems[i] = sortedItems[j];
                sortedItems[j] = temp;
            }
        }
    }

    // Re-add items in sorted order from top-left
    // Scan horizontally first (left to right), then vertically (top to bottom)
    for (i = 0; i < nItemCount; i++)
    {
        BOOL bPlaced = FALSE;

        // Scan row by row (y first), then column by column (x)
        for (int y = 0; y < EQUIPMENT_ROOM_HEIGHT && !bPlaced; y++)
        {
            for (int x = 0; x < EQUIPMENT_ROOM_WIDTH && !bPlaced; x++)
            {
                if (Player[nIndex].m_ItemList.m_Room[room_equipment].CheckRoom(
                    x, y, sortedItems[i].nWidth, sortedItems[i].nHeight))
                {
                    // Add item at this position
                    Player[nIndex].m_ItemList.Add(sortedItems[i].nIdx, pos_equiproom, x, y);
                    bPlaced = TRUE;
                }
            }
        }
    }
    g_DebugLog("[SERVER] PlayerAutoSortEquipment: player=%d sorted %d items", nIndex, nItemCount);
}
void KProtocolProcess::PlayerMoveItem(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ServerMoveItem(pProtocol);
}

void KProtocolProcess::PlayerSellItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;
	Player[nIndex].SellItem(pProtocol);
}

void KProtocolProcess::PlayerBuyItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;
	Player[nIndex].BuyItem(pProtocol);
}

void KProtocolProcess::PlayerDropItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;
	Player[nIndex].ServerThrowAwayItem(pProtocol);
}

void KProtocolProcess::PlayerSelUI(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ProcessPlayerSelectFromUI(pProtocol);
}

void KProtocolProcess::ChatSetChannel(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatSetTakeChannel(pProtocol);
}

void KProtocolProcess::ChatApplyAddFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatTransmitApplyAddFriend(pProtocol);
}

void KProtocolProcess::ChatAddFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatAddFriend(pProtocol);
}

void KProtocolProcess::ChatRefuseFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatRefuseFriend(pProtocol);
}

void KProtocolProcess::ChatApplyReSendAllFriendName(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatResendAllFriend(pProtocol);
}

void KProtocolProcess::ChatApplySendOneFriendName(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatSendOneFriendData(pProtocol);
}

void KProtocolProcess::ChatDeleteFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatDeleteFriend(pProtocol);
}

void KProtocolProcess::ChatReDeleteFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatRedeleteFriend(pProtocol);
}

void	KProtocolProcess::TradeApplyOpen(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyOpen(pProtocol);
}

void	KProtocolProcess::TradeApplyClose(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyClose(pProtocol);
}

void	KProtocolProcess::TradeApplyStart(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyStart(pProtocol);
}

void	KProtocolProcess::TradeMoveMoney(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeMoveMoney(pProtocol);
}

void	KProtocolProcess::TradeDecision(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeDecision(pProtocol);
}

void	KProtocolProcess::DialogNpc(int nIndex, BYTE * pProtocol)
{
	Player[nIndex].DialogNpc(pProtocol)	;
}

void	KProtocolProcess::TeamInviteAdd(int nIndex, BYTE * pProtocol)
{
	Player[nIndex].m_cTeam.InviteAdd(nIndex, (TEAM_INVITE_ADD_COMMAND*)pProtocol);
}

void	KProtocolProcess::ChangeAuraSkill(int nIndex, BYTE * pProtocol)
{
	SKILL_CHANGEAURASKILL_COMMAND * pCommand = (SKILL_CHANGEAURASKILL_COMMAND*) pProtocol;

	Npc[Player[nIndex].m_nIndex].SetAuraSkill(pCommand->m_nAuraSkill);
}

void	KProtocolProcess::TeamReplyInvite(int nIndex, BYTE * pProtocol)
{
	TEAM_REPLY_INVITE_COMMAND	*pReply = (TEAM_REPLY_INVITE_COMMAND*)pProtocol;
	if (!pProtocol ||
		pReply->m_nIndex <= 0 ||
		pReply->m_nIndex >= MAX_PLAYER ||
		Player[pReply->m_nIndex].m_nIndex <= 0)
		return;
	if (pReply->m_btResult && Player[nIndex].m_cTeam.GetCanTeamFlag() == FALSE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return;
	}
	Player[pReply->m_nIndex].m_cTeam.GetInviteReply(pReply->m_nIndex, nIndex, pReply->m_btResult);
}

//void KProtocolProcess::ReplyPing(int nIndex, BYTE* pProtocol)
//{
//	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
//		return;
//
//	PING_COMMAND PingCmd, *pPingCmd;
//
//	pPingCmd = (PING_COMMAND *)pProtocol;
//
//	PingCmd.ProtocolType = s2c_ping;
//	PingCmd.m_dwTime = pPingCmd->m_dwTime;
//	g_pServer->SendData(Player[nIndex].m_nNetConnectIdx, &PingCmd, sizeof(PING_COMMAND));
//	Player[nIndex].m_uLastPingTime = g_SubWorldSet.GetGameTime();
//}

void KProtocolProcess::NpcSitCommand(int nIndex, BYTE* pProtocol)
{
	NPC_SIT_COMMAND *pSitCmd;

	pSitCmd = (NPC_SIT_COMMAND *)pProtocol;

	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;

	if (pSitCmd->m_btSitFlag)
		Npc[Player[nIndex].m_nIndex].SendCommand(do_sit);
	else
		Npc[Player[nIndex].m_nIndex].SendCommand(do_stand);
//	if (Npc[Player[nIndex].m_nIndex].m_Doing != do_sit)
//		Npc[Player[nIndex].m_nIndex].SendCommand(do_sit);
//	else
//		Npc[Player[nIndex].m_nIndex].SendCommand(do_stand);
}

void KProtocolProcess::ObjMouseClick(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading())
		return;

	int		nSubWorldIdx, nRegionIdx, nObjIdx;
	int		nPlayerX, nPlayerY, nObjX, nObjY;
	OBJ_MOUSE_CLICK_SYNC 	*pObj = (OBJ_MOUSE_CLICK_SYNC*)pProtocol;

	nSubWorldIdx = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
	nRegionIdx = SubWorld[nSubWorldIdx].FindRegion(pObj->m_dwRegionID);
	if (nRegionIdx < 0)
		return;
	nObjIdx = SubWorld[nSubWorldIdx].m_Region[nRegionIdx].FindObject(pObj->m_nObjID);
	if (nObjIdx <= 0)
		return;

	SubWorld[nSubWorldIdx].Map2Mps(
		Npc[Player[nIndex].m_nIndex].m_RegionIndex,
		Npc[Player[nIndex].m_nIndex].m_MapX,
		Npc[Player[nIndex].m_nIndex].m_MapY,
		Npc[Player[nIndex].m_nIndex].m_OffX,
		Npc[Player[nIndex].m_nIndex].m_OffY,
		&nPlayerX,
		&nPlayerY);
	SubWorld[nSubWorldIdx].Map2Mps(
		nRegionIdx,
		Object[nObjIdx].m_nMapX,
		Object[nObjIdx].m_nMapY,
		Object[nObjIdx].m_nOffX,
		Object[nObjIdx].m_nOffY,
		&nObjX,
		&nObjY);
/*
	Obj_Kind_MapObj = 0,		// ��ͼ�������Ҫ���ڵ�ͼ����
	Obj_Kind_Body,				// npc ��ʬ��
	Obj_Kind_Box,				// ����
	Obj_Kind_Item,				// ���ڵ��ϵ�װ��
	Obj_Kind_Money,				// ���ڵ��ϵ�Ǯ
	Obj_Kind_LoopSound,			// ѭ����Ч
	Obj_Kind_RandSound,			// �����Ч
	Obj_Kind_Light,				// ��Դ��3Dģʽ�з���Ķ�����
	Obj_Kind_Door,				// ����
	Obj_Kind_Trap,				// ����
	Obj_Kind_Prop,				// С���ߣ�������
	Obj_Kind_Num,				// �����������
*/
	switch (Object[nObjIdx].m_nKind)
	{
	case Obj_Kind_Box:
		if (g_GetDistance(nPlayerX, nPlayerY, nObjX, nObjY) > defMAX_EXEC_OBJ_SCRIPT_DISTANCE)
			break;
		if (Object[nObjIdx].m_nState == OBJ_BOX_STATE_CLOSE)
			Object[nObjIdx].ExecScript(nIndex);
		break;
	case Obj_Kind_Door:
		break;
	case Obj_Kind_Prop:
		if (g_GetDistance(nPlayerX, nPlayerY, nObjX, nObjY) > defMAX_EXEC_OBJ_SCRIPT_DISTANCE)
			break;
		if (Object[nObjIdx].m_nState == OBJ_PROP_STATE_DISPLAY)
			Object[nObjIdx].ExecScript(nIndex);
		break;
	case Obj_Kind_Task:
		if (g_GetDistance(nPlayerX, nPlayerY, nObjX, nObjY) > defMAX_EXEC_OBJ_SCRIPT_DISTANCE)
			break;
			Object[nObjIdx].ExecScript(nIndex);
		break;
	
	default:
		break;
	
	}
}

void KProtocolProcess::StoreMoneyCommand(int nIndex, BYTE* pProtocol)
{
	STORE_MONEY_COMMAND*	pCommand = (STORE_MONEY_COMMAND *)pProtocol;

	if (pCommand->m_byDir)	// ȡǮ
		Player[nIndex].m_ItemList.ExchangeMoney(room_repository, room_equipment, pCommand->m_dwMoney);
	else					// ��Ǯ
		Player[nIndex].m_ItemList.ExchangeMoney(room_equipment, room_repository, pCommand->m_dwMoney);

}

void KProtocolProcess::NpcReviveCommand(int nIndex, BYTE* pProtocol)
{
//	NPC_REVIVE_COMMAND*		pCommand = (NPC_REVIVE_COMMAND *)pProtocol;

	Player[nIndex].Revive(REMOTE_REVIVE_TYPE);
}

void KProtocolProcess::c2sTradeReplyStart(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].c2sTradeReplyStart(pProtocol);
}

void KProtocolProcess::c2sPKApplyChangeNormalFlag(int nIndex, BYTE* pProtocol)
{
	PK_APPLY_NORMAL_FLAG_COMMAND	*pApply = (PK_APPLY_NORMAL_FLAG_COMMAND*)pProtocol;
	Player[nIndex].m_cPK.SetNormalPKState(pApply->m_btFlag);
}

void KProtocolProcess::c2sPKApplyEnmity(int nIndex, BYTE* pProtocol)
{
	PK_APPLY_ENMITY_COMMAND	*pApply = (PK_APPLY_ENMITY_COMMAND*)pProtocol;
	if (Player[nIndex].m_nIndex && !Npc[Player[nIndex].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return;
	}
	if (pApply->m_dwNpcID <= 0 || pApply->m_dwNpcID >= MAX_NPC)
		return;
	int nNpcIdx = Player[nIndex].FindAroundNpc(pApply->m_dwNpcID);
	if (nNpcIdx <= 0)
		return;
	if (Npc[nNpcIdx].m_Kind != kind_player || Npc[nNpcIdx].GetPlayerIdx() <= 0)
		return;

	Player[nIndex].m_cPK.EnmityPKOpen(Npc[nNpcIdx].GetPlayerIdx());
}

#define		defMAX_VIEW_EQUIP_TIME			30
void	KProtocolProcess::c2sViewEquip(int nIndex, BYTE* pProtocol)
{
	if (g_SubWorldSet.GetGameTime() - Player[nIndex].m_nViewEquipTime < defMAX_VIEW_EQUIP_TIME)
		return;
	Player[nIndex].m_nViewEquipTime = g_SubWorldSet.GetGameTime();

	VIEW_EQUIP_COMMAND	*pView = (VIEW_EQUIP_COMMAND*)pProtocol;
	if (pView->m_dwNpcID == Npc[Player[nIndex].m_nIndex].m_dwID)
		return;
	int nPlayerIdx = Player[nIndex].FindAroundPlayer(pView->m_dwNpcID);
	if (nPlayerIdx <= 0)
		return;
	Player[nPlayerIdx].SendEquipItemInfo(nIndex);
}

//
//
//
void KProtocolProcess::LadderQuery(int nIndex, BYTE* pProtocol)
{
	LADDER_QUERY*	pLQ = (LADDER_QUERY *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		int lnID = Player[nIndex].m_nNetConnectIdx;
		if (lnID >= 0)
		{
			LADDER_DATA	LadderData;
			LadderData.ProtocolType = s2c_ladderresult;
			LadderData.dwLadderID = pLQ->dwLadderID;
			void* pData = (void *)Ladder.GetTopTen(LadderData.dwLadderID);
			if (pData)
			{
				memcpy(LadderData.StatData, pData, sizeof(LadderData.StatData));
				g_pServer->PackDataToClient(lnID, &LadderData, sizeof(LadderData));
			}
		}
	}
}

void KProtocolProcess::ItemRepair(int nIndex, BYTE* pProtocol)
{
	ITEM_REPAIR	*pIR = (ITEM_REPAIR *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		Player[nIndex].RepairItem(pIR->dwItemID);
	}
}


#define MAX_POS_EDIT_OFFSET_X 128
#define MAX_POS_EDIT_OFFSET_Y 128
// --

// --
void KProtocolProcess::c2sPosCommand(int nIndex, BYTE *pProtocol)
{
	PLAYER_POS_COMMAND*	pCommand = (PLAYER_POS_COMMAND*)pProtocol;
	if(!pCommand)
		return;
	
	if(Player[nIndex].GetPlayerID() <= 0)
		return;

	int nNpcIndex = Player[nIndex].m_nIndex;
	
// FIX: Khong xu ly pos sync khi player dang ngoi - de khong gian doan hoi mau
	if (Npc[nNpcIndex].m_Doing == do_sit)
	return;
	int nMpsX = 0;
	int nMpsY = 0;
	Npc[nNpcIndex].GetMpsPos(&nMpsX, &nMpsY);
	nMpsX = abs(nMpsX - pCommand->m_nMpsX);
	nMpsY = abs(nMpsY - pCommand->m_nMpsY);

	if(nMpsX <= MAX_POS_EDIT_OFFSET_X && nMpsY <= MAX_POS_EDIT_OFFSET_Y)
		Npc[nNpcIndex].SendCommand(do_run, pCommand->m_nMpsX, pCommand->m_nMpsY);
}

// --

// --
void KProtocolProcess::PlayerRideCommand(int nIndex, BYTE* pProtocol)
{
	PLAYER_RIDE_COMMAND *pRideCmd = (PLAYER_RIDE_COMMAND *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;
	if(!pRideCmd->m_bRideFlag)
	{
		if(Npc[Player[nIndex].m_nIndex].m_bRideHorse)
			return;
	}
	else
	{
		if(!Npc[Player[nIndex].m_nIndex].m_bRideHorse)
			return;
	}
	if(GetTickCount() - Player[nIndex].m_dwTimeHorse >= 10000)
	{
		Player[nIndex].m_dwTimeHorse = GetTickCount();
		Player[nIndex].CheckRideHorse(pRideCmd->m_bRideFlag);
		return;
	}
}

// --

// --
void KProtocolProcess::ExeScriptCommand(int nIndex, BYTE* pProtocol)
{	
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	Player[nIndex].ExeScriptButton(pProtocol);
}

// --

// --
void KProtocolProcess::ItemBreakCommand(int nIndex, BYTE* pProtocol)
{
	ITEM_BREAK_COMMAND *pItemBreak = (ITEM_BREAK_COMMAND *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	int nIdx = 0;
	nIdx = ItemSet.SearchID(pItemBreak->m_dwItemID);
	
	if(nIdx > 0 && nIdx < MAX_ITEM)
	{
		Player[nIndex].BreakItem(pItemBreak->m_dwItemID, pItemBreak->m_btNum);
	}	
}


// --

// --
void	KProtocolProcess::HideMaskCommand(int nIndex, BYTE* pProtocol)
{	
	HIDE_MASK_COMMAND *pCommand = (HIDE_MASK_COMMAND*)pProtocol;

	Player[nIndex].HideMask(pCommand->m_bHide);
}

// --

// --
void	KProtocolProcess::MarkPriceItemCommand(int nIndex, BYTE* pProtocol)
{	
	if(nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if(Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;

	MARK_PRICE_ITEM_COMMAND *pCommand = (MARK_PRICE_ITEM_COMMAND*)pProtocol;
	
	Player[nIndex].m_ItemList.SetMarkPriceItem(pCommand->m_dwItemID, pCommand->m_nPrice);
}

// --

// --
void	KProtocolProcess::MakeAdvTextCommand(int nIndex, BYTE* pProtocol)
{	
	if(nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if(Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;

	ADV_TEXT_COMMAND *pCommand = (ADV_TEXT_COMMAND*)pProtocol;
	
	Player[nIndex].m_cShop.SetAdvText((const char*)pCommand->m_szAdvText);
}

// --

// --
void	KProtocolProcess::PlayerApplyShopState(int nIndex, BYTE* pProtocol)
{	
	if(nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if(Player[nIndex].CheckTrading())
		return;
	if(Npc[Player[nIndex].m_nIndex].m_FightMode)
		return;
	if(strcmp(SubWorld[Npc[Player[nIndex].m_nIndex].m_SubWorldIndex].m_szMapType, "City") &&
		strcmp(SubWorld[Npc[Player[nIndex].m_nIndex].m_SubWorldIndex].m_szMapType, "Capital"))
		return;

	/*
	if(Player[nIndex].TaskGetSaveVal(TV_OPENSHOP) <= 0)
		return ;
	*/

	PLAYER_SHOP_COMMAND *pCommand = (PLAYER_SHOP_COMMAND*)pProtocol;
	
	if(pCommand->m_bOpen)
	{
		if(Player[nIndex].m_cShop.GetState())
		return;
	}
	
	switch (Player[nIndex].m_cMenuState.m_nState)
	{
		case PLAYER_MENU_STATE_TRADEOPEN:
			Player[nIndex].m_cMenuState.SetState(nIndex, PLAYER_MENU_STATE_NORMAL);
			break;
		default:
			break;
	}

	if (Player[nIndex].m_cTeam.m_nFlag && Player[nIndex].m_cTeam.m_nID >= 0)
	{
		PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
		sLeaveTeam.ProtocolType = c2s_teamapplyleave;
		Player[nIndex].LeaveTeam((BYTE*)&sLeaveTeam);
	}
	Player[nIndex].m_cShop.SetState(pCommand->m_bOpen);
}

// --

// --
void	KProtocolProcess::ViewShopCommand(int nIndex, BYTE* pProtocol)
{	
	if(nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if(Player[nIndex].CheckTrading())
		return;
	if(Npc[Player[nIndex].m_nIndex].m_FightMode)
		return;
	
	VIEW_SHOP_COMMAND *pView = (VIEW_SHOP_COMMAND*)pProtocol;

	if (pView->m_dwNpcID == Npc[Player[nIndex].m_nIndex].m_dwID)
		return;
	int nPlayerIdx = Player[nIndex].FindAroundPlayer(pView->m_dwNpcID);
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;

	Player[nPlayerIdx].SendShopItemInfo(nIndex, pView->m_nDataIndex);

}

// --

// --
void	KProtocolProcess::BuyPlayerShopCommand(int nIndex, BYTE* pProtocol)
{	
	if(nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if(Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;
	if(Npc[Player[nIndex].m_nIndex].m_FightMode)
		return;

	Player[nIndex].BuyPlayerShop(pProtocol);
}

void KProtocolProcess::c2sSetAutoSync(int nIndex, BYTE* pProtocol)
{
	C2SPLAYER_REQUEST_AUTO* pInfo = (C2SPLAYER_REQUEST_AUTO*)pProtocol;
	if (Player[nIndex].CheckTrading())
		return;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (pInfo->m_bAuto == 1)
	{
		Player[nIndex].SetAutoFlag((BOOL)pInfo->m_bActive);
		return;
	}
}

void KProtocolProcess::c2sBackToTown(int nIndex, BYTE* pProtocol)
{
	C2SPLAYER_AI_BACKTOTOWN* pInfo = (C2SPLAYER_AI_BACKTOTOWN*)pProtocol;
	if (Player[nIndex].CheckTrading())
		return;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
		Player[nIndex].BackToTownPortal(pInfo->nIdSubWorld);
	return;
}

void KProtocolProcess::c2sGiveBoxScript(int nIndex, BYTE* pProtocol)
{
	GIVEBOX_SCRIPT *pCommand;
	
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	pCommand = (GIVEBOX_SCRIPT *)pProtocol;
	Player[nIndex].ExecuteScript(Player[nIndex].m_dwGiveBoxId, pCommand->szFunc, "");				
}

void KProtocolProcess::c2sGiveBoxCmd(int nIndex, BYTE* pProtocol)
{
	GIVEBOX_CMD *pCommand = (GIVEBOX_CMD *)pProtocol;
	
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (Player[nIndex].CheckTrading())
		return;	
	if(Player[nIndex].m_cShop.GetState())
		return;	
	Player[nIndex].GiveBoxCmd(pCommand->dwID, pCommand->nX, pCommand->nY);			
}

void KProtocolProcess::c2sCrePwBox(int nIndex, BYTE* pProtocol)//LockPlayer by kinnox;
{
	CREPW_BOX *pCommand;

	pCommand = (CREPW_BOX *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	Player[nIndex].LockPlayerInfo(CREATE_PW,pCommand->m_nPW,0);		
}

void KProtocolProcess::c2sLoginPwBox(int nIndex, BYTE* pProtocol)//LockPlayer by kinnox;
{
	LOGINPW_BOX *pCommand;

	pCommand = (LOGINPW_BOX *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	

	Player[nIndex].LockPlayerInfo(LOGIN_PW,pCommand->m_nPW,0);		
}

void KProtocolProcess::c2sChaPwBox(int nIndex, BYTE* pProtocol)//LockPlayer by kinnox;
{
	CHAPW_BOX *pCommand;

	pCommand = (CHAPW_BOX *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	Player[nIndex].LockPlayerInfo(CHANGE_PW,pCommand->m_nPW,pCommand->m_nPWNew);		
}

void KProtocolProcess::c2sLockPwBox(int nIndex, BYTE* pProtocol)//LockPlayer by kinnox;
{
	C2S_LOCKPLAYER *pCommand;

	pCommand = (C2S_LOCKPLAYER *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
 
	Player[nIndex].LockPlayerInfo(LOCK_PW,pCommand->m_bLockPlayer,0);		
}

void KProtocolProcess::c2sReWardScript(int nIndex, BYTE* pProtocol)
{
	REWARD_SCRIPT *pCommand;
	
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	pCommand = (REWARD_SCRIPT *)pProtocol;
	Player[nIndex].ExecuteScript(Player[nIndex].m_dwRewardId, pCommand->szFunc, "");				
}

void KProtocolProcess::c2sTimeBoxScript(int nIndex, BYTE* pProtocol)
{
	C2S_TIME_BOX *pCommand;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER) 
		return;
	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	pCommand = (C2S_TIME_BOX *)pProtocol;
	Player[nIndex].ExecuteScript(Player[nIndex].m_dwTimeBoxId, pCommand->szFunc, (int)pCommand->nIndexNpc);		

}

void KProtocolProcess::c2sTopBattle(int nIndex, BYTE* pProtocol)
{
	OPEN_TOPBATTLE *pCommand;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	if (Player[nIndex].CheckTrading())
		return;
	if(Player[nIndex].m_cShop.GetState())
		return;	
	pCommand = (OPEN_TOPBATTLE *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		try
		{
			bool bExecuteScriptMistake = true;
			KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\mission\\battles\\updatetop.lua");
			if (pScript)
			{
				int nTopIndex = 0;
				pScript->SafeCallBegin(&nTopIndex);
				if (pScript->CallFunction("OpenTop",0, "d",nIndex));
				{ 
					bExecuteScriptMistake = false;
				}
				pScript->SafeCallEnd(nTopIndex);
			}
		}
		catch(...)
		{
			printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\mission\\battles\\updatetop.lua"));
		}
	}

}

void KProtocolProcess::c2sAvatarPlayer(int nIndex, BYTE* pProtocol)//Player Avatar by kinnox;
{
	AVATAR_PLAYER *pCommand;

	pCommand = (AVATAR_PLAYER *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	Player[nIndex].SetNumImgAvatar(pCommand->m_nID);		
}

void KProtocolProcess::RecoveryBoxCmd(int nIndex, BYTE* pProtocol)
{
	RECOVERY_BOX_CMD *pCmd = (RECOVERY_BOX_CMD *)pProtocol;;
	
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	Player[nIndex].GiveBoxCmd(pCmd->dwID, pCmd->nX, pCmd->nY);			
}

#endif
