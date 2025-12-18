-- Tiêu Xa Hồng - Robbed Cart NPC Script
-- Player can click to get Tiêu Kỳ item

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

function main(nIndex)
	-- When player clicks the robbed cart
	if CountFreeBagCell() < 1 then
		Talk(1,"","Hành trang không đủ chỗ trống!")
		return
	end

	-- Give Tiêu Kỳ item
	AddItem(0, 6, ITEM_TIEUKY, 0, 0, 5, 0, 0)

	Talk(1,"","Ngươi đã nhặt được Tiêu Kỳ! Mang về cho Tiêu Đầu!")

	-- Delete this NPC after giving item
	DelNpc(nIndex)
end

function Timeout(nIndex)
	DelNpc(nIndex)
end
