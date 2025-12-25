--═══════════════════════════════════════════════════════════════
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Opens equipment attribute upgrade UI
-- Date: 2025-12-21
--═══════════════════════════════════════════════════════════════

Include("\\script\\lib\\TaskLib.lua")

-- ────────────────────────────────────────────────────────────
-- Configuration - Item IDs
-- ────────────────────────────────────────────────────────────
UPGRADE_MATERIAL_GENRE = 6      -- item_task
UPGRADE_MATERIAL_DETAIL = 18    -- Luc Thuy Tinh (Green Crystal) - tam thoi

-- Upgrade settings
UPGRADE_FIXED_PERCENT = 20      -- % tang co dinh (FIXED 20%)
UPGRADE_SUCCESS_RATE = 100      -- Ti le thanh cong (%)

-- ────────────────────────────────────────────────────────────
-- NPC Talk Entry Point
-- ───────────────────────────────────────────────────────────
function UpgradeAttribTalk()
    local tbSay = {
        "Nang cap thuoc tinh trang bi xanh/OpenUpgradeUI",
        "Huong dan nang cap/ShowGuide",
        "Ta chi ghe ngang qua/no",
    }
    Say("Cao thu ren duc: Ta co the giup nguoi nang cap tung thuoc tinh cua trang bi xanh!",
        getn(tbSay), tbSay)
end

-- ────────────────────────────────────────────────────────────
-- Open Upgrade UI
-- ────────────────────────────────────────────────────────────
function OpenUpgradeUI()
    OpenUpgradeAttribUI()  -- Call C++ function to open UI
end

-- ────────────────────────────────────────────────────────────
-- Execute Upgrade (called when player clicks "Upgrade" button)
-- This function validates items and shows attribute selection menu
-- ────────────────────────────────────────────────────────────
function ExeUpgradeAttrib()
    local nPos = 15  -- pos_builditem (same container as Tremble)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)    -- Equipment slot
    local nMaterialIdx = GetPOItem(nPos, 1) -- Material slot

    -- Validate equipment
    if nEquipIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat trang bi vao!<color>")
        return
    end

    -- Validate material exists
    if nMaterialIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat Da Nang Cap!<color>")
        return
    end

    -- Validate material type (must be correct upgrade material)
    local nMatGenre, nMatDetail = GetItemProp(nMaterialIdx)

    if nMatGenre ~= UPGRADE_MATERIAL_GENRE or nMatDetail ~= UPGRADE_MATERIAL_DETAIL then
        Talk(1, "", "<color=red>Vat lieu khong dung! Can su dung Da Nang Cap.<color>")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    -- Check if equipment is blue (genre 0 with luck < 1000000000)
    if nGenre ~= 0 then
        Talk(1, "", "<color=red>Chi co the nang cap trang bi xanh!<color>")
        return
    end

    if nLuck >= 1000000000 then
        Talk(1, "", "<color=red>Trang bi nay la tim/vang, khong the nang cap!<color>")
        return
    end

    -- Check if equipment has magic attributes
    local bHasMagicAttrib = 0
    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        if nAttribType and nAttribType > 0 then
            bHasMagicAttrib = 1
            break
        end
    end

    if bHasMagicAttrib == 0 then
        Talk(1, "", "<color=red>Trang bi nay khong co thuoc tinh magic!<color>")
        return
    end

    -- TEST: Simple hardcoded menu
    local tbTestOptions = {
        "Test option 1/DoUpgradeAttrib_0",
        "Test option 2/DoUpgradeAttrib_1",
        "Cancel/no"
    }

    Say("TEST MENU - Chon thuoc tinh:", getn(tbTestOptions), tbTestOptions)
end

-- ────────────────────────────────────────────────────────────
-- Perform actual upgrade on selected attribute slot
-- Called by dynamic functions DoUpgradeAttrib_0 through DoUpgradeAttrib_5
-- ────────────────────────────────────────────────────────────
function PerformUpgrade(nAttribSlot)
    local nPos = 15  -- pos_builditem

    -- Get items again (in case they changed)
    local nEquipIdx = GetPOItem(nPos, 0)
    local nMaterialIdx = GetPOItem(nPos, 1)

    if nEquipIdx <= 0 or nMaterialIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Trang bi hoac vat lieu bi mat!<color>")
        return
    end

    -- Get attribute info for the selected slot
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

    -- Use FIXED upgrade percentage
    local nIncreasePercent = UPGRADE_FIXED_PERCENT

    -- Calculate new value
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    -- Create upgraded item (C++ will handle removing old equipment and placing new one)
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)

    if nNewItemIdx == 0 then
        Talk(1, "", "<color=red>Loi: Khong the nang cap! Co the do rong hoac loi khac.<color>")
        return
    end

    -- Delete material only (equipment already handled by C++ function)
    DelItemByIndex(nMaterialIdx)

    -- Success message
    local szMsg = string.format("<color=green>Nang cap thanh cong!<color>\n" ..
                  "Thuoc tinh #%d (Type %d): <color=yellow>%d -> %d<color> (+%d%%)",
                  (nAttribSlot + 1), nAttribType, nOldValue, nNewValue, nIncreasePercent)
    Talk(1, "", szMsg)
end

-- ────────────────────────────────────────────────────────────
-- Dynamic upgrade functions for each attribute slot (0-5)
-- These are called from the Say menu
-- ────────────────────────────────────────────────────────────
function DoUpgradeAttrib_0()
    PerformUpgrade(0)
end

function DoUpgradeAttrib_1()
    PerformUpgrade(1)
end

function DoUpgradeAttrib_2()
    PerformUpgrade(2)
end

function DoUpgradeAttrib_3()
    PerformUpgrade(3)
end

function DoUpgradeAttrib_4()
    PerformUpgrade(4)
end

function DoUpgradeAttrib_5()
    PerformUpgrade(5)
end

-- ────────────────────────────────────────────────────────────
-- Show Upgrade Guide
-- ────────────────────────────────────────────────────────────
function ShowGuide()
    local szGuide = "<color=yellow>=== HUONG DAN NANG CAP THUOC TINH ===<color>\n\n" ..
        "<color=cyan>Cach thuc hien:<color>\n" ..
        "1. Dat <color=blue>trang bi xanh<color> vao o tren\n" ..
        "2. Dat <color=orange>Da Nang Cap<color> vao o duoi\n" ..
        "3. Nhan nut <color=green>Nang Cap<color>\n" ..
        "4. Chon thuoc tinh muon nang cap tu menu\n" ..
        "5. Xac nhan de hoan tat\n\n" ..
        "<color=cyan>Chi tiet:<color>\n" ..
        "- Chi nang cap duoc <color=blue>trang bi xanh<color>\n" ..
        "- Moi lan nang tang <color=yellow>" .. UPGRADE_FIXED_PERCENT .. "%%<color> gia tri\n" ..
        "- Khong the vuot qua gia tri <color=red>MAX<color>\n" ..
        "- Ti le thanh cong: <color=green>" .. UPGRADE_SUCCESS_RATE .. "%%<color>\n" ..
        "- Mat <color=orange>1 Da Nang Cap<color> moi lan\n\n" ..
        "<color=cyan>Luu y:<color>\n" ..
        "- Thuoc tinh da MAX khong the nang them\n" ..
        "- Ban co the chon bat ky thuoc tinh nao de nang\n" ..
        "- Vat lieu bi mat khi nang cap\n" ..
        "- Khong the hoan tac sau khi nang"

    Talk(1, "", szGuide)
end

-- ────────────────────────────────────────────────────────────
-- Main Entry Point
-- ────────────────────────────────────────────────────────────
function main(NpcIndex)
    UpgradeAttribTalk()
end
