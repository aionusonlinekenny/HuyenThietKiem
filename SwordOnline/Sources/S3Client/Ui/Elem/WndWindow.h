#pragma once

#define WND_S_VISIBLE				0x80000000	
#define WND_S_DISABLED				0x40000000	
#define	WND_S_TOPMOST				0x20000000	
#define	WND_S_MOVEALBE				0x10000000	
#define	WND_S_DISABLE				0x04000000	
#define	WND_S_TOBEDESTROY			0x02000000
#define	WND_S_MOVE_WITH_R_EDGE		0x01000000
#define	WND_S_MOVE_WITH_B_EDGE		0x00800000
#define	WND_S_SIZE_WITH_R_EDGE		0x00400000
#define	WND_S_SIZE_WITH_B_EDGE		0x00200000
#define	WND_S_SIZE_WITH_ALL_CHILD	0x00100000

extern	int WND_SHOW_DEBUG_FRAME_TEXT;

class KIniFile;

class KWndWindow
{
protected:
	unsigned int m_Style;
	int		m_Left;	
	int		m_Top;
	int		m_Width;			
	int		m_Height;			
	int		m_nAbsoluteLeft;	
	int		m_nAbsoluteTop;		
#ifdef _DEBUG
	char		m_Caption[32];
#endif

	bool		m_bMoving;
	short		m_nLastMouseHoldPosX;
	short		m_nLastMouseHoldPosY;

	KWndWindow*	m_pPreviousWnd;		
	KWndWindow*	m_pNextWnd;			
	KWndWindow*	m_pFirstChild;		
	KWndWindow*	m_pParentWnd;		

public:
	KWndWindow();
	virtual	~KWndWindow();

	void	Destroy();
	virtual void UpdateData(){}

	virtual int		CloseWindow(bool bDestory){return 0;};
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual int		PtInWindow(int x, int y);				

	virtual void	PaintWindow();							
	void			PaintDebugInfo();

	virtual void	SetSize(int nWidth, int nHeight);

	void			BringToTop();							
	void			GetPosition(int* pLeft, int* pTop);		
	void			SetPosition(int nLeft, int nTop);		
	void			GetAbsolutePos(int* pLeft, int* pTop);	
	void			GetSize(int* pWidth, int* pHeight);		
	void			SetCursorAbove();						
	void			GetAllChildLayoutRect(RECT* pRect);		

	virtual void	Show();									
	virtual void	Hide();									
	int				IsVisible();							
	virtual	void	Enable(int bEnable);									
	void			AddChild(KWndWindow* pChild);			
	void			AddBrother(KWndWindow* pBrother);		

	virtual KWndWindow*	TopChildFromPoint(int x, int y);	

	KWndWindow*		GetPreWnd() const { return m_pPreviousWnd; }
	KWndWindow*		GetNextWnd() const { return m_pNextWnd; }		
	KWndWindow*		GetParent() const { return m_pParentWnd; }		
	KWndWindow*		GetFirstChild() const { return m_pFirstChild; }	
	KWndWindow*		GetOwner();								

	void			Paint();								
	void			LetMeBreathe();							

	void			SplitSmaleFamily();						
	void			LeaveAlone();							

#ifdef _DEBUG
	void			SetCaption(char* pszCaption);
#endif
	int				IsDisable() { return (m_Style & WND_S_DISABLE); }
	int				GetStyle() { return m_Style; }
	int				SetStyle(unsigned int nStyle)
	{
		m_Style = nStyle;
		return m_Style; 
	}
	
protected:
	void			Clone(KWndWindow* pCopy);

private:
	virtual void	Breathe() {}							
	void			OnLBtnDown(int x, int y);				
	void			OnMoveWnd();							

private:
	void			AbsoluteMove(int dx, int dy);			
};

unsigned int	GetColor(const char* pString);
const char* GetColorString(unsigned int nColor);
