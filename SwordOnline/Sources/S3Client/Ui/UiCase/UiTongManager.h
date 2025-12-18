
#if !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
#define AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndlabeledbutton.h"
#include "../elem/wndscrollbar.h"
#include "../elem/wndimage.h"
#include "../elem/wndlist2.h"
#include "../Elem/WndPage.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndMessageListBox.h"
#include "../../Engine/Src/LinkStruct.h"
#include "../../Core/Src/GameDataDef.h"

struct KPilgarlicItem : public KTongMemberItem
{
	int nHashID;
	KPilgarlicItem *next;
    KPilgarlicItem *previous;
};

//================================================================================================================

#define		defMAX_LINE_IN_ONE_PAGE		25

class KUiTongInfo : public KWndPage
{
public:
	KUiTongInfo();
	void	Initialize();
	void	Clear();
	void	InfoArrive(KTongInfo *pTongInfo);
	void	UpdateListView(BYTE btPage = 1);

private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void    UpdateInfoView(KTongInfo* pTongInfo);
	void	OnShowOnline(BOOL bShow);
	void	OnPrevPage();
	void	OnNextPage();
	void	OnJumpPage();
	void	OnCheckInput();
	
private:
	KWndText32				m_TitleTongInfo, m_TitlePersonalInfo, m_TxtHelpTitle, m_TxtHelp, m_TxtRank;
	KWndText32				m_TxtTitle, m_TxtType;

	KWndEdit32				m_EditBoxDestPage;

	KWndLabeledButton		m_TitleTongName, m_TitleMaster, m_TitleLeague, m_TitleCamp, m_TitleBuildLevel;
	KWndLabeledButton		m_TitleMemberNum, m_TitleTongLevel, m_TitleStandFund, m_TitleTongCapital, m_TitleBuildFund;
	KWndLabeledButton		m_TitleBattleFund, m_TitleTotalOffer, m_TitleStoredBuildFund, m_TitlePersonalOffer, m_TitleWeeklyOffer;
	KWndLabeledButton		m_BtnPrevPage, m_BtnNextPage, m_BtnJump, m_BtnOnlinePriority;

	KWndRichLabeledButton	m_TxtTongName, m_TxtMaster, m_TxtLeague, m_TxtCamp, m_TxtBuildLevel;
	KWndRichLabeledButton	m_TxtMemberNum, m_TxtTongLevel, m_TxtStandFund, m_TxtTongCapital, m_TxtBuildFund;
	KWndRichLabeledButton	m_TxtBattleFund, m_TxtTotalOffer, m_TxtStoredBuildFund, m_TxtPersonalOffer, m_TxtWeeklyOffer;
	
	KWndList2				m_MemberList;

private:
	char		m_szJustice[16], m_szBalance[16], m_szEvil[16];

	BYTE		m_btCurrentPage, m_btPageCount;  
	bool		m_bShowOnline;
	
};

//================================================================================================================

class KUiTongFunUseSubPage : public KWndPage
{
public:
	KUiTongFunUseSubPage();
	void	Initialize(int nSub);
private:
	void	LoadScheme(const char* pScheme, int nSub);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	
};

//=============================================

class KUiTongFunUseSub : public KWndPageSet
{
public:
	KUiTongFunUseSub();
	void	Initialize();
	void	Clear();
	void	ArrangeComposition(const char* pszPlayerName, int nIndex);
	void	InfoArrive(KTongInfo *pTongInfo, int nRelation);
	void	UpdateListView(BYTE btPage = 1);

private:
	enum	SUB_PAGES
	{
		SUB_MEMBER = 0,
		SUB_TONG,
		SUB_UNION,
		SUB_OTHER,
	};

private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void    UpdateBtnTheme(int nType);
	void	UpdateSelectBtn();
	void    UpdateInfoView(KTongInfo* pTongInfo);
	void	OnShowOnline(BOOL bShow);
	void	OnPrevPage();
	void	OnNextPage();
	void	OnJumpPage();
	void	OnCheckInput();
	void	ShowSubPage(SUB_PAGES eSub);
	void	PopupConfirmWindow(unsigned int uHandleID);
	void	OnAction(int nType, int nData);
	void	OnChangeTitle(int nType, const char* lpszTitle);
	void	OnActionMoney(int nType, int nMoney);

private:
	KPilgarlicItem*		GetSelectedPlayer();


private:
	KWndText32				m_TitleTongInfo, m_TxtPersonalInfo, m_TxtRank, m_TxtTitle;
	KWndText32				m_TxtType;

	KWndEdit32				m_EditBoxDestPage;

	KWndLabeledButton		m_TitleTongName, m_TitleTongUnion, m_TitleBuildLevel, m_TitleTotalOffer, m_TitleTongMoney;
	KWndLabeledButton		m_TitleBuildFund, m_TitleBattleFund, m_TitleStoredBuildFund, m_TitlePersonalOffer, m_TitleTongMoney2;
	KWndLabeledButton		m_TitleBuildFund2;

	KWndLabeledButton		m_BtnUpgradeBuildLevel, m_BtnAssignTongOffer, m_BtnTransformMoney;
	KWndLabeledButton		m_BtnTransformBuildFund, m_BtnStorePersonalOffer, m_BtnStoreBuildFund, m_BtnStoreTongMoney, m_BtnLeaveTong;
	KWndLabeledButton		m_BtnForceToRetire, m_BtnDemise, m_BtnKickOut, m_BtnDepose;
	KWndLabeledButton		m_Btn_DispenseOffer, m_BtnChangeMaleTitle, m_BtnChangeFemaleTitle, m_BtnChangeTitle, m_BtnGetTongMoney, m_BtnAssignTongMoney;
	KWndLabeledButton		m_BtnChangeCamp, m_BtnCreateTongMap, m_BtnConfigureTongMap, m_BtnTongStunt, m_BtnTongChallenge;
	KWndLabeledButton		m_BtnTongDetect, m_BtnCreateUnion, m_BtnApplyJionUnion, m_BtnAcceptUnionReq, m_BtnLeaveUnion;
	KWndLabeledButton		m_BtnKickUnionTong;
	KWndLabeledButton		m_BtnRetire, m_BtnPrevPage, m_BtnNextPage, m_BtnJump, m_BtnOnlinePriority;

	KWndRichLabeledButton	m_TxtTongName, m_TxtTongUnion, m_TxtBuildLevel, m_TxtTotalOffer, m_TxtTongMoney;
	KWndRichLabeledButton	m_TxtBuildFund, m_TxtBattleFund, m_TxtStoredBuildFund, m_TxtPersonalOffer, m_TxtTongMoney2;
	KWndRichLabeledButton	m_TxtBuildFund2;

	KUiTongFunUseSubPage	m_SubPages[4];
	KWndButton				m_BtnSubPages[4];
	
	KWndList2				m_MemberList;

private:
	enum RESULT_TYPE
	{
		RESULT_T_KICKOUT,
		RESULT_T_ASSIGN,
		RESULT_T_DEMISE,
		RESULT_T_LEAVE,
		RESULT_T_MALE_TITLE,
		RESULT_T_FEMALE_TITLE,
		RESULT_T_TITLE,
		RESULT_T_SAVE_MONEY,
		RESULT_T_GET_MONEY,
		RESULT_T_SND_MONEY,
		RESULT_T_INFO,
	};

private:
	int         m_nRelation;
	int			m_nIndex;
	char		m_szPlayerName[32];

	char		m_szMemberKick[80], m_szMemberAssign[80], m_szMemberDemise[80], m_szSelfLeaveTong[80];

	BYTE		m_btCurrentPage, m_btPageCount;  
	bool		m_bShowOnline;

	int			m_nSelectIndex;
	char		m_szTargetPlayerName[32];        
};

//==================================================

class KUiTongFunUse : public KWndPage
{
public:
	KUiTongFunUse();
	void	Initialize();
	void	Clear();
	void	ArrangeComposition(const char* pszPlayerName, int nIndex);
	void	InfoArrive(KTongInfo *pTongInfo, int nRelation);
	void	UpdateListView(BYTE btPage = 1);

private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	KUiTongFunUseSub	m_InternalPad;
};

//================================================================================================================

#define			defMAX_RIGHT_BTN		14

class KUiTongRightManage : public KWndPage
{
public:
	KUiTongRightManage();
	void	Initialize();
	void	Clear();
	void	UpdateListView();

private:
	enum	RIGHT_PAGE
	{
		SUB_ONE = 0,
		SUB_TWO,
	};

private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	ShowSubPage(RIGHT_PAGE eSub);
	void	OnSelectAll(BOOL bSel);

private:
	KWndLabeledButton		m_BtnSubPages[2];
	KWndLabeledButton		m_BtnRightManage[defMAX_RIGHT_BTN];
	KWndLabeledButton		m_BtnSelectAll, m_BtnDistribute;

	KWndText32				m_TxtRank, m_TxtTitle, m_TxtType;

	KWndList2				m_MemberList;

	KWndLabeledButton		m_BtnOnlinePriority;

private:
	struct	RIGHT_BTN
	{	
		BYTE	btSubPage;
		short	nRightID;
		char szSection[32];
	};

	RIGHT_BTN	m_sRightBtn[defMAX_RIGHT_BTN];
	char	m_szPromptInfo[64];
};

//================================================================================================================

class KUiTongRecord : public KWndPage
{
public:
	KUiTongRecord();
	void	Initialize();

private:
	enum	RECORD_LIST
	{
		LIST_WEEKDAILY = 0,
		LIST_ANNOUNCE,
		LIST_AFFAIR,
		LIST_HISTORY,
	};

private:
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	ShowList(RECORD_LIST eList);

private:
	KWndLabeledButton	m_BtnWeekDaily, m_BtnAnnounce, m_BtnTongAffair, m_BtnTongHistory;

	KScrollMessageListBox		m_RecordList;
	KWndEdit32			m_AnnounceEditor;
	KWndScrollBar		m_EditorScroll;

	KWndButton			m_BtnLeaveWord, m_BtnEditAnnounce;
};

//================================================================================================================

class KUiTongManager : KWndPageSet
{
public:
	KUiTongManager();
	virtual ~KUiTongManager();

	static        KUiTongManager* OpenWindow();
	static        KUiTongManager* OpenWindow(char* pszPlayerName);
	static        KUiTongManager* GetIfVisible();
	static void   CloseWindow(bool bDestory = false);
	static void   LoadScheme(const char* pScheme);

	static void		TongInfoArrive(KUiPlayerRelationWithOther* Relation, KTongInfo *pTongInfo);
	static void		NewDataArrive(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);
	static void		LoadData();
	static int		LoadDataHandler(KLinkStruct<KPilgarlicItem> *pData, const char* szSection);
	static KLinkStruct<KPilgarlicItem>*	GetMemberData();	
	static KLinkStruct<KPilgarlicItem>*	FindPlayer(char *pszName);
	
	static void		ResponseResult(KUiGameObjectWithName *pResult, int nbIsSucceed);
	static int		RemoveTempFile();

	static void		LockOperation();
	static bool		GetLockOperation();
	static void		LockInfomation();
	
public:
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void          ArrangeComposition(char* pszPlayerName);
	void          ClearTongData();
	
	void          ResponseLeave(KUiGameObjectWithName *pResult, int nbIsSucceed);
	void          ResponseDemise(KUiGameObjectWithName *pResult, int nbIsSucceed);
	void          ResponseApply(KUiGameObjectWithName *pResult, int nbIsSucceed);
	void          ResponseKickOut(KUiGameObjectWithName *pResult, int nbIsSucceed);
	void          ResponseAssign(KUiGameObjectWithName *pResult, int nbIsSucceed);
	


private:
	enum	TONG_PAGE
	{
		Info_Page = 0,
		ZhaoMu_Page,
		OtherZhaoMu_Page,
		Workshop_Page,

		FunUse_Page,
		RightManage_Page,
		TongRecord_Page,
		Tong_PageNum,
	};

private:
	void			Initialize();
	void			Breathe();	
	void			ShowPage(TONG_PAGE ePage);
	void			UpdateBtnTheme(int nType, BOOL IsDissable = FALSE);
	void			OnRefresh();

private:
	static void		RequestData(int nType, int nIndex, int nNeedNum);
	static void		SaveNewData(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);
	static void		SaveWholeData(int nType);
	KLinkStruct<KPilgarlicItem>*	HandleFind(KLinkStruct<KPilgarlicItem> *pData, char *pszName);

private:
	static			KUiTongManager *ms_pSelf;

private:
	KUiTongInfo		m_BaseInfoPad;
	KUiTongFunUse	m_FunUsePad;
	KUiTongRightManage		m_RightManagePad;
	KUiTongRecord			m_RecordPad;
	KWndButton		m_BtnBaseInfo, m_BtnZhaoMu, m_BtnOtherZhaoMu, m_BtnWorkshop, m_BtnFunUse, m_BtnRightManage, m_BtnTongRecord, m_BtnTongHelp;
	KWndButton		m_BtnEnterMap, m_BtnTongList, m_BtnRefresh, m_BtnClose;

private:
	KLinkStruct<KPilgarlicItem>	m_MemberData;

	static KTongInfo            m_TongData;

private:
	static int    m_nElderDataIndex;
	static int    m_nMemberDataIndex;
	static int    m_nCaptainDataIndex;

private:
	BYTE		m_btCurPad;
	unsigned int	m_uLockTime;
	bool		m_bLockOperation;
	int			m_nIndex;
	char		m_szPlayerName[defPLAYER_NAME_MAX_LENGTH + 1];
};

unsigned long StringToHash(const char *pString, BOOL bIsCaseSensitive = TRUE);

#endif // !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
