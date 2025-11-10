
#ifndef KTASKFUNCS_H
#define KTASKFUNCS_H

typedef int (* PF_TimerCallBackFun)(void * pOwner, char * szFileName);
#define TASKVALUE_SAVEWAYPOINT_BEGIN		2025
#define	TASKVALUE_MAXWAYPOINT_COUNT			3

#define TASKVALUE_SAVESTATION_BEGIN			2029
#define TASKVALUE_MAXSTATION_COUNT			32

#define TV_TIMERTASK_ID						2047
#define TV_TIMERTASK_RESTTIME				2046
#define TIMERTASK_SETTINGFILE				"\\settings\\timertask.txt"

class KPlayer;
class KNpc;
//=============================
//
//=============================
class KTaskFun
{
public:
	virtual void	SaveTask(int nPlayerIndex) = 0;
	virtual void	LoadTask(int nPlayerIndex) = 0;
};

//=============================
//
//=============================
class KTimerTaskFun :public KTaskFun
{
public:
	KTimerTaskFun();
	
	KTimerTaskFun(void * pOwner)
	{
		m_pTimerOwner = pOwner;
	};

	void SetOwner(void* pOwner) { m_pTimerOwner = pOwner;};
	static BOOL	Init();
	void	SaveTask(int nPlayerIndex);
	void	LoadTask(int nPlayerIndex);
	void *  m_pTimerOwner;
	BOOL	SetTimer(size_t ulTime, unsigned short usTimerTaskId);
	void	CloseTimer();
	DWORD	GetRestTime() const;
	void	GetTimerTaskScript(char * szScriptFileName, unsigned short usTimerTaskId, size_t nScriptFileLen);
	BOOL	Activate(PF_TimerCallBackFun TimerCallBackFun);
	static KTabFile m_TimerTaskTab;
	inline DWORD	GetTaskId()	const { return m_dwTimerTaskId; };
private:
	DWORD	m_dwTimeTaskTime;
	DWORD	m_dwTimerTaskId;
	DWORD	m_dwIntervalTime;	
};

typedef KTaskFun KTask;
extern KTimerTaskFun g_TimerTask;
#endif
