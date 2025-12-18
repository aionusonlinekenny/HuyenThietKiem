--Author: kinnox;
--Date: 20/01/2022;
--Name: Xa loi kim don;
function OnUse(nIdx)
	local nPK = GetPK();
	if (nPK > 0) then
		SetPK(GetPK()-1);
		Talk(1,"","§é tµ ¸c cña ng­¬i ®· ®­îc ta ®é tr× xuèng <color=green>1 ®iÓm<color> cßn l¹i <color=yellow> "..GetPK().." <color>");
	else
		Talk(1,"","B¶n chÊt nhµ ng­¬i l­¬ng thiÖn ®«i tay trong s¹ch ch­a nhuém m¸u giang hå! ");
		return 0;
	end
	return 1
end