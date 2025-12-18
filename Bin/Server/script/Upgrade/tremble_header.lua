--Author: Kinnox
--Date: 17/06/2023
--Tools Name: Tremble Item
Include("\\script\\lib\\TaskLib.lua");
Tremble = {}
Tremble.Gem =
{
	{ 6, 17, 1, "Tö thñy tinh" },
	{ 6, 18, 1, "Lôc thñy tinh" },
	{ 6, 16, 1, "Lam thòy tinh" },
}
function TrembleTalk()
	local tbSay = {
		"Kh¶m n¹m trang bÞ xanh/ExcuteMain",
		"Ta chØ ghÐ ngang qua/no",
	};
	Say(10227,getn(tbSay),tbSay);
end;

function CoverMelee()
	local tbSay = {
		"Tr­êng kiÕm/CoverItem#1",
		"Yªu ®ao/CoverItem#2",
		"C«n bæng/CoverItem#3",
		"Th­¬ng kÝch/CoverItem#4",
		"Song Chïy/CoverItem#5",
		"Song §ao/CoverItem#6",
		"Phi tiªu/CoverItem#7",
		"Phi §ao/CoverItem#8",
		"Tô TiÔn/CoverItem#9",
		"Ta chØ ghÐ ngang qua/no",
	};
	Say(10227,getn(tbSay),tbSay);
end;

function ExcuteMain()
	TrembleItem();
end;

function ExeTremble()
	local nPos = 15; -- mac dinh cua server
	local nIndexGem=0;
	local nIndexItem,nG, nD, nP, nL, Ser;
	local nLocation =0;
	nIndexItem = GetPOItem(nPos,0); -- Kiem tra Trang bi xanh hoac trang
	nG, nD, nP, nL, Ser, nLuc = GetItemProp(nIndexItem);
	nGemTTT = GetPOItem(nPos,1); -- Kiem tra Tu Thuy Tinh
	nGemLuTT = GetPOItem(nPos,2); -- Kiem tra Luc Thuy Tinh
	for i = 3,7 do
		nGemLaTT = GetPOItem(nPos,i); -- Kiem tra Lam Thuy Tinh
		if (nGemLaTT > 0) then
			nLocation = i;
			Msg2Player("nLocation: "..nLocation.."")
			break;
		end;
	end;
	------------------TU THUY TINH UPGRADE LEVEL ITEM
	if (nGemTTT > 0) then
		nIndexGem = nGemTTT;
		if (ConditionInput(nIndexGem,1) > 0) then
			if (ConditionInput(nIndexItem,0) < 0) then
				return
			end;
			if (nL >= 10) then
				Talk(1,"","Trang bÞ ®· ®¹t ®¼ng cÊp tèi ®a, kh«ng thÓ n©ng cÊp thªm n÷a!");
				return
			end;
			-- Thuc thi thanh cong--
				ExecuterMain(1, nIndexGem, nIndexItem, nG, nD, nP, nL, Ser);
			--
			return
		end;
	end;
	------------------LUC THUY TINH CHANGE OPTIONS ITEM
	if (nGemLuTT > 0) then
		nIndexGem = nGemLuTT;
		if (ConditionInput(nIndexGem,2) > 0) then 
			if (ConditionInput(nIndexItem,0) < 0) then
				return
			end;
			-- Thuc thi thanh cong--
				 ExecuterMain(3, nIndexGem, nIndexItem, nG, nD, nP, nL, Ser);
			--
			return
		end;
	end;
	------------------LAM THUY TINH CHANGE SERIES ITEM	
	if (nGemLaTT > 0) then
		nIndexGem = nGemLaTT;
		if (ConditionInput(nIndexGem,nLocation) > 0) then 
			if (ConditionInput(nIndexItem,0) < 0) then
				return
			end;
			nSeries = 0;
			if (nLocation == 3) then
				nSeries = 0;
			elseif (nLocation == 4) then	
				nSeries = 1;
			elseif (nLocation == 5) then	
				nSeries = 2;	
			elseif (nLocation == 6) then	
				nSeries = 3;		
			elseif (nLocation == 7) then	
				nSeries = 4;		
			end;
			if (Ser == nSeries) then
				Talk(1,"","Trang bÞ cïng hÖ víi hÖ muèn chuyÓn ®æi, kh«ng thÓ tiÕn hµnh chuyÓn!");
				return
			end;
			-- Thuc thi thanh cong--
				ExecuterMain(2, nIndexGem, nIndexItem, nG, nD, nP, nL, nSeries);
			--
			return
		end;
	end;

end;

function ConditionInput(nIndex,nLocation)
	local nItemIdx, nG, nD, nP, nL, Ser;
	Msg2Player("DEBUG: "..nIndex.."");
	nItemIdx = tonumber(nIndex);
	if (nItemIdx <= 0)  then
		Talk(1,"","Ph¸t sinh lçi vui lßng liªn hÖ BQT");
		return 0;
	end;
	nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx);
	if (nLocation == 0) then
		if (GetGoldItem(nItemIdx) > 0) then
			Talk(1,"","Chñ cã thÓ ®Æt trang bÞ xanh hoÆc tr¾ng vµo « nµy!");
			return 0;
		end;
	elseif (nLocation == 1) then
		if (nG ~= 6 or nD ~= Tremble.Gem[1][2] or GetItemStackCount(nItemIdx) > 1) then
			Talk(1,"","VËt phÈm kh«ng ®óng yªu cÇu hoÆc sè l­îng lín h¬n 1!");
			return 0;
		end;
	elseif (nLocation == 2) then
		if (nG ~= 6 or nD ~= Tremble.Gem[2][2] or GetItemStackCount(nItemIdx) > 1) then
			Talk(1,"","VËt phÈm kh«ng ®óng yªu cÇu hoÆc sè l­îng lín h¬n 1!");
			return 0;
		end;
	elseif (nLocation >= 3 and nLocation <= 7) then
		if (nG ~= 6 or nD ~= Tremble.Gem[3][2] or GetItemStackCount(nItemIdx) > 1) then
			Talk(1,"","VËt phÈm kh«ng ®óng yªu cÇu hoÆc sè l­îng lín h¬n 1!");
			return 0;
		end;
	else
		-- Phu may man chua lam;
	end;
	return 1;
end;

function ExecuterMain(nType,nIndexGem,nIndexItem,nGenre,nDetail,nParti,nLevel,nSeries)
	local nPos = 15; -- mac dinh tu source;
	local nJbcoin = 0;	-- tien dong;
	local nCash   = 0;	-- tien van;
	if (nType == 1) then
		-- up level
		nJbcoin = 3;
		nCash = 100000;
		if (GetItemCountInBag(6,19,0,0) < nJbcoin) then
			Talk(1,"","Kh«ng t×m thÊy x"..nJbcoin.." tiÒn ®ång trong r­¬ng hµnh trang!");		
			return
		end;

		if (GetCash() < nCash) then
			Talk(1,"","C¸c h¹ kh«ng cã ®ñ "..nCash.." l­îng b¹c trong ng­êi!");
			return
		end;
		nRecord = nParti *10 + nLevel;
		SetItemRecord(nIndexItem,nRecord);
		 nLevel = nLevel + 1;
		SetItemLevel(nIndexItem,nLevel);	
		AddItemAgain(nIndexItem);			
		if (DelMyItem(nIndexItem) ~= 0) then
			AddMyItem(nIndexItem,nPos,0,0);	
		end;
	elseif (nType == 2) then
		-- change series
		nJbcoin = 3;
		nCash = 100000;
		if (GetItemCountInBag(6,19,0,0) < nJbcoin) then
			Talk(1,"","Kh«ng t×m thÊy x"..nJbcoin.." tiÒn ®ång trong r­¬ng hµnh trang!");		
			return
		end;

		if (GetCash() < nCash) then
			Talk(1,"","C¸c h¹ kh«ng cã ®ñ "..nCash.." l­îng b¹c trong ng­êi!");
			return
		end;
		SetItemSeries(nIndexItem,nSeries);	
		AddItemAgain(nIndexItem);
		if (DelMyItem(nIndexItem) ~= 0) then
			AddMyItem(nIndexItem,nPos,0,0);	
		end;
	elseif (nType == 3) then
		-- new option
		nCash = 10000;
		if (GetCash() < nCash) then
			Talk(1,"","C¸c h¹ kh«ng cã ®ñ "..nCash.." l­îng b¹c trong ng­êi!");
			return
		end;
		if (DelItemByIndex(nIndexItem) ~= 0) then
			AddItemEx(nGenre,nDetail,nParti,nLevel,nSeries, GetLucky(0)+1000,10,10,10,10,10,10,1,0,nPos);
		end;
		
	end;
	Msg2Player("Chóc mõng ®¹i hiÖp ®· kh¶m n¹m <color=green> thµnh c«ng <color>");
	DelItemByIndex(nIndexGem);
	if (nType ~= 3) then
	DelTaskItem(19,nJbcoin);
	end;
	Pay(nCash);
end;


function CoverItem(_,nKind)
	SetTaskTemp(TMP_INDEX_PARACOVER,0);
	SetTaskTemp(TMP_INDEX_DETAILCOVER,0);
	local nKind = tonumber(nKind);
	local nPaticular = -1;
	local nDetail 	 = -1;
	if (nKind == 1) then
		nDetail		= 0;
		nPaticular 	= 0;
	elseif(nKind == 2) then
		nDetail		= 0;
		nPaticular 	= 1;
	elseif(nKind == 3) then
		nDetail		= 0;
		nPaticular 	= 2;
	elseif(nKind == 4) then
		nDetail		= 0;
		nPaticular 	= 3;
	elseif(nKind == 5) then
		nDetail		= 0;
		nPaticular 	= 4;
	elseif(nKind == 6) then
		nDetail		= 0;
		nPaticular 	= 5;
	elseif(nKind == 7) then
		nDetail		= 1;
		nPaticular 	= 0;
	elseif(nKind == 8) then
		nDetail		= 1;
		nPaticular 	= 1;
	elseif(nKind == 9) then
		nDetail		= 1;
		nPaticular 	= 2;
	end;

	if (nPaticular < 0 or nDetail < 0) then
		return
	end;
	SetTaskTemp(TMP_INDEX_PARACOVER,nPaticular);
	SetTaskTemp(TMP_INDEX_DETAILCOVER,nDetail);
	OpenGiveBox("§Æt trang bÞ vµo trong","Nguyªn liÖu gåm: 30 v¹n l­îng, x10 tiÒn ®ång\n**Løu ý: ChØ ®Æt x1 trang bÞ xanh vµo trong.\n**Tû lÖ: 75% thµnh c«ng","CalExcuterCover");
end;

function CalExcuterCover()
	local nCash = 300000;
	local nTienDong = 10;
	local nDetail = GetTaskTemp(TMP_INDEX_DETAILCOVER);
	local nPaticular = GetTaskTemp(TMP_INDEX_PARACOVER);

	if (ConditionCover(nPaticular,nDetail,nCash,nTienDong) ~= 1) then
		return
	end;
	if (LuckyPercentCover(Cash,nTienDong) ~= 1) then
		return
	end;

	ExcuterCoverAll(nCash,nTienDong,nDetail,nPaticular) ------DANG LAM O DAY 
end;

function ExcuterCoverAll(nCash,nTienDong,nDetail,nPaticular)
	local nItemIdx, nG, nD, nP, nL, Ser
	local i = 0;
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	if ROOMG ~= 8 then
	return
	end	

	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if (nD ~= nDetail) then
					SetItemDetail(nItemIdx,nDetail);
				end;

				if (nP ~= nPaticular) then
					SetItemParticular(nItemIdx,nPaticular);
					if (nPaticular > nP) then
						--Msg2Player("Recorde 1: P-"..nPaticular.." D- "..nDetail.." " ..((nL-1 + nP*10) +(nPaticular - nP)*10).."" );
						SetItemRecord(nItemIdx,(nL-1 + nP*10) +(nPaticular - nP)*10);
						AddItemAgain(nItemIdx);
					elseif (nP > nPaticular) then
						--Msg2Player("Recorde 2:  P-"..nPaticular.." D- "..nDetail.." "..((nL-1 + nP*10) +(nP - nPaticular)*10).."" );
						SetItemRecord(nItemIdx,(nL-1 + nP*10) - (nP - nPaticular)*10);	
						AddItemAgain(nItemIdx);					
					end;
				end;

				if (DelMyItem(nItemIdx) ~= 0) then
					AddMyItem(nItemIdx,11,0,0);	
				end;
				DelTaskItem(19,nTienDong);
				Pay(nCash);
				break;					
			end
		end
	end

end;

function ConditionCover(nPaticular,nDetail,nCash,nTienDong)
	local nItemIdx, nG, nD, nP, nL, Ser
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	if ROOMG ~= 8 then
	return
	end	
	local nCount = 0;
	for i=0,5 do
		for j=0,3 do	
			nItemIdx = GetROItem(ROOMG,i,j);	
			nG, nD, nP, nL, Ser, nLuc = GetItemProp(nItemIdx)
			if (nG ~= -1 or nD ~= -1 or nP ~= -1 or nL ~= -1 or Ser ~= -1) then
				if(GetGoldItem(nItemIdx) > 0) then
					Talk(1,"","Trang bÞ hoµng kim kh«ng thÓ n©ng cÊp!");
					return 0
				end;
				local nAttrib,_,_,_ = GetItemMagicAttrib(nItemIdx,1)
				if(nAttrib == 0) then
					Talk(1,"","Trang bÞ tr¾ng hoÆc nguyªn liÖu kh«ng ®óng!");
					return 0
				end;

				if(nD == nDetail and nP == nPaticular ) then
					Talk(1,"","Kh«ng thÓ ho¸n binh cïng lo¹i trang bÞ!");
					return 0
				end;

				if(nD > 1) then
					Talk(1,"","Trang bÞ nµy kh«ng ph¶i lµ vò khÝ!");
					return 0
				end;

				nCount = nCount + 1;				
			end
		end
	end

	if (nCount > 1) then
		Talk(1,"","ChØ ®Æt mét trang bÞ xanh cÇn n©ng cÊp!");
		return 0
	end;

	if (GetItemCountInBag(6,19,0,0) < nTienDong) then
		Talk(1,"","Kh«ng t×m thÊy x10 tiÒn ®ång trong r­¬ng hµnh trang!");		
		return 0
	end;

	if (GetCash() < nCash) then
		Talk(1,"","C¸c h¹ kh«ng cã ®ñ 10 v¹n l­îng b¹c trong ng­êi!");
		return 0
	end;

	return 1
end;

function LuckyPercentCover(Cash,nTienDong)
	local nRandom = random(1,2)
	if (nRandom == 2) then
		DelTaskItem(19,nTienDong);
		Pay(Cash,1);
		Msg2Player("RÊt tiÕt ®¹i hiÖp ®· <color=blue> thÊt b¹i <color>, xin thö l¹i!");
		return 0
	end;
	Msg2Player("Chóc mõng ®¹i hiÖp ®· <color=green> thµnh c«ng <color>");
	return 1
end;

function no()

end;