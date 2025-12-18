	
function OnUse(nIdx)
	local TAB_TIENTHAOLO = {
		"B¹n hiÖn t¹i kh«ng bŞ ©m kinh nghiÖm kh«ng thÓ sö dông.",
		"B¹n ®· tÈy ®­îc ®iÓm ©m kinh nghiÖm",
			}
	local aHeo = GetExp()
	if aHeo < 0 then
		local aHeo1 = aHeo*(-1)
		AddOwnExp(aHeo1)
		Talk(1,"",TAB_TIENTHAOLO[2])
		return 1
	else
		Talk(1,"",TAB_TIENTHAOLO[1])
		return 0
	end
	return 1
end