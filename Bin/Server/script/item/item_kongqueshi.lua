-- Khong Tuoc Nguyen Thach (Detail 147) - Thuoc tinh an 1
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\MagicAttribName.lua")

function OnUse(nIdx)
    -- Get stored attribute data using GetItemMagicAttribInfo (returns Type, Value, Min, Max)
    -- ALL khoang thach items store data in slot 0 (as per KItemSet.cpp AddExist)
    local nOp, nValue, nValueMin, nValueMax = GetItemMagicAttribInfo(nIdx, 0)

    if not nOp or nOp <= 0 then
        Talk(1, "", "<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>")
        return 0
    end

    -- Get series from generator level[4] (more reliable than GetItemProp for script items)
    local nGL0, nGL1, nGL2, nGL3, nSeries, nGL5 = GetItemGeneratorLevels(nIdx)
    local szSeriesName = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    local szSeries = szSeriesName[nSeries + 1] or "Unknown"

    -- Get attribute name from ID
    local szAttribName = GetMagicAttribName(nOp)

    local szMsg = "<color=green>===== THONG TIN THUOC TINH =====<color>"
    szMsg = szMsg .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh an 1<color>"
    szMsg = szMsg .. "\n<color=cyan>Ngu Hanh:<color> <color=orange>" .. szSeries .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Thuoc tinh:<color> <color=white>" .. szAttribName .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
    szMsg = szMsg .. "\n<color=green>================================<color>"

    Talk(1, "", szMsg)
    return 0
end
