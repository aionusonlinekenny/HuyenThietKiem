#ifndef KItemSetH
#define	KItemSetH

#include "KLinkArray.h"

#define		IN
#define		OUT
class	KItem;

enum enumAbrade
{
	enumAbradeAttack = 0,
	enumAbradeDefend,
	enumAbradeMove,
	enumAbradeNum,
};

typedef struct
{
	int		m_nIdx;
	int		m_nX;
	int		m_nY;
	int		m_nWidth;
	int		m_nHeight;
} TRADE_ITEM_INFO;

typedef struct
{
	int		nPriceScale;
	int		nMagicScale;
} REPAIR_ITEM_PARAM;

class KItemSet
{
private:
	BYTE			m_bLock;
	DWORD			m_dwIDCreator;
	KLinkArray		m_FreeIdx;			
	KLinkArray		m_UseIdx;
	
public:
	int				m_nItemAbradeRate[enumAbradeNum][itempart_num];
	REPAIR_ITEM_PARAM	m_sRepairParam;

#ifdef _SERVER
	TRADE_ITEM_INFO		*m_psItemInfo;
	TRADE_ITEM_INFO		*m_psBackItemInfo;
	void				BackItemInfo();
	PlayerItem			m_sLoseItemFromEquipmentRoom[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	PlayerItem			m_sLoseEquipItem[itempart_num];
#endif

	//
	int			m_nCoinID;

public:
	KItemSet();
	~KItemSet();
	void			Init();
	int				SearchID(IN DWORD dwID);
	int				Add(KItem* pItem);
	int				Add(IN int nItemGenre, IN int nSeries, IN int nLevel, IN WORD wRecord, IN int nLuck, IN int nDetail = -1, IN int nParticular = -1, IN int* pnMagicLevel = NULL, 
						IN int nVersion = 0, IN DWORD dwRandomSeed = 0, IN BYTE btStackNum = 1, IN DWORD dwExpiredTime = 0);
	int				AddExist(IN int nItemGenre, IN int nSeries, IN int nLevel, IN WORD wRecord, IN int nLuck, IN int nDetail = -1, IN int* pnMagicLevel = NULL, 
							IN int nVersion = 0, IN DWORD dwRandomSeed = 0);
	void			Remove(IN int nIdx);
	int				GetAbradeRange(IN int nType, IN int nPart);
	int				GetItemNumber() {return m_UseIdx.GetCount(); };
#ifdef _SERVER
	int			DropItem(int nSubWorldIdx, int nMpsX, int nMpsY, int nBelongPlayerIdx, int nItemGenre, int nItemDetail, int nItemParticular,
								int nItemLevel, int nItemSeries, int nLuck, int* pnMagicLevel = NULL, int nExpiredTime = 0);
#endif

private:
#ifdef _SERVER
	BOOL			SetID(IN int nIdx);
#endif
	int				FindFree();
	
};

extern KItemSet	ItemSet;
#endif
