----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\head.lua");
function OnDeath(nLauncher,nAttacker)
	local OldSubWorld = SubWorld;
	PlayerIndex = nAttacker; -- ChuyÓn Id Cho ®èi thñ;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	LDHead:FinishMission(3);
	LDHead:JoinHall();	
	DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
	PlayerIndex = nLauncher; -- ChuyÓn Id cho m×nh;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	LDHead:FinishMission(4); 
	DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
	SubWorld = OldSubWorld;
end;