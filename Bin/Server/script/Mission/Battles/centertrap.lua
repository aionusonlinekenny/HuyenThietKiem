----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Cöa doanh tr¹i ra chiÕn tr­êng Tèng Kim
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function main()
	if(GetMissionV(ZBattle.tbMission.STATE) == ZBattle.STATE_SIGNED) then
		if(GetCurCamp() == 1) then
			SetPos(GetMissionV(ZBattle.tbMission.HOMEIN_X1), GetMissionV(ZBattle.tbMission.HOMEIN_Y1))
		elseif(GetCurCamp() == 2) then
			SetPos(GetMissionV(ZBattle.tbMission.HOMEIN_X2), GetMissionV(ZBattle.tbMission.HOMEIN_Y2))
		end
		SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
		SetFightState(0)
		Say("ChiÕn tr­êng ch­a b¾t ®Çu! Kh«ng thÓ tù ý x«ng ra trËn", 0)
	else
		SetFightState(1)
	end
end