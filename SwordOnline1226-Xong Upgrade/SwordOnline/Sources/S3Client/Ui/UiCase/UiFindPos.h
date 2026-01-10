
#ifndef __UiFindPos_H__
#define __UiFindPos_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../elem/wndlabeledbutton.h"


class KUiFindPos : protected KWndImage
{
public:
	static KUiFindPos*	OpenWindow();	
	static KUiFindPos*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	
	static void			CloseWindow();		
private:
	KUiFindPos() {}
	~KUiFindPos() {}
	int		Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
	void	UpdateData();
private:
	static KUiFindPos*	m_pSelf;
	KWndText32			m_Title;
	KWndText32			m_Symbols;
	KWndEdit32			m_PosX;
	KWndEdit32			m_PosY;
	KWndLabeledButton	m_OkBtn;
	KWndLabeledButton	m_CancelBtn;
};


#endif 