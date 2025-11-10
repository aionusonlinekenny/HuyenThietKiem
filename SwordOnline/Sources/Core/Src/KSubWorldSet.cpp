
#include "KCore.h"
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KNpc.h"
#include "KIniFile.h"
#include "KSubWorldSet.h"
#include "KMissleSet.h"
#include "LuaFuns.h"
#include "KNpcTemplate.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KPakList.h"

extern KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL][MAX_SERIES];		//

KSubWorldSet g_SubWorldSet;

// --
//
// --
KSubWorldSet::KSubWorldSet()
{
	m_nLoopRate = 0;
	m_nGameVersion = ITEM_VERSION;
#ifndef _SERVER
	m_dwPing = 0;
#endif
}

// --
//
// --
int KSubWorldSet::SearchWorld(DWORD dwID)
{
	for (int i = 0; i < MAX_SUBWORLD; i++)
	{
		if ((DWORD)SubWorld[i].m_SubWorldID == dwID)
			return i;
	}
	return -1;
}

#ifdef _SERVER
// --

// --
BOOL KSubWorldSet::Load(LPSTR szFileName)
{
	KIniFile	IniFile;
	char		szKeyName[32];
	int			nWorldID;
	int			nWorldCount = 0;
	
	KPakList	cPakList;
	if( !cPakList.Open("\\package.ini") )
		return FALSE;

	if ( !IniFile.Load(szFileName) )
		return FALSE;

	IniFile.GetInteger("Init", "Count", 1, &nWorldCount);
	if (nWorldCount > MAX_SUBWORLD)
		return FALSE;
	
	printf("---------------------------------------\n");
	printf("--SubWorld Count: %d\n", nWorldCount);
	printf("---------------------------------------\n");
	int i = 0;
	for (i = 0; i < nWorldCount; i++)
	{	
		sprintf((char*)szKeyName, "World%02d", i);
		IniFile.GetInteger("World", szKeyName, 1000, &nWorldID);
		SubWorld[i].m_nIndex = i;
		if (!SubWorld[i].LoadMap(nWorldID))
		{
			printf("SubWorld: %d - WorldID: %d Load Failed\n", i, nWorldID);
		}

	}
	cPakList.Close();
	printf("---------------------------------------\n");
	return TRUE;
}
#endif

// --
//
// --
int nActiveRegionCount;

// --
//
// --
void KSubWorldSet::MainLoop()
{
	m_nLoopRate++;

#ifndef _SERVER
	this->m_cMusic.Play(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);
#endif

	nActiveRegionCount = 0;
	int i = 0;
	for (i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].Activate();
#ifndef _SERVER
		NpcSet.CheckBalance();
#endif
	}

#ifdef _SERVER
	PlayerSet.AutoSave();
	
	
	if(!(m_nLoopRate % (10 * 60 * 20)))
	{	
		BYTE btCount = 0;
		i = 0;
		while(1)
		{	
			if(btCount >= defMAX_CITY)
				break;
			if(i >= MAX_SUBWORLD)
				break;

			i++;
		}
	}
	//khong biet cai nay de lam gi by kinnox;
	
#endif
}

// --
//
// --
void KSubWorldSet::MessageLoop()
{
	
	for (int i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].MessageLoop();
	}
}

// --
//
// --
BOOL KSubWorldSet::SendMessage(int nSubWorldID, DWORD dwMsgType, int nParam1, int nParam2, int nParam3)
{
	KWorldMsgNode *pNode = NULL;
	
	pNode = new KWorldMsgNode;
	if (!pNode)
		return FALSE;
	
	pNode->m_dwMsgType	= dwMsgType;
	pNode->m_nParam[0]	= nParam1;
	pNode->m_nParam[1]	= nParam2;
	pNode->m_nParam[2]	= nParam3;
	return SubWorld[nSubWorldID].m_WorldMessage.Send(pNode);
}

// --
//
// --
void KSubWorldSet::Close()
{
	for (int i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].Close();
	}
	NpcSet.RemoveAll();
#ifndef _SERVER
	// 
	Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveAll();
	Player[CLIENT_PLAYER_INDEX].m_ItemList.Release();
	//
	Player[CLIENT_PLAYER_INDEX].m_cTong.Clear();
	Player[CLIENT_PLAYER_INDEX].m_cTask.Release();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	g_Team[0].Release();
	Player[CLIENT_PLAYER_INDEX].Release();
	
	m_cMusic.Stop();
#endif
}

#ifndef _SERVER
// --
//
// --
void KSubWorldSet::Paint()
{
	SubWorld[0].Paint();
}
#endif

#ifdef _SERVER
// --
//
// --
void KSubWorldSet::GetRevivalPosFromId(DWORD dwSubWorldId, int nRevivalId, POINT* pPos)
{
	if (!pPos)
		return;

	KIniFile IniFile;
	char	szKeyName[32];
	char	szSection[32];
	
	g_SetFilePath("\\settings");
	IniFile.Load("RevivePos.ini");
	sprintf(szSection, "%d", dwSubWorldId);
	sprintf(szKeyName, "%d", nRevivalId);
	
	int nX = 51200;
	int nY = 102400;
	IniFile.GetInteger2(szSection, szKeyName, &nX, &nY);
	
	pPos->x = nX;
	pPos->y = nY;

}
#endif







