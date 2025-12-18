-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tho ren;
Include("\\script\\system_config.lua")
Include("\\script\\upgrade\\tremble_header.lua");
BlackSmith = {};

function main(nNpcIdx)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then	
		dofile("script/global/luanpcfunctions/blacksmith.lua");
		Msg2Player("Reload this script");
	end;
BlackSmith:BlackSmith(10227);
end;

function BlackSmith:BlackSmith(nMsgId)
	local tbSay = {
		"Giao dÞch./OnBuy",
		"S÷a trang bÞ h­ tæn/RestoreItem",
		"GhÐp trang bÞ hång ¶nh/ExcuterHA",
		"Kh¶m n¹m trang bÞ xanh/TrembleTalk",
		"Ho¸n binh vò khÝ/CoverMelee",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say(nMsgId,getn(tbSay),tbSay);
end;

function OnBuy()
	local w,x,y = GetWorldPos();
	local nIndex = 0;
	if (w == 11) then
		nIndex = 12;
	elseif (w == 176) then
		nIndex = 13;
	elseif (w == 162) then
		nIndex = 14;
	elseif (w == 80) then
		nIndex = 15;
	elseif (w == 37) then
		nIndex = 16;
	elseif (w == 1) then
		nIndex = 17;
	elseif (w == 78) then
		nIndex = 18;
	else
		nIndex = 11;
	end
	Sale(nIndex);
end;

function RestoreItem()
	OpenGiveBox("Giao diÖn söa ®å","Söa ch÷a trang bÞ h­ tæn       Trang bÞ xanh: 3 tiÒn ®ång + 50 v¹n l­îng.                    Trang bÞ hoµng kim: 50 tiÒn ®ång + 150 v¹n l­îng ","OnRestoreItem")
end;

function OnRestoreItem()
	local nItemIdx, nG, nD, nP, nL, Ser
	local nCount = 0;
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local nCashBlue = 3;
	local nCashGold = 50;
	local nPriceBlue = 500000;
	local nPriceGold = 1500000;
	local i = 0;

	if ROOMG ~= 8 then
		return
	end
	
	if (FindEmptyPlace(6,6) == 0) then 
		Talk(1,"","H·y s¾p xÕp l¹i r­¬ng hµnh trang cña c¸c h¹, ®¶m b¶o ®ñ <color=yellow> 6x6 <color> chç trèng! ");
		return
	end;
	
	--Anti one item;
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then			
				nCount = nCount +1;
			end
		end
	end
	
	if not nCount or nCount == 0 or nCount > 1 then
		Talk(1,"","Mçi lÇn bá mét vËt phÈm, hiÓu vÊn ®Ò ch­a ®¹i hiÖp!");
		return
	end;
		
	--ExcuteTask;
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser,nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				-- Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." - "..nLuc.." ");
				if (nG == 4) then
					if (nLuc == 7531) then
						if ((GetTaskItemCount(19) >= nCashGold)) and (GetCash() >= nPriceGold) then
							DelTaskItem(19,nCashGold);
							Pay(nPriceGold);
						else
							Talk(1,"","Trang bÞ gi¸ trÞ nh­ vËy cÇn nhiÒu c«ng ®o¹n söa ch÷a, Ng­êi kh«ng cã ®ñ <color=red>"..nCashGold.."<color> tiÒn ®ång vµ <color=red>"..(nPriceGold/10000).."<color> v¹n l­îng tiÒn c«ng sao gi¸m ®Õn t×m ta?");
						return
						end;
					else
						if ((GetTaskItemCount(19) >= nCashBlue)) and (GetCash() >= nPriceBlue) then
							DelTaskItem(19,nCashBlue);
							Pay(nPriceBlue);
						else
							Talk(1,"","Trang bÞ gi¸ trÞ nh­ vËy cÇn nhiÒu c«ng ®o¹n söa ch÷a, Ng­êi kh«ng cã ®ñ <color=red>"..nCashBlue.."<color> tiÒn ®ång vµ <color=red>"..(nPriceBlue/10000).."<color> v¹n l­îng tiÒn c«ng sao gi¸m ®Õn t×m ta?");
						return
						end;
					end; 
					
					RestoreBrokenEquip(nItemIdx);
					Talk(1,"","S÷a trang bÞ h­ tæn thµnh c«ng, cã viÖc g× l¹i t×m ta nhÐ!");
				else
					Talk(1,"","Kh«ng cã trang bÞ nµo h­ tæn, ng­¬i ®Õn ®Ó trªu ®ïa ta sao?");
				end;
			end
		end
	end
	
	
end

function ExcuterHA()
	SetTask(899,0);
	local tbSay = {
		"Hång ¶nh ThÈm Viªn UyÓn/ChooseItem#204",
		"Hång ¶nh KiÕm Bµi/ChooseItem#205",
		"Hång ¶nh Môc Tóc/ChooseItem#206",
		"Hång ¶nh Tô Chiªu/ChooseItem#207",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say(10227,getn(tbSay),tbSay);
end;

function ChooseItem(nSel, nIndex)
	local nIDItem = tonumber(nIndex);
	SetTask(899,nIDItem);
	ExcuterGVB();
end;

function ExcuterGVB()
	OpenGiveBox("§Æt vËt phÈm vµo trong ","Ta cÇn 6 m·nh ®Ó cã thÓ Ðp ra trang bÞ cho ng­¬i,300 v¹n l­îng vµ ®Æt thñy tinh ®Ó t¨ng tØ lÖ thµnh c«ng ","Ephonganh")
end;

function Ephonganh()
	local nItemIdx, nG, nD, nP, nL, Ser;
	local nCountItem = 0;
	local nCash = 3000000;
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local nIndexItem={};
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nG == 6 and nD == 29 and nP == 1) then
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
				if (nG == 6 and nD == 30 and nP == 1) then
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
				if (nG == 6 and nD == 31 and nP == 1) then
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
				if (nG == 6 and nD == 32 and nP == 1) then
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
				if (nG == 6 and nD == 33 and nP == 1) then
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
				if (nG == 6 and nD == 34 and nP == 1) then
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
				if (nG == 6 and nD >= 16 and nD <= 18 and nP == 1) then
					bCountGem = bCountGem +1;
					nIndexGem[bCountGem] = nItemIdx;
				end;
			end
		end
	end

	local bCount = 0;
	for i = 1,6 do
		if (nIndexItem[i] == nil) then
			Talk(1,"","C¸c h¹ thiÕu m·nh ghÐp M¶nh trang bÞ hång ¶nh sè <color=yellow> "..i.."<color>. Ta cÇn ®ñ 6 m¶nh.");
			return
		else
			bCount = bCount + 1;
		end;
	end;
	if (GetCash() < nCash) then
		Talk(1,"","C«ng rÌn t¹o ra tr¨ng søc rÊt lín, trªn giang hå nµy ta ch­a mét lÇn nãi th¸ch. Mang 300 v¹n ®Õn ®©y.");
		return
	end
	if (bCountGem < 2 ) then
		Talk(1,"","Ta kh«ng thÓ m¹o hiÓm khi kh«ng cã thñy tinh ®Ýnh kÌm, rÊt nguy hiÓm. Ýt nhÊt ph¶i cã 2 viªn thñy tinh(lo¹i nµo còng ®­îc).");
		return
	end
	----------------Tinh ty le
	--Ty le = 5.6 * gem;
	local nPercen = (5.6/3)*bCountGem;
	local nLucky = GetLucky(0);
	---------------
	if (random(1,100) <= nPercen+nLucky) then
		if (bCount >= 6) then
			AddGoldItem(GetTask(899));
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
			Msg2Player("<color=yellow> C¸c h¹ ghÐp thµnh c«ng trang bÞ Hång ¶nh<color>");
			EndGiveBox();
		end;
	else
		for i = 1,bCountGem do
			DelItemByIndex(nIndexGem[i]);
			nIndexGem[i] = nil;
		end;
		Msg2Player("<color=cyan>GhÐp trang bÞ thÊt b¹i, h«m nay ng­êi kh«ng ®­îc may m¾n. thö t¨ng tØ lÖ xem sao?<color>");
	end;
	Pay(nCash);
end;

function Update()
OpenGiveBox("§Æt vËt phÈm vµo trong ","Test ","CoverMeele")
end;

function CoverMeele()
	-- if not nCount or nCount == 0 or nCount > 1 then 
		-- Talk(1,"","yªu cÇu bá 1 thø vµo. ")
		-- return 0
	-- end
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				Msg2Player(" "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." - "..nLuc.." ");
				local nOp1,nValueMin1,nValueMax1,zValueMax1 = GetItemMagicAttrib(nItemIdx,1); -- hien 1
				local nOp2,nValueMin2,nValueMax2,zValueMax2 = GetItemMagicAttrib(nItemIdx,2); -- an 1
				local nOp3,nValueMin3,nValueMax3,zValueMax3 = GetItemMagicAttrib(nItemIdx,3); -- hien 2
				local nOp4,nValueMin4,nValueMax4,zValueMax4 = GetItemMagicAttrib(nItemIdx,4); -- an 2
				local nOp5,nValueMin5,nValueMax5,zValueMax5 = GetItemMagicAttrib(nItemIdx,5); -- hien 3
				local nOp6,nValueMin6,nValueMax6,zValueMax6 = GetItemMagicAttrib(nItemIdx,6); -- an 3			
				
				Msg2Player( "Hien1: "..nOp1.." - "..nValueMin1.." - "..nValueMax1.." - "..nValueMax1.."");
				Msg2Player( "An1: "..nOp2.." - "..nValueMin2.." - "..nValueMax2.." - "..nValueMax2.."");
				Msg2Player( "Hien2: "..nOp3.." - "..nValueMin3.." - "..nValueMax3.." - "..nValueMax3.."");
				Msg2Player( "An2: "..nOp4.." - "..nValueMin4.." - "..nValueMax4.." - "..nValueMax4.."");
				Msg2Player( "Hien3: "..nOp5.." - "..nValueMin5.." - "..nValueMax5.." - "..nValueMax5.."");
				Msg2Player( "An3: "..nOp6.." - "..nValueMin6.." - "..nValueMax6.." - "..nValueMax6.."");
				-- nang cap level
				--SetItemRecord(nItemIdx,nL);
				--nL = nL + 1;
				--SetItemLevel(nItemIdx,nL);				
				-- doi he
				-- SetItemSeries(nItemIdx,3);
				--SetItemRecord(nItemIdx,nL);
				--SetItemDetail(nItemIdx,1)
				SetItemParticular(nItemIdx,1);
				SetItemRecord(nItemIdx,nL);
				if (DelMyItem(nItemIdx) ~= 0) then
					AddMyItem(nItemIdx,11,0,0);	
				end;	
				Msg2Player("NEW: "..nItemIdx.." "..nG.." - "..nD.." - "..nP.." - "..nL.." - "..Ser.." - "..nLuc.." ");
				-- thuoc tinh
				-- AddItemEx(nG, nD, nP, nL, Ser, nLuc,6,random(1,10),random(1,10),random(1,10),random(1,10),random(1,10),1,0,11)
			end
		end
	end
end



function OnCancel()
end;