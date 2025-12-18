----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Th­ viÖn gióp ®ì m«n ph¸i
----------------------------------

FHelp = {}

FHelp.Level_Str={"®¼ng cÊp ch­a ®Õn 10","h¬n cÊp 10","h¬n cÊp 20","h¬n cÊp 30","h¬n cÊp 40","h¬n cÊp 50","h¬n cÊp 60","h¬n cÊp 70","h¬n cÊp 80","h¬n cÊp 90"}
FHelp.Map_Str = {
	[1] = {
		"Khu vùc t©n thñ th«n",
		"GÇn bªn Bæn m«n, c¸c thµnh thÞ næi danh",
		"KiÕm C¸c Trung Nguyªn, KiÕm C¸c T©y Nam, KiÕm c¸c T©y B¾c, La Tiªu S¬n, Vò L¨ng S¬n, TÇn L¨ng",
		"Thôc C­¬ng S¬n, Miªu LÜnh, YÕn Tö §éng, D­îc V­¬ng §éng TÇn 1, ThiÕu L©m MËt ThÊt",
		"Kinh Hoµng §éng, Phôc Ng­u S¬n T©y, §iÓm Th­¬ng §éng, ThiÕu L©m MËt ThÊt",
		"H­íng Thñy §éng, Thiªn T©m §éng, §¸y §éng §×nh Hå, B¨ng HuyÖt §éng",
		"Thanh Loa §¶o, 108 La H¸n TrËn, Hoµnh S¬n §¹i §×nh, T­¬ng D­¬ng Nha M«n MËt §¹o",
		"§¹i Tï §éng, Thanh Loa ®¶o S¬n §éng, Linh Cèc §éng, L·o Hæ §éng, L©m Du Quan ",
		"Sa M¹c §Þa biÓu, Phï Dung §éng, Phi Thiªn §éng, L­ëng Thñy §éng, Thanh Kª §éng, B¨ng Hµ §éng, Tr­êng B¹ch S¬n C­íc",
		"Sa M¹c S¬n §éng 1, 2, 3, TiÕn Cóc §éng, Xi Vu §éng, Tr­êng B¹ch Nam Léc, Tr­êng B¹ch B¾c Léc"
	},
	[2] = {
		"Khu vùc t©n thñ th«n",
		"GÇn bªn Bæn m«n, c¸c thµnh thÞ næi danh",
		"KiÕm C¸c Trung Nguyªn, KiÕm C¸c T©y Nam, KiÕm c¸c T©y B¾c, La Tiªu S¬n, Vò L¨ng S¬n, TÇn L¨ng",
		"Miªu LÜnh, D­îc V­¬ng §éng TÇng 1, Kho¸i h¹t L©m, Phôc Ng­u S¬n §«ng, Kim Quang §éng, TuyÕt B¸o §éng tÇng 1,2",
		"KiÕn TÝnh Phong s¬n ®éng, Kª Qu¸n §éng, Hoµng Hµ Nguyªn §Çu, Táa V©ng §éng, §iÓm Th­¬ng §éng tÇng 2",
		"H­ëng Thñy §éng, B¨ng NguyÖt mª cung, L­u tiªn ®éng, ThiÕt Th¸p tÇng 1, 2, 3",
		"Hoµnh S¬n Ph¸i, T­¬ng D­¬ng Nha M«n MËt §¹o, L­u Tiªn §éng tÇng 2, 3, 4 ",
		"§µo Hoa Nguyªn, L­u Tiªn §éng tÇng 5, 6, TÇng l¨ng tÇng 2, Long Nh·n §éng, L©m Du Quan",
		"Sa M¹c §Þa BiÓu, L­ëng Thñy §éng, V« Danh §éng, B¨ng Hµ §éng, Thanh khª §éng, Tr­êng B¹ch S¬n C­íc",
		"Sa M¹c S¬n §éng 1, 2, 3, D­îc V­¬ng §éng TÇng 4, C¸n Viªn §éng, Tr­êng B¹ch Nam Léc, Tr­êng B¹ch B¾c Léc"
	},
	[3] = {	
		"Khu vùc t©n thñ th«n",
		"GÇn bªn Bæn m«n, c¸c thµnh thÞ næi danh",
		"KiÕm C¸c Trung Nguyªn, KiÕm C¸c T©y Nam, KiÕm c¸c T©y B¾c, La Tiªu S¬n, Vò L¨ng S¬n, TÇn L¨ng",
		"B¹ch V©n ®éng, Thôc C­¬ng s¬n, Thæ PhØ ®éng, Phôc Ng­u S¬n ®éng, Kim Quang ®éng",
		"Phông Nh·n ®éng, §iÓm Th­¬ng ®éng tÇng 1, ThÇn Tiªn ®éng, Phôc Ng­u S¬n t©y, Kinh Hoµng ®éng",
		"H­ëng Thñy ®éng, Thiªn tÇm Th¸p tÇng 1, 2, §Þa ®¹o hËu viÖn TÝn T­íng tù, ¸c B¸ §Þa §¹o",
		"Thiªn tÇm Th¸p tÇng 3, 108 La H¸n TrËn, Mª cung cÊm ®Þa, Hoµng S¬n Ph¸i ",
		"Tr­êng giang nguyªn ®Çu, S¬n b¶o ®éng, §¶o hoa nguyªn, D­îc V­¬ng ®éng tÇng 2, L©m Du Quan",
		"Sa M¹c §Þa BiÓu, Phi Thiªn §éng, Phïng Dung ®éng, D­¬ng trung ®éng, Cæ D­¬ng ®éng, D­îc V­¬ng ®éng tÇng 3, Tr­êng B¹ch S¬n C­íc",
		"Sa M¹c S¬n §éng 1, 2, 3, Xi V­u §éng, TiÕn Cóc §éng, Tr­êng B¹ch Nam Léc, Tr­êng B¹ch B¾c Léc"
	},
	[4] = {	
		"Khu vùc t©n thñ th«n",
		"GÇn bªn Bæn m«n, c¸c thµnh thÞ næi danh",
		"KiÕm C¸c Trung Nguyªn, KiÕm C¸c T©y Nam, KiÕm c¸c T©y B¾c, La Tiªu S¬n, Vò L¨ng S¬n, TÇn L¨ng",
		"Phôc Ng­u S¬n §«ng, Kim Quang §éng, TÇng L¨ng TÇng 1, Vò Di S¬n ",
		"Táa V©n §éng, Long Cung §éng, §iÓm Th­¬ng §éng tÇng 2, Thanh Thµnh S¬n, §iÓm Th­¬ng S¬n",
		"H­ëng Thñy §éng, BiÖn Kinh ThiÕt Th¸p tÇng 1, 2, 3, T­êng v©n ®éng tÇng 1, Ngù Hoa viªn S¬n ®éng ",
		"T­êng v©n ®éng tÇng 2, 3, 4, Thiªn NhÉn Th¸nh §éng, D­¬ng Gi¸c §éng, Tróc T¬ §éng TÇng 1",
		"D­îc V­¬ng §éng tÇng 2, L·o Hæ §éng, T­êng V©n ®éng tÇng 5, Linh Cèc §«ng, L©m Du Quan",
		"Sa M¹c §Þa BiÓu, Vò L¨ng §éng, V« Danh §éng, Nh¹n Th¹ch §éng, TÇn L¨ng tÇng 3, Tr­êng B¹ch S¬n C­íc",
		"Sa M¹c S¬n §éng 1, 2, 3, D­îc V­¬ng §éng TÇng 4, C¸n Viªn §éng, Tr­êng B¹ch Nam Léc, Tr­êng B¹ch B¾c Léc"	
	},
	[5] = {	
		"Khu vùc t©n thñ th«n",
		"GÇn bªn Bæn m«n, c¸c thµnh thÞ næi danh",
		"KiÕm C¸c Trung Nguyªn, KiÕm C¸c T©y Nam, KiÕm c¸c T©y B¾c, La Tiªu S¬n, Vò L¨ng S¬n, TÇn L¨ng",
		"Miªu LÜnh, D­îc V­¬ng §éng TÇng 1, Kho¸i h¹t L©m, Phôc Ng­u S¬n §«ng, Kim Quang §éng, TuyÕt B¸o §éng tÇng 1,2",
		"KiÕn TÝnh Phong s¬n ®éng, Kª Qu¸n §éng, Hoµng Hµ Nguyªn §Çu, Táa V©ng §éng, §iÓm Th­¬ng §éng tÇng 2",
		"H­ëng Thñy §éng, B¨ng NguyÖt mª cung, L­u tiªn ®éng, ThiÕt Th¸p tÇng 1, 2, 3",
		"Hoµnh S¬n Ph¸i, T­¬ng D­¬ng Nha M«n MËt §¹o, L­u Tiªn §éng tÇng 2, 3, 4 ",
		"§µo Hoa Nguyªn, L­u Tiªn §éng tÇng 5, 6, TÇng l¨ng tÇng 2, Long Nh·n §éng, L©m Du Quan",
		"Sa M¹c §Þa BiÓu, L­ëng Thñy §éng, V« Danh §éng, B¨ng Hµ §éng, Thanh khª §éng, Tr­êng B¹ch S¬n C­íc",
		"Sa M¹c S¬n §éng 1, 2, 3, D­îc V­¬ng §éng TÇng 4, C¸n Viªn §éng, Tr­êng B¹ch Nam Léc, Tr­êng B¹ch B¾c Léc"
	},
}

----------------------------------
--
----------------------------------
function FHelp:Map()
	nSeries = GetSeries() + 1 
	nLevel = floor(GetLevel()/10 + 1)
	if (nLevel > 10) then 
		nLevel = 10 
	end
	Say("§èi víi hÖ "..FHelp:GetSeriesColorText(nSeries).."<color=red>"..FHelp.Level_Str[nLevel].."<color> mµ nãi, n¬i luyÖn c«ng tèt nhÊt gåm cã "..FHelp.Map_Str[nSeries][nLevel]..".",0)
end

----------------------------------
--
----------------------------------
function FHelp:Skill(nFactionId)
	if(nFactionId == 1) then
		Talk(5,"",10027,10028,10029,10030,10031)
	elseif(nFactionId == 2) then
		Talk(5,"",10050,10051,10052,10053,10054)
	elseif(nFactionId == 3) then
		Talk(6,"",10071,10072,10073,10074,10075,10076)	
	elseif(nFactionId == 4) then
		Talk(5,"",10093,10094,10095,10096,10097)	
	elseif(nFactionId == 5) then
		Talk(5,"",10115,10116,10117,10118,10119)	
	elseif(nFactionId == 6) then
		Talk(4,"",10139,10140,10141,10142)
	elseif(nFactionId == 7) then
		Talk(4,"",10159,10160,10161,10162)
	elseif(nFactionId == 8) then
		Talk(5,"",10179,10180,10181,10182,10183)
	elseif(nFactionId == 9) then
		Talk(4,"",10202,10203,10204,10205)
	elseif(nFactionId == 10) then
		Talk(5,"",10222,10223,10224,10225,10226)
	end
end

----------------------------------
-- 
----------------------------------
function FHelp:GetSeriesColorText(nSeries)
	local tbColorText = {
			"<color=metal>Kim <color>",
			"<color=wood>Méc <color>",
			"<color=water>Thñy <color>",
			"<color=fire>Háa <color>",
			"<color=earth>Thæ <color>",		
		}
	if(nSeries < 1) or (nSeries > 5) then
		return tbColorText[1]
	end
	return tbColorText[nSeries]
end