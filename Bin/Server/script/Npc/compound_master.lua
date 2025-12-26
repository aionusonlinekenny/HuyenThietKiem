-- Compound Master NPC
-- Opens the Compound UI for purple item crafting

function main(NpcIndex)
    Talk(1, "", "Ta l� th� r�n huyen bi�n, c� th� gi�p ng��i ch� t�o trang b� t�m t� nh�ng v�t ph�m th�ng th��ng. H�y ch�n loai d�ch v� ng��i c�n:")

    Say("Ch�o m�ng ��n v�i Th� R�n Huy�n Bi�n!", 4,
        "Ch� T�o Trang B� T�m/open_compound",
        "H��ng D�n/guide",
        "Gi�i Thi�u H� Th�ng/introduce",
        "K�t Th�c ��i Tho�i/no")
end

function open_compound()
    -- OpenCompoundUI is a C++ function exported to Lua
    -- This opens the Compound UI window
    OpenCompoundUI()
end

function guide()
    Talk(1, "", [[
<color=yellow>=== H��NG D�N CH� T�O TRANG B� T�M ===<color>

<color=green>1. Ch� T�o Trang B� T�m (Forge):<color>
- C�n: 1 trang b� xanh + 1-7 huy�n tinh
- K�t qu�: Trang b� t�m v�i c�c d�ng thu�c t�nh tr�ng

<color=cyan>2. Kh�m N�m Thu�c T�nh (Enchase):<color>
- C�n: Trang b� t�m + kho�ng th�ch thu�c t�nh
- K�t qu�: �p thu�c t�nh v�o trang b� t�m

<color=orange>3. H�p Th�nh Huy�n Tinh (Compound):<color>
- C�n: 3 huy�n tinh c�ng c�p
- K�t qu�: Huy�n tinh cao c�p h�n

<color=purple>4. Chi�t Xu�t (Distill):<color>
- Chi�t xu�t kho�ng th�ch thu�c t�nh t� trang b�

<color=red>L�u �:<color>
- T� l� th�nh c�ng ph� thu�c v�o s� l��ng v� c�p �� huy�n tinh
- Th�t b�i c� th� m�t nguy�n li�u
]])

    Say("", 2,
        "M� Giao Di�n Ch� T�o/open_compound",
        "Quay L�i/main")
end

function introduce()
    Talk(1, "", [[
<color=yellow>=== H� TH�NG CH� T�O TRANG B� T�M ===<color>

Trang b� t�m l� trang b� cao c�p nh�t trong tr� ch�i, v��t tr�i h�n c� trang b� xanh.

<color=green>�u �i�m c�a Trang B� T�m:<color>
- C� nhi�u d�ng thu�c t�nh h�n trang b� xanh
- C� th� kh�m n�m th�m thu�c t�nh
- S�c m�nh v��t tr�i, ph� h�p cho PvP v� PvE

<color=cyan>Huy�n Tinh:<color>
C� 17 lo�i huy�n tinh, chia l�m 4 c�p:
- <color=cyan>Lam Thuy Tinh<color> (C�p 1-3): C�p th�p
- <color=purple>T� Thuy Tinh<color> (C�p 1-4): C�p trung
- <color=green>L�c Thuy Tinh<color> (C�p 1-5): C�p cao
- <color=red>H�ng Thuy Tinh<color> (C�p 1-5): Huy�n tho�i

S� l��ng v� c�p �� huy�n tinh quy�t ��nh ch�t l��ng trang b� t�m!
]])

    Say("", 2,
        "M� Giao Di�n Ch� T�o/open_compound",
        "Quay L�i/main")
end
