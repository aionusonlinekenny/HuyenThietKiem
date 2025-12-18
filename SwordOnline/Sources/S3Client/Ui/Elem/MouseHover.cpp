#include "KWin32.h"
#include "KIniFile.h"
#include "MouseHover.h"
#include "AutoLocateWnd.h"
#include "WndWindow.h"
#include "../UiBase.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Engine/Src/Text.h"
#include "SpecialFuncs.h"
extern iRepresentShell*	g_pRepresentShell;
#include "KEngine.h"

#include "../../../core/src/CoreShell.h"
extern iCoreShell*	g_pCoreShell;

KMouseOver	g_MouseOver;

#define	SCHEME_INI				"MouseHover.ini"
#define	INFO_MIN_LEN			26
#define FOLLOW_CURSOR_OFFSET_X	16
#define FOLLOW_CURSOR_OFFSET_Y	8
#define	SCREEN_WIDTH			800//1024

static unsigned int	s_uHoverObjDestTextColor = 0xffffffff;

// --
//
// --
void SetHoverObjDescColor(unsigned int uColor)
{
	s_uHoverObjDestTextColor = uColor;
}

// --
//
// --
void SetMouseHoverObjectDesc(void* pWnd, int nObj, unsigned int uGenre,
			unsigned int uId, int nContainer, int x, int y)
{
	KGameObjDesc	Desc;
	int		nLenTitle = 0, nLenProp = 0, nLenDesc = 0;

	g_MouseOver.CancelMouseHoverInfo();

	if (g_pCoreShell)
	{
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre = uGenre;
		Obj.Obj.uId = uId;
		Obj.Region.h = Obj.Region.v = 0;
		Obj.Region.Width = Obj.Region.Height = 0;
		Obj.nContainer = nContainer;

		Desc.szDesc[0] = 0;
		Desc.szProp[0] = 0;
		Desc.szTitle[0] = 0;

		unsigned uIndex = GDI_GAME_OBJ_DESC;
		if (g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
			uIndex = GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO;
		else if (g_UiBase.GetStatus() == UIS_S_TRADE_NPC ||
			g_UiBase.GetStatus() == UIS_S_TRADE_BUY ||
			g_UiBase.GetStatus() == UIS_S_TRADE_SALE)
			uIndex = GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO;

		g_pCoreShell->GetGameData(uIndex, (unsigned int)&Obj, (int)&Desc);
		g_MouseOver.SetMouseHoverInfo(pWnd, nObj, x, y, false, false);
		g_MouseOver.SetMouseHoverImage(g_pCoreShell->GetGameData(GDI_GAME_OBJ_DESC_INCLUDE_MOUSEHOVER, 
			(unsigned int)&Obj, (int)&g_MouseOver.m_HoverImage.szImage) == 1); //them

		if (Desc.szTitle[0])
		{
			nLenTitle = TEncodeText(Desc.szTitle, strlen(Desc.szTitle));
			g_MouseOver.SetMouseHoverTitle(Desc.szTitle, nLenTitle,s_uHoverObjDestTextColor);
		}
		if (Desc.szProp[0])
		{
			nLenProp = TEncodeText(Desc.szProp, strlen(Desc.szProp));
			g_MouseOver.SetMouseHoverProp(Desc.szProp, nLenProp, s_uHoverObjDestTextColor);
		}
		if (Desc.szDesc[0])
		{
			nLenDesc = TEncodeText(Desc.szDesc, strlen(Desc.szDesc));
			g_MouseOver.SetMouseHoverDesc(Desc.szDesc, nLenDesc, s_uHoverObjDestTextColor);
		}
		// FIX: Add CGOG_EQUIP to support gold equip mask items (DetailType=11)
		if(uGenre == CGOG_ITEM || uGenre == CGOG_EQUIP || uGenre == CGOG_NPCSELLITEM || uGenre == CGOG_PLAYERSELLITEM)
			{
			char szIconName[80];
			int nItemGenre = g_pCoreShell->GetGameData(GDI_ITEM_GENRE, (unsigned int)&Obj, 0);
			int nDetailType = g_pCoreShell->GetGameData(GDI_ITEM_DETAIL, 0, (int)&Obj);

			// FIX: Check DetailType=11 directly, don't filter by ItemGenre
			// Gold mask items can have ItemGenre=2, not just 0
			if(nDetailType == 11)
			{
					if(g_pCoreShell->GetGameData(GDI_MASK_IMAGE, (unsigned int)&Obj, (int)&szIconName))
					{
						g_MouseOver.SetMouseHoverIcon((const char*)szIconName);
					}
					}
				}
			}
		}
// --
//
// --
int DrawDraggingGameObjFunc(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult)
{
	g_pCoreShell->DrawGameObj(Obj.uGenre, Obj.uId, x, y, 0, 0, 0);
	return false;
}

// --
//
// --
KMouseOver::KMouseOver()
{
	m_nImgWidth  = 0;
	m_nImgHeight = 0;
	m_nLeft      = 0;
	m_nTop       = 0;
	m_nWndWidth  = 0;
	m_nWndHeight = 0;
	m_nIndent    = 0;
	m_nFontSize  = 12;
	m_pMouseHoverWnd = NULL;
	m_nObj = 0;
	m_ObjTitle[0]  =0;
	m_nTitleLen    = 0;
	m_ObjProp[0]   = 0;
	m_nPropLen     = 0;
	m_ObjDesc[0]   = 0;
	m_nDescLen     = 0;
	m_uTitleColor  = 0;
	m_uPropColor   = 0;
	m_uDescColor   = 0;
	m_uTitleBgColor= 0;
	m_uPropBgColor = 0;
	m_uDescBgColor = 0;
	memset(&m_Image, 0, sizeof(KRUImage));
	m_bHeadTailImg = true;
	m_bFollowCursor = false;
	m_bTempImg = false;
	m_bShow = false;
	memset(&m_IconImage, 0, sizeof(KRUImage));
	m_cLoopFrameTime	= 0;
	m_wNumFrames		= 0;
}

// --
//
// --
int  KMouseOver::IsMoseHoverWndObj(void* pWnd, int nObj)
{
	return ((pWnd == m_pMouseHoverWnd) && (nObj == m_nObj));
}

// --
//
// --
void KMouseOver::CancelMouseHoverInfo()
{
	m_pMouseHoverWnd = 0;
	m_nObj = 0;
	m_ObjTitle[0] = 0;
	m_nTitleLen = 0;
	m_ObjProp[0] = 0;
	m_nPropLen = 0;
	m_ObjDesc[0] = 0;
	m_nDescLen = 0;
	m_bShow = false;
	memset(&m_IconImage, 0, sizeof(KRUImage));
	m_wNumFrames	= 0;
	if (m_bTempImg)
		SetMouseHoverImage(false);
}

// --
//
// --
void KMouseOver::SetMouseHoverInfo(void* pWnd, int nObj, int x, int y,
		                        bool bHeadTailImg, bool bFollowCursor)
{
    m_pMouseHoverWnd = pWnd;
	m_nObj = nObj;
	m_bHeadTailImg = bHeadTailImg;
	m_bFollowCursor = bFollowCursor;
	m_nApplyX = x;
	m_nApplyY = y;
	m_ObjTitle[0] = 0;
	m_nTitleLen = 0;
	m_ObjProp[0] = 0;
	m_nPropLen = 0;
	m_ObjDesc[0] = 0;
	m_nDescLen = 0;
	m_bShow = false;
}

// --
//
// --

void KMouseOver::SetMouseHoverImage(bool bAdd)
{
	KImageParam	Param;
	if (bAdd)
	{
		if (g_pRepresentShell->GetImageParam(m_HoverImage.szImage, &Param, ISI_T_SPR) == true)
		{
			m_HoverImage.nFlipTime = IR_GetCurrentTime();
			//m_HoverImage.nInterval = Param.nInterval;
			m_HoverImage.nInterval = 80; //true
			m_HoverImage.nNumFrames = Param.nNumFrames;
			m_HoverImage.nType = ISI_T_SPR;
			m_HoverImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_HoverImage.Color.Color_b.a = 255;
			m_HoverImage.uImage = 0;
			m_HoverImage.nISPosition = IMAGE_IS_POSITION_INIT;
			m_HoverImage.nFrame = 0;

			m_nWndWidthReduce = Param.nWidth / 2;
			m_nWndHeightReduce = Param.nHeight / 4;
		}
		m_bTempImg = true;
	}
	else
	{
		IR_InitUiImageRef(m_HoverImage);
		m_nWndWidthReduce = 0;
		m_nWndHeightReduce = 0;
		m_nHeightReduce = 0;
		m_nMaxWidthReduce = 0;
		m_nMaxHeightReduce = 0;
		m_bTempImg = false;
	}
}
// --
//
// --
void KMouseOver::SetMouseHoverTitle(const char *pTitleText, int nTitleTextLen, UINT uColor)
{
	if (nTitleTextLen > 0 && pTitleText && nTitleTextLen <= GOD_MAX_OBJ_TITLE_LEN)
	{
		memcpy(m_ObjTitle, pTitleText, nTitleTextLen);
		m_nTitleLen = nTitleTextLen;
		m_uTitleColor = uColor;
		Update(m_nApplyX, m_nApplyY);
	}
	else
	{
		m_ObjTitle[0] = 0;
		m_nTitleLen = 0;
	}
}

// --
//
// --
void KMouseOver::SetMouseHoverProp(const char *pPropText, int nPropTextLen, UINT uColor)
{
	if (nPropTextLen > 0 && pPropText && nPropTextLen <= GOD_MAX_OBJ_PROP_LEN)
	{
		memcpy(m_ObjProp, pPropText, nPropTextLen);
		m_nPropLen = nPropTextLen;
		m_uPropColor = uColor;
		Update(m_nApplyX, m_nApplyY);
	}
	else
	{
		m_ObjProp[0] = 0;
		m_nPropLen = 0;
	}
}

// --
//
// --
void KMouseOver::SetMouseHoverDesc(const char *pDescText, int nDescTextLen, UINT uColor)
{
	if (nDescTextLen > 0 && pDescText && nDescTextLen <= GOD_MAX_OBJ_DESC_LEN)
	{
		memcpy(m_ObjDesc, pDescText, nDescTextLen);
		m_nDescLen = nDescTextLen;
		m_uDescColor = uColor;
		Update(m_nApplyX, m_nApplyY);
	}
	else
	{
		m_ObjDesc[0] = 0;
		m_nDescLen = 0;
	}
}

// --
//
// --
void KMouseOver::SetMouseHoverIcon(const char *pcszIcon)
{
	if(pcszIcon[0])
	{	
		strcpy(m_IconImage.szImage, pcszIcon);
		m_IconImage.nType = ISI_T_SPR;
		m_IconImage.Color.Color_b.a = 255;
		m_IconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		m_IconImage.uImage = 0;
		m_IconImage.nISPosition = IMAGE_IS_POSITION_INIT;
		m_IconImage.bRenderFlag = 1;
		m_IconImage.oPosition.nX = m_nLeft;
		m_IconImage.oPosition.nY = m_nTop; 
		m_IconImage.oPosition.nZ = 0;
		m_IconImage.nFrame = 0;
		
		KImageParam	Param;
		Param.nNumFrames = 0;
		if (g_pRepresentShell)
			g_pRepresentShell->GetImageParam(m_IconImage.szImage, &Param, m_IconImage.nType);
		if(Param.nNumFrames > 0)
		{
			m_wNumFrames = (WORD)Param.nNumFrames;
		}
	}
}

// --
//
// --
void KMouseOver::Update(int nX, int nY)
{
	m_bShow = false;

	if (g_pRepresentShell == NULL)
		return;

	int nMaxTitleLen, nMaxPropLen, nMaxDescLen;

	m_nMaxLineLen = 0;
	if (m_nTitleLen > 0)
	{
		m_nTitleLineNum = TGetEncodedTextLineCount(
			m_ObjTitle, m_nTitleLen, 0, nMaxTitleLen, m_nFontSize);
		m_nMaxLineLen = nMaxTitleLen;
	}
	else
	{
		m_nTitleLineNum = 0;
	}
	if (m_nPropLen > 0)
	{
		m_nPropLineNum = TGetEncodedTextLineCount(
			m_ObjProp, m_nPropLen, 0, nMaxPropLen, m_nFontSize);
		if(m_nMaxLineLen < nMaxPropLen + 3)
			m_nMaxLineLen = nMaxPropLen + 3;
	}
	else
	{
		m_nPropLineNum = 0;
	}
	if (m_nDescLineNum > 0)
	{
		m_nDescLineNum = TGetEncodedTextLineCount(
			m_ObjDesc, m_nDescLen, 0, nMaxDescLen, m_nFontSize);
		if(m_nMaxLineLen < nMaxDescLen)
		    m_nMaxLineLen = nMaxDescLen;
	}
	else
	{
		m_nDescLineNum = 0;
	}


	int nNumLine = m_nTitleLineNum + m_nPropLineNum + m_nDescLineNum;
	if (nNumLine == 0)
		return;

	if (m_bFollowCursor == false && m_nMaxLineLen < INFO_MIN_LEN)
		m_nMaxLineLen = INFO_MIN_LEN;
	m_nWndWidth = m_nFontSize * m_nMaxLineLen / 2 + m_nIndent * 2;
	m_nWndHeight = (m_nFontSize + 1) * nNumLine;
	if (m_bHeadTailImg)
		m_nWndHeight += m_nImgHeight * 2;
	if (m_bFollowCursor)
	{
		m_nLeft = nX + FOLLOW_CURSOR_OFFSET_X;
		m_nTop  = nY + FOLLOW_CURSOR_OFFSET_Y;
	}
	else
	{
		ALW_GetWndPosition(m_nLeft, m_nTop, m_nWndWidth, m_nWndHeight);
	}
	m_bShow = true;
}

// --
//
// --
void KMouseOver::UpdateCursorPos(int nX, int nY)
{
	if (m_bFollowCursor && m_bShow)
	{
		m_nLeft = nX + FOLLOW_CURSOR_OFFSET_X;
		m_nTop  = nY + FOLLOW_CURSOR_OFFSET_Y;
	}
}

// --
//
// --
void KMouseOver::OnWndClosed(void* pWnd)
{
	if (pWnd && pWnd == m_pMouseHoverWnd)
		CancelMouseHoverInfo();
}

// --
//
// --
void KMouseOver::LoadScheme(const char* pScheme)
{	
	if (pScheme == NULL)
		return;

	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Ini.GetShort("MouseOverWnd", "ImgWidth",    0, &m_nImgWidth);
		Ini.GetShort("MouseOverWnd", "ImgHeight",   0, &m_nImgHeight);
		Ini.GetInteger("MouseOverWnd", "Indent",      0, &m_nIndent);
		Ini.GetInteger("MouseOverWnd", "Font",        0, &m_nFontSize);

		if(m_nImgWidth < 0)
			m_nImgWidth = 0;
		if(m_nImgHeight < 0)
			m_nImgWidth = 0;
		if(m_nFontSize < 8)
			m_nFontSize = 8;
		int nValue;
		Ini.GetInteger("MouseOverWnd", "ImgType", 0, &nValue);
		if (nValue == 1)
		{
			m_Image.nType = ISI_T_BITMAP16;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
		}
		else
		{
			m_Image.nType = ISI_T_SPR;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_Image.Color.Color_b.a = 255;
		}
		m_Image.uImage = 0;
		m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
		Ini.GetString("MouseOverWnd", "Image", "" , m_Image.szImage, sizeof(m_Image.szImage));
		Ini.GetInteger("MouseOverWnd", "Frame", 0, &nValue);
		m_Image.nFrame = nValue;

		Ini.GetString("MouseOverWnd", "TitleBgColor", "0, 0, 0", Buff, sizeof(Buff));
		m_uTitleBgColor = ((GetColor(Buff) & 0xffffff) | 0x0a000000);

		Ini.GetString("MouseOverWnd", "PropBgColor", "0, 0, 0", Buff, sizeof(Buff));
		m_uPropBgColor = ((GetColor(Buff) & 0xffffff) | 0x0a000000);

		Ini.GetString("MouseOverWnd", "DescBgColor", "0, 0, 0", Buff, sizeof(Buff));
		m_uDescBgColor = ((GetColor(Buff) & 0xffffff) | 0x0a000000);
		
		Ini.GetString("MouseOverWnd", "PicBgColor", "0, 0, 0", Buff, sizeof(Buff));
		m_uPicBgColor = ((GetColor(Buff) & 0xffffff) | 0x0a000000);

		Ini.GetString("MouseOverWnd", "RectBgColor", "0, 0, 0", Buff, sizeof(Buff));
		m_uRectBgColor = ((GetColor(Buff) & 0xffffff) | 0x0a000000);

		Update((m_nLeft + m_nWndWidth) / 2, m_nTop);
	}
}

// --
//
// --
void KMouseOver::PaintMouseHoverInfo()
{
	if (m_bShow == false || g_pRepresentShell == NULL)
		return;

	KRUShadow Shadow;
	KOutputTextParam	Param;
	Param.BorderColor = 0;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;	
	//fix tip by kinnox;
	if (m_nLeft + m_nWndWidth <= SCREEN_WIDTH)
		Shadow.oPosition.nX = m_nLeft;
	else
	{
		if (m_nLeft - m_nWndWidth  - 10 < 0)
			Shadow.oPosition.nX = 0;
		else
			Shadow.oPosition.nX = m_nLeft - m_nWndWidth  - 10;
	}
	
	Shadow.oPosition.nY = m_nTop;
	//fix tip by kinnox;
	if (m_bHeadTailImg)
	{
		Shadow.oPosition.nY = m_nTop + m_nImgHeight;
	}
	else
	{	
		Shadow.oPosition.nY = m_nTop;
	}
	//fix tip by kinnox;
	if (m_nLeft + m_nWndWidth <= SCREEN_WIDTH)
		Shadow.oEndPos.nX = m_nLeft + m_nWndWidth;
	else
	{
		if (m_nLeft - m_nWndWidth  - 10 < 0)
			Shadow.oEndPos.nX = m_nWndWidth;
		else
			Shadow.oEndPos.nX = m_nLeft - 10;
			
	}
	//
//	//g_DebugLog("Hinh: %s",m_IconImage.szImage[0]);
	if(m_IconImage.szImage[0])
	{
		Shadow.oEndPos.nX = Shadow.oPosition.nX + m_nWndWidth + 120;
		if(Shadow.oEndPos.nX > SCREEN_WIDTH)
		{	
			int nSurs = 0;
			nSurs = Shadow.oEndPos.nX - SCREEN_WIDTH;
			Shadow.oPosition.nX -= nSurs;
			Shadow.oEndPos.nX -= nSurs;
		}
	}
	else
	{
		Shadow.oEndPos.nX = Shadow.oPosition.nX + m_nWndWidth;
	}
	Shadow.oEndPos.nY = Shadow.oPosition.nY;
	if(m_nTitleLen > 0)
	{
		Shadow.Color.Color_dw = m_uTitleBgColor;
		Shadow.oEndPos.nY += (m_nFontSize + 1) * m_nTitleLineNum;
		if(m_IconImage.szImage[0])
			Shadow.oEndPos.nY += 40;
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);

		Param.nSkipLine = 0;
		Param.nNumLine = 1;
		Param.Color = m_uTitleColor;
		Param.BorderColor = 0xff000000;
		Param.nY = Shadow.oPosition.nY;
		int nLineLen;
		while(true)
		{
			if (TGetEncodedTextLineCount(m_ObjTitle, m_nTitleLen, 0, nLineLen, m_nFontSize, Param.nSkipLine, 1) == 0)
				break;
			if(m_IconImage.szImage[0])
			{	
				Param.nX = Shadow.oPosition.nX + m_nWndWidth / 2 - (nLineLen * m_nFontSize) / 4 + 120;
			}
			else
			{
				if (m_nLeft + m_nWndWidth <= SCREEN_WIDTH)
			{
					Param.nX = m_nLeft + (m_nWndWidth + m_nMaxWidthReduce) / 2 - (nLineLen * m_nFontSize) / 4;
			}
			else
			{
				if (m_nLeft - (m_nWndWidth + m_nMaxWidthReduce)  - 10 < 0)
				{
						Param.nX = (m_nWndWidth + m_nMaxWidthReduce) / 2 - (nLineLen * m_nFontSize) / 4;
				}
				else
				{
						Param.nX = m_nLeft  - 10 - (m_nWndWidth + m_nMaxWidthReduce) / 2 - (nLineLen * m_nFontSize) / 4;
				}
			}
			}
			g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_ObjTitle, m_nTitleLen, 0);
			Param.nSkipLine ++;
			Param.nY += m_nFontSize + 1;
		};
	}

	if(m_nPropLen > 0)
	{
		Shadow.Color.Color_dw = m_uPropBgColor;
		Shadow.oPosition.nY = Shadow.oEndPos.nY;
		Shadow.oEndPos.nY += (m_nFontSize + 1) * m_nPropLineNum;
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);

		Param.nSkipLine = 0;
		Param.Color = m_uPropColor;
		Param.BorderColor = 0xff000000;
		Param.nNumLine = m_nPropLineNum;
		Param.nX = Shadow.oPosition.nX + m_nIndent;
		Param.nY = Shadow.oPosition.nY;
		OutputTabSplitText(m_ObjProp, m_nPropLen, m_nWndWidth - m_nIndent * 2, m_nFontSize, &Param);
	}

	if(m_nDescLen > 0)
	{
		Shadow.Color.Color_dw = m_uDescBgColor;
		Shadow.oPosition.nY = Shadow.oEndPos.nY;
		Shadow.oEndPos.nY += (m_nFontSize + 1) * m_nDescLineNum;
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
		
		Param.nSkipLine = 0;
		Param.Color = m_uDescColor;
		Param.BorderColor = 0xff000000;
		Param.nNumLine = m_nDescLineNum;
		Param.nX = Shadow.oPosition.nX + m_nIndent;
		Param.nY = Shadow.oPosition.nY;
		g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_ObjDesc, m_nDescLen, 0);
	}

	if (m_bHeadTailImg && m_nImgWidth > 0)
	{
		m_Image.oPosition.nX = Shadow.oPosition.nX;
		while(m_Image.oPosition.nX < m_nLeft + m_nWndWidth)
		{			
			m_Image.oPosition.nY = m_nTop;
			g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
			m_Image.oPosition.nY = Shadow.oEndPos.nY;
			g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
			m_Image.oPosition.nX += m_nImgWidth;
		};
	}
	if(m_IconImage.szImage[0])
	{
		m_cLoopFrameTime++;
		m_IconImage.oPosition.nX = Shadow.oPosition.nX + 60;
		m_IconImage.oPosition.nY = m_nTop + 80; 
		if(m_cLoopFrameTime % 6 == 0)
		{
			if(m_IconImage.nFrame <= m_wNumFrames)
				m_IconImage.nFrame++;
			else
				m_IconImage.nFrame = 0;
			m_cLoopFrameTime = 0;
		}
		g_pRepresentShell->DrawPrimitives(1, &m_IconImage, RU_T_IMAGE, true);
	}
	
	if (m_bTempImg)
	{
		m_HoverImage.oPosition.nX = m_nLeft - m_nWndWidthReduce / 2;
		m_HoverImage.oPosition.nY = m_nTop - m_nWndHeightReduce - m_nWndHeight / 2 ;
		IR_NextFrame(m_HoverImage);
		g_pRepresentShell->DrawPrimitives(1, &m_HoverImage, RU_T_IMAGE, true);
	}
}


