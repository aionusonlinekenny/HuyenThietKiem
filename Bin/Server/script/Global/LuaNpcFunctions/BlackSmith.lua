-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Tho ren;
Include("\\script\\system_config.lua")
Include("\\script\\upgrade\\tremble_header.lua");
Include("\\script\\Npc\\upgrade_master.lua");  -- Upgrade attributes with minerals
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
		"Giao d�ch./OnBuy",
		"S�a trang b� h� t�n/RestoreItem",
		"Gh�p trang b� h�ng �nh/ExcuterHA",
		"Kh�m n�m trang b� xanh/TrembleTalk",
		"N�ng c�p thu�c t�nh trang b� xanh/UpgradeAttribTalk",
		"Ho�n binh v� kh�/CoverMelee",
		"Ta ch� gh� ngang qua./OnCancel",
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
	OpenGiveBox("Giao di�n s�a ��","S�a ch�a trang b� h� t�n       Trang b� xanh: 3 ti�n ��ng + 50 v�n l��ng.                    Trang b� ho�ng kim: 50 ti�n ��ng + 150 v�n l��ng ","OnRestoreItem")
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
		Talk(1,"","H�y s�p x�p l�i r��ng h�nh trang c�a c�c h�, ��m b�o �� <color=yellow> 6x6 <color> ch� tr�ng! ");
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
		Talk(1,"","M�i l�n b� m�t v�t ph�m, hi�u v�n �� ch�a ��i hi�p!");
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
							Talk(1,"","Trang b� gi� tr� nh� v�y c�n nhi�u c�ng �o�n s�a ch�a, Ng��i kh�ng c� �� <color=red>"..nCashGold.."<color> ti�n ��ng v� <color=red>"..(nPriceGold/10000).."<color> v�n l��ng ti�n c�ng sao gi�m ��n t�m ta?");
						return
						end;
					else
						if ((GetTaskItemCount(19) >= nCashBlue)) and (GetCash() >= nPriceBlue) then
							DelTaskItem(19,nCashBlue);
							Pay(nPriceBlue);
						else
							Talk(1,"","Trang b� gi� tr� nh� v�y c�n nhi�u c�ng �o�n s�a ch�a, Ng��i kh�ng c� �� <color=red>"..nCashBlue.."<color> ti�n ��ng v� <color=red>"..(nPriceBlue/10000).."<color> v�n l��ng ti�n c�ng sao gi�m ��n t�m ta?");
						return
						end;
					end; 
					
					RestoreBrokenEquip(nItemIdx);
					Talk(1,"","S�a trang b� h� t�n th�nh c�ng, c� vi�c g� l�i t�m ta nh�!");
				else
					Talk(1,"","Kh�ng c� trang b� n�o h� t�n, ng��i ��n �� tr�u ��a ta sao?");
				end;
			end
		end
	end
	
	
end

function ExcuterHA()
	SetTask(899,0);
	local tbSay = {
		"H�ng �nh Th�m Vi�n Uy�n/ChooseItem#204",
		"H�ng �nh Ki�m B�i/ChooseItem#205",
		"H�ng �nh M�c T�c/ChooseItem#206",
		"H�ng �nh T� Chi�u/ChooseItem#207",
		"Ta ch� gh� ngang qua./OnCancel",
	}
	Say(10227,getn(tbSay),tbSay);
end;

function ChooseItem(nSel, nIndex)
	local nIDItem = tonumber(nIndex);
	SetTask(899,nIDItem);
	ExcuterGVB();
end;

function ExcuterGVB()
	OpenGiveBox("��t v�t ph�m v�o trong ","Ta c�n 6 m�nh �� c� th� �p ra trang b� cho ng��i,300 v�n l��ng v� ��t th�y tinh �� t�ng t� l� th�nh c�ng ","Ephonganh")
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
			Talk(1,"","C�c h� thi�u m�nh gh�p M�nh trang b� h�ng �nh s� <color=yellow> "..i.."<color>. Ta c�n �� 6 m�nh.");
			return
		else
			bCount = bCount + 1;
		end;
	end;
	if (GetCash() < nCash) then
		Talk(1,"","C�ng r�n t�o ra tr�ng s�c r�t l�n, tr�n giang h� n�y ta ch�a m�t l�n n�i th�ch. Mang 300 v�n ��n ��y.");
		return
	end
	if (bCountGem < 2 ) then
		Talk(1,"","Ta kh�ng th� m�o hi�m khi kh�ng c� th�y tinh ��nh k�m, r�t nguy hi�m. �t nh�t ph�i c� 2 vi�n th�y tinh(lo�i n�o c�ng ���c).");
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
			Msg2Player("<color=yellow> C�c h� gh�p th�nh c�ng trang b� H�ng �nh<color>");
			EndGiveBox();
		end;
	else
		for i = 1,bCountGem do
			DelItemByIndex(nIndexGem[i]);
			nIndexGem[i] = nil;
		end;
		Msg2Player("<color=cyan>Gh�p trang b� th�t b�i, h�m nay ng��i kh�ng ���c may m�n. th� t�ng t� l� xem sao?<color>");
	end;
	Pay(nCash);
end;

function Update()
OpenGiveBox("��t v�t ph�m v�o trong ","Test ","CoverMeele")
end;

function CoverMeele()
	-- if not nCount or nCount == 0 or nCount > 1 then 
		-- Talk(1,"","y�u c�u b� 1 th� v�o. ")
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