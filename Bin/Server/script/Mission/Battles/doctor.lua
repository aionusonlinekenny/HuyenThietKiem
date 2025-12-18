----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Qu©n Nhu Tèng Kim
----------------------------------

----------------------------------
--
----------------------------------
function main()
	local sInfo = "HËu doanh do ta phôc tr¸ch. Ng­¬i cÇn ta gióp g×??"
	local tbSay = {
		"Mua thuèc./OnTrade",
		-- "NhËn qu©n l­¬ng./OnGetRice.",
		"Mua 30 b×nh Ngò hoa ngäc lé hoµn[5000l]/Buyblood",
		"Kh«ng cã g× c¶./OnCancel",
	}
	Say(sInfo,getn(tbSay),tbSay)
end;

----------------------------------
--
----------------------------------
function OnTrade()
	Sale(29)
end;

----------------------------------
--
----------------------------------
function OnGetRice()
	-- FindEmptyPlace(1,1)
end;
function Buyblood()
	local nNeedCash = 5000;
	local nNum = 30;
	if (GetCash() < nNeedCash) then
	 Talk(1,"","Ta ®· giao th­¬ng ph¸ gi¸ thÞ tr­êng råi, ng­¬i kh«ng cã ®ñ 5000 l­îng sao?")
	return end;
	for i = 1,nNum do
		AddItem(5,8,0,5,0,0,0);
	end;
	Pay(nNeedCash);
end;
----------------------------------
--
----------------------------------
function OnCancel()
end;