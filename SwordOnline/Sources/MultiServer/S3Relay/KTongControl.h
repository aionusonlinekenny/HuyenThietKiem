
#if !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
#define AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct STONG_MEMBER
{
	DWORD	m_dwNameID;
	BYTE	m_bGender;
	char	m_szName[defPLAYER_NAME_MAX_LENGTH + 1];
};

typedef struct
{
	int			MemberCount;
	char		szName[defTONG_NAME_MAX_LENGTH + 1];
}TTongList;

class CTongControl
{
	friend class CTongSet;
	friend class CTongDB;
public:
	CTongControl(int nCamp, const char *lpszPlayerName, const char *lpszTongName, BOOL bGender);		//
	CTongControl(TTongList sList);
	virtual ~CTongControl();

private:
	BYTE		m_btCamp;
	BYTE		m_btLevel;
	DWORD		m_dwMoney;
	
	int			m_nDirectorNum;							
	int			m_nManagerNum;							
	int			m_nMemberNum;							

	DWORD		m_dwNameID;								
	char		m_szName[defTONG_NAME_MAX_LENGTH + 1];				

	char		m_szMasterTitle[defTONG_TITLE_MAX_LENGTH + 1];	
	char		m_szDirectorTitle[defTONG_MAX_DIRECTOR][defTONG_TITLE_MAX_LENGTH + 1];
	char		m_szManagerTitle[defTONG_MAX_MANAGER][defTONG_TITLE_MAX_LENGTH + 1];
	
	char		m_szMaleTitle[defTONG_TITLE_MAX_LENGTH + 1];
	char		m_szFemaleTitle[defTONG_TITLE_MAX_LENGTH + 1];
	

	DWORD		m_dwMasterID;							
	char		m_szMasterName[defPLAYER_NAME_MAX_LENGTH + 1];
	BYTE		m_bMasterGender;

	DWORD		m_dwDirectorID[defTONG_MAX_DIRECTOR];	
	char		m_szDirectorName[defTONG_MAX_DIRECTOR][defPLAYER_NAME_MAX_LENGTH + 1];
	BYTE		m_bDirectorGender[defTONG_MAX_DIRECTOR];

	DWORD		m_dwManagerID[defTONG_MAX_MANAGER];
	char		m_szManagerName[defTONG_MAX_MANAGER][defPLAYER_NAME_MAX_LENGTH + 1];
	BYTE		m_bManagerGender[defTONG_MAX_MANAGER];

	STONG_MEMBER	*m_psMember;
	int				m_nMemberPointSize;

	bool		m_bLockOperation;

public:
	BOOL		SearchOne(char *lpszName, STONG_ONE_LEADER_INFO *pInfo);

	BOOL		AddMember(char *lpszPlayerName, BOOL bGender = FALSE);

	BOOL		ChangePassword(char *lpOld, char *lpNew);

	BOOL		GetTongHeadInfo(STONG_HEAD_INFO_SYNC *pInfo);

	BOOL		GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND *pApply, STONG_MANAGER_INFO_SYNC *pInfo);

	BOOL		GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND *pApply, STONG_MEMBER_INFO_SYNC *pInfo);

	BOOL		Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync);

	BOOL		Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync);

	BOOL		Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync);

	BOOL		AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept);
	
	
	BOOL		ChangeCamp(STONG_CHANGE_CAMP_COMMAND* pChange, STONG_CHANGE_CAMP_SYNC* sChange);

	BOOL		ChangeMemberTitle(STONG_CHANGE_MEMBER_TITLE_COMMAND *pChange, STONG_CHANGE_MEMBER_TITLE_SYNC *sChange);

	BOOL		ChangeOneTitle(STONG_CHANGE_ONE_TITLE_COMMAND* pChange, STONG_CHANGE_ONE_TITLE_SYNC* sChange);

	BOOL		SaveMoney(STONG_SAVE_MONEY_COMMAND* pChange, STONG_SAVE_MONEY_SYNC* sChange);

	BOOL		GetMoney(STONG_GET_MONEY_COMMAND* pChange, STONG_GET_MONEY_SYNC* sChange);
	
	BOOL		GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync);
};

#endif // !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
