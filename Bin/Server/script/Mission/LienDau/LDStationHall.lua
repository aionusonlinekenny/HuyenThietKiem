----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Name: Xa Phu hoi truong;
Include("\\script\\mission\\liendau\\head.lua");
LDStationHall = {};
function main()
	LDStationHall:NpcTalk();
end;

function LDStationHall:NpcTalk()
	local MsgHello = "<color=orange>Xa phu héi tr­êng liªn ®Êu<color>: §¹i hiÖp muèn ®i ®©u ta ®­a ®i!"
	local tbSay = {
		"Ta muèn ra ngoµi/warnning",
		"Ta chØ ®Õn th¨m ng­¬i/no",
	};
	Say(MsgHello,getn(tbSay), tbSay);
end;

function warnning()
	local MsgHello = "<color=orange>Xa phu héi tr­êng liªn ®Êu<color>: Ng­¬i kh«ng ®ñ b¶n lÜnh muèn hñy bá t­ c¸ch tham gia sao?"
	local tbSay = {
	"§óng ta muèn rêi khái ®©y/yes",
	"Ta chØ ®Õn th¨m ng­¬i/no",
	};
	Say(MsgHello,getn(tbSay), tbSay);
end;

function yes()
	LDHead:JoinHall();
	SetTask(LDHead.tbTask.SIGNUP,0);
	SetTask(LDHead.tbTask.CALLDAME,0);
	SetTask(LDHead.tbTask.REGISTER,0);
	SetTask(LDHead.tbTask.RIVAL,0);
end;

function no()
end;