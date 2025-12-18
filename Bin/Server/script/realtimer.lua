
-- time server - auto chay 1 phut 1 lan
----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 12/03/2023
----------------------------------
Include("\\script\\mission\\phonglangdo\\head.lua")
Include("\\script\\mission\\vuotai\\head.lua")
Include("\\script\\mission\\battles\\functions.lua");
Include("\\script\\mission\\loanchien\\head.lua");
Include("\\script\\mission\\liendau\\head.lua");
Include("\\script\\npclib\\goldboss\\bosstieuhk.lua");
Include("\\script\\event\\quahuyhoang\\head.lua");
Include("\\script\\event\\quockhanh\\head.lua");
Include("\\script\\event\\hoadang\\head.lua");
Include("\\script\\event\\noichao\\head.lua");
Include("\\script\\event\\pkbang\\head.lua");
Include("\\script\\player\\login.lua");
Include("\\script\\system_config.lua");
Include("\\script\\lib\\tasklib.lua");

RTimer = {}
function OnTimer()
	local ntime = tonumber(date("%H%M"));
	local nCall = 0;
	RTimer:FengLing_Ferry(); -- Phong lang do;
	RTimer:Challengeoftime(); -- Vuot ai;
	RTimer:Warning(); -- Canh bao server;
	RTimer:EventHuyHoang(); -- Event Qua Huy hoang + Hoang kim;
	RTimer:EventHoaDang(); -- Event Hoa dang;
	RTimer:GoldBoss();	-- Goi Boss vang;
	RTimer:LoanChien(); -- Loan Chien;
	RTimer:SongJing(); -- Tong kim;
	RTimer:CallLeague();	-- Lien dau;
	--RTimer:CallSubJect(); -- Mon hoc;
	RTimer:NoiChao(); -- NoiChaoYeuThuong;
	RTimer:PKBang(); -- PKBang;	
	RTimer:EventQuocKhanh();

end
function RTimer:FengLing_Ferry()
-- Phong Lang do;
local nH = tonumber(date("%H"))
	if (nH ~= 20) then -- tranh gio tong kim
		PLD:ACTIVE_PhongLangDo();
	end;
end;

function RTimer:Challengeoftime()
-- Vuot ai;
	if (nH ~= 17) then -- tranh gio bao tri
		VA:ACTIVE_VuotAi();
	end;
end;

function RTimer:LoanChien()
-- Loan chien;
LC:ACTIVE_LoanChien();
end;

	
function RTimer:SongJing()
-- Tong kim;
Active();
end;

function RTimer:CallLeague()
-- Lien dau;
LDHead:Active();
end;

function RTimer:Warning()
-- Canh bao
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if (mod(nH,2) == 0) and (nM == 12) then
		Msg2SubWorld("H·y cÈn thËn víi c¸c m¸nh khãe lõa ®¶o, thÊt tin trªn giang hå, nÕu ph¸t hiÖn h·y b¸o ngay víi tr­ëng th«n "..SYS_NAME_SERVER.." ");
	elseif (mod(nH,2) ~= 0) and (nM == 12) then
		Msg2SubWorld("C¸c h¹ ®ang tham gia m¸y chñ "..SYS_NAME_SERVER..". ®©y lµ phiªn b¶n chÝnh thøc vµ duy nhÊt do "..SYS_NAME_AUTHOR.." ph¸t hµnh!");
	elseif (nM == 30) then
		Msg2SubWorld("Kªnh th«ng tin liªn l¹c:\nGroup: "..SYS_NAME_GROUPFB.."\nWebsite: "..SYS_NAME_WEBSITE.."");
		Msg2SubWorld("Thêi gian b¶o tr× m¸y chñ diÔn ra vµo 18h00\n Thêi gian support ho¹t ®éng: 10h00 -> 23h00");
	end;
end;

function RTimer:EventHuyHoang()
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if(nH == 12 and nM == 0) then
	AddHatHuyHoang();
	return end
	
	if(nH == 12 and nM == 5 ) then
	DelEventHuyHoang(1);
	return end
	
	if(nH == 12 and nM == 15) then
	DelEventHuyHoang(2);
	return end
	
	if(nH == 12 and nM == 20) then
	DelEventHuyHoang(1);
	return end	
	
	if(nH == 12 and nM == 30) then
	DelEventHuyHoang(2);
	return end
	
	if(nH == 12 and nM == 35) then
	DelEventHuyHoang(1);
	return end
	
	if(nH == 12 and nM == 45) then
	DelEventHuyHoang(2);
	return end
	
	
	--qua hoang kim
	if(nH == 13 and nM == 0) then
	AddHatHoangKim();
	return end
	
	if(nH == 13 and nM == 5) then
	DelEvenHoangKim();
	return end	
	
--qua hoang kim
end;

function RTimer:EventQuocKhanh()
    if SYS_SWITCH_QUOC_KHANH == 1 then 
        CheckAndRunQuocKhanh()
    else
        if NPC_QUOCKHANH_ADDED == 1 then
            delnpcquockhanh()
            NPC_QUOCKHANH_ADDED = 0
        end
    end
end

function RTimer:EventHoaDang()
-- hoa dang
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if (nH == 11 and nM == 0) then
		addnpccauhoi();
	elseif (nH == 11 and nM == 30) then
		delnpccauhoi();
		
	elseif(nH == 14 and nM == 0) then
		addnpccauhoi();
	elseif (nH == 14 and nM == 30) then
		delnpccauhoi();	
		
	elseif(nH == 17 and nM == 0) then
		addnpccauhoi();
	elseif (nH == 17 and nM == 30) then
		delnpccauhoi();
		
	elseif(nH == 22 and nM == 0) then
		addnpccauhoi();
	elseif (nH == 22 and nM == 30) then
		delnpccauhoi();	
	end;
end;

function RTimer:GoldBoss()
-- Boss Tieu Hoang Kim
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if (nH == 22 and nM == 0) then
		releasebosstieu();
	elseif (nH == 23 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 23 and nM == 5) then	
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 23 and nM == 10) then	
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");
		

	elseif(nH == 10 and nM == 0) then
		releasebosstieu();
		
	elseif (nH == 14 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 14 and nM == 5) then
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 14 and nM == 10) then	
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");

		
	elseif(nH == 15 and nM == 0) then
		releasebosstieu();

	elseif (nH == 19 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 19 and nM == 5) then
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 19 and nM == 10) then
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");
		
	elseif(nH == 20 and nM == 0) then
		releasebosstieu();
		
	elseif (nH == 21 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 21 and nM == 5) then
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 21 and nM == 10) then
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");	
		
	elseif(nH == 1 and nM == 0) then
		releasebosstieu();
		
	elseif (nH == 2 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 2 and nM == 5) then
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 2 and nM == 10) then
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");	
		
	elseif(nH == 3 and nM == 0) then
		releasebosstieu();
		
	elseif (nH == 5 and nM == 0) then
		ClearMapNpcWithName(78,"DiÖu Nh­");
	elseif (nH == 5 and nM == 5) then
		ClearMapNpcWithName(37,"LiÔu Thanh Thanh");
	elseif (nH == 5 and nM == 10) then
		ClearMapNpcWithName(80,"Tr­¬ng T«ng ChÝnh");
		
	end;
end;

function RTimer:CallSubJect()
local nM = tonumber(date("%M"))
local Tab= {
	"To¸n",
	"Hãa",
	"Lý",
	"V¨n",
	"Sö",
	"§Þa",
}
local Subject1 = RANDOM(1,getn(Tab));
local Subject2 = RANDOM(1,getn(Tab));
local Subject3 = RANDOM(1,getn(Tab));
local nNumber  = RANDOMC(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50);
	if (nM == 0 or nM == 5 or nM == 10 or nM == 15 or nM == 20 or nM == 25 or nM == 30 or nM == 35 or nM == 40 or nM == 45 or nM == 50 or nM == 55) then
		Msg2SubWorld("<color=green>Bµi häc b©y giê:<color> <color=orange>"..Tab[Subject1].." - "..Tab[Subject2].." - "..Tab[Subject3].." <color>. (6 m«n To¸n,Lý,Hßa,V¨n,Sö,§Þa) 5 phót/lÇn.");
		Msg2SubWorld("<color=green>Con sè dß bµi: <color> <color=orange>"..nNumber.." <color> . ")
	end;
end;


function RTimer:NoiChao()
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if (nH == 19 and nM == 20) then
		releasenoichao();
	elseif (nH == 19 and nM == 45) then
		ClearMapNpcWithName(53,"Nåi ch¸o");
	elseif (nH == 2 and nM == 0) then
		releasenoichao();
	elseif (nH == 19 and nM == 45) then
		ClearMapNpcWithName(53,"Nåi ch¸o");
	end;
end;


function RTimer:PKBang()
local nH = tonumber(date("%H"))
local nM = tonumber(date("%M"))
	if(nH == 21 and nM == 45) then
	AddBossBang();
	return end	
end;