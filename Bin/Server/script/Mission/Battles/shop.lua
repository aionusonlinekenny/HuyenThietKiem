----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: B∏n thuËc lag
----------------------------------

----------------------------------
--
----------------------------------
function main()
	local sInfo = "C∏c dÚng s‹ n≠Ìc Kim c„ th” dÔng Æi”m t›ch lÚy Æ” ÆÊi l y <color=yellow>Æi”m kinh nghi÷m, Nhπc V≠¨ng hÂn thπch<color> vµ c„ th” mua <color=yellow>Æπo cÙ TËng Kim<color>. N’u ÆÒ sË l≠Óng Nhπc v≠¨ng hÂn thπch c„ th” ÆÊi l y Nhπc V≠¨ng Ki’m! Ng≠¨i c„ c«n g◊ kh´ng? "
	local tbSay = {
		"Ta muËn mua Æπo cÙ TËng Kim/OnTrade",
		"Ta muËn ÆÊi Æi”m t›ch lÚy TËng Kim l y Æi”m kinh nghi÷m/CovertPointToExp",
		"DÔng Nhπc V≠¨ng HÂn Thπch ÆÊi l y Nhπc V≠¨ng Ki’m/Help",
		"Xem Æi”m t›ch lÚy/CheckPoint",
		"Mua L÷nh bµi (100v 10 cai)/shoplb",
		-- "Mua Æ∏ –p ÆÂ t›m/OnMine",
		-- "Mua b…y gÀp [500 Æi”m]/OnEvent",
		"ß” ta suy ngh‹ lπi/OnCancel",
	}
	Say(sInfo,getn(tbSay),tbSay)
end
function OnEvent()
	if(GetTask(2013) < 500) then
	Talk(1,"","Kh´ng ÆÒ Æi”m t›ch lu¸")
		return
	end
	-- AddEventItem(63,2017051514)
	SetTask(2013,GetTask(2013)-500)
end
function OnMine()
	local tb_da = {
		"ß∏ hi÷n 1/OnShow#2",
		"ß∏ hi÷n 2/OnShow#4",
		"ß∏ hi÷n 3/OnShow#6",
		"ß∏ »n 1/OnShow#3",
		"ß∏ »n 2/OnShow#5",
		"ß∏ »n 3/OnShow#7",
		"Kh´ng giao dﬁch./OnCancel",
	}
	Say("Ng≠¨i muËn mua loπi Æ∏ nµo?, mÁi vi™n c„ gi∏ 400 Æi”m tËng kim",getn(tb_da),tb_da)
end
function OnShow(nKind)
	if(GetTask(2013) < 400) then
	Talk(1,"","Kh´ng ÆÒ Æi”m t›ch lu¸")
		return
	end
	AddMineItem(nKind,0,-1,0)
	SetTask(2013,GetTask(2013)-400)
	
end
----------------------------------
--
----------------------------------
function OnTrade()
	Sale(28)
end
----------------------------------
--
----------------------------------
function CovertPointToExp()
	Talk(1,"","1000 Æi”m t›ch lÚy TËng kim = 100.000 Æi”m kinh nghi÷m , ßi”m t›ch lÚy hi÷n tπi cÒa nhµ ng≠¨i lµ: <color=green> "..GetTask(2013).." <color> Æi”m");
	local Point = GetTask(2013);
	if (Point < 1E3) then
		Msg2Player("ßi”m tËng kim kh´ng ÆÒ 1000 Æi”m kh´ng th” ÆÊi");
	elseif (Point > 1000) then
		SetTask(2013,Point - 1E3);
		AddOwnExp(1E5);
		Msg2Player("ChÛc mıng Æπi hi÷p ÆÊi thµnh c´ng 100.000 Æi”m kinh nghi÷m");
	end;

end;
----------------------------------
--
----------------------------------
function CheckPoint()
	Talk(1,"","ßi”m t›ch lÚy hi÷n tπi cÒa nhµ ng≠¨i lµ: <color=green> "..GetTask(2013).." <color> Æi”m");
end;
----------------------------------
--
----------------------------------
function Help()
	Talk(1,"","T›nh n®ng nµy ch≠a ph∏t tri”n!");
end;
----------------------------------
--
----------------------------------
function shoplb()
	local nNeedCash = 1000000;
	if (GetCash() < nNeedCash) then
		Talk(1,"","ng≠Íi kh´ng c„ ÆÒ ti“n");
		return
	end;
	for i = 1,10 do
		AddScriptItem(82);
	end;
	Pay(nNeedCash);
end;
----------------------------------
--
----------------------------------
function OnCancel()
end