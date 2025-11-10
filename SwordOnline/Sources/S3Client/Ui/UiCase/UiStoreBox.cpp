/*****************************************************************************************
//	½çÃæ// --´¢ÎïÏä½çÃæ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiStoreBox.h"
#include "UiGetMoney.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../core/src/GameDataDef.h"

#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiPlayerShop.h"
#include "UiInformation.h"
#include <crtdbg.h>
#include "KEngine.h"
//LockPlayer by kinnox;
#include "UiInformation.h"
//
#include "UiCreatPW.h"
#include "UiLoginPW.h"
#include "UiChaPW.h"
//
extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*		g_pCoreShell;


#define SCHEME_INI	"UiStoreBox.ini"

KUiStoreBox* KUiStoreBox::m_pSelf = NULL;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UISTOREBOX_WAIT_GETMONEY,
};


KUiStoreBox* KUiStoreBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiStoreBox* KUiStoreBox::OpenWindow(int nToPage)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStoreBox;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		if (!KUiItem::GetIfVisible())
			KUiItem::OpenWindow();

		Wnd_GameSpaceHandleInput(false);

		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData(nToPage);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


void KUiStoreBox::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -
// ¹¦ÄÜ	: ³õÊ¼»¯
// -
void KUiStoreBox::Initialize()
{
	m_nCurPage = -1;

	AddChild(&m_Money);
	AddChild(&m_GetMoneyBtn);
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);
	//LockPlayer by kinnox;
	AddChild(&m_PreBtn);
    AddChild(&m_NextBtn);
	AddChild(&m_LockBtn);
	AddChild(&m_UnLockBtn);
	AddChild(&m_CrePWBtn);
	AddChild(&m_ChaPWBtn);
	//
	m_ItemBox.SetContainerId((int)UOC_STORE_BOX);
	AddChild(&m_CurPage);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_nMoney = 0;
	Wnd_AddWindow(this);
}
void KUiStoreBox::UpdateClientData() //LockPlayer by kinnnox;
{
	if (m_pSelf)
	{
		m_pSelf->UpdateData(-1);
	}
}

void KUiStoreBox::UpdateData(int nCurPage)
{
	m_nExpandBoxNum = g_pCoreShell->GetGameData(GDI_EXBOX_NUM, NULL, NULL);
	m_nCurPage = nCurPage;
	//LockPlayer by kinnox;
	POINT Check = g_pCoreShell->GetStatusLockPlayer();
	if (!Check.x && !Check.y)
	// mo khoa va chua dat pass 
	{
		m_CrePWBtn.Show();
		m_ChaPWBtn.Hide();
	}
	else
	{
		m_ChaPWBtn.Show();
		m_CrePWBtn.Hide();
	}
	
	if (!Check.x && Check.y)
	{
		m_LockBtn.Show();
		m_UnLockBtn.Hide();
	}
	else
	{
		m_UnLockBtn.Show();
		m_LockBtn.Hide();
	}
	//
	m_ItemBox.Clear();
	m_nMoney = 0;

	KUiObjAtRegion* pObjs = NULL;
	m_ItemBox.SetContainerId((int)UOC_STORE_BOX);
	unsigned uIndex = GDI_ITEM_IN_STORE_BOX;
	
	switch (m_nCurPage)
	{
		case REPOSITORY_ONE:
			m_CurPage.SetText("R­¬ng më réng 1");
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX1);
			uIndex = GDI_ITEM_IN_STORE_BOX1;
			break;
		case REPOSITORY_TWO:
			m_CurPage.SetText("R­¬ng më réng 2");
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX2);
			uIndex = GDI_ITEM_IN_STORE_BOX2;
			break;
		case REPOSITORY_THREE:
			m_CurPage.SetText("R­¬ng më réng 3");
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX3);
			uIndex = GDI_ITEM_IN_STORE_BOX3;
			break;
		case REPOSITORY_FOUR:
			m_CurPage.SetText("R­¬ng më réng 4");
			uIndex = GDI_ITEM_IN_STORE_BOX4;
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX4);
			break;
		case REPOSITORY_FIVE:
			m_CurPage.SetText("R­¬ng më réng 5");
			uIndex = GDI_ITEM_IN_STORE_BOX5;
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX5);
			break;
		default:
			m_CurPage.SetText("R­¬ng chøa ®å");
			m_ItemBox.SetContainerId((int)UOC_STORE_BOX);
			uIndex = GDI_ITEM_IN_STORE_BOX;
			break;
	}

	int nCount = g_pCoreShell->GetGameData(uIndex, 0, 0);
	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(uIndex, (unsigned int)pObjs, nCount);
		for (int i = 0; i < nCount; i++)
			UpdateItem(m_nCurPage, &pObjs[i], true);
		free(pObjs);
		pObjs = NULL;
	}
}


//

void KUiStoreBox::UpdateItem(int nToPage, KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem->Obj.uGenre != CGOG_MONEY && m_nCurPage != nToPage)
		return;

	if (pItem)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		if (pItem->Obj.uGenre != CGOG_MONEY)
		{
			KUiDraggedObject Obj;
			Obj.uGenre = pItem->Obj.uGenre;
			Obj.uId = pItem->Obj.uId;
			Obj.DataX = pItem->Region.h;
			Obj.DataY = pItem->Region.v;
			Obj.DataW = pItem->Region.Width;
			Obj.DataH = pItem->Region.Height;
			if (bAdd)
				m_ItemBox.AddObject(&Obj, 1);
			else
				m_ItemBox.RemoveObject(&Obj);
		}
		else
		{
			m_nMoney = pItem->Obj.uId;
			m_Money.SetMoneyText(m_nMoney);
		}
	}
	else
	{
		UpdateData(m_nCurPage);
	}
}

// -
// ¹¦ÄÜ	: ÔØÈë½çÃæ·½°¸
// -
void KUiStoreBox::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Money.Init(&Ini, "Money");
		m_pSelf->m_GetMoneyBtn.Init(&Ini, "GetMoneyBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_ItemBox.EnableTracePutPos(true);
		m_pSelf->m_CurPage.Init(&Ini, "CurPage");
		//LockPlayer by kinnox;
		m_pSelf->m_PreBtn.Init(&Ini, "LeftBtn");
		m_pSelf->m_NextBtn.Init(&Ini,"RightBtn");
		m_pSelf->m_LockBtn.Init(&Ini,"LockBtn");
		m_pSelf->m_UnLockBtn.Init(&Ini,"UnLockBtn"); 
		m_pSelf->m_CrePWBtn.Init(&Ini,"CrePWBtn");
		m_pSelf->m_ChaPWBtn.Init(&Ini,"ChaPWBtn");
		//
	}
}

// -
// ¹¦ÄÜ	: ´°¿Úº¯Êý
// -
int KUiStoreBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
			OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_RIGHT_CLICK_ITEM:
			OnClickItem((KUiDraggedObject*)uParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow();
		else if (uParam == (unsigned int)(KWndWindow*)&m_GetMoneyBtn)
			KUiGetMoney::OpenWindow(0, m_nMoney, this, UISTOREBOX_WAIT_GETMONEY, &m_Money);
		else if (uParam == (unsigned int)(KWndWindow*)&m_PreBtn)
		{
			m_nCurPage--;
			if(m_nCurPage < -1)
			{
				m_nCurPage = -1;
				return 0;
			}
			this->UpdateData(m_nCurPage);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_NextBtn)
		{
			m_nCurPage++;
			if(m_nCurPage > REPOSITORY_FIVE)
			{
				m_nCurPage = REPOSITORY_FIVE;
				return 0;
			}
			this->UpdateData(m_nCurPage);
		}			
		else if (uParam == (unsigned int)(KWndWindow*)&m_LockBtn)//LockPlayer by kinnox;
		{
			POINT Check = g_pCoreShell->GetStatusLockPlayer();
			if (!Check.x && Check.y)
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_LOCK_PWD,TRUE,0);
			}
			else
			{
				UIMessageBox("C¸c h¹ ch­a thiÕt lËp mËt khÈu b¶o vÖ nh©n vËt!",this,"Tho¸t");
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_UnLockBtn)
		{
			POINT Check = g_pCoreShell->GetStatusLockPlayer();
			if (Check.x && Check.y)
			{
				if (KUiChaPW::GetIfVisible())
				{
					/* if KUiChaPW  is Open, then run this function  */
				}
				else if (KUiCreatPW::GetIfVisible())
				{
					/* if KUiCreatePWBox  is Open, then run this function  */
				}
				else
				{
					KUiLoginPW::OpenWindow();
				}
			}
			else
			{
				UIMessageBox("C¸c h¹ ch­a thiÕt lËp mËt khÈu b¶o vÖ nh©n vËt!",this,"Tho¸t");
			}	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CrePWBtn)
		{
			POINT Check = g_pCoreShell->GetStatusLockPlayer();
			if (!Check.x && !Check.y)
			{
				if (KUiChaPW::GetIfVisible())
				{
					// /* if KUiChaPW  is Open, then run this function  */
				}
				else if (KUiLoginPW::GetIfVisible())
				{
					/* if KUiLoginPW is Open, then run this function  */
				}
				else
				{
					KUiCreatPW::OpenWindow();
				}
			}
			else
			{
				UIMessageBox("C¸c h¹ ch­a më khãa b¶o vÖ nh©n vËt!",this,"Tho¸t");
			}	
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChaPWBtn)
		{
			POINT Check = g_pCoreShell->GetStatusLockPlayer();
			if (!Check.x && Check.y)
			{
				if (KUiCreatPW::GetIfVisible())
				{
					/* if KUiCreatPW  is Open, then run this function  */
				}
				else if (KUiLoginPW::GetIfVisible())
				{
					/* if KUiLoginPW  is Open, then run this function  */
				}
				else
				{
					KUiChaPW::OpenWindow();
				}
			}
			else
			{
				UIMessageBox("C¸c h¹ ch­a më khãa b¶o vÖ nh©n vËt!",this,"Tho¸t");
			}	
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (uParam == UISTOREBOX_WAIT_GETMONEY)
			OnGetMoney(nParam);
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiStoreBox::OnGetMoney(int nMoney)
{
	if (nMoney > 0)
	{
		g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,
			false, nMoney);
	}
}

void KUiStoreBox::OnClickItem(KUiDraggedObject* pItem)
{
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.h = pItem->DataX;
	Obj.Region.v = pItem->DataY;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.eContainer = UOC_STORE_BOX;
}

void KUiStoreBox::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (g_pCoreShell == NULL)
		return;

	if (g_UiBase.GetStatus() != UIS_S_IDLE)
		return;
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);		
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(
			Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY;
		//
		switch (m_nCurPage)
		{
			case REPOSITORY_ONE:
				Pick.eContainer = UOC_STORE_BOX1;
				break;
			case REPOSITORY_TWO:
				Pick.eContainer = UOC_STORE_BOX2;
				break;
			case REPOSITORY_THREE:
				Pick.eContainer = UOC_STORE_BOX3;
				break;
			case REPOSITORY_FOUR:
				Pick.eContainer = UOC_STORE_BOX4;
				break;
			case REPOSITORY_FIVE:
				Pick.eContainer = UOC_STORE_BOX5;
				break;
			default:
				Pick.eContainer = UOC_STORE_BOX;
				break;
		}
		//

	}

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = pDropPos->h;
		Drop.Region.v = pDropPos->v;
		//
		switch (m_nCurPage)
		{
			case REPOSITORY_ONE:
				Drop.eContainer = UOC_STORE_BOX1;
				break;
			case REPOSITORY_TWO:
				Drop.eContainer = UOC_STORE_BOX2;
				break;
			case REPOSITORY_THREE:
				Drop.eContainer = UOC_STORE_BOX3;
				break;
			case REPOSITORY_FOUR:
				Drop.eContainer = UOC_STORE_BOX4;
				break;
			case REPOSITORY_FIVE:
				Drop.eContainer = UOC_STORE_BOX5;
				break;
			default:
				Drop.eContainer = UOC_STORE_BOX;
				break;
		}
		//
	}
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

void KUiStoreBox::PaintWindow() 
{
	KWndShowAnimate::PaintWindow();
	if (g_pRepresentShell)
	{
		if (m_nCurPage >= m_nExpandBoxNum)
		{
			KRUShadow	Shadow;
			m_ItemBox.GetAbsolutePos(&Shadow.oPosition.nX, &Shadow.oPosition.nY);
			Shadow.oPosition.nX;
			Shadow.oPosition.nY;
			Shadow.Color.Color_dw = 0x191079a5;
			m_ItemBox.GetSize(&Shadow.oEndPos.nX, &Shadow.oEndPos.nY);
			Shadow.oEndPos.nX += Shadow.oPosition.nX;
			Shadow.oEndPos.nY += Shadow.oPosition.nY;
			g_pRepresentShell->DrawPrimitives(1, &Shadow, RU_T_SHADOW, true);	
		}
	}
}
int KUiStoreBox::GetCurrentContainerID()
{
    switch (m_nCurPage)
    {
        case REPOSITORY_ONE:   return UOC_STORE_BOX1;
        case REPOSITORY_TWO:   return UOC_STORE_BOX2;
        case REPOSITORY_THREE: return UOC_STORE_BOX3;
        case REPOSITORY_FOUR:  return UOC_STORE_BOX4;
        case REPOSITORY_FIVE:  return UOC_STORE_BOX5;
        default: return UOC_STORE_BOX;
    }
}