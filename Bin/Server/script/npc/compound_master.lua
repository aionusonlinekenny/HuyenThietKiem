-- Compound Master NPC
-- Opens the Compound UI for purple item crafting

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\item_helpers.lua")

-- Configuration
HUYEN_TINH_GENRE = 6      -- item_task
HUYEN_TINH_MIN_DETAIL = 74    -- Huyen Tinh Cap 1
HUYEN_TINH_MAX_DETAIL = 79    -- Huyen Tinh Cap 6

-- DEPRECATED: Old luck-based detection (kept for reference only)
-- PURPLE_LUCK_FLAG = 1000000000

function main(NpcIndex)
    Say("Ta lµ thÓ rÃn huy“n b›n, c„ th” giÛp ng≠Íi ch’ tπo trang bﬁ t›m tı nh˜ng vÀt ph»m th≠Íng th≠Íng. H∑y ch‰n loπi dﬁch vÙ ng≠Íi c«n:", 4,
        "Ch’ Tπo Trang Bﬁ T›m/open_compound",
        "H≠Ìng D…n/guide",
        "GiÌi Thi÷u H÷ ThËng/introduce",
        "K’t ThÛc ßËi Thoπi/no")
end


function open_compound()
    -- OpenCompoundUI is a C++ function exported to Lua
    -- This opens the Compound UI window
    OpenCompoundUI()
end

function guide()
    Talk(1, "", [[
<color=yellow>=== HUONG DAN CHE TAO TRANG BI TIM ===<color>

<color=green>1. Che Tao Trang Bi Tim (Forge):<color>
- Can: 1 trang bi xanh + 1-7 huyen tinh
- Ket qua: Trang bi tim voi cac dong thuoc tinh trong

<color=cyan>2. Kham Nam Thuoc Tinh (Enchase):<color>
- Can: Trang bi tim + khoang thach thuoc tinh
- Ket qua: Ep thuoc tinh vao trang bi tim

<color=orange>3. Hop Thanh Huyen Tinh (Compound):<color>
- Can: 3 huyen tinh cung cap
- Ket qua: Huyen tinh cao cap hon

<color=purple>4. Chiet Xuat (Distill):<color>
- Chiet xuat khoang thach thuoc tinh tu trang bi

<color=red>Luu y:<color>
- Ty le thanh cong phu thuoc vao so luong va cap do huyen tinh
- That bai co the mat nguyen lieu
]])

    Say("", 2,
        "Mo Giao Dien Che Tao/open_compound",
        "Quay Lai/main")
end

function introduce()
    Talk(1, "", [[
<color=yellow>=== HE THONG CHE TAO TRANG BI TIM ===<color>

Trang bi tim la trang bi cao cap nhat trong tro choi, vuot troi hon ca trang bi xanh.

<color=green>Uu diem cua Trang Bi Tim:<color>
- Co nhieu dong thuoc tinh hon trang bi xanh
- Co the kham nam them thuoc tinh
- Suc manh vuot troi, phu hop cho PvP va PvE

<color=cyan>Huyen Tinh:<color>
Co 17 loai huyen tinh, chia lam 4 cap:
- <color=cyan>Lam Thuy Tinh<color> (Cap 1-3): Cap thap
- <color=purple>Tu Thuy Tinh<color> (Cap 1-4): Cap trung
- <color=green>Luc Thuy Tinh<color> (Cap 1-5): Cap cao
- <color=red>Hong Thuy Tinh<color> (Cap 1-5): Huyen thoai

So luong va cap do huyen tinh quyet dinh chat luong trang bi tim!
]])

    Say("", 2,
        "Mo Giao Dien Che Tao/open_compound",
        "Quay Lai/main")
end

-- ------------------------------------------------------------
-- Execute Forge (called when player clicks "Che Tao" button)
-- ------------------------------------------------------------
function ExeCompoundForge()
    local nPos = 15  -- pos_builditem (same container as Tremble/Upgrade)

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)      -- BigBox = slot 0 (UIEP_BUILDITEM1)
    local nCrystalIdx = GetPOItem(nPos, 1)    -- SmallBox = slot 1 (UIEP_BUILDITEM2)

    -- Validate equipment exists
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat trang bi vao o lon!<color>")
        return
    end

    -- Validate crystal exists
    if not nCrystalIdx or nCrystalIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat Huyen Tinh vao o nho!<color>")
        return
    end

    -- Get equipment properties
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    -- Debug: Log blue item series
    local szSeriesNames = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    Msg2Player(format("[SERIES DEBUG] Blue item series = %d (%s)", nSeries or -1, szSeriesNames[nSeries + 1] or "Unknown"))

    -- Validate equipment is blue (using new genre-based detection)
    if not IsBlueEquipment(nEquipIdx) then
        local itemType = GetItemTypeByGenre(nEquipIdx)
        if itemType == "purple" or itemType == "gold" or itemType == "platinum" then
            Talk(1, "", "<color=red>Trang bi nay da la tim/vang, khong the che tao lai!<color>")
        else
            Talk(1, "", "<color=red>Chi duoc dat trang bi xanh vao o lon!<color>")
        end
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

    -- Determine number of magic attribute lines based on crystal level
    local nNumLines = 0
    if nCrystalLevel >= 5 then
        -- Level 5-6: Guaranteed 6 lines
        nNumLines = 6
    else
        -- Level 1-4: Random based on crystal level
        -- Level 1: 2-3, Level 2: 2-4, Level 3: 3-5, Level 4: 3-6
        local nMinLines = 2
        if nCrystalLevel >= 3 then
            nMinLines = 3
        end
        local nMaxLines = nCrystalLevel + 2
        nNumLines = random(nMinLines, nMaxLines)
    end

    -- CRITICAL: Delete source items BEFORE creating purple item!
    -- If we create purple item first, it changes inventory indices and
    -- nEquipIdx/nCrystalIdx become invalid, causing DelItemByIndex to fail
    DelItemByIndex(nEquipIdx)   -- Delete equipment
    DelItemByIndex(nCrystalIdx) -- Delete crystal

    -- Create purple equipment using dedicated AddItemPurple function
    -- This function handles all the complexity internally:
    -- - Sets genre = item_purpleequip
    -- - Sets luck = 1000000000 (new purple item marker)
    -- - Sets randomSeed = 1000000000 (for DecodePurple to generate Type=53 empty slots)
    -- - Calls Gen_PurpleEquipment to generate 6 "Chua kham nam" slots
    -- - Adds item directly to player's equipment room
    Msg2Player(format("[SERIES DEBUG] Creating purple with series = %d (%s)", nSeries or -1, szSeriesNames[nSeries + 1] or "Unknown"))
    local nPurpleIdx = AddItemPurple(nDetail, nParti, nLevel, nSeries)

    if not nPurpleIdx or nPurpleIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Khong the tao trang bi tim!<color>")
        return
    end

    -- Verify purple item series
    local _, _, _, _, nPurpleSeries = GetItemProp(nPurpleIdx)
    Msg2Player(format("[SERIES DEBUG] Purple item created with series = %d (%s)", nPurpleSeries or -1, szSeriesNames[nPurpleSeries + 1] or "Unknown"))

    if nPurpleSeries ~= nSeries then
        Msg2Player(format("<color=red>[SERIES ERROR] Series mismatch! Blue=%d, Purple=%d<color>", nSeries, nPurpleSeries))
    end

    -- Success message
    local szMsg = "<color=green>Che tao thanh cong trang bi tim voi " .. nNumLines .. " dong thuoc tinh!<color>"
    Talk(1, "", szMsg)
    Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da hop thanh <color=purple>TRANG BI TIM<color> voi " .. nNumLines .. " dong!")
end

-- ------------------------------------------------------------
-- Execute Compound Equipment (Mode 1 - Equipment to Huyen Tinh)
-- Called when player clicks "Tinh Luyen" in COMPOUND tab Mode 1
-- ------------------------------------------------------------
function ExeCompoundEquipment()
    local nPos = 15  -- pos_builditem

    -- Get items from 3 UI slots
    local nRingIdx = GetPOItem(nPos, 0)      -- Box1 = slot 0 (ring)
    local nNecklaceIdx = GetPOItem(nPos, 1)  -- Box2 = slot 1 (necklace)
    local nPendantIdx = GetPOItem(nPos, 2)   -- Box3 = slot 2 (pendant)

    -- Validate all 3 items exist
    if not nRingIdx or nRingIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat NHAN vao o 1!<color>")
        return
    end

    if not nNecklaceIdx or nNecklaceIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat DAY CHUYEN vao o 2!<color>")
        return
    end

    if not nPendantIdx or nPendantIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat NGOC BOI vao o 3!<color>")
        return
    end

    -- Get properties of all 3 items
    local nRingGenre, nRingDetail, nRingParti, nRingLevel, nRingSeries = GetItemProp(nRingIdx)
    local nNeckGenre, nNeckDetail, nNeckParti, nNeckLevel, nNeckSeries = GetItemProp(nNecklaceIdx)
    local nPendGenre, nPendDetail, nPendParti, nPendLevel, nPendSeries = GetItemProp(nPendantIdx)

    -- Validate item types (should already be validated by client, but double-check)
    -- equip_ring=3, equip_amulet=4, equip_pendant=9
    if nRingDetail ~= 3 then
        Talk(1, "", "<color=red>O 1 phai la NHAN!<color>")
        return
    end

    if nNeckDetail ~= 4 then
        Talk(1, "", "<color=red>O 2 phai la DAY CHUYEN!<color>")
        return
    end

    if nPendDetail ~= 9 then
        Talk(1, "", "<color=red>O 3 phai la NGOC BOI!<color>")
        return
    end

    -- Calculate average level of 3 items
    -- No need to floor - comparisons work fine with decimal values
    local nAvgLevel = (nRingLevel + nNeckLevel + nPendLevel) / 3

    -- Determine Huyen Tinh level (1-6) based on average equipment level
    -- Level mapping:
    -- Equip Avg 1-20:   70% chance level 1, 25% level 2, 5% level 3
    -- Equip Avg 21-40:  50% level 2, 35% level 3, 15% level 4
    -- Equip Avg 41-60:  40% level 3, 40% level 4, 15% level 5, 5% level 6
    -- Equip Avg 61-80:  30% level 4, 40% level 5, 25% level 6, 5% level 7 (cap at 6)
    -- Equip Avg 81+:    20% level 5, 50% level 6, 30% level 7 (cap at 6)

    local nHuyenTinhLevel = 1
    local nRand = random(1, 100)

    if nAvgLevel <= 20 then
        -- Low level: mostly level 1
        if nRand <= 70 then
            nHuyenTinhLevel = 1
        elseif nRand <= 95 then
            nHuyenTinhLevel = 2
        else
            nHuyenTinhLevel = 3
        end
    elseif nAvgLevel <= 40 then
        -- Mid-low level
        if nRand <= 50 then
            nHuyenTinhLevel = 2
        elseif nRand <= 85 then
            nHuyenTinhLevel = 3
        else
            nHuyenTinhLevel = 4
        end
    elseif nAvgLevel <= 60 then
        -- Mid level
        if nRand <= 40 then
            nHuyenTinhLevel = 3
        elseif nRand <= 80 then
            nHuyenTinhLevel = 4
        elseif nRand <= 95 then
            nHuyenTinhLevel = 5
        else
            nHuyenTinhLevel = 6
        end
    elseif nAvgLevel <= 80 then
        -- Mid-high level
        if nRand <= 30 then
            nHuyenTinhLevel = 4
        elseif nRand <= 70 then
            nHuyenTinhLevel = 5
        else
            nHuyenTinhLevel = 6
        end
    else
        -- High level: better chances for level 5-6
        if nRand <= 20 then
            nHuyenTinhLevel = 5
        else
            nHuyenTinhLevel = 6
        end
    end

    -- Create Huyen Tinh item
    -- Genre = 6 (item_task), Detail = 74-79 (level 1-6)
    local nHuyenTinhDetail = 73 + nHuyenTinhLevel
    local nHuyenTinhIdx = AddItemEx(
        HUYEN_TINH_GENRE,    -- genre = 6 (item_task)
        nHuyenTinhDetail,    -- detail = 74-79 (level 1-6)
        0,                   -- particular
        0,                   -- level
        0,                   -- series
        0,                   -- luck
        0, 0, 0, 0, 0, 0,   -- magic attributes
        1,                   -- version
        0                    -- randomSeed
    )

    if not nHuyenTinhIdx or nHuyenTinhIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Khong the tao Huyen Tinh!<color>")
        return
    end

    -- Remove source items
    DelItemByIndex(nRingIdx)
    DelItemByIndex(nNecklaceIdx)
    DelItemByIndex(nPendantIdx)

    -- Success message
    local szLevelName = {"I", "II", "III", "IV", "V", "VI"}
    local szMsg = "<color=green>Hop thanh thanh cong Huyen Tinh cap " .. szLevelName[nHuyenTinhLevel] .. "!<color>"
    Talk(1, "", szMsg)
    Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da hop thanh <color=cyan>HUYEN TINH CAP " .. szLevelName[nHuyenTinhLevel] .. "<color> tu 3 trang bi!")
end

-- ------------------------------------------------------------
-- Execute Compound Crystal (Mode 2 - Crystal Upgrading)
-- Called when player clicks "Tinh Luyen" in COMPOUND tab Mode 2
-- ------------------------------------------------------------
function ExeCompoundCrystal()
    local nPos = 15  -- pos_builditem

    -- Get items from 3 UI slots
    local nCrystal1Idx = GetPOItem(nPos, 0)  -- Box1 = slot 0
    local nCrystal2Idx = GetPOItem(nPos, 1)  -- Box2 = slot 1
    local nCrystal3Idx = GetPOItem(nPos, 2)  -- Box3 = slot 2

    -- Validate all 3 items exist
    if not nCrystal1Idx or nCrystal1Idx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH vao o 1!<color>")
        return
    end

    if not nCrystal2Idx or nCrystal2Idx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH vao o 2!<color>")
        return
    end

    if not nCrystal3Idx or nCrystal3Idx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH vao o 3!<color>")
        return
    end

    -- Get properties of all 3 crystals
    local nGenre1, nDetail1 = GetItemProp(nCrystal1Idx)
    local nGenre2, nDetail2 = GetItemProp(nCrystal2Idx)
    local nGenre3, nDetail3 = GetItemProp(nCrystal3Idx)

    -- Validate all are Huyen Tinh (genre = 6)
    if nGenre1 ~= HUYEN_TINH_GENRE then
        Talk(1, "", "<color=red>O 1 phai la HUYEN TINH!<color>")
        return
    end

    if nGenre2 ~= HUYEN_TINH_GENRE then
        Talk(1, "", "<color=red>O 2 phai la HUYEN TINH!<color>")
        return
    end

    if nGenre3 ~= HUYEN_TINH_GENRE then
        Talk(1, "", "<color=red>O 3 phai la HUYEN TINH!<color>")
        return
    end

    -- Validate all are within valid range (74-79 for levels 1-6)
    if nDetail1 < HUYEN_TINH_MIN_DETAIL or nDetail1 > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 1 phai la Huyen Tinh cap 1-6!<color>")
        return
    end

    if nDetail2 < HUYEN_TINH_MIN_DETAIL or nDetail2 > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 2 phai la Huyen Tinh cap 1-6!<color>")
        return
    end

    if nDetail3 < HUYEN_TINH_MIN_DETAIL or nDetail3 > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 3 phai la Huyen Tinh cap 1-6!<color>")
        return
    end

    -- Check that all 3 crystals are the same level
    if nDetail1 ~= nDetail2 or nDetail1 ~= nDetail3 then
        Talk(1, "", "<color=red>Ca 3 Huyen Tinh phai cung cap!<color>")
        return
    end

    -- Check if already at max level (detail = 79 = level 6)
    if nDetail1 >= HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>Huyen Tinh da dat cap toi da, khong the nang cap!<color>")
        return
    end

    -- Calculate success rate based on crystal level
    -- Level 1: 80% success, Level 2: 70%, Level 3: 60%, Level 4: 50%, Level 5: 40%
    local nCrystalLevel = nDetail1 - 73
    local nSuccessRate = 90 - (nCrystalLevel * 10)  -- 80, 70, 60, 50, 40

    -- Random check for success/failure
    local nRand = random(1, 100)
    local bSuccess = (nRand <= nSuccessRate)

    -- Always delete the 3 source crystals
    DelItemByIndex(nCrystal1Idx)
    DelItemByIndex(nCrystal2Idx)
    DelItemByIndex(nCrystal3Idx)

    if bSuccess then
        -- Create higher level crystal (detail + 1)
        local nNewDetail = nDetail1 + 1
        local nNewCrystalIdx = AddItemEx(
            HUYEN_TINH_GENRE,    -- genre = 6 (item_task)
            nNewDetail,          -- detail = current + 1
            0,                   -- particular
            0,                   -- level
            0,                   -- series
            0,                   -- luck
            0, 0, 0, 0, 0, 0,   -- magic attributes
            1,                   -- version
            0                    -- randomSeed
        )

        if not nNewCrystalIdx or nNewCrystalIdx <= 0 then
            Talk(1, "", "<color=red>Loi: Khong the tao Huyen Tinh!<color>")
            return
        end

        -- Success message
        local szLevelName = {"I", "II", "III", "IV", "V", "VI"}
        local nNewLevel = nNewDetail - 73
        local szMsg = "<color=green>Nang cap thanh cong! Nhan duoc Huyen Tinh cap " .. szLevelName[nNewLevel] .. "!<color>"
        Talk(1, "", szMsg)
        Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da nang cap thanh cong <color=cyan>HUYEN TINH CAP " .. szLevelName[nNewLevel] .. "<color>!")
    else
        -- Failure message
        Talk(1, "", "<color=red>Nang cap that bai! Mat tat ca Huyen Tinh!<color>")
        Msg2SubWorld("<color=red> " .. GetName() .. "<color> da that bai khi nang cap Huyen Tinh!")
    end
end

-- ------------------------------------------------------------
-- Execute Extract Attribute (DISTILL Tab)
-- Called when player clicks "Tinh Luyen" in DISTILL tab
-- Extracts attribute from green equipment and stores in khoang thach
-- ------------------------------------------------------------
function ExeExtractAttribute()
    local nPos = 15  -- pos_builditem

    -- Get items from UI slots
    local nEquipIdx = GetPOItem(nPos, 0)     -- BigBox (slot 0) = Green equipment
    local nHuyenTinhIdx = GetPOItem(nPos, 1) -- Box1 (slot 1) = Huyen Tinh
    local nKhoangIdx = GetPOItem(nPos, 2)    -- Box2 (slot 2) = Khoang thach

    -- Validate all 3 items exist
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat TRANG BI XANH vao o lon!<color>")
        return
    end

    if not nHuyenTinhIdx or nHuyenTinhIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH vao o 1!<color>")
        return
    end

    if not nKhoangIdx or nKhoangIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat KHOANG THACH vao o 2!<color>")
        return
    end

    -- Get properties (including series for element checking)
    local nEquipGenre, nEquipDetail, _, _, nEquipSeries = GetItemProp(nEquipIdx)
    local nHTGenre, nHTDetail = GetItemProp(nHuyenTinhIdx)
    local nKhoangGenre, nKhoangDetail = GetItemProp(nKhoangIdx)

    -- Validate equipment
    if nEquipGenre ~= 0 then  -- item_equip = 0
        Talk(1, "", "<color=red>O lon phai la TRANG BI!<color>")
        return
    end

    -- Validate Huyen Tinh
    if nHTGenre ~= HUYEN_TINH_GENRE or nHTDetail < HUYEN_TINH_MIN_DETAIL or nHTDetail > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 1 phai la HUYEN TINH cap 1-6!<color>")
        return
    end

    -- Validate Khoang thach (genre 7, detail 146-151)
    if nKhoangGenre ~= 7 or nKhoangDetail < 146 or nKhoangDetail > 151 then
        Talk(1, "", "<color=red>O 2 phai la KHOANG THACH (detail 146-151)!<color>")
        return
    end

    -- Map khoang thach detail to attribute POSITION (user wants 1st, 2nd, 3rd visible attribute):
    -- 146 = 1st visible attribute
    -- 147 = 2nd visible attribute
    -- 148 = 3rd visible attribute
    -- etc.
    local nTargetPosition = nKhoangDetail - 145  -- 146->1, 147->2, 148->3, 149->4, 150->5, 151->6

    -- Debug: enumerate ALL attribute slots to understand equipment structure
    Msg2Player(format("[EXTRACT DEBUG] Equipment Series=%d, KhoangDetail=%d, TargetPosition=%d", nEquipSeries or -999, nKhoangDetail, nTargetPosition))
    Msg2Player("[EXTRACT DEBUG] Enumerating ALL equipment attributes:")

    -- Find the Nth visible attribute by enumerating all 6 slots
    local nCurrentPosition = 0
    local nOp, nValue, nValueMin, nValueMax = 0, 0, 0, 0
    local nFoundSlot = -1

    for nSlot = 0, 5 do
        local nSlotOp, nSlotValue, nSlotMin, nSlotMax = GetItemMagicAttribInfo(nEquipIdx, nSlot)

        -- Debug: show ALL slots
        Msg2Player(format("  Slot[%d]: Type=%d, Value=%d, Min=%d, Max=%d", nSlot, nSlotOp or 0, nSlotValue or 0, nSlotMin or 0, nSlotMax or 0))

        -- Count non-empty attributes (Type > 0 means attribute exists)
        if nSlotOp and nSlotOp > 0 then
            nCurrentPosition = nCurrentPosition + 1

            -- Is this the attribute position we want?
            if nCurrentPosition == nTargetPosition then
                nOp = nSlotOp
                nValue = nSlotValue
                nValueMin = nSlotMin
                nValueMax = nSlotMax
                nFoundSlot = nSlot
                Msg2Player(format("[EXTRACT DEBUG] FOUND! Position %d is in Slot %d: Type=%d, Value=%d, Min=%d, Max=%d", nTargetPosition, nSlot, nOp, nValue, nValueMin, nValueMax))
                break
            end
        end
    end

    -- Check if we found the target attribute
    if nFoundSlot == -1 or nOp <= 0 then
        Talk(1, "", format("<color=red>Trang bi khong co thuoc tinh thu %d!<color>", nTargetPosition))
        return
    end

    -- Use the ACTUAL value from equipment, not calculated average!
    -- nValue is the equipment's current attribute value (e.g., 74)
    Msg2Player(format("[EXTRACT DEBUG] Using ACTUAL value from equipment: Value=%d (NOT average)", nValue))

    -- Debug: log AddItemEx parameters (all 6 generator levels)
    Msg2Player(format("[EXTRACT DEBUG] Calling AddItemEx with: Genre=7, Detail=%d, Series=%d, GenLvl=[%d,%d,%d,%d,%d,%d]", nKhoangDetail, nEquipSeries or -999, nOp, nValueMin, nValueMax, nValue, nEquipSeries, nEquipDetail))

    -- Create new khoang thach item FIRST (before deleting anything)
    -- Pass generator levels directly in AddItemEx to ensure ITEM_SYNC includes attributes
    -- GenLvl[0]=Type, [1]=Min, [2]=Max, [3]=Value, [4]=Series, [5]=SourceEquipDetail
    -- GenLvl[5] stores source equipment detail type (0=meleeweapon, 1=rangeweapon, 2+=other)
    -- This is used to enforce weapon attribute restriction: weapon attrs can only go on weapons
    local nNewKhoangIdx = AddItemEx(
        7,                   -- genre = 7 (script items)
        nKhoangDetail,       -- detail = same as original khoang thach (146-151)
        0,                   -- particular
        0,                   -- level
        nEquipSeries,        -- series = KEEP equipment series (Kim/Moc/Thuy/Hoa/Tho)
        0,                   -- luck
        nOp, nValueMin, nValueMax, nValue, nEquipSeries, nEquipDetail,   -- ma1-ma6: Type,Min,Max,Value,Series,SourceEquipDetail
        1,                   -- version
        0                    -- randomSeed
    )

    if not nNewKhoangIdx or nNewKhoangIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Khong the tao Khoang thach!<color>")
        return
    end

    -- Debug: check what series the new item actually has
    local _, _, _, _, nNewSeries = GetItemProp(nNewKhoangIdx)
    Msg2Player(format("[EXTRACT DEBUG] Created khoang thach idx=%d, checking series: Expected=%d, Actual=%d", nNewKhoangIdx, nEquipSeries or -999, nNewSeries or -999))

    -- Set m_aryMagicAttrib directly for server-side logic
    -- Pass the 6th parameter (nValue) to use ACTUAL value instead of average
    local bSuccess = SetItemMagicAttrib(nNewKhoangIdx, 0, nOp, nValueMin, nValueMax, nValue)
    if not bSuccess or bSuccess == 0 then
        Talk(1, "", "<color=red>Loi: Khong the luu thuoc tinh vao Khoang thach!<color>")
        -- Delete the failed new item before returning
        DelItemByIndex(nNewKhoangIdx)
        return
    end

    -- Only NOW delete the source items (after successful creation)
    DelItemByIndex(nEquipIdx)
    DelItemByIndex(nHuyenTinhIdx)
    DelItemByIndex(nKhoangIdx)

    -- No need for explicit SyncItem - AddItemEx already synced with generator levels!

    -- Success message
    local szKhoangName = {
        "Huyen Thiet Nguyen Khoang",   -- 146
        "Khong Tuoc Nguyen Thach",     -- 147
        "Mat Ngan Nguyen Khoang",      -- 148
        "Phu Dung Nguyen Thach",       -- 149
        "Chu Sa Nguyen Khoang",        -- 150
        "Chung Nho Nguyen Thach"       -- 151
    }
    local nKhoangIdx = nKhoangDetail - 145  -- 146->1, 147->2, etc.
    local szMsg = "<color=green>Chiet xuat thanh cong! Nhan duoc <color=yellow>" .. szKhoangName[nKhoangIdx] .. "<color=green> chua thuoc tinh!<color>"
    Talk(1, "", szMsg)
    Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da chiet xuat thanh cong thuoc tinh tu trang bi!")
end

-- ------------------------------------------------------------
-- Execute Enchase Attribute (ENCHASE Tab)
-- Called when player clicks button in ENCHASE tab
-- Inlays khoang thach attribute into purple item at matching slot
-- ------------------------------------------------------------
function ExeEnchaseAttribute()
    local nPos = 15

    local nPurpleIdx = GetPOItem(nPos, 0)
    local nHuyenTinhIdx = GetPOItem(nPos, 1)
    local nKhoangIdx = GetPOItem(nPos, 2)

    -- Validate purple item
    if not nPurpleIdx or nPurpleIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat TRANG BI TIM vao o lon!<color>")
        return
    end

    if not IsPurpleEquipment(nPurpleIdx) then
        Talk(1, "", "<color=red>O lon phai la TRANG BI TIM!<color>")
        return
    end

    -- Get purple item properties (including series)
    local nPurpleGenre, nPurpleDetail, nPurpleParti, nPurpleLevel, nPurpleSeries = GetItemProp(nPurpleIdx)

    -- Validate Huyen Tinh
    if not nHuyenTinhIdx or nHuyenTinhIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH vao o 1!<color>")
        return
    end

    local nHTGenre, nHTDetail = GetItemProp(nHuyenTinhIdx)
    if nHTGenre ~= HUYEN_TINH_GENRE or nHTDetail < 76 or nHTDetail > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 1 phai la HUYEN TINH cap 4-6!<color>")
        return
    end

    -- Validate Khoang thach
    if not nKhoangIdx or nKhoangIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat KHOANG THACH vao o 2!<color>")
        return
    end

    local nKGenre, nKDetail = GetItemProp(nKhoangIdx)
    if nKGenre ~= 7 or nKDetail < 146 or nKDetail > 151 then
        Talk(1, "", "<color=red>O 2 phai la KHOANG THACH!<color>")
        return
    end

    -- Get attribute from khoang thach (including its series and source equipment detail)
    local nType, nMin, nMax, nValue, nKhoangSeries, nSourceEquipDetail = GetItemGeneratorLevels(nKhoangIdx)

    if not nType or nType <= 0 then
        Talk(1, "", "<color=red>Khoang thach khong chua thuoc tinh!<color>")
        return
    end

    -- ============================================================
    -- SEQUENTIAL SLOT FILLING VALIDATION
    -- ============================================================
    -- Must fill slots in order: 0 -> 1 -> 2 -> 3 -> 4 -> 5
    -- Cannot skip empty slots (e.g., cannot fill slot 2 if slot 0 or 1 is empty)
    -- ============================================================

    -- Calculate target slot (146->0, 147->1, ..., 151->5)
    local nTargetSlot = nKDetail - 146

    -- Check all slots before target slot to ensure they're filled
    for nSlot = 0, nTargetSlot - 1 do
        local nSlotType = GetItemMagicAttrib(nPurpleIdx, nSlot)
        if not nSlotType or nSlotType <= 0 or nSlotType == 53 then
            -- Found empty slot before target slot - reject enchasing
            Talk(1, "", format(
                "<color=red>Phai kham nam tuan tu! Slot %d con trong, khong the kham slot %d!<color>",
                nSlot + 1, nTargetSlot + 1
            ))
            Msg2Player(format("[SLOT ORDER] Cannot skip slot %d (empty) to fill slot %d", nSlot + 1, nTargetSlot + 1))
            return
        end
    end

    Msg2Player(format("[SLOT ORDER] All slots before slot %d are filled, can proceed", nTargetSlot + 1))

    -- ============================================================
    -- WEAPON ATTRIBUTE RESTRICTION
    -- ============================================================
    -- Attributes extracted from WEAPONS can ONLY be enchased into WEAPONS
    -- Attributes from other equipment can be enchased into any purple item
    --
    -- Equipment detail types:
    -- 0 = meleeweapon (vu khi can chien)
    -- 1 = rangeweapon (vu khi tam xa)
    -- 2+ = armor, ring, amulet, boots, belt, helm, cuff, pendant, etc.
    -- ============================================================

    local bSourceIsWeapon = (nSourceEquipDetail == 0 or nSourceEquipDetail == 1)
    local bTargetIsWeapon = (nPurpleDetail == 0 or nPurpleDetail == 1)

    if bSourceIsWeapon and not bTargetIsWeapon then
        -- Trying to put weapon attribute into non-weapon item - REJECT!
        local szSourceType = (nSourceEquipDetail == 0) and "Vu Khi Can Chien" or "Vu Khi Tam Xa"
        local szTargetType = "Trang Bi Khong Phai Vu Khi"
        Talk(1, "", format(
            "<color=red>Thuoc tinh tu %s chi co the kham vao VU KHI TIM!<color>\n<color=yellow>Khong the kham vao %s!<color>",
            szSourceType, szTargetType
        ))
        Msg2Player(format("[WEAPON RESTRICTION] Cannot enchase weapon attribute (detail=%d) into non-weapon (detail=%d)", nSourceEquipDetail, nPurpleDetail))
        return
    end

    if bSourceIsWeapon then
        Msg2Player(format("[WEAPON RESTRICTION] Weapon attribute ? Weapon target: OK (source=%d, target=%d)", nSourceEquipDetail, nPurpleDetail))
    else
        Msg2Player(format("[WEAPON RESTRICTION] Non-weapon attribute ? Any target: OK (source=%d, target=%d)", nSourceEquipDetail, nPurpleDetail))
    end

    -- ============================================================
    -- SERIES (NGU HANH) VALIDATION LOGIC
    -- ============================================================
    -- Minerals 147, 149, 151 (odd): MUST match purple item series
    -- Minerals 146, 148, 150 (even): Can be any series (no validation needed)
    --
    -- Series values: 0=Kim, 1=Moc, 2=Thuy, 3=Hoa, 4=Tho
    --
    -- NOTE: Series is an item-level property, not attribute-level.
    -- The enchased attribute inherits the series from the purple item automatically.
    -- We only validate that certain minerals require matching series before enchasing.
    -- ============================================================

    local szSeriesNames = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    local bStrictMineral = (nKDetail == 147 or nKDetail == 149 or nKDetail == 151)

    if bStrictMineral then
        -- Minerals 147/149/151: Series MUST match purple item series
        if nKhoangSeries ~= nPurpleSeries then
            local szPurpleSeries = szSeriesNames[nPurpleSeries + 1] or "Unknown"
            local szKhoangSeries = szSeriesNames[nKhoangSeries + 1] or "Unknown"
            Talk(1, "", format(
                "<color=red>Khoang thach (%s) khong cung ngu hanh voi trang bi (%s)! Khong the kham nam!<color>",
                szKhoangSeries, szPurpleSeries
            ))
            return
        end
        -- Success message for matching series
        Msg2Player(format(
            "[NGU HANH] Khoang thach va trang bi cung ngu hanh (%s), cho phep kham nam",
            szSeriesNames[nPurpleSeries + 1] or "Unknown"
        ))
    else
        -- Minerals 146/148/150: No series validation needed
        -- Allow enchasing regardless of mineral series
        Msg2Player(format(
            "[NGU HANH] Khoang thach loai %d khong can kiem tra ngu hanh, cho phep kham nam",
            nKDetail
        ))
    end

    -- CRITICAL FIX: Don't delete and recreate item!
    -- Just update the target slot directly on the existing purple item in BuildBox
    -- This preserves ALL existing Type=53 slots

    -- CRITICAL ORDER: Update attribute BEFORE deleting materials!
    -- SetPurpleItemMagicAttrib will call SyncPurpleItem which needs item to still
    -- be in m_Items[] array to find its BuildBox position (Place=15, X=0, Y=0).
    -- If we delete materials first, client receives s2cRemoveItem and clears BuildBox
    -- references, causing SyncPurpleItem to fail finding the item position!

    -- Update the target slot with new attribute from khoang thach
    -- This directly modifies Item[nPurpleIdx].m_aryMagicAttrib[nTargetSlot]
    -- AND calls SyncPurpleItem to send updated item to client
    SetPurpleItemMagicAttrib(nPurpleIdx, nTargetSlot, nType, nMin, nMax, nValue)

    -- NOW delete materials after purple item is synced
    -- Client will receive s2cRemoveItem for these and auto-clear Box1/Box2
    DelItemByIndex(nKhoangIdx)
    DelItemByIndex(nHuyenTinhIdx)

    -- Success message
    local szMsg = format("<color=yellow>Kham nam thanh cong! Thuoc tinh da duoc ep vao o thu %d!<color>", nTargetSlot + 1)
    Talk(1, "", szMsg)
    Msg2SubWorld("<pic=135><color=yellow> " .. GetName() .. "<color> da kham nam thanh cong thuoc tinh vao trang bi tim!")
end

function no()
end