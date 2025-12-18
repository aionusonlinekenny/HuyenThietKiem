-- Author:	J
-- Date:	15-09-2015
-- 
Include("\\script\\player\\head.lua")

--
ZBattle 				= {};	-- Kh«ng ®­îc söa
dstop 					= {};	-- danh sach mang top
--
ZBattle.FRAME2TIME 		= 1;	-- Kh«ng ®­îc söa
ZBattle.BAOMING_TIME 	= 15;	-- Thêi gian b¸o danh (phót) - MÆc ®Þnh: 15
ZBattle.FIGHTING_TIME 	= 45;	-- Thêi gian 1 trËn (phót) - MÆc ®Þnh: 45	
ZBattle.ANNOUNCE_TIME 	= 20;	-- Thêi gian th«ng b¸o (gi©y) - MÆc ®Þnh: 20
--
ZBattle.TIMER_1 		= ZBattle.ANNOUNCE_TIME * ZBattle.FRAME2TIME;	-- Thêi gian ch¹y smalltimer.lua (gi©y) - MÆc ®Þnh: 20*18
ZBattle.TIMER_2 		= (ZBattle.FIGHTING_TIME + ZBattle.BAOMING_TIME) * 60  * ZBattle.FRAME2TIME;	-- Thêi gian ch¹y totaltimer.lua (phót) - MÆc ®Þnh: (60 + 30) * 60 * 18
ZBattle.RUNGAME_TIME 	= ZBattle.BAOMING_TIME * 60 * ZBattle.FRAME2TIME / ZBattle.TIMER_1;	-- Giê chiÕn ®Êu (phót) - MÆc ®Þnh: (30 * 60 * 18) / (20 * 18)
ZBattle.MARSHALL_TIME 	= (ZBattle.BAOMING_TIME + 30) * 60 * ZBattle.FRAME2TIME / ZBattle.TIMER_1; -- Giê gäi Nguyªn So¸i (phót) - MÆc ®Þnh: (30 + 30) * 60 * 18 / (20 * 18)
ZBattle.VANISHGAME_TIME = (ZBattle.BAOMING_TIME + ZBattle.FIGHTING_TIME - 20) * 60 * ZBattle.FRAME2TIME / ZBattle.TIMER_1;	-- Giê gäi qu¸i (phót) - MÆc ®Þnh: (30 + 60 - 20) * 60 * 18 / (20 * 18)
--
ZBattle.tbMission = {
	MAIN				= 3,
	STATE				= 2,
	MARSHAL				= 3,
	MARSHAL_DEATH		= 4,
	SONGPOINT			= 5,
	JINPOINT			= 6,
	TIMER				= 7,
	AREASONG			= 8,
	AREAJIN				= 9,
	HOMEIN_X1 			= 10,
	HOMEIN_Y1 			= 11,
	HOMEOUT_X1 			= 12,
	HOMEOUT_Y1 			= 13,
	HOMEIN_X2 			= 14,
	HOMEIN_Y2 			= 15,
	HOMEOUT_X2 			= 16,
	HOMEOUT_Y2 			= 17,
	CALLNPCCOUNT_S 		= 18,
	CALLNPCCOUNT_J 		= 19, 
	KEY_SJ				= 20,
	MEMBER_COUNT		= 21,
	TONG_SOLIDER		= 22,	--MemberCount 	SetGlbMissionV UpdateTabSort
	KIM_SOLIDER			= 23,	--MemberCount 	SetGlbMissionV UpdateTabSort
	TIMERSHOW			= 24,	--Time 			SetGlbMissionV
	TIMERCOUNT			= 25,
	FLAGCLEARTOP		= 26,	--Clear Top TK;
}
--
ZBattle.STATE_WAIT		= 0;
ZBattle.STATE_SIGNED	= 1;
ZBattle.STATE_FIGHT		= 2;
ZBattle.STATE_END		= 3;	
--
--
ZBattle.SMALLTIMER			= 2;
ZBattle.TOTALTIMER			= 3;
--
ZBattle.tbTask = {
	-- LAST_JOIN		= 6,	-- Phe ®· vµo
	-- GROUP			= 7,	-- Phe
	-- TEMP_POINT		= 8,	-- Tæng ®iÓm trong trËn ®¸nh
	-- KILL_ROLE		= 9,	-- Sè ng­êi ch¬i ®· giÕt
	-- ROLE_POINT		= 10,	-- §iÓm giÕt ng­êi ch¬i
	-- KILL_NPC		= 11,	-- Sè Npc ®· giÕt
	-- NPC_POINT		= 12,	-- §iÓm giÕt Npc
	-- BE_KILL			= 13,	-- Sè lÇn tö vong
	-- SERIES			= 14,	-- Sè liªn tr¶m
	-- SERIES_POINT	= 15,	-- §iÓm liªn tr¶m
	-- MAX_SERIES		= 16,	-- Liªn tr¶m cao nhÊt 
	-- SNAP_FLAG		= 17,	-- Sè cê ®· ®o¹t
	-- FLAG_POINT		= 18,	-- §iÓm ®o¹t cê
	-- GET_ITEM		= 19,	-- Sè b¸u vËt ®· nhÆt
	-- ITEM_POINT		= 20,	-- §iÓm b¸u vËt
	-- RANK			= 21,	-- H¹ng
	-- LAST_KEY		= 22,	-- TrËn ®· vµo
	-- LAST_TITLE		= 23,
	-- WEEK_POINT		= 24,
	-- AY_POINT		= 2013	-- §iÓm Tèng Kim
	
	LAST_JOIN		= 300,	-- Phe ®· vµo
	GROUP			= 7,	-- Phe
	TEMP_POINT		= 302,	-- Tæng ®iÓm trong trËn ®¸nh
	KILL_ROLE		= 303,	-- Sè ng­êi ch¬i ®· giÕt
	ROLE_POINT		= 304,	-- §iÓm giÕt ng­êi ch¬i
	KILL_NPC		= 305,	-- Sè Npc ®· giÕt
	NPC_POINT		= 306,	-- §iÓm giÕt Npc
	BE_KILL			= 307,	-- Sè lÇn tö vong
	SERIES			= 308,	-- Sè liªn tr¶m
	SERIES_POINT	= 309,	-- §iÓm liªn tr¶m
	MAX_SERIES		= 310,	-- Liªn tr¶m cao nhÊt 
	SNAP_FLAG		= 311,	-- Sè cê ®· ®o¹t
	FLAG_POINT		= 312,	-- §iÓm ®o¹t cê
	GET_ITEM		= 313,	-- Sè b¸u vËt ®· nhÆt
	ITEM_POINT		= 314,	-- §iÓm b¸u vËt
	RANK			= 315,	-- H¹ng
	LAST_KEY		= 316,	-- TrËn ®· vµo
	LAST_TITLE		= 317,
	WEEK_POINT		= 318,
	AY_POINT		= 2013	-- §iÓm Tèng Kim
}
--
ZBattle.tbTaskTmp = {
	TITLE		= 6,	-- Qu©n hµm
	LAST_DEATH  = 7,	-- Thêi gian ®Õm ng­îc trong doanh tr¹i
	ANTI_POST	= 8,	-- Chèng post ®iÓm
}
--
ZBattle.MAX_MEMBERCOUNT 		= 150;		-- Sè l­îng player mçi bªn -- MÆc ®Þnh: 150
ZBattle.TIME_PLAYER_REV 		= 8;		-- Thêi gian delay doanh tr¹i (gi©y) - MÆc ®Þnh: 8
ZBattle.TIME_PLAYER_STAY		= 120;		-- Thêi gian check AFK (gi©y) - MÆc ®Þnh: 120
--
ZBattle.tbRuleName = { "<color=plum>Cõu s¸t <color>","<color=plum>B¶o vÖ nguyªn so¸i <color>","<color=plum>§o¹t cê <color>",}
--
ZBattle.tbRankName = {
	{"<color=white>Binh sÜ <color>",0,1000,""},
	{"<color=green>HiÖu óy <color>",1000,3000,"Sinh mÖnh tèi ®a t¨ng 20%"},
	{"<color=blue>Thèng lÜnh <color>",3000,6000,"Sinh mÖnh tèi ®a t¨ng 30% Kh¸ng tÊt c¶ t¨ng 5%",},
	{"<color=purple>Phã t­íng <color>",6000,10000,"Sinh mÖnh tèi ®a t¨ng 40% Kh¸ng tÊt c¶ t¨ng 10%",},
	{"<color=gold>§¹i t­íng <color>",10000,100000,"Sinh mÖnh tèi ®a t¨ng 50% Kh¸ng tÊt c¶ t¨ng 15%",},
	{"",0,0,""},
}
--
ZBattle.RANK_SKILL 			= 661;	-- Kü n¨ng qu©n hµm 
--
ZBattle.SIGN_MAP 			= 325;	-- ID b¶n ®å b¸o danh
ZBattle.tbSignMapPos 		= {{1541, 3178} , {1570, 3085}};	-- Täa ®é b¶n ®å b¸o danh
ZBattle.FIGHT_MAP 			= 380;	-- ID b¶n då chiÕn ®Êu
ZBattle.tbFightMapPos 		= {{1152, 3158}, {1516, 2537}};	-- Täa ®é b¶n ®å chiÕn ®Êu
--
ZBattle.tbSecNpcPos 		= {"npcpos1", "npcpos2"};	-- Section täa ®é add npc
ZBattle.DoctorId 			= {55, 49};		-- ID Npc b¸n thuèc
ZBattle.SymbolId 			= {629,630};	-- Ch­a dïng
ZBattle.DepositId 			= 625;	-- ID R­¬ng chøa ®å
ZBattle.FlagId 				= {626,628,627};	-- Ch­a dïng	
ZBattle.TransportId 		= {55,49};	--
ZBattle.NpcSoliderId 		= {{631,632,633,634,635,636}, {637, 638, 639, 640, 641, 642}} -- ID Qu¸i  
ZBattle.tbNpcSoliderCount	= {50,10,8,4,1} -- Sè l­îng qu¸i
ZBattle.tbNpcPoint 			= {5,50,200,400,500} -- §iÓm th­ëng khi ®¸nh chÕt qu¸i
--
ZBattle.BOSS_BONUS_POINT	= 1000
--
ZBattle.WIN_BONUS_POINT		= 1000
ZBattle.TIE_BONUS_POINT		= 750
ZBattle.LOSE_BONUS_POINT	= 500
--
ZBattle.ITEM_BONUS_POINT	= 25;
--
ZBattle.MAX_CALL_NPC		= 10;
--
ZBattle.tbFile = {
	FILE_HOMETRAP1 				= "\\script\\mission\\battles\\hometrap1.lua",
	FILE_HOMETRAP2 				= "\\script\\mission\\battles\\hometrap2.lua",
	FILE_CENTERTRAP 			= "\\script\\mission\\battles\\centertrap.lua",
	FILE_CENTERTOHOMETRAP1 		= "\\script\\mission\\battles\\homeouttrap1.lua",
	FILE_CENTERTOHOMETRAP2 		= "\\script\\mission\\battles\\homeouttrap2.lua",
	FILE_DOCTOR1 				= "\\script\\mission\\battles\\doctor.lua",
	FILE_DOCTOR2 				= "\\script\\mission\\battles\\doctor.lua",
	FILE_DEPOSIT 				= "\\script\\mission\\battles\\openbox.lua",
	FILE_PLAYERDEATH 			= "\\script\\mission\\battles\\playerdeath.lua",
	FILE_PLAYERLOGOUT 			= "\\script\\mission\\battles\\playerlogout.lua",
}
--
ZBattle.tbFileNpcDeath = {
	"\\script\\mission\\battles\\npcdeath_1.lua",
	"\\script\\mission\\battles\\npcdeath_2.lua",
	"\\script\\mission\\battles\\npcdeath_3.lua",
	"\\script\\mission\\battles\\npcdeath_4.lua",
	"\\script\\mission\\battles\\npcdeath_5.lua",
	"\\script\\mission\\battles\\npcdeath_6.lua",
}
--
ZBattle.tbAwardRank = {250,500,1000,1500,2000}	-- Exp
--
ZBattle.tbAwardTopEx = { -- ItemID, ItemName
	{11,"<color=green>Phóc Duyªn Lé (®¹i)<color>"},
	{12,"<color=green>Phóc Duyªn Lé (trung)<color>"},
	{13,"<color=green>Phóc Duyªn Lé (tiÓu)<color>"},
}
--
ZBattle.tbAwardTop = { -- ItemID, ItemName
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
	{9,"<color=green>Qu¶ Huy Hoµng (Cao)<color>"},
}

ZBattle.tbAwardWeek = {
	50,45,40,35,30,25,
}

----------------------------------
--  Rêi khái Tèng Kim
----------------------------------
function ZBattle:LeaveMap()
	i = GetTask(self.tbTask.GROUP)
	if(i == 0) then
		i = 1
	end
	NewWorld(self.SIGN_MAP, self.tbSignMapPos[i][1], self.tbSignMapPos[i][2])
	SetFightState(0)
end

----------------------------------
--
----------------------------------
function ZBattle:LeaveGame()
	ZPlayer:Refresh()
	SetTitle(GetTask(self.tbTask.LAST_TITLE))
	SetTask(self.tbTask.LAST_TITLE, 0)
	SetTask(self.tbTask.SERIES,0)
	SetTask(self.tbTask.SERIES_POINT,0)
	SetTaskTemp(self.tbTaskTmp.TITLE,0)
	SetTaskTemp(self.tbTaskTmp.LAST_DEATH,0)
	SetTaskTemp(self.tbTaskTmp.ANTI_POST,0)
	SetDeathScript("")
	SetLogoutScript("")
	SetFightState(0)
	SetPunish(0)
	SetCreateTeam(1)
	ForbitTrade(0)
	ForbidChangePK(0)
	SetPKFlag(0)
	ForbidTownPortal(0)
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(ZBattle.FIGHT_MAP)
	if(GetMissionV(ZBattle.tbMission.STATE) < 2) then
		SetTask(self.tbTask.LAST_JOIN, 0);
	end
	SubWorld = nOldSubWorld;
end

----------------------------------
--
----------------------------------
function ZBattle:Pop2SignMap()
	local nOldPlayerIndex = PlayerIndex
	local tbPlayer = {}
	local nCount = 0
	local i, nIdx, nPlayerIdx = 0,0,0
	for i = 1, 300 do 
		nIdx, nPlayerIdx = GetNextPlayer(self.tbMission.MAIN, nIdx, 0)
		if(nPlayerIdx > 0) then
		 	PlayerIndex = nPlayerIdx
			local nRestTime = GetGameTime() - GetTaskTemp(self.tbTaskTmp.LAST_DEATH)
		 	if( (GetFightState() == 0) and (nRestTime >= self.TIME_PLAYER_STAY ) ) then
		 		nCount = nCount + 1
		 		tbPlayer[nCount] = nPlayerIdx
		 	end
		end
		if(nIdx <= 0) then 
	 		break
	 	end
	end
	
	if(nCount > 0 ) then
		for i = 1, nCount do
			PlayerIndex = tbPlayer[i]
		 	local nCurCamp = GetCurCamp()
			Msg2Player("Ngåi ë hËu doanh qu¸ <color=green>"..(floor(self.TIME_PLAYER_STAY/60)).." phót<color>. BÞ ®Èy ra ngoµi!")
			NewWorld(self.SIGN_MAP, self.tbSignMapPos[nCurCamp][1], self.tbSignMapPos[nCurCamp][2])			
		end
	end
	PlayerIndex = nOldPlayerIndex
end

----------------------------------
--
----------------------------------
function ZBattle:AddTrapSJ(sTrapFile, sScriptFile)
	local nCount = self:GetTabFileHeight(sTrapFile)
	local nMapId = SubWorldIdx2ID(SubWorld);
	local dwScriptId = FileName2Id(sScriptFile)
	local i = 0
	for i = 1, nCount - 1 do
		local x = self:GetTabFileData(sTrapFile, i + 1, 1);
		local y = self:GetTabFileData(sTrapFile, i + 1, 2);
		AddMapTrap(nMapId, x, y, dwScriptId)
	end
	self:ClearTabFile(sTrapFile)
end

----------------------------------
--
----------------------------------
function ZBattle:Add_A_DialogNpc(scriptfile, tnpcid, x, y, name)
		local npcidx = 0
		if (name ~= nil or name ~= "") then
			npcidx = AddNpc(tnpcid, 1, SubWorld, x, y, 0, name, 0);
		else
			npcidx = AddNpc(tnpcid, 1, SubWorld, x, y )
		end
		
		if (npcidx > 0) then
			SetNpcScript(npcidx, scriptfile)
		end
		return npcidx
end

----------------------------------
--
----------------------------------
function ZBattle:Add_FightNpc(sNpcFile, nNpcID, nLevel, nCamp, sDeathScript, sName)
	local nAddCount = 0
	local nCount = self:GetTabFileHeight(sNpcFile) - 1;
		
	local sNpcName = ""
	if(sName ~= nil or sName ~= "" ) then
		sNpcName = sName
	end
	
	local nNpcIdx = 0
	for i = 1, nCount  do
		x = self:GetTabFileData(sNpcFile, i + 1, 1)
		y = self:GetTabFileData(sNpcFile, i + 1, 2)
		nNpcIdx = AddNpc(nNpcID, nLevel, SubWorld, x, y)
		if(nNpcIdx > 0) then
			SetNpcCurCamp(nNpcIdx, nCamp)
			SetNpcScript(nNpcIdx, sDeathScript)
			nAddCount = nAddCount + 1
		end
	end
	self:ClearTabFile(sNpcFile)
	return nAddCount
end

----------------------------------
--
----------------------------------
function ZBattle:Add_Rand_FightNpc(sNpcFile, nNpcID, nLevel, nCamp, nCount, sDeathScript, bRemoveWhenDeath, sName, nBoss,nKind)
	local nAddCount = 0
	if(nCount <= 0) then
		return
	end
	local nTotalCount = self:GetTabFileHeight(sNpcFile) - 1;
	if(nTotalCount <= 0 ) then
		return
	end
	
	local bRemoveDeath = 0
	local sNpcName = ""
	local nBossId = 0
	local nKindNormal = 0
	
	if(bRemoveWhenDeath ~= nil) then
		bRemoveDeath = bRemoveWhenDeath
	end
	
	if( (sName ~= nil) or (sName ~= "") ) then
		sNpcName = sName
	end
	
	if(nBoss ~= nil) then
		nBossId = nBoss
	end
	if(nKind ~= nil) then
		nKindNormal = nKind
	end
	local i, id, x, y, nNpcIdx = 0, 0, 0, 0, 0
	for i = 1, nCount  do
		id = random(nTotalCount)
		x = self:GetTabFileData(sNpcFile, id + 1, 1)
		y = self:GetTabFileData(sNpcFile, id + 1, 2)
		nNpcIdx = AddNpc(nNpcID, nLevel, SubWorld, x, y, bRemoveDeath, sNpcName, nBossId,nKindNormal)
		if (nNpcIdx > 0) then
			SetNpcCurCamp(nNpcIdx, nCamp)
			SetNpcScript(nNpcIdx, sDeathScript)
			nAddCount = nAddCount + 1
		end
	end
	self:ClearTabFile(sNpcFile)
	return nAddCount
end

----------------------------------
--
----------------------------------
function ZBattle:AutoSelectMapArea(mapfile,areacount)
	local s_area = 0
	local j_area = 0
	local areatmp = random(areacount);
	s_area = areatmp
	while(j_area == 0) do 
		areatmp = random(areacount)
		if(areatmp ~= s_area) then
			j_area = areatmp
		end
	end
	return s_area, j_area
end

----------------------------------
--
----------------------------------
function ZBattle:GetAData(file)
	local totalcount = self:GetTabFileHeight(file) - 1;
	local id = random(totalcount);
	local x = self:GetTabFileData(file, id + 1, 1);
	local y = self:GetTabFileData(file, id + 1, 2);
	
	self:ClearTabFile(file)
	return x,y
end

----------------------------------
--
----------------------------------
function ZBattle:Str2XYData(str)
	local m = strfind(str,",")
	local x = tonumber(strsub(str,0,m-1))
	local y = tonumber(strsub(str,m+1))
	return x,y
end

----------------------------------
--
----------------------------------
function ZBattle:GetIniFileData(sMapFile, sSect, sKey)
	if (IniFile_Load(sMapFile) == 0) then 
		print("Load IniFile Error!"..sMapFile)
		return ""
	else
		sResult = IniFile_GetData(sMapFile, sSect, sKey)
		IniFile_Clear(sMapFile)
		return sResult
	end
end

----------------------------------
--
----------------------------------
function ZBattle:SaveIniFileData(mapfile, sect, key, value)
	if (IniFile_Load(mapfile) == 0) then 
		return
	else
		IniFile_SaveData(mapfile, sect, key, value)	
	end
end

----------------------------------
--
----------------------------------
function ZBattle:GetTabFileHeight(sMapFile)
	if (TabFile_Load(sMapFile) == 0) then
		print("Load TabFileError!"..sMapFile);
		return 0
	end
	return TabFile_GetRowCount(sMapFile)
end;

----------------------------------
--
----------------------------------
function ZBattle:GetTabFileData(sMapFile, nRow, nCol)
	if (TabFile_Load(sMapFile) == 0) then
		print("Load TabFile Error!"..sMapFile)
		return 0
	else
		return tonumber(TabFile_GetCell(sMapFile, nRow, nCol))
	end
end

----------------------------------
--
----------------------------------
function ZBattle:ClearTabFile(sFile)
	return TabFile_Clear(sFile)
end

----------------------------------
--
----------------------------------
function ZBattle:GetMinAndSec(nSec)
	local nRestMin = floor(nSec / 60)
	local nRestSec = mod(nSec,60)
	return nRestMin, nRestSec
end
----------------------------------
--
----------------------------------