----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\Head.lua");
function InitMission()
	local nMapID = SubWorldIdx2ID(SubWorld);	
	SetMissionV(LDHead.tbMission.STATE,1);
	StartMissionTimer(LDHead.tbMission.MAIN, LDHead.tbTimer.TIMER_SUBTIMER, LDHead.tbTimer.TIMER_CONTDOWN) 
	print("Init mission Lien Dau: ",nMapID);
end;

function EndMission()
	--Start when mission run CloseMission(MissionID);
	local OldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	StopMissionTimer(LDHead.tbMission.MAIN,LDHead.tbTimer.TIMER_SUBTIMER);
	SetMissionV(LDHead.tbMission.STATE,0);
	SetMissionV(LDHead.tbMission.TIMER_SECOND,0);
	GameOver();
	SubWorld = OldSubWorld;
	local nMapID = SubWorldIdx2ID(SubWorld);	
	for i = 1,200 do
		SetMissionV(i,0);
	end;
	print("Close mission Lien Dau: ",nMapID);
end

function OnLeave(nIndex)
local nOldIndex = PlayerIndex;
	PlayerIndex = nIndex;
	--Start when player out mission or ondeath;
	SetTask(LDHead.tbTask.SIGNUP,0);
	SetTask(LDHead.tbTask.CALLDAME,0);
	SetTask(LDHead.tbTask.REGISTER,0);
	SetDeathScript("");
	SetLogoutScript("");
	SetPKFlag(0);
	SetFightState(0);
	SetPunish(0);
	ForbidTownPortal(0);
	LDHead:RemovePlayer();
PlayerIndex = nOldIndex;
end

function GameOver()
	local OldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	for i = 1, GetMissionV(LDHead.tbTask.TOTAL_MEMBER) do
		PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
		if (GetMissionV(LDHead.tbTask.SWORDMAN + i) > 0) then
			local Attacker = GetTask(LDHead.tbTask.RIVAL);
			local nW = GetWorldPos()
			if ((nW == LDHead.tbMap.SUBWORLD_2 or nW == LDHead.tbMap.SUBWORLD_3 or nW == LDHead.tbMap.SUBWORLD_4) and (Attacker > 0)) then
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
				local nDamagePlayer = GetDamage(GetTask(LDHead.tbTask.RIVAL)); --damage player
				
				PlayerIndex = Attacker;
				local nDamageAttacker = GetDamage(GetTask(LDHead.tbTask.RIVAL));--damage attacker
						
				-- Process Attacker
				PlayerIndex = Attacker;
					if (nDamageAttacker > nDamagePlayer) then
						LDHead:FinishMission(3);
					elseif (nDamageAttacker < nDamagePlayer) then
						LDHead:FinishMission(4);
					else
						LDHead:FinishMission(5);
					end
					LDHead:JoinHall();
					DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
				-- Process Player
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
					if (nDamagePlayer > nDamageAttacker) then
						LDHead:FinishMission(3);
					elseif (nDamagePlayer < nDamageAttacker) then
						LDHead:FinishMission(4);
					else
						LDHead:FinishMission(5);
					end
					LDHead:JoinHall();
					DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
			end;
		end;
	end;
	SubWorld = OldSubWorld;
end;