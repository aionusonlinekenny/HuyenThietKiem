#ifndef	KBuySellH
#define	KBuySellH

class KItem;

typedef struct
{
	int		nGenre;
	int		nDetailType;
	int		nParticularType;
	int		nSeriesReq;
	int		nLevel;
	int		nPrice;
	int		nStackNum;
} ItemGenerate;

class KInventory;

class KBuySell
{
public:
	KBuySell();
	~KBuySell();
	BOOL			Init();
	int				GetWidth() { return m_Width; }
	int				GetHeight() { return m_Height; }
	KItem*			GetItem(int nIndex);
	int				GetItemIndex(int nShop, int nIndex);
private:
	int**			m_SellItem;
	KItem*			m_Item;
	int				m_Width;
	int				m_Height;
	int				m_MaxItem;
#ifndef _SERVER
	BYTE			m_MarketHeight;
	int*			m_MarketID;
	char**			m_MarketName;
#endif
public:
#ifdef _SERVER
	void			OpenSale(int nPlayerIdx, int nShop);
	BOOL			Buy(int nPlayerIdx, int nBuy, int nBuyIdx, int nPlace, int nX, int nY);
	BOOL			Sell(int nPlayerIdx, int nBuy, int nIdx);
#endif
#ifndef _SERVER
	KInventory*		m_pShopRoom;
	void			OpenSale(int nShop);
	void			PaintItem(int nIdx, int nX, int nY);
#endif
#ifndef _SERVER
	inline BYTE		GetMarketHeight() const { return m_MarketHeight; };
	inline int		GetMarketID(int nMarketIdx) const { return m_MarketID[nMarketIdx]; }
	inline char*	GetMarketName(int nMarketIdx) const { return m_MarketName[nMarketIdx]; }
#endif
};

extern KBuySell	BuySell;
#endif
