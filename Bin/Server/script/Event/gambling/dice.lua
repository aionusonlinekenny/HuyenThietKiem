
-- dice.lua — Xúc xắc (Lua 4/5.0 safe, không dùng số lớn 2147483648)
-- Phụ thuộc: GetCash, Pay, Earn, Msg2Player/Talk/Say, GetGlbMissionV/SetGlbMissionV


-- helper: length for array-like tables (Lua 4/5.0 safe)
function __tlen(t)
    local i = 0
    while t[i+1] ~= nil do
        i = i + 1
    end
    return i
end

-- Lua 4.0-safe RNG (không dùng 2147483648); luôn khả dụng cho script
__rng_seed = (__rng_seed or 13579)
function __rng_seed_init()
    if GetTime then
        __rng_seed = GetTime()
    elseif GetTickCount then
        __rng_seed = GetTickCount()
    else
        __rng_seed = __rng_seed + 7777
    end
end
__rng_seed_init()

-- LCG: seed = (a*seed + c) % m, với m = 16777216 (2^24)
function __rng_next()
    __rng_seed = (1103515245 * __rng_seed + 12345)
    -- modulo 2^24 bằng cách lấy phần dư theo 16777216 (an toàn Lua 4)
    __rng_seed = __rng_seed - math.floor(__rng_seed / 16777216) * 16777216
    return __rng_seed
end

function __rand_unit()
    return __rng_next() / 16777216
end

function __rand_int(a, b)
    if a == nil and b == nil then
        return __rand_unit()
    end
    if b == nil then
        -- 1..a
        local r = __rand_unit()
        return math.floor(r * a) + 1
    end
    -- a..b
    local r = __rand_unit()
    return math.floor(r * (b - a + 1)) + a
end


GAMB_POOL_ID = 8001
DICE_MIN_BET = 10 * 10000     -- 10 lượng -> đồng
DICE_MAX_BET = 200 * 10000    -- 200 lượng -> đồng

function __say(msg)
    if AddLocalNews then AddLocalNews(msg) end
    if Msg2Player then Msg2Player(msg) end
    if Talk then Talk(msg) end
end

function __get_pool()
    if GetGlbMissionV then return GetGlbMissionV(GAMB_POOL_ID) end
    if GetMissionV then return GetMissionV(GAMB_POOL_ID) end
    return 0
end

function __set_pool(v)
    if SetGlbMissionV then SetGlbMissionV(GAMB_POOL_ID, v) return end
    if SetMissionV then SetMissionV(GAMB_POOL_ID, v) return end
end

-- kind: 1 = Tai, 2 = Xiu, 3 = Doi
function dice_play(kind, money)
    if kind ~= 1 and kind ~= 2 and kind ~= 3 then
        __say("[Dice] Cửa không hợp lệ (1=Tài,2=Xỉu,3=Đôi)")
        return
    end
    if money == nil or money <= 0 then
        __say("[Dice] Mức cược không hợp lệ.")
        return
    end
    if money < DICE_MIN_BET or money > DICE_MAX_BET then
        __say("[Dice] Cược phải trong khoảng 10 ~ 200 lượng.")
        return
    end

    local cash = 0
    if GetCash then cash = GetCash() end
    if cash < money then
        __say("[Dice] Không đủ tiền cược.")
        return
    end

    if Pay then Pay(money) end

    local d1 = __rand_int(1,6)
    local d2 = __rand_int(1,6)
    local d3 = __rand_int(1,6)
    local sum = d1 + d2 + d3

    local twin = 0
    if d1 == d2 or d1 == d3 or d2 == d3 then twin = 1 end

    local win = 0
    if kind == 1 and (sum >= 11 and sum <= 17) and twin == 0 then win = 1 end
    if kind == 2 and (sum >= 4 and sum <= 10)  and twin == 0 and win == 0 then win = 2 end
    if kind == 3 and twin == 1 then win = 3 end

    local pool = __get_pool()
    local msg  = "[Dice] Kết quả: "..d1..","..d2..","..d3.." (Tổng "..sum..") "

    if win == 0 then
        pool = pool + money
        __set_pool(pool)
        __say(msg .. "Bạn thua!")
        return
    end

    local payout = money
    if win == 1 or win == 2 then payout = money * 2 end
    if win == 3 then payout = money * 12 end

    if pool >= payout then
        pool = pool - payout
        __set_pool(pool)
    end
    if Earn then Earn(payout) end

    local wname = (win==1 and "TÀI") or (win==2 and "XỈU") or "ĐÔI"
    __say(msg .. "Bạn thắng cửa "..wname.." nhận "..payout.." đồng!")
end
