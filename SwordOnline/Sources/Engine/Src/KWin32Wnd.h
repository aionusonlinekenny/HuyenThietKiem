
#ifndef KWin32Wnd_H
#define KWin32Wnd_H
ENGINE_API HWND	g_GetMainHWnd();
ENGINE_API void	g_SetMainHWnd(HWND hWnd);
ENGINE_API HWND	g_GetDrawHWnd();
ENGINE_API void	g_SetDrawHWnd(HWND hWnd);
ENGINE_API void	g_GetClientRect(LPRECT lpRect);
ENGINE_API void	g_ClientToScreen(LPRECT lpRect);
ENGINE_API void	g_ScreenToClient(LPRECT lpRect);
ENGINE_API void g_ScreenToClient(LPPOINT lpPoint);
ENGINE_API void	g_ChangeWindowStyle();
#endif
