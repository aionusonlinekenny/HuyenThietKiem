
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiParadeItem.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI			"UiParadeItem.ini"
#define	SCHEME_INI_ATTRIB	"UiParadeItem_Attrib.ini"
#define	SCHEME_INI_EQUIP	"UiParadeItem_Equip.ini"

KUiParadeItem* KUiParadeItem::m_pSelf = NULL;


KUiParadeItem* KUiParadeItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiParadeItem* KUiParadeItem::OpenWindow(KUiPlayerItem* pDest)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiParadeItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->UpdateData(pDest);
		m_pSelf->BringToTop();
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiParadeItem::Show()
{
	KWndPageSet::Show();
	ShowPage(EQUIP_PAGE);
}


void KUiParadeItem::CloseWindow(bool bDestroy)
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

		if (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERITEM_END, 0, 0);
	}
}

void KUiParadeItem::Initialize()
{
	AddChild(&m_BtnAttribPage);
	m_AttribPage.Initialize();
	AddPage(&m_AttribPage,&m_BtnAttribPage);

	AddChild(&m_BtnEquipPage);
	m_EquipPage.Initialize();
	AddPage(&m_EquipPage,&m_BtnEquipPage);

	AddChild(&m_Close);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}


void KUiParadeItem::LoadScheme(const char* pScheme)
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


//	
void KUiParadeItem::LoadScheme(class KIniFile* pIni)
{	
	Init(pIni, "Main");
	m_BtnAttribPage.Init(pIni, "BtnAttribPage");
	m_BtnEquipPage.Init(pIni, "BtnEquipPage");

	m_Close	 .Init(pIni, "Close");
}


int KUiParadeItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnAttribPage)
		{
			ShowPage(ATTRIB_PAGE);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnEquipPage)
		{
			ShowPage(EQUIP_PAGE);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
			Hide();
		}
		break;
	default:
		nRet = KWndPageSet::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}


void KUiParadeItem::ShowPage(char cPage)
{
	switch(cPage)
	{
		case ATTRIB_PAGE:
			m_AttribPage.Show();
			m_BtnAttribPage.CheckButton(true);
			m_EquipPage.Hide();
			m_BtnEquipPage.CheckButton(false);
			break;
		case EQUIP_PAGE:
			m_EquipPage.Show(&m_Dest);
			m_BtnEquipPage.CheckButton(true);
			m_AttribPage.Hide();
			m_BtnAttribPage.CheckButton(false);
			break;
		case JUDGE_PAGE:
			break;
		default:
			break;
	}
}

void KUiParadeItem::UpdateData(KUiPlayerItem* pDest)
{
	if (pDest == NULL)
		return;
	
	m_AttribPage.UpdateBaseData(pDest);
	m_EquipPage.UpdateBaseData(pDest);
	
	m_AttribPage.UpdateRuntimeInfo(pDest);
	
	m_AttribPage.UpdateRuntimeAttribute(pDest);
	m_EquipPage.UpdateRuntimeAttribute(pDest);

	m_EquipPage.UpdateAllEquips(pDest);

	m_Dest = *pDest;
}

void KUiParadeItem::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if(pEquip)
	{
		m_EquipPage.UpdateEquip(pEquip, bAdd);
	}
}

//===========================================================================

KUiParadeItemAttrib::KUiParadeItemAttrib()
{
}


void KUiParadeItemAttrib::Initialize()
{
	AddChild(&m_Face);
	AddChild(&m_Name);
	AddChild(&m_Title);
	AddChild(&m_Prestige);
	AddChild(&m_Luck);
	AddChild(&m_Level);
	AddChild(&m_WorldRank);

	AddChild(&m_Life);
	AddChild(&m_Mana);
	AddChild(&m_Stamina);
	AddChild(&m_StatusDesc);
	
	AddChild(&m_RemainPoint);
	AddChild(&m_Strength);
	AddChild(&m_Dexterity);
	AddChild(&m_Vitality);
	AddChild(&m_Energy);

	AddChild(&m_AddStrength);
	AddChild(&m_AddDexterity);
	AddChild(&m_AddVitality);
	AddChild(&m_AddEnergy);

	AddChild(&m_Experience);

	AddChild(&m_LeftDamage);
	AddChild(&m_RightDamage);
	AddChild(&m_Attack);
	AddChild(&m_Defence);
	AddChild(&m_MoveSpeed);
	AddChild(&m_AttackSpeed);

	AddChild(&m_PhyDef);
	AddChild(&m_CoolDef);
	AddChild(&m_LightDef);
	AddChild(&m_FireDef);
	AddChild(&m_PoisonDef);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

// --
//
// --
void KUiParadeItemAttrib::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ATTRIB);

	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");
		m_Face.Init(&Ini, "Face");
		m_Name.Init(&Ini, "Name");
		m_Title.Init(&Ini, "Title");
		m_Prestige.Init(&Ini, "Prestige");
		m_Luck.Init(&Ini, "Luck");
		m_Level.Init(&Ini, "Level");
		m_WorldRank.Init(&Ini, "WorldRank");

		m_Life.Init(&Ini, "Life");
		m_Mana.Init(&Ini, "Mana");
		m_Stamina.Init(&Ini, "Stamina");
		m_StatusDesc.Init(&Ini, "Status");
		
		m_RemainPoint.Init(&Ini, "RemainPoint");
		m_Strength.Init(&Ini, "Strength");
		m_Dexterity.Init(&Ini, "Dexterity");
		m_Vitality.Init(&Ini, "Vitality");
		m_Energy.Init(&Ini, "Energy");

		m_AddStrength.Init(&Ini, "AddStrength");
		m_AddDexterity.Init(&Ini, "AddDexterity");
		m_AddVitality.Init(&Ini, "AddVitality");
		m_AddEnergy.Init(&Ini, "AddEnergy");

		m_Experience.Init(&Ini, "Exp");

		m_LeftDamage .Init(&Ini, "LeftDamage");
		m_RightDamage.Init(&Ini, "RightDamage");
		m_Attack.Init(&Ini, "Attack");
		m_Defence.Init(&Ini, "Defense");
		m_MoveSpeed.Init(&Ini, "MoveSpeed");
		m_AttackSpeed.Init(&Ini, "AttackSpeed");

		m_PhyDef.Init(&Ini, "ResistPhy");
		m_CoolDef.Init(&Ini, "ResistCold");
		m_LightDef.Init(&Ini, "ResistLighting");
		m_FireDef.Init(&Ini, "ResistFire");
		m_PoisonDef.Init(&Ini, "ResistPoison");
	}
}


void KUiParadeItemAttrib::UpdateBaseData(KUiPlayerItem* pDest)
{
	if (g_pCoreShell)
	{	
				KUiPlayerBaseInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, pDest->uId);
		m_Name.SetText(Info.Name);
		m_Title.SetText(Info.Title);
		m_WorldRank.SetWorldRankText(Info.nRankInWorld);
		if (pDest->nAvatarID == 1) //Avatar Player
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu1.spr",true);
		}
		else if (pDest->nAvatarID == 2)
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu2.spr",true);		
		}	
		else if (pDest->nAvatarID == 3)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu3.spr",true);		
		}
		else if (pDest->nAvatarID == 4)
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu4.spr",true);	
		}
		else if (pDest->nAvatarID == 5)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu5.spr",true);
		}
		else if (pDest->nAvatarID == 6)
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu6.spr",true);
		}
		else if (pDest->nAvatarID == 7)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu7.spr",true);
		}
		else if (pDest->nAvatarID == 8)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu8.spr",true);
		}
		else if (pDest->nAvatarID == 9)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu9.spr",true);
		}
		else if (pDest->nAvatarID == 10)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu10.spr",true);
		}
		else if (pDest->nAvatarID == 11)
		{
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu11.spr",true);	
		}
		else if (pDest->nAvatarID == 12)
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nu12.spr",true);		
		}
		else if (pDest->nAvatarID == 13)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam1.spr",true);			
		}
		else if (pDest->nAvatarID == 14)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam2.spr",true);			
		}
		else if (pDest->nAvatarID == 15)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam3.spr",true);			
		}
		else if (pDest->nAvatarID == 16)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam4.spr",true);			
		}
		else if (pDest->nAvatarID == 17)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam5.spr",true);			
		}
		else if (pDest->nAvatarID == 18)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam6.spr",true);			
		}
		else if (pDest->nAvatarID == 19)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam7.spr",true);		
		}
		else if (pDest->nAvatarID == 20)
		{	
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam8.spr",true);		
		}
		else if (pDest->nAvatarID == 21)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam9.spr",true);			
		}
		else if (pDest->nAvatarID == 22)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam10.spr",true);			
		}
		else if (pDest->nAvatarID == 23)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam11.spr",true);		
		}
		else if (pDest->nAvatarID == 24)
		{		
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam12.spr",true);		
		}
		else if (pDest->nAvatarID == 25)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam13.spr",true);			
		}
		else if (pDest->nAvatarID == 26)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam14.spr",true);			
		}
		else if (pDest->nAvatarID == 27)
		{			
			m_Face.SetImage(ISI_T_SPR,"\\spr\\Ui3\\UiChooseFace\\Nam15.spr",true);		
		}
	}
}


void KUiParadeItemAttrib::UpdateRuntimeInfo(KUiPlayerItem* pDest)
{
	if (g_pCoreShell)
	{	
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, pDest->uId);
		m_Life.SetText("--/--");
		m_Mana.SetText("--/--");
		m_Stamina.SetText("--/--");
		m_Experience.SetText("--/--");
	}
}

void KUiParadeItemAttrib::UpdateRuntimeAttribute(KUiPlayerItem* pDest)
{	
	if (pDest)	
	{	
		KUiPlayerAttribute	Info;
		memset(&Info, 0, sizeof(KUiPlayerAttribute));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, pDest->uId);
		
		m_AddStrength.Enable(false);
		m_AddDexterity.Enable(false);
		m_AddVitality.Enable(false);
		m_AddEnergy.Enable(false);
		//if(!bShow)
		{
			m_RemainPoint.SetText("// --");
			m_Strength.SetText("// --");
			m_Dexterity.SetText("// --");
			m_Vitality.SetText("// --");
			m_Energy.SetText("// --");

			m_LeftDamage.SetText("// --/// --");
			m_RightDamage.SetText("// --/// --");
			m_Attack.SetText("// --");
			m_Defence.SetText("// --");
			m_MoveSpeed.SetText("// --");
			m_AttackSpeed.SetText("// --/// --");

			m_PhyDef.SetText("// --%");
			m_CoolDef.SetText("// --%");
			m_LightDef.SetText("// --%");
			m_FireDef.SetText("// --%");
			m_PoisonDef.SetText("// --%");

			m_Level.SetText("// --");
		}
		/*else
		{	
			m_RemainPoint.SetIntText(Info.nBARemainPoint);
			m_Strength.SetIntText(Info.nStrength);
			m_Dexterity.SetIntText(Info.nDexterity);
			m_Vitality.SetIntText(Info.nVitality);
			m_Energy.SetIntText(Info.nEnergy);

			m_LeftDamage.Set2IntText(Info.nKillMIN, Info.nKillMAX, '/');
			m_RightDamage.Set2IntText(Info.nRightKillMin, Info.nRightKillMax, '/');
			m_Attack.SetIntText(Info.nAttack);
			m_Defence.SetIntText(Info.nDefence);
			m_MoveSpeed.SetIntText(Info.nMoveSpeed);
			
			char	TextInfo[32];
			sprintf(TextInfo, "%d%%", Info.nPhyDef);
			m_PhyDef	.SetText(TextInfo);
			sprintf(TextInfo, "%d%%", Info.nCoolDef);
			m_CoolDef  .SetText(TextInfo);
			sprintf(TextInfo, "%d%%", Info.nLightDef);
			m_LightDef .SetText(TextInfo);
			sprintf(TextInfo, "%d%%", Info.nFireDef);
			m_FireDef  .SetText(TextInfo);
			sprintf(TextInfo, "%d%%", Info.nPoisonDef);
			m_PoisonDef.SetText(TextInfo);

			m_Level.SetIntText(Info.nLevel);
		}*/
		m_StatusDesc.SetText(Info.StatusDesc);
		m_Prestige.SetShortText(Info.nRepute);
		m_Luck.SetShortText(Info.nFuyuan);
	}
}

//===========================================================================

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


KUiParadeItemEquip::KUiParadeItemEquip()
{
}


void KUiParadeItemEquip::Initialize()
{	
	AddChild(&m_Name);
	AddChild(&m_PKValue);
	AddChild(&m_TransLife);

	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_EquipBox[i]);
		m_EquipBox[i].SetContainerId((int)UOC_EQUIPTMENT);
	}
	
	AddChild(&m_MateNameBg);
	AddChild(&m_MateCall);
	AddChild(&m_MateName);

	Wnd_AddWindow(this);
}

void KUiParadeItemEquip::Show(KUiPlayerItem* pDest)
{
	KWndPage::Show();

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme, pDest);
}

// --
//
// --
void KUiParadeItemEquip::LoadScheme(const char* pScheme, KUiPlayerItem* pDest)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_EQUIP);

	if (Ini.Load(Buff))
	{
		if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, (unsigned int)pDest->uId))
		{
			Init(&Ini, "Male");
		}
		else
		{
			Init(&Ini, "Female");
		}

		m_Name.Init(&Ini, "Name");
		m_PKValue.Init(&Ini, "PKValue");
		m_TransLife.Init(&Ini, "TransLife");

		for (int i = 0; i < _ITEM_COUNT; i ++)
		{
			m_EquipBox[i].Init(&Ini, CtrlItemMap[i].pIniSection);
		}
		
		m_MateNameBg.Init(&Ini, "MateNameBg");
		m_MateCall.Init(&Ini, "MateCall");
		m_MateName.Init(&Ini, "MateName");	
	}
}


void KUiParadeItemEquip::UpdateBaseData(KUiPlayerItem* pDest)
{
	if (g_pCoreShell)
	{	
		if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, (unsigned int)pDest->uId))
		{
			m_MateCall.SetText("Phu nh©n");
		}
		else
		{
			m_MateCall.SetText("T­íng c«ng");
		}

		KUiPlayerBaseInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, pDest->uId);

		m_Name.SetText(Info.Name);
		m_MateName.SetText("V« hÖ");
	}
}


void KUiParadeItemEquip::UpdateRuntimeAttribute(KUiPlayerItem* pDest)
{	
	if (pDest)	
	{	
		KUiPlayerAttribute	Info;
		memset(&Info, 0, sizeof(KUiPlayerAttribute));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, (unsigned int)pDest->uId);

		m_PKValue.SetCharText(Info.cPKValue);
		m_TransLife.SetTransLifeText(Info.cTransLife);
	}
}


void KUiParadeItemEquip::UpdateAllEquips(KUiPlayerItem* pDest)
{
	if (g_pCoreShell)
	{
		KUiObjAtRegion	Equips[_ITEM_COUNT];
		int nCount = g_pCoreShell->GetGameData(GDI_PARADE_EQUIPMENT, (unsigned int)&Equips, 0);
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


void KUiParadeItemEquip::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
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