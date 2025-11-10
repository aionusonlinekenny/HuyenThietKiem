-- Author:	Kinnox;
-- Date:	25-08-2021
-- Functions: Script Ondeath _ Droprate  Normal / Blue Boss / Gold Boss;
Include("\\script\\mission\\vuotai\\killerboss.lua");
Include("\\script\\lib\\libevent.lua")
function OnDeath(nNpcIndex,nLastdamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastdamage);
	PlayerIndex = nPlayerIndex;
	
	local nX, nY, nSub 		= GetNpcPos(nNpcIndex);
	local nSeries			= GetNpcSeries(nNpcIndex);
	local nLevel  			= GetNpcLevel(nNpcIndex);
	local nDropItem			= "";
	local OldPlayerIndex 	= PlayerIndex;
	local OldSubWorld		= SubWorld;
	local nParamNpc 		= GetNpcParam(nNpcIndex,1);
	local bDrop 			= 0;
	local ExpKillerBoss		= random(100000,500000);
	if (GetTask(KLB.TASK.TASK_ACCEPT_KILLER) < 1) or (GetTask(KLB.TASK.TASK_REGIS_KILLER) < 1) then
	return end;
	
	if (GetTeamSize() > 0) then
		local TAB_Player = {}
		for i=1,GetTeamSize() do
			TAB_Player[i] = GetTeamMember(i)
		end
		PlayerIndex = OldPlayerIndex;
		for i=1,getn(TAB_Player) do
			PlayerIndex = TAB_Player[i]
			if (GetTask(KLB.TASK.TASK_ACCEPT_KILLER) == nParamNpc) then
				SetTask(KLB.TASK.TASK_SUCCES_KILLER,GetTask(KLB.TASK.TASK_SUCCES_KILLER) + 1);
				if (GetTask(KLB.TASK.TASK_SUCCES_KILLER) >= 8) then
					EventKillerBoss(); ---- event killer boss;
				end;
				AddRepute(3);
				AddOwnExp(ExpKillerBoss);
				AddSkillState(541,1,0,180);	
				local Msg = "<color=cyan>Ng≠¨i Æ∑ hπ s∏t Æ≠Óc ta, nh≠ng ta ch≠a ch’t. <color=red>NhÀn Æ≠Óc 3 Æi”m danh v‰ng. <color>H∑y quay v“ phÙc m÷nh Æi."
				Talk(1,"func_Exit",Msg);
				SetTask(KLB.TASK.TASK_ACCEPT_KILLER,0);
				SetTask(KLB.TASK.TASK_REGIS_KILLER,0);
				if (bDrop == 0) then
					nLevel = random(9,10);
					nDropItem = "\\settings\\droprate\\boss\\bosstask_lev90.ini"
					DropRateItem(nNpcIndex,random(5,10),nDropItem,1,nLevel,nSeries);
					DropItem(nSub,nX,nY,PlayerIndex,6,nSeries+2,0,0,0,0,0); -- sat thu lenh;
					local nRandomBK =random(1,5);
					local nRandomIDBK =random(31,58);
					if (nRandomBK == 2) then
						DropItem(nSub,nX,nY,PlayerIndex,7,nRandomIDBK,0,0,0,0,0); -- bikip;
					end;
					if (random(1,100) == 30) then
						DropItem(nSub,nX,nY,PlayerIndex,7,131,0,0,0,0,0); -- stg le hop;
					end;
				end;
				bDrop = 1;
			end;
		end
		return
	end;
	PlayerIndex = OldPlayerIndex;	
	if (GetTask(KLB.TASK.TASK_ACCEPT_KILLER) == nParamNpc) then
		SetTask(KLB.TASK.TASK_SUCCES_KILLER,GetTask(KLB.TASK.TASK_SUCCES_KILLER) + 1);
		if (GetTask(KLB.TASK.TASK_SUCCES_KILLER) >= 8) then
			EventKillerBoss();---- event killer boss;
		end;
		AddRepute(3);
		AddOwnExp(ExpKillerBoss);
		local Msg = "<color=cyan>Ng≠¨i Æ∑ hπ s∏t Æ≠Óc ta, nh≠ng ta ch≠a ch’t. <color=red>NhÀn Æ≠Óc 3 Æi”m danh v‰ng. <color>H∑y quay v“ phÙc m÷nh Æi."
		Talk(1,"func_Exit",Msg)
		nLevel = random(9,10);
		nDropItem = "\\settings\\droprate\\boss\\bosstask_lev90.ini"
		DropRateItem(nNpcIndex,random(5,10),nDropItem,1,nLevel,nSeries);
		DropItem(nSub,nX,nY,PlayerIndex,6,nSeries+2,0,0,0,0,0); -- sat thu lenh;
		local nRandomBK =random(1,5);
		local nRandomIDBK =random(31,58);
		if (nRandomBK == 2) then
			DropItem(nSub,nX,nY,PlayerIndex,7,nRandomIDBK,0,0,0,0,0); -- bikip;
		end;
		if (random(1,100) == 30) then
			DropItem(nSub,nX,nY,PlayerIndex,7,131,0,0,0,0,0); -- stg le hop;
		end;
		AddSkillState(541,1,0,180);	
		SetTask(KLB.TASK.TASK_ACCEPT_KILLER,0);
		SetTask(KLB.TASK.TASK_REGIS_KILLER,0);
	end;
	bDrop = 0;
	SubWorld = OldSubWorld;
end;

function OnRevive(nNpcIndex)
	local Series = GetNpcSeries(nNpcIndex);
	IsBoss(nNpcIndex,1);
	SetNpcSeries(nNpcIndex,Series);
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
	SetNpcScript(nNpcIndex, "\\script\\global\\luanpcmonsters\\ondeath_killerboss.lua")
	SetNpcReduceDamge(nNpcIndex, 30);	
end;

function func_Exit()
end;