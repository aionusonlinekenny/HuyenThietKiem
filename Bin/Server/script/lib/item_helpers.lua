-----------------------------------------------------------------
-- Item Helper Functions
-- Author: Claude Code
-- Date: 2026-01-01
-- Purpose: Centralized item type detection using GENRE instead of LUCK
--
-- This replaces the old system that used luck >= 1000000000 to detect purple items.
-- New system uses genre values directly for cleaner, more maintainable code.
-----------------------------------------------------------------

-- Constants for item genres (from C++ GameDataDef.h)
ITEM_GENRE_EQUIP = 0        -- Blue/White equipment
ITEM_GENRE_PURPLE = 1       -- Purple equipment
ITEM_GENRE_GOLD = 2         -- Gold equipment
ITEM_GENRE_PLATINUM = 3     -- Platinum equipment
ITEM_GENRE_BROKEN = 4       -- Broken equipment
ITEM_GENRE_MEDICINE = 5     -- Medicine
ITEM_GENRE_TASK = 6         -- Task items
ITEM_GENRE_SCRIPT = 7       -- Script items
ITEM_GENRE_MINE = 8         -- Mine items

-- Legacy luck values (for reference only - DO NOT USE for new code)
LEGACY_PURPLE_LUCK = 1000000000  -- Old purple detection method (DEPRECATED)
LEGACY_GOLD_LUCK = 7531          -- Old gold detection method (DEPRECATED)

-----------------------------------------------------------------
-- Get item type by genre (primary method)
-- Returns: "white", "blue", "purple", "gold", "platinum", or "unknown"
-----------------------------------------------------------------
function GetItemTypeByGenre(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return "unknown"
    end

    local nGenre, _, _, _, _, nLuck = GetItemProp(nItemIdx)

    if nGenre == ITEM_GENRE_PURPLE then
        return "purple"
    elseif nGenre == ITEM_GENRE_GOLD then
        return "gold"
    elseif nGenre == ITEM_GENRE_PLATINUM then
        return "platinum"
    elseif nGenre == ITEM_GENRE_EQUIP then
        -- For genre=0, distinguish white vs blue by luck value
        if not nLuck or nLuck == 0 then
            return "white"
        else
            return "blue"
        end
    end

    return "unknown"
end

-----------------------------------------------------------------
-- Check if item is blue equipment
-- Blue = genre 0, luck > 0 (has magic attributes)
-----------------------------------------------------------------
function IsBlueEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre, _, _, _, _, nLuck = GetItemProp(nItemIdx)
    return (nGenre == ITEM_GENRE_EQUIP and nLuck and nLuck > 0)
end

-----------------------------------------------------------------
-- Check if item is white equipment
-- White = genre 0, luck = 0 (no magic attributes)
-----------------------------------------------------------------
function IsWhiteEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre, _, _, _, _, nLuck = GetItemProp(nItemIdx)
    return (nGenre == ITEM_GENRE_EQUIP and (not nLuck or nLuck == 0))
end

-----------------------------------------------------------------
-- Check if item is purple equipment
-- Purple = genre 1
-----------------------------------------------------------------
function IsPurpleEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre = GetItemProp(nItemIdx)
    return (nGenre == ITEM_GENRE_PURPLE)
end

-----------------------------------------------------------------
-- Check if item is gold equipment
-- Gold = genre 2
-----------------------------------------------------------------
function IsGoldEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre = GetItemProp(nItemIdx)
    return (nGenre == ITEM_GENRE_GOLD)
end

-----------------------------------------------------------------
-- Check if item is platinum equipment
-- Platinum = genre 3
-----------------------------------------------------------------
function IsPlatinumEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre = GetItemProp(nItemIdx)
    return (nGenre == ITEM_GENRE_PLATINUM)
end

-----------------------------------------------------------------
-- Check if item is any type of equipment (genre 0-3)
-----------------------------------------------------------------
function IsEquipment(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        return false
    end

    local nGenre = GetItemProp(nItemIdx)
    return (nGenre >= ITEM_GENRE_EQUIP and nGenre <= ITEM_GENRE_PLATINUM)
end

-----------------------------------------------------------------
-- Get equipment color name for display
-- Returns: "trắng", "xanh", "tím", "vàng", "bạch kim"
-----------------------------------------------------------------
function GetEquipmentColorName(nItemIdx)
    local itemType = GetItemTypeByGenre(nItemIdx)

    if itemType == "white" then
        return "trắng"
    elseif itemType == "blue" then
        return "xanh"
    elseif itemType == "purple" then
        return "tím"
    elseif itemType == "gold" then
        return "vàng"
    elseif itemType == "platinum" then
        return "bạch kim"
    end

    return "không xác định"
end

-----------------------------------------------------------------
-- Debug: Print item type info
-----------------------------------------------------------------
function DebugPrintItemType(nItemIdx)
    if not nItemIdx or nItemIdx <= 0 then
        Msg2Player("[DEBUG] Invalid item index: " .. tostring(nItemIdx))
        return
    end

    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nItemIdx)
    local itemType = GetItemTypeByGenre(nItemIdx)
    local colorName = GetEquipmentColorName(nItemIdx)

    Msg2Player("[DEBUG ITEM] Index=" .. nItemIdx)
    Msg2Player("  Genre=" .. (nGenre or -1) .. ", Type=" .. itemType .. ", Color=" .. colorName)
    Msg2Player("  Detail=" .. (nDetail or -1) .. ", Parti=" .. (nParti or -1))
    Msg2Player("  Level=" .. (nLevel or -1) .. ", Series=" .. (nSeries or -1) .. ", Luck=" .. (nLuck or -1))
end
