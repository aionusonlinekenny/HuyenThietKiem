Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\event\\quockhanh\\head.lua");

function OnUse(nIdx)
    if SYS_SWITCH_QUOC_KHANH == 0 then
	return 1
	end
    if type(FindEmptyPlace)=="function" and FindEmptyPlace(1,1)==0 then
        Talk(1,"","H·y s¾p xÕp hµnh trang, cÇn Ýt nhÊt <color=yellow>1x1<color> « trèng!")
        return 0
    end

    -- BASE list nguyên liệu (genre=6, particular=1, detail, weight)
    local BASE = {
        {"Hoa Quèc Kh¸nh", 6, 64, 10},
        {"GiÊy G·i Hoa",   6, 65, 10},
        {"D©y Buéc Hoa",   6, 66, 10},
        {"Giá §ïng Hoa",   6, 67, 10},
    }

    -- Tính tổng trọng số
    local total = 0
    for i=1,getn(BASE) do
        total = total + (BASE[i][4] or 0)
    end

    -- Random
    local r = (type(random)=="function") and random(1,total) or math.random(1,total)

    local pick, acc = 1, 0
    for i=1,getn(BASE) do
        acc = acc + (BASE[i][4] or 0)
        if r <= acc then pick = i; break end
    end

    local szName = BASE[pick][1]
    local szGr   = BASE[pick][2]
    local szDet  = BASE[pick][3]

    AddTaskItem(szDet, DAYS)
    Msg2Player("NhËn ®­îc 1 <color=yellow>"..szName.."<color>.")
    return 1
end
