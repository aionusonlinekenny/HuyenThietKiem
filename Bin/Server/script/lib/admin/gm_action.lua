----------------------------------
--	Copyright: AJX Team
--	Author: [N]
--	Date: 16/08/2015
--	Desc: GM lµm viÖc
----------------------------------
Include("\\script\\player\\head.lua")
Include("\\script\\lib\\TaskLib.lua")

----------------------------------
--
----------------------------------
MAX_PLAYER = 1200;
FindFALSE = "Kh«ng t×m thÊy ®èi t­îng!"
function Player_Action()
	local szHello = "Mêi ®¹i hiÖp chän chøc n¨ng bªn d­íi!"
	local szSay = {
		"T×m tªn nh©n vËt/findName",
		"Kick Account/KickByCharNameInput",
		"T×m tªn nh©n vËt b»ng ID/findID",
		"T×m ID nh©n vËt toµn m¸y chñ/findAll",
		"Tho¸t/Exit"
	};
	Say(szHello,getn(szSay),szSay);

end;
function KickByCharNameInput()
    AskClientForString("DoKickChar", "", 1, "32", "NhËp tªn nh©n vËt muèn kick")
end;

function DoKickChar(__, szName)
    local name = tostring(szName);
    if name == "" then
        Talk(1,"","B¹n ch­a nhËp tªn nh©n vËt!");
        return
    end

    for i = 1, MAX_PLAYER do
        local nName = GetName(i);
        if nName == name then
            local acc = GetAccount(i);
            KickOutAccount(acc);
            Talk(1,"","§· kick nh©n vËt "..name.." (account "..acc..")");
            return
        end
    end
    Talk(1,"","Kh«ng t×m thÊy nh©n vËt nµy online!");
end;

function findName()
	AskClientForString("ExcuterName","",1,"32","NhËp tªn");
end;

function ExcuterName( __, szString)
	local String = tostring(szString);
	local nPlayerIndex = PlayerIndex;
	SetTaskTemp(TMP_INDEX_ADMIN,nPlayerIndex);
	for i = 1,MAX_PLAYER do
		PlayerIndex = i;
		if (GetName() == String) then
			PlayerIndex = nPlayerIndex;
			SetTaskTemp(TMP_INDEX_PLAYER,i);
			ActionManager();
			return 0;
		end;	
	end;
	PlayerIndex = nPlayerIndex;
	Talk(1,"",FindFALSE);
end;



function findID()
	AskClientForNumber("ExcuterID",0,1200,"NhËp ID")
end;

function ExcuterID( __, nNumber)
	local nzNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,nzNumber);
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	if (GetNpcIdx() > 0) then
		PlayerIndex = nPlayerIndex;	
		ActionManager();
		return 0;
	else
		PlayerIndex = nPlayerIndex;	
		Msg2Player("Nh©n vËt hiÖn t¹i kh«ng cã trªn m¹ng hoÆc ID kh«ng ®óng");
	end;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	PlayerIndex = nPlayerIndex;
end;

function findAll()
	local szStringName;
	local szStringAcName;
	local szStringCash;
	local nPlayerIndex = PlayerIndex;
	for i = 1,MAX_PLAYER do
		PlayerIndex = i;
		if (GetNpcIdx() > 0) then
			szStringName = GetName();
			szStringAcName = GetAccount();
			szStringCash = GetCash();
			PlayerIndex = nPlayerIndex;
			 Msg2Player("INFO <color=green>[ID:"..i.."]<color> - <color=yellow>[ID:"..szStringName.."]<color> - <color=blue>[Account:"..szStringAcName.."]<color> - <color=orange>[Money:"..szStringCash.."]<color>");
			--Msg2Player("INFO <color=green>[ID:"..i.."]<color> - <color=yellow>[ID:"..szStringName.."]<color> ");
		end;
	end;
end;

function ActionManager()
	local nPlayerIndex = PlayerIndex;
	local szName;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
		szName = GetName();
	PlayerIndex = nPlayerIndex;
	local szHello = "§¹i hiÖp ®ang thao t¸c trªn ng­êi ch¬i : <color=red> "..szName.." <color>"
	local szSay = {
		"KiÓm tra th«ng tin/investex",
		"Khãa trß chuyÖn/LockTalk#1",
		"Më trß chuyÖn/LockTalk#2",
		"Di chuyÓn ®Õn gÇn ng­êi ch¬i/MovePlayer#1",
		"Di chuyÓn ng­êi ch¬i ®Õn gÇn/MovePlayer#2",
		"Di chuyÓn ng­êi ch¬i vÒ Ba L¨ng HuyÖn/MovePlayer#3", 
		"T¨ng ®iÓm phóc duyªn/PointNumber#1",
		"T¨ng ®iÓm danh väng/PointNumber#2",
		"T¨ng ®iÓm tèng kim/PointNumber#3",
		"T¨ng ®iÓm tµi l·nh ®¹o/PointNumber#4",
		"T¨ng VIP/PointNumber#5",
		"Khãa tµi kho¶n vÜnh viÔn/BandAccount",
		"Më khãa tµi kho¶n vÜnh viÔn/UnBandAccount",
		"Xãa mËt khÈu r­¬ng/ExcuterDelPW",
		"Tho¸t/Exit"
	};
	Say(szHello,getn(szSay),szSay);
end;

function LockTalk(__,nSel)
	local nSel = tonumber(nSel);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	if (nSel == 1) then
		Msg2SubWorld("BQT game ®· b¾t thµnh c«ng mét c¸i mám thèi")
		SetChatFlag(1);
		SetTitle(324);
	else
		Msg2Player("BQT game ®· tha thø cho c¸i mám thói ngµy x­a")
		SetChatFlag(0);
		SetTitle(0);
	end;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	PlayerIndex = nPlayerIndex;
	Success();
end;

function MovePlayer( __, nSel)
	local nSel = tonumber(nSel);
	local nPlayerIndex = PlayerIndex;
	local nFight = 0;
	local nW,nX,nY;
	if (nSel == 1) then
		PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
		nW,nX,nY = GetWorldPos();
		nFight = GetFightState();
		PlayerIndex = nPlayerIndex;
		NewWorld(nW,nX,nY);
		SetFightState(nFight);
		SetTaskTemp(TMP_INDEX_PLAYER,0);
		Success();
	elseif (nSel == 2) then
		PlayerIndex = nPlayerIndex;
		nW,nX,nY = GetWorldPos();
		nFight = GetFightState();
		PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
		NewWorld(nW,nX,nY);
		SetFightState(nFight);
		PlayerIndex = nPlayerIndex;
		SetTaskTemp(TMP_INDEX_PLAYER,0);
		Success();
	else
		PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
		NewWorld(53,1581,3242);
		SetFightState(0);
		PlayerIndex = nPlayerIndex;
		SetTaskTemp(TMP_INDEX_PLAYER,0);
		Success();
	end;
end;

function PointNumber( _, nSel)
	local nSel = tonumber(nSel);
	local nPlayerIndex = PlayerIndex;
	local szFunc;
	if (nSel == 1) then
		szFunc = "ExcuterFuYuan";
	elseif (nSel == 2) then
		szFunc = "ExcuterRepute";
	elseif (nSel == 3) then
		szFunc = "ExcuterSongJin";
	elseif (nSel == 4) then
		szFunc = "ExcuterLeadExp";
	elseif (nSel == 5) then
		szFunc = "ExcuterVIP";	
	end;
	AskClientForNumber(szFunc,0,1200,"NhËp ®iÓm");
end;

function ExcuterFuYuan( _, nNumber)
	local nNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	AddFuyuan(nNumber);
	Msg2Player("Phóc duyªn cña ®¹i hiÖp ®· t¨ng thªm: "..nNumber.." tæng ®iÓm:"..GetFuyuan().." ®iÓm");
	PlayerIndex = nPlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();
end;

function ExcuterRepute( _, nNumber)
	local nNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	AddRepute(nNumber);
	Msg2Player("Danh väng cña ®¹i hiÖp ®· t¨ng thªm : "..nNumber.." tæng ®iÓm:"..GetRepute().." ®iÓm");
	PlayerIndex = nPlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();
end;

function ExcuterSongJin( _, nNumber)
	local nNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	SetTask(2013,GetTask(2013)+nNumber);
	PlayerIndex = nPlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();
end;

function ExcuterLeadExp( _, nNumber)
	local nNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	if ((nNumber >= 100) or (nNumber <= 0) or (nNumber >= GetLeadLevel() + nNumber)) then
		PlayerIndex = nPlayerIndex;
		SetTaskTemp(TMP_INDEX_PLAYER,0);
		Msg2Player("Thao t¸c thÊt b¹i, kh«ng thÓ t¨ng tµi l·nh ®¹o cho nh©n vËt");
		return 0;
	else
		while (GetLeadLevel() < nNumber) do 
			AddLeadExp(9E6);
		end;
		PlayerIndex = nPlayerIndex;
		SetTaskTemp(TMP_INDEX_PLAYER,0);
		Success();
	end;
end;

function ExcuterVIP( _, nNumber)
	local nNumber = tonumber(nNumber);
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	if (nNumber == 1) then
		SetTask(914,1);
	elseif (nNumber == 2) then
		SetTask(915,1);
		nIndex = AddScriptItem(15);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(70);
		SetItemBindState(nIndex,2);
	elseif (nNumber == 3) then
		SetTask(916,1);
		while(GetLevel() < 100) do
			AddOwnExp(1000000)
		end
		nIndex = AddScriptItem(0);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(0);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(0);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(2);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(2);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(2);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(15);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(16);
		SetItemBindState(nIndex,2);
		nIndex = AddScriptItem(70);
		SetItemBindState(nIndex,2);	
	end;
	Msg2Player("Chóc mõng ®¹i hiÖp trë thµnh VIP "..nNumber.." cña m¸y chñ ");
	Msg2SubWorld("Chóc mõng ®¹i hiÖp <color=yellow>"..GetName().." trë thµnh VIP "..nNumber.." <color> cña m¸y chñ ")
	PlayerIndex = nPlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();
end;

function investex()
	local nPlayerIndex 	= PlayerIndex;
	PlayerIndex 	= GetTaskTemp(TMP_INDEX_PLAYER);
	local Name 		= GetName();
	local TK 		= GetAccount();
	local IP 		= GetIP();
	local Level 	= GetLevel();
	local Exp 		= GetExp();
	local Tien 		= GetCash();
	local Pass		= GetTask(T_PWBOX);
	local Mau 		= GetLife(0);
	local Mau2 		= GetLife(1);
	local Mana 		= GetMana(0);
	local Mana2 	= GetMana(1);
	local VLMT 		= GetTask(T_VLMT)
	local TTK 		= GetTask(T_TTK)
	local DiemPD	= GetFuyuan();
	local DiemDV	= GetRepute();
	local TienD		= GetTaskItemCount(19);
	local nW,nX,nY 	= GetWorldPos();
	local nMapName	= GetMapName(nW);
	local TongName	= GetTongName();
	PlayerIndex = nPlayerIndex;
	Talk(3,"","- Tªn:<color=yellow> "..Name.." <color>\n- Tµi kho¶n: <color=blue>"..TK.."<color>\n- IP: <color=green>"..IP.."<color>\n- CÊp: <color=fire>"..Level.."<color>, kinh nghiÖm: <color=fire>"..Exp.."<color>\n- TiÒn: <color=yellow>"..Tien.."<color>",
	"- MËt khÈu r­¬ng: <color=yellow>"..Pass.."<color>\n- M¸u: <color=fire>"..Mau.."/"..Mau2.."<color>, mana: <color=blue>"..Mana.."/"..Mana2.."<color>\n- Täa ®é: <color=green>"..nMapName.." :: ("..floor(nX/8).."/"..floor(nY/16)..")<color>\n- Phóc duyªn: <color=green>"..DiemPD.."<color>\n- Danh väng: <color=green>"..DiemDV.."",
	"- TiÒn ®ång: <color=fire>"..TienD.."<color>\n- VLMT: <color=fire>"..VLMT.."<color>\n- VLMT: <color=fire>"..(TTK/5).."<color>\n- Tªn Bang: <color=green>"..TongName.."<color>")	
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();		
end

function BandAccount()
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	Msg2SubWorld("<color=pink>BQT Game võa c¾t chim dßng hä nhµ "..GetName().." <color>.");
	BanAccount(GetAccount()); -- khoa tai khoan vinh vien;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	PlayerIndex = nPlayerIndex;
	Success();
end;

function UnBandAccount()
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	UnBanAccount(GetAccount()); -- khoa tai khoan vinh vien;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	PlayerIndex = nPlayerIndex;
	Success();
end;

function ExcuterDelPW()
	local nPlayerIndex = PlayerIndex;
	PlayerIndex = GetTaskTemp(TMP_INDEX_PLAYER);
	DelPwbox();
	Msg2Player("Admin ®· xãa mËt khÈu r­¬ng cho ®¹i hiÖp!");
	PlayerIndex = nPlayerIndex;
	SetTaskTemp(TMP_INDEX_PLAYER,0);
	Success();
end;

function Exit()

end;

function Success()
	Msg2Player("§· hoµn thµnh thao t¸c qu¶n lý ")
end;

