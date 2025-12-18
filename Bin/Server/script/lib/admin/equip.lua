----------------------------------
--	Copyright: JxOnline by kinnox;
--	Author: kinnox
--	Date: 15/11/2021
--	Desc: Script GM Equip
----------------------------------
Include("\\script\\lib\\admin\\equip\\horse.lua")
Include("\\script\\lib\\admin\\equip\\basicgold.lua")
function WPlayer_Equip()
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Trang bÞ xanh./PL_BlueEquip",
			"ChiÕn m·./GM_GetHorse",
			"MÆt n¹./PL_Mask",
			"Trang bi Hoµng Kim C¬ B¶n/PL_BasicGoldEquip",
			"trang bÞ Hoµng Kim M«n Ph¸i /PL_PremiumGoldEquip",
			"Phi Phong/PL_FiFong",
			-- "Trang bÞ hoµng kim cao cÊp./setselect",
			"Trë vÒ tr­íc./main",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BlueEquip()
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Vò KhÝ./PL_BlueEquipA",
			"Y Phôc./PL_BlueEquipA",
			"NhÉn./PL_BlueEquipA",
			"H¹ng Liªn./PL_BlueEquipA",
			"Hµi Tö./PL_BlueEquipA",
			"Yªu §¸i./PL_BlueEquipA",
			"§Ønh M¹o./PL_BlueEquipA",
			"Hé UyÓn./PL_BlueEquipA",
			"Ngäc Béi./PL_BlueEquipA",
			-- "Trë vÒ tr­íc./WPlayer_Equip",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BlueEquipA(nSel)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	if(nSel == 0) then
		local tbSay = {
			"Tr­êng KiÕm./PL_Weapon",
			"§¬n §ao./PL_Weapon",
			"C«n-Bæng./PL_Weapon",
			"Th­¬ng-KÝch./PL_Weapon",
			"Song Chïy./PL_Weapon",
			"Song §ao./PL_Weapon",
			"Phi Tiªu./PL_Weapon",
			"Phi §ao./PL_Weapon",
			"Tô TiÔn./PL_Weapon",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 1) then
		local tbSay = {
			"Nam./PL_Armor",
			"N÷./PL_Armor",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 2) then
		local tbSay = {
			"CÊp 1./PL_Ring",
			"CÊp 2./PL_Ring",
			"CÊp 3./PL_Ring",
			"CÊp 4./PL_Ring",
			"CÊp 5./PL_Ring",
			"CÊp 6./PL_Ring",
			"CÊp 7./PL_Ring",
			"CÊp 8./PL_Ring",
			"CÊp 9./PL_Ring",
			"CÊp 10./PL_Ring",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 3) then	
		local tbSay = {
			"Nam./PL_Amulet",
			"N÷./PL_Amulet",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 4) then	
		local tbSay = {
			"Nam./PL_Boot",
			"N÷./PL_Boot",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 5) then
		local tbSay = {
			"§ai Da./PL_Belt",
			"§ai Kim Lo¹i./PL_Belt",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 6) then	
		local tbSay = {
			"Nam./PL_Helm",
			"N÷./PL_Helm",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 7) then	
		local tbSay = {
			"Nam./PL_Cuff",
			"N÷./PL_Cuff",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	elseif(nSel == 8) then	
		local tbSay = {
			"Nam./PL_Pendant",
			"N÷./PL_Pendant",
			--"Trë vÒ tr­íc./PL_BlueEquip",
			"Tho¸t ra./ExitFunc",
		}
		Say(sInfo,getn(tbSay),tbSay)
	end
end

----------------------------
--
----------------------------
function PL_Weapon(nSel)
	if(nSel < 6) then
		SetTaskTemp(GMRole.Task.EQUIP_DETAIL,0)
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR,nSel)
	else
		SetTaskTemp(GMRole.Task.EQUIP_DETAIL,1)
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR,nSel-6)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_WeaponA",
			"CÊp 2./PL_WeaponA",
			"CÊp 3./PL_WeaponA",
			"CÊp 4./PL_WeaponA",
			"CÊp 5./PL_WeaponA",
			"CÊp 6./PL_WeaponA",
			"CÊp 7./PL_WeaponA",
			"CÊp 8./PL_WeaponA",
			"CÊp 9./PL_WeaponA",
			"CÊp 10./PL_WeaponA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_WeaponA(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_Weapon",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Armor(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"T¨ng Y•/PL_ArmorA",
			"§¹o SÜ•/PL_ArmorA",
			"S¸t thñ•/PL_ArmorA",
			"§Ñp Trai-Xinh G¸i•/PL_ArmorA",
			"Kim Gi¸p•/PL_ArmorA",
			"¡n Mµy•/PL_ArmorA",
			"Long Bµo•/PL_ArmorA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_ArmorA(nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,2)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel + 7)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_ArmorB",
			"CÊp 2./PL_ArmorB",
			"CÊp 3./PL_ArmorB",
			"CÊp 4./PL_ArmorB",
			"CÊp 5./PL_ArmorB",
			"CÊp 6./PL_ArmorB",
			"CÊp 7./PL_ArmorB",
			"CÊp 8./PL_ArmorB",
			"CÊp 9./PL_ArmorB",
			"CÊp 10./PL_ArmorB",
			--"Trë vÒ tr­íc./PL_Armor",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_ArmorB(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_ArmorA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Ring(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,3)
	SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 0)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Amulet(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,4)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 1)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 0)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_AmuletA",
			"CÊp 2./PL_AmuletA",
			"CÊp 3./PL_AmuletA",
			"CÊp 4./PL_AmuletA",
			"CÊp 5./PL_AmuletA",
			"CÊp 6./PL_AmuletA",
			"CÊp 7./PL_AmuletA",
			"CÊp 8./PL_AmuletA",
			"CÊp 9./PL_AmuletA",
			"CÊp 10./PL_AmuletA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_AmuletA(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_Amulet",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Boot(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Giµy Cá./PL_BootA",
			"Giµy Da./PL_BootA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BootA(nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,5)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel + 2)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_BootB",
			"CÊp 2./PL_BootB",
			"CÊp 3./PL_BootB",
			"CÊp 4./PL_BootB",
			"CÊp 5./PL_BootB",
			"CÊp 6./PL_BootB",
			"CÊp 7./PL_BootB",
			"CÊp 8./PL_BootB",
			"CÊp 9./PL_BootB",
			"CÊp 10./PL_BootB",
			--"Trë vÒ tr­íc./PL_Boot",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BootB(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_BootA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Belt(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,6)
	SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_BeltA",
			"CÊp 2./PL_BeltA",
			"CÊp 3./PL_BeltA",
			"CÊp 4./PL_BeltA",
			"CÊp 5./PL_BeltA",
			"CÊp 6./PL_BeltA",
			"CÊp 7./PL_BeltA",
			"CÊp 8./PL_BeltA",
			"CÊp 9./PL_BeltA",
			"CÊp 10./PL_BeltA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BeltA(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_Belt",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Helm(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"T¨ng Y•/PL_HelmA",
			"§¹o SÜ•/PL_HelmA",
			"S¸t thñ•/PL_HelmA",
			"§Ñp Trai-Xinh G¸i•/PL_HelmA",
			"Kim Gi¸p•/PL_HelmA",
			"¡n Mµy•/PL_HelmA",
			"Long Bµo•/PL_HelmA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_HelmA(nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,7)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, nSel + 7)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_HelmB",
			"CÊp 2./PL_HelmB",
			"CÊp 3./PL_HelmB",
			"CÊp 4./PL_HelmB",
			"CÊp 5./PL_HelmB",
			"CÊp 6./PL_HelmB",
			"CÊp 7./PL_HelmB",
			"CÊp 8./PL_HelmB",
			"CÊp 9./PL_HelmB",
			"CÊp 10./PL_HelmB",
			--"Trë vÒ tr­íc./PL_Helm",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_HelmB(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_HelmA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Cuff(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,8)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 1)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 0)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_CuffA",
			"CÊp 2./PL_CuffA",
			"CÊp 3./PL_CuffA",
			"CÊp 4./PL_CuffA",
			"CÊp 5./PL_CuffA",
			"CÊp 6./PL_CuffA",
			"CÊp 7./PL_CuffA",
			"CÊp 8./PL_CuffA",
			"CÊp 9./PL_CuffA",
			"CÊp 10./PL_CuffA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_CuffA(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_Cuff",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Pendant(nSel)
	SetTaskTemp(GMRole.Task.SEX_CHOOSE, nSel)
	local nSex = GetTaskTemp(GMRole.Task.SEX_CHOOSE)
	SetTaskTemp(GMRole.Task.EQUIP_DETAIL,9)
	if(nSex == 0) then
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 1)
	else
		SetTaskTemp(GMRole.Task.EQUIP_PARTICULAR, 0)
	end
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"CÊp 1./PL_PendantA",
			"CÊp 2./PL_PendantA",
			"CÊp 3./PL_PendantA",
			"CÊp 4./PL_PendantA",
			"CÊp 5./PL_PendantA",
			"CÊp 6./PL_PendantA",
			"CÊp 7./PL_PendantA",
			"CÊp 8./PL_PendantA",
			"CÊp 9./PL_PendantA",
			"CÊp 10./PL_PendantA",
			--"Trë vÒ tr­íc./PL_BlueEquipA",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_PendantA(nSel)
	SetTaskTemp(GMRole.Task.EQUIP_LEVEL, nSel + 1)
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"HÖ Kim./PL_AddEquip",
			"HÖ Méc./PL_AddEquip",
			"HÖ Thñy./PL_AddEquip",
			"HÖ Háa./PL_AddEquip",
			"HÖ Thæ./PL_AddEquip",
			--"Trë vÒ tr­íc./PL_Pendant",
			"Tho¸t ra./ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_AddEquip(nSel)
	local nDetail = GetTaskTemp(GMRole.Task.EQUIP_DETAIL)
	local nParti = GetTaskTemp(GMRole.Task.EQUIP_PARTICULAR)
	local nLevel = GetTaskTemp(GMRole.Task.EQUIP_LEVEL)
	for i=1,10 do
		if(FindEmptyPlace(2,4) == 0) then
			Msg2Player("<color=green>Hµnh trang kh«ng ®ñ chç trèng!<color>")
			break
		end
		AddItem(0,nDetail,nParti,nLevel,nSel,10,10)
	end
end 

----------------------------
--
----------------------------
function PL_SelHorse(nPage)
	if(type(nPage) ~= "number") then return end
	
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"¤ V©n §¹p TuyÕt•/PL_HorseA",
			"XÝch Thè/PL_HorseA",
			"TuyÖt ¶nh/PL_HorseA",
			"§Ých L«/PL_HorseA",
			"ChiÕu D¹ Ngäc S­ Tö/PL_HorseA",
			"B«n Tiªu/PL_HorseA",
			"Phiªn Vò/PL_HorseA",
			"Phi V©n•/PL_HorseC",
			"XÝch Long C©u/PL_HorseC",
			"TuyÖt §Þa/PL_HorseC",
			"Du Huy/PL_HorseC",
			"§»ng Vô/PL_HorseC",
			"Siªu Quang/PL_HorseC",
			"•Háa Tinh Kim Hæ V­¬ng•/GM_Horse_5",
			"•Kim Tinh B¹ch Hæ V­¬ng•/GM_Horse_5",
			"•Long Tinh H¾c Hæ V­¬ng•/GM_Horse_5",
			"•H·n HuyÕt Long C©u•/GM_Horse_5",
			"•Phong V©n B¹ch M·•/GM_Horse_5",
			"•Phong V©n ChiÕn M·•/GM_Horse_5",
			"•Phong V©n ThÇn M·•/GM_Horse_7",
			"•S­ tö•/GM_Horse_7",
			"•L¹c ®µ•/GM_Horse_7",
			"•D­¬ng §µ•/GM_Horse_7",
			"•TuÇn Léc•/GM_Horse_7",
			"•D­¬ng Sa•/GM_Horse_7",
			
			"Trang kÕ/PL_HorseB",
			-- "Trë vÒ/WPlayer_Equip",
			"Tho¸t/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)	
end

----------------------------
--
----------------------------
function PL_HorseA(nSel)
	local nParti = 0
	local nLevel = 0
	local nSeries = GetSeries()
	if(nSel < 5) then
		nParti = 5
		nLevel = nSel + 6
	else
		nParti = nSel + 1
		nLevel = 10
	end
	if(FindEmptyPlace(2,3) == 0) then
		Msg2Player("<color=green>Hµnh trang kh«ng ®ñ chç trèng!<color>")
		return
	end
	AddItem(0,10,nParti,nLevel,nSeries,0,0)
end

----------------------------
--
----------------------------
function PL_HorseB()
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Phi V©n•/PL_HorseC",
			"XÝch Long C©u/PL_HorseC",
			"TuyÖt §Þa/PL_HorseC",
			"Du Huy/PL_HorseC",
			"§»ng Vô/PL_HorseC",
			"Siªu Quang/PL_HorseC",
			"Trang tr­íc/PL_Horse",
			"Trang kÕ/PL_HorseD",
			"Tho¸t•/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)	
end

----------------------------
--
----------------------------
function PL_HorseC(nSel)
	local nParti = 0
	local nLevel = 0
	local nSeries = GetSeries()
	nParti = nSel + 8
	nLevel = 10
	AddItem(0,10,nParti,nLevel,nSeries,0,0)
end

----------------------------
--
----------------------------
function GM_Horse_4()
	local sInfo = "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {
			"•Háa Tinh Kim Hæ V­¬ng•/GM_Horse_5",
			"•Kim Tinh B¹ch Hæ V­¬ng•/GM_Horse_5",
			"•Long Tinh H¾c Hæ V­¬ng•/GM_Horse_5",
			"•H·n HuyÕt Long C©u•/GM_Horse_5",
			"•Phong V©n B¹ch M·•/GM_Horse_5",
			"•Phong V©n ChiÕn M·•/GM_Horse_5",
			"•Trang tr­íc•/GM_Horse_2",
			"•Trang kÕ•/GM_Horse_6",
			"•Trë vÒ•/GM_Equip",
			"•Tho¸t•/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)	
end

----------------------------
--
----------------------------
function GM_Horse_5(nSel)
	local nParti = 0
	local nLevel = 0
	local nSeries = GetSeries()
	nParti = nSel + 15
	nLevel = 10
	AddItem(0,10,nParti,nLevel,nSeries,0,0)
end

----------------------------
--
----------------------------
function GM_Horse_6()
	local sInfo = "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {
			"•Phong V©n ThÇn M·•/GM_Horse_7",
			"•S­ tö•/GM_Horse_7",
			"•L¹c ®µ•/GM_Horse_7",
			"•D­¬ng §µ•/GM_Horse_7",
			"•TuÇn Léc•/GM_Horse_7",
			"•D­¬ng Sa•/GM_Horse_7",
			"•Trang tr­íc•/GM_Horse_4",
			"•Trang kÕ•/GM_Horse_8",
			"•Trë vÒ•/GM_Equip",
			"•Tho¸t•/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)	
end

----------------------------
--
----------------------------
function GM_Horse_7(nSel)
	local nParti = 0
	local nLevel = 0
	local nSeries = GetSeries()
	nParti = nSel + 21
	nLevel = 10
	AddItem(0,10,nParti,nLevel,nSeries,0,0)
end

----------------------------
--
----------------------------
function GM_Horse_8()
	local sInfo = "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {
			"•Ngù Phong•/GM_Horse_9",
			"•Truy ®iÖn•/GM_Horse_9",
			"•L­u Tinh•/GM_Horse_9",
			"•Trang tr­íc•/GM_Horse_6",
			"•Trë vÒ•/GM_Equip",
			"•Tho¸t•/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)	
end

----------------------------
--
----------------------------
function GM_Horse_9(nSel)
	local nParti = 0
	local nLevel = 0
	local nSeries = GetSeries()
	nParti = nSel + 27
	nLevel = 10
	AddItem(0,10,nParti,nLevel,nSeries,0,0)
end


----------------------------
--
----------------------------
function PL_Mask()

		AddItem(0,11,0,1,0,0,0)
		AddItem(0,11,0,2,0,0,0)
		AddItem(0,11,0,3,0,0,0)
		AddItem(0,11,0,4,0,0,0)
	
end 

----------------------------
--
----------------------------
function PL_BasicGoldEquip()
	local sInfo = " Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"Kim phong./PL_BasicGoldEquipA",
			"An Bang./PL_BasicGoldEquipA",
			"§Þnh Quèc./PL_BasicGoldEquipA",
			"Thiªn Hoµng./PL_BasicGoldEquipA",
			"§éng S¸t./ExitFunc",
			"Hång ¶nh./PL_BasicGoldEquipA",
			"HiÖp Cèt./PL_BasicGoldEquipA",
			"Nhu T×nh./PL_BasicGoldEquipA",
			"An Bang [Hoµn Mü]./PL_BasicGoldEquipA",
			-- "Trë vÒ tr­íc./WPlayer_Equip",
			"Tho¸t./ExitFunc"
		}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_BasicGoldEquipA(nSel)

	if(nSel == 0) then
		for i=177, 185 do
			AddGoldItem(i)
		end
	elseif(nSel == 1) then
		for i=216,219 do
			AddGoldItem(i)
		end
	elseif(nSel == 2) then
		for i=398, 402 do
			AddGoldItem(i)
		end
	elseif(nSel == 3) then
		for i=168, 176 do
			AddGoldItem(i)
		end
	elseif(nSel == 4) then
		for i=143, 146 do
			AddGoldItem(i)
		end
	elseif(nSel == 5) then
		for i=204, 207 do
			AddGoldItem(i)
		end
	elseif(nSel == 6) then
		for i=186, 189 do
			AddGoldItem(i)
		end
	elseif(nSel == 7) then
		for i=190, 193 do
			AddGoldItem(i)
		end
	elseif(nSel == 8) then
		for i=210, 213 do
			AddGoldItem(i)
		end
	end
end

-- do hang kim -
function PL_PremiumGoldEquip()
	Say("<color=metal>¡n Mµy<color>: ¸i chµ! L©u l¾m råi míi thÊy Admin ghÐ th¨m kÎ hÌn nµy. T×nh h×nh server cã g× bÊt tr¾c sao?",11,
				"§å HKMP ThiÕu L©m/hkmp6",
				"§å HKMP Thiªn V­¬ng/hkmp1",
				"§å HKMP Ngò §éc/hkmp4",
				"§å HKMP §­êng M«n/hkmp5",
				"§å HKMP Nga My/hkmp2",
				"§å HKMP Thóy Yªn/hkmp3",
				"§å HKMP Thiªn NhÉn/hkmp7",
				"§å HKMP C¸i Bang/hkmp8",
				"§å HKMP Vâ §ang/hkmp9",
				"§å HKMP C«n L«n/hkmp10",
				-- "Trë vÒ/WPlayer_Equip",
				"Tho¸t ra/no")
end

function hkmp5()
Say("Xin chµo <color=wood>"..GetName().."<color>!",5,
"§­êng M«n B¨ng Hµn/dm1",
"§­êng M«n TiÓu Lý/dm2",
"§­êng M«n Bao Vò/dm3",
"§­êng M«n BÉy/dm4",
"KÕt thóc ®æi tho¹i/no")
end


function dm1()
	for i=71,75 do
		AddGoldItem(i)
	end
end

function dm2()
	for i=81,85 do
		AddGoldItem(i)
	end
end

function dm3()
	for i=76,80 do
		AddGoldItem(i)
	end
end

function dm4()
	for i=86,90 do
		AddGoldItem(i)
	end
end

function hkmp4()
Say("Xin chµo <color=wood>"..GetName().."<color>!",4,
"Ngò §éc §ao/nd1",
"Ngò §éc Ch­ëng /nd2",
"Ngò §éc Bïa /nd3",
"KÕt thóc ®æi tho¹i/no")
end


function nd1()
	for i=61,65 do
		AddGoldItem(i)
	end
end

function nd2()
	for i=56,60 do
		AddGoldItem(i)
	end
end

function nd3()
	for i=66,70 do
		AddGoldItem(i)
	end
end


function hkmp3()
Say("Xin chµo <color=wood>"..GetName().."<color>!",3,
"Thóy Yªn §ao/ty1",
"Thóy Yªn S«ng §ao/ty2",
"KÕt thóc ®æi tho¹i/no")
end


function ty1()
	for i=46,50 do
		AddGoldItem(i)
	end
end

function ty2()
	for i=51,55 do
		AddGoldItem(i)
	end
end




function hkmp2()
Say("Xin chµo <color=wood>"..GetName().."<color>!",4,
"Nga My KiÕm/nm1",
"Nga My Ch­ëng /nm2",
"Nga My Buff /nm3",
"KÕt thóc ®æi tho¹i/no")
end


function nm1()
	for i=31,35 do
		AddGoldItem(i)
	end
end

function nm2()
	for i=36,40 do
		AddGoldItem(i)
	end
end

function nm3()
	for i=41,45 do
		AddGoldItem(i)
	end
end

function hkmp1()
Say("Xin chµo <color=wood>"..GetName().."<color>!",4,
"Thiªn V­¬ng Chïy/tv1",
"Thiªn V­¬ng Th­¬ng /tv2",
"Thiªn V­¬ng §ao /tv3",
"KÕt thóc ®æi tho¹i/no")
end


function tv1()
	for i=16,20 do
		AddGoldItem(i)
	end
end

function tv2()
	for i=21,25 do
		AddGoldItem(i)
	end
end

function tv3()
	for i=26,30 do
		AddGoldItem(i)
	end
end

function hkmp6()
Say("Xin chµo <color=wood>"..GetName().."<color>!",4,
"ThiÕu L©m QuyÒn/tl1",
"ThiÕu L©m Bæng /tl2",
"ThiÕu L©m §ao /tl3",
"KÕt thóc ®æi tho¹i/no")
end


function tl1()
	for i=1,5 do
		AddGoldItem(i)
	end
end

function tl2()
	for i=6,10 do
		AddGoldItem(i)
	end
end

function tl3()
	for i=11,15 do
		AddGoldItem(i)
	end
end

function hkmp7()
Say("Xin chµo <color=wood>"..GetName().."<color>!",5,
"Thiªn NhÉn S¸t/boTN1",
"Thiªn NhÉn Hoµng/boTN2",
"Thiªn NhÉn ThÞ/boTN3",
"KÕt thóc ®æi tho¹i/no")
end


function boTN1()
for i=101,105 do
		AddGoldItem(i)
	end
end

function boTN2()
for i=106,110 do
		AddGoldItem(i)
	end
end

function boTN3()
for i=111,115 do
		AddGoldItem(i)
	end
end

function hkmp8()
Say("Xin chµo <color=wood>"..GetName().."<color>!",3,
"C¸i Bang Rång/boCB1",
"C¸i Bang Bæng/boCB2",
"KÕt thóc ®æi tho¹i/no")
end


function boCB1()
for i=91,95 do
		AddGoldItem(i)
	end
end

function boCB2()
for i=96,100 do
		AddGoldItem(i)
	end
end

function hkmp9()
Say("Xin chµo <color=wood>"..GetName().."<color>!",3,
"Vâ §ang KhÝ/boVD1",
"Vâ §ang KiÕm/boVD2",
"KÕt thóc ®æi tho¹i/no")
end

function boVD1()
for i=116,120 do
		AddGoldItem(i)
	end
end

function boVD2()
for i=121,125 do
		AddGoldItem(i)
	end
end


function hkmp10()
Say("Xin chµo <color=wood>"..GetName().."<color>!",4,
"C«n L«n §ao/boCL1",
"Con L«n KiÕm/boCL2",
"C«n L«n Buff/boCL3",
"KÕt thóc ®æi tho¹i/no")
end

function boCL1()
for i=126,130 do
		AddGoldItem(i)
	end
end

function boCL2()
for i=131,135 do
		AddGoldItem(i)
	end
end

function boCL3()
for i=136,140 do
		AddGoldItem(i)
	end
end

function PL_FiFong()
	local szHello = "Cã 43 lo¹i phi phong, ®¹i hiÖp muèn chän lo¹i nµo?"
	local szSay = {
		"Lo¹i mét/Excuter_FiFong#1",
		"Lo¹i hai/Excuter_FiFong#2",
		"Lo¹i ba/Excuter_FiFong#3",
		"Lo¹i bèn/Excuter_FiFong#4",
		"Lo¹i n¨m/Excuter_FiFong#5",
		"Tho¸t/ExitFunc"
	};
	Say(szHello,getn(szSay),szSay);
end;

function Excuter_FiFong(nSel,nLine)
	local nLine = tonumber(nLine);
	local nMin = 1;
	local nMax = 10;
	local nLevel = 0;
	if (nLine == 1) then
		nMin = 1;
		nMax = 10;
		nLevel = 0;
	elseif(nLine == 2)then	
		nMin = 1;
		nMax = 10;
		nLevel = 1;
	elseif(nLine == 3)then
		nMin = 1;
		nMax = 10;
		nLevel = 2;
	elseif(nLine == 4)then
		nMin = 1;
		nMax = 10;
		nLevel = 3;
	else
		nMin = 1;
		nMax = 5;
		nLevel = 3;
	end;	

	for i = nMin,nMax do
		AddItem(0,12,nLevel,i,0,0,0);
	end;
	Msg2Player("§¹i hiÖp nhËn ®­îc "..nMax.." phi phong.");
end;

function ExitFunc()
end;

function no()
end;