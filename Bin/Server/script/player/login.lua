Include("\\script\\player\\head.lua")
Include("\\script\\lib\\TaskLib.lua")
Include("\\script\\mission\\bw\\head.lua")
Include("\\script\\system_config.lua");
----------------------------------
-- Hµm gäi chÝnh
----------------------------------
function main(bExchangeIn)
	LoginDelaySync(0) -- phai co ham nay mem moi thao tac dc trong game, k co thi k lam dc gi het
	AddSkillState(963, 1, 0, 18*3) -- Anti 3s;
	-- Lenh bai Admin
	--if (GetName() == GM_01 or GetName() == GM_02 or GetName() == GM_03 or GetName() == GM_04) then
		if (GetItemCountInBag(7,17,0,0) < 1) then
			nIndex = AddItem(7,17,0,0,0,0,0)
			SetItemBindState(nIndex,2);
		end;
	--end;
	
	local nW,_,_ = GetWorldPos();
	if (nW == 342) then
		NewWorld(341, 1278, 2497)
		local nOldSubWorld = SubWorld;
		SubWorld = SubWorldID2Idx(341);
		SetRevPos(1);
		SetFightState(0);
		LeaveTeam();
		SetPunish(1);
		SetDeathScript("\\script\\event\\tichtinhdong\\player_death.lua");
		Suborld = nOldSubWorld;
	elseif (nW == 341) then
		SetDeathScript("\\script\\event\\tichtinhdong\\player_death.lua");
	elseif (nW >= 337 and nW <= 339) then
		SetDeathScript("");
		SetLogoutScript(""); 
		ForbidChangePK(0);
		SetFightState(0);
		SetPunish(0);
		SetLogoutRV(0)
		SetRevPos(53,19);
		ForbidTownPortal(0);
		SetTask(601,0); -- COUNT JOIN BOAT
	end;
	
	-- Khinh Cong
	if (HaveMagic(210) < 1 ) then
		AddMagic(210,1);
	end
	-- Dua di tu khi PK 10;
		if (GetPK() > 9) then
			SetFightState(0);
			NewWorld(208,1585,3210);
		end;
	-- Vong Sang Loan Chien
	if (GetTitle() == 193) then
		AddSkillState(1584, 20, 1, 24*60*60*18)
	elseif (GetTitle() == 194) then
		AddSkillState(1585, 20, 1, 24*60*60*18)
	elseif (GetTitle() == 195) then
		AddSkillState(1586, 20, 1, 24*60*60*18)
	end;
	---
	--- Clear rank
	if GetTitle() < 100 then
		SetTitle(0);
	end;
	---
	--Lien dau
	if (nW == 396) then 
		SetCurCamp(3);
	end;
	--dau truong
	if (nW == 209) then
		if (GetTask(ZBW.tbTask.SIGN_WORLD) > 0) then
			NewWorld(GetTask(ZBW.tbTask.SIGN_WORLD), GetTask(ZBW.tbTask.SIGN_POSX), GetTask(ZBW.tbTask.SIGN_POSY));
		else
			NewWord(78,1579,3362);
			SetFightState(0);
		end;
	end;
	---
	---
	if (HaveMagic(539) > 0 ) then
		DelMagic(539);
	end
	-- Restore Tien Thao lo
	local nTimeExpEnhance = GetTask(2015)
	if(nTimeExpEnhance > 0) then
		AddSkillState(440, 1, 1, nTimeExpEnhance)
	end
	-- Restore Que hoa tuu
	local nTimeQueHoaTuu = GetTask(2018)
	if(nTimeQueHoaTuu > 0) then
		AddSkillState(450, 1, 1, nTimeQueHoaTuu)
	end
	-- Restore Que hoa tuu
	local nTimeThienSonBaoLo = GetTask(2019)
	if(nTimeThienSonBaoLo > 0) then
		AddSkillState(441, 1, 1, nTimeThienSonBaoLo)
	end
	-- Vong sang ho tro tan thu
	--if (GetLevel() < 80) then
		--AddSkillState(1583, 20, 1, 24*60*60*18)
	--end;
	
	ZPlayer:ResetTask(); -- reset task;
	if (GetLevel() < 2) then
	Msg2SubWorld("Thiªn h¹ ®ån ®o¸n mét cao thñ míi nçi danh hiÖu lµ <color=green>"..GetName().."<color> ®Õn víi "..SYS_NAME_SERVER.."");
	Msg2Player("Chµo mõng ®¹i hiÖp <color=green>"..GetName().."<color> ®Õn víi "..SYS_NAME_SERVER.."!. Phiªn b¶n t¸i hiÖn vâ l©m truyÒn kú nh÷ng n¨m 2005 cña "..SYS_NAME_AUTHOR.."");
	else
	Msg2Player("Chµo mõng ®¹i hiÖp <color=green>"..GetName().."<color> ®Õn víi "..SYS_NAME_SERVER.."!. Phiªn b¶n t¸i hiÖn vâ l©m truyÒn kú nh÷ng n¨m 2005 cña "..SYS_NAME_AUTHOR.."");
	end;
	--Msg2Player("<color=green>Thêi gian ho¹t ®éng<color>\nTèng Kim: 10h30-15h30-20h30\nV­ît ¶i:phót thø 45 giê lÎ | Hoa ®¨ng 11h-14h-17h-22h \nPhong L¨ng ®é: phót thø 45 mçi giê ch¼n\nBoss hoµng kim: 1h-10h-15h-22h\nD· tÈu: C¶ ngµy\nQu¶ hñy hoµng: 12h - Hoµng kim: 13h\nLiªn ®Êu:19h15-19h30-19h45 || Lo¹n chiÕn: 18h55");
	Msg2Player("\n<color=green>Thêi gian ho¹t ®éng: Nót sè 8 d­íi b¶n ®å mini\nThêi gian b¶o tr× ®Þnh kú: 18h00 h»ng ngµy <color> ");
	SendNotification("","<color=yellow>Giftcode: "..SYS_GIFT_CODE.." <color=yellow>");
end

