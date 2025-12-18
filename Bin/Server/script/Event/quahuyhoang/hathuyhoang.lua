function main(nNpcIndex)
	local nTime = tonumber(date("%M"));
	local nParam = GetNpcParam(nNpcIndex,1);
	if (nTime <= 5) then
		nMin = 5 - nTime;
	elseif (nTime > 5 and nTime <= 20) then
		nMin = 20 - nTime;
	elseif (nTime > 20 and nTime <= 35) then
		nMin = 35 - nTime;
	end;

	if (nParam == 1) then
		Talk(1,"","H¹t huy hoµng s¬ cÊp sÏ chÝn trong "..nMin.." phót n÷a, ®¹i hiÖp h·y ®îi nhÐ!");
	elseif (nParam == 2)	then
		Talk(1,"","H¹t huy hoµng trung cÊp sÏ chÝn trong "..nMin.." phót n÷a, ®¹i hiÖp h·y ®îi nhÐ!");
	elseif (nParam == 3)	then
		Talk(1,"","H¹t huy hoµng cao cÊp sÏ chÝn trong "..nMin.." phót n÷a, ®¹i hiÖp h·y ®îi nhÐ!");
	end;
end;