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
		"N©ng cÊp trang bÞ/upLevel",
		"§æi hÖ trang bÞ/chaSeries",
		"§«i thuéc tÝnh trang bÞ/chaOption",
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

function upLevel()
	OpenGiveBox("§Æt trang bÞ vµo trong","Nguyªn liÖu gåm: x1 Tö thñy tinh,10 v¹n l­îng, x1 tiÒn ®ång\n**Løu ý: ChØ ®Æt x1 trang bÞ xanh vµo trong.\n**Tû lÖ: 50%","ExcuterUpLevel")
end;

function ExcuterUpLevel()
	local nIDGem = 17; -- ID GEM
	local Cash = 100000; -- CASH
	local nTienDong = 1; -- Tien Dong
	local nType = 1;	-- Select
	local nPecent = 2;	-- 2 la 50%; 1 la 100%;
	if (ConditionUpgrade(nType,nIDGem,Cash,nTienDong,0) ~= 1) then
		return
	end

	if (LuckyPercent(nIDGem,Cash,nTienDong,nPecent) ~= 1) then
		return
	end

	ExcuterAll(nType,nIDGem,nTienDong,Cash,0)
end

function chaSeries()
	SetTaskTemp(TMP_INDEX_TREMBLE,100);
	local tbSay = {
		"HÖ Kim/ExCalSeries#0",
		"HÖ Méc/ExCalSeries#1",
		"HÖ Thñy/ExCalSeries#2",
		"HÖ Háa/ExCalSeries#3",
		"HÖ Thæ/ExCalSeries#4",
		"Ta chØ ghÐ ngang qua/no",
	};
	Say(10227,getn(tbSay),tbSay);

end;

function ExCalSeries(_,nSeries)
	local nSeries = tonumber(nSeries);
	SetTaskTemp(TMP_INDEX_TREMBLE,nSeries);
	OpenGiveBox("§Æt trang bÞ vµo trong","Nguyªn liÖu gåm: x1 Lam thñy tinh,10 v¹n l­îng, x1 tiÒn ®ång\n**Løu ý: ChØ ®Æt x1 trang bÞ xanh vµo trong.\n**Tû lÖ: 50%","ExcuterChaSeries");
end;

function ExcuterChaSeries()
	local nSeries = GetTaskTemp(TMP_INDEX_TREMBLE);
	local nIDGem = 16; -- ID GEM
	local Cash = 100000; -- CASH
	local nTienDong = 1; -- Tien Dong
	local nType = 2;	-- Select

	if (ConditionUpgrade(nType,nIDGem,Cash,nTienDong,nSeries) ~= 1) then
		return
	end

	if (LuckyPercent(nIDGem,Cash,nTienDong) ~= 1) then
		return
	end

	ExcuterAll(nType,nIDGem,nTienDong,Cash,nSeries)
end


function chaOption()
	OpenGiveBox("§Æt trang bÞ vµo trong","Nguyªn liÖu gåm: x1 Lôc thñy tinh,10 v¹n l­îng, x1 tiÒn ®ång\n**Løu ý: ChØ ®Æt x1 trang bÞ xanh vµo trong.\n**Tû lÖ: 100%","ExcuterChaOp");
end;

function ExcuterChaOp()
	local nIDGem = 18; -- ID GEM
	local Cash = 100000; -- CASH
	local nTienDong = 1; -- Tien Dong
	local nType = 3;	-- Select
	local nPecent = 1; -- 2 la 50%; 1 la 100%;
	if (ConditionUpgrade(nType,nIDGem,Cash,nTienDong,0) ~= 1) then
		return
	end
	
	if (LuckyPercent(nIDGem,Cash,nTienDong,	nPecent) ~= 1) then
		return
	end
	ExcuterAll(nType,nIDGem,nTienDong,Cash,0)
end

function ExcuterAll(nType,nIDGem,nTienDong,Cash,nSeries)
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
					-- up level
				if (nType == 1) then
					nRecord = nP *10 + nL;
					SetItemRecord(nItemIdx,nRecord);
					 nL = nL + 1;
					SetItemLevel(nItemIdx,nL);	
					AddItemAgain(nItemIdx);			
					if (DelMyItem(nItemIdx) ~= 0) then
						AddMyItem(nItemIdx,11,0,0);	
					end;
				elseif (nType == 2) then
					-- change series
					SetItemSeries(nItemIdx,nSeries);	
					AddItemAgain(nItemIdx);
					if (DelMyItem(nItemIdx) ~= 0) then
						AddMyItem(nItemIdx,11,0,0);	
					end;
				elseif (nType == 3) then		
					-- new option
					if (DelItemByIndex(nItemIdx) ~= 0) then
						AddItemEx(nG, nD, nP, nL, Ser, GetLucky(0),10,10,10,10,10,10,1,0,11);
					end;
				end;

				DelTaskItem(nIDGem,1);
				DelTaskItem(19,nTienDong);
				Pay(Cash);

				break;					
			end
		end
	end

end;


function ConditionUpgrade(nType,nIDGem,Cash,nTienDong,nSeries)
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

				if (nType == 2) then
					if (Ser == nSeries) then
						Talk(1,"","Trang bÞ cïng hÖ kh«ng thÓ tiÕn hµnh ®æi!");
						return 0
					end;
				end;

				if (nType == 1 and nL >= 10) then
					Talk(1,"","Trang bÞ ®· ®¹t ®¼ng cÊp tèi ®a, kh«ng thÓ n©ng cÊp!");
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

	if (GetItemCountInBag(6,nIDGem,0,0) < 1) then
		 for i = 1,getn(Tremble.Gem) do
		 	 if (nIDGem == Tremble.Gem[i][2]) then
		 	 	Talk(1,"","Kh«ng t×m thÊy "..Tremble.Gem[i][4].." trong r­¬ng hµnh trang!");
		 	 	break;
		 	 end;
		 end;		
		return 0
	end;

	if (GetItemCountInBag(6,19,0,0) < 1) then
		Talk(1,"","Kh«ng t×m thÊy x1 tiÒn ®ång trong r­¬ng hµnh trang!");		
		return 0
	end;

	if (GetCash() < Cash) then
		Talk(1,"","C¸c h¹ kh«ng cã ®ñ 10 v¹n l­îng b¹c trong ng­êi!");
		return 0
	end;

	return 1
end;

function LuckyPercent(nIDGem,Cash,nTienDong,nPecent)
	local nRandom = random(1,nPecent)
	if (nRandom == nPecent) then
		DelTaskItem(19,nTienDong);
		DelTaskItem(nIDGem,1);
		Pay(Cash,1);
		Msg2Player("RÊt tiÕt ®¹i hiÖp ®· <color=blue> thÊt b¹i <color>, xin thö l¹i!");
		return 0
	end;
	Msg2Player("Chóc mõng ®¹i hiÖp ®· <color=green> thµnh c«ng <color>");
	return 1
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
						Msg2Player("Recorde 1: P-"..nPaticular.." D- "..nDetail.." " ..((nL-1 + nP*10) +(nPaticular - nP)*10).."" );
						SetItemRecord(nItemIdx,(nL-1 + nP*10) +(nPaticular - nP)*10);
						AddItemAgain(nItemIdx);
					elseif (nP > nPaticular) then
						Msg2Player("Recorde 2:  P-"..nPaticular.." D- "..nDetail.." "..((nL-1 + nP*10) +(nP - nPaticular)*10).."" );
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