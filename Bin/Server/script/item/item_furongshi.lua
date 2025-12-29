-- Phu Dung Nguyen Thach (Detail 149)
-- Stores hidden attribute 2 from green equipment (same element)
-- Right-click to view stored attribute

Include("\\script\\lib\\TaskLib.lua")

function OnUse(nIdx)
    local nOp, nValueMin, nValueMax = GetItemMagicAttrib(nIdx, 1)

    if not nOp or nOp <= 0 then
        Talk(1, "", "<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>")
        return 0
    end

    local _, _, _, _, nSeries = GetItemProp(nIdx)
    local szSeriesName = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    local szSeries = szSeriesName[nSeries + 1] or "Unknown"

    local szMsg = "<color=green>===== THONG TIN THUOC TINH =====<color>"
    szMsg = szMsg .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh an 2<color>"
    szMsg = szMsg .. "\n<color=cyan>Ngu Hanh (Series):<color> <color=orange>" .. szSeries .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Ma thuoc tinh:<color> <color=white>" .. nOp .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
    szMsg = szMsg .. "\n<color=green>================================<color>"

    Talk(1, "", szMsg)
    return 0
end
