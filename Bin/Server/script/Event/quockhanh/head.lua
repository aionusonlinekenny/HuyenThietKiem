-- Author: Kinnox
-- Date: 12/04/2022
-- Chuc nang: Quoc Khanh
Include("\\script\\system_config.lua");
Include("\\script\\lib\\TaskLib.lua")
NPCPOS_QUOCKHANH = {
[53] = {
	{52000,102368},
	
}
}
QUOCKHANHTALK = "<color=green>%s<color> ®ang diÔn ra sù kiÖn Quèc Kh¸nh, h·y nhanh ch©n tham gia.";
FILE_SCRIPT = "\\script\\event\\quockhanh\\npcsukien.lua";
NPC_QUOCKHANH_ADDED = NPC_QUOCKHANH_ADDED or 0
DAYS = 2510301930
function CheckAndRunQuocKhanh()
    local nYr, nMo, nDy, nHr, nMi, nSe = GetTimeNow()
    local currentKey = nYr * 100000000 + nMo * 1000000 + nDy * 10000 + nHr * 100 + nMi

    if currentKey >= DAYS then
        if NPC_QUOCKHANH_ADDED == 0 then
            addnpcquockhanh()
            NPC_QUOCKHANH_ADDED = 1
        end
    else
        if NPC_QUOCKHANH_ADDED == 1 then
            delnpcquockhanh()
            NPC_QUOCKHANH_ADDED = 0
        end
    end
end
function addnpcquockhanh()
	local nSubWorldId = SubWorldID2Idx(53)
	if nSubWorldId < 0 then	--chua mo map, ngung ham`
	return end

	local nNpcIndex;
	for i = 1,getn(NPCPOS_QUOCKHANH[53]) do
		nNpcIndex = AddNpc(1456, 1, nSubWorldId, NPCPOS_QUOCKHANH[53][i][1], NPCPOS_QUOCKHANH[53][i][2], 0, "[NPC] Sù KiÖn Quèc Kh¸nh", 1);
		SetNpcScript(nNpcIndex, FILE_SCRIPT);
		SetNpcParam(nNpcIndex,1,i);	
	end;
	Msg2SubWorld(format(QUOCKHANHTALK,GetMapName(53)));	
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>[NPC] Sù KiÖn Quèc Kh¸nh<color> ®· xuÊt hiÖn ë Ba L¨ng HuyÖn <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
return end;

function delnpcquockhanh()
	ClearMapNpcWithName(53,"[NPC] Sù KiÖn Quèc Kh¸nh");
end;