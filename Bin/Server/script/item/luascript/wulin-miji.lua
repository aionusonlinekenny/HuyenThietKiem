----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: Vâ l©m mËt tŞch
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
		Talk(1,"","Ch­a ®¹t <color=red>®¼ng cÊp 80<color>, kh«ng thÓ sö dông Vâ L©m MËt TŞch.")
		return 0
	end
	local nCurPoint = GetTask(T_VLMT) + 1
	if(nCurPoint > 15) then
		Talk(1,"","§· tu luyÖn ®ñ <color=green>15 quyÓn<color> Vâ L©m MËt TŞch! Kh«ng thÓ tiÕp tôc tu luyÖn!")
		return 0
	end
	SetTask(T_VLMT, nCurPoint);
	AddMagicPoint(1)
	Talk(1,"","Tu luyÖn nöa ngµy nhËn ®­îc <color=green>1 ®iÓm kü n¨ng<color>! §· tu luyÖn <color=red>"..nCurPoint.." quyÓn<color>.")
	return 1
end