----------------------------------
--	Copyright: JX Online by kinnox
--	Author: kinnox
--	Date: 15/11/2021
--	Desc: Script Get Horse
----------------------------------

ZHORSE = {
	[1] = {
		{"LiÖt Hoµng M· ",0,1}, 
		{"LiÖt Hoµng M· ",0,2},
		{"Hoµng M· ",0,3},
		{"Hoµng M· ",0,4},
		{"Hoµng Phiªu",0,5},
		{"Hoµng Phiªu",0,6},
		{"§¹i UyÓn Hoµng M· ",0,7}, 
		{"§¹i UyÓn Hoµng M· ",0,8},
		{"Phi Hoµng",0,9},
		{"Phi Hoµng",0,10},
	},
	[2] = {
		{"LiÖt Thanh M· ",1,1}, 
		{"LiÖt Thanh M· ",1,2},
		{"Thanh Th«ng",1,3},
		{"Thanh Th«ng",1,4},
		{"Tö L­u",1,5},
		{"Tö L­u",1,6},
		{"§¹i UyÓn Thanh M· ",1,7}, 
		{"§¹i UyÓn Thanh M· ",1,8},
		{"Hoa L­u",1,9},
		{"Hoa L­u",1,10},
	},
	[3] = {
		{"LiÖt B¹ch M· ",2,1}, 
		{"LiÖt B¹ch M· ",2,2},
		{"B¹ch M· ",2,3},
		{"B¹ch M· ",2,4},
		{"Ngäc Hoa Th«ng",2,5},
		{"Ngäc Hoa Th«ng",2,6},
		{"§¹i UyÓn B¹ch M· ",2,7}, 
		{"§¹i UyÓn B¹ch M· ",2,8},
		{"Tóc S­¬ng",2,9},
		{"Tóc S­¬ng",2,10},
	},
	[4] = {
		{"LiÖt H¾c M· ",3,1}, 
		{"LiÖt H¾c M· ",3,2},
		{"H¾c M· ",3,3},
		{"H¾c M· ",3,4},
		{"H¾c Kú ",3,5},
		{"H¾c Kú ",3,6},
		{"§¹i UyÓn H¾c M· ",3,7}, 
		{"§¹i UyÓn H¾c M· ",3,8},
		{"¤ Chïy",3,9},
		{"¤ Chïy",3,10},
	},
	[5] = {
		{"LiÖt Hång M· ",4,1}, 
		{"LiÖt Hång M· ",4,2},
		{"Hång M· ",4,3},
		{"Hång M· ",4,4},
		{"Hång Ly ",4,5},
		{"Hång Ly ",4,6},
		{"§¹i UyÓn Hång M· ",4,7}, 
		{"§¹i UyÓn Hång M· ",4,8},
		{"XÝch Ký",4,9},
		{"XÝch Ký",4,10},
	},
	[6] = {
		{"¤ V©n §¹p TuyÕt",5,6}, 
		{"XÝch Thè ",5,7},
		{"TuyÖt ¶nh",5,8},
		{"§Ých L« ",5,9},
		{"ChiÕu D¹ Ngäc S­ Tö ",5,10},
	},
	[7] = {
		{"B«n Tiªu",6,10}, 
		{"Phiªn Vò ",7,10},
		{"Phi V©n",8,10},
		{"XÝch Long C©u",9,10},
		{"Siªu Quang",13,10},
		{"Kim Tinh Hæ V­¬ng",14,10}, 
		{"Háa Tinh Kim Hæ V­¬ng",15,10},
		{"Kim Tinh B¹ch Hæ V­¬ng",16,10},
		{"Long Tinh H¾c Hæ V­¬ng",17,10},
		{"H·n HuyÕt Long C©u",18,10},
	},
	[8] = {
		{"S­ tö ",22,10},
		{"L¹c ®µ ",23,10},
		{"D­¬ng §µ ",24,10},
		{"TuÇn Léc",25,10},
		{"D­¬ng Sa ",26,10},
		{"Ngù Phong",27,10},
		{"Truy ®iÖn",28,10},
		{"L­u Tinh",29,10},
	},
}

----------------------------
--
----------------------------
function GM_GetHorse()
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
		"Hoµng M·./GM_GetHorseA",
		"Thanh M·./GM_GetHorseA",
		"B¹ch M·./GM_GetHorseA",
		"H¾c M·./GM_GetHorseA",
		"Hång M·./GM_GetHorseA",
		"ChiÕn M· 80./GM_GetHorseA",
		"ChiÕn M· Cao CÊp./GM_GetHorseA",
		"ChiÕn M· Cao CÊp 2./GM_GetHorseA",
		"KÕt thóc ®èi tho¹i./ExitFunc",
	}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GM_GetHorseA(nType)
	if(type(nType) ~= "number") then return end
	
	nType = nType + 1
	if( (nType < 1) or (nType > getn(ZHORSE))) then return end
	
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {}
	for i = 1, getn(ZHORSE[nType]) do
		tinsert(tbSay, ZHORSE[nType][i][1].."/GM_AddHorse#"..nType)
	end
	tinsert(tbSay, "Trë vÒ./GM_GetHorse")
	tinsert(tbSay, "KÕt thóc ®èi tho¹i./ExitFunc")
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GM_AddHorse(nItem, nType)	
	nType = tonumber(nType)
	if(type(nType) ~= "number") then return end
	if( (nType < 1) or (nType > getn(ZHORSE))) then return end
	
	if(type(nItem) ~= "number") then return end
	nItem = nItem + 1
	if( (nItem < 1) or (nItem > getn(ZHORSE[nType]))) then return end
	
	if(FindEmptyPlace(2,4) == 0) then
		Msg2Player("<color=green>H·y s¾p xÕp 2x4 « trèng ®Ó nhËn ChiÕn M·!<color>")
		return
	end	
	AddItem(0,10,ZHORSE[nType][nItem][2],ZHORSE[nType][nItem][3],GetSeries(),0,0)
end


----------------------------
--
----------------------------
function ExitFunc()

end;
















