-- Tiêu Sư - Vận Tiêu Quest Receiver NPC
-- Ported from ThienDieuOnline to HuyenThietKiem

Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\event\\VanTieu\\lib.lua");

-- Workaround functions for compatibility
function FindNearNpc(dwNpcID)
	-- Workaround: Use FindAroundNpc since FindNearNpc may not exist
	return FindAroundNpc(dwNpcID)
end

function main(NpcIndex)
	dofile("script/event/VanTieu/tieusu.lua")

	Say("Ta là người của Long Môn tiêu cục. Ta tiếp nhận trung chuyển những chuyến tiêu ở nơi này! Ngươi có việc gì?",2,
	"Giao tiêu/giaotieu",
	"Hỏi thăm thôi/no")
end

function giaotieu()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if(nTask == 0) then
		Talk(1,"","Hử! Ngươi đâu phải là người do Long Môn tiêu cục phái tới!")
	elseif(nTask < TASK_STATE_DONG) then
		local dwCartID = GetTask(TASK_NPCVANTIEU)
		local nNpcIdx = FindAroundNpc(dwCartID)

		Msg2Player("DEBUG: dwCartID = "..tostring(dwCartID))
		Msg2Player("DEBUG: FindAroundNpc returned nNpcIdx = "..tostring(nNpcIdx))
		Msg2Player("DEBUG: nTask = "..tostring(nTask))

		-- FindAroundNpc returns:
		-- 0 or negative = not found
		-- 1 = NPC is very close (right next to player)
		-- >1 = NPC index found but farther away
		-- Try FindNearNpc if FindAroundNpc returns 1 (too close)
		if nNpcIdx == 1 then
			nNpcIdx = FindNearNpc(dwCartID)
			Msg2Player("DEBUG: Too close, trying FindNearNpc = "..tostring(nNpcIdx))
		end

		if(nNpcIdx > 0) then
			-- REMOVED BROKEN CHECK: Previously checked GetByte(nTaskValue, 2) which was always > 0
			-- Byte 2 is set random(1,3) in tieudau.lua, NOT for monster kill tracking
			-- For now, allow delivery without monster kill requirement

			-- Success! Delete cart and mark quest as complete
			DelNpc(nNpcIdx)
			SetTask(TASK_VANTIEU, SetByte(nTaskValue, 1, nTask + 3))

			Msg2Player("✓ Quest completed! New task state = "..(nTask + 3))
			Msg2Player("✓ Return to Tiêu Đầu to claim rewards!")
			Talk(1,"","Tốt lắm! Hãy về gặp ông chủ để nhận lao phù đi bạn trẻ!")
		else
			Msg2Player("ERROR: Cart NPC not found! nNpcIdx = "..tostring(nNpcIdx))
			Talk(1,"","Tiêu xa của ngươi đâu? Ta không nhìn thấy!")
		end
	else	-- Already completed
		Talk(1,"","Hãy về gặp ông chủ để nhận lao phù đi bạn trẻ!")
	end
end

function no()
end
