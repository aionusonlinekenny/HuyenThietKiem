#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndWindow.h"
#include "shlwapi.h"
#include "Wnds.h"
#include "MouseHover.h"
#ifdef _DEBUG
#include "KEngine.h"
#endif

#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
extern iRepresentShell*	g_pRepresentShell;


int WND_SHOW_DEBUG_FRAME_TEXT = false;

#define	MOSE_HOVER_TIME_VALVE		1000	


KWndWindow::KWndWindow()
{
#ifdef _DEBUG
	m_Caption[0]	= 0;
	m_Caption[31]	= 0;
#endif
	m_Left			= 0;
	m_Top			= 0;
	m_Width			= 0;
	m_Height		= 0;
	m_nAbsoluteLeft = 0;
	m_nAbsoluteTop	= 0;

	m_bMoving = false;
	m_nLastMouseHoldPosX = m_nLastMouseHoldPosY = 0;

	m_pPreviousWnd	= NULL;
	m_pNextWnd		= NULL;
	m_pFirstChild	= NULL;
	m_pParentWnd	= NULL;
	m_Style			= WND_S_VISIBLE;
}


KWndWindow::~KWndWindow()
{
	LeaveAlone();
	Wnd_OnWindowDelete(this);
	g_MouseOver.OnWndClosed((void*)this);
}


void KWndWindow::BringToTop()
{
	if (m_pNextWnd)
	{
		KWndWindow*	pLast = m_pNextWnd;
		while(pLast->m_pNextWnd)
			pLast = pLast->m_pNextWnd;

		pLast->m_pNextWnd = this;
		m_pNextWnd->m_pPreviousWnd = m_pPreviousWnd;
		if (m_pPreviousWnd)
			m_pPreviousWnd->m_pNextWnd = m_pNextWnd;
		else if (m_pParentWnd && m_pParentWnd->m_pFirstChild == this)
			m_pParentWnd->m_pFirstChild = m_pNextWnd;
		m_pNextWnd = NULL;
		m_pPreviousWnd = pLast;
	}
}


void KWndWindow::GetPosition(int* pLeft, int* pTop)
{
	if (pLeft)
		*pLeft = m_Left;
	if (pTop)
		*pTop = m_Top;
}


void KWndWindow::SetPosition(int nLeft, int nTop)
{
	AbsoluteMove(nLeft - m_Left, nTop - m_Top);
	m_Left = nLeft;
	m_Top  = nTop;
}

void KWndWindow::GetAbsolutePos(int* pLeft, int* pTop)
{
	if (pLeft)
		*pLeft = m_nAbsoluteLeft;
	if (pTop)
		*pTop = m_nAbsoluteTop;
}


void KWndWindow::GetSize(int* pWidth, int* pHeight)
{
	if (pWidth)
		*pWidth = m_Width;
	if (pHeight)
		*pHeight = m_Height;
}


void KWndWindow::SetSize(int nWidth, int nHeight)
{
	if (nWidth < 0)
		nWidth = 0;
	if (nHeight < 0)
		nHeight = 0;
	int nDX = nWidth - m_Width;
	int nDY = nHeight - m_Height;
	if (nDX == 0 && nDY == 0)
		return;
	m_Width = nWidth;
	m_Height = nHeight;

	KWndWindow* pChild = m_pFirstChild;
	while(pChild)
	{
		int	x, y, w, h;
		pChild->GetPosition(&x, &y);
		pChild->GetSize(&w, &h);
		if(nDX)
		{
			if (pChild->m_Style & WND_S_SIZE_WITH_R_EDGE)
				pChild->SetSize(w + nDX, h);
			if (pChild->m_Style & WND_S_MOVE_WITH_R_EDGE)
				pChild->SetPosition(x + nDX, y);
		}
		if (nDY)
		{
			if (pChild->m_Style & WND_S_SIZE_WITH_B_EDGE)
				pChild->SetSize(w, h + nDY);
			if (pChild->m_Style & WND_S_MOVE_WITH_B_EDGE)
				pChild->SetPosition(x, y + nDY);
		}
		pChild = pChild->m_pNextWnd;
	};
}


void KWndWindow::AbsoluteMove(int dx, int dy)
{
	m_nAbsoluteLeft += dx;
	m_nAbsoluteTop  += dy;
	KWndWindow* pChild = m_pFirstChild;

	while(pChild)
	{
		pChild->AbsoluteMove(dx, dy);
		pChild = pChild->m_pNextWnd;
	}
}


void KWndWindow::Show()
{
	m_Style |= WND_S_VISIBLE;
}


void KWndWindow::Hide()
{
	m_Style &= ~WND_S_VISIBLE;
	g_MouseOver.OnWndClosed((void*)this);
	KWndWindow* pFocus = Wnd_GetFocusWnd();
	while(pFocus)
	{
		if (pFocus != this)
			pFocus = pFocus->m_pParentWnd;
		else
		{
			Wnd_SetFocusWnd(NULL);
			break;
		}
	}
	Wnd_ReleaseExclusive(this);
}


int KWndWindow::IsVisible()
{
	return (m_Style & WND_S_VISIBLE);
}

void KWndWindow::Enable(int bEnable)
{
	if (bEnable)
		m_Style &= ~WND_S_DISABLE;
	else
		m_Style |= WND_S_DISABLE;
}

void KWndWindow::Destroy()
{
	m_Style |= WND_S_TOBEDESTROY;
}

void KWndWindow::Clone(KWndWindow* pCopy)
{
	if (pCopy)
	{
		pCopy->m_Width	= m_Width;
		pCopy->m_Height = m_Height;
		pCopy->m_Style = m_Style;
		pCopy->SetPosition(m_Left, m_Top);
	}
}

int KWndWindow::Init(KIniFile* pIniFile, const char* pSection)
{
	int		nValue1, nValue2;

	if (pIniFile && pSection)
	{
#ifdef _DEBUG
		strncpy(m_Caption, pSection, 32);
#endif
		pIniFile->GetInteger(pSection, "Width", 0, &m_Width);
		pIniFile->GetInteger(pSection, "Height",0, &m_Height);
		pIniFile->GetInteger(pSection, "Left",  0, &nValue1);
		pIniFile->GetInteger(pSection, "Top",   0, &nValue2);
		SetPosition(nValue1, nValue2);		
		pIniFile->GetInteger(pSection, "Disable",  0, &nValue1);
		pIniFile->GetInteger(pSection, "Moveable", 0, &nValue2);
		if (nValue1)
			m_Style |= WND_S_DISABLE;
		else
			m_Style &= ~WND_S_DISABLE;
		if (nValue2)
			m_Style |= WND_S_MOVEALBE;
		else
			m_Style &= ~WND_S_MOVEALBE;

		nValue1 = nValue2 = 0;
		pIniFile->GetInteger2(pSection, "FollowMove", &nValue1, &nValue2);
		if (nValue1)
			m_Style |= WND_S_MOVE_WITH_R_EDGE;
		else
			m_Style &= ~WND_S_MOVE_WITH_R_EDGE;
		if (nValue2)
			m_Style |= WND_S_MOVE_WITH_B_EDGE;
		else
			m_Style &= ~WND_S_MOVE_WITH_B_EDGE;

		nValue1 = nValue2 = 0;
		pIniFile->GetInteger2(pSection, "FollowSize", &nValue1, &nValue2);
		if (nValue1)
			m_Style |= WND_S_SIZE_WITH_R_EDGE;
		else
			m_Style &= ~WND_S_SIZE_WITH_R_EDGE;
		if (nValue2)
			m_Style |= WND_S_SIZE_WITH_B_EDGE;
		else
			m_Style &= ~WND_S_SIZE_WITH_B_EDGE;
		pIniFile->GetInteger(pSection, "DummyWnd", 0, &nValue1);
		if (nValue1 == 0)
			m_Style &= ~WND_S_SIZE_WITH_ALL_CHILD;
		else
			m_Style |= WND_S_SIZE_WITH_ALL_CHILD;
		
		return true;
	}
	return false;
}

int KWndWindow::PtInWindow(int x, int y)
{
	int nRet = 0;
	if (m_Style & WND_S_VISIBLE)
	{
		if ((m_Style & WND_S_SIZE_WITH_ALL_CHILD) == 0)
		{
			nRet = (x >= m_nAbsoluteLeft && y >= m_nAbsoluteTop &&
				x < (int)m_nAbsoluteLeft + m_Width && y < (int)m_nAbsoluteTop + m_Height);
		}
		else
		{
			KWndWindow* pChild = m_pFirstChild;
			while(pChild)
			{
				if (pChild->PtInWindow(x, y))
				{
					nRet = 1;
					break;
				}
				pChild = pChild->m_pNextWnd;
			};
		}
	}
	return nRet;
}

void KWndWindow::Paint()
{
	if (m_Style & WND_S_VISIBLE)
	{
		PaintWindow();
		if (m_pFirstChild)
			m_pFirstChild->Paint();
	}
	if (m_pNextWnd)
		m_pNextWnd->Paint();
}

void KWndWindow::LetMeBreathe()
{
	if (m_pNextWnd)
		m_pNextWnd->LetMeBreathe();

	if (m_Style & WND_S_TOBEDESTROY)
		delete this;
	else if ((m_Style & (WND_S_VISIBLE | WND_S_DISABLE)) == WND_S_VISIBLE)
		Breathe();
}

void KWndWindow::AddChild(KWndWindow* pChild)
{
	if (pChild)
	{
		pChild->SplitSmaleFamily();

		pChild->AbsoluteMove(m_nAbsoluteLeft + pChild->m_Left - pChild->m_nAbsoluteLeft,
			m_nAbsoluteTop + pChild->m_Top - pChild->m_nAbsoluteTop);

		pChild->m_pParentWnd = this;
		if (m_pFirstChild == NULL)
			m_pFirstChild = pChild;
		else
		{
			KWndWindow* pBrother = m_pFirstChild;
			while(pBrother->m_pNextWnd)
				pBrother = pBrother->m_pNextWnd;
			pBrother->m_pNextWnd = pChild;
			pChild->m_pPreviousWnd = pBrother;
		}
	}
}

void KWndWindow::AddBrother(KWndWindow* pBrother)
{
	if (pBrother)
	{
		pBrother->SplitSmaleFamily();

		if (m_pParentWnd)
		{
			pBrother->AbsoluteMove(m_pParentWnd->m_nAbsoluteLeft + pBrother->m_Left - pBrother->m_nAbsoluteLeft,
				m_pParentWnd->m_nAbsoluteTop + pBrother->m_Top - pBrother->m_nAbsoluteTop);
		}

		pBrother->m_pParentWnd = m_pParentWnd;
		if (m_pNextWnd == NULL)
		{
			m_pNextWnd = pBrother;
			pBrother->m_pPreviousWnd = this;
		}
		else
		{
			KWndWindow* pWnd = m_pNextWnd;
			while(pWnd->m_pNextWnd)
				pWnd = pWnd->m_pNextWnd;
			pWnd->m_pNextWnd = pBrother;
			pBrother->m_pPreviousWnd = pWnd;
		}
	}
}

void KWndWindow::LeaveAlone()
{
	SplitSmaleFamily();
	while (m_pFirstChild)
	{
		m_pFirstChild->m_pParentWnd = NULL;
		m_pFirstChild = m_pFirstChild->m_pNextWnd;
	}
}

void KWndWindow::SplitSmaleFamily()
{
	if (IsVisible())
	{
		KWndWindow::Hide();
		KWndWindow::Show();
	}
	if (m_pPreviousWnd)
		m_pPreviousWnd->m_pNextWnd = m_pNextWnd;
	else if (m_pParentWnd)
		m_pParentWnd->m_pFirstChild = m_pNextWnd;
	if (m_pNextWnd)
		m_pNextWnd->m_pPreviousWnd = m_pPreviousWnd;
	m_pPreviousWnd = NULL;
	m_pNextWnd     = NULL;
	m_pParentWnd   = NULL;
}

int KWndWindow::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLBtnDown(LOWORD(nParam), HIWORD(nParam));
		nRet = 1;
		break;
	case WM_RBUTTONDOWN:
		if (m_pParentWnd)
		{
			m_pParentWnd->WndProc(WND_M_POPUPMENU, (unsigned int)(KWndWindow*)this, nParam);
			nRet = 1;
		}
		break;
	case WM_MOUSEMOVE:
		if (m_bMoving)
		{
			OnMoveWnd();
			nRet = 1;
		}
		break;
	case WM_LBUTTONUP:
		if (m_bMoving)
		{
			Wnd_ReleaseCapture();
			m_bMoving = false;
			if (m_pParentWnd)
			{
				m_pParentWnd->WndProc(WND_N_CHILD_MOVE, (unsigned int)(KWndWindow*)this, 0);
			}
		}
		nRet = 1;
		break;
	case WND_M_CAPTURECHANGED:
		m_bMoving = false;
		nRet = 1;
		break;
	}
	return nRet;
}

void KWndWindow::OnLBtnDown(int x, int y)
{
	if (m_Style & WND_S_MOVEALBE)
	{
		m_bMoving = true;
		m_nLastMouseHoldPosX = x;
		m_nLastMouseHoldPosY = y;
		Wnd_SetCapture(this);
	}
}

void KWndWindow::OnMoveWnd()
{
	int x, y;
	Wnd_GetCursorPos(&x, &y);
	if (x != m_nLastMouseHoldPosX || y != m_nLastMouseHoldPosY)
	{
		x -= m_nLastMouseHoldPosX;
		y -= m_nLastMouseHoldPosY;
		SetPosition(x + m_Left, y + m_Top);
		m_nLastMouseHoldPosX += x;
		m_nLastMouseHoldPosY += y;

		x = ((x & 0xffff) | ((y & 0xffff) << 16));
		if (x && m_pParentWnd)
		{
			m_pParentWnd->WndProc(WND_N_CHILD_MOVE, (unsigned int)(KWndWindow*)this, x);
		}
	}
}

KWndWindow* KWndWindow::TopChildFromPoint(int x, int y)
{
	KWndWindow* pLastMatch = NULL;
	if (PtInWindow(x, y))
	{
		pLastMatch = this;
		KWndWindow*	pWnd = m_pFirstChild;
		while(pWnd)
		{
			while(pWnd->m_pNextWnd)
				pWnd = pWnd->m_pNextWnd;
			while(pWnd)
			{
				if (pWnd->PtInWindow(x, y) && !pWnd->IsDisable())
				{
					pLastMatch = pWnd;
					pWnd = pLastMatch->m_pFirstChild;
					break;
				}
				pWnd = pWnd->m_pPreviousWnd;
			}
		}
	}	
	return pLastMatch;
}

KWndWindow* KWndWindow::GetOwner()
{
	KWndWindow* pWnd = this;
	while(pWnd->m_pParentWnd)
		pWnd = pWnd->m_pParentWnd;
	return pWnd;
}

#ifdef _DEBUG
void KWndWindow::SetCaption(char* pszCaption)
{
	if (pszCaption)
		strncpy(m_Caption, pszCaption, 31);
}
#endif

void KWndWindow::PaintDebugInfo()
{
	char	szInfo[128];
	szInfo[0] = 0;
	int nInfoLen = 0;

	int nOld = WND_SHOW_DEBUG_FRAME_TEXT;
	WND_SHOW_DEBUG_FRAME_TEXT = true;
	KWndWindow::PaintWindow();
	WND_SHOW_DEBUG_FRAME_TEXT = nOld;

#ifdef _DEBUG
	sprintf(szInfo, "Name:%s, Pos:%d,%d,Size:%d,%d", m_Caption,
		m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height);
	nInfoLen = strlen(szInfo);
#else
	sprintf(szInfo, "Pos:%d,%d,Size:%d,%d", m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height);
	nInfoLen = strlen(szInfo);
#endif
	if (m_Style & WND_S_SIZE_WITH_ALL_CHILD)
	{
		RECT	rc;
		GetAllChildLayoutRect(&rc);
		sprintf(&szInfo[nInfoLen], ",Rect:%d,%d-%d,%d", rc.left, rc.top,
			rc.right, rc.bottom);
		nInfoLen = strlen(szInfo);
	}
	int x, y, w, h;
	Wnd_GetCursorPos(&x, &y);
	Wnd_GetScreenSize(w, h);
	if (x + nInfoLen * 6  + 24 > w)
		x = w - nInfoLen * 6 - 24;
	if (y + 25 > h)
		y = h - 13;
	else
		y += 12;
	g_pRepresentShell->OutputText(12, szInfo, nInfoLen, x, y, 0xFFFF0000,
		0, TEXT_IN_SINGLE_PLANE_COORD, 0xffffffff);
}

void KWndWindow::PaintWindow()
{
	if (m_bMoving)
		OnMoveWnd();

	if (g_pRepresentShell && WND_SHOW_DEBUG_FRAME_TEXT)
	{
		KRULine		Line[4];

		for(int i = 0; i < 4; i++)
		{
			Line[i].Color.Color_dw = 0xff0000ff;
			Line[i].oPosition.nX = Line[i].oEndPos.nX = m_nAbsoluteLeft;
			Line[i].oPosition.nY = Line[i].oEndPos.nY = m_nAbsoluteTop;
		}

		Line[0].oEndPos.nX += m_Width;
		Line[1].oPosition.nY += m_Height;
		Line[1].oEndPos.nX += m_Width;
		Line[1].oEndPos.nY += m_Height;
		Line[2].oEndPos.nY += m_Height;
		Line[3].oPosition.nX += m_Width;
		Line[3].oEndPos.nX += m_Width;
		Line[3].oEndPos.nY += m_Height;

		g_pRepresentShell->DrawPrimitives(4, Line, RU_T_LINE, true);

		if (m_Style & WND_S_SIZE_WITH_ALL_CHILD)
		{
			for(int i = 0; i < 4; i++)
				Line[i].Color.Color_dw = 0xffff0000;
			RECT	rc;
			GetAllChildLayoutRect(&rc);
			Line[0].oPosition.nX = Line[1].oPosition.nX = rc.left;
			Line[0].oEndPos.nX = Line[1].oEndPos.nX = rc.right;
			Line[0].oPosition.nY = Line[0].oEndPos.nY = rc.top;
			Line[1].oPosition.nY = Line[1].oEndPos.nY = rc.bottom;

			Line[0].oPosition.nY = Line[1].oPosition.nY = rc.top;
			Line[0].oEndPos.nY = Line[1].oEndPos.nY = rc.bottom;
			Line[0].oPosition.nX = Line[0].oEndPos.nX = rc.left;
			Line[1].oPosition.nX = Line[1].oEndPos.nX = rc.right;

			g_pRepresentShell->DrawPrimitives(4, Line, RU_T_LINE, true);
		}

#ifdef _DEBUG
		g_pRepresentShell->OutputText(12, m_Caption, -1, m_nAbsoluteLeft, m_nAbsoluteTop, 0xFFFFFFFF);
#endif
	}
}

unsigned int GetColor(LPCTSTR pString)
{
	if (pString == NULL)
		return 0;

	unsigned int Color = 0xFF000000;

	char Buf[16] = "";
	int  i = 0;
	int  n = 0;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 16);
	n = 0;
	i++;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 8);
	n = 0;
	i++;
	while (pString[i] != 0)
	{
		if (n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += (atoi(Buf) & 0xFF);
	return Color;
}

const char* GetColorString(unsigned int nColor)
{
	static char szColor[12];
	KRColor c;
	c.Color_dw = nColor;
	sprintf(szColor, "%d,%d,%d", c.Color_b.r, c.Color_b.g, c.Color_b.b);
	szColor[11] = 0;

	return szColor;
}

void KWndWindow::SetCursorAbove()
{
	Wnd_SetCursorPos(m_nAbsoluteLeft + m_Width / 2,
		m_nAbsoluteTop + m_Height / 2);
}

void KWndWindow::GetAllChildLayoutRect(RECT* pRect)
{
	if (pRect == NULL)
		return;
	if (m_pFirstChild == NULL)
	{
		pRect->left = pRect->right = m_nAbsoluteLeft;
		pRect->top = pRect->bottom = m_nAbsoluteTop;
	}
	else
	{
		m_pFirstChild->GetAbsolutePos((int*)&pRect->left, (int*)&pRect->top);
		pRect->right = pRect->left;
		pRect->bottom = pRect->top;

		KWndWindow* pChild = m_pFirstChild;
		do
		{
			RECT	rc;
			pChild->GetAllChildLayoutRect(&rc);
			int		x, y, w, h;
			pChild->GetAbsolutePos(&x, &y);
			pChild->GetSize(&w, &h);

			if (rc.left > x)
				rc.left = x;
			if (rc.top > y)
				rc.top = y;
			if (rc.right < x + w)
				rc.right = x + w;
			if (rc.bottom < y + h)
				rc.bottom = y + h;

			if (pRect->left > rc.left)
				pRect->left = rc.left;
			if (pRect->top > rc.top)
				pRect->top = rc.top;
			if (pRect->right < rc.right)
				pRect->right = rc.right;
			if (pRect->bottom < rc.bottom)
				pRect->bottom = rc.bottom;
		}while(pChild = pChild->m_pNextWnd);
	}
}
