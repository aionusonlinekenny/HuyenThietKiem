#ifndef KFACTION_H
#define KFACTION_H

#include "GameDataDef.h"
 
#define		FACTIONS_PRR_SERIES				2
#define		FACTIONS_HUASHAN				10
#define		MAX_FACTION						11

class KFaction
{
public:
	struct	SFactionAttirbute							
	{
		int		m_nIndex;								
		int		m_nSeries;								
		int		m_nCamp;								
		char	m_szName[16];
		char	m_szShowName[32];
	} m_sAttribute[MAX_FACTION];			

public:
	BOOL			Init();								
	int				GetID(int nSeries, int nNo);		
	int				GetID(int nSeries, char *lpszName);	
	int				GetCamp(int nFactionID);			
};

extern	KFaction	g_Faction;
#endif
