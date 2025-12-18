#ifndef	KItemListH
#define	KItemListH

#include "KLinkArray.h"
#include "KItem.h"
#define	MAX_ITEM_ACTIVE	2

#define	REQUEST_EQUIP_ITEM		1
#define	REQUEST_EAT_MEDICINE	2
#define	REQUEST_EAT_OTHER		3
#define	REQUEST_EQUIP_ITEM_TO_EQUIPROOM		4
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY		5
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY1		6
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY2		7
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY3		8
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY4		9
#define	REQUEST_EQUIP_ITEM_TO_REPOSITORY5		10
#ifndef _SERVER
extern bool g_RightMoveBusy;   // ch? khai báo (extern)
#endif
typedef struct
{
	int		nPlace;
	int		nX;
	int		nY;
} ItemPos;


class CORE_API KItemList
{
private:
	int			m_PlayerIdx;
	int			m_nListCurIdx;
	int			m_Hand;
	int			m_nBackHand;
	PlayerItem	m_Items[MAX_PLAYER_ITEM];						
	PlayerItem	m_sBackItems[MAX_PLAYER_ITEM];
	//
	static int	ms_RoomMap[room_num];
	//
	static int	ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE];
	static int	ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE];
	//
	KLinkArray	m_FreeIdx;
	KLinkArray	m_UseIdx;
	//
	int**		m_GoldInfo;
	//
	BYTE		m_bActiveSet;
	BYTE		m_bMaskLock;
	//
	int			m_BuildItem[MAX_PART_BUILD];//TrembleItem by kinnox;
	//	
#ifndef _SERVER
	BYTE		m_bLockOperation;
#endif

public:
	int			m_EquipItem[itempart_num];//AutoAI by kinnox;
	KInventory	m_Room[room_num];


private:
	int			FindFree();
	int			FindSame(int nGameIdx);
	int			GetEquipPlace(int nType);						
	BOOL		Fit(int nIdx, int nPlace);						
	BOOL		Fit(KItem* pItem, int nPlace);
	//
	int			GetEquipEnhance(int nPlace);					
	int			GetActiveEquipPlace(int nPlace, int nCount);
	void		InfectionNextEquip(int nPlace, BOOL bEquip = FALSE);
	//
	BOOL		CheckSameDetailType(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType);
	//
	BOOL		FindSameDetailType(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, int *pnIdx, int *pnX, int *pnY);
	//
	void		UpdateGoldInfo(const int nPlace, const int nSuiteNo, int nSuiteId, int nSuiteCount);

#ifdef _SERVER
	BOOL		FindSameItem(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, const int nParticularType, const int nSeries, const int nLevel, int *pnIdx, int *pnX, int *pnY);
	//
	BOOL		CheckStackItem(const int nIdxBeStack, const int nIdxForStack);
	int			FindItemToStack(INVENTORY_ROOM eRoom, const int nGameIdx);
	int			StackItem(const int nIdxBeStack, const int nIdxForStack);
	BOOL		ExchangeStack(const int nIdxBeStack, const int nIdxForStack);
#endif
	friend	class KPlayer;

public:
	KItemList();
	~KItemList();
	int			Init(const int nPlayerIdx);
	void		Release();
	int			PositionToRoom(int nPlace);
	//
	int			CalcFreeItemCellCount(int nWidth, int nHeight, int nRoom);	//AutoAI by kinnox;
	//
	int			GetBuildItem(int nIdx) { return m_BuildItem[nIdx];}	 //TrembleItem by kinnox;
	BOOL		BuildItem(int nIdx, int nPlace = -1);	//TrembleItem by kinnox;
	void		UnBuildItem(int nIdx, int nPlace = -1);	//TrembleItem by kinnox;
	//
	int			GetEquipment(int nIdx) const { return m_EquipItem[nIdx]; }
	//
	int			GetActiveAttribNum(int nIdx);
	//
	int			GetWeaponType();
	int			GetWeaponParticular();
	void		GetWeaponDamage(int* nMin, int* nMax);
	//
	int			Add(int nIdx, int nPlace, int nX, int nY);		
	BOOL		Remove(int nIdx);								
	void		RemoveAll();
	//
	BOOL		CanEquip(int nIdx, int nPlace = -1);			
	BOOL		CanEquip(KItem* pItem, int nPlace = -1);
	//
	int			HaveDamageItem(int nDur); //AutoAI by kinnox;
	//
	BOOL		EnoughAttrib(void* pData);
	//
	BOOL		Equip(int nIdx, int nPlace = -1);				
	BOOL		UnEquip(int nIdx, int nPlace = -1);
	//
	BOOL		CheckTrembleItem(int nIdx, int nPlace = -1);	
	BOOL		UnTrembleItem(int nIdx, int nPlace = -1);
	int			GetTrembleItem(int nIdx = -1) const;
	//
	BOOL		CheckFoundryItem(int nIdx, int nPlace = -1);	
	BOOL		UnFoundryItem(int nIdx, int nPlace = -1);
	int			GetFoundryItem(int nPlace = -1) const;
	//
	BOOL		EatMecidine(const int nGameIdx);							
	PlayerItem*	GetFirstItem();
	PlayerItem*	GetNextItem();
	//
	int			SearchID(int nID);
	void		ExchangeMoney(int nSrcRoom, int DesRoom, int nMoney);
	void		ExchangeItem(ItemPos* SrcPos,ItemPos* DesPos);
	//
	int			GetMoneyAmount();					
	int			GetEquipmentMoney();	
	int			GetTradeMoney();//fix trade money by kinnox;	
	int			GetRepositoryMoney();//AutoAI by kinnox;			
	BOOL		AddMoney(int nRoom, int nMoney);
	BOOL		CostMoney(int nMoney);
	BOOL		DecMoney(int nMoney);
	void		SetMoney(int nMoney1, int nMoney2, int nMoney3);
	void		SetRoomMoney(int nRoom, int nMoney);
	int			GetMoney(int nRoom) { return m_Room[nRoom].GetMoney(); }
	void		SetPlayerIdx(int nIdx);
	int			Hand() { return m_Hand; };
	void		ClearRoom(int nRoom);
	void		BackupTrade();
	void		RecoverTrade();
	void		StartTrade();
	//
	BOOL		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);
	//
	BOOL		GetIfActive();
	BOOL		GetMaskLock() {return m_bMaskLock;};
	void		SetMaskLock(BOOL bFlag);
	//
	int			IsMyItem(int nIndex);
	int			CountSameItem(INVENTORY_ROOM eRoom, const int nGenre, const int nDetailType, const int nParticularType, const int nSeries, const int nLevel);
	int			GetSameItemNumInEquipment(const int nGenre, const int nDetailType, const int nParticularType, const int nSeries, const int nLevel);
	//
	int			MapPlaceToUIContainer(int nPlace);
	BOOL		HandleAutoMoveFromServer(ItemPos sSrc, ItemPos sDest, int nItemID);
	
#ifdef	_SERVER
	void		Abrade(int nType);
	void		TradeMoveMoney(int nMoney);
	void		SendMoneySync();
	BOOL 		AutoMoveToRepository(int nItemID /*=0*/, int nSrcX, int nSrcY, int nSrcPlace, int nDestPlace);
    BOOL 		AutoMoveToInventory (int nItemID /*=0*/, int nSrcX, int nSrcY, int nSrcPlace, int nDestPlace);
	//
	BOOL		RemoveByIndex(const int nGameIdx, BOOL bClient = FALSE);
	//
	BOOL		IsTaskItemExist(short nDetailType);
	//
	int			GetTaskItemNum(const int nDetailType);
	//
	BOOL		RemoveTaskItem(const int nDetailType, int nCount = 1);
	//
	BOOL		UseStackItem(const int nGameIdx, const int nCount = 1);
	//
	BOOL		RemoveStackItem(const int nGameIdx);
	//
	void		SyncItem(int nIdx, BOOL bIsNew = FALSE, int nPlace = 0, int nX = 0, int nY = 0, int nPlayerIndex = 0);
	void		GetTradeRoomItemInfo();
	BOOL		TradeCheckCanPlace();
	//
	BOOL		FindEmptyPlaceEquipment(BYTE btWidth, BYTE btHeight);
	int			GetEmptyCellNumInEquipment();
	//
	BOOL		RemoveItemInEquipment(const int nCount, const int nGenre, const int nDetailType, const int nPaticularType, const int nSeries, const int nLevel);
	//
	BOOL		CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY);
	//
	BOOL		EatMecidine(int nPlace, int nX, int nY);		
	BOOL		AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY);
	void		AutoLoseItemFromEquipmentRoom(int nRate);
	void		AutoLoseEquip();
	//
	void		SetItemBindState(int nIndex, BYTE btState, BYTE btDay);
	char		GetItemBindState(int nIndex);
	void		SetMarkPriceItem(DWORD dwItemID, int nPrice);
	void		AutoMoveItem(int nPos);
	
	int			Add_AutoStack(INVENTORY_ROOM eRoom, const int nGameIdx, const int nX, const int nY);
	//
	int			GetItemCount() { return m_UseIdx.GetCount(); }
	//
	BOOL		BackupBrokenEquip(const int nGameIdx);
	BOOL		RestoreBrokenEquip(const int nGameIdx);
	BOOL		AddAgain(const int nGameIdx);
#endif

#ifndef	_SERVER
	int			UseItem(int nIdx, BYTE btPos = 0);
	int 		SearchPlaceEquip(int nItemID,BYTE nPos);
	BOOL		AutoMoveItem(ItemPos SrcPos,ItemPos DesPos);
	void		MenuSetMouseItem();
	void		RemoveAllInOneRoom(int nRoom);
	void		LockOperation();
	void		UnlockOperation();
	BOOL		IsLockOperation() { return m_bLockOperation; };
	//
	int			GetSameDetailItemNum(int nImmediatePos);
	BYTE		GetGoldColor(short nGoldId, BYTE btId);
	
	BOOL		FindSameInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	BOOL		FindSameInImmediacy(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	
	int			GetNextItemEquipmentRoom(int nItemIdx, int nX, int nY, int *nSrcX, int *nSrcY);
	int			GetEmptyCellNumInEquipment();

	int			GetSuiteDescColor(int nRecord, int nSuiteId);

	int			FindItemInRoom(INVENTORY_ROOM eRoom, const int nX, const int nY);
	//
	int			CheckFoundryBox(const int nGameIdx, const int nPlace,  const int nMachine);
	//
	inline	int	IsActiveSet() const { return m_bActiveSet; };
	BOOL		CountCommonItem(int nGenre, int nDetailType, int P = pos_equiproom);//AutoAI by kinnox;
	BOOL		SearchEquipment(int nWidth, int nHeight);//AutoAI by kinnox;
#endif
};
#endif
