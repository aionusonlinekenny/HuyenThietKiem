----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Tam Thanh Chi Kh›
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add505() 
	CastSkill(505, 1)
	Msg2Player("<color=OrangeRed>ßπi hi÷p nhÀn Æ≠Óc Tam Thanh Chi Kh›, khi’n kŒ dﬁch xung quanh bﬁ h´n m™.<color>") 
end 

----------------------------------
--
----------------------------------
function add506() 
	CastSkill(506, 1)
	Msg2Player("<color=OrangeRed>ßπi hi÷p nhÀn Æ≠Óc Tam Thanh Chi Kh›, khi’n kŒ dﬁch xung quanh bﬁ Æ„ng b®ng.<color>") 
end 

----------------------------------
--
----------------------------------
function add507() 
	CastSkill(507, 1)
	Msg2Player("<color=OrangeRed>ßπi hi÷p nhÀn Æ≠Óc Tam Thanh Chi Kh›, khi’n kŒ dﬁch xung quanh bﬁ trÛng ÆÈc.<color>") 
end 

----------------------------------
--
----------------------------------
function add508() 
	CastSkill(508, 1)
	Msg2Player("<color=OrangeRed>ßπi hi÷p nhÀn Æ≠Óc Tam Thanh Chi Kh›, khi’n kŒ dﬁch xung quanh bﬁ gi∂m tËc ÆÈ di chuy”n.<color>") 
end 

Tab={add505,add506,add507,add508} 

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