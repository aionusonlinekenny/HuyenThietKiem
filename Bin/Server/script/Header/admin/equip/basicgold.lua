----------------------------------
--	Copyright: JX Online by Kinnox Team
--	Author: Kinnox
--	Date: 05/11/2015
--	Desc: Script Get Basic Gold
----------------------------------

ZBASICGOLD = {
	[1] = {
		{"Kim Phong",1,9}, 
		{"HiÖp Cèt",10,13},
		{"Nhu T×nh",14,17},
		{"Thiªn Hoµng",18,26},
		{"An Bang",27,30},
		{"§Þnh Quèc",31,35},
		{"§éng S¸t",36,39}, 
		{"Hång ¶nh",40,43},
		{"V« Danh",53,54},
	},
}

----------------------------
--
----------------------------
function GM_GetBasicGold()
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
		"Hoµng Kim Th­êng./GM_GetBasicGoldA",
		"Trë vÒ./GMTools",
		"KÕt thóc ®èi tho¹i./ExitFunc",
	}
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GM_GetBasicGoldA(nType)
	if(type(nType) ~= "number") then return end
	
	nType = nType + 1
	if( (nType < 1) or (nType > getn(ZBASICGOLD))) then return end
	
	local sInfo = "<color=gold>HÖ Thèng<color>: Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {}
	for i = 1, getn(ZBASICGOLD[nType]) do
		tinsert(tbSay, ZBASICGOLD[nType][i][1].."/GM_AddBasicGold#"..nType)
	end
	tinsert(tbSay, "Trë vÒ./GM_GetBasicGold")
	tinsert(tbSay, "KÕt thóc ®èi tho¹i./ExitFunc")
	Say(sInfo,getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function GM_AddBasicGold(nItem, nType)	
	nType = tonumber(nType)
	if(type(nType) ~= "number") then return end
	if( (nType < 1) or (nType > getn(ZBASICGOLD))) then return end
	
	if(type(nItem) ~= "number") then return end
	nItem = nItem + 1
	if( (nItem < 1) or (nItem > getn(ZBASICGOLD[nType]))) then return end
	
	for i = ZBASICGOLD[nType][nItem][2], ZBASICGOLD[nType][nItem][3] do
		AddGoldItem(i)
	end
end



















