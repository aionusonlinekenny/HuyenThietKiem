--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\phonglangdo\\head.lua");
function OnUse(nNpcIndex)
	local nNpcIndex = 0;
	local nW,nX,nY = GetWorldPos();
	nX = nX*32;
	nY = nY*32;
	
	if (nW ~= 336) then
		Msg2Player("ChØ sö dông vËt phÈm nµy t¹i Phong L¨ng §é");
		return 0;
	end;

	local SubWorld = SubWorldID2Idx(336);
	nNpcIndex = AddNpc(819, 95, SubWorld,nX, nY, 1, "ThiÕt Tö §¹o Nh©n",1, 0);
	Msg2SubWorld(" §¹i hiÖp "..GetName().." triÖu håi ThiÕt Tö §¹o Nh©n t¹i "..floor(nX/256).." - "..floor(nY/512).." phong l¨ng ®é. ");
	local nRanSer = random(0,4);
	SetNpcSeries(nNpcIndex,nRanSer);
	local Series = GetNpcSeries(nNpcIndex);
	if (Series == 0) then
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	elseif (Series == 1) then
		SetNpcSkill(nNpcIndex,425,50,1);
		SetNpcSkill(nNpcIndex,425,50,2);
		SetNpcSkill(nNpcIndex,425,50,3);
		SetNpcSkill(nNpcIndex,425,50,4);
	elseif (Series == 2) then
		SetNpcSkill(nNpcIndex,426,50,1);
		SetNpcSkill(nNpcIndex,426,50,2);
		SetNpcSkill(nNpcIndex,426,50,3);
		SetNpcSkill(nNpcIndex,426,50,4);
	elseif (Series == 3) then
		SetNpcSkill(nNpcIndex,427,50,1);
		SetNpcSkill(nNpcIndex,427,50,2);
		SetNpcSkill(nNpcIndex,427,50,3);
		SetNpcSkill(nNpcIndex,427,50,4);
	elseif (Series == 4) then
		SetNpcSkill(nNpcIndex,428,50,1);
		SetNpcSkill(nNpcIndex,428,50,2);
		SetNpcSkill(nNpcIndex,428,50,3);
		SetNpcSkill(nNpcIndex,428,50,4);
	else
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	end;
	SetNpcReduceDamge(nNpcIndex, 130);
	SetNpcScript(nNpcIndex,PLD.MAIN.NPC_DEATH);
	return 1;
end;