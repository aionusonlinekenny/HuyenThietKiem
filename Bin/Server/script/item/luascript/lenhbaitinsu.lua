Include("\\script\\lib\\tasklib.lua");
--Author: Kinnox;
--Date: 17/04/2022;
function OnUse(nIndex)
	-- dofile("script/item/luascript/lenhbaitinsu.lua")
	if (GetTask(T_TIN_SU) <= 0) then
		Talk(1,"","§¹i hiÖp ch­a tham gia ho¹t ®éng Thiªn b¶o khè");
		return
	end;
	
	if (GetTask(T_TIN_SU_LB) >= 1) then
		Talk(1,"","§¹i hiÖp qu¸ giµu cã, nh­ng ®é giµu cã cña ®¹i hiÖp ®· kh«ng cßn t¸c dông ë lÖnh bµi råi, ngµy mai h·y thö l¹i!");
		return
	end;
	if (GetTask(T_TIN_SU) > 0) then
		SetTask(T_TIN_SU,0);
		SetTask(T_BK_KEY_1,0);
		SetTask(T_BK_KEY_2,0);
		SetTask(T_BK_KEY_3,0);
		SetTask(T_BK_KEY_4,0);
		SetTask(T_BK_KEY_5,0);
		SetTask(T_BK_KEY_DONE_1,0);
		SetTask(T_BK_KEY_DONE_2,0);
		SetTask(T_BK_KEY_DONE_3,0);
		SetTask(T_BK_KEY_DONE_4,0);
		SetTask(T_BK_KEY_DONE_5,0);
		Talk(1,"","§¹i hiÖp sö dông vËt phÈm thµnh c«ng!. Sè lÇn tham gia ho¹t ®éng Thiªn b¶o khè hiÖn t¹i cßn l¹i lµ 1");
		SetTask(T_TIN_SU_LB,1);
	end;
	return 1;
end;