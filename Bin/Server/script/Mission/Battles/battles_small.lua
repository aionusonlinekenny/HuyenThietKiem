----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 12/03/2023
--	Desc: Timer Tèng Kim 1
----------------------------------
Include("\\script\\mission\\battles\\head.lua")
----------------------------
-- Hµm chÝnh
----------------------------
function OnTimer()
	-- LÊy thêi gian
	local nT = GetMissionV(ZBattle.tbMission.TIMER) + 1
	SetMissionV(ZBattle.tbMission.TIMER, nT);
	-- §Õn giê khai chiÕn
	if(nT == ZBattle.RUNGAME_TIME) then
		RunMission(ZBattle.tbMission.MAIN)
		AddGlobalCountNews("ChiÕn tr­êng tèng kim h×nh thøc "..ZBattle.tbRuleName[GetMissionV(ZBattle.tbMission.MAIN)].." ®· b¾t ®Çu. H·y mau tíi chiÕn tr­êng b¸o danh!", 2)
		Msg2SubWorld("Tèng kim ®· chÝnh thøc khai chiÕn!")
	end
	-- §ang trong thêi gian b¸o danh
	if( (nT < ZBattle.RUNGAME_TIME) and (mod(nT, 18) == 0) ) then
		local nRestTime = (ZBattle.RUNGAME_TIME - nT) * ZBattle.TIMER_1 / ZBattle.FRAME2TIME
		local nRestMin,_ = ZBattle:GetMinAndSec(nRestTime)
		AddGlobalCountNews("ChiÕn tr­êng tèng kim h×nh thøc "..ZBattle.tbRuleName[GetMissionV(ZBattle.tbMission.MAIN)].." ®· b¾t ®Çu chiªu mé. Thêi gian b¸o danh cßn l¹i lµ: <color=green>"..nRestMin.." phót<color>.", 2);
		Msg2SubWorld("Thêi gian b¸o danh cßn l¹i lµ: <color=green>"..nRestMin.." phót<color>.")
	end
	-- §ang chiÕn ®Êu
	if(nT >= ZBattle.RUNGAME_TIME) then
		local nSongCount = GetMSPlayerCount(ZBattle.tbMission.MAIN, 1);
		local nJinCount = GetMSPlayerCount(ZBattle.tbMission.MAIN, 2);
		if (nJinCount == 0  or nSongCount) then
			Msg2SubWorld("ChiÕn tr­êng Tèng Kim kÕt thóc v× kh«ng cã tuyÓn thñ tham gia.");
			CloseMission(ZBattle.tbMission.MAIN);
			return
		end;

		if(nT == ZBattle.RUNGAME_TIME) then
			-- Gäi lÝnh Tèng Kim
			if(GetMissionV(ZBattle.tbMission.MAIN) >= 2) then
				ZBattle:BuildFightNpcData()
			end
		else
			-- TÝnh thêi gian gäi nguyªn so¸i
			if(GetMissionV(ZBattle.tbMission.MAIN) == 2) then
				-- Tíi giê gäi nguyªn so¸i
				if(nT == ZBattle.MARSHALL_TIME) then
					if(GetMissionV(ZBattle.tbMission.SONGPOINT) < GetMissionV(ZBattle.tbMission.JINPOINT)) then
						ZBattle:CallMarshal(1)
						SetMissionV(ZBattle.tbMission.MARSHAL, 1)
					elseif(GetMissionV(ZBattle.tbMission.SONGPOINT) > GetMissionV(ZBattle.tbMission.JINPOINT)) then
						ZBattle:CallMarshal(2)
						SetMissionV(ZBattle.tbMission.MARSHAL, 2)
					elseif(GetMissionV(ZBattle.tbMission.SONGPOINT) == GetMissionV(ZBattle.tbMission.JINPOINT)) then
						ZBattle:CallMarshal(2)
						ZBattle:CallMarshal(1)
						SetMissionV(ZBattle.tbMission.MARSHAL, 3)
					end
				end
				-- Gäi nguyªn so¸i cña bªn cßn l¹i
				if (nT == ZBattle.VANISHGAME_TIME) then
					local nMar = GetMissionV(ZBattle.tbMission.MARSHAL)
					if(nMar == 1) then
						ZBattle:CallMarshal(2)
					elseif(nMar == 2) then
						ZBattle:CallMarshal(1)
					end
				end
			end
			-- Check AFK
			ZBattle:Pop2SignMap()
			-- CËp nhËt b¶ng xÕp h¹ng vµ th«ng b¸o ®iÓm
			if(mod(nT, 3) == 0 ) then
				if(GetMissionV(ZBattle.tbMission.MAIN) >= 2) then
					ZBattle:CallFightNpc(nT - ZBattle.RUNGAME_TIME, ZBattle.VANISHGAME_TIME - ZBattle.RUNGAME_TIME)
					local nCountTime = GetMissionV(ZBattle.tbMission.TIMERCOUNT) + 1;
					SetMissionV(ZBattle.tbMission.TIMERCOUNT,nCountTime);
					SetGlbMissionV(ZBattle.tbMission.TIMERSHOW,(45 - GetMissionV(ZBattle.tbMission.TIMERCOUNT)));
				end
				local nCount = GetMissionV(ZBattle.tbMission.MEMBER_COUNT)
				Msg2MSAll(ZBattle.tbMission.MAIN,"Th«ng b¸o: §iÓm tÝch lòy gi÷a 2 phe lµ <color=sbrown>Tèng: "..GetMissionV(ZBattle.tbMission.SONGPOINT).."<color>:<color=plum>Kim "..GetMissionV(ZBattle.tbMission.JINPOINT).."<color>.")
			end
		end
	end
end

----------------------------------
--
----------------------------------
function ZBattle:BuildFightNpcData()
	local mapfile = GetMapInfoFile(self.FIGHT_MAP)	
	local s_area = GetMissionV(self.tbMission.AREASONG)
	local j_area = GetMissionV(self.tbMission.AREAJIN)
	local area_section1 = "Area_"..s_area
	local area_section2 = "Area_"..j_area
	local npcfile = ""
	npcfile = self:GetIniFileData(mapfile, area_section1, self.tbSecNpcPos[2])
	self:Add_Rand_FightNpc(npcfile, self.NpcSoliderId[1][1], 90, 1, self.tbNpcSoliderCount[1], self.tbFileNpcDeath[1],0,"",0,0)
	
	npcfile = self:GetIniFileData(mapfile, area_section2, self.tbSecNpcPos[2])
	self:Add_Rand_FightNpc(npcfile, self.NpcSoliderId[2][1], 90, 2, self.tbNpcSoliderCount[1], self.tbFileNpcDeath[1],0,"",0,0)
end

----------------------------------
--
----------------------------------
function ZBattle:CallFightNpc(nUseTime, nTotalTime)
	if (nUseTime > nTotalTime) then
		return
	end
	
	local mapfile = GetMapInfoFile(self.FIGHT_MAP)	
	local s_area = GetMissionV(self.tbMission.AREASONG)
	local j_area = GetMissionV(self.tbMission.AREAJIN)
	local area_section1 = "Area_"..s_area
	local area_section2 = "Area_"..j_area
	
	local i, nPreNpcCount, nNpcCount, nNowAdd, npcfile = 0,0,0,0,"";
	for i = 2, 5 do 
		nPreNpcCount = floor((nUseTime - 1) / nTotalTime * self.tbNpcSoliderCount[i])
		nNpcCount = floor(nUseTime / nTotalTime * self.tbNpcSoliderCount[i])
		nNowAdd = nNpcCount - nPreNpcCount
		if(nNowAdd > 0) then
			-- print("call song npc count= "..nNowAdd.." rank="..i)
			npcfile = self:GetIniFileData(mapfile, area_section1, self.tbSecNpcPos[random(2)])
			self:Add_Rand_FightNpc(npcfile, self.NpcSoliderId[1][i], 90, 1, nNowAdd, self.tbFileNpcDeath[i], 1,"",0,0)
		end
	end
	
	for i = 2, 5 do 
		nPreNpcCount = floor((nUseTime - 1) / nTotalTime * self.tbNpcSoliderCount[i])
		nNpcCount = floor(nUseTime / nTotalTime * self.tbNpcSoliderCount[i])
		nNowAdd = nNpcCount - nPreNpcCount
		if(nNowAdd > 0) then
			-- print("call jin npc count= "..nNowAdd.." rank="..i)
			npcfile = self:GetIniFileData(mapfile, area_section2, self.tbSecNpcPos[random(2)]);
			self:Add_Rand_FightNpc(npcfile, self.NpcSoliderId[2][i], 90, 2, nNowAdd, self.tbFileNpcDeath[i],1,"",0,0)
		end
	end
end

----------------------------------
--
----------------------------------
function ZBattle:CallMarshal(nCamp)
	local mapfile = GetMapInfoFile(self.FIGHT_MAP)	
	local s_area = GetMissionV(self.tbMission.AREASONG)
	local j_area = GetMissionV(self.tbMission.AREAJIN)
	local area_section1 = "Area_"..s_area
	local area_section2 = "Area_"..j_area
	if(nCamp == 1) then
		local enterpos = self:GetIniFileData(mapfile, area_section1, "generalpos");
		local x,y = self:Str2XYData(enterpos)	
		local npcidx_s = AddNpc(self.NpcSoliderId[1][6], 90, SubWorld, x*32, y*32, 1, "Tèng Nguyªn So¸i", 0,0)
		SetNpcCurCamp(npcidx_s, 1)
		SetNpcScript(npcidx_s, self.tbFileNpcDeath[6])
		Msg2MSAll(self.tbMission.MAIN, "ChiÕn tr­êng th«ng b¸o: <color=sbrown>Nguyªn so¸i phe tèng ®· xuÊt hiÖn<color>.")
	else
		local enterpos = self:GetIniFileData(mapfile, area_section2, "generalpos");
		local x,y = self:Str2XYData(enterpos)	
		local npcidx_j = AddNpc(self.NpcSoliderId[2][6], 90, SubWorld, x*32, y*32, 1, "Kim Nguyªn So¸i", 0,0)
		SetNpcCurCamp(npcidx_j, 2)
		SetNpcScript(npcidx_j, self.tbFileNpcDeath[6])
		Msg2MSAll(self.tbMission.MAIN, "ChiÕn tr­êng th«ng b¸o: <color=plum>Nguyªn so¸i phe kim ®· xuÊt hiÖn<color>.")
	end		
end