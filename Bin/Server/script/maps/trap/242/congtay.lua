-- Author:	SkyGold
-- Date:	15-04-2016
-- CÊng Bæc Bi÷n Kinh


function main()
	if(GetFightState() == 0) then
		SetPos(1582, 3163)
		SetFightState(1)
	else	
		SetPos(1591, 3175)	
		SetFightState(0)
	end;
	-- AddStation(10)
	-- SetProtectTime(18*3)
	AddSkillState(963, 1, 0, 18*3)
end
