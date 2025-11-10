----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\head.lua");
function OnLogout(nLauncher)
	local OldSubWorld = SubWorld;
	local nAttacker = GetTask(LDHead.tbTask.RIVAL);
	PlayerIndex = nAttacker; -- ChuyÓn Id Cho ®èi thñ;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	LDHead:FinishMission(1);
	LDHead:JoinHall();
	DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
	PlayerIndex = nLauncher; -- ChuyÓn id Cho m×nh;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	LDHead:FinishMission(2);
	LDHead:JoinHall();
	DelMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
	LDHead:RemovePlayer();
end;