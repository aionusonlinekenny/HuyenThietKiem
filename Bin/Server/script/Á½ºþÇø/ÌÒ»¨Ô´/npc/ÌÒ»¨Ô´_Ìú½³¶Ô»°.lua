-- Thî rÌn ë Vi S¬n §¶o - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");
Include("\\script\\global\\equip_system.lua");

function main(sel)
	-- Say("X­a nay anh hïng phèi mü töu, danh kiÕm phèi hiÖp sÜ, mét cµnh liÔu còng cã thÓ ®¶ th­¬ng ®­îc ng­êi, nh­ng ng­êi häc vâ ai l¹i kh«ng thİch cã mét thanh b¶o kiÕm s¾c bĞn chø? NÕu muèn chÕ t¹o trang bŞ HuyÒn Tinh hoÆc trang bŞ Hoµng Kim th× cã thÓ ®Õn t×m ta gi¸ c¶ ph¶i ch¨ng th«i!", 3, "Giao dŞch/yes", "chÕ t¹o/onFoundry", "Nh©n tiÖn ghĞ qua th«i/no");
	Say("<color=green>Thî rÌn<color>: X­a nay anh hïng phèi mü töu, danh kiÕm phèi hiÖp sÜ, mét cµnh liÔu còng cã thÓ ®¶ th­¬ng ®­îc ng­êi, nh­ng ng­êi häc vâ ai l¹i kh«ng thİch cã mét thanh b¶o kiÕm s¾c bĞn chø?"..Note("thoren_daohoanguyen"), 2, "Giao dŞch/yes", NOTTRADE);
end;

function yes()
	Sale(40);  			--µ¯³ö½»Ò×¿ò
end;

function no()
end;







