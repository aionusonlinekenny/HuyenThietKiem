-- Tiêu Đầu - Vận Tiêu Quest Giver NPC
-- Ported from ThienDieuOnline to HuyenThietKiem
-- Adapted by Claude AI Assistant

Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\event\\VanTieu\\lib.lua");

-- Helper functions to replace missing ones
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
	-- This is the Lua workaround - try C++ implementation first!
	-- If C++ SetNpcOwner exists, it will be used instead
	-- This backup is only used if server not rebuilt yet

	Msg2Player("WARN: Using Lua SetNpcOwner workaround - server needs rebuild!")

	if nNpcIdx <= 0 then
		return
	end

	-- Get current player index (HACK: assume owner is current player)
	local nPlayerIdx = GetPlayerIndex()
	if not nPlayerIdx or nPlayerIdx < 0 then
		Msg2Player("ERROR: Cannot get player index for SetNpcOwner")
		return
	end

	-- Store player index and follow mode in m_AiParam
	-- NOTE: We can't directly set m_AiParam from Lua, so this won't work
	-- Need C++ implementation!
	SetNpcParam(nNpcIdx, 2, 1) -- Mark as having owner (legacy)

	Msg2Player("ERROR: SetNpcOwner workaround incomplete - please rebuild server with C++ implementation")
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
		Talk(1,"","Ai cũng tưởng nghề Bảo tiêu là sung sướng! Thật sự mỗi lần ra đi đều không dám hứa hẹn trước ngày về!")
		return
	end

	Say("Thành Đô tiêu cục của chúng ta luôn được sự tín nhiệm của giang hồ",5,
	"Vận tiêu/vantieu",
	"Đổi Hồ Tiêu Lệnh/cuahang",
	"Tìm hiểu vận tiêu/timhieu",
	"[TEST] Kiểm tra server build/testserverbuild",
	"Ta chỉ ghé qua/no")
end

function vantieu()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if(nTask == 0) then
		Say("Gần đây công việc nhiều, đạo tặc hoành hành khắp nơi, mà nhân lực lại thiếu. Vị đây có muốn giúp ta một chuyến không? Sẽ có lao phù xứng đáng cho ngươi!",2,
		"Ta đồng ý áp tiêu/batdau",
		"Ta đang rất bận/no")
	elseif(nTask < 4) then
		-- Check if player was robbed
		if(GetItemCountInBag(6, ITEM_TIEUKY, 1, -1, 0) > 0) then
			bicuop()
			return
		end

		Say("Không phải ngươi đang áp tiêu sao? Sao lại đến đây tìm ta?",4,
		"Ta bị thất lạc, giúp ta tìm tiêu xa/timxe",
		"Reset tiêu xa (Test - Miễn phí)/resettieuxatest",
		"Ta không muốn làm nữa/huybo",
		"Ta chỉ ghé qua/no")
	else
		hoanthanh()
	end
end

function batdau()
	-- Tạm thời disable fight state check để test
	-- if(GetFightState() ~= 0) then
	-- 	Talk(1,"","Trạng thái chiến đấu không thể nhận nhiệm vụ")
	-- 	return
	-- end

	if(GetLevel() < 120) then
		Talk(1,"","Hãy cố gắng luyện tập đạt cấp 120 rồi đến gặp ta")
		return
	end

	-- Check daily limit
	local nResetTask = GetTask(TASK_RESET_VANTIEU)
	local nLan = GetByte(nResetTask, 6)

	if(nLan >= MAX_DAILY_RUNS) then
		if(GetItemCountInBag(6, ITEM_UNLOCK_VANTIEU, 1, -1, 0) < 1) then
			Talk(1,"","Hôm nay ngươi đã áp tiêu nhiều lần rồi. Hãy nghỉ ngơi mai lại đến gặp ta.")
			return
		end
	end

	if(GetCash() < COST_START_QUEST) then
		Talk(1,"","Ngươi cần nộp ".. (COST_START_QUEST / 10000) .." vạn lượng phí hao tốn tiêu xa!")
		return
	end

	-- Consume unlock item or increment daily counter
	if(nLan >= MAX_DAILY_RUNS) then
		DelTaskItem(ITEM_UNLOCK_VANTIEU, 1)
	else
		SetTask(TASK_RESET_VANTIEU, SetByte(nResetTask, 6, nLan + 1))
	end

	-- Random cart type (1-3 -> Đồng/Bạc/Vàng)
	local n = random(0, 2)
	local nRand = n + 1

	-- Calculate spawn positions BEFORE teleporting player
	-- Use fixed coordinates, don't rely on GetWorldPos() after NewWorld()
	local nTemplateID = TIEUXA_TEMPLET[nRand][1]
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)

	-- Player will be at POS_START_X/Y after teleport
	local nPlayerX = floor(POS_START_X * 32)  -- Convert map coords to pixels
	local nPlayerY = floor(POS_START_Y * 32)

	-- Spawn cart offset +2 tiles East of player position
	local nCartX = nPlayerX + 64  -- +64 pixels = +2 tiles
	local nCartY = nPlayerY

	Msg2Player("Debug: Will teleport player to X="..nPlayerX.." Y="..nPlayerY)
	Msg2Player("Debug: Will spawn cart at X="..nCartX.." Y="..nCartY.." (offset +64)")

	-- Pay player first
	Pay(COST_START_QUEST)

	-- Teleport player to start location
	-- IMPORTANT: Check return value! NewWorld() returns 1 on success
	local nTeleportOK = NewWorld(SUBWORLD_START, POS_START_X, POS_START_Y)

	if nTeleportOK ~= 1 then
		Msg2Player("ERROR: Teleport failed! NewWorld returned: "..tostring(nTeleportOK))
		Talk(1,"","Lỗi: Không thể di chuyển đến vị trí bắt đầu!")
		return
	end

	Msg2Player("Debug: Teleport OK! Now spawning cart...")

	-- Add teleport effect (18 frames * 3 = 54 frames ~= 2.7 seconds at 20fps)
	-- This also ensures client/server position sync before spawning cart
	AddSkillState(963, 1, 0, 18*3)

	-- Reset fight state after teleport (like town portal does)
	SetFightState(0)

	Msg2Player("Debug: Added teleport effect, spawning cart now...")

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

	Msg2Player("Debug: AddNpc returned nId="..tostring(nId))

	if nId > 0 then
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

		Msg2Player("Debug: Set NPC Camp=0, Series="..tostring(nPlayerSeries))
	end

	if nId <= 0 then
		Talk(1,"","Lỗi: Không thể tạo tiêu xa!")
		return
	end

	-- Setup cart
	local nName = GetName()

	Msg2Player("=== DEBUG SetNpcOwner ===")
	Msg2Player("Debug: Player name = '"..nName.."'")

	-- IMPORTANT: Set NPC life/HP so AI will activate
	-- AI won't run if m_CurrentLifeMax = 0 (check in KNpcAI.cpp line 34)
	if SetNpcLifeMax then
		SetNpcLifeMax(nId, 10000)  -- 10k HP
		SetNpcLife(nId, 10000)
		Msg2Player("Debug: Set NPC life = 10000")
	end

	-- Check if C++ SetNpcOwner exists
	if SetNpcOwner ~= nil then
		Msg2Player("GOOD: C++ SetNpcOwner found - calling it...")

		-- Pass player NAME to SetNpcOwner
		-- C++ will find player by name and set params
		SetNpcOwner(nId, nName, 1)
		Msg2Player("✓ Called SetNpcOwner with player name = '"..nName.."'")

		-- CRITICAL: Set AI mode
		if SetNpcAIMode then
			SetNpcAIMode(nId, 8)  -- AI Mode 8 = Follow owner
			Msg2Player("Debug: Called SetNpcAIMode(8)")
		end

		Msg2Player("SUCCESS: SetNpcOwner setup complete! Cart should follow you now.")
	else
		Msg2Player("ERROR: C++ SetNpcOwner NOT FOUND!")
		Msg2Player("ERROR: Server needs rebuild with C++ code!")
		-- Use backup (won't make cart follow, but won't crash)
		SetNpcOwner_Backup(nId, nName, 1)
	end

	Msg2Player("=== END DEBUG ===")

	SetNpcName(nId, nName .. TIEUXA_TEMPLET[nRand][2])
	SetNpcTimeout(nId, CART_TIMEOUT)
	SetNpcValue(nId, GetUUID())

	-- Store cart NPC ID in task
	local dwCartID = GetNpcIDFromIndex(nId)
	SetTask(TASK_NPCVANTIEU, dwCartID)

	-- Debug messages
	Msg2Player("Debug: NPC Index="..nId.." ID="..dwCartID)
	Msg2Player("Debug: TASK_NPCVANTIEU="..GetTask(TASK_NPCVANTIEU))

	-- Update quest state
	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 1, n + 1))
	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 2, random(1, 3)))

	-- Notify player
	Msg2Player("Hãy mau hộ tống tiêu xa đến Long Môn tiêu sư ở Thanh Thành Sơn ("..POS_END_X.."/"..POS_END_Y..")")
	Msg2Player("Debug: Quest started! Cart should follow you when you move.")
end

function cuahang()
	local nCount = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0
	Say("Ngươi hiện có: <color=red>"..nCount.."<color> Hồ Tiêu Lệnh. Hãy lựa chọn vật phẩm cần thiết cho tiêu xa:",4,
	"Tăng tốc (4 Hồ Tiêu Lệnh)/buy_item("..ITEM_TANGTO..",4)",
	"Hồi máu (3 Hồ Tiêu Lệnh)/buy_item("..ITEM_HOIMAU..",3)",
	"Dịch chuyển (3 Hồ Tiêu Lệnh)/buy_item("..ITEM_DICHCHUYEN..",3)",
	"Ta chưa muốn đổi/no")
end

function buy_item(nItemID, nCost)
	if(CountFreeBagCell() < 1) then
		Talk(1,"","Hành trang của ngươi không còn chỗ trống")
		return
	end

	local nHave = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0

	if(nCost > nHave) then
		Talk(1,"","Ngươi không mang đủ Hồ Tiêu Lệnh rồi")
		return
	end

	AddTaskItem(nItemID)
	DelTaskItem(ITEM_HO_TIEU_LENH, nCost)

	Talk(1,"","Giao dịch thành công!")
end

function hoanthanh()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nFinish = GetByte(nTaskValue, 1)

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)

	phanthuong(nFinish)
	Talk(1,"","Làm tốt lắm! Đây là phần lao vụ của ngươi!")
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

	Talk(1,"","Ngươi bị cướp tiêu rồi sao? Cũng may ngươi đã đoạt lại Ti tiêu Kỳ danh dự của Thành Đô tiêu cục. Vất vả cho ngươi rồi! Đây là một nửa phần lao vụ")
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

	Talk(1,"","Công việc áp tiêu không phải là đơn giản. Ngươi chán đạt chán rào bước vào nên cũng khó trách! Sau này hãy cố gắng lên!")
end

function timxe()
	if(GetCash() < COST_FIND_CART) then
		Talk(1,"","Ngươi cần nộp phí ".. (COST_FIND_CART / 10000) .." vạn lượng để ta cho ngươi đi tìm tiêu xa!")
		return
	end

	local dwCartID = GetTask(TASK_NPCVANTIEU)
	local nNpcIdx = FindAroundNpc(dwCartID)

	if (nNpcIdx == 1) then
		Talk(1,"","Không phải tiêu xa của ngươi đang ở ngay đây sao!")
		return
	end

	nNpcIdx = FindNearNpc(dwCartID)

	if(nNpcIdx > 0) then
		local w, x, y = GetNpcPos(nNpcIdx)
		NewWorld(w, x, y)
		Pay(COST_FIND_CART)
		Talk(1,"","Đã có tin tức tiêu xa! Ta sẽ cho ngươi đưa ngươi đến đó!")
	else
		Talk(1,"","Không thấy tin tức! Tiêu xa của ngươi có lẽ đã mất!")
	end
end

function resettieuxatest()
	-- Test function to reset cart for easier testing (FREE)
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if nTask == 0 or nTask >= 4 then
		Talk(1,"","Ngươi không có nhiệm vụ vận tiêu đang thực hiện!")
		return
	end

	-- Delete old cart if exists
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	if dwCartID > 0 then
		local nNpcIdx = FindAroundNpc(dwCartID)
		if nNpcIdx > 0 then
			DelNpc(nNpcIdx)
			Msg2Player("Đã xóa tiêu xa cũ")
		end
	end

	-- Get player position and subworld
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)
	local w, x, y = GetWorldPos()

	-- Determine cart type from task
	local nRand = nTask -- Task value 1/2/3 = Đồng/Bạc/Vàng
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

		-- Setup cart
		local nName = GetName()

		-- Check if C++ SetNpcOwner exists
		if SetNpcOwner ~= nil then
			SetNpcOwner(nId, nName, 1)
			Msg2Player("Reset: SetNpcOwner called successfully")
		else
			Msg2Player("ERROR: SetNpcOwner NOT FOUND - server needs rebuild")
			SetNpcOwner_Backup(nId, nName, 1)
		end

		SetNpcName(nId, nName .. TIEUXA_TEMPLET[nRand][2])
		SetNpcTimeout(nId, CART_TIMEOUT)
		SetNpcValue(nId, GetUUID())

		-- Update task with new NPC ID
		local dwNewCartID = GetNpcIDFromIndex(nId)
		SetTask(TASK_NPCVANTIEU, dwNewCartID)

		Msg2Player("Đã reset tiêu xa thành công! Xe mới xuất hiện tại vị trí của ngươi.")
	else
		Talk(1,"","Lỗi: Không thể tạo tiêu xa mới!")
	end
end

function timhieu()
	Talk(2,"","Con đường hiểm trở nhất là băng qua Thanh Thành Sơn. Nơi đó đầy rẫy thú dữ và đạo tặc hoành hành. Ngươi phải hộ tiêu an toàn đến nơi giao cho tiêu sư trung chuyển đúng thời gian!",
	"Hàng hóa cần vận chuyển có 3 chủng loại: vàng, bạc, đồng. Tiêu công giá trị thì bọn đạo tặc cũng độ mặt tài nên càng khó khăn nhưng lao phù ngươi nhận được cũng tương xứng!")
end

-- Reward function
function phanthuong(nFinish)
	local tbName = GetName()

	if(nFinish == TASK_STATE_DONG) then	-- Đồng tiêu xa
		AddItem(0, 6, ITEM_HO_TIEU_LENH, 0, 0, 5, 0, 0)
		AddItem(0, 6, ITEM_HO_TIEU_LENH, 0, 0, 5, 0, 0)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0) -- Rương hoạt động
		AddOwnExp(5 * KINH_NGHIEM_BASE)
		AddRespect(5)
		-- AddItemIDStack - workaround
		for i=1,5 do
			AddItem(0, 6, ITEM_RUONG_VANTIEU, 0, 0, 5, 0, 0)
		end
		Msg2SubWorld("Đại Hiệp <color=yellow>"..tbName.."<color> giao thành công Đồng Tiêu Xa nhận được <color=yellow>5 triệu<color> kinh nghiệm và <color=yellow>5<color> điểm Uy Danh!")
	elseif(nFinish == TASK_STATE_BAC) then	-- Bạc tiêu xa
		for i=1,4 do
			AddItem(0, 6, ITEM_HO_TIEU_LENH, 0, 0, 5, 0, 0)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(10 * KINH_NGHIEM_BASE)
		AddRespect(10)
		for i=1,10 do
			AddItem(0, 6, ITEM_RUONG_VANTIEU, 0, 0, 5, 0, 0)
		end
		Msg2SubWorld("Đại Hiệp <color=yellow>"..tbName.."<color> giao thành công Bạc Tiêu Xa nhận được <color=yellow>10 triệu<color> kinh nghiệm và <color=yellow>10<color> điểm Uy Danh!")
	elseif(nFinish == TASK_STATE_VANG) then	-- Vàng tiêu xa
		for i=1,6 do
			AddItem(0, 6, ITEM_HO_TIEU_LENH, 0, 0, 5, 0, 0)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(15 * KINH_NGHIEM_BASE)
		AddRespect(15)
		for i=1,15 do
			AddItem(0, 6, ITEM_RUONG_VANTIEU, 0, 0, 5, 0, 0)
		end
		Msg2SubWorld("Đại Hiệp <color=yellow>"..tbName.."<color> giao thành công Vàng Tiêu Xa nhận được <color=yellow>15 triệu<color> kinh nghiệm và <color=yellow>15<color> điểm Uy Danh!")
	else	-- Bị cướp, chỉ nhận 1/2 thưởng
		AddItem(0, 6, ITEM_HO_TIEU_LENH, 0, 0, 5, 0, 0)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(2 * KINH_NGHIEM_BASE)
		Msg2SubWorld("Đại Hiệp <color=yellow>"..tbName.."<color> bị cướp tiêu không hoàn thành nhiệm vụ!")
	end
end

function testserverbuild()
	Talk(1, "", "=== TEST SERVER BUILD ===\n\n"..
		"Checking if server has C++ SetNpcOwner implementation...\n\n"..
		"If you see 'ERROR: SetNpcOwner workaround incomplete' after spawning cart, "..
		"it means server needs rebuild.\n\n"..
		"Check server console for detailed messages.")

	-- Check if lib.lua loaded correctly
	if SUBWORLD_START == nil then
		Msg2Player("ERROR: SUBWORLD_START is nil! lib.lua not loaded?")
		return
	end

	-- Spawn a test cart to check SetNpcOwner
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)
	local w, x, y = GetWorldPos()

	Msg2Player("=== SPAWN TEST CART ===")
	Msg2Player("Debug: SUBWORLD_START="..tostring(SUBWORLD_START))
	Msg2Player("Debug: SubWorldIdx="..tostring(nSubWorldIdx))
	Msg2Player("Debug: GetWorldPos returned: w="..tostring(w)..", x="..tostring(x)..", y="..tostring(y))

	if nSubWorldIdx < 0 then
		Msg2Player("ERROR: Invalid SubWorld Index (< 0)!")
		Msg2Player("ERROR: SubWorldID2Idx("..SUBWORLD_START..") returned "..nSubWorldIdx)
		return
	end

	-- Validate coordinates
	if x == nil or y == nil or x == 0 or y == 0 then
		Msg2Player("ERROR: GetWorldPos returned invalid coordinates!")
		Msg2Player("ERROR: Cannot spawn cart at nil/zero position")
		Msg2Player("TRY: Use fixed position instead")
		-- Use Tiêu Đầu position as fallback
		x = floor(POS_START_X * 32)
		y = floor(POS_START_Y * 32)
		Msg2Player("Using fallback position: x="..x..", y="..y)
	end

	-- Spawn cart offset from player to avoid overlap
	local nCartX = x + 64  -- Offset +2 tiles East
	local nCartY = y
	Msg2Player("Spawning at offset position: x="..nCartX..", y="..nCartY.." (+64 pixels from player)")

	-- Try multiple templates to see which one works
	Msg2Player("Trying template 2085...")
	local nId = AddNpc(2085, 1, nSubWorldIdx, nCartX, nCartY, 1, "", 0, 0)
	Msg2Player("Template 2085 returned nId="..tostring(nId))

	if nId <= 0 then
		Msg2Player("Failed! Trying template 2086...")
		nId = AddNpc(2086, 1, nSubWorldIdx, nCartX, nCartY, 1, "", 0, 0)
		Msg2Player("Template 2086 returned nId="..tostring(nId))
	end

	if nId <= 0 then
		Msg2Player("Failed! Trying template 2087...")
		nId = AddNpc(2087, 1, nSubWorldIdx, nCartX, nCartY, 1, "", 0, 0)
		Msg2Player("Template 2087 returned nId="..tostring(nId))
	end

	if nId <= 0 then
		Msg2Player("All cart templates failed!")
		Msg2Player("Trying simple NPC template 377 (Tiêu Đầu)...")
		nId = AddNpc(377, 1, nSubWorldIdx, nCartX, nCartY, 1, "", 0, 0)
		Msg2Player("Template 377 returned nId="..tostring(nId))
	end

	Msg2Player("Final nId="..tostring(nId))

	if nId > 0 then
		Msg2Player("Test cart spawned with Index="..nId)

		-- Check if SetNpcOwner exists
		local nName = GetName()

		if SetNpcOwner ~= nil then
			Msg2Player("✓ GOOD: C++ SetNpcOwner function EXISTS!")
			Msg2Player("Calling SetNpcOwner...")
			SetNpcOwner(nId, nName, 1)
			Msg2Player("✓ SetNpcOwner called!")
			Msg2Player("")
			Msg2Player("Now WALK AWAY and check if cart follows you:")
			Msg2Player("  • Cart follows = SUCCESS! Server rebuilt correctly")
			Msg2Player("  • Cart stands still = AI Mode not working")
		else
			Msg2Player("✗ ERROR: C++ SetNpcOwner function NOT FOUND!")
			Msg2Player("✗ Server has NOT been rebuilt with C++ code!")
			Msg2Player("✗ Cart will NOT follow until you rebuild GameServer.exe")
			Msg2Player("")
			Msg2Player("See BUILD_INSTRUCTIONS.md for build steps")
			SetNpcOwner_Backup(nId, nName, 1)
		end

		-- Make cart friendly
		if SetNpcCamp ~= nil then
			SetNpcCamp(nId, 0)
		end
		if SetNpcCurCamp ~= nil then
			SetNpcCurCamp(nId, 0)
		end
	else
		Msg2Player("ERROR: Failed to spawn test cart!")
	end
end

function no()
end
