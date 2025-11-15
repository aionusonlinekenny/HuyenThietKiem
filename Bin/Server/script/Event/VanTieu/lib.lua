-- Vận Tiêu (Escort) Event Library
-- Ported from ThienDieuOnline
-- Author: Claude AI Assistant
-- Date: 2025-11-15

Include("\\script\\lib\\TaskLib.lua")

-- NPC Template IDs cho tiêu xa
TIEUXA_TEMPLET = {
	{2084, " - Tiêu Xa [Đồng] "},
	{2085, " - Tiêu Xa [Bạc] "},
	{2086, " - Tiêu Xa [Vàng] "},
}

-- Tỷ lệ spawn các loại tiêu xa (hiện tại chưa dùng)
TIEUXA_RATE = {
	{1,2,3},
	{20,20,10},
}

-- Item IDs (cần tạo trong item database)
ITEM_TIEUKY = 4771			-- Tiêu Kỳ (dropped when robbed)
ITEM_UNLOCK_VANTIEU = 4772	-- Vé mở khóa vận tiêu thêm
ITEM_HO_TIEU_LENH = 4774	-- Hồ Tiêu Lệnh (currency/reward)
ITEM_TANGTO = 4775			-- Tăng tốc
ITEM_HOIMAU = 4776			-- Hồi máu
ITEM_DICHCHUYEN = 4778		-- Dịch chuyển
ITEM_RUONG_VANTIEU = 4838	-- Rương Vận Tiêu (reward)

-- NPC Template IDs
NPC_DONG_TIEUXA = 2084		-- Đồng Tiêu Xa
NPC_BAC_TIEUXA = 2085		-- Bạc Tiêu Xa
NPC_VANG_TIEUXA = 2086		-- Vàng Tiêu Xa
NPC_HONG_TIEUXA = 1185		-- Tiêu Xa Hồng (robbed)
NPC_RUONG_CUOP = 844		-- Rương cướp

-- SubWorld IDs (adjust cho HuyenThiet server)
-- ThienDieu dùng SubWorld 21 = Thành Đô
-- HuyenThiet cần check SubWorld ID tương ứng
SUBWORLD_START = 21			-- Map bắt đầu (Thành Đô hoặc tương đương)

-- Coordinates (cần adjust cho HuyenThiet)
POS_START_X = 2623			-- Vị trí spawn tiêu xa
POS_START_Y = 4503

POS_END_X = 243				-- Vị trí giao tiêu (Thanh Thành Sơn)
POS_END_Y = 219

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
