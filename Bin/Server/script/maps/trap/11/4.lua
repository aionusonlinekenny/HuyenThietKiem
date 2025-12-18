--Modified: Support V?n Ti?u (escort cart transport)
----------------------------------------
Include("\\script\\maps\\libtrap.lua");
Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\Event\\VanTieu\\lib.lua");

function main()
	-- CRITICAL FIX: Check current map first to prevent spam
	-- Only process if player is actually on map 11 (source map)
	local nCurrentMapID = GetWorldPos()
	if nCurrentMapID ~= 11 then
		-- Player not on map 11, don't process trap
		return
	end

	--print("TRAP 4 - Qua Thanh Th?nh Son (Map 21)");

	-- Check if player is doing escort quest
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)  -- Cart type: 1=??ng, 2=B?c, 3=V?ng

	local nCartType = 0
	local nCartTemplateID = 0
	local nNpcIdx = 0

	-- If player has active escort quest (task value 1-3)
	if nTask > 0 and nTask < 4 then
		-- Get cart NPC ID
		local dwCartID = GetTask(TASK_NPCVANTIEU)

		if dwCartID > 0 then
			-- Find cart NPC (only once before teleport)
			nNpcIdx = FindAroundNpc(dwCartID)
			if nNpcIdx <= 0 then
				nNpcIdx = FindNearNpc(dwCartID)
			end

			-- If cart found, save info for spawning later
			if nNpcIdx > 0 then
				nCartType = nTask
				nCartTemplateID = TIEUXA_TEMPLET[nTask][1]
			end
		end
	end

	-- Teleport player to Thanh Th?nh Son (map 21)
	if (NewWorld(21, 2613,4490) == 1) then
		-- Teleport successful
		SetFightState(1)
		AddSkillState(963, 1, 0, 18*3)

		-- Delete cart from OLD map (map 11) after successful teleport
		if nNpcIdx > 0 then
			DelNpc(nNpcIdx)
			--print("V?n Ti?u: Deleted old cart at map 11")
		end

		-- If player had a cart, spawn it at new location
		if nCartType > 0 and nCartTemplateID > 0 then
			-- Get destination subworld index
			local nSubWorldIdx = SubWorldID2Idx(21)
 
			if nSubWorldIdx >= 0 then
				-- Spawn cart near player at new location
				-- Player is at 2625, 4507
				local nCartX = 2613 * 32 + 64  -- +2 tiles offset
				local nCartY = 4490 * 32
 
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
 
					Msg2Player("Tiªu xa ®· theo b¹n qua Thanh Thµnh S¬n!")
					--print("V?n Ti?u: Spawned new cart at map 21, NPC ID: " .. dwNewCartID)
				else
					Msg2Player("Lçi: Kh«ng thÓ t¹o tiªu xa ë Thanh Thµnh S¬n!")
					--print("ERROR: Failed to spawn cart at map 21")
				end
			end
		end
	end
end;