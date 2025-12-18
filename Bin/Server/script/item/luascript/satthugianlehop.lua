----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: sat thu gian
----------------------------------

----------------------------------
--
----------------------------------
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	AddTaskItem(7);
	AddTaskItem(8);
	AddTaskItem(9);
	AddTaskItem(10);
	AddTaskItem(11);
	Msg2Player("§¹i hiÖp nhËn ®­îc 1 bé s¸t thñ gi¶n.");
	return 1
end