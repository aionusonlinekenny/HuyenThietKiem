#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"

class KUiNewPlayerStartMsg : protected KWndShowAnimate
{
public:
	static KUiNewPlayerStartMsg* OpenWindow();		
	static void	CloseWindow(bool bDestroy);         

private:
	KUiNewPlayerStartMsg();
	~KUiNewPlayerStartMsg();
	void	Initialize();						
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnOk();
	int		OnKeyDown(unsigned int uKey);
	void	OnClickButton(KWndButton* pBtn);
private:
	void	LoadList();
	static  KUiNewPlayerStartMsg* m_pSelf;
private:
	KScrollMessageListBox	m_List;
	KScrollMessageListBox	m_AdvanceList;
	KWndButton				m_NewerBtn;
	KWndButton				m_AdvanceBtn;
	KWndButton		        m_Ok;

};
