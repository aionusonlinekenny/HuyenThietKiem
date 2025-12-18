----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: 
----------------------------------
Include("\\script\\mission\\bw\\head.lua")

----------------------------------
--
----------------------------------
function ZBW:GetKey()
	local tbKey = {}
	tbKey[1] = Random(9999)
	tbKey[2] = Random(9999)
	if(tbKey[2] == tbKey[1]) then
		if (tbKey[1] < 9996) then
			tbKey[2] = tbKey[1] + 3
		else
			tbKey[2] = tbKey[1] - 3
		end
	end
	return tbKey
end

----------------------------------
-- Gia nhËp ®Êu tr­êng
----------------------------------
function ZBW:Join(nCamp)
	ZPlayer:Refresh()
	LeaveTeam()
	local nCampF = 0
	if(nCamp == 3) then
		nCampF = 6
	else
		nCampF = nCamp + 1
	end
	AddMSPlayer(self.tbMission.MAIN, nCamp)
	if(nCamp == 1) then
		local i
		for i = 1, GetMissionV(self.tbMission.MEMBER_COUNT) do
			if(GetMissionV(self.tbMission.INDEX_GROUP1 + i) <= 0) then
				SetMissionV(self.tbMission.INDEX_GROUP1 + i, PlayerIndex)
			end
		end
	elseif(nCamp == 2) then
		local i
		for i = 1, GetMissionV(self.tbMission.MEMBER_COUNT) do
			if(GetMissionV(self.tbMission.INDEX_GROUP2 + i) <= 0) then
				SetMissionV(self.tbMission.INDEX_GROUP2 + i, PlayerIndex)
			end
		end
	end
	SetTask(self.tbTask.CALC_DAMAGE,1)
	SetCurCamp(nCampF)
	SetFightState(0)
	SetPunish(1)
	SetCreateTeam(0)
	ForbitTrade(1)
	if(nCampF == 6) then
		SetPKFlag(0)
		ChangeOwnFeature(1204,733,999999)
		Msg2Player("§· chuyÓn sang tr¹i th¸i kh¸n gi¶. ")
	else
		SetPKFlag(1)
		SetDeathScript(self.DEATH_FILE)
	end
	ForbidChangePK(1)
	ForbidTownPortal(1)
	local nW,nX,nY = GetTask(self.tbTask.SIGN_WORLD),GetTask(self.tbTask.SIGN_POSX),GetTask(self.tbTask.SIGN_POSY)
	SetTempRevPos(nW, nX * 32, nY * 32)

	if(nCamp == 1) then
		NewWorld(self.FIGHT_MAP, self.tbFightPos[1][1], self.tbFightPos[1][2])
	elseif(nCamp == 2) then
		NewWorld(self.FIGHT_MAP, self.tbFightPos[2][1], self.tbFightPos[2][2])
	else 
		NewWorld(self.FIGHT_MAP, self.tbFightPos[3][1], self.tbFightPos[3][2])
	end
	if(nCamp == 3) then
		Msg2MSAll(self.tbMission.MAIN, "Kh¸n gi¶ <color=yellow>"..GetName().." <color>®· tiÕn vµo ®Êu tr­êng. ")
	else
		Msg2MSAll(self.tbMission.MAIN, "<color=wood>"..GetName().." <color>®· tiÕn vµo ®Êu tr­êng. ")
	end
end

----------------------------------
-- LÊy tªn ®éi tr­ëng
----------------------------------
function ZBW:GetCaptainName()
	local OldSubWorld = SubWorld
	local nIdx = SubWorldID2Idx(209)
	local szCaptainName = {}
	if(nIdx == -1) then
		return szCaptainName
	end
	SubWorld = nIdx
	szCaptainName[1] = GetMissionS(1)
	szCaptainName[2] = GetMissionS(2)
	SubWorld = OldSubWorld
	return szCaptainName
end

----------------------------------
-- HiÓn thÞ m· trËn ®Êu
----------------------------------
function ZBW:OnShowKey()
	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(self.FIGHT_MAP)
	if(SubWorld < 0) then
		return
	end
	
	if(GetMissionV(self.tbMission.MEMBER_COUNT) <= 1) then
		Say(10410,0)
		SubWorld = OldSubWorld
		return
	end
	local nKey = 0
	local sName = GetName()
	if(sName == tbCaptainName[1]) then
		nKey = GetMissionV(self.tbMission.KEY_TEAM_1)
	elseif(sName == tbCaptainName[2]) then 
		nKey = GetMissionV(self.tbMission.KEY_TEAM_2)
	end
	if(nKey == 0) then
		Say(10411,0)
	else
		Say("M· trËn ®Êu lµ <color=fire>"..nKey.."<color>. H·y mau th«ng b¸o cho c¸c thµnh viªn trong ®éi! Thµnh viªn nµo cã m· chÝnh x¸c míi cã thÓ tham gia thi ®Êu!",0)
	end
	SubWorld = OldSubWorld
end

----------------------------------
-- B¾t ®Çu chiÕn ®Êu
----------------------------------
function ZBW:BeginCompete()
	local nGroup1PlayerCount = GetMSPlayerCount(ZBW.tbMission.MAIN, 1)
	local nGroup2PlayerCount = GetMSPlayerCount(ZBW.tbMission.MAIN, 2)
	local sStr = ""
	if( (nGroup1PlayerCount <= 0) and (nGroup2PlayerCount <= 0) ) then 
		sStr = "§· ®Õn giê thi ®Êu! §éi <color=wood>"..tbCaptainName[1].." <color>cïng ®éi <color=wood>"..tbCaptainName[2].." <color>®· kh«ng vµo l«i ®µi. Hñy bá thi ®Êu!"
		Msg2MSAll(ZBW.tbMission.MAIN, sStr)
		SetMissionV(ZBW.tbMission.STATE, 3)
		CloseMission(ZBW.tbMission.MAIN)
		return
	end
	
	if(nGroup1PlayerCount <= 0 ) then 
		sStr = "§· ®Õn giê thi ®Êu! §éi <color=wood>"..tbCaptainName[1].." <color>®· kh«ng vµo l«i ®µi. Hñy bá thi ®Êu! "
		Msg2MSAll(ZBW.tbMission.MAIN, sStr)
		SetMissionV(ZBW.tbMission.STATE, 3)
		CloseMission(ZBW.tbMission.MAIN)
		return
	end

	if(nGroup2PlayerCount <= 0 ) then 
		sStr = "§· ®Õn giê thi ®Êu! §éi <color=wood>"..tbCaptainName[2].." <color>®· kh«ng vµo l«i ®µi. Hñy bá thi ®Êu! "
		Msg2MSAll(ZBW.tbMission.MAIN, sStr)
		SetMissionV(ZBW.tbMission.STATE, 3)
		CloseMission(ZBW.tbMission.MAIN)
		return
	end
	Msg2MSAll(ZBW.tbMission.MAIN, "L«i ®µi tû vâ chÝnh thøc b¾t ®Çu! ")
	RunMission(ZBW.tbMission.MAIN)
end

----------------------------------
-- Th«ng b¸o thêi gian chuÈn bÞ
----------------------------------
function ZBW:ReportMemberState(nTime)
	if(nTime == ZBW.GO_TIME) then
		self:BeginCompete()
		return nil
	end
	
	local nRestMin = floor((self.GO_TIME - nTime) / 3)
	local nRestSec = mod((self.GO_TIME - nTime),3) * 20
	local szStr = ""
	if( (nRestMin > 0) and (nRestSec == 0) ) then
		szStr = "Thêi gian b¾t ®Çu thi ®Êu cßn <color=yellow>"..nRestMin.."<color> phót. Xin h·y chuÈn bÞ. "
		Msg2MSAll(self.tbMission.MAIN, szStr)
	elseif(nRestMin == 0) then
		szStr = "Thêi gian b¾t ®Çu thi ®Êu cßn <color=yellow>"..nRestSec.."<color> gi©y. "
		Msg2MSAll(self.tbMission.MAIN, szStr)
	end
	return szStr
end

----------------------------------
-- TÝnh ®iÓm
----------------------------------
function ZBW:CalcDamagePoint(nGroup)
	local OldPlayerIndex = PlayerIndex
	local i, nPlayerIdx
	local nPoint, nCount = 0, 0
	local nMSID = 0
	if(nGroup == 1) then
		nMSID = self.tbMission.INDEX_GROUP1
	else
		nMSID = self.tbMission.INDEX_GROUP2
	end
	for i = 1, GetMissionV(self.tbMission.MEMBER_COUNT) do
		nPlayerIdx = GetMissionV(nMSID + i) 
		if(nPlayerIdx <= 0)then
			break
		end
		PlayerIndex = nPlayerIdx
		nCount = GetDamageCounter() 
		if(nCount == nil) then
			nCount = 0
		end
		nPoint = nPoint + nCount
	end
	PlayerIndex = OldPlayerIndex
	return nPoint
end

----------------------------------
-- KiÓm tra ®éi th¾ng
----------------------------------
function ZBW:CheckWiner()
	local nGroup1PlayerCount = GetMSPlayerCount(self.tbMission.MAIN, 1)
	local nGroup2PlayerCount = GetMSPlayerCount(self.tbMission.MAIN, 2)
	local nFinalWiner = -1
	-- KiÓm tra kÕt qu¶ dùa vµo sè ng­êi
	if( (nGroup1PlayerCount <= 0 ) and (nGroup2PlayerCount <= 0 ) ) then 
		nFinalWiner = 0
	end
	if(nGroup1PlayerCount <= 0) then 
		nFinalWiner = 2
	end
	if(nGroup2PlayerCount <= 0) then 
		nFinalWiner = 1
	end
	-- KiÓm tra kÕt qu¶ dùa vµo ®iÓm
	if(nFinalWiner == 0) then
		local nPointGroup1 = self:CalcDamagePoint(2)
		local nPointGroup2 = self:CalcDamagePoint(1)
		if(nPointGroup1 == nPointGroup2) then
			nFinalWiner = 0
		elseif(nPointGroup1 > nPointGroup2) then
			nFinalWiner = 1
		elseif(nPointGroup1 < nPointGroup2) then
			nFinalWiner = 2
		end
		-- Th«ng b¸o sè ®iÓm
		Msg2MSAll(self.tbMission.MAIN, "§éi <color=wood>"..tbCaptainName[1].." <color><color=yellow>"..nPointGroup1.." ®iÓm<color> - §éi <color=wood>"..tbCaptainName[2].." <color><color=yellow>"..nPointGroup2.." ®iÓm<color>.")
	end
	-- Th«ng b¸o kÕt qu¶
	local szStr
	if(nFinalWiner == 0) then
		szStr = "§éi <color=wood>"..tbCaptainName[1].."<color> cïng ®éi <color=wood>"..tbCaptainName[2].."<color> ngang tµi ngang søc! "
		self:AllGone(szStr)
	elseif(nFinalWiner == 1) then
		szStr = "§éi <color=wood>"..tbCaptainName[1].."<color> ®¸nh b¹i ®éi <color=wood>"..tbCaptainName[2].."<color>, giµnh ®­îc th¾ng lîi cuèi cïng! "
		self:AllGone(szStr)
	elseif(nFinalWiner == 2) then
		szStr = "§éi <color=wood>"..tbCaptainName[2].."<color> ®¸nh b¹i ®éi <color=wood>"..tbCaptainName[1].."<color>, giµnh ®­îc th¾ng lîi cuèi cïng! "
		self:AllGone(szStr)
	end
	return szStr
end

----------------------------------
-- Th«ng b¸o thêi gian ®ang chiÕn ®Êu
----------------------------------
function ZBW:ReportBattle(nTime)
	local szStr = self:CheckWiner()
	if(szStr ~= nil) then
		return
	end
	local nGameTime = (floor(GetMSRestTime(self.tbMission.MAIN, self.TOTALTIMER)/18))
	local nRestMin = floor(nGameTime / 60)
	local nRestSec = mod(nGameTime, 60)
	if(nRestMin == 0) then
		Msg2MSAll(self.tbMission.MAIN, "Thêi gian cßn <color=yellow>"..nRestSec.."<color> gi©y. ")
	elseif(nRestSec == 0) then
		Msg2MSAll(self.tbMission.MAIN, "Thêi gian cßn <color=yellow>"..nRestMin.."<color> phót. ");
	else
		Msg2MSAll(self.tbMission.MAIN, "Thêi gian cßn <color=yellow>"..nRestMin.."<color> phót <color=yellow>"..nRestSec.."<color> gi©y. ")
	end
end

----------------------------------
-- Th«ng b¸o tin tøc ®éi tr­ëng
----------------------------------
function ZBW:NoticeCaptainNews(szStr)
	local nCount = GetMissionV(self.tbMission.MEMBER_COUNT)
	if(nCount <= 1) then
		return
	end
	local i
	local OldPlayerIndex = PlayerIndex
	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(self.FIGHT_MAP)
	local nCaptainIdx, szName
	for i = 1, 2 do
		nCaptainIdx = SearchPlayer(GetMissionS(i))
		if(nCaptainIdx > 0) then
			PlayerIndex = nCaptainIdx
			if( (PIdx2MSDIdx(self.tbMission.MAIN, nCaptainIdx) <= 0) and (szStr ~= nil) ) then
				Msg2Player(szStr)
			end
		end
	end
	SubWorld = OldSubWorld
	PlayerIndex = OldPlayerIndex
end

----------------------------------
-- KÕt thóc trËn chiÕn
----------------------------------
function ZBW:AllGone(szStr)
	Msg2MSAll(self.tbMission.MAIN, szStr)
	SetMissionV(self.tbMission.STATE, 3)
	self:NoticeCaptainNews(szStr)
	CloseMission(self.tbMission.MAIN)
end

----------------------------------
--
----------------------------------
function ZBW:NoticeCaptainKey(szStr)
	local nCount = GetMissionV(self.tbMission.MEMBER_COUNT)
	if(nCount <= 1) then
		return
	end
	
	local i
	local OldPlayerIndex = PlayerIndex
	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(self.FIGHT_MAP)
	local nCaptainIdx, szName, nKey, szStr1
	for i = 1, 2 do
		nCaptainIdx = SearchPlayer(GetMissionS(i))
		if(nCaptainIdx > 0) then
			PlayerIndex = nCaptainIdx
			if( (PIdx2MSDIdx(BW_MISSIONID, nCaptainIdx) <= 0) and (szStr ~= nil) ) then
				Msg2Player(szStr)
			end
			szName = GetMissionS(i)
			nKey = GetMissionV(self.tbMission.TIMER + i)
			nCount = GetMissionV(self.tbMission.MEMBER_COUNT)
			szStr1 = "M· trËn ®Êu lµ <color=yellow>"..nKey.."<color>. H·y th«ng b¸o cho c¸c thµnh viªn nhanh chãng vµo l«i ®µi! "
			Msg2Player(szStr1)
		end
	end
	SubWorld = OldSubWorld
	PlayerIndex = OldPlayerIndex
end

----------------------------------
-- KÕt thóc trËn ®Êu
----------------------------------
function ZBW:GameOver()
	local tbPlayer = {}
	local i, nIdx, nPlayerIdx = 0,0,0,0
	local nCount = 0
	for i = 1, 300 do 
		nIdx, nPlayerIdx = GetNextPlayer(self.tbMission.MAIN, nIdx, 0)
		if(nPlayerIdx > 0) then
			nCount = nCount + 1
			tbPlayer[nCount] = nPlayerIdx
		end
		if(nIdx == 0) then 
			break
		end
	end
	local OldPlayerIndex = PlayerIndex
	for i = 1, nCount do 
		PlayerIndex = tbPlayer[i]
		self:LeaveMap()
	end
	PlayerIndex = OldPlayerIndex	
end