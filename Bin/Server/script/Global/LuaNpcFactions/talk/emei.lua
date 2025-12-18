----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Nga My CÈm Y Ni
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10098)
	elseif(sPlayer_Faction == "emei") then
		local sNpcName = format("<color=water>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "-85/-85/36"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." TiÓu s­ muéi l¹i lang thang ng¾m c¶nh µ? Cã cÇn ta gióp g× kh«ng?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." §· l©u kh«ng gÆp! TiÓu muéi gia nhËp bang héi <color=green>"..GetTongName().."<color> råi ­? Hy väng víi kü n¨ng cña m×nh muéi cã thÓ gióp hä v­ît qua c¬n nguy kÞch!",sImage,1, "§a t¹ s­ tû./OnCancel")
				else
					SayImage(""..sNpcName.." N÷ nh©n l·nh ®¹o bang héi <color=green>"..GetTongName().."<color> sao? RÊt tèt! Ai nãi chèn giang hå chØ giµnh cho nam nh©n chø! Haha!",sImage,1, "§a t¹ s­ tû./OnCancel")
				end
			else
				SayImage(""..sNpcName.." ThÕ giíi bªn ngoµi cã nh­ muéi t­ëng t­îng kh«ng? NÕu c¶m thÊy mÖt mái cã thÓ quay vÒ nói Nga My!",sImage,2, "Quay vÒ Nga My ph¸i./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10099)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10100)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10101)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10102)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10103)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10104)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10105)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10106)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10107)
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
		Talk(1,"", 10109)
		return
	end
	Say(10108, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(5) ~= 0) then
		Talk(1,"",10110)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10111, 2, "XuÊt s­ xuèng nói./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(5) ~= 0) then
		Talk(1,"",10112)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10113, 2, "§· chuÈn bÞ xong./OnReturn", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(5) ~= 0) then
		Talk(1,"",10114)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(5)
end