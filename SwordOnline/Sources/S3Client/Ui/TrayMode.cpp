#include "KWin32.h"
#include "KWin32Wnd.h"
#include "TrayMode.h"
#include "UiShell.h"
// static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
TrayMode gTrayMode;
// an hien windows by kinnox;
// ----------------------------------------------------------------------------------------------
TrayMode* TrayMode::m_pSelf = NULL;
BOOL bFPS = FALSE;

void TrayMode::ShowNotify(bool Mode)
{
	bFPS = Mode;
	HICON hIcon = static_cast<HICON>(LoadImage(NULL,
    TEXT("game.ico"),
    IMAGE_ICON,
    0, 0,
    LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE | LR_LOADFROMFILE));	
		
	NOTIFYICONDATA Icon		= { 0 };
	// ----
	Icon.cbSize				= sizeof(NOTIFYICONDATA);
	Icon.uID				= TRAYMODE_ICON_ID;
	Icon.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	Icon.hIcon				= hIcon;
	Icon.hWnd				= g_GetMainHWnd();
	Icon.uCallbackMessage	= TRAYMODE_ICON_MESSAGE;
	// ----
	strcpy(Icon.szTip,"Jx Online MiniMine");
	// ----
	if( Mode )
	{
		Shell_NotifyIcon(NIM_ADD, &Icon);
		UpdateWindow(g_GetMainHWnd());
		ShowWindow(g_GetMainHWnd(), SW_SHOWMINNOACTIVE);
		ShowWindow(g_GetMainHWnd(), SW_HIDE);

	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &Icon);
		UpdateWindow(g_GetMainHWnd());
		ShowWindow(g_GetMainHWnd(), SW_SHOW);
		ShowWindow(g_GetMainHWnd(), SW_RESTORE);
	}
	// ----
	DeleteObject(Icon.hIcon);
}
BOOL 	TrayMode::CheckTrayFPS()
{
	if (bFPS)
		return bFPS;
	else 
		return FALSE;
}
// ----------------------------------------------------------------------------------------------

LRESULT TrayMode::WndProc(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
	case TRAYMODE_ICON_MESSAGE:
		{
			switch(lParam)
			{
			case WM_LBUTTONUP:
				{
					ShowWindow(Window, SW_SHOW);
					UpdateWindow(g_GetMainHWnd());
				}
				break;
			}
		}
		break;
	}
	// ----
	// return CallWindowProc((WNDPROC)gTrayMode.TempWindowProc, Window, Message, wParam, lParam);
	return DefWindowProc(Window, Message, wParam, lParam);
}

