Include("\\script\\lib\\filedirectory.lua")
Include("\\script\\mission\\vuotai\\killerboss.lua");
JPreLoadMap = {}
--------------------------------------
--
--------------------------------------
JPreLoadMap.tbUnLoad = {-- Dialoger, Normal, Trap, Object
	[53] = {1,1,1,1}, -- ba lang huyen
	[78] = {1,1,1,1},	-- tuong duong
	[1] = {1,1,1,1},	-- phuong tuong
	[37] = {1,1,1,1},	-- bien kinh
	[11] = {1,1,1,1},	-- thanh do
	[162] = {1,1,1,1},	-- dai ly
	[80] = {1,1,1,1},	-- Duong chau
	[176] = {1,1,1,1},	-- Lam An
	[2] = {1,1,1,0},	-- Hoa Son
	[3] = {0,0,1,0},	-- Kiem cac tay bac
	[70] = {0,0,1,0},	-- Vu Lang Son
	[4] = {0,0,1,0},	-- kim quang dong
	[74] = {0,0,1,0},	-- mieu linh
	[90] = {0,0,1,0},	-- phuc nguu son dong
	[6] = {0,0,1,0},	-- toa van dong
	[41] = {0,0,1,0},	-- phuc nguu son tay
	[91] = {0,0,1,0},	-- me cung ke quan dong
	[38] = {0,0,1,0},	-- thiet thap tang 1
	[39] = {0,0,0,0},	-- thiet thap tang 2
	[42] = {0,0,1,0},	-- thien tam dong 
	[79] = {0,0,1,0},	-- mat dao nha mon 
	[158] = {0,0,1,0},	-- me cung cam dia 
	[166] = {0,0,1,0},	-- thiet thap tang 3 
	[72] = {0,1,1,0},	-- dai tu dong 
	[142] = {0,1,1,0},	-- duoc vuong dong 1 -> 2 
	[319] = {0,1,1,0},	-- lam du quan 
	[75] = {0,1,1,0},	-- khoa lang dong  
	[224] = {0,1,1,0},	-- sa mac dia bieu 
	[320] = {0,1,1,0},	-- chan nui truong bach 
	[225] = {0,1,1,0},	-- sa mac 1 
	[226] = {0,1,1,0},	-- sa mac 2 
	[227] = {0,1,1,0},	-- sa mac 3 
	[321] = {0,1,1,0},	-- truong bach nam 
	[322] = {0,1,1,0},	-- truong bach bac 
	[336] = {1,1,1,0},	-- phong lang do 
	[340] = {0,1,1,0},	-- mac cao quat 
	[208] = {1,0,0,0},	-- prison
	[396] = {1,0,0,1},	-- lien dau
	[325] = {1,0,0,0},	-- tong kim
	[380] = {1,0,0,0},	-- tong kim
	[242] = {1,1,1,1},	-- dao tay tuy
	[342] = {1,1,0,1},	-- tich tinh dong
	[394] = {1,1,0,0},	-- thien bao kho
	[21] = {1,0,1,0},	-- thanh thanh son
}
--------------------------------------
--
--------------------------------------
function JPreLoadMap:KillerBoss()
InitLoadBoss();--Boss sat thu;
end;
--------------------------------------
--
--------------------------------------
function JPreLoadMap:OnLoadMaps(mapid)
	self:AddDiagloger(mapid)
	self:AddNormal(mapid)
	self:AddTrap(mapid)
	self:AddObject(mapid)
	self:AddTrapTichTinhDong()
	self:AddTrapThienBaoKho()
	--
	--

end;
--------------------------------------
--
--------------------------------------
function JPreLoadMap:AddTrapTichTinhDong()
	sScriptFile = "\\script\\maps\\trap\\342\\excuter.lua";
	local nMapId = SubWorldIdx2ID(57);
	local dwScriptId = FileName2Id(sScriptFile)
	AddMapTrap(nMapId, 37312, 76768, dwScriptId)
	AddMapTrap(nMapId, 37344, 76768, dwScriptId)
	AddMapTrap(nMapId, 37344, 76736, dwScriptId)
	AddMapTrap(nMapId, 37376, 76704, dwScriptId)
	AddMapTrap(nMapId, 37408, 76704, dwScriptId)
	AddMapTrap(nMapId, 37312, 76800, dwScriptId)
end;

function JPreLoadMap:AddTrapThienBaoKho()
	sScriptFile = "\\script\\event\\thienbaokho\\trap.lua";
	local nMapId = SubWorldIdx2ID(60);
	local dwScriptId = FileName2Id(sScriptFile)
	AddMapTrap(nMapId, 45152, 102272, dwScriptId)
	AddMapTrap(nMapId, 45184, 102272, dwScriptId)
	AddMapTrap(nMapId, 45216, 102272, dwScriptId)
	AddMapTrap(nMapId, 45248, 102272, dwScriptId)
	AddMapTrap(nMapId, 45280, 102272, dwScriptId)
	AddMapTrap(nMapId, 45312, 102272, dwScriptId)
	AddMapTrap(nMapId, 45344, 102272, dwScriptId)
	AddMapTrap(nMapId, 45376, 102272, dwScriptId)
end;
--------------------------------------
--
--------------------------------------
function JPreLoadMap:AddObject(mapid)
	if(self.tbUnLoad[mapid][4] == 0) then
		return
	end
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(mapid)
	local sFile = SetFileName(format("\\library\\maps\\obj\\%d.txt",mapid))
	local nResult = IsValidFile(sFile)
	local nNum = GetRow(sFile)
	local i, nObjID, nMpsX, nMpsY, nDir, nState, sScriptFile, bIsLoad
	for i = 2, nNum do
		bIsLoad = tonumber( GetCell(sFile,i,8) )
		if(bIsLoad == 1) then
			nObjID = tonumber( GetCell(sFile,i,1) )
			nMpsX = tonumber( GetCell(sFile,i,3) )
			nMpsY = tonumber( GetCell(sFile,i,4) )
			nDir = tonumber( GetCell(sFile,i,5) )
			nState = tonumber( GetCell(sFile,i,6) )
			sScriptFile = GetCell(sFile,i,7)
			AddMapObject(nObjID, nMpsX, nMpsY, nDir, nState, sScriptFile)
		end
	end
	CloseFile(sFile)
	SubWorld = nOldSubWorld
end
--------------------------------------
--
--------------------------------------
function JPreLoadMap:AddTrap(mapid)

	if(self.tbUnLoad[mapid][3] == 0) then
		return
	end
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(mapid)
	local sFile = SetFileName(format("\\library\\maps\\trap\\%s.txt", mapid))
	local nResult = IsValidFile(sFile)
	local nNum = GetRow(sFile)
	local i, nRegion, nCellX, nCellY, sScriptFile, bIsLoad
	for i = 2, nNum do
		bIsLoad = tonumber( GetCell(sFile,i,6) )
		if(bIsLoad == 1) then
			nRegion = tonumber( GetCell(sFile,i,2) )
			nCellX = tonumber( GetCell(sFile,i,3) )
			nCellY = tonumber( GetCell(sFile,i,4) )
			sScriptFile = GetCell(sFile,i,5)
			AddMapTrapByCell(nRegion, nCellX, nCellY, sScriptFile)
		end
	end
	CloseFile(sFile)
	SubWorld = nOldSubWorld
end
--------------------------------------
--
--------------------------------------
function JPreLoadMap:LoadOldNpc()
	
	local nOldSubWorld = SubWorld
	local sFile = SetFileName("\\library\\maps\\npc_load.txt")
	local nResult = IsValidFile(sFile)
	local nNum = GetRow(sFile)
	local i, nNpcIdx, nNpcID, nMapIdx, nPosX, nPosY, sScriptFile, sName, bIsLoad
	for i = 2, nNum do
		bIsLoad = tonumber( GetCell(sFile,i,7) )
		if(bIsLoad == 1) then
			nNpcID = tonumber( GetCell(sFile,i,1) )
			nMapID = tonumber( GetCell(sFile,i,2) )
			nPosX = tonumber( GetCell(sFile,i,3) )
			nPosY = tonumber( GetCell(sFile,i,4) )
			sScriptFile = GetCell(sFile,i,5)
			sName = GetCell(sFile,i,6)
			SubWorld = SubWorldID2Idx(nMapID)
			nNpcIdx = AddNpc(nNpcID, 1, SubWorld, nPosX, nPosY, 0, sName, 0, 3)
			if(sScriptFile ~= nil) then
				SetNpcScript(nNpcIdx, sScriptFile)
			end
		end
	end
	CloseFile(sFile)
	SubWorld = nOldSubWorld
end

--------------------------------------
--
--------------------------------------

function JPreLoadMap:AddDiagloger(mapid)
	if(self.tbUnLoad[mapid][1] == 0) then
		return
	end
	local nOldSubWorld = SubWorld
	local sFile = SetFileName(format("\\library\\maps\\dialoger\\%s.txt", mapid))
	local nResult = IsValidFile(sFile)
	local nNum = GetRow(sFile)
	local i, nNpcIdx, nNpcID, nMapIdx, nPosX, nPosY, sScriptFile, sName, bIsLoad
	for i = 2, nNum do
		bIsLoad = tonumber( GetCell(sFile,i,7) )
		if(bIsLoad == 1) then
			nNpcID = tonumber( GetCell(sFile,i,1) )
			nMapID = tonumber( GetCell(sFile,i,2) )
			nPosX = tonumber( GetCell(sFile,i,3) )
			nPosY = tonumber( GetCell(sFile,i,4) )
			sScriptFile = GetCell(sFile,i,5)
			sName = GetCell(sFile,i,6)
			SubWorld = SubWorldID2Idx(nMapID)
			nNpcIdx = AddNpc(nNpcID, 1, SubWorld, nPosX, nPosY, 0, sName, 0, 3)
			if(sScriptFile ~= nil) then
				SetNpcScript(nNpcIdx, sScriptFile)
			else
				SetNpcScript(nNpcIdx, "\\script\\npc_death_test.lua")
			end
		end
	end
	CloseFile(sFile)
	SubWorld = nOldSubWorld
end
--------------------------------------
--
--------------------------------------
function JPreLoadMap:AddNormal(mapid)
	if(self.tbUnLoad[mapid][2] == 0) then
		return
	end
	local nOldSubWorld = SubWorld
	local sFile = SetFileName(format("\\library\\maps\\normal\\%s.txt", mapid))
	local nResult = IsValidFile(sFile)
	local nNum = GetRow(sFile)
	local i, nNpcIdx, nNpcID, nNpcLevel, nMapIdx, nPosX, nPosY, sScriptFile, sName, bIsLoad
	for i = 2, nNum do
		bIsLoad = tonumber( GetCell(sFile,i,8) )
		if(bIsLoad == 1) then
			nNpcID = tonumber( GetCell(sFile,i,1) )
			nMapID = tonumber( GetCell(sFile,i,2) )
			nPosX = tonumber( GetCell(sFile,i,3) )
			nPosY = tonumber( GetCell(sFile,i,4) )
			sScriptFile = GetCell(sFile,i,5)
			sName = GetCell(sFile,i,6)
			nNpcLevel = GetCell(sFile,i,7)
			SubWorld = SubWorldID2Idx(nMapID)
			nNpcIdx = AddNpc(nNpcID, nNpcLevel, SubWorld, nPosX, nPosY, 0, sName, 0, 0)
			 --IsBoss(nNpcIdx,13);
			SetNpcCurCamp(nNpcIdx,5)
			if (nMapID == 242) then
				IsBoss(nNpcIdx,2);
			elseif (nMapID == 342) then
				IsBoss(nNpcIdx,random(13,15));
			end;
			
			if (nMapID == 242) then
				sScriptFile = "\\script\\global\\luanpcmonsters\\ondeath_luyenskill.lua"
				SetNpcScript(nNpcIdx, sScriptFile)
			elseif (nMapID == 342) then
				sScriptFile = "\\script\\event\\tichtinhdong\\npc_death.lua"
				SetNpcScript(nNpcIdx, sScriptFile)
			elseif (nMapID == 394) then
				sScriptFile = "\\script\\event\\thienbaokho\\npc_death.lua"
				SetNpcScript(nNpcIdx, sScriptFile)
				IsBoss(nNpcIdx,1)	
				SetNpcReduceDamge(nNpcIndex, 30);	
			elseif(sScriptFile ~= nil) then
				sScriptFile = "\\script\\global\\luanpcmonsters\\ondeath_normal.lua"
				SetNpcScript(nNpcIdx, sScriptFile)
			else
				SetNpcScript(nNpcIdx, "\\script\\global\\luanpcmonsters\\ondeath_normal.lua")
			end
		end
	end
	CloseFile(sFile)
	SubWorld = nOldSubWorld
end





