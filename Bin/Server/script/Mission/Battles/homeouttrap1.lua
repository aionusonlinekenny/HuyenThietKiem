----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Cöa doanh tr¹i vµo hËu ph­¬ng Tèng Kim (phe Tèng)
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function main()
	if(GetCurCamp() == 1) then
		SetPos(GetMissionV(ZBattle.tbMission.HOMEIN_X1), GetMissionV(ZBattle.tbMission.HOMEIN_Y1))
		SetFightState(0)
		SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
	elseif (GetCurCamp() == 2) then
		Msg2Player("<color=orangered><color=yellow>Kim binh<color> sao cã thÓ lät vµo doanh tr¹i <color=lightsalmon>Tèng<color>.")
		SetFightState(1)
	end
end