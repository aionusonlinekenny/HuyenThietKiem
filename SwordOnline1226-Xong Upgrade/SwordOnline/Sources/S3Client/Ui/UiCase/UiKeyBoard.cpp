/*****************************************************************************************
//	界面// --储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Kinnox
//	CreateTime:	03.06.2021
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiKeyBoard.h"
#include "../../../core/src/coreshell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../core/src/GameDataDef.h"
#include "../../../Engine/src/KDebug.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiInformation.h"
#include <cstdlib> // calculation random
#include <ctime> // get real time
#include <iostream> // library covert num
#include <sstream>	// library covert num   
#include <crtdbg.h>
#include "KEngine.h"


extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;


#define SCHEME_INI	"UiKeyBoard.ini"

KUiKeyBoard* KUiKeyBoard::m_pSelf = NULL;

KUiKeyBoard::KUiKeyBoard()
{
	m_pRequester	= NULL;
}


KUiKeyBoard* KUiKeyBoard::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiKeyBoard* KUiKeyBoard::OpenWindow(KWndWindow* pRequester, unsigned int uParam)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiKeyBoard;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(false);
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_pRequester	= pRequester;
		m_pSelf->m_uRequesterParam = uParam;
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiKeyBoard::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -
// 功能	: 初始化
// -
void KUiKeyBoard::Initialize()
{

	AddChild(&m_InputEdit);
	AddChild(&m_ReInputBtn);
	AddChild(&m_CloseBtn);
	AddChild(&m_ConfirmBtn);
	
	for (int i = 0; i < MAX_NUMBER_KEY; i++)
	{
		AddChild(&m_Number[i]);
	}
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

// -
// 功能	: 载入界面方案
// -
void KUiKeyBoard::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	//RanDom number
	int RAN_ARRAY[10][10] = {
	{0,1,2,3,4,5,6,7,8,9},{1,6,9,5,8,7,3,0,4,2},
	{2,6,4,9,3,5,1,0,7,8},{3,5,9,0,8,2,1,4,6,7},
	{4,2,0,1,6,7,3,9,5,8},{5,3,1,6,0,2,8,7,4,9},
	{6,2,1,3,8,5,4,7,0,9},{7,3,6,9,0,8,4,5,2,1},
	{8,9,6,7,2,5,1,3,0,4},{9,6,1,3,7,5,4,2,0,8},	
	};
	int ArrRayNumber[10];
	srand(time(NULL));
	int a = rand() % 6 +0;
	ArrRayNumber[1] = RAN_ARRAY[a][0];
	ArrRayNumber[2] = RAN_ARRAY[a][1];
	ArrRayNumber[3] = RAN_ARRAY[a][2];
	ArrRayNumber[4] = RAN_ARRAY[a][3];
	ArrRayNumber[5] = RAN_ARRAY[a][4];
	ArrRayNumber[6] = RAN_ARRAY[a][5];
	ArrRayNumber[7] = RAN_ARRAY[a][6];
	ArrRayNumber[8] = RAN_ARRAY[a][7];
	ArrRayNumber[9] = RAN_ARRAY[a][8];
	ArrRayNumber[10] = RAN_ARRAY[a][9];
	//
	
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");	
		char szKey[16],szKeyName[16], szTemp[128];
		for (int i = 0; i < MAX_NUMBER_KEY ;i++)
		{
			sprintf(szKey, "Pos%d", i);
			m_pSelf->m_Number[i].Init(&Ini,szKey);
			m_pSelf->m_Value[i] = ArrRayNumber[i+1];
			sprintf(szKeyName, "Number_%d_Image",ArrRayNumber[i+1]);	
			Ini.GetString("NumberList", szKeyName, "",  szTemp, sizeof(szTemp));	
			m_pSelf->m_Number[i].SetImage(ISI_T_SPR, szTemp, true);
		}
		m_pSelf->m_InputEdit.Init(&Ini, "InputEdit");
		m_pSelf->m_ReInputBtn.Init(&Ini, "ReInPutBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ConfirmBtn.Init(&Ini, "ConfirmBtn");
	}
}

// -
// 功能	: 窗口函数
// -
int KUiKeyBoard::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ConfirmBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_ReInputBtn)
		{	m_InputEdit.SetText("");
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_0])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[0]);
			m_InputEdit.InsertString(szBuff,1);
			
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_1])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[1]);
			m_InputEdit.InsertString(szBuff,1);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_2])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[2]);
			m_InputEdit.InsertString(szBuff,1);
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_3])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[3]);
			m_InputEdit.InsertString(szBuff,1);
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_4])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[4]);
			m_InputEdit.InsertString(szBuff,1);
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_5])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[5]);
			m_InputEdit.InsertString(szBuff,1);
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_6])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[6]);
			m_InputEdit.InsertString(szBuff,1);
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_7])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[7]);
			m_InputEdit.InsertString(szBuff,1);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_8])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[8]);
			m_InputEdit.InsertString(szBuff,1);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Number[NUMBER_9])
		{
			char szBuff[6];
			sprintf(szBuff,"%d",m_Value[9]);
			m_InputEdit.InsertString(szBuff,1);
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_InputEdit)
		{
			OnOk();
			return 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiKeyBoard::OnOk()
{	
	OnCheckInput();
	
	if (m_pRequester)
	{
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, m_NumFinal);
	}
	CloseWindow();

}

void KUiKeyBoard::OnCancel()
{
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, 0);
	CloseWindow();
}

using namespace std;
void KUiKeyBoard::OnCheckInput()
{
	int m_MaxNumber = 999999;
	char	szBuff[16];
	m_InputEdit.GetText(szBuff, sizeof(szBuff), true);
	string CoverNum = szBuff;
	stringstream geek(CoverNum);
	geek >> m_NumFinal;
	if (m_NumFinal < 0)
		m_NumFinal = 0;
	else if (m_NumFinal > m_MaxNumber)
		m_NumFinal = m_MaxNumber;	
}

