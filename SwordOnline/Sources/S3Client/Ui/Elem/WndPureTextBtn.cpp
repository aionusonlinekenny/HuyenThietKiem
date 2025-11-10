#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndPureTextBtn.h"
#include "../UiCase/UiChatItemInfo.h"
#include "Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

// --
//
// --
KWndPureTextBtn::KWndPureTextBtn()
{
	m_Flag = 0;
	m_sText[0] = 0;
	m_nTextLen = 0;
	m_nFontSize = 12;
	m_NormalColor = m_OverColor = m_PressedColor = 0;
	m_NormalBorderColor = m_OverBorderColor = m_PressedBorderColor = 0;
	memset(m_szItemInfo, 0, sizeof(m_szItemInfo));
}

// --
//
// --
void KWndPureTextBtn::Clone(KWndPureTextBtn* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_NormalColor = m_NormalColor;
		pCopy->m_OverColor = m_OverColor;
		pCopy->m_PressedColor = m_PressedColor;
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_NormalBorderColor = m_NormalBorderColor;
		pCopy->m_OverBorderColor = m_OverBorderColor;
		pCopy->m_PressedBorderColor = m_PressedBorderColor;
	}
}

// --
//
// --
int KWndPureTextBtn::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 12, &m_nFontSize);
		if (m_nFontSize < 8)
			m_nFontSize = 8;
		int	nValue;
		pIniFile->GetInteger(pSection, "CentreAlign", 1, &nValue);
		if (nValue)
			m_Style |= WNDPTB_S_CENTRE_ALIGN;
		else
			m_Style &= ~WNDPTB_S_CENTRE_ALIGN;
		
		char	Buff[16];
		pIniFile->GetString(pSection, "Color", "", Buff, sizeof(Buff));
		m_NormalColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "0,0,0", Buff, 16);
		m_NormalBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverColor", "", Buff, sizeof(Buff));
		m_OverColor = GetColor(Buff);
		pIniFile->GetString(pSection, "OverBorderColor", "0,0,0", Buff, 16);
		m_OverBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelColor", "", Buff, sizeof(Buff));
		m_PressedColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SelBorderColor", "0,0,0", Buff, 16);
		m_PressedBorderColor = GetColor(Buff);
		pIniFile->GetString(pSection, "Text", "", Buff, 16);
		SetText(Buff);
	}
	return false;
}

// --
//
// --
void KWndPureTextBtn::AdjustPosition()
{
	int nTextWidth = m_nTextLen * m_nFontSize / 2;
	if (m_Style & WNDPTB_S_CENTRE_ALIGN)
		SetPosition(m_Left + (m_Width - nTextWidth) / 2, m_Top);
	SetSize(nTextWidth, m_Height);

}

// --
//
// --
void KWndPureTextBtn::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (m_sText == NULL || m_sText[0] == 0 || g_pRepresentShell == NULL)
		return;
	KOutputTextParam param;
	if (m_Flag & WNDPTB_F_BE_PRESSEDDOWN ||
		m_Flag & WNDPTB_F_CHECK)
	{
		param.Color = m_PressedColor;
		param.BorderColor = m_PressedBorderColor;
	}
	else if (m_Flag & WNDPTB_F_OVER)
	{
		param.Color = m_OverColor;
		param.BorderColor = m_OverBorderColor;
	}
	else
	{
		param.Color = m_NormalColor;
		param.BorderColor = m_NormalBorderColor;
	}
	param.nX = m_nAbsoluteLeft + (m_Width - m_nTextLen * m_nFontSize / 2) / 2;
	param.nY = m_nAbsoluteTop;
	param.nZ = TEXT_IN_SINGLE_PLANE_COORD;
	param.nSkipLine = 0;
	param.nNumLine = 100;
	g_pRepresentShell->OutputRichText(m_nFontSize, &param, m_sText, m_nTextLen);
}

// --
//
// --
void KWndPureTextBtn::SetText(const char* pText, int nLen)
{
	if (pText == NULL)
		nLen = 0;
	else if (nLen < 0)
		nLen = strlen(pText);
	if (nLen <= WNDPTB_MAX_TEXT_LEN)
		m_nTextLen = nLen;
	else
		m_nTextLen = WNDPTB_MAX_TEXT_LEN;

	if (m_nTextLen > 0)
		memcpy(m_sText, pText, m_nTextLen);
}

// --
//
// --
void KWndPureTextBtn::SetColor(unsigned int nNormalColor, unsigned int nOverColor, unsigned int nPressedColor, 
							   unsigned int nNormalBorderColor, unsigned int nOverBorderColor, unsigned int nPressedBorderColor)
{
	m_NormalColor	= nNormalColor;
	m_OverColor		= nOverColor;
	m_PressedColor	= nPressedColor;
	m_NormalBorderColor	= nNormalBorderColor;
	m_OverBorderColor	= nOverBorderColor;
	m_PressedBorderColor	= nPressedBorderColor;
}

// --
//
// --
void KWndPureTextBtn::CheckButton(int bChecked)
{
	if (bChecked)
		m_Flag |= WNDPTB_F_CHECK;
	else
		m_Flag &= ~WNDPTB_F_CHECK;
}


int KWndPureTextBtn::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (IsDisable())
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		if (m_Flag & WNDPTB_F_BE_PRESSEDDOWN)
		{
			m_Flag &= ~WNDPTB_F_BE_PRESSEDDOWN;
			if(m_szItemInfo[0])
			{
				if(KUiChatItemInfo::GetIfVisible()) 
					KUiChatItemInfo::CloseWindow();

				GenChatItem(m_szItemInfo);
			}
			else if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_BUTTON_CLICK,
					(unsigned int)(KWndWindow*)this, 0);
		}
		break;
	case WM_LBUTTONDOWN:
		m_Flag |= WNDPTB_F_BE_PRESSEDDOWN;
		break;
	case WM_MOUSEMOVE:
		m_Flag |= WNDPTB_F_OVER;
		break;
	case WND_M_MOUSE_LEAVE:
		m_Flag &= ~WNDPTB_F_OVER;
		m_Flag &= ~WNDPTB_F_BE_PRESSEDDOWN;
		KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}


void KWndPureTextBtn::GenChatItem(char* pszInfo)
{
	if(!pszInfo[0])
		return;

	char * pszCheck1 = NULL;
	char * pszCheck2 = NULL;
	int nOffset = 0;
	int i = 0;
	int j = 0;
	int nCount = 0;
	BOOL bOk = FALSE;
	char szNum[16];
	KUiChatItemParam	cItemInfo;
	int nLen = strlen(pszInfo);

	pszCheck1 = pszInfo + i;
	while(i < nLen)
	{	
		if(*pszCheck1 == '(')
		{
			pszCheck2 = pszInfo + i;
			j = i;
			while(j < nLen)
			{
				 if(*pszCheck2 == ')' && nCount == 17) 
				 {	
					 bOk = TRUE;
					 break;
				 }

				 if(*pszCheck2 == '•') 
					 nCount++;

				 pszCheck2++;
				 j++;
			}
			if(bOk)
			{
				break;
			}
		}
		pszCheck1++;
		i++;

	}
	if(!bOk)
		return;

	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// Color
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_ColorID = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Genre
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Genre = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Detail
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_DetailType = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Record
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Record = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Series
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Series = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Level
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Level = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Luck
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 11) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Luck = atoi(szNum);
	// End
	// Magic Level
	for (i = 0; i < 6; i++)
	{	
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_MagicLevel[i] = atoi(szNum);
	}
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// RandomSeed
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 11) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_RandomSeed = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// Version
	while(1)
	{
		if(*pszCheck1 == '#')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_Version = atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// StackNum
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 5) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_StackCount = (BYTE)atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	// BindState
	while(1)
	{
		if(*pszCheck1 == '•')
			break;
		if(nOffset < 11)
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_BindState = (DWORD)atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End
	
	// Expired Time
	while(1)
	{
		if(*pszCheck1 == ')')
			break;
		if(nOffset < 11) 
		{
			szNum[nOffset] = (*pszCheck1);
		}
		pszCheck1++;
		nOffset++;
	}
	cItemInfo.m_ExpiredTime = (DWORD)atoi(szNum);
	ZeroMemory(szNum, sizeof(szNum));
	nOffset = 0;
	pszCheck1++;
	// End

	KUiChatItemDesc pDesc = { 0 };

	if(g_pCoreShell->GetGameData(GDI_CHAT_ITEM_DESC, (unsigned int)&cItemInfo, (unsigned int)&pDesc))
	{	
		char szDescName[GOD_MAX_OBJ_TITLE_LEN], szImageName[MAX_PATH];
		int nWidth, nHeight, nLenDesc;
		strcpy(szDescName, pDesc.szItemDesc);
		nLenDesc = TEncodeText(szDescName, strlen(szDescName));
		strcpy(szImageName, pDesc.szImageIcon);
		nWidth = pDesc.nWidth;
		nHeight = pDesc.nHeight;

		KUiChatItemInfo::OpenWindow(szDescName, nLenDesc, szImageName, nWidth, nHeight);
	}
}