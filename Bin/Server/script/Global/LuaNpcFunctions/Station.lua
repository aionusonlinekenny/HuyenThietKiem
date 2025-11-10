-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Xa phu;
-- Include("\\script\\missions\\liendau\\single\\medium\\head.lua")
Include("\\script\\system_config.lua")
Include("\\script\\global\\luanpcfunctions\\datau.lua")
Station = {};	
TAB_CITY = {
--	stt		ID		Name
	{1,		11,		"Thµnh §« Phñ"},		--Thanh Do
	{2,		176,	"L©m An Phñ"},			--Lam An
	{3,		162,	"§¹i Lý Phñ"},			--Dai Ly
	{4,		80,		"D­¬ng Ch©u Phñ"},		--Duong Chau
	{5,		37,		"BiÖn Kinh Phñ"},		--Bien Kinh
	{6,		1,		"Ph­îng T­êng Phñ"},	--Phuong Tuong
	{7,		78,		"T­¬ng D­¬ng Phñ"},		--Tuong Duong
	{8,		53,		"Ba L¨ng HuyÖn "},		--Ba lang Huyen;
	{9,		100,	"Chu Tiªn TrÊn"},		--Chu Tien Tran;
	{10,	101,	"§¹o H­¬ng Th«n"},		--Dao Huong Thon;
	{11,	99,		"VÜnh L¹c TrÊn"}, 		--Vinh Lac Tran;
	{12,	121,	"Long M«n TrÊn"}, 		--Long Mon Tran;
	{13,	153,	"Th¹ch Th¹ch Cæ TrÊn"}, --Thch Co Tran;
	{14,	174,	"Long TuyÒn Th«n"}, 	--Long Tuyen Thon;
	{15,	20,		"Giang T©n Th«n"}, 		--Giang Tan Thon;
	{16,	54,		"Nam Nh¹c TrÊn"}, 		--Nam Nhac Tran;
	};
	
TAB_PRICE = {
	{200,300,300,400,300,100,500},
	{500,500,500,500,500,300,200,200},
	{500,300,500,400,500,500,200,100},
	{400,400,500,300,200,200,100},
	{300,300,500,200,300,500,100},
	{200,500,300,300,400,500,200,100},
	{300,300,400,500,200,200,300,200,100},
	{200,200,100},
	{100},
	{100,100},
	{100},
	{100},
	{100},
	{200},
	{100,200,200},
	{100},
};

	
function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/Station.lua");
		Msg2Player("Reload this script");
	end;
if (ActionOpen() <= 0) then
	Talk(1,"","Vïng ®Êt nµy hiÖn t¹i ch­a khai th«ng, c¸c h¹ vui lßng ®îi m¸y chñ <color=orange>Vâ L©m §å Xanh<color> <color=red>Open Beta<color>");
	return
end;
Station:Station(10356);
end;

function Station:Station(nMsgId)
	local tbSay = {
		"Nh÷ng n¬i ®· ®i qua /RememberPlace",
		"Nh÷ng thµnh thÞ ®· ®i qua /ChooseCityMaps",
		"Trë l¹i ®iÓm cò /TownPortalFun",
		"§­a ta ®Õn Hoa s¬n c¶nh kü tr­êng /MoveToHoaSon",
		"§­a ta ®Õn ®¶o TÈy Tñy /GotoClearSkillMap",
		"§­a ta ®Õn b¶n ®å luyÖn c«ng /SelectTrainning",
		"§­a ta ®Õn n¬i lµm nhiÖm vô D· tÈu/Datautalk",
		--"§­a ta ®Õn b¶n ®å TÝch t×nh ®éng/MoveTichTinhDong",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
		Say(nMsgId,getn(tbSay),tbSay);
end;

function ChooseCityMaps()
	local w,x,y = GetWorldPos();
	local WAYPOINT={};
	local nIndex = 0;
	if (w == 11) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[1][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[1][2].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[1][3].." l­îng]".."/MoveToBienKinh");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[1][4].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[1][5].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[1][6].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[15][3].."["..TAB_PRICE[1][7].." l­îng]".."/MoveToGiangTanThon");		
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
		Say(10356,getn(WAYPOINT),WAYPOINT);
	elseif (w == 176) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[2][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[2][2].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[2][3].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[2][4].." l­îng]".."/MoveToBienKinh");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[2][5].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[2][6].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[14][3].."["..TAB_PRICE[2][7].." l­îng]".."/MoveToLongTuyenThon");		
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
		Say(10356,getn(WAYPOINT),WAYPOINT);
	elseif (w == 162) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[3][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[3][2].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[3][3].." l­îng]".."/MoveToBienKinh");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[3][4].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[3][5].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[3][6].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[15][3].."["..TAB_PRICE[3][7].." l­îng]".."/MoveToGiangTanThon");		
		tinsert (WAYPOINT,TAB_CITY[13][3].."["..TAB_PRICE[3][8].." l­îng]".."/MoveToThachCoTran");		
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
		Say(10356,getn(WAYPOINT),WAYPOINT);
	elseif (w == 80) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[4][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[4][2].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[4][3].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[4][4].." l­îng]".."/MoveToBienKinh");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[4][5].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[4][6].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[10][3].."["..TAB_PRICE[4][7].." l­îng]".."/MoveToDaoHuongThon");			
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
		Say(10356,getn(WAYPOINT),WAYPOINT);
	elseif (w == 37) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[5][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[5][2].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[5][3].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[5][4].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[5][5].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[5][6].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[9][3].."["..TAB_PRICE[5][7].." l­îng]".."/MoveToChuTienTran");			
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
		Say(10356,getn(WAYPOINT),WAYPOINT);	
	elseif (w == 1) then
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[6][1].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[6][2].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[6][3].." l­îng]".."/MoveToBienKinh");	
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[6][4].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[6][5].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[6][6].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[12][3].."["..TAB_PRICE[6][7].." l­îng]".."/MoveToLongMonTran");			
		tinsert (WAYPOINT,TAB_CITY[11][3].."["..TAB_PRICE[6][8].." l­îng]".."/MoveToVinhLacTran");		
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 78) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[7][1].." l­îng]".."/MoveToPhuongTuong");	
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[7][2].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[7][3].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[7][4].." l­îng]".."/MoveToBienKinh");
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[7][6].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[7][7].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,TAB_CITY[8][3].."["..TAB_PRICE[7][8].." l­îng]".."/MoveToBaLangHuyen");			
		tinsert (WAYPOINT,TAB_CITY[10][3].."["..TAB_PRICE[7][9].." l­îng]".."/MoveToDaoHuongThon");		
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 53) then
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[8][1].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[15][3].."["..TAB_PRICE[8][2].." l­îng]".."/MoveToGiangTanThon");	
		tinsert (WAYPOINT,TAB_CITY[16][3].."["..TAB_PRICE[8][3].." l­îng]".."/MoveToNamNhacTran");				
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 100) then
		tinsert (WAYPOINT,TAB_CITY[5][3].."["..TAB_PRICE[9][1].." l­îng]".."/MoveToBienKinh");					
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");		
	elseif (w == 101) then
		tinsert (WAYPOINT,TAB_CITY[7][3].."["..TAB_PRICE[10][1].." l­îng]".."/MoveToTuongDuong");	
		tinsert (WAYPOINT,TAB_CITY[4][3].."["..TAB_PRICE[10][2].." l­îng]".."/MoveToDuongChau");	
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 99) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[11][1].." l­îng]".."/MoveToPhuongTuong");
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 121) then
		tinsert (WAYPOINT,TAB_CITY[6][3].."["..TAB_PRICE[12][1].." l­îng]".."/MoveToPhuongTuong");
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 153) then
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[13][1].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 174) then
		tinsert (WAYPOINT,TAB_CITY[2][3].."["..TAB_PRICE[14][1].." l­îng]".."/MoveToLamAn");	
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 20) then
		tinsert (WAYPOINT,TAB_CITY[1][3].."["..TAB_PRICE[15][1].." l­îng]".."/MoveToThanhDo");	
		tinsert (WAYPOINT,TAB_CITY[3][3].."["..TAB_PRICE[15][2].." l­îng]".."/MoveToDaiLy");	
		tinsert (WAYPOINT,TAB_CITY[8][3].."["..TAB_PRICE[15][3].." l­îng]".."/MoveToBaLangHuyen");
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	elseif (w == 54) then
		tinsert (WAYPOINT,TAB_CITY[8][3].."["..TAB_PRICE[15][3].." l­îng]".."/MoveToBaLangHuyen");
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	else
		tinsert (WAYPOINT,TAB_CITY[8][3].."["..TAB_PRICE[15][3].." l­îng]".."/MoveToBaLangHuyen");
		tinsert (WAYPOINT,"Ta chØ ghÐ ngang qua./OnCancel");	
	end
	Say(10356,getn(WAYPOINT),WAYPOINT);	
end;
--/////////////////////////MOVE TO CITY
function MoveToThanhDo()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(11);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 176) then
		nPrice = TAB_PRICE[2][2];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][2];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][2];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][2];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][1] ;
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][2] ;
	elseif (w == 20) then
		nPrice = TAB_PRICE[15][1];
	end
	TAB_POS = {
		{3193, 5192},
		{3266, 5004},
		{3011, 5101},
		{3031, 4969},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(11,TAB_POS[nPos][1],TAB_POS[nPos][2]);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[1][3].." ");
end

function MoveToPhuongTuong()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(1);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][1];
	elseif (w == 176) then
		nPrice = TAB_PRICE[2][1];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][1];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][1];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][1];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][1] ;
	elseif (w == 99) then
		nPrice = TAB_PRICE[11][1];
	elseif (w == 121) then
		nPrice = TAB_PRICE[12][1];
	end
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
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[6][3].." ");
end

function MoveToLamAn()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(176);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][6];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][6];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][6];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][6];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][6];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][7];
	elseif (w == 174) then
		nPrice = TAB_PRICE[14][1];
	end
	TAB_POS = {
		{1603, 2917},
		{1692, 3296},
		{1375, 3337},
		{1356, 3017},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(176,TAB_POS[nPos][1],TAB_POS[nPos][2]);
	Pay(nPrice) 
	SetFightState(0)
    AddSkillState(963,1,0,18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[2][3].." ");
end

function MoveToDaiLy()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(162);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][2];
	elseif (w == 176) then
		nPrice = TAB_PRICE[2][3];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][3];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][3];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][2];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][3];
	elseif (w == 153) then
		nPrice = TAB_PRICE[13][1];
	elseif (w == 20) then
		nPrice = TAB_PRICE[15][2];
	end
	TAB_POS = {
		{1669, 3129},
		{1696, 3280},
		{1472, 3273},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(162,TAB_POS[nPos][1],TAB_POS[nPos][2]);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[3][3].." ");
end

function MoveToDuongChau()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(80);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][5];
	elseif (w == 176) then
		nPrice = TAB_PRICE[2][6];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][5];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][5];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][5];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][6];
	elseif (w == 101) then
		nPrice = TAB_PRICE[10][2];
	end
	TAB_POS = {
		{1670, 2996},
		{1598, 3201},
		{1722, 3210},
		{1834, 3063},
	};
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	nPos = random(1,getn(TAB_POS));
	NewWorld(80,TAB_POS[nPos][1],TAB_POS[nPos][2]);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[4][3].." ");
end

function MoveToBienKinh()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(37);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][3];
	elseif (w == 176) then
		nPrice = TAB_PRICE[2][4];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][3];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][4];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][5];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][3];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][4];
	elseif (w == 100) then
		nPrice = TAB_PRICE[9][1];
	end
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
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[5][3].." ");
end

function MoveToTuongDuong()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(78);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][4];
	elseif (w == 176) then
		nPrice = TAB_PRICE[2][5];
	elseif (w == 162) then
		nPrice = TAB_PRICE[3][4];
	elseif (w == 80) then
		nPrice = TAB_PRICE[4][5];
	elseif (w == 37) then
		nPrice = TAB_PRICE[5][4];
	elseif (w == 1) then
		nPrice = TAB_PRICE[6][4];
	elseif (w == 78) then
		nPrice = TAB_PRICE[7][5];
	elseif (w == 53) then
		nPrice = TAB_PRICE[8][1];
	elseif (w == 101) then
		nPrice = TAB_PRICE[10][1];
	end
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
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[7][3].." ");
end
--/////////////////////////MOVE TO COUNTRY
function MoveToBaLangHuyen()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(53);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 78) then
	  nPrice = TAB_PRICE[7][8]
	elseif (w == 20) then
	  nPrice = TAB_PRICE[15][3] 
	elseif (w == 54) then
	  nPrice = TAB_PRICE[8][3]
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(53,1582, 3237);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[8][3].." ");
end

function MoveToChuTienTran()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(100);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 37 ) then
		nPrice = TAB_PRICE[5][7];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(100,1615, 3100);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[9][3].." ");
end

function MoveToDaoHuongThon()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(101);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 80) then
	  nPrice = TAB_PRICE[4][7]
	elseif (w == 78) then
	  nPrice = TAB_PRICE[7][9]
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(101,1621, 3104);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[10][3].." ");
end

function MoveToVinhLacTran()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(99);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 1 ) then
		nPrice = TAB_PRICE[6][8];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(99,1606, 3165);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[11][3].." ");
end

function MoveToLongMonTran()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(121);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 1 ) then
		nPrice = TAB_PRICE[6][7];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(121,1924, 4435);	
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[12][3].." ");
end

function MoveToThachCoTran()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(153);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 162 ) then
		nPrice = TAB_PRICE[3][8];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(153,1632, 3182);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[13][3].." ");
end

function MoveToLongTuyenThon()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(174);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 176 ) then
		nPrice = TAB_PRICE[2][7];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(174,1632, 3199);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[14][3].." ");
end

function MoveToGiangTanThon()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(20);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 11 ) then
		nPrice = TAB_PRICE[1][7];
	elseif (w == 162 ) then
		nPrice = TAB_PRICE[3][7];
	elseif (w == 53 ) then
		nPrice = TAB_PRICE[8][2];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(20,3450, 6106);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[15][3].." ");
end

function MoveToNamNhacTran()
	local nPrice = 0;
	local nSubWorldId = SubWorldID2Idx(54);
	local w,x,y = GetWorldPos();
	--if nSubWorldId < 0 then	--chua mo map, ngung ham`
	--return end
	if (w == 53 ) then
		nPrice = TAB_PRICE[8][3];
	end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(54,1588, 3098);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i "..TAB_CITY[16][3].." ");
end

----------------------------
--Ban do luyen cong
----------------------------
tbTrainArea = {
	--20
	[20] = {
	-- {"Hoa S¬n.","MoveToHoaSon", 2, 2605, 3592},
	{"KiÕm C¸c T©y B¾c.","MoveToKiemCacTayBac", 3, 1159, 3715},
	{"Vò L¨ng s¬n.","MoveToVuLangSon", 70, 1608, 3230},
	},
	--30
	[30] = {
	{"Kim Quang ®éng.","MoveToKimQuangDong", 4, 1596, 3282},
	{"Miªu LÜnh.","MoveToMieuLinh", 74, 2040, 3259},
	{"Phôc Ng­u S¬n §«ng.","MoveToPhucNguuSonDong", 90, 1649, 3567},
	},
	--40
	[40] = {	
	{"Táa V©n ®éng.","MoveToToaVanDong", 6, 1660, 3314},
	{"Phôc Ng­u S¬n T©y.","MoveToPhucNguuSonTay", 41, 2056, 2816},
	{"Mª cung Kª Qu¸n ®éng.","MoveToMeCungKeQuanDong", 91, 1469, 2870},
	},
	--50
	[50] = {
	{"ThiÕt th¸p tÇng 1.","MoveToThietThap1", 38, 1603, 3204},
	{"ThiÕt th¸p tÇng 2.","MoveToThietThap2", 39, 1681, 3129},
	{"Thiªn T©m ®éng.","MoveToThienTamDong", 42, 1584, 3221},
	},
	--60
	[60] = {	
	{"MËt ®¹o Nha m«n T­¬ng D­¬ng.","MoveToMatDaoNhaMon", 79, 1594, 3211},
	{"Mª cung cÊm ®Þa.","MoveToCamDia", 158, 1583, 3192},
	{"Thiªn TÇm th¸p tÇng 3.","MoveToThienTamThap3", 166, 1667, 3244},
	},
	--70
	[70] = {
	{"§¹i Tï ®éng.","MoveToDaiTuDong", 72, 1583, 3042},
	{"D­îc V­¬ng ®éng tÇng 2-1.","MoveToDuocVuongDong21", 142, 1467, 3284},
	{"D­îc V­¬ng ®éng tÇng 2-2.","MoveToDuocVuongDong22", 142, 1515, 3340},
	{"L©m Du Quan.","MoveToLamDuQuang", 319, 1612, 3612},
	},
	--80
	[80] = {
	{"Kho¶ Lang ®éng.","MoveToKhoaLangDong", 75, 1872, 3071},
	-- {"Phï Dung ®éng.","MoveToPhuDungDong", 202, 1786, 2823},
	{"Sa m¹c ®Þa biÓu.","MoveToSaMacDiaBieu", 224, 1619, 3213},
	{"Ch©n nói Tr­êng B¹ch.","MoveToChanNuiTruongBach", 320, 1137, 3151},
	},
	--90
	[90] = {
	{"Sa M¹c s¬n ®éng 1.","MoveToSaMac1", 225, 1579, 3188},
	{"Sa M¹c s¬n ®éng 2.","MoveToSaMac2", 226, 1692, 3247},
	{"Sa M¹c s¬n ®éng 3.","MoveToSaMac3", 227, 1583, 3241},
	{"Tr­êng B¹ch s¬n Nam.","MoveToTruongBachNam", 321, 966, 2296},
	{"Tr­êng B¹ch s¬n B¾c.","MoveToTruongBachBac", 322, 2048,4120},
	{"Phong L¨ng ®é.","MoveToPhongLangDo", 336, 1112, 3189},
	{"M¹c Cao QuËt.","MoveToMacCaoQuat", 340, 1853, 3446},
	},
}

function SelectTrainning()
	local tbSay = {
			"Khu vùc cÊp 20 - 30./TrainningLevel",
			"Khu vùc cÊp 30 - 40./TrainningLevel",
			"Khu vùc cÊp 40 - 50./TrainningLevel",
			"Khu vùc cÊp 50 - 60./TrainningLevel",
			"Khu vùc cÊp 60 - 70./TrainningLevel",
			"Khu vùc cÊp 70 - 80./TrainningLevel",
			"Khu vùc cÊp 80 - 90./TrainningLevel",
			"Khu vùc cÊp 90 trë lªn./TrainningLevel",
			"Ta chØ ghÐ ngang qua./OnCancel"	
		}
	if(GetSex() == 0) then
		Say(10372,getn(tbSay),tbSay)
	else
		Say(10373,getn(tbSay),tbSay)
	end
end

function TrainningLevel(nSel)
	if(type(nSel) ~= "number") then
		nSel = tonumber(nSel)
	end
	nSel = (nSel + 2) * 10
	if(not tbTrainArea[nSel]) then
		return
	end
	
	if(GetLevel() < nSel) then
		Talk(1,"","Thø lçi ta kh«ng thÓ ®­a ng­¬i ®Õn ®©y.Vïng ®Êt nµy rÊt nguy hiÓm, ¸c tÆc hoµnh hµnh, b¶n lÜnh nhµ ng­¬i ch­a ®ñ ®Ó ®i ®Õn ®©y.CÇn ®¹t ®¼ng cÊp <color=yellow>"..nSel.."<color> . ")
		return
	end
	
	local tbSay = {}
	for i = 1, getn(tbTrainArea[nSel]) do
		tinsert(tbSay, tbTrainArea[nSel][i][1].."/"..tbTrainArea[nSel][i][2].."")
	end
	tinsert(tbSay, "Ta chØ ghÐ ngang qua./OnCancel"	)
	if(GetSex() == 0) then
		Say(10372,getn(tbSay),tbSay)
	else
		Say(10373,getn(tbSay),tbSay)
	end
end

function MoveToHoaSon()
	if (Condition() < 1) then
		return end;
	if (NewWorld(2, 2605, 3592) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Hoa S¬n C¶nh Kû Tr­êng.");
end

function MoveToKiemCacTayBac()
	if (Condition() < 1) then
		return end;
	if (NewWorld(3, 1159, 3715) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i KiÕm C¸c T©y B¾c.");
end

function MoveToVuLangSon()
	if (Condition() < 1) then
		return end;
	if (NewWorld(70, 1608, 3230) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Vò L¨ng s¬n.");
end

function MoveToKimQuangDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(4, 1596, 3282) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Kim Quang ®éng.");
end

function MoveToMieuLinh()
	if (Condition() < 1) then
		return end;
	if (NewWorld(74, 2040, 3259) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Miªu LÜnh.");
end

function MoveToPhucNguuSonDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(90, 1649, 3567) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Phôc Ng­u S¬n §«ng.");
end

function MoveToToaVanDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(6, 1660, 3314) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Táa V©n ®éng.");
end

function MoveToPhucNguuSonTay()
	if (Condition() < 1) then
		return end;
	if (NewWorld(41, 2056, 2816) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Phôc Ng­u S¬n T©y.");
end

function MoveToMeCungKeQuanDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(91, 1469, 2870) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Mª cung Kª Qu¸n ®éng.");
end

function MoveToThietThap1()
	if (Condition() < 1) then
		return end;
	if (NewWorld(38, 1603, 3204) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Mª cung ThiÕt th¸p tÇng 1.");
end

function MoveToThietThap2()
	if (Condition() < 1) then
		return end;
	if (NewWorld(39, 1681, 3129) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Mª cung ThiÕt th¸p tÇng 2.");
end

function MoveToThienTamDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(42, 1584, 3221) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Mª cung Thiªn T©m ®éng.");
end

function MoveToMatDaoNhaMon()
	if (Condition() < 1) then
		return end;
	if (NewWorld(79, 1594, 3211) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i MËt ®¹o Nha m«n T­¬ng D­¬ng.");
end

function MoveToCamDia()
	if (Condition() < 1) then
		return end;
	if (NewWorld(158, 1583, 3192) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Mª cung cÊm ®Þa.");
end

function MoveToThienTamThap3()
	if (Condition() < 1) then
		return end;
	if (NewWorld(166, 1667, 3244) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Thiªn TÇm th¸p tÇng 3.");
end

function MoveToDaiTuDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(72, 1583, 3042) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Thiªn §¹i Tï ®éng.");
end

function MoveToDuocVuongDong21()
	if (Condition() < 1) then
		return end;
	if (NewWorld(142, 1467, 3284) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i D­îc V­¬ng ®éng tÇng 2-1.");
end

function MoveToDuocVuongDong22()
	if (Condition() < 1) then
		return end;
	if (NewWorld(142, 1515, 3340) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i D­îc V­¬ng ®éng tÇng 2-2.");
end

function MoveToLamDuQuang()
	if (Condition() < 1) then
		return end;
	if (NewWorld(319, 1612, 3612) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i L©m Du Quan.");
end

function MoveToKhoaLangDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(75, 1872, 3071) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Kho¶ Lang ®éng.");
end

function MoveToPhuDungDong()
	if (Condition() < 1) then
		return end;
	if (NewWorld(202, 1786, 2823) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Phï Dung ®éng.");
end

function MoveToSaMacDiaBieu()
	if (Condition() < 1) then
		return end;
	if (NewWorld(224, 1619, 3213) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Sa m¹c ®Þa biÓu.");
end

function MoveToChanNuiTruongBach()
	if (Condition() < 1) then
		return end;
	if (NewWorld(320, 1137, 3151) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Ch©n nói Tr­êng B¹ch.");
end

function MoveToSaMac1()
	if (Condition() < 1) then
		return end;
	if (NewWorld(225, 1579, 3188) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	AddTermini(195);
	Msg2Player("Ngåi yªn! Chóng ta ®i Sa M¹c S¬n §éng 1.");
end

function MoveToSaMac2()
	if (Condition() < 1) then
		return end;
	if (NewWorld(226, 1692, 3247) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Sa M¹c s¬n ®éng 2.");
end

function MoveToSaMac3()
	if (Condition() < 1) then
		return end;
	if (NewWorld(227, 1583, 3241) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Sa M¹c s¬n ®éng 3.");
end

function MoveToTruongBachNam()
	if (Condition() < 1) then
		return end;
	if (NewWorld(321, 966, 2296) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Tr­êng B¹ch s¬n Nam.");
end

function MoveToTruongBachBac()
	if (Condition() < 1) then
		return end;
	if (NewWorld(322, 2048,4120) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Tr­êng B¹ch s¬n B¾c.");
end

function MoveToPhongLangDo()
	if (Condition() < 1) then
		return end;
	if (NewWorld(336, 1112, 3189) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i Phong L¨ng ®é.");
end

function MoveToMacCaoQuat()
	if (Condition() < 1) then
		return end;
	if (NewWorld(340, 1853, 3446) == 0)then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng");
		return
	end
	SetFightState(1);
	AddSkillState(963, 1, 0, 18*3)
	Msg2Player("Ngåi yªn! Chóng ta ®i M¹c Cao QuËt.");
end

function Condition()
	if (GetLevel() <= 30) then
		nPrice = 200;
	elseif (GetLevel() > 30) and (GetLevel() <= 60) then
		nPrice = 300;
	elseif (GetLevel() > 60) and  (GetLevel() <= 80) then
		nPrice = 400;
	elseif (GetLevel() > 80) and  (GetLevel() <= 120) then
		nPrice = 500;
	else
		nPrice = 1000;
	end
	
	if (GetCash() < nPrice) then
		Talk(1,"","§¹i gia cÇn cã "..nPrice.." l­îng lé phÝ ®i ®­êng vµ b¶o tiªu hµnh lý c¸ nh©n cho ®¹i hiÖp!");
		return 0;
	end
	Pay(nPrice);
	return 1;
end	

----------------------------
--Quay lai diem cu
----------------------------
function TownPortalFun()
	 ReturnFromPortal();
end
----------------------------
--
----------------------------
function RememberPlace()
	p1 = GetWayPoint(1);
	p2 = GetWayPoint(2);
	p3 = GetWayPoint(3);
	WAYPOINT={};
	local i = 1;
	if (p1 ~= 0)  then
	name = GetWayPointName(p1);
	name = name .."/SelWayPoint";
	WAYPOINT[i]= name;
	i = i + 1;
	end;
	
	if (p2 ~= 0)  then
	name = GetWayPointName(p2);
	name = name .."/SelWayPoint";
	WAYPOINT[i]= name;
	i = i + 1;
	end;
	
	if (p3 ~= 0)  then
	name = GetWayPointName(p3);
	name = name .."/SelWayPoint";
	WAYPOINT[i]= name;
	i = i + 1;
	end;
	
	WAYPOINT[i] = "Kh«ng ®i ®©u c¶ /OnCancel";
	if (i == 1) then 
	Talk(1,"","Ng­¬i ch­a ®i ®©u c¶, kh«ng cã ®Þa ®iÓm nµo ®· ®i qua!");
	else
	Say("H·y lùa chän:", i, WAYPOINT);
	end;	
end

function SelWayPoint(nSel)
	Msg2Player("nSel "..nSel.."");
	local nWayPoint = GetWayPoint(nSel + 1);
	if (nWayPoint ~= 0) then
	local nW, nX, nY = GetWayPointPos(nWayPoint)
	nFightState = GetTerminiFState(nWayPoint)
	nResult = NewWorld(nW, nX, nY);
	if (nResult == 1) then
	SetFightState(nFightState)
	end
	end
end;
----------------------------
--
----------------------------
--function MoveToHoaSon()
--NewWorld(2, 2605, 3592);
--SetFightState(1);
--SetLogoutRV(0);
--end;
----------------------------
--
----------------------------
function MoveTichTinhDong()
	local szHello = "<color=orange>Xa phu<color>: N¬i nµy khi ®i ®­îc sñ dông tói m¸u, nh­ng kh«ng thÓ thæ ®Þa phï. Mét khi tö trËn sÏ mÊt lÖnh bµi!";
	local szSay = {
		"Ta cã lÖnh bµi, ®­a ta ®i/ExMoveTichTinhDong",
		"Ta chØ ®Õn ®Ó t×m hiÓu/OnCancel",
	};
	Say(szHello,getn(szSay),szSay);
end;

function ExMoveTichTinhDong()
	if (GetTaskItemCount(26) == 0 ) then
		Talk(1,"","Cao thñ thÞ néi nhiÒu ë ®©y thÕ trËn c¨ng th¼ng, mét khi ng­¬i tö trËn kh«ng thÓ quay l¹i ®­îc. Ta cÇn lÖnh bµi TÝch t×nh ®éng.");
		return
	end;
	
	if (GetLevel() < 80) then
		Talk(1,"","§¹t ®¼ng cÊp 80 h·y quay l¹i t×m ta, n¬i nµy kh«ng dµnh cho nh÷ng cao thñ b¶n lÜnh trªn giang hå!");
		return
	end;
	-- NewWorld(342, 1162, 2392);
	NewWorld(341, 1278, 2497)
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(341);
	SetRevPos(1);
	SetFightState(0);
	LeaveTeam();
	SetPunish(1);
	-- ForbidTownPortal(1);
	DelTaskItem(26);
	SetDeathScript("\\script\\event\\tichtinhdong\\player_death.lua");
	SubWorld = nOldSubWorld;
end;
----------------------------
--TAY TUY
----------------------------
szClearSkill = {
	"ChØ tÈy tiÒm n¨ng[6 Tinh hång b¶o th¹ch]./GotoClearSkillMap_2",
	"ChØ tÈy kü n¨ng[1 bé thñy tinh]./GotoClearSkillMap_2",
	"TÈy tiÒm n¨ng vµ kü n¨ng[6 Tinh hång b¶o th¹ch + 1 bé thñy tinh]./GotoClearSkillMap_2",
	"§Ó ta suy nghÜ l¹i./OnCancel"
}

----------------------------
--
----------------------------
function GotoClearSkillMap()
	local nLevel = GetLevel()
	if(GetLevel() < 50) then
		Talk(1,"", "§¼ng cÊp ch­a ®¹t <color=red>50<color>, kh«ng thÓ lªn ®¶ tÈy tñy.")
		return
	end
	if(GetCamp() == 0) then
		Talk(1,"", "Ng­¬i ch­a gia nhËp m«n ph¸i, kh«ng thÓ lªn ®¶o tÈy tñy.")
		return
	end

	 local n = GetTask(0)
	 if(GetByte(n,4) == 0) then
		 Say("<color=gold>Xa Phu<color>: ChØ cã <color=red>c¬ héi 1 lÇn<color> lªn ®¶o tÈy tñy miÔn phÝ. Ng­¬i ®· suy nghÜ kü ch­a?",2, "Lªn ®¶o tÈy tñy./GotoClearFree", "§Ó ta suy nghÜ l¹i./OnCancel")
		 return
	 end
	 Say("<color=gold>Xa Phu<color>: Sè lÇn tÈy tñy miÔn phÝ ®· hÕt, ng­¬i cã thÓ sö dông <color=green>Thñy Tinh<color> hoÆc <color=green>Tinh Hång B¶o Th¹ch<color> ®Ó lªn ®¶o?",getn(szClearSkill),szClearSkill)
end

----------------------------
--
----------------------------
function GotoClearSkillMap_2(nSel)
	nSel = tonumber(nSel) + 1
	if(nSel == 1) then
		GotoClearProp(0)
	elseif(nSel == 2) then
		GotoClearSkill(0)
	elseif(nSel == 3) then
		GotoClearAll()
	end
end

----------------------------
--
----------------------------
function GotoClearFree()
	SetTask(0,SetByte(n,4,4))
	GotoClearFreeCore()
end

----------------------------
--
----------------------------
function GotoClearFreeCore()
	--ForbidTownPortal(1)
	SetFightState(0)
	SetPunish(0)
	SetLogoutRV(1)
	SetRevPos(242,1)
	NewWorld(242,1613,3197)	
	Talk(1,"","§· ®Õn ®¶o tÈy tñy! H·y tËn dông c¬ héi ®· th«ng kinh m¹ch cÈn thËn nhÐ.")
end

----------------------------
--
----------------------------
function GotoClearProp(bCheck)
	local nCount = GetItemCountInBag(6,15,1,-1,1)
	if(nCount < 6) then
		Talk(1,"","Ng­¬i kh«ng mang ®ñ <color=red>6 viªn<color> <color=green>Tinh Hång B¶o Th¹ch<color>. Kh«ng nªn nãng lßng, t×m ®ñ råi h·y trë l¹i.")
		return 0
	end
	
	if(bCheck == 2) then
		return 1
	end
	
	local i
	for i = 1, 6 do
		DelTaskItem(15,1)
	end
	
	if(bCheck == 3) then
		return 2
	end
	local n = GetTask(0)
	SetTask(0,SetByte(n,4,3))
	GotoClearFreeCore()
end

----------------------------
--
----------------------------
function GotoClearSkill(bCheck)
	x = GetItemCountInBag(6,16,1,-1,1)
	y = GetItemCountInBag(6,17,1,-1,1)
	z = GetItemCountInBag(6,18,1,-1,1)
	
	if( (x == 0) or (y == 0) or (z == 0) ) then
		Talk(1,"","Ng­¬i kh«ng mang ®ñ <color=red>3 viªn<color> <color=green>Thñy Tinh<color>. §õng nãng lßng, t×m ®ñ h·y quay l¹i.")
		return 0
	end
	
	if(bCheck == 2) then
		return 1
	end
	
	DelTaskItem(16,1)
	DelTaskItem(17,1)
	DelTaskItem(18,1)
	
	if(bCheck == 3) then
		return 2
	end
	
	local n = GetTask(0)
	SetTask(0,SetByte(n,4,2))
	GotoClearFreeCore()
end

----------------------------
--
----------------------------
function GotoClearAll()
	local nCount = GetItemCountInBag(6,15,1,-1,1)
	x = GetItemCountInBag(6,16,1,-1,1)
	y = GetItemCountInBag(6,17,1,-1,1)
	z = GetItemCountInBag(6,18,1,-1,1)

	if((nCount < 6) or (x == 0) or (y == 0) or (z == 0)) then
		Talk(1,"","CÇn 6 tinh hång b¶o th¹ch vµ 1 bé thñy tinh.")
		return 0
	end
	if(GotoClearProp(2) == 0) or (GotoClearSkill(2) == 0) then
		return
	end
	if(GotoClearProp(3) == 0) or (GotoClearSkill(3) == 0) then
		return
	end
	local n = GetTask(0)
	SetTask(0,SetByte(n,4,1))
	GotoClearFree()
end

----------------------------
--
----------------------------

function Datautalk()
	if (GetTask(TASK_KIND) == 1 or GetTask(TASK_KIND) == 2 or GetTask(TASK_KIND) == 3) then 
		if (GetTask(TASK_SAVEQUEST_1) > 0) then 
			for i = 1, getn(TAB_MAP) do
				for j = 1, getn(TAB_MAP[i]) do
				--	Msg2Player( "Show me: "..(TAB_MAP[i][j]).." " )
				--	Msg2Player("Task: "..GetTask(TASK_SAVEQUEST_1).."");
					if (TAB_MAP[i][j] == GetTask(TASK_SAVEQUEST_1)) then
					--	Msg2Player( "Show me: "..(TAB_MAP[i][j]).." " )
						nW, nX, nY = GetWayPointPos(WAYPOINT[i][j]);
					--	Msg2Player("Show me: "..nW.." - "..nX.." - "..nY.." ")
						local szMapName = GetWayPointName(WAYPOINT[i][j]);
					--	Msg2Player("Ngåi yªn ta ®­a ng­¬i ®i "..szMapName.." lµm nhiÖm vô D· TÈu ca ca!.");
						NewWorld(nW,nX,nY);
						SetFightState(1);
						AddSkillState(963, 1, 0, 18*3) -- Anti 3s;
						break;
					end;
				end;
			end;
			
		end;
	else
		return 0;
	end;
end;

----------------------------
--
----------------------------
function OnCancel()
Talk(1,"","Kh¸ch quan ®i thong th¶, nÕu cÇn ta gióp ®ì th× h·y trë l¹i ®©y! ");
end;