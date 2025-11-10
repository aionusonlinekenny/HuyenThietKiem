----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Hµnh Qu©n §¬n
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add462() 
	local level = random(2,10)
	AddSkillState(462, level, 1, 12)
	Msg2Player("<color=OrangeRed>§¹i hiÖp nhËn ®­îc nöa gi©y håi phôc "..(level*500).." ®iÓm sinh lùc.<color>") 
end 

----------------------------------
--
----------------------------------
function add464() 
	local level = random(1,10) 
	AddSkillState(464, level, 1, 540)
	Msg2Player("<color=OrangeRed>§¹i hiÖp nhËn ®­îc 30 gi©y håi phôc "..(level*100).." ®iÓm sinh lùc.<color>") 
end 

----------------------------------
--
----------------------------------
function add488() 
	local level = random(10,20) 
	AddSkillState(488, level, 1, 540)
	Msg2Player("<color=OrangeRed>§¹i hiÖp nhËn ®­îc 30 gi©y t¨ng "..(level*100).." ®iÓm sinh lùc tèi ®a.<color>") 
end 

Tab={add462,add464,add488} 

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