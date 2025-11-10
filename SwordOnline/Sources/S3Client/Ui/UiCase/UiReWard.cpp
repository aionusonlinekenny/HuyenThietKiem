/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	08.06.2021
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiShell.h"
#include "../../../core/src/coreshell.h"
#include "../UiSoundSetting.h"
#include <crtdbg.h>
#include "UiReWard.h"
#include "../UiBase.h"
#include "../../../Engine/src/KDebug.h"
extern iCoreShell*	g_pCoreShell;

#define SCHEME_INI		"UiReWard.ini"

KUiReWard* KUiReWard::m_pSelf = NULL;


//--------------------------------------------------------------------------
//	Mo hop thoai, tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiReWard* KUiReWard::OpenWindow(const char* pszTitle, const char* pszString1, const char* pszString2, const char* pszString3)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiReWard;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		if (pszTitle)
			m_pSelf->m_Notice.SetText(pszTitle);

		strcpy(m_pSelf->szString1, pszString1);	
		strcpy(m_pSelf->szString2, pszString2);	
		strcpy(m_pSelf->szString3, pszString3);
		
		m_pSelf->Show();
		m_pSelf->ShowButton();
		m_pSelf->BringToTop();		
	}
	return m_pSelf;
}

KUiReWard*	KUiReWard::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}
//--------------------------------------------------------------------------
// Dong hop thoai 
//--------------------------------------------------------------------------
void KUiReWard::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

// -------------------------------------------------------------------------
// Khoi tao hop thoai
// -------------------------------------------------------------------------
void KUiReWard::Initialize()
{
	AddChild(&m_Notice);
	AddChild(&BtnReWard1);
	AddChild(&BtnReWard2);
	AddChild(&BtnReWard3);
	
	AddChild(&m_CloseBtn);
	
	char schemePath[256];
	g_UiBase.GetCurSchemePath(schemePath, 256);
	LoadScheme(schemePath);
	Wnd_AddWindow(this, WL_TOPMOST);
}


// -------------------------------------------------------------------------
// Tao layout hop thoai
// -------------------------------------------------------------------------
void KUiReWard::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_Notice.Init(&Ini, "Notice");
		
		BtnReWard1.Init(&Ini, "BtnReward1");	
		
		BtnReWard2.Init(&Ini, "BtnReward2");		
		
		BtnReWard3.Init(&Ini, "BtnReward3");

		m_CloseBtn.Init(&Ini, "CloseBtn");
		
		Ini.GetString("Main", "StringMoney", "", szStringMoney, sizeof(szStringMoney));
		Ini.GetString("Main", "StringExp", "", szStringExp, sizeof(szStringExp));	
		Ini.GetString("Main", "StringStone", "", szStringStone, sizeof(szStringStone));	
		Ini.GetString("Main", "StringRandom", "", szStringRandom, sizeof(szStringRandom));	
		Ini.GetString("Main", "StringEquip", "", szStringEquip, sizeof(szStringEquip));	
		Ini.GetString("Main", "StringMedicine", "", szStringMedicine, sizeof(szStringMedicine));	
		Ini.GetString("Main", "StringGold", "", szStringGold, sizeof(szStringGold));	
		Ini.GetString("Main", "StringViolet", "", szStringViolet, sizeof(szStringViolet));			
		Ini.GetString("Main", "StringRoll", "", szStringRoll, sizeof(szStringRoll));	

		Ini.GetString("Main", "ImgMoney", "", szImgMoney, sizeof(szImgMoney));
		Ini.GetString("Main", "ImgExp", "", szImgExp, sizeof(szImgExp));
		Ini.GetString("Main", "ImgStone", "", szImgStone, sizeof(szImgStone));	
		Ini.GetString("Main", "ImgRandom", "", szImgRandom, sizeof(szImgRandom));	
		Ini.GetString("Main", "ImgEquip", "", szImgEquip, sizeof(szImgEquip));	
		Ini.GetString("Main", "ImgMedicine", "", szImgMedicine, sizeof(szImgMedicine));
		Ini.GetString("Main", "ImgGold", "", szImgGold, sizeof(szImgGold));
		Ini.GetString("Main", "ImgViolet", "", szImgViolet, sizeof(szImgViolet));			
		Ini.GetString("Main", "ImgRoll", "", szImgRoll, sizeof(szImgRoll));			
	}
}

void KUiReWard::ShowButton()
{
	char m_BtnEx1[32], m_BtnEx2[32], m_BtnEx3[32];
	
	char*szBtnEx1 = strtok(szString1, "|");
	strcpy(m_BtnEx1, szBtnEx1);	
	m_Btn1 = atoi(m_BtnEx1);	
	char*szFuncEx1 = strtok(NULL, "|");
	strcpy(szFunc1, szFuncEx1);
	
	char*szBtnEx2 = strtok(szString2, "|");
	strcpy(m_BtnEx2, szBtnEx2);		
	m_Btn2 = atoi(m_BtnEx2);
	char*szFuncEx2 = strtok(NULL, "|");
	strcpy(szFunc2, szFuncEx2);
	
	char*szBtnEx3 = strtok(szString3, "|");
	strcpy(m_BtnEx3, szBtnEx3);		
	m_Btn3 = atoi(m_BtnEx3);
	char*szFuncEx3 = strtok(NULL, "|");
	strcpy(szFunc3, szFuncEx3);	
	if (m_Btn1 == 1)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgMoney);
		BtnReWard1.SetToolTipInfo(szStringMoney, sizeof(szStringMoney));		
	}
	else if (m_Btn1 == 2)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgExp);
		BtnReWard1.SetToolTipInfo(szStringExp, sizeof(szStringExp));
	}
	else if (m_Btn1 == 3)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgStone);
		BtnReWard1.SetToolTipInfo(szStringStone, sizeof(szStringStone));		
	}
	else if (m_Btn1 == 4)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgRandom);
		BtnReWard1.SetToolTipInfo(szStringRandom, sizeof(szStringRandom));		
	}
	else if (m_Btn1 == 5)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgEquip);
		BtnReWard1.SetToolTipInfo(szStringEquip, sizeof(szStringEquip));		
	}
	else if (m_Btn1 == 6)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgMedicine);
		BtnReWard1.SetToolTipInfo(szStringMedicine, sizeof(szStringMedicine));		
	}
	else if (m_Btn1 == 7)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgGold);
		BtnReWard1.SetToolTipInfo(szStringGold, sizeof(szStringGold));		
	}
	else if (m_Btn1 == 8)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgViolet);
		BtnReWard1.SetToolTipInfo(szStringViolet, sizeof(szStringViolet));
	}
	else if (m_Btn1 == 9)
	{
		BtnReWard1.SetImage(ISI_T_SPR, szImgRoll);
		BtnReWard1.SetToolTipInfo(szStringRoll, sizeof(szStringRoll));
	}

	if (m_Btn2 == 1)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgMoney);
		BtnReWard2.SetToolTipInfo(szStringMoney, sizeof(szStringMoney));		
	}
	else if (m_Btn2 == 2)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgExp);
		BtnReWard2.SetToolTipInfo(szStringExp, sizeof(szStringExp));	
	}
	else if (m_Btn2 == 3)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgStone);
		BtnReWard2.SetToolTipInfo(szStringStone, sizeof(szStringStone));		
	}
	else if (m_Btn2 == 4)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgRandom);
		BtnReWard2.SetToolTipInfo(szStringRandom, sizeof(szStringRandom));		
	}
	else if (m_Btn2 == 5)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgEquip);
		BtnReWard2.SetToolTipInfo(szStringEquip, sizeof(szStringEquip));		
	}
	else if (m_Btn2 == 6)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgMedicine);
		BtnReWard2.SetToolTipInfo(szStringMedicine, sizeof(szStringMedicine));		
	}
	else if (m_Btn2 == 7)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgGold);
		BtnReWard2.SetToolTipInfo(szStringGold, sizeof(szStringGold));		
	}
	else if (m_Btn2 == 8)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgViolet);
		BtnReWard2.SetToolTipInfo(szStringViolet, sizeof(szStringViolet));
	}
	else if (m_Btn2 == 9)
	{
		BtnReWard2.SetImage(ISI_T_SPR, szImgRoll);
		BtnReWard2.SetToolTipInfo(szStringRoll, sizeof(szStringRoll));
	}	

	if (m_Btn3 == 1)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgMoney);
		BtnReWard3.SetToolTipInfo(szStringMoney, sizeof(szStringMoney));		
	}
	else if (m_Btn3 == 2)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgExp);
		BtnReWard3.SetToolTipInfo(szStringExp, sizeof(szStringExp));
	}
	else if (m_Btn3 == 3)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgStone);
		BtnReWard3.SetToolTipInfo(szStringStone, sizeof(szStringStone));		
	}
	else if (m_Btn3 == 4)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgRandom);
		BtnReWard3.SetToolTipInfo(szStringRandom, sizeof(szStringRandom));		
	}
	else if (m_Btn3 == 5)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgEquip);
		BtnReWard3.SetToolTipInfo(szStringEquip, sizeof(szStringEquip));		
	}
	else if (m_Btn3 == 6)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgMedicine);
		BtnReWard3.SetToolTipInfo(szStringMedicine, sizeof(szStringMedicine));		
	}
	else if (m_Btn3 == 7)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgGold);
		BtnReWard3.SetToolTipInfo(szStringGold, sizeof(szStringGold));		
	}
	else if (m_Btn3 == 8)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgViolet);
		BtnReWard3.SetToolTipInfo(szStringViolet, sizeof(szStringViolet));
	}	
	else if (m_Btn3 == 9)
	{
		BtnReWard3.SetImage(ISI_T_SPR, szImgRoll);
		BtnReWard3.SetToolTipInfo(szStringRoll, sizeof(szStringRoll));
	}		

}
// -------------------------------------------------------------------------
// Xu ly khi hop thoai dang mo
// -------------------------------------------------------------------------
int KUiReWard::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&BtnReWard1)
		{
			g_pCoreShell->OperationRequest(GOI_REWARD_CMD_SCRIPT, (unsigned int)m_pSelf->szFunc1, 0);
			CloseWindow(true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&BtnReWard2)
		{
			g_pCoreShell->OperationRequest(GOI_REWARD_CMD_SCRIPT, (unsigned int)m_pSelf->szFunc2, 0);
			CloseWindow(true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&BtnReWard3)
		{
			g_pCoreShell->OperationRequest(GOI_REWARD_CMD_SCRIPT, (unsigned int)m_pSelf->szFunc3, 0);
			CloseWindow(true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{
			CloseWindow(true);
		}		
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}


