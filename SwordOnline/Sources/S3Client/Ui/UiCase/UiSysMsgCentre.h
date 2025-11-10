
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../../../core/src/gamedatadef.h"

#define	MAX_SYS_MSG_TYPE			8
#define	POPUPING_MSG_HEAP_INDEX		MAX_SYS_MSG_TYPE
#define	RECYCLE_MSG_HEAP_INDEX		POPUPING_MSG_HEAP_INDEX + 1

class KUiSysMsgCentre : protected KWndWindow
{
public:
	static KUiSysMsgCentre*	OpenWindow();		
	static void				CloseWindow();		
	static void				LoadScheme(const char* pScheme);	

	static bool				AMessageArrival(KSystemMessage* pMsg, void* pParam);

private:
	KUiSysMsgCentre();
	~KUiSysMsgCentre();
	void	Initialize();
	void	Clear();
	void	LoadScheme(KIniFile* pIni);			
	KSystemMessage*	GetAMsgSpace(int nParamSize);
	bool	AddAMsgToHeap(KSystemMessage*pMsg, int nHeapIndex, bool bSort);
	void	DeleteMsgInHeap(int nHeapIndex, int nMsgIndex, bool bImmedDel, bool bTobeConfirm);
	void	ConfirmMsg(KSystemMessage* pMsg, bool bImmedDel);
	void	MovePopupedMsgToHeap();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

	void	OnConfirmOperFinished(unsigned int uParam, int nSelAction);	
	void	SetPopupMsgDest();
	void	PaintWindow();							
	int		PtInWindow(int x, int y);
	void	Breathe();
	bool	FilterSameMsg(KSystemMessage* pMsg, void* pParam);
private:
	static KUiSysMsgCentre* m_pSelf;
private:
	struct	SYS_MSG_HEAP
	{
		KSystemMessage**	pMsgList;						
		int					nListSpace;						
		int					nNumValid;
	} m_MsgHeap[MAX_SYS_MSG_TYPE + 2];

	KUiMsgParam				m_SysMsgParam;					

	int						m_nPopupMsgDestX;				
	int						m_nPopupMsgX;					
	int						m_nPopupMsgY;					
	int						m_nPopupMsgDestIndex;			
	unsigned int			m_uLastMovementTime;
	unsigned int			m_uMoveInterval;
	unsigned int			m_uDisappearInterval;

	KWndText80				m_MsgTextWnd;
	int						m_bShowMsgText;
	KWndButton				m_MsgIconBtn[MAX_SYS_MSG_TYPE];
	KUiImageRef				m_MsgIcon[MAX_SYS_MSG_TYPE];
	short					m_nMsgIconFrame[MAX_SYS_MSG_TYPE];
	KSystemMessage*			m_pHandlingMsg;
};