/*****************************************************************************************
//	界面窗口体系结构// --列表窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-11
// --
    列表窗口只列出文字。但是每项都可以关联其他数据。
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndScrollBar.h"

#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0004
#define	WNDLIST_ERROR					-1

class KWndListFilterItem : public KWndWindow
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();							
	int				AddString(int nIndex, const char* pString1, int nOption, int nValue, BOOL bHighLight = TRUE);
	int				GetString(int nIndex, char* pBuffer1, unsigned int nSize);
	int				DeleteString(int nIndex);				
	void			ResetContent();							
	int				FindOptionIndex(int pOption);
	int				SetItemValue(int nIndex, int nValue);	
	int				GetItemValue(int nIndex);
	int				GetItemOption(int nIndex);
	void			SetTopItemIndex(int nTopItemIndex);	
	int				GetVisibleLineCount() const;			
	int				GetCount()  { return m_nNumItem; }		
	int				GetCurSel() { return m_nSelItemIndex;  }
	int				SetCurSel(int nSel);
	void			SetScrollbar(KWndScrollBar* pScroll);
	KWndListFilterItem();
	virtual ~KWndListFilterItem();
protected:
	void			Clone(KWndListFilterItem* pCopy);
private:
	void			OnLButtonDown(int x, int y);			//响应鼠标左键按下
	void			OnMouseMove(int x, int y);				//响应鼠标在列表窗口上移动
	
private:
	void**			m_pContent;				//列表项内容数据
	int				m_nItemSpace;			//可以保存的item的数目
	int				m_nNumItem;				//数据项的数目

	int				m_nTopItemIndex;
	int				m_nSelItemIndex;	
	int				m_nHighLightItemIndex;

	unsigned int	m_ItemColor;			//显示数据项文本的颜色
	unsigned int	m_ItemBorderColor;		//显示数据项文本的边缘颜色
	unsigned int	m_SelItemColor;			//被选中的数据项文本的颜色
	unsigned int	m_SelItemBorderColor;	//被选中的数据项文本的边缘颜色
	unsigned int	m_HighLightColor;		//高亮度显示的数据项文本的颜色
	unsigned int	m_HighLightBorderColor;	//高亮度显示的数据项文本的边缘颜色
	int				m_nFontSize;			//显示字体大小

	KWndScrollBar  *m_pScrollbar;
};