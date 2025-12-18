#pragma once
#include "WndWindow.h"
#include "WndScrollBar.h"

#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0004			
#define	WNDLIST_ERROR					-1

class KWndList2 : public KWndWindow
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();						
	int				AddString(int nIndex, const char* pString1, const char* pString2, const char* pString3, bool bStatus = true);
	int				GetString(int nIndex, char* pBuffer, unsigned int nSize);
	int				DeleteString(int nIndex);				
	void			ResetContent();							
	int				FindString(int nPrecedingStart, const char* pString);
	int				SetItemData(int nIndex, int nData);		
	int				GetItemData(int nIndex);				
	void			SetTopItemIndex(int nTopItemIndex);		
	int				GetVisibleLineCount() const;			
	int				GetCount()  { return m_nNumItem; }		
	int				GetCurSel() { return m_nSelItemIndex;  }
	int				SetCurSel(int nSel);
	int				IsItemSel(int nIndex);
	void			SetScrollbar(KWndScrollBar* pScroll);
	KWndList2();
	virtual ~KWndList2();
protected:
	void			Clone(KWndList2* pCopy);
private:
	void			OnLButtonDown(int x, int y);			
	void			OnMouseMove(int x, int y);				
	
private:
	void**			m_pContent;				
	int				m_nItemSpace;			
	int				m_nNumItem;				

	int				m_nTopItemIndex;		
	int				m_nSelItemIndex;		
	int				m_nHighLightItemIndex;	

	unsigned int	m_ItemOnlineColor;			
	unsigned int	m_ItemOnlineBorderColor;		
	unsigned int	m_SelItemOnlineColor;			
	unsigned int	m_SelItemOnlineBorderColor;
	unsigned int	m_ItemOfflineColor;			
	unsigned int	m_ItemOfflineBorderColor;		
	unsigned int	m_SelItemOfflineColor;			
	unsigned int	m_SelItemOfflineBorderColor;	
	unsigned int	m_HighLightColor;		
	unsigned int	m_HighLightBorderColor;
	unsigned int	m_uSelItemBgColor;
	int				m_nFontSize;			

	KWndScrollBar  *m_pScrollbar;
};