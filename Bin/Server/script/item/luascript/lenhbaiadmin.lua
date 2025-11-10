
Include("\\script\\lib\\admin\\point.lua");
Include("\\script\\lib\\admin\\char.lua");
Include("\\script\\lib\\admin\\gm_action.lua");
Include("\\script\\lib\\admin\\item.lua");
Include("\\script\\lib\\admin\\role.lua");
Include("\\script\\lib\\admin\\tool.lua");
Include("\\script\\lib\\admin\\equip.lua");
Include("\\script\\lib\\admin\\movemap.lua");
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\global\\LuaNpcFunctions\\Station.lua");
Include("\\script\\global\\LuaNpcFunctions\\datau.lua");
Include("\\script\\mission\\loanchien\\head.lua");
Include("\\script\\mission\\vuotai\\head.lua");
Include("\\script\\mission\\phonglangdo\\head.lua");
Include("\\script\\mission\\liendau\\head.lua");
Include("\\script\\event\\quahuyhoang\\head.lua");
Include("\\script\\event\\hoadang\\head.lua");
Include("\\script\\event\\noichaoyeuthuong\\head.lua");
Include("\\script\\mission\\battles\\head.lua");
Include("\\script\\system_config.lua");

----------------------------------
-- Lenh bai admin
----------------------------------
TAB_ADMIN = { "123456", "1234567", "kennylucia" }
function IsAdmin(acc)

    local n = (getn and getn(TAB_ADMIN))
    for i = 1, n do
        if acc == TAB_ADMIN[i] then
            return 1
        end
    end
    return nil
end
function OnUse(nNpcIndex)
    local acc = GetAccount()
    if IsAdmin(acc) then
        AskClientForNumber("mainx", 0, 999999, "NhËp mËt khÈu admin:")
    else
		Talk(1,"","Kh«ng ph¶i lµ admin kh«ng thÓ dïng lÖnh bµi nµy ");
	return end;
end

function mainx(nIndex, nID)
    local id = tonumber(nID) or 0

    if id == 12345 then
        dofile("script/item/luascript/lenhbaiadmin.lua")
        Talk(1, "", "Reload Script Successfull!")
        return

    elseif id == 92 then
    local nW, nX, nY = GetWorldPos()
	local nPlayerCount = GetPlayerCount()
	local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: <color><color=white>"..nW.."<color><color=green>\n    - X: <color><color=white>"..nX.."<color><color=green> - Y: <color><color=white>"..nY.."\n"..nW.."<color> <color=green>- X: <color><color=white>"..(nX*32).."<color><color=green> - Y: <color><color=white>"..(nY*32).."<color><color=green>\nSè l­îng ng­êi online: <color><color=white>"..nPlayerCount..""
	Tab_inSert = {
	"Më shop m¸u/Shop",
	"NhËn Trang BÞ/WPlayer_Equip",
	"NhËn vËt phÈm/GM_Item",
	"NhËn Epx/kinhnghiem",
	"Më réng r­¬ng/ExPandBox",
	"Liªn quan ®Õn nh©n vËt Admin/GM_Char",
	"Di chuyÓn vÒ Ba L¨ng HuyÖn/movetoBLH",
	"Di chuyÓn b¶n ®å luyÖn c«ng/SelectTrainning",
	"Di chuyÓn b¶n ®å ho¹t ®éng/MoveMaps",
	"§iÒu khiÓn m¸y chñ/ServerRemote",
	"Thao t¸c trªn ng­êi ch¬i/Player_Action",
	"Thö hµm/func_Test",
	"Ta kh«ng cÇn ng­¬i d¹y b¶o/no",
	}
	Say(szHello,getn(Tab_inSert),Tab_inSert);
	return
    else
        Talk(1, "", "Sai m· lÖnh hoÆc mËt khÈu.")
        return
    end
end
--script add boss
BOSSTIEU_ARRAY={
{"DiÖu Nh­",703,2,{1,6}},
{"LiÔu Thanh Thanh",704,1,{2,5}},
{"Tr­¬ng T«ng ChÝnh",705,4,{3,4}}
};

POSITION_ARRAY={
{53,1672*32,3264*32,"Ba Lang Huyen"},
{53,1672*32,3264*32,"Ba Lang Huyen"},
{53,1672*32,3264*32,"Ba Lang Huyen"},
{53,1672*32,3264*32,"Ba Lang Huyen"},
{53,1672*32,3264*32,"Ba Lang Huyen"},
{53,1672*32,3264*32,"Ba Lang Huyen"},
};

COMMON_INFO = "Cã ng­êi nh×n thÊy <color=Yellow> %s <color> xuÊt hiÖn t¹i %s täa ®é <color=green> %d/%d <color>"

-- function release2bosstieu()
	-- local nNpcIndex = 0;
	-- local nPos = 1;
	-- for i=1,250 do
		-- nPos = RANDOM(1,2);
		-- local SubWorld = SubWorldID2Idx(POSITION_ARRAY[nPos][1]);
		-- nNpcIndex = AddNpc(985, 95, SubWorld, POSITION_ARRAY[nPos][2], POSITION_ARRAY[nPos][3], 0, "Boss tét", 2, 0);
	-- end
-- end;

function kinhnghiem()
SetLevel(150);
end

function release2bosstieu()
	local nNpcIndex = 0;
	local nPos = 1;
	-- for i=1,250 do
		-- nPos = RANDOM(1,2);
		local SubWorld = SubWorldID2Idx(53);
		nNpcIndex = AddNpc(1677, 95, SubWorld, 49632, 104576, 1, "D©m TÆc", 2, 0);
		SetNpcSeries(nNpcIndex,0);
		SetNpcReduceDamge(nNpcIndex, 30);	
		SetNpcScript(nNpcIndex, "\\script\\global\\luanpcmonsters\\ondeath_normal.lua")
	-- end
	

end;

function func_Test()
	-- for i = 2075,2084 do
		-- AddGoldItem(i);
	-- end;

	--SetRank(9)
	--AddSkillState(1585, 20, 1, 24*60*60*18)
	--SendNotification("","Th«ng b¸o: ChiÕn tr­êng tèng kim ®ang trong giai ®o¹n b¸o danh!");
	--for i = 1, 100 do
		--PlayerIndex = i;
	--SendNotification("","(Lo)<color=red>Th«ng b¸o: <color> <color=green>Hoa ®¨ng<color> ®· xuÊt hiÖn t¹i b¶n ®å Hoa s¬n c¶nh kü tr­êng täa ®é 999/999 !");
		--			SendNotification("","<pic=138><color=red>Th«ng b¸o: <color> <color=green>Tèng kim<color> ®ang trong thêi gian b¸o danh h·y mau chãng tham gia <pic=127> !:U !");
	--SendNotification("","(Lo)<color=red>Th«ng b¸o: <color> <color=green>Boss tiÓu Hoµng kim<color> xuÊt hiÖn t¹i T­¬ng d­¬ng(Täa ®é:217/207), BiÖn kinh(Täa ®é:197/203), D­¬ng ch©u(Täa ®é:173/213) <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	--SetLockSongJin("jxkinnox",1);
	--Msg2Player("DEBUG SONG JIN : "..GetLockSongJin("jxkinnox01").."");
	--Msg2Player("Dang long");
	--end;
	--TrembleItem();
	--SendNotification("","<color=red>Th«ng b¸o: <color> <color=green>Boss tiÓu Hoµng kim<color> xuÊt hiÖn t¹i T­¬ng d­¬ng(Täa ®é:217/207), BiÖn kinh(Täa ®é:197/203), D­¬ng ch©u(Täa ®é:173/213) (!hh) (qd) (dz) !");
	-- dstop = {};
	-- for i=1,10 do
		-- dstop[getn(dstop)+1] = {"",0,0,0,0,0,0,0};
	-- end;
	-- for i = 1,3 do
	-- AddScriptItem(0);
	-- AddScriptItem(0);
	-- AddScriptItem(0);
	-- AddScriptItem(2);
	-- AddScriptItem(2);
	-- AddScriptItem(2);
	-- AddScriptItem(70);
	-- AddScriptItem(15);
	-- AddScriptItem(16);
	-- end;
	-- SetTitle(258);
	
	-- ClearMapNpcWithName(53,"Nåi ch¸o yªu th­¬ng");
	-- local nPlayerIndex = PlayerIndex;
	-- local IDTong;
	-- for i = 1,300 do
		-- PlayerIndex = i
			-- nW,nX,nY = GetWorldPos()
			-- if (nW ~= 380) then
				-- SetTitle(GetTask(self.tbTask.LAST_TITLE))
				-- SetTask(ZBattle.tbTask.LAST_TITLE, 0)
				-- SetTask(ZBattle.tbTask.SERIES,0)
				-- SetTask(ZBattle.tbTask.SERIES_POINT,0)
				-- SetTaskTemp(ZBattle.tbTaskTmp.TITLE,0)
				-- SetTaskTemp(ZBattle.tbTaskTmp.LAST_DEATH,0)
				-- SetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST,0)
				-- SetTask(ZBattle.tbTask.LAST_JOIN, 0);
				-- if (nW ~= 1 and nW ~= 53) then
				 -- SetFightState(0)
				-- end;
			-- end;
			-- if (nW ~= 1 and nW ~= 53) then
				 -- SetFightState(1)
				-- end;
		-- if GetName() == "DuongGiaTuong" then
		
			-- for i = 1,3 do
			-- nIndex = AddScriptItem(131,2); --STG le hop;
			-- SetItemBindState(nIndex,2);
			-- nIndex = AddTaskItem(12,2); --LB PLD;
			-- SetItemBindState(nIndex,2);
			-- nIndex = AddScriptItem(4,2); --TTL DB;
			-- SetItemBindState(nIndex,2);
			-- nIndex = AddScriptItem(6,2); --Que hoa tuu;
			-- SetItemBindState(nIndex,2);
			-- end;
			-- for i = 1,5 do
			-- nIndex = AddTaskItem(59,2); --Hoa toc trieu lenh;
			-- SetItemBindState(nIndex,2);
			-- end;
			-- Earn(1500000);
			-- AddMagic(325,20);
			-- SetTask(7,0);
			-- SetTask(300,0);
			-- for i = 302,318 do
				-- SetTask(i,0);
			-- end;
			-- SetTitle(0);
			-- AddMagic(321,20);
			-- while(GetLevel() < 137) do
			-- AddOwnExp(50999999)
			-- end
			-- SetLevel(118);
			-- for i = 1,50 do
			-- AddScriptItem(115)
			-- end;
			-- SetTask(T_EVENT_MAX1,500);
			-- SetTask(600,0);
			-- SetTask(601,0);
			-- SetLogoutScript(""); 
			-- SetDeathScript("");  
			-- ForbidChangePK(0);
			-- SetFightState(0);
			-- SetPunish(0);
			-- SetLogoutRV(0)
			-- SetRevPos(53,19);
			-- ForbidTownPortal(0);
			-- AddOwnExp(5000000)
			-- Msg2Player("nhËn ®­îc <color=pink>5.000.000 ®iÓm kinh nghiÖm<color> tõ H­ng•Cu•Bù");
			-- IDTong = GetTongName();
			
			-- Msg2Player("GM Check Test");
		-- end;
	-- end;
	-- PlayerIndex = nPlayerIndex;
	-- Msg2Player(" "..IDTong.." ")
	-- Msg2SubWorld("Kªnh th«ng tin liªn l¹c:\nGroup: https://www.facebook.com/groups/volamhuyenthietkiem/\nWebsite: jx-online.com");
	-- Msg2SubWorld("Thêi gian b¶o tr× m¸y chñ diÔn ra vµo 18h00\n Thêi gian support ho¹t ®éng: 10h00 -> 23h00");
	-- for i = 1,20 do
		-- AddTaskItem(12);
	-- end;
	-- local n = GetTask(0);
	-- Msg2Player(" "..GetItemCountInBag(6,19,0,0).." ")
	-- for i = 1,30 do
	-- AddScriptItem(136)
	
	-- end;
	-- ChangeOwnFeature(1204,733,999999)
	-- release2bosstieu();
	-- ClearMapNpcWithName(53,"D©m TÆc");
	-- AddGoldItem(207);
	--AddItem(0,10,30,10,0,0,0)
	--end;
	--SetTask(600,3);
	--SetTask(602,3);
	-- for i = 1,50 do
		-- AddTaskItem(28);
	-- end;
	--NewWorld(336,1158,2964);
	-- SetTask(TASK_DT_TOTAL,300);
	-- SetTask(TASK_DT_REWAR40,0);
	-- SetTask(TASK_DT_REWAR40,0);
	-- SetTask(TASK_COUNT_QUEST,0);
	-- for i = 729,734 do
		-- Msg2Player(" "..GetTask(i).."");
		-- SetTask(i,0);
	-- end;
	--AddItem(0,0,0,10,1,GetLucky(0),10);
--AddMagic(1073,20);
--AddMagic(1074,20);
--AddMagic(1077,20);
--AddMagic(1079,20);
--AddMagic(1078,20);
--AddSkillState(1586, 20, 1, 24*60*60*18);
Msg2Player("Kiem tra Task Truoc khi add ::: <color=green>"..GetTask(2024).."")
SetTask(2024,10000);
Msg2Player("Kiem tra Task Sau khi add ::: <color=pink>"..GetTask(2024).."")
end;

function OnExchangeItem()
	-- if not nCount or nCount == 0 or nCount > 1 then 
		-- Talk(1,"","yªu cÇu bá 1 thø vµo. ")
		-- return 0
	-- end
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." - "..nLuc.." ");
				local nOp1,nValueMin1,nValueMax1,zValueMax1 = GetItemMagicAttrib(nItemIdx,1); -- hien 1
				local nOp2,nValueMin2,nValueMax2,zValueMax2 = GetItemMagicAttrib(nItemIdx,2); -- an 1
				local nOp3,nValueMin3,nValueMax3,zValueMax3 = GetItemMagicAttrib(nItemIdx,3); -- hien 2
				local nOp4,nValueMin4,nValueMax4,zValueMax4 = GetItemMagicAttrib(nItemIdx,4); -- an 2
				local nOp5,nValueMin5,nValueMax5,zValueMax5 = GetItemMagicAttrib(nItemIdx,5); -- hien 3
				local nOp6,nValueMin6,nValueMax6,zValueMax6 = GetItemMagicAttrib(nItemIdx,6); -- an 3			
				
				Msg2Player( "Hien1: "..nOp1.." - "..nValueMin1.." - "..nValueMax1.." - "..nValueMax1.."");
				Msg2Player( "An1: "..nOp2.." - "..nValueMin2.." - "..nValueMax2.." - "..nValueMax2.."");
				Msg2Player( "Hien2: "..nOp3.." - "..nValueMin3.." - "..nValueMax3.." - "..nValueMax3.."");
				Msg2Player( "An2: "..nOp4.." - "..nValueMin4.." - "..nValueMax4.." - "..nValueMax4.."");
				Msg2Player( "Hien3: "..nOp5.." - "..nValueMin5.." - "..nValueMax5.." - "..nValueMax5.."");
				Msg2Player( "An3: "..nOp6.." - "..nValueMin6.." - "..nValueMax6.." - "..nValueMax6.."");
				DelItemByIndex(nItemIdx);
				-- nItemIdx = AddItem(nG,nD,nP,nL,Ser,0,0);
				-- nItemIdx = AddItem(nG,nD,nP,nL,Ser,nLuc,0);
				-- nItemIdx= AddItem(nG,nD,nP,nL,Ser);
			end
		end
	end
end

function ReceiveReward()--nhan thuong
	Tab_Button = {
	{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1},
	{1,2,4},{1,4,2},{2,1,4},{2,4,1},{4,1,2},{4,2,1},
	{1,3,4},{1,4,3},{3,1,4},{3,4,1},{4,1,3},{4,3,1},
	{3,2,4},{3,4,2},{2,3,4},{2,4,9},{4,1,3},{4,3,1},
	{9,2,4},{9,4,2},{9,3,4},{9,4,3},{9,1,2},{9,3,1},
	{1,9,4},{1,4,9},{9,1,4},{9,4,1},{4,9,1},{4,3,9},
	{1,2,9},{1,9,2},{2,1,9},{2,9,1},{9,1,2},{9,2,1},
	};
	local nRandom = random(1,getn(Tab_Button))
	local Reward1 = Tab_Button[nRandom][1]
	local Reward2 = Tab_Button[nRandom][2]
	local Reward3 = Tab_Button[nRandom][3]
	-- SetTask(175,Reward1)
	-- SetTask(176,Reward2)
	-- SetTask(177,Reward3)
	OpenReWardBox("D· TÈu: XuÊt s¾c hoµn thµnh thö th¸ch!, h·y chän phÇn th­ëng!" ,""..Reward1.."|RwardAccept1",""..Reward2.."|RwardAccept2",""..Reward3.."|RwardAccept3");
end
----------------------------
--
----------------------------
-- function SelectTrainning()
-- Station:Station(10356);
-- end;
----------------------------
--
----------------------------
function Shop()
Sale(2);
end;
----------------------------
--
----------------------------
function ExPandBox()
	local szHello ="<color=red>HÖ Thèng:<color> Xin chµo!"
	Tab_inSert = {
	"Më r­¬ng 1/ID_ExPandBox",
	"Më r­¬ng 2/ID_ExPandBox",
	"Më r­¬ng 3/ID_ExPandBox",
	"Më r­¬ng 4/ID_ExPandBox",
	"Më r­¬ng 5/ID_ExPandBox",
	"Më tói hµnh trang/ID_EquipItemBox",
	"KiÓm tra r­¬ng ®· më/ID_GExPandBox",
	"Ta kh«ng cÇn ng­¬i d¹y b¶o/no"
	}
	Say(szHello,getn(Tab_inSert),Tab_inSert);
end

----------------------------
--
----------------------------
function ID_ExPandBox(nSel)
	local nSel = nSel + 1 ;
	SetStoreBox(nSel);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end

function ID_EquipItemBox()
	SetExPandBox(1);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng tói hµnh trang!");
end
function ID_GExPandBox()
	Msg2Player("C¸c h¹ ®· më ®Õn r­¬ng thø "..GetStoreBox().."");
end
----------------------------
--
----------------------------
function movetoBLH()
	SetFightState(0);
	NewWorld(53,1582,3237);
	Msg2Player("Ngåi yªn! chóng ta ®i Ba L¨ng HuyÖn.");
end
----------------------------
--
----------------------------
function ServerRemote()
local nW,nX,nY = GetWorldPos()
local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY..""
	Tab_inSert = {
	"Khëi ®éng Tèng Kim/ActiveBattle",
	"Khëi ®éng Liªn §Êu/ActiveLienDau",
	"Khëi ®éng V­ît ¶i/ActiveVuotAi",
	"Khëi ®éng Phong L¨ng §é/ActiveFeiling",
	"Khëi ®éng Qu¶ Huy Hoµng/ActiveHuyHoang",
	"Khëi ®éng H¹t Hoµng Kim/ActiveQuaHoangKim",
	"Kh¬i ®éng Lo¹n ChiÕn/ActiveLoanChien",
	"Khëi ®éng Boss Test/ActiveBoss",
	"Khëi ®éng Nåi ch¸o yªu th­¬ng/ActiveNoiChao",
	"Kh¬i ®éng Ho¹t ®éng PK Bang/ActivePKTong",
	"Kh¬i ®éng Reload Script/chaylaiscript",
	"Khëi ®éng m¸y chñ/OpenServer",
	"Khãa m¸y chñ/CloseServer",
	"KiÓm tra Online/TestALL",
	"Ta kh«ng cÇn ng­¬i d¹y b¶o/no"
	}
	Say(szHello,getn(Tab_inSert),Tab_inSert);
end;
function chaylaiscript()
ReloadAllScript();
print("Khoi dong lai Script thanh cong ....");
end
function ActiveBattle() 
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(380);
	OpenMission(3);
	-- CloseMission(3)
	SubWorld = nOldSubWorld;
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Tèng kim ");
end;

function ActiveLienDau()
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(396);
	OpenMission(5);
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Liªn ®Êu ");
	SubWorld = nOldSubWorld;
end;

function ActiveVuotAi()
	local nOldSubWorld = SubWorld;
	for i = 464,470 do
		SubWorld = SubWorldID2Idx(i); --mutil vuot ai;
		OpenMission(VA.MAIN.MISSION_VA);
	end;
	SubWorld = nOldSubWorld;
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng v­ît ¶i ");
end;

function ActiveFeiling()
	local nOldSubWorld = SubWorld;
	for i = 337,339 do
		SubWorld = SubWorldID2Idx(i); -- mutil ben thuyen;
		OpenMission(PLD.MAIN.MISSION_PLD);
	end;
	SubWorld = nOldSubWorld;
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Phong L¨ng §é ");
end;
Include("\\script\\event\\quahuyhoang\\head.lua");
function ActiveHuyHoang()
	AddHatHuyHoang();
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng qu¶ Huy hoµng ");
end;
function ActiveQuaHoangKim()
	AddHatHoangKim();
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng qu¶ Hoµng kim ");
end;

function ActiveLoanChien()
	local nOldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(LC.MAIN.MAP_FIGHT); -- loan chien map;
	OpenMission(LC.MAIN.MISSION_LC);
	SubWorld = nOldSubWorld;
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Lo¹n chiÕn ");
end;

function ActiveBoss()
	Include("\\script\\npclib\\goldboss\\bosstieuhk.lua");
	releasebosstieu();
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Boss tiÓu hoµng kim ");
end;

function ActiveNoiChao()
	Include("\\script\\event\\noichao\\head.lua");
	releasenoichao();
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng Nåi ch¸o yªu th­¬ng");
end;

function OpenServer()
	SetGlbMissionV(GBM_OPEN_SERVER,1);
	print("Active Server Huyen thiet Kiem Kinnox Sucessfull!");
	Msg2SubWorld(""..SYS_NAME_SERVER.."<color=green> chÝnh thøc khëi ®éng, mêi c¸c quý ®ång ®¹o b«n tÈu giang hå. Xin c¶m ¬n quý ®ång ®¹o ®· ñng hé, chóc c¸c h¹ b«n tÈu vui vÎ!<color>");
	AddGlobalNews("M¸y chñ "..SYS_NAME_SERVER.." chÝnh thøc ra m¾t. Chóc quý ®ång ®¹o b«n bÈu vui vÎ!");
end

function CloseServer()
	SetGlbMissionV(GBM_OPEN_SERVER,0);
	print("Close Server "..SYS_NAME_SERVER.." Sucessfull!");
	Msg2SubWorld(""..SYS_NAME_SERVER.."<color=green> ®­a vÒ tr¹ng th¸i chê khai më, quý ®ång ®¹o vui lßng chê ®îi<color>");
	AddGlobalNews("M¸y chñ "..SYS_NAME_SERVER.." ®­a vÒ tr¹ng th¸i chê khai më, quý ®ång ®¹o vui lßng chê ®îi");
end;

function ActivePKTong()
	Include("\\script\\event\\pkbang\\head.lua");
	AddBossBang();
	Msg2SubWorld("Admin khëi ®éng thµnh c«ng ho¹t ®éng PK Bang");
end;

function TestALL()
Talk(1,"","So luong Acc online : "..GetPlayerCount().." - "..tonumber(date("%H"))..":"..tonumber(date("%M")).."\nNPC:"..GetNpcCount().."\nITEM:"..GetItemCount().."\nOBJ:"..GetObjCount().." ") 		-- print so acc online ra GS
end;
function no()
end