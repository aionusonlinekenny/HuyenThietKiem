----------------------------------
--	Copyright: JxOnline by kinnox;
--	Author: [Kinnox]
--	Date: 15/11/2021
--	Desc: Script Support Char
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\faction_head.lua")
Include("\\script\\lib\\admin\\point.lua")

ZFACTION = {
	[1] = {
		{"ThiÕu L©m Ph¸i",1},
		{"Thiªn V­¬ng Bang",2},
	},
	[2] = {
		{"§­êng M«n",3},
		{"Ngò §éc Gi¸o",4},	
	},
	[3] = {
		{"Nga My Ph¸i",5},
		{"Thóy Yªn M«n",6},
		--{"Hoa S¬n ph¸i",11},
	},
	[4] = {
		{"C¸i Bang",7},
		{"Thiªn NhÉn Gi¸o",8},
	},
	[5] = {
		{"Vâ §ang Ph¸i",9},
		{"C«n L«n Ph¸i",10},
	},
}
----------------------------
--
----------------------------
function GM_Char()
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Gia nhËp m«n ph¸i./GM_GoFaction",
			"Häc kü n¨ng 90./GM_LearnSkill#80",
			"Häc kü n¨ng 120./GM_LearnSkill#120",
			"§æi mµu PK./GM_ChangeCamp",
			--"TiÕn Hµnh TÈy Tñy/GM_Reset",
			"Phôc håi m¸u & mana/GM_Restore",
			--"NhËn ®iÓm tiÒm n¨ng/GM_Point",
			--"NhËn ®iÓm kü n¨ng/GM_Point",
			"Th¨ng §¼ng cÊp 150./PL_Level",
			"Th¨ng 10 cÊp [Tèi ®a 150]./PL_Exp",
			"NhËn 1000 v¹n l­îng./PL_Cash",
			"NhËn 1 v¹n xu./PL_Coin",
			"Th¨ng Tµi l·nh ®¹o./PL_LeadLevel",
			"Th¨ng Danh väng./PL_Repute",
			"Th¨ng Phóc duyªn./PL_Fuyuan",
			"KÕt thóc ®èi tho¹i./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

function GM_Reset()
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"TÈy ®iÓm TiÒm N¨ng/OnClearProp",
			"TÈy ®iÓm Kü N¨ng/OnClearMagic",
			"TÈy tÊt c¶ c¸c ®iÓm/OnClearAll",
			"KÕt thóc ®èi tho¹i./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end;
----------------------------
--
----------------------------
function GM_GoFaction()
	if(GetFactionNo() >= 0) then
		Talk(1,"","Ng­¬i ®· ë trong m«n ph¸i.")
		return
	end
	local sInfo = "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {}
	local i = GetSeries() + 1
	for j = 1, getn(ZFACTION[i]) do
		tinsert(tbSay, ZFACTION[i][j][1].."/GoFactionA#"..ZFACTION[i][j][2])
	end
	tinsert(tbSay, "Trë vÒ./GM_Char")
	tinsert(tbSay, "KÕt thóc ®èi tho¹i./ExitFunc")
	
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GoFactionA(nSel,nFactionID)
	nFactionID = tonumber(nFactionID)
	if(type(nFactionID) ~= "number") then return end
	if( (nFactionID < 1) or (nFactionID > 11)) then return end
	return Faction:JoinFaction(nFactionID)
end

----------------------------
--
----------------------------
function GM_LearnSkill(nSel,nLevel)
	nLevel = tonumber(nLevel)
	if(type(nLevel) ~= "number") then return end
	
	local nFactionID = GetFactionNo() + 1
	if(nFactionID == 0) then
		Talk(1,"","Ch­a gia nhËp m«n ph¸i kh«ng thÓ häc kü n¨ng.")
		return
	end
	local tbMessage = {
			"<color=green>Chóc mõng ®¹i hiÖp ®· häc ®­îc kü n¨ng "..nLevel.." "..Faction.TFACTION_INFO[nFactionID].sFactionName..".",
			"Ng­¬i ®· lÜnh héi kü n¨ng nµy råi!",
			"H·y luyÖn lªn ®¼ng cÊp "..nLevel.." ®·.",
		}
	
	if(GetLevel() < nLevel) then
		Msg2Player(tbMessage[3])
		return
	end
		
	if(nLevel == 80) then
		if (HaveMagic(Faction.TFACTION_INFO[nFactionID].tbSkill90[1][2]) >= 20) then
			Msg2Player(tbMessage[2])
			return
		end
		local nCount = getn(Faction.TFACTION_INFO[nFactionID].tbSkill90)
		local nMagicLevel = 20
		for i = 1, nCount do
			if(nFactionID > 2 and nCount > 2) then
				if(i == nCount) then
					nMagicLevel = 0
				end
			end
			AddMagic(Faction.TFACTION_INFO[nFactionID].tbSkill90[i][2], Faction.TFACTION_INFO[nFactionID].tbSkill90[i][3])
		end
		Msg2Player(tbMessage[1])
		
	elseif(nLevel == 120) then
		if (HaveMagic(Faction.TFACTION_INFO[nFactionID].tbSkill120[2]) >= 20) then
			Msg2Player(tbMessage[2])
			return
		end 
		AddMagic(Faction.TFACTION_INFO[nFactionID].tbSkill120[2], Faction.TFACTION_INFO[nFactionID].tbSkill120[3])
		Msg2Player(tbMessage[1])
	end
end

----------------------------
--
----------------------------
function GM_ChangeCamp()
	local sInfo = "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {
			"T©n thñ./GM_ChangeCamp_1#1",
			"ChÝnh ph¸i./GM_ChangeCamp_1#2",
			"Tµ ph¸i./GM_ChangeCamp_1#3",
			"Trung lËp./GM_ChangeCamp_1#4",
			"S¸t thñ./GM_ChangeCamp_1#5",
			"Trë vÒ tr­íc./WPlayer_Char",
			"Tho¸t ra./ExitFunc",
		}
	SayImage(sInfo,"44/44/15",getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GM_ChangeCamp_1(nCamp)
	-- if(GetCamp() == 0) then
		-- Talk(1,"","T©n thñ kh«ng thÓ ®æi.")
		-- return
	-- end
	-- if(GetTongID() > 0) then
		-- Talk(1,"","Thµnh viªn bang héi kh«ng thÓ ®æi.")
		-- return
	-- end
	nCamp = tonumber(nCamp)
	SetCurCamp(nCamp)
	SetCamp(nCamp)
end
----------------------------
--
----------------------------
function GM_Restore()
	RestoreLife();
	RestoreMana();
end;
----------------------------
--
----------------------------
function GM_Point(nSel)

	if (nSel == 6) then
		AddProp(5000);
	elseif (nSel == 7) then
		AddMagicPoint(1000);
	end;
	KickOutSelf();
end;
----------------------------
--
----------------------------
function ExitFunc()
end;