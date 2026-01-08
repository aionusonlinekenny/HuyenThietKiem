-----------------------------------------------------------------
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Upgrade blue equipment attributes with minerals
-- Architecture: Lua = Game Logic, C++ = UI Presentation
-----------------------------------------------------------------

-- CRITICAL: Global-level log to verify file is loaded
print("[LUA FILE LOAD] upgrade_master.lua is being loaded/executed")

-- Also try to send to player if possible (may not work at file load time)
-- This will execute when file is loaded, BEFORE any function is called

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
UPGRADE_BASE_SUCCESS_RATE = 30  -- 30% co ban
MINERAL_LEVEL1_BONUS = 5        -- +5% per level 1 mineral
MINERAL_LEVEL2_BONUS = 10       -- +10% per level 2 mineral
MINERAL_LEVEL3_BONUS = 15       -- +15% per level 3 mineral
LUCKY_STONE_BONUS = 10          -- +10% from lucky stone

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
    print("[LUA-UPGRADE] PerformUpgrade called with param: " .. tostring(nAttribSlotStr))

    -- Parse attribute slot from string
    local nAttribSlot = tonumber(nAttribSlotStr)
    if not nAttribSlot then
        Msg2Player("Lỗi: Không thể xác định thuộc tính cần nâng cấp")
        print("[LUA-UPGRADE] ERROR: Invalid attribute slot parameter")
        return
    end

    print("[LUA-UPGRADE] Attribute slot to upgrade: " .. nAttribSlot)

    -- Step 1: Get equipment from build container
    local nEquipIdx = GetPOItem(15, 0)  -- Container 15 (pos_builditem), slot 0
    if not nEquipIdx or nEquipIdx <= 0 then
        Msg2Player("Lỗi: Không tìm thấy trang bị trong ô nâng cấp")
        print("[LUA-UPGRADE] ERROR: Equipment not found in build container")
        return
    end

    print("[LUA-UPGRADE] Equipment index: " .. nEquipIdx)

    -- Step 2: Get equipment info
    local nGenre, nDetail, nParticular, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    print(string.format("[LUA-UPGRADE] Equipment: genre=%d, detail=%d, particular=%d, level=%d, series=%d, luck=%d",
        nGenre or -1, nDetail or -1, nParticular or -1, nLevel or -1, nSeries or -1, nLuck or -1))

    -- Step 3: Get current attribute value
    local nAttribType, nCurrentValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
    if not nAttribType or nAttribType <= 0 or not nCurrentValue or nCurrentValue <= 0 then
        Msg2Player("Thuộc tính này chưa có giá trị, không thể nâng cấp")
        print("[LUA-UPGRADE] ERROR: Attribute slot " .. nAttribSlot .. " has no value")
        return
    end

    print(string.format("[LUA-UPGRADE] Current attribute slot %d: type=%d, value=%d, min=%d, max=%d",
        nAttribSlot, nAttribType, nCurrentValue, nMin or 0, nMax or 0))

    -- Step 4: Calculate success rate (use base rate for now)
    local nSuccessRate = UPGRADE_BASE_SUCCESS_RATE

    print("[LUA-UPGRADE] Success rate: " .. nSuccessRate .. "%")

    -- Step 5: Random check for success/failure
    local nRandom = random(1, 100)
    local bSuccess = (nRandom <= nSuccessRate)

    print(string.format("[LUA-UPGRADE] Random roll: %d/%d, Success: %s", nRandom, nSuccessRate, tostring(bSuccess)))

    -- Step 6: Perform upgrade
    local nUpgradePercent = 0
    if bSuccess then
        -- SUCCESS: Increase by UPGRADE_FIXED_PERCENT
        nUpgradePercent = UPGRADE_FIXED_PERCENT
        print("[LUA-UPGRADE] UPGRADE SUCCESS! Increasing by " .. nUpgradePercent .. "%")
    else
        -- FAILURE: No increase (0%)
        nUpgradePercent = 0
        print("[LUA-UPGRADE] UPGRADE FAILED! No increase (0%)")
    end

    -- Call C++ upgrade function
    -- This will update the item and trigger UpdateItem event on client
    print(string.format("[LUA-UPGRADE] Calling UpgradeItemMagicAttrib(itemIdx=%d, slot=%d, percent=%d)",
        nEquipIdx, nAttribSlot, nUpgradePercent))

    local nResult = UpgradeItemMagicAttrib(nEquipIdx, nAttribSlot, nUpgradePercent)

    print(string.format("[LUA-UPGRADE] UpgradeItemMagicAttrib returned: %d", nResult or -1))

    -- Get new value after upgrade
    local _, nNewValue, _, _ = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

    print(string.format("[LUA-UPGRADE] After upgrade: %d -> %d", nCurrentValue, nNewValue or nCurrentValue))

    -- Show result message
    if bSuccess then
        local nIncrease = (nNewValue or nCurrentValue) - nCurrentValue
        Msg2Player(string.format("Nâng cấp thành công! Thuộc tính tăng: %d -> %d (+%d)",
            nCurrentValue, nNewValue or nCurrentValue, nIncrease))
    else
        Msg2Player(string.format("Nâng cấp thất bại! Thuộc tính vẫn giữ nguyên: %d", nCurrentValue))
    end

    print("[LUA-UPGRADE] PerformUpgrade finished")
end

-- ------------------------------------------------------------
-- NPC main function (called when player talks to NPC)
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end
