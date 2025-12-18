-- Author: Kinnox
-- Date: 25/12/2021
-- Item: Tong hop cac loai bi kip 9x
function OnUse(nIdx)
	local _, nD, _ = GetItemProp(nIdx);
	local nIDMagic = 0;
	local sMenpai = "";
	local nLevel = GetLevel();
	if (nD >= 31 and nD < 59) then
		if (nLevel < 80) then
			Talk(1,"","Ng­¬i ch­a ®ñ ®¼ng cÊp ®Ó lÜnh héi tuyÖt kü nµy, h·y rÌn luyÖn thªm ®Ó ®¹t cÊp 80!");
		return end;
	else
		if (nLevel < 120) then
			Talk(1,"","Ng­¬i ch­a ®ñ ®¼ng cÊp ®Ó lÜnh héi tuyÖt kü nµy, h·y rÌn luyÖn thªm ®Ó ®¹t cÊp 120!");
		return end;
	end
	
	if (GetFaction() == "shaolin") then
		sMenpai = "ThiÕu L©m Ph¸i"
		if (nD == 31) then
		 nIDMagic = 318;
		elseif (nD == 32) then
		 nIDMagic = 321;
		elseif (nD == 33) then
		 nIDMagic = 319;
		elseif (nD == 61) then --120
		 nIDMagic = 709;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "tianwang")  then
		sMenpai = "ThiÕu V­¬ng Bang"
		if (nD == 34) then
		 nIDMagic = 323;
		elseif (nD == 35) then
		 nIDMagic = 325;
		elseif (nD == 36) then
		 nIDMagic = 322;
		elseif (nD == 67) then --120
		 nIDMagic = 708;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "tangmen")  then
		sMenpai = "§­¬ng M«n Ph¸i"
		if (nD == 37) then
		 nIDMagic = 302;
		elseif (nD == 38) then
		 nIDMagic = 339;
		elseif (nD == 39) then
		 nIDMagic = 342;
		elseif (nD == 40) then 
		 nIDMagic = 351;
		elseif (nD == 64) then --120
		 nIDMagic = 710;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "wudu")  then
		sMenpai = "Ngò §éc Gi¸o Ph¸i"
		if (nD == 41) then
		 nIDMagic = 353;
		elseif (nD == 42) then
		 nIDMagic = 355;
		elseif (nD == 43) then
		 nIDMagic = 390;
		elseif (nD == 60) then --120
		 nIDMagic = 711;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "emei")  then
		sMenpai = "Nga My Ph¸i"
		if (nD == 44) then
		 nIDMagic = 380;
		elseif (nD == 45) then
		 nIDMagic = 328;
		elseif (nD == 46) then
		 nIDMagic = 332;
		elseif (nD == 59) then --120
		 nIDMagic = 712;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "cuiyan")  then
		sMenpai = "Thóy Yªn Ph¸i"
		if (nD == 47) then
		 nIDMagic = 336;
		elseif (nD == 48) then
		 nIDMagic = 337;
		elseif (nD == 63) then --120
		 nIDMagic = 713;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "gaibang")  then
		sMenpai = "C¸i bang Ph¸i"
		if (nD == 49) then
		 nIDMagic = 357;
		elseif (nD == 50) then
		 nIDMagic = 359;
		elseif (nD == 62) then --120
		 nIDMagic = 714;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "tianren")  then
		sMenpai = "Thiªn NhÉn Gi¸o Ph¸i"
		if (nD == 51) then
		 nIDMagic = 362;
		elseif (nD == 52) then
		 nIDMagic = 361;
		elseif (nD == 53) then
		 nIDMagic = 391;
		elseif (nD == 66) then --120
		 nIDMagic = 715;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "wudang")  then
		sMenpai = "Vâ §ang Ph¸i"
		if (nD == 54) then
		 nIDMagic = 365;
		elseif (nD == 55) then
		 nIDMagic = 368;
		elseif (nD == 68) then --120
		 nIDMagic = 716;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	elseif (GetFaction() == "kunlun")  then
		sMenpai = "C«n Lu©n Ph¸i"
		if (nD == 56) then
		nIDMagic = 375;
		elseif (nD == 57) then
		nIDMagic = 372;
		elseif (nD == 58) then
		 nIDMagic = 394;
		elseif (nD == 65) then --120
		 nIDMagic = 717;
		else
			Msg2Player("Ng­¬i lµ ®Ö tö cña "..sMenpai..", kh«ng thÓ lÜnh héi kü n¨ng cña m«n ph¸i nµy!.");
			return 0;
		end;
	else
		Msg2Player("Ng­¬i kh«ng lµ ®Ö tö cña m«n ph¸i nµo trªn giang hå c¶!.");
		return 0;
	end;
	
	if( HaveMagic(nIDMagic) >= 1 ) then
		Talk(1,"","Ng­¬i ®· lÜnh héi kü n¨ng nµy råi, kh«ng thÓ tiÕp tôc sö dông bİ quyÕt.")
		return 0;
	end
	
	AddMagic(nIDMagic,1);
	Msg2Player("Chóc mõng c¸c h¹ lÜnh héi ®­îc kü n¨ng thÊt truyÒn cña ph¸i <color=yellow> "..sMenpai.." <color>");
	return 1;
end


