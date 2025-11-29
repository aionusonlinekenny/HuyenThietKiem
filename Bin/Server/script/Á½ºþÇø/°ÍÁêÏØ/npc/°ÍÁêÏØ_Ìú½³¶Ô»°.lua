-- Thî rÌn ë Ba L¨ng huyÖn - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");
Include("\\script\\global\\global_tiejiang.lua")

TIEJIANG_DIALOG = "<dec><npc>ViÖc bu«n b¸n ë ®©y rÊt thuËn lîi, ®Õn ngay c¶ c¸c huynh ®Ö cña Thiªn V­¬ng Bang th­êng ®Õn ®©y nhê ta chÕ t¹o binh khÝ."..Note("thoren_balanghuyen");

function main(sel)
	tiejiang_village()
end;


function yes()
Sale(37);  			--µ¯³ö½»Ò×¿ò
end;
