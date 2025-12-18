----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: B¨ng Phßng Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(475, 5, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét b¨ng phßng hoµn.<color>")
	return 1
end