
#pragma once

#if !defined _FACESELECTOR
#define _FACESELECTOR

#include "../Elem/WndButton.h"

class KUiFaceSelector : public KWndWindow
{
public:
	static KUiFaceSelector* OpenWindow(KWndWindow* pCaller, void* pvParam);
	static void				CloseWindow(bool bDestroy);
	static KUiFaceSelector*	GetIfVisible();
	void					LoadScheme(const char* pScheme);

	static void				LoadFaceList();
	static void				Clear();
	static int				ConvertFaceText(char* pDest, const char* pSrc, int nCount);
	
private:
	KUiFaceSelector();
	virtual		~KUiFaceSelector() {}
	static int	ConvertFace(char* pDest, int& nConvertCount, const char* pSrc, int nCount, int& nReadPos);
	void		PaintWindow();
	int			WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void		Show();
	void		Hide();

	void		Initialize();
	int			GetFaceAtPos(int x, int y);
	void		UpdateFaceTip(int x, int y);
	void		OnSelFace();
	void		OnExpandWnd();
	void		OnUnExpandWnd();

private:
	struct	KFaceItem
	{
		char	szFaceText[32];
		char	szFaceTip[32];
		WORD	wFaceID;
	};
	static KUiFaceSelector* m_pSelf;
	static KFaceItem*	ms_pFaceList;
	static int			ms_nNumFaces;

private:
	KWndButton			m_LessBtn;
	KWndButton			m_MoreBtn;

	int					m_nNormColuCount;
	int                 m_nExpColuCount;
	int					m_nRowCount;

	int                 m_nIndentH, m_nIndentV;
	int					m_nBtnWidth,  m_nBtnHeight;

	unsigned int		m_nBgColor;
	unsigned int		m_nBgOver;
	unsigned int		m_nBgAlpha;
	unsigned int		m_nBorderColor;

	int					m_nCurrIndex;
	BOOL                m_bIsExpandMode;

	KWndWindow*			m_pCallerWnd;
	void*				m_pvCallerParam;
};

#endif