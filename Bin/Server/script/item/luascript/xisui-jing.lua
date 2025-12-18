----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: TÈy tñy kinh
----------------------------------
Include("\\script\\lib\\TaskLib.lua")
----------------------------------
--
----------------------------------
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	if(GetLevel() < 80) then
		Talk(1,"","Ch­a ®¹t <color=red>®¼ng cÊp 80<color>, kh«ng thÓ sö dông TÈy Tñy Kinh.")
		return 0
	end
	local nCurPoint = GetTask(T_TTK) + 5;
	if(nCurPoint > 75) then
		Talk(1,"","§· tu luyÖn ®ñ <color=green>15 quyÓn<color> TÈy Tñy Kinh! Kh«ng thÓ tiÕp tôc tu luyÖn!")
		return 0
	end
	SetTask(T_TTK,nCurPoint)
	AddProp(5)
	Talk(1,"","Tu luyÖn nöa ngµy nhËn ®­îc <color=green>5 ®iÓm tiÒm n¨ng<color>! §· tu luyÖn <color=red>"..(nCurPoint/5).." quyÓn<color>.")
	return 1
end