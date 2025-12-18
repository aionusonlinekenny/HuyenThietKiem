
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndObjContainer.h"

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo;
struct KUiObjAtRegion;
struct KUiPlayerAttribute;

#define			_ITEM_COUNT			15		//			

class KUiStatus : public KWndShowAnimate
{
public:
	static KUiStatus*	OpenWindow();
	static KUiStatus*	GetIfVisible();
	static void			CloseWindow(bool bDestroy);
	static void			LoadScheme(const char* pScheme);
	static void			UpdateClientDaTa();//LockPlayer by kinnox;
	void	UpdateBaseData();
	void	UpdateData();
	void	UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo);
	void	UpdateAllEquips();
	void	UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo);
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);
	
	void	OnNpcTradeMode(bool bTrue);
	static void			UpdateButton(bool bShow = true);
	

private:
	KUiStatus() {}
	~KUiStatus() {}
	void	Initialize();
	void	LoadScheme(class KIniFile* pIni);
	void	Breathe();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UseRemainPoint(UI_PLAYER_ATTRIBUTE type, int nPoint = 1);		//
	void	OnDirectAddPoint(int nResultParam);
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	
	void	OnClickMaskFeatureBtn();
	void	OnClickLockBtn();
	void	OnClickUnLockBtn();//LockPlayer by kinnox;
	void	OnClickBindBtn();
	void	OnClickUnBindBtn();
	void	OnBindItem(KUiDraggedObject* pItem);
	void	OnUnBindItem(KUiDraggedObject* pItem);
	void	OnRepairItem(KUiDraggedObject* pItem);
	

private:
	static KUiStatus*	m_pSelf;

	enum WAIT_OTHER_WND_OPER_PARAM
	{
		WAIT_UP_STRENGHT,
		WAIT_UP_DEXTERITY,
		WAIT_UP_VITALITY,
		WAIT_UP_ENERGY,
	};
	
	short		m_nRemainPoint;

private:
	//
	KWndText32	m_Name;
	KWndText32	m_Title;
	KWndText32	m_Luck;
	KWndText32	m_Prestige;
	KWndText32	m_Level;
	KWndText32	m_WorldRank;
	//	
	KWndText32	m_Life;
	KWndText32	m_Mana;
	KWndText32	m_Stamina;
	KWndText32	m_StatusDesc;
	KWndText32	m_Experience;
	//
	KWndText32	m_Strength, m_Dexterity, m_Vitality, m_Energy;
	KWndButton	m_AddStrength, m_AddDexterity, m_AddVitality, m_AddEnergy;
	//
	KWndText32	m_LeftDamage, m_RightDamage;
	//
	KWndText32	m_Attack, m_Defence, m_MoveSpeed, m_AttackSpeed;
	KWndText32	m_PhyDef, m_CoolDef, m_LightDef, m_FireDef, m_PoisonDef;
	//
	KWndText32	m_RemainPoint;
	//
	KWndText32		m_PKValue;
	KWndText32		m_TransLife;
	//
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];
	//
	KWndButton		m_BtnLock;
	KWndButton		m_BtnBind;
	KWndButton		m_BtnUnLock;//LockPlayer by kinnox;
	KWndButton		m_BtnUnBind;
	//Player Avatar by kinnox;
	KWndText80	m_chooseavatar;
	KWndButton  m_Avatar;
	int			nNumIcon;
	//end code
	//
	KWndButton	m_OpenItemPad;
	KWndButton	m_Close;
	KWndImage	m_FiFong;
};
