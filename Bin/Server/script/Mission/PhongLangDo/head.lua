--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\player\\head.lua")
PLD = {};
FRAME2TIME = 18; 
PLD.MAIN ={
	MISSION_PLD 		= 4, 	-- ID mission; 
	STATE				= 1,	-- STATE; [MissionV]
	PLAYER_DEATH 		= "\\script\\mission\\phonglangdo\\player_death.lua",	--script player death;
	PLAYER_LOGOUT		= "\\script\\mission\\phonglangdo\\player_logout.lua",	--script player logout;
	NPC_DEATH 			= "\\script\\mission\\phonglangdo\\normal_death.lua",	--script npc death ( normal, gold npc);
	NPC_NORMAL 			= 724,	-- ID npc normal;
	NPC_BOSS 			= 725,	-- ID npc normal;
	BOATID				= 0,    -- ID ben thuyen;
};

PLD.TIMER ={
	TIMER_LANGDING 		= 10, -- ID timer trong setting\timertask.lua
	TIMER_TICK_SECCON	= 20*FRAME2TIME, -- count time mission 20 seccons;
	TIMER_COUNT_SECCON	= 2, -- count time mission 20 seccons; [MissionV]
	TIMER_COUNT_MIN		= 3, -- count time mission TIMER_COUNT_SECCON*3; [MissionV]
	TIMER_NOTIFI1		= 5,  --timer call notifi;
	TIMER_NOTIFI2		= 10, --timer call notifi;
	TIMER_NOTIFI3		= 14, --timer call notifi;
	TIMER_WAITING		= 15, --waiting register 15 minute;
	TIMER_START			= 16, --start if misnute == 16;
	TIMER_BOSS1			= 20, --timer call boss TIMER_START + 4; phut thu 5 sau khi bat dau;
	TIMER_BOSS2			= 30, --timer call boss TIMER_BOSS1 + 10; phut thu 15 sau khi bat dau;
	TIMER_BOSS3			= 35, --timer call boss TIMER_BOSS2 + 5; phut thu 20 sau khi bat dau;
	TIMER_NOTIFI4		= 40, --timer call notifi;
	TIMER_END			= 45,
};

PLD.TASK ={
	TPLD_JOIN_BOAT 			= 600, -- count join;
	TPLD_JOIN_MACBAC 		= 601, -- count join;
};

 ---------------TIME  MISSION RUNNING
function PLD:TIMER_LANGDING()
	SubWorld = SubWorld;
	if (GetMissionV(self.MAIN.STATE) == 1) then
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			----Time Start
			if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING) then
				--Anti Close Mission;
				if (GetMSPlayerCount(self.MAIN.MISSION_PLD,1) < 1) then
					CloseMission(self.MAIN.MISSION_PLD);
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
		if (GetMSPlayerCount(self.MAIN.MISSION_PLD,1) < 1) then
			CloseMission(self.MAIN.MISSION_PLD);
			return
		end;
		---
	
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		--Notifi warning;
		if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING and nTimerMinutes > 45 and nTimerMinutes < 48) then
			Msg2MSAll(self.MAIN.MISSION_PLD,"<color=cyan>Cßn "..(60 - (nTimerMinutes-45)*20).."gi©y n÷a sÏ b¾t ®Çu.<color>");
		end;
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			local nMinuteStart = GetMissionV(self.TIMER.TIMER_COUNT_MIN);
			if (nMinuteStart == self.TIMER.TIMER_START) then
				self:AddMonsterMS(SubWorld); -- add monster;
				Msg2MSAll(self.MAIN.MISSION_PLD,"<color=cyan>Thêi gian ®i thuyÒn cßn l¹i lµ 30 phót<color>");
			end;
			
			if (nMinuteStart == self.TIMER.TIMER_BOSS1 or nMinuteStart == self.TIMER.TIMER_BOSS2 or nMinuteStart == self.TIMER.TIMER_BOSS3) then
				self:AddBossMS(SubWorld); -- add boss;
				Msg2MSAll(self.MAIN.MISSION_PLD,"<color=cyan>Thñy tÆc ®Çu lÜnh xuÊt hiÖn, h·y mau tiªu diÖt<color>");
			end
			--Notifi 5 final;
			if (nMinuteStart == self.TIMER.TIMER_NOTIFI4) then
				Msg2MSAll(self.MAIN.MISSION_PLD,"<color=cyan>Cßn 5 phót n÷a thuyÒn sÏ cËp bê M¹c B¾c, h·y thËn träng trªn bê cã cao thñ!<color>");
			end;
			--End mission;
			if (nMinuteStart == self.TIMER.TIMER_END) then
				Msg2MSAll(self.MAIN.MISSION_PLD,"<color=cyan>ThuyÒn ®· cËp bÕn bê M¹c B¾c, h·y thËn träng khi lªn bê. §¹o tÆc hoµnh hµnh!<color>");
				CloseMission(self.MAIN.MISSION_PLD); -- finish close;
			end;
		end;
	end;
	
	if (GetMissionV(self.MAIN.STATE) ~= 1 and GetMissionV(self.MAIN.STATE) ~= 2) then
		-- Truong hop sai;
		--Anti Close Mission;
		if (GetMSPlayerCount(self.MAIN.MISSION_PLD,1) < 1) then
			CloseMission(self.MAIN.MISSION_PLD);
			return
		end;
		---
	end
	
end;
 ---------------END TIME  MISSION RUNNING

 -------------CONTROLER
 function PLD:ACTIVE_PhongLangDo() --public function to realtimer.lua
 local nHr = tonumber(date("%H"));
 local nMi = tonumber(date("%M"));
 local OldSubWorld = SubWolrd;

	-- bao danh gio chan - di gio le;
	if( mod(nHr,2) == 0) then
	 	--Time OpenMission
		SubWorld = SubWorldID2Idx(337);
		self:Notifi()--call notifi;
		if (nMi == 45) then
			for i = 337,339 do
				SubWorld = SubWorldID2Idx(i); -- mutil ben thuyen;
				OpenMission(self.MAIN.MISSION_PLD);
			end;
			for i = 1,SYS_MAX_PLAYER do
				PlayerIndex = i;
				SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Phong l¨ng ®é<color> ®ang trong thêi gian b¸o danh h·y mau chãng tham gia <pic="..random(1,140).."> <pic="..random(1,140).."> !");
			end;
		end;
	end;
	SubWorld = OldSubWorld;
	--/.
 end;
 
 function PLD:Notifi()
	----Time notifi
	if (GetMissionV(self.MAIN.STATE) == 1) then
		if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI1 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI2 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI3) then
			AddGlobalCountNews("<color=cyan>BÕn thuyÒn Phong l¨ng ®é<color> s¾p tíi thêi gian khëi hµnh. H·y ®Õn gÆp thuyÒn phu t¹i Phong L¨ng §é ®Ó tham gia.", 3)	
			Msg2SubWorld("<color=cyan>BÕn thuyÒn Phong l¨ng ®é<color> s¾p tíi thêi gian khëi hµnh. H·y ®Õn gÆp thuyÒn phu t¹i Phong L¨ng §é ®Ó tham gia. Thêi gian cßn l¹i "..(15-GetMissionV(self.TIMER.TIMER_COUNT_MIN)).." phót.");	
		end;
	end;
 end;
 ----------
 
function InitMission()
	local nMapID = SubWorldIdx2ID(SubWorld);
	ClearMapNpc(nMapID);
	ClearMapTrap(nMapID); 
	ClearMapObj(nMapID);	
	SetMissionV(PLD.MAIN.STATE, 1);
	PLD:Start_TIMER_LANGDING();
	print("Init mission Phong lang do: ",nMapID);
end;

function EndMission()
	nMapID = SubWorldIdx2ID(SubWorld);
	PLD:Close_TIMER_LANGDING();
	PLD:FinishMS(nMapID);
	ClearMapNpc(nMapID); --reset mission;
	ClearMapTrap(nMapID);  --reset mission;
	ClearMapObj(nMapID); --reset mission;	
	SetMissionV(PLD.MAIN.STATE,0); --reset mission;
	SetMissionV(PLD.TIMER.TIMER_COUNT_SECCON,0); --reset mission;
	SetMissionV(PLD.TIMER.TIMER_COUNT_MIN,0); --reset mission;
	print("Close mission Phong lang do: ",nMapID);
end;

function OnLeave(RoleIndex)
	PlayerIndex = RoleIndex;
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	if (PLD.MAIN.BOATID == 1) then
		SubWorld = SubWorldID2Idx(337);
	elseif (PLD.MAIN.BOATID == 2) then
		SubWorld = SubWorldID2Idx(338);
	else
		SubWorld = SubWorldID2Idx(339);
	end
	PLD:LeaveGame(PlayerIndex,0);
	SubWorld = OldSubWorld;
end


function PLD:Start_TIMER_LANGDING()
	StartMissionTimer(self.MAIN.MISSION_PLD, self.TIMER.TIMER_LANGDING, self.TIMER.TIMER_TICK_SECCON);
end;

function PLD:Close_TIMER_LANGDING()
	StopMissionTimer(self.MAIN.MISSION_PLD, self.TIMER.TIMER_LANGDING);
end;

function PLD:FinishMS(nMapID)
	local OldSubWorld = SubWorld;
		SubWorld = SubWorldID2Idx(nMapID);
		Msg2MSAll(self.MAIN.MISSION_PLD, "§¹i hiÖp ®· ®Õn bê B¾c Phong L¨ng §é.")
		local tbPlayer = {};
		nIndexPlayer = 0;
		for i = 1 , 100 do 
			nIndexPlayer, pIndexPlayer = GetNextPlayer(self.MAIN.MISSION_PLD,nIndexPlayer, 0)
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


function PLD:NpcTalk(nPlayerIndex)
	local szHello = "";
	local szSay = {
		"Ta muèn ®Õn M¹c B¾c/JoinMS#"..nPlayerIndex.."",
		"Ta muèn quay vÒ Ba L¨ng HuyÖn/MovetoBaLangHuyen",
		"Ta chØ ghÐ ngang qua/no"
	}
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nPlayerIndex;
	if (self.MAIN.BOATID == 1) then
		SubWorld = SubWorldID2Idx(337);
	elseif (self.MAIN.BOATID == 2) then
		SubWorld = SubWorldID2Idx(338);
	else
		SubWorld = SubWorldID2Idx(339);
	end

	if (GetMissionV(self.MAIN.STATE) == 1) then
		szHello = "<color=orange>ThuyÒn phu<color>: Ng­¬i muèn ®Õn m¹c B¾c sao? mêi ng­¬i lªn thuyÒn. Nh­ng h·y tù l­îng søc, phÝa tr­íc sãng to giã lín.";
		Say(szHello,2,szSay[1],szSay[3]);
	elseif (GetMissionV(self.MAIN.STATE) > 1) then
		szHello = "<color=orange>ThuyÒn phu<color>: ThuyÒn võa khëi hµnh Ýt phót tr­íc xin ®îi chuyÕn sau.";
		Say(szHello,1,szSay[3]);
	elseif (GetTask(601) > 0) then
		szHello = "<color=orange>ThuyÒn phu<color>: Ng­¬i muèn quay vÒ quª h­¬ng sao?. N¬i nµy qu¶ thËt rÊt nhiÒu s¬n tÆc.";
		Say(szHello,2,szSay[2],szSay[3]);
	else
		szHello = "<color=orange>ThuyÒn phu<color>: H·y ®îi mét ngµy sãng yªn biÓn lÆng, ta míi cã thÓ ®­a ng­¬i ®i ®­îc.";
		Say(szHello,1,szSay[3]);
	end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;

function JoinMS(nNo,nPlayerIndex)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
	local nIDMaps = 0;
		PlayerIndex = nPlayerIndex;
	if (PLD.MAIN.BOATID == 1) then
		nIDMaps = 337;
		SubWorld = SubWorldID2Idx(nIDMaps);
	elseif (PLD.MAIN.BOATID == 2) then
		nIDMaps = 338;
		SubWorld = SubWorldID2Idx(nIDMaps);
	else
		nIDMaps = 339;
		SubWorld = SubWorldID2Idx(nIDMaps);
	end

	local NeedItem = 12; -- lenh bai pld 
	if (PLD:CheckSlotOnDeck() == 1) then
		return 0
	end
	if (GetTaskItemCount(NeedItem) == 0) then
		Say("<color=cyan>§¹i hiÖp ph¶i mang lÖnh bµi <color=red>Phong l¨ng ®é <color> míi cã thÓ lªn thuyÒn.", 0)
		return 0
	end
	
	if (GetTask(PLD.TASK.TPLD_JOIN_BOAT) > 3) then
		Talk(1,"","H«m nay ®¹i hiÖp ®· ®i qu¸ nhiÒu, ngµy mai h·y quay l¹i");
		return
	end
	ZPlayer:ResetTask(); -- kiem tra reset Task;
	DelTaskItem(NeedItem,1);
	LeaveTeam();   
	SetPunish(1);
	SetFightState(0);
	posx, posy = fld_getadata(npcMAP_POS);
	posx = floor(posx/32)
	posy = floor(posy/32)
	AddMSPlayer(PLD.MAIN.MISSION_PLD,1);
	NewWorld(nIDMaps, posx, posy)
	SetRevPos(53,19);
	ForbidTownPortal(1);
	SetDeathScript(PLD.MAIN.PLAYER_DEATH);
	SetLogoutScript(PLD.MAIN.PLAYER_LOGOUT); 
	SetTask(PLD.TASK.TPLD_JOIN_BOAT,GetTask(PLD.TASK.TPLD_JOIN_BOAT)+1); -- COUNT JOIN BOAT
	SetTask(601,1); -- COUNT JOIN BOAT
	SubWorld = OldSubWorld;
	PlayerIndex = OldPlayerIndex;
end;

function PLD:LeaveGame(nPlayerIndex,nSel)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nPlayerIndex;
	if (self.MAIN.BOATID == 1) then
		SubWorld = SubWorldID2Idx(337);
	elseif (self.MAIN.BOATID == 2) then
		SubWorld = SubWorldID2Idx(338);
	else
		SubWorld = SubWorldID2Idx(339);
	end

	DelMSPlayer(self.MAIN.MISSION_PLD,1);
	camp = GetCamp();
	SetCurCamp(camp)
	SetCreateTeam(1)
	SetLogoutScript(""); 
	SetDeathScript("");  
	ForbidChangePK(0);
	SetFightState(0);
	SetPunish(0);
	SetLogoutRV(0)
	SetRevPos(53,19);
	ForbidTownPortal(0);
	if (nSel == 0) then -- finishMS
		NewWorld(336,1158,2964);
		if (GetTask(601) > 0) then
			ForbidTownPortal(1);
			SetFightState(1);
			SetDeathScript(PLD.MAIN.PLAYER_DEATH);
		end;
	elseif (nSel == 1) then --LogoutMS
		NewWorld(53,1616,3186);
		SetTask(601,0); -- COUNT JOIN BOAT
	else -- DieMS
		Msg2Player("§¹i hiÖp tö vong khi ®i thuyÒn.");
		SetTask(601,0); -- COUNT JOIN BOAT
	end;
	
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;

function PLD:CheckSlotOnDeck()
	if (GetMSPlayerCount(self.MAIN.MISSION_PLD, 1) >= 100 ) then
		Say("Ng­¬i ®Õn trÔ råi! ThuyÒn ®· ®Çy råi, h·y chê chuyÕn sau ®i!", 0)
		return 1
	end;
	return 0
end;


function PLD:AddBossMS(nSubWorld) -- add monster;
	local OldSubWorld = SubWorld;
	SubWorld = nSubWorld;
	local TABLE_POS = {
			{1653, 3222},
			{1629, 3197},
			{1604, 3173},
			{1584, 3193},
			{1609, 3221},
			{1632, 3244},
		}

	local indextable = random(1, getn(TABLE_POS))		
	local posx = TABLE_POS[indextable][1] * 32
	local posy = TABLE_POS[indextable][2] * 32
	local npcindex = AddNpc(725, 95, SubWorld, posx, posy, 1, "Thñy tÆc ®Çu lÜnh", 6, 0)
	local nRanSer = random(0,4);
	SetNpcSeries(nNpcIndex,nRanSer);
	local Series = GetNpcSeries(nNpcIndex);
	if (Series == 0) then
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	elseif (Series == 1) then
		SetNpcSkill(nNpcIndex,425,50,1);
		SetNpcSkill(nNpcIndex,425,50,2);
		SetNpcSkill(nNpcIndex,425,50,3);
		SetNpcSkill(nNpcIndex,425,50,4);
	elseif (Series == 2) then
		SetNpcSkill(nNpcIndex,426,50,1);
		SetNpcSkill(nNpcIndex,426,50,2);
		SetNpcSkill(nNpcIndex,426,50,3);
		SetNpcSkill(nNpcIndex,426,50,4);
	elseif (Series == 3) then
		SetNpcSkill(nNpcIndex,427,50,1);
		SetNpcSkill(nNpcIndex,427,50,2);
		SetNpcSkill(nNpcIndex,427,50,3);
		SetNpcSkill(nNpcIndex,427,50,4);
	elseif (Series == 4) then
		SetNpcSkill(nNpcIndex,428,50,1);
		SetNpcSkill(nNpcIndex,428,50,2);
		SetNpcSkill(nNpcIndex,428,50,3);
		SetNpcSkill(nNpcIndex,428,50,4);
	else
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	end;
	
	SetNpcScript(npcindex,self.MAIN.NPC_DEATH);
	Msg2MSAll(self.MAIN.MISSION_PLD, "<color=yellow>Thñy tÆc ®Çu lÜnh <color>®· xuÊt hiÖn.")
end

function PLD:AddMonsterMS(nSubWorld) -- add monster;
	local OldSubWorld = SubWorld;
	SubWorld = nSubWorld;
	local oldPlayerIndex = PlayerIndex;
	for i=1, 30 do
		local posx, posy = fld_getadata(npcthiefpos)
		local npcindex	= AddNpc(724, 95, SubWorld, posx, posy, 0, "Thñy TÆc ", 0, 0);
		SetNpcScript(npcindex,self.MAIN.NPC_DEATH);
	end
	Msg2MSAll(self.MAIN.MISSION_PLD, "<color=yellow>Thñy tÆc <color>®· xuÊt hiÖn xin h·y cÈn thËn.")
	idx = 0;
	for i = 1 , 200 do 
 		idx, pidx = GetNextPlayer(self.MAIN.MISSION_PLD,idx, 0);

 		if (pidx > 0) then
   			PlayerIndex = pidx;
			SetFightState(1) ;
 		end 
		
		if (idx == 0) then 
 			break
 		end 
 	end 
	PlayerIndex = oldPlayerIndex;
	SubWorld = OldSubWorld;
end

function MovetoBaLangHuyen()
	NewWorld(53,1621,3186);
	SetFightState(0);
	ForbidTownPortal(0);
	SetDeathScript("");  
	SetTask(601,0); -- COUNT JOIN BOAT
end;


function fld_getadata(file)
	local totalcount = getn(npcMAP_POS);
	id = random(1, totalcount);
	x = npcMAP_POS[id][1];
	y = npcMAP_POS[id][2];
	return x,y
end

npcMAP_POS = {
	{50400, 102240},
	{50560, 102048},
	{50752, 101856},
	{51008, 101792},
	{51072, 101536},
	{51232, 101344},
	{51424, 101184},
	{50656, 102496},
	{50848, 102272},
	{51040, 102080},
	{51264, 101728},
	{51424, 101568},
	{51616, 101408},
	{50848, 102688},
	{51040, 102496},
	{51264, 102336},
	{51424, 101984},
	{51584, 101760},
	{51744, 101600},
	{51104, 102944},
	{51264, 102752},
	{51488, 102528},
	{51616, 102144},
	{51776, 101952},
	{51936, 101760},
	{51296, 103168},
	{51488, 102976},
	{51680, 102720},
	{51776, 102272},
	{51936, 102112},
	{52096, 101920},
	{51488, 103360},
	{51680, 103136},
	{51872, 102912},
	{51936, 102432},
	{52128, 102272},
	{52288, 102112},
	{51680, 103552},
	{51872, 103360},
	{52064, 103136},
	{52160, 102688},
	{52352, 102496},
	{52480, 102304},
	{51872, 103712},
	{52032, 103520},
	{52256, 103296},
	{52384, 102848},
	{52544, 102656},
	{52672, 102528},
	{52032, 103904},
	{52224, 103712},
	{52416, 103520},
	{52544, 103040},
	{52704, 102848},
	{52864, 102688},
	{52224, 104096},
	{52416, 103936},
	{52544, 103776},
	{52736, 103200},
	{52896, 103040},
	{53056, 102848},
	{53024, 103168},
	{53152, 103072},
}

function no()

end;