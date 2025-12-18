----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Timer l«i ®µi tû vâ 2
----------------------------------
Include("\\script\\mission\\bw\\functions.lua")

tbCaptainName = {}
----------------------------------
--
----------------------------------
function OnTimer()
	local nState = GetMissionV(ZBW.tbMission.STATE)
	if(nState == 0) then
		return
	end
	tbCaptainName = ZBW:GetCaptainName()
	ZBW:CheckWiner()
	CloseMission(ZBW.tbMission.MAIN)
end