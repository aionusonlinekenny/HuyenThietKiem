----------------------------------
--	Copyright: JX Online by Kinnox
--	Author: Kinnox
--	Date: 16/08/2014
--	Desc: Th­ viÖn m«n ph¸i
----------------------------------
Include("\\script\\lib\\TaskLib.lua")
Faction = {}
Faction.SexName = {"S­ ®Ö", "S­ muéi"}
Faction.BASE_SKILL_LEVEL = 0
--
Faction.LEAVE_LEVEL	 = 60
Faction.LEAVE_COST	 = 10000
Faction.RETURN_COST	 = 10000
--
Faction.TFACTION_INFO = {
	[1] = {
		sIDName = "shaolin",
		sFactionName = "ThiÕu L©m ph¸i",
		tbSkillBase = {10,14,4,6,8,15,16,20,271,11,19,273,21},
		tbSkill90 = {
			{"§¹t Ma §é Giang", 318, 20},
			{"Hoµnh T¶o Thiªn Qu©n", 319, 20},
			{"V« T­íng Tr¶m", 321, 20}
		},
		tbSkill120 = {"§¹i Thõa Nh­ Lai Chó ",709, 20},
		nCamp = 1,
		nRankID = 72,
	},
	[2] = {
		sIDName = "tianwang",
		sFactionName = "Thiªn V­¬ng Bang",
		tbSkillBase = {34,30,29,23,24,26,33,37,35,31,40,42,36,32,41,324},
		tbSkill90 = {
			{"Ph¸ Thiªn Tr¶m", 322,20},
			{"Truy Tinh Trôc NguyÖt", 323,20},
			{"Truy Phong QuyÕt", 325,20}
		},
		tbSkill120 = {"§¶o H­ Thiªn",708, 20},
		nCamp = 3,
		nRankID = 79,
	},
	[3] = {
		sIDName = "tangmen",
		sFactionName = "§­êng M«n",
		tbSkillBase = {45,43,347,303,50,54,47,343,345,349,249,58,341,48},
		tbSkill90 = {
			{"Cöu Cung Phi Tinh", 342,20},
			{"NhiÕp Hån NguyÖt ¶nh", 339,20},
			{"B¹o Vò Lª Hoa", 302,20},
			{"Lo¹n Hoµn KÝch", 351,0},
		},
		tbSkill120 = {"Mª ¶nh Tung",710, 20},
		tbSkill150 = {
			{"§¹i Lùc Kim Cang Ch­ëng", 1069, 20},
			{"Vi §µ HiÕn Xö", 1070, 20},
			{"Tam Giíi Quy ThiÒn", 1071, 20}
		},
		nCamp = 3,
		nRankID = 76,
	},
	[4] = {
		sIDName = "wudu",
		sFactionName = "Ngò §éc Gi¸o",
		tbSkillBase = {63,65,62,60,67,70,66,68,384,64,69,356,73,72,75,71,74},
		tbSkill90 = {
			{"¢m Phong Thùc Cèt", 353, 20},
			{"HuyÒn ¢m Tr¶m", 355, 20},
			{"§o¹n C©n Hñ Cèt", 390, 0}
		},
		tbSkill120 = {"HÊp Tinh YÓm",711, 20},
		nCamp = 2,
		nRankID = 80,
	},
	[5] = {
		sIDName = "emei",
		sFactionName = "Nga My ph¸i",
		tbSkillBase = {85,80,77,79,93,385,82,89,86,92,252,88,91,282},
		tbSkill90 = {
			{"Tam Nga TÒ TuyÕt", 328, 20},
			{"Phong S­¬ng To¸i ¶nh", 380, 20},
			{"Phæ §é Chóng Sinh", 332, 20}
		},
		tbSkill120 = {"BÕ NguyÖt PhÊt TrÇn",712, 20},
		nCamp = 1,
		nRankID = 74,
	},
	[6] = {
		sIDName = "cuiyan",
		sFactionName = "Thóy Yªn M«n",
		tbSkillBase = {99,102,95,97,269,105,113,100,109,114,108,111},
		tbSkill90 = {
			{"B¨ng Tung V« ¶nh", 336, 20},
			{"B¨ng T©m Tiªn Tö ", 337, 20},
		},
		tbSkill120 = {"Ngù TuyÕt Èn",713, 20},
		nCamp = 3,
		nRankID = 77,
	},
	[7] = {
		sIDName = "gaibang",
		sFactionName = "C¸i Bang",
		tbSkillBase = {122,119,116,115,129,274,124,277,128,125,130,360},
		tbSkill90 = {
			{"Phi Long T¹i Thiªn", 357,20},
			{"Thiªn H¹ V« CÈu", 359,20},
		},
		tbSkill120 = {"Hçn Thiªn KhÝ C«ng",714, 20},
		tbSkill150 = {
			{"§¹i Lùc Kim Cang Ch­ëng", 1073, 20},
			{"Vi §µ HiÕn Xö", 1074, 20},
		},
		nCamp = 1,
		nRankID = 78,
	},
	[8] = {
		sIDName = "tianren",
		sFactionName = "Thiªn NhÉn Gi¸o",
		tbSkillBase = {135,145,132,131,136,137,141,138,140,364,143,150,142,148},
		tbSkill90 = {
			{"V©n Long KÝch", 361,20},
			{"Thiªn Ngo¹i L­u Tinh", 362,20},
			{"NhiÕp Hån Lo¹n T©m", 391,0}
		},
		tbSkill120 = {"Ma ¢m PhÖ Ph¸ch",715, 20},
		tbSkill150 = {
			{"§¹i Lùc Kim Cang Ch­ëng", 1075, 20},
			{"Vi §µ HiÕn Xö", 1076, 20},
		},
		nCamp = 2,
		nRankID = 81,
	},
	[9] = {
		sIDName = "wudang",
		sFactionName = "Vâ §ang ph¸i",
		tbSkillBase = {153,155,152,151,159,164,158,160,157,166,165,267},
		tbSkill90 = {
			{"Thiªn §Þa V« Cùc", 365, 20},
			{"Nh©n KiÕm Hîp NhÊt", 368, 20},
		},
		tbSkill120 = {"XuÊt ø BÊt NhiÔm",716, 20},
		nCamp = 1,
		nRankID = 73,
	},
	[10] = {
		sIDName = "kunlun",
		sFactionName = "C«n L«n ph¸i",
		tbSkillBase = {169,179,167,168,174,171,392,178,172,393,173,175,181,176,182,90,275,630},
		tbSkill90 = {
			{"Ng¹o TuyÕt Tiªu Phong", 372, 20},
			{"L«i ®éng Cöu Thiªn", 375, 20},
			{"Tóy Tiªn T¸ Cèt", 394, 0}
		},
		tbSkill120 = {"L­ìng Nghi Ch©n KhÝ",717,20},
		nCamp = 3,
		nRankID = 75,
	},
	[11] = {
		sIDName = "huashan",
		sFactionName = "Hoa S¬n ph¸i",
		tbSkillBase = {1347,1372,1349,1374,1350,1375,1351,1376,1354,1378,1355,1379,1358,1360,1380},
		tbSkill90 = {
			{"Th¸i Nh¹c Tam Thanh Phong", 1363, 20},
			{"Ph¸ch Th¹ch Ph¸ Ngäc", 1382, 20},
		},
		tbSkill120 = {"Tö Hµ KiÕm KhÝ",1365, 20},
		nCamp = 3,
		nRankID = 75,
	},	
}
--
Faction.TRANSLIFE = {
	[1] = {0,50,100,150,200,250},
	[2] = {0,10,20,30,40,50},	
}

----------------------------------
-- Gia nhËp m«n ph¸i
----------------------------------
function Faction:JoinFaction(nNo)
	if(type(nNo) ~= "number") then
		return 0
	end
	if(nNo < 1 or nNo > getn(self.TFACTION_INFO)) then
		return 0
	end
	SetFaction(self.TFACTION_INFO[nNo].sIDName)
	for i = 1, getn(self.TFACTION_INFO[nNo].tbSkillBase) do
		AddMagic(self.TFACTION_INFO[nNo].tbSkillBase[i],self.BASE_SKILL_LEVEL)
	end
--	SetRank(self.TFACTION_INFO[nNo].nRankID)
	Msg2Player("Tin Ph¸i","B¹n ®· gia nhËp "..self.TFACTION_INFO[nNo].sFactionName..".")
	return 1
end

----------------------------------
-- XuÊt s­ khái ph¸i
----------------------------------
function Faction:LeaveFaction(nNo)
	if(GetLevel() < self.LEAVE_LEVEL) then
		Talk(1,"", 10020)
		return 0
	end
	local nCash = GetCash()
	if (nCash < self.LEAVE_COST) then
		Talk(1,"", 10021)
		return 0
	end
	Pay(self.LEAVE_COST)
	if(GetCash() == nCash - self.LEAVE_COST) then
		local n = GetTask(0)
		SetTask(0, SetByte(n,1,nNo))
		SetCamp(4)
		SetCurCamp(4)
		Msg2Player("Tin Ph¸i","B¹n ®· rêi khái "..self.TFACTION_INFO[nNo].sFactionName..".")
		return 1
	else
		Talk(1, "", 10002)
	end
	return 0
end

----------------------------------
-- Quay vÒ m«n ph¸i
----------------------------------
function Faction:ReturnFaction(Sel,nNo)
	local nCash = GetCash()
	if (nCash < self.RETURN_COST) then
		Talk(1,"", 10023)
		return 0
	end
	Pay(self.RETURN_COST)
	if(GetCash() == nCash - self.RETURN_COST) then
		local n = GetTask(0)
		SetTask(0, SetByte(n,1,0))
		SetCurCamp(self.TFACTION_INFO[nNo].nCamp)
		SetCamp(self.TFACTION_INFO[nNo].nCamp) 
--		SetRank(self.TFACTION_INFO[nNo].nRankID)
		Msg2Player("Tin Ph¸i","B¹n ®· quay l¹i "..self.TFACTION_INFO[nNo].sFactionName..".")
		return 1
	else
		Talk(1, "", 10002)
		return 0
	end
end
--*--
function OnClearProp()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy tiÒm n¨ng? ",2, 
		"TÈy tiÒm n¨ng./OnClearPropCore", 
		"Kh«ng tÈy./OnCanCel")
end


function OnClearPropCore()
	Faction:ResetAttr()
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end


function OnClearMagic()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy kü n¨ng? ",2, 
	"TÈy kü n¨ng./OnClearMagicCore", 
	"Kh«ng tÈy./OnCanCel")
end


function OnClearMagicCore()
	Faction:ResetMagic(0)
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end


function OnClearAll()
	Say("Ng­¬i cã ch¾c r»ng muèn tÈy tÊt c¶? ",2, "TÈy tÊt c¶./OnClearAllCore", "Kh«ng tÈy./OnCanCel")
end


function OnClearAllCore()
	Faction:ResetAttr()
	Faction:ResetMagic(0)
	Talk(1,"","Kinh m¹ch ®· ®­îc ®¶ th«ng, h·y suy xÐt thËt kü.")
end
--*--
function Faction:ResetAttr()
	local nSeries = GetSeries() -- Thuéc tÝnh ngò hµnh cña nh©n vËt
	local nCurTL = GetTransLife() + 1 -- Sè lÇn trïng sinh
	local nPoint = GetTask(T_TTK) + self.TRANSLIFE[1][nCurTL] -- §iÓm tiÒm n¨ng
	ResetBaseAttribute(nSeries)
	AddProp(nPoint)
end

--*--
function Faction:ResetMagic(bIsTransLife)
	local i = HaveMagic(210)		-- Tr¶ vÒ cÊp kü n¨ng khinh c«ng
	local n = RollbackSkill()		-- TÈy hÕt kü n¨ng vµ tr¶ vÒ sè ®iÓm kü n¨ng
	local x = 0
	if(i ~= -1) then x = x + i end		--Chªnh lÖch ®iÓm kü n¨ng khi cã c¸c kü n¨ng ngoµi
	n = n - x		-- Tr¶ vÓ ®iÓm kü n¨ng ®óng Êp
	if( (n + GetMagicPoint() ) < 0) then
		n = -1 + GetMagicPoint()
	end
	local nExPoint = GetTask(T_VLMT)		-- §iÓm vâ l©m mËt tÞch
	local nMaxPoint = GetLevel() - 1		--> §iÓm kü n¨ng theo level
	if(n > nMaxPoint) then
		n = nMaxPoint
	end
	n = n + nExPoint
	AddMagicPoint(n)
	if(i ~= -1) then AddMagic(210,i) end		-- Tr¶ l¹i kü n¨ng khinh c«ng
end


----------------------------------
--	Tho¸t khái ®èi tho¹i
----------------------------------
function OnCancel()
end