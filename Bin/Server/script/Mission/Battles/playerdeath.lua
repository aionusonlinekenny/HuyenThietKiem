----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Ng­¬i ch¬i Tèng Kim chÕt
----------------------------------
Include("\\script\\mission\\battles\\functions.lua")

----------------------------------
--
----------------------------------
function OnDeath(nLauncher, nAttacker)
	-- KiÓm tra tr¹ng th¸i Tèng Kim
	if(GetMissionV(ZBattle.tbMission.STATE) ~= ZBattle.STATE_FIGHT) then
		return
	end
	-- Xö lý nÕu lµ Npc ®¸nh chÕt
	if(nAttacker == nil) then
		Msg2Player("§¹i hiÖp ®· bÞ ®¸nh träng th­¬ng, liªn tr¶m vÒ <color=green>0<color>.")
		SetTask(ZBattle.tbTask.BE_KILL, GetTask(ZBattle.tbTask.BE_KILL) + 1)
		if(GetTask(ZBattle.tbTask.SERIES) > 0) then
			SetTask(ZBattle.tbTask.SERIES, 0) 
			SyncTask(ZBattle.tbTask.SERIES)
		end
		if(GetTask(ZBattle.tbTask.SERIES_POINT) > 0) then
			SetTask(ZBattle.tbTask.SERIES_POINT, 0) 
			SyncTask(ZBattle.tbTask.SERIES_POINT)
		end
		SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
		SyncTask(ZBattle.tbTask.BE_KILL)
		return
	end
	-- LÊy th«ng tin ng­êi chÕt
	local sDeathName = GetName()
	local nDeathCamp = GetCurCamp()
	local nDeathRank = GetTaskTemp(ZBattle.tbTaskTmp.TITLE)
	local nCurDeath  = GetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST) + 1
	-- ChuyÓn Idx call script cho ng­êi giÕt
	PlayerIndex = nAttacker
	-- NÕu cïng Camp => lçi nªn tr¶ vÒ
	local nKillCamp = GetCurCamp()
	if(nKillCamp == nDeathCamp) then
		return
	end
	-- LÊy th«ng tin ng­êi giÕt
	local sKillName = GetName()
	local nKillRank = GetTaskTemp(ZBattle.tbTaskTmp.TITLE)
	local nCalcPoint  = nKillRank - nDeathRank
	local nSeriesKill = 0
	-- KiÓm tra xem cã post ®iÓm hay kh«ng
	if( (nCalcPoint >= 3) or (nCurDeath >= 7) ) then
		Msg2Player("§¹i hiÖp ®¸nh träng th­¬ng ®èi thñ qu¸ yÕu nªn kh«ng nhËn ®­îc tÝch lòy.")
	else
		-- TÝnh liªn tr¶m
		nSeriesKill = GetTask(ZBattle.tbTask.SERIES) + 1
		SetTask(ZBattle.tbTask.SERIES, nSeriesKill)
		if( nSeriesKill > GetTask(ZBattle.tbTask.MAX_SERIES) ) then
			SetTask(ZBattle.tbTask.MAX_SERIES, nSeriesKill)
			SyncTask(ZBattle.tbTask.MAX_SERIES)
		end
		-- TÝnh PK
		local nPK = GetTask(ZBattle.tbTask.KILL_ROLE) + 1
		SetTask(ZBattle.tbTask.KILL_ROLE, nPK)
		SetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST,0)
		-- TÝnh ®iÓm th­ëng theo liªn tr¶m
		local nPointSeries = 0
		if(mod(nSeriesKill,3) == 0) then
			nPointSeries = 15 * nSeriesKill
			if(nPointSeries >= 100) then
				nPointSeries = 100
			end
			SetTask(ZBattle.tbTask.SERIES_POINT, nPointSeries)
			SyncTask(ZBattle.tbTask.SERIES_POINT)
		end 
		-- TÝnh ®iÓm tÝch lòy
		local nPointKill = floor(150 * (nDeathRank/nKillRank))
		local nPoint = nPointSeries + nPointKill
		ZBattle:AddTotalPoint(nPoint)
		ZBattle:AddMissionPoint(nPoint, nKillCamp)
		SetTask(ZBattle.tbTask.ROLE_POINT, GetTask(ZBattle.tbTask.ROLE_POINT) + nPoint)
		-- Lµm míi qu©n hµm
		ZBattle:UpdateTitle(0,nKillCamp)
		--
		SyncTask(ZBattle.tbTask.SERIES)
		SyncTask(ZBattle.tbTask.KILL_ROLE)
		SyncTask(ZBattle.tbTask.ROLE_POINT)
	end
	UpdateTopTKNew();
	-- ChuyÓn Idx call script cho ng­êi chÕt
	PlayerIndex = nLauncher
	-- CËp nhËt th«ng tin ng­êi chÕt
	SetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST, nCurDeath)
	SetTask(ZBattle.tbTask.BE_KILL,GetTask(ZBattle.tbTask.BE_KILL) + 1)
	if(GetTask(ZBattle.tbTask.SERIES) > 0) then
		SetTask(ZBattle.tbTask.SERIES, 0) 
		SyncTask(ZBattle.tbTask.SERIES)
	end
	if(GetTask(ZBattle.tbTask.SERIES_POINT) > 0) then
		SetTask(ZBattle.tbTask.SERIES_POINT, 0) 
		SyncTask(ZBattle.tbTask.SERIES_POINT)
	end
	SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
	SyncTask(ZBattle.tbTask.BE_KILL)
	-- Th«ng b¸o cho ng­êi chÕt
	UpdateTopTKNew();
	if( (nCalcPoint >= 3) or (nCurDeath >= 7) ) then
		Msg2Player("Chªnh lÖch søc m¹nh qu¸ lín hoÆc ®¹i hiÖp ®ang cè t×nh post ®iÓm.")
		return
	end
	if(nKillCamp == 1) then
		Msg2Player("§¹i hiÖp ®· bÞ <color=sbrown>Tèng<color> "..ZBattle.tbRankName[nKillRank][1].."<color=wood>"..sKillName.." <color> ®¸nh träng th­¬ng, liªn tr¶m vÒ <color=green>0<color>.")
		if((ZBattle.tbRankName[nKillRank][1] == nil) or (sKillName == nil) or (ZBattle.tbRankName[nDeathRank][1] == nil) or (sDeathName == nil) or (nSeriesKill == nil) or (nKillCamp == nil)) then
			return
		end
		Msg2MSGroup(ZBattle.tbMission.MAIN,""..ZBattle.tbRankName[nKillRank][1].."<color=wood>"..sKillName.." <color>®· ®¸nh träng th­¬ng <color=plum>Kim<color> "..ZBattle.tbRankName[nDeathRank][1].."<color=wood>"..sDeathName.." <color>®¹t liªn tr¶m <color=green>"..nSeriesKill.."<color>.",nKillCamp)
	else
		Msg2Player("§¹i hiÖp ®· bÞ <color=plum>Kim<color> "..ZBattle.tbRankName[nKillRank][1].."<color=wood>"..sKillName.." <color> ®¸nh träng th­¬ng, liªn tr¶m vÒ <color=green>0<color>.")
		if((ZBattle.tbRankName[nKillRank][1] == nil) or (sKillName == nil) or (ZBattle.tbRankName[nDeathRank][1] == nil) or (sDeathName == nil) or (nSeriesKill == nil) or (nKillCamp == nil)) then
			return
		end
		Msg2MSGroup(ZBattle.tbMission.MAIN,""..ZBattle.tbRankName[nKillRank][1].."<color=wood>"..sKillName.." <color>®· ®¸nh träng th­¬ng <color=sbrown>Tèng<color> "..ZBattle.tbRankName[nDeathRank][1].."<color=wood>"..sDeathName.." <color>®¹t liªn tr¶m <color=green>"..nSeriesKill.."<color>.",nKillCamp)
	end
end