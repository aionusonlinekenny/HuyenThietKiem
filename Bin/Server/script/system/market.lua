
-- cai nay de nguyen - shop ky tran cac
----------------------------------
--
----------------------------------
function OpenMarket(nSellID)
	local sMsg = "<color=yellow>Kú Tr©n C¸c chØ cã thÓ më ë c¸c thµnh thÞ, t©n thñ th«n vµ mét sè khu vùc phi chiÕn ®Êu kh¸c!<color>"
	if(GetFightState() == 1) then
		Msg2Player(sMsg)
		return
	end
	local nID = tonumber(nSellID)
	Sale(nID)
end	