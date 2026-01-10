
#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTong.h"
#ifndef _SERVER
#include	"CoreShell.h"
#endif

#ifndef _SERVER
extern IClientCallback* l_pDataChangedNotifyFunc;
#endif


//

void	KPlayerTong::Init(int nPlayerIdx)
{
	m_nPlayerIndex = nPlayerIdx;

	Clear();
}


//	

void	KPlayerTong::Clear()
{
	m_nFlag				= 0;
	m_nFigure			= enumTONG_FIGURE_MEMBER;
	m_nCamp				= 0;
	m_dwTongNameID		= 0;
	m_szName[0]			= 0;
	m_szTitle[0]		= 0;
	m_szMasterName[0]	= 0;
	m_nApplyTo			= 0;

#ifdef _SERVER
	m_dwMoney			= 0;
#endif
}

#ifndef _SERVER



void	KPlayerTong::SendSystemMsg(const char* lpszMsg)
{
	l_pDataChangedNotifyFunc->ChannelMessageArrival(0,"Bang héi", lpszMsg, strlen(lpszMsg), TRUE);
}
#endif

#ifndef _SERVER

//

BOOL	KPlayerTong::ApplyCreateTong(int nCamp, const char *lpszTongName)
{
	SendSystemMsg(MSG_TONG_APPLY_CREATE);
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR01);
		return FALSE;
	}
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR02);
		return FALSE;
	}
	if (m_nFlag)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR03);
		return FALSE;
	}
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR04);
		return FALSE;
	}
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR05);
		return FALSE;
	}
	if ((int)Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR06);
		return FALSE;
	}
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop)
	{
		return FALSE;
	} 
	
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoneyAmount() < PlayerSet.m_sTongParam.m_nMoney)
	{
		char	szBuf[80];
		sprintf(szBuf, MSG_TONG_CREATE_ERROR07, PlayerSet.m_sTongParam.m_nMoney);
		SendSystemMsg(szBuf);
		return FALSE;
	}
	
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CountSameItem(room_equipment, item_task, PlayerSet.m_sTongParam.m_nItemIdx, 1, -1, 1) < 1)
	{	
		SendSystemMsg(MSG_TONG_CREATE_ERROR12);
		return FALSE;
	}
	if (Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_REPUTE) < PlayerSet.m_sTongParam.m_nRepute)
	{
		char	szBuf[80];
		sprintf(szBuf, MSG_TONG_CREATE_ERROR13, PlayerSet.m_sTongParam.m_nRepute);
		SendSystemMsg(szBuf);
		return FALSE;
	}
	
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		SendSystemMsg(MSG_TONG_CREATE_ERROR08);
		return FALSE;
	}

	TONG_APPLY_CREATE_COMMAND	sApply;
	sApply.ProtocolType	= c2s_extendtong;
	sApply.m_wLength	= sizeof(TONG_APPLY_CREATE_COMMAND) - 1;
	sApply.m_btMsgId	= enumTONG_COMMAND_ID_APPLY_CREATE;
	sApply.m_bGender	= (BOOL)Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nSex;
	sApply.m_btCamp		= (BYTE)nCamp;
	strcpy(sApply.m_szName, lpszTongName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::ApplyAddTong(DWORD dwNpcID)
{
	SendSystemMsg(MSG_TONG_APPLY_ADD);
	if (m_nFlag)
	{
		SendSystemMsg(MSG_TONG_APPLY_ADD_ERROR1);
		return FALSE;
	}
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		SendSystemMsg(MSG_TONG_APPLY_ADD_ERROR2);
		return FALSE;
	}
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		SendSystemMsg(MSG_TONG_APPLY_ADD_ERROR3);
		return FALSE;
	}

	TONG_APPLY_ADD_COMMAND	sApply;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_wLength = sizeof(TONG_APPLY_ADD_COMMAND) - 1;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_ADD;
	sApply.m_dwNpcID = dwNpcID;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TONG_APPLY_ADD_COMMAND));

	return TRUE;
}
#endif

#ifdef _SERVER

//

int		KPlayerTong::CheckCreateCondition(int nCamp, char *lpszTongName)
{
	if (Player[m_nPlayerIndex].m_nIndex <= 0)
		return enumTONG_CREATE_ERROR_ID1;

	if (Player[m_nPlayerIndex].CheckTrading())
		return enumTONG_CREATE_ERROR_ID2;

	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
		return enumTONG_CREATE_ERROR_ID3;

	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return enumTONG_CREATE_ERROR_ID4;

	if (m_nFlag)
		return enumTONG_CREATE_ERROR_ID5;

	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp != camp_free)
		return enumTONG_CREATE_ERROR_ID6;

	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel || 
		(int)Player[m_nPlayerIndex].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
		return enumTONG_CREATE_ERROR_ID7;

	if (Player[m_nPlayerIndex].m_ItemList.GetMoneyAmount() < PlayerSet.m_sTongParam.m_nMoney)
		return enumTONG_CREATE_ERROR_ID8;

	
	if (Player[m_nPlayerIndex].m_ItemList.CountSameItem(room_equipment, item_task, PlayerSet.m_sTongParam.m_nItemIdx, 1, -1, 1) < 1)
		return enumTONG_CREATE_ERROR_ID15;
	if (Player[m_nPlayerIndex].TaskGetSaveVal(TV_REPUTE) < PlayerSet.m_sTongParam.m_nRepute)
		return enumTONG_CREATE_ERROR_ID16;
	

	if (Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		return enumTONG_CREATE_ERROR_ID9;

	return 0;
}
#endif

#ifdef _SERVER

//

BOOL	KPlayerTong::Create(int nCamp, const char *lpszTongName)
{
	m_nFlag			= 1;
	m_nFigure		= enumTONG_FIGURE_MASTER;
	m_nCamp			= nCamp;
	strcpy(m_szTitle, "Bang chñ");
	strcpy(m_szName, lpszTongName);
	strcpy(m_szMasterName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
	Player[m_nPlayerIndex].m_ItemList.DecMoney(PlayerSet.m_sTongParam.m_nMoney);
	
	Player[m_nPlayerIndex].m_ItemList.RemoveItemInEquipment(1, item_task, PlayerSet.m_sTongParam.m_nItemIdx, 1, -1, 1);
	Player[m_nPlayerIndex].TaskSetSaveVal(TV_REPUTE, Player[m_nPlayerIndex].TaskGetSaveVal(TV_REPUTE) - PlayerSet.m_sTongParam.m_nRepute);
	

	TONG_CREATE_SYNC	sCreate;
	sCreate.ProtocolType = s2c_tongcreate;
	sCreate.m_btCamp = nCamp;
	if (strlen(lpszTongName) < sizeof(sCreate.m_szName))
		strcpy(sCreate.m_szName, lpszTongName);
	else
	{
		memcpy(sCreate.m_szName, lpszTongName, sizeof(sCreate.m_szName) - 1);
		sCreate.m_szName[sizeof(sCreate.m_szName) - 1] = 0;
	}
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sCreate, sizeof(TONG_CREATE_SYNC));

	return TRUE;
}
#endif

#ifndef _SERVER

//	

void	KPlayerTong::Create(TONG_CREATE_SYNC *psCreate)
{
	if (!psCreate)
		return;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = psCreate->m_btCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = psCreate->m_btCamp;

	m_nFlag			= 1;
	m_nFigure		= enumTONG_FIGURE_MASTER;
	m_nCamp			= psCreate->m_btCamp;
	m_szTitle[0]	= 0;
	memset(m_szName, 0, sizeof(m_szName));
	memcpy(m_szName, psCreate->m_szName, sizeof(psCreate->m_szName));
	strcpy(m_szMasterName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	SendSystemMsg(MSG_TONG_CREATE_SUCCESS);

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

}
#endif

#ifdef _SERVER

//

BOOL	KPlayerTong::GetOpenFlag()
{
	return (m_nFlag && m_nFigure != enumTONG_FIGURE_MEMBER);
}
#endif

#ifdef _SERVER

//

BOOL	KPlayerTong::TransferAddApply(DWORD dwNpcID)
{
	if (m_nFlag)
		return FALSE;
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp != camp_free)
		return FALSE;
	if (Player[m_nPlayerIndex].CheckTrading())
		return FALSE;
	
	Player[m_nPlayerIndex].m_cShop.GetState();
	
	if (Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		return FALSE;

	int	nTarget = Player[m_nPlayerIndex].FindAroundPlayer(dwNpcID);
	if (nTarget == -1)
		return FALSE;
	if (!Player[nTarget].m_cTong.CheckAcceptAddApplyCondition())
		return FALSE;

	m_nApplyTo = nTarget;

	TONG_APPLY_ADD_SYNC	sAdd;
	sAdd.ProtocolType = s2c_extendtong;
	sAdd.m_btMsgId = enumTONG_SYNC_ID_TRANSFER_ADD_APPLY;
	sAdd.m_nPlayerIdx = m_nPlayerIndex;
	strcpy(sAdd.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sAdd.m_wLength = sizeof(TONG_APPLY_ADD_SYNC) - 1 - sizeof(sAdd.m_szName) + strlen(sAdd.m_szName);
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nTarget].m_nNetConnectIdx, &sAdd, sAdd.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckAcceptAddApplyCondition()
{
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MEMBER)
		return FALSE;

	return TRUE;
}
#endif

#ifndef _SERVER

//

void	KPlayerTong::AcceptMember(int nPlayerIdx, DWORD dwNameID, BOOL bFlag)
{
	if (nPlayerIdx <= 0)
		return;

	TONG_ACCEPT_MEMBER_COMMAND	sAccept;
	sAccept.ProtocolType	= c2s_extendtong;
	sAccept.m_wLength		= sizeof(TONG_ACCEPT_MEMBER_COMMAND) - 1;
	sAccept.m_btMsgId		= enumTONG_COMMAND_ID_ACCEPT_ADD;
	sAccept.m_nPlayerIdx	= nPlayerIdx;
	sAccept.m_dwNameID		= dwNameID;
	sAccept.m_btFlag		= (bFlag != 0);

	if (g_pClient)
		g_pClient->SendPackToServer(&sAccept, sAccept.m_wLength + 1);
}
#endif

#ifdef _SERVER

//	

void	KPlayerTong::SendRefuseMessage(int nPlayerIdx, DWORD dwNameID)
{
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[nPlayerIdx].m_cTong.m_nApplyTo != m_nPlayerIndex ||
		Player[nPlayerIdx].m_nIndex <= 0 ||
		Player[m_nPlayerIndex].m_nIndex <= 0)
		return;
	if (g_FileName2Id(Npc[Player[nPlayerIdx].m_nIndex].Name) != dwNameID)
		return;
	
	int nLength = strlen(Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TONG_REFUSE_ADD;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength;
	sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];

	memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
	memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), Npc[Player[m_nPlayerIndex].m_nIndex].Name, nLength);

	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
}
#endif


//	

void	KPlayerTong::GetTongName(char *lpszGetName)
{
	if (!lpszGetName)
		return;
	if (!m_nFlag)
	{
		lpszGetName[0] = 0;
		return;
	}

	strcpy(lpszGetName, m_szName);
}


//	

void	KPlayerTong::GetTongTitle(char* lpszGetTitle)
{
	if (!lpszGetTitle)
		return;

	if (!m_nFlag)
	{
		lpszGetTitle[0] = 0;
		return;
	}

	strcpy(lpszGetTitle, m_szTitle);
}

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckAddCondition(int nPlayerIdx)
{
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MEMBER)
		return FALSE;

	if (Player[nPlayerIdx].m_cTong.m_nApplyTo != this->m_nPlayerIndex)
		return FALSE;

	if (Player[nPlayerIdx].m_cTong.m_nFlag)
		return FALSE;

	if (Npc[Player[nPlayerIdx].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[nPlayerIdx].m_nIndex].m_Camp != camp_free)
		return FALSE;

	if (Player[nPlayerIdx].CheckTrading())
		return FALSE;
	
	if (Player[nPlayerIdx].m_cShop.GetState())
		return FALSE;
	
	if (Player[nPlayerIdx].m_cTeam.m_nFlag)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::AddTong(int nCamp, char *lpszTongName, char *lpszMasterName, char *lpszTitleName)
{
	if (!lpszTongName || !lpszMasterName || !lpszTitleName)
		return FALSE;
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return FALSE;

	m_nFlag		= 1;
	m_nFigure	= enumTONG_FIGURE_MEMBER;
	m_nCamp		= nCamp;
	strcpy(this->m_szName, lpszTongName);
	strcpy(this->m_szMasterName, lpszMasterName);
	strcpy(this->m_szTitle, lpszTitleName);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;

	TONG_Add_SYNC	sAdd;
	sAdd.ProtocolType = s2c_extendtong;
	sAdd.m_wLength = sizeof(sAdd) - 1;
	sAdd.m_btMsgId = enumTONG_SYNC_ID_ADD;
	sAdd.m_btCamp = this->m_nCamp;
	strcpy(sAdd.m_szTongName, m_szName);
	strcpy(sAdd.m_szTitle, m_szTitle);
	strcpy(sAdd.m_szMaster, m_szMasterName);

	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sAdd, sAdd.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::AddTong(int nCamp, char *lpszTongName, char *lpszTitle, char *lpszMaster)
{
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
		return FALSE;

	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return FALSE;

	m_nFlag		= 1;
	m_nFigure	= enumTONG_FIGURE_MEMBER;
	m_nCamp		= nCamp;
	strcpy(m_szName, lpszTongName);
	if (!lpszTitle)
		this->m_szTitle[0] = 0;
	else
		strcpy(m_szTitle, lpszTitle);
	if (!lpszMaster)
		this->m_szMasterName[0] = 0;
	else
		strcpy(m_szMasterName, lpszMaster);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;

	SendSystemMsg(MSG_TONG_ADD_SUCCESS);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::ApplyInstate(int nCurFigure, int nCurPos, int nNewFigure, int nNewPos, char *lpszName)
{
	if (!lpszName)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (nCurFigure < 0 || nCurFigure >= enumTONG_FIGURE_NUM ||
		nNewFigure < 0 || nNewFigure >= enumTONG_FIGURE_NUM)
		return FALSE;
	if (nCurPos < 0 || nNewPos < 0)
		return FALSE;
	if (nCurFigure == nNewFigure && nCurPos == nNewPos)
		return FALSE;

	switch (m_nFigure)
	{
	case enumTONG_FIGURE_MEMBER:
		return FALSE;
	case enumTONG_FIGURE_MANAGER:
		return FALSE;
	case enumTONG_FIGURE_DIRECTOR:
		if ((nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER) ||
			(nNewFigure != enumTONG_FIGURE_MANAGER && nNewFigure != enumTONG_FIGURE_MEMBER))
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		break;
	case enumTONG_FIGURE_MASTER:
		if (nCurFigure == enumTONG_FIGURE_MASTER || nNewFigure == enumTONG_FIGURE_MASTER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_DIRECTOR && nCurPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_DIRECTOR && nNewPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		break;
	default:
		return FALSE;
	}

	TONG_APPLY_INSTATE_COMMAND	sApply;

	if (strlen(lpszName) >= sizeof(sApply.m_szName))
		return FALSE;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_INSTATE;
	sApply.m_dwTongNameID = g_FileName2Id(this->m_szName);
	sApply.m_btCurFigure = nCurFigure;
	sApply.m_btCurPos = nCurPos;
	sApply.m_btNewFigure = nNewFigure;
	sApply.m_btNewPos = nNewPos;
	memset(sApply.m_szName, 0, sizeof(sApply.m_szName));
	strcpy(sApply.m_szName, lpszName);
	sApply.m_wLength = sizeof(sApply) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::ApplyKick(int nCurFigure, int nCurPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER)
		return FALSE;
	if (nCurFigure == enumTONG_FIGURE_MANAGER && (nCurPos < 0 || nCurPos >= defTONG_MAX_MANAGER))
		return FALSE;

	TONG_APPLY_KICK_COMMAND	sKick;
	sKick.ProtocolType		= c2s_extendtong;
	sKick.m_wLength			= sizeof(sKick) - 1;
	sKick.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_KICK;
	sKick.m_btFigure		= nCurFigure;
	sKick.m_btPos			= nCurPos;
	sKick.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sKick.m_szName, lpszName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sKick, sKick.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::ApplyChangeMaster(int nCurFigure, int nPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;

	TONG_APPLY_CHANGE_MASTER_COMMAND	sChange;
	sChange.ProtocolType	= c2s_extendtong;
	sChange.m_wLength		= sizeof(sChange) - 1;
	sChange.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_CHANGE_MASTER;
	sChange.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sChange.m_btFigure		= nCurFigure;
	sChange.m_btPos			= nPos;
	strcpy(sChange.m_szName, lpszName);
	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//	

BOOL	KPlayerTong::ApplyLeave()
{
	if(!m_nFlag)
		return FALSE;
	if(m_nFigure == enumTONG_FIGURE_MASTER)
	{
		SendSystemMsg(MSG_TONG_CANNOT_LEAVE1);
		return FALSE;
	}
	if(m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		SendSystemMsg(MSG_TONG_CANNOT_LEAVE2);
		return FALSE;
	}
	if(Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment) < PlayerSet.m_sTongParam.m_nLeaveMoney)
	{
		SendSystemMsg(MSG_SHOP_NO_MONEY);
		return FALSE;
	}

	TONG_APPLY_LEAVE_COMMAND	sLeave;
	sLeave.ProtocolType		= c2s_extendtong;
	sLeave.m_wLength		= sizeof(sLeave) - 1;
	sLeave.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_LEAVE;
	sLeave.m_btFigure		= m_nFigure;
	sLeave.m_btPos			= 0;
	sLeave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sLeave.m_szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	if (g_pClient)
		g_pClient->SendPackToServer(&sLeave, sLeave.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER

//

BOOL	KPlayerTong::ApplyInfo(int nInfoID, int nParam1, int nParam2, int nParam3, char *lpszName/*=NULL*/)
{
	if (nInfoID < 0 || nInfoID >= enumTONG_APPLY_INFO_ID_NUM)
		return FALSE;

	TONG_APPLY_INFO_COMMAND	sInfo;
	sInfo.ProtocolType = c2s_extendtong;
	sInfo.m_btMsgId = enumTONG_COMMAND_ID_APPLY_INFO;
	sInfo.m_btInfoID = nInfoID;
	sInfo.m_nParam1 = nParam1;
	sInfo.m_nParam2 = nParam2;
	sInfo.m_nParam3 = nParam3;

	switch (nInfoID)
	{
	case enumTONG_APPLY_INFO_ID_SELF:
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;

	case enumTONG_APPLY_INFO_ID_MASTER:
		break;

	case enumTONG_APPLY_INFO_ID_DIRECTOR:
		break;

	case enumTONG_APPLY_INFO_ID_MANAGER:
		if (!m_nFlag)
			break;
		//
		if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR && 
			this->m_nFigure != enumTONG_FIGURE_MANAGER &&
			this->m_nFigure != enumTONG_FIGURE_MEMBER)
			break;
		

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;

	case enumTONG_APPLY_INFO_ID_MEMBER:
		if (!m_nFlag)
			break;
		//
		if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR && 
			this->m_nFigure != enumTONG_FIGURE_MANAGER &&
			this->m_nFigure != enumTONG_FIGURE_MEMBER)
			break;
		

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;

	case enumTONG_APPLY_INFO_ID_ONE:
		if (!lpszName || !lpszName[0])
			break;
		if (strlen(lpszName) > defPLAYER_NAME_MAX_LENGTH)
			break;
		strcpy(sInfo.m_szBuf, lpszName);
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf) + strlen(lpszName);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	case enumTONG_APPLY_INFO_ID_TONG_HEAD:
		if (nParam1 <= 0 || nParam1 >= MAX_NPC)
			break;
		if (nParam1 != Player[CLIENT_PLAYER_INDEX].m_nIndex && Npc[nParam1].m_nTongFlag == 0)
			break;
		sInfo.m_nParam1 = Npc[nParam1].m_dwID;
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	}
	return TRUE;
}
#endif


//	

DWORD	KPlayerTong::GetTongNameID()
{
	return (m_nFlag ? m_dwTongNameID : 0);
}


//	

BOOL	KPlayerTong::CanGetManagerInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	if (m_nFigure >= enumTONG_FIGURE_MEMBER || m_nFigure <= enumTONG_FIGURE_MASTER)
		return TRUE;
	return FALSE;
}


//	

BOOL	KPlayerTong::CanGetMemberInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	//
	if (m_nFigure >= enumTONG_FIGURE_MEMBER || m_nFigure <= enumTONG_FIGURE_MASTER)
		return TRUE;
	
	return FALSE;
}

#ifdef _SERVER

//

void	KPlayerTong::SendSelfInfo()
{
	TONG_SELF_INFO_SYNC	sInfo;
	sInfo.ProtocolType = s2c_extendtong;
	sInfo.m_wLength = sizeof(sInfo) - 1;
	sInfo.m_btMsgId = enumTONG_SYNC_ID_SELF_INFO;
	sInfo.m_btJoinFlag = this->m_nFlag;
	sInfo.m_btFigure = this->m_nFigure;
	sInfo.m_btCamp = this->m_nCamp;
	strcpy(sInfo.m_szMaster, this->m_szMasterName);
	strcpy(sInfo.m_szTongName, this->m_szName);
	if (g_pServer)
		g_pServer->PackDataToClient(Player[this->m_nPlayerIndex].m_nNetConnectIdx, &sInfo, sInfo.m_wLength + 1);
}
#endif

#ifndef _SERVER

//	

void	KPlayerTong::SetSelfInfo(TONG_SELF_INFO_SYNC *pInfo)
{
	if (pInfo->m_btJoinFlag == 0)
	{
		if (m_nFlag)
		{
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_LEAVE;
			sUi.nParam = m_nFigure;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);

			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = camp_free;
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = camp_free;
		}
		Clear();
		CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		return;
	}

	this->m_nFlag	= 1;
	this->m_nFigure = pInfo->m_btFigure;
	this->m_nCamp	= pInfo->m_btCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag == 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;
	memcpy(this->m_szMasterName, pInfo->m_szMaster, sizeof(pInfo->m_szMaster));
	memcpy(this->m_szName, pInfo->m_szTongName, sizeof(pInfo->m_szTongName));
	m_dwTongNameID	= g_FileName2Id(m_szName);
	
	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckInstatePower(TONG_APPLY_INSTATE_COMMAND *pApply)
{
	if (!pApply)
		return FALSE;
	if (pApply->m_wLength <= sizeof(TONG_APPLY_INSTATE_COMMAND) - 1 - sizeof(pApply->m_szName))
		return FALSE;
	if (!m_nFlag || (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR))
		return FALSE;
	if (pApply->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if (pApply->m_btCurFigure == pApply->m_btNewFigure && pApply->m_btCurPos == pApply->m_btNewPos)
		return FALSE;

	char	szName[32];
	memcpy(szName, pApply->m_szName, sizeof(pApply->m_szName));
	szName[31] = 0;
	if (strlen(szName) >= 31)
		return FALSE;

	if (m_nFigure == enumTONG_FIGURE_MASTER)
	{
		if (pApply->m_btCurFigure == enumTONG_FIGURE_DIRECTOR)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_DIRECTOR)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}

		if (pApply->m_btNewFigure == enumTONG_FIGURE_DIRECTOR)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_DIRECTOR)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (pApply->m_btCurFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}

		if (pApply->m_btNewFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckKickPower(TONG_APPLY_KICK_COMMAND *pKick)
{
	if (!pKick)
		return FALSE;
	if (pKick->m_wLength + 1 != sizeof(TONG_APPLY_KICK_COMMAND))
		return FALSE;
	if (!m_nFlag || (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR))
		return FALSE;
	if (pKick->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;

	if (pKick->m_btFigure == enumTONG_FIGURE_MANAGER)
	{
		if (pKick->m_btPos >= defTONG_MAX_MANAGER)
			return FALSE;
	}
	else if (pKick->m_btFigure == enumTONG_FIGURE_MEMBER)
	{
	}
	else
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER

//	

void	KPlayerTong::BeInstated(STONG_SERVER_TO_CORE_BE_INSTATED *pSync)
{
	this->m_nFlag = 1;
	this->m_nFigure = pSync->m_btFigure;
	strcpy(m_szTitle, pSync->m_szTitle);

	SendSelfInfo();
}
#endif

#ifdef _SERVER

//	

void	KPlayerTong::BeKicked(STONG_SERVER_TO_CORE_BE_KICKED *pSync)
{
	Clear();

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
	if (!Player[this->m_nPlayerIndex].m_cTeam.m_nFlag)
		Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = camp_free;

	SendSelfInfo();

	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TONG_BE_KICK;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckLeavePower(TONG_APPLY_LEAVE_COMMAND *pLeave)
{
	if (!pLeave)
		return FALSE;
	if (pLeave->m_wLength + 1 != sizeof(TONG_APPLY_LEAVE_COMMAND))
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (pLeave->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if(Player[m_nPlayerIndex].m_ItemList.GetMoney(room_equipment) < PlayerSet.m_sTongParam.m_nLeaveMoney)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER

//	

void	KPlayerTong::Leave(STONG_SERVER_TO_CORE_LEAVE *pLeave)
{
	if (strcmp(pLeave->m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name) != 0)
		return;

	if (pLeave->m_bSuccessFlag)
	{
		Clear();
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
		if (!Player[this->m_nPlayerIndex].m_cTeam.m_nFlag)
			Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = camp_free;
		SendSelfInfo();

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TONG_LEAVE_SUCCESS;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	}
	else
	{
		SendSelfInfo();

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TONG_LEAVE_FAIL;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	}
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckChangeMasterPower(TONG_APPLY_CHANGE_MASTER_COMMAND *pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_APPLY_CHANGE_MASTER_COMMAND))
		return FALSE;
	if (!m_nFlag || m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pChange->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	char	szName[32];
	memcpy(szName, pChange->m_szName, sizeof(pChange->m_szName));
	szName[31] = 0;
	if (strlen(szName) >= 31)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER

//	

BOOL	KPlayerTong::CheckGetMasterPower(STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER *pCheck)
{
	if (!pCheck)
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pCheck->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if (strcmp(pCheck->m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name) != 0)
		return FALSE;
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel || 
		(int)Player[m_nPlayerIndex].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER

//	

void	KPlayerTong::ChangeAs(STONG_SERVER_TO_CORE_CHANGE_AS *pAs)
{
	if (!pAs)
		return;
	if (!m_nFlag)
		return;
	if (pAs->m_dwTongNameID != g_FileName2Id(m_szName))
		return;

	m_nFigure = pAs->m_btFigure;
	strcpy(m_szTitle, pAs->m_szTitle);
	strcpy(this->m_szMasterName, pAs->m_szName);

	this->SendSelfInfo();

	SHOW_MSG_SYNC	sMsg;

	sMsg.ProtocolType = s2c_msgshow;
	if (m_nFigure == enumTONG_FIGURE_MASTER)
		sMsg.m_wMsgID = enumMSG_ID_TONG_CHANGE_AS_MASTER;
	else
		sMsg.m_wMsgID = enumMSG_ID_TONG_CHANGE_AS_MEMBER;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

	return;
}
#endif

#ifdef _SERVER

//

void	KPlayerTong::ChangeMaster(char *lpszMaster)
{
	if (!lpszMaster || !lpszMaster[0])
		return;
	if (strlen(lpszMaster) >= sizeof(this->m_szMasterName))
		return;
	strcpy(m_szMasterName, lpszMaster);

	this->SendSelfInfo();
}
#endif

#ifdef _SERVER

//

void	KPlayerTong::Login(STONG_SERVER_TO_CORE_LOGIN *pLogin)
{
	if (strcmp(Npc[Player[this->m_nPlayerIndex].m_nIndex].Name, pLogin->m_szName) != 0)
		return;

	if (pLogin->m_nFlag == 0)
	{
		Clear();
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
		if (!Player[m_nPlayerIndex].m_cTeam.m_nFlag)
			Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
		this->SendSelfInfo();

		return;
	}

	m_nFlag			= 1;
	m_nFigure		= pLogin->m_nFigure;
	m_nCamp			= pLogin->m_nCamp;
	m_dwMoney		= pLogin->m_dwMoney;
	m_dwTongNameID	= g_FileName2Id(pLogin->m_szTongName);
	m_nApplyTo		= 0;
	strcpy(m_szName, pLogin->m_szTongName);
	strcpy(m_szTitle, pLogin->m_szTitle);
	strcpy(m_szMasterName, pLogin->m_szMaster);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	if (!Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
	this->SendSelfInfo();	
}
#endif

#ifndef _SERVER

//	

void    KPlayerTong::OpenCreateInterface()
{
	CoreDataChanged(GDCNI_OPEN_TONG_CREATE_SHEET, 1, 0);
}
#endif

#ifndef _SERVER



BOOL	KPlayerTong::ApplyChangeCamp(int nCamp)
{	
	if(!m_nFlag)
		return FALSE;
	if(this->m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if(nCamp == m_nCamp)
	{
		SendSystemMsg(MSG_TONG_CHANGE_CAMP_ERROR1);
		return FALSE;
	}
	if(nCamp < camp_justice || nCamp > camp_balance)
		return FALSE;

	TONG_CHANGE_CAMP_COMMAND		sChange;
	sChange.ProtocolType = c2s_extendtong;
	sChange.m_btMsgId = enumTONG_COMMAND_ID_APPLY_CHANGE_CAMP;
	sChange.m_dwTongNameID = m_dwTongNameID;
	sChange.m_btNewCamp = nCamp;
	sChange.m_wLength = sizeof(sChange) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER



BOOL	KPlayerTong::CheckChangeCampPower(TONG_CHANGE_CAMP_COMMAND* pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_CHANGE_CAMP_COMMAND))
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if(m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pChange->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if(pChange->m_btNewCamp == m_nCamp)
		return FALSE;
	if(pChange->m_btNewCamp < camp_justice || pChange->m_btNewCamp > camp_balance)
		return FALSE;
	if(m_dwMoney < PlayerSet.m_sTongParam.m_nChangePrice)
	{
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Bang héi", MSG_NO_TONG_MONEY, strlen(MSG_NO_TONG_MONEY));
		return FALSE;
	}
	
	pChange->m_nPrice = PlayerSet.m_sTongParam.m_nChangePrice;

	return TRUE;
}
#endif

#ifdef _SERVER



void	KPlayerTong::ChangeCamp(STONG_SERVER_TO_CORE_CHANGE_CAMP* pChange)
{
	if(!m_nFlag)
		return;
	if(m_dwTongNameID != pChange->m_dwTongNameID)
		return;

	m_dwMoney = pChange->m_dwMoney;
	m_nCamp = pChange->m_btNewCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = pChange->m_btNewCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = pChange->m_btNewCamp;

	this->SendSelfInfo();
}
#endif

#ifndef _SERVER



BOOL	KPlayerTong::ApplyChangeMemberTitle(const char* lpszTitle, BOOL bGender)
{	
	if(!m_nFlag)
		return FALSE;
	if(this->m_nFigure != enumTONG_FIGURE_MASTER &&
		this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	
	if(strlen(lpszTitle) > defTONG_TITLE_MAX_LENGTH)
		return FALSE;

	TONG_CHANGE_MEMBER_TITLE_COMMAND		sChange;
	sChange.ProtocolType = c2s_extendtong;
	sChange.m_btMsgId = enumTONG_COMMAND_ID_APPLY_CHANGE_MEMBER_TITLE;
	sChange.m_dwTongNameID = g_FileName2Id(this->m_szName);
	strcpy(sChange.m_szTitle, lpszTitle);
	sChange.m_bGender = bGender;
	sChange.m_wLength = sizeof(sChange) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}
#endif


#ifdef _SERVER



BOOL	KPlayerTong::CheckChangeMemberTitlePower(TONG_CHANGE_MEMBER_TITLE_COMMAND* pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_CHANGE_MEMBER_TITLE_COMMAND))
		return FALSE;
	if (!m_nFlag || m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pChange->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if(strlen(pChange->m_szTitle) > defTONG_TITLE_MAX_LENGTH)
		return FALSE;

	return TRUE;
}
#endif

#ifndef _SERVER



BOOL	KPlayerTong::ApplyChangeOneTitle(const char* lpszName, BYTE bFigureFlag, const char* lpszTitle)
{	
	if(!m_nFlag)
		return FALSE;
	if(this->m_nFigure != enumTONG_FIGURE_MASTER &&
		this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if(bFigureFlag < enumTONG_FIGURE_MANAGER)
		return FALSE;
	if(strlen(lpszName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;
	if(strlen(lpszTitle) > defTONG_TITLE_MAX_LENGTH)
		return FALSE;

	TONG_CHANGE_ONE_TITLE_COMMAND		sChange;
	sChange.ProtocolType = c2s_extendtong;
	sChange.m_btMsgId = enumTONG_COMMAND_ID_APPLY_CHANGE_ONE_TITLE;
	sChange.m_dwTongNameID = g_FileName2Id(this->m_szName);
	strcpy(sChange.m_szName, lpszName);
	strcpy(sChange.m_szTitle, lpszTitle);
	sChange.m_bFigureFlag = bFigureFlag;
	sChange.m_wLength = sizeof(sChange) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER



BOOL	KPlayerTong::CheckChangeOneTitlePower(TONG_CHANGE_ONE_TITLE_COMMAND* pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_CHANGE_ONE_TITLE_COMMAND))
		return FALSE;
	if(!m_nFlag)
		return FALSE;
	if(this->m_nFigure != enumTONG_FIGURE_MASTER &&
		this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (pChange->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if(strlen(pChange->m_szName) > defPLAYER_NAME_MAX_LENGTH)
		return FALSE;
	if(strlen(pChange->m_szTitle) > defTONG_TITLE_MAX_LENGTH)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER



void	KPlayerTong::ChangeTitle(const char *lpszTitle)
{
	if(!lpszTitle || !lpszTitle[0])
		return;
	if(!m_nFlag)
		return;

	strcpy(m_szTitle, lpszTitle);
	
	this->SendSelfInfo();
}
#endif

#ifndef _SERVER



BOOL	KPlayerTong::ApplySaveMoney(int nMoney)
{
	if(!m_nFlag)
		return FALSE;
	if(nMoney <= 0)
		return FALSE;
	
	if(nMoney < MIN_TONG_MONEY_VALUE)
	{	
		char szMsg[64];
		sprintf(szMsg, MSG_TONG_SAVE_MONEY_MIN, MIN_TONG_MONEY_VALUE);
		SendSystemMsg(szMsg);
		return FALSE;
	}
	if(Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment) < nMoney)
	{
		SendSystemMsg(MSG_SHOP_NO_MONEY);
		return FALSE;
	}
	TONG_APPLY_SAVE_MONEY_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_SAVE_MONEY;
	sSave.m_dwTongNameID	= m_dwTongNameID;
	sSave.m_nMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER



BOOL	KPlayerTong::CheckSaveMoneyPower(TONG_APPLY_SAVE_MONEY_COMMAND* pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_APPLY_SAVE_MONEY_COMMAND))
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (pChange->m_dwTongNameID != m_dwTongNameID)
		return FALSE;
	if(pChange->m_nMoney <= 0)
		return FALSE;
	if(pChange->m_nMoney < MIN_TONG_MONEY_VALUE)
		return FALSE;
	if(Player[m_nPlayerIndex].m_ItemList.GetMoney(room_equipment) < pChange->m_nMoney)
		return FALSE;
	
	Player[m_nPlayerIndex].Pay(pChange->m_nMoney);
	pChange->m_dwMoney = m_dwMoney;

	return TRUE;
}
#endif

#ifdef _SERVER



void	KPlayerTong::SaveMoney(STONG_SERVER_TO_CORE_SAVE_MONEY* pSave)
{
	if (!pSave)
		return;
	if (!m_nFlag)
		return;
	if (pSave->m_dwTongNameID != m_dwTongNameID)
		return;
	if(pSave->m_dwMoney <= 0)
		return;

	m_dwMoney = pSave->m_dwMoney;
}
#endif

#ifndef _SERVER



BOOL	KPlayerTong::ApplyGetMoney(int nMoney)
{
	if(!m_nFlag)
		return FALSE;
	if(m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if(nMoney <= 0)
		return FALSE;
	
	if(nMoney < MIN_TONG_MONEY_VALUE)
	{	
		char szMsg[64];
		sprintf(szMsg, MSG_TONG_GET_MONEY_MIN, MIN_TONG_MONEY_VALUE);
		SendSystemMsg(szMsg);
		return FALSE;
	}
	
	TONG_APPLY_GET_MONEY_COMMAND	cGet;
	cGet.ProtocolType		= c2s_extendtong;
	cGet.m_wLength			= sizeof(cGet) - 1;
	cGet.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_GET_MONEY;
	cGet.m_dwTongNameID	= m_dwTongNameID;
	cGet.m_nMoney			= nMoney;
	strcpy(cGet.m_szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	if (g_pClient)
		g_pClient->SendPackToServer(&cGet, cGet.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER



BOOL	KPlayerTong::CheckGetMoneyPower(TONG_APPLY_GET_MONEY_COMMAND* pChange)
{
	if(!pChange)
		return FALSE;
	if(pChange->m_wLength + 1 != sizeof(TONG_APPLY_GET_MONEY_COMMAND))
		return FALSE;
	if(!m_nFlag)
		return FALSE;
	if(m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if(pChange->m_dwTongNameID != m_dwTongNameID)
		return FALSE;
	if(pChange->m_nMoney <= 0)
		return FALSE;
	if(pChange->m_nMoney < MIN_TONG_MONEY_VALUE)
		return FALSE;
	if(m_dwMoney < pChange->m_nMoney)
	{	
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Bang héi", MSG_NO_TONG_MONEY, strlen(MSG_NO_TONG_MONEY));
		return FALSE;
	}
	pChange->m_dwMoney = m_dwMoney;

	return TRUE;
}
#endif

#ifdef _SERVER



void	KPlayerTong::GetMoney(STONG_SERVER_TO_CORE_GET_MONEY* pGet, bool bSelf /*= true*/)
{
	if (!pGet)
		return;
	if (!m_nFlag)
		return;
	if (pGet->m_dwTongNameID != m_dwTongNameID)
		return;
	if(pGet->m_nMoney <= 0)
		return;
	
	if(bSelf)
	{
		Player[m_nPlayerIndex].Earn(pGet->m_nMoney);
	}
	m_dwMoney = pGet->m_dwMoney;
}
#endif




