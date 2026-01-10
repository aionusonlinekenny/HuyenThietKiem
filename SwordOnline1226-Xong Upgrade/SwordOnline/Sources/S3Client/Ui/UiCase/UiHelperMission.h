
#pragma once

#if !defined _UIHELPERMISSION
#define _UIHELPERMISSION

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
#include "../Elem/WndMessageListBox.h"

#define		MAX_INDEX_NUM		512

class KUiHelperMission : protected KWndShowAnimate
{
public:
	static KUiHelperMission*	OpenWindow(bool bShow);
	static KUiHelperMission*  GetIfVisible();
	static void			 CloseWindow(bool bDestory);
	static void			 LoadScheme(const char* pScheme);

private:
	KUiHelperMission();
	~KUiHelperMission();
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadIndexList(const char* pScheme);
	void	Clear();
	void	UpdateData(int nIndex);

private:
	static KUiHelperMission*	m_pSelf;
	KWndButton				m_BtnClose;
	KWndButton				m_BtnPic;
	KWndButton              m_BtnKeyboard;
	KWndButton              m_BtnWuxing;
	KWndList				m_IndexList;
	KWndScrollBar			m_IndexScroll;
	KWndMessageListBox		m_MessageList;
	KWndScrollBar			m_MessageScroll;
	int						m_nIndexCount;
	KWndText32				m_Title;
	KWndText32				m_Content;
	typedef struct _IndexData
	{
		char szName[128];
		char szIni[128];
		char szIniSection[128];
	} IndexData;
	IndexData m_IndexData[MAX_INDEX_NUM];
};

#endif