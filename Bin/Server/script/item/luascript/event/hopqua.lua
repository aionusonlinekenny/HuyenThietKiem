	
function OnUse(nIdx)
	local szEvent ={
		"bã cñi",
		"nåi b¸nh",
		"bao nÕp",
	}
	local nRanEvent = random(60,62);	
	if (nRanEvent == 60) then
		Msg2Player("§¹i hiÖp nhËn ®­îc <color=green>bã cñi<color> ");	
	elseif (nRanEvent == 61) then
		Msg2Player("§¹i hiÖp nhËn ®­îc <color=green>nåi b¸nh<color> ");	
	else
		Msg2Player("§¹i hiÖp nhËn ®­îc <color=green>bao nÕp<color> ");	
	end;
	AddTaskItem(nRanEvent);
	return 1
end
