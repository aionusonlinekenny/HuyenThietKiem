Include("\\script\\lib\\tasklib.lua")
MAX_HUYHOANG = 5
MAX_HOANGKIM = 1
function OnUse(nIdx)
	local Msg = "H«m nay ®¹i hiÖp ®· ¨n <color=yellow> ®ñ sè qu¶<color>, vâ c«ng th¨ng tiÕn qu¸ nhanh dÔ g©y t¸c dông ph¶n nghÞch, ngµy mai h·y tiÕp tôc vËy"
	if(GetHandItem() > 0) then
		Talk(1,"","<color=red>Trªn tay cã vËt phÈm, kh«ng thÓ thao t¸c.<color>")
		return 0
	end
	local szfruitlname = "";
	local award_exp = 0;
	local nPlayerLevel = GetLevel();	
	local _, nD, _ = GetItemProp(nIdx);
	
	if (nD >= 7 and nD <=9) then
		if (GetTask(T_LIMIT_HUYHOANG) > MAX_HUYHOANG) then
			Talk(1,"",Msg);
		return 0;
		end;
	else
		if (GetTask(T_LIMIT_HOANGKIM) > MAX_HOANGKIM) then
			Talk(1,"",Msg);
		return 0;
		end;
	end;
	
	
	if (nD == 7) then
		szfruitlname = "H¹t huy hoµng s¬ cÊp"
		if (nPlayerLevel < 80) then
			award_exp = 500000;
		else
			Msg2Player(""..szfruitlname.." dµnh cho nh÷ng kÎ v« danh tiÓu tèt cã ®¼ng cÊp d­íi 80");
			return 0;
		end;
	elseif (nD == 8) then
		szfruitlname = "H¹t huy hoµng trung cÊp"
		if (nPlayerLevel >= 80 and nPlayerLevel < 100) then
			award_exp = 1500000;
		else
			Msg2Player(""..szfruitlname.." dµnh cho nh÷ng ®¹i hiÖp cã ®¼ng cÊp tõ 80 ®Õn 100");
			return 0;
		end;
	elseif (nD == 9) then
		szfruitlname = "H¹t huy hoµng cao cÊp"
		if (nPlayerLevel >= 100 ) then
			award_exp = 2500000;
		else
			Msg2Player(""..szfruitlname.." dµnh cho nh÷ng ®¹i hiÖp cã ®¼ng cÊp tõ 100 trë lªn");
			return 0;
		end;
	else
		szfruitlname = "H¹t hoµng kim"
		if (nPlayerLevel >= 100) then
			award_exp = 5500000;
		else
			Msg2Player(""..szfruitlname.." dµnh cho nh÷ng ®¹i hiÖp cã ®¼ng cÊp tõ 100 trë lªn");
			return 0;
		end;
	end;
	
	AddOwnExp(award_exp);
	if (nD >= 7 and nD <= 9) then
		SetTask(T_LIMIT_HUYHOANG,GetTask(T_LIMIT_HUYHOANG)+1);
		return 1
	else
		SetTask(T_LIMIT_HOANGKIM,GetTask(T_LIMIT_HOANGKIM)+1);
		return 1
	end;	
	
end;
