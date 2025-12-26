/*******************************************************************************
File        : UiPurpleSmith.cpp
Creator     : Based on UiTrembleItem by AlexKing
Create Date : 2025-12-26
Function    : Purple Item Crafting System - Tho Ren Huyen Bi
*******************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiPurpleSmith.h"
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
#define PURPLE_SMITH_INI "UiPurpleSmith.ini"

// Crystal detail IDs
#define SP_LAMTHUYTINH   16
#define SP_TUTHUYTINH    17
#define SP_LUCTHUYTINH   18
#define SP_HONGTHUYTINH  100

KUiPurpleSmith* KUiPurpleSmith::m_pSelf = NULL;

static struct PS_CTRL_MAP
{
	int nPosition;
	const char* pIniSection;
}

CtrlItemMap[_PURPLE_SMITH_SLOT_COUNT] =
{
	// Blue equipment slots (10 slots)
	{ UIEP_BUILDITEM1,  "BlueItem1"  },
	{ UIEP_BUILDITEM2,  "BlueItem2"  },
	{ UIEP_BUILDITEM3,  "BlueItem3"  },
	{ UIEP_BUILDITEM4,  "BlueItem4"  },
	{ UIEP_BUILDITEM5,  "BlueItem5"  },
	{ UIEP_BUILDITEM6,  "BlueItem6"  },
	{ UIEP_BUILDITEM7,  "BlueItem7"  },
	{ UIEP_BUILDITEM8,  "BlueItem8"  },
	{ UIEP_BUILDITEM9,  "BlueItem9"  },
	{ UIEP_BUILDITEM10, "BlueItem10" },
	// Crystal slots (7 slots)
	{ UIEP_BUILDITEM11, "Crystal1"   },
	{ UIEP_BUILDITEM12, "Crystal2"   },
	{ UIEP_BUILDITEM13, "Crystal3"   },
	{ UIEP_BUILDITEM14, "Crystal4"   },
	{ UIEP_BUILDITEM15, "Crystal5"   },
	{ UIEP_BUILDITEM16, "Crystal6"   },
	{ UIEP_BUILDITEM17, "Crystal7"   }
};

/*********************************************************************
 * Open Window
 *********************************************************************/
KUiPurpleSmith* KUiPurpleSmith::OpenWindow()
{
	g_DebugLog("[CLIENT] KUiPurpleSmith::OpenWindow() called");

	if (m_pSelf == NULL)
	{
		g_DebugLog("[CLIENT] Creating new KUiPurpleSmith instance");
		m_pSelf = new KUiPurpleSmith;
		if (m_pSelf)
		{
			g_DebugLog("[CLIENT] Calling Initialize()");
			m_pSelf->Initialize();
			g_DebugLog("[CLIENT] Initialize() completed");
		}
		else
		{
			g_DebugLog("[CLIENT] ERROR: Failed to allocate KUiPurpleSmith!");
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
KUiPurpleSmith* KUiPurpleSmith::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

/*********************************************************************
 * Close Window
 *********************************************************************/
void KUiPurpleSmith::CloseWindow(bool bDestory)
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
void KUiPurpleSmith::Initialize()
{
	int i = 0;
	AddChild(&m_BtnCraft);
	AddChild(&m_BtnClose);
	AddChild(&m_CraftEffect);
	AddChild(&m_TextSuccessRate);
	AddChild(&m_TextCost);
	AddChild(&m_BlueItemsLabel);
	AddChild(&m_CrystalsLabel);

	for (i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
	{
		m_CraftSlot[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_CraftSlot[i]);
		m_CraftSlot[i].SetContainerId((int)UOC_BUILD_ITEM);
	}

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

/*********************************************************************
 * Load Scheme
 *********************************************************************/
void KUiPurpleSmith::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char Buff[128];
		KIniFile Ini;
		int i = 0;
		sprintf(Buff, "%s\\%s", pScheme, PURPLE_SMITH_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");

			m_pSelf->m_CraftEffect.Init(&Ini, "Effect");
			m_pSelf->m_CraftEffect.Hide();
			m_pSelf->m_EffectTime = 0;

			m_pSelf->m_BtnCraft.Init(&Ini, "CraftBtn");
			m_pSelf->m_BtnClose.Init(&Ini, "CloseBtn");
			m_pSelf->m_TextSuccessRate.Init(&Ini, "TextSuccessRate");
			m_pSelf->m_TextCost.Init(&Ini, "TextCost");
			m_pSelf->m_BlueItemsLabel.Init(&Ini, "BlueItemsLabel");
			m_pSelf->m_CrystalsLabel.Init(&Ini, "CrystalsLabel");

			for (i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
			{
				m_pSelf->m_CraftSlot[i].Init(&Ini, CtrlItemMap[i].pIniSection);
			}

			char szTemp[2];
			for (i = 0; i < 10; i++)
			{
				sprintf(szTemp, "%d", i);
				Ini.GetString("ReturnInfo", szTemp, "", m_pSelf->m_szReturnInfo[i], sizeof(m_pSelf->m_szReturnInfo[i]));
			}

			// Set default labels
			m_pSelf->m_BlueItemsLabel.SetText("Trang bi xanh (6-10 mon)");
			m_pSelf->m_CrystalsLabel.SetText("Huyen Tinh (toi da 7 vien)");
			m_pSelf->m_TextSuccessRate.SetText("Ti le thanh cong: --");
			m_pSelf->m_TextCost.SetText("Chi phi: --");
		}
	}
}

/*********************************************************************
 * Window Procedure
 *********************************************************************/
int KUiPurpleSmith::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)&m_BtnCraft)
		{
			if (ValidateCraftReady())
			{
				if (m_EffectTime) break;
				UIMessageBox(m_szReturnInfo[0], this, "Xac nhan", "Huy bo", ISP_DO_EVENT);
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
			if (ValidateCraftReady())
			{
				UIMessageBox(m_szReturnInfo[0], this, "Xac nhan", "Huy bo", ISP_DO_EVENT);
			}
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)  // Esc
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
				m_CraftEffect.SetFrame(-1);
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
 * Validate Craft Ready
 *********************************************************************/
BOOL KUiPurpleSmith::ValidateCraftReady()
{
	int nLen;
	char szWarning[128];
	int nBlueCount = 0;
	int nCrystalCount = 0;
	int i;
	KUiDraggedObject pObj;

	// Count blue items (first 10 slots)
	for (i = 0; i < 10; i++)
	{
		pObj.uId = 0;
		m_CraftSlot[i].GetObject(pObj);
		if (pObj.uId > 0)
			nBlueCount += 1;
	}

	// Count crystals (last 7 slots)
	for (i = 10; i < _PURPLE_SMITH_SLOT_COUNT; i++)
	{
		pObj.uId = 0;
		m_CraftSlot[i].GetObject(pObj);
		if (pObj.uId > 0)
			nCrystalCount += 1;
	}

	// Minimum 6 blue items required
	if (nBlueCount < 6)
	{
		strcpy(szWarning, m_szReturnInfo[1]);
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	// At least 1 crystal required
	if (nCrystalCount < 1)
	{
		strcpy(szWarning, m_szReturnInfo[2]);
		nLen = strlen(szWarning);
		KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
		return FALSE;
	}

	return TRUE;
}

/*********************************************************************
 * Validate Item Pick Drop
 *********************************************************************/
BOOL KUiPurpleSmith::ValidateItemPickDrop(KWndWindow* pWnd, int nIndex)
{
	int nLen;
	char szWarning[128];
	int nGenre, nDetail, nParti, nSeries, nLevel, nStack;
	int i;

	nGenre = g_pCoreShell->GetGenreItem(nIndex);
	nDetail = g_pCoreShell->GetDetailItem(nIndex);
	nParti = g_pCoreShell->GetParticularItem(nIndex);
	nLevel = g_pCoreShell->GetLevelItem(nIndex);
	nSeries = g_pCoreShell->GetSeriesItem(nIndex);
	nStack = g_pCoreShell->GetNumStack(nIndex);

	// Check if it's a blue equipment slot (first 10 slots)
	for (i = 0; i < 10; i++)
	{
		if (pWnd == (KWndWindow*)&m_CraftSlot[i])
		{
			if (nGenre != item_equip)
			{
				strcpy(szWarning, m_szReturnInfo[3]);
				nLen = strlen(szWarning);
				KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
				return FALSE;
			}

			// Don't allow horse or mask
			if (nDetail == equip_horse || nDetail == equip_mask)
				return FALSE;

			UpdateSuccessRate();
			return TRUE;
		}
	}

	// Check if it's a crystal slot (last 7 slots)
	for (i = 10; i < _PURPLE_SMITH_SLOT_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_CraftSlot[i])
		{
			if (nGenre != item_task)
			{
				strcpy(szWarning, m_szReturnInfo[4]);
				nLen = strlen(szWarning);
				KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
				return FALSE;
			}

			// Check if it's a valid crystal
			if (nDetail != SP_LAMTHUYTINH && nDetail != SP_TUTHUYTINH &&
				nDetail != SP_LUCTHUYTINH && nDetail != SP_HONGTHUYTINH)
			{
				strcpy(szWarning, m_szReturnInfo[4]);
				nLen = strlen(szWarning);
				KUiMsgCentrePad::SystemMessageArrival(szWarning, nLen);
				return FALSE;
			}

			UpdateSuccessRate();
			return TRUE;
		}
	}

	return TRUE;
}

/*********************************************************************
 * Update Success Rate Display
 *********************************************************************/
void KUiPurpleSmith::UpdateSuccessRate()
{
	int nBlueCount = 0;
	int i;
	KUiDraggedObject pObj;

	// Count blue items
	for (i = 0; i < 10; i++)
	{
		pObj.uId = 0;
		m_CraftSlot[i].GetObject(pObj);
		if (pObj.uId > 0)
			nBlueCount += 1;
	}

	char szText[64];
	if (nBlueCount >= 6 && nBlueCount <= 7)
		sprintf(szText, "Ti le thanh cong: 60%% | Chi phi: 500,000 luong");
	else if (nBlueCount == 8)
		sprintf(szText, "Ti le thanh cong: 70%% | Chi phi: 800,000 luong");
	else if (nBlueCount == 9)
		sprintf(szText, "Ti le thanh cong: 75%% | Chi phi: 1,000,000 luong");
	else if (nBlueCount == 10)
		sprintf(szText, "Ti le thanh cong: 80%% | Chi phi: 1,500,000 luong");
	else
		sprintf(szText, "Ti le thanh cong: -- | Chi phi: --");

	m_TextSuccessRate.SetText(szText);
}

/*********************************************************************
 * On Item Pick Drop
 *********************************************************************/
void KUiPurpleSmith::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion Drop, Pick;
	KUiDraggedObject Obj;
	KWndWindow* pWnd = NULL;

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

	for (int i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_CraftSlot[i])
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
 * Breathe (animation update)
 *********************************************************************/
void KUiPurpleSmith::Breathe()
{
	if (m_CraftEffect.IsVisible())
		m_CraftEffect.NextFrame();

	if (m_EffectTime)
		m_EffectTime++;

	if (m_EffectTime == (m_CraftEffect.GetMaxFrame()) * (LOOP * 2) / 2 + 1)
	{
		StopEffect();
		m_EffectTime = 0;
	}
}

/*********************************************************************
 * Start Effect
 *********************************************************************/
void KUiPurpleSmith::StartEffect()
{
	m_CraftEffect.Show();
	UpdatePickPut(false);
}

/*********************************************************************
 * Stop Effect
 *********************************************************************/
void KUiPurpleSmith::StopEffect()
{
	m_CraftEffect.Hide();
	UpdatePickPut(true);
	OnCraft();
}

/*********************************************************************
 * Is Effect
 *********************************************************************/
BOOL KUiPurpleSmith::IsEffect()
{
	if (m_EffectTime) return TRUE;
	return FALSE;
}

/*********************************************************************
 * Update Item
 *********************************************************************/
void KUiPurpleSmith::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{
		for (int i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
		{
			if (CtrlItemMap[i].nPosition == pItem->Region.v)
			{
				if (bAdd)
					m_CraftSlot[i].HoldObject(pItem->Obj.uGenre, pItem->Obj.uId,
						pItem->Region.Width, pItem->Region.Height);
				else
					m_CraftSlot[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}

/*********************************************************************
 * Update Data
 *********************************************************************/
void KUiPurpleSmith::UpdateData()
{
	if (!g_pCoreShell) return;

	KUiObjAtRegion Item[_PURPLE_SMITH_SLOT_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);
	int i;

	for (i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
		m_CraftSlot[i].HoldObject(CGOG_NOTHING, 0, 0, 0);

	for (i = 0; i < nCount; i++)
		UpdateItem(&Item[i], TRUE);

	UpdatePickPut(true);
	UpdateSuccessRate();
}

/*********************************************************************
 * On Craft - Execute purple item crafting
 *********************************************************************/
void KUiPurpleSmith::OnCraft()
{
	if (g_pCoreShell->GetLixian())
	{
		char szFunc[32];
		sprintf(szFunc, "ExecuteCreatePurple");
		g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
	}
}

/*********************************************************************
 * On Cancel - Return all items
 *********************************************************************/
void KUiPurpleSmith::OnCancel()
{
	if (g_pCoreShell)
	{
		KUiObjAtRegion Item[_PURPLE_SMITH_SLOT_COUNT];
		int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Item, 0);
		if (nCount)
			g_pCoreShell->OperationRequest(GOI_RECOVERY_BOX_COMMAND, pos_builditem, 0);
	}
}

/*********************************************************************
 * Update Pick Put - Enable/Disable item manipulation
 *********************************************************************/
void KUiPurpleSmith::UpdatePickPut(bool bLock)
{
	for (int i = 0; i < _PURPLE_SMITH_SLOT_COUNT; i++)
		m_CraftSlot[i].EnablePickPut(bLock);

	m_BtnCraft.Enable(bLock);
	m_BtnClose.Enable(bLock);
}
