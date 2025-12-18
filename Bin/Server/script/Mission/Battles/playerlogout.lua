--Author: Kinnox;
--Date: 08/11/2021;
--Name: Ng­êi ch¬i rêi khái chiÕn tr­êng Tèng Kim;
Include("\\script\\mission\\battles\\head.lua");
function OnLogout(nLauncher)
local OldPlayerIndex = PlayerIndex;
	PlayerIndex = nLauncher;
	SetTitle(GetTask(ZBattle.tbTask.LAST_TITLE)); --xoa rank
	SetTask(ZBattle.tbTask.LAST_TITLE, 0); -- xoa rank
	SetTask(ZBattle.tbTask.SERIES,0); --xoa lien tram
	SetTaskTemp(ZBattle.tbTaskTmp.TITLE,0)--xoa rank
	SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH,0)--xoa time delay
	SetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST,0)--xoa anti post diem tong kim;
	SetDeathScript("");
	SetLogoutScript("");
	SetFightState(0);
	SetLogoutRV(0);
	SetPunish(0);
	SetCreateTeam(1);
	ForbidTownPortal(0);
	SetPKFlag(0);
	ForbidChangePK(0);
	RestoreCamp();
	DelMSPlayer(PlayerIndex,GetTask(ZBattle.tbTask.GROUP));
	if (GetTask(ZBattle.tbTask.GROUP) == 1) then
	SetGlbMissionV(ZBattle.tbMission.TONG_SOLIDER,GetMSPlayerCount(ZBattle.tbMission.MAIN,1)); -- task tinh so luon cua bang xep hang quan tong
	else
	SetGlbMissionV(ZBattle.tbMission.KIM_SOLIDER,GetMSPlayerCount(ZBattle.tbMission.MAIN,2));-- task tinh so luon cua bang xep hang quan kim
	end;
	SetTask(ZBattle.tbTask.GROUP,0);
	ZBattle:LeaveMap();
	PlayerIndex = OldPlayerIndex;
end;