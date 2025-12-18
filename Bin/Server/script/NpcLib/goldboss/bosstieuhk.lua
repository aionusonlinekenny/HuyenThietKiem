--script add boss
Include("\\script\\system_config.lua");
BOSSTIEU_ARRAY={
{"DiÖu Nh­",513,2,{1,6},78,1740*32,3323*32,"T­¬ng d­¬ng"},
{"LiÔu Thanh Thanh",523,1,{2,5},37,1582*32,3253*32,"Biªn kinh"},
{"Tr­¬ng T«ng ChÝnh",511,4,{3,4},80,1389*32,3410*32,"D­¬ng ch©u"}
};

COMMON_INFO = "Cã ng­êi nh×n thÊy <color=Yellow> %s <color> xuÊt hiÖn t¹i %s täa ®é <color=green> %d/%d <color>"

function releasebosstieu()
	local nNpcIndex = 0;
	local nPos = 1;
	for i=1,getn(BOSSTIEU_ARRAY) do
		local SubWorld = SubWorldID2Idx(BOSSTIEU_ARRAY[i][5]);
		nNpcIndex = AddNpc(BOSSTIEU_ARRAY[i][2], 95, SubWorld, BOSSTIEU_ARRAY[i][6], BOSSTIEU_ARRAY[i][7], 1, BOSSTIEU_ARRAY[i][1], 2, 0);
		SetNpcSeries(nNpcIndex,BOSSTIEU_ARRAY[i][3]);
		SetNpcScript(nNpcIndex, "\\script\\global\\luanpcmonsters\\ondeath_normal.lua");
		SetNpcReduceDamge(nNpcIndex, 160);	
		IsBoss(nNpcIndex,2);
		if(nNpcIndex > 0) then
			Msg2SubWorld(format(COMMON_INFO,BOSSTIEU_ARRAY[i][1],
			BOSSTIEU_ARRAY[i][8],
			floor(BOSSTIEU_ARRAY[i][6]/256),
			floor(BOSSTIEU_ARRAY[i][7]/512)));
			AddLocalNews(format(COMMON_INFO,BOSSTIEU_ARRAY[i][1],
			BOSSTIEU_ARRAY[i][8],
			floor(BOSSTIEU_ARRAY[i][6]/256),
			floor(BOSSTIEU_ARRAY[i][7]/512)));
		end
	end
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Boss tiÓu Hoµng kim<color> xuÊt hiÖn t¹i T­¬ng d­¬ng(Täa ®é:217/207), BiÖn kinh(Täa ®é:197/203), D­¬ng ch©u(Täa ®é:173/213) <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
end;