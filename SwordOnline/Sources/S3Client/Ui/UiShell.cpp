#include "KWin32.h"
#include "KIniFile.h"
#include "Elem/Wnds.h"
#include "Elem/UiImage.h"
#include "UiShell.h"
#include "UiBase.h"
#include "../../core/src/coreshell.h"
#include "../../Engine/src/KDebug.h"
#include "UiCase/UiInit.h"
#include "UiCase/UiConnectInfo.h"
#include "UiCase/UiInformation.h"
#include "UiCase/UiInformation2.h"
#include "UiCase/UiInformation3.h"
#include "UiCase/UiTongInformation.h"
#include "UiCase/UiPlayerBar.h"
#include "UiCase/UiStatus.h"
#include "UiCase/UiParadeItem.h"
#include "UiCase/UiItem.h"
#include "UiCase/UiSkills.h"
#include "UiCase/UiSkillTree.h"
#include "UiCase/UiEscDlg.h"
#include "UiCase/UiGame.h"
#include "UiCase/UiOptions.h"
#include "UiCase/UiMsgCentrePad.h"
#include "UiCase/UiLoginBg.h"
#include "UiCase/UiLogin.h"
#include "UiCase/UiSelPlayer.h"
#include "UiCase/UiSelServer.h"
#include "UiCase/UiNewPlayer.h"
#include "UiCase/UiSelNativePlace.h"
#include "UiCase/UiChatCentre.h"
#include "UiCase/UiChatStatus.h"
#include "UiCase/UiSysMsgCentre.h"
#include "UiCase/UiHeaderControlBar.h"
#include "UiCase/UiStoreBox.h"
#include "UiCase/UiChannelSubscibe.h"
#include "UiCase/UiSelPlayerNearby.h"
#include "UiCase/UiMsgSel.h"
#include "UiCase/UiTeamManage.h"
#include "UiCase/UiShop.h"
#include "UiCase/UiMiniMap.h"
#include "UiCase/UiTrade.h"
#include "UiCase/UiTradeConfirmWnd.h"
#include "UiCase/UiNewsMessage.h"
#include "UiCase/UiNewsMessageEx.h"
#include "ShortcutKey.h"
#include "UiSoundSetting.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "../NetConnect/NetConnectAgent.h"
#include "../Login/Login.h"
#include "UiCase/UiNewPlayerStartMsg.h"
#include "UiCase/UiHelper.h"
#include "UiCase/UiHelper2.h"
#include "UiCase/UiTaskNote.h"
#include "UiCase/UiReconnect.h"
#include "UiCase/UiFaceSelector.h"
#include "UiCase/UiStrengthRank.h"
#include "UiCase/UiTongManager.h"
#include "UiCase/UiTongCreateSheet.h"
#include "UiCase/UiFindPos.h"
#include "UiCase/UiChatItemInfo.h"
#include "UiCase/UiPlayerShop.h"
#include "UiCase/UiMsgSel2.h"
#include "UiCase/UiTeamStatus.h"
#include "UiCase/UiAutoPlay.h"//AutoAI by kinnox;
#include "UiCase/UiCreatPW.h" //LockPlayer by kinnox;
#include "UiCase/UiLoginPW.h" //LockPlayer by kinnox;
#include "UiCase/UiChaPW.h" //LockPlayer by kinnox;
#include "UiCase/UiKeyBoard.h" //LockPlayer by kinnox;
#include "UiCase/UiExpandItem.h" //ExPandBox by kinnox;
#include "UiCase/UiGiveBox.h" //GiveBox by kinnox;
#include "UiCase/UiReward.h" //Reward by kinnox;
#include "UiCase/UiTopTKNew.h" //Report Battle by kinnox;
#include "UiCase/UiChooseFace.h" //Player Avatar by kinnox;
#include "UiCase/UiNotification.h" //Player Notification by kinnox;
#include "UiChatPhrase.h"

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;
extern KUiChatPhrase    g_UiChatPhrase;

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
enum UI_LIVING_STATUS
{
	UI_LIVING_S_DEAD,
	UI_LIVING_S_OUTGAME,		
	UI_LIVING_S_INGAME,
};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
static UI_LIVING_STATUS	s_UiLiveSeed = UI_LIVING_S_DEAD;

int			g_bDisconnect = false;
	#include "KTimer.h"
	static KTimer	s_Timer;
	static int		s_nFrameRate = 30;  
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	#include <shlwapi.h>
	char			s_VersionInfo[60];
	int				g_nShowDebugInfo = true;
#endif

#define DYNAMIC_LINK_REPRESENT_LIBRARY

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	extern 	int		g_bRepresent3;
#endif

void		UiCloseWndsOutGame(bool bAll);
bool		UiCloseWndsInGame(bool bAll);

#ifdef SWORDONLINE_SHOW_DBUG_INFO
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool GetFileTimeVersionString(const char* pszFile, char* pszVersionString, int nSize)
{
	if (pszVersionString)
		pszVersionString[0] = 0;

	HANDLE	hFile = CreateFile(pszFile, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	FILETIME	time, FileTime;
	SYSTEMTIME	SysTime;
	if (::GetFileTime(hFile, NULL, NULL, &time) == FALSE)
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	hFile = NULL;
	FileTimeToLocalFileTime(&time, &FileTime);
	FileTimeToSystemTime(&FileTime, &SysTime);
	int nLen = GetDateFormat(LOCALE_USER_DEFAULT, 0, &SysTime, "M-d ", pszVersionString, nSize);
	if (nLen)
		nLen --;
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &SysTime, "H:mm", &pszVersionString[nLen], nSize - nLen);
	return true;
}
#endif

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
int	UiGetFrameRate()
{
	s_Timer.GetFPS(&s_nFrameRate);
	return s_nFrameRate;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiPostQuitMsg()
{	
	s_UiLiveSeed = UI_LIVING_S_DEAD;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool UiIsAlreadyQuit()
{
	return (s_UiLiveSeed == UI_LIVING_S_DEAD);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int	UiInit()
{
	Player_Life::RegisterSelfClass();
	Player_Mana::RegisterSelfClass();
	Player_Stamina::RegisterSelfClass();
	Player_Exp::RegisterSelfClass();
	Player_Level::RegisterSelfClass();
	Player_WorldSort::RegisterSelfClass();
	Player_Status::RegisterSelfClass();
	Player_Items::RegisterSelfClass();
	Player_ItemEx::RegisterSelfClass();
	Player_Skills::RegisterSelfClass();
	Player_Team::RegisterSelfClass();
	Player_Sit::RegisterSelfClass();
	Player_Run::RegisterSelfClass();
	Player_Horse::RegisterSelfClass();
	Player_Exchange::RegisterSelfClass();
	Player_PK::RegisterSelfClass();
	Player_Faction::RegisterSelfClass();
	Player_Task::RegisterSelfClass();
	Player_AutoPlay::RegisterSelfClass(); // paint auto by kinnox
	Player_Lock::RegisterSelfClass();//LockPlayer by kinnox;
	Wnd_ShowCursor(false);
	if (g_UiBase.Init() == false)
		return false;
	IR_UpdateTime();
	g_UiInformation.Initialize();
	g_UiInformation2.Initialize();
	g_UiInformation3.Initialize();
	KUiLoginBackGround::OpenWindow("Init");
	Wnd_SetGameSpaceWnd(&g_WndGameSpace);

	UiSoundLoadSetting();

	KUiFaceSelector::LoadFaceList();
	KShortcutKeyCentre::InitScript();

#ifdef SWORDONLINE_SHOW_DBUG_INFO
	char	szFile[MAX_PATH];
	GetModuleFileName(NULL, szFile, MAX_PATH);
	strcpy(s_VersionInfo, PathFindFileName(szFile));
	PathRemoveExtension(s_VersionInfo);
	strcat(s_VersionInfo, ":");	
	int nLen = strlen(s_VersionInfo);
	GetFileTimeVersionString(szFile, &s_VersionInfo[nLen], sizeof(s_VersionInfo) - nLen);
	PathRemoveFileSpec(szFile);
	strcat(szFile, "\\CoreClient.dll");
	strcat(s_VersionInfo, " CoreClient:");
	nLen = strlen(s_VersionInfo);
	GetFileTimeVersionString(szFile, &s_VersionInfo[nLen], sizeof(s_VersionInfo) - nLen);
#endif

	g_UiChatPhrase.LoadEntireEmote();

	g_UiBase.NotifyEvent(APP_START);

	return true;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void	UiExit()
{
	if (s_UiLiveSeed == UI_LIVING_S_INGAME)
	{
		g_UiBase.SavePrivateConfig();
	}
	KReconnectWnd::Exit(false);

	KUiInit::StopTitleMusic();

	KUiStrengthRank::RemoveTempFile();
	KUiTongManager::RemoveTempFile();

	UiCloseWndsInGame(true);
	UiCloseWndsOutGame(true);

	Wnd_SetGameSpaceWnd(NULL);
	g_LoginLogic.ReturnToIdle();
	g_LoginLogic.SaveLoginChoice();
	g_UiBase.Exit();
	KShortcutKeyCentre::UninitScript();
	s_UiLiveSeed = UI_LIVING_S_DEAD;
	KUiFaceSelector::Clear();
	Wnd_Cleanup();

	g_UiBase.NotifyEvent(APP_EXIT);
	g_UiBase.CleanTempDataFolder();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int UiStart()
{	
	s_Timer.Start();
	s_UiLiveSeed = UI_LIVING_S_OUTGAME;
	Wnd_ShowCursor(true);
	Wnd_SwitchCursor(CURSOR_NORMAL);
	return (KUiInit::OpenWindow(true, true) != NULL);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void UiPaint(int nGameLoop)
{
	if (g_pRepresentShell == NULL || 
		g_pRepresentShell->RepresentBegin(false, 0) == false)
	{
		if (g_pCoreShell)
			g_pCoreShell->SetRepresentShell(g_pRepresentShell);
		return;
	}
	
	Wnd_RenderWindows();

#ifdef _DEBUG
	DWORD	dwPing = 0;
	//char	Info[128];
	if (g_pCoreShell)
		dwPing = g_pCoreShell->GetPing();
#endif

/*#ifdef SWORDONLINE_SHOW_DBUG_INFO
	if (g_nShowDebugInfo)
	{		
		sprintf(Info,"FPS=%d LOOP=%d PING=%d", s_nFrameRate, nGameLoop, dwPing);
		g_pRepresentShell->OutputText(12, Info, -1, 10, 20, 0xffffffff, 0);

		g_pRepresentShell->OutputText(12, s_VersionInfo, -1, 2, 510, 0xffffffff, 0);

		#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
		{
			int nWidth, nHeight;
			Wnd_GetScreenSize(nWidth, nHeight);
			g_pRepresentShell->OutputText(12, g_bRepresent3 ? "Represent3" : "Represent2",
				-1, nWidth - 100, 10, 0xffffffff, 0);
		}
		#endif
	}
#endif
*/
	//them by kinnox;
	static DOUBLE s_dGameFPS = 0;
	static INT s_nOldGameLoop = 0;
	INT		nElapse = s_Timer.GetElapse();
	if (s_Timer.GetFPS(&s_nFrameRate))
	{
		s_dGameFPS = (nGameLoop - s_nOldGameLoop) * 1000.0 / nElapse;
		s_nOldGameLoop = nGameLoop;
	} 
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	if (g_nShowDebugInfo)
	{		
	//	sprintf(Info,"FPS=%d LOOP=%d PING=%d", s_nFrameRate, nGameLoop, dwPing);
		char	Info[128];
		sprintf(Info,"FRAME=%d FPS=%.1f PING=%d", s_nFrameRate, s_dGameFPS, dwPing);
		if (g_bRepresent3)
		{
			g_pRepresentShell->OutputText(12, Info, -1, 10, 20, 0xffffffff,0,TEXT_IN_SINGLE_PLANE_COORD,0xff000000);
			g_pRepresentShell->OutputText(12, s_VersionInfo, -1, 2, 510, 0xffffffff, 0,TEXT_IN_SINGLE_PLANE_COORD,0xff000000);
		}
		else
		{
			g_pRepresentShell->OutputText(12, Info, -1, 10, 20, 0xffffffff,0);
			g_pRepresentShell->OutputText(12, s_VersionInfo, -1, 2, 510, 0xffffffff,0);
		}

		/*#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
		{
			int nWidth, nHeight;
			Wnd_GetScreenSize(nWidth, nHeight);
			g_pRepresentShell->OutputText(12, g_bRepresent3 ? "Represent3" : "Represent2",
				-1, nWidth - 100, 10, 0xffffffff, 0);
		}
		#endif*/
	}
#endif
	int nWidth, nHeight;
			Wnd_GetScreenSize(nWidth, nHeight);
			g_pRepresentShell->OutputText(12, g_bRepresent3 ? "Represent3" : "Represent2",
				-1, nWidth - 100, 10, 0xffffffff, 0);
	g_pRepresentShell->RepresentEnd();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int UiHeartBeat()
{
	if (s_UiLiveSeed != UI_LIVING_S_DEAD)
	{
		if (g_bDisconnect == false)
		{
			IR_UpdateTime();
			Wnd_Heartbeat();
		}
		else
		{
			if (s_UiLiveSeed == UI_LIVING_S_INGAME &&
				KReconnectWnd::IsReconnecttingGoingOn() == false)
			{
				g_UiBase.SavePrivateConfig();
				g_pCoreShell->OperationRequest(GOI_GAMESPACE_DISCONNECTED, 0, 0);
				KReconnectWnd::LaunchReconnect();
				KUiItem::CloseWindow(true);
				KUiSkills::CloseWindow(true);
				KUiSkillTree::CloseWindow(true);
				KUiChatStatus::CloseWindow();
				KUiStoreBox::CloseWindow();
				KUiTeamManage::CloseWindow();
				KUiTradeConfirm::CloseWindow(true);
				KUiShop::CloseWindow();
				KUiSelPlayerNearby::CloseWindow(true);
				KUiHelper::CloseWindow(true);
				KUiHelper2::CloseWindow(true);
				KUiTaskNote::CloseWindow(true);
				KUiParadeItem::CloseWindow(true);
				KUiStrengthRank::CloseWindow();
				KUiTongManager::CloseWindow(true);
				KUiTongCreateSheet::CloseWindow();
				KUiFindPos::CloseWindow();
				KUiChatItemInfo::CloseWindow();
				KUiAutoPlay::CloseWindow(true); //AutoAI by kinnox;
				KUiCreatPW::CloseWindow();//LockPlayer by kinnox;
				KUiLoginPW::CloseWindow();//LockPlayer by kinnox;
				KUiChaPW::CloseWindow();//LockPlayer by kinnox;
				KUiKeyBoard::CloseWindow();//LockPlayer by kinnox;	
				KUiExpandItem::CloseWindow(); //ExPandBox by kinnox;	
				KUiGiveBox::CloseWindow(true);//GiveBox by kinnox;	
				KUiReWard::CloseWindow(true);//Reward by kinnox;
				KUiTopTKNew::CloseWindow();//Report Battle by kinnox;
			}
			else
			{
				g_LoginLogic.NotifyDisconnect();
			}
			g_bDisconnect = false;
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void	UiProcessInput(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WM_COPYDATA)
	{
		if (nParam)
		{
			COPYDATASTRUCT* pData = (COPYDATASTRUCT*)nParam;
			if (pData && pData->cbData > 0 && pData->lpData && ((char*)pData->lpData)[pData->cbData - 1] == 0)
			{
				KShortcutKeyCentre::ExcuteHWNDScript((const char *)pData->lpData);
			}
		}
	}
	else if (uMsg != WM_ACTIVATEAPP)
        Wnd_ProcessInput(uMsg, uParam, nParam);
	else if (uParam)
	{
		Wnd_RestoreCursor();
		KUiOptions::LoadSetting(false, true);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiCloseWndsOutGame(bool bAll)
{
	KUiLogin::CloseWindow(true);
	KUiSelServer::CloseWindow(true);
	KUiOptions::CloseWindow();
	KUiInit::CloseWindow();
	KUiSelPlayer::CloseWindow();
	KUiNewPlayer::CloseWindow(true);
	KUiSelNativePlace::CloseWindow(true);
	KUiConnectInfo::CloseWindow(true);
	KUiAutoPlay::CloseWindow(true);//AutoAI by kinnox;
	KUiExpandItem::CloseWindow();//Expandbox by kinnox;	
	KUiGiveBox::CloseWindow(true);//GiveBox by kinnox;	
	KUiShop::CloseWindow();
	g_UiInformation.Close();
	g_UiInformation2.Close();
	g_UiInformation3.Close();
	if (bAll)
		KUiLoginBackGround::CloseWindow(true);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiStartGame()
{
	UiOnGameServerConnected();

	g_UiBase.CleanTempDataFolder();

	KUiInit::StopTitleMusic();

	UiCloseWndsOutGame(true);
	KUiLoginBackGround::CloseWindow(false);
	KUiConnectInfo::CloseWindow(true);

	KUiMsgCentrePad::OpenWindow();
	KUiPlayerBar::OpenWindow();
	KUiSysMsgCentre::OpenWindow();
	KUiHeaderControlBar::OpenWindow();
	KUiNewsMessage::OpenWindow();
	KUiNewsMessageEx::OpenWindow();
	Wnd_ShowHideGameSpace(true);
	g_UiBase.SetStatus(UIS_S_IDLE);
	Wnd_GameSpaceHandleInput(true);

	KShortcutKeyCentre::Enable(true);

	if (g_LoginLogic.IsRoleNewCreated())
    {   
        KUiNewPlayerStartMsg::OpenWindow();
    }

	KUiMsgCentrePad::ReleaseActivateChannelAll();
	KUiMsgCentrePad::QueryAllChannel();
	MapSetMode(MINIMAP_M_BRIEF_PIC);
	g_UiBase.LoadPrivateConfig();
	s_UiLiveSeed = UI_LIVING_S_INGAME;

	char szRole[64];
	szRole[0] = 0;
	g_LoginLogic.GetLoginAccount(szRole);
	if (szRole[0] != 0)
	{
		char szEvent[256];
		sprintf(szEvent, APP_STARTGAME, szRole);
		g_UiBase.NotifyEvent(szEvent);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiResumeGame()
{
	s_UiLiveSeed = UI_LIVING_S_INGAME;
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool UiCloseWndsInGame(bool bAll)
{
	if (bAll == false)
	{
		if(!g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0)) // fix ban hang di chuyen by kinnox;
		{
			if (KUiStatus::GetIfVisible() == NULL &&
			KUiItem::GetIfVisible() == NULL &&
			KUiSkills::GetIfVisible() == NULL &&
			KUiSkillTree::GetIfVisible() == NULL &&
			KUiOptions::GetIfVisible() == NULL &&
			KUiChatStatus::GetIfVisible() == NULL &&
			KUiStoreBox::GetIfVisible() == NULL &&
			KUiTeamManage::GetIfVisible() == NULL &&
			KUiTradeConfirm::GetIfVisible() == NULL &&
			KUiShop::GetIfVisible() == NULL &&
			KUiHelper::GetIfVisible() == NULL &&
			KUiHelper2::GetIfVisible() == NULL &&
			KUiTaskNote::GetIfVisible() == NULL &&
			KUiSelPlayerNearby::DoesHaveVisibleWnd() == false &&
			KUiParadeItem::GetIfVisible() == NULL &&
			KUiFaceSelector::GetIfVisible() == NULL &&
			KUiStrengthRank::GetIfVisible() == NULL &&
			KUiTongManager::GetIfVisible() == NULL &&
			KUiTongCreateSheet::GetIfVisible() == NULL &&
			KUiFindPos::GetIfVisible() == NULL && 
			KUiChatItemInfo::GetIfVisible() == NULL &&
			(KUiMiniMap::GetIfVisible() == NULL || 
			MapGetMode() == MINIMAP_M_BRIEF_NOT_PIC || 
			MapGetMode() == MINIMAP_M_BRIEF_PIC) &&
			KUiCreatPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiLoginPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiChaPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiKeyBoard::GetIfVisible() == NULL && //LockPlayer by kinnox;
			KUiExpandItem::GetIfVisible() == NULL &&//ExPandBox by kinnox;
			KUiGiveBox::GetIfVisible() == NULL &&//GiveBox by kinnox;			
			KUiAutoPlay::GetIfVisible() == NULL && //AutoAI by kinnox;
			KUiTopTKNew::GetIfVisible() == NULL && //Report Battle by kinnox;
			KUiChooseFace::GetIfVisible() == NULL &&  //Player Avatar by kinnox;
			KUiNotification::GetIfVisible() == NULL

			)
			{
				return false;
			}
		}
		else
		{
			if (KUiStatus::GetIfVisible() == NULL &&
			// KUiItem::GetIfVisible() == NULL &&
			KUiSkills::GetIfVisible() == NULL &&
			KUiSkillTree::GetIfVisible() == NULL &&
			KUiOptions::GetIfVisible() == NULL &&
			KUiChatStatus::GetIfVisible() == NULL && 
			KUiStoreBox::GetIfVisible() == NULL &&
			KUiTeamManage::GetIfVisible() == NULL &&
			KUiTradeConfirm::GetIfVisible() == NULL &&
			KUiShop::GetIfVisible() == NULL &&
			KUiHelper::GetIfVisible() == NULL &&
			KUiHelper2::GetIfVisible() == NULL &&
			KUiTaskNote::GetIfVisible() == NULL &&
			KUiSelPlayerNearby::DoesHaveVisibleWnd() == false &&
			KUiParadeItem::GetIfVisible() == NULL &&
			KUiFaceSelector::GetIfVisible() == NULL &&
			KUiStrengthRank::GetIfVisible() == NULL &&
			KUiTongManager::GetIfVisible() == NULL &&
			KUiTongCreateSheet::GetIfVisible() == NULL &&
			KUiFindPos::GetIfVisible() == NULL && 
			KUiChatItemInfo::GetIfVisible() == NULL &&
			(KUiMiniMap::GetIfVisible() == NULL || 
			MapGetMode() == MINIMAP_M_BRIEF_NOT_PIC || 
			MapGetMode() == MINIMAP_M_BRIEF_PIC) &&
			KUiCreatPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiLoginPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiChaPW::GetIfVisible() == NULL &&//LockPlayer by kinnox;
			KUiKeyBoard::GetIfVisible() == NULL && //LockPlayer by kinnox;		
			KUiExpandItem::GetIfVisible() == NULL &&//ExPandBox by kinnox;	
			KUiGiveBox::GetIfVisible() == NULL &&//GiveBox by kinnox;			
			KUiAutoPlay::GetIfVisible() == NULL && //AutoAI by kinnox;
			KUiTopTKNew::GetIfVisible() == NULL && //Report Battle by kinnox;
			KUiChooseFace::GetIfVisible() == NULL &&  //Player Avatar by kinnox;
			KUiNotification::GetIfVisible() == NULL
			)
			{
				return false;
			}
		}
	
	}
	else
	{
		KUiMsgSel::CloseWindow(true);
		KUiTrade::CloseWindow();
		KUiPlayerBar::CloseWindow(true);
		KUiMsgCentrePad::CloseWindow(true);
		KUiHeaderControlBar::CloseWindow();
		KUiESCDlg::CloseWindow(true);
		KUiSysMsgCentre::CloseWindow();
		KUiChatCentre::CloseWindow(true);
		MapSetMode(MINIMAP_M_NONE);
		KUiNewsMessage::CloseWindow(TRUE); 
		KUiNewsMessageEx::CloseWindow(TRUE);
		g_UiInformation.Close();
		g_UiInformation2.Close();
		g_UiInformation3.Close();
		KUiTongInformation::CloseWindow(true);
		KUiMsgSel2::CloseWindow(true);
		KUiNotification::CloseWindow(true);
	}
	if (MapGetMode() != MINIMAP_M_BRIEF_NOT_PIC && 
		MapGetMode() != MINIMAP_M_NONE)
		MapSetMode(MINIMAP_M_BRIEF_PIC);
	KUiFaceSelector::CloseWindow(bAll);
	KUiStatus::CloseWindow(bAll);
	if(!g_pCoreShell->GetGameData(GDI_PLAYER_OPEN_SHOP, 0, 0)) // fix ban hang di chuyen by kinnox;
	KUiItem::CloseWindow(bAll);
	KUiSkills::CloseWindow(bAll);
	KUiSkillTree::CloseWindow(bAll);
	KUiOptions::CloseWindow();
	KUiChatStatus::CloseWindow();
	KUiStoreBox::CloseWindow();
	KUiTeamManage::CloseWindow();
	KUiTradeConfirm::CloseWindow(bAll);
	KUiShop::CloseWindow();
	KUiSelPlayerNearby::CloseWindow(bAll);
	KUiHelper::CloseWindow(bAll);
	KUiHelper2::CloseWindow(bAll);
	KUiTaskNote::CloseWindow(bAll);
	KUiParadeItem::CloseWindow(bAll);
	KUiStrengthRank::CloseWindow();
	KUiTongManager::CloseWindow(bAll);
	KUiTongCreateSheet::CloseWindow();
	KUiFindPos::CloseWindow();
	KUiChatItemInfo::CloseWindow();
	KUiChooseFace::CloseWindow(); //Player Avatar by kinnox;
	KUiAutoPlay::CloseWindow(bAll); //AutoAI by kinnox;
	KUiCreatPW::CloseWindow();//LockPlayer by kinnox;
	KUiLoginPW::CloseWindow();//LockPlayer by kinnox;
	KUiChaPW::CloseWindow();//LockPlayer by kinnox;
	KUiKeyBoard::CloseWindow();//LockPlayer by kinnox;	
	KUiExpandItem::CloseWindow(); //ExPandBox by kinnox;	
	KUiGiveBox::CloseWindow(bAll);//GiveBox by kinnox;	
	KUiReWard::CloseWindow(bAll);//Reward by kinnox;
	KUiTopTKNew::CloseWindow();//Report Battle by kinnox;
	KUiNotification::CloseWindow(bAll);
	return true;
}
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiEndGame()
{
	KReconnectWnd::Exit(false);
	//g_UiBase.SavePrivateConfig();

	Wnd_DragFinished();
	Wnd_SwitchCursor(CURSOR_NORMAL);
	g_NetConnectAgent.DisconnectClient();
	s_UiLiveSeed = UI_LIVING_S_OUTGAME;

	KShortcutKeyCentre::Enable(false);

	UiCloseWndsInGame(true);
	Wnd_ShowHideGameSpace(false);
	Wnd_GameSpaceHandleInput(false);
	KUiLoginBackGround::OpenWindow("Init");
	g_UiBase.SetStatus(UIS_S_IDLE);

	g_UiBase.NotifyEvent(APP_EXITGAME);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiSetScreenSize(int nWidth, int nHeight)
{
	Wnd_SetScreenSize(nWidth, nHeight);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiOnGameServerConnected()
{
	g_UiBase.SavePrivateConfig();

	if (g_pCoreShell)
	g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, 0, 0);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void UiOnGameServerStartSyncEnd()
{
	KUiChatCentre::OpenWindow(false);
	KUiChatCentre::CreateSeverUnit();
	g_UiBase.LoadPrivateConfig();
	if (g_pCoreShell)
	g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_COMCLASS(Player_Life)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool Player_Life::m_bText = true;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Life::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

		Info.nLife = max(Info.nLife, 0);
		Info.nLifeFull = max(Info.nLifeFull, 0);

		Set2IntValue(Info.nLife, Info.nLifeFull);
		if (m_bText)
			Set2IntText(Info.nLife, Info.nLifeFull, '/');
		else
			SetText(NULL, 0);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Life::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SHOWPLAYERNUMBER);
	UpdateData();
}


IMPLEMENT_COMCLASS(Player_Mana)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool Player_Mana::m_bText = true;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Mana::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

		Info.nMana = max(Info.nMana, 0);
		Info.nManaFull = max(Info.nManaFull, 0);

		Set2IntValue(Info.nMana, Info.nManaFull);
		
		if (m_bText)
			Set2IntText(Info.nMana, Info.nManaFull, '/');
		else
			SetText(NULL, 0);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Mana::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SHOWPLAYERNUMBER);
	UpdateData();
}


IMPLEMENT_COMCLASS(Player_Stamina)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool Player_Stamina::m_bText = true;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Stamina::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

		Info.nStamina = max(Info.nStamina, 0);
		Info.nStaminaFull = max(Info.nStaminaFull, 0);

		Set2IntValue(Info.nStamina, Info.nStaminaFull);

		if (m_bText)
			Set2IntText(Info.nStamina, Info.nStaminaFull, '/');
		else
			SetText(NULL, 0);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Stamina::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SHOWPLAYERNUMBER);
	UpdateData();
}


IMPLEMENT_COMCLASS(Player_Exp)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
bool Player_Exp::m_bText = true;

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Exp::UpdateData()
{
	int nFull, nCurrLevelExp, nCurrent;

	if (KUiPlayerBar::GetExp(nFull, nCurrLevelExp, nCurrent))
	{	
		if(g_pCoreShell)
		{
			KUiPlayerAttribute	Info;

			memset(&Info, 0, sizeof(KUiPlayerAttribute));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);

			float np = 100;
			if (nCurrLevelExp != 0)
				np = ((nCurrent - nFull + nCurrLevelExp) * 100.0) / nCurrLevelExp;
			np = max(np, -100);
			np = min(np, 100);
			Set2IntValue(np, 100);

			if (m_bText)
			{
				char szExp[32];
				int nLen = 0;

				if (Info.nLevel >= 80)
				{
					nLen = sprintf(szExp, "%0.2f%%", np);
				}
				else
				{
					nLen = sprintf(szExp, "%0.0f%%", np);
				}
				
				SetText(szExp, nLen);
			}
			else
				SetText(NULL, 0);
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Exp::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SHOWPLAYERNUMBER);
	UpdateData();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int Player_Exp::GetToolTipInfo(char* szTip, int nMax)
{
	int		nFull, nCurrLevelExp, nCurrent;
	int		nLen = 0;
	if (KUiPlayerBar::GetExp(nFull, nCurrLevelExp, nCurrent) && szTip && nMax > 0)
	{
		char	szBuffer[64];
		nLen = sprintf(szBuffer, "Kinh nghi÷m: %d/%d", nCurrent, nFull);
		if (nLen <= nMax)
			memcpy(szTip, szBuffer, nLen);
		else
			nLen = 0;
	}
	return nLen;
}

IMPLEMENT_COMCLASS(Player_Level)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Level::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerAttribute	Info;
		memset(&Info, 0, sizeof(KUiPlayerAttribute));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
		SetIntText(Info.nLevel, 0);
	}
}

IMPLEMENT_COMCLASS(Player_WorldSort)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_WorldSort::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerBaseInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (unsigned int)&Info, 0);
		SetWorldRankText(Info.nRankInWorld, 0);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_WorldSort::OnButtonClick()
{
	KUiStrengthRank::OpenDefaultWindow();
}

IMPLEMENT_COMCLASS(Player_Status)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Status::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_STATUS);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Status::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_STATUS)));
}

IMPLEMENT_COMCLASS(Player_Items)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Items::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_ITEMS);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Items::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_ITEMS)));
}

IMPLEMENT_COMCLASS(Player_ItemEx)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_ItemEx::OnButtonClick()
{	
	/*if(g_pCoreShell)
	{
		g_pCoreShell->SystemMessages(GSM_FEATURE_BUIDING, 0, 0);
	}*/
	if(g_pCoreShell)
	{
		KUiExpandItem::OpenWindow(); //ExPandBox by kinnox;
	}
}


IMPLEMENT_COMCLASS(Player_Skills)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Skills::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SKILLS);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Skills::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_SKILLS)));
}


IMPLEMENT_COMCLASS(Player_Team)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Team::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TEAM);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Team::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_TEAM)));
}

IMPLEMENT_COMCLASS(Player_Faction)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Faction::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TONG);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Faction::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_TONG)));
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------


IMPLEMENT_COMCLASS(Player_Sit)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Sit::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SIT);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Sit::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
		CheckButton(Info.byAction & PA_SIT);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Sit::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_SIT)));
}


IMPLEMENT_COMCLASS(Player_Run)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Run::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_RUN);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Run::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
		CheckButton(Info.byAction & PA_RUN);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Run::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_RUN)));
}

IMPLEMENT_COMCLASS(Player_Horse)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Horse::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_HORSE);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Horse::UpdateData()
{
	if (g_pCoreShell)
	{
		KUiPlayerRuntimeInfo	Info;
		memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
		g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
		CheckButton(Info.byAction & PA_RIDE);
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Horse::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_HORSE)));
}

IMPLEMENT_COMCLASS(Player_Exchange)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Exchange::OnButtonClick()
{
	KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TRADE);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Exchange::UpdateData()
{
	if (g_pCoreShell)
	{
		CheckButton(g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_WILLING, 0));
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_Exchange::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_TRADE)));
}

IMPLEMENT_COMCLASS(Player_PK)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_PK::OnButtonClick()
{	
	int nX, nY;
	Wnd_GetCursorPos(&nX, &nY);
	KUiPlayerBar::PopupPKMenu(nX, nY);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_PK::UpdateData()
{
	if (g_pCoreShell)
	{
		SetFrame(g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0));
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
const char*	Player_PK::GetShortKey()
{
	return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
		KShortcutKeyCentre::FindCommandByScript(SCK_SHORTCUT_PK)));
}


IMPLEMENT_COMCLASS(Player_Task)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Task::OnButtonClick()
{
	if(g_pCoreShell)
	{
#ifdef _DEBUG
		g_pCoreShell->Debug(DEBUG_SHOWINFO, 0, 0);
#endif
		g_pCoreShell->SystemMessages(GSM_FEATURE_BUIDING, 0, 0);
	}
}
IMPLEMENT_COMCLASS(Player_AutoPlay)//AutoAI by kinnox;
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_AutoPlay::OnButtonClick()//AutoAI by kinnox;
{
	KUiAutoPlay::OpenWindow();
}
IMPLEMENT_COMCLASS(Player_Lock)
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void Player_Lock::OnButtonClick()
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
			/* if KUiCreatPW  is Open, then run this function  */
		}
		else
		{
			KUiLoginPW::OpenWindow();
		}
	}
	/*else
	{
		UIMessageBox("C∏c hπ ch≠a thi’t lÀp mÀt kh»u b∂o v÷ nh©n vÀt!",this,"Tho∏t");
	}*/
	else if (!Check.x && Check.y)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_LOCK_PWD,TRUE,0);
	}
	else
	{
		UIMessageBox("C∏c hπ ch≠a thi’t lÀp mÀt kh»u b∂o v÷ nh©n vÀt!",this,"Tho∏t");
	}
}