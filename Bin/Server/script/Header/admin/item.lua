----------------------------------
--	Copyright: JX Online by Kinnox Team
--	Author: Kinnox
--	Date: 06/06/2014
--	Desc: Script GM Item
----------------------------------

----------------------------
--
----------------------------
function GM_Item()
	local sInfo =  "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!"
	local tbSay = {
			"[1] - NhËn Questkey th­êng/I_QK_Normal",
			"[2] - NhËn ng©n l­îng/GM_Money",
			"[3] - Trë vÒ/GM_Choose",
			"[4] - Tho¸t/ExitFunc",
		}
	SayImage(sInfo,"44/44/15",getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function I_QK_Normal()
	AskClientForNumber("R_I_QK_Normal" ,0,1316,"NhËp ID.")
end
function GM_Money()
	AskClientForNumber("GM_Money1" ,0,999999999,"NhËp sè tiÒn.")
end	
function GM_Money1(num)
Earn(num)
end
----------------------------
--
----------------------------
function R_I_QK_Normal(nId)
	AddEventItem(tonumber(nId))
end