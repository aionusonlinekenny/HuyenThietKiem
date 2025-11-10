#pragma once
#include "WndWindow.h"

#define	WNDTEXT_ES_HALIGN_CENTRE	0x0001		
#define	WNDTEXT_ES_HALIGN_RIGHT		0x0002		
#define	WNDTEXT_ES_VALIGN_CENTRE	0x0004		
#define	WNDTEXT_ES_VALIGN_BOTTOM	0x0008		
#define	WNDTEXT_ES_HALIGN_FILTER	(WNDTEXT_ES_HALIGN_RIGHT | WNDTEXT_ES_HALIGN_CENTRE)
#define	WNDTEXT_ES_VALIGN_FILTER	(WNDTEXT_ES_VALIGN_BOTTOM | WNDTEXT_ES_VALIGN_CENTRE)
#define	WNDTEXT_ES_ALIGN_FILTER		(WNDTEXT_ES_HALIGN_FILTER | WNDTEXT_ES_VALIGN_FILTER)

#define	WNDTEXT_ES_MULTILINE		0x0010


//=================================
//		
//=================================
class KWndText : public KWndWindow
{
public:
	KWndText();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual void	PaintWindow();							
	int				GetText(char* pBuffer, int nSize);		
	void			SetText(const char* pText, int nLen = -1);	
	void			SetTextColor(unsigned int Color);		
	void			SetIntText(int nNumber, char Separator = 0);				
	void			Set2IntText(int nNumber1, int nNumber2, char Separator);
	void			SetCharText(char nNumber, char Separator = 0);	
	void			SetByteText(BYTE nNumber, char Separator = 0);	
	void			SetShortText(short nNumber, char Separator = 0);	
	void			Set2ShortText(short nNumber1, short nNumber2, char Separator);
	void			SetMoneyText(int nNumber, char Separator = 0);		
	void			SetJbCoinText(int nNumber, char Separator = 0);
	void			SetWorldRankText(int nNumber, char Separator = 0);
	void			SetTransLifeText(char nNumber, char Separator = 0);
	void			Set2UIntText(UINT nNumber1, UINT nNumber2, char Separator);
	void			SetStackText(int nNumber, char Separator = 0);
	void			SetDefText(int nNumber, bool bEx = false);
	void			Set2IntTextPlus(int nNumber1, int nNumber2, char Separator);
	void			Set3IntText(int nNumber1, int nNumber2, int nNumber3, char Separator, char Separator2);//AutoAI by kinnox;
	int				SetTopLine(int nTopLine);
	int				GetLineCount() { return m_nLineCount; }
	void			Clone(KWndText* pCopy);
protected:
	void			SetTextPtr(char* pText, int nBuffLen);

private:
	int				m_nFontSize;		
	unsigned int	m_TextColor;		
	unsigned int	m_BorderColor;		

	char*			m_pText;			
	int				m_nBuffLen;			
	int				m_nTextLen;				
	int				m_nTopLine;			
	int				m_nLineCount;		
};

//=================================
//	
//=================================
class KWndText32 : public KWndText
{
private:
	char			m_Text[32];
public:
	KWndText32();
};

//=================================
//	
//=================================
class KWndText80 : public KWndText
{
private:
	char			m_Text[80];
public:
	KWndText80();
};

//=================================
//	
//=================================
class KWndText128 : public KWndText
{
private:
	char			m_Text[128];
public:
	KWndText128();
};

//=================================
//	
//=================================
class KWndText256 : public KWndText
{
private:
	char			m_Text[256];
public:
	KWndText256();
};

//=================================
//	
//=================================
class KWndText512 : public KWndText
{
private:
	char			m_Text[512];
public:
	KWndText512();
};

