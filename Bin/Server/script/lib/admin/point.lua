----------------------------------
--	Copyright: JxOnline by kinnox;
--	Author: kinnox
--	Date: 15/11/2021
--	Desc: Script Support Point
----------------------------------

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
	for i = 1,1000 do
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
	if(nCurLevel >= 100) then
		Talk(1,"","Tµi l·nh ®¹o ®· ®ñ 100 cÊp")
		return
	end
	while(GetLeadLevel() < 100) do
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
----------------------------
--
----------------------------
function PL_Fuyuan()
	if(GetFuyuan() >= 300) then
		Talk(1,"","Phóc duyªn ®· ®ñ 300 ®iÓm")
		return
	end
	AddFuyuan(300)
end