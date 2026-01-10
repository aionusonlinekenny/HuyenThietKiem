#ifndef KProtocolProcessH
#define	KProtocolProcessH

#include "KProtocol.h"
class KProtocolProcess
{
private:
#ifndef _SERVER
	void (KProtocolProcess::*ProcessFunc[s2c_end])(BYTE* pMsg);
#else
	void (KProtocolProcess::*ProcessFunc[c2s_end])(int nIndex, BYTE* pMsg);
#endif
public:
	KProtocolProcess();
	~KProtocolProcess();
#ifndef _SERVER
	void ProcessNetMsg(BYTE* pMsg);
#else
	void ProcessNetMsg(int nIndex, BYTE* pMsg);
#endif
private:
#ifdef TOOLVERSION
public:
#endif
#ifndef _SERVER
	void	s2cAccoutLoginResult(BYTE * pMsg);
	void	SyncCurNormalData(BYTE* pMsg);
	void	SyncWorld(BYTE* pMsg);
	void	SyncNpc(BYTE* pMsg);
	void	SyncNpcMin(BYTE* pMsg);
	void	SyncNpcMinPlayer(BYTE* pMsg);
	void	SyncPlayer(BYTE* pMsg);
	void	SyncPlayerMin(BYTE* pMsg);
	void	SyncCurPlayer(BYTE* pMsg);
	void	SyncObjectAdd(BYTE* pMsg);
	void	SyncObjectState(BYTE* pMsg);
	void	SyncObjectDir(BYTE* pMsg);
	void	SyncObjectRemove(BYTE* pMsg);
	void	SyncObjectTrap(BYTE* pMsg);
	void	NetCommandWalk(BYTE* pMsg);
	void	NetCommandRun(BYTE* pMsg);
	void	NetCommandJump(BYTE* pMsg);
	void	NetCommandGlide(BYTE* pMsg);
	void	NetCommandSkill(BYTE* pMsg);
	void	NetCommandHurt(BYTE* pMsg);
	void	NetCommandDeath(BYTE* pMsg);
	void	NetCommandRemoveNpc(BYTE* pMsg);
	void	NetCommandChgCurCamp(BYTE* pMsg);
	void	NetCommandChgCamp(BYTE* pMsg);
	void	NetCommandSit(BYTE* pMsg);
	void	OpenSaleBox(BYTE* pMsg);
	void	OpenStoreBox(BYTE* pMsg);
	void	s2cShowTeamInfo(BYTE* pMsg);
	void	s2cUpdataSelfTeamInfo(BYTE* pMsg);
	void	s2cApplyTeamInfoFalse(BYTE* pMsg);
	void	s2cCreateTeam(BYTE* pMsg);
	void	s2cApplyCreateTeamFalse(BYTE* pMsg);
	void	s2cSetTeamState(BYTE* pMsg);
	void	s2cApplyAddTeam(BYTE* pMsg);
	void	s2cTeamAddMember(BYTE* pMsg);
	void	s2cLeaveTeam(BYTE* pMsg);
	void	s2cTeamChangeCaptain(BYTE* pMsg);
	void	s2cSetFactionData(BYTE* pMsg);
	void	s2cLeaveFaction(BYTE* pMsg);
	void	s2cFactionSkillOpen(BYTE* pMsg);
	void	s2cGetChat(BYTE* pMsg);
	void	s2cGetLeadExp(BYTE* pMsg);
	void	s2cGetTeammateLevel(BYTE* pMsg);
	void	s2cLevelUp(BYTE* pMsg);
	void	s2cGetCurAttribute(BYTE* pMsg);
	void	s2cGetSkillLevel(BYTE* pMsg);
	void	s2cGetSkillRemove(BYTE* pMsg);
	void	s2cSyncAllSkill(BYTE * pMsg);
	void	s2cSyncMoney(BYTE* pMsg);
	void	s2cMoveItem(BYTE* pMsg);
	void	s2cRemoveItem(BYTE* pMsg);
	void	s2cSyncItem(BYTE* pMsg);
	void	SyncScriptAction(BYTE* pMsg);
	void	s2cChatGetApplyAddFriend(BYTE* pMsg);
	void	s2cSyncRoleList(BYTE* pMsg);
	void	s2cChatLoginFriendNoName(BYTE* pMsg);
	void	s2cChatLoginFriendName(BYTE* pMsg);
	void	s2cChatAddOneFriend(BYTE* pMsg);
	void	s2cChatBeRefusedAddFriend(BYTE* pMsg);
	void	s2cChatAddFriend(BYTE* pMsg);
	void	s2cChatAddFriendFail(BYTE* pMsg);
	void	s2cChatFriendOnline(BYTE* pMsg);
	void	s2cChatDeleteFriend(BYTE* pMsg);
	void	s2cChatFriendOffLine(BYTE* pMsg);
	void	SyncEnd(BYTE* pMsg);
	void	s2cTradeChangeState(BYTE* pMsg);
	void	s2cNpcSetMenuState(BYTE* pMsg);
	void	s2cTradeMoneySync(BYTE* pMsg);
	void	s2cTradeDecision(BYTE* pMsg);
	void	s2cPlayerExp(BYTE* pMsg);
	void	s2cChatScreenSingleError(BYTE* pMsg);
	void	s2cUpdateNpcState(BYTE * pMsg);
	void	s2cTeamInviteAdd(BYTE * pMsg);
	void	s2cTradePressOkSync(BYTE * pMsg);
	void	s2cPing(BYTE* pMsg);
	void	s2cDirectlyCastSkill(BYTE * pMsg);
	void	s2cShowMsg(BYTE * pMsg);
	void	SyncStateEffect(BYTE* pMsg);
	void	PlayerRevive(BYTE* pMsg);
	void	RequestNpcFail(BYTE* pMsg);
	void	s2cTradeApplyStart(BYTE* pMsg);
	void	s2cItemAutoMove(BYTE* pMsg);
	void	FinishedItemExchange(BYTE* pMsg);
	void	s2cChangeWeather(BYTE* pMsg);
	void	s2cPKSyncNormalFlag(BYTE* pMsg);
	void	s2cPKSyncEnmityState(BYTE* pMsg);
	void	s2cPKSyncExerciseState(BYTE* pMsg);
	void	s2cPKValueSync(BYTE* pMsg);
	void	NpcSleepSync(BYTE* pMsg);
	void	s2cViewEquip(BYTE* pMsg);
	void	LadderResult(BYTE* pMsg);
	void	LadderList(BYTE* pMsg);
	void	s2cTongCreate(BYTE* pMsg);
	void	ServerReplyClientPing(BYTE* pMsg);
	void	s2cNpcGoldChange(BYTE* pMsg);
	void	TaskValueSync(BYTE* pMsg);
	void	s2cFindPathSync(BYTE* pMsg);
	void	PlayerStopMove(BYTE* pMsg);
	void	s2cPlayerPos(BYTE* pMsg);
	void	SyncPlayerRide(BYTE* pMsg);
	void	NetCommandBloodNo(BYTE* pMsg);
	void	s2cPlayerRefesh(BYTE* pMsg);
	void	ItemChangeInfo(BYTE* pMsg);
	void	s2cMaskLock(BYTE* pMsg);
	void	s2cViewShop(BYTE* pMsg);
	void	s2cPlayerAutoDoSkill(BYTE* pMsg);
	void	NetCommandRandMove(BYTE* pMsg);
	void	s2cExtend(BYTE* pMsg);
	void	s2cExtendChat(BYTE* pMsg);
	void	s2cExtendFriend(BYTE* pMsg);
	void	s2cExtendTong(BYTE* pMsg);
	void	s2cAutoPlaySync(BYTE* pMsg);//AutoAI by kinnox;
	void	s2cLockPlayerSync(BYTE* pMsg);//LockPlayer by kinnox;
	void	s2cReposiPlayer(BYTE* pMsg);//load 3 storebox by kinnox;
	void	s2cExpandboxPlayer(BYTE* pMsg);//Expandbox by kinnox;
	void	s2cGiveBox(BYTE* pMsg);//GiveBox by kinnox;	
	void 	s2cNotification(BYTE* pMsg);//Notification by kinnox;	
	void	s2cReWardBox(BYTE* pMsg);//Reward by kinnox;	
	void	s2cTimeBox(BYTE* pMsg);//TimeBox by kinnox;
	void	s2cPlayerPointSync(BYTE* pMsg);//SyncPoint by kinnox;
	void	PlayerSendTopTKNew(BYTE* pMsg);//Report Battle by kinnox;
	void 	s2cRightItemAutoMove(BYTE* pMsg);
	void 	GetPlaceName(int place, char* out, size_t outsz);
	void 	ShowLienTram(BYTE* pMsg);
#else
	void	RemoveRole(int nIndex, BYTE * pProtocol);
	void	NpcRequestCommand(int nIndex, BYTE* pMsg);
	void	ObjRequestCommand(int nIndex, BYTE* pProtocol);
	void	NpcWalkCommand(int nIndex, BYTE* pProtocol);
	void	NpcRunCommand(int nIndex, BYTE* pProtocol);
	void	NpcSkillCommand(int nIndex, BYTE* pProtocol);
	void	NpcJumpCommand(int nIndex, BYTE* pProtocol);
	void	NpcTalkCommand(int nIndex, BYTE* pProtocol);
	void	PlayerTalkCommand(int nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamInfo(int nIndex, BYTE* pProtocol);
	void	PlayerApplyCreateTeam(int nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamOpenClose(int nIndex, BYTE* pProtocol);
	void	PlayerApplyAddTeam(int nIndex, BYTE* pProtocol);
	void	PlayerAcceptTeamMember(int nIndex, BYTE* pProtocol);
	void	PlayerApplyLeaveTeam(int nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamKickMember(int nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamChangeCaptain(int nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamDismiss(int nIndex, BYTE* pProtocol);
	void	PlayerApplySetPK(int nIndex, BYTE* pProtocol);
	void	PlayerApplyFactionData(int nIndex, BYTE* pProtocol);
	void	PlayerSendChat(int nIndex, BYTE* pProtocol);
	void	PlayerAddBaseAttribute(int nIndex, BYTE* pProtocol);
	void	PlayerApplyAddSkillPoint(int nIndex, BYTE* pProtocol);
	void	PlayerEatItem(int nIndex, BYTE* pProtocol);
	void	PlayerPickUpItem(int nIndex, BYTE* pProtocol);
	void	PlayerMoveItem(int nIndex, BYTE* pProtocol);
	void	PlayerSellItem(int nIndex, BYTE* pProtocol);
	void	PlayerBuyItem(int nIndex, BYTE* pProtocol);
	void	PlayerDropItem(int nIndex, BYTE* pProtocol);
	void	PlayerSelUI(int nIndex, BYTE* pProtocol);
	void	ChatSetChannel(int nIndex, BYTE* pProtocol);
	void	ChatApplyAddFriend(int nIndex, BYTE* pProtocol);
	void	ChatAddFriend(int nIndex, BYTE* pProtocol);
	void	ChatRefuseFriend(int nIndex, BYTE* pProtocol);
	void	ChatApplyReSendAllFriendName(int nIndex, BYTE* pProtocol);
	void	ChatApplySendOneFriendName(int nIndex, BYTE* pProtocol);
	void	ChatDeleteFriend(int nIndex, BYTE* pProtocol);
	void	ChatReDeleteFriend(int nIndex, BYTE* pProtocol);
	void	TradeApplyOpen(int nIndex, BYTE* pProtocol);
	void	TradeApplyClose(int nIndex, BYTE* pProtocol);
	void	TradeApplyStart(int nIndex, BYTE* pProtocol);
	void	TradeMoveMoney(int nIndex, BYTE* pProtocol);
	void	TradeDecision(int nIndex, BYTE* pProtocol);
	void	DialogNpc(int nIndex, BYTE * pProtocol);
	void	TeamInviteAdd(int nIndex, BYTE * pProtocol);
	void	ChangeAuraSkill(int nIndex, BYTE * pProtocol);
	void	TeamReplyInvite(int nIndex, BYTE * pProtocol);
	//void	ReplyPing(int nIndex, BYTE* pProtocol);
	void	NpcSitCommand(int nIndex, BYTE* pProtocol);
	void	ObjMouseClick(int nIndex, BYTE* pProtocol);
	void	StoreMoneyCommand(int nIndex, BYTE* pProtocol);
	void	NpcReviveCommand(int nIndex, BYTE* pProtocol);
	void	c2sTradeReplyStart(int nIndex, BYTE* pProtocol);
	void	c2sPKApplyChangeNormalFlag(int nIndex, BYTE* pProtocol);
	void	c2sPKApplyEnmity(int nIndex, BYTE* pProtocol);
	void	c2sViewEquip(int nIndex, BYTE* pProtocol);
	void	LadderQuery(int nIndex, BYTE* pProtocol);
	void	ItemRepair(int nIndex, BYTE* pProtocol);
	
	void	c2sPosCommand(int nIndex, BYTE* pProtocol);
	void	PlayerRideCommand(int nIndex, BYTE* pProtocol);
	void	ExeScriptCommand(int nIndex, BYTE* pProtocol);
	void	ItemBreakCommand(int nIndex, BYTE* pProtocol);
	void	c2sPassword(int nIndex, BYTE* pProtocol);
	void	HideMaskCommand(int nIndex, BYTE* pProtocol);
	void	MarkPriceItemCommand(int nIndex, BYTE* pProtocol);
	void	MakeAdvTextCommand(int nIndex, BYTE* pProtocol);
	void	PlayerApplyShopState(int nIndex, BYTE* pProtocol);
	void	ViewShopCommand(int nIndex, BYTE* pProtocol);
	void	BuyPlayerShopCommand(int nIndex, BYTE* pProtocol);
	void 	c2sSetAutoSync(int nIndex, BYTE* pProtocol);//AutoAI by kinnox;
	void	c2sBackToTown(int nIndex, BYTE* pProtocol);
	void	c2sCrePwBox(int nIndex, BYTE* pProtocol);//LockPlayer by kinnox;
	void	c2sLoginPwBox(int nIndex, BYTE* pProtocol);//LockPlayer by kinnox;
	void	c2sChaPwBox(int nIndex, BYTE* pProtocol);//LockPlayer by kinnox;	
	void	c2sLockPwBox(int nIndex, BYTE* pProtocol);//LockPlayer by kinnox;	
	void	c2sGiveBoxScript(int nIndex, BYTE* pProtocol);//GiveBox by kinnox;
	void	c2sGiveBoxCmd(int nIndex, BYTE* pProtocol);//GiveBox by kinnox;	
	void	c2sReWardScript(int nIndex, BYTE* pProtocol);//Reward by kinnox;	
	void	c2sTimeBoxScript(int nIndex, BYTE* pProtocol);//TimeBox by kinnox;
	void	c2sTopBattle(int nIndex, BYTE* pProtocol);//Report Battle by kinnox;
	void	c2sAvatarPlayer(int nIndex, BYTE* pProtocol);//Player Avatar by kinnox;
	void	RecoveryBoxCmd(int nIndex, BYTE* pProtocol);//TrembleItem by kinnox;
	// Add By ShinT
	void	c2sEndRunTime(int nIndex, BYTE* pProtocol);
	void	PlayerRightAutoMove(int nIndex, BYTE* pProtocol);
	void	PlayerAutoSortEquipment(int nIndex, BYTE* pProtocol); // Auto-sort inventory
	// End
#endif
};

extern KProtocolProcess g_ProtocolProcess;
#endif
