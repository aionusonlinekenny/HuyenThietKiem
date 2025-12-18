----------------------------------------
--Author: Kinnox
--Date: 28/03/2023
--Name: Head mission Lien dau;
Include("\\script\\system_config.lua");
LDHead = {};
LDHead.Tab_Pos = {
	{397, 1396, 3331},{397, 1309, 3419},{397, 1265, 3463},{397, 1530, 3288},{397, 1529, 3197},{397, 1528, 3375},
	{397, 1530, 3459},{397, 1529, 3547},{397, 1529, 3640},{397, 1529, 3726},{397, 1569, 3769},{397, 1566, 3682},
	{397, 1573, 3155},{397, 1572, 3240},{397, 1572, 3331},{397, 1573, 3420},{397, 1569, 3507},{397, 1572, 3593},
	{397, 1618, 3198},{397, 1616, 3284},{397, 1619, 3109},{397, 1617, 3371},{397, 1617, 3461},{397, 1615, 3547},
	{397, 1615, 3638},{397, 1613, 3723},{397, 1615, 3814},{397, 1664, 3063},{397, 1664, 3154},{397, 1661, 3243},
	{397, 1659, 3329},{397, 1660, 3507},{397, 1658, 3593},{397, 1658, 3681},{397, 1658, 3768},{397, 1657, 3855},
	{397, 1750, 3240},{397, 1748, 3328},{397, 1746, 3503},{397, 1745, 3680},{397, 1744, 3770},{397, 1791, 3372},
	{397, 1790, 3547},{397, 1789, 3637},{397, 1833, 3414},{397, 1832, 3679},{397, 1882, 3281},{397, 1876, 3634},
	{401, 1396, 3331},{401, 1309, 3419},{401, 1265, 3463},{401, 1530, 3288},{401, 1529, 3197},{401, 1528, 3375},
	{401, 1530, 3459},{401, 1529, 3547},{401, 1529, 3640},{401, 1529, 3726},{401, 1569, 3769},{401, 1566, 3682},
	{401, 1573, 3155},{401, 1572, 3240},{401, 1572, 3331},{401, 1573, 3420},{401, 1569, 3507},{401, 1572, 3593},
	{401, 1615, 3638},{401, 1613, 3723},{401, 1615, 3814},{401, 1664, 3063},{401, 1664, 3154},{401, 1661, 3243},
	{401, 1618, 3198},{401, 1616, 3284},{401, 1619, 3109},{401, 1617, 3371},{401, 1617, 3461},{401, 1615, 3547},
	{401, 1659, 3329},{401, 1660, 3507},{401, 1658, 3593},{401, 1658, 3681},{401, 1658, 3768},{401, 1657, 3855},
	{401, 1750, 3240},{401, 1748, 3328},{401, 1746, 3503},{401, 1745, 3680},{401, 1744, 3770},{401, 1791, 3372},
	{401, 1790, 3547},{401, 1789, 3637},{401, 1833, 3414},{401, 1832, 3679},{401, 1882, 3281},{401, 1876, 3634},
	{403, 1396, 3331},{403, 1309, 3419},{403, 1265, 3463},{403, 1530, 3288},{403, 1529, 3197},{403, 1528, 3375},
	{403, 1530, 3459},{403, 1529, 3547},{403, 1529, 3640},{403, 1529, 3726},{403, 1569, 3769},{403, 1566, 3682},
	{403, 1573, 3155},{403, 1572, 3240},{403, 1572, 3331},{403, 1573, 3420},{403, 1569, 3507},{403, 1572, 3593},
	{403, 1618, 3198},{403, 1616, 3284},{403, 1619, 3109},{403, 1617, 3371},{403, 1617, 3461},{403, 1615, 3547},
	{403, 1615, 3638},{403, 1613, 3723},{403, 1615, 3814},{403, 1664, 3063},{403, 1664, 3154},{403, 1661, 3243},
	{403, 1659, 3329},{403, 1660, 3507},{403, 1658, 3593},{403, 1658, 3681},{403, 1658, 3768},{403, 1657, 3855},
	{403, 1750, 3240},{403, 1748, 3328},{403, 1746, 3503},{403, 1745, 3680},{403, 1744, 3770},{403, 1791, 3372},
	{403, 1790, 3547},{403, 1789, 3637},{403, 1833, 3414},{403, 1832, 3679},{403, 1882, 3281},{403, 1876, 3634},
	}
LDHead.tbMission ={
	MAIN 							= 5, -- khai bao ID o settings/task/missions.txt;
	STATE							= 1, -- mac dinh;
	LEVEL							= 80, -- level khi can;
	MAX_PLAYER						= 1200, -- quy dinh o server;
	EXP								= 500000, --500.000 diem Exp;
	DIR_DEATH						= "\\script\\mission\\liendau\\death.lua",
	DIR_LOGOUT_WAIT					= "\\script\\mission\\liendau\\logout_wait.lua",
	DIR_LOGOUT_FIGHTHALL			= "\\script\\mission\\liendau\\logout_fighthall.lua",
};
FRAME2TIMER  = 18;
LDHead.tbTimer = {
	TIMER_SUBTIMER 					= 14,	-- khai bao ID o settings/timertask.txt;
	TIMER_CONTDOWN					= FRAME2TIMER, --5giay sau bat dau chay TIMER_SUBTIMER;
	TIMER_SECOND					= 53,	--task de sao cung duoc;
	TIMER_STARTFIGHT				= 60*3,	-- 3 phut bat dau;
	TIMER_READY						= 60*3 + 5,   -- 5 giay chuan bi
	TIMER_STOP						= 60*3 + 60*10,  	-- tran chien keo dai 10 phut;
	NTC_HOUR_1						= 19, -- thoi gian thong bao lan 1; 19h10
	NTC_MINUTES_1					= 10, -- thoi gian thong bao lan 1;
	NTC_HOUR_2						= 19, -- thoi gian thong bao lan 2; 19h25
	NTC_MINUTES_2					= 25, -- thoi gian thong bao lan 2;
	NTC_HOUR_3						= 19, -- thoi gian thong bao lan 3; 19h40
	NTC_MINUTES_3					= 40, -- thoi gian thong bao lan 3;
	
	HOUR_1							= 19, -- thoi gian dien ra tran 1; 19h15
	MINUTES_1						= 15, -- thoi gian dien ra tran 1;
	HOUR_2							= 19, -- thoi gian dien ra tran 2; 19h30
	MINUTES_2						= 30, -- thoi gian dien ra tran 2;
	HOUR_3							= 19, -- thoi gian dien ra tran 3; 19h45
	MINUTES_3						= 45, -- thoi gian dien ra tran 3;
};
LDHead.tbTask = {
	REGISTER						= 50,
	SIGNUP							= 51,
	RIVAL							= 52,
	CALLDAME						= 2012,
	POINT							= 1020,
	TOTAL_WIN						= 54,
	TOTAL_JOIN						= 55,
	TOTAL_MEMBER					= 56,
	SWORDMAN						= 57,
};


LDHead.tbMap = {
	SUBWORLD_1 						= 396,
	SUBWORLD_2 						= 397,
	SUBWORLD_3 						= 401,
	SUBWORLD_4						= 403,
};

function LDHead:Active()
	local nDay, nHour, nMinutes = date("%A"), tonumber(date("%H")), tonumber(date("%M"))
	if(SYS_SWITCH_LEAGUE == 0) then
		return
	end
	--Thong bao Notification;
	if	(nHour == self.tbTimer.NTC_HOUR_1 and nMinutes == self.tbTimer.NTC_MINUTES_1) or 
		(nHour == self.tbTimer.NTC_HOUR_2 and nMinutes == self.tbTimer.NTC_MINUTES_2) or
		(nHour == self.tbTimer.NTC_HOUR_2 and nMinutes == self.tbTimer.NTC_MINUTES_3) then
		Msg2SubWorld("Thêi gian b¸o danh liªn ®Êu ®· ®Õn, Cßn <color=green>5 phót<color> ®Ó b¸o danh");
		for i = 1,SYS_MAX_PLAYER do
			PlayerIndex = i;
			SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Liªn ®Êu<color> ®ang trong thêi gian b¸o danh h·y mau chãng tham gia <pic="..random(1,140).."> <pic="..random(1,140).."> !");
		end;
	end;
	--Active Mission;
	if	(nHour == self.tbTimer.HOUR_1 and nMinutes == self.tbTimer.MINUTES_1) or 
		(nHour == self.tbTimer.HOUR_2 and nMinutes == self.tbTimer.MINUTES_2) or
		(nHour == self.tbTimer.HOUR_3 and nMinutes == self.tbTimer.MINUTES_3) then
			local nOldSubWorld = SubWorld;
			SubWorld = SubWorldID2Idx(396);
			OpenMission(self.tbMission.MAIN);
			SubWorld = nOldSubWorld;
			AddGlobalNews("Liªn ®Êu thÕ giíi s¾p b¾t ®Çu, nhanh chãng vµo khu vùc b¸o danh")
			Msg2SubWorld("Liªn ®Êu thÕ giíi s¾p b¾t ®Çu, nhanh chãng vµo khu vùc b¸o danh")
	end;
end;

function LDHead:JoinHall()
	local nSubWorld = SubWorldID2Idx(self.tbMap.SUBWORLD_1);
	if (nSubWorld < 0) then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng!");
	return end;
	local tbPos = {
					{1528, 3079},
					{1526, 3057},
					{1536, 3045},
					{1541, 3057},
					{1535, 3037},
					{1526, 3017},
					{1521, 3002},
					{1508, 2998},
					{1500, 2987},
					{1490, 2999}
				 }
	local aHeo = random(1,getn(tbPos));
	NewWorld(self.tbMap.SUBWORLD_1,tbPos[aHeo][1],tbPos[aHeo][2]);
	SetCurCamp(3);
	SetDeathScript("");
	SetLogoutScript("");
	ForbidTownPortal(1);
	local nW,_,_ = GetWorldPos();
	if (nW == self.tbMap.SUBWORLD_1) then
		Msg2Player("C¸c h¹ ®· vµo héi tr­êng tËp trung liªn ®Êu.");
	end;
end;

function LDHead:JoinFightHall()
	local nSubWorld = SubWorldID2Idx(self.tbMap.SUBWORLD_1);
	if (nSubWorld < 0) then
		Msg2Player("§­êng phÝa tr­íc kh«ng th«ng!");
	return end;
	
	if (GetTask(self.tbTask.REGISTER) < 0 ) then
	return end;
	
	local tbPos = {
					{1591, 2993},
					{1596, 2985},
					{1589, 2971},
					{1581, 2968},
					{1573, 2960},
					{1566, 2966},
					{1557, 2975},
					{1561, 2984}, 
					{1589, 2971}
				 }
	local aHeo = random(1,getn(tbPos));
	SetPos(tbPos[aHeo][1],tbPos[aHeo][2]);
	SetCurCamp(3);
	SetLogoutScript(self.tbMission.DIR_LOGOUT_WAIT);
	SetMissionV(self.tbTask.TOTAL_MEMBER,GetMissionV(self.tbTask.TOTAL_MEMBER) + 1);
	for i = 1,GetMissionV(self.tbTask.TOTAL_MEMBER) do
		if(GetMissionV(self.tbTask.SWORDMAN + i) <= 0) then
			SetMissionV(self.tbTask.SWORDMAN + i, PlayerIndex);
		end
	end;
	SetTask(self.tbTask.CALLDAME,1);
	SetTask(self.tbTask.RIVAL,0);
	ClearDamage(GetTask(self.tbTask.RIVAL));
	SetCreateTeam(0);
	SetRevPos(1);
	ClearStateSkill(1);
	local nWorld,_,_ = GetWorldPos();
	if (nWorld == self.tbMap.SUBWORLD_1) then
		Msg2Player("C¸c h¹ ®· vµo héi tr­êng tuyÓn thñ thi ®Êu.");
	end;
end;

function LDHead:FinishMission(nStatus)
	local Msg =	"";
	if (nStatus == 1) then
		Msg = "§èi thñ bá ch¹y! §¹i hiÖp ®· chiÕn th¾ng,chøng minh sù uy phong cña m×nh. NhËn ®­îc 15 ®iÓm Liªn §Êu vµ 500.000 ®iÓm kinh nghiÖm.";
		SetTask(self.tbTask.TOTAL_WIN,GetTask(self.tbTask.TOTAL_WIN) + 1 );
		SetTask(self.tbTask.POINT,GetTask(self.tbTask.POINT) + 15 );
	elseif (nStatus == 2) then
		--Nothing;
	elseif (nStatus == 3) then
		Msg = "§¹i hiÖp xuÊt s¾c chiÕn th¾ng tr­íc ®èi thñ hïng m¹nh thiªn h¹. NhËn ®­îc 15 ®iÓm Liªn §Êu vµ 500.000 ®iÓm kinh nghiÖm.";
		SetTask(self.tbTask.TOTAL_WIN,GetTask(self.tbTask.TOTAL_WIN) + 1 );
		SetTask(self.tbTask.POINT,GetTask(self.tbTask.POINT) + 15 );
	elseif (nStatus == 4) then
		Msg = "§¹i hiÖp thua ë trËn nµy, h·y rÌn luyÖn thªm ®Ó chiÕn th¾ng. NhËn ®­îc 500.000 ®iÓm kinh nghiÖm.";
		SetCurCamp(3);
	elseif (nStatus == 5) then
		Msg = "§¹i hiÖp gÆp cao thñ ngang tµi ngang søc, bÊt ph©n th¾ng b¹i. NhËn ®­îc 10 ®iÓm Liªn §Êu vµ 500.000 ®iÓm kinh nghiÖm.";
		SetTask(self.tbTask.POINT,GetTask(self.tbTask.POINT) + 10 );
	end;
		SetTask(self.tbTask.TOTAL_JOIN,GetTask(self.tbTask.TOTAL_JOIN) + 1 );
		Msg2Player(Msg);
		AddOwnExp(self.tbTask.EXP);
		
end;

function LDHead:RemovePlayer()
	local OldPlayerIndex = PlayerIndex; 
	for i = 1, GetMissionV(self.tbTask.TOTAL_MEMBER) do
		if (PlayerIndex == GetMissionV(self.tbTask.SWORDMAN + i)) then
			SetMissionV(self.tbTask.SWORDMAN + i,0);
			SetMissionV(self.tbTask.TOTAL_MEMBER ,GetMissionV(self.tbTask.TOTAL_MEMBER - 1));
		end;
	end;
	PlayerIndex = OldPlayerIndex;
end;

function LDHead:Condition()
	local tbTalk ={
		"<color=orange>Sø gi¶ liªn ®Êu<color>: §Çu tiªn nhµ ng­¬i ph¶i <color=red>®¹t ®¼ng cÊp "..LDHead.tbMission.LEVEL.."<color> th× míi cã thÓ tham chiÕn.",
		"Thêi gian b¸o danh vµo lóc <color=green>18h50<color> hµng ngµy, <color=green>19h00<color> sÏ b¾t ®Çu, Thêi gian chê lµ <color=red>3 phót<color>.",
		"Mçi trËn chiÕn ng­êi th¾ng sÏ ®­îc <color=green>15 ®iÓm liªn ®¸u<color>\nNg­êi thua kh«ng cã ®iÓm liªn ®Êu.\nC¶ hai ®Òu ®­îc <color=green>500.000 ®iÓm kinh nghiÖm<color>",
		"Khi tham chiÕn c¸c tuyÓn thñ <color=red>kh«ng ®­îc mang theo<color> c¸c vËt phÈm sau:\n<color=green>Tiªn th¶o lé, thiªn s¬n b¶o lé, tói m¸u, lÖnh bµi phi tèc vµ c¸c vËt phÈm t¨ng søc m¹nh nh©n vËt kh¸c<color>",
	};
	Talk(4,"", tbTalk[1], tbTalk[2], tbTalk[3], tbTalk[4]);
end;

function LDHead:Top10(nID)
	local tbTalk = {
		"<color=orange>Sø gi¶ liªn ®Êu<color>:Nhµ ng­¬i hiÖn t¹i së h÷u\n- §iÓm liªn ®Êu: <color=green>"..GetTask(self.tbTask.POINT).."<color> ®iÓm.\n- ChiÕn th¾ng: <color=green>"..GetTask(self.tbTask.TOTAL_WIN).."<color> trËn.\n- Tham gia: <color=green>"..GetTask(self.tbTask.TOTAL_JOIN).."<color> trËn.",
	};
	Talk(1,"", tbTalk[1]);
end;

function LDHead:LDShop()
	Talk(1,"","C¶m ¬n §¹i hiÖp ®· quan t©m, triÒu ®Þnh hiÖn ch­a cho ta giao th­¬ng!.");
end;

function LDHead:ExcuteSubTimer()
	local Msg = "";
	local szNameRIVAL = "";
	local OldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(LDHead.tbMap.SUBWORLD_1);
	local OldPlayerIndex = PlayerIndex;
	if (GetMissionV(LDHead.tbMission.STATE) == 1) then
		SetMissionV(LDHead.tbMission.STATE,2);
		SetMissionV(LDHead.tbTimer.TIMER_SECOND,1);
	end;
	if (GetMissionV(LDHead.tbMission.STATE) == 2) then
		local nTimer = GetMissionV(LDHead.tbTimer.TIMER_SECOND) + 1;
		SetMissionV(LDHead.tbTimer.TIMER_SECOND,nTimer);
	if (nTimer == LDHead.tbTimer.TIMER_STARTFIGHT) then	
		if (GetMissionV(LDHead.tbTask.TOTAL_MEMBER) <= 1 ) then
			Msg = "Liªn ®Êu bÞ hñy bá v× kh«ng cã tuyÓn thñ!";
			Msg2SubWorld(Msg);
			CloseMission(LDHead.tbMission.MAIN);
			SubWorld = OldSubWorld;
		end;
		
		local Temp = {};
		for i = 1, GetMissionV(LDHead.tbTask.TOTAL_MEMBER) do
			if (GetMissionV(LDHead.tbTask.SWORDMAN + i) > 0) then
				Temp[getn(Temp)+1] = GetMissionV(LDHead.tbTask.SWORDMAN + i);
			end;
		end;
		
		for i = 1, getn(Temp) do
			SetMissionV(LDHead.tbTask.SWORDMAN + i,Temp[i]);
		end;
		
		Temp = {};
		for i = 1, GetMissionV(LDHead.tbTask.TOTAL_MEMBER) do
			PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
			local nPos = 0;
			if ( i == GetMissionV(LDHead.tbTask.TOTAL_MEMBER) and (mod(i,2) ~= 0) ) then
				local Msg = "HÖ thèng kh«ng thÓ s¾p xÕp ®èi thñ cho ®¹i hiÖp!";
				LDHead:JoinHall();		
				Msg2Player(Msg);
			end;
			
			if (mod(i,2) == 0) then
				nPos = floor(i/2);
				SetCurCamp(1);
				SetTask(LDHead.tbTask.RIVAL,GetMissionV(LDHead.tbTask.SWORDMAN + i - 1));
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i - 1);
				szNameRIVAL = GetName();
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
				Msg = "§èi thñ cña ®¹i hiÖp lµ <color=green>"..szNameRIVAL.."<color>.CÈn thËn kh«ng ®­îc khinh ®Þch!";
				Msg2Player(Msg);
				ClearStateSkill(1);
			else
				nPos = floor(i/2) + 1;
				SetCurCamp(2);
				SetTask(LDHead.tbTask.RIVAL,GetMissionV(LDHead.tbTask.SWORDMAN + i + 1));
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i + 1);
				szNameRIVAL = GetName();
				PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
				Msg = "§èi thñ cña ®¹i hiÖp lµ <color=green>"..szNameRIVAL.."<color>.CÈn thËn kh«ng ®­îc khinh ®Þch!";
				Msg2Player(Msg);
				ClearStateSkill(1);
			end;

			SetDeathScript(LDHead.tbMission.DIR_DEATH);
			SetLogoutScript(LDHead.tbMission.DIR_LOGOUT_FIGHTHALL);
			NewWorld(LDHead.Tab_Pos[nPos][1],LDHead.Tab_Pos[nPos][2],LDHead.Tab_Pos[nPos][3]);
			AddMSPlayer(LDHead.tbMission.MAIN,PlayerIndex);
			SetPKFlag(2);
			SetPunish(1);
		end;
	elseif (nTimer < LDHead.tbTimer.TIMER_STARTFIGHT) then
		local nCount = LDHead.tbTimer.TIMER_STARTFIGHT - nTimer;
		if (nCount == 180 or nCount == 120 or nCount == 60 or nCount == 30 or nCount == 10) then 
		Msg2SubWorld("Cßn <color=blue> "..nCount.." <color> gi©y n÷a liªn ®Êu sÏ b¾t ®Çu, c¸c nh©n sÜ h·y nhanh chãng tËp trung héi tr­êng thi ®Êu. ");
		end;
	elseif (nTimer < LDHead.tbTimer.TIMER_READY) then
		nCount = LDHead.tbTimer.TIMER_READY - nTimer;
			Msg2MSAll(LDHead.tbMission.MAIN,"Thêi gian chuÈn bÞ cßn <color=blue>"..nCount.."<color> gi©y!");
	elseif (nTimer == LDHead.tbTimer.TIMER_READY) then
		for i = 1,GetMissionV(LDHead.tbTask.TOTAL_MEMBER) do
			PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
			local nW,_,_ = GetWorldPos();
			if (nW == LDHead.tbMap.SUBWORLD_2 or nW == LDHead.tbMap.SUBWORLD_3 or nW == LDHead.tbMap.SUBWORLD_4) then 
				SetFightState(1);
			end;
		end;
		Msg2MSAll(LDHead.tbMission.MAIN,"Tû thÝ b¾t ®Çu ®¹i hiÖp h·y tung c¸c tuyÖt kü!");
	end;
	if (nTimer > LDHead.tbTimer.TIMER_READY and nTimer < LDHead.tbTimer.TIMER_STOP) then
		for i = 1,GetMissionV(LDHead.tbTask.TOTAL_MEMBER) do
			PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
			local nDamagePlayer	  = GetDamage(GetTask(LDHead.tbTask.RIVAL)); 
			local Attacker =  GetTask(LDHead.tbTask.RIVAL);
			PlayerIndex = Attacker;
			local nDamageAttacker = GetDamage(GetTask(LDHead.tbTask.RIVAL)); 
			PlayerIndex = GetMissionV(LDHead.tbTask.SWORDMAN + i);
			local nW,_,_ = GetWorldPos();
			if (nW == LDHead.tbMap.SUBWORLD_2 or nW == LDHead.tbMap.SUBWORLD_3 or nW == LDHead.tbMap.SUBWORLD_4) then 
				Msg2Player("<color=blue>S¸t th­¬ng b¶n th©n g¸nh chÞu <color> "..nDamageAttacker.." ,<color=blue> S¸t th­¬ng lªn ®èi thñ <color> "..nDamagePlayer.."");
			end;
		end;
	elseif (nTimer  == LDHead.tbTimer.TIMER_STOP) then
		Msg2MSAll(LDHead.tbMission.MAIN,"HÕt thêi gian tû thÝ Liªn ®Êu!");
		CloseMission(LDHead.tbMission.MAIN);
	end;
	end;
end;
