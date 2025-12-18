----------------------------------
--	Copyright: JX Online by Kinnox
--	Copyright: Kinnox
--	Date: 16/08/2014
--	Desc: Th­ viªn L«i ®µi C«ng B×nh Tö
----------------------------------
Include("\\script\\player\\head.lua")

--
ZBW 				= {};	-- Kh«ng ®­îc söa
--
ZBW.FRAME2TIME		= 18;	-- Kh«ng ®­îc söa
ZBW.TIMER_1 		= 20 * ZBW.FRAME2TIME;	-- Thêi gian ch¹y bw_small.lua - MÆc ®Þnh: 20 * 18
ZBW.TIMER_2 		= 12 * 3 * ZBW.TIMER_1;	-- Thêi gian ch¹y bw_total.lua - MÆc ®Þnh: 12 * 3 * 20 * 18
ZBW.GO_TIME 		= 6;	-- Thêi gian b¾t ®Çu ®Êu - MÆc ®Þnh: 6	
--
ZBW.SMALLTIMER		= 4;
ZBW.TOTALTIMER		= 5;
--	
ZBW.tbMission = {
	MAIN 			= 6,
	STATE 			= 1,
	TIMER			= 3,
	KEY_TEAM_1		= 4,
	KEY_TEAM_2		= 5,
	KEY				= 6,
	INDEX_GROUP1	= 6,
	INDEX_GROUP2	= 14,
	MEMBER_COUNT	= 23,
}
--
ZBW.tbTask = {
	SIGN_WORLD 	= 22,
	SIGN_POSX	= 23,
	SIGN_POSY	= 24,
	CALC_DAMAGE = 2012,
}
--
ZBW.FIGHT_MAP	= 209;	-- B¶n ®å chiÕn ®Êu
ZBW.tbFightPos	= {
	{1608,3209},
	{1612,3187},
	{1604,3202},
}
ZBW.tbNpc = {
	{1599, 3202, "Tr­¬ng Tam"},
	{1608, 3211, "Lý Tù"},
}
ZBW.sNpcFile = "\\script\\mission\\bw\\transport.lua"
--
ZBW.DEATH_FILE	= "\\script\\mission\\bw\\death.lua"

----------------------------------
-- Rêi khái trËn ®Êu
----------------------------------
function ZBW:LeaveGame()
	ZPlayer:Refresh()
	RestoreOwnFeature()
	ClearDamageCounter()
	SetTask(self.SIGN_WORLD,0)
	SetTask(self.SIGN_POSX,0)
	SetTask(self.SIGN_POSY,0)
	SetTask(self.tbTask.CALC_DAMAGE,0)
	RestoreCamp()
	SetFightState(0)
	SetPunish(0)
	SetCreateTeam(1)
	ForbitTrade(0)
	ForbidChangePK(0)
	SetPKFlag(0)
	ForbidTownPortal(0)
	SetDeathScript("")
end

----------------------------------
-- Rêi khái b¶n ®å
----------------------------------
function ZBW:LeaveMap()
	local nW,nX,nY = GetTask(self.tbTask.SIGN_WORLD),GetTask(self.tbTask.SIGN_POSX),GetTask(self.tbTask.SIGN_POSY)
	NewWorld(nW, nX, nY)
	RestoreOwnFeature();
	ClearStateSkill(1);
	Msg2Player("Ngåi yªn! chóng ta rêi khái chèn giang hå tranh ®Êu nµy!.")
end
