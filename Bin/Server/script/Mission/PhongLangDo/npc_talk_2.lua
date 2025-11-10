--Author: Kinnox;
--Date: 17/04/2022;
--Ben: 2;
Include("\\script\\mission\\phonglangdo\\head.lua");
function main(nNpcIndex)
	PLD.MAIN.BOATID = 2;
	PLD:NpcTalk(PlayerIndex);
end;