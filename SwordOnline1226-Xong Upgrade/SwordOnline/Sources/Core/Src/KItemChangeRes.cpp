// --
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemChangeRes.h
// Date:	2002.12
// Code:	Spe
// Desc:	Header File
// --
#include "KCore.h"
#include "KEngine.h"
#include "KItemChangeRes.h"
#include "KItem.h"
#include "CoreUseNameDef.h"


KItemChangeRes	g_ItemChangeRes;
// --
//
// --
BOOL KItemChangeRes::Init()
{
	if (!m_MeleeWeapon.Load(CHANGERES_MELEE_FILE))
		return FALSE;
	if (!m_RangeWeapon.Load(CHANGERES_RANGE_FILE))
		return FALSE;
	if (!m_Armor.Load(CHANGERES_ARMOR_FILE))
		return FALSE;
	if (!m_Helm.Load(CHANGERES_HELM_FILE))
		return FALSE;
	if (!m_Horse.Load(CHANGERES_HORSE_FILE))
		return FALSE;
	if (!m_Gold.Load(CHANCERES_GOLD_FILE))
		return FALSE;
	return TRUE;
}

// --
//
// --
int	KItemChangeRes::GetWeaponRes(const int nDetailType, const int nRecord)
{
	
	if(nRecord < 0)
		return 0;

	int nRet;
	char szRow[16];
	sprintf(szRow, "%d", nRecord + 2);
	switch(nDetailType)
	{
	case equip_meleeweapon:
		m_MeleeWeapon.GetInteger(szRow, "ResType", 2, &nRet);
		break;
	case equip_rangeweapon:
		m_RangeWeapon.GetInteger(szRow, "ResType", 2, &nRet);
		break;
	}

	return nRet - 2;
}

// --
//
// --
int	KItemChangeRes::GetArmorRes(const int nRecord)
{	
	if(nRecord < 0)
		return 18;

	int nRet;
	char szRow[16];
	sprintf(szRow, "%d", nRecord + 2);

	m_Armor.GetInteger(szRow, "ResType", 20, &nRet);
	return nRet - 2;
}

// --
//
// --
int	KItemChangeRes::GetHelmRes(const int nRecord)
{	
	if(nRecord < 0)
		return 18;

	int nRet;
	char szRow[16];
	sprintf(szRow, "%d", nRecord + 2);
	m_Helm.GetInteger(szRow, "ResType", 20, &nRet);
	return nRet - 2;
}

// --
//
// --
int KItemChangeRes::GetHorseRes(const int nRecord)
{
	if(nRecord < 0)
		return -1;

	int nRet;
	char szRow[16];
	sprintf(szRow, "%d", nRecord + 2);
	m_Horse.GetInteger(szRow, "ResType", 2, &nRet);
	return nRet - 2;
}

// --
//
// --
int	KItemChangeRes::GetGoldEquipRes(const int nRecord)
{
	int nRet;
	char szBuff[16];
	sprintf(szBuff, "%d", nRecord);
		
	m_Gold.GetInteger(szBuff, "ResType", 2, &nRet);
	return nRet - 2;
}
