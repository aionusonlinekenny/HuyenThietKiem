--Author: Kinnox
--Date: 22/04/2022;
--App: Excuter KillerBoss;
-------KILLER BOSS
KLB ={};
KLB.TASK = {
	TASK_REGIS_KILLER = 802,
	TASK_SUCCES_KILLER = 803,
	TASK_ACCEPT_KILLER = 804,
	NEEDCASH = 20000,
};
-------
function Help(nSel)
	if (nSel == 1) then
	Talk(1,"","<color=orange>NhiÕp NhÝ TrÇn<color>: NhiÖm vô s¸t thñ yªu cÇu ®¨ng cÊp <color=red>80<color>, mçi ngµy ®¹i hiÖp ®­îc phông mÖnh <color=red>8<color> boss s¸t thñ vµ chi phÝ mçi lÇn lµ <color=red>1 v¹n l­îng <color>. ");
	else
	Talk(1,"","<color=orange>NhiÕp NhÝ TrÇn<color>: TrËn chiÕn diÔn ra vµo phót thø <color=red>00<color> mçi giê ch¼n, b¸o danh tr­íc <color=red>15<color> phót. Mçi ngµy ®¹i hiÖp ®­îc chinh ph¹t <color=red>3<color> trËn.");
	end;
end
--==////////////////////////BOSS SAT THU////////////////=---
TAB_BOSSVNG ={
--boss cap 90 141->160 VNG
{768,95,226,1654,3283,0,"Gi¶ Giíi Nh©n","Sa M¹c s¬n ®éng 2"}, 
{812,95,226,1636,3119,0,"TrÞnh Cöu NhËt","Sa M¹c s¬n ®éng 2"}, 
{813,95,226,1736,3213,0,"Chu Së B¸","Sa M¹c s¬n ®éng 2"}, 
{814,95,226,1610,3152,0,"Trang Minh Trung","Sa M¹c s¬n ®éng 2"}, 
{815,95,225,1590,3325,0,"Cam ChÝnh C«","Sa M¹c s¬n ®éng 1"}, 
{816,95,225,1261,3247,0,"Vò NhÊt ThÕ","Sa M¹c s¬n ®éng 1"},  
{817,95,225,1452,3377,0,"D­¬ng Phong  DËt","Sa M¹c s¬n ®éng 1"}, 
{818,95,225,1425,3107,0,"Hµ Sinh Vong","Sa M¹c s¬n ®éng 1"}, 
{819,95,75,1711,3187,0,"T¨ng ChØ ãan","Kho¶ Lang ®éng"}, 
{820,95,75,1752,3124,0,"VÖ Biªn Thµnh","Kho¶ Lang ®éng"}, 
{768,95,75,1831,3190,0,"Cè Thñ §»ng","Kho¶ Lang ®éng"}, 
{812,95,75,1639,3159,0,"Gia C¸t Kinh Hång","Kho¶ Lang ®éng"}, 
{813,95,321,1253,3002,0,"Phan Ng¹t Nh¹n","Tr­êng B¹ch nam"}, 
{814,95,321,1483,2742,0,"Liªn H×nh Th¸i","Tr­êng B¹ch nam"}, 
{815,95,321,1289,2613,0,"Bµo TriÖt s¬n","Tr­êng B¹ch nam"}, 
{816,95,321,1113,2569,0,"V¹n Hå Tinh","Tr­êng B¹ch nam"}, 
{817,95,340,1217,2740,0,"Trö Thiªn MÉn","M¹c Cao QuËt"}, 
{820,95,340,1723,2765,0,"§o¹n L¨ng NguyÖt","M¹c Cao QuËt"}, 
{819,95,340,1275,2749,0,"T¶  DËt Minh","M¹c Cao QuËt"}, 
{818,95,340,1932,2759,0,"NhËm Th­¬ng Khung","M¹c Cao QuËt"}, 
}

function InitLoadBoss()
	for i = 1,getn(TAB_BOSSVNG) do
		local nIndexNpc;
		local Series = random(0,4);		
		-- local Series = 1;		
		nIndexNpc = AddNpc(TAB_BOSSVNG[i][1], TAB_BOSSVNG[i][2], SubWorldID2Idx(TAB_BOSSVNG[i][3]), TAB_BOSSVNG[i][4] * 32, TAB_BOSSVNG[i][5] * 32, 0, TAB_BOSSVNG[i][7],1,0);
		IsBoss(nIndexNpc,1);
		SetNpcSeries(nIndexNpc,Series);
		if (Series == 0) then
			SetNpcSkill(nIndexNpc,424,50,1);
			SetNpcSkill(nIndexNpc,424,50,2);
			SetNpcSkill(nIndexNpc,424,50,3);
			SetNpcSkill(nIndexNpc,424,50,4);
		elseif (Series == 1) then
			SetNpcSkill(nIndexNpc,425,50,1);
			SetNpcSkill(nIndexNpc,425,50,2);
			SetNpcSkill(nIndexNpc,425,50,3);
			SetNpcSkill(nIndexNpc,425,50,4);
		elseif (Series == 2) then
			SetNpcSkill(nIndexNpc,426,50,1);
			SetNpcSkill(nIndexNpc,426,50,2);
			SetNpcSkill(nIndexNpc,426,50,3);
			SetNpcSkill(nIndexNpc,426,50,4);
		elseif (Series == 3) then
			SetNpcSkill(nIndexNpc,427,50,1);
			SetNpcSkill(nIndexNpc,427,50,2);
			SetNpcSkill(nIndexNpc,427,50,3);
			SetNpcSkill(nIndexNpc,427,50,4);
		elseif (Series == 4) then
			SetNpcSkill(nIndexNpc,428,50,1);
			SetNpcSkill(nIndexNpc,428,50,2);
			SetNpcSkill(nIndexNpc,428,50,3);
			SetNpcSkill(nIndexNpc,428,50,4);
		else
			SetNpcSkill(nIndexNpc,424,50,1);
			SetNpcSkill(nIndexNpc,424,50,2);
			SetNpcSkill(nIndexNpc,424,50,3);
			SetNpcSkill(nIndexNpc,424,50,4);
		end;
			SetNpcScript(nIndexNpc, "\\script\\global\\luanpcmonsters\\ondeath_killerboss.lua")
			SetNpcParam(nIndexNpc,1,i);
			SetNpcReduceDamge(nIndexNpc, 30);	
	end;
end;

function KillerMan()
	if (CheckCondition() == 1) then
	return end;
	Pay(KLB.TASK.NEEDCASH);
	local nIDRandom = random(1,20);
	SetTask(KLB.TASK.TASK_ACCEPT_KILLER,nIDRandom);
	AddNote(format("H·y Tiªu DiÖt Gióp Ta: <color=yellow> %s <color> ë <color=green> %d/%d <color> %s.Ta kh«ng tin ng­¬i th¾ng ®­îc h¾n! ",TAB_BOSSVNG[nIDRandom][7],TAB_BOSSVNG[nIDRandom][4]/8,TAB_BOSSVNG[nIDRandom][5]/16,TAB_BOSSVNG[nIDRandom][8]));
	Talk(1,"",format("H·y Tiªu DiÖt Gióp Ta: <color=yellow> %s <color> ë <color=green> %d/%d <color> %s.Ta kh«ng tin ng­¬i th¾ng ®­îc h¾n!  ",TAB_BOSSVNG[nIDRandom][7],TAB_BOSSVNG[nIDRandom][4]/8,TAB_BOSSVNG[nIDRandom][5]/16,TAB_BOSSVNG[nIDRandom][8]));
	SetTask(KLB.TASK.TASK_REGIS_KILLER,1);
end;

function GotoBoss()
	local NeedCash = 5000;
	if (GetCash() < NeedCash) then
		Talk(1,"","C¸c h¹ cÇn tr¶ lé phÝ ®i ®­êng cho thuéc h¹ cña ta, cÇn <color=red>5000<color> l­îng");
	return end;
	Pay(NeedCash);
	NewWorld(TAB_BOSSVNG[GetTask(KLB.TASK.TASK_ACCEPT_KILLER)][3],TAB_BOSSVNG[GetTask(KLB.TASK.TASK_ACCEPT_KILLER)][4],TAB_BOSSVNG[GetTask(KLB.TASK.TASK_ACCEPT_KILLER)][5]);
	SetFightState(1);
	RestoreMana();
	RestoreLife();
	Msg2Player("Ngåi yªn! ta ®­a ng­¬i ®Õn chç "..TAB_BOSSVNG[GetTask(KLB.TASK.TASK_ACCEPT_KILLER)][7].." ë "..TAB_BOSSVNG[GetTask(KLB.TASK.TASK_ACCEPT_KILLER)][8].." ");
end;

function CheckCondition()
	local nRet = 0;
	if (GetLevel() < 80) then
		Talk(1,"","§¼ng cÊp cña nhµ ng­¬i ch­a ®ñ søc ®Ó nhËn nhiÖm vô nµy, h·y rÌn luyÖn lªn ®¼ng cÊp 80");
		nRet = 1;
	end;
	
	if (GetTask(KLB.TASK.TASK_SUCCES_KILLER) > 8) then
		Talk(1,"","§¹i hiÖp qu¶ lµ mét nh©n tµi hiÕm cã toµn bé bän thæ phØ ®· bÞ ®¹i hiÖp tiªu diÖt trän vÑn trong 1 ngµy, ngµy mai h·y quay l¹i nhÐ!");
		nRet = 1;
	end;
	if (GetCash() < KLB.TASK.NEEDCASH) then
		Talk(1,"","NÕu trong qu¸ tr×nh hé tèng ®¹i hiÖp ®i, thuéc h¹ cña triÒu ®×nh tö vong. Ta cÇn <color=red>5<color> v¹n l­îng míi ®Ó lo hËu sù cho hä. Mong c¸c h¹ quyªn gãp!");
		nRet = 1;
	end;
	return nRet;
end;

function RejectQuest()
	SetTask(KLB.TASK.TASK_REGIS_KILLER,0);
	SetTask(KLB.TASK.TASK_ACCEPT_KILLER,0);
end;

function STG()
	OpenGiveBox("§Æt vËt phÈm vµo trong ","Ta cÇn 5 c¸i s¸t thñ lÖnh cïng hÖ ®Ó hîp thµnh!. VD: ng­êi Ðp hÖ háa th× cÇn 5 c¸i hÖ háa ®Ó hîp thµnh. PhÝ ghÐp 10 v¹n l­îng/lÇn","STGExcute")
end;

function STGExcute()
	local nItemIdx, nG, nD, nP, nL, Ser,nGFirst,nDFirst,nPFirst;
	local nCountItem = 0;
	local nCash = 10000;
	local nNumNeed = 5;
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
				if (nG == 6 and (nD >= 2 and nD <= 6) and nP == 1) then
					nGFirst = nG;
					nDFirst = nD;
					nPFirst = nP;
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
				if (nG == nGFirst and nD == nDFirst and nP == nPFirst) then
					nCountItem = nCountItem + 1;
					nIndexItem[nCountItem] = nItemIdx;
				end;
			end
		end
	end
	if (GetCash() < nCash) then
		Talk(1,"","Ng­¬i tiÕc <color=red>1<color> v¹n l­îng ®ª ghÐp s¸t thñ gi¶n sao?. Ng­¬i ®i ®i");
		return
	end;
	
	if (nCountItem == nNumNeed) then
		for i = 1,nNumNeed do
			DelItemByIndex(nIndexItem[i]);
		end;
	else
		Talk(1,"","Mçi lÇn bá vµo ®óng 5 vËt phÈm cïng hÖ, kh«ng nhiÒu còng kh«ng Ýt h¬n!")
		return
	end;
	
	Pay(nCash);
	AddTaskItem(nDFirst+5);
	nIndexItem ={};
	nGFirst = 0;
	nDFirst = 0;
	nPFirst = 0;
	Msg2Player("C¸c h¹ ®· ghÐp thµnh c«ng 1 S¸t thñ gi¶n!");
end;

function no()
end;