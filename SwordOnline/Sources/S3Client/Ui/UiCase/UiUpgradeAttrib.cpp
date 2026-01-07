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
	{ UIEP_BUILDITEM1, "Equipment" },  // Equipment slot
	{ UIEP_BUILDITEM2, "Material" },   // Material slot
	{ UIEP_BUILDITEM3, "Spare" },      // Spare slot (future use)
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

		g_DebugLog("[CLIENT] Calling BringToTop() before Show()");
		m_pSelf->BringToTop();
		g_DebugLog("[CLIENT] Calling Show()");
		m_pSelf->Show();

		g_DebugLog("[CLIENT] Opening KUiItem if needed");
		if (!KUiItem::GetIfVisible())
			KUiItem::OpenWindow();

		// CRITICAL: Bring upgrade UI to top AFTER opening inventory
		// This ensures upgrade UI receives keyboard input (like ESC) before inventory
		m_pSelf->BringToTop();
		g_DebugLog("[UPGRADE-ATTRIB] BringToTop called after inventory open to receive ESC key");

		g_DebugLog("[CLIENT] Calling UpdateData()");
		m_pSelf->UpdateData();
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

	for (i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		m_UpgradeSlot[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_UpgradeSlot[i]);
		m_UpgradeSlot[i].SetContainerId((int)UOC_BUILD_ITEM);
	}

	m_nSelectedAttrib = 0;  // Default to first attribute

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
		m_pSelf->m_TextPercent.SetText("S�n s�ng n�ng c�p");  // Set initial text

		// Initialize slot labels
		m_pSelf->m_EquipmentLabel.Init(&Ini, "EquipmentLabel");
		m_pSelf->m_MaterialLabel.Init(&Ini, "MaterialLabel");

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
		if (uParam == (unsigned int)&m_BtnUpgrade)
		{
			g_DebugLog("[CLIENT] Upgrade button clicked");
			if (ValidateUpgradeReady())
			{
				g_DebugLog("[CLIENT] Validation passed, showing confirmation dialog");
				if (m_EffectTime) break;
				UIMessageBox("B�n c� ch�c mu�n n�ng c�p trang b� n�y?", this, "X�c nh�n", "H�y b�", ISP_DO_EVENT);
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
		break;

	case WM_KEYDOWN:
		g_DebugLog("[UPGRADE-ATTRIB] WM_KEYDOWN received: uParam=%d (VK_ESCAPE=%d)", uParam, VK_ESCAPE);
		if (uParam == VK_RETURN)  // Enter
		{
			g_DebugLog("[UPGRADE-ATTRIB] Enter pressed, attempting upgrade");
			if (ValidateUpgradeReady())
			{
				UIMessageBox("B�n c� ch�c mu�n n�ng c�p trang b� n�y?", this, "X�c nh�n", "H�y b�", ISP_DO_EVENT);
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

	// Check equipment slot
	pObj.uId = 0;
	m_UpgradeSlot[0].GetObject(pObj);
	if (pObj.uId == 0)
	{
		strcpy(szWarning, "Ch�a ��t trang bi v�o!");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// Check material slot
	pObj.uId = 0;
	m_UpgradeSlot[1].GetObject(pObj);
	if (pObj.uId == 0)
	{
		strcpy(szWarning, "Ch�a ��t �� n�ng c�p v�o.");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// Check attribute selection (skip this check for now - will use first attribute)
	// User can select attributes in future version
	if (m_nSelectedAttrib < 0)
	{
		m_nSelectedAttrib = 0;  // Default to first attribute
	}

	return TRUE;
}

/*********************************************************************
 * Validate Item Pick Drop
 *********************************************************************/
BOOL KUiUpgradeAttrib::ValidateItemPickDrop(KWndWindow* pWnd, int nIndex)
{
	// Slot 0: Only blue equipment
	if (pWnd == &m_UpgradeSlot[0])
	{
		// Will add validation for blue equipment here
		return TRUE;
	}
	// Slot 1: Only upgrade materials
	else if (pWnd == &m_UpgradeSlot[1])
	{
		// Will add validation for material items here
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
	if (m_UpgradeEffect.IsVisible())
		m_UpgradeEffect.NextFrame();
	if (m_EffectTime)
		m_EffectTime++;
	if (m_EffectTime == (m_UpgradeEffect.GetMaxFrame()) * (LOOP * 2) / 2 + 1)
	{
		StopEffect();
		m_EffectTime = 0;
	}
}

/*********************************************************************
 * Start Effect
 *********************************************************************/
void KUiUpgradeAttrib::StartEffect()
{
	m_UpgradeEffect.Show();
	UpdatePickPut(false);
}

/*********************************************************************
 * Stop Effect
 *********************************************************************/
void KUiUpgradeAttrib::StopEffect()
{
	m_UpgradeEffect.Hide();
	UpdatePickPut(true);
	OnUpgrade();
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
				}
				else
				{
					g_DebugLog("[CLIENT] Calling HoldObject(CGOG_NOTHING)");
					m_UpgradeSlot[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
					g_DebugLog("[CLIENT] HoldObject completed");
				}
				break;
			}
		}
		g_DebugLog("[CLIENT] UpdateItem() completed");
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
	char szFunc[32];
	sprintf(szFunc, "ExeUpgradeAttrib");

	g_DebugLog("[CLIENT] OnUpgrade() - Calling script: %s, SelectedAttrib=%d", szFunc, m_nSelectedAttrib);

	if (g_pCoreShell->GetLixian())
	{
		g_DebugLog("[CLIENT] GetLixian() = TRUE, calling OperationRequest");
		g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
		g_DebugLog("[CLIENT] OnUpgrade() - Script call sent");
	}
	else
	{
		g_DebugLog("[CLIENT] ERROR: GetLixian() = FALSE! Cannot execute script!");
		g_DebugLog("[CLIENT] This usually means player is in offline/test mode");
	}
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
		m_UpgradeSlot[i].EnablePickPut(bLock);

	m_BtnUpgrade.Enable(bLock);
	m_BtnClose.Enable(bLock);
}