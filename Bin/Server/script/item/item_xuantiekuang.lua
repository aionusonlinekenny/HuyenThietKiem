-- Huyen Thiet Nguyen Khoang (Detail 146)
-- Stores attribute extracted from green equipment
-- Used to apply attribute to purple equipment

Include("\\script\\lib\\TaskLib.lua")

function main(nItemIdx)
    -- Right-click to view detailed attribute information
    Talk(1, "", GetDesc(nItemIdx))
end

function GetDesc(nItemIdx)
    local szDesc = ""

    -- Get magic attribute data stored in this khoang thach
    -- ma1 = attribute type (nOp)
    -- ma2 = min value
    -- ma3 = max value
    local nOp, nValueMin, nValueMax = GetItemMagicAttrib(nItemIdx, 1)

    if not nOp or nOp <= 0 then
        szDesc = szDesc .. "\n<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>"
    else
        szDesc = szDesc .. "\n<color=green>===== THONG TIN THUOC TINH =====<color>"
        szDesc = szDesc .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh hien 1<color>"
        szDesc = szDesc .. "\n<color=cyan>Ma thuoc tinh:<color> <color=white>" .. nOp .. "<color>"
        szDesc = szDesc .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
        szDesc = szDesc .. "\n<color=green>================================<color>"
    end

    return szDesc
end
