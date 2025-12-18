----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: ThÇn bÝ vËt phÈm
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function add462() 
level = random(2,10); 
AddSkillState(462, level, 0, 12 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi nöa gi©y ®Ých l­îng m¸u håi phôc "..(level*500).."<#> ®iÓm ") 
end 


function add463() 
level = random(2,10); 
AddSkillState(463, level, 0, 12 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i néi lùc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi nöa gi©y ®Ých néi lùc håi phôc "..(level*500).."<#> ®iÓm ") 
end 

function add464() 
level = random(1,10); 
AddSkillState(464, level, 0, 540 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u kÐo dµi kh«i phôc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 30 gi©y ®Ých l­îng m¸u håi phôc , mçi nöa gi©y håi phôc "..(level*100).."<#> ®iÓm ") 
end 

function add465() 
level = random(1,10); 
AddSkillState(465, level, 0, 540 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i néi lùc kÐo dµi håi phôc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 30 gi©y ®Ých néi lùc håi phôc , mçi nöa gi©y håi phôc "..(level*100).."<#> ®iÓm ") 
end 

function add466() 
level = random(1,4); 
AddSkillState(466, level, 0, 1080 ) 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót bÞ th­¬ng ®éng t¸c gi¶m bít #"..(level*10).."<#>#") 
end 

function add467() 
level = random(1,4); 
AddSkillState(467, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  nhµ ch¬i ®¹t ®­îc tróng ®éc thêi gian gi¶m bít 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót tróng ®éc thêi gian gi¶m bít #"..(level*10).."<#>#") 
end 

function add468() 
level = random(1,4); 
AddSkillState(468, level, 0, 1080 ) 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót chËm l¹i thêi gian gi¶m bít #"..(level*10).."<#>#") 
end 

function add469() 
level = random(1,4); 
AddSkillState(469, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  nhµ ch¬i ®¹t ®­îc h«n mª thêi gian gi¶m bít 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót h«n mª thêi gian gi¶m bít #"..(level*10).."<#>#") 
end 

function add471() 
level = random(2,10); 
AddSkillState(471, level, 0, 12 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u cïng néi lùc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi nöa gi©y ®Ých l­îng m¸u håi phôc "..(level*500).."<#> ®iÓm cïng kÐo dµi nöa gi©y ®Ých néi lùc håi phôc "..(level*500).."<#> ®iÓm ") 
end 

function add472() 
level = random(1,10); 
AddSkillState(472, level, 0, 540 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u cïng néi lùc kÐo dµi kh«i phôc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 30 gi©y ®Ých l­îng m¸u håi phôc , mçi nöa gi©y håi phôc "..(level*100).."<#> ®iÓm cïng kÐo dµi 30 gi©y ®Ých néi lùc håi phôc , mçi nöa gi©y håi phôc "..(level*100).."<#> ®iÓm ") 
end 

function add473() 
level = random(1,7); 
AddSkillState(473, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i phæ phßng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót phæ phßng # gia t¨ng "..(level*10).."<#>#") 
end 

function add474() 
level = random(1,7); 
AddSkillState(474, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i ®éc phßng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót ®éc phßng # gia t¨ng "..(level*10).."<#>#") 
end 

function add475() 
level = random(1,7); 
AddSkillState(475, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i b¨ng phßng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót b¨ng phßng # gia t¨ng "..(level*10).."<#>#") 
end 

function add476() 
level = random(1,7); 
AddSkillState(476, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i löa phßng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót löa phßng # gia t¨ng "..(level*10).."<#>#") 
end 

function add477() 
level = random(1,7); 
AddSkillState(477, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l«i phßng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót l«i phßng # gia t¨ng "..(level*10).."<#>#") 
end 

function add478() 
level = random(1,4); 
AddSkillState(478, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i toµn kh¸ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót tÊt c¶ kh¸ng tÝnh # gia t¨ng "..(level*10).."<#>#") 
end 

function add479() 
level = random(10,20); 
AddSkillState(479, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn trong phæ tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót néi c«ng hÖ phæ c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add480() 
level = random(1,5); 
AddSkillState(480, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn trong ®éc tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót néi c«ng hÖ ®éc c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add481() 
level = random(10,20); 
AddSkillState(481, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn trong b¨ng tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót néi c«ng hÖ b¨ng c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add482() 
level = random(10,20); 
AddSkillState(482, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i néi háa tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót néi c«ng hÖ löa c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add483() 
level = random(10,20); 
AddSkillState(483, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn trong l«i tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót néi c«ng hÖ l«i c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add485() 
level = random(1,5); 
AddSkillState(485, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn ngoµi ®éc tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót ngo¹i c«ng hÖ ®éc c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add486() 
level = random(1,4); 
AddSkillState(486, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn ngoµi b¨ng tæn th­¬ng 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót ngo¹i c«ng hÖ b¨ng c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#> ®iÓm ") 
end 

function add487() 
level = random(5,10); 
AddSkillState(487, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i bªn ngoµi phæ tæn th­¬ng phÇn tr¨m so 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót ngo¹i c«ng hÖ phæ c«ng tæn th­¬ng # gia t¨ng "..(level*10).."<#>#") 
end 

function add488() 
level = random(10,20); 
AddSkillState(488, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u th­îng h¹n 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 1 phót l­îng m¸u th­îng h¹n gia t¨ng #"..(level*100).."<#> ®iÓm ") 
end 

function add489() 
level = random(10,20); 
AddSkillState(489, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i néi lùc th­îng h¹n 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 1 phót néi lùc th­îng h¹n gia t¨ng #"..(level*100).."<#> ®iÓm ") 
end 

function add490() 
level = random(10,20); 
AddSkillState(490, level, 0, 540 ) -- script viet hoa By http://tranhba.com  gia t¨ng nhµ ch¬i l­îng m¸u cïng néi lùc th­îng h¹n 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7> kÐo dµi 30 gi©y ®Ých néi lùc th­îng h¹n gia t¨ng #"..(level*100).."<#> ®iÓm cïng kÐo dµi 30 gi©y ®Ých néi lùc th­îng h¹n gia t¨ng #"..(level*100).."<#> ®iÓm ") 
end 

function add493() 
level = random(1,3); 
AddSkillState(493, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  nhµ ch¬i ®¹t ®­îc gia t¨ng ch¹y tèc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót tèc ®é di ®éng # gia t¨ng "..(level*10).."<#>#") 
end 

function add495() 
level = random(10,20); 
AddSkillState(495, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  nhµ ch¬i ®¹t ®­îc gia t¨ng ch¹y tèc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót c«ng kÝch mÖnh trung tû sè # gia t¨ng "..(level*10).."<#>#") 
end 

function add496() 
level = random(1,2); 
AddSkillState(496, level, 0, 1080 ) -- script viet hoa By http://tranhba.com  nhµ ch¬i ®¹t ®­îc gia t¨ng ch¹y tèc 
Msg2Player("<#> ng­¬i thu ®­îc <color=0xB5FDD7>1 phót vâ c«ng ra chiªu ®éng t¸c # gia t¨ng "..(level*10).."<#>#") 
end 

Tab={add462,add463,add464,add465,add466,add467,add468,add469,add471,add472,add473,add474,add475,add476,add477,add478,add479,add480,add481,add482,add483,add485,add486,add487,add488,add489,add490,add493,add495,add496} 

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