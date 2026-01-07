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
-- Perform Upgrade (called directly from C++ with attribute slot)
-- C++ sends: PerformUpgrade#X where X is the attribute slot (0-5)
-- IMPORTANT: Materials + money + xu are consumed BEFORE upgrade
-- ------------------------------------------------------------
function PerformUpgrade(_, nAttribSlot)
    -- DEBUG: Function entry
    Msg2Player("[LUA DEBUG 1] PerformUpgrade function entered")
    Msg2Player("[LUA DEBUG 2] Raw parameter received: " .. tostring(nAttribSlot))

    -- Parse attribute slot from parameter
    nAttribSlot = tonumber(nAttribSlot)
    if not nAttribSlot then
        Msg2Player("[LUA ERROR] Invalid attribute slot parameter")
        return
    end

    Msg2Player("[LUA DEBUG 3] Parsed slot: " .. nAttribSlot)

    local nPos = 15  -- pos_builditem
    Msg2Player("[LUA DEBUG 4] About to call GetPOItem(15, 0)")

    -- Get equipment (must exist)
    local nEquipIdx = GetPOItem(nPos, 0)
    Msg2Player("[LUA DEBUG 5] GetPOItem returned: " .. tostring(nEquipIdx))

    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "Loi: Trang bi bi mat!")
        Msg2Player("[LUA DEBUG 6] Equipment not found in slot")
        return
    end

    Msg2Player("[LUA DEBUG 7] Equipment found, index: " .. nEquipIdx)

    -- Validate equipment is blue
    Msg2Player("[LUA DEBUG 8] Calling GetItemProp")
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    Msg2Player("[LUA DEBUG 9] GetItemProp - Genre: " .. tostring(nGenre) .. ", Luck: " .. tostring(nLuck))

    if not nGenre or nGenre ~= 0 then
        Talk(1, "", "Chi co the nang cap trang bi xanh!")
        Msg2Player("[LUA DEBUG 10] Equipment is not blue (genre != 0)")
        return
    end

    if nLuck >= 1000000000 then
        Talk(1, "", "Trang bi nay la tim/vang, khong the nang cap!")
        Msg2Player("[LUA DEBUG 11] Equipment is purple/gold (luck >= 1000000000)")
        return
    end

    Msg2Player("[LUA DEBUG 12] Equipment validation passed")

    -- Validate minerals in slots 1-4 (if present)
    for i = 1, 4 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            local nGenre, nDetail = GetItemProp(nItemIdx)
            if nGenre then
                if IsMineralItem(nGenre, nDetail) == 0 then
                    Talk(1, "", "Khoang thach khong hop le!")
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
            if IsLuckyStone(nGenre, nDetail) == 0 then
                Talk(1, "", "Chi chap nhan Da May Man o vi tri nay!")
                return
            end
        end
    end

    -- Check money and xu requirements
    local nPlayerMoney = GetCash()
    local nPlayerXu = GetTaskItemCount(19)

    if nPlayerMoney < UPGRADE_COST_MONEY then
        Talk(1, "", "Khong du tien! Ban can co it nhat 1,000,000 luong.")
        return
    end

    if nPlayerXu < UPGRADE_COST_XU then
        Talk(1, "", "Khong du xu! Ban can co it nhat 2 xu.")
        return
    end

    -- Get attribute info for the selected slot
    Msg2Player("[LUA DEBUG 13] About to call GetItemMagicAttribInfo with ItemIdx=" .. nEquipIdx .. ", Slot=" .. nAttribSlot)
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
    Msg2Player("[LUA DEBUG 14] GetItemMagicAttribInfo returned - Type: " .. tostring(nAttribType) .. ", Value: " .. tostring(nOldValue) .. ", Min: " .. tostring(nMin) .. ", Max: " .. tostring(nMax))

    if not nAttribType or nAttribType <= 0 then
        Talk(1, "", "Loi: Khong tim thay thuoc tinh!")
        Msg2Player("[LUA DEBUG 15] Attribute type invalid or not found")
        return
    end

    Msg2Player("[LUA DEBUG 16] Attribute info retrieved successfully")

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
    Pay(UPGRADE_COST_MONEY)  -- Pay() deducts money
    DelTaskItem(19, UPGRADE_COST_XU)  -- Delete xu (task item ID 19)

    -- Use FIXED upgrade percentage
    local nIncreasePercent = UPGRADE_FIXED_PERCENT

    -- Calculate new value
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    -- Now perform upgrade (materials and costs already paid)
    Msg2Player("[LUA DEBUG 17] About to call UpgradeItemAttributes with Slot=" .. nAttribSlot .. ", Percent=" .. nIncreasePercent)
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)
    Msg2Player("[LUA DEBUG 18] UpgradeItemAttributes returned: " .. tostring(nNewItemIdx))

    local szAttribName = GetAttribName(nAttribType)

    if nNewItemIdx == 0 then
        -- FAILURE: Upgrade failed
        Msg2Player("[LUA DEBUG 19] Upgrade FAILED")
        local szFailMsg = "<color=red>[THAT BAI]<color> Nang cap that bai!\n" ..
                          szAttribName .. ": " .. nOldValue .. " (khong doi)\n" ..
                          "Da mat: " .. nMineralsUsed .. " khoang thach, 1,000,000 luong + 2 xu"
        Talk(1, "", szFailMsg)
        Msg2Player(szFailMsg)
        return
    end

    -- SUCCESS: Upgrade succeeded
    Msg2Player("[LUA DEBUG 20] Upgrade SUCCESS")
    local szSuccessMsg = "<color=green>[THANH CONG]<color> Nang cap thanh cong!\n" ..
                         szAttribName .. ": " .. nOldValue .. " -> " .. nNewValue .. " (+" .. nIncreasePercent .. "%)\n" ..
                         "Da tru: " .. nMineralsUsed .. " khoang thach, 1,000,000 luong + 2 xu"
    Talk(1, "", szSuccessMsg)
    Msg2Player(szSuccessMsg)
end

-- ------------------------------------------------------------
-- NPC main function (called when player talks to NPC)
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end
