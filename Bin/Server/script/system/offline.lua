--*--
function Offline()	
	local szHello = "<pic=136> <color=red>HÖ Thèng:<color> ñy th¸c b¸n hµng hoµn toµn <color=green>miÔn phÝ <color> \nnÕu ®¹i hiÖp muèn ñy th¸c thªm kinh nghiÖm th× nªn sö dông b¹ch cÇu hoµn\nThêi gian BCH cßn l¹i lµ : <color=green>"..GetTask(2024).."<color> gi©y\n®¹i hiÖp muèn ñy th¸c chø?";
	local tbSay = {
		"LËp tøc óy th¸c/yes",
		"Ta sÏ quay l¹i sau/OnExit"
	}
	Say(szHello, getn(tbSay), tbSay);
end;

function yes()
	local nOffline = 0;
	local nLive = 1;
	SetStatusLixian(nLive);
end;

function OnExit()
end;