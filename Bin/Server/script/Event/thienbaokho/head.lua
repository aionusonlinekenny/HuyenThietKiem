Include("\\script\\lib\\tasklib.lua");
Include("\\script\\lib\\libevent.lua");
EXIT = "Ta chØ ®Õn th¨m ng­¬i/no"
EXP = 20000000
function NpcTalk(nIndex,nNumber)
	--local nNumber = tonumber(nNumber);
	-- Msg2Player("DEBUG "..nNumber.." "..GetNpcID(nIndex).." "..GetNpcName(nIndex).."");
	local szHello = "<color=orange>Thiªn b¶o khè <color>: Hoµn thµnh nhiÖm vô Thiªn B¶o Khè nhËn ®­îc <color=green>"..EXP.."<color> ®iÓm kinh nghiÖm vµ mét phÇn quµ hÊp dÉn!"
	local EXI
	local tbSay = {};
	if (nNumber == 1) then
		tbSay = {
			"Ta ®Õn t×m hiÓu ho¹t ®éng/help",
			"Ta muèn nhËn nhiÖm vô/AcceptQuest",
			"Ta muèn tiÕp tôc nhiÖm vô/ContinuteQuest",
			"Ta muèn rêi khái ®©y/MoveToBaLang",
			EXIT,
		}
	elseif (nNumber == 3) then
		tbSay = {
			"Ta ®Õn t×m hiÓu ho¹t ®éng/help",
			"Ta ®Õn b¶n ®å Thiªn B¶o Khè/moveMap",
			"Ta ®· hoµn thµnh nhiÖm vô/FinishQuest",
			EXIT,
		}
	else
		tbSay = {
			"Ta kh«ng rêi khái ®©y/moveMap",
			EXIT,
		}
	end;
	Say(szHello,getn(tbSay),tbSay);
end;


function help()
	Talk(1,"","Mçi nµy tham gia ®­îc 1 lÇn, khi hoµn thµnh cã thÓ nhËn ®­îc "..EXP.." ®iÓm kinh nghiÖm vµ mét phÇn quµ hÊp dÉn!");
end;

function moveMap()
	SetFightState(0);
	NewWorld(394,1415,3204);
	Msg2Player("Ngåi yªn! ta ®­a ng­¬i ®Õn Thiªn b¶o khè.");
end;

function AcceptQuest()
	if (GetTask(T_TIN_SU) > 1) then
		Talk(1,"","Mçi ngµy chØ ®­îc tham gia 1 lÇn, nÕu ng­¬i cã lÖnh bµi Thiªn b¶o khè ta cã thÓ gióp ®ì ng­¬i th«ng qua");
		return
	end;
	
	if (GetTask(T_TIN_SU) == 1) then
		Talk(1,"","Ng­¬i ®· nhËn nhiÖm vô råi, nhµ ng­¬i nªn tiÕp tôc thùc hiÖn!.");
		return
	end;
	SetFightState(1);
	SetPunish(1);
	SetRevPos(53,19);
	ForbidTownPortal(1);
	NewWorld(394,1414,3183);
	SetTask(T_TIN_SU,1);
	SetDeathScript("\\script\\event\\thienbaokho\\player_death.lua");
	local tbRan = {};
	tbRan = {
		{5,3,1,2,4},
		{5,4,2,1,3},
		{9,6,7,2,3},
		{6,9,2,1,3},
		{1,5,9,2,4},
		{4,9,3,1,2},
		{8,6,1,2,4},
		{7,6,3,2,4},
		{4,6,8,2,5},
		{3,6,1,8,4},
		{5,6,1,8,4},
		{3,7,1,9,4},
		{3,7,5,9,4},
		{3,7,2,9,4},
		{8,7,2,9,4},
		{1,2,3,4,5},
	}
	local nRan = random(1,getn(tbRan))
	SetTask(T_BK_KEY_1,tbRan[nRan][1])
	SetTask(T_BK_KEY_2,tbRan[nRan][2])
	SetTask(T_BK_KEY_3,tbRan[nRan][3])
	SetTask(T_BK_KEY_4,tbRan[nRan][4])
	SetTask(T_BK_KEY_5,tbRan[nRan][5])
	Msg2Player("ChØ dÉn nhiÖm vô cña ng­êi lµ <color=yellow> "..tbRan[nRan][1].." - "..tbRan[nRan][2].." - "..tbRan[nRan][3].." - "..tbRan[nRan][4].." - "..tbRan[nRan][5].." <color> h·y mau hoµn thµnh!." );
	AddNote("ChØ dÉn Thiªn b¶o khè :<color=yellow> "..tbRan[nRan][1].."-"..tbRan[nRan][2].."-"..tbRan[nRan][3].."-"..tbRan[nRan][4].."-"..tbRan[nRan][5].."<color>  ");
	-- Msg2Player("DEBUG<color=yellow> "..GetTask(T_BK_KEY_1).." - "..GetTask(T_BK_KEY_2).." - "..GetTask(T_BK_KEY_3).." - "..GetTask(T_BK_KEY_4).." - "..GetTask(T_BK_KEY_5).." <color> h·y mau hoµn thµnh!." );
end;

function ContinuteQuest()
	local szTask_1 = ""
	local szTask_2 = ""
	local szTask_3 = ""
	local szTask_4 = ""
	local szTask_5 = ""
	if (GetTask(T_TIN_SU) > 1) then
		Talk(1,"","Mçi ngµy chØ ®­îc tham gia 1 lÇn, nÕu ng­¬i cã lÖnh bµi Thiªn b¶o khè ta cã thÓ gióp ®ì ng­¬i th«ng qua");
		return
	end;
	if (GetTask(T_TIN_SU) < 1) then
		Talk(1,"","Ng­¬i ch­a nhËn nhiÖm vô lµm sao cã thÓ tiÕp tôc?, hay mau nhËn nhiÖm vô ®i");
		return
	end;
	if (GetTask(T_BK_KEY_DONE_1) >= 1 and GetTask(T_BK_KEY_DONE_2) >= 1 and GetTask(T_BK_KEY_DONE_3) >= 1  and GetTask(T_BK_KEY_DONE_4) >= 1  and GetTask(T_BK_KEY_DONE_5) >= 1) then
		Talk(1,"","§¹i hiÖp mau trë vÒ Ba l¨ng huyÖn phông mÖnh tÝn sø ®Ó nhËn quµ!");
		return
	end;
	SetFightState(1);
	SetPunish(1);
	SetRevPos(53,19);
	ForbidTownPortal(1);
	NewWorld(394,1414,3183);
	SetDeathScript("\\script\\event\\thienbaokho\\player_death.lua");
	if (GetTask(T_BK_KEY_1) > 0) then
		szTask_1 = ""..GetTask(T_BK_KEY_1).."";
	end;
	if (GetTask(T_BK_KEY_2) > 0) then
		szTask_2 = ""..GetTask(T_BK_KEY_2).."";
	end;
	if (GetTask(T_BK_KEY_3) > 0) then
		szTask_3 = ""..GetTask(T_BK_KEY_3).."";
	end;
	if (GetTask(T_BK_KEY_4) > 0) then
		szTask_4 = ""..GetTask(T_BK_KEY_4).."";
	end;
	if (GetTask(T_BK_KEY_5) > 0) then
		szTask_5 = ""..GetTask(T_BK_KEY_5).."";
	end;
	Msg2Player("ChØ dÉn nhiÖm vô cña ng­êi cßn l¹i lµ <color=yellow> "..szTask_1.." - "..szTask_2.." - "..szTask_3.." - "..szTask_4.." - "..szTask_5.." <color> h·y mau hoµn thµnh!.");
	AddNote("ChØ dÉn nhiÖm vô cña ng­êi cßn l¹i lµ <color=yellow> "..szTask_1.." - "..szTask_2.." - "..szTask_3.." - "..szTask_4.." - "..szTask_5.." <color> h·y mau hoµn thµnh!.");
end;

function MoveToBaLang()
	NewWorld(53,1582, 3237);	
	SetFightState(0);
	Msg2Player("Ngåi yªn! Chóng ta ®i Ba l¨ng huyÖn ");
end;

function CheckKey(nIndex,szNameNpc,nPlayerIndex)
	if (GetTask(T_TIN_SU) < 1) then
		return
	end;
	local Check = 0;
	if (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 01") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(1,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 02") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(2,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 03") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(3,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 04") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(4,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 05") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(5,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 06") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(6,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")	
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 07") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(7,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")			
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 08") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(8,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")		
	elseif (szNameNpc == "B¶o Khè Thñ Hé Gi¶ 09") then
		PlayerIndex = nPlayerIndex;
		Check = CheckCondition(9,nPlayerIndex);
		if (Check == 0) then
		return end;
		SetTask(740+Check,1);
		-- Msg2Player(""..Check.."")				
	end;

end;

function CheckCondition(nID,nPlayerIndex)
local nSTT = 0;
local nID = nID;
PlayerIndex = nPlayerIndex
	for i = 1,5 do
		-- Msg2Player(" "..i.." "..(735+i).." "..GetTask(735+i).." "..nID.."");
			nSTT = i;
		if (GetTask(735+i) == nID) then
			-- Msg2Player(""..(735+i).."")
			if (nSTT ~= 1) then
				nSTT = nSTT - 1;	
				for k=1,nSTT do
					if (GetTask(735+k) > 0) then
						Talk(1,"","Nhµ ng­¬i ®· ®i sai lé tr×nh, vui lßng kiÓm tra l¹i.");
						return 0;
					end;
				end;				
			end;
			-- Msg2Player("DEBUG nSTT: "..nSTT.." ");
			if (735+i ~= 736) then
				return nSTT + 1;
			end;
			return nSTT;
		end;
	end;
	return 1;
end;

function Baokho(nID,nPlayerIndex)
	local nSTT = 0;
	local nID = nID;
	PlayerIndex = nPlayerIndex
	for i = 1,5 do
		if (GetTask(735+i) == nID) then
			if (GetTask(740+i) == 1) then
				Msg2Player("C­íp b¶o khè sè <color=yellow> "..GetTask(735+i).." <color> thµnh c«ng!");
				SetTask(735+i,0);	
				SetTask(740+i,2);
				return
			else
				Talk(1,"","Ng­¬i ch­a h¹ gôc ®­îc ng­êi gi÷ b¶o khè mµ muèn c­íp b¶o khè ­?");
				return
			end;	
		end;
	end;
end;

function FinishQuest()
	if (GetTask(T_BK_KEY_DONE_1) >= 1 and GetTask(T_BK_KEY_DONE_2) >= 1 and GetTask(T_BK_KEY_DONE_3) >= 1  and GetTask(T_BK_KEY_DONE_4) >= 1  and GetTask(T_BK_KEY_DONE_5) >= 1) then
		Talk(1,"","Chóc mõng ®¹i hiÖp <color=yellow>"..GetName().."<color> ®· hoµn thµnh nhiÖm vô Thiªn b¶o khè, phÇn th­ëng ®¹i hiÖp nhËn ®­îc <color=green>20.000.000 ®iÓm kinh nghiÖm vµ mét vËt phÈm quý hiÕm <color>");	
		Msg2SubWorld("Chóc mõng ®¹i hiÖp <color=yellow>"..GetName().."<color> ®· hoµn thµnh nhiÖm vô Thiªn b¶o khè, phÇn th­ëng ®¹i hiÖp nhËn ®­îc <color=green>20.000.000 ®iÓm kinh nghiÖm<color> vµ mét vËt phÈm quý hiÕm <color>");	
		SetTask(T_BK_KEY_1,0)
		SetTask(T_BK_KEY_2,0)
		SetTask(T_BK_KEY_3,0)
		SetTask(T_BK_KEY_4,0)
		SetTask(T_BK_KEY_5,0)
		SetTask(T_BK_KEY_DONE_1,0)
		SetTask(T_BK_KEY_DONE_2,0)
		SetTask(T_BK_KEY_DONE_3,0)
		SetTask(T_BK_KEY_DONE_4,0)
		SetTask(T_BK_KEY_DONE_5,0)
		SetTask(T_TIN_SU,2);
		AddOwnExp(EXP)
		AddEventThienBaoKho();
	else
		Talk(1,"","Ng­¬i ch­a hoµn thµnh nhiÖm vô ®Õn ®©y gÆp ta cã chuyÖn g×?")
	end;

end;


function no()
end;