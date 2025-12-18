-- Author: Kinnox
-- Date: 25/12/2021
-- Item: Than bi mat chi
Include("\\script\\lib\\tasklib.lua");
function OnUse(nItemIndex)
	--Da tau;
	local nW,nX,nY = GetWorldPos();
	local OldPlayerIndex = PlayerIndex;
	if (GetTeamSize() > 0) then
		local TAB_Player = {}
		for i=1,GetTeamSize() do
			TAB_Player[i] = GetTeamMember(i)
		end
		for i=1,getn(TAB_Player) do
			PlayerIndex = TAB_Player[i];
			if (GetTask(TASK_KIND) == 2) and (GetTask(TASK_SAVEQUEST_1) == nW) then
				if (GetTask(TASK_DT_MATCHI) > 0) then
					SetTask(TASK_DT_MATCHI,GetTask(TASK_DT_MATCHI) - 1);
					SyncTask(TASK_DT_MATCHI);
					Msg2Player("Sè mËt chÝ cÇn ph¶i t×m cßn l¹i lµ <color=yellow> "..GetTask(TASK_DT_MATCHI).." <color> !");
				end
			else
				if (PlayerIndex == OldPlayerIndex) then
					SetTask(TASK_MATCHI,GetTask(TASK_MATCHI)+1);
					Msg2Player("Ng­¬i nhÆt ®­îc mét cuèn <color=yellow> ThÇn bÝ §Þa §å ChÝ <color>, tæng sè ®Þa ®å chÝ trong ng©n khè lµ <color=cyan>"..GetTask(TASK_DOCHI).."<color> !");
					SyncTask(TASK_MATCHI);
				end;
			end;
		end;
		TAB_Player = {};
	else	
		PlayerIndex = OldPlayerIndex;
		if (GetTask(TASK_KIND) == 2) and (GetTask(TASK_SAVEQUEST_1) == nW) then
			if (GetTask(TASK_DT_MATCHI) > 0) then
				SetTask(TASK_DT_MATCHI,GetTask(TASK_DT_MATCHI) - 1);
				SyncTask(TASK_DT_MATCHI);
				Msg2Player("Sè mËt chÝ cÇn ph¶i t×m cßn l¹i lµ <color=yellow> "..GetTask(TASK_DT_MATCHI).." <color> !");
			end
		else
			SetTask(TASK_MATCHI,GetTask(TASK_MATCHI)+1);
			Msg2Player("Ng­¬i nhÆt ®­îc mét cuèn <color=yellow> ThÇn bÝ MËt ChÝ <color>, tæng sè ®Þa ®å chÝ trong ng©n khè lµ <color=cyan>"..GetTask(TASK_MATCHI).."<color> !");
			SyncTask(TASK_MATCHI);
		end;
	end;
end


