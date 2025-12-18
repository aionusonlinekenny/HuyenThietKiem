
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../../Login/Login.h"

class KUiTeamBuild : protected KWndImage
{
public:
	static KUiTeamBuild*	OpenWindow();
	static void				CloseWindow(bool bDestroy);

private:
	KUiTeamBuild();
	~KUiTeamBuild() {}
	void	Initialize();							
	void	LoadScheme(const char* pScheme);		
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	
	void	 OnClickConfirmBtn();
	void	 OnClickCancelBtn();
private:
	static 	KUiTeamBuild* m_pSelf;
	KWndImage	m_WarningBg;
	KWndButton m_ConfirmBtn;
	KWndButton m_CancelBtn;
	
	KWndText512	m_TextWarning;
	KWndText256	m_TextTeam;
private:
};
