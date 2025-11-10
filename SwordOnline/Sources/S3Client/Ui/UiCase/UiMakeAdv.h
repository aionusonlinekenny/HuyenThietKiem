
#ifndef __UiMakeAdv_H__
#define __UiMakeAdv_H__

#include "../Elem/WndButton.h"
#include "../elem/wndlabeledbutton.h"
#include "../Elem/WndEdit.h"


class KUiMakeAdv : protected KWndImage
{
public:
	static KUiMakeAdv*	OpenWindow(const char* pszInitString = NULL, int nMinLen = 4, int nMaxLen = 32);
	static KUiMakeAdv*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);
private:
	KUiMakeAdv();
	~KUiMakeAdv() {}
	void	Initialize();							
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
private:
	static KUiMakeAdv*	m_pSelf;
	short				m_nMinLen, m_nMaxLen;
	KWndEdit32			m_AdvEditor;
	KWndLabeledButton	m_OkBtn;
	KWndLabeledButton	m_CancelBtn;
	KWndText32			m_Title;
};


#endif // __UiMakeAdv_H__