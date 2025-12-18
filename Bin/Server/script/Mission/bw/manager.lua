----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: 
----------------------------------
Include("\\script\\system_config.lua")
Include("\\script\\mission\\bw\\functions.lua")

tbCaptainName = {}
----------------------------------
--
----------------------------------
function main()
	if(SYS_SWITCH_BW == 0) then
		Talk(1,"","TÝnh n¨ng nµy hiÖn t¹i t¹m ®ãng.")
		return
	end
	local nW,nX,nY = GetWorldPos()
	SetTask(ZBW.tbTask.SIGN_WORLD, nW)
	SetTask(ZBW.tbTask.SIGN_POSX, nX)
	SetTask(ZBW.tbTask.SIGN_POSY, nY)

	local nIdx = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(nIdx == -1) then
		ErrorMsg(3) 
		return
	end
	
	local OldSubWorld = SubWorld
	SubWorld = nIdx
	local nState = GetMissionV(ZBW.tbMission.STATE)
	tbCaptainName = ZBW:GetCaptainName()
	if(nState == 0) then 
		SayImage(10394,"44/44/41",3,"§­îc th«i./OnRegister","Quy t¾c thi ®Êu ra sao?/OnHelp", "§Ó suy nghÜ c¸i ®·./OnCancel")
	elseif(nState == 1) then
		local nMemCount = GetMissionV(ZBW.tbMission.MEMBER_COUNT)
		local sInfo = "<color=gold>C«ng B×nh Tö <color>: Hai chiÕn ®éi <color=wood>"..tbCaptainName[1].." <color>- <color=wood>"..tbCaptainName[2].." <color>tranh tµi <color=yellow>"..nMemCount.." vs "..nMemCount.."<color>. Ng­¬i cÇn g×?"
		SayImage(sInfo,"44/44/41",4,"Ta muèn kiÓm tra m· trËn ®Êu./OnShowKey", "Ta lµ tuyÓn thñ, muèn th­îng ®µi./OnEnterMatch", "Ta lµ kh¸n gi¶, muèn xem thi ®Êu./OnLook", "Ta kh«ng cã høng thó./OnCancel")
	elseif(nState == 2) then 
		OnFighting()
	else
		ErrorMsg(2)
	end
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnRegister()
	if(GetTeamSize() ~= 2) then
		Say(10403, 0)
		return
	end

	if(IsCaptain() ~= 1) then 
		ErrorMsg(5)
		return
	end

	Say(10405,9, "Rêi khái!/OnCancel", "1 vs 1/SignUpFinal#1", "2 vs 2/SignUpFinal#2", "3 vs 3/SignUpFinal#3","4 vs 4/SignUpFinal#4","5 vs 5/SignUpFinal#5","6 vs 6/SignUpFinal#6","7 vs 7/SignUpFinal#7","8 vs 8/SignUpFinal#8")
end

----------------------------------
--
----------------------------------
function SignUpFinal(szMemberCount)
	local nCount = tonumber(szMemberCount)
	if(nCount == nil) then
		return
	end
	if( (nCount <= 0) or (nCount > 8) ) then
		return
	end
	if(GetTeamSize() ~= 2) then
		Say(10403, 0)
		return
	end

	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBW.FIGHT_MAP)
	local nState = GetMissionV(ZBW.tbMission.STATE)

	if(nState ~= 0) then 
		ErrorMsg(8)
		return
	end
	OpenMission(ZBW.tbMission.MAIN)
	
	local nW = GetTask(ZBW.tbTask.SIGN_WORLD)
	if(nW == 80) then
		SetMissionS(3,"D­¬ng Ch©u")
	elseif(nW == 78) then
		SetMissionS(3,"T­¬ng D­¬ng")
	else
		SetMissionS(3,"V« hÖ")
	end
	
	local tbKey = {}
	tbKey = ZBW:GetKey()
	SetMissionV(ZBW.tbMission.KEY_TEAM_1, tbKey[1])
	SetMissionV(ZBW.tbMission.KEY_TEAM_2, tbKey[2])
	
	local OldPlayerIndex = PlayerIndex
	for i = 1, 2 do 
		PlayerIndex = GetTeamMember(i);
		SetMissionS(i, GetName())
		tbCaptainName = ZBW:GetCaptainName()
		if(nCount > 1) then
			Msg2Player("§¹i hiÖp ®· ®¨ng ký l«i ®µi <color=green>"..nCount.." vs "..nCount.."<color>.")		
			local szMsg = format("L«i ®µi <color=green>%d vs %d<color>\nChiÕn ®éi: <color=wood>%s<color>\nM· trËn ®Êu: <color=fire>%d<color>", nCount,nCount,tbCaptainName[i],tbKey[i])
			Say(szMsg,0)
		end
	end	
	PlayerIndex = OldPlayerIndex
	SetMissionV(ZBW.tbMission.MEMBER_COUNT, nCount)
	SubWorld = OldSubWorld
	local szMsg = format("C¸c thµnh viªn nhanh chãng vµo l«i ®µi! <color=yellow>%d phót<color> sau tranh tµi chÝnh thøc b¾t ®Çu.", floor(ZBW.GO_TIME/3))
	Msg2Team(szMsg)
end

----------------------------------
--
----------------------------------
function OnHelp()
	Talk(3,"",10407,10408,10409)
end

----------------------------------
--
----------------------------------
function OnShowKey()
	ZBW:OnShowKey()
end

----------------------------------
--
----------------------------------
function OnEnterMatch()
	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(SubWorld < 0) then
		SubWorld = OldSubWorld
		return
	end
	
	local szName = GetName()
	if ((szName == tbCaptainName[1]) or (szName == tbCaptainName[2])) then 
		OnJoin(0)
	else
		Say(10412,2,"H·y khoan, cho ta lªn./OnEnterKey", "Ta kh«ng nhí, chê mét chót ®Ó ta hái §éi tr­ëng./OnCancel")
	end;
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnEnterKey()
	AskClientForNumber("OnEnterKey1", 0, 10000, "M· trËn ®Êu")
end

----------------------------------
--
----------------------------------
function OnEnterKey1(nNothing,nKey)
	nKey = tonumber(nKey)
	if(nKey == nil) then
		return
	end
	local OldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(SubWorld < 0) then
		SubWorld = OldSubWorld
		return
	end
	if(nKey == GetMissionV(ZBW.tbMission.KEY_TEAM_1)) then
		OnJoin(1)
	elseif(nKey == GetMissionV(ZBW.tbMission.KEY_TEAM_2)) then
		OnJoin(2)
	else
		Say(10414,0)
		Msg2Player(""..nKey.."");
	end
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnJoin(nGroup)
	local nIdx = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(nIdx < 0) then
		return
	end
	local OldSubWorld = SubWorld
	SubWorld = nIdx
	local szName = GetName()
	if(szName == tbCaptainName[1]) then 
		JoinMission(ZBW.tbMission.MAIN, 1)
	elseif(szName == tbCaptainName[2]) then 
		JoinMission(ZBW.tbMission.MAIN, 2)
	elseif( (nGroup == 1) or (nGroup == 2)) then
		local nCaptainIdx = SearchPlayer(tbCaptainName[nGroup])
		local nCaptainNum = 0
		if(nCaptainIdx > 0) then
			if(PIdx2MSDIdx(ZBW.tbMission.MAIN, nCaptainIdx) > 0) then
				nCaptainNum = 1
			end
		end
		if( (GetMSPlayerCount(ZBW.tbMission.MAIN, nGroup) - nCaptainNum) < (GetMissionV(ZBW.tbMission.MEMBER_COUNT) - 1) ) then
			JoinMission(ZBW.tbMission.MAIN, nGroup)
		else
			ErrorMsg(10)
		end
	elseif(nGroup == 3) then
		JoinMission(ZBW.tbMission.MAIN, 3)
	else
		ErrorMsg(4)
	end
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnLook()
	local nIdx = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(nIdx == -1) then
		return
	end
	local OldSubWorld = SubWorld
	SubWorld = nIdx
	local szInfo = "§éi <color=wood>"..tbCaptainName[1].." <color>vµ ®éi <color=wood>"..tbCaptainName[2].." <color> ®· s½n sµng thi ®Êu, c¸c h¹ nghÜ ai sÏ lµ ng­êi chiÕn th¾ng cuèi cïng?"
	local str1 = "Ta muèn vµo l«i ®µi<#> xem/onwatch";
	Say(szInfo, 2, "Ta muèn vµo l«i ®µi./OnWatch", "Kh«ng ph¶i chuyÖn cña ta./OnCancel")
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnWatch()
	local nIdx = SubWorldID2Idx(ZBW.FIGHT_MAP)
	if(nIdx == -1) then
		return
	end
	local OldSubWorld = SubWorld
	SubWorld = nIdx
	local szName = GetName()
	if( (szName == tbCaptainName[1]) or (szName == tbCaptainName[2]) ) then 
		OnJoin(0)
	else
		OnJoin(3)
	end
	SubWorld = OldSubWorld
end

----------------------------------
--
----------------------------------
function OnFighting()
	local szInfo = "HiÖn t¹i ®éi <color=wood>"..tbCaptainName[1].." <color>vµ ®éi <color=wood>"..tbCaptainName[2].." <color>®ang thi ®Êu!"
	Say(szInfo, 1, "Quay l¹i sau./OnCancel")
end

----------------------------------
--
----------------------------------
function OnCancel()
end

----------------------------------
--
----------------------------------
function ErrorMsg(nErrorId)
	if (nErrorId == 1) then 
		Say("Muèn b¸o danh ph¶i häp thµnh ®éi sau ®ã míi cã thÓ b¸o danh",0)
	elseif (nErrorId == 2) then 
		Say("Tham gia thi ®Êu ph¶i mang ®ñ ng©n l­îng",0)
	elseif (nErrorId == 3) then 
		Say(10002,0)
	elseif (nErrorId == 4) then 
		Say(10413, 0)
	elseif (nErrorId == 5) then 
		Say(10404,0)
	elseif (nErrorId == 6) then 
		Say("Ng­¬i mang kh«ng ®ñ ng©n l­îng",0);
	elseif (nErrorId == 7) then 
		Say("Kh«ng ®­îc, c¸c h¹ cßn ®ang tham gia tranh tµi!",0);
	elseif (nErrorId == 8) then
		Say(10406,0)
	elseif (nErrorId == 9) then 
		Say("Kh«ng ®­îc, l«i ®µi ®· b¾t ®Çu, c¸c h¹ kh«ng thÓ ®ãng phÝ!",0);
	elseif (nErrorId == 10) then 
		Say(10415,0)
	end
end
