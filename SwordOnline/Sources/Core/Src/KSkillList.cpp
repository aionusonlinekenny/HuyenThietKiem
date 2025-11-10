 #include "KEngine.h"
#include "KCore.h"
#include "SkillDef.h"
#include "KSkillList.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#include "crtdbg.h"
#else
#include "S3DBInterface.h"
#endif
#include "KSkills.h"
#include "KPlayer.h"

#ifndef _SERVER
#include "cOREsHELL.H"
#endif

KSkillList::KSkillList()
{
	ZeroMemory(m_Skills, sizeof(NPCSKILL) * MAX_NPCSKILL);
	m_nNpcIndex = 0;

#ifdef _SERVER
	m_nSkillId120	= 0;
	m_nLevelTask120 = 0;
	m_nNextExp120	= 0;
#endif

}

KSkillList::~KSkillList()
{
}


static int g_CallCount = 0;

int	KSkillList::FindSame(int nSkillID)
{
    g_CallCount++;

	if (!nSkillID)
		return 0;
	
	for (int i=1; i<MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId == nSkillID)
		{
			return i;
		}
	}
	return 0;	
}

int KSkillList::FindFree()
{
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (!m_Skills[i].SkillId)
		{
			return i;
		}
	}
	return 0;	
}

#ifdef _SERVER
void KSkillList::SetNpcSkill(int nSkillNo, int nSkillId, int nSkillLevel)
{
	if (nSkillNo <= 0 || nSkillLevel <= 0 || nSkillId <= 0)
		return ;
	if (nSkillNo >= MAX_NPCSKILL) 
		return ;
	
	m_Skills[nSkillNo].SkillId = nSkillId;
	m_Skills[nSkillNo].CurrentSkillLevel = nSkillLevel;
	m_Skills[nSkillNo].SkillLevel = nSkillLevel;

	_ASSERT(nSkillId > 0 && nSkillLevel > 0);
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nSkillLevel);
	if (!pOrdinSkill) 
        return ;
	
    if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}
}

void KSkillList::RemoveNpcSkill(int nSkillNo)
{
	if (nSkillNo <= 0 )
		return ;
	if (nSkillNo >= MAX_NPCSKILL) 
		return ;
	
	m_Skills[nSkillNo].SkillId = 0;
	m_Skills[nSkillNo].CurrentSkillLevel = 0;
	m_Skills[nSkillNo].SkillLevel = 0;
}

#endif

int KSkillList::GetCount()
{
	int nCount = 0;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			nCount++;
		}
	}
	return nCount;
}

#ifndef _SERVER
void KSkillList::SetSkillLevel(int nId/*技能的ID号*/, int nLevel)
{
	int i = FindSame(nId);
	
	if (!i)
	{
		Add(nId, nLevel);
		return;		
	}
	else
		//m_Skills[i].SkillLevel = nLevel;
		SetLevel(i, nLevel);
}
#endif

#ifndef _SERVER
BOOL KSkillList::SetLevel(int nIndex, int nLevel)
{
	if (nIndex <= 0 || nIndex >= MAX_NPCSKILL)
		return FALSE;

	if (nLevel < 0 || nLevel >= MAX_SKILLLEVEL)
		return FALSE;
	
	int nInc = nLevel - m_Skills[nIndex].SkillLevel;
	IncreaseLevel(nIndex, nInc);
	return TRUE;
}
#endif


//	

BOOL KSkillList::IncreaseLevel(int nIdx, int nLvl, BOOL bAdd /*= FALSE*/)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;
	
			
	if(bAdd)
	{	
		m_Skills[nIdx].PointAdd += nLvl;
	}
	
	else
	{
		m_Skills[nIdx].SkillLevel += nLvl;
		m_Skills[nIdx].CurrentSkillLevel += nLvl;
	}
	
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[nIdx].SkillId, m_Skills[nIdx].CurrentSkillLevel + m_Skills[nIdx].PointAdd);		//
	if (!pOrdinSkill) 
        return FALSE;

	if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{	
		if(nLvl < 0)
		{
			pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex, 0, SKILL_SLT_Npc, TRUE);
		}
		
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}
	return TRUE;
}




int KSkillList::Add(int nSkillID, int nLevel, bool bExpand /*= false*/)
{

	int i;
	
	if (nSkillID <= 0 || nLevel < 0)
		return 0;
	i = FindSame(nSkillID);
	
	if (i)
	{	
		//g_DebugLog("Find Same");
		if (nLevel > m_Skills[i].SkillLevel)
		{	
			int nInc = nLevel - m_Skills[i].SkillLevel;
			IncreaseLevel(i, nInc);
		}
			
#ifdef _SERVER
		this->SetNpcSkill120(nSkillID, nLevel);
#endif	
		
		if (nLevel  > 0)
		{
			ISkill * pSkill = g_SkillManager.GetSkill(nSkillID, m_Skills[i].CurrentSkillLevel);
			if (!pSkill) 
				return 0;
			
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill *)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}

		return i;
	}

	i = FindFree();
	if (i)
	{
		//g_DebugLog("Find Free");

		m_Skills[i].SkillId				= nSkillID;
		m_Skills[i].SkillLevel			= nLevel;
		m_Skills[i].CurrentSkillLevel	= nLevel;
			
		m_Skills[i].SkillExpand			= bExpand;
		//g_DebugLog("Increa 120 Same");
#ifdef _SERVER
		this->SetNpcSkill120(nSkillID, nLevel);
#endif	
		//g_DebugLog("Eend");
		
		if (nLevel > 0)
		{
			ISkill * pSkill =  g_SkillManager.GetSkill(nSkillID, nLevel);
			if (!pSkill) 
				return 0 ;
			
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}
		
		return i;
	}
	
	return 0;
}

//
//
//
void KSkillList::Remove(int nSkillID)
{
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL) 
		return ;
	int i = FindSame(nSkillID);
	if (i)
	{
		ISkill * pSkill =  g_SkillManager.GetSkill(nSkillID, 1);
		if (!pSkill) 
			return;

		if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
		{
			((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex, 0, SKILL_SLT_Npc, TRUE);
		}

		m_Skills[i].CurrentSkillLevel = 0;
		m_Skills[i].SkillId = 0;
		m_Skills[i].SkillLevel = 0;
	} 
};

int KSkillList::GetLevel(int nSkillID)
{
	int i;
	
	if (!nSkillID)
		return 0;
	
	i = FindSame(nSkillID);
	if (i)
	{
		_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		return m_Skills[i].SkillLevel;
	}

	return 0;
}


//	

int KSkillList::GetCurrentLevel(int nSkillID)
{
	int i;
	
	if (!nSkillID)
		return 0;
	
	i = FindSame(nSkillID);

	if (i)
	{
		return m_Skills[i].CurrentSkillLevel;
	}

	return 0;
}


//	

char KSkillList::GetPointAdd(int nSkillID)
{
	int i;
	
	if (!nSkillID)
		return 0;
	
	i = FindSame(nSkillID);

	if (i)
	{
		return m_Skills[i].PointAdd;
	}

	return 0;
}


//	

BOOL KSkillList::CanCast(int nSkillID, DWORD dwTime)
{
	
	if (!nSkillID)
		return FALSE;
	
	int i = FindSame(nSkillID);
	if (!i)
	{
		return FALSE;
	}

	if (m_Skills[i].CurrentSkillLevel <= 0 || m_Skills[i].NextCastTime > dwTime)
		return FALSE;
	
	return TRUE;
}

// --
//
// --
void KSkillList::SetNextCastTime(int nSkillID, DWORD dwTime)
{
	if (!nSkillID)
		return;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return;
	
	m_Skills[i].NextCastTime = dwTime;
}

// --

// --
void KSkillList::SetMaxCastTime(int nSkillID, DWORD dwTime)
{
	if (!nSkillID)
		return;;
	
	int i = FindSame(nSkillID);
	if (!i)
		return;
	
	m_Skills[i].MaxCastTimes = dwTime;
}

// --

// --
int KSkillList::GetEnhanceSkills(int nSkillId)
{
	int nPercent = 0;
	if (!nSkillId)
		return nPercent;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		KSkill * pSkill = (KSkill *) g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
		if (pSkill) 
		{
			int x = pSkill->GetEnhanceSkillsNum();
			KMagicAttrib* pMagicData = NULL;
			pMagicData = pSkill->GetEnhanceSkills();	
			for (int j =0 ; j < x ; j ++)
			{
				if (!pMagicData[j].nAttribType) continue;
				if ( pMagicData[j].nValue[0] == nSkillId)
				{
					nPercent += pMagicData[j].nValue[2];
				}
			}
		}
	}
	return nPercent;
}
	
#ifndef _SERVER

//	

int	KSkillList::GetSkillSortList(KUiSkillData * pSkillList)
{
	if (!pSkillList) 
		return 0;

	memset(pSkillList, 0, sizeof(KUiSkillData) * 50);

	int nCount = 0;

	ISkill* pSkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			_ASSERT(m_Skills[i].SkillLevel >= 0);
			
			if(m_Skills[i].SkillExpand)
				continue;
			
			if (!m_Skills[i].SkillLevel)
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			}
			else 
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			}
			
			if (!pSkill)
				continue;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
			
			switch(eStyle)
			{
				case SKILL_SS_Missles:
				case SKILL_SS_Melee:
				case SKILL_SS_InitiativeNpcState:
				case SKILL_SS_PassivityNpcState:
					{	
						//
						pOrdinSkill = (KSkill * ) pSkill;
						if (pOrdinSkill->GetSkillAttrib() < 100)
							continue;
						

					}break;
				case SKILL_SS_Thief:					
					{
						
					}
					break;
				default:
					continue;
			}
			
            KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_FIGHT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nLevel = m_Skills[i].SkillLevel;
			
			pSkill->cPointAdd = m_Skills[i].PointAdd;
			
			if ((++nCount) == 50)
			{		
				break;
			}
		}
	}
	return nCount;
}

int KSkillList::GetLeftSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);
	int nCount = 1;
	
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();
	pSkillList->nData = 0;
	
	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			if (!pISkill)
				continue;
			
			eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();
			switch(eStyle)
			{
			case SKILL_SS_Missles:					
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	
			case SKILL_SS_PassivityNpcState:
				{	
					//
					pOrdinSkill = (KSkill * ) pISkill;
					if ((pOrdinSkill->GetSkillAttrib() >= 100) &&	
						(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
						(pOrdinSkill->GetSkillLRInfo() == leftOnlySkill)
						)
					{
						
					}
					else 
					{
						continue;
					}
					
					
				}
				break;
				
			case SKILL_SS_Thief:
				{
					
					continue;
					
				}break;
			}
			
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 5;
			nCount ++;
			if (nCount >= 65)
				break;
			
		}
	}
	return nCount;
}

int KSkillList::GetRightSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);
	
	int nCount = 1;
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();	 
	pSkillList->nData = 0;
	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill  =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			if(!pISkill)
				continue;

			eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();
			
			switch(eStyle)
			{
			case SKILL_SS_Missles:
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:
			case SKILL_SS_PassivityNpcState:
				{	
					//
					pOrdinSkill = (KSkill * ) pISkill;
					if (
						(pOrdinSkill->GetSkillAttrib() >= 100) && 
						(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
						(pOrdinSkill->GetSkillLRInfo() == RightOnlySkill)
						)
					{
						
					}
					else 
					{
						continue;
					}
					
				}
				break;
				
			case SKILL_SS_Thief:
				{
					
				}break;
			}
			
			
			
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 5;
			nCount ++;
			if (nCount >= 65)
				break;
			
		}
	}
	return nCount;
}




int KSkillList::GetSkillPosition(int nSkillId)//获得技能在技能界面的位置
{
	if (nSkillId <= 0) return -1;
	KSkill * pOrdinSkill = NULL;
	
	ISkill * pISkill = NULL;
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		_ASSERT(m_Skills[i].SkillLevel >= 0);
		if (m_Skills[i].SkillId<= 0) continue;
		
		if (m_Skills[i].SkillLevel == 0)
		{
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
		}
		else
		{
			pISkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
		}

		if (!pISkill) 
        	continue ;


		eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Missles:			//	子弹类		本技能用于发送子弹类
		case SKILL_SS_Melee:
		case SKILL_SS_InitiativeNpcState:	//	主动类		本技能用于改变当前Npc的主动状态
		case SKILL_SS_PassivityNpcState:		//	被动类		本技能用于改变Npc的被动状态
			{	
				//
				pOrdinSkill = (KSkill * ) pISkill;
				if (
					(!m_Skills[i].SkillId) ||
					(pOrdinSkill->GetSkillAttrib() < 100)
					)
				{
					continue;
				}
				
			}
			break;
			
		case SKILL_SS_Thief:
			{
				
				
			}break;
		}
		
		
		if (m_Skills[i].SkillId == nSkillId)
		{
			return nCount;
		}
		nCount ++;
		
	}
	return nCount;
}
#endif

#ifndef _SERVER
// --

// --
DWORD KSkillList::GetNextCastTime(int nSkillID)
{
	 if (!nSkillID)
	  return 0;
 
	 int i = FindSame(nSkillID);
	 if (!i)
	  return 0;
 
	 return m_Skills[i].NextCastTime;
}

// --

// --
DWORD KSkillList::GetMaxCastTime(int nSkillID)
{
	 if (!nSkillID)
	  return 0;
 
	 int i = FindSame(nSkillID);
	 if (!i)
	  return 0;
 
	 return m_Skills[i].MaxCastTimes;
}
#endif

#ifdef _SERVER
int	KSkillList::UpdateDBSkillList(BYTE * pSkillBuffer)
{
	if (!pSkillBuffer)
		return -1;
	
	int nCount = 0;

	TDBSkillData * pSkillData = (TDBSkillData *) pSkillBuffer;
	for (int i = 0 ; i < MAX_NPCSKILL; i ++)
	{	
		if (m_Skills[i].SkillId > 0)
		{	
			
			if(m_Skills[i].SkillExpand)
				continue;
			

			pSkillData->m_nSkillId = m_Skills[i].SkillId;
			pSkillData->m_nSkillLevel = m_Skills[i].SkillLevel;
			pSkillData++;
			nCount ++;
		}
	}
	return nCount;
}

int		KSkillList::ClearSkillLevel(bool bClearPhysicalLevel)
{
	int nTotalSkillPoint = 0;

	for (int i = 0 ; i < MAX_NPCSKILL; i ++)
	{
		if (m_Skills[i].SkillId > 0)
		{
			ISkill * pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			if (pSkill)
			{	
				if (pSkill->GetSkillAttrib() < 100)
					continue;
				if( pSkill->IsExpSkill() )
					continue;

				nTotalSkillPoint				+= m_Skills[i].SkillLevel;
				m_Skills[i].SkillLevel			= 0;
				m_Skills[i].CurrentSkillLevel	= 0;
				
				m_Skills[i].PointAdd			= 0;
				
			}
		}
	}
	return nTotalSkillPoint;
}
#endif

#ifdef _SERVER
//// --
//
//// --
void KSkillList::SetNpcSkill120(int nSkillID, int nSkillLevel)
{
	if (nSkillLevel <= 0 || nSkillID <=0 )
		return;
	
	if(nSkillID < MIN_ID_SKILL120 ||  nSkillID > MAX_ID_SKILL120)
		return;

	m_nSkillId120 = nSkillID;
	IncreaseSkill120();
}
#endif

#ifdef _SERVER
//// --
// 
//// --
void KSkillList::AddExpSkill120(int nExp)
{	
	if (m_nSkillId120 <= 0)
		return;

	if (nExp <= 0)
		return;
	
	if (m_nLevelTask120 <= 0 || m_nNextExp120 <= 0)
		return;
	

	if (Npc[m_nNpcIndex].IsPlayer())
	{	
		int nMaxLevel = MAX_LEVEL_SKILL_EXP + Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskGetSaveVal(TV_TRANSLIFE);
		if(this->GetLevel(m_nSkillId120) < nMaxLevel)
		{	
			if (Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskGetSaveVal(TV_ENHANCE_SKILL) > 0) 
			{
				nExp += nExp;
			}

			nExp += Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskGetSaveVal(m_nLevelTask120);
			
			
			Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskSetSaveVal(m_nLevelTask120, nExp);

			if (Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskGetSaveVal(m_nLevelTask120) >= m_nNextExp120)
			{
				Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskSetSaveVal(m_nLevelTask120, 0);
				IncreaseLevel(GetSkillByIndex(m_nSkillId120),1);

				PLAYER_SKILL_LEVEL_SYNC NewSkill;
				NewSkill.ProtocolType	= s2c_playerskilllevel;
				NewSkill.m_nSkillID		= m_nSkillId120;
				NewSkill.m_nSkillLevel	= GetLevel(m_nSkillId120);
				NewSkill.m_nLeavePoint	= Player[Npc[m_nNpcIndex].GetPlayerIdx()].m_nSkillPoint;
				NewSkill.m_btPointAdd	= this->GetPointAdd(m_nSkillId120);
				NewSkill.m_bExpand		= false;
				g_pServer->PackDataToClient(Player[Npc[m_nNpcIndex].GetPlayerIdx()].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
				
				IncreaseSkill120();
			}
		}
	}
}
#endif

#ifdef _SERVER
//// --
// Tang level skill 120
//// --
void KSkillList::IncreaseSkill120()
{
	if (m_nSkillId120 <=0)
		return;

	BYTE btLevel = GetLevel(m_nSkillId120);
	
	if (btLevel <= 0)
	{
		m_nLevelTask120 = 0;
		m_nNextExp120 = 0;
		return;
	}
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nSkillId120, btLevel);

	KMagicAttrib *DamageAttribs = pOrdinSkill->GetDamageAttribs();
	
	for (int i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType) continue;

		if ((DamageAttribs + i)->nAttribType == magic_skill_skillexp_v)
		{
			m_nLevelTask120 = (DamageAttribs + i)->nValue[2];
			m_nNextExp120 = (DamageAttribs + i)->nValue[0];
			break;
		}
	}
	
}
#endif

#ifdef _SERVER
//// --
// 
//// --
BOOL KSkillList::IsAddExp(int nSkillId)
{
	if (nSkillId <= 0)
		return FALSE;
	
	int nMaxLevel = MAX_LEVEL_SKILL_EXP + Player[Npc[m_nNpcIndex].GetPlayerIdx()].TaskGetSaveVal(TV_TRANSLIFE);
	if(GetLevel(nSkillId) <= 0 || GetLevel(nSkillId) >= nMaxLevel)
		return FALSE;

	return TRUE;
}
#endif

#ifndef _SERVER
int KSkillList::GetSkillInfoList(int nType, KUiSkillItem *pList, int nCount)
{
	int nNum = 0, nPointAdd = 0, nCurLevel = 0;
	
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{	
		if (nNum == 19)
			break;

		if (m_Skills[i].SkillId)
		{
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId) + nPointAdd; 
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId,nCurLevel);
				if (!pOrdinSkill) 
					continue;

				switch(nType)
				{
					case 0:
						{
							if (!pOrdinSkill->IsAura()) 
								continue;

							strcpy(pList[nNum].Name, pOrdinSkill->GetSkillName());
							pList[nNum].Index = m_Skills[i].SkillId;
							nNum++;
						}
						break;
					case 1:
						{
							if (pOrdinSkill->IsAura()) 
								continue;
							if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState)  && pOrdinSkill->IsTargetSelf())
							{
								strcpy(pList[nNum].Name, pOrdinSkill->GetSkillName());
								pList[nNum].Index = m_Skills[i].SkillId;
								nNum++;
							}
						}
						break;
					case 2:
						{
							eSkillStyle eStyle = (eSkillStyle)pOrdinSkill->GetSkillStyle();
							switch(eStyle)
							{
							case SKILL_SS_Missles:
							case SKILL_SS_Melee:
							case SKILL_SS_InitiativeNpcState:
							case SKILL_SS_PassivityNpcState:
								{
									pOrdinSkill = (KSkill * ) pOrdinSkill;
									if (
										(pOrdinSkill->GetSkillAttrib() >= 100) && 
										(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
										(pOrdinSkill->GetSkillLRInfo() == RightOnlySkill)
										)
									{
										
									}
									else 
									{
										continue;
									}
								}
								break;
								
							case SKILL_SS_Thief:
								{
									
								}break;
							}

							strcpy(pList[nNum].Name, pOrdinSkill->GetSkillName());
							pList[nNum].Index = m_Skills[i].SkillId;
							nNum++;
						}
						break;
					case 3:
						{
							eSkillStyle eStyle = (eSkillStyle)pOrdinSkill->GetSkillStyle();
							switch(eStyle)
							{
								case SKILL_SS_Missles:					
								case SKILL_SS_Melee:
								case SKILL_SS_InitiativeNpcState:	
								case SKILL_SS_PassivityNpcState:
								{	
									//
									pOrdinSkill = (KSkill * ) pOrdinSkill;
									if ((pOrdinSkill->GetSkillAttrib() >= 100) &&	
										(pOrdinSkill->GetSkillLRInfo() == BothSkill) || 
										(pOrdinSkill->GetSkillLRInfo() == leftOnlySkill)
										)
									{
										
									} 
									else 
									{
										continue;
									}
									
									
								}
								break;
								
							case SKILL_SS_Thief:
								{
									
									continue;	
								}
								break;
							}
							strcpy(pList[nNum].Name, pOrdinSkill->GetSkillName());
							pList[nNum].Index = m_Skills[i].SkillId;
							nNum++;
						}
						break;	
					default:
						break;
				}
			}
		}
	}

	return nNum;
}
#endif

//AutoAI by kinnox;
#ifndef _SERVER
int KSkillList::GetSkillState(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
		
					if(pOrdinSkill->IsAura() || pOrdinSkill->IsTargetEnemy())
						continue;

					if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf())
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if(pOrdinSkill->IsAura())
					continue;

				if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
					pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf())
					nCount++;
			}
			else
				continue;
		}
	}
	return nCount;
}

int KSkillList::GetSkillFight(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
						
					if(pOrdinSkill->IsAura())
						continue;

					if (pOrdinSkill->IsBuffEffect())
						continue;
					
					if (pOrdinSkill->GetSkillStyle() == 3)
						continue;
				
						j++;
						if (j == nIndex)
						return m_Skills[i].SkillId;
					
									
				}
				else
					continue;
			}
		}
		return 0;
	}
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
				
				if(pOrdinSkill->IsAura())
					continue;
				if (pOrdinSkill->IsBuffEffect())
					continue;
				if (pOrdinSkill->GetSkillStyle() == 3)
					continue;
					nCount++;
					
			}
			else
				continue;
		}
	}
	return nCount;
}

int KSkillList::FindSkillAura(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
		
					if(pOrdinSkill->IsAura())
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int _nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if(pOrdinSkill->IsAura())
					_nCount++;
			}
			else
				continue;
		}
	}
	return _nCount;
}

int KSkillList::FindSkillRight(int nIndex)
{
	if (nIndex > 0)
	{
		int i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			int nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{	
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if(nCurLevel > 0)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill) 
						continue;
					
					if(pOrdinSkill->IsAura())
						continue;

					if (pOrdinSkill->IsBuffEffect())
						continue;
					
					if (pOrdinSkill->GetSkillStyle() == 3)
						continue;
					if (pOrdinSkill->GetSkillLRInfo() == 2)
					{
						j++;
						if (j == nIndex)
						return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	int _nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if(pOrdinSkill->IsAura())
					continue;
				if (pOrdinSkill->IsBuffEffect())
					continue;
				if (pOrdinSkill->GetSkillStyle() == 3)
					continue;
				if (pOrdinSkill->GetSkillLRInfo() == 2)
				{
					_nCount++;
				}
			}
			else
				continue;
		}
	}
	return _nCount;
}

int KSkillList::FindSkillLifeReplenish()
{
	int i = 0, j = 0;
	for (i = 1; i < MAX_NPCSKILL; i++)
	{
		int nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{	
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if(nCurLevel > 0)
			{
				KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill) 
					continue;
	
				if (pOrdinSkill->IsSkillLifeReplenish()) 
					return m_Skills[i].SkillId;
			}
			else
				continue;
		}
	}
	return 0;
}

#endif
//end code;