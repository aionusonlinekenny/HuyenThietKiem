----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\head.lua");
function OnLogout(nLauncher)
	PlayerIndex = nLauncher;
	local tbPos = {
					{1528, 3079},
					{1526, 3057},
					{1536, 3045},
					{1541, 3057},
					{1535, 3037},
					{1526, 3017},
					{1521, 3002},
					{1508, 2998},
					{1500, 2987},
					{1490, 2999}
				 }
	local aHeo = random(1,getn(tbPos));
	SetPos(tbPos[aHeo][1],tbPos[aHeo][2]);
	SetTask(LDHead.tbTask.SIGNUP,0);
	SetCreateTeam(1);
	SetDeathScript("");
	SetLogoutScript("");
	LDHead:RemovePlayer();
end;