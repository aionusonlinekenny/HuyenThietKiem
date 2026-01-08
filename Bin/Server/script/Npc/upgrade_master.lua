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
        return
    end

    print("[LUA-UPGRADE] Attribute slot to upgrade: " .. nAttribSlot)

    -- IMPORTANT: Materials, money, xu already consumed by C++ BEFORE calling this function
    -- We only need to check equipment and perform the upgrade logic

    -- Step 1: Get equipment from build container
    local nEquipPos = GetPlayerEquipRoom(15, 0, 0)  -- Container 15, slot 0
    if nEquipPos == 0 or nEquipPos == -1 then
        Msg2Player("Lỗi: Không tìm thấy trang bị trong ô nâng cấp")
        print("[LUA-UPGRADE] ERROR: Equipment not found in build container")
        return
    end

    print("[LUA-UPGRADE] Equipment found at position: " .. nEquipPos)

    -- Step 2: Get equipment info
    local nGenre, nDetail, nParticular, nLevel, nSeries, nLuck = GetPlayerEquipInfo(15, 0)
    print(string.format("[LUA-UPGRADE] Equipment: genre=%d, detail=%d, particular=%d, level=%d, series=%d, luck=%d",
        nGenre, nDetail, nParticular, nLevel, nSeries, nLuck))

    -- Step 3: Validate equipment is blue quality (Lam)
    if nLuck ~= 200 then
        Msg2Player("Chỉ có thể nâng cấp trang bị phẩm chất Lam (200 may)")
        print("[LUA-UPGRADE] ERROR: Equipment is not blue quality, luck=" .. nLuck)
        return
    end

    -- Step 4: Get current attribute value
    local nCurrentValue = GetItemPartAttrib(15, 0, nAttribSlot)
    if nCurrentValue <= 0 then
        Msg2Player("Thuộc tính này chưa có giá trị, không thể nâng cấp")
        print("[LUA-UPGRADE] ERROR: Attribute slot " .. nAttribSlot .. " has no value")
        return
    end

    print("[LUA-UPGRADE] Current attribute value at slot " .. nAttribSlot .. ": " .. nCurrentValue)

    -- Step 5: Calculate success rate (this was already calculated in C++ UI, but recalculate for verification)
    local nSuccessRate = UPGRADE_BASE_SUCCESS_RATE

    -- Add mineral bonuses (already consumed, but we calculated in C++)
    -- For now, use base rate since materials are already gone
    -- In full implementation, C++ should pass the calculated rate

    print("[LUA-UPGRADE] Success rate: " .. nSuccessRate .. "%")

    -- Step 6: Random check for success/failure
    local nRandom = random(1, 100)
    local bSuccess = (nRandom <= nSuccessRate)

    print(string.format("[LUA-UPGRADE] Random roll: %d/%d, Success: %s", nRandom, nSuccessRate, tostring(bSuccess)))

    if bSuccess then
        -- SUCCESS CASE
        print("[LUA-UPGRADE] UPGRADE SUCCESS!")

        -- Calculate new value (increase by fixed percentage)
        local nIncreaseValue = floor(nCurrentValue * UPGRADE_FIXED_PERCENT / 100)
        if nIncreaseValue < 1 then
            nIncreaseValue = 1  -- Minimum increase of 1
        end

        local nNewValue = nCurrentValue + nIncreaseValue
        print(string.format("[LUA-UPGRADE] Upgrading attribute: %d -> %d (+%d)", nCurrentValue, nNewValue, nIncreaseValue))

        -- Get attribute type for display
        local nAttribType = GetItemPartAttribType(15, 0, nAttribSlot)
        local szAttribName = GetAttribName(nAttribType)

        -- Perform the upgrade using C++ function
        -- This will automatically update the item and trigger UpdateItem event on client
        UpgradeItemAttributes(15, 0, nAttribSlot, nNewValue)

        -- Show success message
        Msg2Player(string.format("Nâng cấp thành công! %s: %d -> %d (+%d)",
            szAttribName, nCurrentValue, nNewValue, nIncreaseValue))
        print("[LUA-UPGRADE] UpgradeItemAttributes completed, UpdateItem sent to client")

    else
        -- FAILURE CASE
        print("[LUA-UPGRADE] UPGRADE FAILED!")

        -- CRITICAL FIX: Equipment is preserved, but we need to trigger UpdateItem
        -- to notify client that upgrade is complete
        -- Solution: Remove equipment and re-add it back to same position

        print("[LUA-UPGRADE] Saving all equipment attributes before removal")

        -- Save all 6 attribute slots BEFORE removing equipment
        local tbAttributes = {}
        for i = 0, 5 do
            tbAttributes[i] = {
                nType = GetItemPartAttribType(15, 0, i),
                nValue = GetItemPartAttrib(15, 0, i)
            }
            if tbAttributes[i].nType > 0 and tbAttributes[i].nValue > 0 then
                print(string.format("[LUA-UPGRADE] Saved slot %d: type=%d, value=%d",
                    i, tbAttributes[i].nType, tbAttributes[i].nValue))
            end
        end

        print("[LUA-UPGRADE] Triggering UpdateItem by removing and re-adding equipment")

        -- Remove equipment from build container
        RemovePlayerEquip(15, 0)
        print("[LUA-UPGRADE] Equipment removed from build container")

        -- Re-add equipment back to build container at slot 0
        -- This triggers UpdateItem event on client, signaling operation complete
        AddPlayerEquip(15, 0, nGenre, nDetail, nParticular, nLevel, nSeries, nLuck,
            tbAttributes[0].nType, tbAttributes[0].nValue,
            tbAttributes[1].nType, tbAttributes[1].nValue,
            tbAttributes[2].nType, tbAttributes[2].nValue,
            tbAttributes[3].nType, tbAttributes[3].nValue,
            tbAttributes[4].nType, tbAttributes[4].nValue,
            tbAttributes[5].nType, tbAttributes[5].nValue)
        print("[LUA-UPGRADE] Equipment re-added to build container with all attributes, UpdateItem triggered")

        -- Show failure message
        Msg2Player("Nâng cấp thất bại! Trang bị được giữ nguyên.")
        print("[LUA-UPGRADE] Failure handling complete")
    end

    print("[LUA-UPGRADE] PerformUpgrade finished")
end

-- ------------------------------------------------------------
-- NPC main function (called when player talks to NPC)
-- ------------------------------------------------------------
function main(NpcIndex)
    UpgradeAttribTalk()
end
