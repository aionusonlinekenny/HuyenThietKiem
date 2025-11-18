-- Ruong V?n Tiêu - V?n Tiêu Event Reward Box
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

-- Equipment reward table (replacing Th? Trang B?)
-- HuyenThietKiem doesn't have items 4827-4835, use random equipment instead
AWARD_EQUIPMENT = {
	{0, 102, 70, 30}, -- Áo (Armor)
	{0, 103, 30, 30}, -- Nón (Helm)
	{0, 100, 10, 30}, -- Vu khí (Weapon)
	{0, 101, 10, 30}, -- Nh?n (Ring)
	{0, 108, 30, 30}, -- H? uy?n (Cuff)
	{0, 107, 30, 30}, -- Hài (Boot)
	{0, 104, 30, 30}, -- Ðai (Belt)
	{0, 105, 30, 30}, -- Ng?c b?i (Pendant)
}

-- Bí kíp 90 theo môn phái
TAB_Menpai = {
	{318, 321, 319},  -- Thi?u Lâm (Shaolin)
	{323, 325, 322},  -- Thiên Vuong (Tianwang)
	{302, 339, 342, 351},  -- Ðu?ng Môn (Tangmen)
	{353, 355, 390},  -- Ngu Ð?c (Wudu)
	{380, 381, 382},  -- Nga My (Emei)
	{362, 363, 364},  -- Thúy Yên (Cuiyan)
	{372, 373, 375},  -- Cái Bang (Gaibang)
	{334, 335, 336},  -- Thiên Nh?n (Tianren)
	{348, 349, 350},  -- Võ Ðang (Wudang)
	{313, 314, 315},  -- Côn Lôn (Kunlun)
}

-- Main function
function OnUse(nItemIdx)
	-- Check inventory space
	local nFreeSpace = CalcFreeItemCellCount()
	if nFreeSpace < 5 then
		Talk(1, "", "Hµnh trang kh«ng ®ñ chç trèng! CÇn Ýt nhÊt <color=yellow>5 « trèng<color> ®Ó më r­­¬ng.")

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
		Msg2Player("B¹n nhËn ®­îc <color=yellow>Trang BÞ CÊp 30+<color>")
Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ®· nhËn ®­îc <color=cyan>Trang BÞ Quý<color> tõ B¶o R­¬ng VËn Tiªu!")

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
				-- AddItem for skill book: Genre 7, Detail 15 = Ð?i Thành Bí Kíp 90
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
		Msg2Player("B¹n nhËn ®­îc <color=yellow>BÝ KÝp 90<color>")
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ®· nhËn ®­îc bÝ kÝp 90 tõ B¶o R­¬ng VËn Tiªu!")

	elseif (sel < 47 and sel > 40) then
        -- 12% chance (6/50): Thñy Tinh (Crystals)
        -- Genre 6, Detail 15-18:
        -- 15 = Tinh Häng B¶o Th¹ch
        -- 16 = Lam Thñy Tinh (change equipment element)
        -- 17 = Tö Thñy Tinh (upgrade equipment)
        -- 18 = Lôc Thñy Tinh (change equipment stats)

		local nCrystalType = random(15, 18)
		local nItemIdx = AddItem(6, nCrystalType, 1, 0, 0, 0, 0)
		if nItemIdx > 0 then
			SetItemBindState(nItemIdx, 1)
		end

		local sCrystalName = "Thñy Tinh"
		if nCrystalType == 15 then sCrystalName = "Tinh Höng B¶o Th¹ch"
		elseif nCrystalType == 16 then sCrystalName = "Lam Thñy Tinh"
		elseif nCrystalType == 17 then sCrystalName = "T÷ Thñy Tinh"
		elseif nCrystalType == 18 then sCrystalName = "Lôc Thñy Tinh"
		end

		Msg2Player("B¹n nhËn ®­îc <color=yellow>"..sCrystalName.."<color> tù B¶o R¦ìng VËn Tiªu!")
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ·· nhËn ®­îc <color=cyan>"..sCrystalName.."<color> tù B¶o R¦ìng VËn Tiªu!")

	elseif (sel <= 40 and sel > 20) then
		-- 40% chance (20/50): Phóc Duyên Lô
		-- Genre 7, Detail 11-13 (Phóc Duyên Lô tiÓu/trung/§¹i)
		local nType = random(11, 13)
		local nItemIdx = AddItem(7, nType, 1, 0, 0, 0, 0)
		if nItemIdx > 0 then
			SetItemBindState(nItemIdx, 1)
		end
		Msg2Player("B¹n nhËn ®­îc <color=yellow>Phóc Duyên Lô<color>!")

	else
		-- 40% chance (20/50): Random common items
		local nRand = RANDOMC(3, 4, 5, 6, 11, 12, 13) -- Various consumables

		if nRand == 3 then
			-- Tiªn Th¶o Lô (7,3)
			local nItemIdx = AddItem(7, 3, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("B¹n nhËn ®­îc <color=yellow>Tiªn Th¶o Lô<color>!")
			Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ·· nhËn ®­îc <color=cyan>Tiªn Th¶o Lô<color> tù B¶o R¦ìng VËn Tiªu!")

		elseif nRand == 4 then
			-- Tiªn Th¶o Lô ®Æc biÖt (7,4)
			local nItemIdx = AddItem(7, 4, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("B¹n nhËn ®­îc <color=yellow>Tiªn Th¶o Lô [§Æc BiÖt]<color>!")
			Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ·· nhËn ®­îc <color=cyan>Tiªn Th¶o Lô [§Æc BiÖt]<color> tù B¶o R¦ìng VËn Tiªu!")

		elseif nRand == 5 then
			-- Thiªn S¬n B¶o Lô (7,5)
			local nItemIdx = AddItem(7, 5, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("B¹n nhËn ®­îc <color=yellow>Thiªn S¬n B¶o Lô<color>!")
			Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ·· nhËn ®­îc <color=cyan>Thiªn S¬n B¶o Lô<color> tù B¶o R¦ìng VËn Tiªu!")

		elseif nRand == 6 then
			-- Qu¶ Hoa Tùu (7,6)
			local nItemIdx = AddItem(7, 6, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("B¹n nhËn ®­îc <color=yellow>Qu¶ Hoa Tùu<color>!")
			Msg2SubWorld("§¹i HiÖp <color=yellow>"..szName.."<color> ·· nhËn ®­îc <color=cyan>Qu¶ Hoa Tùu<color> tù B¶o R¦ìng VËn Tiªu!")

		else
			-- Phóc Duyên Lô (backup)
			local nType = random(11, 13)
			local nItemIdx = AddItem(7, nType, 1, 0, 0, 0, 0)
			if nItemIdx > 0 then SetItemBindState(nItemIdx, 1) end
			Msg2Player("B¹n nhËn ®­îc <color=yellow>Phóc Duyên Lô<color>!")
		end
	end
	-- Always give some silver
	local nSilver = random(50000, 200000) -- 5-20 v¬n
	Earn(nSilver)
	Msg2Player("NhËn thªm <color=yellow>"..(nSilver/10000).." v¬n<color> b¹c!")
end

function no()
end
