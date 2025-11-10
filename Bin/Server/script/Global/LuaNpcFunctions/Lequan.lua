-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: Le Quan;
Include("\\script\\system_config.lua")
LeQuan = {};
Include("\\script\\event\\banhchung\\head.lua");
function main(nNpcIdx)
 	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/Lequan.lua");
		Msg2Player("Reload this script");
	end;
	local tbSay = {
		--"Sù kiÖn b¸nh ch­ng may m¾n/banhchung",
		"Tiªu hñy trang bÞ kh«ng dïng/OnRemoveItem",
		"Gióp ta lÜnh héi tuyÖt kü 120 thÊt truyÒn/OnChooseSkill120",
		"Hñy bá mËt m· b¶o vÖ nh©n vËt/OnRemovePW",
		"Vßng quay may m¾n/VongQuayMayMan",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("<color=orange>LÔ Quan: <color>Ta ®­îc giao träng tr¸ch qu¶n lý tÊt c¶ c¸c ho¹t ®éng lín bÐ ë tÊt c¶ thµnh thÞ,th«ng trang. §©u ®©u còng cã ng­êi cña ta, ng­¬i cÇn g× ë ta?",getn(tbSay),tbSay);
end;
function VongQuayMayMan()
	local tbSay = {
      "Quay B»ng Xu(20Xu)/quaytienxu",
      "Quay B»ng TiÒn V¹n(150V¹n)/quaytienvan",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("<color=orange>LÔ Quan: <color>Muèn thö vËn may th× ph¶i cã vèn.",getn(tbSay),tbSay);
end
function quaytienvan()
if GetCash() < 10000*150 then Talk(1,"","CÇn 150v ®Ó ch¬i vßng quay may m¾n","") return end
SetTask(993,1)
SetTask(994,0)
OpenLucky()
end
function quaytienxu()
if GetTask(104) < 20 then Talk(1,"","CÇn 20 Xu ®Ó tham gia") return end
SetTask(993,0)
SetTask(994,1)
OpenLucky()
end
function OnRemoveItem()
OpenGiveBox("§Æt vËt phÈm vµo trong ","Tiªu hñy vËt phÈm, h·y ®Æt vËt phÈm cÇn tiªu hñy vµo trong.\n\n*L­u ý:Mäi vËt phÈm ®Òu cã thÓ tiªu hñy, h·y cÈn träng!","OnDel")
end;

function OnDel()
	local nIndexEquip = 0;
	local nIndex = 0;
	local nCountEquip = 0;
	local nNum = 0;
	local ROOMG = 8; --ID MAC DINH CUA GIVE BOX
	local i = 0;
	
	if ROOMG ~= 8 then
	return
	end
	for i=0,5 do
		for j=0,3 do
		nIndex = GetROItem(ROOMG,i,j);	
			nCountEquip = nCountEquip + 1;
			nIndexEquip = nIndex;
			DelItemByIndex(nIndexEquip)
		end
	end
	Talk(1,"","Thµnh c«ng tiªu hñy trang bÞ!")
end;

function OnRemovePW()
	Talk(1,"","Vui lßng cung cÊp <color=yellow>MËt khÈu cÊp 2, Email, S®t<color> cho BQT ®Ó hñy bá mËt m· b¶o vÖ!");
end;

function OnChooseSkill120()
	SetTask(899,0);
	local tbSay = {
		"Lu©n håi. BÕ NguyÖt PhÊt TrÇn tuyÖt kü/OnExcuteSkill120#59",
		"DÞch Cèt Kinh. HÊp Tinh YÓm tuyÖt kü/OnExcuteSkill120#60",
		"§¹i Bi Chó.  §¹i Thõa Nh­ Lai Chó tuyÖt kü/OnExcuteSkill120#61",
		"TÝnh Ng¹o Tam §«ng. Hçn Thiªn KhÝ C«ng tuyÖt kü/OnExcuteSkill120#62",
		"B¨ng TuyÕt S¬ Dung. Ngù TuyÕt Èn tuyÖt kü/OnExcuteSkill120#63",
		"L­u Quang Phi Vò. Mª ¶nh Tung tuyÖt kü/OnExcuteSkill120#64",
		"Tam Sinh H÷u H¹nh. L­ìng Nghi Ch©n KhÝ tuyÖt kü/OnExcuteSkill120#65",
		"Ng­ng ¢m Quy Nguyªn. Ma ©m phÖ Ph¸ch tuyÖt kü/OnExcuteSkill120#66",
		"Tam Phôc Chi KhÝ. §¶o H­ Thiªn tuyÖt kü/OnExcuteSkill120#67",
		"ThuÇn D­¬ng V« Cùc. Xu©n ­ BÊt NhiÔm tuyÖt kü/OnExcuteSkill120#68",
		"Ta chØ ghÐ ngang qua./OnCancel",
	}
	Say("<color=orange>LÔ Quan: H·y cho ta biÕt tuyÖt kü ®¹i hiÖp muèn lÜnh héi.<color>",getn(tbSay),tbSay);
end;

function OnExcuteSkill120(nSel, nIndex)
	local nIDItem = tonumber(nIndex);
	SetTask(899,nIDItem);
	OpenGiveBox("§Æt binh ch­¬ng kü n¨ng 120","Binh ph¸p thiªn h¹ nhiÒu v« kÓ, thiªn biÕn v¹n hãa ghÐp cÇn 1000 v¹n l­îng. 6 trang binh ch­¬ng.","OnRead");
end;



function OnRead()
	local nItemIdx, nG, nD, nP, nL, Ser;
	local nCountItem = 0;
	local nCash = 10000000;
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
				if (nG == 6 and nD == 53 and nP == 1) then
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
				if (nG == 6 and nD == 54 and nP == 1) then
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
				if (nG == 6 and nD == 55 and nP == 1) then
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
				if (nG == 6 and nD == 56 and nP == 1) then
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
				if (nG == 6 and nD == 57 and nP == 1) then
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
				if (nG == 6 and nD == 58 and nP == 1) then
					nIndexItem[6] = nItemIdx;
					break;
				end;
			end
		end
	end
	
	local bCount = 0;
	for i = 1,6 do
		if (nIndexItem[i] == nil) then
			Talk(1,"","C¸c h¹ thiÕu binh ch­¬ng kü n¨ng 120 sè <color=yellow> "..i.."<color>. Ta cÇn ®ñ 6 trang binh ch­¬ng 120.");
			return
		else
			bCount = bCount + 1;
		end;
	end;
	if (GetCash() < nCash) then
		Talk(1,"","C«ng rÌn t¹o ra tr¨ng søc rÊt lín, trªn giang hå nµy ta ch­a mét lÇn nãi th¸ch. Mang 1000 v¹n ®Õn ®©y.");
		return
	end

	---------------
	if (bCount >= 6) then
		for i = 1,6 do
			DelItemByIndex(nIndexItem[i]);
			nIndexItem[i] = nil;
		end
		nIndexItem = {};
		AddScriptItem(GetTask(899));
		Pay(nCash);
		Msg2Player("<color=yellow>Binh ch­¬ng nµy ®· ®­îc ta ghÐp thµnh mét bé, ng­¬i cã thÓ ®äc hiÓu nã ®­îc råi<color>");
		SetTask(899,0)
		EndGiveBox();
	end;

end;


function OnCancel()
end;