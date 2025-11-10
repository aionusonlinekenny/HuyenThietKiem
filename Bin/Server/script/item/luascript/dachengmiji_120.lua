-- Author:	Kinnox
-- Date:	15-11-2022
-- §¹i thµnh bÝ kÝp 12x
Include("\\script\\global\\luanpcfactions\\faction_head.lua")


function OnUse(nIdx)
	if(GetHandItem() > 0) then
		Talk(1,"",10267)
		return 0
	end
	
	if(GetLevel() < 120) then
		Talk(1,"","§¼ng cÊp d­íi 120 kh«ng thÓ häc kü n¨ng nµy")
		return 0
	end
	local nFactionID = GetFactionNo() + 1
	if(nFactionID == 0) then
		Talk(1,"","Ch­a gia nhËp m«n ph¸i kh«ng thÓ häc kü n¨ng nµy.")
		return 0
	end
	local tbSay = {}
	local i
	tinsert(tbSay, format("%s./OnLearn",Faction.TFACTION_INFO[nFactionID].tbSkill120[1]))
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
	if(not Faction.TFACTION_INFO[nFactionID].tbSkill120) then
		Talk(1,"","Cã lçi x¶y ra vui lãng b¸o GM.")
		return
	end
	local nItemIdx = GetTaskTemp(0)

	local nSkillID = Faction.TFACTION_INFO[nFactionID].tbSkill120[2]
	if( HaveMagic(nSkillID) > 19 ) then
		Talk(1,"","Ng­¬i ®· lÜnh héi kü n¨ng nµy råi, kh«ng thÓ tiÕp tôc sö dông bÝ quyÕt.")
		return
	end

	if(DelItemByIndex(nItemIdx) == 0) then
		Say(10002,0)
		return
	end	
	
	AddMagic(nSkillID, 20)
	Talk(1,"","Nghiªn cøu c¶ ngµy, cuèi cïng ®· linh héi ®­îc kü n¨ng "..Faction.TFACTION_INFO[nFactionID].tbSkill120[1]..".")
end

function OnExit()

end;