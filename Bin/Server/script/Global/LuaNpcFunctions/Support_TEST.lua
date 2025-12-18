-- Author:	Kinnox;
-- Date:	28-05-2021
-- Functions: Support Test;
Include("\\script\\system_config.lua")
Include("\\script\\lib\\admin\\point.lua");
Include("\\script\\lib\\admin\\char.lua");
Include("\\script\\lib\\admin\\gm_action.lua");
Include("\\script\\lib\\admin\\item.lua");
Include("\\script\\lib\\admin\\role.lua");
Include("\\script\\lib\\admin\\tool.lua");
Include("\\script\\lib\\admin\\equip.lua");
Include("\\script\\lib\\admin\\movemap.lua");
Include("\\script\\lib\\tasklib.lua");
function main(nNpcIndex)
	if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		dofile("script/global/luanpcfunctions/Support.lua");
		Msg2Player("Reload this script");
	end;

	local nW,nX,nY = GetWorldPos()
	local szHello = "<color=red>HÖ Thèng:<color> <color=orange>Cuéc sèng nµy biÕt bao ®iÒu tu¬i ®Ñp, §¹i hiÖp xin h·y b×nh tÜnh suy xÐt\n<color=green>Täa ®é hiÖn t¹i: B¶n ®å: "..nW.." - X: "..nX.."-Y: "..nY.."\n"..nW.." - X: "..(nX*32).."-Y: "..(nY*32)..""
	Tab_inSert = {
	"C¸c lo¹i ®iÓm/GM_Char",
	"Trang bÞ xanh./PL_BlueEquip",
	"ChiÕn m·./GM_GetHorse",
	"MÆt n¹./PL_Mask",
	"Trang bi Hoµng Kim c¬ b¶n./PL_BasicGoldEquip",
	--"Trang bÞ hoµng kim m«n ph¸i VIP/PL_PremiumGoldEquip",
	--"Më réng r­¬ng/ExPandBox",
	"Ta kh«ng cÇn ng­¬i d¹y b¶o/no",
	}
	Say(szHello,getn(Tab_inSert),Tab_inSert);
end;
