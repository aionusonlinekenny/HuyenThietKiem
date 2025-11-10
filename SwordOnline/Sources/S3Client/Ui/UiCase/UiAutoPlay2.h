#include "../elem/wndpage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../../../Core/src/GameDataDef.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndList2.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndShadow.h"
struct KUiSceneTimeInfo; ///////////////////
enum DEFENSE
{
	enumDefense,
	enumBoss,
	enumFight,
};

enum MAGIC
{
	enumMagic_ID1,
	enumMagic_ID2,
	enumMagic_ID3,
};


class KUiRecovery : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void	SaveRecoverySetting();
	void	LoadRecoverySetting();
	void	Breathe();
private:
	static KUiRecovery* m_pSelf;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCheckInput();
	void	PopupSelRoomNE();
	void	SetPortalRoomNE(int nAction);	
private:
	KWndImage			m_BgRecovery;
	KWndButton			m_LifeCK;
	KWndEdit32			m_LifeEdit1;
	KWndEdit32			m_LifeEdit2;
	KWndEdit32			m_LifeEdit3;
	KWndButton			m_ManaCK;
	KWndEdit32			m_ManaEdit1;
	KWndEdit32			m_ManaEdit2;
	KWndEdit32			m_ManaEdit3;	
	KWndButton			m_TPLifeLessCK;
	KWndEdit32			m_TPLifeEdit;
	KWndButton			m_TPManaLessCK;
	KWndEdit32			m_TPManaEdit;
	KWndButton			m_TPNotMedicineBloodCK;
	KWndButton			m_TPNotMedicineManaCK;
	KWndButton			m_TPHightMoneyCK;
	KWndEdit32			m_TPHightMoneyEdit;
	KWndButton			m_TPDamageEquipCK;
	KWndEdit32			m_TPDamageEquipEdit;
	KWndButton			m_TPNotEquipmentChecker;
	KWndText80			m_TPNotEquipmentText;
	KWndButton			m_TPNotEquipmentPopup;
	KWndButton			m_OtherAntiPoisonCK;
	KWndButton			m_OtherEnchaseExpCK;
	KWndButton			m_OtherRepairEqCK;
	KWndButton			m_OtherOpenMedicineCK;
	KWndButton			m_OtherInventoryMoneyCK;
	KWndButton			m_OtherInventoryItemCK;		
	KWndButton			m_OtherBackToTownCK;	
	KWndButton			m_OtherRePortPKCK;
	KWndButton			m_OtherEatBloodCK;	
	KWndButton			m_OtherEmeiBuffCK;
	KWndEdit32			m_OtherEmeiBuffEdit;
	KWndButton			m_OtherSpamChatCK;
	KWndEdit512			m_OtherSpamChatEdit;
	int					m_nSelRoom;	
	int					TimerSpamChat;
	char* 				m_cMessenger; 
};

class KUiPick : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void 	LoadPickSetting();
	void	SavePickSetting();
private:
	static KUiPick* m_pSelf;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnPick();
	void	OnGiveItem();
	void	OnCheckPrice();
	void	OnCheckInput();
	void	OnCheckLevel();
	void	OnFillterItem();
	void	OnSelectPickType(int nIndex);
	void	OnSelectReturnPlace();	
	void 	SetPick(int nIndex, int nSel);
	void	SetFilterMagicList();
	int 	FilterSameMagic(const char* szMagic);
	int 	FindSameMagic(const char* szMagic);
private:
	KWndImage			m_BgPick;
	KWndButton	m_PickAllCK;
	KWndText32	m_PickAllEdit;
	KWndButton	m_PickAllPopup;
	KWndButton	m_PickFilterCK;
	KWndButton	m_PickFollowTargetCK;
	KWndEdit32	m_PickFollowTargetEdit;
	KWndText32	m_PickChooseFilterTxt;
	KWndButton	m_PickChooseFilterPopup;
	KWndEdit32	m_PickChooseFilterEdit;
	KWndList2	m_PickFilterListTxt;
	KWndScrollBar	m_PickFilterListScb;
	KWndLabeledButton m_PickFilterAdd;
	KWndLabeledButton m_PickFilterDel;
	KWndLabeledButton m_PickFilterDelAll;
	KWndButton	m_PickPriceItemsCK;
	KWndEdit80	m_PickPriceItemsEdit;
	KWndButton	m_PickLevelItemsCK;
	KWndEdit80	m_PickLevelItemsEdit;
	KWndButton	m_PickJewelryCK;
	int						m_CurrentPickKind;
	int						m_CurrentSelMagic;
	int						m_FilterMagicIndex[defMAX_AUTO_FILTERL];
	char					m_cFilterMagic[defMAX_AUTO_FILTERL][32];
	int						m_FilterMagicV[defMAX_AUTO_FILTERL];
	int						m_nSelectIndex;
};


class KUiFight : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void 	LoadFightSetting();
	void	SaveFightSetting();
	void	OnCheckInput();

private:
	static KUiFight* m_pSelf;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	SupportAnyOption(int nIndex);
	void	SetSupportAnyOption(int nIndex, int nID);
	void	SupportSkillPopup(int nIndex);
	void	SetSupportSkill(int nIndex, int nID);	
	void	SupportAuraPopup(int nIndex);
	void	SetSupportAura(int nIndex, int nID);
	void	SupportSkillWhenManaSmallPopup(int nIndex);
	void	SetSupportWhenManaSmall(int nIndex, int nID);
	void	SetSupportRightHandPopup(int nIndex);
	void	SetSupportRightHand(int nIndex, int nID);

private: 
	KWndImage			m_BgFight;
	KWndEdit32			e_FightRadiusEdit;			
	KWndButton			c_FightVicinity;
	KWndEdit32			e_FightVicinityEdit;
	KWndButton			c_FightUseSpace;
	KWndButton			c_FightFollowTarget;
	KWndButton			c_FightNormal;
	KWndButton			c_FightPeople;
	KWndText80			t_FightMenu;
	KWndButton			p_FightMenuPopup;
	KWndText80			t_FightMenuSelfDefense;
	KWndButton			p_FightMenuSelfDefensePopup;
	KWndText80			t_FightMenuSuportBoss;
	KWndButton			p_FightMenuSuportBossPopup;
	KWndText80			t_FightMenuSuportSkill01;
	KWndButton			p_FightMenuSuportSkillPopup01;
	KWndText80			t_FightMenuSuportSkill02;
	KWndButton			p_FightMenuSuportSkillPopup02;
	KWndText80			t_FightMenuSuportSkill03;
	KWndButton			p_FightMenuSuportSkillPopup03;
	KWndText80			t_FightMenuSupportAura1;
	KWndButton			p_FightMenuSupportAura1Popup;
	KWndText80			t_FightMenuSupportAura2;
	KWndButton			p_FightMenuSupportAura2Popup;
	KWndText80			t_FightMenuSupportWhenManaSmallText;
	KWndText80			t_FightMenuSupportWhenManaSmall;
	KWndButton			p_FightMenuSupportWhenManaSmall;
	KWndText80			t_FightMenuSupportRightHandText;
	KWndText80			t_FightMenuSupportRightHand;
	KWndButton			p_FightMenuSupportRightHand;
	
	int					m_CurrentSelOption[defMAX_ARRAY_OPTION];
	int					m_CurrentSelMagic[defMAX_ARRAY_STATESKILL];
	int					m_CurrentSelAura[defMAX_ARRAY_AURA];
	int					m_CurrentSelWhenManaSmall[defMAX_ARRAY_FIGHT];
	int					m_CurrentSelRightHand[defMAX_ARRAY_FIGHT];
};

class KUiOther : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void 	LoadOtherSetting();
	void	SaveOtherSetting();
private:
	static KUiOther* m_pSelf;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void 	MenuParty();
	void 	SetMenuParty(int nID);
	void	MenuListParty();
	void 	SetMenuListParty(int nID);
	BOOL 	InsertNameList();
	void 	SetNameListPT();
	void 	MenuJoinParty();
	void 	SetMenuJoinParty(int nID);
private: 
	KWndImage	m_BgOther;
	KWndButton	m_OtherPartyCK;
	KWndText32	m_OtherPartyTxt;
	KWndText80	m_OtherPartyEdit;
	KWndButton	m_OtherPartyPopup;
	KWndText32	m_OtherPartyNameTxt;
	KWndButton	m_OtherPartyNamePopup;
	KWndList2			m_OtherPartyListTxt;
	KWndScrollBar		m_OtherPartyListSbc;
	KWndLabeledButton	m_OtherPartyListAdd;
	KWndLabeledButton	m_OtherPartyListUp;
	KWndLabeledButton	m_OtherPartyListDown;
	KWndLabeledButton	m_OtherPartyListDel;
	KWndLabeledButton	m_OtherPartyListAll;
	KWndButton	m_OtherPartyJoinCK;
	KWndText80	m_OtherPartyJoinTxt;
	KWndButton	m_OtherPartyJoinPopup;	
	KWndButton	m_OtherOptimalNpcCK;
	KWndButton	m_OtherOptimalPlayerCK;	
	KWndButton	m_OtherOptimalAniCK;
	char	m_cNearName[32];
	const char*	m_cNearNameSave;
	BOOL	m_bNamePT;
	char	m_ListName[defMAX_LIST_PARTY][32];
	char	m_ListNameSave[defMAX_LIST_PARTY][32];
	BYTE		m_bSaveMenuPT;
	BYTE		m_bSaveMenuJPT;

};

class KUiMovePlayer : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void 	LoadMoveSetting();
	void	SaveMoveSetting();
private:
	static KUiMovePlayer* m_pSelf;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCheckInput();
	void	PopUpFollowPeople();
	void	SetFollowPeople(int nAction);
	BOOL	InsertMoveMpsList(int nSubWorldId, int nMpsX, int nMpsY);
	void	SetMoveMpsList();
private:
	KWndImage m_BgMove;
	KWndButton	c_MoveFollow;
	KWndText32	t_MoveFollowPeople;
	KWndButton	p_MoveFollowPopup;
	KWndText32	t_MoveFollowPeopleRadius;
	KWndEdit32	e_MoveFollowPeopleRadius;
	KWndButton	c_MoveAround;
	KWndText80	t_MoveAround;
	KWndLabeledButton	c_MoveAroundAdd;

	KWndButton c_MoveCoordiNates;
	KWndList2	t_MoveCoordiNatesList;
	KWndScrollBar	scb_MoveCoordiNatesListScb;
	KWndLabeledButton	c_MoveCoordiNatesAdd;
	KWndLabeledButton	c_MoveCoordiNatesUp;
	KWndLabeledButton	c_MoveCoordiNatesDown;
	KWndLabeledButton	c_MoveCoordiNatesDel;
	KWndLabeledButton	c_MoveCoordiNatesDelAll;

	BOOL		m_MoveAroundBtn;
	int			m_MoveMpsList[defMAX_AUTO_MOVEMPSL][3];
	int			m_MoveMpsListSave[defMAX_AUTO_MOVEMPSL][3];
	char		m_cFollowName[32];	
public:
	KUiSceneTimeInfo	m_CurrentMps;
};

class KUiSupportPlayer : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme(KIniFile* pFile);
	void 	LoadSupportSetting();
	void	SaveSupportSetting(); 
	static void UpdateClientDaTa();
private: 
	static KUiSupportPlayer* m_pSelf;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam); 
	void	OnCheckInput();

private:
	KWndImage			m_BgSupport;
	KWndButton			c_BackToTown;
	KWndEdit32			e_BackToTown;
	KWndButton			c_PlayerSellItem;
	KWndText32			t_PlayerSellItem;
	KWndButton			c_PlayerMoveItem;
	KWndText32			t_PlayerMoveItem;
};

class KUiAutoPlay : public KWndPageSet
{
public:
	static KUiAutoPlay* OpenWindow(bool bShow = true);
	static KUiAutoPlay* GetIfVisible();
	static void		 CloseWindow(bool bDestroy);
	static void		 LoadScheme(const char* pScheme);
	static void		 UpdateData();	
	static void		 UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo);
	static BOOL	SavePrivateSetting();
	static BOOL	LoadPrivateSetting();
private:
	KUiAutoPlay() {}
	~KUiAutoPlay() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateButton(BOOL IsAuto);		
	void	OnAuto(BOOL nIsAuto);	
	void	Breathe();
	BOOL	IsAuto;	
private:
	static KUiAutoPlay* m_pSelf;
private:
	KWndButton	m_ActiveBtn;
	KWndButton	m_PauseBtn;
	BOOL 				m_ActiveCheck;
	KWndLabeledButton	m_CloseBtn;
	KUiRecovery			m_RecoveryPad;
	KUiPick				m_PickPad;
	KUiFight			m_FightPad;
	KUiOther			m_OtherPad;
	KUiMovePlayer		m_MovePlayerPad;
	KUiSupportPlayer		m_SupportPlayerPad;
	KWndLabeledButton	m_RecoveryBtn;
	KWndLabeledButton	m_PickBtn;
	KWndLabeledButton	m_FightBtn;
	KWndLabeledButton	m_MovePlayerBtn;
	KWndLabeledButton	m_SupporPlayerBtn;
	KWndLabeledButton	m_OtherBtn;
};

