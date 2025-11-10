
#ifndef KTimer_H
#define KTimer_H
#ifdef WIN32
class ENGINE_API KTimer
#else
class KTimer
#endif
{
private:
#ifdef WIN32
	LARGE_INTEGER	m_nFrequency;
	LARGE_INTEGER	m_nTimeStart;
	LARGE_INTEGER	m_nTimeStop;
#else
	timeval m_nTimeStart;
	timeval m_nTimeStop;
#endif
	int				m_nFPS;
public:
	KTimer();
	void			Start();
	void			Stop();
	DWORD			GetElapse();
	DWORD			GetElapseFrequency();
	DWORD			GetInterval();
	BOOL			Passed(int nTime);
	BOOL			GetFPS(int* nFPS);
};
#endif
