
-- luckyhead.lua — Vòng quay rút gọn (Lua 4/5.0 safe, RNG 2^24)


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


LUCKY_REWARDS = {
    { kind="cash", amount=  50000, label="5 vạn đồng" },
    { kind="cash", amount= 100000, label="10 vạn đồng" },
    { kind="cash", amount= 300000, label="30 vạn đồng" },
    { kind="exp",  amount=  20000, label="20,000 EXP" },
    { kind="exp",  amount=  80000, label="80,000 EXP" },
    { kind="cash", amount= 500000, label="50 vạn đồng" },
}

function __sayL(msg)
    if AddLocalNews then AddLocalNews(msg) end
    if Msg2Player then Msg2Player(msg) end
    if Talk then Talk(msg) end
end

function Lucky_Open()
    __sayL("[Lucky] Quay thưởng: cash/exp đơn giản, không item.")
end

function Lucky_Spin()
    local n = __tlen(LUCKY_REWARDS)
    if n <= 0 then __sayL("[Lucky] Không có thưởng.") return end
    local i = __rand_int(1, n)
    local r = LUCKY_REWARDS[i]
    if r.kind == "cash" and Earn then
        Earn(r.amount)
        __sayL("[Lucky] Bạn nhận "..r.label.."!")
        return
    end
    if r.kind == "exp" and AddOwnExp then
        AddOwnExp(r.amount)
        __sayL("[Lucky] Bạn nhận "..r.label.."!")
        return
    end
    __sayL("[Lucky] Thưởng không áp dụng trên engine này.")
end
