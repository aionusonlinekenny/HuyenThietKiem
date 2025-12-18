-- Tiêu Xa - Escort Cart NPC Script
-- Ported from ThienDieuOnline to HuyenThietKiem
-- Follow mechanism implemented in C++ (AI Mode 8)

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

function LastDamage(NpcIndex)
	-- When cart is killed (robbed)
	local dwPID = GetNpcParam(NpcIndex, 1) -- Owner UUID
	local w, x, y = GetNpcPos(NpcIndex)

	-- Spawn treasure chest
	local nChestId = AddNpc(
		NPC_RUONG_CUOP,		-- Ruong cu?p
		1,					-- Level
		w,					-- SubWorld
		x * 32 - 64,		-- X
		y * 32,				-- Y
		1,					-- Remove on death
		"Tàn l?c tiêu v?t"	-- Name
	)

	if nChestId > 0 then
		SetNpcScript(nChestId, "\\script\\Event\\VanTieu\\ruongcuop.lua")
		SetNpcParam(nChestId, 3, CHEST_TIMEOUT) -- Timeout
	end

	-- Spawn robbed cart (red cart)
	local nRobbedId = AddNpc(
		NPC_HONG_TIEUXA,	-- Tiêu Xa H?ng
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
	-- Cart initialization
	-- Follow mechanism handled by C++ AI Mode 8
end

function DeathSelf(NpcIndex)
	DelNpc(NpcIndex)
end
