#pragma once

#include "WndWindow.h"
#include "WndScrollbar.h"
#include "../../../Core/src/GameDataDef.h"

struct KOneMsgInfo
{
	int			 nLines;				
	int			 nCharWidth;
	char		 szName[80];
	char		 szInfo[MAX_CHATITEM_INFO_LENGTH];
	int			 nPos;					
	int			 nFace;				
	int			 nLinePos;
	unsigned int uTextBKColor;		
	int			 nMaxSize;				
	int			 nLen;					
	char		 Msg[1];				
};

class KWndScrollBar;

class KWndMessageListBox : public KWndWindow
{
public:
	KWndMessageListBox();
	virtual ~KWndMessageListBox();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);

	void	SetFirstShowLine(int nLine);	
	int		GetFirstShowLine();				
	void	SetScrollbar(KWndScrollBar* pScroll);

	int		SetCapability(int nNumMessage);
	int		GetCapability(){ return m_nCapability;}
	int		GetCurSel() { return m_nSelMsgIndex; }
	int		SetCurSel(int nIndex);

	int		AddOneMessage(const char* pText, int nLen, const char* pszName = NULL, const char* pszInfo = NULL, 
							int nPos = 0, int nLinePos = 1, int nFace = 0, unsigned int uTextBKColor = 0);
	int		GetOneMessage(int nIndex, char* pBuffer, int nLen, bool bExcludeCtrl);
	void	RemoveAMessage(int nIndex);
	int		GetMsgCount() { return m_nNumMessage; }
	int		HitTextAtPoint(int x, int y);

	void	SetSize(int nWidth, int nHeight);
	void	Clear();
	void	Clone(KWndMessageListBox* pCopy);
	int		GetMaxShowLine() {return m_nNumMaxShowLine;}
	int		GetItemLineCount(int nIndex);

	unsigned int	SplitData();
	unsigned int	BindData(unsigned int	hData);
	static void		FreeData(unsigned int	hData);

	virtual int		PtInWindow(int x, int y);

	void ClearHideLine();
	void HideNextLine();
	void HideAllLine();
	
	int	GetMinHeight();
	
	void SetFontSize(int nFontSize);
	int GetFontSize()	{return m_nFontSize;}
	void	UpdateChatItem();
	int		GetMaxLineWidth() { return m_nNumBytesPerLine;}
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateData();					
	virtual void	PaintWindow();			
	void	OnMouseMove(int x, int y);
	void	OnLButtonDown(int x, int y);
	void	OnLButtonDClick(int x, int y);
	int		GetMsgAtPoint(int x, int y);

private:
	struct KMessageListData
	{
		KOneMsgInfo**	pMessages;			
		int				nNumMessage;		
		int				nCapability;		
		int				nStartShowMsg;		
		int				nStartMsgSkipLine;	
		int				nSelMsgIndex;		
	};
	
private:
	KWndScrollBar*	m_pScrollbar;			
	KOneMsgInfo**	m_pMessages;			

	int				m_nNumMessage;			
	int				m_nCapability;			

	int				m_nNumMaxShowLine;		
	int				m_nNumBytesPerLine;		
	int				m_nNumVisibleTextLine;	
	
	int				m_nHideNumLine;

	int				m_nFontSize;			
	int				m_nStartShowMsg;		
	int				m_nStartMsgSkipLine;	

	int				m_nSelMsgIndex;			
	int				m_nHLMsgIndex;			
	unsigned int	m_MsgColor;				
	unsigned int	m_MsgBorderColor;		
	unsigned int	m_SelMsgColor;			
	unsigned int	m_SelMsgBorderColor;	
		
	unsigned int	m_SelMsgBgColor;		
	unsigned int	m_HLMsgColor;			
	unsigned int	m_HLMsgBorderColor;		
	unsigned int	m_uTextLineShadowColor;	

	BOOL m_bHitText;
	unsigned int	GetOffsetTextHeight();

public:
	bool			m_bItemActived;
};

class KScrollMessageListBox : public KWndWindow
{
public:
	KScrollMessageListBox();

	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	
	KWndMessageListBox* GetMessageListBox()
	{
		return &m_MsgList;
	}
	KWndScrollBar* GetScrollBar()
	{
		return &m_Scroll;
	}
	int		HeightToLineHeight(int nHeight);
	int		WidthToCharWidth(int nWidth);
	int		GetMinHeight();
protected:
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_Scroll;
	int m_nLineHeight;
	int m_nMinLineCount;
	int	m_nMaxLineCount;
	int m_nCurrentLineCount;

	void	SetMsgLineCount(int nCount);
	void	InitMinMaxLineCount(int nMin, int nMax);
};

bool	MsgListBox_LoadContent(KWndMessageListBox* pBox, KIniFile* pFile, const char* pszSection);