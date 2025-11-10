----------------------------------
--	Copyright: JX Online by HAN Team
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Tèng Kim Chiªu Th­
----------------------------------
Include("\\script\\mission\\battles\\head.lua")

----------------------------------
--
----------------------------------
function OnUse(nItemIdx)
	local W,_,_ = GetWorldPos()
	if(W == ZBattle.SIGN_MAP) then
		Talk(1,"","Kh«ng thÓ sö dông vËt phÈm t¹i b¶n ®å nµy!")
		return 0
	end
	if(W == ZBattle.FIGHT_MAP) then
		Talk(1,"","Khu vùc nµy kh«ng thÓ sö dông <color=green>Tèng Kim chiªu th­ <color>!")
		return 0
	end
	local tbSay = {
			"B¸o danh bªn Tèng./JoinSongJin",
			"B¸o danh bªn Kim./JoinSongJin",
			"§Ó suy nghÜ lai./OnCancel",
		}
	SetTaskTemp(0, nItemIdx)
	Say("ChiÕn tr­êng lµ n¬i khèc liÖt cã thÓ cã ®i mµ kh«ng cã vÒ! Ng­¬i c¶m thÊy chÝnh m×nh cã ®ñ can ®¶m vµ søc lùc ®Ó lªn ®­êng hay kh«ng? Nªn nhí cÊp ®é tèi thiÓu ®Ó chÞu ®ùng ®­îc ë n¬i nµy lµ <color=green>80 cÊp<color>!", getn(tbSay), tbSay)
	return 0
end

----------------------------------
--
----------------------------------
function JoinSongJin(nSel)
	if(GetLevel() < 80) then
		Say("§¼ng cÊp ph¶i ®¹t tõ <color=red>80 trë lªn<color> míi cã thÓ tham gia chiÕn tr­êng <color=lightsalmon>Tèng<color> <color=yellow>Kim<color>.",0)
		return
	end
	local nItemIdx = GetTaskTemp(0)
	if (nItemIdx == 0) then
		Talk(1,"","VËt phÈm lçi vui lßng b¸o víi GM!");
		return
	end;
	local nCamp = nSel + 1
	NewWorld(ZBattle.SIGN_MAP, ZBattle.tbSignMapPos[nCamp][1], ZBattle.tbSignMapPos[nCamp][2])
	SetFightState(0)
	if(nCamp == 1) then
		Msg2Player("<color=orangered>§¹i hiÖp ®· vµo khu vùc b¸o danh phe <color=lightsalmon>Tèng<color>.")
	else
		Msg2Player("<color=orangered>§¹i hiÖp ®· vµo khu vùc b¸o danh phe <color=yellow>Kim<color>.")
	end
	DelItemByIndex(nItemIdx)
end

----------------------------------
--
----------------------------------
function OnCancel()
end