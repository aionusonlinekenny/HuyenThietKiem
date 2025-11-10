----------------------------------------
--Script Trap qua maps
--Code By Kinnox
----------------------------------------
Include("\\script\\maps\\trap\\libtrap.lua");
function main()
	-- print("TRAP 7");
	local nIDMapsTrap = 72; --ID Map Trap Move
	local nLevel = GetLevel(); -- Check Level Player
	local nState = GetFightState(); -- Check Fight Sate Player
	local nSubWorld = SubWorldMapID2Idx(nIDMapsTrap) -- Check Open Maps
	
	if (nSubWorld <= 0) then -- Show Msg Trap
		Talk(1,"",LIB_STRING[1]);
		return
	end;
	
	if (nLevel < BALANGHUYEN_1) then -- Show Msg Level
		Talk(1,"",LIB_STRING[2]);
		return
	end;
	
	CheckFightState(2);	-- SetFightState
	
	-- if (nState < 1) then -- Check Pos
		-- SetPos(1500,3200);
	-- else
		-- SetPos(1500,3200);
	-- end;
	NewWorld(72, 1796, 3088);
	AddSkillTrap(); -- Add Anti Damage Magic 3s;
end;