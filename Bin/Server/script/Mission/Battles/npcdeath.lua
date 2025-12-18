----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Qu¸i Tèng Kim chÕt
----------------------------------
Include("\\script\\mission\\battles\\functions.lua")

ITEM_DROPRATE_TABLE = { 
	{ { 7,81,1,1,-1,0 }, { 0.003,0.0200,0.0520,0.0400,0.0500,0.0600 } },	-- ChiÕn cæ 
	{ { 7,82,1,1,-1,0 }, { 0.003,0.0200,0.0520,0.0400,0.0500,0.0600 } },	-- LÖnh bµi
	{ { 7,83,1,1,-1,0 }, { 0.003,0.0200,0.0300,0.0400,0.0500,0.0600 } },	-- Binh SÜ hiÖu phï 
	{ { 7,85,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0360 } },	-- Kim Chi ChiÕn Hån 
	{ { 7,86,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0360 } },	-- Méc Chi ChiÕn Hån 
	{ { 7,87,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0360 } },	-- Thñy Chi ChiÕn Hån 
	{ { 7,88,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0360 } },	-- Háa Chi ChiÕn Hån 
	{ { 7,89,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0360 } },	-- Thæ Chi ChiÕn Hån 
	{ { 7,90,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0120 } },	-- Kim Chi Hé gi¸p  
	{ { 7,91,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0120 } },	-- Méc Chi Hé gi¸p  
	{ { 7,92,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0120 } },	-- Thñy Chi Hé gi¸p  
	{ { 7,93,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0120 } },	-- Háa Chi Hé gi¸p  
	{ { 7,94,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0376,0.0120 } },	-- Thæ Chi Hé gi¸p  
	{ { 7,95,1,1,-1,0 }, { 0.002,0.0360,0.0180,0.0390,0.0140,0.0360 } },	-- Kim Chi Lîi NhËn
	{ { 7,96,1,1,-1,0 }, { 0.002,0.0360,0.0180,0.0390,0.0140,0.0360 } },	-- Méc Chi Lîi NhËn
	{ { 7,97,1,1,-1,0 }, { 0.002,0.0360,0.0180,0.0390,0.0140,0.0360 } },	-- Thñy Chi Lîi NhËn
	{ { 7,98,1,1,-1,0 }, { 0.002,0.0360,0.0180,0.0390,0.0140,0.0360 } },	-- Háa Chi Lîi NhËn
	{ { 7,99,1,1,-1,0 }, { 0.002,0.0360,0.0180,0.0390,0.0140,0.0360 } },	-- Thæ Chi Lîi NhËn
	{ { 7,100,1,1,-1,0 }, { 0.002,0.0200,0.0400,0.0390,0.0140,0.0120 } },	-- Hµnh qu©n ®¬n 
	{ { 7,101,1,1,-1,0 }, { 0.002,0.0200,0.0400,0.0390,0.0140,0.0120 } },	-- TiÓu hoµn ®¬n 
	{ { 7,102,1,1,-1,0 }, { 0.002,0.0200,0.0400,0.0390,0.0140,0.0120 } },	-- Ngò Hoa Lé 
	{ { 7,103,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Ngo¹i Phæ hoµn
	{ { 7,104,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Ngo¹i §éc hoµn
	{ { 7,105,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Ngo¹i B¨ng hoµn
	{ { 7,106,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Néi Phæ hoµn
	{ { 7,107,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Néi §éc hoµn
	{ { 7,108,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Néi B¨ng hoµn
	{ { 7,109,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Néi Háa hoµn
	{ { 7,110,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Néi L«i hoµn
	{ { 7,111,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Tr­êng MÖnh hoµn
	{ { 7,112,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Gia Bµo hoµn
	{ { 7,113,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Cao ThiÓm hoµn
	{ { 7,114,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Cao Trung hoµn
	{ { 7,115,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Phi Tèc hoµn
	{ { 7,116,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n Phæ Phßng hoµn
	{ { 7,117,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Phong V©n B¨ng Phßng hoµn
	{ { 7,118,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Chuyªn dïng cho Phong V©n L«i phßng hoµn
	{ { 7,119,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Chuyªn dïng cho Phong V©n Háa phßng hoµn
	{ { 7,120,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Chuyªn dïng cho Phong V©n §éc phßng hoµn
	{ { 7,121,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0360 } },	-- TËt phong ngoa 	
	{ { 7,123,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0360 } },	-- B¹ch C©u Hé uyÓn	
	{ { 7,124,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- HËu NghÖ Chi Nh·n 	
	{ { 7,125,1,1,-1,0 }, { 0.002,0.0200,0.0130,0.0160,0.0140,0.0120 } },	-- ThÇn bÝ vËt phÈm 
	{ { 7,122,1,1,-1,0 }, { 0.002,0.0200,0.0180,0.0160,0.0140,0.0120 } },	-- Tam Thanh Chi KhÝ 
	{ { 7,126,1,1,-1,0 }, { 0.003,0.0200,0.0150,0.0200,0.0200,0.0200 } },	-- Bå c©u 
	{ { 7,127,1,1,-1,0 }, { 0.003,0.0200,0.0520,0.0200,0.0200,0.0200 } },	-- Kh¸ng §¬n Chi Gi¸c 
  }
					  
NPC_RANK_DROPRATE_TABLE = { 1, 1, 2, 3, 4, 5 }

----------------------------------
--
----------------------------------
function OnDeath(nNpcIdx,nLastDamage)
	local nPlayerIndex = NpcIdx2PIdx(nLastDamage);
	PlayerIndex = nPlayerIndex;
	
	if(GetMissionV(ZBattle.tbMission.STATE) ~= ZBattle.STATE_FIGHT) then
		return
	end
	if( (PlayerIndex == nil) or (PlayerIndex == 0) ) then
		return
	end
	dropItem(nNpcIdx, nRank, PlayerIndex);
	local nCamp = GetCurCamp()
	SetTaskTemp(ZBattle.tbTaskTmp.ANTI_POST,0)
	SetTask(ZBattle.tbTask.KILL_NPC, GetTask(ZBattle.tbTask.KILL_NPC) + 1)
	if(nRank < 6) then
		SetTask(ZBattle.tbTask.NPC_POINT, GetTask(ZBattle.tbTask.NPC_POINT) + ZBattle.tbNpcPoint[nRank])
		ZBattle:AddTotalPoint(ZBattle.tbNpcPoint[nRank])
		ZBattle:AddMissionPoint(nRank, nCamp)
	elseif(nRank == 6) then -- loi tong kim chua fix
		local i = GetTaskTemp(ZBattle.tbTaskTmp.TITLE)
		local nRate = 0
		if(nCamp == 1) then
			nRate,_ = ZBattle:BonusRate()
			nRate = floor(nRate * ZBattle.BOSS_BONUS_POINT)
			Msg2MSAll(ZBattle.tbMission.MAIN,"<color=sbrown>Tèng<color> "..ZBattle.tbRankName[i][1].."<color=wood>"..GetName().." <color>®· h¹ gôc <color=plum>Nguyªn So¸i Kim<color>.")
			SetMissionV(ZBattle.tbMission.MARSHAL_DEATH,1)
		else
			_,nRate = ZBattle:BonusRate()
			nRate = floor(nRate * ZBattle.BOSS_BONUS_POINT)
			Msg2MSAll(ZBattle.tbMission.MAIN,"<color=plum>Kim<color> "..ZBattle.tbRankName[i][1].."<color=wood>"..GetName().." <color>®· h¹ gôc <color=sbrown>Nguyªn So¸i Tèng<color>.")
			SetMissionV(ZBattle.tbMission.MARSHAL_DEATH,2)
		end
		ZBattle:AddTotalPoint(nRate)
		ZBattle:AddMissionPoint(nRate, nCamp)
		ZBattle:UpdateTitle(0,nCamp)
		CloseMission(ZBattle.tbMission.MAIN)
		UpdateTopTKNew();
		return
	end
	SyncTask(ZBattle.tbTask.KILL_NPC)
	SyncTask(ZBattle.tbTask.NPC_POINT)
	ZBattle:UpdateTitle(0,nCamp)
	UpdateTopTKNew();
end

function OnRevive(nNpcIdx)

end;
----------------------------------
--
----------------------------------
function dropItem(nNpcIndex, nNpcRank, nBelongPlayerIdx)
	local nItemCount = getn(ITEM_DROPRATE_TABLE)
	local nMpsX, nMpsY, nSubWorldIdx = GetNpcPos(nNpcIndex)
	for nDropTimes = 1, NPC_RANK_DROPRATE_TABLE[nNpcRank] do
		local nRandNum = random();
		local nSum = 0;
		for i = 1, nItemCount do
			nSum = nSum + ITEM_DROPRATE_TABLE[i][2][nNpcRank];
			if( nSum > nRandNum ) then
				DropItem(nSubWorldIdx, nMpsX, nMpsY, nBelongPlayerIdx, ITEM_DROPRATE_TABLE[i][1][1], ITEM_DROPRATE_TABLE[i][1][2], ITEM_DROPRATE_TABLE[i][1][3], ITEM_DROPRATE_TABLE[i][1][4], ITEM_DROPRATE_TABLE[i][1][5], ITEM_DROPRATE_TABLE[i][1][6])
				break
			end
		end
	end
	-- DropItem(nSubWorldIdx, nMpsX, nMpsY, nBelongPlayerIdx, ITEM_DROPRATE_TABLE[5][1][1], ITEM_DROPRATE_TABLE[5][1][2], ITEM_DROPRATE_TABLE[5][1][3], ITEM_DROPRATE_TABLE[5][1][4], ITEM_DROPRATE_TABLE[5][1][5], ITEM_DROPRATE_TABLE[5][1][6],0)
end