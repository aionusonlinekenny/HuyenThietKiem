----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Binh sÜ hiÖu phï
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine()
	if(GetFightState() == 0) then 
		Say("Kh«ng thÓ sö dông ®¹o cô nµy khi ë tr¹ng th¸i phi chiÕn ®Êu.", 0) 
		return 0
	end
	local nCamp = nCamp
	if(nCamp == 1) then 
		if (GetMissionV(ZBattle.tbMission.CALLNPCCOUNT_S) >= ZBattle.MAX_CALL_NPC) then 
			Msg2Player("Sè l­îng binh sÜ gäi ra ®· ®Õn giíi h¹n, kh«ng thÓ gäi thªm.") 
			return 0 
		else 
			SetMissionV(ZBattle.tbMission.CALLNPCCOUNT_S, GetMissionV(ZBattle.tbMission.CALLNPCCOUNT_S) + 1)
		end 
	elseif(nCamp == 2) then 
		if (GetMissionV(ZBattle.tbMission.CALLNPCCOUNT_J) >= ZBattle.MAX_CALL_NPC) then 
			Msg2Player("Sè l­îng binh sÜ gäi ra ®· ®Õn giíi h¹n, kh«ng thÓ gäi thªm.") 
			return 0
		else 
			SetMissionV(ZBattle.tbMission.CALLNPCCOUNT_J, GetMissionV(ZBattle.tbMission.CALLNPCCOUNT_J) + 1)		
		end 
	end 
	local W, X, Y = GetWorldPos()
	if (GetLevel() >= 40 and GetLevel() <= 79) then
		if(nCamp == 1) then
			CallSjNpc(682, 50, W, X, Y," - Tèng Binh")
		elseif(nCamp == 2) then 
			CallSjNpc(688, 50, W, X, Y," - Kim Binh")
		end 
	elseif (GetLevel() >= 80 and GetLevel() <= 119) then
		if(nCamp == 1) then 
			CallSjNpc(682, 70, W, X, Y," - Tèng Binh") 
		elseif(nCamp == 2) then 
			CallSjNpc(688, 70, W, X, Y," - Kim Binh")
		end 
	else
		if(nCamp == 1) then 
			CallSjNpc(682, 90, W, X, Y," - Tèng Binh")
		elseif(nCamp == 2) then 
			CallSjNpc(688, 90, W, X, Y," - Kim Binh")
		end 
	end
	return 1
end 

----------------------------------
--
----------------------------------
function CallSjNpc(NpcId, NpcLevel, W, X, Y, Name) 
	local sPName = GetName() 
	AddNpc(NpcId, NpcLevel, SubWorldID2Idx(W), ( X - 3 ) * 32, Y * 32, 1, sPName..Name, 0) 
	AddNpc(NpcId, NpcLevel, SubWorldID2Idx(W), ( X +  3 ) * 32, Y * 32, 1, sPName..Name, 0)
end