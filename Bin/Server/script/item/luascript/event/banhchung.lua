Include("\\script\\lib\\tasklib.lua");	
Include("\\script\\event\\banhchung\\head.lua");
function OnUse(nIdx)

	if (GetTask(T_EVENT_MAX2) >= MAX_EVENT) then 
		Msg2Player("§¹i hiÖp lµ bËc kú tµi trong thiªn h¹, ®· thu thËp vµ sö dông hÕt b¸nh ch­ng, mêi ®¹i hiÖp vÒ LÔ Quang nhËn quµ");
		return
	end;

	local EarExp = 1500000;
	local TAB_TaskItem = {
		12,15,16,17,18,22,23,24,25,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59
	}
	local TAB_ScriptItem ={
		3,4,5,6,11,12,13,14,15,16,131,
	}
	local TAB_HorseItem_Limited ={
		{0,10,6,10},
		{0,10,8,10},
		{0,10,7,10}
	}
	local TAB_HorseItem_UnLimited ={
		{0,10,6,10},
		{0,10,8,10},
		{0,10,7,10}
	}


	local nRanTAB = RANDOMC(1,2,3);
	local nRanItem = RANDOMC(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40);
	local bLucky = RANDOMC(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30);
	if (bLucky <= 10) then
		if (nRanTAB == 1) then
			AddTaskItem(TAB_TaskItem[RANDOM(1,getn(TAB_TaskItem))]);
		elseif (nRanTAB == 2) then
			AddScriptItem(TAB_ScriptItem[RANDOM(1,getn(TAB_ScriptItem))]);
		else
			if (nRanItem == 18) then
				local nRanID = RANDOM(1,getn(TAB_HorseItem_Limited));
				AddItem(0,TAB_HorseItem_Limited[nRanID][2],TAB_HorseItem_Limited[nRanID][3],10,random(0,4),10,0,0,0,0,0,0,RANDOMC(5,7,15,30));
			end;
		end;
		Msg2Player("Sö dông b¸nh tÐt nhËn ®­îc <color=green> mét vËt phÈm ®Æc biÖt <color> !");
	end;
	AddOwnExp(EarExp);
	SetTask(T_EVENT_MAX2,GetTask(T_EVENT_MAX2)+1);
	Msg2Player("Sö dông b¸nh ch­ng nhËn ®­îc 1.500.000 ®iÓm kinh nghiªm!")
	if (GetTask(T_EVENT_MAX2) == MAX_EVENT) then 
		Msg2SubWorld("<color=pink> Chóc mõng ®¹i hiÖp "..GetName().." ®· sö dông Max event b¸nh ch­ng cña m¸y chñ! <color>")
	end;
	return 1
end