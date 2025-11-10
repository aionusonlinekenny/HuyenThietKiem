
#ifndef	KBasPropTblH
#define	KBasPropTblH

#define		SZBUFLEN_0	80		
#define		SZBUFLEN_1	256		

#define		MAX_MAGIC_PREFIX	20
#define		MAX_MAGIC_SUFFIX	20

//======================
//
//======================
typedef struct
{
	int			nAttrib;
	int			nValue;
	int			nTime;
} KMEDATTRIB;
//======================
//
//======================
typedef struct
{
	int			nMin;
	int			nMax;
} KMINMAXPAIR;
//======================
//
//======================
typedef struct
{
	int			nType;						
	KMINMAXPAIR	sRange;						
} KEQCP_BASIC;
//======================
//
//======================
typedef struct
{
	int			nType;						
	int			nPara;						
} KEQCP_REQ;
//======================
//
//======================
typedef struct
{
	int			nPropKind;					
	KMINMAXPAIR	aryRange[3];				
} KMACP;	


#define			MATF_CBDR		    11
#define         MATF_PREFIXPOSFIX   2       
#define         MATF_SERIES         5       
#define         MATF_LEVEL          10    
  
//======================
//
//======================
typedef struct
{
	int			m_nPos;						
	char		m_szName[SZBUFLEN_0];		
	int			m_nClass;					
	int			m_nLevel;					
	char		m_szIntro[SZBUFLEN_1];		
	KMACP		m_MagicAttrib;				
	int			m_DropRate[MATF_CBDR];		
    int         m_nUseFlag;                 
} KMAGICATTRIB_TABFILE;
//===================================
//
//===================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;			
	int			m_nDetailType;				
	int			m_nParticularType;
	int			m_nLevel;
	int			m_nSeries;
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;					
	char		m_szIntro[SZBUFLEN_1];							
	int			m_nPrice;					
	KEQCP_BASIC	m_aryPropBasic[7];			
	KEQCP_REQ	m_aryPropReq[6];			
} KBASICPROP_EQUIPMENT;
//===================================
//
//===================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;				
	int			m_nDetailType;				
	int			m_nParticularType;			
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;					
	char		m_szIntro[SZBUFLEN_1];		
	int			m_nSeries;					
	int			m_nPrice;					
	int			m_nLevel;								
	int			m_nSet;						
	int			m_nSetNum;					
	int			m_nSetId;
	KEQCP_BASIC	m_aryPropBasic[7];			
	KEQCP_REQ	m_aryPropReq[6];			
	int			m_aryMagicAttribs[6];			
} KBASICPROP_GOLD_EQUIPMENT;
//===================================
//
//==================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;			
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;			
} KBASICPROP_BROKEN_EQUIPMENT;
//===================================
//
//===================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;				
	int			m_nDetailType;				
	int			m_nParticularType;
	int			m_nLevel;
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;					
	char		m_szIntro[SZBUFLEN_1];					
	int			m_nPrice;					
	KMEDATTRIB	m_aryAttrib[3];				
} KBASICPROP_MEDICINE;
//==================================
//	
//==================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;				
	int			m_nDetailType;
	int			m_nParticularType;
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;	
	int			m_nMaxStack;			
	char		m_szIntro[SZBUFLEN_1];
} KBASICPROP_QUEST;
//==================================
//	
//==================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;				
	int			m_nDetailType;
	int			m_nParticularType;	
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;
	int			m_nMaxStack;
	int			m_nTaskIdScript;//nhim
	int			m_nTaskIdScriptMax;//nhim
	char		m_szIntro[SZBUFLEN_1];
	char		m_szScriptFile[SZBUFLEN_0];
} KBASICPROP_SCRIPT;
//==================================
//	
//==================================
typedef struct
{
	char		m_szName[SZBUFLEN_0];		
	int			m_nGenre;				
	int			m_nDetailType;
	int			m_nParticularType;
	char		m_szIcon[SZBUFLEN_0];	
	int			m_nObjID;					
	int			m_nWidth;					
	int			m_nHeight;	
	char		m_szIntro[SZBUFLEN_1];	
	int			m_nMaxStack;	
} KBASICPROP_MINE;
//===============================================
//
//===============================================
class KBasicPropertyTable
{
public:
	KBasicPropertyTable();
	~KBasicPropertyTable();

protected:
	void*		m_pBuf;														
	int			m_nNumOfEntries;			
    int         m_nSizeOfEntry;				
	char		m_szTabFile[MAX_PATH];

public:
	virtual BOOL Load();
	int NumOfEntries() const { return m_nNumOfEntries; }

protected:
	BOOL GetMemory();
	void ReleaseMemory();
	void SetCount(int);
	virtual BOOL LoadRecord(int i, KTabFile* pTF) = 0;
};

//===============================================
//
//===============================================
class KBPT_MagicAttrib_TF : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib_TF();
	~KBPT_MagicAttrib_TF();

protected:
	int m_naryMACount[2][MATF_CBDR];
										
public:
	void GetMACount(int*) const;
	const KMAGICATTRIB_TABFILE* GetRecord(IN int) const;
	const KMAGICATTRIB_TABFILE* FindRecord(IN int, IN int, OUT int*) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
	void Init();
};

//===============================================
//
//===============================================
class KBPT_ClassMAIT
{
public:
	KBPT_ClassMAIT();
	~KBPT_ClassMAIT();

protected:
	int*	m_pnTable;				
	int		m_nSize;
	int		m_nNumOfValidData;		
								
public:
    BOOL Clear();
	BOOL Insert(int nItemIndex);
	int  Get(int i) const;
    int  GetCount() const { return m_nNumOfValidData; }
};

//===============================================
//
//===============================================
class KBPT_ClassifiedMAT
{
public:
	KBPT_ClassifiedMAT();
	~KBPT_ClassifiedMAT();

protected:
	int*	m_pnTable;	
	int		m_nSize;				
	int		m_nNumOfValidData;
									
public:
	BOOL GetMemory(int);
	BOOL Set(int);
	int Get(int) const;
	BOOL GetAll(int*, int*) const;

protected:
	void ReleaseMemory();
};

//===============================================
//
//===============================================
class KBPT_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Equipment();
	~KBPT_Equipment();
	void Init(IN int);

public:
	const KBASICPROP_EQUIPMENT* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT* FindRecord(IN int, IN int, IN int) const;
	

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
// --
//
// --
class KBPT_Gold_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Gold_Equipment();
	~KBPT_Gold_Equipment();

public:
	const KBASICPROP_GOLD_EQUIPMENT* GetRecord(IN int) const;
	const KBASICPROP_GOLD_EQUIPMENT* FindRecord(IN int, IN int, IN int) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
// --
//
// --
class KBPT_Broken_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Broken_Equipment();
	~KBPT_Broken_Equipment();
public:
	const KBASICPROP_BROKEN_EQUIPMENT* GetRecord(IN int) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
//===============================================
//
//===============================================
class KBPT_Medicine : public KBasicPropertyTable
{
public:
	KBPT_Medicine();
	~KBPT_Medicine();

public:
	const KBASICPROP_MEDICINE* GetRecord(IN int) const;
	const KBASICPROP_MEDICINE* FindRecord(IN int, IN int) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
//===============================================
//
//===============================================
class KBPT_Quest : public KBasicPropertyTable
{
public:
	KBPT_Quest();
	~KBPT_Quest();

public:
	const KBASICPROP_QUEST* GetRecord(IN int) const;
	const KBASICPROP_QUEST* FindRecord(IN int, OUT int*) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
//===============================================
//
//===============================================
class KBPT_Script : public KBasicPropertyTable
{
public:
	KBPT_Script();
	~KBPT_Script();

public:
	const KBASICPROP_SCRIPT* GetRecord(IN int) const;
	const KBASICPROP_SCRIPT* FindRecord(IN int, OUT int*) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
//===============================================
//
//===============================================
class KBPT_Mine : public KBasicPropertyTable
{
public:
	KBPT_Mine();
	~KBPT_Mine();

public:
	const KBASICPROP_MINE* GetRecord(IN int) const;
	const KBASICPROP_MINE* FindRecord(IN int, OUT int*) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};
//===========================================
//
//===========================================
class KLibOfBPT
{
public:
	KLibOfBPT();
	~KLibOfBPT();

protected:
	//
	KBPT_Equipment			m_BPTMeleeWeapon;
	KBPT_Equipment			m_BPTRangeWeapon;
	KBPT_Equipment			m_BPTArmor;
	KBPT_Equipment			m_BPTHelm;
	KBPT_Equipment			m_BPTBoot;
	KBPT_Equipment			m_BPTBelt;
	KBPT_Equipment			m_BPTAmulet;
	KBPT_Equipment			m_BPTRing;
	KBPT_Equipment			m_BPTCuff;
	KBPT_Equipment			m_BPTPendant;
	KBPT_Equipment			m_BPTHorse;
	KBPT_Equipment			m_BPTMask;
	KBPT_Equipment			m_BPTMantle;
	//
	KBPT_Gold_Equipment		m_BPTGoldEquip;
	//
	KBPT_Broken_Equipment	m_BPTBrokenEquip;
	//
	KBPT_Medicine			m_BPTMedicine;
	//
	KBPT_Quest				m_BPTQuest;
	//
	KBPT_Script				m_BPTScript;
	//
	KBPT_Mine				m_BPTMine;
	//
    KBPT_MagicAttrib_TF		m_BPTMagicAttrib;
	  
    KBPT_ClassMAIT          m_CMAIT[MATF_PREFIXPOSFIX][MATF_CBDR][MATF_SERIES][MATF_LEVEL];

	KBPT_ClassifiedMAT		m_CMAT[2][MATF_CBDR];

public:
	BOOL Init();

	const KMAGICATTRIB_TABFILE* FindMARecord(IN int, IN int, OUT int*) const;
	const KMAGICATTRIB_TABFILE* GetMARecord(IN int) const;
	const int					GetMARecordNumber() const;
	//
    const KBPT_ClassMAIT*       GetCMIT(IN int nPrefixPostfix, IN int nType, IN int nSeries, int nLevel) const;
	const KBPT_ClassifiedMAT*	GetCMAT(IN int, int) const;
	//
	const KBASICPROP_EQUIPMENT*	GetMeleeWeaponRecord(IN int) const;
	const int					GetMeleeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRangeWeaponRecord(IN int) const;
	const int					GetRangeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetArmorRecord(IN int) const;
	const int					GetArmorRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetHelmRecord(IN int) const;
	const int					GetHelmRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetBootRecord(IN int) const;
	const int					GetBootRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetBeltRecord(IN int) const;
	const int					GetBeltRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetAmuletRecord(IN int) const;
	const int					GetAmuletRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRingRecord(IN int) const;
	const int					GetRingRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetCuffRecord(IN int) const;
	const int					GetCuffRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetPendantRecord(IN int) const;
	const int					GetPendantRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHorseRecord(IN int) const;
	const int					GetHorseRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMaskRecord(IN int) const;
	const int					GetMaskRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMantleRecord(IN int) const;
	const int					GetMantleRecordNumber() const;
	//
	const KBASICPROP_GOLD_EQUIPMENT*	GetGoldEquipRecord(IN int nIndex) const;
	const int							GetGoldEquipRecordNumber() const;
	//
	const KBASICPROP_BROKEN_EQUIPMENT*	GetBrokenEquipRecord(IN int) const;
	const int							GetBrokenEquipRecordNumber() const;
	//
	const KBASICPROP_MEDICINE*	GetMedicineRecord(IN int) const;
	const int					GetMedicineRecordNumber() const;
	//
	const KBASICPROP_QUEST*		FindQuestRecord(IN int, OUT int*) const;
	const KBASICPROP_QUEST*		GetQuestRecord(IN int) const;
	const int					GetQuestRecordNumber() const;
	//
	const KBASICPROP_SCRIPT*	FindScriptRecord(IN int, OUT int*) const;
	const KBASICPROP_SCRIPT*	GetScriptRecord(IN int) const;
	const int					GetScriptRecordNumber() const;
	//
	const KBASICPROP_MINE*		FindMineRecord(IN int, OUT int*) const;
	const KBASICPROP_MINE*		GetMineRecord(IN int) const;
	const int					GetMineRecordNumber() const;

protected:
	BOOL InitMALib();
    BOOL InitMAIT();
};
#endif		// #ifndef KBasPropTblH
