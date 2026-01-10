
#ifndef KPlayerSetH
#define	KPlayerSetH

#include "KLinkArray.h"

#define		MAX_LEVEL					200		//
#define		MAX_LEAD_LEVEL				100

typedef struct
{
	int		m_nExp;
	int		m_nMoney;
	int		m_nItem;
	int		m_nEquip;
} KPK_DEATH_PUNISH_PARAM;

typedef struct
{
	int		m_nLevel;
	int		m_nLeadLevel;
	int		m_nMoney;
	short	m_nItemIdx;
	short	m_nRepute;
	int		m_nChangePrice;
	int		m_nLeaveMoney;
} STONG_CREATE_PARAM;

class KLevelAdd
{
private:
	int		m_nLevelExp[MAX_LEVEL];
	int		m_nLifePerLevel[MAX_SERIES];
	int		m_nStaminaMalePerLevel[MAX_SERIES];		
	int		m_nStaminaFemalePerLevel[MAX_SERIES];	
	int		m_nManaPerLevel[MAX_SERIES];
	int		m_nLifePerVitality[MAX_SERIES];
	int		m_nStaminaPerVitality[MAX_SERIES];
	int		m_nManaPerEnergy[MAX_SERIES];
	int		m_nLeadExpShare[MAX_SERIES];
	int		m_nFireResistPerLevel[MAX_SERIES];
	int		m_nColdResistPerLevel[MAX_SERIES];
	int		m_nPoisonResistPerLevel[MAX_SERIES];
	int		m_nLightResistPerLevel[MAX_SERIES];
	int		m_nPhysicsResistPerLevel[MAX_SERIES];
public:
	KLevelAdd();
	BOOL		Init();
	int			GetLevelExp(BYTE btLevel);
	int			GetLifePerLevel(int nSeries);
	int			GetStaminaPerLevel(int nSeries, BOOL bSex);
	int			GetManaPerLevel(int nSeries);
	int			GetLifePerVitality(int nSeries);
	int			GetStaminaPerVitality(int nSeries);
	int			GetManaPerEnergy(int nSeries);
	int			GetLeadExpShare(int nSeries);
	int			GetFireResist(int nSeries, int nLevel);
	int			GetColdResist(int nSeries, int nLevel);
	int			GetPoisonResist(int nSeries, int nLevel);
	int			GetLightResist(int nSeries, int nLevel);
	int			GetPhysicsResist(int nSeries, int nLevel);
};


class KTeamLeadExp
{
public:
	struct LEADLEVEL
	{
		DWORD	m_dwExp;								
		DWORD	m_dwMemNum;								
	}	m_sPerLevel[MAX_LEAD_LEVEL];
public:
	KTeamLeadExp();
	BOOL		Init();									
	int			GetLevel(DWORD dwExp, int nCurLeadLevel = 0);
	int			GetMemNumFromExp(DWORD dwExp);			
	int			GetMemNumFromLevel(int nLevel);			
	int			GetLevelExp(int nLevel);			
};

class KNewPlayerAttribute
{
public:
	int			m_nStrength[series_num];
	int			m_nDexterity[series_num];
	int			m_nVitality[series_num];
	int			m_nEngergy[series_num];
	int			m_nLucky[series_num];
public:
	KNewPlayerAttribute();
	BOOL		Init();
};

class KPlayerSet
{
private:
	KLinkArray		m_FreeIdx;				
	KLinkArray		m_UseIdx;				
	int				m_nListCurIdx;
#ifdef _SERVER
	unsigned long	m_ulNextSaveTime;
	unsigned long	m_ulDelayTimePerSave;
	unsigned long	m_ulMaxSaveTimePerPlayer;
	
	BYTE			m_bLock;
	short			m_nExpRate;
	short			m_nMoneyRate;
	
#endif
public:
	KLevelAdd				m_cLevelAdd;
	KTeamLeadExp			m_cLeadExp;
	KNewPlayerAttribute		m_cNewPlayerAttribute;
	STONG_CREATE_PARAM		m_sTongParam;

#ifdef _SERVER
	KPK_DEATH_PUNISH_PARAM	m_sPKPunishParam[MAX_DEATH_PUNISH_PK_VALUE + 1];
	
	KTaskObject*		m_pResetTask;
	short				m_nResetTaskCount;
	KTaskObject*		m_pSyncTask;
	short				m_nSyncTaskCount;
	
#endif

#ifndef _SERVER
	char					m_szFortuneRankPic[MAX_ITEM_LEVEL+1][MAX_PATH];
#endif

public:
	KPlayerSet();
	~KPlayerSet();
	BOOL	Init();
	int		FindSame(DWORD dwID);
	int		GetFirstPlayer();				
	int		GetNextPlayer();
	int		GetOnlinePlayerCount() { return m_UseIdx.GetCount(); }

#ifdef	_SERVER
	void	Activate();
	int		Add(LPSTR szPlayerID, void* pGuid);
	void	PrepareRemove(int nClientIdx);
	void	PrepareExchange(int nIndex);
	void	PrepareLoginFailed(int nIndex);
	void	RemoveQuiting(int nIndex);
	void	RemoveLoginTimeOut(int nIndex);
	void	RemoveExchanging(int nIndex);
	void	AutoSave();
	void	ProcessClientMessage(int nClient, const char* pChar, int nSize);
	int		GetPlayerNumber() { return m_nNumPlayer; }
	BOOL	GetPlayerName(int nClient, char* szName);
	BOOL	GetPlayerAccount(int nClient, char* szName);
	int		Broadcasting(char* pMessage, int nLen);
	void	SetSaveBufferPtr(void* pData);
	int		AttachPlayer(const unsigned long lnID, GUID* pGuid);
	int		GetPlayerIndexByGuid(GUID* pGuid);
	
	short	GetExpRate() const { return m_nExpRate; };
	short	GetMoneyRate() const { return m_nMoneyRate; };
	int		GetFirstPlayerEx();				
	int		GetNextPlayerEx(int nCurIdx);
	
#endif

private:
	int		FindFree();

#ifdef	_SERVER

private:
	int		m_nNumPlayer;
#endif
};

extern KPlayerSet PlayerSet;
#endif //KPlayerSetH
