--Author: Kinnox;
--Date: 17/04/2022;
function main(nNpcIndex)
	local szHello = "<color=orange>B¹o Nh©n<color>: N¬i nµy lo¹n l¹c khã, hiÕm anh hïng d¸m ë l¹i ®©y!";
	local szSay = {
		"Ta muèn ®æi KiÕm HiÖp LÖnh/excuterkth",
		"Ta ®Õn th¨m hái nhµ ng­êi/no",
	};
	Say(szHello,getn(szSay),szSay);
end;

function excuterkth()
	local NeedItem = 28;
	if (GetTaskItemCount(NeedItem) < 50) then
		Talk(1,"","H·y mang ®ñ 50 Khiªu chiÕn lÖnh ®Õn gÆp ta!");
		return
	end;
	Msg2Player(""..GetTaskItemCount(NeedItem).."");
	DelTaskItem(NeedItem,50);
	AddScriptItem(130);
	Msg2Player("<color=yellow> Ng­¬i nhËn ®­îc mét KiÕm hiÖp lªnh<color>");
	return
end;


function no()
end;