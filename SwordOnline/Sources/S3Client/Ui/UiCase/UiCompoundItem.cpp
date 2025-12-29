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
#define    SCHEME_INI_INOUT            "UiCompoundItem_Inout.ini"

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
                // Switch to COMPOUND case 3 (Mineral mode)
                m_nNum = WINDOWS_COMP3;
                ShowWindow(0);
                m_CompoundPad.SetPosText(3);
                // Hide dropdown menu after selection
                m_UpCryoliteBtn.Hide();
                m_UpPropMineBtn.Hide();
                g_DebugLog("[UI] Switched to COMPOUND case 3 (Mineral), dropdown hidden");
            } else if (uParam == (unsigned int) &m_UpCryoliteBtn && m_UpCryoliteBtn.IsVisible()) {
                // Switch to COMPOUND case 2 (Crystal mode)
                m_nNum = WINDOWS_COMP2;
                ShowWindow(0);
                m_CompoundPad.SetPosText(2);
                // Hide dropdown menu after selection
                m_UpCryoliteBtn.Hide();
                m_UpPropMineBtn.Hide();
                g_DebugLog("[UI] Switched to COMPOUND case 2 (Crystal), dropdown hidden");
            } else if (uParam == (unsigned int) &m_CompoundPadBtn && m_UpCryoliteBtn.IsVisible()) {
                // Switch to COMPOUND case 1 (Equipment mode)
                m_nNum = WINDOWS_COMP;
                ShowWindow(0);
                m_CompoundPad.SetPosText(1);
                // Hide dropdown menu after selection
                m_UpCryoliteBtn.Hide();
                m_UpPropMineBtn.Hide();
                g_DebugLog("[UI] Switched to COMPOUND case 1 (Equipment), dropdown hidden");
            } else if (uParam == (unsigned int) &m_CompoundPadBtn && !m_UpCryoliteBtn.IsVisible()) {
                // Show dropdown menu
                m_CompoundPadBtn.CheckButton(TRUE);
                int nX, nY;
                m_CompoundPadBtn.GetPosition(&nX, &nY);
                m_UpCryoliteBtn.SetPosition(nX, nY + 19);
                m_UpPropMineBtn.SetPosition(nX, nY + 38);
                m_UpCryoliteBtn.BringToTop();
                m_UpPropMineBtn.BringToTop();
                m_UpCryoliteBtn.Show();
                m_UpPropMineBtn.Show();
                g_DebugLog("[UI] COMPOUND dropdown menu shown");
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
            m_pSelf->m_CompoundPad.UpdateData(); // Load items and show boxes when showing COMPOUND tab
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
            m_pSelf->m_DistillPad.UpdateData(); // Load items and show boxes when showing DISTILL tab
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
            m_pSelf->m_EnchasePad.UpdateData(); // Load items and show boxes when showing ENCHASE tab
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
        case WINDOWS_COMP:  // Mode 1: Equipment → Huyen Tinh
            g_DebugLog("[CLIENT COMPOUND Mode1] Equipment crafting button clicked!");
            if (g_pCoreShell) {
                g_DebugLog("[CLIENT COMPOUND Mode1] g_pCoreShell is valid");
                if (g_pCoreShell->GetLixian()) {
                    g_DebugLog("[CLIENT COMPOUND Mode1] GetLixian() = TRUE, sending GOI_EXESCRIPT_BUTTON");
                    char szFunc[32];
                    sprintf(szFunc, "ExeCompoundEquipment");
                    g_DebugLog("[CLIENT COMPOUND Mode1] Function name: %s", szFunc);
                    g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
                    g_DebugLog("[CLIENT COMPOUND Mode1] OperationRequest sent successfully");
                } else {
                    g_DebugLog("[CLIENT COMPOUND Mode1] ERROR: GetLixian() = FALSE! Cannot execute script!");
                }
            } else {
                g_DebugLog("[CLIENT COMPOUND Mode1] ERROR: g_pCoreShell is NULL!");
            }
            break;
        case WINDOWS_COMP2:  // Mode 2: Crystal → Higher level crystal (m_nSelect = 1)
            g_DebugLog("[CLIENT COMPOUND Mode2] Crystal upgrade button clicked!");
            if (g_pCoreShell) {
                g_DebugLog("[CLIENT COMPOUND Mode2] g_pCoreShell is valid");
                if (g_pCoreShell->GetLixian()) {
                    g_DebugLog("[CLIENT COMPOUND Mode2] GetLixian() = TRUE, sending GOI_EXESCRIPT_BUTTON");
                    char szFunc[32];
                    sprintf(szFunc, "ExeCompoundCrystal");
                    g_DebugLog("[CLIENT COMPOUND Mode2] Function name: %s", szFunc);
                    g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
                    g_DebugLog("[CLIENT COMPOUND Mode2] OperationRequest sent successfully");
                } else {
                    g_DebugLog("[CLIENT COMPOUND Mode2] ERROR: GetLixian() = FALSE! Cannot execute script!");
                }
            } else {
                g_DebugLog("[CLIENT COMPOUND Mode2] ERROR: g_pCoreShell is NULL!");
            }
            break;
        case WINDOWS_COMP3:
            if (g_pCoreShell) {
                g_pCoreShell->OperationRequest(GOI_COMPITEM_COM, pItem, 3);
            }
            break;
        case WINDOWS_DISTill:
            g_DebugLog("[CLIENT DISTILL] Extract attribute button clicked!");
            if (g_pCoreShell) {
                g_DebugLog("[CLIENT DISTILL] g_pCoreShell is valid");
                if (g_pCoreShell->GetLixian()) {
                    g_DebugLog("[CLIENT DISTILL] GetLixian() = TRUE, sending GOI_EXESCRIPT_BUTTON");
                    char szFunc[32];
                    sprintf(szFunc, "ExeExtractAttribute");
                    g_DebugLog("[CLIENT DISTILL] Function name: %s", szFunc);
                    g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
                    g_DebugLog("[CLIENT DISTILL] OperationRequest sent successfully");
                } else {
                    g_DebugLog("[CLIENT DISTILL] ERROR: GetLixian() = FALSE! Cannot execute script!");
                }
            } else {
                g_DebugLog("[CLIENT DISTILL] ERROR: g_pCoreShell is NULL!");
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

                // Map window pointer to Region.v (slot) + ADD VALIDATION
                if (pWnd == (KWndWindow*)&m_Box1) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM1;  // Slot 0
                    g_DebugLog("[COMPOUND] Mapped to Box1 (UIEP_BUILDITEM1 = %d)", UIEP_BUILDITEM1);

                    // VALIDATION: Mode 1 (Equipment) - Box1 only accepts RINGS
                    if (pDropPos && m_nSelect == 0) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be equipment
                        if (nGenre != item_equip && nGenre != item_purpleequip &&
                            nGenre != item_goldequip && nGenre != item_platinaequip) {
                            g_DebugLog("[COMPOUND] REJECT Box1 Mode1: Genre %d is not equipment", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat trang bi vao o nay!", 256);
                            break;
                        }

                        // Must be RING (equip_ring = 3)
                        if (nDetail != equip_ring) {
                            g_DebugLog("[COMPOUND] REJECT Box1 Mode1: Detail %d is not ring (need %d)", nDetail, equip_ring);
                            KUiMsgCentrePad::SystemMessageArrival("O nay chi duoc dat NHAN! (Ring)", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box1 Mode1 validation PASSED: ring accepted");
                    }

                    // VALIDATION: Mode 2 (Crystal) - Box1 only accepts HUYEN TINH
                    if (pDropPos && m_nSelect == 1) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be item_task (genre = 6)
                        if (nGenre != item_task) {
                            g_DebugLog("[COMPOUND] REJECT Box1 Mode2: Genre %d is not item_task", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH vao o nay!", 256);
                            break;
                        }

                        // Must be Huyen Tinh (detail = 74-79 for levels 1-6)
                        if (nDetail < 74 || nDetail > 79) {
                            g_DebugLog("[COMPOUND] REJECT Box1 Mode2: Detail %d is not Huyen Tinh (need 74-79)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH cap 1-6!", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box1 Mode2 validation PASSED: Huyen Tinh accepted (detail=%d)", nDetail);
                    }


                } else if (pWnd == (KWndWindow*)&m_Box2) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[COMPOUND] Mapped to Box2 (UIEP_BUILDITEM2 = %d)", UIEP_BUILDITEM2);

                    // VALIDATION: Mode 1 (Equipment) - Box2 only accepts NECKLACES
                    if (pDropPos && m_nSelect == 0) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be equipment
                        if (nGenre != item_equip && nGenre != item_purpleequip &&
                            nGenre != item_goldequip && nGenre != item_platinaequip) {
                            g_DebugLog("[COMPOUND] REJECT Box2 Mode1: Genre %d is not equipment", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat trang bi vao o nay!", 256);
                            break;
                        }

                        // Must be NECKLACE (equip_amulet = 4)
                        if (nDetail != equip_amulet) {
                            g_DebugLog("[COMPOUND] REJECT Box2 Mode1: Detail %d is not necklace (need %d)", nDetail, equip_amulet);
                            KUiMsgCentrePad::SystemMessageArrival("O nay chi duoc dat DAY CHUYEN! (Necklace/Amulet)", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box2 Mode1 validation PASSED: necklace accepted");
                    }

                    // VALIDATION: Mode 2 (Crystal) - Box2 only accepts HUYEN TINH
                    if (pDropPos && m_nSelect == 1) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        if (nGenre != item_task) {
                            g_DebugLog("[COMPOUND] REJECT Box2 Mode2: Genre %d is not item_task", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH vao o nay!", 256);
                            break;
                        }

                        if (nDetail < 74 || nDetail > 79) {
                            g_DebugLog("[COMPOUND] REJECT Box2 Mode2: Detail %d is not Huyen Tinh (need 74-79)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH cap 1-6!", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box2 Mode2 validation PASSED: Huyen Tinh accepted (detail=%d)", nDetail);
                    }


                } else if (pWnd == (KWndWindow*)&m_Box3) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM3;  // Slot 2
                    g_DebugLog("[COMPOUND] Mapped to Box3 (UIEP_BUILDITEM3 = %d)", UIEP_BUILDITEM3);

                    // VALIDATION: Mode 1 (Equipment) - Box3 only accepts PENDANTS
                    if (pDropPos && m_nSelect == 0) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be equipment
                        if (nGenre != item_equip && nGenre != item_purpleequip &&
                            nGenre != item_goldequip && nGenre != item_platinaequip) {
                            g_DebugLog("[COMPOUND] REJECT Box3 Mode1: Genre %d is not equipment", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat trang bi vao o nay!", 256);
                            break;
                        }

                        // Must be PENDANT (equip_pendant = 9)
                        if (nDetail != equip_pendant) {
                            g_DebugLog("[COMPOUND] REJECT Box3 Mode1: Detail %d is not pendant (need %d)", nDetail, equip_pendant);
                            KUiMsgCentrePad::SystemMessageArrival("O nay chi duoc dat NGOC BOI! (Pendant)", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box3 Mode1 validation PASSED: pendant accepted");
                    }

                    // VALIDATION: Mode 2 (Crystal) - Box3 only accepts HUYEN TINH
                    if (pDropPos && m_nSelect == 1) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        if (nGenre != item_task) {
                            g_DebugLog("[COMPOUND] REJECT Box3 Mode2: Genre %d is not item_task", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH vao o nay!", 256);
                            break;
                        }

                        if (nDetail < 74 || nDetail > 79) {
                            g_DebugLog("[COMPOUND] REJECT Box3 Mode2: Detail %d is not Huyen Tinh (need 74-79)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH cap 1-6!", 256);
                            break;
                        }

                        g_DebugLog("[COMPOUND] Box3 Mode2 validation PASSED: Huyen Tinh accepted (detail=%d)", nDetail);
                    }


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
                g_DebugLog("[COMPOUND] Compound button clicked");

                // Don't allow crafting if animation is running
                if (m_nStatus != STATUS_WAITING_MATERIALS) {
                    g_DebugLog("[COMPOUND] Animation already running, ignoring click");
                    return 1;
                }

                // Validate all 3 items exist BEFORE starting effect
                KUiDraggedObject pObj;
                m_Box1.GetObject(pObj);
                if (pObj.uId <= 0) {
                    g_DebugLog("[COMPOUND] No item in Box1");
                    KUiMsgCentrePad::SystemMessageArrival("Vui long dat item vao o 1!", 256);
                    return 1;
                }

                pObj.uId = 0;
                m_Box2.GetObject(pObj);
                if (pObj.uId <= 0) {
                    g_DebugLog("[COMPOUND] No item in Box2");
                    KUiMsgCentrePad::SystemMessageArrival("Vui long dat item vao o 2!", 256);
                    return 1;
                }

                pObj.uId = 0;
                m_Box3.GetObject(pObj);
                if (pObj.uId <= 0) {
                    g_DebugLog("[COMPOUND] No item in Box3");
                    KUiMsgCentrePad::SystemMessageArrival("Vui long dat item vao o 3!", 256);
                    return 1;
                }

                // Start animation effect
                // Request will be sent AFTER animation finishes in UpdateResult()
                g_DebugLog("[COMPOUND] All items validated, starting effect animation");
                m_nStatus = STATUS_BEGIN_TREMBLE;

                // Disable picking during animation
                m_Box1.EnablePickPut(false);
                m_Box2.EnablePickPut(false);
                m_Box3.EnablePickPut(false);
            }
            break;

        default:
            return KWndImage::WndProc(uMsg, uParam, nParam);
    }
    return 1;
}

void KUiCompound::UpdateResult() {
    g_DebugLog("[COMPOUND] UpdateResult: Animation finished, sending craft request");

    // Send crafting request based on mode
    KUiComItem *pSelf = KUiComItem::GetIfVisible();
    if (!pSelf) {
        g_DebugLog("[COMPOUND] ERROR: KUiComItem not visible!");
        m_nStatus = STATUS_WAITING_MATERIALS;
        return;
    }

    // Get item IDs for all 3 boxes
    KUiDraggedObject pObj;
    unsigned int pUP[3];

    m_Box1.GetObject(pObj);
    pUP[0] = pObj.uId;

    pObj.uId = 0;
    m_Box2.GetObject(pObj);
    pUP[1] = pObj.uId;

    pObj.uId = 0;
    m_Box3.GetObject(pObj);
    pUP[2] = pObj.uId;

    // Send request to server
    pSelf->ComItem((unsigned int) (&pUP), m_nSelect, 3);
    g_DebugLog("[COMPOUND] Craft request sent, m_nSelect=%d", m_nSelect);

    // Clean boxes to remove visual representation
    // Server will delete actual items
    CleanItem();

    // Re-enable picking
    m_Box1.EnablePickPut(true);
    m_Box2.EnablePickPut(true);
    m_Box3.EnablePickPut(true);

    // Reset status
    m_nStatus = STATUS_WAITING_MATERIALS;
    g_DebugLog("[COMPOUND] UpdateResult complete, status reset");
}

void KUiCompound::Breathe() {
    if (m_nStatus == STATUS_BEGIN_TREMBLE) {
        // Show all 3 effects for the 3 boxes
        m_TrembleEffect1.Show();
        m_TrembleEffect1.SetFrame(0);
        m_TrembleEffect2.Show();
        m_TrembleEffect2.SetFrame(0);
        m_TrembleEffect3.Show();
        m_TrembleEffect3.SetFrame(0);
        m_nStatus = STATUS_TREMBLING;
        g_DebugLog("[COMPOUND] Started effect animation on all 3 boxes");
        g_DebugLog("[COMPOUND] Effect1: MaxFrame=%d, CurrentFrame=%d, IsVisible=%d",
            m_TrembleEffect1.GetMaxFrame(), m_TrembleEffect1.GetCurrentFrame(),
            m_TrembleEffect1.IsVisible());
    } else if (m_nStatus == STATUS_TREMBLING) {
        if (!PlayEffect()) {
            m_nStatus = STATUS_CHANGING_ITEM;
            // Hide all 3 effects when animation finishes
            m_TrembleEffect1.Hide();
            m_TrembleEffect2.Hide();
            m_TrembleEffect3.Hide();
            g_DebugLog("[COMPOUND] Animation finished, hiding effects");
        }
    } else if (m_nStatus == STATUS_CHANGING_ITEM) {
        UpdateResult();
        m_nStatus = STATUS_FINISH;
        g_DebugLog("[COMPOUND] Breathe: Set status to STATUS_FINISH");
    } else if (m_nStatus == STATUS_FINISH) {
        // Reset to waiting state for next crafting
        m_nStatus = STATUS_WAITING_MATERIALS;
        g_DebugLog("[COMPOUND] Breathe: Reset status to STATUS_WAITING_MATERIALS, ready for next craft");
    }
}

int KUiCompound::PlayEffect() {
    // Advance frame on all 3 effects simultaneously
    int nMaxFrame = m_TrembleEffect1.GetMaxFrame();
    int nCurrentFrame = m_TrembleEffect1.GetCurrentFrame();

    static int nWaitCount = 0;  // Track how long we've waited for sprite to load

    // CRITICAL: Call NextFrame() even when MaxFrame=0 to trigger sprite loading!
    // This matches FORGE behavior where sprite loads asynchronously during animation
    if (m_TrembleEffect1.IsVisible()) {
        m_TrembleEffect1.NextFrame();
        m_TrembleEffect2.NextFrame();
        m_TrembleEffect3.NextFrame();
    }

    if (nMaxFrame == 0) {
        // Sprite not loaded yet, but keep calling NextFrame() to trigger load
        nWaitCount++;

        // Timeout after 300 frames (~6 seconds at 50fps)
        if (nWaitCount >= 300) {
            g_DebugLog("[COMPOUND] PlayEffect: Sprite failed to load after %d frames, giving up", nWaitCount);
            nWaitCount = 0;
            return 0;  // Finish animation
        }

        if (nWaitCount % 50 == 0) {
            g_DebugLog("[COMPOUND] PlayEffect: Waiting for sprite to load, MaxFrame still 0 (waited %d frames)", nWaitCount);
        }
        return 1;  // Keep waiting and calling NextFrame()
    }

    // Sprite loaded successfully!
    if (nWaitCount > 0) {
        g_DebugLog("[COMPOUND] PlayEffect: Sprite loaded successfully after %d frames! MaxFrame=%d", nWaitCount, nMaxFrame);
        nWaitCount = 0;
    }

    // Check if animation is complete (current frame reached max)
    if (nCurrentFrame >= nMaxFrame - 1) {
        m_TrembleEffect1.SetFrame(0);
        m_TrembleEffect2.SetFrame(0);
        m_TrembleEffect3.SetFrame(0);
        g_DebugLog("[COMPOUND] Animation COMPLETE: CurrentFrame=%d reached MaxFrame=%d", nCurrentFrame, nMaxFrame);
        return 0;  // Animation complete
    }

    // Log progress
    if (nCurrentFrame % 10 == 0) {
        g_DebugLog("[COMPOUND] Animating: Frame %d/%d", nCurrentFrame, nMaxFrame);
    }
    return 1;  // Still animating
}

void KUiCompound::PaintWindow() {
    KWndPage::PaintWindow();
}

void KUiCompound::LoadScheme(const char *pScheme) {
    g_DebugLog("[COMPOUND] LoadScheme START - initializing from INI");

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

        // Log sprite paths to verify what we're trying to load
        char szSpritePath[256];
        Ini.GetString("Effect_0", "Image", "", szSpritePath, sizeof(szSpritePath));
        g_DebugLog("[COMPOUND] Effect_0 sprite path from INI: %s", szSpritePath);

        g_DebugLog("[COMPOUND] Before Init Effect_0: INI file loaded from %s", Buff);
        m_TrembleEffect1.Init(&Ini, "Effect_0");
        g_DebugLog("[COMPOUND] After Init Effect_0: MaxFrame=%d, IsVisible=%d",
            m_TrembleEffect1.GetMaxFrame(), m_TrembleEffect1.IsVisible());
        m_TrembleEffect1.Hide();
        g_DebugLog("[COMPOUND] After Hide Effect_0: IsVisible=%d", m_TrembleEffect1.IsVisible());

        Ini.GetString("Effect_1", "Image", "", szSpritePath, sizeof(szSpritePath));
        g_DebugLog("[COMPOUND] Effect_1 sprite path from INI: %s", szSpritePath);
        m_TrembleEffect2.Init(&Ini, "Effect_1");
        g_DebugLog("[COMPOUND] After Init Effect_1: MaxFrame=%d", m_TrembleEffect2.GetMaxFrame());
        m_TrembleEffect2.Hide();

        Ini.GetString("Effect_2", "Image", "", szSpritePath, sizeof(szSpritePath));
        g_DebugLog("[COMPOUND] Effect_2 sprite path from INI: %s", szSpritePath);
        m_TrembleEffect3.Init(&Ini, "Effect_2");
        g_DebugLog("[COMPOUND] After Init Effect_2: MaxFrame=%d", m_TrembleEffect3.GetMaxFrame());
        m_TrembleEffect3.Hide();

        g_DebugLog("[COMPOUND] All effects initialized - Effect1 MaxFrame=%d, Effect2 MaxFrame=%d, Effect3 MaxFrame=%d",
            m_TrembleEffect1.GetMaxFrame(), m_TrembleEffect2.GetMaxFrame(), m_TrembleEffect3.GetMaxFrame());
        //m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");

        // Bring boxes to top so they can receive mouse events
        m_Box1.BringToTop();
        m_Box2.BringToTop();
        m_Box3.BringToTop();
        g_DebugLog("[COMPOUND] LoadScheme: Boxes brought to top for mouse events");

        int nX, nY, nColor;
        // Use Box_0 position for Pos1 text (above Box1)
        Ini.GetInteger2("Box_0", "Pos", &nX, &nY);

        if (Ini.GetString("TextColor", "Font", "", Buffer, sizeof(Buffer))) {
            nColor = (::GetColor(Buffer) & 0xFFFFFF);
        }
        m_nSelect = 0;
        m_Pos1.SetPosition(nX - 14, nY - 4);
        m_Pos1.SetText("[1] Nh�n");  // Add [1] prefix for case 1
        m_Pos1.SetTextColor(nColor);
        m_Pos1.BringToTop();  // Bring text to top of boxes

        // Use Box_1 position for Pos2 text (above Box2)
        Ini.GetInteger2("Box_1", "Pos", &nX, &nY);

        m_Pos2.SetPosition(nX - 14, nY - 4);
        m_Pos2.SetText("[1] D�y chuy�n/h� th�n ph�");  // Add [1] prefix for case 1
        m_Pos2.SetTextColor(nColor);
        m_Pos2.BringToTop();  // Bring text to top of boxes

        // Use Box_2 position for Pos3 text (above Box3)
        Ini.GetInteger2("Box_2", "Pos", &nX, &nY);

        m_Pos3.SetPosition(nX - 14, nY - 4);
        m_Pos3.SetText("[1] Ng�c b�i/h��ng nang");  // Add [1] prefix for case 1
        m_Pos3.SetTextColor(nColor);
        m_Pos3.BringToTop();  // Bring text to top of boxes

        g_DebugLog("[COMPOUND] LoadScheme: Default text set to case 1 (Equipment), m_nSelect=0");

        // 		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
        // 		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
    }
    g_DebugLog("[COMPOUND] LoadScheme COMPLETE");
}

void KUiCompound::Initialize() {
    // Initialize status to waiting for materials
    m_nStatus = STATUS_WAITING_MATERIALS;
    g_DebugLog("[COMPOUND] Initialize: m_nStatus set to STATUS_WAITING_MATERIALS");

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
    g_DebugLog("[COMPOUND] SetPosText called with case=%d", i);

    // Update box labels based on mode
    switch (i) {
        case 1:
            m_nSelect = 0;
            m_Pos1.SetText("[1] Nh�n");
            m_Pos2.SetText("[1] D�y chuy�n/h� th�n ph�");
            m_Pos3.SetText("[1] Ng�c b�i/h��ng nang");
            g_DebugLog("[COMPOUND] SetPosText: Switched to case 1 (Equipment) - [1] Nhẫn, Dây chuyền, Ngọc bội");
            break;
        case 2:
            m_nSelect = 1;
            m_Pos1.SetText("[2] Huy�n tinh 1");
            m_Pos2.SetText("[2] Huy�n tinh 2");
            m_Pos3.SetText("[2] Huy�n tinh 3");
            g_DebugLog("[COMPOUND] SetPosText: Switched to case 2 (Crystal) - [2] Huyền tinh 1, 2, 3");
            break;
        case 3:
            m_nSelect = 2;
            m_Pos1.SetText("[3] Kho�ng th�ch 1");
            m_Pos2.SetText("[3] Kho�ng th�ch 2");
            m_Pos3.SetText("[3] Kho�ng th�ch 3");
            g_DebugLog("[COMPOUND] SetPosText: Switched to case 3 (Mineral) - [3] Khoáng thạch 1, 2, 3");
            break;
    }

    // Force update text labels visibility and position
    m_Pos1.Show();
    m_Pos2.Show();
    m_Pos3.Show();
    m_Pos1.BringToTop();
    m_Pos2.BringToTop();
    m_Pos3.BringToTop();

    // Force redraw/repaint by hiding and showing
    m_Pos1.Hide();
    m_Pos2.Hide();
    m_Pos3.Hide();
    m_Pos1.Show();
    m_Pos2.Show();
    m_Pos3.Show();

    g_DebugLog("[COMPOUND] SetPosText: m_nSelect=%d, text labels force refreshed with [%d] prefix", m_nSelect, i);

    // Update guide text based on mode
    char Scheme[256];
    char Buff[128];
    KIniFile Ini;

    g_UiBase.GetCurSchemePath(Scheme, 256);
    sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);

    if (Ini.Load(Buff)) {
        // Clear current guide text
        m_Guide.Clear();
        g_DebugLog("[COMPOUND] SetPosText: Cleared guide text");

        // Load appropriate text based on mode
        const char* key1 = "";
        const char* key2 = "";

        switch (i) {
            case 1: // Equipment mode
                key1 = "CompoundRule";
                key2 = "CompoundRule2";
                g_DebugLog("[COMPOUND] SetPosText: Loading Equipment guide text");
                break;
            case 2: // Crystal mode
                key1 = "Compound";
                key2 = "UpCryoliteRule";  // Use crystal-specific text, not equipment text
                g_DebugLog("[COMPOUND] SetPosText: Loading Crystal guide text");
                break;
            case 3: // Mineral mode
                key1 = "UpPropMine";
                key2 = "UpPropMineRule";
                g_DebugLog("[COMPOUND] SetPosText: Loading Mineral guide text");
                break;
        }

        // Add first message
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", key1, "", Buff, sizeof(Buff));
        if (Buff[0] != '\0') {
            m_Guide.AddOneMessage(Buff, sizeof(Buff));
            g_DebugLog("[COMPOUND] SetPosText: Added guide text 1: %s", key1);
        }

        // Add second message
        ZeroMemory(Buff, sizeof(Buff));
        Ini.GetString("RuleInfo", key2, "", Buff, sizeof(Buff));
        if (Buff[0] != '\0') {
            m_Guide.AddOneMessage(Buff, sizeof(Buff));
            g_DebugLog("[COMPOUND] SetPosText: Added guide text 2: %s", key2);
        }
    }
}

void KUiCompound::CleanItem() {
    m_Box1.Clear();
    m_Box2.Clear();
    m_Box3.Clear();
}

// Load items from server and show boxes (similar to FORGE UpdateData)
void KUiCompound::UpdateData() {
    g_DebugLog("[COMPOUND] UpdateData called - loading items and showing boxes");

    // Request build items from server (same as FORGE/TrembleItem pattern)
    KUiObjAtRegion Items[MAX_PART_BUILD];
    int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Items, 0);

    g_DebugLog("[COMPOUND] UpdateData: Got %d items from server", nCount);

    // Clear all boxes first
    m_Box1.Clear();
    m_Box2.Clear();
    m_Box3.Clear();

    // Update boxes with items from server
    for (int i = 0; i < nCount; i++) {
        if (Items[i].Obj.uGenre != CGOG_NOTHING) {
            g_DebugLog("[COMPOUND] UpdateData: Item[%d] Region.v=%d, genre=%d, id=%d",
                i, Items[i].Region.v, Items[i].Obj.uGenre, Items[i].Obj.uId);

            // Map Region.v to boxes: 0=Box1, 1=Box2, 2=Box3
            if (Items[i].Region.v == UIEP_BUILDITEM1) { // Slot 0 = Box1
                m_Box1.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
                g_DebugLog("[COMPOUND] UpdateData: Loaded item into Box1");
            } else if (Items[i].Region.v == UIEP_BUILDITEM2) { // Slot 1 = Box2
                m_Box2.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
                g_DebugLog("[COMPOUND] UpdateData: Loaded item into Box2");
            } else if (Items[i].Region.v == UIEP_BUILDITEM3) { // Slot 2 = Box3
                m_Box3.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
                g_DebugLog("[COMPOUND] UpdateData: Loaded item into Box3");
            }
        }
    }

    // CRITICAL: Show boxes to enable drag-drop functionality
    m_Box1.Show();
    m_Box2.Show();
    m_Box3.Show();

    // CRITICAL: Enable pick/put operations on boxes
    m_Box1.EnablePickPut(true);
    m_Box2.EnablePickPut(true);
    m_Box3.EnablePickPut(true);

    g_DebugLog("[COMPOUND] UpdateData: All boxes shown, pick/put enabled, ready for drag-drop");
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

                    // VALIDATION: BigBox only accepts green equipment (item_equip with quality)
                    if (pDropPos) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be equipment (genre = 0)
                        if (nGenre != item_equip) {
                            g_DebugLog("[DISTILL] REJECT BigBox: Genre %d is not item_equip", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat TRANG BI XANH vao o nay!", 256);
                            break;
                        }

                        g_DebugLog("[DISTILL] BigBox validation PASSED: Equipment accepted (genre=%d, detail=%d)", nGenre, nDetail);
                    }
                } else if (pWnd == (KWndWindow*)&m_Box1) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM2;  // Slot 1
                    g_DebugLog("[DISTILL] Mapped to Box1 (slot 1)");

                    // VALIDATION: Box1 only accepts Huyen Tinh (genre=6, detail=74-79)
                    if (pDropPos) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be item_task (genre = 6)
                        if (nGenre != item_task) {
                            g_DebugLog("[DISTILL] REJECT Box1: Genre %d is not item_task", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH vao o nay!", 256);
                            break;
                        }

                        // Must be Huyen Tinh (detail = 74-79 for levels 1-6)
                        if (nDetail < 74 || nDetail > 79) {
                            g_DebugLog("[DISTILL] REJECT Box1: Detail %d is not Huyen Tinh (need 74-79)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat HUYEN TINH cap 1-6!", 256);
                            break;
                        }

                        g_DebugLog("[DISTILL] Box1 validation PASSED: Huyen Tinh accepted (detail=%d)", nDetail);
                    }
                } else if (pWnd == (KWndWindow*)&m_Box2) {
                    Drop.Region.v = Pick.Region.v = UIEP_BUILDITEM3;  // Slot 2
                    g_DebugLog("[DISTILL] Mapped to Box2 (slot 2)");

                    // VALIDATION: Box2 only accepts Khoang thach (genre=7, detail=146-151)
                    if (pDropPos) {
                        int nGenre = g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre);
                        int nDetail = g_pCoreShell->GetDetailItem(Obj.uId);

                        // Must be item_script (genre = 7)
                        if (nGenre != item_script) {
                            g_DebugLog("[DISTILL] REJECT Box2: Genre %d is not item_script", nGenre);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat KHOANG THACH vao o nay!", 256);
                            break;
                        }

                        // Must be Khoang thach (detail = 146-151)
                        if (nDetail < 146 || nDetail > 151) {
                            g_DebugLog("[DISTILL] REJECT Box2: Detail %d is not Khoang thach (need 146-151)", nDetail);
                            KUiMsgCentrePad::SystemMessageArrival("Chi duoc dat KHOANG THACH (detail 146-151)!", 256);
                            break;
                        }

                        g_DebugLog("[DISTILL] Box2 validation PASSED: Khoang thach accepted (detail=%d)", nDetail);
                    }
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

// Load items from server and show boxes (similar to FORGE UpdateData)
void KUiDistill::UpdateData() {
    g_DebugLog("[DISTILL] UpdateData called - loading items and showing boxes");

    // Request build items from server
    KUiObjAtRegion Items[MAX_PART_BUILD];
    int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Items, 0);

    g_DebugLog("[DISTILL] UpdateData: Got %d items from server", nCount);

    // Clear all boxes first
    m_BigBox.Clear();
    m_Box1.Clear();
    m_Box2.Clear();
    m_ItemBox.Clear();

    // Update boxes with items from server
    for (int i = 0; i < nCount; i++) {
        if (Items[i].Obj.uGenre != CGOG_NOTHING) {
            g_DebugLog("[DISTILL] UpdateData: Item[%d] Region.v=%d, genre=%d, id=%d",
                i, Items[i].Region.v, Items[i].Obj.uGenre, Items[i].Obj.uId);

            // Map Region.v to boxes
            if (Items[i].Region.v == UIEP_BUILDITEM1) {
                m_BigBox.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            } else if (Items[i].Region.v == UIEP_BUILDITEM2) {
                m_Box1.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            } else if (Items[i].Region.v == UIEP_BUILDITEM3) {
                m_Box2.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            }
        }
    }

    // CRITICAL: Show boxes to enable drag-drop functionality
    m_BigBox.Show();
    m_Box1.Show();
    m_Box2.Show();
    m_ItemBox.Show();

    // CRITICAL: Enable pick/put operations on boxes
    m_BigBox.EnablePickPut(true);
    m_Box1.EnablePickPut(true);
    m_Box2.EnablePickPut(true);

    g_DebugLog("[DISTILL] UpdateData: All boxes shown, pick/put enabled, ready for drag-drop");
}

KUiForge::KUiForge() {
    m_EffectTime = 0;
}

void KUiForge::PaintWindow() {
    KWndPage::PaintWindow();
}

// Animation frame update - called every frame
void KUiForge::Breathe() {
    // Only process if effect is running
    if (m_EffectTime == 0)
        return;

    if (m_TrembleEffect1.IsVisible())
        m_TrembleEffect1.NextFrame();

    m_EffectTime++;

    // Use LOOP=2 like UiTrembleItem
    // Formula: stop at (MaxFrame * LOOP) + 1
    #define LOOP 2
    int nMaxFrame = m_TrembleEffect1.GetMaxFrame();

    // CRITICAL: GetMaxFrame() returns 0 initially until sprite loads
    // Skip stop check until MaxFrame is valid (> 0)
    if (nMaxFrame == 0) {
        if (m_EffectTime % 10 == 0) {
            g_DebugLog("[FORGE EFFECT] Waiting for sprite to load - EffectTime=%d, MaxFrame=0",
                m_EffectTime);
        }
        return;
    }

    int nStopTime = nMaxFrame * LOOP + 1;

    if (m_EffectTime >= nStopTime) {
        // Log box state before stopping
        KUiDraggedObject objBig, objSmall;
        m_BigBox.GetObject(objBig);
        m_SmallBox.GetObject(objSmall);
        g_DebugLog("[FORGE EFFECT] BEFORE STOP - BigBox has item: %s (id=%d), SmallBox has item: %s (id=%d)",
            objBig.uId > 0 ? "YES" : "NO", objBig.uId,
            objSmall.uId > 0 ? "YES" : "NO", objSmall.uId);

        g_DebugLog("[FORGE EFFECT] Stopping - EffectTime=%d, MaxFrame=%d, StopTime=%d",
            m_EffectTime, nMaxFrame, nStopTime);
        StopEffect();
    } else if (m_EffectTime % 10 == 0) {
        // Log every 10 frames for debugging + check box state
        KUiDraggedObject objBig, objSmall;
        m_BigBox.GetObject(objBig);
        m_SmallBox.GetObject(objSmall);
        g_DebugLog("[FORGE EFFECT] Animating - EffectTime=%d/%d, CurrentFrame=%d/%d, BigBox=%s, SmallBox=%s",
            m_EffectTime, nStopTime, m_TrembleEffect1.GetCurrentFrame(), nMaxFrame,
            objBig.uId > 0 ? "HAS_ITEM" : "EMPTY",
            objSmall.uId > 0 ? "HAS_ITEM" : "EMPTY");
    }
}

// Start crafting effect animation
void KUiForge::StartEffect() {
    m_TrembleEffect1.Show();
    m_TrembleEffect1.SetFrame(0);  // Start from frame 0
    m_EffectTime = 1;

    // Keep boxes VISIBLE during animation so user can see what they're crafting
    // Items will be cleared by UpdateData() after server responds

    // Disable item picking during animation
    m_BigBox.EnablePickPut(false);
    m_SmallBox.EnablePickPut(false);

    int nMaxFrame = m_TrembleEffect1.GetMaxFrame();
    g_DebugLog("[FORGE EFFECT] Animation started - MaxFrame=%d, will run for %d frames, boxes visible",
        nMaxFrame, nMaxFrame * 2 + 1);

    // Log box state at start
    KUiDraggedObject objBig, objSmall;
    m_BigBox.GetObject(objBig);
    m_SmallBox.GetObject(objSmall);
    g_DebugLog("[FORGE EFFECT] START - BigBox has item: %s (id=%d), SmallBox has item: %s (id=%d)",
        objBig.uId > 0 ? "YES" : "NO", objBig.uId,
        objSmall.uId > 0 ? "YES" : "NO", objSmall.uId);
}

// Stop effect and send craft request (like UiTrembleItem::StopEffect -> OnOk)
void KUiForge::StopEffect() {
    m_TrembleEffect1.Hide();
    m_EffectTime = 0;

    // Re-enable item picking
    m_BigBox.EnablePickPut(true);
    m_SmallBox.EnablePickPut(true);

    g_DebugLog("[FORGE EFFECT] Animation finished, now sending craft request to server");

    // CRITICAL: Send request to server AFTER animation finishes (like UiTrembleItem)
    if (g_pCoreShell && g_pCoreShell->GetLixian()) {
        char szFunc[32];
        sprintf(szFunc, "ExeCompoundForge");
        g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, (unsigned int)szFunc, 4);
        g_DebugLog("[FORGE] Sent ExeCompoundForge to server");
    }

    // Clean boxes immediately after sending request to remove shadow/ghost items
    // Server will delete the actual items, we just clear the visual representation
    CleanItem();
    g_DebugLog("[FORGE EFFECT] Animation stopped, request sent, boxes cleaned");
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
                g_DebugLog("[FORGE] Forge button clicked");

                // Don't allow crafting if animation is running
                if (IsEffect()) {
                    g_DebugLog("[FORGE] Effect is running, ignoring click");
                    return 1;
                }

                // Validate items BEFORE starting effect
                KUiDraggedObject pObj;
                m_BigBox.GetObject(pObj);
                if (pObj.uId <= 0) {
                    g_DebugLog("[FORGE] No item in BigBox");
                    KUiMsgCentrePad::SystemMessageArrival("Vui long dat trang bi vao o lon!", 256);
                    return 1;
                }

                pObj.uId = 0;
                m_SmallBox.GetObject(pObj);
                if (pObj.uId <= 0) {
                    g_DebugLog("[FORGE] No item in SmallBox");
                    KUiMsgCentrePad::SystemMessageArrival("Vui long dat Huyen Tinh vao o nho!", 256);
                    return 1;
                }

                // Start animation effect (like UiTrembleItem)
                // Request will be sent AFTER animation finishes in StopEffect()
                g_DebugLog("[FORGE] Items validated, starting effect animation");
                StartEffect();
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

    // CRITICAL: Don't update while effect is running - let animation finish first
    if (IsEffect()) {
        g_DebugLog("[FORGE] UpdateData BLOCKED - effect is running, will update when effect stops");
        return;
    }

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

    // Show boxes after updating (in case they were hidden during effect)
    m_BigBox.Show();
    m_SmallBox.Show();

    g_DebugLog("[FORGE] UpdateData complete, boxes shown");
}

void KUiForge::UpdateItem(KUiDraggedObject *pItem, int bAdd) {
    g_DebugLog("[FORGE] UpdateItem called: pItem=%p, bAdd=%d", pItem, bAdd);
    if (!pItem) {
        g_DebugLog("[FORGE] UpdateItem: pItem is NULL, returning");
        return;
    }

    // CRITICAL: Don't update while effect is running - prevents boxes from clearing during animation
    if (IsEffect()) {
        g_DebugLog("[FORGE] UpdateItem BLOCKED - effect is running, ignoring server update");
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
    g_DebugLog("[FORGE] CleanItem called");

    // CRITICAL: Don't clean while effect is running
    if (IsEffect()) {
        g_DebugLog("[FORGE] CleanItem BLOCKED - effect is running");
        return;
    }

    m_BigBox.Clear();
    m_SmallBox.Clear();
    g_DebugLog("[FORGE] Boxes cleared");
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
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_INOUT);
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

// Load items from server and show boxes (similar to FORGE UpdateData)
void KUiEnchaseTim::UpdateData() {
    g_DebugLog("[ENCHASE] UpdateData called - loading items and showing boxes");

    // Request build items from server
    KUiObjAtRegion Items[MAX_PART_BUILD];
    int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, (unsigned int)&Items, 0);

    g_DebugLog("[ENCHASE] UpdateData: Got %d items from server", nCount);

    // Clear all boxes first
    m_BigBox.Clear();
    m_Box1.Clear();
    m_Box2.Clear();
    m_ItemBox.Clear();

    // Update boxes with items from server
    for (int i = 0; i < nCount; i++) {
        if (Items[i].Obj.uGenre != CGOG_NOTHING) {
            g_DebugLog("[ENCHASE] UpdateData: Item[%d] Region.v=%d, genre=%d, id=%d",
                i, Items[i].Region.v, Items[i].Obj.uGenre, Items[i].Obj.uId);

            // Map Region.v to boxes
            if (Items[i].Region.v == UIEP_BUILDITEM1) {
                m_BigBox.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            } else if (Items[i].Region.v == UIEP_BUILDITEM2) {
                m_Box1.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            } else if (Items[i].Region.v == UIEP_BUILDITEM3) {
                m_Box2.HoldObject(Items[i].Obj.uGenre, Items[i].Obj.uId,
                    Items[i].Region.Width, Items[i].Region.Height);
            }
        }
    }

    // CRITICAL: Show boxes to enable drag-drop functionality
    m_BigBox.Show();
    m_Box1.Show();
    m_Box2.Show();
    m_ItemBox.Show();

    // CRITICAL: Enable pick/put operations on boxes
    m_BigBox.EnablePickPut(true);
    m_Box1.EnablePickPut(true);
    m_Box2.EnablePickPut(true);

    g_DebugLog("[ENCHASE] UpdateData: All boxes shown, pick/put enabled, ready for drag-drop");
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
