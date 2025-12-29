-- Khong Tuoc Nguyen Thach (Detail 147)
-- Stores hidden attribute 1 from green equipment (same element)
-- Right-click to view stored attribute

Include("\\script\\lib\\TaskLib.lua")

function OnUse(nIdx)
    local _, _, nOp, nValueMin, nValueMax = GetItemProp(nIdx)

    if not nOp or nOp <= 0 then
        Talk(1, "", "<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>")
        return 0
    end

    local szMsg = "<color=green>===== THONG TIN THUOC TINH =====<color>"
    szMsg = szMsg .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh an 1<color>"
    szMsg = szMsg .. "\n<color=cyan>Ma thuoc tinh:<color> <color=white>" .. nOp .. "<color>"
    szMsg = szMsg .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
    szMsg = szMsg .. "\n<color=green>================================<color>"

    Talk(1, "", szMsg)
    return 0
end
