-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Ban ngua;
Include("\\script\\system_config.lua")
HorseSale = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/horsesale.lua");
		Msg2Player("Reload this script");
	end;
HorseSale:HorseSale(10344);
end;

function HorseSale:HorseSale(nMsgId)
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
		nIndex = 20;
	elseif (w == 176) then
		nIndex = 21;
	elseif (w == 162) then
		nIndex = 19;
	elseif (w == 80) then
		nIndex = 22;
	elseif (w == 37) then
		nIndex = 23;
	elseif (w == 1) then
		nIndex = 23;
	elseif (w == 78) then
		nIndex = 19;
	else
		nIndex = 19;
	end
	Sale(nIndex);
end;

function OnCancel()
end;
