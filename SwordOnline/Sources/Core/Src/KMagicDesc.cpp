#include "KCore.h"

#include "KEngine.h"
#include "KMagicAttrib.h"
#include "KMagicDesc.h"
#include "GameDataDef.h"
#include "KFaction.h"

#define		MAGICDESC_FILE		"\\settings\\MagicDesc.Ini"
extern const char * g_MagicID2String(int nAttrib);
const char MAGIC_ATTRIB_STRING[][100] = 
{
	"skill_begin",//0
	"skill_cost_v",//1	
	"skill_costtype_v",//2
	"skill_mintimepercast_v",//3
	"skill_misslenum_v",//4
	"skill_misslesform_v",//5		
	"skill_param1_v",//6		
	"skill_param2_v",//7
	"skill_startevent",//8
	"skill_flyevent",//9
	"skill_collideevent",//10
	"skill_vanishedevent",//11
	"skill_eventskilllevel",//12
	"skill_end",//13

	"missle_begin",//14
	"missle_movekind_v",//15
	"missle_speed_v",//16
	"missle_lifetime_v",//17
	"missle_height_v",//18
	"missle_damagerange_v",//19
	"missle_radius_v",//20
	"missle_missrate",//21
	"missle_hitcount",//22
	"missle_reserve3",//23
	"missle_reserve4",//24
	"missle_reserve5",//25
	"missle_end",//26
	
	"item_begin",//27
	"weapondamagemin_v",//28
	"weapondamagemax_v",//29
	"armordefense_v",//30
	"durability_v",//31
	"requirestr",//32
	"requiredex",//33
	"requirevit",//34
	"requireeng",//35
	"requirelevel",//36
	"requireseries",//37
	"requiresex",//38
	"requiremenpai",//39
	"weapondamageenhance_p",//40
	"armordefenseenhance_p",//41
	"requirementreduce_p",//42
	"indestructible_b",//43
	"item_reserve1",//44
	"item_reserve2",//45
	"item_reserve3",//46
	"require_fortune_value",//47
	"item_reserve5",//48
	"item_reserve6",//49
	"item_reserve7",//50
	"item_reserve8",//51
	"item_reserve9",//52
	"item_reserve10",//53
	"item_end",//54

	"damage_begin",//55		
	"attackrating_v",//56
	"attackrating_p",//57
	"ignoredefense_p",//58
	"physicsdamage_v",//59
	"colddamage_v",//60
	"firedamage_v",//61
	"lightingdamage_v",//62
	"poisondamage_v",//63
	"magicdamage_v",//64
	"physicsenhance_p",//65
	"steallife_p",//66
	"stealmana_p",//67
	"stealstamina_p",//68
	"knockback_p",//69
	"deadlystrike_p",//70
	"fatallystrike_p",//71
	"stun_p",//72
	"seriesdamage_p",//73
	"skill_skillexp_v",//74
	"damage_reserve3",//75
	"damage_reserve4",//76
	"damage_reserve5",//77
	"damage_reserve6",//78
	"damage_reserve7",//79
	"damage_reserve8",//80
	"damage_reserve9",//81
	"damage_reserve10",//82
	"damage_end",//83

	"normal_begin",//84
	"lifemax_v",//85
	"lifemax_p",//86
	"life_v",//87
	"lifereplenish_v",//88
	"manamax_v",//89
	"manamax_p",//90
	"mana_v",//91
	"manareplenish_v",//92
	"staminamax_v",//93
	"staminamax_p",//94
	"stamina_v",//95
	"staminareplenish_v",//96
	"strength_v",//97
	"dexterity_v",//98
	"vitality_v",//99
	"energy_v",//100
	"poisonres_p",//101
	"fireres_p",//102
	"lightingres_p",//103
	"physicsres_p",//104
	"coldres_p",//105
	"freezetimereduce_p",//106
	"burntimereduce_p",//107
	"poisontimereduce_p",//108
	"poisondamagereduce_v",//109
	"stuntimereduce_p",//110
	"fastwalkrun_p",//111
	"visionradius_p",//112
	"fasthitrecover_v",//113
	"allres_p",//114
	"attackspeed_v",//115
	"castspeed_v",//116
	"meleedamagereturn_v",//117
	"meleedamagereturn_p",//118
	"rangedamagereturn_v",//119
	"rangedamagereturn_p",//120
	"addphysicsdamage_v",//121
	"addfiredamage_v",//122
	"addcolddamage_v",//123
	"addlightingdamage_v",//124
	"addpoisondamage_v",//125
	"addphysicsdamage_p",//126
	"slowmissle_b",//127
	"changecamp_b",//128
	"physicsarmor_v",//129
	"coldarmor_v",//130
	"firearmor_v",//131
	"poisonarmor_v",//132
	"lightingarmor_v",//133
	"damagetomana_p",//134
	"lucky_v",//135
	"steallifeenhance_p",//136
	"stealmanaenhance_p",//137
	"stealstaminaenhance_p",//138
	"allskill_v",//139
	"metalskill_v",//140
	"woodskill_v",//141
	"waterskill_v",//142
	"fireskill_v",//143
	"earthskill_v",//144
	"knockbackenhance_p",//145
	"deadlystrikeenhance_p",//146
	"stunenhance_p",//147
	"badstatustimereduce_v",//148
	"manashield_p",//149
	"adddefense_v",//150
	"adddefense_p",//151
	"fatallystrikeenhance_p",//152
	"lifepotion_v",//153
	"manapotion_v",//154
	"physicsresmax_p",//155
	"coldresmax_p",//156
	"fireresmax_p",//157
	"lightingresmax_p",//158
	"poisonresmax_p",//159
	"allresmax_p",//160
	"coldenhance_p",//161
	"fireenhance_p",//162
	"lightingenhance_p",//163
	"poisonenhance_p",//164
	"magicenhance_p",//165
	"attackratingenhance_v",//166
	"attackratingenhance_p",//167
	"addphysicsmagic_v",//168
	"addcoldmagic_v",//169
	"addfiremagic_v",//170
	"addlightingmagic_v",//171
	"addpoisonmagic_v",//172
	"fatallystrikeres_p",//173
	"dynamicmagicshield_v",//174
	"skill_attackradius",//175
	"expenhance_p",//176
	"addskilldamage1",//177
	"addskilldamage2",//178
	"addskilldamage3",//179
	"addskilldamage4",//180
	"addskilldamage5",//181
	"addskilldamage6",//182
	"returnres_p",//183
	"poisondamagereturn_v",//184
	"poisondamagereturn_p",//185
	"returnskill_p",//186
	"autoreplyskill",//187
	"skill_mintimepercastonhorse_v",//188
	"ignoreskill_p",//189
	"lifereplenish_p",//190
	"autoattackskill",//191
	"poison2decmana_p",//192
	"skill_appendskill",//193
	"hide",//194
	"autodeathskill",//195
	"randmove",//196
	"ignorenegativestate_p",//197
	"autorescueskill",//198
	"staticmagicshield_p",//199
	"skill_desc",//200
	"skill_showevent",//201
	"unuse002",//202
	"unuse003",//203
	"unuse004",//204
	"unuse005",//205
	"unuse006",//206
	"unuse007",//207
	"unuse008",//208
	"unuse009",//209
	"unuse010",//210
	"unuse011",//211
	"unuse012",//212
	"unuse013",//213
	"unuse014",//214
	"unuse015",//215
	"unuse016",//216
	"unuse017",//217
	"sorbdamage_p",//218
	"anti_hitrecover",//219
	"anti_stuntimereduce_p",//220
	"anti_poisonres_p",//221
	"anti_fireres_p",//222
	"anti_lightingres_p",//223
	"anti_physicsres_p",//224
	"anti_coldres_p",//225
	"block_rate",//226
	"enhancehit_rate",//227
	"poisonres_yan_p",//228
	"lightingres_yan_p",//229
	"fireres_yan_p",//230
	"physicsres_yan_p",//231
	"coldres_yan_p",//232
	"lifemax_yan_v",//233
	"lifemax_yan_p",//234
	"manamax_yan_v",//235
	"manamax_yan_p",//236
	"sorbdamage_yan_p",//237
	"fastwalkrun_yan_p",//238
	"attackspeed_yan_v",//239
	"castspeed_yan_v",//240
	"allres_yan_p",//241
	"anti_maxres_p",//242
	"skill_enhance",//243
	"magicdamage_p",//244
	"fasthitrecover_yan_v",//245
	"five_elements_enhance_v",//246
	"five_elements_resist_v",//247
	"manareplenish_p",//248
	"add_damage_p",//249
	"pk_punish_weaken",//250
	"pk_punish_enhance",//251
	"anti_poisontimereduce_p",//252
	"do_hurt_p",//253
	"anti_do_hurt_p",//254
	"do_stun_p",//255
	"anti_do_stun_p",//256
	"anti_physicsres_yan_p",//257
	"anti_poisonres_yan_p",//258
	"anti_coldres_yan_p",//259
	"anti_fireres_yan_p",//260
	"anti_lightingres_yan_p",//261
	"anti_allres_yan_p",//262
	"unuse018",//263
	"unuse019",//264
	"unuse020",//265
	"unuse021",//266
	"unuse022",//267
	"unuse023",//268
	"anti_sorbdamage_yan_p",//269
	"anti_block_rate",//270
	"normal_end",
};

KMagicDesc	g_MagicDesc;

// --
//
// --
KMagicDesc::KMagicDesc()
{
	m_szDesc[0] = 0;
}

// --
//
// --
KMagicDesc::~KMagicDesc()
{
}

// --
//
// --
BOOL KMagicDesc::Init()
{
	return (m_IniFile.Load(MAGICDESC_FILE));
}

// --
//
// --
const char* KMagicDesc::GetDesc(void *pData)
{
	
	char	szTempDesc[512];
	char*	pTempDesc = szTempDesc;
	int		i = 0;

	ZeroMemory(m_szDesc, 512);
	
	if (!pData)
		return NULL;

	KMagicAttrib* pAttrib = (KMagicAttrib *)pData;

	const char	*pszKeyName = g_MagicID2String(pAttrib->nAttribType);
	m_IniFile.GetString("Descript", pszKeyName, "", szTempDesc, 512);
	
	while(*pTempDesc)
	{
		if (*pTempDesc == '#')
		{
			int	nDescAddType = 0;
			switch(*(pTempDesc + 3))
			{
				case '+':
					nDescAddType = 1;
					break;
				case '~':
					nDescAddType = 2;
					break;
				default:
					nDescAddType = 0;
					break;
			}
			int nValue = 0;
			
			switch(*(pTempDesc + 2))
			{
			case '1':
				nValue = pAttrib->nValue[0];
				break;
			case '2':
				nValue = pAttrib->nValue[1];
				break;
			case '3':
				nValue = pAttrib->nValue[2];
				break;
			case '4':
				nValue = pAttrib->nValue[0] / 256;
				break;
			case '6':
				nValue = pAttrib->nValue[2] / 256 / 18;
				break;
			case '7':
				nValue = pAttrib->nValue[0] % 256;
				break;
			case '9':
				nValue = pAttrib->nValue[2] % 256;
				break;
			default:
				nValue = pAttrib->nValue[0];
				break;
			}
			switch(*(pTempDesc+1))
			{
			case 'm':
				{
					char szMsg[32];
					strcpy(szMsg, g_Faction.m_sAttribute[nValue].m_szShowName);
					strcat(m_szDesc, szMsg);
					i += strlen(szMsg);/*
					strcat(m_szDesc,g_Faction.m_sAttribute[nValue].m_szName);
					i += strlen(g_Faction.m_sAttribute[nValue].m_szName);
					*/
				}
				break;
			case 's':
				{	
					char szMsg[32];
					switch(nValue)
					{
						case series_metal:
							sprintf(szMsg, "Kim");
							break;
						case series_wood:
							sprintf(szMsg, "Méc");
							break;
						case series_water:
							sprintf(szMsg, "Thñy");
							break;
						case series_fire:
							sprintf(szMsg, "Háa");
							break;
						case series_earth:
							sprintf(szMsg, "Thæ ");
							break;
						default:
							sprintf(szMsg, "V« hÖ ");
							break;
					}
					//strcat(m_szDesc, szMsg);
					//i += strlen(szMsg);
					i += 4;
				}
				break;
			case 'k':
				{
					char szMsg[32];
					switch(nValue)
					{
						case 0:
							sprintf(szMsg, "Néi Lùc");
							break;
						case 1:
							sprintf(szMsg, "Sinh Lùc");
							break;
						case 2:
							sprintf(szMsg, "ThÓ Lùc");
							break;
						case 3:
							sprintf(szMsg, "TiÒn");
							break;
						default:
							sprintf(szMsg, "Néi Lùc");
							break;
					}
					//strcat(m_szDesc, szMsg);
					//i += strlen(szMsg);
					i += 4;
				}
				break;
			case 'l':
				{	
					if(nValue <= 0)
					{	
						char	szMsg[80];
						sprintf(szMsg, "%s", "vâ c«ng vèn cã");
						strcat(m_szDesc, szMsg);
						i += strlen(szMsg);
					}
					else
					{
						KSkill * pSkill = NULL;
						pSkill = (KSkill *) g_SkillManager.GetSkill(nValue, 1);
						if (!pSkill) 
							continue;
						char	szMsg[80];
						sprintf(szMsg, "[ %s ]", pSkill->GetSkillName());
						strcat(m_szDesc, szMsg);
						i += strlen(szMsg);
					}
				}
				break;
			case 'd':
				{
					switch(nDescAddType)
					{
					case 1:
						if (nValue >= 0)
						{
							strcat(m_szDesc, "+");
							
						}
						else
						{
							nValue = -nValue;
							strcat(m_szDesc, "-");
						}
						i += 1;
						break;
				/*	case 2:
						if (nValue >= 0)
						{
							strcat(m_szDesc, "-");
							
						}
						else
						{
							nValue = -nValue;
							strcat(m_szDesc, "+");
						}
						i += 1;
						break;
						*/
					default:
						break;
					}
				//	if(nValue)
				//	{
						char	szMsg[16];
						sprintf(szMsg, "%d", nValue);
						strcat(m_szDesc, szMsg);
						i += strlen(szMsg);
				//	}
				}
				break;
			case 'x':
				if (nValue)
					strcat(m_szDesc, "N÷  ");
				else
					strcat(m_szDesc, "Nam");
				i += 4;
				break;
			default:
				break;
			}
			pTempDesc += 4;
		}
		else
		{
			m_szDesc[i] = *pTempDesc;
			pTempDesc++;
			i++;
		}
	}
	return m_szDesc;
}

// --
//
// --
const char * g_MagicID2String(int nAttrib)
{
	if ((nAttrib < 0) || nAttrib >= magic_normal_end) return MAGIC_ATTRIB_STRING[magic_normal_end];
	return 	MAGIC_ATTRIB_STRING[nAttrib];
}

// --
//
// --
int	g_String2MagicID(char * szMagicAttribName)
{
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return -1;
	
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, g_MagicID2String(i)))
			return i;
	}
	return -1;
}
// --
//AutoAI by kinnox;
// --
int	KMagicDesc::String2MagicID(char * szMagicAttribName)
{
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return -1;

	//nValue2 µ±ÖµÎª-1Ê±ÎªÓÀ¾ÃÐÔ×´Ì¬£¬0Îª·Ç×´Ì¬£¬ÆäËüÖµÎªÓÐÊ±Ð§ÐÔ×´Ì¬Ä§·¨Ð§¹û
	//ÐèÒª½«×´Ì¬Êý¾ÝÓë·Ç×´Ì¬Êý¾Ý·ÖÀë³öÀ´£¬·ÅÈëÏàÓ¦µÄÊý×éÄÚ£¬²¢¼ÇÂ¼×ÜÊýÁ¿
	
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, g_MagicID2String(i)))
			return i;
	}
	return -1;
}
//end code;

