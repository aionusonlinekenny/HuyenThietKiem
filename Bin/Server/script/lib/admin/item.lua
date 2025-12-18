----------------------------------
--	Copyright: JxOnline by kinnox;
--	Author: kinnox
--	Date: 15/11/2021
--	Desc: Script GM Item
----------------------------------

----------------------------
--
----------------------------
function GM_Item()
	local sInfo =  "<color=red>HÖ Thèng<color>: Xin mêi<color=green> "..GetName().." <color>lùa chän!Gîi ý:\nThÇn hµnh phï[ID:20] --- LB Phong l¨ng ®é[ID:12]\nSTG[ID:7->11]"
	local tbSay = {
			"NhËn Task Item/GMTask",
			"NhËn Script Item/GMScript",
			"NhËn Gold Item/GMGold",
			"NhËn Ngùa/GMHouse",
			"NhËn Item/GMItem",
			"NhËn ng©n l­îng/GMMoney",
			"NhËn xu/GMCoin",
			"Tho¸t/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end
function GMHouse()
	AskClientForNumber("AddHouseItems" ,0,9999,"NhËp ID.")
end
function AddHouseItems(nIndex,nID)
	AddItem(0,10,nID,10,5,0,0,0);
end
function GMCoin()
	for j=0,500 do
	AddTaskItem(19);
	end
end	
function GMMoney()
	Earn(2000000)
end	
function GMGold()
	AskClientForNumber("AddGoldItems" ,0,9999,"NhËp ID.")
	end;
	
function AddGoldItems(nIndex,nID)
	AddGoldItem(nID);
	Msg2Player("C¸c h¹ nhËn ®­îc vËt phÈm TaskItem "..nID.."");
end;
function GMTask()
AskClientForNumber("AddTaskItems" ,0,999,"NhËp ID.")
end;

function AddTaskItems(nIndex,nID)
AddTaskItem(nID);
Msg2Player("C¸c h¹ nhËn ®­îc vËt phÈm TaskItem "..nID.."");
end;
function GMItem()
	AskClientForNumber("AddItems" ,0,999,"NhËp ID.")
	end;
	
function AddItems(nIndex,nID)
	local i = nID;
	AddItem(0,2,i,10,0,0,0)
	Msg2Player("C¸c h¹ nhËn ®­îc vËt phÈm "..nID.."");
end;
function GMScript()
AskClientForNumber("AddScriptItems" ,0,999,"NhËp ID.")
end;

function AddScriptItems(nIndex,nID)
AddScriptItem(nID);
Msg2Player("C¸c h¹ nhËn ®­îc vËt phÈm ScriptsItem");
end;

function ExitFunc()
end