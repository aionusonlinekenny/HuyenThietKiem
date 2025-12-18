-- Author:	Kinnox;
-- Date:	21-05-2023
-- Functions: Shop Vip;
Include("\\script\\system_config.lua")
ShopVip = {};
Include("\\script\\lib\\tasklib.lua");
function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/shopvip.lua");
		Msg2Player("Reload this script");
	end;
	local tbSay = {
		"T×m hiÓu gãi VIP/Help",
		"NhËn th­ëng VIP 1/VIP1",
		"NhËn th­ëng VIP 2/VIP2",
		"NhËn th­ëng VIP 3/VIP3",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("<color=orange>Th­¬ng Nh©n T©y Vùc: <color>ta chØ b¸n nh÷ng vËt phÈm quý hiÕm víi ­u ®·i lín dµnh cho c¸c vÞ phó gia thËt sù, nhµ ng­¬i còng muèn mua sao? <pic=121> ",getn(tbSay),tbSay);
end;

function Help()
	Talk(4,"","<color=orange>Th­¬ng Nh©n T©y Vùc: <color> C¸c h¹ cã thÓ tïy chän mét trong ba gãi vip ®Ó tiÕt kiÖm chi phÝ! <color=green> H·y chia sÏ g¸nh nÆng duy tr× m¸y chñ víi BQT, ®¹i hiÖp sÏ cã mét s©n ch¬i c«ng b»ng - v÷ng m¹nh - v¨n minh <color> <pic=122> ",
		"<color=orange>VIP 1 gi¸ 69k <color> <pic=121> : Mçi ngµy nhËn ®­îc 2 STG lÔ hép, 2 LB PLD, 3 Háa tèc triÒu lÖnh, 3 QuÕ hoa töu, 10 v¹n l­îng",
		"<color=orange>VIP 2 gi¸ 89k <color> <pic=121> : §¹i thµnh bÝ kÝp 90, Tói m¸u vÜnh viÔn, Mçi ngµy nhËn ®­îc 3 STG lÔ hép, 3 LB PLD, 3 Háa tèc triÒu lÖnh, 3 QuÕ hoa töu, 20 v¹n l­îng",
		"<color=orange>VIP 3 gi¸ 139k <color> <pic=121> : 3 VLMT, 3 TTT, §TBK 90, §TBK 120, Tói m¸u vÜnh viÔn, Mçi ngµy nhËn ®­îc 3 STG lÔ hép, 3 LB PLD, 5 Háa tèc triÒu lÖnh, 3 QuÕ hoa töu, 20 v¹n l­îng")
end;

function VIP1()
	local nVIP1 = GetTask(T_VIP1);
	if (nVIP1 == 1) then

		for i = 1,2 do
		nIndex = AddScriptItem(131,2); --STG le hop;
		SetItemBindState(nIndex,2);
		nIndex = AddTaskItem(12,2); --LB PLD;
		SetItemBindState(nIndex,2);
		end;

		for i = 1,3 do
		--nIndex = AddScriptItem(3,2); --TTL;
		--SetItemBindState(nIndex,2);
		nIndex = AddTaskItem(59,2); --Hoa toc trieu lenh;
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(6,2); --Que hoa tuu;
		SetItemBindState(nIndex,2);
		end;
		Earn(100000);
		SetTask(T_VIP1,2);
		Msg2SubWorld("<color=yellow>§¹i hiÖp "..GetName().." võa nhËn thµnh c«ng<color> <color=green>VIP1<color>");
	elseif (nVIP1 == 2) then
		Msg2Player("H«m nay ®¹i hiÖp ®· nhËn quµ råi, ngµy mai h·y quay l¹i");
	else
	Msg2Player("C¸c h¹ ch­a ®¨ng ký VIP, liªn hÖ admin");
	end;
end;

function VIP2()
	local nVIP2 = GetTask(T_VIP2);
	if (nVIP2 == 1) then
		for i = 1,3 do
		nIndex = AddScriptItem(131,2); --STG le hop;
		SetItemBindState(nIndex,2);
		nIndex = AddTaskItem(12,2); --LB PLD;
		SetItemBindState(nIndex,2);
		--nIndex = AddScriptItem(4,2); --TTL DB;
		--SetItemBindState(nIndex,2);
		nIndex = AddTaskItem(59,2); --Hoa toc trieu lenh;
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(6,2); --Que hoa tuu;
		SetItemBindState(nIndex,2);
		end;
		Earn(200000);
		SetTask(T_VIP2,2);
		Msg2SubWorld("<color=yellow>§¹i hiÖp "..GetName().." võa nhËn thµnh c«ng<color> <color=green>VIP2<color>");
	elseif (nVIP2 == 2) then
		Msg2Player("H«m nay ®¹i hiÖp ®· nhËn quµ råi, ngµy mai h·y quay l¹i");
	else
	Msg2Player("C¸c h¹ ch­a ®¨ng ký VIP, liªn hÖ admin");
	end;
end;

function VIP3()
	local nVIP3 = GetTask(T_VIP3);
	if (nVIP3 == 1) then
		for i = 1,3 do
		nIndex = AddScriptItem(131,2); --STG le hop;
		SetItemBindState(nIndex,2);
		nIndex = AddTaskItem(12,2); --LB PLD;
		SetItemBindState(nIndex,2);
		--nIndex = AddScriptItem(4,2); --TTL DB;
		--SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(6,2); --Que hoa tuu;
		SetItemBindState(nIndex,2);
		end;
		for i = 1,5 do
		nIndex = AddTaskItem(59,2); --Hoa toc trieu lenh;
		SetItemBindState(nIndex,2);
		end;
		Earn(200000);
		SetTask(T_VIP3,2);
		Msg2SubWorld("<color=yellow>§¹i hiÖp "..GetName().." võa nhËn thµnh c«ng<color> <color=green>VIP3<color>");
	elseif (nVIP3 == 2) then
		Msg2Player("H«m nay ®¹i hiÖp ®· nhËn quµ råi, ngµy mai h·y quay l¹i");
	else
	Msg2Player("C¸c h¹ ch­a ®¨ng ký VIP, liªn hÖ admin");
	end;
end;

function ResetVIP()
	local nVIP1 = GetTask(T_VIP1);
	local nVIP2 = GetTask(T_VIP2);
	local nVIP3 = GetTask(T_VIP3);
	if (nVIP1 > 0) then
		SetTask(914,1);
	end;
	if (nVIP2 > 0) then
		SetTask(915,1);
	end;
	if (nVIP3 > 0) then
		SetTask(916,1);
	end;
end;


function OnCancel()
end;