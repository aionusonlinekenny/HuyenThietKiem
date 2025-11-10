-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tap Hoa;
Include("\\script\\system_config.lua")
Grocery = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/grocery.lua");
		Msg2Player("Reload this script");
	end;
Grocery:Grocery(10228);
end;

function Grocery:Grocery(nMsgId)
	local tbSay = {
		"Giao dﬁch./OnBuy",
		"Ta chÿ gh– ngang qua./OnCancel",
	}
	Say(nMsgId,getn(tbSay),tbSay);
end;

function OnBuy()
	local w,x,y = GetWorldPos();
	local nIndex = 0;
	if (w == 11) then
		nIndex = 4;
	elseif (w == 176) then
		nIndex = 5;
	elseif (w == 162) then
		nIndex = 6;
	elseif (w == 80) then
		nIndex = 7;
	elseif (w == 37) then
		nIndex = 8;
	elseif (w == 1) then
		nIndex = 9;
	elseif (w == 78) then
		nIndex = 10;
	else
		nIndex = 3;
	end
	Sale(nIndex);

end;

function OnCancel()
end;