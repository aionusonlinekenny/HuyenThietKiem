#include	"KCore.h"
#include	"MyAssert.H"
#include	"KItem.h"
#include	"KItemSet.h"
#include	"KNpc.h"
#include	"KMath.h"
#include	"KPlayer.h"
#include	"KItemList.h"
#include	"KItemChangeRes.h"
#ifdef _SERVER
//#include	"../../Headers/IServer.h"
#include	"KObjSet.h"
#include	<time.h>
#endif
#ifndef _SERVER
#include	"CoreShell.h"
#include	"../../Headers/IClient.h"
#endif

#define		defEQUIP_POWER
#ifdef defEQUIP_POWER
	int		g_nEquipPower[itempart_num] =
	{2, 4, 2, 4, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1};
#endif

// --
//
// --
int KItemList::ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE] = 
{
	{ itempart_foot, itempart_ring1	},	//	itempart_head = 0,
	{ itempart_weapon, itempart_head },	//	itempart_body,
	{ itempart_body, itempart_amulet },	//	itempart_belt,
	{ itempart_ring1, itempart_foot },	//	itempart_weapon,
	{ itempart_cuff, itempart_pendant },//	itempart_foot,
	{ itempart_belt, itempart_ring2 },	//	itempart_cuff,
	{ itempart_head, itempart_weapon },	//	itempart_amulet,
	{ itempart_cuff, itempart_pendant },//	itempart_ring1,
	{ itempart_amulet, itempart_body },	//	itempart_ring2,
	{ itempart_belt, itempart_ring2 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
	{ itempart_mask, itempart_mask },	//	itempart_mask,
	{ itempart_mantle, itempart_mantle },	//	itempart_mantle,
	{ itempart_signet, itempart_signet },	//	itempart_signet,
	{ itempart_shipin, itempart_shipin },	//	itempart_shipin,
};
// --
//
// --
int KItemList::ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE] =
{
	{ itempart_body, itempart_amulet },	//	itempart_head = 0,
	{ itempart_ring2, itempart_belt },	//	itempart_body,
	{ itempart_pendant, itempart_cuff },//	itempart_belt,
	{ itempart_amulet, itempart_body },	//	itempart_weapon,
	{ itempart_weapon, itempart_head },	//	itempart_foot,
	{ itempart_foot, itempart_ring1 },	//	itempart_cuff,
	{ itempart_belt, itempart_ring2 },	//	itempart_amulet,
	{ itempart_weapon, itempart_head },	//	itempart_ring1,
	{ itempart_cuff, itempart_pendant },//	itempart_ring2,
	{ itempart_foot, itempart_ring1 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
	{ itempart_mask, itempart_mask },	//	itempart_mask,
	{ itempart_mantle, itempart_mantle },	//	itempart_mantle,
	{ itempart_signet, itempart_signet },	//	itempart_signet,
	{ itempart_shipin, itempart_shipin },	//	itempart_shipin,
};
// --
//
// --
int KItemList::ms_RoomMap[room_num] = {
	pos_equiproom, 
	pos_repositoryroom,
	//load 3 storebox by kinnox;
	pos_repositoryroom1,
	pos_repositoryroom2,
	pos_repositoryroom3,
	pos_repositoryroom4,
	pos_repositoryroom5,
	pos_expandtoryroom1,//Expandbox by kinnox;
	pos_givebox, //GiveBox by kinnox;
	//	
	pos_traderoom, 
	-1, pos_trade1, 
	pos_immediacy, 
	pos_builditem,//TrembleItem by kinnox;
};
// --
//
// --
KItemList::KItemList()
{
	m_PlayerIdx = 0;
	m_nListCurIdx = 0;
	m_GoldInfo = NULL;
}

// --
//
// --
KItemList::~KItemList()
{
	if(m_GoldInfo)
	{	
		int i;
		for(i = 0; i < itempart_horse; i++)
			delete [] m_GoldInfo[i];
		delete []m_GoldInfo;
		m_GoldInfo = NULL;
	}
}

// --
//
// --
int KItemList::GetWeaponType()
{
	if (m_EquipItem[itempart_weapon])
		return Item[m_EquipItem[itempart_weapon]].GetDetailType();
	else
		return -1;
}

// --
//
// --
void KItemList::GetWeaponDamage(int* nMin, int* nMax)
{
	int nWeaponIdx = m_EquipItem[itempart_weapon];
	if (nWeaponIdx)
	{
		int nMinDamage, nMaxDamage, nEnhance;
		int nDamageMinBase = Item[nWeaponIdx].m_aryBaseAttrib[0].nValue[0];
		int	nDamageMaxBase = Item[nWeaponIdx].m_aryBaseAttrib[1].nValue[0];
		nMinDamage = 0;
		nMaxDamage = 0;
		nEnhance = 0;
		for (int i = 0; i < 6; i++)
		{
			switch(Item[nWeaponIdx].m_aryMagicAttrib[i].nAttribType)
			{
			case magic_weapondamagemin_v:
				nMinDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamagemax_v:
				nMaxDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamageenhance_p:
				nEnhance += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			default:
				break;
			}
		}
		*nMin = (nDamageMinBase + nMinDamage) * (100 + nEnhance) / 100;
		*nMax = (nDamageMaxBase + nMaxDamage) * (100 + nEnhance) / 100;
	}
	else
	{
		_ASSERT(STRENGTH_SET_DAMAGE_VALUE > 0);
		*nMin = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
		*nMax = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	}
}

// --
//
// --
int KItemList::Add(int nIdx, int nPlace, int nX, int nY)
{
	// printf("vao ham add : %d %d %d %d \n",nIdx, nPlace, nX, nY);
	if( IsMyItem(nIdx) )
		return 0;

	const int i = FindFree();
	if (!i)
		return 0;

	switch(nPlace)
	{
	case pos_hand:
		if (m_Hand)
			return 0;
		m_Items[i].nPlace = pos_hand;
		m_Items[i].nX = 0;
		m_Items[i].nY = 0;
		m_Hand = nIdx;
		break;
	case pos_equip:
		if (nX < 0 || nX >= itempart_num)
			return 0;
		if (m_EquipItem[nX])
			return 0;

		m_Items[i].nPlace = pos_equip;
		m_Items[i].nX = nX;
		m_Items[i].nY = 0;
		break;
	case pos_equiproom:
		if (!m_Room[room_equipment].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_equiproom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
#ifndef _SERVER
	case pos_trade1:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
			return 0;
		if (!m_Room[room_trade1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_trade1;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
#endif
	case pos_repositoryroom:
		if (!m_Room[room_repository].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_immediacy:
		if (!m_Room[room_immediacy].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_immediacy;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_builditem://TrembleItem by kinnox;
		if (nX < 0 || nX >= MAX_PART_BUILD)
			return 0;
		if (m_BuildItem[nX])
			return 0;
		m_Items[i].nPlace = pos_builditem;
		m_Items[i].nX = nX;
		m_Items[i].nY = 0;
		break;
	//load 3 storebox by kinnox;
	case pos_repositoryroom1:
		if (!m_Room[room_repository1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom1;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_repositoryroom2:
		if (!m_Room[room_repository2].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom2;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_repositoryroom3:
		if (!m_Room[room_repository3].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom3;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_repositoryroom4:
		if (!m_Room[room_repository4].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom4;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_repositoryroom5:
		if (!m_Room[room_repository5].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom5;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_expandtoryroom1://Expandbox by kinnox;
		if (!m_Room[room_expandtory1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_expandtoryroom1;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;		
	case pos_givebox://GiveBox by kinnox;
		if (!m_Room[room_givebox].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_givebox;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;		
	//end code		
	default:
		return 0;
	}

	m_Items[i].nIdx = nIdx;
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);

	if(m_Items[i].nPlace == pos_equip)
	{
		Equip(m_Items[i].nIdx, nX);
	}
	if (m_Items[i].nPlace == pos_builditem)//TrembleItem by kinnox;
	{
		BuildItem(m_Items[i].nIdx, nX);
	}
#ifdef _SERVER
	ITEM_SYNC	sItem;
	sItem.ProtocolType	= s2c_syncitem;
	sItem.m_ID			= Item[nIdx].GetID();
	sItem.m_Genre		= Item[nIdx].GetGenre();
	sItem.m_Detail		= Item[nIdx].GetDetailType();
	sItem.m_Level		= Item[nIdx].GetLevel();
	sItem.m_Series		= Item[nIdx].GetSeries();
	sItem.m_Luck		= Item[nIdx].m_GeneratorParam.nLuck;
	sItem.m_Place		= m_Items[i].nPlace;
	sItem.m_X			= m_Items[i].nX;
	sItem.m_Y			= m_Items[i].nY;
	int j = 0;
	for(j = 0; j < 6; j++)
		sItem.m_MagicLevel[j] = (BYTE)Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];

	sItem.m_RandomSeed	= Item[nIdx].m_GeneratorParam.dwRandomSeed;
	sItem.m_Version		= Item[nIdx].m_GeneratorParam.nVersion;
	sItem.m_Durability	= Item[nIdx].GetDurability();
	//
	sItem.m_Record		= Item[nIdx].GetRecord();
	//
	sItem.m_ExpiredTime	= Item[nIdx].GetTime();
	//
	sItem.m_BindState	= Item[nIdx].GetBindState();
	sItem.m_ShopPrice	= Item[nIdx].GetPlayerShopPrice();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));
#endif

#ifndef _SERVER
	KUiObjAtContRegion	pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK,
		UIEP_MANTLE,
		UIEP_SIGNET,
		UIEP_SHIPIN,
	};

	int PartBuildItem[MAX_PART_BUILD] = //TrembleItem by kinnox;
	{
		UIEP_BUILDITEM1,
		UIEP_BUILDITEM2,
		UIEP_BUILDITEM3,
		UIEP_BUILDITEM4,
		UIEP_BUILDITEM5,
		UIEP_BUILDITEM6,
		UIEP_BUILDITEM7,
		UIEP_BUILDITEM8,
		UIEP_BUILDITEM9,
	};

	pInfo.Obj.uGenre = CGOG_ITEM;
	pInfo.Obj.uId = nIdx;
	pInfo.Region.Width = Item[nIdx].GetWidth();
	pInfo.Region.Height = Item[nIdx].GetHeight();

	switch(nPlace)
	{
	case pos_immediacy:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_builditem://TrembleItem by kinnox;
		pInfo.Region.h = 0;
		pInfo.Region.v = PartBuildItem[nX];
		pInfo.eContainer = UOC_BUILD_ITEM;
		break;		
	case pos_hand:
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;
	case pos_equiproom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;
	//load 3 storebox by kinnox;
	case pos_repositoryroom1:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX1;
		break;
	case pos_repositoryroom2:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX2;
		break;
	case pos_repositoryroom3:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX3;
		break;
	case pos_repositoryroom4:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX4;
		break;
	case pos_repositoryroom5:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX5;
		break;
	case pos_expandtoryroom1://Expandbox by kinnox;
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_EXPAND_BOX1;
		break;	
	case pos_givebox: //GiveBox by kinnox;
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_GIVE_BOX;
		break;		
	//end code;			
	case pos_trade1:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		break;
	}
	if (nPlace != pos_trade1)
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 1);
	else
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 1);
	}

#ifdef _DEBUG
	int nLoopIdx = 0;
	nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	//g_DebugLog("[ITEM]Item Begin");
	while(nLoopIdx)
	{
		//g_DebugLog("[ITEM]ItemListIdx:%d, Item:%d, ItemId:%d", nLoopIdx, m_Items[nLoopIdx].nIdx, Item[m_Items[nLoopIdx].nIdx].GetID());
		nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	}
#endif
#endif

	return i;
}

// --
//
// --
BOOL KItemList::Remove(int nGameIdx)
{	
	int nIdx = IsMyItem(nGameIdx);
	if (!nIdx)
		return FALSE;

#ifdef _SERVER
	ITEM_REMOVE_SYNC	sRemove;
	sRemove.ProtocolType = s2c_removeitem;
	sRemove.m_ID = Item[nGameIdx].m_dwID;
#endif

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		m_Hand = 0;
		break;
	case pos_equip:
		UnEquip(m_Items[nIdx].nIdx);
		break;
	case pos_immediacy:
		m_Room[room_immediacy].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_builditem://TrembleItem by kinnox;
		UnBuildItem(m_Items[nIdx].nIdx);
		break;		
	case pos_equiproom:
		m_Room[room_equipment].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom:
		m_Room[room_repository].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	//load 3 storebox by kinnox;
	case pos_repositoryroom1:
		m_Room[room_repository1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom2:
		m_Room[room_repository2].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom3:
		m_Room[room_repository3].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom4:
		m_Room[room_repository4].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom5:
		m_Room[room_repository5].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_expandtoryroom1://Expandbox by kinnox;
		m_Room[room_expandtory1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;	
	case pos_givebox://GiveBox by kinnox;
		m_Room[room_givebox].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;		
	//end code		
	case pos_traderoom:
		m_Room[room_trade].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
#ifndef _SERVER
	case pos_trade1:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			_ASSERT(0);
		}
		m_Room[room_trade1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
#endif
	default:
		return FALSE;
	}
#ifndef _SERVER
	ItemSet.Remove(m_Items[nIdx].nIdx);

	KUiObjAtContRegion pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK,
		UIEP_MANTLE,
		UIEP_SIGNET,
		UIEP_SHIPIN,
	};

	int PartBuildItem[MAX_PART_BUILD] = //TrembleItem by kinnox;
	{
		UIEP_BUILDITEM1,
		UIEP_BUILDITEM2,
		UIEP_BUILDITEM3,
		UIEP_BUILDITEM4,
		UIEP_BUILDITEM5,
		UIEP_BUILDITEM6,
		UIEP_BUILDITEM7,
		UIEP_BUILDITEM8,
		UIEP_BUILDITEM9,
	};
	pInfo.Obj.uGenre = CGOG_ITEM;
	pInfo.Obj.uId = m_Items[nIdx].nIdx;
	pInfo.Region.Width = Item[m_Items[nIdx].nIdx].GetWidth();
	pInfo.Region.Height = Item[m_Items[nIdx].nIdx].GetHeight();

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		pInfo.Obj.uGenre = CGOG_NOTHING;
		pInfo.Region.h = 0;
		pInfo.Region.v = 0;
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equiproom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;
	//load 3 storebox by kinnox;
	case pos_repositoryroom1:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX1;
		break;
	case pos_repositoryroom2:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX2;
		break;
	case pos_repositoryroom3:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX3;
		break;
	case pos_repositoryroom4:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX4;
		break;
	case pos_repositoryroom5:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX5;
		break;
	case pos_expandtoryroom1://ExpandBox by kinnox;
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_EXPAND_BOX1;
		break;		
	case pos_givebox://GiveBox by kinnox;
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_GIVE_BOX;
		break;		
	//end code;		
	case pos_immediacy:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_builditem://Tremble by kinnox;
		pInfo.Region.h = 0;
		pInfo.Region.v = PartBuildItem[m_Items[nIdx].nX];
		pInfo.eContainer = UOC_BUILD_ITEM;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[m_Items[nIdx].nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;
	case pos_trade1:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		break;

	}
	if (m_Items[nIdx].nPlace != pos_trade1)
	{
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 0);
	}
	else
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 0);
	}
#endif
	m_Items[nIdx].nIdx = 0;
	m_Items[nIdx].nPlace = 0;
	m_Items[nIdx].nX = 0;
	m_Items[nIdx].nY = 0;
	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);

#ifdef _SERVER
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
#endif
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[ItemList.RemoveS] ItemListIdx: %d - ItemIdx: %d - ItemSetCount: %d", nIdx, nGameIdx, ItemSet.GetItemNumber());
#else
	//g_DebugLog("[ItemList.RemoveC] ItemListIdx: %d - ItemIdx: %d - ItemSetCount: %d", nIdx, nGameIdx, ItemSet.GetItemNumber());
#endif
#endif
	return TRUE;
}

// --
//
// --
int KItemList::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

// --
//
// --
int KItemList::FindSame(int nGameIdx)
{
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;

		if (m_Items[nIdx].nIdx == nGameIdx)
			return nIdx;
	}
	return 0;
}

// --
//
// --
BOOL KItemList::Init(const int nPlayerIdx)
{
	m_PlayerIdx = nPlayerIdx;
	m_Hand = 0;
	m_nBackHand = 0;
	m_Room[room_equipment].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_repository].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	//load 3 storebox by kinnox;
	m_Room[room_repository1].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_repository2].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_repository3].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_repository4].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_repository5].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_expandtory1].Init(EXPANDTORY_ROOM_WIDTH, EXPANDTORY_ROOM_HEIGHT);//Expandbox by kinnox;
	m_Room[room_givebox].Init(GIVEBOX_ROOM_WIDTH, GIVEBOX_ROOM_HEIGHT);//GiveBox by kinnox;	
	//end code;	
	m_Room[room_trade].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);
	m_Room[room_tradeback].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_immediacy].Init(IMMEDIACY_ROOM_WIDTH, IMMEDIACY_ROOM_HEIGHT);

#ifndef _SERVER
	m_Room[room_trade1].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);
#endif

	ZeroMemory(m_EquipItem, sizeof(m_EquipItem));
	ZeroMemory(m_Items, sizeof(m_Items));
	ZeroMemory(m_sBackItems, sizeof(m_sBackItems));
	m_nListCurIdx = 0;	
	
	m_FreeIdx.Init(MAX_PLAYER_ITEM);
	m_UseIdx.Init(MAX_PLAYER_ITEM);
	
	int i;
	for(i = MAX_PLAYER_ITEM - 1; i > 0 ; i--)
	{
		m_FreeIdx.Insert(i);
	}
	m_bActiveSet		= FALSE;
	m_bMaskLock			= FALSE;
	//	
	if(m_GoldInfo)
	{
		for(i = 0; i < itempart_horse; i++)
			delete [] m_GoldInfo[i];
		delete []m_GoldInfo;
		m_GoldInfo = NULL;
	}

	m_GoldInfo = (int**)new int*[itempart_horse];
	for(i = 0; i < itempart_horse; i++)
	{
		m_GoldInfo[i] = NULL;
		m_GoldInfo[i] = (int*)new int[3];
		UpdateGoldInfo(i,0,0,0);

	}

	return TRUE;
}

// --
//
// --
void KItemList::Release()
{
	m_Hand = 0;
	m_nBackHand = 0;
	//
	m_Room[room_equipment].Clear();
	m_Room[room_repository].Clear();
	m_Room[room_repository1].Clear();
	m_Room[room_repository2].Clear();
	m_Room[room_repository3].Clear();
	m_Room[room_repository4].Clear();
	m_Room[room_repository5].Clear();
	m_Room[room_expandtory1].Clear();//ExpandBox by kinnox;	
	m_Room[room_givebox].Clear();//GiveBox by kinnox;	
	m_Room[room_trade].Clear();
	m_Room[room_tradeback].Clear();
	m_Room[room_immediacy].Clear();
	

#ifndef _SERVER
	m_Room[room_trade1].Clear();
#endif
	//
	ZeroMemory(m_EquipItem, sizeof(m_EquipItem));
	ZeroMemory(m_Items, sizeof(m_Items));
	ZeroMemory(m_sBackItems, sizeof(m_sBackItems));	
	//
	//Thang chat GPT no keu reset by kinnox 24/03/2024;
	ZeroMemory(m_BuildItem, sizeof(m_BuildItem));
	//
	m_nListCurIdx		= 0;
	//
	int i;
	//
	for(i = 0; i < itempart_horse; i++)
	{
		UpdateGoldInfo(i,0,0,0);
	}
	m_bActiveSet		= FALSE;
	m_bMaskLock			= FALSE;	
}	

// --
//
// --
BOOL KItemList::CanEquip(int nIdx, int nPlace)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return FALSE;
	if( (nIdx <= 0) || (nIdx >= MAX_ITEM) )
		return FALSE;
	if( (Item[nIdx].GetGenre() < item_equip) || (Item[nIdx].GetGenre() > item_platinaequip) )
		return FALSE;

	if( (nPlace != -1) && !Fit(nIdx, nPlace))
		return FALSE;
	
	KMagicAttrib* pData = NULL;
	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)Item[nIdx].GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} 
	while (pData != NULL);

	return TRUE;
}

// --
//
// --
BOOL KItemList::CanEquip(KItem* pItem, int nPlace /* = -1 */)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return FALSE;
	if (!pItem)
		return FALSE;

	if( (nPlace != -1) && !Fit(pItem, nPlace))
		return FALSE;

	KMagicAttrib* pData = NULL;
	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)pItem->GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} 
	while (pData != NULL);

	return TRUE;
}

// --
//
// --
BOOL KItemList::EnoughAttrib(void* pAttrib)
{
	KMagicAttrib*	pData = (KMagicAttrib *)pAttrib;
	_ASSERT(pData);
	switch(pData->nAttribType)
	{
	case magic_requirestr:
		if (Player[m_PlayerIdx].m_nCurStrength < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiredex:
		if (Player[m_PlayerIdx].m_nCurDexterity < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirevit:
		if (Player[m_PlayerIdx].m_nCurVitality < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireeng:
		if (Player[m_PlayerIdx].m_nCurEngergy < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirelevel:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Level < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiremenpai:
		if (Player[m_PlayerIdx].m_cFaction.m_nCurFaction != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireseries:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Series != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiresex:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_nSex != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_require_fortune_value:
		if (Player[m_PlayerIdx].m_nFortunePoint < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}
// --
//AutoAI by kinnox;
// --
int KItemList::HaveDamageItem(int nDur)
{
	int nIndex = 0;
	for (int i = 0; i < itempart_num; i ++)
	{
		nIndex = GetEquipment(i);
		/* // sua theo %
		if (Item[nIndex].GetDurability() >= 0 && 
			Item[nIndex].GetMaxDurability() > 0
		{
			int DuraPercent = Item[nIndex].GetDurability() * Item[nIndex].GetMaxDurability()/100 ;
			if (DuraPercent < nDur)
			{
				return nIndex;
			}
		}
		*/
		// sua theo diem
		/*if (Item[nIndex].GetDurability() >= 0 && 
			Item[nIndex].GetMaxDurability() > 0 && 
			(Item[nIndex].GetMaxDurability() - Item[nIndex].GetDurability() >= nDur))
		{
			return nIndex;
		}*/
		// sua theo diem thap nhat
		if (Item[nIndex].GetDurability() >= 0 && 
			Item[nIndex].GetMaxDurability() > 0 && 
			Item[nIndex].GetDurability() <= nDur)
		{
			return nIndex;
		}
		
	}
	return 0;
}
// --
//
// --
BOOL KItemList::Equip(int nIdx, int nPlace /* = -1 */)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return FALSE;

	const int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return FALSE;

	if( (nIdx <= 0) || (nIdx >= MAX_ITEM) )
		return FALSE;

	if( (Item[nIdx].GetGenre() < item_equip) || (Item[nIdx].GetGenre() > item_platinaequip) )
		return FALSE;

	int nItemListIdx = IsMyItem(nIdx);
	if(!nItemListIdx)
		return FALSE;

	int nEquipPlace = nPlace;
	if (-1 == nEquipPlace)
	{
		nEquipPlace = GetEquipPlace(Item[nIdx].GetDetailType());
	}
	else if (!Fit(nIdx, nEquipPlace))
	{
		return FALSE;
	}
	switch(nEquipPlace)
	{
	case itempart_head:
		{	
			if(Item[nIdx].GetGenre() == item_goldequip)
			{
				Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetGoldEquipRes( Item[nIdx].GetRecord() );
			}
			else
			{
				Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes( Item[nIdx].GetRecord() );
			}
		}
		break;
	case itempart_body:
		{	
			if(Item[nIdx].GetGenre() == item_goldequip)
			{
				Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetGoldEquipRes( Item[nIdx].GetRecord() );
			}
			else
			{
				Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes( Item[nIdx].GetRecord() );
			}
		}
		break;
	case itempart_weapon:
		{
			if(Item[nIdx].GetGenre() == item_goldequip)
			{
				Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetGoldEquipRes( Item[nIdx].GetRecord() );
			}
			else
			{
				Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes( Item[nIdx].GetDetailType(), Item[nIdx].GetRecord() );
			}
		}
		break;
	case itempart_horse:
		if(Item[nIdx].GetGenre() == item_goldequip)
		{
			Npc[nNpcIdx].m_HorseType = g_ItemChangeRes.GetGoldEquipRes( Item[nIdx].GetRecord() );
		}
		else
		{
			Npc[nNpcIdx].m_HorseType = g_ItemChangeRes.GetHorseRes( Item[nIdx].GetRecord() );
		}
		if (Npc[nNpcIdx].m_HorseType >= 0)
		{
			Npc[nNpcIdx].m_bRideHorse = TRUE;
		}
		else
		{
			Npc[nNpcIdx].m_bRideHorse = FALSE;
		}
		break;
	case itempart_mask:
		if (!m_bMaskLock)
		{
			// FIX: All masks (regular AND gold equip) use GetBaseMagic()
			// GetBaseMagic() reads BasicProp1PAMin which contains the mask NPC ID (e.g., 1310, 1311, 1314)
			// GetGoldEquipRes() is for armor/weapons, not masks (GoldEquipRes.txt has ResType=0 for masks)
			Npc[nNpcIdx].m_wMaskType = (WORD)Item[nIdx].GetBaseMagic();
	
		}
		break;
	case itempart_mantle:
		Npc[nNpcIdx].m_byMantleLevel = Item[nIdx].GetLevel();
		if(Item[nIdx].GetGenre() == item_goldequip)
		{
			Npc[nNpcIdx].m_MantleType = g_ItemChangeRes.GetGoldEquipRes( Item[nIdx].GetRecord() );
		}
		else
		{
			Npc[nNpcIdx].m_MantleType = 0;
		}
		break;
	default:
		break;
	}

	if(Item[nIdx].GetGenre() == item_goldequip && nEquipPlace < itempart_horse)
		UpdateGoldInfo(nEquipPlace, Item[nIdx].GetSuiteNo(), Item[nIdx].GetSuiteId(), Item[nIdx].GetSuiteCount());



	m_EquipItem[nEquipPlace] = nIdx;
	m_Items[nItemListIdx].nPlace = pos_equip;
	m_Items[nItemListIdx].nX = nEquipPlace;
	m_Items[nItemListIdx].nY = 0;
	//
	int nActive = GetEquipEnhance(nEquipPlace);
	Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], nActive);
	InfectionNextEquip(nEquipPlace, TRUE);

	if (itempart_weapon == nEquipPlace)
	{		
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}
	//
	if(nEquipPlace < itempart_horse)
		m_bActiveSet = GetIfActive();

	Player[m_PlayerIdx].UpdataCurData();
		// Notify UI to refresh skill display when item bonuses change
#ifndef _SERVER
	CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[m_PlayerIdx].m_nSkillPoint);
#endif
	return TRUE;
}

// --
//
// --
void KItemList::InfectionNextEquip(int nEquipPlace, BOOL bEquip/* = FALSE */)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return;

	const int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return;
	
	int i;
	int nInfectionEquip, nActive;
	for (i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		nInfectionEquip = ms_ActiveEquip[nEquipPlace][i];
		if (!m_EquipItem[nInfectionEquip])
			continue;
		if (g_IsAccrue(Item[m_EquipItem[nEquipPlace]].GetSeries(),
			Item[m_EquipItem[nInfectionEquip]].GetSeries()))
		{
			nActive = GetEquipEnhance(nInfectionEquip);
			if (bEquip)
				Item[m_EquipItem[nInfectionEquip]].ApplyHiddenMagicAttribToNPC(&Npc[nNpcIdx], nActive);
			else
				Item[m_EquipItem[nInfectionEquip]].RemoveHiddenMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
		}
	}
}

// --
//
// --
BOOL KItemList::UnEquip(int nIdx, int nPos/* = -1*/)
{
	
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return FALSE;

	const int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return FALSE;

	if (!IsMyItem(nIdx))
		return FALSE;
	
	int i = 0;
	if (nPos <= 0)
	{
		for (i = 0; i < itempart_num; i++)
		{
			if (m_EquipItem[i] == nIdx)
			{
				break;
			}
		}
		if (i == itempart_num)
			return FALSE;

	}
	else
	{
		if (m_EquipItem[nPos] != nIdx)
			return FALSE;
		i = nPos;
	}
	

	if(i == itempart_horse)
	{
		if (Npc[nNpcIdx].m_bRideHorse)
		{	
			Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], 3);
			InfectionNextEquip(i, FALSE);
			
		}
	}
	else
	{
		int nActive = GetEquipEnhance(i);
		if (m_bActiveSet)
			nActive = 3;
		Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
		InfectionNextEquip(i, FALSE);
	}
	m_EquipItem[i] = 0;

	switch(i)
	{
	case itempart_head:
		Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes(-1);
		break;
	case itempart_body:
		Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes(-1);
		break;
	case itempart_weapon:
		Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes(0, -1);
		break;
	case itempart_horse:
		Npc[nNpcIdx].m_HorseType = -1;
		Npc[nNpcIdx].m_bRideHorse = FALSE;
		break;
	case itempart_mask:
		if (!m_bMaskLock)
			Npc[nNpcIdx].m_wMaskType = 0;
		break;
	case itempart_mantle:
		Npc[nNpcIdx].m_byMantleLevel = 0;
		Npc[nNpcIdx].m_MantleType = -1;
		break;
	default:
		break;
	}
	
	if(i < itempart_horse)
		UpdateGoldInfo(i, 0, 0, 0);

	if (itempart_weapon == i)
	{
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}
	if(i < itempart_horse)
		m_bActiveSet = GetIfActive();

	if(!m_bActiveSet)
		{
		Player[m_PlayerIdx].UpdataCurData();
		// Notify UI to refresh skill display when item bonuses change

#ifndef _SERVER
		CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[m_PlayerIdx].m_nSkillPoint);
#endif
	}
	return TRUE;
}

// --
//
// --
int KItemList::GetEquipPlace(int nType)
{
	int nRet = -1;
	switch(nType)
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		nRet = itempart_weapon;
		break;
	case equip_armor:
		nRet = itempart_body;
		break;
	case equip_helm:
		nRet = itempart_head;
		break;
	case equip_boots:
		nRet = itempart_foot;
		break;
	case equip_ring:
		nRet = itempart_ring1;
		break;
	case equip_amulet:
		nRet = itempart_amulet;
		break;
	case equip_belt:
		nRet = itempart_belt;
		break;
	case equip_cuff:
		nRet = itempart_cuff;
		break;
	case equip_pendant:
		nRet = itempart_pendant;
		break;
	case equip_horse:
		nRet = itempart_horse;
		break;
	case equip_mask:
		nRet = itempart_mask;
		break;
	case equip_mantle:
		nRet = itempart_mantle;
		break;
	case equip_signet:
		nRet = itempart_signet;
		break;
	case equip_shipin:
		nRet = itempart_shipin;
		break;
	default:
		break;
	}
	return nRet;
}

// --
//
// --
BOOL KItemList::Fit(int nIdx, int nPlace)
{
	BOOL	bRet = FALSE;
	switch(Item[nIdx].GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask:
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;
	case equip_mantle:
		if (nPlace == itempart_mantle)
			bRet = TRUE;
		break;
	case equip_signet:
		if (nPlace == itempart_signet)
			bRet = TRUE;
		break;
	case equip_shipin:
		if (nPlace == itempart_shipin)
			bRet = TRUE;
		break;	
	}
	return bRet;
}

// --
//
// --
BOOL KItemList::Fit(KItem* pItem, int nPlace)
{
	BOOL	bRet = FALSE;
	switch(pItem->GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask:
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;
	case equip_mantle:
		if (nPlace == itempart_mantle)
			bRet = TRUE;
		break;
	case equip_signet:
		if (nPlace == itempart_signet)
			bRet = TRUE;
		break;
	case equip_shipin:
		if (nPlace == itempart_shipin)
			bRet = TRUE;
		break;
	}
	return bRet;
}

// --
//
// --
int KItemList::GetEquipEnhance(int nPlace)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return 0;

	const int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return 0;

	if( (nPlace < 0) || (nPlace >= itempart_num) )
		return 0;

	int nRet = 0;
	int nSeries = Item[m_EquipItem[nPlace]].GetSeries();
	if( g_IsAccrue(Npc[nNpcIdx].m_Series, nSeries) )
		nRet++;

	int i, nActivedEquip;
	for(i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		nActivedEquip = m_EquipItem[ms_ActivedEquip[nPlace][i]];
		if (nActivedEquip)
		{
			if (g_IsAccrue(Item[nActivedEquip].GetSeries(), nSeries))
				nRet++;
		}
	}
	return nRet;
}


// --
//
// --
BOOL KItemList::EatMecidine(const int nGameIdx)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return FALSE;

	const int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if( (nNpcIdx <= 0) || (nNpcIdx >= MAX_NPC) )
		return FALSE;

	if(Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	
	if( !IsMyItem(nGameIdx) )
		return FALSE;

	const int nGenre = Item[nGameIdx].GetGenre();
	//
	if(nGenre == item_medicine)
	{
		Item[nGameIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], 3);

#ifdef _SERVER
		Remove(nGameIdx);
		ItemSet.Remove(nGameIdx);
#endif
		return TRUE;
	}
	//
	if(nGenre == item_script)
	{
		BOOL oReturn = FALSE;
		if( Item[nGameIdx].GetScriptID() )
		{
#ifdef _SERVER
			oReturn = Player[m_PlayerIdx].ExecuteScriptRet(Item[nGameIdx].GetScriptID(), "OnUse", nGameIdx);
			//
			if(Item[nGameIdx].GetDetailType() != 1)
			{
				char szData[128];
				//
				Item[nGameIdx].GetItemBackupInfo(szData);
				//
//				Player[m_PlayerIdx].SaveLog(2, szData, "USE ITEM", Item[nGameIdx].GetName());
			}
#else
			oReturn = TRUE;
#endif
		}

#ifdef _SERVER
		if(!oReturn)
			return TRUE;
#else
		return oReturn;
#endif

#ifdef _SERVER
		if( Item[nGameIdx].CanStack() )
		{
			this->UseStackItem(nGameIdx);
		}
		else
		{
			Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
		}
		
		return TRUE;
#endif
		
	}
	return FALSE;
}

#ifndef _SERVER
// --
//
// --
int KItemList::UseItem(int nIdx, BYTE btPos /* = 0*/)
{
	if( (m_PlayerIdx <= 0) || (m_PlayerIdx >= MAX_PLAYER) )
		return 0;

	if( !IsMyItem(nIdx) )
		return 0;

	int	nRet = 0;
	switch(Item[nIdx].GetGenre())
	{
	case item_equip:
	case item_purpleequip:
	case item_goldequip:
	case item_platinaequip:
		if (btPos > 0)
		{
			switch(btPos)
			{
				case pos_equiproom:
					nRet = REQUEST_EQUIP_ITEM;
					break;
				case pos_equip:
					nRet = REQUEST_EQUIP_ITEM_TO_EQUIPROOM;
					break;
				default:
				nRet = REQUEST_EQUIP_ITEM;
				break;
			}
		}
		else
		{
			nRet = REQUEST_EQUIP_ITEM;
		}
		break;
	case item_medicine:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	case item_script:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	default:
		nRet = REQUEST_EAT_OTHER;
		break;
	}

	return nRet;
}

int KItemList::SearchPlaceEquip(int nItemID, BYTE nPos)
{
	int nRet = 0;
	if (nItemID <= 0 || nItemID > MAX_ITEM || nPos <= 0)
		return nRet;
	switch(Item[nItemID].GetDetailType())
	{
		case equip_meleeweapon:
			nRet = itempart_weapon;
			break;	
		case equip_rangeweapon:
			nRet = itempart_weapon;
			break;
		case equip_armor:
			nRet = itempart_body;
			break;	
		case equip_ring:
			switch(nPos)
			{
				case pos_equip: 
					if (GetEquipment(itempart_ring1) <= 0 || GetEquipment(itempart_ring2) > 0)
						nRet = itempart_ring1;
					else if(GetEquipment(itempart_ring2) <=0 ||GetEquipment(itempart_ring1) > 0)
						nRet = itempart_ring2;
					break;
				case pos_equiproom:
					if (GetEquipment(itempart_ring1) > 0 && GetEquipment(itempart_ring2) > 0 || 
						(GetEquipment(itempart_ring2) <=0 || GetEquipment(itempart_ring1) > 0))
						nRet = itempart_ring1;
					else if (GetEquipment(itempart_ring1) <= 0 || GetEquipment(itempart_ring2) > 0)
						nRet = itempart_ring2;
					break;	
				default:
					nRet = itempart_ring1;
				break;
			}
			break;	
		case equip_amulet:
			nRet = itempart_amulet;
			break;	
		case equip_boots:
			nRet = itempart_foot;
			break;	
		case equip_belt:
			nRet = itempart_belt;
			break;	
		case equip_helm:
			nRet = itempart_head;
			break;
		case equip_cuff:
			nRet = itempart_cuff;
			break;
		case equip_pendant:
			nRet = itempart_pendant;
			break;	
		case equip_horse:
			nRet = itempart_horse;
			break;
		case equip_mask:
			nRet = itempart_mask;
			break;	
		case equip_mantle:
			nRet = itempart_mantle;
			break;
		case equip_signet:
			nRet = itempart_signet;
			break;
		case equip_shipin:
			nRet = itempart_shipin;
			break;				
		default:
			nRet = 0;
		break;
	}
	return nRet;
} 
#endif
BOOL KItemList::SearchPosition(int nWidth, int nHeight, ItemPos* pPos)
{
	if (nWidth <= 0 || nHeight <= 0 || NULL == pPos)
	{
		return FALSE;
	}

	POINT	pPt;
	if(!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{

		if (0 != m_Hand)
		{
			return FALSE;
		}
		pPos->nPlace = pos_hand;
		pPos->nX = 0;
		pPos->nY = 0;
	}
	else
	{
		pPos->nPlace = pos_equiproom;
		pPos->nX = pPt.x;
		pPos->nY = pPt.y;
	}
	return TRUE;
}

// --
//
// --
int	KItemList::SearchID(int nID)
{
	if (m_PlayerIdx <= 0)
		return 0;
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		if (Item[m_Items[nIdx].nIdx].GetID() == (DWORD)nID)
			return m_Items[nIdx].nIdx;
	}
	return 0;
}

// --
//
// --
void KItemList::ExchangeMoney(int pos1, int pos2, int nMoney)
{
	if (pos1 < 0 || pos2 < 0 || pos1 > room_trade || pos2 > room_trade)
		return;
	
	if (Player[m_PlayerIdx].m_bLockPlayer && (pos1 == room_repository && pos2 == room_equipment || pos1 == room_equipment && pos2 == room_repository))//LockPlayer by kinnox;
	   return;
			
	if (m_Room[pos1].AddMoney(-nMoney))
	{
		if (!m_Room[pos2].AddMoney(nMoney))
		{
			m_Room[pos1].AddMoney(nMoney);
		}
	}
	else
	{
		return;
	}

#ifndef _SERVER
	if (pos1 == room_equipment && pos2 == room_repository)
		SendClientCmdStoreMoney(0, nMoney);
	else if (pos1 == room_repository && pos2 == room_equipment)
		SendClientCmdStoreMoney(1, nMoney);
#endif
#ifdef _SERVER
	SendMoneySync();
#endif	
}

int KItemList::GetMoneyAmount()
{
	return (m_Room[room_equipment].GetMoney() + m_Room[room_repository].GetMoney());
}

int KItemList::GetEquipmentMoney()
{
	return m_Room[room_equipment].GetMoney();
}

int KItemList::GetTradeMoney()//fix trade money by kinnox;
{
	return m_Room[room_trade].GetMoney();
}

int KItemList::GetRepositoryMoney() //Repairitem by kinnox;
{
	return m_Room[room_repository].GetMoney();
}
BOOL KItemList::AddMoney(int nRoom, int nMoney)
{
	if (nRoom < 0 || nRoom >= room_num)
		return FALSE;

	if ( !m_Room[nRoom].AddMoney(nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

// --
//
// --
BOOL KItemList::CostMoney(int nMoney)
{
	if (nMoney > GetEquipmentMoney())
		return FALSE;

	if ( !m_Room[room_equipment].AddMoney(-nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

// --
//
// --
BOOL KItemList::DecMoney(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (nMoney > m_Room[room_equipment].GetMoney())
	{
		nMoney -= m_Room[room_equipment].GetMoney();
		SetRoomMoney(room_equipment, 0);
		if (nMoney > m_Room[room_repository].GetMoney())
			SetRoomMoney(room_repository, 0);
		else
			AddMoney(room_repository, -nMoney);
	}
	else
	{
		AddMoney(room_equipment, -nMoney);
	}

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

#ifdef _SERVER
void	KItemList::TradeMoveMoney(int nMoney)
{
	m_Room[room_trade].SetMoney(nMoney);
	SendMoneySync();

	TRADE_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_trademoneysync;
	sMoney.m_nMoney = nMoney;
	g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(TRADE_MONEY_SYNC));
}
#endif

#ifdef _SERVER
void	KItemList::SendMoneySync()
{
	PLAYER_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_syncmoney;
	sMoney.m_nMoney1 = m_Room[room_equipment].GetMoney();
	sMoney.m_nMoney2 = m_Room[room_repository].GetMoney();
	sMoney.m_nMoney3 = m_Room[room_trade].GetMoney();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(PLAYER_MONEY_SYNC));
}
#endif

// --
//
// --
void KItemList::SetMoney(int nMoney1, int nMoney2, int nMoney3)
{
	m_Room[room_equipment].SetMoney(nMoney1);
	m_Room[room_repository].SetMoney(nMoney2);
	m_Room[room_trade].SetMoney(nMoney3);
#ifndef _SERVER
	KUiObjAtContRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = nMoney2;
	sMoney.eContainer = UOC_STORE_BOX;
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sMoney, 1);
#endif
}

// --
//
// --
void KItemList::SetRoomMoney(int nRoom, int nMoney)
{
	if (nRoom >= 0 && nRoom < room_num)
		m_Room[nRoom].SetMoney(nMoney);
}

// --
//
// --
void KItemList::ExchangeItem(ItemPos* SrcPos, ItemPos* DesPos)
{
	if (SrcPos->nPlace != DesPos->nPlace)
		return;
	int nTempHand	= m_Hand;
	int	nEquipIdx1	= 0;
	int nListIdx	= 0;

#ifdef _SERVER

	if (Player[m_PlayerIdx].m_bLockPlayer)//LockPlayer by kinnox;
		return;

	PLAYER_MOVE_ITEM_SYNC	sMove;
	sMove.ProtocolType = s2c_playermoveitem;
	sMove.m_btDownPos = SrcPos->nPlace;
	sMove.m_btDownX = SrcPos->nX;
	sMove.m_btDownY = SrcPos->nY;
	sMove.m_btUpPos = DesPos->nPlace;
	sMove.m_btUpX = DesPos->nX;
	sMove.m_btUpY = DesPos->nY;
#endif
	//load 3 storebox by kinnox;
	if(Npc[Player[m_PlayerIdx].m_nIndex].m_FightMode && DesPos->nPlace >= pos_repositoryroom && DesPos->nPlace <= pos_repositoryroom5)
		return;
	//end code

	switch(SrcPos->nPlace)
	{
	case pos_hand:
		return;
		break;
	case pos_equip:
		if( Player[this->m_PlayerIdx].CheckTrading() )
			return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= itempart_num)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= itempart_num)
			return;
		//
		nEquipIdx1 = m_EquipItem[SrcPos->nX];
		if(nEquipIdx1)
		{
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if (CanEquip(nTempHand, DesPos->nX))
			{	
				
				if (nEquipIdx1)
				{	
					UnEquip(nEquipIdx1, SrcPos->nX);
				}
			
				Equip(nTempHand, DesPos->nX);

				if(nEquipIdx1)
				{	
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
		}
		else
		{	
			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				UnEquip(nEquipIdx1, SrcPos->nX);

				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
		}
		break;
	case pos_immediacy:
		if(Player[m_PlayerIdx].CheckTrading())
			return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_immediacy].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_immediacy].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_immediacy].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_immediacy].m_nHeight)
			return;
		//
		if(nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;

			char cGenre = Item[nTempHand].GetGenre();
			if(cGenre != item_medicine &&
				cGenre != item_script)
				return;
			if(cGenre == item_script)
			{
				if(Item[nTempHand].GetMaxStackCount() > 0)
					return;
			}
		}
		//
		nEquipIdx1 = m_Room[room_immediacy].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_immediacy].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if(nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_immediacy].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_immediacy;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			else
			{	
				m_Room[room_immediacy].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		else
		{	
			

			if(nEquipIdx1)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;
	case pos_repositoryroom:
		//g_DebugLog("VAO DAY MOI TAI CHU");
		if( Player[m_PlayerIdx].CheckTrading() )
		{
			return;
		}

		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository].m_nHeight)
			return;
		//
		nEquipIdx1 = m_Room[room_repository].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_repository].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_repository].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_repositoryroom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
#ifdef _SERVER
			//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#else
			//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#endif
			}
			else
			{	
				m_Room[room_repository].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
			}
		}
		else
		{
			

			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
				
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;
	//load 3 storebox by kinnox;
	case pos_repositoryroom1:
		if( Player[m_PlayerIdx].CheckTrading() )
			{
				return;
			}
			//
			if (m_Hand && Player[m_PlayerIdx].m_btRepositoryCount <= REPOSITORY_ONE)
			return;
			//
			//
			if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository1].m_nWidth)
				return;
			if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository1].m_nHeight)
				return;
			if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository1].m_nWidth)
				return;
			if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository1].m_nHeight)
				return;
			//
			nEquipIdx1 = m_Room[room_repository1].FindItem(SrcPos->nX, SrcPos->nY);
			if(nEquipIdx1)
			{	
				nListIdx = IsMyItem(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				if( !m_Room[room_repository1].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
					return;
			}
			//
			if (nTempHand)
			{	
				nListIdx = IsMyItem(nTempHand);
				if(nListIdx <= 0)
					return;
				
				if( m_Room[room_repository1].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
				{
					m_Items[nListIdx].nPlace = pos_repositoryroom1;
					m_Items[nListIdx].nX = DesPos->nX;
					m_Items[nListIdx].nY = DesPos->nY;
					
					if(nEquipIdx1 > 0)
					{
						nListIdx = FindSame(nEquipIdx1);
						m_Hand = nEquipIdx1;
						m_Items[nListIdx].nPlace = pos_hand;
					}
					else
					{
						m_Hand = 0;
					}
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
	#ifdef _SERVER
				//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#else
				//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#endif
				}
				else
				{	
					m_Room[room_repository1].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
				}
			}
			else
			{
				

				if(nEquipIdx1 > 0)
				{	
					nListIdx = FindSame(nEquipIdx1);
					if(nListIdx <= 0)
						return;
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
					
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
				}

			}
			break;
	//
		case pos_repositoryroom2:
			if( Player[m_PlayerIdx].CheckTrading() )
			{
				return;
			}
			//
			if (m_Hand && Player[m_PlayerIdx].m_btRepositoryCount <= REPOSITORY_TWO)
			return;
			//
			if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository2].m_nWidth)
				return;
			if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository2].m_nHeight)
				return;
			if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository2].m_nWidth)
				return;
			if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository2].m_nHeight)
				return;
			//
			nEquipIdx1 = m_Room[room_repository2].FindItem(SrcPos->nX, SrcPos->nY);
			if(nEquipIdx1)
			{	
				nListIdx = IsMyItem(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				if( !m_Room[room_repository2].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
					return;
			}
			//
			if (nTempHand)
			{	
				nListIdx = IsMyItem(nTempHand);
				if(nListIdx <= 0)
					return;
				
				if( m_Room[room_repository2].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
				{
					m_Items[nListIdx].nPlace = pos_repositoryroom2;
					m_Items[nListIdx].nX = DesPos->nX;
					m_Items[nListIdx].nY = DesPos->nY;
					
					if(nEquipIdx1 > 0)
					{
						nListIdx = FindSame(nEquipIdx1);
						m_Hand = nEquipIdx1;
						m_Items[nListIdx].nPlace = pos_hand;
					}
					else
					{
						m_Hand = 0;
					}
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
	#ifdef _SERVER
				//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#else
				//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#endif
				}
				else
				{	
					m_Room[room_repository2].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
				}
			}
			else
			{
				

				if(nEquipIdx1 > 0)
				{	
					nListIdx = FindSame(nEquipIdx1);
					if(nListIdx <= 0)
						return;
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
					
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
				}

			}
			break;
//
	case pos_repositoryroom3:
		if( Player[m_PlayerIdx].CheckTrading() )
		{
			return;
		}
		//
		if (m_Hand && Player[m_PlayerIdx].m_btRepositoryCount <= REPOSITORY_THREE)
		return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository3].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository3].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository3].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository3].m_nHeight)
			return;
		//
		nEquipIdx1 = m_Room[room_repository3].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_repository3].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_repository3].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_repositoryroom3;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
#ifdef _SERVER
			//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#else
			//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#endif
			}
			else
			{	
				m_Room[room_repository3].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
			}
		}
		else
		{
			

			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
				
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;
//
	case pos_repositoryroom4:
		if( Player[m_PlayerIdx].CheckTrading() )
		{
			return;
		}
		//
		if (m_Hand && Player[m_PlayerIdx].m_btRepositoryCount <= REPOSITORY_FOUR)
		return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository4].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository4].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository4].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository4].m_nHeight)
			return;
		//
		nEquipIdx1 = m_Room[room_repository4].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_repository4].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_repository4].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_repositoryroom4;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
#ifdef _SERVER
			//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#else
			//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#endif
			}
			else
			{	
				m_Room[room_repository4].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
			}
		}
		else
		{
			

			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
				
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;
//
	case pos_repositoryroom5:
		if( Player[m_PlayerIdx].CheckTrading() )
		{
			return;
		}
		//
		if (m_Hand && Player[m_PlayerIdx].m_btRepositoryCount <= REPOSITORY_FIVE)
		return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_repository5].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_repository5].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_repository5].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_repository5].m_nHeight)
			return;
		//
		nEquipIdx1 = m_Room[room_repository5].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_repository5].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_repository5].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_repositoryroom5;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
#ifdef _SERVER
			//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#else
			//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#endif
			}
			else
			{	
				m_Room[room_repository5].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
			}
		}
		else
		{
			

			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
				
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;
	case pos_expandtoryroom1://Expandbox by kinnox;
		if( Player[m_PlayerIdx].CheckTrading() )
		{
			return;
		}
		//
		if (m_Hand && Player[m_PlayerIdx].m_btExpandtoryCount <= EXPANDTORY_ONE)
		return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_expandtory1].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_expandtory1].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_expandtory1].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_expandtory1].m_nHeight)
			return;
		//
		nEquipIdx1 = m_Room[room_expandtory1].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;

			if( !m_Room[room_expandtory1].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;
			
			if( m_Room[room_expandtory1].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_expandtoryroom1;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				
				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
#ifdef _SERVER
			//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#else
			//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
#endif
			}
			else
			{	
				m_Room[room_expandtory1].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
			}
		}
		else
		{
			

			if(nEquipIdx1 > 0)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;
				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
				
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}

		}
		break;	
	case pos_givebox: //GiveBox by kinnox;
			if( Player[m_PlayerIdx].CheckTrading() )
			{
				return;
			}
			if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_givebox].m_nWidth)
				return;
			if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_givebox].m_nHeight)
				return;
			if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_givebox].m_nWidth)
				return;
			if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_givebox].m_nHeight)
				return;
			//
			nEquipIdx1 = m_Room[room_givebox].FindItem(SrcPos->nX, SrcPos->nY);
			if(nEquipIdx1)
			{	
				nListIdx = IsMyItem(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				if( !m_Room[room_givebox].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
					return;
			}
			//
			if (nTempHand)
			{	
				nListIdx = IsMyItem(nTempHand);
				if(nListIdx <= 0)
					return;
			
				if( m_Room[room_givebox].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
				{
					m_Items[nListIdx].nPlace = pos_givebox;
					m_Items[nListIdx].nX = DesPos->nX;
					m_Items[nListIdx].nY = DesPos->nY;
				
					if(nEquipIdx1 > 0)
					{
						nListIdx = FindSame(nEquipIdx1);
						m_Hand = nEquipIdx1;
						m_Items[nListIdx].nPlace = pos_hand;
					}
					else
					{
						m_Hand = 0;
					}
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
	#ifdef _SERVER
				//g_DebugLog("[SERVER]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#else
				//g_DebugLog("[CLIENT]EXREPO PutItem: Idx[%d] Name[%s]", nTempHand, Item[nTempHand].GetName());
	#endif
				}
				else
				{	
					m_Room[room_givebox].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetWidth());
				}
			}
			else
			{
			

				if(nEquipIdx1 > 0)
				{	
					nListIdx = FindSame(nEquipIdx1);
					if(nListIdx <= 0)
						return;
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				
	#ifdef _SERVER
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
	#endif
				}

			}
			break;			
	//end code;
	case pos_equiproom:
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_equipment].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_equipment].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_equipment].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_equipment].m_nHeight)
			return;

		nEquipIdx1 = m_Room[room_equipment].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{	
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;
#ifdef _SERVER
			if(nTempHand)
			{	
				if( ExchangeStack(nEquipIdx1, nTempHand) )
					return;
			}
#endif
			if( !m_Room[room_equipment].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
#ifdef _DEBUG
#ifdef _SERVER
			//g_DebugLog("[SERVER]PickItem: Idx[%d] Name[%s]", nEquipIdx1, Item[nEquipIdx1].GetName());
#else
			//g_DebugLog("[CLIENT]PickItem: Idx[%d] Name[%s]", nEquipIdx1, Item[nEquipIdx1].GetName());
#endif
#endif
		}
		//
		if(nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;

			if( m_Room[room_equipment].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_equiproom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;

				if(nEquipIdx1 > 0)
				{
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			else
			{	
				m_Room[room_equipment].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		else
		{
			if(nEquipIdx1)
			{	
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
		}
		break;
	//
	case pos_builditem: //TrembleItem by kinnox;
		if (Player[this->m_PlayerIdx].CheckTrading())	// »Áπ˚’˝‘⁄Ωª“◊
			return;
		if (SrcPos->nX < 0 || SrcPos->nX >= MAX_PART_BUILD || DesPos->nX < 0 || DesPos->nX >= MAX_PART_BUILD)
			return;

		nEquipIdx1 = m_BuildItem[SrcPos->nX];
		if (m_Hand)
		{
		    if(BuildItem(m_Hand, DesPos->nX) == TRUE)
			{
				if (nEquipIdx1)
				{
					UnBuildItem(nEquipIdx1, SrcPos->nX);
				}
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			else if (nEquipIdx1)
			{
				BuildItem(nEquipIdx1, SrcPos->nX);
			}	
		}
		else
		{
			if (nEquipIdx1)
			{
				UnBuildItem(nEquipIdx1, SrcPos->nX);
			}
			m_Hand = nEquipIdx1;
			m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif		
			
		}
		break;
	//
	case pos_traderoom:
		if( !Player[m_PlayerIdx].CheckTrading() )
			return;
		//
		if(SrcPos->nX < 0 || SrcPos->nX >= m_Room[room_trade].m_nWidth)
			return;
		if(SrcPos->nY < 0 || SrcPos->nY >= m_Room[room_trade].m_nHeight)
			return;
		if(DesPos->nX < 0 || DesPos->nX >= m_Room[room_trade].m_nWidth)
			return;
		if(DesPos->nY < 0 || DesPos->nY >= m_Room[room_trade].m_nHeight)
			return;
		//
#ifdef _SERVER
		if(nTempHand)
		{	
			if(Item[nTempHand].GetBindState() > 0)
				return;
			if(Item[nTempHand].GetGenre() == item_task || Item[nTempHand].GetGenre() == item_script)
			{	
				if(Item[nTempHand].GetParticular() == 0)
					return;
				
			}
		}
#endif
		//
		nEquipIdx1 = m_Room[room_trade].FindItem(SrcPos->nX, SrcPos->nY);
		if(nEquipIdx1)
		{
			nListIdx = IsMyItem(nEquipIdx1);
			if(nListIdx <= 0)
				return;
			if( !m_Room[room_trade].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()) )
				return;
		}
		//
		if (nTempHand)
		{	
			nListIdx = IsMyItem(nTempHand);
			if(nListIdx <= 0)
				return;

			if (Item[nTempHand].GetGenre() == item_medicine)
				return;

			if( m_Room[room_trade].PlaceItem(DesPos->nX, DesPos->nY, nTempHand, Item[nTempHand].GetWidth(), Item[nTempHand].GetHeight()) )
			{
				m_Items[nListIdx].nPlace = pos_traderoom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
#ifdef _SERVER
				if (nEquipIdx1)	
				{
					ITEM_REMOVE_SYNC	sRemove;
					sRemove.ProtocolType = s2c_removeitem;
					sRemove.m_ID = Item[nEquipIdx1].m_dwID;
					g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
				}

				ITEM_SYNC	sItem;
				sItem.ProtocolType = s2c_syncitem;
				sItem.m_ID			= Item[nTempHand].GetID();
				sItem.m_Genre		= Item[nTempHand].GetGenre();
				sItem.m_Detail		= Item[nTempHand].GetDetailType();
				sItem.m_Series		= Item[nTempHand].GetSeries();
				sItem.m_Level		= Item[nTempHand].GetLevel();
				sItem.m_Luck		= Item[nTempHand].m_GeneratorParam.nLuck;
				sItem.m_Place		= pos_trade1;
				sItem.m_X			= DesPos->nX;
				sItem.m_Y			= DesPos->nY;
				int j;
				for(j = 0; j < 6; j++)
					sItem.m_MagicLevel[j] = (BYTE)Item[nTempHand].m_GeneratorParam.nGeneratorLevel[j];
				sItem.m_RandomSeed	= Item[nTempHand].m_GeneratorParam.dwRandomSeed;
				sItem.m_Version		= Item[nTempHand].m_GeneratorParam.nVersion;
				sItem.m_Durability	= Item[nTempHand].GetDurability();
				sItem.m_Record		= Item[nTempHand].GetRecord();
				//
				sItem.m_BindState	= Item[nTempHand].GetBindState();
				sItem.m_ExpiredTime	= Item[nTempHand].GetTime();
				
				sItem.m_ShopPrice	= 0;
				//
				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));

				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				if(nEquipIdx1 > 0)
				{	
					nListIdx = FindSame(nEquipIdx1);
					m_Hand = nEquipIdx1;
					m_Items[nListIdx].nPlace = pos_hand;
				}
				else
				{
					m_Hand = 0;
				}
			}
			else
			{	
				m_Room[room_trade].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		else
		{				
			if(nEquipIdx1 > 0)
			{
				nListIdx = FindSame(nEquipIdx1);
				if(nListIdx <= 0)
					return;

				m_Hand = nEquipIdx1;
				m_Items[nListIdx].nPlace = pos_hand;
#ifdef _SERVER
				ITEM_REMOVE_SYNC	sRemove;
				sRemove.ProtocolType = s2c_removeitem;
				sRemove.m_ID = Item[nEquipIdx1].m_dwID;
				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));

				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
		}
		//
		break;

	}


#ifndef _SERVER
	if (m_Hand != nTempHand)
	{
		KUiObjAtContRegion pInfo1, pInfo2;
		if (nTempHand == 0)
		{
			pInfo2.Obj.uGenre = CGOG_NOTHING;
			pInfo2.Obj.uId = 0;
			pInfo2.Region.Width = 0;
			pInfo2.Region.Height = 0;
		}
		else
		{
			pInfo2.Obj.uGenre = CGOG_ITEM;
			pInfo2.Obj.uId = nTempHand;
			pInfo2.Region.Width = Item[nTempHand].GetWidth();
			pInfo2.Region.Height = Item[nTempHand].GetHeight();
		}
		
		if (m_Hand == 0)
		{
			pInfo1.Obj.uGenre = CGOG_NOTHING;
			pInfo1.Obj.uId = 0;
			pInfo1.Region.Width = 0;
			pInfo1.Region.Height = 0;
		}
		else
		{
			pInfo1.Obj.uGenre = CGOG_ITEM;
			pInfo1.Obj.uId = m_Hand;
			pInfo1.Region.Width = Item[m_Hand].GetWidth();
			pInfo1.Region.Height = Item[m_Hand].GetHeight();
		}

		int PartConvert[itempart_num] = 
		{
			UIEP_HEAD,
			UIEP_BODY,
			UIEP_WAIST,
			UIEP_HAND,
			UIEP_FOOT,
			UIEP_FINESSE,
			UIEP_NECK,
			UIEP_FINGER1,
			UIEP_FINGER2,
			UIEP_WAIST_DECOR,
			UIEP_HORSE,
			UIEP_MASK,
			UIEP_MANTLE,
			UIEP_SIGNET,
			UIEP_SHIPIN,
		};
		
	int PartBuildItem[MAX_PART_BUILD] = //TrembleItem by kinnox;
		{
			UIEP_BUILDITEM1,
			UIEP_BUILDITEM2,
			UIEP_BUILDITEM3,
			UIEP_BUILDITEM4,
			UIEP_BUILDITEM5,
			UIEP_BUILDITEM6,
			UIEP_BUILDITEM7,
			UIEP_BUILDITEM8,
			UIEP_BUILDITEM9,
		};

		switch(SrcPos->nPlace)
		{
		case pos_immediacy:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_IMMEDIA_ITEM;
			pInfo2.eContainer = UOC_IMMEDIA_ITEM;
			break;
		case pos_builditem://TrembleItem by kinnox;
			pInfo1.Region.h = 0;
			pInfo1.Region.v = PartBuildItem[SrcPos->nX];
			pInfo2.Region.h = 0;
			pInfo2.Region.v = PartBuildItem[DesPos->nX];
			pInfo1.eContainer = UOC_BUILD_ITEM;
			pInfo2.eContainer = UOC_BUILD_ITEM;
			break;
		case pos_equiproom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_ITEM_TAKE_WITH;
			pInfo2.eContainer = UOC_ITEM_TAKE_WITH;
			break;
		case pos_equip:
			pInfo1.Region.h = 0;
			pInfo1.Region.v = PartConvert[SrcPos->nX];
			pInfo2.Region.h = 0;
			pInfo2.Region.v = PartConvert[DesPos->nX];
			pInfo1.eContainer = UOC_EQUIPTMENT;
			pInfo2.eContainer = UOC_EQUIPTMENT;
			break;
		case pos_repositoryroom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX;
			pInfo2.eContainer = UOC_STORE_BOX;
			break;
//load 3 storebox by kinnox;
		case pos_repositoryroom1:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX1;
			pInfo2.eContainer = UOC_STORE_BOX1;
			break;
		case pos_repositoryroom2:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX2;
			pInfo2.eContainer = UOC_STORE_BOX2;
			break;
		case pos_repositoryroom3:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX3;
			pInfo2.eContainer = UOC_STORE_BOX3;
			break;
		case pos_repositoryroom4:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX4;
			pInfo2.eContainer = UOC_STORE_BOX4;
			break;
		case pos_repositoryroom5:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX5;
			pInfo2.eContainer = UOC_STORE_BOX5;
			break;
		case pos_expandtoryroom1://Expandbox by kinnox;
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_EXPAND_BOX1;
			pInfo2.eContainer = UOC_EXPAND_BOX1;
			break;			
		case pos_givebox://GiveBox by kinnox;
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_GIVE_BOX;
			pInfo2.eContainer = UOC_GIVE_BOX;
			break;			
		//end code;			
		case pos_traderoom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_TO_BE_TRADE;
			pInfo2.eContainer = UOC_TO_BE_TRADE;
			break;
		}
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo1, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo2, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo1, 1);
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo2, 1);
	}
#endif
	if (Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
}

#ifndef	_SERVER

//	

BOOL	KItemList::AutoMoveItem(ItemPos SrcPos,ItemPos DesPos)
{
	if (Player[this->m_PlayerIdx].CheckTrading())	
		return FALSE;
	if (Npc[Player[this->m_PlayerIdx].m_nIndex].m_bOpenShop)	
		return FALSE;

	BOOL	bMove = FALSE;
	int		nIdx, nListIdx;
	KUiObjAtContRegion sSrcInfo, sDestInfo;

	switch (SrcPos.nPlace)
	{
	case pos_equiproom:
		{
			switch (DesPos.nPlace)
			{
			case pos_immediacy:
				{	
					if(DesPos.nX < 0 || DesPos.nX >= m_Room[room_immediacy].m_nWidth)
								return FALSE;
					if(DesPos.nY < 0 || DesPos.nY >= m_Room[room_immediacy].m_nHeight)
						return FALSE;
					nIdx = m_Room[room_equipment].FindItem(SrcPos.nX, SrcPos.nY);
					if (nIdx <= 0)
						return FALSE;
					BYTE btGenre = Item[nIdx].GetGenre();
					if (btGenre != item_medicine)
						return FALSE;
					if(Item[nIdx].GetWidth() != 1)
						return FALSE;
					if(Item[nIdx].GetHeight() != 1)
						return FALSE;
					if (!m_Room[room_equipment].PickUpItem(nIdx, SrcPos.nX, SrcPos.nY, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
						return FALSE;
					m_Room[room_immediacy].PlaceItem(DesPos.nX, DesPos.nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight());
					nListIdx = FindSame(nIdx);
					if (nListIdx <= 0)
						return FALSE;
					m_Items[nListIdx].nPlace = pos_immediacy;
					m_Items[nListIdx].nX = DesPos.nX;
					m_Items[nListIdx].nY = DesPos.nY;
					bMove = TRUE;

					sSrcInfo.Region.h		= SrcPos.nX;
					sSrcInfo.Region.v		= SrcPos.nY;
					sSrcInfo.eContainer		= UOC_ITEM_TAKE_WITH;
					sDestInfo.eContainer	= UOC_IMMEDIA_ITEM;
				}
				break;
			}
		}
		break;
	}

	if (!bMove)
		return bMove;

	sSrcInfo.Obj.uGenre		= CGOG_ITEM;
	sSrcInfo.Obj.uId		= nIdx;
	sSrcInfo.Region.Width	= Item[nIdx].GetWidth();
	sSrcInfo.Region.Height	= Item[nIdx].GetHeight();

	sDestInfo.Obj.uGenre	= CGOG_ITEM;
	sDestInfo.Obj.uId		= nIdx;
	sDestInfo.Region.Width	= Item[nIdx].GetWidth();
	sDestInfo.Region.Height	= Item[nIdx].GetHeight();
	sDestInfo.Region.h		= DesPos.nX;
	sDestInfo.Region.v		= DesPos.nY;

	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sSrcInfo, 0);
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sDestInfo, 1);
	

	return bMove;
}
#endif

#ifndef	_SERVER

//	

void	KItemList::MenuSetMouseItem()
{
	KUiObjAtContRegion	sInfo;
	if (!m_Hand)
	{
		CoreDataChanged(GDCNI_HOLD_OBJECT, 0, 0);
	}
	else
	{
		sInfo.Obj.uGenre = CGOG_ITEM;
		sInfo.Obj.uId = m_Hand;
		sInfo.Region.Width = Item[m_Hand].GetWidth();
		sInfo.Region.Height = Item[m_Hand].GetHeight();
		sInfo.Region.h = 0;
		sInfo.Region.v = 0;
		sInfo.eContainer = UOC_IN_HAND;
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&sInfo, 0);
	}
}
#endif

#ifdef _SERVER
// --
//
// --
BOOL KItemList::EatMecidine(int nPlace, int nX, int nY)
{
	int nGameIdx = 0;
	switch(nPlace)
	{
	case pos_equiproom:
		{
			if(nX < 0 || nX >= m_Room[room_equipment].m_nWidth)
				return FALSE;
			if(nY < 0 || nY >= m_Room[room_equipment].m_nHeight)
				return FALSE;

			nGameIdx = m_Room[room_equipment].FindItem(nX, nY);
			if( !IsMyItem(nGameIdx) )
				return FALSE;

			const int nGenre = Item[nGameIdx].GetGenre();
			if( (nGenre != item_medicine) &&
				(nGenre != item_script))
				return FALSE;

			return EatMecidine(nGameIdx);
		}
		break;
	case pos_immediacy:
		{
			if(nX < 0 || nX >= m_Room[room_immediacy].m_nWidth)
				return FALSE;
			if(nY < 0 || nY >= m_Room[room_immediacy].m_nHeight)
				return FALSE;
			//
			nGameIdx = m_Room[room_immediacy].FindItem(nX, nY);

			if( !IsMyItem(nGameIdx) )
				return FALSE;
			//
			const int nGenre = Item[nGameIdx].GetGenre();
			if( (nGenre != item_medicine) &&
				(nGenre != item_script))
				return FALSE;

			if(Item[nGameIdx].GetWidth() != 1)
				return FALSE;

			if(Item[nGameIdx].GetHeight() != 1)
				return FALSE;
			
			const int nDetailType = Item[nGameIdx].GetDetailType();
			if(nGenre == item_medicine || ( (nGenre == item_script) && (nDetailType == 1) ) )
			{
				
				int	nNewGameIdx, nPosX, nPosY;

				if( FALSE == FindSameDetailType(room_equipment, nGenre, nDetailType, &nNewGameIdx, &nPosX, &nPosY) )
					return EatMecidine(nGameIdx);

				return EatMecidine(nNewGameIdx);
			}
			
			return EatMecidine(nGameIdx);
		}
		break;
	default:
		break;
	}

	return FALSE;
}
#endif

// --
//
// --
PlayerItem* KItemList::GetFirstItem()
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return &m_Items[m_nListCurIdx];
}

// --
//
// --
PlayerItem* KItemList::GetNextItem()
{
	if ( !m_nListCurIdx )
		return NULL;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return &m_Items[m_nListCurIdx];
}

// --
//
// --
void	KItemList::ClearRoom(int nRoom)
{
	if (nRoom >= 0 && nRoom < room_num)
		this->m_Room[nRoom].Clear();
}

// --
//
// --
void	KItemList::BackupTrade()
{
	if ( !m_Room[room_tradeback].m_pArray )
		m_Room[room_tradeback].Init(m_Room[room_equipment].m_nWidth, m_Room[room_equipment].m_nHeight);
	memcpy(m_Room[room_tradeback].m_pArray, m_Room[room_equipment].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

	memcpy(this->m_sBackItems, this->m_Items, sizeof(PlayerItem) * MAX_PLAYER_ITEM);

	m_nBackHand = m_Hand;
}

// --
//
// --
void	KItemList::RecoverTrade()
{
	memcpy(m_Room[room_equipment].m_pArray, m_Room[room_tradeback].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

#ifndef _SERVER
	int nIdx = 0;
	while((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (m_Items[nIdx].nPlace == pos_trade1)
			Remove(m_Items[nIdx].nIdx);
	}
#endif
	memcpy(m_Items, m_sBackItems, sizeof(PlayerItem) * MAX_PLAYER_ITEM);
	m_Hand = m_nBackHand;
}

// --
//
// --
void KItemList::StartTrade()
{
	BackupTrade();
	ClearRoom(room_trade);
	ClearRoom(room_trade1);
}

void KItemList::RemoveAll()
{
	int nIdx = m_UseIdx.GetNext(0);
	int nGameIdx = 0;
	while(nIdx)
	{
		nGameIdx = m_Items[nIdx].nIdx;
		nIdx = m_UseIdx.GetNext(nIdx);
		if(nGameIdx <= 0)
			continue;	
#ifdef _SERVER
		RemoveByIndex(nGameIdx);
#else
		Remove(nGameIdx);
#endif
	}
}

// --
//
// --
int KItemList::GetWeaponParticular()
{
	if (m_EquipItem[itempart_weapon])
		return Item[m_EquipItem[itempart_weapon]].GetParticular();
	return -1;
}

#ifdef _SERVER
// --
//
// --
BOOL KItemList::IsTaskItemExist(short nDetailType)
{
	int nGameIdx = 0;
	int	nListIdx = m_UseIdx.GetNext(0);
	while(nListIdx)
	{	
		nGameIdx = m_Items[nListIdx].nIdx;
		nListIdx = m_UseIdx.GetNext(nListIdx);

		if(Item[nGameIdx].GetGenre() != item_task)
			continue;

		if(Item[nGameIdx].GetDetailType() == (short)nDetailType)
			return TRUE;
	}
	return FALSE;
}
#endif

#ifdef _SERVER
// --
//	
// --
BOOL KItemList::RemoveByIndex(const int nGameIdx, BOOL bClient /*= FALSE*/)
{	

	if( !IsMyItem(nGameIdx) )
		return FALSE;

	Remove(nGameIdx);
	if(!bClient)
		ItemSet.Remove(nGameIdx);

	return TRUE;
}
#endif

#ifdef _SERVER
// --
//
// --
int KItemList::GetTaskItemNum(const int nDetailType)
{
	int nGameIdx = 0, nNo = 0;
	int	nListIdx = m_UseIdx.GetNext(0);
	while(nListIdx)
	{	
		nGameIdx = m_Items[nListIdx].nIdx;
		nListIdx = m_UseIdx.GetNext(nListIdx);

		if(Item[nGameIdx].GetGenre() != item_task)
			continue;

		if(Item[nGameIdx].GetDetailType() == (short)nDetailType)
		{
			int CountStack = Item[nGameIdx].GetStackCount();
			if (CountStack == 1)
				nNo++;
			else
				nNo += CountStack;
		}
	}
	return nNo;
}
#endif

#ifdef _SERVER
// --
//
// --
BOOL KItemList::RemoveTaskItem(const int nDetailType, int nCount /*= 1*/)
{	
	if(nCount <= 0)
		return FALSE;
	
	int nCurNo = nCount;
	int nTempListIdx = 0, nGameIdx = 0;
	int	nListIdx = m_UseIdx.GetNext(0);

	while(nListIdx)
	{	
		if(nCurNo == 0)
			break;

		nTempListIdx = m_UseIdx.GetNext(nListIdx);
		nGameIdx = m_Items[nListIdx].nIdx;
		//
		nListIdx = nTempListIdx;

		if(Item[nGameIdx].GetGenre() != item_task)
			continue;
		//
		if(Item[nGameIdx].GetDetailType() != (short)nDetailType)
			continue;
		//
		if( Item[nGameIdx].CanStack() )		
		{	
			int nStackCount = Item[nGameIdx].GetDurability();
			if(nCurNo < nStackCount)
			{
				Item[nGameIdx].SetDurability(nStackCount - nCurNo);
				nCurNo = 0;
				//
				ITEM_CHANGE_INFO sChange;
				sChange.ProtocolType	= s2c_itemchangeinfosync;
				sChange.m_btType		= 1;
				sChange.m_dwItemID		= Item[nGameIdx].GetID();
				sChange.m_uChange		= (UINT)Item[nGameIdx].GetDurability();
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
			}
			else
			{
				Remove(nGameIdx);
				ItemSet.Remove(nGameIdx);
				nCurNo = nCurNo - nStackCount;
			}
		}
		else
		{
			Remove(nGameIdx);
			ItemSet.Remove(nGameIdx);
			nCurNo--;
		}
		if (nCurNo <= 0)
			return TRUE;
	}
	return FALSE;
}
#endif
#ifdef _SERVER
// --
//
// --
BOOL KItemList::UseStackItem(const int nGameIdx, const int nCount /*= 1*/)
{	
	if( !IsMyItem(nGameIdx) )
		return FALSE;

	if( !Item[nGameIdx].CanStack() )
		return FALSE;
	
	int nStackCount = Item[nGameIdx].GetDurability();
	if(nStackCount > nCount)
		nStackCount = nStackCount - nCount;
	else 
		nStackCount = 0;

	if(nStackCount > 0)
	{
		Item[nGameIdx].SetDurability(nStackCount);
		//
		ITEM_CHANGE_INFO sChange;
		sChange.ProtocolType	= s2c_itemchangeinfosync;
		sChange.m_btType		= 1; 
		sChange.m_dwItemID		= Item[nGameIdx].GetID();
		sChange.m_uChange		= (UINT)Item[nGameIdx].GetDurability();
		if (g_pServer)
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
	}
	else
	{
		Remove(nGameIdx);
		ItemSet.Remove(nGameIdx);
	}
	return TRUE;
}
#endif
#ifdef _SERVER
// --
//
// --
BOOL KItemList::RemoveStackItem(const int nGameIdx)
{	
	if( !IsMyItem(nGameIdx) )
		return FALSE;

	if( !Item[nGameIdx].CanStack() )
		return FALSE;

	Remove(nGameIdx);
	ItemSet.Remove(nGameIdx);

	return TRUE;
}
#endif

#ifdef _SERVER
void	KItemList::GetTradeRoomItemInfo()
{
	_ASSERT(ItemSet.m_psItemInfo);
	memset(ItemSet.m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{	
		if(nPos >= MAX_TRADE_ITEM)
			break;
		nItemIdx = m_Room[room_trade].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < MAX_TRADE_ITEM);

		ItemSet.m_psItemInfo[nPos].m_nIdx = nItemIdx;
		ItemSet.m_psItemInfo[nPos].m_nWidth = Item[nItemIdx].GetWidth();
		ItemSet.m_psItemInfo[nPos].m_nHeight = Item[nItemIdx].GetHeight();
		nPos++;
	}

	TRADE_ITEM_INFO	sTemp;
	for (int i = nPos - 1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (ItemSet.m_psItemInfo[j].m_nWidth * ItemSet.m_psItemInfo[j].m_nHeight < 
				ItemSet.m_psItemInfo[j + 1].m_nWidth * ItemSet.m_psItemInfo[j + 1].m_nHeight)
			{
				sTemp = ItemSet.m_psItemInfo[j];
				ItemSet.m_psItemInfo[j] = ItemSet.m_psItemInfo[j + 1];
				ItemSet.m_psItemInfo[j + 1] = sTemp;
			}
		}
	}
}
#endif

#ifdef _SERVER
BOOL	KItemList::TradeCheckCanPlace()
{
	LPINT	pnTempRoom;
	pnTempRoom = new int[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	memcpy(pnTempRoom, m_Room[room_equipment].m_pArray, sizeof(int) * EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT);

	int		nPos, i, j, a, b, nFind, nNext;
	for (nPos = 0; nPos < TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT; nPos++)
	{
		if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
			break;
		nFind = 0;
		for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - ItemSet.m_psItemInfo[nPos].m_nHeight + 1; i++)
		{
			for (j = 0; j < EQUIPMENT_ROOM_WIDTH - ItemSet.m_psItemInfo[nPos].m_nWidth + 1; j++)
			{
				nNext = 0;
				for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
				{
					for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
					{
						if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
						{
							nNext = 1;
							break;
						}
					}
					if (nNext)
						break;
				}
				if (!nNext)
				{
					ItemSet.m_psItemInfo[nPos].m_nX = j;
					ItemSet.m_psItemInfo[nPos].m_nY = i;
					for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
					{
						for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
							pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b] = ItemSet.m_psItemInfo[nPos].m_nIdx;
					}

					nFind = 1;
					break;
				}
			}
			if (nFind)
				break;
		}
		if (!nFind)
		{
			delete []pnTempRoom;
			pnTempRoom = NULL; //them code 16/09/2023 by kinnox;
			return FALSE;
		}
	}

	delete []pnTempRoom;
	pnTempRoom = NULL; //them code 16/09/2023 by kinnox;
	return TRUE;
}
#endif

#ifdef _SERVER
BOOL	KItemList::FindEmptyPlaceEquipment(BYTE btWidth, BYTE btHeight)
{
	return m_Room[room_equipment].FindEmptyPlace(btWidth, btHeight);
}
#endif

#ifdef _SERVER
int		KItemList::GetEmptyCellNumInEquipment()
{
	return m_Room[room_equipment].GetEmptyCellNum();
}
#endif
#ifndef _SERVER
int		KItemList::GetEmptyCellNumInEquipment()
{
	return m_Room[room_equipment].GetEmptyCellNum();
}
#endif
int		KItemList::GetSameItemNumInEquipment(const int nGenre, const int nDetailType, const int nParticularType, const int nSeries,
											 const int nLevel)
{
	return m_Room[room_equipment].GetSameItemNum(nGenre, nDetailType, nParticularType, nSeries, nLevel);
}

int	KItemList::CountSameItem(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, const int nParticularType, const int nSeries, const int nLevel)
{
	if(eRoom < room_equipment || eRoom >= room_num)
		return 0;

	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;
	int nNo = 0;

	while(1)
	{
		nExistGameIdx = m_Room[eRoom].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;

		if( Item[nExistGameIdx].GetGenre() != nGenre )
			continue;
		if( Item[nExistGameIdx].GetDetailType() != nDetailType )
			continue;
		if( Item[nExistGameIdx].GetParticular() != nParticularType )
			continue;
		if( Item[nExistGameIdx].GetSeries() != nSeries )
			continue;
		if( Item[nExistGameIdx].GetLevel() != nLevel )
			continue;
		

		if( Item[nExistGameIdx].CanStack() )
			nNo = nNo + Item[nExistGameIdx].GetDurability();
		else 
			nNo++;
	}
	
	return nNo;
}

#ifdef _SERVER
BOOL KItemList::RemoveItemInEquipment(const int nCount, const int nGenre, const int nDetailType, const int nPaticularType, const int nSeries, const int nLevel)
{	
	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;
	int nCurCount = 0, j = 0;

	int* pnAryIdx;
	pnAryIdx = new int[nCount];
	ZeroMemory(pnAryIdx, sizeof(int) * nCount);

	while(1)
	{
		nExistGameIdx = m_Room[room_equipment].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;
		if(nCurCount >= nCount)
			break;

		if( Item[nExistGameIdx].GetGenre() != nGenre )
			continue;
		if( Item[nExistGameIdx].GetDetailType() != nDetailType )
			continue;
		if( Item[nExistGameIdx].GetParticular() != nPaticularType )
			continue;
		if( Item[nExistGameIdx].GetSeries() != nSeries )
			continue;
		if( Item[nExistGameIdx].GetLevel() != nLevel )
			continue;
		
		pnAryIdx[j] = nExistGameIdx;
		if( Item[nExistGameIdx].CanStack() )
			nCurCount = nCurCount + Item[nExistGameIdx].GetStackCount();
		else
			nCurCount++;

		j++;

	}
	
	if(nCurCount <= 0 || j <= 0)
		return FALSE;

	int nTempCount = nCount;
	int nStack = 0;
	for(int i = 0; i < j; i++)
	{
		if(nTempCount <= 0)
			break;
		if( pnAryIdx[i] )
		{
			if( Item[pnAryIdx[i]].CanStack() )
			{
				nStack = Item[pnAryIdx[i]].GetStackCount();
				if(nStack >= nTempCount)
					UseStackItem(pnAryIdx[i], nTempCount);
				else
					UseStackItem(pnAryIdx[i], nStack);

				nTempCount = nTempCount - nStack;
			}
			else
			{
				nTempCount--;
				Remove(pnAryIdx[i]);
				ItemSet.Remove(pnAryIdx[i]);
				
			}
		}
	}
	if(pnAryIdx)
		delete [] pnAryIdx;
	pnAryIdx = NULL;

	return TRUE;
}
#endif

#ifdef _SERVER
BOOL	KItemList::CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY)
{
	if (nWidth <= 0 || nHeight <= 0 || !pnX || !pnY)
		return FALSE;

	_ASSERT(m_Room[room_equipment].m_pArray);

	LPINT	pnTempRoom;
	int		i, j, a, b, nNext;

	pnTempRoom = m_Room[room_equipment].m_pArray;

	for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - nHeight + 1; i++)
	{
		for (j = 0; j < EQUIPMENT_ROOM_WIDTH - nWidth + 1; j++)
		{
			nNext = 0;
			for (a = 0; a < nHeight; a++)
			{
				for (b = 0; b < nWidth; b++)
				{
					if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
					{
						nNext = 1;
						break;
					}
				}
				if (nNext)
					break;
			}
			if (!nNext)
			{
				*pnX = j;
				*pnY = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}
#endif
// --
//
// --
BOOL KItemList::CheckSameDetailType(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType)
{	
	if(eRoom < room_equipment || eRoom >= room_num)
		return FALSE;
	
	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;

	while(1)
	{
		nExistGameIdx = m_Room[eRoom].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;

		if( Item[nExistGameIdx].GetGenre() != nGenre )
			continue;
		if( Item[nExistGameIdx].GetDetailType() != nDetailType )
			continue;

		break;
	}
	if(nExistGameIdx)
		return TRUE;

	return FALSE;

}
// --
//
// --
BOOL KItemList::FindSameDetailType(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, int *pnIdx, int *pnX, int *pnY)
{	
	if(eRoom < room_equipment || eRoom >= room_num)
		return FALSE;
	
	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;

	while(1)
	{
		nExistGameIdx = m_Room[eRoom].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;

		if( Item[nExistGameIdx].GetGenre() != nGenre )
			continue;
		if( Item[nExistGameIdx].GetDetailType() != nDetailType )
			continue;

		break;
	}
	if(nExistGameIdx == 0)
		return FALSE;
	
	*pnIdx = nExistGameIdx;
	*pnX = nCurX;
	*pnY = nCurY;

	return TRUE;

}
#ifdef _SERVER
// --
//
// --
BOOL	KItemList::FindSameItem(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, const int nParticularType, const int nSeries, const int nLevel, int *pnIdx, int *pnX, int *pnY)
{	
	if(eRoom < room_equipment || eRoom >= room_num)
		return FALSE;

	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;

	while(1)
	{
		nExistGameIdx = m_Room[room_equipment].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;

		if( Item[nExistGameIdx].GetGenre() != nGenre )
			continue;
		if( Item[nExistGameIdx].GetDetailType() != nDetailType )
			continue;
		if( Item[nExistGameIdx].GetParticular() != nParticularType )
			continue;
		if( Item[nExistGameIdx].GetSeries() != nSeries )
			continue;
		if( Item[nExistGameIdx].GetLevel() != nLevel )
			continue;

		break;
	}

	if(nExistGameIdx == 0)
		return FALSE;
	
	*pnIdx = nExistGameIdx;
	*pnX = nCurX;
	*pnY = nCurY;

	return TRUE;
}
#endif

#ifdef _SERVER
BOOL	KItemList::AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY)
{	
	if(Player[this->m_PlayerIdx].CheckTrading()) 
		return FALSE;
	if(Player[this->m_PlayerIdx].m_cShop.GetState())
		return FALSE;

	if (!m_Room[room_equipment].m_pArray || !m_Room[room_immediacy].m_pArray)
		return FALSE;
	if (nSrcX < 0 || nSrcX >= m_Room[room_equipment].m_nWidth || nSrcY < 0 || nSrcY >= m_Room[room_equipment].m_nHeight)
		return FALSE;
	if (nDestX < 0 || nDestX >= m_Room[room_immediacy].m_nWidth || nDestY < 0 || nDestY >= m_Room[room_immediacy].m_nHeight)
		return FALSE;
	if (nItemIdx != m_Room[room_equipment].m_pArray[nSrcY * m_Room[room_equipment].m_nWidth + nSrcX] ||
		0 != m_Room[room_immediacy].m_pArray[nDestY * m_Room[room_immediacy].m_nWidth + nDestX])
		return FALSE;
	if(Item[nItemIdx].GetWidth() != 1)
		return FALSE;
	if(Item[nItemIdx].GetHeight() != 1)
		return FALSE;
	if (!m_Room[room_equipment].PickUpItem(nItemIdx, nSrcX, nSrcY, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
		return FALSE;
	if (!m_Room[room_immediacy].PlaceItem(nDestX, nDestY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
	{
		m_Room[room_equipment].PlaceItem(nSrcX, nSrcY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight());
		return FALSE;
	}
	int nListIdx = IsMyItem(nItemIdx);
	_ASSERT(nListIdx > 0);
	m_Items[nListIdx].nPlace = pos_immediacy;
	m_Items[nListIdx].nX = nDestX;
	m_Items[nListIdx].nY = nDestY;

	ITEM_AUTO_MOVE_SYNC	sMove;
	sMove.ProtocolType = s2c_ItemAutoMove;
	sMove.m_btSrcPos = pos_equiproom;
	sMove.m_btSrcX = nSrcX;
	sMove.m_btSrcY = nSrcY;
	sMove.m_btDestPos = pos_immediacy;
	sMove.m_btDestX = nDestX;
	sMove.m_btDestY = nDestY;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(ITEM_AUTO_MOVE_SYNC));

	return TRUE;
}
#endif

#ifndef	_SERVER
// --
//
// --
void	KItemList::RemoveAllInOneRoom(int nRoom)
{
	if (nRoom < 0 || nRoom >= room_num)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[nRoom].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < m_Room[nRoom].m_nWidth * m_Room[nRoom].m_nHeight);

		Remove(nItemIdx);

		nPos++;
	}
}
#endif
//AutoAI by kinnox;
int KItemList::CalcFreeItemCellCount(int nWidth, int nHeight, int nRoom)
{
	_ASSERT(m_Room[nRoom].m_pArray);

	return m_Room[nRoom].FindFreeCell(nWidth, nHeight);
}
//end code
#ifndef _SERVER
// --
//
// --
void KItemList::LockOperation()
{
	if (IsLockOperation())
	{
		return;
	}
	m_bLockOperation = TRUE;
}
#endif

#ifndef _SERVER
// --
//
// --
void KItemList::UnlockOperation()
{
	if (!IsLockOperation())
	{
		return;
	}
	m_bLockOperation = FALSE;
}
#endif

// --
//
// --
int KItemList::GetActiveAttribNum(int nIdx)
{
	for (int i = 0; i < itempart_num; i++)
	{
		if (nIdx == m_EquipItem[i])
		{
			return GetEquipEnhance(i);
		}
	}
	return 0;
}

#ifdef _SERVER
// --
//	
// --
void	KItemList::AutoLoseItemFromEquipmentRoom(int nRate)
{
	if (Player[m_PlayerIdx].CheckTrading())
		return;
	if (nRate <= 0 || nRate > 100)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[room_equipment].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;

		if (item_task == Item[nItemIdx].GetGenre())
			continue;
		if (item_script == Item[nItemIdx].GetGenre())
			continue;
		if (Item[nItemIdx].GetBindState() > 0)
			continue;

		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nIdx = nItemIdx;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nPlace = pos_equiproom;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nX = nXpos;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nY = nYpos;
		nPos++;
	}
	if (nPos == 0)
		return;

	KMapPos			sMapPos;
	int				nSelect;
	int				nObj;
	KObjItemInfo	sInfo;

	for (int i = 0; i < nPos; i++)
	{
		if (g_Random(100) >= nRate)
			continue;
		nItemIdx = ItemSet.m_sLoseItemFromEquipmentRoom[i].nIdx;
		Player[m_PlayerIdx].GetAboutPos(&sMapPos);
		if (Remove(nItemIdx))
		{
			sInfo.m_nItemID = nItemIdx;
			sInfo.m_dwItemID = Item[nItemIdx].GetID();
			sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
			sInfo.m_nColorID = 0;
			//AutoAI by kinnox;
			sInfo.m_nGenre = Item[nItemIdx].GetGenre();
			sInfo.m_nDetailType = Item[nItemIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;
			//end code;

			nObj = ObjSet.Add(Item[nItemIdx].GetObjID(), sMapPos, sInfo);
			if (nObj == -1)
			{
				ItemSet.Remove(nItemIdx);
			}
			else
			{
				Object[nObj].SetItemBelong(-1);
			}

			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
			sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
			memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
			memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
		}
	}
}
#endif

#ifdef _SERVER
// --
//
// --
void	KItemList::AutoLoseEquip()
{
#ifndef defEQUIP_POWER
	int		i, nPos = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;

		if (i == itempart_horse)
			continue;
		if (i == itempart_mask)
			continue;
		if (i == itempart_mantle)
			continue;
		if(Item[m_EquipItem[i]].GetBind() > 0)
			continue;

		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		nPos++;
	}
	if (nPos == 0)
		return;

	int		nSelect = g_Random(nPos);
#endif

#ifdef defEQUIP_POWER
	int		i, nPos = 0, nTotalPower = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;
		if(Item[m_EquipItem[i]].GetBindState() > 0)
			continue;
		nTotalPower += g_nEquipPower[i];
		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		ItemSet.m_sLoseEquipItem[nPos].nX = nTotalPower;
		nPos++;
	}
	if (nTotalPower == 0)
		return;
	int		nSelect = g_Random(nTotalPower);
	for (i = 0; i < nPos; i++)
	{
		if (ItemSet.m_sLoseEquipItem[i].nX > nSelect)
		{
			nSelect = i;
			break;
		}
	}
	if (i >= nPos)
		return;
#endif

	int			 nItemIdx;
	KMapPos		sMapPos;

	nItemIdx = ItemSet.m_sLoseEquipItem[nSelect].nIdx;
	Player[m_PlayerIdx].GetAboutPos(&sMapPos);
	if (Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_dwItemID = Item[nItemIdx].GetID();
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
		sInfo.m_nColorID = 0;
		//AutoAI by kinnox;
		sInfo.m_nGenre = Item[nItemIdx].GetGenre();
		sInfo.m_nDetailType = Item[nItemIdx].GetDetailType();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_dwNpcId = 0;
		//end code;
		nObj = ObjSet.Add(Item[nItemIdx].GetObjID(), sMapPos, sInfo);
		if (nObj == -1)
		{
			ItemSet.Remove(nItemIdx);
		}
		else
		{
			Object[nObj].SetItemBelong(-1);
		}

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
		sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
		memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
		memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
}
#endif

#ifndef _SERVER
// --
//	
// --
int		KItemList::GetSameDetailItemNum(int nImmediatePos)
{
	if (nImmediatePos < 0 || nImmediatePos >= IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)
		return 0;
	int		nIdx = m_Room[room_immediacy].FindItem(nImmediatePos, 0);
	if (nIdx <= 0)
		return 0;
	return m_Room[room_equipment].CalcSameDetailType(Item[nIdx].GetGenre(), Item[nIdx].GetDetailType()) + 1;
}
#endif

#ifdef _SERVER
// --
//	
// --
void KItemList::Abrade(int nType)
{
	int nItemIdx = 0;
	int i, nMaxDur, nOldDur, nDur;

	for(i = 0; i < itempart_num; i++)
	{
		nItemIdx = m_EquipItem[i];
		if( (nItemIdx <= 0) || (nItemIdx >= MAX_ITEM) )
			continue;
		
		nMaxDur = Item[nItemIdx].GetMaxDurability();
		if(nMaxDur == -1) 
			continue;

		nOldDur = Item[nItemIdx].GetDurability();
		if(nOldDur == -1) 
			continue;

		nDur = Item[nItemIdx].Abrade(ItemSet.GetAbradeRange(nType, i));
		if(nDur == -1)
			continue;

		if( nDur == 0 )
		{	
			if( !BackupBrokenEquip(nItemIdx) )
				continue;

			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_ITEM_DAMAGED;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
			sMsg.m_lpBuf = (void *)Item[nItemIdx].m_dwID;
			if (g_pServer)
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_lpBuf = 0;

		}
		else if(nOldDur > nDur)
		{
			ITEM_CHANGE_INFO sChange;
			sChange.ProtocolType	= s2c_itemchangeinfosync;
			sChange.m_btType		= 0;
			sChange.m_dwItemID		= Item[nItemIdx].GetID();
			sChange.m_uChange		= (UINT)(nOldDur - nDur);
			if (g_pServer)
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
		}
	}
}
#endif
#ifdef	_SERVER
BOOL KItemList::BackupBrokenEquip(const int nGameIdx)
{	

	if(Player[m_PlayerIdx].CheckTrading())
		return FALSE;

	int i = IsMyItem(nGameIdx);
	if(!i)
		return FALSE;
	
	if( (Item[nGameIdx].GetGenre() < item_equip) || (Item[nGameIdx].GetGenre() > item_platinaequip) )
		return FALSE;

	char* pszName = Item[nGameIdx].GetName();
	short nDetailType = Item[nGameIdx].GetDetailType();
	char cSeries = Item[nGameIdx].GetSeries();
	const int nLevel = Item[nGameIdx].GetLevel();
	WORD wRecord = Item[nGameIdx].GetRecord();
	DWORD dwRand = Item[nGameIdx].GetGeneratorParam()->dwRandomSeed;
	int nMagicLevel[6];
	for(i = 0; i < 6; i++)
	{	
		nMagicLevel[i] = Item[nGameIdx].GetGeneratorParam()->nGeneratorLevel[i];
	}
	int nLuck = Item[nGameIdx].GetGeneratorParam()->nLuck;
	int nVersion = Item[nGameIdx].GetGeneratorParam()->nVersion;
	const DWORD dwBindState = Item[nGameIdx].GetBindState();
	const DWORD dwExpiredTime = Item[nGameIdx].GetTime();
	//
	Remove(nGameIdx);
	ItemSet.Remove(nGameIdx);
	//
	const int nNewIdx = ItemSet.AddExist(item_brokenequip, cSeries, nLevel, wRecord, nLuck, nDetailType, nMagicLevel, nVersion, dwRand);
	if( (nNewIdx <= 0) || (nNewIdx >= MAX_ITEM) )
		return FALSE;
	//
	if(dwBindState > 0)
		Item[nNewIdx].SetBindState(dwBindState);
	if(dwExpiredTime > 0)
		Item[nNewIdx].SetTime(dwExpiredTime);

	int x, y;
	if(CheckCanPlaceInEquipment(Item[nNewIdx].GetWidth(), Item[nNewIdx].GetHeight(), &x, &y))
	{
		Add(nNewIdx, pos_equiproom, x, y);
	}
	else
	{
		if(m_Hand)
		{	
			int nTempIdx = m_Hand;
			//
			Remove(nTempIdx);
			ItemSet.Remove(nTempIdx);
		}
		Add(nNewIdx, pos_hand, 0 ,0);
	}

	return TRUE;
}
#endif

#ifdef _SERVER
// --
//
// --
BOOL KItemList::RestoreBrokenEquip(const int nGameIdx)
{
	if(Player[m_PlayerIdx].CheckTrading())
		return FALSE;

	int i = IsMyItem(nGameIdx);
	if(!i)
		return FALSE;
	
	if(Item[nGameIdx].GetGenre() != item_brokenequip)
		return FALSE;

	char* pszName = Item[nGameIdx].GetName();
	short nDetailType = Item[nGameIdx].GetDetailType();
	char cSeries = Item[nGameIdx].GetSeries();
	const int nLevel = Item[nGameIdx].GetLevel();
	WORD wRecord = Item[nGameIdx].GetRecord();
	DWORD dwRand = Item[nGameIdx].GetGeneratorParam()->dwRandomSeed;
	int nMagicLevel[6];
	for(i = 0; i < 6; i++)
	{
		nMagicLevel[i] = Item[nGameIdx].GetGeneratorParam()->nGeneratorLevel[i];
	}
	int nLuck = Item[nGameIdx].GetGeneratorParam()->nLuck;
	int nVersion = Item[nGameIdx].GetGeneratorParam()->nVersion;
	//
	const DWORD dwBindState = Item[nGameIdx].GetBindState();
	const DWORD dwExpiredTime = Item[nGameIdx].GetTime();
	//
	char cGenre = item_equip;
	if(nLuck == 7531)
		cGenre = item_goldequip;
	else if(nLuck >= 1000000000)
		cGenre = item_purpleequip;
	
	//
	Remove(nGameIdx);
	ItemSet.Remove(nGameIdx);

	const int nNewIdx = ItemSet.AddExist(cGenre, cSeries, nLevel, wRecord, nLuck, nDetailType, nMagicLevel, nVersion, dwRand);
	if( (nNewIdx <= 0) || (nNewIdx >= MAX_ITEM) )
		return FALSE;
	
	//
	if(dwBindState > 0)
		Item[nNewIdx].SetBindState(dwBindState);
	if(dwExpiredTime > 0)
		Item[nNewIdx].SetTime(dwExpiredTime);

	int x, y;
	if(CheckCanPlaceInEquipment(Item[nNewIdx].GetWidth(), Item[nNewIdx].GetHeight(), &x, &y))
	{
		Add(nNewIdx, pos_equiproom, x, y);
	}
	else
	{
		if(m_Hand)
		{	
			int nTempIdx = m_Hand;
			//
			Remove(nTempIdx);
			ItemSet.Remove(nTempIdx);
		}
		Add(nNewIdx, pos_hand, 0 ,0);
	}

	return TRUE;
}
// --
//
// --
BOOL KItemList::AddAgain(const int nGameIdx)
{
	if(Player[m_PlayerIdx].CheckTrading())
		return FALSE;

	int i = IsMyItem(nGameIdx);
	if(!i)
		return FALSE;
	
	char* pszName = Item[nGameIdx].GetName();
	short nDetailType = Item[nGameIdx].GetDetailType();
	char cSeries = Item[nGameIdx].GetSeries();
	const int nLevel = Item[nGameIdx].GetLevel();
	WORD wRecord = Item[nGameIdx].GetRecord();
	DWORD dwRand = Item[nGameIdx].GetGeneratorParam()->dwRandomSeed;
	int nMagicLevel[6];
	for(i = 0; i < 6; i++)
	{
		nMagicLevel[i] = Item[nGameIdx].GetGeneratorParam()->nGeneratorLevel[i];
	}
	int nLuck = Item[nGameIdx].GetGeneratorParam()->nLuck;
	int nVersion = Item[nGameIdx].GetGeneratorParam()->nVersion;
	//
	const DWORD dwBindState = Item[nGameIdx].GetBindState();
	const DWORD dwExpiredTime = Item[nGameIdx].GetTime();
	//
	char cGenre = item_equip;
	if(nLuck == 7531)
		cGenre = item_goldequip;
	else if(nLuck >= 1000000000)
		cGenre = item_purpleequip;
	
	//
	Remove(nGameIdx);
	ItemSet.Remove(nGameIdx);

	const int nNewIdx = ItemSet.AddExist(cGenre, cSeries, nLevel, wRecord, nLuck, nDetailType, nMagicLevel, nVersion, dwRand);
	if( (nNewIdx <= 0) || (nNewIdx >= MAX_ITEM) )
		return FALSE;
	
	//
	if(dwBindState > 0)
		Item[nNewIdx].SetBindState(dwBindState);
	if(dwExpiredTime > 0)
		Item[nNewIdx].SetTime(dwExpiredTime);

	int x, y;
	if(CheckCanPlaceInEquipment(Item[nNewIdx].GetWidth(), Item[nNewIdx].GetHeight(), &x, &y))
	{
		Add(nNewIdx, pos_equiproom, x, y);
	}
	else
	{
		if(m_Hand)
		{	
			int nTempIdx = m_Hand;
			//
			Remove(nTempIdx);
			ItemSet.Remove(nTempIdx);
		}
		Add(nNewIdx, pos_hand, 0 ,0);
	}

	return TRUE;
}

#endif

#ifdef _SERVER
// --
//	
// --
void KItemList::SetItemBindState(int nIndex, BYTE btState, BYTE btDay)
{	
	if(Player[m_PlayerIdx].CheckTrading())
		return;

	if(m_Hand)
		return;

	if(!IsMyItem(nIndex))
		return;
	
	if(Item[nIndex].GetPlayerShopPrice() != 0) // them dieu kien check playershop by kinnox;
		return;

	if(btDay > 0)
	{
		Item[nIndex].SetBindState(btDay, TRUE);
	}
	else
	{
		Item[nIndex].SetBindState(btState);
	}
	// if (Item[nIndex].GetGenre() != item_medicine)
	// {
		// if(btDay > 0)
		// {	
			// char szMsg[512];//80  //luu y do dai cua ky tu khi build release by kinnox;
			// sprintf(szMsg, "ß∑ mÎ kh„a b∂o hi”m vÀt ph»m <color=green>[%s]<color>.", Item[nIndex].GetName());
			// KPlayerChat::SendSystemInfo(1, m_PlayerIdx, "VÀt ph»m", szMsg, strlen(szMsg));
		// }
		// else
		// {	
			// char szMsg[512];//80  //luu y do dai cua ky tu khi build release by kinnox;
			// sprintf(szMsg, "ß∑ kh„a b∂o hi”m vÀt ph»m <color=green>[%s]<color>.", Item[nIndex].GetName());
			// KPlayerChat::SendSystemInfo(1, m_PlayerIdx, "VÀt ph»m", szMsg, strlen(szMsg));
		// }
	// }
	ITEM_CHANGE_INFO sChange;
	sChange.ProtocolType	= s2c_itemchangeinfosync;
	sChange.m_btType		= 2;
	sChange.m_dwItemID		= Item[nIndex].GetID();
	sChange.m_uChange		= (UINT)Item[nIndex].GetBindState();
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));

}
#endif

#ifdef _SERVER
// --
//	
// --
char KItemList::GetItemBindState(int nIndex)
{	
	char nRet = -1;

	if(Player[m_PlayerIdx].CheckTrading())
		return nRet;

	if(m_Hand)
		return nRet;

	if(!IsMyItem(nIndex))
		return nRet;	
	
	return Item[nIndex].GetBindState();
}
#endif

// --
//
// --
BOOL KItemList::GetIfActive()
{	
	if(!m_GoldInfo)
		return FALSE;

	int i, j, nCount;
	for(i = 0; i < itempart_horse; i++)
	{	
		if(m_GoldInfo[i][0] > 0)
		{	
			nCount = 1;
			for(j = 0; j < itempart_horse; j++)
			{	
				if(j == i)
					continue;
				if(m_GoldInfo[j][0] > 0)
				{
					if( m_GoldInfo[i][0] == m_GoldInfo[j][0] )
					{
						if( j == itempart_ring1 )
						{
							if( m_GoldInfo[itempart_ring2][0] == m_GoldInfo[j][0] )
								if( m_GoldInfo[itempart_ring2][1] == m_GoldInfo[j][1] )
									continue;
						}
						if( j == itempart_ring2 )
						{		
							if( m_GoldInfo[itempart_ring1][0] == m_GoldInfo[j][0] )
								if( m_GoldInfo[itempart_ring1][1] == m_GoldInfo[j][1] )
									continue;
						}
						nCount++;
					}
				}
				
				if( nCount >= m_GoldInfo[i][2] )
					return TRUE;				
			}
		}
	}
	return FALSE;
}

// --
//
// --
void KItemList::SetMaskLock( BOOL bFlag )
{
	m_bMaskLock = bFlag;
#ifdef _SERVER 
	NPC_MASK_LOCK_SYNC Sync;
	Sync.ProtocolType = s2c_masklocksync;
	Sync.m_bFlag = m_bMaskLock;
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&Sync, sizeof(NPC_MASK_LOCK_SYNC));
#endif
}

// --
//	
// --
int KItemList::IsMyItem(int nIndex)
{	
	int nIdx = 0;
	if(nIndex <= 0 || nIndex >= MAX_ITEM) 
		return nIdx;

	nIdx = FindSame(nIndex);

	return nIdx;
}

#ifdef _SERVER
// --
//	
// --
void KItemList::SetMarkPriceItem(DWORD dwItemID, int nPrice)
{	
	int nGameIdx = SearchID(dwItemID);
	if(nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
		return;

	if(Item[nGameIdx].GetGenre() == item_task ||
		Item[nGameIdx].GetGenre() == item_script)
	{
		if(Item[nGameIdx].GetParticular() == 0)
			return;
	}
	if(Item[nGameIdx].GetBindState() > 0)
		return;
	
	if( Item[nGameIdx].CanStack() )
	{
		nPrice = nPrice * Item[nGameIdx].GetStackCount();
	}
	if(nPrice < 0 || nPrice >= 2000000000)
		nPrice = 0;
	Item[nGameIdx].SetPlayerShopPrice(nPrice);

	ITEM_CHANGE_INFO sChange;
	sChange.ProtocolType	= s2c_itemchangeinfosync;
	sChange.m_btType		= 5;
	sChange.m_dwItemID		= dwItemID;
	sChange.m_uChange		= (UINT)nPrice;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));	
}
#endif



#ifdef _SERVER
// --
//	
// --
void KItemList::AutoMoveItem(int nPos)
{
	if (Player[this->m_PlayerIdx].CheckTrading()) 
		return;
	if (Player[this->m_PlayerIdx].m_cShop.GetState())
		return;
	if (!m_Room[room_equipment].m_pArray)
		return;

	int nTempIdx = 0, nSrcX = 0, nSrcY = 0, nDesX = 0, nDesY = 0, nListIdx = 0, i = 0;
	BYTE	btSrcPlace, btDestPlace;
	
	ITEM_AUTO_MOVE_SYNC	sMove;
	sMove.ProtocolType = s2c_ItemAutoMove;
}
#endif



#ifndef _SERVER
BOOL	KItemList::FindSameInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY)
{
	return m_Room[room_equipment].FindSameDetailType(nGenre, nDetail, pnIdx, pnX, pnY);
}

int		KItemList::GetNextItemEquipmentRoom(int nItemIdx, int nX, int nY, int *nSrcX, int *nSrcY)
{
	return m_Room[room_equipment].GetNextItem(nItemIdx, nX, nY, nSrcX, nSrcY);
}
#endif

#ifndef _SERVER
BOOL	KItemList::FindSameInImmediacy(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY)
{
	return m_Room[room_immediacy].FindSameDetailType(nGenre, nDetail, pnIdx, pnX, pnY);
}
#endif

#ifdef _SERVER
// --
//
// --
int KItemList::Add_AutoStack(INVENTORY_ROOM eRoom, const int nGameIdx, const int nX, const int nY )
{
	if( (nGameIdx <= 0) || (nGameIdx >= MAX_ITEM) )
		return 0;

	if( (eRoom < room_equipment) || (eRoom > room_givebox) )//Expandbox by kinnox; //GiveBox by kinnox; chu y cho nay
		return 0;


	if( !Item[nGameIdx].CanStack() )
		return 0;
	
	const int nExistGameIdx = FindItemToStack(eRoom, nGameIdx);
	if(nExistGameIdx)
	{
		const int nResult = StackItem(nExistGameIdx, nGameIdx);

		ITEM_CHANGE_INFO sChange;
		sChange.ProtocolType	= s2c_itemchangeinfosync;
		sChange.m_btType		= 1;
		sChange.m_dwItemID		= Item[nExistGameIdx].GetID();
		sChange.m_uChange		= (UINT)Item[nExistGameIdx].GetDurability();
		if(g_pServer)
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));

		if(nResult == 0)
		{
			ItemSet.Remove(nGameIdx);
			return FindSame(nExistGameIdx);
		}

		if(nResult < 0)
		{
			ItemSet.Remove(nGameIdx);
			return FindSame(nExistGameIdx);
		}
	}
	
	return Add(nGameIdx, ms_RoomMap[eRoom], nX, nY);


}
// --
//
// --
int KItemList::FindItemToStack(INVENTORY_ROOM eRoom, const int nGameIdx)
{
	int nCurX = 0;
	int nCurY = 0;
	int	nExistGameIdx = 0;

	while(1)
	{
		nExistGameIdx = m_Room[eRoom].GetNextItem(nExistGameIdx, nCurX, nCurY, &nCurX, &nCurY);
		if(nExistGameIdx == 0)
			break;
		if( CheckStackItem(nExistGameIdx, nGameIdx) )
			break;
	}
	return nExistGameIdx;
}

// --
//
// --
BOOL KItemList::CheckStackItem(const int nIdxBeStack, const int nIdxForStack)
{
	if( (nIdxBeStack <= 0) || (nIdxBeStack >= MAX_ITEM) )
		return FALSE;

	if( (nIdxForStack <= 0) || (nIdxForStack >= MAX_ITEM) )
		return FALSE;

	if( !Item[nIdxBeStack].CanStack() )
		return FALSE;

	if( Item[nIdxBeStack].GetDurability() >= Item[nIdxBeStack].GetMaxStackCount() )
		return FALSE;

	if( Item[nIdxForStack].GetDurability() >= Item[nIdxForStack].GetMaxStackCount() )
		return FALSE;

	if(Item[nIdxBeStack].GetGenre() != Item[nIdxForStack].GetGenre())
		return FALSE;
	if(Item[nIdxBeStack].GetRecord() != Item[nIdxForStack].GetRecord())
		return FALSE;
	if(Item[nIdxBeStack].GetLevel() != Item[nIdxForStack].GetLevel())
		return FALSE;
	if(Item[nIdxBeStack].GetSeries() != Item[nIdxForStack].GetSeries())
		return FALSE;
	if(Item[nIdxBeStack].GetGeneratorParam()->nLuck != Item[nIdxForStack].GetGeneratorParam()->nLuck)
		return FALSE;

	if(Item[nIdxBeStack].GetBindState() != Item[nIdxForStack].GetBindState())
		return FALSE;
	if(Item[nIdxBeStack].GetTime() != Item[nIdxForStack].GetTime())
		return FALSE;

	return TRUE;
}
// --
//
// --
int KItemList::StackItem(const int nIdxBeStack, const int nIdxForStack)
{
	const int nTotalValue = Item[nIdxBeStack].GetDurability() + Item[nIdxForStack].GetDurability();
	const int nOdd = nTotalValue - Item[nIdxBeStack].GetMaxStackCount();

	if(nOdd == 0)
	{
		Item[nIdxBeStack].SetDurability( Item[nIdxBeStack].GetMaxStackCount() );
		return 0;
	}
	
	if(nOdd > 0)
	{
		Item[nIdxBeStack].SetStackCount( Item[nIdxBeStack].GetMaxStackCount() );
		Item[nIdxForStack].SetDurability(nOdd);
	}
	else
	{
		Item[nIdxBeStack].SetDurability( nTotalValue );
	}

	return nOdd;
}

// --
//
// --
BOOL KItemList::ExchangeStack(const int nIdxBeStack, const int nIdxForStack)
{
	if( !CheckStackItem(nIdxBeStack, nIdxForStack) )
		return FALSE;

	const int nResult = StackItem(nIdxBeStack, nIdxForStack);
	// 
	if(nResult == 0)
	{
		RemoveByIndex(nIdxForStack);
	}
	
	ITEM_CHANGE_INFO sChange;
	sChange.ProtocolType	= s2c_itemchangeinfosync;
	sChange.m_btType		= 1;
	sChange.m_dwItemID		= Item[nIdxBeStack].GetID();
	sChange.m_uChange		= (UINT)Item[nIdxBeStack].GetDurability();
	if(g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
	
	//
	if(nResult < 0)
	{	
		RemoveByIndex(nIdxForStack);
	}
	//
	if(nResult > 0)
	{	
		ITEM_CHANGE_INFO sChange;
		sChange.ProtocolType	= s2c_itemchangeinfosync;
		sChange.m_btType		= 1;
		sChange.m_dwItemID		= Item[nIdxForStack].GetID();
		sChange.m_uChange		= (UINT)Item[nIdxForStack].GetDurability();
		if(g_pServer)
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sChange, sizeof(ITEM_CHANGE_INFO));
	}

	return TRUE;
}
#endif

#ifndef _SERVER
// --
//
// --
int KItemList::GetSuiteDescColor(int nRecord, int nSuiteId)
{	
	int i;
	for(i = 0; i < itempart_horse ; i++)
	{
		if(Item[m_EquipItem[i]].GetGenre() != item_goldequip)
			continue;
		if(Item[m_EquipItem[i]].GetRecord() == nRecord && 
			Item[m_EquipItem[i]].GetSuiteId() == nSuiteId)
		{
			return 1;
		}
	}
	return 0;
}
#endif

// --
//
// --
void KItemList::UpdateGoldInfo(const int nPlace, const int nSuiteNo, int nSuiteId, int nSuiteCount)
{	
	if(!m_GoldInfo[nPlace])
		return;

	m_GoldInfo[nPlace][0] = nSuiteNo;
	m_GoldInfo[nPlace][1] = nSuiteId;
	m_GoldInfo[nPlace][2] = nSuiteCount;
}
#ifndef _SERVER
int KItemList::MapPlaceToUIContainer(int nPlace)
{
    switch (nPlace)
    {
    case pos_equiproom:        return UOC_ITEM_TAKE_WITH;

    case pos_repositoryroom:   return UOC_STORE_BOX;
    case pos_repositoryroom1:  return UOC_STORE_BOX1;
    case pos_repositoryroom2:  return UOC_STORE_BOX2;
    case pos_repositoryroom3:  return UOC_STORE_BOX3;
    case pos_repositoryroom4:  return UOC_STORE_BOX4;
    case pos_repositoryroom5:  return UOC_STORE_BOX5;

    case pos_expandtoryroom1:  return UOC_EXPAND_BOX1;
    case pos_givebox:          return UOC_GIVE_BOX;

    // N?u c?n thÏ b? sung thÍm c·c place kh·c ? ?‚y

    default:                   return 0; // 0/NULL: container khÙng h?p l?
    }
}
#endif // !_SERVER


#ifndef _SERVER
int KItemList::FindItemInRoom(INVENTORY_ROOM eRoom, const int nX, const int nY)
{	
	
	if(eRoom < room_equipment || eRoom >= room_num)
		return 0;

	return m_Room[eRoom].FindItem(nX, nY);
}

//AutoAI by kinnox;
BOOL	KItemList::CountCommonItem(int nGenre,int nDetailType, int P)
{
	int		nIdx = 0;
	int		bExits = FALSE;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		// if (nDetailType == 4)//ngu hoa ngoc lo hoang
		// {		
		// 	int nLevel = 5;
		// 	if (nLevel != Item[nGameIdx].GetLevel()) 
		// 	continue;
		// }
			
		if (nGenre != Item[nGameIdx].GetGenre())
			continue;
		else if (nDetailType != Item[nGameIdx].GetDetailType())
			continue;


		else if (m_Items[nIdx].nPlace != P)
			continue;

		bExits = TRUE;
	}
	return bExits;
}

BOOL KItemList::SearchEquipment(int nWidth, int nHeight)//AutoAI by kinnox;
{
	if (nWidth < 0 || nHeight < 0)
	{
		return FALSE;
	}

	POINT	pPt;
	if (!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{
		return FALSE;
	}
	return TRUE;
}
#endif

BOOL KItemList::BuildItem(int nIdx, int nPlace /* = -1 */)
{
	if (m_PlayerIdx <= 0 || nIdx <= 0)
		return FALSE;

	
	int nItemListIdx = FindSame(nIdx);
	if (!nItemListIdx)
	{
		_ASSERT(0);
		return FALSE;
	}

	m_BuildItem[nPlace] = nIdx;
	m_Items[nItemListIdx].nPlace = pos_builditem;
	m_Items[nItemListIdx].nX = nPlace;
	m_Items[nItemListIdx].nY = 0;
	return TRUE;
}


void KItemList::UnBuildItem(int nIdx, int nPos/* = -1*/)
{
	int i = 0;
	if (m_PlayerIdx <= 0)
		return;

	if (nIdx <= 0)
		return;

	if (nPos <= 0)
	{
		for (i = 0; i < MAX_PART_BUILD; i++)
		{
			if (m_BuildItem[i] == nIdx)
				break;
		}
		if (i == MAX_PART_BUILD)
			return;

	}
	else
	{
		if (m_BuildItem[nPos] != nIdx)	// ∂´Œ˜≤ª∂‘
			return;
		i = nPos;
	}

	m_BuildItem[i] = 0;
	return;
}
int KItemList::PositionToRoom(int nPlace)
{
    if (nPlace == pos_equiproom)
        return room_equipment;
    else if (nPlace == pos_expandtoryroom1)
        return room_expandtory1;
    else if (nPlace >= pos_repositoryroom && nPlace < pos_repositoryroom + 10)
        return room_repository + (nPlace - pos_repositoryroom);
    else
        return -1;  // ? KhÙng h?p l?
}

#ifdef _SERVER
BOOL KItemList::AutoMoveToRepository(int nItemID, int nSrcX, int nSrcY, int nSrcPlace, int nDestPlace)
{
    if (Player[m_PlayerIdx].CheckTrading()) return FALSE;

    int nSrcRoom  = PositionToRoom(nSrcPlace);
    int nDestRoom = PositionToRoom(nDestPlace);
    if (nSrcRoom < 0 || nDestRoom < 0) return FALSE;

    if (nItemID <= 0) nItemID = m_Room[nSrcRoom].GetIndex(nSrcX, nSrcY);
    if (nItemID <= 0 || nItemID >= MAX_ITEM || !Item[nItemID].GetID()) return FALSE;

    const int w = Item[nItemID].GetWidth();
    const int h = Item[nItemID].GetHeight();

    POINT pt;
    if (!m_Room[nDestRoom].FindRoom(w, h, &pt)) return FALSE;

    if (!m_Room[nSrcRoom].PickUpItem(nItemID, nSrcX, nSrcY, w, h)) return FALSE;
    if (!m_Room[nDestRoom].PlaceItem(pt.x, pt.y, nItemID, w, h))
    {
        // rollback
        m_Room[nSrcRoom].PlaceItem(nSrcX, nSrcY, nItemID, w, h);
        return FALSE;
    }

    int idx = FindSame(nItemID);
    if (idx <= 0) return FALSE;

    m_Items[idx].nPlace = nDestPlace;
    m_Items[idx].nX = pt.x;
    m_Items[idx].nY = pt.y;

    // G?i s2c sync
    ITEM_RIGHT_AUTO_MOVE pkt;
    pkt.ProtocolType = s2c_rightitemautomove;
    pkt.SrcPlace = (BYTE)nSrcPlace; pkt.SrcX = (BYTE)nSrcX; pkt.SrcY = (BYTE)nSrcY;
    pkt.DestPlace = (BYTE)nDestPlace; pkt.DestX = (BYTE)pt.x; pkt.DestY = (BYTE)pt.y;
    pkt.ItemID = Item[nItemID].GetID();

    g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&pkt, sizeof(pkt));
    return TRUE;
}

BOOL KItemList::AutoMoveToInventory(int nItemID, int nSrcX, int nSrcY, int nSrcPlace, int nDestPlace)
{
    if (Player[m_PlayerIdx].CheckTrading()) return FALSE;

    int nSrcRoom  = PositionToRoom(nSrcPlace);
    int nDestRoom = PositionToRoom(nDestPlace);
    if (nSrcRoom < 0 || nDestRoom < 0) return FALSE;

    if (nItemID <= 0) nItemID = m_Room[nSrcRoom].GetIndex(nSrcX, nSrcY);
    if (nItemID <= 0 || nItemID >= MAX_ITEM || !Item[nItemID].GetID()) return FALSE;

    const int w = Item[nItemID].GetWidth();
    const int h = Item[nItemID].GetHeight();

    POINT pt;
    if (!m_Room[nDestRoom].FindRoom(w, h, &pt)) return FALSE;

    if (!m_Room[nSrcRoom].PickUpItem(nItemID, nSrcX, nSrcY, w, h)) return FALSE;
    if (!m_Room[nDestRoom].PlaceItem(pt.x, pt.y, nItemID, w, h))
    {
        m_Room[nSrcRoom].PlaceItem(nSrcX, nSrcY, nItemID, w, h);
        return FALSE;
    }

    int idx = FindSame(nItemID);
    if (idx <= 0) return FALSE;

    m_Items[idx].nPlace = nDestPlace;
    m_Items[idx].nX = pt.x;
    m_Items[idx].nY = pt.y;

    ITEM_RIGHT_AUTO_MOVE pkt;
    pkt.ProtocolType = s2c_rightitemautomove;
    pkt.SrcPlace = (BYTE)nSrcPlace; pkt.SrcX = (BYTE)nSrcX; pkt.SrcY = (BYTE)nSrcY;
    pkt.DestPlace = (BYTE)nDestPlace; pkt.DestX = (BYTE)pt.x; pkt.DestY = (BYTE)pt.y;
    pkt.ItemID = Item[nItemID].GetID();

    g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&pkt, sizeof(pkt));
    return TRUE;
}
#endif
#ifndef _SERVER
BOOL KItemList::HandleAutoMoveFromServer(ItemPos sSrc, ItemPos sDest, int nItemID)
{
    //g_DebugLog("[CLIENT] HandleAutoMoveFromServer Src=(%d,%d,%d) Dest=(%d,%d,%d) ID=%d", sSrc.nPlace, sSrc.nX, sSrc.nY, sDest.nPlace, sDest.nX, sDest.nY, nItemID);

    int nSrcRoom  = PositionToRoom(sSrc.nPlace);
    int nDestRoom = PositionToRoom(sDest.nPlace);
    if (nSrcRoom < 0 || nDestRoom < 0)
        return FALSE;


    int nItem = nItemID;
    if (nItem <= 0)
        nItem = m_Room[nSrcRoom].GetIndex(sSrc.nX, sSrc.nY);
    if (nItem <= 0)
    {
        return FALSE;
    }

    int w = Item[nItem].GetWidth();
    int h = Item[nItem].GetHeight();

    if (!m_Room[nSrcRoom].PickUpItem(nItem, sSrc.nX, sSrc.nY, w, h))
    {
        return FALSE;
    }
    int occupied = m_Room[nDestRoom].GetIndex(sDest.nX, sDest.nY);
    if (occupied > 0 && occupied != nItem)
    {
        int occList = FindSame(occupied);
        if (occList > 0)
        {
            m_Room[nDestRoom].PickUpItem(occupied, sDest.nX, sDest.nY, Item[occupied].GetWidth(), Item[occupied].GetHeight());
            m_Items[occList].nPlace = 0; m_Items[occList].nX = 0; m_Items[occList].nY = 0;
        }
    }

    if (!m_Room[nDestRoom].PlaceItem(sDest.nX, sDest.nY, nItem, w, h))
    {
        m_Room[nSrcRoom].PlaceItem(sSrc.nX, sSrc.nY, nItem, w, h);
        return FALSE;
    }

    int idx = FindSame(nItem);
    if (idx <= 0)
    {
        return FALSE;
    }

    m_Items[idx].nPlace = sDest.nPlace;
    m_Items[idx].nX = sDest.nX;
    m_Items[idx].nY = sDest.nY;

    // Update UI
    KUiObjAtContRegion srcRegion, destRegion;
    srcRegion.Obj.uGenre = CGOG_ITEM; srcRegion.Obj.uId = nItem;
    srcRegion.Region.Width = w; srcRegion.Region.Height = h;
    srcRegion.Region.h = sSrc.nX; srcRegion.Region.v = sSrc.nY;
    srcRegion.eContainer = (UIOBJECT_CONTAINER)MapPlaceToUIContainer(sSrc.nPlace);

    destRegion = srcRegion;
    destRegion.Region.h = sDest.nX; destRegion.Region.v = sDest.nY;
    destRegion.eContainer = (UIOBJECT_CONTAINER)MapPlaceToUIContainer(sDest.nPlace);

    CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&srcRegion, 0);
    CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&destRegion, 1);
    return TRUE;
}
#endif
#ifdef _SERVER
void KItemList::SyncItem(int nIdx, BOOL bIsNew, int nPlace, int nX, int nY, int nPlayerIndex)
{

    ITEM_SYNC sItem;
    sItem.ProtocolType = s2c_syncitem;
    sItem.m_ID         = Item[nIdx].GetID();
    sItem.m_Genre      = Item[nIdx].GetGenre();
    sItem.m_Detail     = Item[nIdx].GetDetailType();
    sItem.m_Level      = Item[nIdx].GetLevel();
    sItem.m_Series     = Item[nIdx].GetSeries();
    sItem.m_Place      = (BYTE)nPlace;
    sItem.m_X          = (BYTE)nX;
    sItem.m_Y          = (BYTE)nY;
    sItem.m_Luck       = Item[nIdx].m_GeneratorParam.nLuck;

    for (int i = 0; i < 6; ++i)
        sItem.m_MagicLevel[i] = (BYTE)Item[nIdx].m_GeneratorParam.nGeneratorLevel[i];

    sItem.m_Version     = Item[nIdx].m_GeneratorParam.nVersion;
    sItem.m_Durability  = Item[nIdx].GetDurability();
    sItem.m_RandomSeed  = Item[nIdx].m_GeneratorParam.dwRandomSeed;
    sItem.m_Record      = Item[nIdx].GetRecord();
    sItem.m_BindState   = Item[nIdx].GetBindState();
    sItem.m_ExpiredTime = Item[nIdx].GetTime();
    sItem.m_ShopPrice   = Item[nIdx].GetPlayerShopPrice();

    int netIdx = (nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
                 ? Player[nPlayerIndex].m_nNetConnectIdx
                 : Player[m_PlayerIdx].m_nNetConnectIdx;

    g_pServer->PackDataToClient(netIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));

}
#endif // !_SERVER