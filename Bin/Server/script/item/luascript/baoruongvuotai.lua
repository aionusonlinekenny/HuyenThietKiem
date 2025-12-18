Include("\\script\\lib\\libevent.lua")
function OnUse(nNpcIndex)
	if (FindEmptyPlace(6,6) == 0) then
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0;
	end
	
	if (BaoRuongVuotAi() == 1) then
		return 1;
	end;
	return 0;
end