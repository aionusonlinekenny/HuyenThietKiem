-----------------------------------------------------------------
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Upgrade blue equipment attributes with minerals
-- Architecture: Lua = Game Logic, C++ = UI Presentation
-----------------------------------------------------------------

-- ------------------------------------------------------------
-- Configuration
-- ------------------------------------------------------------
UPGRADE_MATERIAL_GENRE = 6      -- item_task

-- Mineral Details (genre 6, particular 1/2/3 = level)
MINERAL_FIRE_DETAIL_MIN = 74    -- Hoa Nguyen Thach Level 1
MINERAL_FIRE_DETAIL_MAX = 76    -- Hoa Nguyen Thach Level 3
MINERAL_METAL_DETAIL_MIN = 77   -- Kim Linh Thach Level 1
MINERAL_METAL_DETAIL_MAX = 79   -- Kim Linh Thach Level 3
MINERAL_WOOD_DETAIL_MIN = 80    -- Moc Linh Thach Level 1
MINERAL_WOOD_DETAIL_MAX = 82    -- Moc Linh Thach Level 3
MINERAL_WATER_DETAIL_MIN = 83   -- Thuy Linh Thach Level 1
MINERAL_WATER_DETAIL_MAX = 85   -- Thuy Linh Thach Level 3
LUCKY_STONE_DETAIL = 86         -- Da May Man

-- Upgrade settings
UPGRADE_FIXED_PERCENT = 10      -- % tang co dinh
UPGRADE_COST_MONEY = 1000000    -- 100 van luong
UPGRADE_COST_XU = 2             -- 2 xu (task item ID 19)

-- ------------------------------------------------------------
-- Validation Helpers (Lua = Game Logic Layer)
-- ------------------------------------------------------------
function IsMineralItem(nGenre, nDetail)
    if nGenre ~= UPGRADE_MATERIAL_GENRE then
        return 0
    end

    if nDetail >= MINERAL_FIRE_DETAIL_MIN and nDetail <= MINERAL_WATER_DETAIL_MAX then
        return 1
    end

    return 0
end

function IsLuckyStone(nGenre, nDetail)
    if nGenre ~= UPGRADE_MATERIAL_GENRE then
        return 0
    end

    if nDetail == LUCKY_STONE_DETAIL then
        return 1
    end

    return 0
end

-- ------------------------------------------------------------
-- Get Vietnamese Attribute Name
-- ------------------------------------------------------------
function GetAttribName(nAttribType)
    -- Map attribute type to Vietnamese name
    local tbAttribNames = {
        [0] = "Khong ro",
        -- Sinh luc/Noi cong/Ngoai cong
        [2] = "Sinh luc toi da",
        [6] = "Noi cong",
        [5] = "Ngoai cong",
        -- Phong ngu
        [3] = "Phong ngu",
        -- He
        [27] = "Kim cong",
        [28] = "Moc cong",
        [29] = "Thuy cong",
        [30] = "Hoa cong",
        [31] = "Tho cong",
        -- Khang he
        [32] = "Kim khang",
        [33] = "Moc khang",
        [34] = "Thuy khang",
        [35] = "Hoa khang",
        [36] = "Tho khang",
        -- Cac loai khac
        [93] = "Luc",
        [85] = "Than phap",
        [123] = "May man",
        [126] = "Linh hoat",
        [169] = "Sinh menh"
    }

    return tbAttribNames[nAttribType] or ("Thuoc tinh #" .. nAttribType)
end

-- ------------------------------------------------------------
-- NPC Talk Entry Point
-- ------------------------------------------------------------
function UpgradeAttribTalk()
    OpenUpgradeAttribUI()  -- Call C++ function to open UI
end

-- ------------------------------------------------------------
-- Execute Upgrade (called when player clicks "Upgrade" button)
-- This validates items and shows attribute selection menu
-- ------------------------------------------------------------
function ExeUpgradeAttrib()
    Msg2Player("DEBUG: ExeUpgradeAttrib called")

    local nPos = 15  -- pos_builditem (same container as Tremble)

    -- Get equipment from slot 0
    local nEquipIdx = GetPOItem(nPos, 0)

    -- Validate equipment exists
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "Chua dat trang bi vao!")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    if not nGenre then
        Talk(1, "", "Loi: Khong doc duoc thong tin trang bi!")
        return
    end

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

    -- Show attribute selection menu
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
-- Perform Upgrade (called after player selects attribute)
-- IMPORTANT: Materials + money + xu are consumed BEFORE upgrade
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
    local nMineralsUsed = 0
    for i = 1, 5 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            DelItemByIndex(nItemIdx)
            nMineralsUsed = nMineralsUsed + 1
        end
    end

    -- IMPORTANT: Deduct money and xu BEFORE upgrade attempt
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
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)

    if nNewItemIdx == 0 then
        Talk(1, "", "Loi: Khong the nang cap! (Luu y: Vat lieu va tien da bi tru)")
        return
    end

    -- Success message
    local szAttribName = GetAttribName(nAttribType)
    local szMsg = "Nang cap thanh cong!\n" .. szAttribName .. ": " .. nOldValue .. " -> " .. nNewValue .. " (+" .. nIncreasePercent .. "%)\nDa tru: "..nMineralsUsed.." khoang thach, 1,000,000 luong + 2 xu"
    Talk(1, "", szMsg)
end

-- ------------------------------------------------------------
-- Dynamic upgrade functions for each attribute slot (0-5)
-- These are called from the SayImage menu options
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
-- NPC main function (called when player talks to NPC)
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end
