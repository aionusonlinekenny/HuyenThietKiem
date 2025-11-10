
-- dinh thao item
----------------------------------
--
----------------------------------
function OnBind(nIndex)
	if(GetFightState() == 1) then
		return
	end
	local nItemIdx = nIndex
	if(nItemIdx == 0 or nItemIdx == nil) then
		return
	end
	local sInfo = "Khi trang bÞ ®­îc khãa th× kh«ng thÓ giao dÞch, bµy b¸n, vøt bá hoÆc ®Æt vµo hép ®èi tho¹i. §ång thêi kh«ng cho phÐp chÕ t¹o, kh¶m n¹m, ®Æc biÖt lµ khi PK sÏ kh«ng bÞ r¬i ra. Trang bÞ ®­îc khãa b¶o hiÓm vÉn cã t¸c dông nh­ b×nh th­êng, <color=yellow>më khãa b¶o hiÓm<color> b»ng nót 'Th¸o' trªn giao diÖn trang bÞ. Tuy nhiªn sau <color=green>48 giê <color>míi cã thÓ hoµn toµn ®­îc më khãa."
	local tbSay = {
				"Ta muèn khãa b¶o hiÓm trang bÞ nµy./AcceptBind#"..nItemIdx.."",
				"§Ó ta xem l¹i./ExitFunc",
			}
	Say(sInfo, getn(tbSay), tbSay)
end

----------------------------
-- ChÊp nhËn ®Ýnh
----------------------------
function AcceptBind(nSel, nIndex)
	local nItemIdx = tonumber(nIndex)
	if(nItemIdx == 0 or nItemIdx == nil) then
		return
	end
	local nCheck = GetItemBindState(nItemIdx)
	local nG, nD, nP = GetItemProp(nItemIdx)
	if(nG < 0) then
		return
	end
	local tbMsg = {
			"VËt phÈm nµy kh«ng cÇn thiÕt khãa b¶o hiÓm",
			"VËt phÈm nµy kh«ng thÓ khãa b¶o hiÓm",
		}
	if(nG == 5) then
		Say(tbMsg[1],0)
		return
	end
	if(nG == 6 and nP == 0) then
		Say(tbMsg[1],0)
		return
	end
	if(nCheck == 1 or nCheck == -1) then
		Say(tbMsg[2],0)
		return
	end
	SetItemBindState(nItemIdx, 1)
end

----------------------------------
--
----------------------------------
function OnUnBind(nIndex)
	if(GetFightState() == 1) then
		return
	end
	local nItemIdx = nIndex
	if(nItemIdx == 0 or nItemIdx == nil) then
		return
	end
	local sInfo = "Sau khi nhÊp nót 'Th¸o' ®Ó më khãa b¶o hiÓm, cÇn ph¶i ®îi <color=green>48 giê <color>míi cã thÓ hoµn toµn ®­îc më."
	local tbSay = {
				"Ta ®ång ý më khãa b¶o hiÓm./AcceptUnBind#"..nItemIdx.."",
				"§Ó ta xem l¹i./ExitFunc",
			}
	Say(sInfo, getn(tbSay), tbSay)
end

----------------------------
-- ChÊp nhËn th¸o
----------------------------
function AcceptUnBind(nSel, nIndex)
	local nItemIdx = tonumber(nIndex)
	if(nItemIdx == 0 or nItemIdx == nil) then
		return
	end
	local nCheck = GetItemBindState(nItemIdx)
	local nG, nD, nP = GetItemProp(nItemIdx)
	if(nG < 0) then
		return
	end
	local sMsg = "VËt phÈm nµy kh«ng thÓ më khãa b¶o hiÓm"
	if(nG == 5) then
		Say(sMsg,0)
		return
	end
	if(nG == 6 and nP == 0) then
		Say(sMsg,0)
		return
	end
	if(nCheck == 0 or nCheck == -1 or nCheck == 2) then
		Say(sMsg,0)
		return
	end
	SetItemBindState(nItemIdx,1, 7)
end

----------------------------
--
----------------------------
function ExitFunc()
end