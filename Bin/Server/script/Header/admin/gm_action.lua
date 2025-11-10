----------------------------------
--	Copyright: AJX Team
--	Author: [N]
--	Date: 16/08/2015
--	Desc: GM lµm vi÷c
----------------------------------
Include("\\script\\player\\head.lua")

----------------------------------
--
----------------------------------
function OpenSongJin()
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(380)
	OpenMission(1)
	SubWorld = nOldSubWorld
end