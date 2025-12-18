----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Ngò §éc T¸n Nh©n
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

----------------------------------
--
----------------------------------
function main(nNpcIdx)
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(1, "Enroll_Select", 10077)
	elseif(sPlayer_Faction == "wudu") then
		local sNpcName = format("<color=wood>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/35"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." §éc d­îc rÊt nguy hiÓm, ng­êi míi nh­ ng­¬i sao kh«ng lo luyÖn tËp? T×m ta cã viÖc g×?",sImage,4, "Ta muèn xuÊt s­./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn m«n./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Bang héi <color=green>"..GetTongName().."<color> sÏ h­ng thÞnh nhê sù gãp mÆt cña "..Faction.SexName[GetSex() + 1].." ®ã! Cè g¾ng lªn! Haha!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				else
					SayImage(""..sNpcName.." Thµnh lËp <color=green>"..GetTongName().."<color> råi ­? ThËt kh«ng hæ danh lµ ®Ö tö Ngò §éc gi¸o! Thanh danh gi¸o ph¸i råi ®©y sÏ vang xa h¬n c¸i bän tù nhËn lµ danh m«n chÝnh ph¸i th«i! Haha",sImage,1, "§a t¹ s­ huynh./OnCancel")
				end
			else
				SayImage(""..sNpcName.." §éc m«n rÌn luyÖn kh«ng bao giê lµ ®ñ, "..Faction.SexName[GetSex() + 1].." cã muèn quay vÒ ®Ó rÌn luyÖn thªm kh«ng?",sImage,2, "Quay vÒ Ngò §éc gi¸o./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10078)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10079)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10080)
	elseif(sPlayer_Faction == "tianwang") then
		Talk(1, "", 10081)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10082)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10083)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10084)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10085)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10086)
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
	Say(10087, 2, "Gia nhËp Ngò §éc gi¸o./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(4) ~= 0) then
		Talk(1,"",10088)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10089, 2, "Xuèng nói trinh s¸t./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(4) ~= 0) then
		Talk(1,"",10090)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10091, 2, "TÊt nhiªn lµ cã./OnReturn", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(4) ~= 0) then
		Talk(1,"",10092)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(4)
end