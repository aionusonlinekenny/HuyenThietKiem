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
UPGRADE_FIXED_PERCENT = 20      -- % tang co dinh (FIXED 20%)
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
-- This function validates items and shows attribute selection menu
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

    -- Build list of all upgradeable attributes
    Msg2Player("=== Building attribute selection menu ===")
    local tbUpgradeableAttribs = {}
    local tbSayOptions = {}

    for i = 0, 5 do
        Msg2Player(">>> Loop iteration i=" .. i)
        local nAttribType, nValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        Msg2Player("  GetItemMagicAttribInfo returned: type=" .. tostring(nAttribType) .. " val=" .. tostring(nValue) .. " min=" .. tostring(nMin) .. " max=" .. tostring(nMax))

        if nAttribType and nAttribType > 0 then
            Msg2Player("  -> Has attribute type " .. nAttribType)

            -- Check if this attribute can be upgraded
            local bCanUpgrade = (nMax <= 0 or nValue < nMax)
            Msg2Player("  -> bCanUpgrade=" .. tostring(bCanUpgrade) .. " (nMax=" .. tostring(nMax) .. ", nValue=" .. tostring(nValue) .. ")")

            if bCanUpgrade then
                Msg2Player("  -> CAN UPGRADE - building option string")

                -- Calculate potential new value
                local nIncrease = (nValue * UPGRADE_FIXED_PERCENT) / 100
                if nIncrease < 1 then nIncrease = 1 end
                local nNewValue = nValue + nIncrease
                if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

                Msg2Player("  -> Calculated: " .. nValue .. " + " .. nIncrease .. " = " .. nNewValue)

                -- Build option string
                local szOption = "Thuoc tinh #" .. (i + 1) .. " (Type " .. nAttribType .. "): " .. nValue .. " -> " .. nNewValue .. " (+" .. UPGRADE_FIXED_PERCENT .. "%)"
                if nMax > 0 then
                    szOption = szOption .. " [Max: " .. nMax .. "]"
                end

                Msg2Player("  -> Option string: " .. szOption)

                -- Add to tables
                tinsert(tbSayOptions, szOption .. "/DoUpgradeAttrib_" .. i)
                tinsert(tbUpgradeableAttribs, i)

                Msg2Player("  -> Added to menu!")
            else
                Msg2Player("  -> CANNOT UPGRADE (at MAX)")
            end
        else
            Msg2Player("  -> No attribute in this slot")
        end
    end

    Msg2Player("=== Menu building complete ===")
    Msg2Player("Total upgradeable attributes: " .. getn(tbUpgradeableAttribs))

    if getn(tbUpgradeableAttribs) == 0 then
        Msg2Player("ERROR: No upgradeable attributes found (all at MAX or no attributes)")
        Talk(1, "", "<color=red>Tat ca thuoc tinh da dat MAX! Khong the nang cap!<color>")
        return
    end

    -- Add cancel option
    tinsert(tbSayOptions, "Huy bo/no")

    Msg2Player("=== Calling Say() with " .. getn(tbSayOptions) .. " options ===")
    for j = 1, getn(tbSayOptions) do
        Msg2Player("  Option " .. j .. ": " .. tbSayOptions[j])
    end

    -- Show selection menu
    Say("Chon thuoc tinh muon nang cap:\nTang co dinh: +" .. UPGRADE_FIXED_PERCENT .. "%",
        getn(tbSayOptions), tbSayOptions)

    Msg2Player("=== Say() called ===")
end

-- ────────────────────────────────────────────────────────────
-- Perform actual upgrade on selected attribute slot
-- Called by dynamic functions DoUpgradeAttrib_0 through DoUpgradeAttrib_5
-- ────────────────────────────────────────────────────────────
function PerformUpgrade(nAttribSlot)
    Msg2Player("═══ PERFORMING UPGRADE ═══")
    Msg2Player("Selected slot: " .. nAttribSlot)

    local nPos = 15  -- pos_builditem

    -- Get items again (in case they changed)
    local nEquipIdx = GetPOItem(nPos, 0)
    local nMaterialIdx = GetPOItem(nPos, 1)

    if nEquipIdx <= 0 or nMaterialIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Trang bi hoac vat lieu bi mat!<color>")
        return
    end

    -- Get attribute info for the selected slot
    local nAttribType, nOldValue, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, nAttribSlot)
    Msg2Player("═══ SELECTED ATTRIBUTE ═══")
    Msg2Player("  Slot: " .. nAttribSlot)
    Msg2Player("  Type: " .. tostring(nAttribType))
    Msg2Player("  Current Value: " .. tostring(nOldValue))
    Msg2Player("  Min: " .. tostring(nMin) .. " | Max: " .. tostring(nMax))

    -- Use FIXED upgrade percentage
    local nIncreasePercent = UPGRADE_FIXED_PERCENT
    Msg2Player("  Increase: FIXED " .. nIncreasePercent .. "%")

    -- Calculate new value (C++ will cast to int automatically)
    local nIncrease = (nOldValue * nIncreasePercent) / 100
    if nIncrease < 1 then nIncrease = 1 end
    local nNewValue = nOldValue + nIncrease
    if nMax > 0 and nNewValue > nMax then nNewValue = nMax end

    Msg2Player("  Calculation: " .. nOldValue .. " + " .. nIncrease .. " = " .. nNewValue)
    Msg2Player("═══════════════════════════")

    -- Create upgraded item (C++ will handle removing old equipment and placing new one)
    Msg2Player(">>> Calling C++ UpgradeItemAttributes...")
    local nNewItemIdx = UpgradeItemAttributes(nEquipIdx, nAttribSlot, nIncreasePercent, nPos)

    if nNewItemIdx == 0 then
        Msg2Player("<color=red>ERROR: UpgradeItemAttributes returned 0!<color>")
        Talk(1, "", "<color=red>Loi: Khong the nang cap! Co the do rong hoac loi khac.<color>")
        return
    end

    Msg2Player("<color=green>>>> New item created! Index: " .. nNewItemIdx .. "<color>")

    -- Verify new item attributes
    Msg2Player("═══ VERIFYING NEW ITEM ═══")
    for i = 0, 5 do
        local nType, nVal, nMinVal, nMaxVal = GetItemMagicAttribInfo(nNewItemIdx, i)
        if nType and nType > 0 then
            local szStatus = ""
            if i == nAttribSlot then
                if nVal == nNewValue then
                    szStatus = " <color=green>[MATCH!]<color>"
                else
                    szStatus = " <color=red>[MISMATCH! Expected " .. nNewValue .. "]<color>"
                end
            end
            Msg2Player("  Slot " .. i .. ": Type=" .. nType .. ", Val=" .. nVal .. szStatus)
        end
    end

    -- Delete material only (equipment already handled by C++ function)
    Msg2Player("Deleting material...")
    local nResult = DelItemByIndex(nMaterialIdx)
    Msg2Player("Delete material result: " .. nResult)

    -- Success message
    local szMsg = string.format("<color=green>Nang cap thanh cong!<color>\n" ..
                  "Thuoc tinh #%d (Type %d): <color=yellow>%d -> %d<color> (+%d%%)",
                  (nAttribSlot + 1), nAttribType, nOldValue, nNewValue, nIncreasePercent)
    Talk(1, "", szMsg)
end

-- ────────────────────────────────────────────────────────────
-- Dynamic upgrade functions for each attribute slot (0-5)
-- These are called from the Say menu
-- ────────────────────────────────────────────────────────────
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

-- ────────────────────────────────────────────────────────────
-- Show Upgrade Guide
-- ────────────────────────────────────────────────────────────
function ShowGuide()
    local szGuide = "<color=yellow>=== HUONG DAN NANG CAP THUOC TINH ===<color>\n\n" ..
        "<color=cyan>Cach thuc hien:<color>\n" ..
        "1. Dat <color=blue>trang bi xanh<color> vao o tren\n" ..
        "2. Dat <color=orange>Da Nang Cap<color> vao o duoi\n" ..
        "3. Nhan nut <color=green>Nang Cap<color>\n" ..
        "4. Chon thuoc tinh muon nang cap tu menu\n" ..
        "5. Xac nhan de hoan tat\n\n" ..
        "<color=cyan>Chi tiet:<color>\n" ..
        "- Chi nang cap duoc <color=blue>trang bi xanh<color>\n" ..
        "- Moi lan nang tang <color=yellow>" .. UPGRADE_FIXED_PERCENT .. "%%<color> gia tri\n" ..
        "- Khong the vuot qua gia tri <color=red>MAX<color>\n" ..
        "- Ti le thanh cong: <color=green>" .. UPGRADE_SUCCESS_RATE .. "%%<color>\n" ..
        "- Mat <color=orange>1 Da Nang Cap<color> moi lan\n\n" ..
        "<color=cyan>Luu y:<color>\n" ..
        "- Thuoc tinh da MAX khong the nang them\n" ..
        "- Ban co the chon bat ky thuoc tinh nao de nang\n" ..
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
