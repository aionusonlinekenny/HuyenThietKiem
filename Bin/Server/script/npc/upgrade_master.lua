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

-- Attribute names in Vietnamese (no tone marks = pure ASCII, safe for old Lua parser)
local ATTRIB_NAMES = {
    -- Weapon damage
    [28] = "Sat thuong nho nhat",
    [29] = "Sat thuong lon nhat",
    [30] = "Ne tranh",

    -- Attack attributes
    [56] = "Do chinh xac",
    [57] = "Do chinh xac",
    [59] = "Sat thuong vat ly",
    [60] = "Bang sat",
    [61] = "Hoa sat",
    [62] = "Loi sat",
    [63] = "Doc sat",
    [64] = "Sat thuong ngu hanh",
    [70] = "Sat thuong vat ly",

    -- Life/Mana/Stamina
    [85] = "Sinh luc toi da",
    [86] = "Sinh luc toi da",
    [87] = "Sinh luc",
    [88] = "Phuc hoi sinh luc",
    [89] = "Noi luc toi da",
    [90] = "Noi luc toi da",
    [91] = "Noi luc",
    [92] = "Phuc hoi noi luc",
    [93] = "The luc toi da",
    [94] = "The luc toi da",
    [95] = "The luc",

    -- Base attributes
    [97] = "Suc manh",
    [98] = "Than phap",
    [99] = "Sinh khi",
    [100] = "Noi cong",

    -- Resistances
    [101] = "Khang doc",
    [102] = "Khang hoa",
    [103] = "Khang loi",
    [104] = "Phong thu vat ly",
    [105] = "Khang bang",
    [106] = "Giam thoi gian lam cham",
    [108] = "Giam thoi gian trung doc",
    [110] = "Giam thoi gian choang",
    [111] = "Toc do di chuyen",
    [113] = "Thoi gian phuc hoi",
    [114] = "Khang tat ca",
    [115] = "Toc do danh - ngoai cong",
    [116] = "Toc do danh - noi cong",

    -- External damage
    [121] = "Sat thuong vat ly - ngoai cong",
    [122] = "Hoa sat - ngoai cong",
    [123] = "Bang sat - ngoai cong",
    [124] = "Loi sat - ngoai cong",
    [125] = "Doc sat - ngoai cong",
    [126] = "Sat thuong vat ly %",

    -- Special
    [128] = "Mo hoac doi phuong",
    [129] = "Phong thu vat ly",
    [150] = "Ne tranh",
    [151] = "Ne tranh %",
    [166] = "Ti le cong kich chinh xac",
    [167] = "Ti le cong kich chinh xac %",

    -- Internal damage
    [168] = "Sat thuong vat ly - noi cong",
    [169] = "Bang sat - noi cong",
    [170] = "Hoa sat - noi cong",
    [171] = "Loi sat - noi cong",
    [172] = "Doc sat - noi cong",
}

-- Get attribute display name
local function GetAttribName(nAttribType)
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
