----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Bå c©u
----------------------------------
Include("\\script\\item\\battles\\checkmapid.lua")

----------------------------------
--
----------------------------------
function EatMedicine()
	local nCurCamp = GetCurCamp()
	local _,nX,nY = GetWorldPos()
	nX = floor(nX / 8)
	nY = floor(nY / 16) 
	local sStr = "<color=orangered><color=wood>"..GetName().." <color>ë täa ®é <color=yellow>("..nX..","..nY..")<color> ®ang yªu cÇu viÖn trî."
	Msg2MSGroup(ZBattle.tbMission.MAIN, sStr, nCurCamp)
	Msg2Player("<color=orangered>§¹i hiÖp ®· sö dông mét bå c©u.<color>")
	return 1
end 
