----------------------------------
--	Copyright: AJX Team
--	Author: [N]
--	Date: 16/08/2014
--	Desc: Script Support Point
----------------------------------

----------------------------
--
----------------------------
function WPlayer_Point()
	local sInfo = "Xin mêi <color=fire>[ "..GetName().." ]<color> tr¶i nghiÖm"
	local tbSay = {
			"§¼ng cÊp 150./PL_Level",
			"Th¨ng 10 cÊp [Tèi ®a 150]./PL_Exp",
			"1000 v¹n l­îng./PL_Cash",
			"1 v¹n xu./PL_Coin",
			"Tµi l·nh ®¹o./PL_LeadLevel",
			"Danh väng./PL_Repute",
			"Trë vÒ tr­íc./main",
			"Tho¸t ra./ExitFunc",
		}
	SayImage(sInfo,"-85/-85/68",getn(tbSay),tbSay)
end

----------------------------
--
----------------------------
function PL_Level()
	if(GetLevel() >= 150) then
		Talk(1,"","§¼ng cÊp ®· ®¹t 150")
		return
	end
	SetLevel(150)
end

----------------------------
--
----------------------------
function PL_Exp()
	local nCurLevel = GetLevel()
	if(nCurLevel >= 150) then
		Talk(1,"","§¼ng cÊp ®· ®¹t 120")
		return
	end
	local nNextLevel = nCurLevel + 10
	if(nNextLevel > 150) then
		nNextLevel = 150
	end
	while(GetLevel() < nNextLevel) do
		AddOwnExp(1000000)
	end
end

----------------------------
--
----------------------------
function PL_Coin()
	local nCurCoin = GetJbCoin()
	-- if(nCurCoin >= 10000) then
		-- Talk(1,"","§· cã ®ñ 1 v¹n xu")
		-- return
	-- end
	-- local nNextCoin = 10000 - nCurCoin
	for i = 1,10000 do
		AddTaskItem(19)
	end
end

----------------------------
--
----------------------------
function PL_Cash()
	local nCurCash = GetCash()
	if(nCurCash >= 10000000) then
		Talk(1,"","§· cã ®ñ 1000 v¹n l­îng")
		return
	end
	local nNextCash = 10000000 - nCurCash
	Earn(nNextCash)
end

----------------------------
--
----------------------------
function PL_LeadLevel()
	local nCurLevel = GetLeadLevel()
	if(nCurLevel >= 20) then
		Talk(1,"","Tµi l·nh ®¹o ®· ®ñ 20 cÊp")
		return
	end
	while(GetLeadLevel() < 20) do
		AddLeadExp(1000000)
	end
end

----------------------------
--
----------------------------
function PL_Repute()
	if(GetRepute() >= 300) then
		Talk(1,"","Danh väng ®· ®ñ 300 ®iÓm")
		return
	end
	AddRepute(300)
end