----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Cao ThiÓm Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(494, 50, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét cao thiÓm hoµn.<color>")
	return 1
end