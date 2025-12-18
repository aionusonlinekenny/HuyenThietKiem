
#ifndef __UiTongSetTax_H__
#define __UiTongSetTax_H__
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../../Core/Src/GameDataDef.h"


class KUiTongSetTax : protected KWndImage
{
public:
	static KUiTongSetTax*	OpenWindow(const char* lpszTongName, const char* lpszTongMaster,
										int nPriceParam, int nExchangeTax);
	static KUiTongSetTax*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);

private:
	KUiTongSetTax();
	~KUiTongSetTax() {}
	void	Initialize();
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnIncrease(int nKind);
	void	OnDecrease(int nKind);
	void	OnCancel();
	void	OnOk();
private:
	static KUiTongSetTax*	m_pSelf;

private:
	KWndText32			m_TongNameTxt, m_TongMasterTxt;
	KWndEdit32			m_PriceParamEdit, m_ExchangeTaxEdit;
	KWndButton			m_InPriceParam, m_DePriceParam;
	KWndButton			m_InExchangeTax, m_DeExchangeTax;
	KWndButton			m_OkBtn, m_CancelBtn;
};


#endif // __UiTongSetTax_H__