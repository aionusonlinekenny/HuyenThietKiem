#pragma once
#include "WndWindow.h"
#include "WndButton.h"

#define WNDSCROLL_ES_VERTICAL		0x0001		
#define	WNDSCROLL_F_DRAGGING_SLIDE	0x0100		

class KWndScrollBar : public KWndImage
{
private:
	unsigned int	m_Flag;
	int			m_nMinValue;	
	int			m_nMaxValue;	
	int			m_nCurValue;	
	int			m_nPageSize;	
	int			m_nMinPosition;	
	int			m_nMaxPosition;	
	KWndButton	m_SlideBtn;		
	int			m_nImgRange;

public:
	KWndScrollBar();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();						
	void			SetSize(int nWidth, int nHeight);		

	int				ScrollLine(bool bPre);
	int				ScrollPage(bool bPre);
	void			SetScrollPos(int nPosition);
	int				GetScrollPos() { return m_nCurValue; }
	void			SetValueRange(int nMinValue, int nMaxValue);
	int				GetMaxValue() { return m_nMaxValue;}
	int				GetMinValue() { return m_nMinValue;}
	void			Clone(KWndScrollBar* pCopy);
	int				GetMinHeight();
	void			SetParentWnd(KWndWindow* pParentWnd);
private:
	void			OnLButtonDown(int x,int y);
	void			OnSlideBtnPressed();
	void			OnDraggingSlide(int x, int y);
	void			SetSlideBtnPos();
};
