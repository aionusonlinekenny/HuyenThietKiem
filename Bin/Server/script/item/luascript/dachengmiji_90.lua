-- Author:	Kinnox
-- Date:	15-11-2022
-- §¹i thµnh bİ kİp 9x
Include("\\script\\global\\luanpcfactions\\faction_head.lua")


function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	
	if(GetLevel() < 80) then
		Talk(1,"","§¼ng cÊp d­íi 80 kh«ng thÓ häc ®¹i thµnh bİ kİp")
		return 0
	end
	local nFactionID = GetFactionNo() + 1
	if(nFactionID == 0) then
		Talk(1,"","Ch­a gia nhËp m«n ph¸i kh«ng thÓ häc ®¹i thµnh bİ kİp.")
		return 0
	end
	local tbSay = {}
	local i
	for i = 1, getn(Faction.TFACTION_INFO[nFactionID].tbSkill90) do
		if(nFactionID == 3) then
			if(i > 4) then
				break
			end
		elseif (nFactionID == 0) or (nFactionID == 1) or (nFactionID == 2) or (nFactionID == 4) or (nFactionID == 5) or (nFactionID == 8) or (nFactionID == 10)  then
			if (i > 3 ) then
				break
			end;
		else
			if(i > 2) then
				break
			end
		end
		tinsert(tbSay, format("%s./OnLearn",Faction.TFACTION_INFO[nFactionID].tbSkill90[i][1]))
	end
	tinsert(tbSay, "Kh«ng muèn häc./OnExit")
	SetTaskTemp(0, nIdx)
	Say("Xin mêi lùa chän kü n¨ng cÇn häc",getn(tbSay), tbSay)
	return 0
end


function OnLearn(nSel)
	local i = tonumber(nSel) + 1
	local nFactionID = GetFactionNo() + 1
	if(nFactionID == 0) then
		Talk(1,"","Cã lçi x¶y ra vui lãng b¸o GM.")
		return
	end
	if(not Faction.TFACTION_INFO[nFactionID].tbSkill90[i]) then
		Talk(1,"","Cã lçi x¶y ra vui lãng b¸o GM.")
		return
	end
	local nItemIdx = GetTaskTemp(0)
	if(nFactionID == 3) then
		if(i > 4) then
			Talk(1,"","Kü n¨ng nµy kh«ng thÓ häc ®¹i thµnh bİ kİp.")
			return
		end
	elseif (nFactionID == 1) or (nFactionID == 2) or (nFactionID == 4) or (nFactionID == 5) or (nFactionID == 8) or (nFactionID == 10)  then
		if (i > 3 ) then
			Talk(1,"","Kü n¨ng nµy kh«ng thÓ häc ®¹i thµnh bİ kİp.")
			return
		end;		
	else
		if(i > 2) then
			Talk(1,"","Kü n¨ng nµy kh«ng thÓ häc ®¹i thµnh bİ kİp.")
			return
		end
	end
	local nSkillID = Faction.TFACTION_INFO[nFactionID].tbSkill90[i][2]
	if( HaveMagic(nSkillID) >= 20 ) then
		Talk(1,"","Kü n¨ng nµy ®· ®¹ ®¼ng cÊp tèi ®a, kh«ng thÓ häc ®¹i thµnh bİ kİp.")
		return
	end

	if(DelItemByIndex(nItemIdx) == 0) then
		Say(10002,0)
		return
	end	
	
	AddMagic(nSkillID, 20)
	Talk(1,"","§· häc ®¹i thµnh bİ kİp thµnh c«ng.")
end

function OnExit()

end;