-- Example Trap Script for Vận Tiêu Event
-- This trap teleports player AND cart NPC to another map
-- Place this in your map trap folder (e.g., /script/maps/trap/11/X.lua)

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\event\\VanTieu\\lib.lua")  -- IMPORTANT: Include VanTieu lib

function main()
	-- Example: Teleport to map Thanh Thành Sơn (map 20)
	-- Coordinates: 100, 100 (adjust to your actual coordinates)

	local nTargetMap = 20      -- SubWorld ID
	local nTargetX = 100       -- Map X coordinate (in tiles)
	local nTargetY = 100       -- Map Y coordinate (in tiles)

	-- Check level requirement (optional)
	local nLevel = GetLevel()
	if nLevel < 120 then
		Talk(1, "", "Bạn cần đạt cấp <color=yellow>120<color> để qua khu vực này!")
		return
	end

	-- Use VanTieu_NewWorldWithCart instead of NewWorld
	-- This will automatically teleport cart NPC if player has active quest
	local nResult = VanTieu_NewWorldWithCart(nTargetMap, nTargetX, nTargetY)

	if nResult == 1 then
		-- Teleport successful
		-- Cart NPC will be automatically teleported by VanTieu_NewWorldWithCart
		Msg2Player("Đã chuyển đến Thanh Thành Sơn!")
	else
		-- Teleport failed
		Talk(1, "", "Không thể di chuyển đến khu vực này!")
	end
end
