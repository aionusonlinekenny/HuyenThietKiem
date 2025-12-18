----------------------------------------
--Author: Kinnox
--Date: 03/09/2021
--Head: Mission Lien Dau
----------------------------------------	
Include("\\script\\mission\\liendau\\head.lua");
function OnTimer()
	local OldSubWorld = SubWorld;
	SubWorld = SubWorldID2Idx(396);
	local OldPlayerIndex = PlayerIndex;
	LDHead.ExcuteSubTimer();
	SubWorld = OldSubWorld;
	PlayerIndex = OldPlayerIndex;
end;

