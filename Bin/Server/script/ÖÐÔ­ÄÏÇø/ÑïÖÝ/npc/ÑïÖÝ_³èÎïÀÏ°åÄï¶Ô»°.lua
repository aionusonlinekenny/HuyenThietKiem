--ÖĞÔ­ÄÏÇø ÑïÖİ¸® ³èÎïÀÏ°åÄï¶Ô»°
IncludeLib("SETTING")
Include("\\script\\global\\titlefuncs.lua")
Include("\\script\\lib\\awardtemplet.lua")
Include("\\script\\dailogsys\\dailogsay.lua")
Include("\\script\\global\\fuyuan.lua")
Include("\\script\\lib\\log.lua")
Include("\\script\\task\\newtask\\newtask_head.lua") --µ÷ÓÃ nt_getTask Í¬²½±äÁ¿µ½¿Í»§¶ËµÄÀµ
Include("\\script\\task\\partner\\partner_head.lua") --°üº¬ÁËÍ¼Ïóµ÷ÓÃ
Include("\\script\\task\\partner\\partner_problem.lua") --µ÷ÓÃ nt_getTask Í¬²½±äÁ¿µ½¿Í»§¶ËµÄÀµ
IncludeLib("PARTNER")
IncludeLib("RELAYLADDER");	--ÅÅĞĞ°ñ

function main()
dofile("script/Ö?Ô­ÄÏÇø/ÑïÖ?/npc/ÑïÖ?_³èÎïÀÏ°åÄï¶Ô»°.lua")
dialog_main()
end
tbChat = 
{
	"<color=green>NhËn b¹n §ång hµnh b¬i vµo ®©y",
	"<color=orange>Chóc ®¹i hiÖp ch¬i game vui vÎ, nhí like, share server vldoxanh.com nha!"
}
function npcchat_npcmacdinh()
	local nNpcIndex = GetLastDiagNpc();
	local nTaskChat = GetNpcParam(nNpcIndex,1);
	if nTaskChat == 0 then 
		SetNpcParam(nNpcIndex,1,1); --set index ®Çu tiªn cña table npc chat.
		SetNpcTimer(nNpcIndex,500); --Tuú chØnh thêi gian npc tù chat (20 s)
	end
end
function OnTimer(nNpcIndex,nTimeOut)
	local nTaskChat = GetNpcParam(nNpcIndex,1);
	if not nTaskChat or nTaskChat <= 0 then 
		nTaskChat = 1;
	else
		nTaskChat = nTaskChat + 1;
	end
	if nTaskChat > getn(tbChat) then 	
		nTaskChat = 1;
	end
	NpcChat(nNpcIndex,tbChat[nTaskChat],1);
	SetNpcParam(nNpcIndex,1,nTaskChat)
	SetNpcTimer(nNpcIndex,500);
end

function dialog_main()
npcchat_npcmacdinh()
local szTitle = "<npc> Xin chµo "..GetName().."! T­íng c«ng ta ®· qua ®êi tõ l©u, ®Ó l¹i cho ta tiÖm vËt nu«i nµy. Mét m×nh ta qu¸n xuyÕn trong ngoµi, võa nu«i d¹y con cai võa ph¶i tr«ng coi c¸i tiÖm nµy! ThËt mu«n phÇn vÊt v¶."
tbOpt=
{
	{"NhËn Thó Nu«i", partner_givetodo},
	{"NhËn S¸ch luyÖn cÊp ®ång hµnh", sachnv},
	{"Th«i"},
}
CreateNewSayEx(szTitle, tbOpt)
end

function partner_givetodo()                                
	local partnercount = PARTNER_Count()                  
	if ( partnercount == -1 ) then
		Msg2player(".....................................")
	elseif ( partnercount == 5 ) then
		Describe("Bµ chñ tiÖm vËt nu«i: Ng­¬i ®· cã nhiÒu b¹n ®ång hµnh, ®õng cã tham n÷a",1, 
                "KÕt thóc ®èi tho¹i /no")
	else
		Describe("Bµ chñ tiÖm vËt nu«i: NÕu mµ ng­¬i ®· FA l©u n¨m nh­ thÕ th× ta gióp dc, lùa chän 1 ng­êi huynh ®Ö ®i nµo !",8, 
               "NhËn b¹n ®ång hµnh hÖ kim/kim", 
               "NhËn b¹n ®ång hµnh hÖ méc /moc", 
               "NhËn b¹n ®ång hµnh hÖ thñy/thuy", 
               "NhËn b¹n ®ång hµnh hÖ háa /hoa", 
               "NhËn b¹n ®ång hµnh hÖ thæ /tho", 
               "KÕt thóc ®èi tho¹i /no")
	end
end

function tho()
 local partneridex = PARTNER_AddFightPartner(4,4,1,5,5,5,5,5,5)
PARTNER_CallOutCurPartner(1)
end
function moc()
 local partneridex = PARTNER_AddFightPartner(1,1,1,5,5,5,5,5,5)
PARTNER_CallOutCurPartner(1)
end
function thuy()
 local partneridex = PARTNER_AddFightPartner(2,2,1,5,5,5,5,5,5)
PARTNER_CallOutCurPartner(1)
end
function hoa()
 local partneridex = PARTNER_AddFightPartner(3,3,1,5,5,5,5,5,5)
PARTNER_CallOutCurPartner(1)
end
function kim()
 local partneridex = PARTNER_AddFightPartner(5,0,1,5,5,5,5,5,5)
PARTNER_CallOutCurPartner(1)
end

function sachnv()
local szTitle = "<npc> Xin chµo "..GetName().."! T­íng c«ng ta ®· qua ®êi tõ l©u, ®Ó l¹i cho ta tiÖm vËt nu«i nµy. Mét m×nh ta qu¸n xuyÕn trong ngoµi, võa nu«i d¹y con cai võa ph¶i tr«ng coi c¸i tiÖm nµy! ThËt mu«n phÇn vÊt v¶."
local tbOpt =
	{
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 1-20",sach20},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 20-40",sach40},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 40-60",sach60},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 60-80",sach80},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 80-100",sach100},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 100-120",sach120},
		{"Trang Sau",sachnv2},
		{"Tho¸t"},
	}
	CreateNewSayEx(szTitle, tbOpt)
end

function sachnv2()
local szTitle = "<npc> Xin chµo "..GetName().."! T­íng c«ng ta ®· qua ®êi tõ l©u, ®Ó l¹i cho ta tiÖm vËt nu«i nµy. Mét m×nh ta qu¸n xuyÕn trong ngoµi, võa nu«i d¹y con cai võa ph¶i tr«ng coi c¸i tiÖm nµy! ThËt mu«n phÇn vÊt v¶."
local tbOpt =
	{
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 120-140",sach140},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 140-160",sach160},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 160-180",sach180},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 180-200",sach200},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 200-220",sach220},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 220-240",sach240},
		{"Trang Sau",sachnv3},
		{"Trë l¹i",sachnv},
		{"Tho¸t"},
	}
	CreateNewSayEx(szTitle, tbOpt)
end

function sachnv3()
local szTitle = "<npc> Xin chµo "..GetName().."! T­íng c«ng ta ®· qua ®êi tõ l©u, ®Ó l¹i cho ta tiÖm vËt nu«i nµy. Mét m×nh ta qu¸n xuyÕn trong ngoµi, võa nu«i d¹y con cai võa ph¶i tr«ng coi c¸i tiÖm nµy! ThËt mu«n phÇn vÊt v¶."
local tbOpt =
	{
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 240-260",sach260},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 260-280",sach280},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 280-300",sach300},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 300-320",sach320},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 320-340",sach340},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 340-360",sach360},
		{"NhËn s¸ch luyÖn cÊp ®ång hµnh 340-381",sach380},
		{"Trë l¹i",sachnv2},
		{"Tho¸t"},
	}
	CreateNewSayEx(szTitle, tbOpt)
end


function sach20()
for i =447,466 do
AddItem(6,1,i,0,0,0)
end
end

function sach40()
for i =467,486 do
AddItem(6,1,i,0,0,0)
end
end

function sach60()
for i =487,506 do
AddItem(6,1,i,0,0,0)
end
end

function sach80()
for i =507,526 do
AddItem(6,1,i,0,0,0)
end
end

function sach100()
for i =527,546 do
AddItem(6,1,i,0,0,0)
end
end

function sach120()
for i =547,566 do
AddItem(6,1,i,0,0,0)
end
end

function sach140()
for i =567,586 do
AddItem(6,1,i,0,0,0)
end
end

function sach160()
for i =587,606 do
AddItem(6,1,i,0,0,0)
end
end

function sach180()
for i =607,626 do
AddItem(6,1,i,0,0,0)
end
end

function sach200()
for i =627,646 do
AddItem(6,1,i,0,0,0)
end
end

function sach220()
for i =647,666 do
AddItem(6,1,i,0,0,0)
end
end

function sach240()
for i =667,686 do
AddItem(6,1,i,0,0,0)
end
end

function sach260()
for i =687,706 do
AddItem(6,1,i,0,0,0)
end
end

function sach280()
for i =707,726 do
AddItem(6,1,i,0,0,0)
end
end

function sach300()
for i =727,746 do
AddItem(6,1,i,0,0,0)
end
end

function sach320()
for i =747,766 do
AddItem(6,1,i,0,0,0)
end
end

function sach340()
for i =767,786 do
AddItem(6,1,i,0,0,0)
end
end

function sach360()
for i =787,806 do
AddItem(6,1,i,0,0,0)
end
end

function sach380()
for i =807,827 do
AddItem(6,1,i,0,0,0)
end
end