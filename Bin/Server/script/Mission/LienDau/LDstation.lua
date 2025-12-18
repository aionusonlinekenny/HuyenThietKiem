----------------------------------------
--Author: Kinnox
--Date: 03/09/2021
----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau\\ Xa Phu
----------------------------------------

LDStaTion = {};
function main()
	LDStaTion:NpcTalk();
end;

function LDStaTion:NpcTalk()
	local MsgHello = "<color = Orange>Dﬁch Trπm<color>: ßπi hi÷p muËn Æi Æ’n Æ©u, ta sœ Æ≠a Æπi hi÷p Æi."
	local tbSay = {
		"Ba L®ng Huy÷n[500]/MoveToBaLangHuyen",
		"Ta chÿ gh– ngang qua/no",
	};
	Say(MsgHello,getn(tbSay),tbSay);
end;

function MoveToBaLangHuyen()
	local nPrice = 500;
	local nSubWorldId = SubWorldID2Idx(53);
	local w,x,y = GetWorldPos();
	if nSubWorldId < 0 then	--chua mo map, ngung ham`
	return end
	if (GetCash() < nPrice) then
	Talk(1,"",10336);
	return
	end
	NewWorld(53,1582, 3237);	
	Pay(nPrice);
	Msg2Player("NgÂi y™n! ChÛng ta Æi Ba L®ng Huy÷n ");
	SetCreateTeam(1);
	RestoreCamp();
	SetFightState(0);
	SetRevPos(53,19);
end;

function no()
end;