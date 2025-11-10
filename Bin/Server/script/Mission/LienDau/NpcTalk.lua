----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Head: Mission Lien Dau/ Su Gia Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\head.lua");
Include("\\script\\system_config.lua");
LDNPC = {};
function main()
	LDNPC:NpcTalk();
end;

function LDNPC:NpcTalk()
	if(SYS_SWITCH_LEAGUE == 0) then
		Talk(1,"","TÝnh n¨ng nµy hiÖn ch­a khai më!")
		return
	end
	
	local nW,_,_ = GetWorldPos();
	local nDay, nHour, nMinutes = date("%A"), tonumber(date("%H")), tonumber(date("%M"))
	local SayHello = "<color=orange>Sø gi¶ liªn ®Êu<color>: Ph¶i! Ta chÝnh lµ sø gi¶ cña triÒu ®×nh phông mÖnh t×m kiÕm c¸c cao thñ trong vâ l©m.";
	local tbSay = {
		"Ta muèn ®Õn héi tr­êng tËp trung/JoinHall",
		"Ta ®Õn t×m hiÓu ®iÒu kiÖn tham chiÕn/Condition",
		"Ta chØ ghÐ ngang qua/no",
	};
		
	local tbSay2 = {
		"Ta muèn ®Õn héi tr­êng thi ®Êu/JoinFightHall",
		"XÕp h¹ng b¶n th©n cña ®¹i ta/Top10",
		-- "Ta ®Õn t×m hiÓu xÕp h¹ng 10 cao thñ lõng danh/Top10",
		"Ta ®Õn t×m hiÓu ®iÒu kiÖn tham chiÕn/Condition",
		"Ta muèn xem qua cöa hµng liªn ®Êu/LDShop",
		"Ta chØ ghÐ ngang qua/no",
	};
	
	if (GetCurCamp() == 0) then
		Talk(1,"","<color=orange>Sø gi¶ liªn ®Êu<color>: V« danh tiÓu tèt, ch­a lµ g× trong thiªn h¹ sao d¸m ®Õn t×m ta. H·y t×m mét m«n ph¸i gia nhËp häc hái.");
	return end;
	
	if (GetLevel() < LDHead.tbMission.LEVEL) then
		Talk(1,"","<color=orange>Sø gi¶ liªn ®Êu<color>: Thµnh thËt mµ nãi ®¼ng cÊp cña ng­¬i ch­a xøng tÇm tranh hïng, ng­êi h·y tu luyÖn lªn "..LDHead.tbMission.LEVEL.." thªm råi quay l¹i ®©y t×m ta.");
	return end;
	
	if (nW ~= LDHead.tbMap.SUBWORLD_1) then
		Say(SayHello,getn(tbSay),tbSay);
	elseif (nW == LDHead.tbMap.SUBWORLD_1) then
		if ( 	(nHour == LDHead.tbTimer.HOUR_1 and nMinutes <  LDHead.tbTimer.MINUTES_1) or  
				(nHour == LDHead.tbTimer.HOUR_2 and nMinutes >  LDHead.tbTimer.MINUTES_1+9 and nMinutes < LDHead.tbTimer.MINUTES_2) or 
				(nHour == LDHead.tbTimer.HOUR_3 and nMinutes >  LDHead.tbTimer.MINUTES_2+9 and nMinutes < LDHead.tbTimer.MINUTES_3) ) then
		Say(SayHello,getn(tbSay2),tbSay2);
		else
			Talk(1,"","<color=orange>Sø gi¶ liªn ®Êu<color>: §¹i hiÖp lµ ng­êi ë ph­¬ng nµo vËy?. Thêi gian thi ®Êu b¾t ®Çu vµo 19h15,19h30,19h45");
		end;
	else
		Talk(1,"","<color=orange>Sø gi¶ liªn ®Êu<color>: §¹i hiÖp lµ ng­êi ë ph­¬ng nµo vËy?.");
	end;
end;

function PlayerRegister()
	LDHead:PlayerRegister();
end;

function Condition()
	LDHead:Condition();
end;

function LDShop()
	LDHead:LDShop();
end;

function JoinHall()
	LDHead:JoinHall();
end;

function JoinFightHall()
	LDHead:JoinFightHall();
end;

function Top10(nID)
	LDHead:Top10(nID);
end;
function no()
end;