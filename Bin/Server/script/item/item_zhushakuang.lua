-- Chu Sa Nguyen Khoang (Detail 150) - Thuoc tinh hien 3
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\MagicAttribName.lua")

function OnUse(nIdx)
    -- Get stored attribute data from magic attribute slot 1
    local nOp, nValueMin, nValueMax = GetItemMagicAttrib(nIdx, 1)

    if not nOp or nOp <= 0 then
        Talk(1, "", "<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>")
        return 0
    end

    -- Get series (element type) from item properties
    local _, _, _, _, nSeries = GetItemProp(nIdx)
    local szSeriesName = {"Kim", "Moc", "Thuy", "Hoa", "Tho"}
    local szSeries = szSeriesName[nSeries + 1] or "Unknown"

    -- Get attribute name from ID
    local szAttribName = GetMagicAttribName(nOp)

    local szMsg = "<color=green>===== THONG TIN THUOC TINH =====<color>"
    szMsg = szMsg .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh hien 3<color>"
    szMsg = szMsg .. "\n<color=cyan>Ngu Hanh:<color> <color=orange>" .. szSeries .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Thuoc tinh:<color> <color=white>" .. szAttribName .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
    szMsg = szMsg .. "\n<color=green>================================<color>"

    Talk(1, "", szMsg)
    return 0
end
