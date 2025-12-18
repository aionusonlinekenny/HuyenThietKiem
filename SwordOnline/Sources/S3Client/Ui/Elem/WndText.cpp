#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndText.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"

extern iRepresentShell*	g_pRepresentShell;

KWndText::KWndText()
{
	m_nFontSize = 12;
	m_pText = NULL;
	m_nTextLen   = 0;
	m_nBuffLen   = 0;
	m_nTopLine   = 0;
	m_nLineCount = 0;
	m_BorderColor = 0;
}

void KWndText::SetText(const char* pText, int nLen/*= -1*/)
{
	if (m_pText && g_pRepresentShell)
	{
		if (pText && pText[0])
		{
			if (nLen < 0)
				nLen = strlen(pText);
			if (nLen > m_nBuffLen)
				nLen = TSplitString(pText, m_nBuffLen - 1, true);
			memcpy(m_pText, pText, nLen);
			m_pText[nLen] = 0;
			m_nTextLen = nLen;
			if (m_Style & WNDTEXT_ES_MULTILINE)
			{
				int nLineLen;
				m_nLineCount = TGetEncodedTextLineCount(
					m_pText, m_nTextLen, (m_Width * 2) / m_nFontSize, nLineLen, m_nFontSize);
			}
		}
		else
		{
			m_pText[0] = 0;
			m_nTextLen = 0;
			m_nLineCount = 0;
		}
		m_nTopLine = 0;
	}
}

int KWndText::GetText(char* pBuffer, int nSize)
{
	int nRet = 0;
	if (m_pText && pBuffer)
	{
		if (m_nTextLen + 1 <= nSize)
		{
			memcpy(pBuffer, m_pText, m_nTextLen + 1);
			nRet = m_nTextLen;
		}
		else
			nRet = m_nTextLen + 1;
	}
	return nRet;
}

int	KWndText::SetTopLine(int nTopLine)
{
	if (nTopLine >= 0 && nTopLine < m_nLineCount)
		m_nTopLine = nTopLine;
	return 0;
}

void KWndText::SetTextPtr(char* pText, int nMaxLen)
{	
	m_pText = pText;
	m_nBuffLen = nMaxLen;
	m_nTextLen = 0;
	m_nTopLine = 0;
	m_nLineCount = 0;
}


void KWndText::Clone(KWndText* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_TextColor = m_TextColor;
		pCopy->m_BorderColor = m_BorderColor;
	}
}


int KWndText::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 12, &m_nFontSize);
		m_Style &= ~WNDTEXT_ES_ALIGN_FILTER;
		int	nValue;
		pIniFile->GetInteger(pSection, "HAlign", 0, &nValue);		
		if (nValue == 1)
			m_Style |= WNDTEXT_ES_HALIGN_CENTRE;
		else if (nValue == 2)
			m_Style |= WNDTEXT_ES_HALIGN_RIGHT;
		pIniFile->GetInteger(pSection, "VAlign", 0, &nValue);
		if (nValue == 1)
			m_Style |= WNDTEXT_ES_VALIGN_CENTRE;	
		else if (nValue == 2)
			m_Style |= WNDTEXT_ES_VALIGN_BOTTOM;

		char	Buff[256];
		pIniFile->GetString(pSection, "Color", "0,0,0", Buff, 16);
		m_TextColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "0,0,0", Buff, 16);
		m_BorderColor = GetColor(Buff);
		if (m_pText && m_pText[0] == 0)
		{
			if (pIniFile->GetString(pSection, "Text", "", Buff, 256))
			{
				strncpy(m_pText, Buff, m_nBuffLen - 1);
				m_pText[m_nBuffLen - 1] = 0;
				m_nTextLen = strlen(m_pText);
			}
		}
		pIniFile->GetInteger(pSection, "MultiLine", 0, &nValue);
		if (nValue)
		{
			m_Style |= WNDTEXT_ES_MULTILINE;
			if (g_pRepresentShell)
			{
				int nLineLen;
				m_nLineCount = TGetEncodedTextLineCount(
					m_pText, m_nTextLen, (m_Width * 2) / m_nFontSize, nLineLen, m_nFontSize);
			}
			else
				m_nLineCount = 0;
			if (m_nTopLine >= m_nLineCount)
				m_nTopLine = 0;
		}
		else
		{
			m_Style &= ~ WNDTEXT_ES_MULTILINE;
			m_nLineCount = 0;
			m_nTopLine = 0;
		}

		return true;
	}
	return false;
}

void KWndText::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (m_pText == NULL || m_pText[0] == 0 || g_pRepresentShell == NULL)
		return;
	KOutputTextParam	Param;
	Param.nX = m_nAbsoluteLeft;
	Param.nY = m_nAbsoluteTop;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;

	if ((m_Style & WNDTEXT_ES_MULTILINE) == 0)
	{
		switch(m_Style & WNDTEXT_ES_HALIGN_FILTER)
		{
		case WNDTEXT_ES_HALIGN_CENTRE:
			Param.nX += (m_Width - m_nTextLen * m_nFontSize / 2) / 2;
			if (Param.nX < m_nAbsoluteLeft)
				Param.nX = m_nAbsoluteLeft;
			break;
		case WNDTEXT_ES_HALIGN_RIGHT:
			Param.nX += m_Width - m_nTextLen * m_nFontSize / 2;
			if (Param.nX < m_nAbsoluteLeft)
				Param.nX = m_nAbsoluteLeft;
			break;
		}
		switch(m_Style & WNDTEXT_ES_VALIGN_FILTER)
		{
		case WNDTEXT_ES_VALIGN_CENTRE:
			Param.nY += (m_Height - m_nFontSize - 1) / 2;
			break;
		case WNDTEXT_ES_VALIGN_BOTTOM:
			Param.nY += m_Height - m_nFontSize - 1;
			break;
		}
		Param.nNumLine = 1;
	}
	else		
		Param.nNumLine = m_Height / (m_nFontSize + 1);

	Param.nSkipLine = m_nTopLine;
	Param.Color = m_TextColor;
	Param.BorderColor = m_BorderColor;
	g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_pText, m_nTextLen, m_Width);
}

void KWndText::SetTextColor(unsigned int Color)
{
	m_TextColor = Color;
}

void KWndText::SetIntText(int nNumber, char Separator)
{
	char	Buff[16];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::Set2IntText(int nNumber1, int nNumber2, char Separator)
{
	char	Buff[32];
	int		nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "%d%c%d", nNumber1, Separator, nNumber2);
		SetText(Buff, nLen);
	}
}

void KWndText::SetCharText(char nNumber, char Separator)
{
	char	Buff[16];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::SetByteText(BYTE nNumber, char Separator)
{
	char	Buff[16];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::SetShortText(short nNumber, char Separator)
{
	char	Buff[16];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::Set2ShortText(short nNumber1, short nNumber2, char Separator)
{
	char	Buff[32];
	int		nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "%d%c%d", nNumber1, Separator, nNumber2);
		SetText(Buff, nLen);
	}
}

void KWndText::SetMoneyText(int nNumber, char Separator)
{
	char	Buff[128];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
		{
			if(nNumber < 10000)
			{	
				if(nNumber < 0)
					nNumber = 0;
				nLen = sprintf(Buff, "%d l­îng", nNumber);
			}
			else 
			{	
				int nDivisor = nNumber / 10000;
				int nMod = nNumber % 10000;

				if(nMod == 0)
				{
					nLen = sprintf(Buff, "%d v¹n l­îng ", nDivisor);
				}
				else
				{
					nLen = sprintf(Buff, "%d v¹n %d l­îng ", nDivisor, nMod);
				}
			}
		}
		SetText(Buff, nLen);
	}
}

void KWndText::SetJbCoinText(int nNumber, char Separator)
{
	char	Buff[64];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
		{
			if(nNumber < 10000)
			{	
				if(nNumber < 0)
					nNumber = 0;
				nLen = sprintf(Buff, "%d xu", nNumber);
			}
			else 
			{	
				int nDivisor = nNumber / 10000;
				int nMod = nNumber % 10000;

				if(nMod == 0)
				{
					nLen = sprintf(Buff, "%d v¹n xu", nDivisor);
				}
				else
				{
					nLen = sprintf(Buff, "%d v¹n %d xu", nDivisor, nMod);
				}
			}
		}
		SetText(Buff, nLen);
	}
}

void KWndText::SetWorldRankText(int nNumber, char Separator)
{
	char	Buff[16];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
		{
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		}
		else
		{	
			if(nNumber <= 0)
			{
				nLen = sprintf(Buff, "?");
			}
			else if(nNumber >= 5000)
			{
				nLen = sprintf(Buff, ">5000");
			}
			else
			{
				nLen = sprintf(Buff, "%d", nNumber);
			}
		}
		SetText(Buff, nLen);
	}
}

void KWndText::SetTransLifeText(char nNumber, char Separator)
{
	char	Buff[32];
	int		nLen;
	if (m_pText)
	{
		if (Separator)
		{
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		}
		else
		{	
			if(nNumber <= 0)
			{
				nLen = sprintf(Buff, "0");
			}
			else
			{
				nLen = sprintf(Buff, "%d", nNumber);
			}
		}
		SetText(Buff, nLen);
	}
}

void KWndText::Set2UIntText(UINT nNumber1, UINT nNumber2, char Separator)
{
	char	Buff[64];
	int		nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "%u%c%u", nNumber1, Separator, nNumber2);
		SetText(Buff, nLen);
	}
}

void KWndText::SetStackText(int nNumber, char Separator)
{
	char	Buff[10];
	BYTE	nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d c¸i", nNumber);
			
		SetText(Buff, nLen);
	}
}

void KWndText::SetDefText(int nNumber, bool bEx /*= false*/)
{
	char	Buff[10];
	BYTE	nLen;
	if (m_pText)
	{
		if (bEx)
		{
			if(nNumber > 75)
				nNumber = nNumber - 75;
			else
				nNumber = 0;
			if(nNumber == 0)
			{
				nLen = sprintf(Buff, "");
			}
			else
			{
				nLen = sprintf(Buff, "+%d%%", nNumber);
			}
		}
		else
		{	
			if(nNumber > 75)
				nNumber = 75;
			nLen = sprintf(Buff, "%d%%", nNumber);
		}
			
		SetText(Buff, nLen);
	}
}

void KWndText::Set2IntTextPlus(int nNumber1, int nNumber2, char Separator)
{
	
	if(m_pText)
	{	
		char	Buff[32];
		int		nLen;
		if( (nNumber1 < 0) && (nNumber2 >= 0 ))
		{
			nLen = sprintf(Buff, "%s%c%d", "// --", Separator, nNumber2);
		}
		else if( (nNumber1 >= 0) && (nNumber2 < 0 ))
		{
			nLen = sprintf(Buff, "%d%c%s", nNumber1, Separator, "// --");
		}
		else if( (nNumber1 < 0) && (nNumber2 < 0 ))
		{
			nLen = sprintf(Buff, "%s%c%s", "// --", Separator, "// --");
		}
		else
		{
			nLen = sprintf(Buff, "%d%c%d", nNumber1, Separator, nNumber2);
		}

		SetText(Buff, nLen);
	}
}

//AutoAI by kinnox;
void KWndText::Set3IntText(int nNumber1, int nNumber2, int nNumber3, char Separator, char Separator2)
{
	char	Buff[32];
	int		nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "[IDMap:%d]%c%d%c%d", nNumber1, Separator, nNumber2, Separator2, nNumber3);
		SetText(Buff, nLen);
	}
}
//end code;

KWndText32::KWndText32()
{
	m_Text[0] = m_Text[31] = 0;
	SetTextPtr(m_Text, 31);
}


KWndText80::KWndText80()
{
	m_Text[0] = m_Text[79] = 0;
	SetTextPtr(m_Text, 79);
}


KWndText128::KWndText128()
{
	m_Text[0] = m_Text[127] = 0;
	SetTextPtr(m_Text, 127);
}


KWndText256::KWndText256()
{
	m_Text[0] = m_Text[255] = 0;
	SetTextPtr(m_Text, 255);
}

	

KWndText512::KWndText512()
{
	m_Text[0] = m_Text[511] = 0;
	SetTextPtr(m_Text, 511);
}
