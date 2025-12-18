#include "KCore.h"

#ifdef _SERVER
#include "KMission.h"

// --
//
// --
BOOL KMission::Activate()
{
	m_cTimerTaskSet.Activate();
	return TRUE;
}

// --
//
// --
BOOL KMission::ExecuteScript(char* ScriptFileName, char* szFunName, int nParam1 /*= 0*/, int nParam2 /*= 0*/)
{
	if(!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) 
		return FALSE;
	return ExecuteScript(g_FileName2Id(ScriptFileName), szFunName, nParam1, nParam2);	
}

// --
//
// --
BOOL KMission::ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam1, int nParam2)
{
	try
	{
		KLuaScript* pScript = (KLuaScript*)g_GetScript(dwScriptId);
		if (pScript)
		{
			KSubWorld* pSubWorld = (KSubWorld*)GetOwner();
			
			if(pSubWorld)
			{
				Lua_PushNumber(pScript->m_LuaState, pSubWorld->m_nIndex);
				pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			}
			
			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			pScript->CallFunction(szFunName,0, "dd", nParam1, nParam2);
			pScript->SafeCallEnd(nTopIndex);
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!\n", dwScriptId);
		return FALSE;
	}
	return TRUE;
}

// --
//
// --
int g_MissionTimerCallBackFun(void * pOwner, char * szScriptFile)
{
	if(!pOwner) 
		return FALSE;
	KMission* pMission = (KMission*)pOwner;
	pMission->ExecuteScript(szScriptFile, "OnTimer", 0);
//	if (szScriptFile)
//		printf("timer %s\n", szScriptFile);
	return 1;
}
#endif
