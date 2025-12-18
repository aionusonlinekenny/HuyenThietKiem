Include("\\script\\lib\\tasklib.lua");
ERROR_1 = "Nhµ ng­¬i kh«ng cã ®ñ <color=green>bã cñi<color>"
ERROR_2 = "Nhµ ng­¬i kh«ng cã ®ñ <color=green>nåi b¸nh<color>"
ERROR_3 = "Nhµ ng­¬i kh«ng cã ®ñ <color=green>bao nÕp<color>"
ERROR_4 = "Nhµ ng­¬i kh«ng cã ®ñ <color=green>l¸ gãi<color>"
MAX_EVENT = 500;

function banhchung()
	local tbSay = {
		"T×m hiÓu c«ng thøc/Help",
		"GhÐp b¸nh tÐt/ghepbanh#1",
		"GhÐp b¸nh ch­ng/ghepbanh#2",
		"NhËn quµ Max b¸nh tÐt/AwardEvent#1",
		"NhËn quµ Max b¸nh ch­ng/AwardEvent#2",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("<color=orange>LÔ Quan: <color>ta chØ b¸n nh÷ng vËt phÈm quý hiÕm víi ­u ®·i lín dµnh cho c¸c vÞ phó gia thËt sù, nhµ ng­¬i còng muèn mua sao? <pic=121> ",getn(tbSay),tbSay);
end;

function Help()
	Talk(1,"","<color=orange>LÔ Quan: <color> C«ng thøc ghÐp b¸nh TÐt : Bã cñi + Nåi b¸nh + Bao nÕp + 1 v¹n = 1 b¸nh tÐt\n C«ng thøc ghÐp b¸nh ch­ng: Bã cñi + Nåi b¸nh + Bao nÕp + 5000 l­îng + l¸ gãi(1XU) = 1 b¸nh ch­ng")
end;

function ghepbanh(_,nNumber)
	local nNumber = tonumber(nNumber);
	if (nNumber == 1) then
		AskClientForNumber("Executerbanhtet",1,100,"NhËp sè l­îng")
	else
		AskClientForNumber("Executerbanhchung",1,100,"NhËp sè l­îng")
	end;
end;

function Executerbanhtet(_,nNumber)
	local nNumber = tonumber(nNumber);
	local nNeedCash = 10000;
	if (nNumber < 1) then
		return
	end;
	if (GetTaskItemCount(60) < nNumber) then
		Msg2Player(ERROR_1);
		return
	end;
	if (GetTaskItemCount(61) < nNumber) then
		Msg2Player(ERROR_2);
		return
	end;
	if (GetTaskItemCount(62) < nNumber) then
		Msg2Player(ERROR_3);
		return
	end;
	if (GetCash() < nNeedCash * nNumber) then
		Msg2Player("Nhµ ng­¬i kh«ng cã ®ñ "..(nNeedCash * nNumber).." l­îng b¹c");
		return
	end;
	if (nNumber == 1) then
		AddScriptItem(133);
	else
		for i = 1, nNumber do
			AddScriptItem(133);
		end;
	end;
	DelTaskItem(60,nNumber);
	DelTaskItem(61,nNumber);
	DelTaskItem(62,nNumber);
	Pay(nNeedCash * nNumber);
	Msg2Player("Chóc mõng ®¹i hiÖp ghÐp thµnh c«ng <color=green>x"..nNumber.." b¸nh tÐt<color=green> ")
end;

function Executerbanhchung(_,nNumber)
	local nNumber = tonumber(nNumber);
	local nNeedCash = 5000;
	if (nNumber < 1) then
		return
	end;
	if (GetTaskItemCount(60) < nNumber) then
		Msg2Player(ERROR_1);
		return
	end;
	if (GetTaskItemCount(61) < nNumber) then
		Msg2Player(ERROR_2);
		return
	end;
	if (GetTaskItemCount(62) < nNumber) then
		Msg2Player(ERROR_3);
		return
	end;
	if (GetTaskItemCount(63) < nNumber) then
		Msg2Player(ERROR_4);
		return
	end;
	if (GetCash() < nNeedCash * nNumber) then
		Msg2Player("Nhµ ng­¬i kh«ng cã ®ñ "..(nNeedCash * nNumber).." l­îng b¹c");
		return
	end;
	if (nNumber == 1) then
		AddScriptItem(134);
	else
		for i = 1, nNumber do
			AddScriptItem(134);
		end;
	end;
	DelTaskItem(60,nNumber);
	DelTaskItem(61,nNumber);
	DelTaskItem(62,nNumber);
	DelTaskItem(63,nNumber);
	Pay(nNeedCash * nNumber);
	Msg2Player("Chóc mõng ®¹i hiÖp ghÐp thµnh c«ng <color=green>x"..nNumber.." b¸nh ch­ng<color=green> ");
end;

function AwardEvent(_,nNumber)
	local nNumber = tonumber(nNumber);
	if (nNumber == 1) then
		if (GetTask(T_EVENT_MAX1) < MAX_EVENT) then
			Msg2Player("Nhµ ng­¬i chua thÓ nhËn phÇn th­ëng nµy");
			return
		elseif (GetTask(T_EVENT_MAX1) > MAX_EVENT) then
			Msg2Player("§¹i hiÖp ®· nhËn phÇn th­ëng nµy råi!");
			return	
		end;
		AddGoldItem(random(204,207));
		SetTask(T_EVENT_MAX1,GetTask(T_EVENT_MAX1)+1);
		Msg2Player("Chóc mõng ®¹i hiÖp nhËn th­ëng thµnh c«ng <color=green>MAX EVENT B¸NH TÐT<color=green> ");
	else
		if (GetTask(T_EVENT_MAX2) < MAX_EVENT) then
				Msg2Player("Nhµ ng­¬i chua thÓ nhËn phÇn th­ëng nµy");
			return
		elseif (GetTask(T_EVENT_MAX2) > MAX_EVENT) then
			Msg2Player("§¹i hiÖp ®· nhËn phÇn th­ëng nµy råi!");
			return	
		end;
			local szAward = {
				"Hång ¶nh ThÈm Viªn UyÓn/AwardVip#204",
				"Hång ¶nh KiÕm Bµi/AwardVip#205",
				"Hång ¶nh Môc Tóc/AwardVip#206",
				"Hång ¶nh Tô Chiªu/AwardVip#207",
				"Ta ch­a muèn nhËn lóc nµy/OnCancel",
			}
			Say("Mêi ®¹i hiÖp chän phÇn th­ëng muèn nhËn!",getn(szAward),szAward);
	end;
end;

function AwardVip(_,nNumber)
	local nNumber = tonumber(nNumber);
	AddGoldItem(nNumber);
	Msg2Player("Chóc mõng ®¹i hiÖp nhËn th­ëng thµnh c«ng <color=green>MAX EVENT B¸NH CH¦NG<color=green>");
	SetTask(T_EVENT_MAX2,GetTask(T_EVENT_MAX2)+1);
end;

function OnCancel()

end;