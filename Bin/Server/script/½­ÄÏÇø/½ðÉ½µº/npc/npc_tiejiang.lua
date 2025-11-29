-- Thî rÌn ë Vi S¬n §¶o - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");
Include("\\script\\global\\equip_system.lua");

function main(sel)
	--Say("Muèn sèng ®­îc trªn T©y S¬n Tù µh, kh«ng cã binh khİ lîi h¹i cña ta e r»ng ng­¬i chŞu nhiÒu thiÖt thßi ®ã. Ng­¬i cã muèn mua g× kh«ng? Cßn muèn chÕ t¹o Trang bŞ HuyÒn Tinh hoÆc trang bŞ Hoµng Kim th× cø ®Õn t×m ta gi¸ c¶ ph¶i ch¨ng th«i!", 3, "Giao dŞch/yes", "chÕ t¹o/onFoundry", "Nh©n tiÖn ghĞ qua th«i/no")
	Say("<color=green>Thî rÌn<color>: Muèn sèng ®­îc trªn Vi S¬n ®¶o µh, kh«ng cã binh khİ lîi h¹i cña ta e r»ng ng­¬i chŞu nhiÒu thiÖt thßi ®ã. Ng­¬i cã muèn mua g× kh«ng?"..Note("thoren_visondao"), 2, "Giao dŞch/yes", NOTTRADE);
end;

function yes()
	Sale(13);  				
end;
