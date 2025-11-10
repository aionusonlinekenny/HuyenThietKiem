/*****************************************************************************************
//	界面窗口体系结构// --带文字按键窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-4
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndLabeledButton.h"
#include "Wnds.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

KWndLabeledButton::KWndLabeledButton()
{
	m_Label[0]  = 0;
	m_Label[31] = 0;
	m_nLabelLen = 0;
	m_nFontSize = 16;
	m_nLabelXOffset = 0;
	m_nLabelYOffset = 0;
}

void KWndLabeledButton::Clone(KWndLabeledButton* pCopy)
{
	if (pCopy)
	{
		KWndButton::Clone(pCopy);
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_FontColor = m_FontColor;
		pCopy->m_OverColor = m_OverColor;
		pCopy->m_SelectColor = m_SelectColor;
		pCopy->m_DisableColor = m_DisableColor;
		pCopy->m_nLabelXOffset = m_nLabelXOffset;
		pCopy->m_nLabelYOffset = m_nLabelYOffset;
		pCopy->m_FontBorderColor = m_FontBorderColor;
		pCopy->m_OverBorderColor = m_OverBorderColor;
		pCopy->m_SelectBorderColor = m_SelectBorderColor;
		pCopy->m_DisableBorderColor = m_DisableBorderColor;
		strcpy(pCopy->m_Label, m_Label);
	}
}

int KWndLabeledButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 16, &m_nFontSize);
		pIniFile->GetInteger(pSection, "LabelXOffset", 0, &m_nLabelXOffset);
		pIniFile->GetInteger(pSection, "LabelYOffset", 0, &m_nLabelYOffset);
		if (m_nFontSize < 12)
			m_nFontSize = 16;

		char	Buff[32];
		pIniFile->GetString(pSection, "Color", "", Buff, sizeof(Buff));
		m_FontColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "", Buff, sizeof(Buff));
		m_FontBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverColor", "", Buff, sizeof(Buff));
		m_OverColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverBorderColor", "", Buff, sizeof(Buff));
		m_OverBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelectColor", "", Buff, sizeof(Buff));
		m_SelectColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelectBorderColor", "", Buff, sizeof(Buff));
		m_SelectBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "DisableColor", "", Buff, sizeof(Buff));
		m_DisableColor = GetColor(Buff);
		if(!Buff[0])
		{
			m_DisableColor = m_FontColor;
		}
		pIniFile->GetString(pSection, "DisableBorderColor", "", Buff, sizeof(Buff));
		m_DisableBorderColor = GetColor(Buff);
		if(!Buff[0])
		{
			m_DisableBorderColor = m_FontBorderColor;
		}
		
		pIniFile->GetString(pSection, "Label", "", Buff, sizeof(Buff));
		SetLabel(Buff);

		int nValue;
		pIniFile->GetInteger(pSection, "Enable", 1, &nValue);
		if(!nValue)
			Enable(nValue);

		return true;
	}
	return false;
}

void KWndLabeledButton::SetLabel(const char* pLabel)
{
	if (pLabel)
	{
		m_nLabelLen = strlen(pLabel);
		if (m_nLabelLen > 31)
			m_nLabelLen = 31;
		memcpy(m_Label, pLabel, m_nLabelLen);
		m_Label[m_nLabelLen] = 0;
	}
}

int KWndLabeledButton::GetLabel(char* pLabel, int nSize)
{
	int nRet = 0;
	if (pLabel)
	{		
		if (m_nLabelLen < nSize)
		{			
			memcpy(pLabel, m_Label, m_nLabelLen + 1);
			nRet = m_nLabelLen;
		}
		else
		{
			pLabel[0] = 0;
			nRet = 0;
		}
	}
	return nRet;
}

void KWndLabeledButton::SetLabelColor(unsigned int Color)
{
	m_FontColor = Color;
}

void KWndLabeledButton::PaintWindow()
{
	KWndButton::PaintWindow();
	if (g_pRepresentShell)
	{
		char	Buffer[32];
		int nMaxLen = m_Width * 2 / m_nFontSize;
		const char* pShowString = TGetLimitLenString(m_Label, -1, Buffer, nMaxLen);
		if (pShowString)
		{
			int nLen = strlen(pShowString);
			unsigned int uColor = m_FontColor;
			unsigned int uBorderColor = m_FontBorderColor;
			if (m_Flag & WNDBTN_F_OVER)
			{
				uColor = m_OverColor;
				uBorderColor = m_OverBorderColor;
			}
			if (m_Flag & WNDBTN_F_CHECKED)
			{
				uColor = m_SelectColor;
				uBorderColor = m_SelectBorderColor;
			}
			if (IsDisable())
			{	
				uColor = m_DisableColor;
				uBorderColor = m_DisableBorderColor;
			}
			g_pRepresentShell->OutputText(m_nFontSize, pShowString, nLen,
				m_nAbsoluteLeft + m_nLabelXOffset + (m_Width - nLen * m_nFontSize / 2 ) / 2,
				m_nAbsoluteTop + m_nLabelYOffset,
				uColor , 0,
				TEXT_IN_SINGLE_PLANE_COORD, uBorderColor);
		}
	}
}


int KWndRichLabeledButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		m_Label.Init(pIniFile, pSection);
		m_Label.SetPosition(m_nAbsoluteLeft, m_nAbsoluteTop);
		int nValue;
		pIniFile->GetInteger(pSection, "Enable", 1, &nValue);
		if(!nValue)
			Enable(nValue);
	}
	return false;
}

void KWndRichLabeledButton::SetLabel(const char* pLabel, int nLen)
{
	m_Label.SetText(pLabel, nLen);
}

void KWndRichLabeledButton::SetMoneyLabel(DWORD dwMoney)
{
	char szLabel[32];
	int nLen;
	if(dwMoney < 10000)
	{
		nLen = sprintf(szLabel, "%u", dwMoney);
	}
	else
	{
		int nDivisor = dwMoney / 10000;
		int nMod = dwMoney % 10000;

		if(nMod == 0)
		{
			nLen = sprintf(szLabel, "%d v筺", nDivisor);
		}
		else
		{
			nLen = sprintf(szLabel, "%d v筺 %d", nDivisor, nMod);
		}
	}
	m_Label.SetText(szLabel, nLen);
}

void KWndRichLabeledButton::SetLabelColor(unsigned int Color)
{
	m_Label.SetTextColor(Color);
}


int KWndRichLabeledButton::GetLabel(char* pLabel)
{
	int nRet = 0;
	if(pLabel)
	{		
		nRet = m_Label.GetText(pLabel, 32);
	}
	return nRet;
}


void KWndRichLabeledButton::PaintWindow()
{
	KWndButton::PaintWindow();
	m_Label.SetPosition(m_nAbsoluteLeft, m_nAbsoluteTop);
	m_Label.PaintWindow();
}
