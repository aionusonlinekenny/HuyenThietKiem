#pragma once

#include "WndMessage.h"
#include "UiImage.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../core/src/GameDataDef.h"

class KMouseOver
{
public:
	KMouseOver();
	int		IsMoseHoverWndObj(void* pWnd, int nObj);
	void    SetMouseHoverInfo(void* pWnd, int nObj, int x, int y,
		                        bool bHeadTailImg, bool bFollowCursor);
	void    SetMouseHoverImage(bool bAdd);//them
    void    SetMouseHoverTitle(const char *pcszTitleText, int nTitleTextLen, UINT uColor);
	void    SetMouseHoverProp(const char *pcszPropText, int nPropTextLen, UINT uColor);
	void    SetMouseHoverDesc(const char *pcszDescText, int nDescTextLen, UINT uColor);
	void    SetMouseHoverIcon(const char *pcszIcon);
	void	CancelMouseHoverInfo();
	void	PaintMouseHoverInfo();
	void	LoadScheme(const char* pScheme);
	void	UpdateCursorPos(int nX, int nY);
	void	OnWndClosed(void* pWnd);
private:
	void	Update(int nX, int nY);
	void	Initialize();
private:
	short	m_nImgWidth;	        
	short	m_nImgHeight;	        
	int	m_nLeft;		        
	int	m_nTop;			        
	int	m_nWndWidth;	        
	int	m_nWndHeight;	        

	int		m_nIndent;		        
	int		m_nFontSize;

	int     m_nApplyX;              
	int     m_nApplyY;              
	int     m_nTitleLineNum;
	int     m_nPropLineNum;
	int     m_nDescLineNum;
	int     m_nMaxLineLen;

	unsigned int m_uTitleBgColor;
	unsigned int m_uTitleColor;
	unsigned int m_uPropBgColor;
	unsigned int m_uPropColor;
	unsigned int m_uDescBgColor;
	unsigned int m_uDescColor;
	unsigned int m_uPicBgColor;
	unsigned int m_uRectBgColor;
	KRUImagePart m_Image;
	KRUImage	 m_IconImage;
	WORD		 m_wNumFrames;
	char		 m_cLoopFrameTime;
	void*	m_pMouseHoverWnd;
	int		m_nObj;
	char    m_ObjTitle[GOD_MAX_OBJ_TITLE_LEN];
	char    m_ObjProp[GOD_MAX_OBJ_PROP_LEN];
	char	m_ObjDesc[GOD_MAX_OBJ_DESC_LEN];
	int		m_nTitleLen;
	int		m_nPropLen;
	int		m_nDescLen;
	bool	m_bHeadTailImg;
	bool	m_bFollowCursor;
	bool	m_bTempImg;
	BOOL	m_bShow;
	int		m_nWndWidthReduce;
	int		m_nWndHeightReduce;	 
	int		m_nWidthReduce;
	int		m_nHeightReduce;
	int		m_nMaxWidthReduce;
	int		m_nMaxHeightReduce;
public:
	KUiImageRef	 m_HoverImage;
};

extern KMouseOver	g_MouseOver;

void SetHoverObjDescColor(unsigned int uColor);
void SetMouseHoverObjectDesc(void* pWnd, int nObj, unsigned int uGenre,
			unsigned int uId, int nContainer, int x, int y);
int DrawDraggingGameObjFunc(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult);
