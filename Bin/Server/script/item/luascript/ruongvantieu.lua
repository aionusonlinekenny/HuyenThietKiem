-- Rương Vận Tiêu - Vận Tiêu Event Reward Box
-- Ported from ThienDieuOnline to HuyenThietKiem
-- Author: Claude AI Assistant
-- Date: 2025-11-17
-- Updated: 2025-11-18 - Using ThienDieu reward logic

Include("\\script\\lib\\TaskLib.lua")

-- Helper function: Random select from list
function RANDOMC(...)
	local tArgs = arg
	local nCount = getn(tArgs)
	if nCount <= 0 then
		return 0
	end
	return tArgs[random(1, nCount)]
end

-- Equipment reward table (replacing Thẻ Trang Bị)
-- HuyenThietKiem doesn't have items 4827-4835, use random equipment instead
AWARD_EQUIPMENT = {
	{0, 102, 70, 30}, -- Áo (Armor)
	{0, 103, 30, 30}, -- Nón (Helm)
	{0, 100, 10, 30}, -- Vũ khí (Weapon)
	{0, 101, 10, 30}, -- Nhẫn (Ring)
	{0, 108, 30, 30}, -- Hộ uyển (Cuff)
	{0, 107, 30, 30}, -- Hài (Boot)
	{0, 104, 30, 30}, -- Đai (Belt)
	{0, 105, 30, 30}, -- Ngọc bội (Pendant)
}

-- Bí kíp 90 theo môn phái
TAB_Menpai = {
	{318, 321, 319},  -- Thiếu Lâm (Shaolin)
	{323, 325, 322},  -- Thiên Vương (Tianwang)
	{302, 339, 342, 351},  -- Đường Môn (Tangmen)
	{353, 355, 390},  -- Ngũ Độc (Wudu)
	{380, 381, 382},  -- Nga My (Emei)
	{362, 363, 364},  -- Thúy Yên (Cuiyan)
	{372, 373, 375},  -- Cái Bang (Gaibang)
	{334, 335, 336},  -- Thiên Nhẫn (Tianren)
	{348, 349, 350},  -- Võ Đang (Wudang)
	{313, 314, 315},  -- Côn Lôn (Kunlun)
}

-- Main function
function OnUse(nItemIdx)
	-- Check inventory space
	local nFreeSpace = CalcFreeItemCellCount()
	if nFreeSpace < 5 then
		Talk(1, "", "Hành trang không đủ chỗ trống! Cần ít nhất <color=yellow>5 ô trống<color> để mở rương.")
		return 0
	end

	-- Open chest using ThienDieu logic
	levatchung()
	return 1
end

function levatchung()
	local sel = random(1, 50)
	local szName = GetName()

	if (sel > 47) then
		-- 6% chance (3/50): Random equipment level 30+
		local nIdx = random(1, getn(AWARD_EQUIPMENT))
		local equip = AWARD_EQUIPMENT[nIdx]
		local nItemIdx = AddItem(equip[1], equip[2], equip[3], equip[4], 0, 0, 0)
		if nItemIdx > 0 then
			SetItemBindState(nItemIdx, 1)
		end
		Msg2Player("Bạn nhận được <color=yellow>Trang Bị Cấp 30+<color>")
		Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Trang Bị Quý<color> từ Bảo Rương Vận Tiêu!")

	elseif (sel == 47) then
		-- 2% chance (1/50): Bí kíp 90
		-- Try to give faction-specific skill book
		local nFaction = GetFaction()
		if nFaction and nFaction ~= "" then
			local nMenpaiIdx = 1
			if nFaction == "shaolin" then nMenpaiIdx = 1
			elseif nFaction == "tianwang" then nMenpaiIdx = 2
			elseif nFaction == "tangmen" then nMenpaiIdx = 3
			elseif nFaction == "wudu" then nMenpaiIdx = 4
			elseif nFaction == "emei" then nMenpaiIdx = 5
			elseif nFaction == "cuiyan" then nMenpaiIdx = 6
			elseif nFaction == "gaibang" then nMenpaiIdx = 7
			elseif nFaction == "tianren" then nMenpaiIdx = 8
			elseif nFaction == "wudang" then nMenpaiIdx = 9
			elseif nFaction == "kunlun" then nMenpaiIdx = 10
			end

			if TAB_Menpai[nMenpaiIdx] then
				local nSkillIdx = random(1, getn(TAB_Menpai[nMenpaiIdx]))
				local nSkillID = TAB_Menpai[nMenpaiIdx][nSkillIdx]
				-- Note: AddMagicScript may not exist in HuyenThietKiem, use AddItem instead
				-- AddItem for skill book: Genre 7, Detail 15 = Đại Thành Bí Kíp 90
				local nItemIdx = AddItem(7, 15, 1, 0, 0, 0, 0)
				if nItemIdx > 0 then
					SetItemBindState(nItemIdx, 1)
				end
			end
		else
			-- No faction, give generic skill book
			local nItemIdx = AddItem(7, 15, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then
				SetItemBindState(nItemIdx, 1)
			end
		end
		Msg2Player("Bạn nhận được <color=yellow>Bí Kíp 90<color>")
		Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được bí kíp 90 từ Bảo Rương Vận Tiêu!")

	elseif (sel < 47 and sel > 40) then
		-- 12% chance (6/50): Ngọc (replacing Thủy Tinh)
		-- Genre 1, Detail 6 = Precious stones
		local nNgocType = random(1, 10) -- Various gem types
		local nItemIdx = AddItem(1, 6, nNgocType, 1, 0, 0, 0)
		if nItemIdx > 0 then
			SetItemBindState(nItemIdx, 1)
		end
		Msg2Player("Bạn nhận được <color=yellow>Ngọc Quý<color> từ Bảo Rương Vận Tiêu!")
		Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Ngọc Quý<color> từ Bảo Rương Vận Tiêu!")

	elseif (sel <= 40 and sel > 20) then
		-- 40% chance (20/50): Phúc Duyên Lộ
		-- Genre 7, Detail 11-13 (Phúc Duyên Lộ tiểu/trung/đại)
		local nType = random(11, 13)
		local nItemIdx = AddItem(7, nType, 1, 0, 0, 0, 0)
		if nItemIdx > 0 then
			SetItemBindState(nItemIdx, 1)
		end
		Msg2Player("Bạn nhận được <color=yellow>Phúc Duyên Lộ<color>!")

	else
		-- 40% chance (20/50): Random common items
		local nRand = RANDOMC(3, 4, 5, 6, 11, 12, 13) -- Various consumables

		if nRand == 3 then
			-- Tiên Thảo Lộ (7,3)
			local nItemIdx = AddItem(7, 3, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("Bạn nhận được <color=yellow>Tiên Thảo Lộ<color>!")
			Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Tiên Thảo Lộ<color> từ Bảo Rương Vận Tiêu!")

		elseif nRand == 4 then
			-- Tiên Thảo Lộ đặc biệt (7,4)
			local nItemIdx = AddItem(7, 4, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("Bạn nhận được <color=yellow>Tiên Thảo Lộ [Đặc Biệt]<color>!")
			Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Tiên Thảo Lộ [Đặc Biệt]<color> từ Bảo Rương Vận Tiêu!")

		elseif nRand == 5 then
			-- Thiên Sơn Bảo Lộ (7,5)
			local nItemIdx = AddItem(7, 5, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("Bạn nhận được <color=yellow>Thiên Sơn Bảo Lộ<color>!")
			Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Thiên Sơn Bảo Lộ<color> từ Bảo Rương Vận Tiêu!")

		elseif nRand == 6 then
			-- Quả Hoa Tựu (7,6)
			local nItemIdx = AddItem(7, 6, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("Bạn nhận được <color=yellow>Quả Hoa Tựu<color>!")
			Msg2SubWorld("Đại Hiệp <color=yellow>"..szName.."<color> đã nhận được <color=cyan>Quả Hoa Tựu<color> từ Bảo Rương Vận Tiêu!")

		else
			-- Phúc Duyên Lộ (backup)
			local nType = random(11, 13)
			local nItemIdx = AddItem(7, nType, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("Bạn nhận được <color=yellow>Phúc Duyên Lộ<color>!")
		end
	end

	-- Always give some silver
	local nSilver = random(50000, 200000) -- 5-20 vạn
	Earn(nSilver)
	Msg2Player("Nhận thêm <color=yellow>"..(nSilver/10000).." vạn<color> bạc!")
end

function no()
end
