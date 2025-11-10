
#if !defined(AFX_UITONGASSIGNBOX_H__1D36E55C_C9D4_44AF_8E20_0FF51BDB2BE0__INCLUDED_)
#define AFX_UITONGASSIGNBOX_H__1D36E55C_C9D4_44AF_8E20_0FF51BDB2BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndlabeledbutton.h"
#include "../elem/WndPureTextBtn.h"
#include "../elem/wndimage.h"
#include "../elem/wndtext.h"

class KUiTongAssignBox : public KWndImage
{
public:
	KUiTongAssignBox();
	virtual ~KUiTongAssignBox();

	static        KUiTongAssignBox* OpenWindow();
	static        KUiTongAssignBox* GetIfVisible();
	static void   CloseWindow(bool bDestory = TRUE);
	static void   LoadScheme(const char* pScheme);

public:
	static void   LinkToMainWindow(KWndWindow *pMain);

	static void   ArrangeData(int nType, char* szTarName, char* szTarAgName, int nCurrentFigure, int nCurrentPosition = 0);
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	static        KUiTongAssignBox* ms_pSelf;

private:
	void          PopupConfirmWindow(unsigned int uHandleID);
	void          UpdateCheckButton(int nFigure);   
	void          Initialize();
	void          OnConfirm();                      

private:
	enum RESULT_TYPE
	{
		RESULT_T_ASSIGN,
	};

private:
	KWndPureTextBtn		m_BtnElder;
	KWndPureTextBtn		m_BtnCaptain;
	KWndPureTextBtn		m_BtnPilgarlic;
	KWndPureTextBtn		m_BtnConfirm;
	KWndPureTextBtn		m_BtnCancel;

	KWndText80			m_TargetName;
	KWndText32			m_TitlePlayerTxt, m_TitlePositionTxt;
	KWndText80			m_TextError;

	KWndWindow			*m_pMain;

private:
	int                      m_nType, m_nCurrentFigure, m_nCurrentPostion;
	char                     m_szTargetPlayerName[32];

	char                     m_szAssign[64];
	int                      m_nSelectFigure;

	char                     m_szErrorNotSelectFigure[32];
};

#endif // !defined(AFX_UITONGASSIGNBOX_H__1D36E55C_C9D4_44AF_8E20_0FF51BDB2BE0__INCLUDED_)
