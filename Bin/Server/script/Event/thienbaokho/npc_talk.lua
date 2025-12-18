Include("\\script\\event\\thienbaokho\\head.lua");
function main(nIndex)
	local nW,nX,nY = GetWorldPos();
	if (nW ~= 53) then
		if (GetNpcName(nIndex) == "DÞch quan") then
			NpcTalk(nIndex,1);
		else
			NpcTalk(nIndex,2);
		end
	else
		NpcTalk(nIndex,3);
	end
end;