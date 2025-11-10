Include("\\script\\lib\\tasklib.lua");
Include("\\script\\mission\\phonglangdo\\head.lua");
--Author: Kinnox;
--Date: 17/04/2022;
function OnUse(nIndex)
	-- dofile("script/item/luascript/hailongchau.lua")
	if (GetTask(PLD.TASK.TPLD_JOIN_BOAT) <= 0) then
		Talk(1,"","§¹i hiÖp ch­a tham gia phong l¨ng ®é");
		return
	end;
	if (GetTask(PLD.TASK.TPLD_JOIN_BOAT) > 0) then
		SetTask(PLD.TASK.TPLD_JOIN_BOAT,GetTask(PLD.TASK.TPLD_JOIN_BOAT) - 1);
		Talk(1,"","§¹i hiÖp sö dông vËt phÈm thµnh c«ng!. Sè lÇn tham gia phong l¨ng ®é hiÖn t¹i cßn l¹i lµ "..(3-GetTask(PLD.TASK.TPLD_JOIN_BOAT)).."");
	end;
	return 1;
end;