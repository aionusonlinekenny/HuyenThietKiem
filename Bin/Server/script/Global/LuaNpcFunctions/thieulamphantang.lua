-- Author:	Kinnox;
-- Date:	15-09-2023
-- ThiÕu L©m Ph¶n T¨ng TÈy tñy ®¶o
Include("\\script\\system_config.lua")
Include("\\script\\global\\LuaNpcFactions\\faction_head.lua")


function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/thieulamphantang.lua");
		Msg2Player("Reload this script");
	end;
	TalkNpcPhanTang(nNpcIdx)
end


function TalkNpcPhanTang(nNpcIdx)
	local tbSay = {}
	local n = GetTask(0)
	local nStep = GetByte(n,4)
	if( (nStep == 1) or (nStep == 4) ) then
		tbSay = {
			"TÈy tiÒm n¨ng./OnClearProp",
			"TÈy kü n¨ng./OnClearMagic",
			"TÈy tÊt c¶./OnClearAll",
			"Kh«ng tÈy./OnCancel",
			"Liªn quan tÈy tñy./OnHelp",
			"Rêi khái ®¶o./OnLeaveHere",
		}
	elseif(nStep == 3) then
		tbSay = {
			"TÈy tiÒm n¨ng./OnClearProp",
			"Kh«ng tÈy./OnCancel",
			"Liªn quan tÈy tñy./OnHelp",
			"Rêi khái ®¶o./OnLeaveHere",
		}
	elseif(nStep == 2) then
		tbSay = {
			"TÈy kü n¨ng./OnClearMagic",
			"Kh«ng tÈy./OnCancel",
			"Liªn quan tÈy tñy./OnHelp",
			"Rêi khái ®¶o./OnLeaveHere",
		}
	else
		Talk(1,"",10002)
		return
	end 
	SayImage(10418,"-85/-85/70",getn(tbSay),tbSay)
end


function OnLeaveHere()
	Say("Ng­¬i cã ch¾c r»ng muèn rêi khái ®©y kh«ng? ",2, "Muèn rêi khái./OnLeaveHere2", "§Ó ta suy nghÜ lai./OnCancel")
end

function OnLeaveHere2()
	Say("NÕu ®· rêi khái th× kh«ng cã c¸ch nµo quay trë l¹i dÔ dµng ®©u!", 2, "Ta muèn rêi khái n¬i quû qu¸i nµy./OnLeaveMap", "Kh«ng! Ta sÏ ë l¹i./OnCancel")
end


function OnLeaveMap()
	SetClearPos();
	ForbidTownPortal(0)
	LeaveTeam()
	SetFightState(0)
	SetPunish(1)
	SetLogoutRV(0)
	SetRevPos(53, 19)
	NewWorld(53,1582, 3237)
end


function OnClearProp()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy tiÒm n¨ng? ",2, 
		"TÈy tiÒm n¨ng./OnClearPropCore", 
		"Kh«ng tÈy./OnCanCel")
end


function OnClearPropCore()
	Faction:ResetAttr()
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end


function OnClearMagic()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy kü n¨ng? ",2, 
	"TÈy kü n¨ng./OnClearMagicCore", 
	"Kh«ng tÈy./OnCanCel")
end


function OnClearMagicCore()
	Faction:ResetMagic(0)
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end


function OnClearAll()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy tÊt c¶? ",2, "TÈy tÊt c¶./OnClearAllCore", "Kh«ng tÈy./OnCanCel")
end


function OnClearAllCore()
	Faction:ResetAttr()
	Faction:ResetMagic(0)
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end


function OnHelp()
	local strHelp = 
	{
		"Sau khi tÈy tñy ng­¬i cã thÓ ra ngoµi ®¸nh méc nhËn,hoÆc cã thÓ lËp nhãm vµo s¬n ®éng thùc hµnh chiÕn ®Êu, sau ®ã rêi ®¶o míi cã thÓ xem lµ ®¾c ®¹o. ",
		"ChØ c©n ng­¬i kh«ng rêi ®¶o, ta cã thÓ gióp ng­¬i tÈy tñy",
		"Bªn ngoµi cã c¸c cét gæ 3000 sinh lùc ;  bao c¸t 10000 sinh lùc ; Méc nh©n 30000 sinh lùc ®Ó ng­¬i tËp luyÖn hiÖu qu¶!",
		"NÕu nh­ muèn thùc hµnh cã thÓ vµo s¬n ®éng nÕu nh­ cã chÕt còng kh«ng mÊt m¸t g×",
		"Trong s¬n ®éng rÊt hÑp nhiÒu nhÊt chØ chøa 20 ng­êi nÕu ®· ®ñ ng­êi th× kh«ng nªn vµo trong. ",
		"Cuèi cïng lóc nµo khi thÊy kü n¨ng ®· thµnh th¹o th× cã thÓ gÆp ta ®­a vÒ chæ cñ"
	}
	Talk(6, "", strHelp[1], strHelp[2], strHelp[3], strHelp[4], strHelp[5], strHelp[6]);
end;

function OnCancel()

end;
