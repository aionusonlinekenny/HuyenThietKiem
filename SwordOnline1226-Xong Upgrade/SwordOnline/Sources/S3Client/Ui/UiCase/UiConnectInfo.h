
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../../Login/Login.h"

enum LOGIN_BG_INFO_MSG_INDEX
{
	CI_MI_CONNECTING = 1,		
	CI_MI_CONNECT_FAILED,		
	CI_MI_CONNECT_SERV_BUSY,	
	CI_MI_CONNECT_TIMEOUT,		
	CI_MI_ACCOUNT_PWD_ERROR,	
	CI_MI_ACCOUNT_LOCKED,		
	CI_MI_ERROR_ROLE_NAME,		
	CI_MI_CREATING_ROLE,		
	CI_MI_DELETING_ROLE,		
	CI_MI_GETTING_ROLE_DATA,	
	CI_MI_ENTERING_GAME,		
	CI_MI_SVRDOWN,				

	CI_MI_TO_DEL_ROLE,	
	
    CI_MI_INVALID_PROTOCOLVERSION,  

	CI_MI_ERROR_LOGIN_INPUT,	
	CI_MI_ERROR_CONFIRM_INPUT,	
	CI_MI_INVALID_LOGIN_INPUT1,	
	CI_MI_INVALID_LOGIN_INPUT2,	
	CI_MI_NOT_ENOUGH_ACCOUNT_POINT,	
	CI_MI_INVALID_PASSWORD,		
	CI_MI_ACCOUNT_FREEZE = 30,
	CI_MI_DEL_ROLE_HAVE_TONG = 31,
	CI_MI_CONNECT_SERV_LOCKED,
	CI_MI_ACCOUNT_EXTPOINT,
	CI_MI_CONNECT_SERV_DENIED,
	CI_MI_CONNECT_SERV_BANED,
};

enum LOGIN_BG_NEXT_STEP
{
	CI_NS_INIT_WND	= 1000,		
	CI_NS_SEL_SERVER,			
	CI_NS_LOGIN_WND,			
	CI_NS_SEL_ROLE_WND,			
	CI_NS_NEW_ROLE_WND,			
	CI_NS_CONFIRM_DEL_ROLE,		
	CI_NS_EXIT_PROGRAM,
};

class KUiConnectInfo : protected KWndImage
{
public:
	static KUiConnectInfo*	OpenWindow(LOGIN_BG_INFO_MSG_INDEX eIndex,
									int eDesireLoginStatus,
									int nParam = 0);
	static void				CloseWindow(bool bDestroy);

private:
	KUiConnectInfo();
	~KUiConnectInfo() {}
	void	Initialize();							
	void	LoadScheme(const char* pScheme);		
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	
	void	PaintWindow();
	void	SetInfoMsg(LOGIN_BG_INFO_MSG_INDEX eIndex);
	void	Breathe();							
	bool	OnDelRole();
	void	OnCancelDelRole();
	void	OnClickConfirmBtn();
	void	GotoNextStep();
private:
	static 	KUiConnectInfo* m_pSelf;
private:
	KWndImage		m_DelRoleBgImg;
	KWndButton		m_ConfirmBtn;
	KWndButton		m_DelRoleBtn;
	KWndButton		m_CancelDelRoleBtn;
	KWndEdit32		m_InputPwdWnd;

	int				m_nDesireLoginStatus;

	char			m_szMessage[64];
	int				m_nMsgLen;
	int				m_nFont;
	int				m_nColor2CharacterStartIndex;
	int				m_nColor2CharacterEndIndex;
	int				m_nTextCentreX, m_nTextCentreY;

	unsigned int	m_uMsgColor;
	unsigned int	m_uMsgBorderColor;
	unsigned int	m_uMsgColor2;
	unsigned int	m_uMsgBorderColor2;
	unsigned int	m_uMsgColorChangeInterval;
	unsigned int	m_uLastMsgColorChanageTime;

	char			m_szLoginBg[32];
	int				m_nParam;
};
