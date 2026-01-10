
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTongSetTax.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTongSetTax.ini"

KUiTongSetTax* KUiTongSetTax::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongSetTax::KUiTongSetTax()
{

}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongSetTax* KUiTongSetTax::OpenWindow(const char* lpszTongName, const char* lpszTongMaster,
										int nPriceParam, int nExchangeTax)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTongSetTax;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		if(lpszTongName)
		{
			m_pSelf->m_TongNameTxt.SetText(lpszTongName);
		}
		if(lpszTongMaster)
		{
			m_pSelf->m_TongMasterTxt.SetText(lpszTongMaster);
		}
		m_pSelf->m_PriceParamEdit.SetIntText(nPriceParam);
		m_pSelf->m_ExchangeTaxEdit.SetIntText(nExchangeTax);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
KUiTongSetTax*	KUiTongSetTax::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::Show()
{
	KWndImage::Show();
	m_PriceParamEdit.Enable(false);
	m_ExchangeTaxEdit.Enable(false);
	Wnd_SetExclusive((KWndWindow*)this);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::Initialize()
{	
	AddChild(&m_TongNameTxt);
	AddChild(&m_TongMasterTxt);

	AddChild(&m_PriceParamEdit);
	AddChild(&m_InPriceParam);
	AddChild(&m_DePriceParam);

	AddChild(&m_ExchangeTaxEdit);
	AddChild(&m_InExchangeTax);
	AddChild(&m_DeExchangeTax);

	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_TongNameTxt.Init(&Ini, "TongNameTxt");
			m_pSelf->m_TongMasterTxt.Init(&Ini, "TongMasterTxt");

			m_pSelf->m_PriceParamEdit.Init(&Ini, "PriceParamEdit");
			m_pSelf->m_InPriceParam.Init(&Ini, "InPriceParam");
			m_pSelf->m_DePriceParam.Init(&Ini, "DePriceParam");

			m_pSelf->m_ExchangeTaxEdit.Init(&Ini, "ExchangeTaxEdit");
			m_pSelf->m_InExchangeTax.Init(&Ini, "InExchangeTax");
			m_pSelf->m_DeExchangeTax.Init(&Ini, "DeExchangeTax");

			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int KUiTongSetTax::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)(KWndWindow*)&m_InPriceParam)
			OnIncrease(0);
		else if(uParam == (unsigned int)(KWndWindow*)&m_DePriceParam)
			OnDecrease(0);
		if(uParam == (unsigned int)(KWndWindow*)&m_InExchangeTax)
			OnIncrease(1);
		else if(uParam == (unsigned int)(KWndWindow*)&m_DeExchangeTax)
			OnDecrease(1);
		else if(uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if(uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::OnIncrease(int nKind)
{
	short nNumber = 0;
	if(nKind == 0)
		nNumber = (short)m_PriceParamEdit.GetIntNumber();
	else
		nNumber = (short)m_ExchangeTaxEdit.GetIntNumber();
	nNumber++;
	if (nNumber < 0)
	{
		nNumber = 0;
	}
	else if (nNumber > defMAX_TAX_VALUE)
	{
		nNumber = defMAX_TAX_VALUE;
	}
	if(nKind == 0)
		m_PriceParamEdit.SetIntText(nNumber);
	else
		m_ExchangeTaxEdit.SetIntText(nNumber);
	
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::OnDecrease(int nKind)
{
	short nNumber = 0;
	if(nKind == 0)
		nNumber = (short)m_PriceParamEdit.GetIntNumber();
	else
		nNumber = (short)m_ExchangeTaxEdit.GetIntNumber();
	nNumber--;
	if (nNumber < 0)
	{
		nNumber = 0;
	}
	else if (nNumber > defMAX_TAX_VALUE)
	{
		nNumber = defMAX_TAX_VALUE;
	}
	if(nKind == 0)
		m_PriceParamEdit.SetIntText(nNumber);
	else
		m_ExchangeTaxEdit.SetIntText(nNumber);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::OnOk()
{	
	short nPriceParam = (short)m_PriceParamEdit.GetIntNumber();
	short nExchangeTax = (short)m_ExchangeTaxEdit.GetIntNumber();
	
	if(nPriceParam >= 0 && nPriceParam <= defMAX_TAX_VALUE)
	{
		if(nExchangeTax >= 0 && nExchangeTax <= defMAX_TAX_VALUE)	
		{
			if(g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_CHANGE_TAX_CHOOSE, (unsigned int)nPriceParam, (int)nExchangeTax);
		}
	}
	CloseWindow(true);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongSetTax::OnCancel()
{	
	if(g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_CHANGE_TAX_CHOOSE, 0, -1);
	CloseWindow(true);
}
