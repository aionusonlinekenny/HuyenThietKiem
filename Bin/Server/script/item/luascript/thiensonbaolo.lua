-- Author:	Kinnox
-- Date:	31-05-2021
-- Thiªn S¬n B¶o Lé
Include("\\script\\item\\luascript\\head.lua")
TV_THIENSOWNBAOLO = 2019;

function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local _, nD, _ = GetItemProp(nIdx)
	local nCurTime = GetTask(TV_THIENSOWNBAOLO)
	local nTimeAdd = 30*60*18
	if(nCurTime > 0) then
		Say("Cßn <color=red>"..floor(nCurTime/18).." gi©y<color> may m¾n, kh«ng thÓ sö dông <color=green> Thiªn s¬n b¶o lé <color>.", 0)
		return 0
	end
	AddSkillState(441, 1, 1, nTimeAdd)
	SetTask(TV_THIENSOWNBAOLO, nTimeAdd)
	Msg2Player("§¹i hiÖp ®· uèng <color=green> Thiªn S¬n B¶o Lé <color>.")
	return 1
end