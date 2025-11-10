#include "KCore.h"
#include "KNpc.h"
#include "KMissle.h"
#include "KItem.h"
#include "KBuySell.h"
#include "KPlayer.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KSubWorldSet.h"
#include "scene/KScenePlaceC.h"
#include "ImgRef.h"
#include "GameDataDef.h"
#include "KObjSet.h"

#define  PHYSICSSKILLICON "\\spr\\Ui\\¼¼ÄÜÍ¼±ê\\icon_sk_ty_ap.spr"

#define		SHOW_SPACE_HEIGHT			5
#define		SHOW_RANK_HEIGHT			12
#define		SKILL_CELL_WIDTH	36
#define		SKILL_CELL_HEIGHT	36


void	CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam)
{
	switch(uObjGenre)
	{
	case CGOG_NPC:
		if (uId > 0)
		{
			if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			{
				int nHeight = Npc[uId].GetNpcPate();

				Npc[uId].PaintBlood(nHeight / 2);
				
		
				if(Npc[uId].m_Kind == kind_player)
				{
					if(Npc[uId].m_bOpenShop)
					{
						Npc[uId].PaintShopInfo(12, nHeight);
					}
				}
		

				int nnHeight = nHeight;
				nHeight = Npc[uId].PaintChat(nnHeight);

				if (nHeight == nnHeight)
				{	
					/*
					if (NpcSet.CheckShowMana() && uId == Player[CLIENT_PLAYER_INDEX].m_nIndex)
					{
						nHeight = Npc[uId].PaintMana(nnHeight);
					}
					*/

					if (NpcSet.CheckShowLife())
					{	
						//if(uId != Player[CLIENT_PLAYER_INDEX].m_nIndex)
						{
							nHeight += 3;
						}
						Npc[uId].PaintWarning(nHeight, false);//AutoAI by kinnox;
						nHeight = Npc[uId].PaintLife(nHeight, false);
					}

					

					if(Npc[uId].m_Kind == kind_normal)
					{
						nHeight = Npc[uId].PaintLife(nHeight, true);
					}

					if (NpcSet.CheckShowName() && Npc[uId].m_Kind != kind_normal)
					{
						if (nnHeight != nHeight)
						{
							nHeight += SHOW_SPACE_HEIGHT;
						}

						if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc() && Player[CLIENT_PLAYER_INDEX].GetTargetNpc() == uId)
						{
							nHeight = Npc[uId].PaintInfo(nHeight, false, 14);
							nHeight -= SHOW_RANK_HEIGHT;
							Npc[uId].PaintRankInfo(nHeight, 14);
						}
						else
						{
							nHeight = Npc[uId].PaintInfo(nHeight, false);
							nHeight -= SHOW_RANK_HEIGHT;
							Npc[uId].PaintRankInfo(nHeight);
						}
					}

					if(Npc[uId].m_Kind == kind_normal)
					{	
						
						if (nnHeight != nHeight)
						{
							nHeight += SHOW_SPACE_HEIGHT;
						}

						if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc() && Player[CLIENT_PLAYER_INDEX].GetTargetNpc() == uId)
						{
							nHeight = Npc[uId].PaintInfo(nHeight, true, 14);
						}
						else
						{
							nHeight = Npc[uId].PaintInfo(nHeight, true);
						}
					}
				}
			}
			else if ((nParam & IPOT_RL_OBJECT) == IPOT_RL_OBJECT)
			{
				Npc[uId].Paint();
				if ((int)uId == Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
				{
					Npc[uId].DrawBorder();
				}
			}			
		}
		break;
	case CGOG_MISSLE:
		if (uId > 0)
			Missle[uId].Paint();
		break;
	case CGOG_PLAYERSELLITEM:
	case CGOG_IME_ITEM:
	case CGOG_ITEM:
		if (uId == 0)
			break;

		if (Width == 0 && Height == 0)
		{
#define	ITEM_CELL_WIDTH		26
#define	ITEM_CELL_HEIGHT	26
			Width = Item[uId].GetWidth() * ITEM_CELL_WIDTH;
			Height = Item[uId].GetHeight() * ITEM_CELL_HEIGHT;
			x -= Width / 2;
			y -= Height / 2;
		}
		else
		{
			x += (Width - Item[uId].GetWidth() * ITEM_CELL_WIDTH) / 2;
			y += (Height - Item[uId].GetHeight() * ITEM_CELL_HEIGHT) / 2;
		}

		if (uObjGenre == CGOG_IME_ITEM)
		{
			if (uId < 0)
			{
				ISkill *pISkill = g_SkillManager.GetSkill(abs(uId), 1);
				if (pISkill)
				{
					pISkill->DrawSkillIcon(uId, x, y, Width, Height);
				}
			}
			else
				Item[uId].Paint(x, y, false);
		}
		else
		{
			if (uId < 0)
			{
				ISkill *pISkill = g_SkillManager.GetSkill(abs(uId), 1);
				if (pISkill)
				{
					pISkill->DrawSkillIcon(uId, x, y, Width, Height);
				}
			}
			else
				Item[uId].Paint(x, y, TRUE);
		}

		break;
	case CGOG_MENU_NPC:
		if (nParam)
		{
			((KNpcRes *)nParam)->SetPos(0, x + Width / 2, y + Height / 2 + 28, 0, FALSE, TRUE);
#define		STAND_TOTAL_FRAME	15
			int nFrame = g_SubWorldSet.m_nLoopRate % STAND_TOTAL_FRAME;
			((KNpcRes *)nParam)->Draw(0, 0, STAND_TOTAL_FRAME, nFrame, true);
		}
		break;
	
	case CGOG_NPC_BLUR_DETAIL(1):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(1);
		break;
	case CGOG_NPC_BLUR_DETAIL(2):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(2);
		break;
	case CGOG_NPC_BLUR_DETAIL(3):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(3);
		break;
	case CGOG_NPC_BLUR_DETAIL(4):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(4);
		break;
	case CGOG_NPC_BLUR_DETAIL(5):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(5);
		break;
	case CGOG_NPC_BLUR_DETAIL(6):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(6);
		break;
	case CGOG_NPC_BLUR_DETAIL(7):
		Npc[uId].GetNpcRes()->m_cNpcBlur.Draw(7);
		break;
	case CGOG_SKILL:
	case CGOG_SKILL_FIGHT:
	case CGOG_SKILL_LIVE:
	case CGOG_SKILL_CUSTOM:
	case CGOG_SKILL_SHORTCUT:
		{
			int nSkillId = (int)uId;
			if (nSkillId > 0)
			{
				int i = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(nSkillId);
				ISkill *pISkill = g_SkillManager.GetSkill(nSkillId, 1);
				if (pISkill)
				{
					if (uObjGenre == CGOG_SKILL_CUSTOM)
					{
						if (Width == 1)
							x -= SKILL_CELL_WIDTH / 2;
						if (Height == 1)
							y -= SKILL_CELL_HEIGHT / 2;
					}
					pISkill->DrawSkillIcon(nSkillId, x, y, Width, Height);
				}

				if ((uObjGenre == CGOG_SKILL_CUSTOM || uObjGenre == CGOG_SKILL_SHORTCUT) && 
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].SkillLevel <= 0)
				{
					KRUShadow	Shadow;
					Shadow.Color.Color_dw = 0x14000000;
					Shadow.oPosition.nX = x;
					Shadow.oPosition.nY = y;
					Shadow.oEndPos.nX = x + Width;
					Shadow.oEndPos.nY = y + Height;
					g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
				}
				else if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].MaxCastTimes > 0 && 
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].MaxCastTimes > SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_dwCurrentTime)
				{
					int nPercent = (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].NextCastTime - SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_dwCurrentTime) * 100 / Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].NextCastTime;
					KRUShadow	Shadow;
					Shadow.Color.Color_dw = 0x14000000;
					Shadow.oPosition.nX = x;
					Shadow.oPosition.nY = y;
					Shadow.oEndPos.nX = x + Width;
					Shadow.oEndPos.nY = y + Height * (MAX_PERCENT - nPercent) / MAX_PERCENT;
					g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
					Shadow.Color.Color_dw = 0x16ffff80;
					Shadow.oPosition.nX = x;
					Shadow.oPosition.nY = y + (Height - Height * nPercent / MAX_PERCENT);
					Shadow.oEndPos.nX = x + Width;
					Shadow.oEndPos.nY = y + Height;
					g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
				}
			}
			else
			{
				if(uId == -1) 
				{
					KRUImage RUIconImage;
					RUIconImage.nType = ISI_T_SPR;
					RUIconImage.Color.Color_b.a = 255;
					RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUIconImage.uImage = 0;
					RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
					RUIconImage.bRenderFlag = 0;
					strcpy(RUIconImage.szImage, PHYSICSSKILLICON);
					RUIconImage.oPosition.nX = x;
					RUIconImage.oPosition.nY = y;
					RUIconImage.oPosition.nZ = 0;
					RUIconImage.nFrame = 0;
					g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);
				}
			}
		}
		break;
	case CGOG_OBJECT:
		if (uId)
		{
			if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			{
				if (ObjSet.CheckShowName())
					Object[uId].DrawInfo();
			}
			else //if ((nParam & IPOT_RL_COVER_GROUND) == IPOT_RL_COVER_GROUND)
			{
				Object[uId].Draw();
				if ((int)uId == Player[CLIENT_PLAYER_INDEX].GetTargetObj())
					Object[uId].DrawBorder();
			}
		}
		break;
	case CGOG_NPCSELLITEM:
		if (uId < 0)
			break;
		BuySell.PaintItem(uId, x, y);
		break;
	default:
		break;
	}
}

void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo)
{
	switch (uObjGenre)
	{
	case CGOG_NPC:
		if (uId > 0 && uId < MAX_NPC)
		{
			Npc[uId].GetMpsPos(&pLightInfo->oPosition.nX, &pLightInfo->oPosition.nY);
			pLightInfo->oPosition.nZ = Npc[uId].m_Height;
			pLightInfo->dwColor = 0;
			if (Npc[uId].m_RedLum > 255)
			{
				Npc[uId].m_RedLum = 255;
			}
			if (Npc[uId].m_GreenLum > 255)
			{
				Npc[uId].m_GreenLum = 255;
			}
			if (Npc[uId].m_BlueLum > 255)
			{
				Npc[uId].m_BlueLum = 255;
			}
			pLightInfo->dwColor = 0xff000000 | Npc[uId].m_RedLum << 16 | Npc[uId].m_GreenLum << 8 | Npc[uId].m_BlueLum;
			pLightInfo->nRadius = Npc[uId].m_CurrentVisionRadius;
		}
		break;
	case CGOG_OBJECT:
		break;
	case CGOG_MISSLE:
		if (uId > 0 && uId < MAX_MISSLE)
		{
			if (Missle[uId].m_nMissleId > 0)
			{
				Missle[uId].GetLightInfo(pLightInfo);
			}
		}
		break;
	default:
		break;
	}
}