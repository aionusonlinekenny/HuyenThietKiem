-- Author:	SkyGold
-- Date:	15-09-2015
-- HuÖ TuyÒn Töu (kü n¨ng)
Include("\\script\\item\\head.lua")


function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local _, nD, _ = GetItemProp(nIdx)
	local nCurTime = GetTask(2014)
	if(nCurTime >= 1 ) then
		Say("Cßn <color=red>"..floor(nCurTime/18).." gi©y<color> nh©n 2 kinh nghiÖm kü n¨ng, kh«ng thÓ sö dông <color=green>N÷ Nhi Hång<color> them.", 0)
		return 0
	end
	
	SetTask(2014, (8*60*60*18) )
	Msg2Player("B¹n ®· uèng <color=green>N÷ Nhi Hång<color>.")
	return 1
end