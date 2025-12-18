
#pragma once
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiLogin : protected KWndShowAnimate
{
public:
	static KUiLogin* OpenWindow();				
	static void		 CloseWindow(bool bDestroy);
	static bool		 IsValidPassword(const char* pszPassword, int nLen);
private:
	KUiLogin();
	~KUiLogin();
	void	Show();
	void	Initialize();							
	void	LoadScheme(const char* pScheme);		
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	
	int		GetInputInfo(char* pszAccount, char* pszPassword);
	void	OnLogin();								
	void	OnCancel();								
	void	OnKeyDown(unsigned int uKey);
	void	OnEnableAutoLogin();

	void	OnFocusAccount(bool bEc);
	BOOL	IsAccountNameHaveWord(char* pszWord, char* pszAccountName);
private:
	static KUiLogin* m_pSelf;
private:
	enum UILOGIN_STATUS
	{
		UILOGIN_S_IDLE,
		UILOGIN_S_LOGINING,
	} m_Status;

	KWndEdit32	m_Account;
	KWndEdit32	m_PassWord;
	KWndButton	m_Login, m_Cancel;
	KWndButton	m_RememberAccount;
	KWndButton	m_VirtualKeyboard;
	KWndText256	m_Text;
	char		m_szLoginBg[32];

	bool		m_bAccountFocus;
};
