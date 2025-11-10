--Author: Kinnox;
--Date: 14/09/2021;
--Name: Xa Phu Lien Dau;
Include("\\script\\system_config.lua")
LDStation = {};
function main()
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/ldstation.lua");
		Msg2Player("Reload this script");
	end;	

	local szHello = "<color=orange>Xa phu liªn ®Êu<color>: §¹i hiÖp muèn ®i ®©u ta ®­a ®i!";
	local tbSay = {
		"Ba L¨ng HuyÖn/MoveToBaLangHuyen",
		"Ta chØ ghÐ ngang qua ®©y/no",
	};
	Say(szHello,getn(tbSay),tbSay);
end;

function LDStation:LDNpcTalk()
	
end;

function MoveToBaLangHuyen()
	local nPrice = 500;
	NewWorld(53,1582, 3237);	
	if (GetFightState() > 0) then
		SetFightState(0);
	end;
	SetCreateTeam(1);
	SetRevPos(53,19);
	Pay(nPrice);
	Msg2Player("Ngåi yªn! Chóng ta ®i Ba L¨ng HuyÖn.");
end;

function no()
end;