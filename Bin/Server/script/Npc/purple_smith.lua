-----------------------------------------------------------------
-- NPC: Purple Smith (Tho Ren Huyen Bi)
-- Function: Creates purple/violet equipment from blue equipment + crystals
-- Based on: ThienDieuOnline builditem.lua system
-- Date: 2025-12-26
-----------------------------------------------------------------

Include("\\script\\lib\\TaskLib.lua")

-- ------------------------------------------------------------
-- Configuration - Purple Item Creation
-- ------------------------------------------------------------

-- Crystal types (Huyen Tinh - khoang thach)
CRYSTAL_GENRE = 6       -- item_task
CRYSTAL_DETAIL_LOW = 16  -- Lam Thuy Tinh (Blue Crystal) - Low luck
CRYSTAL_DETAIL_MED = 17  -- Tu Thuy Tinh (Purple Crystal) - Medium luck
CRYSTAL_DETAIL_HIGH = 18 -- Luc Thuy Tinh (Green Crystal) - High luck

-- Luck values per crystal type
CRYSTAL_LUCK = {
    [16] = 2,  -- Lam Thuy Tinh = +2 luck
    [17] = 5,  -- Tu Thuy Tinh = +5 luck
    [18] = 8,  -- Luc Thuy Tinh = +8 luck
}

-- Purple creation settings
PURPLE_MIN_BLUE_ITEMS = 6   -- Minimum blue items needed
PURPLE_MAX_BLUE_ITEMS = 10  -- Maximum blue items allowed
PURPLE_BASE_COST = 1000000  -- 100 van luong
PURPLE_DIFFICULTY = 3       -- Difficulty modifier (subtracted from luck)
PURPLE_MAX_LINES = 10       -- Maximum magic attribute lines
PURPLE_SUCCESS_RATE = 70    -- Base success rate (%)

-- Build container position (same as Tremble system)
BUILD_POS = 12  -- pos_builditem

-- ------------------------------------------------------------
-- NPC Talk Entry Point
-- ------------------------------------------------------------
function main(NpcIndex)
    PurpleSmithTalk()
end

function PurpleSmithTalk()
    local tbSay = {
        "Hop thanh trang bi TIM (Purple)/ShowPurpleGuide",
        "Bat dau hop thanh/OpenPurpleUI",
        "Ta chi ghe ngang qua/no",
    }
    Say("Tho Ren Huyen Bi: Ta co the hop thanh trang bi TIM tu trang bi xanh va Huyen Tinh!",
        getn(tbSay), tbSay)
end

-- ------------------------------------------------------------
-- Show Guide
-- ------------------------------------------------------------
function ShowPurpleGuide()
    local szGuide = "<color=yellow>=== HUONG DAN HOP THANH TRANG BI TIM ===<color>\n\n" ..
        "<color=cyan>Nguyen lieu can thiet:<color>\n" ..
        "1. <color=blue>" .. PURPLE_MIN_BLUE_ITEMS .. "-" .. PURPLE_MAX_BLUE_ITEMS .. " trang bi xanh<color> (cung loai)\n" ..
        "2. <color=orange>Huyen Tinh<color> (cang nhieu cang tot)\n" ..
        "3. <color=yellow>" .. (PURPLE_BASE_COST/10000) .. " van luong<color>\n\n" ..
        "<color=cyan>Co che hoat dong:<color>\n" ..
        "- Huyen Tinh level cao -> So dong thuoc tinh nhieu hon\n" ..
        "- Lam Thuy Tinh (xanh): +2 luck\n" ..
        "- Tu Thuy Tinh (tim): +5 luck\n" ..
        "- Luc Thuy Tinh (xanh la): +8 luck\n\n" ..
        "<color=cyan>Cong thuc:<color>\n" ..
        "- Luck = (Tong luck tu Huyen Tinh) - " .. PURPLE_DIFFICULTY .. "\n" ..
        "- So dong = random(Luck, " .. PURPLE_MAX_LINES .. ")\n" ..
        "- Ti le thanh cong: <color=green>" .. PURPLE_SUCCESS_RATE .. "%%<color>\n\n" ..
        "<color=red>LUU Y:<color>\n" ..
        "- That bai se mat 50%% nguyen lieu\n" ..
        "- Trang bi TIM khong the nang cap them"

    Talk(1, "", szGuide)
end

-- ------------------------------------------------------------
-- Open Purple Creation UI
-- ------------------------------------------------------------
function OpenPurpleUI()
    OpenGiveBox("Hop thanh trang bi TIM",
                "Dat trang bi xanh vao khung, them Huyen Tinh de tang so dong!",
                "ExecuteCreatePurple")
end

-- ------------------------------------------------------------
-- Execute Purple Item Creation
-- ------------------------------------------------------------
function ExecuteCreatePurple()
    Msg2Player("========================================")
    Msg2Player("=== ExecuteCreatePurple START ===")
    Msg2Player("========================================")

    local ROOMG = 8  -- GiveBox room ID

    -- Count blue equipment in GiveBox
    local nBlueCount = 0
    local nFirstBlueIdx = 0
    local nBlueGenre, nBlueDetail, nBlueParti, nBlueLevel, nBlueSeries = 0, 0, 0, 0, 0

    for i = 0, 5 do
        for j = 0, 3 do
            local nItemIdx = GetROItem(ROOMG, i, j)
            if nItemIdx > 0 then
                local nG, nD, nP, nL, nS, nLuck = GetItemProp(nItemIdx)

                -- Check if blue equipment (genre=0, luck < 1000000000)
                if nG == 0 and nLuck < 1000000000 then
                    if nBlueCount == 0 then
                        -- Save first blue item properties
                        nFirstBlueIdx = nItemIdx
                        nBlueGenre = nG
                        nBlueDetail = nD
                        nBlueParti = nP
                        nBlueLevel = nL
                        nBlueSeries = nS
                    else
                        -- Check if same type as first blue
                        if nD ~= nBlueDetail or nP ~= nBlueParti then
                            Talk(1, "", "<color=red>Tat ca trang bi xanh phai cung loai (detail + particular)!<color>")
                            return
                        end
                    end
                    nBlueCount = nBlueCount + 1
                end
            end
        end
    end

    Msg2Player("Found " .. nBlueCount .. " blue items")

    -- Validate blue item count
    if nBlueCount < PURPLE_MIN_BLUE_ITEMS then
        Talk(1, "", "<color=red>Can it nhat " .. PURPLE_MIN_BLUE_ITEMS .. " trang bi xanh!<color>")
        return
    end

    if nBlueCount > PURPLE_MAX_BLUE_ITEMS then
        Talk(1, "", "<color=red>Toi da " .. PURPLE_MAX_BLUE_ITEMS .. " trang bi xanh!<color>")
        return
    end

    -- Calculate luck from crystals in BUILD_POS container
    local nTotalLuck = 0
    local nCrystalCount = 0

    for i = 1, 7 do  -- Slots 1-7 for crystals
        local nCrystalIdx = GetPOItem(BUILD_POS, i)
        if nCrystalIdx > 0 then
            local nG, nD = GetItemProp(nCrystalIdx)
            if nG == CRYSTAL_GENRE and CRYSTAL_LUCK[nD] then
                nTotalLuck = nTotalLuck + CRYSTAL_LUCK[nD]
                nCrystalCount = nCrystalCount + 1
                Msg2Player("Crystal slot " .. i .. ": detail=" .. nD .. " luck=+" .. CRYSTAL_LUCK[nD])
            end
        end
    end

    Msg2Player("Total crystals: " .. nCrystalCount .. ", Total luck: " .. nTotalLuck)

    if nCrystalCount == 0 then
        Talk(1, "", "<color=red>Can it nhat 1 Huyen Tinh!<color>")
        return
    end

    -- Check money
    if GetCash() < PURPLE_BASE_COST then
        Talk(1, "", "<color=red>Khong du " .. (PURPLE_BASE_COST/10000) .. " van luong!<color>")
        return
    end

    -- Calculate final luck value (like ThienDieuOnline)
    local nFinalLuck = nTotalLuck - PURPLE_DIFFICULTY

    -- Clamp luck between 0 and PURPLE_MAX_LINES
    if nFinalLuck > PURPLE_MAX_LINES then
        nFinalLuck = PURPLE_MAX_LINES
    end
    if nFinalLuck < 0 then
        nFinalLuck = 0
    end

    Msg2Player("Final luck after difficulty: " .. nFinalLuck)

    -- Random number of magic lines (like ThienDieuOnline: random(nLuck, 10))
    local nMagicLines = random(nFinalLuck, PURPLE_MAX_LINES)

    Msg2Player("Magic lines will be: " .. nMagicLines)

    -- Success rate check
    local nRandom = random(1, 100)
    local bSuccess = (nRandom <= PURPLE_SUCCESS_RATE)

    Msg2Player("Success roll: " .. nRandom .. " <= " .. PURPLE_SUCCESS_RATE .. " = " .. tostring(bSuccess))

    if not bSuccess then
        -- Failed - consume 50% materials
        local nLoseCount = math.floor(nBlueCount / 2)
        local nLoseCrystals = math.floor(nCrystalCount / 2)

        Msg2Player("FAILED! Losing " .. nLoseCount .. " blues and " .. nLoseCrystals .. " crystals")

        -- Remove half blue items
        local nRemoved = 0
        for i = 0, 5 do
            for j = 0, 3 do
                if nRemoved >= nLoseCount then break end
                local nItemIdx = GetROItem(ROOMG, i, j)
                if nItemIdx > 0 then
                    local nG, nD, nP, nL, nS, nLuck = GetItemProp(nItemIdx)
                    if nG == 0 and nLuck < 1000000000 then
                        DelItemByIndex(nItemIdx)
                        nRemoved = nRemoved + 1
                    end
                end
            end
            if nRemoved >= nLoseCount then break end
        end

        -- Remove half crystals
        nRemoved = 0
        for i = 1, 7 do
            if nRemoved >= nLoseCrystals then break end
            local nCrystalIdx = GetPOItem(BUILD_POS, i)
            if nCrystalIdx > 0 then
                DelItemByIndex(nCrystalIdx)
                nRemoved = nRemoved + 1
            end
        end

        Pay(PURPLE_BASE_COST / 5)  -- Lose 20% money

        Talk(1, "", "<color=red>Hop thanh THAT BAI! Mat 50%% nguyen lieu!<color>")
        return
    end

    -- SUCCESS - Remove all materials
    Msg2Player("SUCCESS! Removing all materials...")

    -- Remove all blue items
    for i = 0, 5 do
        for j = 0, 3 do
            local nItemIdx = GetROItem(ROOMG, i, j)
            if nItemIdx > 0 then
                local nG, nD, nP, nL, nS, nLuck = GetItemProp(nItemIdx)
                if nG == 0 and nLuck < 1000000000 then
                    DelItemByIndex(nItemIdx)
                end
            end
        end
    end

    -- Remove all crystals
    for i = 1, 7 do
        local nCrystalIdx = GetPOItem(BUILD_POS, i)
        if nCrystalIdx > 0 then
            DelItemByIndex(nCrystalIdx)
        end
    end

    -- Pay money
    Pay(PURPLE_BASE_COST)

    -- CREATE PURPLE ITEM!
    Msg2Player("Creating PURPLE item with " .. nMagicLines .. " magic lines...")

    -- Use AddItemEx to create purple item
    -- Luck >= 1000000000 = Purple item in HuyenThietKiem
    local nPurpleLuck = 1000000000

    -- Add to BUILD_POS slot 0
    AddItemEx(nBlueGenre, nBlueDetail, nBlueParti, nBlueLevel, nBlueSeries,
              nPurpleLuck,      -- Luck = 1 billion = PURPLE
              nMagicLines,      -- Level for magic line 1
              nMagicLines,      -- Level for magic line 2
              nMagicLines,      -- Level for magic line 3
              nMagicLines,      -- Level for magic line 4
              nMagicLines,      -- Level for magic line 5
              nMagicLines,      -- Level for magic line 6
              1,                -- Bind
              0,                -- Expire time
              BUILD_POS)        -- Position

    -- Success message
    local szSuccess = "<color=green>HOP THANH THANH CONG!<color>\n" ..
                      "Trang bi TIM voi <color=yellow>" .. nMagicLines .. " dong<color> thuoc tinh!\n" ..
                      "Luck: <color=orange>" .. nPurpleLuck .. "<color>"

    Msg2Player(szSuccess)
    Msg2SubWorld("<pic=135><color=green> " .. GetName() .. "<color> da hop thanh thanh cong <color=purple>TRANG BI TIM<color> voi " .. nMagicLines .. " dong!")

    Msg2Player("========================================")
    Msg2Player("=== ExecuteCreatePurple END ===")
    Msg2Player("========================================")
end

-- ------------------------------------------------------------
-- Helper Functions
-- ------------------------------------------------------------
function no()
end
