// --
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerFaction.cpp
// Date:	2002.09.26
// Code:	±ß³ÇÀË×Ó
// Desc:	PlayerFaction Class
// --

#include	"KCore.h"
#include	"MyAssert.H"
#include	"GameDataDef.h"
#include	"KFaction.h"
#include	"KPlayerFaction.h"

#define		FACTION_NEW			"Míi nhËp giang hå "
#define		FACTION_OLD			"giang hå hiÖp kh¸ch"

// --
//	
// --
KPlayerFaction::KPlayerFaction()
{
	Release();
}

// --
//	
// --
void	KPlayerFaction::Release()
{
	m_nCurFaction = -1;
	m_nFirstAddFaction = -1;
	m_nAddTimes = 0;
}

// --
//	
// --
void	KPlayerFaction::SetSeries(int nSeries)
{
	return;
/*
	if (nSeries < series_metal || nSeries >= series_num)
		return;
	int		nID;
	for (int i = 0; i < FACTIONS_PRR_SERIES; i++)
	{
		nID = g_Faction.GetID(nSeries, i);
		_ASSERT(nID >= 0);
		if (m_sSkillOpen[i].m_nID != nID)
		{
			m_sSkillOpen[i].m_nID = nID;
			m_sSkillOpen[i].m_nOpenLevel = 0;
		}
	}
*/
}

// --
//	
// --
BOOL	KPlayerFaction::AddFaction(int nSeries, char *lpszFactionName)
{
	int		nID;

	nID = g_Faction.GetID(nSeries, lpszFactionName);
	if (nID < 0)
		return FALSE;
#ifdef _DEBUG
	//g_DebugLog("[AddFaction] Series: %d - Faction: %s - ID: %d", nSeries, lpszFactionName, nID);
#endif
	return AddFaction(nSeries, nID);
}

// --
//	
// --
BOOL	KPlayerFaction::AddFaction(int nSeries, int nFactionID)
{
	if (nSeries < series_metal || nSeries >= series_num)
		return FALSE;
	//
	if (nFactionID < nSeries * FACTIONS_PRR_SERIES)
		return FALSE;

	if(nSeries != series_water)
	{
		if(nFactionID >= (nSeries + 1) * FACTIONS_PRR_SERIES)
			return FALSE;
	}
	else
	{
		if(nFactionID >= MAX_FACTION)
			return FALSE;
	}
	
	SetSeries(nSeries);
	m_nCurFaction = nFactionID;
	m_nAddTimes++;
	if (m_nAddTimes == 1)
		m_nFirstAddFaction = nFactionID;
	
#ifdef _DEBUG
	//g_DebugLog("[AddFaction] Series: %d - FactionID: %d", nSeries, nFactionID);
#endif

	return TRUE;
}

// --
//	
// --
void	KPlayerFaction::LeaveFaction()
{
	m_nCurFaction = -1;
}

// --
//	
// --
BOOL	KPlayerFaction::OpenCurSkillLevel(int nLevel, KSkillList *pSkillList)
{
	return TRUE;
/*
	if (nLevel < 0 || nLevel >= FACTION_SKILL_LEVEL || !pSkillList)
		return FALSE;
	if (m_nCurFaction < 0)
		return FALSE;

	int		i, j;

	// Éè¶¨µ±Ç°¿ª·ÅµÈ¼¶
	for (i = 0; i < FACTIONS_PRR_SERIES; i++)
	{
		if (m_nCurFaction == m_sSkillOpen[i].m_nID)
		{
			m_sSkillOpen[i].m_nOpenLevel = nLevel;
			break;
		}
	}

	// Éè¶¨Íæ¼Ò¼¼ÄÜ
	for (i = 0; i <= nLevel; i++)
	{
		for (j = 0; j < FACTION_SKILLS_PER_LEVEL; j++)
		{
			if (g_Faction.m_sAttribute[m_nCurFaction].m_nSkill[i][j] <= 0)
				continue;
			pSkillList->Add(g_Faction.m_sAttribute[m_nCurFaction].m_nSkill[i][j], 0);
		}
	}

	return TRUE;
*/
}

// --
//	
// --
int		KPlayerFaction::GetGurFactionCamp()
{
	if (m_nCurFaction < 0)
	{
		if (m_nAddTimes == 0)
			return camp_begin;
		else
			return camp_free;
	}
	else
	{
		if (g_Faction.GetCamp(m_nCurFaction) >= 0)
			return g_Faction.GetCamp(m_nCurFaction);
		else
			return camp_begin;
	}
}

// --
//	¹¦ÄÜ£º»ñµÃµ±Ç°ÃÅÅÉ
// --
int		KPlayerFaction::GetCurFactionNo()
{
	return m_nCurFaction;
}

// --
//	¹¦ÄÜ£º»ñµÃµ±Ç°ÃÅÅÉ
// --
void	KPlayerFaction::GetCurFactionName(char *lpszGetName)
{
	if (!lpszGetName)
		return;

	if (this->m_nCurFaction == -1)
	{
		if (this->m_nAddTimes == 0)
		{
			lpszGetName[0] = 0;
		}
		else
		{
			strcpy(lpszGetName, FACTION_NEW);
		}
	}
	else
	{
		strcpy(lpszGetName, g_Faction.m_sAttribute[m_nCurFaction].m_szName);
	}

	return;
}



