-- Tiêu Xa - Escort Cart NPC Script
-- Ported from ThienDieuOnline to HuyenThietKiem

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

-- Global timer for NPC follow mechanism
local FOLLOW_TIMER_ID = 1
local FOLLOW_INTERVAL = 18 -- Frames (18 frames ~ 1 second)
local FOLLOW_DISTANCE = 10 -- Tiles - distance to maintain from player

function OnTimer(NpcIndex, nTimerID)
	if nTimerID == FOLLOW_TIMER_ID then
		FollowOwner(NpcIndex)
	end
end

function FollowOwner(NpcIndex)
	-- Get owner UUID from NpcParam
	local dwOwnerUUID = GetNpcParam(NpcIndex, 1)
	if not dwOwnerUUID or dwOwnerUUID == 0 then
		return
	end

	-- Find owner player
	local nPlayerIdx = GetPlayer(dwOwnerUUID)
	if not nPlayerIdx or nPlayerIdx <= 0 then
		return
	end

	-- Get player position
	PlayerIndex = nPlayerIdx
	local pWorld, pX, pY = GetWorldPos()

	-- Get NPC position
	local nWorld, nX, nY = GetNpcPos(NpcIndex)

	-- Check if on same map
	if nWorld ~= pWorld then
		return
	end

	-- Calculate distance (in pixels)
	local distX = pX - (nX * 32)
	local distY = pY - (nY * 32)
	local distPixels = sqrt(distX * distX + distY * distY)

	-- If too far, move closer
	if distPixels > FOLLOW_DISTANCE * 32 then
		-- Convert player pixel position to tiles
		local pTileX = floor(pX / 32)
		local pTileY = floor(pY / 32)

		-- Move NPC towards player
		SetPos(pTileX, pTileY, NpcIndex)
	end
end

function LastDamage(NpcIndex)
	-- When cart is killed (robbed)
	local dwPID = GetNpcParam(NpcIndex, 1) -- Owner UUID
	local w, x, y = GetNpcPos(NpcIndex)

	-- Spawn treasure chest
	local nChestId = AddNpc(
		NPC_RUONG_CUOP,		-- Rương cướp
		1,					-- Level
		w,					-- SubWorld
		x * 32 - 64,		-- X
		y * 32,				-- Y
		1,					-- Remove on death
		"Tàn lộc tiêu vật"	-- Name
	)

	if nChestId > 0 then
		SetNpcScript(nChestId, "\\script\\Event\\VanTieu\\ruongcuop.lua")
		SetNpcParam(nChestId, 3, CHEST_TIMEOUT) -- Timeout
	end

	-- Spawn robbed cart (red cart)
	local nRobbedId = AddNpc(
		NPC_HONG_TIEUXA,	-- Tiêu Xa Hồng
		1,					-- Level
		w,					-- SubWorld
		x * 32,				-- X
		y * 32,				-- Y
		1,					-- Remove on death
		""					-- Name
	)

	if nRobbedId > 0 then
		SetNpcScript(nRobbedId, "\\script\\Event\\VanTieu\\tieuxahong.lua")
		SetNpcParam(nRobbedId, 1, dwPID) -- Store owner UUID
		SetNpcParam(nRobbedId, 3, ROBBED_CART_TIMEOUT) -- Timeout

		-- Update player's task to robbed state
		local nPlayerIdx = GetPlayer(dwPID)
		if nPlayerIdx > 0 then
			PlayerIndex = nPlayerIdx
			local dwRobbedCartID = GetNpcID(2, nRobbedId)
			SetTask(TASK_NPCVANTIEU, dwRobbedCartID)
		end
	end
end

function Timeout(nIndex)
	DelNpc(nIndex)
end

function Revive(NpcIndex)
	-- Cart initialization - start follow timer
	if SetNpcTimer ~= nil then
		SetNpcTimer(NpcIndex, FOLLOW_INTERVAL, FOLLOW_TIMER_ID)
	end
end

function DeathSelf(NpcIndex)
	DelNpc(NpcIndex)
end
