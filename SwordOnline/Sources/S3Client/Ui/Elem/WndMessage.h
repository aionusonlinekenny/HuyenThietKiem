#pragma once

#define	WND_GAMESPACE			0xFFFFFFFF

#ifndef WM_MOUSEHOVER
	#define	WM_MOUSEHOVER	0x02A1
#endif

#define	WND_M_BASE				0x500
#define WND_M_OTHER_WORK_RESULT	WND_M_BASE + 1
#define	WND_M_MENUITEM_SELECTED	WND_M_BASE + 2
#define	WND_M_MENUITEMRIGHT_SELECTED	WND_M_BASE + 3
#define WND_M_MOUSE_ENTER		WND_M_BASE + 6
#define	WND_M_MOUSE_LEAVE		WND_M_BASE + 7
#define	WND_N_MOUSE_ENTER_CHILD	WND_M_BASE + 8
#define	WND_N_MOUSE_LEAVE_CHILD	WND_M_BASE + 9
#define	WND_M_SET_FOCUS			WND_M_BASE + 10
#define	WND_M_KILL_FOCUS		WND_M_BASE + 11
#define	WND_M_CAPTURECHANGED	WND_M_BASE + 12
#define	WND_M_HOTKEY			WND_M_BASE + 13
#define WND_M_POPUPMENU			WND_M_BASE + 14	
#define WND_N_ITEM_PICKDROP		WND_M_BASE + 17
#define WND_N_LEFT_CLICK_ITEM	WND_M_BASE + 18
#define WND_N_RIGHT_CLICK_ITEM	WND_M_BASE + 19
#define WND_N_CHILD_MOVE		WND_M_BASE + 20
#define WND_M_BUTTON_BASE			WND_M_BASE + 100
#define WND_N_BUTTON_CLICK			WND_M_BUTTON_BASE + 1
#define	WND_N_BUTTON_DOWN			WND_M_BUTTON_BASE + 2
#define	WND_N_BUTTON_MR_DOWN		WND_M_BUTTON_BASE + 3
#define	WND_N_BUTTON_HOLD			WND_M_BUTTON_BASE + 4
#define WND_N_BUTTON_OVER			WND_M_BUTTON_BASE + 5
#define WND_N_BUTTON_DB_CLICK		WND_M_BUTTON_BASE + 6
#define WND_M_SCORLLBAR_BASE		WND_M_BASE + 200
#define WND_N_SCORLLBAR_POS_CHANGED	WND_M_SCORLLBAR_BASE + 1
#define	WND_M_EDIT_BASE				WND_M_BASE + 300
#define	WND_N_EDIT_CHANGE			WND_M_EDIT_BASE + 1
#define	WND_N_EDIT_SPECIAL_KEY_DOWN	WND_M_EDIT_BASE + 2
#define	WND_M_LIST_BASE				WND_M_BASE + 400
#define	WND_N_LIST_ITEM_SEL			WND_M_LIST_BASE + 1
#define	WND_N_LIST_ITEM_ACTIVE		WND_M_LIST_BASE + 2
#define	WND_N_LIST_ITEM_D_CLICK		WND_M_LIST_BASE + 3
#define WND_N_LIST_ITEM_R_CLICK		WND_M_LIST_BASE + 4
#define	WND_N_LIST_ITEM_HIGHLIGHT	WND_M_LIST_BASE + 5
#define WND_OPER_RESULT_0		0
#define WND_OPER_RESULT(n)		(n)
#if !defined WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif
#if !defined WHEEL_DELTA
#define WHEEL_DELTA                     120
#endif

//=========================
//
//=========================
struct KUiDraggedObject
{
	unsigned int	uGenre;
	unsigned int	uId;
	int				DataX;
	int				DataY;
	int				DataW;
	int				DataH;
	int			  m_nPos;
	unsigned char m_nDir;
	unsigned char m_nLastFrame;
};

//=========================
//
//=========================
struct ITEM_PICKDROP_PLACE
{
	class KWndWindow*	pWnd;
	int					h;
	int					v;
};
