----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Néi §éc Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(480, 5, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét néi ®éc hoµn.<color>")
	return 1
end