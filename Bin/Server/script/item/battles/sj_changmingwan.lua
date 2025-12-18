----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Tr­êng MÖnh Hoµn
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine() 
	AddSkillState(488, 5, 1, 3240)
	Msg2Player("<color=OrangeRed>§¹i hiÖp sö dông mét tr­êng mÖnh hoµn.<color>")
	return 1
end