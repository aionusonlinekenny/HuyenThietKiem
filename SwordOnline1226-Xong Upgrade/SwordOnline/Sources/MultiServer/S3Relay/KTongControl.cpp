#include "stdafx.h"
#include "Global.h"
#include "KTongControl.h"

#define		defTONG_INIT_MEMBER_SIZE		100
#define		defTONG_MEMBER_SIZE_ADD			100

CTongControl::CTongControl(int nCamp, const char *lpszPlayerName, const char *lpszTongName, BOOL bGender)
{
	m_btCamp		= 0;
	m_btLevel		= 1;
	m_dwMoney		= 0;
	
	m_nDirectorNum	= 0;
	m_nManagerNum	= 0;
	m_nMemberNum	= 0;

	m_dwNameID		= 0;
	m_szName[0]		= 0;

	m_szMasterTitle[0] = 0;
	memset(m_szDirectorTitle, 0, sizeof(m_szDirectorTitle));
	memset(m_szManagerTitle, 0, sizeof(m_szManagerTitle));
	// 
	m_szMaleTitle[0]		= 0;
	m_szFemaleTitle[0]		= 0;
	

	m_dwMasterID			= 0;
	m_szMasterName[0]		= 0;
	m_bMasterGender			= FALSE;		// 

	memset(m_dwDirectorID, 0, sizeof(m_dwDirectorID));
	memset(m_szDirectorName, 0, sizeof(m_szDirectorName));
	memset(m_bDirectorGender, 0, sizeof(m_bDirectorGender));		// 

	memset(m_dwManagerID, 0, sizeof(m_dwManagerID));
	memset(m_szManagerName, 0, sizeof(m_szManagerName));
	memset(m_bManagerGender, 0, sizeof(m_bManagerGender));			// 

	m_psMember = (STONG_MEMBER*)new STONG_MEMBER[defTONG_INIT_MEMBER_SIZE];
	m_nMemberPointSize = defTONG_INIT_MEMBER_SIZE;
	memset(m_psMember, 0, sizeof(STONG_MEMBER) * m_nMemberPointSize);

	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return;
	if (!lpszPlayerName || !lpszPlayerName[0] || strlen(lpszPlayerName) > defPLAYER_NAME_MAX_LENGTH)
		return;
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
		return;

	m_btCamp = nCamp;
	strcpy(m_szMasterName, lpszPlayerName);
	m_dwMasterID = g_String2Id(m_szMasterName);
	strcpy(m_szName, lpszTongName);
	m_dwNameID = g_String2Id(m_szName);
	// 
	strcpy(m_szMasterTitle, "Bang chñ");
	m_bMasterGender		= bGender;
	int i = 0;
	for(i = 0; i < defTONG_MAX_DIRECTOR; ++i)
	{
		memcpy(m_szDirectorTitle[i], "Tr­ëng l·o", sizeof(m_szDirectorTitle[i]));
	}
	for(i = 0; i < defTONG_MAX_MANAGER; ++i)
	{
		memcpy(m_szManagerTitle[i], "§éi tr­ëng", sizeof(m_szDirectorTitle[i]));
	}
	strcpy(m_szMaleTitle, "M«n ®Ö");
	strcpy(m_szFemaleTitle, "M«n ®Ö");

	m_bLockOperation = false;
	
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
CTongControl::CTongControl(TTongList sList)
{	
	m_btCamp		= 0;
	m_btLevel		= 1;
	m_dwMoney		= 0;
	
	m_nDirectorNum	= 0;
	m_nManagerNum	= 0;

	strcpy(m_szName, sList.szName);
	m_dwNameID		= g_String2Id(m_szName);
	
	m_szMasterTitle[0] = 0;
	memset(m_szDirectorTitle, 0, sizeof(m_szDirectorTitle));
	memset(m_szManagerTitle, 0, sizeof(m_szManagerTitle));
	// 
	m_szMaleTitle[0]		= 0;
	m_szFemaleTitle[0]	= 0;
	

	m_dwMasterID		= 0;
	m_szMasterName[0]	= 0;
	m_bMasterGender		= FALSE;		// 

	memset(m_dwDirectorID, 0, sizeof(m_dwDirectorID));
	memset(m_szDirectorName, 0, sizeof(m_szDirectorName));
	memset(m_bDirectorGender, 0, sizeof(m_bDirectorGender));		// 

	memset(m_dwManagerID, 0, sizeof(m_dwManagerID));
	memset(m_szManagerName, 0, sizeof(m_szManagerName));
	memset(m_bManagerGender, 0, sizeof(m_bManagerGender));			// 

	m_nMemberNum = sList.MemberCount;
	m_nMemberPointSize = m_nMemberNum + defTONG_INIT_MEMBER_SIZE;
	m_psMember = (STONG_MEMBER*)new STONG_MEMBER[m_nMemberPointSize];
	memset(m_psMember, 0, sizeof(STONG_MEMBER) * m_nMemberPointSize);

	m_bLockOperation = false;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
CTongControl::~CTongControl()
{
	if (m_psMember)
		delete []m_psMember;
}

BOOL	CTongControl::AddMember(char *lpszPlayerName, BOOL bGender)
{
	if (m_nMemberPointSize <= 0 || !m_psMember)
		return FALSE;
	if (!lpszPlayerName || strlen(lpszPlayerName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;

	int i = 0;
	for (i = 0; i < m_nMemberPointSize; i++)
	{
		if (m_psMember[i].m_dwNameID == 0)
			break;
	}

	if (i >= m_nMemberPointSize)
	{
		i = m_nMemberPointSize;
		STONG_MEMBER	*pTemp;
		pTemp = (STONG_MEMBER*)new STONG_MEMBER[m_nMemberPointSize];
		memcpy(pTemp, m_psMember, sizeof(STONG_MEMBER) * m_nMemberPointSize);

		delete []m_psMember;
		m_psMember = (STONG_MEMBER*)new STONG_MEMBER[m_nMemberPointSize + defTONG_MEMBER_SIZE_ADD];
		memcpy(m_psMember, pTemp, sizeof(STONG_MEMBER) * m_nMemberPointSize);
		memset(&m_psMember[m_nMemberPointSize], 0, sizeof(STONG_MEMBER) * defTONG_MEMBER_SIZE_ADD);
		m_nMemberPointSize += defTONG_MEMBER_SIZE_ADD;

		delete []pTemp;
	}

	strcpy(m_psMember[i].m_szName, lpszPlayerName);
	m_psMember[i].m_dwNameID	= g_String2Id(lpszPlayerName);
	m_psMember[i].m_bGender		= bGender;
	m_nMemberNum++;

	return TRUE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongControl::ChangePassword(char *lpOld, char *lpNew)
{
	if (!lpOld || !lpNew)
		return FALSE;

	return TRUE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongControl::GetTongHeadInfo(STONG_HEAD_INFO_SYNC *pInfo)
{
	if (!pInfo)
		return FALSE;

	int	i, j;

	pInfo->ProtocolFamily = pf_tong;
	pInfo->ProtocolID = enumS2C_TONG_HEAD_INFO;
	pInfo->m_dwMoney = m_dwMoney;
	pInfo->m_btCamp = m_btCamp;
	pInfo->m_btLevel = m_btLevel;
	pInfo->m_btManagerNum = m_nManagerNum;
	pInfo->m_dwMemberNum = m_nMemberNum;
	strcpy(pInfo->m_szTongName, m_szName);

	pInfo->m_sMember[0].m_btFigure = enumTONG_FIGURE_MASTER;
	pInfo->m_sMember[0].m_btPos = 0;
	strcpy(pInfo->m_sMember[0].m_szTitle, this->m_szMasterTitle);
	strcpy(pInfo->m_sMember[0].m_szName, this->m_szMasterName);

	pInfo->m_btDirectorNum = 0;
	for (i = 0, j = 1; i < defTONG_MAX_DIRECTOR; i++)
	{
		if (!m_szDirectorName[i][0])
			continue;
		pInfo->m_sMember[j].m_btFigure = enumTONG_FIGURE_DIRECTOR;
		pInfo->m_sMember[j].m_btPos = i;
		strcpy(pInfo->m_sMember[j].m_szTitle, this->m_szDirectorTitle[i]);
		strcpy(pInfo->m_sMember[j].m_szName, this->m_szDirectorName[i]);
		j++;
		pInfo->m_btDirectorNum++;
	}

	pInfo->m_wLength = sizeof(STONG_HEAD_INFO_SYNC) - sizeof(pInfo->m_sMember) + sizeof(STONG_ONE_LEADER_INFO) * (1 + pInfo->m_btDirectorNum);
	return TRUE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongControl::GetTongManagerInfo(
			STONG_GET_MANAGER_INFO_COMMAND *pApply,
			STONG_MANAGER_INFO_SYNC *pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (pApply->m_nParam3 <= 0)
		return FALSE;

	pInfo->ProtocolFamily	= pf_tong;
	pInfo->ProtocolID		= enumS2C_TONG_MANAGER_INFO;
	pInfo->m_dwParam		= pApply->m_dwParam;
	pInfo->m_dwMoney		= m_dwMoney;
	pInfo->m_btCamp			= m_btCamp;
	pInfo->m_btLevel		= m_btLevel;
	pInfo->m_btDirectorNum	= m_nDirectorNum;
	pInfo->m_btManagerNum	= m_nManagerNum;
	pInfo->m_dwMemberNum	= m_nMemberNum;
	pInfo->m_btStartNo		= pApply->m_nParam2;
	strcpy(pInfo->m_szTongName, m_szName);

	int		nNeedNum, nStartNum, i, j;

	nStartNum = pApply->m_nParam2;
	nNeedNum = pApply->m_nParam3;
	if (nNeedNum > defTONG_ONE_PAGE_MAX_NUM)
		nNeedNum = defTONG_ONE_PAGE_MAX_NUM;

	pInfo->m_btCurNum = 0;

	for (i = 0, j = 0; i < defTONG_MAX_MANAGER; i++)
	{
		if (j >= nStartNum)
			break;
		if (m_szManagerName[i][0])
			j++;
	}
	if (i >= defTONG_MAX_MANAGER || j < nStartNum)
		return FALSE;

	for (; i < defTONG_MAX_MANAGER; i++)
	{
		if (pInfo->m_btCurNum >= nNeedNum)
			break;
		if (!m_szManagerName[i][0])
			continue;

		pInfo->m_sMember[pInfo->m_btCurNum].m_btFigure = enumTONG_FIGURE_MANAGER;
		pInfo->m_sMember[pInfo->m_btCurNum].m_btPos = i;
		strcpy(pInfo->m_sMember[pInfo->m_btCurNum].m_szTitle, m_szManagerTitle[i]);
		strcpy(pInfo->m_sMember[pInfo->m_btCurNum].m_szName, m_szManagerName[i]);

		pInfo->m_btCurNum++;
	}
	if (pInfo->m_btCurNum == 0)
		return FALSE;

	pInfo->m_wLength = sizeof(STONG_MANAGER_INFO_SYNC) - sizeof(STONG_ONE_LEADER_INFO) * (defTONG_ONE_PAGE_MAX_NUM - pInfo->m_btCurNum);

	return TRUE;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
BOOL	CTongControl::GetTongMemberInfo(
			STONG_GET_MEMBER_INFO_COMMAND *pApply,
			STONG_MEMBER_INFO_SYNC *pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (pApply->m_nParam3 <= 0)
		return FALSE;
	if (!m_psMember || m_nMemberPointSize <= 0)
		return FALSE;

	pInfo->ProtocolFamily	= pf_tong;
	pInfo->ProtocolID		= enumS2C_TONG_MEMBER_INFO;
	pInfo->m_dwParam		= pApply->m_dwParam;
	pInfo->m_dwMoney		= m_dwMoney;
	pInfo->m_btCamp			= m_btCamp;
	pInfo->m_btLevel		= m_btLevel;
	pInfo->m_btDirectorNum	= m_nDirectorNum;
	pInfo->m_btManagerNum	= m_nManagerNum;
	pInfo->m_dwMemberNum	= m_nMemberNum;
	pInfo->m_btStartNo		= pApply->m_nParam2;
	strcpy(pInfo->m_szTongName, m_szName);

	int		nNeedNum, nStartNum, i, j;

	nStartNum = pApply->m_nParam2;
	nNeedNum = pApply->m_nParam3;
	if (nNeedNum > defTONG_ONE_PAGE_MAX_NUM)
		nNeedNum = defTONG_ONE_PAGE_MAX_NUM;

	pInfo->m_btCurNum = 0;
	for (i = 0, j = 0; i < m_nMemberPointSize; i++)
	{
		if (j >= nStartNum)
			break;
		if (m_psMember[i].m_szName[0])
			j++;
	}
	if (i >= m_nMemberPointSize || j < nStartNum)
		return FALSE;

	for (; i < m_nMemberPointSize; i++)
	{
		if (pInfo->m_btCurNum >= nNeedNum)
			break;
		if (!m_psMember[i].m_szName[0])
			continue;

		strcpy(pInfo->m_sMember[pInfo->m_btCurNum].m_szName, m_psMember[i].m_szName);
		// 
		if(m_psMember[i].m_bGender)
			strcpy(pInfo->m_sMember[pInfo->m_btCurNum].m_szTitle, m_szFemaleTitle);
		else
			strcpy(pInfo->m_sMember[pInfo->m_btCurNum].m_szTitle, m_szMaleTitle);
		

		pInfo->m_btCurNum++;
	}
	if (pInfo->m_btCurNum == 0)
		return FALSE;

	pInfo->m_wLength = sizeof(STONG_MEMBER_INFO_SYNC) - sizeof(STONG_ONE_MEMBER_INFO) * (defTONG_ONE_PAGE_MAX_NUM - pInfo->m_btCurNum);

	return TRUE;
}

BOOL CTongControl::Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync)
{
	if(!pInstate || !pSync)
		return FALSE;
	if(pInstate->m_btCurFigure == pInstate->m_btNewFigure && pInstate->m_btCurPos == pInstate->m_btNewPos)
		return FALSE;
	
	if(strlen(pInstate->m_szName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;

	int		i, nOldPos, nNewPos;
	DWORD	dwNameID;
	char	szName[defPLAYER_NAME_MAX_LENGTH + 1];

	memcpy(szName, pInstate->m_szName, sizeof(pInstate->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1] = 0;
	dwNameID = g_String2Id(szName);
	if (dwNameID == 0)
		return FALSE;

	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_INSTATE;
	pSync->m_btOldFigure	= pInstate->m_btCurFigure;
	pSync->m_btOldPos		= 0;
	pSync->m_btNewFigure	= pInstate->m_btNewFigure;
	pSync->m_btNewPos		= 0;
	pSync->m_dwParam		= pInstate->m_dwParam;
	pSync->m_btSuccessFlag	= 0;
	pSync->m_dwTongNameID	= pInstate->m_dwTongNameID;
	pSync->m_szTitle[0]		= 0;
	strcpy(pSync->m_szName, szName);
	
	BOOL	bGender;
	if (pInstate->m_btCurFigure == enumTONG_FIGURE_DIRECTOR)
	{
		for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
		{
			if (m_dwDirectorID[i] == dwNameID && strcmp(szName, m_szDirectorName[i]) == 0)
				break;
		}
		if (i >= defTONG_MAX_DIRECTOR)
			return FALSE;
		nOldPos = i;
		
		memcpy(m_szDirectorTitle[nOldPos], "Tr­ëng l·o", sizeof(m_szDirectorTitle[nOldPos]));
		bGender = m_bDirectorGender[nOldPos];
		switch (pInstate->m_btNewFigure)
		{
		case enumTONG_FIGURE_MANAGER:
			for (i = 0; i < defTONG_MAX_MANAGER; i++)
			{
				if (m_dwManagerID[i] == 0 && !m_szManagerName[i][0])
					break;
			}
			if (i >= defTONG_MAX_MANAGER)
				return FALSE;
			nNewPos = i;

			strcpy(m_szManagerName[nNewPos], szName);
			m_dwManagerID[nNewPos]			= dwNameID;
			m_bManagerGender[nNewPos]		= bGender;
			//
			m_szDirectorName[nOldPos][0]	= 0;
			m_dwDirectorID[nOldPos]			= 0;
			m_bDirectorGender[nOldPos]		= 0;
			m_nDirectorNum--;
			m_nManagerNum++;
			strcpy(pSync->m_szTitle, m_szManagerTitle[nNewPos]);
			break;
		case enumTONG_FIGURE_MEMBER:
			if(!AddMember(szName, bGender))
				return FALSE;
			//
			m_szDirectorName[nOldPos][0] = 0;
			m_dwDirectorID[nOldPos] = 0;
			m_nDirectorNum--;
			if(bGender)
			{
				strcpy(pSync->m_szTitle, m_szFemaleTitle);
			}
			else
			{
				strcpy(pSync->m_szTitle, m_szMaleTitle);
			}
			break;
		default:
			return FALSE;
		}
	}
	else if (pInstate->m_btCurFigure == enumTONG_FIGURE_MANAGER)
	{
		for (i = 0; i < defTONG_MAX_MANAGER; i++)
		{
			if (m_dwManagerID[i] == dwNameID && strcmp(szName, m_szManagerName[i]) == 0)
				break;
		}
		if (i >= defTONG_MAX_MANAGER)
			return FALSE;
		nOldPos = i;
		
		memcpy(m_szManagerTitle[nOldPos], "§éi tr­ëng", sizeof(m_szManagerTitle[nOldPos]));
		bGender = m_bManagerGender[nOldPos];
		switch (pInstate->m_btNewFigure)
		{
		case enumTONG_FIGURE_DIRECTOR:
			for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
			{
				if (m_dwDirectorID[i] == 0 && !m_szDirectorName[i][0])
					break;
			}
			if (i >= defTONG_MAX_DIRECTOR)
				return FALSE;
			nNewPos = i;

			strcpy(m_szDirectorName[nNewPos], szName);
			m_dwDirectorID[nNewPos]			= dwNameID;
			m_bDirectorGender[nNewPos]		= bGender;
			//
			m_szManagerName[nOldPos][0]		= 0;
			m_dwManagerID[nOldPos]			= 0;
			m_bManagerGender[nOldPos]		= 0;
			m_nDirectorNum++;
			m_nManagerNum--;
			strcpy(pSync->m_szTitle, m_szDirectorTitle[nNewPos]);
			break;
		case enumTONG_FIGURE_MEMBER:
			if(!AddMember(szName, bGender))
				return FALSE;
			//
			m_szManagerName[nOldPos][0] = 0;
			m_dwManagerID[nOldPos]		= 0;
			m_bManagerGender[nOldPos]	= 0;
			m_nManagerNum--;
			if(bGender)
			{
				strcpy(pSync->m_szTitle, m_szFemaleTitle);
			}
			else
			{
				strcpy(pSync->m_szTitle, m_szMaleTitle);
			}
			break;
		default:
			return FALSE;
		}
	}
	else if (pInstate->m_btCurFigure == enumTONG_FIGURE_MEMBER)
	{
		if (!m_psMember)
			return FALSE;
		int		nPos = -1;

		for (i = 0; i < m_nMemberPointSize; i++)
		{
			if (m_psMember[i].m_dwNameID != dwNameID)
				continue;
			nPos = i;
			break;
		}
		if (nPos < 0)
			return FALSE;
		nOldPos = 0;
		
		bGender = this->m_psMember[nPos].m_bGender;
		switch (pInstate->m_btNewFigure)
		{
		case enumTONG_FIGURE_DIRECTOR:
			for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
			{
				if (m_dwDirectorID[i] == 0 && !m_szDirectorName[i][0])
					break;
			}
			if (i >= defTONG_MAX_DIRECTOR)
				return FALSE;
			nNewPos = i;

			strcpy(m_szDirectorName[nNewPos], szName);
			m_dwDirectorID[nNewPos]				= dwNameID;
			m_bDirectorGender[nNewPos]			= bGender;
			//
			this->m_psMember[nPos].m_szName[0]	= 0;
			this->m_psMember[nPos].m_dwNameID	= 0;
			this->m_psMember[nPos].m_bGender	= 0;
			m_nDirectorNum++;
			m_nMemberNum--;
			strcpy(pSync->m_szTitle, m_szDirectorTitle[nNewPos]);
			break;
		case enumTONG_FIGURE_MANAGER:
			for (i = 0; i < defTONG_MAX_MANAGER; i++)
			{
				if (m_dwManagerID[i] == 0 && !m_szManagerName[i][0])
					break;
			}
			if (i >= defTONG_MAX_MANAGER)
				return FALSE;
			nNewPos = i;

			strcpy(m_szManagerName[nNewPos], szName);
			m_dwManagerID[nNewPos]				= dwNameID;
			m_bManagerGender[nNewPos]			= bGender;
			//
			this->m_psMember[nPos].m_szName[0]	= 0;
			this->m_psMember[nPos].m_dwNameID	= 0;
			this->m_psMember[nPos].m_bGender	= 0;
			m_nManagerNum++;
			m_nMemberNum--;
			strcpy(pSync->m_szTitle, m_szManagerTitle[nNewPos]);
			break;
		default:
			return FALSE;
		}
	}
	pSync->m_btSuccessFlag = 1;
	pSync->m_btOldPos = nOldPos;
	pSync->m_btNewPos = nNewPos;

	{{
		CNetConnectDup conndup;
		DWORD nameid = 0;
		unsigned long param = 0;
		if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szName), &conndup, NULL, &nameid, &param))
		{
			CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup.GetIP());
			if (tongconndup.IsValid())
			{
				STONG_BE_INSTATED_SYNC	sSync;

				sSync.ProtocolFamily = pf_tong;
				sSync.ProtocolID = enumS2C_TONG_BE_INSTATED;
				sSync.m_btFigure = pInstate->m_btNewFigure;
				sSync.m_btPos = nNewPos;
				sSync.m_dwParam = param;
				strcpy(sSync.m_szName, szName);
				switch (pInstate->m_btNewFigure)
				{
				case enumTONG_FIGURE_DIRECTOR:
					strcpy(sSync.m_szTitle, m_szDirectorTitle[nNewPos]);
					break;
				case enumTONG_FIGURE_MANAGER:
					strcpy(sSync.m_szTitle, m_szManagerTitle[nNewPos]);
					break;
				case enumTONG_FIGURE_MEMBER:
					if(bGender)
					{
						strcpy(sSync.m_szTitle, this->m_szFemaleTitle);
					}
					else
					{
						strcpy(sSync.m_szTitle, this->m_szMaleTitle);
					}
					break;
				}

				tongconndup.SendPackage((const void *)&sSync, sizeof(sSync));
			}
			else
			{
				//tong is not connect
			}

		}
		else
		{
			//not find
		}
	}}

	char	szMsg[73];
	sprintf(szMsg, "\\O%u", m_dwNameID);

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		switch (pSync->m_btNewFigure)
		{
		case enumTONG_FIGURE_DIRECTOR:
			sprintf(szMsg, "%s ®­îc bæ nhiÖm lµm tr­ëng l·o! Anh em cã ý kiÕn g× kh«ng?", szName);
			break;
		case enumTONG_FIGURE_MANAGER:
			sprintf(szMsg, "%s ®­îc bæ nhiÖm lµm ®éi tr­ëng! Anh em cã ý kiÕn g× kh«ng?", szName);
			break;
		case enumTONG_FIGURE_MEMBER:
			sprintf(szMsg, "%s trë thµnh m«n ®Ö! Anh em cã ý kiÕn g× kh«ng?", szName);
			break;
		}
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}

	TMemberStruct	sMember;
	sMember.MemberClass = (TONG_MEMBER_FIGURE)pInstate->m_btNewFigure;
	sMember.nTitleIndex = nNewPos;
	sMember.bGender		= bGender;
	strcpy(sMember.szTong, this->m_szName);
	strcpy(sMember.szName, szName);
	g_cTongDB.ChangeMember(sMember);

	return TRUE;
}

BOOL	CTongControl::Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync)
{
	if (!pKick || !pSync)
		return FALSE;
	
	if(strlen(pKick->m_szName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;

	char	szName[defPLAYER_NAME_MAX_LENGTH + 1];
	DWORD	dwNameID;
	int		i, nKickPos;

	memcpy(szName, pKick->m_szName, sizeof(pKick->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1] = 0;
	dwNameID = g_String2Id(szName);
	if (dwNameID == 0)
		return FALSE;

	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_KICK;
	pSync->m_btFigure		= pKick->m_btFigure;
	pSync->m_btPos			= pKick->m_btPos;
	pSync->m_dwParam		= pKick->m_dwParam;
	pSync->m_dwTongNameID	= pKick->m_dwTongNameID;
	pSync->m_btSuccessFlag	= 0;
	strcpy(pSync->m_szName, szName);

	if (pKick->m_btFigure == enumTONG_FIGURE_MANAGER)
	{
		if (pKick->m_btPos >= defTONG_MAX_MANAGER)
			return FALSE;
		for (i = 0; i < defTONG_MAX_MANAGER; i++)
		{
			if (m_dwManagerID[i] == dwNameID && strcmp(m_szManagerName[i], szName) == 0)
				break;
		}
		if (i >= defTONG_MAX_MANAGER)
			return FALSE;
		nKickPos = i;

		memcpy(this->m_szManagerTitle[i], "§éi tr­ëng", sizeof(this->m_szManagerTitle[i]));
		this->m_szManagerName[i][0] = 0;
		this->m_dwManagerID[i]		= 0;
		this->m_bManagerGender[i]	= 0;
		this->m_nManagerNum--;
	}
	else if (pKick->m_btFigure == enumTONG_FIGURE_MEMBER)
	{
		if (!m_psMember)
			return FALSE;

		int		nPos = -1;
		DWORD	dwNameID = g_String2Id(szName);
		if (dwNameID == 0)
			return FALSE;

		for (int i = 0; i < m_nMemberPointSize; i++)
		{
			if (m_psMember[i].m_dwNameID != dwNameID)
				continue;
			nPos = i;
			break;
		}
		if (nPos < 0)
			return FALSE;

		if (!m_psMember[nPos].m_szName[0] ||
			strcmp(m_psMember[nPos].m_szName, szName) != 0)
			return FALSE;
		m_psMember[nPos].m_szName[0]	= 0;
		m_psMember[nPos].m_dwNameID		= 0;
		m_psMember[nPos].m_bGender		= 0;
		m_nMemberNum--;
		nKickPos = 0;
	}
	else
	{
		return FALSE;
	}
	pSync->m_btSuccessFlag = 1;
	pSync->m_btPos = nKickPos;


	{{
		CNetConnectDup conndup;
		DWORD nameid = 0;
		unsigned long param = 0;
		if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szName), &conndup, NULL, &nameid, &param))
		{
			CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup.GetIP());
			if (tongconndup.IsValid())
			{
				STONG_BE_KICKED_SYNC	sSync;

				sSync.ProtocolFamily = pf_tong;
				sSync.ProtocolID = enumS2C_TONG_BE_KICKED;
				sSync.m_btFigure = pKick->m_btFigure;
				sSync.m_btPos = nKickPos;
				sSync.m_dwParam = param;
				strcpy(sSync.m_szName, szName);

				tongconndup.SendPackage((const void *)&sSync, sizeof(sSync));
			}
			else
			{
				//tong is not connect
			}

		}
		else
		{
			//not find
		}
	}}

	g_cTongDB.DelMember(szName);

	char	szMsg[71];
	sprintf(szMsg, "\\O%u", m_dwNameID);

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		sprintf(szMsg, "%s bÞ trôc xuÊt khái bang héi! Anh em cã ý kiÕn g× kh«ng?", szName);
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}

	return TRUE;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
BOOL	CTongControl::Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync)
{
	if(!pLeave || !pSync)
		return FALSE;
	if(strlen(pLeave->m_szName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;

	char	szName[defPLAYER_NAME_MAX_LENGTH + 1];
	memcpy(szName, pLeave->m_szName, sizeof(pLeave->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1] = 0;

	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_LEAVE;
	pSync->m_dwParam		= pLeave->m_dwParam;
	pSync->m_btSuccessFlag	= 0;
	strcpy(pSync->m_szName, szName);

	switch (pLeave->m_btFigure)
	{
	case enumTONG_FIGURE_MEMBER:
		{
			if (!m_psMember)
				break;
			int		nPos = -1;
			DWORD	dwNameID = g_String2Id(szName);
			if (dwNameID == 0)
				return FALSE;
			for (int i = 0; i < m_nMemberPointSize; i++)
			{
				if (m_psMember[i].m_dwNameID != dwNameID)
					continue;
				nPos = i;
				break;
			}
			if (nPos < 0)
				return FALSE;
			if (!m_psMember[nPos].m_szName[0] ||
				strcmp(m_psMember[nPos].m_szName, szName) != 0)
				return FALSE;

			m_psMember[nPos].m_szName[0]	= 0;
			m_psMember[nPos].m_dwNameID		= 0;
			m_psMember[nPos].m_bGender		= 0;
			m_nMemberNum--;
		}
		break;
	case enumTONG_FIGURE_MANAGER:
		{
			if (pLeave->m_btPos >= 0 && pLeave->m_btPos < defTONG_MAX_MANAGER)
			{
				if (strcmp(m_szManagerName[pLeave->m_btPos], szName) == 0)
				{
					m_szManagerName[pLeave->m_btPos][0] = 0;
					m_dwManagerID[pLeave->m_btPos] = 0;
					m_nManagerNum--;
					break;
				}
			}

			int		nPos = -1;
			DWORD	dwNameID = g_String2Id(szName);
			if (dwNameID == 0)
				return FALSE;
			for (int i = 0; i < defTONG_MAX_MANAGER; i++)
			{
				if (this->m_dwManagerID[i] != dwNameID)
					continue;
				nPos = i;
				break;
			}
			if (nPos < 0)
				return FALSE;
			if (!this->m_szManagerName[nPos][0] ||
				strcmp(szName, this->m_szManagerName[nPos]) != 0)
				return FALSE;
			
			memcpy(this->m_szManagerTitle[nPos], "§éi tr­ëng", sizeof(this->m_szManagerTitle[nPos]));
			this->m_szManagerName[nPos][0]	= 0;
			this->m_dwManagerID[nPos]		= 0;
			this->m_bManagerGender[nPos]	= 0;
			this->m_nManagerNum--;
		}
		break;
	case enumTONG_FIGURE_DIRECTOR:
		return FALSE;
	case enumTONG_FIGURE_MASTER:
		return FALSE;
	}
	pSync->m_btSuccessFlag = 1;

	g_cTongDB.DelMember(szName);

	char	szMsg[17];
	sprintf(szMsg, "\\O%u", m_dwNameID);
	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		sprintf(szMsg, "T¹m biÖt anh em!");
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(szName), std::string(szMsg));
	}

	return TRUE;
}

BOOL	CTongControl::AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept)
{	
	if(!pAccept)
		return FALSE;

	if(strlen(pAccept->m_szName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;

	char	szName[defPLAYER_NAME_MAX_LENGTH + 1], szOldMaster[defPLAYER_NAME_MAX_LENGTH + 1];
	memcpy(szName, pAccept->m_szName, sizeof(pAccept->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1] = 0;
	if (!szName[0])
		return FALSE;

	strcpy(szOldMaster, this->m_szMasterName);

	if (pAccept->m_btAcceptFalg == 0)
	{
		CNetConnectDup conndup;
		DWORD nameid = 0;
		unsigned long param = 0;
		if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(m_szMasterName), &conndup, NULL, &nameid, &param))
		{
			CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup.GetIP());
			if (tongconndup.IsValid())
			{
				STONG_CHANGE_MASTER_FAIL_SYNC	sFail;
				sFail.ProtocolFamily	= pf_tong;
				sFail.ProtocolID		= enumS2C_TONG_CHANGE_MASTER_FAIL;
				sFail.m_dwParam			= param;
				sFail.m_btFailID		= 1;
				sFail.m_dwTongNameID	= pAccept->m_dwTongNameID;
				memcpy(sFail.m_szName, szName, sizeof(szName));
				tongconndup.SendPackage((const void *)&sFail, sizeof(sFail));
			}
			else
			{
				//tong is not connect
			}
		}
		else
		{
			//not find
		}

		return FALSE;
	}

	DWORD	dwNameID;
	int		i;

	dwNameID = g_String2Id(szName);
	if (dwNameID == 0)
		return FALSE;

	BOOL	bOldGender, bNewGender;
	
	bOldGender = m_bMasterGender;
	if (pAccept->m_btFigure == enumTONG_FIGURE_DIRECTOR)
	{
		for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
		{
			if (m_dwDirectorID[i] == dwNameID && strcmp(szName, m_szDirectorName[i]) == 0)
				break;
		}
		if (i >= defTONG_MAX_DIRECTOR)
			return FALSE;
		
		memcpy(m_szDirectorTitle[i], "Tr­ëng l·o", sizeof(m_szDirectorTitle[i]));
		bNewGender = m_bDirectorGender[i];

		if(!AddMember(this->m_szMasterName, bOldGender))
			return FALSE;
		strcpy(this->m_szMasterName, szName);
		this->m_dwMasterID		= dwNameID;
		this->m_bMasterGender	= bNewGender;
		//
		this->m_szDirectorName[i][0]	= 0;
		this->m_dwDirectorID[i]			= 0;
		this->m_bDirectorGender[i]		= 0;
		this->m_nDirectorNum--;
	}
	else if (pAccept->m_btFigure == enumTONG_FIGURE_MANAGER)
	{
		for (i = 0; i < defTONG_MAX_MANAGER; i++)
		{
			if (m_dwManagerID[i] == dwNameID && strcmp(szName, m_szManagerName[i]) == 0)
				break;
		}
		if (i >= defTONG_MAX_MANAGER)
			return FALSE;
		
		memcpy(m_szManagerTitle[i], "§éi tr­ëng", sizeof(m_szManagerTitle[i]));
		bNewGender = m_bManagerGender[i];

		if (!AddMember(this->m_szMasterName, bOldGender))
			return FALSE;

		strcpy(this->m_szMasterName, szName);
		this->m_dwMasterID = dwNameID;
		this->m_bMasterGender	= bNewGender;
		//
		this->m_szManagerName[i][0] = 0;
		this->m_dwManagerID[i]		= 0;
		this->m_bManagerGender[i]	= 0;
		this->m_nManagerNum--;
	}
	else if (pAccept->m_btFigure == enumTONG_FIGURE_MEMBER)
	{
		if (!m_psMember || m_nMemberPointSize <= 0)
			return FALSE;
		for (i = 0; i < this->m_nMemberPointSize; i++)
		{
			if (m_psMember[i].m_dwNameID == dwNameID &&
				strcmp(szName, m_psMember[i].m_szName) == 0)
				break;
		}
		if (i >= m_nMemberPointSize)
			return FALSE;

		bNewGender = this->m_psMember[i].m_bGender;

		if (!AddMember(this->m_szMasterName, bOldGender))
			return FALSE;

		strcpy(this->m_szMasterName, szName);
		this->m_dwMasterID		= dwNameID;
		this->m_bMasterGender	= bNewGender;

		this->m_psMember[i].m_szName[0] = 0;
		this->m_psMember[i].m_dwNameID	= 0;
		this->m_psMember[i].m_bGender	= 0;
		this->m_nMemberNum--;
	}
	else
	{
		return FALSE;
	}

	STONG_CHANGE_AS_SYNC	sChange;

	sChange.ProtocolFamily	= pf_tong;
	sChange.ProtocolID		= enumS2C_TONG_CHANGE_AS;
	sChange.m_dwTongNameID	= this->m_dwNameID;

	DWORD nameid = 0;
	unsigned long param = 0;
	CNetConnectDup conndup1;
	if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szOldMaster), &conndup1, NULL, &nameid, &param))
	{
		CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup1.GetIP());
		if (tongconndup.IsValid())
		{
			sChange.m_dwParam		= param;
			sChange.m_btFigure		= enumTONG_FIGURE_MEMBER;
			sChange.m_btPos			= 0;
			if(bOldGender)
			{
				strcpy(sChange.m_szTitle, m_szFemaleTitle);
			}
			else
			{
				strcpy(sChange.m_szTitle, m_szMaleTitle);
			}
			strcpy(sChange.m_szName, m_szMasterName);

			tongconndup.SendPackage((const void *)&sChange, sizeof(sChange));
		}
	}

	CNetConnectDup conndup2;
	if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szName), &conndup2, NULL, &nameid, &param))
	{
		CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup2.GetIP());
		if (tongconndup.IsValid())
		{
			sChange.m_dwParam		= param;
			sChange.m_btFigure		= enumTONG_FIGURE_MASTER;
			sChange.m_btPos			= 0;
			strcpy(sChange.m_szTitle, this->m_szMasterTitle);
			strcpy(sChange.m_szName, this->m_szMasterName);

			tongconndup.SendPackage((const void *)&sChange, sizeof(sChange));
		}
	}

	STONG_CHANGE_MASTER_SYNC	sMaster;
	sMaster.ProtocolFamily	= pf_tong;
	sMaster.ProtocolID		= enumS2C_TONG_CHANGE_AS;
	sMaster.m_dwTongNameID	= m_dwNameID;
	strcpy(sMaster.m_szName, m_szMasterName);

	g_TongServer.BroadPackage((const void*)&sMaster, sizeof(sMaster));

	TMemberStruct	sMember1;
	sMember1.MemberClass	= enumTONG_FIGURE_MASTER;
	sMember1.nTitleIndex	= 0;
	sMember1.bGender		= bNewGender;
	strcpy(sMember1.szTong, this->m_szName);
	strcpy(sMember1.szName, szName);
	g_cTongDB.ChangeMember(sMember1);

	TMemberStruct	sMember2;
	sMember2.MemberClass	= enumTONG_FIGURE_MEMBER;
	sMember2.nTitleIndex	= 0;
	sMember2.bGender		= bOldGender;
	strcpy(sMember2.szTong, this->m_szName);
	strcpy(sMember2.szName, szOldMaster);
	g_cTongDB.ChangeMember(sMember2);
	
	g_cTongSet.SaveTong(m_dwNameID);

	char	szMsg[77];
	sprintf(szMsg, "\\O%u", m_dwNameID);

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		sprintf(szMsg, "%s lªn lµm t©n bang chñ! Hy väng bang héi ngµy cµng h­ng thÞnh!", szName);
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(szOldMaster), std::string(szMsg));
	}

	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongControl::ChangeCamp(STONG_CHANGE_CAMP_COMMAND* pChange, STONG_CHANGE_CAMP_SYNC* pSync)
{
	if(!pChange)
		return FALSE;
	if(!pSync)
		return FALSE;
	if(m_dwMoney < pChange->m_nPrice)
		return FALSE;
	if(pChange->m_btNewCamp == m_btCamp)
		return FALSE;
	
	m_dwMoney	= m_dwMoney - pChange->m_nPrice;
	m_btCamp	= pChange->m_btNewCamp;
	g_cTongSet.SaveTong(m_dwNameID);

	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_CHANGE_CAMP;
	pSync->m_dwTongNameID	= m_dwNameID;
	pSync->m_dwMoney		= m_dwMoney;
	pSync->m_btNewCamp		= m_btCamp;

	char	szMsg[83];
	sprintf(szMsg, "\\O%u", m_dwNameID);

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{	
		switch(m_btCamp)
		{
		case camp_justice:
			sprintf(szMsg, "Phe ph¸i bang héi ®· ®­îc chuyÓn sang chÝnh ph¸i, tæn hao %d l­îng ng©n quü!", pChange->m_nPrice);
			break;
		case camp_evil:
			sprintf(szMsg, "Phe ph¸i bang héi ®· ®­îc chuyÓn sang tµ ph¸i!, tæn hao %d l­îng ng©n quü!", pChange->m_nPrice);
			break;
		case camp_balance:
			sprintf(szMsg, "Phe ph¸i bang héi ®· ®­îc chuyÓn sang trung lËp!, tæn hao %d l­îng ng©n quü!", pChange->m_nPrice);
			break;
		}
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}
	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongControl::ChangeMemberTitle(STONG_CHANGE_MEMBER_TITLE_COMMAND* pChange, STONG_CHANGE_MEMBER_TITLE_SYNC* pSync)
{
	if(!pChange)
		return FALSE;
	if(!pSync)
		return FALSE;

	if(pChange->m_bGender)
	{
		strcpy(m_szFemaleTitle, pChange->m_szTitle);
	}
	else
	{
		strcpy(m_szMaleTitle, pChange->m_szTitle);
	}

	pSync->ProtocolFamily	= pf_tong;
	pSync->ProtocolID		= enumS2C_TONG_CHANGE_MEMBER_TITLE;
	pSync->m_dwTongNameID	= m_dwNameID;
	pSync->m_bGender		= pChange->m_bGender;
	strcpy(pSync->m_szTitle, pChange->m_szTitle);
		
	g_cTongSet.SaveTong(m_dwNameID);

	char	szMsg[83];
	sprintf(szMsg, "\\O%u", m_dwNameID);

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{	
		if(pChange->m_bGender)
		{
			sprintf(szMsg, "Tªn thµnh viªn n÷ ®· thay ®æi thµnh %s! Anh em cã ý kiÕn g× kh«ng?", m_szFemaleTitle);
		}
		else
		{
			sprintf(szMsg, "Tªn thµnh viªn nam ®· thay ®æi thµnh %s! Anh em cã ý kiÕn g× kh«ng?", m_szMaleTitle);
		}
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}
	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongControl::ChangeOneTitle(STONG_CHANGE_ONE_TITLE_COMMAND* pChange, STONG_CHANGE_ONE_TITLE_SYNC* pSync)
{
	if(!pChange)
		return FALSE;
	if(!pSync)
		return FALSE;
	
	DWORD dwNameID;
	char szName[defPLAYER_NAME_MAX_LENGTH + 1];
	int i;

	dwNameID = g_String2Id(pChange->m_szName);
	memcpy(szName, pChange->m_szName, sizeof(pChange->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1] = 0;
	
	switch(pChange->m_bFigureFlag)
	{
	case enumTONG_FIGURE_MANAGER:
		{
			for (i = 0; i < defTONG_MAX_MANAGER; i++)
			{
				if (m_dwManagerID[i] == dwNameID && strcmp(szName, m_szManagerName[i]) == 0)
					break;
			}
			if (i >= defTONG_MAX_MANAGER)
				return FALSE;

			memcpy(m_szManagerTitle[i], pChange->m_szTitle, sizeof(m_szManagerTitle[i]));
		}
		break;
	case enumTONG_FIGURE_DIRECTOR:
		{
			for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
			{
				if (m_dwDirectorID[i] == dwNameID && strcmp(szName, m_szDirectorName[i]) == 0)
					break;
			}
			if (i >= defTONG_MAX_DIRECTOR)
				return FALSE;

			memcpy(m_szDirectorTitle[i], pChange->m_szTitle, sizeof(m_szDirectorTitle[i]));
		}
		break;
	default:
		return FALSE;
	}
		
	DWORD nameid = 0;
	unsigned long param = 0;
	CNetConnectDup conndup;
	if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szName), &conndup, NULL, &nameid, &param))
	{
		CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup.GetIP());
		if (tongconndup.IsValid())
		{	
			pSync->ProtocolFamily	= pf_tong;
			pSync->ProtocolID		= enumS2C_TONG_CHANGE_ONE_TITLE;
			pSync->m_dwParam		= param;
			strcpy(pSync->m_szName, szName);
			strcpy(pSync->m_szTitle, pChange->m_szTitle);
		}
	}

	g_cTongSet.SaveTong(m_dwNameID);

	char	szMsg[82];
	sprintf(szMsg, "\\O%u", m_dwNameID); 

	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{	
		sprintf(szMsg, "%s ®­îc ®æi tªn thµnh %s! Anh em cã ý kiÕn g× kh«ng?", szName, pChange->m_szTitle);
		
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}
	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongControl::SaveMoney(STONG_SAVE_MONEY_COMMAND* pChange, STONG_SAVE_MONEY_SYNC* sChange)
{	
	if(m_bLockOperation)
		return FALSE;
	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;

	if(pChange->m_dwMoney != m_dwMoney)
		return FALSE;
	
	char szName[defPLAYER_NAME_MAX_LENGTH + 1];
	memcpy(szName, pChange->m_szName, sizeof(pChange->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1];
	if(!szName[0])
		return FALSE; 
	
	m_bLockOperation = true;

	m_dwMoney = m_dwMoney + pChange->m_nMoney;

	sChange->ProtocolFamily	= pf_tong;
	sChange->ProtocolID		= enumS2C_TONG_SAVE_MONEY;
	sChange->m_dwTongNameID	= m_dwNameID;
	sChange->m_dwMoney		= m_dwMoney;

	g_cTongSet.SaveTong(m_dwNameID);

	m_bLockOperation = false;

	char	szMsg[96];
	sprintf(szMsg, "\\O%u", m_dwNameID);
	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if(channid != -1)
	{
		sprintf(szMsg, "%s ®· ®ãng gãp vµo quü bang %d l­îng!", pChange->m_szName, pChange->m_nMoney);
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}
	return TRUE;
}

//----------------------------------------------------------------------
//	
//----------------------------------------------------------------------
BOOL	CTongControl::GetMoney(STONG_GET_MONEY_COMMAND* pChange, STONG_GET_MONEY_SYNC* sChange)
{	
	if(m_bLockOperation)
		return FALSE;

	if(!pChange)
		return FALSE;
	if(!sChange)
		return FALSE;

	if(pChange->m_dwMoney != m_dwMoney)
		return FALSE;
	
	char szName[defPLAYER_NAME_MAX_LENGTH + 1];
	memcpy(szName, pChange->m_szName, sizeof(pChange->m_szName));
	szName[defPLAYER_NAME_MAX_LENGTH + 1];
	if(!szName[0])
		return FALSE; 
	
	m_bLockOperation = true;

	m_dwMoney = m_dwMoney - pChange->m_nMoney;

	DWORD nameid = 0;
	unsigned long param = 0;
	CNetConnectDup conndup;
	if (g_HostServer.FindPlayerByRole(NULL, std::_tstring(szName), &conndup, NULL, &nameid, &param))
	{
		CNetConnectDup tongconndup = g_TongServer.FindTongConnectByIP(conndup.GetIP());
		if (tongconndup.IsValid())
		{	
			sChange->ProtocolFamily	= pf_tong;
			sChange->ProtocolID		= enumS2C_TONG_GET_MONEY;
			sChange->m_dwParam		= param;
			sChange->m_dwTongNameID	= m_dwNameID;
			sChange->m_dwMoney		= m_dwMoney;
			sChange->m_nMoney		= pChange->m_nMoney;
		}
	}

	g_cTongSet.SaveTong(m_dwNameID);

	m_bLockOperation = false;

	char	szMsg[96];
	sprintf(szMsg, "\\O%u", m_dwNameID);
	DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
	if (channid != -1)
	{
		sprintf(szMsg, "%s ®· rót khái quü bang %d l­îng!", pChange->m_szName, pChange->m_nMoney);
		g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(m_szMasterName), std::string(szMsg));
	}

	return TRUE;
}

BOOL	CTongControl::GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync)
{
	if (!pLogin || !pSync)
		return FALSE;

	int		i;
	DWORD	dwNameID = g_String2Id(pLogin->m_szName);
	if (dwNameID == 0)
		return FALSE;

	pSync->m_btCamp		= this->m_btCamp;
	pSync->m_dwMoney	= this->m_dwMoney;
	strcpy(pSync->m_szTongName, this->m_szName);
	strcpy(pSync->m_szMaster, this->m_szMasterName);
	strcpy(pSync->m_szName, pLogin->m_szName);

	if (this->m_dwMasterID == dwNameID)
	{
		pSync->m_btFigure	= enumTONG_FIGURE_MASTER;
		pSync->m_btPos		= 0;
		pSync->m_btFlag		= 1;
		strcpy(pSync->m_szTitle, this->m_szMasterTitle);
		return TRUE;
	}

	for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
	{
		if (this->m_dwDirectorID[i] == dwNameID)
		{
			pSync->m_btFigure	= enumTONG_FIGURE_DIRECTOR;
			pSync->m_btPos		= i;
			pSync->m_btFlag		= 1;
			strcpy(pSync->m_szTitle, this->m_szDirectorTitle[i]);
			return TRUE;
		}
	}

	for (i = 0; i < defTONG_MAX_MANAGER; i++)
	{
		if (this->m_dwManagerID[i] == dwNameID)
		{
			pSync->m_btFigure	= enumTONG_FIGURE_MANAGER;
			pSync->m_btPos		= i;
			pSync->m_btFlag		= 1;
			strcpy(pSync->m_szTitle, this->m_szManagerTitle[i]);
			return TRUE;
		}
	}

	if (this->m_psMember)
	{
		for (i = 0; i < this->m_nMemberPointSize; i++)
		{
			if (m_psMember[i].m_dwNameID == dwNameID)
			{
				pSync->m_btFigure	= enumTONG_FIGURE_MEMBER;
				pSync->m_btPos		= 0;
				pSync->m_btFlag		= 1;
				if(m_psMember[i].m_bGender)
					strcpy(pSync->m_szTitle, this->m_szFemaleTitle);
				else
					strcpy(pSync->m_szTitle, this->m_szMaleTitle);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL	CTongControl::SearchOne(char *lpszName, STONG_ONE_LEADER_INFO *pInfo)
{
	if (!lpszName || !lpszName[0] || !pInfo)
		return FALSE;

	int		i;
	DWORD	dwNameID = g_String2Id(lpszName);
	if (dwNameID == 0)
		return FALSE;

	if (m_dwMasterID == dwNameID)
	{
		pInfo->m_btFigure	= enumTONG_FIGURE_MASTER;
		pInfo->m_btPos		= 0;
		strcpy(pInfo->m_szTitle, m_szMasterTitle);
		strcpy(pInfo->m_szName, m_szMasterName);
		return TRUE;
	}

	for (i = 0; i < defTONG_MAX_DIRECTOR; i++)
	{
		if (m_dwDirectorID[i] == dwNameID)
		{
			pInfo->m_btFigure	= enumTONG_FIGURE_DIRECTOR;
			pInfo->m_btPos		= i;
			strcpy(pInfo->m_szTitle, m_szDirectorTitle[i]);
			strcpy(pInfo->m_szName, m_szDirectorName[i]);
			return TRUE;
		}
	}

	for (i = 0; i < defTONG_MAX_MANAGER; i++)
	{
		if (m_dwManagerID[i] == dwNameID)
		{
			pInfo->m_btFigure	= enumTONG_FIGURE_MANAGER;
			pInfo->m_btPos		= i;
			strcpy(pInfo->m_szTitle, m_szManagerTitle[i]);
			strcpy(pInfo->m_szName, m_szManagerName[i]);
			return TRUE;
		}
	}

	if (m_psMember)
	{
		for (i = 0; i < m_nMemberPointSize; i++)
		{
			if (m_psMember[i].m_dwNameID == dwNameID)
			{
				pInfo->m_btFigure	= enumTONG_FIGURE_MEMBER;
				pInfo->m_btPos		= 0;
				if(m_psMember[i].m_bGender)
					strcpy(pInfo->m_szTitle, m_szFemaleTitle);
				else
					strcpy(pInfo->m_szTitle, m_szMaleTitle);
				strcpy(pInfo->m_szName, m_psMember[i].m_szName);
				return TRUE;
			}
		}
	}

	return FALSE;
}









