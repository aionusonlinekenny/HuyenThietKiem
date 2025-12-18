----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: nhac vuong kiem
----------------------------------

----------------------------------
--
----------------------------------
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	AddTaskItem(13);
	Msg2Player("§¹i hiÖp nhËn ®­îc 1 Nh¹c v­¬ng kiÕm, vËt phÈm quı ®Ó thµnh lËp bang héi");
	return 1
end