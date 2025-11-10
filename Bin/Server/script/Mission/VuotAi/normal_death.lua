Include("\\script\\mission\\vuotai\\head.lua")
Include("\\script\\lib\\libevent.lua")
function OnDeath(nIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
	local OldSubWorld = SubWorld;
	local nX, nY, nSub = GetNpcPos(nIndex);
	SubWorld = nSub;
	if nIndex > 0 then
		local nIdMaps = SubWorldIdx2ID(SubWorld);
		local nParamNpc = GetNpcParam(nIndex,1);
		local nBoardTime = GetMissionV(VA.MAIN.BOARD_TIMES);
		nLevel = random(9,10);
		nSeries = random(0,4);
		nDropItem = "\\settings\\droprate\\boss\\bosstask_lev90.ini"
		AddOwnExp(random(17000,22000));
		if nParamNpc == 1 then
			local nCountNpc = GetMissionV(VA.MAIN.BOARD_NPC) - 1;
			
			if (nCountNpc > 0 and CountNpcInMap(nIdMaps) > 1) then
				SetMissionV(VA.MAIN.BOARD_NPC, nCountNpc);
				if nCountNpc < 10 then
					Msg2MSAll(VA.MAIN.MISSION_VA,format("Sè l­îng qu¸i cßn l¹i lµ <color=yellow>%d<color> qu¸i.",nCountNpc));
				end
				AddBlood(1,2,nIndex);
				DropRateItem(nIndex,1,nDropItem,1,nLevel,nSeries); -- roi trang bi xanh
			else
				SetMissionV(VA.MAIN.BOARD_TIMES, nBoardTime + 1);
				VA:AddMonsterMS(SubWorld) -- add monster;
			end
		elseif nParamNpc == 2 then
			local nCountNpc = GetMissionV(VA.MAIN.BOARD_NPC) - 1;
			if (nCountNpc > 0 and CountNpcInMap(nIdMaps) > 1) then
				SetMissionV(VA.MAIN.BOARD_NPC, nCountNpc);
				if nCountNpc < 10 then
					Msg2MSAll(VA.MAIN.MISSION_VA,format("Sè l­îng qu¸i cßn l¹i lµ <color=yellow>%d<color> qu¸i.",nCountNpc));
				end
				AddBlood(1,2,nIndex);
				DropRateItem(nIndex,1,nDropItem,1,nLevel,nSeries); -- roi trang bi xanh
			else
				VA:AddBossMS(SubWorld, nBoardTime);
			end
		elseif nParamNpc == 3 then 
			SetMissionV(VA.MAIN.BOARD_TIMES, nBoardTime + 1);
			VA:AddMonsterMS(SubWorld) -- add monster;
			AddBlood(1,10,nIndex);
			DropVuotAi(); --phan thuong boss XU
			-------------------ADD EXP
			local nOldPlayer = PlayerIndex;
			if (GetTeamSize() > 0) then
				local TAB_Player = {}
				for i=1,GetTeamSize() do
					TAB_Player[i] = GetTeamMember(i)
				end
				for i=1,getn(TAB_Player) do
					PlayerIndex = TAB_Player[i]
					AddOwnExp(500000);
				end;
			end;
			PlayerIndex = nOldPlayer;
			-------------------
		elseif nParamNpc == 4 then -- phandame
			local nCountNpc = GetMissionV(VA.MAIN.BOARD_NPC) - 1;
			if (nCountNpc > 0 and CountNpcInMap(nIdMaps) > 1) then
				SetMissionV(VA.MAIN.BOARD_NPC, nCountNpc);
				if nCountNpc < 10 then
					Msg2MSAll(VA.MAIN.MISSION_VA,format("Sè l­îng qu¸i cßn l¹i lµ <color=yellow>%d<color> qu¸i.",nCountNpc));
				end
				AddBlood(1,10,nIndex);			
			else
			SetMissionV(VA.MAIN.BOARD_TIMES, nBoardTime + 1);
			VA:AddBossAntiDamgeMS(SubWorld);
			end;
		elseif nParamNpc == 5 then -- phan dame
			local nCountNpc = GetMissionV(VA.MAIN.BOARD_NPC) - 1;
			if nCountNpc > 0 and CountNpcInMap(nIdMaps) > 1 then
				SetMissionV(VA.MAIN.BOARD_NPC, nCountNpc);
				if nCountNpc < 10 then
					Msg2MSAll(VA.MAIN.MISSION_VA,format("Sè l­îng qu¸i cßn l¹i lµ <color=yellow>%d<color> qu¸i.",nCountNpc));
				end
				AddBlood(1,10,nIndex);
			else
			SetMissionV(VA.MAIN.BOARD_TIMES, nBoardTime + 1);
			 VA:AddBossNhipNhiTranMS(SubWorld);	
			end;
		elseif nParamNpc == 6 then
			DropVuotAi(); --phan thuong boss XU
			SetMissionV(VA.MAIN.LOSE_WIN,1);
			CloseMission(VA.MAIN.MISSION_VA);
		end
	end
	SubWorld = OldSubWorld;
end

function AddBlood(nMin,nMax,nIndex)
	local Blood = random(1,50);
	local MAX = random(nMin+1,nMax);
	local nX, nY, nSub = GetNpcPos(nIndex);
	if (Blood == 1) then
		nPlayer = PlayerIndex;
		for i = nMin,MAX do
			local nPosX = random(1,10);
			local nPosY = random(1,10);
			local nCal = random (1,2);
			if (nCal == 1 ) then
				nX = nX + nPosX;
				DropItem(nSub,nX,nY,nPlayer,5,2,0,5,0,0,0);
			else
				nX = nX - nPosX;
				DropItem(nSub,nX,nY,nPlayer,5,2,0,5,0,0,0);
			end;
		end;
	end;
end;