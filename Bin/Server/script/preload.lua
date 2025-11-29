-- sau khi khoi dong xong gs, auto chay cai nay de add npc vv
Include("\\script\\npclib\\beginning.lua")

-- Stub functions for compatibility with Linux scripts
function IncludeLib(szLibName)
	-- Stub function for C++ library loading
	-- HuyenThietKiem may have these libraries pre-loaded or not implemented
	-- Common libraries: FILESYS, SETTING, LEAGUE, PARTNER, TASKSYS, RELAYLADDER
end

function main()
	SetGlbMissionV(48,100000000)
	SetGlbMissionV(49,1)
	SetGlbMissionV(97,0)
	SetGlbMissionV(98,0)
	SetGlbMissionV(99,0)
	Begin_Init()
end

