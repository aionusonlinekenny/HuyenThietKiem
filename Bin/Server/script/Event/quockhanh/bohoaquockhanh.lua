Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\event\\quockhanh\\head.lua");
--===============Config Event===============--
DangCapSuDung 	= 60
ToiDaSuDung 	= 1000
TenVatPham		= "Bã Hoa Quèc Kh¸nh" 
SoKinhNghiem	= {{2500000,5/2},}

-----------------------------------------------


function OnUse(nIdx)
dofile("script/item/event/event0323.lua")
	nTaskValue = GetTask(T_BOHOAQUOCKHANH);
	if (GetLevel() < DangCapSuDung) then Talk(1,"","§¼ng cÊp trªn "..DangCapSuDung.." míi cã thÓ sö dông vËt phÈm nµy.") return end
	----------------------------------------------------
          if nTaskValue >= ToiDaSuDung then Talk(1,"","Ng­¬i ®· dïng <color=red>"..ToiDaSuDung.." c¸i "..TenVatPham.." <color>råi kh«ng thÓ sö dông thªm.") return end
        SetTask(T_BOHOAQUOCKHANH,GetTask(T_BOHOAQUOCKHANH)+1); 
		 Msg2Player("<color=white>B¹n sö dông<color=cyan> "..GetTask(T_BOHOAQUOCKHANH).."/"..ToiDaSuDung.."<color><color=orange> "..TenVatPham.." <color>nhËn ®­îc <color=yellow>"..SoKinhNghiem[1][2].."<color> triÖu ®iÓm kinh nghiÖm.")
		AddOwnExp(SoKinhNghiem[1][1])
		nhanthuong(50) --% Nhan Thuong Trang Bi Hoang Kim --> 10 = 10%
	----------------------------------------------------
	return 1
end;
--========================================================
-- NHAN THUONG GOLD THEO GIOI TINH + TIEN DO KHONG LAP
-- Can them vao taskid.lua:
--   T_NTBOHOAQUOCKHANH_NAM = <ID trong>
--   T_NTBOHOAQUOCKHANH_NU  = <ID trong>
--========================================================

-- Fallback: vat lieu (group=6, detail: 13/14/15/20)
function _give_fallback_material()
    if type(FindEmptyPlace)=="function" and FindEmptyPlace(1,1)==0 then
        Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn Ýt nhÊt <color=yellow>1x1<color> « trèng!")
        return 0
    end

    -- { Tªn hiÓn thÞ,  Genre, Detail, Ty Le }
    local TAB_FALLBACK = {
        {"Phóc Duyªn Lé (tiÓu)",  7, 11, 10},  -- 10%
        {"Phóc Duyªn Lé (trung)", 7, 12, 10},  -- 10%
        {"Phóc Duyªn Lé (§¹i)",   7, 13, 10},  -- 10%
		{"Tiªn Th¶o Lé",		  7,  3, 10},  -- 10%
		{"Thiªn S¬n B¶o Lé",	  7,  5, 10},  -- 10%
		{"QuÕ Hoa Töu",	 		  7,  6, 10},  -- 10%
		{"Lam Thuû Tinh",		  6, 16,  5},  --  2%
		{"Lôc Thuû Tinh",		  6, 18,  4},  --  2%
		{"Tö Thuû Tinh",		  6, 17,  3},  --  2%
		{"Tinh Hång B¶o Th¹ch",	  6, 15,  2},  --  1%
		{"ThiÕt La H¸n",		  7,  1,  3},  --  3%
		{"Vâ L©m MËt TÞch",		  7,  2,  1},  --  3%
		{"B¸n nh­îc t©m kinh",     7, 24,  3},  --  3%
    }

    local total = 0
    local n = (type(getn)=="function") and getn(TAB_FALLBACK) or 0
    if n == 0 then
        local i = 0
        while TAB_FALLBACK[i+1] do i = i + 1 end
        n = i
    end
    local i = 1
    while i <= n do
        total = total + (TAB_FALLBACK[i][4] or 0)
        i = i + 1
    end
    if total <= 0 then return 0 end

    -- Random theo träng sç
    local r = (type(random)=="function") and random(1,total)
    local pick = 1
    local acc = 0
    i = 1
    while i <= n do
        acc = acc + (TAB_FALLBACK[i][4] or 0)
        if r <= acc then pick = i; break end
        i = i + 1
    end

    local szName = TAB_FALLBACK[pick][1]
    local nGr    = TAB_FALLBACK[pick][2]
    local nDet   = TAB_FALLBACK[pick][3]

    AddItem(nGr, nDet, 0, 0, 0, 0, 0)
    Msg2Player("NhËn ®­îc 1 <color=yellow>"..szName.."<color>.")

    if type(logPhanThuong) == "function" then
        local acc = (type(GetAccount)=="function") and GetAccount() or ""
        local nm  = (type(GetName)=="function") and GetName() or ""
        logPhanThuong(acc.."\t"..nm.."\tNhanThuong -> Fallback: "..szName.." ("..nGr..","..nDet..")")
    end
    return 1
end


--=== B¶NG TÊN + ID (TCVN3) ===--
TAB_GOLD_MALE = {
    { id = 186, name = "HiÖp Cèt ThiÕt HuyÕt Sam" },
    { id = 187, name = "HiÖp Cèt §a T×nh Hoµn"    },
    { id = 188, name = "HiÖp Cèt §an T©m Giíi"    },
    { id = 189, name = "HiÖp Cèt T×nh ý KÕt"      },
}

TAB_GOLD_FEMALE = {
    { id = 190, name = "Nhu T×nh C©n Quèc Nghª Th­êng" },
    { id = 191, name = "Nhu T×nh Thôc N÷ H¹ng Liªn"   },
    { id = 192, name = "Nhu T×nh Phông Nghi Giíi ChØ" },
    { id = 193, name = "Nhu T×nh TuÖ T©m Ngäc Béi"    },
}

-- ÄÕm phÇn tö töng ®Çu (kh«ng dïng #/ipairs)
function _len(tbl)
    local i = 0
    while tbl[i+1] do i = i + 1 end
    return i
end

function _get_table_by_sex()
    local sex = tonumber(GetSex()) or 0  -- 0=nam, 1=n÷
    if sex == 0 then return TAB_GOLD_MALE, T_NTBOHOAQUOCKHANH_NAM
    else              return TAB_GOLD_FEMALE, T_NTBOHOAQUOCKHANH_NU
    end
end

-- Kiem tra cho trong 1x1
function _check_bag_1x1()
    if type(CheckRoom) == "function" then
        if CheckRoom(1,1) == 0 then Talk(1,"",15679); return 0 end
    elseif type(FindEmptyPlace) == "function" then
        if FindEmptyPlace(1,1) == 0 then
            Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn Ýt nhÊt <color=yellow>1x1<color> « trèng!")
            return 0
        end
    end
    return 1
end

-- LÊy món GOLD kÕ tiÕp theo theo giíi tÝnh (tõ b¶ng Name+ID)
function _next_gold_by_sex()
    local TAB, taskKey = _get_table_by_sex()
    local pos = tonumber(GetTask(taskKey)) or 0   -- 0..4
    if pos < 0 then pos = 0 end

    local n = _len(TAB)
    local idx = pos + 1
    if idx > n then
        return 0, 1, ""
    end
    return TAB[idx].id, 0, TAB[idx].name
end

-- Tang tiÕn ®é sau khi nhËn 1 món GOLD
function _advance_progress()
    local _, taskKey = _get_table_by_sex()
    local pos = tonumber(GetTask(taskKey)) or 0
    if pos < 0 then pos = 0 end
    SetTask(taskKey, pos + 1)
end

-- rateGold: % cç g¨ng ph¸t GOLD (0..100). Kh«ng truyÒn -> 100%.
function nhanthuong(rateGold)
    if _check_bag_1x1() == 0 then return end

    local rate = tonumber(rateGold) or 100
    if rate < 0 then rate = 0 end
    if rate > 100 then rate = 100 end

    local goldId, doneAll, goldName = _next_gold_by_sex()

    -- Roll tû lÖ
    local roll = 1
    if type(random) == "function" then
        roll = random(1,100)
    elseif type(math)=="table" and type(math.random)=="function" then
        roll = math.random(1,100)
    end

    if doneAll == 0 and roll <= rate and goldId ~= 0 then
        -- GOLD CHæ DïNG API AddGoldItem(id), kh«ng cÇn DAYS
        if type(AddGoldItem) == "function" then
            AddGoldItem(goldId)
        else
            Msg2Player("API AddGoldItem kh«ng kh? dông!")
            return
        end

        _advance_progress()

        if goldName ~= "" then
            Msg2Player("Ng­¬i nhËn <color=yellow>"..goldName.."<color>.")
        else
            Msg2Player("Ng­¬i nhËn <color=yellow>Trang bÞ Hoµng Kim<color>.")
        end

        if type(logPhanThuong) == "function" then
            local acc = (type(GetAccount)=="function") and GetAccount() or ""
            local nm  = (type(GetName)=="function") and GetName() or ""
            logPhanThuong(acc.."\t"..nm.."\tNhanThuong -> GOLD "..tostring(goldId))
        end
        return
    end

    -- Tr­ît GOLD hoÆc hÕt set -> quµ dï phßng
    if type(_give_fallback_material) == "function" then
        _give_fallback_material()
    end
end

