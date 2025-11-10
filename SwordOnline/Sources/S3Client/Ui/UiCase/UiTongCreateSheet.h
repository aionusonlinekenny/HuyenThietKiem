
#if !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
#define AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"

class KUiTongCreateSheet : KWndImage
{
public:
	KUiTongCreateSheet();
	virtual ~KUiTongCreateSheet();

	static        KUiTongCreateSheet* OpenWindow(const char* lpszTongName);
	static        KUiTongCreateSheet* GetIfVisible();
	static void   CloseWindow(bool bDestory = TRUE); 
	static void   LoadScheme(const char* pScheme); 

private:
	static        KUiTongCreateSheet *ms_pSelf;

private:
	void          Initialize();               
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	void          AlignmentButtonCheck(NPCCAMP eSide);
	void          OnDone();                   

private:
	KWndText32    m_TextError;
	KWndImage		m_TitleImg;
	KWndText32    m_TxtTongName; 
	KWndEdit32    m_EditTongName;
	KWndText32    m_TxtTongCamp; 
	KWndButton    m_BtnOrder, m_BtnNatural;   
	KWndButton    m_BtnChaos;
    KWndLabeledButton	m_BtnDone, m_BtnCancel;

private:
	bool		m_bCreate;
	int         m_nSelectSide;              
	char        m_szNameNullString[32];     
	char        m_szAlignmentNullString[32];
};


#endif // !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
