
#include	"KCore.h"
#include	"KNpcTemplate.h"
#include	"KNpc.h"
#include	"KNpcGold.h"

#define		defNPC_GOLD_SKILL_NO		7

#ifdef _SERVER
extern KLuaScript		*g_pNpcLevelScript;
#endif

// --
//	
// --
void KNpcGold::Init(int nIdx)
{
	this->m_nNpcIdx				= nIdx;
	//
	Release();
}

// --
//	
// --
void KNpcGold::Release()
{
	this->m_nGoldType			= 0;

	this->m_nFireResist			= 0;
	this->m_nFireResistMax		= 0;
	this->m_nColdResist			= 0;
	this->m_nColdResistMax		= 0;
	this->m_nLightingResist		= 0;
	this->m_nLightingResistMax	= 0;
	this->m_nPoisonResist		= 0;
	this->m_nPoisonResistMax	= 0;
	this->m_nPhycicsResist		= 0;
	this->m_nPhycicsResistMax	= 0;
	this->m_nAiMode				= 0;
	memset(this->m_nAiParam, 0, sizeof(this->m_nAiParam));
	this->m_nAiMaxTime			= 0;
}

// --
//	
// --
void	KNpcGold::SetGoldType(BOOL bFlag)
{
}

// --
//	
// --
void	KNpcGold::SetGoldTypeAndBackData()
{
	m_nGoldType		= 0;
	m_nFireResist			= Npc[m_nNpcIdx].m_CurrentFireResist;
	m_nFireResistMax		= Npc[m_nNpcIdx].m_CurrentFireResistMax;
	m_nColdResist			= Npc[m_nNpcIdx].m_CurrentColdResist;
	m_nColdResistMax		= Npc[m_nNpcIdx].m_CurrentColdResistMax;
	m_nLightingResist		= Npc[m_nNpcIdx].m_CurrentLightResist;
	m_nLightingResistMax	= Npc[m_nNpcIdx].m_CurrentLightResistMax;
	m_nPoisonResist			= 1;//Npc[m_nNpcIdx].m_CurrentPoisonResist;
	m_nPoisonResistMax		= 1;//Npc[m_nNpcIdx].m_CurrentPoisonResistMax;
	m_nPhycicsResist		= Npc[m_nNpcIdx].m_CurrentPhysicsResist;
	m_nPhycicsResistMax		= Npc[m_nNpcIdx].m_CurrentPhysicsResistMax;

	m_nAiMode				= Npc[m_nNpcIdx].m_AiMode;
	memcpy(m_nAiParam, Npc[m_nNpcIdx].m_AiParam, sizeof(m_nAiParam));
	m_nAiMaxTime			= Npc[m_nNpcIdx].m_AIMAXTime;

}


// --
//	
// --
void	KNpcGold::RecoverBackData()
{
	if (m_nGoldType <= 0 || m_nGoldType >= NpcSet.m_cGoldTemplate.m_nEffectTypeNum)
		return;
#ifdef _SERVER
	Npc[m_nNpcIdx].m_nReducedDamge				= 0;	
	Npc[m_nNpcIdx].m_CurrentFireResist			= m_nFireResist;
	Npc[m_nNpcIdx].m_CurrentFireResistMax		= m_nFireResistMax;
	Npc[m_nNpcIdx].m_CurrentColdResist			= m_nColdResist;
	Npc[m_nNpcIdx].m_CurrentColdResistMax		= m_nColdResistMax;
	Npc[m_nNpcIdx].m_CurrentLightResist			= m_nLightingResist;
	Npc[m_nNpcIdx].m_CurrentLightResistMax		= m_nLightingResistMax;
	Npc[m_nNpcIdx].m_CurrentPoisonResist		= 1;//m_nPoisonResist;
	Npc[m_nNpcIdx].m_CurrentPoisonResistMax		= 1;//m_nPoisonResistMax;
	Npc[m_nNpcIdx].m_CurrentPhysicsResist		= m_nPhycicsResist;
	Npc[m_nNpcIdx].m_CurrentPhysicsResistMax	= m_nPhycicsResistMax;
	
	Npc[m_nNpcIdx].m_AiMode						= m_nAiMode;
	// memcpy(Npc[m_nNpcIdx].m_AiParam, m_nAiParam, sizeof(Npc[m_nNpcIdx].m_AiParam));
	for(int i = 0; i < MAX_AI_PARAM - 1; i++)
	{
		if(Npc[m_nNpcIdx].m_AiParam[i] < m_nAiParam[i])
		{
			Npc[m_nNpcIdx].m_AiParam[i] = m_nAiParam[i];
		}
	}
	Npc[m_nNpcIdx].m_AIMAXTime					= m_nAiMaxTime;

	KNpcGoldTemplateInfo	*pInfo = &NpcSet.m_cGoldTemplate.m_sInfo[m_nGoldType - 1];
	
	Npc[m_nNpcIdx].m_Experience					/= (pInfo->m_nExp / 100);
	Npc[m_nNpcIdx].m_CurrentLifeMax				/= (pInfo->m_nLife / 100);
	Npc[m_nNpcIdx].m_CurrentLifeReplenish		/= pInfo->m_nLifeReplenish;
	Npc[m_nNpcIdx].m_CurrentAttackRating		/= (pInfo->m_nAttackRating / 100);
	Npc[m_nNpcIdx].m_CurrentDefend				/= (pInfo->m_nDefense / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0]	/= (pInfo->m_nMinDamage / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2]	/= (pInfo->m_nMaxDamage / 100);

	Npc[m_nNpcIdx].m_CurrentTreasure	-= pInfo->m_nTreasure;
	Npc[m_nNpcIdx].m_CurrentWalkSpeed	-= pInfo->m_nWalkSpeed;
	Npc[m_nNpcIdx].m_CurrentRunSpeed	-= pInfo->m_nRunSpeed;
	Npc[m_nNpcIdx].m_CurrentAttackSpeed	-= pInfo->m_nAttackSpeed;
	Npc[m_nNpcIdx].m_CurrentCastSpeed	-= pInfo->m_nCastSpeed;

	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0]				/= (pInfo->m_nPhysicalDamageBase / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2]				/= (pInfo->m_nPhysicalDamageBase / 100);
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[0]			/= (pInfo->m_nPhysicalMagicBase / 100);
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[2]			/= (pInfo->m_nPhysicalMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[0]			/= (pInfo->m_nPoisonDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[2]			/= (pInfo->m_nPoisonDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[0]		/= (pInfo->m_nPoisonMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[2]		/= (pInfo->m_nPoisonMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0]			/= (pInfo->m_nColdDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0]			/= (pInfo->m_nColdDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[0]		/= (pInfo->m_nColdMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[2]		/= (pInfo->m_nColdMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[0]			/= (pInfo->m_nFireDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[2]			/= (pInfo->m_nFireDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[0]		/= (pInfo->m_nFireMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[2]		/= (pInfo->m_nFireMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[0]			/= (pInfo->m_nLightingDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[2]			/= (pInfo->m_nLightingDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[0]		/= (pInfo->m_nLightingMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[2]		/= (pInfo->m_nLightingMagicBase / 100);

	if ( !Npc[m_nNpcIdx].IsPlayer() )
		Npc[m_nNpcIdx].m_SkillList.RemoveNpcSkill(defNPC_GOLD_SKILL_NO);

	if(Npc[m_nNpcIdx].m_pDropRate)
		Npc[m_nNpcIdx].m_pDropRate = NULL;
#endif

	m_nGoldType	= 0;

#ifdef _DEBUG
	//g_DebugLog("[Recover] Name: %s - Gold: %d - Life: %d", Npc[m_nNpcIdx].Name, m_nGoldType, Npc[m_nNpcIdx].m_CurrentLifeMax);
#endif

	

	Release();
}


// --
//	
// --
void	KNpcGold::UpdateNpcData(int nGoldType)
{
	KNpcGoldTemplateInfo	*pInfo = &NpcSet.m_cGoldTemplate.m_sInfo[nGoldType - 1];
	
	Npc[m_nNpcIdx].m_nReducedDamge				 = 30;
	Npc[m_nNpcIdx].m_Experience					*= (pInfo->m_nExp / 100);
	Npc[m_nNpcIdx].m_CurrentLifeMax				*= (pInfo->m_nLife / 100);
	Npc[m_nNpcIdx].m_CurrentLifeReplenish		*= pInfo->m_nLifeReplenish;
	Npc[m_nNpcIdx].m_CurrentAttackRating		*= (pInfo->m_nAttackRating / 100);
	Npc[m_nNpcIdx].m_CurrentDefend				*= (pInfo->m_nDefense / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0]	*= (pInfo->m_nMinDamage / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2]	*= (pInfo->m_nMaxDamage / 100);
	
	Npc[m_nNpcIdx].m_CurrentWalkSpeed	+= pInfo->m_nWalkSpeed;
	Npc[m_nNpcIdx].m_CurrentRunSpeed	+= pInfo->m_nRunSpeed;
	Npc[m_nNpcIdx].m_CurrentAttackSpeed	+= pInfo->m_nAttackSpeed;
	Npc[m_nNpcIdx].m_CurrentCastSpeed	+= pInfo->m_nCastSpeed;
	Npc[m_nNpcIdx].m_CurrentTreasure	+= pInfo->m_nTreasure;


	if(Npc[m_nNpcIdx].m_CurrentPoisonResist < pInfo->m_nPoisonResist)
	{
		Npc[m_nNpcIdx].m_CurrentPoisonResist		= 1;//pInfo->m_nPoisonResist;
	}
	if(Npc[m_nNpcIdx].m_CurrentPoisonResistMax < pInfo->m_nPoisonResistMax)
	{
		Npc[m_nNpcIdx].m_CurrentPoisonResistMax	= 1;//pInfo->m_nPoisonResistMax;
	}	

	if(Npc[m_nNpcIdx].m_CurrentLightResist < pInfo->m_nLightingResist)
	{
		Npc[m_nNpcIdx].m_CurrentLightResist		= pInfo->m_nLightingResist;
	}
	if(Npc[m_nNpcIdx].m_CurrentLightResistMax < pInfo->m_nLightingResistMax)
	{
		Npc[m_nNpcIdx].m_CurrentLightResistMax	= pInfo->m_nLightingResistMax;
	}	

	if(Npc[m_nNpcIdx].m_CurrentFireResist < pInfo->m_nFireResist)
	{
		Npc[m_nNpcIdx].m_CurrentFireResist		= pInfo->m_nFireResist;
	}
	if(Npc[m_nNpcIdx].m_CurrentFireResistMax < pInfo->m_nFireResistMax)
	{
		Npc[m_nNpcIdx].m_CurrentFireResistMax	= pInfo->m_nFireResistMax;
	}

	if(Npc[m_nNpcIdx].m_CurrentPhysicsResist < pInfo->m_nPhycicsResist)
	{
		Npc[m_nNpcIdx].m_CurrentPhysicsResist		= pInfo->m_nPhycicsResist;
	}
	if(Npc[m_nNpcIdx].m_CurrentPhysicsResistMax < pInfo->m_nPhycicsResistMax)
	{
		Npc[m_nNpcIdx].m_CurrentPhysicsResistMax	= pInfo->m_nPhycicsResistMax;
	}

	if(Npc[m_nNpcIdx].m_CurrentColdResist < pInfo->m_nColdResist)
	{
		Npc[m_nNpcIdx].m_CurrentColdResist		= pInfo->m_nColdResist;
	}
	if(Npc[m_nNpcIdx].m_CurrentColdResistMax < pInfo->m_nColdResistMax)
	{
		Npc[m_nNpcIdx].m_CurrentColdResistMax	= pInfo->m_nColdResistMax;
	}
	//AntiLag
	//Vat ly ngoai cong
	if (Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0]				= 1;
	if (Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2]				= 1;
	//Vat ly noi cong
	if (Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[0]			= 1;
	if (Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[2]			= 1;
	//Doc sat ngoai cong
	if (Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[0]			= 1;
	if (Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[2]			= 1;
	//Doc sat noi cong
	if (Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[0]		= 1;
	if (Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[2]		= 1;
	//Bang sat ngoai cong
	if (Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0]			= 1;
	if (Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[2]			= 1;
	//Bang sat noi cong
	if (Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[0]		= 1;
	if (Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[2]		= 1;
	//Hoa sat ngoai cong
	if (Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[0]			= 1;
	if (Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[2]			= 1;
	//Hoa sat noi cong
	if (Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[0]		= 1;
	if (Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[2]		= 1;
	//Loi sat ngoai cong
	if (Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[0]			= 1;
	if (Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[2]			= 1;
	//Hoa sat noi cong
	if (Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[0] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[0]		= 1;
	if (Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[2] <= 0)
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[2]		= 1;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[0]				*= (pInfo->m_nPhysicalDamageBase / 100);
	Npc[m_nNpcIdx].m_PhysicsDamage.nValue[2]				*= (pInfo->m_nPhysicalDamageBase / 100);
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[0]			*= (pInfo->m_nPhysicalMagicBase / 100);
	Npc[m_nNpcIdx].m_MagicPhysicsDamage.nValue[2]			*= (pInfo->m_nPhysicalMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[0]			*= (pInfo->m_nPoisonDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentPoisonDamage.nValue[2]			*= (pInfo->m_nPoisonDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[0]		*= (pInfo->m_nPoisonMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicPoisonDamage.nValue[2]		*= (pInfo->m_nPoisonMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0]			*= (pInfo->m_nColdDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentColdDamage.nValue[0]			*= (pInfo->m_nColdDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[0]		*= (pInfo->m_nColdMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicColdDamage.nValue[2]		*= (pInfo->m_nColdMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[0]			*= (pInfo->m_nFireDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentFireDamage.nValue[2]			*= (pInfo->m_nFireDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[0]		*= (pInfo->m_nFireMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicFireDamage.nValue[2]		*= (pInfo->m_nFireMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[0]			*= (pInfo->m_nLightingDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentLightDamage.nValue[2]			*= (pInfo->m_nLightingDamageBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[0]		*= (pInfo->m_nLightingMagicBase / 100);
	Npc[m_nNpcIdx].m_CurrentMagicLightDamage.nValue[2]		*= (pInfo->m_nLightingMagicBase / 100);

#ifdef _SERVER
	if(pInfo->m_szSkillName[0] && pInfo->m_nSkillLevel > 0)
	{
		Npc[m_nNpcIdx].m_SkillList.SetNpcSkill(defNPC_GOLD_SKILL_NO, KNpcTemplate::SkillString2Id((char*)pInfo->m_szSkillName), pInfo->m_nSkillLevel);
	}
#endif
	
	if(Npc[m_nNpcIdx].m_AiMode > pInfo->m_nAiMode)
	{
		Npc[m_nNpcIdx].m_AiMode		= pInfo->m_nAiMode;
	}
	for(int i = 0; i < MAX_AI_PARAM - 1; i++)
	{
		if(Npc[m_nNpcIdx].m_AiParam[i] < pInfo->m_nAiParam[i])
		{
			Npc[m_nNpcIdx].m_AiParam[i] = pInfo->m_nAiParam[i];
		}
	}
	if(Npc[m_nNpcIdx].m_AIMAXTime > pInfo->m_nAiMaxTime)
	{
		Npc[m_nNpcIdx].m_AIMAXTime	= pInfo->m_nAiMaxTime;
	}
	
	Npc[m_nNpcIdx].m_CurrentLife = Npc[m_nNpcIdx].m_CurrentLifeMax;

#ifdef _SERVER
	if(Npc[m_nNpcIdx].m_SkillList.m_Skills[defNPC_GOLD_SKILL_NO].SkillId)
	{
		KSkill	*pSkill;
		pSkill = (KSkill*)g_SkillManager.GetSkill(Npc[m_nNpcIdx].m_SkillList.m_Skills[defNPC_GOLD_SKILL_NO].SkillId, Npc[m_nNpcIdx].m_SkillList.m_Skills[defNPC_GOLD_SKILL_NO].SkillLevel);
		if (pSkill)
		{
			pSkill->Cast(m_nNpcIdx,0,0);
		}
	}

	if(SubWorld[Npc[m_nNpcIdx].m_SubWorldIndex].m_pBossDropRate)
	{	
		if(Npc[m_nNpcIdx].m_pDropRate != NULL)
		{
			Npc[m_nNpcIdx].m_pDropRate = SubWorld[Npc[m_nNpcIdx].m_SubWorldIndex].m_pBossDropRate;
		}
	}
#endif
}

#ifdef _SERVER
// --
//	
// --
void	KNpcGold::RandChangeGold(int nRand, int nGoldType)
{
	if(nRand <= 0)
		return;
	
	int i = g_Random(1000000);
	
	if(nRand != 1000000)
	{
		if (i > nRand)
			return;
	}

	if (NpcSet.m_cGoldTemplate.m_nEffectTypeNum <= 0)
		return;
	
	SetGoldCurrentType(nGoldType);
}
#endif

// --
// 
// --
int		KNpcGold::GetGoldType()
{
	return m_nGoldType;
}

// --
// 
// --
void	KNpcGold::SetGoldCurrentType(int nType)
{
	if (nType <= 0)
	{
		m_nGoldType = nType;
		return;
	}

	if(m_nGoldType > 0)
	{	
		return;
	}
	
	SetGoldTypeAndBackData();
	m_nGoldType		= nType;
	UpdateNpcData(nType);

#ifdef _SERVER
	NPC_GOLD_CHANGE_SYNC	sSync;
	sSync.ProtocolType = s2c_npcgoldchange;
	sSync.m_dwNpcID = Npc[this->m_nNpcIdx].m_dwID;
	sSync.m_wGoldFlag = m_nGoldType;
	Npc[m_nNpcIdx].SendDataToNearRegion((LPVOID)&sSync, sizeof(NPC_GOLD_CHANGE_SYNC));
#endif

#ifdef _DEBUG
	//g_DebugLog("[Gold] Name: %s - Gold: %d - Life: %d", Npc[m_nNpcIdx].Name, m_nGoldType, Npc[m_nNpcIdx].m_CurrentLifeMax);
#endif
}

// --
// 
// --
KNpcGoldTemplate::KNpcGoldTemplate()
{
	memset(this->m_sInfo, 0, sizeof(this->m_sInfo));
	this->m_nEffectTypeNum = 0;
}

// --
// 
// --
BOOL	KNpcGoldTemplate::Init()
{
	KTabFile	cFile;

	if (!cFile.Load(NPC_GOLD_TEMPLATE_FILE))
		return FALSE;

	char	szTemp[80];
	int i;
	for (i = 0; i < defMAX_NPC_GOLD_TYPE; i++)
	{
		cFile.GetString(i + 2, 1, "", szTemp, sizeof(szTemp));
		if (szTemp[0] == 0)
			break;

		cFile.GetInteger(i + 2,  2, 1, &m_sInfo[i].m_nExp);
		cFile.GetInteger(i + 2,  3, 1, &m_sInfo[i].m_nLife);
		cFile.GetInteger(i + 2,  4, 1, &m_sInfo[i].m_nLifeReplenish);
		cFile.GetInteger(i + 2,  5, 1, &m_sInfo[i].m_nAttackRating);
		cFile.GetInteger(i + 2,  6, 1, &m_sInfo[i].m_nDefense);
		cFile.GetInteger(i + 2,  7, 1, &m_sInfo[i].m_nMinDamage);
		cFile.GetInteger(i + 2,  8, 1, &m_sInfo[i].m_nMaxDamage);

		cFile.GetInteger(i + 2,  9, 0, &m_sInfo[i].m_nTreasure);
		cFile.GetInteger(i + 2, 10, 0, &m_sInfo[i].m_nWalkSpeed);
		cFile.GetInteger(i + 2, 11, 0, &m_sInfo[i].m_nRunSpeed);
		cFile.GetInteger(i + 2, 12, 0, &m_sInfo[i].m_nAttackSpeed);
		cFile.GetInteger(i + 2, 13, 0, &m_sInfo[i].m_nCastSpeed);

		cFile.GetString(i + 2, 14, "", m_sInfo[i].m_szSkillName, sizeof(m_sInfo[i].m_szSkillName));
		cFile.GetInteger(i + 2, 15, 0, &m_sInfo[i].m_nSkillLevel);

		cFile.GetInteger(i + 2, 16, 0, &m_sInfo[i].m_nFireResist);
		cFile.GetInteger(i + 2, 17, 0, &m_sInfo[i].m_nFireResistMax);
		cFile.GetInteger(i + 2, 18, 0, &m_sInfo[i].m_nColdResist);
		cFile.GetInteger(i + 2, 19, 0, &m_sInfo[i].m_nColdResistMax);
		cFile.GetInteger(i + 2, 20, 0, &m_sInfo[i].m_nLightingResist);
		cFile.GetInteger(i + 2, 21, 0, &m_sInfo[i].m_nLightingResistMax);
		cFile.GetInteger(i + 2, 22, 0, &m_sInfo[i].m_nPoisonResist);
		cFile.GetInteger(i + 2, 23, 0, &m_sInfo[i].m_nPoisonResistMax);
		cFile.GetInteger(i + 2, 24, 0, &m_sInfo[i].m_nPhycicsResist);
		cFile.GetInteger(i + 2, 25, 0, &m_sInfo[i].m_nPhycicsResistMax);

		cFile.GetInteger(i + 2, 26, 0, &m_sInfo[i].m_nAiMode);
		cFile.GetInteger(i + 2, 27, 0, &m_sInfo[i].m_nAiParam[0]);
		cFile.GetInteger(i + 2, 28, 0, &m_sInfo[i].m_nAiParam[1]);
		cFile.GetInteger(i + 2, 29, 0, &m_sInfo[i].m_nAiParam[2]);
		cFile.GetInteger(i + 2, 30, 0, &m_sInfo[i].m_nAiParam[3]);
		cFile.GetInteger(i + 2, 31, 0, &m_sInfo[i].m_nAiParam[4]);
		cFile.GetInteger(i + 2, 32, 0, &m_sInfo[i].m_nAiParam[5]);
		cFile.GetInteger(i + 2, 33, 0, &m_sInfo[i].m_nAiParam[6]);
		cFile.GetInteger(i + 2, 34, 0, &m_sInfo[i].m_nAiParam[7]);
		cFile.GetInteger(i + 2, 35, 0, &m_sInfo[i].m_nAiParam[8]);
		cFile.GetInteger(i + 2, 36, 0, &m_sInfo[i].m_nAiParam[9]);
		cFile.GetInteger(i + 2, 37, 0, (int*)&m_sInfo[i].m_nAiMaxTime);

		cFile.GetInteger(i + 2, 38, 0, &m_sInfo[i].m_nPhysicalDamageBase);
		cFile.GetInteger(i + 2, 39, 0, &m_sInfo[i].m_nPhysicalMagicBase);
		cFile.GetInteger(i + 2, 40, 0, &m_sInfo[i].m_nPoisonDamageBase);
		cFile.GetInteger(i + 2, 41, 0, &m_sInfo[i].m_nPoisonMagicBase);
		cFile.GetInteger(i + 2, 42, 0, &m_sInfo[i].m_nColdDamageBase);
		cFile.GetInteger(i + 2, 43, 0, &m_sInfo[i].m_nColdMagicBase);
		cFile.GetInteger(i + 2, 44, 0, &m_sInfo[i].m_nFireDamageBase);
		cFile.GetInteger(i + 2, 45, 0, &m_sInfo[i].m_nFireMagicBase);
		cFile.GetInteger(i + 2, 46, 0, &m_sInfo[i].m_nLightingDamageBase);
		cFile.GetInteger(i + 2, 47, 0, &m_sInfo[i].m_nLightingMagicBase);
	}
	this->m_nEffectTypeNum = i;

	return TRUE;
}
