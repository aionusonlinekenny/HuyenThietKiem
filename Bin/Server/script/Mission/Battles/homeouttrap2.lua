----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Cöa doanh tr¹i vµo hËu ph­¬ng Tèng Kim (phe Kim)
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function main()
	if(GetCurCamp() == 2) then
		SetPos(GetMissionV(ZBattle.tbMission.HOMEIN_X2), GetMissionV(ZBattle.tbMission.HOMEIN_Y2))
		SetFightState(0)
		SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
	elseif (GetCurCamp() == 1) then
		Msg2Player("<color=orangered><color=lightsalmon>Tèng binh<color> sao cã thÓ lät vµo doanh tr¹i <color=yellow>Kim<color>.")
		SetFightState(1)
	end
end