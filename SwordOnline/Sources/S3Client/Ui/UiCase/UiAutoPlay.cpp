#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/popupmenu.h"
#include "../elem/wnds.h"
#include "UiAutoPlay.h"
#include "UiPlayerBar.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/SkillDef.h"
#include "../../../Engine/src/KDebug.h"
#include "../UiBase.h" 
#include "../UiSoundSetting.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Headers/KProtocolDef.h"  // cho c2s_autosortequipment
#include "../../../core/src/KCore.h"        // cho g_pClient
extern iRepresentShell* g_pRepresentShell;
extern iCoreShell* g_pCoreShell;

KUiPlayerItem*	m_pNearbyPlayersList; //

#define 	SCHEME_INI				"UiAutoPlay.ini"
#define		MSG_NON_SETTINGS		"Kh«ng thiÕt lËp"
#define		MSG_FORMAT_POS			"%d:%d/%d"
#define		MSG_FORMAT_MAGIC		"%s >= %d"
#define		MENU_SELECT_DEFENSE							0x00
#define		MENU_SELECT_BOSS							0x02
#define		MENU_SELECT_FIGHT							0x04
#define		MENU_SELECT_SUPPORT_SKILL1					0x06
#define		MENU_SELECT_SUPPORT_SKILL2					0x08
#define		MENU_SELECT_SUPPORT_SKILL3					0x10
#define		MENU_SELECT_AURA_SKILL1						0x12
#define		MENU_SELECT_AURA_SKILL2						0x14
#define		MENU_SELECT_MANA_SKILL_SMALL				0x16
#define		MENU_SELECT_RIGHT_HAND_SKILL				0x18
#define		MENU_SELECT_FOLLOW_PEOPLE					0x20
#define		MENU_SELECT_PT								0x22
#define		MENU_SELECT_LISTPT							0x24
#define		MENU_SELECT_JPT								0x26
#define		MENU_SELECT_PICK_TYPE						0x28
#define		MENU_SELECT_SORT_MAGIC						0x30
#define		MENU_SELECT_ROOM							0x32
#define		MENU_SELECT_MAPTRAIN						0x34
#define		FTitle 					"F:0x5198"
#define		MTitle					"F:0x2643"
#define		OTitle					"F:0x9831"
#define		PTitle					"F:0x6114"
#define		RTitle					"F:0x0542"
#define		STitle					"F:0x0279"
////////////////////////////////////////////
//////////////////////RECOVERY
////////////////////////////////////////////
KUiRecovery* KUiRecovery::m_pSelf = NULL;
char szArray_SelRoom[][64] =
{
	"Hµnh trang hÕt chç 0 «",
	"Hµnh trang hÕt chç 2 «",
	"Hµnh trang hÕt chç 4 «",
	"Hµnh trang hÕt chç 6 «",
};
void KUiRecovery::Initialize()
{
	AddChild(&m_BgRecovery);//load background

	AddChild(&t_LifeGuide);
	AddChild(&t_TownPortalGuide); 

	AddChild(&m_LifeCK);
	AddChild(&m_LifeEdit1);
	AddChild(&m_LifeEdit2);
	AddChild(&m_LifeEdit3);
	AddChild(&m_ManaCK);
	AddChild(&m_ManaEdit1);
	AddChild(&m_ManaEdit2);
	AddChild(&m_ManaEdit3);
	AddChild(&m_TPLifeLessCK);
	AddChild(&m_TPLifeEdit);
	AddChild(&m_TPManaLessCK);
	AddChild(&m_TPManaEdit);
	AddChild(&m_TPNotMedicineBloodCK);
	AddChild(&m_TPNotMedicineManaCK);
	AddChild(&m_TPHightMoneyCK);
	AddChild(&m_TPHightMoneyEdit);
	AddChild(&m_TPDamageEquipCK);
	AddChild(&m_TPDamageEquipEdit);
	AddChild(&m_TPNotEquipmentChecker);
	AddChild(&m_TPNotEquipmentText);
	AddChild(&m_TPNotEquipmentPopup);
	AddChild(&m_OtherAntiPoisonCK);
	AddChild(&m_OtherEnchaseExpCK);
	AddChild(&m_OtherRepairEqCK);
	AddChild(&m_OtherOpenMedicineCK);
	AddChild(&m_OtherInventoryMoneyCK);
	//AddChild(&m_OtherBackToTownCK);
	AddChild(&m_OtherRePortPKCK);
	AddChild(&m_OtherEatBloodCK);
	AddChild(&m_OtherEmeiBuffCK);
	AddChild(&m_OtherEmeiBuffEdit);
	AddChild(&m_OtherSpamChatCK);
	AddChild(&m_OtherSpamChatEdit);
	m_nSelRoom = 0;
	TimerSpamChat = 0;
}

void KUiRecovery::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	m_BgRecovery.Init(pIni, "BgRecovery");

	t_LifeGuide.Init(pIni, "LifeGuide");
	t_TownPortalGuide.Init(pIni, "TownPortalGuide");

	m_LifeCK.Init(pIni, "LifeCK");
	m_LifeEdit1.Init(pIni, "LifeEdit1");
	m_LifeEdit1.SetIntText(3000);//
	m_LifeEdit2.Init(pIni, "LifeEdit2");
	m_LifeEdit2.SetIntText(80);//
	m_LifeEdit3.Init(pIni, "LifeEdit3");
	m_LifeEdit3.SetIntText(3000);//
	m_ManaCK.Init(pIni, "ManaCK");
	m_ManaEdit1.Init(pIni, "ManaEdit1");
	m_ManaEdit1.SetIntText(3000);//
	m_ManaEdit2.Init(pIni, "ManaEdit2");
	m_ManaEdit2.SetIntText(80);//
	m_ManaEdit3.Init(pIni, "ManaEdit3");
	m_ManaEdit3.SetIntText(3000);//
	m_TPLifeLessCK.Init(pIni, "TPLifeLessCK");
	m_TPLifeEdit.Init(pIni, "TPLifeEdit");
	m_TPLifeEdit.SetIntText(0);
	m_TPManaLessCK.Init(pIni, "TPManaLessCK");
	m_TPManaEdit.Init(pIni, "TPManaEdit");
	m_TPManaEdit.SetIntText(0);
	m_TPNotMedicineBloodCK.Init(pIni, "TPNotMedicineBloodCK");
	m_TPNotMedicineManaCK.Init(pIni, "TPNotMedicineManaCK");
	m_TPHightMoneyCK.Init(pIni, "TPHightMoneyCK");
	m_TPHightMoneyEdit.Init(pIni, "TPHightMoneyEdit");
	m_TPHightMoneyEdit.SetText("0");
	m_TPDamageEquipCK.Init(pIni, "TPDamageEquipCK");
	m_TPDamageEquipEdit.Init(pIni, "TPDamageEquipEdit");
	m_TPDamageEquipEdit.SetText("0");
	m_TPNotEquipmentChecker.Init(pIni, "TPNotEquipmentChecker");
	m_TPNotEquipmentText.Init(pIni, "TPNotEquipmentText");
	m_TPNotEquipmentPopup.Init(pIni, "TPNotEquipmentPopup");
	m_OtherAntiPoisonCK.Init(pIni, "OtherAntiPoisonCK");
	m_OtherEnchaseExpCK.Init(pIni, "OtherEnchaseExpCK");
	m_OtherRepairEqCK.Init(pIni, "OtherRepairEqCK");
	m_OtherOpenMedicineCK.Init(pIni, "OtherOpenMedicineCK");
	m_OtherInventoryMoneyCK.Init(pIni, "OtherInventoryMoneyCK");
	m_OtherInventoryItemCK.Init(pIni, "OtherInventoryItemCK");
	//m_OtherBackToTownCK.Init(pIni, "OtherBackToTownCK");
	m_OtherRePortPKCK.Init(pIni, "OtherRePortPKCK");
	m_OtherEatBloodCK.Init(pIni, "OtherEatBloodCK");	
	m_OtherEmeiBuffCK.Init(pIni, "OtherEmeiBuffCK");
	m_OtherEmeiBuffEdit.Init(pIni, "OtherEmeiBuffEdit");
	m_OtherEmeiBuffEdit.SetText(" 50% ");
	m_OtherSpamChatCK.Init(pIni, "OtherSpamChatCK");
	m_OtherSpamChatEdit.Init(pIni, "OtherSpamChatEdit");
	m_cMessenger = " (xh)<color=orange>Vâ L©m TruyÒn Kú <color> (dd)<color=red>AutoPlay<color> ";
	m_OtherSpamChatEdit.SetText(m_cMessenger);
	LoadRecoverySetting();

}

int KUiRecovery::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_LifeCK)
		{
			if (m_LifeCK.IsButtonChecked())
			{
				m_LifeCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_LIFE,true,NULL,NULL);	
			}
			else
			{
				m_LifeCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_LIFE, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ManaCK)
		{
			if (m_ManaCK.IsButtonChecked())
			{
				m_ManaCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_MANA,true,NULL,NULL);	
			}
			else
			{
				m_ManaCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_MANA, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPLifeLessCK)
		{
			if (m_TPLifeLessCK.IsButtonChecked())
			{
				m_TPLifeLessCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_BTOWNLIF,true,NULL,NULL);	
			}
			else
			{
				m_TPLifeLessCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_BTOWNLIF, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPManaLessCK)
		{
			if (m_TPManaLessCK.IsButtonChecked())
			{
				m_TPManaLessCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_BTOWNMAN,true,NULL,NULL);	
			}
			else
			{
				m_TPManaLessCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_BTOWNMAN, false, NULL, NULL);
			}
		}	
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotMedicineBloodCK)
		{
			if (m_TPNotMedicineBloodCK.IsButtonChecked())
			{
				m_TPNotMedicineBloodCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_NOBLOOD,true,NULL,NULL);	
			}
			else
			{
				m_TPNotMedicineBloodCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_NOBLOOD, false, NULL, NULL);
			}
		}	
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotMedicineManaCK)
		{
			if (m_TPNotMedicineManaCK.IsButtonChecked())
			{
				m_TPNotMedicineManaCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_NOMANA,true,NULL,NULL);	
			}
			else
			{
				m_TPNotMedicineManaCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_NOMANA, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPHightMoneyCK)
		{
			if (m_TPHightMoneyCK.IsButtonChecked())
			{
				m_TPHightMoneyCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_NOGMON,true,NULL,NULL);	
			}
			else
			{
				m_TPHightMoneyCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_NOGMON, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPDamageEquipCK)
		{
			if (m_TPDamageEquipCK.IsButtonChecked())
			{
				m_TPDamageEquipCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_DURABI,true,NULL,NULL);	
			}
			else
			{
				m_TPDamageEquipCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_DURABI, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotEquipmentPopup)
			PopupSelRoomNE();
		else if (uParam == (unsigned int)(KWndWindow*)&m_TPNotEquipmentChecker) 
		{
			if (m_TPNotEquipmentChecker.IsButtonChecked())
			{
				m_TPNotEquipmentChecker.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_NOROOMCK, true, NULL, NULL);
			}
			else
			{
				m_TPNotEquipmentChecker.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_NOROOMCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherAntiPoisonCK) 
		{
			if (m_OtherAntiPoisonCK.IsButtonChecked())
			{
				m_OtherAntiPoisonCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_ANTIPOISON, true, NULL, NULL);
			}
			else
			{
				m_OtherAntiPoisonCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_ANTIPOISON, false, NULL, NULL);
			}
		}	
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherEnchaseExpCK) 
		{
			if (m_OtherEnchaseExpCK.IsButtonChecked())
			{
				m_OtherEnchaseExpCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_USETTL, true, NULL, NULL);
			}
			else
			{
				m_OtherEnchaseExpCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_USETTL, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherOpenMedicineCK) 
		{
			if (m_OtherOpenMedicineCK.IsButtonChecked())
			{
				m_OtherOpenMedicineCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_MEDIBAG, true, NULL, NULL);
			}
			else
			{
				m_OtherOpenMedicineCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_MEDIBAG, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherRepairEqCK) 
		{
			if (m_OtherRepairEqCK.IsButtonChecked())
			{
				m_OtherRepairEqCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_REPAIREQCK, true, NULL, NULL);
			}
			else
			{
				m_OtherRepairEqCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_REPAIREQCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherInventoryMoneyCK) 
		{
			if (m_OtherInventoryMoneyCK.IsButtonChecked())
			{
				m_OtherInventoryMoneyCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_SAVEMONEY, true, NULL, NULL);
			}
			else
			{
				m_OtherInventoryMoneyCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_SAVEMONEY, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherEmeiBuffCK) 
		{
			if (m_OtherEmeiBuffCK.IsButtonChecked())
			{
				m_OtherEmeiBuffCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_EMEIBUFF, true, NULL, NULL);
			}
			else
			{
				m_OtherEmeiBuffCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_EMEIBUFF, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherEatBloodCK) 
		{
			if (m_OtherEatBloodCK.IsButtonChecked())
			{
				m_OtherEatBloodCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_EATFULLBOX, true, NULL, NULL);
			}
			else
			{
				m_OtherEatBloodCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_EATFULLBOX, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherRePortPKCK) 
		{
			if (m_OtherRePortPKCK.IsButtonChecked())
			{
				m_OtherRePortPKCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_R_WARNINGPK, true, NULL, NULL);
			}
			else
			{
				m_OtherRePortPKCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_R_WARNINGPK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherSpamChatCK) 
		{
			if (m_OtherSpamChatCK.IsButtonChecked())
			{
				m_OtherSpamChatCK.CheckButton(true);

			}
			else
			{
				m_OtherSpamChatCK.CheckButton(false);
			}
		}
		SaveRecoverySetting();
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == MENU_SELECT_ROOM && (short)(LOWORD(nParam)) >= 0)
			{
				SetPortalRoomNE(LOWORD(nParam));
			}
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiRecovery::OnCheckInput()
{
	int nLife 			= m_LifeEdit1.GetIntNumber();
	int nLifePercent	= m_LifeEdit2.GetIntNumber();
	int nLifeTime 		= m_LifeEdit3.GetIntNumber();
	int nMana 			= m_ManaEdit1.GetIntNumber();
	int nManaPercent 	= m_ManaEdit2.GetIntNumber();
	int nManaTime 		= m_ManaEdit3.GetIntNumber();
	int nTownLife		= m_TPLifeEdit.GetIntNumber();
	int nTownMana		= m_TPManaEdit.GetIntNumber();
	int nNoMoney		= m_TPHightMoneyEdit.GetIntNumber(); 
	int nDurabality		= m_TPDamageEquipEdit.GetIntNumber(); 
	int nEmeiBuff		= m_OtherEmeiBuffEdit.GetIntNumber();
	m_cMessenger 		= m_OtherSpamChatEdit.GetTextString();
	g_pCoreShell->PAIOperation(GPI_R_EATLIFE,nLifeTime,nLife,nLifePercent);
	g_pCoreShell->PAIOperation(GPI_R_EATMANA,nManaTime,nMana,nManaPercent);
	g_pCoreShell->PAIOperation(GPI_R_TOWLIFV,nTownLife,NULL,NULL);
	g_pCoreShell->PAIOperation(GPI_R_TOWMANV,nTownMana,NULL,NULL);
	g_pCoreShell->PAIOperation(GPI_R_NOGMONV,nNoMoney,NULL,NULL);
	g_pCoreShell->PAIOperation(GPI_R_DURABIV,nDurabality,NULL,NULL);
	g_pCoreShell->PAIOperation(GPI_R_EMEIBUFFNUM,nEmeiBuff,NULL,NULL);
}

void KUiRecovery::PopupSelRoomNE()
{
	int nActionDataCount = sizeof(szArray_SelRoom) / sizeof(szArray_SelRoom[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if ((i == enumRoomNotEnough1) ||
			(i == enumRoomNotEnough2) ||
			(i == enumRoomNotEnough4) ||
			(i == enumRoomNotEnough6)
			)
		{
			strncpy(pSelUnitMenu->Items[i].szData, szArray_SelRoom[i], sizeof(szArray_SelRoom[i]));
			pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
			pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->nNumItem++;
		}
	}
	int x, y;
	m_TPNotEquipmentText.GetAbsolutePos(&x, &y);
	pSelUnitMenu->nX = x;
	pSelUnitMenu->nY = y;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_ROOM);
}

void KUiRecovery::SetPortalRoomNE(int nSel)
{
	m_nSelRoom = nSel;
	m_TPNotEquipmentText.SetText(szArray_SelRoom[m_nSelRoom]);
	g_pCoreShell->PAIOperation(GPI_R_NOROOM, m_nSelRoom, NULL, NULL);
}

void KUiRecovery::Breathe()
{
	if (m_OtherSpamChatCK.IsButtonChecked())
	{	
		if(GetTickCount() - TimerSpamChat >= 36000)
		{
		 	TimerSpamChat = GetTickCount();
			KUiPlayerBar::MyMessage(m_cMessenger);
		}
	}
}

void KUiRecovery::LoadRecoverySetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue,m_nTemp,m_nTemp1;
	if (pConfigFile)
	{			
		pConfigFile->GetInteger(RTitle, "F1",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_LifeCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_LIFE, true, NULL, NULL);
		}
		

		pConfigFile->GetInteger(RTitle, "F2",3000, (int*)(&m_nTemp));
		if (m_nTemp <= 0)
		m_nTemp = 3000;
		m_LifeEdit3.SetIntText(m_nTemp);	
		
		pConfigFile->GetInteger(RTitle, "F3", 3000, (int*)(&m_nValue));
		if (m_nValue <= 0)
		m_nValue = 3000;
		m_LifeEdit1.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(RTitle, "F4", 80, (int*)(&m_nTemp1));
		if (m_nTemp1 <= 0)
		m_nTemp1 = 80;
		m_LifeEdit2.SetIntText(m_nTemp1);
		g_pCoreShell->PAIOperation(GPI_R_EATLIFE,m_nTemp,m_nValue,m_nTemp1);		
		
		pConfigFile->GetInteger(RTitle, "F5",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_ManaCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_MANA, true, NULL, NULL);
		}

		pConfigFile->GetInteger(RTitle, "F6",3000, (int*)(&m_nTemp));
		if (m_nTemp <= 0)
		m_nTemp = 3000;
		m_ManaEdit3.SetIntText(m_nTemp);			
		
		pConfigFile->GetInteger(RTitle, "F7", 3000, (int*)(&m_nValue));
		if (m_nValue <= 0)
		m_nValue = 3000;
		m_ManaEdit1.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(RTitle, "F8", 80, (int*)(&m_nTemp1));
		if (m_nTemp1 <= 0)
		m_nTemp1 = 80;
		m_ManaEdit2.SetIntText(m_nTemp1);
		g_pCoreShell->PAIOperation(GPI_R_EATMANA,m_nTemp,m_nValue,m_nTemp1);	
		
		pConfigFile->GetInteger(RTitle, "F9",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPLifeLessCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_BTOWNLIF,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F10",0, (int*)(&m_nValue));
		m_TPLifeEdit.SetIntText(m_nValue);
		pConfigFile->GetInteger(RTitle, "F11",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPManaLessCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_BTOWNMAN,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F12",0, (int*)(&m_nValue));
		m_TPManaEdit.SetIntText(m_nValue);
		pConfigFile->GetInteger(RTitle, "F13",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPNotMedicineBloodCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_NOBLOOD,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F14",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPNotMedicineManaCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_NOMANA,true,NULL,NULL);	
		}
		
		pConfigFile->GetInteger(RTitle, "F15",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPHightMoneyCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_NOGMON,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F16", 0, (int*)(&m_nValue));	
		m_TPHightMoneyEdit.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(RTitle, "F17",1, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPDamageEquipCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_DURABI,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F18", 10, (int*)(&m_nValue));
		m_TPDamageEquipEdit.SetIntText(m_nValue);
		g_pCoreShell->PAIOperation(GPI_R_DURABIV,m_nValue,NULL,NULL);
		
		pConfigFile->GetInteger(RTitle, "F19",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_TPNotEquipmentChecker.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_NOROOMCK,true,NULL,NULL);	
		}
		pConfigFile->GetInteger(RTitle, "F20", 0, &m_nValue);
		SetPortalRoomNE(m_nValue);
		
		pConfigFile->GetInteger(RTitle, "F21",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherAntiPoisonCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_ANTIPOISON, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F22",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherEnchaseExpCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_USETTL, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F23",1, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherOpenMedicineCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_MEDIBAG, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F24",1, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherRepairEqCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_REPAIREQCK, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F25",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherInventoryMoneyCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_SAVEMONEY, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F26",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherEmeiBuffCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_EMEIBUFF, true, NULL, NULL);
		}
		pConfigFile->GetInteger(RTitle, "F27", 0, &m_nValue);
		m_OtherEmeiBuffEdit.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(RTitle, "F28",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherEatBloodCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_EATFULLBOX, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(RTitle, "F29",0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{
			m_OtherRePortPKCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_R_WARNINGPK, true, NULL, NULL);
		}
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiRecovery::SaveRecoverySetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		if (m_LifeCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F1",true);
		else
			pConfigFile->WriteInteger(RTitle, "F1",false);

			
		pConfigFile->WriteInteger(RTitle, "F2",m_LifeEdit3.GetIntNumber());
		pConfigFile->WriteInteger(RTitle, "F3",m_LifeEdit1.GetIntNumber());
		pConfigFile->WriteInteger(RTitle, "F4",m_LifeEdit2.GetIntNumber());
		
		if (m_ManaCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F5",true);
		else
			pConfigFile->WriteInteger(RTitle, "F5",false);
			
		pConfigFile->WriteInteger(RTitle, "F6",m_ManaEdit3.GetIntNumber());
		pConfigFile->WriteInteger(RTitle, "F7",m_ManaEdit1.GetIntNumber());
		pConfigFile->WriteInteger(RTitle, "F8",m_ManaEdit2.GetIntNumber());
		
		if (m_TPLifeLessCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F9",true);
		else
			pConfigFile->WriteInteger(RTitle, "F9",false);
		pConfigFile->WriteInteger(RTitle, "F10",m_TPLifeEdit.GetIntNumber());	
		if (m_TPManaLessCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F11",true);
		else
			pConfigFile->WriteInteger(RTitle, "F11",false);
		pConfigFile->WriteInteger(RTitle, "F12",m_TPManaEdit.GetIntNumber());	
		if (m_TPNotMedicineBloodCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F13",true);
		else
			pConfigFile->WriteInteger(RTitle, "F13",false);
		
		if (m_TPNotMedicineManaCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F14",true);
		else
			pConfigFile->WriteInteger(RTitle, "F14",false);	
			
		if (m_TPHightMoneyCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F15",true);
		else
			pConfigFile->WriteInteger(RTitle, "F15",false);
			
		if (m_TPDamageEquipCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F17",true);
		else
			pConfigFile->WriteInteger(RTitle, "F17",false);	
			
		pConfigFile->WriteInteger(RTitle, "F16",m_TPHightMoneyEdit.GetIntNumber());	
		
		if (m_TPDamageEquipCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F17",true);
		else
			pConfigFile->WriteInteger(RTitle, "F17",false);	
		
		
		pConfigFile->WriteInteger(RTitle, "F18",m_TPDamageEquipEdit.GetIntNumber());	
		
		if (m_TPNotEquipmentChecker.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F19",true);
		else
			pConfigFile->WriteInteger(RTitle, "F19",false);
			
		pConfigFile->WriteInteger(RTitle, "F20", m_nSelRoom);	
		
		if (m_OtherAntiPoisonCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F21",true);
		else
			pConfigFile->WriteInteger(RTitle, "F21",false);
		
		if (m_OtherEnchaseExpCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F22",true);
		else
			pConfigFile->WriteInteger(RTitle, "F22",false);
		
		if (m_OtherOpenMedicineCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F23",true);
		else
			pConfigFile->WriteInteger(RTitle, "F23",false);
		
		if (m_OtherRepairEqCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F24",true);
		else
			pConfigFile->WriteInteger(RTitle, "F24",false);
		
		if (m_OtherInventoryMoneyCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F25",true);
		else
			pConfigFile->WriteInteger(RTitle, "F25",false);
			
		if (m_OtherEmeiBuffCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F26",true);
		else
			pConfigFile->WriteInteger(RTitle, "F26",false);
				
		pConfigFile->WriteInteger(RTitle, "F27",m_OtherEmeiBuffEdit.GetIntNumber());	
		
		if (m_OtherEatBloodCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F28",true);
		else
			pConfigFile->WriteInteger(RTitle, "F28",false);
		
		if (m_OtherRePortPKCK.IsButtonChecked())
			pConfigFile->WriteInteger(RTitle, "F29",true);
		else
			pConfigFile->WriteInteger(RTitle, "F29",false);
	}
	g_UiBase.CloseAutoSettingFile(true);
}
////////////////////////////////////////////
//////////////////////PICK
////////////////////////////////////////////
KUiPick* KUiPick::m_pSelf = NULL;
char Data_Pick[][64] = 
{
	"NhÆt Ng©n L­îng",
	"NhÆt VËt PhÈm",	
	"NhÆt Trang BÞ",	
	"NhÆt TÊt C¶",
};

/*char Data_Magic[defMAX_AUTO_FILTERL][2][64] = 
{
	"Sinh lùc(®)", 		"lifemax_v",
	"Néi lùc(®)", 		"manamax_v",
	"ThÓ lùc(®)", 		"staminamax_v",
	"Phôc håi sinh lùc(®)", 	"lifereplenish_v",
	"Phôc håi néi lùc(®)", 	"manareplenish_v",
	"Phôc håi thÓ lùc(®)", 	"staminareplenish_v",
	"Hót sinh lùc(%)", 	"steallifeenhance_p",
	"Hót néi lùc(%)", 	"stealmanaenhance_p",
	"Uy danh(h)", 		"addtitle_v",
	"Søc m¹nh(®)", 		"strength_v",
	"Th©n ph¸p(®)", 	"dexterity_v",
	"Sinh khÝ(®)", 		"vitality_v",
	"Néi c«ng(®)", 		"energy_v",
	"Phßng thñ(%)", 	"physicsres_p",
	"Kh¸ng ®éc(%)", 	"poisonres_p",
	"Kh¸ng b¨ng(%)", 	"coldres_p",
	"Kh¸ng háa(%)", 	"fireres_p",
	"Kh¸ng l«i(%)", 	"lightingres_p",
	"Kh¸ng tÊt c¶(%)", 	"allres_p",
	"Tèc ®éc ch¹y(%)", 	"fastwalkrun_p",
	"Tèc ®é ®¸nh(%)", 	"attackspeed_v",
	"Ph¶n ®ßn cËn(®)", 	"meleedamagereturn_v",
	"STVL ngo¹i(®)", 	"addphysicsdamage_v",
	"B¨ng s¸t ngo¹i(®)", "addcolddamage_v",
	"§éc s¸t ngo¹i(®)", "addpoisondamage_v",
	"S¸t th­¬ng VLý ngo¹i(%)", 	"addphysicsdamage_p",
	"CHSTTNL(%)", 		"damage2addmana_p",
	"May m¾n(%)", 		"lucky_v",
	"T©m ph¸p(t)", 		"addskill_v",	
//	"Bá qua nÐ tr¸nh(%)", "ignoredefense_p",
	"§é chÝnh x¸c(®)", 	"attackrating_v",
	"S¸t th­¬ng VLý néi(®)", 	"addphysicsmagic_v",
	"B¨ng s¸t néi c«ng(®)", 	"addcoldmagic_v",
	"Háa s¸t néi c«ng(®)", 	"addfiremagic_v",
	"L«i s¸t néi c«ng(®)", 	"addlightingmagic_v",
	"§éc s¸t néi c«ng(®)", 	"addpoisonmagic_v",
	"TGian chËm(%)", 	"freezetimereduce_p",
	"TGian tróng ®éc(%)", "poisontimereduce_p",
	"TGian cho¸ng(%)", 	"stuntimereduce_p",
	"TGian phôc håi(%)", 	"fasthitrecover_v",
	"Kü n¨ng vèn cã(®)", "allskill_v",
};*/
char Data_Magic[defMAX_AUTO_FILTERL][2][64] = 
{
	"Sinh lùc ®", 		"lifemax_v",
	"Néi lùc ®", 		"manamax_v",
	"ThÓ lùc ®", 		"staminamax_v",
	"PH Sl ®", 	"lifereplenish_v",
	"PH Nl ®", 	"manareplenish_v",
	"PH Tl ®", 	"staminareplenish_v",
	"Hót Sl %", 	"steallifeenhance_p",
	"Hót Nl %", 	"stealmanaenhance_p",
	"Uy danh h", 		"addtitle_v",
	"Søc m¹nh ®", 		"strength_v",
	"Th©n ph¸p ®", 	"dexterity_v",
	"Sinh khÝ ®", 		"vitality_v",
	"Néi c«ng ®", 		"energy_v",
	"Phßng thñ %", 	"physicsres_p",
	"K ®éc %", 	"poisonres_p",
	"K b¨ng %", 	"coldres_p",
	"K háa %", 	"fireres_p",
	"K l«i %", 	"lightingres_p",
	"KTC %", 	"allres_p",
	"T§ ch¹y %", 	"fastwalkrun_p",
	"T§ ®¸nh %", 	"attackspeed_v",
	"Ph¶n ®ßn ®", 	"meleedamagereturn_v",
	"STVL ngo¹i ®", 	"addphysicsdamage_v",
	"BS ngo¹i ®", "addcolddamage_v",
	"§S ngo¹i ®", "addpoisondamage_v",
	"STVL ngo¹i %", 	"addphysicsdamage_p",
	"CHSTTNL %", 		"damage2addmana_p",
	"May m¾n %", 		"lucky_v",
	"T©m ph¸p(t)", 		"addskill_v",	
//	"Bá qua nÐ tr¸nh-%", "ignoredefense_p",
	"ChÝnh x¸c ®", 	"attackrating_v",
	"STVL néi ®", 	"addphysicsmagic_v",
	"BS néi ®", 	"addcoldmagic_v",
	"HS néi ®", 	"addfiremagic_v",
	"LS néi ®", 	"addlightingmagic_v",
	"§S néi ®", 	"addpoisonmagic_v",
	"TGian chËm %", 	"freezetimereduce_p",
	"TGian T§ %", "poisontimereduce_p",
	"TG cho¸ng %", 	"stuntimereduce_p",
	"TGian PH %", 	"fasthitrecover_v",
	"KN vèn cã ®", "allskill_v",
};

void KUiPick::Initialize()
{
	AddChild(&m_BgPick);

	AddChild(&t_PickGuild);
	AddChild(&t_PickPriceItemsTxt);
	AddChild(&t_PickLevelItemsTxt);
	AddChild(&t_PickJewelryTxt);


	AddChild(&m_PickAllCK); //m_PickBtn
	AddChild(&m_PickAllEdit);//m_PickTypeTxt
	AddChild(&m_PickAllPopup);// m_PickAllPopup
	AddChild(&m_PickFilterCK); // m_PickFilterCK
	AddChild(&m_PickAutoSortCK);
	AddChild(&t_PickAutoSortTxt);
	//AddChild(&m_PickFollowTargetCK);
	//AddChild(&m_PickFollowTargetEdit);
	AddChild(&m_PickChooseFilterTxt); //m_FilterEdit
	AddChild(&m_PickChooseFilterPopup); //m_PutListBtn
	AddChild(&m_PickChooseFilterEdit); //m_FillterEdit
	AddChild(&m_PickFilterListTxt); //m_FilterL
	AddChild(&m_PickFilterListScb);
	m_PickFilterListTxt.SetScrollbar(&m_PickFilterListScb);
	AddChild(&m_PickFilterAdd); //m_FilterAddBtn
	AddChild(&m_PickFilterDel);  //m_FilterDelBtn
	AddChild(&m_PickFilterDelAll); //m_FilterDelAll
	AddChild(&m_PickPriceItemsCK);
	AddChild(&m_PickPriceItemsEdit);
	AddChild(&m_PickLevelItemsCK);
	AddChild(&m_PickLevelItemsEdit);
	AddChild(&m_PickJewelryCK); //m_ReturnBtn
	m_CurrentPickKind = 0;
	m_CurrentSelMagic = 0;
	m_nSelectIndex = 0;
	memset(m_FilterMagicIndex, 0, sizeof(m_FilterMagicIndex));
	memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
	memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
}

void KUiPick::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	
	m_BgPick.Init(pIni,"BgPick");
	t_PickGuild.Init(pIni,"PickGuide");
	t_PickPriceItemsTxt.Init(pIni,"PickPriceItemsTxt");
	t_PickLevelItemsTxt.Init(pIni,"PickLevelItemsTxt");
	t_PickJewelryTxt.Init(pIni,"PickJewelryTxt");

	m_PickAllCK.Init(pIni,"PickAllCK");
	m_PickAllEdit.Init(pIni,"PickAllEdit");
	m_PickAllPopup.Init(pIni,"PickAllPopup");
	m_PickFilterCK.Init(pIni,"PickFilterCK");
	m_PickAutoSortCK.Init(pIni,"PickAutoSortCK");
	t_PickAutoSortTxt.Init(pIni,"PickAutoSortTxt");
	//m_PickFollowTargetCK.Init(pIni,"PickFollowTargetCK");
	//m_PickFollowTargetEdit.Init(pIni,"PickFollowTargetEdit");
	m_PickChooseFilterTxt.Init(pIni,"PickChooseFilterTxt");
	m_PickChooseFilterPopup.Init(pIni,"PickChooseFilterPopup");
	m_PickChooseFilterEdit.Init(pIni,"PickChooseFilterEdit");
	m_PickFilterListTxt.Init(pIni,"PickFilterListTxt");
	m_PickFilterListScb.Init(pIni,"PickFilterListScb");
	m_PickFilterListTxt.SetScrollbar(&m_PickFilterListScb);
	m_PickFilterAdd.Init(pIni,"PickFilterAdd");
	m_PickFilterDel.Init(pIni,"PickFilterDel");
	m_PickFilterDelAll.Init(pIni,"PickFilterDelAll");
	m_PickPriceItemsCK.Init(pIni,"PickPriceItemsCK");
	m_PickPriceItemsEdit.Init(pIni,"PickPriceItemsEdit");
	m_PickLevelItemsCK.Init(pIni,"PickLevelItemsCK");
	m_PickLevelItemsEdit.Init(pIni,"PickLevelItemsEdit");
	m_PickJewelryCK.Init(pIni,"PickJewelryCK");
	LoadPickSetting();
}

int KUiPick::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_PickAllCK)
			OnPick();		
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickJewelryCK)
			OnGiveItem();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickFilterCK)
			OnFillterItem();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickAutoSortCK)
			OnAutoSortClick();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickAllPopup)
			OnSelectPickType(MENU_SELECT_PICK_TYPE);
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickChooseFilterPopup)
			OnSelectPickType(MENU_SELECT_SORT_MAGIC);
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickFilterAdd)
		{
			int i = FilterSameMagic(Data_Magic[m_CurrentSelMagic][1]);

			if (i >= 0 && i < defMAX_AUTO_FILTERL)
			{
				strcpy(m_cFilterMagic[i], Data_Magic[m_CurrentSelMagic][1]);
				m_FilterMagicV[i] = m_PickChooseFilterEdit.GetIntNumber();
			}
			else
			{
				for (i = 0; i < defMAX_AUTO_FILTERL; i++)
				{
					if (!m_cFilterMagic[i][0])
					{
						strcpy(m_cFilterMagic[i], Data_Magic[m_CurrentSelMagic][1]);
						m_FilterMagicV[i] = m_PickChooseFilterEdit.GetIntNumber();
						break;
					}
				}
			}
			SetFilterMagicList();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickFilterDel)
		{
			int nSel = m_PickFilterListTxt.GetCurSel();
			memset(m_cFilterMagic[m_FilterMagicIndex[nSel]], 0, sizeof(m_cFilterMagic[nSel]));
			m_FilterMagicV[m_FilterMagicIndex[nSel]] = 0;
			SetFilterMagicList();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickFilterDelAll)
		{
			memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
			memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
			SetFilterMagicList();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickPriceItemsCK)
			OnCheckPrice();
		else if (uParam == (unsigned int)(KWndWindow*)&m_PickLevelItemsCK)
			OnCheckLevel();
		SavePickSetting();	
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();		
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if ((short)(LOWORD(nParam)) >= 0)
			{
				SetPick(HIWORD(nParam), (short)(LOWORD(nParam)));
			}
		}		
		break;
	case WND_N_LIST_ITEM_SEL:
		if(uParam == (unsigned int)&m_PickFilterListTxt)
		{
			m_nSelectIndex = nParam;
		}
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_PickFilterListScb)
		{
    		m_PickFilterListTxt.SetTopItemIndex(nParam);
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiPick::OnPick()
{
	BYTE btFlag = 0;
	if (m_PickAllCK.IsButtonChecked() > 0 )
		btFlag = 1;
	else
		btFlag = 0;
	g_pCoreShell->PAIOperation(GPI_P_BPICKUP,btFlag,NULL,NULL);	
}

void KUiPick::OnFillterItem()
{	
	BYTE btFlag = 0;
	if (m_PickFilterCK.IsButtonChecked() > 0 )
		btFlag = 1;
	else
		btFlag = 0;
	g_pCoreShell->PAIOperation(GPI_P_FILTEQUIP,btFlag,NULL,NULL);
}
void KUiPick::OnAutoSortClick()
{
	
	if (g_pClient)
	{
		BYTE protocol = c2s_autosortequipment; // c2s_autosortequipment - temporary ID, will define properly
		g_pClient->SendPackToServer(&protocol, sizeof(protocol));
	}
}
void KUiPick::OnGiveItem() 
{	
	BYTE btFlag = 0;
	if (m_PickJewelryCK.IsButtonChecked() > 0 )
		btFlag = 1;
	else
		btFlag = 0;
	g_pCoreShell->PAIOperation(GPI_P_SAVERAP,btFlag,NULL,NULL);	
}

void KUiPick::OnCheckPrice()
{
	BYTE btFlag = 0;
	if (m_PickPriceItemsCK.IsButtonChecked() > 0 )
		btFlag = 1;
	else
		btFlag = 0;
	g_pCoreShell->PAIOperation(GPI_P_PRICEITEMCK,btFlag,NULL,NULL);	
}
void KUiPick::OnCheckLevel()
{
	BYTE btFlag = 0;
	if (m_PickLevelItemsCK.IsButtonChecked() > 0 )
		btFlag = 1;
	else
		btFlag = 0;	
	g_pCoreShell->PAIOperation(GPI_S_LEVELITEMCK,btFlag,NULL,NULL);	
}
void KUiPick::OnCheckInput()
{
	int nPrice = m_PickPriceItemsEdit.GetIntNumber();
	int nLevel = m_PickLevelItemsEdit.GetIntNumber();
	g_pCoreShell->PAIOperation(GPI_P_PRICEITEM,nPrice,NULL,NULL);	
	g_pCoreShell->PAIOperation(GPI_S_LEVELITEM,nLevel,NULL,NULL);	
}
void KUiPick::SetFilterMagicList()
{
	memset(m_FilterMagicIndex, 0, sizeof(m_FilterMagicIndex));
	m_PickFilterListTxt.ResetContent();

	char Buff[64];
	int Count = 0;
	for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
	{
		g_pCoreShell->SetSortItem(NULL, 0, i);
		if (m_cFilterMagic[i][0])
		{
			g_pCoreShell->SetSortItem((unsigned int)m_cFilterMagic[i], m_FilterMagicV[i], Count);

			sprintf(Buff, MSG_FORMAT_MAGIC, Data_Magic[FindSameMagic(m_cFilterMagic[i])][0], m_FilterMagicV[i]);
			m_PickFilterListTxt.AddString(Count, Buff,"","");
			m_FilterMagicIndex[Count] = i;
			Count++;
		}
	}
}

int KUiPick::FilterSameMagic(const char* szMagic)
{
	for (int i = 0; i < defMAX_AUTO_FILTERL; i ++)
	{
		if (strcmp(szMagic, m_cFilterMagic[i]) == 0)
			return i;
	}
	return -1;
}

int KUiPick::FindSameMagic(const char* szMagic)
{
	for (int i = 0; i < defMAX_AUTO_FILTERL; i ++)
	{
		if (strcmp(szMagic, Data_Magic[i][1]) == 0)
			return i;
	}
	return -1;
}
void KUiPick::SetPick(int nIndex, int nSel)
{
	switch (nIndex)
	{
		case MENU_SELECT_PICK_TYPE:
			if (nSel >= enumPickUpEarn && nSel < enumPickNum)
			{
				m_CurrentPickKind = nSel;
				m_PickAllEdit.SetText(Data_Pick[m_CurrentPickKind]);
				g_pCoreShell->PAIOperation(GPI_P_PICKUPK,m_CurrentPickKind,NULL,NULL);
			}
			break;
		case MENU_SELECT_SORT_MAGIC:
			m_CurrentSelMagic = nSel;
			m_PickChooseFilterTxt.SetText(Data_Magic[m_CurrentSelMagic][0]);
			break;			
	}
	//SavePickSetting();
}

void KUiPick::OnSelectPickType(int nIndex)
{
	int nActionDataCount = 0;
	if (nIndex == MENU_SELECT_PICK_TYPE)
		nActionDataCount = sizeof(Data_Pick) / sizeof(Data_Pick[0]);
	else if (nIndex = MENU_SELECT_SORT_MAGIC)
		nActionDataCount = sizeof(Data_Magic) / sizeof(Data_Magic[0]);	
	else return;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (nIndex == MENU_SELECT_PICK_TYPE)
			strncpy(pSelUnitMenu->Items[i].szData, Data_Pick[i], sizeof(Data_Pick[i]));
		else if (nIndex = MENU_SELECT_SORT_MAGIC)
			strncpy(pSelUnitMenu->Items[i].szData, Data_Magic[i][0], sizeof(Data_Magic[i][0]));	
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int Left, Top, nWidth;
	GetSize(&nWidth, NULL);
	GetAbsolutePos(&Left, &Top);
	KIniFile	Ini;
	if (nIndex == MENU_SELECT_PICK_TYPE)
		Ini.GetInteger2("PopupPosition","PickItem",&nX, &nY);
	else if (nIndex = MENU_SELECT_SORT_MAGIC) 
		Ini.GetInteger2("PopupPosition","PickMagic",&nX, &nY);		
	pSelUnitMenu->nX = Left + nX;
	pSelUnitMenu->nY = Top + nY;
	KPopupMenu::Popup(pSelUnitMenu, this, nIndex);
}

void KUiPick::LoadPickSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue;
	if (pConfigFile)
	{
		pConfigFile->GetInteger(PTitle, "F1", 0, &m_CurrentSelMagic);
		m_PickChooseFilterTxt.SetText(Data_Magic[m_CurrentSelMagic][0]);
		
		pConfigFile->GetInteger(PTitle, "F2", 0, (int*)(&m_nValue));
		m_PickChooseFilterEdit.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(PTitle, "F3", 1, (int*)(&m_nValue));
		m_PickJewelryCK.CheckButton(m_nValue > 0);
		g_pCoreShell->PAIOperation(GPI_P_SAVERAP,m_nValue,NULL,NULL);		
		
		pConfigFile->GetInteger(PTitle, "F4", 0, (int*)(&m_nValue));
		m_PickAllCK.CheckButton(m_nValue > 0);
		g_pCoreShell->PAIOperation(GPI_P_BPICKUP,m_nValue,NULL,NULL);		
		
		//pConfigFile->GetInteger(PTitle, "F5", 0, (int*)(&m_nValue));
		//if (m_nValue <= 0 )
			//m_PickFollowTargetEdit.SetIntText(0);
		//else
			//m_PickFollowTargetEdit.SetIntText(m_nValue);
		
		pConfigFile->GetInteger(PTitle, "F6", 0, (int*)(&m_nValue));
		m_PickFilterCK.CheckButton(m_nValue > 0);
		g_pCoreShell->PAIOperation(GPI_P_FILTEQUIP,m_nValue,NULL,NULL);		
		
		pConfigFile->GetInteger(PTitle, "F7", 0, &m_CurrentPickKind);
		m_PickAllEdit.SetText(Data_Pick[m_CurrentPickKind]);
		g_pCoreShell->PAIOperation(GPI_P_PICKUPK,m_CurrentPickKind,NULL,NULL);
		
		memset(m_cFilterMagic, 0, sizeof(m_cFilterMagic));
		memset(m_FilterMagicV, 0, sizeof(m_FilterMagicV));
		if (pConfigFile->IsSectionExist("F8"))
		{
			char szKeyName[10];
			for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
			{
				sprintf(szKeyName, "F8CN%d", i);
				pConfigFile->GetString("F8", szKeyName, "", m_cFilterMagic[i], sizeof(m_cFilterMagic[i]));
				sprintf(szKeyName, "F8VL%d", i);
				pConfigFile->GetInteger("F8", szKeyName, 0, &m_FilterMagicV[i]);
			}
		}	
		SetFilterMagicList();
		
		pConfigFile->GetInteger(PTitle, "F9", 1, (int*)(&m_nValue));
		m_PickPriceItemsCK.CheckButton(m_nValue > 0);
		g_pCoreShell->PAIOperation(GPI_P_PRICEITEMCK,m_nValue,NULL,NULL);	
		
		pConfigFile->GetInteger(PTitle, "F10", 100, (int*)(&m_nValue));
		if (m_nValue <= 0 )
		{	m_PickPriceItemsEdit.SetIntText(10000);
			g_pCoreShell->PAIOperation(GPI_P_PRICEITEM,m_PickPriceItemsEdit.GetIntNumber(),NULL,NULL);
		}
		else
		{
			m_PickPriceItemsEdit.SetIntText(m_nValue);
			g_pCoreShell->PAIOperation(GPI_P_PRICEITEM,m_nValue,NULL,NULL);
		}
			
		pConfigFile->GetInteger(PTitle, "F11", 1, (int*)(&m_nValue));
		m_PickLevelItemsCK.CheckButton(m_nValue > 0);
		g_pCoreShell->PAIOperation(GPI_S_LEVELITEMCK,m_nValue,NULL,NULL);	
		
		pConfigFile->GetInteger(PTitle, "F12", 1, (int*)(&m_nValue));
		if (m_nValue <= 0 )
		{	m_PickLevelItemsEdit.SetIntText(1);
			g_pCoreShell->PAIOperation(GPI_S_LEVELITEM,m_PickLevelItemsEdit.GetIntNumber(),NULL,NULL);
		}
		else
		{
			m_PickLevelItemsEdit.SetIntText(m_nValue);
			g_pCoreShell->PAIOperation(GPI_S_LEVELITEM,m_nValue,NULL,NULL);
		}
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiPick::SavePickSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		pConfigFile->WriteInteger(PTitle, "F7", m_CurrentPickKind);
		pConfigFile->WriteInteger(PTitle, "F3", m_PickJewelryCK.IsButtonChecked());
		pConfigFile->WriteInteger(PTitle, "F4", m_PickAllCK.IsButtonChecked());
		//pConfigFile->WriteInteger(PTitle, "F5", m_PickFollowTargetEdit.GetIntNumber());
		pConfigFile->WriteInteger(PTitle, "F6", m_PickFilterCK.IsButtonChecked());
		pConfigFile->WriteInteger(PTitle, "F2", m_PickChooseFilterEdit.GetIntNumber());
		pConfigFile->WriteInteger(PTitle, "F9", m_PickPriceItemsCK.IsButtonChecked());
		pConfigFile->WriteInteger(PTitle, "F10", m_PickPriceItemsEdit.GetIntNumber());
		pConfigFile->WriteInteger(PTitle, "F11", m_PickLevelItemsCK.IsButtonChecked());
		pConfigFile->WriteInteger(PTitle, "F12", m_PickLevelItemsEdit.GetIntNumber());
		char szKeyName[10];
		int Count = 0;
		for (int i = 0; i < defMAX_AUTO_FILTERL; i++)
		{
			sprintf(szKeyName, "F8CN%d", i);
			pConfigFile->WriteString("F8", szKeyName, m_cFilterMagic[i]);
			sprintf(szKeyName, "F8VL%d", i);
			pConfigFile->WriteInteger("F8", szKeyName, m_FilterMagicV[i]);
		}
		pConfigFile->WriteInteger("Pick", "F1", m_CurrentSelMagic);
	}
	g_UiBase.CloseAutoSettingFile(true);
}


////////////////////////////////////////////
//////////////////////FIGHT
////////////////////////////////////////////
KUiFight* KUiFight::m_pSelf = NULL;
//Tu ve
char szArray_FightBack[][64] =
{
	MSG_NON_SETTINGS,
	"Thæ ®Þa phï",
};
//end code

//San boss
char szArray_FightBoss[][64] =
{
	MSG_NON_SETTINGS,
	"Tr¸nh tÊt c¶ Boss",
	"Tr¸nh Boss Xanh",
	"Tr¸nh Boss Vµng",
	"Tr¸nh Boss TÝm",
};
//end code

//Bo qua
char szArray_Ingore[][64] =
{
	"Bá qua",
	"Lªn ngùa",
	"Xuèng ngùa",
};
void KUiFight::Initialize()
{
	AddChild(&m_BgFight);

	AddChild(&t_FightGuild);
	AddChild(&t_FightSkillGuild);
	AddChild(&t_FightEmiGuild);
	AddChild(&t_FightMenuSuportLight1Txt);
	AddChild(&t_FightMenuSuportLight2Txt);


	AddChild(&e_FightRadiusEdit);
	AddChild(&c_FightVicinity);
	AddChild(&e_FightVicinityEdit);
	AddChild(&c_FightUseSpace);
	AddChild(&c_FightFollowTarget);
	AddChild(&t_FightFollowTarget);
	AddChild(&c_FightNormal);
	AddChild(&c_FightPeople);
	AddChild(&t_FightMenu);
	AddChild(&p_FightMenuPopup);
	AddChild(&t_FightMenuSelfDefense);
	AddChild(&p_FightMenuSelfDefensePopup);
	AddChild(&t_FightMenuSuportBoss);
	AddChild(&p_FightMenuSuportBossPopup);
	AddChild(&t_FightMenuSuportSkill01);
	AddChild(&p_FightMenuSuportSkillPopup01);
	AddChild(&t_FightMenuSuportSkill02);
	AddChild(&p_FightMenuSuportSkillPopup02);
	AddChild(&t_FightMenuSuportSkill03);
	AddChild(&p_FightMenuSuportSkillPopup03);
	AddChild(&t_FightMenuSupportAura1);
	AddChild(&p_FightMenuSupportAura1Popup);
	AddChild(&t_FightMenuSupportAura2);
	AddChild(&p_FightMenuSupportAura2Popup);
	AddChild(&t_FightMenuSupportWhenManaSmallText);
	AddChild(&t_FightMenuSupportWhenManaSmall);
	AddChild(&p_FightMenuSupportWhenManaSmall);
	AddChild(&t_FightMenuSupportRightHandText);
	AddChild(&t_FightMenuSupportRightHand);
	AddChild(&p_FightMenuSupportRightHand);
	memset(m_CurrentSelOption,0,sizeof(m_CurrentSelOption));
	memset(m_CurrentSelMagic,0,sizeof(m_CurrentSelMagic));
	memset(m_CurrentSelAura,0,sizeof(m_CurrentSelAura));
	memset(m_CurrentSelWhenManaSmall,0,sizeof(m_CurrentSelWhenManaSmall));
	memset(m_CurrentSelRightHand,0,sizeof(m_CurrentSelRightHand));
}

void KUiFight::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	m_BgFight.Init(pIni, "BgFight");

	t_FightGuild.Init(pIni, "FightGuide");
	t_FightSkillGuild.Init(pIni, "FightSkillsGuide");
	t_FightEmiGuild.Init(pIni, "FightEmeiGuide");
	t_FightMenuSuportLight1Txt.Init(pIni, "FightMenuSuportLight1Txt");
	t_FightMenuSuportLight2Txt.Init(pIni, "FightMenuSuportLight2Txt");

	e_FightRadiusEdit.Init(pIni, "FightRadiusEdit");
	c_FightVicinity.Init(pIni, "FightVicinity");
	e_FightVicinityEdit.Init(pIni, "FightVicinityEdit");
	c_FightUseSpace.Init(pIni, "FightUseSpace");
	c_FightFollowTarget.Init(pIni, "FightFollowTarget");
	t_FightFollowTarget.Init(pIni, "TextFightFollowTarget");	
	
	c_FightNormal.Init(pIni, "FightNormal");
	c_FightPeople.Init(pIni, "FightPeople");
	
	t_FightMenu.Init(pIni,"FightMenu");
	t_FightMenu.SetText("Bá qua");
	p_FightMenuPopup.Init(pIni,"FightMenuPopup");
	
	t_FightMenuSelfDefense.Init(pIni,"FightMenuSelfDefense");
	t_FightMenuSelfDefense.SetText(MSG_NON_SETTINGS);
	p_FightMenuSelfDefensePopup.Init(pIni,"FightMenuSelfDefensePopup");
	
	t_FightMenuSuportBoss.Init(pIni,"FightMenuSuportBoss");
	t_FightMenuSuportBoss.SetText(MSG_NON_SETTINGS);
	p_FightMenuSuportBossPopup.Init(pIni,"FightMenuSuportBossPopup");

	t_FightMenuSuportSkill01.Init(pIni,"FightMenuSuportSkill01");
	t_FightMenuSuportSkill01.SetText(MSG_NON_SETTINGS);
	p_FightMenuSuportSkillPopup01.Init(pIni,"FightMenuSuportSkillPopup01");
	t_FightMenuSuportSkill02.Init(pIni,"FightMenuSuportSkill02");
	t_FightMenuSuportSkill02.SetText(MSG_NON_SETTINGS);
	p_FightMenuSuportSkillPopup02.Init(pIni,"FightMenuSuportSkillPopup02");
	t_FightMenuSuportSkill03.Init(pIni,"FightMenuSuportSkill03");
	t_FightMenuSuportSkill03.SetText(MSG_NON_SETTINGS);
	p_FightMenuSuportSkillPopup03.Init(pIni,"FightMenuSuportSkillPopup03");

	t_FightMenuSupportAura1.Init(pIni,"FightMenuSupportAura1");
	t_FightMenuSupportAura1.SetText(MSG_NON_SETTINGS);
	p_FightMenuSupportAura1Popup.Init(pIni,"FightMenuSupportAura1Popup");
	t_FightMenuSupportAura2.Init(pIni,"FightMenuSupportAura2");
	t_FightMenuSupportAura2.SetText(MSG_NON_SETTINGS);
	p_FightMenuSupportAura2Popup.Init(pIni,"FightMenuSupportAura2Popup");
	t_FightMenuSupportWhenManaSmallText.Init(pIni,"FightMenuSupportWhenManaSmallText");
	t_FightMenuSupportWhenManaSmall.Init(pIni,"FightMenuSupportWhenManaSmall");
	t_FightMenuSupportWhenManaSmall.SetText(MSG_NON_SETTINGS);
	p_FightMenuSupportWhenManaSmall.Init(pIni,"FightMenuSupportWhenManaSmallPopup");
	
	t_FightMenuSupportRightHandText.Init(pIni,"FightMenuSupportRightHandText");
	t_FightMenuSupportRightHand.Init(pIni,"FightMenuSupportRightHand");
	t_FightMenuSupportRightHand.SetText(MSG_NON_SETTINGS);
	p_FightMenuSupportRightHand.Init(pIni,"FightMenuSupportRightHandPopup");
	
	LoadFightSetting();
	
}

int KUiFight::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&c_FightVicinity)
		{
			if (c_FightVicinity.IsButtonChecked())
			{
				c_FightVicinity.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_F_VINCINITY, true, NULL, NULL);
			}
			else
			{
				c_FightVicinity.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_F_VINCINITY, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_FightUseSpace)
		{
			if (c_FightUseSpace.IsButtonChecked())
			{
				c_FightUseSpace.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_F_SPACE, true, NULL, NULL);
			}
			else
			{
				c_FightUseSpace.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_F_SPACE, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_FightFollowTarget)
		 {
			if (c_FightFollowTarget.IsButtonChecked())
			 {
				 c_FightFollowTarget.CheckButton(true);
				 g_pCoreShell->PAIOperation(GPI_F_FOLLOW_TAGET, true, NULL, NULL);
			 }
			 else
			 {
				 c_FightFollowTarget.CheckButton(false);
				 g_pCoreShell->PAIOperation(GPI_F_FOLLOW_TAGET, false, NULL, NULL);
			 }
		 }	
		else if (uParam == (unsigned int)(KWndWindow*)&c_FightNormal)
		{
			if (c_FightNormal.IsButtonChecked())
			{
				c_FightNormal.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_F_NORMAL, true, NULL, NULL);
			}
			else
			{
				c_FightNormal.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_F_NORMAL, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_FightPeople)
		{
			if (c_FightPeople.IsButtonChecked())
			{
				c_FightPeople.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_F_PEOPLE, true, NULL, NULL);
			}
			else
			{
				c_FightPeople.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_F_PEOPLE, false, NULL, NULL);
			}
		}	
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSelfDefensePopup)
			SupportAnyOption(enumDefense);	
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSuportBossPopup)
			SupportAnyOption(enumBoss);	
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuPopup)
			SupportAnyOption(enumFight);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSuportSkillPopup01)
			SupportSkillPopup(enumMagic_ID1);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSuportSkillPopup02)
			SupportSkillPopup(enumMagic_ID2);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSuportSkillPopup03)
			SupportSkillPopup(enumMagic_ID3);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSupportAura1Popup)
			SupportAuraPopup(enumMagic_ID1);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSupportAura2Popup)
			SupportAuraPopup(enumMagic_ID2);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSupportWhenManaSmall)
			SupportSkillWhenManaSmallPopup(enumMagic_ID1);
		else if (uParam == (unsigned int)(KWndWindow*)&p_FightMenuSupportRightHand)
			SetSupportRightHandPopup(enumMagic_ID1);
		SaveFightSetting();
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{

			if (HIWORD(nParam) == MENU_SELECT_DEFENSE && (short)(LOWORD(nParam)) >= 0)
				SetSupportAnyOption(enumDefense, LOWORD(nParam));
			else if (HIWORD(nParam) == MENU_SELECT_BOSS && (short)(LOWORD(nParam)) >= 0)
				SetSupportAnyOption(enumBoss, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_FIGHT && (short)(LOWORD(nParam)) >= 0)
				SetSupportAnyOption(enumFight, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL1 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID1, LOWORD(nParam));
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL2 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID2, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_SUPPORT_SKILL3 && (short)(LOWORD(nParam)) >= 0)
				SetSupportSkill(enumMagic_ID3, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_AURA_SKILL1 && (short)(LOWORD(nParam)) >= 0)
				SetSupportAura(enumMagic_ID1, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_AURA_SKILL2 && (short)(LOWORD(nParam)) >= 0)
				SetSupportAura(enumMagic_ID2, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_MANA_SKILL_SMALL && (short)(LOWORD(nParam)) >= 0)
				SetSupportWhenManaSmall(enumMagic_ID1, LOWORD(nParam));	
			else if (HIWORD(nParam) == MENU_SELECT_RIGHT_HAND_SKILL && (short)(LOWORD(nParam)) >= 0)
				SetSupportRightHand(enumMagic_ID1, LOWORD(nParam));	
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiFight::OnCheckInput()
{
	int n_Radius = e_FightRadiusEdit.GetIntNumber();
	int n_Vincinity = e_FightVicinityEdit.GetIntNumber();
	g_pCoreShell->PAIOperation(GPI_F_RADIUS_NUM, n_Radius, NULL, NULL);
	g_pCoreShell->PAIOperation(GPI_F_VINCINITY_NUM, n_Vincinity, NULL, NULL);
}

void KUiFight::SupportAnyOption(int nIndex)
{
	int nActionDataCount = 0;
	switch(nIndex)
	{
	case enumDefense:
		nActionDataCount = sizeof(szArray_FightBack) / sizeof(szArray_FightBack[0]);
		break;
	case enumBoss:
		nActionDataCount = sizeof(szArray_FightBoss) / sizeof(szArray_FightBoss[0]);
		break;
	case enumFight: //Bo qua
		nActionDataCount = sizeof(szArray_Ingore) / sizeof(szArray_Ingore[0]);
		break;
	default:
		return;
	}
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		switch (nIndex)
		{
		case enumDefense:
			strncpy(pSelUnitMenu->Items[i].szData, szArray_FightBack[i], sizeof(szArray_FightBack[i]));
			break;
		case enumBoss:
			strncpy(pSelUnitMenu->Items[i].szData, szArray_FightBoss[i], sizeof(szArray_FightBoss[i]));
			break;
		case enumFight: //Bo qua
			strncpy(pSelUnitMenu->Items[i].szData, szArray_Ingore[i], sizeof(szArray_Ingore[i]));
			break;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX, nY;
	int nWidth;
	GetSize(&nWidth, NULL);
	switch (nIndex)
	{
	case enumDefense:
		t_FightMenuSelfDefense.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_DEFENSE);
		break;
	case enumBoss:
		t_FightMenuSuportBoss.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_BOSS);
		break;
	case enumFight:
		t_FightMenu.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_FIGHT);
		break;
	}
}

void KUiFight::SetSupportAnyOption(int nIndex, int nID)
{
	if (nID > 0 && nID <= MAX_SKILL)
	{ 
		switch(nIndex)
		{
			case enumDefense:
				m_CurrentSelOption[enumDefense] = nID;
				t_FightMenuSelfDefense.SetText(szArray_FightBack[nID]);
				g_pCoreShell->PAIOperation(GPI_F_DEFENSE, nID, NULL, NULL);
				break;
			case enumBoss:
				m_CurrentSelOption[enumBoss] = nID;
				t_FightMenuSuportBoss.SetText(szArray_FightBoss[nID]);
				g_pCoreShell->PAIOperation(GPI_F_FIGHTBOSS, nID, NULL, NULL);
				break;
			case enumFight:
				m_CurrentSelOption[enumFight] = nID;
				t_FightMenu.SetText(szArray_Ingore[nID]);
				g_pCoreShell->PAIOperation(GPI_F_RIDER, nID, NULL, NULL);
				break;
		}
	}
	else
	{
		switch(nIndex)
		{
			case enumDefense:
			m_CurrentSelOption[enumDefense] = 0;
			t_FightMenuSelfDefense.SetText(MSG_NON_SETTINGS);
			break;
			case enumBoss:
			m_CurrentSelOption[enumBoss] = 0;
			t_FightMenuSuportBoss.SetText(MSG_NON_SETTINGS);
			g_pCoreShell->PAIOperation(GPI_F_FIGHTBOSS, 0, NULL, NULL);
			break;
			case enumFight:
			m_CurrentSelOption[enumFight] = 0;
			t_FightMenu.SetText("Bá qua");
			g_pCoreShell->PAIOperation(GPI_F_RIDER, 0, NULL, NULL);
			break;
		}
	}
}

void KUiFight::SupportSkillPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(0,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(0,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int nWidth;
	GetSize(&nWidth, NULL);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		t_FightMenuSuportSkill01.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL1);
		break;
	case enumMagic_ID2:
		t_FightMenuSuportSkill02.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL2);
		break;
	case enumMagic_ID3:
		t_FightMenuSuportSkill03.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_SUPPORT_SKILL3);
		break;		
	}
}

void KUiFight::SetSupportSkill(int nIndex, int nID)
{
	char Name[128];
	if (nID > 0 && nID <= MAX_SKILL)
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelMagic[enumMagic_ID1] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSuportSkill01.SetText(Name);
		}
		else if (nIndex == enumMagic_ID2)
		{
			m_CurrentSelMagic[enumMagic_ID2] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSuportSkill02.SetText(Name);
		}
		else if (nIndex == enumMagic_ID3)
		{
			m_CurrentSelMagic[enumMagic_ID3] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSuportSkill03.SetText(Name);
		}

	}
	else
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelMagic[enumMagic_ID1] = 0;
			t_FightMenuSuportSkill01.SetText(MSG_NON_SETTINGS);
		}
		else if (nIndex == enumMagic_ID2)
		{
			m_CurrentSelMagic[enumMagic_ID2] = 0;
			t_FightMenuSuportSkill02.SetText(MSG_NON_SETTINGS);
		}
		else if (nIndex == enumMagic_ID3)
		{
			m_CurrentSelMagic[enumMagic_ID3] = 0;
			t_FightMenuSuportSkill03.SetText(MSG_NON_SETTINGS);
		}
	}	
	g_pCoreShell->PAIOperation(GPI_F_SUPPORT_SKILLID,nIndex,nID,NULL);
}

void KUiFight::SupportAuraPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(2,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(2,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int nWidth;
	GetSize(&nWidth, NULL);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		t_FightMenuSupportAura1.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_AURA_SKILL1);
		break;
	case enumMagic_ID2:
		t_FightMenuSupportAura2.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_AURA_SKILL2);
		break;		
	}
}

void KUiFight::SetSupportAura(int nIndex, int nID)
{
	char Name[128];
	if (nID > 0 && nID <= MAX_SKILL)
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelAura[enumMagic_ID1] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSupportAura1.SetText(Name);
		}
		else if (nIndex == enumMagic_ID2)
		{
			m_CurrentSelAura[enumMagic_ID2] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSupportAura2.SetText(Name);
		}

	}
	else
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelAura[enumMagic_ID1] = 0;
			t_FightMenuSupportAura1.SetText(MSG_NON_SETTINGS);
		}
		else if (nIndex == enumMagic_ID2)
		{
			m_CurrentSelAura[enumMagic_ID2] = 0;
			t_FightMenuSupportAura2.SetText(MSG_NON_SETTINGS);
		}
	}	
	g_pCoreShell->PAIOperation(GPI_F_AURA_SKILLID,nIndex,nID,NULL);
}

void KUiFight::SupportSkillWhenManaSmallPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(1,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(1,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int nWidth;
	GetSize(&nWidth, NULL);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		t_FightMenuSupportWhenManaSmall.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_MANA_SKILL_SMALL);
		break;	
	}
}

void KUiFight::SetSupportWhenManaSmall(int nIndex, int nID)
{
	char Name[128];
	if (nID > 0 && nID <= MAX_SKILL)
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelWhenManaSmall[enumMagic_ID1] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSupportWhenManaSmall.SetText(Name);
		}
	}
	else
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelWhenManaSmall[enumMagic_ID1] = 0;
			t_FightMenuSupportWhenManaSmall.SetText(MSG_NON_SETTINGS);
		}
	}	
	g_pCoreShell->PAIOperation(GPI_F_LEFT_SKILL,nIndex,nID,NULL);
}

void KUiFight::SetSupportRightHandPopup(int nIndex)
{
	int nActionDataCount = g_pCoreShell->FindSkillInfo(3,0) + 1;
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		if (i == 0)
		{
			strncpy(pSelUnitMenu->Items[i].szData, MSG_NON_SETTINGS, sizeof(MSG_NON_SETTINGS));
			pSelUnitMenu->Items[i].uID = 0;
		}
		else
		{
			int _nID = g_pCoreShell->FindSkillInfo(3,i);
			g_pCoreShell->GetSkillName(_nID, pSelUnitMenu->Items[i].szData);
			pSelUnitMenu->Items[i].uID = _nID;
		}
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX = 0, nY = 0;
	int nWidth;
	GetSize(&nWidth, NULL);
	KIniFile	Ini;
	switch (nIndex)
	{
	case enumMagic_ID1:
		t_FightMenuSupportRightHand.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_RIGHT_HAND_SKILL);
		break;	
	}
}


void KUiFight::SetSupportRightHand(int nIndex, int nID)
{
	char Name[128];
	if (nID > 0 && nID <= MAX_SKILL)
	{
		if (nIndex == enumMagic_ID1)
		{ 
			m_CurrentSelRightHand[enumMagic_ID1] = nID;
			g_pCoreShell->GetSkillName(nID, Name);
			t_FightMenuSupportRightHand.SetText(Name);
		}
	}
	else
	{
		if (nIndex == enumMagic_ID1)
		{
			m_CurrentSelRightHand[enumMagic_ID1] = 0;
			t_FightMenuSupportRightHand.SetText(MSG_NON_SETTINGS);
		}
	}	
	g_pCoreShell->PAIOperation(GPI_F_RIGHTHAND_SKILL,nIndex,nID,NULL);
}

void KUiFight::LoadFightSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue;
	char szKeyName[10];
	if (pConfigFile)
	{
		pConfigFile->GetInteger(FTitle, "F1", 800, (int*)(&m_nValue));
		if (m_nValue <= 0)
			m_nValue =	800;
			e_FightRadiusEdit.SetIntText(m_nValue);
			g_pCoreShell->PAIOperation(GPI_F_RADIUS_NUM, m_nValue, NULL, NULL);
		pConfigFile->GetInteger(FTitle, "F2", 75, (int*)(&m_nValue));
		if (m_nValue <= 0)
			m_nValue = 75;
			e_FightVicinityEdit.SetIntText(m_nValue);
			g_pCoreShell->PAIOperation(GPI_F_VINCINITY_NUM, m_nValue, NULL, NULL);
			
		pConfigFile->GetInteger(FTitle, "F3", 1, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_FightVicinity.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_F_VINCINITY, m_nValue, NULL, NULL);
		pConfigFile->GetInteger(FTitle, "F4", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_FightUseSpace.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_F_SPACE, m_nValue, NULL, NULL);
		 pConfigFile->GetInteger(FTitle, "F5", 1, (int*)(&m_nValue));
		if (m_nValue > 0)
			 m_nValue = true;
		c_FightFollowTarget.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_F_FOLLOW_TAGET, m_nValue, NULL, NULL);
		pConfigFile->GetInteger(FTitle, "F6", 1, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_FightNormal.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_F_NORMAL, m_nValue, NULL, NULL);
		pConfigFile->GetInteger(FTitle, "F7", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_FightPeople.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_F_PEOPLE, m_nValue, NULL, NULL);
		for (int i = 0; i < defMAX_ARRAY_OPTION; i ++)
		{
			sprintf(szKeyName, "F8%d", i);
			pConfigFile->GetInteger(FTitle, szKeyName, 0, &m_CurrentSelOption[i]);
			SetSupportAnyOption(i, m_CurrentSelOption[i]);
		}
		for (i = 0; i < defMAX_ARRAY_STATESKILL; i ++)
		{
			sprintf(szKeyName, "F9%d", i);
			pConfigFile->GetInteger(FTitle, szKeyName, 0, &m_CurrentSelMagic[i]);
			SetSupportSkill(i, m_CurrentSelMagic[i]);
		}
		for (i = 0; i < defMAX_ARRAY_AURA; i ++)
		{
			sprintf(szKeyName, "F10%d", i);
			pConfigFile->GetInteger(FTitle, szKeyName, 0, &m_CurrentSelAura[i]);
			SetSupportAura(i, m_CurrentSelAura[i]);
		}
		for (i = 0; i < 10; i ++)
		{
			sprintf(szKeyName, "F11%d", i);
			pConfigFile->GetInteger(FTitle, szKeyName, 0, &m_CurrentSelWhenManaSmall[i]);
			SetSupportWhenManaSmall(i, m_CurrentSelWhenManaSmall[i]);
		}
		for (i = 0; i < defMAX_ARRAY_FIGHT; i ++)
		{
			sprintf(szKeyName, "F12%d", i);
			pConfigFile->GetInteger(FTitle, szKeyName, 0, &m_CurrentSelRightHand[i]);
			SetSupportRightHand(i, m_CurrentSelRightHand[i]);
		}
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiFight::SaveFightSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	char szKeyName[10];
	if (pConfigFile)
	{
		pConfigFile->WriteInteger(FTitle, "F1",e_FightRadiusEdit.GetIntNumber());
		pConfigFile->WriteInteger(FTitle, "F2",e_FightVicinityEdit.GetIntNumber());
		pConfigFile->WriteInteger(FTitle, "F3",c_FightVicinity.IsButtonChecked());
		pConfigFile->WriteInteger(FTitle, "F4",c_FightUseSpace.IsButtonChecked());
		pConfigFile->WriteInteger(FTitle, "F5",c_FightFollowTarget.IsButtonChecked());
		pConfigFile->WriteInteger(FTitle, "F6",c_FightNormal.IsButtonChecked());
		pConfigFile->WriteInteger(FTitle, "F7",c_FightPeople.IsButtonChecked());

		for (int i = 0; i < defMAX_ARRAY_OPTION; i ++)
		{
			sprintf(szKeyName, "F8%d", i);
			pConfigFile->WriteInteger(FTitle, szKeyName, m_CurrentSelOption[i]);
		}

		for (i = 0; i < defMAX_ARRAY_STATESKILL; i ++)
		{
			sprintf(szKeyName, "F9%d", i);
			pConfigFile->WriteInteger(FTitle, szKeyName, m_CurrentSelMagic[i]);
		}

		for (i = 0; i < defMAX_ARRAY_AURA; i ++)
		{
			sprintf(szKeyName, "F10%d", i);
			pConfigFile->WriteInteger(FTitle, szKeyName, m_CurrentSelAura[i]);
		}

		for (i = 0; i < 10; i ++)
		{
			sprintf(szKeyName, "F11%d", i);
			pConfigFile->WriteInteger(FTitle, szKeyName, m_CurrentSelWhenManaSmall[i]);
		}
		
		for (i = 0; i < defMAX_ARRAY_FIGHT; i ++)
		{
			sprintf(szKeyName, "F12%d", i);
			pConfigFile->WriteInteger(FTitle, szKeyName, m_CurrentSelRightHand[i]);
		}
		
	}
	g_UiBase.CloseAutoSettingFile(true);
}


////////////////////////////////////////////
//////////////////////OTHER
////////////////////////////////////////////
KUiOther* KUiOther::m_pSelf = NULL;

char szArray_Team[][64] =
{
	MSG_NON_SETTINGS,
	"Tù mêi nhãm tÊt c¶",
	"Tù mêi nhãm theo danh s¸ch",
};


char szArray_Team_Join_Group[][64] =
{
	MSG_NON_SETTINGS,
	"Tù nhËn tÊt c¶ lêi mêi nhãm",
	"ChØ nhËn nhãm cã trong danh s¸ch",
	"Tõ chèi vµo nhãm",
};

void KUiOther::Initialize()
{
	AddChild(&m_BgOther);

	AddChild(&t_OtherPartyGuide);
	AddChild(&t_OtherOptimalGuide);
	AddChild(&t_OtherOptimalNpcTxt);
	AddChild(&t_OtherOptimalPlayerTxt);
	AddChild(&t_OtherOptimalAniTxt);


	AddChild(&m_OtherPartyCK);
	AddChild(&m_OtherPartyEdit);
	AddChild(&m_OtherPartyPopup);
	AddChild(&m_OtherPartyNameTxt);
	AddChild(&m_OtherPartyNamePopup);
	AddChild(&m_OtherPartyListTxt);
	AddChild(&m_OtherPartyListSbc);
	m_OtherPartyListTxt.SetScrollbar(&m_OtherPartyListSbc);
	AddChild(&m_OtherPartyListAdd);
	AddChild(&m_OtherPartyListUp);
	AddChild(&m_OtherPartyListDown);
	AddChild(&m_OtherPartyListDel);
	AddChild(&m_OtherPartyListAll);
	AddChild(&m_OtherPartyJoinCK);
	AddChild(&m_OtherPartyJoinTxt);
	AddChild(&m_OtherPartyJoinPopup);
	AddChild(&m_OtherOptimalNpcCK);
	AddChild(&m_OtherOptimalPlayerCK);
	AddChild(&m_OtherOptimalAniCK); 
	memset(m_ListName,0,sizeof(m_ListName));
	memset(m_ListNameSave,0,sizeof(m_ListNameSave));
	memset(m_cNearName,0,sizeof(m_cNearName));
	// memset(m_bSaveMenuPT,0,sizeof(m_bSaveMenuPT));
	// memset(m_bSaveMenuJPT,0,sizeof(m_bSaveMenuJPT));
	m_bNamePT = FALSE;
}

void KUiOther::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	m_BgOther.Init(pIni,"BgOther");

	t_OtherPartyGuide.Init(pIni,"OtherPartyGuide");
	t_OtherOptimalGuide.Init(pIni,"OtherOptimalGuide");
	t_OtherOptimalNpcTxt.Init(pIni,"OtherOptimalNpcTxt");
	t_OtherOptimalPlayerTxt.Init(pIni,"OtherOptimalPlayerTxt");
	t_OtherOptimalAniTxt.Init(pIni,"OtherOptimalAniTxt");

	m_OtherPartyCK.Init(pIni,"OtherPartyCK");
	m_OtherPartyEdit.Init(pIni,"OtherPartyEdit");
	m_OtherPartyPopup.Init(pIni,"OtherPartyPopup");
	m_OtherPartyNameTxt.Init(pIni,"OtherPartyNameTxt");
	m_OtherPartyNamePopup.Init(pIni,"OtherPartyNamePopup");
	m_OtherPartyListTxt.Init(pIni,"OtherPartyListTxt");
	m_OtherPartyListSbc.Init(pIni,"OtherPartyListSbc");
	m_OtherPartyListAdd.Init(pIni,"OtherPartyListAdd");
	m_OtherPartyListUp.Init(pIni,"OtherPartyListUp");
	m_OtherPartyListDown.Init(pIni,"OtherPartyListDown");
	m_OtherPartyListDel.Init(pIni,"OtherPartyListDel");
	m_OtherPartyListAll.Init(pIni,"OtherPartyListAll");
	m_OtherPartyJoinCK.Init(pIni,"OtherPartyJoinCK");
	m_OtherPartyJoinTxt.Init(pIni,"OtherPartyJoinTxt");
	m_OtherPartyJoinPopup.Init(pIni,"OtherPartyJoinPopup");
	m_OtherOptimalNpcCK.Init(pIni,"OtherOptimalNpcCK");	
	m_OtherOptimalPlayerCK.Init(pIni,"OtherOptimalPlayerCK");
	m_OtherOptimalAniCK.Init(pIni,"OtherOptimalAniCK");
	LoadOtherSetting();
	
}

int KUiOther::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyCK)
		{
			if (m_OtherPartyCK.IsButtonChecked())
			{
				m_OtherPartyCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_O_PARTYCK, true, NULL, NULL);
			}
			else
			{
				m_OtherPartyCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_O_PARTYCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyPopup)
			MenuParty();
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyNamePopup)
			MenuListParty();
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyListAdd)
			{
				if (m_bNamePT == TRUE)
					InsertNameList(); 
			}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyListUp)
		{		
			int nSel = m_OtherPartyListTxt.GetCurSel();
			memset(m_ListNameSave[0], 0, sizeof(m_ListNameSave[0]));	
			if (nSel <= 0)
				return FALSE;	
				
			for (int i = 0; i < 64; i++)
			{
				m_ListNameSave[0][i] = m_ListName[nSel][i];	
				m_ListName[nSel][i] = m_ListName[nSel-1][i];
				m_ListName[nSel-1][i] = m_ListNameSave[0][i];
			}
		
			SetNameListPT();	
			m_OtherPartyListTxt.SetCurSel(nSel-1);
			memset(m_ListNameSave, 0, sizeof(m_ListNameSave));
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyListDown)
		{		
			int nSel = m_OtherPartyListTxt.GetCurSel();
			int nCount = 0;
			int i;
			memset(m_ListNameSave[0], 0, sizeof(m_ListNameSave[0]));	
			
			for (i = 0; i < defMAX_LIST_PARTY; i++)
			{
				if (m_ListName[i][0])
				{
					nCount++;
				}
			}
			if (nSel < 0 || nSel >= nCount-1)
				return FALSE;
			for (i = 0; i < 64; i++)
			{
				m_ListNameSave[0][i] = m_ListName[nSel][i];
				m_ListName[nSel][i] = m_ListName[nSel+1][i];
				m_ListName[nSel+1][i] = m_ListNameSave[0][i];
			}
			
			SetNameListPT();	
			if (nSel == nCount-1)
				m_OtherPartyListTxt.SetCurSel(nSel);
			else
				m_OtherPartyListTxt.SetCurSel(nSel+1);
			memset(m_ListNameSave, 0, sizeof(m_ListNameSave));	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyListDel)
		{
			int nSel = m_OtherPartyListTxt.GetCurSel();
			int nCount = 0;
			int i;
			memset(m_ListName[nSel], 0, sizeof(m_ListName[nSel]));	

			for (i = 0; i < defMAX_LIST_PARTY; i++)
			{
				if (m_ListName[i][0])
				{
					nCount++;
				}
			}
			
			for (i = nSel; i < nCount; i++)			
			{
				for (int j = 0; j < 3; j++)
				{
					m_ListName[i][j] = m_ListName[i+1][j];
					m_ListName[i+1][j] = 0;				
				}		
			}			
			nCount--;			
							
			SetNameListPT();			
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyListAll)
		{
			memset(m_ListName, 0, sizeof(m_ListName));
			SetNameListPT();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyJoinPopup)
			MenuJoinParty();
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherPartyJoinCK)
		{
			if (m_OtherPartyJoinCK.IsButtonChecked())
			{
				m_OtherPartyJoinCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT, true, NULL, NULL);
			}
			else
			{
				m_OtherPartyJoinCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherOptimalNpcCK)
		{
			if (m_OtherOptimalNpcCK.IsButtonChecked())
			{
				m_OtherOptimalNpcCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_NPC, true, NULL, NULL);
			}
			else
			{
				m_OtherOptimalNpcCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_NPC, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherOptimalPlayerCK)
		{
			if (m_OtherOptimalPlayerCK.IsButtonChecked())
			{
				m_OtherOptimalPlayerCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_PLAYER, true, NULL, NULL);
			}
			else
			{
				m_OtherOptimalPlayerCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_PLAYER, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_OtherOptimalAniCK)
		{
			if (m_OtherOptimalAniCK.IsButtonChecked())
			{
				m_OtherOptimalAniCK.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_ANI, true, NULL, NULL);
			}
			else
			{
				m_OtherOptimalAniCK.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_ANI, false, NULL, NULL);
			}
		}
		SaveOtherSetting();		
		break;
	case WND_N_EDIT_CHANGE:

		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == MENU_SELECT_PT && (short)(LOWORD(nParam)) >= 0)
			{
				SetMenuParty(LOWORD(nParam));
			}
			else if (HIWORD(nParam) == MENU_SELECT_LISTPT && (short)(LOWORD(nParam)) >= 0)
			{
				SetMenuListParty(LOWORD(nParam));
				m_cNearNameSave = m_pNearbyPlayersList[(short)(LOWORD(nParam))].Name;
				m_bNamePT = TRUE;
			}			
			else if (HIWORD(nParam) == MENU_SELECT_JPT && (short)(LOWORD(nParam)) >= 0)
			{
				if ( LOWORD(nParam) == 2)		
				{	
					if (m_ListName[0][0] == NULL)
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.byParamSize = 0;
						Msg.byPriority = 0;
						Msg.eType = SMT_NORMAL;
						Msg.uReservedForUi = 0;
						strcpy(Msg.szMessage, "Danh s¸ch trèng th­a ®¹i hiÖp!");
						KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
					}
					else
					{
						SetMenuJoinParty(LOWORD(nParam));
					}
				
				}
				else
				{
					SetMenuJoinParty(LOWORD(nParam));
				}
			}
		}	
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiOther::MenuParty()
{
	int nActionDataCount = 0;
		nActionDataCount = sizeof(szArray_Team) / sizeof(szArray_Team[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		strncpy(pSelUnitMenu->Items[i].szData, szArray_Team[i], 64);
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX, nY;
	int nWidth;
	GetSize(&nWidth, NULL);

	m_OtherPartyEdit.GetAbsolutePos(&nX, &nY);
	pSelUnitMenu->nX = nX;
	pSelUnitMenu->nY = nY;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_PT);

}

void KUiOther::SetMenuParty(int nID)
{
	if (nID < 0 || nID > sizeof(szArray_Team))
		return;
	m_bSaveMenuPT = nID;

	m_OtherPartyEdit.SetText(szArray_Team[nID]);
	// g_pCoreShell->PAIOperation(GPI_O_PARTYINVITE, FALSE, NULL, NULL);
	// g_pCoreShell->PAIOperation(GPI_O_PARTYLIST, FALSE, NULL, NULL);
	switch (nID)
	{
		case enumMagic_ID1:
			break;
		case enumMagic_ID2:
			g_pCoreShell->PAIOperation(GPI_O_PARTYINVITE, TRUE, NULL, NULL);
			break;
		case enumMagic_ID3:
			g_pCoreShell->PAIOperation(GPI_O_PARTYINVITE, TRUE, NULL, NULL);
			g_pCoreShell->PAIOperation(GPI_O_PARTYLIST, TRUE, NULL, NULL);
			break;
		default:
			return;
	}
}

void KUiOther::MenuListParty()
{
	if (m_pNearbyPlayersList)
	{
		free (m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}
	int nActionDataCount = g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, 0, 0);
	if (nActionDataCount > 0) 
	{
		struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
		if (pSelUnitMenu == NULL)
			return;
		KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
		pSelUnitMenu->nNumItem = 0;
		pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;

		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nActionDataCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, (unsigned int)m_pNearbyPlayersList, nActionDataCount);
			for (int i = 0; i< nActionDataCount;i++)
			{
				strncpy(pSelUnitMenu->Items[i].szData, m_pNearbyPlayersList[i].Name, 63);
				pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
				pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
				pSelUnitMenu->nNumItem++;
			}
		}
		int nWidth;
		GetSize(&nWidth, NULL);
		int nX = 0, nY = 0;
		KIniFile	Ini;
		m_OtherPartyNameTxt.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_LISTPT);
	} 
	else 
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.byParamSize = 0;
		Msg.byPriority = 0;
		Msg.eType = SMT_NORMAL;
		Msg.uReservedForUi = 0;
		strcpy(Msg.szMessage, "N¬i nµy ®ång kh«ng m«ng qu¹nh, kh«ng mét bãng ng­êi!");
		KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
	}
}

void KUiOther::SetMenuListParty(int nID)
{
	strcpy(m_cNearName,m_pNearbyPlayersList[nID].Name);
	m_OtherPartyNameTxt.SetText(m_cNearName);
	
}

BOOL KUiOther::InsertNameList()
{	
	for (int i = 0; i < defMAX_LIST_PARTY; i ++)
	{
		if (m_ListName[i][0]&& strcmp(m_cNearNameSave, m_ListName[i]) == 0)
		{
			break;
		}
		else if (m_ListName[i][0] == 0)
		{
			break;
		}
	}
	if (i < defMAX_LIST_PARTY)
	{
		strcpy(m_ListName[i], m_cNearNameSave);
		SetNameListPT();
	}
		return TRUE;
}

void KUiOther::SetNameListPT()
{
	char szList[32];
	m_OtherPartyListTxt.ResetContent();
	for (int i = 0; i < 15; i ++)
	{
		g_pCoreShell->PAIOperation(GPI_O_PARTYLISTNAME, (unsigned int)&m_ListName[i], i, NULL);
		if(m_ListName[i][0])
		{
			sprintf(szList,"%s", m_ListName[i]);
			m_OtherPartyListTxt.AddString(i, szList,"","");
		}
	}
}

void KUiOther::MenuJoinParty()
{
	int nActionDataCount = 0;
		nActionDataCount = sizeof(szArray_Team_Join_Group) / sizeof(szArray_Team_Join_Group[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		strncpy(pSelUnitMenu->Items[i].szData, szArray_Team_Join_Group[i],64);
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int nX, nY;
	int nWidth;
	GetSize(&nWidth, NULL);

	m_OtherPartyJoinTxt.GetAbsolutePos(&nX, &nY);
	pSelUnitMenu->nX = nX;
	pSelUnitMenu->nY = nY;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_JPT);

}

void KUiOther::SetMenuJoinParty(int nID)
{
	if (nID < 0 || nID > sizeof(szArray_Team_Join_Group))
		return;
	m_OtherPartyJoinTxt.SetText(szArray_Team_Join_Group[nID]);
	m_bSaveMenuJPT = nID;
	// g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_ALL, FALSE, NULL, NULL);
	// g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_WITH_NAME, FALSE, NULL, NULL);
	switch (nID)
	{
		case enumMagic_ID1:
			break;
		case enumMagic_ID2:
			g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_ALL, TRUE, NULL, NULL);
			break;
		case enumMagic_ID3:
			g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_WITH_NAME, TRUE, NULL, NULL);
			break;
		default:
			return;
	}
}

void KUiOther::LoadOtherSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue;
	char szKeyName[64];
	if (pConfigFile)
	{			
		pConfigFile->GetInteger(OTitle, "F1", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			m_OtherPartyCK.CheckButton(TRUE);
			g_pCoreShell->PAIOperation(GPI_O_PARTYCK, TRUE, NULL, NULL);
		}
		
		pConfigFile->GetInteger(OTitle, "F2", 1, (int*)(&m_nValue));
		m_OtherPartyEdit.SetText(szArray_Team[m_nValue]);
		if (m_nValue == 1)
		{
			g_pCoreShell->PAIOperation(GPI_O_PARTYINVITE, TRUE, NULL, NULL);
		}
		else if (m_nValue == 2)
		{
			m_OtherPartyEdit.SetText(szArray_Team[m_nValue]);
			g_pCoreShell->PAIOperation(GPI_O_PARTYINVITE, TRUE, NULL, NULL);
			g_pCoreShell->PAIOperation(GPI_O_PARTYLIST, TRUE, NULL, NULL);
		}
		SetMenuParty(m_nValue);
		
		for (int i = 0; i < defMAX_LIST_PARTY; i++)
		{
			sprintf(szKeyName, "F2CN%d", i);
			pConfigFile->GetString(OTitle, szKeyName, "", m_ListName[i], sizeof(m_ListName[i]));
		}
		SetNameListPT();
		
		
		pConfigFile->GetInteger(OTitle, "F3", 1, (int*)(&m_nValue));
		m_OtherPartyJoinTxt.SetText(szArray_Team_Join_Group[m_nValue]);
		if(m_nValue == 1)
		{
			g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_ALL, TRUE, NULL, NULL);
		}
		else if(m_nValue == 2)
		{
			g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT_WITH_NAME, TRUE, NULL, NULL);
		}
		SetMenuJoinParty(m_nValue);
		
		pConfigFile->GetInteger(OTitle, "F4", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			m_OtherPartyJoinCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_O_PARTYACCEPT, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(OTitle, "F5", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			m_OtherOptimalNpcCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_NPC, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(OTitle, "F6", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			m_OtherOptimalPlayerCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_PLAYER, true, NULL, NULL);
		}
		
		pConfigFile->GetInteger(OTitle, "F7", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			m_OtherOptimalAniCK.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_O_OPTIMAL_ANI, true, NULL, NULL);
		}
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiOther::SaveOtherSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	char szKeyName[64];
	int nCount;
	if (pConfigFile)
	{
		if (m_OtherPartyCK.IsButtonChecked())
			pConfigFile->WriteInteger(OTitle, "F1",true);
		else
			pConfigFile->WriteInteger(OTitle, "F1",false);
		pConfigFile->WriteInteger(OTitle, "F2",m_bSaveMenuPT);
		pConfigFile->WriteInteger(OTitle, "F3",m_bSaveMenuJPT);
		
		nCount = 0;
		for (int i = 0; i < defMAX_LIST_PARTY; i++)
		{
			if (m_ListName[i][0])
			{
				sprintf(szKeyName, "F2CN%d", nCount);
				pConfigFile->WriteString(OTitle, szKeyName, m_ListName[i]);
				nCount++;
			}
		}
		
		if (m_OtherPartyJoinCK.IsButtonChecked())
			pConfigFile->WriteInteger(OTitle, "F4",true);
		else
			pConfigFile->WriteInteger(OTitle, "F4",false);
		
		if (m_OtherOptimalNpcCK.IsButtonChecked())
			pConfigFile->WriteInteger(OTitle, "F5",true);
		else
			pConfigFile->WriteInteger(OTitle, "F5",false);
		
		if (m_OtherOptimalPlayerCK.IsButtonChecked())
			pConfigFile->WriteInteger(OTitle, "F6",true);
		else
			pConfigFile->WriteInteger(OTitle, "F6",false);
		
		if (m_OtherOptimalAniCK.IsButtonChecked())
			pConfigFile->WriteInteger(OTitle, "F7",true);
		else
			pConfigFile->WriteInteger(OTitle, "F7",false);		
	}
	g_UiBase.CloseAutoSettingFile(true);
}

////////////////////////////////////////////
//////////////////////MOVEPLAYER
////////////////////////////////////////////
KUiMovePlayer* KUiMovePlayer::m_pSelf = NULL;
void KUiMovePlayer::Initialize()
{
	AddChild(&m_BgMove);
	
	AddChild(&c_MoveFollow);
	AddChild(&t_MoveFollowPeople);
	AddChild(&p_MoveFollowPopup);
	AddChild(&t_MoveFollowPeopleRadius);
	AddChild(&e_MoveFollowPeopleRadius);
	AddChild(&c_MoveAround);
	AddChild(&t_MoveAround);
	AddChild(&c_MoveAroundAdd);

	AddChild(&c_MoveCoordiNates);
	AddChild(&t_MoveCoordiNatesList);
	AddChild(&scb_MoveCoordiNatesListScb);
	t_MoveCoordiNatesList.SetScrollbar(&scb_MoveCoordiNatesListScb);
	AddChild(&c_MoveCoordiNatesAdd);
	AddChild(&c_MoveCoordiNatesUp);
	AddChild(&c_MoveCoordiNatesDown);
	AddChild(&c_MoveCoordiNatesDel);
	AddChild(&c_MoveCoordiNatesDelAll);
	
	memset(m_cFollowName, 0, sizeof(m_cFollowName));	
	memset(m_MoveMpsList, 0, sizeof(m_MoveMpsList));
	memset(m_MoveMpsListSave, 0, sizeof(m_MoveMpsListSave));

	m_MoveAroundBtn = FALSE;
}

void KUiMovePlayer::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	m_BgMove.Init(pIni,"BgMove");
	c_MoveFollow.Init(pIni,"MoveFollow");
	t_MoveFollowPeople.Init(pIni,"MoveFollowPeople");
	t_MoveFollowPeople.SetText(MSG_NON_SETTINGS);
	p_MoveFollowPopup.Init(pIni,"MoveFollowPopup");
	t_MoveFollowPeopleRadius.Init(pIni,"MoveFollowRadiusText");
	e_MoveFollowPeopleRadius.Init(pIni,"MoveFollowRadiusEdit");
	e_MoveFollowPeopleRadius.SetIntText(200);
	c_MoveAround.Init(pIni,"MoveAround");
	t_MoveAround.Init(pIni,"MoveAroundText");
	c_MoveAroundAdd.Init(pIni,"MoveAroundAdd");
	
	c_MoveCoordiNates.Init(pIni,"MoveCoordiNatesCK");
	t_MoveCoordiNatesList.Init(pIni,"MoveCoordiNatesListTxt");
	scb_MoveCoordiNatesListScb.Init(pIni,"MoveCoordiNatesListScb");
	c_MoveCoordiNatesAdd.Init(pIni,"MoveCoordiNatesAdd");
	c_MoveCoordiNatesUp.Init(pIni,"MoveCoordiNatesUp");
	c_MoveCoordiNatesDown.Init(pIni,"MoveCoordiNatesDown");
	c_MoveCoordiNatesDel.Init(pIni,"MoveCoordiNatesDel");
	c_MoveCoordiNatesDelAll.Init(pIni,"MoveCoordiNatesDelAll");

	LoadMoveSetting();
}

int KUiMovePlayer::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:	
		if (uParam == (unsigned int)(KWndWindow*)&c_MoveFollow)
		{
			if (c_MoveFollow.IsButtonChecked())
			{
				c_MoveFollow.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE,true,NULL,NULL);	
				OnCheckInput();
			}
			else
			{
				c_MoveFollow.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&p_MoveFollowPopup)
			PopUpFollowPeople();
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveAround)
		{
			if (c_MoveAround.IsButtonChecked())
			{	
				if (m_MoveAroundBtn)
				{
					if (c_MoveCoordiNates.IsButtonChecked() > 0)
					{
						c_MoveCoordiNates.CheckButton(false);	
						g_pCoreShell->PAIOperation(GPI_M_MOVEPOS,false,NULL,NULL);
					}
					else
					{
						c_MoveAround.CheckButton(true);
						g_pCoreShell->PAIOperation(GPI_M_AROUND, true, NULL, NULL);	
					}					
				}
				else
				{
					c_MoveAround.CheckButton(false);
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_NONE;
					Msg.byParamSize = 0;
					Msg.byPriority = 0;
					Msg.eType = SMT_NORMAL;
					Msg.uReservedForUi = 0;
					strcpy(Msg.szMessage, "Täa ®é chØ ®Þnh ch­a thiÕt lËp!");
					KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
				}
			}
			else
			{
				c_MoveAround.CheckButton(false);	
				m_MoveAroundBtn = FALSE;
				g_pCoreShell->PAIOperation(GPI_M_AROUND, false, NULL, NULL);
				g_pCoreShell->PAIOperation(GPI_M_AROUND_NUM, false, NULL, NULL);	
				t_MoveAround.SetText("");
			}			
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveAroundAdd)
		{
			m_MoveAroundBtn = TRUE;
			t_MoveAround.Set3IntText(m_CurrentMps.nSceneId, (m_CurrentMps.nScenePos0) / 8, (m_CurrentMps.nScenePos1) / 8, '-', '/');
			g_pCoreShell->PAIOperation(GPI_M_AROUND_NUM, true, NULL, NULL);	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNates)
		{
			if (c_MoveCoordiNates.IsButtonChecked())
			{	
				if (c_MoveAround.IsButtonChecked() > 0)
				{
					c_MoveAround.CheckButton(false);
					m_MoveAroundBtn = FALSE;
					g_pCoreShell->PAIOperation(GPI_M_AROUND, false, NULL, NULL);
					g_pCoreShell->PAIOperation(GPI_M_AROUND_NUM, false, NULL, NULL);	
					t_MoveAround.SetText("");
				}
				c_MoveCoordiNates.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_M_MOVEPOS,true,NULL,NULL);
			}
			else
			{
				c_MoveCoordiNates.CheckButton(false);	
				g_pCoreShell->PAIOperation(GPI_M_MOVEPOS,false,NULL,NULL);
			}			
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNatesAdd)
		{
			InsertMoveMpsList(m_CurrentMps.nSceneId,(m_CurrentMps.nScenePos0)*32,(m_CurrentMps.nScenePos1)*64);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNatesUp)
		{		
			int nSel = t_MoveCoordiNatesList.GetCurSel();
			memset(m_MoveMpsListSave[0], 0, sizeof(m_MoveMpsListSave[0]));	
			if (nSel <= 0)
				return FALSE;
			m_MoveMpsListSave[0][0] = m_MoveMpsList[nSel][0];
			m_MoveMpsListSave[0][1] = m_MoveMpsList[nSel][1];
			m_MoveMpsListSave[0][2] = m_MoveMpsList[nSel][2];
			
			m_MoveMpsList[nSel][0] = m_MoveMpsList[nSel-1][0];
			m_MoveMpsList[nSel][1] = m_MoveMpsList[nSel-1][1];
			m_MoveMpsList[nSel][2] = m_MoveMpsList[nSel-1][2];

			m_MoveMpsList[nSel-1][0] = m_MoveMpsListSave[0][0];
			m_MoveMpsList[nSel-1][1] = m_MoveMpsListSave[0][1];
			m_MoveMpsList[nSel-1][2] = m_MoveMpsListSave[0][2];
			SetMoveMpsList();	
			t_MoveCoordiNatesList.SetCurSel(nSel-1);
			memset(m_MoveMpsListSave, 0, sizeof(m_MoveMpsListSave));
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNatesDown)
		{		
			int nSel = t_MoveCoordiNatesList.GetCurSel();
			int nCount = 0;
			int i;
			memset(m_MoveMpsListSave[0], 0, sizeof(m_MoveMpsListSave[0]));	
			
			for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
			{
				if (m_MoveMpsList[i][0])
				{
					nCount++;
				}
			}
			if (nSel < 0 || nSel >= nCount-1)
				return FALSE;
			m_MoveMpsListSave[0][0] = m_MoveMpsList[nSel][0];
			m_MoveMpsListSave[0][1] = m_MoveMpsList[nSel][1];
			m_MoveMpsListSave[0][2] = m_MoveMpsList[nSel][2];
			
			m_MoveMpsList[nSel][0] = m_MoveMpsList[nSel+1][0];
			m_MoveMpsList[nSel][1] = m_MoveMpsList[nSel+1][1];
			m_MoveMpsList[nSel][2] = m_MoveMpsList[nSel+1][2];

			m_MoveMpsList[nSel+1][0] = m_MoveMpsListSave[0][0];
			m_MoveMpsList[nSel+1][1] = m_MoveMpsListSave[0][1];
			m_MoveMpsList[nSel+1][2] = m_MoveMpsListSave[0][2];
			SetMoveMpsList();	
			if (nSel == nCount-1)
				t_MoveCoordiNatesList.SetCurSel(nSel);
			else
				t_MoveCoordiNatesList.SetCurSel(nSel+1);
			memset(m_MoveMpsListSave, 0, sizeof(m_MoveMpsListSave));	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNatesDel)
		{
			int nSel = t_MoveCoordiNatesList.GetCurSel();
			int nCount = 0;
			int i;
			memset(m_MoveMpsList[nSel], 0, sizeof(m_MoveMpsList[nSel]));	

			for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
			{
				if (m_MoveMpsList[i][0])
				{
					nCount++;
				}
			}
			
			for (i = nSel; i < nCount; i++)			
			{
				for (int j = 0; j < 3; j++)
				{
					m_MoveMpsList[i][j] = m_MoveMpsList[i+1][j];
					m_MoveMpsList[i+1][j] = 0;				
				}		
			}			
			nCount--;			
							
			SetMoveMpsList();			
		}		
		else if (uParam == (unsigned int)(KWndWindow*)&c_MoveCoordiNatesDelAll)
		{
			memset(m_MoveMpsList, 0, sizeof(m_MoveMpsList));
			SetMoveMpsList();
		}
		SaveMoveSetting();
		break;
	case WND_N_SCORLLBAR_POS_CHANGED:
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == MENU_SELECT_FOLLOW_PEOPLE && (short)(LOWORD(nParam)) >= 0)
				SetFollowPeople(LOWORD(nParam));
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiMovePlayer::OnCheckInput()
{
	int nRadiusFollow = e_MoveFollowPeopleRadius.GetIntNumber();
	g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE_RADIUS,NULL,nRadiusFollow,NULL);
}

void KUiMovePlayer::PopUpFollowPeople()
{
	if (m_pNearbyPlayersList)
	{
		free (m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}
	int nActionDataCount = g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, 0, 0);
	if (nActionDataCount > 0) 
	{
		struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
		if (pSelUnitMenu == NULL)
			return;
		KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
		pSelUnitMenu->nNumItem = 0;
		pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;

		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nActionDataCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, (unsigned int)m_pNearbyPlayersList, nActionDataCount);
			for (int i = 0; i< nActionDataCount;i++)
			{
				strncpy(pSelUnitMenu->Items[i].szData, m_pNearbyPlayersList[i].Name, 63);
				pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
				pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
				pSelUnitMenu->nNumItem++;
			}
		}
		int nWidth;
		GetSize(&nWidth, NULL);
		int nX = 0, nY = 0;
		KIniFile	Ini;
		t_MoveFollowPeople.GetAbsolutePos(&nX, &nY);
		pSelUnitMenu->nX = nX;
		pSelUnitMenu->nY = nY;
		KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_FOLLOW_PEOPLE);
	} 
	else 
	{
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.byParamSize = 0;
		Msg.byPriority = 0;
		Msg.eType = SMT_NORMAL;
		Msg.uReservedForUi = 0;
		strcpy(Msg.szMessage, "N¬i nµy ®ång kh«ng m«ng qu¹nh, kh«ng mét bãng ng­êi!");
		KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
	}
}

void KUiMovePlayer::SetFollowPeople(int nAction)
{
	strcpy(m_cFollowName, m_pNearbyPlayersList[nAction].Name);
	t_MoveFollowPeople.SetText(m_cFollowName);
	g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE_NAME,(unsigned int)&m_cFollowName,NULL,NULL);
}

BOOL KUiMovePlayer::InsertMoveMpsList(int nSubWorldId, int nMpsX, int nMpsY)
{
	for (int i = 0; i < defMAX_AUTO_MOVEMPSL; i ++)
	{
		if (m_MoveMpsList[i][0] == nSubWorldId && 
			m_MoveMpsList[i][1] == nMpsX && 
			m_MoveMpsList[i][2] == nMpsY)
		{
			i = defMAX_AUTO_MOVEMPSL;
			break;
		}
		else if (m_MoveMpsList[i][0] == 0 && 
				m_MoveMpsList[i][1] == 0 && 
				m_MoveMpsList[i][2] == 0)
		{
			break;
		}
	}
	if (i < defMAX_AUTO_MOVEMPSL)
	{
		m_MoveMpsList[i][0] = nSubWorldId;
		m_MoveMpsList[i][1] = nMpsX;
		m_MoveMpsList[i][2] = nMpsY;
		SetMoveMpsList();
	}
	return TRUE;
}

void KUiMovePlayer::SetMoveMpsList()
{
	char szList[32];
	t_MoveCoordiNatesList.ResetContent();
	for (int i = 0; i < defMAX_AUTO_MOVEMPSL; i ++)
	{
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSID, i, m_MoveMpsList[i][0]);
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSX, i, m_MoveMpsList[i][1]);
		g_pCoreShell->SetMoveMap(GAUTO_AUTO_MOVEMPSY, i, m_MoveMpsList[i][2]);

		if(m_MoveMpsList[i][0])
		{
			sprintf(szList,MSG_FORMAT_POS, m_MoveMpsList[i][0], m_MoveMpsList[i][1], m_MoveMpsList[i][2]);
			t_MoveCoordiNatesList.AddString(i, szList,"","");
		}
	}
}

void KUiMovePlayer::LoadMoveSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue,i;
	char szKeyName[16];
	if (pConfigFile)
	{
		pConfigFile->GetInteger(MTitle, "F1", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_MoveFollow.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE,m_nValue,NULL,NULL);	

		pConfigFile->GetInteger(MTitle,"F2",200,(int*)(&m_nValue));
		if (m_nValue <= 0)
			m_nValue = 200;
		else
			m_nValue = m_nValue;
		e_MoveFollowPeopleRadius.SetIntText(m_nValue);
		g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE_RADIUS,NULL,m_nValue,NULL);

		pConfigFile->GetString(MTitle, "F1CN", "", m_cFollowName, sizeof(m_cFollowName));
		if (m_cFollowName[0])
		{
			t_MoveFollowPeople.SetText(m_cFollowName);
			g_pCoreShell->PAIOperation(GPI_M_FOLLOW_PEOPLE_NAME,(unsigned int)&m_cFollowName,NULL,NULL);
		}

		pConfigFile->GetInteger(MTitle, "F3", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
		{	
			c_MoveCoordiNates.CheckButton(true);
			g_pCoreShell->PAIOperation(GPI_M_MOVEPOS,true,NULL,NULL);
		}
		
		for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
		{
			sprintf(szKeyName, "F3D%d", i);
			pConfigFile->GetInteger(MTitle, szKeyName, 0, (int*)(&m_MoveMpsList[i][0]));
			sprintf(szKeyName, "F3X%d", i);
			pConfigFile->GetInteger(MTitle, szKeyName, 0, (int*)(&m_MoveMpsList[i][1]));
			sprintf(szKeyName, "F3Y%d", i);
			pConfigFile->GetInteger(MTitle, szKeyName, 0, (int*)(&m_MoveMpsList[i][2]));			
		}
		SetMoveMpsList();	
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiMovePlayer::SaveMoveSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		int i;
		char szKeyName[16];
		pConfigFile->WriteInteger(MTitle, "F1",c_MoveFollow.IsButtonChecked());
		pConfigFile->WriteInteger(MTitle, "F2",e_MoveFollowPeopleRadius.GetIntNumber());
		pConfigFile->WriteString(MTitle, "F1CN", m_cFollowName);	
		pConfigFile->WriteInteger(MTitle, "F3",c_MoveCoordiNates.IsButtonChecked());
		for (i = 0; i < defMAX_AUTO_MOVEMPSL; i++)
		{ 
			sprintf(szKeyName, "F3D%d", i);
			pConfigFile->WriteInteger(MTitle, szKeyName, m_MoveMpsList[i][0]);
			sprintf(szKeyName, "F3X%d", i);
			pConfigFile->WriteInteger(MTitle, szKeyName, m_MoveMpsList[i][1]);
			sprintf(szKeyName, "F3Y%d", i);
			pConfigFile->WriteInteger(MTitle, szKeyName, m_MoveMpsList[i][2]);				
		}	
	}
	g_UiBase.CloseAutoSettingFile(true);
}

////////////////////////////////////////////
//////////////////////SUPPORTPLAYER
////////////////////////////////////////////
char szArray_Map[][64] =
{
	"Trë l¹i khi chÕt",
	"Kháa lang ®éng",
	"Sa m¹c mª cung",
	"Tr­êng b¹ch s¬n",
	"Sa m¹c 1",
	"Sa m¹c 2",
	"Sa m¹c 3",
	"Tr­êng b¹ch B¾c",
	"Tr­êng b¹ch Nam",
	"Phong l¨ng ®é",
	"M¹c cao quËt",
};

void KUiSupportPlayer::Initialize()
{
	AddChild(&m_BgSupport);
	AddChild(&c_BackToTown);
	AddChild(&e_BackToTown);
	AddChild(&c_PlayerSellItem);
	AddChild(&t_PlayerSellItem);
	AddChild(&c_PlayerMoveItem);
	AddChild(&t_PlayerMoveItem);
	AddChild(&t_CombackMap);
	AddChild(&p_CombackMap); //dang lam
	AddChild(&t_NameComBackMap);
	m_nSelMapTrain = 0;
}

void KUiSupportPlayer::LoadScheme(KIniFile* pIni)
{
	KWndPage::Init(pIni, "Page");
	m_BgSupport.Init(pIni,"BgSupport");
	c_BackToTown.Init(pIni,"SupportBackToTown");
	e_BackToTown.Init(pIni,"SupportBackToTownEdit");
	e_BackToTown.SetIntText(10);
	c_PlayerSellItem.Init(pIni,"SupportSellItem");
	t_PlayerSellItem.Init(pIni,"SupportSellItemText");
	c_PlayerMoveItem.Init(pIni,"SupportMoveItem");
	t_PlayerMoveItem.Init(pIni,"SupportMoveItemText");
	t_CombackMap.Init(pIni,"CombackMap");
	p_CombackMap.Init(pIni,"PopupCombackMap");
	t_NameComBackMap.Init(pIni,"NameCombackMap");
	LoadSupportSetting();
}

int KUiSupportPlayer::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&c_BackToTown)
		{
			if (c_BackToTown.IsButtonChecked())
			{
				c_BackToTown.CheckButton(true);
				g_pCoreShell->PAIOperation(GPI_S_BACKTOMAPCK, true, NULL, NULL);
			}
			else
			{
				c_BackToTown.CheckButton(false);
				g_pCoreShell->PAIOperation(GPI_S_BACKTOMAPCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_PlayerSellItem)
		{
			if (c_PlayerSellItem.IsButtonChecked())
			{
				c_PlayerSellItem.CheckButton(true);
				g_pCoreShell->PAIOperation(GDI_S_SELL_ITEMCK, true, NULL, NULL);
			}
			else
			{
				c_PlayerSellItem.CheckButton(false);
				g_pCoreShell->PAIOperation(GDI_S_SELL_ITEMCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&c_PlayerMoveItem)
		{
			if (c_PlayerMoveItem.IsButtonChecked())
			{
				c_PlayerMoveItem.CheckButton(true);
				g_pCoreShell->PAIOperation(GDI_S_MOVE_ITEMCK, true, NULL, NULL);
			}
			else
			{
				c_PlayerMoveItem.CheckButton(false);
				g_pCoreShell->PAIOperation(GDI_S_MOVE_ITEMCK, false, NULL, NULL);
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&p_CombackMap)
		{
			PopupSelMapTrain();
		}
		SaveSupportSetting();
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == MENU_SELECT_MAPTRAIN && (short)(LOWORD(nParam)) >= 0)
			{
				SetPortalMapTrain(LOWORD(nParam));
			}
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiSupportPlayer::UpdateClientDaTa()
{
	if (g_pCoreShell->GetAutoFlag() == 1)
	{
		KUiObjAtRegion* pObjs = NULL;
		int nCount = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0);
		if (nCount == 0)
			return;

		if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
		{
			g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (unsigned int)pObjs, nCount);
			for (int i = 0; i < nCount; i++)
			{
				g_pCoreShell->PAIOperation(GDI_S_SELL_ITEM, NULL, (int)pObjs[i].Obj.uId, NULL);
			}
			free(pObjs);
			pObjs = NULL;
		}
	}

}

void KUiSupportPlayer::OnCheckInput()
{
	int ValueTime = e_BackToTown.GetIntNumber();
	if (ValueTime < 10)
	{	
		ValueTime = 10;
	}
	g_pCoreShell->PAIOperation(GPI_S_BACKTOMAPEDIT, NULL,(int)ValueTime, NULL);
}

void KUiSupportPlayer::PopupSelMapTrain()
{
	int nActionDataCount = sizeof(szArray_Map) / sizeof(szArray_Map[0]);
	struct KPopupMenuData* pSelUnitMenu = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nActionDataCount));
	if (pSelUnitMenu == NULL)
		return;
	KPopupMenu::InitMenuData(pSelUnitMenu, nActionDataCount);
	pSelUnitMenu->nNumItem = 0;
	pSelUnitMenu->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	for (int i = 0; i < nActionDataCount; i++)
	{
		strncpy(pSelUnitMenu->Items[i].szData, szArray_Map[i], sizeof(szArray_Map[i]));
		pSelUnitMenu->Items[i].szData[sizeof(pSelUnitMenu->Items[i].szData) - 1] = 0;
		pSelUnitMenu->Items[i].uDataLen = strlen(pSelUnitMenu->Items[i].szData);
		pSelUnitMenu->nNumItem++;
	}
	int x, y;
	t_NameComBackMap.GetAbsolutePos(&x, &y);
	pSelUnitMenu->nX = x;
	pSelUnitMenu->nY = y;
	KPopupMenu::Popup(pSelUnitMenu, this, MENU_SELECT_MAPTRAIN);
}

void KUiSupportPlayer::SetPortalMapTrain(int nSel)
{
	m_nSelMapTrain = nSel;
	t_NameComBackMap.SetText(szArray_Map[m_nSelMapTrain]);
	g_pCoreShell->PAIOperation(GPI_S_MAP_TRAIN, m_nSelMapTrain, NULL, NULL);
}


void KUiSupportPlayer::LoadSupportSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	int m_nValue;
	if (pConfigFile)
	{
		pConfigFile->GetInteger(STitle, "F1", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_BackToTown.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GPI_S_BACKTOMAPCK, m_nValue, NULL, NULL);
		
		pConfigFile->GetInteger(STitle, "F2", 10, (int*)(&m_nValue));
		if (m_nValue > 0)
		e_BackToTown.SetIntText(m_nValue);
		g_pCoreShell->PAIOperation(GPI_S_BACKTOMAPEDIT, NULL, m_nValue, NULL);

		pConfigFile->GetInteger(STitle, "F3", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_PlayerSellItem.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GDI_S_SELL_ITEMCK, m_nValue, NULL, NULL);
		
		pConfigFile->GetInteger(STitle, "F4", 0, (int*)(&m_nValue));
		if (m_nValue > 0)
			m_nValue = true;
		c_PlayerMoveItem.CheckButton(m_nValue);
		g_pCoreShell->PAIOperation(GDI_S_MOVE_ITEMCK, m_nValue, NULL, NULL);
		
		pConfigFile->GetInteger(STitle, "F5", 0, &m_nSelMapTrain);
		t_NameComBackMap.SetText(szArray_Map[m_nSelMapTrain]);
		g_pCoreShell->PAIOperation(GPI_S_MAP_TRAIN,m_nSelMapTrain,NULL,NULL);
		
	}
	g_UiBase.CloseAutoSettingFile(true);
}

void KUiSupportPlayer::SaveSupportSetting()
{
	KIniFile* pConfigFile = NULL;
	pConfigFile = g_UiBase.GetAutoSettingFile();
	if (pConfigFile)
	{
		pConfigFile->WriteInteger(STitle, "F1",c_BackToTown.IsButtonChecked());
		pConfigFile->WriteInteger(STitle, "F2",e_BackToTown.GetIntNumber());
		pConfigFile->WriteInteger(STitle, "F3",c_PlayerSellItem.IsButtonChecked());
		pConfigFile->WriteInteger(STitle, "F4",c_PlayerMoveItem.IsButtonChecked());
		pConfigFile->WriteInteger(STitle, "F5", m_nSelMapTrain);
	}
	g_UiBase.CloseAutoSettingFile(true);
}

////////////////////////////////////////////
//////////////////////AUTOPLAY
////////////////////////////////////////////

KUiAutoPlay* KUiAutoPlay::m_pSelf = NULL;

KUiAutoPlay* KUiAutoPlay::OpenWindow(bool bShow)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoPlay;
		if (m_pSelf)
		{
			m_pSelf->Initialize();
		}
		if (bShow)m_pSelf->m_RecoveryPad.Show();

	}
	if (m_pSelf && bShow)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


KUiAutoPlay* KUiAutoPlay::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiAutoPlay::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf->m_RecoveryPad.SaveRecoverySetting();
			m_pSelf->m_FightPad.SaveFightSetting();
			m_pSelf->m_PickPad.SavePickSetting();
			m_pSelf->m_OtherPad.SaveOtherSetting();			
			m_pSelf->m_MovePlayerPad.SaveMoveSetting();			
			m_pSelf->m_SupportPlayerPad.SaveSupportSetting();			
			m_pSelf = NULL;
		}
	}
}

void KUiAutoPlay::Initialize()
{
	m_RecoveryPad.Initialize();
	AddPage(&m_RecoveryPad, &m_RecoveryBtn);
	m_PickPad.Initialize();
	AddPage(&m_PickPad, &m_PickBtn);
	m_FightPad.Initialize();
	AddPage(&m_FightPad, &m_FightBtn);
	m_OtherPad.Initialize();
	AddPage(&m_OtherPad, &m_OtherBtn);
	m_MovePlayerPad.Initialize();
	AddPage(&m_MovePlayerPad, &m_MovePlayerBtn);
	m_SupportPlayerPad.Initialize();
	AddPage(&m_SupportPlayerPad, &m_SupporPlayerBtn);
	AddChild(&m_ActiveBtn);
	AddChild(&m_PauseBtn);
	AddChild(&m_CloseBtn);
	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	IsAuto = FALSE;
	Wnd_AddWindow(this);
}

void KUiAutoPlay::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		
		m_pSelf->m_ActiveBtn.Init(&Ini, "ActiveText");
		m_pSelf->m_PauseBtn.Init(&Ini, "PauseText");
		m_pSelf->m_PauseBtn.Hide();
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseText");

		m_pSelf->m_RecoveryBtn.Init(&Ini, "RecoveryBtn");
		m_pSelf->m_PickBtn.Init(&Ini, "PickBtn");
		m_pSelf->m_FightBtn.Init(&Ini, "FightBtn");
		m_pSelf->m_MovePlayerBtn.Init(&Ini, "MovePlayerBtn");
		m_pSelf->m_SupporPlayerBtn.Init(&Ini, "SupportBtn");
		m_pSelf->m_OtherBtn.Init(&Ini, "OtherBtn");

		m_pSelf->m_RecoveryPad.LoadScheme(&Ini);
		m_pSelf->m_PickPad.LoadScheme(&Ini);
		m_pSelf->m_FightPad.LoadScheme(&Ini);
		m_pSelf->m_MovePlayerPad.LoadScheme(&Ini);
		m_pSelf->m_SupportPlayerPad.LoadScheme(&Ini);
		m_pSelf->m_OtherPad.LoadScheme(&Ini);
		
		if (g_pCoreShell->GetAutoFlag()) {
			m_pSelf->IsAuto = TRUE;
			m_pSelf->UpdateButton(m_pSelf->IsAuto);
		}
		else
		{
			m_pSelf->IsAuto = FALSE;
			m_pSelf->UpdateButton(m_pSelf->IsAuto);
		}
	}
}

int KUiAutoPlay::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_ActiveBtn)
		{
			if (g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0))
				return FALSE;
			m_ActiveBtn.Hide();
			m_PauseBtn.Show();
			IsAuto = TRUE;
			OnAuto(IsAuto);
		}
		else if(uParam == (unsigned int)(KWndWindow*)&m_PauseBtn)
		{
			m_PauseBtn.Hide();	
			m_ActiveBtn.Show();
			IsAuto = FALSE;
			OnAuto(IsAuto);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow(false);
		break;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

void KUiAutoPlay::UpdateButton(BOOL nIsAuto)
{
	if (nIsAuto)
	{
			m_ActiveBtn.Hide();
			m_PauseBtn.Show();
			//g_pCoreShell->PAIOperation(GPI_SWITCH_ACTIVE, nIsAuto, NULL, NULL);
	}
	else
	{
			m_PauseBtn.Hide();	
			m_ActiveBtn.Show();
			//g_pCoreShell->PAIOperation(GPI_SWITCH_ACTIVE, nIsAuto, NULL, NULL);
	}
}

void KUiAutoPlay::UpdateData()
{
	m_pSelf->OpenWindow(false);
	if (g_pCoreShell->GetAutoFlag()) {
			m_pSelf->IsAuto = TRUE;
			m_pSelf->UpdateButton(m_pSelf->IsAuto);
	}
	else
	{
		m_pSelf->IsAuto = FALSE;
		m_pSelf->UpdateButton(m_pSelf->IsAuto);
	}
}

void KUiAutoPlay::OnAuto(BOOL nIsAuto)
{	
		g_pCoreShell->PAIOperation(GPI_SWITCH_ACTIVE, nIsAuto, NULL, NULL);
}

void KUiAutoPlay::UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo)
{
	if (m_pSelf && pInfo)
		m_pSelf->m_MovePlayerPad.m_CurrentMps = *pInfo;
}

void KUiAutoPlay::Breathe()
{
	m_pSelf->m_RecoveryPad.Breathe();
}

BOOL KUiAutoPlay::SavePrivateSetting()
{
	KIniFile* pFile = g_UiBase.GetAutoSettingFile();// call bien file
	if (m_pSelf)
	{
		pFile->Clear();
		m_pSelf->m_RecoveryPad.SaveRecoverySetting();
		m_pSelf->m_FightPad.SaveFightSetting();
		m_pSelf->m_PickPad.SavePickSetting();
		m_pSelf->m_OtherPad.SaveOtherSetting();			
		m_pSelf->m_MovePlayerPad.SaveMoveSetting();			
		m_pSelf->m_SupportPlayerPad.SaveSupportSetting();				
		g_UiBase.CloseAutoSettingFile(true);
	}
	return TRUE;
}

BOOL KUiAutoPlay::LoadPrivateSetting()
{
	KIniFile* pFile = g_UiBase.GetAutoSettingFile();
	if (m_pSelf)
	{
		m_pSelf->m_RecoveryPad.LoadRecoverySetting();//1 Phuc hoi
		m_pSelf->m_PickPad.LoadPickSetting();//2 Nhat do
		m_pSelf->m_FightPad.LoadFightSetting();//3 chien dau 
		m_pSelf->m_OtherPad.LoadOtherSetting(); //4 Muc khac
		m_pSelf->m_MovePlayerPad.LoadMoveSetting(); //5 Di chuyen
		m_pSelf->m_SupportPlayerPad.LoadSupportSetting(); //6 Ho tro
		g_UiBase.CloseAutoSettingFile(true);
	}
	return TRUE;
}