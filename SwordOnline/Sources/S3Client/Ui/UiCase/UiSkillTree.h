
#pragma once

#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../../../core/src/gamedatadef.h"

#define	SKILLTREE_MAX_SKILL_COUNT		65
#define	SKILLTREE_SHORTCUT_SKILL_COUNT	9
enum SHORTCUT_KEY_INDEX;

class KUiSkillTree : protected KWndWindow
{
public:
	static KUiSkillTree*	OpenWindow(int bLeft);			
	static KUiSkillTree*	GetIfVisible();					
	static void				CloseWindow(bool bDestroy);		
	static void				LoadScheme(const char* pScheme);
	static void				HandleShortcutKey(int nIndex);
	static void				LoadConfig(KIniFile* pIni);
	static void				SaveConfig(KIniFile* pIni);		

	static void				DirectHandleShortcutKey(int nIndex);
private:
	KUiSkillTree();
	~KUiSkillTree() {}
	void	Initialize();							
	void	PaintWindow();							
	int		PtInWindow(int x, int y);
	void	UpdateData();							
	void	UpdateWndRect();
	void	Hide();									
	int		GetSkillAtPos(short x, short y);			
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	static KUiSkillTree*	m_pSelf;
private:
	KUiSkillData	m_Skills[SKILLTREE_MAX_SKILL_COUNT];

private:
	short			m_nNumSkills;
	int				m_nFont;
	unsigned int	m_uColor;

	unsigned int	m_bLeft;
	int				m_nLeftLeft, m_nLeftTop;
	int				m_nRightLeft, m_nRightTop;
	int				m_nWidthPerSkill, m_nHeightPerSkill;
	static KUiSkillData	ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];

	short			m_MaxColum;
};