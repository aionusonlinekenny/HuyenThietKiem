/*****************************************************************************************
//	界面窗口体系结构// --容纳游戏对象的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-25
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "WndObjContainer.h"
#include "../Elem/MouseHover.h"
#include "../Elem/SpaceOver.h"
#include "../Elem/LightLoop.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/CoreShell.h"
#include "../../../core/src/GameDataDef.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Engine/Src/Text.h"
#include "../UiCase/UiPlayerBar.h"
#include "../UiCase/UiStoreBox.h"
#include "../UiCase/UiItem.h"
#include "KEngine.h"
#include "../UiBase.h"
extern iCoreShell*	g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;


unsigned int l_BgColors[] =
{
	0x0a001e13,//dark green
	0x0a2c0000,//dark red
	0x0a654915,//dark yellow		
	0x0a000636,//dark blue		
	0x0a0000ff,//blue
	0x0af6facb,//yellow
};

void WndObjContainerInit(KIniFile* pIni)
{
#define	COLOR_SECTION	"ObjContColor"
	if (pIni)
	{
		char	Buff[16];
		int		nAlpha;
		pIni->GetInteger(COLOR_SECTION, "Alpha", 0, &nAlpha);
		if (nAlpha < 0)
			nAlpha = 0;
		else if (nAlpha > 255)
			nAlpha = 255;
		nAlpha = ((nAlpha << 21) & 0xff000000);
		pIni->GetString(COLOR_SECTION, "NormalColor", "", Buff, sizeof(Buff));
		l_BgColors[0] = (GetColor(Buff) & 0xffffff) | nAlpha;
		pIni->GetString(COLOR_SECTION, "NotUseableColor", "", Buff, sizeof(Buff));
		l_BgColors[1] = (GetColor(Buff) & 0xffffff) | nAlpha;
		pIni->GetString(COLOR_SECTION, "SpecialColor", "", Buff, sizeof(Buff));
		l_BgColors[2] = (GetColor(Buff) & 0xffffff) | nAlpha;
		pIni->GetString(COLOR_SECTION, "MouseOverColor", "", Buff, sizeof(Buff));
		l_BgColors[3] = (GetColor(Buff) & 0xffffff) | nAlpha;
		pIni->GetString(COLOR_SECTION, "PutdownColor", "", Buff, sizeof(Buff));
		l_BgColors[4] = (GetColor(Buff) & 0xffffff) | nAlpha;
		pIni->GetString(COLOR_SECTION, "PriceMarkedColor", "", Buff, sizeof(Buff));
		l_BgColors[5] = (GetColor(Buff) & 0xffffff) | nAlpha;
	}
}


KWndObjectBox::KWndObjectBox()
{
	m_uAcceptableGenre = CGOG_NOTHING;
	m_Object.uGenre = CGOG_NOTHING;
	m_Object.DataX = 0;
	m_Object.DataY = 0;
	m_nContainerId = 0;
	m_Object.m_nPos = 0;
	m_Object.m_nDir = 0;
	m_Object.m_nLastFrame = 0;
	m_ulTimeDelay = 0;
	m_nFrame = 0;
	m_bCountFrame = 0;
}

void KWndObjectBox::SetContainerId(int nId)
{
	m_nContainerId = nId;
}


void KWndObjectBox::SetObjectGenre(unsigned int uGenre)
{
	m_uAcceptableGenre = uGenre;
}


void KWndObjectBox::Clear()
{
	m_Object.uGenre = CGOG_NOTHING;
}


int KWndObjectBox::GetObject(KUiDraggedObject& Obj) const
{
	if ((Obj.uGenre = m_Object.uGenre) != CGOG_NOTHING)
	{
		Obj = m_Object;
		return true;
	}
	return false;
}

void KWndObjectBox::HoldObject(unsigned int uGenre, unsigned int uId, int nDataW, int nDataH)
{
	m_Object.uGenre = uGenre;
	m_Object.uId = uId;
	m_Object.DataW = nDataW;
	m_Object.DataH = nDataH;

	if (g_MouseOver.IsMoseHoverWndObj(this, 0))
	{
		if (m_Object.uGenre != CGOG_NOTHING)
		{
			int x, y;
			Wnd_GetCursorPos(&x, &y);
			SetMouseHoverObjectDesc(this, 0, m_Object.uGenre,
				m_Object.uId, m_nContainerId, x, y);
		}
		else
		{
			g_MouseOver.CancelMouseHoverInfo();
		}
	}	
}

void KWndObjectBox::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (m_Object.uGenre != CGOG_NOTHING && g_pRepresentShell)
	{
		KRUShadow	Shadow;
		Shadow.Color.Color_dw = 0;
		if (m_Style & OBJCONT_F_MOUSE_HOVER)
			Shadow.Color.Color_dw = l_BgColors[3];
		else if (m_Style & OBJCONT_S_HAVEOBJBGCOLOR)
		{
			KUiObjAtContRegion	Obj;
			Obj.Obj.uGenre = m_Object.uGenre;
			Obj.Obj.uId = m_Object.uId;
			Obj.Region.h = Obj.Region.v = 0;
			Obj.Region.Width = Obj.Region.Height = 0;
			Obj.nContainer = m_nContainerId;

			ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
				GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
			// if (eProp == IIEP_NORMAL)
				// Shadow.Color.Color_dw = l_BgColors[0];
			// else if (eProp == IIEP_NOT_USEABLE)
				// Shadow.Color.Color_dw = l_BgColors[1];
			// else if (eProp == IIEP_SPECIAL)
				// Shadow.Color.Color_dw = l_BgColors[2];
				if (g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_equip||
					g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_goldequip)
					{		
						if (eProp == IIEP_NORMAL)
							Shadow.Color.Color_dw = l_BgColors[0];
						else if (eProp == IIEP_NOT_USEABLE)
							Shadow.Color.Color_dw = l_BgColors[1];
						else if (eProp == IIEP_SPECIAL)
							Shadow.Color.Color_dw = l_BgColors[2];
					}
		}
		if (Shadow.Color.Color_dw)
		{
			Shadow.oPosition.nX = m_nAbsoluteLeft;
			Shadow.oPosition.nY = m_nAbsoluteTop;
			Shadow.oEndPos.nX = m_nAbsoluteLeft + m_Width;
			Shadow.oEndPos.nY = m_nAbsoluteTop + m_Height;
			g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
		}
		g_pCoreShell->DrawGameObj(m_Object.uGenre, m_Object.uId,
			m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height, 0);
		if(m_Object.uGenre == CGOG_ITEM)
		{
			KUiObjAtContRegion	Obj;
			Obj.Obj.uGenre = m_Object.uGenre;
			Obj.Obj.uId = m_Object.uId;
			Obj.Region.h = 0;
			Obj.Region.v = 0;
			Obj.Region.Width  = m_Object.DataW;
			Obj.Region.Height = m_Object.DataH;
			Obj.nContainer = m_nContainerId;
			
			int nType = g_pCoreShell->GetGameData(GDI_ITEM_GENRE, (unsigned int)&Obj, 0);

			if (g_pCoreShell->GetGameData(GDI_ITEM_DETAIL, 0, (int)&Obj) == 12 && g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) != item_goldequip &&
				g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_equip) 	
				nType = gold_item;//Draw border Item_Mantle by kinnox;
				
			if( (nType != 0) && (nType < num_item) )
			{
				DrawBorder(m_bCountFrame, m_nFrame, m_ulTimeDelay, m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height, nType); 
				DrawLight(m_Object.m_nPos, m_Object.m_nDir, m_Object.m_nLastFrame,
				m_nAbsoluteLeft - 1, m_nAbsoluteTop - 1, m_Width, m_Height, ((m_Width - 13) + (m_Height - 13)) / 2, nType);
			}
		}
	}
}

void KWndObjectBox::Clone(KWndObjectBox* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_uAcceptableGenre = m_uAcceptableGenre;
	}
}

int	KWndObjectBox::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		int nValue;
		pIniFile->GetInteger(pSection, "EnableClickEmpty", 0, &nValue);
		if (nValue)
			m_Style |= OBJCONT_S_ENABLE_CLICK_EMPTY;
		else
			m_Style &= ~OBJCONT_S_ENABLE_CLICK_EMPTY;

		pIniFile->GetInteger(pSection, "HaveBgColor", 1, &nValue);
		if (nValue)
			m_Style |= OBJCONT_S_HAVEOBJBGCOLOR;
		else
			m_Style &= ~OBJCONT_S_HAVEOBJBGCOLOR;
		return true;
	}
	return false;
}

void KWndObjectBox::EnablePickPut(bool bEnable)
{
	if (bEnable == false)
		m_Style |= OBJCONT_S_DISABLE_PICKPUT;
	else
		m_Style &= ~OBJCONT_S_DISABLE_PICKPUT;
}


int KWndObjectBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	// DEBUG: Log WM_MOUSEMOVE messages - SAFE version without member access
	if (uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEHOVER)
	{
		FILE* pLog = fopen("mask_tooltip_debug.log", "a");
		if(pLog)
		{
			fprintf(pLog, "[WNDBOX-WNDPROC-SAFE] uMsg=%s, this=%p\n",
				   (uMsg == WM_MOUSEMOVE) ? "WM_MOUSEMOVE" : "WM_MOUSEHOVER", this);
			fclose(pLog);
		}

		// Try to log member variables (may crash if corrupted)
		pLog = fopen("mask_tooltip_debug.log", "a");
		if(pLog)
		{
			fprintf(pLog, "[WNDBOX-WNDPROC] m_Object.uGenre=%d, m_Object.uId=%d\n",
				   m_Object.uGenre, m_Object.uId);
			fclose(pLog);
		}
	}

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		if (m_pParentWnd)
		{
			if ((m_Style & OBJCONT_S_DISABLE_PICKPUT) == 0)
			{
				//------------------------------------------------
				if(m_nContainerId == UOC_EQUIPTMENT)
				{
					if(GetKeyState(VK_CONTROL) & 0x8000)
					{
						KUiDraggedObject	Obj;
						Obj = m_Object;
						KUiObjAtRegion CItem;
						CItem.Obj.uGenre = Obj.uGenre;
						CItem.Obj.uId = Obj.uId;
						CItem.Region.Width = CItem.Region.Height = 0;
						CItem.Region.h = Obj.DataX;
						CItem.Region.v =Obj.DataY;

						KUiChatItemParam pInfo = { 0 };
						if (g_pCoreShell->GetGameData(GDI_CHAT_ITEM_INFO,
						(unsigned int)&CItem, (unsigned int)&pInfo))
						{
						KUiPlayerBar::SetChatItemInfo(pInfo, CItem.Obj.uId);
						}
						break;
					}
				}
				//------------------------------------------------


				if (Wnd_GetDragObj(NULL))
				{
					DropObject(false);
				}
				else if (m_Object.uGenre != CGOG_NOTHING)
				{
					ITEM_PICKDROP_PLACE	Pick;
					Pick.pWnd = this;
					Pick.h = 0;
					Pick.v = 0;
					m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP,
						(unsigned int)&Pick, NULL);
				}
			}
			else if (m_Object.uGenre != CGOG_NOTHING)
			{
				KUiDraggedObject	Obj;
				Obj = m_Object;
				m_pParentWnd->WndProc(WND_N_LEFT_CLICK_ITEM,
					(unsigned int)&Obj, (int)(KWndWindow*)this);
			}
			else if (m_Style & OBJCONT_S_ENABLE_CLICK_EMPTY)
			{
				m_pParentWnd->WndProc(WND_N_LEFT_CLICK_ITEM,
					NULL, (int)(KWndWindow*)this);
			}				
		}
		break;
		/*
	case WM_RBUTTONDOWN:
		if (m_pParentWnd)
		{
			if (m_Object.uGenre != CGOG_NOTHING)
			{
				KUiDraggedObject	Obj;
				Obj = m_Object;
				if ( (g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) >= item_equip && g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) <= item_platinaequip))
				{
					KUiObjAtRegion Info;
					Info.Obj.uGenre = Obj.uGenre;
					Info.Obj.uId = Obj.uId;
					Info.Region.Width = Info.Region.Height = 0;
					Info.Region.h = Obj.DataX;
					Info.Region.v =Obj.DataY;
					g_pCoreShell->OperationRequest(GOI_USE_ITEM,(unsigned int)(&Info), UOC_EQUIPTMENT);
				}
				else
				{
					m_pParentWnd->WndProc(WND_N_RIGHT_CLICK_ITEM,
					(unsigned int)&m_Object, (int)(KWndWindow*)this);
				}
			}
			else if (m_Style & OBJCONT_S_ENABLE_CLICK_EMPTY)
			{
				m_pParentWnd->WndProc(WND_N_LEFT_CLICK_ITEM,
					NULL, (int)(KWndWindow*)this);
			}
		}
		break;*/
	case WM_RBUTTONDOWN:
    if (m_pParentWnd)
    {
        if (m_Object.uGenre != CGOG_NOTHING)
        {
            KUiDraggedObject Obj = m_Object;

            if (KUiStoreBox::GetIfVisible() != NULL)
            {
                // Ph鈔 bi?t theo container hi?n t?i
                int cont = m_nContainerId;
                // UOC_STORE_BOX..UOC_STORE_BOX5, UOC_EXPAND_BOX1 ... t鵼 game c?a b?n
                BOOL inStore =
                    (cont == UOC_STORE_BOX)  || (cont == UOC_STORE_BOX1) ||
                    (cont == UOC_STORE_BOX2) || (cont == UOC_STORE_BOX3) ||
                    (cont == UOC_STORE_BOX4) || (cont == UOC_STORE_BOX5) ||
                    (cont == UOC_EXPAND_BOX1);

                if (inStore)
                    KUiItem::MoveItemToInventory(&Obj);   // r??ng -> h鄋h trang
                else
                    KUiItem::MoveItemToRepository(&Obj);   // h鄋h trang -> r??ng
            }
            else
            {
                // Kh鬾g m? StoreBox -> logic c?
                if (g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) >= item_equip &&
                    g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) <= item_platinaequip)
                {
                    KUiObjAtRegion Info;
                    Info.Obj.uGenre    = Obj.uGenre;
                    Info.Obj.uId       = Obj.uId;
                    Info.Region.Width  = 0;
                    Info.Region.Height = 0;
                    Info.Region.h      = Obj.DataX;
                    Info.Region.v      = Obj.DataY;

                    g_pCoreShell->OperationRequest(GOI_USE_ITEM, (unsigned int)&Info, UOC_EQUIPTMENT);
                }
                else
                {
                    g_pCoreShell->OperationRequest(GOI_USE_ITEM, (unsigned int)&Obj, m_nContainerId);
                }
            }
        }
        else if (m_Style & OBJCONT_S_ENABLE_CLICK_EMPTY)
        {
            m_pParentWnd->WndProc(WND_N_LEFT_CLICK_ITEM, NULL, (int)(KWndWindow*)this);
        }
    }
    break;


	case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
		// DEBUG: Log when mouse moves over object box
		{
			FILE* pLog = fopen("mask_tooltip_debug.log", "a");
			if(pLog)
			{
				fprintf(pLog, "[WNDOBJ-MOUSEMOVE] uGenre=%d, uId=%d, ContainerId=%d\n",
					   m_Object.uGenre, m_Object.uId, m_nContainerId);
				fclose(pLog);
			}
		}
		m_Style |= OBJCONT_F_MOUSE_HOVER;
		if (m_Object.uGenre != CGOG_NOTHING && g_MouseOver.IsMoseHoverWndObj(this, 0) == 0)
{
			FILE* pLog = fopen("mask_tooltip_debug.log", "a");
			if(pLog)
			{
				fprintf(pLog, "[WNDOBJ-CALLING] About to call SetMouseHoverObjectDesc for Genre=%d, ID=%d\n",
					   m_Object.uGenre, m_Object.uId);
				fclose(pLog);
			}
			SetMouseHoverObjectDesc(this, 0, m_Object.uGenre,
				m_Object.uId, m_nContainerId, LOWORD(nParam), HIWORD(nParam));
		}
		else
		{
			FILE* pLog = fopen("mask_tooltip_debug.log", "a");
			if(pLog)
			{
				fprintf(pLog, "[WNDOBJ-SKIP] NOT calling SetMouseHoverObjectDesc - Genre=%d or IsMoseHoverWndObj failed\n",
					   m_Object.uGenre);
				fclose(pLog);
			}
}
		break;
	case WND_M_MOUSE_LEAVE:
		m_Style &= ~OBJCONT_F_MOUSE_HOVER;
		KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}


int KWndObjectBox::DropObject(bool bTestOnly)
{
	KUiDraggedObject	DragObj;
	Wnd_GetDragObj(&DragObj);

	if (m_uAcceptableGenre != CGOG_NOTHING && DragObj.uGenre != CGOG_SKILL_CUSTOM)
	{
		if ((m_uAcceptableGenre != DragObj.uGenre) &&
			((m_uAcceptableGenre & 0xFFFF) != (DragObj.uGenre & 0xFFFF)) || ((m_uAcceptableGenre & 0xFFFF0000) != 0))
		return false;
	}
	if (bTestOnly)
		return true;
	
	ITEM_PICKDROP_PLACE	Pick, Drop;
	Drop.pWnd = this;
	Drop.h = Drop.v = 0;
	if (m_Object.uGenre == CGOG_NOTHING)
		m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP, NULL, (int)&Drop);
	else
	{
		Pick.pWnd = this;
		Pick.h = 0;
		Pick.v = 0;
		m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP, (unsigned int)&Pick, (int)&Drop);
	}		
	return true;
}

#define	NO_MATCHED_PUT_POS			-761209
#define	REPLACE_ITEM_POS(iItem)		(-(iItem) - 1)
#define	REPLACE_ITEM_INDEX(iPos)	((-iPos) - 1)


KWndObjectMatrix::KWndObjectMatrix()
{
	m_nNumUnitHori = 1;
	m_nNUmUnitVert = 1;
	m_nUnitWidth = 1;
	m_nUnitHeight = 1;
	m_nNumObjects = 0;
	m_pObjects = NULL;
	m_nMouseOverObj = -1;
	m_nPutPosX = NO_MATCHED_PUT_POS;
	m_nContainerId = 0;
	m_ulTimeDelay = 0;
	m_nFrame = 0;
	m_bCountFrame = 0;
}

void KWndObjectMatrix::SetContainerId(int nId)
{
	m_nContainerId = nId;
}

KWndObjectMatrix::~KWndObjectMatrix()
{
	Clear();
}

void KWndObjectMatrix::Clone(KWndObjectMatrix* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_nNumUnitHori = m_nNumUnitHori;
		pCopy->m_nNUmUnitVert = m_nNUmUnitVert;
		pCopy->m_nUnitWidth  = m_nUnitWidth;
		pCopy->m_nUnitHeight = m_nUnitHeight;
	}
}


//

int KWndObjectMatrix::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "HUnits", 1, &m_nNumUnitHori);
		pIniFile->GetInteger(pSection, "VUnits", 1, &m_nNUmUnitVert);
		if (m_nNumUnitHori < 1)
			m_nNumUnitHori = 1;
		if (m_nNUmUnitVert < 1)
			m_nNUmUnitVert = 1;
		m_nUnitWidth = m_Width / m_nNumUnitHori;
		if (m_nUnitWidth < 1)
			m_nUnitWidth = 1;
		m_nUnitHeight = m_Height / m_nNUmUnitVert;
		if (m_nUnitHeight < 1)
			m_nUnitHeight = 1;
		int nValue;
		pIniFile->GetInteger(pSection, "HaveBgColor", 1, &nValue);
		if (nValue)
			m_Style |= OBJCONT_S_HAVEOBJBGCOLOR;
		else
			m_Style &= ~OBJCONT_S_HAVEOBJBGCOLOR;
		pIniFile->GetInteger(pSection, "AcceptFree", 0, &nValue);
		if (nValue)
			m_Style |= OBJCONT_S_ACCEPT_FREE;
		else
			m_Style &= ~OBJCONT_S_ACCEPT_FREE;
		pIniFile->GetInteger(pSection, "UnitBorder", 0, &m_nUnitBorder);
		if (m_nUnitBorder >= m_nUnitWidth)
			m_nUnitBorder = m_nUnitWidth - 1;
		if (m_nUnitBorder >= m_nUnitHeight)
			m_nUnitBorder = m_nUnitHeight - 1;
		if (m_nUnitBorder < 0)
			m_nUnitBorder = 0;

		m_nPutPosX = NO_MATCHED_PUT_POS;
		return true;
	}
	return false;
}


//

void KWndObjectMatrix::PaintWindow()
{
	KWndWindow::PaintWindow();
	KRUShadow	Shadow;
	for (int i = 0; i < m_nNumObjects; i++)
	{
		KUiDraggedObject* pObj = &m_pObjects[i];
		Shadow.Color.Color_dw = 0;
		if (i == REPLACE_ITEM_INDEX(m_nPutPosX))
			Shadow.Color.Color_dw = l_BgColors[4];
		else if ((m_Style & OBJCONT_F_MOUSE_HOVER) && m_nMouseOverObj == i)
			Shadow.Color.Color_dw = l_BgColors[3];
		else if (m_Style & OBJCONT_S_HAVEOBJBGCOLOR)
		{
			KUiObjAtContRegion	Obj;
			Obj.Obj.uGenre = pObj->uGenre;
			Obj.Obj.uId = pObj->uId;
			Obj.Region.h = Obj.Region.v = 0;
			Obj.Region.Width = Obj.Region.Height = 0;
			Obj.nContainer = m_nContainerId;
			ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
				GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (unsigned int)&Obj, 0);
			if(g_UiBase.GetStatus() == UIS_S_MARKPRICE_ITEM)
			{
				if (eProp == IIEP_PRICED)
					Shadow.Color.Color_dw = l_BgColors[5];
				else
					Shadow.Color.Color_dw = 0;
			}
			else
			{	
				// int nRetItemNormal	= 0; // item thuong ( bao gom item xanh va questkey)
				// int nRetItemPurple	= 1; // item tim
				// int nRetItemGold 	= 2; // item gold
				if (g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_equip||
					g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_goldequip) 
				{
					if (eProp == IIEP_NORMAL && 
					(pObj->uGenre == CGOG_ITEM|| pObj->uGenre == CGOG_NPCSELLITEM || pObj->uGenre == CGOG_PLAYERSELLITEM ))
						Shadow.Color.Color_dw = l_BgColors[0];
					else if (eProp == IIEP_NOT_USEABLE)
						Shadow.Color.Color_dw = l_BgColors[1];
					else if (eProp == IIEP_SPECIAL && g_UiBase.GetStatus() != IIEP_PRICED)
						Shadow.Color.Color_dw = l_BgColors[2];
					else if (eProp == IIEP_PRICED)
						Shadow.Color.Color_dw = l_BgColors[5];

				}
				else
				{
					if (eProp == IIEP_PRICED)
					{
						if (pObj->uGenre == CGOG_ITEM)
						Shadow.Color.Color_dw = l_BgColors[5];
					}
					else
						Shadow.Color.Color_dw = 0;
				}
			}
		}

		int width = m_nUnitWidth * pObj->DataW - m_nUnitBorder * 2;
		int height = m_nUnitHeight * pObj->DataH - m_nUnitBorder * 2;
		Shadow.oPosition.nX = m_nAbsoluteLeft + m_nUnitWidth * pObj->DataX + m_nUnitBorder;
		Shadow.oPosition.nY = m_nAbsoluteTop + m_nUnitHeight * pObj->DataY + m_nUnitBorder;
		if (Shadow.Color.Color_dw)
		{
			Shadow.oEndPos.nX = Shadow.oPosition.nX + width;
			Shadow.oEndPos.nY = Shadow.oPosition.nY + height;
			g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
		}

		g_pCoreShell->DrawGameObj(pObj->uGenre, pObj->uId,
			Shadow.oPosition.nX, Shadow.oPosition.nY, width, height, 0);

		if(pObj->uGenre == CGOG_ITEM || pObj->uGenre == CGOG_NPCSELLITEM ||
			pObj->uGenre == CGOG_PLAYERSELLITEM)
		{
			KUiObjAtContRegion	Obj;
			Obj.Obj.uGenre = pObj->uGenre;
			Obj.Obj.uId = pObj->uId;
			Obj.Region.h = pObj->DataX;
			Obj.Region.v = pObj->DataY;
			Obj.Region.Width  = pObj->DataW;
			Obj.Region.Height = pObj->DataH;
			
			int nType = g_pCoreShell->GetGameData(GDI_ITEM_GENRE, (unsigned int)&Obj, 1);

			if (g_pCoreShell->GetGameData(GDI_ITEM_DETAIL, 0, (int)&Obj) == 12 && g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) != item_goldequip &&
				g_pCoreShell->GetGenreItem(Obj.Obj.uId, Obj.Obj.uGenre) == item_equip) 		
				nType = gold_item; //Draw border Item_Mantle by kinnox;				

			if( (nType != 0) && (nType < num_item) )
			{
				if((m_Style & OBJCONT_F_MOUSE_HOVER) && (m_nMouseOverObj == i))
				{
					/* Cho nay la Disable vong sang by kinnox*/
				}
				else
				{
				DrawBorder(m_bCountFrame, m_nFrame, m_ulTimeDelay, Shadow.oPosition.nX, Shadow.oPosition.nY, width, height, nType); 
				DrawLight(pObj->m_nPos, pObj->m_nDir, pObj->m_nLastFrame,
				Shadow.oPosition.nX, Shadow.oPosition.nY, width, height, ((width - 12) + (height - 12)) / 2, nType);
				}
			}
		}
	}

	if (m_nPutPosX >= 0)
	{
		Shadow.oPosition.nX = m_nAbsoluteLeft + m_nUnitWidth * m_nPutPosX + m_nUnitBorder;
		Shadow.oPosition.nY = m_nAbsoluteTop + m_nUnitHeight * m_nPutPosY + m_nUnitBorder;
		Shadow.oEndPos.nX = Shadow.oPosition.nX + m_nUnitWidth * m_nPutWidth - m_nUnitBorder * 2;
		Shadow.oEndPos.nY = Shadow.oPosition.nY + m_nUnitHeight * m_nPutHeight - m_nUnitBorder * 2;
		Shadow.Color.Color_dw = l_BgColors[4];
		g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);
	}
}


//

int KWndObjectMatrix::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (pObject && nCount > 0)
	{
		int	i, nValidCount = 0;
		for (i = 0; i < nCount; i++)
		{
			if (pObject[i].uGenre)
				nValidCount++;
		}
		if (nValidCount)
		{
			KUiDraggedObject* pNewList = (KUiDraggedObject*)realloc(m_pObjects, sizeof(KUiDraggedObject) * (m_nNumObjects + nValidCount));
			if (pNewList)
			{
				m_pObjects = pNewList;
				for (i = 0; i < nCount; i++)
				{
					if (pObject[i].uGenre)
					{
						m_pObjects[m_nNumObjects] = pObject[i];
						m_pObjects[m_nNumObjects].m_nPos = 0;
						m_pObjects[m_nNumObjects].m_nDir = 0;
						m_pObjects[m_nNumObjects].m_nLastFrame = 0;
						m_nNumObjects ++;
					}
				}
			}
			m_nPutPosX = NO_MATCHED_PUT_POS;
			return true;
		}
	}
	return false;
}


//

int KWndObjectMatrix::RemoveObject(KUiDraggedObject* pObject)
{
	if (pObject)
	{
		m_nPutPosX = NO_MATCHED_PUT_POS;
		m_nMouseOverObj = -1;
		for (int i = 0; i < m_nNumObjects; i++)
		{
			KUiDraggedObject* pHolded = &m_pObjects[i];
			if (pHolded->DataX == pObject->DataX &&
				pHolded->DataY == pObject->DataY)
			{
				if (g_MouseOver.IsMoseHoverWndObj(this, i))
					g_MouseOver.CancelMouseHoverInfo();
				m_nNumObjects--;
				for (; i < m_nNumObjects; i++)
					m_pObjects[i] = m_pObjects[i + 1];
				return true;
			}
		}
	}
	return false;
}


//

int	KWndObjectMatrix::GetObject(KUiDraggedObject& Obj, int x, int y) const
{
	for (int i = 0; i < m_nNumObjects; i++)
	{
		KUiDraggedObject*	pHolded = &m_pObjects[i];
		if (x == pHolded->DataX && y == pHolded->DataY)
		{
			Obj = *pHolded;
			return true;
		}
	}
	return false;
}

void KWndObjectMatrix::Clear()
{
	m_nNumObjects = 0;
	m_nMouseOverObj = -1;
	if (m_pObjects)
	{
		free(m_pObjects);
		m_pObjects = NULL;
	}
}

int KWndObjectMatrix::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
// DEBUG: Log WM_MOUSEMOVE messages to catch gold mask hover
	if (uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEHOVER)
	{
		FILE* pLog = fopen("mask_tooltip_debug.log", "a");
		if(pLog)
		{
			int x = LOWORD(nParam);
			int y = HIWORD(nParam);
			fprintf(pLog, "[WNDMATRIX-WNDPROC] uMsg=%s at (%d,%d), m_nNumObjects=%d\n",
				   (uMsg == WM_MOUSEMOVE) ? "WM_MOUSEMOVE" : "WM_MOUSEHOVER",
				   x, y, m_nNumObjects);
			fclose(pLog);
		}
	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		if ((m_Style & OBJCONT_S_DISABLE_PICKPUT)== 0)
		{
			if (Wnd_GetDragObj(NULL))
				DropObject(LOWORD(nParam), HIWORD(nParam), false);
			else
				PickUpObjectAt(LOWORD(nParam), HIWORD(nParam));
		}
		else if (m_pParentWnd)
		{
			m_nPutPosX = NO_MATCHED_PUT_POS;
			int nObj = GetObjectAt(LOWORD(nParam), HIWORD(nParam));
			if (nObj >= 0)
			{
				KUiDraggedObject	Obj;
				Obj = m_pObjects[nObj];
				m_pParentWnd->WndProc(WND_N_LEFT_CLICK_ITEM,
					(unsigned int)&Obj, (int)(KWndWindow*)this);
			}
		}
		break;
	case WM_RBUTTONDOWN:
    if ((m_Style & OBJCONT_S_DISABLE_PICKPUT) == 0)
    {
        int nObj = GetObjectAt(LOWORD(nParam), HIWORD(nParam));
        if (nObj < 0)
            break;

        KUiDraggedObject Obj = m_pObjects[nObj];

        if (KUiStoreBox::GetIfVisible() != NULL)
        {
            int cont = m_nContainerId;
            BOOL inStore =
                (cont == UOC_STORE_BOX)  || (cont == UOC_STORE_BOX1) ||
                (cont == UOC_STORE_BOX2) || (cont == UOC_STORE_BOX3) ||
                (cont == UOC_STORE_BOX4) || (cont == UOC_STORE_BOX5) ||
                (cont == UOC_EXPAND_BOX1);

            if (inStore)
                KUiItem::MoveItemToInventory(&Obj);   // r??ng -> h鄋h trang
            else
                KUiItem::MoveItemToRepository(&Obj);   // h鄋h trang -> r??ng
        }
        else if (g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) >= item_equip &&
                 g_pCoreShell->GetGenreItem(Obj.uId, Obj.uGenre) <= item_platinaequip)
        {
            KUiObjAtRegion Info;
            Info.Obj.uGenre    = Obj.uGenre;
            Info.Obj.uId       = Obj.uId;
            Info.Region.Width  = 0;
            Info.Region.Height = 0;
            Info.Region.h      = Obj.DataX;
            Info.Region.v      = Obj.DataY;

            g_pCoreShell->OperationRequest(GOI_USE_ITEM, (unsigned int)&Info, UOC_ITEM_TAKE_WITH);
        }
        else
        {
            if (Wnd_GetDragObj(NULL))
            {
                DropObject(LOWORD(nParam), HIWORD(nParam), false);
            }
            else if (m_pParentWnd)
            {
                m_nPutPosX = NO_MATCHED_PUT_POS;
                m_pParentWnd->WndProc(
                    WND_N_RIGHT_CLICK_ITEM,
                    (unsigned int)&Obj,
                    (int)(KWndWindow*)this
                );
            }
        }
    }
    else if (m_pParentWnd)
    {
        m_nPutPosX = NO_MATCHED_PUT_POS;
        int nObj = GetObjectAt(LOWORD(nParam), HIWORD(nParam));
        if (nObj >= 0)
        {
            KUiDraggedObject Obj = m_pObjects[nObj];
            m_pParentWnd->WndProc(
                WND_N_RIGHT_CLICK_ITEM,
                (unsigned int)&Obj,
                (int)(KWndWindow*)this
            );
        }
    }
    break;

	case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
		m_Style |= OBJCONT_F_MOUSE_HOVER;
		{
			int	x = LOWORD(nParam);
			int y = HIWORD(nParam);
			int nObj = GetObjectAt(x, y);

			// DEBUG: Log mouse move in matrix
			FILE* pLog = fopen("mask_tooltip_debug.log", "a");
			if(pLog)
			{
				fprintf(pLog, "[WNDMATRIX-MOUSEMOVE] nObj=%d at (%d,%d)\n", nObj, x, y);
				fclose(pLog);
			}
			m_nMouseOverObj = nObj;
			if (nObj >= 0)
			{
				// DEBUG: Log BEFORE accessing m_pObjects array
				pLog = fopen("mask_tooltip_debug.log", "a");
				if(pLog)
				{
					fprintf(pLog, "[WNDMATRIX-ACCESS] About to access m_pObjects[%d], m_nNumObjects=%d, m_pObjects=%p\n",
						   nObj, m_nNumObjects, m_pObjects);
					fclose(pLog);
				}

				// DEBUG: Log object found
				pLog = fopen("mask_tooltip_debug.log", "a");
				if(pLog)
				{
					fprintf(pLog, "[WNDMATRIX-OBJ] Found obj %d: Genre=%d, uId=%d\n",
						   nObj, m_pObjects[nObj].uGenre, m_pObjects[nObj].uId);
					fclose(pLog);
				}

				if (g_MouseOver.IsMoseHoverWndObj(this, nObj) == 0)
				{
					// DEBUG: Log parameters BEFORE calling function
					pLog = fopen("mask_tooltip_debug.log", "a");
					if(pLog)
					{
						unsigned int uGenre = m_pObjects[nObj].uGenre;
						unsigned int uId = m_pObjects[nObj].uId;
						fprintf(pLog, "[WNDMATRIX-CALLING] About to call SetMouseHoverObjectDesc\n");
						fprintf(pLog, "[WNDMATRIX-PARAMS] this=%p, nObj=%d, uGenre=%d, uId=%d, ContainerId=%d, x=%d, y=%d\n",
							   this, nObj, uGenre, uId, m_nContainerId, x, y);
						fclose(pLog);
					}
					SetMouseHoverObjectDesc(this, nObj, m_pObjects[nObj].uGenre,
						m_pObjects[nObj].uId, m_nContainerId, x, y);

					// Log AFTER successful call
					pLog = fopen("mask_tooltip_debug.log", "a");
					if(pLog)
					{
						fprintf(pLog, "[WNDMATRIX-RETURNED] SetMouseHoverObjectDesc returned successfully\n");
						fclose(pLog);
					}
				}
				else
				{
					pLog = fopen("mask_tooltip_debug.log", "a");
					if(pLog)
					{
						fprintf(pLog, "[WNDMATRIX-SKIP] IsMoseHoverWndObj returned non-zero\n");
						fclose(pLog);
					}
				}
			}
			else
				g_MouseOver.CancelMouseHoverInfo();
			if ((m_Style & OBJCONT_S_TRACE_PUT_POS) && Wnd_GetDragObj(NULL))
			{
				DropObject(LOWORD(nParam), HIWORD(nParam), true);
			}
		}
		break;
	case WND_M_MOUSE_LEAVE:
		m_nPutPosX = NO_MATCHED_PUT_POS;
		m_Style &= ~OBJCONT_F_MOUSE_HOVER;
		KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

int KWndObjectMatrix::GetObjectAt(int x, int y)
{
	x = (x - m_nAbsoluteLeft) / m_nUnitWidth;
	y = (y - m_nAbsoluteTop) / m_nUnitHeight;
	for (int i = 0; i < m_nNumObjects; i++)
	{
		KUiDraggedObject*	pHolded = &m_pObjects[i];
		if (x < pHolded->DataX || y < pHolded->DataY ||
			x >= pHolded->DataX + pHolded->DataW ||
			y >= pHolded->DataY + pHolded->DataH)
			continue;
		return i;
	}
	return -1;
}

int KWndObjectMatrix::PickUpObjectAt(int x, int y)
{
	int nPicked = GetObjectAt(x, y);
	if (nPicked >= 0)
	{
		ITEM_PICKDROP_PLACE	Pick;
		Pick.pWnd = this;
		Pick.h = m_pObjects[nPicked].DataX;
		Pick.v = m_pObjects[nPicked].DataY;
		m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP, (unsigned int)&Pick, NULL);
		return true;
	}
	return false;
}

int KWndObjectMatrix::DropObject(int x, int y, bool bTestOnly)
{
	KUiDraggedObject	DragObj;

	m_nPutPosX = NO_MATCHED_PUT_POS;
	if (m_Style & OBJCONT_S_ACCEPT_FREE)
	{
		if (bTestOnly == false)
			DropObject(x, y, (KUiDraggedObject*)NULL);
		return true;
	}

	Wnd_GetDragObj(&DragObj);
	if (DragObj.uGenre == CGOG_SKILL_CUSTOM)
	{
		if (bTestOnly == false)
			Wnd_DragFinished();
	}

	if (DragObj.DataW > m_nNumUnitHori || DragObj.DataH > m_nNUmUnitVert)
		return false;

	RECT	or;
	KUiDraggedObject* pOverlaped = NULL;

	x = (x - m_nAbsoluteLeft) / m_nUnitWidth;
	y = (y - m_nAbsoluteTop) / m_nUnitHeight;
	if ((or.right = x + (DragObj.DataW + 1) / 2) > m_nNumUnitHori)
		or.right = m_nNumUnitHori;
	if ((or.bottom = y + (DragObj.DataH + 1) / 2) > m_nNUmUnitVert)
		or.bottom = m_nNUmUnitVert;
	if (or.right >= DragObj.DataW)
		or.left = or.right - DragObj.DataW;
	else
	{
		or.left = 0;
		or.right = DragObj.DataW;
	}
	if (or.bottom >= DragObj.DataH)
		or.top = or.bottom - DragObj.DataH;
	else
	{
		or.top = 0;
		or.bottom = DragObj.DataH;
	}

	if (TryDropObjAtPos(or, pOverlaped))
	{
		if (bTestOnly == false)
			DropObject(or.left, or.top, pOverlaped);
		else if (pOverlaped)
		{
			m_nPutPosX = REPLACE_ITEM_POS(pOverlaped - m_pObjects);
		}
		else
		{
			m_nPutPosX = or.left;
			m_nPutPosY = or.top;
			m_nPutWidth = DragObj.DataW;
			m_nPutHeight = DragObj.DataH;
		}
		return true;
	}

	if (DragObj.DataW == 1 && DragObj.DataH == 1)
		return false;

	RECT	Try;
	Try.right = x;
	Try.bottom = y;
	if ((Try.left = x - DragObj.DataW + 1) < 0)
		Try.left = 0;
	if ((Try.top = y - DragObj.DataH + 1) < 0)
		Try.top = 0;

	for (or.left = Try.left; or.left <= Try.right; or.left ++)
	{
		or.right = or.left + DragObj.DataW;
		for (or.top = Try.top; or.top <= Try.bottom; or.top ++)
		{
			or.bottom = or.top + DragObj.DataH;
			if (TryDropObjAtPos(or, pOverlaped))
			{
				if (bTestOnly == false)
					DropObject(or.left, or.top, pOverlaped);
				return true;
			}
		}
	}
	return false;
}

int KWndObjectMatrix::TryDropObjAtPos(const RECT& dor, KUiDraggedObject*& pOverlaped)
{
	pOverlaped = NULL;
	int i = 0;
	for (i = 0; i < m_nNumObjects; i++)
	{
		KUiDraggedObject*	pHolded = &m_pObjects[i];
		if (pHolded->DataX >= dor.right || pHolded->DataY >= dor.bottom ||
			pHolded->DataX + pHolded->DataW <= dor.left ||
			pHolded->DataY + pHolded->DataH <= dor.top)
			continue;
		if (pOverlaped)
			break;
		pOverlaped = pHolded;
	}
	return (i == m_nNumObjects);
}

void KWndObjectMatrix::DropObject(int x, int y, KUiDraggedObject* pToPickUpObj)
{
	ITEM_PICKDROP_PLACE	Drop;
	Drop.pWnd = this;
	Drop.h = x;
	Drop.v = y;

	if (pToPickUpObj)
	{
		ITEM_PICKDROP_PLACE	Pick;
		Pick.pWnd = this;
		Pick.h = pToPickUpObj->DataX;
		Pick.v = pToPickUpObj->DataY;
		m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP, (unsigned int)&Pick, (int)&Drop);
	}
	else
		m_pParentWnd->WndProc(WND_N_ITEM_PICKDROP, NULL, (int)&Drop);
}

void KWndObjectMatrix::EnableTracePutPos(bool bEnable)
{
	if (bEnable)
		m_Style |= OBJCONT_S_TRACE_PUT_POS;
	else
		m_Style &= ~OBJCONT_S_TRACE_PUT_POS;
}

void KWndObjectMatrix::EnablePickPut(bool bEnable)
{
	if (bEnable == false)
		m_Style |= OBJCONT_S_DISABLE_PICKPUT;
	else
		m_Style &= ~OBJCONT_S_DISABLE_PICKPUT;
}
