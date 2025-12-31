-- Compound Master NPC
-- Opens the Compound UI for purple item crafting

Include("\\script\\lib\\TaskLib.lua")

-- Configuration
HUYEN_TINH_GENRE = 6      -- item_task
HUYEN_TINH_MIN_DETAIL = 74    -- Huyen Tinh Cap 1
HUYEN_TINH_MAX_DETAIL = 79    -- Huyen Tinh Cap 6
PURPLE_LUCK_FLAG = 1000000000

function main(NpcIndex)
    Say("Ta la tho ren huyen bien, co the giup nguoi che tao trang bi tim tu nhung vat pham thuong thuong. Hay chon loai dich vu nguoi can:", 4,
        "Che Tao Trang Bi Tim/open_compound",
        "Huong Dan/guide",
        "Gioi Thieu He Thong/introduce",
        "Ket Thuc Doi Thoai/no")
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

    -- Create purple equipment with encoded empty slots
    -- DecodePurple() decodes luck and randomSeed to get magic attribute records
    -- When record = 0, it creates "Chưa khảm nạm" (empty enchantable slot)
    -- luck = 1000000000 encodes to [0,0,0] for slots 3,4,5
    -- randomSeed = 1000000000 encodes to [0,0,0] for slots 0,1,2
    -- Result: All 6 slots will be "Chưa khảm nạm" (empty)
    local nPurpleLuck = 1000000000      -- Encodes to empty records [0,0,0]
    local nRandomSeed = 1000000000      -- Encodes to empty records [0,0,0]

    -- Create purple equipment using AddItemEx with encoded empty slots
    local nPurpleIdx = AddItemEx(
        1,           -- genre = 1 (item_purpleequip) for purple equipment
        nDetail,     -- detail type (weapon, armor, etc.)
        nParti,      -- particular type
        nLevel,      -- level
        nSeries,     -- series
        nPurpleLuck, -- luck = 1000000000 (encodes slots 3,4,5 as empty)
        10,          -- magic attribute levels (need > 0 to create slots)
        10,          -- but records=0 will override to create empty slots
        10,
        10,
        10,
        10,
        1,           -- version = 1
        nRandomSeed  -- randomSeed = 1000000000 (encodes slots 0,1,2 as empty)
    )

    if not nPurpleIdx or nPurpleIdx <= 0 then
        Talk(1, "", "<color=red>Loi: Khong the tao trang bi tim!<color>")
        return
    end

    -- Remove source items from build slots using DelItemByIndex
    DelItemByIndex(nEquipIdx)   -- Delete equipment
    DelItemByIndex(nCrystalIdx) -- Delete crystal

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
    Msg2Player(format("[EXTRACT DEBUG] Calling AddItemEx with: Genre=7, Detail=%d, Series=%d, GenLvl=[%d,%d,%d,%d,%d,%d]", nKhoangDetail, nEquipSeries or -999, nOp, nValueMin, nValueMax, nValue, nEquipSeries, 0))

    -- Create new khoang thach item FIRST (before deleting anything)
    -- Pass generator levels directly in AddItemEx to ensure ITEM_SYNC includes attributes
    -- GenLvl[0]=Type, [1]=Min, [2]=Max, [3]=Value, [4]=Series, [5]=unused
    local nNewKhoangIdx = AddItemEx(
        7,                   -- genre = 7 (script items)
        nKhoangDetail,       -- detail = same as original khoang thach (146-151)
        0,                   -- particular
        0,                   -- level
        nEquipSeries,        -- series = KEEP equipment series (Kim/Moc/Thuy/Hoa/Tho)
        0,                   -- luck
        nOp, nValueMin, nValueMax, nValue, nEquipSeries, 0,   -- ma1-ma6: Type,Min,Max,Value,Series,unused
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
    -- DEBUG: Function entry log
    Msg2Player("=== ExeEnchaseAttribute CALLED ===")
    WriteLog("[LUA ENCHASE] ExeEnchaseAttribute function started")

    local nPos = 15  -- pos_builditem

    -- Get items from UI slots
    local nPurpleIdx = GetPOItem(nPos, 0)    -- BigBox (slot 0) = Purple equipment
    local nHuyenTinhIdx = GetPOItem(nPos, 1) -- Box1 (slot 1) = Huyen Tinh (catalyst)
    local nKhoangIdx = GetPOItem(nPos, 2)    -- Box2 (slot 2) = Khoang thach (attribute)

    -- DEBUG: Log retrieved item indices
    Msg2Player(format("[LUA ENCHASE] Items: Purple=%d, HuyenTinh=%d, Khoang=%d",
        nPurpleIdx or -1, nHuyenTinhIdx or -1, nKhoangIdx or -1))

    -- Validate purple item exists
    if not nPurpleIdx or nPurpleIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat TRANG BI TIM vao o lon!<color>")
        return
    end

    -- Get purple item properties
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nPurpleIdx)

    -- Validate purple item (genre = 1 = item_purpleequip)
    if nGenre ~= 1 then
        Talk(1, "", "<color=red>O lon phai la TRANG BI TIM!<color>")
        return
    end

    -- Validate Huyen Tinh exists and is level 3+
    if not nHuyenTinhIdx or nHuyenTinhIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat HUYEN TINH CAP 3+ vao o 1!<color>")
        return
    end

    local nHTGenre, nHTDetail = GetItemProp(nHuyenTinhIdx)

    -- Must be Huyen Tinh (genre = 6, detail = 76-79 for levels 3-6)
    if nHTGenre ~= HUYEN_TINH_GENRE or nHTDetail < 76 or nHTDetail > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>O 1 phai la HUYEN TINH CAP 3 TRO LEN!<color>")
        return
    end

    -- Validate Khoang thach exists
    if not nKhoangIdx or nKhoangIdx <= 0 then
        Talk(1, "", "<color=red>Vui long dat KHOANG THACH vao o 2!<color>")
        return
    end

    -- Get khoang thach properties
    local nKGenre, nKDetail = GetItemProp(nKhoangIdx)

    -- Validate khoang thach (genre=7, detail=146-151)
    if nKGenre ~= 7 or nKDetail < 146 or nKDetail > 151 then
        Talk(1, "", "<color=red>Khoang thach khong hop le (phai la detail 146-151)!<color>")
        return
    end

    -- Get attribute from khoang thach using GetItemGeneratorLevels
    -- Returns table with [1]=Type, [2]=Min, [3]=Max, [4]=Value, [5]=Series, [6]=unused
    local GenLvl = GetItemGeneratorLevels(nKhoangIdx)

    if not GenLvl or not GenLvl[1] or GenLvl[1] <= 0 then
        Talk(1, "", "<color=red>Khoang thach khong chua thuoc tinh!<color>")
        return
    end

    local nType = GenLvl[1]        -- Attribute type
    local nMin = GenLvl[2]         -- Min value
    local nMax = GenLvl[3]         -- Max value
    local nValue = GenLvl[4]       -- Current value
    local nAttrSeries = GenLvl[5]  -- Series from attribute (not used for enchase)

    -- Map khoang detail to slot: 146→0, 147→1, 148→2, 149→3, 150→4, 151→5
    local nSlot = nKDetail - 146

    Msg2Player(format("[ENCHASE DEBUG] Khoang Detail=%d → Slot=%d, Attrib=[Type=%d, Min=%d, Max=%d, Value=%d]",
        nKDetail, nSlot, nType, nMin, nMax, nValue))

    -- Success rate (100% for testing - can be changed later)
    local nSuccessRate = 100  -- TODO: Adjust success rate based on game balance
    local nRand = random(1, 100)

    if nRand <= nSuccessRate then
        -- SUCCESS: Write attribute to purple item at the matching slot
        -- SetItemMagicAttrib(itemIdx, slot, type, min, max, value)
        local bSuccess = SetItemMagicAttrib(nPurpleIdx, nSlot, nType, nMin, nMax, nValue)

        if not bSuccess or bSuccess == 0 then
            Talk(1, "", "<color=red>Loi: Khong the kham nam thuoc tinh vao dong " .. (nSlot + 1) .. "!<color>")
            return
        end

        -- Delete consumed items after successful enchase
        DelItemByIndex(nKhoangIdx)    -- Delete khoang thach
        DelItemByIndex(nHuyenTinhIdx) -- Delete Huyen Tinh catalyst

        Msg2Player(format("<color=green>Kham nam THANH CONG dong %d!<color>", nSlot + 1))
        Talk(1, "", format("<color=green>Da kham nam thanh cong dong %d vao trang bi tim!<color>", nSlot + 1))
        Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da kham nam thanh cong dong " .. (nSlot + 1) .. " vao trang bi tim!")
    else
        -- FAILURE: Delete both khoang thach and Huyen Tinh, purple item unchanged
        DelItemByIndex(nKhoangIdx)
        DelItemByIndex(nHuyenTinhIdx)

        Msg2Player(format("<color=red>Kham nam THAT BAI dong %d! Mat nguyen lieu!<color>", nSlot + 1))
        Talk(1, "", "<color=yellow>Kham nam that bai! Da mat Khoang thach va Huyen Tinh!<color>")
    end
end

function no()
end
