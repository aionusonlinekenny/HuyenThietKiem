--Author: Kinnox;
--Date: 21/04/2022;
--App: Head vuot ai;
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\player\\head.lua");
Include("\\script\\lib\\libevent.lua");
Include("\\script\\mission\\vuotai\\killerboss.lua");
Include("\\script\\system_config.lua");
VA = {};
FRAME2TIME = 18; 
VA.MAIN ={
	MISSION_VA 			= 1, 	-- ID mission; 
	STATE				= 1,	-- STATE; [MissionV]
	PLAYER_DEATH 		= "\\script\\mission\\vuotai\\player_death.lua",	--script player death;
	PLAYER_LOGOUT		= "\\script\\mission\\vuotai\\player_logout.lua",	--script player logout;
	NPC_DEATH 			= "\\script\\mission\\vuotai\\normal_death.lua",	--script npc death ( normal, gold npc);
	BOATID				= 0,    -- ID ai;
	LIMITEDDAY			= 2,
	TEAMSIZE			= 2,
	BOARD_TIMES			= 4,	-- Count BOARD_TIMES;[MissionV]
	BOARD_NPC			= 5,	-- Count BOARD_NPC;[MissionV]
	LOSE_WIN			= 6,	-- Check Winner;[MissionV]
	LIMITEDLEVEL		= 80,
	NEEDCASH			= 5000, --Check Cash;
	EXPFINISH			= 2000000,
};

VA.TIMER = {
	TIMER_LANGDING 		= 1, -- ID timer trong setting\timertask.lua
	TIMER_TICK_SECCON	= 20*FRAME2TIME, -- count time mission 20 seccons;
	TIMER_COUNT_SECCON	= 2, -- count time mission 20 seccons; [MissionV]
	TIMER_COUNT_MIN		= 3, -- count time mission TIMER_COUNT_SECCON*3; [MissionV]
	TIMER_NOTIFI1		= 5,  --timer call notifi;
	TIMER_NOTIFI2		= 10, --timer call notifi;
	TIMER_NOTIFI3		= 14, --timer call notifi;
	TIMER_WAITING		= 15,
	TIMER_START			= 16,
	TIMER_NOTIFI4		= 40,
	TIMER_END			= 45,
};

VA.TASK = {
	TVA_JOIN_BOAT = 602; -- Task count join;
};

function VA:TIMER_LANGDING(nSubWorld)
local OldSubWorld = SubWorld;
SubWorld = nSubWorld;
	if (GetMissionV(self.MAIN.STATE) == 1) then
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			----Time Start
			if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING) then
				--Anti Close Mission;
				if (GetMSPlayerCount(self.MAIN.MISSION_VA,1) < 1) then
					CloseMission(self.MAIN.MISSION_VA);
					return
				end;
				---
				SetMissionV(self.MAIN.BOARD_TIMES,1);
				SetMissionV(self.MAIN.STATE,2) -- run start;
				return
			end;
		end;
	end;
	
	----Mission Langning
	if (GetMissionV(self.MAIN.STATE) == 2) then
		--Anti Close Mission;
		if (GetMSPlayerCount(self.MAIN.MISSION_VA,1) < 1) then
			CloseMission(self.MAIN.MISSION_VA);
			return
		end;
		---
	
		SetMissionV(self.TIMER.TIMER_COUNT_SECCON,GetMissionV(self.TIMER.TIMER_COUNT_SECCON)+1);
		local nTimerMinutes = GetMissionV(self.TIMER.TIMER_COUNT_SECCON);		
		--Notifi warning;
		if (GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_WAITING and nTimerMinutes > 45 and nTimerMinutes < 48) then
			Msg2MSAll(self.MAIN.MISSION_VA,"<color=cyan>Cﬂn "..(60 - (nTimerMinutes-45)*20).."gi©y n˜a sœ bæt Æ«u.<color>");
		end;
		if ( mod(nTimerMinutes,3) == 0 ) then
			SetMissionV(self.TIMER.TIMER_COUNT_MIN,GetMissionV(self.TIMER.TIMER_COUNT_MIN)+1);
			local nMinuteStart = GetMissionV(self.TIMER.TIMER_COUNT_MIN);
			if (nMinuteStart == self.TIMER.TIMER_START) then
				self:SetFightStatePlayer(SubWorld); -- change fight sate player;
				self:AddMonsterMS(SubWorld); -- add monster;
				Msg2MSAll(self.MAIN.MISSION_VA,"<color=cyan>ThÍi gian Æ” tÊ ÆÈi hoµn thµnh chinh phπt cﬂn 30 phÛt<color>");
			end;
			
			--Notifi 5 final;
			if (nMinuteStart == self.TIMER.TIMER_NOTIFI4) then
				Msg2MSAll(self.MAIN.MISSION_VA,"<color=cyan>ThÍi gian Æ” tÊ ÆÈi hoµn thµnh chinh phπt cﬂn 5 phÛt<color>");
			end;
			--End mission;
			if (nMinuteStart == self.TIMER.TIMER_END) then
				CloseMission(self.MAIN.MISSION_VA); -- finish close;
			end;
		end;
	end;
	
	if (GetMissionV(self.MAIN.STATE) ~= 1 and GetMissionV(self.MAIN.STATE) ~= 2) then
		-- Truong hop sai;
		--Anti Close Mission;
		if (GetMSPlayerCount(self.MAIN.MISSION_VA,1) < 1) then
			CloseMission(self.MAIN.MISSION_VA);
			return
		end;
		---
	end
SubWorld = OldSubWorld;
end;


 -------------CONTROLER
 function VA:ACTIVE_VuotAi() --public function to realtimer.lua
 local nHr = tonumber(date("%H"));
 local nMi = tonumber(date("%M"));
 	local OldSubWorld = SubWolrd;
	
	if (mod(nHr,2) ~= 0) then
		--Time OpenMission
		SubWorld = SubWorldID2Idx(464);
		self:Notifi()--call notifi;
		--
		-- bao danh gio le - di gio chan;
		
		if (nMi == 45) then
			for i = 464,470 do
				SubWorld = SubWorldID2Idx(i); --mutil vuot ai;
				OpenMission(self.MAIN.MISSION_VA);
			end;
			for i = 1,SYS_MAX_PLAYER do
				PlayerIndex = i;
				SendNotification("","(Lo) <color=red>Th´ng b∏o: <color> <color=green>V≠Ót ∂i<color> Æang trong thÍi gian b∏o danh h∑y mau ch„ng tham gia <pic="..random(1,140).."> <pic="..random(1,140).."> !");
			end;
		end;
		SubWorld = OldSubWorld;
		--/.
	end;
 end;
 
function VA:Notifi()
 ----Time notifi
	if (GetMissionV(self.MAIN.STATE) == 1) then
		if ( GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI1 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI2 or GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_NOTIFI3) then
			AddGlobalCountNews("<color=cyan>V≠¨t ∂i Nh›p nh› tr«n<color>sæp Æ’n thÍi gian khai mÎ, c∏c ÆÂng Æπo h∑y nhanh ch„ng b∏o danh", 3)	
			Msg2SubWorld("<color=cyan>V≠¨t ∂i Nh›p nh› tr«n<color> sæp Æ’n thÍi gian khai mÎ. H∑y Æ’n g∆p Nh›p nh› tr«n tπi th t Æπi thµnh thﬁ Æ” b∏o danh. ThÍi gian cﬂn lπi "..(15-GetMissionV(self.TIMER.TIMER_COUNT_MIN)).." phÛt.");	
		end;
	end;
 end;
 ----------
 
function InitMission()
	local nMapID = SubWorldIdx2ID(SubWorld);
	ClearMapNpc(nMapID);
	ClearMapTrap(nMapID); 
	ClearMapObj(nMapID);	
	SetMissionV(VA.MAIN.STATE, 1);
	VA:Start_TIMER_LANGDING();
	print("Init mission Vuot Ai: ",nMapID);
end;

function EndMission()
	local nMapID = SubWorldIdx2ID(SubWorld);
	VA:Close_TIMER_LANGDING();
	VA:FinishMS(nMapID);
	ClearMapNpc(nMapID); --reset mission;
	ClearMapTrap(nMapID);  --reset mission;
	ClearMapObj(nMapID); --reset mission;	
	SetMissionV(VA.MAIN.STATE,0); --reset mission;
	SetMissionV(VA.TIMER.TIMER_COUNT_SECCON,0); --reset mission;
	SetMissionV(VA.TIMER.TIMER_COUNT_MIN,0); --reset mission;
	print("Close mission Vuot Ai: ",nMapID);
end;

function OnLeave(RoleIndex)
	-- khong su dung vi FinishMS da chay roi;
end


function VA:Start_TIMER_LANGDING()
	StartMissionTimer(self.MAIN.MISSION_VA, self.TIMER.TIMER_LANGDING, self.TIMER.TIMER_TICK_SECCON);
end;

function VA:Close_TIMER_LANGDING()
	StopMissionTimer(self.MAIN.MISSION_VA, self.TIMER.TIMER_LANGDING);
end;

 function VA:FinishMS(nMapID)
	local OldSubWorld = SubWorld;
		SubWorld = SubWorldID2Idx(nMapID);
	local OldPlayerIndex = PlayerIndex;
		local tbPlayer = {};
		nIndexPlayer = 0;
		for i = 1 , 100 do 
			nIndexPlayer, pIndexPlayer = GetNextPlayer(self.MAIN.MISSION_VA,nIndexPlayer, 0)
			if (pIndexPlayer > 0) then
				tbPlayer[i] = pIndexPlayer;
			end;
			
			if (nIndexPlayer == 0) then 
				break
			end;
		end;
		
		for i= 1, getn(tbPlayer) do 
			PlayerIndex = tbPlayer[i];
			DelMSPlayer(self.MAIN.MISSION_VA,1);
			camp = GetCamp();
			SetCurCamp(camp)
			SetCreateTeam(1)
			SetLogoutScript(""); 
			SetDeathScript("");  
			ForbidChangePK(0);
			SetFightState(0);
			SetPunish(0);
			SetLogoutRV(0)
			SetRevPos(78,29);
			ForbidTownPortal(0);
			NewWorld(78,1507,3207);
			LeaveTeam();
			if (GetMissionV(self.MAIN.LOSE_WIN) > 0) then -- team win;
				Msg2Player("Ch¯c mıng tÊ ÆÈi Æπi hi÷p hoµn thµnh nhi÷m vÙ khi™u chi’n giang hÂ.");
				EventVuotAi();
				AddOwnExp(self.MAIN.EXPFINISH);
				Msg2Player("ßπi hi÷p nhÀn Æ≠Óc <color=yellow>2 tri÷u <color>kinh nghi÷m vµ hÈp l‘ bao v≠Ót ∂i !")
				--reward here;
			else--team lose
				Msg2Player("TÊ ÆÈi Æπi hi÷p khi™u chi’n th t bπi.");
			end;
		end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld
end;

function VA:OUT_DEATH(nPlayerIndex,nSel)
	local OldPlayerIndex = PlayerIndex;
	PlayerIndex = nPlayerIndex;
	local OldSubWorld = SubWorld;
	if (nSel == 1) then --LogoutMS
		NewWorld(78,1507,3207);
	else -- DieMS
		Msg2Player("ßπi hi÷p tˆ vong khi Æang v≠Ót ∂i.");
	end;
	DelMSPlayer(self.MAIN.MISSION_VA,1);
	camp = GetCamp();
	SetCurCamp(camp)
	SetCreateTeam(1)
	SetLogoutScript(""); 
	SetDeathScript("");  
	ForbidChangePK(0);
	SetFightState(0);
	SetPunish(0);
	SetLogoutRV(0)
	SetRevPos(78,29);
	ForbidTownPortal(0);
	LeaveTeam();
	PlayerIndex = PlayerIndex;
	SubWorld = OldSubWorld;
end;

function VA:NpcTalk(nPlayerIndex)
	if (SYS_SWITCH_KILLER == 0 or SYS_SWITCH_LEAGUE == 0) then
		Talk(1,"","T›nh n®ng nµy hi÷n ch≠a mÎ!")
	return
	end;
	local szHello = "";
	local nSubPlayer = 463;
	local OldPlayerIndex = PlayerIndex;
	PlayerIndex = nPlayerIndex;
	local OldSubWorld = SubWorld;
	--mutil vuot ai;
	local nW,_,_ = GetWorldPos()
	if (nW == 78) then --tuong duong
		nSubPlayer = 463;
	elseif (nW == 1) then --phuong tuong
		nSubPlayer = 464;
	elseif (nW == 80) then --duong chau
		nSubPlayer = 465;
	elseif (nW == 11) then --thanh do
		nSubPlayer = 466;	
	elseif (nW == 37) then --bien kinh
		nSubPlayer = 467;
	elseif (nW == 162) then --dai ly
		nSubPlayer = 468;	
	end;
	SubWorld = SubWorldID2Idx(VA.MAIN.BOATID+nSubPlayer);			
	szHello = "<color=orange>Nhi’p Nh› Tr«n<color>: Ph∂i ta lµ s¯ gi∂ tri“u Æ◊nh c˜ Æ’n Æ” tuy”n c∏c Æ‰i nh„m cao thÒ rÃn luy÷n th” ch t trinh phπt nh˜ng thˆ th∏ch tri“u Æ◊nh Æ≠a ra!."
	local tbSay_VA = {
		"Ta Æ’n Æ” t◊m hi”u thˆ th∏c v≠Ót ∂i/Help",
		"Ta Æ’n Æ” t◊m hi”u thˆ th∏ch s∏t thÒ/Help",
		"Ta Æ’n Æ” gh–p s∏t thÒ gi∂n/STG",
		"Ta Æ’n Æ” nhÀn nhi÷m vÙ s∏t thÒ/KillerMan",
		"Ta Æ’n Æ” chinh phπt v≠Ót ∂i/TalkQuest#"..PlayerIndex.."",
		"Ta Æ’n mua S∏t thÒ gi∂n l‘ hÈp/ShopNTT",
		"Ta chÿ Æ’n th®m ng≠Íi/no",
	};
	local tbSay_KILLER = {
		"Ta ß’n hÒy nhi÷m vÙ phÙng m÷nh s∏t thÒ/RejectQuest",
		"Ta ß’n nhÍ ng≠Íi Æ≠a ta ra Æﬁa Æi”m s∏t thÒ/GotoBoss",
		"Ta Æ’n Æ” gh–p s∏t thÒ gi∂n/STG",
		"Ta Æ’n Æ” t◊m hi”u thˆ th∏c v≠Ót ∂i/Help",	
		"Ta Æ’n Æ” t◊m hi”u thˆ th∏ch s∏t thÒ/Help",
		"Ta Æ’n Æ” chinh phπt v≠Ót ∂i/TalkQuest#"..PlayerIndex.."",
		"Ta Æ’n mua S∏t thÒ gi∂n l‘ hÈp/ShopNTT",
		"Ta chÿ Æ’n th®m ng≠Íi/no",
	};
	if (GetTask(KLB.TASK.TASK_REGIS_KILLER) > 0) then
		Say(szHello,getn(tbSay_KILLER),tbSay_KILLER);
	else
		Say(szHello,getn(tbSay_VA),tbSay_VA);
	end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;



function TalkQuest(nNo,nPlayerIndex)
	local szHello = "";
	local szSay = {
		"Ph∂i! Ta muËn d…n dæt ÆÈi cÒa ta tham gia./JoinMS#"..PlayerIndex.."",
		"Ta chÿ Æ’n th®m ng≠Íi/no"
	}
	local OldSubWorld = SubWorld;
	local nSubPlayer = 463;
	local OldPlayerIndex = PlayerIndex;
	PlayerIndex = nPlayerIndex;
	local nW,_,_ = GetWorldPos()
	if (nW == 78) then --tuong duong
		nSubPlayer = 463;
	elseif (nW == 1) then --phuong tuong
		nSubPlayer = 464;
	elseif (nW == 80) then --duong chau
		nSubPlayer = 465;
	elseif (nW == 11) then --thanh do
		nSubPlayer = 466;	
	elseif (nW == 37) then --bien kinh
		nSubPlayer = 467;
	elseif (nW == 162) then --dai ly
		nSubPlayer = 468;	
	end;

	SubWorld = SubWorldID2Idx(VA.MAIN.BOATID+nSubPlayer);
	if (SubWorld <= 0) then
		szHello = "<color=orange>Nhi’p Nh› Tr«n<color>: Hi÷n tπi ÆÒ tÊ ÆÈi tham gia Th∏ch Th¯c ThÍi Gian, H∑y Æ’n sÌm h¨n vµo l«n sau.";
		Say(szHello,0);
	elseif ((GetMissionV(VA.MAIN.STATE) == 1 and  (VA:CheckSlotVA(VA.MAIN.BOATID+nSubPlayer,PlayerIndex) > 0 ))) then
		szHello = "<color=yellow>Nhi’p Nh› Tr«n<color>: Giai Æoπn b∏o danh, v…n cﬂn <color=yellow> "..(VA:CheckSlotVA(VA.MAIN.BOATID+nSubPlayer,PlayerIndex)).." <color>b∂n ÆÂ nhi÷m vÙ mi‘n ph›, thÍi gian khi™u chi’n c«n ph∂i giao <color=red>5000 <color> ng©n l≠Óng, ng≠¨i muËn tham gia kh´ng?";
		Say(szHello,getn(szSay),szSay);
	elseif (GetMissionV(VA.MAIN.STATE) == 0) then
		szHello = "<color=yellow>Nhi’p Nh› Tr«n<color>: Hoπt ÆÈng v…n ch≠a di‘n ra, QuËc th∏i D©n An, Thi™n hπ th∏i b◊nh. N’u c„ loπn Æ∂ng xu t hi÷n ta sœ c´ng b∏o toµn thi™n hπ.";
		Say(szHello,0);
	elseif ((GetMissionV(VA.MAIN.STATE) > 1) and  (VA:CheckSlotVA(VA.MAIN.BOATID+nSubPlayer,PlayerIndex)) == 0) then
		szHello = "<color=yellow>Nhi’p Nh› Tr«n<color>: Ta thay m∆t d©n lµng c∂m tπ lﬂng tËt cÒa c∏c ng≠¨i, nh≠ng Æ∑ c„ mÈt nh„m Æπi hi÷p Æi tr≠Ìc rÂi .C∏c ng≠¨i Æi thµnh thﬁ kh∏c xem sao!";
		Say(szHello,0);
	end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;

function VA:CheckSlotVA(nSubWorld,nPlayerIndex)
	local OldSubWorld = SubWorld;
			SubWorld = SubWorldID2Idx(nSubWorld);
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nPlayerIndex;
	local nCountSlot = GetMSPlayerCount(self.MAIN.MISSION_VA,1);
	if (nCountSlot > 0) then
		return 0;
	else
		return 1;
	end;
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
	return 1;
end;


 function JoinMS(nNo,nPlayerIndex)
	--mutil vuot ai;
	local OldSubWorld = SubWorld;
	local nSubPlayer = 463;
	local OldPlayerIndex = PlayerIndex;
	local nIDMaps = 0;
	local nTeamSize = GetTeamSize();
		PlayerIndex = nPlayerIndex;
		local nW,_,_ = GetWorldPos()
		if (nW == 78) then --tuong duong
			nSubPlayer = 463;
		elseif (nW == 1) then --phuong tuong
			nSubPlayer = 464;
		elseif (nW == 80) then --duong chau
			nSubPlayer = 465;
		elseif (nW == 11) then --thanh do
			nSubPlayer = 466;	
		elseif (nW == 37) then --bien kinh
			nSubPlayer = 467;
		elseif (nW == 162) then --dai ly
			nSubPlayer = 468;	
		end;
		SubWorld = SubWorldID2Idx(VA.MAIN.BOATID+nSubPlayer);
		nIDMaps = VA.MAIN.BOATID+nSubPlayer;
	ZPlayer:ResetTask(); -- kiem tra reset Task;
	if (IsCaptain() ~= 1) then
		Talk(1, "", "<color=orange>Nhi’p Nh› Tr«n<color>:Xin lÁi, chÿ c„ ÆÈi tr≠Îng mÌi c„ quy“n ki”m tra Æi“u ki÷n tham gia cÒa tÊ ÆÈi!");
		return
	end;
	
	if (nTeamSize < VA.MAIN.TEAMSIZE) then
		Say("<color=orange>Nhi’p Nh› Tr«n<color>: TÊ ÆÈi ph∂i c„ tËi thi”u <color=yellow>"..VA.MAIN.TEAMSIZE.." thµnh vi™n",0);
		return 
	end;
	 
	for i = 7,11 do
		if (GetTaskItemCount(i) == 0) then
			Say("<color=orange>Nhi’p Nh› Tr«n<color>: ßπi hi÷p lµ ÆÈi tr≠Îng ph∂i mang theo <color=red>5 s∏t thÒ gi∂n<color>, ÆÈi vi™n kh´ng c«n. ßÈi vi™n mang theo <color=red>5000 l≠Óng lÈ ph› <color>. mÌi c„ th” Æ∂m b∂o lÈ tr◊nh!", 0)
			return 0
		end;
	end;
	
	if (GetCash() < VA.MAIN.NEEDCASH) then
			Say("<color=orange>Nhi’p Nh› Tr«n<color>: Ng≠¨i kh´ng c„ ÆÒ <color=red>5000 l≠Óng<color> cÌ sao lπi d∏m Æ∂m nhi÷m ch¯c vﬁ ÆÈi tr≠Îng?.",0);	
		return
	end;
	
	if (GetTask(VA.TASK.TVA_JOIN_BOAT) > VA.MAIN.LIMITEDDAY) then
		Talk(1,"","<color=orange>Nhi’p Nh› Tr«n<color>: H´m nay Æπi hi÷p Æ∑ Æi qu∏ nhi“u, ngµy mai h∑y quay lπi "..GetTask(VA.TASK.TVA_JOIN_BOAT).."/"..VA.MAIN.LIMITEDDAY..".");
		return
	end;
	
	local nNeed, strDailog = VA:CheckCondition();
	
	if nNeed == 1 then 
		Say(strDailog, 0);
		return
	end;
	
	
	local tab_Player = {};
	for i=1,GetTeamSize() do
		tab_Player[i] = GetTeamMember(i);
	end
	
	-------------------------------
	for i=1,getn(tab_Player) do -- send excuter member in party
		PlayerIndex = tab_Player[i];
		LeaveTeam();   
		camp = GetCamp();
		SetCurCamp(camp);
		Pay(VA.MAIN.NEEDCASH);
		SetPunish(1);
		SetFightState(0);
		AddMSPlayer(VA.MAIN.MISSION_VA,1);
		NewWorld(nIDMaps, 1559, 3226);
		SetRevPos(53,19);
		ForbidTownPortal(1);
		SetLogoutRV(1);
		SetPKFlag(0);
		SetDeathScript(VA.MAIN.PLAYER_DEATH);
		SetLogoutScript(VA.MAIN.PLAYER_LOGOUT); 
		SetTask(VA.TASK.TVA_JOIN_BOAT,GetTask(VA.TASK.TVA_JOIN_BOAT)+1); -- COUNT JOIN BOAT
	end
	PlayerIndex = OldPlayerIndex;
	----------------DEL NEED ITEM
	DelTaskItem(7,1);
	DelTaskItem(8,1);
	DelTaskItem(9,1);
	DelTaskItem(10,1);
	DelTaskItem(11,1);
	SubWorld = OldSubWorld;
end;


function VA:SetFightStatePlayer(nSubWorld)
	local OldSubWorld = SubWorld;
	SubWorld = nSubWorld;
	local OldPlayerIndex = PlayerIndex;
	-----------change fight sate;
	if 	(GetMissionV(self.TIMER.TIMER_COUNT_MIN) == self.TIMER.TIMER_START) then
		idx = 0;
		for i = 1 , 10 do 
			idx, pidx = GetNextPlayer(self.MAIN.MISSION_VA,idx, 0);
			if (pidx > 0) then
				PlayerIndex = pidx;
				SetFightState(1);
			end 
			
			if (idx == 0) then 
				break;
			end;
		end;
	end;
	-----------
	PlayerIndex = OldPlayerIndex;
	SubWorld =OldSubWorld;
end;

map_lnpc = {
	{975, "S≠¨ng ßao ", 75,0,1},
	{976, "Phi Sa", 75,1,1},
	{977, "S≠¨ng Li™m", 75,2,1},
	{975, "S≠¨ng ßao ", 75,3,2}, ---ai bosss
	{978, "Thıa Phong", 75,4,1},
	{979, "ThÒy HÂn", 75,-1,2}, -- ai boss
	{980, "Th«n T˝", 75,0,1},
	{976, "Phi Sa", 75,1,2}, -- ai bosss
	{981, "Tranh H∏n", 75,2,1},
	{982, "Ph∏ Lang", 75,3,1},
	{977, "S≠¨ng Li™m", 75,4,2}, -- ai boss
	{983, "∂nh C´n", 75,-1,1},
	{978, "Thıa Phong", 75,-1,1},
	{987, "Giang Tr«m Y’n", 75,-1,1},
	{984, "ßao Tˆ", 75,-1,1},
	{979, "ThÒy HÂn", 75,-1,2}, -- ai boss
	{988, "C´ D˘ T»u", 75,-1,1},
	{980, "Th«n T˝", 75,2,2}, -- ai boss 
	{989, "Ch≠Îng B∏t Ph≠¨ng", 75,-1,1},
	{985, "Lang bÊng", 75,-1,1},
	{981, "Tranh H∏n", 75,-1,2},-- ai boss
	{990, "Ti‘n Than Th≠", 75,-1,1},
	{991, "NhÀm T´ng Hoµnh", 75,-1,1},
	{992, "ßÂ Tµn Sinh", 75,-1,1},
	{984, "ßao Tˆ", 75,3,2},--ai boss
	{986, "Hæc c©n", 75,-1,1}, --4 series , 5 npc pram
	{985, "Lang bÊng", 75,-1,4},--ai boss nhiep tri tran 
	{993, "Thi’u L©m Ph∂n D©m", 75,-1,5},--ai boss nhiep tri tran 
};



function VA:AddMonsterMS(nSubWorld) -- add monster; 
	local OldSubWorld = SubWorld;
	SubWorld = nSubWorld;
	local OldPlayerIndex = PlayerIndex;
	local nCrt = GetMissionV(self.MAIN.BOARD_TIMES);
	local nIdMaps = SubWorld;
	local nIdNpcs = map_lnpc[nCrt][1];
	local nNameNpcs = map_lnpc[nCrt][2];
	local nCountNpc = 0;
	local nTotalNpc = random(15,20); -- Number monster;
	local i = 0;
	for i=1,nTotalNpc do
		local nXpos, nYpos = self:GetNpcPosMap(i);
		local nIndex = AddNpc(nIdNpcs,90,nIdMaps,nXpos*32,nYpos*32,1,nNameNpcs,0,0);
		SetNpcScript(nIndex,self.MAIN.NPC_DEATH);
		SetNpcParam(nIndex,1,map_lnpc[nCrt][5])	;
		
		if map_lnpc[nCrt][4] > 0 then
			nSeries = map_lnpc[nCrt][4];
		else
			nSeries = random(0,4);
		end
		
		SetNpcSeries(nIndex,nSeries);
		SetNpcReduceDamge(nIndex, 30);
		
		if nIndex > 0 and nIndex ~= nil then
			nCountNpc = nCountNpc + 1;
		end;
	end;
	SetMissionV(self.MAIN.BOARD_NPC, nCountNpc);
	Msg2MSAll(VA.MAIN.MISSION_VA,format("Hi÷n tπi Æ∑ lµ ∂i th¯ <color=yellow>%d<color> xu t hi÷n <color=yellow>%d<color> Æ∏m qu∏i h∑y c»n thÀn.",nCrt,nCountNpc));
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end;


function VA:AddBossAntiDamgeMS(nSubWorld)	
	local nCrt = GetMissionV(self.MAIN.BOARD_TIMES);
	local nIdMaps = nSubWorld;
	local nIdNpcs = map_lnpc[nCrt][1];
	local nNameNpcs = map_lnpc[nCrt][2];
	local nCountNpc = 0;
	local nTotalNpc = random(15,20);
	local i = 0;
	for i=1,nTotalNpc do
		local nXpos, nYpos = self:GetNpcPosMap(i);
		local nIndex = AddNpc(nIdNpcs,95,nIdMaps,nXpos*32,nYpos*32,1,nNameNpcs,6,0);
		IsBoss(nIndex,6);
		SetNpcScript(nIndex,self.MAIN.NPC_DEATH);
		SetNpcParam(nIndex,1,map_lnpc[nCrt][5]);
		SetNpcSeries(nIndex,random(0,4));
		SetNpcReduceDamge(nIndex, 30);
		
		if nIndex > 0 and nIndex ~= nil then
			nCountNpc = nCountNpc + 1;
		end;
	end;
	SetMissionV(self.MAIN.BOARD_NPC, nCountNpc);
	Msg2MSAll(VA.MAIN.MISSION_VA,format("Hi÷n tπi Æ∑ lµ ∂i th¯ <color=yellow>%d<color> xu t hi÷n <color=yellow>%d<color> Æ∏m qu∏i h∑y c»n thÀn.",nCrt,nCountNpc));
end;


function VA:AddBossNhipNhiTranMS(nSubWorld)	
	local nNhiepThiTran = AddNpc(1038,95,nSubWorld,1568*32, 3227*32,1,"Nhi’p Th› Tr«n",6,0)
	SetNpcScript(nNhiepThiTran,self.MAIN.NPC_DEATH);
	SetNpcParam(nNhiepThiTran,1,6);
	SetNpcSeries(nNhiepThiTran,random(0,4));
	SetNpcReduceDamge(nNhiepThiTran, 30);
	Msg2MSAll(VA.MAIN.MISSION_VA,"<color=yellow>Nhi’p Th› Tr«n<color> Æ∑ vµo ∂i, hi÷n Æang l»n trËn. H∑y c»n thÀn phﬂng bﬁ ∏m s∏t.");
end;

TAB_ID_BOSS = {
	{994, 1001},
	{1002, 1005}
};
function VA:AddBossMS(nSubWorld, nBoardTime)
	local OldPlayerIndex = PlayerIndex;
	local tab_Name = {};
	local index = 0;
	local player = 0;
	while(1) do
		index, player = GetNextPlayer(self.MAIN.MISSION_VA, index, 0);
		PlayerIndex = player;
		if (player > 0) then
			tab_Name[getn(tab_Name)+1] = {GetName(),GetSeries()};
		end;
		if (index == 0) then
			break;
		end;	
	end;
	PlayerIndex = OldPlayerIndex;
	local nNamerd = random(1,getn(tab_Name));
	local nRandom = 1;
	local nIdMaps = nSubWorld;
	local nIdNpcs = random(TAB_ID_BOSS[nRandom][1],TAB_ID_BOSS[nRandom][2]);
	local nNpcsBoss = AddNpc(nIdNpcs,95,nIdMaps,1568*32, 3227*32,1,tab_Name[nNamerd][1],6,0);
	SetNpcScript(nNpcsBoss,self.MAIN.NPC_DEATH);
	if (nBoardTime == getn(map_lnpc)) then
		SetNpcParam(nNpcsBoss,1,4);
	else
		SetNpcParam(nNpcsBoss,1,3);
	end;
	SetNpcSeries(nNpcsBoss,tab_Name[nNamerd][2]) ;
	Msg2MSAll(self.MAIN.MISSION_VA,format("C„ mÈt s∏t thÒ Æ∑ gi∂ dπng <color=yellow>%s<color>, Hi÷n tπi Æang »n m◊nh trong b∂n ÆÂ V≠Ót ∂i, H∑y c»n thÀn.",tab_Name[nNamerd][1]));
	SetNpcReduceDamge(nNpcsBoss, 30);
end;


 
 function VA:CheckCondition()
	local nOldPlayerIndex = PlayerIndex; -- doi truong 
	local tbDialog = {};
	local nTeamSize = GetTeamSize();
	
	if (self:GetLevelCondition() == 0) then
		tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " kh´ng ÆÒ level tham gia."));
		return
	end;
	
	if (self:GetLimited() > self.MAIN.LIMITEDDAY) then
		tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " Æ∑ tham gia ÆÒ sË l«n nhi÷m vÙ trong ngµy."));
		return
	end;
	
	if (self:Cash(self.MAIN.NEEDCASH) == 0 )then
		tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " kh´ng mang theo ÆÒ 5000 l≠Óng."));
		return
	end
	
	nFlag = 0 ;
	for i = 1, nTeamSize do
		PlayerIndex = GetTeamMember(i);
		if (PlayerIndex ~= nOldPlayerIndex) then
			if (self:GetLevelCondition() == 0) then
				tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " kh´ng ÆÒ level tham gia."));
				break;
			end;
			
			if (self:GetLimited() > self.MAIN.LIMITEDDAY) then
				tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " Æ∑ tham gia ÆÒ sË l«n nhi÷m vÙ trong ngµy."));
				break;
			end;
			
			if (self:Cash(self.MAIN.NEEDCASH) == 0 )then
				tinsert(tbDialog, getn(tbDialog) + 1, format("<color=red>%s<color>: %s", GetName(), " kh´ng mang theo ÆÒ 5000 l≠Óng."));
				break;
			end
		else
		end;
	end;
	
	PlayerIndex = nOldPlayerIndex;
	
	if (getn(tbDialog) > 0) then
		local strTittle = "<color=orange>Nhi’p Nh› Tr«n<color>: TÊ ÆÈi hi÷n tπi kh´ng th” tham gia khi™u chi’n:";
		local nLoop = getn(tbDialog);
		if nLoop > 8 then
			nLoop = 8;
		end;
		for i = 1, nLoop do
			strTittle = strTittle.."\n"..tbDialog[i];
		end;
		return 1, strTittle;
	else
		return 0, "";
	end;
end;


function VA:GetLevelCondition()
	if (GetLevel() >= self.MAIN.LIMITEDLEVEL) then
		return 1;
	else
		return 0;
	end;
end;

function VA:GetLimited()
	return GetTask(VA.TASK.TVA_JOIN_BOAT);
end;

function VA:Cash(nNeedCash)
	if (GetCash() >= nNeedCash) then
		return 1;
	else
		return 0;
	end;
end;

function VA:GetNpcPosMap(nID)

	if (nID <= 0) then
		nID = 1
	end
	
	if (nID >= getn(TAB_POSNPC)) then
		nID = getn(TAB_POSNPC)
	end
	
	return TAB_POSNPC[nID][1], TAB_POSNPC[nID][2]
end

TAB_POSNPC = {
	{1542, 3222},
	{1548, 3216},
	{1554, 3210},
	{1559, 3205},
	{1565, 3198},
	{1548, 3229},
	{1554, 3223},
	{1560, 3217},
	{1565, 3212},
	{1571, 3206},
	{1553, 3235},
	{1559, 3230},
	{1570, 3219},
	{1576, 3214},
	{1558, 3241},
	{1564, 3236},
	{1570, 3230},
	{1575, 3225},
	{1580, 3221},
	{1563, 3249},
	{1569, 3244},
	{1575, 3239},
	{1580, 3234},
	{1585, 3228},
	{1565, 3224},
	{1537, 3227},
	{1543, 3234},
	{1548, 3240},
	{1553, 3247},
	{1559, 3255},
	{1570, 3255},
	{1571, 3263},
	{1576, 3257},
	{1576, 3269},
	{1581, 3265},
	{1597, 3223},
	{1599, 3232},
	{1604, 3227},
	{1605, 3239},
	{1609, 3235},
	{1534, 3222},
	{1540, 3217},
	{1546, 3211},
	{1551, 3206},
	{1556, 3201},
	{1562, 3195},
	{1567, 3190},
	{1571, 3194},
	{1576, 3201},
	{1581, 3209},
	{1585, 3216},
	{1590, 3223},
	{1573, 3249},
	{1578, 3244},
	{1583, 3239},
	{1588, 3234},
}

function ShopNTT()
	local nNeedCash = 250000;
	if (GetCash() < nNeedCash) then
		Msg2Player("Nhµ ng≠¨i kh´ng c„ ÆÒ <color=orange> 25 vπn l≠Óng <color> Æıng Æ’n t◊m ta");
		return
	end;
	Pay(nNeedCash);
	Msg2Player("ßπi hi÷p nhÀn Æ≠Óc <color=green> 1 S∏t thÒ gi∂n l‘ hÈp <color> !.")
	AddItem(7,131,0,0,0,0,0);
end;

function no()
end;


