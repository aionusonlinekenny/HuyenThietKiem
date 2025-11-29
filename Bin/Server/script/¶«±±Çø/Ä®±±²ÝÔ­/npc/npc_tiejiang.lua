-- Thî rÌn ë M¹c B¾c Th¶o Nguyªn - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");
Include("\\script\\global\\equip_system.lua");

function main(sel)
	-- Say("<color=green>Thî rÌn<color>: C¶ tr¨m dÆm gÇn ®©y ai kh«ng biÕt ®Õn ®¹i danh cña ta, tõ ®ao, kiÕm, c«n, th­¬ng, kİch, m­êi t¸m mãn vò khİ ta ®Òu cã ®ñ, kh¸ch quan muèn mua mãn nµo? Cßn muèn chÕ t¹o Trang bŞ HuyÒn Tinh hay trang bŞ Hoµng Kim ®Òu ph¶i ®Õn t×m ta, nhÊt ®Şnh gi¸ c¶ ph¶i ch¨ng!"..Note("thoren_macbacthaonguyen"), 3, "Giao dŞch/yes", "ChÕ t¹o/OnFoundry", "Nh©n tiÖn ghĞ qua th«i/no");
	Say("<color=green>Thî rÌn<color>: C¶ tr¨m dÆm gÇn ®©y ai kh«ng biÕt ®Õn ®¹i danh cña ta, tõ ®ao, kiÕm, c«n, th­¬ng, kİch, m­êi t¸m mãn vò khİ ta ®Òu cã ®ñ, kh¸ch quan muèn mua mãn nµo?"..Note("thoren_macbacthaonguyen"), 2, "Giao dŞch/yes", NOTTRADE);
end;

function yes()
	Sale(13);  				
end;
