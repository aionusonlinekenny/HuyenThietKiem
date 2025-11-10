
#ifndef	KList_H
#define	KList_H
#include "KDebug.h"
#include "KNode.h"

class KList
{
public:
	KNode m_ListHead; // 头节点（不是第一个节点）
	KNode m_ListTail; // 尾节点（不是最后的节点）
public:
	KList(void);
	KNode* GetHead(void); // 取得第一个节点
	KNode* GetTail(void); // 取得最后一个节点
	void AddHead(KNode *pNode); // 在最前面增加一个节点
	void AddTail(KNode *pNode); // 在最后面增加一个节点
	KNode* RemoveHead(void); // 删除第一个节点
	KNode* RemoveTail(void); // 删除最后一个节点
	BOOL IsEmpty(void); // 是否是个空的链表
	LONG GetNodeCount(void);
};

inline KList::KList(void)
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}

inline BOOL KList::IsEmpty(void)
{
	return (m_ListHead.GetNext() == NULL);
}

inline KNode* KList::GetHead(void)
{
	return m_ListHead.GetNext();
}

inline KNode* KList::GetTail(void)
{
	return m_ListTail.GetPrev();
}

inline void KList::AddHead(KNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}

inline void KList::AddTail(KNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}

inline KNode* KList::RemoveHead(void)
{
	KNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}

inline KNode* KList::RemoveTail(void)
{
	KNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}

inline LONG KList::GetNodeCount(void)
{
	long nNode = 0;
	KNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}


#endif
