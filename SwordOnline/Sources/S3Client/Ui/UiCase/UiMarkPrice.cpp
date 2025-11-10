
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiMarkPrice.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiMarkPrice.ini"

KUiMarkPrice* KUiMarkPrice::m_pSelf = NULL;

KUiMarkPrice* KUiMarkPrice::OpenWindow(KUiObjAtContRegion* pObj, KUiItemBuySelInfo* pPriceInfo)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMarkPrice;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_ItemInfo		= *pObj;
		m_pSelf->m_ObjectName.SetText(pPriceInfo->szItemName);
		m_pSelf->m_PriceInput.SetIntText(0);
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


KUiMarkPrice::KUiMarkPrice()
{
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

KUiMarkPrice* KUiMarkPrice::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiMarkPrice::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}


void KUiMarkPrice::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_PriceInput);
	Wnd_SetExclusive((KWndWindow*)this);
}

void KUiMarkPrice::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

int KUiMarkPrice::Initialize()
{
	AddChild(&m_ObjectName);
	AddChild(&m_PriceInput);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_Title);
	AddChild(&m_Name);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

void KUiMarkPrice::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ObjectName.Init(&Ini, "ObjectName");
		m_PriceInput.Init(&Ini, "PriceInput");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_Title.Init(&Ini, "Title");
		m_Name.Init(&Ini, "Name");
	}
}

int KUiMarkPrice::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			OnMarkPrice();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
		{
			OnCancel();
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{	
			OnMarkPrice();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiMarkPrice::OnMarkPrice()
{	
	if(g_pCoreShell == NULL)
		return;

	OnCheckInput();
	int nPrice = m_PriceInput.GetIntNumber();

	if(g_pCoreShell->OperationRequest(GOI_MARK_PRICE_ITEM,
				(unsigned int)(&m_ItemInfo), nPrice))
	CloseWindow(false);
}

void KUiMarkPrice::OnCheckInput()
{
	int nPrice = m_PriceInput.GetIntNumber();
	if (nPrice < 0)
	{
		nPrice = 0;
	}
	char	szBuff1[16], szBuff2[16];
	_itoa(nPrice, szBuff1, 10);
	m_PriceInput.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_PriceInput.SetIntText(nPrice);	
}

void KUiMarkPrice::OnCancel()
{	
	if(g_pCoreShell->OperationRequest(GOI_MARK_PRICE_ITEM,
				(unsigned int)(&m_ItemInfo), 0))
	CloseWindow(false);
}
