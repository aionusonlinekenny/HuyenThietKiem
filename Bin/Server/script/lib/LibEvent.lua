-- Author:	Kinnox;
-- Date:	17-04-2021
-- Functions: Lib Event;
Include("\\script\\system_config.lua");
function EventVuotAi()
	AddScriptItem(128);
	--event
	--for i = 1,10 do
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;
function EventQuocKhanh()
    if SYS_SWITCH_QUOC_KHANH == 0 then
	return
	end
	-- 25 = Year ; 10 = month;  29 = Date ; 19 = Hour ; 30 = Min
    local DAYS = 2510301930
    local POOL = {
        { id = 64, weight = 35 },
        { id = 65, weight = 25 },
        { id = 66, weight = 1 },
        { id = 67, weight = 0  },
    }
    local NONE_WEIGHT = 35
    local total = NONE_WEIGHT
    local i = 1
    while POOL[i] do
        local w = POOL[i].weight or 0
        if w > 0 then total = total + w end
        i = i + 1
    end
    if total <= 0 then return 0 end

    -- Random 1..total
    local roll = random(total)
    if roll <= NONE_WEIGHT then
        return 0
    end
    roll = roll - NONE_WEIGHT
    i = 1
    while POOL[i] do
        local w = POOL[i].weight or 0
        if w > 0 then
            if roll <= w then
                AddTaskItem(POOL[i].id, DAYS)
                return 1
            end
            roll = roll - w
        end
        i = i + 1
    end
    return 0
end
   
function DropVuotAi()
	for i = 1,random(2,3) do
		AddTaskItem(19);
		--event
		--AddScriptItem(135,30) -- hop qua su kien
	end;
end;


function BaoRuongVuotAi()
	local nRandom = random(1,100);
	Earn(50000);
	Msg2Player("<color=green>NhËn ®­îc tiÒn ®ång.<color>");

	if (nRandom <= 50) then
		AddOwnExp(random(1000000,2000000));
		return 1;
	elseif (nRandom > 50 and nRandom <= 80) then
		AddTaskItem(random(35,52));
		Msg2Player("NhËn thµnh c«ng 1 B¾c §Èu ThuÇn M· ThuËt.");
		return 1;
	else
		for i = 1,random(1,2) do
			AddTaskItem(19);
		end;
		return 1;
	end;
	return 0;
end;

function EventTongKim()
	AddScriptItem(129);
	--for i = 1,20 do
		--event
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;

function BonusTongKim()
	AddScriptItem(129);
	--event
	--for i = 1,3 do
	--	AddTaskItem(63) -- la goi
	--end;
	AddOwnExp(20000000);
end;

function WinTongKim()
 	--event
	--for i = 1,5 do
	--	AddTaskItem(63) -- la goi
	--end;
	AddOwnExp(50000000);
end;

function LoseTongKim()
	--event
	--for i = 1,3 do
	--	AddTaskItem(63) -- la goi
	--end;
	AddOwnExp(30000000);
end;

function BaoRuongTongKim()
	local nRandom = random(1,100);
	if (nRandom <= 50) then
		AddOwnExp(random(1000000,2000000));
		return 1;
	elseif (nRandom > 50 and nRandom <= 70) then
		AddTaskItem(random(29,34));
		Msg2Player("NhËn thµnh c«ng 1 m¶nh trang bÞ hång ¶nh.");
		return 1;
	elseif 	(nRandom > 70 and nRandom <= 90) then
		AddTaskItem(random(35,52));
		Msg2Player("NhËn thµnh c«ng 1 B¾c §Èu ThuÇn M· ThuËt.");
		return 1;
	else
		Earn(100000);
		return 1;
	end;
	nRandom = (random(1,2));
	for i = 1,nRandom do
		AddTaskItem(19);
	end;
	Msg2Player("NhËn ®­îc "..nRandom.." tiÒn ®ång.");
	return 0;
end;

function EventKillerBoss()
	local nRandom = random(53,58);	
	AddTaskItem(nRandom);
	AddOwnExp(3000000);
	Msg2Player("NhËn ®­îc mét Binh ch­¬ng kü n¨ng 120.");
	--event
	--for i = 1,5 do
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;

function EventBossHoangKim(nNpcIndex,nW,nX,nY,PlayerIndex,nSeries)
	AddSumExp(20000000);
	DropItem(nW,nX,nY,PlayerIndex,6,random(29,34),1,1,-1,0,0);
	DropItem(nW,nX,nY,PlayerIndex,6,random(53,58),1,1,-1,0,0);
	local nXu = random(1,3);
	for i = 1,nXu do
		DropItem(nW,nX,nY,PlayerIndex,6,19,1,1,-1,0,0);
	end;
	if (random(1,10) < 6) then
		DropItem(nW,nX,nY,PlayerIndex,7,random(11,13),1,1,-1,0,0);
		DropItem(nW,nX,nY,PlayerIndex,7,random(11,13),1,1,-1,0,0);
	else
		DropItem(nW,nX,nY,PlayerIndex,6,random(16,18),1,1,-1,0,0);
	end;
	DropRateItem(nNpcIndex,5,"\\settings\\droprate\\boss\\bosstask_lev90.ini",1,10,nSeries)
	--event
	--for i = 1,10 do
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;

function AddEventLoanChien()
	-- local nXu = 2;
	AddOwnExp(2000000);
	Earn(10000);
	-- for i = 1,2 do
		-- AddTaskItem(19);
	-- end;
	--event
	--for i = 1,2 do
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;

function EventPhongLangDo(nNpcIndex,nW,nX,nY,PlayerIndex,nSeries)
	local nXu = random(1,2);
	for i = 1,nXu do
		DropItem(nW,nX,nY,PlayerIndex,6,19,1,1,-1,0,0);
	end;
		--event
	--for i = 1,10 do
	--	AddScriptItem(135,30) -- hop qua su kien
	--end;
end;

function AddEventThienBaoKho()
	TAB_REWARD = {
--phan loai theo kind nhiem vu
--[1] money;
--[2] exp;
--[3] item;
	[1] ={{1000,12345},{2000,23456},{3000,34567},{4000,45678},{5000,56789},{5000,678919},{5000,78919},{6000,89919},{6000,99999},{5000,86914}},
	[2] ={{200000,1234560},{300000,2345670},{400000,3456780},{500000,4567890},{600000,5678990},{700000,6789100},{800000,7899100},{900000,8915100},{1000000,9999999},{2000000,9999999},},
	[3] ={
		{"ThiÕt La H¸n",7,1},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},
		{"Long HuyÕt Hoµn",7,23},{"B¸n nh­îc t©m kinh",7,24},{"ThiÕt La H¸n",7,1},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},{"B¸n nh­îc t©m kinh",7,24},
		{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"B¸n nh­îc t©m kinh",7,24},{"ThiÕt La H¸n",7,1},{"B¸n nh­îc t©m kinh",7,24},{"ThiÕt La H¸n",7,1},
		{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"X¸ lîi kim ®¬n (®¹i)",7,30},{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"X¸ lîi kim ®¬n (®¹i)",7,30},
		{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"H¶i Long Ch©u",7,137},{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"X¸ lîi kim ®¬n (®¹i)",7,30},
		{"Phi Phong",7,73},
		{"ThiÕt La H¸n",7,1},{"Tiªn Th¶o Lé",7,3},{"Thiªn S¬n B¶o Lé",7,5},{"QuÕ Hoa Töu",7,6},
		{"Qu¶ Huy Hoµng (cao)",7,9},{"Bµn Nh­îc T©m Kinh",7,24},{"Phóc Duyªn Lé (tiÓu)",7,11},
		{"Phóc Duyªn Lé (trung)",7,12},{"Phóc Duyªn Lé (®¹i)",7,13},{"Danh Väng Lé",7,72},{"Tinh Hång B¶o Th¹ch",6,15},
		{"Lam Thuû Tinh",6,16},{"Lôc Thuû Tinh",6,18},{"Tö Thuû Tinh",6,17},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},
		{"H¶i Long Ch©u",7,137},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"H¶i Long Ch©u",7,137},{"ThiÕt La H¸n",7,1},
		{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"X¸ lîi kim ®¬n (®¹i)",7,30},{"X¸ lîi kim ®¬n (tiÓu)",7,28},{"X¸ lîi kim ®¬n (trung)",7,29},{"X¸ lîi kim ®¬n (®¹i)",7,30},
		{"B¸n nh­îc t©m kinh",7,24},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"ThiÕt La H¸n",7,1},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},
		{"H¶i Long Ch©u",7,137},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},{"H¶i Long Ch©u",7,137},
		{"ThiÕt La H¸n",7,1},{"Tï Vµ TriÖu Håi Boss HK",7,137},{"ThiÕt La H¸n",7,1},{"Long HuyÕt Hoµn",7,23},{"ThiÕt La H¸n",7,1},{"Tï Vµ TriÖu Håi Boss HK",7,137},
	},
}

	local nRandomItem = random(1,getn(TAB_REWARD[3]));
	local szName = TAB_REWARD[3][nRandomItem][1];
	local szGr = TAB_REWARD[3][nRandomItem][2];
	local szDetail = TAB_REWARD[3][nRandomItem][3];
	AddItem(szGr,szDetail,0,0,0,0,0);
end;