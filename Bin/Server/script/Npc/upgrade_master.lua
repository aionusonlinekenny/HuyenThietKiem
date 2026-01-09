-----------------------------------------------------------------
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Upgrade blue equipment attributes with minerals
-- Architecture: Lua = Game Logic, C++ = UI Presentation
-----------------------------------------------------------------

-- ------------------------------------------------------------
-- Helper Functions
-- ------------------------------------------------------------
-- Floor function (Lua 5.0 doesn't have math library or % operator)
function floor(n)
    if n >= 0 then
        -- For positive numbers, truncate decimal part
        local int_part = 0
        while int_part + 1 <= n do
            int_part = int_part + 1
        end
        return int_part
    else
        -- For negative numbers, round down
        local int_part = 0
        while int_part - 1 >= n do
            int_part = int_part - 1
        end
        return int_part
    end
end

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

-- Success rate settings
UPGRADE_BASE_SUCCESS_RATE = 20  -- 30% co ban
MINERAL_LEVEL1_BONUS = 5        -- +5% per level 1 mineral
MINERAL_LEVEL2_BONUS = 10       -- +10% per level 2 mineral
MINERAL_LEVEL3_BONUS = 15       -- +15% per level 3 mineral
LUCKY_STONE_BONUS = 100          -- +10% from lucky stone

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
function PerformUpgrade(nAttribSlotStr, _)
    -- DEBUG: Function entry
    print("[LUA-UPGRADE] 1] PerformUpgrade function entered")
    print("[LUA-UPGRADE] 2] Raw parameter received: " .. tostring(nAttribSlotStr))

    -- Parse attribute slot from parameter (C++ sends as string in first param, 0 in second)
    local nAttribSlot = tonumber(nAttribSlotStr)
    if not nAttribSlot then
        Msg2Player("[LUA ERROR] Invalid attribute slot parameter")
        return
    end

    print("[LUA-UPGRADE] 3] Parsed slot: " .. nAttribSlot)

    local nPos = 15  -- pos_builditem
    print("[LUA-UPGRADE] 4] About to call GetPOItem(15, 0)")

    -- Get equipment (must exist)
    local nEquipIdx = GetPOItem(nPos, 0)
    print("[LUA-UPGRADE] 5] GetPOItem returned: " .. tostring(nEquipIdx))

    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "Loi: Trang bi bi mat!")
        print("[LUA-UPGRADE] 6] Equipment not found in slot")
        return
    end

    print("[LUA-UPGRADE] 7] Equipment found, index: " .. nEquipIdx)

    -- Validate equipment is blue
    print("[LUA-UPGRADE] 8] Calling GetItemProp")
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    print("[LUA-UPGRADE] 9] GetItemProp - Genre: " .. tostring(nGenre) .. ", Luck: " .. tostring(nLuck))

    if not nGenre or nGenre ~= 0 then
        Talk(1, "", "Chi co the nang cap trang bi xanh!")
        print("[LUA-UPGRADE] 10] Equipment is not blue (genre != 0)")
        return
    end

    if nLuck >= 1000000000 then
        Msg2Player("Trang bi nay la tim/vang, khong the nang cap!")
        print("[LUA-UPGRADE] 11] Equipment is purple/gold (luck >= 1000000000)")
        return
    end

    print("[LUA-UPGRADE] 12] Equipment validation passed")

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
                Msg2Player("Chi chap nhan Da May Man o vi tri nay!")
                return
            end
        end
    end

    -- Check money and xu requirements
    local nPlayerMoney = GetCash()
    local nPlayerXu = GetTaskItemCount(19)

    if nPlayerMoney < UPGRADE_COST_MONEY then
        Msg2Player("Khong du tien! Ban can co it nhat 1,000,000 luong.")
        return
    end

    if nPlayerXu < UPGRADE_COST_XU then
        Msg2Player("Khong du xu! Ban can co it nhat 2 xu.")
        return
    end

    -- Get attribute info for the selected slot
    print("[LUA-UPGRADE] 13] About to call GetItemMagicAttribInfo with ItemIdx=" .. nEquipIdx .. ", Slot=" .. nAttribSlot)
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
    print("[LUA-UPGRADE] 14] GetItemMagicAttribInfo returned - Type: " .. tostring(nAttribType) .. ", Value: " .. tostring(nOldValue) .. ", Min: " .. tostring(nMin) .. ", Max: " .. tostring(nMax))

    if not nAttribType or nAttribType <= 0 then
        Msg2Player("Loi: Khong tim thay thuoc tinh!")
        print("[LUA-UPGRADE] 15] Attribute type invalid or not found")
        return
    end

    print("[LUA-UPGRADE] 16] Attribute info retrieved successfully")

    -- STEP 1: Calculate success rate BEFORE deleting materials
    -- Count minerals and their levels
    local nSuccessRate = UPGRADE_BASE_SUCCESS_RATE
    local nMineralCount = 0

    for i = 1, 4 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            local nGenre, nDetail, nParti = GetItemProp(nItemIdx)
            if nGenre then
                -- Get mineral level from particular (1/2/3)
                local nMineralLevel = nParti
                if nMineralLevel == 1 then
                    nSuccessRate = nSuccessRate + MINERAL_LEVEL1_BONUS
                elseif nMineralLevel == 2 then
                    nSuccessRate = nSuccessRate + MINERAL_LEVEL2_BONUS
                elseif nMineralLevel == 3 then
                    nSuccessRate = nSuccessRate + MINERAL_LEVEL3_BONUS
                end
                nMineralCount = nMineralCount + 1
            end
        end
    end

    -- Check lucky stone
    local bHasLuckyStone = 0
    local nLuckyStoneIdx = GetPOItem(nPos, 5)
    if nLuckyStoneIdx and nLuckyStoneIdx > 0 then
        nSuccessRate = nSuccessRate + LUCKY_STONE_BONUS
        bHasLuckyStone = 1
    end

    print("[LUA-UPGRADE] 17] Success rate calculated: " .. nSuccessRate .. "% (base=" .. UPGRADE_BASE_SUCCESS_RATE .. ", minerals=" .. nMineralCount .. ", lucky=" .. bHasLuckyStone .. ")")

    -- STEP 2: Delete materials and deduct costs (consumed regardless of success)
    local nMineralsUsed = 0
    for i = 1, 5 do
        local nItemIdx = GetPOItem(nPos, i)
        if nItemIdx and nItemIdx > 0 then
            DelItemByIndex(nItemIdx)
            nMineralsUsed = nMineralsUsed + 1
        end
    end

    Pay(UPGRADE_COST_MONEY)
    DelTaskItem(19, UPGRADE_COST_XU)

    -- STEP 3: Random check for success/failure
    local nRoll = random(1, 100)  -- Random 1-100
    local bUpgradeSuccess = (nRoll <= nSuccessRate)

    print("[LUA-UPGRADE] 18] Random roll: " .. nRoll .. " vs success rate: " .. nSuccessRate .. "% => " .. (bUpgradeSuccess and "SUCCESS" or "FAIL"))

    local szAttribName = GetAttribName(nAttribType)

    if not bUpgradeSuccess then
        -- FAILURE: Equipment keeps original value - NO ACTION NEEDED
        -- DON'T call UpgradeItemAttributes (can return 0 and lose item)
        -- Equipment stays in BUILD_CONTAINER unchanged
        print("[LUA-UPGRADE] 19] Upgrade FAILED - equipment unchanged (no item modification)")

        local szFailMsg = "<color=red>[THAT BAI]<color> Nang cap that bai! " ..
                          szAttribName .. ": " .. nOldValue .. " (khong doi). " ..
                          "Da mat: " .. nMineralsUsed .. " khoang thach + tien"
        Msg2Player(szFailMsg)
        print("[LUA-UPGRADE] 19d] Failure handling complete")
        return
    end

    -- STEP 4: SUCCESS - Perform actual upgrade
    -- Use UpgradeItemAttributes C++ function (same as old working code)
    local nIncreasePercent = UPGRADE_FIXED_PERCENT

    -- Calculate new value for display message
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    print("[LUA-UPGRADE] 20] Calling UpgradeItemAttributes with Slot=" .. nAttribSlot .. ", Percent=" .. nIncreasePercent)

    -- Call UpgradeItemAttributes with pos=15 (BUILD_CONTAINER) - same as old code
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)
    print("[LUA-UPGRADE] 21] UpgradeItemAttributes returned: " .. tostring(nNewItemIdx))

    if nNewItemIdx == 0 then
        -- Technical error during upgrade
        print("[LUA-UPGRADE] 22] ERROR: UpgradeItemAttributes failed")
        local szErrorMsg = "<color=red>Loi ky thuat:<color> <color=yellow>Khong the tra lai trang bi! Lien he GM.<color>"
        Msg2Player(szErrorMsg)
        return
    end

    -- SUCCESS
    print("[LUA-UPGRADE] 23] Upgrade SUCCESS")
    local szSuccessMsg = "<color=green>[THANH CONG]<color> <color=yellow>Nang cap thanh cong! " ..
                         szAttribName .. ": " .. nOldValue .. " -> " .. nNewValue .. " (+" .. nIncreasePercent .. "%). " ..
                         "Da tru: " .. nMineralsUsed .. " khoang thach, 1,000,000 luong + 2 xu<color>"

    Msg2Player(szSuccessMsg)
end

-- ------------------------------------------------------------
-- NPC main function (called when player talks to NPC)
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end