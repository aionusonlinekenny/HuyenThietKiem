-- Huyen Thiet Nguyen Khoang (Detail 146) - Thuoc tinh hien 1
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\MagicAttribName.lua")

function OnUse(nIdx)
    -- Get stored attribute data using GetItemMagicAttribInfo (returns Type, Value, Min, Max)
    -- Use slot 0 for detail 146 (visible attribute 1)
    local nOp, nValue, nValueMin, nValueMax = GetItemMagicAttribInfo(nIdx, 0)

    -- Debug: check what series and values we got
    local nGenre, nDetail, _, _, nSeries = GetItemProp(nIdx)
    Msg2Player(format("[ONUSE DEBUG] Idx=%d, Genre=%d, Detail=%d, Series=%d, Type=%d, Min=%d, Max=%d",
        nIdx, nGenre or -1, nDetail or -1, nSeries or -999, nOp or -1, nValueMin or -1, nValueMax or -1))

    if not nOp or nOp <= 0 then
        Talk(1, "", "<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>")
        return 0
    end

    local szSeriesName = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    local szSeries = szSeriesName[nSeries + 1] or "Unknown"

    -- Get attribute name from ID
    local szAttribName = GetMagicAttribName(nOp)

    local szMsg = "<color=green>===== THONG TIN THUOC TINH =====<color>"
    szMsg = szMsg .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh hien 1<color>"
    szMsg = szMsg .. "\n<color=cyan>Ngu Hanh:<color> <color=orange>" .. szSeries .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Thuoc tinh:<color> <color=white>" .. szAttribName .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
    szMsg = szMsg .. "\n<color=green>================================<color>"

    Talk(1, "", szMsg)
    return 0
end
