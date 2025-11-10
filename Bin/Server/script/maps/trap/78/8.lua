----------------------------------------
--Script Trap qua maps
--Code By Kinnox
----------------------------------------
Include("\\script\\maps\\libtrap.lua");
function main()
	-- print("TRAP 8");
	local nIDMapsTrap = 78; --ID Map Trap Move
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
		SetPos(1622,3416);
	else
		SetPos(1620,3415);
	end;
	CheckFightState(1);	-- SetFightState	
	AddSkillTrap(); -- Add Anti Damage Magic 3s;
end;