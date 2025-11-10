nMaxNumberChao = 30;
nNumberChao = 0;
Include("\\script\\system_config.lua");
COMMON_INFOS = "<color=Yellow> Nåi ch¸o yªu th­¬ng <color> xuÊt hiÖn t¹i %s täa ®é <color=green> %d/%d <color>, kinh nghiÖm lªn ®Õn 5tr EXP, sè l­îng cã h¹n."

CODINATE = { 
	{1275,51488,102784,53,"Ba l¨ng huyÖn"},
	{1275,51872,102144,53,"Ba l¨ng huyÖn"},
	{1275,50976,103104,53,"Ba l¨ng huyÖn"},
}
function releasenoichao()
	local nNpcIndex = 0;
	local nPos = 1;
	for i=1,getn(CODINATE) do
		local SubWorld = SubWorldID2Idx(CODINATE[i][4]);
		nNpcIndex = AddNpc(CODINATE[i][1], 95, SubWorld, CODINATE[i][2], CODINATE[i][3], 1, "Nåi ch¸o", 0, 3);
		SetNpcScript(nNpcIndex, "\\script\\event\\noichao\\noichao.lua");
		if(nNpcIndex > 0) then
			Msg2SubWorld(format(COMMON_INFOS,
			CODINATE[i][5],
			floor(CODINATE[i][2]/256),
			floor(CODINATE[i][3]/512)));
			AddLocalNews("Nåi ch¸o yªu th­¬ng xuÊt hiÖn!");
		end
	end
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Nåi ch¸o yªu th­¬ng :0<color> xuÊt hiÖn t¹i Ba l¨ng huyÖn täa ®é <color=green> 201/200 <color> <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
end;