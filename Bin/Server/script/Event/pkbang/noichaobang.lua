Include("\\script\\event\\pkbang\\head.lua")
function main(nNpcIndex)
	if (GetTongName() ~= GetNpcName(nNpcIndex)) then
		Talk(1,"","Ng­¬i kh«ng thuéc Bang <color=yellow>"..GetNpcName(nNpcIndex).."<colo> tuæi g× mµ ®Õn ®©y ¨n ch¸o, mêi ®i cho ");
	return end
	TimeBox("§ang hóp ch¸o...",30,"noichaobang",tonumber(nNpcIndex));

	
end;

function noichaobang(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end
	DelNpc(nNpcIndex);
	local Exp = 5000000;
	AddOwnExp(Exp);
	Earn(10000)
	Msg2Player("§¹i hiÖp thµnh c«ng lÜnh héi ®­îc <color=pink> "..Exp.."  vµ 1 v¹n l­îng <color>!. ")
end;