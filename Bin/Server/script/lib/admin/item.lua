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
	local sInfo =  "<color=red>H÷ ThËng<color>: Xin mÍi<color=green> "..GetName().." <color>l˘a ch‰n!GÓi ˝:\nTh«n hµnh phÔ[ID:20] --- LB Phong l®ng ÆÈ[ID:12]\nSTG[ID:7->11]"
	local tbSay = {
			"NhÀn Task Item/GMTask",
			"NhÀn Script Item/GMScript",
			"NhÀn Gold Item/GMGold",
			"NhÀn Ng˘a/GMHouse",
			"NhÀn Item/GMItem",
			"NhÀn ng©n l≠Óng/GMMoney",
			"NhÀn Huy“n Tinh/NhanHuyenTinh",
			"NhÀn Kho∏ng Thπch/Nhankhoangthach",
			"NhÀn xu/GMCoin",
			"Tho∏t/ExitFunc",
		}
	Say(sInfo,getn(tbSay),tbSay)
end
function GMHouse()
	AskClientForNumber("AddHouseItems" ,0,9999,"NhÀp ID.")
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
	AskClientForNumber("AddGoldItems" ,0,9999,"NhÀp ID.")
	end;
	
function AddGoldItems(nIndex,nID)
	AddGoldItem(nID);
	Msg2Player("C∏c hπ nhÀn Æ≠Óc vÀt ph»m TaskItem "..nID.."");
end;
function GMTask()
AskClientForNumber("AddTaskItems" ,0,999,"NhÀp ID.")
end;

function AddTaskItems(nIndex,nID)
AddTaskItem(nID);
Msg2Player("C∏c hπ nhÀn Æ≠Óc vÀt ph»m TaskItem "..nID.."");
end;
function GMItem()
	AskClientForNumber("AddItems" ,0,999,"NhÀp ID.")
	end;
	
function AddItems(nIndex,nID)
	local i = nID;
	AddItem(0,2,i,10,0,0,0)
	Msg2Player("C∏c hπ nhÀn Æ≠Óc vÀt ph»m "..nID.."");
end;
function GMScript()
AskClientForNumber("AddScriptItems" ,0,999,"NhÀp ID.")
end;
function NhanHuyenTinh()
for i=0, 20 do
AddTaskItem(77);
end
end;
function Nhankhoangthach()
for i=146, 151 do
AddScriptItem(i);
end
end
function AddScriptItems(nIndex,nID)
AddScriptItem(nID);
Msg2Player("C∏c hπ nhÀn Æ≠Óc vÀt ph»m ScriptsItem");
end;

function ExitFunc()
end