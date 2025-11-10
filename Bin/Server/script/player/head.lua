Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\player\\resettask.lua")
if (not PLAYER_HEAD) then
PLAYER_HEAD = 1

ZPlayer = {}
function ZPlayer:Refresh()
	--MyRefresh(1)
	self:RestoreState()
end;

function ZPlayer:RestoreState()
	local nTimeExpEnhance = GetTask(2015)
	if(nTimeExpEnhance > 0) then
		AddSkillState(440, 1, 1, nTimeExpEnhance)
	end
end;

function ZPlayer:AddKhinhCong()
	if (HaveMagic(210) < 1 ) then
		AddMagic(210,1);
	end
end;

function ZPlayer:ResetTask()
	CheckTaskReset();
end;


end;