-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Prison;
Include("\\script\\system_config.lua")
PrisonNpc = {};
szHello = "<color=orange>Lao ®Çu:<color> Ta ®· qu¶n ngôc h¬n n÷a ®êi ng­êi, ta ch­a thÊy ng­êi nµo nÆng mïi s¸t khÝ nh­ ng­¬i!";
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
		"Ta cã chót quµ män biÕu ng­¬i/Help",
		"Ta ®· hèi hËn vµ muèn rêi khái ®©y/MoveBLH",
		"Ta ®Õn trß chuyÖn víi ng­¬i th«i/OnCancel",
	}
	Say(szHello,getn(tbSay),tbSay);
end;

function MoveBLH()
	local nCash = GetCash();
	local nNeed = GetPK() * 2E5;
	if (GetPK() > 0) then
		if (nCash < nNeed) then
			Talk(1,"","Ng­¬i kh«ng cã ®ñ <color=yellow> "..nNeed.." l­îng <color> mµ ®ßi rêi ®i sao, ta sÏ m·i m·i giam ng­¬i ë ®©y");
			return
		end;
		Pay(nNeed);
		SetPK(0);
	end;
	NewWorld(53,1625,3177);
	SetFightState(0);
	Talk(1,"","Ng­¬i ®· ®­îc xãa hÕt PK, ta mong nhµ ng­¬i sÏ sèng l­¬ng thiÖn.");
end;

function Help()
	Talk(1,"MoveBLH","ChØ sè s¸t thñ cña nhµ ng­¬i lµ <color=red>"..GetPK().."<color> cho nªn quy ra v¹n l­îng ta cã thÓ tÝnh lµ sè ®iÓm PK * 20 v¹n l­îng = Sè tiÒn rêi khái ®©y");
end;


function OnCancel()
end;
