----------------------------------------
--Script Lib Trap qua maps
--Code By Kinnox 11/2021
----------------------------------------
Include("\\script\\system_config.lua");
LIB_STRING = {
	"VÔng Æ t nµy hi÷n ch≠a khai mÎ, h∑y Æi Æ≠Íng kh∏c!",
	"VÔng Æ t nµy r t nguy hi”m, c∏c hπ ch≠a ÆÒ Æºng c p Æ” qua",
}
--LEVEL TRAP
NOTLEVEL   = 0;
HOASON     = 20;
function CheckFightState(nGroup)
	--	1: Country;
	--	2: FightMaps;
	local nState = GetFightState();
	if (nGroup == 1) then
		if (nState == 0) then
			SetFightState(1);
			return nState;
		else
			SetFightState(0);
			return nState;
		end;
	else
		SetFightState(1);
		return nState;
	end;
end;

function CheckActionOpen()
	if (ActionOpen() < 1) then
		Talk(1, "",
			"VÔng Æ t nµy hi÷n tπi ch≠a khai th´ng, c∏c hπ vui lﬂng ÆÓi m∏y chÒ <color=orange>V‚ L©m Huy“n Thi’t Ki’m<color> <color=red>Open Beta<color>");
		SetFightState(0);
		SetPos(1625, 3185);
		return 1;
	else
		return 0;
	end;
end;

function main()
	Talk(1, "",
		"Nhµ ng≠¨i n™n Æi bªng th«n hµnh phÔ ho∆c xa phu, con Æ≠Íng nµy hi÷n tπi Æang tu sˆa kh´ng th” Æi bÈ qua!");
end;

function AddSkillTrap()
	AddSkillState(963, 1, 0, 18 * 3) -- Anti 3s;
end;
