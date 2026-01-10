#pragma once
#include "WndWindow.h"

#define	WNDLIST_ES_HALIGN_CENTRE		0x0001		
#define	WNDLIST_ES_HALIGN_RIGHT			0x0002		
#define	WNDLIST_ES_HALIGN_FILTER		(WNDLIST_ES_HALIGN_RIGHT | WNDLIST_ES_HALIGN_CENTRE)
#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0004		
#define WNDLIST_ES_MULTI_SEL			0x0008		

#define	NO_CUSTOM_COLOR					0xffffffff

class KWndScrollBar;

class KWndList : public KWndWindow
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();							
	void			SetContent(BYTE* pContent, int nNumItem, int nItemSize, int nStringOffset);
	void			GetString(int nItemIndex, char* pBuffer);
	int				GetStrignLen(int nItemIndex);		
	int				FindString(int nPrecedingStart, const char* pString);
	int				GetItemDataInt(int nItemIndex, int nDataOffset);
	void			SetTopItemIndex(int nTopItemIndex);		
	int				GetTopItemIndex() { return m_nTopItemIndex; }
	int				GetVisibleItemCount() const;			
	int				GetCount()  { return m_nNumItem; }		
	int				GetCurSel() { return m_nSelItemIndex;  }
	int				SetCurSel(int nSel);
	int				IsItemSel(int nIndex);					
	void			SetCustomColorOffset(int nOffset);
	void			EnableMultiSel(int bEnable);
	void			SetScrollbar(KWndScrollBar* pScroll);
	void			SetImage(const char* pszImage, short nWidth, short nHeight);
	KWndList();
	~KWndList();
protected:
	void			Clone(KWndList* pCopy);
private:
	void			OnLButtonDown(int x, int y);			
	void			OnRButtonDown(int x, int y);			
	void			OnLButtonDClick(int x, int y);			
	void			OnMouseMove(int x, int y);				
	int				GetIemIndexAtPoint(int x, int y);		
	void			UpdateData();
private:
	KWndScrollBar*	m_pScrollbar;			

	BYTE*			m_pContent;				
	int				m_nNumItem;				
	int				m_nItemSize;			
	int				m_nStringOffset;		
	int				m_nColorOffset;			
	int				m_nTopItemIndex;		
	int				m_nSelItemIndex;		
	int				m_nHighLightItemIndex;

	unsigned int	m_ItemColor;			
	unsigned int	m_ItemBorderColor;		
	unsigned int	m_SelItemColor;			
	unsigned int	m_SelItemBorderColor;	
	unsigned int	m_HighLightColor;		
	unsigned int	m_HighLightBorderColor;	
	unsigned int	m_uSelItemBgColor;		
	int				m_nFontSize;			
	
	int				m_nNumColumn;
	short			m_nRowSpace;
	int				m_nRowDis;

	int				m_nLastData;
	unsigned int	m_uIsItemSelStatus;		
	unsigned int*	m_puItemSelStatusList;	
	unsigned int	m_uNumSelStatusList;
	
	char			m_szImage[80];
	short			m_nImgWidth;
	short			m_nImgHeight;
	short			m_nItemTextInent;
};