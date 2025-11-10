
#ifndef KPLAYERTASK_H
#define KPLAYERTASK_H

#define		MAX_TASK		2048
#define		MAX_TEMP_TASK	2560

#ifdef _SERVER
#define TASKGLOBALVALUENUM 5000
extern int		g_TaskGlobalValue[TASKGLOBALVALUENUM];

extern KTabFile g_WayPointTabFile;
extern KTabFile g_StationTabFile;
extern KTabFile g_DockTabFile;

extern KTabFile g_StationPriceTabFile;
extern KTabFile g_WayPointPriceTabFile;
extern KTabFile g_DockPriceTabFile;

extern int	*g_pStationPriceTab;
extern int  *g_pWayPointPriceTab;
extern int  *g_pDockPriceTab;

#endif

class KPlayerTask
{
public:
	int			nSave[MAX_TASK];					
	int			nClear[MAX_TEMP_TASK];				
public:
	KPlayerTask();									
	void		Release();							
	void		ClearTempVar();						
	int			GetSaveVal(int nNo);				
	void		SetSaveVal(int nNo, BOOL bFlag);	
	int			GetClearVal(int nNo);
	void		SetClearVal(int nNo, int nVal);
};
#endif
