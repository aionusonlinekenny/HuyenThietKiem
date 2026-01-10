#include "KEngine.h"
#include "KCore.h"
#include "KTabFile.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "KItemGenerator.h"
#include "KSubWorldSet.h"
#include "KItemSet.h"
#include "KBuySell.h"
#ifndef _STANDALONE
#include "CoreShell.h"
#include "crtdbg.h"
#endif
#include "CoreUseNameDef.h"
#ifdef _SERVER
//#include "../MultiServer/Heaven/Interface/iServer.h"
#endif

#define	SHOP_BOX_WIDTH		6
#define	SHOP_BOX_HEIGHT		10

KBuySell	BuySell;

// --
//
// --
KBuySell::KBuySell()
{
#ifndef _SERVER
	m_pShopRoom = NULL;
#endif
	m_Item = NULL;
	m_SellItem = NULL;
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
#ifndef _SERVER
	m_MarketID = NULL;
	m_MarketName = NULL;
	m_MarketHeight = 0;
#endif

}

// --
//
// --
KBuySell::~KBuySell()
{
#ifndef _SERVER
	if (m_pShopRoom)
	{
		delete m_pShopRoom;
		m_pShopRoom = NULL;
	}
#endif
	if (m_Item)
	{
		delete [] m_Item;
		m_Item = NULL;
	}
	int i = 0;
	if (m_SellItem)
	{
		for (i = 0; i < m_Height; i++)
		{
			if (m_SellItem[i])
			{
				delete m_SellItem[i];
				m_SellItem[i] = NULL;
			}
		}
		delete m_SellItem;
		m_SellItem = NULL;
	}
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
#ifndef _SERVER
	if(m_MarketID)
	{
		delete m_MarketID;
		m_MarketID = NULL;
	}
	if(m_MarketName)
	{	
		for(i = 0; i < m_MarketHeight; i++)
		{
			if(m_MarketName[i])
			{
				delete m_MarketName[i];
				m_MarketName[i] = NULL;
			}
		}

		delete m_MarketName;
		m_MarketName = NULL;
	}
	m_MarketHeight = 0;
#endif
	
}

// --
//
// --
BOOL KBuySell::Init()
{
	KTabFile		GoodsFile;
	KTabFile		BuySellFile;

	if (!BuySellFile.Load(BUYSELL_FILE) || !GoodsFile.Load(GOODS_FILE))
		return FALSE;

	int nHeight = GoodsFile.GetHeight() - 1;
	int nWidth = GoodsFile.GetWidth() - 1;
	if (nWidth == 0 || nHeight == 0)
		return FALSE;

	m_Item = (KItem *)new KItem[nHeight];
	if (!m_Item)
		return FALSE;

	ItemGenerate ItemGenerator;
	int k;
	for (k = 0; k < nHeight; k++)
	{
		GoodsFile.GetInteger(k + 2, 1, -1, &ItemGenerator.nGenre);
		GoodsFile.GetInteger(k + 2, 2, -1, &ItemGenerator.nDetailType);
		GoodsFile.GetInteger(k + 2, 3, -1, &ItemGenerator.nParticularType);
		GoodsFile.GetInteger(k + 2, 4, -1, &ItemGenerator.nSeriesReq);
		GoodsFile.GetInteger(k + 2, 5, -1, &ItemGenerator.nLevel);
		switch(ItemGenerator.nGenre)
		{
		case item_equip:
			ItemGen.Gen_Equipment(ItemGenerator.nDetailType,
				ItemGenerator.nParticularType,
				ItemGenerator.nSeriesReq,
				ItemGenerator.nLevel,
				NULL,
				0,
				g_SubWorldSet.GetGameVersion(),
				&m_Item[k]);
			break;
		case item_purpleequip:
			break;
		case item_goldequip:
			ItemGen.Gen_GoldEquipment(ItemGenerator.nDetailType, ItemGenerator.nSeriesReq, &m_Item[k]);
			break;
		case item_platinaequip:
			break;
		case item_brokenequip:
			break;
		case item_medicine:
			ItemGen.Gen_Medicine(ItemGenerator.nDetailType,	ItemGenerator.nLevel, &m_Item[k]);
			break;
		case item_task:
			ItemGen.Gen_Quest(ItemGenerator.nDetailType, &m_Item[k]);
			break;
		case item_script:
			ItemGen.Gen_Script(ItemGenerator.nDetailType, &m_Item[k]);
			break;
		default:
			break;
		}
		int h;
		for(h = 0; h < 6; h++)
		{
			GoodsFile.GetInteger(k + 2, 7 + h, -1, &ItemGenerator.nPrice);
			if(ItemGenerator.nPrice > 0)
			{
				m_Item[k].SetPrice(ItemGenerator.nPrice, h);
				break;
			}
		}
		GoodsFile.GetInteger(k + 2, 13, -1, &ItemGenerator.nPrice);
		if(ItemGenerator.nPrice > 0)
			m_Item[k].SetDisCountPrice((BYTE)ItemGenerator.nPrice);

		GoodsFile.GetInteger(k + 2, 14, -1, &ItemGenerator.nPrice);
		if(ItemGenerator.nPrice > 0)
		{	
			if(ItemGenerator.nPrice > 20000000)
			{
				m_Item[k].SetTime(ItemGenerator.nPrice);
			}
			else
			{
				m_Item[k].SetDaySell(ItemGenerator.nPrice);
			}
		}
#ifndef _SERVER
		GoodsFile.GetInteger(k + 2, 15, -1, &ItemGenerator.nPrice);
		if(ItemGenerator.nPrice > 0)
			m_Item[k].SetHotSell(TRUE);
#endif

		m_MaxItem++;
	}

	m_Height = BuySellFile.GetHeight() - 1;
	m_Width  = BuySellFile.GetWidth();
	
	if (m_Width == 0 || m_Height == 0)
		return FALSE;

	m_SellItem = (int **)new int*[m_Height];
	if (!m_SellItem)
		return FALSE;
	
	int i, j;
	for (i = 0; i < m_Height; i++)
	{
		m_SellItem[i] = NULL;
		m_SellItem[i] = (int *)new int[m_Width];
		if (!m_SellItem[i])
			return FALSE;
		for (j = 0; j < m_Width; j++)
		{
			BuySellFile.GetInteger(i + 2, j + 1, -1, &m_SellItem[i][j]);
			if (m_SellItem[i][j] == -1)
			{
				continue;
			}
			_ASSERT(m_SellItem[i][j] > 0);		
			if (m_SellItem[i][j] > 0)
				m_SellItem[i][j] -= 1;			
		}
	}

#ifndef _SERVER
	if (!m_pShopRoom)
	{
		m_pShopRoom = new KInventory;
		m_pShopRoom->Init(SHOP_BOX_WIDTH, SHOP_BOX_HEIGHT);
	}
#endif
#ifndef _SERVER
	KTabFile	MarketFile;
	if (!MarketFile.Load(MARKET_FILE))
		return FALSE;

	nHeight = MarketFile.GetHeight() - 1;
	nWidth = MarketFile.GetWidth() - 1;

	if (nWidth == 0 || nHeight == 0)
		return FALSE;

	m_MarketHeight = nHeight;
	m_MarketID = (int *)new BYTE*[m_MarketHeight];
	m_MarketName = (char **)new BYTE*[m_MarketHeight];

	for (i = 0; i < m_MarketHeight; i++)
	{	
		m_MarketName[i] = NULL;
		m_MarketName[i] = (char *)new char[32];
		
		MarketFile.GetString(i + 2, 1, "", m_MarketName[i], sizeof(char) * 32);
		if (!m_MarketName[i][0])
		{
			continue;
		}
		MarketFile.GetInteger(i + 2, 2, -1, &m_MarketID[i]);
		if (m_MarketID[i] == -1)
		{
			continue;
		}	
	}
#endif
	
	return TRUE;
}

// --
//
// --
KItem* KBuySell::GetItem(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_MaxItem || !m_Item)
		return NULL;

	return &m_Item[nIndex];
}

// --
//
// --
int KBuySell::GetItemIndex(int nShop, int nIndex)
{
	if (!m_SellItem || nShop < 0 || nShop >= m_Height || nIndex < 0 || nIndex >= m_Width)
		return -1;

	if (!m_SellItem[nShop])
		return -1;

	return m_SellItem[nShop][nIndex];
}

#ifdef _SERVER
// --
//
// --
BOOL KBuySell::Buy(int nPlayerIdx, int nBuy, int nBuyIdx, int nPlace, int nX, int nY)
{
	KASSERT(nPlayerIdx >= 0 && nPlayerIdx < MAX_PLAYER);
	if(Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
		return FALSE;
	if (nBuy != Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx)
		return FALSE;

	if (nBuyIdx > m_Width)
		return FALSE;

	if (m_SellItem[nBuy][nBuyIdx] < 0 || m_SellItem[nBuy][nBuyIdx] >= m_MaxItem)
		return FALSE;

	//LockPlayer by kinnox;
	if (Player[nPlayerIdx].m_bLockPlayer)
	{
		BYTE	byFinished = s2c_itemexchangefinish;
		if (g_pServer)
			g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
		return FALSE;
	}
	//			
		
	int nIdx = m_SellItem[nBuy][nBuyIdx];
	BYTE btType = m_Item[nIdx].GetPriceType();
	int nPrice = m_Item[nIdx].GetPrice();
	switch(btType)
	{	
		case 1:
			{

				if (Player[nPlayerIdx].m_ItemList.CountSameItem(room_equipment, item_task, ItemSet.m_nCoinID, 1, -1, 1) < nPrice)
					return FALSE;
			}
			break;
		case 2:
			{
				if(Player[nPlayerIdx].TaskGetSaveVal(TV_FUYUAN) < nPrice)
					return FALSE;
			}
			break;
		case 3:
			{	
				if(Player[nPlayerIdx].TaskGetSaveVal(TV_SJPOINT) < nPrice)
					return FALSE;
			}
			break;
		case 4:
			{	
				if(Player[nPlayerIdx].TaskGetSaveVal(TV_FPOINT) < nPrice)
					return FALSE;
			}
			break;
		case 5:
			if(Player[nPlayerIdx].GetLockBronzeCoin() < nPrice)
				return FALSE;
			break;
		default:
			{	
				//char szTongName[defTONG_NAME_MAX_LENGTH + 1];
				//int nSubWorldIdx = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
			//	Player[nPlayerIdx].m_cTong.GetTongName(szTongName);
				//nPrice = m_Item[nIdx].GetPrice(TRUE, szTongName, nSubWorldIdx);

				if(Player[nPlayerIdx].m_ItemList.GetEquipmentMoney() < nPrice)
					return FALSE;
			}
			break;
	}
	BYTE btWidth = m_Item[nIdx].GetWidth();
	BYTE btHeight = m_Item[nIdx].GetHeight();
	ItemPos	Pos;
	if (!Player[nPlayerIdx].m_ItemList.SearchPosition((int)btWidth, (int)btHeight, &Pos))
		return FALSE;
	if (Pos.nPlace != pos_equiproom)
		return FALSE;
	if(Pos.nX != nX || Pos.nY != nY)
		return FALSE;

	int nItemIdx = ItemSet.Add(&m_Item[nIdx]);
	if (!nItemIdx)
		return FALSE;
	if(m_Item[nIdx].GetDaySell() > 0)
	{
		Item[nItemIdx].SetTime(m_Item[nIdx].GetDaySell());
	}
	switch(btType)
	{	
		case 1:
			if( !Player[nPlayerIdx].m_ItemList.RemoveItemInEquipment(nPrice, item_task, ItemSet.m_nCoinID, 1, -1, 1) )
				return FALSE;
			break;
		case 2:
			Player[nPlayerIdx].m_cTask.SetSaveVal(TV_FUYUAN, Player[nPlayerIdx].TaskGetSaveVal(TV_FUYUAN) - nPrice);
			break;
		case 3:
			Player[nPlayerIdx].m_cTask.SetSaveVal(TV_SJPOINT, Player[nPlayerIdx].TaskGetSaveVal(TV_SJPOINT) - nPrice);
			break;
		case 4:
			Player[nPlayerIdx].m_cTask.SetSaveVal(TV_FPOINT, Player[nPlayerIdx].TaskGetSaveVal(TV_FPOINT) - nPrice);
			break;
		case 5:
			if( !Player[nPlayerIdx].PayLockBronzeCoin(nPrice) )
				return FALSE;
			break;
		default:
			{
				if(!Player[nPlayerIdx].Pay(nPrice))
					return FALSE;
			}
			break;
	}
	if(btType == 0)
	{
		
	}
	
	if(btType == 5)
	{
		Item[nItemIdx].SetBindState(2);
	}

	if( !Item[nItemIdx].CanStack() || (nPlace == pos_hand) )
		Player[nPlayerIdx].m_ItemList.Add(nItemIdx, nPlace, nX, nY);
	else
		Player[nPlayerIdx].m_ItemList.Add_AutoStack(room_equipment, nItemIdx, nX, nY);
	
	if(Item[nItemIdx].GetGenre() != item_medicine)
	{
		char szData[128];
		//
		Item[nItemIdx].GetItemBackupInfo(szData);
		//
//		Player[nPlayerIdx].SaveLog(2, szData, "BUY ITEM", Item[nItemIdx].GetName());
	}

	return TRUE;
}

// --
//
// --
BOOL KBuySell::Sell(int nPlayerIdx, int nBuy, int nIdx)
{
	KASSERT(nPlayerIdx >= 0 && nPlayerIdx < MAX_PLAYER);
	KASSERT(nIdx >= 0 && nIdx < MAX_ITEM);

	if(Npc[Player[nPlayerIdx].m_nIndex].m_FightMode)
		return FALSE;

	if(Item[nIdx].GetBindState() > 0)
		return FALSE;

	if(Item[nIdx].GetGenre() == item_task || Item[nIdx].GetGenre() == item_script)
	{	
		if(Item[nIdx].GetParticular() == 0)
			return FALSE;
	}

	if (nBuy != Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx)
		return FALSE;

	//LockPlayer by kinnox;
	if (Player[nPlayerIdx].m_bLockPlayer)
	{
		BYTE	byFinished = s2c_itemexchangefinish;
		if (g_pServer)
			g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
		return FALSE;
	}
	//		
		
	int nMoney = Item[nIdx].GetPrice();
	nMoney /= BUY_SELL_SCALE;
	if (nMoney <= 0)
		nMoney = 1;

	if(Item[nIdx].GetGenre() != item_medicine)
	{
		char szData[128];
		Item[nIdx].GetItemBackupInfo(szData);
//		Player[nPlayerIdx].SaveLog(2, szData, "SELL ITEM", Item[nIdx].GetName());
	}

	if (Player[nPlayerIdx].m_bActiveAuto)// auto ban trang bi bang 1/3 gia ban tay by kinnox;
		Player[nPlayerIdx].Earn(nMoney/3);
	else
		Player[nPlayerIdx].Earn(nMoney);
	Player[nPlayerIdx].m_ItemList.RemoveByIndex(nIdx);
	
	return TRUE;
}
#endif

#ifndef _SERVER
// --
//
// --
void KBuySell::PaintItem(int nIdx, int nX, int nY)
{
	int nShop = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
	if (nShop < 0 || nShop >= m_Height)
		return;
	int nItemIdx = GetItemIndex(nShop, nIdx);

	int x = nX;
	int y = nY;

	KItem* pItem = GetItem(nItemIdx);

	if (pItem)
	{
		pItem->Paint(x, y, FALSE);
	}
}

// --
//
// --
void KBuySell::OpenSale(int nShop)
{
	if (nShop < 0 || nShop >= m_Height)
		return;

	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx = nShop;
	for(BYTE i = 0; i < m_MarketHeight; i++)
	{
		if(nShop == m_MarketID[i] - 1)
		{
			CoreDataChanged(GDCNI_MARKET_TRADE, NULL, NULL);
			return;
		}
	}
	CoreDataChanged(GDCNI_NPC_TRADE, NULL, TRUE);
}
#endif

#ifdef _SERVER
// --
//
// --
void KBuySell::OpenSale(int nPlayerIdx, int nShop)
{
	if (nPlayerIdx <= 0 || nPlayerIdx > MAX_PLAYER)
	{
		return;
	}

	Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx = nShop;
	Player[nPlayerIdx].m_BuyInfo.m_SubWorldID = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
	Npc[Player[nPlayerIdx].m_nIndex].GetMpsPos(
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsX,
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsY);

	SALE_BOX_SYNC saleSync;
	saleSync.ProtocolType = s2c_opensalebox;
	saleSync.nShopIndex = nShop;
	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &saleSync, sizeof(SALE_BOX_SYNC));
}
#endif
