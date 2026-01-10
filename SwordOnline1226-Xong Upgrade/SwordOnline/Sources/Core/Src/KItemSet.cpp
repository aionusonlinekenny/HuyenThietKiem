#include "KCore.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "KItemSet.h"
#ifdef _SERVER
#include "KLicense.h"
#include "KObjSet.h"
#endif

KItemSet	ItemSet;

#ifdef _SERVER
KLicense	g_ItemLicense;
#endif

// --
//
// --
KItemSet::KItemSet()
{	
	m_dwIDCreator = 100;
	ZeroMemory(&m_sRepairParam, sizeof(REPAIR_ITEM_PARAM));
#ifdef _SERVER
	m_psItemInfo = NULL;
	m_psBackItemInfo = NULL;
#endif

	m_nCoinID = 0;
}

// --
//
// --
KItemSet::~KItemSet()
{
#ifdef _SERVER
	if (m_psItemInfo)
		delete [] m_psItemInfo;
	m_psItemInfo = NULL;
	if (m_psBackItemInfo)
		delete [] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
#endif
}

// --
//
// --
void KItemSet::Init()
{	
	m_bLock		= FALSE;

	m_FreeIdx.Init(MAX_ITEM);
	m_UseIdx.Init(MAX_ITEM);

	int i = 0;
	for (i = MAX_ITEM - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
	}
	
#ifdef _SERVER
	if (m_psItemInfo)
		delete [] m_psItemInfo;
	m_psItemInfo = NULL;
	m_psItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
	if (m_psBackItemInfo)
		delete [] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
	m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psBackItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
#endif

	g_GameSettingFile.GetInteger("SHOP", "CoinID", 19, &m_nCoinID);

	KIniFile	IniFile;
	IniFile.Load(ITEM_ABRADE_FILE);
	IniFile.GetInteger("Repair", "ItemPriceScale", 100, &m_sRepairParam.nPriceScale);
	IniFile.GetInteger("Repair", "MagicPriceScale", 10, &m_sRepairParam.nMagicScale);
	IniFile.GetInteger("Attack", "Weapon", 256, &m_nItemAbradeRate[enumAbradeAttack][itempart_weapon]);
	IniFile.GetInteger("Attack", "Head", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_head]);
	IniFile.GetInteger("Attack", "Body", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_body]);
	IniFile.GetInteger("Attack", "Belt", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_belt]);
	IniFile.GetInteger("Attack", "Foot", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_foot]);
	IniFile.GetInteger("Attack", "Cuff", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_cuff]);
	IniFile.GetInteger("Attack", "Amulet", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_amulet]);
	IniFile.GetInteger("Attack", "Ring1", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring1]);
	IniFile.GetInteger("Attack", "Ring2", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring2]);
	IniFile.GetInteger("Attack", "Pendant", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_pendant]);
	IniFile.GetInteger("Attack", "Horse", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_horse]);
	IniFile.GetInteger("Defend", "Weapon", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_weapon]);
	IniFile.GetInteger("Defend", "Head", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_head]);
	IniFile.GetInteger("Defend", "Body", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_body]);
	IniFile.GetInteger("Defend", "Belt", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_belt]);
	IniFile.GetInteger("Defend", "Foot", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_foot]);
	IniFile.GetInteger("Defend", "Cuff", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_cuff]);
	IniFile.GetInteger("Defend", "Amulet", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_amulet]);
	IniFile.GetInteger("Defend", "Ring1", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring1]);
	IniFile.GetInteger("Defend", "Ring2", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring2]);
	IniFile.GetInteger("Defend", "Pendant", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_pendant]);
	IniFile.GetInteger("Defend", "Horse", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_horse]);
	IniFile.GetInteger("Move", "Weapon", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_weapon]);
	IniFile.GetInteger("Move", "Head", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_head]);
	IniFile.GetInteger("Move", "Body", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_body]);
	IniFile.GetInteger("Move", "Belt", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_belt]);
	IniFile.GetInteger("Move", "Foot", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_foot]);
	IniFile.GetInteger("Move", "Cuff", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_cuff]);
	IniFile.GetInteger("Move", "Amulet", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_amulet]);
	IniFile.GetInteger("Move", "Ring1", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring1]);
	IniFile.GetInteger("Move", "Ring2", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring2]);
	IniFile.GetInteger("Move", "Pendant", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_pendant]);
	IniFile.GetInteger("Move", "Horse", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_horse]);

}

// --
//
// --
int KItemSet::SearchID(DWORD dwID)
{
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if(!nIdx)
			break;
		if(Item[nIdx].GetID() == dwID)
			break;
	}
	return nIdx;
	
}
// --
//
// --
int KItemSet::Add(KItem* pItem)
{	
	const int i = FindFree();
	if (!i)
	{	
		printf("[Add*] Find Free Node Item Error!\n");
		return 0;
	}

	Item[i] = *pItem;
#ifdef _SERVER
	if( !SetID(i) )
	{	
		return 0;
	}
#endif
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);
	return i;

}
// --
//
// --
int KItemSet::Add(int nItemGenre, int nSeries, 
				  int nLevel, WORD wRecord, int nLuck, int nDetailType/*=-1*/, 
				  int nParticularType/*=-1*/, int* pnMagicLevel, int nVersion/*=0*/, DWORD dwRandomSeed,
				  IN BYTE btStackNum /*= 1*/, IN DWORD dwExpiredTime /*= 0*/)
{	
	const int i = FindFree();
	if (!i)
	{	
		printf("[Add] Find Free Node Item Error!\n");
		return 0;
	}

	KItem*	pItem = &Item[i];

	pItem->SetRandomSeed(dwRandomSeed);
	pItem->SetGeneratorLevel(pnMagicLevel);
	pItem->SetGeneratorLuck(nLuck);
	pItem->SetGeneratorVersion(nVersion);
	
	switch(nItemGenre)
	{
	case item_equip:
		ItemGen.Gen_Equipment(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_purpleequip:
		ItemGen.Gen_PurpleEquipment(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_goldequip:
		ItemGen.Gen_GoldEquipment(wRecord, nSeries, pItem);
		break;
	case item_platinaequip:
		break;
	case item_brokenequip:
		ItemGen.Gen_BrokenEquipment(wRecord, nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, TRUE, pItem);
		break;
	case item_medicine:
		ItemGen.Gen_Medicine(nDetailType, nLevel, pItem);
		break;
	case item_task:
		ItemGen.Gen_Quest(nDetailType, pItem);
		break;
	case item_script:
		ItemGen.Gen_Script(nDetailType, pItem);
		break;
	case item_mine:
		ItemGen.Gen_Mine(nDetailType, nLevel, nSeries, pItem);
		break;
	default:
		break;
	}

#ifdef _SERVER
	if( !SetID(i) )
	{	
		return 0;
	}
#endif
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);

	return i;
}

// --
//
// --
int KItemSet::AddExist(IN int nItemGenre, IN int nSeries, IN int nLevel,
					   IN WORD wRecord, IN int nLuck, IN int nDetailType/*=-1*/, 
					   IN int* pnMagicLevel, IN int nVersion/*=0*/, IN DWORD dwRandomSeed /*= 0*/)
{	
	int i = FindFree();
	
	if (!i)
	{	
		printf("[AddExist] Find Free Node Item Error!\n");
		return 0;
	}
	
	KItem*	pItem = &Item[i];

	pItem->SetRandomSeed(dwRandomSeed);
	pItem->SetGeneratorLevel(pnMagicLevel);
	pItem->SetGeneratorLuck(nLuck);
	pItem->SetGeneratorVersion(nVersion);
	switch(nItemGenre)
	{
	case item_equip:
		ItemGen.Gen_ExistEquipment(wRecord, nDetailType, nSeries, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_purpleequip:
		ItemGen.Gen_ExistPurpleEquipment(wRecord, nDetailType, nSeries, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_goldequip:
		ItemGen.Gen_GoldEquipment(wRecord, nSeries, pItem);
		break;
	case item_platinaequip:
		break;
	case item_brokenequip:
		ItemGen.Gen_BrokenEquipment(wRecord, nDetailType, -1, nSeries, 0, pnMagicLevel, nLuck, nVersion, FALSE, pItem);
		break;
	case item_medicine:
		ItemGen.Gen_ExistMedicine(wRecord, pItem);
		break;
	case item_task:
		ItemGen.Gen_ExistQuest(wRecord, pItem);
		break;
	case item_script:
		ItemGen.Gen_ExistScript(wRecord, pItem);
		break;
	case item_mine:
		ItemGen.Gen_ExistMine(wRecord, nLevel, nSeries, pItem);
		break; 
	default:
		break;
	}

#ifdef _SERVER
	if(!SetID(i))
	{	
		return 0;
	}
#endif
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);
	return i;
}

// --
//
// --
int KItemSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

// --
//
// --
void KItemSet::Remove(IN int nIdx)
{	

	Item[nIdx].Remove();
	
	m_UseIdx.Remove(nIdx);
	m_FreeIdx.Insert(nIdx);

}

#ifdef _SERVER
// --
//
// --
BOOL KItemSet::SetID(IN int nIdx)
{	
	
	Item[nIdx].SetID(m_dwIDCreator);
	m_dwIDCreator++;

	return TRUE;
}
#endif

#ifdef _SERVER
// --
//
// --
void	KItemSet::BackItemInfo()
{
	_ASSERT(this->m_psItemInfo);
	_ASSERT(this->m_psBackItemInfo);
	if (!m_psItemInfo)
		return;
	if (!m_psBackItemInfo)
		m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memcpy(m_psBackItemInfo, this->m_psItemInfo, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
}
#endif

// --
//
// --
int KItemSet::GetAbradeRange(IN int nType, IN int nPart)
{
	if (nType < 0 || nType >= enumAbradeNum)
		return 0;
	if (nPart < 0 || nPart >= itempart_num)
		return 0;

	return m_nItemAbradeRate[nType][nPart];
}

#ifdef _SERVER
// --
//
// --
int	KItemSet::DropItem(int nSubWorldIdx, int nMpsX, int nMpsY, int nBelongPlayerIdx, int nItemGenre, int nItemDetail, int nItemParticular,
								int nItemLevel, int nItemSeries, int nLuck, int* pnMagicLevel /*= NULL*/, int nExpiredTime /*= 0*/)
{
	if(nSubWorldIdx < 0 || nSubWorldIdx >= MAX_SUBWORLD)
		return 0;

	int nIdx = ItemSet.Add(nItemGenre, nItemSeries, nItemLevel, 0, nLuck, nItemDetail, nItemParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion());

	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return 0;
	
	if(nExpiredTime > 0)
	{
		Item[nIdx].SetTime(nExpiredTime);
	}

	POINT	ptLocal;
	KMapPos	Pos;

	ptLocal.x = nMpsX;
	ptLocal.y = nMpsY;
	SubWorld[nSubWorldIdx].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = nSubWorldIdx;
	SubWorld[nSubWorldIdx].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	KObjItemInfo sInfo;
	sInfo.m_nItemID = nIdx;
	sInfo.m_dwItemID = Item[nIdx].GetID();
	sInfo.m_nItemWidth = Item[nIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	if(Item[nIdx].GetGenre() == item_task ||
		Item[nIdx].GetGenre() == item_script)
	{
		if(Item[nIdx].GetMaxStackCount() > 0)
		{	
			char szTemp[FILE_NAME_LENGTH];
			sprintf(szTemp, "%s x%d", Item[nIdx].GetName(), Item[nIdx].GetStackCount());
			strcpy(sInfo.m_szName, szTemp);
		}
		else
		{
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
		}
	}
	else
	{
		strcpy(sInfo.m_szName, Item[nIdx].GetName());
	}
	sInfo.m_nColorID = 0;
	//AutoAI by kinnox;
	sInfo.m_nGenre = Item[nIdx].GetGenre();
	sInfo.m_nDetailType = Item[nIdx].GetDetailType();
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
	sInfo.m_dwNpcId = 0;
	//end code;
	nObj = ObjSet.Add(Item[nIdx].GetObjID(), Pos, sInfo);
	if (nObj == -1)
	{
		ItemSet.Remove(nIdx);
	}
	else
	{
		if (nBelongPlayerIdx > 0)
			Object[nObj].SetItemBelong(nBelongPlayerIdx);
		else
			Object[nObj].SetItemBelong(-1);
	}
	return nObj;//executeitem by kinnox;
}
#endif