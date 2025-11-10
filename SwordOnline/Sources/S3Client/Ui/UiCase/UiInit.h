
#ifndef __UIINIT_H__
#define __UIINIT_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiInit : protected KWndShowAnimate
{
public:
	static KUiInit*		OpenWindow(bool bStartMusic = true, bool bJustLaunched = false);
	static void			CloseWindow();
	static void			PlayTitleMusic();
	static void			StopTitleMusic();

private:
	KUiInit() {}
	~KUiInit() {}
    void    ShowCompleted();
	void	Initialize();					
	void	LoadScheme(const char* pScheme);
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndButton* pWnd);
	KWndButton*	GetActiveBtn();
//	void	OnAutoLogin();
	int		OnKeyDown(unsigned int uKey);
	void	PlayStartMovie();
private:
	static KUiInit* m_pSelf;
private:
	char		m_szLoginBg[32];
	KWndImage  m_EnterGameBorder;				
	KWndImage  m_OpenRepBorder;				
	KWndImage  m_GameConfigBorder;				
	KWndImage  m_ExitGameBorder;
	KWndButton m_EnterGame;					
	KWndButton m_GameConfig;				
	KWndButton m_OpenRep;				
	KWndButton m_ExitGame;

	KWndImage	m_CopyRight;

	int			m_nCurrentMovieIndex;
};

#endif // __UIINIT_H__