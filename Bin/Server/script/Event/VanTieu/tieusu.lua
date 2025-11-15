-- Tiêu Sư - Vận Tiêu Quest Receiver NPC
-- Ported from ThienDieuOnline to HuyenThietKiem

Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\Event\\VanTieu\\lib.lua")

function main(nIndex)
	Say("Ta là người của Long Môn tiêu cục. Ta tiếp nhận trung chuyển những chuyến tiêu ở nơi này! Ngươi có việc gì?",2,
	"Giao tiêu/giaotieu",
	"Hỏi thăm thôi/no")
end

function giaotieu()
	SubWorld = SubWorldID2Idx(SUBWORLD_START)
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

		if(nNpcIdx > 0) then
			-- Check if any monsters killed (bonus condition)
			local nKilled = GetByte(nTaskValue, 2)

			if (nKilled > 0) then
				Talk(1,"","Trên đường ngươi không hề gặp chuyện gì bất trắc sao?")
				return
			end

			-- Success! Delete cart and mark quest as complete
			DelNpc(nNpcIdx)
			SetTask(TASK_VANTIEU, SetByte(nTaskValue, 1, nTask + 3))

			Talk(1,"","Tốt lắm! Hãy về gặp ông chủ để nhận lao phù đi bạn trẻ!")
		else
			Talk(1,"","Tiêu xa của ngươi đâu? Ta không nhìn thấy!")
		end
	else	-- Already completed
		Talk(1,"","Hãy về gặp ông chủ để nhận lao phù đi bạn trẻ!")
	end
end

function no()
end
