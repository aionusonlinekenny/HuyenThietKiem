-- mem thoat
Include("\\script\\mission\\phonglangdo\\player_logout.lua")
Include("\\script\\mission\\battles\\playerlogout.lua");
function main()
	local nW,_,_ = GetWorldPos()	
	if(nW == 337 or nW == 338 or nW == 339) then
		OnLogout(PlayerIndex,0)
	end;
	
	if (nW == 380) then
		OnLogout(PlayerIndex)
	end;
end
