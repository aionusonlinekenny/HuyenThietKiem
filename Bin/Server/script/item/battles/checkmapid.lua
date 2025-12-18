----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: KiÓm tra b¶n ®å Tèng Kim
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function OnUse(nItemIdx) 
	local nMapId = SubWorldIdx2ID(SubWorld)
	if(nMapId == ZBattle.FIGHT_MAP) then
		return EatMedicine(nItemIdx)
	end 
	Msg2Player("<color=orangered>§¹o cô nµy chØ ®­îc phÐp sö dông ë chiÕn tr­êng.<color>")
	return 0
end
