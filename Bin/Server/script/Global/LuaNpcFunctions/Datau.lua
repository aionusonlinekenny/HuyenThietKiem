-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tho ren;
Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");
datau = {};
MAX_TASK = 40;
-- TASK_ACCEPT = 1;
-- TASK_COUNT_QUEST = 2;
-- TASK_CANCEL_QUEST = 3;
-- TASK_KIND = 10;
---TIM DO CHI - MAT CHI
-- TASK_DT_DOCHI = 4;
-- TASK_DT_MATCHI = 5;
---DIEM
-- TASK_DT_PHUCDUYEN = 6;
-- TASK_DT_DANHVONG = 7;
-- TASK_DT_TONGKIM = 8;
-- TASK_DT_LIENDAU = 9;
---DANH QUAI
-- TASK_DT_DANHQUAI = 14;
---
-- TASK_SAVEQUEST_1 = 11;
-- TASK_SAVEQUEST_2 = 12;
-- TASK_SAVEQUEST_3 = 13;
-- TASK_SAVEQUEST_3 = 16;
-- TASK_SAVEQUEST_4 = 17;
-- TASK_SAVEQUEST_5 = 18;
-- TASK_SAVEQUEST_6 = 19;
-- TASK_SAVEQUEST_7 = 20;
-- TASK_SAVEQUEST_8 = 21;
----
MSG_CONDITIONS = "Ng­¬i ch­a lµm ®óng nhiÖm vô ta yªu cÇu h·y xem kü l¹i nhiÖm vô ! ";
TAB_MAP ={
--IDMAPS-RANDOM
--LEVEL 20
[1] = {1,2,3,11,37,78,80,162,176,53,70},
--LEVEL 30
[2] = {4,74,90},
--LEVEL 40
[3] = {6,41,91},
--LEVEL 50
[4] = {38,39,42},
--LEVEL 60
[5] = {79,158,166},
--LEVEL 70
[6] = {72,142,319},
--LEVEL 80
[7] = {75,202,224,320},
--LEVEL 90 >
[8] = {225,226,227,321,322,336,340},
}
-- WAYPOINT the same TAB_MAP
WAYPOINT = {
--LEVEL 20
[1] = {240,1,2,239,238,237,236,235,234,44,47},
--LEVEL 30
[2] = {4,46,150},
--LEVEL 40
[3] = {14,105,114},
--LEVEL 50
[4] = {106,229,108},
--LEVEL 60
[5] = {230,231,232},
--LEVEL 70
[6] = {85,233,196},
--LEVEL 80
[7] = {68,192,195,197},
--LEVEL 90 >
[8] = {226,227,228,198,200,224,225},
}

TAB_PHUCDUYEN = {
[1] = {1,10}, 
[2] = {5,15}, 
[3] = {10,15}, 
[4] = {10,20}, 
[5] = {10,50}, 
[6] = {20,50}, 
[7] = {30,50}, 
[8] = {40,50}, 
}

TAB_DANHVONG = {
[1] = {1,10}, 
[2] = {5,15}, 
[3] = {10,15}, 
[4] = {10,20}, 
[5] = {10,50}, 
[6] = {20,50}, 
[7] = {30,50}, 
[8] = {40,50}, 
}

TAB_LIENDAU = {
[1] = {0,0}, 
[2] = {0,0}, 
[3] = {0,0}, 
[4] = {0,0}, 
[5] = {0,0}, 
[6] = {0,0}, 
[7] = {5,50}, 
[8] = {20,50}, 
}

TAB_TONGKIM = {
[1] = {0,0}, 
[2] = {0,0}, 
[3] = {0,0}, 
[4] = {0,0}, 
[5] = {0,0}, 
[6] = {0,0}, 
[7] = {100,500}, 
[8] = {500,5000}, 
}

TAB_BUYITEM = {
{53,"T¹p hãa","Hoµng Bè Ph¸t §¸i (CÊp 1) - HÖ <color=fire>Háa <color>",0,7,13,1,3},
{53,"Thî rÌn","Tóc §Ó Ch©m (CÊp 2) - HÖ <color=green>Méc <color>",0,1,2,2,1},
{1,"Thî rÌn","§iÓm Cang Th­¬ng (CÊp 3) - HÖ <color=green>Méc <color>",0,0,3,3,1},
{1,"T¹p hãa","Ng©n T¬ Tr¹c (CÊp 4) - HÖ <color=blue>Thñy <color>",0,8,0,4,2},
{78,"T¹p hãa","Tinh §ång Yªu §¸i (CÊp 5) - HÖ <color=blue>Thæ <color>",0,6,1,5,4},
{78,"Thî rÌn","Thanh Phong KiÕm (CÊp 3) - HÖ <color=yellow>Kim <color>",0,0,0,3,0},
{11,"Thî rÌn","Bµn Hoa C«n (CÊp 3) - HÖ <color=blue>Thñy <color>",0,0,2,3,2},
{11,"T¹p hãa","Kim T¬ Tr¹c (CÊp 5) - HÖ <color=blue>Thñy <color>",0,8,0,5,2},
{162,"T¹p hãa","T­îng B× Yªu §¸i (CÊp 6) - HÖ <color=red>Háa <color>",0,6,0,6,3},
{162,"Thî rÌn","NguyÖt Nha ThÝch (CÊp 3) - HÖ <color=green>Méc<color>",0,0,5,3,1},
{37,"Thî rÌn","Quû §Çu §ao (CÊp 3) - HÖ <color=green>Thæ <color>",0,0,1,3,4},
{37,"T¹p hãa","Ng¹c B× Hé UyÓn (CÊp 5) - HÖ <color=blue>Thñy <color>",0,8,1,5,2},
{80,"T¹p hãa","M·ng B× Hé UyÓn (CÊp 4) - HÖ <color=blue>Thñy <color>",0,8,1,4,2},
{80,"Thî rÌn","NguyÖt Nha ThÝch (CÊp 3) - HÖ <color=red>Háa <color>",0,0,5,3,3},
{176,"Thî rÌn","Bµn Hoa C«n (CÊp 3) - HÖ <color=green>Méc <color>",0,0,2,3,1},
{176,"T¹p hãa","Toan Ngoa (CÊp 5) - HÖ <color=green>Méc <color>",0,5,2,5,1},
};

TAB_FINDITEM = {
{"Hoµng Ngäc Giíi ChØ (CÊp 1)",0,3,0,1},
{"C¶m L·m Th¹ch Giíi ChØ (CÊp 2)",0,3,0,2},
{"Phï Dung Th¹ch Giíi ChØ (CÊp 3)",0,3,0,3},
{"PhØ Thóy Giíi ChØ (CÊp 4)",0,3,0,4},
{"Thóy Lùu Th¹ch Giíi ChØ (CÊp 5)",0,3,0,5},
{"Tæ MÉu Lôc Giíi ChØ (CÊp 6)",0,3,0,6},
{"H¶i Lam B¶o Th¹ch Giíi ChØ (CÊp 7)",0,3,0,7},
{"Hång B¶o Th¹ch Giíi ChØ (CÊp 8)",0,3,0,8},
{"Lam B¶o Th¹ch Giíi ChØ (CÊp 9)",0,3,0,9},
{"Toµn Th¹ch Giíi ChØ (CÊp 10)",0,3,0,10},

{"Hu©n Y H­¬ng Nang (CÊp 1)",0,9,0,1},
{"M¹t LÞ H­¬ng Nang (CÊp 2)",0,9,0,2},
{"Nhò H­¬ng H­¬ng Nang (CÊp 3)",0,9,0,3},
{"Lan Hoa H­¬ng Nang (CÊp 4)",0,9,0,4},
{"Hîp Hoan H­¬ng Nang (CÊp 5)",0,9,0,5},
{"Tö T« H­¬ng Nang (CÊp 6)",0,9,0,6},
{"TrÇm §µn H­¬ng Nang (CÊp 7)",0,9,0,7},
{"Tiªn X¹ H­¬ng Nang (CÊp 8)",0,9,0,8},
{"Giµ Nam H­¬ng Nang (CÊp 9)",0,9,0,9},
{"Long Tiªn H­¬ng Nang (CÊp 10)",0,9,0,10},
{"Du Diªn Ngäc Béi (CÊp 1)",0,9,1,1},
{"Kinh B¹ch Ngäc Béi (CÊp 2)",0,9,1,2},
{"§µo Hoa Ngäc Béi (CÊp 3)",0,9,1,3},
{"Mai Hoa  Ngäc Béi (CÊp 4)",0,9,1,4},
{"Ngò S¾c Ngäc Béi (CÊp 5)",0,9,1,5}, 
{"Thanh Ngäc Ngäc Béi (CÊp 6)",0,9,1,6}, 
{"BÝch Ngäc Ngäc Béi (CÊp 7)",0,9,1,7},
{"MÆc Ngäc Ngäc Béi (CÊp 8)",0,9,1,8}, 
{"Hoµng Ngäc Ngäc Béi (CÊp 9)",0,9,1,9}, 
{"D­¬ng Chi B¹ch Ngäc (CÊp 10)",0,9,1,10},
};

TAB_SERIES = {
"<color=yellow> Kim <color>","<color=green> Méc <color>","<corlor=blue> Thñy <color>","<color=fire> Háa <color>","<color=blue> Thæ <color>",
};

TAB_OPTION = {
 {110,"Thêi gian cho¸ng gi¶m",20,30,40}, --option dac biet 1
 {106,"Thêi gian lµm chËm gi¶m",20,30,40},--option dac biet 2
 {108,"Thêi gian tróng ®éc gi¶m",20,30,40},--option dac biet 3
 {113,"Thêi gian phôc håi gi¶m",20,30,40},--option dac biet 4
 {115,"Tèc ®é ®¸nh",10,20,30},--option dac biet 5
 {88,"Phôc håi sinh lùc mçi nöa gi©y",1,3},
 {96,"Phôc håi thÓ lùc mçi nöa gi©y",1,3},
 {92,"Phôc håi néi lùc mçi nöa gi©y",1,3},
 {105,"Kh¸ng b¨ng",1,18},
 {102,"Kh¸ng háa",1,18},
 {101,"Kh¸ng ®éc",1,18},
 {103,"Kh¸ng l«i",1,18},
 {97,"Søc m¹nh",1,20},
 {99,"Sinh khÝ",1,20},
 {98,"Th©n ph¸p",1,20},
 {104,"Phßng thñ vËt lý",1,20},
 {111,"Tèc ®é ch¹y",10,30},
 {85,"Sinh lùc tèi ®a",10,120},
 {89,"Néi lùc tèi ®a",10,120},
 {93,"ThÓ lùc tèi ®a",10,120},
};



function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/datau.lua");
		Msg2Player("Reload this script");
	end;
local TabSay ={};
	if (SYS_SWITCH_DAYTASK == 0) then
		Talk(1,"","TÝnh n¨ng nµy hiÖn ch­a ho¹t ®éng");
		return
	end
	
	if (GetLevel() < 80) then
		Talk(1,"","NhiÖm vô cña ta rÊt khã kh¨n, ®¹i hiÖp cÇn lªn ®¼ng cÊp 80 míi cã thÓ lµm ®­îc nhiÖm vô! ");
		return
	end
	
	if (GetTask(TASK_COUNT_QUEST) > MAX_TASK) then
		Talk(1,"","VÞ anh hïng nµy, ng­¬i ®· qu¸ tr­ëng nghÜa råi ta nghÜ ng­¬i nªn nghÜ ng¬i ngµy mai gÆp l¹i! ");
		return
	end;
	
	if (GetTask(TASK_CANCEL_QUEST) >= 5) then
		Talk(1,"","Ng­¬i b¶n lÜnh kÐm câi chót nhiÖm vô ta giao ng­¬i kh«ng thÓ hoµn thµnh, hÑn ng­¬i lóc kh¸c gÆp nhau hy väng ng­¬i kh«ng hñy qóa 5 nhiÖm vô n÷a ");
		return
	end;
	
	if (GetTask(TASK_ACCEPT) < 1) then
		TabSay = {
			"Ta ®Õn ®Ó nhËn nhiÖm vô/AcceptQuest",
			"Ta muèn t×m hiÓu nhiÖm vô d· tÈu/HelpQuest",
			"NhËn phÇn th­ëng khi hoµn thµnh mèc nhiÖm vô/CountQuest",
			"Ta ®Õn th¨m nhµ ng­¬i/OnCancel",
		};
	else
		TabSay = {
			"Ta ®Õn ®Ó hoµn thµnh nhiÖm vô/FinishQuest",
			"Ta cã háa tèc triÒu lÖnh hoµn thµnh ngay lËp tøc/FinishNow",
			"Ta muèn hñy nhiÖm vô nµy/CancelQuest",
			"Ta muèn t×m hiÓu nhiÖm vô d· tÈu/HelpQuest",
			"NhËn phÇn th­ëng khi hoµn thµnh mèc nhiÖm vô/CountQuest",		
			"Ta ®Õn th¨m nhµ ng­¬i/OnCancel",
		};
	end;
	if (GetTask(TASK_KIND) == 1) then -- tim do chi 
		szHello = format("H·y ®Õn <color=green>%s<color> t×m cho ta <color=yellow>%d tÊm thÇn bÝ ®Þa ®å chÝ <color> hy väng ng­¬i cã thÓ phôc mÖnh ta! ",GetMapName(GetTask(TASK_SAVEQUEST_1)),GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 2) then -- tim mat chi 
		szHello = format("H·y ®Õn <color=green>%s<color> t×m cho ta <color=yellow>%d tÊm thÇn bÝ mËt chÝ <color>hy väng ng­¬i cã thÓ phôc mÖnh ta! ",GetMapName(GetTask(TASK_SAVEQUEST_1)),GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 3) then -- danh quai
		szHello = format("H·y ®Õn <color=green>%s<color> tiªu diÖt cho ta  <color=yellow>%d tªn ®¹o tÆc lµm lo¹n <color> hy väng ng­¬i cã thÓ phôc mÖnh ta! ",GetMapName(GetTask(TASK_SAVEQUEST_1)),GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 4) then -- nang cap phuc duyen 
		szHello = format("H·y n©ng cÊp cho ta <color=yellow>%d<color> ®iÓm Phóc duyªn cña ng­¬i, hy väng ng­¬i lµm ®­îc nhiÖm vô nµy! ",GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 5) then -- nang cap danh vong
		szHello = format("H·y n©ng cÊp cho ta <color=yellow>%d<color> ®iÓm Danh väng cña ng­¬i, hy väng ng­¬i lµm ®­îc nhiÖm vô nµy! ",GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 6) then -- nang cap lien dau
		szHello = format("H·y n©ng cÊp cho ta <color=yellow>%d<color> ®iÓm Liªn ®Êu cña ng­¬i, hy väng ng­¬i lµm ®­îc nhiÖm vô nµy! ",GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 7) then -- nang cap tong kim
		szHello = format("H·y n©ng cÊp cho ta <color=yellow>%d<color> ®iÓm Tèng kim cña ng­¬i, hy väng ng­¬i lµm ®­îc nhiÖm vô nµy! ",GetTask(TASK_SAVEQUEST_2));
	elseif (GetTask(TASK_KIND) == 8) then -- mua do
		szHello = format("H·y ®Õn tiÖm %s %s mua cho ta mét c¸i %s ta cÇn ®Ó cho d©n nghÌo ë ®©y, ng­¬i lµm ®­îc chø? ",TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][2],GetMapName(TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][1]),TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][3]);
	elseif (GetTask(TASK_KIND) == 9) then -- tim option trang bi
		if (GetTask(TASK_SAVEQUEST_1) <= 5) then
			szHello = format("H·y t×m cho ta mét trang bÞ cã chøa dßng kü n¨ng <color=yellow> %s <color> <color=blue> %d <color>, hy väng ng­¬i ®i kh¾p ch©n trêi gãc bÓ ®Ó t×m cho ta! ",TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][2],GetTask(TASK_SAVEQUEST_2));
		else
			szHello = format("H·y t×m cho ta mét trang bÞ cã chøa dßng kü n¨ng <color=yellow> %s <color> tõ <color=blue> %d <color> ®Õn <color=blue> %d <color>, hy väng ng­¬i ®i kh¾p ch©n trêi gãc bÓ ®Ó t×m cho ta! ",TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][2],GetTask(TASK_SAVEQUEST_3),GetTask(TASK_SAVEQUEST_2));
		end
	elseif (GetTask(TASK_KIND) == 10) then -- tim trang bi theo he
		szHello = format("H·y t×m cho ta mét c¸i <color=yellow> %s <color> hÖ %s , hy väng ®¹i hiÖp cã thÓ t×m ®­îc mãn trang bÞ quý gi¸ nµy ! ",TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][1],TAB_SERIES[GetTask(TASK_SAVEQUEST_2)+1]);
	else
		szHello = format("Sè nhiÖm vô h«m nay cña ng­êi lµ : <color=yellow> %d/%d <color>, ng­¬i lµ nh÷ng ng­êi trÎ trong lµng h·y gióp ta hoµn thµnh t©m nguyÖn! ",GetTask(TASK_COUNT_QUEST),MAX_TASK);
	end;
	Say(szHello,getn(TabSay),TabSay);
	if (GetTask(TASK_KIND > 0))then
		AddNote(szHello);
	end;
end;

function AcceptQuest()
	--- Phan loai nhiem vu;
	local nRandom = random(1,10);
	SetTask(TASK_KIND,nRandom);
	--- Phan muc do nhiem vu theo level;
	local nLevel;
	if (GetLevel() < 20) then
		nLevel = 1;
	elseif (GetLevel() < 30) then
		nLevel = 2;
	elseif (GetLevel() < 40) then
		nLevel = 3;
	elseif (GetLevel() < 50) then
		nLevel = 4;
	elseif (GetLevel() < 60) then
		nLevel = 5;
	elseif (GetLevel() < 70) then
		nLevel = 6;
	elseif (GetLevel() < 80) then
		nLevel = 7;
	else
		nLevel = 8;
	end;
	
	if (GetTask(TASK_KIND) == 1) then -- dia do chi	
		local nRanLevel = TAB_MAP[nLevel][random(1,getn(TAB_MAP[nLevel]))];
		local nRanSoLuong = random(1,15);
		SetTask(TASK_SAVEQUEST_1,nRanLevel); -- lay id maps name 
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_DOCHI,nRanSoLuong);
	elseif (GetTask(TASK_KIND) == 2) then -- mat chi	
		local nRanLevel = TAB_MAP[nLevel][random(1,getn(TAB_MAP[nLevel]))];
		local nRanSoLuong = random(1,5);
		SetTask(TASK_SAVEQUEST_1,nRanLevel); -- lay id maps name 
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_MATCHI,nRanSoLuong);	
	elseif (GetTask(TASK_KIND) == 3) then -- danh quai
		local nRanLevel = TAB_MAP[nLevel][random(1,getn(TAB_MAP[nLevel]))];
		local nRanSoLuong = random(100,275);
		SetTask(TASK_SAVEQUEST_1,nRanLevel); -- lay id maps name 
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_DANHQUAI,nRanSoLuong);		
	elseif (GetTask(TASK_KIND) == 4) then -- phuc duyen
		local nRanSoLuong = random(TAB_PHUCDUYEN[nLevel][1],TAB_PHUCDUYEN[nLevel][2]);
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_PHUCDUYEN,GetFuyuan() + nRanSoLuong);	
	elseif (GetTask(TASK_KIND) == 5) then -- danh vong
		local nRanSoLuong = random(TAB_DANHVONG[nLevel][1],TAB_DANHVONG[nLevel][2]);
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_DANHVONG,GetRepute() + nRanSoLuong);	
	elseif (GetTask(TASK_KIND) == 6) then -- lien dau
		local nRanSoLuong = random(TAB_LIENDAU[nLevel][1],TAB_LIENDAU[nLevel][2]);
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_LIENDAU,GetTask(1020) + nRanSoLuong);	
	elseif (GetTask(TASK_KIND) == 7) then -- tong kim
		local nRanSoLuong = random(TAB_TONGKIM[nLevel][1],TAB_TONGKIM[nLevel][2]);
		SetTask(TASK_SAVEQUEST_2,nRanSoLuong); -- lay id so luong;
		SetTask(TASK_DT_TONGKIM,GetTask(2013) + nRanSoLuong);	
	elseif (GetTask(TASK_KIND) == 8) then -- mua do
		local nRandomShop = random(1,getn(TAB_BUYITEM));
		SetTask(TASK_SAVEQUEST_1,nRandomShop);	-- lay id random;	
	elseif (GetTask(TASK_KIND) == 9) then -- tim option trang bi;
		local nRandom = random(1,getn(TAB_OPTION));
		local nRanOption;
		SetTask(TASK_SAVEQUEST_1,nRandom);  -- luu id option;
		 if (nRandom <= 5) then
			nRanOption = RANDOMC(TAB_OPTION[nRandom][3],TAB_OPTION[nRandom][4],TAB_OPTION[nRandom][5]);
		 else
			nRanOption = random(TAB_OPTION[nRandom][3],TAB_OPTION[nRandom][4]);
			-- tinh gia tri nho nhat 		
			 if (nRanOption <= 5) then
				SetTask(TASK_SAVEQUEST_3,1);
			 elseif (nRanOption > 5) and (nRanOption < 20) then
				SetTask(TASK_SAVEQUEST_3,nRanOption-random(1,5));
			 elseif (nRanOption > 20) then 
				SetTask(TASK_SAVEQUEST_3,nRanOption-random(10,15));
			 end;
			--
		 end;
		 SetTask(TASK_SAVEQUEST_2,nRanOption); -- luu gia tri lon nhat cua option;
	elseif (GetTask(TASK_KIND) == 10) then -- tim trang bi theo he;
		local nRandom = random(1,getn(TAB_FINDITEM));
		local nRandomSeries = random(0,4);
		SetTask(TASK_SAVEQUEST_1,nRandom);
		SetTask(TASK_SAVEQUEST_2,nRandomSeries);
	end;
	SetTask(TASK_ACCEPT,1);
	main();
end;


function FinishQuest()
local nKind = GetTask(TASK_KIND);
	if (nKind == 1 ) then
		if (GetTask(TASK_DT_DOCHI) == 0) then
			SetTask(TASK_DT_DOCHI,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;
	elseif (nKind == 2 ) then
		if (GetTask(TASK_DT_MATCHI) == 0) then
			SetTask(TASK_DT_MATCHI,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;
	elseif (nKind == 3 ) then
		if (GetTask(TASK_DT_DANHQUAI) == 0) then
			SetTask(TASK_DT_DANHQUAI,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;
	elseif (nKind == 4 ) then
		if (GetFuyuan() >= GetTask(TASK_DT_PHUCDUYEN)) then
			SetTask(TASK_DT_PHUCDUYEN,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;
	elseif (nKind == 5 ) then
		if (GetRepute() >= GetTask(TASK_DT_DANHVONG)) then
			SetTask(TASK_DT_DANHVONG,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;	
	elseif (nKind == 6 ) then
		if (GetTask(1020) >= GetTask(TASK_DT_LIENDAU)) then
			SetTask(TASK_DT_LIENDAU,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;		
	elseif (nKind == 7 ) then
		if (GetTask(2013) >= GetTask(TASK_DT_TONGKIM)) then
			SetTask(TASK_DT_TONGKIM,0);
			-- SetTask(TASK_KIND,0);
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
			return
		else
			Talk(1,"",MSG_CONDITIONS);
			return
		end;		
	elseif (nKind == 8 ) then
		OpenGiveBox("Giao ®å D· TÈu","H·y ®Æt vËt phÈm ta cÇn vµo ®©y!\n"..TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][3].."","FinishBuyBox")
	elseif (nKind == 9) then
		if (GetTask(TASK_SAVEQUEST_1) <= 5) then
			OpenGiveBox("Giao ®å D· TÈu","H·y ®Æt vËt phÈm ta cÇn vµo ®©y!\n"..TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][2].."\n Gi¸ trÞ :"..GetTask(TASK_SAVEQUEST_2).."","FinishOptionBox")
		else
			OpenGiveBox("Giao ®å D· TÈu","H·y ®Æt vËt phÈm ta cÇn vµo ®©y!\n"..TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][2].."\n Gi¸ trÞ : nhá nhÊt "..GetTask(TASK_SAVEQUEST_3).." lín nhÊt"..GetTask(TASK_SAVEQUEST_2).."","FinishOptionBox")
		end;
	elseif (nKind == 10) then
		OpenGiveBox("Giao ®å D· TÈu","H·y ®Æt vËt phÈm ta cÇn vµo ®©y!\n"..TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][1].." hÖ "..TAB_SERIES[GetTask(TASK_SAVEQUEST_2)+1].." ","FinishFindBox")
	end;
end;

function FinishNow()
	if (GetItemCountInBag(6,59,0,0) < 1) then
		Talk(1,"","ChØ nh÷ng anh hïng hµo kiÖt míi ®­îc th¸i thó bang tÆng lÖnh bµi nµy, nhµ ng­êi lµ h¹n ng­êi tiÓu nh©n sao cã thÓ sì h÷u nã chø. Mau rêi khái ®©y!");
		return
	end;
	
	if (GetTask(TASK_DT_KEY) >= 20) then
		Talk(1,"","Tuy ng­¬i lµ ng­êi cã c«ng víi triÒu ®×nh, víi Th¸i thó. Nh­ng ta kh«ng thÓ v× thÕ mµ nh©n nh­îng cho ng­êi m·i ®­îc. HÑn ng­¬i ngµy kh¸c <color=yellow> "..GetTask(TASK_DT_KEY).."/20 <color> ");
		return
	end;
	DelTaskItem(59);
	
	local nKind = GetTask(TASK_KIND);
	if (nKind == 1 ) then
		SetTask(TASK_DT_DOCHI,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		SetTask(TASK_DT_KEY,GetTask(TASK_DT_KEY)+1);
		return
	elseif (nKind == 2 ) then
		SetTask(TASK_DT_MATCHI,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return
	elseif (nKind == 3 ) then
		SetTask(TASK_DT_DANHQUAI,0);
		-- SetTask(TASK_KIND,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return
	elseif (nKind == 4 ) then
		SetTask(TASK_DT_PHUCDUYEN,0);
		-- SetTask(TASK_KIND,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return
	elseif (nKind == 5 ) then
		SetTask(TASK_DT_DANHVONG,0);
		-- SetTask(TASK_KIND,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return	
	elseif (nKind == 6 ) then
		SetTask(TASK_DT_LIENDAU,0);
		-- SetTask(TASK_KIND,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return	
	elseif (nKind == 7 ) then
		SetTask(TASK_DT_TONGKIM,0);
		-- SetTask(TASK_KIND,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
		return	
	elseif (nKind == 8 ) then
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
		AddReward();
	elseif (nKind == 9) then
		if (GetTask(TASK_SAVEQUEST_1) <= 5) then
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_SAVEQUEST_3,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
		else
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_SAVEQUEST_3,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
		end;
	elseif (nKind == 10) then
			SetTask(TASK_SAVEQUEST_1,0);
			SetTask(TASK_SAVEQUEST_2,0);
			SetTask(TASK_ACCEPT,0);
			SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
			AddReward();
	end;
	
end;



function FinishBuyBox()
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][4] and nD == TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][5] and nP == TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][6] and nL == TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][7] and Ser == TAB_BUYITEM[GetTask(TASK_SAVEQUEST_1)][8]) then
					-- Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." ");
					Msg2Player("§a t¹ §¹i hiÖp ®· ñng hé trang bÞ cho d©n lµng n¬i ®©y h·y nhËn cña ta mét mãn quµ ");
					-- SetTask(TASK_KIND,0);
					SetTask(TASK_SAVEQUEST_1,0);
					SetTask(TASK_ACCEPT,0);
					SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
					DelItemByIndex(nItemIdx);
					EndGiveBox();
					AddReward();
					break
				else
					Talk(1,"","§¹i hiÖp ch­a lµm ®óng yªu cÇu cña ta, d©n lµng rÊt mong chê ®¹i hiÖp ");
					EndGiveBox();
					return
				end
			end
		end
	end
end;

function FinishOptionBox()
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				local nOp1,nValueMin1,nValueMax1 = GetItemMagicAttrib(nItemIdx,1); -- hien 1
				local nOp2,nValueMin2,nValueMax2 = GetItemMagicAttrib(nItemIdx,2); -- an 1
				local nOp3,nValueMin3,nValueMax3 = GetItemMagicAttrib(nItemIdx,3); -- hien 2
				local nOp4,nValueMin4,nValueMax4 = GetItemMagicAttrib(nItemIdx,4); -- an 2
				local nOp5,nValueMin5,nValueMax5 = GetItemMagicAttrib(nItemIdx,5); -- hien 3
				local nOp6,nValueMin6,nValueMax6 = GetItemMagicAttrib(nItemIdx,6); -- an 3			
				if  (nOp1 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1] or
					nOp2 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1] or
					nOp3 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1] or
					nOp4 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1] or
					nOp5 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1] or
					nOp6 == TAB_OPTION[GetTask(TASK_SAVEQUEST_1)][1]) then
					if (GetTask(TASK_SAVEQUEST_1) <= 5) then
						if (nValueMin1 == GetTask(TASK_SAVEQUEST_2) or
							nValueMin2 == GetTask(TASK_SAVEQUEST_2) or
							nValueMin3 == GetTask(TASK_SAVEQUEST_2) or
							nValueMin4 == GetTask(TASK_SAVEQUEST_2) or
							nValueMin5 == GetTask(TASK_SAVEQUEST_2) or
							nValueMin6 == GetTask(TASK_SAVEQUEST_2) )then
							-- Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." ");
							Msg2Player("§a t¹ §¹i hiÖp ®· ñng hé trang bÞ cho d©n lµng n¬i ®©y h·y nhËn cña ta mét mãn quµ ");
							-- SetTask(TASK_KIND,0);
							SetTask(TASK_SAVEQUEST_1,0);
							SetTask(TASK_SAVEQUEST_2,0);
							SetTask(TASK_SAVEQUEST_3,0);
							SetTask(TASK_ACCEPT,0);
							SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
							DelItemByIndex(nItemIdx);
							EndGiveBox();
							AddReward();
							break
						else
							Talk(1,"","§¹i hiÖp ch­a lµm ®óng yªu cÇu cña ta, d©n lµng rÊt mong chê ®¹i hiÖp ");
							EndGiveBox();
						end
					else
						if ((nValueMin1 >= GetTask(TASK_SAVEQUEST_3) and nValueMin1 <= GetTask(TASK_SAVEQUEST_2)) or
							(nValueMin2 >= GetTask(TASK_SAVEQUEST_3) and nValueMin2 <= GetTask(TASK_SAVEQUEST_2)) or
							(nValueMin3 >= GetTask(TASK_SAVEQUEST_3) and nValueMin3 <= GetTask(TASK_SAVEQUEST_2)) or
							(nValueMin4 >= GetTask(TASK_SAVEQUEST_3) and nValueMin4 <= GetTask(TASK_SAVEQUEST_2)) or
							(nValueMin5 >= GetTask(TASK_SAVEQUEST_3) and nValueMin5 <= GetTask(TASK_SAVEQUEST_2))or
							(nValueMin6 >= GetTask(TASK_SAVEQUEST_3) and nValueMin6 <= GetTask(TASK_SAVEQUEST_2)) ) then
							-- Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." ");
							Msg2Player("§a t¹ §¹i hiÖp ®· ñng hé trang bÞ cho d©n lµng n¬i ®©y h·y nhËn cña ta mét mãn quµ ");							
							-- SetTask(TASK_KIND,0);
							SetTask(TASK_SAVEQUEST_1,0);
							SetTask(TASK_SAVEQUEST_2,0);
							SetTask(TASK_SAVEQUEST_3,0);
							SetTask(TASK_ACCEPT,0);
							SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
							DelItemByIndex(nItemIdx);
							EndGiveBox();
							AddReward();
							break
						else
							Talk(1,"","§¹i hiÖp ch­a lµm ®óng yªu cÇu cña ta, d©n lµng rÊt mong chê ®¹i hiÖp ");
							EndGiveBox();
						end						
					end;
				else
					Talk(1,"","§¹i hiÖp ch­a lµm ®óng yªu cÇu cña ta, d©n lµng rÊt mong chê ®¹i hiÖp ");
					EndGiveBox();
					return
				end
			end
		end
	end
end;


function FinishFindBox()
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][2] and nD == TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][3] and nP == TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][4] and nL == TAB_FINDITEM[GetTask(TASK_SAVEQUEST_1)][5] and Ser == GetTask(TASK_SAVEQUEST_2) )then
					-- Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." ");
					Msg2Player("§a t¹ §¹i hiÖp ®· ñng hé trang bÞ cho d©n lµng n¬i ®©y h·y nhËn cña ta mét mãn quµ ");
					-- SetTask(TASK_KIND,0);
					SetTask(TASK_SAVEQUEST_1,0);
					SetTask(TASK_SAVEQUEST_2,0);
					SetTask(TASK_ACCEPT,0);
					SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
					DelItemByIndex(nItemIdx);
					EndGiveBox();
					AddReward();
					break
				else
					Talk(1,"","§¹i hiÖp ch­a lµm ®óng yªu cÇu cña ta, d©n lµng rÊt mong chê ®¹i hiÖp ");
					EndGiveBox();
					return
				end
			end
		end
	end
end;

function CancelQuest()
local szHello = "Ng­¬i thËt sù muèn bá qua nhiÖm vô nµy sao, ta thÊy nã kh«ng mÊy khã kh¨n víi nhµ ng­¬i!\nNg­¬i cßn l¹i <color=red>"..(5-GetTask(TASK_CANCEL_QUEST)).."/5<color> ";
local TAB_SAY ={
	"Ta muèn dïng 100 m·nh s¬n hµ x· t¾t ®Ó hñy/ExeCancelQuest",
	"Ta muèn dïng c¬ héi hñy bá ®Ó hñy nhiÖm vô nµy/ExeCancelQuest",
	"Ta muèn hñy ngay lËp tøc nhiÖm vô nµy/ExeCancelQuest",
	"Ng­¬i nãi ®óng ta cÇn suy nghÜ thªm/OnCancel",
}
	Say(szHello,getn(TAB_SAY),TAB_SAY);
end;


function ExeCancelQuest(nSel)
	
	if (nSel == 0 and GetTask(TASK_DOCHI) >= 100) then 
		SetTask(TASK_KIND,0);
		SetTask(TASK_DT_DOCHI,0);
		SetTask(TASK_DT_MATCHI,0);
		SetTask(TASK_DT_PHUCDUYEN,0);
		SetTask(TASK_DT_DANHVONG,0);
		SetTask(TASK_DT_LIENDAU,0);
		SetTask(TASK_DT_DANHQUAI,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_SAVEQUEST_3,0);
		SetTask(TASK_SAVEQUEST_4,0);
		SetTask(TASK_SAVEQUEST_5,0);
		SetTask(TASK_SAVEQUEST_6,0);
		SetTask(TASK_SAVEQUEST_7,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_DOCHI,GetTask(TASK_DOCHI)-100);
		Msg2Player("Sè m·nh ®å chÝ cña nhµ ng­¬i cßn l¹i lµ "..GetTask(TASK_DOCHI).." .");
		return 
	elseif (nSel == 0 and GetTask(TASK_DOCHI) < 100) then
		Talk(1,"","Ng­¬i ch­a tÝch lòy ®ñ 100 m·nh s¬n hµ x· t¾t (®å chÝ) kh«ng thÓ hñy nhiÖm vô ");
		return
	elseif (nSel == 1 and GetTask(TASK_DT_CANCEL) > 0) then
		SetTask(TASK_KIND,0);
		SetTask(TASK_DT_DOCHI,0);
		SetTask(TASK_DT_MATCHI,0);
		SetTask(TASK_DT_PHUCDUYEN,0);
		SetTask(TASK_DT_DANHVONG,0);
		SetTask(TASK_DT_LIENDAU,0);
		SetTask(TASK_DT_DANHQUAI,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_SAVEQUEST_3,0);
		SetTask(TASK_SAVEQUEST_4,0);
		SetTask(TASK_SAVEQUEST_5,0);
		SetTask(TASK_SAVEQUEST_6,0);
		SetTask(TASK_SAVEQUEST_7,0);
		SetTask(TASK_ACCEPT,0);
		SetTask(TASK_DT_CANCEL,GetTask(TASK_DT_CANCEL)-1);
		Msg2Player("S« c¬ héi hñy bá nhiÖm vô cña ng­¬i gi¶m cßn "..GetTask(TASK_DT_CANCEL).." ");
	elseif (nSel == 1 and GetTask(TASK_DT_CANCEL) <= 0) then
		Talk(1,"","Ng­¬i kh«ng cßn c¬ héi hñy bá nµo c¶ nªn kh«ng hñy bá ®­îc nhiÖm vô nµy! ");
		return
	else 
		SetTask(TASK_KIND,0);
		SetTask(TASK_DT_DOCHI,0);
		SetTask(TASK_DT_MATCHI,0);
		SetTask(TASK_DT_PHUCDUYEN,0);
		SetTask(TASK_DT_DANHVONG,0);
		SetTask(TASK_DT_LIENDAU,0);
		SetTask(TASK_DT_DANHQUAI,0);
		SetTask(TASK_SAVEQUEST_1,0);
		SetTask(TASK_SAVEQUEST_2,0);
		SetTask(TASK_SAVEQUEST_3,0);
		SetTask(TASK_SAVEQUEST_4,0);
		SetTask(TASK_SAVEQUEST_5,0);
		SetTask(TASK_SAVEQUEST_6,0);
		SetTask(TASK_SAVEQUEST_7,0);
		SetTask(TASK_ACCEPT,0);
	end;
	SetTask(TASK_COUNT_QUEST,GetTask(TASK_COUNT_QUEST) +1);
	SetTask(TASK_CANCEL_QUEST,GetTask(TASK_CANCEL_QUEST) + 1);
end;

function AddReward()
	SetTask(TASK_DT_TOTAL,GetTask(TASK_DT_TOTAL)+1);
	ReceiveReward()
end;


function ReceiveReward()--nhan thuong
	Tab_Button = {
	{1,2,5},{1,5,2},{2,1,5},{2,5,1},{5,1,2},{5,2,1},
	{1,2,4},{1,4,2},{2,1,4},{2,4,1},{4,1,2},{4,2,1},
	{1,5,4},{1,4,5},{5,1,4},{5,4,1},{4,1,5},{4,5,1},
	{5,2,4},{5,4,2},{2,5,4},{2,4,9},{4,1,5},{4,5,1},
	{9,2,4},{9,4,2},{9,5,4},{9,4,5},{9,1,2},{9,5,1},
	{1,9,4},{1,4,9},{9,1,4},{9,4,1},{4,9,1},{4,5,9},
	{1,2,9},{1,9,2},{2,1,9},{2,9,1},{9,1,2},{9,2,1},
	};
	local nRandom = random(1,getn(Tab_Button))
	local Reward1 = Tab_Button[nRandom][1]
	local Reward2 = Tab_Button[nRandom][2]
	local Reward3 = Tab_Button[nRandom][3]
	SetTask(TASK_DT_REWARD1,Reward1)
	SetTask(TASK_DT_REWARD2,Reward2)
	SetTask(TASK_DT_REWARD3,Reward3)
	OpenReWardBox("D· TÈu: XuÊt s¾c hoµn thµnh thö th¸ch!, h·y chän phÇn th­ëng!" ,""..Reward1.."|RwardAccept1",""..Reward2.."|RwardAccept2",""..Reward3.."|RwardAccept3");
end

function RwardAccept1()
	local nCheck = GetTask(TASK_DT_REWARD1);
	if (nCheck == 1) then
	OnAwardOnAdd("Money")
	elseif (nCheck == 2) then
	OnAwardOnAdd("Exp")
	elseif (nCheck == 5) then
	OnAwardOnAdd("Item")
	elseif (nCheck == 4) then
	OnAwardOnAdd("ran") 
	elseif (nCheck == 9) then
	OnAwardOnAdd("169")
	else
	end
	SetTask(TASK_DT_REWARD1,0)
	SetTask(TASK_DT_REWARD2,0)
	SetTask(TASK_DT_REWARD3,0)
end;

function RwardAccept2()
	local nCheck = GetTask(TASK_DT_REWARD2);
	if (nCheck == 1) then
	OnAwardOnAdd("Money")
	elseif (nCheck == 2) then
	OnAwardOnAdd("Exp")
	elseif (nCheck == 5) then
	OnAwardOnAdd("Item")
	elseif (nCheck == 4) then
	OnAwardOnAdd("ran") 
	elseif (nCheck == 9) then
	OnAwardOnAdd("169")
	else
	end
	SetTask(TASK_DT_REWARD1,0)
	SetTask(TASK_DT_REWARD2,0)
	SetTask(TASK_DT_REWARD3,0)
end;

function RwardAccept3()
	local nCheck = GetTask(TASK_DT_REWARD3);
	if (nCheck == 1) then
	OnAwardOnAdd("Money")
	elseif (nCheck == 2) then
	OnAwardOnAdd("Exp")
	elseif (nCheck == 5) then
	OnAwardOnAdd("Item")
	elseif (nCheck == 4) then
	OnAwardOnAdd("ran") 
	elseif (nCheck == 9) then
	OnAwardOnAdd("169")
	else
	end
	SetTask(TASK_DT_REWARD1,0)
	SetTask(TASK_DT_REWARD2,0)
	SetTask(TASK_DT_REWARD3,0)
end;

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

function OnAwardOnAdd(szString)
	local nKind = GetTask(TASK_KIND);
	if (szString == "Money") then
		Earn(random(TAB_REWARD[1][nKind][1],TAB_REWARD[1][nKind][2]));
	elseif (szString == "Exp") then
		AddSumExp(random(TAB_REWARD[2][nKind][1],TAB_REWARD[2][nKind][2]));
	elseif 	(szString == "Item") then
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
	elseif (szString == "ran") then
		local Rand = Random(1,5)
		if(Rand == 1) then 
			local nRepute = random(1,7);
			AddRepute(nRepute);
			Msg2Player("Ng­¬i nhËn ®­îc "..nRepute.." danh väng hoµn thµnh nhiÖm vô");
		elseif(Rand == 2) then
			local nFuyuan = random(2,5)
			AddFuyuan(nFuyuan);
			Msg2Player("Ng­¬i nhËn ®­îc "..nFuyuan.." phóc duyªn hoµn thµnh nhiÖm vô");
		elseif(Rand == 3) then 
		local Money = random(10000,30000)
			Earn(Money);
		elseif(Rand == 4) then 
			local nExp = GetLevel()* 5 * random(200,300);
				AddSumExp(nExp); 
				Msg2Player("Ng­¬i nhËn ®­îc "..nExp.." kinh nghiÖm hoµn thµnh nhiÖm vô!");
		elseif(Rand == 5) then 
			SetTask(TASK_DT_CANCEL,GetTask(TASK_DT_CANCEL) + 1); 
			Msg2Player("Ng­¬i nhËn ®­îc mét c¬ héi hñy bá, tæng sè c¬ héi hiÖn t¹i lµ <color=green> "..GetTask(TASK_DT_CANCEL).." <color> !");
		end
	elseif(szString == "169") then -- nhan co hoi huy bo nv
		SetTask(TASK_DT_CANCEL,GetTask(TASK_DT_CANCEL) + 1);
		Msg2Player("Ng­¬i nhËn ®­îc mét c¬ héi hñy bá, tæng sè c¬ héi hiÖn t¹i lµ <color=green> "..GetTask(TASK_DT_CANCEL).." <color> !");
	end;
	SetTask(TASK_KIND,0);
end;

function CountQuest()
	local szHello = "§a t¹ ng­¬i thêi gian qua ®· theo ta lµm rÊt nhiÒu viÖc khã kh¨n cho d©n lµng, ta cã chót quµ hËu t¹ ng­¬i!\nSè nhiÖm vô ng­¬i lµm ®­îc lµ <color=yellow> "..GetTask(TASK_DT_TOTAL).." <color> ! ";
	local TAB_SAY = {
		"Ta ®· hoµn thµnh 40 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 100 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 200 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 300 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 500 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 700 nhiÖm vô/RewardCountQuest",
		"Ta ®· hoµn thµnh 1000 nhiÖm vô/RewardCountQuest",
		"Ta chØ ghÐ ngang qua/OnCancel",
	
	};
	Say(szHello,getn(TAB_SAY),TAB_SAY);
end;

function RewardCountQuest(nSel)
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 
	end;
	
	if (nSel == 0 and GetTask(TASK_DT_TOTAL) >= 40 and GetTask(TASK_DT_REWAR40) < 1) then
		Earn(500000);
		AddSumExp(30000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		SetTask(TASK_DT_REWAR40,1);
	elseif (nSel == 1 and GetTask(TASK_DT_TOTAL) >= 100 and GetTask(TASK_DT_REWAR100) < 1) then
		Earn(1500000);
		AddSumExp(50000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		SetTask(TASK_DT_REWAR100,1);
	elseif (nSel == 2 and GetTask(TASK_DT_TOTAL) >= 200 and GetTask(TASK_DT_REWAR200) < 1) then
		Earn(2500000);
		AddSumExp(80000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		SetTask(TASK_DT_REWAR200,1);
	elseif (nSel == 3 and GetTask(TASK_DT_TOTAL) >= 300 and GetTask(TASK_DT_REWAR300) < 1) then
		Earn(2500000);
		AddSumExp(100000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		SetTask(TASK_DT_REWAR300,1);
	elseif (nSel == 4 and GetTask(TASK_DT_TOTAL) >= 500 and GetTask(TASK_DT_REWAR500) < 1) then
		Earn(3500000);
		AddSumExp(30000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		AddGoldItem(random(186,193));
		AddGoldItem(random(186,193));
		Msg2Player("Hai trang bÞ hoµng kim");
		SetTask(TASK_DT_REWAR500,1);
	elseif (nSel == 5 and GetTask(TASK_DT_TOTAL) >= 700 and GetTask(TASK_DT_REWAR700) < 1) then
		Earn(5500000);
		AddSumExp(30000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		AddGoldItem(random(186,193));
		AddGoldItem(random(186,193));
		AddGoldItem(random(186,193));
		Msg2Player("Ba trang bÞ hoµng kim");
		SetTask(TASK_DT_REWAR700,1);
	elseif (nSel == 6 and GetTask(TASK_DT_TOTAL) >= 1000 and GetTask(TASK_DT_REWAR1000) < 1) then
		Earn(15000000);
		AddSumExp(300000000);
		local nRandomItem = random(1,getn(TAB_REWARD[3]));
		local szName = TAB_REWARD[3][nRandomItem][1];
		local szGr = TAB_REWARD[3][nRandomItem][2];
		local szDetail = TAB_REWARD[3][nRandomItem][3];
		AddItem(szGr,szDetail,0,0,0,0,0);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn ®­îc mét <color=yell¬> "..szName.." <color> vÜnh viÔn");
		AddGoldItem(random(186,193));
		AddGoldItem(random(186,193));
		AddGoldItem(random(186,193));
		AddItem(0,10,7,10,random(0,4),0,0)
		SetTask(TASK_DT_REWAR1000,1);
		Msg2Player("Ba trang bÞ hoµng kim - 1 ngùa phiªn vò vÜnh viÔn");
	else
		Talk(1,"","Ng­¬i ch­a hoµn thµnh mèc nµy hoÆc ®· nhËn råi!");
	end;
end;

function HelpQuest()
Talk(8,"","<color=orange>D· tÈu<color>: Møc ®é phÇn th­ëng t¨ng theo §å ChÝ - MËt ChÝ - N©ng cÊp ®iÓm - mua ®å - t×m trang bÞ ","Khi hoµn thµnh 40 nhiÖm vô nhËn ®­îc:\n50 v¹n + 30tr Exp céng dån + 1 vËt phÈm bÊt kú vÜnh viÔn",
"Khi hoµn thµnh 100 nhiÖm vô nhËn ®­îc:\n150 v¹n + 50tr Exp céng dån + 1 vËt phÈm bÊt kú vÜnh viÔn",
"Khi hoµn thµnh 200 nhiÖm vô nhËn ®­îc:\n250 v¹n + 80tr Exp céng dån + 1 vËt phÈm bÊt kú vÜnh viÔn",
"Khi hoµn thµnh 300 nhiÖm vô nhËn ®­îc:\n350 v¹n + 300tr Exp céng dån + 1 vËt phÈm bÊt kú + 1 vËt phÈm hiÖp cèt hoÆc nhu t×nh ",
"Khi hoµn thµnh 500 nhiÖm vô nhËn ®­îc:\n350 v¹n + 300tr Exp céng dån + 1 vËt phÈm bÊt kú + 2 vËt phÈm hiÖp cèt hoÆc nhu t×nh vÜnh viÔn",
"Khi hoµn thµnh 700 nhiÖm vô nhËn ®­îc:\n550 v¹n + 300tr Exp céng dån + 1 vËt phÈm bÊt kú + 3 vËt phÈm hiÖp cèt hoÆc nhu t×nh vÜnh viÔn",
"Khi hoµn thµnh 1000 nhiÖm vô nhËn ®­îc:\n1500 v¹n + 300tr Exp céng dån + 1 vËt phÈm bÊt kú + 3 vËt phÈm hiÖp cèt hoÆc nhu t×nh + 1 Phiªn vò vÜnh viÔn");
end;


function OnCancel()
end;