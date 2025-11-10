--Author: Kinnox;
--Date: 14/09/2021;
--Name: Xa Phu Tong Kim;
Include("\\script\\mission\\battles\\head.lua")
function main()
	local szHello = "<color=orange>Xa Phu:<color> Xin chµo <color=gold>["..GetName().."]<color> §­êng ®i ®Õn c¸c thµnh thÞ lín thÕ nµy rÊt xa. Cã khi ph¶i mÊt vµi ngµy ®­êng míi tíi n¬i."
	local TAB_CITY = {
			"Ph­îng T­êng./MoveToPhuongTuong",
			"T­¬ng D­¬ng./MoveToTuongDuong",
			"BiÖn Kinh./MoveToBienKinh",
			"B¸o danh bªn Tèng./JoinSong",
			"B¸o danh bªn Kim./JoinJin",
			"KÕt thóc ®èi tho¹i./func_Exit",
		}
	Say(szHello,getn(TAB_CITY),
		TAB_CITY)
end;

function MoveToPhuongTuong()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(1);
	local w,x,y = GetWorldPos();
	nPrice = 500;
	TAB_POS = {
		{1557, 3112},
		{1537, 3237},
		{1649, 3287},
		{1656, 3167},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(1,TAB_POS[nPos][1],TAB_POS[nPos][2]);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i Ph­îng t­êng phñ ");
	SetFightState(0);
end;

function MoveToBienKinh()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(37);
	local w,x,y = GetWorldPos();
	nPrice = 500;
	TAB_POS = {
		{1598, 3000},
		{1866, 2930},
		{1701, 3224},
		{1636, 3191},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(37,TAB_POS[nPos][1],TAB_POS[nPos][2]);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i BiÖn kinh phñ ");
	SetFightState(0);
end;

function MoveToTuongDuong()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(78);
	local w,x,y = GetWorldPos();
	nPrice = 500;
	TAB_POS = {
		{1592, 3377},
		{1704, 3225},
		{1508, 3147},
		{1440, 3219},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(78,TAB_POS[nPos][1],TAB_POS[nPos][2]);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i T­¬ng d­¬ng phñ ");
		SetFightState(0);
end;

function JoinSong()
	NewWorld(ZBattle.SIGN_MAP, ZBattle.tbSignMapPos[1][1], ZBattle.tbSignMapPos[1][2]);
	SetFightState(0);
end;

function JoinJin()
	NewWorld(ZBattle.SIGN_MAP, ZBattle.tbSignMapPos[2][1], ZBattle.tbSignMapPos[2][2]);
	SetFightState(0);
end;

function func_Exit()
end;