--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\vuotai\\head.lua");
function OnDeath(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
		PlayerIndex = nLauncher;
	Msg2MSAll(VA.MAIN.MISSION_VA, " "..GetName().." §¹i hiÖp kh«ng may tö vong khi ®ang v­ît ¶i");
	VA:OUT_DEATH(PlayerIndex,2);
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end