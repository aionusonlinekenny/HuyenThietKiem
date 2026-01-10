// --
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemChangeRes.h
// Date:	2002.12
// Code:	Spe
// Desc:	Header File
// --

#ifndef	KItemChangeResH
#define	KItemChangeResH

#include "KTabFile.h"

class KItemChangeRes
{
private:
	KTabFile	m_MeleeWeapon;
	KTabFile	m_RangeWeapon;
	KTabFile	m_Armor;
	KTabFile	m_Helm;
	KTabFile	m_Horse;
	KTabFile	m_Gold;

public:
	BOOL		Init();
	int			GetWeaponRes(const int nDetailType, const int nRecord);
	int			GetArmorRes(const int nRecord);
	int			GetHelmRes(const int nRecord);
	int			GetHorseRes(const int nRecord);
	int			GetGoldEquipRes(const int nRecord);
};

extern KItemChangeRes	g_ItemChangeRes;
#endif
