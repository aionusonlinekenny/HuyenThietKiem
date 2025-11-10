// KTongSet.cpp: implementation of the CTongSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Global.h"
#include "TongDB.h"
#include "KTongSet.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
CTongSet::CTongSet()
{
	m_pcTong = NULL;
	m_nTongPointSize = 0;
	Init();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
CTongSet::~CTongSet()
{
	DeleteAll();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void	CTongSet::Init()
{
	DeleteAll();

	m_pcTong = (CTongControl**)new LPVOID[defTONG_SET_INIT_POINT_NUM];
	m_nTongPointSize = defTONG_SET_INIT_POINT_NUM;
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		m_pcTong[i] = NULL;
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void	CTongSet::DeleteAll()
{
	if (m_pcTong)
	{	
		int i = 0;
		for (i = 0; i < m_nTongPointSize; i++)
		{
			if (m_pcTong[i])
			{
				delete m_pcTong[i];
				m_pcTong[i] = NULL;
			}
		}
		delete []m_pcTong;
		m_pcTong = NULL;
	}
	m_nTongPointSize = 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int	 CTongSet::Create(int nCamp, const char *lpszPlayerName, const char *lpszTongName, BOOL bGender)
{	
	if (!m_pcTong || m_nTongPointSize <= 0)
		return enumTONG_CREATE_ERROR_ID10;

	if (!lpszPlayerName || !lpszTongName)
		return enumTONG_CREATE_ERROR_ID11;
	
	if (strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH ||
		strlen(lpszPlayerName) > defPLAYER_NAME_MAX_LENGTH)
		return enumTONG_CREATE_ERROR_ID12;

	int		i, nPos;
	DWORD	dwTongNameID, dwPlayerNameID;

	dwTongNameID = g_String2Id(lpszTongName);
	dwPlayerNameID = g_String2Id(lpszPlayerName);

	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && (m_pcTong[i]->m_dwNameID == dwTongNameID || m_pcTong[i]->m_dwMasterID == dwPlayerNameID))
			break;
	}

	if (i < m_nTongPointSize)
		return enumTONG_CREATE_ERROR_ID13;
	
	nPos = -1;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (!m_pcTong[i])
		{
			nPos = i;
			break;
		}
	}
	
	if (nPos < 0)
	{
		CTongControl**	pTemp;
		pTemp = (CTongControl**)new LPVOID[m_nTongPointSize];
		for (i = 0; i < m_nTongPointSize; i++)
			pTemp[i] = m_pcTong[i];

		delete []m_pcTong;
		m_pcTong = NULL;

		m_pcTong = (CTongControl**)new LPVOID[m_nTongPointSize * 2];
		for (i = 0; i < m_nTongPointSize; i++)
			m_pcTong[i] = pTemp[i];
		delete []pTemp;
		m_nTongPointSize *= 2;
		for (i = m_nTongPointSize / 2; i < m_nTongPointSize; i++)
			m_pcTong[i] = NULL;
		nPos = m_nTongPointSize / 2;
	}

	m_pcTong[nPos] = new CTongControl(nCamp, lpszPlayerName, lpszTongName, bGender);		

	if (m_pcTong[nPos]->m_dwNameID == 0)
	{
		delete m_pcTong[nPos];
		m_pcTong[nPos] = NULL;
		return enumTONG_CREATE_ERROR_ID14;
	}
	
	TMemberStruct	sMember;
	sMember.MemberClass = enumTONG_FIGURE_MASTER;
	sMember.nTitleIndex = 0;
	sMember.bGender		= bGender;
	strcpy(sMember.szTong, m_pcTong[nPos]->m_szName);
	strcpy(sMember.szName, m_pcTong[nPos]->m_szMasterName);
	
	g_cTongDB.ChangeTong(*m_pcTong[nPos]);
	g_cTongDB.ChangeMember(sMember);
	
	// 
	char	szMsg[124];
	sprintf(szMsg, "WORLD");
				
	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		sprintf(szMsg, "%s ®· ®øng ra thµnh lËp bang héi %s! Tranh ®o¹t gi÷a c¸c bang ph¸i trong thiªn h¹ cµng thªm gay g¾t!", m_pcTong[nPos]->m_szMasterName, m_pcTong[nPos]->m_szName);
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string("Tin Tøc"), std::string(szMsg));
	}
	
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int		CTongSet::AddMember(char *lpszPlayerName, char *lpszTongName, BOOL bGender)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return -1;
	if (!lpszPlayerName || !lpszPlayerName[0] || !lpszTongName || !lpszTongName[0])
		return -1;
	if (strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH ||
		strlen(lpszPlayerName) > defPLAYER_NAME_MAX_LENGTH)
		return -1;

	int		i;
	DWORD	dwTongNameID;

	dwTongNameID = g_String2Id(lpszTongName);

	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && (m_pcTong[i]->m_dwNameID == dwTongNameID))
		{
			if (!m_pcTong[i]->AddMember(lpszPlayerName, bGender))
				return -1;
			else
			{
				TMemberStruct	sMember;
				sMember.MemberClass = enumTONG_FIGURE_MEMBER;
				sMember.nTitleIndex = 0;
				sMember.bGender		= bGender;
				strcpy(sMember.szTong, m_pcTong[i]->m_szName);
				strcpy(sMember.szName, lpszPlayerName);
				g_cTongDB.ChangeMember(sMember);

				char	szMsg[101];
				sprintf(szMsg, "\\O%u", m_pcTong[i]->m_dwNameID);		

				DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
				if (channid != -1)
				{
					sprintf(szMsg, "%s võa gia nhËp! Bang héi h­ng thÞnh gÊp béi! Anh em c¹n chÐn mõng thµnh viªn míi nµo!", lpszPlayerName);		
					g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_pcTong[i]->m_szMasterName), std::string(szMsg));		
				}

				return i;
			}
		}
	}

	return -1;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
int		CTongSet::GetTongCamp(int nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return -1;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return -1;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_btCamp;

	return -1;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
BOOL CTongSet::GetMasterName(int nTongIdx, char *lpszName)
{
	if (!lpszName)
		return FALSE;
	if (!m_pcTong || m_nTongPointSize <= 0)
		return FALSE;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return FALSE;
	if (!m_pcTong[nTongIdx])
		return FALSE;
	strcpy(lpszName, m_pcTong[nTongIdx]->m_szMasterName);
	return TRUE;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
BOOL CTongSet::GetMemberTitle(int nTongIdx, char *lpszTitle, BOOL bGender)
{
	if (!lpszTitle)
		return FALSE;
	if (!m_pcTong || m_nTongPointSize <= 0)
		return FALSE;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return FALSE;
	if (!m_pcTong[nTongIdx])
		return FALSE;
	if(bGender)
		strcpy(lpszTitle, m_pcTong[nTongIdx]->m_szFemaleTitle);
	else
		strcpy(lpszTitle, m_pcTong[nTongIdx]->m_szMaleTitle);
	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::GetTongHeadInfo(DWORD dwTongNameID, STONG_HEAD_INFO_SYNC *pInfo)
{
	if (!m_pcTong || m_nTongPointSize <= 0 || dwTongNameID == 0)
		return FALSE;

	for (int i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == dwTongNameID)
		{
			return m_pcTong[i]->GetTongHeadInfo(pInfo);
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongSet::GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND *pApply, STONG_MANAGER_INFO_SYNC *pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (!m_pcTong)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == (DWORD)pApply->m_nParam1)
		{
			return m_pcTong[i]->GetTongManagerInfo(pApply, pInfo);
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongSet::GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND *pApply, STONG_MEMBER_INFO_SYNC *pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == (DWORD)pApply->m_nParam1)
		{
			return m_pcTong[i]->GetTongMemberInfo(pApply, pInfo);
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync)
{
	if (!pInstate || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pInstate->m_dwTongNameID)
		{
			return m_pcTong[i]->Instate(pInstate, pSync);
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync)
{
	if (!pKick || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pKick->m_dwTongNameID)
		{
			return m_pcTong[i]->Kick(pKick, pSync);
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync)
{
	if (!pLeave || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pLeave->m_dwTongNameID)
		{
			return m_pcTong[i]->Leave(pLeave, pSync);
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept)
{
	if (!pAccept)
		return FALSE;
	if (!m_pcTong)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pAccept->m_dwTongNameID)
		{
			return m_pcTong[i]->AcceptMaster(pAccept);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------
BOOL CTongSet::ChangeCamp(STONG_CHANGE_CAMP_COMMAND* pChange, STONG_CHANGE_CAMP_SYNC* sChange)
{
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;
	if(!m_pcTong)
		return FALSE;
	
	int i = 0;
	for(i = 0; i < m_nTongPointSize; i++)
	{
		if(m_pcTong[i] && m_pcTong[i]->m_dwNameID == pChange->m_dwTongNameID)
		{
			return m_pcTong[i]->ChangeCamp(pChange, sChange);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------
BOOL CTongSet::ChangeMemberTitle(STONG_CHANGE_MEMBER_TITLE_COMMAND* pChange, STONG_CHANGE_MEMBER_TITLE_SYNC* sChange)
{
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;
	if(!m_pcTong)
		return FALSE;
	
	int i = 0;
	for(i = 0; i < m_nTongPointSize; i++)
	{
		if(m_pcTong[i] && m_pcTong[i]->m_dwNameID == pChange->m_dwTongNameID)
		{
			return m_pcTong[i]->ChangeMemberTitle(pChange, sChange);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------
BOOL CTongSet::ChangeOneTitle(STONG_CHANGE_ONE_TITLE_COMMAND* pChange, STONG_CHANGE_ONE_TITLE_SYNC* sChange)
{
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;
	if(!m_pcTong)
		return FALSE;
	
	int i = 0;
	for(i = 0; i < m_nTongPointSize; i++)
	{
		if(m_pcTong[i] && m_pcTong[i]->m_dwNameID == pChange->m_dwTongNameID)
		{
			return m_pcTong[i]->ChangeOneTitle(pChange, sChange);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------
BOOL CTongSet::SaveMoney(STONG_SAVE_MONEY_COMMAND* pChange, STONG_SAVE_MONEY_SYNC* sChange)
{
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;
	if(!m_pcTong)
		return FALSE;

	int i = 0;
	for(i = 0; i < m_nTongPointSize; i++)
	{
		if(m_pcTong[i] && m_pcTong[i]->m_dwNameID == pChange->m_dwTongNameID)
		{
			return m_pcTong[i]->SaveMoney(pChange, sChange);
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------
BOOL CTongSet::GetMoney(STONG_GET_MONEY_COMMAND* pChange, STONG_GET_MONEY_SYNC* sChange)
{
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;
	if(!m_pcTong)
		return FALSE;

	int i = 0;
	for(i = 0; i < m_nTongPointSize; i++)
	{
		if(m_pcTong[i] && m_pcTong[i]->m_dwNameID == pChange->m_dwTongNameID)
		{
			return m_pcTong[i]->GetMoney(pChange, sChange);
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::InitFromDB()
{	
	int		nTongNum;

	nTongNum = g_cTongDB.GetTongCount();
	if (nTongNum < 0)
		return FALSE;
	if (nTongNum == 0)
		return TRUE;
	
	if(nTongNum >= m_nTongPointSize)		
	{
		DeleteAll();
		m_pcTong = (CTongControl**)new LPVOID[nTongNum + defTONG_SET_INIT_POINT_NUM];
		m_nTongPointSize = nTongNum + defTONG_SET_INIT_POINT_NUM;
	}
	int	 i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		m_pcTong[i] = NULL;
	}
	TTongList	*pList = new TTongList[m_nTongPointSize];
	memset(pList, 0, sizeof(TTongList) * m_nTongPointSize);
	int nGetNum = g_cTongDB.GetTongList(pList, m_nTongPointSize);

	if (nGetNum <= 0)
		return TRUE;

	for (i = 0; i < nGetNum; i++)
	{
		m_pcTong[i] = new CTongControl(pList[i]);
		if (!m_pcTong[i]->m_szName[0])
		{
			delete m_pcTong[i];
			m_pcTong[i] = NULL;
		}
	}

	for (i = 0; i < nGetNum; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_szName[0])
			g_cTongDB.SearchTong(m_pcTong[i]->m_szName, *(m_pcTong[i]));
	}

	if (pList)
	{
		delete []pList;
		pList = NULL; //them code 16/09/2023 by kinnox;
	}

	return TRUE;
}

//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
void	CTongSet::SaveTong(DWORD dwTongNameID)
{	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && (m_pcTong[i]->m_dwNameID == dwTongNameID))
		{	
			g_cTongDB.ChangeTong(*m_pcTong[i]);
			break;
		}
	}
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync)
{
	if (!pLogin || !pSync)
		return FALSE;

	memset(pSync, 0, sizeof(STONG_LOGIN_DATA_SYNC));
	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_LOGIN_DATA;
	pSync->m_btFlag			= 0;
	pSync->m_dwParam		= pLogin->m_dwParam;

	if (!m_pcTong)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pLogin->m_dwTongNameID)
		{
			return m_pcTong[i]->GetLoginData(pLogin, pSync);
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongSet::SearchOne(DWORD dwTongNameID, char *lpszName, STONG_ONE_LEADER_INFO *pInfo)
{
	if (!m_pcTong)
		return FALSE;
	if (dwTongNameID == 0 || !lpszName || !lpszName[0] || !pInfo)
		return FALSE;
	
	int i = 0;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == dwTongNameID)
		{
			return m_pcTong[i]->SearchOne(lpszName, pInfo);
		}
	}

	return FALSE;
}









