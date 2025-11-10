#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "../Elem/MouseHover.h"
#include "UiChatItemInfo.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Engine/src/Text.h"
#include "../../../core/src/coreshell.h"
#include <crtdbg.h>

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;
#define		SCHEME_INI_SHOWITEM			"UiChatItemInfo.ini"

KUiChatItemInfo*			KUiChatItemInfo::m_pSelf = NULL;

// --
//
// --
KUiChatItemInfo* KUiChatItemInfo::OpenWindow(const char* pItem, int nLen, const char* plszIconImage, int nIconWith, int nIconHeight)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiChatItemInfo;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	
	if (m_pSelf)
	{
		memcpy(m_pSelf->pItemDesc, pItem, nLen);
		m_pSelf->pItemLen = nLen;
		m_pSelf->m_nIconWidth = nIconWith;
		m_pSelf->m_nIconHeight = nIconHeight; 
		m_pSelf->UpdateData(plszIconImage);
		m_pSelf->Show();
		m_pSelf->BringToTop();
	}
	return m_pSelf;
}

// --
//
// --
void KUiChatItemInfo::CloseWindow()
{	
	if (m_pSelf)
	{	
		m_pSelf->Hide();
	}
}

// --
//
// --
KUiChatItemInfo::KUiChatItemInfo()
{	
	m_nBgColor		= 0;
	m_nBgAlpha		= 0;
	pItemLen		= 0;
	pItemDesc[0]	= 0;
	nNumLine		= 0;
	nMaxLineLen		= 0;
	memset(&m_Image, 0, sizeof(KRUImage));
	m_nImgWidth		= 0;
	m_nImgHeight	= 0;
	m_nIconWidth	= 0;	        
	m_nIconHeight	= 0;	 
	memset(&Rect, 0, sizeof(KRURect));
 
}

// --
//
// --
KUiChatItemInfo* KUiChatItemInfo::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	else
		return NULL;
}

// --
//
// --
void KUiChatItemInfo::UpdateData(const char* lpszIconImage)
{	
	if(lpszIconImage[0])
	{
		m_IconImage.SetImage(ISI_T_SPR, lpszIconImage);
		if(m_nIconWidth == 5)
		{
			m_IconImage.SetPosition(-90, -100);
		}
		else
		{
			m_IconImage.SetPosition(0, 0);
		}
		m_IconImage.SetFrame(0);
		m_IconImage.Show();
	}
	else
	{
		m_IconImage.Hide();
	}
}

// --
//
// --
void KUiChatItemInfo::Initialize()
{	
	AddChild(&m_IconImage);

	char szScheme[128];
	g_UiBase.GetCurSchemePath(szScheme, sizeof(szScheme));
	LoadScheme(szScheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}

// --
//
// --
int KUiChatItemInfo::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nResult = false;
	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
			CloseWindow();
			nResult = true;
			break;
		default:
			nResult = KWndWindow::WndProc(uMsg, uParam, nParam);
			break;
	}
	return nResult;
}

// --
//
// --
void KUiChatItemInfo::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHOWITEM);

	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		Ini.GetInteger("Main", "ImgWidth",    0, (int*)&m_nImgWidth);
		Ini.GetInteger("Main", "ImgHeight",   0, (int*)&m_nImgHeight);
		if(m_nImgWidth < 0)
			m_nImgWidth = 0;
		if(m_nImgHeight < 0)
			m_nImgWidth = 0;
		
		m_Image.nType = ISI_T_SPR;
		m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		m_Image.Color.Color_b.a = 255;
		m_Image.uImage = 0;
		m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
		Ini.GetString("Main", "Image", "" , m_Image.szImage, sizeof(m_Image.szImage));
		m_Image.nFrame = 0;

		Ini.GetString("Main", "BgColor", "0,0,0", Buff, 128);
		m_nBgColor = (GetColor(Buff) & 0x00ffffff);
		Ini.GetString("Main", "PicColor", "0,0,0", Buff, 128);
		m_nPicColor = (GetColor(Buff) & 0x00ffffff);
		Ini.GetInteger("Main", "BgAlpha", 0, (int*)&m_nBgAlpha);
		m_nBgColor |= (m_nBgAlpha << 24);
		Ini.GetString("Main", "RectColor", "0,0,0", Buff, 128);
		m_nRectColor = (GetColor(Buff) & 0x00ffffff);

		m_IconImage.Hide();
	}
	return;
}

// --
//
// --
void KUiChatItemInfo::Breathe()
{	
	if(m_nIconWidth == 5)
		m_IconImage.NextFrame();
}

// --
//
// --
void KUiChatItemInfo::PaintWindow() 
{	
	KWndWindow::PaintWindow();
	
	KRUShadow	Shadow;
	KOutputTextParam	Param;
	Param.BorderColor = 0xff000000;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;
	
	Shadow.Color.Color_dw = m_nBgColor;
	Shadow.oPosition.nX = m_nAbsoluteLeft;
	Shadow.oPosition.nY = m_nAbsoluteTop;
	Shadow.oEndPos.nX = m_nAbsoluteLeft + m_Width + m_nIconWidth * 24;
	Shadow.oEndPos.nY = m_nAbsoluteTop + m_Height;
	if(m_nIconWidth == 5)
		Shadow.oEndPos.nY += m_nIconHeight * 4;
	Shadow.oEndPos.nZ = Shadow.oPosition.nZ = 0;

	Param.nSkipLine = 0;
	Param.nNumLine = 1;
	Param.Color = 0xffffffff;
	Param.nY = Shadow.oPosition.nY;
	
	if (pItemLen > 0)
	{
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
		int m_nFontSize = 12;
		int nLineLen;
		while(true)
		{
			if (TGetEncodedTextLineCount(pItemDesc, pItemLen , 0, nLineLen, m_nFontSize, Param.nSkipLine, 1) == 0)
				break;
			Param.nX = m_nAbsoluteLeft + m_Width / 2 - (nLineLen * 12) / 4 + m_nIconWidth * 24;
			g_pRepresentShell->OutputRichText(m_nFontSize, &Param, pItemDesc, pItemLen , 0);
			Param.nSkipLine ++;
			Param.nY += m_nFontSize + 1;
		};
		
		m_Image.oPosition.nX = m_nAbsoluteLeft;
		
		while(m_Image.oPosition.nX < m_nAbsoluteLeft + m_Width + m_nIconWidth * 24)
		{			
			m_Image.oPosition.nY = m_nAbsoluteTop - m_nFontSize * 3 / 4;
			g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
			m_Image.oPosition.nY = Shadow.oEndPos.nY;
			g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
			m_Image.oPosition.nX += m_nImgWidth;
		};
		
		if(m_nIconWidth == 5)
			return;

		Shadow.Color.Color_dw = m_nPicColor;
		Shadow.oPosition.nX = m_nAbsoluteLeft;
		Shadow.oPosition.nY = m_nAbsoluteTop;
		Shadow.oEndPos.nX = Shadow.oPosition.nX + m_nIconWidth * 25;
		Shadow.oEndPos.nY = Shadow.oPosition.nY + m_nIconHeight * 25;
		Shadow.oEndPos.nZ = Shadow.oPosition.nZ = 0;
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
		
		Rect.Color.Color_dw = m_nRectColor;
		Rect.oPosition.nX = m_nAbsoluteLeft;
		Rect.oPosition.nY = m_nAbsoluteTop;
		Rect.oEndPos.nX = Rect.oPosition.nX + m_nIconWidth * 25;
		Rect.oEndPos.nY = Rect.oPosition.nY + m_nIconHeight * 25;
		Rect.oEndPos.nZ = Rect.oPosition.nZ = 0;
		g_pRepresentShell->DrawPrimitives(1, &Rect, RU_T_RECT, true);

	}
}

// --
//
// --
void KUiChatItemInfo::Show()
{	
	nNumLine = TGetEncodedTextLineCount(pItemDesc, pItemLen, 0, (int&)nMaxLineLen, 12);
	m_Width = 12 * nMaxLineLen / 2 + 20;
	m_Height = (12 + 1) * nNumLine + 20;

	if(m_Width % m_nImgWidth != 0) 
		m_Width += m_nImgWidth - m_Width % m_nImgWidth;
	KWndWindow::Show();
}

// --
//
// --
void KUiChatItemInfo::Hide()
{ 
 nNumLine = 0;
 nMaxLineLen = 0;

 KWndWindow::Hide();
}

