#pragma once

#include "../../NetConnect/NetConnectAgent.h"

#ifndef MAX_PLAYER_PER_ACCOUNT
	#define	MAX_PLAYER_PER_ACCOUNT	3
#endif

//=====================================
//
//=====================================
enum SSC_STATUS
{
	SSC_S_IDLE	= 0,
	SSC_S_LOADING_DATA,
	SSC_S_STANDBY,
	SSC_S_LOADING_CHARACTER,
	SSC_S_LOAD_CHARACTER_LOADED,
	SSC_S_CREATING_CHARACTER,
	SSC_S_DELETING_CHARACTER,
};

//=====================================
//
//=====================================
enum SSC_RESULT
{
	SSC_R_IN_PROGRESS = 0,
	SSC_R_NOTHING,
	SSC_R_UPDATE,
	SSC_R_INVALID_ROLENAME,
	SSC_R_CREATE_ROLE_SUCCEED,
	SSC_R_START_GAME,
	SSC_R_SVR_DOWN,
	SSC_R_FAILED,
};

//=====================================
//
//=====================================
struct KNewCharacterInfo
{
	char				Name[32];
	unsigned	char	Gender;
	unsigned	char	Attribute;
	union
	{
		unsigned short	NativePlaceId;
		short			nLevel;
	};
};

class KSelSavedCharacter
{
public:
	KSelSavedCharacter();
	~KSelSavedCharacter();
	void	AcceptNetMsg(void* pMsgData);
	int		LoadData();
	int		GetStatus();
	void	SetLastActionResult(int nResult);
	int		GetLastActionResult();
	int		GetCharacterNum();
	int		GetCharacterInfo(int nIndex, KNewCharacterInfo* pInfo);
	int		NewCharacter(KNewCharacterInfo* pData);
	int		DeleteCharacter(int nIndex, const char* pszPassword);
	int		SelCharacter(int nIndex);
	void	FreeData();
	void	SetCharacterBaseInfo(int nNum, const RoleBaseInfo *pInfo);
	void	SetAccountName(const char* pAccount);
private:
	SSC_STATUS			m_Status;
	RoleBaseInfo		m_BaseInfo[MAX_PLAYER_PER_ACCOUNT];
	int					m_nNumCharacter;
	unsigned int		m_nRequestTime;
	int					m_nLastOperResult;
	char				m_AccountName[32];
	char				m_szProcessingRoleName[32];
};
