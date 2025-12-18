--Author: Kinnox;
--Date: 17/04/2022;
Include("\\script\\mission\\phonglangdo\\head.lua");
Include("\\script\\lib\\libevent.lua");
function OnDeath(nNpcIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
	local nSer = GetNpcSeries(nNpcIndex);
	local nGoldNpc = GetNpcGold(nNpcIndex);
	local nX,nY,nW = GetNpcPos(nNpcIndex);	
	if (nGoldNpc ~= 6 and nGoldNpc ~= 1) then -- thuy tac
		local sZFile = "\\settings\\droprate\\drop_fld.ini"
		DropRateItem(nNpcIndex, 1, sZFile, 0, random(9,10), nSer);

		local nX,nY,nW = GetNpcPos(nNpcIndex);	
		if (random(1,10) < 2) then
			DropItem(nW,nX,nY,PlayerIndex,6,28,1,1,-1,0,0);
		end;
		
		if (random(1,2) == 2) then
			AddBlood(1,2,nNpcIndex);
		end;
	else -- thuy tac dau linh
		local sZFile = "\\settings\\droprate\\drop_fld.ini"
		DropRateItem(nNpcIndex, 15, sZFile, 0, 10, nSer)
		if (random(1,10) < 3) then
			DropItem(nW,nX,nY,PlayerIndex,7,11,1,1,-1,0,0);
			DropItem(nW,nX,nY,PlayerIndex,7,12,1,1,-1,0,0);
			DropItem(nW,nX,nY,PlayerIndex,7,13,1,1,-1,0,0);
		end;
		--Award Phong Lang do;
		EventPhongLangDo(nNpcIndex,nW,nX,nY,PlayerIndex,nSer)

		-------------------ADD EXP
		local bDrop = 0;
		local OldPlayerIndex = PlayerIndex;
		if (GetTeamSize() > 0) then
			local TAB_Player = {};
			for i=1,GetTeamSize() do
				TAB_Player[i] = GetTeamMember(i);
			end
			PlayerIndex = OldPlayerIndex;
			for i=1,getn(TAB_Player) do
				PlayerIndex = TAB_Player[i];
				if (bDrop == 0) then
					AddOwnExp(500000);
				end;
				bDrop = 1;
			end
		return
		end;
		PlayerIndex = OldPlayerIndex;
		-------------------
	end;
end


function OnRevive(nNpcIndex)
	SetNpcScript(nNpcIndex,PLD.MAIN.NPC_DEATH);
end;

function AddBlood(nMin,nMax,nNpcIndex)
	local Blood = random(1,50);
	local MAX = random(nMin+1,nMax);
	local nX, nY, nSub = GetNpcPos(nNpcIndex)
	if (Blood == 1) then
		nPlayer = PlayerIndex;
		for i = nMin,MAX do
			local nPosX = random(1,10)
			local nPosY = random(1,10)
			local nCal = random (1,2)
			if (nCal == 1 ) then
				nX = nX + nPosX;
				DropItem(nSub,nX,nY,nPlayer,5,2,0,5,0,0,0);
			else
				nX = nX - nPosX;
				DropItem(nSub,nX,nY,nPlayer,5,2,0,5,0,0,0);
			end;
		end;
	end;
end;