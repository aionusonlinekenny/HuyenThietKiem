----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Timer L«i ®µi tû vâ 1
----------------------------------
Include("\\script\\mission\\bw\\functions.lua")

tbCaptainName = {}
----------------------------------
--
----------------------------------
function OnTimer()
	local nState = GetMissionV(ZBW.tbMission.STATE)
	local nT = GetMissionV(ZBW.tbMission.TIMER) + 1
	SetMissionV(ZBW.tbMission.TIMER, nT)
	
	tbCaptainName = ZBW:GetCaptainName()
	if(nState == 1) then
		local szStr = ""
		szStr = ZBW:ReportMemberState(nT)
		ZBW:NoticeCaptainKey(szStr)
	elseif(nState == 2) then
		ZBW:ReportBattle(nT)
	elseif(nState == 3) then
		Msg2MSAll(ZBW.tbMission.MAIN, "TrËn chiÕn kÕt thóc!")
		CloseMission(ZBW.tbMission.MAIN)
	end
end