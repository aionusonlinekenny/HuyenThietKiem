----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: C«n L«n Hé Ph¸p
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10206)
	elseif(sPlayer_Faction == "kunlun") then
		local sNpcName = format("<color=earth>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/41"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." ®¹o thuËt kh«ng thÓ mét sím mét chiÒu lµ tu luyÖn xong! Sao ng­¬i cßn lang thang n¬i ®©y?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." "..Faction.SexName[GetSex() + 1].." gia nhËp <color=green>"..GetTongName().."<color> thËt ®¸ng tù hµo cho C«n L«n ph¸i chóng ta!",sImage,0)
				else
					SayImage(""..sNpcName.." Bang héi <color=green>"..GetTongName().."<color> do "..Faction.SexName[GetSex() + 1].." l·nh ®¹o thËt kh«ng thÓ xem th­êng! Lµm ®Ö tö C«n L«n thËt lµ ®¸ng mÆt mòi! Haha!",sImage,0)
				end
			else
				SayImage(""..sNpcName.." Nghe nãi sau khi ng­¬i xuèng nói ®· lËp chót c«ng danh, cã nhí ®Õn s­ ®Ö s­ muéi chóng ta kh«ng?",sImage,2, "Quay vÒ C«n L«n./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10209)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10210)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10211)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10212)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10213)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10214)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10215)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10217)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10216)
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
	Say(10207, 2, "Ta muèn gia nhËp./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(10) ~= 0) then
		Talk(1,"",10208)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10218, 2, "Rêi khái T©y Vùc./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(10) ~= 0) then
		Talk(1,"",10219)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10220, 2, "XuÊt ph¸t th«i./OnReturn", "Quªn mang theo råi./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(10) ~= 0) then
		Talk(1,"",10221)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(10)
end