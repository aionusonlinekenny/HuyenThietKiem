-- Author:	Kinnox
-- Date:	14-05-2022
-- Tich tinh dong

function main()
	if(GetFightState() == 0) then
		SetPos(1169, 2401);
		SetFightState(1)
	else	
		SetPos(1164, 2395);	
		SetFightState(0)
	end;
	AddSkillState(963, 1, 0, 18*3)
end