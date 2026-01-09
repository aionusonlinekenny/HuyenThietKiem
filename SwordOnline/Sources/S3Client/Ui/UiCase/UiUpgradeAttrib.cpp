/*******************************************************************************
File        : UiUpgradeAttrib.cpp
Creator     : Based on UiTrembleItem by AlexKing
Create Date : 2025-12-21
Function    : Upgrade equipment magic attributes
*******************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiUpgradeAttrib.h"
#include "UiItem.h"
#include "UiMsgCentrePad.h"
#include "UiInformation.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../../../Engine/src/Text.h"
#include "../../../Engine/src/KDebug.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>

extern iCoreShell* g_pCoreShell;

#define LOOP 2
#define UPGRADE_ATTRIB_INI "UiUpgradeAttrib.ini"

KUiUpgradeAttrib* KUiUpgradeAttrib::m_pSelf = NULL;

static struct UA_CTRL_MAP
{
	int nPosition;
	const char* pIniSection;
}

CtrlItemMap[_UPGRADE_ATTRIB_SLOT_COUNT] =
{
	{ UIEP_BUILDITEM1, "Equipment" },   // Slot 0: Blue equipment to upgrade
	{ UIEP_BUILDITEM2, "Mineral1" },    // Slot 1: Special mineral type 1 (level 1-3)
	{ UIEP_BUILDITEM3, "Mineral2" },    // Slot 2: Special mineral type 2 (level 1-3)
	{ UIEP_BUILDITEM4, "Mineral3" },    // Slot 3: Special mineral type 3 (level 1-3)
	{ UIEP_BUILDITEM5, "Mineral4" },    // Slot 4: Special mineral type 4 (level 1-3)
	{ UIEP_BUILDITEM6, "LuckyStone" },  // Slot 5: Lucky stone for success boost
};

/*********************************************************************
 * Open Window
 *********************************************************************/
KUiUpgradeAttrib* KUiUpgradeAttrib::OpenWindow()
{
	g_DebugLog("[CLIENT] KUiUpgradeAttrib::OpenWindow() called");

	if (m_pSelf == NULL)
	{
		g_DebugLog("[CLIENT] Creating new KUiUpgradeAttrib instance");
		m_pSelf = new KUiUpgradeAttrib;
		if (m_pSelf)
		{
			g_DebugLog("[CLIENT] Calling Initialize()");
			m_pSelf->Initialize();
			g_DebugLog("[CLIENT] Initialize() completed");
		}
		else
		{
			g_DebugLog("[CLIENT] ERROR: Failed to allocate KUiUpgradeAttrib!");
			return NULL;
		}
	}

	if (m_pSelf)
	{
		g_DebugLog("[CLIENT] Playing open sound");
		UiSoundPlay(UI_SI_WND_OPENCLOSE);

		g_DebugLog("[CLIENT] Opening KUiItem if needed");
		if (!KUiItem::GetIfVisible())
			KUiItem::OpenWindow();

		g_DebugLog("[CLIENT] Calling UpdateData()");
		m_pSelf->UpdateData();
		g_DebugLog("[CLIENT] Calling BringToTop()");
		m_pSelf->BringToTop();
		g_DebugLog("[CLIENT] Calling Show()");
		m_pSelf->Show();
		g_DebugLog("[CLIENT] Setting input handling");
		Wnd_GameSpaceHandleInput(false);
		g_DebugLog("[CLIENT] OpenWindow() completed successfully");
	}
	return m_pSelf;
}

/*********************************************************************
 * Get If Visible
 *********************************************************************/
KUiUpgradeAttrib* KUiUpgradeAttrib::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

/*********************************************************************
 * Close Window
 *********************************************************************/
void KUiUpgradeAttrib::CloseWindow(bool bDestory)
{
	if (m_pSelf)
	{
		m_pSelf->OnCancel();  // Call BEFORE destroying!
		Wnd_GameSpaceHandleInput(true);
		if (bDestory)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

/*********************************************************************
 * Initialize
 *********************************************************************/
void KUiUpgradeAttrib::Initialize()
{
	int i = 0;
	AddChild(&m_BtnUpgrade);
	AddChild(&m_BtnClose);
	AddChild(&m_UpgradeEffect);
	AddChild(&m_TextPercent);
	AddChild(&m_EquipmentLabel);
	AddChild(&m_MaterialLabel);
	AddChild(&m_TextGuide);

	// NEW: Initialize attribute selection buttons
	for (i = 0; i < 6; i++)
	{
		AddChild(&m_BtnAttrib[i]);
		m_BtnAttrib[i].Hide();  // Hidden until equipment is placed
	}

	for (i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		m_UpgradeSlot[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_UpgradeSlot[i]);
		m_UpgradeSlot[i].SetContainerId((int)UOC_BUILD_ITEM);
	}

	m_nSelectedAttrib = -1;  // No attribute selected initially
	m_nAttributeCount = 0;   // No attributes loaded
	m_bUpgradeInProgress = FALSE;  // Not upgrading

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

/*********************************************************************
 * Load Scheme
 *********************************************************************/
void KUiUpgradeAttrib::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char Buff[128];
		KIniFile Ini;
		int i = 0;

		sprintf(Buff, "%s\\%s", pScheme, UPGRADE_ATTRIB_INI);
		if (!Ini.Load(Buff))
		{
			// CRITICAL: INI file failed to load!
			char szError[256];
			sprintf(szError, "[ERROR] Failed to load UI config file: %s", Buff);
			g_DebugLog(szError);
			_ASSERT(FALSE && "UiUpgradeAttrib.ini not found!");
			return;  // Don't initialize if INI failed to load
		}

		m_pSelf->Init(&Ini, "Main");

		m_pSelf->m_UpgradeEffect.Init(&Ini, "Effect");
		m_pSelf->m_UpgradeEffect.Hide();
		m_pSelf->m_EffectTime = 0;

		m_pSelf->m_BtnUpgrade.Init(&Ini, "UpgradeBtn");
		m_pSelf->m_BtnUpgrade.SetLabel("");  // Set button label
		m_pSelf->m_BtnClose.Init(&Ini, "CloseBtn");
		m_pSelf->m_BtnClose.SetLabel("");  // Set button label
		m_pSelf->m_TextPercent.Init(&Ini, "TextPercent");
		m_pSelf->m_TextPercent.SetText("T˚ l÷: ");  // Set initial text

		// Initialize slot labels
		m_pSelf->m_EquipmentLabel.Init(&Ini, "EquipmentLabel");
		m_pSelf->m_MaterialLabel.Init(&Ini, "MaterialLabel");
		m_pSelf->m_TextGuide.Init(&Ini, "TextGuide");
		m_pSelf->m_TextGuide.SetText("ß∆t trang bﬁ xanh vµo sœ xem thuÈc t›nh");

		// NEW: Initialize attribute selection buttons
		char szBtnName[32];
		for (i = 0; i < 6; i++)
		{
			sprintf(szBtnName, "BtnAttrib%d", i);
			m_pSelf->m_BtnAttrib[i].Init(&Ini, szBtnName);
			m_pSelf->m_BtnAttrib[i].SetLabel("");
			m_pSelf->m_BtnAttrib[i].Hide();  // Start hidden
		}

		for (i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
		{
			m_pSelf->m_UpgradeSlot[i].Init(&Ini, CtrlItemMap[i].pIniSection);
		}

		// Load error messages
		char szTemp[2];
		for (i = 0; i < 8; i++)
		{
			sprintf(szTemp, "%d", i);
			Ini.GetString("ReturnInfo", szTemp, "", m_pSelf->m_szReturnInfo[i], sizeof(m_pSelf->m_szReturnInfo[i]));
		}
	}
}

/*********************************************************************
 * Window Procedure
 *********************************************************************/
int KUiUpgradeAttrib::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		{
			// Check if it's an attribute selection button
			for (int i = 0; i < 6; i++)
			{
				if (uParam == (unsigned int)&m_BtnAttrib[i])
				{
					g_DebugLog("[CLIENT] Attribute button %d clicked", i);
					OnSelectAttribute(i);
					return 1;
				}
			}

			if (uParam == (unsigned int)&m_BtnUpgrade)
			{
				g_DebugLog("[CLIENT] Upgrade button clicked");
				if (ValidateUpgradeReady())
				{
					g_DebugLog("[CLIENT] Validation passed, showing confirmation dialog");
					if (m_EffectTime) break;
					UIMessageBox("Bπn c„ chæc muËn n©ng c p trang bﬁ nµy?", this, "X∏c nhÀn", "HÒy b·", ISP_DO_EVENT);
				}
				else
				{
					g_DebugLog("[CLIENT] Validation failed");
				}
			}
			else if (uParam == (unsigned int)&m_BtnClose)
			{
				if (m_EffectTime) break;
				CloseWindow(true);
			}
		}
		break;

	case WM_KEYDOWN:
		g_DebugLog("[UPGRADE-ATTRIB] WM_KEYDOWN received: uParam=%d (VK_ESCAPE=%d)", uParam, VK_ESCAPE);
		if (uParam == VK_RETURN)  // Enter
		{
			g_DebugLog("[UPGRADE-ATTRIB] Enter pressed, attempting upgrade");
			if (ValidateUpgradeReady())
			{
				UIMessageBox("Bπn c„ chæc muËn n©ng c p trang bﬁ nµy?", this, "X∏c nhÀn", "HÒy b·", ISP_DO_EVENT);
			}
			return 1;
		}
		else if (uParam == VK_ESCAPE)  // ESC
		{
			g_DebugLog("[UPGRADE-ATTRIB] ESC pressed, calling CloseWindow()");
			if (m_EffectTime) {
				g_DebugLog("[UPGRADE-ATTRIB] ESC blocked - effect in progress");
				return 1;
			}
			CloseWindow(true);
			return 1;
		}
		break;

	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;

	case WND_M_OTHER_WORK_RESULT:
		if (uParam == ISP_DO_EVENT)
		{
			if (!nParam)
			{
				m_UpgradeEffect.SetFrame(-1);
				m_EffectTime = 1;
				StartEffect();
			}
		}
		break;

	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

/*********************************************************************
 * Validate Upgrade Ready
 *********************************************************************/
BOOL KUiUpgradeAttrib::ValidateUpgradeReady()
{
	int nLen;
	char szWarning[128];
	KUiDraggedObject pObj;

	// Check equipment slot (mandatory)
	pObj.uId = 0;
	m_UpgradeSlot[SLOT_EQUIPMENT].GetObject(pObj);
	if (pObj.uId == 0)
	{
		strcpy(szWarning, "Ch≠a Æ∆t trang bﬁ vµo!");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// Check if at least one mineral is placed (not mandatory but recommended)
	// User can proceed without minerals but success rate will be low
	int nMineralCount = 0;
	for (int i = SLOT_MINERAL1; i <= SLOT_MINERAL4; i++)
	{
		pObj.uId = 0;
		m_UpgradeSlot[i].GetObject(pObj);
		if (pObj.uId > 0)
			nMineralCount++;
	}

	// Check attribute selection
	if (m_nSelectedAttrib < 0)
	{
		strcpy(szWarning, m_szReturnInfo[5]);  // "Chua chon thuoc tinh can nang cap"
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// NOTE: Money/xu validation is done in Lua script (ExeUpgradeAttrib)
	// Following game architecture: Lua = Game Logic, C++ = UI Presentation
	// C++ UI only validates basic requirements (items present), not game rules

	return TRUE;
}

/*********************************************************************
 * Validate Item Pick Drop
 *********************************************************************/
BOOL KUiUpgradeAttrib::ValidateItemPickDrop(KWndWindow* pWnd, int nIndex)
{
	if (!g_pCoreShell)
		return FALSE;

	int nGenre = g_pCoreShell->GetGenreItem(nIndex);
	int nDetail = g_pCoreShell->GetDetailItem(nIndex);
	int nParticular = g_pCoreShell->GetParticularItem(nIndex);
	char szWarning[128];
	int nLen;

	// Slot 0: Only blue equipment (genre = item_equip, quality = blue)
	if (pWnd == &m_UpgradeSlot[SLOT_EQUIPMENT])
	{
		if (nGenre != item_equip)
		{
			strcpy(szWarning, m_szReturnInfo[3]);  // "O nay chi dat trang bi xanh vao"
			nLen = strlen(szWarning);
			KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
			return FALSE;
		}
		// TODO: Add blue quality check when item system is implemented
		return TRUE;
	}
	// Slots 1-4: Mineral slots
	// NOTE: Only validate genre here (generic UI rule)
	// Lua script will validate specific mineral types via IsMineralItem()
	else if (pWnd == &m_UpgradeSlot[SLOT_MINERAL1] ||
		     pWnd == &m_UpgradeSlot[SLOT_MINERAL2] ||
		     pWnd == &m_UpgradeSlot[SLOT_MINERAL3] ||
		     pWnd == &m_UpgradeSlot[SLOT_MINERAL4])
	{
		// UI Layer: Only check genre (must be task item)
		// Game Logic Layer (Lua): Will validate specific details in ExeUpgradeAttrib()
		if (nGenre != item_task)
		{
			strcpy(szWarning, m_szReturnInfo[4]);  // "O nay chi dat Khoang thach dac biet"
			nLen = strlen(szWarning);
			KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
			return FALSE;
		}
		return TRUE;
	}
	// Slot 5: Lucky stone slot
	// NOTE: Only validate genre here (generic UI rule)
	// Lua script will validate specific lucky stone via IsLuckyStone()
	else if (pWnd == &m_UpgradeSlot[SLOT_LUCKY_STONE])
	{
		// UI Layer: Only check genre (must be task item)
		// Game Logic Layer (Lua): Will validate detail = 86 in ExeUpgradeAttrib()
		if (nGenre != item_task)
		{
			strcpy(szWarning, m_szReturnInfo[7]);  // "O nay chi dat Da May Man"
			nLen = strlen(szWarning);
			KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

/*********************************************************************
 * On Item Pick Drop
 *********************************************************************/
void KUiUpgradeAttrib::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion Pick, Drop;
	KUiDraggedObject Obj;
	KWndWindow* pWnd = NULL;

	// CRITICAL FIX: Handle PICKING item OUT of slot (removing item)
	if (pPickPos)
	{
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_BUILD_ITEM;
		pWnd = pPickPos->pWnd;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	// Handle DROPPING item INTO slot (adding item)
	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		if (ValidateItemPickDrop(pWnd, Obj.uId))
		{
			Drop.Obj.uGenre = Obj.uGenre;
			Drop.Obj.uId = Obj.uId;
			Drop.Region.Width = Obj.DataW;
			Drop.Region.Height = Obj.DataH;
			Drop.Region.h = 0;
			Drop.eContainer = UOC_BUILD_ITEM;
		}
	}

	for (int i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_UpgradeSlot[i])
		{
			Drop.Region.v = Pick.Region.v = CtrlItemMap[i].nPosition;
			break;
		}
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}


/*********************************************************************
 * Breathe (Animation Update)
 *********************************************************************/
void KUiUpgradeAttrib::Breathe()
{
	// Advance animation frame
	if (m_UpgradeEffect.IsVisible())
	{
		m_UpgradeEffect.NextFrame();
	}

	// Count effect time
	if (m_EffectTime)
	{
		m_EffectTime++;
	}

	// When effect animation completes, stop effect and execute upgrade
	if (m_EffectTime == (m_UpgradeEffect.GetMaxFrame()) * (LOOP * 2) / 2 + 1)
	{
		StopEffect();  // This calls OnUpgrade() which sends Lua script
		m_EffectTime = 0;
	}
}

/*********************************************************************
 * Start Effect
 *********************************************************************/
void KUiUpgradeAttrib::StartEffect()
{
	g_DebugLog("[CLIENT-EFFECT] StartEffect() called");
	m_UpgradeEffect.Show();
	UpdatePickPut(false);
	g_DebugLog("[CLIENT-EFFECT] StartEffect() finished");
}

/*********************************************************************
 * Stop Effect
 *********************************************************************/
void KUiUpgradeAttrib::StopEffect()
{
	g_DebugLog("[CLIENT-EFFECT] StopEffect() called");
	g_DebugLog("[CLIENT-EFFECT] Calling m_UpgradeEffect.Hide()");
	m_UpgradeEffect.Hide();
	g_DebugLog("[CLIENT-EFFECT] m_UpgradeEffect.Hide() completed");
	g_DebugLog("[CLIENT-EFFECT] Calling UpdatePickPut(true)");
	UpdatePickPut(true);
	g_DebugLog("[CLIENT-EFFECT] UpdatePickPut(true) completed");
	g_DebugLog("[CLIENT-EFFECT] Calling OnUpgrade()");
	OnUpgrade();
	g_DebugLog("[CLIENT-EFFECT] OnUpgrade() returned");
	g_DebugLog("[CLIENT-EFFECT] StopEffect() about to return");
}

/*********************************************************************
 * Is Effect Running
 *********************************************************************/
BOOL KUiUpgradeAttrib::IsEffect()
{
	return (m_EffectTime > 0);
}

/*********************************************************************
 * Update Data
 *********************************************************************/
void KUiUpgradeAttrib::UpdateData()
{
	g_DebugLog("[CLIENT] UpdateData() started");

	if (!g_pCoreShell)
	{
		g_DebugLog("[CLIENT] ERROR: g_pCoreShell is NULL!");
		return;
	}

	// CRITICAL FIX: GetGameData returns ALL 9 build container slots, not just our 3!
	// Must allocate array for 9 items to prevent buffer overflow and stack corruption
	const int BUILD_CONTAINER_SIZE = 20;  // Same as UiTrembleItem
	g_DebugLog("[CLIENT] Creating Item array, size = %d (was %d)", BUILD_CONTAINER_SIZE, _UPGRADE_ATTRIB_SLOT_COUNT);
	KUiObjAtRegion Item[BUILD_CONTAINER_SIZE];

	g_DebugLog("[CLIENT] Calling GetGameData(GDI_BUILD_ITEM)");
	int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);
	g_DebugLog("[CLIENT] GetGameData returned nCount = %d", nCount);

	// Safety check: nCount should never exceed array size
	if (nCount > BUILD_CONTAINER_SIZE)
	{
		g_DebugLog("[CLIENT] WARNING: nCount (%d) exceeds array size (%d)! Clamping.", nCount, BUILD_CONTAINER_SIZE);
		nCount = BUILD_CONTAINER_SIZE;
	}

	for (int i = 0; i < nCount; i++)
	{
		g_DebugLog("[CLIENT] Checking item %d, genre = %d", i, Item[i].Obj.uGenre);
		if (Item[i].Obj.uGenre != CGOG_NOTHING)
		{
			g_DebugLog("[CLIENT] Calling UpdateItem for slot %d", i);
			UpdateItem(&Item[i], true);
			g_DebugLog("[CLIENT] UpdateItem completed for slot %d", i);
		}
	}

	g_DebugLog("[CLIENT] UpdateData() completed");
}

/*********************************************************************
 * Update Item
 *********************************************************************/
void KUiUpgradeAttrib::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	g_DebugLog("[CLIENT] UpdateItem() called, pItem=%p, bAdd=%d", pItem, bAdd);

	if (pItem)
	{
		g_DebugLog("[CLIENT] pItem->Region.v = %d", pItem->Region.v);

		for (int i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
		{
			g_DebugLog("[CLIENT] Checking slot %d, CtrlItemMap[%d].nPosition = %d", i, i, CtrlItemMap[i].nPosition);

			if (CtrlItemMap[i].nPosition == pItem->Region.v)
			{
				g_DebugLog("[CLIENT] Match found at slot %d", i);
				if (bAdd)
				{
					g_DebugLog("[CLIENT] Calling HoldObject(genre=%d, id=%d, w=%d, h=%d)",
						pItem->Obj.uGenre, pItem->Obj.uId, pItem->Region.Width, pItem->Region.Height);
					m_UpgradeSlot[i].HoldObject(pItem->Obj.uGenre, pItem->Obj.uId,
						pItem->Region.Width, pItem->Region.Height);
					g_DebugLog("[CLIENT] HoldObject completed");

					// NEW: Load equipment attributes when equipment is added to slot 0
					if (i == SLOT_EQUIPMENT)
					{
						// CRITICAL: Unlock upgrade processing BEFORE loading attributes
						// This happens after successful upgrade or initial placement
						if (m_bUpgradeInProgress)
						{
							m_bUpgradeInProgress = FALSE;
							g_DebugLog("[CLIENT] Upgrade lock released - equipment added to slot 0");
						}

						// Now safe to load attributes (lock is released)
						LoadEquipmentAttributes();
					}
				}
				else
				{
					g_DebugLog("[CLIENT] Calling HoldObject(CGOG_NOTHING)");
					m_UpgradeSlot[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
					g_DebugLog("[CLIENT] HoldObject completed");

					// CRITICAL: Also unlock when equipment is REMOVED from slot 0
					// This handles failure case: DelItemByIndex(equip) triggers UpdateItem(slot=0, bAdd=0)
					if (i == SLOT_EQUIPMENT)
					{
						if (m_bUpgradeInProgress)
						{
							m_bUpgradeInProgress = FALSE;
							g_DebugLog("[CLIENT] Upgrade lock released - equipment removed from slot 0");
						}

						ClearAttributeList();
					}
				}
				break;
			}
		}
		g_DebugLog("[CLIENT] UpdateItem() completed");

		// CRITICAL: Skip success rate update during upgrade processing
		// When materials are deleted during upgrade failure, this prevents crash
		if (m_bUpgradeInProgress)
		{
			return;
		}

		// Update success rate display after item is added/removed
		UpdateSuccessRateDisplay();
	}
	else
	{
		g_DebugLog("[CLIENT] UpdateItem() - pItem is NULL!");
	}
}

/*********************************************************************
 * On Upgrade - Execute Upgrade Script
 *********************************************************************/
void KUiUpgradeAttrib::OnUpgrade()
{
	// Check if player selected an attribute
	if (m_nSelectedAttrib < 0)
	{
		// TODO: Show error message to player
		return;
	}

	// CRITICAL: Use ACTUAL slot index from equipment, not button index!
	// m_nSelectedAttrib = button index (0-5 in filtered list)
	// nActualSlot = actual equipment slot (0-5 in equipment)
	int nActualSlot = m_Attributes[m_nSelectedAttrib].nActualSlot;

	// Call Lua function with ACTUAL equipment slot index
	// Format: PerformUpgrade#<slot> where slot is actual equipment slot 0-5
	char szFunc[64];
	sprintf(szFunc, "PerformUpgrade#%d", nActualSlot);
	if (g_pCoreShell->GetLixian())
	{
		// Enable upgrade lock to prevent UI updates during server processing
		// Lock will be released when UpdateItem is received (equipment updated)
		m_bUpgradeInProgress = TRUE;
		// CRITICAL: Pass m_btExeId = 4 (upgrade system), NOT strlen!
		// Server expects case 4 for ExeTremble/ExeUpgradeAttrib/PerformUpgrade
		g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
	}
	else
	{
		g_DebugLog("[CLIENT] ERROR: GetLixian() = FALSE! Cannot execute script!");
	}

	g_DebugLog("[CLIENT] OnUpgrade() finished");
}

/*********************************************************************
 * On Cancel - Return Items to Inventory
 *********************************************************************/
void KUiUpgradeAttrib::OnCancel()
{
	if (g_pCoreShell)
	{
		// CRITICAL FIX: Same buffer overflow bug as UpdateData!
		const int BUILD_CONTAINER_SIZE = 20;
		KUiObjAtRegion Item[BUILD_CONTAINER_SIZE];
		int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);
		if (nCount)
			g_pCoreShell->OperationRequest(GOI_RECOVERY_BOX_COMMAND, pos_builditem, 0);
	}
}

/*********************************************************************
 * Update Pick Put Enable
 *********************************************************************/
void KUiUpgradeAttrib::UpdatePickPut(bool bLock)
{
	for (int i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		m_UpgradeSlot[i].EnablePickPut(bLock);
	}
	m_BtnUpgrade.Enable(bLock);
	m_BtnClose.Enable(bLock);
}
/*********************************************************************
 * Get Mineral Level (1-3)
 * Returns the level of a mineral item based on its particular value
 *
 * NOTE: This is a generic UI helper that reads item properties.
 * It does NOT validate if the item is actually a valid mineral.
 * Lua script handles validation via IsMineralItem() in ExeUpgradeAttrib().
 *
 * Mineral level is stored in particular field (1, 2, or 3)
 *********************************************************************/
int KUiUpgradeAttrib::GetMineralLevel(int nItemIndex)
{
	if (!g_pCoreShell || nItemIndex == 0)
		return 0;

	// Simply read particular value (1, 2, 3) = level
	// No game-specific validation here (Lua handles that)
	int nParticular = g_pCoreShell->GetParticularItem(nItemIndex);

	// Particular value (1, 2, 3) = mineral level
	if (nParticular >= 1 && nParticular <= 3)
		return nParticular;

	// Default to 1 if particular is out of range
	return 1;
}

/*********************************************************************
 * Calculate Success Rate
 * Base rate + bonuses from minerals (level-based) + lucky stone
 *********************************************************************/
int KUiUpgradeAttrib::CalculateSuccessRate()
{
	int nSuccessRate = 20;  // Base success rate: 30%
	KUiDraggedObject obj;

	g_DebugLog("[UPGRADE-ATTRIB] Calculating success rate...");

	// Check each mineral slot (slots 1-4)
	for (int i = SLOT_MINERAL1; i <= SLOT_MINERAL4; i++)
	{
		obj.uId = 0;
		m_UpgradeSlot[i].GetObject(obj);
		if (obj.uId > 0 && g_pCoreShell)
		{
			// Verify item still exists before accessing
			int nGenre = g_pCoreShell->GetGenreItem(obj.uId);
			if (nGenre < 0)
			{
				g_DebugLog("[UPGRADE-ATTRIB] Mineral slot %d: item %d no longer valid, skipping", i, obj.uId);
				continue;
			}

			int nLevel = GetMineralLevel(obj.uId);
			int nBonus = 0;

			// Level-based bonus:
			// Level 1: +5%
			// Level 2: +10%
			// Level 3: +15%
			switch (nLevel)
			{
			case 1: nBonus = 5; break;
			case 2: nBonus = 15; break;
			case 3: nBonus = 20; break;
			default: nBonus = 0; break;
			}

			nSuccessRate += nBonus;
			g_DebugLog("[UPGRADE-ATTRIB] Mineral slot %d: level %d, bonus +%d%%", i, nLevel, nBonus);
		}
	}

	// Check lucky stone slot (slot 5)
	obj.uId = 0;
	m_UpgradeSlot[SLOT_LUCKY_STONE].GetObject(obj);
	if (obj.uId > 0 && g_pCoreShell)
	{
		// Verify item still exists
		int nGenre = g_pCoreShell->GetGenreItem(obj.uId);
		if (nGenre >= 0)
		{
			int nLuckyBonus = 10;  // Lucky stone adds +10%
			nSuccessRate += nLuckyBonus;
			//g_DebugLog("[UPGRADE-ATTRIB] Lucky stone present, bonus +%d%%", nLuckyBonus);
		}
		else
		{
			g_DebugLog("[UPGRADE-ATTRIB] Lucky stone item %d no longer valid, skipping", obj.uId);
		}
	}

	// Cap at 100%
	if (nSuccessRate > 100)
		nSuccessRate = 100;

	g_DebugLog("[UPGRADE-ATTRIB] Total success rate: %d%%", nSuccessRate);
	return nSuccessRate;
}

/*********************************************************************
 * Update Success Rate Display
 * Updates the percentage text whenever items are added/removed
 *********************************************************************/
void KUiUpgradeAttrib::UpdateSuccessRateDisplay()
{
	// CRITICAL: Skip all updates during upgrade processing to prevent crash
	if (m_bUpgradeInProgress)
	{
		g_DebugLog("[UPGRADE-ATTRIB] Skipping success rate update - upgrade in progress");
		return;
	}

	// Safety check: Only update if window is visible
	if (!IsVisible())
		return;

	// Safety check: Only calculate if we have equipment
	KUiDraggedObject obj;
	m_UpgradeSlot[SLOT_EQUIPMENT].GetObject(obj);
	if (obj.uId == 0)
	{
		m_TextPercent.SetText("T˚ l÷: 0%");
		return;
	}

	int nRate = CalculateSuccessRate();
	char szText[128];
	sprintf(szText, "T˚ l÷: %d%%", nRate);
	m_TextPercent.SetText(szText);
	//g_DebugLog("[UPGRADE-ATTRIB] Updated success rate display: %s", szText);
}

/*********************************************************************
 * Load Equipment Attributes
 * Called when equipment is placed in slot 0
 * Reads all magic attributes and displays them for selection
 *********************************************************************/
void KUiUpgradeAttrib::LoadEquipmentAttributes()
{
	g_DebugLog("[UPGRADE-ATTRIB] LoadEquipmentAttributes() called");

	// CRITICAL: Skip if upgrade in progress to prevent crash during material deletion
	if (m_bUpgradeInProgress)
	{
		g_DebugLog("[UPGRADE-ATTRIB] Skipping LoadEquipmentAttributes - upgrade in progress");
		return;
	}

	// Clear previous data
	m_nAttributeCount = 0;
	m_nSelectedAttrib = -1;

	// Get equipment from slot 0
	KUiDraggedObject obj;
	m_UpgradeSlot[SLOT_EQUIPMENT].GetObject(obj);

	if (obj.uId == 0)
	{
		g_DebugLog("[UPGRADE-ATTRIB] No equipment in slot 0");
		ClearAttributeList();
		return;
	}

	// Get equipment series and level (needed to look up max values from attribute definition table)
	int nSeries = g_pCoreShell ? g_pCoreShell->GetSeriesItem(obj.uId) : -1;
	int nLevel = g_pCoreShell ? g_pCoreShell->GetLevelItem(obj.uId) : 0;

	// Read all magic attributes from equipment
	for (int i = 0; i < 6; i++)
	{
		int nType, nValue, nMin, nMax;
		if (g_pCoreShell && g_pCoreShell->GetItemMagicAttribInfo(obj.uId, i, &nType, &nValue, &nMin, &nMax))
		{
			// Only check nType > 0, allow nValue = 0 (some attributes can be 0)
			if (nType > 0)
			{
				m_Attributes[m_nAttributeCount].nType = nType;
				m_Attributes[m_nAttributeCount].nValue = nValue;
				m_Attributes[m_nAttributeCount].nMin = nMin;
				m_Attributes[m_nAttributeCount].nMax = nMax;

				// CRITICAL FIX: Get REAL max value from attribute definition table
				// Blue equipment doesn't store max in item data - it's in the definition table
				int nRealMax = g_pCoreShell->GetMagicAttribMaxValue(nType, nSeries, nLevel);

				// Calculate new value (10% increase)
				int nIncrease = (nValue * 10) / 100;
				if (nIncrease < 1) nIncrease = 1;
				int nNewValue = nValue + nIncrease;
				if (nRealMax > 0 && nNewValue > nRealMax) nNewValue = nRealMax;
				m_Attributes[m_nAttributeCount].nNewValue = nNewValue;

				// Check if can upgrade using REAL max value:
				// - If nRealMax <= 0: no max limit found, always can upgrade
				// - If nRealMax > 0: can upgrade only if current value < real max
				// - If nValue >= nRealMax: already at or over max, cannot upgrade
				BOOL bCanUpgrade = (nRealMax <= 0) || (nValue < nRealMax);
				m_Attributes[m_nAttributeCount].bCanUpgrade = bCanUpgrade;

				// CRITICAL: Store ACTUAL slot index from equipment (0-5)
				// Player selects button index, but we must upgrade correct slot!
				m_Attributes[m_nAttributeCount].nActualSlot = i;

				//g_DebugLog("[UPGRADE-ATTRIB] Attribute %d: type=%d, value=%d, realMax=%d, newValue=%d, actualSlot=%d, canUpgrade=%d",
					//m_nAttributeCount, nType, nValue, nRealMax, nNewValue, i, m_Attributes[m_nAttributeCount].bCanUpgrade);

				m_nAttributeCount++;
			}
		}
	}

	//g_DebugLog("[UPGRADE-ATTRIB] Loaded %d attributes", m_nAttributeCount);

	// Display the attribute list
	DisplayAttributeList();
}

/*********************************************************************
 * Display Attribute List
 * Shows all attributes as buttons for player to click
 *********************************************************************/
void KUiUpgradeAttrib::DisplayAttributeList()
{
	//g_DebugLog("[UPGRADE-ATTRIB] DisplayAttributeList() - showing %d attributes", m_nAttributeCount);

	// Hide all buttons first
	for (int i = 0; i < 6; i++)
	{
		m_BtnAttrib[i].Hide();
	}

	if (m_nAttributeCount == 0)
	{
		m_TextGuide.SetText("Trang bﬁ nµy kh´ng c„ thuÈc t›nh magic!");
		return;
	}

	// Show buttons for each attribute
	char szLabel[128];
	for (int j = 0; j < m_nAttributeCount; j++)
	{
		if (m_Attributes[j].bCanUpgrade)
		{
			// Show: current/max -> new (if has max)
			if (m_Attributes[j].nMax > 0)
			{
				sprintf(szLabel, "%d/%d -> %d",
					m_Attributes[j].nValue, m_Attributes[j].nMax, m_Attributes[j].nNewValue);
			}
			else
			{
				sprintf(szLabel, "%d -> %d (+10%%)",
					m_Attributes[j].nValue, m_Attributes[j].nNewValue);
			}
		}
		else
		{
			// Attribute at MAX - show value and MAX indicator
			sprintf(szLabel, "%d/%d (MAX)", m_Attributes[j].nValue, m_Attributes[j].nMax);
		}

		m_BtnAttrib[j].SetLabel(szLabel);
		m_BtnAttrib[j].Show();
		m_BtnAttrib[j].Enable(m_Attributes[j].bCanUpgrade ? 1 : 0);  // Disable if at MAX

		//g_DebugLog("[UPGRADE-ATTRIB] Button %d: %s, canUpgrade=%d", j, szLabel, m_Attributes[j].bCanUpgrade);
	}

	m_TextGuide.SetText("Ch‰n thuÈc t›nh muËn n©ng c p:");
}

/*********************************************************************
 * On Select Attribute
 * Called when player clicks an attribute button
 *********************************************************************/
void KUiUpgradeAttrib::OnSelectAttribute(int nSlot)
{
	if (nSlot < 0 || nSlot >= m_nAttributeCount)
	{
		g_DebugLog("[UPGRADE-ATTRIB] OnSelectAttribute() - invalid slot %d", nSlot);
		return;
	}

	if (!m_Attributes[nSlot].bCanUpgrade)
	{
		g_DebugLog("[UPGRADE-ATTRIB] OnSelectAttribute() - attribute %d cannot be upgraded (at MAX)", nSlot);
		return;
	}

	m_nSelectedAttrib = nSlot;
	g_DebugLog("[UPGRADE-ATTRIB] OnSelectAttribute() - selected attribute %d (type=%d, %d->%d)", 
		nSlot, m_Attributes[nSlot].nType, m_Attributes[nSlot].nValue, m_Attributes[nSlot].nNewValue);

	// Highlight selected button
	for (int i = 0; i < m_nAttributeCount; i++)
	{
		// You can add visual feedback here (e.g., change button color)
		// For now, just log it
	}

	char szText[128];
	sprintf(szText, "ß∑ ch‰n: %d -> %d", m_Attributes[nSlot].nValue, m_Attributes[nSlot].nNewValue);
	m_TextGuide.SetText(szText);
}

/*********************************************************************
 * Clear Attribute List
 * Called when equipment is removed from slot 0
 *********************************************************************/
void KUiUpgradeAttrib::ClearAttributeList()
{
	g_DebugLog("[UPGRADE-ATTRIB] ClearAttributeList() called");

	m_nAttributeCount = 0;
	m_nSelectedAttrib = -1;

	// Hide all attribute buttons
	for (int i = 0; i < 6; i++)
	{
		m_BtnAttrib[i].Hide();
	}

	m_TextGuide.SetText("ß∆t trang bﬁ xanh vµo Æ” xem thuÈc t›nh:");
}