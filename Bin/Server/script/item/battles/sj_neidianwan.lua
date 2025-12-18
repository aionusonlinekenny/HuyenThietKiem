----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Néi L«i Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(483, 10, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét néi l«i hoµn.<color>")
	return 1
end