-- Author:	Kinnox;
-- Date:	17-04-2021
-- Functions: Task System all global;
-- #define 		TASKVALUE_SAVEWAYPOINT_BEGIN		2025
-- #define		TASKVALUE_MAXWAYPOINT_COUNT			3
-- #define 		TASKVALUE_SAVESTATION_BEGIN			2029
-- #define 		TASKVALUE_MAXSTATION_COUNT			32
-- #define 		TV_TIMERTASK_ID						2047
-- #define 		TV_TIMERTASK_RESTTIME				2046
-- #define		TV_BATTLE_GROUP						7
-- #define		TV_LOCKSTATE						2023
-- #define		TV_TRANSLIFE						2022
-- #define		TV_REPUTE							2021
-- #define		TV_FUYUAN							2020
-- #define		TV_ENHANCE_EXP						2015
-- #define		TV_ENHANCE_SKILL					2014
-- #define		TV_CALCDAMAGE						2012
-- #define		TV_SJPOINT							2013
-- #define		TV_FPOINT							2011
-- TV_PRISON_PK		= 5;
--------------------OPEN SERVER------------
GBM_OPEN_SERVER			= 20	-- da su dung de open sv
--------------------TASK-------------------
T_Menpai							 = 	1; --menpai;
T_VLMT								 =  2;
T_TTK								 =  3;
T_PWBOX								 = 2017 -- PASS BOX;
T_REWARD 							 = 903; --Phan Thuong Tan Thu
T_GIFTCODE 							 = 904; --Gift Code NewBie
T_REWARD20							 = 905; --Phan Thuong khi dat cap 20
T_REWARD50 							 = 906; --Phan Thuong khi dat cap 50
T_REWARD80 							 = 907; --Phan Thuong khi dat cap 80
T_REWARD120							 = 908; --Phan Thuong khi dat cap 120
T_REWARDBIKIP8X						 = 909; --Phan Thuong bi kip 8x
T_LIMIT_HUYHOANG					 = 910; --Qua Huy Hoang
T_LIMIT_HOANGKIM					 = 911; --Qua Hoang Kim
T_DENBU								 = 912; -- Den bu; 
T_HOTROOLDMEMBER					 = 913; -- Den bu; 
T_VIP1								 = 914; -- Den bu; 
T_VIP2								 = 915; -- Den bu; 
T_VIP3								 = 916; -- Den bu;
T_EVENT_MAX1						 = 917; -- EVENT BANH TET; 
T_EVENT_MAX2						 = 918; -- EVENT BANH CHUNG;  
T_DROPTIENDONG						 = 919; -- EVENT BANH CHUNG;  
--------------------TASKTEMP---------------
TMP_TGIANMOTUI						 = 4; --open medicine box;
TMP_INDEX_PLAYER					 = 5; --admin manager player;
TMP_INDEX_ADMIN 			 		 = 6; --admin manager player;
TMP_INDEX_TREMBLE 			 		 = 7; --Tremble item
TMP_INDEX_PARACOVER 			 	 = 8; --Particular item
TMP_INDEX_DETAILCOVER 			 	 = 9; --Detail item
-- scrip\lib\admin\role.lua --control item admin
	-- EQUIP_DETAIL		= 8,
	-- EQUIP_PARTICULAR 	= 9,
	-- EQUIP_LEVEL			= 10,
	-- SEX_CHOOSE			= 11,
-------------------TASK BW----------------- STATE 1
-- MAIN 							= 2,
-- STATE 							= 1,
-- MEMBER_COUNT						= 23,
-- SIGN_WORLD 						= 1002,	
-- SIGN_POSX						= 1003,
-- SIGN_POSY						= 1004,
-------------------TASK VUOT AI------------ STATE 4
-- MAIN								= 3	
-- VARV_STATE 						= 4	
-- TASK_DAYLY						= 801;
-- TASK_REGIS_KILLER				= 802;
-- TASK_SUCCES_KILLER				= 803;
-- TASK_ACCEPT_KILLER				= 804;
-------------------TASK DA TAU------------ 
TASK_DOCHI							= 701;
TASK_MATCHI							= 702;
TASK_ACCEPT = 703;
TASK_COUNT_QUEST = 704;
TASK_CANCEL_QUEST = 705;
TASK_KIND = 706;
--TIM DO CHI - MAT CHI
TASK_DT_DOCHI = 707;
TASK_DT_MATCHI = 708;
--DIEM
TASK_DT_PHUCDUYEN = 709;
TASK_DT_DANHVONG = 710;
TASK_DT_TONGKIM = 711;
TASK_DT_LIENDAU = 712;
--DANH QUAI
TASK_DT_DANHQUAI = 713;
--
TASK_SAVEQUEST_1 = 714;
TASK_SAVEQUEST_2 = 715;
TASK_SAVEQUEST_3 = 716;
TASK_SAVEQUEST_3 = 717;
TASK_SAVEQUEST_4 = 718;
TASK_SAVEQUEST_5 = 719;
TASK_SAVEQUEST_6 = 720;
TASK_SAVEQUEST_7 = 721;
TASK_SAVEQUEST_8 = 722;
TASK_DT_TOTAL	= 723;
TASK_DT_REWARD1	= 724;
TASK_DT_REWARD2	= 725;
TASK_DT_REWARD3	= 726;
TASK_DT_CANCEL	= 727;
TASK_DT_REWAR40	= 728;
TASK_DT_REWAR100	= 729;
TASK_DT_REWAR200	= 730;
TASK_DT_REWAR300	= 731;
TASK_DT_REWAR500	= 732;
TASK_DT_REWAR700	= 733;
TASK_DT_REWAR1000	= 734;
TASK_DT_KEY			= 735; -- lenh bai hoan thanh cap toc

------------------TASK THIEN BAO KHO --------------
T_BK_KEY_1			= 736; 
T_BK_KEY_2			= 737; 
T_BK_KEY_3			= 738; 
T_BK_KEY_4			= 739; 
T_BK_KEY_5			= 740; 
T_BK_KEY_DONE_1		= 741; 
T_BK_KEY_DONE_2		= 742; 
T_BK_KEY_DONE_3		= 743; 
T_BK_KEY_DONE_4		= 744; 
T_BK_KEY_DONE_5		= 744; 
T_TIN_SU			= 746;
T_TIN_SU_LB			= 747;
-------------------TASK VAN TIEU (ESCORT)------------
TASK_VANTIEU		= 750; -- Van Tieu main task: [1]=type(4/5/6), [2]=random, [7-9]=boss kills
TASK_NPCVANTIEU		= 751; -- NPC ID of escort cart
TASK_RESET_VANTIEU	= 752; -- Daily reset: [6]=daily runs, [7]=chest loots
-------------------TASK LIEN DAU------------
-------------------TASK PHONG LANG DO------------ 
-- MISSIONID 				= 4
-- STATE					= 1,	-- STATE; [MissionV]
-- TIMER_COUNT_SECCON		= 2, -- count time mission 20 seccons; [MissionV]
-- TIMER_COUNT_MIN			= 3, -- count time mission TIMER_COUNT_SECCON*3; [MissionV]
-- TPLD_JOIN_BOAT 			= 600; -- count join;
-- TPLD_JOIN_MACBAC 		= 601; -- count join;
-------------------TASK TONG KIM------------ 
-------------------TASK VUOT AI------------ 
-- MISSION_VA	 		= 1, 	-- ID mission; 
-- STATE				= 1,	-- STATE; [MissionV]
-- TIMER_COUNT_SECCON	= 2, 	-- count time mission 20 seccons; [MissionV]
-- TIMER_COUNT_MIN		= 3, 	-- count time mission TIMER_COUNT_SECCON*3; [MissionV]
-- BOARD_TIMES			= 4,	-- Count BOARD_TIMES;[MissionV]
-- BOARD_NPC			= 5,	-- Count BOARD_NPC;[MissionV]
-- TVA_JOIN_BOAT 		= 602, -- count join;
-------------------TASK LOAN CHIEN------------ 

-------------------TASK RESET NGAY THANG NAM--------------------
T_DAY = 2000;
T_MONTH = 2001;
T_YEAR = 2002;
-------------------------HOA DANG--------------------------------
T_REPCAUHOI = 888;
T_RIGHTQUESTION = 887;
-------------------------------------------------------------------
-------------------------DOC LAP--------------------------------
T_TETDOCLAP = 0;
T_THOIGIAN5 = 42;
T_NHANMOCTQUOCKHANH = 337;
T_RESTEVENTQUOCKHANH = 338;
T_GIOHOAQUOCKHANH = 221;
T_BOHOAQUOCKHANH = 222;
T_NTBOHOAQUOCKHANH_NAM = 223;
T_NTBOHOAQUOCKHANH_NU  = 224;
T_SONGBAC_BASE = 225;
