--Author: Kinnox;
--Date: 17/04/2022;
--Ben: 1;
Include("\\script\\mission\\phonglangdo\\head.lua");
function main(nNpcIndex)
	PLD.MAIN.BOATID = 1;
	PLD:NpcTalk(PlayerIndex);
end;