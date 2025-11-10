----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Phi Tèc Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(496, 5, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét phi tèc hoµn.<color>")
	return 1
end