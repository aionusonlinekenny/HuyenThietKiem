----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Néi B¨ng Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(481, 10, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét néi b¨ng hoµn.<color>")
	return 1
end