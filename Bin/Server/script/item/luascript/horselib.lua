-- Author: Kinnox
-- Date: 25/12/2021
-- Item: ma bai
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\lib\\Revivepos.lua")
function OnUse(nIdx)
	local _, nD, _ = GetItemProp(nIdx);
	local nIDGen = 0;
	local nIDDet = 0;
	local nNameHorse ="";
	if (nD == 74) then
	nNameHorse = "Ngùa xÝch thè";
	nIDGen = 5;
	nIDDet = 7;
	elseif (nD == 75) then
	nNameHorse = "§Þch l«";
	nIDGen = 5;
	nIDDet = 9;
	elseif (nD == 76) then
	nNameHorse = "TuyÖt ¶nh";
	nIDGen = 5;
	nIDDet = 8;
	elseif (nD == 77) then
	nNameHorse = "¤ v©n ®¹p tuyÕt";
	nIDGen = 5;
	nIDDet = 6;
	elseif (nD == 78) then
	nNameHorse = "ChiÕu d¹ ngäc s­ tö";
	nIDGen = 5;
	nIDDet = 10;
	elseif (nD == 79) then
	nNameHorse = "B«n Tiªu";
	nIDGen = 6;
	nIDDet = 10;
	elseif (nD == 80) then
	nNameHorse = "Phi Vò";
	nIDGen = 7;
	nIDDet = 10;
	end;
	AddItem(0,10,nIDGen,nIDDet,random(0,4),0,0,0,0,0,0,0,30) -- 30 ngay;
	Msg2Player("Chóc mõng c¸c h¹ nhËn ®­îc chiÕn m· <color=yellow> "..nNameHorse.." <color>");
	return 1;
end
