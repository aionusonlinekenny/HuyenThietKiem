-- cái này là mem ch?t di song lai no se chay ham nay ngay khi song lai, thich lam gi thi lam
function main()
	-- Restore Tien Thao lo
	local nTimeExpEnhance = GetTask(2015)
	if(nTimeExpEnhance > 0) then
		AddSkillState(440, 1, 1, nTimeExpEnhance)
	end
	-- Vong sang ho tro tan thu
	if (GetLevel() < 80) then
		AddSkillState(1583, 20, 1, 0);
		AddSkillState(1583, 20, 1, 24*60*60*18)
	end;
	-- Vong Sang Loan Chien
	if (GetRank() == 193) then
		AddSkillState(1584, 20, 1, 24*60*60*18)
	elseif (GetRank() == 194) then
		AddSkillState(1585, 20, 1, 24*60*60*18)
	elseif (GetRank() == 195) then
		AddSkillState(1586, 20, 1, 24*60*60*18)
	end;	
end