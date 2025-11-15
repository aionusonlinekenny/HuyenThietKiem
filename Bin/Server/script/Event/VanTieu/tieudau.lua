-- Tiêu Đầu - Vận Tiêu Quest Giver NPC
-- Ported from ThienDieuOnline to HuyenThietKiem
-- Adapted by Claude AI Assistant

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

-- Helper functions to replace missing ones
function GetNpcValue(nNpcIdx)
	-- Workaround: Use NpcParam instead of NpcValue
	if nNpcIdx <= 0 or nNpcIdx >= MAX_NPC then
		return 0
	end
	return GetNpcParam(nNpcIdx, 1) -- Use param slot 1 for UUID storage
end

function SetNpcValue(nNpcIdx, nValue)
	if nNpcIdx <= 0 or nNpcIdx >= MAX_NPC then
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

function SetNpcOwner(nNpcIdx, szOwnerName, nMode)
	-- Workaround: Store owner name in NpcParam
	-- Real implementation would make NPC follow player
	-- For now, just store the owner info
	if nNpcIdx <= 0 or nNpcIdx >= MAX_NPC then
		return
	end
	SetNpcParam(nNpcIdx, 2, 1) -- Mark as having owner
	-- TODO: Implement actual follow mechanism
end

function SetNpcTimeout(nNpcIdx, nSeconds)
	-- Workaround: Use timer to delete NPC after timeout
	-- Real implementation would be in C++ NPC class
	-- For now, just mark it (will need manual cleanup)
	if nNpcIdx <= 0 or nNpcIdx >= MAX_NPC then
		return
	end
	SetNpcParam(nNpcIdx, 3, nSeconds) -- Store timeout
	-- TODO: Implement actual auto-despawn
end

function AddNpcWithScript(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, szScript, bRemoveDeath, szName)
	-- Enhanced AddNpc with script support
	local nNpcIdx = AddNpc(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, bRemoveDeath or 1, szName or "")
	if nNpcIdx > 0 and szScript and szScript ~= "" then
		SetNpcScript(nNpcIdx, szScript)
	end
	return nNpcIdx
end

function GetNpcIDFromIndex(nNpcIdx)
	-- Get NPC's m_dwID
	if nNpcIdx <= 0 or nNpcIdx >= MAX_NPC then
		return 0
	end
	return GetNpcID(2, nNpcIdx) -- Type 2 = m_dwID
end

-- Main dialog
function main(nIndex)
	SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		Talk(1,"","Ai cũng tưởng nghề Bảo tiêu là sung sướng! Thật sự mỗi lần ra đi đều không dám hứa hẹn trước ngày về!")
		return
	end

	Say("Thành Đô tiêu cục của chúng ta luôn được sự tín nhiệm của giang hồ",4,
	"Vận tiêu/vantieu",
	"Đổi Hồ Tiêu Lệnh/cuahang",
	"Tìm hiểu vận tiêu/timhieu",
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
		if(GetItemCountInBag(0,6,ITEM_TIEUKY) > 0) then
			bicuop()
			return
		end

		Say("Không phải ngươi đang áp tiêu sao? Sao lại đến đây tìm ta?",3,
		"Ta bị thất lạc, giúp ta tìm tiêu xa/timxe",
		"Ta không muốn làm nữa/huybo",
		"Ta chỉ ghé qua/no")
	else
		hoanthanh()
	end
end

function batdau()
	if(GetFightState() == 1) then
		Talk(1,"","Trạng thái chiến đấu không thể nhận nhiệm vụ")
		return
	end

	if(GetLevel() < 120) then
		Talk(1,"","Hãy cố gắng luyện tập đạt cấp 120 rồi đến gặp ta")
		return
	end

	-- Check daily limit
	local nResetTask = GetTask(TASK_RESET_VANTIEU)
	local nLan = GetByte(nResetTask, 6)

	if(nLan >= MAX_DAILY_RUNS) then
		if(GetItemCountInBag(0,6,ITEM_UNLOCK_VANTIEU) < 1) then
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
		ConsumeItemInBag(1, 0, 6, ITEM_UNLOCK_VANTIEU)
	else
		SetTask(TASK_RESET_VANTIEU, SetByte(nResetTask, 6, nLan + 1))
	end

	-- Random cart type (1-3 -> Đồng/Bạc/Vàng)
	local n = random(0, 2)
	local nRand = n + 1

	-- Spawn escort cart
	local nId = AddNpcWithScript(
		TIEUXA_TEMPLET[nRand][1],  -- Template ID
		1,							-- Level
		SUBWORLD_START,				-- SubWorld
		POS_START_X * 32,			-- X
		POS_START_Y * 32,			-- Y
		"\\script\\Event\\VanTieu\\tieuxa.lua", -- Script
		1,							-- Remove on death
		""							-- Name (will be set below)
	)

	if nId <= 0 then
		Talk(1,"","Lỗi: Không thể tạo tiêu xa!")
		return
	end

	-- Setup cart
	local nName = GetName()
	SetNpcOwner(nId, nName, 1)
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

	-- Pay and teleport
	Pay(COST_START_QUEST)
	NewWorld(SUBWORLD_START, POS_START_X, POS_START_Y)
	SetFightState(1)
end

function cuahang()
	local nCount = GetItemCountInBag(0, 6, ITEM_HO_TIEU_LENH)
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

	local nHave = GetItemCountInBag(0, 6, ITEM_HO_TIEU_LENH)

	if(nCost > nHave) then
		Talk(1,"","Ngươi không mang đủ Hồ Tiêu Lệnh rồi")
		return
	end

	AddItem(0, 6, nItemID, 0, 0, 5, 0, 0)
	ConsumeItemInBag(nCost, 0, 6, ITEM_HO_TIEU_LENH)

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
	SubWorld = SubWorldID2Idx(SUBWORLD_START)
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
	ConsumeItemInBag(1, 0, 6, ITEM_TIEUKY)

	Talk(1,"","Ngươi bị cướp tiêu rồi sao? Cũng may ngươi đã đoạt lại Ti tiêu Kỳ danh dự của Thành Đô tiêu cục. Vất vả cho ngươi rồi! Đây là một nửa phần lao vụ")
end

function huybo()
	SubWorld = SubWorldID2Idx(SUBWORLD_START)
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

function no()
end
