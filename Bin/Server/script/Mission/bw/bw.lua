----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Mission L«i ®µi tû vâ
----------------------------------
Include("\\script\\mission\\bw\\functions.lua")

----------------------------------
--
----------------------------------
function InitMission()
	SetMissionV(ZBW.tbMission.STATE, 1)
	SetMissionV(ZBW.tbMission.KEY, random(10000000)+1)
	StartMissionTimer(ZBW.tbMission.MAIN, ZBW.SMALLTIMER, ZBW.TIMER_1)
	StartMissionTimer(ZBW.tbMission.MAIN, ZBW.TOTALTIMER, ZBW.TIMER_2)
	
	local i, nNpcIdx
	for i = 1, getn(ZBW.tbNpc) do
		nNpcIdx = AddNpc(332, 1, SubWorld, ZBW.tbNpc[i][1] * 32, ZBW.tbNpc[i][2] * 32, 0, ZBW.tbNpc[i][3])
		SetNpcScript(nNpcIdx, ZBW.sNpcFile)
	end
end

----------------------------------
--
----------------------------------
function RunMission()
	-- CËp nhËt d÷ liÖu
	local OldPlayerIndex = PlayerIndex
	local i, nIdx, nPlayerIdx = 0,0,0,0
	for i = 1, 300 do 
		nIdx, nPlayerIdx = GetNextPlayer(ZBW.tbMission.MAIN, nIdx, 0)
		if(nPlayerIdx > 0) then
			PlayerIndex = nPlayerIdx
			SetFightState(1)
		end
		if(nIdx <= 0) then 
			break
		end
	end
	PlayerIndex = OldPlayerIndex
	--
	SetMissionV(ZBW.tbMission.STATE, 2)
end

----------------------------------
--
----------------------------------
function EndMission()
	for i = 1, ZBW.tbMission.MEMBER_COUNT do 
		SetMissionV(i , 0)
	end
	SetMissionS(1, "")
	SetMissionS(2, "")
	ZBW:GameOver()
	StopMissionTimer(ZBW.tbMission.MAIN, ZBW.SMALLTIMER)
	StopMissionTimer(ZBW.tbMission.MAIN, ZBW.TOTALTIMER)
	
	ClearMapNpc(ZBW.FIGHT_MAP)
end

----------------------------------
--
----------------------------------
function JoinMission(RoleIndex, nCamp)
	PlayerIndex = RoleIndex
	ZBW:Join(nCamp)
end

----------------------------------
--
----------------------------------
function OnLeave(RoleIndex)
	PlayerIndex = RoleIndex
	ZBW:LeaveGame();
end