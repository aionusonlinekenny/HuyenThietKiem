
#ifndef KPLAYERTONG_H
#define KPLAYERTONG_H

class KPlayerTong
{
	friend class KPlayer;
private:
	int			m_nPlayerIndex;
	int			m_nFlag;			
	int			m_nFigure;			
	int			m_nCamp;			
	DWORD		m_dwTongNameID;
	char		m_szName[defTONG_NAME_MAX_LENGTH + 1];		
	char		m_szTitle[defTONG_TITLE_MAX_LENGTH + 1];		
	int			m_nApplyTo;
	
#ifdef _SERVER
	DWORD		m_dwMoney;
#endif

public:
	char		m_szMasterName[defPLAYER_NAME_MAX_LENGTH + 1];

private:
	BOOL		CheckAcceptAddApplyCondition();

public:
	void		Init(int nPlayerIdx);
	void		Clear();
	void		GetTongName(char *lpszGetName);
	void		GetTongTitle(char* lpszGetTitle);		
	DWORD		GetTongNameID();
	void		SetTongNameID(DWORD dwID) { m_dwTongNameID = dwID; };
	int			CheckIn() {return m_nFlag;};
	int			GetCamp() {return m_nCamp;};
	int			GetFigure() {return m_nFigure;};
	BOOL		CanGetManagerInfo(DWORD dwTongNameID);
	BOOL		CanGetMemberInfo(DWORD dwTongNameID);

#ifndef _SERVER
	BOOL		ApplyCreateTong(int nCamp, const char *lpszTongName);
	void		Create(TONG_CREATE_SYNC *psCreate);
	BOOL		ApplyAddTong(DWORD dwNpcID);
	void		AcceptMember(int nPlayerIdx, DWORD dwNameID, BOOL bFlag);
	BOOL		AddTong(int nCamp, char *lpszTongName, char *lpszTitle, char *lpszMaster);
	BOOL		ApplyInstate(int nCurFigure, int nCurPos, int nNewFigure, int nNewPos, char *lpszName);
	BOOL		ApplyKick(int nCurFigure, int nCurPos, char *lpszName);
	BOOL		ApplyChangeMaster(int nCurFigure, int nPos, char *lpszName);
	BOOL		ApplyLeave();
	BOOL		ApplyInfo(int nInfoID, int nParam1, int nParam2, int nParam3, char *lpszName = NULL);
	
	BOOL		ApplyChangeCamp(int nCamp);
	BOOL		ApplyChangeMemberTitle(const char* lpszTitle, BOOL bGender);
	BOOL		ApplyChangeOneTitle(const char* lpszName, BYTE bFigureFlag, const char* lpszTitle);
	BOOL		ApplySaveMoney(int nMoney);
	BOOL		ApplyGetMoney(int nMoney);
	
	void		SetSelfInfo(TONG_SELF_INFO_SYNC *pInfo);
	void        OpenCreateInterface();
	void		SendSystemMsg(const char* lpszMsg);
#endif

#ifdef _SERVER
	int			CheckCreateCondition(int nCamp, char *lpszTongName);
	BOOL		Create(int nCamp, const char *lpszTongName);
	BOOL		GetOpenFlag();
	BOOL		TransferAddApply(DWORD dwNpcID);
	void		SendRefuseMessage(int nPlayerIdx, DWORD dwNameID);
	BOOL		CheckAddCondition(int nPlayerIdx);
	BOOL		AddTong(int nCamp, char *lpszTongName, char *lpszMasterName, char *lpszTitleName);
	void		SendSelfInfo();
	BOOL		CheckInstatePower(TONG_APPLY_INSTATE_COMMAND *pApply);
	void		BeInstated(STONG_SERVER_TO_CORE_BE_INSTATED *pSync);
	BOOL		CheckKickPower(TONG_APPLY_KICK_COMMAND *pKick);
	void		BeKicked(STONG_SERVER_TO_CORE_BE_KICKED *pSync);
	BOOL		CheckLeavePower(TONG_APPLY_LEAVE_COMMAND *pLeave);
	void		Leave(STONG_SERVER_TO_CORE_LEAVE *pLeave);
	BOOL		CheckChangeMasterPower(TONG_APPLY_CHANGE_MASTER_COMMAND *pChange);
	BOOL		CheckGetMasterPower(STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER *pCheck);
	void		ChangeAs(STONG_SERVER_TO_CORE_CHANGE_AS *pAs);
	void		ChangeMaster(char *lpszMaster);
	
	BOOL		CheckChangeCampPower(TONG_CHANGE_CAMP_COMMAND* pChange);
	void		ChangeCamp(STONG_SERVER_TO_CORE_CHANGE_CAMP* pChange);
	BOOL		CheckChangeMemberTitlePower(TONG_CHANGE_MEMBER_TITLE_COMMAND* pChange);
	BOOL		CheckChangeOneTitlePower(TONG_CHANGE_ONE_TITLE_COMMAND* pChange);
	void		ChangeTitle(const char *lpszTitle);
	BOOL		CheckSaveMoneyPower(TONG_APPLY_SAVE_MONEY_COMMAND* pChange);
	void		SaveMoney(STONG_SERVER_TO_CORE_SAVE_MONEY* pSave);
	BOOL		CheckGetMoneyPower(TONG_APPLY_GET_MONEY_COMMAND* pChange);
	void		GetMoney(STONG_SERVER_TO_CORE_GET_MONEY* pGet, bool bSelf = true);
	
	void		Login(STONG_SERVER_TO_CORE_LOGIN *pLogin);
	void		DBSetTongNameID(DWORD dwID) { m_dwTongNameID = dwID; if (dwID) m_nFlag = 1;};
#endif
};
#endif
