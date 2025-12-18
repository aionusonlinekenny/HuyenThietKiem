----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Vâ §ang §¹o Nh©n
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(3, "Enroll_Select", 10184,10185,10186)
	elseif(sPlayer_Faction == "wudang") then
		local sNpcName = format("<color=earth>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "-85/-85/40"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." v× sao kh«ng lo tËp luyÖn l¹i ®i ®Õn n¬i nµy?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." xuÊt s­ kh«ng l©u mµ ®· gia nhËp bang héi <color=green>"..GetTongName().."<color> råi sao? Cã g©y tiÕng xÊu g× cho Vâ §ang ta kh«ng?",sImage,0)
				else
					SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." thèng lÜnh bang héi <color=green>"..GetTongName().."<color> thËt lµ b¶n lÜnh h¬n ng­êi! Lµm s­ huynh nh­ ta thËt lÊy lµm hæ thÑn!",sImage,0)
				end
			else
				SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." cã muèn quay vÒ nói Vâ §ang th¨m s­ phô hay kh«ng?",sImage,2, "Quay vÒ Vâ §ang./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10189)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10190)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10191)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10192)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10193)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10194)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10195)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10196)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10197)
	end
end

----------------------------------
--
----------------------------------
function Enroll_Select()
	if(GetSeries() ~= 4) then
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
	Say(10187, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(9) ~= 0) then
		Talk(1,"",10188)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10198, 2, "Rêi khái nói Vâ §ang./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(9) ~= 0) then
		Talk(1,"",10199)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10200, 2, "XuÊt ph¸t th«i./OnReturn", "Quªn mang theo råi./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(9) ~= 0) then
		Talk(1,"",10201)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(9)
end