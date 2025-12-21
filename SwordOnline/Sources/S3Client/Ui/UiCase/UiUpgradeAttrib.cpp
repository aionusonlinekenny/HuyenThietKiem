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
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiUpgradeAttrib;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);

		if (!KUiItem::GetIfVisible())
			KUiItem::OpenWindow();

		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		Wnd_GameSpaceHandleInput(false);
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
		Wnd_GameSpaceHandleInput(true);
		if (bDestory)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();

		m_pSelf->OnCancel();
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
	AddChild(&m_AttribList);

	for (i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		m_UpgradeSlot[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_UpgradeSlot[i]);
		m_UpgradeSlot[i].SetContainerId((int)UOC_BUILD_ITEM);
	}

	m_nSelectedAttrib = -1;  // No attribute selected initially

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
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");

			m_pSelf->m_UpgradeEffect.Init(&Ini, "Effect");
			m_pSelf->m_UpgradeEffect.Hide();
			m_pSelf->m_EffectTime = 0;

			m_pSelf->m_BtnUpgrade.Init(&Ini, "UpgradeBtn");
			m_pSelf->m_BtnClose.Init(&Ini, "CloseBtn");
			m_pSelf->m_TextPercent.Init(&Ini, "TextPercent");
			m_pSelf->m_AttribList.Init(&Ini, "AttribList");

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
				UIMessageBox("Bạn có chắc muốn nâng cấp thuộc tính này?", this, "Xác nhận", "Hủy bỏ", ISP_DO_EVENT);
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
				UIMessageBox("Bạn có chắc muốn nâng cấp thuộc tính này?", this, "Xác nhận", "Hủy bỏ", ISP_DO_EVENT);
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
	if (!pPickPos || !pDropPos)
		return;

	if (pDropPos->pWnd == &m_UpgradeSlot[0] || pDropPos->pWnd == &m_UpgradeSlot[1])
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pPickPos->uGenre;
		Obj.uId = pPickPos->uId;
		Obj.DataX = pPickPos->DataX;
		Obj.DataW = pPickPos->DataW;

		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,
				(unsigned int)(&Obj),
				pos_builditem | (pDropPos->nIndex << 16));

			// Load attribute list if equipment was placed
			if (pDropPos->pWnd == &m_UpgradeSlot[0])
			{
				LoadAttributeList();
			}
		}
	}
}

/*********************************************************************
 * Load Attribute List
 *********************************************************************/
void KUiUpgradeAttrib::LoadAttributeList()
{
	// TODO: Implement attribute list loading from equipment
	// This will query the equipment's magic attributes and display them
	m_nSelectedAttrib = 0;  // Default select first attribute
	UpdateSuccessRate();
}

/*********************************************************************
 * On Attribute Selected
 *********************************************************************/
void KUiUpgradeAttrib::OnAttributeSelected(int nIndex)
{
	if (nIndex >= 0 && nIndex < 6)
	{
		m_nSelectedAttrib = nIndex;
		UpdateSuccessRate();
	}
}

/*********************************************************************
 * Update Success Rate
 *********************************************************************/
void KUiUpgradeAttrib::UpdateSuccessRate()
{
	m_TextPercent.SetText("Tỷ lệ thành công: 100%");
}

/*********************************************************************
 * Breathe (Animation Update)
 *********************************************************************/
void KUiUpgradeAttrib::Breathe()
{
	if (m_EffectTime)
	{
		m_EffectTime++;
		m_UpgradeEffect.NextFrame();

		int nEffectMaxTime = (m_UpgradeEffect.GetMaxFrame()) * (LOOP * 2) / 2 + 1;
		if (m_EffectTime >= nEffectMaxTime)
		{
			StopEffect();
			OnUpgrade();
			CloseWindow(true);
		}
	}
	KWndShowAnimate::Breathe();
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
	m_EffectTime = 0;
	m_UpgradeEffect.Hide();
	UpdatePickPut(true);
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
	if (!g_pCoreShell) return;

	KUiObjAtRegion Item[_UPGRADE_ATTRIB_SLOT_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);

	for (int i = 0; i < nCount; i++)
	{
		if (Item[i].Obj.uGenre != CGOG_NOTHING)
			UpdateItem(&Item[i], true);
	}
}

/*********************************************************************
 * Update Item
 *********************************************************************/
void KUiUpgradeAttrib::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (!pItem) return;

	for (int i = 0; i < _UPGRADE_ATTRIB_SLOT_COUNT; i++)
	{
		if (pItem->Region.h == i)
		{
			if (bAdd)
			{
				m_UpgradeSlot[i].SetObject(&pItem->Obj, pItem->Region);

				// Load attributes if equipment was added
				if (i == 0)
				{
					LoadAttributeList();
				}
			}
			else
			{
				m_UpgradeSlot[i].ClearObject();
			}
			break;
		}
	}
}

/*********************************************************************
 * On Upgrade - Execute Upgrade Script
 *********************************************************************/
void KUiUpgradeAttrib::OnUpgrade()
{
	if (g_pCoreShell)
	{
		// Set selected attribute index to TaskTemp for Lua to read
		g_pCoreShell->OperationRequest(GOI_TASK_SET_TEMP, 200, m_nSelectedAttrib);

		// Execute Lua script
		char szFunc[32];
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
