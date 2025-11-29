-- Vận Tiêu (Escort) Event Library
-- Ported from ThienDieuOnline
-- Author: Claude AI Assistant
-- Date: 2025-11-15

Include("\\script\\lib\\TaskLib.lua")

-- NPC Template IDs cho tiêu xa
-- CONFIRMED WORKING: NPCs 2085/2086/2087 work correctly!
-- These spawn properly and follow player with AI Mode 8
TIEUXA_TEMPLET = {
	{2085, "Đồng Tiêu Xa"},
	{2086, "Bạc Tiêu Xa"},
	{2087, "Vàng Tiêu Xa"},
}

-- Tỷ lệ spawn các loại tiêu xa (hiện tại chưa dùng)
TIEUXA_RATE = {
	{1,2,3},
	{20,20,10},
}

-- Item IDs (DetailType in Genre 6 = questkey)
-- ✅ ĐÃ TẠO trong questkey.txt (DetailType 68-74)
ITEM_TIEUKY = 68			-- Tiêu Kỳ (dropped when robbed)
ITEM_UNLOCK_VANTIEU = 69	-- Vé mở khóa vận tiêu thêm
ITEM_HO_TIEU_LENH = 70		-- Hồ Tiêu Lệnh (currency/reward)
ITEM_TANGTO = 71			-- Tăng tốc
ITEM_HOIMAU = 72			-- Hồi máu
ITEM_DICHCHUYEN = 73		-- Dịch chuyển
ITEM_RUONG_VANTIEU = 74		-- Rương Vận Tiêu (reward)

-- NPC Template IDs
-- CONFIRMED WORKING: Original IDs work perfectly
NPC_DONG_TIEUXA = 2085		-- Đồng Tiêu Xa
NPC_BAC_TIEUXA = 2086		-- Bạc Tiêu Xa
NPC_VANG_TIEUXA = 2087		-- Vàng Tiêu Xa
NPC_HONG_TIEUXA = 1185		-- Tiêu Xa Hồng (robbed)
NPC_RUONG_CUOP = 844		-- Rương cướp

-- SubWorld IDs (HuyenThiet server)
-- Map 11 = Dương Châu (major city)
SUBWORLD_START = 11			-- Map bắt đầu (Dương Châu)

-- Coordinates (HuyenThiet - map 11)
-- Tiêu Đầu location: 98848, 164768 (updated - working position)
-- Tiêu Sư location: 98784, 164672
POS_START_X = 98848 / 32		-- Vị trí spawn tiêu xa (near Tiêu Đầu)
POS_START_Y = 164768 / 32

POS_END_X = 98784 / 32			-- Vị trí giao tiêu (near Tiêu Sư)
POS_END_Y = 164672 / 32

-- Costs & Limits
COST_START_QUEST = 150000		-- 15 vạn lượng để bắt đầu
COST_FIND_CART = 100000			-- 10 vạn để tìm xe
MAX_DAILY_RUNS = 3				-- Giới hạn 3 lần/ngày
MAX_DAILY_CHEST_LOOTS = 5		-- Giới hạn 5 rương cướp/ngày

-- Timeout
CART_TIMEOUT = 30 * 60			-- 30 phút (in seconds)
CHEST_TIMEOUT = 180				-- 3 phút (in seconds)
ROBBED_CART_TIMEOUT = 3240		-- 54 phút

-- Experience rewards
KINH_NGHIEM_BASE = 1000000		-- 1 triệu exp base

-- Task state values
TASK_STATE_DONG = 4				-- Hoàn thành đồng tiêu xa
TASK_STATE_BAC = 5				-- Hoàn thành bạc tiêu xa
TASK_STATE_VANG = 6				-- Hoàn thành vàng tiêu xa

-- ============================================================================
-- HELPER FUNCTIONS FOR CART NPC TELEPORT
-- ============================================================================

-- Check if player has active Vận Tiêu quest
function VanTieu_HasActiveQuest()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	-- Task 1-3 = active quest (Đồng/Bạc/Vàng)
	-- Task 4-6 = completed, waiting for reward
	if nTask > 0 and nTask < TASK_STATE_DONG then
		return 1
	end
	return 0
end

-- Find cart NPC belonging to player
function VanTieu_FindCartNpc()
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	if dwCartID <= 0 then
		return 0
	end

	-- Try FindAroundNpc first
	local nNpcIdx = FindAroundNpc(dwCartID)
	if nNpcIdx == 1 then
		-- Too close, try FindNearNpc
		nNpcIdx = FindNearNpc(dwCartID)
	end

	return nNpcIdx or 0
end

-- Teleport cart NPC to player's new position
-- Call this AFTER player has been teleported
function VanTieu_TeleportCartToPlayer()
	-- Check if player has active quest
	if VanTieu_HasActiveQuest() == 0 then
		return 0
	end

	-- Find cart NPC
	local nCartIdx = VanTieu_FindCartNpc()
	if nCartIdx <= 0 then
		return 0
	end

	-- Get player's new position
	local nPlayerW, nPlayerX, nPlayerY = GetWorldPos()

	-- Teleport cart NPC to player position (offset slightly)
	-- SetNpcWorldPos(nNpcIdx, subworld, x, y)
	if SetNpcWorldPos then
		SetNpcWorldPos(nCartIdx, nPlayerW, nPlayerX + 64, nPlayerY)
		Msg2Player("Tiêu xa đã theo bạn qua map!")
		return 1
	else
		-- Fallback: Delete old cart and spawn new one at player position
		DelNpc(nCartIdx)

		local nTaskValue = GetTask(TASK_VANTIEU)
		local nTask = GetByte(nTaskValue, 1)
		local nTemplateID = TIEUXA_TEMPLET[nTask][1]
		local nPlayerName = GetName()

		local nSubWorldIdx = SubWorldID2Idx(nPlayerW)
		local nNewCart = AddNpc(nTemplateID, 1, nSubWorldIdx, nPlayerX + 64, nPlayerY, 1, "", 0, 0)

		if nNewCart > 0 then
			SetNpcScript(nNewCart, "\\script\\event\\VanTieu\\tieuxa.lua")
			SetNpcName(nNewCart, nPlayerName .. " - " .. TIEUXA_TEMPLET[nTask][2])

			-- Re-setup owner
			if SetNpcOwner then
				SetNpcOwner(nNewCart, 1)
			end

			-- Update task with new NPC ID
			local dwNewCartID = GetNpcID(2, nNewCart)
			SetTask(TASK_NPCVANTIEU, dwNewCartID)

			Msg2Player("Tiêu xa đã theo bạn qua map!")
			return 1
		end
	end

	return 0
end

-- Helper function for trap scripts
-- Usage: Include this lib, then call VanTieu_NewWorldWithCart(mapID, x, y)
function VanTieu_NewWorldWithCart(nMapID, nX, nY)
	-- Teleport player first
	local nResult = NewWorld(nMapID, nX, nY)

	-- If teleport succeeded and player has active quest, teleport cart too
	if nResult == 1 then
		VanTieu_TeleportCartToPlayer()
	end

	return nResult
end
