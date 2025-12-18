----------------------------------
--	Copyright: JX Online by Kinnox;
--	Author: [Kinnox]
--	Date: 23/08/2021
--	Desc: Thæ ®Þa phï v« h¹n
----------------------------------
Include("\\script\\lib\\Revivepos.lua")
function OnUse(nIdx)  

	local nW, nX, nY = GetWorldPos()
	
	if (nW == 341) or (nW == 342) or (nW == 242)then	
		-- nothing;
	else 
		if CheckMapNoForTHP(nW) == 1  or GetTask(601) > 0 then
		Talk(1,"", "N¬i nµy kh«ng thÓ sö dông <color=green>Thæ ®Þa phï <color>.")	
		return end	
	end;
	

	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	
	if(GetTownPortalFlag() ~= 0) then
		Msg2Player("N¬i nµy kh«ng thÓ sö dông <color=green>Thæ ®Þa phï <color>.")
		return 0
	end
		
	if(GetFightState() == 0) then 
		Msg2Player("Tr¹ng th¸i phi chiÕn ®Êu kh«ng thÓ sö dông <color=green>Thæ ®Þa phï <color>.")
		return 0
	end
	UseTownPortal()
	AddSkillState(963, 1, 0, 18*3)
	return 0
end