----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: ThiÕt la h¸n
----------------------------------

----------------------------------
--
----------------------------------
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local nLevel = GetLevel();
	if(nLevel > 130) then
		Talk(1,"","§¼ng cÊp <color=green>trªn 130<color> kh«ng thÓ sö dông ThiÕt La H¸n!")
		return 0
	end
	
	AddOwnExp(300000)
	Msg2Player("B¹n ®¸nh nhau víi ThiÕt La H¸n n÷a ngµy cuèi cïng còng cã mét İt kinh nghiÖm.") 
	
	return 1
end