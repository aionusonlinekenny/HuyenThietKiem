#include "KCore.h"
#include "GameDataDef.h"
#include "CoreShell.h"

#include "CoreDrawGameObj.h"
#include "ImgRef.h"

#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KItemList.h"
#include "KItemSet.h"
#include "KObjSet.h"
#include "KMagicDesc.h"//AutoAI by kinnox;
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"

#include "KNpcResList.h"
#include "Scene/KScenePlaceC.h"
#include "kskills.h"
#include "GameDataDef.h"
#include "MsgGenreDef.h"
#include "KOption.h"
#include "KSubWorld.h"
#include "KViewItem.h"
#include "KTongProtocol.h"
#include "malloc.h"

#include "KBuySell.h"
#include "KThiefSkill.h"
#include "KMath.h" // them g_Distance
#include <math.h> //
#include "../KJXPathFinder.h" // find way by kinnox;
#define	NPC_TRADE_BOX_WIDTH		6
#define	NPC_TRADE_BOX_HEIGHT	10
#define	MAX_TRADE_ITEM_WIDTH	2
#define	MAX_TRADE_ITEM_HEIGHT	4
IClientCallback* l_pDataChangedNotifyFunc = 0;

class KCoreShell : public iCoreShell
{
public:
	int	 GetProtocolSize(BYTE byProtocol);
	int	 Debug(unsigned int uDataId, unsigned int uParam, int nParam);
	int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam);
	void ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
	int	 FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind);
	int FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind);
	int FindSpecialNPC(char* Name, void* pReturn, int& nKind);
	int ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength);
	void ApplyAddTeam(void* pPlayer);
	void TradeApplyStart(void* pPlayer);
	int UseSkill(int x, int y, int nSkillID,BOOL bFlag = FALSE);
	int LockSomeoneUseSkill(int nTargetIndex, int nSkillID);
	int LockSomeoneAction(int nTargetIndex);
	int LockObjectAction(int nTargetIndex);
	void GotoWhere(int x, int y, int mode);
	void GotoWhereDirect(int x, int y, int mode);	//mode 0 is auto, 1 is walk, 2 is run // find way by kinnox;
	BOOL AutoMove();// find way by kinnox;
	void Goto(int nDir, int mode);
	void Turn(int nDir);
	int ThrowAwayItem();
	int GetNPCRelation(int nIndex);
	BOOL GetNPCShopState(int nIndex);
	int	SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam);
	int	TongOperation(unsigned int uOper, unsigned int uParam, int nParam);
	int TeamOperation(unsigned int uOper, unsigned int uParam, int nParam);
	int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam);
	void DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam);
	void DrawGameSpace();
	DWORD GetPing();
	int	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc);
	void NetMsgCallbackFunc(void* pMsgData);
	void SetRepresentShell(struct iRepresentShell* pRepresent);
	void SetMusicInterface(void* pMusicInterface);
	void SetRepresentAreaSize(int nWidth, int nHeight);
	int  Breathe();
	void Release();
	void SetClient(LPVOID pClient);
	void SendNewDataToServer(void* pData, int nLength);
	int	 SystemMessages(unsigned int uDataId, unsigned int uParam, int nParam);
	BOOL CanGetSkill(int nSkillId);
	// fix kiem tra chuot by kinnox;
	BOOL	CheckMouse();
	//end code
	int		PAIOperation(unsigned int uOper, unsigned int uParam, int nParam, int zParam); // AutoAi by kinnox;
	BOOL	GetAutoFlag();//AutoAi AutoAi by kinnox;
	BOOL	GetSpaceFlag();//check space AutoAi by kinnox;
	int		FindSkillInfo(int nType, int nIndex);//AutoAI by kinnox;
	void	GetSkillName(int nSkillId, char* szSkillName);//AutoAI by kinnox;
	void	SetSortItem(int nType, int nValue, int nPos);//AutoAI by kinnox;
	void	SetMoveMap(int nType, int nPos, int nValue);//AutoAI by kinnox;
	int		GetGenreItem(unsigned int uItemId, unsigned int uGenre); // fix xoa nen item questkey by kinnox;
	POINT	GetStatusLockPlayer();	//LockPlayer by kinnox;
	int		GetLixian();//Lixian by kinnox;
	BOOL	GetPaintFindPos();//PaintFindPos by kinnox;
	void	SetPaintFindPos(BOOL nIndex);//PaintFindPos by kinnox;
	int		SceneMapFindPosOperation(unsigned int uOper, unsigned int uParam, int nParam, bool bCalc, bool bLine);//PaintFindPos by kinnox;
	int 	GetGenreItem(unsigned int uId );
	int 	GetDetailItem(unsigned int uId );
	int 	GetParticularItem(unsigned int uId );
	int 	GetLevelItem(unsigned int uId );
	int 	GetSeriesItem(unsigned int uId );
	int 	GetNumStack(unsigned int uId );
};

static KCoreShell	g_CoreShell;
CORE_API void g_InitCore();
#ifndef _STANDALONE
extern "C" __declspec(dllexport)
#endif


iCoreShell* CoreGetShell()
{
	g_InitCore();
	return &g_CoreShell;
}


void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam)
{
	if (l_pDataChangedNotifyFunc)
		l_pDataChangedNotifyFunc->CoreDataChanged(uDataId, uParam, nParam);
}

void KCoreShell::Release()
{
	g_ReleaseCore();
}

void KCoreShell::NetMsgCallbackFunc(void* pMsgData)
{
	g_ProtocolProcess.ProcessNetMsg((BYTE *)pMsgData);
}

int	KCoreShell::SetCallDataChangedNofify(IClientCallback* pNotifyFunc)
{
	l_pDataChangedNotifyFunc = pNotifyFunc;
	return true;
}

int	KCoreShell::GetGameData(unsigned int uDataId, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uDataId)
	{
	case GDI_PLAYER_IS_MALE:
		{
			int nIndex = 0;
			if (nParam == 0)
				nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
			else
				nIndex = NpcSet.SearchID(nParam);

			if (nIndex)
				nRet = (Npc[nIndex].m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID);
			else
				nRet = 1;
		}
		break;
	case GDI_REPAIR_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						pItem = &Item[pObj->Obj.uId];
					}
				}
				break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
		
			if (pObj->eContainer == UOC_NPC_SHOP)
				break;
			pInfo->nPrice = pItem->GetRepairPrice();
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = pItem->CanBeRepaired();
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	case GDI_TRADE_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						pItem = &Item[pObj->Obj.uId];
					}
				}
				break;
			case CGOG_NPCSELLITEM:
				{
					int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
					if (nBuyIdx != -1)
					{
						int nIndex = BuySell.GetItemIndex(nBuyIdx, pObj->Obj.uId);
						if (nIndex >= 0)
							pItem = BuySell.GetItem(nIndex);
					}
				}
				break;
			case CGOG_PLAYERSELLITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
						pInfo->nPrice = Item[pObj->Obj.uId].GetPlayerShopPrice();
						strcpy(pInfo->szItemName,Item[pObj->Obj.uId].GetName());
						return 1;
					}
				}
				break;
			default:
				break;
			}

			if (!pItem)
				break;

			/*KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
		
			if (pObj->eContainer == UOC_NPC_SHOP)
			{	
				char szTongName[defTONG_NAME_MAX_LENGTH + 1];
				Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);
				pInfo->nPrice = pItem->GetPrice(TRUE, szTongName);
			}
			else
				pInfo->nPrice = pItem->GetPrice() / BUY_SELL_SCALE;
			strcpy(pInfo->szItemName, pItem->GetName());
			pInfo->btDisCountPrice	= pItem->GetDisCountPrice();
			pInfo->bHotSell			= pItem->GetHotSell();
			pInfo->btPriceType = pItem->GetPriceType();*/
			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
			if (pObj->eContainer == UOC_NPC_SHOP)
				pInfo->nPrice = pItem->GetPrice();
			else
				pInfo->nPrice = pItem->GetPrice() / BUY_SELL_SCALE;
			strcpy(pInfo->szItemName, pItem->GetName());
			pInfo->btDisCountPrice	= pItem->GetDisCountPrice();
			pInfo->bHotSell			= pItem->GetHotSell();
			pInfo->btPriceType = pItem->GetPriceType();
			nRet = 1;
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	case GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO:
	case GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE);
				}
				break;			
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript, true);
				}
				break;
			case CGOG_IME_ITEM:
				{
					if (pObj->Obj.uId <= 0)
						return 0;
					Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			case CGOG_PLAYERSELLITEM:
				{
					Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			}
		}
		break;
	case GDI_GAME_OBJ_DESC:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);

						if( Player[CLIENT_PLAYER_INDEX].m_ItemList.IsActiveSet() )
						{
							nActive = 3;
						}
						Item[pObj->Obj.uId].GetDesc(pszDescript, false, 1, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			case CGOG_SKILL:
			case CGOG_SKILL_FIGHT:
			case CGOG_SKILL_LIVE:
			case CGOG_SKILL_CUSTOM:
			case CGOG_SKILL_SHORTCUT:
				{
					int nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(pObj->Obj.uId);
					int nPointAdd = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetPointAdd(pObj->Obj.uId);
					nLevel += nPointAdd;
					_ASSERT(nLevel >= 0);
					if (pObj->Obj.uId >0)
					{
						
						ISkill * pISkill = g_SkillManager.GetSkill(pObj->Obj.uId, 1);
						if (!pISkill)
							break;
						eSkillStyle eStyle = (eSkillStyle) pISkill->GetSkillStyle();
						
						switch(eStyle)
						{
						case SKILL_SS_Missles:					
						case SKILL_SS_Melee:
						case SKILL_SS_InitiativeNpcState:			
						case SKILL_SS_PassivityNpcState:
							{
								KSkill::GetDesc(
									pObj->Obj.uId,
									nLevel,
									pszDescript,
									Player[CLIENT_PLAYER_INDEX].m_nIndex,
									(pObj->Obj.uGenre == CGOG_SKILL_CUSTOM || pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT)?false:true
									);
							}
							break;
							
						case SKILL_SS_Thief:
							{
								((KThiefSkill *)pISkill)->GetDesc(
									pObj->Obj.uId,
									nLevel,
									pszDescript,
									Player[CLIENT_PLAYER_INDEX].m_nIndex,
									(pObj->Obj.uGenre == CGOG_SKILL_CUSTOM || pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT)?false:true
								);
			
							}break;
							
						}
					}
					
				}
				break;
			
			case CGOG_PLAYER_FACE:
				break;
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript);
				}
				break;
			case CGOG_PLAYERSELLITEM:
				{
					Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			}
		}
		break;	
	case GDI_PLAYER_BASE_INFO:
		if (uParam)
		{
			KUiPlayerBaseInfo* pInfo = (KUiPlayerBaseInfo*)uParam;
			int nIndex = 0;
			if (nParam == 0)
			{
				nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
				pInfo->nCurFaction	= Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction;
				pInfo->nRankInWorld = Player[CLIENT_PLAYER_INDEX].m_nWorldStat;
				pInfo->btCurBattleTeam = (BYTE)Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_BATTLE_GROUP);
				pInfo->dwCurTong	= Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID();
				pInfo->dwTongMasterID = g_FileName2Id(Player[CLIENT_PLAYER_INDEX].m_cTong.m_szMasterName);
				
			}
			else
			{
				nIndex = NpcSet.SearchID(nParam);
				pInfo->nCurFaction	= -1;
				pInfo->nRankInWorld = Npc[nIndex].m_nWorldStat;
				pInfo->dwCurTong	= 0;
				pInfo->btCurBattleTeam = 0;
			}
			if (nIndex)
			{
				strcpy(pInfo->Name, Npc[nIndex].Name);
				pInfo->Title[0] = 0;
				if (Npc[nIndex].m_btRankId)
				{
					char szRankId[5];
					itoa(Npc[nIndex].m_btRankId, szRankId, 10);
					g_RankTabSetting.GetString(szRankId, "RANKSTR", "", pInfo->Title, sizeof(pInfo->Title));
				}	
			}
		}
		break;
	case GDI_PLAYER_RT_INFO:
		if (uParam)
		{
			KUiPlayerRuntimeInfo* pInfo = (KUiPlayerRuntimeInfo*)uParam;
			pInfo->nLifeFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax;		
			pInfo->nLife = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife;				
			pInfo->nManaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax;		
			pInfo->nMana = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana;				
			pInfo->nStaminaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax;
			pInfo->nStamina = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina;		

			pInfo->nExperienceFull = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;		
			pInfo->nExperience = Player[CLIENT_PLAYER_INDEX].m_nExp;					
			pInfo->nCurLevelExperience = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;
			
			pInfo->byActionDisable = 0;

			pInfo->byAction = PA_NONE;

			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
				pInfo->byAction |= PA_RUN;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
				pInfo->byAction |= PA_SIT;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
				pInfo->byAction |= PA_RIDE;
		}
		break;
	case GDI_PLAYER_RT_ATTRIBUTE:
		if (uParam)
		{	
			int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
			if(nParam)
			{
				nIndex = NpcSet.SearchID(nParam);
			}

			KUiPlayerAttribute* pInfo = (KUiPlayerAttribute*)uParam;
			KNpc*	pNpc			= &Npc[nIndex];
			pInfo->nMoney			= Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);				
			pInfo->nBARemainPoint	= Player[CLIENT_PLAYER_INDEX].m_nAttributePoint;						
			pInfo->nStrength		= Player[CLIENT_PLAYER_INDEX].m_nCurStrength;								
			pInfo->nDexterity		= Player[CLIENT_PLAYER_INDEX].m_nCurDexterity;								
			pInfo->nVitality		= Player[CLIENT_PLAYER_INDEX].m_nCurVitality;								
			pInfo->nEnergy			= Player[CLIENT_PLAYER_INDEX].m_nCurEngergy;									
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nRightKillMin , &pInfo->nRightKillMax, &pInfo->nAttack, 1);
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nKillMIN, &pInfo->nKillMAX, &pInfo->nAttack, 0);
			pInfo->nDefence			= pNpc->m_CurrentDefend;					
			pInfo->nMoveSpeed		= pNpc->m_CurrentRunSpeed;				
			pInfo->nAttackSpeed		= pNpc->m_CurrentAttackSpeed;			
			pInfo->nCastSpeed		= pNpc->m_CurrentCastSpeed;

			if (pNpc->m_CurrentPhysicsResistMax >= pNpc->m_CurrentPhysicsResist)
			{
				pInfo->nPhyDef		= pNpc->m_CurrentPhysicsResist;
			}
			else
			{
				pInfo->nPhyDef		= pNpc->m_CurrentPhysicsResistMax;
			}

			if (pNpc->m_CurrentColdResistMax >= pNpc->m_CurrentColdResist)
			{
				pInfo->nCoolDef		= pNpc->m_CurrentColdResist;
			}
			else
			{
				pInfo->nCoolDef		= pNpc->m_CurrentColdResistMax;
			}
			
			if (pNpc->m_CurrentLightResistMax >= pNpc->m_CurrentLightResist)
			{
				pInfo->nLightDef	= pNpc->m_CurrentLightResist;
			}
			else
			{
				pInfo->nLightDef	= pNpc->m_CurrentLightResistMax;
			}
			
			if (pNpc->m_CurrentFireResistMax >= pNpc->m_CurrentFireResist)
			{
				pInfo->nFireDef		= pNpc->m_CurrentFireResist;
			}
			else
			{
				pInfo->nFireDef		= pNpc->m_CurrentFireResistMax;
			}
			
			if (pNpc->m_CurrentPoisonResistMax >= pNpc->m_CurrentPoisonResist)
			{
				pInfo->nPoisonDef	= pNpc->m_CurrentPoisonResist;
			}
			else
			{
				pInfo->nPoisonDef	= pNpc->m_CurrentPoisonResistMax;
			}
			
			pInfo->nLevel = pNpc->m_Level;

			memset(pInfo->StatusDesc, 0, sizeof(pInfo->StatusDesc));
			switch(pNpc->m_Series)
			{
				case series_water:
					strcpy(pInfo->StatusDesc, "H? Th?y");
					break;
				case series_wood:
					strcpy(pInfo->StatusDesc, "H? M?c");
					break;
				case series_metal:
					strcpy(pInfo->StatusDesc, "H? Kim");
					break;
				case series_fire:
					strcpy(pInfo->StatusDesc, "H? H?a");
					break;
				case series_earth:
					strcpy(pInfo->StatusDesc, "H? Th? ");
					break;
			}

			pInfo->cPKValue		= pNpc->m_cPKValue;
			pInfo->nRepute		= pNpc->m_nRepute;
			pInfo->nFuyuan		= pNpc->m_nFuyuan;
			pInfo->cTransLife	= pNpc->m_cTransLife;
		}
		break;

	case GDI_PLAYER_IMMED_ITEMSKILL:
		if (uParam)
		{
			KUiPlayerImmedItemSkill* pInfo = (KUiPlayerImmedItemSkill*)uParam;
			memset(pInfo,0,sizeof(KUiPlayerImmedItemSkill));
			pInfo->IMmediaSkill[0].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[0].uId		= Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			pInfo->IMmediaSkill[1].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[1].uId		= Player[CLIENT_PLAYER_INDEX].GetRightSkill();

			for (int i = 0; i < MAX_IMMEDIACY_ITEM; i++)
			{
				pInfo->ImmediaItem[i].uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.FindItemInRoom(room_immediacy, i, 0);
				if (pInfo->ImmediaItem[i].uId > 0)
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_ITEM;
				}
				else
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_NOTHING;
				}
			}
		}
		break;

	case GDI_PLAYER_HOLD_MONEY:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);
		break;
	case GDI_ITEM_TAKEN_WITH:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
					nCount++;
			}
			nRet = nCount;
		}
		break;

	case GDI_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			if (nParam == 1)
				break;

			int PartConvert[itempart_num] =
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,
				UIEP_MANTLE,	UIEP_SIGNET,
				UIEP_SHIPIN,
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PARADE_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			if (nParam == 1)
				break;

			int PartConvert[itempart_num] =
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,
				UIEP_MANTLE,	UIEP_SIGNET,
				UIEP_SHIPIN,
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = g_cViewItem.m_sItem[i].nIdx;
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
	case GDI_BUILD_ITEM:
		nRet = 0;
		if (uParam)
		{
			// TODO?????�????????????
			if (nParam == 1)
				break;

			int PartBuildItem[MAX_PART_BUILD] = 
			{
				UIEP_BUILDITEM1,
				UIEP_BUILDITEM2,
				UIEP_BUILDITEM3,
				UIEP_BUILDITEM4,
				UIEP_BUILDITEM5,
				UIEP_BUILDITEM6,
				UIEP_BUILDITEM7,
				UIEP_BUILDITEM8,
				UIEP_BUILDITEM9,
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < MAX_PART_BUILD; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetBuildItem(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartBuildItem[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;	
	case GDI_TRADE_OPER_DATA:
		if (uParam == UTOD_IS_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeLock;
		else if (uParam == UTOD_IS_TRADING)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeState;
		else if (uParam == UTOD_IS_OTHER_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeDestLock;
		else if (uParam == UTOD_IS_WILLING)
		{
			nRet = (Player[CLIENT_PLAYER_INDEX].m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN);
		}
		break;
	case GDI_LIVE_SKILL_BASE:
		if (uParam)
		{
			KUiPlayerLiveSkillBase* pInfo = (KUiPlayerLiveSkillBase*)uParam;
			pInfo->nLiveExperience = 0 ;
			pInfo->nRemainPoint = 0 ;
			pInfo->nLiveExperienceFull = 0 ;
			memset(pInfo,0,sizeof(KUiPlayerLiveSkillBase));
		}
		break;

	case GDI_LIVE_SKILLS:
		if (uParam)
		{
			KUiSkillData* pInfo = (KUiSkillData*)uParam;
			memset(pInfo,0,sizeof(KUiSkillData)*10);
		}
		break;
	case GDI_FIGHT_SKILL_POINT:
		nRet = Player[CLIENT_PLAYER_INDEX].m_nSkillPoint;
		break;
	case GDI_FIGHT_SKILLS:
		if (uParam)
		{
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillSortList(pSkills);
		}
		break;
	case GDI_LEFT_ENABLE_SKILLS:
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLeftSkillSortList(pSkills);
			return nCount;

		}
		break;
	case GDI_RIGHT_ENABLE_SKILLS:
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetRightSkillSortList(pSkills);
			return nCount;
		}
		break;
	case GDI_NEARBY_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayer((KUiPlayerItem*)uParam, nParam);
		break;
	case GDI_NEARBY_IDLE_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayerForTeamInvite((KUiPlayerItem*)uParam, nParam);
		break;
	case GDI_PLAYER_LEADERSHIP:
		if (uParam)
		{
			KUiPlayerLeaderShip* pInfo = (KUiPlayerLeaderShip*)uParam ;
			pInfo->nLeaderShipExperience = Player[CLIENT_PLAYER_INDEX].m_dwLeadExp ;
			pInfo->nLeaderShipExperienceFull = Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp;
			pInfo->nLeaderShipLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel ;
		}
		break;
	case GDI_ITEM_IN_ENVIRO_PROP:
		{
			if (!nParam)
			{
				KUiGameObject *pObj = (KUiGameObject *)uParam;
				if (pObj->uGenre != CGOG_ITEM && pObj->uGenre != CGOG_NPCSELLITEM)
					break;

				KItem* pItem = NULL;
				if(pObj->uGenre == CGOG_ITEM && pObj->uId > 0 && pObj->uId < MAX_ITEM)
				{
					pItem = &Item[pObj->uId];
				}
				else if (pObj->uGenre == CGOG_NPCSELLITEM)
				{
					int nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->uId);
					pItem = BuySell.GetItem(nIdx);
				}

				if (!pItem)
					break;

				const int nGenre = pItem->GetGenre();
				if( (nGenre < item_equip) || (nGenre > item_mine) )
					break;

				UINT uPrice = pItem->GetPlayerShopPrice();
				switch(nGenre)
				{	
					case item_equip:
					case item_goldequip: // fix nen item hoang kim by kinnox;
						{
							BOOL bCanEquip = Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pItem);
							int nCurDur = pItem->GetDurability();

							switch(bCanEquip)
							{
								case 0:
									{
										if(uPrice <= 0)
										{
											nRet = IIEP_NOT_USEABLE;
										}
										else
										{
											nRet = IIEP_PRICED;
										}
									}
									break;
								case 1:
									{
										if(nCurDur > 0 && uPrice <= 0)
										{
											nRet = IIEP_NORMAL;
										}
										else if(nCurDur == 0 && uPrice <= 0)
										{
											nRet = IIEP_NOT_USEABLE;
										}
										else if(nCurDur < 0 && uPrice <= 0)
										{	
											nRet = IIEP_NORMAL;
										}
										else
										{	
											nRet = IIEP_PRICED;
										}
									}
									break;
								case 2: // fix nen item hoang kim by kinnox;
									{
										if(uPrice <= 0)
										{
											nRet = IIEP_NOT_USEABLE;
										}
										else
										{
											nRet = IIEP_PRICED;
										}
									}
									break;										
								default:
									break;
							}
						}
						break;
					case item_task:
					case item_script:
					case item_mine:
					case item_medicine:// fix nen item medicine khi dinh gia ban hang player sell by kinnox;					
						{
							if(uPrice > 0)
							{
								nRet = IIEP_PRICED;
							}
							else
							{
								nRet = IIEP_NORMAL;
							}
						}
						break;
					default:
						break;
				}
			}
			else
			{
				KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
				if (pObj->Obj.uGenre != CGOG_ITEM || pObj->Obj.uId >= MAX_ITEM)
					break;

				int PartConvert[itempart_num] = 
				{ 
					itempart_head,		itempart_weapon,
					itempart_amulet,	itempart_cuff,
					itempart_body,		itempart_belt,
					itempart_ring1,		itempart_ring2,
					itempart_pendant,	itempart_foot,
					itempart_horse,		itempart_mask,
					itempart_mantle,
				};

				_ASSERT(pObj->eContainer < itempart_num);
				if (pObj->eContainer >= itempart_num || pObj->eContainer < 0)
					break;

				if (Item[pObj->Obj.uId].GetGenre() != item_equip)
					break;

				int nPlace = PartConvert[pObj->eContainer];

				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pObj->Obj.uId, nPlace))
				{
					nRet = IIEP_NORMAL;
				}
				else
				{
					nRet = IIEP_NOT_USEABLE;
				}
			}
		}
		break;
	case GDI_IMMEDIATEITEM_NUM:
		if (uParam >= 0 && uParam < 9)
			nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetSameDetailItemNum(uParam);
		break;
	case GDI_TRADE_NPC_ITEM:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			int nPage = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion *)uParam;

			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			if (!BuySell.m_pShopRoom)
				break;
			BuySell.m_pShopRoom->Clear();
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
					pInfo->Obj.uId = i;

					POINT	Pos;
					if (BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos))
					{
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					else
					{
						nPage++;
						BuySell.m_pShopRoom->Clear();
						BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos);
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					pInfo->Region.h = Pos.x;
					pInfo->Region.v = Pos.y;
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					pInfo->nContainer = nPage;
					nCount++;
					pInfo++;
				}
			}			
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_CHAT_SEND_CHANNEL_LIST:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.SendSelectChannelList((KUiChatChannel*)uParam, nParam);
		break;
	case GDI_CHAT_CURRENT_SEND_CHANNEL:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.GetCurChannel((KUiChatChannel*)uParam);
		break;
	case GDI_ITEM_IN_STORE_BOX:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	//load 3 storebox by kinnox;
	case GDI_ITEM_IN_STORE_BOX1:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom1)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom1)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom1)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom1)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX2:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom2)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom2)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom2)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom2)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX3:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom3)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom3)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom3)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom3)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX4:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom4)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom4)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom4)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom4)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX5:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom5)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom5)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom5)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom5)
					nCount++;
			}
			nRet = nCount;
		}
		break;
		//end code
		//Expandbox by kinnox;
	case GDI_ITEM_IN_EXPAND_BOX1:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository); //default not change
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_expandtoryroom1)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_expandtoryroom1)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			nCount++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_expandtoryroom1)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_expandtoryroom1)
					nCount++;
			}
			nRet = nCount;
		}
		break;	
	case GDI_GIVEBOX_ITEM:		// GiveBox by kinnox
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_givebox)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_givebox)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_givebox)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_givebox)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PK_SETTING:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState();
		break;
	case GDI_SHOW_PLAYERS_NAME:
		nRet = NpcSet.CheckShowName();
		break;
	case GDI_SHOW_PLAYERS_LIFE:
		nRet = NpcSet.CheckShowLife();
		break;
	case GDI_SHOW_PLAYERS_MANA:
		nRet = NpcSet.CheckShowMana();
		break;
	case GDI_SHOW_OBJ_NAME:            
        nRet = ObjSet.CheckShowName();
        break;
	case GDI_EXBOX_NUM:
		nRet = Player[CLIENT_PLAYER_INDEX].m_btRepositoryCount;
		break;
	case GDI_EXPANDBOX_NUM://Expandbox by kinnox;
		nRet = Player[CLIENT_PLAYER_INDEX].m_btExpandtoryCount;
		break;	
	case GDI_GAME_OBJ_DESC_INCLUDE_MOUSEHOVER:
		{
			if (nParam && uParam)
			{
				KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
				char* pszFileName = (char*)nParam;
				switch(pObj->Obj.uGenre)
				{
				case 999999:
					break;
				default:
					break;
				}
				if(pszFileName[0])
					nRet = 1;
			}
		}
		break;
	case GDI_ITEM_NAME:
		{
			if(nParam)
			{
				KItem* pItem = NULL;

				pItem = &Item[nParam];
				if(!pItem)
				{	
					uParam = 0;
					break;
				}

				strcpy((char*)uParam, pItem->GetName());
				nRet = 1;
			}
			else
			{
				uParam = 0;
			}
		}
		break;

	case GDI_ITEM_GENRE:
		{
			if(uParam)
			{
				KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
				
				int nIdx = 0;
				KItem* pItem = NULL;

				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject->Obj.uId);
				if(pObject->Obj.uGenre == CGOG_NPCSELLITEM)
				{
					pItem = BuySell.GetItem(nIdx);
				}
				else
				{
					pItem = &Item[pObject->Obj.uId];
				}

				if(!pItem)
				{	
					nRet = -1;
					break;
				}
				
				if(nParam > 0)
				{
					if(pItem->GetGeneratorParam()->nLuck >= 1000000000)
					{
						nRet = 1;
					}
					else if(pItem->GetGeneratorParam()->nLuck == 7531)
					{
						nRet = 2;
					}
					else 
					{
						nRet = 0;
					}
				}
				else
				{
					nRet = pItem->GetGenre();
				}
			}
			else
			{
				nRet = -1;
			}
		}
		break;
	case GDI_ITEM_DETAIL:
		{
			if(nParam)
			{
				KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)nParam;
				
				int nIdx = 0;
				KItem* pItem = NULL;

				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject->Obj.uId);
				if(pObject->Obj.uGenre == CGOG_NPCSELLITEM)
				{
					pItem = BuySell.GetItem(nIdx);
				}
				else
				{
					pItem = &Item[pObject->Obj.uId];
				}
				
				if(!pItem)
					break;
				
				nRet = pItem->GetDetailType();
			}
			else
			{
				nRet = -1;
			}
		}
		break;
	case GDI_MASK_IMAGE:
		{
			if(uParam)
			{
				KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
				
				int nIdx = 0;
				KItem* pItem = NULL;

				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject->Obj.uId);
				if(pObject->Obj.uGenre == CGOG_NPCSELLITEM)
				{
					pItem = BuySell.GetItem(nIdx);
				}
				else
				{
					pItem = &Item[pObject->Obj.uId];
				}

				if(!pItem)
				{	
					nRet = 0;
					break;
				}
				WORD wNpcId = (WORD)pItem->GetBaseMagic();
				char szNpcTypeName[32], szResPath[80];
				ZeroMemory(szNpcTypeName, sizeof(szNpcTypeName));
				g_NpcSetting.GetString(wNpcId + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
				g_NpcKindFile.GetString(szNpcTypeName, "ResFilePath", "", szResPath, sizeof(szResPath));

				KTabFile	NormalNpc;
				if (!NormalNpc.Load(NPC_NORMAL_RES_FILE))
				{	
					nRet = 0;
					break;
				}
				char szSprName[32];
				int i = GetRandomNumber(1,5);
				switch(i)
				{
					case 1:
						NormalNpc.GetString(szNpcTypeName, "Wound", "", szSprName, sizeof(szSprName));
						break;
					case 2:
						NormalNpc.GetString(szNpcTypeName, "Die", "", szSprName, sizeof(szSprName));
						break;
					case 3:
						NormalNpc.GetString(szNpcTypeName, "Attack1", "", szSprName, sizeof(szSprName));
						break;
					case 4:
						NormalNpc.GetString(szNpcTypeName, "Attack2", "", szSprName, sizeof(szSprName));
						break;
					case 5:
						NormalNpc.GetString(szNpcTypeName, "Magic", "", szSprName, sizeof(szSprName));
						break;
					default:
						break;
				}
				if(szSprName[0])
				{
					sprintf((char*)nParam, "\\%s\\%s",szResPath, szSprName);
					nRet = 1;
				}
				else
				{
					nRet = 0;
				}
			}
			else
			{
				nRet = 0;
			}
		}
		break;
	case GDI_CHAT_ITEM_INFO:			
		{	
			if (uParam)
			{
				if(Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand() > 0)
				{
					nParam = 0;
					break;
				}

				KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
				KItem*	pItem = NULL;

				switch(pObj->Obj.uGenre)
				{
					case CGOG_ITEM:
						{
							if (pObj->Obj.uId > 0)
							{
								pItem = &Item[pObj->Obj.uId];
							}
						}
						break;
					default:
						break;
				}

				if (!pItem)
				{
					nParam = 0;
					break;
				}

				KUiChatItemParam* pInfo = (KUiChatItemParam*)nParam;
				
				if(pItem->GetGenre() == item_goldequip)
				{
					pInfo->m_ColorID = 3;
				}
				else if(pItem->GetGenre() == item_purpleequip)
				{
					pInfo->m_ColorID = 2;
				}
				else if(pItem->GetGenre() == item_brokenequip)
				{
					pInfo->m_ColorID = 4;
				}
				else if( (pItem->GetGenre() == item_equip) && (pItem->GetGeneratorParam()->nGeneratorLevel[0] > 0) )
				{
					pInfo->m_ColorID = 1;
				}
				else
				{
					pInfo->m_ColorID = 0;
				}
				
				
				pInfo->m_Genre	= pItem->GetGenre();
				pInfo->m_DetailType	= pItem->GetDetailType();
				pInfo->m_Record	= pItem->GetRecord();
				pInfo->m_Series	= pItem->GetSeries();
				pInfo->m_Level	= pItem->GetLevel();
				pInfo->m_Luck	= pItem->GetGeneratorParam()->nLuck;
				int i;
				for(i = 0; i < 6; i++)
				{
					pInfo->m_MagicLevel[i] = pItem->GetGeneratorParam()->nGeneratorLevel[i];
				}
				pInfo->m_RandomSeed	= pItem->GetGeneratorParam()->dwRandomSeed;
				pInfo->m_Version	= KPROTOCOL_VERSION;
				if( pItem->CanStack() )
					pInfo->m_StackCount	= pItem->GetDurability();
				pInfo->m_BindState	= pItem->GetBindState();
				pInfo->m_ExpiredTime	= pItem->GetTime();
				nRet = 1;
			}
			else
			{	
				nParam = 0;
			}	
		}
		break;
	case GDI_CHAT_ITEM_NAME:
		{	
			
			if(uParam)
			{
				KUiChatItemParam* pInfo = (KUiChatItemParam*)uParam;
				int pnMagicParam[6];
				
				for (int i = 0; i < 6; i++)
				{
					pnMagicParam[i] = pInfo->m_MagicLevel[i];
				}
				
				const int nItemIdx = ItemSet.AddExist(pInfo->m_Genre,
										pInfo->m_Series,
										pInfo->m_Level,
										pInfo->m_Record,
										pInfo->m_Luck,
										pInfo->m_DetailType,
										pnMagicParam,
										0,
										pInfo->m_RandomSeed);

				if(nItemIdx)
				{	
					char szTemp[64];
					strcpy(szTemp, "<");
					strcat(szTemp, Item[nItemIdx].GetName());
					strcat(szTemp, ">");
					int nLen = strlen(szTemp);
					szTemp[nLen + 1] = '\0';

					ItemSet.Remove(nItemIdx);
					strcpy((char*)nParam, szTemp);	
				}
			}
			
		}
		break;
	case GDI_CHAT_ITEM_DESC:
		{	
			if(uParam)
			{
				KUiChatItemParam* pInfo = (KUiChatItemParam*)uParam;
				int pnMagicParam[6];
				for (int i = 0; i < 6; i++)
				{
					pnMagicParam[i] = pInfo->m_MagicLevel[i];
				}

				const int nItemIdx = ItemSet.AddExist(pInfo->m_Genre,
										pInfo->m_Series,
										pInfo->m_Level,
										pInfo->m_Record,
										pInfo->m_Luck,
										pInfo->m_DetailType,
										pnMagicParam,
										0,
										pInfo->m_RandomSeed);
				if(nItemIdx)
				{
					if(pInfo->m_StackCount > 0)
						Item[nItemIdx].SetDurability(pInfo->m_StackCount);
					/*if(pInfo->m_BindState > 0)
						Item[nItemIdx].SetBindState(pInfo->m_BindState);*/
					if(pInfo->m_ExpiredTime > 0)
						Item[nItemIdx].SetTime(pInfo->m_ExpiredTime);

					char szItemDescShow[GOD_MAX_OBJ_TITLE_LEN], szImageName[MAX_PATH];
					int nWidth, nHeight;
					WORD wNpcId = 0;

					Item[nItemIdx].GetDesc(szItemDescShow);
					strcpy(szImageName, Item[nItemIdx].GetImageName());
					nWidth = Item[nItemIdx].GetWidth();
					nHeight = Item[nItemIdx].GetHeight();

					if(pInfo->m_Genre == item_equip)
					{
						if(pInfo->m_DetailType == itempart_mask)
						{
							wNpcId = (WORD)Item[nItemIdx].GetBaseMagic();
						}
					}
			
					ItemSet.Remove(nItemIdx);

					KUiChatItemDesc* pDesc = (KUiChatItemDesc*)nParam;
					strcpy(pDesc->szItemDesc, szItemDescShow);
					if(wNpcId > 0)
					{
						char szNpcTypeName[32], szResPath[80];
						ZeroMemory(szNpcTypeName, sizeof(szNpcTypeName));
						g_NpcSetting.GetString(wNpcId + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
						g_NpcKindFile.GetString(szNpcTypeName, "ResFilePath", "", szResPath, sizeof(szResPath));

						KTabFile	NormalNpc;
						if (!NormalNpc.Load(NPC_NORMAL_RES_FILE))
						{	
							nParam = 0;
							nRet = 0;
							break;
						}
						char szSprName[32];
						int i = GetRandomNumber(1,5);
						switch(i)
						{
							case 1:
								NormalNpc.GetString(szNpcTypeName, "Wound", "", szSprName, sizeof(szSprName));
								break;
							case 2:
								NormalNpc.GetString(szNpcTypeName, "Die", "", szSprName, sizeof(szSprName));
								break;
							case 3:
								NormalNpc.GetString(szNpcTypeName, "Attack1", "", szSprName, sizeof(szSprName));
								break;
							case 4:
								NormalNpc.GetString(szNpcTypeName, "Attack2", "", szSprName, sizeof(szSprName));
								break;
							case 5:
								NormalNpc.GetString(szNpcTypeName, "Magic", "", szSprName, sizeof(szSprName));
								break;
							default:
								break;
						}
						sprintf(szImageName, "\\%s\\%s", szResPath, szSprName);
						nWidth = 5;
						nHeight = 5;
					}
					strcpy(pDesc->szImageIcon, szImageName);
					pDesc->nWidth = nWidth;
					pDesc->nHeight = nHeight;
					nRet = 1;
				}
				else
				{
					nRet = 0;
				}
			}
		}
		break;
	case GDI_MARKET_COUNT:
		{
			nRet = BuySell.GetMarketHeight();
		}
		break;
	case GDI_MARKET_SELLID:
		{	
			if(uParam >= BuySell.GetMarketHeight())
				break;

			nRet = BuySell.GetMarketID(uParam);
		}
		break;
	case GDI_MARKET_SELLNAME:
		{	
			if(nParam >= BuySell.GetMarketHeight())
			{
				uParam = 0;
				break;
			}

			sprintf((char*)uParam, BuySell.GetMarketName(nParam));
		}
		break;
	case GDI_TRADE_MARKET_ITEM:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			int nPage = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion *)uParam;

			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			if (!BuySell.m_pShopRoom)
				break;
			BuySell.m_pShopRoom->Clear();
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
					pInfo->Obj.uId = i;

					if ((i != 0) && (i % defMAX_GOODS_PER_PAGE == 0))
					{
						nPage++;
					}

					pInfo->Region.h = 0;
					pInfo->Region.v = 0;
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					pInfo->nContainer = nPage;
					nCount++;
					pInfo++;
				}
			}			
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_BREAK_INFO:
		if (uParam)
		{	
			if(Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand())
			{
				nRet = 0;
				break;
			}

			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			if (pObj->eContainer == UOC_NPC_SHOP)
				break;

			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
				case CGOG_ITEM:
					{
						if (pObj->Obj.uId > 0)
						{
							pItem = &Item[pObj->Obj.uId];
						}
					}
					break;
				default:
					break;
			}

			if (!pItem)
				break;
			
			if(pItem->GetGenre() != item_task && pItem->GetGenre() != item_script)
				break;

			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
	
			if (!pItem->GetMaxStackCount())
				break;

			if (pItem->GetStackCount() == 1)
				break;

			pInfo->nPrice = pItem->GetStackCount();
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = pItem->GetMaxStackCount();
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	case GDI_PLAYER_FIGHT_MODE:
		nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode;
		break;
	case GDI_PLAYER_HOLD_JBCOIN:
		{
			nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.CountSameItem(room_equipment, item_task, ItemSet.m_nCoinID, 1, -1, 1);
		}
		break;
	case GDI_PLAYER_HOLD_LOCKCOIN:
		{
			nRet = Player[CLIENT_PLAYER_INDEX].GetLockBronzeCoin();
		}
		break;
	case GDI_PLAYER_OPEN_SHOP:
		{	
			if(uParam)
			{
				nRet = Npc[uParam].m_bOpenShop;
			}
			else
			{
				nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop;
			}
		}
		break;
	case GDI_PLAYER_SHOP_ADV:
		{
			if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szShopAdv[0])
				nRet = 1;
		}
		break;
	case GDI_PLAYER_RIDE_HORSE:
		{
			nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse;
		}
		break;
	case GDI_MAP_TYPE:
		{
			if(!strcmp(SubWorld[0].m_szMapType, "City") ||
				!strcmp(SubWorld[0].m_szMapType, "Capital"))
				nRet = 1;
		}
		break;
	case GDI_PLAYER_SHOP_ITEM:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			int nIndex = 0;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion *)uParam;

			for (int i = 0; i < Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemCountMax(); i++)
			{
				nIndex = Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemIndex(i);
				KItem* pItem = &Item[nIndex];
				
				if (nIndex >= 0 && pItem)
				{
					pInfo->Obj.uGenre = CGOG_PLAYERSELLITEM;
					pInfo->Obj.uId = nIndex;
					pInfo->Region.h = Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemPosX(i);
					pInfo->Region.v = Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemPosY(i);
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					nCount++;
					pInfo++;
				}
			}			
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			int nIndex = 0;
			for (int i = 0; i < Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemCountMax(); i++)
			{
				nIndex = Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemIndex(i);
				KItem* pItem = &Item[nIndex];
				
				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_PLAYER_SHOP_ITEM_COUNT:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cShop.GetItemCount();
		break;
	case GDI_NPC_STATE_SKILL://UiMiniBuff by kinnox;
		nRet = 0;
		if (uParam)
		{
			KStateNode* pNode = NULL;
			pNode = (KStateNode *)Npc[CLIENT_PLAYER_INDEX].m_StateSkillList.GetHead();
			KStateControl* pInfo = (KStateControl *)uParam;
			
			while(pNode)
			{
				if (pNode->m_SkillID > 0 && pNode->m_SkillID < MAX_SKILL && pNode->m_Level > 0 && pNode->m_Level < MAX_SKILLLEVEL)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(pNode->m_SkillID, pNode->m_Level);
					if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState)
					{
						pInfo->nSkillId = pNode->m_SkillID;
						pInfo->nLeftTime = pNode->m_LeftTime;
						pInfo++;
					}
				}
				pNode = (KStateNode *)pNode->GetNext();
			}	
		}
		else
		{
			int nCount = 0;
			KStateNode* pNode = NULL;
			pNode = (KStateNode *)Npc[CLIENT_PLAYER_INDEX].m_StateSkillList.GetHead();
			while(pNode)
			{
				if (pNode->m_SkillID > 0 && pNode->m_SkillID < MAX_SKILL && pNode->m_Level > 0 && pNode->m_Level < MAX_SKILLLEVEL)
				{
					KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(pNode->m_SkillID, pNode->m_Level);
					if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles || 
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState)
					{
						nCount++;
					}
				}
				pNode = (KStateNode *)pNode->GetNext();
			}
			nRet = nCount;
		}
		break;	
	case GDI_AVATAR_IMAGE://Player Avatar by kinnox;
		nRet = Player[CLIENT_PLAYER_INDEX].m_ImagePlayer;
		break;	
	}
	return nRet;
}


int	KCoreShell::OperationRequest(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 1;
	switch(uOper)
	{
	case GOI_QUERY_RANK_INFORMATION:
		SendClientCmdQueryLadder(uParam);
		break;
	//uParam = (const char*)pszFileName
	case GOI_PLAY_SOUND:
		if (uParam)
		{
			static KCacheNode* pSndNode = NULL;
			KWavSound* pSound = NULL;
			pSndNode	= (KCacheNode*)g_SoundCache.GetNode((char *)uParam, (KCacheNode * )pSndNode);
			pSound		= (KWavSound*)pSndNode->m_lpData;
			if (pSound)
			{
				if (pSound->IsPlaying())
					break;
				pSound->Play(0, -10000 + Option.GetSndVolume() * 100, 0);
			}
		}
		break;
	case GOI_PLAYER_RENASCENCE:
		{
			int nReviveType;
			if (nParam)	// bBackTown
			{
				nReviveType = REMOTE_REVIVE_TYPE;
			}
			else
			{
				nReviveType = LOCAL_REVIVE_TYPE;
			}
//			SendClientCmdRevive(nReviveType);
			SendClientCmdRevive();
		}
		break;
	case GOI_MONEY_INOUT_STORE_BOX:
		{
			BOOL	bIn = (BOOL)uParam;
			int		nMoney = nParam;
			int		nSrcRoom, nDesRoom;


			if (bIn)
			{
				nSrcRoom = room_equipment;
				nDesRoom = room_repository;
			}
			else
			{
				nDesRoom = room_equipment;
				nSrcRoom = room_repository;
			}
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(nSrcRoom, nDesRoom, nMoney);
		}
		break;
	case GOI_EXIT_GAME:
		g_SubWorldSet.Close();
		g_ScenePlace.ClosePlace();
		break;
	case GOI_GAMESPACE_DISCONNECTED:
		g_SubWorldSet.Close();
		break;
	case GOI_TRADE_NPC_BUY:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
					break;

			int nIdx = 0;
			KItem* pItem = NULL;

			nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
			pItem = BuySell.GetItem(nIdx);

			int nWidth, nHeight;
			ItemPos	Pos;

			nWidth = pItem->GetWidth();
			nHeight = pItem->GetHeight();
			if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
			{
				nRet = 0;
				break;
			}
			if (Pos.nPlace != pos_equiproom)
			{
				nRet = 0;

				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
			
			BYTE btType = pItem->GetPriceType();
			switch(btType)
			{
			case 1:
				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CountSameItem(room_equipment, item_task, ItemSet.m_nCoinID, 1, -1, 1) < pItem->GetPrice())
				{
					nRet = 0;
					KSystemMessage	sMsg;
					
					strcpy(sMsg.szMessage, MSG_SHOP_NO_JBCOIN);
					sMsg.eType = SMT_SYSTEM;
					sMsg.byConfirmType = SMCT_CLICK;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return nRet;
				}
				break;
			case 2:
				if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_FUYUAN) < pItem->GetPrice())
				{
					nRet = 0;
					KSystemMessage	sMsg;
					strcpy(sMsg.szMessage, MSG_SHOP_NO_FUYUAN);
					sMsg.eType = SMT_SYSTEM;
					sMsg.byConfirmType = SMCT_CLICK;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return nRet;
				}
				break;
			case 3:
				if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_SJPOINT) < pItem->GetPrice())
				{
					nRet = 0;
					KSystemMessage	sMsg;
					
					strcpy(sMsg.szMessage, MSG_SHOP_NO_SJPOINT);
					sMsg.eType = SMT_SYSTEM;
					sMsg.byConfirmType = SMCT_CLICK;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return nRet;
				}
				break;
			case 4:
				if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_FPOINT) < pItem->GetPrice())
				{
					nRet = 0;
					KSystemMessage	sMsg;
					
					strcpy(sMsg.szMessage, MSG_SHOP_NO_FPOINT);
					sMsg.eType = SMT_SYSTEM;
					sMsg.byConfirmType = SMCT_CLICK;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return nRet;
				}
				break;
			case 5:
				if(Player[CLIENT_PLAYER_INDEX].GetLockBronzeCoin() < pItem->GetPrice())
				{
					nRet = 0;
					KSystemMessage	sMsg;
					
					strcpy(sMsg.szMessage, MSG_SHOP_NO_JBCOIN2);
					sMsg.eType = SMT_SYSTEM;
					sMsg.byConfirmType = SMCT_CLICK;
					sMsg.byPriority = 1;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return nRet;
				}
				break;
			default:
				{	
					/*char szTongName[defTONG_NAME_MAX_LENGTH + 1];
					Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice(TRUE, szTongName))*/
					
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						return nRet;
					}
				}
				break;
			}
			SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
		}
		break;
	case GOI_TRADE_NPC_SELL:
		{	
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;

			int nIdx = pObject1->Obj.uId;
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{	
				if(Item[nIdx].GetBindState() > 0)
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_NOT_SELL_ITEM);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				if(Item[nIdx].GetGenre() == 4)
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_NOT_SELL_ITEM);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				if (Item[nIdx].GetGenre() == item_task || Item[nIdx].GetGenre() == item_script)
				{	
					if(Item[nIdx].GetParticular() == 0)
					{
						KSystemMessage	sMsg;
						sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
						sMsg.eType = SMT_NORMAL;
						sMsg.byConfirmType = SMCT_NONE;
						sMsg.byPriority = 0;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						return 0;
					}
				}
				SendClientCmdSell(Item[nIdx].GetID());
				return 1;
				
			}
			else
			{
				return 0;
			}
		}
		break;
	case GOI_TRADE_NPC_REPAIR:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;
			int nIdx = pObject1->Obj.uId;
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{		
				if (Item[nIdx].GetGenre() >= item_brokenequip)
				{
					return 0;
				}
				else if (Item[nIdx].GetDurability() == -1 || Item[nIdx].GetDurability() == Item[nIdx].GetMaxDurability())
				{
					return 0;
				}
				else if (Item[nIdx].GetRepairPrice() <= Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney())
				{
					SendClientCmdRepair(Item[nIdx].GetID());
				}
				else
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_SHOP_NO_MONEY);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;
	case GOI_SWITCH_OBJECT:
		{
			ItemPos	P1, P2;
			int PartConvert[itempart_num] =
			{
				itempart_head,		itempart_weapon,
				itempart_amulet,	itempart_cuff,
				itempart_body,		itempart_belt,
				itempart_ring1,		itempart_ring2,
				itempart_pendant,	itempart_foot,
				itempart_horse,		itempart_mask,
				itempart_mantle,	itempart_signet,
				itempart_shipin,
			};

			int PartBuildItem[MAX_PART_BUILD] = //TrembleItem by kinnox;
			{ 
				itempart_build1,	itempart_build2,
				itempart_build3,	itempart_build4,
				itempart_build5,	itempart_build6,
				itempart_build7,	itempart_build8,
				itempart_build9,
			};


			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			KUiObjAtContRegion* pObject2 = (KUiObjAtContRegion*)nParam;
			
			if (!pObject1 && !pObject2)
				break;
			
			if (pObject1)
			{
				switch(pObject1->eContainer)
				{
				case UOC_STORE_BOX:
					P1.nPlace = pos_repositoryroom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				//load 3 storebox by kinnox;
				case UOC_STORE_BOX1:
					P1.nPlace = pos_repositoryroom1;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_STORE_BOX2:
					P1.nPlace = pos_repositoryroom2;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_STORE_BOX3:
					P1.nPlace = pos_repositoryroom3;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_STORE_BOX4:
					P1.nPlace = pos_repositoryroom4;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;				
				case UOC_STORE_BOX5:
					P1.nPlace = pos_repositoryroom5;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EXPAND_BOX1://Expandbox by kinnox;
					P1.nPlace = pos_expandtoryroom1;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;	
				case UOC_GIVE_BOX: //GiveBox by kinnox;
					P1.nPlace = pos_givebox;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;					
				case UOC_IMMEDIA_ITEM:
					P1.nPlace = pos_immediacy;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_BUILD_ITEM:
					{
						if (pObject1->Region.h == 1)
							break;
						P1.nPlace = pos_builditem;
						P1.nX = PartBuildItem[pObject1->Region.v];
					}
					break;
				case UOC_ITEM_TAKE_WITH:
					P1.nPlace = pos_equiproom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					//g_DebugLog("RUN 1 %d - %d",P1.nX,P1.nY);
					break;
				case UOC_EQUIPTMENT:
					{
						if (pObject1->Region.h == 1)
							break;
						P1.nPlace = pos_equip;
						P1.nX = PartConvert[pObject1->Region.v];
						P1.nY = 0;
					}
					break;
				case UOC_TO_BE_TRADE:
					P1.nPlace = pos_traderoom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_NPC_SHOP:
					{
						if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
							break;

						int nIdx = 0;
						KItem* pItem = NULL;
						
						nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
						pItem = BuySell.GetItem(nIdx);
						
						int nWidth, nHeight;
						ItemPos	Pos;
						
						nWidth = pItem->GetWidth();
						nHeight = pItem->GetHeight();
						if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
						{
							nRet = 0;
							KSystemMessage	sMsg;
							
							strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
							sMsg.eType = SMT_SYSTEM;
							sMsg.byConfirmType = SMCT_CLICK;
							sMsg.byPriority = 1;
							sMsg.byParamSize = 0;
							CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
							break;
						}
						if (Pos.nPlace != pos_equiproom)
						{
							nRet = 0;
							KSystemMessage	sMsg;
							
							strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
							sMsg.eType = SMT_SYSTEM;
							sMsg.byConfirmType = SMCT_CLICK;
							sMsg.byPriority = 1;
							sMsg.byParamSize = 0;
							CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
							break;
						}
						BYTE btType = pItem->GetPriceType();
						switch(btType)
						{
						case 1:
							if(Player[CLIENT_PLAYER_INDEX].GetBronzeCoin() < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_JBCOIN);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						case 2:
							if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_FUYUAN) < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_FUYUAN);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						case 3:
							if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_SJPOINT) < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_SJPOINT);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						case 4:
							if(Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(TV_FPOINT) < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_FPOINT);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						case 5:
							if(Player[CLIENT_PLAYER_INDEX].GetLockBronzeCoin() < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_JBCOIN2);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						default:
							if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
							{
								nRet = 0;
								KSystemMessage	sMsg;
								
								strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
								sMsg.eType = SMT_SYSTEM;
								sMsg.byConfirmType = SMCT_CLICK;
								sMsg.byPriority = 1;
								sMsg.byParamSize = 0;
								CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
								return nRet;
							}
							break;
						}
						SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
					}
					break;
		
				}
			}
			
			if (pObject2)
			{
				switch(pObject2->eContainer)
				{
				case UOC_STORE_BOX:
					P2.nPlace = pos_repositoryroom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				//load 3 storebox by kinnox;
				case UOC_STORE_BOX1:
					P2.nPlace = pos_repositoryroom1;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_STORE_BOX2:
					P2.nPlace = pos_repositoryroom2;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_STORE_BOX3:
					P2.nPlace = pos_repositoryroom3;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_STORE_BOX4:
					P2.nPlace = pos_repositoryroom4;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_STORE_BOX5:
					P2.nPlace = pos_repositoryroom5;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EXPAND_BOX1://Expandbox by kinnox;
					P2.nPlace = pos_expandtoryroom1;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;					
				//GiveBox by kinnox;
				case UOC_GIVE_BOX: 
					P2.nPlace = pos_givebox;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;							
				case UOC_IMMEDIA_ITEM:
					P2.nPlace = pos_immediacy;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_BUILD_ITEM://TrembleItem by kinnox;
					{
						// TODO:???�???????
						if (pObject2->Region.h == 1)
							break;
						P2.nPlace = pos_builditem;
						P2.nX = PartBuildItem[pObject2->Region.v];
					}
					break;
				case UOC_ITEM_TAKE_WITH:
					P2.nPlace = pos_equiproom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EQUIPTMENT:
					{
						if (pObject2->Region.h == 1)
							break;
						P2.nPlace = pos_equip;
						P2.nX = PartConvert[pObject2->Region.v];
						P2.nY = 0;
						//g_DebugLog("RUN 2 %d - %d",P2.nX,P2.nY);
					}
					break;
				case UOC_TO_BE_TRADE:
					P2.nPlace = pos_traderoom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_NPC_SHOP:
					break;
				}
			}
			/*if (pObject2 && pObject2->Region.h == -1 && pObject2->Region.v == -1)
			{
				int nPlace = -1;
				if (pObject1->eContainer == UOC_ITEM_TAKE_WITH)
					nPlace = pos_equiproom;
				else if (pObject1->eContainer == UOC_EXPAND_BOX1)
					nPlace = pos_expandtoryroom1;
				else if (pObject1->eContainer >= UOC_STORE_BOX && pObject1->eContainer <= UOC_STORE_BOX5)
					nPlace = pos_repositoryroom + (pObject1->eContainer - UOC_STORE_BOX);
				else
					break;

				int nDestPlace = -1;
				if (pObject2->eContainer == UOC_ITEM_TAKE_WITH)
					nDestPlace = pos_equiproom;
				else if (pObject2->eContainer == UOC_EXPAND_BOX1)
					nDestPlace = pos_expandtoryroom1;
				else if (pObject2->eContainer >= UOC_STORE_BOX && pObject2->eContainer <= UOC_STORE_BOX5)
					nDestPlace = pos_repositoryroom + (pObject2->eContainer - UOC_STORE_BOX);
				else
					nDestPlace = pos_repositoryroom; // fallback

				ITEM_RIGHT_AUTO_MOVE_REQ req;
				req.ProtocolType = c2s_rightitemautomove;
				req.SrcPlace = nPlace;
				req.SrcX = pObject1->Region.h;
				req.SrcY = pObject1->Region.v;
				req.DestPlace = nDestPlace;
				req.DestX = -1;
				req.DestY = -1;

				//g_DebugLog("[CLIENT] GOI_SWITCH_OBJECT: Right Click Move Src=(%d,%d,%d) DestPlace=%d", req.SrcPlace, req.SrcX, req.SrcY, req.DestPlace);

				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)&req, sizeof(req));

				return TRUE;
			}*/
		if (pObject2 && pObject2->Region.h == -1 && pObject2->Region.v == -1)
			{
				int nPlace = -1;
				if (pObject1->eContainer == UOC_ITEM_TAKE_WITH)
					nPlace = pos_equiproom;
				else if (pObject1->eContainer == UOC_EXPAND_BOX1)
					nPlace = pos_expandtoryroom1;
				else if (pObject1->eContainer >= UOC_STORE_BOX && pObject1->eContainer <= UOC_STORE_BOX5)
					nPlace = pos_repositoryroom + (pObject1->eContainer - UOC_STORE_BOX);
				else
					return TRUE;

				int nDestPlace = -1;
				if (pObject2->eContainer == UOC_ITEM_TAKE_WITH)
					nDestPlace = pos_equiproom;
				else if (pObject2->eContainer == UOC_EXPAND_BOX1)
					nDestPlace = pos_expandtoryroom1;
				else if (pObject2->eContainer >= UOC_STORE_BOX && pObject2->eContainer <= UOC_STORE_BOX5)
					nDestPlace = pos_repositoryroom + (pObject2->eContainer - UOC_STORE_BOX);
				else
					nDestPlace = pos_repositoryroom; // fallback

				ITEM_RIGHT_AUTO_MOVE_REQ req;
				req.ProtocolType = c2s_rightitemautomove;
				req.SrcPlace = (BYTE)nPlace;
				req.SrcX = (BYTE)pObject1->Region.h;
				req.SrcY = (BYTE)pObject1->Region.v;
				req.DestPlace = (BYTE)nDestPlace;
				req.DestX = (BYTE)0xFF;  // -1
				req.DestY = (BYTE)0xFF;  // -1

				//g_DebugLog("[CLIENT] GOI_SWITCH_OBJECT: Right Click Move Src=(%d,%d,%d) DestPlace=%d", req.SrcPlace, req.SrcX, req.SrcY, req.DestPlace);

				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)&req, sizeof(req));
				return TRUE;
			}
			if (!pObject1)
			{
				memcpy(&P1, &P2, sizeof(P1));
			}
			if (!pObject2)
			{
				memcpy(&P2, &P1, sizeof(P1));
			}
			Player[CLIENT_PLAYER_INDEX].MoveItem(P1, P2);
		}
		break;
	case GOI_INFORMATION_CONFIRM_NOTIFY:
	{
		PLAYER_SELECTUI_COMMAND command;
		command.nSelectIndex = 0;
		command.m_szParam[0] = 0;
		Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_TALKDIALOG);
		break;
	}
	case GOI_QUESTION_CHOOSE:
		if (g_bUISelLastSelCount == 0 )
			break;	
		{
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex = nParam;
			command.m_szParam[0] = 0;
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_SELECTDIALOG);
		}
		break;

	case GOI_USE_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			int nPlace = nParam;
			ItemPos	Pos;
			switch(nPlace)
			{
			case UOC_ITEM_TAKE_WITH:
				Pos.nPlace = pos_equiproom;
				break;
			case UOC_IMMEDIA_ITEM:
				Pos.nPlace = pos_immediacy;
				break;
			case UOC_EXPAND_BOX1://Expandbox by kinnox;
				Pos.nPlace = pos_expandtoryroom1;
				break;
			case UOC_EQUIPTMENT:
				Pos.nPlace = pos_equip;
				break;				
			default:
				Pos.nPlace = -1;
				break;
			}
			Pos.nX = pInfo->Region.h;
			Pos.nY = pInfo->Region.v;
			if ((pInfo->Obj.uGenre == CGOG_ITEM || pInfo->Obj.uGenre == CGOG_IME_ITEM) && pInfo->Obj.uId > 0 && Pos.nPlace != -1)
				Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pInfo->Obj.uId, Pos);
		}
		break;
	case GOI_SET_IMMDIA_SKILL:
		if (uParam)
		{
			KUiGameObject* pSkill = (KUiGameObject*)uParam;
			if (nParam == 0)
			{
				if ( (int)pSkill->uId > 0 )
					Player[CLIENT_PLAYER_INDEX].SetLeftSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1)
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
					}
					
				}
			}
			else if (nParam == 1)
			{
				if ((int)pSkill->uId > 0)
					Player[CLIENT_PLAYER_INDEX].SetRightSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1)
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
					}
				}
			}
		}
		break;
	case GOI_TONE_UP_SKILL:
		Player[CLIENT_PLAYER_INDEX].ApplyAddSkillLevel((int)nParam, 1);
		break;
	case GOI_TONE_UP_ATTRIBUTE:
		if(nParam)
		{
			switch (uParam)
			{
				case UIPA_STRENGTH:
					Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(0, nParam);
					break;
				case UIPA_DEXTERITY:
					Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(1, nParam);
					break;		
				case UIPA_VITALITY:
					Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(2, nParam);
					break;
				case UIPA_ENERGY:
					Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(3, nParam);
					break;		
			}
		}
		break;
	case GOI_TRADE_INVITE_RESPONSE:
		if (uParam)
		{
			KTrade::ReplyInvite(((KUiPlayerItem*)uParam)->uId, nParam);
		}
		break;
	case GOI_TRADE_DESIRE_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			if (pInfo->Obj.uGenre != CGOG_MONEY)
				break;
			Player[CLIENT_PLAYER_INDEX].TradeMoveMoney(pInfo->Obj.uId);
		}
		break;
	case GOI_TRADE_WILLING:
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyClose();
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyOpen(NULL, 0);
		}
		break;
	case GOI_TRADE_LOCK:
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeLock)
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(FALSE);
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(TRUE);
		}
		break;
	case GOI_TRADE:
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeLock != TRUE || Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeDestLock != TRUE)
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_bTradeState == FALSE)
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(1);		
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(2);		
		}
		break;
	case GOI_TRADE_CANCEL:
		Player[CLIENT_PLAYER_INDEX].TradeDecision(0);
		break;
	case GOI_SEND_MSG:
		if (uParam && nParam)
			Player[CLIENT_PLAYER_INDEX].SendChat((KUiMsgParam*)nParam, (char*)uParam);
		break;
	case GOI_SET_SEND_CHAT_CHANNEL:
		if (uParam)
		{
			KUiChatChannel* pChannelInfo = (KUiChatChannel*)uParam;// pChannelInfo ????�?????????
			Player[CLIENT_PLAYER_INDEX].m_cChat.SetCurChannel(pChannelInfo->nChannelNo, pChannelInfo->uChannelId, pChannelInfo->nChannelIndex);
		}
		break;
	case GOI_CHAT_FRIEND_ADD:
		{
			if (g_pClient)
			{
				size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(ASK_ADDFRIEND_CMD);

				tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
				pExHeader->ProtocolType = c2s_extendfriend;
				pExHeader->wLength = pckgsize - 1;

				ASK_ADDFRIEND_CMD* pAafCmd = (ASK_ADDFRIEND_CMD*)(pExHeader + 1);
				pAafCmd->ProtocolFamily = pf_friend;
				pAafCmd->ProtocolID = friend_c2c_askaddfriend;
				pAafCmd->pckgid = -1;
				strncpy(pAafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
				g_pClient->SendPackToServer(pExHeader, pckgsize);
		
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_APPLY_ADD_FRIEND, ((KUiPlayerItem*)uParam)->Name);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
		break;
	case GOI_CHAT_WORDS_TO_FRIEND:
		{
			char	szSentence[MAX_SENTENCE_LENGTH];
			KUiChatMessage	*pMsg = (KUiChatMessage*)uParam;
			KUiMsgParam	sMsg;

			sMsg.eGenre = MSG_G_CHAT;
			sMsg.szName[0] = 0;
			memset(sMsg.cChatPrefix, 0, sizeof(sMsg.cChatPrefix));
			sMsg.cChatPrefixLen = sizeof(unsigned int);
			sMsg.cChatPrefix[0] = ((char*)(&pMsg->uColor))[3];
			sMsg.cChatPrefix[1] = ((char*)(&pMsg->uColor))[2];
			sMsg.cChatPrefix[2] = ((char*)(&pMsg->uColor))[1];
			sMsg.cChatPrefix[3] = ((char*)(&pMsg->uColor))[0];

			if (pMsg->nContentLen >= MAX_SENTENCE_LENGTH)
			{
				memcpy(szSentence, pMsg->szContent, MAX_SENTENCE_LENGTH - 1);
				szSentence[MAX_SENTENCE_LENGTH - 1] = 0;
				sMsg.nMsgLength = MAX_SENTENCE_LENGTH - 1;
			}
			else
			{
				memcpy(szSentence, pMsg->szContent, pMsg->nContentLen);
				szSentence[pMsg->nContentLen] = 0;
				sMsg.nMsgLength = pMsg->nContentLen;
			}

			Player[CLIENT_PLAYER_INDEX].m_cChat.QQSendSentence(
				((KUiPlayerItem*)nParam)->uId,
				((KUiPlayerItem*)nParam)->nIndex, &sMsg, szSentence);
		}
		break;
	case GOI_CHAT_FRIEND_INVITE:
		if (uParam)
		{
			if (g_pClient)
			{
				size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(REP_ADDFRIEND_CMD);

				tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
				pExHeader->ProtocolType = c2s_extendfriend;
				pExHeader->wLength = pckgsize - 1;
				
				REP_ADDFRIEND_CMD* pRafCmd = (REP_ADDFRIEND_CMD*)(pExHeader + 1);
				pRafCmd->ProtocolFamily = pf_friend;
				pRafCmd->ProtocolID = friend_c2c_repaddfriend;
				pRafCmd->pckgid = -1;
				strncpy(pRafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
				pRafCmd->answer = nParam ? answerAgree : answerDisagree;
				g_pClient->SendPackToServer(pExHeader, pckgsize);
			}
		}
		break;

	case GOI_OPTION_SETTING:
		if (uParam == OPTION_DYNALIGHT)
		{
			g_ScenePlace.EnableDynamicLights(nParam != 0);
			if (g_pRepresent)
				g_pRepresent->SetOption(DYNAMICLIGHT, nParam != 0);
		}
		else if (uParam == OPTION_PERSPECTIVE)
		{
			if (g_pRepresent)
				g_pRepresent->SetOption(PERSPECTIVE, nParam != 0);
		}
		else if (uParam == OPTION_MUSIC_VALUE)
			Option.SetMusicVolume(nParam);
		else if (uParam == OPTION_SOUND_VALUE)
			Option.SetSndVolume(nParam);
		else if (uParam == OPTION_BRIGHTNESS)
			Option.SetGamma(nParam);
		else if (uParam == OPTION_WEATHER)
			g_ScenePlace.EnableWeather(nParam);
		break;

	case GOI_VIEW_PLAYERITEM:
		{
			g_cViewItem.ApplyViewEquip(uParam);
		}
		break;
	case GOI_VIEW_PLAYERITEM_END:
		g_cViewItem.DeleteAll();
		break;

	case GOI_PLAYER_ACTION:
		{
			switch(uParam)
			{
			case PA_RUN:
				Player[CLIENT_PLAYER_INDEX].m_RunStatus = !Player[CLIENT_PLAYER_INDEX].m_RunStatus;
				break;
			case PA_SIT:
				if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
				{
					if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
						SendClientCmdSit(TRUE);
					}
					else
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
						SendClientCmdSit(FALSE);
					}
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_CLICK;
					Msg.eType = SMT_PLAYER;
					Msg.byPriority = 1;
					Msg.byParamSize = 0;
					strcpy(Msg.szMessage, MSG_NOT_SIT);
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
			case PA_RIDE: 
				if(Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse) <= 0)
					break;
				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
				{
				   if(GetTickCount() - Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse >= 10000)
					{	
					   if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
					   {
						   Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse = GetTickCount();
						   SendClientCmdRide(FALSE);
					   }
					   else
					   {
						   Player[CLIENT_PLAYER_INDEX].m_dwTimeHorse = GetTickCount();
						   SendClientCmdRide(TRUE);
					   }
					}
					else
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMT_NORMAL;
						Msg.eType = SMCT_NONE;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						sprintf(Msg.szMessage, MSG_RIDE_HORSE01);
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					 }
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMT_NORMAL;
					Msg.eType = SMCT_NONE;
					Msg.byPriority = 1;
					Msg.byParamSize = 0;
					sprintf(Msg.szMessage, MSG_RIDE_HORSE02);
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
			}
		}
		break;
	case GOI_PK_SETTING:
		Player[CLIENT_PLAYER_INDEX].m_cPK.ChangePKState(nParam);
		break;
	case GOI_REVENGE_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem	*pTarget = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cPK.ApplyEnmityPK(pTarget->uId);
		}
		break;
	case GOI_FOLLOW_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem	*pTarget = (KUiPlayerItem*)uParam;
			if (Npc[pTarget->nIndex].m_Kind == kind_player)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = pTarget->nIndex;
		}
		break;
	case GOI_SHOW_PLAYERS_NAME:
		NpcSet.SetShowNameFlag(nParam);
		break;
	case GOI_SHOW_PLAYERS_LIFE:
		NpcSet.SetShowLifeFlag(nParam);
		break;
	case GOI_SHOW_PLAYERS_MANA:
		NpcSet.SetShowManaFlag(nParam);
		break;
	case GOI_SHOW_OBJ_NAME:
        ObjSet.SetShowNameFlag(nParam); 
        break;
	case GOI_EXESCRIPT_BUTTON:
		SendClientCmdExeScript(nParam, (const char*)uParam);
		break;
	case GOI_PLAYER_ITEM_BREAK:
		{	
			if(nParam)
			{
				if(Player[CLIENT_PLAYER_INDEX].CheckTrading())
					break;
				
				if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop)
					break;

				if(Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand())
					break;

				KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

				if(!pObject1)
					break;
				if (CGOG_ITEM != pObject1->Obj.uGenre)
					break;
				if( (pObject1->Obj.uId <= 0) || (pObject1->Obj.uId >= MAX_ITEM) )
					break;
				if( !Item[pObject1->Obj.uId].CanStack() )
					break;
				if( Item[pObject1->Obj.uId].GetDurability() <=  nParam)
					break;

				SendClientCmdBreakItem(Item[pObject1->Obj.uId].GetID(), (int)nParam);// fix gioi han max number tach item by kinnox;
				nRet = 1;
			}
		}
		break;
	case GOI_PLAYER_BIND_ITEM:
		{	
			if(nParam)
			{
				if(Player[CLIENT_PLAYER_INDEX].CheckTrading())
					break;

				if(Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand())
					break;

				KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
				if(!pObject1)
					break;

				if (CGOG_ITEM != pObject1->Obj.uGenre)
					break;
				
				KItem* pItem = NULL;

				pItem = &Item[pObject1->Obj.uId];	
				if(!pItem)
					break;
				
				char szFunc[64];
				sprintf(szFunc, "%s|%ld", (char*)nParam, pItem->GetID());
				SendClientCmdExeScript(2, szFunc);
				nRet = 1;
			}
		}
		break;
	case GOI_HIDE_MASK:
		{
			Player[CLIENT_PLAYER_INDEX].HideMask(uParam);
		}
		break;
	case GOI_MARK_PRICE_ITEM:
		{
			if(uParam)
			{
				if(Player[CLIENT_PLAYER_INDEX].CheckTrading())
					break;
				if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop)
					break;

				KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
				if (CGOG_ITEM != pObject->Obj.uGenre)
						break;

				KItem* pItem = NULL;
				pItem = &Item[pObject->Obj.uId];

				if(!pItem)
					break;

				if(pItem->GetGenre() == item_task ||
					pItem->GetGenre() == item_script)
				{
					if(pItem->GetParticular() == 0)
						break;
				}
				if(pItem->GetBindState() > 0)
					break;

				SendClientCmdMarkPriceItem(pItem->GetID(), nParam);
			}
				
		}
		break;
	case GOI_MAKE_ADV_TEXT:
		{
			if(uParam)
			{
				if(Player[CLIENT_PLAYER_INDEX].CheckTrading())
					break;
				if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop)
					break;

				SendClientCmdMakeAdvText((const char*)uParam);
			}
				
		}
		break;
	case GOI_SHOP_WILLING:
		{	
			if(uParam)
			{
				nRet = Player[CLIENT_PLAYER_INDEX].ShopApplyOpen();
			}
			else
			{
				nRet = Player[CLIENT_PLAYER_INDEX].ShopApplyClose();
			}
		}
		break;
	case GOI_VIEW_PLAYERSHOP:
		if(uParam)
		{
			Player[CLIENT_PLAYER_INDEX].m_cShop.ApplyViewShop(uParam);
		}
		break;
	case GOI_VIEW_PLAYERSHOP_END:
		Player[CLIENT_PLAYER_INDEX].m_cShop.DeleteAll();
		break;
	case GOI_BUY_PLAYER_SHOP:
		if(uParam)
		{
			KUiObjAtContRegion* pObject = (KUiObjAtContRegion*)uParam;
			if (CGOG_PLAYERSELLITEM != pObject->Obj.uGenre)
				break;

			int nWidth, nHeight;
			ItemPos	Pos;
			
			nWidth = Item[pObject->Obj.uId].GetWidth();
			nHeight = Item[pObject->Obj.uId].GetHeight();

			if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
			{
				nRet = 0;
				break;
			}

			if (Pos.nPlace != pos_equiproom)
			{
				nRet = 0;
				KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}

			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < Item[pObject->Obj.uId].GetPlayerShopPrice())
			{
				nRet = 0;
				KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
				
			}

			DWORD dwID = Player[CLIENT_PLAYER_INDEX].m_cShop.FindID(pObject->Obj.uId);
			SendClientCmdBuyPlayerShop(dwID);
		}
		break;
	case GOI_ASK_CLIENT_NUMBER_CHOOSE:
		{	
			if (g_bUISelLastSelCount == 0 )
				break;
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex	= nParam;
			sprintf(command.m_szParam, "%ld", uParam);
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_ASKFORNUMBER);
		}
		break;
	case GOI_ASK_CLIENT_STRING_CHOOSE:
		{	
			if (g_bUISelLastSelCount == 0 )
				break;
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex	= nParam;
			sprintf(command.m_szParam, "%s", (char*)uParam);
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_ASKFORSTRING);
		}
		break;
	case GOI_CHANGE_TAX_CHOOSE:
		{	
			if (g_bUISelLastSelCount == 0 )
				break;
			PLAYER_SELECTUI_COMMAND command;
			if(nParam >= 0)
			{
				command.nSelectIndex	= 0;
				sprintf(command.m_szParam, "%d|%d", uParam, nParam);
			}
			else
			{
				command.nSelectIndex	= 1;
				command.m_szParam[0]	= 0;
			}
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_CHANGETAXDIALOG);
		}
		break;
	case GOI_PLAYER_CREATE_PWD://LockPlayer by kinnox;
		{
			SendClientCreatPWCmd(uParam);
		}
		break;
	case GOI_PLAYER_LOGIN_LOCK_PWD://LockPlayer by kinnox;
		{
			SendClientLoginPWCmd(uParam);
		}
		break;
	case GOI_PLAYER_CHANGE_PWD://LockPlayer by kinnox;
		{
			SendClientChaPWCmd(uParam,nParam);
		}
		break;
	case GOI_PLAYER_LOCK_PWD://LockPlayer by kinnox;
		{
			SendClientLockPWCmd(uParam);
		}
		break;		
	case GOI_GIVEBOX_SCRIPT://GiveBox by kinnox;
		if (uParam)
		{
			SendClientGiveBoxScript((char*)uParam);
		}
		break;	
	case GOI_GIVEBOX_COMMAND://GiveBox by kinnox;
		if (uParam)
		{
			DWORD dwID;
			int nW, nH;		
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == uParam)
			{
				dwID = Item[pItem->nIdx].GetID();
				nW = Item[pItem->nIdx].GetWidth();
				nH = Item[pItem->nIdx].GetHeight();
				
				SendClientGiveBoxCmd(dwID, nW, nH);
			}
			
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == uParam)
				{
					dwID = Item[pItem->nIdx].GetID();
					nW = Item[pItem->nIdx].GetWidth();
					nH = Item[pItem->nIdx].GetHeight();
									
					SendClientGiveBoxCmd(dwID, nW, nH);	
				}
			}
		}	
		break;	
	case GOI_REWARD_CMD_SCRIPT://Reward by kinnox;
		if (uParam)
		{
			SendClientReWardScript((char*)uParam);
		}
		break;
	case GOI_OPEN_TOP_TK_NEW:
		SendClientCmdOpenTopTKNew(); //Report Battle By Kinnox;
		break;
	case GOI_TIME_BOX:
		SendClientCmdTimeBox((char*)uParam,(int)nParam);//TimeBox By Kinnox;
		break;		
	case GOI_SET_IMAGE_PLAYER://Player Avatar by kinnox;
		{
			SendClientCPSetImageCmd(uParam);
		}
		break;	
	case GOI_RECOVERY_BOX_COMMAND://TrembleItem by kinnox;
		if (uParam)
		{
			DWORD dwID;
			int nW, nH;		
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == uParam)
			{
				dwID = Item[pItem->nIdx].GetID();
				nW = Item[pItem->nIdx].GetWidth();
				nH = Item[pItem->nIdx].GetHeight();
				
				SendClientRecoveryBox(dwID, nW, nH);
			}
			
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == uParam)
				{
					dwID = Item[pItem->nIdx].GetID();
					nW = Item[pItem->nIdx].GetWidth();
					nH = Item[pItem->nIdx].GetHeight();
									
					SendClientRecoveryBox(dwID, nW, nH);	
				}
			}
		}	
		break;			
	default:
		nRet = 0;
		break;
	}

	return nRet;
}


void KCoreShell::ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam)
{
	Player[CLIENT_PLAYER_INDEX].ProcessInputMsg(uMsg, uParam, nParam);
}

int KCoreShell::FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, nRelation);
	int nT = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();

	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);

	if (nT > 0)
	{	
		if(Npc[nT].m_HideState.nTime > 0)
			return false;

		if (pReturn)
		{
			KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
			strncpy(p->Name, Npc[nT].Name, 32);
			p->nIndex = Npc[nT].m_Index;
			p->uId = Npc[nT].m_dwID;
			p->nData = Npc[nT].GetMenuState();
		}
		nKind = Npc[nT].m_Kind;
		return true;
	}
	return false;
}

int KCoreShell::FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
	int nT = Player[CLIENT_PLAYER_INDEX].GetTargetObj();
	
	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetObj(0);

	if (nT > 0)
	{
		nObjectIdx = nT;
		nKind = Object[nT].m_nKind;
		return true;
	}
	return false;
}


int KCoreShell::FindSpecialNPC(char* Name, void* pReturn, int& nKind)
{
	if (Name == NULL || Name[0] == 0)
		return false;
	for (int nT = 0; nT < MAX_NPC; nT++)
	{
		if	(strcmp(Npc[nT].Name, Name) == 0)
		{
			if (pReturn)
			{
				KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
				strncpy(p->Name, Npc[nT].Name, 32);
				p->nIndex = Npc[nT].m_Index;
				p->uId = Npc[nT].m_dwID;
				p->nData = Npc[nT].GetMenuState();
			}
			nKind = Npc[nT].m_Kind;
			return true;
		}
	}
	return false;
}

int KCoreShell::ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC)
		{
			int nTalker = p->nIndex;
			if (Npc[nTalker].m_Kind == kind_player &&
				Npc[nTalker].m_dwID == p->uId)
			{
				Npc[nTalker].SetChatInfo(p->Name, pMsgBuff, nMsgLength);
				return true;
			}
		}
	}

	return false;
}

void KCoreShell::ApplyAddTeam(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(p->nIndex);
		}
	}
}


//	

void KCoreShell::TradeApplyStart(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{	
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC)
		{	
			Player[CLIENT_PLAYER_INDEX].TradeApplyStart(p->nIndex);
		}
	}
}


//

int KCoreShell::UseSkill(int x, int y, int nSkillID, BOOL bFlag /* = FALSE*/)
{	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (Npc[nIndex].m_bOpenShop)
		return 0;
	if( Player[CLIENT_PLAYER_INDEX].CheckTrading() )
		return 0;
	
	int nX = x;
	int nY = y;
	int nZ = 0;

	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);

	if( Npc[nIndex].IsCanInput() )
	{
		int nListIdx = 0;
		nListIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		if(nListIdx <= 0 || nListIdx >= MAX_SKILL)
			return 0;
		Npc[nIndex].SetActiveSkill(nListIdx);
	}
	else
	{
		return 0;
	}

	ISkill* pSkill =  Npc[nIndex].GetActiveSkill();
	if(!pSkill) 
        return 0;

	if (pSkill->IsAura())
		return 0;

	int nAttackRange = pSkill->GetAttackRadius();
	//
	if (bFlag && !Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].FindStateSkill(nSkillID) && nAttackRange >= 300)
	{
		int nDestX, nDestY;
		Npc[nIndex].GetMpsPos(&nDestX, &nDestY);

		int	nRange = g_GetDistance(nDestX, nDestY, nX, nY);
		if (nAttackRange >= 300)
			nAttackRange = nAttackRange - 100;
		if (nRange > nAttackRange)
		{
			return 0;
		}
	}	

	//

	int nTargetIdx = 0;
	//
	if( pSkill->IsTargetAlly() )
	{
		Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_ally);
		nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
	}
	//
	if( pSkill->IsTargetEnemy() )
	{
		Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_enemy);
		nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
	}
	//
	if( pSkill->IsTargetObj() )
	{
		Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
		nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetObj();
	}
	//
	if( pSkill->IsTargetOnly() )
    {	
		if(!nTargetIdx)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}
	}
	//
	if(nTargetIdx == nIndex)
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
		return 0;
	}
	//
	if( !Npc[nIndex].m_SkillList.CanCast(Npc[nIndex].m_ActiveSkillID, SubWorld[Npc[nIndex].m_SubWorldIndex].m_dwCurrentTime) )
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
		return 0;
	}
	//
	if( !Npc[nIndex].Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(&Npc[nIndex]), TRUE) )
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
		return 0;
	}

	if(!nTargetIdx)
	{
		Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, nX, nY);
		SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, nX, nY);
	}
	else
	{	
		int nTemp = -1;
		if( !pSkill->CanCastSkill(nIndex, nTemp, nTargetIdx) )
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(nTargetIdx);
			return 0;
		}
		Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, nTemp, nTargetIdx);
		SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, nTemp, Npc[nTargetIdx].m_dwID);
	}
	Npc[nIndex].m_nPeopleIdx = 0;
	return 1;
}


//

int KCoreShell::LockSomeoneUseSkill(int nTargetIndex, int nSkillID)
{	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (Npc[nIndex].m_bOpenShop)
		return 0;
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	if (nTargetIndex == nIndex)
		return 0;

	if( Npc[nIndex].IsCanInput() )
	{
		int nListIdx = 0;
		nListIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		if(nListIdx <= 0 || nListIdx >= MAX_SKILL)
			return 0;
		Npc[nIndex].SetActiveSkill(nListIdx);
	}
	else
	{
		return 0;
	}

	int nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if(nRelation == relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}
	return 0;
}


//

int KCoreShell::LockSomeoneAction(int nTargetIndex)
{	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (Npc[nIndex].m_bOpenShop)
		return 0;
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	if(nTargetIndex == nIndex)
		return 0;

	if(nTargetIndex <= 0 || nTargetIndex >= MAX_NPC)
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		return 1;
	}

	int nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if (nRelation != relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}
	return 0;
}


//	

int KCoreShell::LockObjectAction(int nTargetIndex)
{	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (Npc[nIndex].m_bOpenShop)
		return 0;

	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	if(nTargetIndex <= 0)
		Npc[nIndex].m_nObjectIdx = 0;
	else
		Npc[nIndex].m_nObjectIdx = nTargetIndex;

	return 1;
}


//	

void KCoreShell::GotoWhere(int x, int y, int mode)
{
	
	if (mode < 0 || mode > 2)
		return;
	g_ScenePlace.JxPathFinderClear();
	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		int bRun = false;

		if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
			mode == 2)
			bRun = true;

		int nX = x;
		int nY = y;
		int nZ = 0;
		g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
		int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		
		if(!bRun)
		{
			Npc[nIndex].SendCommand(do_walk, nX, nY);
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[nIndex].SendCommand(do_run, nX, nY);
			SendClientCmdRun(nX, nY);
		}
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}

// find way by kinnox;
void KCoreShell::GotoWhereDirect(int x, int y, int mode)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return;

	if (mode < 0 || mode > 2)
		return;

	int nPlayerX, nPlayerY;
	int nDestX, nDestY, dX, dY;

	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		int bRun = false;

		if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
			mode == 2)
			bRun = true;

		int nX = x;
		int nY = y;
		int nZ = 0;

		int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nPlayerX, &nPlayerY);
		dX = nPlayerX & 0x1F;
		dY = nPlayerY & 0x1F;
		nDestX = nX + 0x10 - dX; // Anti-LAG
		nDestY = nY + 0x10 - dY; // Anti-LAG	

		if (!bRun)
		{
			Npc[nIndex].SendCommand(do_walk, nDestX, nDestY);
			// Send to Server
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[nIndex].SendCommand(do_run, nDestX, nDestY);
			// Send to Server
			SendClientCmdRun(nX, nY);
		}
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}

//find way by kinnox
bool CloseToTarget(const FindPathNode& point1, const FindPathNode& point2, int delta) 
 {
	int distance = sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
	//g_DebugLog("Distance %d\n", distance);
	return distance <= delta;
}

BOOL KCoreShell::AutoMove() //find way by kinnox
{
	FindPathNode nextPoint;
	FindPathNode currentPoint;
	FindPathNode endPoint;
	int dX, dY;
	INT nCurX, nCurY;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nCurX, &nCurY);
	BOOL nRet = FALSE;
	if (!GetPaintFindPos())
		return nRet;
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
	{
		//go to the next point in auto move path
		nextPoint = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.front();
		currentPoint.x = nCurX;
		currentPoint.y = nCurY;
		if (g_JXPathFinder.GetNextStep(currentPoint, nextPoint, 2) == emKNEXTSTEP_RESULT_SUCCESS) 
		{		
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
				return nRet;
			nextPoint = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.front();
			endPoint  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.back();		
			dX = currentPoint.x & 0x1F;
			dY = currentPoint.y & 0x1F;
			nextPoint.x = nextPoint.x + 0x10 - dX; // Anti-LAG
			nextPoint.y = nextPoint.y + 0x10 - dY; // Anti-LAG	
			if ((CloseToTarget(currentPoint, nextPoint, 189) || (g_JXPathFinder.getStand().x == currentPoint.x && g_JXPathFinder.getStand().y == currentPoint.y)))
			{
				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_stand)
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing = do_run;
				if ((nextPoint.x == endPoint.x) && (nextPoint.y == endPoint.y))
				{
					if ((CloseToTarget(currentPoint, nextPoint, 3)))
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
				}
				else
				{
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
				}
			}
			else
				g_JXPathFinder.setStand(currentPoint.x, currentPoint.y);
				//g_DebugLog("Running to %d, %d from Current %d, %d\n", nextPoint.x, nextPoint.y, currentPoint.x, currentPoint.y);
			if ((nextPoint.x - currentPoint.x) < 0 && (nextPoint.y - currentPoint.y > 0) || (nextPoint.x - currentPoint.x) < 0 && (nextPoint.y == currentPoint.y))
			{
				int nBarrier = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].GetBarrier(nextPoint.x, nextPoint.y);
				if (Obstacle_NULL == nBarrier)
					GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			}
			else
			{
				int nBarrier = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].GetBarrier(nextPoint.x, nextPoint.y);	
				if (Obstacle_NULL == nBarrier)
					GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			}
			nRet = TRUE;

		}
		else if (g_JXPathFinder.GetNextStep(currentPoint, nextPoint, 1) == emKNEXTSTEP_RESULT_ARRIVAL)
		{
				GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			return nRet; //Still moving
		}
	}
	else
	{
		return nRet; //Nothing in auto move path
	}
	return 1;
 }

//
//	

void KCoreShell::Goto(int nDir, int mode)
{
	if (nDir < 0 || nDir > 63)
		return;

	if (mode < 0 || mode > 2)
		return;

	int bRun = false;

	if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
		mode == 2)
		bRun = true;

	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	int nSpeed;
	if (bRun)
		nSpeed = Npc[nIndex].m_CurrentRunSpeed;
	else
		nSpeed = Npc[nIndex].m_CurrentWalkSpeed;

	Player[CLIENT_PLAYER_INDEX].Walk(nDir, nSpeed);

	Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
}


//

void KCoreShell::Turn(int nDir)
{
	if (nDir < 0 || nDir > 3)
		return;

	if (nDir == 0)
		Player[CLIENT_PLAYER_INDEX].TurnLeft();
	else if (nDir == 1)
		Player[CLIENT_PLAYER_INDEX].TurnRight();
	else
		Player[CLIENT_PLAYER_INDEX].TurnBack();
}


//	

int KCoreShell::ThrowAwayItem()
{
	return Player[CLIENT_PLAYER_INDEX].ThrowAwayItem();
}
// fix kiem tra chuot by kinnox
BOOL KCoreShell::CheckMouse() 
{
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand())
		return TRUE;
	return FALSE;
}
//endcode


//

int KCoreShell::GetNPCRelation(int nIndex)
{
	return NpcSet.GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex, nIndex);
}


BOOL KCoreShell::GetNPCShopState(int nIndex)
{
	return Npc[nIndex].m_bOpenShop;
}


void KCoreShell::DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam)
{
	if(g_pRepresent)
		CoreDrawGameObj(uObjGenre, uId, x, y, Width, Height, nParam);
}

#include "../../Represent/iRepresent/iRepresentshell.h"


void KCoreShell::DrawGameSpace()
{
	if(g_pRepresent)
	{
		g_ScenePlace.Paint();
		Player[CLIENT_PLAYER_INDEX].DrawSelectInfo();
	}
}

void KCoreShell::SetRepresentShell(struct iRepresentShell* pRepresent)
{
	g_pRepresent = pRepresent;
	g_ScenePlace.RepresentShellReset();
	if (g_pAdjustColorTab && g_ulAdjustColorCount && g_pRepresent)
		g_pRepresent->SetAdjustColorList(g_pAdjustColorTab, g_ulAdjustColorCount);
}


//

void KCoreShell::SetMusicInterface(void* pMusicInterface)
{
	g_pMusic = (KMusic*)pMusicInterface;
	Option.SetMusicVolume(Option.GetMusicVolume());
}


//

int KCoreShell::Breathe()
{
	g_SubWorldSet.MessageLoop();
	g_SubWorldSet.MainLoop();
	g_ScenePlace.Breathe();
	return true;
}


//

int KCoreShell::GetProtocolSize(BYTE byProtocol)
{
	if (byProtocol <= s2c_clientbegin || byProtocol >= s2c_end)
		return -1;
	return g_nProtocolSize[byProtocol - s2c_clientbegin - 1];
}

#ifdef SWORDONLINE_SHOW_DBUG_INFO
extern int		g_bShowObstacle;
extern bool		g_bShowGameInfo;
#endif


//

int KCoreShell::Debug(unsigned int uDataId, unsigned int uParam, int nParam)
{
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	switch(uDataId)
	{
	case DEBUG_SHOWINFO:
		Player[CLIENT_PLAYER_INDEX].m_bDebugMode = !Player[CLIENT_PLAYER_INDEX].m_bDebugMode;
		g_bShowGameInfo = !g_bShowGameInfo;
		break;
	case DEBUG_SHOWOBSTACLE:
		g_bShowObstacle = !g_bShowObstacle;
		break;
	}
#endif
	return 0;
}


//

DWORD KCoreShell::GetPing()
{
	return g_SubWorldSet.GetPing();
}


//

void KCoreShell::SetRepresentAreaSize(int nWidth, int nHeight)
{
	g_ScenePlace.SetRepresentAreaSize(nWidth, nHeight);
}


//

void KCoreShell::SetClient(LPVOID pClient)
{
	g_SetClient(pClient);
}


//

void KCoreShell::SendNewDataToServer(void* pData, int nLength)
{
	if (g_pClient)
		g_pClient->SendPackToServer(pData, nLength);
}
// --
//
// --

int KCoreShell::SystemMessages(unsigned int uDataId, unsigned int uParam, int nParam)
{	
	switch(uDataId)
	{	
	case GSM_FIGHT_MODE_ON:
		l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H? th?ng", MSG_FIGHT_MODE_ON, 
														strlen(MSG_FIGHT_MODE_ON), TRUE); 
		break;
	case GSM_FEATURE_BUIDING:
		{
#ifdef _DEBUG
			KTabFile Tab;
			if(!Tab.Load("\\Unpack\\List.txt")) 
				break;

			//g_DebugLog("[Unpack] Load List File OK");
			char szString[128]={0};
			char szNewPath[128]={0};
			char szCreated[128]={0};
			char szCreated2[128]={0};
			int i = 0;
			while(1)
			{
				Tab.GetString(i+1, 1, "", szString, sizeof(szString));
				if(!szString[0]) 
					break;
				KPakFile PakFile;
				ZeroMemory(szNewPath,sizeof(szNewPath));
				ZeroMemory(szCreated,sizeof(szCreated));
				ZeroMemory(szCreated2,sizeof(szCreated2));
				if(PakFile.Open4Unpack(szString, nParam - 1))
				{
					g_ExtractFilePath(szCreated,szString);
					//g_DebugLog("[Unpack]Create File Path: [%s]",szCreated);
					strcat(szNewPath,"\\UnPack");
					strcat(szCreated2,"\\UnPack");
					strcat(szCreated2,szCreated);
					g_CreatePath(szCreated2);
					strcat(szNewPath,szString);
					//g_DebugLog("[Unpack]File: [%s]",szNewPath);
					PakFile.Save(szNewPath);
					PakFile.Close();
				}
				i++;
			}
#endif
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H? th?ng", MSG_FEATURE_BUIDING, 
															strlen(MSG_FEATURE_BUIDING), TRUE); 
		}
		break;
	case GSM_PLAYER_RIDEHORSE:
		l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H? th?ng", MSG_PLAYER_RIDEHORSE, 
															strlen(MSG_PLAYER_RIDEHORSE), TRUE); 
		break;
	case GSM_MAP_TYPE_ERROR:
		l_pDataChangedNotifyFunc->ChannelMessageArrival(0, "H? th?ng", MSG_MAP_TYPE_ERROR, 
															strlen(MSG_MAP_TYPE_ERROR), TRUE); 
		break;
	default:
		break;
	}
	return 0;
}

// --
//
// --
int	KCoreShell::SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case GSMOI_SCENE_TIME_INFO:
		if (uParam)
		{
			KUiSceneTimeInfo* pInfo = (KUiSceneTimeInfo*)uParam;
			g_ScenePlace.GetSceneNameAndFocus(pInfo->szSceneName, pInfo->nSceneId,
				pInfo->nScenePos0, pInfo->nScenePos1);
			pInfo->nGameSpaceTime = (SubWorld[0].m_dwCurrentTime / 100) % 1440;
		}
		break;
	case GSMOI_SCENE_MAP_INFO:
		nRet = g_ScenePlace.GetMapInfo((KSceneMapInfo*)uParam);
		break;
	case GSMOI_IS_SCENE_MAP_SHOWING:
		g_ScenePlace.SetMapParam(uParam, nParam);
		break;
	case GSMOI_PAINT_SCENE_MAP:
		g_ScenePlace.PaintMap(uParam, nParam);
		break;
	case GSMOI_SCENE_MAP_FOCUS_OFFSET:
		g_ScenePlace.SetMapFocusPositionOffset((int)uParam, nParam);
		break;
	case GSMOI_SCENE_FOLLOW_WITH_MAP:
		g_ScenePlace.FollowMapMove(nParam);
		break;
	case GSMOI_PAINT_LINE:
		{	

		}
		break;
	case GSMOI_SCENE_CALCFLAG:
		if (uParam)
		{
			KUiSceneCalcPos* pInfo = (KUiSceneCalcPos*)uParam;
			g_ScenePlace.GetSceneCalgFlagPos(pInfo->nScenePos0, pInfo->nScenePos1);
		}
		break;	

	}
	return nRet;
}

// --
//
// --
int	KCoreShell::TongOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case GTOI_TONG_CREATE:
		Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyCreateTong(nParam, (char *)uParam);
		break;
	case GTOI_TONG_IS_RECRUIT:
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			nRet = Npc[pItem->nIndex].m_nTongFlag;
		}
		break;
	case GTOI_TONG_RECRUIT:
		break;
	case GTOI_TONG_ACTION:         
		{
    		KTongOperationParam *Oper = (KTongOperationParam *)uParam;
		    KTongMemberItem *TargetInfo = (KTongMemberItem *)nParam;
		    KUiPlayerItem TargetPlayer;
		    int nKind;

		    switch(Oper->eOper)
		    {
		    case TONG_ACTION_KICKOUT:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyKick(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
    			break;
		    case TONG_ACTION_ASSIGN:
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInstate(Oper->nData[0], Oper->nData[1], Oper->nData[2], Oper->nData[3], TargetInfo->Name);
			    break;
		    case TONG_ACTION_DEMISE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeMaster(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
    			break;
		    case TONG_ACTION_LEAVE:
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyLeave();
    			break;
		    case TONG_ACTION_RECRUIT:
    			break;
		    case TONG_ACTION_APPLY:
				if(!FindSpecialNPC(TargetInfo->Name, &TargetPlayer, nKind))
				{
    				break;
				}
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyAddTong(TargetPlayer.uId);
			    break;
		    }
		}
		break;
	case GTOI_TONG_JOIN_REPLY:
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cTong.AcceptMember(pItem->nIndex, g_FileName2Id(pItem->Name), nParam);
		}
		break;

	case GTOI_REQUEST_PLAYER_TONG:	
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			if (pItem->nIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
		}
		break;
	case GTOI_REQUEST_TONG_DATA:     
		if (uParam)
		{
			if (Player[CLIENT_PLAYER_INDEX].m_cTong.CheckIn() == 0)
				break;

			KUiGameObjectWithName	*pObj = (KUiGameObjectWithName*)uParam;
			char	szTongName[32];
			DWORD	dwTongNameID;

			szTongName[0] = 0;
			Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);
			if (!szTongName[0])
				break;

			dwTongNameID = g_FileName2Id(pObj->szName);
			if (dwTongNameID != Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID())
				break;

			switch (pObj->nData)
			{
			case enumTONG_FIGURE_MASTER:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex, 0, 0);
				break;
			case enumTONG_FIGURE_DIRECTOR:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex, 0, 0);
				break;
			case enumTONG_FIGURE_MANAGER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetManagerInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MANAGER,
					dwTongNameID, pObj->nParam, nParam);
				break;
			case enumTONG_FIGURE_MEMBER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetMemberInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MEMBER,
					dwTongNameID, pObj->nParam, nParam);
				break;
			}
		}
		break;
	case GTOI_TONG_CHANGE_CAMP:
		if(uParam)
		{
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeCamp(uParam);
		}
		break;
	case GTOI_TONG_CHANGE_TITLE:
		{
			if(!uParam)
				break;
			nRet = 0;
			KTongMemberItem* TargetInfo = (KTongMemberItem*)uParam;
			switch(TargetInfo->nIndex)
			{
			case TONG_ACTION_MALE_TITLE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeMemberTitle(TargetInfo->szAgname, 0);
				break;
			case TONG_ACTION_FEMALE_TITLE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeMemberTitle(TargetInfo->szAgname, 1);
				break;
			case TONG_ACTION_TITLE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeOneTitle(TargetInfo->Name, TargetInfo->nData, TargetInfo->szAgname);
				break;
			default:
				break;
			}

		}
		break;
	case GTOI_TONG_CHANGE_MONEY:
		{
			if(!uParam)
				break;
			
			nRet = 0;
			switch(uParam)
			{
			case TONG_ACTION_SAVE_MONEY:
				nRet = Player[CLIENT_PLAYER_INDEX].m_cTong.ApplySaveMoney(nParam);
				break;
			case TONG_ACTION_GET_MONEY:
				nRet = Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyGetMoney(nParam);
				break;
			case TONG_ACTION_SND_MONEY:
				break;
			default:
				break;
			}
		}
		break;
	}
	return nRet;
}


int KCoreShell::TeamOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case TEAM_OI_GD_INFO:
		if (uParam)
		{
			KUiPlayerTeam* pTeam = (KUiPlayerTeam*)uParam;
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetInfo(pTeam);
		}
		break;
	case TEAM_OI_GD_MEMBER_LIST:
		nRet = g_Team[0].GetMemberInfo((KUiPlayerItem *)uParam, nParam);
		break;
	case TEAM_OI_GD_REFUSE_INVITE_STATUS:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetAutoRefuseState();
		break;
	case TEAM_OI_COLLECT_NEARBY_LIST:
		NpcSet.GetAroundOpenCaptain(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp);
		break;
	case TEAM_OI_APPLY:
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(((KUiTeamItem*)uParam)->Leader.nIndex);
		break;
	case TEAM_OI_CREATE:
		Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();
		break;
	case TEAM_OI_APPOINT:
		Player[CLIENT_PLAYER_INDEX].ApplyTeamChangeCaptain(((KUiPlayerItem*)uParam)->uId);		
		break;
	case TEAM_OI_INVITE:
		if (uParam)
		{
			Player[CLIENT_PLAYER_INDEX].TeamInviteAdd(((KUiPlayerItem*)uParam)->uId);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_SEND_INVITE, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case TEAM_OI_KICK:
		Player[CLIENT_PLAYER_INDEX].TeamKickMember(((KUiPlayerItem*)uParam)->uId);
		break;
	case TEAM_OI_LEAVE:
		Player[CLIENT_PLAYER_INDEX].LeaveTeam();
		break;
	case TEAM_OI_CLOSE:
		Player[CLIENT_PLAYER_INDEX].ApplyTeamOpenClose(nParam);
		break;
	case TEAM_OI_REFUSE_INVITE:
		Player[CLIENT_PLAYER_INDEX].m_cTeam.SetAutoRefuseInvite(nParam);
		break;
	case TEAM_OI_APPLY_RESPONSE:
		if (uParam)
		{
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].AcceptTeamMember(((KUiPlayerItem*)uParam)->uId);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(((KUiPlayerItem*)uParam)->uId);
			}
		}
		break;
	case TEAM_OI_INVITE_RESPONSE:
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		break;
	}
	return nRet;
}

BOOL KCoreShell::CanGetSkill(int nSkillId)
{
	if (nSkillId > 0 && nSkillId < MAX_SKILL)
	{
		ISkill * pISkill =  g_SkillManager.GetSkill(nSkillId, 1);
		if (!pISkill) 
            return FALSE;

		if (pISkill->IsAura())
			return FALSE;

		if (pISkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			return FALSE;
	
		if (/*pISkill->IsTargetOnly() || */
			nSkillId == 1 || 
			nSkillId == 2 || 
			nSkillId == 53)
			return FALSE;

		return TRUE;
	}
	return FALSE;
}

int KCoreShell::PAIOperation(unsigned int uOper, unsigned int uParam, int nParam, int zParam) //Auto by kinnox;
 {
	int nRet = 1;
	switch(uOper)
	{
		case GPI_SWITCH_ACTIVE: 
		{
			if (uParam)
				Player[CLIENT_PLAYER_INDEX].m_cAI.ResetAuto();
			SendClientCmdAutoPlay(1,uParam);
		}
		break;
		//////////////////////////AUTO FIGHT
		case GPI_F_FOLLOW_TAGET:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFollowAttack = uParam;	
		}
		break;
		case GPI_F_NORMAL:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAttackNpc = uParam;
		}
		break;
		case GPI_F_PEOPLE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAttackPeople = uParam;
		}
		break;
		case GPI_F_SPACE:
		{
			 Player[CLIENT_PLAYER_INDEX].m_cAI.m_SpaceBar = uParam;
		}
		break;
		case GPI_F_HOLDSPACE:
		{
			 Player[CLIENT_PLAYER_INDEX].m_cAI.m_HoldSpaceBar = uParam;
		}
		break;
		case GPI_F_MOUSE:
		{
			 Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPriorityUseMouse = uParam;
		}
		break;
		case GPI_F_RADIUS_NUM:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_RadiusAuto = uParam;
		}
		break;
		case GPI_F_VINCINITY:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFightDistance = uParam;
		}
		break;
		case GPI_F_VINCINITY_NUM:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_DistanceAuto = uParam;
		}
		break;
		case GPI_F_DEFENSE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nDefense = uParam;
		}
		break;
		case GPI_F_FIGHTBOSS:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFightBoss = uParam;
		}
		break;
		case GPI_F_RIDER:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bRiderHorse = uParam;
		}
		break;
		case GPI_F_SUPPORT_SKILLID:
		{
			if (uParam >= 0 && nParam <= MAX_SKILL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_ArrayStateSkill[uParam] = nParam;
		}
		break;	
		case GPI_F_AURA_SKILLID:
		{
			if (uParam >= 0 && nParam <= MAX_SKILL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_AuraSkill[uParam] = nParam;
		}
		break;
		case GPI_F_LEFT_SKILL:
		{
			if (uParam >= 0 && nParam <= MAX_SKILL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_nLeftSkillSet[uParam] = nParam;
		}
		break;
		case GPI_F_RIGHTHAND_SKILL:
		{
			if (uParam >= 0 && nParam <= MAX_SKILL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_nRightHandSkillSet[uParam] = nParam;
		}
		break;
		//////////////////////////AUTO MOVE
		case GPI_M_FOLLOW_PEOPLE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFollowPeople = uParam;
		}
		break;
		case GPI_M_FOLLOW_PEOPLE_NAME:	
		{
			strcpy(Player[CLIENT_PLAYER_INDEX].m_cAI.m_FollowPeopleName, (char*)uParam);
		}
		break;
		case GPI_M_FOLLOW_PEOPLE_RADIUS:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nRadiusFollow = nParam;
		}
		break;
		case GPI_M_AROUND:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAroundBtn=uParam;
		}		
		case GPI_M_AROUND_NUM:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.SendAroundAuto(uParam);
		}
		break;
		case GPI_M_MOVEPOS:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoMove = uParam;
		}
		break;
		//////////////////////////AUTO OTHER
		case GPI_O_PARTYCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoParty = uParam;
		}
		break;
		case GPI_O_PARTYINVITE: 
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoInvite = uParam;
		}
		break;
		case GPI_O_PARTYLIST: 
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoInviteList = uParam;
		}
		break;
		case GPI_O_PARTYLISTNAME:
		{
			if (nParam >= 0 && nParam < defMAX_LIST_PARTY)
				strcpy(Player[CLIENT_PLAYER_INDEX].m_cAI.m_szPartyList[nParam], (char*)uParam);
		}
		break;
		case GPI_O_PARTYACCEPT:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoAccept = uParam;
		}
		break;
		case GPI_O_PARTYACCEPT_ALL:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoAcceptAll = uParam;
		}
		break;
		case GPI_O_PARTYACCEPT_WITH_NAME:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoAcceptWithName = uParam;
		}
		break;
		case GPI_O_OPTIMAL_NPC:
		{
			 Player[CLIENT_PLAYER_INDEX].m_bHideNpc = uParam;
		}
		break;
		case GPI_O_OPTIMAL_PLAYER:	
		{
			 Player[CLIENT_PLAYER_INDEX].m_bHidePlayer = uParam;
		}
		break;
		case GPI_O_OPTIMAL_ANI:	
		{
			 Player[CLIENT_PLAYER_INDEX].m_bHideAnimation = uParam;
		}
		break;
		//////////////////////////AUTO PICKUP
		case GPI_P_BPICKUP:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPickItem = uParam;
		}
		break;
		case GPI_P_PICKUPK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_btPickUpKind = uParam;
		}
		break;
		case GPI_P_FILTEQUIP:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFilterEquipment = uParam;
		}
		break;
		case GPI_P_SAVERAP:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_SaveRAP = uParam;
		}
		break;
		case GPI_P_PRICEITEMCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPiceItem = uParam;
		}
		break;
		case GPI_P_PRICEITEM:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nPiceItem = uParam;
		}
		break;
		case GPI_S_LEVELITEMCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bLevelItem = uParam;
		}
		break;
		case GPI_S_LEVELITEM:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nLevelItem = uParam;
		}
		break;
		//////////////////////////AUTO RECOVERY
		case GPI_R_LIFE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_EatLife = uParam;
		}
		break;
		case GPI_R_MANA:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_EatMana = uParam;
		}
		break;
		case GPI_R_EATLIFE:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_LifeTimeUse = uParam;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_LifeAutoV = nParam;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_LifeAutoP = zParam;
		}
		break;
		case GPI_R_EATMANA:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_ManaTimeUse = uParam;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_ManaAutoV = nParam;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_ManaAutoP = zParam;
		}
		break;
		case GPI_R_BTOWNLIF:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPLife = uParam;
		}
		break;
		case GPI_R_BTOWNMAN:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPMana = uParam;
		}
		break;
		case GPI_R_TOWLIFV:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPLifeV = uParam;
		}
		break;
		case GPI_R_TOWMANV:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPManaV = uParam;
		}
		break;
		case GPI_R_NOBLOOD:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPNotMedicineBlood = uParam;
		}
		break;
		case GPI_R_NOMANA:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPNotMedicineMana = uParam;
		}
		break;
		case GPI_R_NOGMON:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPHightMoney = uParam;
		}
		break;
		case GPI_R_NOGMONV:	
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_TPHightMoneyV = uParam;
		}
		break;
		case GPI_R_DURABI:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_btDurabilityItem = uParam;
		}
		break;
		case GPI_R_DURABIV:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_btDurabilityValue = uParam;
		}
		break;
		case GPI_R_NOROOMCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPNotEquipment = uParam;
		}
		break;
		case GPI_R_NOROOM:  
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPNotEquipmentV = uParam;
		}
		break;
		case GPI_R_ANTIPOISON: 
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoAntiPoison = uParam;
		}
		break;
		case GPI_R_USETTL:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoTTL = uParam;
		}
		break;
		case GPI_R_MEDIBAG:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoBoodBag = uParam;
		}
		break;
		case GPI_R_REPAIREQCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bRepairEQuip = uParam;
		}
		break;
		case GPI_R_SAVEMONEY:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bSaveMoney = uParam;
		}
		break;
		case GPI_R_EMEIBUFF:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoLifeReplenish = uParam;
		}
		break;
		case GPI_R_EMEIBUFFNUM:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_AutoLifeReplenishP = uParam;
		}
		break;
		case GPI_R_EATFULLBOX:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_EatFullBox = uParam;
		}
		break;
		case GPI_R_WARNINGPK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nWarningPK = uParam;
		}
		break;
		//////////////////////////AUTO SUPPORT
		case GPI_S_BACKTOMAPCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_BackToTownPor = uParam;
		}
		break;
		case GPI_S_BACKTOMAPEDIT:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTimeSetBack = nParam;
		}
		break;
		case GDI_S_SELL_ITEMCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.bPlayerSellItem = uParam;
		}
		break;
		case GDI_S_SELL_ITEM:
		{
			if (Item[nParam].GetGenre() != item_equip)
				break;
			/*if (Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPiceItem && Item[nParam].GetPrice() >= Player[CLIENT_PLAYER_INDEX].m_cAI.m_nPiceItem)
				break;*/
			if (Player[CLIENT_PLAYER_INDEX].m_cAI.m_SaveRAP && Player[CLIENT_PLAYER_INDEX].m_cAI.IsRAPEquip(Item[nParam].GetDetailType()))
				break;
			if (Item[nParam].GetGenre() == item_goldequip)
				break;
			if (Player[CLIENT_PLAYER_INDEX].m_cAI.IsEquipSatisfyCondition(nParam))
				break;
			if (Item[nParam].GetBindState() > 0)
				break;		
			if (Item[nParam].GetGenre() == item_equip && 
						(Item[nParam].GetDetailType() == equip_horse ||
						 Item[nParam].GetDetailType() == equip_mask))
				break;
			int nPlayerIndex = Player[CLIENT_PLAYER_INDEX].GetPlayerID();			
			int Delay = GetTickCount();
			if (g_SubWorldSet.GetGameTime()% 16 == 0 ) // delay code by kinnox;
			{
				if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
				SendClientCmdSell(Item[nParam].GetID());		
			}

		}
		break;
		case GDI_S_MOVE_ITEMCK:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.bPlayerMoveItem = uParam;
		}
		break;
		case GPI_S_MAP_TRAIN:
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_BackMapTrain = uParam;
		}
		break;

		default:
		nRet = 0;
		break;
	}

	return nRet;
 }
 
BOOL KCoreShell::GetAutoFlag() //Active auto by kinnox;
{
	return Player[CLIENT_PLAYER_INDEX].GetAutoFlag();
}


BOOL KCoreShell::GetSpaceFlag()//CheckSpace auto by kinnox;
{
	return Player[CLIENT_PLAYER_INDEX].m_cAI.GetSpaceFlag();
}

int KCoreShell::FindSkillInfo(int nType, int nIndex)
{
	if(nType == 0)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillState(nIndex);
	else if(nType == 1)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillFight(nIndex);
	else if(nType == 2)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSkillAura(nIndex);
	else if(nType == 3)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSkillRight(nIndex);

	return 0;
}

void KCoreShell::GetSkillName(int nSkillId, char* szSkillName)
{
	if (nSkillId)
	{
		ISkill * pISkill = g_SkillManager.GetSkill(nSkillId, 1);
		strcpy(szSkillName, pISkill->GetSkillName());
	}
}

void KCoreShell::SetMoveMap(int nType, int nPos, int nValue)
{
	switch(nType)
	{
		case GAUTO_AUTO_MOVEMPSID:
			if (nPos >= 0 && nPos < defMAX_AUTO_MOVEMPSL)
			{
				if (nPos == 0)
					Player[CLIENT_PLAYER_INDEX].m_cAI.m_MoveCount = 0;
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_MoveMps[nPos][0] = nValue;
				// FIX: Count all coordinates including index 0
				if (nValue > 0)  // Only count if valid map ID
					Player[CLIENT_PLAYER_INDEX].m_cAI.m_MoveCount++;
			}
			break;
		case GAUTO_AUTO_MOVEMPSX:
			if (nPos >= 0 && nPos < defMAX_AUTO_MOVEMPSL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_MoveMps[nPos][1] = nValue;
			break;
		case GAUTO_AUTO_MOVEMPSY:
			if (nPos >= 0 && nPos < defMAX_AUTO_MOVEMPSL)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_MoveMps[nPos][2] = nValue;
			break;
	}
}

void KCoreShell::SetSortItem(int nType, int nValue, int nPos)
{
	int nIndex = (int)g_MagicDesc.String2MagicID((char*)nType);
	if (nIndex >= magic_skill_begin && nIndex < magic_normal_end)
	{
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_FilterMagic[nPos][0] = nIndex;
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_FilterMagic[nPos][1] = nValue;
	}
	else
	{
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_FilterMagic[nPos][0] = 0;
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_FilterMagic[nPos][1] = 0;
	}
}

// fix xoa nen item questkey by kinnox
int KCoreShell::GetGenreItem(unsigned int uItemId, unsigned int uGenre /* = CGOG_ITEM*/) 
{
	int nIndex = uItemId;
	KItem*	pItem = NULL;
	if (uGenre == CGOG_ITEM)
		return Item[nIndex].GetGenre();
	else if (uGenre == CGOG_NPCSELLITEM)
	{
		int nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
		if (nBuyIdx != -1)
		{
			nIndex = BuySell.GetItemIndex(nBuyIdx, uItemId);
			if (nIndex >= 0)
			{
				pItem = BuySell.GetItem(nIndex);
				return pItem->GetGenre();
			}
		}
	}
	return Item[nIndex].GetGenre();
}
//end code

POINT KCoreShell::GetStatusLockPlayer()//LockPlayer by kinnox;
{
	POINT Pass;
	Pass.x = Player[CLIENT_PLAYER_INDEX].m_bLockPlayer;
	Pass.y = Player[CLIENT_PLAYER_INDEX].m_bPass;
	return Pass;
}

//Lixian by kinnox;
int KCoreShell::GetLixian() 
{
	BOOL nRet = FALSE;
	int nMapID = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode && (nMapID == 1 ||
	nMapID == 11 || nMapID == 53 || nMapID == 162 ||
	nMapID == 176 || nMapID == 80 || nMapID == 78 ||
	nMapID == 37))
		nRet = TRUE;
	if (!nRet)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage,MSG_OFFLINE_MARKET);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}	
	return nRet;
}

//PaintFindPos by kinnox;
BOOL KCoreShell::GetPaintFindPos()
{
	return g_ScenePlace.nPaintFindPos;
}

void KCoreShell::SetPaintFindPos(BOOL nIndex)
{
	g_ScenePlace.nPaintFindPos = nIndex;
	if (!nIndex)
	{
		g_ScenePlace.CalcFlagPos(0, 0, false, false);
	}
}

int	KCoreShell::SceneMapFindPosOperation(unsigned int uOper, unsigned int uParam, int nParam, bool bCalc, bool bLine)
{
	int nRet = 0;
	switch(uOper)
	{
	case GSMOI_PAINT_SCENE_FIND_POS:
		g_ScenePlace.CalcFlagPos(uParam, nParam, bCalc, bLine);
		break;
	}
	return nRet;
}
//

//
int KCoreShell::GetGenreItem(unsigned int uId )
{
	return Item[uId].GetGenre();
}

int KCoreShell::GetDetailItem(unsigned int uId )
{
	return Item[uId].GetDetailType();
}

int KCoreShell::GetParticularItem(unsigned int uId )
{
	return Item[uId].GetParticular();
}

int KCoreShell::GetLevelItem(unsigned int uId )
{
	return Item[uId].GetLevel();
}

int KCoreShell::GetSeriesItem(unsigned int uId )
{
	return Item[uId].GetSeries();
}

int KCoreShell::GetNumStack(unsigned int uId )
{
	return Item[uId].GetStackCount();
}

//