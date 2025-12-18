-- Rương Cướp - Treasure Chest (dropped when cart is robbed)

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

function main(nIndex)
	-- Check daily limit
	local nResetTask = GetTask(TASK_RESET_VANTIEU)
	local nLan = GetByte(nResetTask, 7)

	if(nLan >= MAX_DAILY_CHEST_LOOTS) then
		Talk(1,"","Hôm nay bạn đã cướp tiêu "..nLan.."/5 lần")
		return
	end

	-- Update counter
	SetTask(TASK_RESET_VANTIEU, SetByte(nResetTask, 7, nLan + 1))

	-- Give reward
	AddOwnExp(2500000) -- 2.5M exp

	Msg2Player("Hôm nay bạn cướp tiêu "..GetByte(GetTask(TASK_RESET_VANTIEU), 7).."/5 lần")

	-- Delete chest
	DelNpc(nIndex)
end

function Timeout(nIndex)
	DelNpc(nIndex)
end

function no()
end
