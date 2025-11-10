#pragma once
// ----------------------------------------------------------------------------------------------
#define TRAYMODE_ICON_ID		0x444
#define TRAYMODE_ICON_MESSAGE	0x445
// ----------------------------------------------------------------------------------------------
class TrayMode
{
private:
	static TrayMode* m_pSelf;
public:
	static BOOL 	CheckTrayFPS();
	void	ShowNotify(bool Mode);
	LRESULT WndProc(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam);
}; extern TrayMode gTrayMode;