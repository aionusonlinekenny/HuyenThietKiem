-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Than bi thuong nhan;
Include("\\script\\system_config.lua")
tbthuongnhan = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/thanbithuongnhan.lua");
		Msg2Player("Reload this script");
	end;
tbthuongnhan:tbthuongnhan(10228);
end;

function tbthuongnhan:tbthuongnhan(nMsgId)
	local tbSay = {
		"Cöa hµng trao ®æi Phóc Duyªn./OnBuy",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("Ta lµ th­¬ng nh©n T©y Vùc, chuyªn b¸n nh÷ng vËt ph¶m quý hiÕm, ng­¬i cø tõ tõ mµ lùa chän.",getn(tbSay),tbSay);
end;

function OnBuy()
	Sale(31);
end;

function OnCancel()
end;