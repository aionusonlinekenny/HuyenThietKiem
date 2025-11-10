
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "../Elem/MouseHover.h"
#include <crtdbg.h>
#include "UiPlayerBar.h"
#include "UiStatus.h"
#include "UiSkillTree.h"
#include "UiEscDlg.h"
#include "UiSelColor.h"
#include "UiMsgCentrePad.h"
#include "UiFaceSelector.h"
#include "UiMiniMap.h"
#include "UiHelper.h"
#include "UiShop.h"
#include "UiMarket.h"
#include "UiGetString.h"
#include "UiGetNumber.h"
#include "UiTongSetTax.h"
#include "UiSysMsgCentre.h"
#include "UiTeamStatus.h"
#include "UiTaskNote.h"
#include "UiHelperMission.h"
#include "UiAutoPlay.h"//AutoAI by kinnox;
#include "UiMiniBuff.h"//AutoAI by kinnox;
#include "../TrayMode.h" // an hien windows by kinnox;
#include "../ShortcutKey.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../core/src/MsgGenreDef.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../../TextCtrlCmd/TextCtrlCmd.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Engine/src/Text.h"
#include "../../../engine/src/kstrbase.h"

#include "../../../Headers/KRelayProtocol.h"
#include "../../../Headers/KProtocolDef.h"
#include "../../../Headers/KProtocol.h"
#include "malloc.h"
#include "UiChatCentre.h"
#include "../UiChatPhrase.h"
#include "../../Login/Login.h"
#ifdef _DEBUG
#include "KEngine.h"
#endif
#include <crtdbg.h>
#include <io.h>
extern KUiChatPhrase g_UiChatPhrase;

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI			"UiPlayerBar.ini"
#define GAME_LOGO			"Vâ L©m TruyÒn Kú"
#define	SWITCH_LOGO_INTERVAL	5000

#define		SEL_CHANNEL_MENU		1
#define		SEL_PHRASE_MENU			2
#define		SEL_PK_MENU				3

// --
//
// --
const char*		s_TimeName[12] =
{
	"Nöa ®ªm",
	"Gµ g¸y",
	"R¹ng s¸ng",
	"MÆt trêi mäc",
	"Khi ¨n",
	"Gãc trong",
	"Buæi tr­a",
	"XÕ chiÒu",
	"Giê th©n",
	"NhËp ngµy",
	"Hoµng h«n",
	"Nh©n ®Þnh",
};

// --
//
// --
const char*		s_TimeStone[12] = 
{
	"TÝ.",
	"Söu",
	"DÇn",
	"M·o",
	"Th×n",
	"TÞ.",
	"Ngä.",
	"Mïi",
	"Th©n",
	"DËu",
	"TuÊt",
	"Hîi"
};

// --
//
// --
const char*		s_TimeLine[4] = { "Mét", "Hai", "Ba", "Bèn" };

// --
//
// --
int GetFormatedTimeString(int nTime, char* pString)
{
	nTime += 60;
	return sprintf(pString, "Thêi %s %s Kh¾c %s!",
		s_TimeName[(nTime / 120) % 12], s_TimeStone[(nTime / 120) % 12],
		s_TimeLine[(nTime % 120) / 30]);
}

#include "time.h"
// --
//
// --
int GetFormatedTimeString(struct tm* ptm, char* pString)
{
	return sprintf(pString, "%02d:%02d | %d-%d-%d",
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_mday,	
			ptm->tm_mon + 1,
			ptm->tm_year + 1900);
}

GameWorld_DateTime::GameWorld_DateTime()
{
	m_bTrueWorld = true;
	m_bLogo    = false;
	m_uLastSwitchTime = IR_GetCurrentTime();
}

IMPLEMENT_COMCLASS(GameWorld_DateTime)
void GameWorld_DateTime::OnButtonClick()
{
	m_bTrueWorld = !m_bTrueWorld;
	UpdateData();
}

int	GameWorld_DateTime::Init(KIniFile* pIniFile, const char* pSection)
{
	if(KWndLabeledButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "SmoothPing", 0, &m_nSmoothPing);
		pIniFile->GetInteger(pSection, "CrowdPing", 0, &m_nCrowdPing);
		pIniFile->GetInteger(pSection, "BlockPing", 0, &m_nBlockPing);

		pIniFile->GetString(pSection, "SmoothMsg", "", m_szSmoothMsg, sizeof(m_szSmoothMsg));
		pIniFile->GetString(pSection, "CrowdMsg", "", m_szCrowdMsg, sizeof(m_szCrowdMsg));
		pIniFile->GetString(pSection, "BlockMsg", "", m_szBlockMsg, sizeof(m_szBlockMsg));
		KIniFile* pSetting = g_UiBase.GetCommSettingFile();
		pSetting->GetString("Login", "LastGameServer", GAME_LOGO, m_szLogo, sizeof(m_szLogo));

		char Buff[16];
		pIniFile->GetString(pSection, "Color", "0,0,0", Buff, sizeof(Buff));
		m_uDefaultColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SmoothColor", "0,0,0", Buff, sizeof(Buff));
		m_uSmoothColor = GetColor(Buff);
		pIniFile->GetString(pSection, "CrowdColor", "0,0,0", Buff, sizeof(Buff));
		m_uCrowdColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BlockColor", "0,0,0", Buff, sizeof(Buff));
		m_uBlockColor = GetColor(Buff);
	}
	return 1;
}

void GameWorld_DateTime::UpdateData()
{
	char szTime[64];
	if(!m_bLogo && !m_bNetStatus)
	{
        if (!m_bTrueWorld && g_pCoreShell)
	    {
    		KUiSceneTimeInfo Info;
		    memset(&Info, 0, sizeof(KUiSceneTimeInfo));
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0);
		    int nLen = GetFormatedTimeString(Info.nGameSpaceTime, szTime);
	    }
	    else
	    {		
    		time_t curtm = ::time(NULL);
		    struct tm* ptm = localtime(&curtm);
		    int nLen = GetFormatedTimeString(ptm, szTime);
	    }
		SetLabelColor(m_uDefaultColor);
		SetLabel(szTime);
	}

	if (IR_IsTimePassed(SWITCH_LOGO_INTERVAL, m_uLastSwitchTime))
	{
		if(m_bLogo)
		{
			m_bLogo = false;
			UpdateNetStatus();
			m_bNetStatus = true;
		}
		else if(m_bNetStatus)
		{
			m_bNetStatus = false;
		}
		else
		{
			SetLabelColor(m_uDefaultColor);
			SetLabel(m_szLogo);
			m_bLogo = true;
		}
	}

	if (m_Flag & WNDBTN_F_OVER)
	{
		m_bLogo = false;
		m_bNetStatus = false;
		m_uLastSwitchTime = IR_GetCurrentTime();
	}

	NextFrame();
}

void GameWorld_DateTime::UpdateNetStatus()
{
	int i, nPing;
	char Number[32], Buff[32];
	unsigned int uDefaultColor;

	if(g_pCoreShell) nPing = g_pCoreShell->GetPing();
	_itoa(nPing, Number, 10);

	if(nPing >= 0 && nPing <= m_nSmoothPing)
	{
		i = strlen(m_szSmoothMsg);
		memcpy(Buff, m_szSmoothMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uSmoothColor;
	}
	else if(nPing > m_nSmoothPing && nPing <= m_nCrowdPing)
	{
		i = strlen(m_szCrowdMsg);
		memcpy(Buff, m_szCrowdMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uCrowdColor;
	}
	else
	{
		i = strlen(m_szBlockMsg);
		memcpy(Buff, m_szBlockMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uBlockColor;
	}
	SetLabelColor(uDefaultColor);
	SetLabel(Buff);
}

int	KImmediaItem::Init(KIniFile* pIniFile, const char* pSection)
{
	int nRet = KWndObjectBox::Init(pIniFile, pSection);
	if (nRet)
	{
		char ChName[64];
		pIniFile->GetString(pSection, "TextColor", "255,255,0", ChName, sizeof(ChName));
		m_TextColor = GetColor(ChName);
		return true;
	}
	return nRet;
}

void KImmediaItem::PaintWindow()
{
	KWndObjectBox::PaintWindow();
	if (g_pCoreShell)
	{
		KUiDraggedObject Obj;
		GetObject(Obj);
		if (Obj.uGenre == CGOG_SKILL_CUSTOM && Obj.uId)
		{
			char szNum[4];
			sprintf(szNum, "%d", m_nIndex + 1);
			g_pRepresentShell->OutputText(NORMAL_FONTSIZE, szNum, KRF_ZERO_END,
				m_nAbsoluteLeft,
				m_nAbsoluteTop, m_TextColor);
		}


		int nNum = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, m_nIndex, 0);
		if (nNum > 1 && nNum < 1000)
		{
			int nFontSize = 12;
			char szNum[4];
			int nLen = sprintf(szNum, "%d", nNum);
			szNum[3] = 0;
			g_pRepresentShell->OutputText(nFontSize, szNum, KRF_ZERO_END,
				m_nAbsoluteLeft + m_Width - nLen * (nFontSize + 1) / 2,
				m_nAbsoluteTop + m_Height - nFontSize - 1, m_TextColor);
		}
	}
}
KUiPlayerBar* KUiPlayerBar::m_pSelf = NULL;

KUiPlayerBar::KUiPlayerBar()
{
	m_cPreMsgCounter = 0;
	m_cLatestMsgIndex= 0;
	for (int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
		m_RecentMsg[i][0] = 0;
	m_szSelfName[0] = 0;
	m_nCurChannel = -1;
	m_nRecentPlayerName = 0;
	memset(m_RecentPlayerName, 0, sizeof(m_RecentPlayerName));
	m_nExperienceFull = 0;
	m_nCurLevelExperience = 0;
	m_nExperience = 0;
	memset(m_szChatItemName, 0, sizeof(m_szChatItemName));
	memset(m_szChatItemInfo, 0, sizeof(m_szChatItemInfo));
}

KUiPlayerBar::~KUiPlayerBar()
{
}


KUiPlayerBar* KUiPlayerBar::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiPlayerBar* KUiPlayerBar::OpenWindow()
{
	KUiChatCentre::OpenWindow(false);

	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPlayerBar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->UpdateData();
		m_pSelf->Show();
	}
	m_pSelf->m_nCurChannel = -1;
	m_pSelf->m_nRecentPlayerName = 0;

	return m_pSelf;
}

void KUiPlayerBar::UpdateData()
{
	//_ASSERT(g_pCoreShell);
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	strcpy(m_szSelfName, Info.Name);

	int nA;
	int nA2;
	UpdateXXXNumber(nA, nA2);

	KUiPlayerImmedItemSkill immedItemSkillInfo;
	memset(&immedItemSkillInfo, 0, sizeof(KUiPlayerImmedItemSkill));
	g_pCoreShell->GetGameData(GDI_PLAYER_IMMED_ITEMSKILL, (int)&immedItemSkillInfo, 0);
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].HoldObject(CGOG_IME_ITEM/*immedItemSkillInfo.ImmediaItem[i].uGenre*/,
			immedItemSkillInfo.ImmediaItem[i].uId, 0, 0);
	}
	
	m_ImmediaSkill[0].HoldObject(immedItemSkillInfo.IMmediaSkill[0].uGenre,
		immedItemSkillInfo.IMmediaSkill[0].uId, 0, 0);
	m_ImmediaSkill[1].HoldObject(immedItemSkillInfo.IMmediaSkill[1].uGenre,
		immedItemSkillInfo.IMmediaSkill[1].uId, 0, 0);

	int nM;
	int nL;
	UpdateRuntimeAttribute(nM, nL);
}


void KUiPlayerBar::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_cLatestMsgIndex = 0;
		m_pSelf->m_cPreMsgCounter = 0;
		for(int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
			m_pSelf->m_RecentMsg[i][0] = 0;

		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

void KUiPlayerBar::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s" , pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini, pScheme);
	}
}

unsigned int s_CanFocusColor = 0;
unsigned int s_NotCanFocusColor = 0;

void KUiPlayerBar::LoadScheme(KIniFile* pIni, const char* pScheme)
{
	Init(pIni, $Main);
	
	m_Face.Init(pIni, "Face");
	m_Friend.Init(pIni, "Friend");
	m_Options.Init(pIni, "Options");
	char szBuffer[32], szBuffer2[32];
	KIniFile	cFile;
	pIni->GetString("Main", "ToolBox", "", szBuffer, sizeof(szBuffer));
	if(szBuffer[0])
	{
		sprintf(szBuffer2, "%s\\%s" , pScheme, szBuffer);
		if(cFile.Load(szBuffer2))
		{
			m_ToolBox.Init(&cFile, "Main");
		}
	}
	m_Market.Init(pIni, "Market");
	m_DateTime.Init(pIni, "DateTime");
	BYTE i = 0;
	for (i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		sprintf(szBuffer, "Item_%d", i);
		m_pSelf->m_ImmediaItem[i].Init(pIni, szBuffer);
		m_pSelf->m_ImmediaItem[i].m_nIndex = i;
	}
	m_ImmediaSkill[0].Init(pIni, "ImediaLeftSkill");
	m_ImmediaSkill[1].Init(pIni, "ImediaRightSkill");
	m_ImmediaSkill[0].EnablePickPut(false);
	m_ImmediaSkill[1].EnablePickPut(false);

	m_InputEdit	.Init(pIni, "InputEdit");
	s_CanFocusColor = m_InputEdit.GetFocusBkColor();

	if (pIni->GetString("InputEdit", "FocusNoCanBKColor", "0,0,0", szBuffer, sizeof(szBuffer)))
	{
		s_NotCanFocusColor = (GetColor(szBuffer) & 0xffffff);
		int	nValue;
		if (pIni->GetInteger("InputEdit", "FocusNoCanBKColorAlpha", 128, &nValue))
		{
			if (nValue > 255 || nValue < 0)
				s_NotCanFocusColor |= 0x16000000;
			else
				s_NotCanFocusColor |= (((255 - nValue) << 21) & 0xff000000);
		}
	}
	else
	{
		KRColor nColor;
		nColor.Color_dw = s_CanFocusColor;
		nColor.Color_b.a = 12;
		nColor.Color_b.r = 108;
		s_NotCanFocusColor = nColor.Color_dw;
	}

	m_SendBtn	.Init(pIni, "SendBtn");
	m_ChannelSwitchBtn.Init(pIni, "ChannelBtn");
	m_ChannelOpenBtn.Init(pIni, "OpenChannelBtn");
	m_InputBack.Init(pIni, "InputBack");
	for (i = 0; i < defMAX_CHATITEM_BUTTON; i++)
	{	
		m_ChatItemBtn[i].SetPosition(0,0);
		m_ChatItemBtn[i].Hide();
	}
	m_HideWindow	.Init(pIni, "HideWindow");// an hien windows by kinnox;
	m_HideMiniBuff	.Init(pIni, "HideButton");
	m_HideMiniBuff.Hide();
	m_ShowMiniBuff	.Init(pIni, "ShowButton");
	//m_ShowMiniBuff.Hide();
	//
	for (int j = 0; j < MAX_NUM_BUTTERFLY; j++)
		{
			m_NoteQuest.Init(pIni, "NoteQuest");
			
			m_uAdviceIntervalMin[j] = m_uAdviceIntervalMax[j] = 0;
			pIni->GetInteger2("NoteQuest", "Interval", (int*)&m_uAdviceIntervalMin[j],
				(int*)&m_uAdviceIntervalMax[j]);
			if (m_uAdviceIntervalMin[j] && m_uAdviceIntervalMax[j] < m_uAdviceIntervalMin[j])
			{
				m_uAdviceIntervalMax[j] = m_uAdviceIntervalMin[j];
			}

			m_uInterval[j] = 0;
			//------
			m_HelpMission.Init(pIni, "HelpMission");
			m_uAdviceIntervalMin_1[j] = m_uAdviceIntervalMax_1[j] = 0;
			pIni->GetInteger2("HelpMission", "Interval", (int*)&m_uAdviceIntervalMin_1[j],
				(int*)&m_uAdviceIntervalMax_1[j]);
			if (m_uAdviceIntervalMin_1[j] && m_uAdviceIntervalMax_1[j] < m_uAdviceIntervalMin_1[j])
			{
				m_uAdviceIntervalMax_1[j] = m_uAdviceIntervalMin_1[j];
			}
			m_uInterval_1[j] = 0;
		}
	//
}

void KUiPlayerBar::Initialize()
{	
	AddChild(&m_InputBack);
	AddChild(&m_DateTime);
	AddChild(&m_Face);
	AddChild(&m_Friend);
	AddChild(&m_Options);
	AddChild(&m_ToolBox);
	AddChild(&m_Market);
	AddChild(&m_NoteQuest);
	AddChild(&m_HelpMission);
	AddChild(&m_HideWindow);// an hien windows by kinnox;	
	AddChild(&m_HideMiniBuff);// MiniBuff by kinnox;	
	AddChild(&m_ShowMiniBuff);// MiniBuff by kinnox;	
	//
	for (int z = 0; z < MAX_NUM_BUTTERFLY; z++)
	{
		m_uAdviceIntervalMin[z] = 0;
		m_uInterval[z] = 0;
		m_uLastSleepTime[z] = 0;

		m_uAdviceIntervalMin_1[z] = 0;
		m_uInterval_1[z] = 0;
		m_uLastSleepTime_1[z] = 0;
	}
	//

	int i = 0;
	for (i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].SetObjectGenre(CGOG_ITEM);
		m_ImmediaItem[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
		AddChild(&m_ImmediaItem[i]);
		m_ImmediaItem[i].SetContainerId((int)UOC_IMMEDIA_ITEM);
	}
	AddChild(&m_ImmediaSkill[0]);
	AddChild(&m_ImmediaSkill[1]);
	m_ImmediaSkill[0].SetContainerId((int)UOC_IMMEDIA_SKILL);
	m_ImmediaSkill[1].SetContainerId((int)UOC_IMMEDIA_SKILL);
	AddChild(&m_InputEdit);
	AddChild(&m_ChannelSwitchBtn);
	AddChild(&m_SendBtn);
	AddChild(&m_ChannelOpenBtn);
	
	for (i =0; i < defMAX_CHATITEM_BUTTON; i++)
	{
		AddChild(&m_ChatItemBtn[i]);
	}
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	m_pSelf->LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

int KUiPlayerBar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_M_POPUPMENU:
		if (uParam == (unsigned int)(KWndWindow*)(&m_InputEdit))
		{
			PopupPhraseMenu(LOWORD(nParam), HIWORD(nParam), true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		break;
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnObjPickedDropped((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)(KWndWindow*)&m_Friend)
		{
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_FRIEND);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_Options)
		{
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SYSTEM);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_Face)
		{
			KUiFaceSelector::OpenWindow(this, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SendBtn)
			OnSend(false);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Market)
		{	
			if(KUiShop::GetIfVisible() != NULL)
				break;
			if(KUiGetString::GetIfVisible() != NULL)
				break;
			if(KUiGetNumber::GetIfVisible() != NULL)
				break;
			if(KUiTongSetTax::GetIfVisible() != NULL)
				break;
			if(g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0))
				break;
			char	szFunc[32];
			sprintf(szFunc, "OpenMarket|%d", g_pCoreShell->GetGameData(GDI_MARKET_SELLID, 0, 0));
			g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 1);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_HideWindow)
		{
			gTrayMode.ShowNotify(true);// an hien windows by kinnox;
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ShowMiniBuff)
		{
			m_ShowMiniBuff.Hide();
			m_HideMiniBuff.Show();
			UiMiniBuff::OpenWindow();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_HideMiniBuff)
		{
			m_HideMiniBuff.Hide();
			m_ShowMiniBuff.Show();
			UiMiniBuff::CloseWindow(); 
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_NoteQuest)
		{
			KUiTaskNote::OpenWindow();	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_HelpMission)
		{
			KUiHelperMission::OpenWindow(true);	
		}
		break;
	case WND_N_LEFT_CLICK_ITEM:
		if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[0])
			KUiSkillTree::OpenWindow(true);
		else if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[1])
			KUiSkillTree::OpenWindow(false);
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[0] || 
			nParam == (int)(KWndWindow*)&m_ImmediaSkill[1])
		{
			KUiDraggedObject* pObj = (KUiDraggedObject*)uParam;
			if (pObj->uGenre != CGOG_NOTHING)
			{
				KUiDraggedObject Obj;
				if (Wnd_GetDragObj(&Obj))
				{
					if (Obj.uGenre != CGOG_NOTHING && Obj.uGenre != CGOG_SKILL_CUSTOM)
						return 0;
				}
				
				if (!g_pCoreShell->CanGetSkill(pObj->uId))
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_NONE;
					Msg.byParamSize = 0;
					Msg.byPriority = 0;
					Msg.eType = SMT_NORMAL;
					Msg.uReservedForUi = 0;
					strcpy(Msg.szMessage, "Kh«ng thÓ lÊy kü n¨ng nµy ra!");
					KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
					return 0;
				}
				Wnd_DragFinished();
				Obj.uGenre = CGOG_SKILL_CUSTOM;
				Obj.uId = pObj->uId;
				Obj.DataW = 1;
				Obj.DataH = 1;			
				Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
			}
		}
		else if (g_UiBase.IsOperationEnable(UIS_O_USE_ITEM))
		{
			for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
			{
				if (nParam == (int)(KWndWindow*)&m_ImmediaItem[i])
					OnUseItem(i);
			}
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN)
		{
			OnSend((GetKeyState(VK_CONTROL) & 0x8000));
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		else if (nParam == VK_UP || nParam == VK_DOWN)
		{
			if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				int x, y;
				m_InputEdit.GetAbsolutePos(&x, &y);
				int nCX = 0;
				m_InputEdit.GetCaretPosition(&nCX, NULL);
				PopupPhraseMenu(x + nCX, y, nParam == VK_DOWN);
			}
			else
			{
				InputRecentMsg(nParam == VK_UP);
				nRet = 1;
			}
		}
		else if (nParam == VK_PRIOR || nParam == VK_NEXT)
		{
			if (m_nCurChannel >= 0)
			{
				int nToChannel;
				if (nParam == VK_PRIOR)
					nToChannel = (m_nCurChannel - 1 >= 0) ? m_nCurChannel - 1 : KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName - 1;
				else
					nToChannel = (DWORD(m_nCurChannel + 1) < (KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName)) ? m_nCurChannel + 1 : 0;

				if (nToChannel != m_nCurChannel)
				{
					InputCurrentChannel(nToChannel, true);
				}
			}				
			nRet = 1;
		}
		else if (nParam == VK_ESCAPE)
		{
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		m_cPreMsgCounter = 0;
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					InputCurrentChannel(LOWORD(nParam), true);
				}

				KPopupMenu::Cancel();
			}
			else if (HIWORD(nParam) == SEL_PHRASE_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					char szPhrase[64];
					int nLen = g_UiChatPhrase.GetPhrase(LOWORD(nParam), szPhrase);
					m_InputEdit.InsertString(szPhrase, nLen);
				}
			}
			else if (HIWORD(nParam) == SEL_PK_MENU)
			{
				if (BYTE(LOWORD(nParam)) >= 0)
				{
					OnChoosePKMenu((BYTE)nParam);
				}
			}
		}
		break;
	case WND_M_MENUITEMRIGHT_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					if (KUiMsgCentrePad::CheckChannel(LOWORD(nParam), !KUiMsgCentrePad::GetChannelSubscribe(LOWORD(nParam))))
					{
						KPopupMenuData* p = KPopupMenu::GetMenuData();
						if (p)
						{
							PopupChannelMenu(p->nXX, p->nYY);
						}
					}
					else
						KPopupMenu::Cancel();
				}
				else
					KPopupMenu::Cancel();
			}
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (nParam)
		{
			m_InputEdit.InsertString((const char*)nParam, -1);
			Wnd_SetFocusWnd(&m_InputEdit);
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiPlayerBar::PaintWindow()
{
	KWndImage::PaintWindow();

	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName;
	if (m_nCurChannel < 0 || m_nCurChannel >= nChannelDataCount)
	{
		SetCurrentChannel(0);
	}
	KUiDraggedObject Obj;
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{	
		
		memset(&Obj, 0, sizeof(KUiDraggedObject));
		m_ImmediaItem[i].GetObject(Obj);

		if(Obj.uId > 0)
		{
			int nFontSize = 14;
			char szNum[2];
			int nLen = sprintf(szNum, "%d", i + 1);
			szNum[1] = 0;
			int nLeft, nTop;
			m_ImmediaItem[i].GetPosition(&nLeft, &nTop);
			nLeft += 1;
			nTop -= 3;
			g_pRepresentShell->OutputText(nFontSize, szNum, KRF_ZERO_END, nLeft, nTop, 0xffffffff);
		}
	}
}

void KUiPlayerBar::PopupChannelMenu(int x, int y)
{
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nChannelDataCount + m_nRecentPlayerName <= 0)
		return;

	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nChannelDataCount + m_nRecentPlayerName));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nChannelDataCount + m_nRecentPlayerName);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= (PM_F_AUTO_DEL_WHEN_HIDE | PM_F_CANCEL_BY_CALLER);

	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	pMenuData->nItemHeight = 0;
	WORD nHei = 0;
	char szText[64];
	for (i = 0; i < nChannelDataCount; i++)
	{
		WORD nPicIndex = -1;
		int nOffset = 0;
		KRColor uColor;
		KRColor uBKColor;
		short nCheckPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(i, &nPicIndex, &nHei, &uColor, &uBKColor, szText, &nCheckPicIndex))
		{
			pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
			*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
			pMenuData->Items[i].szData[3] = uColor.Color_b.a;
			pMenuData->Items[i].szData[4] = uColor.Color_b.r;
			pMenuData->Items[i].szData[5] = uColor.Color_b.g;
			pMenuData->Items[i].szData[6] = uColor.Color_b.b;
			nOffset = 7;
			if (nHei > pMenuData->nItemHeight)
				pMenuData->nItemHeight = nHei;
			strcpy(pMenuData->Items[i].szData + nOffset, szText);
			pMenuData->Items[i].uDataLen = strlen(szText) + nOffset;
			
			pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen++] = KTC_TAB;

			if (nCheckPicIndex != -1)
			{
				pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + pMenuData->Items[i].uDataLen + 1)) = nCheckPicIndex;
				pMenuData->Items[i].uDataLen += 3;
			}
		}
		pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		pMenuData->usMenuFlag |= PM_F_TAB_SPLIT_ITEM_TEXT;
	}
	if (nHei > 0)
        pMenuData->nItemRightWidth = MENU_ITEM_DEFAULT_RIGHT_WIDTH;
	else
		pMenuData->nItemRightWidth = 0;

	for (; i < nChannelDataCount + m_nRecentPlayerName; i++)
	{
		if (m_RecentPlayerName[i - nChannelDataCount][0] != 0)
		{
			WORD nPicIndex = -1;
			int nOffset = 0;
			KRColor uColor;
			KRColor uBKColor;
			if (KUiMsgCentrePad::GetPeopleMenuinfo(m_RecentPlayerName[i - nChannelDataCount], &nPicIndex, &nHei, &uColor, &uBKColor))
			{
				pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
				pMenuData->Items[i].szData[3] = uColor.Color_b.a;
				pMenuData->Items[i].szData[4] = uColor.Color_b.r;
				pMenuData->Items[i].szData[5] = uColor.Color_b.g;
				pMenuData->Items[i].szData[6] = uColor.Color_b.b;
				nOffset = 7;
				if (nHei > pMenuData->nItemHeight)
					pMenuData->nItemHeight = nHei;
			}
			strcpy(pMenuData->Items[i].szData + nOffset, m_RecentPlayerName[i - nChannelDataCount]);
			pMenuData->Items[i].uDataLen = strlen(m_RecentPlayerName[i - nChannelDataCount]) + nOffset;
			pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		}
		else
		{
			break;
		}
	}
	pMenuData->nNumItem = i;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = m_nCurChannel;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_CHANNEL_MENU);
}
void KUiPlayerBar::PopupPhraseMenu(int x, int y, bool bFirstItem)
{
	int nDataCount = g_UiChatPhrase.GetPhraseCount();
	if (nDataCount <= 0)
		return;
	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nDataCount));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nDataCount);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	for (i = 0; i < nDataCount; i++)
	{
		pMenuData->Items[i].uDataLen = g_UiChatPhrase.GetPhrase(i, pMenuData->Items[i].szData);
	}
	pMenuData->nNumItem = nDataCount;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = bFirstItem ? 0 : nDataCount - 1;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_PHRASE_MENU);
}

#include "../ChatFilter.h"
extern CChatFilter g_ChatFilter;
void KUiPlayerBar::OnSend(BOOL bDirectSend)
{
	char	Buffer[512];
	int     nName = 0;
	int	    nMsgLength = m_InputEdit.GetText(Buffer, sizeof(Buffer), true);
	if (nMsgLength <= 0)
		return;
	memcpy(m_RecentMsg[m_cLatestMsgIndex], Buffer, nMsgLength);
	m_RecentMsg[m_cLatestMsgIndex][nMsgLength] = 0;
	m_cLatestMsgIndex = (m_cLatestMsgIndex + 1) % MAX_RECENT_MSG_COUNT;
	m_cPreMsgCounter = 0;

	if (!bDirectSend)
	{
		if (TextMsgFilter(Buffer, nMsgLength))
		{
			m_InputEdit.ClearText();
			return;
		}
	}

	if (g_pCoreShell == NULL)
		return;

	int bChannel = false;
	int nDestChannel = -1;
	char Name[32];
	Name[0] = 0;
	if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX || Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX)
	{
		bChannel = (Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX);
		while (nName < nMsgLength)
		{
			if (Buffer[nName] == ' ')
			{
				Buffer[nName] = 0;
				nName++;
				break;
			}
			nName++;
		}

		if (bChannel)
		{
			ReplaceSpecialName(Name, 31, Buffer + 1);
			nDestChannel = GetChannelIndex(Name);
		}
		else
		{
			strncpy(Name, Buffer + 1, 31);
		}
		Name[31] = 0;
	}

	if (Name[0] == 0)
	{
		bChannel = true;
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		if (m_nCurChannel >= 0 && m_nCurChannel < nChannelDataCount)
		{
			nDestChannel = m_nCurChannel;
		}
		else
		{
			for (int n = 0; n < nChannelDataCount; n++)
			{
				if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_Screen))
				{
					nDestChannel = n;
					break;
				}
			}
		}

		if (nDestChannel >= 0)
			strncpy(Name, KUiMsgCentrePad::GetChannelTitle(nDestChannel), 31);
		
		Name[31] = 0;
	}
	else
	{
		Buffer[nName - 1] = ' ';
	}

	nMsgLength -= nName;
	
	if (bChannel && !KUiMsgCentrePad::IsChannelType(nDestChannel, KUiMsgCentrePad::ch_GM))
	{
		if (nMsgLength > 3 &&
			Buffer[nName] == '?' &&
			(Buffer[nName + 1] == 'g' || Buffer[nName + 1] == 'G') &&
			(Buffer[nName + 2] == 'm' || Buffer[nName + 2] == 'M'))
		return;
	}
	if (!g_ChatFilter.IsTextPass(Buffer + nName))
	{
		char szWarning[] = "Vui lßng kh«ng sö dông tõ ng÷ kh«ng thÝch hîp trong th­!"; 
		KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
		return;
	}

	char szEvent[1024];
	if (bChannel)
		sprintf(szEvent, APP_CHAT, Name, Buffer + nName);
	else
		sprintf(szEvent, APP_SAY, Name, Buffer + nName);
	szEvent[1023] = 0;

	if (g_UiBase.NotifyEvent(szEvent) == 0)
		return;
	char	Buffer2[1600];
	ZeroMemory(Buffer2, sizeof(Buffer2));
   	nMsgLength = KUiFaceSelector::ConvertFaceText(Buffer2, Buffer + nName, nMsgLength + 1); // loi chat cho nay bat buoc them +1 de khoi thieu chu;
	if(m_szChatItemName[0])
	{	
		char* pItemName = strstr(Buffer2, m_szChatItemName);
		char* pItemInfo	= strstr(Buffer2, m_szChatItemName);
		if(pItemName)
		{
			int nCheck = 0;
			while(1)
			{
				if(*pItemName == '>') break;
				nCheck++;
				pItemName++;
			}
			nCheck++;
			pItemName++;

			int nLen = strlen(m_szChatItemInfo);
			int nMove = nLen - nCheck;
			if(nMove + strlen(Buffer2) > 1600) 
				return;

			int nAfterLen = strlen(pItemName);
			if(nMove > 0)
				memmove(pItemName + nMove, pItemName, nAfterLen);
			memcpy(pItemInfo, m_szChatItemInfo, nLen);
			nMsgLength += nMove;
		}
	}
	nMsgLength = TEncodeText(Buffer2, nMsgLength); 
	if (bChannel)
	{
		DWORD nChannelID = KUiMsgCentrePad::GetChannelID(nDestChannel);
		if (nChannelID != -1)
		{
			KUiMsgCentrePad::CheckChannel(nDestChannel, true);
			OnSendChannelMessage(nChannelID, Buffer2, nMsgLength);

			m_InputEdit.ClearText();
			InputCurrentChannel(nDestChannel, true);
		}
	}
	else if (!IsSelfName(Name))
	{
		OnSendSomeoneMessage(Name, Buffer2, nMsgLength);
		
		int nAdd = AddRecentPlayer(Name);
		if (nAdd >= 0)
		{
			m_InputEdit.ClearText();
			InputCurrentChannel(nAdd, true);
		}
	}
}

BOOL KUiPlayerBar::IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID)
{
	if (m_pSelf)
	{
		if (Buffer && nLen > 0 && !g_ChatFilter.IsTextPass(Buffer))
			return FALSE;

		char szEvent[1024];
		if (nChannelID >= 0)
			sprintf(szEvent, APP_CHAT, szDestName, Buffer);
		else
			sprintf(szEvent, APP_SAY, szDestName, Buffer);

		szEvent[1023] = 0;

		if (g_UiBase.NotifyEvent(szEvent) == 0)
			return FALSE;
	}
	return TRUE;
}

void KUiPlayerBar::OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen)				
{
	if (nChannelID != -1 && Buffer && nLen > 0)
	{	
		int nLeft = KUiMsgCentrePad::PushChannelData(nChannelID, Buffer, nLen);
		if (nLeft < 0)
		{
			char szWarning[] = "Thiªn lý truyÒn ©m kh«ng c¸ch nµo sö dông liªn tôc, mêi b¹n lÊy h¬i chèc l¸t!";
			KUiMsgCentrePad::NewChannelMessageArrival(0, "HÖ thèng", szWarning, sizeof(szWarning));
		}
		else if (nLeft > 0)
		{
			char szWarning[64];
			sprintf(szWarning, "Th«ng b¸o sÏ ®­îc göi sau %d gi©y!", (nLeft + 999 ) / 1000);
			KUiMsgCentrePad::NewChannelMessageArrival(0, "HÖ thèng", szWarning, strlen(szWarning) + 1);
		}
	}
}

void KUiPlayerBar::OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen)				
{
	if (g_pCoreShell && nChannelID != -1 && Buffer && nLen > 0)
	{
		size_t chatsize = sizeof(CHAT_CHANNELCHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_CHANNELCHAT_CMD* pCccCmd = (CHAT_CHANNELCHAT_CMD*)(pExHeader + 1);
		pCccCmd->ProtocolType = chat_channelchat;
		pCccCmd->dwTimePacker = GetTickCount();
		pCccCmd->wSize = chatsize - 1;
		pCccCmd->packageID = -1;
		pCccCmd->filter = 1;
		pCccCmd->channelid = nChannelID;
		pCccCmd->cost = cost;
		pCccCmd->sentlen = nLen;
		memcpy(pCccCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}

void KUiPlayerBar::OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen)
{
	if (g_pCoreShell && Name && Name[0] != 0 && Buffer && nLen > 0 && m_pSelf)
	{
		size_t chatsize = sizeof(CHAT_SOMEONECHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_SOMEONECHAT_CMD* pCscCmd = (CHAT_SOMEONECHAT_CMD*)(pExHeader + 1);
		pCscCmd->ProtocolType = chat_someonechat;
		pCscCmd->dwTimePacker = GetTickCount();
		pCscCmd->wSize = chatsize - 1;
		pCscCmd->packageID = -1;
		strcpy(pCscCmd->someone, Name);
		pCscCmd->sentlen = nLen;
		memcpy(pCscCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}
void KUiPlayerBar::InputRecentMsg(bool bPrior)
{
	int nCounter;
	if (bPrior)
		nCounter = m_cPreMsgCounter - 1;
	else
		nCounter = m_cPreMsgCounter + 1;
	if (nCounter < 0 && nCounter >= - MAX_RECENT_MSG_COUNT)
	{
		int nIndex = m_cLatestMsgIndex + nCounter;
		if (nIndex < 0)
			nIndex += 8;
		if (m_RecentMsg[nIndex][0])
		{
			m_InputEdit.SetText(m_RecentMsg[nIndex]);
			m_cPreMsgCounter = nCounter;
		}
	}
	else if (nCounter >= 0)
	{
		m_InputEdit.ClearText();
		m_cPreMsgCounter = 0;
	}
}
void KUiPlayerBar::OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;
	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.v = 0;
		Pick.eContainer = UOC_IMMEDIA_ITEM;
		pWnd = pPickPos->pWnd;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	if (pDropPos)
	{
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.v = 0;
		Drop.eContainer = UOC_IMMEDIA_ITEM;
	}

	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_ImmediaItem[i])
		{
			Drop.Region.h =	Pick.Region.h = i;
			if (pPickPos)
			{
				if (pDropPos)
				{
					KUiDraggedObject	DragObj;
					Wnd_GetDragObj(&DragObj);
					m_ImmediaItem[i].GetObject(Obj);
					if (DragObj.uGenre == CGOG_SKILL_CUSTOM && Obj.uGenre == CGOG_SKILL_CUSTOM)
					{
						if (!CanDragObj(DragObj.uId))
							return;

						UiSoundPlay(UI_SI_PICKPUT_ITEM);
						Wnd_DragFinished();
						Obj.DataW = 1;
						Obj.DataH = 1;
						Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
						m_ImmediaItem[i].HoldObject(DragObj.uGenre, DragObj.uId, 0, 0);
						return;
					}
				}
				else
				{
					m_ImmediaItem[i].GetObject(Obj);
					if (Obj.uGenre == CGOG_SKILL_CUSTOM)
					{
						UiSoundPlay(UI_SI_PICKPUT_ITEM);
						Wnd_DragFinished();
						Obj.DataW = 1;
						Obj.DataH = 1;
						Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
						m_ImmediaItem[i].HoldObject(CGOG_IME_ITEM, 0, 0, 0);
						return;
					}
				}
			}

			if (pDropPos)
			{
				Wnd_GetDragObj(&Obj);
				if (Obj.uGenre == CGOG_SKILL_CUSTOM)
				{
					if (!CanDragObj(Obj.uId))
						return;

					UiSoundPlay(UI_SI_PICKPUT_ITEM);
					m_ImmediaItem[i].HoldObject(Obj.uGenre, Obj.uId, 0, 0);
					Wnd_DragFinished();
					pDropPos = NULL;
				}
			}
			break;
		}
	}
	_ASSERT(i < UPB_IMMEDIA_ITEM_COUNT);
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, 
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}
BOOL KUiPlayerBar::CanDragObj(unsigned int uId)
{
	KUiDraggedObject	Obj;
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].GetObject(Obj);
		if (Obj.uGenre == CGOG_SKILL_CUSTOM && 
			Obj.uId == uId)
		{
			KSystemMessage	Msg;
			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 0;
			Msg.eType = SMT_NORMAL;
			Msg.uReservedForUi = 0;
			strcpy(Msg.szMessage, "Khung phÝm t¾t ®· cã kü n¨ng nµy!");
			KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
			return FALSE;
		}
	}
	return TRUE;
}
void KUiPlayerBar::OnUseItem(int nIndex)
{
	if(KUiGetString::GetIfVisible() != NULL)
		return;
	if(KUiGetNumber::GetIfVisible() != NULL)
		return;
	if(KUiTongSetTax::GetIfVisible() != NULL)
		return;

	if (m_pSelf && nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		KUiDraggedObject Obj;
		m_pSelf->m_ImmediaItem[nIndex].GetObject(Obj);

		if (Obj.uGenre == CGOG_SKILL_CUSTOM)
		{
			int	x, y;
			Wnd_GetCursorPos(&x, &y);
			g_pCoreShell->UseSkill(x, y, Obj.uId,true);
			return;
		}


		KUiObjAtRegion Info;
		{
			Info.Obj.uGenre = Obj.uGenre;
			Info.Obj.uId = Obj.uId;
			Info.Region.h = nIndex;
			Info.Region.v = 0;
			Info.Region.Width = Info.Region.Height = 0;
		}
		g_pCoreShell->OperationRequest(GOI_USE_ITEM, (int)&Info, UOC_IMMEDIA_ITEM);
	}
}

int KUiPlayerBar::GetChannelIndex(const char* pTitle)
{
	if (pTitle)
	{
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		int n = KUiMsgCentrePad::GetChannelIndex((char*)pTitle);
		if (n >= 0 && n < nChannelDataCount)
			return n;
		
		n = nChannelDataCount;
		for (; n < nChannelDataCount + m_nRecentPlayerName; n++)
		{
			if (m_RecentPlayerName[n - nChannelDataCount][0] == 0)
				break;
			if (strcmp(m_RecentPlayerName[n - nChannelDataCount], pTitle) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}

void KUiPlayerBar::SetCurrentChannel(int nIndex)
{
	if (!m_pSelf)
		return;
	if (nIndex < 0)
		return;

	if (nIndex == m_pSelf->m_nCurChannel)
		return;
	
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(m_pSelf->m_nCurChannel, &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
	}
	else if (nIndex < nChannelDataCount + m_pSelf->m_nRecentPlayerName)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetPeopleMenuinfo(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount], &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
		else
			m_pSelf->m_ChannelSwitchBtn.SetText(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount]);
	}
}

void KUiPlayerBar::InputCurrentChannel(int nIndex, bool bFocus)
{
	if (nIndex < 0)
		return;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		InputNameMsg(true, KUiMsgCentrePad::GetChannelTitle(nIndex), bFocus);
		SetCurrentChannel(nIndex);
	}
	else if (nIndex < nChannelDataCount + m_nRecentPlayerName)
	{
		InputNameMsg(false, m_RecentPlayerName[nIndex - nChannelDataCount], bFocus);
		SetCurrentChannel(nIndex);
	}
}

void KUiPlayerBar::SwitchChannel(BOOL bUp)
{
	if(m_pSelf)
	{
		int nNewChannelID;
		int nCount = KUiMsgCentrePad::GetChannelCount();
		if(bUp)
		{
			if(m_pSelf->m_nCurChannel < 1)
		    {
			    nNewChannelID = nCount - 1;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel - 1;
		    }
		}
		else
		{
		    if(m_pSelf->m_nCurChannel > nCount - 2)
		    {
			    nNewChannelID = 0;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel + 1;
		    }
		}
		m_pSelf->InputCurrentChannel(nNewChannelID, FALSE);
	}
}

int KUiPlayerBar::GetCurChannel()
{
	if(m_pSelf)
	    return m_pSelf->m_nCurChannel;
	else
		return -1;
}

void KUiPlayerBar::UpdateXXXNumber(int& nMana, int& nFullMana)
{
	
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));

	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

	m_nExperienceFull = Info.nExperienceFull;
	m_nCurLevelExperience = Info.nCurLevelExperience;
	m_nExperience = Info.nExperience;

	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	if (pStatus)
		pStatus->UpdateRuntimeInfo(&Info);

	KUiSceneTimeInfo	Spot;
	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Spot, 0);
	KUiMiniMap::UpdateSceneTimeInfo(&Spot);
	KUiAutoPlay::UpdateSceneTimeInfo(&Spot);//AutoAI by kinnox;
	nMana = max(Info.nMana, 0);
	nFullMana = max(Info.nManaFull, 0);
}

void KUiPlayerBar::UpdateRuntimeAttribute(int& nMoney, int& nLevel)
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);

	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	if (pStatus)
		pStatus->UpdateRuntimeAttribute(&Info);

	nMoney = Info.nMoney;
	nLevel = Info.nLevel;
}

void KUiPlayerBar::UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		KUiDraggedObject Obj;
		m_ImmediaItem[nIndex].GetObject(Obj);
		if (Obj.uGenre == CGOG_SKILL_CUSTOM)
		{
			if (uId > 0)
			{
				Obj.uGenre = CGOG_SKILL_CUSTOM;
				Obj.uId = Obj.uId;
				Obj.DataW = 1;
				Obj.DataH = 1;
				Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);			
			}
			else
				return;
		}
		m_ImmediaItem[nIndex].HoldObject(CGOG_IME_ITEM, uId, 0, 0);
	}
}


void KUiPlayerBar::UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex == 0 || nIndex == 1)
	{
		m_ImmediaSkill[nIndex].HoldObject(uGenre, uId, 0, 0);
	}
}

void KUiPlayerBar::Breathe()
{
	if (g_LoginLogic.GetStatus() != LL_S_IN_GAME)
		return;

	m_DateTime.UpdateData();
	m_ToolBox.UpdateData();
	int nMana;
	int nFullMana;
	UpdateXXXNumber(nMana, nFullMana);
	int nMoney;
	int nLevel;
	UpdateRuntimeAttribute(nMoney, nLevel);

	DWORD dwID;
	BYTE cost;
	char* Buffer;
	int nLen;
	int nUseMana = 0;
	int nUseMoney = 0;
	int nUseLevel = 0;

	while(KUiMsgCentrePad::GetChannelData(dwID, cost, Buffer, nLen))
	{
		int nRet = IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana);

		bool bSend = !nRet;
		char szSystem[256];
		szSystem[0] = 0;

		if (!bSend)
		{
			if (nRet == 2 && nUseLevel > 0)
				sprintf(szSystem, "§¼ng cÊp cña b¹n kh«ng ®ñ %d cÊp, kh«ng c¸ch nµo sö dông thiªn lý truyÒn ©m!", nUseLevel);

			if (nRet == 1 && nUseMoney > 0)
				sprintf(szSystem, "TiÒn cña b¹n kh«ng ®ñ %d l­îng, kh«ng c¸ch nµo sö dông thiªn lý truyÒn ©m!", nUseMoney);

			if (nRet == 3 && nUseMana > 0)
				sprintf(szSystem, "Néi lùc cña b¹n kh«ng ®ñ %d ®iÓm, kh«ng c¸ch nµo sö dông thiªn lý truyÒn ©m!", nUseMana);
		}
		else
		{
			if (nUseMoney > 0)
				sprintf(szSystem, "B¹n sö dông thiªn lý truyÒn ©m tæn hao ng©n l­îng %d l­îng!", nUseMoney);
			if (nUseMana > 0)
				sprintf(szSystem, "B¹n sö dông thiªn lý truyÒn ©m tæn hao néi lùc %d ®iÓm!", nUseMana);
		}

		if (bSend)
			OnDirectSendChannelMessage(dwID, cost, Buffer, nLen);

		KUiMsgCentrePad::PopChannelData(dwID);
		
		if (szSystem[0])
			KUiMsgCentrePad::SystemMessageArrival(szSystem, strlen(szSystem));
	}
	
	if (m_nCurChannel >= 0 && m_nCurChannel < (int)KUiMsgCentrePad::GetChannelCount() &&
		Wnd_GetFocusWnd() == (KWndWindow*)&m_InputEdit)
	{
		cost = KUiMsgCentrePad::GetChannelCost(KUiMsgCentrePad::GetChannelID(m_nCurChannel));
		if (IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana) != 0)
			m_InputEdit.SetFocusBkColor(s_NotCanFocusColor);
		else
			m_InputEdit.SetFocusBkColor(s_CanFocusColor);
	}


	for (int j = 0; j < MAX_NUM_BUTTERFLY; j++)
	{
		if (m_uAdviceIntervalMin[j])
		{
			if (m_uInterval[j] == 0)
			{
				if (m_NoteQuest.NextFrame())
				{
					m_uLastSleepTime[j] = IR_GetCurrentTime();
					m_uInterval[j] = m_uAdviceIntervalMin[j] +
						(rand() % (m_uAdviceIntervalMax[j] - m_uAdviceIntervalMin[j]));
				}
			}
			else if (IR_IsTimePassed(m_uInterval[j], m_uLastSleepTime[j]))
			{
				m_NoteQuest.UpdateTimer();
				m_uInterval[j] = 0;
			}
		}
		else
		{
			m_NoteQuest.NextFrame();
		}


		if (m_uAdviceIntervalMin_1[j])
		{
			if (m_uInterval_1[j] == 0)
			{
				if (m_HelpMission.NextFrame())
				{
					m_uLastSleepTime_1[j] = IR_GetCurrentTime();
					m_uInterval_1[j] = m_uAdviceIntervalMin_1[j] +
						(rand() % (m_uAdviceIntervalMax_1[j] - m_uAdviceIntervalMin_1[j]));
				}
			}
			else if (IR_IsTimePassed(m_uInterval_1[j], m_uLastSleepTime_1[j]))
			{
				m_HelpMission.UpdateTimer();
				m_uInterval_1[j] = 0;
			}
		}
		else
		{
			m_HelpMission.NextFrame();
		}
	}
}

int KUiPlayerBar::IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana)
{
	if (cost == 0)
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 1)
	{
		if (nMoney < 10)
		{
			nUseMoney = 10;
			return 1;
		}
		nUseMoney = 10;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 2)
	{
		if (nLevel < 10)
		{
			nUseLevel = 10;
			return 2;
		}

		if (nMana < nFullMana / 2)
		{
			nUseMana = nFullMana / 2;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana / 2;
	}
	else if (cost == 3)
	{
		if (nMana < nFullMana / 10)
		{
			nUseMana = nFullMana / 10;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana / 10;
	}
	else if (cost == 4)
	{
		if (nLevel < 20)
		{
			nUseLevel = 20;
			return 2;
		}

		if (nMana < nFullMana * 4 / 5)
		{
			nUseMana = nFullMana * 4 / 5;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana * 4 / 5;
	}
	else
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	return 0;
}


void KUiPlayerBar::InputNameMsg(char bChannel, const char* szName, bool bFocus)
{
	if (m_pSelf == NULL)
		return;

	if (bFocus)
		Wnd_SetFocusWnd(&m_pSelf->m_InputEdit);
	
	if (szName == NULL || szName[0] == 0)
		return;

	if (bChannel)
		return;

	char Buffer[512];
	int nLen = m_pSelf->m_InputEdit.GetText(Buffer, sizeof(Buffer), false);
	if (nLen < 0)
		nLen = 0;
	Buffer[nLen] = 0;

	int nName = 0;
	if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX)
	{
		while (nName <= nLen)
		{
			if (Buffer[nName] == ' ' ||
				Buffer[nName] == 0)
			{
				Buffer[nName] = 0;
				break;
			}
			nName++;
		}

		if (nName > nLen)
			nName = 0;
	}
	
	if (bChannel)
	{
		m_pSelf->m_InputEdit.SetText("");
		m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
	}
	else
	{
		if (nName > 0 &&
			(KShortcutKeyCentre::FindCommand(Buffer + 1) >= 0 ||
			KShortcutKeyCentre::ms_FunsMap.find(Buffer + 1) != KShortcutKeyCentre::ms_FunsMap.end()
			))
		{
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
		}
		else
		{
			m_pSelf->m_InputEdit.SetText("/");
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
		}
	}
}

int	KUiPlayerBar::FindRecentPlayer(const char* szName)
{
	if (szName == NULL || szName[0] == 0)
		return -1;
	for (int n = 0; n < m_pSelf->m_nRecentPlayerName; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			break;
		}
		if (strcmp(m_pSelf->m_RecentPlayerName[n], szName) == 0)
		{
			return n;
		}
	}

	return -1;
}


char* KUiPlayerBar::GetRecentPlayerName(int nIndex)
{
	int nChannelCount = KUiMsgCentrePad::GetChannelCount();
	if(m_pSelf && nIndex >= nChannelCount && nIndex < m_pSelf->m_nRecentPlayerName + nChannelCount)
	{
		return m_pSelf->m_RecentPlayerName[nIndex - nChannelCount];
	}
	else
	{
		return "";
	}
}


int KUiPlayerBar::AddRecentPlayer(const char* szName)
{
	if (!m_pSelf)
		return -1;
	if (szName == NULL || szName[0] == 0)
		return -1;
	if (strcmp(m_pSelf->m_szSelfName, szName) == 0)
		return -1;
	int n = m_pSelf->GetChannelIndex(szName);
	if (n >= 0)
		return n;
	n = 0;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	for (; n < MAX_RECENTPLAYER_COUNT; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
			m_pSelf->m_nRecentPlayerName++;
			return n + nChannelDataCount;
		}
	}

	if (n >= MAX_RECENTPLAYER_COUNT)
	{
		memmove(m_pSelf->m_RecentPlayerName[0], m_pSelf->m_RecentPlayerName[1], sizeof(m_pSelf->m_RecentPlayerName) - 32);
		n = MAX_RECENTPLAYER_COUNT - 1;
		strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
		return n + nChannelDataCount;
	}

	return -1;
}


void KUiPlayerBar::ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc)
{
	KUiMsgCentrePad::ReplaceChannelName(szDest, nDestSize, szSrc);
}

bool KUiPlayerBar::GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp)
{
	if (m_pSelf)
	{
		nFull = m_pSelf->m_nExperienceFull;
		nCurrLevelExp = m_pSelf->m_nCurLevelExperience;
		nCurrentExp = m_pSelf->m_nExperience;
		return true;
	}
	return false;
}


BOOL KUiPlayerBar::LoadPrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		short nName	= 0;
		short nLife	= 0;
		short nMana	= 0;
		short nObj	= 0;
		short nPK	= 0;
		
		if (pFile->GetShort("Player", "ShowLife", 0, &nLife))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_LIFE, 0, (int)nLife);
		if (pFile->GetShort("Player", "ShowMana", 0, &nMana))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_MANA, 0, (int)nMana);
		if (pFile->GetShort("Player", "ShowObj", 0, &nObj))
			g_pCoreShell->OperationRequest(GOI_SHOW_OBJ_NAME, 0, (int)nObj);
		if (pFile->GetShort("Player", "ShowName", 0, &nName))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_NAME, 0, (int)nName);
		if (pFile->GetShort("Player", "PK", 0, &nPK))
			g_pCoreShell->OperationRequest(GOI_PK_SETTING, 0, (int)nPK);
	}

	if (m_pSelf)
	{
		KUiDraggedObject	Obj;
		char szShortcutName[32];
		for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
		{
			memset(&Obj, 0, sizeof(Obj));
			sprintf(szShortcutName, "Item_%d", i);
			pFile->GetStruct("Player", szShortcutName, &Obj, sizeof(KUiDraggedObject));
			if (Obj.uGenre == CGOG_SKILL_CUSTOM)
			{
				//if (g_pCoreShell->CanGetSkill(Obj.uId))
					m_pSelf->m_ImmediaItem[i].HoldObject(Obj.uGenre, Obj.uId, 0, 0);
				//else
				//	pFile->EraseKey("Player", szShortcutName);
			}
		}
	}

	return TRUE;
}

int	KUiPlayerBar::SavePrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		int nLife = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_LIFE, 0, 0);
		pFile->WriteInteger("Player", "ShowLife", nLife);
		int nMana = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_MANA, 0, 0);
		pFile->WriteInteger("Player", "ShowMana", nMana);
		int nObj = g_pCoreShell->GetGameData(GDI_SHOW_OBJ_NAME, 0, 0);
		pFile->WriteInteger("Player", "ShowObj", nObj);
		int nName = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_NAME, 0, 0);
		pFile->WriteInteger("Player", "ShowName", nName);
		int nPK = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
		pFile->WriteInteger("Player", "PK", nPK);
	}
	if (m_pSelf)
	{
		char szShortcutName[32];
		for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
		{
			sprintf(szShortcutName, "Item_%d", i);
			KUiDraggedObject Obj;
			m_pSelf->m_ImmediaItem[i].GetObject(Obj);
			if (Obj.uGenre == CGOG_SKILL_CUSTOM)
			{
				pFile->WriteStruct("Player", szShortcutName,
					&Obj, sizeof(KUiDraggedObject));
			}
			else
			{
				pFile->EraseKey("Player", szShortcutName);
			}
		}
	}

	return 1;
}


void	KUiPlayerBar::SetChatItemInfo(KUiChatItemParam cItem, unsigned int uId)
{	
	if(cItem.m_Genre < 0)
		return;

	if(uId <= 0)
		return;

	if (m_pSelf)
	{
		memset(m_pSelf->m_szChatItemInfo, 0, sizeof(m_pSelf->m_szChatItemInfo));
		memset(m_pSelf->m_szChatItemName, 0, sizeof(m_pSelf->m_szChatItemName));

		char	szBuffer[16];
		int		nOffSet = 0;

		m_pSelf->m_szChatItemInfo[nOffSet] = '(';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-Init] OffSet: %d - Info: %s - Lengh: %d", nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// ColorID
		_itoa(cItem.m_ColorID, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_ColorID-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_ColorID, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Genre
		_itoa(cItem.m_Genre, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Genre-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Genre, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Detail
		_itoa(cItem.m_DetailType, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_DetailType-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_DetailType, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Particular
		_itoa(cItem.m_Record, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Record-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Record, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Series
		_itoa(cItem.m_Series, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Series-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Series, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Level
		_itoa(cItem.m_Level, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Level-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Level, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Luck
		_itoa(cItem.m_Luck, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Luck-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Luck, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		// Magic Level
		int i;
		for (i = 0; i < 6 ; i++)
		{
			_itoa(cItem.m_MagicLevel[i], szBuffer, 10);
			strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
			nOffSet += strlen(szBuffer);
			m_pSelf->m_szChatItemInfo[nOffSet] = '•';
			nOffSet++;
#ifdef _DEBUG
			//g_DebugLog("[SC-m_MagicLevel-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_MagicLevel[i], nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif
		}
		// End
		// RandomSeed
		_itoa(cItem.m_RandomSeed, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-m_RandomSeed-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_RandomSeed, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif	
		// End
		// Version
		_itoa(cItem.m_Version, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-m_Version-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_Version, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif	
		// End
		// StackNum
		_itoa(cItem.m_StackCount, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-m_StackCount-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_StackCount, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif	
		// End
		// BindState
		_itoa(cItem.m_BindState, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = '•';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-m_BindState-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_BindState, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif	
		// End
		// Expried Time
		_itoa(cItem.m_ExpiredTime, szBuffer, 10);
		strcat(&m_pSelf->m_szChatItemInfo[nOffSet], szBuffer);
		nOffSet += strlen(szBuffer);
		m_pSelf->m_szChatItemInfo[nOffSet] = ')';
		nOffSet++;
#ifdef _DEBUG
		//g_DebugLog("[SC-m_ExpiredTime-%d] OffSet: %d - Info: %s - Lengh: %d", cItem.m_ExpiredTime, nOffSet, m_pSelf->m_szChatItemInfo, nOffSet);
#endif	
		// End
		m_pSelf->m_szChatItemInfo[nOffSet] = '\0';
		if(g_pCoreShell)
		{
			char szName[80];
			char szName2[80];
			if(g_pCoreShell->GetGameData(GDI_ITEM_NAME, (unsigned int)&szName, uId))
			{	
				int nLen = strlen(szName);
				szName2[0] = '<';
				strncpy(szName2 + 1, szName, nLen);
				strncpy(szName2 + 1 + nLen, ">", 1);
				nLen += 2;
				szName2[nLen] = '\0';
				memcpy(m_pSelf->m_szChatItemName, szName2, sizeof(m_pSelf->m_szChatItemName));
				m_pSelf->m_InputEdit.InsertString(szName2, nLen);
#ifdef _DEBUG
				//g_DebugLog("[SC-Item] Name: %s - Lengh: %d", m_pSelf->m_szChatItemName, strlen(m_pSelf->m_szChatItemName));
#endif
			}
		}
	}
}

void	KUiPlayerBar::SetChatItemInfo(short nBtnNo, char* pszName, char* pszInfo)
{	
	if(nBtnNo < 0 || nBtnNo >= defMAX_CHATITEM_BUTTON)
		return;

	if(!pszName[0]) 
		return;

	if(!pszInfo[0])
		return;

	if(m_pSelf)
	{
		memcpy(m_pSelf->m_ChatItemBtn[nBtnNo].m_szItemInfo, pszInfo, sizeof(m_pSelf->m_ChatItemBtn[nBtnNo].m_szItemInfo));

		char * pszCheck1 = NULL;
		char * pszCheck2 = NULL;
		int nOffset = 0;
		int i = 0;
		int j = 0;
		int nCount = 0;
		BOOL bOk = FALSE;
		char szNum[16];
		BYTE nColorID = 0;
		int nLen = strlen(pszInfo);

		pszCheck1 = pszInfo + i;
		while(i < nLen)
		{	
			if(*pszCheck1 == '(')
			{
				pszCheck2 = pszInfo + i;
				j = i;
				while(j < nLen)
				{
					 if(*pszCheck2 == ')' && nCount == 17) 
					 {	
						 bOk = TRUE;
						 break;
					 }

					 if(*pszCheck2 == '•') 
						 nCount++;

					 pszCheck2++;
					 j++;
				}
				if(bOk)
				{
					break;
				}
			}
			pszCheck1++;
			i++;

		}
		if(!bOk)
			return;

		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		while(1) 
		{
			if(*pszCheck1 == '•')
				break;

			if(nOffset < 5)
			{
				szNum[nOffset] = (*pszCheck1);
				
			}
			pszCheck1++;
			nOffset++;
		}
		nColorID = atoi(szNum);
			
		if(nColorID < 0)
			return;
		
		switch(nColorID)
		{
			case 0:
				m_pSelf->m_ChatItemBtn[nBtnNo].SetColor(0xffffffff,0xff00ffff,0xffff647a,0xff000000,0xff000000,0xff000000);
				break;
			case 1:
				m_pSelf->m_ChatItemBtn[nBtnNo].SetColor(0xff3366fa,0xff00ffff,0xffff647a,0xff000000,0xff000000,0xff000000);
				break;
			case 2:
				m_pSelf->m_ChatItemBtn[nBtnNo].SetColor(0xffbc40ff,0xff00ffff,0xffff647a,0xff000000,0xff000000,0xff000000);
				break;
			case 3:
				m_pSelf->m_ChatItemBtn[nBtnNo].SetColor(0xffffcc00,0xff00ffff,0xffff647a,0xff000000,0xff000000,0xff000000);
				break;
			case 4:
				m_pSelf->m_ChatItemBtn[nBtnNo].SetColor(0xffff0000,0xff00ffff,0xffff647a,0xff000000,0xff000000,0xff000000);
				break;
			default:
				break;
		}
	
		nLen = strlen(pszName);
		m_pSelf->m_ChatItemBtn[nBtnNo].SetSize(nLen * 6, 14);
		m_pSelf->m_ChatItemBtn[nBtnNo].SetText(pszName, nLen);
	}
}


void	KUiPlayerBar::SetChatItemPosition(short nBtnNo, short nLeft, short nTop, bool bShow)
{
	if(nBtnNo >= 0 && nBtnNo < defMAX_CHATITEM_BUTTON)
	{
		if(m_pSelf)
		{	
			if(!m_pSelf->m_ChatItemBtn[nBtnNo].m_szItemInfo[0]) 
				return;
			
			if(!bShow)
			{
				if(!m_pSelf->m_ChatItemBtn[nBtnNo].IsVisible())
					return;
			}

			m_pSelf->m_ChatItemBtn[nBtnNo].SetPosition(nLeft, nTop);
			if(bShow)
			{
				m_pSelf->m_ChatItemBtn[nBtnNo].Show();
				Wnd_AddWindow(&m_pSelf->m_ChatItemBtn[nBtnNo], WL_LOWEST);
			}
			else
			{
				m_pSelf->m_ChatItemBtn[nBtnNo].Hide();
			}
		}
	}
}


void KUiPlayerBar::ClearChatItem()
{
	if(m_pSelf)
	{
		for (int i = 0; i < defMAX_CHATITEM_BUTTON; i++)
		{
			if(!m_pSelf->m_ChatItemBtn[i].IsVisible()) 
				continue;
			m_pSelf->m_ChatItemBtn[i].SetPosition(0,0);
			m_pSelf->m_ChatItemBtn[i].Hide();
		}
	}
}

//-
//
//-
void KUiPlayerBar::EncodeChatItemInfo(const char* pMsgBuff, unsigned int nMsgLength, char* pszName, char* pszInfo)
{	
	if(!pMsgBuff[0])
		return;
	
	if(m_pSelf)
	{
		char * pszCheck1 = NULL;
		char * pszCheck2 = NULL;
		int nOffset = 0;
		int i = 0;
		int j = 0;
		int nCount = 0;
		BOOL bOk = FALSE;
		char szNum[16];
		char szInfo[MAX_CHATITEM_INFO_LENGTH];
		KUiChatItemParam	cItemInfo;

		pszCheck1 = (char*)(pMsgBuff + i);
		while(i < nMsgLength)
		{	
			if(*pszCheck1 == '(')
			{
				pszCheck2 = (char*)(pMsgBuff + i);
				j = i;
				while(j < nMsgLength)
				{
					 if(*pszCheck2 == ')' && nCount == 17) 
					 {	
						 szInfo[nOffset] = (*pszCheck2);
						 szInfo[nOffset + 1] = '\0';
						 bOk = TRUE;
						 break;
					 }

					 if(*pszCheck2 == '•') 
						 nCount++;

					 szInfo[nOffset] = (*pszCheck2);
					 pszCheck2++;
					 nOffset++;
					 j++;
				}
				if(bOk)
				{
					break;
				}
			}
			pszCheck1++;
			i++;

		}
		if(!bOk)
			return;

		nOffset = 0;
		pszCheck1++;
		// Color
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_ColorID = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_ColorID] %d", cItemInfo.m_ColorID);
#endif
		// Genre
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Genre = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_Genre] %d", cItemInfo.m_Genre);
#endif
		// Detail
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_DetailType = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_DetailType] %d", cItemInfo.m_DetailType);
#endif
		// Record
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Record = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_Record] %d", cItemInfo.m_Record);
#endif
		// Series
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Series = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_Series] %d", cItemInfo.m_Series);
#endif
		// Level
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Level = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-Level] %d", cItemInfo.m_Level);
#endif
		// Luck
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 11) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Luck = atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
#ifdef _DEBUG
		//g_DebugLog("[EC-m_Luck] %d", cItemInfo.m_Luck);
#endif
		// Magic Level
		for (i = 0; i < 6; i++)
		{	
			ZeroMemory(szNum, sizeof(szNum));
			nOffset = 0;
			pszCheck1++;
			while(1)
			{
				if(*pszCheck1 == '•')
					break;
				if(nOffset < 5) 
				{
					szNum[nOffset] = (*pszCheck1);
				}
				pszCheck1++;
				nOffset++;
			}
			cItemInfo.m_MagicLevel[i] = atoi(szNum);
#ifdef _DEBUG
			//g_DebugLog("[EC-m_MagicLevel] %d", cItemInfo.m_MagicLevel[i]);
#endif
		}
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
		// End
		// RandomSeed
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 11) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_RandomSeed = (DWORD)atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
#ifdef _DEBUG
		//g_DebugLog("[EC-m_RandomSeed] %d", cItemInfo.m_RandomSeed);
#endif	
		// End
		// RandomSeed
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_Version = (DWORD)atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
#ifdef _DEBUG
		//g_DebugLog("[EC-m_Version] %d", cItemInfo.m_Version);
#endif	
		// End
		// Stack Count
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 5) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_StackCount = (WORD)atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
#ifdef _DEBUG
		//g_DebugLog("[EC-m_StackCount] %d", cItemInfo.m_StackCount);
#endif	
		// End
		// Bind State
		while(1)
		{
			if(*pszCheck1 == '•')
				break;
			if(nOffset < 11) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_BindState = (DWORD)atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
#ifdef _DEBUG
		//g_DebugLog("[EC-m_BindState] %d", cItemInfo.m_BindState);
#endif	
		// End
		// ExpiredTime
		while(1)
		{
			if(*pszCheck1 == ')')
				break;
			if(nOffset < 11) 
			{
				szNum[nOffset] = (*pszCheck1);
			}
			pszCheck1++;
			nOffset++;
		}
		cItemInfo.m_ExpiredTime = (DWORD)atoi(szNum);
		ZeroMemory(szNum, sizeof(szNum));
		nOffset = 0;
		pszCheck1++;
#ifdef _DEBUG
		//g_DebugLog("[EC-m_ExpiredTime] %d", cItemInfo.m_ExpiredTime);
#endif	
		// End

		if(g_pCoreShell)
		{
			g_pCoreShell->GetGameData(GDI_CHAT_ITEM_NAME, (unsigned int)&cItemInfo, (unsigned int)pszName);
		}
		strcpy(pszInfo, szInfo);
		}

}

//-
//
//-
char g_ActionPKName[][32] = 
{
	"LuyÖn c«ng",
	"ChiÕn ®Êu",
	"§å s¸t",
};

//-
//
//-
void KUiPlayerBar::PopupPKMenu(short nX, short nY)
{
	int nActionDataCount = sizeof(g_ActionPKName) / 32;
	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pMenuData == NULL)
		return;
	KPopupMenu::InitMenuData(pMenuData, nActionDataCount);
	pMenuData->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;

	int nMenuCount = 0;
	int i;
	pMenuData->nItemHeight = 0;
	for (i = 0; i < nActionDataCount; i++)
	{
		if (i == 0 || i == 1 || i == 2)
		{
			strcpy(pMenuData->Items[nMenuCount].szData, g_ActionPKName[i]);
			pMenuData->Items[nMenuCount].uDataLen = strlen(g_ActionPKName[i]);
			pMenuData->Items[nMenuCount].uID = i;
			nMenuCount++;
		}
	}
	pMenuData->nNumItem = nMenuCount;
	
	pMenuData->nX = nX;
	pMenuData->nY = nY;
	KPopupMenu::Popup(pMenuData, m_pSelf->GetIfVisible(), SEL_PK_MENU);
}

//-
//
//-
void KUiPlayerBar::OnChoosePKMenu(BYTE btChoose)
{
	g_pCoreShell->OperationRequest(GOI_PK_SETTING, 0, (unsigned int)btChoose);
}

//AutoAI by kinnox;
void KUiPlayerBar::MyMessage(char* strMessage)
{
	char * strChannelName = "CH_NEARBY";
	int nIndex = KUiMsgCentrePad::GetChannelIndex(strChannelName);
	DWORD nChannelID = KUiMsgCentrePad::GetChannelID(nIndex);
	int nLen = strlen(strMessage);	
	if (nChannelID != -1)
	{
		if (IsCanSendMessage(strMessage, nLen, strChannelName, nChannelID))
		{
			char Buffer[1536];
			nLen = KUiFaceSelector::ConvertFaceText(Buffer, strMessage, nLen);
			nLen = TEncodeText(Buffer, nLen);
			KUiMsgCentrePad::CheckChannel(nIndex, true);
			OnSendChannelMessage(nChannelID, Buffer, nLen);
		}
	}
}
