----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 12/03/2023
--	Desc: Timer TËng Kim 2
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------
-- Hµm ch›nh
----------------------------
function OnTimer()
	if(GetMissionV(ZBattle.tbMission.STATE) == ZBattle.STATE_WAIT) then
		return
	end
	CloseMission(ZBattle.tbMission.MAIN)
end