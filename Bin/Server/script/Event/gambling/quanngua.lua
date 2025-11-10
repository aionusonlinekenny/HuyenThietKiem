-- =========================================================
--  HORSE RACE NPC DIALOG (Lua 4/5) - PHIÊN B?N Ð?NG NH?T
--  Không _G, không local function, không "Input".
--  M? c?a cu?c NGAY khi ngu?i choi b?m (idempotent).
-- =========================================================

Include("\\script\\lib\\TaskLib.lua");

if RACE_MISSION == nil then RACE_MISSION = 16 end
if TM_OPEN == nil then TM_OPEN = 17 end
if TM_MOVE == nil then TM_MOVE = 18 end
if TM_PACE == nil then TM_PACE = 19 end
if TM_CLAIM == nil then TM_CLAIM = 20 end

-- Luu lane dã ch?n (n?u server chua có h?ng)
if SELSKILLNO == nil then SELSKILLNO = 4321 end

function main(NpcIndex)
    local nW, nX, nY = GetWorldPos()
    local sz = "<color=yellow>Giai dua Linh Thu - Lam An<color>\n" ..
               "Ban: " .. nW .. "  X:" .. nX .. "  Y:" .. nY

    local Tab = {
        "Tham gia dat cuoc/datcuoc",
        "Nhan thuong/thuong",
        "Xem ket qua/ketqua",
        "Tim hieu luat choi/timhieu",
        "Thoat/no"
    }
    Say(sz, getn(Tab), Tab)
end

-- B?t d?u d?t cu?c
function datcuoc()
    -- N?u dang dua thì không cho d?t
    if GetMissionV(0) == 1 then
        Talk(1, "", "Cuoc dua dang dien ra. Vui long doi vong sau!")
        return
    end

    if IsMission(RACE_MISSION) == 0 then OpenMission(RACE_MISSION) end

    -- Luôn cho phép d?t khi KHÔNG dang dua:
    -- Ch? d?ng reset “vòng cu?c” nh? nhàng (idempotent)
    if OpenBettingNow ~= nil then OpenBettingNow() end

    -- N?u dã d?t r?i, báo l?i thông tin
    local idx = PIdx2MSDIdx(RACE_MISSION, PlayerIndex)
    if idx > 0 then
        local nNo  = GetPMParam(RACE_MISSION, idx, 1)
        local nKNB = GetPMParam(RACE_MISSION, idx, 3)
        Talk(1, "", "Ban da dat Linh thu so " .. nNo .. " voi " .. nKNB .. " KNB. Hay cho ket qua!")
        return
    end

    -- Ch?n ng?a (1..6)
    local opt = {
        "Chon Linh thu #1/selngua#0",
        "Chon Linh thu #2/selngua#1",
        "Chon Linh thu #3/selngua#2",
        "Chon Linh thu #4/selngua#3",
        "Chon Linh thu #5/selngua#4",
        "Chon Linh thu #6/selngua#5",
        "Thoi, de lan sau./no"
    }
    Say("Chon linh thu ban muon dat:", getn(opt), opt)
end

-- Luu lane, chuy?n qua ch?n s? KNB b?ng menu (1..15)
function selngua(sel)
    SetTaskTemp(SELSKILLNO, sel + 1) -- lane 1..6
    chonknb1()
end

-- Trang 1: 1..10 KNB + next
function chonknb1()
    local opt = {
        "Dat 1 KNB/knb#1",
        "Dat 2 KNB/knb#2",
        "Dat 3 KNB/knb#3",
        "Dat 4 KNB/knb#4",
        "Dat 5 KNB/knb#5",
        "Dat 6 KNB/knb#6",
        "Dat 7 KNB/knb#7",
        "Dat 8 KNB/knb#8",
        "Dat 9 KNB/knb#9",
        "Dat 10 KNB/knb#10",
        "Trang sau (11-15)/chonknb2",
        "Huy/no"
    }
    Say("Chon so KNB muon dat:", getn(opt), opt)
end

-- Trang 2: 11..15 KNB + back
function chonknb2()
    local opt = {
        "Dat 11 KNB/knb#11",
        "Dat 12 KNB/knb#12",
        "Dat 13 KNB/knb#13",
        "Dat 14 KNB/knb#14",
        "Dat 15 KNB/knb#15",
        "Quay lai (1-10)/chonknb1",
        "Huy/no"
    }
    Say("Chon so KNB muon dat:", getn(opt), opt)
end

-- X? lý d?t v?i s? KNB dã ch?n
function knb(num)
    do_datcuoc(num)
end

-- Th?c s? t?o vé cu?c
function do_datcuoc(num)
    if GetLevel() < 10 then Talk(1, "", "Can cap 10 tro len.") return end
    if num < 1 or num > 15 then Talk(1, "", "Chi cho dat tu 1 den 15 KNB.") return end

    -- KNB theo co ch? cu: Item 17,3
    if GetItemCount(17, 3) < num then
        Talk(1, "", "Ban khong du " .. num .. " KNB!")
        return
    end

    -- Ch? m?t di?u ki?n duy nh?t: KHÔNG du?c dang dua
    if GetMissionV(0) == 1 then
        Talk(1, "", "Cuoc dua dang dien ra!")
        return
    end

    -- Ðã có vé ván này?
    local idx = PIdx2MSDIdx(RACE_MISSION, PlayerIndex)
    if idx > 0 then
        Talk(1, "", "Ban da co ve cuoc vong nay.")
        return
    end

    -- L?y lane dã ch?n
    local nNo = GetTaskTemp(SELSKILLNO)
    if nNo == nil or nNo < 1 or nNo > 6 then
        Talk(1, "", "Chon linh thu truoc khi dat.")
        return
    end

    -- Cho vào mission và ghi vé
    idx = AddMSPlayer(RACE_MISSION, 0)
    if idx > 0 then
        SetPMParam(RACE_MISSION, idx, 1, nNo) -- lane
        SetPMParam(RACE_MISSION, idx, 3, num) -- KNB
        DelItem(17, 3, num)                   -- tr? KNB
        Talk(1, "", "Da dat Linh thu so " .. nNo .. " voi " .. num .. " KNB.")
    else
        if Msg2Player ~= nil then Msg2Player("Khong the dat cuoc! Vui long thu lai.") end
    end
end


-- Nh?n thu?ng (ch? Ve Nh?t, x5 KNB)
function thuong()
    if IsMission(RACE_MISSION) == 0 then Talk(1, "", "Su kien chua mo.") return end
    if GetMissionV(0) ~= 0 then Talk(1, "", "Cuoc dua dang dien ra!") return end
    if GetMissionV(7) == 0 then Talk(1, "", "Chua co ket qua!") return end

    local idx = PIdx2MSDIdx(RACE_MISSION, PlayerIndex)
    if idx <= 0 then Talk(1, "", "Ban chua dat cuoc vong nay.") return end
    if GetPMParam(RACE_MISSION, idx, 2) == 1 then Talk(1, "", "Ban da nhan thuong roi.") return end

    local nNo  = GetPMParam(RACE_MISSION, idx, 1)
    local nKNB = GetPMParam(RACE_MISSION, idx, 3)
    if nNo == GetMissionV(7) then
        SetPMParam(RACE_MISSION, idx, 2, 1)
        local cnt = nKNB * 5
        local i
        for i = 1, cnt do AddMat(17) end
        if Msg2Player ~= nil then Msg2Player("Chuc mung ban THANG CUOC! Da nhan " .. cnt .. " KNB.") end
    else
        Talk(1, "", "Ve Nhat: " .. GetMissionV(7) .. " - Tiec la khong trung.")
    end
end

function ketqua()
    if GetMissionV(0) ~= 0 then Talk(1, "", "Cuoc dua dang dien ra!") return end
    if GetMissionV(7) == 0 then Talk(1, "", "Chua co ket qua!") return end
    Talk(1, "", "Ket qua: " .. GetMissionV(7) .. ", " .. GetMissionV(8) .. ", " .. GetMissionV(9))
end

function timhieu()
    Talk(1, "", "Quy tac: Mo cua cuoc 60s, moi nguoi dat toi da 15 KNB.\nKet thuc, chi tra thuong cho Ve Nhat (x5 so KNB da dat).")
end

function no() end
function noinput() end
