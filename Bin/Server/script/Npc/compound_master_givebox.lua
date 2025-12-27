-- Compound Master NPC
-- Opens GiveBox UI for purple item crafting (NO server rebuild needed!)

Include("\\script\\lib\\TaskLib.lua")

-- Configuration
HUYEN_TINH_GENRE = 6      -- item_task
HUYEN_TINH_MIN_DETAIL = 74    -- Huyen Tinh Cap 1
HUYEN_TINH_MAX_DETAIL = 79    -- Huyen Tinh Cap 6
PURPLE_LUCK_FLAG = 1000000000

function main(NpcIndex)
    Say("Tho Ren Huyen Bi: Ta co the hop thanh trang bi TIM tu trang bi xanh va Huyen Tinh!", 3,
        "Bat dau che tao/open_forge",
        "Huong dan/guide",
        "Ket thuc/no")
end

function open_forge()
    -- Use OpenGiveBox - ALREADY IMPLEMENTED in server, no rebuild needed!
    OpenGiveBox("Che Tao Trang Bi Tim",
                "Dat trang bi xanh vao o dau tien, Huyen Tinh vao o thu hai",
                "ExeCompoundForge")
end

function guide()
    Talk(1, "", [[
<color=yellow>=== HUONG DAN CHE TAO TRANG BI TIM ===<color>

<color=cyan>Nguyen lieu:<color>
- 1 trang bi xanh
- 1 Huyen Tinh

<color=cyan>Quy tac:<color>
- Huyen Tinh Cap 5-6: <color=green>100% ra 6 dong<color>
- Huyen Tinh Cap 1-4: Random 2-6 dong

<color=red>Luu y: Nguyen lieu se bi tieu hao!<color>
]])
    main()
end

-- Execute when player clicks confirm in GiveBox
function ExeCompoundForge()
    WriteLog("[COMPOUND_FORGE] ExeCompoundForge called via GiveBox")

    local ROOMG = 8  -- GiveBox room ID

    -- Get equipment from slot 0
    local nEquipIdx = GetROItem(ROOMG, 0)
    -- Get crystal from slot 1
    local nCrystalIdx = GetROItem(ROOMG, 1)

    WriteLog(string.format("[FORGE] Equipment idx=%d, Crystal idx=%d", nEquipIdx or -1, nCrystalIdx or -1))

    -- Validate items exist
    if not nEquipIdx or nEquipIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat trang bi!<color>")
        return
    end

    if not nCrystalIdx or nCrystalIdx <= 0 then
        Talk(1, "", "<color=red>Chua dat Huyen Tinh!<color>")
        return
    end

    -- Get equipment properties
    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

    -- Validate equipment is blue
    if nGenre ~= 0 then
        Talk(1, "", "<color=red>Chi duoc dat trang bi!<color>")
        return
    end

    if nLuck >= PURPLE_LUCK_FLAG then
        Talk(1, "", "<color=red>Trang bi da la tim!<color>")
        return
    end

    -- Validate NOT ring/amulet/pendant/horse/mask
    if nDetail == 3 or nDetail == 4 or nDetail == 8 or nDetail == 10 or nDetail == 11 then
        Talk(1, "", "<color=red>Khong duoc dat nhan, ngoc boi, day chuyen, ngua, mat na!<color>")
        return
    end

    -- Get crystal properties
    local nCrystalGenre, nCrystalDetail = GetItemProp(nCrystalIdx)

    -- Validate crystal
    if nCrystalGenre ~= HUYEN_TINH_GENRE then
        Talk(1, "", "<color=red>Chi duoc dat Huyen Tinh!<color>")
        return
    end

    if nCrystalDetail < HUYEN_TINH_MIN_DETAIL or nCrystalDetail > HUYEN_TINH_MAX_DETAIL then
        Talk(1, "", "<color=red>Huyen Tinh khong hop le!<color>")
        return
    end

    -- Calculate number of magic lines
    local nCrystalLevel = nCrystalDetail - 73  -- 74->1, 75->2, ..., 79->6
    local nNumLines = 0

    if nCrystalLevel >= 5 then
        nNumLines = 6  -- Level 5-6: guaranteed 6 lines
    else
        local nMinLines = (nCrystalLevel >= 3) and 3 or 2
        local nMaxLines = nCrystalLevel + 2
        nNumLines = random(nMinLines, nMaxLines)
    end

    WriteLog(string.format("[FORGE] Crystal level %d -> %d magic lines", nCrystalLevel, nNumLines))

    -- Delete source items
    DelItemByIndex(nEquipIdx)
    DelItemByIndex(nCrystalIdx)

    -- Create purple equipment
    local nPurpleLuck = PURPLE_LUCK_FLAG + random(1, 999)

    WriteLog(string.format("[FORGE] Creating purple: genre=%d, detail=%d, parti=%d, level=%d, series=%d, luck=%d, lines=%d",
        nGenre, nDetail, nParti, nLevel, nSeries, nPurpleLuck, nNumLines))

    -- Use AddItemEx - parameters: genre, detail, particular, level, series, luck, mag1-6, version, randseed, pos
    local bSuccess = AddItemEx(
        nGenre,      -- 0 = equipment
        nDetail,     -- weapon/armor type
        nParti,      -- particular
        nLevel,      -- level
        nSeries,     -- series
        nPurpleLuck, -- luck >= 1000000000 = purple
        nNumLines,   -- magic line 1 level
        nNumLines,   -- magic line 2 level
        nNumLines,   -- magic line 3 level
        nNumLines,   -- magic line 4 level
        nNumLines,   -- magic line 5 level
        nNumLines,   -- magic line 6 level
        1,           -- version
        0,           -- randseed
        0            -- pos = 0 (inventory)
    )

    WriteLog(string.format("[FORGE] AddItemEx returned: %s", tostring(bSuccess)))

    if not bSuccess or bSuccess == 0 then
        Talk(1, "", "<color=red>Loi tao trang bi!<color>")
        return
    end

    -- Success!
    Talk(1, "", string.format("<color=green>Tao thanh cong trang bi tim %d dong!<color>", nNumLines))
    Msg2SubWorld(string.format("<pic=135><color=green>%s<color> da tao <color=purple>TRANG BI TIM<color> %d dong!", GetName(), nNumLines))

    WriteLog("[FORGE] Success!")
end

function no()
end
