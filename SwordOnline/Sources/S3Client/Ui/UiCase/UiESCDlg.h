
#ifndef __UIESCDLG_H__
#define __UIESCDLG_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiESCDlg : protected KWndShowAnimate
{
public:
	static KUiESCDlg*	OpenWindow();
	static KUiESCDlg*	GetIfVisible();
	static void			CloseWindow(bool bDestroy);
	static void  		LixianOut();
	int 				TimeLixian;
private:
	KUiESCDlg();
	~KUiESCDlg();
	void	Show();
	void	Hide();
	int		Initialize();						
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndWindow* pWnd);
	int		OnKeyDown(unsigned int uKey);
	void	Breathe();//Lixian by kinnox;
	KWndButton*	GetActiveBtn();
private:
	static KUiESCDlg*	m_pSelf;
	KWndButton	m_ExitGameBtn;					
	KWndButton	m_HelpBtn;
	KWndButton	m_OptionsBtn;
	KWndButton	m_OfflineBtn;
	KWndButton	m_ContinueGameBtn;

};

#endif // __UIESCDLG_H__