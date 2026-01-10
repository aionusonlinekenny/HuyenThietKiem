
#pragma once

#include "../Elem/Wndpage.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo; // Add By ShinT
struct KUiObjAtRegion;

#define _ITEM_COUNT 15

//===========================================================================

class KUiParadeItemAttrib : public KWndPage
{
public:
	KUiParadeItemAttrib();
	void	Initialize();								
	void	UpdateBaseData(KUiPlayerItem* pDest);
	void	UpdateRuntimeInfo(KUiPlayerItem* pDest);
	void	UpdateRuntimeAttribute(KUiPlayerItem* pDest);
private:
	void	LoadScheme(const char* pScheme);

private:
	KWndButton	m_Face;
	KWndText32	m_Name;
	KWndText32	m_Title;
	KWndText32	m_Prestige;
	KWndText32	m_Luck;
	KWndText32	m_Level;
	KWndText32	m_WorldRank;

	KWndText32	m_Life;
	KWndText32	m_Mana;
	KWndText32	m_Stamina;
	KWndText32	m_StatusDesc;
	
	KWndText32	m_RemainPoint;
	KWndText32	m_Strength, m_Dexterity, m_Vitality, m_Energy;
	KWndButton	m_AddStrength, m_AddDexterity, m_AddVitality, m_AddEnergy;

	KWndText32	m_Experience;
	KWndText32	m_LeftDamage, m_RightDamage, m_Attack, m_Defence, m_MoveSpeed, m_AttackSpeed;
	KWndText32	m_PhyDef, m_CoolDef, m_LightDef, m_FireDef, m_PoisonDef;
};

//===========================================================================

class KUiParadeItemEquip : public KWndPage
{
public:
	KUiParadeItemEquip();
	void	Initialize();
	void	Show(KUiPlayerItem* pDest);
	void	UpdateBaseData(KUiPlayerItem* pDest);
	void	UpdateRuntimeAttribute(KUiPlayerItem* pDest);
	void	UpdateAllEquips(KUiPlayerItem* pDest);
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);

private:
	void	LoadScheme(const char* pScheme, KUiPlayerItem* pDest);

private:
	KWndText32		m_Name;
	KWndText32		m_PKValue;
	KWndText32		m_TransLife;
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];
	
	KWndImage		m_MateNameBg;
	KWndText32		m_MateCall;
	KWndText32		m_MateName;
};

//===========================================================================

class KUiParadeItem : public KWndPageSet
{
public:
	static KUiParadeItem*	OpenWindow(KUiPlayerItem* pDest);
	static KUiParadeItem*	GetIfVisible();					
	static void			CloseWindow(bool bDestroy);		
	static void			LoadScheme(const char* pScheme);
	
	void	UpdateData(KUiPlayerItem* pDest);

private:
	KUiParadeItem() {}
	~KUiParadeItem() {}
	void	Initialize();
	void	Show();
	void	Show(bool bTradeNpc);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	
	void	LoadScheme(class KIniFile* pIni);
	void	ShowPage(char cPage);
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);

private:
	static KUiParadeItem*	m_pSelf;
	
	enum PARADEITEM_PAGE
	{
		ATTRIB_PAGE = 0,
		EQUIP_PAGE,
		JUDGE_PAGE,
	};

private:
	KUiParadeItemAttrib	m_AttribPage;
	KWndLabeledButton	m_BtnAttribPage;
	KUiParadeItemEquip	m_EquipPage;
	KWndLabeledButton	m_BtnEquipPage;
//	KWndLabeledButton	m_BtnJudgePage;

	KWndButton			m_Close;

private:
	KUiPlayerItem m_Dest;
};
