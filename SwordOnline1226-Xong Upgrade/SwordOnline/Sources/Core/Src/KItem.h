#ifndef	KItemH
#define	KItemH

#include	"KBasPropTbl.h"
#include	"KMagicAttrib.h"
#include	"GameDataDef.h"
#ifndef _SERVER
#include	 "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#include	"KTabFile.h"
class KPlayer;
class KIniFile;

#define		IN
#define		OUT

#ifdef _SERVER
#define		MAX_ITEM	50000//15000
#else
#define		MAX_ITEM	1024
#endif
#define		KItemNormalAttrib KMagicAttrib

enum MEDICINEDETAILTYPE
{
	medicine_blood = 0,
	medicine_mana,
	medicine_both,
	medicine_stamina,
	medicine_antipoison,
	medicine_detailnum,
};

//==================================
//	
//==================================
typedef struct
{
	char	cGenre;
	short	nDetailType;
	short	nParticularType;
	BYTE	bLevel;
	char	cSeries;
	WORD	wObjID;
	BYTE	bWidth;
	BYTE	bHeight;
	int		nPrice;
	int		nTaskIdScript;//nhim
	int		nTaskIdScriptMax;//nhim
	char	szItemName[80];
#ifndef _SERVER
	char	szImageName[80];
	char	szIntro[256];
#endif
} KItemCommonAttrib;

//==================================
//	
//==================================
typedef struct
{
	DWORD	dwRandomSeed;
	int		nGeneratorLevel[6];
	int		nVersion;
	int		nLuck;
} KItemGeneratorParam;

class KNpc;

class KItem
{
public:
	KItem();
	~KItem();

private:
	KItemCommonAttrib	m_CommonAttrib;		
	KItemNormalAttrib	m_aryBaseAttrib[7];		
	KItemNormalAttrib	m_aryRequireAttrib[6];	
	// KItemNormalAttrib	m_aryMagicAttrib[6];
	KItemNormalAttrib	m_aryExtraMagicAttrib[2];
	KItemGeneratorParam	m_GeneratorParam;
	//						
	DWORD	m_dwID;								
	int		m_nCurrentDur;
	//
	WORD	m_wRecord;
	//
	DWORD	m_dwBindState;
	//
	DWORD	m_dwExpiredTime;

	BYTE	m_btPriceType;
	BYTE	m_btDisCountPrice;
	BYTE	m_btDaySell;
	int		m_nShopPrice;
	//
	int		m_nParam1;
	//
	DWORD	m_dwScriptID;

#ifndef _SERVER
	KRUImage	m_Image;
	BOOL		m_bHotSell;
#endif
public: 
	KItemNormalAttrib	m_aryMagicAttrib[6];//AutoAI by kinnox;
private:
	inline	void	Release();

public:
	//
		int		GetGoldId() {if(m_CommonAttrib.cGenre == item_goldequip) return m_dwID; else return 0;}//AutoAI by kinnox;
	void	Remove();
	//
	inline	void	SetID(const DWORD dwID) { m_dwID = dwID; }
	inline	DWORD	GetID() const { return m_dwID; }
	//
	void	ApplyMagicAttribToNPC(IN KNpc*, IN int = 0) const;
	void	RemoveMagicAttribFromNPC(IN KNpc*, IN int = 0) const;
	void	ApplyHiddenMagicAttribToNPC(IN KNpc*, IN int) const;
	void	RemoveHiddenMagicAttribFromNPC(IN KNpc*, IN int) const;
	//
	inline	void	SetRandomSeed(const DWORD dwRand) { m_GeneratorParam.dwRandomSeed = dwRand; }
	void			SetGeneratorLevel(const int* pnaryMALevel);
	inline	void	SetGeneratorLuck(const int nLuck) { m_GeneratorParam.nLuck = nLuck; }
	inline	void	SetGeneratorVersion(const int nVersion) { m_GeneratorParam.nVersion = nVersion; }
	inline	KItemGeneratorParam * GetGeneratorParam(){return &m_GeneratorParam;};
	
	//
	inline	void	SetGenre(const int nValue) { m_CommonAttrib.cGenre = (char)nValue;	}
	inline	char	GetGenre() const { return m_CommonAttrib.cGenre; }
	//
	inline	void	SetRecord(const int nValue)	{ m_wRecord = (WORD)nValue; }
	inline	WORD	GetRecord()	const { return m_wRecord; }
	//
	inline	void	SetDetailType(const int nValue) { m_CommonAttrib.nDetailType = (short)nValue; }
	inline short	GetDetailType() const { return m_CommonAttrib.nDetailType; };
	//
	inline	void	SetParticular(const int nValue) { m_CommonAttrib.nParticularType = (short)nValue; }
	inline	short	GetParticular() { return m_CommonAttrib.nParticularType; }
	//
	inline	void	SetLevel(const int nValue) { m_CommonAttrib.bLevel = (BYTE)nValue; }
	inline	BYTE	GetLevel() const { return m_CommonAttrib.bLevel; }
	//
	inline	void	SetSeries(const int nValue) { m_CommonAttrib.cSeries = (char)nValue; }
	inline	char	GetSeries() const { return m_CommonAttrib.cSeries; }
	//
	inline	WORD	GetObjID() { return m_CommonAttrib.wObjID; }
	//
	inline	BYTE	GetWidth() const { return m_CommonAttrib.bWidth; }
	inline	BYTE	GetHeight() const { return m_CommonAttrib.bHeight; }

	void			SetPrice(int nPrice, BYTE btType);
	int				GetPrice(BOOL bRuled = FALSE, const char* lpszTongName = NULL, int nSubWorldId = 0) const;

	inline	char*	GetName() const { return (char*)m_CommonAttrib.szItemName; };
	//
	int				GetSuiteNo() const;
	int				GetSuiteId() const;
	int				GetSuiteCount() const;
	int				GetParam1() const { return m_nParam1; };
	//
	void*	GetRequirement(IN int);
	int		GetMaxDurability();
	int		GetTotalMagicLevel();
	int		GetRepairPrice();

	

	BOOL	SetBaseAttrib(IN const KItemNormalAttrib*);
	BOOL	SetRequireAttrib(IN const KItemNormalAttrib*);
	BOOL	SetMagicAttrib(IN const KItemNormalAttrib*);
	inline	void	SetDurability(IN const int nDur) { m_nCurrentDur = nDur; };
	inline	int		GetDurability() const { return m_nCurrentDur; };
	int		Abrade(IN const int nRange);
	BOOL	CanBeRepaired();
	inline int	GetAttribType(int i = 0) const 
	{	
		if(i < 0 || i >= 6)
			return 0;

		return m_aryMagicAttrib[i].nAttribType; 
	};

	inline int	GetAttribValue(int i = 0) const 
	{	
		if(i < 0 || i >= 6)
			return 0;

		return m_aryMagicAttrib[i].nValue[0]; 
	};
	BOOL			CanStack();
	inline	int		GetMaxStackCount() const { return m_aryBaseAttrib[0].nValue[0]; }
	inline	void	SetStackCount(const int nValue) { m_nCurrentDur = nValue; }
	inline	int		GetStackCount() const { return m_nCurrentDur; }
	void			SetBindState(const DWORD dwState, BOOL bSetTime = FALSE);
	inline	DWORD	GetBindState() const { return m_dwBindState; };
	void			SetTime(const DWORD dwTime, BOOL bBind = FALSE);
	DWORD			GetTime(BOOL bBind = FALSE) const;
	BOOL			DecodeTime(int* pnHour, int* pnMin, int* pnDate, int* pnMonth, int* pnYear, BOOL bBind = FALSE);
	void	SetDisCountPrice(BYTE btDisCount) { m_btDisCountPrice = btDisCount; };
	inline	BYTE GetDisCountPrice() const { return m_btDisCountPrice;};
	inline BYTE GetPriceType() const { return m_btPriceType; };
	void	SetDaySell(BYTE btDay) { m_btDaySell = btDay; };
	inline BYTE GetDaySell() const { return m_btDaySell; };
	inline int	GetBaseMagic()  const { return m_aryBaseAttrib[0].nValue[0];};
	void	SetPlayerShopPrice(int nPrice);
	inline int	GetPlayerShopPrice() const { return m_nShopPrice; };
	int	GetMagicAttrib(BYTE btNo, BYTE btType) const;
	inline	DWORD	GetScriptID() const { return m_dwScriptID; }
	void	SetBrokenEquip(IN const KBASICPROP_BROKEN_EQUIPMENT *pBroken);
	int		GetBaseMagicAttribType(int i = 0)  const { return m_aryBaseAttrib[i].nAttribType; }; //AutoAI by kinnox;
#ifndef _SERVER
	void	Paint(int nX, int nY, BOOL bStack);
	void	GetDesc(char* pszMsg, bool bShowPrice = false, int nPriceScale = 1, int nActiveAttrib = 0);
	void	GetSuiteDesc(char* pszSuite);
	void	GetTimeDesc(char* pszTime, BOOL bBind = FALSE);
	char*	GetImageName() const { return (char*)m_CommonAttrib.szImageName; };
	char*	GetRequireSeries(char cSeries) const;
	void	GetPriceDesc(char cDisCount, int nPriceScale, const char* pszUnit, const char* pszColor, char* pszPrice) const;
	void	SetHotSell(BOOL bFlag) { m_bHotSell = bFlag; };
	BOOL	GetHotSell() const { return m_bHotSell; };
#else
	void			GetItemBackupInfo(OUT char* czInfo);
	void			RestoreItemInfo(IN char* czInfo);
	BOOL			CheckVaildTime(BOOL bBind = FALSE);
#endif


friend class	KItemGenerator;
friend class	KPlayer;
friend class	KItemList;
private:
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT*);
	BOOL SetAttrib_CBR(IN const KBASICPROP_GOLD_EQUIPMENT*);
	BOOL SetAttrib_MA(IN const KItemNormalAttrib*);
	BOOL SetAttrib_MA(IN const KMACP*);
	BOOL SetAttrib_ExtraMA(IN const KItemNormalAttrib*);
	void operator = (const KBASICPROP_EQUIPMENT&);
	void operator = (const KBASICPROP_GOLD_EQUIPMENT&);
	void operator = (const KBASICPROP_BROKEN_EQUIPMENT&);
	void operator = (const KBASICPROP_MEDICINE&);
	void operator = (const KBASICPROP_QUEST&);
	void operator = (const KBASICPROP_SCRIPT&);
	void operator = (const KBASICPROP_MINE&);

private:
	BOOL SetAttrib_Base(const KEQCP_BASIC*);
	BOOL SetAttrib_Req(const KEQCP_REQ*);
};

extern KItem Item[MAX_ITEM];

#endif
