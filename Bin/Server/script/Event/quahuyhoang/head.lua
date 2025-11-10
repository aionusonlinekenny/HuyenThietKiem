Include("\\script\\system_config.lua");
-- Duong Chau
HUY_HOANG_SO_CAP = {
{44576,108544},
{44256,108608},
{44000,108608},
{43808,108800},
{44096,109024},
{44416,109024},
{44320,109216},
{44096,109344},
{44512,109440},
{44352,109792},
{44064,109888},
{44512,110176},
{44192,110432},
{44736,109824},
{45120,109472},
{45216,109120},
{45120,108640},
{44896,108480},
{44160,108032},
{43872,108096},
{44096,107808},
{44512,107776},
{44224,108288},
{44800,107776},
{44576,109120},
{44512,109664},
{45280,109088},
{44960,109024},
};
--320 Chan nui trong bach
HUY_HOANG_TRUNG_CAP = {
{38848,97920},
{39008,98048},
{39264,97920},
{39136,97504},
{39488,97984},
{39360,98592},
{39776,98400},
{39648,98016},
{39520,97632},
{39424,97632},
{39392,97312},
{39200,97248},
{39520,96896},
{39424,96640},
{39232,96832},
{38944,96960},
{39328,97152},
{39744,97376},
{39616,97568},
{40000,97440},
{40224,97600},
{40032,97248},
{39808,96992},
{40256,96896},
{40448,97408},
{40192,97280},
{39744,96992},
{39584,97152},
{39264,97568},
{38848,97600},
{38592,97856},
{39232,97344},
{39680,97856},
{40128,97952},
{39744,97728},
{39680,98336},
{39296,96672},
{39168,96864},
};

--340 Mac Cao Quat
HUY_HOANG_CAO_CAP = {
{58272,91616},
{57984,91392},
{58240,91264},
{58592,91232},
{58304,90880},
{58304,91008},
{58112,90624},
{57920,90944},
{58048,91168},
{58144,90432},
{57920,90176},
{58208,90144},
{58528,90048},
{58784,90048},
{58816,90304},
{58464,90528},
{58688,90272},
{58432,89888},
{58240,90272},
{58176,89888},
{58592,89792},
{58752,89888},
{58400,90336},
{58112,90272},
{58400,89536},
{58624,89728},
{58816,89920},
{59008,90240},
{58752,90368},
{58624,89792},
{58656,89312},
{58912,89248},
{59200,89472},
{58560,89952},
{58304,89824},
{58720,89760},
{58304,90112},
{58656,90400},
{59168,90656},
{58688,91008},
{58272,91072},
{58656,90912},
{58496,90304},
{58784,89952},
{59136,90144},
{58688,90656},
{57984,90336},
{59328,90336},
{58624,90816},
{59168,91104},
{59296,91584},
{58752,91488},
{58560,90912},
{58144,90496},
{58912,90400},
};

-- 336 Phong Lang do
HOANG_KIM_CAO_CAP ={
{39104,98976},
{39616,99008},
}

--------------------
SCRIPT_HAT = "\\script\\event\\quahuyhoang\\hathuyhoang.lua"
--------------------
SCRIPT_QUA = "\\script\\event\\quahuyhoang\\quahuyhoang.lua"
--------------------
function AddHatHuyHoang()
	local nSubWorld;
	--So cap
	for i = 1,getn(HUY_HOANG_SO_CAP) do
		SubWorld = SubWorldID2Idx(80);
		nNpcIndex = AddNpc(1110, 1, SubWorld, HUY_HOANG_SO_CAP[i][1], HUY_HOANG_SO_CAP[i][2], 0, "Hπt Huy Hoµng[S¨ c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_HAT);
		SetNpcParam(nNpcIndex,1,1);
	end;
	--Trung cap
	for i = 1,getn(HUY_HOANG_TRUNG_CAP) do
		SubWorld = SubWorldID2Idx(320);
		nNpcIndex = AddNpc(1110, 1, SubWorld, HUY_HOANG_TRUNG_CAP[i][1], HUY_HOANG_TRUNG_CAP[i][2], 0, "Hπt Huy Hoµng[Trung c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_HAT);
		SetNpcParam(nNpcIndex,1,2);
	end;
	--Cao cap
	for i = 1,getn(HUY_HOANG_CAO_CAP) do
		SubWorld = SubWorldID2Idx(340);
		nNpcIndex = AddNpc(1110, 1, SubWorld, HUY_HOANG_CAO_CAP[i][1], HUY_HOANG_CAO_CAP[i][2], 0, "Hπt Huy Hoµng[Cao c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_HAT);
		SetNpcParam(nNpcIndex,1,3);
	end;
	--thong bao--
	local commonstr = "<color=green>Qu∂ huy hoµng sæp ch›n tπi %s (%d/%d). C∏c Æπi hi÷p h∑y chu»n bﬁ thu hoπch!"
	Msg2SubWorld(format(commonstr,GetMapName(80),floor(HUY_HOANG_SO_CAP[1][1]/256), floor(HUY_HOANG_SO_CAP[1][2]/512)));
	Msg2SubWorld(format(commonstr,GetMapName(320),floor(HUY_HOANG_TRUNG_CAP[1][1]/256), floor(HUY_HOANG_TRUNG_CAP[1][2]/512)));
	Msg2SubWorld(format(commonstr,GetMapName(340),floor(HUY_HOANG_CAO_CAP[1][1]/256), floor(HUY_HOANG_CAO_CAP[1][2]/512)));	
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th´ng b∏o: <color> <color=green>Qu∂ huy hoµng<color> xu t hi÷n tπi D≠¨ng ch©u(Ti”u-T‰a ÆÈ 174/212), Ch©n nÛi Tr≠Íng bπch(Trung-T‰a ÆÈ 151/191), Mπc cao qu©t(Cao-T‰a ÆÈ 227/178) <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
end;

function AddQuaHuyHoang()
	local nSubWorld;
	--So cap
	for i = 1,getn(HUY_HOANG_SO_CAP) do
		SubWorld = SubWorldID2Idx(80);
		nNpcIndex = AddNpc(1111, 1, SubWorld, HUY_HOANG_SO_CAP[i][1], HUY_HOANG_SO_CAP[i][2], 0, "Qu∂ Huy Hoµng[S¨ c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_QUA);
		SetNpcParam(nNpcIndex,1,1);
	end;
	--Trung cap
	for i = 1,getn(HUY_HOANG_TRUNG_CAP) do
		SubWorld = SubWorldID2Idx(320);
		nNpcIndex = AddNpc(1111, 1, SubWorld, HUY_HOANG_TRUNG_CAP[i][1], HUY_HOANG_TRUNG_CAP[i][2], 0, "Qu∂ Huy Hoµng[Trung c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_QUA);
		SetNpcParam(nNpcIndex,1,2);
	end;
	--Cao cap
	for i = 1,getn(HUY_HOANG_CAO_CAP) do
		SubWorld = SubWorldID2Idx(340);
		nNpcIndex = AddNpc(1111, 1, SubWorld, HUY_HOANG_CAO_CAP[i][1], HUY_HOANG_CAO_CAP[i][2], 0, "Qu∂ Huy Hoµng[Cao c p]", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_QUA);
		SetNpcParam(nNpcIndex,1,3);
	end;
end;

function DelEventHuyHoang(Select)
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if (Select == 1) then -- Del hat huy hoang
		ClearMapNpcWithName(80,"Hπt Huy Hoµng[S¨ c p]");
		ClearMapNpcWithName(320,"Hπt Huy Hoµng[Trung c p]");
		ClearMapNpcWithName(340,"Hπt Huy Hoµng[Cao c p]");
		AddQuaHuyHoang();
	else --Del qua huy hoang
		ClearMapNpcWithName(80,"Qu∂ Huy Hoµng[S¨ c p]");
		ClearMapNpcWithName(320,"Qu∂ Huy Hoµng[Trung c p]");
		ClearMapNpcWithName(340,"Qu∂ Huy Hoµng[Cao c p]");
		if (nM < 40) then
			AddHatHuyHoang();
		end
	end;
end;

function AddHatHoangKim()
	local nSubWorld;
	--Hoang kim
	for i = 1,getn(HOANG_KIM_CAO_CAP) do
		SubWorld = SubWorldID2Idx(336);
		nNpcIndex = AddNpc(1117, 1, SubWorld, HOANG_KIM_CAO_CAP[i][1], HOANG_KIM_CAO_CAP[i][2], 0, "Hπt Hoµng Kim", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_HAT);
		SetNpcParam(nNpcIndex,1,4);
	end;
	local commonstr = "<color=orange>Qu∂ hoµng kim sæp ch›n tπi %s (%d/%d). C∏c Æπi hi÷p h∑y chu»n bﬁ thu hoπch!"
	Msg2SubWorld(format(commonstr,GetMapName(336),floor(HOANG_KIM_CAO_CAP[1][1]/256), floor(HOANG_KIM_CAO_CAP[1][2]/512)));	
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th´ng b∏o: <color> <color=green>Qu∂ hoµng kim<color> xu t hi÷n tπi "..GetMapName(336).." "..floor(HOANG_KIM_CAO_CAP[1][1]/256).."/"..floor(HOANG_KIM_CAO_CAP[1][1]/256).." <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
end;

function AddQuaHoangKim()
	local nSubWorld;
	--Hoang kim
	for i = 1,getn(HOANG_KIM_CAO_CAP) do
		SubWorld = SubWorldID2Idx(336);
		nNpcIndex = AddNpc(1118, 1, SubWorld, HOANG_KIM_CAO_CAP[i][1], HOANG_KIM_CAO_CAP[i][2], 0, "Qu∂ Hoµng Kim", 0, 3);
		SetNpcScript(nNpcIndex, SCRIPT_QUA);
		SetNpcParam(nNpcIndex,1,4);
	end;
end;

function DelEvenHoangKim()
	ClearMapNpcWithName(336,"Hπt Hoµng Kim");
	AddQuaHoangKim();
end;


