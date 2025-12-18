-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Prison Ba Lang Huyen;
Include("\\script\\system_config.lua")
PrisonNpc = {};
szHello = "<color=orange>Bæ ®Çu cai ngôc:<color> Ta cã thÓ ®­a c¸c ng­¬i ®i th¨m nh÷ng ph¹m nh©n, nh­ng sÏ tèn chi phÝ cho cai ngôc. 1 v¹n l­îng";
function main(nNpcIdx)
 	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/prinson.lua");
		Msg2Player("Reload this script");
	end;
PrisonNpc:PrisonNpc()
end;

function PrisonNpc:PrisonNpc()
	local tbSay = {}
	tbSay = {
		"Mong bæ ®Çu gióp ®ì ®­a ta ®i/MovePrison",
		"Ta muèn thay ®æi tr¹ng th¸i PK/ChangeCamp",
		"Ta ®Õn trß chuyÖn víi ng­¬i th«i/OnCancel",
	}
	Say(szHello,getn(tbSay),tbSay);
end;

function MovePrison()
	local nCash = GetCash();
	local nNeed =  1E4;
	if (nCash < nNeed) then
		Talk(1,"","ChØ cã 1 v¹n l­îng b¹c ®Ó lµm viÖc ng­¬i cÇn mµ ng­¬i còng kh«ng bá ra sao?.");
		return
	end
	Pay(nNeed)
	NewWorld(208,1585,3210);
	SetFightState(0);
	Talk(1,"","Ng­¬i ®· ®Õn n¬i, h·y mau lªn, quan viªn s¾p ®Õn ®©y.");
end;


----------------------------
--
----------------------------
function ChangeCamp()
	local sInfo = "<color=orange>Bæ ®Çu cai ngôc:<color> Xin mêi<color=green> "..GetName().." <color>lùa chän!, ng­êi giÕt ng­êi sÏ bÞ ta nhèt l¹i\n20v¹n/lÇn ®æi "
	local tbSay = {
			--"T©n thñ./ChangeCamp_1#1",
			"ChÝnh ph¸i./ChangeCamp_1",
			"Tµ ph¸i./ChangeCamp_1",
			"Trung lËp./ChangeCamp_1",
			"S¸t thñ./ChangeCamp_1",
			"Tho¸t ra./ExitFunc",
		}
	SayImage(sInfo,"44/44/15",getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function ChangeCamp_1(nCamp)
	local nCamp = nCamp + 1 ;
	if(GetCamp() == 0) then
		Talk(1,"","T©n thñ kh«ng thÓ ®æi.")
		return
	end
	if(GetTongID() > 0) then
		Talk(1,"","Thµnh viªn bang héi kh«ng thÓ ®æi.")
		return
	end
	if (GetCash() < 200000) then
		Talk(1,"","Ng­¬i kh«ng cã tiÒn kh«ng thÓ ®æi sang kiÓu kh¸c!")
		return
	end;
	Pay(200000);
	nCamp = tonumber(nCamp)
	SetCurCamp(nCamp)
	SetCamp(nCamp)
end


function OnCancel()
end;
