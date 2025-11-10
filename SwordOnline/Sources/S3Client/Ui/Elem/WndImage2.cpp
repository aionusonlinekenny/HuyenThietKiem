#include "KWin32.h"
#include "KIniFile.h"
#include "WndImage2.h"
#include "MouseHover.h"
#include "../../../Engine/src/Text.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

KWndImage2::KWndImage2()
{
	IR_InitUiImageRef(m_Image);
	m_szTip[0] = 0;
	m_nTipLen = 0;
	memset(m_nData, 0, sizeof(m_nData));
}

void KWndImage2::Clone(KWndImage2* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_Image		= m_Image;
		strcpy(pCopy->m_szTip, m_szTip);
		pCopy->m_nTipLen	= m_nTipLen;
		memcpy(pCopy->m_nData, m_nData, sizeof(m_nData));
	}
}

int KWndImage2::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		int		nValue = 0;
		pIniFile->GetInteger(pSection, "Trans", 0, &nValue);
		if (nValue)
			m_Style |= WNDIMG_ES_EXCLUDE_TRANS;
		else
			m_Style &= ~WNDIMG_ES_EXCLUDE_TRANS;
		m_Image.nFlipTime = 0;
		m_Image.nInterval = 0;
		m_Image.nNumFrames = 0;
		pIniFile->GetInteger(pSection, "ImgType", 0, &nValue);
		if (nValue == 1)
		{
			m_Image.nType = ISI_T_BITMAP16;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
			m_Style &= ~WNDIMG_ES_EXCLUDE_TRANS;
		}
		else
		{
			m_Image.nType = ISI_T_SPR;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_Image.Color.Color_b.a = 255;
		}
		m_Image.uImage = 0;
		m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
		pIniFile->GetString(pSection, "Image", "" , m_Image.szImage, sizeof(m_Image.szImage));
		if (pIniFile->GetInteger(pSection, "Frame", -1, &nValue) && nValue != -1)
			m_Image.nFrame = nValue;

		if ((m_Width == 0 || m_Height == 0) && g_pRepresentShell)
		{
			KImageParam	Param = { 0 };
			if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, m_Image.nType))
			{
				if (m_Width == 0)
					m_Width = Param.nWidth;
				if (m_Height == 0)
					m_Height = Param.nHeight;
			}
		}
		return true;
	}
	return false;
}

int KWndImage2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_M_MOUSE_ENTER:
	case WM_MOUSEMOVE:
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_M_MOUSE_ENTER, (unsigned int)(KWndWindow*)this, 0);
		break;
	case WM_MOUSEHOVER:
		if (m_szTip[0] && !g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
		{
			g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
				LOWORD(nParam), HIWORD(nParam), false, true);
			g_MouseOver.SetMouseHoverTitle(m_szTip, m_nTipLen, 0xffffffff);
		}
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

int KWndImage2::PtInWindow(int x, int y)
{
	int	bIn = KWndWindow::PtInWindow(x, y);
	
	if ((bIn &&
			(m_Style & WND_S_SIZE_WITH_ALL_CHILD) == 0) || 
		((m_Style & WND_S_VISIBLE) &&
			bIn == false &&
			(m_Style & WND_S_SIZE_WITH_ALL_CHILD)))
	{
		if ((m_Style & WNDIMG_ES_EXCLUDE_TRANS) && g_pRepresentShell)
				bIn =  g_pRepresentShell->GetImagePixelAlpha(m_Image.szImage, m_Image.nFrame, x - m_nAbsoluteLeft, y - m_nAbsoluteTop, m_Image.nType);
	}
	return bIn;
}

void KWndImage2::SetFrame(int nFrame)
{
	m_Image.nFrame = nFrame;
}


void KWndImage2::SetImage(short nType, const char* pszImgName, bool bAdjustWndSize)
{
	m_Image.nType = nType;
	strncpy(m_Image.szImage, pszImgName, sizeof(m_Image.szImage));
	m_Image.szImage[sizeof(m_Image.szImage) - 1] = 0;
	m_Image.nNumFrames = 0;
	m_Image.uImage = 0;
	m_Image.nFlipTime = IR_GetCurrentTime();
	if (bAdjustWndSize && g_pRepresentShell)
	{
		KImageParam	Param;
		if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, nType))
			SetSize(Param.nWidth, Param.nHeight);
	}
}

void KWndImage2::UpdateTimer()
{
	m_Image.nFlipTime = IR_GetCurrentTime();
}

int KWndImage2::NextFrame()
{
	return IR_NextFrame(m_Image);
}

void KWndImage2::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell)
	{
		m_Image.oPosition.nX = m_nAbsoluteLeft;
		m_Image.oPosition.nY = m_nAbsoluteTop;
		g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
	}
}

int KWndImage2::GetCurFrame()
{
	return IR_GetCurFrame(m_Image);
}

int KWndImage2::GetNumFrames()
{
	return IR_GetNumFrames(m_Image);
}

// --
//
// --
int KWndImage2::SetToolTipInfo(char* szTip, int nMax)
{
	m_nTipLen = min(nMax, sizeof(m_szTip) - 1);
	memcpy(m_szTip, szTip, m_nTipLen);
	m_szTip[m_nTipLen] = 0;
	m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
	return m_nTipLen;
}

void KWndImage2::SetData(int nData, int nNo)
{
	m_nData[nNo] = nData;
}