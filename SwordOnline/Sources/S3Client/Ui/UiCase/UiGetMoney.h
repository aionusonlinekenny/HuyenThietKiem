
#ifndef __UiGetMoney_H__
#define __UiGetMoney_H__

#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetMoney : protected KWndImage
{
public:
	static KUiGetMoney*	OpenWindow(int nMoney, int nMaxMoney, KWndWindow* pRequester,
							 unsigned int uParam, KWndWindow* pMoneyWnd);
	static KUiGetMoney*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);
private:
	KUiGetMoney();
	~KUiGetMoney() {}
	int		Initialize();								
	void	Show(KWndWindow* pMoneyWnd);
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
private:
	static KUiGetMoney*	m_pSelf;
	KWndEdit32			m_Money;
	KWndLabeledButton			m_OkBtn;
	KWndLabeledButton			m_CancelBtn;
	KWndText32					m_Text;
	int					m_nMaxMoney;
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;

};


#endif // __UiGetMoney_H__