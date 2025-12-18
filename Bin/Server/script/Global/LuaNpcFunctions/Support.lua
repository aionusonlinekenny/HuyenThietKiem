-- Author:	Kinnox;
-- Date:	28-05-2021
-- Functions: Support;
Include("\\script\\lib\\tasklib.lua");
Include("\\script\\GiftCode\\head.lua")
Include("\\script\\GiftCode\\tb_head.lua")
Include("\\script\\Global\\LuaNpcFactions\\faction_head.lua")
Include("\\script\\system_config.lua")
Support = {};
ERROR_LIST ={"C¸c h¹ ®· nhËn vËt phÈm nµy råi","C¸c h¹ ch­a ®¹t ®¼ng cÊp 20 h·y rÌn luyÖn thªm!","C¸c h¹ ch­a ®¹t ®¼ng cÊp 50 h·y rÌn luyÖn thªm!",
"C¸c h¹ ch­a ®¹t ®¼ng cÊp 80 h·y rÌn luyÖn thªm!","C¸c h¹ ch­a ®¹t ®¼ng cÊp 120 h·y rÌn luyÖn thªm!"};
function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/Support.lua");
		Msg2Player("Reload this script");
	end;
local tbSay = {
		"NhËn phÇn th­ëng Hç Trî T©n Thñ/SP_newbie",
		"Gift code hç trî ®ång ®¹o/SP_giftcode",
		"NhËn phÇn th­ëng lªn cÊp/SP_Level",
		"NhËn bÝ kÝp kü n¨ng cÊp 80/SP_Skill",
		"Ta chØ ghÐ ngang qua/no",
	}
	Say("<color=orange>Npc Hç Trî <color>: Ph¶i! Ta chÝnh lµ s­ gi¶ mang nh÷ng lîi Ých lín ®Õn cho nhµ ng­¬i. H·y nãi cha ta biÕt ng­êi cÇn g×?",getn(tbSay),tbSay);
end;

function SP_newbie(nNpcIdx)	
	if (GetTask(T_REWARD) > 0) then
		Talk(1,"",ERROR_LIST[1]);
		return 0
	end;
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;
	OnSuccessAdd();
end;

function OnSuccessAdd()
---Trang bi kim phong
	for i = 177,185 do
	nIndex = AddGoldItem(i); -- Item Kim Phong
	SetItemBindState(nIndex,2);
	end;
	for i = 19,20 do
	nIndex = AddScriptItem(i); -- Tho Dia Phu
	SetItemBindState(nIndex,2);
	end;
	 for i = 1,3 do
	 nIndex = AddScriptItem(3); -- Item Tien Thao Lo [Tieu]
	 SetItemBindState(nIndex,2);
	 end;

	-- for i = 1,2 do
	-- nIndex = AddScriptItem(5);-- Item Thien Son Bao Lo [Tieu]
	-- SetItemBindState(nIndex,2);
	-- end;

	
	Earn(1000);
	
	while GetLevel() < 90 do
		AddOwnExp(9E9);
	end;
	
	SetTask(T_REWARD,1);
	Msg2Player("§¹i hiÖp nhËn ®­îc 1 Bé trang bÞ Kim Phong, 5 Tiªn th¶o lé [TiÓu], 2 Thiªn s¬n b¶o lé, 1 ThÇn hµnh phï, 1 Thæ ®Þa phï! ");
end;

function SP_giftcode(nNpcIdx)
	if (FindEmptyPlace(6,6) == 0) then
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end
	AskClientForString("GiftCodeCheck","",1,"30","NhËp Gift Code");
end

function GiftCodeCheck(nNum,sCode)
	sCode = tostring(sCode)
	if(not sCode) then
		Talk(1,"","§¹i hiÖp ch­a nhËp Gif Code!!!");
		return
	end
	
	-- local tbTmp = GIFT_CODE
	-- if(not tbTmp) then
		-- return
	-- end
	
	-- local nKey = SearchGift(tbTmp, sCode)
	-- if(nKey == 0) then
		-- Talk(1,"","<color=red>Gift Code §¹i hiÖp nhËp kh«ng tån t¹i!<color>");
		-- return
	-- end
	-- local nEc = CheckVaildCode(tbTmp, nKey)
	-- if(nEc == -1) then
		-- Talk(1,"","<color=red>HÖ thèng lçi, vui lßng liªn hÖ Game Master!<color>");
		-- return
	-- end
	-- if(nEc == 0) then
		-- Talk(1,"","<color=red>Gift Code ®· ®­îc sö dông råi!<color>");
		-- return
	-- end
	-- tbTmp = SetUsedCode(tbTmp, nKey);
	-- if(not tbTmp) then
		-- print("Save Table GiftCode Error!");
	-- end
	-- local sTmp = CreateGiftTB(tbTmp, "GIFT_CODE", "")	;	
	-- SaveGiftTB("script/giftcode/tb_head.lua", sTmp);
	-- OnSuccessGift();
	if (sCode == "JX-ONLINE.COM") then
		if(GetTask(T_GIFTCODE) > 0) then
		Talk(1,"","§¹i hiÖp ®· dïng gift code råi ")
		return 0
		end
		OnSuccessGift();
	end;
	
	-- if (sCode == "TICHTINHDONG") then
		-- if (GetTask(T_HOTROOLDMEMBER) > 0) then
			-- Talk(1,"","C¸c h¹ ®· nhËn råi");
			-- return 0
		-- end;
		
		-- if (GetLevel() <= 85) then
			-- Talk(1,"","ChØ cã ng­êi ch¬i tr­íc míi cã thÓ nhËp gift code nµy!");
			-- return 0
		-- end;
		-- GiftDenBu();
	-- end;	
end;

function GiftDenBu()
	for i = 1,2 do
	nIndex = AddTaskItem(26,7);-- 7 date; lenh bai Tich tinh dong
	SetItemBindState(nIndex,2);
	end;
	
	-- for i = 1,3 do
	-- nIndex = AddScriptItem(14,7);-- 7 date; Item nu nhi hong [Dai]
	-- SetItemBindState(nIndex,2);
	-- end;
	Msg2Player("Chóc mõng ®¹i hiÖp ®· nhËn ®­îc 2 lÖnh bµi tÝch t×nh ®éng");
	SetTask(T_HOTROOLDMEMBER,1);
end;


function OnSuccessGift()
	if(GetTask(T_GIFTCODE) == 0) then
		AwardCodeAdd()
	else
		Talk(1,"","§¹i hiÖp ®· dïng gift code råi ")
	end;
end;

function AwardCodeAdd()

	--nIndex = AddScriptItem(4);-- 7 date; Item Tien Thao Lo [Dai]
	--SetItemBindState(nIndex,2);

		
	--nIndex = AddScriptItem(6);-- 7 date; Item Que Hoa Tuu [Dai]
	--SetItemBindState(nIndex,2);

	--nIndex = AddTaskItem(15); -- tinh hong bao thach
	--SetItemBindState(nIndex,2);
	--nIndex = AddTaskItem(15);
	--SetItemBindState(nIndex,2);
	--nIndex = AddTaskItem(15);
	--SetItemBindState(nIndex,2);

	--for i = 7,11 do
	--	nIndex = AddTaskItem(i); -- sat thu gian nguyen bo
	--	SetItemBindState(nIndex,2);
	--end;

	--for i = 1,2 do
		--nIndex = AddTaskItem(26); -- lenh bai tich tinh dong
		--SetItemBindState(nIndex,2);
		--nIndex = AddTaskItem(12);
		--SetItemBindState(nIndex,2);
		--nIndex = AddScriptItem(14);
		--SetItemBindState(nIndex,2);
	--end;

	--for i = 1,30 do
		--nIndex = AddTaskItem(28);
		--SetItemBindState(nIndex,2);
	--end;
	nIndex = AddScriptItem(19);-- 15 date; Tho dia phu 
	SetItemBindState(nIndex,2);
		
	nIndex = AddScriptItem(20);-- 15 date; Than hanh phu
	SetItemBindState(nIndex,2);
	Msg2Player("Chóc mõng ®¹i hiÖp ®· nhËp thµnh c«ng m· Gift Code, nhËn ®­îc phÇn th­ëng tri ©n tõ m¸y chñ.");
	SetTask(T_GIFTCODE,1);
end;

function SP_Level()
	local tbSay = {
		"NhËn phÇn th­ëng lªn cÊp 20/Reward_20",
		"NhËn phÇn th­ëng lªn cÊp 50/Reward_50",
		"NhËn phÇn th­ëng lªn cÊp 80/Reward_80",
		"NhËn phÇn th­ëng lªn cÊp 120/Reward_120",
		"Ta chØ ghÐ ngang qua/no",
	}
	Say(10227,getn(tbSay),tbSay);
end;

function Reward_20()
	
	if (GetTask(T_REWARD20) > 0) then
		Talk(1,"",ERROR_LIST[1]);
		return 0
	end;
	
	if (GetLevel() < 20) then
		Talk(1,"",ERROR_LIST[2]);
		return 0
	end;
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;
	
	for i = 1,2 do
		nIndex = AddScriptItem(3,7);-- 7 date; Item Tien Thao Lo [Tieu]
		SetItemBindState(nIndex,2);
	end;
	nIndex = AddItem(0,10,2,1,0,0,0);
	SetItemBindState(nIndex,2);
	Earn(10000);
	SetTask(T_REWARD20,1);
end;

function Reward_50()

	if (GetTask(T_REWARD50) > 0) then
		Talk(1,"",ERROR_LIST[1]);
		return 0
	end;
	
	if (GetLevel() < 50) then
		Talk(1,"",ERROR_LIST[3]);
		return 0
	end;
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;
	
	-- for i = 1,2 do
	-- 	nIndex = AddScriptItem(3,7);-- 7 date; Item Tien Thao Lo [Tieu]
	-- 	SetItemBindState(nIndex,2);
	-- end;
	nIndex = AddItem(0,10,2,10,0,0,0);
	SetItemBindState(nIndex,2);
	Earn(2000);
	SetTask(T_REWARD50,1);
end;

function Reward_80()
	local nRandom =random(1,10)
	
	if (GetTask(T_REWARD80) > 0) then
		Talk(1,"",ERROR_LIST[1]);
		return 0
	end;
	
	if (GetLevel() < 80) then
		Talk(1,"",ERROR_LIST[4]);
		return 0
	end;
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;
	
	-- for i = 1,3 do
	-- 	nIndex = AddScriptItem(4,7);-- 7 date; Item Tien Thao Lo [Dai]
	-- 	SetItemBindState(nIndex,2);
	-- end;
	
	-- for i = 1,3 do
	--	nIndex = AddScriptItem(6,7);-- 7 date; Que Hoa Tuu
	--	SetItemBindState(nIndex,2);
	--end;
	
	nIndex = AddTaskItem(16); -- lam thuy tinh
	SetItemBindState(nIndex,2);
	
	nIndex = AddTaskItem(17); -- tu thuy tinh
	SetItemBindState(nIndex,2);
	
	nIndex = AddTaskItem(18); -- luc thuy tinh
	SetItemBindState(nIndex,2);
	
	nIndex = AddScriptItem(70,7); -- tui mau 7 ngay
	SetItemBindState(nIndex,2);
	
	for  i = 1,3 do
	nIndex = AddScriptItem(14,7); -- x2 luyen skills 7 ngay
	SetItemBindState(nIndex,2);
	end;
	
	if (nRandom == 5) then -- chieu da ngoc su du cap 5
		AddItem(0,10,5,nRandom,0,0,0);
	elseif (nRandom == 10) then -- chieu da ngoc su du cap 10	
		AddItem(0,10,5,nRandom,0,0,0);
	elseif (nRandom < 5) then
	nIndex = AddItem(0,10,5,nRandom+random(1,4),0,0,0); -- cac loai ngua khac
	SetItemBindState(nIndex,2);
	elseif (nRandom > 5 and nRandom < 10) then
	nIndex = AddItem(0,10,5,10-nRandom,0,0,0); -- cac loai ngua khac
	SetItemBindState(nIndex,2);
	end;
	Earn(2000);
	SetTask(T_REWARD80,1);
end;

function Reward_120()
	local nRandom =random(1,10)
	
	if (GetTask(T_REWARD120) > 0) then
		Talk(1,"",ERROR_LIST[1]);
		return 0
	end;
	
	if (GetLevel() < 120) then
		Talk(1,"",ERROR_LIST[5]);
		return 0
	end;
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return 0
	end;
	
	for i = 1,3 do
		nIndex = AddScriptItem(4,7);-- 7 date; Item Tien Thao Lo [Dai]
	end;
	
	for i = 1,3 do
		nIndex = AddScriptItem(6,7);-- 7 date; Que Hoa Tuu
	end;
	
	AddTaskItem(16); -- lam thuy tinh
	
	AddTaskItem(17); -- tu thuy tinh
	
	AddTaskItem(18); -- luc thuy tinh
	
	--for i = 1,3 do 
	--AddTaskItem(26); -- lenh bai tich tinh dong.
	--AddTaskItem(26); -- lenh bai tich tinh dong.
	--AddTaskItem(26); -- lenh bai tich tinh dong.
	--end;
	Earn(1000000);
	AddScriptItem(71); -- manh son ha xa tac x1000.
	SetTask(T_REWARD120,1);
end;

--///////////////////////////////////////////////////LEARN SKILLS
ZFACTION = {
	[1] = {
		{"ThiÕu L©m Ph¸i",1},
		{"Thiªn V­¬ng Bang",2},
	},
	[2] = {
		{"§­êng M«n",3},
		{"Ngò §éc Gi¸o",4},	
	},
	[3] = {
		{"Nga My Ph¸i",5},
		{"Thóy Yªn M«n",6},
		--{"Hoa S¬n ph¸i",11},
	},
	[4] = {
		{"C¸i Bang",7},
		{"Thiªn NhÉn Gi¸o",8},
	},
	[5] = {
		{"Vâ §ang Ph¸i",9},
		{"C«n L«n Ph¸i",10},
	},
}
function SP_Skill()
	local szHello = "Mçi nh©n vËt sÏ ®­îc nhËn mét cuèn bÝ kÝp cña m«n ph¸i m×nh h·y gia nhËp råi ®Õn ®©y lÜnh th­ëng";
	local TAB_SAY = {
		"Häc kü n¨ng 90./SP_ChooseSkill",
		"Ta chØ ghÐ th¨m ng­¬i/no"		
	}
	Say(szHello,getn(TAB_SAY),TAB_SAY);
end;

function SP_ChooseSkill()
	local szHello = "Mçi nh©n vËt sÏ ®­îc nhËn mét cuèn bÝ kÝp cña m«n ph¸i m×nh h·y gia nhËp råi ®Õn ®©y lÜnh th­ëng";
	local nFactionID = GetFactionNo() + 1;
	local nLevel = 80;
	if(nFactionID == 0) then
		Talk(1,"","Ch­a gia nhËp m«n ph¸i kh«ng thÓ häc kü n¨ng.")
		return
	end
	
	local tbMessage = {
			"<color=green>Chóc mõng ®¹i hiÖp ®· häc ®­îc kü n¨ng "..nLevel.." "..Faction.TFACTION_INFO[nFactionID].sFactionName..".",
			"Ng­¬i ®· lÜnh héi kü n¨ng nµy råi!",
			"H·y luyÖn lªn ®¼ng cÊp "..nLevel.." ®·.",
			"Ng­¬i ®· nhËn råi cßn muèn nhËn n÷a sao, thËt lµ tham lam kh«ng xøng danh anh hïng!",
		}
	
	if(GetLevel() < nLevel) then
		Msg2Player(tbMessage[3])
		return
	end
	
	if (GetTask(T_REWARDBIKIP8X) > 0) then
		Msg2Player(tbMessage[4])
		return
	end
	
	local TAB_SAY ={};
	local nCount = 0;
	for i = 1,getn(Faction.TFACTION_INFO[nFactionID].tbSkill90) do
		if (HaveMagic(Faction.TFACTION_INFO[nFactionID].tbSkill90[i][2]) < 1) then
		tinsert(TAB_SAY,Faction.TFACTION_INFO[nFactionID].tbSkill90[i][1].."/AddSkillsPlayer#"..Faction.TFACTION_INFO[nFactionID].tbSkill90[i][2]);	
		nCount = nCount +1;
		end;
	end;
	tinsert(TAB_SAY,"Ta chØ ghÐ th¨m ng­¬i/no")
	if (nCount > 0) then
		Say(szHello,getn(TAB_SAY),TAB_SAY);
	else
	Talk(1,"","Ng­¬i ®· lÜnh héi hÕt kü n¨ng m«n ph¸i ko cÇn ®Õn ®©y nhËn thªm!");
	end;
	
end;

function AddSkillsPlayer(nSel,nIDMagic)
	local nIdxMagic = tonumber(nIDMagic);
	AddMagic(nIdxMagic,5);
	SetTask(T_REWARDBIKIP8X,1);
end;

function no()
end;
