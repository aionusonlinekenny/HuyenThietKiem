-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Hieu Thuoc;
Include("\\script\\system_config.lua")
GodMedicine = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/godmedicine.lua");
		Msg2Player("Reload this script");
	end;
GodMedicine:GodMedicine(10229);
end;

function GodMedicine:GodMedicine(nMsgId)
	local tbSay = {
		"Giao dﬁch./OnBuy",
		"Ta chÿ gh– ngang qua./OnCancel",
	}
	Say(nMsgId,getn(tbSay),tbSay);
end;

function OnBuy()
	local w,x,y = GetWorldPos();
	local nIndex = 0;
	TAB_COUNTRY = {
	53,		--Ba lang Huyen;
	100,	--Chu Tien Tran;
	101,	--Dao Huong Thon;
	99, 	--Vinh Lac Tran;
	121, 	--Long Mon Tran;
	153, 	--Thch Co Tran;
	174, 	--Long Tuyen Thon;
	};
	for i = 1, getn(TAB_COUNTRY) do
		if ( w == TAB_COUNTRY[i] ) then
			Sale(1);
			return
		else
			Sale(2);
			return
		end
	end
end;

function OnCancel()
end;