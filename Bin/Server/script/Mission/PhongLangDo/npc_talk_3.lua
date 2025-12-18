--Author: Kinnox;
--Date: 17/04/2022;
--Ben: 3;
Include("\\script\\mission\\phonglangdo\\head.lua");
function main(nNpcIndex)
	PLD.MAIN.BOATID = 3;
	PLD:NpcTalk(PlayerIndex);
end;