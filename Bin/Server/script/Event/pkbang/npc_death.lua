Include("\\script\\lib\\tasklib.lua");
Include("\\script\\lib\\libevent.lua")
Include("\\script\\event\\pkbang\\head.lua")
function OnDeath(nNpcIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
	local nX,nY,nW = GetNpcPos(nNpcIndex);
	local nSeries			= GetNpcSeries(nNpcIndex);
	local nLevel  			= GetNpcLevel(nNpcIndex);
	local nGoldNpc 			= GetNpcGold(nNpcIndex);
	----///BOSS Bang
	if (GetTongID() > 0) then
		EventBossHoangKim(nNpcIndex,nW,nX,nY,PlayerIndex,nSeries);
		EventBossHoangKim(nNpcIndex,nW,nX,nY,PlayerIndex,nSeries);
		AddNoiChaoBang(GetTongName());
	end;
end;