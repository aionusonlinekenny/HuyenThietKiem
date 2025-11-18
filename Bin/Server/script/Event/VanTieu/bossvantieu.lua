-- Boss Váº­n TiÃªu - Boss monsters for escort event

Include("\\script\\lib\\TaskLib.lua")

function DropRate(NpcIndex)
	-- Track boss kills
	local nValue = GetNpcParam(NpcIndex, 1) -- Boss type (1/2/3)
	local nTaskValue = GetTask(TASK_VANTIEU)

	if(nValue == 1) then
		SetTask(TASK_VANTIEU, SetByte(nTaskValue, 7, 1))
	elseif (nValue == 2) then
		SetTask(TASK_VANTIEU, SetByte(nTaskValue, 8, 1))
	elseif (nValue == 3) then
		SetTask(TASK_VANTIEU, SetByte(nTaskValue, 9, 1))
	end

	Msg2Player("§· h¹ s¸t mét tªn s¬n tÆc!")
end

function LastDamage(NpcIndex)
	-- Last hit handler
end

nDAMAGE = 20

function Revive(NpcIndex)
	-- Set boss skills based on series
	local nSeries = GetNpcSeries(NpcIndex)
	local nSTVL, nDoc, nBang, nHoa, nLoi = 0,0,0,0,0

	if(nSeries == 0) then		-- KIM
		SetNpcSkill(NpcIndex, 321, 20, 1)
		SetNpcSkill(NpcIndex, 321, 20, 2)
		SetNpcSkill(NpcIndex, 321, 20, 3)
		SetNpcSkill(NpcIndex, 321, 20, 4)
		nSTVL = nDAMAGE
	elseif(nSeries == 1) then	-- MOC
		SetNpcSkill(NpcIndex, 355, 20, 1)
		SetNpcSkill(NpcIndex, 355, 20, 2)
		SetNpcSkill(NpcIndex, 355, 20, 3)
		SetNpcSkill(NpcIndex, 355, 20, 4)
		nDoc = math.floor(nDAMAGE/4)
	elseif(nSeries == 2) then	-- THUY
		SetNpcSkill(NpcIndex, 337, 20, 1)
		SetNpcSkill(NpcIndex, 337, 20, 2)
		SetNpcSkill(NpcIndex, 337, 20, 3)
		SetNpcSkill(NpcIndex, 337, 20, 4)
		nBang = nDAMAGE
	elseif(nSeries == 3) then	-- HOA
		SetNpcSkill(NpcIndex, 362, 20, 1)
		SetNpcSkill(NpcIndex, 362, 20, 2)
		SetNpcSkill(NpcIndex, 362, 20, 3)
		SetNpcSkill(NpcIndex, 362, 20, 4)
		nHoa = nDAMAGE
	elseif(nSeries == 4) then	-- THO
		SetNpcSkill(NpcIndex, 375, 20, 1)
		SetNpcSkill(NpcIndex, 375, 20, 2)
		SetNpcSkill(NpcIndex, 375, 20, 3)
		SetNpcSkill(NpcIndex, 375, 20, 4)
		nLoi = nDAMAGE
	else
		SetNpcSkill(NpcIndex, 321, 20, 1)
		nSTVL = nDAMAGE
	end

	SetNpcDamage(NpcIndex, nSTVL, nDoc, nBang, nHoa, nLoi)
end

function DeathSelf(NpcIndex)
	-- Boss death handler
end
