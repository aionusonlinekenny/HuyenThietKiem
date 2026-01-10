#include "KCore.h"

#ifdef _SERVER
#include "KNpc.h"
#include "KPlayerDef.h"
#include "KPlayer.h"
#include "KNpcDeathCalcExp.h"

// --
//
// --
void KNpcDeathCalcExp::Init(int nNpcIdx)
{	
	m_nNpcIdx = (nNpcIdx > 0 ? nNpcIdx : 0);
	memset(m_sCalcInfo, 0, sizeof(KCalcExpInfo) * defMAX_CALC_EXP_NUM);
}

// --
//
// --
void KNpcDeathCalcExp::Active()
{	
	if (Npc[m_nNpcIdx].m_Kind != kind_normal)
		return;

	int i;
	for (i = 0; i < defMAX_CALC_EXP_NUM; i++)
	{
		if (m_sCalcInfo[i].m_nAttackIdx <= 0)
			continue;

		m_sCalcInfo[i].m_nTime--;
		if (m_sCalcInfo[i].m_nTime <= 0)
		{
			m_sCalcInfo[i].m_nTime = 0;
			m_sCalcInfo[i].m_nAttackIdx = 0;
			m_sCalcInfo[i].m_nTotalDamage = 0;
		}
	}
}

// --
//
// --
void	KNpcDeathCalcExp::AddDamage(int nPlayerIdx, int nDamage)
{	
	//
	if (Npc[m_nNpcIdx].m_Kind != kind_normal && 
		Npc[m_nNpcIdx].m_Kind != kind_player)
		return;
	
	
	
	if(Npc[m_nNpcIdx].m_Kind == kind_player)
	{	
		m_sCalcInfo[0].m_nAttackIdx = nPlayerIdx;
		m_sCalcInfo[0].m_nTotalDamage += nDamage;
		m_sCalcInfo[0].m_nTime = defMAX_CALC_EXP_TIME;
		return;
	}
	

	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER || Player[nPlayerIdx].m_nIndex <= 0)
		return;
	
	if (Player[nPlayerIdx].m_cTeam.m_nFlag)
	{
		nPlayerIdx = g_Team[Player[nPlayerIdx].m_cTeam.m_nID].m_nCaptain;
		if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER || Player[nPlayerIdx].m_nIndex <= 0)
			return;
	}

	int		i;
	for (i = 0; i < defMAX_CALC_EXP_NUM; i++)
	{
		if (m_sCalcInfo[i].m_nAttackIdx == nPlayerIdx)
		{
			this->m_sCalcInfo[i].m_nTotalDamage += nDamage;
			this->m_sCalcInfo[i].m_nTime = defMAX_CALC_EXP_TIME;
			return;
		}
	}

	for (i = 0; i < defMAX_CALC_EXP_NUM; i++)
	{
		if (m_sCalcInfo[i].m_nAttackIdx == 0)
			break;
	}

	if (i >= defMAX_CALC_EXP_NUM)
		return;

	m_sCalcInfo[i].m_nAttackIdx = nPlayerIdx;
	m_sCalcInfo[i].m_nTotalDamage = nDamage;
	m_sCalcInfo[i].m_nTime = defMAX_CALC_EXP_TIME;
}

// --
//
// --
int KNpcDeathCalcExp::CalcExp(int nAttacker)//chia exp team by kinnox;
{	
	if(Npc[m_nNpcIdx].m_Kind != kind_normal)
		return 0;

	if (Npc[m_nNpcIdx].m_CurrentLifeMax <= 0)
		return 0;
	int i, j, nDamage = 0, nMaxPlayer = 0;

	for (i = 0; i < defMAX_CALC_EXP_NUM; i++)
	{
		if (m_sCalcInfo[i].m_nAttackIdx <= 0)
			continue;

		if (Player[m_sCalcInfo[i].m_nAttackIdx].m_cTeam.m_nFlag && Player[m_sCalcInfo[i].m_nAttackIdx].m_cTeam.m_nID >= 0)
		{
			int		nTeam, nPlayer, nDistance, nMinDistance, nPlayerP; // chia exp team by kinnox;

			nPlayer = 0;
			nPlayerP = 0;
			nMinDistance = PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE;
			nTeam = Player[m_sCalcInfo[i].m_nAttackIdx].m_cTeam.m_nID;
			nDistance = KNpcSet::GetDistanceSquare(m_nNpcIdx, Player[g_Team[nTeam].m_nCaptain].m_nIndex);

			// chia exp team by kinnox;
			if (nDistance >= 0 && nDistance < nMinDistance)
			{
				nMinDistance = nDistance;
				nPlayerP = g_Team[nTeam].m_nCaptain;
			}

			if (nAttacker == Player[g_Team[nTeam].m_nCaptain].m_nIndex)
			{
				nPlayer = g_Team[nTeam].m_nCaptain;
			}
			//end code;

			for (j = 0; j < MAX_TEAM_MEMBER; j++)
			{
				if (g_Team[nTeam].m_nMember[j] <= 0)
					continue;
				nDistance = KNpcSet::GetDistanceSquare(m_nNpcIdx, Player[g_Team[nTeam].m_nMember[j]].m_nIndex);
				
				//chia exp team by kinnox;
				if (nDistance >= 0 && nDistance < nMinDistance)
				{
					nMinDistance = nDistance;
					nPlayerP = g_Team[nTeam].m_nMember[j];
				}

				if (nAttacker == Player[g_Team[nTeam].m_nMember[j]].m_nIndex)
				{
				    nPlayer = g_Team[nTeam].m_nMember[j];
				}
				//end code;

			}

			if (nPlayer > 0)
			{
				if (m_sCalcInfo[i].m_nTotalDamage > nDamage)
				{
					nDamage = m_sCalcInfo[i].m_nTotalDamage;
					nMaxPlayer = nPlayer;
				}
				//
				float nCalcExp = (float)m_sCalcInfo[i].m_nTotalDamage / (float)Npc[m_nNpcIdx].m_CurrentLifeMax;
				Player[nPlayer].AddExp(nCalcExp, Npc[m_nNpcIdx].m_Level);
				
			}
			else if (nPlayerP > 0)
			{
				nPlayer = nPlayerP;
				if (m_sCalcInfo[i].m_nTotalDamage > nDamage)
				{
					nDamage = m_sCalcInfo[i].m_nTotalDamage;
					nMaxPlayer = nPlayer;
				}
				Player[nPlayer].AddExp(m_sCalcInfo[i].m_nTotalDamage/ Npc[m_nNpcIdx].m_CurrentLifeMax, Npc[m_nNpcIdx].m_Level,TRUE);
			
			}
		}
		else
		{
			int nDistance = KNpcSet::GetDistanceSquare(m_nNpcIdx, Player[m_sCalcInfo[i].m_nAttackIdx].m_nIndex);
			if (nDistance >= PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
				continue;
			if (m_sCalcInfo[i].m_nTotalDamage > nDamage)
			{
				nDamage = m_sCalcInfo[i].m_nTotalDamage;
				nMaxPlayer = m_sCalcInfo[i].m_nAttackIdx;
			}
			//
			float nCalcExp = (float)m_sCalcInfo[i].m_nTotalDamage / (float)Npc[m_nNpcIdx].m_CurrentLifeMax;
			Player[m_sCalcInfo[i].m_nAttackIdx].AddExp(nCalcExp, Npc[m_nNpcIdx].m_Level);
			
		}
	}
	Clear();
	return nMaxPlayer;
}

// --

// --
int KNpcDeathCalcExp::GetDamage() const
{	
	return m_sCalcInfo[0].m_nTotalDamage;
}


// --
//
// --
void KNpcDeathCalcExp::Clear()
{
	memset(m_sCalcInfo, 0, sizeof(KCalcExpInfo) * defMAX_CALC_EXP_NUM);
}

#endif	// _SERVER



