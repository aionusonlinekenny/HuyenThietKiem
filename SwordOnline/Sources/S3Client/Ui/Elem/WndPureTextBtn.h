#pragma once
#include "WndWindow.h"
#include "../../../Core/src/GameDataDef.h"

#define	WNDPTB_MAX_TEXT_LEN	64

#define	WNDPTB_S_CENTRE_ALIGN	0x00008000

#define	WNDPTB_F_BE_PRESSEDDOWN	0x00000001
#define	WNDPTB_F_OVER			0x00000002
#define	WNDPTB_F_CHECK			0x00000004

class KWndPureTextBtn : public KWndWindow
{
public:
	KWndPureTextBtn();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual void	PaintWindow();								
	void			SetText(const char* pText, int nLen = -1);
	void			SetColor(unsigned int nNormalColor, unsigned int nOverColor, unsigned int nPressedColor, 
								unsigned int nNormalBorderColor, unsigned int nOverBorderColor, unsigned int nPressedBorderColor);
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void			CheckButton(int bChecked);
	void			Clone(KWndPureTextBtn* pCopy);
private:
	void			AdjustPosition();
	void			GenChatItem(char* pszInfo);
private:
	unsigned int	m_Flag;
	char			m_sText[WNDPTB_MAX_TEXT_LEN];
	int				m_nTextLen;
	unsigned int	m_NormalColor;
	unsigned int	m_OverColor;
	unsigned int	m_PressedColor;
	unsigned int	m_NormalBorderColor;
	unsigned int	m_OverBorderColor;
	unsigned int	m_PressedBorderColor;
	int				m_nFontSize;

public:
	char			m_szItemInfo[MAX_CHATITEM_INFO_LENGTH];
};
