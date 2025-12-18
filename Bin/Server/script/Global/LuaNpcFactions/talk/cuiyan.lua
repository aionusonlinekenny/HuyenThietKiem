----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Thóy Yªn Hoa Sø
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")
Include("\\script\\lib\\TaskLib.lua")

----------------------------------
-- Hµm gäi chÝnh
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10120)
	elseif(sPlayer_Faction == "cuiyan") then
		local sNpcName = format("<color=water>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/37"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." Phong c¶nh thóy yªn thËt tuyÖt ph¶i kh«ng? S­ muéi cÇn ta gióp g× nµo?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Bang héi <color=green>"..GetTongName().."<color> cã sù gia nhËp cña ®Ö tö Thóy Yªn m«n nh­ hæ thªm c¸nh! Ta nãi kh«ng sai chø!",sImage,1, "§a t¹ s­ tû./OnCancel")
				else
					SayImage(""..sNpcName.." Bang héi <color=green>"..GetTongName().."<color> ®­îc l·nh ®¹o bëi kú nh©n Thóy Yªn th× cßn g× b»ng! ThËt lµ vang danh thiªn h¹! H·y cho bän nam nh©n h¸o s¾c biÕt mïi!",sImage,1, "§a t¹ s­ tû./OnCancel")
				end
			else
				SayImage(""..sNpcName.." TiÓu muéi cã muèn trë vÒ nói kh«ng! Ta thËt cã nhiÒu chuyÖn muèn t©m sù cïng muéi!",sImage,2, "Quay vÒ Nga My ph¸i./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10122)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10123)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10124)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10125)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10126)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10127)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10128)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10129)
	elseif(sPlayer_Faction == "gaibang") then
		if(GetSex() == 0) then
			Talk(2, "", 10130, 10132)
		else
			Talk(2, "", 10131, 10132)
		end
	end
end

----------------------------------
--
----------------------------------
function Enroll_Select()
	if(GetSeries() ~= 2) then
		return
	end
	if(GetLevel() < 10) then
		Talk(1,"",10005)
		return
	end
	if(GetCamp() ~= 0) then
		Talk(1,"", 10006)
		return
	end
	if(GetSex() ~= 1) then
		Talk(1,"", 10133)
		return
	end
	Say(10121, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(6) ~= 0) then
		Talk(1,"",10134)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10135, 2, "XuÊt s­ xuèng nói./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(6) ~= 0) then
		Talk(1,"",10136)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10137, 2, "TÊt nhiªn lµ cã./OnReturn", "Quªn mang theo råi./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(6) ~= 0) then
		Talk(1,"",10138)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(6)
end