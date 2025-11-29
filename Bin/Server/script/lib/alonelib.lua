-- Author by AloneScript (Linh Em)

-- Stub for IncludeLib if not available (C++ library loader)
if not IncludeLib then
	function IncludeLib(szLibName)
		-- Stub function for C++ library loading
		-- HuyenThietKiem may have these libraries pre-loaded or not implemented
	end
end

if IncludeLib then
	IncludeLib("LEAGUE");
end
Include("\\script\\lib\\awardtemplet.lua");

------------------------------ TONG HOP DANH SACH MODULE -------------------------------
SUPPORT_PLAYER = 1; --ID MODULE gi�nh ri�ng cho h� tr� t�n th� theo c�p ��
	--ID Task h� tr� t�n th� theo t�ng c�p
		TASKMODULE_NEWBIE = 1; --Task: H� tr� ng��i ch�i m�i gia nh�p v�o game
		-- TASK 10-200: kh�ng ���c s� d�ng (ch� ���c s� d�ng khi cho nh�n th��ng h� tr� theo t�ng c�p)
		
PLAYER_CHARGECARD = 2; --ID MODULE h� tr� t�n th� cho m�c n�p th�
	--ID Task h� tr� t�n th� theo t�ng c�p
		TASKMODULE_CURCARD = 1; --M�c n�p th� hi�n t�i (��n v�: KNB ho�c Ti�n ��ng) m�c ��nh l� KNB
		TASKMODULE_DATACARD = 2; --Gi� tr� ti�n n�p khi ng��i ch�i r�t ti�n t�i NPC Ti�n Trang
		TASKMODULE_CHARGECARD_20K = 20;
		TASKMODULE_CHARGECARD_50K = 50;
		TASKMODULE_CHARGECARD_100K = 100;
		TASKMODULE_CHARGECARD_200K = 200;
		TASKMODULE_CHARGECARD_500K = 500;

----------------------- Hi�n th� n�i dung th�m cho c�u h�i tho�i khi l� GM thao t�c l�n n� -----------------------
function Note(szCode)
	return ""
end

----------------------- T�o h�p tho�i theo danh s�ch table (trang tr��c, trang k�) -------------------------------
function AddItemByTable(szTitle, tbListItem, nPage)
	local tbOption = {}
	local nMaxOption = 10;
	nPage = nPage or 1;

	if (nPage > 1) then
		tinsert(tbOption, {"Tr� v� trang tr��c�", AddItemByTable, {szTitle, tbListItem, (nPage-1)}})
	end
	if (getn(tbListItem) <= nMaxOption*nPage) then
		for i = (nMaxOption*(nPage-1)+1), getn(tbListItem) do
			tinsert(tbOption, {format("%s", tbListItem[i]["szName"]), AddItemByTableCheckRoom, {tbListItem[i]}})
		end
	else
		for i = (nMaxOption*(nPage-1)+1), (nMaxOption*nPage) do
			tinsert(tbOption, {format("%s", tbListItem[i]["szName"]), AddItemByTableCheckRoom, {tbListItem[i]}})
		end
		tinsert(tbOption, {"�i ��n trang kՅ", AddItemByTable, {szTitle, tbListItem, (nPage+1)}})
	end
		tinsert(tbOption, {"K�t th�c ��i tho�i."})
	CreateNewSayEx(szTitle, tbOption)
end

function AddItemByTableCheckRoom(tbItem)
	local n_IsRoom = IsRoom(tbItem)
	if (n_IsRoom == 0) then
	return end
	
	tbAwardTemplet:GiveAwardByList(tbItem, "AloneScript")
end

function IsRoom(tbItem)
	if not (tbItem["nWidth"]) and not (tbItem["nHeight"]) then
	return 1; end
	
	if (CountFreeRoomByWH(tbItem["nWidth"],tbItem["nHeight"]) < 1) then
		Talk(1, "", format("�� nh�n ���c <color=yellow>%s<color> c�n �t nh�t <color=red>%dx%d<color> � tr�ng", tbItem["szName"],tbItem["nWidth"],tbItem["nHeight"]))
	return 0; end
return 1 end

----------------------- NO / CANCEL ------------------------------
SAYNO = "K�t th�c ��i tho�i./OnCancel";
THINKAGIAN = "�� ta suy ngh� l�i xem./OnCancel";
NOTTRADE = "Kh�ng giao d�ch./OnCancel";

function OnCancel()
end


function GetTaskModule(nModuleID, szModuleName, nTaskID)
	local lid = LG_GetLeagueObj(nModuleID, szModuleName);
	if (lid == 0) or (lid == nil) then
	return 0 end;
return LG_GetLeagueTask(lid, nTaskID) end

function SetTaskModule(nModuleID, szModuleName, nTaskID, nValueTask)
	local lid = LG_GetLeagueObj(nModuleID, szModuleName);
	if (lid == 0) or (lid == nil) then
		lid = LG_CreateLeagueObj()
		local memberObj = LGM_CreateMemberObj();
		LG_SetLeagueInfo(lid, nModuleID, szModuleName);
		LGM_SetMemberInfo(memberObj, szModuleName, 1, nModuleID, szModuleName);
		LG_AddMemberToObj(lid, memberObj);
		LG_ApplyAddLeague(lid,"\\script\\global\\npc\\hotrotanthu.lua", "CreateTaskModule")
		LG_FreeLeagueObj(lid)
	end
	LG_ApplySetLeagueTask(nModuleID, szModuleName, nTaskID, nValueTask)
end

function CreateTaskModule(nLeagueType, szLeagueName, szMemberName, bSucceed)
	if (bSucceed == 0) then
		WriteLog("TaskModule Create Fail: "..szLeagueName.." - szMember: "..szMemberName)
		print("TaskModule Create Fail: "..szLeagueName.." - szMember: "..szMemberName)
	end
	print("TaskModule Create Success: "..szLeagueName.." - szMember: "..szMemberName)
end