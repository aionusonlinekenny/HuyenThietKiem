-- =========================================================
--  HORSE RACE (server-side) - Lua 4.x/5.0 compatible
--  PHIÊN B?N Ð?NG NH?T: 1 c? m? cu?c (MV(1000)), idempotent
--  Không dùng _G, không local function, không vararg "...", không Input.
-- =========================================================

Include("\\script\\lib\\TaskLib.lua");

-- ===== Config =====
if RACE_MISSION == nil then RACE_MISSION = 16 end
if TM_OPEN == nil then TM_OPEN = 17 end       -- v?n dùng timer d? auto start
if TM_MOVE == nil then TM_MOVE = 18 end
if TM_PACE == nil then TM_PACE = 19 end
if TM_CLAIM == nil then TM_CLAIM = 20 end

if MAP_ID == nil then MAP_ID = 176 end
if TICKS_PER_SEC == nil then TICKS_PER_SEC = 18 end

-- ===== Fallback helpers =====
if floor == nil and math and math.floor then
    function floor(x) return math.floor(x) end
end
if min == nil then function min(a,b) if a<b then return a else return b end end end
if max == nil then function max(a,b) if a>b then return a else return b end end end

function pix2tile(px) return floor(px / 32) end
function clamp(a, lo, hi) if a<lo then return lo elseif a>hi then return hi else return a end end
function RNG(a, b)
    if RANDOM ~= nil then return RANDOM(a, b) end
    if Random ~= nil then return Random(a, b) end
    if random ~= nil then return random(a, b) end
    return a
end
function msg(s) if Msg2SubWorld ~= nil then Msg2SubWorld(s) end end

-- ===== Markers & lanes (pixel) =====
if NPCPOS_DUANGUA == nil then
    NPCPOS_DUANGUA = {
      [MAP_ID] = {
        {49384, 95921, "[DuaNgua]- Dich"},
        {42537,103039, "[DuaNgua]- XuatPhat"},
      }
    }
end
if HORSETAB == nil then
    HORSETAB = {
      {727,  42125,102452,"1. Chu Tuoc",   48836, 95716},
      {1644, 42177,102508,"2. Huyen Vu",   48904, 95786},
      {1502, 42232,102564,"3. Bach Ho",    48949, 95828},
      {1141, 42305,102634,"4. Thanh Long", 49009, 95884},
      {728,  42380,102696,"5. Ky Lan",     49077, 95938},
      {1397, 42436,102764,"6. Thien Dieu", 49147, 96008},
    }
end

-- ===== MissionV s? d?ng =====
-- MV(0)       : 0=idle/claim ; 1=racing
-- MV(1..6)    : NPC index 6 lane
-- MV(7..12)   : th? h?ng v? dích (luu lane 1..6)
-- MV(20+i)    : ti?n d? lane i
-- MV(80+i)    : t?c d? lane i
-- MV(1000)    : 1 = c?a cu?c dang m? ; 0 = dóng
-- MV(3000)    : markers/horses dã spawn 1 l?n chua (idempotent)

-- ===== Ch?ng dánh nhau (PACIFY) =====
-- ===== Ch?ng dánh nhau (PACIFY) – phiên b?n “c?ng tay” =====
-- Luu ý: n?u camp trung l?p c?a server b?n KHÔNG ph?i 7, hãy ch?nh CAMP_CANDIDATES bên du?i.
function PACIFY(npcId)
    if npcId == nil or npcId <= 0 then return end

    -- 0) d?ng m?i hành d?ng hi?n t?i n?u có API
    if StopNpcAction ~= nil then StopNpcAction(npcId) end
    if ClearNpcAction ~= nil then ClearNpcAction(npcId) end
    if ResetNpcAI ~= nil then ResetNpcAI(npcId) end

    -- 1) t?t AI/dánh nhau/PK/target (th? nhi?u tên hàm có th? có trong engine cu)
    if SetFightState       ~= nil then SetFightState(npcId, 0) end
    if SetNpcFightState    ~= nil then SetNpcFightState(npcId, 0) end
    if SetNpcAI            ~= nil then SetNpcAI(npcId, 0) end
    if SetNpcAIMode        ~= nil then SetNpcAIMode(npcId, 0) end
    if SetNpcActive        ~= nil then SetNpcActive(npcId, 0) end
    if SetNpcGroup         ~= nil then SetNpcGroup(npcId, 0) end
    if SetNpcPKMode        ~= nil then SetNpcPKMode(npcId, 0) end
    if SetNpcPKState       ~= nil then SetNpcPKState(npcId, 0) end
    if SetNpcPKValue       ~= nil then SetNpcPKValue(npcId, 0) end
    if SetNpcTarget        ~= nil then SetNpcTarget(npcId, 0) end
    if SetNpcEnemy         ~= nil then SetNpcEnemy(npcId, 0) end
    if SetNpcEnemyID       ~= nil then SetNpcEnemyID(npcId, 0) end

    -- 2) không cho tham gia combat/không b? target/không va ch?m/không selection (tùy API có)
    if SetNpcCanAttack         ~= nil then SetNpcCanAttack(npcId, 0) end
    if SetNpcCanBeAttacked     ~= nil then SetNpcCanBeAttacked(npcId, 0) end
    if SetNpcAttackable        ~= nil then SetNpcAttackable(npcId, 0) end
    if SetNpcBeAttackable      ~= nil then SetNpcBeAttackable(npcId, 0) end
    if SetNpcSelectable        ~= nil then SetNpcSelectable(npcId, 0) end
    if SetNpcSelectAble        ~= nil then SetNpcSelectAble(npcId, 0) end
    if SetNpcCollide           ~= nil then SetNpcCollide(npcId, 0) end
    if SetNpcCollision         ~= nil then SetNpcCollision(npcId, 0) end
    if SetNpcBodyType          ~= nil then SetNpcBodyType(npcId, 3) end  -- 3 = phi combat (tùy engine)
    if SetNpcGhost             ~= nil then SetNpcGhost(npcId, 1) end     -- “ghost” tránh va ch?m
    if SetNpcVisibleAttack     ~= nil then SetNpcVisibleAttack(npcId, 0) end

    -- 3) ép camp trung l?p: th? l?n lu?t các ?ng viên (d?i theo server b?n n?u bi?t chính xác)
    local CAMP_CANDIDATES = {7, 6, 0}
    local i
    for i = 1, getn(CAMP_CANDIDATES) do
        local c = CAMP_CANDIDATES[i]
        if SetNpcCamp     ~= nil then SetNpcCamp(npcId, c) end
        if SetNpcCurCamp  ~= nil then SetNpcCurCamp(npcId, c) end
        if ChangeNpcCamp  ~= nil then ChangeNpcCamp(npcId, c) end
    end

    -- 4) clear thù h?n/enmity n?u có
    if ClearNpcHatred       ~= nil then ClearNpcHatred(npcId) end
    if ClearNpcEnmity       ~= nil then ClearNpcEnmity(npcId) end
    if ResetNpcEnmity       ~= nil then ResetNpcEnmity(npcId) end
    if ClearNpcAttackList   ~= nil then ClearNpcAttackList(npcId) end

    -- 5) HP dày d? l? dính hit l? cung không ch?t
    if SetNpcMaxLife ~= nil then SetNpcMaxLife(npcId, 99999999) end
    if SetNpcLife    ~= nil then SetNpcLife(npcId, 99999999) end
end


-- ===== Spawn markers + horses (idempotent) =====
function add_duangua()
    PlayerIndex = 0
    local sw = SubWorldID2Idx(MAP_ID)
    if sw < 0 then return end
    if IsMission(RACE_MISSION) == 0 then OpenMission(RACE_MISSION) end

    local i
    for i = 1, getn(NPCPOS_DUANGUA[MAP_ID]) do
        local x = NPCPOS_DUANGUA[MAP_ID][i][1]
        local y = NPCPOS_DUANGUA[MAP_ID][i][2]
        local name = NPCPOS_DUANGUA[MAP_ID][i][3]
        local idx = AddNpc(229, 1, sw, x, y, 7, name, 0)
        if strfind(name, "XuatPhat") then
            SetNpcScript(idx, "\\script\\event\\gambling\\quanngua.lua")
            if SetNpcParam ~= nil then SetNpcParam(idx, 1, 91001) end
        else
            SetNpcScript(idx, "")
        end
        PACIFY(idx)
    end

    for i = 1, getn(HORSETAB) do
        local tId = HORSETAB[i][1]
        local sx = HORSETAB[i][2]
        local sy = HORSETAB[i][3]
        local sName = HORSETAB[i][4]
        local npcId = AddNpc(tId, 1, sw, sx, sy, 7, sName, 0)
        SetMissionV(i, npcId)
        SetPos(pix2tile(sx), pix2tile(sy), npcId)
        PACIFY(npcId)
    end
end

-- ===== API m? c?a cu?c (IDEMPOTENT) =====
function OpenBettingNow()
    if IsMission(RACE_MISSION) == 0 then OpenMission(RACE_MISSION) end

    -- n?u dang dua -> không m?
    if GetMissionV(0) ~= 0 then
        SetMissionV(1000, 0)
        return
    end

    -- reset th? h?ng + ti?n d?
    local i
    for i = 7, 12 do SetMissionV(i, 0) end
    for i = 1, getn(HORSETAB) do
        SetMissionV(20 + i, 0)
        -- dua ng?a v? v? trí xu?t phát & pacify l?i
        local id = GetMissionV(i)
        if id > 0 then
            local tx = pix2tile(HORSETAB[i][2])
            local ty = pix2tile(HORSETAB[i][3])
            SetPos(tx, ty, id)
            PACIFY(id)
        end
    end

    -- c? c?a m?
    SetMissionV(1000, 1)
    SetMissionV(0, 0)

    -- timer: sau 60s t? vào dua
    StartMissionTimer(RACE_MISSION, TM_OPEN, 1080)
    -- pace d? lâu lâu c?p nh?t t?c d?
    StartMissionTimer(RACE_MISSION, TM_PACE, 2 * TICKS_PER_SEC)

    msg("<bclr=red><color=yellow>[Dua thu]<color><bclr> Cua cuoc da mo! Den moc Xuat Phat de dat cuoc.")
end

-- ===== B?T Ð?U ÐUA =====
function duangua_batdau()
    -- dóng c?a
    SetMissionV(1000, 0)
    SetMissionV(0, 1) -- racing

    local i
    for i = 1, getn(HORSETAB) do
        local id = GetMissionV(i)
        if id > 0 then
            local tx = pix2tile(HORSETAB[i][2])
            local ty = pix2tile(HORSETAB[i][3])
            SetPos(tx, ty, id)
            SetMissionV(20 + i, 0)
            local sp = RNG(7, 10)
            SetMissionV(80 + i, sp)
            if SetNpcSpeed ~= nil then SetNpcSpeed(id, sp) end
            PACIFY(id) -- nh?c l?i ngay tru?c khi ch?y
        end
    end

    StopMissionTimer(RACE_MISSION, TM_OPEN)
    StartMissionTimer(RACE_MISSION, TM_MOVE, TICKS_PER_SEC)

    msg("<color=yellow>Truong dua Lam An: Bat dau!")
end

-- ===== Ð?I NH?P T?C Ð? (dang dua) =====
function change_duangua()
    if GetMissionV(0) ~= 1 then return end
    local i
    for i = 1, getn(HORSETAB) do
        local sp = RNG(7, 10)
        SetMissionV(80 + i, sp)
        local id = GetMissionV(i)
        if id > 0 and SetNpcSpeed ~= nil then SetNpcSpeed(id, sp) end
        PACIFY(id) -- luôn d?m b?o “hi?n” (engine có th? b?t l?i AI)
    end
end

-- ===== TICK DI CHUY?N =====
function race_tick()
    if GetMissionV(0) ~= 1 then return end

    local all_done = 1
    local i
    for i = 1, getn(HORSETAB) do
        local id = GetMissionV(i)
        if id > 0 then
            PACIFY(id) -- tái áp d?t m?i tick
            local sx, sy = HORSETAB[i][2], HORSETAB[i][3]
            local fx, fy = HORSETAB[i][5], HORSETAB[i][6]
            local tx, ty = pix2tile(sx), pix2tile(sy)
            local ftx, fty = pix2tile(fx), pix2tile(fy)

            local prog = GetMissionV(20 + i)
            if prog == 0 then prog = 0 end

            local dir = 1
            if fty < ty then dir = -1 end

            local not_finished = 0
            if dir > 0 and (ty + prog) < fty then not_finished = 1 end
            if dir < 0 and (ty + prog) > fty then not_finished = 1 end

            if not_finished == 1 then
                local step = GetMissionV(80 + i)
                if step <= 0 then step = 8 end
                prog = prog + dir * step
                local cur_ty = clamp(ty + prog, min(ty, fty), max(ty, fty))
                SetPos(tx, cur_ty, id)
                SetMissionV(20 + i, prog)
                all_done = 0

                local crossed = 0
                if dir > 0 and cur_ty >= fty then crossed = 1 end
                if dir < 0 and cur_ty <= fty then crossed = 1 end
                if crossed == 1 then
                    local slot
                    for slot = 7, 12 do
                        if GetMissionV(slot) == 0 then
                            SetMissionV(slot, i)
                            break
                        end
                    end
                end
            end
        end
    end

    if all_done == 1 then
        StopMissionTimer(RACE_MISSION, TM_MOVE)
        StopMissionTimer(RACE_MISSION, TM_PACE)
        SetMissionV(0, 0)
        -- 7 phút cho ngu?i choi nh?n thu?ng
        StartMissionTimer(RACE_MISSION, TM_CLAIM, 7 * 1080)

        local first = GetMissionV(7)
        local second = GetMissionV(8)
        if first <= 0 then first = -1 end
        if second <= 0 then second = -1 end
        msg("<color=yellow>Ket qua: Ve nhat = Linh thu so " .. first .. ", Ve nhi = Linh thu so " .. second .. ". Den NPC de nhan thuong!")
    end
end

-- ===== D?N NPC theo tên (n?u mu?n) =====
function del_duangua()
    local i
    for i = 1, getn(NPCPOS_DUANGUA[MAP_ID]) do
        ClearMapNpcWithName(MAP_ID, NPCPOS_DUANGUA[MAP_ID][i][3])
    end
end

-- ===== Boot hook (g?i t? realtimer.lua) =====
if SYS_SWITCH_DUA_NGUA == nil then SYS_SWITCH_DUA_NGUA = 1 end

function CheckAndRunDuaNgua()
    if SYS_SWITCH_DUA_NGUA ~= 1 then return end
    if IsMission(RACE_MISSION) == 0 then OpenMission(RACE_MISSION) end
    if GetMissionV(3000) == 0 then
        add_duangua()
        SetMissionV(3000, 1)
    end
    -- Không t? m? cu?a ? dây n?a; d? ngu?i choi b?m vào NPC là m?,
    -- nhung n?u b?n v?n mu?n auto m?, g?i OpenBettingNow() t?i dây.
end

-- ===== TimerTask callbacks =====
function SubTimer16_17()  -- H?t c?a cu?c -> b?t d?u dua
    duangua_batdau()
end
function SubTimer16_18()  -- Tick ch?y
    race_tick()
end
function SubTimer16_19()  -- Ð?i nh?p
    change_duangua()
end
function SubTimer16_20()  -- H?t th?i gian claim (noop)
end
