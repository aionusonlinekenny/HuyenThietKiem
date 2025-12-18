--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\vuotai\\head.lua");
function OnLogout(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nLauncher;
	Msg2MSAll(VA.MAIN.MISSION_VA, " "..GetName().." §¹i hiÖp kh«ng ®ñ b¶n lÜnh quyÕt ®Þnh rêi b¶n ®å v­ît ¶i, ®Ó l¹i ®ång ®éi bÞ hiÕp n¬i xa tr­êng khãi löa.");
	VA:OUT_DEATH(PlayerIndex,1);
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end

