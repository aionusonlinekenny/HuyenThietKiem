Include("\\script\\lib\\TaskLib.lua");
Include("\\script\\event\\VanTieu\\lib.lua");

function GetNpcValue(nNpcIdx)
	-- Workaround: Use NpcParam instead of NpcValue
	if nNpcIdx <= 0 then
		return 0
	end
	return GetNpcParam(nNpcIdx, 1) -- Use param slot 1 for UUID storage
end

function SetNpcValue(nNpcIdx, nValue)
	if nNpcIdx <= 0 then
		return
	end
	SetNpcParam(nNpcIdx, 1, nValue)
end

function AddRespect(nAmount)
	-- Workaround: Use AddRepute instead
	-- HuyenThiet has AddRepute, ThienDieu uses AddRespect
	-- They might be the same thing
	AddRepute(nAmount)
end

function FindNearNpc(dwNpcID)
	-- Workaround: Since we don't have FindNearNpc, use FindAroundNpc
	-- This is less accurate but functional
	return FindAroundNpc(dwNpcID)
end

function AddItemIDStack(nItemIdx, nStack)
	-- Workaround: Add items one by one or use loop
	-- For now, just add the item (stack will be set by item properties)
	return nItemIdx
end

function SetNpcOwner_Backup(nNpcIdx, szOwnerName, nMode)
	if nNpcIdx <= 0 then
		return
	end
	local nPlayerIdx = GetPlayerIndex()
	if not nPlayerIdx or nPlayerIdx < 0 then
		return
	end
	SetNpcParam(nNpcIdx, 2, 1) -- Mark as having owner (legacy)
end

function SetNpcTimeout(nNpcIdx, nSeconds)
	-- Workaround: Use timer to delete NPC after timeout
	-- Real implementation would be in C++ NPC class
	-- For now, just mark it (will need manual cleanup)
	if nNpcIdx <= 0 then
		return
	end
	SetNpcParam(nNpcIdx, 3, nSeconds) -- Store timeout
	-- TODO: Implement actual auto-despawn
end

function AddNpcWithScript(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, szScript, bRemoveDeath, szName)
	-- Enhanced AddNpc with script support
	-- AddNpc signature: (templateID, level, subworld, x, y, removeOnDeath, name, param8, param9)
	local nNpcIdx = AddNpc(nTemplateID, nLevel, nSubWorldIdx, nMpsX, nMpsY, bRemoveDeath or 1, szName or "", 0, 0)
	if nNpcIdx > 0 and szScript and szScript ~= "" then
		SetNpcScript(nNpcIdx, szScript)
	end
	return nNpcIdx
end

function GetNpcIDFromIndex(nNpcIdx)
	-- Get NPC's m_dwID
	if nNpcIdx <= 0 then
		return 0
	end
	return GetNpcID(2, nNpcIdx) -- Type 2 = m_dwID
end

-- Main dialog
function main(NpcIndex)
	dofile("script/event/VanTieu/tieudau.lua")

	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		Talk(1,"","Ai còng t­ëng nghÒ B¶o tiªu lµ sung s­íng! ThËt sù mçi lÇn ra ®i ®Òu kh«ng d¸m huaø hÑn tr­íc ngµy vÒ!")
		return
	end

	Say("Thµnh §« tiªu côc cña chóng ta lu«n ®­îc sù tÝn nhiÖm cña giang hå",4,
	"VËn tiªu/vantieu",
	"§æi Hå Tiªu LÖnh/cuahang",
	"T×m hiÓu vËn tiªu/timhieu",
	"Ta chØ ghÐ qua/no")
end

function vantieu()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if(nTask == 0) then
		Say("GÇn ®©y c«ng viÖc nhiÒu, ®¹o tÆc hoµnh hµnh kh¾p n¬i, mµ nh©n lùc l¹i thiÕu. VËy ®©y cã muèn giñp ta mét chuyÕn không? SÏ cã thï lao xøng ®¸ng cho ng­¬i!",2,
		"Ta ®ång ý ¸p tiªu/batdau",
		"Ta ®ang rÊt bËn/no")
	elseif(nTask < 4) then
		-- Check if player was robbed
		if(GetItemCountInBag(6, ITEM_TIEUKY, 1, -1, 0) > 0) then
			bicuop()
			return
		end

		Say("Kh«ng ph¶i ng­êi ®ang ¸p tiªu sao? Sao l¹i ®Õn ®©y t×m ta?",4,
		"Ta bÞ thÊt l¹c, giñp ta t×m tiªu xa/timxe",
		"Reset tiªu xa (Test - MiÔn phÝ)/resettieuxatest",
		"Ta kh«ng muèn lµm n÷a/huybo",
		"Ta chØ ghÐ qua/no")
	else
		hoanthanh()
	end
end


function batdau()

	if(GetLevel() < 120) then
		Talk(1,"","H·y cè g¾ng luyÖn tËp ®¹t cËp 120 rèi ®Õn gËp ta")
		return
	end

	-- Check daily limit
	local nResetTask = GetTask(TASK_RESET_VANTIEU)
	local nLan = GetByte(nResetTask, 6)

	if(nLan >= MAX_DAILY_RUNS) then
		if(GetItemCountInBag(6, ITEM_UNLOCK_VANTIEU, 1, -1, 0) < 1) then
			Talk(1,"","H«m nay ng­êi ®· ¸p tiªu nhiÒu lÇn råi. H·y nghØ ng¬i mai l¹i ®Õn gËp ta.")
			return
		end
	end

	if(GetCash() < COST_START_QUEST) then
		Talk(1,"","Ng­êi cÇn n¹p ".. (COST_START_QUEST / 10000) .." v¹n l­îng phÝ hao tèn tiªu xa!")
		return
	end

	-- Consume unlock item or increment daily counter
	if(nLan >= MAX_DAILY_RUNS) then
		DelTaskItem(ITEM_UNLOCK_VANTIEU, 1)
	else
		SetTask(TASK_RESET_VANTIEU, SetByte(nResetTask, 6, nLan + 1))
	end

	-- Random cart type (1-3 -> §ång/B¹c/Vµng)
	local n = random(0, 2)
	local nRand = n + 1

	-- Calculate spawn positions BEFORE teleporting player
	-- Use fixed coordinates, don't rely on GetWorldPos() after NewWorld()
	local nTemplateID = TIEUXA_TEMPLET[nRand][1]

	-- CRITICAL: Check if we should use SubWorld ID or Index
	-- Try both approaches to see which works
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)

	-- Player will be at POS_START_X/Y after teleport
	local nPlayerX = floor(POS_START_X * 32)  -- Convert map coords to pixels
	local nPlayerY = floor(POS_START_Y * 32)

	-- Spawn cart offset +2 tiles East of player position
	local nCartX = nPlayerX + 64  -- +64 pixels = +2 tiles
	local nCartY = nPlayerY

	-- Pay player first
	Pay(COST_START_QUEST)

	-- Teleport player to start location
	-- IMPORTANT: Check return value! NewWorld() returns 1 on success
	local nTeleportOK = NewWorld(SUBWORLD_START, POS_START_X, POS_START_Y)

	if nTeleportOK ~= 1 then
		Talk(1,"","Lçi: Kh«ng thÓ ®i chuyÓn ®Õn vÞ trÝ b¾t ®Çu!")
		return
	end
	-- Add teleport effect (18 frames * 3 = 54 frames ~= 2.7 seconds at 20fps)
	-- This also ensures client/server position sync before spawning cart
	AddSkillState(963, 1, 0, 18*3)

	-- Reset fight state after teleport (like town portal does)
	SetFightState(0)
	-- Spawn escort cart at calculated position
	-- NOTE: Cart spawns AFTER successful teleport
	local nId = AddNpc(
		nTemplateID,				-- Template ID
		1,							-- Level
		nSubWorldIdx,				-- SubWorld Index
		nCartX,						-- X (player X + 64)
		nCartY,						-- Y (player Y)
		1,							-- Remove on death
		"",							-- Name (will be set below)
		0,							-- Param 8
		0							-- Param 9
	)

	if nId > 0 then
		-- CRASH ISOLATION: Test SetNpcScript
		SetNpcScript(nId, "\\script\\event\\VanTieu\\tieuxa.lua")

		-- Set NPC to friendly/neutral camp so it won't attack player
		if SetNpcCamp ~= nil then
			SetNpcCamp(nId, 0)  -- Camp 0 = neutral
		end
		if SetNpcCurCamp ~= nil then
			SetNpcCurCamp(nId, 0)
		end

		-- Set NPC series to match player
		local nPlayerSeries = GetSeries()
		if nPlayerSeries and SetNpcSeries ~= nil then
			SetNpcSeries(nId, nPlayerSeries)
		end

	end

	if nId <= 0 then
		Talk(1,"","Lçi: Kh«ng thÓ t¹o tiªu xa!")
		return
	end

	-- Setup cart
	local nName = GetName()


	-- IMPORTANT: Set NPC life/HP so AI will activate
	-- AI won't run if m_CurrentLifeMax = 0 (check in KNpcAI.cpp line 34)
	if SetNpcLifeMax then
		SetNpcLifeMax(nId, 10000)  -- 10k HP
		SetNpcLife(nId, 10000)
	end

	-- Setup NPC owner and follow behavior
	if SetNpcOwner ~= nil then
		SetNpcOwner(nId, 1)  -- 1 = enable follow
	end

	local sCartName = nName .. " - " .. TIEUXA_TEMPLET[nRand][2]

	SetNpcName(nId, sCartName)
	SetNpcTimeout(nId, CART_TIMEOUT)
	local nUUID = GetUUID()
	SetNpcValue(nId, nUUID)
	local dwCartID = GetNpcIDFromIndex(nId)
	SetTask(TASK_NPCVANTIEU, dwCartID)


	-- Update quest state

	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 1, n + 1))
	SetTask(TASK_VANTIEU, SetByte(GetTask(TASK_VANTIEU), 2, random(1, 3)))


	-- Notify player

	Msg2Player("H·y mau hé tèng tiªu xa ®Õn Long M«n Tiªu S­ ë Thanh Thµnh S¬n ("..POS_END_X.."/"..POS_END_Y..")")

end


function cuahang()
	local nCount = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0
	Say("Ng­êi hiÖn cã: <color=red>"..nCount.."<color> Hå Tiªu LÖnh. H·y lùa chän vËt phÈm cÇn thiÕt cho tiªu xa:",4,
	"T¨ng tãc (4 Hå Tiªu LÖnh)/buy_item("..ITEM_TANGTO..",4)",
	"Hèi m¸u (3 Hå Tiªu LÖnh)/buy_item("..ITEM_HOIMAU..",3)",
	"DÞch chuyÓn (3 Hå Tiªu LÖnh)/buy_item("..ITEM_DICHCHUYEN..",3)",
	"Ta cha muèn ®æi/no")
end

function buy_item(nItemID, nCost)
	if(CountFreeBagCell() < 1) then
		Talk(1,"","Hµnh trang cña ng­êi kh«ng cßn chç trèng")
		return
	end

	local nHave = GetItemCountInBag(6, ITEM_HO_TIEU_LENH, 1, -1, 0) or 0

	if(nCost > nHave) then
		Talk(1,"","Ng­êi kh«ng mang ®ñ Hå Tiªu LÖnh rïi")
		return
	end

	AddTaskItem(nItemID)
	DelTaskItem(ITEM_HO_TIEU_LENH, nCost)

	Talk(1,"","Giao dÞch thµnh c«ng!")
end

function hoanthanh()
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nFinish = GetByte(nTaskValue, 1)

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)

	phanthuong(nFinish)
	Talk(1,"","Lµm tæt l¾m! ®©y lµ phÇn lao vô cña ng­êi!")
end

function bicuop()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	-- Try to delete cart if still exists
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	local nNpcIdx = FindNearNpc(dwCartID)

	if(nNpcIdx > 0) then
		DelNpc(nNpcIdx)
	end

	local nKind = GetByte(GetTask(TASK_VANTIEU), 1)
	phanthuong(nKind) -- Give 50% reward

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)
	DelTaskItem(ITEM_TIEUKY, 1)

	Talk(1,"","Ng­¬i bÞ c­íp tiªu råi sao? Còng may ng­­¬i ®· ®o¹t l¹i TÝ tiªu Kú danh dô cña Thµnh §« tiªu côc. VÊt vá cho ng­êi rïi! ®©y lµ mét n÷a phÇn lao vô")
end


function huybo()
	local SubWorld = SubWorldID2Idx(SUBWORLD_START)
	if (SubWorld < 0) then
		return
	end

	-- Delete cart
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	local nNpcIdx = FindNearNpc(dwCartID)

	if(nNpcIdx > 0) then
		DelNpc(nNpcIdx)
	end

	SetTask(TASK_VANTIEU, 0)
	SetTask(TASK_NPCVANTIEU, 0)

	Talk(1,"","C«ng viÖc ¸p tiªu kh«ng ph¶i lµ ®¬n giÇn. Ng­êi ch¸n ®Æt ch¸n rµo b­íc vµo nªn còng khã tr¸ch! Sau nµy h·y cô g¾ng lªn!")
end


function timxe()
	if(GetCash() < COST_FIND_CART) then
	 Talk(1,"","Ng­êi cÇn nép phÝ ".. (COST_FIND_CART / 10000) .." v¹n l­îng ®Ó ta cho ng­êi ®i t×m tiªu xa!")
	 return
end

local dwCartID = GetTask(TASK_NPCVANTIEU)
local nNpcIdx = FindAroundNpc(dwCartID)

if (nNpcIdx == 1) then
	 Talk(1,"","Kh«ng ph¶i tiªu xa cña ng­¬i ®ang ë ngay ®©y sao!")
	 return
end

nNpcIdx = FindNearNpc(dwCartID)

if(nNpcIdx > 0) then
	 local w, x, y = GetNpcPos(nNpcIdx)
	 NewWorld(w, x, y)
	 Pay(COST_FIND_CART)
	 Talk(1,"","§· cã tin töc tiªu xa! Ta sÏ cho ng­êi ®Õn ®ã!")
else
	 Talk(1,"","Kh«ng thÊy tin töc! Tiªu xa cña ng­­¬i cã lÎ ®· mÊt!")
end

end

function resettieuxatest()
	-- Test function to reset cart for easier testing (FREE)
	local nTaskValue = GetTask(TASK_VANTIEU)
	local nTask = GetByte(nTaskValue, 1)

	if nTask == 0 or nTask >= 4 then
		Talk(1,"","Ng­êi kh«ng cã nhiÖm vô vËn tiªu ®ang thùc hiÖn!")
		return
	end

	-- Delete old cart if exists
	local dwCartID = GetTask(TASK_NPCVANTIEU)
	if dwCartID > 0 then
		local nNpcIdx = FindAroundNpc(dwCartID)
		if nNpcIdx > 0 then
			DelNpc(nNpcIdx)
			Msg2Player("§· xóa tiªu xa cò")
		end
	end

	-- Get player position and subworld
	local nSubWorldIdx = SubWorldID2Idx(SUBWORLD_START)
	local w, x, y = GetWorldPos()

	-- Determine cart type from task
	local nRand = nTask -- Task value 1/2/3 = Ð?ng/B?c/Vàng
	local nTemplateID = TIEUXA_TEMPLET[nRand][1]

	-- Spawn new cart at player position
	local nId = AddNpc(
		nTemplateID,
		1,
		nSubWorldIdx,
		x,
		y,
		1,
		"",
		0,
		0
	)

	if nId > 0 then
		SetNpcScript(nId, "\\script\\event\\VanTieu\\tieuxa.lua")

		-- Set friendly
		if SetNpcCamp ~= nil then
			SetNpcCamp(nId, 0)
		end
		if SetNpcCurCamp ~= nil then
			SetNpcCurCamp(nId, 0)
		end

		-- Set series
		local nPlayerSeries = GetSeries()
		if nPlayerSeries and SetNpcSeries ~= nil then
			SetNpcSeries(nId, nPlayerSeries)
		end
		local nName = GetName()

		if SetNpcOwner ~= nil then
			SetNpcOwner(nId, nName, 1)
		else
			SetNpcOwner_Backup(nId, nName, 1)
		end

		SetNpcName(nId, nName .. TIEUXA_TEMPLET[nRand][2])
		SetNpcTimeout(nId, CART_TIMEOUT)
		SetNpcValue(nId, GetUUID())

		-- Update task with new NPC ID
		local dwNewCartID = GetNpcIDFromIndex(nId)
		SetTask(TASK_NPCVANTIEU, dwNewCartID)

		Msg2Player("§· reset tiªu xa thµnh c«ng! Xe míi xuÊt hiÖn t¹i vÞ trÝ cña ng­êi.")
		else
			Talk(1,"","Lçi: Kh«ng thÓ t¹o tiªu xa míi!")
		end

end

function timhieu()
	Talk(2,"","Con ®­êng hiÓm trë nhÊt lµ b¨ng qua Thanh Thµnh S¬n. N¬i ®ã ®Êy r·y thó d÷ vµ ®¹o tÆc hoµnh hµnh. Ng­êi ph¶i hå Tiªu an toµn ®Õn n¬i giao cho Tiªu S trung chuyÓn ®óng thêi gian!",
	"Hµng ho¸ cÇn vËn chuyÓn cã 3 chñng lo¹i: vµng, b¹c, ®ång. Tiªu c«ng gi¸ trÞ thÕ bän ®¹o tÆc còng dÌ mÆt tµi nªn cµng khã kh¨n nh­ng thï lao ng­êi nhËn ®ùc còng t­¬ng xøng!")
end


-- Reward function
function phanthuong(nFinish)
	local tbName = GetName()
	if(nFinish == TASK_STATE_DONG) then	-- Ð?ng tiêu xa
		AddTaskItem(ITEM_HO_TIEU_LENH)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0) -- Ruong ho?t d?ng
		AddOwnExp(5 * KINH_NGHIEM_BASE)
		AddRespect(5)
		-- AddItemIDStack - workaround
		for i=1,5 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..tbName.."<color> giao thµnh c«ng §ång Tiªu Xa nhËn ®­îc <color=yellow>5 triÖu ®iÓm<color> kinh nghiÖm vµ <color=yellow>5<color> ®iÓm Uy Danh!")
		elseif (nFinish == TASK_STATE_BAC) then
		for i=1,4 do
			AddTaskItem(ITEM_HO_TIEU_LENH)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(10 * KINH_NGHIEM_BASE)
		AddRespect(10)
		for i=1,10 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..tbName.."<color> giao thµnh c«ng B¹c Tiªu Xa nhËn ®­îc <color=yellow>10 triÖu ®iÓm<color> kinh nghiÖm vµ <color=yellow>10<color> ®iÓm Uy Danh!")
	elseif(nFinish == TASK_STATE_VANG) then	-- Vàng tiêu xa
		for i=1,6 do
			AddTaskItem(ITEM_HO_TIEU_LENH)
		end
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(15 * KINH_NGHIEM_BASE)
		AddRespect(15)
		for i=1,15 do
			AddScriptItem(ITEM_RUONG_VANTIEU)
		end
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..tbName.."<color> giao thµnh c«ng Vµng Tiªu Xa nhËn ®­îc <color=yellow>15 triÖu ®iÓm<color> kinh nghiÖm vµ <color=yellow>10<color> ®iÓm Uy Danh!")
	else
		AddTaskItem(ITEM_HO_TIEU_LENH)
		AddItem(0, 2, 34, 0, 0, 5, 0, 0)
		AddOwnExp(2 * KINH_NGHIEM_BASE)
		Msg2SubWorld("§¹i HiÖp <color=yellow>"..tbName.."<color> bÞ c­íp tiªu kh«ng hoµn thµnh nhiÖm vô!")
	end
end

function no()
end