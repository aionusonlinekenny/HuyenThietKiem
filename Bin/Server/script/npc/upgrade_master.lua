--═══════════════════════════════════════════════════════════════
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Opens equipment attribute upgrade UI
-- Date: 2025-12-21
--═══════════════════════════════════════════════════════════════

Include("\\script\\Upgrade\\upgrade_attrib.lua")

function main(NpcIndex)
    dofile("script/npc/upgrade_master.lua")
    UpgradeAttribTalk()
end
