-- Compound Master NPC
-- Opens the Compound UI for purple item crafting

function main(NpcIndex)
    Say("Ta la tho ren huyen bien, co the giup nguoi che tao trang bi tim tu nhung vat pham thuong thuong. Hay chon loai dich vu nguoi can:", 4,
        "Che Tao Trang Bi Tim/open_compound",
        "Huong Dan/guide",
        "Gioi Thieu He Thong/introduce",
        "Ket Thuc Doi Thoai/no")
end

function open_compound()
    -- OpenCompoundUI is a C++ function exported to Lua
    -- This opens the Compound UI window
    OpenCompoundUI()
end

function guide()
    Talk(1, "", [[
<color=yellow>=== HUONG DAN CHE TAO TRANG BI TIM ===<color>

<color=green>1. Che Tao Trang Bi Tim (Forge):<color>
- Can: 1 trang bi xanh + 1-7 huyen tinh
- Ket qua: Trang bi tim voi cac dong thuoc tinh trong

<color=cyan>2. Kham Nam Thuoc Tinh (Enchase):<color>
- Can: Trang bi tim + khoang thach thuoc tinh
- Ket qua: Ep thuoc tinh vao trang bi tim

<color=orange>3. Hop Thanh Huyen Tinh (Compound):<color>
- Can: 3 huyen tinh cung cap
- Ket qua: Huyen tinh cao cap hon

<color=purple>4. Chiet Xuat (Distill):<color>
- Chiet xuat khoang thach thuoc tinh tu trang bi

<color=red>Luu y:<color>
- Ty le thanh cong phu thuoc vao so luong va cap do huyen tinh
- That bai co the mat nguyen lieu
]])

    Say("", 2,
        "Mo Giao Dien Che Tao/open_compound",
        "Quay Lai/main")
end

function introduce()
    Talk(1, "", [[
<color=yellow>=== HE THONG CHE TAO TRANG BI TIM ===<color>

Trang bi tim la trang bi cao cap nhat trong tro choi, vuot troi hon ca trang bi xanh.

<color=green>Uu diem cua Trang Bi Tim:<color>
- Co nhieu dong thuoc tinh hon trang bi xanh
- Co the kham nam them thuoc tinh
- Suc manh vuot troi, phu hop cho PvP va PvE

<color=cyan>Huyen Tinh:<color>
Co 17 loai huyen tinh, chia lam 4 cap:
- <color=cyan>Lam Thuy Tinh<color> (Cap 1-3): Cap thap
- <color=purple>Tu Thuy Tinh<color> (Cap 1-4): Cap trung
- <color=green>Luc Thuy Tinh<color> (Cap 1-5): Cap cao
- <color=red>Hong Thuy Tinh<color> (Cap 1-5): Huyen thoai

So luong va cap do huyen tinh quyet dinh chat luong trang bi tim!
]])

    Say("", 2,
        "Mo Giao Dien Che Tao/open_compound",
        "Quay Lai/main")
end
