----------------------------------------
--Script Trap qua maps
--Code By Kinnox
--Modified: Support Vận Tiêu (escort cart transport)
----------------------------------------
Include("\\script\\maps\\libtrap.lua");
Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\Event\\VanTieu\\lib.lua");

function main()
	print("TRAP 1 (Map 21) - Quay về Dương Châu (Map 11)");

	-- Check if player is doing escort quest
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)  -- Cart type: 1=Đồng, 2=Bạc, 3=Vàng

	local nCartType = 0
	local nCartTemplateID = 0

	-- If player has active escort quest (task value 1-3)
	if nTask > 0 and nTask < 4 then
		-- Get cart NPC ID
		local dwCartID = GetTask(TASK_NPCVANTIEU)

		if dwCartID > 0 then
			-- Find cart NPC
			local nNpcIdx = FindAroundNpc(dwCartID)
			if nNpcIdx <= 0 then
				nNpcIdx = FindNearNpc(dwCartID)
			end

			-- If cart found, prepare to transport it
			if nNpcIdx > 0 then
				-- Save cart info
				nCartType = nTask
				nCartTemplateID = TIEUXA_TEMPLET[nTask][1]

				-- Delete old cart
				DelNpc(nNpcIdx)
				print("Vận Tiêu: Deleted old cart at map 21")
			end
		end
	end

	-- Teleport player back to Dương Châu (map 11)
	if (NewWorld(11, 2845, 4811) == 1) then
		SetFightState(1)
		AddSkillState(963, 1, 0, 18*3)

		-- If player had a cart, spawn it at new location
		if nCartType > 0 and nCartTemplateID > 0 then
			-- Get destination subworld index
			local nSubWorldIdx = SubWorldID2Idx(11)

			if nSubWorldIdx >= 0 then
				-- Spawn cart near player at new location
				-- Player is at 2845, 4811
				local nCartX = 2845 * 32 + 64  -- +2 tiles offset
				local nCartY = 4811 * 32

				local nId = AddNpc(
					nCartTemplateID,
					1,
					nSubWorldIdx,
					nCartX,
					nCartY,
					1,
					"",
					0,
					0
				)

				if nId > 0 then
					-- Setup cart script and properties
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

					-- Set HP
					if SetNpcLifeMax then
						SetNpcLifeMax(nId, 10000)
						SetNpcLife(nId, 10000)
					end

					-- Setup owner and follow
					if SetNpcOwner ~= nil then
						SetNpcOwner(nId, 1)
					end

					-- Set name
					local nName = GetName()
					local sCartName = nName .. " - " .. TIEUXA_TEMPLET[nCartType][2]
					SetNpcName(nId, sCartName)

					-- Set timeout
					if SetNpcParam then
						SetNpcParam(nId, 3, CART_TIMEOUT)
					end

					-- Store owner UUID
					local nUUID = GetUUID()
					if SetNpcParam then
						SetNpcParam(nId, 1, nUUID)
					end

					-- Update task with new NPC ID
					local dwNewCartID = GetNpcID(2, nId)
					SetTask(TASK_NPCVANTIEU, dwNewCartID)

					Msg2Player("Tiêu xa đã theo bạn về Dương Châu!")
					print("Vận Tiêu: Spawned new cart at map 11, NPC ID: " .. dwNewCartID)
				else
					Msg2Player("Lỗi: Không thể tạo tiêu xa ở Dương Châu!")
					print("ERROR: Failed to spawn cart at map 11")
				end
			end
		end
	end
end;