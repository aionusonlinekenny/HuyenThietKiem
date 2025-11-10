#include "KCore.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KNpcAttribModify.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

KNpcAttribModify	g_NpcAttribModify;
KNpcAttribModify::KNpcAttribModify()
{
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
	ProcessFunc[magic_physicsresmax_p] = &KNpcAttribModify::PhysicsResMaxP;
	ProcessFunc[magic_coldresmax_p] = &KNpcAttribModify::ColdResMaxP;
	ProcessFunc[magic_fireresmax_p] = &KNpcAttribModify::FireResMaxP;
	ProcessFunc[magic_lightingresmax_p] = &KNpcAttribModify::LightingResMaxP;
	ProcessFunc[magic_poisonresmax_p] = &KNpcAttribModify::PoisonResMaxP;
	ProcessFunc[magic_allresmax_p] = &KNpcAttribModify::AllResMaxP;
	ProcessFunc[magic_lifepotion_v] = &KNpcAttribModify::LifePotionV;
	ProcessFunc[magic_manapotion_v] = &KNpcAttribModify::ManaPotionV;
	ProcessFunc[magic_meleedamagereturn_v] = &KNpcAttribModify::MeleeDamageReturnV;
	ProcessFunc[magic_meleedamagereturn_p] = &KNpcAttribModify::MeleeDamageReturnP;
	ProcessFunc[magic_rangedamagereturn_v] = &KNpcAttribModify::RangeDamageReturnV;
	ProcessFunc[magic_rangedamagereturn_p] = &KNpcAttribModify::RangeDamageReturnP;
	ProcessFunc[magic_damagetomana_p] = &KNpcAttribModify::Damage2ManaP;
	ProcessFunc[magic_adddefense_v] = &KNpcAttribModify::ArmorDefenseV;
	ProcessFunc[magic_poisonenhance_p] = &KNpcAttribModify::PoisonEnhanceP;
	ProcessFunc[magic_lightingenhance_p] = &KNpcAttribModify::LightingEnhanceP;
	ProcessFunc[magic_fireenhance_p] = &KNpcAttribModify::FireEnhanceP;
	ProcessFunc[magic_coldenhance_p] = &KNpcAttribModify::ColdEnhanceP;
	ProcessFunc[magic_armordefense_v] = &KNpcAttribModify::ArmorDefenseV;
	ProcessFunc[magic_lifemax_v] = &KNpcAttribModify::LifeMaxV;
	ProcessFunc[magic_lifemax_p] = &KNpcAttribModify::LifeMaxP;
	ProcessFunc[magic_life_v] = &KNpcAttribModify::LifeV;
	ProcessFunc[magic_lifereplenish_v] = &KNpcAttribModify::LifeReplenishV;
	ProcessFunc[magic_manamax_v] = &KNpcAttribModify::ManaMaxV;
	ProcessFunc[magic_manamax_p] = &KNpcAttribModify::ManaMaxP;
	ProcessFunc[magic_mana_v] = &KNpcAttribModify::ManaV;
	ProcessFunc[magic_manareplenish_v] = &KNpcAttribModify::ManaReplenishV;
	ProcessFunc[magic_staminamax_v] = &KNpcAttribModify::StaminaMaxV;
	ProcessFunc[magic_staminamax_p] = &KNpcAttribModify::StaminaMaxP;
	ProcessFunc[magic_stamina_v] = &KNpcAttribModify::StaminaV;
	ProcessFunc[magic_staminareplenish_v] = &KNpcAttribModify::StaminaReplenishV;
	ProcessFunc[magic_strength_v] = &KNpcAttribModify::StrengthV;
	ProcessFunc[magic_dexterity_v] = &KNpcAttribModify::DexterityV;
	ProcessFunc[magic_vitality_v] = &KNpcAttribModify::VitalityV;
	ProcessFunc[magic_energy_v] = &KNpcAttribModify::EnergyV;
	ProcessFunc[magic_poisonres_p] = &KNpcAttribModify::PoisonresP;
	ProcessFunc[magic_fireres_p] = &KNpcAttribModify::FireresP;
	ProcessFunc[magic_lightingres_p] = &KNpcAttribModify::LightingresP;
	ProcessFunc[magic_physicsres_p] = &KNpcAttribModify::PhysicsresP;
	ProcessFunc[magic_coldres_p] = &KNpcAttribModify::ColdresP;
	ProcessFunc[magic_freezetimereduce_p] = &KNpcAttribModify::FreezeTimeReduceP;
	ProcessFunc[magic_burntimereduce_p] = &KNpcAttribModify::BurnTimeReduceP;
	ProcessFunc[magic_poisontimereduce_p] = &KNpcAttribModify::PoisonTimeReduceP;
	ProcessFunc[magic_poisondamagereduce_v] = &KNpcAttribModify::PoisonDamageReduceV;
	ProcessFunc[magic_stuntimereduce_p] = &KNpcAttribModify::StunTimeReduceP;
	ProcessFunc[magic_fastwalkrun_p] = &KNpcAttribModify::FastWalkRunP;
	ProcessFunc[magic_visionradius_p] = &KNpcAttribModify::VisionRadiusP;
	ProcessFunc[magic_fasthitrecover_v] = &KNpcAttribModify::FastHitRecoverV;
	ProcessFunc[magic_allres_p] = &KNpcAttribModify::AllresP;
	ProcessFunc[magic_attackrating_v] = &KNpcAttribModify::AttackRatingV;
	ProcessFunc[magic_attackratingenhance_v] = &KNpcAttribModify::AttackRatingV;
	ProcessFunc[magic_attackrating_p] = &KNpcAttribModify::AttackRatingP;
	ProcessFunc[magic_attackratingenhance_p] = &KNpcAttribModify::AttackRatingP;
	ProcessFunc[magic_attackspeed_v] = &KNpcAttribModify::AttackSpeedV;
	ProcessFunc[magic_castspeed_v] = &KNpcAttribModify::CastSpeedV;
	ProcessFunc[magic_addphysicsdamage_v] = &KNpcAttribModify::AddPhysicsDamageV;
	ProcessFunc[magic_addfiredamage_v] = &KNpcAttribModify::AddFireDamageV;
	ProcessFunc[magic_addcolddamage_v] = &KNpcAttribModify::AddColdDamageV;
	ProcessFunc[magic_addlightingdamage_v] = &KNpcAttribModify::AddLightingDamageV;
	ProcessFunc[magic_addpoisondamage_v] = &KNpcAttribModify::AddPoisonDamageV;
	ProcessFunc[magic_addphysicsdamage_p] = &KNpcAttribModify::AddPhysicsDamageP;
	ProcessFunc[magic_slowmissle_b] = &KNpcAttribModify::SlowMissleB;
	ProcessFunc[magic_changecamp_b] = &KNpcAttribModify::ChangeCampV;
	ProcessFunc[magic_physicsarmor_v] = &KNpcAttribModify::PhysicsArmorV;
	ProcessFunc[magic_coldarmor_v] = &KNpcAttribModify::ColdArmorV;
	ProcessFunc[magic_firearmor_v] = &KNpcAttribModify::FireArmorV;
	ProcessFunc[magic_poisonarmor_v] = &KNpcAttribModify::PoisonArmorV;
	ProcessFunc[magic_lightingarmor_v] = &KNpcAttribModify::LightingArmorV;
	ProcessFunc[magic_lucky_v] = &KNpcAttribModify::LuckyV;
	ProcessFunc[magic_steallife_p] = &KNpcAttribModify::StealLifeP;
	ProcessFunc[magic_steallifeenhance_p] = &KNpcAttribModify::StealLifeP;
	ProcessFunc[magic_stealstamina_p] = &KNpcAttribModify::StealStaminaP;
	ProcessFunc[magic_stealstaminaenhance_p] = &KNpcAttribModify::StealStaminaP;
	ProcessFunc[magic_stealmana_p] = &KNpcAttribModify::StealManaP;
	ProcessFunc[magic_stealmanaenhance_p] = &KNpcAttribModify::StealManaP;
	ProcessFunc[magic_allskill_v] = &KNpcAttribModify::AllSkillV;
	ProcessFunc[magic_metalskill_v] = &KNpcAttribModify::MetalSkillV;
	ProcessFunc[magic_woodskill_v] = &KNpcAttribModify::WoodSkillV;
	ProcessFunc[magic_waterskill_v] = &KNpcAttribModify::WaterSkillV;
	ProcessFunc[magic_fireskill_v] = &KNpcAttribModify::FireSkillV;
	ProcessFunc[magic_earthskill_v] = &KNpcAttribModify::EarthSkillV;
	ProcessFunc[magic_knockback_p] = &KNpcAttribModify::KnockBackP;
	ProcessFunc[magic_knockbackenhance_p] = &KNpcAttribModify::KnockBackP;
	ProcessFunc[magic_deadlystrike_p] = &KNpcAttribModify::DeadlyStrikeP;
	ProcessFunc[magic_deadlystrikeenhance_p] = &KNpcAttribModify::DeadlyStrikeP;
	ProcessFunc[magic_badstatustimereduce_v] = &KNpcAttribModify::FatallyStrikeP;
	ProcessFunc[magic_manashield_p] = &KNpcAttribModify::ManaShieldP;
	
	ProcessFunc[magic_addphysicsmagic_v] = &KNpcAttribModify::AddPhysicsMagicV;
	ProcessFunc[magic_addcoldmagic_v] = &KNpcAttribModify::AddColdMagicV;
	ProcessFunc[magic_addfiremagic_v] = &KNpcAttribModify::AddFireMagicV;
	ProcessFunc[magic_addlightingmagic_v] = &KNpcAttribModify::AddLightingMagicV;
	ProcessFunc[magic_addpoisonmagic_v] = &KNpcAttribModify::AddPoisonMagicV;
	ProcessFunc[magic_dynamicmagicshield_v] = &KNpcAttribModify::DynamicMagicShieldV;
	ProcessFunc[magic_returnres_p] = &KNpcAttribModify::ReturnResP;
	ProcessFunc[magic_expenhance_p] = &KNpcAttribModify::ExpEnhanceP;
	ProcessFunc[magic_poisondamagereturn_p] = &KNpcAttribModify::PoisonDamageReturnP;
	ProcessFunc[magic_returnskill_p] = &KNpcAttribModify::ReturnSkillP;
	ProcessFunc[magic_autoreplyskill] = &KNpcAttribModify::AutoReplySkill;
	ProcessFunc[magic_lifereplenish_p] = &KNpcAttribModify::LifeReplenishP;
	ProcessFunc[magic_ignoreskill_p] = &KNpcAttribModify::IgnoreSkillP;
	ProcessFunc[magic_autoattackskill] = &KNpcAttribModify::AutoAttackSkill;
	ProcessFunc[magic_poison2decmana_p] = &KNpcAttribModify::Poison2DecManaP;
	ProcessFunc[magic_skill_appendskill] = &KNpcAttribModify::SkillAppendSkill;
	ProcessFunc[magic_hide] = &KNpcAttribModify::HideNpc;
	ProcessFunc[magic_autodeathskill] = &KNpcAttribModify::AutoDeathSkill;
	ProcessFunc[magic_randmove] = &KNpcAttribModify::RandMove;
	ProcessFunc[magic_ignorenegativestate_p] = &KNpcAttribModify::IgnoreNegativeStateP;
	ProcessFunc[magic_autorescueskill] = &KNpcAttribModify::AutoRescueSkill;
	ProcessFunc[magic_staticmagicshield_p] = &KNpcAttribModify::StaticMagicShieldP;

	ProcessFunc[magic_poisonres_yan_p] = &KNpcAttribModify::PoisonresP;
	ProcessFunc[magic_fireres_yan_p] = &KNpcAttribModify::FireresP;
	ProcessFunc[magic_lightingres_yan_p] = &KNpcAttribModify::LightingresP;
	ProcessFunc[magic_physicsres_yan_p] = &KNpcAttribModify::PhysicsresP;
	ProcessFunc[magic_coldres_yan_p] = &KNpcAttribModify::ColdresP;
	ProcessFunc[magic_lifemax_yan_v] = &KNpcAttribModify::LifeMaxV;
	ProcessFunc[magic_lifemax_yan_p] = &KNpcAttribModify::LifeMaxP;
	ProcessFunc[magic_manamax_yan_v] = &KNpcAttribModify::ManaMaxV;
	ProcessFunc[magic_manamax_yan_p] = &KNpcAttribModify::ManaMaxP;
	ProcessFunc[magic_attackspeed_yan_v] = &KNpcAttribModify::AttackSpeedV;
	ProcessFunc[magic_castspeed_yan_v] = &KNpcAttribModify::CastSpeedV;
	ProcessFunc[magic_allres_yan_p] = &KNpcAttribModify::AllresP;
	ProcessFunc[magic_fasthitrecover_yan_v] = &KNpcAttribModify::FastHitRecoverV;
	
}


//	

KNpcAttribModify::~KNpcAttribModify()
{
}


//	

void KNpcAttribModify::ModifyAttrib(KNpc* pNpc, void* pData)
{
	if (!pData || !pNpc)
		return;

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nAttribType < 0 || pMagic->nAttribType >= magic_normal_end || NULL == ProcessFunc[pMagic->nAttribType])
		return;
	
	(this->*ProcessFunc[pMagic->nAttribType])(pNpc, pData);
}


//	

void KNpcAttribModify::AddColdDamageV(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentColdDamage.nValue[2] += pMagic->nValue[0];
	if (pNpc->m_CurrentColdDamage.nValue[0] > 0 && pNpc->m_CurrentColdDamage.nValue[2] > 0)
	{	
		//
		pNpc->m_CurrentColdDamage.nValue[1] = pNpc->m_CurrentColdDamage.nValue[0] * defNORMAL_COLD_TIME / 100;
		if(pNpc->m_CurrentColdDamage.nValue[1] > defNORMAL_COLD_TIME)
			pNpc->m_CurrentColdDamage.nValue[1] = defNORMAL_COLD_TIME;
		
	}
	else
	{
		pNpc->m_CurrentColdDamage.nValue[0] = 0;
		pNpc->m_CurrentColdDamage.nValue[1] = 0;
		pNpc->m_CurrentColdDamage.nValue[2] = 0;
	}
}


//	

void KNpcAttribModify::AddFireDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentFireDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[数值]%s火伤害增加(%d)", pNpc->Name, pMagic->nValue[0]);
}


//	

void KNpcAttribModify::AddLightingDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentLightDamage.nValue[2] += pMagic->nValue[0];
	//g_DebugLog("[数值]%s电伤害增加(%d)", pNpc->Name, pMagic->nValue[0]);
}


//	

void KNpcAttribModify::AddPhysicsDamageP(KNpc* pNpc, void* pData)
{
#define WEAPON_ALL			((MAX_MELEE_WEAPON) + 0)
#define	WEAPON_RANGE_ALL	((MAX_MELEE_WEAPON) + 1)
#define	WEAPON_MELEE_ALL	((MAX_MELEE_WEAPON) + 2)
#define	WEAPON_NONE			((MAX_MELEE_WEAPON) + 3)

	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	int nType = abs(pMagic->nValue[2]);

	if (nType == WEAPON_ALL)
	{
		for (int i = 0; i < MAX_MELEE_WEAPON; i++)
		{
			pNpc->m_CurrentMeleeEnhance[i] += pMagic->nValue[0];
		}
		pNpc->m_CurrentHandEnhance += pMagic->nValue[0];
		pNpc->m_CurrentRangeEnhance += pMagic->nValue[0];
	}
	else if (nType == WEAPON_RANGE_ALL)
	{
		pNpc->m_CurrentRangeEnhance += pMagic->nValue[0];
	}
	else if (nType == WEAPON_NONE)
	{
		pNpc->m_CurrentHandEnhance += pMagic->nValue[0];
	}
	else if (nType == WEAPON_MELEE_ALL)
	{
		for (int i = 0; i < MAX_MELEE_WEAPON; i++)
		{
			pNpc->m_CurrentMeleeEnhance[i] += pMagic->nValue[0];
		}
	}
	else if (nType >= 0 && nType < MAX_MELEE_WEAPON)
	{
		pNpc->m_CurrentMeleeEnhance[nType] += pMagic->nValue[0];
	}
}


//	

void KNpcAttribModify::AddPoisonDamageV(KNpc* pNpc, void* pData)
{
#define	defPOISON_DAMAGE_TIME		60
#define	defPOISON_DAMAGE_INTERVAL	10
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonDamage.nValue[0] += pMagic->nValue[0];

	if (pNpc->m_CurrentPoisonDamage.nValue[0] > 0)
	{
		pNpc->m_CurrentPoisonDamage.nValue[1] = defPOISON_DAMAGE_TIME;//pMagic->nValue[1];
		pNpc->m_CurrentPoisonDamage.nValue[2] = defPOISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
	}
	else
	{
		pNpc->m_CurrentPoisonDamage.nValue[0] = 0;
		pNpc->m_CurrentPoisonDamage.nValue[1] = 0;
		pNpc->m_CurrentPoisonDamage.nValue[2] = 0;
	}	
	//g_DebugLog("[数值]%s毒状态变为：间隔%d帧发作一次%d伤害，共持续%d帧", pNpc->Name, pNpc->m_CurrentPoisonDamage.nValue[2], pNpc->m_CurrentPoisonDamage.nValue[0], pNpc->m_CurrentPoisonDamage.nValue[1]);	
}


//	

void KNpcAttribModify::AddPhysicsDamageV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAddPhysicsDamage += pMagic->nValue[0];
}


//	

void KNpcAttribModify::AllresP(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResist += pMagic->nValue[0];
	pNpc->m_CurrentColdResist += pMagic->nValue[0];
	pNpc->m_CurrentLightResist += pMagic->nValue[0];
	pNpc->m_CurrentPoisonResist += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsResist += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[magic_allres_p] Name: %s - Value: %d - Add: %d", pNpc->Name, pNpc->m_CurrentFireResist, pMagic->nValue[0]);
#endif
}


//	

void KNpcAttribModify::AllSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;

	if (pNpc->m_nPlayerIdx <= 0)
		return;
	
	if(pMagic->nValue[2] <= 0 && pMagic->nValue[0] > 0)
	{
		Player[pNpc->m_nPlayerIdx].AddAllSkillPoint(pMagic->nValue[0]);
	}
	else
	{
		Player[pNpc->m_nPlayerIdx].AddSkillPoint(pMagic->nValue[2], pMagic->nValue[0]);
	}
#ifdef _DEBUG
	//g_DebugLog("[magic_allskill_v] Name: %s - SkillID: %d - PointAdd: %d", pNpc->Name, pMagic->nValue[2], pMagic->nValue[0]);
#endif
}

void KNpcAttribModify::AttackRatingP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRating += pNpc->m_AttackRating * pMagic->nValue[0] / 100;
	//g_DebugLog("[数值]%s命中率增加百分之%d(%d点)", pNpc->Name, pMagic->nValue[0], pNpc->m_AttackRating * pMagic->nValue[0] / 100);
}

void KNpcAttribModify::AttackRatingV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackRating += pMagic->nValue[0];
	//g_DebugLog("[数值]%s五防抗性增加%d", pNpc->Name, pMagic->nValue[0]);
	
}

void KNpcAttribModify::AttackSpeedV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentAttackSpeed += pMagic->nValue[0];
	//g_DebugLog("[数值]%s攻击速度增加%d", pNpc->Name, pMagic->nValue[0]);
	
}

void KNpcAttribModify::BadStatusTimeReduceV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	return;
}

void KNpcAttribModify::BurnTimeReduceP(KNpc* pNpc, void* pData)
{
	return;
}

void KNpcAttribModify::CastSpeedV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentCastSpeed += pMagic->nValue[0];
	//g_DebugLog("[数值]%s施法速度增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ChangeCampV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
	{
		if (pMagic->nValue[0] > 0 && pMagic->nValue[0] < camp_num)
			pNpc->m_CurrentCamp = pMagic->nValue[0];
		else
			pNpc->m_CurrentCamp = pNpc->m_Camp;
		//g_DebugLog("[数值]%s当前阵营变为%d", pNpc->Name, pMagic->nValue[0]);
	}
	
}

void KNpcAttribModify::ColdArmorV(KNpc* pNpc, void* pData)
{
	//KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}

void KNpcAttribModify::ColdresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdResist += pMagic->nValue[0];
	//g_DebugLog("[数值]%s冰抗性增加了%d点", pNpc->Name, pMagic->nValue[0]);
}


//

void KNpcAttribModify::DeadlyStrikeP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDeadlyStrike += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[deadlystrike_p] Name: % - Value: %d", pNpc->Name, pMagic->nValue[0]);
#endif
}

void KNpcAttribModify::DexterityV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurDexterity(pMagic->nValue[0]);
	//g_DebugLog("[数值]%s当前敏捷增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::EarthSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	//g_DebugLog("[数值]%s土系技能加%d级", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::EnergyV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurEngergy(pMagic->nValue[0]);
	//g_DebugLog("[数值]%s当前精力增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::FastHitRecoverV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentHitRecover += pMagic->nValue[0];
	//g_DebugLog("[数值]%s快速命中回复增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FastWalkRunP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	// 文档要求修改为直接加点数
	pNpc->m_CurrentWalkSpeed += (pNpc->m_WalkSpeed * pMagic->nValue[0]) / 100;
	pNpc->m_CurrentRunSpeed += (pNpc->m_RunSpeed * pMagic->nValue[0]) / 100;
	//g_DebugLog("[数值]%s移动速度增加%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::FireArmorV(KNpc* pNpc, void* pData)
{
	//KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}

void KNpcAttribModify::FireSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	//g_DebugLog("[数值]%s火系技能加%d级", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FreezeTimeReduceP(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFreezeTimeReducePercent += pMagic->nValue[0];
}

void KNpcAttribModify::KnockBackP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentKnockBack += pMagic->nValue[0];
	//g_DebugLog("[数值]%s震退百分比增加%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::LifeMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeMax += UINT(pNpc->m_LifeMax * float(pMagic->nValue[0] / 100.0));
#ifdef _DEBUG
	//g_DebugLog("[lifemax_p] Name: %s - Value: %d - Add: %d", pNpc->Name, pMagic->nValue[0], UINT(pNpc->m_LifeMax * float(pMagic->nValue[0] / 100.0)));	
#endif
}

void KNpcAttribModify::LifeMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s生命上限增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LifeReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeReplenish += pMagic->nValue[0];
	//g_DebugLog("[数值]%s生命回复速度增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LifeV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLife += pMagic->nValue[0];
	//g_DebugLog("[数值]%s生命增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LightingArmorV(KNpc* pNpc, void* pData)
{
	//KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}

void KNpcAttribModify::LightingresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightResist += pMagic->nValue[0];
	//g_DebugLog("[数值]%s电抗性增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::LuckyV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->IsPlayer())
	{	
		Player[pNpc->m_nPlayerIdx].m_nCurLucky += pMagic->nValue[0];

	}
}

void KNpcAttribModify::ManaMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaMax += pNpc->m_ManaMax * pMagic->nValue[0] / 100;
	//g_DebugLog("[数值]%s内力上限增加了百分之%d(%d点)", pNpc->Name, pMagic->nValue[0], pNpc->m_ManaMax * pMagic->nValue[0] / 100);	
}

void KNpcAttribModify::ManaMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s内力上限增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ManaReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaReplenish += pMagic->nValue[0];
	//g_DebugLog("[数值]%s内力回复速度增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ManaV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMana += pMagic->nValue[0];
	//g_DebugLog("[数值]%s内力增加了%d点", pNpc->Name, pMagic->nValue[0]);
}


//	

void KNpcAttribModify::ManaShieldP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_ManaShield += pMagic->nValue[0];

#ifdef _DEBUG
	//g_DebugLog("[manashield_p] Name: %s - Value: %d - Add: %d", pNpc->Name, pNpc->m_ManaShield, pMagic->nValue[0]);
#endif
}

void KNpcAttribModify::MeleeDamageReturnP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMeleeDmgRetPercent += pMagic->nValue[0];
	//g_DebugLog("[数值]%s近程物理伤害反弹百分比增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::MeleeDamageReturnV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMeleeDmgRet += pMagic->nValue[0];
	//g_DebugLog("[数值]%s近程物理伤害反弹增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::MetalSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	//g_DebugLog("[数值]%s金系技能加%d级, 未完成", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PhysicsArmorV(KNpc* pNpc, void* pData)
{
	//KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}

void KNpcAttribModify::PhysicsresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPhysicsResist += pMagic->nValue[0];
	//g_DebugLog("[数值]%s物理抗性增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::Damage2ManaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDamage2Mana += pMagic->nValue[0];
	//g_DebugLog("[数值]%s伤害转内力百分比增加了%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::PoisonArmorV(KNpc* pNpc, void* pData)
{
	//KMagicAttrib* pMagic = (KMagicAttrib *)pData;
}

void KNpcAttribModify::PoisonDamageReduceV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_PoisonState.nValue[0] -= pMagic->nValue[0];
	if (pNpc->m_PoisonState.nValue[0] <= 0)
	{
		pNpc->m_PoisonState.nTime = 0;
	}
	//g_DebugLog("[数值]%s中毒伤害减少%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PoisonresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonResist += pMagic->nValue[0];
	//g_DebugLog("[数值]%s毒抗性增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PoisonTimeReduceP(KNpc* pNpc, void* pData)
{
// 装备里，需要修改TODO
//	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
//	pNpc->m_PoisonState.nTime *= (100 - pMagic->nValue[0]);
//	pNpc->m_PoisonState.nTime /= 100;
//	if (pNpc->m_PoisonState.nTime <= 0)
//	{
//		pNpc->m_PoisonState.nTime = 0;
//	}
//	//g_DebugLog("[数值]%s中毒时间减少百分之%d", pNpc->Name, pMagic->nValue[0]);	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonTimeReducePercent += pMagic->nValue[0];
	//g_DebugLog("[数值]%s中毒时间减少百分之%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::RangeDamageReturnV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentRangeDmgRet += pMagic->nValue[0];
	//g_DebugLog("[数值]%s远程伤害反弹增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::RangeDamageReturnP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentRangeDmgRetPercent += pMagic->nValue[0];
	//g_DebugLog("[数值]%s远程伤害反弹百分比增加%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::SlowMissleB(KNpc* pNpc, void* pData) // fix hien thi res skills 15x by kinnox;
{
	/*KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[0] > 0)
		pNpc->m_CurrentSlowMissle = 1;
	else
		pNpc->m_CurrentSlowMissle = 0;
	//g_DebugLog("[数值]%s子弹减速状态变为%d", pNpc->Name, pMagic->nValue[0]);*/
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_CurrentSlowMissle += pMagic->nValue[0];

	if (pNpc->m_CurrentSlowMissle > 0)
		pNpc->m_CurrentSlowMissle = 1;
	else
		pNpc->m_CurrentSlowMissle = 0;
}

void KNpcAttribModify::StaminaMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaMax += pNpc->m_StaminaMax * pMagic->nValue[0] / 100;
	//g_DebugLog("[数值]%s体力上限增加了百分之%d(%d点)", pNpc->Name, pMagic->nValue[0], pNpc->m_StaminaMax * pMagic->nValue[0] / 100);	
}

void KNpcAttribModify::StaminaMaxV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s体力上限增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StaminaReplenishV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaGain += pMagic->nValue[0];
	//g_DebugLog("[数值]%s体力回复速度增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StaminaV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStamina += pMagic->nValue[0];
	//g_DebugLog("[数值]%s体力增加了%d点", pNpc->Name, pMagic->nValue[0]);
}


void KNpcAttribModify::StealLifeP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLifeStolen += pMagic->nValue[0];
	//g_DebugLog("[数值]%s偷生命百分比为%d(比原本上升了%d点)", pNpc->Name, pNpc->m_CurrentLifeStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StealManaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentManaStolen += pMagic->nValue[0];
	//g_DebugLog("[数值]%s偷内力百分比为%d(比原本上升了%d点)", pNpc->Name, pNpc->m_CurrentManaStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StealStaminaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStaminaStolen += pMagic->nValue[0];
	//g_DebugLog("[数值]%s偷体力百分比为%d(比原本上升了%d点)", pNpc->Name, pNpc->m_CurrentStaminaStolen, pMagic->nValue[0]);	
}

void KNpcAttribModify::StrengthV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurStrength(pMagic->nValue[0]);
	//g_DebugLog("[数值]%s当前力量增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::StunTimeReduceP(KNpc* pNpc, void* pData)
{	
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentStunTimeReducePercent += pMagic->nValue[0];
	//g_DebugLog("[数值]%s眩晕时间减少%d", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::VisionRadiusP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentVisionRadius += pMagic->nValue[0];
	//g_DebugLog("[数值]%s视野范围增加%d", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::VitalityV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if (pNpc->m_Kind != kind_player)
		return;
	if (pNpc->m_nPlayerIdx <= 0)
		return;
	Player[pNpc->m_nPlayerIdx].ChangeCurVitality(pMagic->nValue[0]);
	//g_DebugLog("[数值]%s当前活力增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::WaterSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	//g_DebugLog("[数值]%s水系技能加%d级，未完成", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::WoodSkillV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	//g_DebugLog("[数值]%s木系技能加%d级，未完成", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::FireresP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResist += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[magic_fireres_p]Name: %s - Value:%d - Add: %d", pNpc->Name, pNpc->m_CurrentFireResist, pMagic->nValue[0]);	
#endif
}

void KNpcAttribModify::ArmorDefenseV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentDefend += pMagic->nValue[0];
	//g_DebugLog("[数值]%s防御增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::ColdEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdEnhance += pMagic->nValue[0];
	//g_DebugLog("[数值]%s冰增强增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::FireEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireEnhance += pMagic->nValue[0];
	//g_DebugLog("[数值]%s火增强增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LightingEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightEnhance += pMagic->nValue[0];
	//g_DebugLog("[数值]%s电增强增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::PoisonEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonEnhance += pMagic->nValue[0];
	//g_DebugLog("[数值]%s毒增强增加了%d点", pNpc->Name, pMagic->nValue[0]);
}

void KNpcAttribModify::LifePotionV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[1] <= 0)
		return;
	int nX1, nY1, nX2, nY2;
	nX1 = pNpc->m_LifeState.nValue[0];
	nY1 = pNpc->m_LifeState.nTime;
	nX2 = pMagic->nValue[0];
	nY2 = pMagic->nValue[1];
	pNpc->m_LifeState.nTime = max(nY1, nY2);
	// 传入参数保证时间大于零，所以最大值一定大于零，所以不用判断除零
	pNpc->m_LifeState.nValue[0] = (nX1 * nY1 + nX2 * nY2) / pNpc->m_LifeState.nTime;
	//g_DebugLog("[数值]%s喝了补血药，获得%d帧内每10帧回复%d点生命的效果", pNpc->Name,pNpc->m_LifeState.nTime,pNpc->m_LifeState.nValue[0]);
}

void KNpcAttribModify::ManaPotionV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[1] <= 0)
		return;
	int nX1, nY1, nX2, nY2;
	nX1 = pNpc->m_ManaState.nValue[0];
	nY1 = pNpc->m_ManaState.nTime;
	nX2 = pMagic->nValue[0];
	nY2 = pMagic->nValue[1];
	pNpc->m_ManaState.nTime = max(nY1, nY2);
	// 传入参数保证时间大于零，所以最大值一定大于零，所以不用判断除零
	pNpc->m_ManaState.nValue[0] = (nX1 * nY1 + nX2 * nY2) / pNpc->m_ManaState.nTime;
	//g_DebugLog("[数值]%s喝了补内力药，获得%d帧内每10帧回复%d点内力的效果", pNpc->Name,pNpc->m_ManaState.nTime,pNpc->m_ManaState.nValue[0]);
}

void KNpcAttribModify::PhysicsResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_CurrentPhysicsResistMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s物理抗性上限增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::ColdResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentColdResistMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s冰抗性上限增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::FireResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResistMax += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[magic_fireresmax_p]Name: %s - Value:%d - Add: %d", pNpc->Name, pNpc->m_CurrentFireResistMax, pMagic->nValue[0]);	
#endif
}

void KNpcAttribModify::LightingResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentLightResistMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s电抗性上限增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::PoisonResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentPoisonResistMax += pMagic->nValue[0];
	//g_DebugLog("[数值]%s毒抗性上限增加了%d点", pNpc->Name, pMagic->nValue[0]);	
}

void KNpcAttribModify::AllResMaxP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentFireResistMax += pMagic->nValue[0];
	pNpc->m_CurrentColdResistMax += pMagic->nValue[0];
	pNpc->m_CurrentLightResistMax += pMagic->nValue[0];
	pNpc->m_CurrentPoisonResistMax += pMagic->nValue[0];
	pNpc->m_CurrentPhysicsResistMax += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[magic_allresmax_p]Name: %s - Value:%d - Add: %d", pNpc->Name, pNpc->m_CurrentFireResistMax, pMagic->nValue[0]);	
#endif
}

void KNpcAttribModify::FatallyStrikeP(KNpc* pNpc, void* pData)
{
}

void KNpcAttribModify::MixPoisonDamage(KMagicAttrib* pDes, KMagicAttrib* pSrc)
{
	int d1, d2, t1, t2, c1, c2;
	d1 = pDes->nValue[0];
	d2 = pSrc->nValue[0];
	t1 = pDes->nValue[1];
	t2 = pSrc->nValue[1];
	c1 = pDes->nValue[2];
	c2 = pSrc->nValue[2];
	if (c1 == 0 || d1 == 0)
	{
		memcpy(pDes, pSrc, sizeof(KMagicAttrib));
		return;
	}
	if (c2 == 0 || d2 == 0)
	{
		return;
	}
	pDes->nValue[0] = ((c1 + c2) * d1 / c1 + (c1 + c2) * d2 / c2) / 2;
	pDes->nValue[1] = (t1 * d1 * c2 + t2 *d2 * c1) /(d1 * c2 + d2 * c1);
	pDes->nValue[2] = (c1 + c2) / 2;
}

// --
//
// --
void KNpcAttribModify::AddPhysicsMagicV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_MagicPhysicsDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_MagicPhysicsDamage.nValue[2] += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[addphysicsmagic_v] Name: %s - Value1: %d - Value2: %d", pNpc->Name, pNpc->m_MagicPhysicsDamage.nValue[0], pNpc->m_MagicPhysicsDamage.nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::AddColdMagicV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicColdDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentMagicColdDamage.nValue[2] += pMagic->nValue[0];
	if (pNpc->m_CurrentMagicColdDamage.nValue[0] > 0 && pNpc->m_CurrentMagicColdDamage.nValue[2] > 0)
	{
		//
		pNpc->m_CurrentMagicColdDamage.nValue[1] = pNpc->m_CurrentMagicColdDamage.nValue[0] * defNORMAL_COLD_TIME / 100;
		if(pNpc->m_CurrentMagicColdDamage.nValue[1] > defNORMAL_COLD_TIME)
			pNpc->m_CurrentMagicColdDamage.nValue[1] = defNORMAL_COLD_TIME;
		
	}
	else
	{
		pNpc->m_CurrentMagicColdDamage.nValue[0] = 0;
		pNpc->m_CurrentMagicColdDamage.nValue[1] = 0;
		pNpc->m_CurrentMagicColdDamage.nValue[2] = 0;
	}
#ifdef _DEBUG
	//g_DebugLog("[addcoldmagic_v] Name: %s - Value1: %d - Value2: %d - Value3: %d", pNpc->Name, pNpc->m_CurrentMagicColdDamage.nValue[0], pNpc->m_CurrentMagicColdDamage.nValue[1], pNpc->m_CurrentMagicColdDamage.nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::AddFireMagicV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicFireDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentMagicFireDamage.nValue[2] += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[addfiremagic_v] Name: %s - Value1: %d - Value2: %d", pNpc->Name, pNpc->m_CurrentMagicFireDamage.nValue[0], pNpc->m_CurrentMagicFireDamage.nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::AddLightingMagicV( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicLightDamage.nValue[0] += pMagic->nValue[0];
	pNpc->m_CurrentMagicLightDamage.nValue[2] += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[addlightingmagic_v] Name: %s - Value1: %d - Value2: %d", pNpc->Name, pNpc->m_CurrentMagicLightDamage.nValue[0], pNpc->m_CurrentMagicLightDamage.nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::AddPoisonMagicV( KNpc* pNpc, void* pData )
{
#define	defPOISON_DAMAGE_TIME		60
#define	defPOISON_DAMAGE_INTERVAL	10
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentMagicPoisonDamage.nValue[0] += pMagic->nValue[0];
	
	if (pNpc->m_CurrentMagicPoisonDamage.nValue[0] > 0)
	{
		pNpc->m_CurrentMagicPoisonDamage.nValue[1] = defPOISON_DAMAGE_TIME;
		pNpc->m_CurrentMagicPoisonDamage.nValue[2] = defPOISON_DAMAGE_INTERVAL;
	}
	else
	{
		pNpc->m_CurrentMagicPoisonDamage.nValue[0] = 0;
		pNpc->m_CurrentMagicPoisonDamage.nValue[1] = 0;
		pNpc->m_CurrentMagicPoisonDamage.nValue[2] = 0;
	}
#ifdef _DEBUG
	//g_DebugLog("[addpoisonmagic_v] Name: %s - Value1: %d - Value2: %d - Value3: %d", pNpc->Name, pNpc->m_CurrentMagicPoisonDamage.nValue[0], pNpc->m_CurrentMagicPoisonDamage.nValue[1], pNpc->m_CurrentMagicPoisonDamage.nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::DynamicMagicShieldV(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_DynamicMagicShield += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[dynamicmagicshield_v] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_DynamicMagicShield, pMagic->nValue[0]);
#endif
}

// --
//
// --
void KNpcAttribModify::ReturnResP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentReturnResist += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[returnres_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_CurrentReturnResist, pMagic->nValue[0]);
#endif
}

// --
//	
// --
void KNpcAttribModify::ExpEnhanceP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_CurrentExpEnhance += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[expenhance_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_CurrentExpEnhance, pMagic->nValue[0]);
#endif
}

// --
//	Phan don doc sat (percent)
// --
void KNpcAttribModify::PoisonDamageReturnP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_PoisonDamageReturn += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[poisondamagereturn_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_PoisonDamageReturn, pMagic->nValue[0]);
#endif
}

// --
//	Phan bua chu 
// --
void KNpcAttribModify::ReturnSkillP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_ReturnSkill += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[returnskill_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_ReturnSkill, pMagic->nValue[0]);
#endif
}

// --
//	Tu dong phan skill
// --
void KNpcAttribModify::AutoReplySkill(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if(pMagic->nValue[0] > 0)
	{
		pNpc->m_AutoReplySkill.nValue[0] = pMagic->nValue[0];
		pNpc->m_AutoReplySkill.nValue[1] = pMagic->nValue[1];
		pNpc->m_AutoReplySkill.nValue[2] = pMagic->nValue[2];
	}
	else
	{
		pNpc->m_AutoReplySkill.nValue[0] = 0;
		pNpc->m_AutoReplySkill.nValue[1] = 0;
		pNpc->m_AutoReplySkill.nValue[2] = 0;
	}
#ifdef _DEBUG
	//g_DebugLog("[autoreplyskill] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_AutoReplySkill.nValue[2], pMagic->nValue[2]);
#endif
}

// --
//	Hoi phuc sinh luc (percent)
// --
void KNpcAttribModify::LifeReplenishP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_LifeReplenishPercent += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[lifereplenish_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_LifeReplenishPercent, pMagic->nValue[0]);
#endif
}

// --
//	Bo qua bua chu giam khang
// --
void KNpcAttribModify::IgnoreSkillP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_IgnoreSkill	+= pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[ignoreskill_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_IgnoreSkill, pMagic->nValue[0]);
#endif
}

// --
//	Tu dong thuc thi ky nang
// --
void KNpcAttribModify::AutoAttackSkill(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_AutoAttackSkill.nValue[0] = pMagic->nValue[0];
	pNpc->m_AutoAttackSkill.nValue[1] = pMagic->nValue[1];
	pNpc->m_AutoAttackSkill.nValue[2] = pMagic->nValue[2];
#ifdef _DEBUG
	//g_DebugLog("[autoattackskill] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_AutoAttackSkill.nValue[2], pMagic->nValue[2]);
#endif
}

// --
//	Doc rut mana
// --
void KNpcAttribModify::Poison2DecManaP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	if(pMagic->nValue[0] > 0)
	{
		pNpc->m_Poison2DecManaState.nValue[0] = pMagic->nValue[0];
		pNpc->m_Poison2DecManaState.nValue[1] = pMagic->nValue[2];
		pNpc->m_Poison2DecManaState.nTime = pMagic->nValue[1];
	}
	else
	{
		pNpc->m_Poison2DecManaState.nValue[0] = 0;
		pNpc->m_Poison2DecManaState.nValue[1] = 0;
		pNpc->m_Poison2DecManaState.nTime = 0;
	}
#ifdef _DEBUG
	//g_DebugLog("[poison2decmana_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_Poison2DecManaState.nValue[0], pMagic->nValue[0]);
#endif
}

// --
//
// --
void KNpcAttribModify::SkillAppendSkill(KNpc* pNpc, void* pData) 
{
		KMagicAttrib* pMagic = (KMagicAttrib *)pData;
		if (pMagic->nValue[0] > 0)
		{
			if(pNpc->m_AppendSkill[0] <= 0)
			{
				pNpc->m_AppendSkill[0] = pMagic->nValue[0];
				return;
			}
			if(pNpc->m_AppendSkill[1] <= 0)
			{
				pNpc->m_AppendSkill[1] = pMagic->nValue[0];
				return;
			}
			if(pNpc->m_AppendSkill[2] <= 0)
			{
				pNpc->m_AppendSkill[2] = pMagic->nValue[0];
				return;
			}
			if(pNpc->m_AppendSkill[3] <= 0)
			{
				pNpc->m_AppendSkill[3] = pMagic->nValue[0];
				return;
			}
			if(pNpc->m_AppendSkill[4] <= 0)
			{
				pNpc->m_AppendSkill[4] = pMagic->nValue[0];
				return;
			}
		}
		else
		{
			if(abs(pNpc->m_AppendSkill[0]) <= abs(pMagic->nValue[0]))
			{
				pNpc->m_AppendSkill[0] = 0;
				return;
			}
			if(abs(pNpc->m_AppendSkill[1]) <= abs(pMagic->nValue[0]))
			{
				pNpc->m_AppendSkill[0] = 0;
				return;
			}
			if(abs(pNpc->m_AppendSkill[2]) <= abs(pMagic->nValue[0]))
			{
				pNpc->m_AppendSkill[0] = 0;
				return;
			}
			if(abs(pNpc->m_AppendSkill[3]) <= abs(pMagic->nValue[0]))
			{
				pNpc->m_AppendSkill[0] = 0;
				return;
			}
			if(abs(pNpc->m_AppendSkill[4]) <= abs(pMagic->nValue[0]))
			{
				pNpc->m_AppendSkill[0] = 0;
				return;
			}
		}
}

// --
//	Tang hinh
// --
void KNpcAttribModify::HideNpc(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	pNpc->m_HideState.nValue[0] = pMagic->nValue[0];
	pNpc->m_HideState.nTime = pMagic->nValue[1];
#ifdef _DEBUG
	//g_DebugLog("[hide] Name: %s - Time: %d - Add: %d", pNpc->Name,  pNpc->m_HideState.nTime, pMagic->nValue[1]);
#endif
}

// --
//
// --
void KNpcAttribModify::AutoDeathSkill(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_AutoDeathSkill.nValue[0] = pMagic->nValue[0];
	pNpc->m_AutoDeathSkill.nValue[1] = pMagic->nValue[1];
	pNpc->m_AutoDeathSkill.nValue[2] = pMagic->nValue[2];
#ifdef _DEBUG
	//g_DebugLog("[autodeathskill] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_AutoDeathSkill.nValue[0], pMagic->nValue[0]);
#endif
}

// --
//
// --
void KNpcAttribModify::RandMove( KNpc* pNpc, void* pData )
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;

	if (pMagic->nValue[1] > 0)
	{
		pNpc->m_RandMoveState.nTime = pMagic->nValue[1];
	}
	else
	{
		pNpc->m_RandMoveState.nTime = 0;
	}
#ifdef _DEBUG
	//g_DebugLog("[randmove] Name: %s - Time: %d - Add: %d", pNpc->Name, pNpc->m_RandMoveState.nTime, pMagic->nValue[1]);
#endif
}

// --
//
// --
void KNpcAttribModify::IgnoreNegativeStateP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	
	if(pMagic->nValue[0] > 0)
	{
		pNpc->m_IgnoreNegativeState.nValue[0] = pMagic->nValue[0];
		pNpc->m_IgnoreNegativeState.nTime = pMagic->nValue[1];
	}
	else if(pMagic->nValue[0] != -1)
	{
		pNpc->m_IgnoreNegativeState.nValue[0] = 0;
		pNpc->m_IgnoreNegativeState.nTime = 0;

	}
#ifdef _DEBUG
	//g_DebugLog("[ignorenegativestate_p] Name: %s - Value: %d - Add: %d - Time: %d - Add: %d", pNpc->Name,  pNpc->m_IgnoreNegativeState.nValue[0], pMagic->nValue[0], pNpc->m_IgnoreNegativeState.nTime, pMagic->nValue[1]);
#endif
}

// --
//
// --
void KNpcAttribModify::AutoRescueSkill(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_AutoRescueSkill.nValue[0] = pMagic->nValue[0];
	pNpc->m_AutoRescueSkill.nValue[1] = pMagic->nValue[1];
	pNpc->m_AutoRescueSkill.nValue[2] = pMagic->nValue[2];
#ifdef _DEBUG
	//g_DebugLog("[autorescueskill] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_AutoRescueSkill.nValue[2], pMagic->nValue[2]);
#endif
}

// --
//
// --
void KNpcAttribModify::StaticMagicShieldP(KNpc* pNpc, void* pData)
{
	KMagicAttrib* pMagic = (KMagicAttrib *)pData;
	pNpc->m_StaticMagicShield += pMagic->nValue[0];
#ifdef _DEBUG
	//g_DebugLog("[staticmagicshield_p] Name: %s - Value: %d - Add: %d", pNpc->Name,  pNpc->m_StaticMagicShield, pMagic->nValue[2]);
#endif
}

