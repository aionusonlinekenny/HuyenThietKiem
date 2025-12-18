-- Author: kinnox
-- Date: 23/08/2021
-- Lib: Than hanh phu
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\Revivepos.lua")
Include("\\script\\system_config.lua")


----------------------------------------------
SELECT_GO = "ThÇn hµnh phï, ®i ®Õn n¬i ng­¬i muèn."
SELECT_REVIVE = "ThiÕt ®Æt ®iÓm håi sinh cho lÇn sau sö dông thæ ®Þa phï."
SELECT_OUT = "Rêi khái/no"
SUCCESS = "ThiÕt ®Æt ®iÓm håi sinh t¹i %s thµnh c«ng."
PLACE_TOGO = "H·y lùa chän n¬i cÇn ®Õn"

function OnUse(nIndex)
 	-- dofile("script/item/luascript/townportal_l.lua")
	if (ActionOpen() <= 0) then
	Talk(1,"","Vïng ®Êt nµy hiÖn t¹i ch­a khai th«ng, c¸c h¹ vui lßng ®îi m¸y chñ <color=orange>Vâ L©m HuyÒn ThiÕt KiÕm<color> <color=red>Open Beta<color>");
	return
	end;
	thanhanhphu(nIndex)
end

function thanhanhphu(nIndex)
	local nW, nX, nY = GetWorldPos()
	if CheckMapNoForTHP(nW) == 1  or GetTask(601) > 0 then
		Talk(1,"", "N¬i nµy kh«ng thÓ sö dông <color=green>Thæ ®Þa phï <color>.")
	return end	

	Say(SELECT_GO,4,
	"ThiÕt ®Æt ®iÓm håi sinh ®Ó sö dông thæ ®Þa phï./diemphucsinh",
	"Sö dông thuËt thÇn hµnh ®i ®Õn n¬i chØ ®Þnh./chondiadiem",
	"Quay l¹i n¬i cò /quaylai",
	SELECT_OUT)
end

function quaylai()
	-- if ((GetItemCountInBag(7,18,0,0) >= 1) or  (GetItemCountInBag(7,19,0,0) >= 1)) then
		ReturnFromPortal();
		if (GetFightState() > 0) then
			AddSkillState(963, 1, 0, 18*3);
		end;
	-- else
		-- Talk(1,"","§¹i hiÖp kh«ng cã Thæ §Þa Phï, nÕu kh«ng mua sÏ bÞ kÑt l¹i b¶n ®å luyÖn c«ng.")
	-- end
end;

function chondiadiem()
	Say(SELECT_GO,5,
	SELECT_OUT,
	"Thµnh thÞ /thanhthi",
	"Th«n trang/thon",
--	"M«n ph¸i/monphai",
	"ChiÕn tr­êng Tèng Kim/tongkim",
	"B¶n ®å luyÖn c«ng/bando1"
	)
end;

function thanhthi()
	local count = 1;
	local CityTab = {}; -- khoi tao mang
	
	for i=1,getn(THANH_ARRAY) do
		CityTab[count] = THANH_ARRAY[i][2] .."/chonthanh";	-- 
		count = count + 1;
	end
	if(count == 1) then
	return end;
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO,count,CityTab)
end;

function chonthanh(nNo)
	local nSel = nNo + 1;
	local count = 1;
	local CityPosTab = {};
	SetTaskTemp(TMP_INDEX_ITEM,nSel);
	for i=1,getn(THANH_ARRAY[nSel][3]) do
		CityPosTab[count] = THANH_ARRAY[nSel][3][i][3] .."/posthanh";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityPosTab[count] = SELECT_OUT;
	Say(SELECT_GO,count,CityPosTab);
end;

function posthanh(nNo)
	local nSel = nNo + 1;
	local nMoveAR = GetTaskTemp(TMP_INDEX_ITEM);
	local nW = THANH_ARRAY[nMoveAR][1];
	local nX = THANH_ARRAY[nMoveAR][3][nSel][1];
	local nY = THANH_ARRAY[nMoveAR][3][nSel][2];
	if(nX ~= nil and nX > 0) then
	if(NewWorld(nW,floor(nX/32),floor(nY/32)) == 1) then
	SetFightState(0)
	AddSkillState(963, 1, 0, 18*3);
	end
	end;
end;

function thon()
	local count = 1;
	local CityTab = {};
	for i=1,getn(THON_ARRAY) do
		CityTab[count] = THON_ARRAY[i][4] .."/chonthon";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO,count,CityTab)
end;

function chonthon(nNo)
	local nSel = nNo + 1
	local nW = THON_ARRAY[nSel][1];
	local nX = THON_ARRAY[nSel][2];
	local nY = THON_ARRAY[nSel][3];
	if(nX ~= nil and nX > 0) then
	if(NewWorld(nW,floor(nX/32),floor(nY/32)) == 1) then
	SetFightState(0)
	AddSkillState(963, 1, 0, 18*3)
	end
	end;
end

function monphai()
	if(1 == 1) then
	Talk(1,"","Chøc n¨ng t¹m thêi ch­a më!");
	return end
	local count = 1;
	local CityTab = {};
	
	for i=1,getn(MONPHAI_ARRAY) do
		CityTab[count] = MONPHAI_ARRAY[i][2] .."/chonmonphai";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO,count,CityTab)
end;

function chonmonphai(nNo)
	local nSel = nNo + 1;
	local x,y = GetRevPos(MONPHAI_ARRAY[nSel][3],MONPHAI_ARRAY[nSel][1]);
	if(x ~= nil and x > 0) then
	if(NewWorld(MONPHAI_ARRAY[nSel][1],floor(x/32),floor(y/32))==1) then
	SetFightState(0)
	AddSkillState(963, 1, 0, 18*3)
	end
	end;
end;

function tongkim()
	Say(PLACE_TOGO,3,
	SELECT_OUT,
	"§iÓm b¸o danh bªn Tèng/ditong",
	"§iÓm b¸o danh bªn Kim/dikim"
	)
end;

function ditong()
	if(NewWorld(325,1541, 3178) == 1) then
		SetFightState(0)
	end
end;

function dikim()
	if(NewWorld(325,1570, 3085) == 1) then
		SetFightState(0)
	end
end;

function bando1() 
	if GetLevel() < 80 then
	Msg2Player("Thùc lùc cña nhµ ng­êi kh«ng thÎ ra ngoµi kia nguy hiÓm ®­îc");
	return
	end;
	
	local count = 1;
	local CityTab = {};
	
	for i=1,getn(PLACES_ARRAY1) do
		CityTab[count] = GetWayPointName(PLACES_ARRAY1[i]).."/chonbando1";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityTab[count] = SELECT_OUT;
	Say(PLACE_TOGO,count,CityTab)
end;

function chonbando1(nNo)
	local nSel = nNo + 1;
	local w,x,y = GetWayPointPos(PLACES_ARRAY1[nSel]);
	if(NewWorld(w,x,y) == 1) then
		SetFightState(1)
		AddSkillState(963, 1, 0, 18*3);
	end;
end;


function diemphucsinh()
	Say(SELECT_REVIVE,3,
	SELECT_OUT,
	"Thµnh thÞ /psthanhthi",
	"Th«n trang/psthon"
	-- "M«n ph¸i/psmonphai"
	)
end;

function psthanhthi()
	local count = 1;
	local CityTab = {};
	
	for i=1,getn(THANH_ARRAY) do
		CityTab[count] = THANH_ARRAY[i][2] .."/pschonthanh";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE,count,CityTab)
end;

function pschonthanh(nNo)
	local nSel = nNo + 1;
	local count = 1;
	local CityPosTab = {};
	SetTaskTemp(TMP_INDEX_ITEM,nSel);
	for i=1,getn(THANH_ARRAY[nSel][3]) do
		CityPosTab[count] = THANH_ARRAY[nSel][3][i][3] .."/psposthanh";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityPosTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE,count,CityPosTab)
end;

function psposthanh(nNo)
	local nSel = nNo + 1;
	local nMoveAR = GetTaskTemp(TMP_INDEX_ITEM);
	SetRevPos(THANH_ARRAY[nMoveAR][1],THANH_ARRAY[nMoveAR][3][nSel][4]);
	Talk(1,"", format(SUCCESS,THANH_ARRAY[nMoveAR][3][nSel][3]))
end;

function psthon()
	local count = 1;
	local CityTab = {};
	for i=1,getn(THON_ARRAY) do
		CityTab[count] = THON_ARRAY[i][4] .."/pschonthon";
		count = count + 1;
	end
	if(count == 1) then
	return end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE,count,CityTab)
end;

function pschonthon(nNo)
	local nSel = nNo + 1;
	SetRevPos(THON_ARRAY[nSel][1],THON_ARRAY[nSel][5]);
	Talk(1,"", format(SUCCESS,THON_ARRAY[nSel][4]))
end;

function no()
end;
