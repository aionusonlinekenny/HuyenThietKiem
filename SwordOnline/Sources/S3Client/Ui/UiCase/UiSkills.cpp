
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "../Elem/MouseHover.h"
#include "UiSkills.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;

#define 	SCHEME_INI_SHEET			"UiSkills.ini"
#define 	SCHEME_INI_LIVE				"UiSkills_Live.ini"
#define 	SCHEME_INI_FIGHT			"UiSkills_Fight.ini"
#define 	SCHEME_INI_FIGHT_SUB_PAGE	"UiSkills_FightSub.ini"
#define		SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE	9
#define		AUTO_SET_IMMED_SKILL_MSG_ID					"24"


KUiFightSkillSubPage::KUiFightSkillSubPage()
{
	m_nRemainSkillPoint = 0;
	memset(m_nSkillId, 0, sizeof(m_nSkillId));
}

void KUiFightSkillSubPage::Initialize()
{
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i ++)
	{
		AddChild(&m_FightSkills[i]);
		AddChild(&m_AddPointBtn[i]);

		m_FightSkills[i].Clear();
		m_FightSkills[i].SetContainerId((int)UOC_SKILL_LIST);
	}
}

void KUiFightSkillSubPage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT_SUB_PAGE, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
		{
			sprintf(Buff, "Skill_%d", i);
			m_FightSkills[i].Init(&Ini, Buff);
			m_FightSkills[i].EnablePickPut(false);
			m_AddPointBtn[i].Init(&Ini, "AddPointBtn");
			m_AddPointBtn[i].Hide();
			m_AddPointBtn[i].Enable(false);
		}

		Ini.GetInteger("SkillText", "Font", 12, &m_SkillTextParam.nFont);
		Ini.GetInteger2("SkillText", "Offset",
			(int*)&m_SkillTextParam.Offset.cx, (int*)&m_SkillTextParam.Offset.cy);
		Ini.GetString("SkillText", "Color", "", Buff, 16);
		m_SkillTextParam.Color = GetColor(Buff);
	}
}

void KUiFightSkillSubPage::UpdateRemainPoint(int nPoint)
{
	m_nRemainSkillPoint = nPoint;
	
	
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
	{	
		if(m_nRemainSkillPoint > 0)
		{	
			if(m_AddPointBtn[i].IsDisable())
			{
				m_AddPointBtn[i].Enable(true);
			}
		}
		else
		{	
			if(!m_AddPointBtn[i].IsDisable())
			{
				m_AddPointBtn[i].Enable(false);
			}
		}
	}
}

void KUiFightSkillSubPage::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill && nIndex >= 0 && nIndex < FIGHT_SKILL_COUNT_PER_PAGE);
	if(pSkill->uGenre != CGOG_SKILL_FIGHT)
		return;

	m_FightSkills[nIndex].HoldObject(pSkill->uGenre, pSkill->uId, pSkill->nLevel, pSkill->cPointAdd);		//
	m_nSkillId[nIndex] = pSkill->uId;
}


void KUiFightSkillSubPage::UpdateData(KUiSkillData* pSkills)
{
	_ASSERT(pSkills);
	int nLeft, nTop;
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
	{	
		if(pSkills[i].uId <= 0)
			break;

		if(pSkills[i].uGenre != CGOG_SKILL_FIGHT)
			break;

		m_FightSkills[i].HoldObject(pSkills[i].uGenre, pSkills[i].uId, pSkills[i].nLevel, pSkills[i].cPointAdd);		//
		m_nSkillId[i] = pSkills[i].uId;

		m_FightSkills[i].GetPosition(&nLeft, &nTop);
		nLeft += 20;
		nTop += 36;
		m_AddPointBtn[i].SetPosition(nLeft, nTop);
		m_AddPointBtn[i].Show();
	}
}

int	KUiFightSkillSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		{
			for(int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
			{
				if (uParam == (unsigned int)(KWndWindow*)&m_AddPointBtn[i])
				{	
					if(m_nRemainSkillPoint > 0)
					{
						m_nRemainSkillPoint--;
						g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_FIGHT, m_nSkillId[i]);
					}
					break;
				}
			}
		}
		break;
	case WND_N_LEFT_CLICK_ITEM:
		{
			if (uParam)
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
		}
		break;
	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiFightSkillSubPage::PaintWindow()
{
	KWndPage::PaintWindow();
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
	{
		KUiDraggedObject	Obj;
		m_FightSkills[i].GetObject(Obj);
		if (Obj.uGenre != CGOG_NOTHING)
		{				
			int nLeft, nTop, nWidth, nHeight;
			m_FightSkills[i].GetAbsolutePos(&nLeft, &nTop);
			m_FightSkills[i].GetSize(&nWidth, &nHeight);
			g_pCoreShell->DrawGameObj(CGOG_SKILL_LIVE, Obj.uId, nLeft, nTop, nWidth, nHeight, Obj.DataW);
			if (Obj.DataW)
			{
				char	szLevel[8];
				int		nLen;
				
				if(Obj.DataH)
				{
					Obj.DataW += Obj.DataH;
				}
				
				_itoa(Obj.DataW, szLevel, 10);
				nLen = strlen(szLevel);
				nLeft += (short)m_SkillTextParam.Offset.cx;
				nTop += (short)m_SkillTextParam.Offset.cy;
				nLeft += (nWidth - nLen * m_SkillTextParam.nFont / 2) / 4;
				//
				if(Obj.DataH)
				{	
					g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
													0 << 16 | 102 << 8 | 204, 0, TEXT_IN_SINGLE_PLANE_COORD, 0xff000000);
					
				}
				else 
				{
					g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
													m_SkillTextParam.Color, 0, TEXT_IN_SINGLE_PLANE_COORD, 0xff000000);
				}
				
			}
		}
	}
}

void KUiFightSkillSub::Initialize()
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
	{
		m_SubPages[i].Initialize();
		AddPage(&m_SubPages[i], &m_SubPageBtn[i]);
	}
}


void KUiFightSkillSub::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPageSet::Init(&Ini, "Main");
		m_oFixPos.x = 0;
		m_oFixPos.y = 0;
		SetPosition(0, 0);
		for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		{
			m_SubPages[i].LoadScheme(pScheme);
			sprintf(Buff, "SubPageBtn_%d", i);
			m_SubPageBtn[i].Init(&Ini, Buff);
		}
		Show();
	}
}

void KUiFightSkillSub::UpdateRemainPoint(int nPoint)
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateRemainPoint(nPoint);
}

void KUiFightSkillSub::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill);
	int nPage = nIndex / FIGHT_SKILL_COUNT_PER_PAGE;
	nIndex = nIndex % FIGHT_SKILL_COUNT_PER_PAGE;
	_ASSERT(nPage >= 0 && nPage < FIGHT_SKILL_SUB_PAGE_COUNT);
	m_SubPages[nPage].UpdateSkill(pSkill, nIndex);
}


void KUiFightSkillSub::UpdateData()
{
	KUiSkillData	Skills[FIGHT_SKILL_COUNT];
	g_pCoreShell->GetGameData(GDI_FIGHT_SKILLS, (unsigned int)Skills, 0);
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateData(&Skills[i * FIGHT_SKILL_COUNT_PER_PAGE]);
}


void KUiFightSkill::Initialize()
{
	m_InternalPad.Initialize();
	AddChild(&m_InternalPad);
	AddChild(&m_RemainSkillPoint);
}

void KUiFightSkill::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_InternalPad.LoadScheme(pScheme);
		m_RemainSkillPoint.Init(&Ini, "RemainPoint");
	}
}


void KUiFightSkill::UpdateData()
{	
	short nRemainSkillPoint = (short)g_pCoreShell->GetGameData(GDI_FIGHT_SKILL_POINT, 0, 0);
	m_InternalPad.UpdateRemainPoint(nRemainSkillPoint);
	m_RemainSkillPoint.SetShortText(nRemainSkillPoint);
	m_InternalPad.UpdateData();
}

void KUiFightSkill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	m_InternalPad.UpdateSkill(pSkill, nIndex);
}


void KUiFightSkill::UpdateRemainPoint(int nPoint)
{
	m_RemainSkillPoint.SetIntText(nPoint);
	m_InternalPad.UpdateRemainPoint(nPoint);

}


KUiLiveSkill::KUiLiveSkill()
{
	m_nRemainSkillPoint = 0;
}


void KUiLiveSkill::Initialize()
{
	AddChild(&m_RemainSkillPoint);

	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
	{
		m_LiveSkill[i].Clear();
		AddChild(&m_LiveSkill[i]);
		m_LiveSkill[i].SetContainerId((int)UOC_SKILL_LIST);
	}
}


void KUiLiveSkill::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_LIVE);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_RemainSkillPoint.	Init(&Ini, "RemainPoint");

		for (int i = 0; i < LIVE_SKILL_COUNT; i++)
		{
			sprintf(Buff, "Skill_%d", i);
			m_LiveSkill[i].Init(&Ini, Buff);
			m_LiveSkill[i].EnablePickPut(false);
		}

		Ini.GetInteger("SkillText", "Font", 12, &m_SkillTextParam.nFont);
		Ini.GetInteger2("SkillText", "Offset",
			(int*)&m_SkillTextParam.Offset.cx, (int*)&m_SkillTextParam.Offset.cy);
		Ini.GetString("SkillText", "Color", "", Buff, 16);
		m_SkillTextParam.Color = GetColor(Buff);
	}
}


int KUiLiveSkill::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_LEFT_CLICK_ITEM && uParam && m_nRemainSkillPoint)
	{
		KUiDraggedObject* pObj = (KUiDraggedObject*)uParam;
		if (pObj->uGenre != CGOG_NOTHING)
		{
			m_nRemainSkillPoint--;
			g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_LIVE, pObj->uId);
		}
		return 0;
	}
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

void KUiLiveSkill::PaintWindow()
{
	KWndPage::PaintWindow();
	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
	{
		KUiDraggedObject	Obj;
		m_LiveSkill[i].GetObject(Obj);
		if (Obj.uGenre != CGOG_NOTHING)
		{
			int nLeft, nTop, nWidth, nHeight;
			m_LiveSkill[i].GetAbsolutePos(&nLeft, &nTop);
			m_LiveSkill[i].GetSize(&nWidth, &nHeight);
			g_pCoreShell->DrawGameObj(CGOG_SKILL_LIVE, Obj.uId, nLeft, nTop, nWidth, nHeight, Obj.DataW);
			if (Obj.DataW)
			{
				char	szLevel[8];
				int		nLen;
				_itoa(Obj.DataW, szLevel, 10);
				nLen = strlen(szLevel);
				nLeft += (short)m_SkillTextParam.Offset.cx;
				nTop += (short)m_SkillTextParam.Offset.cy;
				nLeft += (nWidth - nLen * m_SkillTextParam.nFont / 2) / 2;
				g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
					m_SkillTextParam.Color, 0);
			}
		}
	}
}

void KUiLiveSkill::UpdateBaseData()
{
	KUiPlayerLiveSkillBase	Info;
	g_pCoreShell->GetGameData(GDI_LIVE_SKILL_BASE, (unsigned int)&Info, 0);
	m_RemainSkillPoint.SetIntText(m_nRemainSkillPoint = Info.nRemainPoint);
}

void KUiLiveSkill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill && nIndex >= 0 && nIndex < LIVE_SKILL_COUNT)
	{
		m_LiveSkill[nIndex].HoldObject(pSkill->uGenre, pSkill->uId, pSkill->nLevel, 0);
	}
}

void KUiLiveSkill::UpdateData()
{
	UpdateBaseData();
	KUiSkillData	Skills[LIVE_SKILL_COUNT];
	g_pCoreShell->GetGameData(GDI_LIVE_SKILLS, (unsigned int)Skills, 0);
	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
		m_LiveSkill[i].HoldObject(Skills[i].uGenre, Skills[i].uId, Skills[i].nLevel, 0);
}

KUiSkills* KUiSkills::m_pSelf = NULL;
KUiSkills* KUiSkills::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiSkills;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_FightSkillPad.UpdateData();
		m_pSelf->m_LiveSkillPad.UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiSkills* KUiSkills::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiSkills::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

void KUiSkills::Initialize()
{
	m_FightSkillPad.Initialize();
	AddPage(&m_FightSkillPad, &m_FightSkillPadBtn);
	m_LiveSkillPad.Initialize();
	AddPage(&m_LiveSkillPad, &m_LiveSkillPadBtn);

	AddChild(&m_Close);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

void KUiSkills::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHEET);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_FightSkillPadBtn.Init(&Ini, "FightBtn");
		m_pSelf->m_LiveSkillPadBtn .Init(&Ini, "LiveBtn");
		m_pSelf->m_Close           .Init(&Ini, "CloseBtn");

		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
	}
}

void KUiSkills::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill)
	{
		if (m_pSelf)
		{
			if (pSkill->uGenre == CGOG_SKILL_LIVE)
				m_pSelf->m_LiveSkillPad.UpdateSkill(pSkill, nIndex);
			else if (pSkill->uGenre == CGOG_SKILL_FIGHT)
				m_pSelf->m_FightSkillPad.UpdateSkill(pSkill, nIndex);
		}
		if (g_pCoreShell)
		{
			KUiPlayerAttribute	Info;
			memset(&Info, 0, sizeof(KUiPlayerAttribute));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
			if (Info.nLevel <= SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE)
			{
				g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, (unsigned int)pSkill, 1);
				KSystemMessage	Msg;
				Msg.byConfirmType = SMCT_NONE;
				Msg.byParamSize = 0;
				Msg.byPriority = 0;
				Msg.eType = SMT_NORMAL;
				Msg.uReservedForUi = 0;

				KIniFile*	pIni = g_UiBase.GetCommConfigFile();
				if (pIni)
				{
					if (pIni->GetString("InfoString", AUTO_SET_IMMED_SKILL_MSG_ID,
						"", Msg.szMessage, sizeof(Msg.szMessage)))
					{
						KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
					}
					g_UiBase.CloseCommConfigFile();
				}
			}
		}
	}
}


void KUiSkills::UpdateFightRemainPoint(int nPoint)
{
	m_FightSkillPad.UpdateRemainPoint(nPoint);
}

void KUiSkills::UpdateLiveBaseData()
{
	m_LiveSkillPad.UpdateBaseData();
}
void KUiSkills::UpdateFightSkillData()
{
	m_FightSkillPad.UpdateData();
}
int KUiSkills::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_Close)
	{
		Hide();
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}