
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiStatus.h"
#include "UiItem.h"
#include "UiTradeConfirmWnd.h"
#include "UiPlayerBar.h"
#include "UiGetNumber.h"
#include "UiChooseFace.h"//Player Avatar by kinnox;
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../UiBase.h"
#include "KEngine.h"
#include "UiInformation.h"
#include "UiShop.h"
#include "UiCreatPW.h"//LockPlayer by kinnox;
#include "UiLoginPW.h"//LockPlayer by kinnox;
#include "UiChaPW.h"//LockPlayer by kinnox;
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI				"UiStatus.ini"


KUiStatus* KUiStatus::m_pSelf = NULL;


static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		},	
	{ UIEP_HAND,		"Weapon"	},	
	{ UIEP_NECK,		"Necklace"	},
	{ UIEP_FINESSE,		"Bangle"	},
	{ UIEP_BODY,		"Cloth"		},
	{ UIEP_WAIST,		"Sash"		},
	{ UIEP_FINGER1,		"Ring1"		},
	{ UIEP_FINGER2,		"Ring2"		},
	{ UIEP_WAIST_DECOR,	"Pendant"	},
	{ UIEP_FOOT,		"Shoes"		},
	{ UIEP_HORSE,		"Horse"		},
	{ UIEP_MASK,		"Mask"		},
	{ UIEP_MANTLE,		"Mantle"	},
	{ UIEP_SIGNET,		"Signet"	},
	{ UIEP_SHIPIN,		"Shipin"	},
	
};


KUiStatus* KUiStatus::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiStatus* KUiStatus::OpenWindow()
{	
	if(m_pSelf == NULL)
	{
		m_pSelf = new KUiStatus;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
        if (KUiShop::GetIfVisible() != NULL)
        {        
            g_UiBase.SetStatus(UIS_S_TRADE_NPC);
        }		
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiStatus::CloseWindow(bool bDestroy)
{	
	// g_UiBase.SetStatus(UIS_S_IDLE);//fix ban hang by kinnox;
	if(m_pSelf)
	{
		if (bDestroy == false)
		{
			m_pSelf->Hide();
		}
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}



void KUiStatus::Initialize()
{
	AddChild(&m_Name);
	AddChild(&m_Title);
	AddChild(&m_Luck);
	AddChild(&m_Prestige);
	AddChild(&m_Level);
	AddChild(&m_WorldRank);
	//
	AddChild(&m_Life);
	AddChild(&m_Mana);
	AddChild(&m_Stamina);
	AddChild(&m_StatusDesc);
	AddChild(&m_Experience);
	//	
	AddChild(&m_Strength);
	AddChild(&m_Dexterity);
	AddChild(&m_Vitality);
	AddChild(&m_Energy);
	//
	AddChild(&m_AddStrength);
	AddChild(&m_AddDexterity);
	AddChild(&m_AddVitality);
	AddChild(&m_AddEnergy);
	//
	AddChild(&m_LeftDamage);
	AddChild(&m_RightDamage);
	//
	AddChild(&m_Attack);
	AddChild(&m_Defence);
	AddChild(&m_MoveSpeed);
	AddChild(&m_AttackSpeed);
	//
	AddChild(&m_PhyDef);
	AddChild(&m_CoolDef);
	AddChild(&m_LightDef);
	AddChild(&m_FireDef);
	AddChild(&m_PoisonDef);
	//
	AddChild(&m_RemainPoint);
	//
	AddChild(&m_PKValue);
	AddChild(&m_TransLife);
	AddChild(&m_FiFong);
	//
	int i;
	for(i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_EquipBox[i]);
		m_EquipBox[i].SetContainerId((int)UOC_EQUIPTMENT);
	}
	//
	AddChild(&m_BtnLock);
	AddChild(&m_BtnUnLock);
	AddChild(&m_BtnBind);
	AddChild(&m_BtnUnBind);

	//Player Avatar by kinnox;
	AddChild(&m_chooseavatar);
	AddChild(&m_Avatar);	
	//end code	
	//
	AddChild(&m_OpenItemPad);
	AddChild(&m_Close);
	

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}


void KUiStatus::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);	
	}
}


void KUiStatus::LoadScheme(class KIniFile* pIni)
{
	if(g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
	{
		Init(pIni, "Male");
	}
	else
	{
		Init(pIni, "Female");
	}
	//
	m_Name.Init(pIni, "Name");
	m_Title.Init(pIni, "Title");
	m_Luck.Init(pIni, "FuYuan");
	m_Prestige.Init(pIni, "Prestige");
	m_Level.Init(pIni, "Level");
	m_WorldRank.Init(pIni, "WorldRank");
	//
	m_Life.Init(pIni, "Life");
	m_Mana.Init(pIni, "Mana");
	m_Stamina.Init(pIni, "Stamina");
	m_StatusDesc.Init(pIni, "Status");
	m_Experience.Init(pIni, "Exp");
	//
	m_Strength.Init(pIni, "Strength");
	m_Dexterity.Init(pIni, "Dexterity");
	m_Vitality.Init(pIni, "Vitality");
	m_Energy.Init(pIni, "Energy");
	//
	m_AddStrength.Init(pIni, "AddStrength");
	m_AddDexterity.Init(pIni, "AddDexterity");
	m_AddVitality.Init(pIni, "AddVitality");
	m_AddEnergy.Init(pIni, "AddEnergy");
	//	
	m_LeftDamage.Init(pIni, "LeftDamage");
	m_RightDamage.Init(pIni, "RightDamage");
	//
	m_Attack.Init(pIni, "Attack");
	m_Defence.Init(pIni, "Defense");
	m_MoveSpeed.Init(pIni, "MoveSpeed");
	m_AttackSpeed.Init(pIni, "AttackSpeed");
	//
	m_PhyDef.Init(pIni, "ResistPhy");
	m_CoolDef.Init(pIni, "ResistCold");
	m_LightDef.Init(pIni, "ResistLighting");
	m_FireDef.Init(pIni, "ResistFire");
	m_PoisonDef.Init(pIni, "ResistPoison");
	//
	m_RemainPoint.Init(pIni, "RemainPoint");
	//
	m_PKValue.Init(pIni, "PKValue");
	m_TransLife.Init(pIni, "TransLife");
	m_FiFong.Init(pIni, "FiFong");
	//
	int i;
	for(i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].Init(pIni, CtrlItemMap[i].pIniSection);
	}
	//
	m_BtnLock.Init(pIni, "BtnLock");
	m_BtnUnLock.Init(pIni, "BtnUnLock");
	m_BtnBind.Init(pIni, "BtnBind");
	m_BtnUnBind.Init(pIni, "BtnUnBind"); 
	//Player Avatar by kinnox;
	m_chooseavatar.Init(pIni, "ClickHere");
	m_chooseavatar.SetText("         Dung m¹o");
	m_Avatar  .Init(pIni, "ClickHere");
	//
	m_OpenItemPad.Init(pIni, "Item");
	m_Close.Init(pIni, "Close");

	
}
// --
//
// --
void KUiStatus::Breathe()
{
	//Player Avatar by kinnox;
	nNumIcon = g_pCoreShell->GetGameData(GDI_AVATAR_IMAGE, 0, 0);
	int nX, nY;
	nX = 9;
	nY = 132;
	if (nNumIcon == 0)
	{
		//
	}
	if (nNumIcon == 1)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu1.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 2)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu2.spr");
		m_chooseavatar.SetText("");
	}	
	if (nNumIcon == 3)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu3.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 4)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu4.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 5)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu5.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 6)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu6.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 7)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu7.spr");
		m_chooseavatar.SetText("");
		
	}
	if (nNumIcon == 8)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu8.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 9)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu9.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 10)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu10.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 11)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu11.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 12)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu12.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 13)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam1.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 14)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam2.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 15)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam3.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 16)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam4.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 17)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam5.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 18)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam6.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 19)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam7.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 20)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam8.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 21)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam9.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 22)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam10.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 23)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam11.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 24)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam12.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 25)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam13.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 26)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam14.spr");
		m_chooseavatar.SetText("");
	}
	if (nNumIcon == 27)
	{
		m_Avatar.SetPosition(nX,nY);
		m_Avatar.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam15.spr");
		m_chooseavatar.SetText("");
	}
	
	//end code
}

int KUiStatus::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
			CloseWindow(false);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OpenItemPad)
		{
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_ITEMS);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Avatar)//Player Avatar by kinnox;
		{
			if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
			{
				KUiChooseFace::OpenWindow(2);
			}
			else
			{	
				KUiChooseFace::OpenWindow(1);
			}
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnLock)
		{
			OnClickLockBtn();
		}
		else if	(uParam == (unsigned int)(KWndWindow*)&m_BtnUnLock)
		{
			OnClickUnLockBtn();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnBind)
		{
			OnClickBindBtn();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnUnBind)
		{
			OnClickUnBindBtn();
		}
		else if (m_nRemainPoint > 0)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_AddStrength)
			{	
				if(GetKeyState(VK_CONTROL) & 0x8000)
				{
					OnDirectAddPoint(WAIT_UP_STRENGHT);
				}
				else
				{
					UseRemainPoint(UIPA_STRENGTH);
				}
			}
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddDexterity)
			{	
				if(GetKeyState(VK_CONTROL) & 0x8000)
				{
					OnDirectAddPoint(WAIT_UP_DEXTERITY);
				}
				else
				{
					UseRemainPoint(UIPA_DEXTERITY);
				}
			}
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddVitality)
			{	
				if(GetKeyState(VK_CONTROL) & 0x8000)
				{
					OnDirectAddPoint(WAIT_UP_VITALITY);
				}
				else
				{
					UseRemainPoint(UIPA_VITALITY);
				}
			}
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddEnergy)
			{	
				if(GetKeyState(VK_CONTROL) & 0x8000)
				{
					OnDirectAddPoint(WAIT_UP_ENERGY);
				}
				else
				{
					UseRemainPoint(UIPA_ENERGY);
				}
			}
		}
		break;
	case WND_N_LEFT_CLICK_ITEM:
		if (g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
			OnRepairItem((KUiDraggedObject*)uParam);
		else if (g_UiBase.GetStatus() == UIS_S_BIND_ITEM)
			OnBindItem((KUiDraggedObject*)uParam);
		else if (g_UiBase.GetStatus() == UIS_S_UNBIND_ITEM)
			OnUnBindItem((KUiDraggedObject*)uParam);
		break;	
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_M_OTHER_WORK_RESULT:
		if(uParam == WAIT_UP_STRENGHT && nParam)
		{	
			UseRemainPoint(UIPA_STRENGTH, nParam);
		}
		else if(uParam == WAIT_UP_DEXTERITY && nParam)
		{
			UseRemainPoint(UIPA_DEXTERITY, nParam);
		}
		else if(uParam == WAIT_UP_VITALITY && nParam)
		{
			UseRemainPoint(UIPA_VITALITY, nParam);
		}
		else if(uParam == WAIT_UP_ENERGY && nParam)
		{
			UseRemainPoint(UIPA_ENERGY, nParam);
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiStatus::UseRemainPoint(UI_PLAYER_ATTRIBUTE type, int nPoint /*= 1*/)
{
	if(nPoint <= 0 || nPoint > m_nRemainPoint)
		return;

	if(g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE, type, nPoint);
		m_nRemainPoint -= nPoint;
		m_RemainPoint.SetIntText(m_nRemainPoint);
		m_AddStrength.Enable(m_nRemainPoint);
		m_AddDexterity.Enable(m_nRemainPoint);
		m_AddVitality.Enable(m_nRemainPoint);
		m_AddEnergy.Enable(m_nRemainPoint);
	}
}

void KUiStatus::OnDirectAddPoint(int nResultParam)
{
	g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE, 0, 0); //fix cong diem by kinnox;
	m_nRemainPoint -= 0;//fix cong diem by kinnox;
	int Point = 0;
	if(m_nRemainPoint >= 250)
	{
		Point = 250;
	}else{
		Point = m_nRemainPoint;
	}

	if(nResultParam == WAIT_UP_STRENGHT){
		KUiGetNumber::OpenWindow("N©ng Søc M¹nh", Point, 250, this, nResultParam);
	}else if(nResultParam == WAIT_UP_DEXTERITY){
		KUiGetNumber::OpenWindow("N©ng Th©n Ph¸p", Point, 250, this, nResultParam);
	}else if(nResultParam == WAIT_UP_VITALITY){
		KUiGetNumber::OpenWindow("N©ng Sinh KhÝ", Point, 250, this, nResultParam);
	}else if(nResultParam == WAIT_UP_ENERGY){
		KUiGetNumber::OpenWindow("N©ng Néi C«ng", Point, 250, this, nResultParam);
	}

	
}

void KUiStatus::UpdateBaseData()
{
	if(!g_pCoreShell)
		return;

	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	m_Name.SetText(Info.Name);
	m_Title.SetText(Info.Title);
	m_WorldRank.SetWorldRankText(Info.nRankInWorld);
}

void KUiStatus::UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo)
{
	if(!pInfo)
		return;
	m_Life.Set2IntText(pInfo->nLife, pInfo->nLifeFull, '/');
	m_Mana.Set2IntText(pInfo->nMana, pInfo->nManaFull, '/');
	m_Stamina.Set2IntText(pInfo->nStamina, pInfo->nStaminaFull, '/');
	m_Experience.Set2IntText(pInfo->nExperience, pInfo->nExperienceFull, '/');
}

void KUiStatus::UpdateClientDaTa()//LockPlayer by kinnox;
{
	if (m_pSelf)
	{
		m_pSelf->UpdateData();
	}
}

void KUiStatus::UpdateData()
{
	UpdateAllEquips();
	UpdateBaseData();
	//LockPlayer by kinnox;
	POINT Check = g_pCoreShell->GetStatusLockPlayer();
	if (!Check.x && Check.y)
	{
		m_BtnLock.Show();
		m_BtnUnLock.Hide();
	}
	else
	{
		m_BtnUnLock.Show();
		m_BtnLock.Hide();
	}
	//end
}

void KUiStatus::UpdateAllEquips()
{	
	if(g_pCoreShell)
	{
		KUiObjAtRegion	Equips[_ITEM_COUNT];
		int nCount = g_pCoreShell->GetGameData(GDI_EQUIPMENT, (unsigned int)&Equips, 0);
		int	i;
		for (i = 0; i < _ITEM_COUNT; i++)
			m_EquipBox[i].Clear();
		for (i = 0; i < nCount; i++)
		{
			if (Equips[i].Obj.uGenre != CGOG_NOTHING)
				UpdateEquip(&Equips[i], true);
		}
	}
}

void KUiStatus::UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo)
{
	if(!pInfo)
		return;

	char szBuffer[16];

	m_Prestige.SetShortText(pInfo->nRepute);
	m_Luck.SetShortText(pInfo->nFuyuan);
	m_Level.SetIntText(pInfo->nLevel);
	m_StatusDesc.SetText(pInfo->StatusDesc);
	//
	m_Strength.SetIntText(pInfo->nStrength);
	m_Dexterity.SetIntText(pInfo->nDexterity);
	m_Vitality.SetIntText(pInfo->nVitality);
	m_Energy.SetIntText(pInfo->nEnergy);
	//
	m_AddStrength.Enable(m_nRemainPoint);
	m_AddDexterity.Enable(m_nRemainPoint);
	m_AddVitality.Enable(m_nRemainPoint);
	m_AddEnergy.Enable(m_nRemainPoint);
	//
	m_LeftDamage.Set2IntText(pInfo->nKillMIN,pInfo->nKillMAX,'/');
	m_RightDamage.Set2IntText(pInfo->nRightKillMin, pInfo->nRightKillMax, '/');
	//
	m_Attack.SetIntText(pInfo->nAttack);
	
	m_Defence.SetIntText(pInfo->nDefence);
	m_MoveSpeed.SetIntText(pInfo->nMoveSpeed);
	m_AttackSpeed.Set2ShortText(pInfo->nAttackSpeed, pInfo->nCastSpeed, '/');

	
	sprintf(szBuffer, "%d%%", pInfo->nPhyDef);
	m_PhyDef.SetText(szBuffer);
	sprintf(szBuffer, "%d%%", pInfo->nCoolDef);
	m_CoolDef.SetText(szBuffer);
	sprintf(szBuffer, "%d%%", pInfo->nLightDef);
	m_LightDef.SetText(szBuffer);
	sprintf(szBuffer, "%d%%", pInfo->nFireDef);
	m_FireDef.SetText(szBuffer);

	sprintf(szBuffer, "%d%%", pInfo->nPoisonDef);
	m_PoisonDef.SetText(szBuffer);
	//
	m_nRemainPoint = pInfo->nBARemainPoint;
	m_RemainPoint.SetIntText(pInfo->nBARemainPoint);
	//
	m_PKValue.SetIntText(pInfo->cPKValue);
	m_TransLife.SetTransLifeText(pInfo->cTransLife);
}

void KUiStatus::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{	
	if (g_pCoreShell == NULL)
		return;

	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_EQUIPTMENT;
		pWnd = pPickPos->pWnd;

		if (eStatus == UIS_S_TRADE_REPAIR)
		{	
			KUiItemBuySelInfo        Price = { 0 };
            if (g_pCoreShell->GetGameData(GDI_REPAIR_ITEM_PRICE, (unsigned int)(&Pick), (int)(&Price)))
            {
                KUiTradeConfirm::OpenWindow(this, &Pick, &Price, TCA_REPAIR);
            }
            return;
            return;
		}
		else if (eStatus == UIS_S_TRADE_SALE)
		{
			return;
		}
		else if (eStatus == UIS_S_TRADE_NPC)
		{
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
		{
			return;
		}
		else if (eStatus == UIS_S_MARKPRICE_ITEM)
		{
			return;
		}

		
		if(GetKeyState(VK_CONTROL) & 0x8000)
		{
			KUiChatItemParam pInfo = { 0 };

			if (g_pCoreShell->GetGameData(GDI_CHAT_ITEM_INFO,
				(unsigned int)&Pick, (unsigned int)&pInfo))
			{
				KUiPlayerBar::SetChatItemInfo(pInfo, Pick.Obj.uId);
			}
			return;
		}
		

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
	{
		return;
	}

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_EQUIPTMENT;
	}

	for (int i = 0; i < _ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_EquipBox[i])
		{
			Drop.Region.v = Pick.Region.v = CtrlItemMap[i].nPosition;
			break;
		}
	}
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

void KUiStatus::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; i++)
		{
			if (CtrlItemMap[i].nPosition == pEquip->Region.v)
			{
				if (bAdd)
					m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}

void KUiStatus::OnNpcTradeMode(bool bTrue)
{	
	int i;
	for(i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].EnablePickPut(!bTrue);
	}
}


void KUiStatus::UpdateButton(bool bShow)
{	
	m_pSelf->OnNpcTradeMode(false);
	if (KUiItem::GetIfVisible() != NULL)
		KUiItem::OnNpcTradeMode(false);

	if(bShow == false)
	{	
		if(g_UiBase.GetStatus() != UIS_S_IDLE)
			g_UiBase.SetStatus(UIS_S_IDLE);

	}
	else
	{	
		if(g_UiBase.GetStatus() != UIS_S_BIND_ITEM)
		{	
			if(m_pSelf->m_BtnBind.IsButtonChecked())
			{
				m_pSelf->m_BtnBind.CheckButton(false);
			}
		}

		if(g_UiBase.GetStatus() != UIS_S_UNBIND_ITEM)
		{
			if(m_pSelf->m_BtnUnBind.IsButtonChecked())
			{
				m_pSelf->m_BtnUnBind.CheckButton(false);
			}
		}
	}
}



void KUiStatus::OnClickLockBtn()
{	
UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus == UIS_S_TRADE_REPAIR)
	{	
		return;
	}
	else if (eStatus == UIS_S_TRADE_SALE)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_NPC)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_BUY)
	{
		return;
	}
	POINT Check = g_pCoreShell->GetStatusLockPlayer();
	if (!Check.x && Check.y)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_LOCK_PWD,TRUE,0);
	}
	else
	{
		UIMessageBox("C¸c h¹ ch­a thiÕt lËp mËt khÈu b¶o vÖ nh©n vËt!",this,"Tho¸t");
	}
}

void KUiStatus::OnClickUnLockBtn()
{
	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus == UIS_S_TRADE_REPAIR)
	{	
		return;
	}
	else if (eStatus == UIS_S_TRADE_SALE)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_NPC)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_BUY)
	{
		return;
	}
	
	POINT Check = g_pCoreShell->GetStatusLockPlayer();
	if (Check.x && Check.y)
	{
		if (KUiChaPW::GetIfVisible())
		{
			/* if KUiChaPW  is Open, then run this function  */
		}
		else if (KUiCreatPW::GetIfVisible())
		{
			/* if KUiCreatPW  is Open, then run this function  */
		}
		else
		{
			KUiLoginPW::OpenWindow();
		}
	}
	else
	{
		UIMessageBox("C¸c h¹ ch­a thiÕt lËp mËt khÈu b¶o vÖ nh©n vËt!",this,"Tho¸t");
	}	
}


void KUiStatus::OnClickBindBtn()
{	
	if (g_pCoreShell == NULL)
		return;

	if(g_pCoreShell->GetGameData(GDI_PLAYER_FIGHT_MODE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_FIGHT_MODE_ON, 0, 0);
		return;
	}

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus == UIS_S_TRADE_REPAIR)
	{	
		return;
	}
	else if (eStatus == UIS_S_TRADE_SALE)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_NPC)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_BUY)
	{
		return;
	}

	OnNpcTradeMode(true);
	if (KUiItem::GetIfVisible() == NULL)
		KUiItem::OpenWindow();
	KUiItem::OnNpcTradeMode(true);

	if(eStatus == UIS_S_BIND_ITEM)
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		UpdateButton();
	}
	else
	{	
		g_UiBase.SetStatus(UIS_S_BIND_ITEM);
		if(!m_BtnBind.IsButtonChecked())
			m_BtnBind.CheckButton(true);
		if(m_BtnUnBind.IsButtonChecked())
			m_BtnUnBind.CheckButton(false);
	}
}

void KUiStatus::OnClickUnBindBtn()
{	
	if (g_pCoreShell == NULL)
		return;


	if(g_pCoreShell->GetGameData(GDI_PLAYER_FIGHT_MODE, 0, 0))
	{	
		g_pCoreShell->SystemMessages(GSM_FIGHT_MODE_ON, 0, 0);
		return;
	}

	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (eStatus == UIS_S_TRADE_REPAIR)
	{	
		return;
	}
	else if (eStatus == UIS_S_TRADE_SALE)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_NPC)
	{
		return;
	}
	else if (eStatus == UIS_S_TRADE_BUY)
	{
		return;
	}

	OnNpcTradeMode(true);
	if (KUiItem::GetIfVisible() == NULL)
		KUiItem::OpenWindow();
	KUiItem::OnNpcTradeMode(true);

	if(eStatus == UIS_S_UNBIND_ITEM)
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		UpdateButton();	
	}
	else
	{	
		g_UiBase.SetStatus(UIS_S_UNBIND_ITEM);
		if(!m_BtnUnBind.IsButtonChecked())
			m_BtnUnBind.CheckButton(true);
		if(m_BtnBind.IsButtonChecked())
			m_BtnBind.CheckButton(false);
	}	
}

void KUiStatus::OnBindItem(KUiDraggedObject* pItem)
{	
if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.Region.h = 0;
	Obj.eContainer = UOC_EQUIPTMENT;
	
	char szFunc[16];
	sprintf(szFunc, "OnBind");
	if(g_pCoreShell->OperationRequest(GOI_PLAYER_BIND_ITEM, (unsigned int)(&Obj), (int)szFunc))
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		UpdateButton();
	}
}

void KUiStatus::OnUnBindItem(KUiDraggedObject* pItem)
{	
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.Region.h = 0;
	Obj.eContainer = UOC_EQUIPTMENT;
	
	char szFunc[16];
	sprintf(szFunc, "OnUnBind");
	if(g_pCoreShell->OperationRequest(GOI_PLAYER_BIND_ITEM, (unsigned int)(&Obj), (int)szFunc))
	{
		g_UiBase.SetStatus(UIS_S_IDLE);
		UpdateButton();
	}
}


void KUiStatus::OnRepairItem(KUiDraggedObject* pItem)
{	
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.Region.h = 0;
	Obj.eContainer = UOC_EQUIPTMENT;

	KUiItemBuySelInfo	Price = { 0 };
	if (g_pCoreShell->GetGameData(GDI_REPAIR_ITEM_PRICE,
		(unsigned int)(&Obj), (int)(&Price)))
	{
		KUiTradeConfirm::OpenWindow(this, &Obj, &Price, TCA_REPAIR);
	}
}