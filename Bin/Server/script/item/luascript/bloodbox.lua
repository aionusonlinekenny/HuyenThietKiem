-- Author: Kinnox
-- Date: 25/12/2021
-- Item: Tui mau tan thu
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\Revivepos.lua")

WARNING_TALK = "Kh«ng thÓ thao t¸c liªn tôc, vui lßng chê sau<color=yellow> %d gi©y<color> n÷a.";
nYr,nMo,nDy,nHr,nMi,nSe,nDyfW = 0,0,0,0,0,0,0;
function OnUse(nIndex)
	BloodBox(nIndex)
end

function BloodBox(nIndex)	
	local nW, nX, nY = GetWorldPos();	
	if (nW ~= 242 and nW ~= 342 and nW ~= 341 and nW ~= 337 and nW ~= 338 and nW ~= 339 and 
	nW ~= 464 and nW ~= 465 and nW ~= 466 and nW ~= 467 and nW ~= 468 and nW ~= 469 and nW ~= 470) then -- map tay tuy, vuot ai, phong lang do;
		if CheckMapNoForTHP(nW) == 1 then
		Msg2Player("Kh«ng thÓ sö dông Tói ®­îc phÉm ë ®©y.")
		return end	
	end
	
	nYr,nMo,nDy,nHr,nMi,nSe = GetTimeNow();
	local nNowMi = nDy * 24 * 60 * 60 + nHr * 60 * 60 + nMi * 60 + nSe;
	
	if (nNowMi < 15) then
		Msg2Player(format(WARNING_TALK, nNowMi));
		--Talk(1, "",format(WARNING_TALK,nNowMi));
	return end
	
	if (GetTaskTemp(TMP_TGIANMOTUI) == 0) then
		SetTaskTemp(TMP_TGIANMOTUI, nNowMi)
		AddBlood()
		Msg2Player(format(WARNING_TALK, 15));
		--Talk(1, "",format(WARNING_TALK,15));
	return end
	
	local nLasMi = nNowMi - 15;
	if (nLasMi < GetTaskTemp(TMP_TGIANMOTUI)) then
		local nEndMi = GetTaskTemp(TMP_TGIANMOTUI) - nLasMi;
		if (nEndMi <= 15) then
			Msg2Player(format(WARNING_TALK, nEndMi));
			--Talk(1, "",format(WARNING_TALK,nEndMi));
		else
			SetTaskTemp(TMP_TGIANMOTUI,0);
			Msg2Player("Tù ®éng reset thêi gian më tói m¸u..");
			--Talk(1, "","Tù ®éng reset thêi gian më tói m¸u.");
		return end
	return end
	
	AddBlood(nW)
	SetTaskTemp(TMP_TGIANMOTUI, nNowMi);
end


function AddBlood(nW)
	local nMax = 20;
	local nCount = CalcFreeItemCellCount();
	if(GetPKFlag() > 0) then
	Msg2Player("ChØ sö dông tói m¸u trong tr¹ng th¸i <color=yellow>luyÖn c«ng<color>");
	--Talk(1,"","ChØ sö dông tói m¸u trong tr¹ng th¸i <color=yellow>luyÖn c«ng<color>");
	return end;
	
	if (nCount >= nMax) then
		nCount = nMax
	end
	local nIndex;
	if (GetLevel() > 80 or nW == 242 or nW == 342 or nW == 341) then
		for i = 1,nCount do
		nIndex = AddItem(5,2,0,5,0,0,0);
		SetItemBindState(nIndex,2);
		end
	else
		for i = 1,nCount do
		nIndex = AddItem(5,2,0,4,0,0,0);
		SetItemBindState(nIndex,2);
		end
	end
	Msg2Player("§¹i hiÖp nhËn ®­îc <color=green>x"..nCount.."<color>  d­îc phÈm luyÖn c«ng!");
end

function no()

end