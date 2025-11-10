Include("\\script\\event\\thienbaokho\\head.lua");
function OnDeath(nNpcIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
local szNameNpc = GetNpcName(nNpcIndex);
CheckKey(nNpcIndex,szNameNpc,PlayerIndex);

end;
function OnRevive(nNpcIndex)
	local sScriptFile = "\\script\\event\\thienbaokho\\npc_death.lua"
	

	local Series = GetNpcSeries(nNpcIndex);
	IsBoss(nNpcIndex,1);
	SetNpcSeries(nNpcIndex,random(0,4));
	if (Series == 0) then
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	elseif (Series == 1) then
		SetNpcSkill(nNpcIndex,425,50,1);
		SetNpcSkill(nNpcIndex,425,50,2);
		SetNpcSkill(nNpcIndex,425,50,3);
		SetNpcSkill(nNpcIndex,425,50,4);
	elseif (Series == 2) then
		SetNpcSkill(nNpcIndex,426,50,1);
		SetNpcSkill(nNpcIndex,426,50,2);
		SetNpcSkill(nNpcIndex,426,50,3);
		SetNpcSkill(nNpcIndex,426,50,4);
	elseif (Series == 3) then
		SetNpcSkill(nNpcIndex,427,50,1);
		SetNpcSkill(nNpcIndex,427,50,2);
		SetNpcSkill(nNpcIndex,427,50,3);
		SetNpcSkill(nNpcIndex,427,50,4);
	elseif (Series == 4) then
		SetNpcSkill(nNpcIndex,428,50,1);
		SetNpcSkill(nNpcIndex,428,50,2);
		SetNpcSkill(nNpcIndex,428,50,3);
		SetNpcSkill(nNpcIndex,428,50,4);
	else
		SetNpcSkill(nNpcIndex,424,50,1);
		SetNpcSkill(nNpcIndex,424,50,2);
		SetNpcSkill(nNpcIndex,424,50,3);
		SetNpcSkill(nNpcIndex,424,50,4);
	end;
	SetNpcScript(nNpcIdx, sScriptFile)
	SetNpcReduceDamge(nNpcIndex, 30);
end;