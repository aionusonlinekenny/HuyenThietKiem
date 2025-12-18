--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\phonglangdo\\head.lua");
function OnLogout(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nLauncher;
	Msg2MSAll(PLD.MAIN.MISSION_PLD, " "..GetName().." §¹i hiÖp kh«ng ®ñ b¶n lÜnh quyÕt ®Þnh rêi thuyÒn.");
	PLD:LeaveGame(PlayerIndex,1);
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end