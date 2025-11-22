
#ifndef KCore_H
#define KCore_H
#ifdef _STANDALONE
#define CORE_API
#else
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif
#define	DIR_DOWN		0
#define	DIR_LEFTDOWN	1
#define	DIR_LEFT		2
#define	DIR_LEFTUP		3
#define	DIR_UP			4
#define	DIR_RIGHTUP		5
#define	DIR_RIGHT		6
#define	DIR_RIGHTDOWN	7
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#ifndef _SERVER
#include "KSpriteCache.h"
#include "KFont.h"
#endif

#include "KTabFile.h"
#include "KProtocol.h"
#include "KEngine.h"
#include "KScriptList.h"
#include "KScriptCache.h"
#include "KSkillManager.h"
#include "MyAssert.H"

class ISkill ;
#ifdef _SERVER
#ifdef _STANDALONE
#include "IServer.h"
#else
#include "../../Headers/IServer.h"
#include "../../Headers/IClient.h"
#endif
#else
#include "../../Headers/IClient.h"
#include "KMusic.h"
#include "KSoundCache.h"
#endif

#pragma warning (disable: 4512)
#pragma warning (disable: 4786)
#define TASKCONTENT
//#define		BMPMAPDEBUG
#ifdef TOOLVERSION
extern CORE_API int g_ScreenX;
extern CORE_API int g_ScreenY;
#endif

#define ITOA(NUMBER)  #NUMBER

#define __TEXT_LINE__(LINE) ITOA(LINE)

//#ifdef _DEBUG
#define ATTENTION(MSG) __FILE__"("__TEXT_LINE__(__LINE__)") : ¡ïATTENTION¡ï ¡ú "#MSG
//#else
//#define ATTENTION(MSG) __FILE__"("__TEXT_LINE__(__LINE__)") : ATTENTION!! error: "#MSG
//#endif

#define	NET_DEBUG
extern CORE_API	KTabFile		g_OrdinSkillsSetting, g_MisslesSetting;
extern CORE_API	KTabFile		g_SkillLevelSetting;
extern CORE_API	KTabFile		g_NpcSetting;
extern CORE_API	KTabFile		g_NpcImageSetting;

#ifndef _SERVER
extern KTabFile					g_RankTabSetting;
extern KIniFile					g_StateIniSetting;
extern KIniFile					g_MapTrafficSetting;
#else
extern KTabFile					g_ReplaceName_Npc;
#endif
extern KTabFile					g_TitleTabSetting;

#ifdef TOOLVERSION
#ifndef __linux
extern CORE_API	KSpriteCache	g_SpriteCache;
#endif
#endif

#ifdef _SERVER

	extern KLuaScript*	g_pStartScript;
	extern int			g_TimerLoop;

#else
	extern KSoundCache		g_SoundCache;
	extern KMusic			*g_pMusic;
	extern unsigned int	* g_pAdjustColorTab;
	extern unsigned int g_ulAdjustColorCount;
#endif

extern CORE_API BOOL			g_bDebugScript;




#ifndef _SERVER
	extern KTabFile		g_cGoldEquip;
#endif

extern KIniFile		g_GameSettingFile;
extern KTabFile		g_QuestStackTabFile;
extern KTabFile		g_MagicScriptStackTabFile;
extern KTabFile		g_GoldMagicTab;
extern KIniFile		g_BattleSettingFile;
extern KTabFile		g_NpcKindFile;

#ifndef _SERVER
extern BOOL g_bUISelIntelActiveWithServer;
extern BOOL g_bUISpeakActiveWithServer;
extern int g_bUISelLastSelCount;
#endif

#ifdef _SERVER

enum DBMESSAGE
{
	DBMSG_PUSH,
	DBMSG_POP,
};

enum DBI_COMMAND
{
	DBI_PLAYERSAVE,
	DBI_PLAYERLOAD,
	DBI_PLAYERDELETE,
	DBI_GETPLAYERLISTFROMACCOUNT,
};
BOOL CORE_API g_AccessDBMsgList(DBMESSAGE Msg,  int* pnPlayerIndex, DBI_COMMAND * pnDBICommand, void ** ppParam1, void ** ppParam2);
extern KLuaScript g_WorldScript;
extern KList g_DBMsgList;


class KDBMsgNode :public KNode
{
public:
	void * pParam1;
	void * pParam2;
	int	   nPlayerIndex;
	DBI_COMMAND Command;
	KDBMsgNode(){pParam1 = pParam2 = NULL; nPlayerIndex =  0;	Command = DBI_PLAYERSAVE;};
};

#endif

#ifndef _SERVER
class KImageNode : public KNode
{
public:
	char	m_szFile[32];
	int		m_nFrame;
	int		m_nXpos;
	int		m_nYpos;
};
#endif

#ifndef _SERVER
extern char* g_GetStringRes(int nStringID, char * szString, int nMaxLen);
#endif

#ifdef _SERVER
void OutputServerInfo();
#endif
BOOL InitGameSetting();
BOOL InitSkillSetting();
BOOL InitMissleSetting();
BOOL InitNpcSetting();
int PositionToRoom(int P);
void g_ReleaseCore();
inline int GetRandomNumber(int nMin, int nMax)
{
	return g_Random(nMax - nMin + 1) + nMin;
}
#ifdef _SERVER
	void g_SetServer(LPVOID pServer);
	//extern IServer* g_pServer;
	extern CORE_API IServer* g_pServer;
#else
	void g_SetClient(LPVOID pClient);
	//extern IClient* g_pClient;
	extern CORE_API IClient* g_pClient;
#endif
#endif
