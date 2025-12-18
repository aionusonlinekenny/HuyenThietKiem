-- Author:	Kinnox
-- Date:	31-05-2021
-- Phóc Duyªn Lé (tiÓu) (trung) (§¹i)
Include("\\script\\item\\luascript\\head.lua")

function OnUse(nIdx)  
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local _, nD, _ = GetItemProp(nIdx)
	AddFuyuan(ZUseItem.tbFuYuan[nD][1])
	return 1
end