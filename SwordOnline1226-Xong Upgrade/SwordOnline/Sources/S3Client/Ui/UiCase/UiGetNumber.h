
#ifndef __UiGetNumber_H__
#define __UiGetNumber_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetNumber : protected KWndImage
{
public:
	static KUiGetNumber*	OpenWindow(const char* pszTitle,
				int nNumber, int nMaxNumber,
				KWndWindow* pRequester, unsigned int uParam);
	static KUiGetNumber*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);
private:
	KUiGetNumber();
	~KUiGetNumber() {}
	void	Initialize();
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
private:
	static KUiGetNumber*	m_pSelf;
	int					m_nMaxNumber;

	KWndText32			m_Title;
	KWndEdit32			m_NumberEdit;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;

private:
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;				;
};


#endif // __UiGetNumber_H__