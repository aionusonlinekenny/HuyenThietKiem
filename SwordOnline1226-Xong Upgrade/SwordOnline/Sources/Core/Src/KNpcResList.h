

#pragma once

#ifndef _SERVER

#include "KList.h"
#include "KNpcResNode.h"
#include "KPlayerMenuState.h"

class KNpcResList : public KList
{
public:
	CActionName				m_cActionName;
	CActionName				m_cNpcAction;
	CStateMagicTable		m_cStateTable;
	KPlayerMenuStateGraph	m_cMenuState;

private:

	// 查找同名的 NpcResNode 并返回节点
	KNpcResNode*		FindNpcRes(char *lpszNpcName);

public:
    KNpcResList();
    ~KNpcResList();
	
	BOOL				Init();
	KNpcResNode*		AddNpcRes(char *lpszNpcName);
	void				AutoCleanup();			// LRU: Remove old unused NPC resources				 
};
#ifndef _SERVER
extern KNpcResList	g_NpcResList;
#endif

#endif