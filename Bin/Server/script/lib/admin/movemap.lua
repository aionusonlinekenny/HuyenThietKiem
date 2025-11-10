-- Author: Kinnox
-- NameTool: Lenh Bai Gm
-- Option: Move Maps

---Toa do boss
POSITION_ARRAY_BOSS={
{78,1740,3323}, -- vi tri 1 tuong duong
{78,1677,3541}, -- vi tri 2 tuong duong	
{1,1550,2790},
{1,1550,2790},
{80,1389,3410},
{80,1389,3410},
{37,1559,3449},
{37,1655,2627},
};
---Toa do Tong Kim
POSITION_ARRAY_TONGKIM={
{380,1440,3440}, -- Giua ban do
{380,1567,3205}, -- Ben ngoai doanh trai Kim
{380,1330,3445}, -- Ben ngoai doanh trai Tong
{380,1688,3072}, -- Ben trong doanh trai Kim
{380,1242,3549}, -- Ben trong doanh trai Tong
};
function MoveMaps()
nW,nX,nY = GetWorldPos()
	local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY..""
	Tab_inSert = {
	"Di chuyÓn vµo Tèng Kim/MoveTK",
	"Di chuyÓn ®Õn Boss Hoµng Kim/MoveBoss",
	"Di chuyÓn ®Õn b¶n ®å V­ît ¶i/MoveVuotAi",
	"Di chuyÓn ®Õn b¶n ®å Liªn ®Êu/MoveLienDau",
	"Di chuyÓn ®Õn b¶n ®å Phong L¨ng §é/MovePLD",
	"Di chuyÓn ®Õn Ba l¨ng huyÖn/MoveBLH",
	"Ta kh«ng cÇn ng¬i d¹y b¶o/no"
	}
Say(szHello,getn(Tab_inSert),Tab_inSert)	
end;

----------Di chuyen den ban do tong kim ----------

function MoveTK()
nW,nX,nY = GetWorldPos()
	local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY..""
	Tab_inSert = {
	"Gi÷a b¶n ®å/ExMoveTK",
	"Bªn ngoµi doanh tr¹i Kim/ExMoveTK",
	"Bªn ngoµi oanh tr¹i Tèng/ExMoveTK",
	"Bªn trong doanh tr¹i Kim/ExMoveTK",
	"Bªn trong doanh tr¹i Tèng/ExMoveTK",
	"Ta kh«ng cÇn ng¬i d¹y b¶o/no"
	}
Say(szHello,getn(Tab_inSert),Tab_inSert)
end;
function ExMoveTK(nSel)
local nSel = nSel + 1;
	NewWorld(POSITION_ARRAY_TONGKIM[nSel][1],POSITION_ARRAY_TONGKIM[nSel][2],POSITION_ARRAY_TONGKIM[nSel][3]);
	SetFightState(0);
end
----------Di chuyen den ban do boss hoang kim ----------

function MoveBoss()
nW,nX,nY = GetWorldPos()
	local szHello ="<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY..""
	Tab_inSert = {
	"Boss T­¬ng D­¬ng vÞ trÝ 1/ExMoveBoss",
	"Boss T­¬ng D­¬ng vÞ trÝ 2/ExMoveBoss",
	"Boss Ph­îng T­êng vÞ trÝ 1/ExMoveBoss",
	"Boss Ph­îng T­êng vÞ trÝ 2/ExMoveBoss",
	"Boss D­¬ng Ch©u vÞ trÝ 1/ExMoveBoss",
	"Boss D­¬ng Ch©u vÞ trÝ 2/ExMoveBoss",
	"Boss BiÖn Kinh vÞ trÝ 1/ExMoveBoss",
	"Boss BiÖn Kinh vÞ trÝ 2/ExMoveBoss",
	"Ta kh«ng cÇn ng¬i d¹y b¶o/no"
	}
Say(szHello,getn(Tab_inSert),Tab_inSert)
end;

function ExMoveBoss(nSel)
local nSel = nSel + 1;
	NewWorld(POSITION_ARRAY_BOSS[nSel][1],POSITION_ARRAY_BOSS[nSel][2],POSITION_ARRAY_BOSS[nSel][3]);
	SetFightState(1);
end;
----------Di chuyen den ban do vuot ai ----------

function MoveVuotAi()
AskClientForNumber("ExMoveVuotAi",0,1314,"NhËp ID Maps.")
end;

function ExMoveVuotAi(nIndex,nIDMap)
local nNum = tonumber(nIDMap);
 NewWorld(nNum,1560,3216)
 SetFightState(0);
end;

----------Di chuyen den ban do Lien Dau ----------

function MoveLienDau()
	NewWorld(396, 1614,3203)
	SetFightState(0);
end;

----------Di chuyen den ban do Phong Lang do ----------

function MovePLD()
nW,nX,nY = GetWorldPos()
	local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, Minh Qu©n xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY..""
	Tab_inSert = {
	"B¶n ®å Phong L¨ng §é BÕn 1/ExMovePLD",
	"B¶n ®å Phong L¨ng §é BÕn 2/ExMovePLD",
	"B¶n ®å Phong L¨ng §é BÕn 3/ExMovePLD",
	"Ta kh«ng cÇn ng¬i d¹y b¶o/no"
	}
Say(szHello,getn(Tab_inSert),Tab_inSert)
end;

TAB_PLD = 
{
337,
338,
339
};

function ExMovePLD(nSel)
	local nSel = nSel + 1;
	local nMapsID = 0;
	if (nSel == 1) then
		nMapsID = TAB_PLD[1];
	elseif (nSel == 2) then
		nMapsID = TAB_PLD[2];
	elseif (nSel == 3) then
		nMapsID = TAB_PLD[3];
	end;
	NewWorld(nMapsID,1616,3204)
	SetFightState(0);
end;

function MoveBLH()
	NewWorld(53,1582, 3237);
	SetFightState(0);
end