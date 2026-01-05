#include "KEngine.h"
#include "KCore.h"
#include "MyAssert.H"
#include "KItem.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"

#include <iostream>
#include "time.h"

KItemGenerator	ItemGen;

// --
//
// --
KItemGenerator::KItemGenerator()
{
}

// --
//
// --
KItemGenerator::~KItemGenerator()
{
}

// --
//
// --
BOOL KItemGenerator::Init()
{
	if (!m_BPTLib.Init())
		return FALSE;

	int i;
	for(i = equip_meleeweapon; i < equip_detailnum - equip_meleeweapon; i++)
	{
		switch(i)
		{
		case equip_meleeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMeleeWeaponRecordNumber();
			break;
		case equip_rangeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRangeWeaponRecordNumber();
			break;
		case equip_armor:
			m_EquipNumOfEntries[i] = m_BPTLib.GetArmorRecordNumber();
			break;
		case equip_helm:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHelmRecordNumber();
			break;
		case equip_boots:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBootRecordNumber();
			break;
		case equip_belt:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBeltRecordNumber();
			break;
		case equip_cuff:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCuffRecordNumber();
			break;
		case equip_amulet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetAmuletRecordNumber();
			break;
		case equip_ring:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRingRecordNumber();
			break;
		case equip_pendant:
			m_EquipNumOfEntries[i] = m_BPTLib.GetPendantRecordNumber();
			break;
		case equip_horse:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHorseRecordNumber();
			break;
		case equip_mask:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMaskRecordNumber();
			break;
		case equip_mantle:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMantleRecordNumber();
			break;

		default:
			break;
		}
	}
	//
	m_GoldEquipNumOfEntries	= m_BPTLib.GetGoldEquipRecordNumber();
	//
	m_MedNumOfEntries = m_BPTLib.GetMedicineRecordNumber();
	//
	m_QuestNumOfEntries	= m_BPTLib.GetQuestRecordNumber();
	//
	m_ScriptNumOfEntries = m_BPTLib.GetScriptRecordNumber();
	//
	m_MineNumOfEntries = m_BPTLib.GetMineRecordNumber();

	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_Equipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	if( (nDetailType < 0) || (nDetailType >= equip_detailnum) )
		return FALSE;

	BOOL bEC = FALSE;
#ifdef _SERVER
	if(!pItem->m_GeneratorParam.dwRandomSeed)
		pItem->m_GeneratorParam.dwRandomSeed = g_GetRandomSeed();
#else
	g_RandomSeed(pItem->m_GeneratorParam.dwRandomSeed);
#endif

	const int i = nParticularType * 10 + nLevel - 1;

	if( (i < 0) || (i >= m_EquipNumOfEntries[nDetailType]) )
		return FALSE;

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_mantle:
		pEqu = m_BPTLib.GetMantleRecord(i);
		break;

	default:
		break;
	}
	if(pEqu == NULL)
		return bEC; 
	
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetGenre(item_equip);
	pItem->SetSeries(nSeriesReq);
	pItem->SetRecord(i);
	
	if(pnaryMALevel == NULL)
		return TRUE;

	KItemNormalAttrib	sMA[6];
	// Pass random seed for exact value mode support (upgraded items)
	bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nVersion, TRUE, pItem->m_GeneratorParam.dwRandomSeed);
	if(bEC)
		pItem->SetAttrib_MA(sMA);
	
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeriesReq);
#else
	//g_DebugLog("[CLIENT]GenEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeriesReq);
#endif
#endif

	return bEC;
}
// --
//
// --
BOOL KItemGenerator::Gen_ExistEquipment(IN int nRecord,
								   IN int nDetailType,
								   IN int nSeries,								  
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{	
	if( (nDetailType < 0) || (nDetailType >= equip_detailnum) )
		return FALSE;
	if( (nRecord < 0) || (nRecord >= m_EquipNumOfEntries[nDetailType]) )
		return FALSE;

	BOOL bEC = FALSE;
	g_RandomSeed(pItem->m_GeneratorParam.dwRandomSeed);

	const int i = nRecord;

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_mantle:
		pEqu = m_BPTLib.GetMantleRecord(i);
		break;

	default:
		break;
	}
	if(pEqu == NULL)
		return bEC;
	
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeries);
	pItem->SetRecord(i);

#ifndef _SERVER
	pItem->SetGenre(item_equip);
#endif
	
	if(pnaryMALevel == NULL)
		return TRUE;

	KItemNormalAttrib	sMA[6];
	// Pass random seed for exact value mode support (upgraded items)
	bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeries, nLucky, sMA, nVersion, FALSE, pItem->m_GeneratorParam.dwRandomSeed);
	if(bEC)
		pItem->SetAttrib_MA(sMA);

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeries);
#else
	//g_DebugLog("[CLIENT]GenExistEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeries);
#endif
#endif

	return bEC;
}
// --
//
// --
BOOL KItemGenerator::Gen_PurpleEquipment(IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeriesReq,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLuck,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{
	if( (nDetailType < 0) || (nDetailType >= equip_detailnum) )
		return FALSE;

	BOOL bEC = FALSE;
	const int i = nParticularType * 10 + nLevel - 1;
	
	if( (i < 0) || (i >= m_EquipNumOfEntries[nDetailType]) )
		return FALSE;
	
	pItem->m_GeneratorParam.nLuck = nLuck;

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_mantle:
		pEqu = m_BPTLib.GetMantleRecord(i);
		break;
	default:
		break;
	}
	if(pEqu == NULL)
		return bEC;

	// CRITICAL FIX: Check luck to determine attribute handling
	// luck=1000000001: Enchased purple - SKIP everything (attributes already set manually)
	// luck=1000000000: Newly created purple - RUN SetAttrib_CBR and DecodePurple for Type=53
	// luck < 1000000000: Normal purple - RUN SetAttrib_CBR and DecodePurple normally
	//
	// SetAttrib_CBR sets base properties (damage, defense, requirements) - NOT magic attributes!
	// DecodePurple sets magic attributes (6 slots) - Type=53 for empty slots

	// Step 1: Set series and record first
	pItem->SetSeries(nSeriesReq);
	pItem->SetRecord(i);

	// Step 2: Call SetAttrib_CBR to copy base attributes from equipment table
	// This will set genre=item_equip (0) temporarily - we'll override it in step 3
	// CRITICAL: SetAttrib_CBR uses "*this = *pData" which OVERRIDES series to -1!
	//           We must call SetSeries AGAIN after SetAttrib_CBR!
	pItem->SetAttrib_CBR(pEqu);

	// Step 2.5: RESTORE series after SetAttrib_CBR (which overrode it to -1)
	// This is CRITICAL for purple items to inherit series from blue items
	pItem->SetSeries(nSeriesReq);

	// Step 3: NOW set genre to purple (this overwrites the genre from SetAttrib_CBR)
	// This MUST be after SetAttrib_CBR to ensure genre stays as item_purpleequip
	pItem->SetGenre(item_purpleequip);

#ifdef _DEBUG
	if(pItem->m_GeneratorParam.nLuck == 1000000001)
	{
		g_DebugLog("[GEN_PURPLE] Enchased purple (luck=1000000001), skip DecodePurple to preserve manually set attributes");
	}
	else if(pItem->m_GeneratorParam.nLuck == 1000000000)
	{
		g_DebugLog("[GEN_PURPLE] Newly created purple (luck=1000000000), will run DecodePurple for Type=53 empty slots");
	}
	else
	{
		g_DebugLog("[GEN_PURPLE] Normal purple (luck=%d), running standard generation", pItem->m_GeneratorParam.nLuck);
	}
#endif

	// For luck=1000000001: SKIP DecodePurple to preserve magic attributes set by SetPurpleItemMagicAttrib
	if(pItem->m_GeneratorParam.nLuck == 1000000001)
	{
		return TRUE;  // Keep m_aryMagicAttrib set manually
	}

	if(pnaryMALevel == NULL)
		return TRUE;
	int j;
	for(j = 0; j < 6;j++)
	{
		if(pnaryMALevel[j] == 0)
			break;
	}

	KItemNormalAttrib	sMA[6];
	int pnaryMARecord[6];
	memset(pnaryMARecord, 0, sizeof(pnaryMARecord));

	bEC = DecodePurple(j, pnaryMARecord, pItem->m_GeneratorParam.dwRandomSeed, pItem->m_GeneratorParam.nLuck);

	if(bEC)
		bEC = Gen_PurpleMagicAttrib(nDetailType, pnaryMALevel, pnaryMARecord, nSeriesReq, j, sMA, nVersion);

	if(bEC)
		pItem->SetAttrib_MA(sMA);
	
	
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenPurpleEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeriesReq);
#else
	//g_DebugLog("[CLIENT]GenPurpleEq: Record[%d] Name[%s] Series[%d]", i, pItem->GetName(), nSeriesReq);
#endif
#endif

	return bEC;
}

// --
//
// --
BOOL KItemGenerator::Gen_ExistPurpleEquipment(IN int nRecord,
								   IN int nDetailType,
								   IN int nSeries,
								   IN const int* pnaryMALevel,
								   IN int nLuck,
								   IN int nVersion,
								   IN OUT KItem* pItem)
{	
	if( (nDetailType < 0) || (nDetailType >= equip_detailnum) )
		return FALSE;
	if( (nRecord < 0) || (nRecord >= m_EquipNumOfEntries[nDetailType]) )
		return FALSE;
	
	pItem->m_GeneratorParam.nLuck = nLuck;

	BOOL bEC = FALSE;
	const int i = nRecord;

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_mantle:
		pEqu = m_BPTLib.GetMantleRecord(i);
		break;
	default:
		break;
	}
	if(pEqu == NULL)
		return bEC;

	// CRITICAL FIX: Check luck to determine attribute handling
	// luck=1000000001: Enchased purple - RUN SetAttrib_CBR but SKIP DecodePurple
	// luck=1000000000: Newly created purple - RUN SetAttrib_CBR and DecodePurple for Type=53
	// luck < 1000000000: Normal purple - RUN SetAttrib_CBR and DecodePurple normally
	//
	// SetAttrib_CBR sets base properties (damage, defense, requirements) - ALWAYS NEEDED!
	// DecodePurple sets magic attributes (6 slots) - SKIP for enchased items

	// Step 1: Set series and record first
	pItem->SetSeries(nSeries);
	pItem->SetRecord(i);

	// Step 2: Call SetAttrib_CBR to copy base attributes from equipment table
	// This will set genre=item_equip (0) temporarily - we'll override it in step 3
	// CRITICAL: This is REQUIRED even for luck=1000000001 to get name, damage, defense, etc.
	pItem->SetAttrib_CBR(pEqu);

	// Step 3: NOW set genre to purple (this overwrites the genre from SetAttrib_CBR)
	// This MUST be after SetAttrib_CBR to ensure genre stays as item_purpleequip
	pItem->SetGenre(item_purpleequip);

#ifdef _DEBUG
	if(pItem->m_GeneratorParam.nLuck == 1000000001)
	{
		g_DebugLog("[GEN_EXIST_PURPLE] Enchased purple (luck=1000000001), skip DecodePurple to preserve DB attributes");
	}
	else if(pItem->m_GeneratorParam.nLuck == 1000000000)
	{
		g_DebugLog("[GEN_EXIST_PURPLE] Newly created purple (luck=1000000000), will run DecodePurple for Type=53 empty slots");
	}
	else
	{
		g_DebugLog("[GEN_EXIST_PURPLE] Normal purple (luck=%d), running standard generation", pItem->m_GeneratorParam.nLuck);
	}
#endif

	// For luck=1000000001: SKIP DecodePurple to preserve magic attributes from database
	if(pItem->m_GeneratorParam.nLuck == 1000000001)
	{
		return TRUE;  // Keep m_aryMagicAttrib restored from database
	}

	if(pnaryMALevel == NULL)
		return TRUE;
	int j;
	for(j = 0; j < 6;j++)
	{
		if(pnaryMALevel[j] == 0)
			break;
	}

	KItemNormalAttrib	sMA[6];
	int pnaryMARecord[6];
	memset(pnaryMARecord, 0, sizeof(pnaryMARecord));

	bEC = DecodePurple(j, pnaryMARecord, pItem->m_GeneratorParam.dwRandomSeed, pItem->m_GeneratorParam.nLuck);
	
	//g_DebugLog("RECORD[%d]", pnaryMARecord[0]);

	if(bEC)
		bEC = Gen_PurpleMagicAttrib(nDetailType, pnaryMALevel, pnaryMARecord, nSeries, j, sMA, nVersion);

	if(bEC)
		pItem->SetAttrib_MA(sMA);

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistPurpleEq: Record[%d] Name[%s] Series[%d] Luck[%d] RandSeed[%d]", i, pItem->GetName(), nSeries, pItem->m_GeneratorParam.nLuck, pItem->m_GeneratorParam.dwRandomSeed);
#else
	//g_DebugLog("[CLIENT]GenExistPurpleEq: Record[%d] Name[%s] Series[%d] Luck[%d] RandSeed[%d]", i, pItem->GetName(), nSeries, pItem->m_GeneratorParam.nLuck, pItem->m_GeneratorParam.dwRandomSeed);
#endif
#endif

	return bEC;
}
// --
//
// --
BOOL KItemGenerator::Gen_GoldEquipment(IN int nRecord, IN int nSeries, OUT KItem * pItem)
{
	if( (nRecord <= 0) || (nRecord > m_GoldEquipNumOfEntries) )
		return FALSE;
	
	if(!pItem->m_GeneratorParam.nGeneratorLevel[0])
	{	
		int i;
		for (i = 0; i < 6;i++)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0,10);
		}
	}
#ifdef _SERVER
	pItem->m_GeneratorParam.nLuck	= 7531;
#endif
	const KBASICPROP_GOLD_EQUIPMENT* pTemp = m_BPTLib.GetGoldEquipRecord(nRecord - 1);

	if(pTemp == NULL)
		return FALSE;

	pItem->SetAttrib_CBR((KBASICPROP_GOLD_EQUIPMENT*)pTemp);
	if(nSeries < 0)
	{
		nSeries = pTemp->m_nSeries;
	}
	pItem->SetGenre(item_goldequip);
	pItem->SetSeries(nSeries);
	pItem->SetRecord(nRecord);
		
	KItemNormalAttrib	sMA[6];
	int i;
	for (i = 0; i < 6; i++)
	{
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pTemp->m_aryMagicAttribs[i]);
		pDst = &(sMA[i]);

		int nType,nLow,nHigh;
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMagicType",0,&nType);
		pDst->nAttribType = nType;
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMin1",0,&nLow);
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMax1",0,&nHigh);
		pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nMin = nLow;		// Store min value (TamLTM custom)
		pDst->nMax = nHigh;		// Store max value (TamLTM custom)
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMin2",0,&nLow);
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMax2",0,&nHigh);
		pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMin3",0,&nLow);
		g_GoldMagicTab.GetInteger(*pSrc + 1,"nMax3",0,&nHigh);
		pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
	}
	pItem->SetAttrib_MA(sMA);

	//pItem->SetAttrib_ExtraMA(pTemp->m_aryExMagicAttribs);

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenGoldEq: Record[%d] Name[%s] Param1[%d]", nRecord - 1, pTemp->m_szName, pItem->GetParam1());
#else
	//g_DebugLog("[CLIENT]GenGoldEq: Record[%d] Name[%s]", nRecord - 1, pTemp->m_szName);
#endif
#endif
	
	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_BrokenEquipment(IN int nRecord,
								   IN int nDetailType,
								   IN int nParticularType,
								   IN int nSeries,
								   IN int nLevel,
								   IN const int* pnaryMALevel,
								   IN int nLucky,
								   IN int nVersion,
								   IN BOOL bNewCreate,
								   IN OUT KItem* pItem)
{	
	BOOL bEC = FALSE;

	const KBASICPROP_BROKEN_EQUIPMENT* pBroken = NULL;
	pBroken = m_BPTLib.GetBrokenEquipRecord(0);
	if(pBroken == NULL)
		return bEC;
	
	if(bNewCreate)
	{	
		if(nRecord > 0)
			bEC = Gen_GoldEquipment(nRecord, nSeries, pItem);
		else if(nLucky >= 1000000000)
			bEC = Gen_PurpleEquipment(nDetailType, nParticularType, nSeries, nLevel, pnaryMALevel, nLucky, nVersion, pItem);
		else
			bEC = Gen_Equipment(nDetailType, nParticularType, nSeries, nLevel, pnaryMALevel, nLucky, nVersion, pItem);
	}
	else
	{
		if(nLucky == 7531)
			bEC = Gen_GoldEquipment(nRecord, nSeries, pItem);
		else if(nLucky >= 1000000000)
			bEC = Gen_ExistPurpleEquipment(nRecord, nDetailType, nSeries, pnaryMALevel, nLucky, nVersion, pItem);
		else
			bEC = Gen_ExistEquipment(nRecord, nDetailType, nSeries, pnaryMALevel, nLucky, nVersion, pItem); //moi lam

	}
	
	if(pItem == NULL)
		return FALSE;

	pItem->SetBrokenEquip(pBroken);
	pItem->SetGenre(item_brokenequip);

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenBrokenEq: Record[%d] Name[%s] Series[%d]", nRecord, pItem->GetName(), nSeries);
#else
	//g_DebugLog("[CLIENT]GenBrokenEq: Record[%d] Name[%s] Series[%d]", nRecord, pItem->GetName(), nSeries);
#endif
#endif

	return bEC;
}
// --
//
// --
BOOL KItemGenerator::Gen_Medicine(IN int nDetailType,
								  IN int nLevel,
								  IN OUT KItem* pItem)
{

	BOOL bEC = FALSE;
	
	const int i = nDetailType * 5 + nLevel - 1;
	if( (i < 0) || (i >= m_MedNumOfEntries) )
		return FALSE;

	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if(pMed == NULL)
		return bEC;

	*pItem = *pMed;
	pItem->SetGenre(item_medicine);
	pItem->SetRecord(i);
	

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenMedicine: Record[%d] Name[%s]", i, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenMedicine: Record[%d] Name[%s]", i, pItem->GetName());
#endif
#endif

	return TRUE;
}

// --
//
// --
BOOL KItemGenerator::Gen_ExistMedicine(IN int nRecord, IN OUT KItem* pItem)
{
	if( (nRecord < 0) || (nRecord >= m_MedNumOfEntries) )
		return FALSE;

	const int i = nRecord;
	
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if(pMed == NULL)
		return FALSE;

	*pItem = *pMed;
#ifndef _SERVER
	pItem->SetGenre(item_medicine);
	pItem->SetRecord(i);
#endif
	
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistMedicine: Record[%d] Name[%s]", nRecord, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenExistMedicine: Record[%d] Name[%s]", nRecord, pItem->GetName());
#endif
#endif

	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_Quest(IN int nDetailType, IN OUT KItem* pItem)
{
	const KBASICPROP_QUEST* pQuest = NULL;
	int i = -1;
	pQuest = m_BPTLib.FindQuestRecord(nDetailType, &i);
	if(pQuest == NULL)
		return FALSE;

	if( (i < 0) || (i >= m_QuestNumOfEntries) )
		return FALSE;

	*pItem = *pQuest;
	pItem->SetGenre(item_task);
	pItem->SetRecord(i);
	
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenQuest: Record[%d] Name[%s]", i, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenQuest: Record[%d] Name[%s]", i, pItem->GetName());
#endif
#endif

	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_ExistQuest(IN int nRecord, IN OUT KItem* pItem)
{	
	if( (nRecord < 0) || (nRecord >= m_QuestNumOfEntries) )
		return FALSE;

	const int i = nRecord;

	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(i);
	if(pQuest == NULL)
		return FALSE;

	*pItem = *pQuest;
#ifndef _SERVER
	pItem->SetGenre(item_task);
	pItem->SetRecord(i);
#endif
	
	
#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistQuest: Record[%d] Name[%s]", nRecord, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenExistQuest: Record[%d] Name[%s]", nRecord, pItem->GetName());
#endif
#endif

	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_Script(IN int nDetailType, IN OUT KItem* pItem)
{
	
	const KBASICPROP_SCRIPT* pScript = NULL;
	
	int i = -1;
	pScript = m_BPTLib.FindScriptRecord(nDetailType, &i);
	if(pScript == NULL)
		return FALSE;

	if( (i < 0) || (i >= m_ScriptNumOfEntries) )
		return FALSE;

	*pItem = *pScript;
	pItem->SetGenre(item_script);
	pItem->SetRecord(i);

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenScript: Record[%d] Name[%s] [%d|%d]", i, pItem->GetName(), pItem->GetWidth(), pItem->GetMaxStackCount());
#else
	//g_DebugLog("[CLIENT]GenScript: Record[%d] Name[%s] [%d|%d]", i, pItem->GetName(), pItem->GetWidth(), pItem->GetMaxStackCount());
#endif
#endif
	
	return TRUE;
}

// --
//
// --
BOOL KItemGenerator::Gen_ExistScript(IN int nRecord, IN OUT KItem* pItem)
{	
	
	if( (nRecord < 0) || (nRecord >= m_ScriptNumOfEntries) )
		return FALSE;

	const int i = nRecord;

	const KBASICPROP_SCRIPT* pScript = NULL;
	pScript = m_BPTLib.GetScriptRecord(i);

	if(pScript == NULL)
		return FALSE;

	*pItem = *pScript;
#ifndef _SERVER
	pItem->SetGenre(item_script);
	pItem->SetRecord(i);
#endif

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistScript: Record[%d] Name[%s]", nRecord, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenExistScript: Record[%d] Name[%s]", nRecord, pItem->GetName());
#endif
#endif
	
	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::Gen_Mine(IN int nDetailType, IN int nLevel, IN int nSeries, IN OUT KItem* pItem)
{
	
	const KBASICPROP_MINE* pMine = NULL;
	
	int i = -1;
	pMine = m_BPTLib.FindMineRecord(nDetailType, &i);
	if(pMine == NULL)
		return FALSE;

	if( (i < 0) || (i >= m_MineNumOfEntries) )
		return FALSE;

	*pItem = *pMine;
	pItem->SetGenre(item_mine);
	if(nSeries >= 0)
		pItem->SetSeries(nSeries);
	pItem->SetLevel(nLevel);
	pItem->SetRecord(i);
	//
	if(pItem->GetGeneratorParam()->nLuck > 0)
	{
		KItemNormalAttrib	sMA[6];
		ZeroMemory(sMA, sizeof(KItemNormalAttrib) * 6);
		
		sMA[0].nAttribType = pItem->GetGeneratorParam()->nLuck;
		sMA[0].nValue[0] = 0;
		sMA[0].nValue[1] = 0;
		sMA[0].nValue[2] = 0;

		pItem->SetAttrib_MA(sMA);
	}

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenMine: Record[%d] Name[%s] [%d|%d]", i, pItem->GetName(), pItem->GetWidth(), pItem->GetMaxStackCount());
#else
	//g_DebugLog("[CLIENT]GenMine: Record[%d] Name[%s] [%d|%d]", i, pItem->GetName(), pItem->GetWidth(), pItem->GetMaxStackCount());
#endif
#endif
	
	return TRUE;
}

// --
//
// --
BOOL KItemGenerator::Gen_ExistMine(IN int nRecord, IN int nLevel, IN int nSeries, IN OUT KItem* pItem)
{	
	
	if( (nRecord < 0) || (nRecord >= m_MineNumOfEntries) )
		return FALSE;

	const int i = nRecord;

	const KBASICPROP_MINE* pMine = NULL;
	pMine = m_BPTLib.GetMineRecord(i);

	if(pMine == NULL)
		return FALSE;

	*pItem = *pMine;

#ifndef _SERVER
	pItem->SetGenre(item_mine);
	pItem->SetRecord(i);
#endif
	if(nSeries >= 0)
		pItem->SetSeries(nSeries);
	pItem->SetLevel(nLevel);
	
	//
	if(pItem->GetGeneratorParam()->nLuck > 0)
	{
		KItemNormalAttrib	sMA[6];
		ZeroMemory(sMA, sizeof(KItemNormalAttrib) * 6);
		
		sMA[0].nAttribType = pItem->GetGeneratorParam()->nLuck;
		sMA[0].nValue[0] = 0;
		sMA[0].nValue[1] = 0;
		sMA[0].nValue[2] = 0;

		pItem->SetAttrib_MA(sMA);
	}

#ifdef _DEBUG
#ifdef _SERVER
	//g_DebugLog("[SERVER]GenExistMine: Record[%d] Name[%s]", nRecord, pItem->GetName());
#else
	//g_DebugLog("[CLIENT]GenExistMine: Record[%d] Name[%s]", nRecord, pItem->GetName());
#endif
#endif
	
	return TRUE;
}
// --
//
// --

//Ham tra ve mot Level ngau nhien dua vao phan tram truyen vao cac moc Level
int KItemGenerator::getRandomLevel(const double percentages[], int numLevels) {
 	int totalPercentage = 0;
 	int i = 0;
    for ( i = 0; i < numLevels; ++i) {
        totalPercentage += percentages[i];
    }

    int randomNum = rand() % totalPercentage + 1;
    int cumulativePercentage = 0;

    for (i = 0; i < numLevels; ++i) {
        cumulativePercentage += percentages[i];
        if (randomNum <= cumulativePercentage) {
            return i;
        }
    }

     // Neu co loi, tra ve Level cuoi cung
    return numLevels - 1;
}

// Ham kiem tra xem mot trang bi co cot level bang 10 hay khong
BOOL KItemGenerator::IsMagicAttribLuckyLevel(const KMAGICATTRIB_TABFILE* pMAItem,int nLucky) 
{
   	_ASSERT(pMAItem);
   	if (nLucky < 0) //Neu may man be hon 0 thi trang bi cap do trang bi roi ra bang 1;
	{
		printf("[KItemGenerator] Loi nLucky < 0");
		return 1;
	}
	// Khai bao cac moc level va phan tram tuong ung
    const int numLevels = 10; //MAX LEVEL
    int i = 0;
    //			LEVEL                  1    2     3     4     5     6     7     8     9    10
    double percentages[numLevels] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
 	
 	if (nLucky < 5)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 5.0, 15.0, 20.0, 20.0, 20.0, 19.0, 1.0, 0.0};
    }
    else if(nLucky >= 6 && nLucky < 10)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 15.0, 25.0, 30.0, 15.0, 10.0, 5.0, 0.0};
    }
    else if(nLucky >= 10 && nLucky < 14)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 5.0, 15.0, 20.0, 20.0, 20.0, 19.0, 1.0};
    }
    else if(nLucky >= 14 && nLucky < 18)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 5.0, 20.0, 30.0, 20.0, 22.0, 3.0};
    }
    else if(nLucky >= 18 && nLucky < 22)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 5.0, 25.0, 20.0, 20.0, 25.0, 5.0};
    }
    else if(nLucky >= 22 && nLucky < 26)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 30.0, 25.0, 25.0, 10.0};
    }   
    else if(nLucky >= 26 && nLucky < 30)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 25.0, 30.0, 27.0, 13.0};
    }
    else if(nLucky >= 30 && nLucky < 34)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0, 30.0, 30.0, 15.0};
    }
    else if(nLucky >= 34 && nLucky < 38)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 33.0, 30.0, 17.0};
    }    
    else if(nLucky >= 38 && nLucky < 42)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 35.0, 30.0, 20.0};
    }
    else if(nLucky >= 42)
    {
    //		LEVEL               		1    2    3    4     5     6     7     8     9    10
    	double percentages[numLevels] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 30.0, 35.0, 25.0};
    }   
      

    // Kiem tra tong phan tram co bang 100 hay khong
    double totalPercentage = 0.0;
    for (i = 0; i < numLevels; ++i) {
        totalPercentage += percentages[i];
    }
    if (totalPercentage != 100.0) {
        printf("Loi Item May Man: Tong phan tram khong bang 100!");
        return 1;
    }

	// Seed thoi gian hien tai da co gia tri ngau nhien khac nhau moi lan chay
	srand( (unsigned long)time(NULL) );
    // Lay ket qua ngau nhien dua vao phan tram
    int randomLevel = getRandomLevel(percentages, numLevels);

    // In ket qua ra man hinh
//    g_DebugLog("Ket qua ngau nhien: Level %d - Lucky: %d",randomLevel + 1,nLucky);
    return (pMAItem && pMAItem->m_nLevel == randomLevel + 1);
}

// --
// Helper function to get global min/max across all levels for an attribute type
// This scans all magic attribute records and finds the minimum Min and maximum Max
// across all levels of the same attribute type (nPropKind)
// --
void KItemGenerator::GetGlobalMinMax(int nAttribType, int& outGlobalMin, int& outGlobalMax)
{
	outGlobalMin = 999999;  // Start with very large value
	outGlobalMax = -999999; // Start with very small value

	// Get total number of magic attribute records
	int nTotalRecords = m_BPTLib.GetMARecordNumber();

	// Scan through all records
	for (int i = 0; i < nTotalRecords; i++)
	{
		const KMAGICATTRIB_TABFILE* pMAItem = m_BPTLib.GetMARecord(i);
		if (!pMAItem)
			continue;

		// Check if this record is for the same attribute type
		if (pMAItem->m_MagicAttrib.nPropKind != nAttribType)
			continue;

		// Update global min/max from aryRange[0] (primary range)
		if (pMAItem->m_MagicAttrib.aryRange[0].nMin < outGlobalMin)
			outGlobalMin = pMAItem->m_MagicAttrib.aryRange[0].nMin;

		if (pMAItem->m_MagicAttrib.aryRange[0].nMax > outGlobalMax)
			outGlobalMax = pMAItem->m_MagicAttrib.aryRange[0].nMax;

		// For damage range attributes, also check aryRange[2]
		BOOL bIsDamageRange = (
			nAttribType == magic_colddamage_v ||
			nAttribType == magic_firedamage_v ||
			nAttribType == magic_lightingdamage_v ||
			nAttribType == magic_poisondamage_v ||
			nAttribType == magic_physicsdamage_v ||
			nAttribType == magic_addphysicsmagic_v ||
			nAttribType == magic_addcoldmagic_v ||
			nAttribType == magic_addfiremagic_v ||
			nAttribType == magic_addlightingmagic_v ||
			nAttribType == magic_addpoisonmagic_v
		);

		if (bIsDamageRange)
		{
			// Check if aryRange[2] has valid range (not constant)
			if (pMAItem->m_MagicAttrib.aryRange[2].nMin != pMAItem->m_MagicAttrib.aryRange[2].nMax)
			{
				if (pMAItem->m_MagicAttrib.aryRange[2].nMax > outGlobalMax)
					outGlobalMax = pMAItem->m_MagicAttrib.aryRange[2].nMax;
			}
		}
	}

	// Sanity check - if no records found, set to 0
	if (outGlobalMin == 999999 || outGlobalMax == -999999)
	{
		outGlobalMin = 0;
		outGlobalMax = 0;
	}
}

// --
// Helper function to decode 6 exact attribute values from 32-bit seed
// Encoding: 5 bits per value (supports deltas 0-31 from min)
// Format: bits [0-4]=slot0, [5-9]=slot1, [10-14]=slot2, [15-19]=slot3, [20-24]=slot4, [25-29]=slot5
// --
void KItemGenerator::DecodeExactValues(DWORD dwSeed, int* pOutValues)
{
	if (!pOutValues)
		return;

	// Extract 5-bit deltas from seed
	for (int i = 0; i < 6; i++)
	{
		int nShift = i * 5;  // bits 0,5,10,15,20,25
		int nDelta = (dwSeed >> nShift) & 0x1F;  // Extract 5 bits (0-31)
		pOutValues[i] = nDelta;  // Store delta
	}
}

// --
// Helper function to encode 6 exact attribute values into 32-bit seed
// Values are stored as deltas from their min values (to fit in 5 bits each)
// --
DWORD KItemGenerator::EncodeExactValues(const int* pInValues, const int* pMinValues)
{
	if (!pInValues || !pMinValues)
		return 0;

	DWORD dwSeed = 0;

	// Pack 6 deltas into 30 bits (5 bits each)
	for (int i = 0; i < 6; i++)
	{
		int nDelta = pInValues[i] - pMinValues[i];  // Calculate delta from min

		// Clamp delta to 0-31 range (5 bits)
		if (nDelta < 0) nDelta = 0;
		if (nDelta > 31) nDelta = 31;

		int nShift = i * 5;  // bits 0,5,10,15,20,25
		dwSeed |= ((DWORD)nDelta << nShift);
	}

	return dwSeed;
}

// -- 
//
// --
BOOL KItemGenerator::Gen_MagicAttrib(
    int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
	KItemNormalAttrib* pnaryMA, int nGameVersion, BOOL bChangeOption /*= FALSE*/, DWORD dwRandomSeed /*= 0*/
)
{
    int nResult = false;

    KBPT_ClassMAIT  SelectedMagicTable;	
    KMAGICATTRIB_TABFILE *pMagicAttrTable[6] = { NULL, NULL, NULL, NULL, NULL, NULL };

    int i = 0;

    _ASSERT(pnaryMALevel);
    _ASSERT(pnaryMA);

    // Check for EXACT ATTRIBUTE MODE (upgraded items)
    // NEW ENCODING: nLucky >= 999900000 indicates exact attribute mode
    // - pnaryMALevel[i] = exact VALUES (not generator levels!)
    // - dwRandomSeed = packed attribute TYPES 0-3 (8 bits each)
    // - nGameVersion = packed attribute TYPES 4-5 (8 bits each)
    BOOL bExactAttributeMode = (nLucky >= 999900000);

    if (bExactAttributeMode)
    {
        // Extract original luck value
        int nOriginalLucky = nLucky - 999900000;
        if (nOriginalLucky < 0) nOriginalLucky = 0;

#ifdef _DEBUG
        g_DebugLog("[Gen_MagicAttrib] --- EXACT ATTRIBUTE MODE ---");
        g_DebugLog("[Gen_MagicAttrib] Luck=%d, OriginalLuck=%d, Seed=0x%08X, Version=%d",
            nLucky, nOriginalLucky, dwRandomSeed, nGameVersion);
#endif

        // Decode attribute TYPES from seed and version
        int nExactTypes[6];
        nExactTypes[0] = (dwRandomSeed >> 0) & 0xFF;   // bits 0-7
        nExactTypes[1] = (dwRandomSeed >> 8) & 0xFF;   // bits 8-15
        nExactTypes[2] = (dwRandomSeed >> 16) & 0xFF;  // bits 16-23
        nExactTypes[3] = (dwRandomSeed >> 24) & 0xFF;  // bits 24-31
        nExactTypes[4] = (nGameVersion >> 0) & 0xFF;   // bits 0-7
        nExactTypes[5] = (nGameVersion >> 8) & 0xFF;   // bits 8-15

        // pnaryMALevel contains VALUES directly (not levels!)
        // Set attributes directly without random generation
        for (i = 0; i < 6; i++)
        {
            if (nExactTypes[i] == 0)  // No attribute in this slot
                break;

            pnaryMA[i].nAttribType = nExactTypes[i];
            pnaryMA[i].nValue[0] = pnaryMALevel[i];  // Value stored directly
            pnaryMA[i].nValue[1] = 0;
            pnaryMA[i].nValue[2] = 0;

            // Get global min/max for this attribute type
            int nGlobalMin = 0, nGlobalMax = 0;
            GetGlobalMinMax(nExactTypes[i], nGlobalMin, nGlobalMax);
            pnaryMA[i].nMin = nGlobalMin;
            pnaryMA[i].nMax = nGlobalMax;

#ifdef _DEBUG
            g_DebugLog("[Gen_MagicAttrib] Slot[%d]: Type=%d, Value=%d, Min=%d, Max=%d",
                i, nExactTypes[i], pnaryMALevel[i], nGlobalMin, nGlobalMax);
#endif
        }

        // Fill remaining slots with empty attributes
        for (NULL; i < 6; i++)
        {
            pnaryMA[i].nAttribType = 0;
            pnaryMA[i].nValue[0] = 0;
            pnaryMA[i].nValue[1] = 0;
            pnaryMA[i].nValue[2] = 0;
            pnaryMA[i].nMin = 0;
            pnaryMA[i].nMax = 0;
        }

#ifdef _DEBUG
        g_DebugLog("[Gen_MagicAttrib] --- EXACT MODE COMPLETE ---");
#endif

        nResult = true;
        return nResult;  // Early exit - skip normal generation
    }
    for (i = 0; i < 6; i++) 
    {
        if (pnaryMALevel[i] == 0)	
        	break;
	
        SelectedMagicTable.Clear();
	
        const KBPT_ClassMAIT *pCMITItem = m_BPTLib.GetCMIT(
            1 - (i & 1),        
            nType,              
            nSeriesReq,          
            pnaryMALevel[i]     
        );

        _ASSERT(pCMITItem);
        if (!pCMITItem)
             break;

        int nCMITItemCount = pCMITItem->GetCount();
        int j = 0;
        int nDecide = GetRandomNumber(0, 100 - 1) / (1 + nLucky * 20 / 100);

        for (j = 0; j < nCMITItemCount; j++)
        {
            int nMAIndex = pCMITItem->Get(j);
            KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nMAIndex);
            if (!pMAItem)
            {
                _ASSERT(pMAItem);
                continue;
            }
			
            if (pMAItem->m_nUseFlag)
            {
                continue;
            }

            if ((pMAItem->m_DropRate[nType]) <= nDecide)
                continue;
            

            // Ki?m tra t? l? may m?n c?a nh?n v?t xem c? ra d? VIP kh?ng?;
            // Date func: 25/07/2023;
            // Author Name: Kinnox;
            if (nLucky >= 0 && bChangeOption)
            {
	        	if (!IsMagicAttribLuckyLevel(pMAItem,nLucky))
	         	   continue;
	        }

	        //
            int k = 0;
            for (k = 0; k < i; k++)
            {
                _ASSERT(pMagicAttrTable[k]);
                if ((pMagicAttrTable[k]->m_MagicAttrib.nPropKind) == (pMAItem->m_MagicAttrib.nPropKind))
                    break;
            }

            if (k < i)
                continue;

            SelectedMagicTable.Insert(nMAIndex);
        }

        int nSelectedCount = SelectedMagicTable.GetCount();

        if (nSelectedCount == 0)
        {   
            break;
        }
        
        int nLuckyItemIndex = GetRandomNumber(0, nSelectedCount - 1);
        KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(
             SelectedMagicTable.Get(nLuckyItemIndex)
        );

        pMagicAttrTable[i] = pMAItem;

        _ASSERT(pMAItem);
        _ASSERT(!(pMAItem->m_nUseFlag)); 

        pMAItem->m_nUseFlag = true;

        pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;

        // NORMAL MODE: Random generation of values
        pnaryMA[i].nValue[0] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[0].nMin,
            pMAItem->m_MagicAttrib.aryRange[0].nMax
        );
        pnaryMA[i].nValue[1] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[1].nMin,
            pMAItem->m_MagicAttrib.aryRange[1].nMax
        );
        pnaryMA[i].nValue[2] = GetRandomNumber(
            pMAItem->m_MagicAttrib.aryRange[2].nMin,
            pMAItem->m_MagicAttrib.aryRange[2].nMax
        );
        // FIX: Get GLOBAL min/max across ALL levels for this attribute type
        // This shows players the full possible range [lowest-highest] regardless of which level was rolled
        // Example: Attack Speed has level 2 (10-10), level 5 (20-20), level 10 (30-30)
        // ? Display [10-30] for all levels to show full range
        int nGlobalMin = 0, nGlobalMax = 0;
        GetGlobalMinMax(pMAItem->m_MagicAttrib.nPropKind, nGlobalMin, nGlobalMax);
        pnaryMA[i].nMin = nGlobalMin;
        pnaryMA[i].nMax = nGlobalMax;
#ifdef _DEBUG
		/*g_DebugLog(					
			"[GenMagicAttrib] pnaryMA[%d]: (%d:%s), nAttribType = %d, nValue[0] = %d, nValue[1] = %d, nValue[2] = %d,", 
			i,
			SelectedMagicTable.Get(nLuckyItemIndex),
			pMAItem->m_szName,
			pnaryMA[i].nAttribType,
			pnaryMA[i].nValue[0],
			pnaryMA[i].nValue[1],
			pnaryMA[i].nValue[2]
		);*/
#endif

    }
    for (NULL; i < 6; i++)
    {
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
		pnaryMA[i].nMin = 0;
        pnaryMA[i].nMax = 0;
    }

    for (i = 0; i < 6; i++)
    {
        if (!pMagicAttrTable[i])
            break;

        _ASSERT(pMagicAttrTable[i]->m_nUseFlag);

        pMagicAttrTable[i]->m_nUseFlag = false;
    }

    nResult = true;

	return nResult;
}
// --
//
// --
BOOL KItemGenerator::Gen_PurpleMagicAttrib(
    int nType, const int* pnaryMALevel, const int* pnaryMARecord, int nSeriesReq, int nCount,
	KItemNormalAttrib* pnaryMA, int nGameVersion
)
{
	if(!pnaryMALevel)
		return FALSE;

    if(!pnaryMARecord)
		return FALSE;
	
	int i;
	KMAGICATTRIB_TABFILE *pMAItem = NULL;	
	for(i = 0; i < nCount; i++)
	{
		//g_DebugLog("Luck[%d] i %d", pnaryMARecord[i], i);
		if(pnaryMARecord[i] == 0)
		{
			pnaryMA[i].nAttribType = magic_item_reserve10;
			pnaryMA[i].nValue[0] = 0;
			pnaryMA[i].nValue[1] = 0;
			pnaryMA[i].nValue[2] = 0;
			pnaryMA[i].nMin = 0;
			pnaryMA[i].nMax = 0;

		}
		else
		{
			pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(pnaryMARecord[i] - 1);
			if(pMAItem->m_nPos != (1- (i & 1)) )
				continue;
			if( (pMAItem->m_nClass != -1) && (pMAItem->m_nClass != nSeriesReq) )
				continue;
			if(pMAItem->m_DropRate[nType] == 0)
				continue;

			pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;
			pnaryMA[i].nValue[0] = pMAItem->m_MagicAttrib.aryRange[0].nMax - pnaryMALevel[i] + 1;

		// FIX: Get GLOBAL min/max across ALL levels for this attribute type (same as Gen_MagicAttrib)
		int nGlobalMin = 0, nGlobalMax = 0;
		GetGlobalMinMax(pMAItem->m_MagicAttrib.nPropKind, nGlobalMin, nGlobalMax);
		pnaryMA[i].nMin = nGlobalMin;
		pnaryMA[i].nMax = nGlobalMax;
   
			pnaryMA[i].nValue[1] = pMAItem->m_MagicAttrib.aryRange[1].nMax - pnaryMALevel[i] + 1;
			pnaryMA[i].nValue[2] = pMAItem->m_MagicAttrib.aryRange[2].nMax - pnaryMALevel[i] + 1;
		}
	}

	return TRUE;
}
// --
//
// --
BOOL KItemGenerator::GMA_GetAvaliableMA(int nType)
{
	const KBPT_ClassifiedMAT* pCMAT;
	int i;
	for (i = 0; i < 2; i++)
	{
		pCMAT = m_BPTLib.GetCMAT(i, nType);
		if(pCMAT == NULL)
		{ 
			_ASSERT(FALSE); 
			return FALSE; 
		}
		m_GMA_nCount[i] = NUMOFCMA;
		pCMAT->GetAll((int*)m_GMA_naryMA[i], &(m_GMA_nCount[i]));
	}
	return TRUE;
}

// --
//
// --
BOOL KItemGenerator::GMA_GetLevelAndDropRate(int nType)
{
	const KMAGICATTRIB_TABFILE* pMATF = NULL;
	int i, n;
	for (i = 0; i < 2; i++)
	{
		for (n = 0; n < m_GMA_nCount[i]; n++)
		{
			
			pMATF = GetMARecord(m_GMA_naryMA[i][n]);
			if(pMATF == NULL)
			{ 
				_ASSERT(FALSE); 
				return FALSE; 
			}
			m_GMA_naryLevel[i][n] = pMATF->m_nLevel;
			m_GMA_naryDropRate[i][n] = pMATF->m_DropRate[nType];
		}
	}
	return TRUE;
}

// --
//
// --
void KItemGenerator::GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA)
{
	_ASSERT(pINA != NULL);


	pINA->nAttribType = 0;
	pINA->nValue[0] = 0;
	pINA->nValue[1] = 0;
	pINA->nValue[2] = 0;
	pINA->nMin = 0;
	pINA->nMax = 0;
	int nDropRate = ::GetRandomNumber(0, 100) - nLucky;
	int nCount = GMA_GetCandidateMA(nPos, nLevel, nDropRate);
	if (nCount > 0)
	{
		int nFinal = ::GetRandomNumber(0, nCount);
		int nMAi = m_GMA_naryCandidateMA[nFinal];
		int nMA = m_GMA_naryMA[nPos][nMAi];
		if (nMA == -1)
		{
			pINA->nAttribType = 0;
			return;
		}
		m_GMA_naryMA[nPos][nMAi] = -1;
		const KMAGICATTRIB_TABFILE* pMATF = GetMARecord(nMA);
		if (NULL != pMATF)
		{
			const KMACP* pMACP = &(pMATF->m_MagicAttrib);
			pINA->nAttribType = pMACP->nPropKind;
			pINA->nValue[0] = ::GetRandomNumber(pMACP->aryRange[0].nMin, pMACP->aryRange[0].nMax);
			pINA->nValue[1] = ::GetRandomNumber(pMACP->aryRange[1].nMin, pMACP->aryRange[1].nMax);
			pINA->nValue[2] = ::GetRandomNumber(pMACP->aryRange[2].nMin, pMACP->aryRange[2].nMax);
			pINA->nMin = pMACP->aryRange[0].nMin;	// Store min value (TamLTM custom)
			pINA->nMax = pMACP->aryRange[0].nMax;	// Store max value (TamLTM custom)
		}
	}
}

// --
//
// --
const KMAGICATTRIB_TABFILE* KItemGenerator::GetMARecord(int i) const
{
	return m_BPTLib.GetMARecord(i);
}

// --
//
// --
int KItemGenerator::GMA_GetCandidateMA(int nPos, int nLevel, int nDropRate)
{
	int i, nCount = 0;
	for (i = 0; i < m_GMA_nCount[nPos]; i++)
	{
		if (m_GMA_naryMA[nPos][i] != -1 &&
			m_GMA_naryLevel[nPos][i] == nLevel &&
			m_GMA_naryDropRate[nPos][i] >= nDropRate)
		{
			m_GMA_naryCandidateMA[nCount++] = i;
		}
	}
	return nCount;
}
// --
//
// --
BOOL KItemGenerator::DecodePurple(const int nCount, int* pMagicRecord, DWORD _ParamOne, DWORD _ParamTwo)
{
	if(nCount <= 0)
		return FALSE;

	if(_ParamOne < 1000000000)
		return FALSE;
	if(_ParamTwo < 1000000000)
		return FALSE;

	int nPos = 0;
	// 
	_ParamOne = _ParamOne % 1000000000;
	pMagicRecord[nPos] = _ParamOne / 1000000;
	nPos++;
	if(nPos >= nCount)
		return TRUE;
	//
	_ParamOne = _ParamOne % 1000000;
	pMagicRecord[nPos] = _ParamOne / 1000;
	nPos++;
	if(nPos >= nCount)
		return TRUE;
	//
	_ParamOne = _ParamOne % 1000;
	pMagicRecord[nPos] = _ParamOne; 
	nPos++;
	if(nPos >= nCount)
		return TRUE;
	//
	_ParamTwo = _ParamTwo % 1000000000;
	pMagicRecord[nPos] = _ParamTwo / 1000000;
	nPos++;
	if(nPos >= nCount)
		return TRUE;
	//
	_ParamTwo = _ParamTwo % 1000000;
	pMagicRecord[nPos] = _ParamTwo / 1000;
	nPos++;
	if(nPos >= nCount)
		return TRUE;
	//
	_ParamTwo = _ParamTwo % 1000;
	pMagicRecord[nPos] = _ParamTwo; 
	//
	return TRUE;
}	
#ifdef _SERVER
// --
//
// --
int KItemGenerator::CheckMA(int nRecord, int nPos, int nType, int nSeriesReq)
{
	KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nRecord);

	if(pMAItem->m_nPos != (1 - (nPos & 1)) )
		return FALSE;
	if( (pMAItem->m_nClass != -1) && (pMAItem->m_nClass != nSeriesReq) )
		return FALSE;
	if(pMAItem->m_DropRate[nType] == 0)
		return FALSE;

	return TRUE;
}
// --
//
// --
int KItemGenerator::GMA_Record(const int nMagicType, const int nMagicSeries)
{
	int nRecord = 0;
	
	const KMAGICATTRIB_TABFILE* pMAItem = NULL;
	pMAItem = (KMAGICATTRIB_TABFILE*)m_BPTLib.FindMARecord(nMagicType, nMagicSeries, &nRecord);
	if( !pMAItem )
		return 0;

	return nRecord + 1;
}
// --
//
// --
const KMAGICATTRIB_TABFILE* KItemGenerator::GMA_ByRecord(IN const int nRecord) const
{
	return (KMAGICATTRIB_TABFILE*)m_BPTLib.GetMARecord(nRecord);
}
#endif