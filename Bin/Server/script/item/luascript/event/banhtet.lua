Include("\\script\\lib\\tasklib.lua");	
Include("\\script\\event\\banhchung\\head.lua");
function OnUse(nIdx)

	if (GetTask(T_EVENT_MAX1) >= MAX_EVENT) then 
		Msg2Player("§¹i hiÖp lµ bËc kú tµi trong thiªn h¹, ®· thu thËp vµ sö dông hÕt b¸nh tÐt, mêi ®¹i hiÖp vÒ LÔ Quang nhËn quµ");
		return
	end;

	local EarExp = 1000000;
	local TAB_TaskItem = {
		12,15,16,17,18,59
	}
	local TAB_ScriptItem ={
		131,11,12,13
	}

	local nRanTAB = RANDOMC(1,2);
	local bLucky = RANDOMC(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30);
	if (bLucky <= 5) then
		if (nRanTAB == 1) then
			AddTaskItem(TAB_TaskItem[RANDOM(1,getn(TAB_TaskItem))]);
		else
			AddScriptItem(TAB_ScriptItem[RANDOM(1,getn(TAB_ScriptItem))]);
		end;
		Msg2Player("Sö dông b¸nh tÐt nhËn ®­îc <color=green> mét vËt phÈm ®Æc biÖt <color> !");
	end;
	AddOwnExp(EarExp);
	SetTask(T_EVENT_MAX1,GetTask(T_EVENT_MAX1)+1);
	Msg2Player("Sö dông b¸nh tÐt nhËn ®­îc 1.000.000 ®iÓm kinh nghiªm!");
	if (GetTask(T_EVENT_MAX1) == MAX_EVENT) then 
		Msg2SubWorld("<color=pink> Chóc mõng ®¹i hiÖp "..GetName().." ®· sö dông Max event b¸nh tÐt cña m¸y chñ! <color>")
	end;
	return 1
end