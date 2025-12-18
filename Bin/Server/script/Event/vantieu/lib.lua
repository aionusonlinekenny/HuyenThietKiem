-- Vận Tiêu (Escort) Event Library
-- Ported from ThienDieuOnline
-- Author: Claude AI Assistant
-- Date: 2025-11-15

Include("\\script\\lib\\TaskLib.lua")

-- NPC Template IDs cho tiêu xa
-- CRITICAL FIX: Original IDs 2085/2086/2087 have NO NpcResType (3D model) -> CRASH!
-- Using working NPC IDs with valid models instead:
-- NPC 2159 = "Tiêu Xa bị phá hoại" (has passerby379 model)
-- NPC 1185 = Building model (for robbed cart)
-- For now, use same model for all 3 types (can differentiate by name)
TIEUXA_TEMPLET = {
	{2085, "Ti�u Xa - [��ng]"},  -- Changed from 2085
	{2086, "Ti�u Xa - [B�c]"},   -- Changed from 2086
	{2087, "Ti�u Xa - [V�ng]"},  -- Changed from 2087
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
ITEM_RUONG_VANTIEU = 145		-- Rương Vận Tiêu (reward)

-- NPC Template IDs
-- FIXED: Using NPC 2159 which has valid model (passerby379)
NPC_DONG_TIEUXA = 2085		-- Đồng Tiêu Xa (using working model)
NPC_BAC_TIEUXA = 2086		-- Bạc Tiêu Xa (using working model)
NPC_VANG_TIEUXA = 2087		-- Vàng Tiêu Xa (using working model)
NPC_HONG_TIEUXA = 1185		-- Tiêu Xa Hồng (robbed)
NPC_RUONG_CUOP = 844		-- Rương cướp

-- SubWorld IDs (HuyenThiet server)
-- Map 11 = Dương Châu (major city)
SUBWORLD_START = 11			-- Map bắt đầu (Dương Châu)
SUBWORLD_END = 21			-- Map k?t th�c (Thanh Th�nh Son)
-- Coordinates (HuyenThiet - map 11)
-- Tiêu Đầu location: 98848, 164768 (updated - working position)
-- Tiêu Sư location: 98784, 164672
POS_START_X = 98848 / 32		-- Vị trí spawn tiêu xa (near Tiêu Đầu)
POS_START_Y = 164768 / 32


-- Ti�u Su location: Map 21 - Thanh Th�nh Son (end NPC)


POS_END_X = 89120 / 32			-- V? tr� giao ti�u (Ti�u Su at Map 21)
POS_END_Y = 128192 / 32

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