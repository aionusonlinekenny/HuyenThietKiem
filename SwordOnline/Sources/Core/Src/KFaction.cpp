#include	"KCore.h"
#include	"KIniFile.h"
#include	"KSkills.h"
#include	"KFaction.h"
#include	"CoreUseNameDef.h"
 
KFaction	g_Faction;

// --
//
// --
BOOL	KFaction::Init()
{
	KIniFile	Ini;
	char		szSection[80], szBuffer[32];
	char		szSeries[series_num][16] = {"S_GOLD", "S_WOOD", "S_WATER", "S_FIRE", "S_EARTH"};
	char		szCamp[camp_num][16] = {"S¬ nhËp", "C_JUSTICE", "C_EVIL", "C_BALANCE", "S¸t thñ", "Qu¸i thó", "Sù kiÖn"};
	int			i, j, k, nArrayPos;

	for (i = 0; i < MAX_FACTION; i++)
	{
		m_sAttribute[i].m_nIndex = i;
		m_sAttribute[i].m_nSeries = series_metal;
		m_sAttribute[i].m_nCamp = camp_justice;
		m_sAttribute[i].m_szName[0] = 0;
	}

	if ( !Ini.Load(FACTION_FILE) )
		return FALSE;

	for (i = 0; i < MAX_FACTION; i++)
	{
		sprintf(szSection, "%d", i);
		Ini.GetString(szSection, "Series", "S_GOLD", szBuffer, sizeof(szBuffer));
		for (j = 0; j < series_num; j++)
		{
			if (strcmp(szBuffer, szSeries[j]) != 0)
				continue;
			
			if(i == FACTIONS_HUASHAN)
			{
				if (m_sAttribute[i].m_szName[0] == 0)
				{
					nArrayPos = i;
					m_sAttribute[nArrayPos].m_nSeries = j;
				}
			}
			else
			{
				for (k = 0; k < FACTIONS_PRR_SERIES; k++)
				{
					if (m_sAttribute[j * FACTIONS_PRR_SERIES + k].m_szName[0] == 0)
					{
						nArrayPos = j * FACTIONS_PRR_SERIES + k;
						m_sAttribute[nArrayPos].m_nSeries = j;
						break;
					}
				}
			}
			break;
		}
		_ASSERT(j < series_num);
		Ini.GetString(szSection, "Name", "shaolin", m_sAttribute[nArrayPos].m_szName, sizeof(m_sAttribute[nArrayPos].m_szName));
		Ini.GetString(szSection, "ShowName", "ThiÕu L©m ph¸i", m_sAttribute[nArrayPos].m_szShowName, sizeof(m_sAttribute[nArrayPos].m_szShowName));
		Ini.GetString(szSection, "Camp", "C_JUSTICE", szBuffer, sizeof(szBuffer));
		for (j = 0; j < camp_num; j++)
		{
			if (strcmp(szBuffer, szCamp[j]) == 0)
			{
				m_sAttribute[nArrayPos].m_nCamp = j;
				break;
			}
		}
		_ASSERT(j < camp_num);
	}

	return TRUE;
}

// --
//	
// --
int		KFaction::GetID(int nSeries, int nNo)
{
	if (nSeries < series_metal || nSeries >= series_num || nNo < 0 || nNo >= FACTIONS_PRR_SERIES)
		return -1;

	return nSeries * FACTIONS_PRR_SERIES + nNo;
}

// --
//	
// --
int		KFaction::GetID(int nSeries, char *lpszName)
{
	if (nSeries < series_metal || nSeries >= series_num)
		return -1;
	if ( !lpszName || !lpszName[0])
		return -1;
	int nEnd = (nSeries + 1) * FACTIONS_PRR_SERIES;
	if(nSeries == series_water)
	{
		nEnd = FACTIONS_HUASHAN + 1;
	}
	for (int i = nSeries * FACTIONS_PRR_SERIES; i < nEnd; i++)
	{
		if (strcmp(lpszName, m_sAttribute[i].m_szName) == 0)
			return i;
	}
	return -1;
}

// --
//
// --
int		KFaction::GetCamp(int nFactionID)
{
	if (nFactionID < 0 || nFactionID >= MAX_FACTION)
		return -1;
	return m_sAttribute[nFactionID].m_nCamp;
}
