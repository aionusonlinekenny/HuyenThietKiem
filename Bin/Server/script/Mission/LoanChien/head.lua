--Author: Kinnox;
--Date: 16/05/2022;
--function: loan chien;
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\player\\head.lua")
Include("\\script\\system_config.lua");
LC = {};
FRAME2TIME = 18;
BOOLCall_PK = 0;
LC.MAIN ={
	MISSION_LC	 		= 2, 	-- ID mission; 
	STATE				= 1,	-- STATE; [MissionV]
	PLAYER_DEATH 		= "\\script\\mission\\loanchien\\player_death.lua",	--script player death;
	PLAYER_LOGOUT		= "\\script\\mission\\loanchien\\player_logout.lua",	--script player logout;
	MAP_FIGHT			= 372,
};

LC.TIMER ={
	TIMER_LANGDING 		= 11, -- ID timer trong setting\timertask.lua
	TIMER_CHECKPK		= 12, -- ID timer trong setting\timertask.lua
	TIME_START_CHECKPK  = 0,  -- when == 1 start TIMER_CHECKPK
	TIMER_TICK_SECCON	= 20*FRAME2TIME, -- count time mission 20 seccons;
	TIMER_TICK_SECCONPK = 18, -- *FRAME2TIME, -- count time mission 0.18 seccons;
	TIMER_COUNT_SECCON	= 2, -- count time mission 20 seccons; [MissionV]
	TIMER_COUNT_MIN		= 3, -- count time mission TIMER_COUNT_SECCON*3; [MissionV]
	TIMER_NOTIFI1		= 1,  --timer call notifi;
	TIMER_NOTIFI2		= 3, --timer call notifi;
	TIMER_NOTIFI3		= 4, --timer call notifi;
	TIMER_WAITING		= 5, --waiting register 5 minute;
	TIMER_START			= 6, --start if misnute == 6;
	TIMER_NOTIFI4		= 15, --timer call notifi;
	TIMER_END			= 20,
};

LC.TASK ={
	TCOUNT_DEATH				= 603, -- count death;
};

 ---------------TIME  MISSION RUNNING
function LC:TIMER_LANGDING()
	SubWorld = SubWorld;
	if (GetMissionV(self.MAIN.STATE) == 1) then
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			----Time Start
			if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING) then
				--Anti Close Mission;
				if (GetMSPlayerCount(self.MAIN.MISSION_LC,1) <= 1) then
					Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>KÕt thóc ho¹t ®éng lo¹n chiÕn, kh«ng cã ®ñ hµo kiÖt thi ®Êu!<color>");
					CloseMission(self.MAIN.MISSION_LC);
					return
				end;
				---
				SetMissionV(self.MAIN.STATE,2) -- run start;
				return
			end;
		end;
	end;
	
	----Mission Langning
	if (GetMissionV(self.MAIN.STATE) == 2) then
		--Anti Close Mission;
		if (GetMSPlayerCount(self.MAIN.MISSION_LC,1) <= 1) then
			Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>KÕt thóc ho¹t ®éng lo¹n chiÕn, kh«ng cã ®ñ hµo kiÖt thi ®Êu!<color>");
			CloseMission(self.MAIN.MISSION_LC);
			return
		end;
		---
	
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		--Notifi warning;
		if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING and nTimerMinutes > 15 and nTimerMinutes < 18) then
			Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>Cßn "..(60 - (nTimerMinutes-15)*20).."gi©y n÷a sÏ b¾t ®Çu.<color>");
		end;
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			local nMinuteStart = GetMissionV(self.TIMER.TIMER_COUNT_MIN);
			if (nMinuteStart == self.TIMER.TIMER_START) then
				self:SetPlayerPK(SubWorld); -- add player maps;
				StartMissionTimer(self.MAIN.MISSION_LC, self.TIMER.TIMER_CHECKPK, self.TIMER.TIMER_TICK_SECCONPK); -- 0.18s
				Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>Thêi gian kÕt thóc lo¹n chiÕn cßn 20 phót<color>");
			end;		
	
			-- Notifi 5 final;
			if (nMinuteStart == self.TIMER.TIMER_NOTIFI4) then
				Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>Cßn 5 phót n÷a sÏ kÕt thóc ho¹t ®éng lo¹n chiÕn!<color>");
			end;
			-- End mission;
			if (nMinuteStart == self.TIMER.TIMER_END) then
				Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>KÕt thóc ho¹t ®éng lo¹n chiÕn!<color>");
				CloseMission(self.MAIN.MISSION_LC); -- finish close;
			end;
		end;
	end;
	
	if (GetMissionV(self.MAIN.STATE) ~= 1 and GetMissionV(self.MAIN.STATE) ~= 2) then
		-- Truong hop sai;
		--Anti Close Mission;
		if (GetMSPlayerCount(self.MAIN.MISSION_LC,1) <= 1 ) then
			Msg2MSAll(self.MAIN.MISSION_LC,"<color=cyan>KÕt thóc ho¹t ®éng lo¹n chiÕn, kh«ng cã ®ñ hµo kiÖt thi ®Êu!<color>");
			CloseMission(self.MAIN.MISSION_LC);
			return
		end;
		---
	end	
end;
---------------END TIME  MISSION RUNNING

 ---------------TIME  CHECK PK MISSION RUNNING
function LC:TIMER_CHECKPK()
	local OldSubWorld = SubWolrd;
		SubWorld = SubWorldID2Idx(self.MAIN.MAP_FIGHT);
		self:SetPlayerPK(SubWorld); -- add player maps;
	SubWorld = OldSubWorld;
end;
---------------END TIME CHECK PK MISSION RUNNING

-------------CONTROLER
 function LC:ACTIVE_LoanChien() --public function to realtimer.lua
 local nHr = tonumber(date("%H"));
 local nMi = tonumber(date("%M"));
 local OldSubWorld = SubWolrd;

	-- bao danh 18h55 di 19h00;
	if( nHr == 18) then
	 	--Time OpenMission
		SubWorld = SubWorldID2Idx(self.MAIN.MAP_FIGHT);
		self:Notifi()--call notifi;
		if (nMi == 55) then
			SubWorld = SubWorldID2Idx(self.MAIN.MAP_FIGHT); -- loan chien map;
			OpenMission(self.MAIN.MISSION_LC);
			for i = 1,SYS_MAX_PLAYER do
				PlayerIndex = i;
				SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Lo¹n chiÕn<color> ®ang trong thêi gian b¸o danh h·y mau chãng tham gia <pic="..random(1,140).."> <pic="..random(1,140).."> !");
			end;
		end;
	end;
	SubWorld = OldSubWorld;
	--/.
 end;
 
 function LC:Notifi()
	----Time notifi
	if (GetMissionV(self.MAIN.STATE) == 1) then
		if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI1 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI2 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI3) then
			AddGlobalCountNews("<color=cyan>Lo¹n chiÕn<color> s¾p tíi thêi gian lo¹n chiÕn b¾t ®Çu.", 3)	
			Msg2SubWorld("<color=cyan>Lo¹n chiÕn<color> s¾p hÕt thêi gian b¸o danh lo¹n chiÕn, h·y mau chãng gÆp Npc Lo¹n ChiÕn ë 192/202 T­¬ng d­¬ng. Thêi gian cßn l¹i "..(5-GetMissionV(self.TIMER.TIMER_COUNT_MIN)).." phót.");	
		end;
	end;
 end;
 ----------
 
 function InitMission()
	local nMapID = SubWorldIdx2ID(SubWorld);	
	SetMissionV(LC.MAIN.STATE, 1);
	LC:Start_TIMER_LANGDING();
	print("Init mission Loan Chien: ",nMapID);
end;

function EndMission()
	nMapID = SubWorldIdx2ID(SubWorld);
	LC:Close_TIMER_LANGDING();
	LC:FinishMS(nMapID);
	SetMissionV(LC.MAIN.STATE,0); --reset mission;
	SetMissionV(LC.TIMER.TIMER_COUNT_SECCON,0); --reset mission;
	SetMissionV(LC.TIMER.TIMER_COUNT_MIN,0); --reset mission;
	print("Close mission Loan Chien: ",nMapID);
end;

function OnLeave(RoleIndex)
	PlayerIndex = RoleIndex;
	local OldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(LC.MAIN.MAP_FIGHT);
	LC:LeaveGame(PlayerIndex,0);
	SubWorld = OldSubWorld;
end


function LC:Start_TIMER_LANGDING()
	StartMissionTimer(self.MAIN.MISSION_LC, self.TIMER.TIMER_LANGDING, self.TIMER.TIMER_TICK_SECCON); -- 20s
end;

function LC:Close_TIMER_LANGDING()
	StartMissionTimer(self.MAIN.MISSION_LC, self.TIMER.TIMER_CHECKPK, self.TIMER.TIMER_TICK_SECCONPK); -- 0.18s
	StopMissionTimer(self.MAIN.MISSION_LC, self.TIMER.TIMER_LANGDING);
end;

function LC:FinishMS(nMapID)
	local OldSubWorld = SubWorld;
		SubWorld = SubWorldID2Idx(nMapID);
		Msg2MSAll(self.MAIN.MISSION_LC, "Lo¹n chiÕn kÕt thóc.")
		local tbPlayer = {};
		nIndexPlayer = 0;
		for i = 1 , 100 do 
			nIndexPlayer, pIndexPlayer = GetNextPlayer(self.MAIN.MISSION_LC,nIndexPlayer, 0)
			if (pIndexPlayer > 0) then
				tbPlayer[i] = pIndexPlayer;
			end;
			
			if (nIndexPlayer == 0) then 
				break
			end;
		end;
		
		for i= 1, getn(tbPlayer) do 
			PlayerIndex = tbPlayer[i]
			OnLeave(PlayerIndex);
		end;
end;

function LC:NpcTalk(nPlayerIndex)
	local szHello = "";
	local szSay = {
		"Ta muèn b¸o danh Lo¹n ChiÕn/JoinMS#"..nPlayerIndex.."",
		"Ta chØ ghÐ ngang qua/OnCancel"
	}
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nPlayerIndex;
		SubWorld = SubWorldID2Idx(LC.MAIN.MAP_FIGHT);

	if (GetMissionV(self.MAIN.STATE) > 1) then
		szHello = "<color=orange>Lo¹n ChiÕn T­¬ng Qu©n<color>: Lo¹n chiÕn ®· hÕt thêi gian b¸o danh, ng­êi h·y ®îi trËn kÕ tiÕp vµo <color=red>18h55<color>!";
		Say(szHello,1,szSay[2]);
	elseif (GetMissionV(self.MAIN.STATE) == 1) then
		szHello = "<color=orange>Lo¹n ChiÕn T­¬ng Qu©n<color>: Lo¹n chiÕn ®ang trong thêi gian b¸o danh, c¸c h¹ ®· s½n sµng tham chiÕn ch­a?";
		Say(szHello,2,szSay[1],szSay[2]);
	else
		szHello = "<color=orange>Lo¹n ChiÕn T­¬ng Qu©n<color>: Ch­a ®Õn thêi gian tham gia b¸o danh c¸c h¹ véi vµng g× chø, ®îi ®Õn <color=red>18h55<color> nhÐ!";
		Say(szHello,1,szSay[2]);
	end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;

function JoinMS(nNo,nPlayerIndex)
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
	local nIDMaps = LC.MAIN.MAP_FIGHT;
		PlayerIndex = nPlayerIndex;
		SubWorld = SubWorldID2Idx(nIDMaps);
	ZPlayer:ResetTask(); -- kiem tra reset Task;
	SetTitle(0);
	SetCurCamp(4);
	LeaveTeam();   
	SetPunish(1);
	SetFightState(0);
	ForbidChangePK(1);
	-- SetTempRevPos(372,random(1,8));
	SetTempRevPos(372,50336,100704);
	AddMSPlayer(LC.MAIN.MISSION_LC,1);
	posx, posy = lc_getadata(npcMAP_POS);
	posx = floor(posx/32)
	posy = floor(posy/32)
	NewWorld(nIDMaps, posx, posy);
	ForbidTownPortal(1);
	SetTask(LC.TASK.TCOUNT_DEATH,3);
	SetDeathScript(LC.MAIN.PLAYER_DEATH);
	SetLogoutScript(LC.MAIN.PLAYER_LOGOUT); 
	SubWorld = OldSubWorld;
	PlayerIndex = OldPlayerIndex;
end;

function LC:LeaveGame(nPlayerIndex,nSel)
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nPlayerIndex;
		SubWorld = SubWorldID2Idx(LC.MAIN.MAP_FIGHT);
	DelMSPlayer(self.MAIN.MISSION_LC,1);
	camp = GetCamp();
	SetCurCamp(camp)
	SetCreateTeam(1)
	SetLogoutScript(""); 
	SetDeathScript("");  
	ForbidChangePK(0);
	SetFightState(0);
	SetPunish(0);
	ForbidTownPortal(0);
	SetRevPos(78,32)
	NewWorld(78,1545,3241);

	SetTask(LC.TASK.TCOUNT_DEATH,0);
	if (nSel == 0) then -- finishMS
		NewWorld(78,1545,3241);
	elseif (nSel == 1) then --LogoutMS
		NewWorld(53,1616,3186);
	else -- DieMS
	
	end;
	
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;

function LC:SetPlayerPK(SubWorld)
	local OldPlayerIndex = PlayerIndex;
	local OldSubWorld = SubWorld;
	local tbPlayer = {};
		nIndexPlayer = 0;
		for i = 1 , 100 do 
			nIndexPlayer, pIndexPlayer = GetNextPlayer(self.MAIN.MISSION_LC,nIndexPlayer, 0)
			if (pIndexPlayer > 0) then
				tbPlayer[i] = pIndexPlayer;
			end;
			
			if (nIndexPlayer == 0) then 
				break
			end;
		end;
		
		for i= 1, getn(tbPlayer) do 
			PlayerIndex = tbPlayer[i]
			SetFightState(1);
			ForbidChangePK(0); -- mo khoa pk;
			SetPKFlag(1);
			ForbidChangePK(1); -- dong khoa pk;
			SetCurCamp(4);
		end;
	SubWorld  = OldSubWorld;
	PlayerIndex = OldPlayerIndex;
end;

function lc_getadata(file)
	local totalcount = getn(npcMAP_POS);
	id = random(1, totalcount);
	x = npcMAP_POS[id][1];
	y = npcMAP_POS[id][2];
	return x,y
end

npcMAP_POS = {
{50336,100704},
{50304,101312},
{50560,100928},
{51008,100576},
{50720,101728},
{50368,102240},
{50784,102304},
{50976,102016},
{51072,102496},
}

function OnCancel()

end;