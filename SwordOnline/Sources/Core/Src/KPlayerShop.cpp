
#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerShop.h"
#ifndef _SERVER
#include	"CoreShell.h"
#include	"KItemSet.h"
#include	"KItem.h"
#endif

// --
//
// --
KShop::KShop()
{
	Release();	
}

// --
//
// --
void	KShop::Release()
{	
#ifdef _SERVER
	m_bIsOpen		= FALSE;
	ZeroMemory(m_szAdvText, sizeof(m_szAdvText));
#else
	m_dwNpcID		= 0;
	ZeroMemory(m_szName, sizeof(m_szName));
	if(m_sItem)
	{
		delete []m_sItem;
		m_sItem = NULL;
	}
	if(m_dwItemID)
	{
		delete []m_dwItemID;
		m_dwItemID = NULL;
	}
	m_btUseIdx		= 0;
	m_btMaxItem		= 0;
	m_nDataIndex	= 0;
#endif
}

#ifndef _SERVER
// --
//
// --
void	KShop::ApplyViewShop(DWORD dwNpcID, int nDataIndex /*= 0*/)
{
	VIEW_SHOP_COMMAND	sView;
	sView.ProtocolType	= c2s_viewshop;
	sView.dwID = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
	sView.dwTimePacker = GetTickCount();
	sView.m_dwNpcID		= dwNpcID;
	sView.m_nDataIndex	= nDataIndex;
	if (g_pClient)
		g_pClient->SendPackToServer(&sView, sizeof(sView));
}
#endif


#ifndef _SERVER
// --
//
// --
void	KShop::DeleteAll()
{
	m_dwNpcID	= 0;
	ZeroMemory(m_szName, sizeof(m_szName));

	for (int i = 0; i < m_btMaxItem; i ++)
	{	
		if (m_sItem[i].nIdx > 0)
			ItemSet.Remove(m_sItem[i].nIdx);
	}
	if(m_sItem)
	{
		delete []m_sItem;
		m_sItem = NULL;
	}
	if(m_dwItemID)
	{
		delete []m_dwItemID;
		m_dwItemID = NULL;
	}
	m_btMaxItem		= 0;
	m_btUseIdx		= 0;
	m_nDataIndex	= 0;
}
#endif

#ifndef _SERVER
// --
//
// --
void	KShop::GetData(BYTE* pMsg)
{
	if (!pMsg)
		return;
	
	int	i, j;
	int	nNpcIdx, nItemIdx, nMagicParam[6];
	KUiPlayerItem	sPlayer;

	VIEW_SHOP_SYNC		*pView = (VIEW_SHOP_SYNC*)pMsg;
	
	
	m_dwNpcID = pView->m_dwNpcID;
	sPlayer.uId = m_dwNpcID;
	nNpcIdx = NpcSet.SearchID(m_dwNpcID);
	if (nNpcIdx > 0)
	{	
		strcpy(m_szName, Npc[nNpcIdx].Name);
		sPlayer.nIndex = nNpcIdx;
		strcpy(sPlayer.Name, m_szName);
		sPlayer.nData = Npc[nNpcIdx].GetMenuState();
	}
	else
	{
		sPlayer.nIndex = 0;
		sPlayer.Name[0] = 0;
		sPlayer.nData = 0;
	}

	if(pView->m_btCount > 0)
	{
		m_btMaxItem = pView->m_btCount;
		m_sItem	= new PlayerItem[m_btMaxItem];
		m_dwItemID = new DWORD[m_btMaxItem];
	}

	m_nDataIndex = pView->m_nDataIndex;

	for (i = 0; i < defMAX_SHOPITEM_PER_UPDATE; i ++)
	{
		if (pView->m_sInfo[i].m_ID == 0)
			continue;
		for (j = 0; j < 6; j++)
			nMagicParam[j] = pView->m_sInfo[i].m_MagicLevel[j];
		nItemIdx = ItemSet.AddExist(
			pView->m_sInfo[i].m_Genre,
			pView->m_sInfo[i].m_Series,
			pView->m_sInfo[i].m_Level,
			
			pView->m_sInfo[i].m_Record,
			
			pView->m_sInfo[i].m_Luck,
			pView->m_sInfo[i].m_Detail,
			nMagicParam,
			pView->m_sInfo[i].m_Version,
			pView->m_sInfo[i].m_RandomSeed);
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
			continue;
		Item[nItemIdx].SetID(pView->m_sInfo[i].m_ID);
		Item[nItemIdx].SetPlayerShopPrice(pView->m_sInfo[i].m_StorePrice);
		//them SendStackNum fix by player shop by kinnox;
		Item[nItemIdx].SetStackCount(pView->m_sInfo[i].m_StackNum);
		//
		m_sItem[m_btUseIdx].nIdx = nItemIdx;
		m_sItem[m_btUseIdx].nX	= pView->m_sInfo[i].m_X;
		m_sItem[m_btUseIdx].nY	= pView->m_sInfo[i].m_Y;
		m_dwItemID[m_btUseIdx]	= Item[nItemIdx].GetID();
		
		m_btUseIdx++;
	}
	
	if(m_btUseIdx < m_btMaxItem)
	{
		ApplyViewShop(m_dwNpcID, m_nDataIndex);
	}
	if(m_btUseIdx >= m_btMaxItem)
		CoreDataChanged(GDCNI_VIEW_PLAYERSHOP, (DWORD)&sPlayer, 0);
}
#endif

#ifndef _SERVER
// --
//
// --
DWORD	KShop::FindID(int nIndex)
{	
	if(nIndex <= 0 || nIndex >= MAX_ITEM)
		return 0;

	for(int i = 0; i < m_btMaxItem; i++)
	{
		if(m_sItem[i].nIdx == nIndex)
		{
			ItemSet.Remove(nIndex);
			m_sItem[i].nIdx = 0;
			m_btUseIdx--;
			return m_dwItemID[i];
		}
	}
	return 0;
}
#endif

#ifndef _SERVER
// --
//
// --
void	KShop::UpdateData()
{	
	CoreDataChanged(GDCNI_VIEW_PLAYERSHOP, NULL, 0);
}
#endif