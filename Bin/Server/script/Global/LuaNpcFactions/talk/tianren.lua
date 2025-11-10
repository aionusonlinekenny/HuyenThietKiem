----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Thiªn NhÉn Tö SÜ
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10163)
	elseif(sPlayer_Faction == "tianren") then
		local sNpcName = format("<color=fire>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/39"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." Ng­¬i kh«ng lo luyÖn tËp n©ng cao kü n¨ng mµ lang thang n¬i nµy lµm g×?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Haha! Gia nhËp bang héi <color=green>"..GetTongName().."<color> råi sao! RÊt tèt, vâ l©m Trung Nguyªn sím muén g× còng r¬i vµo tay chóng ta! Ha!Ha!Ha",sImage,0)
				else
					SayImage(""..sNpcName.." Thµnh lËp bang héi <color=green>"..GetTongName().."<color> nh­ng h·y nhí lý t­ëng vµ sù nghiÖp t­¬ng lai cña Thiªn NhÉn gi¸o chóng ta! Ha!Ha!Ha",sImage,0)
				end
			else
				SayImage(""..sNpcName.." Nghe nãi ng­¬i sau khi xuÊt s­ ®· lËp nªn nhiÒu c«ng danh! Qu¶ kh«ng hæ danh lµ ®Ö tö cña bæn gi¸o!",sImage,2, "Quay vÒ Thiªn NhÉn gi¸o./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10166)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10167)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10168)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10169)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10170)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10171)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10172)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10173)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10174)
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
	Say(10164, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(8) ~= 0) then
		Talk(1,"",10165)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10175, 2, "Ta ®ång ý./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(8) ~= 0) then
		Talk(1,"",10176)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10177, 2, "TÊt nhiªn lµ cã./OnReturn", "Quªn mang theo råi./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(8) ~= 0) then
		Talk(1,"",10178)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(8)
end