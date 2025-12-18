--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\vuotai\\head.lua");
function main(nNpcIndex)
	local nW,_,_ = GetWorldPos();
	if (nW == 78) then
	VA.MAIN.BOATID = 1;
	elseif (nW == 1)then
	VA.MAIN.BOATID = 2;
	elseif (nW == 37)then
	VA.MAIN.BOATID = 3;
	elseif (nW == 11)then
	VA.MAIN.BOATID = 4;
	elseif (nW == 162)then
	VA.MAIN.BOATID = 5;
	elseif (nW == 176)then
	VA.MAIN.BOATID = 6;
	elseif (nW == 80)then
	VA.MAIN.BOATID = 7;
	end;
	VA:NpcTalk(PlayerIndex);
end;