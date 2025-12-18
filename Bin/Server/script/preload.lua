-- sau khi khoi dong xong gs, auto chay cai nay de add npc vv
Include("\\script\\npclib\\beginning.lua")

function main()
	SetGlbMissionV(48,100000000)
	SetGlbMissionV(49,1)
	SetGlbMissionV(97,0)
	SetGlbMissionV(98,0)
	SetGlbMissionV(99,0)
	Begin_Init()
end

