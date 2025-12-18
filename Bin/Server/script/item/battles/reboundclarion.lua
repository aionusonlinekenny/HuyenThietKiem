----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Kh¸ng ®¬n chi gi¸c
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	CastSkill(510, 1)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét kh¸ng ®¬n chi gi¸c.<color>")
	return 1
end
