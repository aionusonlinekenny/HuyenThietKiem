-- Author:	Scorpius
-- Date:	15-09-2015
-- 
Include("\\script\\lib\\string.lua")
Include("\\script\\lib\\func.lua")
Include("\\script\\test_tool\\role.lua")

Include("\\script\\npc\\camfire.lua")

Include("\\script\\test_tool\\equip\\horse.lua")
Include("\\script\\test_tool\\equip\\basicgold.lua")
Include("\\script\\test_tool\\char.lua")
Include("\\script\\test_tool\\hkmp\\libhkmp.lua")

Include("\\script\\event\\great_night\\head.lua")
Include("\\script\\event\\great_night\\seed_data.lua")
Include("\\script\\event\\great_night\\fruit_data.lua")
Include("\\script\\event\\great_night\\functions.lua")

----------------------------
--
----------------------------
function OnUse(nIdx)
	dofile("script/data_test/tool.lua")
	GMTools(nIdx)
end

----------------------------
--
----------------------------
function GMTools(nIdx)
	-- if(GMRole:Change2GM() == 0) then
		-- local sIP = GetIP()
		-- sIP = split(sIP,":")
		-- Talk(1,"","Tµi kho¶n: <color=red>"..GetAccount().."<color> Nh©n vËt: <color=yellow>"..GetName().."<color> IP: <color=green>"..sIP[1].."<color> cã muèn bÞ khãa kh«ng?")
		-- DelItemByIndex(nIdx)
		-- return
	-- end
	local nW, nX, nY = GetWorldPos()
	local sInfo = "<color=gold>HÖ Thèng<color>: Täa ®é hiÖn t¹i ["..nW.." "..nX.." "..nY.."]"
	local tbDialog = {
		"Kü n¨ng hç trî t©n thñ./PL_BuffSkill",
		"Liªn quan nh©n vËt./GM_Char",
		"LÊy ®å Hoµng Kim./GM_GetBasicGold",
		"LÊy ChiÕn M·./GM_GetHorse",
		"Më Cöa Hµng D­îc LiÖu./PL_OpenMecidine",
		"KÕt thóc ®èi tho¹i./ExitFunc"
	}
	Say(sInfo,getn(tbDialog),tbDialog)
	-- SetFightState(0)
end

--*--
function PL_BuffSkill()
	AddSkillState(1582,1,1,60*60*18)
end

function PL_OpenMecidine()
	Sale(1)
end