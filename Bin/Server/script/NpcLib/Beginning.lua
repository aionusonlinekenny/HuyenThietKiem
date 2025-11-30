-----------------------------
--  Copyright: CLJ Team
--  Author: Jocker
--  Date: 9/2017
--	Desc: load maps npc
------------------------------
Include("\\script\\npclib\\begin_head.lua")
------------------------------
--Load Npc In Maps
------------------------------
function Begin_Init()
	JPreLoadMap:OnLoadMaps(53) -- ba lang huyen -- demo code ad npc
	JPreLoadMap:OnLoadMaps(78) -- tuong duong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(1) -- phuong tuong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(37) -- bien kinh -- demo code ad npc
	JPreLoadMap:OnLoadMaps(11) -- thanh do -- demo code ad npc
	JPreLoadMap:OnLoadMaps(162) -- dai ly -- demo code ad npc
	JPreLoadMap:OnLoadMaps(80) -- Duong chau -- demo code ad npc
	JPreLoadMap:OnLoadMaps(176) -- lam an -- demo code ad npc
	JPreLoadMap:OnLoadMaps(2) -- hoa son -- demo code ad npc
	JPreLoadMap:OnLoadMaps(3) -- kiem cac tay bac -- demo code ad npc
	JPreLoadMap:OnLoadMaps(70) -- vu lang son -- demo code ad npc
	JPreLoadMap:OnLoadMaps(4) -- kim quang dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(74) -- mieu linh -- demo code ad npc
	JPreLoadMap:OnLoadMaps(90) -- phuc nguu son dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(6) -- toa van dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(41) -- phuc nguu son tay -- demo code ad npc
	JPreLoadMap:OnLoadMaps(91) -- me cung ke quan dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(38) -- thiet thap tang 1 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(39) -- thiet thap tang 2 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(42) -- thien tam dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(79) -- mat dao nha mon -- demo code ad npc
	JPreLoadMap:OnLoadMaps(158) -- me cung cam dia -- demo code ad npc
	JPreLoadMap:OnLoadMaps(166) -- thiet thap tang 3 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(72) -- dai tu dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(142) -- duoc vuong dong 1 -> 2 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(319) -- lam du quan -- demo code ad npc
	JPreLoadMap:OnLoadMaps(75) -- khoa lang dong  -- demo code ad npc
	JPreLoadMap:OnLoadMaps(224) -- sa mac dia bieu -- demo code ad npc
	JPreLoadMap:OnLoadMaps(320) -- chan nui truong bach -- demo code ad npc
	JPreLoadMap:OnLoadMaps(225) -- sa mac 1 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(226) -- sa mac 2 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(227) -- sa mac 3 -- demo code ad npc
	JPreLoadMap:OnLoadMaps(321) -- truong bach nam -- demo code ad npc
	JPreLoadMap:OnLoadMaps(322) -- truong bach bac -- demo code ad npc
	JPreLoadMap:OnLoadMaps(336) -- phong lang do -- demo code ad npc
	JPreLoadMap:OnLoadMaps(340) -- mac cao quat -- demo code ad npc
	JPreLoadMap:OnLoadMaps(208) -- Prison -- demo code ad npc
	JPreLoadMap:OnLoadMaps(396) -- lien dau -- demo code ad npc
	JPreLoadMap:OnLoadMaps(325) -- tong kim -- demo code ad npc
	JPreLoadMap:OnLoadMaps(380) -- tong kim  -- demo code ad npc
	JPreLoadMap:OnLoadMaps(242) -- dao tay tuy -- demo code ad npc
	JPreLoadMap:OnLoadMaps(342) -- tich tinh dong -- demo code ad npc
	JPreLoadMap:OnLoadMaps(394) -- thien bao kho -- demo code ad npc
	JPreLoadMap:OnLoadMaps(21) -- thien bao kho -- demo code ad npc
	JPreLoadMap:LoadOldNpc() -- thien bao kho -- demo code ad npc
	JPreLoadMap:KillerBoss()-- boss sat thu -- demo code ad npc
end

