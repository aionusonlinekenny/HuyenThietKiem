----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Thñy Chi Lîi NhËn
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add481() 
	local level = random(10,20) 
	AddSkillState(481, level, 1, 1080)
	Msg2Player("<color=OrangeRed>§¹i hiÖp ®· nhËn ®­îc 1 phót t¨ng "..(level*10).." ®iÓm b¨ng s¸t néi c«ng.<color>") 
end 

----------------------------------
--
----------------------------------
function add486() 
	local level = random(1,10) 
	AddSkillState(486, level, 1, 1080)
	Msg2Player("<color=OrangeRed>§¹i hiÖp ®· nhËn ®­îc 1 phót t¨ng "..(level*10).." ®iÓm b¨ng s¸t ngo¹i c«ng.<color>") 
end 

Tab={add481,add486} 

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