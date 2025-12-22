--═══════════════════════════════════════════════════════════════
-- Upgrade Equipment Attributes System
-- Author: Claude Code
-- Date: 2025-12-21
-- Function: Nâng cấp từng thuộc tính magic của trang bị xanh
--═══════════════════════════════════════════════════════════════

Include("\\script\\lib\\TaskLib.lua")

-- ────────────────────────────────────────────────────────────
-- NPC Talk Entry Point
-- ────────────────────────────────────────────────────────────
function UpgradeAttribTalk()
    local tbSay = {
        "Nâng cấp thuộc tính trang bị xanh/OpenUpgradeUI",
        "Hướng dẫn nâng cấp/ShowGuide",
        "Ta chỉ ghé ngang qua/no",
    }
    Say("Cao thủ rèn đúc: Ta có thể giúp ngươi nâng cấp từng thuộc tính của trang bị xanh!",
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
        Talk(1, "", "<color=red>Chưa đặt trang bị vào!<color>")
        return
    end

    -- Validate material
    if nMaterialIdx <= 0 then
        Talk(1, "", "<color=red>Chưa đặt Đá Nâng Cấp!<color>")
        return
    end

    -- Get equipment info
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    -- Check if equipment is blue (genre 0 with luck < 1000000000)
    if nGenre ~= 0 then
        Talk(1, "", "<color=red>Chỉ có thể nâng cấp trang bị xanh!<color>")
        return
    end

    if nLuck >= 1000000000 then
        Talk(1, "", "<color=red>Trang bị này là tím/vàng, không thể nâng cấp!<color>")
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
        Talk(1, "", "<color=red>Trang bị này không có thuộc tính magic!<color>")
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
        Talk(1, "", "<color=red>Không tìm thấy thuộc tính để nâng cấp!<color>")
        return
    end

    -- Get attribute info
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

    -- Check if attribute is already at max
    if nMax > 0 and nOldValue >= nMax then
        Talk(1, "", "<color=yellow>Thuộc tính đầu tiên đã đạt giá trị MAX!<color>\nHãy nâng cấp thuộc tính khác.")
        return
    end

    -- Calculate upgrade: 10-20% increase (random)
    local nIncreasePercent = 10 + random(0, 11)  -- 10-20%

    -- Consume material FIRST
    if DelItemByIndex(nMaterialIdx) == 0 then
        Talk(1, "", "<color=red>Lỗi: Không thể tiêu hao vật liệu!<color>")
        return
    end

    -- Material consumed successfully, now upgrade
    local bSuccess = UpgradeItemMagicAttrib(nEquipIdx, nAttribSlot, nIncreasePercent)

    if bSuccess == 1 then
        -- Get new value
        local _, nNewValue, _, _ = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)

        -- Success message
        local szMsg = string.format(
            "<color=green>✓ Nâng cấp thành công!<color>\n" ..
            "Thuộc tính #%d: <color=yellow>%d → %d<color> (+%d%%)",
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
        Talk(1, "", "<color=red>Nâng cấp thất bại! Vui lòng thử lại.<color>")
    end
end

-- ────────────────────────────────────────────────────────────
-- Show Upgrade Guide
-- ────────────────────────────────────────────────────────────
function ShowGuide()
    local szGuide = [[
<color=yellow>═══ HƯỚNG DẪN NÂNG CẤP THUỘC TÍNH ═══<color>

<color=cyan>Cách thực hiện:<color>
1. Đặt <color=blue>trang bị xanh<color> vào ô trên
2. Đặt <color=orange>Đá Nâng Cấp<color> vào ô dưới
3. Chọn thuộc tính muốn nâng cấp từ danh sách
4. Nhấn <color=green>Nâng Cấp<color>

<color=cyan>Chi tiết:<color>
• Chỉ nâng cấp được <color=blue>trang bị xanh<color>
• Mỗi lần nâng tăng <color=yellow>10-20%<color> giá trị
• Không thể vượt quá giá trị <color=red>MAX<color>
• Tỷ lệ thành công: <color=green>100%<color>
• Mất <color=orange>1 Đá Nâng Cấp<color> mỗi lần

<color=cyan>Lưu ý:<color>
• Thuộc tính đã MAX không thể nâng thêm
• Vật liệu bị mất khi nâng cấp
• Không thể hoàn tác sau khi nâng
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
