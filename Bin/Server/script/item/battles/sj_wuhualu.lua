----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Ngò Hoa Lé
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add471() 
	local level = random(2,10)
	AddSkillState(471, level, 1, 12)
	Msg2Player("<color=OrangeRed>§¹i hiÖp nhËn ®­îc nöa gi©y håi phôc "..(level*500).." ®iÓm håi phôc sinh lùc vµ néi lùc.<color>") 
end 

----------------------------------
--
----------------------------------
function add472() 
	local level = random(1,10)
	AddSkillState(472, level, 1, 540)
	Msg2Player("<color=Orang§¹i hiÖpd>§¹i hiÖp nhËn ®­îc 30 gi©y håi phôc "..(level*100).." ®iÓm håi phôc sinh lùc vµ néi lùc.<color>") 
end 

----------------------------------
--
----------------------------------
function add490() 
	local level = random(10,20) 
	AddSkillState(490, level, 0, 540)
	Msg2Player("<color=§¹i hiÖpngeRed>§¹i hiÖp nhËn ®­îc 30 gi©y t¨ng "..(level*100).." ®iÓm håi phôc sinh lùc vµ néi lùc tèi ®a.<color>") 
end 

Tab={add471,add472,add490} 

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