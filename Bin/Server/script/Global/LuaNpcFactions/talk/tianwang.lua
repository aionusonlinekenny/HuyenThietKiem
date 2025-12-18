----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Thiªn V­¬ng T­íng LÜnh
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")

TianWangNpc = {}
----------------------------------
-- KiÓm tra nhiÖm vô t©n thñ th«n
----------------------------------
function TianWangNpc:OnCheckCountryTask(nWorldID)
	-- Nh©n vËt ®ang ®øng ë Ba L¨ng HuyÖn
	if(nW == 53) then
		-- KiÓm tra nhiÖm vô mua Tú Bµ Hoµn
		local n = GetTask(4)
		if( (GetByte(n,1) == 2) and (HaveEventItem(180) == 0) ) then
			Talk(1,"",10275)
			AddEventItem(180)
			local sStr = "B¹n nhËn ®­îc mét viªn <color=green>Tú Bµ Hoµn<color>."
			Msg2Player("NhiÖm Vô",sStr) 
			AddNote(sStr) 
			return 1
		end
	end
	return 0
end

----------------------------------
-- Hµm gäi chÝnh
----------------------------------
function main(nNpcIdx)
	local nW,_,_ = GetWorldPos()
	if(TianWangNpc:OnCheckCountryTask(nW) == 1) then
		return
	end
	local sPlayer_Faction = GetFaction()
	if(sPlayer_Faction == "") then
		Talk(3, "Enroll_Select", 10032, 10033, 10034)
	elseif(sPlayer_Faction == "tianwang") then
		local sNpcName = format("<color=metal>%s<color>:", GetNpcName(nNpcIdx))
		local sImage = "44/44/33"
		local n = GetTask(0)
		if(GetByte(n,1) == 0) then
			SayImage(""..sNpcName.." Ng­¬i kh«ng lo luyÖn c«ng tiÕn cÊp mµ t×m ta cã viÖc g×?",sImage,4, "XuÊt s­ rêi khái ®¶o./Leave", "T×m hiÓu khu vùc luyÖn c«ng./map_help","T×m hiÓu vâ nghÖ bæn bang./SkillHelp", "Nh©n tiÖn ghÐ qua./OnCancel")
		else
			if(GetTongID() ~= 0) then
				if(GetTongFigure() < 3) then
					SayImage(""..sNpcName.." Gia nhËp bang héi <color=green>"..GetTongName().."<color> nh­ng còng ®õng quªn trau dåi vâ c«ng bæn bang! Kim Quèc ®ang l¨m le bê câi Trung Nguyªn cho nªn sau nµy sÏ cã viÖc nhê ng­¬i gióp søc.",sImage,1, "§a t¹ s­ huynh./OnCancel")
				else
					SayImage(""..sNpcName.." L·nh ®¹o bang héi <color=green>"..GetTongName().."<color> thËt kh«ng ph¶i lµ chuyÖn dÔ! Haha! Danh väng Thiªn V­¬ng bang råi ®©y sÏ t¨ng m¹nh!",sImage,1, "§a t¹ s­ huynh./OnCancel")
				end
			else
				SayImage(""..sNpcName.." Ng­¬i cã muèn quay vÒ ®¶o ®Ó trau dåi vâ häc kh«ng?",sImage,2, "Quay vÒ Thiªn V­¬ng Bang./Return", "Nh©n tiÖn ghÐ qua./OnCancel")
			end
		end
	elseif(sPlayer_Faction == "emei") then
		Talk(1, "", 10035)
	elseif(sPlayer_Faction == "cuiyan") then
		Talk(1, "", 10036)
	elseif(sPlayer_Faction == "tangmen") then
		Talk(1, "", 10037)
	elseif(sPlayer_Faction == "wudu") then
		Talk(1, "", 10038)
	elseif(sPlayer_Faction == "shaolin") then
		Talk(1, "", 10039)
	elseif(sPlayer_Faction == "wudang") then
		Talk(1, "", 10040)
	elseif(sPlayer_Faction == "kunlun") then
		Talk(1, "", 10041)
	elseif(sPlayer_Faction == "tianren") then
		Talk(1, "", 10042)
	elseif(sPlayer_Faction == "gaibang") then
		Talk(1, "", 10043)
	end
end

----------------------------------
-- KiÓm tra gia nhËp ph¸i
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
	Say(10049, 2, "Gia nhËp Thiªn V­¬ng Bang./Go", "§Ó ta suy nghÜ kü l¹i xem./OnCancel")
end

----------------------------------
--
----------------------------------
function Go()
	if(join_main(2) ~= 0) then
		Talk(1,"",10048)
	end
end

----------------------------------
--
----------------------------------
function Leave()
	Say(10044, 2, "Ta muèn rêi khái ®¶o./OnLeave", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnLeave()
	if(leave_main(2) ~= 0) then
		Talk(1,"",10045)
	end
end

----------------------------------
--
----------------------------------
function Return()
	Say(10046, 2, "§­a ta trë l¹i ®¶o./OnReturn", "§Ó ta xem l¹i ®·./OnCancel")
end

----------------------------------
--
----------------------------------
function OnReturn()
	if(return_main(2) ~= 0) then
		Talk(1,"",10047)
	end
end

----------------------------------
--
---------------------------------
function SkillHelp()
	skill_help(2)
end