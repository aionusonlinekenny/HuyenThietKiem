----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Cöa ra doanh tr¹i (phe Kim)
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function main()
	if(GetCurCamp() == 2) then
		if( GetFightState() == 1) then
			SetPos(GetMissionV(ZBattle.tbMission.HOMEIN_X2), GetMissionV(ZBattle.tbMission.HOMEIN_Y2))
			SetFightState(0)
		else
			local nRestTime = GetGameTime() - GetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH)
			if(nRestTime < ZBattle.TIME_PLAYER_REV) then
				Say("<color=red>"..(ZBattle.TIME_PLAYER_REV - nRestTime) .." gi©y<color> n÷a míi cã thÓ ra chiÕn tr­êng.", 0)
			else
				local file = GetMissionS(2)
				local x,y = ZBattle:GetAData(file)
				SetPos(floor(x/32), floor(y/32))
				SetFightState(1)
			end
		end
	elseif (GetCurCamp() == 1) then
		local file = GetMissionS(1)
		local x,y = ZBattle:GetAData(file)
		SetPos(floor(x/32), floor(y/32))
		Msg2Player("<color=orangered><color=lightsalmon>Tèng binh<color> sao cã thÓ lät vµo doanh tr¹i <color=yellow>Kim<color>.")
	end
end
