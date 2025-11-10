-- Author: Kinnox
-- Date: 12/04/2022
-- Chuc nang: Thuc thi
Include("\\script\\event\\hoadang\\head.lua")
Include("\\script\\lib\\tasklib.lua")
MAX_NUM = 90;
function main(NpcIndex)
	local nFactionID = GetFactionNo() + 1;
	if nFactionID <= 0 then
		Talk(1,"","§¹i hiÖp ch­a vµo m«n ph¸i kh«ng thÓ tham gia.")
	return end	
	
	if GetTask(T_REPCAUHOI) >= MAX_NUM then
		Talk(1,"", "Ng­¬i ®· tr¶ lêi hoµn thµnh "..MAX_NUM.." c©u hái.")
	return end	
	
	SetTask(T_REPCAUHOI,GetTask(T_REPCAUHOI)+1)
	cauhoi(NpcIndex)
	AddSkillState(764,1,0,18*10);
end

function cauhoi(NpcIndex)
	-- local nValue = GetNpcValue(NpcIndex);
	
	local i = random(1,getn(CAUHOI))
	local a = random(1,4)
	
	if a == 1 then
		Say(CAUHOI[i][1] , 4,
			"A. "..CAUHOI[i][2].."/correct",
			"B. "..CAUHOI[i][3].."/wrong",
			"C. "..CAUHOI[i][4].."/wrong",
			"D. "..CAUHOI[i][5].."/wrong")
	elseif a == 2 then
		Say(CAUHOI[i][1] , 4,
			"A. "..CAUHOI[i][5].."/wrong",
			"B. "..CAUHOI[i][2].."/correct",
			"C. "..CAUHOI[i][3].."/wrong",
			"D. "..CAUHOI[i][4].."/wrong")
	elseif a == 3 then
		Say(CAUHOI[i][1] , 4,
			"A. "..CAUHOI[i][4].."/wrong",
			"B. "..CAUHOI[i][5].."/wrong",
			"C. "..CAUHOI[i][2].."/correct",
			"D. "..CAUHOI[i][3].."/wrong")
	elseif a == 4 then
		Say(CAUHOI[i][1] , 4,
			"A. "..CAUHOI[i][3].."/wrong",
			"B. "..CAUHOI[i][4].."/wrong",
			"C. "..CAUHOI[i][5].."/wrong",
			"D. "..CAUHOI[i][2].."/correct")
	end
	
	-- if (nValue == 0) then
	
	-- return end
	DelNpc(NpcIndex);	
end

function correct()
	local nRand = random(1,2);
	if nRand == 1 then
	AddRepute(random(1,5));
	Msg2Player("Tr¶ lêi chÝnh x¸c c©u  sè "..GetTask(T_REPCAUHOI)..".");		
	else
	AddFuyuan(random(1,5))
	Msg2Player("Tr¶ lêi chÝnh x¸c c©u  sè "..GetTask(T_REPCAUHOI)..".");			
	end
	AddOwnExp(random(500000,2000000));
	local nRandom = random(1,3) 
	if (nRandom == 3) then
		AddTaskItem(RANDOMC(16,18));
		Msg2Player("<color=green>§¹i hiÖp nhËn ®­îc 1 vËt phÈm quý hiÕm tõ ho¹t ®éng<color>");
	end;
	SetTask(T_RIGHTQUESTION,GetTask(T_RIGHTQUESTION)+1);
	Msg2Player("Tr¶ lêi chÝnh x¸c tæng céng <color=green>"..GetTask(T_RIGHTQUESTION).."/"..MAX_NUM.."<color>.");				
end


function wrong()
	Msg2Player("RÊt tiÕc b¹n tr¶ lêi kh«ng chÝnh x¸c <color=purple>c©u "..GetTask(T_REPCAUHOI).."<color>.");	
end
