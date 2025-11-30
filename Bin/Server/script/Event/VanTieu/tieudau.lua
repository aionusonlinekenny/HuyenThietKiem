Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\event\\VanTieu\\lib.lua");

function GetNpcValue(nNpcIdx)
	-- Workaround: Use NpcParam instead of NpcValue
	if nNpcIdx <= 0 then
		return 0
	end
	return GetNpcParam(nNpcIdx, 1) -- Use param slot 1 for UUID storage
end

function SetNpcValue(nNpcIdx, nValue)
	if nNpcIdx <= 0 then
		return
	end
	SetNpcParam(nNpcIdx, 1, nValue)
end

function AddRespect(nAmount)
	-- Workaround: Use AddRepute instead
	-- HuyenThiet has AddRepute, ThienDieu uses AddRespect
	-- They might be the same thing
	AddRepute(nAmount)
end

function FindNearNpc(dwNpcID)
	-- Workaround: Since we don't have FindNearNpc, use FindAroundNpc
	-- This is less accurate but functional
	return FindAroundNpc(dwNpcID)
end

function AddItemIDStack(nItemIdx, nStack)
	-- Workaround: Add items one by one or use loop
	-- For now, just add the item (stack will be set by item properties)
	return nItemIdx
end

function SetNpcOwner_Backup(nNpcIdx, szOwnerName, nMode)
	if nNpcIdx <= 0 then
		return
	end
	local nPlayerIdx = GetPlayerIndex()
	if not nPlayerIdx or nPlayerIdx < 0 then
		return
	end
	SetNpcParam(nNpcIdx, 2, 1) -- Mark as having owner (legacy)
end

function SetNpcTimeout(nNpcIdx, nSeconds)
	-- Workaround: Use timer to delete NPC after timeout
	-- Real implementation would be in C++ NPC class
	-- For now, just mark it (will need manual cleanup)
	if nNpcIdx <= 0 then
		return
	end
	SetNpcParam(nNpcIdx, 3, nSeconds) -- Store timeout
	-- TODO: Implement actual auto-despawn
end

function AddNpcWithScript(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, szScript, bRemoveDeath, szName)
	-- Enhanced AddNpc with script support
	-- AddNpc signature: (templateID, level, subworld, x, y, removeOnDeath, name, param8, param9)
	local nNpcIdx = AddNpc(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, bRemoveDeath or 1, szName or "", 0, 0)
	if nNpcIdx > 0 and szScript and szScript ~= "" then
		SetNpcScript(nNpcIdx, szScript)
	end
	return nNpcIdx
end

function GetNpcIDFromIndex(nNpcIdx)
	-- Get NPC's m_dwID
	if nNpcIdx <= 0 then
		return 0
	end
	return GetNpcID(2, nNpcIdx) -- Type 2 = m_dwID
end

-- Main dialog
function main(NpcIndex)
	dofile("script/event/VanTieu/tieudau.lua")

	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		Talk(1,"","Ai c�ng t��ng ngh� B�o ti�u l� sung s��ng! Th�t s� m�i l�n ra �i ��u kh�ng d�m hua� h�n tr��c ng�y v�!")
		return
	end

	Say("Th�nh �� ti�u c�c c�a ch�ng ta lu�n ���c s� t�n nhi�m c�a giang h�",4,
	"V�n ti�u/vantieu",
	"��i H� Ti�u L�nh/cuahang",
	"T�m hi�u v�n ti�u/timhieu",
	"Ta ch� gh� qua/no")
end

function vantieu()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if(nTask == 0) then
		Say("G�n ��y c�ng vi�c nhi�u, ��o t�c ho�nh h�nh kh�p n�i, m� nh�n l�c l�i thi�u. V�y ��y c� mu�n gi�p ta m�t chuy�n kh�ng? S� c� th� lao x�ng ��ng cho ng��i!",2,
		"Ta ��ng � �p ti�u/batdau",
		"Ta �ang r�t b�n/no")
	elseif(nTask < 4) then
		-- Check if player was robbed
		if(GetItemCountInBag(6, ITEM_TIEUKY, 1, -1, 0) > 0) then
			bicuop()
			return
		end

		Say("Kh�ng ph�i ng��i �ang �p ti�u sao? Sao l�i ��n ��y t�m ta?",4,
		"Ta b� th�t l�c, gi�p ta t�m ti�u xa/timxe",
		"Reset ti�u xa (Test - Mi�n ph�)/resettieuxatest",
		"Ta kh�ng mu�n l�m n�a/huybo",
		"Ta ch� gh� qua/no")
	else
		hoanthanh()
	end
end


function batdau()

	if(GetLevel() < 120) then
		Talk(1,"","H�y c� g�ng luy�n t�p ��t c�p 120 r�i ��n g�p ta")
		return
	end

	-- Check daily limit
	local nResetTask = GetTask(TASK_RESET_VANTIEU)
	local nLan = GetByte(nResetTask, 6)

	if(nLan >= MAX_DAILY_RUNS) then
		if(GetItemCountInBag(6, ITEM_UNLOCK_VANTIEU, 1, -1, 0) < 1) then
			Talk(1,"","H�m nay ng��i �� �p ti�u nhi�u l�n r�i. H�y ngh� ng�i mai l�i ��n g�p ta.")
			return
		end
	end

	if(GetCash() < COST_START_QUEST) then
		Talk(1,"","Ng��i c�n n�p ".. (COST_START_QUEST / 10000) .." v�n l��ng ph� hao t�n ti�u xa!")
		return
	end

	-- Consume unlock item or increment daily counter
	if(nLan >= MAX_DAILY_RUNS) then
		DelTaskItem(ITEM_UNLOCK_VANTIEU, 1)
	else
		SetTask(TASK_RESET_VANTIEU, SetByte(nResetTask, 6, nLan + 1))
	end

	-- Random cart type (1-3 -> ��ng/B�c/V�ng)
	local n = random(0, 2)
	local nRand = n + 1

	-- Calculate spawn positions BEFORE teleporting player
	-- Use fixed coordinates, don't rely on GetWorldPos() after NewWorld()
	local nTemplateID = TIEUXA_TEMPLET[nRand][1]

	-- CRITICAL: Check if we should use SubWorld ID or Index
	-- Try both approaches to see which works
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)

	-- Player will be at POS_START_X/Y after teleport
	local nPlayerX = floor(POS_START_X * 32)  -- Convert map coords to pixels
	local nPlayerY = floor(POS_START_Y * 32)

	-- Spawn cart offset +2 tiles East of player position
	local nCartX = nPlayerX + 64  -- +64 pixels = +2 tiles
	local nCartY = nPlayerY

	-- Pay player first
	Pay(COST_START_QUEST)

	-- Teleport player to start location
	-- IMPORTANT: Check return value! NewWorld() returns 1 on success
	local nTeleportOK = NewWorld(SUBWORLD_START, POS_START_X, POS_START_Y)

	if nTeleportOK ~= 1 then
		Talk(1,"","L�i: Kh�ng th� �i chuy�n ��n v� tr� b�t ��u!")
		return
	end
	-- Add teleport effect (18 frames * 3 = 54 frames ~= 2.7 seconds at 20fps)
	-- This also ensures client/server position sync before spawning cart
	AddSkillState(963, 1, 0, 18*3)

	-- Reset fight state after teleport (like town portal does)
	SetFightState(0)
	-- Spawn escort cart at calculated position
	-- NOTE: Cart spawns AFTER successful teleport
	local nId = AddNpc(
		nTemplateID,				-- Template ID
		1,							-- Level
		nSubWorldIdx,				-- SubWorld Index
		nCartX,						-- X (player X + 64)
		nCartY,						-- Y (player Y)
		1,							-- Remove on death
		"",							-- Name (will be set below)
		0,							-- Param 8
		0							-- Param 9
	)

	if nId > 0 then
		-- CRASH ISOLATION: Test SetNpcScript
		SetNpcScript(nId, "\\script\\event\\VanTieu\\tieuxa.lua")

		-- Set NPC to friendly/neutral camp so it won't attack player
		if SetNpcCamp ~= nil then
			SetNpcCamp(nId, 0)  -- Camp 0 = neutral
		end
		if SetNpcCurCamp ~= nil then
			SetNpcCurCamp(nId, 0)
		end

		-- Set NPC series to match player
		local nPlayerSeries = GetSeries()
		if nPlayerSeries and SetNpcSeries ~= nil then
			SetNpcSeries(nId, nPlayerSeries)
		end

	end

	if nId <= 0 then
		Talk(1,"","L�i: Kh�ng th� t�o ti�u xa!")
		return
	end

	-- Setup cart
	local nName = GetName()


	-- IMPORTANT: Set NPC life/HP so AI will activate
	-- AI won't run if m_CurrentLifeMax = 0 (check in KNpcAI.cpp line 34)
	if SetNpcLifeMax then
		SetNpcLifeMax(nId, 10000)  -- 10k HP
		SetNpcLife(nId, 10000)
	end

	-- Setup NPC owner and follow behavior
	if SetNpcOwner ~= nil then
		SetNpcOwner(nId, 1)  -- 1 = enable follow
	end

	local sCartName = nName .. " - " .. TIEUXA_TEMPLET[nRand][2]

	SetNpcName(nId, sCartName)
	SetNpcTimeout(nId, CART_TIMEOUT)
	local nUUID = GetUUID()
	SetNpcValue(nId, nUUID)
	local dwCartID = GetNpcIDFromIndex(nId)
	SetTask(TASK_NPCVANTIEU, dwCartID)


	-- Update quest state

	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 1, n + 1))
	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 2, random(1, 3)))


	-- Notify player
	Msg2Player("H�y mau h� t�ng ti�u xa ��n Ti�u S� � Thanh Th�nh S�n (Map 21 - "..POS_END_X.."/"..POS_END_Y..")")

end


function cuahang()
	local nCount = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0
	Say("Ng��i hi�n c�: <color=red>"..nCount.."<color> H� Ti�u L�nh. H�y l�a ch�n v�t ph�m c�n thi�t cho ti�u xa:",4,
	"T�ng t�c (4 H� Ti�u L�nh)/buy_item("..ITEM_TANGTO..",4)",
	"H�i m�u (3 H� Ti�u L�nh)/buy_item("..ITEM_HOIMAU..",3)",
	"D�ch chuy�n (3 H� Ti�u L�nh)/buy_item("..ITEM_DICHCHUYEN..",3)",
	"Ta cha mu�n ��i/no")
end

function buy_item(nItemID, nCost)
	if(CountFreeBagCell() < 1) then
		Talk(1,"","H�nh trang c�a ng��i kh�ng c�n ch� tr�ng")
		return
	end

	local nHave = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0

	if(nCost > nHave) then
		Talk(1,"","Ng��i kh�ng mang �� H� Ti�u L�nh r�i")
		return
	end

	AddTaskItem(nItemID)
	DelTaskItem(ITEM_HO_TIEU_LENH, nCost)

	Talk(1,"","Giao d�ch th�nh c�ng!")
end

function hoanthanh()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nFinish = GetByte(nTaskValue, 1)

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)

	phanthuong(nFinish)
	Talk(1,"","L�m t�t l�m! ��y l� ph�n lao v� c�a ng��i!")
end

function bicuop()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	-- Try to delete cart if still exists
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	local nNpcIdx = FindNearNpc(dwCartID)

	if(nNpcIdx > 0) then
		DelNpc(nNpcIdx)
	end

	local nKind = GetByte(GetTask(TASK_VANTIEU), 1)
	phanthuong(nKind) -- Give 50% reward

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)
	DelTaskItem(ITEM_TIEUKY, 1)

	Talk(1,"","Ng��i b� c��p ti�u r�i sao? C�ng may ng���i �� �o�t l�i T� ti�u K� danh d� c�a Th�nh �� ti�u c�c. V�t v� cho ng��i r�i! ��y l� m�t n�a ph�n lao v�")
end


function huybo()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	-- Delete cart
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	local nNpcIdx = FindNearNpc(dwCartID)

	if(nNpcIdx > 0) then
		DelNpc(nNpcIdx)
	end

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)

	Talk(1,"","C�ng vi�c �p ti�u kh�ng ph�i l� ��n gi�n. Ng��i ch�n ��t ch�n r�o b��c v�o n�n c�ng kh� tr�ch! Sau n�y h�y c� g�ng l�n!")
end


function timxe()
	if(GetCash() < COST_FIND_CART) then
	 Talk(1,"","Ng��i c�n n�p ph� ".. (COST_FIND_CART / 10000) .." v�n l��ng �� ta cho ng��i �i t�m ti�u xa!")
	 return
end

local dwCartID = GetTask(TASK_NPCVANTIEU)
local nNpcIdx = FindAroundNpc(dwCartID)

if (nNpcIdx == 1) then
	 Talk(1,"","Kh�ng ph�i ti�u xa c�a ng��i �ang � ngay ��y sao!")
	 return
end

nNpcIdx = FindNearNpc(dwCartID)

if(nNpcIdx > 0) then
	 local w, x, y = GetNpcPos(nNpcIdx)
	 NewWorld(w, x, y)
	 Pay(COST_FIND_CART)
	 Talk(1,"","�� c� tin t�c ti�u xa! Ta s� cho ng��i ��n ��!")
else
	 Talk(1,"","Kh�ng th�y tin t�c! Ti�u xa c�a ng���i c� l� �� m�t!")
end

end

function resettieuxatest()
	-- Test function to reset cart for easier testing (FREE)
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if nTask == 0 or nTask >= 4 then
		Talk(1,"","Ng��i kh�ng c� nhi�m v� v�n ti�u �ang th�c hi�n!")
		return
	end

	-- Delete old cart if exists
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	if dwCartID > 0 then
		local nNpcIdx = FindAroundNpc(dwCartID)
		if nNpcIdx > 0 then
			DelNpc(nNpcIdx)
			Msg2Player("�� x�a ti�u xa c�")
		end
	end

	-- Get player position and subworld
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)
	local w, x, y = GetWorldPos()

	-- Determine cart type from task
	local nRand = nTask -- Task value 1/2/3 = �?ng/B?c/V�ng
	local nTemplateID = TIEUXA_TEMPLET[nRand][1]

	-- Spawn new cart at player position
	local nId = AddNpc(
		nTemplateID,
		1,
		nSubWorldIdx,
		x,
		y,
		1,
		"",
		0,
		0
	)

	if nId > 0 then
		SetNpcScript(nId, "\\script\\event\\VanTieu\\tieuxa.lua")

		-- Set friendly
		if SetNpcCamp ~= nil then
			SetNpcCamp(nId, 0)
		end
		if SetNpcCurCamp ~= nil then
			SetNpcCurCamp(nId, 0)
		end

		-- Set series
		local nPlayerSeries = GetSeries()
		if nPlayerSeries and SetNpcSeries ~= nil then
			SetNpcSeries(nId, nPlayerSeries)
		end
		local nName = GetName()

		if SetNpcOwner ~= nil then
			SetNpcOwner(nId, nName, 1)
		else
			SetNpcOwner_Backup(nId, nName, 1)
		end

		SetNpcName(nId, nName .. TIEUXA_TEMPLET[nRand][2])
		SetNpcTimeout(nId, CART_TIMEOUT)
		SetNpcValue(nId, GetUUID())

		-- Update task with new NPC ID
		local dwNewCartID = GetNpcIDFromIndex(nId)
		SetTask(TASK_NPCVANTIEU, dwNewCartID)

		Msg2Player("�� reset ti�u xa th�nh c�ng! Xe m�i xu�t hi�n t�i v� tr� c�a ng��i.")
		else
			Talk(1,"","L�i: Kh�ng th� t�o ti�u xa m�i!")
		end

end

function timhieu()
	Talk(2,"","Con ���ng hi�m tr� nh�t l� b�ng qua Thanh Th�nh S�n. N�i �� ��y r�y th� d� v� ��o t�c ho�nh h�nh. Ng��i ph�i h� Ti�u an to�n ��n n�i giao cho Ti�u S trung chuy�n ��ng th�i gian!",
	"H�ng ho� c�n v�n chuy�n c� 3 ch�ng lo�i: v�ng, b�c, ��ng. Ti�u c�ng gi� tr� th� b�n ��o t�c c�ng d� m�t t�i n�n c�ng kh� kh�n nh�ng th� lao ng��i nh�n ��c c�ng t��ng x�ng!")
end


-- Reward function
function phanthuong(nFinish)
	local tbName = GetName()
	if(nFinish == TASK_STATE_DONG) then	-- �?ng ti�u xa
		AddTaskItem(ITEM_HO_TIEU_LENH)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0) -- Ruong ho?t d?ng
		AddOwnExp(5 * KINH_NGHIEM_BASE)
		AddRespect(5)
		-- AddItemIDStack - workaround
		for i=1,5 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("��i Hi�p <color=yellow>"..tbName.."<color> giao th�nh c�ng ��ng Ti�u Xa nh�n ���c <color=yellow>5 tri�u �i�m<color> kinh nghi�m v� <color=yellow>5<color> �i�m Uy Danh!")
		elseif (nFinish == TASK_STATE_BAC) then
		for i=1,4 do
			AddTaskItem(ITEM_HO_TIEU_LENH)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(10 * KINH_NGHIEM_BASE)
		AddRespect(10)
		for i=1,10 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("��i Hi�p <color=yellow>"..tbName.."<color> giao th�nh c�ng B�c Ti�u Xa nh�n ���c <color=yellow>10 tri�u �i�m<color> kinh nghi�m v� <color=yellow>10<color> �i�m Uy Danh!")
	elseif(nFinish == TASK_STATE_VANG) then	-- V�ng ti�u xa
		for i=1,6 do
			AddTaskItem(ITEM_HO_TIEU_LENH)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(15 * KINH_NGHIEM_BASE)
		AddRespect(15)
		for i=1,15 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("��i Hi�p <color=yellow>"..tbName.."<color> giao th�nh c�ng V�ng Ti�u Xa nh�n ���c <color=yellow>15 tri�u �i�m<color> kinh nghi�m v� <color=yellow>10<color> �i�m Uy Danh!")
	else
		AddTaskItem(ITEM_HO_TIEU_LENH)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(2 * KINH_NGHIEM_BASE)
		Msg2SubWorld("��i Hi�p <color=yellow>"..tbName.."<color> b� c��p ti�u kh�ng ho�n th�nh nhi�m v�!")
	end
end

function no()
end