----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 12/03/2023
--	Desc: Mission Tèng Kim
----------------------------------
Include("\\script\\mission\\battles\\functions.lua")
----------------------------------
-- Khëi ®éng Misson
----------------------------------
function InitMission()
	local mapfile = GetMapInfoFile(ZBattle.FIGHT_MAP)
	local areacount = tonumber(ZBattle:GetIniFileData(mapfile, "MapInfo", "Area_Count"))
	local s_area, j_area = ZBattle:AutoSelectMapArea(mapfile, areacount)
	SetMissionV(ZBattle.tbMission.AREASONG, tonumber(s_area))
	SetMissionV(ZBattle.tbMission.AREAJIN, tonumber(j_area))
	local area_section1 = "Area_"..s_area
	local area_section2 = "Area_"..j_area
	local file = ""
	local x,y = 0,0
	SetGlbMissionV(25,60); -- ZBattle.tbMission.TIMERSHOW;
	file = ZBattle:GetIniFileData(mapfile, area_section1 , "centertohomepos")
    x, y =ZBattle:GetAData(file)
	SetMissionV(ZBattle.tbMission.HOMEIN_X1, tonumber(floor(x/32)))
	SetMissionV(ZBattle.tbMission.HOMEIN_Y1, tonumber(floor(y/32)))
	
	file = ZBattle:GetIniFileData(mapfile, area_section2 , "centertohomepos")
    x, y =ZBattle:GetAData(file)
	SetMissionV(ZBattle.tbMission.HOMEIN_X2, tonumber(floor(x/32)))
	SetMissionV(ZBattle.tbMission.HOMEIN_Y2, tonumber(floor(y/32)))
	
	file = ZBattle:GetIniFileData(mapfile, area_section1 , "hometocenterpos")
	SetMissionS(1, file)
	x,y = ZBattle:GetAData(file)
	SetMissionV(ZBattle.tbMission.HOMEOUT_X1, tonumber(floor(x/32)))
	SetMissionV(ZBattle.tbMission.HOMEOUT_Y1, tonumber(floor(y/32)))
	
	file = ZBattle:GetIniFileData(mapfile, area_section2 , "hometocenterpos")
	SetMissionS(2, file)
	x,y = ZBattle:GetAData(file)
	SetMissionV(ZBattle.tbMission.HOMEOUT_X2, tonumber(floor(x/32)))
	SetMissionV(ZBattle.tbMission.HOMEOUT_Y2, tonumber(floor(y/32)))
	
	local hometrapfile = ZBattle:GetIniFileData(mapfile, area_section1, "hometrap")
	ZBattle:AddTrapSJ(hometrapfile, ZBattle.tbFile.FILE_HOMETRAP1)
	hometrapfile = ZBattle:GetIniFileData(mapfile, area_section2, "hometrap");
	ZBattle:AddTrapSJ(hometrapfile, ZBattle.tbFile.FILE_HOMETRAP2)
	
	local centertohometrapfile = ZBattle:GetIniFileData(mapfile, area_section1, "centertohometrap")
	ZBattle:AddTrapSJ(centertohometrapfile, ZBattle.tbFile.FILE_CENTERTOHOMETRAP1)
	centertohometrapfile = ZBattle:GetIniFileData(mapfile, area_section2, "centertohometrap")
	ZBattle:AddTrapSJ(centertohometrapfile, ZBattle.tbFile.FILE_CENTERTOHOMETRAP2)
	
	local centertrapfile = ZBattle:GetIniFileData(mapfile, area_section1, "centertrap")
	ZBattle:AddTrapSJ(centertrapfile, ZBattle.tbFile.FILE_CENTERTRAP)
	centertrapfile = ZBattle:GetIniFileData(mapfile, area_section2, "centertrap")
	ZBattle:AddTrapSJ(centertrapfile, ZBattle.tbFile.FILE_CENTERTRAP)
	
	----------------------------
	--Hieu thuoc
	----------------------------
	-- local doctorxy = ZBattle:GetIniFileData(mapfile, area_section1, "doctornpc")
	-- x,y = ZBattle:Str2XYData(doctorxy)
	-- ZBattle:Add_A_DialogNpc(ZBattle.tbFile.FILE_DOCTOR1, ZBattle.DoctorId[1], x * 32 ,y * 32 , "Qu©n Nhu Quan (Tèng)")
	
	-- local doctorxy2 = ZBattle:GetIniFileData(mapfile, area_section2, "doctornpc")
	-- x,y = ZBattle:Str2XYData(doctorxy2)
	-- ZBattle:Add_A_DialogNpc(ZBattle.tbFile.FILE_DOCTOR2, ZBattle.DoctorId[2], x * 32, y * 32, "Kim Quèc Qu©n Nhu Quan")
	----------------------------
	--Ruong
	----------------------------
	local depositxy = ZBattle:GetIniFileData(mapfile, area_section1, "depositobj");
	x,y = ZBattle:Str2XYData(depositxy)
	ZBattle:Add_A_DialogNpc(ZBattle.tbFile.FILE_DEPOSIT, ZBattle.DepositId, x * 32,y * 32, "");
	
	local depositxy2 = ZBattle:GetIniFileData(mapfile, area_section2, "depositobj");
	x,y = ZBattle:Str2XYData(depositxy2)
	ZBattle:Add_A_DialogNpc(ZBattle.tbFile.FILE_DEPOSIT, ZBattle.DepositId, x * 32, y * 32, "");
	----------------------------
	--
	----------------------------
	local nGameRule = ZBattle:SetBattleRule() 
	SetMissionV(ZBattle.tbMission.MAIN, nGameRule)
	SetMissionV(ZBattle.tbMission.STATE, ZBattle.STATE_SIGNED)
	SetMissionV(ZBattle.tbMission.KEY_SJ, random(10) + tonumber(date("%d%H")))
	
	StartMissionTimer(ZBattle.tbMission.MAIN, ZBattle.TOTALTIMER, ZBattle.TIMER_2)
	StartMissionTimer(ZBattle.tbMission.MAIN, ZBattle.SMALLTIMER, ZBattle.TIMER_1)
	
	local nRestMin,_ = ZBattle:GetMinAndSec(ZBattle.BAOMING_TIME * 60)
	AddGlobalNews("ChiÕn tr­êng tèng kim ®· b¾t ®Çu. Thêi gian tíi b¸o danh lµ: <color=red>"..nRestMin.." phót<color>.")
	Msg2SubWorld("<color=green>ChiÕn tr­êng tèng kim ®· b¾t ®Çu. Thêi gian tíi b¸o danh lµ: <color=red>"..nRestMin.." phót<color>.")
	local nMapID = SubWorldIdx2ID(SubWorld);
	
	--free danh sach top
	SetGlbMissionV(ZBattle.tbMission.TONG_SOLIDER,0);
	SetGlbMissionV(ZBattle.tbMission.KIM_SOLIDER,0);
	print("Init mission Tong Kim: ",nMapID);
end

----------------------------------
--
----------------------------------
function RunMission()
	SetMissionV(ZBattle.tbMission.STATE, ZBattle.STATE_FIGHT)
	-- CËp nhËt d÷ liÖu
	local idx = 0
	local pidx = 0
	local oldPlayerIndex = PlayerIndex
	local i = 0
	for i = 1, 500 do 
		idx, pidx = GetNextPlayer(ZBattle.tbMission.MAIN,idx, 0)
		if(pidx > 0) then
			PlayerIndex = pidx
			SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH, GetGameTime())
		end
		if(idx <= 0) then 
			break
		end
	end
	PlayerIndex = oldPlayerIndex
end

----------------------------------
--
----------------------------------
function EndMission()
	SetMissionV(ZBattle.tbMission.STATE, 3)
	StopMissionTimer(ZBattle.tbMission.MAIN, ZBattle.TOTALTIMER)
	StopMissionTimer(ZBattle.tbMission.MAIN, ZBattle.SMALLTIMER)
	SetMissionV(ZBattle.tbMission.KEY_SJ,0)
	-- §­a ng­êi ch¬i ra ngoµi
	ZBattle:GameOver()
	-- Reset Mission
	local i = 0
	-- for i = 1, ZBattle.tbMission.MEMBER_COUNT do 
		-- SetMissionV(i , 0)
	-- end
	 for i = 1, 100 do 
		SetMissionV(i , 0)
	end
	SetGlbMissionV(22,0);
	SetGlbMissionV(23,0);
	SetGlbMissionV(24,0);
	SetGlbMissionV(26,0); -- Clear Top TK;
	
	SetMissionS(1,"")
	SetMissionS(2,"")
	-- ClearMapNpc(ZBattle.FIGHT_MAP)
	-- ClearMapTrap(ZBattle.FIGHT_MAP)
	-- ClearMapObj(ZBattle.FIGHT_MAP)
	
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng binh")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng HiÖu óy")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Thèng lÜnh")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Phã t­íng")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng §¹i t­íng")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Nguyªn So¸i")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim binh")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim HiÖu óy")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Thèng LÜnh")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Phã t­íng")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim §¹i t­íng")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Nguyªn So¸i")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Nguyªn So¸i")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Nguyªn So¸i")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng binh ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng HiÖu óy ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Thèng lÜnh ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Phã t­íng ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng §¹i t­íng ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Nguyªn So¸i ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim binh ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim HiÖu óy ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Thèng LÜnh ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Phã t­íng ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim §¹i t­íng ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Nguyªn So¸i ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Kim Nguyªn So¸i ")
	 ClearMapNpcWithName(ZBattle.FIGHT_MAP,"Tèng Nguyªn So¸i ")
	 local nMapID = SubWorldIdx2ID(SubWorld);	
	print("Close mission Tong Kim: ",nMapID);
end

----------------------------------
--
----------------------------------
function JoinMission(RoleIndex, nCamp)
	if((nCamp ~= 1) and (nCamp ~= 2)) then
		return
	end
	PlayerIndex = RoleIndex
	if (GetLockSongJin(GetAccount()) > 0) then
		Talk(1,"","Tèng kim hiÖn t¹i giíi h¹n <color=red>1/pc<color> nh©n vËt, ®· cã <color=red>1<color> nh©n vËt cña ®¹i hiÖp vµo råi!");
		return
	end;
	Talk(1,"","§¹i hiÖp thµnh c«ng di chuyÓn ®Õn hËu doanh");
	ZBattle:Join(nCamp)
end

----------------------------------
--
----------------------------------
function OnLeave(RoleIndex)
	PlayerIndex = RoleIndex
	ZBattle:UpdateMemberCount(-1)
	SetLockSongJin(GetAccount(),0); --Gioi han account SongJin by kinnox;
	RestoreCamp()
	ZBattle:LeaveGame()
	SetLogoutRV(0)

end
