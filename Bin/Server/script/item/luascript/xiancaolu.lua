-- Author:	Kinnox
-- Date:	31-05-2021
-- Tiªn Th¶o Lé
Include("\\script\\item\\luascript\\head.lua")


function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local _, nD, _ = GetItemProp(nIdx)
	local nCurTime = GetTask(2015)
	if(nCurTime > 0) then
		Say("Cßn <color=red>"..floor(nCurTime/18).." gi©y<color> nh©n 2 kinh nghiÖm, kh«ng thÓ sö dông <color=green>"..ZUseItem.tbExpTime[nD][2].." <color>.", 0)
		return 0
	end
	local nTimeAdd = ZUseItem.tbExpTime[nD][1]*60*18
	SetTask(2015, nTimeAdd)
	AddSkillState(440, 1, 1, nTimeAdd)
	Msg2Player("§¹i hiÖp ®· uèng <color=green>"..ZUseItem.tbExpTime[nD][2].." <color>.")
	return 1
end