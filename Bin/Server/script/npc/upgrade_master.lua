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
UPGRADE_MIN_PERCENT = 10        -- % tang toi thieu
UPGRADE_MAX_PERCENT = 20        -- % tang toi da
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
-- ────────────────────────────────────────────────────────────
function ExeUpgradeAttrib()
    Msg2Player("========================================")
    Msg2Player("=== ExeUpgradeAttrib START ===")
    Msg2Player("========================================")

    local nPos = 15  -- pos_builditem (same container as Tremble)
    Msg2Player("Build container pos = " .. nPos)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)    -- Equipment slot
    local nMaterialIdx = GetPOItem(nPos, 1) -- Material slot
    Msg2Player("Equipment idx = " .. tostring(nEquipIdx) .. ", Material idx = " .. tostring(nMaterialIdx))

    -- Validate equipment
    if nEquipIdx <= 0 then
        Msg2Player("ERROR: No equipment (nEquipIdx <= 0)")
        Talk(1, "", "<color=red>Chua dat trang bi vao!<color>")
        return
    end

    -- Validate material exists
    if nMaterialIdx <= 0 then
        Msg2Player("ERROR: No material (nMaterialIdx <= 0)")
        Talk(1, "", "<color=red>Chua dat Da Nang Cap!<color>")
        return
    end

    -- Validate material type (must be correct upgrade material)
    local nMatGenre, nMatDetail = GetItemProp(nMaterialIdx)
    Msg2Player("Material: genre=" .. tostring(nMatGenre) .. " detail=" .. tostring(nMatDetail) .. " (expected: " .. UPGRADE_MATERIAL_GENRE .. "," .. UPGRADE_MATERIAL_DETAIL .. ")")

    if nMatGenre ~= UPGRADE_MATERIAL_GENRE or nMatDetail ~= UPGRADE_MATERIAL_DETAIL then
        Msg2Player("ERROR: Wrong material type!")
        Talk(1, "", "<color=red>Vat lieu khong dung! Can su dung Da Nang Cap.<color>")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    Msg2Player("Equipment: genre=" .. tostring(nGenre) .. " detail=" .. tostring(nDetail) .. " luck=" .. tostring(nLuck))

    -- Check if equipment is blue (genre 0 with luck < 1000000000)
    if nGenre ~= 0 then
        Msg2Player("ERROR: Not equipment (genre != 0)")
        Talk(1, "", "<color=red>Chi co the nang cap trang bi xanh!<color>")
        return
    end

    if nLuck >= 1000000000 then
        Msg2Player("ERROR: Purple/Gold equipment (luck >= 1000000000)")
        Talk(1, "", "<color=red>Trang bi nay la tim/vang, khong the nang cap!<color>")
        return
    end

    -- Check if equipment has magic attributes (USE 1/0 instead of true/false)
    local bHasMagicAttrib = 0
    Msg2Player("=== DEBUG GetItemMagicAttribInfo ===")

    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        local szMsg = "Slot" .. i .. ": type=" .. tostring(nAttribType) ..
                      " val=" .. tostring(nValue) ..
                      " min=" .. tostring(nMin) ..
                      " max=" .. tostring(nMax)
        Msg2Player(szMsg)

        -- Debug the condition check
        if nAttribType and nAttribType > 0 then
            Msg2Player("  -> Slot" .. i .. " PASSED check, setting bHasMagicAttrib=1")
            bHasMagicAttrib = 1
        else
            Msg2Player("  -> Slot" .. i .. " FAILED check")
        end
    end

    Msg2Player("Final bHasMagicAttrib = " .. tostring(bHasMagicAttrib))

    if bHasMagicAttrib == 0 then
        Talk(1, "", "<color=red>Trang bi nay khong co thuoc tinh magic!<color>")
        return
    end

    -- Find the FIRST attribute that is NOT at max
    Msg2Player("=== Finding first upgradeable attribute ===")
    local nAttribSlot = -1
    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        if nAttribType and nAttribType > 0 then
            -- Check if this attribute is already at max
            if nMax > 0 and nValue >= nMax then
                Msg2Player("Slot " .. i .. " is at MAX (" .. nValue .. "/" .. nMax .. "), skipping...")
            else
                nAttribSlot = i
                Msg2Player("Found upgradeable attribute at slot " .. i .. " (value=" .. nValue .. ", max=" .. nMax .. ")")
                break
            end
        end
    end

    if nAttribSlot < 0 then
        Msg2Player("ERROR: No upgradeable attribute found (all at MAX or no attributes)")
        Talk(1, "", "<color=red>Tat ca thuoc tinh da dat MAX! Khong the nang cap!<color>")
        return
    end

    -- Get attribute info for the selected slot
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
    Msg2Player("Selected slot " .. nAttribSlot .. ": type=" .. tostring(nAttribType) .. " oldVal=" .. tostring(nOldValue) .. " min=" .. tostring(nMin) .. " max=" .. tostring(nMax))

    -- Calculate upgrade percentage (random between min and max)
    local nIncreasePercent = UPGRADE_MIN_PERCENT + random(0, UPGRADE_MAX_PERCENT - UPGRADE_MIN_PERCENT + 1)
    Msg2Player("Calculated increase: " .. nIncreasePercent .. "%")

    -- Calculate new value (C++ will cast to int automatically)
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    Msg2Player("Calculated: " .. nOldValue .. " + " .. nIncrease .. " = " .. nNewValue)

    -- Read all item properties for recreation
    Msg2Player("=== Reading item properties for recreation ===")

    -- Get all 6 generator levels (tiers 1-10) from the item
    local l0, l1, l2, l3, l4, l5 = GetItemGeneratorLevels(nEquipIdx)
    local nOldGenLevels = {l0, l1, l2, l3, l4, l5}

    Msg2Player("Old generator levels (tiers): [" .. l0 .. "," .. l1 .. "," .. l2 .. "," .. l3 .. "," .. l4 .. "," .. l5 .. "]")
    Msg2Player("Upgrading slot " .. nAttribSlot .. " from value " .. nOldValue .. " to " .. nNewValue)

    -- Upgrade strategy: Increase the generator level (tier) by 1
    -- This moves the attribute to the next tier in MagicAttrib.txt
    local nOldTier = nOldGenLevels[nAttribSlot+1]
    local nNewTier = nOldTier + 1

    -- Cap at tier 10 (max generator level)
    if nNewTier > 10 then
        nNewTier = 10
        Msg2Player("WARNING: Tier already at max (10), keeping at 10")
    end

    nOldGenLevels[nAttribSlot+1] = nNewTier
    Msg2Player("Slot " .. nAttribSlot .. " tier: " .. nOldTier .. " -> " .. nNewTier)

    -- CRITICAL: Delete items in correct order to prevent index invalidation
    -- Must delete HIGHER index first, then LOWER index
    -- Otherwise deleting lower index shifts higher index and causes deletion to fail
    Msg2Player("Equipment idx=" .. nEquipIdx .. ", Material idx=" .. nMaterialIdx)

    if nEquipIdx > nMaterialIdx then
        -- Delete equipment first (higher index)
        Msg2Player("Deleting equipment first (higher index)...")
        if DelItemByIndex(nEquipIdx) == 0 then
            Msg2Player("ERROR: Failed to delete equipment!")
            Talk(1, "", "<color=red>Loi: Khong the xoa trang bi!<color>")
            return
        end
        Msg2Player("Equipment deleted successfully!")

        -- Then delete material (lower index, still valid)
        Msg2Player("Deleting material (lower index)...")
        if DelItemByIndex(nMaterialIdx) == 0 then
            Msg2Player("ERROR: Failed to delete material!")
            Talk(1, "", "<color=red>Loi: Khong the xoa vat lieu!<color>")
            return
        end
        Msg2Player("Material deleted successfully!")
    else
        -- Delete material first (higher index)
        Msg2Player("Deleting material first (higher index)...")
        if DelItemByIndex(nMaterialIdx) == 0 then
            Msg2Player("ERROR: Failed to delete material!")
            Talk(1, "", "<color=red>Loi: Khong the xoa vat lieu!<color>")
            return
        end
        Msg2Player("Material deleted successfully!")

        -- Then delete equipment (lower index, still valid)
        Msg2Player("Deleting equipment (lower index)...")
        if DelItemByIndex(nEquipIdx) == 0 then
            Msg2Player("ERROR: Failed to delete equipment!")
            Talk(1, "", "<color=red>Loi: Khong the xoa trang bi!<color>")
            return
        end
        Msg2Player("Equipment deleted successfully!")
    end

    -- Create new item with upgraded generator level
    Msg2Player("Creating new item with upgraded attributes...")
    Msg2Player("Genre=" .. nGenre .. " Detail=" .. nDetail .. " Parti=" .. nParti .. " Level=" .. nLevel .. " Series=" .. nSeries .. " Luck=" .. nLuck)
    Msg2Player("GenLevels: [" .. nOldGenLevels[1] .. "," .. nOldGenLevels[2] .. "," .. nOldGenLevels[3] .. "," .. nOldGenLevels[4] .. "," .. nOldGenLevels[5] .. "," .. nOldGenLevels[6] .. "]")

    AddItemEx(
        nGenre, nDetail, nParti, nLevel, nSeries, nLuck,
        nOldGenLevels[1], nOldGenLevels[2], nOldGenLevels[3],
        nOldGenLevels[4], nOldGenLevels[5], nOldGenLevels[6],
        1, 0,  -- version, randseed
        nPos   -- pos_builditem
    )
    Msg2Player("New item created successfully!")

    -- Success message
    local szMsg = "<color=green>Nang cap thanh cong!<color>\n" ..
                  "Thuoc tinh #" .. (nAttribSlot + 1) ..
                  ": <color=yellow>" .. nOldValue .. " -> " .. nNewValue .. "<color> (+" .. nIncreasePercent .. "%)"
    Talk(1, "", szMsg)
end

-- ────────────────────────────────────────────────────────────
-- Show Upgrade Guide
-- ────────────────────────────────────────────────────────────
function ShowGuide()
    local szGuide = "<color=yellow>=== HUONG DAN NANG CAP THUOC TINH ===<color>\n\n" ..
        "<color=cyan>Cach thuc hien:<color>\n" ..
        "1. Dat <color=blue>trang bi xanh<color> vao o tren\n" ..
        "2. Dat <color=orange>Da Nang Cap<color> vao o duoi\n" ..
        "3. Chon thuoc tinh muon nang cap tu danh sach\n" ..
        "4. Nhan <color=green>Nang Cap<color>\n\n" ..
        "<color=cyan>Chi tiet:<color>\n" ..
        "- Chi nang cap duoc <color=blue>trang bi xanh<color>\n" ..
        "- Moi lan nang tang <color=yellow>" .. UPGRADE_MIN_PERCENT .. "-" .. UPGRADE_MAX_PERCENT .. "%%<color> gia tri\n" ..
        "- Khong the vuot qua gia tri <color=red>MAX<color>\n" ..
        "- Ti le thanh cong: <color=green>" .. UPGRADE_SUCCESS_RATE .. "%%<color>\n" ..
        "- Mat <color=orange>1 Da Nang Cap<color> moi lan\n\n" ..
        "<color=cyan>Luu y:<color>\n" ..
        "- Thuoc tinh da MAX khong the nang them\n" ..
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
