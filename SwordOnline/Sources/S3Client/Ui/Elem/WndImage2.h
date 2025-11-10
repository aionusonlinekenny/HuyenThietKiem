#pragma once
#include "WndWindow.h"
#include "UiImage.h"

#define	WNDIMG_ES_EXCLUDE_TRANS		0x0001

class KWndImage2 : public KWndWindow
{
private:
	char			m_szTip[128];
	short			m_nTipLen;
	int				m_nData[2];

public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual int		PtInWindow(int x, int y);
	virtual void	PaintWindow();							
	void			Clone(KWndImage2* pCopy);
	void	SetFrame(int nFrame);							
	int		NextFrame();									
	void	SetImage(short nType, const char* pszImgName, bool bAdjustWndSize = false);
	void	UpdateTimer();
	int		GetCurFrame();
	int		GetNumFrames();
	int		SetToolTipInfo(char* szTip, int nMax);
	void	SetData(int nData, int nNo);
	int		GetData(int nNo) const { return m_nData[nNo]; };
	KWndImage2();
protected:
	KUiImageRef		m_Image;	
};