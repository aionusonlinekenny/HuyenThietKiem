
#pragma once

#include "../elem/wndpage.h"
#include "../elem/wndlabeledbutton.h"
#include "../Elem/WndImagePart.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define LIVE_SKILL_COUNT			10
#define	FIGHT_SKILL_COUNT			50
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define	FIGHT_SKILL_SUB_PAGE_COUNT	FIGHT_SKILL_COUNT / FIGHT_SKILL_COUNT_PER_PAGE

//===========================================================================

class KUiFightSkillSubPage : public KWndPage
{
public:
	KUiFightSkillSubPage();
	void	Initialize();				
	void	LoadScheme(const char* pScheme);			
	void	UpdateRemainPoint(int nPoint);				
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	
	void	UpdateData(KUiSkillData* pSkills);			
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();								
private:
	KWndObjectBox	m_FightSkills[FIGHT_SKILL_COUNT_PER_PAGE];
	KWndButton		m_AddPointBtn[FIGHT_SKILL_COUNT_PER_PAGE];

private:
	short		m_nRemainSkillPoint;
	unsigned int	m_nSkillId[FIGHT_SKILL_COUNT_PER_PAGE];

	struct
	{
		int		nFont;
		SIZE	Offset;
		unsigned int Color;
	} m_SkillTextParam;
};

class KUiFightSkillSub : public KWndPageSet
{
public:	
	void	Initialize();								
	void	LoadScheme(const char* pScheme);			
	void	UpdateRemainPoint(int nPoint);				
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	
	void	UpdateData();								
private:
	KUiFightSkillSubPage	m_SubPages[FIGHT_SKILL_SUB_PAGE_COUNT];
	KWndLabeledButton				m_SubPageBtn[FIGHT_SKILL_SUB_PAGE_COUNT];
};

//===========================================================================

class KUiFightSkill : public KWndPage
{
public:
	void	Initialize();								
	void	LoadScheme(const char* pScheme);			
	void	UpdateRemainPoint(int nPoint);				
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	
	void	UpdateData();	
private:
	KWndText32			m_RemainSkillPoint;
	KUiFightSkillSub	m_InternalPad;
};

//===========================================================================

class KUiLiveSkill : public KWndPage
{
public:
	KUiLiveSkill();
	void	Initialize();								
	void	LoadScheme(const char* pScheme);		
	void	UpdateBaseData();							
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	
	void	UpdateData();								
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();								
private:
	KWndText32		m_RemainSkillPoint;
	int				m_nRemainSkillPoint;

	KWndObjectBox	m_LiveSkill[LIVE_SKILL_COUNT];

	struct
	{
		int		nFont;
		SIZE	Offset;
		unsigned int Color;
	} m_SkillTextParam;
};

//===========================================================================

class KUiSkills : public KWndPageSet
{
public:
	static KUiSkills* OpenWindow();							
	static KUiSkills* GetIfVisible();						
	static void		 CloseWindow(bool bDestroy);			
	static void		 LoadScheme(const char* pScheme);		
	static void		 UpdateSkill(KUiSkillData* pSkill, int nIndex);	

	void	UpdateFightRemainPoint(int nPoint);				
	void	UpdateLiveBaseData();							
	void	UpdateFightSkillData();
private:
	KUiSkills(){}
	~KUiSkills() {}
	void	Initialize();							
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	static KUiSkills* m_pSelf;
private:
	KUiFightSkill	m_FightSkillPad;
	KUiLiveSkill	m_LiveSkillPad;
	KWndLabeledButton		m_FightSkillPadBtn;
	KWndLabeledButton		m_LiveSkillPadBtn;
	KWndButton		m_Close;
};
