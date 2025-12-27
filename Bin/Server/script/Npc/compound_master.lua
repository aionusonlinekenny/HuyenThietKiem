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

    -- Create purple equipment with random luck value
    local nPurpleLuck = PURPLE_LUCK_FLAG + random(1, 999)

    -- Add purple equipment to player's inventory using AddItemEx
    -- Use genre = 1 (item_purpleequip) to create purple equipment (NOT genre = 0)
    -- Pass nNumLines for first N slots, 0 for remaining to create empty enchantable slots
    local bSuccess = AddItemEx(
        1,           -- genre = 1 (item_purpleequip) for purple equipment
        nDetail,     -- detail type (weapon, armor, etc.)
        nParti,      -- particular type
        nLevel,      -- level
        nSeries,     -- series
        nPurpleLuck, -- luck (>= 1000000000 = purple metadata)
        nNumLines >= 1 and 1 or 0,  -- magic attribute 1 level (1 = has slot, 0 = no slot)
        nNumLines >= 2 and 1 or 0,  -- magic attribute 2 level
        nNumLines >= 3 and 1 or 0,  -- magic attribute 3 level
        nNumLines >= 4 and 1 or 0,  -- magic attribute 4 level
        nNumLines >= 5 and 1 or 0,  -- magic attribute 5 level
        nNumLines >= 6 and 1 or 0,  -- magic attribute 6 level
        1,           -- version (1 for exact mode)
        0            -- random seed (NO position param - auto add to inventory)
    )

    if not bSuccess or bSuccess == 0 then
        Talk(1, "", "<color=red>Loi: Khong the tao trang bi tim!<color>")
        return
    end

    -- Remove source items from build slots
    SetPOItem(nPos, 0, 0)  -- Clear equipment slot
    SetPOItem(nPos, 1, 0)  -- Clear crystal slot

    -- Success message
    local szMsg = string.format("<color=green>Che tao thanh cong trang bi tim voi %d dong thuoc tinh!<color>", nNumLines)
    Talk(1, "", szMsg)
    Msg2SubWorld(string.format("<pic=135><color=green> %s<color> da hop thanh <color=purple>TRANG BI TIM<color> voi %d dong!", GetName(), nNumLines))
end

function no()
end
