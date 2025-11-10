-- Author: Kinnox
-- Date: 1/6/2023
-- Chuc nang: Chuyen Phai
Include("\\script\\lib\\TaskLib.lua.lua")
Include("\\script\\Global\\LuaNpcFactions\\talk\\master_main.lua")
Include("\\script\\Global\\LuaNpcFactions\\factionhead.lua")
Include("\\script\\system_config.lua")
NOW_END_SAY = "KÕt thóc ®èi tho¹i./no";
ONTEAM = "Ng­¬i ®ang trong tæ ®é, kh«ng thÓ chuyÔn hÖ!"
ONFACTION = "Ng­¬i ®ang trong bang héi, kh«ng thÓ chuyÔn hÖ!"
NOTEQUIPNOW = "Ng­¬i kh«ng ®­îc mÆc trang bÞ trªn ng­êi!"
NOWTALK = "§iÒu kiÖn chuyÓn ®æi cÇn:\n";
NOEXITSITEM = "- Th¸o toµn bé trang bÞ trªn ng­êi\n";
DELETESKILL = "- MÊt toµn bé kü n¨ng 90 vµ 120\n";
NOONTEAM = "- Rêi tæ ®éi hoÆc bang ph¸i\n";
HAVEITEM = "- VËt phÈm cÇn 'TÝn vËt D­¬ng Anh'\n- N÷ cã thÓ chuyÓn sang 'TVB'\n";

HEPHAI = 
{
	{"ThiÕu L©m ph¸i"  			,0,1,0},
	{"Thiªn V­¬ng Bang"			,0,2,1},
	{"§­êng M«n"				,1,3,0},
	{"Ngò §éc Gi¸o"				,1,4,0},
	{"Nga My ph¸i"				,2,5,0},
	{"Thóy Yªn m«n"				,2,6,0},
	{"C¸i Bang"					,3,7,0},
	{"Thiªn NhÉn Gi¸o"			,3,8,0},
	{"Vâ §ang ph¸i"				,4,9,0},
	{"C«n L«n ph¸i"				,4,10,0},
};

GIOITINH = 
{
	{"Nam",0},
	{"N÷" ,1},
};

function main()
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/sugiachuyenphai.lua");
		Msg2Player("Reload this script");
	end;	
	local tb = {};
	local count = 10;
	local szTalk = NOWTALK;
	for i = 1,count do
	tinsert(tb,"Ta muèn chuyÓn "..HEPHAI[i][1].."/tienhanhchuyen#"..i.."")	
	end			
	--tinsert(tb,"Ta muèn mua TÝn vËt d­¬ng anh/shopvp")
	tinsert(tb,NOW_END_SAY)	
	szTalk = szTalk..NOEXITSITEM;
	szTalk = szTalk..DELETESKILL;
	szTalk = szTalk..NOONTEAM;	
	szTalk = szTalk..HAVEITEM;			
	Say(szTalk,count+1,tb);
end

function tienhanhchuyen(_,nRow)
	local nRow = tonumber(nRow);
	local szTalk = NOWTALK;
	if (GetItemCountInBag(6,21,0,0) < 1) then
		Talk(1,"", "Ng­¬i kh«ng mang theo 'TÝn vËt D­¬ng Anh'.")
		return 
	end	

	szTalk = szTalk..NOEXITSITEM;
	szTalk = szTalk..DELETESKILL;
	szTalk = szTalk..NOONTEAM;	
	szTalk = szTalk..HAVEITEM;		
	if (GetSex() == 0 and (HEPHAI[nRow][2] == 2))then
		Talk(1,"","Ng­¬i lµ Nam nhi kh«ng thÓ gia nhËp m«n ph¸i <color=blue> hÖ thñy <color> ®­îc");
		return
	elseif (GetSex() == 1 and (HEPHAI[nRow][2] == 0) and (HEPHAI[nRow][3] == 0) )then
		Talk(1,"","Ng­¬i lµ N÷ giíi kh«ng thÓ gia nhËp m«n ph¸i <color=blue> ThiÕu L©m Ph¸i <color> ®­îc");
		return
	end;
	commonchuyen(nRow);
end


function commonchuyen(nRow)
	local nRow = tonumber(nRow);
	local nIndex;
	local nCount = 0;
	for i = 1,12 do
		nIndex = GetEquipItem(i);
		if (nIndex > 0) then 
			nCount = nCount + 1 
		end;
	end;
	
	
	local nTeamID = GetTeam();
	local sTongInfo = GetTongName();

	if (nTeamID) then
	Talk(1,"",ONTEAM)
	return end;
	
	if (sTongInfo ~= "") then
	Talk(1,"",ONFACTION)
	return end;	

	if (nCount > 0) then
	Talk(1,"",NOTEQUIPNOW)
	return end;
	OnClearAllCore();
	
	for i = 3, 1000 do
		if i ~= 53 then
			DelMagic(i);
		end;
	end;

	SetSeries(HEPHAI[nRow][2]);
	local nLevel = GetLevel();
	local nExp = GetExp();	
	SetLevel(nLevel);
	AddOwnExp(nExp);	
	SetPos(1613,3197);
	
	local n = GetTask(0)
	SetTask(0, SetByte(n,1,0))
	join_main(HEPHAI[nRow][3]);
	DelTaskItem(21);	
	KickOutSelf();
end

function shopvp()
	local nNeed = 100;
	if GetTaskItemCount(19) < 100 then
		Talk(1,"","Ta lµ mÖnh quan triÒu ®×nh, ng­¬i t­ëng lêi nãi cña ta kh«ng ®¸ng <color=yellow> 100 XU <color> sao?")
		return
	end; 
	AddTaskItem(21);
	DelTaskItem(19,100);
end;

function no()
end