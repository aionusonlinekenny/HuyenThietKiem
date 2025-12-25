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

-- Attribute names (simple English for maximum compatibility)
ATTRIB_NAMES = {
    [28] = "Min Damage",
    [29] = "Max Damage",
    [30] = "Defense",
    [56] = "Accuracy",
    [85] = "Max HP",
    [89] = "Max MP",
    [93] = "Max Stamina",
    [97] = "Strength",
    [98] = "Dexterity",
    [99] = "Vitality",
    [100] = "Energy",
    [101] = "Poison Resist",
    [102] = "Fire Resist",
    [103] = "Lightning Resist",
    [104] = "Physical Resist",
    [105] = "Cold Resist",
    [114] = "All Resist",
    [115] = "Attack Speed",
    [121] = "Physical Damage",
    [122] = "Fire Damage",
    [123] = "Cold Damage",
    [124] = "Lightning Damage",
    [125] = "Poison Damage"
}

-- Get attribute display name
function GetAttribName(nAttribType)
    return ATTRIB_NAMES[nAttribType] or ("Type " .. nAttribType)
end

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
        Talk(1, "", "Chua dat trang bi vao!")
        return
    end

    -- Validate material exists
    if nMaterialIdx <= 0 then
        Talk(1, "", "Chua dat Da Nang Cap!")
        return
    end

    -- Validate material type (must be correct upgrade material)
    local nMatGenre, nMatDetail = GetItemProp(nMaterialIdx)

    if nMatGenre ~= UPGRADE_MATERIAL_GENRE or nMatDetail ~= UPGRADE_MATERIAL_DETAIL then
        Talk(1, "", "Vat lieu khong dung! Can su dung Da Nang Cap.")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    -- Check if equipment is blue (genre 0 with luck < 1000000000)
    if nGenre ~= 0 then
        Talk(1, "", "Chi co the nang cap trang bi xanh!")
        return
    end

    if nLuck >= 1000000000 then
        Talk(1, "", "Trang bi nay la tim/vang, khong the nang cap!")
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
        Talk(1, "", "Trang bi nay khong co thuoc tinh magic!")
        return
    end

    -- Build list of all upgradeable attributes
    local tbSayOptions = {}
    local nCount = 0

    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)

        if nAttribType and nAttribType > 0 then
            -- Check if this attribute can be upgraded
            if nMax <= 0 or nValue < nMax then
                -- Calculate potential new value
                local nIncrease = (nValue * UPGRADE_FIXED_PERCENT) / 100
                if nIncrease < 1 then nIncrease = 1 end
                local nNewValue = nValue + nIncrease
                if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

                -- Build option string with Vietnamese name (no tone marks)
                local szAttribName = GetAttribName(nAttribType)
                local szOption = szAttribName .. ": " .. nValue .. " -> " .. nNewValue .. "/DoUpgradeAttrib_" .. i

                -- Add to table
                tinsert(tbSayOptions, szOption)
                nCount = nCount + 1
            end
        end
    end

    -- Check if we have any upgradeable attributes
    if nCount == 0 then
        Talk(1, "", "Tat ca thuoc tinh da dat MAX!")
        return
    end

    -- Add cancel option
    tinsert(tbSayOptions, "Huy bo/no")

    -- Show selection menu
    Say("Chon thuoc tinh muon nang cap:", getn(tbSayOptions), tbSayOptions)
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
        Talk(1, "", "Loi: Trang bi hoac vat lieu bi mat!")
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
        Talk(1, "", "Loi: Khong the nang cap! Co the do rong hoac loi khac.")
        return
    end

    -- Delete material only (equipment already handled by C++ function)
    DelItemByIndex(nMaterialIdx)

    -- Success message
    local szAttribName = GetAttribName(nAttribType)
    local szMsg = "Nang cap thanh cong!\n" .. szAttribName .. ": " .. nOldValue .. " -> " .. nNewValue .. " (+" .. nIncreasePercent .. "%)"
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
    local szGuide = "=== HUONG DAN NANG CAP THUOC TINH ===\n\n" ..
        "Cach thuc hien:\n" ..
        "1. Dat trang bi xanh vao o tren\n" ..
        "2. Dat Da Nang Cap vao o duoi\n" ..
        "3. Nhan nut Nang Cap\n" ..
        "4. Chon thuoc tinh muon nang cap tu menu\n" ..
        "5. Xac nhan de hoan tat\n\n" ..
        "Chi tiet:\n" ..
        "- Chi nang cap duoc trang bi xanh\n" ..
        "- Moi lan nang tang 20% gia tri\n" ..
        "- Khong the vuot qua gia tri MAX\n" ..
        "- Ti le thanh cong: 100%\n" ..
        "- Mat 1 Da Nang Cap moi lan\n\n" ..
        "Luu y:\n" ..
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
