----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox;
--	Date: 11/11/2021
--	Desc: Tèng Mé Binh
----------------------------------
Include("\\script\\mission\\battles\\functions.lua")
----------------------------------
--
----------------------------------
function main()
	if (SYS_SWITCH_BATTLES == 0) then
		Talk(1,"","TÝnh n¨ng nµy hiÖn ch­a khai më!");
		return
	end;
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBattle.FIGHT_MAP)
	if(GetMissionV(ZBattle.tbMission.STATE) == 0) then
		Talk(1,"MoreInfo","Qu©n ®éi hiÖn nay ch­a tËp trung vµo chiÕn tr­êng, trong lóc chê ®îi ng­¬i h·y ®i luyÖn tËp thªm.")
		return
	end
	
	if(GetLevel() < 80 ) then
		Say("Ta rÊt tiÕc, chiÕn tr­êng Tèng Kim ®ßi hái cÊp ®é ph¶i ®¹t <color=red> 80 trë lªn<color>. §¼ng cÊp hiÖn t¹i cña ng­¬i qu¸ thÊp, h·y ®i luyÖn tËp thªm!",2, "Ta biÕt råi./OnCancel", "Ta muèn biÕt thªm th«ng tin./MoreInfo");
		return
	end
	
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBattle.FIGHT_MAP)
	local nSongCount = GetMSPlayerCount(ZBattle.tbMission.MAIN, 1)
	local nJinCount = GetMSPlayerCount(ZBattle.tbMission.MAIN, 2)
	Say("ChiÕn tr­êng ®· b¾t ®Çu! HiÖn t¹i bªn <color=sbrown>Tèng: "..nSongCount.."<color> ng­êi, bªn <color=plum>Kim: "..nJinCount.."<color> ng­êi, ng­¬i cã muèn gia nhËp qu©n ®éi ®Ó phôc vô ®Êt n­íc kh«ng?", 3,
			"Gia nhËp qu©n ®éi./CheckCondition",
			"T×m hiÓu th«ng tin./MoreInfo",
			"§Ó ta xem xÐt lai./OnCancel")
	SubWorld = nOldSubWorld	
end
----------------------------------
-- 
----------------------------------
function CheckCondition()
	local nOldSubWorld = SubWorld;
	local bFlag = 0;
	SubWorld = SubWorldID2Idx(ZBattle.FIGHT_MAP)
	if(GetMissionV(ZBattle.tbMission.STATE) >= 2) then
		bFlag = 1;
	end;
	SubWorld = nOldSubWorld;

	-- KiÓm tra phe ®· vµo
	nCamp = 1;
	if (bFlag == 1) then
		local nResult = ZBattle:CheckLastGroup(nCamp)
		if((nResult ~= 0) and (nResult ~= nCamp) ) then
			if (nCamp == 2) then	
				Talk(1,"","Kim quèc ai còng thiÖn chiÕn. Bän ng­¬i sao cã thÓ chèng l¹i chóng ta!")
			else
				Talk(1,"","Bän ng­¬i lµ ng­êi Kim x©m ph¹m giang s¬n ta. Ta thÒ cïng ng­¬i ®ång quy ­ tËn!")		
			end
			return
		end
	end;	
	local nWSeccon = 10;
	TimeBox("§ang kiÓm tra..",nWSeccon,"ExeAnti",0);	
end;
function ExeAnti()
	JoinSong();
end;
----------------------------------
-- 
----------------------------------
function JoinSong()
	local nOldSubWorld = SubWorld
	SubWorld = SubWorldID2Idx(ZBattle.FIGHT_MAP)
	JoinMission(ZBattle.tbMission.MAIN,1)
end

----------------------------------
-- 
----------------------------------
function MoreInfo()
	ZBattle:Info()
end

----------------------------------
-- 
----------------------------------
function OnCancel()
end