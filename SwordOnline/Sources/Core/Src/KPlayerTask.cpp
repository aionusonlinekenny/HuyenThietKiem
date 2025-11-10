
#include	"KCore.h"
#include	"KPlayerTask.h"

#ifdef _SERVER
int			g_TaskGlobalValue[TASKGLOBALVALUENUM];
KTabFile	g_WayPointTabFile;
KTabFile	g_StationTabFile;
KTabFile	g_DockTabFile;
KTabFile	g_StationPriceTabFile;
KTabFile	g_WayPointPriceTabFile;
KTabFile	g_DockPriceTabFile;

int	*		g_pStationPriceTab = NULL;
int	*		g_pWayPointPriceTab = NULL;
int	*		g_pDockPriceTab = NULL;
#endif

// --
//	
// --
KPlayerTask::KPlayerTask()
{
	Release();
}

// --
//	
// --
void	KPlayerTask::Release()
{
	memset(nSave, 0, sizeof(nSave));
	memset(nClear, 0, sizeof(nClear));
}

// --
//	
// --
void	KPlayerTask::ClearTempVar()
{
	memset(nClear, 0, sizeof(nClear));
}

// --
//	
// --
int		KPlayerTask::GetSaveVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return 0;

	return nSave[nNo];
}

// --
//	
// --
void	KPlayerTask::SetSaveVal(int nNo, BOOL bFlag)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return;

#ifdef _DEBUG
//	//g_DebugLog("[SetTask] TaskId: %d - TaskValue: %d", nNo, bFlag);
#endif

	nSave[nNo] = bFlag;
}

// --
//	
// --
int		KPlayerTask::GetClearVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return 0;
	return nClear[nNo];
}

// --
//	
// --
void	KPlayerTask::SetClearVal(int nNo, int nVal)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return;

#ifdef _DEBUG
	//g_DebugLog("[SetTaskTemp] TaskTempId: %d - TaskTempValue: %d", nNo, nVal);
#endif

	nClear[nNo] = nVal;
}
