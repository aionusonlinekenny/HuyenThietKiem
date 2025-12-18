----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Th­ viÖn hµm Tèng Kim
----------------------------------
Include("\\script\\mission\\battles\\head.lua")
Include("\\script\\lib\\libevent.lua");
Include("\\script\\system_config.lua");
----------------------------------
-- Th«ng tin
----------------------------------
function ZBattle:Info()
	Talk(2,"","Thêi gian b¸o danh lµ vµo <color=fire>10 giê 30 - 14 giê 30 - 20 giê 30<color>","Thêi gian thi ®Êu lµ 60 phót.")
end 

----------------------------------
-- 
----------------------------------
function ZBattle:CheckSignUp()
	if(GetMissionV(self.tbMission.STATE) >= self.STATE_SIGNED) then
		return 1
	end
	return 0
end

----------------------------------
--
----------------------------------
function ZBattle:SetBattleRule()
	local nDay, nHour = tonumber(date("%d")), tonumber(date("%H"))
	local i = 0
	if(mod(nDay,2) == 0) then
		if((nHour == 15)  or (nHour == 23)) then
			i = 1
		else
			i = 2
		end
	else
		if((nHour == 13) or (nHour == 17)) then
			i = 1
		else
			i = 2
		end
	end
	return i
end

----------------------------------
-- KiÓm tra phe trËn cuèi cïng
----------------------------------
function ZBattle:CheckLastGroup(nCamp)
	local nLast = GetTask(self.tbTask.LAST_JOIN)
	if(nLast == 0) then
		return nLast
	end
	if( mod(nLast,2) ~= 0 and (nCamp == 1) ) then
		return nCamp
	end
	if( mod(nLast,2) == 0 and (nCamp == 2) ) then
		return nCamp
	end	
end

----------------------------------
-- KiÓm tra ®iÒu kiÖn lµm míi task
----------------------------------
function ZBattle:IsResetTask()
	local nKey = GetMissionV(self.tbMission.KEY_SJ)
	if(GetTask(self.tbTask.LAST_KEY) ~= nKey) then
		SetTask(self.tbTask.LAST_KEY, nKey)
		if(GetTask(self.tbTask.LAST_JOIN) > 0) then
			return 1
		end
		return 0
	end
	return 0
end

----------------------------------
-- Lµm míi Task
----------------------------------
function ZBattle:ResetAllTask()
	SetTask(self.tbTask.LAST_JOIN,0)
	SetTask(self.tbTask.TEMP_POINT,0)
	SetTask(self.tbTask.KILL_ROLE,0)
	SetTask(self.tbTask.ROLE_POINT,0)
	SetTask(self.tbTask.KILL_NPC,0)
	SetTask(self.tbTask.NPC_POINT,0)
	SetTask(self.tbTask.BE_KILL,0)
	SetTask(self.tbTask.SERIES,0)
	SetTask(self.tbTask.SERIES_POINT,0)
	SetTask(self.tbTask.MAX_SERIES,0)
	SetTask(self.tbTask.SNAP_FLAG,0)
	SetTask(self.tbTask.FLAG_POINT,0)
	SetTask(self.tbTask.GET_ITEM,0)
	SetTask(self.tbTask.ITEM_POINT,0)
	SetTask(self.tbTask.RANK,0)
	self:SyncAllTask()
end

----------------------------------
-- Lµm míi Task
----------------------------------
function ZBattle:SyncAllTask()
	SyncTask(self.tbTask.TEMP_POINT)
	SyncTask(self.tbTask.KILL_ROLE)
	SyncTask(self.tbTask.ROLE_POINT)
	SyncTask(self.tbTask.KILL_NPC)
	SyncTask(self.tbTask.NPC_POINT)
	SyncTask(self.tbTask.BE_KILL)
	SyncTask(self.tbTask.SERIES)
	SyncTask(self.tbTask.SERIES_POINT)
	SyncTask(self.tbTask.MAX_SERIES)
	SyncTask(self.tbTask.SNAP_FLAG)
	SyncTask(self.tbTask.FLAG_POINT)
	SyncTask(self.tbTask.GET_ITEM)
	SyncTask(self.tbTask.ITEM_POINT)
	SyncTask(self.tbTask.RANK)
end

----------------------------------
-- 
----------------------------------
function ZBattle:UpdateMemberCount(nAdd)
	local nMemCount = GetMissionV(self.tbMission.MEMBER_COUNT) + nAdd
	SetMissionV(self.tbMission.MEMBER_COUNT, nMemCount)
end

----------------------------------
--
----------------------------------
function ZBattle:UpdateTitle(nJoin, nCamp)
	if((nCamp ~= 1) and (nCamp ~= 2)) then
		return
	end
	local nTitleID = GetTaskTemp(self.tbTaskTmp.TITLE) + 1
	if(nTitleID > 5) then
		return
	end
	local nTempPoint = GetTask(self.tbTask.TEMP_POINT)
	if(nTempPoint < self.tbRankName[nTitleID][2]) then
		return
	end
	SetTaskTemp(self.tbTaskTmp.TITLE, nTitleID)
	AddSkillState(self.RANK_SKILL, nTitleID, 1, 999999)
	if(nCamp == 1) then
		SetTitle(nTitleID + 305);
	else
		SetTitle(nTitleID + 310);
	end
	if(nJoin == 1) then
		Msg2Player("HiÖn t¹i víi <color=green>"..nTempPoint.."<color> ®iÓm tÝch lòy. Ng­¬i ®­îc phong lµm "..self.tbRankName[nTitleID][1]..self.tbRankName[nTitleID][4]..".")
		return
	end
	Msg2MSGroup(self.tbMission.MAIN,"<color=wood>"..GetName().." <color>trong chiÕn tr­êng x«ng x¸o ®¹t ®­îc danh hiÖu "..self.tbRankName[nTitleID][1]..self.tbRankName[nTitleID][4]..".",nCamp)
end

----------------------------------
--
----------------------------------
function ZBattle:Join(nCamp)
	-- Rêi khái ®éi
	LeaveTeam()
	-- LÊy sè l­îng qu©n mçi bªn
	local nSongCount = GetMSPlayerCount(self.tbMission.MAIN, 1)
	local nJinCount  = GetMSPlayerCount(self.tbMission.MAIN, 2)
	-- KiÓm tra sè l­îng qu©n
	if(nCamp == 1) then
		if(nSongCount >= self.MAX_MEMBERCOUNT) then
			Msg2Player("Qu©n ®éi hiÖn ®· ®ñ ng­êi kh«ng thÓ nhËn thªm, h·y ®îi mét l¸t.")
			return
		end
		if(nSongCount - nJinCount >= 2) then
			Say("Chªnh lÖch qu©n sè qu¸ cao. Xin h·y chê 1 l¸t.",0)
			return
		end
	else
		if(nJinCount >= self.MAX_MEMBERCOUNT) then
			Msg2Player("Qu©n ®éi hiÖn ®· ®ñ ng­êi kh«ng thÓ nhËn thªm, h·y ®îi mét l¸t.")
			return
		end
		if(nJinCount - nSongCount >= 2) then
			Say("Chªnh lÖch qu©n sè qu¸ cao. Xin h·y chê 1 l¸t.",0)
			return
		end
	end

	SetLockSongJin(GetAccount(),1); --Gioi han account SongJin by kinnox;

	-- KiÓm tra reset Task
	if(self:IsResetTask() == 1) then
		self:ResetAllTask()
	end
	-- TiÕn hµnh cho vµo chiÕn tr­êng
	self:UpdateMemberCount(1)
	AddMSPlayer(self.tbMission.MAIN, nCamp)
	SetTask(self.tbTask.LAST_JOIN, nCamp)
	SetTask(self.tbTask.GROUP, nCamp)
	SyncTask(self.tbTask.GROUP)
	SetTask(self.tbTask.LAST_TITLE, GetTitle())
	SetTaskTemp(self.tbTaskTmp.LAST_DEATH, GetGameTime())
	self:UpdateTitle(1,nCamp)
	SetCurCamp(nCamp)
	SetPunish(1)
	SetFightState(0)
	SetCreateTeam(0)
	ForbitTrade(1)
	SetPKFlag(1)
	ForbidChangePK(1)
	ForbidTownPortal(1)
	SetDeathScript(self.tbFile.FILE_PLAYERDEATH)
	SetLogoutScript(self.tbFile.FILE_PLAYERLOGOUT)
	local nPosX, nPosY = 0,0
	if(nCamp == 1) then
		nPosX, nPosY = GetMissionV(self.tbMission.HOMEIN_X1), GetMissionV(self.tbMission.HOMEIN_Y1) 
	else
		nPosX, nPosY = GetMissionV(self.tbMission.HOMEIN_X2), GetMissionV(self.tbMission.HOMEIN_Y2) 
	end
	Msg2MSGroup(self.tbMission.MAIN,"<color=orangered><color=wood>"..GetName().." <color>®· vµo chiÕn tr­êng.",nCamp)
	SetLogoutRV(1)
	SetRevPos(self.SIGN_MAP, nCamp)
	NewWorld(self.FIGHT_MAP, nPosX, nPosY)
	SetTempRevPos(self.FIGHT_MAP, nPosX * 32, nPosY * 32)
	if (nCamp == 1) then
	SetGlbMissionV(self.tbMission.TONG_SOLIDER,GetMSPlayerCount(self.tbMission.MAIN,1)); -- task tinh so luon cua bang xep hang quan tong
	else
	SetGlbMissionV(self.tbMission.KIM_SOLIDER,GetMSPlayerCount(self.tbMission.MAIN,2));-- task tinh so luon cua bang xep hang quan kim
	end;
	UpdateTopTKNew();
end

----------------------------------
--
----------------------------------
function ZBattle:AddTotalPoint(nPoint)
	if(nPoint == 0) then
		return 0
	end
	SetTask(self.tbTask.TEMP_POINT, GetTask(self.tbTask.TEMP_POINT) + nPoint)
	SetTask(self.tbTask.AY_POINT, GetTask(self.tbTask.AY_POINT) + nPoint) 
	Msg2Player("NhËn ®­îc <color=green>"..nPoint.." ®iÓm tÝch lòy<color>.")
	SyncTask(self.tbTask.TEMP_POINT)
	return nPoint
end 

----------------------------------
--
----------------------------------
function ZBattle:AddMissionPoint(nRank, nCamp)
	if((nCamp ~= 1) and (nCamp ~= 2)) then
		return
	end
	local nPoint = 0
	if(nRank < 6) then
		nPoint = self.tbNpcPoint[nRank]
	else
		nPoint = nRank
	end
	if(nCamp == 1) then
		SetMissionV(self.tbMission.SONGPOINT, GetMissionV(self.tbMission.SONGPOINT) + nPoint)
	else
		SetMissionV(self.tbMission.JINPOINT, GetMissionV(self.tbMission.JINPOINT) + nPoint)
	end
end

----------------------------------
--  Rêi khái Tèng Kim
----------------------------------
function ZBattle:LeaveBattle(nCamp)
	if(nCamp ~= 1) and (nCamp ~= 2) then
		return
	end
	--SetTask(self.tbTask.LAST_JOIN,0) --reset task las_joint;
	NewWorld(self.SIGN_MAP, self.tbSignMapPos[nCamp][1], self.tbSignMapPos[nCamp][2])
	SetFightState(0)
	SetLockSongJin(GetAccount(),0); --Gioi han account SongJin by kinnox;
end

----------------------------------
--
----------------------------------
function ZBattle:TopAward(nTop)
	local i = GetTaskTemp(self.tbTaskTmp.TITLE)
	AddOwnExp(self.tbAwardRank[i])
	Msg2Player("Qu©n hµm hiÖn t¹i lµ "..self.tbRankName[i][1].." nhËn ®­îc thªm <color=purple>"..self.tbAwardRank[i].."<color> ®iÓm kinh nghiÖm.")
	if(nTop >= 1 and nTop <= 10) then
		local sTmpStr = ""
		if(nTop < 4) then
			AddScriptItem(self.tbAwardTopEx[1][1])
			sTmpStr = self.tbAwardTopEx[1][2]
		else
			n = random(2,3)
			AddScriptItem(self.tbAwardTopEx[n][1])
			sTmpStr = self.tbAwardTopEx[n][2]
		end
		sTmpStr = sTmpStr.." vµ "..self.tbAwardTop[nTop][2]
		AddScriptItem(self.tbAwardTop[nTop][1])
		if(i < 6) then
			SetTask(self.tbTask.WEEK_POINT, self.tbAwardWeek[nTop])
		end
		Msg2MSAll(self.tbMission.MAIN,"<color=wood>"..GetName().."<color> <color=sbrown>®¹t h¹ng "..nTop.."<color> ë Tèng Kim nhËn ®­îc "..sTmpStr..".")
	end
	if(nTop >= 6) then
		SetTask(self.tbTask.WEEK_POINT, self.tbAwardWeek[6])
	end
end

----------------------------------
--
----------------------------------
function ZBattle:GroupAward(nWinGroup)
	local nCamp = GetCurCamp()
	if((nCamp ~= 1) and (nCamp ~= 2)) then
		return
	end
	local nPoint = 0
	if(nWinGroup > 0) then
		if(nCamp == nWinGroup) then
			nPoint = self.WIN_BONUS_POINT
			if (GetTask(self.tbTask.TEMP_POINT) >= 3000) then
				BonusTongKim()
				Msg2Player("<color=purple>§¹i hiÖp lµ cao thñ trªn 3000 ®iÓm ®­îc tÆng thªm 5.000.000 kinh nghiÖm vµ r­¬ng Tèng Kim <color>")
			end;
			Msg2Player("Phe ®¹i hiÖp th¾ng cuéc nhËn ®­îc thªm <color=purple>"..nPoint.."<color> ®iÓm tÝch lòy Tèng Kim.")
			EventTongKim();
			WinTongKim();
		else
			nPoint = self.LOSE_BONUS_POINT
			if (GetTask(self.tbTask.TEMP_POINT) >= 3000) then
				BonusTongKim()
				Msg2Player("<color=purple>§¹i hiÖp lµ cao thñ trªn 3000 ®iÓm ®­îc tÆng thªm 5.000.000 kinh nghiÖm vµ r­¬ng Tèng Kim <color>")
			end;
			EventTongKim();
			LoseTongKim();
			Msg2Player("Phe ®¹i hiÖp thua cuéc nhËn ®­îc thªm <color=purple>"..nPoint.."<color> ®iÓm tÝch lòy Tèng Kim.")
		end
	else
		nPoint = self.TIE_BONUS_POINT
		Msg2Player("KÕt thóc hßa nhËn ®­îc thªm <color=purple>"..nPoint.."<color> ®iÓm tÝch lòy Tèng Kim.")
	end
	SetTask(self.tbTask.AY_POINT,GetTask(self.tbTask.AY_POINT) + nPoint)
	SyncTask(self.tbTask.AY_POINT)
	self:LeaveBattle(nCamp)
end

----------------------------------
--
----------------------------------
function ZBattle:GameOver()
	local nCount = GetMissionV(self.tbMission.MEMBER_COUNT)

	local nSongPoint = GetMissionV(self.tbMission.SONGPOINT)
	local nJinPoint = GetMissionV(self.tbMission.JINPOINT)
	
	local nOldPlayerIndex = PlayerIndex
	local tbPlayer = {}
	local i, nIdx, nPlayerIdx = 0,0,0,0
	nCount = 0
	for i = 1, 300 do 
		nIdx, nPlayerIdx = GetNextPlayer(self.tbMission.MAIN, nIdx, 0)
		if(nPlayerIdx > 0) then
			PlayerIndex = nPlayerIdx
			self:TopAward(GetTask(self.tbTask.RANK))
			UpdateTopTKNew();
			nCount = nCount + 1
			tbPlayer[nCount] = nPlayerIdx
			SetLockSongJin(GetAccount(),0); --Gioi han account SongJin by kinnox;
		end
		if(nIdx <= 0) then 
	 		break
	 	end
	end
	local nFinalWiner = GetMissionV(self.tbMission.MARSHAL_DEATH)
	if(nFinalWiner == 0) then
		if(nSongPoint > nJinPoint) then
			nFinalWiner = 1
		elseif(nSongPoint < nJinPoint) then
			nFinalWiner = 2
		end		
	end
	if(nCount > 0) then
		for i = 1, nCount do
			PlayerIndex = tbPlayer[i];
			self:BattleBroad(self.FIGHT_MAP) -- tam dich la tu do 
			self:GroupAward(nFinalWiner)
		end
	end
	PlayerIndex = nOldPlayerIndex
end

function ZBattle:BattleBroad(nSubWorld)
	local OldSubWorld = SubWorld;
	SubWorld = nSubWorld;
	
	SubWorld = OldSubWorld;
end;
----------------------------------
--
----------------------------------
function ZBattle:BonusRate()
	local nSongCount = GetMSPlayerCount(self.tbMission.MAIN, 2)
	local nJinCount = GetMSPlayerCount(self.tbMission.MAIN, 2)
	local nAvgCount = (nSongCount + nJinCount)/2
	local nRateSong, nRateJin = 0,0
	if(nAvgCount == 0) then
		nRateSong = 1
		nRateJin = 1
	else
		nRateSong = 1 - (nSongCount-nAvgCount)/nAvgCount
		nRateJin = 1 - (nJinCount-nAvgCount)/nAvgCount
	end
	return nRateSong,nRateJin
end


function OnSongJin()

	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(380);
	OpenMission(ZBattle.tbMission.MAIN);
	SubWorld = nOldSubWorld;
end

function Active()
	local nDay, nHour, nMinutes = date("%A"), tonumber(date("%H")), tonumber(date("%M"))
	if(SYS_SWITCH_BATTLES == 0) then
		return
	end
	--Thong bao Notification;
	if	(nHour == 10 and nMinutes == 25) or 
		(nHour == 14 and nMinutes == 25) or
		(nHour == 20 and nMinutes == 25) then
		Msg2SubWorld("Thêi gian b¸o danh Tèng kim ®· ®Õn, Cßn <color=green>5 phót<color> ®Ó b¸o danh");
	end;
	--Active Mission;
	if	(nHour == 10 and nMinutes == 30) or 
		(nHour == 14 and nMinutes == 30) or
		(nHour == 20 and nMinutes == 30) then
			OnSongJin();
			for i = 1,SYS_MAX_PLAYER do
				PlayerIndex = i;
				SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Tèng kim<color> ®ang trong thêi gian b¸o danh h·y mau chãng tham chiÕn <pic="..random(1,140).."> <pic="..random(1,140).."> !");
			end;
	end;
end;
---
