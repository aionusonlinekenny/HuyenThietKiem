-- ¤ng chñ tiÒn trang ë thµnh thŞ - Editor by AloneScript (Linh Em)

Include("\\script\\\global\\npc\\chutientrang.lua")

function main(sel)
	i = random(0,1)
	if (i == 0) then
		Talk(1,"main_proc","Ng­êi lµm ¨n ®­¬ng nhiªn muèn lêi cµng nhiÒu cµng Tèt! Thiªn h¹ cã mÊy ai chª tiÒn?")
	else
		Talk(1,"main_proc","Mäi thø ®Òu lµ gi¶, chØ cã vµng thËt, b¹c tr¾ng míi lµ thËt!")
	end
end;
