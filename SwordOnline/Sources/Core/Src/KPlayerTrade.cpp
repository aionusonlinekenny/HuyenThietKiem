
#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTrade.h"
#ifndef _SERVER
#include	"CoreShell.h"
#endif

// --
//
// --
KTrade::KTrade()
{
	Release();
#ifdef _SERVER
	m_nApplyIdx = -1;
#endif
}

// --
//
// --
void	KTrade::Release()
{	
	//
	m_bIsTrading			= FALSE;
	m_nTradeDest			= -1;
	m_bTradeState			= FALSE;
	m_bTradeLock			= FALSE;
#ifdef _SERVER
	m_bForbitTrade			= false;
#else
	m_nBackEquipMoney		= 0;
	m_nBackRepositoryMoney	= 0;
	m_bTradeDestState		= FALSE;
	m_bTradeDestLock		= FALSE;
	m_szDestName[0]			= 0;
#endif
	
}

// --
//
// --
BOOL	KTrade::StartTrade(int nDest, int nEquipMoney, int nRepositoryMoney)
{	
	//
	if (m_bIsTrading)
		return FALSE;
	if (nDest < 0)
		return FALSE;

	m_bIsTrading = TRUE;
	m_nTradeDest = nDest;
	m_bTradeState = FALSE;
	m_bTradeLock = FALSE;
#ifndef _SERVER
	m_nBackEquipMoney = nEquipMoney;
	m_nBackRepositoryMoney = nRepositoryMoney;
	m_bTradeDestState = FALSE;
	m_bTradeDestLock = FALSE;
	int nIdx = NpcSet.SearchID(nDest);
	if (nIdx > 0)
		strcpy(m_szDestName, Npc[nIdx].Name);
	else
		m_szDestName[0] = 0;
#endif
	return TRUE;
	
}

// --
//
// --
void	KTrade::Lock(BOOL bFlag)
{	
	//
	if (bFlag)
	{
		m_bTradeLock = TRUE;
	}
	else
	{
		m_bTradeLock = FALSE;
#ifndef _SERVER
		m_bTradeDestLock = FALSE;
#endif
	}
	
}

#ifndef _SERVER
// --
//
// --
void	KTrade::SetTradeState(int nSelfLock, int nDestLock, int nSelfStateOk, int nDestStateOk)
{
	if (nSelfLock)
	{	
		if (m_bTradeLock == FALSE)		//
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_SELF_LOCK);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
	else
	{
		if (m_bTradeLock != FALSE)		//
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_SELF_UNLOCK);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}

	if (nDestLock)
	{
		if (m_bTradeDestLock == FALSE)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_DEST_LOCK, m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
	else
	{
		if (m_bTradeDestLock != FALSE)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_DEST_UNLOCK, m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
	}
	
	//
	this->m_bTradeLock = (nSelfLock != 0);
	this->m_bTradeDestLock = (nDestLock != 0);
	this->m_bTradeState = (nSelfStateOk != 0);
	this->m_bTradeDestState = (nDestStateOk != 0);
	
	CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
}
#endif

#ifndef _SERVER



void	KTrade::ReplyInvite(unsigned int uId, int nReply)
{	
	if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop)
		return;
	if(Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return;

	int nDestIdx = NpcSet.SearchID(uId);
	if(Npc[nDestIdx].m_bOpenShop)
		return;
	

	if (nDestIdx < 0)
		return;
	
	TRADE_REPLY_START_COMMAND	sReply;

	sReply.ProtocolType		= c2s_tradereplystart;
	sReply.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sReply.dwTimePacker = GetTickCount();
	sReply.m_bDecision		= (nReply != 0);
	sReply.m_dwNpcID		= (DWORD)uId;
	if (g_pClient)
		g_pClient->SendPackToServer(&sReply, sizeof(TRADE_REPLY_START_COMMAND));
}
#endif

#ifdef _SERVER
// --
//
// --
void KTrade::SetForbitTrade(int nSelfIdx, bool bFlag)
{
	m_bForbitTrade =  bFlag;
	if(!bFlag)
		return;
	if(Player[nSelfIdx].m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN)
		Player[nSelfIdx].m_cMenuState.SetState(nSelfIdx, PLAYER_MENU_STATE_NORMAL);
}
#endif
