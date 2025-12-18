Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\lib\\math.lua");
Include("\\script\\event\\quockhanh\\head.lua");
-----------------EVENT CONFIG-----------------
END_TALK = "Tho¸t/no";
NHANROI = "Ng­¬i ®· nhËn phÇn th­ëng nµy råi.";
MAX_RESTTIME = 1

----------------------------------------------

function main(NpcIndex)
dofile("script/event/quockhanh/npcsukien.lua")
	if (SYS_SWITCH_QUOC_KHANH == 1) then
	HoatDong1(NpcIndex)
	else
	Talk(1,"","HiÖn t¹i ch­a tíi ngµy diÔn ra ho¹t ®éng Quèc Kh¸nh.\n".."Xin ®¹i hiÖp vui lßng ghÐ l¹i sau.");
	end
end

function HoatDong1(NpcIndex)
    local szHello =
        "<color=yellow>NPC Sù KiÖn Quèc Kh¸nh<color>:\n" ..
        "Ho¹t ®éng <color=white>Mõng ngµy Quèc Kh¸nh 2-9<color> diÔn ra tõ ngµy\n" ..
        "<bclr=blue>02-09-2025 ®Õn 10-10-2025<bclr>.\n" ..
        "Tham gia sù kiÖn ®ång ®¹o sÏ nhËn ®­îc nhiÒu vËt phÈm gi¸ trÞ."

    local Tab_Menu = {
        "Mua Giá Hoa Quèc Kh¸nh(5 Xu)/muagiohoaquockhanh#1",
        "GhÐp Bã Hoa Quèc Kh¸nh (10 v¹n)/ghepevent",
        "Mua Giá §ùng Hoa (2 Xu)/muahoahong#1",
        "Mua D©y Buéc Hoa (10 v¹n)/muadaybuothoa#1",
        "NhËn Th­ëng Mèc Event/NhanMocEvent",
        "Reset Event Giá Hoa Quèc Kh¸nh Xanh/reset",
		--"Reset Event Test Game/resetG",
        "Ta kh«ng cÇn ng­¬i d¹y b¶o/no",
    }

    
    Say(szHello, getn(Tab_Menu), Tab_Menu)
end


function muagiohoaquockhanh(_, nNumber)
    nNumber = tonumber(nNumber)
    if nNumber == 1 then
        AskClientForNumber("muagiohoaquockhanh1", 1, 60, "NhËp sè l­îng")
    end
end

function muagiohoaquockhanh1(_, nNumber)
    nNumber = tonumber(nNumber)
    if nNumber < 1 then return end

    local ITEM_ID_XU   = 19   -- ID Xu
    local ITEM_ID_GIO  = 142  -- ID Gio hoa Quoc Khanh
    local BIND_STATE   = 2    -- Trang thai khoa
    local COST_PER     = 5    -- 5 Xu / 1 gio

    local cost = COST_PER * nNumber

    if GetTaskItemCount(ITEM_ID_XU) < cost then
        Msg2Player("§¹i hiÖp cÇn "..cost.." Xu ®Ó mua "..nNumber.." Giá Hoa Hoa")
        return
    end
	if (FindEmptyPlace(1,1) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;

    for i = 1, nNumber do
        local nIndex = AddScriptItem(ITEM_ID_GIO,DAYS)
    end
    DelTaskItem(ITEM_ID_XU, cost)

    Msg2Player("Mua thµnh c«ng <color=yellow>x"..nNumber.."<color> Giá hoa Quèc KhÊnh (®· khãa).")
end

function muahoahong(_, nNumber)
    nNumber = tonumber(nNumber)
    if nNumber == 1 then
        AskClientForNumber("muahoahong1", 1, 60, "NhËp sè l­îng")
    end
end

function muahoahong1(_, nNumber)
    nNumber = tonumber(nNumber)
    if nNumber < 1 then return end

    local ITEM_ID_XU   = 19   -- ID Xu
    local ITEM_ID_GIO  = 67  -- ID Gio hoa Quoc Khanh
    local BIND_STATE   = 2    -- Trang thai khoa
    local COST_PER     = 2    -- 2 Xu / 1 gio

    local cost = COST_PER * nNumber

    if GetTaskItemCount(ITEM_ID_XU) < cost then
        Msg2Player("§¹i hiÖp cÇn "..cost.." Xu ®Ó mua "..nNumber.." Giá Hoa Hoa")
        return
    end
	if (FindEmptyPlace(1,1) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;

    for i = 1, nNumber do
        local nIndex = AddTaskItem(ITEM_ID_GIO,DAYS)
    end
    DelTaskItem(ITEM_ID_XU, cost)

    Msg2Player("Mua thµnh c«ng <color=yellow>x"..nNumber.."<color> Giá hoa Quèc KhÊnh (®· khãa).")
end
-- Mua "D©y Buéc Hoa" (group=6, detail=66) = 10 v¹n / c¸i
function muadaybuothoa(_, nNumber)
    nNumber = tonumber(nNumber)
    if nNumber == 1 then
        AskClientForNumber("muadaybuothoa1", 1, 500, "NhËp sè l­îng")
    end
end

function muadaybuothoa1(_, nNumber)
    nNumber = tonumber(nNumber) or 0
    if nNumber < 1 then return end

    local COST_PER = 100000           -- 10 v¹n / c¸i
    local total    = COST_PER * nNumber
    if GetCash() < total then
        Talk(1,"","§¹i HiÖp cÇn "..(total/10000).." v¹n l­îng §Ó mua "..nNumber.." D©y Buéc Hoa")
        return
    end
    if type(FindEmptyPlace)=="function" and FindEmptyPlace(1,1)==0 then
        Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn Ýt nhÊt <color=yellow>1x1<color> « trèng!")
        return
    end
    for i = 1, nNumber do
        AddTaskItem(66,DAYS)
    end
    Pay(total)
    Msg2Player("Mua thµnh c«ng <color=yellow>x"..nNumber.."<color> D©y Buéc Hoa.")
end
-- CÊu H×nh Nguyªn LiÖu
REQ_HOA  = REQ_HOA  or 20  -- detail 64 (Hoa Quèc Kh¸nh)
REQ_GIAY = REQ_GIAY or 10   -- detail 65 (GiÊy G·i Hoa)
REQ_DAY  = REQ_DAY  or 1   -- detail 66 (D©y Buéc Hoa)
REQ_GIO  = REQ_GIO  or 1   -- detail 67 (Giá §ïng Hoa)

function ghepevent()

    OpenGiveBox("§Æt vËt phÈm vµo trong",
        "CÇn: * Hoa Quèc Kh¸nh x"..REQ_HOA.." \n"..
        "  	  * GiÊy G·i Hoa x"..REQ_GIAY.." \n"..
        "     * D©y Buéc Hoa x"..REQ_DAY.." \n"..
        "     * Giá §ïng Hoa x"..REQ_GIO.." \n" ..
        "**Mçi bã tèn 10 v¹n, ®ñ yªu cÇu cho ra 1 Bã.",
        "GhepEventExcute")
end
function GhepEventExcute()
    if SYS_SWITCH_QUOC_KHANH == 0 then return end

    local DAYS = DAYS or 15
    local COST_PER = 100000           -- 10 v¹n / bó hoa

    local ROOMG = 8
    local gNL, pNL = 6, 1
    local dHoa, dGiay, dDay, dGio = 64, 65, 66, 67
    local dOUT = 143 -- ID script item bó hoa

    local idxList = {}
    local idxN = 0
    local totHoa, totGiay, totDay, totGio = 0, 0, 0, 0

    for i = 0, 5 do
        for j = 0, 3 do
            local nIdx = GetROItem(ROOMG, i, j)
            local nG, nD, nP, nL, Ser, nLuc = GetItemProp(nIdx)
            if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
                if nG == gNL and nP == pNL then
                    idxN = idxN + 1
                    idxList[idxN] = nIdx
                    local st = 1
                    if type(GetItemStackCount) == "function" then
                        local v = GetItemStackCount(nIdx)
                        if v and v > 0 then st = v end
                    end
                    if     nD == dHoa  then totHoa  = totHoa  + st
                    elseif nD == dGiay then totGiay = totGiay + st
                    elseif nD == dDay  then totDay  = totDay  + st
                    elseif nD == dGio  then totGio  = totGio  + st
                    end
                end
            end
        end
    end

    local nSetHoa  = IntDiv(totHoa,  REQ_HOA)
    local nSetGiay = IntDiv(totGiay, REQ_GIAY)
    local nSetDay  = IntDiv(totDay,  REQ_DAY)
    local nSetGio  = IntDiv(totGio,  REQ_GIO)

    local nSet = nSetHoa
    if nSetGiay < nSet then nSet = nSetGiay end
    if nSetDay  < nSet then nSet = nSetDay  end
    if nSetGio  < nSet then nSet = nSetGio  end

    if nSet < 1 then
        Talk(1, "", "Ch­a ®ñ nguyªn liÖu: cÇn Hoa x"..REQ_HOA..", GiÊy x"..REQ_GIAY..", D©y x"..REQ_DAY..", Giá x"..REQ_GIO..".")
        return
    end

    local totalCost = COST_PER * nSet
    if GetCash() < totalCost then
        Talk(1,"","§¹i HiÖp cÇn "..(totalCost/10000).." v¹n l­îng §Ó ghÐp "..nSet.." bó Hoa Quèc Kh¸nh")
        return
    end
    Pay(totalCost)

    local needHoa  = nSet * REQ_HOA
    local needGiay = nSet * REQ_GIAY
    local needDay  = nSet * REQ_DAY
    local needGio  = nSet * REQ_GIO

    local leftHoa  = totHoa  - needHoa
    local leftGiay = totGiay - needGiay
    local leftDay  = totDay  - needDay
    local leftGio  = totGio  - needGio

    local slotLeftTypes = 0
    if leftHoa  > 0 then slotLeftTypes = slotLeftTypes + 1 end
    if leftGiay > 0 then slotLeftTypes = slotLeftTypes + 1 end
    if leftDay  > 0 then slotLeftTypes = slotLeftTypes + 1 end
    if leftGio  > 0 then slotLeftTypes = slotLeftTypes + 1 end
    local needSlots = nSet + slotLeftTypes

    if type(CheckRoom) == "function" then
        if CheckRoom(needSlots, 1) == 0 then Talk(1,"",15679); return end
    elseif type(FindEmptyPlace) == "function" then
        if FindEmptyPlace(1, 1) == 0 then
            Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn « trèng cho "..needSlots.." vËt phÈm!")
            return
        end
    end

    for t = 1, idxN do
        if idxList[t] and idxList[t] > 0 then
            DelItemByIndex(idxList[t])
        end
    end

    -- Add bó hoa
    for k = 1, nSet do
        AddScriptItem(dOUT, DAYS)
    end

    -- Tr? l?i ph?n du
    for k = 1, leftHoa do
        AddTaskItem(64, DAYS)
    end
    for k = 1, leftGiay do
        AddTaskItem(65, DAYS)
    end
    for k = 1, leftDay do
        AddTaskItem(66, DAYS)
    end
    for k = 1, leftGio do
        AddTaskItem(67, DAYS)
    end

    if nSet == 1 then
        Msg2Player("B¹n ghÐp thµnh c«ng 1 <color=yellow>Bã Hoa Quèc Kh¸nh<color> (®· tr¶ phÇn d­ vÒ hµnh trang).")
    else
        Msg2Player("B¹n ghÐp thµnh c«ng <color=yellow>x"..nSet.."<color> Bã Hoa Quèc Kh¸nh (®· tr¶ phÇn d­ vÒ hµnh trang).")
    end
end

--========================================================================================================================================
function reset()
    local NEED_USED   = 50000       -- cÇn sö dông ®ñ 50.000
    local NEED_XU     = 1000        -- phÝ reset = 1000 Xu
    local ITEM_ID_XU  = 19          -- Xu (task item)
    local MAX_RESET   = 2           -- tèi ®a 2 lÇn reset

    local used   = GetTask(T_GIOHOAQUOCKHANH)
    local nReset = GetTask(T_RESTEVENTQUOCKHANH)

    -- KiÓm tra ®· sö dông ®ñ mèc
    if used < NEED_USED then
        Talk(1,"","§¹i HiÖp ch­a sö dông §ñ "..NEED_USED.." Giá Hoa Quèc Kh¸nh, sè l­îng hiÖn t¹i: "..used.." ")
        return
    end

    -- Giíi h¹n sç lÇn reset
    if nReset >= MAX_RESET then
        Talk(1,"","Ng­¬i ®· reset ®¹t giíi h¹n <color=yellow>"..MAX_RESET.."<color> lÇn, kh«ng thÓ thùc hiÖn thªm.")
        return
    end

    -- KiÓm tra Xu
    if GetTaskItemCount(ITEM_ID_XU) < NEED_XU then
        Talk(1,"","Kh«ng ®ñ <color=yellow>"..NEED_XU.."<color> Xu, vui lßng kiÓm tra l¹i")
        return
    end

    -- Trõ Xu
    DelTaskItem(ITEM_ID_XU, NEED_XU)

    -- Reset vµ cËp nhËt ®Õm lÇn reset
    SetTask(T_GIOHOAQUOCKHANH, 0)
    SetTask(T_NHANMOCTQUOCKHANH, 0)
    SetTask(T_RESTEVENTQUOCKHANH, nReset + 1)

    local conlai = MAX_RESET - (nReset + 1)
    Talk(1,"","B¹n ®· reset Event thµnh c«ng. Sç lÇn reset: <color=yellow>"..(nReset+1).."/"..MAX_RESET.."<color>. Cßn l¹i: "..conlai.." lÇn.")
end

function NhanMocEvent()
    local szHello =
        "<color=yellow>NPC Sù KiÖn Quèc Kh¸nh<color>:\n" ..
        "Ho¹t ®éng <color=white>Mõng ngµy Quèc Kh¸nh 2-9<color> diÔn ra tõ ngµy\n" ..
        "<bclr=blue>02-09-2025 ®Õn 10-10-2025<bclr>.\n" ..
        "H·y nhËn phÇn th­ëng theo mèc sö dông Giá Hoa Quèc Kh¸nh."

    local Tab_Menu = {
        "NhËn Mèc 10k/Moc10",
        "NhËn Mèc 20k/Moc20",
        "NhËn Mèc 30k/Moc30",
        "NhËn Mèc 50k/Moc50",
		--"NhËn Reset/ResetMoc",
        END_TALK,
    }

    Say(szHello, getn(Tab_Menu), Tab_Menu)
end
----------------------------------------------------------------
-- KHAI BÁO ITEM THUONG
----------------------------------------------------------------
P_BAN_NHUOC_TAM_KINH = {7, 24, 0, 0, 0, 0, 0}
P_TINH_HONG_BAO_THACH = {6, 15, 0, 0, 0, 0, 0}
P_LAM_THUY_TINH       = {6, 16, 0, 0, 0, 0, 0}
P_TU_THUY_TINH        = {6, 17, 0, 0, 0, 0, 0}
P_LOC_THUY_TINH       = {6, 18, 0, 0, 0, 0, 0}

----------------------------------------------------------------
-- KHAI BÁO GOLD ITEM (AddGoldItem dùng theo ID ®óng trong file)
----------------------------------------------------------------
GOLD_VO_DANH_CHI_HOAN_ID = 142
GOLD_VO_DANH_GIOI_CHI_ID = 143

-- Pool GOLD cho c¸c mèc 30/40/50 random trong 204..207
-- B¹n ®æi range hoÆc thªm nhieu entry gold khac tai dây.
GOLD_POOL = {
    -- 1 entry: random range
    { t="gold", id_min=204, id_max=207 },
    -- { t="gold", id=210 },
    -- { t="gold", id=215 },
}

----------------------------------------------------------------
-- CÊu H×nh Qïa Mèc (Cã % + Nhãm CHOICE)
-- entry th­êng:  { t="item"/"script"/"task"/"gold", p=..., detail=..., id=..., id_min=..., id_max=..., count=..., bind=..., rate=..., expire=... }
-- entry nhãm:    { t="choice", rate=..., count=..., list={ <entry1>, <entry2>, ... } }
----------------------------------------------------------------
MOC_REWARD_CFG = {}

-- Mèc 10: random 1 trong [BNTK + 4 vËt liÖu] (100%)
MOC_REWARD_CFG[1] = {
    need = 10000,
    name = "Mèc 10k",
    rewards = {
        { t="choice", rate=100, count=1, list = {
            { t="item", p=P_BAN_NHUOC_TAM_KINH, bind=0 },
            { t="item", p=P_LAM_THUY_TINH,       bind=0 },
            { t="item", p=P_TU_THUY_TINH,        bind=0 },
            { t="item", p=P_LOC_THUY_TINH,       bind=0 },
            { t="item", p=P_TINH_HONG_BAO_THACH, bind=0 },
        } },
    },
}

-- Moc 20: 20% tróng 1 trong 2 GOLD (142/143), 80% tróng 1 vËt liÖu/BNTK
MOC_REWARD_CFG[2] = {
    need = 20000,
    name = "Mèc 20k",
    rewards = {
        { t="choice", rate=5, count=1, list = {
            { t="gold", id=GOLD_VO_DANH_CHI_HOAN_ID },
            { t="gold", id=GOLD_VO_DANH_GIOI_CHI_ID },
        } },
        { t="choice", rate=60, count=1, list = {
            { t="item", p=P_BAN_NHUOC_TAM_KINH, bind=0 },
            { t="item", p=P_LAM_THUY_TINH,       bind=0 },
            { t="item", p=P_TU_THUY_TINH,        bind=0 },
            { t="item", p=P_LOC_THUY_TINH,       bind=0 },
            { t="item", p=P_TINH_HONG_BAO_THACH, bind=0 },
        } },
    },
}

-- Moc 30: random 1 GOLD (100%)
MOC_REWARD_CFG[3] = {
    need = 30000,
    name = "Mèc 30k",
    rewards = {
        { t="choice", rate=100, count=1, list = GOLD_POOL },
    },
}

-- Moc 40: random 1 GOLD (100%)
MOC_REWARD_CFG[4] = {
    need = 40000,
    name = "Mèc 40k",
    rewards = {
        { t="choice", rate=100, count=1, list = GOLD_POOL },
    },
}

-- Moc 50: random 1 GOLD (100%)
MOC_REWARD_CFG[5] = {
    need = 50000,
    name = "Mèc 50k",
    rewards = {
        { t="choice", rate=100, count=1, list = GOLD_POOL },
    },
}

----------------------------------------------------------------
-- LOG SAFE
----------------------------------------------------------------
function logPhanThuong(str)
    local gm_Log = "dulieu/Event/NhanMocEvent.txt"
    local line = date("%H:%M:%S_%d-%m-%y").."\t"..(str or "").."\n"

    if type(openfile) == "function" and type(write) == "function" and type(closefile) == "function" then
        local f = openfile(gm_Log, "a")
        if f then
            write(f, line)
            closefile(f)
        end
        return
    end

    -- Fallback 1: writeto (n?u có)
    if type(writeto) == "function" and type(write) == "function" then
        writeto(gm_Log)
        write(line)
        writeto()
        return
    end

end

----------------------------------------------------------------
-- HELPER CHUNG
----------------------------------------------------------------
function ListLen(t)
    local n = 0
    while t and t[n+1] ~= nil do n = n + 1 end
    return n
end

function RandPercent(p)
    local r = 100
    if type(random) == "function" then r = random(1,100) end
    p = tonumber(p) or 100
    if p < 0 then p = 0 end
    if p > 100 then p = 100 end
    return (r <= p)
end

function BuildGrantedRewards(rewards)
    local out = {}
    local nout = 0
    local i = 1
    while rewards and rewards[i] do
        local e = rewards[i]
        local rate = e.rate; if rate == nil then rate = 100 end
        local cnt = e.count or 1

        if e.t == "choice" then
            local L = ListLen(e.list)
            local j = 1
            while j <= cnt do
                if RandPercent(rate) and L > 0 then
                    local pick = 1
                    if type(random)=="function" then pick = random(1, L) end
                    local c = e.list[pick]
                    if c then
                        nout = nout + 1
                        out[nout] = {
                            t = c.t, p = c.p, detail = c.detail,
                            id = c.id, id_min = c.id_min, id_max = c.id_max,
                            bind = c.bind, expire = c.expire
                        }
                    end
                end
                j = j + 1
            end

        else
            local j = 1
            while j <= cnt do
                if RandPercent(rate) then
                    nout = nout + 1
                    out[nout] = {
                        t = e.t, p = e.p, detail = e.detail,
                        id = e.id, id_min = e.id_min, id_max = e.id_max,
                        bind = e.bind, expire = e.expire
                    }
                end
                j = j + 1
            end
        end

        i = i + 1
    end
    return out
end

function CountBagSlotsGranted(granted)
    local need = 0
    local i = 1
    while granted and granted[i] do
        local g = granted[i]
        if g.t == "item" or g.t == "script" or g.t == "gold" then
            need = need + 1
        end
        i = i + 1
    end
    return need
end

function GiveResolvedReward(g)
    if g.t == "item" then
        local p = g.p or {}
        local pn = 0; while p[pn+1] ~= nil do pn = pn + 1 end
        local p1 = p[1] or 0;  local p2 = p[2] or 0;  local p3 = p[3] or 0
        local p4 = p[4] or 0;  local p5 = p[5] or 0;  local p6 = p[6] or 0
        local p7 = p[7] or 0;  local p8 = p[8] or 0;  local p9 = p[9] or 0
        local p10= p[10]or 0;  local p11= p[11]or 0;  local p12= p[12]or 0
        local p13= p[13]or 0
        local idx = 0
        if pn >= 13 then
            idx = AddItem(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13)
        elseif pn == 8 then
            idx = AddItem(p1,p2,p3,p4,p5,p6,p7,p8)
        else
            idx = AddItem(p1,p2,p3,p4,p5,p6,p7)
        end
        if g.bind and idx and idx > 0 and type(SetItemBindState)=="function" then
            SetItemBindState(idx, g.bind)
        end

    elseif g.t == "script" then
        local idx
        if g.expire then idx = AddScriptItem(g.detail, g.expire)
        else idx = AddScriptItem(g.detail) end
        if g.bind and idx and idx > 0 and type(SetItemBindState)=="function" then
            SetItemBindState(idx, g.bind)
        end

    elseif g.t == "task" then
        AddTaskItem(g.detail)

    elseif g.t == "gold" then
        local gid = g.id
        if not gid then
            local a = g.id_min or 0
            local b = g.id_max or 0
            if type(random)=="function" and a > 0 and b >= a then
                gid = random(a, b)
            end
        end
        if gid then
            AddGoldItem(gid)
        end
    end
end

function GiveResolvedRewards(granted)
    local i = 1
    while granted and granted[i] do
        GiveResolvedReward(granted[i])
        i = i + 1
    end
end

----------------------------------------------------------------
-- (Dùng T_GIOHOAQUOCKHANH và T_NHANMOCTQUOCKHANH)
----------------------------------------------------------------
function ClaimMoc(moc_idx)
    local cfg = MOC_REWARD_CFG[moc_idx]
    if not cfg then return end

    local used = GetTask(T_GIOHOAQUOCKHANH)
    if used < cfg.need then
        Talk(1,"","§¹i HiÖp Ch­a Sö Dông §ñ "..cfg.need.." Giá Hoa Quèc Kh¸nh , Sè l­îng hiÖn t¹i : "..used.." ")
        return
    end

    local got = GetTask(T_NHANMOCTQUOCKHANH) -- m?c cao nh?t dã nh?n (0..5)
    if got >= moc_idx then
        Talk(1,"","§¹i hiÖp ®· nhËn phÇn th­ëng mÈc nµy råi!")
        return
    end

    local granted = BuildGrantedRewards(cfg.rewards)

    local needSlot = CountBagSlotsGranted(granted)
    if needSlot > 0 then
        if type(CheckRoom)=="function" and CheckRoom(needSlot,1)==0 then Talk(1,"",15679) return end
        if type(FindEmptyPlace)=="function" and FindEmptyPlace(1,1)==0 then
            Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn ít nhÊt "..needSlot.." « trèng!")
            return
        end
    end

    GiveResolvedRewards(granted)
    SetTask(T_NHANMOCTQUOCKHANH, moc_idx)

    local len = ListLen(granted)
    if len == 0 then
        Msg2Player("RÊt tiÕt, kh«ng trïng quµ ë "..cfg.name..".")
    end

    if type(logPhanThuong)=="function" then
        logPhanThuong(GetAccount().."\t\t"..GetName().."\t\t Nhan "..cfg.name.." (ti le)")
    end
    if type(Msg2SubWorld)=="function" then
        Msg2SubWorld("<pic=135><color=cyan> Chóc mõng <color=pink>"..GetName().."<color> nhËn "..cfg.name.."!")
    end
end

----------------------------------------------------------------
-- API GOI TU MENU
----------------------------------------------------------------
function Moc10() ClaimMoc(1) end
function Moc20() ClaimMoc(2) end
function Moc30() ClaimMoc(3) end
function Moc40() ClaimMoc(4) end
function Moc50() ClaimMoc(5) end

function resetG()
SetTask(T_NTBOHOAQUOCKHANH_NAM, 0)
SetTask(T_NTBOHOAQUOCKHANH_NU, 0)
end
function no()
end
