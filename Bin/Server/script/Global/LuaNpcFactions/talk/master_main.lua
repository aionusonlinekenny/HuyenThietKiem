----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: M«n ph¸i 
----------------------------------
Include("\\script\\Global\\LuaNpcFactions\\faction_head.lua")
Include("\\script\\Global\\LuaNpcFactions\\faction_help.lua")

----------------------------------
-- Gia nhËp m«n ph¸i
----------------------------------
function join_main(nFactionId)
	return Faction:JoinFaction(nFactionId)
end

----------------------------------
-- XuÊt s­ khái ph¸i
----------------------------------
function leave_main(nFactionId)
	return Faction:LeaveFaction(nFactionId)
end

----------------------------------
-- Quay vÒ m«n ph¸i
----------------------------------
function return_main(nFactionId)
	return Faction:ReturnFaction(nFactionId)
end

----------------------------------
-- T×m hiÓu khu vùc luyÖn c«ng
----------------------------------
function map_help()
	FHelp:Map()
end

----------------------------------
-- T×m hiÓu vâ c«ng
----------------------------------
function skill_help(nFactionId)
	FHelp:Skill(nFactionId)
end
