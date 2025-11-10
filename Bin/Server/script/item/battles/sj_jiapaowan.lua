----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Gia Bµo Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(493, 2, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét gia bµo hoµn.<color>")
	return 1
end