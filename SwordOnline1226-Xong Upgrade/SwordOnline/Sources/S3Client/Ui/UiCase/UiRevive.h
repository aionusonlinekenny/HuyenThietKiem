
#ifndef __UIREVIVE_H__
#define __UIREVIVE_H__

#include "..\Elem\WndWindow.h"

class KReviveWnd : private KWndWindow
{
public:
	static bool LaunchRevive();
private:
	KReviveWnd()  {}
	~KReviveWnd() {}

	virtual int	    WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	Breathe();													
	void			FirstRevive();
	void			Revive();
	void			Reborn();
	void			TimeoutRevive();
	void			CloseWindow(BOOL bTimeout = false);

private:
	static KReviveWnd*	m_pSelf;
	unsigned int			m_uWaitReviveTime;
	bool					m_bWaitToRevive;
	bool					m_bReborn;
};

#endif // __UIREVIVE_H__