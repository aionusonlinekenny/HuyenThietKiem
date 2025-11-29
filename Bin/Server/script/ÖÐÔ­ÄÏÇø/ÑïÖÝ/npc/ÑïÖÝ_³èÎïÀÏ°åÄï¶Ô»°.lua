--��ԭ���� ���ݸ� �����ϰ���Ի�

-- Stub for IncludeLib if not available
if not IncludeLib then
	function IncludeLib(szLibName) end
end

IncludeLib("SETTING")
Include("\\script\\global\\titlefuncs.lua")
Include("\\script\\lib\\awardtemplet.lua")
Include("\\script\\dailogsys\\dailogsay.lua")
Include("\\script\\global\\fuyuan.lua")
Include("\\script\\lib\\log.lua")
Include("\\script\\task\\newtask\\newtask_head.lua") --���� nt_getTask ͬ���������ͻ��˵���
Include("\\script\\task\\partner\\partner_head.lua") --������ͼ�����
Include("\\script\\task\\partner\\partner_problem.lua") --���� nt_getTask ͬ���������ͻ��˵���
IncludeLib("PARTNER")
IncludeLib("RELAYLADDER");	--���а�

function main()
dofile("script/�?ԭ����/���?/npc/���?_�����ϰ���Ի�.lua")
dialog_main()
end
tbChat = 
{
	"<color=green>Nh�n b�n ��ng h�nh b�i v�o ��y",
	"<color=orange>Ch�c ��i hi�p ch�i game vui v�, nh� like, share server vldoxanh.com nha!"
}
function npcchat_npcmacdinh()
	local nNpcIndex = GetLastDiagNpc();
	local nTaskChat = GetNpcParam(nNpcIndex,1);
	if nTaskChat == 0 then 
		SetNpcParam(nNpcIndex,1,1); --set index ��u ti�n c�a table npc chat.
		SetNpcTimer(nNpcIndex,500); --Tu� ch�nh th�i gian npc t� chat (20 s)
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
local szTitle = "<npc> Xin ch�o "..GetName().."! T��ng c�ng ta �� qua ��i t� l�u, �� l�i cho ta ti�m v�t nu�i n�y. M�t m�nh ta qu�n xuy�n trong ngo�i, v�a nu�i d�y con cai v�a ph�i tr�ng coi c�i ti�m n�y! Th�t mu�n ph�n v�t v�."
tbOpt=
{
	{"Nh�n Th� Nu�i", partner_givetodo},
	{"Nh�n S�ch luy�n c�p ��ng h�nh", sachnv},
	{"Th�i"},
}
CreateNewSayEx(szTitle, tbOpt)
end

function partner_givetodo()                                
	local partnercount = PARTNER_Count()                  
	if ( partnercount == -1 ) then
		Msg2player(".....................................")
	elseif ( partnercount == 5 ) then
		Describe("B� ch� ti�m v�t nu�i: Ng��i �� c� nhi�u b�n ��ng h�nh, ��ng c� tham n�a",1, 
                "K�t th�c ��i tho�i /no")
	else
		Describe("B� ch� ti�m v�t nu�i: N�u m� ng��i �� FA l�u n�m nh� th� th� ta gi�p dc, l�a ch�n 1 ng��i huynh �� �i n�o !",8, 
               "Nh�n b�n ��ng h�nh h� kim/kim", 
               "Nh�n b�n ��ng h�nh h� m�c /moc", 
               "Nh�n b�n ��ng h�nh h� th�y/thuy", 
               "Nh�n b�n ��ng h�nh h� h�a /hoa", 
               "Nh�n b�n ��ng h�nh h� th� /tho", 
               "K�t th�c ��i tho�i /no")
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
local szTitle = "<npc> Xin ch�o "..GetName().."! T��ng c�ng ta �� qua ��i t� l�u, �� l�i cho ta ti�m v�t nu�i n�y. M�t m�nh ta qu�n xuy�n trong ngo�i, v�a nu�i d�y con cai v�a ph�i tr�ng coi c�i ti�m n�y! Th�t mu�n ph�n v�t v�."
local tbOpt =
	{
		{"Nh�n s�ch luy�n c�p ��ng h�nh 1-20",sach20},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 20-40",sach40},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 40-60",sach60},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 60-80",sach80},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 80-100",sach100},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 100-120",sach120},
		{"Trang Sau",sachnv2},
		{"Tho�t"},
	}
	CreateNewSayEx(szTitle, tbOpt)
end

function sachnv2()
local szTitle = "<npc> Xin ch�o "..GetName().."! T��ng c�ng ta �� qua ��i t� l�u, �� l�i cho ta ti�m v�t nu�i n�y. M�t m�nh ta qu�n xuy�n trong ngo�i, v�a nu�i d�y con cai v�a ph�i tr�ng coi c�i ti�m n�y! Th�t mu�n ph�n v�t v�."
local tbOpt =
	{
		{"Nh�n s�ch luy�n c�p ��ng h�nh 120-140",sach140},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 140-160",sach160},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 160-180",sach180},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 180-200",sach200},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 200-220",sach220},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 220-240",sach240},
		{"Trang Sau",sachnv3},
		{"Tr� l�i",sachnv},
		{"Tho�t"},
	}
	CreateNewSayEx(szTitle, tbOpt)
end

function sachnv3()
local szTitle = "<npc> Xin ch�o "..GetName().."! T��ng c�ng ta �� qua ��i t� l�u, �� l�i cho ta ti�m v�t nu�i n�y. M�t m�nh ta qu�n xuy�n trong ngo�i, v�a nu�i d�y con cai v�a ph�i tr�ng coi c�i ti�m n�y! Th�t mu�n ph�n v�t v�."
local tbOpt =
	{
		{"Nh�n s�ch luy�n c�p ��ng h�nh 240-260",sach260},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 260-280",sach280},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 280-300",sach300},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 300-320",sach320},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 320-340",sach340},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 340-360",sach360},
		{"Nh�n s�ch luy�n c�p ��ng h�nh 340-381",sach380},
		{"Tr� l�i",sachnv2},
		{"Tho�t"},
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