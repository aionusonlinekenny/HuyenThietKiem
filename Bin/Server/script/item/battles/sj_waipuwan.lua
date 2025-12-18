----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Ngo¹i Phæ Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(487, 5, 1, 3240) 
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét ngo¹i phæ hoµn.<color>")
	return 1
end