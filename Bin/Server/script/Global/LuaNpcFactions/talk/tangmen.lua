----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: §­êng m«n thÞ vÖ
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(2, "Enroll_Select", 10055, 10056)
	elseif(sPlayer_Faction == "tangmen") then
		local sNpcName = format("<color=wood>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "-85/-85/34"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." RÌn luyÖn kü n¨ng ¸m s¸t kh«ng ph¶i mét sím mét chiÒu lµ ®­îc! T¹i sao ng­êi kh«ng tËp luyÖn mµ l¹i lang thang ë ®©y?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Hy väng ng­¬i sÏ trë thµnh mét trong nh÷ng s¸t thñ giái trong bang héi <color=green>"..GetTongName().."<color>. Lµm r¹ng danh §­êng M«n chóng ta!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				else
					SayImage(""..sNpcName.." §Ö tö §­êng M«n l·nh ®¹o bang héi <color=green>"..GetTongName().."<color> råi ®©y danh väng §­êng M«n l¹i t¨ng cao! Haha!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				end
			else
				SayImage(""..sNpcName.." Kü n¨ng cña ng­êi cã vÎ ch­a hoµn thiÖn! Cã muèn quay vÒ rÌn luyÖn thªm kh«ng?",sImage,2, "Quay vÒ §­êng M«n./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10058)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10059)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10061)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10060)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10062)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10062)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10063)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10064)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10065)
	end
end

----------------------------------
--
----------------------------------
function Enroll_Select()
	if(GetSeries() ~= 1) then
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
	Say(10057, 2, "Gia nhËp §­êng M«n./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(3) ~= 0) then
		Talk(1,"",10066)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10067, 2, "Rêi khái m«n ph¸i./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(3) ~= 0) then
		Talk(1,"",10068)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10069, 2, "Quay vÒ m«n ph¸i./OnReturn", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(3) ~= 0) then
		Talk(1,"",10070)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(3)
end