----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Ngo¹i B¨ng Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(486, 10, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét ngo¹i b¨ng hoµn.<color>")
	return 1 
end