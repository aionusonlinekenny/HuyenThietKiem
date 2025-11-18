-- Tiªu S­ - VËn Tiªu Quest Receiver NPC
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

	Say("Ta lµ ng­êi cña Long M«n tiªu côc. Ta tiÕp nhËn trung chuyÓn nh÷ng chuyÕn tiªu ë n¬i nµy! Ng­êi cÇn viÖc göi?",2,
	"Giao tiªu/giaotieu",
	"Hái th¨m th«i/no")
end

function giaotieu()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if(nTask == 0) then
		Talk(1,"","Hõ! Ng­¬i ®©u ph¶i lµ ng­êi do Long M«n tiªu côc ph¸i tíi!")
	elseif(nTask < TASK_STATE_DONG) then
		local dwCartID = GetTask(TASK_NPCVANTIEU)
		local nNpcIdx = FindAroundNpc(dwCartID)
		if nNpcIdx == 1 then
			nNpcIdx = FindNearNpc(dwCartID)
		end

		if(nNpcIdx > 0) then
			DelNpc(nNpcIdx)
			SetTask(TASK_VANTIEU, SetByte(nTaskValue, 1, nTask + 3))
			Talk(1,"","Tèt l¾m! H·y vÒ gÆp «ng chñ ®Ó nhËn lao phÝ ®i b¹n trÎ!")
		else

			Talk(1,"","Tiªu xa cña ng­­¬i ®©u? Ta kh«ng nh×n thÊy!")
		end
	else	-- Already completed
		Talk(1,"","H·y vÒ gÆp «ng chñ ®Ó nhËn lao phÝ ®i b¹n trÎ!")
	end
end

function no()
end
