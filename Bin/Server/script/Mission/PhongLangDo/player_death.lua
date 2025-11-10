--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\phonglangdo\\head.lua");
function OnDeath(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nLauncher;
	Msg2MSAll(PLD.MAIN.MISSION_PLD, " "..GetName().." §¹i hiÖp kh«ng may tö vong khi ®ang ®i thuyÒn.");
	PLD:LeaveGame(PlayerIndex,2);
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end