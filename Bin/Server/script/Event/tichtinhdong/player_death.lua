--Author: Kinnox;
--Date: 13/05/2022;
Include("\\script\\event\\tichtinhdong\\head.lua");
function OnDeath(nLauncher,nAttacker)
	PlayerIndex = nLauncher;
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(53);
	SetRevPos(19);-- dua ve ba lang huyen
	SetFightState(0);
	LeaveTeam();
	SetPunish(0);
	ForbidTownPortal(0);
	SetDeathScript("");
	SubWolrd = nOldSubWorld;
end;