-----------------------------------------------------------------
-- Purple Smith Configuration
-- Full crystal system and crafting recipes
-- Based on ThienDieuOnline
-----------------------------------------------------------------

-- ============================================================
-- HUYEN TINH (CRYSTAL) SYSTEM
-- ============================================================

-- Crystal types with detailed properties
CRYSTAL_TYPES = {
    -- Low tier crystals
    {
        name = "Lam Thuy Tinh Cap 1",
        detail = 16,
        particular = 1,
        luck = 1,
        color = "blue",
        description = "Huyen tinh cap thap nhat, +1 luck"
    },
    {
        name = "Lam Thuy Tinh Cap 2",
        detail = 16,
        particular = 2,
        luck = 2,
        color = "blue",
        description = "Huyen tinh cap thap, +2 luck"
    },
    {
        name = "Lam Thuy Tinh Cap 3",
        detail = 16,
        particular = 3,
        luck = 3,
        color = "blue",
        description = "Huyen tinh cap thap, +3 luck"
    },

    -- Medium tier crystals
    {
        name = "Tu Thuy Tinh Cap 1",
        detail = 17,
        particular = 1,
        luck = 3,
        color = "purple",
        description = "Huyen tinh cap trung, +3 luck"
    },
    {
        name = "Tu Thuy Tinh Cap 2",
        detail = 17,
        particular = 2,
        luck = 4,
        color = "purple",
        description = "Huyen tinh cap trung, +4 luck"
    },
    {
        name = "Tu Thuy Tinh Cap 3",
        detail = 17,
        particular = 3,
        luck = 5,
        color = "purple",
        description = "Huyen tinh cap trung, +5 luck"
    },
    {
        name = "Tu Thuy Tinh Cap 4",
        detail = 17,
        particular = 4,
        luck = 6,
        color = "purple",
        description = "Huyen tinh cap trung cao, +6 luck"
    },

    -- High tier crystals
    {
        name = "Luc Thuy Tinh Cap 1",
        detail = 18,
        particular = 1,
        luck = 5,
        color = "green",
        description = "Huyen tinh cap cao, +5 luck"
    },
    {
        name = "Luc Thuy Tinh Cap 2",
        detail = 18,
        particular = 2,
        luck = 6,
        color = "green",
        description = "Huyen tinh cap cao, +6 luck"
    },
    {
        name = "Luc Thuy Tinh Cap 3",
        detail = 18,
        particular = 3,
        luck = 7,
        color = "green",
        description = "Huyen tinh cap cao, +7 luck"
    },
    {
        name = "Luc Thuy Tinh Cap 4",
        detail = 18,
        particular = 4,
        luck = 8,
        color = "green",
        description = "Huyen tinh cap cao, +8 luck"
    },
    {
        name = "Luc Thuy Tinh Cap 5",
        detail = 18,
        particular = 5,
        luck = 9,
        color = "green",
        description = "Huyen tinh cap rat cao, +9 luck"
    },

    -- Legendary tier crystals
    {
        name = "Hong Thuy Tinh Cap 1",
        detail = 100,
        particular = 1,
        luck = 8,
        color = "red",
        description = "Huyen tinh huyen thoai, +8 luck"
    },
    {
        name = "Hong Thuy Tinh Cap 2",
        detail = 100,
        particular = 2,
        luck = 9,
        color = "red",
        description = "Huyen tinh huyen thoai, +9 luck"
    },
    {
        name = "Hong Thuy Tinh Cap 3",
        detail = 100,
        particular = 3,
        luck = 10,
        color = "red",
        description = "Huyen tinh huyen thoai, +10 luck"
    },
    {
        name = "Hong Thuy Tinh Cap 4",
        detail = 100,
        particular = 4,
        luck = 11,
        color = "red",
        description = "Huyen tinh huyen thoai, +11 luck"
    },
    {
        name = "Hong Thuy Tinh Cap 5",
        detail = 100,
        particular = 5,
        luck = 12,
        color = "red",
        description = "Huyen tinh huyen thoai toi thuong, +12 luck"
    },
}

-- Quick lookup table for luck values
CRYSTAL_LUCK_MAP = {}
for i, crystal in ipairs(CRYSTAL_TYPES) do
    local key = crystal.detail .. "_" .. crystal.particular
    CRYSTAL_LUCK_MAP[key] = crystal.luck
end

-- Function to get luck from crystal
function GetCrystalLuck(nDetail, nParticular)
    local key = nDetail .. "_" .. nParticular
    return CRYSTAL_LUCK_MAP[key] or 0
end

-- ============================================================
-- CRAFTING RECIPES
-- ============================================================

PURPLE_RECIPES = {
    -- Basic recipe (6 blue items)
    {
        name = "Hop thanh co ban",
        min_blues = 6,
        max_blues = 6,
        cost = 500000,      -- 50 van
        difficulty = 4,     -- Subtract from luck
        success_rate = 60,  -- 60%
        description = "Cong thuc co ban, 6 trang bi xanh"
    },

    -- Standard recipe (7 blue items)
    {
        name = "Hop thanh tieu chuan",
        min_blues = 7,
        max_blues = 7,
        cost = 800000,      -- 80 van
        difficulty = 3,     -- Subtract from luck
        success_rate = 70,  -- 70%
        description = "Cong thuc tieu chuan, 7 trang bi xanh"
    },

    -- Advanced recipe (8 blue items)
    {
        name = "Hop thanh nang cao",
        min_blues = 8,
        max_blues = 8,
        cost = 1000000,     -- 100 van
        difficulty = 3,     -- Subtract from luck
        success_rate = 75,  -- 75%
        description = "Cong thuc nang cao, 8 trang bi xanh"
    },

    -- Expert recipe (9 blue items)
    {
        name = "Hop thanh chuyen gia",
        min_blues = 9,
        max_blues = 9,
        cost = 1500000,     -- 150 van
        difficulty = 2,     -- Subtract from luck
        success_rate = 80,  -- 80%
        description = "Cong thuc chuyen gia, 9 trang bi xanh"
    },

    -- Master recipe (10 blue items)
    {
        name = "Hop thanh dai cao thu",
        min_blues = 10,
        max_blues = 10,
        cost = 2000000,     -- 200 van
        difficulty = 2,     -- Subtract from luck
        success_rate = 85,  -- 85%
        description = "Cong thuc cao nhat, 10 trang bi xanh"
    },
}

-- ============================================================
-- MAGIC LINES QUALITY SYSTEM
-- ============================================================

MAGIC_LINES_QUALITY = {
    [0] = {name = "Khong co thuoc tinh", color = "gray", rank = 0},
    [1] = {name = "Rat yeu", color = "white", rank = 1},
    [2] = {name = "Yeu", color = "white", rank = 1},
    [3] = {name = "Binh thuong", color = "white", rank = 2},
    [4] = {name = "Kha", color = "green", rank = 2},
    [5] = {name = "Tot", color = "green", rank = 3},
    [6] = {name = "Rat tot", color = "blue", rank = 3},
    [7] = {name = "Xuat sac", color = "blue", rank = 4},
    [8] = {name = "Tuyet voi", color = "purple", rank = 4},
    [9] = {name = "Huyen thoai", color = "purple", rank = 5},
    [10] = {name = "Than thoai", color = "orange", rank = 5},
}

-- ============================================================
-- HELPER FUNCTIONS
-- ============================================================

-- Get crystal info
function GetCrystalInfo(nDetail, nParticular)
    for i, crystal in ipairs(CRYSTAL_TYPES) do
        if crystal.detail == nDetail and crystal.particular == nParticular then
            return crystal
        end
    end
    return nil
end

-- Get recipe by blue count
function GetRecipeByBlueCount(nBlueCount)
    for i, recipe in ipairs(PURPLE_RECIPES) do
        if nBlueCount >= recipe.min_blues and nBlueCount <= recipe.max_blues then
            return recipe
        end
    end
    return PURPLE_RECIPES[3]  -- Default to advanced recipe
end

-- Calculate final luck
function CalculateFinalLuck(nTotalLuck, nDifficulty)
    local nFinalLuck = nTotalLuck - nDifficulty
    if nFinalLuck > 10 then
        nFinalLuck = 10
    end
    if nFinalLuck < 0 then
        nFinalLuck = 0
    end
    return nFinalLuck
end

-- Get quality description
function GetQualityDesc(nLines)
    local quality = MAGIC_LINES_QUALITY[nLines]
    if quality then
        return quality.name, quality.color, quality.rank
    end
    return "Khong xac dinh", "white", 0
end

-- ============================================================
-- DISPLAY FUNCTIONS
-- ============================================================

-- Generate crystal list display
function GetCrystalListDisplay()
    local str = "<color=yellow>=== DANH SACH HUYEN TINH ===<color>\n\n"

    str = str .. "<color=cyan>Cap Thap (Lam Thuy Tinh):<color>\n"
    for i = 1, 3 do
        local crystal = CRYSTAL_TYPES[i]
        str = str .. string.format("  Cap %d: +%d luck\n", i, crystal.luck)
    end

    str = str .. "\n<color=purple>Cap Trung (Tu Thuy Tinh):<color>\n"
    for i = 4, 7 do
        local crystal = CRYSTAL_TYPES[i]
        str = str .. string.format("  Cap %d: +%d luck\n", i-3, crystal.luck)
    end

    str = str .. "\n<color=green>Cap Cao (Luc Thuy Tinh):<color>\n"
    for i = 8, 12 do
        local crystal = CRYSTAL_TYPES[i]
        str = str .. string.format("  Cap %d: +%d luck\n", i-7, crystal.luck)
    end

    str = str .. "\n<color=red>Cap Huyen Thoai (Hong Thuy Tinh):<color>\n"
    for i = 13, 17 do
        local crystal = CRYSTAL_TYPES[i]
        str = str .. string.format("  Cap %d: +%d luck\n", i-12, crystal.luck)
    end

    return str
end

-- Generate recipe list display
function GetRecipeListDisplay()
    local str = "<color=yellow>=== CONG THUC HOP THANH ===<color>\n\n"

    for i, recipe in ipairs(PURPLE_RECIPES) do
        str = str .. string.format("<color=orange>%s:<color>\n", recipe.name)
        str = str .. string.format("  - Trang bi xanh: %d-%d\n", recipe.min_blues, recipe.max_blues)
        str = str .. string.format("  - Chi phi: %d van luong\n", recipe.cost/10000)
        str = str .. string.format("  - Do kho: -%d luck\n", recipe.difficulty)
        str = str .. string.format("  - Ti le: %d%%\n", recipe.success_rate)
        str = str .. "\n"
    end

    return str
end

-- Generate quality chart
function GetQualityChartDisplay()
    local str = "<color=yellow>=== BANG XEP HANG CHAT LUONG ===<color>\n\n"

    for i = 0, 10 do
        local quality = MAGIC_LINES_QUALITY[i]
        local colorTag = "<color=" .. quality.color .. ">"
        str = str .. string.format("%s%d dong: %s (Rank %d)<color>\n",
                                    colorTag, i, quality.name, quality.rank)
    end

    return str
end

return {
    CRYSTAL_TYPES = CRYSTAL_TYPES,
    CRYSTAL_LUCK_MAP = CRYSTAL_LUCK_MAP,
    PURPLE_RECIPES = PURPLE_RECIPES,
    MAGIC_LINES_QUALITY = MAGIC_LINES_QUALITY,
    GetCrystalLuck = GetCrystalLuck,
    GetCrystalInfo = GetCrystalInfo,
    GetRecipeByBlueCount = GetRecipeByBlueCount,
    CalculateFinalLuck = CalculateFinalLuck,
    GetQualityDesc = GetQualityDesc,
    GetCrystalListDisplay = GetCrystalListDisplay,
    GetRecipeListDisplay = GetRecipeListDisplay,
    GetQualityChartDisplay = GetQualityChartDisplay,
}
