
#include	"KCore.h"
#include	"KPlayer.h"
#include	"KPlayerDef.h"
#include	"KPlayerPK.h"

#ifndef _SERVER
#include "CoreShell.h"
#endif

#ifdef _SERVER

//	

KPlayerPK::KPlayerPK()
{
	m_nPlayerIndex = 0;
}


//	

void	KPlayerPK::Init(int nPlayerIdx)
{
	this->m_nPlayerIndex = nPlayerIdx;

	m_btNormalPKFlag		= 0;
	m_nEnmityPKState		= enumPK_ENMITY_STATE_CLOSE;
	m_nEnmityPKAim			= 0;
	m_nEnmityPKTime			= 0;
	m_nEnmityPKLaunchFlag	= 0;
	m_nExercisePKFlag		= 0;
	m_nExercisePKAim		= 0;
	m_cPKValue				= 0;
	
	m_bForbidChangePK		= false;
	
}


//	

void	KPlayerPK::Active()
{
	EnmityPKCountDown();
}




void	KPlayerPK::SetNormalPKState(BYTE btFlag)
{	
	if(GetPKValue() >= MAX_DEATH_PUNISH_PK_VALUE)
		return;

	
	if(m_bForbidChangePK)
	{
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Nh¾c nhë", MSG_FORBID_CHANGE_PK, strlen(MSG_FORBID_CHANGE_PK));
		return;
	}
	

	if(btFlag > 1)
	{
		if(Player[m_nPlayerIndex].m_nExp < 0)
			return;
	}

	m_btNormalPKFlag = btFlag;

	PK_NORMAL_FLAG_SYNC	sFlag;
	sFlag.ProtocolType = s2c_pksyncnormalflag;
	sFlag.m_btFlag = m_btNormalPKFlag;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sFlag, sizeof(PK_NORMAL_FLAG_SYNC));
}


//

BYTE	KPlayerPK::GetNormalPKState()
{
	return this->m_btNormalPKFlag;
}


//	

void	KPlayerPK::EnmityPKClose()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_CLOSE)
		return;

	PK_ENMITY_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncenmitystate;
	sState.m_btState = enumPK_ENMITY_STATE_CLOSE;
	sState.m_dwNpcID = 0;
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	if (m_nEnmityPKAim > 0 && m_nEnmityPKAim < MAX_PLAYER)
	{
		g_pServer->PackDataToClient(Player[m_nEnmityPKAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKAim = 0;
		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKTime = 0;
	}
	this->m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
	this->m_nEnmityPKAim = 0;
	this->m_nEnmityPKTime = 0;
	m_nEnmityPKLaunchFlag = 0;
}




BOOL	KPlayerPK::EnmityPKOpen(int nAim)
{	
	
	if(m_bForbidChangePK)
		return FALSE;
	if(Player[m_nPlayerIndex].m_nExp < 0)
		return FALSE;
	

	if (nAim <= 0 || nAim >= MAX_PLAYER || Player[nAim].m_nIndex < 1)
		return FALSE;

	if (!Npc[Player[m_nPlayerIndex].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	if (!Npc[Player[nAim].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_6;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp == camp_begin)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (m_nExercisePKFlag == TRUE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_3;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (Player[nAim].m_cPK.GetExercisePKState())
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_7;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_4;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_begin)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_5;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_event)
	{
		return FALSE;
	}
	

	Player[nAim].m_cPK.EnmityPKClose();
	Player[nAim].m_cPK.m_nEnmityPKState = enumPK_ENMITY_STATE_TIME;
	Player[nAim].m_cPK.m_nEnmityPKAim = m_nPlayerIndex;
	Player[nAim].m_cPK.m_nEnmityPKTime = PK_ANMITY_TIME;
	Player[nAim].m_cPK.m_nEnmityPKLaunchFlag = 0;

	m_nEnmityPKState = enumPK_ENMITY_STATE_TIME;
	m_nEnmityPKAim = nAim;
	m_nEnmityPKTime = PK_ANMITY_TIME;
	m_nEnmityPKLaunchFlag = 1;

	PK_ENMITY_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncenmitystate;
	sState.m_btState = enumPK_ENMITY_STATE_TIME;

	sState.m_dwNpcID = Npc[Player[m_nPlayerIndex].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[nAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	sState.m_dwNpcID = Npc[Player[nAim].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[nAim].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	return TRUE;
}


//

int		KPlayerPK::GetEnmityPKState()
{
	return this->m_nEnmityPKState;
}


//

int		KPlayerPK::GetEnmityPKAim()
{
	return this->m_nEnmityPKAim;
}


//

void	KPlayerPK::ExercisePKClose()
{
	if (m_nExercisePKFlag == FALSE)
		return;

	PK_EXERCISE_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncexercisestate;
	sState.m_btState = 0;
	sState.m_dwNpcID = 0;
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	if (m_nExercisePKAim > 0 && m_nExercisePKAim < MAX_PLAYER)
	{
		g_pServer->PackDataToClient(Player[m_nExercisePKAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

		Player[m_nExercisePKAim].m_cPK.m_nExercisePKFlag = FALSE;
		Player[m_nExercisePKAim].m_cPK.m_nExercisePKAim = 0;
	}
	this->m_nExercisePKFlag = FALSE;
	this->m_nExercisePKAim = 0;
}


//

BOOL	KPlayerPK::ExercisePKOpen(int nAim)
{
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp == camp_begin)
		return FALSE;

	if (m_nExercisePKFlag == TRUE)
		return FALSE;

	if (nAim <= 0 || nAim >= MAX_PLAYER || Player[nAim].m_nIndex < 1)
		return FALSE;

	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_begin)
		return FALSE;
	
	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_event)
		return FALSE;
	

	this->EnmityPKClose();
	Player[nAim].m_cPK.EnmityPKClose();
	this->m_nExercisePKFlag = TRUE;
	this->m_nExercisePKAim = nAim;
	Player[nAim].m_cPK.m_nExercisePKFlag = TRUE;
	Player[nAim].m_cPK.m_nExercisePKAim = this->m_nPlayerIndex;

	PK_EXERCISE_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncexercisestate;
	sState.m_btState = 1;

	sState.m_dwNpcID = Npc[Player[m_nPlayerIndex].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[nAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	sState.m_dwNpcID = Npc[Player[nAim].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[nAim].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	return TRUE;
}


//

int		KPlayerPK::GetExercisePKAim()
{
	return this->m_nExercisePKAim;
}


//	

void	KPlayerPK::SetPKValue(char cValue)
{
	this->m_cPKValue = (cValue < 0 ? 0 : cValue);
	if (m_cPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_cPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	PK_VALUE_SYNC	sValue;
	sValue.ProtocolType = s2c_pksyncpkvalue;
	sValue.m_cPKValue = m_cPKValue;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(PK_VALUE_SYNC));
}


//

char	KPlayerPK::GetPKValue() const
{
	return m_cPKValue;
}


//

void	KPlayerPK::AddPKValue(char cAdd)
{
	m_cPKValue += cAdd;
	if (m_cPKValue < 0)
		m_cPKValue = 0;
	if (m_cPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_cPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	PK_VALUE_SYNC	sValue;
	sValue.ProtocolType = s2c_pksyncpkvalue;
	sValue.m_cPKValue = m_cPKValue;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(PK_VALUE_SYNC));
}


//	

void	KPlayerPK::CloseAll()
{
	ExercisePKClose();
	EnmityPKClose();
}


//	

void	KPlayerPK::EnmityPKCountDown()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime > 0)
			return;
		m_nEnmityPKTime = PK_ANMITY_CLOSE_TIME;
		m_nEnmityPKState = enumPK_ENMITY_STATE_PKING;

		if (Player[m_nEnmityPKAim].m_nIndex <= 0)
			return;
		PK_ENMITY_STATE_SYNC	sState;
		sState.ProtocolType = s2c_pksyncenmitystate;
		sState.m_btState = enumPK_ENMITY_STATE_PKING;
		sState.m_dwNpcID = Npc[Player[m_nEnmityPKAim].m_nIndex].m_dwID;
		strcpy(sState.m_szName, Npc[Player[m_nEnmityPKAim].m_nIndex].Name);
		sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);
	}
	else if (m_nEnmityPKState == enumPK_ENMITY_STATE_PKING)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime > 0)
			return;
		m_nEnmityPKTime = 0;
		ExercisePKClose();
	}
}

#endif

#ifndef _SERVER


//

void	KPlayerPK::Init()
{
	m_nEnmityPKState		= enumPK_ENMITY_STATE_CLOSE;
	m_nEnmityPKAim			= 0;
	m_nEnmityPKTime			= 0;
	m_szEnmityAimName[0]	= 0;
	m_nExercisePKFlag		= 0;
	m_nExercisePKAim		= 0;
	m_szExerciseAimName[0]	= 0;
}


//

void	KPlayerPK::Active()
{
	EnmityPKCountDown();
}


//

void	KPlayerPK::SetNormalPKState(BYTE btFlag, BOOL bShowMsg/* = TRUE*/)
{	
	if(GetPKValue() >= MAX_DEATH_PUNISH_PK_VALUE)
		return;
	if (m_btNormalPKFlag == btFlag)
		return;

	m_btNormalPKFlag = btFlag;

	CoreDataChanged(GDCNI_PK_SETTING, 0, m_btNormalPKFlag);

	if (bShowMsg)
	{
		KSystemMessage	sMsg;
		if (btFlag == 1)
			sprintf(sMsg.szMessage, MSG_PK_NORMAL_FLAG_1);
		else if (btFlag == 0)
			sprintf(sMsg.szMessage, MSG_PK_NORMAL_FLAG_0);
		else
			sprintf(sMsg.szMessage, MSG_PK_NORMAL_FLAG_2);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}



//	

void	KPlayerPK::ApplySetNormalPKState(BYTE btFlag)
{
	PK_APPLY_NORMAL_FLAG_COMMAND	sApply;

	sApply.ProtocolType		= c2s_pkapplychangenormalflag;
	sApply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sApply.dwTimePacker = GetTickCount();
	sApply.m_btFlag			= btFlag;
				
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_NORMAL_FLAG_COMMAND));

}


//

BYTE	KPlayerPK::GetNormalPKState()
{
	return this->m_btNormalPKFlag;
}


//	

void	KPlayerPK::ApplyEnmityPK(char *lpszName)
{
	if (!lpszName || !lpszName[0])
		return;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchName(lpszName);
	if (nNpcIdx <= 0)
		return;
	ApplyEnmityPK(Npc[nNpcIdx].m_dwID);
}


//	

void	KPlayerPK::ApplyEnmityPK(int nNpcID)
{	
	
	if(Player[CLIENT_PLAYER_INDEX].m_nExp < 0)
	{		
		KSystemMessage	sMsg;
		strcpy(sMsg.szMessage, MSG_PK_ERROR_8);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}
	if(m_cPKValue == 10)
	{		
		KSystemMessage	sMsg;
		strcpy(sMsg.szMessage, MSG_PK_FULL);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}
	
	if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_PK_ERROR_4);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}
	
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_PK_ERROR_1);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}

	int		nIdx = NpcSet.SearchID(nNpcID);
	if (nIdx == 0 || Npc[nIdx].m_Kind != kind_player)
		return;

	PK_APPLY_ENMITY_COMMAND	sApply;
	sApply.ProtocolType		= c2s_pkapplyenmity;
	sApply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sApply.dwTimePacker = GetTickCount();
	sApply.m_dwNpcID		= nNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_ENMITY_COMMAND));
}


//	

void	KPlayerPK::SetEnmityPKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/) 
{
	if (nState == enumPK_ENMITY_STATE_CLOSE)
	{
		if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ENMITY_CLOSE);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}

		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = 0;
		m_nEnmityPKTime = 0;
		m_szEnmityAimName[0] = 0;
	}
	else if (nState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = nNpcID;
		m_nEnmityPKTime = PK_ANMITY_TIME;
		m_szEnmityAimName[0] = 0;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_SUCCESS_1, m_szEnmityAimName);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_SUCCESS_2);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}	
	else
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_PKING;
		m_nEnmityPKTime = 0;
		if (nNpcID > 0)
			m_nEnmityPKAim = nNpcID;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_OPEN);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}


//	

void	KPlayerPK::EnmityPKCountDown()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime < 0)
			m_nEnmityPKTime = 0;
	}
}


//	

void	KPlayerPK::SetExercisePKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/)
{
	SetEnmityPKState(enumPK_ENMITY_STATE_CLOSE);

	if (nState)
	{
		m_nExercisePKFlag = 1;
		m_nExercisePKAim = nNpcID;
		m_szExerciseAimName[0] = 0;
		if (lpszName)
			strcpy(m_szExerciseAimName, lpszName);
	}
	else
	{
		m_nExercisePKFlag = 0;
		m_nExercisePKAim = 0;
		m_szExerciseAimName[0] = 0;
	}
}


//	

void	KPlayerPK::SetPKValue(char cValue)
{
	if (m_cPKValue == cValue)
		return;

	this->m_cPKValue = (cValue < 0 ? 0 : cValue);
	if (m_cPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_cPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_PK_VALUE, (int)m_cPKValue);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

}


//	

void KPlayerPK::ChangePKState(BYTE btFlag)
{	
	if(GetPKValue() >= MAX_DEATH_PUNISH_PK_VALUE)
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.eType = SMT_NORMAL;
		Msg.byPriority = 1;
		Msg.byParamSize = 0;
		strcpy(Msg.szMessage, "ChØ sè PK qu¸ lín, kh«ng thÓ chuyÓn PK.");
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
		return;
	}
	
	if(Player[CLIENT_PLAYER_INDEX].m_bLockPlayer)//LockPlayer by kinnox;
		return;	
	
	switch(btFlag)
	{
		case 0:
			{	
				if(GetNormalPKState() == 1)
				{	
					if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
					{
						if(GetTickCount() - m_PKChangeTime >= 60000)
						{
							ApplySetNormalPKState(0);
						}
						else
						{	
							KSystemMessage	Msg;
							Msg.byConfirmType = SMCT_NONE;
							Msg.eType = SMT_NORMAL;
							Msg.byPriority = 1;
							Msg.byParamSize = 0;
							strcpy(Msg.szMessage, "Tr¹ng th¸i chiÕn ®Êu chuyÓn sang luyÖn c«ng cÇn 1 phót.");
							CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
						}
					}
					else
					{
						ApplySetNormalPKState(0);
					}
				}
				else if(GetNormalPKState() == 2)
				{		
					if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
					{
						if(GetTickCount() - m_PKChangeTime >= 60000)
						{
							ApplySetNormalPKState(0);
						}
						else
						{	
							KSystemMessage	Msg;
							Msg.byConfirmType = SMCT_NONE;
							Msg.eType = SMT_NORMAL;
							Msg.byPriority = 1;
							Msg.byParamSize = 0;
							strcpy(Msg.szMessage, "Tr¹ng th¸i ®ß s¸t chuyÓn sang luyÖn c«ng cÇn 1 phót.");
							CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
						}
					}
					else
					{
						ApplySetNormalPKState(0);
					}
				}
				else
				{
					break;
				}
			}
			break;
		case 1:
			{	
				if(!GetNormalPKState())
				{
					ApplySetNormalPKState(1);
					m_PKChangeTime = GetTickCount();
				}
				else if(GetNormalPKState() == 2)
				{	
					ApplySetNormalPKState(1);
					m_PKChangeTime = GetTickCount();
				}
				else
				{
					break;
				}
			}
				break;
		case 2:
			{	
				if(Player[CLIENT_PLAYER_INDEX].m_nExp < 0)
				{		
					KSystemMessage	sMsg;
					strcpy(sMsg.szMessage, MSG_PK_ERROR_8);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return;
				}

				if(GetNormalPKState() == 2)
				{
					break;
				}
				else
				{	
					ApplySetNormalPKState(2);
					m_PKChangeTime = GetTickCount();
				}
			}
			break;
	}
}
#endif













