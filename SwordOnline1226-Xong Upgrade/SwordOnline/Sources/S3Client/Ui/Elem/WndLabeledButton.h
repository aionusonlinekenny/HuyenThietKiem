/*****************************************************************************************
//	界面窗口体系结构// --带文字按键窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-4
// --
	带文字按钮窗口。
*****************************************************************************************/
#pragma once
#include "WndButton.h"
#include "WndText.h"

//===========================================
//	简单的带文字按钮窗口。
//	文字单行，居中处理，文字中不包含控制符。
//===========================================
class KWndLabeledButton : public KWndButton
{
public:
	KWndLabeledButton();
	void			SetLabel(const char* pLabel);
	int				GetLabel(char* pLabel, int nSize);
	void			SetLabelColor(unsigned int Color);	
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	
	virtual void	PaintWindow();									
	void			Clone(KWndLabeledButton* pCopy);
private:
	char	m_Label[32];		
	int		m_nLabelLen;		
	int		m_nFontSize;
	int		m_nLabelXOffset;
	int		m_nLabelYOffset;	
	unsigned int m_FontColor;	
	unsigned int m_FontBorderColor;	
	unsigned int m_OverColor;	
	unsigned int m_OverBorderColor;
	unsigned int m_SelectColor;
	unsigned int m_SelectBorderColor;
	unsigned int m_DisableColor;
	unsigned int m_DisableBorderColor;
};

//=====================================================
//
//=====================================================
class KWndRichLabeledButton : public KWndButton
{
public:
	void			SetLabel(const char* pLabel, int nLen);
	void			SetMoneyLabel(DWORD dwMoney);
	void			SetLabelColor(unsigned int Color);
	int				GetLabel(char* pLabel);
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual void	PaintWindow();						
private:
	KWndText256		m_Label;
};