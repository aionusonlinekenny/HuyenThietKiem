#pragma once
#include "WndImage.h"

#define	WNDBTN_ES_FILTER		0x00ff
#define	WNDBTN_ES_ANIMATION		0x0001		
#define	WNDBTN_ES_CHECKBOX		0x0002		
#define	WNDBTN_ES_SEND_HOLD_MSG	0x0004		
#define	WNDBTN_ES_NO_OVERSOUND	0x0008		

#define	WNDBTN_F_DOWN		0x0200			
#define	WNDBTN_F_CHECKED	WNDBTN_F_DOWN	
#define	WNDBTN_F_OVER		0x0400			

class KWndButton : public KWndImage
{
protected:
	unsigned short	m_Flag;
private:
	short			m_nUpFrame;
	short			m_nDownFrame;
	short			m_nCheckOverFrame;
	short			m_nOverStartFrame;
	short			m_nDisableFrame;
	static	KWndButton* m_pPressedDownBtn;	
	char			m_szTip[64];
	int				m_nTipLen;
public:
	KWndButton();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual const char*	GetShortKey() {return NULL;}	
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();							
	int				IsButtonChecked();						
	int				IsButtonActive();
	void			CheckButton(int bChecked);				
	void			Enable(int bEnable);					
	void			Clone(KWndButton* pCopy);
	static void		SetAllButtonTipTextColor(unsigned int uColor);	
	static void		EnableAllButtonTip(int bEnable);					
	virtual void	OnButtonClick(){}
	virtual int		GetToolTipInfo(char* szTip, int nMax);
	int		SetToolTipInfo(char* szTip, int nMax);

private:
	void			OnLBtnDown(bool bDoubleClick);
	void			OnLBtnUp();						
	void			OnLBtnDownMove();				
private:
	static unsigned int	ms_uBtnTipTextColor;	
	static int			ms_nDisableBtnTip;		
};

//===========================================================================

#include "WndText.h"
#include "WndMovingImage.h"
#include "WndImagePart.h"

class KWndImageTextButton : public KWndButton
{
protected:
	KWndMovingImage m_Image;
	KWndImagePart m_ImagePart;
	KWndText32 m_Text;
	BOOL bPart;
public:
	KWndImageTextButton();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	void			Set2IntText(int nNumber1, int nNumber2, char Separator);
	void			SetIntText(int nNumber, char Separator);
	void			SetWorldRankText(int nNumber, char Separator);
	void			Set2IntValue(int nNumber1, int nNumber2);
	void			SetText(const char* pText, int nLen = -1);
	int				GetText(char* pBuffer, int nSize);
};
