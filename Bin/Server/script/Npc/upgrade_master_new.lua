-- NPC: Upgrade Master - NEW VERSION (no Include error)
function main(NpcIndex)
    local tbSay = {
        "Nang cap thuoc tinh trang bi xanh/OpenUpgradeUI",
        "Huong dan/ShowGuide",
        "Thoat/no",
    }
    Say("Cao thu ren duc: Ta co the giup nguoi nang cap thuoc tinh trang bi xanh!", getn(tbSay), tbSay)
end

function OpenUpgradeUI()
    OpenUpgradeAttribUI()
end

function ShowGuide()
    Talk(1, "", "HUONG DAN: Dat trang bi xanh + Da Nang Cap, nhan nut Nang Cap, chon thuoc tinh. Tang 20% gia tri.")
end

function ExeUpgradeAttrib()
    local nPos = 15
    local nEquipIdx = GetPOItem(nPos, 0)
    local nMaterialIdx = GetPOItem(nPos, 1)

    if nEquipIdx <= 0 then
        Talk(1, "", "Chua dat trang bi vao!")
        return
    end

    if nMaterialIdx <= 0 then
        Talk(1, "", "Chua dat Da Nang Cap!")
        return
    end

    local nMatGenre, nMatDetail = GetItemProp(nMaterialIdx)
    if nMatGenre ~= 6 or nMatDetail ~= 18 then
        Talk(1, "", "Vat lieu khong dung! Can Da Nang Cap.")
        return
    end

    local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)
    if nGenre ~= 0 then
        Talk(1, "", "Chi nang cap duoc trang bi xanh!")
        return
    end

    if nLuck >= 1000000000 then
        Talk(1, "", "Trang bi tim/vang khong the nang cap!")
        return
    end

    local bHasMagic = 0
    for i = 0, 5 do
        local nType = GetItemMagicAttribInfo(nEquipIdx, i)
        if nType and nType > 0 then
            bHasMagic = 1
            break
        end
    end

    if bHasMagic == 0 then
        Talk(1, "", "Trang bi khong co thuoc tinh magic!")
        return
    end

    local tbOpts = {}
    local nCount = 0
    for i = 0, 5 do
        local nType, nVal, nMin, nMax = GetItemMagicAttribInfo(nEquipIdx, i)
        if nType and nType > 0 then
            if nMax <= 0 or nVal < nMax then
                local nInc = (nVal * 20) / 100
                if nInc < 1 then nInc = 1 end
                local nNew = nVal + nInc
                if nMax > 0 and nNew > nMax then nNew = nMax end
                nCount = nCount + 1
                tbOpts[nCount] = "Type " .. nType .. ": " .. nVal .. " -> " .. nNew .. "/DoUpgrade_" .. i
            end
        end
    end

    if nCount == 0 then
        Talk(1, "", "Tat ca thuoc tinh da MAX!")
        return
    end

    nCount = nCount + 1
    tbOpts[nCount] = "Huy/no"
    Say("Chon thuoc tinh:", getn(tbOpts), tbOpts)
end

function PerformUpgrade(nSlot)
    local nPos = 15
    local nEquipIdx = GetPOItem(nPos, 0)
    local nMatIdx = GetPOItem(nPos, 1)

    if nEquipIdx <= 0 or nMatIdx <= 0 then
        Talk(1, "", "Loi: Vat pham bi mat!")
        return
    end

    local nType, nOldVal = GetItemMagicAttribInfo(nEquipIdx, nSlot)
    local nNewIdx = UpgradeItemAttributes(nEquipIdx, nSlot, 20, nPos)

    if nNewIdx == 0 then
        Talk(1, "", "Loi: Khong the nang cap!")
        return
    end

    DelItemByIndex(nMatIdx)
    Talk(1, "", "Thanh cong! Type " .. nType .. " tang 20%")
end

function DoUpgrade_0() PerformUpgrade(0) end
function DoUpgrade_1() PerformUpgrade(1) end
function DoUpgrade_2() PerformUpgrade(2) end
function DoUpgrade_3() PerformUpgrade(3) end
function DoUpgrade_4() PerformUpgrade(4) end
function DoUpgrade_5() PerformUpgrade(5) end

-- Cancel/Exit function (called by "Huy/no" and "Thoat/no" options)
function no()
    -- Do nothing, dialog will close automatically
end
