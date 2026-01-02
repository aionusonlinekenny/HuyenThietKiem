-----------------------------------------------------------------
-- Upgrade Equipment Attributes System
-- Author: Claude Code
-- Date: 2025-12-21
-- Function: Nang cap tung thuoc tinh magic cua trang bi xanh
-----------------------------------------------------------------

Include("\\script\\lib\\TaskLib.lua")

-- ------------------------------------------------------------
-- Configuration - Item IDs
-- ------------------------------------------------------------
-- Da Nang Cap (Upgrade Stone) item definition
-- Thay doi cac gia tri nay de su dung item khac
UPGRADE_MATERIAL_GENRE = 6      -- item_task
UPGRADE_MATERIAL_DETAIL = 19    -- ID cua Da Nang Cap (co the thay doi)

-- Upgrade settings
UPGRADE_MIN_PERCENT = 10        -- % tang toi thieu
UPGRADE_MAX_PERCENT = 20        -- % tang toi da
UPGRADE_SUCCESS_RATE = 100      -- Ti le thanh cong (%)

-- ------------------------------------------------------------
-- NPC Talk Entry Point
-- ------------------------------------------------------------
function UpgradeAttribTalk()
    local tbSay = {
        "Nang cap thuoc tinh trang bi xanh/OpenUpgradeUI",
        "Huong dan nang cap/ShowGuide",
        "Ta chi ghe ngang qua/no",
    }
    Say("Cao thu ren duc: Ta co the giup nguoi nang cap tung thuoc tinh cua trang bi xanh!",
        getn(tbSay), tbSay)
end

-- ------------------------------------------------------------
-- Open Upgrade UI
-- ------------------------------------------------------------
function OpenUpgradeUI()
    OpenUpgradeAttribUI()  -- Call C++ function to open UI
end

-- ------------------------------------------------------------
-- Execute Upgrade (called when player clicks "Upgrade" button)
-- ------------------------------------------------------------
function ExeUpgradeAttrib()
    WriteLog("[UPGRADE_ATTRIB] ExeUpgradeAttrib() called")

    local nPos = 15  -- pos_builditem (same container as Tremble)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)    -- Equipment slot
    local nMaterialIdx = GetPOItem(nPos, 1) -- Material slot

    WriteLog(string.format("[UPGRADE_ATTRIB] nEquipIdx=%d, nMaterialIdx=%d", nEquipIdx or -1, nMaterialIdx or -1))

    -- Validate equipment
    if nEquipIdx <= 0 then
        WriteLog("[UPGRADE_ATTRIB] ERROR: No equipment in slot")
        Talk(1, "", "<color=red>Chua dat trang bi vao!<color>")
        return
    end

    -- Validate material exists
    if nMaterialIdx <= 0 then
        WriteLog("[UPGRADE_ATTRIB] ERROR: No material in slot")
        Talk(1, "", "<color=red>Chua dat Da Nang Cap!<color>")
        return
    end

    -- Validate material type (must be correct upgrade material)
    local nMatGenre, nMatDetail = GetItemProp(nMaterialIdx)
    WriteLog(string.format("[UPGRADE_ATTRIB] Material: genre=%d, detail=%d (expected: %d, %d)",
        nMatGenre or -1, nMatDetail or -1, UPGRADE_MATERIAL_GENRE, UPGRADE_MATERIAL_DETAIL))

    if nMatGenre ~= UPGRADE_MATERIAL_GENRE or nMatDetail ~= UPGRADE_MATERIAL_DETAIL then
        WriteLog("[UPGRADE_ATTRIB] ERROR: Wrong material type!")
        Talk(1, "", "<color=red>Vat lieu khong dung! Can su dung Da Nang Cap.<color>")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    WriteLog(string.format("[UPGRADE_ATTRIB] Equipment: genre=%d, detail=%d, luck=%d",
        nGenre or -1, nDetail or -1, nLuck or -1))

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
    local bHasMagicAttrib = false
    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        if nAttribType and nAttribType > 0 then
            bHasMagicAttrib = true
            break
        end
    end

    if not bHasMagicAttrib then
        Talk(1, "", "<color=red>Trang bi nay khong co thuoc tinh magic!<color>")
        return
    end

    -- TODO: Add attribute selection UI on client
    -- For now, upgrade the FIRST attribute with value > 0
    local nAttribSlot = -1
    for i = 0, 5 do
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        if nAttribType and nAttribType > 0 then
            nAttribSlot = i
            break
        end
    end

    if nAttribSlot < 0 then
        Talk(1, "", "<color=red>Khong tim thay thuoc tinh de nang cap!<color>")
        return
    end

    -- Get attribute info
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

    -- Check if attribute is already at max
    if nMax > 0 and nOldValue >= nMax then
        Talk(1, "", "<color=yellow>Thuoc tinh dau tien da dat gia tri MAX!<color>\nHay nang cap thuoc tinh khac.")
        return
    end

    -- Calculate upgrade percentage (random between min and max)
    local nIncreasePercent = UPGRADE_MIN_PERCENT + random(0, UPGRADE_MAX_PERCENT - UPGRADE_MIN_PERCENT + 1)
    WriteLog(string.format("[UPGRADE_ATTRIB] Calculated increase: %d%%, Old value: %d", nIncreasePercent, nOldValue))

    -- Consume material FIRST
    WriteLog("[UPGRADE_ATTRIB] Attempting to consume material...")
    if DelItemByIndex(nMaterialIdx) == 0 then
        WriteLog("[UPGRADE_ATTRIB] ERROR: Failed to delete material item")
        Talk(1, "", "<color=red>Loi: Khong the tieu hao vat lieu!<color>")
        return
    end
    WriteLog("[UPGRADE_ATTRIB] Material consumed successfully")

    -- Material consumed successfully, now upgrade
    WriteLog(string.format("[UPGRADE_ATTRIB] Calling UpgradeItemMagicAttrib(item=%d, slot=%d, percent=%d)",
        nEquipIdx, nAttribSlot, nIncreasePercent))
    local bSuccess = UpgradeItemMagicAttrib(nEquipIdx, nAttribSlot, nIncreasePercent)
    WriteLog(string.format("[UPGRADE_ATTRIB] UpgradeItemMagicAttrib returned: %d", bSuccess or -1))

    if bSuccess == 1 then
        -- Get new value
        local _, nNewValue, _, _ = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
        WriteLog(string.format("[UPGRADE_ATTRIB] SUCCESS: %d -> %d", nOldValue, nNewValue))

        -- Success message
        local szMsg = string.format(
            "<color=green>Nang cap thanh cong!<color>\n" ..
            "Thuoc tinh #%d: <color=yellow>%d -> %d<color> (+%d%%)",
            nAttribSlot + 1,
            nOldValue,
            nNewValue,
            nIncreasePercent
        )
        Msg2Player(szMsg)

        -- Log for debugging
        WriteLog(string.format("[UPGRADE_ATTRIB] COMPLETE: Player=%s, Item=%d, Slot=%d, %d->%d (+%d%%)",
            GetName(), nEquipIdx, nAttribSlot, nOldValue, nNewValue, nIncreasePercent))
    else
        WriteLog("[UPGRADE_ATTRIB] ERROR: Upgrade failed")
        Talk(1, "", "<color=red>Nang cap that bai! Vui long thu lai.<color>")
    end
end

-- ------------------------------------------------------------
-- Show Upgrade Guide
-- ------------------------------------------------------------
function ShowGuide()
    local szGuide = string.format(
        "<color=yellow>=== HUONG DAN NANG CAP THUOC TINH ===<color>\n\n" ..
        "<color=cyan>Cach thuc hien:<color>\n" ..
        "1. Dat <color=blue>trang bi xanh<color> vao o tren\n" ..
        "2. Dat <color=orange>Da Nang Cap<color> vao o duoi\n" ..
        "3. Chon thuoc tinh muon nang cap tu danh sach\n" ..
        "4. Nhan <color=green>Nang Cap<color>\n\n" ..
        "<color=cyan>Chi tiet:<color>\n" ..
        "- Chi nang cap duoc <color=blue>trang bi xanh<color>\n" ..
        "- Moi lan nang tang <color=yellow>%d-%d%%<color> gia tri\n" ..
        "- Khong the vuot qua gia tri <color=red>MAX<color>\n" ..
        "- Ti le thanh cong: <color=green>%d%%<color>\n" ..
        "- Mat <color=orange>1 Da Nang Cap<color> moi lan\n\n" ..
        "<color=cyan>Luu y:<color>\n" ..
        "- Thuoc tinh da MAX khong the nang them\n" ..
        "- Vat lieu bi mat khi nang cap\n" ..
        "- Khong the hoan tac sau khi nang",
        UPGRADE_MIN_PERCENT, UPGRADE_MAX_PERCENT, UPGRADE_SUCCESS_RATE
    )

    Talk(1, "", szGuide)
end

-- ------------------------------------------------------------
-- Count blue equipment in inventory
-- ------------------------------------------------------------
function CountBlueEquipment()
    local nCount = 0

    -- Check all inventory slots (0-59)
    for i = 0, 59 do
        local nItemIdx = GetItemIndex(i)
        if nItemIdx > 0 then
            local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nItemIdx)

            -- Blue equipment: genre 0, luck < 1000000000
            if nGenre == 0 and nLuck < 1000000000 then
                -- Check if has magic attributes
                for j = 0, 5 do
                    local nAttribType = GetItemMagicAttribInfo(nItemIdx, j)
                    if nAttribType and nAttribType > 0 then
                        nCount = nCount + 1
                        break
                    end
                end
            end
        end
    end

    return nCount
end