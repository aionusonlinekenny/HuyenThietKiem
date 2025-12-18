
#pragma once 

#include "../elem/WndShowAnimate.h"
#include "../elem/wndtext.h"
#include "../elem/wndbutton.h"

#ifndef MAX_PLAYER_PER_ACCOUNT
	#define	MAX_PLAYER_PER_ACCOUNT	3		//
#endif

class KUiSelPlayer : protected KWndShowAnimate
{
public:
	static KUiSelPlayer* OpenWindow();					
	static void			 CloseWindow();					
	static void	GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);

private:
	KUiSelPlayer();
	~KUiSelPlayer() {}
	void	Initialize();						
	void	LoadScheme(const char* pScheme);	
	void	PaintWindow();						
	void	UpdateData();						

	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndButton* pWnd, bool bDoubleClick);	
	void	OnNew();
	void	OnDel();
	void	OnCancel();
	void	OnSelectPlayer(int nSel, bool bPlaySound = true);
	void	OnEnterGame();
	int		OnKeyDown(unsigned int uKey);
	KWndButton*	GetActiveBtn();
private:
	static KUiSelPlayer* m_pSelf;
private:
	KWndText80	m_LifeTimeText;
	KWndText32	m_Name[MAX_PLAYER_PER_ACCOUNT];
	KWndText32	m_Level[MAX_PLAYER_PER_ACCOUNT];
	KWndButton	m_player[MAX_PLAYER_PER_ACCOUNT];	
	KWndImage	m_PlayerInfoBg[MAX_PLAYER_PER_ACCOUNT];
	KWndButton	m_btnOk;		
	KWndButton	m_btnCancel;	
	KWndButton	m_btnNew;		
	KWndButton	m_btnDel;

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char			m_szSelPlayerSound[MAX_PLAYER_PER_ACCOUNT][128];

	KWndImage	m_ButterflyWnd;

	char		m_szLoginBg[32];

	char		m_szPlayerImgPrefix[128];

	int			m_ChildPos[4];
	int			m_ChildWndXOffset[4];
	int			m_ChildWndYOffset;

private:
	int			m_bJustClicked;
	short		m_nNumPlayer;
	short		m_nSelPlayer;	

};
