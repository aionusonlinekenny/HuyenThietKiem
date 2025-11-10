#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndList2.h"
#include "UiImage.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//=======================
//
//=======================
struct KWndList2Item
{	
	bool	Status;
	int		Data;
	DWORD	StringSize;
	char	String[3][32];
};


KWndList2::KWndList2()
{
	m_pContent		= NULL;
	m_nItemSpace	= 0;
	m_pScrollbar    = NULL;
	m_nNumItem		= 0;	
	m_nFontSize		= 12;
	m_nTopItemIndex = 0;	
	m_nSelItemIndex		  = -1;
	m_nHighLightItemIndex = -1;
	m_ItemOnlineColor = m_ItemOnlineBorderColor = m_SelItemOnlineColor = 0, m_SelItemOnlineBorderColor = 0;
	m_ItemOfflineColor = m_ItemOfflineBorderColor = m_SelItemOfflineColor = 0, m_SelItemOfflineBorderColor = 0;
	m_uSelItemBgColor	= 0;
}

KWndList2::~KWndList2()
{
	ResetContent();
}


void KWndList2::Clone(KWndList2* pCopy)
{
	if (pCopy)
	{
		pCopy->m_nFontSize	= m_nFontSize;
		pCopy->m_ItemOnlineColor	= m_ItemOnlineColor;
		pCopy->m_ItemOnlineBorderColor	= m_ItemOnlineBorderColor;
		pCopy->m_SelItemOnlineColor	= m_SelItemOnlineColor;
		pCopy->m_SelItemOnlineBorderColor = m_SelItemOnlineBorderColor;
		pCopy->m_ItemOfflineColor = m_ItemOfflineColor;
		pCopy->m_ItemOfflineBorderColor = m_ItemOfflineBorderColor;
		pCopy->m_SelItemOfflineColor = m_SelItemOfflineColor;
		pCopy->m_SelItemOfflineBorderColor = m_SelItemOfflineBorderColor;
		pCopy->m_uSelItemBgColor = m_uSelItemBgColor;
	}
}
	
int KWndList2::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 16, &m_nFontSize);
		if (m_nFontSize < 8)
			m_nFontSize = 8;
		char	Buff[16];
		pIniFile->GetString(pSection, "OnlineColor", "", Buff, 16);
		m_ItemOnlineColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OnlineBorderColor", "", Buff, 16);
		m_ItemOnlineBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OnlineSelColor", "", Buff, 16);
		m_SelItemOnlineColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OnlineSelBorderColor", "", Buff, 16);
		m_SelItemOnlineBorderColor = GetColor(Buff);

		pIniFile->GetString(pSection, "OfflineColor", "", Buff, 16);
		m_ItemOfflineColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OfflineBorderColor", "", Buff, 16);
		m_ItemOfflineBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OfflineSelColor", "", Buff, 16);
		m_SelItemOfflineColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OfflineSelBorderColor", "", Buff, 16);
		m_SelItemOfflineBorderColor = GetColor(Buff);
		
		int		nValue;
		if (pIniFile->GetString(pSection, "SelBgColor", "", Buff, 16) && Buff[0])
		{
			m_uSelItemBgColor = GetColor(Buff) & 0xffffff;
			pIniFile->GetInteger(pSection, "SelBgColorAlpha", 0, &nValue);
			m_uSelItemBgColor |= (((255 - nValue) << 21) & 0xff000000);
		}
		
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
			m_Style &= ~WNDLIST_ES_HIGHLIGHT_ENABLE;

		return true;
	}
	return false;
}

void KWndList2::SetScrollbar(KWndScrollBar* pScroll)
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

int KWndList2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
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

int KWndList2::SetCurSel(int nSel)
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

int	KWndList2::IsItemSel(int nIndex)
{
	return (m_nSelItemIndex == nIndex);
}

void KWndList2::OnLButtonDown(int x, int y)
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

void KWndList2::OnMouseMove(int x, int y)
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

int KWndList2::AddString(int nIndex, const char* pString1, const char* pString2, const char* pString3, bool bStatus /*= true*/)
{
	if (pString1 == NULL || pString2 == NULL)
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
			return WNDLIST_ERROR;
	}
		
	int nStrLen1 = strlen(pString1);
	int nStrLen2 = strlen(pString2);
	int nStrLen3 = strlen(pString3);
	KWndList2Item* pItem = (KWndList2Item*)malloc(sizeof(KWndList2Item));
	if (pItem)
	{	
		pItem->Data = 0;
		pItem->StringSize = nStrLen1 + nStrLen2 + nStrLen3;
		pItem->Status = bStatus;
		memcpy(pItem->String[0], pString1, nStrLen1);
		pItem->String[0][nStrLen1] = 0;
		memcpy(pItem->String[1], pString2, nStrLen2);
		pItem->String[1][nStrLen2] = 0;
		memcpy(pItem->String[2], pString3, nStrLen3);
		pItem->String[2][nStrLen3] = 0;
		
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

int	KWndList2::DeleteString(int nIndex)
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
void KWndList2::ResetContent()
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
void KWndList2::PaintWindow()
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
			if (IsItemSel(i))
			{			
				KRUShadow	Shadow;
				Shadow.Color.Color_dw = m_uSelItemBgColor;
				Shadow.oPosition.nX = m_nAbsoluteLeft;
				Shadow.oPosition.nY = y;
				Shadow.oPosition.nZ = 0;
				Shadow.oEndPos.nX = m_nAbsoluteLeft + m_Width;
				Shadow.oEndPos.nY = y + m_nFontSize + 1;
				g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
			}
			
			KWndList2Item* pItem = (KWndList2Item*)m_pContent[i];

			if(pItem->Status)
			{
				if (i == m_nSelItemIndex)
				{
					Color = m_SelItemOnlineColor;
					BorderColor = m_SelItemOnlineBorderColor;
				}
				else
				{
					Color = m_ItemOnlineColor;
					BorderColor = m_ItemOnlineBorderColor;
				}
			}
			else
			{
				if (i == m_nSelItemIndex)
				{
					Color = m_SelItemOfflineColor;
					BorderColor = m_SelItemOfflineBorderColor;
				}
				else
				{
					Color = m_ItemOfflineColor;
					BorderColor = m_ItemOfflineBorderColor;
				}
			}
			
			const char* pShowString = TGetLimitLenString((const char*)(pItem->String[0]), -1, szBuffer, nMaxLen);
			int nLen = strlen(pShowString);
			g_pRepresentShell->OutputText(m_nFontSize, pShowString, nLen, m_nAbsoluteLeft + 6, y, Color, 0, TEXT_IN_SINGLE_PLANE_COORD, BorderColor);
			pShowString = TGetLimitLenString((const char*)(pItem->String[1]), -1, szBuffer, nMaxLen);
			nLen = strlen(pShowString);
			g_pRepresentShell->OutputText(m_nFontSize, pShowString, nLen, m_nAbsoluteLeft + 35, y, Color, 0, TEXT_IN_SINGLE_PLANE_COORD, BorderColor);
			pShowString = TGetLimitLenString((const char*)(pItem->String[2]), -1, szBuffer, nMaxLen);
			nLen = strlen(pShowString);
			g_pRepresentShell->OutputText(m_nFontSize, pShowString, nLen, m_nAbsoluteLeft + 150, y, Color, 0, TEXT_IN_SINGLE_PLANE_COORD, BorderColor);
			y += m_nFontSize + 1;
		}
	}
}
int	KWndList2::FindString(int nPrecedingStart, const char* pString)
{
	if (pString)
	{
		int nLen = strlen(pString);		
		for (int i = nPrecedingStart + 1; i < m_nNumItem; i++)
		{
			KWndList2Item*	pItem = (KWndList2Item*)m_pContent[i];
			int nLen2 = strlen(pItem->String[0]);	
			if (nLen == nLen2 &&
				memcmp(pItem->String[0], pString, nLen) == 0)
				return i;
		}
	}
	return WNDLIST_ERROR;
}
int	KWndList2::SetItemData(int nIndex, int nData)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		((KWndList2Item*)m_pContent[nIndex])->Data = nData;
		return 0;
	}
	return WNDLIST_ERROR;
}
int KWndList2::GetItemData(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nNumItem)
	{
		return (((KWndList2Item*)m_pContent[nIndex])->Data);
	}
	return WNDLIST_ERROR;
}
int KWndList2::GetString(int nIndex, char* pBuffer, unsigned int nSize)
{
	if (nIndex >= 0 && nIndex < m_nNumItem && pBuffer)
	{
		KWndList2Item* pItem = (KWndList2Item*)m_pContent[nIndex];
		if (pItem->StringSize < nSize)
		{
			memcpy(pBuffer, pItem->String, pItem->StringSize);
			pBuffer[pItem->StringSize] = 0;
			return pItem->StringSize;
		}
	}
	return WNDLIST_ERROR;
}
void KWndList2::SetTopItemIndex(int nTopItemIndex)
{
	if (nTopItemIndex >= 0 && nTopItemIndex < m_nNumItem)
		m_nTopItemIndex = nTopItemIndex;
}

int	KWndList2::GetVisibleLineCount() const
{
	return m_Height / (m_nFontSize + 1);
}
