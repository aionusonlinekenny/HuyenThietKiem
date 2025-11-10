-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Su gia bang hoi;
Include("\\script\\system_config.lua")
TongNpc = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/Tong.lua");
		Msg2Player("Reload this script");
	end;
TongNpc:TongNpc()
end;

function TongNpc:TongNpc()
	local tbSay = {}
	if (GetTongID() > 0) then
		tbSay = {
			"Di chuyÓn ®Õn Khu vùc bang/MoveToAreaTong",
			"Ta chØ ghĞ ngang qua./OnCancel",
		}
	else
		tbSay = {
			"Thµnh lËp bang héi/CreateTong",
			"T×m hiÓu yªu cÇu thµnh bang héi/Help",
			"Ta chØ ghĞ ngang qua./OnCancel",
		}
	end
	Say("<color=orange>Sø Gi¶ Bang Héi<color> : Trong giang hå c¸c thÕ lùc ®ang ©m thÇm thµnh lËp ®Ó thèng trŞ vâ l©m, h·y lµ mét bang chñ ch©n chİnh, hµnh hiÖp tr­îng nghÜa.",getn(tbSay),tbSay);
end;

function CreateTong()
OpenTong(0);
end;

function Help()
Talk(1,"","<color=orange>Sø Gi¶ Bang Héi<color> :§iÒu kiÖn cÇn thiÕt ®Ó thµnh lËp bang héi bao gåm:\n + 1 Nh¹c v­¬ng kiÕm\n + §¹t ®¼ng cÊp 80 + 1000 v¹n l­îng\n + Danh väng 100 + Tµi l·nh ®¹o 20 + §· xuÊt s­ ");

end;

function MoveToAreaTong()
Talk(1,"","Chøc n¨ng nµy hiÖn ch­a ho¹t ®éng!");
end;

function OnCancel()
end;
