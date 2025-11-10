// KTongSet.h: interface for the CTongSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTONGSET_H__B42782F1_FA08_4D1C_A209_1ED1F5E0BAA3__INCLUDED_)
#define AFX_KTONGSET_H__B42782F1_FA08_4D1C_A209_1ED1F5E0BAA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"KTongControl.h"

#define		defTONG_SET_INIT_POINT_NUM		16

class CTongSet
{
public:
	CTongSet();
	virtual ~CTongSet();

private:
	CTongControl**	m_pcTong;
	int				m_nTongPointSize;

public:
	void			Init();
	void			DeleteAll();
	BOOL			InitFromDB();
	void			SaveTong(DWORD dwTongNameID);		

	BOOL			SearchOne(DWORD dwTongNameID, char *lpszName, STONG_ONE_LEADER_INFO *pInfo);

	int				Create(int nCamp, const char *lpszPlayerName, const char *lpszTongName, BOOL bGender);

	int				AddMember(char *lpszPlayerName, char *lpszTongName, BOOL bGender);

	int				GetTongCamp(int nTongIdx);

	BOOL			GetMasterName(int nTongIdx, char *lpszName);

	BOOL			GetMemberTitle(int nTongIdx, char *lpszTitle, BOOL bGender);

	BOOL			GetTongHeadInfo(DWORD dwTongNameID, STONG_HEAD_INFO_SYNC *pInfo);

	BOOL			GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND *pApply, STONG_MANAGER_INFO_SYNC *pInfo);

	BOOL			GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND *pApply, STONG_MEMBER_INFO_SYNC *pInfo);

	BOOL			Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync);

	BOOL			Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync);

	BOOL			Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync);

	BOOL			AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept);
	
	
	BOOL			ChangeCamp(STONG_CHANGE_CAMP_COMMAND* pChange, STONG_CHANGE_CAMP_SYNC* pSync);

	BOOL			ChangeMemberTitle(STONG_CHANGE_MEMBER_TITLE_COMMAND* pChange, STONG_CHANGE_MEMBER_TITLE_SYNC* sChange);

	BOOL			ChangeOneTitle(STONG_CHANGE_ONE_TITLE_COMMAND* pChange, STONG_CHANGE_ONE_TITLE_SYNC* sChange);
	
	BOOL			SaveMoney(STONG_SAVE_MONEY_COMMAND* pChange, STONG_SAVE_MONEY_SYNC* sChange);

	BOOL			GetMoney(STONG_GET_MONEY_COMMAND* pChange, STONG_GET_MONEY_SYNC* sChange);
	
	
	BOOL			GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync);	
};

#endif // !defined(AFX_KTONGSET_H__B42782F1_FA08_4D1C_A209_1ED1F5E0BAA3__INCLUDED_)
