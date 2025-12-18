-- Author:	Kinnox
-- Date:	31-05-2021
-- QuÕ Hoa Töu
Include("\\script\\item\\luascript\\head.lua")
TV_QUEHOATUU = 2018;
function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	local nName = GetName()
	local nTeamSize = GetTeamSize()
	local nCurTime = GetTask(TV_QUEHOATUU)
	local nTimeAdd = 32400
	if(nCurTime > 0) then
		Say("Cßn <color=red>"..floor(nCurTime/18).." gi©y<color> may m¾n, kh«ng thÓ sö dông <color=green> QuÕ hoa töu <color>.", 0)
		return 0
	end

	if(nTeamSize > 1) then -- cai nay co team
		for i = 1, nTeamSize do
			PlayerIndex = GetTeamMember(i);
			if PlayerIndex and PlayerIndex > 0 then 
				Msg2Player(""..nName.." dïng <color=green>QuÕ Hoa Töu <color>.")
				AddSkillState(450, 1, 1, nTimeAdd)			
				SetTask(TV_QUEHOATUU, nTimeAdd)
			end
		end
	else -- cai nay danh 1 minh
		Msg2Player("§¹i hiÖp ®· uèng <color=green>QuÕ Hoa Töu <color>.")
		AddSkillState(450, 1, 1, nTimeAdd)
		SetTask(TV_QUEHOATUU, nTimeAdd)
	end
	return 1
end
