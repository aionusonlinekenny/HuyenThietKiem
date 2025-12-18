----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: M·nh SHXT
----------------------------------
Include("\\script\\lib\\tasklib.lua");
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	SetTask(TASK_DOCHI,GetTask(TASK_DOCHI)+1000);
	Msg2Player("Sè m·nh S¬n hµ x· t¾t cña ng­¬i t¨ng lªn "..GetTask(TASK_DOCHI).."");
	
	return 1
end