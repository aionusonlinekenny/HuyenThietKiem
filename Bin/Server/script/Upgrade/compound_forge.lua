-----------------------------------------------------------------
-- CompoundItem Forge - Create Purple Equipment
-- Author: Claude Code
-- Date: 2025-12-27
-- Function: Tao trang bi tim tu trang bi xanh + Huyen Tinh
-----------------------------------------------------------------

Include("\\script\\lib\\TaskLib.lua")

-- ------------------------------------------------------------
-- Configuration
-- ------------------------------------------------------------
-- Huyen Tinh (Crystal) item definition
HUYEN_TINH_GENRE = 6      -- item_task
HUYEN_TINH_MIN_DETAIL = 74    -- Huyen Tinh Cap 1
HUYEN_TINH_MAX_DETAIL = 79    -- Huyen Tinh Cap 6

-- Purple equipment flag
PURPLE_LUCK_FLAG = 1000000000

-- ------------------------------------------------------------
-- Execute Forge (called when player clicks "Che Tao" button)
-- ------------------------------------------------------------
function ExeCompoundForge()
    WriteLog("[COMPOUND_FORGE] ExeCompoundForge() called")

    local nPos = 15  -- pos_builditem (same container as Tremble/Upgrade)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)      -- BigBox = slot 0 (UIEP_BUILDITEM1)
    local nCrystalIdx = GetPOItem(nPos, 1)    -- SmallBox = slot 1 (UIEP_BUILDITEM2)

    WriteLog(string.format("[COMPOUND_FORGE] nEquipIdx=%d, nCrystalIdx=%d", nEquipIdx or -1, nCrystalIdx or -1))

    -- Validate equipment exists
    if not nEquipIdx or nEquipIdx <= 0 then
        WriteLog("[COMPOUND_FORGE] ERROR: No equipment in slot")
        Talk(1, "", "<color=red>Vui long dat trang bi vao o lon!<color>")
        return
    end

    -- Validate crystal exists
    if not nCrystalIdx or nCrystalIdx <= 0 then
        WriteLog("[COMPOUND_FORGE] ERROR: No crystal in slot")
        Talk(1, "", "<color=red>Vui long dat Huyen Tinh vao o nho!<color>")
        return
    end

    -- Get equipment properties
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    WriteLog(string.format("[COMPOUND_FORGE] Equipment: genre=%d, detail=%d, parti=%d, level=%d, series=%d, luck=%d",
        nGenre or -1, nDetail or -1, nParti or -1, nLevel or -1, nSeries or -1, nLuck or -1))

    -- Validate equipment is blue (genre 0, luck < purple flag)
    if nGenre ~= 0 then
        Talk(1, "", "<color=red>Chi duoc dat trang bi vao o lon!<color>")
        return
    end

    if nLuck >= PURPLE_LUCK_FLAG then
        Talk(1, "", "<color=red>Trang bi nay da la tim/vang, khong the che tao lai!<color>")
        return
    end

    -- Validate equipment is allowed (NOT ring, amulet, pendant, horse, mask)
    -- equip_ring=3, equip_amulet=4, equip_pendant=8, equip_horse=10, equip_mask=11
    if nDetail == 3 or nDetail == 4 or nDetail == 8 or nDetail == 10 or nDetail == 11 then
        Talk(1, "", "<color=red>Khong duoc dat nhan, ngoc boi, day chuyen, ngua, mat na!<color>")
        return
    end

    -- Get crystal properties
    local nCrystalGenre, nCrystalDetail = GetItemProp(nCrystalIdx)
    WriteLog(string.format("[COMPOUND_FORGE] Crystal: genre=%d, detail=%d (expected: %d, %d-%d)",
        nCrystalGenre or -1, nCrystalDetail or -1,
        HUYEN_TINH_GENRE, HUYEN_TINH_MIN_DETAIL, HUYEN_TINH_MAX_DETAIL))

    -- Validate crystal is Huyen Tinh
    if nCrystalGenre ~= HUYEN_TINH_GENRE then
        Talk(1, "", "<color=red>Chi duoc dat Huyen Tinh vao o nho!<color>")
        return
    end

    if nCrystalDetail < HUYEN_TINH_MIN_DETAIL or nCrystalDetail > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>Chi duoc dat Huyen Tinh (Cap 1-6) vao o nho!<color>")
        return
    end

    -- Calculate crystal level (74=1, 75=2, ..., 79=6)
    local nCrystalLevel = nCrystalDetail - 73
    WriteLog(string.format("[COMPOUND_FORGE] Crystal level: %d", nCrystalLevel))

    -- Determine number of magic attribute lines based on crystal level
    local nNumLines = 0
    if nCrystalLevel >= 5 then
        -- Level 5-6: Guaranteed 6 lines
        nNumLines = 6
        WriteLog(string.format("[COMPOUND_FORGE] Crystal level %d >= 5: Guaranteed 6 lines", nCrystalLevel))
    else
        -- Level 1-4: Random based on crystal level
        -- Level 1: 2-3, Level 2: 2-4, Level 3: 3-5, Level 4: 3-6
        local nMinLines = 2
        if nCrystalLevel >= 3 then
            nMinLines = 3
        end
        local nMaxLines = nCrystalLevel + 2
        nNumLines = random(nMinLines, nMaxLines)
        WriteLog(string.format("[COMPOUND_FORGE] Crystal level %d: Random %d-%d lines, got %d",
            nCrystalLevel, nMinLines, nMaxLines, nNumLines))
    end

    -- Create magic attribute level array (all level 10)
    local tbMagicLevel = {}
    for i = 1, 6 do
        if i <= nNumLines then
            tbMagicLevel[i] = 10  -- Level 10 attributes
        else
            tbMagicLevel[i] = 0   -- Empty slot
        end
    end

    -- Calculate equipment record (wRecord = nParti * 10 + nLevel - 1)
    local wRecord = nParti * 10 + nLevel - 1
    WriteLog(string.format("[COMPOUND_FORGE] wRecord=%d (parti=%d, level=%d)", wRecord, nParti, nLevel))

    -- Create purple equipment with random luck value
    local nPurpleLuck = PURPLE_LUCK_FLAG + random(1, 999)

    -- Add purple equipment directly to player's hand using AddItemEx
    -- AddItemEx(genre, detail, particular, level, series, luck, mag1, mag2, mag3, mag4, mag5, mag6, version, randseed, pos)
    -- pos = -1 for hand, 15 for pos_builditem
    local bSuccess = AddItemEx(
        1,           -- genre = 1 (item_purpleequip)
        nDetail,     -- detail type (weapon, armor, etc.)
        nParti,      -- particular type
        nLevel,      -- level
        nSeries,     -- series
        nPurpleLuck, -- luck (>= 1000000000 = purple)
        tbMagicLevel[1] or 0,  -- magic attribute 1 level
        tbMagicLevel[2] or 0,  -- magic attribute 2 level
        tbMagicLevel[3] or 0,  -- magic attribute 3 level
        tbMagicLevel[4] or 0,  -- magic attribute 4 level
        tbMagicLevel[5] or 0,  -- magic attribute 5 level
        tbMagicLevel[6] or 0,  -- magic attribute 6 level
        1,           -- version (1 for exact mode)
        0,           -- random seed
        -1           -- pos = -1 (add to hand)
    )

    if not bSuccess or bSuccess == 0 then
        WriteLog("[COMPOUND_FORGE] ERROR: Failed to create purple equipment (AddItemEx returned nil or 0)")
        Talk(1, "", "<color=red>Loi: Khong the tao trang bi tim!<color>")
        return
    end

    WriteLog(string.format("[COMPOUND_FORGE] Created purple equipment successfully"))

    -- Remove source items from build slots
    -- SetPOItem(pos, slot, 0) to clear slot
    SetPOItem(nPos, 0, 0)  -- Clear equipment slot
    SetPOItem(nPos, 1, 0)  -- Clear crystal slot

    WriteLog("[COMPOUND_FORGE] Removed source items from build slots")

    -- Success message
    local szMsg = string.format("<color=green>Che tao thanh cong trang bi tim voi %d dong thuoc tinh!<color>", nNumLines)
    Talk(1, "", szMsg)

    WriteLog("[COMPOUND_FORGE] ExeCompoundForge() completed successfully")
end
