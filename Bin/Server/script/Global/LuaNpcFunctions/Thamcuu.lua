-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tham Cuu;
Include("\\script\\system_config.lua")
function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/thamcuu.lua");
		Msg2Player("Reload this script");
	end;
ThamCuu(nNpcIdx);
end;

function ThamCuu(nNpcIdx)
	local szHello = "<color=Orange>ThÈm Cöu<color>: C¸c l·o thñ khè trªn giang hå ®Òu ®Õn ®©y ®Ó nhê ta lµm nª nh÷ng chiÕc khãa b¶o mÊt ®¸ng gi¸!";
	local tbSay = {
		"Më réng r­¬ng/ExPandBox",
		"ThuÇn hãa chiÕn m·/HorseSay",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say(szHello,getn(tbSay),tbSay);
end;

function ExPandBox()
	local szHello ="<color=red>HÖ Thèng:<color> Xin chµo!"
	Tab_inSert = {
	"Më r­¬ng 1/ID_ExPandBox_1",
	"Më r­¬ng 2/ID_ExPandBox_2",
	"Më r­¬ng 3/ID_ExPandBox_3",
	"Më r­¬ng 4/ID_ExPandBox_4",
	"Më r­¬ng 5/ID_ExPandBox_5",
	"Më tói hµnh trang/ID_EquipItemBox",
	"KiÓm tra r­¬ng ®· më/ID_GExPandBox",
	"Ta kh«ng cÇn ng­¬i d¹y b¶o/OnCancel"
	}
	if (GetStoreBox() == 0) then
		Say(szHello,3,Tab_inSert[1],Tab_inSert[6],Tab_inSert[8]);
	elseif (GetStoreBox() == 1) then
		Say(szHello,3,Tab_inSert[2],Tab_inSert[6],Tab_inSert[8]);
	elseif (GetStoreBox() == 2) then
		Say(szHello,3,Tab_inSert[3],Tab_inSert[6],Tab_inSert[8]);
	-- elseif (GetStoreBox() == 3) then
		-- Say(szHello,2,Tab_inSert[4],Tab_inSert[8]);
	-- elseif (GetStoreBox() == 4) then
		-- Say(szHello,2,Tab_inSert[5],Tab_inSert[8]);
	else
		Say(szHello,2,Tab_inSert[6],Tab_inSert[8]);
	end;
end

----------------------------
--
----------------------------
function ID_ExPandBox_1()
	local nSel = 1 ;
	
	if (GetItemCountInBag(6,22,0,0)) < 1 then
		Talk(1,"","Ng­¬i kh«ng cã Ch×a khãa r­¬ng 1");
	return
	end;
	DelTaskItem(22,1);
	SetStoreBox(nSel);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end

function ID_ExPandBox_2()
	local nSel = 2 ;
	if (GetItemCountInBag(6,23,0,0)) < 1 then
		Talk(1,"","Ng­¬i kh«ng cã Ch×a khãa r­¬ng 2");
	return
	end;
	DelTaskItem(23,1);
	SetStoreBox(nSel);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end

function ID_ExPandBox_3()
	local nSel = 3 ;
	if (GetItemCountInBag(6,24,0,0)) < 1 then
		Talk(1,"","Ng­¬i kh«ng cã Ch×a khãa r­¬ng 3");
	return
	end;
	DelTaskItem(24,1);
	SetStoreBox(nSel);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end

function ID_ExPandBox_4()
	-- local nSel = 4 ;
	-- SetStoreBox(nSel);
	-- Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end

function ID_ExPandBox_5()
	-- local nSel = 5 ;
	-- SetStoreBox(nSel);
	-- Msg2Player("C¸c h¹ ®· më thµnh c«ng r­¬ng "..nSel.."");
end


function ID_EquipItemBox()
	if (GetItemCountInBag(6,25,0,0)) < 1 then
		Talk(1,"","Ng­¬i kh«ng cã Ch×a khãa tói hµnh trang");
	return
	end;
	SetExPandBox(1);
	DelTaskItem(25,1);
	Msg2Player("C¸c h¹ ®· më thµnh c«ng tói hµnh trang!");
end;

function ID_GExPandBox()
	Msg2Player("C¸c h¹ ®· më ®Õn r­¬ng thø "..GetStoreBox().."");
end;

function HorseSay()
	local szHello = "<color=Orange>ThÈm Cöu<color>: C¸c l·o thñ khè trªn giang hå ®Òu ®Õn ®©y ®Ó nhê ta lµm nª nh÷ng chiÕc khãa b¶o mÊt ®¸ng gi¸!";
	local tbSay = {
		"ThuÇn hãa ngùa phi v©n/HorseBox#8",
		"ThuÇn hãa ngùa b«n tiªu/HorseBox#6",
		"ThuÇn hãa ngùa phiªn vò/HorseBox#7",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say(szHello,getn(tbSay),tbSay);
end;

function HorseBox(nSel,nIndex)
	SetTask(899,0);
	local nIndex = tonumber(nIndex);
	SetTask(899,nIndex);
	OpenGiveBox("§Æt vËt phÈm vµo trong ","ThÇn m· th­êng rÊt khã thuÇn phôc, ta cÇn 6 B¾c §Èu ThuÇn M· ThuËt míi cã thÓ gióp ng­¬i. §Æt phóc duyªn c¸c lo¹i ®Ó t¨ng duyªn phËn víi thÇn m·. ViÖc nµy ta cÇn 1000 v¹n l­îng míi gióp ®­îc.","HorseExecute")
end;

function HorseExecute()
	local nItemIdx, nG, nD1,nD2,nD3,nD4,nD5,nD6, nP, nL, Ser;
	local nCountItem = 0;
	local nCash = 10000000;
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local nIndexItem={};
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	
	if (GetTask(899) == 8) then
	nD1 = 35;
	nD2 = 36;
	nD3 = 37;
	nD4 = 38;
	nD5 = 39;
	nD6 = 40;
	elseif(GetTask(899) == 6) then
	nD1 = 41;
	nD2 = 42;
	nD3 = 43;
	nD4 = 44;
	nD5 = 45;
	nD6 = 46;	
	else
	nD1 = 47;
	nD2 = 48;
	nD3 = 49;
	nD4 = 50;
	nD5 = 51;
	nD6 = 52;	
	end;
	
	
	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD1 and nP == 1) then
					nIndexItem[1] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD2 and nP == 1) then
					nIndexItem[2] = nItemIdx;
					break;
				end;
			end
		end
	end

	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD3 and nP == 1) then
					nIndexItem[3] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD4 and nP == 1) then
					nIndexItem[4] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD5 and nP == 1) then
					nIndexItem[5] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == nD6 and nP == 1) then
					nIndexItem[6] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	local bCountGem =0;
	local nIndexGem = {};
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 7 and nD >= 11 and nD <= 13 and nP == 1) then
					bCountGem = bCountGem +1;
					nIndexGem[bCountGem] = nItemIdx;
				end;
			end
		end
	end

	local bCount = 0;
	for i = 1,6 do
		if (nIndexItem[i] == nil) then
			Talk(1,"","C¸c h¹ thiÕu B¾c §Èu ThuÇn M· ThuËt sè <color=yellow> "..i.."<color>. Ta cÇn ®ñ 6 B¾c §Èu ThuÇn M· ThuËt.");
			return
		else
			bCount = bCount + 1;
		end;
	end;
	if (GetCash() < nCash) then
		Talk(1,"","C«ng rÌn t¹o ra tr¨ng søc rÊt lín, trªn giang hå nµy ta ch­a mét lÇn nãi th¸ch. Mang 1000 v¹n ®Õn ®©y.");
		return
	end
	if (bCountGem < 2 ) then
		Talk(1,"","Ta kh«ng thÓ m¹o hiÓm khi kh«ng cã phóc duyªn lé ®Ýnh kÌm, rÊt nguy hiÓm. Ýt nhÊt ph¶i cã 2 phóc duyªn lé(phÈm chÊt cµng cao duyªn sè cµng tèt).");
		return
	end
	----------------Tinh ty le
	--Ty le = 5.6 * gem;
	local nPercen = (5.6/3)*bCountGem;
	local nLucky = GetLucky(0);
	---------------
	if (random(1,100) <= nPercen+nLucky) then
		if (bCount >= 6) then
			AddItem(0,10,GetTask(899),10,GetSeries(),0,0)
			for i = 1,6 do
				DelItemByIndex(nIndexItem[i]);
				nIndexItem[i] = nil;
			end
			for i = 1,bCountGem do
				DelItemByIndex(nIndexGem[i]);
				nIndexGem[i] = nil;
			end;
			
			SetTask(899,0);
			nIndexItem = {};
			Msg2Player("<color=yellow> C¸c h¹ thuÇn hãa thµnh c«ng mét chiÕn m· trong chiÒn thuyÕt, víi søc phi v¹n lý tr­êng xa.<color>");
			EndGiveBox();
		end;
	else
		for i = 1,bCountGem do
			DelItemByIndex(nIndexGem[i]);
			nIndexGem[i] = nil;
		end;
		Msg2Player("<color=cyan>§· lµ thÇn m· th× thuÇn phôc rÊt khã kh¨n, ®õng n¶n chÝ. mäi viÖc sÏ thµnh.<color>");
	end;
	Pay(nCash);
end;

function OnCancel()

end;