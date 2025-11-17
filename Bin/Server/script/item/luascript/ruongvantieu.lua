-- Rương Vận Tiêu - Vận Tiêu Event Reward Box
-- Ported from ThienDieuOnline to HuyenThietKiem
-- Author: Claude AI Assistant
-- Date: 2025-11-17

Include("\\script\\lib\\TaskLib.lua")

-- Reward tables for different chest types
-- Format: {Genre, DetailType, Particular, Level, Series, Luck, Amount}
REWARDS_COMMON = {
	-- Common rewards (always given)
	{6, 1, 2, 1, 0, 0, 1},     -- Bạch Dương Đơn x1
	{6, 1, 3, 1, 0, 0, 1},     -- Thiên Niên Sâm x1
}

REWARDS_RARE = {
	-- Rare rewards (30% chance)
	{0, 102, 70, 10, 0, 0, 0}, -- Equipment level 10
	{0, 103, 30, 10, 0, 0, 0}, -- Equipment level 10
	{1, 6, 458, 1, 0, 0, 0},   -- Ngũ Hành Kỳ Thạch
}

REWARDS_VERY_RARE = {
	-- Very rare rewards (10% chance)
	{1, 1, 4, 1, 0, 0, 0},     -- Thiên Tâm Đơn
	{0, 102, 70, 30, 0, 0, 0}, -- Equipment level 30
}

-- Main function called when player uses the item
function OnUse(nItemIdx)
	-- Check inventory space
	local nFreeSpace = CalcFreeItemCellCount()
	if nFreeSpace < 5 then
		Talk(1, "", "Hành trang không đủ chỗ trống! Cần ít nhất <color=yellow>5 ô trống<color> để mở rương.")
		return 0
	end

	-- Check player level
	local nLevel = GetLevel()
	if nLevel < 120 then
		Talk(1, "", "Ngươi cần đạt cấp <color=yellow>120<color> mới có thể mở rương này!")
		return 0
	end

	-- Open chest and give rewards
	OpenVanTieuChest(nItemIdx)

	return 1
end

function OpenVanTieuChest(nItemIdx)
	local nRewardCount = 0

	-- Always give common rewards
	for i = 1, #REWARDS_COMMON do
		local reward = REWARDS_COMMON[i]
		local nIdx = AddItem(reward[1], reward[2], reward[3], reward[4], reward[5], reward[6], 0)
		if nIdx > 0 then
			SetItemBindState(nIdx, 1) -- Bind to character
			nRewardCount = nRewardCount + 1
		end
	end

	-- 30% chance for rare reward
	if random(1, 100) <= 30 then
		local reward = REWARDS_RARE[random(1, #REWARDS_RARE)]
		local nIdx = AddItem(reward[1], reward[2], reward[3], reward[4], reward[5], reward[6], 0)
		if nIdx > 0 then
			SetItemBindState(nIdx, 1)
			nRewardCount = nRewardCount + 1
			Msg2Player("Chúc mừng! Nhận được phần thưởng <color=yellow>Hiếm<color>!")
		end
	end

	-- 10% chance for very rare reward
	if random(1, 100) <= 10 then
		local reward = REWARDS_VERY_RARE[random(1, #REWARDS_VERY_RARE)]
		local nIdx = AddItem(reward[1], reward[2], reward[3], reward[4], reward[5], reward[6], 0)
		if nIdx > 0 then
			SetItemBindState(nIdx, 1)
			nRewardCount = nRewardCount + 1
			Msg2Player("Chúc mừng! Nhận được phần thưởng <color=red>Rất Hiếm<color>!")

			-- Announce to world
			local szName = GetName()
			Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> mở Rương Vận Tiêu nhận được vật phẩm quý giá!")
		end
	end

	-- Always give some silver
	local nSilver = random(50000, 200000) -- 5-20 vạn
	Earn(nSilver)

	-- Summary message
	Msg2Player("Mở Rương Vận Tiêu thành công!")
	Msg2Player("Nhận được <color=yellow>"..nRewardCount.."<color> vật phẩm và <color=yellow>"..(nSilver/10000).." vạn<color> bạc!")
end

function no()
end
