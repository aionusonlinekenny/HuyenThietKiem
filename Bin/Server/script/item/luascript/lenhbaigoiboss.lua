--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\lib\\Revivepos.lua")
function OnUse(nIndex)
	--dofile("script/item/luascript/lenhbaigoiboss.lua")
	local nW,nX,nY = GetWorldPos();
	local szName = GetName();
	local szMapName = GetMapName(nW);
	nX = nX*32;
	nY = nY*32;
	if (CheckMapNoForTHP(nW) == 1) then
		Talk(1,"", "Kh«ng thÓ triÖu håi boss ë ®©y.")
	return end	
	
	if (GetFightState() < 1) then
		Talk(1,"","Tr¹ng th¸i phi chiÕn ®Êu kh«ng thÓ triÖu håi boss");
		return
	end;
	
	local nNpcIndex = 0;

	local SubWorld = SubWorldID2Idx(nW);
	nNpcIndex = AddNpc(RANDOMC(513,523,511), 95, SubWorld, nX, nY, 1, szName, 2, 0);
	SetNpcName(nNpcIndex,"§¹o tÆc "..szName.."");
	SetNpcReduceDamge(nNpcIndex, 60);	
	SetNpcScript(nNpcIndex, "\\script\\global\\luanpcmonsters\\ondeath_normal.lua")
	--Msg2SubWorld(" §¹i hiÖp "..GetName().." triÖu håi Boss HK "..szName.." t¹i "..floor(nX/256).." - "..floor(nY/512).." "..szMapName.." ");
	Msg2SubWorld(" §¹i hiÖp <color=green>"..szName.."<color> triÖu håi <color=yellow>Boss HK §¹o tÆc "..szName.."<color> t¹i mét n¬i nµo ®ã!. ");
	
	return 1;
end;