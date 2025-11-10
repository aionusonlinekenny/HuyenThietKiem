----------------------------------------
--Script Trap qua maps
--Code By Kinnox
----------------------------------------
Include("\\script\\maps\\libtrap.lua");
function main() -- huong dong
	-- print("TRAP 15");
	local nIDMapsTrap = 11; --ID Map Trap Move
	local nLevel = GetLevel(); -- Check Level Player
	local nState = GetFightState(); -- Check Fight Sate Player
	local nSubWorld = SubWorldID2Idx(nIDMapsTrap) -- Check Open Maps
	
	if (nSubWorld <= 0) then -- Show Msg Trap
		Talk(1,"",LIB_STRING[1]);
		return
	end;
	
	if (nLevel < NOTLEVEL) then -- Show Msg Level
		Talk(1,"",LIB_STRING[2]);
		return
	end;
	

	
	if (nState < 1) then -- Check Pos
		SetPos(3227,5239);
	else
		SetPos(3216,5226);
	end;
	CheckFightState(1);	-- SetFightState	
	AddSkillTrap(); -- Add Anti Damage Magic 3s;
end;