/*****************************************************************************************
//	界面窗口体系结构// --列表窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-11
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndListFilterItem.h"
#include "UiImage.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

struct KWndListFilterItemItem
{
	int		nOption;
	int		nValue;
	USHORT	StringSize;
	unsigned int m_Color;
	char	String[128];
};

KWndListFilterItem::KWndListFilterItem()
{
	m_pContent		= NULL;
	m_nItemSpace	= 0;
	m_pScrollbar    = NULL;
	m_nNumItem		= 0;	
	m_nFontSize		= 12;
	m_nTopItemIndex = 0;	
	m_nSelItemIndex		  = -1;
	m_nHighLightItemIndex = -1;
	m_ItemBorderColor = m_SelItemBorderColor = m_HighLightBorderColor = 0;
}

KWndListFilterItem::~KWndListFilterItem()
{
	ResetContent();
}

void KWndListFilterItem::Clone(KWndListFilterItem* pCopy)
{
	if (pCopy)
	{
		pCopy->m_nFontSize	= m_nFontSize;
		pCopy->m_ItemColor	= m_ItemColor;
		pCopy->m_SelItemColor	= m_SelItemColor;
		pCopy->m_HighLightColor	= m_HighLightColor;
		pCopy->m_ItemBorderColor = m_ItemBorderColor;
		pCopy->m_SelItemBorderColor = m_SelItemBorderColor;
		pCopy->m_HighLightBorderColor = m_HighLightBorderColor;
	}
}

int KWndListFilterItem::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 16, &m_nFontSize);
		if (m_nFontSize < 8)
			m_nFontSize = 8;
		char	Buff[16];
		pIniFile->GetString(pSection, "Color", "", Buff, 16);
		m_ItemColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "", Buff, 16);
		m_ItemBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelColor", "", Buff, 16);
		m_SelItemColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelBorderColor", "", Buff, 16);
		m_SelItemBorderColor = GetColor(Buff);

		int		nValue;		
		pIniFile->GetInteger(pSection, "HighLight", 0, &nValue);
		if (nValue)
		{
			m_Style |= WNDLIST_ES_HIGHLIGHT_ENABLE;
			pIniFile->GetString(pSection, "HighLightColor", "", Buff, 16);
			m_HighLightColor = GetColor(Buff);
			pIniFile->GetString(pSection, "HighLightBorderColor", "", Buff, 16);
			m_HighLightBorderColor = GetColor(Buff);
		}
		else
		{
			m_Style &= ~WNDLIST_ES_HIGHLIGHT_ENABLE;
		}

		return true;
	}
	return false;
}


void KWndListFilterItem::SetScrollbar(KWndScrollBar* pScroll)
{
	if (m_pScrollbar = pScroll)
	{
		m_pScrollbar->SetStyle(m_pScrollbar->GetStyle() | WND_S_MOVEALBE);
		if (m_nNumItem > GetVisibleLineCount())
		{
			m_pScrollbar->Enable(true);
			m_pScrollbar->SetValueRange(0, m_nNumItem - GetCount());
		}
		else
			m_pScrollbar->Enable(false);
	}
}


int KWndListFilterItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_MOUSEWHEEL:
		if (m_pScrollbar && !m_pScrollbar->IsDisable())
		{
			int zDelta = short(HIWORD(uParam));
			int nPos = m_pScrollbar->GetScrollPos();
			nPos += (-zDelta / WHEEL_DELTA);
			m_pScrollbar->SetScrollPos(nPos);
		}
		break;
	default:
		return KWndWindow::WndProc(uMsg, nParam, nParam);
	}
	return 0;
}


int KWndListFilterItem::SetCurSel(int nSel)
{
	if (nSel >= 0 && nSel < m_nNumItem && nSel != m_nSelItemIndex)
	{
		m_nSelItemIndex = nSel;
		if (m_nSelItemIndex < m_nTopItemIndex)
			m_nTopItemIndex = m_nSelItemIndex;
		else if(m_nSelItemIndex >= m_nTopItemIndex + GetVisibleLineCount())
		{
			m_nTopItemIndex = m_nSelItemIndex - GetVisibleLineCount() + 1;
			if (m_nTopItemIndex < 0)
				m_nTopItemIndex = 0;
		}
		
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelItemIndex);
	}
	return m_nSelItemIndex;
}

void KWndListFilterItem::OnLButtonDown(int x, int y)
{
	int nSel = m_nTopItemIndex + (y - m_nAbsoluteTop) / (m_nFontSize + 1);
	if (nSel >= m_nNumItem)
		nSel = -1;
	if (nSel != m_nSelItemIndex)
	{
		m_nSelItemIndex = nSel;
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelItemIndex);
	}
}


void KWndListFilterItem::OnMouseMove(int x, int y)
{
	if (m_Style & WNDLIST_ES_HIGHLIGHT_ENABLE)
	{
		int nIndex = m_nTopItemIndex + (y - m_nAbsoluteTop) / (m_nFontSize + 1);
		if (nIndex >= m_nNumItem)
			nIndex = -1;
		if (nIndex != m_nHighLightItemIndex)
		{
			m_nHighLightItemIndex = nIndex;
			if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_LIST_ITEM_HIGHLIGHT, (unsigned int)(KWndWindow*)this, m_nHighLightItemIndex);
		}
	}
}


int KWndListFilterItem::AddString(int nIndex, const char* pString1, int nOption, int nValue, BOOL bHighLight /*= TRUE*/)
{	
	if (pString1 == NULL)
		return WNDLIST_ERROR;
	if (m_nItemSpace >= m_nNumItem)
	{
		void** pNew = (void**)realloc(m_pContent, sizeof(void*) * (m_nItemSpace + 10));
		if (pNew)
		{
			m_pContent = pNew;
			m_nItemSpace += 10;
		}
		else
		{
			return WNDLIST_ERROR;
		}
	}
	
	int nStrLen1 = strlen(pString1);
	KWndListFilterItemItem* pItem = (KWndListFilterItemItem*)malloc(sizeof(KWndListFilterItemItem));
	if (pItem)
	{	
		if(bHighLight)
		{
			pItem->m_Color = m_ItemColor;
		}
		else
		{
			pItem->m_Color = 0x0f005952;
		}

		pItem->nOption = nOption;
		pItem->nValue = nValue;
		pItem->StringSize = nStrLen1;;
		memcpy(pItem->String, pString1, nStrLen1);
		pItem->String[nStrLen1] = 0;

		
		if (nIndex < 0)
			nIndex = 0;
		else if (nIndex > m_nNumItem)
			nIndex = m_nNumItem;
		for (int i = m_nNumItem; i > nIndex; i--)
			m_pContent[i] = m_pContent[i - 1];
		m_pContent[nIndex] = pItem;
		m_nNumItem ++;

		if (m_Style & WNDLIST_ES_HIGHLIGHT_ENABLE)
		{
			m_nHighLightItemIndex = -1;
			if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_LIST_ITEM_HIGHLIGHT, (unsigned int)(KWndWindow*)this, m_nHighLightItemIndex);
		}
		if (m_nSelItemIndex >= nIndex)
		{
			m_nSelItemIndex ++;
			if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelItemIndex);
		}
		if (m_pScrollbar)
	    {
    		if(m_nNumItem > GetVisibleLineCount())
		    {
    			m_pScrollbar->Enable(TRUE);
		    }
		    else
		    {
    			m_pScrollbar->Enable(FALSE);
		    }
			m_pScrollbar->SetValueRange(0, m_nNumItem);
	    }
	}
	else
		nIndex = WNDLIST_ERROR;
	return nIndex;
}

int	KWndListFilterItem::DeleteString(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		free(m_pContent[nIndex]);
		m_nNumItem--;
		for (int i = nIndex; i < m_nNumItem; i++)
			m_pContent[i] = m_pContent[i + 1];
		if (m_nTopItemIndex >= m_nNumItem && m_nTopItemIndex)
			m_nTopItemIndex--;
		if (m_nSelItemIndex > nIndex)
			m_nSelItemIndex--;
		else if (m_nSelItemIndex == nIndex)
			m_nSelItemIndex = -1;
		if (m_Style & WNDLIST_ES_HIGHLIGHT_ENABLE)
		{
			m_nHighLightItemIndex = -1;
			if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_LIST_ITEM_HIGHLIGHT, (unsigned int)(KWndWindow*)this, m_nHighLightItemIndex);
		}
		if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelItemIndex);
		if (m_pScrollbar)
	    {
    		if(m_nNumItem > GetVisibleLineCount())
		    {
    			m_pScrollbar->Enable(TRUE);
		    }
		    else
		    {
    			m_pScrollbar->Enable(FALSE);
		    }
			m_pScrollbar->SetValueRange(0, m_nNumItem);
	    }
		return m_nNumItem;
	}
	else
		return WNDLIST_ERROR;
}

void KWndListFilterItem::ResetContent()
{
	if (m_pContent)
	{
		for (int i = 0; i < m_nNumItem; i++)
			free(m_pContent[i]);
		m_nNumItem = 0;
		m_nItemSpace = 0;
		free(m_pContent);
		m_pContent = NULL;
	}
	m_nTopItemIndex = 0;
	m_nSelItemIndex = -1;
	m_nHighLightItemIndex = -1;
	if (m_pParentWnd)
	{
		if (m_Style & WNDLIST_ES_HIGHLIGHT_ENABLE)
			m_pParentWnd->WndProc(WND_N_LIST_ITEM_HIGHLIGHT, (unsigned int)(KWndWindow*)this, m_nHighLightItemIndex);
		m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelItemIndex);
	}
	if(m_pScrollbar)
	{
		m_pScrollbar->Enable(FALSE);
		m_pScrollbar->SetScrollPos(0);
		m_pScrollbar->SetValueRange(0, 0);
	}
}

void KWndListFilterItem::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell)
	{
		int y = m_nAbsoluteTop;
		int EndIndex = m_nTopItemIndex + GetVisibleLineCount();
		if (EndIndex > m_nNumItem)
			EndIndex = m_nNumItem;
		
		int	nMaxLen = (m_Width * 2) / m_nFontSize + 1;
		char	szBuffer[128];
		if (nMaxLen > sizeof(szBuffer))
			nMaxLen = sizeof(szBuffer);

		unsigned int Color;
		unsigned int BorderColor;
		for (int i = m_nTopItemIndex; i < EndIndex; i++)
		{	
			KWndListFilterItemItem* pItem = (KWndListFilterItemItem*)m_pContent[i];
			if (i == m_nSelItemIndex)
			{
				Color = m_SelItemColor;
				BorderColor = m_SelItemBorderColor;
			}
			else if (i == m_nHighLightItemIndex)
			{
				Color = m_HighLightColor;
				BorderColor = m_HighLightBorderColor;
			}
			else
			{
				Color = pItem->m_Color;
				BorderColor = m_ItemBorderColor;
			}
			
			const char* pShowString1 = TGetLimitLenString((const char*)(pItem->String), -1, szBuffer, nMaxLen);
			char czPrint[128];
			sprintf(czPrint, "%s > %d", pShowString1, pItem->nValue);
			int nLen = strlen(czPrint);
			g_pRepresentShell->OutputText(m_nFontSize, czPrint, nLen, m_nAbsoluteLeft, y, Color, 0, TEXT_IN_SINGLE_PLANE_COORD, BorderColor);
			y += m_nFontSize + 1;
		}
	}
}

int	KWndListFilterItem::FindOptionIndex(int pOption)
{	
		for (int i = 0; i < m_nNumItem; i++)
		{
			KWndListFilterItemItem*	pItem = (KWndListFilterItemItem*)m_pContent[i];	
			if (pItem->nOption == pOption)
				return i;
		}
	return WNDLIST_ERROR;
}

int	KWndListFilterItem::SetItemValue(int nIndex, int nValue)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		((KWndListFilterItemItem*)m_pContent[nIndex])->nValue = nValue;
		return 0;
	}
	return WNDLIST_ERROR;
}

int KWndListFilterItem::GetItemValue(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		return (((KWndListFilterItemItem*)m_pContent[nIndex])->nValue);
	}
	return WNDLIST_ERROR;
}

int KWndListFilterItem::GetItemOption(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		return (((KWndListFilterItemItem*)m_pContent[nIndex])->nOption);
	}
	return WNDLIST_ERROR;
}

int KWndListFilterItem::GetString(int nIndex, char* pBuffer1, unsigned int nSize)
{
	if (nIndex >= 0 && nIndex < m_nNumItem && pBuffer1)
	{
		KWndListFilterItemItem* pItem = (KWndListFilterItemItem*)m_pContent[nIndex];
		if (pItem->StringSize < nSize)
		{	
			int nLen = strlen(pItem->String);
			memcpy(pBuffer1, pItem->String, nLen);
			pBuffer1[nLen] = 0;
			return pItem->StringSize;
		}
	}
	return WNDLIST_ERROR;
}

void KWndListFilterItem::SetTopItemIndex(int nTopItemIndex)
{
	if (nTopItemIndex >= 0 && nTopItemIndex < m_nNumItem)
		m_nTopItemIndex = nTopItemIndex;
}


int	KWndListFilterItem::GetVisibleLineCount() const
{
	return m_Height / (m_nFontSize + 1);
}
