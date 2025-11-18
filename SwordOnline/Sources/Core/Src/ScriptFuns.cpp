/*******************************************************************************
// FileName			:	ScriptFuns.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-11-19 15:58:20
// FileDescription	:	?u????
// Revision Count	:	
*******************************************************************************/
#ifndef WIN32
#include <string>
#endif

#include "KWin32.h"
#include "KEngine.h"
#include "KDebug.h"
#include "KStepLuaScript.h"
#include "LuaLib.h"
#include "KScriptList.h"
#include <string.h>
#include "LuaFuns.h"
#include "KCore.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "KObjSet.h"
#include "KItemSet.h"
//#include "KNetClient.h"
#include "KScriptValueSet.h"
#include "KNpcSet.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"
#include "KBuySell.h"
#include "KTaskFuns.h"
#include "KPlayerDef.h"
#include "TaskDef.h"
#include "ZLoadFile.h"
#include "KItemGenerator.h"
//
#include "time.h"
//
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/interface/iServer.h"
#include "KNewProtocolProcess.h"
#include "MSSQL/KB_MSSQL.h"
#endif
#include "KSortScript.h"
#ifndef __linux
#include "Shlwapi.h"
#include "windows.h"
#include "winbase.h"
#include <direct.h>
#else
#include "unistd.h"
#endif

#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif

#ifndef _SERVER
#include "CoreShell.h"
#endif

#ifndef WIN32
typedef struct  _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}	SYSTEMTIME;
typedef struct  _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
}	FILETIME;
#endif

inline const char* _ip2a(DWORD ip) { in_addr ia; ia.s_addr = ip; return inet_ntoa(ia); }
inline DWORD _a2ip(const char* cp) { return inet_addr(cp); }

KScriptList		g_StoryScriptList;
KStepLuaScript * LuaGetScript(Lua_State * L);
int	GetPlayerIndex(Lua_State * L);
extern int g_GetPriceToStation(int,int);
extern int g_GetPriceToWayPoint(int,int);
extern int g_GetPriceToDock(int ,int );

//BitValue = GetBit(Value, BitNo)
int LuaGetBit(Lua_State * L)
{
	int nBitValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);
	
	if (nBitNumber >= 32 || nBitNumber <= 0) 
		goto lab_getbit;
	nBitValue = (nIntValue & (1 << (nBitNumber - 1))) != 0;
lab_getbit:
	Lua_PushNumber(L, nBitValue);
	return 1;
}

//NewBit = SetBit(Value, BitNo, BitValue)
int LuaSetBit(Lua_State * L)
{
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);
	int nBitValue = (int)Lua_ValueToNumber(L,3);
	nBitValue = (nBitValue == 1);
	
	if (nBitNumber > 32 || nBitNumber <= 0) 
		goto lab_setbit;
	
	nIntValue = (nIntValue | (1 << (nBitNumber - 1)));
lab_setbit:
	Lua_PushNumber(L, nIntValue);
	return 1;
}

//ByteValue = GetByte(Value, ByteNo)
int LuaGetByte(Lua_State * L)
{
	int nByteValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);
	
	if (nByteNumber > 4 || nByteNumber <= 0) 
		goto lab_getByte;
	nByteValue = (nIntValue & (0xff << ((nByteNumber - 1) * 8) )) >> ((nByteNumber - 1) * 8);
	
lab_getByte:
	Lua_PushNumber(L, nByteValue);
	return 1;
}

//NewByte = SetByte(Value, ByteNo, ByteValue)
int LuaSetByte(Lua_State * L)
{
	BYTE * pByte =	NULL;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);
	int nByteValue = (int)Lua_ValueToNumber(L,3);
	nByteValue = (nByteValue & 0xff);
	
	if (nByteNumber > 4 || nByteNumber <= 0) 
		goto lab_setByte;
	
	pByte = (BYTE*)&nIntValue;
	*(pByte + (nByteNumber -1)) = (BYTE)nByteValue;
	//nIntValue = (nIntValue | (0xff << ((nByteNumber - 1) * 8) )) ;
lab_setByte:
	Lua_PushNumber(L, nIntValue);
	return 1;
}

#ifdef _SERVER
int LuaModifyTransLife(Lua_State *L)
{
	if (Lua_GetTopIndex(L) <1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	char nValue = (char)Lua_ValueToNumber(L,1);
	
	Player[nPlayerIndex].TaskSetSaveVal(TV_TRANSLIFE, Player[nPlayerIndex].TaskGetSaveVal(TV_TRANSLIFE) + nValue);
	return 1;	
}

int LuaGetTransLife(Lua_State *L)
{
	short nValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		goto lab_gettranslife;
	nValue = Player[nPlayerIndex].TaskGetSaveVal(TV_TRANSLIFE);
lab_gettranslife:
	Lua_PushNumber(L, nValue);
	return 1;	
}

int LuaModifyRepute(Lua_State *L)
{
	if (Lua_GetTopIndex(L) <1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	short nValue = (short)Lua_ValueToNumber(L,1);
	
	Player[nPlayerIndex].TaskSetSaveVal(TV_REPUTE, Player[nPlayerIndex].TaskGetSaveVal(TV_REPUTE) + nValue);
	
	if (nValue < 0)
	{
		char szMsg[100];
		sprintf(szMsg, "Danh väng b¹n gi¶m <color=green>%d ®iÓm<color>!", -nValue);		
		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	else
	{
		char szMsg[100];
		sprintf(szMsg, "Danh väng b¹n t¨ng <color=green>%d ®iÓm<color>!", nValue);		
		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	
	return 1;	
}

int LuaGetRepute(Lua_State *L)
{
	short nValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		goto lab_getrepute;
	nValue = Player[nPlayerIndex].TaskGetSaveVal(TV_REPUTE);
lab_getrepute:
	Lua_PushNumber(L, nValue);
	return 1;	
}

int LuaModifyFuyuan(Lua_State *L)
{
	if (Lua_GetTopIndex(L) <1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	short nValue = (short)Lua_ValueToNumber(L,1);
	
	Player[nPlayerIndex].TaskSetSaveVal(TV_FUYUAN, Player[nPlayerIndex].TaskGetSaveVal(TV_FUYUAN) + nValue);
	
	if (nValue < 0)
	{
		char szMsg[100];
		sprintf(szMsg, "Phóc duyªn b¹n gi¶m <color=green>%d ®iÓm<color>!", -nValue);		
		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	else
	{
		char szMsg[100];
		sprintf(szMsg, "Phóc duyªn b¹n t¨ng <color=green>%d ®iÓm<color>!", nValue);		
		KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg) );
	}
	
	return 1;	
}

int LuaGetFuyuan(Lua_State *L)
{
	short nValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		goto lab_getfuyuan;
	nValue = Player[nPlayerIndex].TaskGetSaveVal(TV_FUYUAN);
lab_getfuyuan:
	Lua_PushNumber(L, nValue);
	return 1;	
}
#endif

int GetSubWorldIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_SUBWORLDINDEX);
	if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_SUBWORLD || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}
	if (SubWorld[nIndex].m_nIndex >= MAX_SUBWORLD || SubWorld[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}

//Idx = SubWorldID2Idx(dwID)
int LuaSubWorldIDToIndex(Lua_State * L)
{
	int nTargetSubWorld = -1;
	int nSubWorldID = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	
	
lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}

int LuaSubWorldIndexToID(Lua_State * L)
{
	int nTargetSubWorld = -1;
	int nSubWorldID = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldidx2id;
	
	nTargetSubWorld = (int)Lua_ValueToNumber(L, 1);
	nSubWorldID = SubWorld[nTargetSubWorld].m_SubWorldID;	
	
lab_subworldidx2id:
	Lua_PushNumber(L, nSubWorldID);
	return 1;
}


/*
Say(sMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln) 
Say(nMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln) 
Say(nMainInfo, nSelCount, SelTab)
????????????????????????????ß???

  Say(100, 3, 10, 23,43)
  Say("????ô??", 2, "??/yes", "??/no");
  Say("??ô?", 2, SelTab);
*/
//**************************************************************************************************************************************************************
//												????u?
//**************************************************************************************************************************************************************
int LuaSelectUI(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;
	
	if (Lua_IsNumber(L,2))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,2);
	}
	else 
	{
		_ASSERT(0);
		return 0;
	}
	
	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//?????????????????????????????
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;
	
	BOOL bStringTab = FALSE;//??????????????????????h????????????????????????
	
	if (Lua_IsString(L,3))
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3))
	{
		bStringTab = TRUE;
	}
	else 
	{if (nOptionNum > 0)  return 0;
	}
	
	if (bStringTab == FALSE)
	{
		//?????????????????
		if (nOptionNum > nParamNum - 2) nOptionNum = nParamNum - 2;
	}
	
	if (nOptionNum > MAX_ANSWERNUM) nOptionNum = MAX_ANSWERNUM;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;//?????????????????(0)??????(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	//???????????
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) //????????????
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}
	
	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;		
	
	for (int i  = 0; i < nOptionNum; i ++)
	{	
		char  pAnswer[100];
		pAnswer[0] = 0;
		
		if (bStringTab)
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 3);
			char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 100);
			}
		}
		else 
		{
			char * pszString = (char *)Lua_ValueToString(L, i + 3);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 100);
		}
		
		char * pFunName = strstr(pAnswer, "/");
		
		if (pFunName)
		{
			g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			*pFunName = 0;
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
		else 
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}
	
	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else 
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}


// --

// --
int LuaSelectUI2(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;

	if (Lua_IsNumber(L,3))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,3);
	}
	else 
	{
		return 0;
	}
	
	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1;
	}
	else if (Lua_IsString(L, 1))
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;
	
	BOOL bStringTab = FALSE;
	
	if (Lua_IsString(L,4))
	{
		bStringTab = FALSE;
	}
	else if (Lua_IsTable(L, 4))
	{
		bStringTab = TRUE;
	}
	else
	{	
		if (nOptionNum > 0)  
			return 0;
	}
	
	if (bStringTab == FALSE)
	{
		if (nOptionNum > nParamNum - 2) 
			nOptionNum = nParamNum - 2;
	}
	
	if (nOptionNum > MAX_ANSWERNUM) 
		nOptionNum = MAX_ANSWERNUM;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1)
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}
	
	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;		
	
	char  pAnswer[100];
	pAnswer[0] = 0;
	char* pszString = NULL;
	char* pFunName = NULL;

	pFunName = (char*)Lua_ValueToString(L, 2);
	if(pFunName)
	{
		sprintf(pContent, "%s|%s", pContent, pFunName);
		pFunName = NULL;
	}
	else
	{
		sprintf(pContent, "%s|%s", pContent, "0");
	}

	for (int i  = 0; i < nOptionNum; i ++)
	{	
		pAnswer[0] = 0;
		if (bStringTab)
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 4);
			pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 100);
			}
		}
		else 
		{
			pszString = (char *)Lua_ValueToString(L, i + 4);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 100);
		}
		
		pFunName = strstr(pAnswer, "/");
		
		if (pFunName)
		{
			g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			*pFunName = 0;
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
		else 
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}
	
	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else 
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 2;
#endif
	
	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}


//PutMsg(szMsg/MsgId)
int LuaSendMessageInfo(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_MSGINFO;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 64);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddGlobalNews(Newsstr)
int LuaAddGlobalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);		//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}


int LuaAddGlobalNewsEx(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFOEX;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}
//AddLocalNews(Newsstr)
int LuaAddLocalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);		//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

// --
//
// --
int LuaAddGlobalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);		//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

// --
//
// --
int LuaAddLocalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	int nTime = (int)Lua_ValueToNumber(L,2);
	
	if (nTime <= 0)
		nTime = 1;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);		//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

// --
//
// --
int LuaAddGlobalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);		//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

// --
//
// --
int LuaAddLocalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else 
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);	//
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}
	
	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));
	
	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));
	
	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif
	
#ifndef _SERVER
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

// --
//
// --
int LuaAddNote(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) 
		return 0;
	
	int nParam2 = 0;
	
	
	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1))
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;
	
	if (nParamNum > 1)
	{
		nParam2 = (int)Lua_ValueToNumber(L, 2);
	}
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NOTEINFO;
	UiInfo.m_bParam1 = nDataType;
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		int nLen = strlen(strMain);
		*(int*)(UiInfo.m_pContent + nLen) = nParam2;
		UiInfo.m_nBufferLen = nLen + sizeof(int);
	}
	else if (nDataType == 1)
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		*(int *)(UiInfo.m_pContent + sizeof(int)) = nParam2;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(int);
	}
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

// --
//
// --
int LuaTalkUI(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3) 
		return 0;
	
	if (Lua_IsNumber(L,1))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,1);
	}
	else 
	{
		_ASSERT(0);
		return 0;
	}
	
	const char * pCallBackFun = Lua_ValueToString(L,2);
	
	if  (Lua_IsNumber(L,3))
	{
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 3)) 
	{
		nDataType = 0 ;
	}
	else
		return 0;
	
	if (nOptionNum > nParamNum - 2) 
		nOptionNum = nParamNum - 2;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_TALKDIALOG;
	UiInfo.m_bParam1 = nDataType;
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	pContent = UiInfo.m_pContent;
	pContent[0] = 0;
	size_t nContentLen = 0;
	for (int i  = 0; i < nOptionNum; i ++)
	{	
		const char * pString = NULL;
		if (!nDataType)
		{
			pString = Lua_ValueToString(L, i + 3);
			if (nContentLen  + strlen(pString) >= MAX_SCIRPTACTION_BUFFERNUM)
			{
				nOptionNum = i;
				UiInfo.m_bOptionNum = nOptionNum;
				break;
			}
			nContentLen += strlen(pString);
			sprintf(pContent, "%s%s|", pContent, pString);
		}
		else
		{
			int j = (int)Lua_ValueToNumber(L, i + 3);
			sprintf(pContent, "%s%d|", pContent, j);
		}
	}
	UiInfo.m_nBufferLen  = strlen(pContent);
	
	if (!pCallBackFun || strlen(pCallBackFun) <= 0)
	{
		UiInfo.m_nParam = 0;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 0;
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		UiInfo.m_nParam = 1;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pCallBackFun, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
	
}

// --
//
// --
int LuaIncludeFile(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	
	if (Lua_IsString(L,1))
	{
		const char * pFileName = lua_tostring(L,1);	
		char lszCurrentDirectory[MAX_PATH];
		int nLen = 0;
		if (pFileName[0] != '\\' && pFileName[0] != '/')
		{
			getcwd(lszCurrentDirectory, MAX_PATH);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
			for (int i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		else
		{
			g_GetRootPath(lszCurrentDirectory);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
			for (int i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		strlwr(lszCurrentDirectory + nLen);
		lua_dofile(L, lszCurrentDirectory);
		return 0;
	}
	else
		return 0;
}

// --
//
// --
int LuaGetTaskValue(Lua_State * L)
{
	if(Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNil(L);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
	{	
		Lua_PushNil(L);
		return 1;
	}
	
	int nValue = (int)Player[nPlayerIndex].m_cTask.GetSaveVal((int)Lua_ValueToNumber(L,1));		
	Lua_PushNumber(L, nValue);
	return 1;
}

#ifdef _SERVER
// --
//
// --
int LuaSetTaskValue(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 2)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, nValue);
	return 0;
}
#endif

#ifdef _SERVER
// --
//
// --
int LuaSyncTaskValue(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	int nValue = Player[nPlayerIndex].m_cTask.GetSaveVal(nValueIndex);

	TASK_VALUE_SYNC		sValue;
	sValue.ProtocolType		= s2c_taskvaluesync;
	sValue.m_bClear			= FALSE;
	sValue.m_nId			= nValueIndex;
	sValue.m_bValue			= nValue;

	if(g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(TASK_VALUE_SYNC));
	return 0;
}
#endif



#ifndef _SERVER
#define MAX_TEMPVALUENUM_INCLIENT 500
int g_TempValue[MAX_TEMPVALUENUM_INCLIENT];
#endif

// --
//
// --
int LuaGetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	
#ifdef _SERVER
	if(nTempIndex >= MAX_TEMP_TASK)
	{
		Lua_PushNil(L) ;
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nValue = Player[nPlayerIndex].m_cTask.GetClearVal(nTempIndex);
	Lua_PushNumber(L, nValue);
#else
	
	if (nTempIndex >= 0 && nTempIndex < MAX_TEMPVALUENUM_INCLIENT)
		Lua_PushNumber(L, g_TempValue[nTempIndex]);
	else 
		Lua_PushNil(L);
#endif
	return 1;
}

// --
//
// --
int LuaSetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L) - 1);
	int nValue = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
#ifdef _SERVER	
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
	int nPlayerIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	Player[nPlayerIndex].TaskSetClearVal(nTempIndex, nValue);
#else
	g_TempValue[nTempIndex] = nValue;
#endif
	return 0;
}

#ifdef _SERVER
// --
//
// --
int LuaSale(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	int nShopId = (int)Lua_ValueToNumber(L,1);
	BuySell.OpenSale(nPlayerIndex, nShopId - 1);
	return 0;
}

// --
//
// --
int LuaOpenBox(Lua_State * L)
{
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) ) 
		return 0;
	
	BYTE	NetCommand = (BYTE)s2c_openstorebox;

	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(BYTE));

	return 0;
}

// --
//
// --
int LuaSetTimer(Lua_State  * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) 
		return 0;
	
	int nNpcIndex = 0;
	int nTimerId = 0;
	DWORD dwRestTime = 0;
	if(nParamNum > 2)
		goto lab_npctimer;
	else
		goto lab_playertimer;
	
lab_npctimer:
	nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;
	dwRestTime = (DWORD)Lua_ValueToNumber(L,2);
	nTimerId = (int)Lua_ValueToNumber(L, 3);
	
	Npc[nNpcIndex].SetTimer(dwRestTime, nTimerId);
	return 0;

lab_playertimer:
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;
	dwRestTime = (DWORD)Lua_ValueToNumber(L,1);
	nTimerId = (int)Lua_ValueToNumber(L, 2);

	Npc[nNpcIndex].SetTimer(dwRestTime, nTimerId);
	return 0;
}

// --
//
// --
int LuaStopTimer(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	int nNpcIndex = 0;
	if(nParamNum > 0)
		goto lab_npctimer;
	else
		goto lab_playertimer;
	
lab_npctimer:
	nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].CloseTimer();
	return 0;

lab_playertimer:
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].CloseTimer();
	return 0;
}

// --
//
// --
int LuaGetCurTimerId(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	int nNpcIndex = 0;
	if(nParamNum > 0)
		goto lab_npctimer;
	else
		goto lab_playertimer;
lab_npctimer:
	nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Npc[nNpcIndex].GetTimerId());
	return 1;

lab_playertimer:
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Npc[nNpcIndex].GetTimerId());
	return 1;
}

// --
//
// --
int LuaGetRestTime(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	int nNpcIndex = 0;
	if(nParamNum > 0)
		goto lab_npctimer;
	else
		goto lab_playertimer;
lab_npctimer:
	nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Npc[nNpcIndex].GetTimerRestTime());
	return 1;

lab_playertimer:
	int nPlayerIndex  = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Npc[nNpcIndex].GetTimerRestTime());
	return 1;
}

// --
//
// --
int LuaGetMissionRestTime(Lua_State * L)
{
	int RestTime = 0;
	if (Lua_GetTopIndex(L) >= 2)
	{
		int nSubWorldIndex = GetSubWorldIndex(L);
		if (nSubWorldIndex >= 0) 
		{
			int nMissionId = (int)Lua_ValueToNumber(L, 1);
			int nTimerId = (int)Lua_ValueToNumber(L, 2);
			
			if (nMissionId < 0 || nTimerId < 0 )
				goto lab_getmissionresttime;
			
			KMission Mission;
			Mission.SetMissionId(nMissionId);
			KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
			if (pMission)
			{
				RestTime = (int)pMission->GetTimerRestTimer(nTimerId);
			}
		}
	}
	
lab_getmissionresttime:
	Lua_PushNumber(L, RestTime);
	return 1;
}
// --
//
// --
int LuaIsLeader(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0 ) 
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag && Player[nPlayerIndex].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			Lua_PushNumber(L,1);
		else 
			Lua_PushNumber(L,0);
		
	}
	else 
		Lua_PushNumber(L, 0);
	return 1;
}
// --
//
// --
int LuaGetTeamId(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
			Lua_PushNumber(L, Player[nPlayerIndex].m_cTeam.m_nID);
		else Lua_PushNil(L);
	}
	else 
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaGetTeamSize(Lua_State * L)
{
	int nTeamSize = 0;
	int nTeamId = -1;
	if (Lua_GetTopIndex(L) >= 1)
	{
		nTeamId = Lua_ValueToNumber(L, 1);
	}
	else
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			if (Player[nPlayerIndex].m_cTeam.m_nFlag)
				nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
			else 
				nTeamId = -1;
		}
	}
	
	if (nTeamId < 0)
		nTeamSize = 0;
	else
		nTeamSize = g_Team[nTeamId].m_nMemNum + 1;
	Lua_PushNumber(L, nTeamSize);
	return 1;
}
// --
//
// --
int LuaLeaveTeam(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
		{
			PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
			sLeaveTeam.ProtocolType = c2s_teamapplyleave;
			Player[nPlayerIndex].LeaveTeam((BYTE*)&sLeaveTeam);
		}
	}
	return 0;
}
// --
//
// --
int LuaGetTeamMember(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNil(L);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNil(L);
		return 1;
	}
	int nTeamMemberIdx = 0;
	int nTeamId = -1;
	int nIndex = (int)Lua_ValueToNumber(L, 1);
	nIndex -= 1;
	if(nIndex < 0 || nIndex >= MAX_TEAM_MEMBER)
	{	
		Lua_PushNil(L);
		return 1;
	}
	if (Player[nPlayerIndex].m_cTeam.m_nFlag)
		nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
	else 
		nTeamId = -1;
	
	if (nTeamId < 0)
	{	
		Lua_PushNil(L);
		return 1;
	}
	
	if(nIndex < g_Team[nTeamId].m_nMemNum)
		nTeamMemberIdx = g_Team[nTeamId].m_nMember[nIndex];
	else
		nTeamMemberIdx = g_Team[nTeamId].m_nCaptain;

	Lua_PushNumber(L, nTeamMemberIdx);
	return 1;
}
// --
//
// --
int LuaGetTeamCaptain(Lua_State * L)
{	
	int nTeamCaptainIdx = 0;
	int nTeamId = -1;
	if (Lua_GetTopIndex(L) >= 1)
	{
		nTeamId = (int)Lua_ValueToNumber(L, 1);
	}
	else
	{
		int nPlayerIndex = GetPlayerIndex(L);
		if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		{	
			Lua_PushNil(L);
			return 1;
		}

		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
			nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
		else 
			nTeamId = -1;
	}
	
	if (nTeamId < 0)
		nTeamCaptainIdx = 0;
	else
		nTeamCaptainIdx = g_Team[nTeamId].m_nCaptain;
	Lua_PushNumber(L, nTeamCaptainIdx);
	return 1;
}

// --
//
// --
int LuaSetCreateTeamOption(Lua_State * L)
{
	int nState = (int)Lua_ValueToNumber(L, 1);
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (nState)
			Player[nPlayerIndex].m_cTeam.SetCanTeamFlag(nPlayerIndex, TRUE);
		else
			Player[nPlayerIndex].m_cTeam.SetCanTeamFlag(nPlayerIndex, FALSE);
	}
	return 0;
}
// --
//
// --
int	LuaMsgToPlayer(Lua_State * L)
{
if (Lua_GetTopIndex(L) <= 0) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	int nParamNum = Lua_GetTopIndex(L);
	if (nPlayerIndex > 0) 
	{
		if (nParamNum == 2)
		{
			const char *  szMsg = Lua_ValueToString(L,1);

			if (szMsg)
				KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *) szMsg, strlen(szMsg));
		}
		else if(nParamNum == 3)
		{
			const char * szMsg = Lua_ValueToString(L,1);
			const char * szMsg1 = Lua_ValueToString(L,2);
			if (szMsg)
				KPlayerChat::SendSystemInfo(1, nPlayerIndex, (char *)szMsg, (char *)szMsg1, strlen(szMsg1) );		
		}
		else
		{
			KPlayerChat::SendSystemInfo(1, nPlayerIndex, "Lçi Bug", "Bug hµm LuaMsgToPlayer vui lßng b¸o BQT", 48);
		}
	}
	return 0;
}
// --
//
// --
int LuaMsgToTeam(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0	) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	if (Player[nPlayerIndex].m_cTeam.m_nID >= 0)
	{
		const char* szMsg = Lua_ValueToString(L,1);
		int nTeamLeaderId = g_Team[Player[nPlayerIndex].m_cTeam.m_nID].m_nCaptain;
		if (nTeamLeaderId > 0)
			KPlayerChat::SendSystemInfo(1, nTeamLeaderId, "Tin tøc tæ ®éi",(char *) szMsg, strlen(szMsg) );
		
		for (int i = 0; i < MAX_TEAM_MEMBER; i ++)
		{
			int nMemberId = g_Team[Player[nPlayerIndex].m_cTeam.m_nID].m_nMember[i] ;
			if (nMemberId > 0)
			{
				if (szMsg)
					KPlayerChat::SendSystemInfo(1, nMemberId, "Tin tøc tæ ®éi", (char *)szMsg, strlen(szMsg) );
			}
		}
	}
	return 0;
}
// --
//
// --
int LuaMsgToSubWorld(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0;
	
	const char * szMsg = Lua_ValueToString(L,1);
	if (szMsg)
		KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, (char *)szMsg, strlen(szMsg) );
	return 0;
}
// --
//
// --
int	LuaMsgToFaction(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nNpcIndex = Lua_ValueToNumber(L, 1);
	char szNpcTypeName[32];
	g_NpcSetting.GetString(Npc[nNpcIndex].m_NpcSettingIdx + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
	if(szNpcTypeName)
		KPlayerChat::SendSystemInfo(1, nPlayerIndex, "Picture ID", (char*)szNpcTypeName, strlen(szNpcTypeName));
	return 0;
}

// --
//
// --
int LuaMsgToAroundRegion(Lua_State * L)
{
	return 0;
}
// --
//
// --
int  LuaEnterNewWorld(Lua_State * L) 
{	
	if(Lua_GetTopIndex(L) < 3)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex < 0 || nPlayerIndex >= MAX_PLAYER) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex < 0 || nNpcIndex >= MAX_NPC) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	DWORD dwWorldId = (DWORD)Lua_ValueToNumber(L, 1);
	int nMapX = (int)Lua_ValueToNumber(L,2) * 32;
	int nMapY = (int)Lua_ValueToNumber(L,3) * 32;

	bool bResult = false;
	bResult = Npc[nNpcIndex].ChangeWorld(dwWorldId, nMapX, nMapY);
	
	Lua_PushNumber(L, bResult);
	return 1;
}
// --
//
// --
int LuaGetNpcLevel(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNil(L);
		return 1; 
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
	{
		Lua_PushNil(L);
		return 1;
	}

	Lua_PushNumber(L, Npc[nNpcIdx].m_Level); 
	return 1;
}
// --
//
// --
int LuaSetPos(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount != 2) return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	
	int nX = (int) Lua_ValueToNumber(L,1);
	int nY = (int) Lua_ValueToNumber(L,2);
	
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetPos(nX * 32, nY * 32);
	}
	return 0;
}

int LuaGetPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex > 0)
	{
		int nPosX = 0;
		int nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);
		Lua_PushNumber(L, nPosX);
		Lua_PushNumber(L, nPosY);
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex); 
	}
	else
		return 0;
	return 3;
}
// --
//
// --
int LuaGetNewWorldPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex > 0)
	{
		int nPosX = 0;
		int nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);

		int nSubWorldIndex = Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex;
		int nSubWorldID = 0;
		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID); 
		Lua_PushNumber(L, ((int)(nPosX / 32)));
		Lua_PushNumber(L, ((int)(nPosY / 32)));
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	return 3;
}
// --
//
// --
int LuaGetNpcName(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
	{
		Lua_PushNil(L);
		return 1;
	}

	Lua_PushString(L, Npc[nNpcIdx].Name); 
	return 1;
}
// --
//
// --
int LuaGetNpcPos(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	int nPosX = 0;
	int nPosY = 0;
	Npc[nNpcIdx].GetMpsPos(&nPosX, &nPosY);

	int nSubWorldIndex = Npc[nNpcIdx].m_SubWorldIndex;
	if (nSubWorldIndex < 0 && nSubWorldIndex >= MAX_SUBWORLD)
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0); 
		return 3;
	}

	Lua_PushNumber(L, nPosX);
	Lua_PushNumber(L, nPosY);
	Lua_PushNumber(L, nSubWorldIndex); 
	return 3;
}
// --
//
// --
int LuaGetNpcGold(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	Lua_PushNumber(L, Npc[nNpcIdx].m_cGold.GetGoldType()); 
	return 1;
}
// --
//
// --
int LuaGetNpcSeries(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	Lua_PushNumber(L, Npc[nNpcIdx].m_Series); 
	return 1;
}
// --
//
// --
int LuaSetNpcParam(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 3)
		return 0;
	
	int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;

	int nParam = (int)Lua_ValueToNumber(L, 2);
	if(nParam <= 0 || nParam > 500)
		return 0;

	int nValue = (int)Lua_ValueToNumber(L, 3);
	
	Npc[nNpcIdx].m_nParam[nParam - 1] = nValue;
	return 0;
}
// --
//
// --
int LuaGetNpcParam(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nNpcIdx = (int)Lua_ValueToNumber(L,1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
	{
		Lua_PushNil(L);
		return 1;
	}
	int nParam = (int)Lua_ValueToNumber(L,2);
	if(nParam <= 0 || nParam > 500)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	Lua_PushNumber(L, Npc[nNpcIdx].m_nParam[nParam - 1]); 
	return 1;
}
// --
//
// --
int LuaDropMoney(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) 
		return 0;	

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	int nIndex = 0;
	int nSubWorldIndex = 0;
	int nX, nY;
	int nMoney = 0;

	nIndex = Player[nPlayerIndex].m_nIndex;
	nMoney = (int)Lua_ValueToNumber(L, 1);

	nX = (int)Lua_ValueToNumber(L, 2);
	nY = (int)Lua_ValueToNumber(L, 3);

	if (nIndex <= 0)
	{
		nIndex = Player[nPlayerIndex].m_nIndex;
	}
	if (nMoney <= 0) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	//Npc[nIndex].GetMpsPos(&nX, &nY);
	nSubWorldIndex = Npc[nIndex].m_SubWorldIndex;

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	POINT	ptLocal;
	KMapPos	Pos;

	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj = ObjSet.AddMoneyObj(Pos, nMoney);
	if (nObj > 0)
	{
		Object[nObj].SetItemBelong(nPlayerIndex);
		Lua_PushNumber(L,nObj);
		return 1;
	}
	return 0;
}
// --
//
// --
int LuaDropItem(Lua_State *L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 11)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nSubWorldIdx = (int)Lua_ValueToNumber(L,1);
	int nMpsX = (int)Lua_ValueToNumber(L,2);
	int nMpsY = (int)Lua_ValueToNumber(L,3);
	int nBelongPlayerIdx = (int)Lua_ValueToNumber(L,4);
	int nItemGenre = (int)Lua_ValueToNumber(L,5);
	int nItemDetailType = (int)Lua_ValueToNumber(L,6);
	int nItemParticularType = (int)Lua_ValueToNumber(L,7);
	int nItemLevel = (int)Lua_ValueToNumber(L,8);
	int nItemSeries = (int)Lua_ValueToNumber(L,9);
	int nItemLuck = (int)Lua_ValueToNumber(L,10);
	
	int nItemMagic[6];
	int nLevelMagic = (int)Lua_ValueToNumber(L,11);
	for (int i = 0; i < 6; i++)
		nItemMagic[i] = nLevelMagic;

	int nExpiredTime		= 0;
	if(nParamNum >= 12)
	{	
		nExpiredTime		= (int)Lua_ValueToNumber(L, 12);
	}
	//executeitem by kinnox;
	int nIndex = ItemSet.DropItem(nSubWorldIdx, nMpsX, nMpsY, nBelongPlayerIdx, nItemGenre, nItemDetailType, nItemParticularType, nItemLevel, nItemSeries, nItemLuck, nItemMagic, nExpiredTime);
	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	Lua_PushNumber(L, nIndex);
	return 1;
}
// --
//
// --
int LuaDropRateItem(Lua_State *L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nNpcIndex = (int)Lua_ValueToNumber(L,1);
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	int nCount = (int)Lua_ValueToNumber(L,2);
	const char* pFileName = (char*)Lua_ValueToString(L,3);
	int nUnknow = (int)Lua_ValueToNumber(L, 4);
	int nItemLevel = (int)Lua_ValueToNumber(L,5);
	int nItemSeries = (int)Lua_ValueToNumber(L,6);
	
	Npc[nNpcIndex].DropRateItem(nCount, pFileName, nUnknow, nItemLevel, nItemSeries, nPlayerIndex);

	return 0;
}
// --
//
// --
int LuaFindEmptyPlace(Lua_State * L)
{	
	short nParamNum = Lua_GetTopIndex(L);
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	BOOL bResult = FALSE;
	BYTE btWidth = 2;
	BYTE btHeight = 4;
	
	if(nParamNum >= 2)
	{
		btWidth = (BYTE)Lua_ValueToNumber(L, 1);
		btHeight = (BYTE)Lua_ValueToNumber(L, 2);
	}

	bResult = Player[nPlayerIndex].m_ItemList.FindEmptyPlaceEquipment(btWidth, btHeight);

	Lua_PushNumber(L, bResult);
	return 1;
}

KTabFile g_EventItemTab;
// --
//
// --
int LuaAddEventItem(Lua_State *L)
{
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nDetailType = (int)Lua_ValueToNumber(L, 1);
	if(nDetailType < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	const int nItemIdx = ItemSet.Add(item_task, -1, 0, -1, 0, nDetailType, -1, NULL, g_SubWorldSet.GetGameVersion(), 0);
	
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) ) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if(nParamNum >= 2)
	{
		Item[nItemIdx].SetTime((DWORD)Lua_ValueToNumber(L,2));
	}
	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight(), &x, &y))
	{
		if( Item[nItemIdx].CanStack() )
			Player[nPlayerIndex].m_ItemList.Add_AutoStack(room_equipment, nItemIdx, x, y);
		else
			Player[nPlayerIndex].m_ItemList.Add(nItemIdx, pos_equiproom, x, y);
		
		char szData[128];
		//
		Item[nItemIdx].GetItemBackupInfo(szData);
		//
		Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD TASK ITEM", Item[nItemIdx].GetName());
				
		Lua_PushNumber(L, nItemIdx);
	}
	else
	{
		ItemSet.Remove(nItemIdx);
		Lua_PushNumber(L, 0);
	}
	
	return 1;
}
// --
//
// --
int LuaAddMineItem(Lua_State *L)
{
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 4)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nDetailType = (int)Lua_ValueToNumber(L, 1);
	if(nDetailType < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	const int nLevel = (int)Lua_ValueToNumber(L, 2);
	const int nSeries = (int)Lua_ValueToNumber(L, 3);
	const int nLuck = (int)Lua_ValueToNumber(L, 4);
	
	const int nItemIdx = ItemSet.Add(item_mine, nSeries, nLevel, -1, nLuck, nDetailType, -1, NULL, g_SubWorldSet.GetGameVersion(), 0);
	
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) ) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if(nParamNum >= 5)
	{
		Item[nItemIdx].SetTime((DWORD)Lua_ValueToNumber(L,5));
	}
	
	int nPlace = pos_equiproom;
	int x, y;

	if(nParamNum >= 8)
	{
		nPlace = (int)Lua_ValueToNumber(L,6);
		x = (int)Lua_ValueToNumber(L,7);
		y = (int)Lua_ValueToNumber(L,8);

		Player[nPlayerIndex].m_ItemList.Add(nItemIdx, nPlace, x, y);
	}
	else
	{
		if( Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight(), &x, &y) )
		{
			
			if( Item[nItemIdx].CanStack() )
				Player[nPlayerIndex].m_ItemList.Add_AutoStack(room_equipment, nItemIdx, x, y);
			else
				Player[nPlayerIndex].m_ItemList.Add(nItemIdx, pos_equiproom, x, y);
		
			char szData[128];
			//
			Item[nItemIdx].GetItemBackupInfo(szData);
			//
			Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD MINE ITEM", Item[nItemIdx].GetName());
			
			Lua_PushNumber(L, nItemIdx);

		}
		else
		{
			ItemSet.Remove(nItemIdx);
			Lua_PushNumber(L, 0);
		}	
	}
	
	return 1;
}
// --
//
// --
int LuaAddItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 7)
	{
#ifdef _DEBUG
		//g_DebugLog("[Script]'??AddItem????????????!");
#endif
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nLuck			= (int)Lua_ValueToNumber(L, 6);
	int nItemLevel[6];
	
	ZeroMemory(nItemLevel, sizeof(nItemLevel));
	nItemLevel[0] = (int)Lua_ValueToNumber(L, 7);
	
	if (nParamNum >= 12)
	{
		nItemLevel[1] = (int)Lua_ValueToNumber(L, 8);
		nItemLevel[2] = (int)Lua_ValueToNumber(L, 9);
		nItemLevel[3] = (int)Lua_ValueToNumber(L, 10);
		nItemLevel[4] = (int)Lua_ValueToNumber(L, 11);
		nItemLevel[5] = (int)Lua_ValueToNumber(L, 12);
	}
	else
	{
		for (int i = 0; i < 5; i ++)
			nItemLevel[i + 1] = nItemLevel[0];
	}
	DWORD dwTime = 0;
	if(nParamNum >= 13)
		dwTime = (int)Lua_ValueToNumber(L, 13);

	int nIndex = ItemSet.Add(nItemClass, nSeries, nLevel, 0, nLuck, nDetailType, nParticularType,  nItemLevel, g_SubWorldSet.GetGameVersion());
	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	if(dwTime)
		Item[nIndex].SetTime(dwTime);

	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);

		char szData[128];
		//
		Item[nIndex].GetItemBackupInfo(szData);
		//
		Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD ITEM", Item[nIndex].GetName());
	}
	else
	{
		int		nIdx = Player[nPlayerIndex].m_ItemList.Hand();
		if (nIdx)
		{
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);
			
			KMapPos sMapPos;
			KObjItemInfo	sInfo;
			
			Player[nPlayerIndex].GetAboutPos(&sMapPos);
			
			sInfo.m_nItemID = nIdx;
			sInfo.m_dwItemID = Item[nIdx].GetID();
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code;
			
			int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
			if (nObj == -1)
			{
				ItemSet.Remove(nIdx);
			}
			else
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					Object[nObj].SetEntireBelong(nPlayerIndex);
				}
				else
				{
					Object[nObj].SetItemBelong(nPlayerIndex);
				}
			}
		}
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_hand, 0 ,0);
	}
	Lua_PushNumber(L,nIndex);
	return 1;
}
// --
//
// --
int LuaAddItemEx(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 14)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	int nItemClass		= (int)Lua_ValueToNumber(L, 1);
	int nDetailType		= (int)Lua_ValueToNumber(L, 2);
	int nParticularType	= (int)Lua_ValueToNumber(L, 3);
	int nLevel			= (int)Lua_ValueToNumber(L, 4);
	int nSeries			= (int)Lua_ValueToNumber(L, 5);
	int nLuck			= (int)Lua_ValueToNumber(L, 6);
	int nItemLevel[6];
	
	ZeroMemory(nItemLevel, sizeof(nItemLevel));
	nItemLevel[0] = (int)Lua_ValueToNumber(L, 7);
	nItemLevel[1] = (int)Lua_ValueToNumber(L, 8);
	nItemLevel[2] = (int)Lua_ValueToNumber(L, 9);
	nItemLevel[3] = (int)Lua_ValueToNumber(L, 10);
	nItemLevel[4] = (int)Lua_ValueToNumber(L, 11);
	nItemLevel[5] = (int)Lua_ValueToNumber(L, 12);

	int nVersion = (int)Lua_ValueToNumber(L, 13);
	DWORD dwRandSeed = (DWORD)Lua_ValueToNumber(L, 14);

	int	nPlace = 0, x = 0, y = 0;
	if(nParamNum >= 15 )
	{
		nPlace = (int)Lua_ValueToNumber(L, 15);
		if(nPlace < pos_hand || nPlace > pos_builditem)
		{
			Lua_PushNumber(L,0);
			return 1;
		}
	}

	const int nIndex = ItemSet.Add(nItemClass, nSeries, nLevel, 0, nLuck, nDetailType, nParticularType,  nItemLevel, nVersion, dwRandSeed);
	if( (nIndex <= 0) || (nIndex >= MAX_ITEM) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	if(nPlace != 0)
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, nPlace, x, y);
	}
	else
	{
		if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
		{
			Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);

			char szData[128];
			//
			Item[nIndex].GetItemBackupInfo(szData);
			//
			Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD ITEM EX", Item[nIndex].GetName());
		}
		else
		{
			int		nIdx = Player[nPlayerIndex].m_ItemList.Hand();
			if (nIdx)
			{
				Player[nPlayerIndex].m_ItemList.Remove(nIdx);
				KMapPos sMapPos;
				KObjItemInfo	sInfo;
				Player[nPlayerIndex].GetAboutPos(&sMapPos);
				sInfo.m_nItemID = nIdx;
				sInfo.m_dwItemID = Item[nIdx].GetID();
				sInfo.m_nItemWidth = Item[nIdx].GetWidth();
				sInfo.m_nItemHeight = Item[nIdx].GetHeight();
				sInfo.m_nMoneyNum = 0;
				strcpy(sInfo.m_szName, Item[nIdx].GetName());
				sInfo.m_nColorID = 0;
				//AutoAI by kinnox;
				sInfo.m_nGenre = Item[nIdx].GetGenre();
				sInfo.m_nDetailType = Item[nIdx].GetDetailType();
				sInfo.m_nMovieFlag = 1;
				sInfo.m_nSoundFlag = 1;
				sInfo.m_dwNpcId = 0;
				//end code;
				
				int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
				if (nObj == -1)
				{
					ItemSet.Remove(nIdx);
				}
				else
				{
					if (Item[nIdx].GetGenre() == item_task)
					{
						Object[nObj].SetEntireBelong(nPlayerIndex);
					}
					else
					{
						Object[nObj].SetItemBelong(nPlayerIndex);
					}
				}
			}
			Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_hand, 0 ,0);
		}
	}
	
	Lua_PushNumber(L, nIndex);
	return 1;
	
}

// --
//
// --
int LuaAddStackItem(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	BYTE btItemClass		= (BYTE)Lua_ValueToNumber(L, 1);
	if(btItemClass != item_task && btItemClass != item_script)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	short nDetailType		= (short)Lua_ValueToNumber(L, 2);
	int nParticularType		= (int)Lua_ValueToNumber(L, 3);
	char cSeries			= (char)Lua_ValueToNumber(L, 4);
	BYTE btLevel			= (BYTE)Lua_ValueToNumber(L, 5);
	BYTE btStackNum			= (BYTE)Lua_ValueToNumber(L, 6);
	BOOL bForceBind			= FALSE;
	int nExpiredTime		= 0;
	if(nParamNum >= 7)
	{	
		bForceBind			= (BOOL)Lua_ValueToNumber(L, 7);
		if(nParamNum >= 8)
		{
			nExpiredTime		= (int)Lua_ValueToNumber(L, 8);
		}
	}

	if(btStackNum <= 0)
	{
		btStackNum = 1;
	}
	
	int nIndex = ItemSet.Add(btItemClass, cSeries, btLevel, 0, 0, nDetailType, nParticularType,  NULL, g_SubWorldSet.GetGameVersion(), 0, btStackNum);
	if (nIndex <= 0 || nIndex >= MAX_ITEM)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	if(Item[nIndex].GetMaxStackCount() == 0)
	{	
		ItemSet.Remove(nIndex);
		Lua_PushNumber(L, 0);
		return 1;
	}

	if(bForceBind)
	{
		Item[nIndex].SetBindState(2);
	}

	Item[nIndex].SetTime(nExpiredTime);

	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_equiproom, x, y);
	}
	else
	{
		int		nIdx = Player[nPlayerIndex].m_ItemList.Hand();
		if (nIdx)
		{
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);
			KMapPos sMapPos;
			KObjItemInfo	sInfo;
			Player[nPlayerIndex].GetAboutPos(&sMapPos);
			sInfo.m_nItemID = nIdx;
			sInfo.m_dwItemID = Item[nIdx].GetID();
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			char szTemp[FILE_NAME_LENGTH];
			sprintf(szTemp, "%s x%d", Item[nIdx].GetName(), Item[nIdx].GetStackCount());
			strcpy(sInfo.m_szName, szTemp);
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code;
			int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
			if (nObj == -1)
			{
				ItemSet.Remove(nIdx);
			}
			else
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					Object[nObj].SetEntireBelong(nPlayerIndex);
				}
				else
				{
					Object[nObj].SetItemBelong(nPlayerIndex);
				}
			}
		}
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_hand, 0 ,0);
	}
	Lua_PushNumber(L, nIndex);
	return 1;
}

// --
//
// --
int LuaGetItemStackCount(Lua_State * L)
{
	if(Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nItemIndex = (int)Lua_ValueToNumber(L,1);
	if(nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if(Item[nItemIndex].GetMaxStackCount() == 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	Lua_PushNumber(L, Item[nItemIndex].GetStackCount());
	return 1;
}

// --
//
// --
int LuaDelStackItem(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nItemIndex = (int)Lua_ValueToNumber(L,1);

	BOOL bResult = Player[nPlayerIndex].m_ItemList.RemoveStackItem(nItemIndex);

	Lua_PushNumber(L,bResult);
	return 1;
}
//// --
//
//// --
int LuaUseStackItem(Lua_State * L)
{
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	//
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nItemIdx = (int)Lua_ValueToNumber(L, 1);

	int nResult = Player[nPlayerIndex].m_ItemList.UseStackItem(nItemIdx);
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaAddScriptItem(Lua_State * L)
{	
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nDetailType = (int)Lua_ValueToNumber(L,1);
	if(nDetailType < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	const int nItemIdx = ItemSet.Add(item_script, -1, 0, -1, 0, nDetailType, -1, NULL, g_SubWorldSet.GetGameVersion(), 0);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) ) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	if(nParamNum >= 2)
	{
		Item[nItemIdx].SetTime((DWORD)Lua_ValueToNumber(L,2));
	}
	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight(), &x, &y))
	{
		if( Item[nItemIdx].CanStack() )
			Player[nPlayerIndex].m_ItemList.Add_AutoStack(room_equipment, nItemIdx, x, y);
		else
			Player[nPlayerIndex].m_ItemList.Add(nItemIdx, pos_equiproom, x, y);

		char szData[128];
		//
		Item[nItemIdx].GetItemBackupInfo(szData);
		//
		Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD SCRIPT ITEM", Item[nItemIdx].GetName());

		Lua_PushNumber(L, nItemIdx);
	}
	else
	{
		ItemSet.Remove(nItemIdx);
		Lua_PushNumber(L, 0);
	}
	return 1;
}
// --
//
// --
int LuaAddMyItem(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nItemIndex = (int)Lua_ValueToNumber(L, 1);
	if(nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nPlace = (int)Lua_ValueToNumber(L, 2);
	if(nPlace < pos_hand || pos_hand > pos_builditem)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nX = 0, nY = 0;
	if(nParamNum >= 4)
	{
		nX = (int)Lua_ValueToNumber(L, 3);
		nY = (int)Lua_ValueToNumber(L, 4);
	}
	BOOL bResult = Player[nPlayerIndex].m_ItemList.Add(nItemIndex, nPlace, nX, nY);
	
	Lua_PushNumber(L,bResult);
	return 1;
}
// --
//
// --
int LuaIsMyItem(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nItemIndex = (int)Lua_ValueToNumber(L,1);
	BOOL bResult = Player[nPlayerIndex].m_ItemList.IsMyItem(nItemIndex);
	
	Lua_PushNumber(L,bResult);
	return 1;
}

// --
//
// --
int LuaDelMyItem(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	int nItemIndex = (int)Lua_ValueToNumber(L,1);
	BOOL bResult = Player[nPlayerIndex].m_ItemList.Remove(nItemIndex);
	
	Lua_PushNumber(L,bResult);
	return 1;
}

// --
//
// --
int LuaDelItemByIndex(Lua_State * L)
{	
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nItemIdx = (int)Lua_ValueToNumber(L,1);
	BOOL bResult = Player[nPlayerIndex].m_ItemList.RemoveByIndex(nItemIdx);

	Lua_PushNumber(L,bResult);
	return 1;
}

// --
//
// --
int LuaGetTaskItemCount(Lua_State * L)
{
	
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nDetailType = (int)Lua_ValueToNumber(L,1);
	if(nDetailType < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nCount = Player[nPlayerIndex].m_ItemList.GetTaskItemNum(nDetailType);

	Lua_PushNumber(L, nCount);
	return 1;
}
// --
//
// --
int LuaCountFreeBagCell(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nResult = Player[nPlayerIndex].m_ItemList.GetEmptyCellNumInEquipment();
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaGetItemCountInBag(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
	{	
		Lua_PushNil(L);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNil(L);
		return 1;
	}
	BYTE btGenre		= (BYTE)Lua_ValueToNumber(L, 1);
	short nDetail		= (short)Lua_ValueToNumber(L, 2);
	short nParticular	= (short)Lua_ValueToNumber(L, 3);
	char cSeries		= (char)Lua_ValueToNumber(L, 4);
	BYTE btLevel		= 0;
	if(nParamNum >= 5)
	{
		btLevel = (BYTE)Lua_ValueToNumber(L, 5);
	}
	int nResult = Player[nPlayerIndex].m_ItemList.GetSameItemNumInEquipment(btGenre, nDetail, nParticular, cSeries, btLevel);
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaConsumeItemInBag(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	BYTE btNum			= (BYTE)Lua_ValueToNumber(L, 1);
	BYTE btGenre		= (BYTE)Lua_ValueToNumber(L, 2);
	short nDetail		= (short)Lua_ValueToNumber(L, 3);
	short nParticular	= (short)Lua_ValueToNumber(L, 4);
	char cSeries		= (char)Lua_ValueToNumber(L, 5);
	BYTE btLevel		= (BYTE)Lua_ValueToNumber(L, 6);

	BOOL bResult = Player[nPlayerIndex].m_ItemList.RemoveItemInEquipment(btNum, btGenre, nDetail, nParticular, cSeries, btLevel);
	Lua_PushNumber(L, bResult);
	return 1;
}
// --
//
// --
int LuaSetItemBindState(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{	
		return 0;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		return 0;
	}
	int nIndex			= (int)Lua_ValueToNumber(L, 1);
	BYTE btState		= (BYTE)Lua_ValueToNumber(L, 2);
	BYTE btDay			= 0;
	if(nParamNum >= 3)
	{
		btDay = (BYTE)Lua_ValueToNumber(L, 3);
	}

	Player[nPlayerIndex].m_ItemList.SetItemBindState(nIndex, btState, btDay);
	return 0;
}
// --
//
// --
int LuaGetItemBindState(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{	
		Lua_PushNumber(L, -1);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, -1);
		return 1;
	}
	int nIndex			= (int)Lua_ValueToNumber(L, 1);

	Lua_PushNumber(L, (int)Player[nPlayerIndex].m_ItemList.GetItemBindState(nIndex));
	return 1;
}
// --
//
// --
int LuaGetItemProp(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{	
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		return 6;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		return 6;
	}
	int nIndex			= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
	{	
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		Lua_PushNumber(L, -1);
		return 6;
	}

	Lua_PushNumber(L, (int)Item[nIndex].GetGenre());
	Lua_PushNumber(L, (int)Item[nIndex].GetDetailType());
	Lua_PushNumber(L, (int)Item[nIndex].GetParticular());
	Lua_PushNumber(L, (int)Item[nIndex].GetLevel());
	Lua_PushNumber(L, (int)Item[nIndex].GetSeries());
	Lua_PushNumber(L, (int)Item[nIndex].GetGeneratorParam()->nLuck);
	return 6;
}
// --
//
// --
int LuaGetItemRandSeed(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	Lua_PushNumber(L, Item[nIndex].GetGeneratorParam()->dwRandomSeed);
	return 1;
}
// --
//
// --
int LuaGetItemVersion(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, (int)Item[nIndex].GetGeneratorParam()->nVersion);
	return 1;
}
// --
//
// --
int LuaSetItemGenre(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 2)
		return 0;
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nGenre	= (int)Lua_ValueToNumber(L, 2);
	
	Item[nIndex].SetGenre(nGenre);
	return 0;
}
// --

// --
int LuaSetItemParticular(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 2)
		return 0;
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nParticular	= (int)Lua_ValueToNumber(L, 2);
	
	Item[nIndex].SetParticular(nParticular);
	return 0;
}
// --
int LuaSetItemDetail(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 2)
		return 0;
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nDetail	= (int)Lua_ValueToNumber(L, 2);
	
	Item[nIndex].SetDetailType(nDetail);
	return 0;
}
// --
//
// --
// --
//
// --
int LuaSetItemRecord(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 2)
		return 0;
	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nRecord	= (int)Lua_ValueToNumber(L, 2);
	
	Item[nIndex].SetRecord(nRecord);
	return 0;
}
// --
//
// --
int LuaSetItemLevel(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nLevel	= (int)Lua_ValueToNumber(L, 2);
	if(nLevel <= 0 || nLevel > 10)
		return 0;
	Item[nIndex].SetLevel(nLevel);
	return 0;
}
// --
//
// --
int LuaSetItemSeries(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	int nIndex	= (int)Lua_ValueToNumber(L, 1);
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	int nSeries	= (int)Lua_ValueToNumber(L, 2);
	if(nSeries < 0 || nSeries > 4)
		return 0;
	Item[nIndex].SetSeries(nSeries);
	return 0;
}

int LuaSetItemMagicLevel(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 7)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return 0;
	
	int nItemLevel[6];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));
	nItemLevel[0] = (int)Lua_ValueToNumber(L, 2);
	nItemLevel[1] = (int)Lua_ValueToNumber(L, 3);
	nItemLevel[2] = (int)Lua_ValueToNumber(L, 4);
	nItemLevel[3] = (int)Lua_ValueToNumber(L, 5);
	nItemLevel[4] = (int)Lua_ValueToNumber(L, 6);
	nItemLevel[5] = (int)Lua_ValueToNumber(L, 7);
	
	Item[nItemIdx].SetGeneratorLevel(nItemLevel);
	return 0;
}
// --
//
// --
int LuaGetItemMagicLevel(Lua_State * L)
{	
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return 0;
	
	int nItemLevel[6];
	nItemLevel[0] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[0];
	nItemLevel[1] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[1];
	nItemLevel[2] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[2];
	nItemLevel[3] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[3];
	nItemLevel[4] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[4];
	nItemLevel[5] = Item[nItemIdx].GetGeneratorParam()->nGeneratorLevel[5];
	
	Lua_PushNumber(L, nItemLevel[0]);
	Lua_PushNumber(L, nItemLevel[1]);
	Lua_PushNumber(L, nItemLevel[2]);
	Lua_PushNumber(L, nItemLevel[3]);
	Lua_PushNumber(L, nItemLevel[4]);
	Lua_PushNumber(L, nItemLevel[5]);
	return 6;
}
// --
//
// --
int LuaSetItemLuck(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return 0;
	
	const int nLuck = (int)Lua_ValueToNumber(L, 2);
	
	Item[nItemIdx].SetGeneratorLuck(nLuck);
	return 0;
}
// --
//
// --
int LuaSetItemRandSeed(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
		return 0;
	
	const int nRandSeed = (int)Lua_ValueToNumber(L, 2);
	
	Item[nItemIdx].SetRandomSeed(nRandSeed);
	return 0;
}
// --
//
// --
int LuaRestoreBrokenEquip(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nResult = Player[nPlayerIndex].m_ItemList.RestoreBrokenEquip(nItemIdx);

	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaAddItemAgain(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nResult = Player[nPlayerIndex].m_ItemList.AddAgain(nItemIdx);

	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaGetItemExpiredTime(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nItemIdx	= (int)Lua_ValueToNumber(L, 1);
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	
	Lua_PushNumber(L, Item[nItemIdx].GetTime() );
	return 1;
}
//// ---
//
//// ---
int LuaGetItemInfo(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0) 
	{	
		Lua_PushNumber(L, 0);
		return 0;
	}
	int nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{	
		Lua_PushNumber(L, 0);
		return 0;
	}

	int nGameIdx = (int)Lua_ValueToNumber(L,1);
	if(nGameIdx <= 0)
	{
		Lua_PushNumber(L, -1);
		return 1;
	}

	int nInfo = (int)Lua_ValueToNumber(L,2);
	if (nInfo < 0 || nInfo > 27)
		nInfo = 0;
	switch(nInfo)
	{
		case 0:
			Lua_PushNumber(L,Item[nGameIdx].GetGenre());
			break;
		case 1:
			Lua_PushNumber(L,Item[nGameIdx].GetDetailType());
			break;
		case 2:
			Lua_PushNumber(L,Item[nGameIdx].GetParticular());
			break;
		case 3:
			Lua_PushNumber(L,Item[nGameIdx].GetLevel());
			break;
		case 4:
			Lua_PushNumber(L,Item[nGameIdx].GetSeries());
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			Lua_PushNumber(L,Item[nGameIdx].GetBindState());
			break;
		case 8:
			Lua_PushString(L,Item[nGameIdx].GetName());
			break;
		case 9:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(0));
			break;
		case 10:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(1));
			break;
		case 11:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(2));
			break;
		case 12:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(3));
			break;
		case 13:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(4));
			break;
		case 14:
			Lua_PushNumber(L,Item[nGameIdx].GetAttribType(5));
			break;
		case 15:
			Lua_PushNumber(L,Item[nGameIdx].GetMaxStackCount());
			break;
		case 16:
			Lua_PushNumber(L,Item[nGameIdx].GetStackCount());
			break;
		default:
			Lua_PushNumber(L, 0);
			break;
	}
	return 1;
}
// --
//
// --
int LuaGetHandItem(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.Hand());
	return 1;
}
// --
//
// --
int LuaGetEquipItem(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nPartNum = (int)Lua_ValueToNumber(L, 1);

	if(nPartNum < itempart_head || nPartNum >= itempart_num)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetEquipment(nPartNum));
	return 1;
}
// --
//
// --
int LuaAddGoldItem(Lua_State *L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	WORD wRecord = (WORD)Lua_ValueToNumber(L, 1);
	DWORD dwExpiredTime = 0;
	if(nParamNum >= 2)
	{
		dwExpiredTime = (DWORD)Lua_ValueToNumber(L, 2);
	}
	
	const int nItemIdx = ItemSet.Add(item_goldequip, -1, 0, wRecord, 0, -1, -1, NULL, g_SubWorldSet.GetGameVersion(), 0);
	
	if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) ) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}

	Item[nItemIdx].SetTime(dwExpiredTime);
	
	int		x, y;
	if (Player[nPlayerIndex].m_ItemList.CheckCanPlaceInEquipment(Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight(), &x, &y))
	{
		Player[nPlayerIndex].m_ItemList.Add(nItemIdx, pos_equiproom, x, y);

		char szData[128];
		//
		Item[nItemIdx].GetItemBackupInfo(szData);
		//
		Player[nPlayerIndex].SaveLog(2, szData, "LUA_ADD GOLD ITEM", Item[nItemIdx].GetName());
	}
	else
	{
		int	nIdx = Player[nPlayerIndex].m_ItemList.Hand();
		if(nIdx)
		{
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);
			
			KMapPos			sMapPos;
			KObjItemInfo	sInfo;
			
			Player[nPlayerIndex].GetAboutPos(&sMapPos);
			
			sInfo.m_nItemID = nIdx;
			sInfo.m_dwItemID = Item[nIdx].GetID();
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code;
			
			int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
			if(nObj == -1)
			{
				ItemSet.Remove(nIdx);
			}
			else
			{
				Object[nObj].SetItemBelong(nPlayerIndex);
			}
		}
		Player[nPlayerIndex].m_ItemList.Add(nItemIdx, pos_hand, 0 ,0);
	}		
	Lua_PushNumber(L, nItemIdx);
	return 1;
}
// --
//
// --
int LuaGetGoldItem(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	int nIndex = (int)Lua_ValueToNumber(L, 1);

	if (nIndex <= 0 || nIndex >= MAX_ITEM) 
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	Lua_PushNumber(L, Item[nIndex].GetGoldId());
	return 1;
}
// --
//
// --
int LuaGetItemMagicAttrib(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 2)
	{	
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 4;
	}
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 4;
	}

	int nIndex = (int)Lua_ValueToNumber(L, 1);
	if (nIndex <= 0 || nIndex >= MAX_ITEM) 
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 4;
	}
	BYTE btNo = (BYTE)Lua_ValueToNumber(L, 2);
	btNo -= 1;

	Lua_PushNumber(L, Item[nIndex].GetMagicAttrib(btNo, 0));
	Lua_PushNumber(L, Item[nIndex].GetMagicAttrib(btNo, 1));
	Lua_PushNumber(L, Item[nIndex].GetMagicAttrib(btNo, 2));
	Lua_PushNumber(L, Item[nIndex].GetMagicAttrib(btNo, 3));
	return 4;
}
// --
//
// --
int LuaSetItemParam(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 3)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nItemIndex = (int)Lua_ValueToNumber(L, 1);
	if(nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
		return 0;

	int nKind = (int)Lua_ValueToNumber(L,2);
	int nValue = (int)Lua_ValueToNumber(L,3);

	switch(nKind)
	{
	case 1:
		Item[nItemIndex].SetStackCount(nValue);
		break;
	default:
		break;
	}
	
	return 0;
}
// --
//
// --
int LuaGetItemParam(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNil(L);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nItemIndex		= (int)Lua_ValueToNumber(L, 1);
	if(nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
	{	
		Lua_PushNil(L);
		return 1;
	}
	int nKind = (int)Lua_ValueToNumber(L, 2);
	int nResult = -1;
	switch(nKind)
	{
	case 1:
		if( Item[nItemIndex].CanStack() )
			nResult = (int)Item[nItemIndex].GetStackCount();
		else 
			nResult = 1;
		break;
	default:
		break;
	}
	

	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaSyncItem(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nItemIndex = (int)Lua_ValueToNumber(L, 1);
	if(nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
		return 0;
	int nKind = (int)Lua_ValueToNumber(L, 2);
	switch(nKind)
	{
	case 1:
		ITEM_CHANGE_INFO sChange;
		sChange.ProtocolType	= s2c_itemchangeinfosync;
		sChange.m_btType		= 1;
		sChange.m_dwItemID		= Item[nItemIndex].GetID();
		sChange.m_uChange		= (UINT)Item[nItemIndex].GetStackCount();;
		if (g_pServer)
			g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
		break;
	default:
		break;
	}
	
	return 0;
}
// --
//
// --
int LuaDelEventItem(Lua_State * L)
{	
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nDetailType = (int)Lua_ValueToNumber(L,1);
	if(nDetailType < 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int	nCount = 1;
	
	if(nParamNum >= 2)
	{
		nCount = (int)Lua_ValueToNumber(L, 2);
	}

	int nResult = Player[nPlayerIndex].m_ItemList.RemoveTaskItem(nDetailType, nCount);

	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaHaveEventItem(Lua_State *L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	short nEventId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nEventId = (int)Lua_ValueToNumber(L, 1);
	}
	else
	{
		char* pEventName = (char*)Lua_ValueToString(L,1);
		if (!g_EventItemTab.GetInteger(pEventName, "DetailType", 0, (int*)&nEventId))
		{	
			Lua_PushNumber(L, 0);
			return 1;
		}
	}
	BOOL bResult = Player[nPlayerIndex].m_ItemList.IsTaskItemExist(nEventId);
	Lua_PushNumber(L, bResult);
	return 1;
}
// --
//
// --
int LuaAddMagic(Lua_State * L)
{
	//g_DebugLog("Begin Add Skill");
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
    int nNpcIdx = Player[nPlayerIndex].m_nIndex;
	int nLevel = 0;
	if (nParamCount >= 2)
	{
		nLevel = (int)Lua_ValueToNumber(L, 2);
	}
	else
	{
		nLevel = 0;
	}
    int nRet = Npc[nNpcIdx].m_SkillList.Add(nSkillId, nLevel);
#ifdef _DEBUG
	//g_DebugLog("[AddMagic] SkillID: %d - SkillLevel: %d - PointAdd: %d", nSkillId, nLevel, Npc[nNpcIdx].m_SkillList.GetPointAdd(nSkillId));
#endif
	if (nRet)
	{
		PLAYER_SKILL_LEVEL_SYNC NewSkill;
		NewSkill.ProtocolType	= s2c_playerskilllevel;
		NewSkill.m_nSkillID		= nSkillId;
		NewSkill.m_nSkillLevel	= Npc[nNpcIdx].m_SkillList.GetCurrentLevel(nSkillId);
		NewSkill.m_nLeavePoint	= Player[nPlayerIndex].m_nSkillPoint;
		NewSkill.m_btPointAdd	= Npc[nNpcIdx].m_SkillList.GetPointAdd(nSkillId);
		NewSkill.m_bExpand		= false;
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
		Player[nPlayerIndex].RefeshPlayer(TRUE);
	}
	
	return 0;
}
// --
//
// --
int LuaDelMagic(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	
    nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) 
			return 0;
	}
	
	Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Remove(nSkillId);
	PLAYER_SKILL_REMOVE_SYNC sRemove;
	sRemove.ProtocolType = s2c_playerskillremove;
	sRemove.m_nSkillID = nSkillId;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sRemove, sizeof(PLAYER_SKILL_REMOVE_SYNC));
	Player[nPlayerIndex].RefeshPlayer(TRUE);
	return 0;
}
// --
//
// --
int LuaHaveMagic(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	if (nParamCount < 1) 
		return 0;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) 
		{
			Lua_PushNumber(L, -1);
		}
		return 1;
	}
	
	if (Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.FindSame(nSkillId))
	{	
		int nLevel = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId);
		Lua_PushNumber(L, nLevel);
	}
	else
	{
		Lua_PushNumber(L, -1);
	}
	
	return 1;
}
// --
//
// --
int LuaGetSkillIdInSkillList(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillIndex = (int)Lua_ValueToNumber (L, 1);
	int nSkillId = 0;
	if (nSkillIndex > 0) 
	{
		nSkillId = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetSkillId(nSkillIndex);
	}
	Lua_PushNumber(L, nSkillId);
	return 1;
}
// --
//
// --
int LuaSetSkillLevel(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillId = 0;
	
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
	int nLevel = (int)Lua_ValueToNumber(L, 2);
	if (nLevel >= 0)
		Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.SetSkillLevelDirectlyUsingId(nSkillId, nLevel);
	return 0;
}
// --
//
// --
int LuaRollBackSkills(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nSkillId = 0;
	int nTotalSkill = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.ClearSkillLevel(true);
	Player[nPlayerIndex].SendSyncData_Skill();
	Lua_PushNumber(L, nTotalSkill);
	return 1;
}
// --
//
// --
int LuaUpdateSkillList(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	Player[nPlayerIndex].SendSyncData_Skill();
	return 0;
}
// --
//
// --
int LuaGetMagicLevel(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	
	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) return 0;
	
	int nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (int)Lua_ValueToNumber (L, 1);
	}
	else 
	{
		const char * sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char *)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0 ) return 0;
	}
	Lua_PushNumber(L,Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId));
	return 1;
	
}
// --
//
// --
int LuaGetNpcTemplateID(Lua_State * L)
{
	if (Lua_GetTopIndex(L) > 0)
	{
		if (Lua_IsString(L,1))
		{
			const char * pName = lua_tostring(L,1);	
			int nId = g_NpcSetting.FindRow((char*)pName) - 2;
			Lua_PushNumber(L, nId);
		}
		else
			return 0;
	}
	else 
		return 0;
	return 1;
}

// --
//
// --
int LuaAddNpc(Lua_State * L)
{
	const int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum < 6)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nTemplateID = (int)Lua_ValueToNumber(L, 1);	
	if( (nTemplateID < 0) || (nTemplateID >= MAX_NPCSTYLE) ) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nLevel = (int)Lua_ValueToNumber(L, 2);
	if( (nLevel <= 0) || (nLevel >= MAX_NPC_LEVEL) ) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nSubWorldIdx = (int)Lua_ValueToNumber(L, 3);
	const int nMpsX = (int)Lua_ValueToNumber(L, 4);
	const int nMpsY = (int)Lua_ValueToNumber(L, 5);

	const int nNpcIdxInfo = MAKELONG(nLevel, nTemplateID);

	int nSeries = ::GetRandomNumber(0,4);
	
	const int nNpcIdx = NpcSet.Add(nSeries, nNpcIdxInfo, nSubWorldIdx, nMpsX, nMpsY);
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	if(nParamNum >= 6)
	{	
		Npc[nNpcIdx].m_bRemoveWhenDeath = (BYTE)Lua_ValueToNumber(L, 6);
		
		char* pszName = NULL;
		pszName = (char*)Lua_ValueToString(L, 7);
		if(strlen(pszName) > 0)
		{
			g_StrCpy(Npc[nNpcIdx].Name, (char*)pszName);
			g_StrCpy(Npc[nNpcIdx].CustomName, (char*)pszName);
		}


		if(nParamNum >= 9)
		{	
			const int nGoldType = (int)Lua_ValueToNumber(L, 8);
			if(nGoldType > 0)
				Npc[nNpcIdx].m_cGold.SetGoldCurrentType(nGoldType);

			Npc[nNpcIdx].m_Kind = (int)Lua_ValueToNumber(L, 9);
		}
	}
	Lua_PushNumber(L, nNpcIdx);
	return 1;
}

int LuaAddNpcSkillState(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 5) 
		return 0;	

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex < 0 || nNpcIndex >= MAX_NPC) return 0;

	int nSkillId = (int)Lua_ValueToNumber(L, 2);
	int nSkillLevel = (int)Lua_ValueToNumber(L, 3);
	int nIfMagic = (int)Lua_ValueToNumber(L, 4);
	int nTime	 = (int)Lua_ValueToNumber(L, 5);

	if (nIfMagic)
	{
		KSkill *pSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, nSkillLevel);
		pSkill->CastStateSkill(nNpcIndex, 0, 0, nTime, TRUE);
	}
	else
	{
		KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
		memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
		DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
		DamageMagicAttribs[0].nValue[0] = 0;
		Npc[nNpcIndex].SetStateSkillEffect(nNpcIndex, nSkillId, nSkillLevel, DamageMagicAttribs, 1, nTime, TRUE);
	}
	return 0;
}

// --
//
// --
int LuaDelNpc(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0 ;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex > 0 && nNpcIndex < MAX_NPC)
	{
		if (Npc[nNpcIndex].m_Kind != kind_player && Npc[nNpcIndex].m_SubWorldIndex >= 0 && Npc[nNpcIndex].m_RegionIndex >= 0)
		{
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
		}
		NpcSet.Remove(nNpcIndex);
	}
	return 0;
}
// --
//
// --
int LuaClearMapNpc(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
	{
		Lua_PushNumber(L , 0);
		return 1;
	}
	int nSubWorldIndex = g_SubWorldSet.SearchWorld((int)Lua_ValueToNumber(L, 1));
	int nResult = 0;
	if (nSubWorldIndex >= 0)
	{
		for (int nNpcIndex=1;nNpcIndex<MAX_NPC;++nNpcIndex)
		{
			if (Npc[nNpcIndex].m_SubWorldIndex == nSubWorldIndex)
			{		
				if (Npc[nNpcIndex].m_RegionIndex >= 0 && Npc[nNpcIndex].m_Kind != kind_player) //fix by kinnox
				{
					SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
					SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
				}
					NpcSet.Remove(nNpcIndex);
					nResult++;
			}
		}
	}
	Lua_PushNumber(L , nResult);
	nResult = 0;
	return 1;
}
// --
//
// --
int LuaClearMapNpcWithName(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L , 0);
		return 1;
	}
	int nSubWorldIndex = g_SubWorldSet.SearchWorld((int)Lua_ValueToNumber(L, 1));

	int nResult = 0;
	if (nSubWorldIndex >= 0)
	{
		for (int nNpcIndex=1;nNpcIndex<MAX_NPC;++nNpcIndex)
		{
			if (Npc[nNpcIndex].m_SubWorldIndex == nSubWorldIndex)
			{
				if (strcmp(Npc[nNpcIndex].Name, (char*)Lua_ValueToString(L, 2)) == 0)
				{
					if (Npc[nNpcIndex].m_RegionIndex >= 0 && Npc[nNpcIndex].m_Kind != kind_player) //fix by kinnox
					{
						SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
						SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
					}
						NpcSet.Remove(nNpcIndex);
						nResult++;
				}
			}
		}
	}
	Lua_PushNumber(L , nResult);
	nResult = 0;
	return 1;
}
// --
//
// --
int LuaFindAroundNpc(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Lua_IsNumber(L, 1))
		{
			int nIndex = Player[nPlayerIndex].FindAroundNpc((DWORD)Lua_ValueToNumber(L,1));
			Lua_PushNumber(L, nIndex);
			return 1;
		}
		else
			Lua_PushNil(L);
	}
	return 0;
}
// --
//
// --
int LuaGetNpcID(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0 ;
	DWORD dwNameID;
	BYTE nType = (BYTE)Lua_ValueToNumber(L, 1);
	int nNpcIndex = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	switch (nType)
	{
	case 1:
		dwNameID = g_FileName2Id(Npc[nNpcIndex].Name);
		Lua_PushNumber(L, dwNameID);
		break;
	case 2:
		dwNameID = Npc[nNpcIndex].m_dwID;
		Lua_PushNumber(L, dwNameID);
		break;
	}
	return 1;
}
// --
//
// --
int LuaDelNpcsInRgn(Lua_State * L)
{
	//Question
	return 0;
}
// --
//
// --
int LuaDelNpcsInWld(Lua_State * L)
{
	return 0;
}
// --
//
// --
int LuaSetNpcPos(Lua_State * L)
{
	int nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 3) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0) return 0;
	Npc[nNpcIndex].m_MapX = (int)Lua_ValueToNumber(L, 2);
	Npc[nNpcIndex].m_MapY = (int)Lua_ValueToNumber(L, 3);
	
	return 0;
}
// --
//
// --
int LuaSetNpcActionScript(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2 ) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	strcpy(Npc[nNpcIndex].ActionScript, Lua_ValueToString(L,2));
	Npc[nNpcIndex].m_ActionScriptID = g_FileName2Id((char *)Lua_ValueToString(L,2));
	return 0;
}
// --
//
// --
int LuaSetPlayerRevivalPos(Lua_State * L)
{
	//Question
	int nParamCount = Lua_GetTopIndex(L);
	int nPlayerIndex = 0;
	int nBeginIndex = 2;
	nPlayerIndex = GetPlayerIndex(L);
	int nSubWorldId = 0;
	int nRevId = 0;
	if (nPlayerIndex < 0) 
	{
		return 0;
	}
	
	if (nParamCount >= 2)
	{
		nSubWorldId = (int) Lua_ValueToNumber(L, 1);
		nRevId = (int) Lua_ValueToNumber(L, 2);
	}
	else if (nParamCount == 1)
	{
		nSubWorldId = -1;
		nRevId = (int) Lua_ValueToNumber(L, 1);
	}
	else 
	{
		return 0;
	}
	
	Player[nPlayerIndex].SetRevivalPos(nSubWorldId, nRevId);
	return 0;
}
// --
//
// --

#define MacroFun_GetPlayerInfoInt(L, MemberName) { int nPlayerIndex = GetPlayerIndex(L);\
	if (nPlayerIndex > 0){	int nNpcIndex = Player[nPlayerIndex].m_nIndex;	if (nNpcIndex > 0)Lua_PushNumber(L, Npc[nNpcIndex].MemberName);\
	else Lua_PushNil(L);}\
	else Lua_PushNil(L);\
return 1;}														

// --
//
// --
int LuaGetPlayerCurrentCamp(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L, m_CurrentCamp);
}
// --
//
// --
int LuaGetPlayerCamp(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L, m_Camp);
}
// --
//
// --
int LuaSetPlayerCamp(Lua_State * L)
{
	int nValue = (int)Lua_ValueToNumber(L,1);
	if (nValue < 0 ) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCamp(nValue);
	}
	return 0;
}

// --
//
// --
int LuaSetPlayerCurrentCamp(Lua_State * L)
{
	int nValue = (int)Lua_ValueToNumber(L,1);
	if (nValue < 0 ) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCurrentCamp(nValue);
	}
	return 0;
}

// --
//
// --
int LuaSetNpcCurCamp(Lua_State * L)
{
	int nNpcIndex = (int)Lua_ValueToNumber(L,1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) 
		return 0;
	int nValue = (int )Lua_ValueToNumber(L,2);
	if (nValue >= camp_num) 
		return 0;
	Npc[nNpcIndex].SetCurrentCamp(nValue);
	return 0;
}
//// --
// 
//// --
int LuaSetNpcName(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2 ) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	strcpy(Npc[nNpcIndex].Name, Lua_ValueToString(L,2));
	strcpy(Npc[nNpcIndex].CustomName, Lua_ValueToString(L,2));
	return 0;
}
//// --
// 
//// --
int LuaSetNpcReduceDamge(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	
	int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;

	BYTE nParam = (BYTE)Lua_ValueToNumber(L, 2);
	Npc[nNpcIdx].m_nReducedDamge = nParam;

	return 0;
}
// --
//
// --
int LuaSetNpcSkill(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4) 
		return 0;
	
	int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;
	int nSkillID = (int)Lua_ValueToNumber(L, 2);
	if (nSkillID <= 0)
		return 0;
	int nSkillLevel = (int)Lua_ValueToNumber(L, 3);
	if (nSkillLevel < 1)
		nSkillLevel = 1;
	int nSkillPos = (int)Lua_ValueToNumber(L, 4);
	if (nSkillPos < 0 || nSkillPos > MAX_NPCSKILL)
		return 0;
	Npc[nNpcIdx].m_SkillList.SetNpcSkill(nSkillPos, nSkillID, nSkillLevel);
	return 1;
}
// --
//
// --
int LuaSetNpcLife(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	int nLife = (int)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0; 

	Npc[nNpcIndex].m_CurrentLife = nLife;
	Npc[nNpcIndex].m_CurrentLifeMax = nLife;

	if (nParamNum > 2 && (int)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_LifeMax = nLife;
	return 0;
}	
// --
//
// --

int LuaSetNpcDamage(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0 ;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetPhysicsDamage((int)Lua_ValueToNumber(L, 2), (int)Lua_ValueToNumber(L, 3));
	return 0;
}	
// --
//
// --
int LuaIsBoss(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 2 ) return 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	Npc[nNpcIndex].m_cGold.SetGoldCurrentType((int)Lua_ValueToNumber(L,2));

	return 0;
}
// --
//
// --
int LuaSetNpcSeries(Lua_State * L)
{
	int nNpcIndex = (int)Lua_ValueToNumber(L,1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) 
		return 0;
	int nValue = (int )Lua_ValueToNumber(L,2);
	if (nValue >= series_num) 
		return 0;
	Npc[nNpcIndex].SetSeries(nValue);
	return 0;
}
// --
// SetNpcOwner - Make NPC follow player
// Parameters: (npcIndex, playerName, followMode)
// --
int LuaSetNpcOwner(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	// Get player index from Lua context (the player who called this script)
	// This is more reliable than finding by name!
	int nPlayerIdx = GetPlayerIndex(L);
	if (nPlayerIdx < 0)
		return 0;

	int nFollowMode = 1; // Default: follow enabled
	if (Lua_GetTopIndex(L) >= 2)
		nFollowMode = (int)Lua_ValueToNumber(L, 2);

	// Validate player index
	if (nPlayerIdx < 0 || nPlayerIdx >= MAX_PLAYER)
		return 0;

	// Check if player is valid and online
	if (Player[nPlayerIdx].m_nIndex <= 0)
		return 0;

	// Check if NPC index is valid
	if (Npc[nNpcIndex].m_Index <= 0)
		return 0;

	// All validations passed - safe to set params
	// Use m_AiParam[] - universal array available everywhere (not #ifdef dependent)
	// m_AiParam[8] = owner player index
	// m_AiParam[9] = follow mode (1=follow, 0=disabled)
	Npc[nNpcIndex].m_AiParam[8] = nPlayerIdx;
	Npc[nNpcIndex].m_AiParam[9] = nFollowMode;

#ifdef _SERVER
	// Set m_nPeopleIdx to link NPC to player (only in server)
	Npc[nNpcIndex].m_nPeopleIdx = Player[nPlayerIdx].m_nIndex;
#endif

	// Set AI mode to 8 (Follow owner)
	if (nFollowMode == 1)
	{
		Npc[nNpcIndex].m_AiMode = 8;
	}

	// Make NPC peaceful/neutral
	Npc[nNpcIndex].m_Kind = 0; // Kind 0 = neutral

	return 0;
}
// --
//
// --
int LuaRestorePlayerCamp(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreCurrentCamp();
	}
	return 0;
}
// --
//
// --
int LuaOpenTong(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_OPENTONGUI;
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;

	UiInfo.m_bParam1 = 0;
	UiInfo.m_nBufferLen = sizeof(int);
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#endif
	return 0;
}
// --
//
// --
int LuaGetPlayerFaction(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{	
		char FactionName[100];
		Player[nPlayerIndex].GetFactionName(FactionName, 100);
		Lua_PushString(L, FactionName);
	}
	else
	{
		Lua_PushString(L,"");
	}
	return 1;
}
// --
//
// --
int LuaChangePlayerFaction(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);				
	int nResult = 0;
	if (nPlayerIndex > 0)							
	{
		const char * szFactionName = Lua_ValueToString(L,1);
		Player[nPlayerIndex].LeaveCurFaction();
		if (strlen(szFactionName) == 0) 
		{
			nResult = 1;
		}
		else
		{
			nResult = Player[nPlayerIndex].AddFaction((char *)szFactionName);
		}
	}
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaGetPlayerFactionNo(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)								
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	
	Lua_PushNumber(L, Player[nPlayerIndex].m_cFaction.GetCurFactionNo());
	return 1;
}
// --
//
// --
int LuaGetPlayerColdResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax);break;
		default:
			Lua_PushNil(L);
		} 
	}
	else 
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaSetPlayerColdResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax = nValue;
			break;
		}	
	} 
	return 0;
}
// --
//
// --
int LuaGetPlayerFireResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else 
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaSetPlayerFireResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax = nValue;
			break;
		}		
	} 
	return 0;
}
// --
//
// --
int LuaGetPlayerLightResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaSetPlayerLightResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nValue = (int)Lua_ValueToNumber(L,2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax;
		
		int nType = (int)Lua_ValueToNumber(L,1);
		
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist = nValue;
			break;
			
		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax = nValue;
			break;
		}
	} 
	else 
		Lua_PushNil(L);
	return 0;
}
// --
//
// --
int LuaGetPlayerPoisonResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist);break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax);break;
		default:
			Lua_PushNil(L);
		}
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaSetPlayerPoisonResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		{
			int nValue = (int)Lua_ValueToNumber(L,2);
            if (nValue < 0) nValue = 0;
            if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax;
			
            int nType = (int)Lua_ValueToNumber(L,1);
			
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist = nValue;
				break;
				
			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist = nValue;
                break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax = nValue;
                break;
			}
		}
		
	} 
	return 0;
}
// --
//
// --
int LuaGetPlayerPhysicsResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaSetPlayerPhysicsResist(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nValue = (int)Lua_ValueToNumber(L,2);
            if (nValue < 0) nValue = 0;
            if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax;
			
            int nType = (int)Lua_ValueToNumber(L,1);
			
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist = nValue;
				break;
				
			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist = nValue;
                break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax = nValue;
                break;
			}
		}
		
	} 
	return 0;
}

//	

int LuaGetPlayerExp(Lua_State *L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNil(L);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].m_nExp);
	return 1;
}


//	

int LuaGetPlayerExpPercent(Lua_State *L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNil(L);
		return 1;
	}
	
	int nPercent = ((Player[nPlayerIndex].m_nExp - Player[nPlayerIndex].m_nNextLevelExp + Player[nPlayerIndex].m_nNextLevelExp) * 100.0) / Player[nPlayerIndex].m_nNextLevelExp;
    Lua_PushNumber(L, nPercent);	
	return 1;
}


//	

int LuaModifyPlayerExp(Lua_State * L)
{
	int bAllTeamGet = 0;
	if(Lua_GetTopIndex(L) >= 3)		bAllTeamGet = (int)Lua_ValueToNumber(L,3);
	
	int nDValue = (int)Lua_ValueToNumber(L,1);
	int nTarLevel = (int)Lua_ValueToNumber(L,2);
	
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		if (bAllTeamGet)
			Player[nPlayerIndex].AddExp(nDValue, nTarLevel);
		else
			Player[nPlayerIndex].AddSelfExp(nDValue, nTarLevel);
	}
	return 0;
}
// --
//
// --
int LuaAddOwnExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <=0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		int nExp = (int)Lua_ValueToNumber(L,1);
		if (nExp >= 0)
			Player[nPlayerIndex].DirectAddExp((int)Lua_ValueToNumber(L,1));
	}
	return 0;
}
// --
//
// --
int LuaAddSumExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <=0 ) return 0;
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex > 0)								
	{									
		int nExp = (int)Lua_ValueToNumber(L,1);
		if (nExp >= 0)
		{
			while (nExp > 0)
			{
				int nExpAdd = Player[nPlayerIndex].m_nNextLevelExp - Player[nPlayerIndex].m_nExp;
				if (nExp >= nExpAdd)
				{
					nExp = nExp - nExpAdd;
				}
				else
				{
					nExpAdd = nExp;
					nExp = 0;
				}
				Player[nPlayerIndex].DirectAddExp(nExpAdd);
			}
		}
	}
	return 0;
}
// --
//
// --
int LuaSetLevel(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);				
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;	
	
	BYTE btLevel = (BYTE)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].SetLevel(btLevel);
	return 0;
}
// --
//
// --
int LuaGetPlayerLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaGetPlayerLife(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLife);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LifeMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LifeMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaRestorePlayerLife(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreLife();
	}
	return 0;
}
// --
//
// --
int LuaRestorePlayerMana(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreMana();
	}
	return 0;
}
// --
//
// --
int LuaRestorePlayerStamina(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreStamina();
	}
	return 0;
}
// --
//
// --
int LuaGetPlayerLifeReplenish(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt( L, m_LifeReplenish);
}
// --
//
// --
int LuaGetPlayerMana(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentMana);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaGetPlayerStamina(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		{
			int nType = (int)Lua_ValueToNumber(L,1);
			switch((int)Lua_ValueToNumber(L,1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentStamina);break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax);break;
			default:
				Lua_PushNil(L);
			}
		}
		
	} 
	else
		Lua_PushNil(L);
	return 1;
}
// --
//
// --
int LuaGetPlayerManaReplenish(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_ManaReplenish);
}
// --
//
// --
int LuaGetPlayerDefend(Lua_State * L)
{
    MacroFun_GetPlayerInfoInt(L , m_Defend);
}
// --
//
// --
int LuaGetPlayerSex(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_nSex);
}
// --
//
// --
int LuaGetPlayerIndex(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , GetPlayerIdx());
	return 0;
}
// --
//
// --
int LuaGetPlayerSeries(Lua_State * L)
{
	MacroFun_GetPlayerInfoInt(L , m_Series);
}
// --
//
// --
int LuaSetPlayerSeries(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nValue = (int)Lua_ValueToNumber(L,1);
		Npc[Player[nPlayerIndex].m_nIndex].SetSeries(nValue);
	} 
	return 0;
    
}

//	

int LuaGetPlayerCount(Lua_State * L)
{
	Lua_PushNumber(L, PlayerSet.GetPlayerNumber());
	return 1;
}

//

int LuaGetItemCount(Lua_State * L)
{
	Lua_PushNumber(L, ItemSet.GetItemNumber());
	return 1;
}

//	

int LuaGetObjCount(Lua_State * L)
{
	Lua_PushNumber(L, ObjSet.GetObjNumber());
	return 1;
}

//

int LuaGetNpcCount(Lua_State * L)
{
	Lua_PushNumber(L, NpcSet.GetNpcCount());
	return 1;
}

//	

int LuaGetPlayerAccount(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNil(L);
		return 1;
	}
    Lua_PushString(L, Player[nPlayerIndex].m_AccoutName);
	return 1;	
}

//

int LuaGetPlayer(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);
	if(nParamNum == 0)
	{
		Lua_PushNumber(L, PlayerSet.GetFirstPlayerEx());
	}
	else
	{	
		int nCurIdx = (int)Lua_ValueToNumber(L, 1);
		Lua_PushNumber(L, PlayerSet.GetNextPlayerEx(nCurIdx));
	}
	return 1;	
}


//

int LuaGetLastDiagNpc(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].m_nLastDialogIdx);
	return 1;
}


//	

int LuaGetPlayerName(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNil(L);
		return 1;
	}
	Lua_PushString(L, Player[nPlayerIndex].m_PlayerName);
	return 1;
	
}


//
#ifdef _SERVER
int LuaGetPlayerDamage(Lua_State * L)
{

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nNewIndex = (int)Lua_ValueToNumber(L, 1);
	nPlayerIndex = nNewIndex;
	int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Npc[nNpcIndex].m_cDeathCalcExp.GetDamage());
	return 1;
}

int LuaClearPlayerDamage(Lua_State * L)  
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, 0);
		return 1; 
	}

	int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}

	Npc[nNpcIndex].m_cDeathCalcExp.Clear();
	 return 0;
}

#endif

int LuaGetPlayerDamageCounter(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
	{	
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Npc[nNpcIndex].m_cDeathCalcExp.GetDamage());
	return 1;
	
}


//	

int LuaClearPlayerDamageCounter(Lua_State * L) 
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;
	
	int nNpcIndex = Player[nPlayerIndex].m_nIndex;
	if(nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_cDeathCalcExp.Clear();
	return 0;
	
}

//	

int LuaSetPlayerName(Lua_State * L)
{	
	return 0;
}


//	

int LuaGetPlayerID(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNil(L);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].m_dwID);
	return 1;
}
// --
//
// --
int LuaGetPlayerLeadExp(Lua_State * L)
{
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_dwLeadExp);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}
// --
//
// --
int LuaGetPlayerLeadLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_dwLeadLevel);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}
// --
//
// --
int LuaGetPlayerRestAttributePoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_nAttributePoint);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}
// --
//
// --
int LuaGetPlayerRestSkillPoint(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
        Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	}
	else
		Lua_PushNil(L);
	
	return 1;
}
// --
//
// --
/*
int LuaModifyPlayerRestSkillPoint(Lua_State *L)
{
//Question
int nPlayerIndex = GetPlayerIndex(L);
if (nPlayerIndex > 0)
{
int nDValue = (int)Lua_ValueToNumber(L, 1);

  int nNewSkillPoint = Player[nPlayerIndex].m_nSkillPoint + nDValue;
  if (nNewSkillPoint < 0 ) return 0;
  
	Player[nPlayerIndex].m_nSkillPoint = nNewSkillPoint;
	}
	
	  return 0;
	  }
*/
// --
//
// --
int LuaGetPlayerLucky(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurLucky);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
}
// --
//
// --
int LuaGetPlayerEngergy(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurEngergy);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy);break;
		default:
			Lua_PushNil(L);
		}
	}
	
	return 1;
}
// --
//
// --
int LuaGetPlayerDexterity(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurDexterity);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
}
// --
//
// --
int LuaGetPlayerStrength(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurStrength);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength);break;
		default:
			Lua_PushNil(L);
		}
	} 
	return 1;
	
}
// --
//
// --
int LuaGetPlayerVitality(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		
		int nType = (int)Lua_ValueToNumber(L,1);
		switch((int)Lua_ValueToNumber(L,1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurVitality);break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality);break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality);break;
		default:
			Lua_PushNil(L);
		}
		
	} 
	return 1;
	
}
// --
//
// --
int LuaGetPlayerCashMoney(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
	
    if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment));
	}
	else Lua_PushNumber(L,0);
	
	return 1;
}

int LuaPlayerPayMoney(Lua_State * L)
{
    
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        if (Player[nPlayerIndex].Pay(nMoney))
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}
// --
//
// --
int LuaPlayerEarnMoney (Lua_State  *L)
{
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        Player[nPlayerIndex].Earn(nMoney);
	}
	return 0;	
}
// --
//
// --
int LuaPlayerPrePayMoney(Lua_State *L)
{   
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex > 0)
	{
        int nMoney = (int)Lua_ValueToNumber(L, 1);
        if (nMoney <= 0) return 0;
        if (Player[nPlayerIndex].PrePay(nMoney))
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}
// --
//
// --
int LuaPlayerExecuteScript(Lua_State * L)
{
	
	return 1;
}
// --
//
// --
int LuaAttackNpc(Lua_State * L)
{
	int nParamCount = 0;
	if ( (nParamCount = Lua_GetTopIndex(L)) < 2) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L,1);
	int nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);//NpcSet.SearchID(nNpcID);
	if (nNpcIndex <= 0) return 0;
	
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 65534;
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	for (int i = 0; i < nParamCount - 1; i++)
	{
		int nVlau = (int)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[0] = (int)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[2] = (int)Lua_ValueToNumber(L, 2 + i);
	}
	
	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}
// --
//
// --
int LuaSetPlayerChatForbiddenFlag(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	int nFlag = Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].SetChatForbiddenFlag(nFlag);
	return 0;
}
// --
//
// --
int LuaKickOutPlayer(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char*	pszName = (const char*)Lua_ValueToString(L, 1);
	
	int nIndex = PlayerSet.GetFirstPlayer();
	while(nIndex > 0)
	{
		if (strcmp(Player[nIndex].m_PlayerName, pszName) == 0)
			break;
		
		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out specific player.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}
// --
//
// --
int LuaKickOutAccount(Lua_State *L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char*	pszName = (const char*)Lua_ValueToString(L, 1);
	
	int nIndex = PlayerSet.GetFirstPlayer();
	while(nIndex > 0)
	{
		if (strcmpi(Player[nIndex].m_AccoutName, pszName) == 0)
			break;
		
		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out specific player.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}
// --
//
// --
int LuaKickOutSelf(Lua_State *L)
{
	int nIndex = GetPlayerIndex(L);
	
	if (nIndex <= 0)
		return 0;
	
	if (Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out player one.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}
// --
//
// --
int LuaKillNpc(Lua_State * L)
{
	int nParamCount = 0;
	if ( (nParamCount = Lua_GetTopIndex(L)) < 1) return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L,1);
	int nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);
	if (nNpcIndex <= 0) return 0;
	
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 50000;
	
	
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	
	DamageMagicAttribs[2].nValue[0] = 50000;
	DamageMagicAttribs[2].nValue[2] = 50000;
	
	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}
// --
//
// --
int LuaKillPlayer(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	
	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = 50000;
	
	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;
	
	
	DamageMagicAttribs[2].nValue[0] = 50000;
	DamageMagicAttribs[2].nValue[2] = 50000;
	
	Npc[Player[nPlayerIndex].m_nIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, 0, DamageMagicAttribs, 0, TRUE);
	return 0;
}
// --
//
// --
int LuaSetFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Npc[Player[nPlayerIndex].m_nIndex].SetFightMode(Lua_ValueToNumber(L,1) != 0);
	return 0;
}
// --
//
// --
int LuaGetFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FightMode);
	return 1;
}
// --
//
// --
int LuaGetLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)		Lua_PushNumber(L,0);
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	return 1;
}
// --
//
// --
int	LuaUseTownPortal(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	Player[nPlayerIndex].UseTownPortal();
	return 0;
}
// --
//
// --
int LuaReturnFromTownPortal(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	Player[nPlayerIndex].BackToTownPortal();
	return 0;
}
#endif
// --
//
// --
int GetPlayerIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
    if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_PLAYER || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}	if (Player[nIndex].m_nIndex >= MAX_NPC || Player[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}
// --
//
// --
int GetObjIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_OBJINDEX);
    if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_OBJECT || nIndex <= 0) 
	{
		_ASSERT(0);
		return -1;
	}
	if (Object[nIndex].m_nIndex != nIndex)
	{
		_ASSERT(0);
		return -1;
	}
    return nIndex;
}
// --
//
// --
int  LuaMessage(Lua_State * L)
{
	const char * szString;
	szString  = lua_tostring (L,1);
	//g_DebugLog((char *)szString);
	return 0;
}
// --
//
// --
#ifdef _SERVER
int LuaAddPlayerWayPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nWayPoint = (int)Lua_ValueToNumber(L,1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode * pNode = (KIndexNode*) Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
		while(pNode)
		{
			if (pNode->m_nIndex == nWayPoint) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}
		
		KIndexNode * pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nWayPoint;
		int nCount = Player[nPlayerIndex].m_PlayerWayPointList.GetNodeCount();
		for (int i = 0; i < nCount - 2; i ++ )
		{
			KIndexNode *  pDelNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerWayPointList.RemoveHead();
			delete pDelNode;
			pDelNode = NULL; //them code 16/09/2023 by kinnox;
		}
		Player[nPlayerIndex].m_PlayerWayPointList.AddTail(pNewNode);
	}
	return 0;
}
// --
//
// --
int LuaAddPlayerStation(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nStation = (int )Lua_ValueToNumber(L,1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode * pNode = (KIndexNode*) Player[nPlayerIndex].m_PlayerStationList.GetHead();
		while(pNode)
		{
			if (pNode->m_nIndex == nStation) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}
		
		KIndexNode * pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nStation;
		Player[nPlayerIndex].m_PlayerStationList.AddTail(pNewNode);
	}
	return 0;
}
// --
//
// --
int LuaGetPlayerStationCount(Lua_State *L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Player[nPlayerIndex].m_PlayerStationList.GetNodeCount());
	return 1;
}
// --
//
// --
int LuaGetPlayerStation(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	int nStationId = 0;
	KIndexNode * pNode = 	(KIndexNode*)Player[nPlayerIndex].m_PlayerStationList.GetHead();
	if (pNode)
	{
		int nNo = (int )Lua_ValueToNumber(L, 1);
		int nCurStation = (int) Lua_ValueToNumber(L,2);
		int nVisitNo = 0;
		while(pNode)
		{
			if (pNode->m_nIndex != nCurStation && g_GetPriceToStation( nCurStation , pNode->m_nIndex) > 0) 
			{
				nVisitNo ++;
				if (nVisitNo == nNo)
				{
					nStationId = pNode->m_nIndex;
					break;
				}
			}
			pNode = (KIndexNode*)pNode->GetNext();
		}
	}
	
	Lua_PushNumber(L, nStationId);
	
	return 1;
}
// --
//
// --
int LuaGetPlayerWayPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	
	KIndexNode * pNode = (KIndexNode*)	Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
	if (pNode)
	{
		int nNo = (int)Lua_ValueToNumber(L, 1);
		if (nNo > TASKVALUE_MAXWAYPOINT_COUNT) 
			Lua_PushNumber(L, 0);
		else
		{
			for (int i = 0; i < nNo - 1; i ++)
			{
				if (pNode == NULL ) break;
				pNode = (KIndexNode *)pNode->GetNext();
			}
			
			if (pNode)
				Lua_PushNumber(L, pNode->m_nIndex);
			else
				Lua_PushNumber(L, 0);
			
		}
	}
	else
		Lua_PushNumber(L, 0);
	
	return 1;
}
// --
//
// --
int LuaGetStationName(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0 ) 
	{
		Lua_PushString(L, "");
		return 1;
	}
	int nStationId = (int)Lua_ValueToNumber(L,1);
	char szName[50];
	g_StationTabFile.GetString(nStationId + 1, "DESC", "ÎÞÃû³Ç",  szName, 50 );
	Lua_PushString(L, szName);
	return 1;
}
// --
//
// --
int LuaGetWayPointName(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0 ) 
	{
		Lua_PushString(L, "");
		return 1;
	}
	int nWayPointId = (int)Lua_ValueToNumber(L,1);
	char szName[50];
	g_WayPointTabFile.GetString(nWayPointId + 1, "DESC", "Î´¼ÇÂ¼",  szName, 50 );
	Lua_PushString(L, szName);
	return 1;
}
// --
//
// --
int LuaGetAllStationCount(Lua_State * L)
{
	int nCityCount = g_StationTabFile.GetHeight() - 1;
	if (nCityCount < 0) nCityCount = 0;
	Lua_PushNumber(L,nCityCount);
	return 1;
}
// --
//
// --
int LuaGetCity(Lua_State * L)
{
	return 0;
}
// --
//
// --
int LuaGetPriceToWayPoint(Lua_State *L)
{
	
	int nCurStation = (int)Lua_ValueToNumber(L,1);
	int nDesWayPoint = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToWayPoint(nCurStation, nDesWayPoint));
	return 1;
}
// --
//
// --
int LuaGetPriceToStation(Lua_State *L)
{
	int nCurStation	 = (int)Lua_ValueToNumber(L,1);
	int nNextStation = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToStation(nCurStation, nNextStation));
	return 1;
}
// --
//
// --
int LuaGetStationPos(Lua_State * L)
{
	int nStationId = (int)Lua_ValueToNumber(L,1);
	char szPos[100] ;
	int nCount = 0;
	int nRow = g_StationTabFile.FindColumn("COUNT");
	g_StationTabFile.GetInteger(nStationId + 1, nRow,  0, &nCount);
	if (nCount <= 0) return 0;
	int nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_StationTabFile.FindColumn(szSectName);
	g_StationTabFile.GetString(nStationId + 1, nRow, "0,0,0", szValue, 100);
	
    int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);
	
    Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}
// --
//
// --
int LuaGetWayPointPos(Lua_State * L)
{
	int nWayPointId = (int)Lua_ValueToNumber(L,1);
	char szPos[100] ;
	int nCount = 0;
	char szValue[30];
	int nRow;
	nRow = g_WayPointTabFile.FindColumn("SECT");
	g_WayPointTabFile.GetString(nWayPointId + 1, nRow, "0,0,0", szValue, 30);
	int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);
	
	Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}
// --
//
// --
int LuaGetRank(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L,Npc[Player[nPlayerIndex].m_nIndex].m_btRankId);
	return 1;
}
// --
//
// --
int LuaSetRank(Lua_State * L)
{
	BYTE btRank = (BYTE)Lua_ValueToNumber(L,1);
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Npc[Player[nPlayerIndex].m_nIndex].m_btRankId = btRank;
	return 0;
}

//

int LuaGetTitle(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].GetTitle());
	return 1;
}

//

int LuaSetTitle(Lua_State * L)
{
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	WORD wTitle = (WORD)Lua_ValueToNumber(L,1);

	Player[nPlayerIndex].SetTitle(wTitle);
	return 0;
}
// --
//
// --
int LuaSetObjPropState(Lua_State *L)
{
	int  nParamNum = ( int ) Lua_GetTopIndex(L);
	int nState  = 1;
	
	if (nParamNum >= 1)
	{
		nState = (int)Lua_ValueToNumber(L,1);
		nState = (nState == 0)?0 : 1;
	}
	
	int nIndex = 0;
	if ((nIndex = GetObjIndex(L)) < 0) 
		return 0;
	
	Object[nIndex].SetState(nState);
	return 0;
}
// --
//
// --
int	LuaGetServerName(Lua_State * L)
{
	char szServerName[100]  ;
	unsigned long   stServerNameLen = 100;

#ifndef __linux
	if (GetComputerName(szServerName, &stServerNameLen))
	{
		Lua_PushString(L, szServerName);
	}
	else
#else
	if (SOCKET_ERROR != gethostname(szServerName, sizeof(szServerName)))
	{
		Lua_PushString(L, szServerName);
	}
	else
#endif
	Lua_PushString(L, "");

	return 1;
}
// --
//
// --
int LuaGetDockCount(Lua_State * L)
{
	int nCount = 0;
	int nCurStation = 0;
	int nTotalCount = 0;
	int i  = 0;
	if (Lua_GetTopIndex(L) < 1 )  
	{
		goto DockCount;
	}
	
	nCurStation = (int)Lua_ValueToNumber(L,1);
	nTotalCount = g_DockPriceTabFile.GetHeight() - 1;
	
	for (i = 0; i < nTotalCount; i ++)
	{
		int nPrice = g_GetPriceToDock(nCurStation, i + 1);
		if (nPrice > 0) nCount ++;
	}
	
DockCount:
	Lua_PushNumber(L, nCount);
	return 1;
}
// --
//
// --
int LuaGetDockPrice(Lua_State * L)
{
	int nCurDock = (int)Lua_ValueToNumber(L,1);
	int nDesDock = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, g_GetPriceToDock(nCurDock, nDesDock));
	return 1;
}
// --
//
// --
int LuaGetDock(Lua_State * L)
{
	int nCurDock = (int)Lua_ValueToNumber(L, 1);
	int nDock = (int)Lua_ValueToNumber(L, 2);
	int nCount = 0;
	int nTotalCount = g_DockPriceTabFile.GetHeight() - 1;
	int nGetDock = 0;
	
	for (int i = 0; i < nTotalCount; i ++)
	{
		int nPrice = g_GetPriceToDock(nCurDock, i + 1);
		if (nPrice > 0) 
		{
			nCount ++ ;
			if (nCount == nDock)
			{
				nGetDock = i + 1;
				break;
			}
		}
	}
	Lua_PushNumber(L, nGetDock);
	return 1;
}
// --
//
// --
int LuaGetDockName(Lua_State * L)
{
	int nDock  = (int)Lua_ValueToNumber(L, 1);
	char szName[100] ;
	
	if (nDock > g_DockPriceTabFile.GetHeight() - 1)
	{
		strcpy(szName, "Î´ÖªÂëÍ·");
		goto DockName;
	}
	
	g_DockTabFile.GetString(nDock + 1, "DESC", "Î´ÖªÂëÍ·", szName, 100);
	
DockName:
	Lua_PushString (L, szName);
	return 1;
}
// --
//
// --
int LuaGetDockPos(Lua_State * L)
{
	int nDock  = (int)Lua_ValueToNumber(L, 1);
	if (nDock > g_DockTabFile.GetHeight() - 1) 
	{
		printf("GetWharfPos Script Is Error!");
		return 0;
	}
	
	char szPos[100] ;
	int nCount = 0;
	int nRow = g_DockTabFile.FindColumn("COUNT");
	g_DockTabFile.GetInteger(nDock + 1, nRow,  0, &nCount);
	if (nCount <= 0) return 0;
	int nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_DockTabFile.FindColumn(szSectName);
	g_DockTabFile.GetString(nDock + 1, nRow, "0,0,0", szValue, 100);
	int nX, nY, nWorld;
    const char *pcszTemp = szValue;
    
    nWorld = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nX = KSG_StringGetInt(&pcszTemp, 0);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    nY = KSG_StringGetInt(&pcszTemp, 0);

	Lua_PushNumber(L,nWorld);
	Lua_PushNumber(L,nX);
	Lua_PushNumber(L,nY);
	return 3;
}
// --
//
// --
int LuaGetWayPointFightState(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	int nFightState = 0;
	if (Lua_GetTopIndex(L) > 0 ) 
	{
		int nWayPointId = (int)Lua_ValueToNumber(L,1);
		g_WayPointTabFile.GetInteger(nWayPointId + 1, "FightState", 0,  &nFightState);
	}
	Lua_PushNumber(L, nFightState);
	return 1;
}
// --
//
// --
int LuaSetMissionValue(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;

	int nSubWorldIndex = -1;
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;
	
	int nValueId = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	
	if (nValueId  < 0)
		return 0;
	SubWorld[nSubWorldIndex].m_MissionArray.SetMissionValue(nValueId, nValue);
	return 0;
}
// --
//
// --
int LuaGetMissionValue(Lua_State * L)
{
	int nResultValue = 0;
	int nSubWorldIndex = -1;
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		goto lab_getmissionvalue;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nSubWorldIndex >= 0) 
	{
		int  nValueId = (int)Lua_ValueToNumber(L, 1);
		if (nValueId > 0)
			nResultValue = SubWorld[nSubWorldIndex].m_MissionArray.GetMissionValue(nValueId);
	}
	
lab_getmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}

//

int LuaSetMissionString(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	
	int nSubWorldIndex = -1;
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0) 
		return 0;

	int nValueId = (int)Lua_ValueToNumber(L, 1);
	char* lpString = (char*)Lua_ValueToString(L, 2);
	
	if (nValueId  < 0)
		return 0;

	SubWorld[nSubWorldIndex].m_MissionArray.SetMissionString(nValueId, lpString);
	return 1;
}

//

int LuaGetMissionString(Lua_State * L)
{
	char* lpszResult = NULL;
	int nSubWorldIndex = -1;

	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		goto lab_getmissionstring;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nSubWorldIndex >= 0) 
	{
		int  nValueId = (int)Lua_ValueToNumber(L, 1);
		if (nValueId > 0)
		{	
			lpszResult = SubWorld[nSubWorldIndex].m_MissionArray.GetMissionString(nValueId);
		}
	}
	
lab_getmissionstring:
	Lua_PushString(L, lpszResult);
	return 1;
}
// --
//
// --
int LuaSetGlobalMissionValue(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	
	int nValueId = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	
	if (nValueId  < 0)
		return 0;
	g_GlobalMissionArray.SetMissionValue(nValueId, nValue);
	return 0;
}
// --
//
// --
int LuaGetGlobalMissionValue(Lua_State * L)
{
	int nResultValue = 0;
	int nValueId = 0;
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1) 
		goto lab_getglobalmissionvalue;
	nValueId = (int)Lua_ValueToNumber(L, 1);
	if (nValueId < 0)
		goto lab_getglobalmissionvalue;
	
	nResultValue = g_GlobalMissionArray.GetMissionValue(nValueId);
	
lab_getglobalmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}
// --
//
// --
int LuaInitMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD) 
		return 0;

	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 || nMissionId >= MAX_MISSION_VALUE_COUNT)
		return 0;
	
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
		return 0;
		
	pMission = SubWorld[nSubWorldIndex].m_MissionArray.Add();
	if (pMission)
	{
		pMission->m_MissionPlayer.Clear();
		pMission->SetMissionId(nMissionId);
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if (szScript[0])
			SubWorld[nSubWorldIndex].ExecuteScript(szScript, "InitMission", 0);
	}
	return 0;
}
// --
//
// --
int LuaRunMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD) 
		return 0;

	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 || nMissionId >= MAX_MISSION_VALUE_COUNT)
		return 0;

	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if(szScript[0])
			SubWorld[nSubWorldIndex].ExecuteScript(szScript, "RunMission", 0);
	}
	return 0;
}
// --
//
// --
int LuaCloseMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if(nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD) 
		return 0;

	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if(nMissionId < 0 || nMissionId >= MAX_MISSION_VALUE_COUNT)
		return 0;

	KMission StopMission;
	StopMission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&StopMission);
	if(pMission)
	{
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if(szScript[0])
			SubWorld[nSubWorldIndex].ExecuteScript(szScript, "EndMission", 0);
		pMission->StopMission();
		SubWorld[nSubWorldIndex].m_MissionArray.Remove(pMission);
		
	}
	return 0;
}
// --
//
// --
int LuaJoinMission(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;

	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0 || nMissionId >= MAX_MISSION_VALUE_COUNT)
		return 0;

	int nGroupId = (int)Lua_ValueToNumber(L, 2);
	if (nGroupId <= 0)
		return 0;
	
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		char szScript[MAX_PATH];
		g_MissionTabFile.GetString(nMissionId + 1, 2, "", szScript, MAX_PATH);
		if (szScript[0])
			SubWorld[nSubWorldIndex].ExecuteScript(szScript, "JoinMission", nPlayerIndex, nGroupId);
	}
	
	return 0;
}
// --
//
// --
int LuaStopMissionTimer(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2) 
		return 0;
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	int nTimerId = (int)Lua_ValueToNumber(L, 2);
	int nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nMissionId < 0 || nTimerId < 0 ) 
		return 0;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun StopTimer;
			StopTimer.SetTimer(1, nTimerId);
			KTimerTaskFun * pTimer = pMission->m_cTimerTaskSet.GetData(&StopTimer);
			if (pTimer)
			{
				pTimer->CloseTimer();
				pMission->m_cTimerTaskSet.Remove(pTimer);
			}
		}
		
	}
	
	return 0;
}
// --
//
// --
int LuaStartMissionTimer(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 3) 
		return 0;
	int nMissionId = (int)Lua_ValueToNumber(L, 1);
	int nTimerId = (int)Lua_ValueToNumber(L, 2);
	int nTimeInterval = (int)Lua_ValueToNumber(L, 3);
	int nSubWorldIndex = GetSubWorldIndex(L);
	
	if (nMissionId < 0 || nTimerId < 0 || nTimeInterval < 0) 
		return 0;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun * pTimer = pMission->m_cTimerTaskSet.Add();
			if (pTimer)
			{
				pTimer->SetTimer(nTimeInterval, nTimerId);
			}
		}
		
	}
	return 0;
}
// --
//
// --
int LuaSetDeathRevivalPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0) 
		return 0;
	int nParamCount = Lua_GetTopIndex(L);
	
	PLAYER_REVIVAL_POS * pTempRev = Player[nPlayerIndex].GetDeathRevivalPos();
	
	if (nParamCount > 2)
	{
		pTempRev->m_nSubWorldID  = (int) Lua_ValueToNumber(L, 1);
		pTempRev->m_nMpsX = (int) Lua_ValueToNumber(L, 2);
		pTempRev->m_nMpsY = (int) Lua_ValueToNumber(L, 3); 
	}
	else if (nParamCount == 1)
	{
		pTempRev->m_nSubWorldID = SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
		POINT Pos;
		int nRevId = (int) Lua_ValueToNumber(L, 1);
		g_SubWorldSet.GetRevivalPosFromId(pTempRev->m_nSubWorldID, nRevId, &Pos);
		pTempRev->m_ReviveID = nRevId;
		pTempRev->m_nMpsX = Pos.x;
		pTempRev->m_nMpsY = Pos.y;
	}
	else 
	{
		return 0;
	}
	
	return 0;
}

int LuaSetClearPos(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	int nParamCount = Lua_GetTopIndex(L);

	if(nParamCount > 1)
	{
		return 0;
	}
	Player[nPlayerIndex].ClearPortal();
	return 0;
}
// --
//
// --
int LuaAddMissionPlayer(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nMissionId = 0;
	int nPlayerIndex = 0;
	int nGroupId = 0;
	if (nParamCount >=3)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nPlayerIndex = (int )Lua_ValueToNumber(L,2);
		nGroupId = (int) Lua_ValueToNumber(L,3);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
		nPlayerIndex = GetPlayerIndex(L);
	}
	
	if (nMissionId < 0 || nPlayerIndex <= 0 || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->AddPlayer(nPlayerIndex, Player[nPlayerIndex].m_dwID, nGroupId);
		}
	}
	return 0;
}

int LuaRemoveMissionPlayer(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nMissionId = 0;
	int nPlayerIndex = 0;
	int nGroupId = 0;
	if (nParamCount >=3)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nPlayerIndex = (int )Lua_ValueToNumber(L,2);
		nGroupId = (int) Lua_ValueToNumber(L,3);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
		nPlayerIndex = GetPlayerIndex(L);
	}
	
	if (nMissionId < 0 || nPlayerIndex <= 0 || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->RemovePlayer(nPlayerIndex);
		}
	}
	return 0;
}
// --
//
// --
int LuaGetNextPlayer(Lua_State * L)
{
	unsigned long nPlayerIndex = 0;
	
	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L ,0);
		Lua_PushNumber(L, 0);
		return 2;
	}
	
	int nMissionId	= (int)Lua_ValueToNumber(L, 1);
	int nIdx		= (int)Lua_ValueToNumber(L, 2);
	int nGroup		= (int)Lua_ValueToNumber(L, 3);
	int nSubWorldIndex = GetSubWorldIndex(L);
	int nResultIdx = 0;
	
	if (nMissionId < 0 || nIdx < 0 || nGroup < 0) 
		goto lab_getnextplayer;
	
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResultIdx = pMission->GetNextPlayer(nIdx, nGroup, nPlayerIndex);
		}
	}
	
lab_getnextplayer:	
	Lua_PushNumber(L, nResultIdx);
	Lua_PushNumber(L, nPlayerIndex);
	return 2;
}
// --
//
// --
int LuaMissionMsg2Group(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	int	nGroupId = (int) Lua_ValueToNumber(L, 3);
	
	if (nMissionId < 0 || !strMsg || nGroupId <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2Group(strMsg, nGroupId);
		}
	}
	
	return 0;
}
// --
//
// --
int LuaMissionMsg2All(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	
	if (nMissionId < 0 || !strMsg)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2All(strMsg);
		}
	}
	
	return 0;
}
// --
//
// --
int LuaMissionMsg2Player(Lua_State * L)
{
	int nMissionId = (int)Lua_ValueToNumber(L,1);
	char * strMsg = (char *)Lua_ValueToString(L, 2);
	int	nPlayerIndex = (int) Lua_ValueToNumber(L, 3);
	
	if (nMissionId < 0 || !strMsg || nPlayerIndex <0)
		return 0;
	
	int nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->Msg2Group(strMsg, nPlayerIndex);
		}
	}
	
	return 0;
}
// --
//
// --
int LuaMissionPlayerCount(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	unsigned long ulCount = 0;
	int nMissionId = 0;
	int nGroupId = 0;
	int nSubWorldIndex = 0;
	if (nParamCount < 1) 
		goto lab_getmissionplayercount;
	
	if (nParamCount >= 2)
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
		nGroupId = (int) Lua_ValueToNumber(L,2);
	}
	else
	{
		nMissionId = (int)Lua_ValueToNumber(L,1);
	}
	
	if (nMissionId < 0 || nGroupId <0)
		goto lab_getmissionplayercount;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			ulCount = pMission->GetGroupPlayerCount(nGroupId);
		}
	}
	
lab_getmissionplayercount:
	Lua_PushNumber(L, ulCount);
	return 1;
}

//	

int LuaSetPlayerDeathScript(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) 
		return 0;
	char * szScript = (char *)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].m_dwDeathScriptId = g_FileName2Id(szScript);
	return 0;
}

//	

int LuaSetPlayerLogoutScript(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;
	char * szScript = (char *)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].m_dwLogoutScriptId = g_FileName2Id(szScript);
	return 0;
}
// --
//
// --
int LuaNpcIndexToPlayerIndex(Lua_State * L)
{
	int nResult = 0;
	int nNpcIndex = (int)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <=  0 || nNpcIndex >= MAX_NPC)
		goto lab_npcindextoplayerindex;
	
	if (Npc[nNpcIndex].m_Index > 0 && Npc[nNpcIndex].IsPlayer())
	{
		if (Npc[nNpcIndex].GetPlayerIdx() > 0)
			nResult = Npc[nNpcIndex].GetPlayerIdx();
	}
	
lab_npcindextoplayerindex:
	Lua_PushNumber(L, nResult);
	return 1;
	
}
// --
//
// -- 
int LuaGetMissionPlayer_PlayerIndex(Lua_State * L)
{
	unsigned long nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayer_npcindex;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nDataIndex < 0)
			goto lab_getmissionplayer_npcindex;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_PlayerIndex(nDataIndex);
		}
	}
	
lab_getmissionplayer_npcindex:
	Lua_PushNumber(L,nResult);
	return 1;
}
// --
//
// --
int LuaGetMissionPlayer_DataIndex(Lua_State * L)
{
	unsigned long nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayer_dataindex;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nPlayerIndex = (int)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nPlayerIndex < 0)
			goto lab_getmissionplayer_dataindex;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_DataIndex(nPlayerIndex);
		}
	}
	
lab_getmissionplayer_dataindex:
	Lua_PushNumber(L,nResult);
	return 1;
}
// --
//
// --
int LuaSetMissionPlayerParam(Lua_State * L)
{
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 4) 
		return 0;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		int nParamId =	 (int)Lua_ValueToNumber(L ,3);
		int nValue =	 (int )Lua_ValueToNumber(L, 4);
		
		if (nMissionId < 0 || nDataIndex < 0 || nParamId > 2)
			return 0;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nParamId == 1)
				pMission->m_MissionPlayer.SetParam1(nDataIndex, nValue);
			else
				pMission->m_MissionPlayer.SetParam2(nDataIndex, nValue);
		}
	}
	return 0;
}
// --
//
// --
int LuaGetMissionPlayerParam(Lua_State * L)
{
	int nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 3) 
		goto lab_getmissionplayerparam;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nDataIndex = (int)Lua_ValueToNumber(L, 2);
		int nParamId =	 (int)Lua_ValueToNumber(L ,3);
		
		if (nMissionId < 0 || nDataIndex < 0 || nParamId > 2)
			goto lab_getmissionplayerparam;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nParamId == 1)
				nResult = pMission->m_MissionPlayer.GetParam1(nDataIndex);
			else
				nResult = pMission->m_MissionPlayer.GetParam2(nDataIndex);
		}
	}
lab_getmissionplayerparam:
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaGetPlayerMissionGroup(Lua_State * L)
{
	int nResult = 0;
	int nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2) 
		goto lab_getmissionplayergroup;
	
	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0) 
	{
		int nMissionId = (int)Lua_ValueToNumber(L, 1);
		int nNpcIndex = (int)Lua_ValueToNumber(L, 2);
		
		if (nMissionId < 0 || nNpcIndex < 0)
			goto lab_getmissionplayergroup;
		
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission * pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_GroupId(nNpcIndex);
		}			
	}
lab_getmissionplayergroup:
	Lua_PushNumber(L ,nResult);
	return 1;
	
}
// --
//
// --
int LuaSetPlayerRevivalOptionWhenLogout(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	int nType = (int)Lua_ValueToNumber(L, 1);
	
	if (nType)
		Player[nPlayerIndex].SetLoginType(1);
	else
		Player[nPlayerIndex].SetLoginType(0);
	
	return 0;
}
// --
//
// --
int LuaSetPlayerPKValue(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;	
	char cPKValue = (char)Lua_ValueToNumber(L,1);
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) 
		return 0;
	Player[nPlayerIndex].m_cPK.SetPKValue(cPKValue);
	return 0;
}

// --
//
// --
int LuaGetPlayerPKValue(Lua_State * L)
{
	int nPKValue = 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		goto lab_getplayerpkvalue;
	
	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) 
		goto lab_getplayerpkvalue;
	nPKValue = Player[nPlayerIndex].m_cPK.GetPKValue();
	
lab_getplayerpkvalue:
	Lua_PushNumber(L, nPKValue);
	return 1;
}
// --
//
// --
int LuaForbidChangePK(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	bool bFlag = (bool)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].m_cPK.SetForbidChangePK(bFlag);
	return 0;	
}
// --
//
// --
int LuaSetPlayerPKFlag(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;	
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	BYTE btPKFlag = (BYTE)Lua_ValueToNumber(L,1);

	Player[nPlayerIndex].m_cPK.SetNormalPKState(btPKFlag);
	return 0;	
}
// --
//
// --
int LuaGetPlayerPKFlag(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, -1);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].m_cPK.GetNormalPKState());
	return 1;
}
// --
//
// --
int LuaForbitTrade(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	bool bFlag = (bool)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].m_cTrade.SetForbitTrade(nPlayerIndex, bFlag);
	return 0;	
}

// --
//
// --
int	LuaGetCurNpcIndex(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNumber(L,0);
		return 1;
	}
	
	Lua_PushNumber(L, Player[nPlayerIndex].m_nIndex);
	return 1;
}
// --
//
// --
int LuaShowLadder(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) 
		return 0;
	int nLadderCount = (DWORD) Lua_ValueToNumber(L, 1);
	if (nLadderCount <= 0)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	
	if (nLadderCount > nParamCount - 1 )
		nLadderCount = nParamCount - 1;
	BYTE Buffer[sizeof(LADDER_LIST) + 50 * sizeof(DWORD)];
	LADDER_LIST * pLadderList = (LADDER_LIST*)&Buffer;
	pLadderList->ProtocolType = s2c_ladderlist;
	pLadderList->nCount = nLadderCount;
	pLadderList->wSize = sizeof(LADDER_LIST) + nLadderCount * sizeof(DWORD) - 1;
	for (int i = 0; i < nLadderCount; i ++)
	{
		pLadderList->dwLadderID[i] = (DWORD)Lua_ValueToNumber(L, i + 2);
	}
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &Buffer, pLadderList->wSize + 1);
	return 0;
	
}
// --
//
// --
int LuaSwearBrother(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	int nTeamId = Lua_ValueToNumber(L, 1);
	
	if (nTeamId >= MAX_TEAM || nTeamId < 0)
		return 0;
	
	KPlayerChat::STRINGLIST BrotherList;
	_ASSERT(g_Team[nTeamId].m_nCaptain > 0);

	std::string strCapName;
	strCapName = Npc[Player[g_Team[nTeamId].m_nCaptain].m_nIndex].Name;
	BrotherList.push_back(strCapName);

	for (int i  = 0; i < g_Team[nTeamId].m_nMemNum; i++)
	{
		int nPlayerIndex = g_Team[nTeamId].m_nMember[i];
		if ( nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
		{
			std::string strName;
			strName = 	Npc[Player[nPlayerIndex].m_nIndex].Name;
			BrotherList.push_back(strName);
		}
	}
	
	KPlayerChat::MakeBrother(BrotherList);
	return 0;
}
// --
//
// --
int LuaMakeEnemy(Lua_State * L)
{
	
	return 0;
}
// --
//
// --
int LuaAddLeadExp(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	
	int nLeadExp = (int)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].AddLeadExp(nLeadExp);
	return 0;
}
// --
//
// --
int LuaGetLeadLevel(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nLeadLevel = 0;
	if (nPlayerIndex <= 0)
		goto lab_getleadlevel;
	nLeadLevel = (int)Player[nPlayerIndex].m_dwLeadLevel;
	
lab_getleadlevel:
	Lua_PushNumber(L, nLeadLevel);
	return 1;
}
// --
//
// --
int LuaAddMagicPoint(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	int nSkillPoint = (int)Lua_ValueToNumber(L, 1);
	
	Player[nPlayerIndex].m_nSkillPoint += nSkillPoint;
	if (Player[nPlayerIndex].m_nSkillPoint < 0)
		Player[nPlayerIndex].m_nSkillPoint = 0;
	S2C_PLAYER_POINT_SYNC NetCommand;
	NetCommand.ProtocolType = s2c_playerpointsync;
	NetCommand.nType = 1;
	NetCommand.nValue = nSkillPoint;
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
			g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_PLAYER_POINT_SYNC));
	return 0;
}
// --
//
// --
int LuaGetMagicPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nSkillPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getmagicpoint;
	
lab_getmagicpoint:
	Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	return 1;
}
// --
//
// --
int LuaAddSkillState(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 4)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	
	int nSillId = (int)Lua_ValueToNumber(L, 1);
	int nSkillLevel= (int)Lua_ValueToNumber(L, 2);
	BOOL bIfMagic = (BOOL)Lua_ValueToNumber(L, 3);
	int nTime = (int)Lua_ValueToNumber(L, 4);

	if(bIfMagic)
	{	
		KSkill *pSkill = (KSkill*)g_SkillManager.GetSkill(nSillId, nSkillLevel);
		if(nTime < -1)
		{
			pSkill->CastStateSkill(Player[nPlayerIndex].m_nIndex, 0, 0, 0, TRUE);
		}
		else
		{
			pSkill->CastStateSkill(Player[nPlayerIndex].m_nIndex, 0, 0, nTime);
		}
	}
	else
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetStateSkillEffect(Player[nPlayerIndex].m_nIndex, nSillId, nSkillLevel, NULL, 0, nTime);
	}

	return 0;
}

// --
//
// --
int LuaCastSkill(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;
	if(Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC)
		return 0;

	int nSkillId = 0;
	int nSkillLevel = 0;

	nSkillId = (int)Lua_ValueToNumber(L, 1);
	nSkillLevel = (int)Lua_ValueToNumber(L, 2);
	
	if(!nSkillId)
		return 0;
	if(!nSkillLevel)
		return 0;

	int nMapY = Player[nPlayerIndex].m_nIndex;
	Npc[Player[nPlayerIndex].m_nIndex].AutoDoSkill(nSkillId, nSkillLevel, -1, nMapY);
	return 0;
}
// --
//
// --

int	LuaClearStateSkill(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex > MAX_PLAYER)
		return 0;
	
	BOOL bEC = Lua_ValueToNumber(L,1);

	if(bEC != 1)
		return 0;

	Player[nPlayerIndex].RefeshPlayer(TRUE);
	return 1;
}
// --
//
// --
int LuaAddPropPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nPropPoint = 0;
	if (nPlayerIndex <= 0)
		return 0;
	nPropPoint = (int)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].m_nAttributePoint += nPropPoint;
	S2C_PLAYER_POINT_SYNC NetCommand;
	NetCommand.ProtocolType = s2c_playerpointsync;
	NetCommand.nType = 0;
	NetCommand.nValue = nPropPoint;
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_PLAYER_POINT_SYNC));
	return 0;
}
// --
//
// --
int LuaPayExtPoint(Lua_State * L)
{
	int nResult = 0;
	int nPay = 0;
	int nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_payextpoint;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_payextpoint;
	nPay = Lua_ValueToNumber(L, 1);
	if (nPay < 0)
		goto lab_payextpoint;
	nResult = Player[nPlayerIndex].PayExtPoint(nPay);

lab_payextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaGetExtPoint(Lua_State * L)
{
	int nResult = 0;
	int nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_getextpoint;
	
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getextpoint;
	
	nResult = Player[nPlayerIndex].GetExtPoint();
	
lab_getextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}
// --
//
// --
int LuaAddJbCoin(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;
	const int nAddCoin = Player[nPlayerIndex].GetBronzeCoin() + (int)Lua_ValueToNumber(L, 1);
	if(nAddCoin > 0)
		Player[nPlayerIndex].SetBronzeCoin(nAddCoin);
	return 0;
}
// --
//
// --
int LuaGetJbCoin(Lua_State * L)
{		
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].GetBronzeCoin());
	return 1;
}
// --
//
// --
int LuaPayJbCoin(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nPayCoin = (int)Lua_ValueToNumber(L, 1);
	if(nPayCoin <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].PayBronzeCoin(nPayCoin) );
	return 1;
}
// --
//
// --
int LuaAddLockBronzeCoin(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;
	
	const int nAddCoin = Player[nPlayerIndex].GetLockBronzeCoin() + (int)Lua_ValueToNumber(L, 1);
	if(nAddCoin > 0)
		Player[nPlayerIndex].SetLockBronzeCoin(nAddCoin);
	return 0;
}

// --
//
// --
int LuaGetLockBronzeCoin(Lua_State * L)
{	
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].GetLockBronzeCoin());
	return 1;
}

// --
//
// --
int LuaPayLockBronzeCoin(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	const int nPayCoin = (int)Lua_ValueToNumber(L, 1);
	if(nPayCoin <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	Lua_PushNumber(L, Player[nPlayerIndex].PayLockBronzeCoin(nPayCoin) );
	return 1;
}

// --
//
// --
int LuaClearLockBronzeCoin(Lua_State * L)
{	
	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;
	Player[nPlayerIndex].SetLockBronzeCoin(0);
	return 0;
}
// --
//
// --
int LuaGetRestPropPoint(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	int nPropPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getrestproppoint;
	nPropPoint = Player[nPlayerIndex].m_nAttributePoint;
	
lab_getrestproppoint:
	Lua_PushNumber(L, nPropPoint);
	return 1;
}

// --
//
// --
int LuaResetBaseAttribute(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	char cSeries = 0;
	cSeries =  (char)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].ResetBaseAttribute(cSeries);
	return 0;
}
// --
//
// --
int LuaMsgToGameMaster(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if ( nParamCount < 1 ) 
		return 0;
	int nParamID = 0;
	const char * szMsg = Lua_ValueToString(L, 1) ;
	if (!szMsg) 
		return 0;

	if (nParamCount < 2)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (int) Lua_ValueToNumber(L, 2);
	}

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0) 
	{
		char szID[32];

		sprintf(szID, "%d", nParamID);
		//KPlayerChat::SendInfoToGM(Player[nPlayerIndex].m_AccoutName, Npc[Player[nPlayerIndex].m_nIndex].Name, (char *) szMsg, strlen(szMsg) );
		KPlayerChat::SendInfoToGM("GM", szID, (char *) szMsg, strlen(szMsg) );
	}
	return 0;
}

//Msg2IP(IP, ID, str)
// --
//
// --
int LuaMsgToIP(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if ( nParamCount < 3 ) 
		return 0;
	int nIP = 0;
	const char * szIP = Lua_ValueToString(L, 1) ;
	nIP = _a2ip(szIP);
	if (nIP == 0)
		return 0;

	int nID = (int) Lua_ValueToNumber(L, 2);
	const char * szMsg = Lua_ValueToString(L, 3) ;
	if (!szMsg) 
		return 0;

	int nParamID = 0;
	if (nParamCount < 4)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (int) Lua_ValueToNumber(L, 4);
	}
	char szID[32];
	sprintf(szID, "%d", nParamID);
	KPlayerChat::SendInfoToIP(nIP, nID, "GM", szID, (char *) szMsg, strlen(szMsg) );
	return 0;
}

// --
//
// --
int LuaGetPlayerInfo(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	char szDesMsg[250];
	char szMsg[800];
	int nNpcIdx = 0;
	KNpc * pNpc = NULL;
	KPlayer* pPlayer = NULL;
	szDesMsg[0] = 0;
	if (nPlayerIndex <= 0)
		goto lab_getplayerinfo;
	
	pPlayer = &Player[nPlayerIndex];
	nNpcIdx = pPlayer->m_nIndex;
	pNpc = &Npc[nNpcIdx];
	sprintf(szMsg, "Name:%s,Lvl:%d,IP:%s,Lf:%d|%d,Mn:%d|%d,Mny:%d,sp:%d,ap:%d", pNpc->Name, pNpc->m_Level, g_pServer->GetClientInfo(pPlayer->m_nNetConnectIdx), pNpc->m_CurrentLife, pNpc->m_CurrentLifeMax, pNpc->m_CurrentMana, pNpc->m_CurrentManaMax, pPlayer->m_ItemList.GetMoney(room_equipment), pPlayer->m_nSkillPoint, pPlayer->m_nAttributePoint );
	g_StrCpyLen(szDesMsg, szMsg, 250);
	
lab_getplayerinfo:
	Lua_PushString(L, szDesMsg);
	return 1;
}

// --
//
// --
int LuaGetIP(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	char szDesMsg[200];
	szDesMsg[0] = 0;
	KPlayer * pPlayer = NULL;
	if (nPlayerIndex <= 0)
		goto lab_getplayerip;
	pPlayer = &Player[nPlayerIndex];
	strcpy(szDesMsg, g_pServer->GetClientInfo(pPlayer->m_nNetConnectIdx));
	
lab_getplayerip:
	Lua_PushString(L, szDesMsg);
	return 1;
}

// --
//
// --
int LuaSetDeathPunish(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	int nState = (int)Lua_ValueToNumber(L, 1);
	if (nState == 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = 0;
	}
	else
	{
		Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = enumDEATH_MODE_PKBATTLE_PUNISH;
	}

	return 0;
}

//// --
//
//// --
int LuaGetMapName(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
    int nPlayerIndex = GetPlayerIndex(L);
    int nWorldID = Lua_ValueToNumber(L,1);

	KIniFile MapListIni;
	MapListIni.Load("\\settings\\MapList.ini");
	char TypeTitle[20];
	char LocalString[256];
	sprintf(TypeTitle, "%d_name", nWorldID);
	MapListIni.GetString("List", TypeTitle, "", LocalString, sizeof(LocalString));

	Lua_PushString(L, LocalString);

    return 1;
}

//

int LuaGetMapInfoFile(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
	{	
		Lua_PushNil(L);
		return 0;
	}
	
	int nSubWorldID  = 0;
	int nTargetSubWorld  = 0;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	

	if (nTargetSubWorld <= 0 || nTargetSubWorld >= MAX_SUBWORLD)
	{	
		Lua_PushNil(L);
		return 0;
	}
	
	Lua_PushString(L, SubWorld[nTargetSubWorld].m_szMapInfo);
	return 1;
}

//

int LuaAddMapTrap(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 4)
		return 0;
	
	int nSubWorldID  = 0;
	int nTargetSubWorld  = 0;
	int nMpsX = 0;
	int nMpsY = 0;
	DWORD dwScriptId = 0;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	

	if (nTargetSubWorld <= 0 || nTargetSubWorld >= MAX_SUBWORLD)
		return 0;

	nMpsX = (int)Lua_ValueToNumber(L, 2);
	nMpsY = (int)Lua_ValueToNumber(L, 3);
	dwScriptId = (DWORD)Lua_ValueToNumber(L, 4);

	SubWorld[nTargetSubWorld].AddTrap(nMpsX, nMpsY, dwScriptId);
   	return 1;
}

//

int LuaAddMapTrapByCell(Lua_State * L)
{
	if(Lua_GetTopIndex(L) < 4)
		return 0;
	
	const int nSubWorldIndex = GetSubWorldIndex(L);
	if( (nSubWorldIndex < 0) || (nSubWorldIndex >= MAX_SUBWORLD) )
		return 0;

	const int nRegionId = (int)Lua_ValueToNumber(L, 1);
	const int nCellX = (int)Lua_ValueToNumber(L, 2);
	const int nCellY = (int)Lua_ValueToNumber(L, 3);
	const DWORD dwScriptId = g_FileName2Id( (char*)Lua_ValueToString(L, 4) );

	SubWorld[nSubWorldIndex].m_Region[nRegionId].AddTrap(nCellX, nCellY, dwScriptId);
   	return 1;
}

//

int LuaAddMapObject(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;
	
	const int nSubWorldIndex = GetSubWorldIndex(L);
	if( (nSubWorldIndex < 0) || (nSubWorldIndex >= MAX_SUBWORLD) )
		return 0;

	const int nObjID = (int)Lua_ValueToNumber(L, 1);
	const int nMpsX = (int)Lua_ValueToNumber(L, 2);
	const int nMpsY = (int)Lua_ValueToNumber(L, 3);
	const int nDir = (int)Lua_ValueToNumber(L, 4);
	const int nState = (int)Lua_ValueToNumber(L, 5);
	const char* pszScript = (char*)Lua_ValueToString(L, 6);
	
	ObjSet.ServerAdd(nObjID, nSubWorldIndex, nMpsX, nMpsY, nDir, nState, pszScript);

   	return 1;
}

//

int LuaCountNpcInMap(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	int nSubWorldID  = 0;
	int nTargetSubWorld  = 0;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	

	if (nTargetSubWorld <= 0 || nTargetSubWorld >= MAX_SUBWORLD)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	
	int nCount =  SubWorld[nTargetSubWorld].CountAllNpc();
	
	Lua_PushNumber(L, nCount);
	return 1;
}



int LuaClearMapObj(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nSubWorldID  = 0;
	int nTargetSubWorld  = 0;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	

	if (nTargetSubWorld <= 0 || nTargetSubWorld >= MAX_SUBWORLD)
		return 0;
	
	SubWorld[nTargetSubWorld].RemoveAllObj();
	return 1;
}

//

int LuaClearMapTrap(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	
	int nSubWorldID  = 0;
	int nTargetSubWorld  = 0;
	
	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);	

	if (nTargetSubWorld <= 0 || nTargetSubWorld >= MAX_SUBWORLD)
		return 0;
	
	SubWorld[nTargetSubWorld].RemoveAllTrap();
	return 1;
}

//

int	LuaSetLoginDelaySync(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	bool bDelay = (bool)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].LoginDelaySync(bDelay);
	return 0;
}

//

int	LuaMyRefresh(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;
	
	bool bClear = (bool)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].RefeshPlayer(bClear);
	return 0;
}

//

int	LuaReloadScript(Lua_State * L)
{
	Lua_PushNumber(L, ReLoadScript((char*)Lua_ValueToString(L, 1)));
	return 1;
}

//

int	LuaReloadAllScript(Lua_State * L)
{
	Lua_PushNumber(L, ReLoadAllScript());
	return 1;
}

//

int LuaSetMask(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 1)
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;

	if (Lua_IsNumber(L,1))
	{
		Player[nPlayerIndex].m_ItemList.SetMaskLock(TRUE);
		Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = (WORD)Lua_ValueToNumber(L,1);
	}
	return 0;
}

//

int LuaResetMask(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) 
		return 0;
	Player[nPlayerIndex].m_ItemList.SetMaskLock(FALSE);
	int nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_mask);
	
	if(nIdx <= 0 || nIdx >= MAX_ITEM)
	{
		Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = 0;
		return 0;
	}
	Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = (WORD)Item[nIdx].GetBaseMagic();
	return 0;
}

//

int LuaChangeOwnFeature(Lua_State * L)
{	
	if(Lua_GetTopIndex(L) < 3)
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;
	
	WORD wMaskId = (WORD)Lua_ValueToNumber(L,1);
	WORD wSkillId = (WORD)Lua_ValueToNumber(L,2);
	int nTime = (int)Lua_ValueToNumber(L,3);

	Player[nPlayerIndex].m_ItemList.SetMaskLock(TRUE);
	Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = wMaskId;
	if(wSkillId > 0)
	{
		KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(wSkillId, 1);
		if(nTime < -1)
		{
			pSkill->CastStateSkill(Player[nPlayerIndex].m_nIndex, 0, 0, 0, TRUE);
		}
		else
		{
			pSkill->CastStateSkill(Player[nPlayerIndex].m_nIndex, 0, 0, nTime);
		}
	}
	return 0;
}

//

int LuaRestoreOwnFeature(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if(nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) 
		return 0;

	Player[nPlayerIndex].m_ItemList.SetMaskLock(FALSE);
	int nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_mask);
	
	if(nIdx <= 0 || nIdx >= MAX_ITEM)
	{
		Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = 0;
		return 0;
	}
	Npc[Player[nPlayerIndex].m_nIndex].m_wMaskType = (WORD)Item[nIdx].GetBaseMagic();
	return 0;
}


//

int LuaForbidTownPortal(Lua_State * L)
{
    if(Lua_GetTopIndex(L) < 1)
		return 0;

    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;

	BOOL bForbid = (BOOL)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].m_bForbidTownPortal = bForbid;
	return 0; 
}

//

int LuaGetTownPortalFlag(Lua_State * L)
{
    int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{	
		Lua_PushNil(L);
		return 1;
	}

	BOOL bForbid = (BOOL)Lua_ValueToNumber(L,1);

	Lua_PushNumber(L, Player[nPlayerIndex].m_bForbidTownPortal);
	return 1; 
}




//

int LuaGetGameTime(Lua_State * L)
{
	DWORD dwResult = g_SubWorldSet.GetGameTime() / 18;
	Lua_PushNumber(L, dwResult);
	return 1;
}

//

int LuaFileName2Id(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;

	char* pszFileName = (char*)Lua_ValueToString(L, 1);
	
	DWORD dwResult = 0;
	
	dwResult = g_FileName2Id(pszFileName);
	
	Lua_PushNumber(L, dwResult);
	return 1;
}

//

int LuaTabFile_Load(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);

	if( g_LoadFile.ReadTabFile(pszFileName) )
	{	
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaTabFile_Clear(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);

	if( g_LoadFile.ClearTabFile(pszFileName) )
	{	
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaTabFile_GetColumCount(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);
	
	int nResult = g_LoadFile.GetTabFileColum(pszFileName);
	if(nResult > 0)
	{	
		Lua_PushNumber(L, nResult);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaTabFile_GetRowCount(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);
	
	int nResult = g_LoadFile.GetTabFileRow(pszFileName);
	if(nResult > 0)
	{	
		Lua_PushNumber(L, nResult);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;

}

//

int LuaTabFile_GetCell(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 3) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);
	int nRow = (int)Lua_ValueToNumber(L, 2);
	int nColum = (int)Lua_ValueToNumber(L, 3);
	
	if(g_LoadFile.GetTabFileColum(pszFileName) > 0)
	{	
		char szResult[256];
		g_LoadFile.GetTabFileCell(pszFileName, nRow, nColum, szResult);
		
		if(!szResult[0])
			Lua_PushNil(L);
		else
			Lua_PushString(L, szResult);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaIniFile_Load(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}

	const char* pszFileName = (char*)Lua_ValueToString(L, 1);
	if( g_LoadFile.ReadIniFile(pszFileName) )
	{	
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaIniFile_Clear(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);

	if( g_LoadFile.ClearIniFile(pszFileName) )
	{	
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}

//

int LuaIniFile_GetData(Lua_State * L)
{	
	if (Lua_GetTopIndex(L) < 3) 
	{
		Lua_PushNil(L);
		return 1;
	}
	
	const char* pszFileName = (char*)Lua_ValueToString(L, 1);
	const char* pszSec = (char*)Lua_ValueToString(L, 2);
	const char* pszKey = (char*)Lua_ValueToString(L, 3);

	char szResult[80];
	if( g_LoadFile.GetIniFileData(pszFileName, pszSec, pszKey, szResult) )
	{
		if(!szResult[0])
			Lua_PushNil(L);
		else
			Lua_PushString(L, szResult);
	}
	else
	{
		Lua_PushNil(L);
	}
	return 1;
}


//

int LuaIniFile_SaveData(Lua_State * L)
{
	return 0;
}


//

int LuaAskClientForNumber(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
	char*  pFunc = (char*)Lua_ValueToString(L, 1);
	if(pFunc[0])
	{
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pFunc, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
	}

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_ASKFORNUMBER;
	UiInfo.m_bParam1 = (int)Lua_ValueToNumber(L, 2); 
	UiInfo.m_bParam2 = 1;
	UiInfo.m_nParam	= (int)Lua_ValueToNumber(L, 3);
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	char* strTitle  = (char*)Lua_ValueToString(L, 4);
	if (strTitle)
		sprintf(UiInfo.m_pContent, "%s", strTitle);

	UiInfo.m_nBufferLen  = strlen(UiInfo.m_pContent);
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//

int LuaAskClientForString(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 5)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	
	Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
	char*  pFunc = (char*)Lua_ValueToString(L, 1);
	if(pFunc[0])
	{
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pFunc, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
	}
	
	char* pContent	= (char*)Lua_ValueToString(L, 2);

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_ASKFORSTRING;
	UiInfo.m_bParam1 = (BYTE)Lua_ValueToNumber(L, 3);
	UiInfo.m_bParam2 = 1;
	UiInfo.m_nParam	= (int)Lua_ValueToNumber(L, 4);
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	char* strTitle  = (char*)Lua_ValueToString(L, 5);
	if (strTitle)
		sprintf(UiInfo.m_pContent, "%s|%s", strTitle, pContent);

	UiInfo.m_nBufferLen  = strlen(UiInfo.m_pContent);
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//

int LuaGetTongID(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	
	Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.GetTongNameID());
	return 1;
}

//

int LuaGetTongName(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushString(L, "");
		return 1;
	}
	char szResult[16];
	ZeroMemory(szResult, sizeof(szResult));
	Player[nPlayerIndex].m_cTong.GetTongName(szResult);

	Lua_PushString(L, szResult);
	return 1;
}

//

int LuaGetTongMaster(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushString(L, "");
		return 1;
	}
	Lua_PushString(L, Player[nPlayerIndex].m_cTong.m_szMasterName);
	return 1;
}

//

int LuaGetTongFigure(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
    if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER)
	{
		Lua_PushNumber(L, -1);
		return 1;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.GetFigure());
	return 1;
}


//

int LuaGetMagicAttribRecord(Lua_State * L)
{	
	if( Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	const int nPlayerIndex = GetPlayerIndex(L);
    if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	const int nMagic = (int)Lua_ValueToNumber(L,1);
	const int nSeries = (int)Lua_ValueToNumber(L,2);
	Lua_PushNumber(L, ItemGen.GMA_Record(nMagic, nSeries) );
	return 1;
}

//

int LuaCheckMagicAttrib(Lua_State * L)
{	
	if( Lua_GetTopIndex(L) < 4)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	const int nPlayerIndex = GetPlayerIndex(L);
    if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	const int nRecord = (int)Lua_ValueToNumber(L,1) - 1;
	const int nPos = (int)Lua_ValueToNumber(L,2) - 1;
	const int nDetailType = (int)Lua_ValueToNumber(L,3);
	const int nSeries = (int)Lua_ValueToNumber(L,4);
	Lua_PushNumber(L, ItemGen.CheckMA(nRecord, nPos, nDetailType, nSeries) );
	return 1;
}

//

int LuaGetMAByRecord(Lua_State * L)
{	
	if( Lua_GetTopIndex(L) < 1)
		return 0;
	const int nPlayerIndex = GetPlayerIndex(L);
    if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	const int nRecord = (int)Lua_ValueToNumber(L,1) - 1;
	const KMAGICATTRIB_TABFILE* pMA = ItemGen.GMA_ByRecord(nRecord);
	

	Lua_PushNumber(L, pMA->m_MagicAttrib.aryRange[0].nMin);
	Lua_PushNumber(L, pMA->m_MagicAttrib.aryRange[0].nMax);
	return 2;
}


int LuaSavePlayerLog(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
	{
		return 1;
	}
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		return 1;
	}

	const char* pData = (char*)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].SaveLog(3, pData, "", "");
	return 1;
}

int LuaPlayerOpenShop(Lua_State * L)
{	
	int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
	{
		Lua_PushNumber(L, -1);
		return 1;
	}
	
	if(Npc[Player[nPlayerIndex].m_nIndex].m_bRideHorse)
	{
		Lua_PushNumber(L, 1);
		return 1;
	}

	if( Player[nPlayerIndex].m_cShop.GetState() )
	{
		Lua_PushNumber(L, 2);
		return 1;
	}
	
	const char* pszAdv = Player[nPlayerIndex].m_cShop.GetAdv();
	if( !pszAdv[0] )
	{
		Lua_PushNumber(L, 3);
		return 1;
	}

	switch (Player[nPlayerIndex].m_cMenuState.m_nState)
	{
		case PLAYER_MENU_STATE_TRADEOPEN:
			Player[nPlayerIndex].m_cMenuState.SetState(nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
			break;
		default:
			break;
	}

	if (Player[nPlayerIndex].m_cTeam.m_nFlag && Player[nPlayerIndex].m_cTeam.m_nID >= 0)
	{
		PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
		sLeaveTeam.ProtocolType = c2s_teamapplyleave;
		Player[nPlayerIndex].LeaveTeam((BYTE*)&sLeaveTeam);
	}

	Player[nPlayerIndex].m_cShop.SetState(TRUE);
	Npc[Player[nPlayerIndex].m_nIndex].SendCommand(do_sit);
	
	Lua_PushNumber(L, 4);
	return 1;	
}


//

int LuaCallClientScript(Lua_State * L)
{	
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	int nPlayerIndex = GetPlayerIndex(L);
    if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	PLAYER_SCRIPTACTION_SYNC  ScriptAction;
	ScriptAction.ProtocolType		= s2c_scriptaction;
	ScriptAction.m_nOperateType		= SCRIPTACTION_EXESCRIPT;
	
	const char* pszScript = (char*)Lua_ValueToString(L,1);
	const char* pszParam = (char*)Lua_ValueToString(L,2);
	if(pszParam == NULL || pszParam[0] == 0) 
	{
		ScriptAction.m_nBufferLen = strlen(pszScript) + 1 ;
		strcpy(ScriptAction.m_pContent, pszScript);
	}
	else
	{
		ScriptAction.m_nBufferLen = strlen(pszScript) + 2 + strlen(pszParam);
		sprintf(ScriptAction.m_pContent, "%s|%s", pszScript, pszParam);
	}
	ScriptAction.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen - 1;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&ScriptAction, sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen);	

	return 0;
}

//

int LuaBanAccount(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	MSSQL_SetBandPC((char*)Lua_ValueToString(L, 1));
	
	return 0;
}

int LuaUnBanAccount(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	MSSQL_UnBandPC((char*)Lua_ValueToString(L, 1));
	
	return 0;
}

// --
//
// --
int LuaDropObj(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 5 ) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	
	int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
	if(nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;

	int nDataID = (int)Lua_ValueToNumber(L, 2);
	char* lpScript = (char*)Lua_ValueToString(L, 3);
	int nRandRate = (int)Lua_ValueToNumber(L, 4); 
	int nMaxRandRate = (int)Lua_ValueToNumber(L, 5);
	
	Npc[nNpcIdx].DropObj(nPlayerIndex, nDataID, lpScript, nRandRate, nMaxRandRate);
   	return 1;
}
// --
//
// --
int LuaDelObj(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0 ) return 0 ;
	int nObjIndex = (int)Lua_ValueToNumber(L, 1);
	if (nObjIndex > 0)
	{	
		Object[nObjIndex].SyncRemove(TRUE); 
		if (Object[nObjIndex].m_nRegionIdx >= 0)
		{	
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].DecRef(Object[nObjIndex].m_nMapX, Object[nObjIndex].m_nMapY, obj_object);
		}
		ObjSet.Remove(nObjIndex);
	}
	return 0;
}

//--
//AutoAI by kinnox;
// --
int LuaSetObjPickExecute(Lua_State * L)//executeitem by kinnox;
{
	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	int nId = (int)Lua_ValueToNumber(L,1);

	if (nId > 0)
	{
		Object[nId].SetObjPickExecute((BOOL)Lua_ValueToNumber(L,2));
	}
    return 0;
}
//
//
//
int LuaCalcFreeItemCellCount(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	int nWidth = 1;
	int nHeight = 1;
	int nRoom = room_equipment;
	if (Lua_GetTopIndex(L) > 2)
	{
		nWidth = (int)Lua_ValueToNumber(L, 1);
		nHeight = (int)Lua_ValueToNumber(L, 2);
	}
	if (Lua_GetTopIndex(L) > 3)
	{
		nRoom = (int)Lua_ValueToNumber(L, 3);
	}
	if (nRoom < room_equipment || nRoom > room_num)
		return 0;
	int nCount = Player[nPlayerIndex].m_ItemList.CalcFreeItemCellCount(nWidth, nHeight, nRoom);
	Lua_PushNumber(L, nCount);
	return 1;
}

// --
// 
// --

int LuaRANDOM(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 1)
		return 0;
	int nResult = 0;
	if (nParamNum > 1)
		nResult = GetRandomNumber((int)Lua_ValueToNumber(L, 1), (int)Lua_ValueToNumber(L, 2));
	else
		nResult = GetRandomNumber(0, (int)Lua_ValueToNumber(L, 1));
	Lua_PushNumber(L, nResult);
	return 1;
}

int LuaRANDOMC(Lua_State * L)
{
	int nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 2)
		return 0;
	
	srand( (unsigned)time( NULL ) );
	if (Lua_IsTable(L,1))
	{
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L,2));
		Lua_RawGet(L, 1);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L)));
	}
	else if (Lua_IsTable(L,2))
	{
		int nResult = ::GetRandomNumber(1, (int)Lua_ValueToNumber(L,1));
		Lua_PushNumber(L, nResult);
		Lua_RawGet(L, 2);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L))));
	}
	else
	{
		int nResult = ::GetRandomNumber(1, nParamNum);
		Lua_PushNumber(L, (int)Lua_ValueToNumber(L, nResult));
	}
	return 1;
}

int LuaGetTimeNow(Lua_State * L)
{
	SYSTEMTIME		tm;
	GetLocalTime(&tm);
	DWORD nYear = 0;
	DWORD nMonth = 0;
	DWORD nDay = 0;
	DWORD nHour = 0;
	DWORD nMinute = 0;
	DWORD nSecond = 0;
	DWORD nDayOfWeek = 0;
	
	nYear = (tm.wYear % 100) + 2000;
	nMonth = tm.wMonth;
	nDay = tm.wDay;
	nHour = tm.wHour;
	nMinute = tm.wMinute;
	nSecond = tm.wSecond;
	nDayOfWeek = tm.wDayOfWeek;
	
	Lua_PushNumber(L, nYear);
	Lua_PushNumber(L, nMonth);
	Lua_PushNumber(L, nDay);
	Lua_PushNumber(L, nHour);
	Lua_PushNumber(L, nMinute);	
	Lua_PushNumber(L, nSecond);	
	Lua_PushNumber(L, nDayOfWeek);		
	return 7;
}
// --
////load 3 storebox by kinnox;
// --
int LuaSetStoreBox(Lua_State * L)
{
	const int nPlayerIndex = GetPlayerIndex(L);
	BYTE nNum = (BYTE)Lua_ValueToNumber(L, 1);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) ) 
		return 0;	
	if (nPlayerIndex > 0 )
	 	Player[nPlayerIndex].SetStoreBoxNum(nNum);
	return 0;
}

int LuaGetStoreBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Lua_PushNumber(L, Player[nPlayerIndex].m_btRepositoryCount);
	return 1;
}
// --
////Expandbox by kinnox;
// --
int LuaSetExpandBox(Lua_State * L)
{
	const int nPlayerIndex = GetPlayerIndex(L);
	BYTE nNum = (BYTE)Lua_ValueToNumber(L, 1);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) ) 
		return 0;	
	if (nPlayerIndex > 0 )
	 	Player[nPlayerIndex].SetExpandBoxNum(nNum);
	return 0;
}

int LuaGetExpandBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Lua_PushNumber(L, Player[nPlayerIndex].m_btExpandtoryCount);
	return 1;
}
//----------------------------------------------------------
// Ham lua mo giao dien tra vat pham nhiem vu
//----------------------------------------------------------
int LuaOpenGiveBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);

	const char * szTitle;
	const char * szInitString;
	const char * szScript;
	const char * szAction1;


	if (nParamNum < 4)
		return 0;

//	//g_DebugLog("nParamNum %d", nParamNum);
	if (nParamNum >= 5)
	{
		szTitle	= Lua_ValueToString(L, 1);

		szInitString = Lua_ValueToString(L, 2);

		szAction1 = Lua_ValueToString(L, 4);

		char * szScript = (char *)Lua_ValueToString(L, 3);
		Player[nPlayerIndex].m_dwGiveBoxId = g_FileName2Id(szScript);
	}
	else
	{
		szTitle	= Lua_ValueToString(L, 1);

		szInitString = Lua_ValueToString(L, 2);

		szAction1 = Lua_ValueToString(L, 3);

		Player[nPlayerIndex].m_dwGiveBoxId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	}

	S2C_GIVE_BOX NetCommand;
	NetCommand.ProtocolType = s2c_opengivebox;
	NetCommand.nType = 1;
	strcpy(NetCommand.Value, szTitle);
	strcpy(NetCommand.Value1, szInitString);
	strcpy(NetCommand.Value2, szAction1);
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(S2C_GIVE_BOX));
	return 0;
}
// --
//
// --
int LuaEndGiveBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	
	S2C_GIVE_BOX NetCommand;
	NetCommand.ProtocolType = s2c_opengivebox;
	NetCommand.nType = 2;
	strcpy(NetCommand.Value, "");
	strcpy(NetCommand.Value1,"");
	strcpy(NetCommand.Value2,"");
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(S2C_GIVE_BOX));
	return 0;
}
// --
//
// --
int LuaGetROItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	BYTE nX, nY, nRoom;
	int nIndex = 0;
	nRoom = (BYTE)Lua_ValueToNumber(L,1);
	if (nRoom != room_givebox) //ID: Givebox = 8 // Check KProtocolDef.h
	{
		Lua_PushNumber(L,0);
		return 0;
	}	
	nX = (BYTE)Lua_ValueToNumber(L,2);
	nY = (BYTE)Lua_ValueToNumber(L,3);
	nIndex = Player[nPlayerIndex].m_ItemList.m_Room[nRoom].FindItem(nX, nY);
	if (nPlayerIndex > 0)
		Lua_PushNumber(L,nIndex);
	return 1; 
}
// --
// ReWardBox
// --
int LuaOpenReWardBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);

	const char * szTitle;
	const char * szAction1;
	const char * szAction2;
	const char * szAction3;

	if (nParamNum > 5)
		return 0;

	szTitle	= Lua_ValueToString(L, 1);

	szAction1	= Lua_ValueToString(L, 2);

	szAction2	= Lua_ValueToString(L, 3);

	szAction3	= Lua_ValueToString(L, 4);

	Player[nPlayerIndex].m_dwRewardId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;

	S2C_REWARD_BOX NetCommand;
	NetCommand.ProtocolType = s2c_openrewardbox;
	strcpy(NetCommand.Value, szTitle);
	strcpy(NetCommand.Value1, szAction1);
	strcpy(NetCommand.Value2, szAction2);
	strcpy(NetCommand.Value3, szAction3);
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_REWARD_BOX));
	
	return 0;
}
// --
// TimeBox
// --
int LuaOpenTimeBox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);
	int nTime = 0;	
	int nIndexNpc = 0;
	const char * szAction;
	const char * Title;

	if (nParamNum < 1)
		return 0;

	Title = Lua_ValueToString(L, 1);
	nTime = (int)Lua_ValueToNumber(L, 2);
	szAction = Lua_ValueToString(L, 3);
	nIndexNpc = (int)Lua_ValueToNumber(L, 4);
	Player[nPlayerIndex].m_dwTimeBoxId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;

	S2C_TIME_BOX NetCommand;
	NetCommand.ProtocolType = s2c_timebox;
	strcpy(NetCommand.Value, Title);
	NetCommand.Value1 = nTime;
	NetCommand.nIndexNpc = nIndexNpc;
	strcpy(NetCommand.Value2, szAction);
	//g_DebugLog(" %s - %d - %d - %s",NetCommand.Value,NetCommand.Value1,NetCommand.nIndexNpc,NetCommand.Value2);
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx,&NetCommand,sizeof(S2C_TIME_BOX));
	
	return 0;
}
// --
//Lixian by kinnox;
// --
int LuaSetStatusLixian(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex > MAX_PLAYER) return 0;
	if (Npc[Player[nPlayerIndex].m_nIndex].m_RegionIndex < 0) return 0;
	if (Lua_GetTopIndex(L) < 1) return 0;

	if (Player[nPlayerIndex].m_cShop.GetState())
	{	
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing = do_sit;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].ProcStatus();
	}

	int nStatus = 0;
	nStatus = (BYTE)Lua_ValueToNumber(L,1);
	Player[nPlayerIndex].m_nLixianStatus = nStatus;


	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_LIXIAN;
	sMsg.m_lpBuf = 0;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

	Lua_PushNumber(L , nStatus);
	return 1;
}

int LuaSetDisconectPlayer(Lua_State * L)
{
	/*int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex > MAX_PLAYER) return 0;
	if (Npc[Player[nPlayerIndex].m_nIndex].m_RegionIndex < 0) return 0;
	if (Lua_GetTopIndex(L) < 1) return 0;
	PlayerSet.PrepareRemove(nPlayerIndex);
	Lua_PushNumber(L , 0);*/
	return 1;
}


int LuaUpdateTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;
	try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\mission\\battles\\updatetop.lua");;
		if (pScript)
		{

			

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("UpdateTop",0, "d",nPlayerIndex));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\mission\\battles\\updatetop.lua"));
	}
	return 0;
}

int LuaDeleteTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 0)
     return 0;
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\mission\\battles\\updatetop.lua");;
		if (pScript)
		{
			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("DeleteTop",0, "d",nPlayerIndex));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\mission\\battles\\updatetop.lua"));
	}   
	return 0;
}

int LuaSendTopTKNew(Lua_State * L)
{
	int nNumberPrama = Lua_GetTopIndex(L);

	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex >= MAX_PLAYER) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0 || Player[nPlayerIndex].m_nIndex >= MAX_NPC) return 0;

	if (nNumberPrama < 94)
     return 0;
   	PLAYER_SEND_TOP_TK_NEW     IDC;
	IDC.ProtocolType = s2c_playersendtoptknew;
	IDC.m_DataTop.m_SoluongSong = (BYTE)Lua_ValueToNumber(L, 1);
	IDC.m_DataTop.m_ThoiGian = (BYTE)Lua_ValueToNumber(L, 2);
	IDC.m_DataTop.m_NguoiChoi = (WORD)Lua_ValueToNumber(L, 3);
	IDC.m_DataTop.m_TNguoiChoi = (WORD)Lua_ValueToNumber(L, 4);
	IDC.m_DataTop.m_NPC = (WORD)Lua_ValueToNumber(L, 5);
	IDC.m_DataTop.m_TNPC = (WORD)Lua_ValueToNumber(L, 6);
	IDC.m_DataTop.m_LienTramHT = (WORD)Lua_ValueToNumber(L, 7);
	IDC.m_DataTop.m_TLienTramHT = (WORD)Lua_ValueToNumber(L, 8);
	IDC.m_DataTop.m_BauVat = (WORD)Lua_ValueToNumber(L, 9);
	IDC.m_DataTop.m_TBauVat = (WORD)Lua_ValueToNumber(L, 10);
	IDC.m_DataTop.m_TichLuy = (WORD)Lua_ValueToNumber(L, 11);
	IDC.m_DataTop.m_TuVong = (WORD)Lua_ValueToNumber(L, 12);
	IDC.m_DataTop.m_LienTram = (WORD)Lua_ValueToNumber(L, 13);


	g_StrCpyLen(IDC.m_DataTop.m_szTop1Name, Lua_ValueToString(L,14), sizeof(IDC.m_DataTop.m_szTop1Name));
	IDC.m_DataTop.m_Top1Phe = (BYTE)Lua_ValueToNumber(L, 15);
	IDC.m_DataTop.m_Top1TichLuy = (WORD)Lua_ValueToNumber(L, 16);
	IDC.m_DataTop.m_Top1NguoiChoi = (WORD)Lua_ValueToNumber(L, 17);
	IDC.m_DataTop.m_Top1NPC = (WORD)Lua_ValueToNumber(L, 18);
	IDC.m_DataTop.m_Top1TuVong = (WORD)Lua_ValueToNumber(L, 19);
	IDC.m_DataTop.m_Top1LienTram = (WORD)Lua_ValueToNumber(L, 20);
	IDC.m_DataTop.m_Top1BaoVat = (WORD)Lua_ValueToNumber(L, 21);

	g_StrCpyLen(IDC.m_DataTop.m_szTop2Name, Lua_ValueToString(L,22), sizeof(IDC.m_DataTop.m_szTop2Name));
	IDC.m_DataTop.m_Top2Phe = (BYTE)Lua_ValueToNumber(L, 23);
	IDC.m_DataTop.m_Top2TichLuy = (WORD)Lua_ValueToNumber(L, 24);
	IDC.m_DataTop.m_Top2NguoiChoi = (WORD)Lua_ValueToNumber(L, 25);
	IDC.m_DataTop.m_Top2NPC = (WORD)Lua_ValueToNumber(L, 26);
	IDC.m_DataTop.m_Top2TuVong = (WORD)Lua_ValueToNumber(L, 27);
	IDC.m_DataTop.m_Top2LienTram = (WORD)Lua_ValueToNumber(L, 28);
	IDC.m_DataTop.m_Top2BaoVat = (WORD)Lua_ValueToNumber(L, 29);

	g_StrCpyLen(IDC.m_DataTop.m_szTop3Name, Lua_ValueToString(L,30), sizeof(IDC.m_DataTop.m_szTop3Name));
	IDC.m_DataTop.m_Top3Phe = (BYTE)Lua_ValueToNumber(L, 31);
	IDC.m_DataTop.m_Top3TichLuy = (WORD)Lua_ValueToNumber(L, 32);
	IDC.m_DataTop.m_Top3NguoiChoi = (WORD)Lua_ValueToNumber(L, 33);
	IDC.m_DataTop.m_Top3NPC = (WORD)Lua_ValueToNumber(L, 34);
	IDC.m_DataTop.m_Top3TuVong = (WORD)Lua_ValueToNumber(L, 35);
	IDC.m_DataTop.m_Top3LienTram = (WORD)Lua_ValueToNumber(L, 36);
	IDC.m_DataTop.m_Top3BaoVat = (WORD)Lua_ValueToNumber(L, 37);

	g_StrCpyLen(IDC.m_DataTop.m_szTop4Name, Lua_ValueToString(L,38), sizeof(IDC.m_DataTop.m_szTop4Name));
	IDC.m_DataTop.m_Top4Phe = (BYTE)Lua_ValueToNumber(L, 39);
	IDC.m_DataTop.m_Top4TichLuy = (WORD)Lua_ValueToNumber(L, 40);
	IDC.m_DataTop.m_Top4NguoiChoi = (WORD)Lua_ValueToNumber(L, 41);
	IDC.m_DataTop.m_Top4NPC = (WORD)Lua_ValueToNumber(L, 42);
	IDC.m_DataTop.m_Top4TuVong = (WORD)Lua_ValueToNumber(L, 43);
	IDC.m_DataTop.m_Top4LienTram = (WORD)Lua_ValueToNumber(L, 44);
	IDC.m_DataTop.m_Top4BaoVat = (WORD)Lua_ValueToNumber(L, 45);

	g_StrCpyLen(IDC.m_DataTop.m_szTop5Name, Lua_ValueToString(L,46), sizeof(IDC.m_DataTop.m_szTop5Name));
	IDC.m_DataTop.m_Top5Phe = (BYTE)Lua_ValueToNumber(L, 47);
	IDC.m_DataTop.m_Top5TichLuy = (WORD)Lua_ValueToNumber(L, 48);
	IDC.m_DataTop.m_Top5NguoiChoi = (WORD)Lua_ValueToNumber(L, 49);
	IDC.m_DataTop.m_Top5NPC = (WORD)Lua_ValueToNumber(L, 50);
	IDC.m_DataTop.m_Top5TuVong = (WORD)Lua_ValueToNumber(L, 51);
	IDC.m_DataTop.m_Top5LienTram = (WORD)Lua_ValueToNumber(L, 52);
	IDC.m_DataTop.m_Top5BaoVat = (WORD)Lua_ValueToNumber(L, 53);

	g_StrCpyLen(IDC.m_DataTop.m_szTop6Name, Lua_ValueToString(L,54), sizeof(IDC.m_DataTop.m_szTop6Name));
	IDC.m_DataTop.m_Top6Phe = (BYTE)Lua_ValueToNumber(L, 55);
	IDC.m_DataTop.m_Top6TichLuy = (WORD)Lua_ValueToNumber(L, 56);
	IDC.m_DataTop.m_Top6NguoiChoi = (WORD)Lua_ValueToNumber(L, 57);
	IDC.m_DataTop.m_Top6NPC = (WORD)Lua_ValueToNumber(L, 58);
	IDC.m_DataTop.m_Top6TuVong = (WORD)Lua_ValueToNumber(L, 59);
	IDC.m_DataTop.m_Top6LienTram = (WORD)Lua_ValueToNumber(L, 60);
	IDC.m_DataTop.m_Top6BaoVat = (WORD)Lua_ValueToNumber(L, 61);

	g_StrCpyLen(IDC.m_DataTop.m_szTop7Name, Lua_ValueToString(L,62), sizeof(IDC.m_DataTop.m_szTop7Name));
	IDC.m_DataTop.m_Top7Phe = (BYTE)Lua_ValueToNumber(L, 63);
	IDC.m_DataTop.m_Top7TichLuy = (WORD)Lua_ValueToNumber(L, 64);
	IDC.m_DataTop.m_Top7NguoiChoi = (WORD)Lua_ValueToNumber(L, 65);
	IDC.m_DataTop.m_Top7NPC = (WORD)Lua_ValueToNumber(L, 66);
	IDC.m_DataTop.m_Top7TuVong = (WORD)Lua_ValueToNumber(L, 67);
	IDC.m_DataTop.m_Top7LienTram = (WORD)Lua_ValueToNumber(L, 68);
	IDC.m_DataTop.m_Top7BaoVat = (WORD)Lua_ValueToNumber(L, 69);

	g_StrCpyLen(IDC.m_DataTop.m_szTop8Name, Lua_ValueToString(L,70), sizeof(IDC.m_DataTop.m_szTop8Name));
	IDC.m_DataTop.m_Top8Phe = (BYTE)Lua_ValueToNumber(L, 71);
	IDC.m_DataTop.m_Top8TichLuy = (WORD)Lua_ValueToNumber(L, 72);
	IDC.m_DataTop.m_Top8NguoiChoi = (WORD)Lua_ValueToNumber(L, 73);
	IDC.m_DataTop.m_Top8NPC = (WORD)Lua_ValueToNumber(L, 74);
	IDC.m_DataTop.m_Top8TuVong = (WORD)Lua_ValueToNumber(L, 75);
	IDC.m_DataTop.m_Top8LienTram = (WORD)Lua_ValueToNumber(L, 76);
	IDC.m_DataTop.m_Top8BaoVat = (WORD)Lua_ValueToNumber(L, 77);

	g_StrCpyLen(IDC.m_DataTop.m_szTop9Name, Lua_ValueToString(L,78), sizeof(IDC.m_DataTop.m_szTop9Name));
	IDC.m_DataTop.m_Top9Phe = (BYTE)Lua_ValueToNumber(L, 79);
	IDC.m_DataTop.m_Top9TichLuy = (WORD)Lua_ValueToNumber(L, 80);
	IDC.m_DataTop.m_Top9NguoiChoi = (WORD)Lua_ValueToNumber(L, 81);
	IDC.m_DataTop.m_Top9NPC = (WORD)Lua_ValueToNumber(L, 82);
	IDC.m_DataTop.m_Top9TuVong = (WORD)Lua_ValueToNumber(L, 83);
	IDC.m_DataTop.m_Top9LienTram = (WORD)Lua_ValueToNumber(L, 84);
	IDC.m_DataTop.m_Top9BaoVat = (WORD)Lua_ValueToNumber(L, 85);

	g_StrCpyLen(IDC.m_DataTop.m_szTop10Name, Lua_ValueToString(L,86), sizeof(IDC.m_DataTop.m_szTop10Name));
	IDC.m_DataTop.m_Top10Phe = (BYTE)Lua_ValueToNumber(L, 87);
	IDC.m_DataTop.m_Top10TichLuy = (WORD)Lua_ValueToNumber(L, 88);
	IDC.m_DataTop.m_Top10NguoiChoi = (WORD)Lua_ValueToNumber(L, 89);
	IDC.m_DataTop.m_Top10NPC = (WORD)Lua_ValueToNumber(L, 90);
	IDC.m_DataTop.m_Top10TuVong = (WORD)Lua_ValueToNumber(L, 91);
	IDC.m_DataTop.m_Top10LienTram = (WORD)Lua_ValueToNumber(L, 92);
	IDC.m_DataTop.m_Top10BaoVat = (WORD)Lua_ValueToNumber(L, 93);
	IDC.m_DataTop.m_SoluongJin = (BYTE)Lua_ValueToNumber(L, 94);
    if (g_pServer)
		 g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(PLAYER_SEND_TOP_TK_NEW));

	return 0;
}
// --
//Lixian by kinnox;
// --
int LuaDelpwbox(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0 || nPlayerIndex > MAX_PLAYER) return 0;
	if (Lua_GetTopIndex(L) < 1) return 0;

	Player[nPlayerIndex].LockPlayerInfo(DEL_PW,0,0);		
	return 1;
}
//----------------------------------------------------------
// Ham lua mo giao dien thong bao hoat dong
//----------------------------------------------------------
int LuaOpenNotification(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);
	const char * szTitle;
	const char * szInitString;
	if (nParamNum < 2)
		return 0;
		szTitle	= Lua_ValueToString(L, 1);
		szInitString = Lua_ValueToString(L, 2);
	S2C_NOTIFICATION NetCommand;
	NetCommand.ProtocolType = s2c_notification;
	strcpy(NetCommand.Value, szTitle);
	strcpy(NetCommand.Value1, szInitString);;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(S2C_NOTIFICATION));
	return 0;
}
// --
// TrembleItem by kinnox
// --
//SetPItemID
int LuaSetPItemID(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L,0);
		return 1;
	}
	BOOL bExist = FALSE;
	int nIndex = (int)Lua_ValueToNumber(L, 1);
	
	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	BYTE nX, nY, nPos;
	nPos = (BYTE)Lua_ValueToNumber(L,2);
	if (nPos < pos_equip || nPos > pos_builditem)
	{
		Lua_PushNumber(L,0);
		return 0;
	}	
	nX = (BYTE)Lua_ValueToNumber(L,3);

	switch (nPos)
	{
		case pos_equip:
			bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
			break;
		case pos_builditem:
			bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
			break;
	}

	if (!bExist)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, nPos, nX, 0);
	else
	{
		int		nIdx = Player[nPlayerIndex].m_ItemList.Hand();
		if (nIdx)
		{
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);
			
			KMapPos sMapPos;
			KObjItemInfo	sInfo;
			
			Player[nPlayerIndex].GetAboutPos(&sMapPos);
			
			sInfo.m_nItemID = nIdx;
			sInfo.m_dwItemID = Item[nIdx].GetID();
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code;
			
			int nObj = ObjSet.Add(Item[nIdx].GetObjID(), sMapPos, sInfo);
			if (nObj == -1)
			{
				ItemSet.Remove(nIdx);
			}
			else
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					Object[nObj].SetEntireBelong(nPlayerIndex);
				}
				else
				{
					Object[nObj].SetItemBelong(nPlayerIndex);
				}
			}
		}
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_hand, 0 ,0);	
	}
	Lua_PushNumber(L,1);
	return 1; 
}

int LuaGetPOItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	BYTE nX, nY, nPos;
	int nIndex = 0;
	nPos = (BYTE)Lua_ValueToNumber(L,1);
	if (nPos < pos_equip || nPos > pos_builditem)
	{
		Lua_PushNumber(L,0);
		return 0;
	}	
	nX = (BYTE)Lua_ValueToNumber(L,2);
	//g_DebugLog("Run vao day1 %d",pos_builditem);
	switch (nPos)
	{
	case pos_equip:
		nIndex = Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
		break;
	case pos_builditem: // pos nay bang 15
		{
				//g_DebugLog("Run vao da2 %d",pos_builditem);
			nIndex = Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
		}
		break;
	default:
		break;
	}

	Lua_PushNumber(L,nIndex);
	return 1; 
}

int LuaOpenTrembleItem(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	int nParamNum = Lua_GetTopIndex(L);

	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TREMBLE;
	sMsg.m_lpBuf = 0;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
	if(g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	return 0;
}
// --
// LockSongJin by kinnox
// --
int LuaSetLockSongJin(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	MSSQL_SetSongJin((char*)Lua_ValueToString(L, 1),(int)Lua_ValueToNumber(L, 2));
	
	return 0;
}

int LuaGetLockSongJin(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	const int nPlayerIndex = GetPlayerIndex(L);
	if( (nPlayerIndex <= 0) || (nPlayerIndex >= MAX_PLAYER) )
		return 0;

	BOOL Result;
	Result = MSSQL_GetSongJin((char*)Lua_ValueToString(L, 1));
	Lua_PushNumber(L, Result);
	return 1;
}
// --
// 
// --
//end code;
#endif

int LuaPlayMusic(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1) 
		return 0;
	
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_PLAYMUSIC;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	
	int nMsgId = 0;
	
	g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), sizeof(UiInfo.m_pContent));
	UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
	UiInfo.m_bParam1 = 0;
	
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

int LuaFadeInMusic(Lua_State * L)
{
	return 0;
}

int LuaFadeOutMusic(Lua_State * L)
{
	return 0;
}

#ifndef _SERVER

int LuaPlaySound(Lua_State * L)
{
	
	return 0;
}

int LuaPlaySprMovie(Lua_State * L)
{
	return 0;
}

int LuaExitGame(Lua_State * L)
{
	CoreDataChanged(GDCNI_EXIT_GAME, 0, 0);
	return 0;
}
#endif
TLua_Funcs GameScriptFuns[] = 
{	
	{"Say",					LuaSelectUI},
	{"SayImage",			LuaSelectUI2},
	{"Talk",				LuaTalkUI},
	{"GetTaskTemp",			LuaGetTempTaskValue},
	{"SetTaskTemp",			LuaSetTempTaskValue},
	{"Message",				LuaMessage},
	{"GetBit",				LuaGetBit},
	{"GetByte",				LuaGetByte},
	{"SetBit",				LuaSetBit},
	{"SetByte",				LuaSetByte},
	{"Include",				LuaIncludeFile},
	{"PutMessage",			LuaSendMessageInfo},
	{"AddGlobalNews",		LuaAddGlobalNews},
	{"AddGlobalNewsEx",		LuaAddGlobalNewsEx},
	{"AddGlobalTimeNews",	LuaAddGlobalTimeNews},
	{"AddGlobalCountNews",	LuaAddGlobalCountNews},
	{"AddLocalNews",		LuaAddLocalNews},
	{"AddLocalTimeNews",	LuaAddLocalTimeNews},
	{"AddLocalCountNews",	LuaAddLocalCountNews},
#ifdef _SERVER
	{"AddTransLife",		LuaModifyTransLife},
	{"GetTransLife",		LuaGetTransLife},
	{"AddRepute",			LuaModifyRepute},
	{"GetRepute",			LuaGetRepute},	
	{"AddFuyuan",			LuaModifyFuyuan},
	{"GetFuyuan",			LuaGetFuyuan},
	{"GetNpcIdx",			LuaGetCurNpcIndex},
	{"SetTimer",			LuaSetTimer},
	{"StopTimer",			LuaStopTimer},
	{"GetRestTime",			LuaGetRestTime},
	{"GetTimerId",			LuaGetCurTimerId},
	{"GetTask",				LuaGetTaskValue},
	{"SetTask",				LuaSetTaskValue},
	{"SyncTask",			LuaSyncTaskValue},
	{"IsCaptain",			LuaIsLeader},
	{"GetTeam",				LuaGetTeamId},
	{"GetTeamSize",			LuaGetTeamSize},
	{"LeaveTeam",			LuaLeaveTeam},
	{"GetTeamMember",		LuaGetTeamMember},
	{"GetTeamCaptain",		LuaGetTeamCaptain},
	{"Msg2Player",			LuaMsgToPlayer},
	{"Msg2Team",			LuaMsgToTeam},
	{"Msg2SubWorld",		LuaMsgToSubWorld},
	{"Msg2Region",			LuaMsgToAroundRegion},
	{"Msg2GM",				LuaMsgToGameMaster},
	{"Msg2IP",				LuaMsgToIP},
	{"Msg2Faction",			LuaMsgToFaction},
	{"GetInfo",				LuaGetPlayerInfo},
	{"GetIP",				LuaGetIP},
	{"SetPos",				LuaSetPos},	
	{"GetPos",				LuaGetPos},	
	{"GetWorldPos",			LuaGetNewWorldPos},
	{"NewWorld",			LuaEnterNewWorld},
	{"GetNpcLevel",			LuaGetNpcLevel},
	{"GetNpcName",			LuaGetNpcName},	
	{"GetNpcPos",			LuaGetNpcPos},
	{"GetNpcGold",			LuaGetNpcGold},
	{"SetNpcSeries",		LuaSetNpcSeries},
	{"GetNpcSeries",		LuaGetNpcSeries},
	{"SetNpcOwner",			LuaSetNpcOwner},
	{"SetNpcParam",			LuaSetNpcParam},
	{"GetNpcParam",			LuaGetNpcParam},
	{"SetNpcCurCamp",		LuaSetNpcCurCamp},
	{"SetNpcName",			LuaSetNpcName},
	{"SetNpcReduceDamge",	LuaSetNpcReduceDamge},
	{"SetNpcSkill",			LuaSetNpcSkill},
	{"SetNpcDamage",		LuaSetNpcDamage},
	{"SetNpcLife",			LuaSetNpcLife},
	{"IsBoss",				LuaIsBoss},
	{"DropMoney",			LuaDropMoney},
	{"DropItem",			LuaDropItem},
	{"DropRateItem",		LuaDropRateItem},
	{"FindEmptyPlace",		LuaFindEmptyPlace},
	{"AddItem",				LuaAddItem},
	{"AddItemEx",			LuaAddItemEx},
	{"AddStackItem",		LuaAddStackItem},
	{"GetItemStackCount",	LuaGetItemStackCount},
	{"UseStackItem",		LuaUseStackItem},	
	{"DelStackItem",		LuaDelStackItem},
	{"AddScriptItem",		LuaAddScriptItem},
	{"AddMyItem",			LuaAddMyItem},
	{"IsMyItem",			LuaIsMyItem},
	{"DelMyItem",			LuaDelMyItem},
	{"DelItemByIndex",		LuaDelItemByIndex},
	{"AddTaskItem",			LuaAddEventItem	},
	{"DelTaskItem",			LuaDelEventItem},
	{"HaveEventItem",		LuaHaveEventItem},
	{"GetTaskItemCount",	LuaGetTaskItemCount},
	{"AddMineItem",			LuaAddMineItem	},
	{"CountFreeBagCell",	LuaCountFreeBagCell},
	{"GetItemCountInBag",	LuaGetItemCountInBag},
	{"ConsumeItemInBag",	LuaConsumeItemInBag},
	{"SetItemBindState",	LuaSetItemBindState},
	{"GetItemBindState",	LuaGetItemBindState},
	{"GetItemProp",			LuaGetItemProp},
	{"GetItemRandSeed",		LuaGetItemRandSeed},
	{"GetItemVersion",		LuaGetItemVersion},
	{"SetItemGenre",		LuaSetItemGenre},
	{"SetItemParticular",	LuaSetItemParticular},
	{"SetItemDetail",		LuaSetItemDetail},// them by kinnox;
	{"SetItemLevel",		LuaSetItemLevel},
	{"SetItemSeries",		LuaSetItemSeries},
	{"SetItemMagicLevel",	LuaSetItemMagicLevel},
	{"GetItemMagicLevel",	LuaGetItemMagicLevel},
	{"SetItemLuck",			LuaSetItemLuck},
	{"SetItemRandSeed",		LuaSetItemRandSeed},
	{"RestoreBrokenEquip",	LuaRestoreBrokenEquip},
	{"AddItemAgain",		LuaAddItemAgain},
	{"GetItemExpiredTime",	LuaGetItemExpiredTime},
	{"GetItemInfo",			LuaGetItemInfo},
	{"SetItemRecord",		LuaSetItemRecord},
	{"GetHandItem",			LuaGetHandItem},
	{"GetEquipItem",		LuaGetEquipItem},
	{"AddGoldItem",			LuaAddGoldItem},
	{"GetGoldItem",			LuaGetGoldItem},
	{"GetItemMagicAttrib",	LuaGetItemMagicAttrib},
	{"SetItemParam",		LuaSetItemParam},
	{"GetItemParam",		LuaGetItemParam},
	{"SyncItem",			LuaSyncItem},
	{"AddMagic",			LuaAddMagic},
	{"DelMagic",			LuaDelMagic},
	{"HaveMagic",			LuaHaveMagic},
	{"GetMagicLevel",		LuaGetMagicLevel},
	{"AddMagicPoint",		LuaAddMagicPoint},
	{"GetMagicPoint",		LuaGetMagicPoint},
	{"AddSkillState",		LuaAddSkillState},
	{"CastSkill",			LuaCastSkill},
	{"ClearStateSkill",		LuaClearStateSkill},
	{"SubWorldID2Idx",		LuaSubWorldIDToIndex},
	{"SubWorldIdx2ID",		LuaSubWorldIndexToID}, 
	{"AddLeadExp",			LuaAddLeadExp},
	{"GetLeadLevel",		LuaGetLeadLevel},
	{"SetFightState",		LuaSetFightState}, 
	{"GetFightState",		LuaGetFightState},
	{"AddNpc",				LuaAddNpc},
	{"AddNpcSkillState",	LuaAddNpcSkillState},
	{"DelNpc",				LuaDelNpc},
	//
	{"ClearMapNpc",			LuaClearMapNpc}, //Del npc by kinnox;
	{"ClearMapNpcWithName",	LuaClearMapNpcWithName}, //Del npc by kinnox;
	{"FindAroundNpc",		LuaFindAroundNpc},	// indexsetting	by kinnox;
	{"GetNpcID",			LuaGetNpcID},	// indexsetting	by kinnox;
	//	
	{"SetNpcScript",		LuaSetNpcActionScript},
	{"SetRevPos",			LuaSetPlayerRevivalPos},
	{"SetTempRevPos",		LuaSetDeathRevivalPos},
	{"SetClearPos",			LuaSetClearPos},
	{"GetCurCamp",			LuaGetPlayerCurrentCamp},
	{"GetCamp",				LuaGetPlayerCamp},
	{"SetCurCamp",			LuaSetPlayerCurrentCamp},
	{"SetCamp",				LuaSetPlayerCamp},
	{"RestoreCamp",			LuaRestorePlayerCamp},
	{"GetFaction",			LuaGetPlayerFaction,},
	{"SetFaction",			LuaChangePlayerFaction},
	{"GetFactionNo",		LuaGetPlayerFactionNo},
	{"GetColdR",			LuaGetPlayerColdResist},	
	{"SetColdR",			LuaGetPlayerColdResist},
	{"GetFireR",			LuaGetPlayerFireResist},
	{"SetFireR",			LuaSetPlayerFireResist},
	{"GetLightR",			LuaGetPlayerLightResist},
	{"SetLightR",			LuaSetPlayerLightResist},
	{"GetPoisonR",			LuaGetPlayerPoisonResist},
	{"SetPoisonR",			LuaSetPlayerPoisonResist},
	{"GetPhyR",				LuaGetPlayerPhysicsResist},
	{"SetPhyR",				LuaSetPlayerPhysicsResist},
	{"GetExp",				LuaGetPlayerExp	},
	{"GetExpPercent",		LuaGetPlayerExpPercent},
	{"AddExp",				LuaModifyPlayerExp},
	{"AddOwnExp",			LuaAddOwnExp},
	{"AddSumExp",			LuaAddSumExp}, //Kinh nghiem cong don by kinnox;	
	{"SetLevel",			LuaSetLevel},
	{"GetLife",				LuaGetPlayerLife},
	{"RestoreLife",			LuaRestorePlayerLife},
	{"GetMana",				LuaGetPlayerMana},
	{"RestoreMana",			LuaRestorePlayerMana},
	{"GetStamina",			LuaGetPlayerStamina},
	{"RestoreStamina",		LuaRestorePlayerStamina},
	{"GetDefend",			LuaGetPlayerDefend},
	{"GetSex",				LuaGetPlayerSex},
	{"GetSeries",			LuaGetPlayerSeries},
	{"SetSeries",			LuaSetPlayerSeries},
	{"GetAccount",			LuaGetPlayerAccount},
	{"GetPlayer",			LuaGetPlayer},
	{"GetLastDiagNpc",		LuaGetLastDiagNpc},
	{"GetName",				LuaGetPlayerName},
	{"GetDamage",			LuaGetPlayerDamage},
	{"ClearDamage",			LuaClearPlayerDamage},
	{"GetDamageCounter",	LuaGetPlayerDamageCounter},
	{"ClearDamageCounter",	LuaClearPlayerDamageCounter},
	{"SetName",				LuaSetPlayerName},
	{"GetUUID",				LuaGetPlayerID},
	{"GetLeadExp",			LuaGetPlayerLeadExp},
	{"GetLeadLevel",		LuaGetPlayerLeadLevel},
	{"GetLevel",			LuaGetLevel},
	{"GetRestAP",			LuaGetPlayerRestAttributePoint},
	{"GetRestSP",			LuaGetPlayerRestSkillPoint},
	{"GetLucky",			LuaGetPlayerLucky},
	{"GetEng",				LuaGetPlayerEngergy},
	{"GetDex",				LuaGetPlayerDexterity},
	{"GetStrg",				LuaGetPlayerStrength},
	{"GetVit",				LuaGetPlayerVitality},
	{"GetCash",				LuaGetPlayerCashMoney},
	{"Pay",					LuaPlayerPayMoney},
	{"Earn",				LuaPlayerEarnMoney},
	{"PrePay",				LuaPlayerPrePayMoney},
	{"ExeScript",			LuaPlayerExecuteScript},
	{"AttackNpc",			LuaAttackNpc},
	{"KillNpc",				LuaKillNpc},
	{"KillPlayer",			LuaKillPlayer},
	{"Sale",				LuaSale},
	{"UseTownPortal",		LuaUseTownPortal},
	{"ReturnFromPortal",	LuaReturnFromTownPortal},
	{"OpenBox",				LuaOpenBox},
	{"AddStation",			LuaAddPlayerStation},
	{"AddTermini",			LuaAddPlayerWayPoint},
	{"GetStation",			LuaGetPlayerStation	},
	{"GetStationCount",		LuaGetPlayerStationCount},
	{"GetCityCount",		LuaGetAllStationCount},
	{"GetCity",				LuaGetCity},
	{"GetWayPoint",			LuaGetPlayerWayPoint},
	{"GetStationName",		LuaGetStationName},
	{"GetWayPointName",		LuaGetWayPointName},
	{"GetPrice2Station",	LuaGetPriceToStation},
	{"GetPrice2WayPoint",	LuaGetPriceToWayPoint},
	{"GetStationPos",		LuaGetStationPos},
	{"GetWayPointPos",		LuaGetWayPointPos},
	{"GetPlayerCount",		LuaGetPlayerCount}, 
	{"GetItemCount",		LuaGetItemCount},
	{"GetObjCount",			LuaGetObjCount},
	{"GetNpcCount",			LuaGetNpcCount},
	{"GetRank",				LuaGetRank},
	{"SetRank",				LuaSetRank},
	{"GetTitle",			LuaGetTitle},
	{"SetTitle",			LuaSetTitle},
	{"SetPropState",		LuaSetObjPropState},
	{"GetServerName",		LuaGetServerName},
	{"GetWharfName",		LuaGetDockName},
	{"GetWharfCount",		LuaGetDockCount},
	{"GetWharfPrice",		LuaGetDockPrice},
	{"GetWharf",			LuaGetDock},
	{"GetWharfPos",			LuaGetDockPos},
	{"GetTerminiFState",	LuaGetWayPointFightState},
	{"KickOutPlayer",		LuaKickOutPlayer},
	{"KickOutSelf",			LuaKickOutSelf},
	{"GetSkillId",			LuaGetSkillIdInSkillList},
	{"SetSkillLevel",		LuaSetSkillLevel},
	{"SetChatFlag",			LuaSetPlayerChatForbiddenFlag},
	{"AddNote",				LuaAddNote},
	{"GetMissionV",			LuaGetMissionValue},
	{"SetMissionV",			LuaSetMissionValue},
	{"GetMissionS",			LuaGetMissionString},
	{"SetMissionS",			LuaSetMissionString},
	{"GetGlbMissionV",		LuaGetGlobalMissionValue},
	{"SetGlbMissionV",		LuaSetGlobalMissionValue},
	{"OpenMission",			LuaInitMission},
	{"RunMission",			LuaRunMission},
	{"CloseMission",		LuaCloseMission},
	{"JoinMission",			LuaJoinMission},
	{"StartMissionTimer",	LuaStartMissionTimer},
	{"StopMissionTimer",	LuaStopMissionTimer},
	{"GetMSRestTime",		LuaGetMissionRestTime},
	{"GetMSIdxGroup",		LuaGetPlayerMissionGroup},
	
	{"AddMSPlayer",			LuaAddMissionPlayer},
	{"DelMSPlayer",			LuaRemoveMissionPlayer},
	{"GetNextPlayer",		LuaGetNextPlayer},
	{"PIdx2MSDIdx",			LuaGetMissionPlayer_DataIndex},
	{"MSDIdx2PIdx",			LuaGetMissionPlayer_PlayerIndex},
	{"NpcIdx2PIdx",			LuaNpcIndexToPlayerIndex},
	{"GetMSPlayerCount",	LuaMissionPlayerCount},
	
	{"SetPMParam",			LuaSetMissionPlayerParam },
	{"GetPMParam",			LuaGetMissionPlayerParam},
	{"Msg2MSGroup",			LuaMissionMsg2Group},
	{"Msg2MSAll",			LuaMissionMsg2All},
	{"Msg2MSPlayer",		LuaMissionMsg2Player},
	{"SetDeathScript",		LuaSetPlayerDeathScript},
	{"SetLogoutScript",		LuaSetPlayerLogoutScript},
	//-
	{"AddMapTrap",			LuaAddMapTrap},
	{"AddMapTrapByCell",	LuaAddMapTrapByCell},
	{"AddMapObject",		LuaAddMapObject },
	{"CountNpcInMap",		LuaCountNpcInMap},
	{"GetMapInfoFile",		LuaGetMapInfoFile},
	{"GetMapName",			LuaGetMapName},
	{"ClearMapObj",			LuaClearMapObj},
	{"ClearMapTrap",		LuaClearMapTrap},
	{"SetLogoutRV",			LuaSetPlayerRevivalOptionWhenLogout},
	{"SetCreateTeam",		LuaSetCreateTeamOption},
	{"GetPK",				LuaGetPlayerPKValue},
	{"SetPK",				LuaSetPlayerPKValue},
	{"ForbidChangePK",		LuaForbidChangePK},
	{"SetPKFlag",			LuaSetPlayerPKFlag},
	{"GetPKFlag",			LuaGetPlayerPKFlag},
	{"ForbitTrade",			LuaForbitTrade},
	//
	{"ShowLadder",			LuaShowLadder}, 
	//
	{"OpenTong",			LuaOpenTong},
	{"SetPunish",			LuaSetDeathPunish},
	//-
	{"SwearBrother",		LuaSwearBrother},
	{"MakeEnemy",			LuaMakeEnemy},
	{"RollbackSkill",		LuaRollBackSkills},
	{"UpdateSkill",			LuaUpdateSkillList},
	//-

	{"AddProp",				LuaAddPropPoint},
	{"GetProp",				LuaGetRestPropPoint },
	{"ResetBaseAttribute",	LuaResetBaseAttribute},
	//-
	{"GetExtPoint",			LuaGetExtPoint},
	{"PayExtPoint",			LuaPayExtPoint},
	//-
	{"AddJbCoin",			LuaAddJbCoin},
	{"GetJbCoin",			LuaGetJbCoin},
	{"PayJbCoin",			LuaPayJbCoin},

	{"AddLockBronzeCoin",	LuaAddLockBronzeCoin},
	{"GetLockBronzeCoin",	LuaGetLockBronzeCoin},
	{"PayLockBronzeCoin",	LuaPayLockBronzeCoin},
	{"ClearLockBronzeCoin",	LuaClearLockBronzeCoin},
	//-
	{"LoginDelaySync",		LuaSetLoginDelaySync},
	{"MyRefresh",			LuaMyRefresh},
	//-
	{"ReloadScript",		LuaReloadScript},
	{"ReloadAllScript",		LuaReloadAllScript},
	//-
	{"SetMask",				LuaSetMask},
	{"ResetMask",			LuaResetMask},
	{"ChangeOwnFeature",	LuaChangeOwnFeature},
	{"RestoreOwnFeature",	LuaRestoreOwnFeature},
	//-
	{"ForbidTownPortal",	LuaForbidTownPortal},
	{"GetTownPortalFlag",	LuaGetTownPortalFlag},
	{"GetGameTime",			LuaGetGameTime},
	{"FileName2Id",			LuaFileName2Id},
	{"TabFile_Load",		LuaTabFile_Load},
	{"TabFile_Clear",		LuaTabFile_Clear},
	{"TabFile_GetColumCount",LuaTabFile_GetColumCount}, 
	{"TabFile_GetRowCount",	LuaTabFile_GetRowCount},
	{"TabFile_GetCell",		LuaTabFile_GetCell},
	{"IniFile_Load",		LuaIniFile_Load},
	{"IniFile_Clear",		LuaIniFile_Clear},
	{"IniFile_GetData",		LuaIniFile_GetData},
	{"IniFile_SaveData",	LuaIniFile_SaveData},
	{"AskClientForNumber",	LuaAskClientForNumber},
	{"AskClientForString",	LuaAskClientForString},
	{"GetTongID",			LuaGetTongID},
	{"GetTongName",			LuaGetTongName},
	{"GetTongMaster",		LuaGetTongMaster},
	{"GetTongFigure",		LuaGetTongFigure},
	{"GetMagicAtrribRecord",LuaGetMagicAttribRecord},
	{"CheckMagicAttrib",	LuaCheckMagicAttrib},
	{"GetMAByRecord",		LuaGetMAByRecord},
	{"SavePlayerLog",		LuaSavePlayerLog},
	{"OpenMyShop",			LuaPlayerOpenShop},
	{"CallClientScript",	LuaCallClientScript },
	{"BanAccount",			LuaBanAccount},
	{"UnBanAccount",		LuaUnBanAccount},//Unband account by kinnox;
	{"DropObj",				LuaDropObj},
	{"DelObj",				LuaDelObj},
	{"CalcFreeItemCellCount",LuaCalcFreeItemCellCount},//AutoAI by kinnox;
	{"SetObjPickExecute",	LuaSetObjPickExecute},//executeitem by kinnox;
	
	// Random by kinnox;
	{"RANDOM",				LuaRANDOM},
	{"RANDOMC",				LuaRANDOMC},
	{"Random",				LuaRANDOM},
	//

	//GetTimeNow By kinnox;
	{"GetTimeNow",			LuaGetTimeNow},
	//
	{"KickOutPlayer",		LuaKickOutPlayer},
	{"KickOutAccount",		LuaKickOutAccount},
	//load 3 storebox by kinnox;
	{"GetStoreBox",			LuaGetStoreBox},
	{"SetStoreBox",			LuaSetStoreBox},
	//end code;

	//ExPandBox by kinnox;
	{"GetExPandBox",		LuaGetExpandBox},
	{"SetExPandBox",		LuaSetExpandBox},
	//	
	
	//GiveBox by kinnox;
	{"OpenGiveBox",			LuaOpenGiveBox},
	{"EndGiveBox",			LuaEndGiveBox},
	{"GetROItem",			LuaGetROItem},
	//
	
	//RewardBox by kinnox;
	{"OpenReWardBox",		LuaOpenReWardBox},
	//
	
	//TimeBox;
	{"TimeBox",				LuaOpenTimeBox},
	//
	
	//Lixian by kinnox;
	{"SetStatusLixian",		LuaSetStatusLixian},
	{"SetDisconectPlayer",  LuaSetDisconectPlayer},
	//
	
	//
	{"UpdateTopTKNew",		LuaUpdateTopTKNew},
	{"DeleteTopTKNew",		LuaDeleteTopTKNew},
	{"SendTopTKNew",		LuaSendTopTKNew},	
	//
	{"DelPwbox",			LuaDelpwbox},
	{"SendNotification",	LuaOpenNotification},
	//TrembleItem
	{"SetPItemID",			LuaSetPItemID},
	{"GetPOItem",			LuaGetPOItem},
	{"TrembleItem",			LuaOpenTrembleItem},
	//
	{"SetLockSongJin",		LuaSetLockSongJin},
	{"GetLockSongJin",		LuaGetLockSongJin},
#else 
	{"PlaySound",			LuaPlaySound},
	{"PlaySprMovie",		LuaPlaySprMovie},
	{"ExitGame",			LuaExitGame },
#endif
	{"PlayMusic",			LuaPlayMusic},
	{"FadeInMusic",			LuaFadeInMusic},
	{"FadeOutMusic",		LuaFadeOutMusic},
};


TLua_Funcs WorldScriptFuns[] =
{
	{"AddLocalNews",		LuaAddLocalNews},
	{"AddLoaclTimeNews",	LuaAddLocalTimeNews},
	{"AddLocalCountNews",	LuaAddLocalCountNews	},
#ifdef _SERVER
	{"Msg2SubWorld",		LuaMsgToSubWorld},
	{"Msg2IP",				LuaMsgToIP},
	{"SubWorldID2Idx",		LuaSubWorldIDToIndex},
	{"SubWorldIdx2ID",		LuaSubWorldIndexToID}, 
	{"GetServerName",		LuaGetServerName},
	{"KickOutPlayer",		LuaKickOutPlayer},
	{"KickOutAccount",		LuaKickOutAccount},
#endif
}; 

int g_GetGameScriptFunNum()
{
	return sizeof(GameScriptFuns)  / sizeof(GameScriptFuns[0]);
}

int g_GetWorldScriptFunNum()
{
	return sizeof(WorldScriptFuns)  / sizeof(WorldScriptFuns[0]);
}