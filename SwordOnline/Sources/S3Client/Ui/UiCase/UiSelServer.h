
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndShowAnimate.h"

class KUiSelServer : protected KWndShowAnimate
{
public:
	static KUiSelServer* OpenWindow();				
	static void			 CloseWindow(bool bDestroy);
private:
	KUiSelServer();
	~KUiSelServer();
	void	Initialize();						
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnSelRegion(int nSel);
	void	OnLogin();
	void	OnCancel();
	int		OnKeyDown(unsigned int uKey);
	void	OnClickButton(KWndButton* pBtn);
	void	GetList();

private:
	static KUiSelServer* m_pSelf;
private:
	struct KLoginServer* m_pRegionList;
	struct KLoginServer* m_pServList;

	KWndList		m_RegionList;
	KWndScrollBar	m_ScrollBar;
	KWndList		m_ServerList;
	KWndText32		m_NameBigger;
	KWndButton		m_Login;
	KWndButton		m_Cancel;
	char			m_szLoginBg[32];

	bool			m_bSelRegion;
	int				m_nRegionIndex;
};
