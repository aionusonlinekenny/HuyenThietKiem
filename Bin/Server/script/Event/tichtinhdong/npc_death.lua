-- Author:	Kinnox;
-- Date:	15-04-2021
-- Functions: NpcDeath tich tinh dong;
function OnDeath(nNpcIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
	local nX,nY,nW = GetNpcPos(nNpcIndex);
	local nSeries			= GetNpcSeries(nNpcIndex);
	local nLevel  			= GetNpcLevel(nNpcIndex);
	local nGoldNpc 			= GetNpcGold(nNpcIndex);
	local nDropItem			= "";
	local nXMoney 			= 1; -- x2 drop tien
	local EXPBoss			= 3; -- x3 nhan kinh nghiem boss xanh;
	local nPriceBossBlue 	= random(100,500);
	local nXu			 	= random(1,300);
	local nIndexItem = 0;
	if (nLevel <= 0) then
	return end;
	if (nSeries < 0 or nSeries > 4) then 
	return end;
	---///DROPITEM
	DropRateItem(nNpcIndex,random(1,5),"\\settings\\droprate\\npcdroprate_tichtinhdong.ini",1,random(9,10),nSeries)
	AddOwnExp(5000*EXPBoss);
	if (random(1,3) == 3) then
		for i = 1,random(2,3) do
			DropMoney(nPriceBossBlue*nXMoney,nX,nY);
		end;
	end;
	if (nXu == 205) then
		AddTaskItem(19);
	end;
	
end;

function OnRevive(nNpcIndex)
	-- local nRandom = random(1,20);
	-- if (nRandom == 12) then
		IsBoss(nNpcIndex,random(13,15));
	-- end;
	SetNpcScript(nNpcIndex, "\\script\\event\\tichtinhdong\\npc_death.lua");
end;

