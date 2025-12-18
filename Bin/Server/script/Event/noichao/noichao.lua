Include("\\script\\event\\noichao\\head.lua")
function main(nNpcIndex)
	TimeBox("§ang ¨n ch¸o...",90,"noichao",tonumber(nNpcIndex));
end;

function noichao(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end

	nNumberChao = nNumberChao + 1;
	if (nNumberChao > nMaxNumberChao) then
		DelNpc(nNpcIndex);
		nNumberChao = 0;
	end;
	local nRanExp = random(100000,5000000);
	AddOwnExp(nRanExp)
	Msg2Player("§¹i hiÖp <color=pink> "..GetName().." <color> võa ¨n xong mét b¸t ch¸o yªu th­¬ng lÜnh héi ®­îc <color=yellow>"..nRanExp.." kinh nghiÖm <color>")
end
