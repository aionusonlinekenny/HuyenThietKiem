// --
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcResList.cpp
// Date:	2002.01.06
// Code:	边城浪子
// Desc:	Obj Class
// --
#include "KCore.h"

#ifndef _SERVER

#include	"CoreUseNameDef.h"
#include	"KNpcResList.h"

#ifndef _SERVER
KNpcResList	g_NpcResList;
#endif

KNpcResList::KNpcResList()
{
}

KNpcResList::~KNpcResList()
{
	KNpcResNode		*pTempNode = NULL;
	KNpcResNode		*pTempNextNode = NULL;

    // 释放所有链表中的节点
   	pTempNode = (KNpcResNode *)this->GetHead();
	while (pTempNode)
    {
		pTempNextNode = (KNpcResNode *)pTempNode->GetNext();
        delete pTempNode;
        pTempNode = pTempNextNode;
	}
}


//	功能：	初始化

BOOL	KNpcResList::Init()
{
	if ( !m_cActionName.Init(ACTION_FILE_NAME) )
		return FALSE;
	if ( !m_cNpcAction.Init(NPC_ACTION_NAME) )
		return FALSE;
	if ( !m_cStateTable.Init() )
		return FALSE;
	if ( !m_cMenuState.Init() )
		return FALSE;
	return TRUE;
}


//	功能：	查找同名的 NpcResNode 并返回节点

KNpcResNode*	KNpcResList::FindNpcRes(char *lpszNpcName)
{
	if ( !lpszNpcName || !lpszNpcName[0])
		return NULL;

	KNpcResNode		*pTempNode;

	pTempNode = (KNpcResNode *)this->GetHead();
	if (pTempNode == NULL)
		return NULL;
	if (strcmp(pTempNode->m_szNpcName, lpszNpcName) == 0)
	{
		return pTempNode;
	}

	while (1)
	{
		pTempNode = (KNpcResNode *)pTempNode->GetNext();
		if (pTempNode == NULL)
			return NULL;
		if (strcmp(pTempNode->m_szNpcName, lpszNpcName) == 0)
		{
			return pTempNode;
		}
	}

	return NULL;
}


//	功能：	添加一个 NpcResNode 并返回节点，如果已存在，直接返回节点

KNpcResNode*	KNpcResList::AddNpcRes(char *lpszNpcName)
{
	if ( !lpszNpcName || !lpszNpcName[0])
		return FALSE;

	DWORD currentTime = GetTickCount();								
	KNpcResNode		*pTempNode;
	pTempNode = FindNpcRes(lpszNpcName);
	if ( pTempNode != NULL )
	{
	// LRU: Update access time when existing node is reused
		pTempNode->m_dwLastAccessTime = currentTime;
		return pTempNode;
	}
	pTempNode = new KNpcResNode;
	if ( !pTempNode->Init(lpszNpcName, &m_cActionName, &m_cNpcAction) )
	{
		delete pTempNode;
		pTempNode = NULL; //them code 16/09/2023 by kinnox;
		return NULL;
	}

	// LRU: Set initial timestamps
	pTempNode->m_dwLastAccessTime = currentTime;
	pTempNode->m_dwCreateTime = currentTime;
	this->AddTail(pTempNode);

	return pTempNode;
}

// --
// LRU: Auto cleanup old unused NPC resource nodes to prevent memory leak
// Strategy: Remove nodes that haven't been accessed for >30 minutes
// CRITICAL: Must be longer than typical AFK time to avoid deleting player/active NPC resources
// Player NPCs remain loaded while player is in-game, even if standing still
// --
void KNpcResList::AutoCleanup()
{
	DWORD currentTime = GetTickCount();
	DWORD timeout = 1800000;  // 30 minutes = 1,800,000 ms (was 5 min, too aggressive)

	KNpcResNode* pNode = (KNpcResNode*)this->GetHead();
	KNpcResNode* pNext = NULL;

	while (pNode)
	{
		pNext = (KNpcResNode*)pNode->GetNext();

		// Remove nodes not accessed for >30 minutes
		// This cleans up old NPC types from areas player left long ago
		// 30 min is safe - players typically logout or move within this time  
		if (pNode->m_dwLastAccessTime > 0 &&
			(currentTime - pNode->m_dwLastAccessTime) > timeout)
		{
			pNode->Remove();
			delete pNode;
		}

		pNode = pNext;
	}
}
#endif