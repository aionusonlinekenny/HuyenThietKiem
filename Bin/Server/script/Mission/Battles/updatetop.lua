Include("\\script\\mission\\battles\\head.lua");

function OpenTop(nLauncher)
	PlayerIndex = nLauncher;
	w,x,y = GetWorldPos();
	
	if getn(dstop) == 10 then
		if w == 325 or w == 380 and (GetTask(ZBattle.tbTask.GROUP) == 1 or GetTask(ZBattle.tbTask.GROUP) == 2) then	
		bFlag = GetGlbMissionV(ZBattle.tbMission.FLAGCLEARTOP); --clear top TK;	
		if (bFlag == 0) and (w == 380)then
			if (GetMSPlayerCount(ZBattle.tbMission.MAIN, 1) == 0 or GetMSPlayerCount(ZBattle.tbMission.MAIN, 2) == 0) then
				dstop = {};
				SetGlbMissionV(ZBattle.tbMission.FLAGCLEARTOP,1);
				print("CLEAR TABLE SONGJIN");
				return
			end;
		end;
			CapNhatTop();
		else
			ShowTopClear();
		end
	else
		KhoiTaoMangTop();
	end
	--SendTopTKNew(100,15,1,2,3,4,5,6,9,8,11,14,22,"abvc",2,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,"a",1,2,3,4,5,6,7,100)
	-- So luong quan Kim , Time, So Luong, .....
end;
-- 11 diem tich luy nguoichoi, 				KILL_ROLE
-- 13 tichluynguoichoi,						ROLE_POINT
-- 14 npc, 									KILL_NPC
-- 20 tich luy npc, 						NPC_POINT
-- 21 lientram, 							SERIES
-- 24 tichluylientram, 						SERIES_POINT
-- 25 bauvat, 								GET_ITEM
-- 26 tich luy bao vat, 					ITEM_POINT
-- 28 tu vong, 								BE_KILL
-- 29 lien tram cao nhat, 					MAX_SERIES
-- 31 diem tich luy 						TEMP_POINT
-- 12 kiem tra phe phai						GROUP

function UpdateTop(nLauncher)
PlayerIndex = nLauncher;
vitri = check_dstop();
	if vitri ~= 0 then
		dstop[vitri][2] = GetTask(ZBattle.tbTask.GROUP); -- GROUP
		dstop[vitri][3] = GetTask(ZBattle.tbTask.TEMP_POINT); -- TEMP_POINT
		dstop[vitri][4] = GetTask(ZBattle.tbTask.KILL_ROLE); -- KILL_ROLE
		dstop[vitri][5] = GetTask(ZBattle.tbTask.KILL_NPC);
		dstop[vitri][6] = GetTask(ZBattle.tbTask.BE_KILL);
		dstop[vitri][7] = GetTask(ZBattle.tbTask.MAX_SERIES);
		dstop[vitri][8] = GetTask(ZBattle.tbTask.GET_ITEM);
	elseif getn(dstop) > 0 then
		vtmin = check_dsmin();
		if (GetTask(ZBattle.tbTask.TEMP_POINT) > dstop[vtmin][3]) then
			dstop[vtmin][1] = GetName();
			dstop[vtmin][2] = GetTask(ZBattle.tbTask.GROUP);
			dstop[vtmin][3] = GetTask(ZBattle.tbTask.TEMP_POINT);
			dstop[vtmin][4] = GetTask(ZBattle.tbTask.KILL_ROLE);
			dstop[vtmin][5] = GetTask(ZBattle.tbTask.KILL_NPC);
			dstop[vtmin][6] = GetTask(ZBattle.tbTask.BE_KILL);
			dstop[vtmin][7] = GetTask(ZBattle.tbTask.MAX_SERIES);
			dstop[vtmin][8] = GetTask(ZBattle.tbTask.GET_ITEM);
		end;
	end;
SapXepLaiDSTOP();
end;


tam = {"",0,0,0,0,0,0,0};
function SapXepLaiDSTOP()
	for i=1,getn(dstop) do
		for j=1,getn(dstop) do
			if dstop[i][3] > dstop[j][3] then
				tam = dstop[i];
				dstop[i] = dstop[j];
				dstop[j] = tam;
			end;
		end;
	end;
end;

function check_dstop()
	for i=1,getn(dstop) do
		if GetName() == dstop[i][1] then
			return i
		end ;
	end;
return 0
end;

function check_dsmin()
min = dstop[1][3];
vt_min = 1;
	for i=2,getn(dstop) do
		if min > dstop[i][3] then
			min = dstop[i][3]
			vt_min = i
		end;
	end;
return vt_min
end;


function CapNhatTop()
Time_SongJin = GetGlbMissionV(ZBattle.tbMission.TIMERSHOW);
MemberSong = 0;
MemberJin = 0;
			MemberSong = GetGlbMissionV(ZBattle.tbMission.TONG_SOLIDER);
			MemberJin = GetGlbMissionV(ZBattle.tbMission.KIM_SOLIDER);
		SendTopTKNew(MemberJin,Time_SongJin,GetTask(ZBattle.tbTask.KILL_ROLE),GetTask(ZBattle.tbTask.ROLE_POINT),GetTask(ZBattle.tbTask.KILL_NPC),GetTask(ZBattle.tbTask.NPC_POINT),GetTask(ZBattle.tbTask.SERIES),GetTask(ZBattle.tbTask.SERIES_POINT),GetTask(ZBattle.tbTask.GET_ITEM),GetTask(ZBattle.tbTask.ITEM_POINT),GetTask(ZBattle.tbTask.TEMP_POINT),GetTask(ZBattle.tbTask.BE_KILL),GetTask(ZBattle.tbTask.MAX_SERIES),
		dstop[1][1],dstop[1][2],dstop[1][3],dstop[1][4],dstop[1][5],dstop[1][6],dstop[1][7],dstop[1][8],
		dstop[2][1],dstop[2][2],dstop[2][3],dstop[2][4],dstop[2][5],dstop[2][6],dstop[2][7],dstop[2][8],
		dstop[3][1],dstop[3][2],dstop[3][3],dstop[3][4],dstop[3][5],dstop[3][6],dstop[3][7],dstop[3][8],
		dstop[4][1],dstop[4][2],dstop[4][3],dstop[4][4],dstop[4][5],dstop[4][6],dstop[4][7],dstop[4][8],
		dstop[5][1],dstop[5][2],dstop[5][3],dstop[5][4],dstop[5][5],dstop[5][6],dstop[5][7],dstop[5][8],
		dstop[6][1],dstop[6][2],dstop[6][3],dstop[6][4],dstop[6][5],dstop[6][6],dstop[6][7],dstop[6][8],
		dstop[7][1],dstop[7][2],dstop[7][3],dstop[7][4],dstop[7][5],dstop[7][6],dstop[7][7],dstop[7][8],
		dstop[8][1],dstop[8][2],dstop[8][3],dstop[8][4],dstop[8][5],dstop[8][6],dstop[8][7],dstop[8][8],
		dstop[9][1],dstop[9][2],dstop[9][3],dstop[9][4],dstop[9][5],dstop[9][6],dstop[9][7],dstop[9][8],
		dstop[10][1],dstop[10][2],dstop[10][3],dstop[10][4],dstop[10][5],dstop[10][6],dstop[10][7],dstop[10][8],MemberSong);
end;

function ShowTopClear()
	SendTopTKNew(0,0,0,0,0,0,0,0,0,0,0,0,0,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,"",0,2,3,4,5,6,7,0)
end;


function KhoiTaoMangTop()
dstop = {};
	for i=1,10 do
		dstop[getn(dstop)+1] = {"",0,0,0,0,0,0,0};
	end;
	print("KHOI TAO MANG TOP")
end