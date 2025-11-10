----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Xa phu l«i ®µi tû vâ
----------------------------------
Include("\\script\\mission\\bw\\functions.lua")

tbCaptainName = {}
----------------------------------
-- Hµm gäi chÝnh
----------------------------------
function main()
	Say("C¸c h¹ muèn rêi l«i ®µi ph¶i kh«ng",3,"§óng vËy, ta muèn rêi khái ®©y!/OnLeave","Ta lµ ®éi tr­ëng, ta muèn biÕt m· trËn ®Êu./OnShowKey","Kh«ng cÇn/OnCancel")
end

----------------------------------
-- Xem m· trËn ®Êu
----------------------------------
function OnShowKey()
	ZBW:OnShowKey()
end

----------------------------------
-- Rêi khái trËn ®Êu
----------------------------------
function OnLeave()
	ZBW:LeaveMap()
end

----------------------------------
-- Hñy bá
----------------------------------
function OnCancel()
end