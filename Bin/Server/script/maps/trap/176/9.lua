----------------------------------------
--Script Trap qua maps
--Code By Kinnox
----------------------------------------
Include("\\script\\maps\\libtrap.lua");
function main()
	-- print("TRAP 9");
	local nIDMapsTrap = 176; --ID Map Trap Move
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
		SetPos(1719,3332);
	else
		SetPos(1715,3328);
	end;
	CheckFightState(1);	-- SetFightState	
	AddSkillTrap(); -- Add Anti Damage Magic 3s;
end;