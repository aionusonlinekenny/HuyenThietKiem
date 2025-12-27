#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiItem.h"
#include "UiCompoundItem.h"
#include "UiMsgCentrePad.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"
#include "../../Engine/src/KDebug.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"

extern iRepresentShell *g_pRepresentShell;

extern iCoreShell *g_pCoreShell;

KUiComItem *KUiComItem::m_pSelf = NULL;

#define    SCHEME_INI_SHEET            "UiCompoundItem_Sheet.ini"
#define    SCHEME_INI_COMPE            "UiCompoundItem_Compe.ini"
#define    SCHEME_INI_BUILD            "UiCompoundItem_Build.ini"
#define    SCHEME_INI_OUTIN            "UiCompoundItem_Outin.ini"
//#define		SCHEME_INI_INOUT			"UiCompoundItem_Inout.ini"//them

void KUiComItem::LoadScheme(const char *pScheme) {
    char Buff[128];
    KIniFile Ini;
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHEET);
    if (m_pSelf && Ini.Load(Buff)) {
        m_pSelf->Init(&Ini, "Main");
        m_pSelf->m_CompoundPadBtn.Init(&Ini, "CompoundBtn");
        m_pSelf->m_DistillPadBtn.Init(&Ini, "DistillBtn");
        m_pSelf->m_ForgePadBtn.Init(&Ini, "ForgeBtn");
        m_pSelf->m_EnchasePadBtn.Init(&Ini, "EnchaseBtn");
        m_pSelf->m_AtlasPadBtn.Init(&Ini, "AtlasBtn");
        m_pSelf->m_UpCryoliteBtn.Init(&Ini, "CryoliteBtn");
        m_pSelf->m_UpPropMineBtn.Init(&Ini, "PropMineBtn");
        m_pSelf->m_CheTao.Init(&Ini, "CheTao");
        m_pSelf->m_CheTao.SetText("Ch� T�o");
        m_pSelf->m_Close.Init(&Ini, "CloseBtn");

        m_pSelf->m_CompoundPad.LoadScheme(pScheme);
// 		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
// 		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
    }
}

void KUiComItem::Initialize() {
    AddChild(&m_CompoundPadBtn);
    AddChild(&m_DistillPadBtn);
    AddChild(&m_EnchasePadBtn);
    AddChild(&m_ForgePadBtn);
    AddChild(&m_AtlasPadBtn);
    AddChild(&m_UpCryoliteBtn);
    AddChild(&m_UpPropMineBtn);
    AddChild(&m_CheTao);
    AddChild(&m_Close);

    m_CompoundPad.Initialize();
    AddPage(&m_CompoundPad, &m_CompoundPadBtn);

    m_DistillPad.Initialize();
    AddPage(&m_DistillPad, &m_DistillPadBtn);

    m_ForgePad.Initialize();
    AddPage(&m_ForgePad, &m_ForgePadBtn);

    m_EnchasePad.Initialize();
    AddPage(&m_EnchasePad, &m_EnchasePadBtn);

    m_AtlasPad.Initialize();
    AddPage(&m_AtlasPad, &m_AtlasPadBtn);

    char Scheme[256];
    g_UiBase.GetCurSchemePath(Scheme, 256);
    LoadScheme(Scheme);

    Wnd_AddWindow(this);
}

/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiComItem *KUiComItem::OpenWindow() {
    if (m_pSelf == NULL) {
        m_pSelf = new KUiComItem;
        if (m_pSelf)
            m_pSelf->Initialize();
    }
    if (m_pSelf) {
        UiSoundPlay(UI_SI_WND_OPENCLOSE);
        m_pSelf->m_nStatus = STATUS_WAITING_MATERIALS;
        m_pSelf->m_nNum = WINDOWS_COMP;
        m_pSelf->m_CompoundPadBtn.CheckButton(TRUE);
        m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
        m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
        m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
        m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
        m_pSelf->m_UpCryoliteBtn.Hide();
        m_pSelf->m_UpPropMineBtn.Hide();
        m_pSelf->BringToTop();
        m_pSelf->Show();

        if (KUiItem::GetIfVisible() == NULL)
            KUiItem::OpenWindow();
        else
            UiSoundPlay(UI_SI_WND_OPENCLOSE);
        // NOTE: Do NOT call OnNpcTradeMode(true) - it disables inventory drag!
        // Crafting UIs need players to drag items FROM inventory
    }
    return m_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiComItem *KUiComItem::GetIfVisible() {
    if (m_pSelf && m_pSelf->IsVisible())
        return m_pSelf;
    return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiComItem::CloseWindow(bool bDestory) {
    if (m_pSelf) {
        m_pSelf->Hide();
        if (bDestory) {
            m_pSelf->Destroy();
            m_pSelf = NULL;
        }
    }
    // NOTE: No need to call OnNpcTradeMode(false) since we never called OnNpcTradeMode(true)
}

int KUiComItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    switch (uMsg) {
        case WND_N_BUTTON_CLICK:
            if (uParam == (unsigned int) &m_Close) {
                CloseWindow();
            } else if (uParam == (unsigned int) &m_UpPropMineBtn && m_UpCryoliteBtn.IsVisible()) {
                m_nNum = WINDOWS_COMP3;
                ShowWindow(0);
                m_CompoundPad.SetPosText(3);
            } else if (uParam == (unsigned int) &m_UpCryoliteBtn && m_UpCryoliteBtn.IsVisible()) {
                m_nNum = WINDOWS_COMP2;
                ShowWindow(0);
                m_CompoundPad.SetPosText(2);
            } else if (uParam == (unsigned int) &m_CompoundPadBtn && m_UpCryoliteBtn.IsVisible()) {
                m_nNum = WINDOWS_COMP;
                ShowWindow(0);
                m_CompoundPad.SetPosText(1);
            } else if (uParam == (unsigned int) &m_CompoundPadBtn && !m_UpCryoliteBtn.IsVisible()) {
                m_CompoundPadBtn.CheckButton(TRUE);
                int nX, nY;
                m_CompoundPadBtn.GetPosition(&nX, &nY);
                m_UpCryoliteBtn.SetPosition(nX, nY + 19);
                m_UpPropMineBtn.SetPosition(nX, nY + 38);
                m_UpCryoliteBtn.BringToTop();
                m_UpPropMineBtn.BringToTop();
                m_UpCryoliteBtn.Show();
                m_UpPropMineBtn.Show();
            } else if (uParam == (unsigned int) &m_DistillPadBtn) {
                m_nNum = WINDOWS_DISTill;
                ShowWindow(1);
            } else if (uParam == (unsigned int) &m_ForgePadBtn) {
                m_nNum = WINDOWS_FORG;
                ShowWindow(2);
            } else if (uParam == (unsigned int) &m_EnchasePadBtn) {
                m_nNum = WINDOWS_ENCHASE;
                ShowWindow(3);
            } else if (uParam == (unsigned int) &m_AtlasPadBtn) {
                m_nNum = WINDOWS_ATLAS;
                ShowWindow(4);
            }
            break;

        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiComItem::ShowWindow(int nNum /*= 0*/ ) {
    m_pSelf->CleanItem();
    switch (nNum) {
        case 0:
            m_pSelf->m_CompoundPad.Show();
            m_pSelf->m_CompoundPadBtn.CheckButton(TRUE);
            m_pSelf->m_UpCryoliteBtn.CheckButton(FALSE);
            m_pSelf->m_UpPropMineBtn.CheckButton(FALSE);
            m_pSelf->m_UpCryoliteBtn.Hide();
            m_pSelf->m_UpPropMineBtn.Hide();
            m_pSelf->m_DistillPad.Hide();
            m_pSelf->m_EnchasePad.Hide();
            m_pSelf->m_ForgePad.Hide();
            m_pSelf->m_AtlasPad.Hide();
            m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
            m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
            m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
            m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
            break;
        case 1:
            m_pSelf->m_DistillPad.Show();
            m_pSelf->m_DistillPadBtn.CheckButton(TRUE);
            m_pSelf->m_UpCryoliteBtn.Hide();
            m_pSelf->m_UpPropMineBtn.Hide();
            m_pSelf->m_CompoundPad.Hide();
            m_pSelf->m_EnchasePad.Hide();
            m_pSelf->m_ForgePad.Hide();
            m_pSelf->m_AtlasPad.Hide();
            m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
            m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
            m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
            m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
            break;
        case 2:
            m_pSelf->m_ForgePad.Show();
            m_pSelf->m_ForgePad.UpdateData(); // Load items from server when showing Forge tab
            m_pSelf->m_ForgePadBtn.CheckButton(TRUE);
            m_pSelf->m_UpCryoliteBtn.Hide();
            m_pSelf->m_UpPropMineBtn.Hide();
            m_pSelf->m_DistillPad.Hide();
            m_pSelf->m_EnchasePad.Hide();
            m_pSelf->m_CompoundPad.Hide();
            m_pSelf->m_AtlasPad.Hide();
            m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
            m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
            m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
            m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
            break;
        case 3:
            m_pSelf->m_EnchasePad.Show();
            m_pSelf->m_EnchasePadBtn.CheckButton(TRUE);
            m_pSelf->m_UpCryoliteBtn.Hide();
            m_pSelf->m_UpPropMineBtn.Hide();
            m_pSelf->m_DistillPad.Hide();
            m_pSelf->m_CompoundPad.Hide();
            m_pSelf->m_ForgePad.Hide();
            m_pSelf->m_AtlasPad.Hide();
            m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
            m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
            m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
            m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
            break;
        case 4:
            m_pSelf->m_AtlasPad.Show();
            m_pSelf->m_AtlasPadBtn.CheckButton(TRUE);
            m_pSelf->m_UpCryoliteBtn.Hide();
            m_pSelf->m_UpPropMineBtn.Hide();
            m_pSelf->m_DistillPad.Hide();
            m_pSelf->m_EnchasePad.Hide();
            m_pSelf->m_ForgePad.Hide();
            m_pSelf->m_CompoundPad.Hide();
            m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
            m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
            m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
            m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
            break;
    }
}

void KUiComItem::UpdateItem(KUiObjAtRegion *pItem, int bAdd) {
    g_DebugLog("[COMPOUND] KUiComItem::UpdateItem called: pItem=%p, bAdd=%d, m_nNum=%d", pItem, bAdd, m_nNum);

    // Add null check to prevent crashes when sharing UOC_BUILD_ITEM container
    if (!pItem) {
        g_DebugLog("[COMPOUND] pItem is NULL, returning");
        return;
    }

    g_DebugLog("[COMPOUND] Item data: genre=%d, id=%d, Region.h=%d, Region.v=%d, Width=%d, Height=%d",
        pItem->Obj.uGenre, pItem->Obj.uId, pItem->Region.h, pItem->Region.v,
        pItem->Region.Width, pItem->Region.Height);

    KUiDraggedObject obj;
    obj.uGenre = pItem->Obj.uGenre;
    obj.uId = pItem->Obj.uId;
    obj.DataX = pItem->Region.h;
    obj.DataY = pItem->Region.v;
    obj.DataW = pItem->Region.Width;
    obj.DataH = pItem->Region.Height;

    g_DebugLog("[COMPOUND] Converted to KUiDraggedObject: DataY(Region.v)=%d", obj.DataY);

    switch (m_nNum) {
        case WINDOWS_COMP: {
            m_CompoundPad.UpdateItem(&obj, bAdd);
        }
            break;
        case WINDOWS_COMP2: {
            m_CompoundPad.UpdateItem(&obj, 4);
        }
            break;
        case WINDOWS_COMP3: {
            m_CompoundPad.UpdateItem(&obj, 4);
        }
            break;
        case WINDOWS_DISTill: {
            m_DistillPad.UpdateItem(&obj, bAdd);
        }
            break;
        case WINDOWS_FORG: {
            m_ForgePad.UpdateItem(&obj, bAdd);
        }
            break;
        case WINDOWS_ENCHASE: {
            m_EnchasePad.UpdateItem(&obj, bAdd);
        }
            break;
        case WINDOWS_ATLAS: {

        }
            break;
    }
}

int KUiComItem::GetWindowsNum() {
    return m_nNum;
}

void KUiComItem::ComItem(unsigned int pItem, int nWindowNum, int nNum) {
    switch (nWindowNum) {
        case WINDOWS_COMP:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_COM, pItem, 1);
            }
            break;
        case WINDOWS_COMP2:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_COM, pItem, 2);
            }
            break;
        case WINDOWS_COMP3:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_COM, pItem, 3);
            }
            break;
        case WINDOWS_DISTill:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_DISTILL, pItem, nNum);
            }
            break;
        case WINDOWS_FORG:
            g_DebugLog("[CLIENT FORGE] Craft button clicked!");
            if (g_pCoreShell) {
                g_DebugLog("[CLIENT FORGE] g_pCoreShell is valid");
                // Call server-side Lua script to handle crafting
                if (g_pCoreShell->GetLixian()) {
                    g_DebugLog("[CLIENT FORGE] GetLixian() = TRUE, sending GOI_EXESCRIPT_BUTTON");
                    char szFunc[32];
                    sprintf(szFunc, "ExeCompoundForge");
                    g_DebugLog("[CLIENT FORGE] Function name: %s", szFunc);
                    g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
                    g_DebugLog("[CLIENT FORGE] OperationRequest sent successfully");
                } else {
                    g_DebugLog("[CLIENT FORGE] ERROR: GetLixian() = FALSE! Cannot execute script!");
                }
            } else {
                g_DebugLog("[CLIENT FORGE] ERROR: g_pCoreShell is NULL!");
            }
            break;
        case WINDOWS_ENCHASE:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_ENCHASE, pItem, nNum);
            }
            break;
        case WINDOWS_ATLAS:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_ALTAL, pItem, nNum);
            }
            break;
    }
}

void KUiComItem::Breathe() {
    // Call Breathe() on child pages to update their animations
    m_ForgePad.Breathe();
    m_CompoundPad.Breathe();
    // m_DistillPad and m_EnchasePad don't have custom Breathe() yet

// 	if(m_nStatus == STATUS_BEGIN_TREMBLE)
// 	{
// 		m_TrembleEffect.Show();
// 		m_TrembleEffect.SetFrame(0);
// 		m_nStatus = STATUS_TREMBLING;
// 	}
// 	else if(m_nStatus == STATUS_TREMBLING)
// 	{
// 		if(!PlayEffect())
// 		{
// 			m_nStatus = STATUS_CHANGING_ITEM;
// 			m_TrembleEffect.Hide();
// 		}
// 	}
// 	else if(m_nStatus == STATUS_CHANGING_ITEM)
// 	{
// 		m_nStatus = STATUS_FINISH;
// 	}
}

int KUiComItem::PlayEffect() {
// 	if(m_TrembleEffect.GetMaxFrame() == 0 ||
// 		m_TrembleEffect.GetMaxFrame() >= m_TrembleEffect.GetCurrentFrame() - 1)
// 	{
// 		m_TrembleEffect.SetFrame(0);
// 		return 0;
// 	}
// 	else
// 	{
// 		m_TrembleEffect.NextFrame();
// 		return 1;
// 	}
    return 0;
}

void KUiComItem::CleanItem() {
    m_CompoundPad.CleanItem();
    m_DistillPad.CleanItem();
    m_ForgePad.CleanItem();
    m_EnchasePad.CleanItem();
}

KUiCompound::KUiCompound() {

}

int KUiCompound::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    switch (uMsg) {
        case WND_N_SCORLLBAR_POS_CHANGED:
            if (uParam == (unsigned int) &m_ListScroll) {
                m_Guide.SetFirstShowLine(nParam);
            }
            break;
        case WND_N_ITEM_PICKDROP:
            {
                g_DebugLog("[COMPOUND] WND_N_ITEM_PICKDROP received");

                ITEM_PICKDROP_PLACE* pPickPos = (ITEM_PICKDROP_PLACE*)uParam;
                ITEM_PICKDROP_PLACE* pDropPos = (ITEM_PICKDROP_PLACE*)nParam;

                g_DebugLog("[COMPOUND] OnItemPickDrop START: pPickPos=%p, pDropPos=%p", pPickPos, pDropPos);

                KUiObjAtContRegion Drop, Pick;
                KUiDraggedObject Obj;
                KWndWindow* pWnd = NULL;

                if (pPickPos) {
                    ((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
                    Pick.Obj.uGenre = Obj.uGenre;
                    Pick.Obj.uId = Obj.uId;
                    Pick.Region.Width = Obj.DataW;
                    Pick.Region.Height = Obj.DataH;
                    Pick.Region.h = 0;
                    Pick.eContainer = UOC_COMPOUND;
                    pWnd = pPickPos->pWnd;
                    g_DebugLog("[COMPOUND] Pick: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                if (pDropPos) {
                    pWnd = pDropPos->pWnd;
                    Wnd_GetDragObj(&Obj);
                    Drop.Obj.uGenre = Obj.uGenre;
                    Drop.Obj.uId = Obj.uId;
                    Drop.Region.Width = Obj.DataW;
                    Drop.Region.Height = Obj.DataH;
                    Drop.Region.h = 0;
                    Drop.eContainer = UOC_COMPOUND;
                    g_DebugLog("[COMPOUND] Drop: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                // Map window pointer to Region.v (slot)
                if (pWnd == (KWndWindow*)&m_Box1) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM1;  // Slot 0
                    g_DebugLog("[COMPOUND] Mapped to Box1 (UIEP_BUILDITEM1 = %d)", UIEP_BUILDITEM1);
                } else if (pWnd == (KWndWindow*)&m_Box2) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[COMPOUND] Mapped to Box2 (UIEP_BUILDITEM2 = %d)", UIEP_BUILDITEM2);
                } else if (pWnd == (KWndWindow*)&m_Box3) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM3;  // Slot 2
                    g_DebugLog("[COMPOUND] Mapped to Box3 (UIEP_BUILDITEM3 = %d)", UIEP_BUILDITEM3);
                } else {
                    g_DebugLog("[COMPOUND] ERROR: pWnd doesn't match any box!");
                    break;
                }

                g_DebugLog("[COMPOUND] Calling OperationRequest GOI_SWITCH_OBJECT with Region.v=%d", Drop.Region.v);
                g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
                    pPickPos ? (unsigned int)&Pick : 0,
                    pDropPos ? (int)&Drop : 0);

                // CRITICAL FIX: Manually update the box visually after OperationRequest
                // The server stores the item but doesn't send notification back for empty->filled case
                Wnd_DragFinished();  // Clear drag state first
                g_DebugLog("[COMPOUND] Drag finished, now manually updating box");

                if (pDropPos && !pPickPos) {
                    // Dropping into empty slot - manually show the item
                    g_DebugLog("[COMPOUND] Manually updating box with item uId=%d", Obj.uId);
                    if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[COMPOUND] Box1.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[COMPOUND] Box2.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box3) {
                        m_Box3.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[COMPOUND] Box3.HoldObject called");
                    }
                } else if (pPickPos && !pDropPos) {
                    // Picking from box (removing item) - manually clear the box
                    g_DebugLog("[COMPOUND] Picking from box, clearing visual");
                    if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.Clear();
                        g_DebugLog("[COMPOUND] Box1.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.Clear();
                        g_DebugLog("[COMPOUND] Box2.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box3) {
                        m_Box3.Clear();
                        g_DebugLog("[COMPOUND] Box3.Clear called");
                    }
                }

                g_DebugLog("[COMPOUND] OnItemPickDrop END");
            }
            break;
        case WND_N_BUTTON_CLICK:
            if (uParam == (unsigned int) &m_Cancle) {
                CleanItem();
            } else if (uParam == (unsigned int) &m_Compound) {
                KUiComItem *pSelf = KUiComItem::GetIfVisible();

                KUiDraggedObject pObj;
                unsigned int pUP[3];

                m_Box1.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[0] = pObj.uId;
                } else {
                    return 1;
                }

                pObj.uId = 0;
                m_Box2.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[1] = pObj.uId;
                } else {
                    return 1;
                }

                pObj.uId = 0;
                m_Box3.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[2] = pObj.uId;
                } else {
                    return 1;
                }
                PlayEffect();
                pSelf->ComItem((unsigned int) (&pUP), m_nSelect, 3);
                CleanItem();
            }
            break;

        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiCompound::UpdateResult() {

}

void KUiCompound::Breathe() {
    if (m_nStatus == STATUS_BEGIN_TREMBLE) {
        m_TrembleEffect1.Show();
        m_TrembleEffect1.SetFrame(0);
        m_nStatus = STATUS_TREMBLING;
    } else if (m_nStatus == STATUS_TREMBLING) {
        if (!PlayEffect()) {
            m_nStatus = STATUS_CHANGING_ITEM;
            m_TrembleEffect1.Hide();
        }
    } else if (m_nStatus == STATUS_CHANGING_ITEM) {
        UpdateResult();
        m_nStatus = STATUS_FINISH;
    }
}

int KUiCompound::PlayEffect() {
    if (m_TrembleEffect1.GetMaxFrame() == 0 ||
        m_TrembleEffect1.GetMaxFrame() >= m_TrembleEffect1.GetCurrentFrame() - 1) {
        m_TrembleEffect1.SetFrame(0);
        return 0;
    } else {
        m_TrembleEffect1.NextFrame();
        return 1;
    }
}

void KUiCompound::PaintWindow() {
    KWndPage::PaintWindow();
}

void KUiCompound::LoadScheme(const char *pScheme) {
    char Buff[128], Buffer[64];
    KIniFile Ini;
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_COMPE);
    if (Ini.Load(Buff)) {
        KWndImage::Init(&Ini, "Main");
        m_Box1.Init(&Ini, "Ore1");
        m_Box2.Init(&Ini, "Ore3");
        m_Box3.Init(&Ini, "Ore2");
        m_Compound.Init(&Ini, "CompoundBtn");
        m_Cancle.Init(&Ini, "CancleBtn");
        m_Guide.Init(&Ini, "GuideList");
        m_ListScroll.Init(&Ini, "GuideList_Scroll");
        m_TrembleEffect1.Init(&Ini, "Effect_0");
        m_TrembleEffect2.Init(&Ini, "Effect_1");
        m_TrembleEffect3.Init(&Ini, "Effect_2");
        //m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");

        int nX, nY, nColor;
        Ini.GetInteger2("Box_0", "Pos", &nX, &nY);

        if (Ini.GetString("TextColor", "Font", "", Buffer, sizeof(Buffer))) {
            nColor = (::GetColor(Buffer) & 0xFFFFFF);
        }
        m_nSelect = 0;
        m_Pos1.SetPosition(nX - 14, nY - 4);
        m_Pos1.SetText("Nh�n");
        m_Pos1.SetTextColor(nColor);
        //m_Pos1.BringToTop();

        Ini.GetInteger2("Box_1", "Pos", &nX, &nY);

        m_Pos2.SetPosition(nX - 14, nY - 4);
        m_Pos2.SetText("D�y chuy�n/h� th�n ph�");
        m_Pos2.SetTextColor(nColor);
        //m_Pos2.BringToTop();

        Ini.GetInteger2("Box_2", "Pos", &nX, &nY);

        m_Pos3.SetPosition(nX - 14, nY - 4);
        m_Pos3.SetText("Ng�c b�i/h��ng nang");
        m_Pos3.SetTextColor(nColor);
        //m_Pos3.BringToTop();

        // 		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
        // 		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
    }
}

void KUiCompound::Initialize() {
    AddChild(&m_Pos1);
    AddChild(&m_Pos2);
    AddChild(&m_Pos3);

    // Setup object boxes BEFORE AddChild - matching UiTrembleItem pattern
    m_Box1.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box1);
    m_Box1.SetContainerId((int)UOC_COMPOUND);

    m_Box2.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box2);
    m_Box2.SetContainerId((int)UOC_COMPOUND);

    m_Box3.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box3);
    m_Box3.SetContainerId((int)UOC_COMPOUND);

    AddChild(&m_Compound);
    AddChild(&m_Cancle);
    AddChild(&m_TrembleEffect1);
    AddChild(&m_TrembleEffect2);
    AddChild(&m_TrembleEffect3);
    AddChild(&m_Guide);
    AddChild(&m_ListScroll);
//	AddChild(&m_ListBtn);


    m_Guide.SetScrollbar(&m_ListScroll);


    char Scheme[256];
    g_UiBase.GetCurSchemePath(Scheme, 256);
    LoadScheme(Scheme);

    char Buff[128];
    KIniFile Ini;

    sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
    if (Ini.Load(Buff)) {
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "Compound", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));

        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "CompoundRule", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));
    }

    Wnd_AddWindow(this);
}

void KUiCompound::UpdateItem(KUiDraggedObject *pItem, int bAdd) {
    if (!pItem) return;

    // Map Region.v to box: 0=Box1, 1=Box2, 2=Box3
    if (pItem->DataY == 0) { // Box1
        if (bAdd) {
            m_Box1.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box1.Clear();
        }
    } else if (pItem->DataY == 1) { // Box2
        if (bAdd) {
            m_Box2.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box2.Clear();
        }
    } else if (pItem->DataY == 2) { // Box3
        if (bAdd) {
            m_Box3.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box3.Clear();
        }
    }
}

void KUiCompound::SetPosText(int i) {
    switch (i) {
        case 1:
            m_nSelect = 0;
            m_Pos1.SetText("Nh�n");
            m_Pos2.SetText("D�y chuy�n/h� th�n ph�");
            m_Pos3.SetText("Ng�c b�i/h��ng nang");
            break;
        case 2:
            m_nSelect = 1;
            m_Pos1.SetText("Huy�n tinh 1");
            m_Pos2.SetText("Huy�n tinh 2");
            m_Pos3.SetText("Huy�n tinh 3");
            break;
        case 3:
            m_nSelect = 2;
            m_Pos1.SetText("Kho�ng th�ch 1");
            m_Pos2.SetText("Kho�ng th�ch 2");
            m_Pos3.SetText("Kho�ng th�ch 3");
            break;
    }
}

void KUiCompound::CleanItem() {
    m_Box1.Clear();
    m_Box2.Clear();
    m_Box3.Clear();
}

KUiDistill::KUiDistill() {

}

void KUiDistill::PaintWindow() {
    KWndPage::PaintWindow();
}

int KUiDistill::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    switch (uMsg) {
        case WND_N_SCORLLBAR_POS_CHANGED:
            if (uParam == (unsigned int) &m_ListScroll) {
                m_Guide.SetFirstShowLine(nParam);
            }
            break;
        case WND_N_ITEM_PICKDROP:
            {
                g_DebugLog("[DISTILL] WND_N_ITEM_PICKDROP received");

                ITEM_PICKDROP_PLACE* pPickPos = (ITEM_PICKDROP_PLACE*)uParam;
                ITEM_PICKDROP_PLACE* pDropPos = (ITEM_PICKDROP_PLACE*)nParam;

                KUiObjAtContRegion Drop, Pick;
                KUiDraggedObject Obj;
                KWndWindow* pWnd = NULL;

                if (pPickPos) {
                    ((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
                    Pick.Obj.uGenre = Obj.uGenre;
                    Pick.Obj.uId = Obj.uId;
                    Pick.Region.Width = Obj.DataW;
                    Pick.Region.Height = Obj.DataH;
                    Pick.Region.h = 0;
                    Pick.eContainer = UOC_COMPOUND;
                    pWnd = pPickPos->pWnd;
                }

                if (pDropPos) {
                    pWnd = pDropPos->pWnd;
                    Wnd_GetDragObj(&Obj);
                    Drop.Obj.uGenre = Obj.uGenre;
                    Drop.Obj.uId = Obj.uId;
                    Drop.Region.Width = Obj.DataW;
                    Drop.Region.Height = Obj.DataH;
                    Drop.Region.h = 0;
                    Drop.eContainer = UOC_COMPOUND;
                }

                // Map window to Region.v (slot) - Distill has BigBox, Box1, Box2
                if (pWnd == (KWndWindow*)&m_BigBox) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM1;  // Slot 0
                    g_DebugLog("[DISTILL] Mapped to BigBox (slot 0)");
                } else if (pWnd == (KWndWindow*)&m_Box1) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[DISTILL] Mapped to Box1 (slot 1)");
                } else if (pWnd == (KWndWindow*)&m_Box2) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM3;  // Slot 2
                    g_DebugLog("[DISTILL] Mapped to Box2 (slot 2)");
                } else {
                    g_DebugLog("[DISTILL] ERROR: Unknown window, aborting");
                    return 0;
                }

                g_DebugLog("[DISTILL] Calling GOI_SWITCH_OBJECT");
                g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
                    pPickPos ? (unsigned int)&Pick : 0,
                    pDropPos ? (int)&Drop : 0);

                // CRITICAL FIX: Manually update the box visually after OperationRequest
                // The server stores the item but doesn't send notification back for empty->filled case
                Wnd_DragFinished();  // Clear drag state first
                g_DebugLog("[DISTILL] Drag finished, now manually updating box");

                if (pDropPos && !pPickPos) {
                    // Dropping into empty slot - manually show the item
                    g_DebugLog("[DISTILL] Manually updating box with item uId=%d", Obj.uId);
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[DISTILL] BigBox.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[DISTILL] Box1.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[DISTILL] Box2.HoldObject called");
                    }
                } else if (pPickPos && !pDropPos) {
                    // Picking from box (removing item) - manually clear the box
                    g_DebugLog("[DISTILL] Picking from box, clearing visual");
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.Clear();
                        g_DebugLog("[DISTILL] BigBox.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.Clear();
                        g_DebugLog("[DISTILL] Box1.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.Clear();
                        g_DebugLog("[DISTILL] Box2.Clear called");
                    }
                }

                g_DebugLog("[DISTILL] OnItemPickDrop END");
            }
            break;
        case WND_N_BUTTON_CLICK:
            if (uParam == (unsigned int) &m_Cancle) {
                CleanItem();
            } else if (uParam == (unsigned int) &m_Distill) {
                int nNum = 0;

                KUiDraggedObject pObj;
                unsigned int pUP[11];

                m_BigBox.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 1;
                }

                pObj.uId = 0;
                m_Box1.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 1;
                }

                pObj.uId = 0;
                m_Box2.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 1;
                }

                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 4; j++) {
                        pObj.uId = 0;
                        m_ItemBox.GetObject(pObj, i, j);
                        if (pObj.uId > 0) {
                            pUP[nNum] = pObj.uId;
                            nNum++;
                        } else {
                            break;
                        }
                    }
                }

                KUiComItem *pSelf = KUiComItem::GetIfVisible();
                pSelf->ComItem((unsigned int) (&pUP), 3, nNum);

                CleanItem();
            }
            break;

        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiDistill::LoadScheme(const char *pScheme) {
    char Buff[128], Buffer[64];
    KIniFile Ini;
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_OUTIN);
    if (Ini.Load(Buff)) {
        KWndImage::Init(&Ini, "Main");
        m_BigBox.Init(&Ini, "BigBox");
        m_Box1.Init(&Ini, "SmallBox1");
        m_Box2.Init(&Ini, "SmallBox2");
        m_ItemBox.Init(&Ini, "ItemBox");
        m_Distill.Init(&Ini, "DistillBtn");
        m_Cancle.Init(&Ini, "CancleBtn");
        m_Guide.Init(&Ini, "GuideList");
        m_ListScroll.Init(&Ini, "GuideList_Scroll");
        //m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");
        m_ItemBox.EnableTracePutPos(FALSE);

        int nX, nY, nColor;
        Ini.GetInteger2("EquipPos", "Pos", &nX, &nY);
        if (Ini.GetString("TextColor", "Font", "", Buffer, sizeof(Buffer))) {
            nColor = (::GetColor(Buffer) & 0xFFFFFF);
        }

        m_Pos1.SetPosition(nX - 14, nY - 4);
        m_Pos1.SetTextColor(nColor);
        m_Pos1.BringToTop();
        m_Pos1.SetText("Trang b� xanh/tr�ng");

        Ini.GetInteger2("CryolitePos", "Pos", &nX, &nY);
        m_Pos2.SetPosition(nX - 12, nY - 4);
        m_Pos2.SetTextColor(nColor);
        m_Pos2.BringToTop();
        m_Pos2.SetText("Huy�n tinh");

        Ini.GetInteger2("PropMinePos", "Pos", &nX, &nY);
        m_Pos3.SetPosition(nX - 12, nY - 4);
        m_Pos3.SetTextColor(nColor);
        m_Pos3.BringToTop();
        m_Pos3.SetText("Nguy�n kho�ng");

        Ini.GetInteger2("ConsumePos", "Pos", &nX, &nY);
        m_Pos4.SetPosition(nX - 14, nY - 4);
        m_Pos4.SetTextColor(nColor);
        m_Pos4.BringToTop();
        m_Pos4.SetText("C� th� ch�n nguy�n li�u");
        // 		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
        // 		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
    }
}

void KUiDistill::Initialize() {
    // Setup object boxes BEFORE AddChild - matching UiTrembleItem pattern
    m_BigBox.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_BigBox);
    m_BigBox.SetContainerId((int)UOC_COMPOUND);

    m_Box1.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box1);
    m_Box1.SetContainerId((int)UOC_COMPOUND);

    m_Box2.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box2);
    m_Box2.SetContainerId((int)UOC_COMPOUND);

    AddChild(&m_ItemBox);
    m_ItemBox.SetContainerId((int)UOC_COMPOUND_BOX);

    AddChild(&m_Distill);
    AddChild(&m_Cancle);

    AddChild(&m_Guide);
    AddChild(&m_ListScroll);
    //AddChild(&m_ListBtn);

    AddChild(&m_Pos1);
    AddChild(&m_Pos2);
    AddChild(&m_Pos3);
    AddChild(&m_Pos4);
    m_Guide.SetScrollbar(&m_ListScroll);


    char Scheme[256];
    g_UiBase.GetCurSchemePath(Scheme, 256);
    LoadScheme(Scheme);

    char Buff[128];
    KIniFile Ini;

    sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
    if (Ini.Load(Buff)) {
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "Distill", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));

        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "DistillRule", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));
    }

    Wnd_AddWindow(this);
}

void KUiDistill::UpdateItem(KUiDraggedObject *pItem, int bAdd) {
    if (!pItem) return;

    // Map Region.v to box: 0=BigBox, 1=Box1, 2=Box2
    // Matrix box (m_ItemBox) uses different container UOC_COMPOUND_BOX
    if (pItem->DataY == 0) { // BigBox
        if (bAdd) {
            m_BigBox.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_BigBox.Clear();
        }
    } else if (pItem->DataY == 1) { // Box1
        if (bAdd) {
            m_Box1.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box1.Clear();
        }
    } else if (pItem->DataY == 2) { // Box2
        if (bAdd) {
            m_Box2.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box2.Clear();
        }
    }
}

void KUiDistill::CleanItem() {
    m_BigBox.Clear();
    m_Box1.Clear();
    m_Box2.Clear();
    m_ItemBox.Clear();
}

KUiForge::KUiForge() {
    m_EffectTime = 0;
}

void KUiForge::PaintWindow() {
    KWndPage::PaintWindow();
}

// Animation frame update - called every frame
void KUiForge::Breathe() {
    if (m_TrembleEffect1.IsVisible())
        m_TrembleEffect1.NextFrame();
    if (m_EffectTime)
        m_EffectTime++;
    // LOOP is defined in UiTrembleItem, use 4 as default
    #ifndef LOOP
    #define LOOP 4
    #endif
    if (m_EffectTime == (m_TrembleEffect1.GetMaxFrame())*(LOOP*2)/2 + 1) {
        StopEffect();
        m_EffectTime = 0;
    }
}

// Start crafting effect animation
void KUiForge::StartEffect() {
    m_TrembleEffect1.Show();
    m_EffectTime = 1;
    // Disable item picking during animation
    m_BigBox.EnablePickPut(false);
    m_SmallBox.EnablePickPut(false);
    g_DebugLog("[FORGE EFFECT] Animation started");
}

// Stop effect and update items
void KUiForge::StopEffect() {
    m_TrembleEffect1.Hide();
    m_EffectTime = 0;
    // Re-enable item picking
    m_BigBox.EnablePickPut(true);
    m_SmallBox.EnablePickPut(true);
    // Refresh items from server
    UpdateData();
    g_DebugLog("[FORGE EFFECT] Animation stopped, items refreshed");
}

// Check if effect is running
BOOL KUiForge::IsEffect() {
    return (m_EffectTime > 0) ? TRUE : FALSE;
}

int KUiForge::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    switch (uMsg) {
        case WND_N_SCORLLBAR_POS_CHANGED:
            if (uParam == (unsigned int) &m_ListScroll) {
                m_Guide.SetFirstShowLine(nParam);
            }
            break;
        case WND_N_ITEM_PICKDROP:
            {
                g_DebugLog("[FORGE] WND_N_ITEM_PICKDROP received");

                ITEM_PICKDROP_PLACE* pPickPos = (ITEM_PICKDROP_PLACE*)uParam;
                ITEM_PICKDROP_PLACE* pDropPos = (ITEM_PICKDROP_PLACE*)nParam;

                g_DebugLog("[FORGE] OnItemPickDrop START: pPickPos=%p, pDropPos=%p", pPickPos, pDropPos);

                KUiObjAtContRegion Drop, Pick;
                KUiDraggedObject Obj;
                KWndWindow* pWnd = NULL;

                if (pPickPos) {
                    ((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
                    Pick.Obj.uGenre = Obj.uGenre;
                    Pick.Obj.uId = Obj.uId;
                    Pick.Region.Width = Obj.DataW;
                    Pick.Region.Height = Obj.DataH;
                    Pick.Region.h = 0;
                    Pick.eContainer = UOC_COMPOUND;
                    pWnd = pPickPos->pWnd;
                    g_DebugLog("[FORGE] Pick: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                if (pDropPos) {
                    pWnd = pDropPos->pWnd;
                    Wnd_GetDragObj(&Obj);
                    Drop.Obj.uGenre = Obj.uGenre;
                    Drop.Obj.uId = Obj.uId;
                    Drop.Region.Width = Obj.DataW;
                    Drop.Region.Height = Obj.DataH;
                    Drop.Region.h = 0;
                    Drop.eContainer = UOC_COMPOUND;
                    g_DebugLog("[FORGE] Drop: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                // Map window pointer to Region.v (slot) + VALIDATE item type
                if (pWnd == (KWndWindow*)&m_BigBox) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM1;  // Slot 0
                    g_DebugLog("[FORGE] Mapped to BigBox (UIEP_BUILDITEM1 = %d)", UIEP_BUILDITEM1);

                    // VALIDATION: BigBox only accepts equipment (NOT ring, amulet, pendant, horse, mask)
                    if (pDropPos) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be equipment (blue/purple/gold/platinum)
                        if (nGenre != item_equip && nGenre != item_purpleequip &&
                            nGenre != item_goldequip && nGenre != item_platinaequip) {
                            g_DebugLog("[FORGE] REJECT BigBox: Genre %d is not equipment", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat trang bi vao o nay!", 256);
                            break;
                        }

                        // Reject: ring, amulet, pendant, horse, mask
                        if (nDetail == equip_ring || nDetail == equip_amulet ||
                            nDetail == equip_pendant || nDetail == equip_horse || nDetail == equip_mask) {
                            g_DebugLog("[FORGE] REJECT BigBox: Detail %d not allowed (ring/amulet/pendant/horse/mask)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Khong duoc dat nhan, ngoc boi, day chuyen, ngua, mat na vao day!", 256);
                            break;
                        }

                        g_DebugLog("[FORGE] BigBox validation PASSED: genre=%d, detail=%d", nGenre, nDetail);
                    }

                } else if (pWnd == (KWndWindow*)&m_SmallBox) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[FORGE] Mapped to SmallBox (UIEP_BUILDITEM2 = %d)", UIEP_BUILDITEM2);

                    // VALIDATION: SmallBox only accepts Huyen Tinh crystals (item_task, detail 74-79)
                    if (pDropPos) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be item_task genre (Huyen Tinh crystals are genre 6)
                        if (nGenre != item_task) {
                            g_DebugLog("[FORGE] REJECT SmallBox: Genre %d is not item_task", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat Huyen Tinh vao o nay!", 256);
                            break;
                        }

                        // Optional: Check if it's specifically Huyen Tinh (detail 74-79)
                        if (nDetail < 74 || nDetail > 79) {
                            g_DebugLog("[FORGE] REJECT SmallBox: Detail %d is not Huyen Tinh (74-79)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat Huyen Tinh vao o nay!", 256);
                            break;
                        }

                        g_DebugLog("[FORGE] SmallBox validation PASSED: genre=%d (item_task), detail=%d", nGenre, nDetail);
                    }

                } else {
                    g_DebugLog("[FORGE] ERROR: pWnd doesn't match any box!");
                    break;
                }

                g_DebugLog("[FORGE] Calling OperationRequest GOI_SWITCH_OBJECT with Region.v=%d", Drop.Region.v);
                g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
                    pPickPos ? (unsigned int)&Pick : 0,
                    pDropPos ? (int)&Drop : 0);

                // CRITICAL FIX: Manually update the box visually after OperationRequest
                // The server stores the item but doesn't send notification back for empty->filled case
                Wnd_DragFinished();  // Clear drag state first
                g_DebugLog("[FORGE] Drag finished, now manually updating box");

                if (pDropPos && !pPickPos) {
                    // Dropping into empty slot - manually show the item
                    g_DebugLog("[FORGE] Manually updating box with item uId=%d", Obj.uId);
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[FORGE] BigBox.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_SmallBox) {
                        m_SmallBox.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[FORGE] SmallBox.HoldObject called");
                    }
                } else if (pPickPos && !pDropPos) {
                    // Picking from box (removing item) - manually clear the box
                    g_DebugLog("[FORGE] Picking from box, clearing visual");
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.Clear();
                        g_DebugLog("[FORGE] BigBox.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_SmallBox) {
                        m_SmallBox.Clear();
                        g_DebugLog("[FORGE] SmallBox.Clear called");
                    }
                }

                g_DebugLog("[FORGE] OnItemPickDrop END");
            }
            break;
        case WND_N_BUTTON_CLICK:
            if (uParam == (unsigned int) &m_Cancle) {
                CleanItem();
            } else if (uParam == (unsigned int) &m_ForgeBtn) {
                g_DebugLog("[FORGE] Forge button clicked - starting effect");

                // Don't allow crafting if animation is running
                if (IsEffect()) {
                    g_DebugLog("[FORGE] Effect is running, ignoring click");
                    return 1;
                }

                // Start animation effect
                StartEffect();

                // Validate items before sending to server
                int nNum = 0;
                KUiDraggedObject pObj;
                unsigned int pUP[2];

                m_BigBox.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    g_DebugLog("[FORGE] No item in BigBox, stopping effect");
                    StopEffect();
                    return 1;
                }

                pObj.uId = 0;
                m_SmallBox.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    g_DebugLog("[FORGE] No item in SmallBox, stopping effect");
                    StopEffect();
                    return 1;
                }

                // Send craft request to server via Lua script
                if (g_pCoreShell && g_pCoreShell->GetLixian()) {
                    g_DebugLog("[FORGE] Sending ExeCompoundForge to server, animation playing");
                    char szFunc[32];
                    sprintf(szFunc, "ExeCompoundForge");
                    g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
                } else {
                    g_DebugLog("[FORGE] ERROR: Cannot execute script, stopping effect");
                    StopEffect();
                }

                // Items will be cleared by server response or StopEffect()
                // Don't CleanItem() here - let the animation finish first
            }
            break;
        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiForge::LoadScheme(const char *pScheme) {
    char Buff[128];
    KIniFile Ini;
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_BUILD);
    if (Ini.Load(Buff)) {
        KWndImage::Init(&Ini, "Main");
        m_BigBox.Init(&Ini, "BigBox");
        m_SmallBox.Init(&Ini, "SmallBox");
        m_ForgeBtn.Init(&Ini, "ForgeBtn");
        m_Cancle.Init(&Ini, "CancleBtn");
        m_Guide.Init(&Ini, "GuideList");
        m_ListScroll.Init(&Ini, "GuideList_Scroll");
        //m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");

        int nX, nY, nColor;
        Ini.GetInteger2("EquipPos", "Pos", &nX, &nY);
        if (Ini.GetString("TextColor", "Font", "", Buff, sizeof(Buff))) {
            nColor = (::GetColor(Buff) & 0xFFFFFF);
        }

        m_Pos1.SetPosition(nX - 14, nY - 4);
        m_Pos1.SetTextColor(nColor);
        m_Pos1.BringToTop();
        m_Pos1.SetText("Trang b� xanh/tr�ng");

        Ini.GetInteger2("CryolitePos", "Pos", &nX, &nY);
        m_Pos2.SetPosition(nX - 12, nY - 4);
        m_Pos2.SetTextColor(nColor);
        m_Pos2.BringToTop();
        m_Pos2.SetText("Huy�n tinh");

        // Initialize effect animation sprite - must match INI section name [EquipEffect]
        m_TrembleEffect1.Init(&Ini, "EquipEffect");
        m_TrembleEffect1.Hide();  // Hide by default
        m_EffectTime = 0;  // Reset effect timer
        g_DebugLog("[FORGE] Effect initialized from ini section [EquipEffect], hidden and timer reset");

    }
}

void KUiForge::Initialize() {
    g_DebugLog("[FORGE] Initialize() START");

    // Setup object boxes BEFORE AddChild - matching UiTrembleItem pattern
    m_BigBox.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_BigBox);
    m_BigBox.SetContainerId((int)UOC_COMPOUND);
    g_DebugLog("[FORGE] BigBox initialized: container=UOC_COMPOUND(%d)", UOC_COMPOUND);

    m_SmallBox.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_SmallBox);
    m_SmallBox.SetContainerId((int)UOC_COMPOUND);
    g_DebugLog("[FORGE] SmallBox initialized: container=UOC_COMPOUND(%d)", UOC_COMPOUND);

    AddChild(&m_ForgeBtn);
    AddChild(&m_Cancle);

    AddChild(&m_Guide);
    AddChild(&m_ListScroll);
    //AddChild(&m_ListBtn);

    AddChild(&m_Pos1);
    AddChild(&m_Pos2);
    m_Guide.SetScrollbar(&m_ListScroll);

    // Add effect sprite as child (will be initialized and hidden in LoadScheme)
    AddChild(&m_TrembleEffect1);
    g_DebugLog("[FORGE] Effect sprite added as child");

    char Scheme[256];
    g_UiBase.GetCurSchemePath(Scheme, 256);
    LoadScheme(Scheme);

    char Buff[128];
    KIniFile Ini;

    sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
    if (Ini.Load(Buff)) {
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "Forge", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));

        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "ForgeRule", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));
    }

    Wnd_AddWindow(this);
}

void KUiForge::UpdateData() {
    g_DebugLog("[FORGE] UpdateData called - loading items from server");

    // Request build items from server (same as TrembleItem pattern)
    KUiObjAtRegion Items[MAX_PART_BUILD];
    int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Items, 0);

    g_DebugLog("[FORGE] UpdateData: Got %d items from server", nCount);

    // Clear all boxes first
    m_BigBox.Clear();
    m_SmallBox.Clear();

    // Update boxes with items from server
    for (int i = 0; i < nCount; i++) {
        if (Items[i].Obj.uGenre != CGOG_NOTHING) {
            g_DebugLog("[FORGE] UpdateData: Item[%d] Region.v=%d, genre=%d, id=%d",
                i, Items[i].Region.v, Items[i].Obj.uGenre, Items[i].Obj.uId);

            // Map Region.v to boxes: UIEP_BUILDITEM1 (0) = BigBox, UIEP_BUILDITEM2 (1) = SmallBox
            if (Items[i].Region.v == UIEP_BUILDITEM1) { // Slot 0 = BigBox
                m_BigBox.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
                g_DebugLog("[FORGE] UpdateData: Loaded item into BigBox");
            } else if (Items[i].Region.v == UIEP_BUILDITEM2) { // Slot 1 = SmallBox
                m_SmallBox.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
                g_DebugLog("[FORGE] UpdateData: Loaded item into SmallBox");
            }
        }
    }

    g_DebugLog("[FORGE] UpdateData complete");
}

void KUiForge::UpdateItem(KUiDraggedObject *pItem, int bAdd) {
    g_DebugLog("[FORGE] UpdateItem called: pItem=%p, bAdd=%d", pItem, bAdd);
    if (!pItem) {
        g_DebugLog("[FORGE] UpdateItem: pItem is NULL, returning");
        return;
    }

    g_DebugLog("[FORGE] UpdateItem: genre=%d, id=%d, DataX=%d, DataY=%d (Region.v), DataW=%d, DataH=%d",
        pItem->uGenre, pItem->uId, pItem->DataX, pItem->DataY, pItem->DataW, pItem->DataH);

    // Map Region.v to box: 0=BigBox, 1=SmallBox
    if (pItem->DataY == 0) { // BigBox
        g_DebugLog("[FORGE] Updating BigBox (Region.v=0)");
        if (bAdd) {
            m_BigBox.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
            g_DebugLog("[FORGE] BigBox.HoldObject called");
        } else {
            m_BigBox.Clear();
            g_DebugLog("[FORGE] BigBox.Clear called");
        }
    } else if (pItem->DataY == 1) { // SmallBox
        g_DebugLog("[FORGE] Updating SmallBox (Region.v=1)");
        if (bAdd) {
            m_SmallBox.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
            g_DebugLog("[FORGE] SmallBox.HoldObject called");
        } else {
            m_SmallBox.Clear();
            g_DebugLog("[FORGE] SmallBox.Clear called");
        }
    } else {
        g_DebugLog("[FORGE] WARNING: Unknown Region.v=%d, not mapped to any box!", pItem->DataY);
    }
}

void KUiForge::CleanItem() {
    m_BigBox.Clear();
    m_SmallBox.Clear();
}

KUiEnchaseTim::KUiEnchaseTim() {

}

void KUiEnchaseTim::PaintWindow() {
    KWndPage::PaintWindow();
}

int KUiEnchaseTim::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    switch (uMsg) {
        case WND_N_SCORLLBAR_POS_CHANGED:
            if (uParam == (unsigned int) &m_ListScroll) {
                m_Guide.SetFirstShowLine(nParam);
            }
            break;
        case WND_N_ITEM_PICKDROP:
            {
                g_DebugLog("[ENCHASE] WND_N_ITEM_PICKDROP received");

                ITEM_PICKDROP_PLACE* pPickPos = (ITEM_PICKDROP_PLACE*)uParam;
                ITEM_PICKDROP_PLACE* pDropPos = (ITEM_PICKDROP_PLACE*)nParam;

                g_DebugLog("[ENCHASE] OnItemPickDrop START: pPickPos=%p, pDropPos=%p", pPickPos, pDropPos);

                KUiObjAtContRegion Drop, Pick;
                KUiDraggedObject Obj;
                KWndWindow* pWnd = NULL;

                if (pPickPos) {
                    ((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
                    Pick.Obj.uGenre = Obj.uGenre;
                    Pick.Obj.uId = Obj.uId;
                    Pick.Region.Width = Obj.DataW;
                    Pick.Region.Height = Obj.DataH;
                    Pick.Region.h = 0;
                    Pick.eContainer = UOC_COMPOUND;
                    pWnd = pPickPos->pWnd;
                    g_DebugLog("[ENCHASE] Pick: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                if (pDropPos) {
                    pWnd = pDropPos->pWnd;
                    Wnd_GetDragObj(&Obj);
                    Drop.Obj.uGenre = Obj.uGenre;
                    Drop.Obj.uId = Obj.uId;
                    Drop.Region.Width = Obj.DataW;
                    Drop.Region.Height = Obj.DataH;
                    Drop.Region.h = 0;
                    Drop.eContainer = UOC_COMPOUND;
                    g_DebugLog("[ENCHASE] Drop: uId=%d, uGenre=%d", Obj.uId, Obj.uGenre);
                }

                // Map window pointer to Region.v (slot)
                if (pWnd == (KWndWindow*)&m_BigBox) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM1;  // Slot 0
                    g_DebugLog("[ENCHASE] Mapped to BigBox (UIEP_BUILDITEM1 = %d)", UIEP_BUILDITEM1);
                } else if (pWnd == (KWndWindow*)&m_Box1) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[ENCHASE] Mapped to Box1 (UIEP_BUILDITEM2 = %d)", UIEP_BUILDITEM2);
                } else if (pWnd == (KWndWindow*)&m_Box2) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM3;  // Slot 2
                    g_DebugLog("[ENCHASE] Mapped to Box2 (UIEP_BUILDITEM3 = %d)", UIEP_BUILDITEM3);
                } else {
                    g_DebugLog("[ENCHASE] ERROR: pWnd doesn't match any box!");
                    break;
                }

                g_DebugLog("[ENCHASE] Calling OperationRequest GOI_SWITCH_OBJECT with Region.v=%d", Drop.Region.v);
                g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
                    pPickPos ? (unsigned int)&Pick : 0,
                    pDropPos ? (int)&Drop : 0);

                // CRITICAL FIX: Manually update the box visually after OperationRequest
                // The server stores the item but doesn't send notification back for empty->filled case
                Wnd_DragFinished();  // Clear drag state first
                g_DebugLog("[ENCHASE] Drag finished, now manually updating box");

                if (pDropPos && !pPickPos) {
                    // Dropping into empty slot - manually show the item
                    g_DebugLog("[ENCHASE] Manually updating box with item uId=%d", Obj.uId);
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[ENCHASE] BigBox.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[ENCHASE] Box1.HoldObject called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.HoldObject(Obj.uGenre, Obj.uId, Obj.DataW, Obj.DataH);
                        g_DebugLog("[ENCHASE] Box2.HoldObject called");
                    }
                } else if (pPickPos && !pDropPos) {
                    // Picking from box (removing item) - manually clear the box
                    g_DebugLog("[ENCHASE] Picking from box, clearing visual");
                    if (pWnd == (KWndWindow*)&m_BigBox) {
                        m_BigBox.Clear();
                        g_DebugLog("[ENCHASE] BigBox.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box1) {
                        m_Box1.Clear();
                        g_DebugLog("[ENCHASE] Box1.Clear called");
                    } else if (pWnd == (KWndWindow*)&m_Box2) {
                        m_Box2.Clear();
                        g_DebugLog("[ENCHASE] Box2.Clear called");
                    }
                }

                g_DebugLog("[ENCHASE] OnItemPickDrop END");
            }
            break;
        case WND_N_BUTTON_CLICK:
            if (uParam == (unsigned int) &m_Cancle) {
                CleanItem();
            } else if (uParam == (unsigned int) &m_Distill) {
                int nNum = 0;

                KUiDraggedObject pObj;
                unsigned int pUP[11];

                m_BigBox.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 0;
                }

                pObj.uId = 0;
                m_Box1.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 0;
                }

                pObj.uId = 0;
                m_Box2.GetObject(pObj);
                if (pObj.uId > 0) {
                    pUP[nNum] = pObj.uId;
                    nNum++;
                } else {
                    return 0;
                }

                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 4; j++) {
                        pObj.uId = 0;
                        m_ItemBox.GetObject(pObj, i, j);
                        if (pObj.uId > 0) {
                            pUP[nNum] = pObj.uId;
                            nNum++;
                        } else {
                            break;
                        }
                    }
                }

                KUiComItem *pSelf = KUiComItem::GetIfVisible();
                pSelf->ComItem((unsigned int) (&pUP), 5, nNum);

                CleanItem();
            }
            break;
        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiEnchaseTim::LoadScheme(const char *pScheme) {
    char Buff[128];
    KIniFile Ini;
//	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_INOUT);
    if (Ini.Load(Buff)) {
        KWndImage::Init(&Ini, "Main");
        m_BigBox.Init(&Ini, "BigBox");
        m_Box1.Init(&Ini, "SmallBox1");
        m_Box2.Init(&Ini, "SmallBox2");
        m_ItemBox.Init(&Ini, "ItemBox");
        m_Distill.Init(&Ini, "DistillBtn");
        m_Cancle.Init(&Ini, "CancleBtn");
        m_Guide.Init(&Ini, "GuideList");
        m_ListScroll.Init(&Ini, "GuideList_Scroll");
        //m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");
        m_ItemBox.EnableTracePutPos(FALSE);


        int nX, nY, nColor;
        Ini.GetInteger2("EquipPos", "Pos", &nX, &nY);
        if (Ini.GetString("TextColor", "Font", "", Buff, sizeof(Buff))) {
            nColor = (::GetColor(Buff) & 0xFFFFFF);
        }

        m_Pos1.SetPosition(nX - 14, nY - 4);
        m_Pos1.SetTextColor(nColor);
        m_Pos1.BringToTop();
        m_Pos1.SetText("Trang b� huy�n tinh");

        Ini.GetInteger2("CryolitePos", "Pos", &nX, &nY);
        m_Pos2.SetPosition(nX - 12, nY - 4);
        m_Pos2.SetTextColor(nColor);
        m_Pos2.BringToTop();
        m_Pos2.SetText("Huy�n tinh");

        Ini.GetInteger2("PropMinePos", "Pos", &nX, &nY);
        m_Pos3.SetPosition(nX - 12, nY - 4);
        m_Pos3.SetTextColor(nColor);
        m_Pos3.BringToTop();
        m_Pos3.SetText("Kho�ng th�ch");

        Ini.GetInteger2("ConsumePos", "Pos", &nX, &nY);
        m_Pos4.SetPosition(nX - 14, nY - 4);
        m_Pos4.SetTextColor(nColor);
        m_Pos4.BringToTop();
        m_Pos4.SetText("C� th� ch�n nguy�n li�u");
        // 		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
        // 		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
    }
}

void KUiEnchaseTim::Initialize() {
    // Setup object boxes BEFORE AddChild - matching UiTrembleItem pattern
    m_BigBox.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_BigBox);
    m_BigBox.SetContainerId((int)UOC_COMPOUND);

    m_Box1.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box1);
    m_Box1.SetContainerId((int)UOC_COMPOUND);

    m_Box2.SetObjectGenre(CGOG_ITEM);
    AddChild(&m_Box2);
    m_Box2.SetContainerId((int)UOC_COMPOUND);

    AddChild(&m_ItemBox);
    m_ItemBox.SetContainerId((int)UOC_COMPOUND_BOX);

    AddChild(&m_Distill);
    AddChild(&m_Cancle);

    AddChild(&m_Guide);
    AddChild(&m_ListScroll);
    //AddChild(&m_ListBtn);

    AddChild(&m_Pos1);
    AddChild(&m_Pos2);
    AddChild(&m_Pos3);
    AddChild(&m_Pos4);
    m_Guide.SetScrollbar(&m_ListScroll);


    char Scheme[256];
    g_UiBase.GetCurSchemePath(Scheme, 256);
    LoadScheme(Scheme);

    char Buff[128];
    KIniFile Ini;

    sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
    if (Ini.Load(Buff)) {
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "Enchase", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));

        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", "EnchaseRule", "", Buff, sizeof(Buff));

        m_Guide.AddOneMessage(Buff, sizeof(Buff));
    }

    Wnd_AddWindow(this);
}

void KUiEnchaseTim::UpdateItem(KUiDraggedObject *pItem, int bAdd) {
    if (!pItem) return;

    // Map Region.v to box: 0=BigBox, 1=Box1, 2=Box2
    // Matrix box (m_ItemBox) uses different container UOC_COMPOUND_BOX
    if (pItem->DataY == 0) { // BigBox
        if (bAdd) {
            m_BigBox.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_BigBox.Clear();
        }
    } else if (pItem->DataY == 1) { // Box1
        if (bAdd) {
            m_Box1.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box1.Clear();
        }
    } else if (pItem->DataY == 2) { // Box2
        if (bAdd) {
            m_Box2.HoldObject(pItem->uGenre, pItem->uId, pItem->DataW, pItem->DataH);
        } else {
            m_Box2.Clear();
        }
    }
}

void KUiEnchaseTim::CleanItem() {
    m_BigBox.Clear();
    m_Box1.Clear();
    m_Box2.Clear();
    m_ItemBox.Clear();
}

KUiAtlas::KUiAtlas() {

}

void KUiAtlas::PaintWindow() {

}

int KUiAtlas::WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
    return 1;
}

void KUiAtlas::LoadScheme(const char *pScheme) {

}

void KUiAtlas::Initialize() {

}

void KUiAtlas::UpdateItem(KUiDraggedObject *pItem, int bAdd) {

}

int KCanGetNumImage2::GetMaxFrame() {
    return m_Image.nNumFrames;
}


int KCanGetNumImage2::GetCurrentFrame() {
    return m_Image.nFrame;
}
