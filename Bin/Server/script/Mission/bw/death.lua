----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Ng­êi chÕt
----------------------------------
Include("\\script\\mission\\bw\\functions.lua")

----------------------------------
--
----------------------------------
function OnDeath(nLauncher, nAttacker)
	-- NÕu kh«ng ph¶i lµ Player giÕt
	if(nAttacker == nil) then
		return
	end
	-- KiÓm tra trang th¸i l«i ®µi
	if(GetMissionV(ZBW.tbMission.STATE) ~= 2) then
		return
	end
	-- LÊy th«ng tin ng­êi chÕt
	local nDeathCamp = GetCurCamp()
	local tbCaptainName = ZBW:GetCaptainName()
	local szDeathName = GetName()
	-- ChuyÓn Idx sang cho giÕt chÕt
	PlayerIndex = nAttacker
	local szKillName = GetName()
	local nKillCamp = GetCurCamp()
	local szStr
	if( (nKillCamp == 2) or (nKillCamp == 3) ) then
		szStr = "L«i ®µi tû vâ ®ang diÔn ra, ®éi <color=wood>"..tbCaptainName[nKillCamp - 1].."<color> <color=metal>"..szKillName.."<color> ®¸nh b¹i ®éi <color=wood>"..tbCaptainName[4 - nKillCamp].."<color> <color=metal>"..szDeathName.."<color>."
		Msg2MSAll(ZBW.tbMission.MAIN, szStr)
	end
	-- ChuyÓn Idx cho ng­êi chÕt
	PlayerIndex = nLauncher
	if(szStr ~= nil) then
		DelMSPlayer(ZBW.tbMission.MAIN, PlayerIndex)
	end
end