#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiMsgSel2.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI	"UiMsgSel2.ini"
#define	PICTURE_INI	"\\Ui\\PictureList.ini"
#define	SECTION	"Picture"

KUiMsgSel2* KUiMsgSel2::m_pSelf = NULL;
	
KUiMsgSel2* KUiMsgSel2::OpenWindow(KUiQuestionAndAnswer* pContent)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMsgSel2;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show(pContent);
	}
	return m_pSelf;
}


void KUiMsgSel2::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_bAutoUp = false;
		m_pSelf->m_bAutoDown = false;

		Wnd_ReleaseExclusive((KWndWindow*)m_pSelf);
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();		
	}
}


int KUiMsgSel2::Initialize()
{	
	AddChild(&m_Image);
	AddChild(&m_MsgList);
	AddChild(&m_MsgScroll);
	AddChild(&m_MsgScrollList);

	m_MsgList.SetScrollbar(&m_MsgScroll);
	
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	return true;
}

void KUiMsgSel2::LoadScheme(const char* pScheme)
{
	if (m_pSelf == NULL)
		return;
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI, pScheme);
	if (Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Image.Init(&Ini, "Image");
		m_pSelf->m_MsgList.Init(&Ini, "MessageList");
		m_pSelf->m_MsgScroll.Init(&Ini, "Msg_Scroll");
		m_pSelf->m_MsgScrollList.Init(&Ini, "Select");

		m_pSelf->m_MsgList.Enable(false);
		
	}
}
int KUiMsgSel2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			int x = LOWORD(nParam) - m_nAbsoluteLeft;
			int y = HIWORD(nParam) - m_nAbsoluteTop;
			if (x >= 0 && x <= m_Width &&
				y >= 0 && y <= m_Height)
			{
				int nMLeft = 0;
				int nMTop = 0;
				m_MsgScrollList.GetMessageListBox()->GetPosition(&nMLeft, &nMTop);
				int nMWidth = 0;
				int nMHeight = 0;
				m_MsgScrollList.GetMessageListBox()->GetSize(&nMWidth, &nMHeight);
				if (x >= nMLeft && x <= nMLeft + nMWidth)
				{
					if (y <= nMTop && y >= nMTop - 50)
						m_bAutoUp = true;
					else
						m_bAutoUp = false;

					if (y >= nMTop + nMHeight && y <= nMTop + nMHeight + 50)
						m_bAutoDown = true;
					else
						m_bAutoDown = false;
				}
			}
		}
		break;
	case WND_N_LIST_ITEM_HIGHLIGHT:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nParam);
		else
		{
			ChangeCurSel(true);
			nRet = 1;
		}
		break;
	case WND_N_LIST_ITEM_ACTIVE:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			OnClickMsg(nParam);
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		if (uParam == (unsigned int)(KWndWindow*)&m_MsgScroll)
			m_MsgList.SetFirstShowLine(nParam);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}

void KUiMsgSel2::OnClickMsg(int nMsg)
{
	CloseWindow(false);
	m_MsgScrollList.GetMessageListBox()->Clear();
	g_pCoreShell->OperationRequest(GOI_QUESTION_CHOOSE, 0, nMsg);
}

void KUiMsgSel2::ChangeCurSel(bool bNext)
{
	int nRet = m_MsgScrollList.GetMessageListBox()->GetCurSel();
	if (bNext)
	{
		if (nRet < m_MsgScrollList.GetMessageListBox()->GetMsgCount() - 1)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet + 1);
	}
	else
	{
		if (nRet  > 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet - 1);
	}
}
void KUiMsgSel2::Show(KUiQuestionAndAnswer* pContent)
{
	if (pContent && pContent->AnswerCount >= 0)
	{	
		m_MsgScrollList.GetMessageListBox()->Clear();
		m_MsgList.Clear();
		m_MsgList.AddOneMessage(pContent->Question, pContent->QuestionLen);
		

		int i = 0;
		if (pContent->AnswerCount <= 0)
		{
			m_MsgScrollList.GetMessageListBox()->AddOneMessage("KÕt thóc ®èi tho¹i.", -1);
			InitImage(pContent->Answer[0].AnswerText);
		}
		else
		{
			for (i = 0; i < pContent->AnswerCount ; i++)
			{
				if (!m_MsgScrollList.GetMessageListBox()->AddOneMessage(pContent->Answer[i].AnswerText, pContent->Answer[i].AnswerLen))
					m_MsgScrollList.GetMessageListBox()->AddOneMessage("[// --Script xö lý lçi// --]", -1);
			}
			InitImage(pContent->Answer[pContent->AnswerCount].AnswerText);
		}
		
	}
	KWndShowAnimate::Show();
	Wnd_SetExclusive((KWndWindow*)this);

	m_uLastScrollTime = IR_GetCurrentTime();
	m_bAutoUp = false;
	m_bAutoDown = false;
}
void KUiMsgSel2::Breathe()
{
	if (m_bAutoUp)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(false);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}

	if (m_bAutoDown)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(true);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}

	if(m_nImgNumFrames > 1)
	{	
		if(m_Image.GetCurFrame() >= m_nImgNumFrames)
			m_Image.SetFrame(0);
		m_Image.NextFrame();
		
	}
}
void KUiMsgSel2::InitImage(const char* pszKey)
{	
	m_Image.SetImage(ISI_T_SPR, "");
	m_nImgNumFrames = 0;

	if(!pszKey[0])
		return;
		
	char* pNewChar = NULL;
	int nPos[2];
	memset(nPos, 0, sizeof(int) * 2);
	for(int i = 0; i < 2; i++)
	{
		pNewChar = strstr(pszKey, "/");
		if(pNewChar)
		{
			*pNewChar = 0;
			nPos[i] = atoi(pszKey);
			pszKey = pNewChar + 1;
		}
		else
		{
			break;
		}
	}
	if(pszKey)
	{	
		KIniFile Ini;
		if(Ini.Load(PICTURE_INI))
		{
			char szImage[80];
			Ini.GetString(SECTION, pszKey, "", szImage, sizeof(szImage));
			if(szImage[0])
			{
				m_Image.SetImage(ISI_T_SPR, szImage);
				m_Image.SetPosition(nPos[0], nPos[1]);
				m_nImgNumFrames = m_Image.GetNumFrames();
				if(m_nImgNumFrames > 10)
					m_nImgNumFrames = 10;
				m_Image.SetFrame(0);
			}
		}
	}
}