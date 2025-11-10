-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tap Hoa;

Grocery = {};

function main(nNpcIdx)
	local tbSay = {
		"ß≠a ta v“ Ba L®ng Huy÷n/MoveToBaLangHuyen",
		"Ta chÿ gh– ngang qua./OnCancel",
	}
	Say(10356,getn(tbSay),tbSay);
end;

function MoveToBaLangHuyen()
	Msg2Player("NgÂi y™n! ChÛng ta Æi Ba L®ng Huy÷n ");
	SetClearPos();
	SetFightState(0);
	LeaveTeam();
	SetPunish(0);
	ForbidTownPortal(0);
	SetDeathScript("");
	NewWorld(53,1582, 3237);	
	SetRevPos(19);
	SetFightState(0);
end

function OnCancel()
end;