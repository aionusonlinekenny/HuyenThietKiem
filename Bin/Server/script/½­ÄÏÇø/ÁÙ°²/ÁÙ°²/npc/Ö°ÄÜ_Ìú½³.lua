-- Thî rÌn ë L©m An - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");
Include("\\script\\global\\global_tiejiang.lua")

TIEJIANG_DIALOG = "<dec><npc>Gian hµng thî rÌn nµy lµ do «ng tæ ta truyÒn l¹i, t¹i L©m An nµy kh«ng ng­êi nµo kh«ng biÕt, xin hái ®¹i hiÖp muèn mua mãn ®å nµo?";
function main()
	tiejiang_city(TIEJIANG_DIALOG..Note("thoren_laman"));
end;


function yes()
	Sale(1);
end;

