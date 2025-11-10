-- Author:	SkyGold
-- Date:	15-04-2016
-- CÊng Bæc Bi÷n Kinh


function main()
	if(GetFightState() == 0) then
		SetPos(1656, 3242)
		SetFightState(1)
	else	
		SetPos(1648, 3233)	
		SetFightState(0)
	end;
	-- AddStation(10)
	-- SetProtectTime(18*3)
	AddSkillState(963, 1, 0, 18*3)
end