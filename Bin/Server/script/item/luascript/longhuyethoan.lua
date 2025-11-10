Include("\\script\\lib\\tasklib.lua");
Include("\\script\\mission\\vuotai\\head.lua");
--Author: Kinnox;
--Date: 17/04/2022;
function OnUse(nIndex)
	-- dofile("script/item/luascript/longhuyethoan.lua")
	if (GetTask(VA.TASK.TVA_JOIN_BOAT) <= 0) then
		Talk(1,"","§¹i hiÖp ch­a tham gia v­ît ¶i");
		return
	end;
	if (GetTask(VA.TASK.TVA_JOIN_BOAT) > 0) then
		SetTask(VA.TASK.TVA_JOIN_BOAT,GetTask(VA.TASK.TVA_JOIN_BOAT) - 1);
		Talk(1,"","§¹i hiÖp sö dông vËt phÈm thµnh c«ng!. Sè lÇn tham gia v­ît ¶i hiÖn t¹i cßn l¹i lµ "..(3-GetTask(VA.TASK.TVA_JOIN_BOAT)).."");
	end;
	return 1;
end;