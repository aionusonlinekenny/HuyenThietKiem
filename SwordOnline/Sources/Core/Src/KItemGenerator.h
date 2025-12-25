
#ifndef	_KItemGenerator_H_
#define	_KItemGenerator_H_

#include "KBasPropTbl.h"
#include "KItem.h"

#define		IN
#define		OUT

#define	NUMOFCMA	150

class KItemGenerator
{
public:
	KItemGenerator();
	~KItemGenerator();

protected:
	KLibOfBPT	m_BPTLib;

	int			m_GMA_naryMA[2][NUMOFCMA];
	int			m_GMA_nCount[2];
	int			m_GMA_naryLevel[2][NUMOFCMA];
	int			m_GMA_naryDropRate[2][NUMOFCMA];
	int			m_GMA_naryCandidateMA[NUMOFCMA];
	//
	int			m_EquipNumOfEntries[equip_detailnum];
	//
	int			m_GoldEquipNumOfEntries;
	//
	int			m_MedNumOfEntries;
	//
	int			m_QuestNumOfEntries;
	//
	int			m_ScriptNumOfEntries;
	//
	int			m_MineNumOfEntries;

public:
	BOOL Init();
	//
	BOOL Gen_Equipment(IN int, IN int, IN int, IN int, IN const int*, IN int, IN int, IN OUT KItem*);
	BOOL Gen_ExistEquipment(IN int, IN int, IN int, IN const int*, IN int, IN int, IN OUT KItem*);
	//
	BOOL Gen_PurpleEquipment(IN int, IN int, IN int, IN int, IN const int*, IN int, IN int, IN OUT KItem*);
	BOOL Gen_ExistPurpleEquipment(IN int, IN int, IN int, IN const int*, IN int, IN int, IN OUT KItem*);
	//
	BOOL Gen_GoldEquipment(IN int, IN int, OUT KItem*);
	//
	BOOL Gen_BrokenEquipment(IN int, IN int, IN int, IN int, IN int, IN const int*, IN int, IN int, IN int, IN OUT KItem*);
	//
	BOOL Gen_Medicine(IN int, IN int, IN OUT KItem*);
	BOOL Gen_ExistMedicine(IN int, IN OUT KItem*);
	//
	BOOL Gen_Quest(IN int, IN OUT KItem*);
	BOOL Gen_ExistQuest(IN int, IN OUT KItem*);
	//
	BOOL Gen_Script(IN int, IN OUT KItem*);
	BOOL Gen_ExistScript(IN int, IN OUT KItem*);
	//
	BOOL Gen_Mine(IN int, IN int, IN int, IN OUT KItem*);
	BOOL Gen_ExistMine(IN int, IN int, IN int, IN OUT KItem*);

#ifdef _SERVER
	BOOL CheckMA(int, int, int, int);
	//
	int	 GMA_Record(const int nMagicType, const int nMagicSeries);
	//
	const KMAGICATTRIB_TABFILE*	 GMA_ByRecord(IN const int nRecord) const;
#endif

	//
	// Helper functions for exact value mode (upgraded items) - PUBLIC for ScriptFuns access
	// Decode 6 exact attribute values from 32-bit seed (5 bits per value)
	void DecodeExactValues(DWORD dwSeed, int* pOutValues);
	// Encode 6 exact attribute values into 32-bit seed (5 bits per value)
	DWORD EncodeExactValues(const int* pInValues, const int* pMinValues);

private:
	//
	// Ham tra ve mot Level ngau nhien dua vao phan tram truyen vao cac muc Level

	int KItemGenerator::getRandomLevel(const double percentages[], int numLevels);
	//
	// Ham kiem tra xem mot trang bi co cot level bang 10 hay khong
	BOOL IsMagicAttribLuckyLevel(const KMAGICATTRIB_TABFILE* pMAItem,int nLucky);

	//
	// Helper function to get global min/max across all levels for an attribute type
	void GetGlobalMinMax(int nAttribType, int& outGlobalMin, int& outGlobalMax);

	//
	BOOL Gen_MagicAttrib(int, const int*, int, int, KItemNormalAttrib*, int nVersion, BOOL bChangeOption = FALSE, DWORD dwRandomSeed = 0);
	//
	BOOL Gen_PurpleMagicAttrib(int, const int*, const int*, int, int, KItemNormalAttrib*, int nVersion);
	//
	const KMAGICATTRIB_TABFILE* GetMARecord(int) const;
	BOOL GMA_GetAvaliableMA(int);
	BOOL GMA_GetLevelAndDropRate(int);
	int  GMA_GetCandidateMA(int, int, int);
	void GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA);
	//
	BOOL DecodePurple(const int nCount, int* pMagicRecord, DWORD _ParamOne, DWORD _ParamTwo);
	
};

extern KItemGenerator	ItemGen;
#endif	// #ifndef	KItemGeneratorH
