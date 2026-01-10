/*****************************************************************************************
//	界面窗口体系结构// --图形窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
// --
	显示(spr)图形的窗口。
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "UiImage.h"

#define	WNDIMG_ES_EXCLUDE_TRANS		0x0001

class KWndImage : public KWndWindow
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual int		PtInWindow(int x, int y);
	virtual void	PaintWindow();							
	void			Clone(KWndImage* pCopy);
	void	SetFrame(int nFrame);							
	int		NextFrame();									
	void	SetImage(short nType, const char* pszImgName, bool bAdjustWndSize = false);
	void	UpdateTimer();
	int		GetCurFrame();
	int		GetNumFrames();
	KWndImage();
protected:
	KUiImageRef		m_Image;	
};

class KCanGetNumImage : public KWndImage
{
public:
	int GetMaxFrame();
	int GetCurrentFrame();
};