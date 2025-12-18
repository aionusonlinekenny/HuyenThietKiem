Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\event\\quockhanh\\head.lua");

--===============Config Event===============--
DangCapSuDung 	= 60
ToiDaSuDung 	= 50000
TenVatPham		= "Giá Hoa Quèc Kh¸nh"
SoKinhNghiem	= {{4000000,4},}
MoKhoaVipSau 	= 20000 

-----------------------------------------------

function OnUse(nIdx)
if (GetTask(T_GIOHOAQUOCKHANH) >= ToiDaSuDung) then 
		Msg2Player("§¹i hiÖp lµ bËc kú tµi trong thiªn h¹, ®· thu thËp vµ sö dông hÕt Giá Hoa Quèc Kh¸nh.");
		return
	end;
	
	nTaskValue = GetTask(T_GIOHOAQUOCKHANH);
	if (GetLevel() < DangCapSuDung) then Talk(1,"","§¼ng cÊp trªn "..DangCapSuDung.." míi cã thÓ sö dông vËt phÈm nµy.") return end
	----------------------------------------------------
	if ( nTaskValue >= ToiDaSuDung ) then Talk(1,"","Ng­¬i ®· dïng <color=red>"..ToiDaSuDung.." c¸i "..TenVatPham.." <color>råi kh«ng thÓ sö dông thªm.") return end
	SetTask(T_GIOHOAQUOCKHANH,GetTask(T_GIOHOAQUOCKHANH)+1);
	----------------------------------------------------
	Msg2Player("<color=white>B¹n sö dông<color=cyan> "..GetTask(T_GIOHOAQUOCKHANH).."/"..ToiDaSuDung.."<color><color=orange> "..TenVatPham.." <color>nhËn ®­îc <color=yellow>"..SoKinhNghiem[1][2].."<color> triÖu ®iÓm kinh nghiÖm.")
	AddOwnExp(SoKinhNghiem[1][1])
	
	if random(1,2) == 1 then
        NhanVatPham()
    else
        Msg2Player("LÇn nµy kh«ng nhËn ®­îc quµ, h·y thö l¹i!")
    end

    return 1
end

function NhanVatPham()
    if type(FindEmptyPlace)=="function" and FindEmptyPlace(1,1)==0 then
        Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn Ýt nhÊt <color=yellow>1x1<color> « trèng!")
        return 0
    end
    local daDung = tonumber(GetTask(T_GIOHOAQUOCKHANH)) or 0

    local BASE = {
        {"Phóc Duyªn Lé (tiÓu)",  7, 11, 15},
        {"Phóc Duyªn Lé (trung)", 7, 12, 15},
        {"Phóc Duyªn Lé (§¹i)",   7, 13, 15},
        {"Tiªn Th¶o Lé",          7,  3, 15},
        {"Thiªn S¬n B¶o Lé",      7,  5, 15},
        {"QuÕ Hoa Töu",           7,  6, 15},
        {"Lam Thuû Tinh",         6, 16,  5},
        {"Lôc Thuû Tinh",         6, 18,  5},
        {"Tö Thuû Tinh",          6, 17,  5},
        {"Tinh Hång B¶o Th¹ch",   6, 15,  5},
        {"ThiÕt La H¸n",          7,  1,  5},
        {"Vâ L©m MËt TÞch",       7,  2,  5},
        {"B¸n nh­îc t©m kinh",    7, 24,  5},
    }

    local VIP = {
        {"¤ V©n §¹p TuyÕt", 10,  6, 1},
        {"Phiªn Vò ",       10,  7, 1},
        {"Phi V©n",         10,  8, 1},
        {"XÝch Long C©u",   10,  9, 1},
    }

    local TAB_REWARD = {}
    local n = getn(BASE)
    for i=1,n do TAB_REWARD[i] = BASE[i] end
    if daDung > MoKhoaVipSau then
        local idx = n
        local m = getn(VIP)
        for i=1,m do
            idx = idx + 1
            TAB_REWARD[idx] = VIP[i]
        end
    end

    local total = 0
    for i=1,getn(TAB_REWARD) do total = total + (TAB_REWARD[i][4] or 0) end

    local r = random(1, total)

    local pick, acc = 1, 0
    for i=1,getn(TAB_REWARD) do
        acc = acc + (TAB_REWARD[i][4] or 0)
        if r <= acc then pick = i; break end
    end

    local szName = TAB_REWARD[pick][1]
    local szGr   = TAB_REWARD[pick][2]
    local szDet  = TAB_REWARD[pick][3]

    AddItem(szGr, szDet, 0, 0, 0, 0, 0)
    Msg2Player("NhËn ®­îc 1 <color=yellow>"..szName.."<color>.")
    return 1
end

