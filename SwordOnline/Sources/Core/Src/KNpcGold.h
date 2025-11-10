

#ifndef _KNPCGOLD_H
#define	_KNPCGOLD_H

#define		defMAX_NPC_GOLD_TYPE		16
#define		defMAX_AI_PARAM				11

class KNpcGold
{
	friend class KNpc;

private:
	int		m_nNpcIdx;
	USHORT	m_nGoldType;

	short	m_nFireResist;
	short	m_nFireResistMax;
	short	m_nColdResist;
	short	m_nColdResistMax;
	short	m_nLightingResist;
	short	m_nLightingResistMax;
	short	m_nPoisonResist;
	short	m_nPoisonResistMax;
	short	m_nPhycicsResist;
	short	m_nPhycicsResistMax;
	int		m_nAiMode;
	int		m_nAiParam[defMAX_AI_PARAM - 1];
	BYTE	m_nAiMaxTime;

private:
	void	UpdateNpcData(int nGoldType);
public:
	void	Init(int nIdx);

	void	Release();

	void	SetGoldType(BOOL bFlag);	
	void	SetGoldTypeAndBackData();					
	int		GetGoldType();
	void	SetGoldCurrentType(int nType);
	void	RecoverBackData();
#ifdef _SERVER
	void	RandChangeGold(int nRand, int nGoldType);
#endif
};

struct	KNpcGoldTemplateInfo
{
	int		m_nExp;						
	int		m_nLife;					
	int		m_nLifeReplenish;			
	int		m_nAttackRating;			
	int		m_nDefense;					
	int		m_nMinDamage;
	int		m_nMaxDamage;

	char	m_szSkillName[32];
	int		m_nSkillLevel;

	int		m_nTreasure;				
	int		m_nWalkSpeed;
	int		m_nRunSpeed;
	int		m_nAttackSpeed;
	int		m_nCastSpeed;

	int		m_nFireResist;				
	int		m_nFireResistMax;		
	int		m_nColdResist;				
	int		m_nColdResistMax;			
	int		m_nLightingResist;			
	int		m_nLightingResistMax;		
	int		m_nPoisonResist;			
	int		m_nPoisonResistMax;			
	int		m_nPhycicsResist;			
	int		m_nPhycicsResistMax;
	
	int		m_nAiMode;
	int		m_nAiParam[defMAX_AI_PARAM - 1];
	BYTE	m_nAiMaxTime;

	int		m_nPhysicalDamageBase;
	int		m_nPhysicalMagicBase;
	int		m_nPoisonDamageBase;
	int		m_nPoisonMagicBase;
	int		m_nColdDamageBase;
	int		m_nColdMagicBase;
	int		m_nFireDamageBase;
	int		m_nFireMagicBase;
	int		m_nLightingDamageBase;
	int		m_nLightingMagicBase;
};

class KNpcGoldTemplate
{
	friend class KNpcSet;
public:
	KNpcGoldTemplateInfo	m_sInfo[defMAX_NPC_GOLD_TYPE];
	int		m_nEffectTypeNum;
public:
	KNpcGoldTemplate();
	BOOL	Init();
};

#endif
