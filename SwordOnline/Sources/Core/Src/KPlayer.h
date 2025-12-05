
#ifndef KPlayerH
#define	KPlayerH
#include "KWorldMsg.h"
#include "KMessage.h"
#include "KInventory.h"
#include "KPlayerFaction.h"
#include "KPlayerChat.h"
#include "KPlayerTask.h"
#include "KPlayerMenuState.h"
#include "KPlayerTrade.h"
#include "KPlayerTeam.h"
#include "KPlayerPK.h"
#include "KPlayerTong.h"
#include "KItemList.h"
#include "KNpc.h"
#include "KPlayerAI.h" //AutoAI by kinnox;
#include "Kautoai.h"
#include "KSkills.h"
#include "KPlayerShop.h"
#include "KPlayerDef.h"

#ifdef _SERVER
	#include "CLogFile.h"
#endif

#define		MAX_ANSWERNUM					50
#define		PLAYER_LIFE_REPLENISH			0
#define		PLAYER_MANA_REPLENISH			0
#define		PLAYER_STAMINA_GAIN				1
#define		PLAYER_STAMINA_LOSS				4
#define		STRENGTH_SET_DAMAGE_VALUE		5
#define		DEXTERITY_SET_DAMAGE_VALUE		5
#define		MAX_AVENGE_NUM					4

#define		MAX_PACKER						100 //50

enum	UIInfo
{
	UI_SELECTDIALOG = 1,
	UI_TRADEDIALOG,
	UI_TALKDIALOG,
	UI_NOTEINFO,
	UI_MSGINFO,
	UI_NEWSINFO,
	UI_NEWSINFOEX,
	UI_PLAYMUSIC,
	UI_OPENTONGUI,
	
	UI_ASKFORNUMBER,
	UI_ASKFORSTRING,
	UI_CHANGETAXDIALOG,
	
};

typedef struct PLAYER_REVIVAL_POS_DATA
{
	int				m_nSubWorldID;		
	int				m_ReviveID;			
	int				m_nMpsX;		
	int				m_nMpsY;			
} PLAYER_REVIVAL_POS;

typedef struct
{
	int				m_nSubWorldId;		
	int				m_nTime;
	int				m_nMpsX;
	int				m_nMpsY;
} PLAYER_TOWNPORTAL_POS;

typedef struct 
{
	DWORD			m_dwMapID;
	int				m_nX;
	int				m_nY;
} PLAYER_EXCHANGE_POS;

// find way by kinnox;
struct KPLAYER_AUTOPATH
{
	BOOL	bAutoPathing;			//	The player is in the automatic pathfinding phase
	INT		nDesX;
	INT		nDesY;
	INT		nCurDesX;
	INT		nCurDesY;
	KPLAYER_AUTOPATH()
	{
		bAutoPathing = FALSE;
		nDesX = 0;
		nDesY = 0;
		nCurDesX = nCurDesY = 0;
	};
};
//

typedef struct
{
	int		m_nBuyIdx;
	DWORD	m_SubWorldID;
	int		m_nMpsX;
	int		m_nMpsY;
	void	Clear() {m_nBuyIdx = -1; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;}
} BuySellInfo;
class KIniFile;


#ifdef TOOLVERSION
class CORE_API KPlayer
#else
class KPlayer
#endif
{
private:

#ifndef _SERVER
	int				m_nLeftSkillID;
	int				m_nLeftSkillLevel;
	int				m_nRightSkillID;
	int				m_nRightSkillLevel;
	BOOL			m_MouseDown[2];
#endif

#ifdef _SERVER
	PLAYER_REVIVAL_POS		m_sLoginRevivalPos;	
	PLAYER_REVIVAL_POS		m_sDeathRevivalPos;	
	PLAYER_TOWNPORTAL_POS	m_sPortalPos;
	BOOL			m_bUseReviveIdWhenLogin;
	// De day mai xoa 
	int				m_nExtPoint;
	int				m_nExtPoint2;
	int				m_nChangeExtPoint;
	// Vi toi thich xoa

					
	bool			m_bLoginDelaySync;
	WORD			m_wTitleId;
	
	
	DWORD			m_nArrayPacket[MAX_PACKER][2]; 
	// CLogFile		m_cBaseLog;
	// CLogFile		m_cItemLog;
	// CLogFile		m_cScriptLog;

#endif
	int				m_nPhysicsSkillID;
	int				m_nPeapleIdx;
	int				m_nObjectIdx;
	int				m_nPickObjectIdx;
	int				m_nPlayerIndex;
	//
	int				m_nBronzeCoin;
	int				m_nLockBronzeCoin;
	//
	KCacheNode *	m_pLastScriptCacheNode;

public:  
		//Kenny fix loi skil chuyen map
	#ifndef _SERVER
	int  GetRightSkillID() const;
	int  GetRightSkillLevel() const;
	
	void ForceReapplyRightAura();   // ép gửi lại aura cho GS mới (nếu là skill aura)
	#endif
#ifdef _SERVER
	int             m_nLixianStatus;  //Lixian by kinnox;
	int 			m_nLixianTime; //Lixian by kinnox;
	PLAYER_EXCHANGE_POS		m_sExchangePos;
	BOOL			m_bIsQuiting;
	UINT			m_uMustSave;
	DWORD			m_ulLastSaveTime;
	DWORD			m_dwLoginTime;


//	DWORD			m_uLastPingTime;
	char			m_AccoutName[32];				
	void*			m_pStatusLoadPlayerInfo;
	BYTE*			m_pCurStatusOffset;
	BOOL			m_bFinishLoading;
	BYTE			m_SaveBuffer[64 * 1024];
	int				m_nLastNetOperationTime;
	BOOL			m_bSleepMode;
	KList			m_PlayerWayPointList;
	KList			m_PlayerStationList;
	int				m_nViewEquipTime;			
	int				m_nPrePayMoney;
	enum
	{
		FF_CHAT = 0x01,
	};
	int				m_nForbiddenFlag;
	
	BOOL			m_bForbidTownPortal;
	KTabFile		m_cTabFile;
	int				m_nLastDialogIdx;
#endif

#ifndef _SERVER
	int				m_RunStatus;
	DWORD			m_dwNextLevelLeadExp;
	int				m_nSendMoveFrames;
	DWORD			m_dwLastPosSyncTime;
	int				m_nLastSyncX;		// Last synced X position for smart sync
	int				m_nLastSyncY;		// Last synced Y position for smart sync
	BOOL			m_bDebugMode;
	
	char			m_szExpAddNo[16];
	BYTE			m_btExpAddTime;
	BYTE			m_btExpAddAlpha;
	BOOL			m_bHideNpc;//AutoAI by kinnox;
	BOOL			m_bHidePlayer;//AutoAI by kinnox;
	BOOL			m_bHideAnimation;//AutoAI by kinnox;	
	KAutoMove		m_cAutoMove;
#endif

	KIndexNode		m_Node;
	GUID			m_Guid;
	BOOL			m_bExchangeServer;
	DWORD			m_dwID;						
	int				m_nIndex;					
	int				m_nNetConnectIdx;			
	KItemList		m_ItemList;					
	BuySellInfo		m_BuyInfo;					
	KPlayerMenuState	m_cMenuState;			
	KTrade			m_cTrade;					
	int				m_nAttributePoint;			
	int				m_nSkillPoint;				

	int				m_nStrength;				
	int				m_nDexterity;				
	int				m_nVitality;				
	int				m_nEngergy;					
	int				m_nLucky;					
	BYTE			m_btChatSpecialChannel;
	int				m_nCurStrength;				
	int				m_nCurDexterity;			
	int				m_nCurVitality;				
	int				m_nCurEngergy;				
	int				m_nCurLucky;				

	int				m_nExp;	
	int				m_nNextLevelExp;			

	DWORD			m_dwLeadExp;				
	DWORD			m_dwLeadLevel;				
	char			m_PlayerName[defPLAYER_NAME_MAX_LENGTH + 1];
	KPlayerTeam		m_cTeam;					
	KPlayerFaction	m_cFaction;					

	KPlayerChat		m_cChat;

	KPlayerTask		m_cTask;

	KPlayerPK		m_cPK;

	KPlayerAI		m_cAI; //AutoAI by kinnox;	
	
	KPlayerTong		m_cTong;
	
	KShop			m_cShop;
	

	DWORD			m_dwDeathScriptId;
	
	DWORD			m_dwLogoutScriptId;
	
	DWORD			m_dwGiveBoxId;//GiveBox by kinnox;

	DWORD			m_dwRewardId;//Reward by kinnox;

	DWORD			m_dwTimeBoxId;//TimeBox by kinnox;
	
	char			m_szTaskAnswerFun[MAX_ANSWERNUM][32];
	int				m_nAvailableAnswerNum;
	bool			m_bWaitingPlayerFeedBack;
//	DWORD			m_dwOutOfDateFeedBackTime;	
//	char			m_CurScriptName[128];
	int				m_nWorldStat;
	int				m_nSectStat;
	
	DWORD			m_dwTimeHorse;
	int				m_nFortunePoint;
	int				m_nTimeCount;
	int				m_nTimeCountMax;
	DWORD			m_nTimeScript;
	int				m_nTimeCountId[MAX_TIME_COUNTID];
	DWORD			m_nTimeScriptId[MAX_TIME_COUNTID];
	char			szTimeFunc[MAX_TIME_COUNTID][128];
	BOOL			m_bTimeCountId;
	int				m_btRepositoryCount;
	int				m_btExpandtoryCount;//Expandbox by kinnox;	
	int				m_nLockPlayer;//LockPlayer by kinnox;
	BOOL			m_bLockPlayer;//LockPlayer by kinnox;
	BOOL			m_bPass;//LockPlayer by kinnox;
	BOOL			m_bActiveAuto;//AutoAI by kinnox;
	int				m_ImagePlayer;//Player Avatar by kinnox;
	//
public:
	KPlayer();
	~KPlayer();
	//load 3 storebox by kinnox;
	void			SetStoreBoxNum(BYTE nNum);
	void			SetExpandBoxNum(BYTE nNum);	 //Expandbox by kinnox;
	//
	void 			LockPlayerInfo(BYTE bType,int nNumber1 = 0, int nNumber2 = 0);//LockPlayer by kinnox;
	//
	void			SetAutoFlag(BOOL nbAuto);//AutoAI by kinnox;
	BOOL			GetAutoFlag() { return m_bActiveAuto; };//AutoAI by kinnox;
	// end
	void			SetPlayerIndex(int nNo);
	void			GetAboutPos(KMapPos *pMapPos);
	int				GetPlayerIndex();
	DWORD			GetPlayerID(){return m_dwID;};
	void			GetFactionName(char *lpszName, int nSize);
	void			ChatFriendOnLine(DWORD dwID, int nFriendIdx);
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, int nParam = 0);
	BOOL			ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam);
	BOOL			ExecuteScript(char * ScriptFileName, char * szFunName, char * szParams);
	BOOL			ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams);
	
	BOOL			ExecuteScript2(char* ScriptFileName, char* szFunName, int nParam1 = 0, int nParam2 = 0, BOOL bSelectUI = TRUE);
	BOOL			ExecuteScript2(DWORD dwScriptId,  char* szFunName, int nParam1, int nParam2, BOOL bSelectUI = TRUE);

	BOOL			ExecuteScript2(char* ScriptFileName, char* szFunName, int nParam, char* szParams, BOOL bSelectUI = TRUE);
	BOOL			ExecuteScript2(DWORD dwScriptId, char* szFunName, int nParam, char*  szParams, BOOL bSelectUI = TRUE);
	
	void			ChangeCurStrength(int nData);
	void			ChangeCurDexterity(int nData);
	void			ChangeCurVitality(int nData);
	void			ChangeCurEngergy(int nData);
	BOOL			ExecuteScript(char * ScriptFileName);
	void			Release();
	void			Active();
	void			ProcessMsg(KWorldMsgNode *lpMsg);
	
	LPSTR			GetPlayerName() { return m_PlayerName; };

	void			ResetBaseAttribute(char cSeries);
	void			AddBaseLucky(int nData);

	void			AddExp(float nExp, int nTarLevel, BOOL bCheck = FALSE); // chia exp team by kinnox;
	void			AddSelfExp(int nExp, int nTarLevel);
	void			DirectAddExp(int nExp);
	void			LevelUp();
	void			AddLeadExp(int nExp);
	void			UpdataCurData();
	void			ReCalcEquip();
	void			ReCalcState();

	void			ChangePlayerCamp(int nCamp);
	void			Revive(int nType);

	BOOL			CheckTrading();
	void			SetFirstDamage();
	void			SetBaseAttackRating();
	void			SetBaseDefence();
	void			SetBaseResistData();
	void			SetBaseSpeedAndRadius();
	int				GetPhysicsSkillId(){return m_nPhysicsSkillID;};
	void			SetPhysicsSkillId(int nPhysicsSkillID)
	{
		if (nPhysicsSkillID <= 0) return ;
		ISkill * pISkill =  g_SkillManager.GetSkill(nPhysicsSkillID, 1);
		if (!pISkill) 
            return ;
		
		//
        if (nPhysicsSkillID > 0 && pISkill->GetSkillAttrib() < 100)
			m_nPhysicsSkillID = nPhysicsSkillID;
		
	};
	
// --
	int				TaskGetSaveVal(int nNo);				
	void			TaskSetSaveVal(int nNo, BOOL bFlag);
	int				TaskGetClearVal(int nNo);				
	void			TaskSetClearVal(int nNo, int nVal);
// --
	void            CheckRideHorse(BOOL bFlagRide);
// --
	void			SetLevel(BYTE btLevel);
	void			RefeshPlayer(bool bClear = false);	
// --
	void			AddAllSkillPoint(int nAdd);				
	void			AddSkillPoint(int SkillID, int nAdd);		
	void			UpdateSkillPointAdd();
// --
	void			HideMask(BOOL bHide);
	

	void			SetBronzeCoin(const int nCoin);
	int				GetBronzeCoin();
	BOOL			PayBronzeCoin(const int nCoin);
	//
	void			SetLockBronzeCoin(const int nCoin);
	int				GetLockBronzeCoin();
	BOOL			PayLockBronzeCoin(const int nCoin);
#ifndef _SERVER
	int				GetTargetNpc() { return m_nPeapleIdx; };
	int				GetTargetObj() { return m_nObjectIdx; };
	void			SetTargetNpc(int n) { m_nPeapleIdx = n; };
	void			SetTargetObj(int n) { m_nObjectIdx = n; };
	void			FindSelectNpc(int x, int y, int nRelation);
	void			FindSelectObject(int x, int y);
	void			Walk(int nDir, int nSpeed);	
	void			TurnLeft();
	void			TurnRight();
	void			TurnBack();
	void			DrawSelectInfo();
	BOOL			ConformIdx(int nIdx);	
	void			GetEchoDamage(int* nMin, int* nMax, int* nAr, int nType);		//
	void			ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void			RecvSyncData();

	void			ApplyTeamInfo(DWORD dwNpcID);
	void			ApplySelfTeamInfo();
	BOOL			ApplyCreateTeam();
	BOOL			ApplyTeamOpenClose(BOOL bFlag);
	void			ApplyAddTeam(int nNpcIndex);
	void			AcceptTeamMember(DWORD dwNpcID);
	void			TeamDropApplyOne(DWORD dwNpcID);
	void			LeaveTeam();
	void			TeamKickMember(DWORD dwNpcID);
	void			ApplyTeamChangeCaptain(DWORD dwNpcID);
	void			ApplyTeamDismiss();							
	void			ApplySetPK(BOOL bPK);						
	void			ApplyFactionData();							
	void			SendChat(KUiMsgParam *pMsg, char *lpszSentence);
	void			ApplyAddBaseAttribute(int nAttribute, int nNo);
	BOOL			ApplyAddSkillLevel(int nSkillID, int nAddPoint);
	BOOL			ApplyUseItem(int nItemID, ItemPos SrcPos);
	void			PickUpObj(int nObjIndex);
	void			ObjMouseClick(int nObjIndex);
	void			MoveItem(ItemPos DownPos, ItemPos UpPos);

	int				ThrowAwayItem();
	int				AutoThrowAwayItem(int nIndexItem);

	void			ChatAddFriend(int nPlayerIdx);
	void			ChatRefuseFriend(int nPlayerIdx);
	void			TradeApplyOpen(char *lpszSentence, int nLength);
	void			TradeApplyClose();
	void			TradeApplyStart(int nNpcIdx);
	BOOL			TradeMoveMoney(int nMoney);
	void			TradeDecision(int nDecision);
	void			TradeApplyLock(BOOL bLockOrNot);		//

	void			SetChatCurChannel(int nChannelNo);
	void			TeamInviteAdd(DWORD dwNpcID);

	void			SetLeftSkill(int nSkillID);
	void			SetRightSkill(int nSkillID);
	void			UpdateWeaponSkill();
	int				GetLeftSkill(){return m_nLeftSkillID;};
	int				GetRightSkill(){return m_nRightSkillID;};
	void			SetDefaultImmedSkill();
	void			s2cApplyAddTeam(BYTE* pProtocol);
	void			s2cTradeChangeState(BYTE* pMsg);
	void			s2cTradeMoneySync(BYTE* pMsg);
	void			s2cTradeDecision(BYTE* pMsg);
	void			SyncCurPlayer(BYTE* pMsg);
	void			s2cLevelUp(BYTE* pMsg);
	void			s2cGetCurAttribute(BYTE* pMsg);
	void			s2cSetExp(int nExp);
	void			s2cSyncMoney(BYTE* pMsg);
	void			s2cTradeApplyStart(BYTE* pMsg);
	void			CheckObject(int nIdx);
	
	BOOL			ShopApplyOpen();
	BOOL			ShopApplyClose();
	
#endif

#ifdef _SERVER
	void			SetExtPoint(int nPoint, int nChangePoint);
	void			SetExtPoint2(int nPoint);
	int				GetExtPoint();
	BOOL			PayExtPoint(int nPoint);
	int				GetExtPointChanged();
	void			SyncBronzeCoin();
	void			SyncLockBronzeCoin();
	//
	void			RepairItem(DWORD dwItemID);
	BOOL			PrePay(int nMoney);
	void			SetLastNetOperationTime(int nTime);
	int				FindAroundPlayer(DWORD dwNpcID);
	int				FindAroundNpc(DWORD dwNpcID);
	BOOL			CheckPlayerAround(int nPlayerIdx);
	BOOL			IsExchangingServer();
	void			TobeExchangeServer(DWORD dwMapID, int nX, int nY);
//	void			UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode);
	BOOL			IsWaitingRemove();
	BOOL			IsLoginTimeOut();
	void			WaitForRemove();
	void			LoginTimeOut();
	void			UseTownPortal();
	void			BackToTownPortal(int nIdSubWorld = 0);
	void			ClearPortal();
	void			GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);		
	void			GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY);	
	void			SetRevivalPos(int nSubWorld, int nRevalId);								
	BOOL			Save();									
	BOOL			CanSave();
	void			ProcessUser();
	BOOL			SendSyncData(int &nStep, unsigned int &nParam);	
	BOOL			SendSyncData_Skill();
	void			SendCurNormalSyncData();
	void			SetChatForbiddenFlag(int nFlag)
	{
		m_nForbiddenFlag = nFlag;
	};

	BOOL			AddFaction(char *lpszFactionName);		
	BOOL			AddFaction(int nFactionID);				
	BOOL			LeaveCurFaction();						
	BOOL			CurFactionOpenSkill(int nLevel);		

	void			TaskClearTempVal();

	//void			SetTimeTaskTime(DWORD time){m_dwTimeTaskTime = time;};
	void			BuyItem(BYTE* pProtocol);
	void			SellItem(BYTE* pProtocol);
	void			QuitGame(int nQuitType);				
	void			S2CSendTeamInfo(BYTE* pProtocol);		
	void			SendSelfTeamInfo();						
	BOOL			CreateTeam(BYTE* pProtocol);			
	BOOL			SetTeamState(BYTE* pProtocol);			
	BOOL			S2CSendAddTeamInfo(BYTE* pProtocol);	
	BOOL			AddTeamMember(BYTE* pProtocol);			
	void			LeaveTeam(BYTE* pProtocol);				
	void			TeamKickOne(BYTE* pProtocol);			
	void			TeamChangeCaptain(BYTE* pProtocol);		
	void			TeamDismiss(BYTE* pProtocol);			
	void			SetPK(BYTE* pProtocol);					
	void			SendFactionData(BYTE* pProtocol);		
	void			ServerSendChat(BYTE* pProtocol);		
	void			AddBaseAttribute(BYTE* pProtocol);
	void			AddSkillPoint(BYTE* pProtocol);
	BOOL			ServerPickUpItem(BYTE* pProtocol);
	void			EatItem(BYTE* pProtocol);
	void			ServerMoveItem(BYTE* pProtocol);
	void			ServerThrowAwayItem(BYTE* pProtocol);
	void			ChatSetTakeChannel(BYTE* pProtocol);
	void			ChatTransmitApplyAddFriend(BYTE* pProtocol);
	BOOL			ChatAddFriend(BYTE* pProtocol);
	void			ChatRefuseFriend(BYTE* pProtocol);
	void			ChatResendAllFriend(BYTE* pProtocol);
	void			ChatSendOneFriendData(BYTE* pProtocol);
	void			ChatDeleteFriend(BYTE* pProtocol);
	void			ChatRedeleteFriend(BYTE* pProtocol);
	void			TradeApplyOpen(BYTE* pProtocol);
	void			TradeApplyClose(BYTE* pProtocol);
	void			TradeApplyStart(BYTE* pProtocol);
	void			TradeMoveMoney(BYTE* pProtocol);
	void			TradeDecision(BYTE* pProtocol);
	void			c2sTradeReplyStart(BYTE* pProtocol);
	void			SyncTradeState();
	void			SendEquipItemInfo(int nTargetPlayer);
	PLAYER_REVIVAL_POS* GetDeathRevivalPos() 
	{
		return &m_sDeathRevivalPos;
	};
	BOOL			IsUseReviveIdWhenLogin() { return m_bUseReviveIdWhenLogin; };
	void			SetLoginType(BOOL bUseReviveId) { m_bUseReviveIdWhenLogin = bUseReviveId; };

	BOOL			CreateTong(int nCamp, char *lpszTongName);
	
	void			LoginDelaySync(bool bDelay) { m_bLoginDelaySync = bDelay; };
	bool			IsLoginDelaySync() const { return m_bLoginDelaySync; };
	void			ProcessEnhance();
	void			ProcessRemoveEnhance();
	void			ExeScriptButton(BYTE* pProtocol);
	void			BreakItem(const DWORD dwItemID, const int nNumber);


	void			SendShopItemInfo(int nTargetPlayer, int nDataIndex);
	void			BuyPlayerShop(BYTE* pProtocol);

	void			RestoreTask();
	
	BOOL			ExecuteScriptRet(char * ScriptFileName, char * szFunName, int nParam = 0);
	BOOL			ExecuteScriptRet(DWORD dwScriptId,  char * szFunName, int nParam);
	BOOL			ExecuteScriptRet(char * ScriptFileName, char * szFunName, char * szParams);
	BOOL			ExecuteScriptRet(DWORD dwScriptId,  char * szFunName, char * szParams);
	//
	void			CloseUI(BYTE btUIId);

	void			SetTitle(WORD wTitleId, bool bLogin = false);
	WORD			GetTitle()	const { return m_wTitleId; };

	void			StopMove();
	//
	//void			InitLog();
	void			SetNumImgAvatar(int ID);//Player Avatar by kinnox;
	void			GiveBoxCmd(DWORD dwID, int nX, int nY);//GiveBox by kinnox;	
#endif

private:
//	void			CalcCurStrength();						
//	void			CalcCurDexterity();						
//	void			CalcCurVitality();						
//	void			CalcCurEngergy();						
	void			CalcCurLucky();							
	void			LevelAddBaseLifeMax();					
	void			LevelAddBaseManaMax();					
	void			LevelAddBaseStaminaMax();				

	void			SendFactionData();						

	void			AddBaseStrength(int nData);				
	void			AddBaseDexterity(int nData);			
	void			AddBaseVitality(int nData);				
	void			AddBaseEngergy(int nData);				
	void			SetNpcPhysicsDamage();				
	void			SetNpcAttackRating();					
	void			SetNpcDefence();						
//	void			SetNpcWalkSpeed();						
//	void			SetNpcRunSpeed();						

#ifndef _SERVER
	void			ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton);
	void			OnButtonUp(int x,int y,MOUSE_BUTTON nButton);				
	void			OnButtonDown(int x,int y,int Key,MOUSE_BUTTON nButton);		
	void			OnButtonMove(int x,int y,int Key,MOUSE_BUTTON nButton);		
	void			OnMouseMove(int x,int y);									
	int				NetCommandPlayerTalk(BYTE* pProtocol);
	friend			LuaInitStandAloneGame(Lua_State * L);
	
	void			SetExpAddNo(int nNo);
	
#endif

#ifdef _SERVER
	int				GetTaxShopItem(int nPrice);		
#endif

private:
	void			S2CExecuteScript(char * ScriptName, char * szParam);

#ifdef _SERVER
private:
	int				LoadPlayerBaseInfo(BYTE * pRoleBuffer, BYTE * &pRoleBaseBuffer, unsigned int &nParam );
	int				LoadPlayerItemList(BYTE * pRoleBuffer, BYTE * &pItemBuffer, unsigned int &nParam );
	int				LoadPlayerFightSkillList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerLifeSkilllList(BYTE * pRoleBuffer, BYTE * &pSkillBuffer, unsigned int &nParam);
	int				LoadPlayerFriendList(BYTE * pRoleBuffer, BYTE * &pFriendBuffer, unsigned int &nParam);
	int				LoadPlayerTaskList(BYTE * pRoleBuffer, BYTE * &pTaskBuffer, unsigned int &nParam);
	int				SavePlayerBaseInfo(BYTE * pRoleBuffer);
	int				SavePlayerItemList(BYTE * pRoleBuffer);
	int				SavePlayerFightSkillList(BYTE * pRoleBuffer);
	int				SavePlayerLifeSkilllList(BYTE * pRoleBuffer);
	int				SavePlayerFriendList(BYTE * pRoleBuffer);
	int				SavePlayerTaskList(BYTE * pRoleBuffer);
#endif

public:
	void			SetNpcDamageAttrib();
	void			DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo); 
	void			ProcessPlayerSelectFromUI(BYTE* pProtocol);			
#ifndef _SERVER
	void			DialogNpc(int nIndex);
	void			OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo);			
	void			OnScriptAction(PLAYER_SCRIPTACTION_SYNC * );
	
	void			ClientSystemMsg(const char* lpszName, const char* lpszMsg);
	
#endif
#ifdef _SERVER
	void			RestoreLiveData();						
	int				AddDBPlayer(char * szPlayerName, int sex, DWORD * pdwID );
	int				LoadDBPlayerInfo(BYTE * pPlayerInfo,  int &nStep, unsigned int &nParam);
	BOOL			GetNewPlayerFromIni(KIniFile * pIniFile, BYTE * pRoleBuffer);
	int				UpdateDBPlayerInfo(BYTE * pPlayerInfo);
	int				DeletePlayer(char * szPlayerName = NULL);
	void			LaunchPlayer();
	BOOL			Pay(int nMoney);
	BOOL			Earn(int nMoney);
	void			DialogNpc(BYTE * pProtocol);

	int				AddTempTaskValue(void* pData);
#endif

// Check paket

#ifdef _SERVER
	BOOL			AddAntiPaket(DWORD idPaket, DWORD timePaket);

	void			SaveLog(int nType, const char* lpszData, const char* lpszDesc, const char* lpszValue);
#endif
};

#ifdef TOOLVERSION
extern CORE_API KPlayer	Player[MAX_PLAYER];
#else
extern KPlayer	Player[MAX_PLAYER];
#endif
#endif //KPlayerH

