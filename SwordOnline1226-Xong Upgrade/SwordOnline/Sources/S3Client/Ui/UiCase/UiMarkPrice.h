
#ifndef __UiMarkPrice_H__
#define __UiMarkPrice_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndEdit.h"
#include "../../../core/src/gamedatadef.h"

class KUiMarkPrice : protected KWndImage
{
public:
	static KUiMarkPrice*	OpenWindow(KUiObjAtContRegion* pObj, KUiItemBuySelInfo* pPriceInfo);	
	static KUiMarkPrice*	GetIfVisible();				
	static void			CloseWindow(bool bDestroy);		
private:
	KUiMarkPrice();
	~KUiMarkPrice() {}
	int		Initialize();								
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnMarkPrice();
	void	OnCheckInput();
private:
	static KUiMarkPrice*	m_pSelf;
	KWndText32			m_ObjectName;
	KWndEdit32			m_PriceInput;
	KWndText32			m_Title;
	KWndText32			m_Name;
	KWndLabeledButton	m_OkBtn;
	KWndLabeledButton	m_CancelBtn;
	KUiObjAtContRegion	m_ItemInfo;
};


#endif // __UiMarkPrice_H__