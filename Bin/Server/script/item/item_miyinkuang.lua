-- Mat Ngan Nguyen Khoang (Detail 82)
-- Stores visible attribute 2 from green equipment

function main(nItemIdx)
    Talk(1, "", GetDesc(nItemIdx))
end

function GetDesc(nItemIdx)
    local szDesc = ""
    local nOp, nValueMin, nValueMax = GetItemMagicAttrib(nItemIdx, 1)

    if not nOp or nOp <= 0 then
        szDesc = szDesc .. "\n<color=yellow>Khoang thach chua rong (chua co thuoc tinh)<color>"
    else
        szDesc = szDesc .. "\n<color=green>===== THONG TIN THUOC TINH =====<color>"
        szDesc = szDesc .. "\n<color=cyan>Loai khoang thach:<color> <color=yellow>Thuoc tinh hien 2<color>"
        szDesc = szDesc .. "\n<color=cyan>Ma thuoc tinh:<color> <color=white>" .. nOp .. "<color>"
        szDesc = szDesc .. "\n<color=cyan>Gia tri:<color> <color=white>" .. nValueMin .. " - " .. nValueMax .. "<color>"
        szDesc = szDesc .. "\n<color=green>================================<color>"
    end

    return szDesc
end
