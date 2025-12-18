
#ifndef KPLAYERTRADE_H
#define KPLAYERTRADE_H

#include	"KPlayerDef.h"

class KTrade
{
	friend class KPlayer;

private:
#ifdef _SERVER
	bool		m_bForbitTrade;
#endif

public:
	//
	BOOL		m_bIsTrading;
	int			m_nTradeDest;
	BOOL		m_bTradeState;
	BOOL		m_bTradeLock;
	
#ifdef _SERVER
	int			m_nApplyIdx;
#endif

#ifndef _SERVER
	int			m_nBackEquipMoney;					
	int			m_nBackRepositoryMoney;				
	BOOL		m_bTradeDestState;					
	BOOL		m_bTradeDestLock;					
	char		m_szDestName[32];					
#endif

public:
	KTrade();
	void		Release();
	BOOL		StartTrade(int nDest, int nEquipMoney = 0, int nRepositoryMoney = 0);
	void		Lock(BOOL bFlag);

#ifdef _SERVER
	void		SetForbitTrade(int nSelfIdx, bool bFlag);
	bool		GetForbitTrade() const { return m_bForbitTrade; };
#else
	void		SetTradeState(int nSelfLock, int nDestLock, int nSelfStateOk, int nDestStateOk);
	static void		ReplyInvite(unsigned int uId, int nReply);		//
#endif

};
#endif
