----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Háa Chi ChiÕn Hån
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add466() 
	local level = random(1,4)
	AddSkillState(466, level, 1, 1080) 
	Msg2Player("<color=OrangeRed>§¹i hiÖp ®· nhËn ®­îc 1 phót gi¶m "..(level*10).." ®iÓm thêi gian phôc håi.<color>")
end 

----------------------------------
--
----------------------------------
function add468() 
	level = random(1,4); 
	AddSkillState(468, level, 1, 1080) 
	Msg2Player("<color=OrangeRed>§¹i hiÖp ®· nhËn ®­îc 1 phót gi¶m "..(level*10).." ®iÓm thêi gian lµm chËm.<color>")
end

Tab={add466,add468} 

----------------------------------
--
----------------------------------
function PickUp(nItemIndex, nPlayerIndex)
	PlayerIndex = nPlayerIndex;
	if( IsMyItem(nItemIndex) ) then
		local i = random(getn(Tab))
		Tab[i]()
		local nCount = GetTask(ZBattle.tbTask.GET_ITEM) + 1
		SetTask(ZBattle.tbTask.GET_ITEM, nCount)
		SetTask(ZBattle.tbTask.ITEM_POINT, nCount * ZBattle.ITEM_BONUS_POINT)
		SetTask(ZBattle.tbTask.TEMP_POINT, GetTask(ZBattle.tbTask.TEMP_POINT) + ZBattle.ITEM_BONUS_POINT)
		local nCamp = GetCurCamp()
		if(nCamp == 1) then
			SetMissionV(ZBattle.tbMission.SONGPOINT, GetMissionV(ZBattle.tbMission.SONGPOINT) + ZBattle.ITEM_BONUS_POINT)
		elseif(nCamp == 2) then
			SetMissionV(ZBattle.tbMission.JINPOINT, GetMissionV(ZBattle.tbMission.JINPOINT) + ZBattle.ITEM_BONUS_POINT)
		end
		SyncTask(ZBattle.tbTask.GET_ITEM)
		SyncTask(ZBattle.tbTask.ITEM_POINT)
		SyncTask(ZBattle.tbTask.TEMP_POINT)
		return 1
	end
end