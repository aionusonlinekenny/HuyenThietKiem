--Author: Kinnox;
--Date: 16/05/2022;
--function: death_player;
Include("\\script\\mission\\loanchien\\head.lua");
function OnLogout(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nLauncher;
	Msg2MSAll(LC.MAIN.MISSION_LC, " "..GetName().." §¹i hiÖp kh«ng ®ñ b¶n lÜnh bá nªn ®· bá ch¹y.");
	LC:LeaveGame(PlayerIndex,1);
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end