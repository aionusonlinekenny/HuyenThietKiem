// --
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCore.cpp
// Date:	2000.08.08
// Code:	Daphnis Wang
// Desc:	Core class
// --
#include "KCore.h"
#include "KEngine.h"
#include "KFilePath.h"
#ifndef _SERVER
#include "KNpcResList.h"
#include "KBmpFile.h"
#include "ImgRef.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "../KJXPathFinder.h"// find way by kinnox;
#endif
#include "KItemChangeRes.h"
#include "KNpcSet.h"
#include "KTabFile.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KMissleSet.h"
#include "KFaction.h"
#include "KMath.h"
#include "time.h"
#include "KPlayerTask.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"
#include "KObjSet.h"
#include "KItemSet.h"
#include "KNpc.h"
#include "KNpcTemplate.h"
#include "CoreUseNameDef.h"
#include "KBuySell.h"
#include "KSortScript.h"
#include "KTaskFuns.h"
#include "TaskDef.h"
#include "LuaFuns.h"
#include "ZLoadFile.h"

#pragma warning (disable: 4512)
#pragma warning (disable: 4786)

#ifdef _SERVER
#include "KPlayer.h"//add by kinnox;
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
#endif

#ifdef _SERVER
extern int g_WayPointPriceUnit;
extern int g_StationPriceUnit;
extern int g_DockPriceUnit;
#endif


#ifndef _SERVER
#define CLIENTWEAPONSKILL_TABFILE		"\\settings\\ClientWeaponSkill.txt"	
KTabFile g_ClientWeaponSkillTabFile;

#define ADJUSTCOLOR_TABFILE				"\\settings\\AdjustColor.txt"	
unsigned int		InitAdjustColorTab();

unsigned int	* g_pAdjustColorTab = NULL;
unsigned int g_ulAdjustColorCount = 0;
extern KJXPathFinder g_JXPathFinder; // find way by kinnox;
#endif

BOOL	InitTaskSetting();

#ifndef _SERVER
#include "KMagicDesc.h"
#include "Scene/KScenePlaceC.h"
BOOL g_bUISelIntelActiveWithServer = FALSE;
BOOL g_bUISpeakActiveWithServer = FALSE;
int	g_bUISelLastSelCount = 0;
extern KTabFile g_StringResourseTabFile;
KTabFile	g_RankTabSetting;
KIniFile	g_StateIniSetting;
KIniFile	g_MapTrafficSetting;
#else
KTabFile	g_ReplaceName_Npc;
#endif
KTabFile	g_TitleTabSetting;

#ifdef _SERVER
IServer* g_pServer;
extern KTabFile g_EventItemTab;
#else
IClient* g_pClient;
BOOL	 g_bPingReply;
#endif

// --
#ifdef TOOLVERSION
KSpriteCache	g_SpriteCache;
#endif
KTabFile		g_OrdinSkillsSetting, g_MisslesSetting;
KTabFile		g_SkillLevelSetting;
KTabFile		g_NpcSetting;
KTabFile		g_NpcKindFile;
KIniFile		g_GameSettingFile;
KTabFile		g_QuestStackTabFile;
KTabFile		g_MagicScriptStackTabFile;
KTabFile		g_GoldMagicTab;
KIniFile		g_BattleSettingFile;
int				g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
int				g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];	
int				g_nHandSkill;

#ifdef	_SERVER
	KLuaScript * 	g_pStartScript =(KLuaScript*) g_GetScript(TIMER_SERVER_SCRIPT_FILE);
	int				g_TimerLoop = 0;
#else
	KSoundCache		g_SoundCache;
	KMusic			*g_pMusic = NULL;
#endif

CORE_API BOOL			g_bDebugScript;


#ifndef _SERVER
KTabFile	g_cGoldEquip;
#endif

KLuaScript	*	g_pNpcLevelScript = NULL;
KLuaScript g_WorldScript;

#ifdef _SERVER
	tag_DATABASEINFO  tagSQLInfo;
	void	g_InitConfigMssql();
#endif

void g_InitProtocol();
//// ---
// Khoi tao MSSQL
//// ---
#ifdef _SERVER
void	g_InitConfigMssql()
{
	KIniFile cIniFile;
	

	if( cIniFile.Load("database.ini") )
	{	
		char szServerName[32];
		char szUserName[32];
		char szPassword[32];
		char szDataBaste[32];

		cIniFile.GetString("account", "Server", "TLINKVN-24216", szServerName, sizeof(szServerName));
		cIniFile.GetString("account", "User", "sa",	szUserName, sizeof(szUserName));
		cIniFile.GetString("account", "PassWord", "@9&Sd#S73h1@SAcxX", szPassword, sizeof(szPassword));
		cIniFile.GetString("account", "DataBase", "jxSQL", szDataBaste, sizeof(szDataBaste));

		tagSQLInfo.strServer = szServerName;
		tagSQLInfo.strUser = szUserName;
		tagSQLInfo.strPassword = szPassword;
		tagSQLInfo.strDataBase = szDataBaste;
	}

}
#endif 
// --
CORE_API void g_InitCore()
{
	g_InitProtocol();
	g_FindDebugWindow("#32770", "DebugWin");

	#ifdef _SERVER
        g_InitConfigMssql();
	#endif

#ifdef _DEBUG
	g_bDebugScript = 0;
	g_FindDebugWindow("#32770", "DebugWin");
	
	{
		srand((unsigned int) time(NULL));
#ifdef DEBUGOPT_SCRIPT_MSG_FILEOUT
		struct tm *newtime;
		time_t long_time;
		time( &long_time );
		newtime = localtime( &long_time );
		char szFileName[MAX_PATH];
		char szFileName1[MAX_PATH];
		char szPathName[MAX_PATH];
		g_CreatePath("\\DebugData");
		g_CreatePath("\\DebugData\\Script");
			
		GetCurrentDirectory(MAX_PATH, szPathName);

		sprintf(szFileName, "%s\\DebugData\\Script\\ScriptOut_%d%d%d%d%d.txt", szPathName, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
		sprintf(szFileName1, "%s\\DebugData\\Script\\ScriptErr_%d%d%d%d%d.txt", szPathName, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
		lua_setdebugout(szFileName, szFileName1);
#endif
	}

#endif
	srand( (unsigned long)time(NULL) );
#ifndef __linux
	g_RandomSeed(rand());
#else
	g_RandomSeed(clock());
#endif
	time_t ltime;
	time( &ltime );
#ifdef _DEBUG
	//g_DebugLog("Starting Core......%s", ctime( &ltime ));
#endif
	
#ifdef TOOLVERSION	
	g_SpriteCache.Init(256);
#endif

#ifndef _SERVER
	g_bPingReply = TRUE;
	g_SoundCache.Init(256);
	g_SubWorldSet.m_cMusic.Init();
#endif

#ifndef TOOLVERSION
	InitGameSetting();
	g_InitSeries();
    if (!g_InitMath())
    {
    }
	ItemSet.Init();
	ItemGen.Init();
#ifndef _SERVER
	InitAdjustColorTab();
	g_MagicDesc.Init();
#endif
	g_ItemChangeRes.Init();
#endif
	NpcSet.Init();
	ObjSet.Init();
	MissleSet.Init();
	
	g_IniScriptEngine();
	g_OrdinSkillsSetting.Load(SKILL_SETTING_FILE);
	g_MisslesSetting.Load(MISSLES_SETTING_FILE);
	g_NpcSetting.Load(NPC_SETTING_FILE);
	
	InitSkillSetting();
	InitMissleSetting();
	InitNpcSetting();

	InitTaskSetting();

	NpcSet.m_cGoldTemplate.Init();

	if (!PlayerSet.Init())
	{
		printf("Init PlayerSet Error!!!!\n");
	}
	if (!g_LoadFile.Init())
	{
		printf("Init Load File Error!!!!\n");
	}
	
#ifdef _SERVER	
	OutputServerInfo();

	memset(g_TaskGlobalValue, 0, sizeof(g_TaskGlobalValue));
	g_TeamSet.Init();
	char ModuleName[1024];
    GetModuleFileName(NULL, ModuleName, sizeof(ModuleName));
    g_StrLower(ModuleName);
	if (strstr(ModuleName, "gs1.exe")) {
        g_SubWorldSet.Load("\\maps\\WorldSet1.ini");
    } else if (strstr(ModuleName, "gs2.exe")) {
        g_SubWorldSet.Load("\\maps\\WorldSet2.ini");
    } else if (strstr(ModuleName, "gs3.exe")) {
        g_SubWorldSet.Load("\\maps\\WorldSet3.ini");    
    } else if (strstr(ModuleName, "gs4.exe")) {
        g_SubWorldSet.Load("\\maps\\WorldSet4.ini");    
    } else {
        g_SubWorldSet.Load("\\maps\\WorldSet.ini");
    }
	g_WorldScript.Init();
	g_WorldScript.RegisterFunctions(WorldScriptFuns, g_GetWorldScriptFunNum());
		
#endif
	
#ifndef _SERVER
	g_ScenePlace.Initialize();
	g_ScenePlace.RepresentShellReset();// moi them by kinnox 01/08/2023;
#endif
	time(&ltime);
#ifdef _DEBUG
	//g_DebugLog("Initting g_Faction %s",  ctime(&ltime));
#endif
	g_Faction.Init();
	time(&ltime);
	
#ifdef _SERVER
#ifdef _DEBUG
	KIniFile ServerSettingIni;
	if (ServerSettingIni.Load("\\Setting.ini"))
	{
		ServerSettingIni.GetInteger("Setting", "DebugScript", 0, &g_bDebugScript);
	}
#endif
#endif
	
	memset(g_nMeleeWeaponSkill, 0, sizeof(g_nMeleeWeaponSkill));
	memset(g_nRangeWeaponSkill, 0, sizeof(g_nRangeWeaponSkill));
	KTabFile Weapon_PhysicsSkillIdFile;
	if (Weapon_PhysicsSkillIdFile.Load(WEAPON_PHYSICSSKILLFILE))
	{
		int nHeight = Weapon_PhysicsSkillIdFile.GetHeight() - 1;
		int nDetailCol		= Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_DETAILTYPE);
		int	nParticularCol	= Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_PARTICULARTYPE);
		int nPhysicsSkillCol = Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_SKILLID);
		
		for (int i = 0; i < nHeight; i ++)
		{
			int nDetail = 0;
			int nParticular  = 0;
			int nPhysicsSkill = 0;
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nDetailCol, -1, &nDetail);
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nParticularCol, -1, &nParticular);
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nPhysicsSkillCol, -1, &nPhysicsSkill);
			
			if (nDetail == 0 )
			{
				if (nParticular >= 0 && nParticular < MAX_MELEEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nMeleeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == 1)
			{
				if (nParticular >= 0 && nParticular < MAX_RANGEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nRangeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == -1)
			{
				if (nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nHandSkill = nPhysicsSkill;
			}
		}
	}

#ifndef TOOLVERSION
	if (!BuySell.Init())
	{
#ifdef _DEBUG
		//g_DebugLog("Buysell init failed!");
#endif
	}
#endif
#ifdef _DEBUG
	//g_DebugLog("End of Init ,Game Started! %s" , ctime(&ltime));
#endif
}

#ifdef _SERVER
// --
//
// --
void	OutputServerInfo()//show server infor by kinnox;
{	
	printf("Author Source: Streamer Kinnox; - Skype: client.volamprivate@gmail.com; - FB: fb.com/iamkinnox;\n");
	printf("Deverlop By: Kenny Nguyen; - Mails: aionus.online@gmail.com; - FB: fb.com/kennynguyen;\n");
	printf("MAX SUBWORD:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_SUBWORLD,sizeof(KSubWorld),MAX_SUBWORLD,MAX_SUBWORLD*sizeof(KSubWorld)/(1024*1024));
	printf("MAX NPC:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_NPC,sizeof(KNpc),MAX_NPC,MAX_NPC*sizeof(KNpc)/(1024*1024));
	printf("MAX PLAYER:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_PLAYER,sizeof(KPlayer),MAX_PLAYER,MAX_PLAYER*sizeof(KPlayer)/(1024*1024));
	printf("MAX OBJECT:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_OBJECT,sizeof(KObj),MAX_OBJECT,MAX_OBJECT*sizeof(KObj)/(1024*1024));
	printf("MAX ITEM:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_ITEM,sizeof(KItem),MAX_ITEM,MAX_ITEM*sizeof(KItem)/(1024*1024));
	printf("MAX MISSLE:	%d	-MEMMORY	%ld*%ld	=	%ldmb\n", MAX_MISSLE,sizeof(KMissle),MAX_MISSLE,MAX_MISSLE*sizeof(KMissle)/(1024*1024));
	printf("TOTAL MEMMORY: %dmb [This number of memory Knpc + KPlayer + KSubWorld + KItem + KObj + KMissle]\n", (MAX_SUBWORLD*sizeof(KSubWorld)/(1024*1024)) + (MAX_NPC*sizeof(KNpc)/(1024*1024)) + (MAX_PLAYER,MAX_PLAYER*sizeof(KPlayer)/(1024*1024)) + (MAX_OBJECT*sizeof(KObj)/(1024*1024)) + (MAX_ITEM*sizeof(KItem)/(1024*1024)) + (MAX_MISSLE*sizeof(KMissle)/(1024*1024)) );
	printf("EXP RATE:	%d\n", PlayerSet.GetExpRate());
	printf("MONEY RATE:	%d\n", PlayerSet.GetMoneyRate());
	printf("\n\n");
}
#endif

// --
//
// --
BOOL	InitGameSetting()
{
	if(!g_GameSettingFile.Load(GAME_SETTING_INI_FILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[GAME]CAN NOT LOAD Game Setting File %s", GAME_SETTING_INI_FILE);
#endif
	}

	return TRUE;
}


BOOL	InitTaskSetting()
{
	if(!g_QuestStackTabFile.Load(QUESTITEM_STACK_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[GAME]CAN NOT LOAD Game Setting File %s", QUESTITEM_STACK_TABFILE);
#endif
	}
	if(!g_MagicScriptStackTabFile.Load(MAGICSCRIPTITEM_STACK_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[GAME]CAN NOT LOAD Game Setting File %s", MAGICSCRIPTITEM_STACK_TABFILE);
#endif
	}
	if(!g_GoldMagicTab.Load(MAGICATTRIBE_GE_FILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD GoldMagic Setting File %s", MAGICATTRIBE_GE_FILE);
#endif
	}
	if(!g_BattleSettingFile.Load(defBATTLE_SETTING_FILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD Battle Setting File %s", defBATTLE_SETTING_FILE);
#endif
	}
	
#ifdef _SERVER
	if (!g_EventItemTab.Load(QUESTITEM_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD EventItem Setting File %s", QUESTITEM_TABFILE);
#endif
	}
	if (!g_WayPointTabFile.Load(WORLD_WAYPOINT_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_WAYPOINT_TABFILE);
#endif
	}

	if (!g_StationTabFile.Load(WORLD_STATION_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_STATION_TABFILE);
#endif
	}

	if (!g_StationPriceTabFile.Load(WORLD_STATIONPRICE_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD STATION PRICE TabFile %s", WORLD_STATIONPRICE_TABFILE);
#endif
	}

	if (!g_WayPointPriceTabFile.Load(WORLD_WAYPOINTPRICE_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD WAYPOINT PRICE TabFile %s", WORLD_WAYPOINTPRICE_TABFILE);
#endif
	}
	
	if (!g_DockTabFile.Load(WORLD_DOCK_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD DOCK TabFile %s", WORLD_DOCK_TABFILE);
#endif
	}

	if (!g_DockPriceTabFile.Load(WORLD_DOCKPRICE_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD DOCK PRICE TabFile %s", WORLD_DOCKPRICE_TABFILE);
#endif
	}

	int nWidth1 = g_StationPriceTabFile.GetWidth();
	int nHeight1 = g_StationPriceTabFile.GetHeight();
	int nWidth2 = g_WayPointPriceTabFile.GetWidth();
	int nHeight2 = g_WayPointPriceTabFile.GetHeight();
	int nWidth3 = g_DockPriceTabFile.GetWidth();
	int nHeight3 = g_DockPriceTabFile.GetHeight();
	
	
	KASSERT(nHeight1 * nWidth1);
	KASSERT(nHeight2 * nWidth2);
	KASSERT(nHeight3 * nWidth3);
	
	if (nWidth1 * nHeight1)
	{
		g_StationPriceTabFile.GetInteger(1,1, 1, &g_StationPriceUnit);
		g_pStationPriceTab = new int[nWidth1 * nHeight1];
		memset(g_pStationPriceTab, -1, nWidth1 * nHeight1 * sizeof(int));
	}
	
	if (nWidth2 * nHeight2)
	{
		g_WayPointPriceTabFile.GetInteger(1,1,1, &g_WayPointPriceUnit);
		g_pWayPointPriceTab = new int[nWidth2 * nHeight2];
		memset(g_pWayPointPriceTab, -1, nWidth2 * nHeight2 * sizeof(int));
	}
	
	if (nWidth3 * nHeight3)
	{
		g_DockPriceTabFile.GetInteger(1,1,1, &g_DockPriceUnit);
		g_pDockPriceTab = new int[nWidth3 * nHeight3];
		memset(g_pDockPriceTab, -1, nWidth3 * nHeight3 * sizeof(int));
	}
	if (!g_TimerTask.Init())
	{
#ifdef _DEBUG
		//g_DebugLog("Timer Task Init Error!");
#endif
	}

	if (!g_MissionTabFile.Load(TASK_MISSION_SETTING_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[error]Can Not Open %s", TASK_MISSION_SETTING_TABFILE);
#endif
	}

	if (!g_ReplaceName_Npc.Load(REPLACE_NAME_NPC_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD %s", REPLACE_NAME_NPC_TABFILE);
#endif
	}
#endif
#ifndef _SERVER

	if(!g_cGoldEquip.Load(GOLD_EQUIP_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[GAME]CAN NOT LOAD Gold Equip File %s", GOLD_EQUIP_TABFILE);
#endif
	}

	if (!g_StringResourseTabFile.Load(STRINGRESOURSE_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD %s", STRINGRESOURSE_TABFILE);
#endif
	}

	if (!g_RankTabSetting.Load(PLAYER_RANK_SETTING_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD %s", PLAYER_RANK_SETTING_TABFILE);
#endif
	}
	if (!g_StateIniSetting.Load(NPC_STATE_FILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD %s", NPC_STATE_FILE);
#endif
	}
	if (!g_TitleTabSetting.Load(PLAYER_TITLE_SETTING_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("[TASK]CAN NOT LOAD %s", PLAYER_TITLE_SETTING_TABFILE);
#endif
	}
#endif	
	return TRUE;
}

BOOL	InitNpcSetting()
{
	int nNpcTemplateNum = g_NpcSetting.GetHeight() - 1;
	
	memset(g_pNpcTemplate, 0, sizeof(void*) * MAX_NPCSTYLE * MAX_NPC_LEVEL * MAX_SERIES);
	if ( !g_NpcKindFile.Load(NPC_RES_KIND_FILE_NAME) )
	{
#ifdef _DEBUG
		//g_DebugLog("CAN NOT LOAD %s!!!", NPC_RES_KIND_FILE_NAME);
#endif
	}
#ifdef _DEBUG
	//g_DebugLog("NpcTempleSize is %d * %d * %d = %d", sizeof(KNpcTemplate) , nNpcTemplateNum , MAX_NPC_LEVEL, sizeof(KNpcTemplate) * nNpcTemplateNum * MAX_NPC_LEVEL);
#endif	

#ifdef _SERVER
	g_pNpcLevelScript = (KLuaScript*)g_GetScript(NPC_LEVELSCRIPT_FILENAME);
#else
	g_pNpcLevelScript = new KLuaScript;
	g_pNpcLevelScript->Init();
	if (!g_pNpcLevelScript->Load(NPC_LEVELSCRIPT_FILENAME))
	{
#ifdef _DEBUG
		g_DebugLog ("CAN NOT LOAD %s!!!", NPC_LEVELSCRIPT_FILENAME);
#endif
		delete g_pNpcLevelScript;
		g_pNpcLevelScript = NULL;
	}
#endif
	
	if (!g_pNpcLevelScript) 
	{
#ifdef _DEBUG
		//g_DebugLog("CAN NOT LOAD %s!!!", NPC_LEVELSCRIPT_FILENAME);
#endif
	}
	
#ifndef _SERVER
	g_NpcResList.Init();
#endif
	
	return TRUE;
}

BOOL	InitMissleSetting()
{
	int nMissleNum = g_MisslesSetting.GetHeight() - 1;
	
	for (int i = 0; i < nMissleNum; i++)
	{
		int nMissleId = 0;
		g_MisslesSetting.GetInteger(i + 2, "MissleId", -1, &nMissleId);
		
		if (nMissleId > 0)
		{
			g_MisslesLib[nMissleId].GetInfoFromTabFile(i + 2);
			g_MisslesLib[nMissleId].m_nMissleId = nMissleId;
		}
	}
#ifdef _DEBUG
	//g_DebugLog("§· load thµnh c«ng Missle Size %d", sizeof(g_MisslesLib));
#endif
	return TRUE;
}

extern int g_LoadSkillInfo();
BOOL	InitSkillSetting()
{
	
	if (!g_SkillManager.Init())
	{
		_ASSERT(0);
	}

#ifndef _SERVER
	if (!g_ClientWeaponSkillTabFile.Load(CLIENTWEAPONSKILL_TABFILE))
	{
#ifdef _DEBUG
		//g_DebugLog("Can Not Load %s", CLIENTWEAPONSKILL_TABFILE);
#endif
	}
#endif

	return TRUE;
}
#ifdef _SERVER
BOOL	LoadNpcSettingFromBinFile(LPSTR BinFile = NPC_TEMPLATE_BINFILE)
{
	return FALSE;
}

BOOL	SaveAsBinFileFromNpcSetting(LPSTR BinFile = NPC_TEMPLATE_BINFILE)
{
	return FALSE;
}
#endif

// --

void g_ReleaseCore()
{

	int nNpcTemplateNum = g_NpcSetting.GetHeight() - 1;
	unsigned long i = 0;
	unsigned long j = 0;
	unsigned long k = 0;
	for (i = 0; i < nNpcTemplateNum; i++)
	{
		for (j = 0; j < MAX_NPC_LEVEL; j++)
		{	
			for (k = 0; k < MAX_SERIES; k++)
			{	
				if (g_pNpcTemplate[i][j][k])
				{
					delete ((KNpcTemplate *)g_pNpcTemplate[i][j][k]);
					g_pNpcTemplate[i][j][k] = NULL;
				}
			}
		}
	}
	

#ifdef _SERVER
	if (g_pServer)
	{
		g_pServer->Release();
		g_pServer = NULL;
	}
	
	if (g_pStationPriceTab)
	{
		delete [] g_pStationPriceTab;
		g_pStationPriceTab = NULL;
	}

	if (g_pWayPointPriceTab)
	{
		delete [] g_pWayPointPriceTab;
		g_pWayPointPriceTab = NULL;
	}
	
	if (g_pDockPriceTab)
	{
		delete [] g_pDockPriceTab;
		g_pDockPriceTab = NULL;
	}
#else
	g_SubWorldSet.Close();
	g_ScenePlace.RepresentShellReset();// moi them by kinnox 01/08/2023;
	g_ScenePlace.ClosePlace();
	if (g_pNpcLevelScript)
	{
		delete g_pNpcLevelScript;
		g_pNpcLevelScript = NULL;
	}
	if (g_pAdjustColorTab)
	{
		delete []g_pAdjustColorTab;
		g_pAdjustColorTab = NULL;
		g_ulAdjustColorCount = 0;
	}
#endif

    g_UnInitMath();
}

#ifdef _SERVER
void g_SetServer(LPVOID pServer)
{
	g_pServer = reinterpret_cast< IServer * >(pServer);
}
#endif

#ifndef _SERVER
void g_SetClient(LPVOID pClient)
{
	g_pClient = reinterpret_cast< IClient * >(pClient);
}

unsigned int	InitAdjustColorTab()
{
	g_pAdjustColorTab = NULL;
	g_ulAdjustColorCount = 0;
	KTabFile TabFile;
	if (!TabFile.Load(ADJUSTCOLOR_TABFILE))
	{
		_ASSERT(0);
#ifdef _DEBUG
		//g_DebugLog("CAN NOT LOAD %s!!!", ADJUSTCOLOR_TABFILE);
#endif
		return 0;
	}

	int nHeight = TabFile.GetHeight() - 1;
	
	if (nHeight <= 0)
		return 0;

	g_pAdjustColorTab = (unsigned int *)new unsigned long [nHeight];
	g_ulAdjustColorCount = nHeight;

	for (int i = 0; i < nHeight; i ++)
	{
		BYTE bAlpha = 0;
		BYTE bRed	= 0;
		BYTE bGreen	= 0;
		BYTE bBlue	= 0;
		int nAlpha;
		int nRed;
		int nGreen;
		int nBlue;
		TabFile.GetInteger(i + 2, "ALPHA", 0x000000ff, &nAlpha);
		nAlpha	&= 0xff;
		TabFile.GetInteger(i + 2,"RED", 0, &nRed);
		nRed	&= 0xff;
		TabFile.GetInteger(i + 2,"GREEN",  0, &nGreen);
		nGreen	&= 0xff;
		TabFile.GetInteger(i + 2,"BLUE",  0, &nBlue);
		nBlue	&= 0xff;
		unsigned long ulAdjustColor = nAlpha << 24 | nRed << 16 | nGreen << 8 | nBlue;
		g_pAdjustColorTab[i] = ulAdjustColor;
	}
	return g_ulAdjustColorCount;
}
#endif
int PositionToRoom(int P)
{
	switch (P)
	{
	case pos_hand:
	case pos_equip:
	case pos_equiproom:
		return room_equipment;
	case pos_expandtoryroom1:
		return room_expandtory1;
	case pos_repositoryroom:
		return room_repository;
	case pos_repositoryroom1:
		return room_repository1;
	case pos_repositoryroom2:
		return room_repository2;
	case pos_repositoryroom3:
		return room_repository3;
	case pos_repositoryroom4:
		return room_repository4;
	case pos_repositoryroom5:
		return room_repository5;
	case pos_traderoom:
		return room_trade;
	case pos_trade1:
		return room_trade1;
	case pos_immediacy:
		return room_immediacy;
	//TamLTM kham nam
	case pos_builditem:
		return room_builditem;
	//end

	default:
		return room_equipment;
	}
	return room_equipment;
}