--═══════════════════════════════════════════════════════════════
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Opens equipment attribute upgrade UI
-- Date: 2025-12-21
--═══════════════════════════════════════════════════════════════

-- Load upgrade functions into GLOBAL scope (not just NPC scope)
-- This ensures ExeUpgradeAttrib is available when UI button is clicked
dofile("\\script\\Upgrade\\upgrade_attrib.lua")

function main(NpcIndex)
    UpgradeAttribTalk()
end
