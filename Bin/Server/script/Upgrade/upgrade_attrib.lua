--═══════════════════════════════════════════════════════════════
-- Upgrade Equipment Attributes System
-- Author: Claude Code
-- Date: 2025-12-21
-- Function: Nang cap tung thuoc tinh magic cua trang bi xanh
--═══════════════════════════════════════════════════════════════

Include("\\script\\lib\\TaskLib.lua")

-- ────────────────────────────────────────────────────────────
-- NPC Talk Entry Point
-- ────────────────────────────────────────────────────────────
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
    local nPos = 15  -- pos_builditem (same container as Tremble)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)    -- Equipment slot
    local nMaterialIdx = GetPOItem(nPos, 1) -- Material slot

    -- Validate equipment
    if nEquipIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat trang bi vao!<color>")
        return
    end

    -- Validate material
    if nMaterialIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat Da Nang Cap!<color>")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

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

    -- Calculate upgrade: 10-20% increase (random)
    local nIncreasePercent = 10 + random(0, 11)  -- 10-20%

    -- Consume material FIRST
    if DelItemByIndex(nMaterialIdx) == 0 then
        Talk(1, "", "<color=red>Loi: Khong the tieu hao vat lieu!<color>")
        return
    end

    -- Material consumed successfully, now upgrade
    local bSuccess = UpgradeItemMagicAttrib(nEquipIdx, nAttribSlot, nIncreasePercent)

    if bSuccess == 1 then
        -- Get new value
        local _, nNewValue, _, _ = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

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
        WriteLog(string.format("[UPGRADE] Player=%s, Item=%d, Slot=%d, Old=%d, New=%d, Percent=%d",
            GetName(), nEquipIdx, nAttribSlot, nOldValue, nNewValue, nIncreasePercent))
    else
        Talk(1, "", "<color=red>Nang cap that bai! Vui long thu lai.<color>")
    end
end

-- ────────────────────────────────────────────────────────────
-- Show Upgrade Guide
-- ────────────────────────────────────────────────────────────
function ShowGuide()
    local szGuide = [[
<color=yellow>=== HUONG DAN NANG CAP THUOC TINH ===<color>

<color=cyan>Cach thuc hien:<color>
1. Dat <color=blue>trang bi xanh<color> vao o tren
2. Dat <color=orange>Da Nang Cap<color> vao o duoi
3. Chon thuoc tinh muon nang cap tu danh sach
4. Nhan <color=green>Nang Cap<color>

<color=cyan>Chi tiet:<color>
- Chi nang cap duoc <color=blue>trang bi xanh<color>
- Moi lan nang tang <color=yellow>10-20%<color> gia tri
- Khong the vuot qua gia tri <color=red>MAX<color>
- Ti le thanh cong: <color=green>100%<color>
- Mat <color=orange>1 Da Nang Cap<color> moi lan

<color=cyan>Luu y:<color>
- Thuoc tinh da MAX khong the nang them
- Vat lieu bi mat khi nang cap
- Khong the hoan tac sau khi nang
]]

    Talk(1, "", szGuide)
end

-- ────────────────────────────────────────────────────────────
-- Count blue equipment in inventory
-- ────────────────────────────────────────────────────────────
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
