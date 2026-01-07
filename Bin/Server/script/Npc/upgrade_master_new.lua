-----------------------------------------------------------------
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Opens equipment attribute upgrade UI with NPC avatar
-- Date: 2025-12-21
-----------------------------------------------------------------

-- NOTE: Removed Include("\\script\\lib\\TaskLib.lua") because:
-- 1. Script doesn't use any functions from TaskLib
-- 2. Include() may not be available in NPC script context
-- 3. Caused "attempt to call a nil value" error

-- ------------------------------------------------------------
-- Configuration - Item IDs
-- ------------------------------------------------------------
UPGRADE_MATERIAL_GENRE = 6      -- item_task

-- Mineral Details (all are genre 6 = item_task, particular 1/2/3 = level)
MINERAL_FIRE_DETAIL_MIN = 74    -- Hoa Nguyen Thach Level 1
MINERAL_FIRE_DETAIL_MAX = 76    -- Hoa Nguyen Thach Level 3
MINERAL_METAL_DETAIL_MIN = 77   -- Kim Linh Thach Level 1
MINERAL_METAL_DETAIL_MAX = 79   -- Kim Linh Thach Level 3
MINERAL_WOOD_DETAIL_MIN = 80    -- Moc Linh Thach Level 1
MINERAL_WOOD_DETAIL_MAX = 82    -- Moc Linh Thach Level 3
MINERAL_WATER_DETAIL_MIN = 83   -- Thuy Linh Thach Level 1
MINERAL_WATER_DETAIL_MAX = 85   -- Thuy Linh Thach Level 3
LUCKY_STONE_DETAIL = 86         -- Da May Man

-- Helper function to check if item is a valid mineral
function IsMineralItem(nGenre, nDetail)
    if nGenre ~= UPGRADE_MATERIAL_GENRE then
        return 0
    end
    -- Check if detail is in any mineral range (74-85)
    if nDetail >= MINERAL_FIRE_DETAIL_MIN and nDetail <= MINERAL_WATER_DETAIL_MAX then
        return 1
    end
    return 0
end

-- Helper function to check if item is lucky stone
function IsLuckyStone(nGenre, nDetail)
    return (nGenre == UPGRADE_MATERIAL_GENRE and nDetail == LUCKY_STONE_DETAIL)
end

-- Upgrade settings
UPGRADE_FIXED_PERCENT = 20      -- % tang co dinh (FIXED 20%)
UPGRADE_SUCCESS_RATE = 100      -- Ti le thanh cong (%)

-- Cost requirements (these values are also defined in C++ for UI display)
UPGRADE_COST_MONEY = 1000000    -- 100 van luong
UPGRADE_COST_XU = 2             -- 2 xu

-- Attribute names (Vietnamese without tone marks, GLOBAL for old Lua compatibility)
ATTRIB_NAMES = {
    [28]  = "S¸t th­¬ng nhá nhÊt",
    [29]  = "S¸t th­¬ng lín nhÊt",
    [30]  = "NÐ tr¸nh",
    [56]  = "§é chÝnh x¸c",
    [57]  = "§é chÝnh x¸c",
    [59]  = "S¸t th­¬ng vËt lý",
    [60]  = "B¨ng s¸t",
    [61]  = "Hãa s¸t",
    [62]  = "L«i s¸t",
    [63]  = "§éc s¸t",
    [64]  = "S¸t th­¬ng ngò hµnh",
    [70]  = "S¸t th­¬ng vËt lý",
    [85]  = "Sinh lùc tèi ®a",
    [86]  = "Sinh lùc tèi ®a",
    [87]  = "Sinh lùc",
    [88]  = "Phôc håi sinh lùc",
    [89]  = "Néi lùc tèi ®a",
    [90]  = "Néi lùc tèi ®a",
    [91]  = "Néi lùc",
    [92]  = "Phôc håi néi lùc",
    [93]  = "ThÓ lùc tèi ®a",
    [94]  = "ThÓ lùc tèi ®a",
    [95]  = "ThÓ lùc",
    [97]  = "Søc m¹nh",
    [98]  = "Th©n ph¸p",
    [99]  = "Sinh khÝ",
    [100] = "Néi c«ng",
    [101] = "Kh¸ng ®éc",
    [102] = "Kh¸ng hãa",
    [103] = "Kh¸ng l«i",
    [104] = "Phßng thñ vËt lý",
    [105] = "Kh¸ng b¨ng",
    [106] = "Gi¶m thêi gian lµm chËm",
    [107] = "Gi¶m thêi gian ®èt",
    [108] = "Gi¶m thêi gian tróng ®éc",
    [110] = "Gi¶m thêi gian cho¸ng",
    [111] = "Tèc ®é di chuyÓn",
    [113] = "Thêi gian phôc håi",
    [114] = "Kh¸ng tÊt c¶",
    [115] = "Tèc ®é ®¸nh ngo¹i c«ng",
    [116] = "Tèc ®é ®¸nh néi c«ng",
    [121] = "S¸t th­¬ng vËt lý ngo¹i c«ng",
    [122] = "Hãa s¸t ngo¹i c«ng",
    [123] = "B¨ng s¸t ngo¹i c«ng",
    [124] = "L«i s¸t ngo¹i c«ng",
    [125] = "§éc s¸t ngo¹i c«ng",
    [126] = "S¸t th­¬ng vËt lý phÇn tr¨m",
    [128] = "Mê hoÆc ®èi ph­¬ng",
    [129] = "Phßng thñ vËt lý",
    [150] = "NÐ tr¸nh",
    [151] = "NÐ tr¸nh phÇn tr¨m",
    [166] = "Tû lÖ tÊn c«ng chÝnh x¸c",
    [167] = "Tû lÖ tÊn c«ng chÝnh x¸c phÇn tr¨m",
    [168] = "S¸t th­¬ng vËt lý néi c«ng",
    [169] = "B¨ng s¸t néi c«ng",
    [170] = "Hãa s¸t néi c«ng",
    [171] = "L«i s¸t néi c«ng",
    [172] = "§éc s¸t néi c«ng"
}

-- Get attribute display name
function GetAttribName(nAttribType)
    return ATTRIB_NAMES[nAttribType] or ("Type " .. nAttribType)
end

-- ------------------------------------------------------------
-- NPC Talk Entry Point
-- -----------------------------------------------------------
function UpgradeAttribTalk()
    -- CHANGED: Say() -> SayImage() with ImageKey 40
    SayImage(
        "Cao thu ren duc: Ta co the giup nguoi nang cap tung thuoc tinh cua trang bi xanh!",
        "10/20/40",  -- ImageKey 40 = enemy169_st.spr
        3,
        "Nang cap thuoc tinh trang bi xanh/OpenUpgradeUI",
        "Huong dan nang cap/ShowGuide",
        "Ta chi ghe ngang qua/no"
    )
end

-- ------------------------------------------------------------
-- Open Upgrade UI
-- ------------------------------------------------------------
function OpenUpgradeUI()
    OpenUpgradeAttribUI()  -- Call C++ function to open UI
end

-- ------------------------------------------------------------
-- Execute Upgrade (called when player clicks "Upgrade" button)
-- This function validates items and shows attribute selection menu
-- ------------------------------------------------------------
function ExeUpgradeAttrib()
    -- DEBUG: Function entry
    Msg2Player("DEBUG: ExeUpgradeAttrib called")
    -- WriteLog removed (function not available)

    local nPos = 15  -- pos_builditem (same container as Tremble)

    -- Get equipment from slot 0
    local nEquipIdx = GetPOItem(nPos, 0)

    -- Validate equipment exists
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "Chua dat trang bi vao!")
    -- WriteLog removed (function not available)
        return
    end

    -- WriteLog removed (function not available)

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    if not nGenre then
        Talk(1, "", "Loi: Khong doc duoc thong tin trang bi!")
    -- WriteLog removed (function not available)
        return
    end

    -- WriteLog removed (function not available)

    -- Check if equipment is blue (genre 0 with luck < 1000000000)
    if nGenre ~= 0 then
    -- WriteLog removed (function not available)
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

    -- Check money and xu requirements FIRST (before validating minerals)
    local nPlayerMoney = GetCash()
    local nPlayerXu = GetTaskItemCount(19)  -- Xu = task item ID 19

    if nPlayerMoney < UPGRADE_COST_MONEY then
        Talk(1, "", "Khong du tien! Ban can co it nhat 1,000,000 luong de nang cap.")
        return
    end

    if nPlayerXu < UPGRADE_COST_XU then
        Talk(1, "", "Khong du xu! Ban can co it nhat 2 xu de nang cap.")
        return
    end

    -- Validate minerals in slots 1-4 (if present)
    -- NOTE: Minerals are optional but if present, must be valid
    for i = 1, 4 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            local nGenre, nDetail = GetItemProp(nItemIdx)
            if nGenre then
                -- Use Lua validation function to check if valid mineral
                if IsMineralItem(nGenre, nDetail) == 0 then
                    Talk(1, "", "Khoang thach khong hop le! Chi chap nhan Hoa/Kim/Moc/Thuy Linh Thach (Cap 1-3).")
                    return
                end
            end
        end
    end

    -- Validate lucky stone in slot 5 (if present)
    local nLuckyStoneIdx = GetPOItem(nPos, 5)
    if nLuckyStoneIdx and nLuckyStoneIdx > 0 then
        local nGenre, nDetail = GetItemProp(nLuckyStoneIdx)
        if nGenre then
            -- Use Lua validation function to check if valid lucky stone
            if IsLuckyStone(nGenre, nDetail) == 0 then
                Talk(1, "", "Chi chap nhan Da May Man o vi tri nay!")
                return
            end
        end
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

                -- Build option string with Vietnamese name
                local szAttribName = GetAttribName(nAttribType)
                local szOption = szAttribName .. ": " .. nValue .. " -> " .. nNewValue .. "/DoUpgradeAttrib_" .. i

                -- Add to table (old Lua style - no tinsert)
                nCount = nCount + 1
                tbSayOptions[nCount] = szOption
            end
        end
    end

    -- Check if we have any upgradeable attributes
    if nCount == 0 then
        Talk(1, "", "Tat ca thuoc tinh da dat MAX!")
        return
    end

    -- Add cancel option (old Lua style - no tinsert)
    nCount = nCount + 1
    tbSayOptions[nCount] = "Huy bo/no"

    -- CHANGED: Say() -> SayImage() with ImageKey 40
    SayImage(
        "Chon thuoc tinh muon nang cap:",
        "10/20/40",  -- ImageKey 40 = enemy169_st.spr
        getn(tbSayOptions),
        tbSayOptions[1] or "",
        tbSayOptions[2] or "",
        tbSayOptions[3] or "",
        tbSayOptions[4] or "",
        tbSayOptions[5] or "",
        tbSayOptions[6] or "",
        tbSayOptions[7] or ""
    )
end

-- ------------------------------------------------------------
-- Perform actual upgrade on selected attribute slot
-- Called by dynamic functions DoUpgradeAttrib_0 through DoUpgradeAttrib_5
-- ------------------------------------------------------------
function PerformUpgrade(nAttribSlot)
    local nPos = 15  -- pos_builditem

    -- Get equipment (must exist)
    local nEquipIdx = GetPOItem(nPos, 0)
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "Loi: Trang bi bi mat!")
        return
    end

    -- Get attribute info for the selected slot
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

    if not nAttribType or nAttribType <= 0 then
        Talk(1, "", "Loi: Khong tim thay thuoc tinh!")
        return
    end

    -- IMPORTANT: Delete materials FIRST (before upgrade attempt)
    -- Minerals and lucky stone are consumed regardless of upgrade success
    -- WriteLog removed (function not available)
    local nMineralsUsed = 0
    for i = 1, 5 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            DelItemByIndex(nItemIdx)
            nMineralsUsed = nMineralsUsed + 1
    -- WriteLog removed (function not available)
        end
    end

    -- IMPORTANT: Deduct money and xu BEFORE upgrade attempt
    -- WriteLog removed (function not available)
    Spend(UPGRADE_COST_MONEY, "Nang cap trang bi xanh")
    DelTaskItem(19, UPGRADE_COST_XU)  -- Delete xu (task item ID 19)

    -- Use FIXED upgrade percentage
    local nIncreasePercent = UPGRADE_FIXED_PERCENT

    -- Calculate new value
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    -- Now perform upgrade (materials and costs already paid)
    -- WriteLog removed (function not available)
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)

    if nNewItemIdx == 0 then
        Talk(1, "", "Loi: Khong the nang cap! (Luu y: Vat lieu va tien da bi tru)")
    -- WriteLog removed (function not available)
        return
    end

    -- Success message
    local szAttribName = GetAttribName(nAttribType)
    local szMsg = "Nang cap thanh cong!\n" .. szAttribName .. ": " .. nOldValue .. " -> " .. nNewValue .. " (+" .. nIncreasePercent .. "%)\nDa tru: "..nMineralsUsed.." khoang thach, 1,000,000 luong + 2 xu"
    Talk(1, "", szMsg)
    -- WriteLog removed (function not available)
end

-- ------------------------------------------------------------
-- Dynamic upgrade functions for each attribute slot (0-5)
-- These are called from the SayImage menu
-- ------------------------------------------------------------
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

-- ------------------------------------------------------------
-- Cancel/Exit function
-- ------------------------------------------------------------
function no()
    -- Do nothing, dialog will close automatically
end

-- ------------------------------------------------------------
-- Show Upgrade Guide
-- ------------------------------------------------------------
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

    -- CHANGED: Talk() -> SayImage() with ImageKey 40
    SayImage(
        szGuide,
        "10/20/40",  -- ImageKey 40 = enemy169_st.spr
        0            -- No options, auto-close when clicked
    )
end

-- ------------------------------------------------------------
-- Main Entry Point
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end
