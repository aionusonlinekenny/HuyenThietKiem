----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: ThiÕu L©m La H¸n
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10004)
	elseif(sPlayer_Faction == "shaolin") then
		local sNpcName = format("<color=metal>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/32"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." S­ ®Ö! S­ phô l¹i ph¸i ®Ö xuèng nói ­?",sImage,4, "XuÊt s­ xuèng nói./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Xin thay ta vÊn an s­ phô./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Ng­¬i ®· gia nhËp bang héi <color=green>"..GetTongName().."<color>! Nªn nhí h·y thÓ hiÖn cho tèt! §õng lµm mÊt mÆt ThiÕu L©m chóng ta!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				else
					SayImage(""..sNpcName.." Huynh ®Ö ®· thµnh lËp bang héi <color=green>"..GetTongName().."<color> råi ­? Haha! ThËt kh«ng hæ danh lµ ®Ö tö tôc gia cña ThiÕu L©m chóng ta!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				end
			else
				SayImage(""..sNpcName.." Huynh ®Ö cã muèn vª l¹i m«n ph¸i kh«ng?",sImage,2, "VÒ l¹i ThiÕu L©m ph¸i./Return", "Xin thay ta vÊn an s­ phô./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10010)
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10011)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10012)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10013)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10014)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10015)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10016)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10017)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10018)
	end
end

----------------------------------
--
----------------------------------
function Enroll_Select()
	if(GetSeries() ~= 0) then
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
	if(GetSex() ~= 0) then
		Talk(1,"", 10008)
		return
	end
	Say(10009, 2, "Gia nhËp ThiÕu L©m ph¸i./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(1) ~= 0) then
		Talk(1,"",10024)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10019, 2, "Ta ®ñ b¶n lÜnh./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(1) ~= 0) then
		Talk(1,"",10025)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10022, 2, "Ta muèn quay vÒ./OnReturn", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(1) ~= 0) then
		Talk(1,"",10026)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(1)
end