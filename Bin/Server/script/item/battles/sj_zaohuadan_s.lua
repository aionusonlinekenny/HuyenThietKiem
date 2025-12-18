 
function OnUse(nIndex)
	local nW = GetWorldPos()
	if nW == 378 or nW == 379 or nW == 380 then
		AddSkillState(472, 5, 1, 54 ) 
		Msg2Player("§¹i hiÖp sö dông 1 Cµn Kh«n T¹o Hãa ®an (Võa) .")	
		return 1
	end
	return 0
end

