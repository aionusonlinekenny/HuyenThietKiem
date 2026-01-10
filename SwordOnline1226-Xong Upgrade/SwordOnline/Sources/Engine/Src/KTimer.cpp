// --
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTimer.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Timer Class
// --
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KTimer.h"
#ifdef __linux
#include <sys/time.h>
#endif

// --
//
// --
KTimer::KTimer()
{
#ifdef WIN32
	m_nFrequency.QuadPart = 200 * 1024 * 1024;
	m_nTimeStart.QuadPart = 0;
	m_nTimeStop.QuadPart = 0;
	m_nFPS = 0;
	QueryPerformanceFrequency(&m_nFrequency);
#endif
}

// --
//
// --
void KTimer::Start()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_nTimeStart);
#else
    gettimeofday(&m_nTimeStart, NULL);
#endif
}

// --
// 
// --
void KTimer::Stop()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_nTimeStop);
#else
	gettimeofday(&m_nTimeStop, NULL);
#endif
}

// --
//
// --
DWORD KTimer::GetElapse()
{
#ifdef WIN32
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	return (DWORD)((nTime.QuadPart - m_nTimeStart.QuadPart) 
		* 1000 / m_nFrequency.QuadPart);
#else
	timeval tv;
	gettimeofday(&tv, NULL);
    return (tv.tv_sec - m_nTimeStart.tv_sec) * 1000 + tv.tv_usec / 1000;
#endif
}

// --
//
// --
DWORD KTimer::GetElapseFrequency()
{
#ifdef WIN32
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	return (DWORD)(nTime.QuadPart - m_nTimeStart.QuadPart);
#endif
	return 0;
}

// --
//
// --
DWORD KTimer::GetInterval()
{
#ifdef WIN32
	return (DWORD)((m_nTimeStop.QuadPart - m_nTimeStart.QuadPart) 
		* 1000 / m_nFrequency.QuadPart);
#endif
	return 0;
}

// --
// 
// --
BOOL KTimer::Passed(int nTime)
{

	if (GetElapse() >= (DWORD)nTime)
	{
		Start();
		return TRUE;
	}
	return FALSE;
}

// --
//
// --
BOOL KTimer::GetFPS(int *nFPS)
{
	if (GetElapse() >= 1000)
	{
		*nFPS = m_nFPS;
		m_nFPS = 0;
		Start();
		return TRUE; 
	}
	m_nFPS++;
	return FALSE;
}
