--═══════════════════════════════════════════════════════════════
-- NPC: Upgrade Master (Cao Thu Ren Duc)
-- Function: Opens equipment attribute upgrade UI
-- Date: 2025-12-21
--═══════════════════════════════════════════════════════════════

function main(NpcIndex)
    -- Load upgrade functions into GLOBAL Lua state
    -- This makes ExeUpgradeAttrib available when UI button is clicked
    dofile("\\script\\Upgrade\\upgrade_attrib.lua")

    -- Now call the talk function (which is now loaded)
    UpgradeAttribTalk()
end
