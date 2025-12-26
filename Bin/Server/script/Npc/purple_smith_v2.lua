-----------------------------------------------------------------
-- NPC: Purple Smith V2 (Tho Ren Huyen Bi)
-- Function: Creates purple/violet equipment with full UI system
-- Based on: ThienDieuOnline builditem.lua + config system
-- Date: 2025-12-26
-----------------------------------------------------------------

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\npc\\purple_smith_config.lua")

-- Import config as GLOBAL variable (not local)
Config = dofile("script/npc/purple_smith_config.lua")

-- ------------------------------------------------------------
-- Configuration Override (can modify here)
-- ------------------------------------------------------------
CRYSTAL_GENRE = 6       -- item_task
BUILD_POS = 12          -- pos_builditem
GIVE_BOX_ROOM = 8       -- GiveBox room ID
PURPLE_MAX_LINES = 10   -- Maximum magic attribute lines

-- ------------------------------------------------------------
-- NPC Main Menu
-- ------------------------------------------------------------
function main(NpcIndex)
    PurpleSmithMainMenu()
end

function PurpleSmithMainMenu()
    local tbSay = {
        "Hop thanh trang bi TIM/SelectRecipe",
        "Xem danh sach Huyen Tinh/ShowCrystalList",
        "Xem cong thuc hop thanh/ShowRecipeList",
        "Bang xep hang chat luong/ShowQualityChart",
        "Huong dan chi tiet/ShowDetailedGuide",
        "Ta chi ghe ngang qua/no",
    }
    Say("Tho Ren Huyen Bi: Chao mung den voi tho ren huyen bi! Ta co the giup nguoi tao ra trang bi TIM tuyet dep!",
        getn(tbSay), tbSay)
end

-- ============================================================
-- MENU FUNCTIONS
-- ============================================================

-- Show crystal list
function ShowCrystalList()
    local str = Config.GetCrystalListDisplay()
    str = str .. "\n<color=yellow>Luu y:<color> Cang nhieu Huyen Tinh cap cao, item TIM cang co nhieu dong thuoc tinh!"
    Talk(1, "", str)
end

-- Show recipe list
function ShowRecipeList()
    local str = Config.GetRecipeListDisplay()
    str = str .. "\n<color=cyan>Meo:<color> Cong thuc cao hon = ti le thanh cong cao hon!"
    Talk(1, "", str)
end

-- Show quality chart
function ShowQualityChart()
    local str = Config.GetQualityChartDisplay()
    str = str .. "\n<color=cyan>Vi du:<color>\n"
    str = str .. "- 10 dong = <color=orange>Than thoai<color> (tot nhat!)\n"
    str = str .. "- 7-9 dong = <color=purple>Tuyet voi - Huyen thoai<color>\n"
    str = str .. "- 5-6 dong = <color=green>Tot - Rat tot<color>\n"
    str = str .. "- 3-4 dong = <color=white>Binh thuong - Kha<color>"
    Talk(1, "", str)
end

-- Show detailed guide
function ShowDetailedGuide()
    local szGuide = "<color=yellow>=== HUONG DAN CHI TIET ===<color>\n\n"

    szGuide = szGuide .. "<color=cyan>1. CHUAN BI NGUYEN LIEU:<color>\n"
    szGuide = szGuide .. "- 6-10 trang bi xanh (CUNG LOAI!)\n"
    szGuide = szGuide .. "- 1-7 Huyen Tinh (cang nhieu cap cao cang tot)\n"
    szGuide = szGuide .. "- 50-200 van luong (tuy cong thuc)\n\n"

    szGuide = szGuide .. "<color=cyan>2. CACH TINH SO DONG:<color>\n"
    szGuide = szGuide .. "Buoc 1: Tinh tong luck tu Huyen Tinh\n"
    szGuide = szGuide .. "Buoc 2: Tru di do kho cong thuc\n"
    szGuide = szGuide .. "Buoc 3: So dong = random(luck, 10)\n\n"

    szGuide = szGuide .. "<color=cyan>3. VI DU CU THE:<color>\n"
    szGuide = szGuide .. "A. Dung 2x Hong Thuy Tinh Cap 5 (+24 luck):\n"
    szGuide = szGuide .. "   Luck = 24 - 3 = 21 -> 10 (max)\n"
    szGuide = szGuide .. "   So dong = random(10,10) = <color=orange>10 DONG<color>\n\n"

    szGuide = szGuide .. "B. Dung 2x Luc Thuy Tinh Cap 3 (+14 luck):\n"
    szGuide = szGuide .. "   Luck = 14 - 3 = 11 -> 10\n"
    szGuide = szGuide .. "   So dong = random(10,10) = <color=orange>10 DONG<color>\n\n"

    szGuide = szGuide .. "C. Dung 2x Tu Thuy Tinh Cap 2 (+8 luck):\n"
    szGuide = szGuide .. "   Luck = 8 - 3 = 5\n"
    szGuide = szGuide .. "   So dong = random(5,10) = <color=green>5-10 DONG<color>\n\n"

    szGuide = szGuide .. "<color=red>LUU Y:<color>\n"
    szGuide = szGuide .. "- That bai se mat 50%% nguyen lieu!\n"
    szGuide = szGuide .. "- Trang bi TIM khong the nang cap!"

    Talk(1, "", szGuide)
end

-- ============================================================
-- RECIPE SELECTION
-- ============================================================

function SelectRecipe()
    local tbSay = {}
    local count = 1

    for i = 1, getn(Config.PURPLE_RECIPES) do
        local recipe = Config.PURPLE_RECIPES[i]
        tbSay[count] = recipe.name .. " (" .. recipe.min_blues .. " xanh, " .. (recipe.cost/10000) .. " van)/ConfirmRecipe#" .. i
        count = count + 1
    end

    tbSay[count] = "Quay lai/main"

    Say("Chon cong thuc hop thanh:", getn(tbSay), tbSay)
end

function ConfirmRecipe(sel, nRecipeIdx)
    local nIdx = tonumber(nRecipeIdx)
    local recipe = Config.PURPLE_RECIPES[nIdx]

    if not recipe then
        Talk(1, "", "Cong thuc khong ton tai!")
        return
    end

    -- Save recipe index to task
    SetTask(999, nIdx)  -- Temporary task to store recipe index

    local szConfirm = string.format("<color=yellow>%s<color>\n\n", recipe.name)
    szConfirm = szConfirm .. string.format("<color=cyan>Yeu cau:<color>\n")
    szConfirm = szConfirm .. string.format("- Trang bi xanh: %d-%d (cung loai)\n", recipe.min_blues, recipe.max_blues)
    szConfirm = szConfirm .. string.format("- Huyen Tinh: 1-7 vien\n")
    szConfirm = szConfirm .. string.format("- Chi phi: %d van luong\n\n", recipe.cost/10000)
    szConfirm = szConfirm .. string.format("<color=cyan>Thong so:<color>\n")
    szConfirm = szConfirm .. string.format("- Do kho: -%d luck\n", recipe.difficulty)
    szConfirm = szConfirm .. string.format("- Ti le thanh cong: <color=green>%d%%<color>\n\n", recipe.success_rate)
    szConfirm = szConfirm .. string.format("<color=yellow>%s<color>", recipe.description)

    Say(szConfirm, 2,
        "Bat dau hop thanh/OpenCraftingUI",
        "Chon cong thuc khac/SelectRecipe")
end

-- ============================================================
-- CRAFTING UI
-- ============================================================

function OpenCraftingUI()
    local nRecipeIdx = GetTask(999)
    local recipe = Config.PURPLE_RECIPES[nRecipeIdx]

    if not recipe then
        Talk(1, "", "Vui long chon cong thuc truoc!")
        SelectRecipe()
        return
    end

    local szGuide = string.format("Cong thuc: <color=yellow>%s<color>\n", recipe.name)
    szGuide = szGuide .. "Dat trang bi xanh vao khung lon\n"
    szGuide = szGuide .. "Dat Huyen Tinh vao khung nho (slot 1-7)"

    OpenGiveBox("Hop thanh trang bi TIM", szGuide, "ExecuteCreatePurple")
end

-- ============================================================
-- EXECUTE CRAFTING
-- ============================================================

function ExecuteCreatePurple()
    Msg2Player("========================================")
    Msg2Player("=== ExecuteCreatePurple V2 START ===")
    Msg2Player("========================================")

    local nRecipeIdx = GetTask(999)
    local recipe = Config.PURPLE_RECIPES[nRecipeIdx]

    if not recipe then
        Talk(1, "", "Loi: Khong tim thay cong thuc!")
        return
    end

    Msg2Player("Using recipe: " .. recipe.name)

    -- Count blue equipment in GiveBox
    local nBlueCount = 0
    local nFirstBlueIdx = 0
    local nBlueGenre, nBlueDetail, nBlueParti, nBlueLevel, nBlueSeries = 0, 0, 0, 0, 0

    for i = 0, 5 do
        for j = 0, 3 do
            local nItemIdx = GetROItem(GIVE_BOX_ROOM, i, j)
            if nItemIdx > 0 then
                local nG, nD, nP, nL, nS, nLuck = GetItemProp(nItemIdx)

                -- Check if blue equipment (genre=0, luck < 1000000000)
                if nG == 0 and nLuck < 1000000000 then
                    if nBlueCount == 0 then
                        nFirstBlueIdx = nItemIdx
                        nBlueGenre = nG
                        nBlueDetail = nD
                        nBlueParti = nP
                        nBlueLevel = nL
                        nBlueSeries = nS
                    else
                        -- Check if same type
                        if nD ~= nBlueDetail or nP ~= nBlueParti then
                            Talk(1, "", "<color=red>Tat ca trang bi xanh phai CUNG LOAI (detail + particular)!<color>")
                            return
                        end
                    end
                    nBlueCount = nBlueCount + 1
                end
            end
        end
    end

    Msg2Player("Found " .. nBlueCount .. " blue items")

    -- Validate blue count matches recipe
    if nBlueCount < recipe.min_blues or nBlueCount > recipe.max_blues then
        Talk(1, "", string.format("<color=red>Cong thuc nay can %d-%d trang bi xanh! (hien co: %d)<color>",
                                   recipe.min_blues, recipe.max_blues, nBlueCount))
        return
    end

    -- Calculate luck from crystals
    local nTotalLuck = 0
    local nCrystalCount = 0
    local crystalInfo = {}

    for i = 1, 7 do
        local nCrystalIdx = GetPOItem(BUILD_POS, i)
        if nCrystalIdx > 0 then
            local nG, nD, nP = GetItemProp(nCrystalIdx)
            if nG == CRYSTAL_GENRE then
                local luck = Config.GetCrystalLuck(nD, nP)
                if luck > 0 then
                    nTotalLuck = nTotalLuck + luck
                    nCrystalCount = nCrystalCount + 1

                    local info = Config.GetCrystalInfo(nD, nP)
                    table.insert(crystalInfo, info)

                    Msg2Player(string.format("Crystal slot %d: %s (+%d luck)", i, info.name, luck))
                end
            end
        end
    end

    Msg2Player("Total crystals: " .. nCrystalCount .. ", Total luck: " .. nTotalLuck)

    if nCrystalCount == 0 then
        Talk(1, "", "<color=red>Can it nhat 1 Huyen Tinh!<color>")
        return
    end

    -- Check money
    if GetCash() < recipe.cost then
        Talk(1, "", string.format("<color=red>Khong du %d van luong!<color>", recipe.cost/10000))
        return
    end

    -- Calculate final luck
    local nFinalLuck = Config.CalculateFinalLuck(nTotalLuck, recipe.difficulty)
    Msg2Player("Final luck after difficulty: " .. nFinalLuck)

    -- Random number of magic lines
    local nMagicLines = random(nFinalLuck, PURPLE_MAX_LINES)
    Msg2Player("Magic lines will be: " .. nMagicLines)

    -- Get quality description
    local qualityName, qualityColor, qualityRank = Config.GetQualityDesc(nMagicLines)

    -- Success rate check
    local nRandom = random(1, 100)
    local bSuccess = (nRandom <= recipe.success_rate)

    Msg2Player("Success roll: " .. nRandom .. " <= " .. recipe.success_rate .. " = " .. tostring(bSuccess))

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
                local nItemIdx = GetROItem(GIVE_BOX_ROOM, i, j)
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

        Pay(recipe.cost / 5)

        Talk(1, "", "<color=red>HOP THANH THAT BAI! Mat 50%% nguyen lieu!<color>")
        return
    end

    -- SUCCESS!
    Msg2Player("SUCCESS! Removing all materials...")

    -- Remove all blue items
    for i = 0, 5 do
        for j = 0, 3 do
            local nItemIdx = GetROItem(GIVE_BOX_ROOM, i, j)
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

    Pay(recipe.cost)

    -- CREATE PURPLE ITEM!
    Msg2Player("Creating PURPLE item with " .. nMagicLines .. " magic lines...")

    local nPurpleLuck = 1000000000

    AddItemEx(nBlueGenre, nBlueDetail, nBlueParti, nBlueLevel, nBlueSeries,
              nPurpleLuck,
              nMagicLines, nMagicLines, nMagicLines,
              nMagicLines, nMagicLines, nMagicLines,
              1, 0, BUILD_POS)

    -- Success message with quality
    local colorTag = "<color=" .. qualityColor .. ">"
    local szSuccess = "<color=green>HOP THANH THANH CONG!<color>\n\n"
    szSuccess = szSuccess .. "Trang bi TIM voi " .. colorTag .. nMagicLines .. " dong<color> thuoc tinh!\n"
    szSuccess = szSuccess .. "Chat luong: " .. colorTag .. qualityName .. "<color> (Rank " .. qualityRank .. ")\n\n"
    szSuccess = szSuccess .. "Huyen Tinh da su dung:\n"

    for i = 1, getn(crystalInfo) do
        local info = crystalInfo[i]
        local tag = "<color=" .. info.color .. ">"
        szSuccess = szSuccess .. tag .. "- " .. info.name .. " (+" .. info.luck .. " luck)<color>\n"
    end

    Msg2Player(szSuccess)

    local worldMsg = string.format("<pic=135><color=green>%s<color> da hop thanh thanh cong <color=purple>TRANG BI TIM<color> voi %s%d dong<color> (%s%s<color>)!",
                                     GetName(), colorTag, nMagicLines, colorTag, qualityName)
    Msg2SubWorld(worldMsg)

    Msg2Player("========================================")
    Msg2Player("=== ExecuteCreatePurple V2 END ===")
    Msg2Player("========================================")

    -- Clear recipe task
    SetTask(999, 0)
end

-- ============================================================
-- HELPER FUNCTIONS
-- ============================================================

function no()
end
