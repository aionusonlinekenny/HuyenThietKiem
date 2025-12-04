----------------------------------------
--Script Trap qua maps - Modified for V?n Tiï¿½u support
--Code By Kinnox
----------------------------------------
Include("\\script\\maps\\trap\\libtrap.lua");
Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\Event\\VanTieu\\lib.lua");

function main()
	-- CRITICAL FIX: Check current map first to prevent spam
	-- Only process if player is actually on map 21 (source map)
	local nCurrentMapID = GetWorldPos()
	if nCurrentMapID ~= 21 then
		-- Player not on map 21, don't process trap
		return
	end


	-- Check if player is doing escort quest
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)  -- Cart type: 1=ï¿½?ng, 2=B?c, 3=Vï¿½ng

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

	-- Teleport player back to D??ng Chï¿½u (map 11)
	if (NewWorld(11, 2845,4811) == 1) then
		-- Teleport successful
		SetFightState(1)
		AddSkillState(963, 1, 0, 18*3)

		-- Delete cart from OLD map (map 21) after successful teleport
		if nNpcIdx > 0 then
			DelNpc(nNpcIdx)
			--print("V?n Tiï¿½u: Deleted old cart at map 21")
		end

		-- If player had a cart, spawn it at new location
		if nCartType > 0 and nCartTemplateID > 0 then
			-- Get destination subworld index
			local nSubWorldIdx = SubWorldID2Idx(11)

			if nSubWorldIdx >= 0 then
				-- Spawn cart near player at new location
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

					Msg2Player("Tiªu xa ®· theo b¹n tíi Thµnh §«")
				else
					Msg2Player("Lçi: Kh«ng thÓ t¹o tiªu xa ë Thµnh §«!")
				end
			end
		end
	end
	-- if (nSubWorld <= 0) then -- Show Msg Trap
	-- 	Talk(1,"",LIB_STRING[1]);
	-- 	return
	-- end;
	
	-- if (nLevel < NOTLEVEL) then -- Show Msg Level
	-- 	Talk(1,"",LIB_STRING[2]);
	-- 	return
	-- end;
		
	-- if (nState < 1) then -- Check Pos
	-- 	SetPos(3006,4944);
	-- else
	-- 	SetPos(3017,4955);
	-- end;
	-- CheckFightState(1);	-- SetFightState
	-- AddSkillTrap(); -- Add Anti Damage Magic 3s;
end;