#ifndef	KSkillListH
#define	KSkillListH
//#include "Windows.h"
#include "GameDataDef.h"

#define	MAX_NPCSKILL	80
#define	MAX_TRAPNUM		5

#define	MAX_LEVEL_SKILL_EXP				20
#define MIN_ID_SKILL120					708
#define MAX_ID_SKILL120					717


typedef struct tagNpcSkill
{	
	int		SkillId;
	int		SkillLevel;
	DWORD	NextCastTime;
	DWORD	MaxCastTimes;		
	int		CurrentSkillLevel;
	int		PointAdd;			
	bool	SkillExpand;		
} NPCSKILL, *LPNPCSKILL;
struct TDBPlayerData;
#ifndef TOOLVERSION
class KSkillList
#else 
class CORE_API KSkillList
#endif
{
public:
	int			m_nTrapNpcID[5];
	NPCSKILL	m_Skills[MAX_NPCSKILL];
	int			m_nNpcIndex;

#ifdef _SERVER
	short		m_nSkillId120;
	short		m_nLevelTask120;
	int			m_nNextExp120;
#endif

public:
	KSkillList();
	~KSkillList();
	int			Add(int nSkillID, int nLevel = 1, bool bExpand = false);
	void		Remove(int nSkillID);		//
	int			GetSkillId(unsigned long ulSkillIdx) const
	{
		if (ulSkillIdx >= MAX_NPCSKILL || ulSkillIdx == 0) 
			return 0;
		else 
			return m_Skills[ulSkillIdx].SkillId;
	};


	int	SetSkillLevelDirectlyUsingIndex(unsigned long ulSkillIdx, unsigned long ulLevel)/*请慎用该函数，因为目前未加任何被动技能降级对数值的影响*/
	{
		if (ulSkillIdx >= MAX_NPCSKILL ||  ulSkillIdx == 0 || ulLevel >= MAX_SKILLLEVEL)
			return 0;
		
		if (m_Skills[ulSkillIdx].SkillId)
		{
			m_Skills[ulSkillIdx].SkillLevel = ulLevel;
			m_Skills[ulSkillIdx].CurrentSkillLevel = ulLevel;
			return 1;
		}	
		return 0;
	}

	int	SetSkillLevelDirectlyUsingId(unsigned long ulSkillId, unsigned long ulLevel)/*请慎用该函数，因为目前未加任何被动技能降级对数值的影响*/
	{
		if (ulSkillId >= MAX_SKILL || ulSkillId == 0 ||  ulLevel >= MAX_SKILLLEVEL)
			return 0;
		unsigned long  ulSkillIdx = FindSame(ulSkillId);
		if (ulSkillIdx)
		{
			if (m_Skills[ulSkillIdx].SkillId)
			{
				m_Skills[ulSkillIdx].SkillLevel = ulLevel;
				m_Skills[ulSkillIdx].CurrentSkillLevel = ulLevel;
				return 1;
			}	
		}
		return 0;
	}

	void		Clear(){memset(m_Skills, 0, sizeof(m_Skills));};
#ifndef _SERVER
	void		SetSkillLevel(int nId, int nLevel);
	BOOL		SetLevel(int nIndex, int nLevel);		
	int			GetSkillSortList(KUiSkillData *);		
	int			GetSkillPosition(int nSkillId);
	int			GetLeftSkillSortList(KUiSkillData*);	
	int			GetRightSkillSortList(KUiSkillData*);
	
	DWORD		GetNextCastTime(int nSkillID);
	DWORD		GetMaxCastTime(int nSkillID);
	
	int			GetSkillInfoList(int nType, KUiSkillItem *pList, int nCount);
	//AutoAI by kinnox;
	int			GetSkillState(int nIndex);
	int			GetSkillFight(int nIndex);
	int			FindSkillAura(int nIndex);
	int			FindSkillRight(int nIndex);
	int			FindSkillLifeReplenish();
	//end code;
#endif
	BOOL		IncreaseLevel(int nIdx, int nLvl, BOOL bAdd = FALSE);
	int			GetLevel(int nSkillID);
	int			GetSkillIdxLevel(int nIdx) const
	{
		if (nIdx <= 0) 
			return 0;
		return m_Skills[nIdx].SkillLevel;
	};
	int			GetCurrentLevel(int nSkillID);
	
	void		ResetSkillPointAdd(int nIdx)
	{
		m_Skills[nIdx].PointAdd = 0;
	}
	int			GetSkillIdxPointAdd(int nIdx) const
	{
		if (nIdx <= 0) 
			return 0;
		return m_Skills[nIdx].PointAdd;
	}
	char		GetPointAdd(int nSkillID);
	
	int			GetCount();
	int			FindSame(int nSkillID);
	int			FindFree();
	BOOL		CanCast(int nSkillID, DWORD dwTime);
	void		SetNextCastTime(int nSkillID, DWORD dwTime);
	
	void		SetMaxCastTime(int nSkillID, DWORD dwTime);
	
	
	int			GetSkillId(int nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].SkillId;
		else 
			return -1;
	};
	
	int		GetEnhanceSkills(int nSkillId);
	
#ifdef _SERVER
	int		UpdateDBSkillList(BYTE *);
	void	SetNpcSkill(int nSkillNo, int nSkillId, int nSkillLevel);//sometime , npc's AI SkillList may have same skills.IT ONLY BE USED FOR NPC(NO PLAYER)
	void	RemoveNpcSkill(int nSkillNo);
	int		ClearSkillLevel(bool bClearPhysicalLevel);
	
	int			GetSkillByIndex(int nIdx)
	{
		if (nIdx <= 0) 
			return 0;
		int nId = 0;
		for (int i = 1; i < MAX_NPCSKILL; i++)
		{
			if (m_Skills[i].SkillId == nIdx)
			{
				nId = i;
				break;
			}
		}	
		return nId;
	}
	void	SetNpcSkill120(int nSkillId, int nSkillLevel);
	void	AddExpSkill120(int nExp);
	void	IncreaseSkill120();
	short	GetSkillId120() const { return m_nSkillId120; };
	BOOL	IsAddExp(int nSkillId);
	
#endif
};
#endif
