
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "../Elem/MouseHover.h"
#include "UiSkillTree.h"
#include "UiSysMsgCentre.h"
#include "../UiBase.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../Engine/Src/KDDraw.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define SCHEME_INI						"UiSkillTree.ini"
#define SKILLTREE_SHORTCUT_SKILL_SAVE	"ShortSkill"
#define	IS_LEFT_SKILL					nLevel

KUiSkillTree*	KUiSkillTree::m_pSelf = NULL;
KUiSkillData	KUiSkillTree::ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];


KUiSkillTree::KUiSkillTree()
{
	m_nNumSkills = 0;
	m_bLeft = true;
	m_nFont = 12;
}


KUiSkillTree* KUiSkillTree::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiSkillTree* KUiSkillTree::OpenWindow(int bLeft)
{
	if (m_pSelf == NULL)	
	{
		m_pSelf = new KUiSkillTree;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		Wnd_SetCapture(m_pSelf);
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_bLeft = !!bLeft;
		m_pSelf->UpdateData();
		m_pSelf->Show();
		m_pSelf->BringToTop();
	}
	return m_pSelf;
}

void KUiSkillTree::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->Hide();
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}


void KUiSkillTree::Hide()
{
	Wnd_ReleaseCapture();
	KWndWindow::Hide();
}


void KUiSkillTree::LoadConfig(KIniFile* pIni)
{
	memset(&ms_ShortcutSkills, 0, sizeof(ms_ShortcutSkills));
	if (pIni == NULL)
		return;

	int		i, j;
	bool	bEncounter = false;
	char szShortcutName[32];
	for (i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
	{
		sprintf(szShortcutName, "ShortcutSkill_%d", i);
		pIni->GetStruct(SKILLTREE_SHORTCUT_SKILL_SAVE, szShortcutName,
			&ms_ShortcutSkills[i], sizeof(KUiSkillData));

		if (ms_ShortcutSkills[i].uGenre != CGOG_NOTHING)
		{
			ms_ShortcutSkills[i].IS_LEFT_SKILL = !!ms_ShortcutSkills[i].IS_LEFT_SKILL;
			for (j = 0; j < i; j++)
			{
				if (ms_ShortcutSkills[i].uId == ms_ShortcutSkills[j].uId &&
					ms_ShortcutSkills[i].uGenre == ms_ShortcutSkills[j].uGenre &&
					ms_ShortcutSkills[i].IS_LEFT_SKILL == ms_ShortcutSkills[j].IS_LEFT_SKILL)
				{
					ms_ShortcutSkills[i].uGenre = CGOG_NOTHING;
					ms_ShortcutSkills[i].uId = 0;
				}
			}
			bEncounter = true;
		}
	}

	bool bLeft = false;
	do
	{
		bLeft = !bLeft;
		
		KUiSkillData	Skills[SKILLTREE_MAX_SKILL_COUNT];
		int nNum = g_pCoreShell->GetGameData(
			bLeft ? GDI_LEFT_ENABLE_SKILLS : GDI_RIGHT_ENABLE_SKILLS, (unsigned int)&Skills, 0);

		if (bEncounter)
		{
			for (i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
			{
				if (ms_ShortcutSkills[i].uGenre == CGOG_NOTHING ||
					(!bLeft) != (!ms_ShortcutSkills[i].IS_LEFT_SKILL))
					continue;
				for (j = 0; j < nNum; j++)
				{
					if (ms_ShortcutSkills[i].uId == Skills[j].uId &&
						ms_ShortcutSkills[i].uGenre == Skills[j].uGenre)
					{
						break;
					}
				}
				if (j >= nNum)
					ms_ShortcutSkills[i].uGenre = CGOG_NOTHING;
			}
		}

		KUiGameObject	Skill;
		memset(&Skill, 0, sizeof(Skill));
		pIni->GetStruct("Player", bLeft ? "LeftSkill" : "RightSkill",
			&Skill, sizeof(KUiGameObject));
		if (Skill.uGenre != CGOG_NOTHING && g_pCoreShell)
		{
			for (i = 0; i < nNum; i++)
			{
				if (Skills[i].uId == Skill.uId &&
					Skills[i].uGenre == Skill.uGenre)
				{
					g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL,
						(unsigned int)&Skill, bLeft ? 0 : 1);
					break;
				}
			}
		}
	}while(bLeft);
}


void KUiSkillTree::SaveConfig(KIniFile* pIni)
{
	if (pIni == NULL)
		return;
	char szShortcutName[32];
	for (int i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
	{
		sprintf(szShortcutName, "ShortcutSkill_%d", i);
		if (ms_ShortcutSkills[i].uGenre != CGOG_NOTHING)
		{
			pIni->WriteStruct(SKILLTREE_SHORTCUT_SKILL_SAVE, szShortcutName,
				&ms_ShortcutSkills[i], sizeof(KUiSkillData));
		}
		else
		{
			pIni->EraseKey(SKILLTREE_SHORTCUT_SKILL_SAVE, szShortcutName);
		}
	}

	if(g_pCoreShell)
	{
	    KUiPlayerImmedItemSkill Skill;
		g_pCoreShell->GetGameData(GDI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Skill, 0);

		if (Skill.IMmediaSkill[0].uGenre != CGOG_NOTHING)
			pIni->WriteStruct("Player", "LeftSkill", &Skill.IMmediaSkill[0], sizeof(KUiGameObject));
		else
			pIni->EraseKey("Player", "LeftSkill");
		if (Skill.IMmediaSkill[1].uGenre != CGOG_NOTHING)
			pIni->WriteStruct("Player", "RightSkill", &Skill.IMmediaSkill[1], sizeof(KUiGameObject));
		else
			pIni->EraseKey("Player", "RightSkill");
	}
}


void KUiSkillTree::Initialize()
{
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}


void KUiSkillTree::UpdateWndRect()
{
	short	nNumRow = 0, nNumColu = 0;

	for (short i = 0; i < m_nNumSkills; i++)
	{
		if(i % m_MaxColum != 0)
		{
			nNumColu++;
		}
		else
		{
			nNumRow++;
			nNumColu = 1;
		}
	}
	
	short nWidth = m_nWidthPerSkill * nNumColu;
	short nHeight = m_nHeightPerSkill * nNumRow;

	short nLeft, nTop;

	nLeft = m_nRightLeft + m_nWidthPerSkill - nWidth;
	nTop = m_nRightTop + m_nHeightPerSkill - nHeight;

	SetPosition(nLeft, nTop);
	SetSize(nWidth, nHeight);
}

void KUiSkillTree::UpdateData()
{
	m_nNumSkills = g_pCoreShell->GetGameData(
		m_bLeft ? GDI_LEFT_ENABLE_SKILLS : GDI_RIGHT_ENABLE_SKILLS, (unsigned int)&m_Skills, 0);
	
	int i = 0, j = 0;
	for (i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
	{
		if (ms_ShortcutSkills[i].uGenre == CGOG_NOTHING ||
			(!m_bLeft) != (!ms_ShortcutSkills[i].IS_LEFT_SKILL))
			continue;
		for (j = 0; j < m_nNumSkills; j++)
		{
			if (ms_ShortcutSkills[i].uId == m_Skills[j].uId &&
				ms_ShortcutSkills[i].uGenre == m_Skills[j].uGenre)
			{
				break;
			}
		}
		if (j >= m_nNumSkills)
			ms_ShortcutSkills[i].uGenre = CGOG_NOTHING;
	}

	UpdateWndRect();
}

int	KUiSkillTree::PtInWindow(int x, int y)
{
	if (KWndWindow::PtInWindow(x, y))
		return (GetSkillAtPos(x, y) >= 0);
	return 0;
}


int	KUiSkillTree::GetSkillAtPos(short x, short y)
{
	short nMaxColu = m_Width / m_nWidthPerSkill;
	short nRow = (y - m_nAbsoluteTop) / m_nHeightPerSkill;
	short nColu, nIndex;

	nColu = (x - m_nAbsoluteLeft) / m_nWidthPerSkill - nMaxColu + 1;
	
	nIndex = 0;
	while (nIndex < m_nNumSkills && m_Skills[nIndex].nLevel < nRow)
		nIndex ++;

	int	nNumColu = 0;
	while(nIndex < m_nNumSkills && m_Skills[nIndex].nLevel == nRow)
	{
		if (nNumColu == nColu)
			return nIndex;
		nIndex++;
		nNumColu ++;
	}

	return -1;
}

void KUiSkillTree::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		Ini.GetInteger2("Main", "LeftBtnPos", &m_pSelf->m_nLeftLeft, &m_pSelf->m_nLeftTop);
		Ini.GetInteger2("Main", "RightBtnPos", &m_pSelf->m_nRightLeft, &m_pSelf->m_nRightTop);
		Ini.GetInteger2("Main", "BtnSize", &m_pSelf->m_nWidthPerSkill, &m_pSelf->m_nHeightPerSkill);
		if (m_pSelf->m_nWidthPerSkill < 0)
			m_pSelf->m_nWidthPerSkill = 1;
		if (m_pSelf->m_nHeightPerSkill <= 0)
			m_pSelf->m_nHeightPerSkill = 1;
		Ini.GetInteger("Main", "KeyFont", 12, &m_pSelf->m_nFont);
		Ini.GetString("Main", "KeyColor", "", Buff, sizeof(Buff));
		m_pSelf->m_uColor = GetColor(Buff);
		Ini.GetShort("Main", "MaxBtnCountPerRow", 0, &m_pSelf->m_MaxColum);
		m_pSelf->UpdateWndRect();
	}
}
int KUiSkillTree::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int x, y, nIndex;
	int	nRet = 0;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD(nParam);
		y = HIWORD(nParam);
		if ((nIndex = GetSkillAtPos(x, y)) >= 0)
		{
			KUiGameObject	Skill;
			Skill.uGenre = m_Skills[nIndex].uGenre;
			Skill.uId = m_Skills[nIndex].uId;
			g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL,
				(unsigned int)&Skill, m_bLeft ? 0 : 1);
		}		
		Hide();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_ESCAPE)
		{
			Hide();
			nRet = 1;
		}
	case WM_MOUSEHOVER:
		{
			int	x = LOWORD(nParam);
			int y = HIWORD(nParam);
			int nSkill = GetSkillAtPos(x, y);
			if (nSkill >= 0)
			{
				if (g_MouseOver.IsMoseHoverWndObj(this, nSkill) == 0)
				{
					SetMouseHoverObjectDesc(this, nSkill, m_Skills[nSkill].uGenre,
						m_Skills[nSkill].uId, UOC_SKILL_TREE, x, y);
				}
			}
			else
				g_MouseOver.CancelMouseHoverInfo();
		}
		break;
	case WM_RBUTTONDOWN:
		x = LOWORD(nParam);
		y = HIWORD(nParam);
		if ((nIndex = GetSkillAtPos(x, y)) >= 0)
		{
			KUiDraggedObject Obj;
			if (Wnd_GetDragObj(&Obj))
			{
				if (Obj.uGenre != CGOG_NOTHING && Obj.uGenre != CGOG_SKILL_CUSTOM)
					return 0;
			}
			
			if (m_Skills[nIndex].uGenre != CGOG_NOTHING)
			{
				if (!g_pCoreShell->CanGetSkill(m_Skills[nIndex].uId))
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
				Obj.uId = m_Skills[nIndex].uId;
				Obj.DataW = 1;
				Obj.DataH = 1;			
				Wnd_DragBegin(&Obj, DrawDraggingGameObjFunc);
			}
			Hide();
		}
		break;
	default:
		nRet = KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiSkillTree::HandleShortcutKey(int nIndex)
{
	if (m_pSelf == NULL || nIndex < 0 || nIndex >= SKILLTREE_SHORTCUT_SKILL_COUNT)
		return;
	if (GetIfVisible())
	{
		int	x, y;
		Wnd_GetCursorPos(&x, &y);
		int nSkill = m_pSelf->GetSkillAtPos(x, y);
		if (nSkill < 0)
			return;

		ms_ShortcutSkills[nIndex].uGenre = m_pSelf->m_Skills[nSkill].uGenre;
		ms_ShortcutSkills[nIndex].uId = m_pSelf->m_Skills[nSkill].uId;
		ms_ShortcutSkills[nIndex].IS_LEFT_SKILL = m_pSelf->m_bLeft;
		for (int i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
		{
			if (i != nIndex)
			{
				if (ms_ShortcutSkills[nIndex].uId == ms_ShortcutSkills[i].uId &&
					ms_ShortcutSkills[nIndex].IS_LEFT_SKILL == ms_ShortcutSkills[i].IS_LEFT_SKILL &&
					ms_ShortcutSkills[nIndex].uGenre == ms_ShortcutSkills[i].uGenre)
				{
					ms_ShortcutSkills[i].uGenre = CGOG_NOTHING;
					ms_ShortcutSkills[i].uId = 0;
				}
			}
		}
	}
	else if (ms_ShortcutSkills[nIndex].uGenre != CGOG_NOTHING)
	{
		g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL,
			(unsigned int)&ms_ShortcutSkills[nIndex],
			ms_ShortcutSkills[nIndex].IS_LEFT_SKILL ? 0 : 1);
	}
}
void KUiSkillTree::DirectHandleShortcutKey(int nIndex)
{
	if (nIndex < 0 || nIndex >= SKILLTREE_SHORTCUT_SKILL_COUNT)
		return;

	int	x, y;
	Wnd_GetCursorPos(&x, &y);
	if (GetIfVisible())
	{
		int nSkill = m_pSelf->GetSkillAtPos(x, y);
		if (nSkill < 0)
			return;

		ms_ShortcutSkills[nIndex].uGenre = m_pSelf->m_Skills[nSkill].uGenre;
		ms_ShortcutSkills[nIndex].uId = m_pSelf->m_Skills[nSkill].uId;
		ms_ShortcutSkills[nIndex].IS_LEFT_SKILL = m_pSelf->m_bLeft;
		for (int i = 0; i < SKILLTREE_SHORTCUT_SKILL_COUNT; i++)
		{
			if (i != nIndex)
			{
				if (ms_ShortcutSkills[nIndex].uId == ms_ShortcutSkills[i].uId &&
					ms_ShortcutSkills[nIndex].IS_LEFT_SKILL == ms_ShortcutSkills[i].IS_LEFT_SKILL &&
					ms_ShortcutSkills[nIndex].uGenre == ms_ShortcutSkills[i].uGenre)
				{
					ms_ShortcutSkills[i].uGenre = CGOG_NOTHING;
					ms_ShortcutSkills[i].uId = 0;
				}
			}
		}
	}
	else if (ms_ShortcutSkills[nIndex].uGenre != CGOG_NOTHING)
	{
		g_pCoreShell->UseSkill(x, y, ms_ShortcutSkills[nIndex].uId,true);
		
	}
}
void KUiSkillTree::PaintWindow()
{
	if (g_pRepresentShell == NULL)
		return;
	KWndWindow::PaintWindow();
	short nLeft, nTop = m_nAbsoluteTop - m_nHeightPerSkill;
	
	for (short i = 0; i < m_nNumSkills; i++)
	{
		if(i % m_MaxColum != 0)
		{
			
			nLeft += m_nWidthPerSkill;
		}
		else
		{
			nTop += m_nHeightPerSkill;
			nLeft = m_nAbsoluteLeft + m_Width - m_nWidthPerSkill;
		}

		g_pCoreShell->DrawGameObj(m_Skills[i].uGenre, m_Skills[i].uId,
			nLeft, nTop, m_nWidthPerSkill, m_nHeightPerSkill, 0);


		for (int j = 0; j < SKILLTREE_SHORTCUT_SKILL_COUNT; j++)
		{
			if (m_bLeft == (unsigned)ms_ShortcutSkills[j].IS_LEFT_SKILL &&
				m_Skills[i].uGenre == ms_ShortcutSkills[j].uGenre &&
				m_Skills[i].uId == ms_ShortcutSkills[j].uId)
			{
				char szScript[64];
				sprintf(szScript, SCK_SHORTCUTSKILL_FORMAT, j);
				int nIndexC = KShortcutKeyCentre::FindCommandByScript(szScript);
				char* pszKey = NULL;
				if (nIndexC >= 0)
				{
					pszKey = (char*)KShortcutKeyCentre::GetKeyName(KShortcutKeyCentre::GetCommandKey(nIndexC));
				}
				else
				{
					sprintf(szScript, SCK_DIRECTSHORTCUTSKILL_FORMAT, j);
					nIndexC = KShortcutKeyCentre::FindCommandByScript(szScript);
					if (nIndexC >= 0)
					{
						pszKey = (char*)KShortcutKeyCentre::GetKeyName(KShortcutKeyCentre::GetCommandKey(nIndexC));
					}
				}

				if (pszKey)
				{
					g_pRepresentShell->OutputText(m_nFont, pszKey, KRF_ZERO_END,
						nLeft + m_nWidthPerSkill - m_nFont, nTop + m_nHeightPerSkill - m_nFont - 1, m_uColor);
					break;
				}
			}
		}
	}
}
