#ifndef __UiMsgSel2_H__
#define __UiMsgSel2_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;

class KUiMsgSel2 : protected KWndShowAnimate
{
public:
	static KUiMsgSel2*	OpenWindow(KUiQuestionAndAnswer* pContent);
	static KUiMsgSel2*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	
	static void			CloseWindow(bool bDestroy);		
private:
	KUiMsgSel2() {}
	~KUiMsgSel2() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();							
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();
	void 	InitImage(const char* pszKey);

private:
	static KUiMsgSel2*	m_pSelf;
	KWndImage			m_Image;
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_MsgScroll;
	KScrollMessageListBox	m_MsgScrollList;

	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;

	short				m_nImgNumFrames;
};


#endif // __UiMsgSel2_H__