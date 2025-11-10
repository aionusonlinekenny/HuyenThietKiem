----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: §Ö Tö C¸i Bang
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10143)
	elseif(sPlayer_Faction == "gaibang") then
		local sNpcName = format("<color=fire>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "-85/-85/38"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." Huynh ®Ö lang thang n¬i ®©y lµm g×? Cã r­îu ngon kh«ng chóng ta cïng uèng! Ha!Ha!",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Ng­êi cña bæn bang kh¾p n¬i! C¶ bang héi <color=green>"..GetTongName().."<color> còng cã! RÊt tèt! Haha",sImage,0)
				else
					SayImage(""..sNpcName.." Ng­¬i l·nh ®¹o bang héi <color=green>"..GetTongName().."<color> th× còng ®­îc xem lµ ph©n nh¸nh cña C¸i Bang! Kh«ng ph¶i chø! Haha!",sImage,0)
				end
			else
				SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." cã muèn quay vÒ tæng ®µ C¸i Bang kh«ng? Chóng ta cïng uèng r­îu luËn tr¨ng sao! Haha!",sImage,2, "Quay vÒ C¸i Bang./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10147)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10146)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10148)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10149)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10150)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10151)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10152)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10153)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10154)
	end
end

----------------------------------
--
----------------------------------
function Enroll_Select()
	if(GetSeries() ~= 3) then
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
	Say(10144, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(7) ~= 0) then
		Talk(1,"",10145)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10155, 2, "Rêi khái tæng ®µ./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(7) ~= 0) then
		Talk(1,"",10156)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10157, 2, "TÊt nhiªn lµ cã./OnReturn", "Quªn mang theo råi./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(7) ~= 0) then
		Talk(1,"",10158)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(7)
end