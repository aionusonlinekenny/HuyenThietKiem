Include("\\script\\system_config.lua");
-- HOA SON
CHAO_BANG = {
{80064,110976},
{80064,111296},
{80224,111296},
{80224,110944},
{80416,110880},
{80448,111136},
{80576,110816},
{80608,111104},
{80800,110816},
{80928,111072},
{80960,110752},
{80576,111424},
{80192,111776},
};

--------------------
SCRIPT_CHAO = "\\script\\event\\pkbang\\noichaobang.lua"
--------------------

function AddBossBang()
	local nNpcIndex = 0;
	local SubWorld = SubWorldID2Idx(2);
	nNpcIndex = AddNpc(RANDOMC(513,523,511), 95, SubWorld, 80416, 111488, 1, "Ng­êi canh gi÷ ch¸o", 13, 0);
	SetNpcReduceDamge(nNpcIndex, 160);	
	SetNpcScript(nNpcIndex, "\\script\\event\\pkbang\\npc_death.lua");
	Msg2SubWorld("<color=blue>Ho¹t ®éng Boss Bang Héi xuÊt hiÖn t¹i <color> <color=green>314/217 Hoa s¬n c¶nh kü tr­êng<color> ");
	for i = 1,SYS_MAX_PLAYER do
		PlayerIndex = i;
		SendNotification("","(Lo) <color=red>Th«ng b¸o: <color> <color=green>Boss Bang héi<color>xuÊt hiÖn t¹i Hoa s¬n c¶nh kü tr­êng <color=green>314/217<color> <pic="..random(1,140).."> <pic="..random(1,140).."> !");
	end;
end;

function AddNoiChaoBang(szName)
	local nSubWorld;
	local szName = tostring(szName)
	--So cap
	for i = 1,getn(CHAO_BANG) do
		SubWorld = SubWorldID2Idx(2);
		nNpcIndex = AddNpc(1275, 1, SubWorld, CHAO_BANG[i][1], CHAO_BANG[i][2], 0, szName, 0, 3);
		SetNpcName(nNpcIndex,szName);
		SetNpcScript(nNpcIndex, SCRIPT_CHAO);
	end;
	--thong bao--
	Msg2SubWorld("Bang héi <color=yellow>"..szName.." thµnh c«ng dµnh chiÕn th¾ng, phÇn th­ëng lµ mét ®èng xu vµ 10 nåi ch¸o bang EXP 5tr/nåi");
end;


