-- T¹p hãa ë L©m An - Editor by AloneScript (Linh Em)

Include("\\script\\lib\\alonelib.lua");

--========½Å±¾ÐÞ¸Ä==================================--
--ÐÞ¸ÄÕß£ºyfeng
--ÐÞ¸ÄÈÕÆÚ£º2005-1-28
--ÐÞ¸ÄÔ­Òò£º´º½ÚÊÐ³¡»î¶¯
--
--
--==================================================--
Include "/script/event/chinesenewyear/eventhead.lua"
Include("\\script\\global\\global_zahuodian.lua");


--if CHINESENEWYEAR then
--	tinsert(Buttons,1,"¹ºÂò½ÚÈÕµÀ¾ß/BuyNewyearItem")
--end 
	
function main()
local Buttons = store_sel_extend();
	local nDate = tonumber(GetLocalDate("%Y%m%d%H"));
	if (nDate >= 2006122200 and nDate <= 2007011424) then
		tinsert(Buttons,1,"Mua ThiÖp/BuyVnXasCard")
	end;
	Say("<color=green>T¹p hãa<color>: Bæn tiÖm x­ng danh lµ “t¹p hãa nam b¾c”. Hµng hãa ®«ng t©y nam b¾c ®Òu cã ®ñ c¶. S¬n Hå ë §«ng H¶i, mò l¹c ®µ ë Gobi, Khæng t­íc Linh ë LÜnh Nam, da chån tÝm ë nói Tr­êng B¹ch, bÊt kÓ lµ thø ng­¬i ®· nh×n thÊy hay ch­a tõng thÊy qua, ë ®©y ta ®Òu cã c¶."..Note("taphoa_laman"),
			getn(Buttons),
			Buttons)
end;

function yes()
	Sale(2);  				--µ¯³ö½»Ò×¿ò
end;

function no()
end;

function BuyNewyearItem() --Mua ®¹o cô ngµy lÔ
	Sale(101)
	return
end

function BuyChristmasCard()
	Sale(97);
end

function BuyVnXasCard()
	Sale(147)
end;