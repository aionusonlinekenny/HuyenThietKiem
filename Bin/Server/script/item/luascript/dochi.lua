-- Author: Kinnox
-- Date: 25/12/2021
-- Item: Than bi do chi
Include("\\script\\lib\\TaskLib.lua")
function OnUse(nItemIndex)
	local nW,nX,nY = GetWorldPos();
	local OldPlayerIndex = PlayerIndex;
	if (GetTeamSize() > 0) then
		local TAB_Player = {}
		for i=1,GetTeamSize() do
			TAB_Player[i] = GetTeamMember(i)
		end
		for i=1,getn(TAB_Player) do
			PlayerIndex = TAB_Player[i];
			if (GetTask(TASK_KIND) == 1) and (GetTask(TASK_SAVEQUEST_1) == nW) then
				if (GetTask(TASK_DT_DOCHI) > 0) then
					SetTask(TASK_DT_DOCHI,GetTask(TASK_DT_DOCHI) - 1);
					SyncTask(TASK_DT_DOCHI);
					Msg2Player("Sè ®Þa ®å chÝ cÇn ph¶i t×m cßn l¹i lµ <color=yellow> "..GetTask(TASK_DT_DOCHI).." <color> !");
				end
			else
				if (PlayerIndex == OldPlayerIndex) then
					SetTask(TASK_DOCHI,GetTask(TASK_DOCHI)+1);
					Msg2Player("Ng­¬i nhÆt ®­îc mét cuèn <color=yellow> ThÇn bÝ §Þa §å ChÝ <color>, tæng sè ®Þa ®å chÝ trong ng©n khè lµ <color=cyan>"..GetTask(TASK_DOCHI).."<color> !");
					SyncTask(TASK_DOCHI);
				end;				
			end;
		end;
		TAB_Player = {};
	else	
		PlayerIndex = OldPlayerIndex;
		if (GetTask(TASK_KIND) == 1) and (GetTask(TASK_SAVEQUEST_1) == nW) then
			if (GetTask(TASK_DT_DOCHI) > 0) then
				SetTask(TASK_DT_DOCHI,GetTask(TASK_DT_DOCHI) - 1);
				SyncTask(TASK_DT_DOCHI);
				Msg2Player("Sè ®Þa ®å chÝ cÇn ph¶i t×m cßn l¹i lµ <color=yellow> "..GetTask(TASK_DT_DOCHI).." <color> !");
			end
		else
			SetTask(TASK_DOCHI,GetTask(TASK_DOCHI)+1);
			Msg2Player("Ng­¬i nhÆt ®­îc mét cuèn <color=yellow> ThÇn bÝ §Þa §å ChÝ <color>, tæng sè ®Þa ®å chÝ trong ng©n khè lµ <color=cyan>"..GetTask(TASK_DOCHI).."<color> !");
			SyncTask(TASK_DOCHI);
		end;
	end;
end