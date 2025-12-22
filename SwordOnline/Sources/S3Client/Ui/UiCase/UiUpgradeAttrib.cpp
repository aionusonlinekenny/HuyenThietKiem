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
		m_pSelf->m_BtnClose.Init(&Ini, "CloseBtn");
		m_pSelf->m_TextPercent.Init(&Ini, "TextPercent");

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
			if (ValidateUpgradeReady())
			{
				if (m_EffectTime) break;
				UIMessageBox("Ban co chac muon nang cap thuoc tinh nay?", this, "Xac nhan", "Huy bo", ISP_DO_EVENT);
			}
		}
		else if (uParam == (unsigned int)&m_BtnClose)
		{
			if (m_EffectTime) break;
			CloseWindow(true);
		}
		break;

	case WM_KEYDOWN:
		if (uParam == VK_RETURN)  // Enter
		{
			if (ValidateUpgradeReady())
			{
				UIMessageBox("Ban co chac muon nang cap thuoc tinh nay?", this, "Xac nhan", "Huy bo", ISP_DO_EVENT);
			}
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)  // ESC
		{
			CloseWindow(true);
			nRet = 1;
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
		strcpy(szWarning, "Chưa đặt trang bị vào!");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// Check material slot
	pObj.uId = 0;
	m_UpgradeSlot[1].GetObject(pObj);
	if (pObj.uId == 0)
	{
		strcpy(szWarning, "Chưa đặt Đá Nâng Cấp vào!");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// Check attribute selection
	if (m_nSelectedAttrib < 0 || m_nSelectedAttrib >= 6)
	{
		strcpy(szWarning, "Chưa chọn thuộc tính cần nâng cấp!");
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
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

	if (pPickPos)
	{
		pWnd = pPickPos->pWnd;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

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

	g_DebugLog("[CLIENT] Creating Item array, size = %d", _UPGRADE_ATTRIB_SLOT_COUNT);
	KUiObjAtRegion Item[_UPGRADE_ATTRIB_SLOT_COUNT];

	g_DebugLog("[CLIENT] Calling GetGameData(GDI_BUILD_ITEM)");
	int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);
	g_DebugLog("[CLIENT] GetGameData returned nCount = %d", nCount);

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
	if (g_pCoreShell->GetLixian())
	{
		char szFunc[16];
		sprintf(szFunc, "ExeUpgradeAttrib");
		g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
	}
}

/*********************************************************************
 * On Cancel - Return Items to Inventory
 *********************************************************************/
void KUiUpgradeAttrib::OnCancel()
{
	if (g_pCoreShell)
	{
		KUiObjAtRegion Item[_UPGRADE_ATTRIB_SLOT_COUNT];
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
