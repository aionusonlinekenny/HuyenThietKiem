function main(nNpcIndex)
	local nParam = GetNpcParam(nNpcIndex,1);
	if (nParam == 1) then
		TimeBox("ßang nh∆t qu∂",10,"nhatquahhso",tonumber(nNpcIndex));
	elseif (nParam == 2)	then
		TimeBox("ßang nh∆t qu∂",10,"nhatquahhtrung",tonumber(nNpcIndex));
	elseif (nParam == 3)	then
		TimeBox("ßang nh∆t qu∂",10,"nhatquahhcao",tonumber(nNpcIndex));
	else
		TimeBox("ßang nh∆t qu∂",10,"nhatquahk",tonumber(nNpcIndex));
	end;
end;

function nhatquahhso(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end
	DelNpc(nNpcIndex);
	AddScriptItem(7);
	Msg2Player("ßπi hi÷p Æ∑ nh∆t Æ≠Óc mÈt qu∂ huy hoµng s¨ c p.")
end

function nhatquahhtrung(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end
	DelNpc(nNpcIndex);
	AddScriptItem(8);
	Msg2Player("ßπi hi÷p Æ∑ nh∆t Æ≠Óc mÈt qu∂ huy hoµng trung c p.")
end

function nhatquahhcao(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end
	DelNpc(nNpcIndex);
	AddScriptItem(9);
	Msg2Player("ßπi hi÷p Æ∑ nh∆t Æ≠Óc mÈt qu∂ huy hoµng cao c p.")
end

function nhatquahk(nNpcIndex)
	if (FindAroundNpc(GetNpcID(2,nNpcIndex)) == 0) then
	return end
	DelNpc(nNpcIndex);
	AddScriptItem(10);
	Msg2Player("<color=orange>ßπi hi÷p Æ∑ nh∆t Æ≠Óc mÈt qu∂ hoµng kim cao c p.<color>")
	Msg2SubWorld("<color=orange>ßπi hi÷p "..GetName().." nh∆t Æ≠Óc mÈt qu∂ hoµng kim cao c p.<color>");
end