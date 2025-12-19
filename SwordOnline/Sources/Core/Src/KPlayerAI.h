#ifndef KPLAYERAI_H
#define KPLAYERAI_H

#ifndef _SEVER

enum
{
	Blood,
	Mana,
	AntiPoison,
	TownPortal,	
	EnchaseExp,
	MecidineBag,
	SpeedPortal,		
};

enum STEPAUTO
{
	normal = 0,
	StepGoToBox,
	StepGoToBuyMedicine,
	StepGoToBuyTownPortant,
	StepGoToBox2,
	StepGotoOldMap,
};

class KEquipmentArray
{
public:
	KLinkArray	m_Link;
	int		FindSame(int nIdx);
};


class KPlayerAI
{
	friend class KPlayer;
private: 
	BOOL			m_bActacker;
	int				m_Actacker;
	int				m_ArrayNpcNeast[defMAX_ARRAY_AUTO];
	int				m_ArrayNpcLag[defMAX_ARRAY_AUTO];
	int				m_ArrayTimeNpcLag[defMAX_ARRAY_AUTO];
	int				m_nLifeLag;
	int				m_nTimeRunLag;
	int				m_nTimeSkip;
	int				m_Count_Acttack_Lag;
	int 			m_nTimerMove;
	unsigned int	m_LifeCountDown;
	unsigned int	m_ManaCountDown;
	unsigned int	m_PortalCountDown;
	unsigned int	m_AntiPoisonCountDown;
	unsigned int	m_BoodBagCountDown;
	unsigned int	m_TTLCountDown;	
	unsigned int	m_EatFullBoxCountDown;	

public:
	unsigned int __stdcall UpdatePositionAsynchronously(LPVOID lpParam);
	int					m_nStartAIX;
	int					m_nStartAIY;
	
	 BOOL			m_bPriorityUseMouse;
	//int				m_nTimePriorityUseMouse;
	int				m_AutoActive;
	BOOL			m_bAutoAttack;
	BOOL			m_bFollowAttack;
	int				m_RadiusAuto;
	BOOL			m_SpaceBar;
	BOOL			m_HoldSpaceBar;
	BOOL			m_bAttackPeople;
	BOOL			m_bAttackNpc;
	int				m_FollowPeopleIdx;
	BOOL			m_bFollowPeople;
	char			m_FollowPeopleName[32];
	int				m_nRadiusFollow;
	int				m_nLeaderCurrentTarget;
	unsigned int	m_nLeaderTargetLostTime;	// Time when leader's target was lost (for grace period)
	int				m_nCachedLeaderPosX;		// FIX: Cached leader X position to prevent stale position
	int				m_nCachedLeaderPosY;		// FIX: Cached leader Y position
	unsigned int	m_dwLastLeaderPosCache;		// FIX: Time of last position cache update
	unsigned int	m_dwCombatEndTime;			// FIX: Time when combat ended (for delay before follow)	
	BOOL			m_bAttackAround;
	BOOL			m_bAroundBtn;
	int				m_PosXAuto;
	int				m_PosYAuto;
	int				m_DistanceAuto;
	BOOL			m_bFightDistance;
	int				m_ArrayStateSkill[defMAX_ARRAY_STATESKILL];
	int				m_AuraSkill[defMAX_ARRAY_AURA];
	int				m_bRiderHorse;
	int				m_nDefense;
	int				m_nFightBoss;
	BOOL			m_EatLife;
	BOOL			m_EatMana;
	unsigned int	m_LifeTimeUse;
	unsigned int	m_ManaTimeUse;
	int				m_LifeAutoV;
	int				m_ManaAutoV;
	int				m_LifeAutoP;
	int				m_ManaAutoP;
	BOOL			m_TPLife;
	int				m_TPLifeV;
	BOOL			m_TPMana;
	int				m_TPManaV;
	BOOL			m_TPNotMedicineBlood;
	BOOL			m_TPNotMedicineMana;
	BOOL			m_TPHightMoney;
	int				m_TPHightMoneyV;
	BYTE			m_btDurabilityItem;
	BYTE			m_btDurabilityValue;
	BOOL			m_bTPNotEquipment;
	BYTE			m_nTPNotEquipmentV;
	BOOL			m_AutoAntiPoison;
	BOOL			m_AutoTTL;
	BOOL			m_AutoBoodBag;
	BOOL			m_bRepairEQuip;
	BOOL 			m_bSaveMoney;
	int				m_nMoney;
	BOOL			m_AutoLifeReplenish;
	int				m_AutoLifeReplenishP;
	BOOL			m_EatFullBox;
	BOOL			m_nWarningPK;
	int				m_nLeftSkillSet[defMAX_ARRAY_FIGHT];
	int				m_nLeftSkillActiving;
	int				m_nOldRightHandActiving;
	int				m_nRightHandSkillSet[defMAX_ARRAY_FIGHT];
	int				m_MoveMps[defMAX_AUTO_MOVEMPSL][3];
	int				m_MoveStep;
	int				m_MoveCount;
	int 			m_MoveRevese;	
	BOOL	 		b_MoveRevese;	
	BOOL			m_AutoMove;
	BOOL			m_bAutoParty;
	BOOL			m_bAutoInvite;
	BOOL			m_bAutoAccept;
	BOOL			m_bAutoInviteList;
	BOOL			m_bAutoAcceptAll;
	BOOL			m_bAutoAcceptWithName;
	char			m_szPartyList[defMAX_LIST_PARTY][32];
	int				m_nArrayInvitePlayer[defMAX_LIST_PARTY];
	int				m_nArrayTimeInvitePlayer[defMAX_LIST_PARTY];
	BOOL			m_bPickItem;
	BYTE			m_btPickUpKind;
	BOOL			m_SaveRAP;
	BOOL			m_bFilterEquipment;
	BOOL			m_bObject;
	int				m_nObject;
	BOOL 			m_bPiceItem;
	int				m_nPiceItem;
	BOOL 			m_bLevelItem;
	int				m_nLevelItem;		
	KEquipmentArray	m_sListEquipment;
	BYTE			m_nNextItem;	
	unsigned int	m_RepairTime;	
	int				m_BackToTownPor;
	int				m_nTimeSetBack;
	int				m_nTimeBacking;
	int				m_nTimeBackToMapTrain;
	int				m_nTimeMove;
	int				m_FilterMagic[defMAX_AUTO_FILTERL][2];
	int				m_ArrayTimeObjectLag[100];
	int				m_ArrayObjectNeast[100];
	int				m_ArrayObjectLag[100];
	int 			m_BackMapTrain;
	BOOL			bPlayerSellItem;
	BOOL			bPlayerMoveItem;
	BOOL			bCheckFilter;
	BOOL			bCheckSellItem;
	int 			nTickAttack;
	KNpcFindPath		m_NpcFind;
	int				nNpcIDPaint;
	int				nDestXPaint;
	int				nDestYPaint;

	// ===== PACKET BURST FIX: LAG DETECTION & RATE LIMITING =====
	// Combat lag detection fields (HP stall + distance stall)
	unsigned int	m_nLastTargetHP;			// Last seen HP of current target
	int				m_nLastTargetDistance;		// Last distance to target (MPS units)
	int				m_nSameHPCounter;			// Ticks with same HP (lag indicator)
	int				m_nSameDistCounter;			// Ticks with same distance (unreachable)
	int				m_nLastTargetPosX;			// For distance tracking
	int				m_nLastTargetPosY;

	// Follow leader fallback to route when leader lost
	unsigned int	m_dwLeaderLostTime;			// Time when leader was lost
	BOOL			m_bFollowFallback;			// In fallback-to-route mode?
	unsigned int	m_dwLastReacquireAttempt;	// Last reacquire attempt time
	int				m_nSavedAutoMove;			// Saved m_AutoMove state before fallback

	// Training area radius limit (prevent running too far from waypoints)
	int				m_nTrainingRadius;			// Max distance from waypoints (in cells, 0 = unlimited)
	BOOL			m_bStayInTrainingArea;		// Force stay within training radius?

	// Follow leader auto-mount
	BOOL			m_bFollowAutoMount;			// Auto-mount horse when following leader and far away

	// Route + combat balance (pause route to fight nearby enemies)
	BOOL			m_bRoutePaused;				// Route paused for combat?
	int				m_nPausedRouteStep;			// Which waypoint we paused at
	int				m_nLastRouteX;				// For stuck detection
	int				m_nLastRouteY;
	int				m_nStuckCounter;			// Ticks stuck at same waypoint
	unsigned int	m_dwLastWaypointTime;		// Time of last waypoint change

	// VPS-safe packet rate limiting (prevent IP blocks from burst)
	DWORD			m_PacketLastSent[4];		// [0]=MOVE [1]=ATTACK [2]=SKILL [3]=ITEM
	int				m_PacketCount[4];			// Counts in current 1-second window
	DWORD			m_PacketWindowStart;		// Start of current 1-second window
	int				m_PacketTotalThisSecond;	// Total packets this second
	DWORD			m_PacketLastLogTime;		// Last time we logged stats (every 5s)

public:
	KPlayerAI() {};	
	~KPlayerAI() {}
	void			Init();		
	void			Release();
	void			Active();
	void			ResetAuto();
	void			MoveTo(int nX, int nY);
	BOOL			GetSpaceFlag() { return m_SpaceBar; };
	void			ClearArrayNpcLag(){memset(m_ArrayNpcLag, 0, sizeof(m_ArrayNpcLag));};
	void			ClearArrayTimeNpcLag(){memset(m_ArrayTimeNpcLag, 0, sizeof(m_ArrayTimeNpcLag));};
	void			PlayerFollowActack(int i);	
	BOOL			AutoAddNpc2Array(int nRelation);
	int				FindNearNpc2Array(int nRelation);
	BOOL			IsNotValidNpc(int nIndex);
	BOOL			DefensePlayer(int nIndex);
	void			CheckPlayerRider();
	BOOL			AutoBuffSkillState();
	BOOL			AutoBuffEmi();
	void			PlayerSwitchAura();
	void			PlayerBuffWhenManaSmall();
	void			PlayerActiveFightHand();
	void			SendAroundAuto(BOOL bUp);
	BOOL			AutoReturn();
	BOOL			PlayerMoveMps();
	void			InviteParty();
	BOOL			CheckPTList(const char *Name);
	void			ClearArrayInvitePlayer(){memset(m_nArrayInvitePlayer, 0, sizeof(m_nArrayInvitePlayer));};
	void			ClearArrayTimeInvitePlayer(){memset(m_nArrayTimeInvitePlayer, 0, sizeof(m_nArrayTimeInvitePlayer));};
	void			ClearArrayObjectLag(){memset(m_ArrayObjectLag, 0, sizeof(m_ArrayObjectLag));};
	void			ClearArrayTimeObjectLag(){memset(m_ArrayTimeObjectLag, 0, sizeof(m_ArrayTimeObjectLag));};
	void			ClearArrayObjectNeast(){memset(m_ArrayObjectNeast, 0, sizeof(m_ArrayObjectNeast));};
	BOOL			FindObjectNearPlayer();
	int				FindNearObject2Array();
	BOOL			PlayerFollowObject(int nObject);
	void			PlayerFilterEquip();
	BOOL			IsRAPEquip(int btDetail);
	BOOL			IsEquipSatisfyCondition(int nIdx);
	BOOL			IsNotValidObject(int nObject);		
	void			BackMapTrain();
	///
	int				FindFreeInvitePlayer()
	{
		for (int i=0; i < defMAX_LIST_PARTY; i++)
		{
			if (m_nArrayInvitePlayer[i] <= 0)
				return i;
		}		
		return -1;
	};	
	///
	BOOL			FindSameInvitePlayer(int uID)
	{
		for (int i=0; i < defMAX_LIST_PARTY; i++)
		{
			if (m_nArrayInvitePlayer[i] == uID)
				return TRUE;
		}		
		return FALSE;
	};
	///
	BOOL			PlayerEatAItem();
	BOOL			PlayerUseItem(int type);
	void			EatFullBox();
	BOOL			WithDrawal();
	BOOL 			RepairEquip();
	BOOL			SaveMoney();
	void			BackToMap();
	void			MoveItemToBox();
	void 			PaintActionAuto(int nType,int nNpcID,int nX,int nY);

	// ===== PACKET BURST FIX: New helper methods =====
	BOOL			IsTargetLagging(int nTargetIdx);
	void			MarkTargetAsLag(int nTargetIdx);
	void			UpdateFollowFallback();
	BOOL			ShouldEngageOnRoute(int *outNpcIdx);
	void			UpdateRouteProgress();
	BOOL			CanSendPacket(int packetType);		// 0=MOVE 1=ATTACK 2=SKILL 3=ITEM
	void			LogPacketStats();

	// ===== TRAINING AREA RADIUS ENFORCEMENT =====
	BOOL			IsWithinTrainingArea(int *outNearestWaypointIdx);
	void			ReturnToTrainingArea();
};
#endif
#endif