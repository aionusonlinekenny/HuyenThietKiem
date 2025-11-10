#ifndef KMISSIONARRAY_H
#define KMISSIONARRAY_H

#include "KTaskFuns.h"
#include "KMission.h"

#define MAX_MISSIONARRAY_VALUE_COUNT 100

template <class T, size_t ulSize>
class KMissionArray : public KLinkArrayTemplate<T,  ulSize>
{
	int	m_MissionValue[MAX_MISSIONARRAY_VALUE_COUNT];
	char m_MissionString[MAX_MISSIONARRAY_VALUE_COUNT][MAX_PATH];
public:
	BOOL	Activate()
	{
		unsigned long ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;
		
		int nIdx = 0;		
		for (int i = 0; i < ulActiveCount ; i ++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].Activate();
		}
		return TRUE;
	};
	
	void SetMissionValue(unsigned long ulValueId, int nValue)
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return ;
		m_MissionValue[ulValueId] = nValue;
	};
	
	int GetMissionValue(unsigned long ulValueId) const
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return 0;
		return m_MissionValue[ulValueId];
	};
	void SetMissionString(unsigned long ulValueId, const char*  lpString)
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return ;

		g_StrCpy(m_MissionString[ulValueId], lpString);
	};
	
	char* GetMissionString(unsigned long ulValueId)
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return NULL;
		return m_MissionString[ulValueId];	
	};
	unsigned long FindSame(T *pMission)
	{
		int nIdx = 0;
		while(1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;
			
			if (pMission->GetMissionId() == m_Data[nIdx].GetMissionId())
				return nIdx;
		}
		return 0;
	};
	BOOL RemovePlayer(unsigned long ulPlayerIndex)
	{
		unsigned long ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;
		
		int nIdx = 0;		
		for (int i = 0; i < ulActiveCount ; i ++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].RemovePlayer(ulPlayerIndex);
		}
		return TRUE;
	}
	
};


//管理定时触发器的集合
template <class T, size_t ulSize, PF_TimerCallBackFun CallFun>
class KTimerFunArray :public KLinkArrayTemplate < T, ulSize>
{
public:
	char m_szScriptFile[MAX_PATH];
	BOOL	Activate()
	{
		unsigned long ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;
		
		int nIdx = 0;		
		for (int i = 0; i < ulActiveCount ; i ++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
		//	_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].Activate(CallFun);
		}
		return TRUE;
	};

	unsigned long FindSame(T *pTask)
	{
		int nIdx = 0;
		while(1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (pTask->GetTaskId() == m_Data[nIdx].GetTaskId())
				return nIdx;
		}
		return 0;
	};

};
#endif
