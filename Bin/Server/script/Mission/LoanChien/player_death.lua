--Author: Kinnox;
--Date: 16/05/2022;
--function: death_player;
Include("\\script\\mission\\loanchien\\head.lua");
Include("\\script\\lib\\libevent.lua");
function OnDeath(nLauncher,nAttacker)
	--mutil ben thuyen;
	local OldSubWorld = SubWorld;
	local OldPlayerIndex = PlayerIndex;
	local szNameLauncher = "";
	SubWorld = SubWorldID2Idx(LC.MAIN.MAP_FIGHT);
	PlayerIndex = nLauncher; -- index nguoi chet;
	szNameLauncher = GetName();
	if (GetTask(LC.TASK.TCOUNT_DEATH) > 0) then
		SetTask(LC.TASK.TCOUNT_DEATH,GetTask(LC.TASK.TCOUNT_DEATH) - 1 );
		RestoreLife();
		RestoreMana();
		Msg2Player("SË l«n hÂi sinh cﬂn lπi lµ : <color=blue>"..GetTask(LC.TASK.TCOUNT_DEATH).."<color>");
		local nTopKiller = GetMSPlayerCount(LC.MAIN.MISSION_LC,1);
		if (nTopKiller == 3) then
			SetTitle(0);
			ClearStateSkill(1);
			SetTitle(195);
			AddSkillState(1586, 20, 1, 24*60*60*18)
			Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 3 Loπn chi’n giang hÂ");
		elseif (nTopKiller == 2) then
			SetTitle(0);
			ClearStateSkill(1);
			SetTitle(194);
			AddSkillState(1585, 20, 1, 24*60*60*18)
			Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 2 Loπn chi’n giang hÂ");
		elseif (nTopKiller == 1) then
			SetTitle(0);
			ClearStateSkill(1);
			SetTitle(193);
			AddSkillState(1584, 20, 1, 24*60*60*18)
			Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 1 Loπn chi’n giang hÂ");
		end;	
	else
		LC:LeaveGame(PlayerIndex,2);
	end;
	PlayerIndex = nAttacker; -- tra index danh chet;
	Msg2MSAll(LC.MAIN.MISSION_LC, " <color=cyan>ßπi hi÷p <color=orange>"..GetName().."<color> dÔng tuy÷t k¸ hπ gÙc  <color=orange>"..szNameLauncher.."<color> .<color>");
	RestoreLife();
	RestoreMana();
	--[phan thuong 3 top]
	local nTopKiller = GetMSPlayerCount(LC.MAIN.MISSION_LC,1);
	if (nTopKiller == 3) then
		SetTitle(0);
		ClearStateSkill(1);
		SetTitle(195);
		AddSkillState(1586, 20, 1, 24*60*60*18)
		Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 3 Loπn chi’n giang hÂ");
	elseif (nTopKiller == 2) then
		SetTitle(0);
		ClearStateSkill(1);
		SetTitle(194);
		AddSkillState(1585, 20, 1, 24*60*60*18)
		Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 2 Loπn chi’n giang hÂ");
	elseif (nTopKiller == 1) then
		SetTitle(0);
		ClearStateSkill(1);
		SetTitle(193);
		AddSkillState(1584, 20, 1, 24*60*60*18)
		Msg2Player("ßπi hi÷p Æ∑ r t xu t sæc nhÀn Æ≠Óc Top 1 Loπn chi’n giang hÂ");
	end;	
	--	
	AddEventLoanChien();
	PlayerIndex = OldPlayerIndex;
	SubWorld = OldSubWorld;
end

function OnRevive(nLauncher)
	RestoreLife();
	RestoreMana();
end;