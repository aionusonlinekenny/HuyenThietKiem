/*****************************************************************************************
//	剑侠引擎，界面窗口元素体系头文件
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
// --
	界面元素（控件）的共同基类为KWndWindow，Wnd_前缀打头的一组函数用于使窗口形成一个大集合，
完成一些特定的功能。
    窗口系统支持热键(HotKey)功能，最多同时支持24个热键设置，此上限数值可被修改。
*****************************************************************************************/

class KWndWindow;
struct KUiDraggedObject;
struct iKWndGameSpace;

enum CURSOR_INDEX
{
	CURSOR_NORMAL = 0,
	CURSOR_POINT_TO_ATTACKABLE,
	CURSOR_POINT_TO_OBJ_NPC,	
	CURSOR_PICKABLE,			
	CURSOR_SKILL_ENABLE,		
	CURSOR_BUY,					
	CURSOR_SELL,				
	CURSOR_REPAIR,				
    CURSOR_USE,
	CURSOR_SHOP,
	CURSOR_INDEX_COUNT,
};

typedef int (*fnDrawDraggedObj)(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult);

enum WND_LAYER_LIST
{
	WL_LOWEST,
	WL_NORMAL,
	WL_TOPMOST,
};

void		Wnd_Cleanup();
void		Wnd_GetScreenSize(int& nWidth, int& nHeight);
void		Wnd_SetScreenSize(int nWidth, int nHeight);
void		Wnd_RenderWindows();
void		Wnd_AddWindow(KWndWindow* pWnd, WND_LAYER_LIST eLayer = WL_NORMAL);	//添加窗口
void		Wnd_OnWindowDelete(KWndWindow* pWnd);
void		Wnd_SetGameSpaceWnd(KWndWindow* pWnd);
void		Wnd_GameSpaceHandleInput(bool bHandle);
void		Wnd_TransmitInputToGameSpace(unsigned int uMsg, unsigned int uParam, int nParam);
void		Wnd_ShowHideGameSpace(bool bShow);
void		Wnd_ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
void		Wnd_Heartbeat();
void		Wnd_ShowCursor(int bShow);
void		Wnd_SetCursorPos(int x, int y);
void		Wnd_GetCursorPos(int* px, int* py);
void		Wnd_LoadCursor(int nIndex, const char* pImgFile);
int			Wnd_SwitchCursor(int nIndex);
void		Wnd_RestoreCursor();
int			Wnd_DragBegin(const KUiDraggedObject* pObj, fnDrawDraggedObj fnDrawObjFunc);
int			Wnd_GetDragObj(KUiDraggedObject* pObj);
void		Wnd_DragFinished();	
void		Wnd_SetFocusWnd(KWndWindow* pWnd);
KWndWindow*	Wnd_GetFocusWnd();
void		Wnd_SetCapture(KWndWindow* pWnd);
void		Wnd_ReleaseCapture();
void		Wnd_SetExclusive(KWndWindow* pWnd);
void		Wnd_ReleaseExclusive(KWndWindow* pWnd);
