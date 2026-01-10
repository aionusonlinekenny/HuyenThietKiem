#ifndef __UINEWPLAYER_H__
#define __UINEWPLAYER_H__

#include "../Elem/WndImage.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndShadow.h"
#include "../../Login/Login.h"

class KUiNewPlayer : protected KWndShowAnimate
{
public:
	static KUiNewPlayer* OpenWindow(int nNativePlaceId);
	static void			CloseWindow(bool bDestroy);

private:
	KUiNewPlayer();
	~KUiNewPlayer();
	void	Initialize();						
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndWindow* pWnd);
	int		GetInputInfo();
	void	OnOk();								
	void	OnCancel();
	void	SelGender();
private:
	static KUiNewPlayer* m_pSelf;
	void	UpdateProperty();
	void	Breathe();
private:
	KWndEdit32	m_Name;						
	KWndButton	m_OK;						
	KWndButton  m_Cancel;					
	KWndButton	m_Male, m_Female;			
	
	KWndText256	m_PropertyShow;
	KWndImage	m_Container;
	KWndButton	m_Gold;						
	KWndButton	m_Wood;						
	KWndButton	m_Water;					
	KWndButton	m_Fire;						
	KWndButton	m_Earth;
	
	KRoleChiefInfo		m_Info;
	char				m_szLoginBg[32];
	char				m_szPlayerImgPrefix[128];
	bool				m_bJustClicked;
	struct PROPTYPEINFO
	{
		KWndWindow*			pBtn;
		int					nShowTextLen;
		char				propertyShow[256];
		char				szMaleImg[128];
		char				szFemaleImg[128];
		char				szMaleSound[128];
		char				szFemaleSound[128];
	}						m_propTypeInfoTable[series_num];
};

const char*		PROP_GetTitleString(OBJ_ATTRIBYTE_TYPE eType);

#endif // __UINEWPLAYER_H__