
#include "KCore.h"
#include "KPlayer.h"
#include "KPlayerTask.h"
#include "KTaskFuns.h"
#include "KNpc.h"
#include "KSubWorldSet.h"
#ifndef _SERVER
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KTabFile g_StringResourseTabFile;
char* g_GetStringRes(int nStringID, char * szString, int nMaxLen)
{
	char szStringId[10];
	sprintf(szStringId, "%d", nStringID);
	g_StringResourseTabFile.GetString(szStringId, "STRING", "", szString, nMaxLen);
	return szString;
}
#endif


#ifdef _SERVER
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KTabFile g_MissionTabFile;
int g_WayPointPriceUnit = 1;
int g_StationPriceUnit = 1;
int g_DockPriceUnit	= 1;
int g_GetPriceToWayPoint(int nStationId, int nWayPoint)
{
	if (g_WayPointPriceTabFile.GetWidth() < nStationId)	
	{
		_ASSERT(0);
		return 0;
	}
	if (g_WayPointPriceTabFile.GetHeight() < nWayPoint)
	{
		_ASSERT(0);
		return 0;
	}
	if (*(g_pWayPointPriceTab + (nStationId - 1) * g_WayPointPriceTabFile.GetWidth() + nWayPoint - 1) < 0)
	{
		g_WayPointPriceTabFile.GetInteger(nWayPoint + 1, nStationId + 1, 0, (g_pWayPointPriceTab + (nStationId - 1) * g_WayPointPriceTabFile.GetWidth() + nWayPoint - 1));
	}
	return 	g_WayPointPriceUnit * (*(g_pWayPointPriceTab + (nStationId - 1) * g_WayPointPriceTabFile.GetWidth() + nWayPoint - 1));
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int g_GetPriceToStation(int nStationId, int nNextStationId)
{
	if (nStationId == nNextStationId) return 0;
	if (g_StationPriceTabFile.GetWidth() < nStationId)	
	{
		_ASSERT(0);
		return 0;
	}
	if (g_StationTabFile.GetHeight() < nNextStationId)
	{
		_ASSERT(0);
		return 0;
	}
	if (*(g_pStationPriceTab + (nStationId - 1) * g_StationPriceTabFile.GetWidth() + nNextStationId - 1) < 0)
	{
		int t;
		g_StationPriceTabFile.GetInteger(nNextStationId + 1, nStationId + 1, 0, &t);
		*(g_pStationPriceTab + (nStationId - 1) * g_StationPriceTabFile.GetWidth() + nNextStationId - 1) = t;
	}
	return 	g_StationPriceUnit * (*(g_pStationPriceTab + (nStationId - 1) * g_StationPriceTabFile.GetWidth() + nNextStationId - 1));
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int g_GetPriceToDock(int nCurDockId, int nNextDockId)
{
	if (nCurDockId == nNextDockId)
	{
		return 0;
	}
	
	if (g_DockPriceTabFile.GetWidth() < nCurDockId)	
	{
		_ASSERT(0);
		return 0;
	}

	if (g_DockTabFile.GetHeight()  - 1< nNextDockId)
	{
		_ASSERT(0);
		return 0;
	}
	
	if (*(g_pDockPriceTab + (nCurDockId - 1) * g_DockPriceTabFile.GetWidth() + nNextDockId - 1) < 0)
	{
		int t;
		g_DockPriceTabFile.GetInteger(nNextDockId + 1, nCurDockId + 1, 0, &t);
		*(g_pDockPriceTab + (nCurDockId - 1) * g_DockPriceTabFile.GetWidth() + nNextDockId - 1) = t;
	}
	return 	g_DockPriceUnit * (*(g_pDockPriceTab + (nCurDockId - 1) * g_DockPriceTabFile.GetWidth() + nNextDockId - 1));


}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KTimerTaskFun::KTimerTaskFun()
{
	m_dwTimeTaskTime = 0;
	m_dwTimerTaskId = 0;
	m_dwIntervalTime = 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KTabFile KTimerTaskFun::m_TimerTaskTab;
BOOL	KTimerTaskFun::Init()
{
	m_TimerTaskTab.Clear();
	if (!m_TimerTaskTab.Load(TIMERTASK_SETTINGFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]Can Not Load %s!", TIMERTASK_SETTINGFILE);
#endif
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
BOOL	KTimerTaskFun::Activate(PF_TimerCallBackFun TimerCallBackFun)
{
	if (m_dwTimeTaskTime && m_dwTimeTaskTime <=  g_SubWorldSet.GetGameTime())
	{
		m_dwTimeTaskTime = g_SubWorldSet.GetGameTime() + m_dwIntervalTime; 
		
		if (m_dwTimerTaskId)
		{
			char szTimerScript[MAX_PATH];
			g_TimerTask.GetTimerTaskScript(szTimerScript, m_dwTimerTaskId, MAX_PATH);
			TimerCallBackFun(m_pTimerOwner, szTimerScript);
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KTimerTaskFun::SaveTask(int nPlayerIndex)
{
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return;

	Player[nPlayerIndex].m_cTask.SetSaveVal(TV_TIMERTASK_ID, 0);
	Player[nPlayerIndex].m_cTask.SetSaveVal(TV_TIMERTASK_RESTTIME, 0);
	
	if(!m_dwTimerTaskId)
		return;

	int nRestTime = m_dwTimeTaskTime - g_SubWorldSet.GetGameTime();
	if(nRestTime <= 0)
		return;

	Player[nPlayerIndex].m_cTask.SetSaveVal(TV_TIMERTASK_ID, m_dwTimerTaskId);
	Player[nPlayerIndex].m_cTask.SetSaveVal(TV_TIMERTASK_RESTTIME, nRestTime);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KTimerTaskFun::LoadTask(int nPlayerIndex)
{
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return;
	
	int nTimerTaskId = Player[nPlayerIndex].m_cTask.GetSaveVal(TV_TIMERTASK_ID);
	int nRestTime = Player[nPlayerIndex].m_cTask.GetSaveVal(TV_TIMERTASK_RESTTIME);
	m_dwTimerTaskId = 0;
	m_dwTimeTaskTime = 0;
	
	if(nRestTime <= 0)
		return;

	int nAtTime = nRestTime + g_SubWorldSet.GetGameTime();
	m_dwTimerTaskId = nTimerTaskId;
	m_dwTimeTaskTime = nAtTime;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
BOOL	KTimerTaskFun::SetTimer(size_t ulTime, unsigned short usTimerTaskId)
{
	if (ulTime == 0) 
	{
		CloseTimer();
		return FALSE;
	}

	m_dwIntervalTime = ulTime;
	m_dwTimeTaskTime = g_SubWorldSet.GetGameTime() + ulTime;
	m_dwTimerTaskId = usTimerTaskId;
	return TRUE;
};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KTimerTaskFun::CloseTimer()
{
	m_dwTimeTaskTime = 0;
	m_dwTimerTaskId = 0;
	m_dwIntervalTime = 0;
};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
DWORD	KTimerTaskFun::GetRestTime() const
{
	DWORD dwGameTime = g_SubWorldSet.GetGameTime();
	return (dwGameTime > m_dwTimeTaskTime) ? 0 : (m_dwTimeTaskTime - dwGameTime);
};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KTimerTaskFun::GetTimerTaskScript(char * szScriptFileName, unsigned short usTimerTaskId, size_t nScriptFileLen)
{
	if (!szScriptFileName)
		return ;
	
	szScriptFileName[0] = 0;
	char szTaskId[20];
	sprintf(szTaskId, "%d", usTimerTaskId);
	m_TimerTaskTab.GetString(szTaskId, "SCRIPT", "", szScriptFileName, nScriptFileLen);
}

KTimerTaskFun g_TimerTask;
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int g_NpcTimerCallBackFun(void * pOwner, char * szScriptFileName)
{
	if (!pOwner)
		return 0;
	KNpc *pNpc = (KNpc *)pOwner;
	pNpc->ExecuteTimerScript(szScriptFileName, "OnTimer", pNpc->m_Index);
#ifdef _DEBUG
	if (szScriptFileName)
		printf("Timer Npc %s - Index: %d\n", szScriptFileName, pNpc->m_Index);
#endif
	return 1;
};
#endif 
