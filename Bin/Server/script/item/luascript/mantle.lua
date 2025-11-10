----------------------------------
--	Copyright: JX Online by Blood Team
--	Author: [S]
--	Date: 16/08/2014
--	Desc: Phi phong
----------------------------------

----------------------------
--
----------------------------
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local nIdAmor = GetEquipItem(1)
	if(nIdAmor <= 0) then
		Talk(1,"",10395)
		return 0
	end
	local nG, nD, nP, nL = GetItemProp(nIdAmor)
	if(nG > 1) then
		Talk(1,"",10396)
		return 0
	end
	
	if(nP > 13) then
		Talk(1,"",10397)
		return 0
	end
	nP = nP + 14
	local nRow = nP * 10 + nL - 1;
	SetItemParticular(nIdAmor, nP)
	SetItemRecord(nIdAmor, nRow)
	if(DelMyItem(nIdAmor) ~= 0) then
		AddMyItem(nIdAmor,2,1,0)
		Talk(1,"",10398)
		return 1
	end
	return 0	
end