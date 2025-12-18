
#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "KPlayerDef.h"
#include "../elem/wnds.h"
#include "../elem/wndmessage.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../Engine/src/KFilePath.h"
#include "UiPopupPasswordQuery.h"
#include "UiTongAssignBox.h"
#include "UiTongManager.h"
#include "UiTongInformation.h"
#include "UiInformation.h"
#include "UiTongChangeTitle.h"
#include "UiGetMoney.h"
#include "UiTongCreateSheet.h"

#include "KEngine.h"

extern iCoreShell* g_pCoreShell;
extern KUiTongInformation g_UiTongInformation;

#define TONG_MANAGER_INI		"UiTongManager.ini"
#define	TONG_INFO_INI			"UiTongInfo.ini"
#define	TONG_FUNUSE_INI			"UiTongFunUse.ini"
#define	TONG_RIGHTMANAGE_INI	"UiTongRightManage.ini"
#define	TONG_RECORD_INI			"UiTongRecord.ini"

#define	TONG_DATA_TEMP_FILE				"TongTempData.ini"
#define	TONG_REQUEST_TIME_INTERVAL		30000
KUiTongManager* KUiTongManager::ms_pSelf = NULL;

KTongInfo       KUiTongManager::m_TongData;
int             KUiTongManager::m_nElderDataIndex = 0;
int             KUiTongManager::m_nMemberDataIndex = 0;
int             KUiTongManager::m_nCaptainDataIndex = 0;


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongManager::KUiTongManager()
{	
	m_uLockTime				= 0;
	m_bLockOperation		= false;
	m_btCurPad				= 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongManager::~KUiTongManager()
{

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongManager* KUiTongManager::OpenWindow()
{
	if(g_pCoreShell)
	{
    	if (ms_pSelf == NULL)
    	{
		    ms_pSelf = new KUiTongManager;
		    if (ms_pSelf)
    			ms_pSelf->Initialize();
    	}
    	if (ms_pSelf)
    	{
		    UiSoundPlay(UI_SI_WND_OPENCLOSE);
			ms_pSelf->BringToTop();
			ms_pSelf->Show();
	    }
	}
	return ms_pSelf;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongManager* KUiTongManager::OpenWindow(char* pszPlayerName)
{
	OpenWindow();
	if(ms_pSelf)
	{
		ms_pSelf->ArrangeComposition(pszPlayerName);
	}
	return ms_pSelf;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ArrangeComposition(char* pszPlayerName)
{
	if(g_pCoreShell)
	{
    	KUiPlayerItem Player;
		int nKind;
		
    	memset(&Player, 0, sizeof(KUiPlayerItem));
    	if(pszPlayerName && pszPlayerName[0])
    	{
			if(g_pCoreShell->FindSpecialNPC(pszPlayerName, &Player, nKind))
				m_nIndex = Player.nIndex;
	    }
	    else
	    {
    		KUiPlayerBaseInfo Me;
		    g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (unsigned int)&Me, 0);
			if(g_pCoreShell->FindSpecialNPC(Me.Name, &Player, nKind))
				m_nIndex = Player.nIndex;
	    }
		if(m_nIndex >= 0)
		{
			strcpy(m_szPlayerName, Player.Name);
			UpdateBtnTheme(0, TRUE);

			g_pCoreShell->TongOperation(GTOI_REQUEST_PLAYER_TONG, (unsigned int)&Player, 0);
			//
			m_FunUsePad.ArrangeComposition(Player.Name, m_nIndex);
			ShowPage((TONG_PAGE)m_btCurPad);
		}
		else
		{
			CloseWindow();
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ClearTongData()
{	
	m_uLockTime		= 0;
	m_bLockOperation = false;
	m_btCurPad = 0;
	m_MemberData.Clear();
	m_nElderDataIndex	= 0;
	m_nCaptainDataIndex	= 0;
	m_nMemberDataIndex	= 0;
	m_BaseInfoPad.Clear();
	m_FunUsePad.Clear();
	m_RightManagePad.Clear();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongManager* KUiTongManager::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		ms_pSelf->Hide();
		if (bDestory)
		{	
			ms_pSelf->ClearTongData();
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongManager::ShowPage(TONG_PAGE ePage)
{	
	m_btCurPad = (BYTE)ePage;
	m_BtnBaseInfo.CheckButton(false);
/*
	m_BtnZhaoMu.CheckButton(false);
	m_BtnOtherZhaoMu.CheckButton(false);
	m_BtnWorkshop.CheckButton(false);
*/
	m_BtnFunUse.CheckButton(false);
	m_BtnRightManage.CheckButton(false);
	m_BtnTongRecord.CheckButton(false);

	m_BaseInfoPad.Hide();
	m_FunUsePad.Hide();
	m_RightManagePad.Hide();
	m_RecordPad.Hide();

	switch(ePage)
	{
	case Info_Page:
		m_BaseInfoPad.Show();
		m_BtnBaseInfo.CheckButton(true);
		break;
/*	case ZhaoMu_Page:
		m_BtnZhaoMu.CheckButton(true);
		break;
	case OtherZhaoMu_Page:
		m_BtnOtherZhaoMu.CheckButton(true);
		break;
	case Workshop_Page:
		m_BtnWorkshop.CheckButton(true);
		break;
*/
	case FunUse_Page:
		m_FunUsePad.Show();
		m_BtnFunUse.CheckButton(true);
		break;
	case RightManage_Page:
		m_RightManagePad.Show();
		m_BtnRightManage.CheckButton(true);
		break;
	case TongRecord_Page:
		m_RecordPad.Show();
		m_BtnTongRecord.CheckButton(true);
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::UpdateBtnTheme(int nType, BOOL IsDissable)
{
/*	m_BtnZhaoMu.Enable(false);
	m_BtnOtherZhaoMu.Enable(false);
	m_BtnWorkshop.Enable(false);
*/
	m_BtnFunUse.Enable(false);
	m_BtnRightManage.Enable(false);
	m_BtnTongRecord.Enable(false);

	m_BtnRefresh.Enable(false);

	if(IsDissable)
	{
		return;
	}

	switch(nType)
	{
	case enumTONG_FIGURE_MASTER:
		m_BtnBaseInfo.Enable(true);
/*		m_BtnOtherZhaoMu.Enable(true);
		m_BtnWorkshop.Enable(true);
*/
		m_BtnFunUse.Enable(true);
//		m_BtnRightManage.Enable(true);
	    m_BtnTongRecord.Enable(true);
		break;

	case enumTONG_FIGURE_DIRECTOR:
		m_BtnBaseInfo.Enable(true);
/*		m_BtnWorkshop.Enable(true);
*/
		m_BtnFunUse.Enable(true);
/*		m_BtnRightManage.Enable(true);
*/
	    m_BtnTongRecord.Enable(true);
		break;

	case enumTONG_FIGURE_MANAGER:
		m_BtnBaseInfo.Enable(true);
/*		m_BtnWorkshop.Enable(true);
*/
		m_BtnFunUse.Enable(true);
	    m_BtnTongRecord.Enable(true);
		break;

	case enumTONG_FIGURE_MEMBER:
		m_BtnBaseInfo.Enable(true);
		m_BtnFunUse.Enable(true);
	    m_BtnTongRecord.Enable(true);
		break;

	default:
		m_BtnBaseInfo.Enable(true);
		break;
	}

	m_BtnRefresh.Enable(true);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::Initialize()
{	
	AddChild(&m_BtnBaseInfo);
	m_BaseInfoPad.Initialize();
	AddPage(&m_BaseInfoPad, &m_BtnBaseInfo);

/*	AddChild(&m_BtnZhaoMu);
	AddChild(&m_BtnOtherZhaoMu);
	AddChild(&m_BtnWorkshop);
*/

	AddChild(&m_BtnFunUse);
	m_FunUsePad.Initialize();
	AddPage(&m_FunUsePad, &m_BtnFunUse);

/*	AddChild(&m_BtnRightManage);
	m_RightManagePad.Initialize();
	AddPage(&m_RightManagePad, &m_BtnRightManage);

	AddChild(&m_BtnTongRecord);
	m_RecordPad.Initialize();
	AddPage(&m_RecordPad, &m_BtnTongRecord);*/

	AddChild(&m_BtnTongHelp);
	
	AddChild(&m_BtnEnterMap);
	AddChild(&m_BtnTongList);
	AddChild(&m_BtnRefresh);
	AddChild(&m_BtnClose);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, TONG_MANAGER_INI);

		if(Ini.Load(Buff))
		{
			ms_pSelf->Init(&Ini, "Main");

			ms_pSelf->m_BtnBaseInfo.Init(&Ini, "BtnBaseInfo");
/*	        ms_pSelf->m_BtnZhaoMu.Init(&Ini, "BtnZhaoMu");
			ms_pSelf->m_BtnOtherZhaoMu.Init(&Ini, "BtnOtherZhaoMu");
	        ms_pSelf->m_BtnWorkshop.Init(&Ini, "BtnWorkshop");
*/
			ms_pSelf->m_BtnFunUse.Init(&Ini, "BtnFunUse");
	        ms_pSelf->m_BtnRightManage.Init(&Ini, "BtnRightManage");
			ms_pSelf->m_BtnTongRecord.Init(&Ini, "BtnTongRecord");
	        ms_pSelf->m_BtnTongHelp.Init(&Ini, "BtnTongHelp");
			
			ms_pSelf->m_BtnEnterMap.Init(&Ini, "BtnEnterMap");
			ms_pSelf->m_BtnTongList.Init(&Ini, "BtnTongList");
			ms_pSelf->m_BtnRefresh.Init(&Ini, "BtnRefresh");
			ms_pSelf->m_BtnClose.Init(&Ini, "BtnClose");
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::Breathe()
{
	if(m_bLockOperation)
	{
		if(IR_GetCurrentTime() -  m_uLockTime >= TONG_REQUEST_TIME_INTERVAL)
		{
			m_bLockOperation = false;
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongManager::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnBaseInfo)
		{
			ShowPage(Info_Page);
		}
/*		else if(uParam == (unsigned int)&m_BtnZhaoMu)
		{
			ShowPage(ZhaoMu_Page);
		}
		else if(uParam == (unsigned int)&m_BtnOtherZhaoMu)
		{
			ShowPage(OtherZhaoMu_Page);
		}
		else if(uParam == (unsigned int)&m_BtnWorkshop)
		{
			ShowPage(Workshop_Page);
		}
*/
		else if(uParam == (unsigned int)&m_BtnFunUse)
		{
			ShowPage(FunUse_Page);
		}
		else if(uParam == (unsigned int)&m_BtnRightManage)
		{
			ShowPage(RightManage_Page);
		}
		else if(uParam == (unsigned int)&m_BtnTongRecord)
		{
			ShowPage(TongRecord_Page);
		}
		else if(uParam == (unsigned int)&m_BtnClose)
		{
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_BtnRefresh)
		{
			OnRefresh();
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		break;
	default:
		return KWndPageSet::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::TongInfoArrive(KUiPlayerRelationWithOther* Relation, KTongInfo *pTongInfo)
{
	if(ms_pSelf && !strcmp(Relation->Name, ms_pSelf->m_szPlayerName))
	{	
		memcpy(&ms_pSelf->m_TongData, pTongInfo, sizeof(KTongInfo));

		ms_pSelf->UpdateBtnTheme(Relation->nRelation);
		ms_pSelf->m_BaseInfoPad.InfoArrive(pTongInfo);
		ms_pSelf->m_FunUsePad.InfoArrive(pTongInfo, Relation->nRelation);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::NewDataArrive(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome)
{	
	KLinkStruct<KPilgarlicItem> *pData;
	int *pNum, nCount;
	
	pData = &ms_pSelf->m_MemberData;
	if(ParamInfo->nData == enumTONG_FIGURE_DIRECTOR)
	{
		pNum = &m_nElderDataIndex;
		nCount = m_TongData.nDirectorCount;
	}
	else if(ParamInfo->nData == enumTONG_FIGURE_MANAGER)
	{
		pNum = &m_nCaptainDataIndex;
		nCount = m_TongData.nManagerCount;
	}
	else if(ParamInfo->nData == enumTONG_FIGURE_MEMBER)
	{
		pNum = &m_nMemberDataIndex;
		nCount = m_TongData.nMemberCount;
	}
	else
	{
		return;
	}
	
	bool bCLoad = false;
	if(*pNum == nCount)
	{	
		bCLoad = true;
	}

	if(ParamInfo->uParam && !bCLoad)
	{
		*pNum += ParamInfo->uParam;
		if(!strcmp(m_TongData.szName, ParamInfo->szName))
		{
			KPilgarlicItem Add;
			for(int i = 0;i < (int)ParamInfo->uParam;i++)
			{
				if(!pIncome[i].Name[0])
				{
					continue;
				}
				memcpy(&Add, pIncome + i, sizeof(KTongMemberItem));
				Add.nHashID = StringToHash(Add.Name);
				pData->AddItem(Add);
			}
			//
			ms_pSelf->m_BaseInfoPad.UpdateListView();
			ms_pSelf->m_FunUsePad.UpdateListView();
			ms_pSelf->m_RightManagePad.UpdateListView();
		}
				
		SaveNewData(ParamInfo, pIncome);
		if(*pNum < nCount)
		{
			RequestData(ParamInfo->nData, *pNum, nCount - *pNum);
		}
		else
		{
			bCLoad = true;
		}
	}
	
	if(ParamInfo->nData == enumTONG_FIGURE_DIRECTOR)
	{	
		if(bCLoad)
		{
			if(m_TongData.nManagerCount > 0)
			{	
				if(m_nCaptainDataIndex == 0)
				{
					RequestData(enumTONG_FIGURE_MANAGER, 0, defTONG_ONE_PAGE_MAX_NUM);
					return;
				}	
			}
			else if(m_TongData.nMemberCount > 0)
			{
				if(m_nMemberDataIndex == 0)
				{
					RequestData(enumTONG_FIGURE_MEMBER, 0, defTONG_ONE_PAGE_MAX_NUM);
					return;
				}
			}

			if(m_nCaptainDataIndex != m_TongData.nManagerCount)
			{
				ms_pSelf->RemoveTempFile();
				ms_pSelf->ClearTongData();
				ms_pSelf->ArrangeComposition(NULL);
				return;
			}
			if(m_nMemberDataIndex != m_TongData.nMemberCount)
			{
				ms_pSelf->RemoveTempFile();
				ms_pSelf->ClearTongData();
				ms_pSelf->ArrangeComposition(NULL);
				return;
			}
			LoadData();
		}
	}
	else if(ParamInfo->nData == enumTONG_FIGURE_MANAGER)
	{
		if(bCLoad)
		{
			if(m_nMemberDataIndex == 0 && m_TongData.nMemberCount > 0)
			{
				RequestData(enumTONG_FIGURE_MEMBER, 0, defTONG_ONE_PAGE_MAX_NUM);
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::SaveNewData(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome)
{	
	KIniFile Ini;
	char szSection[32], szKey[32], szFileName[80];
	int nCount, nDataCount;
	
	g_UiBase.GetUserPrivateDataFolder(szFileName, sizeof(szFileName));
	strcat(szFileName, TONG_DATA_TEMP_FILE);

	if(ParamInfo->nData == enumTONG_FIGURE_DIRECTOR)
	{
		sprintf(szSection, "%s_Director", ParamInfo->szName);
	}
	else if(ParamInfo->nData == enumTONG_FIGURE_MANAGER)
	{
		sprintf(szSection, "%s_Manager", ParamInfo->szName);
	}
	else if(ParamInfo->nData == enumTONG_FIGURE_MEMBER)
	{
		sprintf(szSection, "%s_Member", ParamInfo->szName);
	}

	Ini.Load(szFileName);
	Ini.GetInteger(szSection, "Count", 0, &nCount);
	nDataCount = nCount + ParamInfo->uParam;
	Ini.WriteInteger(szSection, "Count", nDataCount);
	for(int i = nCount;i < nDataCount;i++)
	{	
		sprintf(szKey, "%d_Name", i);
		Ini.WriteString(szSection, szKey, pIncome[i - nCount].Name);

		sprintf(szKey, "%d_AgName", i);
		Ini.WriteString(szSection, szKey, pIncome[i - nCount].szAgname);

		sprintf(szKey, "%d_Status", i);
		Ini.WriteInteger(szSection, szKey, pIncome[i - nCount].bStatus);

		sprintf(szKey, "%d_Figure", i);
		Ini.WriteInteger(szSection, szKey, pIncome[i - nCount].nData);
	
		sprintf(szKey, "%d_Hash", i);
		Ini.WriteInteger(szSection, szKey, StringToHash(pIncome[i - nCount].Name));
	}
	Ini.Save(szFileName);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::SaveWholeData(int nType)
{
	if(ms_pSelf && ms_pSelf->m_TongData.szName[0])
	{
    	KLinkStruct<KPilgarlicItem> *pData = NULL;
		char szSection[32];
		pData = &ms_pSelf->m_MemberData;
		switch(nType)
		{
		case enumTONG_FIGURE_DIRECTOR:
			sprintf(szSection, "%s_Director", ms_pSelf->m_TongData.szName);
			break;
		case enumTONG_FIGURE_MANAGER:
			sprintf(szSection, "%s_Manager", ms_pSelf->m_TongData.szName);
			break;
		case enumTONG_FIGURE_MEMBER:
			sprintf(szSection, "%s_Member", ms_pSelf->m_TongData.szName);
			break;
		}
		if(pData)
		{
			KPilgarlicItem *pNode = pData->Reset();
			int nCount = pData->GetCount();
			char szKey[32], szFileName[80];
			KIniFile Ini;
			
			g_UiBase.GetUserPrivateDataFolder(szFileName, sizeof(szFileName));
			strcat(szFileName, TONG_DATA_TEMP_FILE);
			
			Ini.Load(szFileName);
			Ini.EraseSection(szSection);
			int i = 0, j = 0;
			for(i = 0; i < nCount; i++)
			{		
				//g_DebugLog("Data: Figure:%d - Name: %s", pNode->nData, pNode->Name, nCount);
				if(pNode->nData != nType)
				{
					pNode = pData->NextItem();
					continue;
				}
				sprintf(szKey, "%d_Name", j);
			    Ini.WriteString(szSection, szKey, pNode->Name);

			    sprintf(szKey, "%d_AgName", j);
			    Ini.WriteString(szSection, szKey, pNode->szAgname);

				sprintf(szKey, "%d_Status", j);
			    Ini.WriteInteger(szSection, szKey, pNode->bStatus);

				sprintf(szKey, "%d_Figure", j);
			    Ini.WriteInteger(szSection, szKey, pNode->nData);

			    sprintf(szKey, "%d_Hash", j);
			    Ini.WriteInteger(szSection, szKey, pNode->nHashID);
				
				j++;
				pNode = pData->NextItem();
			}
			sprintf(szKey, "Count");
			Ini.WriteInteger(szSection, szKey, j);
			Ini.Save(szFileName);
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::LoadData()
{
	if(m_TongData.szName[0])
	{
    	char szSection[32];
		//
		ms_pSelf->m_MemberData.Clear();
		sprintf(szSection, "%s_Director", m_TongData.szName);
		m_nElderDataIndex = LoadDataHandler(&ms_pSelf->m_MemberData, szSection);
		sprintf(szSection, "%s_Manager", m_TongData.szName);
		m_nCaptainDataIndex = LoadDataHandler(&ms_pSelf->m_MemberData, szSection);
		sprintf(szSection, "%s_Member", m_TongData.szName);
		m_nMemberDataIndex = LoadDataHandler(&ms_pSelf->m_MemberData, szSection);
		//
		ms_pSelf->m_BaseInfoPad.UpdateListView();
		ms_pSelf->m_FunUsePad.UpdateListView();
		ms_pSelf->m_RightManagePad.UpdateListView();
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongManager::LoadDataHandler(KLinkStruct<KPilgarlicItem> *pData, const char* szSection)
{
	char szFileName[256];
	KIniFile Ini;

	g_UiBase.GetUserPrivateDataFolder(szFileName, sizeof(szFileName));
	strcat(szFileName, TONG_DATA_TEMP_FILE);
	if(Ini.Load(szFileName) && m_TongData.szName[0] && Ini.IsSectionExist(szSection))
	{	
		int nCount, i;
		char szKey[32];
		KPilgarlicItem AddItem;

		Ini.GetInteger(szSection, "Count", 0, &nCount);
		for(i = 0;i < nCount;i++)
		{
			sprintf(szKey, "%d_Name", i);
			Ini.GetString(szSection, szKey, "", AddItem.Name, sizeof(AddItem.Name));

			sprintf(szKey, "%d_AgName", i);
			Ini.GetString(szSection, szKey, "", AddItem.szAgname, sizeof(AddItem.szAgname));
			
			sprintf(szKey, "%d_Status", i);
			Ini.GetInteger(szSection, szKey, 0, (int*)&AddItem.bStatus);

			sprintf(szKey, "%d_Figure", i);
			Ini.GetInteger(szSection, szKey, 0, &AddItem.nData);

			sprintf(szKey, "%d_Hash", i);
			Ini.GetInteger(szSection, szKey, 0, &AddItem.nHashID);

			pData->AddItem(AddItem);
		}
		return nCount;
	}
	return 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KLinkStruct<KPilgarlicItem>*	KUiTongManager::GetMemberData()
{
	return &ms_pSelf->m_MemberData;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KLinkStruct<KPilgarlicItem>* KUiTongManager::FindPlayer(char *pszName)
{
	KLinkStruct<KPilgarlicItem> *pData = NULL;
	if(pszName && pszName[0])
	{
    	pData = ms_pSelf->HandleFind(&ms_pSelf->m_MemberData, pszName);
	}
	return pData;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KLinkStruct<KPilgarlicItem>* KUiTongManager::HandleFind(KLinkStruct<KPilgarlicItem> *pData, char *pszName)
{
	KLinkStruct<KPilgarlicItem> *pReturn = NULL;
	KPilgarlicItem *pNode = pData->Reset();
	int nID = StringToHash(pszName);
	int nCount = pData->GetCount();

	for(int i = 0;i < nCount;i++)
	{
		if(nID == pNode->nHashID)
		{
			if(strcmp(pszName, pNode->Name) == 0)
			{
				pReturn = pData;
				break;
			}
		}
		pNode = pData->NextItem();
	}
	return pReturn;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::RequestData(int nType, int nIndex, int nNeedNum)
{
	if(g_pCoreShell)
	{
    	KUiGameObjectWithName ParamInfo;
    	strcpy(ParamInfo.szName, m_TongData.szName);
    	ParamInfo.nData = nType;
    	ParamInfo.nParam = nIndex;
    	g_pCoreShell->TongOperation(GTOI_REQUEST_TONG_DATA, (unsigned int)&ParamInfo, nNeedNum);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongManager::RemoveTempFile()
{
	char szPath[256], szFile[128];
	
	g_UiBase.GetUserPrivateDataFolder(szFile, sizeof(szFile));
	strcat(szFile, TONG_DATA_TEMP_FILE);
	g_GetFullPath(szPath, szFile);
	return remove(szPath);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::LockOperation()
{	
	if(!ms_pSelf)
		return;

	if(ms_pSelf->m_bLockOperation)
		return;

	ms_pSelf->m_uLockTime = IR_GetCurrentTime();
	ms_pSelf->m_bLockOperation = true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool KUiTongManager::GetLockOperation()
{	
	if(!ms_pSelf)
		return false;

	return ms_pSelf->m_bLockOperation;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::LockInfomation()
{	
	UIMessageBox("Kh«ng thÓ thùc hiÖn thao t¸c liªn tôc!", NULL, "§ãng");
}


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseResult(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(ms_pSelf)
	{
    	switch(pResult->nData)
    	{
		case TONG_ACTION_LEAVE:
			ms_pSelf->ResponseLeave(pResult, nbIsSucceed);
		    break;
		case TONG_ACTION_DEMISE:
			ms_pSelf->ResponseDemise(pResult, nbIsSucceed);
    		break;
		case TONG_ACTION_APPLY:
			ms_pSelf->ResponseApply(pResult, nbIsSucceed);
			break;
    	case TONG_ACTION_KICKOUT:
			ms_pSelf->ResponseKickOut(pResult, nbIsSucceed);
		    break;
		case TONG_ACTION_ASSIGN:
			ms_pSelf->ResponseAssign(pResult, nbIsSucceed);
		    break;		
	    }
	}
	else
	{
		RemoveTempFile();
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseLeave(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(nbIsSucceed)
	{
    	RemoveTempFile();
    	CloseWindow(true);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseDemise(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(nbIsSucceed)
	{
		RemoveTempFile();
		ClearTongData();
		ArrangeComposition(NULL);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseApply(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(nbIsSucceed)
	{	
		RemoveTempFile();
		ClearTongData();
		ArrangeComposition(NULL);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseKickOut(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(nbIsSucceed)
	{
		if(strcmp(pResult->szName, m_szPlayerName) == 0)
		{
			RemoveTempFile();
			CloseWindow(true);
		}
		else
		{
			KLinkStruct<KPilgarlicItem> *pForDelete;
			pForDelete = FindPlayer(pResult->szName);
			if(pForDelete)
			{	
				switch(pResult->nParam)
				{
				case enumTONG_FIGURE_DIRECTOR:
					m_nElderDataIndex--;
					break;
				case enumTONG_FIGURE_MANAGER:
					m_nCaptainDataIndex--;
					break;
				case enumTONG_FIGURE_MEMBER:
					m_nMemberDataIndex--;
					break;
				}
				pForDelete->RemoveItem();
				SaveWholeData(pResult->nParam);
				ArrangeComposition(m_TongData.szMasterName);
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::ResponseAssign(KUiGameObjectWithName *pResult, int nbIsSucceed)
{
	if(nbIsSucceed)
	{
		KLinkStruct<KPilgarlicItem> *pPlayer = FindPlayer(pResult->szName);
		if(pPlayer)
		{	
			KPilgarlicItem Add, *pNode = pPlayer->Current();
			memcpy(&Add, pNode, sizeof(KPilgarlicItem));
			pPlayer->RemoveItem();
			strcpy(Add.szAgname, pResult->szString);
			Add.nData = pResult->nParam;
			m_MemberData.AddItem(Add);
			switch(pResult->uParam)
			{
			case enumTONG_FIGURE_DIRECTOR:
				m_nElderDataIndex--;
				break;
			case enumTONG_FIGURE_MANAGER:
				m_nCaptainDataIndex--;
				break;
			case enumTONG_FIGURE_MEMBER:
				m_nMemberDataIndex--;
				break;
			}
			switch(pResult->nParam)
			{
			case enumTONG_FIGURE_DIRECTOR:
				m_nElderDataIndex++;
				break;
			case enumTONG_FIGURE_MANAGER:
				m_nCaptainDataIndex++;
				break;
			case enumTONG_FIGURE_MEMBER:
				m_nMemberDataIndex++;
				break;
			}
			SaveWholeData(pResult->nParam);
			SaveWholeData(pResult->uParam);
			ArrangeComposition(m_TongData.szMasterName);
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongManager::OnRefresh()
{	
	if(GetLockOperation())
	{
		LockInfomation();
		return;
	}
	RemoveTempFile();
	ClearTongData();
	ArrangeComposition(NULL);
	LockOperation();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
unsigned long StringToHash(const char *pString, BOOL bIsCaseSensitive)
{
	if(pString && pString[0])
	{
		unsigned long id = 0;
		const char *ptr;
		int index = 0;

		if(bIsCaseSensitive)
		{
			ptr = pString;

			while(*ptr)
			{
    			id = (id + (++index) * (*ptr)) % 0x8000000b * 0xffffffef;
		        ptr++;
	        }
		}
		else
		{
			char Buff[256];
			strcpy(Buff, pString);
			_strlwr(Buff);
			ptr = Buff;

        	while(*ptr)
		    {
    			id = (id + (++index) * (*ptr)) % 0x8000000b * 0xffffffef;
		        ptr++;
	        }
		}
		return (id ^ 0x12345678);
	}

	return 0;
}

//==========================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongInfo::KUiTongInfo()
{
	m_btCurrentPage	= 0;
	m_btPageCount	= 0;
	m_bShowOnline	= false;;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongInfo::Initialize()
{
	AddChild(&m_TitleTongInfo);
	AddChild(&m_TitleTongName);
	AddChild(&m_TxtTongName);
	AddChild(&m_TitleMaster);
	AddChild(&m_TxtMaster);
	AddChild(&m_TitleLeague);
	AddChild(&m_TxtLeague);
	AddChild(&m_TitleCamp);
	AddChild(&m_TxtCamp);

/*	AddChild(&m_TitleBuildLevel);
	AddChild(&m_TxtBuildLevel);
*/
	AddChild(&m_TitleMemberNum);
	AddChild(&m_TxtMemberNum);
	AddChild(&m_TitleTongLevel);
	AddChild(&m_TxtTongLevel);
/*	AddChild(&m_TitleStandFund);
	AddChild(&m_TxtStandFund);
*/
	AddChild(&m_TitleTongCapital);
	AddChild(&m_TxtTongCapital);

/*	AddChild(&m_TitleBuildFund);
	AddChild(&m_TxtBuildFund);
	AddChild(&m_TitleBattleFund);
	AddChild(&m_TxtBattleFund);
	AddChild(&m_TitleTotalOffer);
	AddChild(&m_TxtTotalOffer);
	AddChild(&m_TitleStoredBuildFund);
	AddChild(&m_TxtStoredBuildFund);
*/

/*	AddChild(&m_TitlePersonalInfo);
	AddChild(&m_TitlePersonalOffer);
	AddChild(&m_TxtPersonalOffer);
	AddChild(&m_TitleWeeklyOffer);
	AddChild(&m_TxtWeeklyOffer);
*/

/*	AddChild(&m_TxtHelpTitle);
	AddChild(&m_TxtHelp);
*/

	AddChild(&m_TxtRank);
	AddChild(&m_TxtTitle);
	AddChild(&m_TxtType);
	
	AddChild(&m_MemberList);

	AddChild(&m_BtnPrevPage);
	AddChild(&m_BtnNextPage);
	AddChild(&m_BtnJump);
	AddChild(&m_EditBoxDestPage);
	AddChild(&m_BtnOnlinePriority);
	
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongInfo::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_INFO_INI);
	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");
		m_TitleTongInfo.Init(&Ini, "TitleTongInfo");
		m_TitleTongName.Init(&Ini, "TitleTongName");
		m_TxtTongName.Init(&Ini, "TxtTongName");
		m_TitleMaster.Init(&Ini, "TitleMaster");
		m_TxtMaster.Init(&Ini, "TxtMaster");
		m_TitleLeague.Init(&Ini, "TitleLeague");
		m_TxtLeague.Init(&Ini, "TxtLeague");
		m_TitleCamp.Init(&Ini, "TitleCamp");
		m_TxtCamp.Init(&Ini, "TxtCamp");
/*		m_TitleBuildLevel.Init(&Ini, "TitleBuildLevel");
		m_TxtBuildLevel.Init(&Ini, "TxtBuildLevel");
*/
		m_TitleMemberNum.Init(&Ini, "TitleMemberNum");
		m_TxtMemberNum.Init(&Ini, "TxtMemberNum");
		m_TitleTongLevel.Init(&Ini, "TitleTongLevel");
		m_TxtTongLevel.Init(&Ini, "TxtTongLevel");
/*		m_TitleStandFund.Init(&Ini, "TitleStandFund");
		m_TxtStandFund.Init(&Ini, "TxtStandFund");
*/
		m_TitleTongCapital.Init(&Ini, "TitleTongCapital");
		m_TxtTongCapital.Init(&Ini, "TxtTongCapital");
/*		m_TitleBuildFund.Init(&Ini, "TitleBuildFund");
		m_TxtBuildFund.Init(&Ini, "TxtBuildFund");
		m_TitleBattleFund.Init(&Ini, "TitleBattleFund");
		m_TxtBattleFund.Init(&Ini, "TxtBattleFund");
		m_TitleTotalOffer.Init(&Ini, "TitleTotalOffer");
		m_TxtTotalOffer.Init(&Ini, "TxtTotalOffer");
		m_TitleStoredBuildFund.Init(&Ini, "TitleStoredBuildFund");
		m_TxtStoredBuildFund.Init(&Ini, "TxtStoredBuildFund");
*/

/*		m_TitlePersonalInfo.Init(&Ini, "TitlePersonalInfo");
		m_TitlePersonalOffer.Init(&Ini, "TitlePersonalOffer");
		m_TxtPersonalOffer.Init(&Ini, "TxtPersonalOffer");
		m_TitleWeeklyOffer.Init(&Ini, "TitleWeeklyOffer");
		m_TxtWeeklyOffer.Init(&Ini, "TxtWeeklyOffer");
*/

/*		m_TxtHelpTitle.Init(&Ini, "TxtHelpTitle");
		m_TxtHelp.Init(&Ini, "TxtHelp");
*/

		m_TxtRank.Init(&Ini, "TxtRank");
		m_TxtTitle.Init(&Ini, "TxtTitle");
		m_TxtType.Init(&Ini, "TxtType");
		
		m_MemberList.Init(&Ini, "MemberList");

		m_BtnPrevPage.Init(&Ini, "BtnPrevPage");
		m_BtnNextPage.Init(&Ini, "BtnNextPage");
		m_BtnJump.Init(&Ini, "BtnJump");
		m_EditBoxDestPage.Init(&Ini, "EditBoxDestPage");
		m_BtnOnlinePriority.Init(&Ini, "BtnOnlinePriority");

		Ini.GetString("Main", "StringJustice", "", m_szJustice, sizeof(m_szJustice));
		Ini.GetString("Main", "StringBalance", "", m_szBalance, sizeof(m_szBalance));
		Ini.GetString("Main", "StringEvil", "", m_szEvil, sizeof(m_szEvil));
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongInfo::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnOnlinePriority)
		{	
			OnShowOnline((BOOL)nParam);
		}
		else if(uParam == (unsigned int)&m_BtnPrevPage)
		{
			OnPrevPage();
		}
		else if(uParam == (unsigned int)&m_BtnNextPage)
		{
			OnNextPage();
		}
		else if(uParam == (unsigned int)&m_BtnJump)
		{
			OnJumpPage();
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::OnShowOnline(BOOL bShow)
{	
	return;
	if(bShow)
	{
		if(m_bShowOnline)
			return;
		m_bShowOnline = true;
	}
	else
	{
		if(!m_bShowOnline)
			return;
		m_bShowOnline = false;
	}
	this->UpdateListView(m_btCurrentPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::OnPrevPage()
{
	if(m_btCurrentPage <= 1)
		return;

	m_btCurrentPage--;
	UpdateListView(m_btCurrentPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::OnNextPage()
{
	if(m_btPageCount <= 1)
		return;
	if(m_btCurrentPage == m_btPageCount)
		return;

	m_btCurrentPage++;
	UpdateListView(m_btCurrentPage);
}	

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void KUiTongInfo::OnJumpPage()
{
	OnCheckInput();
	BYTE btPage = (BYTE)m_EditBoxDestPage.GetIntNumber();
	if(btPage < 1)
		btPage = 1;

	if(m_btCurrentPage == btPage)
		return;
	UpdateListView(btPage);
}

//--------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------
void KUiTongInfo::OnCheckInput()
{
	BYTE btPage = (BYTE)m_EditBoxDestPage.GetIntNumber();
	if (btPage < 0)
	{
		btPage = 0;
	}
	else if (btPage > m_btPageCount)
	{
		btPage = m_btPageCount;
	}
	char	szBuff1[16], szBuff2[16];
	_itoa((int)(btPage), szBuff1, 10);
	m_EditBoxDestPage.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_EditBoxDestPage.SetIntText((int)btPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::InfoArrive(KTongInfo *pTongInfo)
{	
	UpdateInfoView(pTongInfo);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::Clear()
{
	m_TxtTongName.SetLabel("", 0);
	m_TxtMaster.SetLabel("", 0);
	m_TxtCamp.SetLabel("", 0);
	m_TxtLeague.SetLabel("", 0);
	m_TxtTongLevel.SetLabel("", 0);
//	m_TxtBuildLevel.SetLabel("", 0);
	m_TxtMemberNum.SetLabel("", 0);
	m_TxtTongCapital.SetLabel("", 0);
	m_MemberList.ResetContent();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::UpdateInfoView(KTongInfo* pTongInfo)
{
	char Buff[16];

	m_TxtTongName.SetLabel(pTongInfo->szName, strlen(pTongInfo->szName));
	m_TxtMaster.SetLabel(pTongInfo->szMasterName, strlen(pTongInfo->szMasterName));
	switch(pTongInfo->nFaction)
	{
	case camp_justice:
		m_TxtCamp.SetLabel(m_szJustice, strlen(m_szJustice));
		break;

	case camp_balance:
		m_TxtCamp.SetLabel(m_szBalance, strlen(m_szBalance));
		break;
	case camp_evil:
		m_TxtCamp.SetLabel(m_szEvil, strlen(m_szEvil));
		break;
	}
	sprintf(Buff, "V« hÖ");
	m_TxtLeague.SetLabel(Buff, strlen(Buff));
	_itoa((int)pTongInfo->btLevel, Buff, 10);
	m_TxtTongLevel.SetLabel(Buff, strlen(Buff));
	/*_itoa(1, Buff, 10);
	m_TxtBuildLevel.SetLabel(Buff, strlen(Buff));*/
	int nMemberNum = pTongInfo->nMemberCount + pTongInfo->nManagerCount + pTongInfo->nDirectorCount;
	_itoa(nMemberNum, Buff, 10);
	m_TxtMemberNum.SetLabel(Buff, strlen(Buff));
	m_TxtTongCapital.SetMoneyLabel(pTongInfo->nMoney);
	
	m_btPageCount = (BYTE)(nMemberNum / defMAX_LINE_IN_ONE_PAGE);
	if((nMemberNum % defMAX_LINE_IN_ONE_PAGE))
		m_btPageCount++;
	if(m_btPageCount <= 1)
	{
		m_BtnPrevPage.Enable(false);
		m_BtnNextPage.Enable(false);
		m_BtnJump.Enable(false);
		m_EditBoxDestPage.Enable(false);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongInfo::UpdateListView(BYTE btPage /*= 1*/)
{
	KLinkStruct<KPilgarlicItem> *pData = KUiTongManager::GetMemberData();
	m_EditBoxDestPage.SetIntText(btPage);

	if(pData)
	{	
		m_MemberList.ResetContent();
    	int nDataCount = pData->GetCount();
		int nViewCount = (int)((btPage - 1) * defMAX_LINE_IN_ONE_PAGE);
		if(nDataCount > nViewCount)
		{
			KPilgarlicItem *pNode = pData->GetItem(nViewCount);
			if(pNode)
			{
				char Buff[3][32];
				int nRank = nViewCount;
				for(int i = nViewCount; i < nDataCount; i++)
				{	
					if(i >= nViewCount + defMAX_LINE_IN_ONE_PAGE)
						break;

					if(pNode->Name[0])
					{	
						if(m_bShowOnline)
						{
							if(!pNode->bStatus)
							{
								pNode = pData->NextItem();
								continue;
							}
						}
						nRank++;
						sprintf(Buff[0], "%d", nRank);
						sprintf(Buff[1], "%s", pNode->Name);
						sprintf(Buff[2], "%s", pNode->szAgname);
						m_MemberList.AddString(i, Buff[0], Buff[1], Buff[2], pNode->bStatus);
					}
					pNode = pData->NextItem();
				}
			}
		}
		m_btCurrentPage = btPage;
	}
}

//=============================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongFunUseSubPage::KUiTongFunUseSubPage()
{

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUseSubPage::Initialize(int nSub)
{	
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme, nSub);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUseSubPage::LoadScheme(const char* pScheme, int nSub)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_FUNUSE_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "ImgSubPageMask");
		char szKey[16];
		sprintf(szKey, "ImgSubPage%d", nSub);
		Ini.GetString("ImgSubPageMask", szKey, "", Buff, sizeof(Buff));
		SetImage(ISI_T_SPR, Buff);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongFunUseSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		break;
	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//=============================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongFunUseSub::KUiTongFunUseSub()
{	
	m_szPlayerName[0]	= 0;
	m_nIndex			= 0;

	m_btCurrentPage		= 0;
	m_btPageCount		= 0;
	m_bShowOnline		= false;

	m_nSelectIndex		= -1;
	m_szTargetPlayerName[0] = 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUseSub::Initialize()
{	
	AddChild(&m_TitleTongInfo);
	AddChild(&m_TitleTongName);
	AddChild(&m_TxtTongName);
	AddChild(&m_TitleTongUnion);
	AddChild(&m_TxtTongUnion);
/*	AddChild(&m_TitleBuildLevel);
	AddChild(&m_TxtBuildLevel);
	AddChild(&m_BtnUpgradeBuildLevel);
	AddChild(&m_TitleTotalOffer);
	AddChild(&m_TxtTotalOffer);
	AddChild(&m_BtnAssignTongOffer);
*/
	AddChild(&m_TitleTongMoney);
	AddChild(&m_TxtTongMoney);
/*	AddChild(&m_BtnTransformMoney);
	AddChild(&m_TitleBuildFund);
	AddChild(&m_TxtBuildFund);
	AddChild(&m_BtnTransformBuildFund);
	AddChild(&m_TitleBattleFund);
	AddChild(&m_TxtBattleFund);
	AddChild(&m_TitleStoredBuildFund);
	AddChild(&m_TxtStoredBuildFund);
*/

	AddChild(&m_TxtPersonalInfo);
/*	AddChild(&m_TitlePersonalOffer);
	AddChild(&m_TxtPersonalOffer);
	AddChild(&m_BtnStorePersonalOffer);
*/
	AddChild(&m_TitleTongMoney2);
	AddChild(&m_TxtTongMoney2);
/*	AddChild(&m_TitleBuildFund2);
	AddChild(&m_TxtBuildFund2);
	AddChild(&m_BtnStoreBuildFund);
*/
	AddChild(&m_BtnStoreTongMoney);
	
	for(int i = 0; i < 4; i++)
	{
		AddChild(&m_BtnSubPages[i]);
		m_SubPages[i].Initialize(i + 1);
		AddPage(&m_SubPages[i], &m_BtnSubPages[i]);
	}

/*	AddChild(&m_BtnForceToRetire);
*/
	AddChild(&m_BtnDemise);
	AddChild(&m_BtnKickOut);
	AddChild(&m_BtnDepose);

/*	AddChild(&m_Btn_DispenseOffer);
*/
	AddChild(&m_BtnChangeMaleTitle);
	AddChild(&m_BtnChangeFemaleTitle);
	AddChild(&m_BtnChangeTitle);
	AddChild(&m_BtnGetTongMoney);
	AddChild(&m_BtnAssignTongMoney);

	AddChild(&m_BtnChangeCamp);
	AddChild(&m_BtnCreateTongMap);
	AddChild(&m_BtnConfigureTongMap);
/*	AddChild(&m_BtnTongStunt);
	AddChild(&m_BtnTongChallenge);
	AddChild(&m_BtnTongDetect);
*/

/*	AddChild(&m_BtnCreateUnion);
	AddChild(&m_BtnApplyJionUnion);
	AddChild(&m_BtnAcceptUnionReq);
	AddChild(&m_BtnLeaveUnion);
	AddChild(&m_BtnKickUnionTong);
*/

	AddChild(&m_BtnLeaveTong);
/*	AddChild(&m_BtnRetire);
*/

	AddChild(&m_TxtRank);
	AddChild(&m_TxtTitle);
	AddChild(&m_TxtType);

	AddChild(&m_MemberList);
	AddChild(&m_BtnPrevPage);
	AddChild(&m_BtnNextPage);
	AddChild(&m_BtnJump);
	AddChild(&m_BtnOnlinePriority);
	AddChild(&m_EditBoxDestPage);

	

	
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::Clear()
{
	m_TxtTongName.SetLabel("", 0);
	m_TxtTongUnion.SetLabel("", 0);
	m_TxtTongMoney.SetLabel("", 0);
	//m_TxtBuildLevel.SetLabel("", 0);
	m_MemberList.ResetContent();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUseSub::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_FUNUSE_INI);
	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");
		m_oFixPos.x = 0;
		m_oFixPos.y = 0;
		SetPosition(0, 0);

		m_TitleTongInfo.Init(&Ini, "TitleTongInfo");
		m_TitleTongName.Init(&Ini, "TitleTongName");
		m_TxtTongName.Init(&Ini, "TxtTongName");
		m_TitleTongUnion.Init(&Ini, "TitleTongUnion");
		m_TxtTongUnion.Init(&Ini, "TxtTongUnion");
/*		m_TitleBuildLevel.Init(&Ini, "TitleBuildLevel");
		m_TxtBuildLevel.Init(&Ini, "TxtBuildLevel");
		m_BtnUpgradeBuildLevel.Init(&Ini, "BtnUpgradeBuildLevel");
		m_TitleTotalOffer.Init(&Ini, "TitleTotalOffer");
		m_TxtTotalOffer.Init(&Ini, "TxtTotalOffer");
		m_BtnAssignTongOffer.Init(&Ini, "BtnAssignTongOffer");
*/
		m_TitleTongMoney.Init(&Ini, "TitleTongMoney");
		m_TxtTongMoney.Init(&Ini, "TxtTongMoney");
/*		m_BtnTransformMoney.Init(&Ini, "BtnTransformMoney");
		m_TitleBuildFund.Init(&Ini, "TitleBuildFund");
		m_TxtBuildFund.Init(&Ini, "TxtBuildFund");
		m_BtnTransformBuildFund.Init(&Ini, "BtnTransformBuildFund");
		m_TitleBattleFund.Init(&Ini, "TitleBattleFund");
		m_TxtBattleFund.Init(&Ini, "TxtBattleFund");
		m_TitleStoredBuildFund.Init(&Ini, "TitleStoredBuildFund");
		m_TxtStoredBuildFund.Init(&Ini, "TxtStoredBuildFund");
*/

		m_TxtPersonalInfo.Init(&Ini, "TxtPersonalInfo");
/*		m_TitlePersonalOffer.Init(&Ini, "TitlePersonalOffer");
		m_TxtPersonalOffer.Init(&Ini, "TxtPersonalOffer");
		m_BtnStorePersonalOffer.Init(&Ini, "BtnStorePersonalOffer");
*/
		m_TitleTongMoney2.Init(&Ini, "TitleTongMoney2");
		m_TxtTongMoney2.Init(&Ini, "TxtTongMoney2");
/*		m_TitleBuildFund2.Init(&Ini, "TitleBuildFund2");
		m_TxtBuildFund2.Init(&Ini, "TxtBuildFund2");
		m_BtnStoreBuildFund.Init(&Ini, "BtnStoreBuildFund");
*/
		m_BtnStoreTongMoney.Init(&Ini, "BtnStoreTongMoney");
		

		for(int i = 0; i < 4; i++)
		{	
			sprintf(Buff, "BtnSubPage%d", i + 1);
			m_BtnSubPages[i].Init(&Ini, Buff);
		}

/*		m_BtnForceToRetire.Init(&Ini, "BtnForceToRetire");
*/
		m_BtnDemise.Init(&Ini, "BtnDemise");
		m_BtnKickOut.Init(&Ini, "BtnKickOut");
		m_BtnDepose.Init(&Ini, "BtnDepose");

/*		m_Btn_DispenseOffer.Init(&Ini, "Btn_DispenseOffer");
*/
		m_BtnChangeMaleTitle.Init(&Ini, "BtnChangeMaleTitle");
		m_BtnChangeFemaleTitle.Init(&Ini, "BtnChangeFemaleTitle");
		m_BtnChangeTitle.Init(&Ini, "BtnChangeTitle");
		m_BtnGetTongMoney.Init(&Ini, "BtnGetTongMoney");
		m_BtnAssignTongMoney.Init(&Ini, "BtnAssignTongMoney");

		m_BtnChangeCamp.Init(&Ini, "BtnChangeCamp");
		m_BtnCreateTongMap.Init(&Ini, "BtnCreateTongMap");
		m_BtnConfigureTongMap.Init(&Ini, "BtnConfigureTongMap");
/*		m_BtnTongStunt.Init(&Ini, "BtnTongStunt");
		m_BtnTongChallenge.Init(&Ini, "BtnTongChallenge");
		m_BtnTongDetect.Init(&Ini, "BtnTongDetect");
*/

/*		m_BtnCreateUnion.Init(&Ini, "BtnCreateUnion");
		m_BtnApplyJionUnion.Init(&Ini, "BtnApplyJionUnion");
		m_BtnAcceptUnionReq.Init(&Ini, "BtnAcceptUnionReq");
		m_BtnLeaveUnion.Init(&Ini, "BtnLeaveUnion");
		m_BtnKickUnionTong.Init(&Ini, "BtnKickUnionTong");
*/

		m_BtnLeaveTong.Init(&Ini, "BtnLeaveTong");
/*		m_BtnRetire.Init(&Ini, "BtnRetire");
*/

		m_TxtRank.Init(&Ini, "TxtRank");
		m_TxtTitle.Init(&Ini, "TxtTitle");
		m_TxtType.Init(&Ini, "TxtType");

		m_MemberList.Init(&Ini, "MemberList");
		m_BtnPrevPage.Init(&Ini, "BtnPrevPage");
		m_BtnNextPage.Init(&Ini, "BtnNextPage");
		m_BtnJump.Init(&Ini, "BtnJump");
		m_BtnOnlinePriority.Init(&Ini, "BtnOnlinePriority");
		m_EditBoxDestPage.Init(&Ini, "EditBoxDestPage");

		Ini.GetString("Main", "StrMemberKick", "", m_szMemberKick, sizeof(m_szMemberKick));
		Ini.GetString("Main", "StrMemberAssign", "", m_szMemberAssign, sizeof(m_szMemberAssign));
		Ini.GetString("Main", "StrMemberDemise", "", m_szMemberDemise, sizeof(m_szMemberDemise));
		Ini.GetString("Main", "StrSelfLeaveTong", "", m_szSelfLeaveTong, sizeof(m_szSelfLeaveTong));
	}
	Show();
	ShowSubPage(SUB_MEMBER);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongFunUseSub::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnSubPages[SUB_MEMBER])
		{
			ShowSubPage(SUB_MEMBER);
		}
		else if(uParam == (unsigned int)&m_BtnSubPages[SUB_TONG])
		{
			ShowSubPage(SUB_TONG);
		}
		else if(uParam == (unsigned int)&m_BtnSubPages[SUB_UNION])
		{
			ShowSubPage(SUB_UNION);
		}
		else if(uParam == (unsigned int)&m_BtnSubPages[SUB_OTHER])
		{
			ShowSubPage(SUB_OTHER);
		}
		else if(uParam == (unsigned int)&m_BtnOnlinePriority)
		{
			OnShowOnline((BOOL)nParam);
		}
		else if(uParam == (unsigned int)&m_BtnPrevPage)
		{
			OnPrevPage();
		}
		else if(uParam == (unsigned int)&m_BtnNextPage)
		{
			OnNextPage();
		}
		else if(uParam == (unsigned int)&m_BtnJump)
		{
			OnJumpPage();
		}
		else if(uParam == (unsigned int)&m_BtnDemise)
		{
			PopupConfirmWindow(RESULT_T_DEMISE);
		}
		else if(uParam == (unsigned int)&m_BtnLeaveTong)
		{
			PopupConfirmWindow(RESULT_T_LEAVE);
		}
		else if(uParam == (unsigned int)&m_BtnKickOut)
		{
			PopupConfirmWindow(RESULT_T_KICKOUT);
		}
		else if(uParam == (unsigned int)&m_BtnDepose)
		{
			KPilgarlicItem *pNode = GetSelectedPlayer();

			KUiTongAssignBox::OpenWindow();
			KUiTongAssignBox::LinkToMainWindow(this);
			KUiTongAssignBox::ArrangeData(m_nRelation, pNode->Name, pNode->szAgname, pNode->nData, m_nIndex);
		}
		else if(uParam == (unsigned int)&m_BtnChangeMaleTitle)
		{
			KUiTongChangeTitle::OpenWindow(this, RESULT_T_MALE_TITLE);
		}
		else if(uParam == (unsigned int)&m_BtnChangeFemaleTitle)
		{
			KUiTongChangeTitle::OpenWindow(this, RESULT_T_FEMALE_TITLE);
		}
		else if(uParam == (unsigned int)&m_BtnChangeTitle)
		{
			KUiTongChangeTitle::OpenWindow(this, RESULT_T_TITLE);
		}
		else if(uParam == (unsigned int)&m_BtnStoreTongMoney)
		{
			KUiGetMoney::OpenWindow(0, 10000000, this, RESULT_T_SAVE_MONEY, NULL);
		}
		else if(uParam == (unsigned int)&m_BtnGetTongMoney)
		{
			KUiGetMoney::OpenWindow(0, 10000000, this, RESULT_T_GET_MONEY, NULL);
		}
		else if(uParam == (unsigned int)&m_BtnChangeCamp)
		{	
			if(KUiTongManager::GetLockOperation())
			{
				KUiTongManager::LockInfomation();
				break;
			}
			char szTongName[defTONG_NAME_MAX_LENGTH + 1];
			m_TxtTongName.GetLabel(szTongName);
			KUiTongCreateSheet::OpenWindow(szTongName);
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	case WND_N_LIST_ITEM_SEL:
		if(uParam == (unsigned int)&m_MemberList)
		{
			m_nSelectIndex = nParam;
			UpdateSelectBtn();
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if(uParam == RESULT_T_KICKOUT)
		{
			OnAction(TONG_ACTION_KICKOUT, nParam);
		}
		else if(uParam == RESULT_T_LEAVE)
		{
			OnAction(TONG_ACTION_LEAVE, nParam);
		}
		else if(uParam == RESULT_T_DEMISE)
		{
			OnAction(RESULT_T_DEMISE, nParam);
		}
		else if(uParam >= RESULT_T_MALE_TITLE && uParam <= RESULT_T_TITLE)
		{
			OnChangeTitle(uParam, (char*)nParam);
		}
		else if(uParam >= RESULT_T_SAVE_MONEY && uParam <= RESULT_T_SND_MONEY)
		{
			OnActionMoney(uParam, nParam);
		}
		break;
	default:
		nRet = KWndPageSet::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::ShowSubPage(SUB_PAGES eSub)
{	
	m_BtnDemise.Hide();
	m_BtnKickOut.Hide();
	m_BtnDepose.Hide();
	m_BtnChangeMaleTitle.Hide();
	m_BtnChangeFemaleTitle.Hide();
	m_BtnChangeTitle.Hide();
	m_BtnGetTongMoney.Hide();
	m_BtnAssignTongMoney.Hide();

	m_BtnChangeCamp.Hide();
	m_BtnCreateTongMap.Hide();
	m_BtnConfigureTongMap.Hide();
/*	m_BtnTongStunt.Hide();
	m_BtnTongChallenge.Hide();
	m_BtnTongDetect.Hide();
*/

/*/	m_BtnCreateUnion.Hide();
	m_BtnApplyJionUnion.Hide();
	m_BtnAcceptUnionReq.Hide();
	m_BtnLeaveUnion.Hide();
	m_BtnKickUnionTong.Hide();
*/
	
	m_SubPages[SUB_MEMBER].Hide();
	m_SubPages[SUB_TONG].Hide();
	m_SubPages[SUB_UNION].Hide();
	m_SubPages[SUB_OTHER].Hide();
	m_BtnSubPages[SUB_MEMBER].CheckButton(false);
	m_BtnSubPages[SUB_TONG].CheckButton(false);
	m_BtnSubPages[SUB_UNION].CheckButton(false);
	m_BtnSubPages[SUB_OTHER].CheckButton(false);

	m_SubPages[eSub].Show();
	m_BtnSubPages[eSub].CheckButton(true);

	switch(eSub)
	{
	case SUB_MEMBER:
		m_BtnDemise.Show();
		m_BtnKickOut.Show();
		m_BtnDepose.Show();
		m_BtnChangeMaleTitle.Show();
		m_BtnChangeFemaleTitle.Show();
		m_BtnChangeTitle.Show();
		m_BtnGetTongMoney.Show();
		m_BtnAssignTongMoney.Show();
		break;
	case SUB_TONG:
		m_BtnChangeCamp.Show();
		m_BtnCreateTongMap.Show();
		m_BtnConfigureTongMap.Show();
/*		m_BtnTongStunt.Show();
		m_BtnTongChallenge.Show();
		m_BtnTongDetect.Show();
*/
		break;
	case SUB_UNION:
/*		m_BtnCreateUnion.Show();
		m_BtnApplyJionUnion.Show();
		m_BtnAcceptUnionReq.Show();
		m_BtnLeaveUnion.Show();
		m_BtnKickUnionTong.Show();
*/
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnShowOnline(BOOL bShow)
{	
	return;
	if(bShow)
	{
		if(m_bShowOnline)
			return;
		m_bShowOnline = true;
	}
	else
	{
		if(!m_bShowOnline)
			return;
		m_bShowOnline = false;
	}
	this->UpdateListView(m_btCurrentPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnPrevPage()
{
	if(m_btCurrentPage <= 1)
		return;

	m_btCurrentPage--;
	UpdateListView(m_btCurrentPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnNextPage()
{
	if(m_btPageCount <= 1)
		return;
	if(m_btCurrentPage == m_btPageCount)
		return;

	m_btCurrentPage++;
	UpdateListView(m_btCurrentPage);
}	

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void KUiTongFunUseSub::OnJumpPage()
{
	OnCheckInput();
	BYTE btPage = (BYTE)m_EditBoxDestPage.GetIntNumber();
	if(btPage < 1)
		btPage = 1;

	if(m_btCurrentPage == btPage)
		return;
	UpdateListView(btPage);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiTongFunUseSub::OnCheckInput()
{
	BYTE btPage = (BYTE)m_EditBoxDestPage.GetIntNumber();
	if (btPage < 0)
	{
		btPage = 0;
	}
	else if (btPage > m_btPageCount)
	{
		btPage = m_btPageCount;
	}
	char	szBuff1[16], szBuff2[16];
	_itoa((int)(btPage), szBuff1, 10);
	m_EditBoxDestPage.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_EditBoxDestPage.SetIntText((int)btPage);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::PopupConfirmWindow(unsigned int uHandleID)
{
	KPilgarlicItem *pPlayer = GetSelectedPlayer();
	
	char szInfo[128];
	switch(uHandleID)
	{
	case RESULT_T_KICKOUT:
		sprintf(szInfo, m_szMemberKick, pPlayer->Name);
		break;
	case RESULT_T_DEMISE:
		sprintf(szInfo, m_szMemberDemise, pPlayer->Name);
		break;
	case RESULT_T_LEAVE:
		sprintf(szInfo, m_szSelfLeaveTong, 100);
		break;
	}
	
	if(pPlayer)
	{
		strcpy(m_szTargetPlayerName, pPlayer->Name);
		KUiTongInformation::OpenWindow(szInfo, pPlayer->nData, this, uHandleID);
	}
	else
	{
		KUiTongInformation::OpenWindow(szInfo, 0, this, uHandleID);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnAction(int nType, int nData)
{	
	if(KUiTongManager::GetLockOperation())
	{
		KUiTongManager::LockInfomation();
		return;
	}

	if(g_pCoreShell)
	{	
		KPilgarlicItem *pNode = GetSelectedPlayer();
		KTongOperationParam Param;
		KTongMemberItem Player;

		memset(&Player, 0, sizeof(KTongMemberItem));
		Param.eOper = (TONG_ACTION_TYPE)nType;
		if(nType == TONG_ACTION_LEAVE)
		{
			Param.nData[0] = m_nRelation;
			Param.nData[1] = -1;
		}
		else
		{
			Param.nData[0] = nData;
			Param.nData[1] = m_nIndex;
		}

		if(m_szTargetPlayerName[0])
		{
			strcpy(Player.Name, m_szTargetPlayerName);
		}
	    else if(pNode)
	    {
		    strcpy(Player.Name, pNode->Name);
	    }
		else
		{
			strcpy(Player.Name, m_szPlayerName);
		}
		
		KUiTongManager::LockOperation();
		g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Player);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnChangeTitle(int nType, const char* lpszTitle)
{	
	if(KUiTongManager::GetLockOperation())
	{
		KUiTongManager::LockInfomation();
		return;
	}
	
	if(nType < RESULT_T_MALE_TITLE || nType > RESULT_T_TITLE)
		return;
	if(!lpszTitle || !lpszTitle[0])
		return;

	if(!g_pCoreShell)
		return;	
	
	KPilgarlicItem *pNode = GetSelectedPlayer();
	if(!pNode && nType != RESULT_T_TITLE)
		return;
	
	KTongMemberItem Player;
	memset(&Player, 0, sizeof(KTongMemberItem));
	
	if(pNode)
	{
		strcpy(Player.Name, pNode->Name);
		Player.nData	= pNode->nData;
	}
	Player.nIndex	= nType;
	strcpy(Player.szAgname, (char*)lpszTitle);
	
	if(g_pCoreShell->TongOperation(GTOI_TONG_CHANGE_TITLE, (unsigned int)&Player, 0))
	{
		KUiTongManager::LockOperation();
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::OnActionMoney(int nType, int nMoney)
{	
	if(KUiTongManager::GetLockOperation())
	{
		KUiTongManager::LockInfomation();
		return;
	}
	
	if(nType < RESULT_T_SAVE_MONEY || nType > RESULT_T_SND_MONEY)
		return;
	if(nMoney <= 0)
		return;
	if(!g_pCoreShell)
		return;

	if(g_pCoreShell->TongOperation(GTOI_TONG_CHANGE_MONEY, (unsigned int)nType, nMoney))
	{
		KUiTongManager::LockOperation();
	}
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::ArrangeComposition(const char* pszPlayerName, int nIndex)
{
	strcpy(m_szPlayerName, pszPlayerName);
	m_nIndex = nIndex;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::InfoArrive(KTongInfo *pTongInfo, int nRelation)
{	
	m_nRelation = nRelation;
	UpdateBtnTheme(m_nRelation);
	UpdateInfoView(pTongInfo);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::UpdateBtnTheme(int nType)
{	
	m_BtnPrevPage.Enable(false);
	m_BtnNextPage.Enable(false);
	m_BtnJump.Enable(false);
	m_EditBoxDestPage.Enable(false);

	int i = 0;
	for(i = 0; i < 4; i++)
		m_BtnSubPages[i].Enable(false);

	m_BtnDemise.Enable(false);
	m_BtnKickOut.Enable(false);
	m_BtnDepose.Enable(false);
	m_BtnChangeMaleTitle.Enable(false);
	m_BtnChangeFemaleTitle.Enable(false);
	m_BtnChangeTitle.Enable(false);
	m_BtnChangeCamp.Enable(false);
	m_BtnCreateTongMap.Enable(false);
	m_BtnConfigureTongMap.Enable(false);
	m_BtnGetTongMoney.Enable(false);
	m_BtnAssignTongMoney.Enable(false);
	
	m_BtnLeaveTong.Enable(false);

	switch(nType)
	{
	case enumTONG_FIGURE_MASTER:
		m_BtnSubPages[0].Enable(true);
		m_BtnSubPages[1].Enable(true);
		m_BtnChangeMaleTitle.Enable(true);
		m_BtnChangeFemaleTitle.Enable(true);
		m_BtnChangeCamp.Enable(true);
		m_BtnGetTongMoney.Enable(true);
		m_BtnAssignTongMoney.Enable(true);
		break;
	case enumTONG_FIGURE_DIRECTOR:
		m_BtnChangeCamp.Enable(true);
		break;
	case enumTONG_FIGURE_MANAGER:
		m_BtnLeaveTong.Enable(true);
		break;
	case enumTONG_FIGURE_MEMBER:
		m_BtnLeaveTong.Enable(true);
		break;
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::UpdateSelectBtn()
{	
	m_BtnDemise.Enable(false);
	m_BtnKickOut.Enable(false);
	m_BtnDepose.Enable(false);
	m_BtnChangeTitle.Enable(false);
	
	if(m_nSelectIndex == -1)
		return;

	KPilgarlicItem *pNode = GetSelectedPlayer();
	if(pNode)
	{
		switch(pNode->nData)
		{
		case enumTONG_FIGURE_DIRECTOR:
			if(m_nRelation == enumTONG_FIGURE_MASTER)
			{
				if(m_nSelectIndex != 0)
				{	
					m_BtnDemise.Enable(true);
					m_BtnDepose.Enable(true);
					m_BtnChangeTitle.Enable(true);
				}
				
			}
			break;
		case enumTONG_FIGURE_MANAGER:
			if(m_nRelation == enumTONG_FIGURE_MASTER ||
				m_nRelation == enumTONG_FIGURE_DIRECTOR)
			{	
				if(m_nRelation == enumTONG_FIGURE_MASTER)
				{
					m_BtnDemise.Enable(true);
				}
				m_BtnDepose.Enable(true);
				m_BtnKickOut.Enable(true);
				m_BtnChangeTitle.Enable(true);
			}
			break;
		case enumTONG_FIGURE_MEMBER:
			if(m_nRelation == enumTONG_FIGURE_MASTER || 
				m_nRelation == enumTONG_FIGURE_DIRECTOR)
			{	
				if(m_nRelation == enumTONG_FIGURE_MASTER)
				{
					m_BtnDemise.Enable(true);
				}
				m_BtnDepose.Enable(true);
				m_BtnKickOut.Enable(true);
				m_BtnChangeTitle.Enable(false); 
			}
			break;
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::UpdateInfoView(KTongInfo* pTongInfo)
{
	char Buff[16];

	m_TxtTongName.SetLabel(pTongInfo->szName, strlen(pTongInfo->szName));
	sprintf(Buff, "V« hÖ");
	m_TxtTongUnion.SetLabel(Buff, strlen(Buff));
/*	_itoa(1, Buff, 10);
	m_TxtBuildLevel.SetLabel(Buff, strlen(Buff));
*/
	m_TxtTongMoney.SetMoneyLabel(pTongInfo->nMoney);

	int nMemberNum = pTongInfo->nMemberCount + pTongInfo->nManagerCount + pTongInfo->nDirectorCount;
	m_btPageCount = (BYTE)(nMemberNum / defMAX_LINE_IN_ONE_PAGE);
	if((nMemberNum % defMAX_LINE_IN_ONE_PAGE))
		m_btPageCount++;
	if(m_btPageCount > 1)
	{
		m_BtnPrevPage.Enable(true);
		m_BtnNextPage.Enable(true);
		m_BtnJump.Enable(true);
		m_EditBoxDestPage.Enable(true);
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUseSub::UpdateListView(BYTE btPage /*= 1*/)
{
	KLinkStruct<KPilgarlicItem> *pData = KUiTongManager::GetMemberData();
	m_EditBoxDestPage.SetIntText(btPage);

	if(pData)
	{	
		m_MemberList.ResetContent();
    	int nDataCount = pData->GetCount();
		int nViewCount = (int)((btPage - 1) * defMAX_LINE_IN_ONE_PAGE);
		if(nDataCount > nViewCount)
		{
			KPilgarlicItem *pNode = pData->GetItem(nViewCount);
			if(pNode)
			{
				char Buff[3][32];
				int nRank = nViewCount;
				for(int i = nViewCount; i < nDataCount; i++)
				{	
					if(i >= nViewCount + defMAX_LINE_IN_ONE_PAGE)
						break;

					if(pNode->Name[0])
					{	
						if(m_bShowOnline)
						{
							if(!pNode->bStatus)
							{
								pNode = pData->NextItem();
								continue;
							}
						}
						nRank++;
						sprintf(Buff[0], "%d", nRank);
						sprintf(Buff[1], "%s", pNode->Name);
						sprintf(Buff[2], "%s", pNode->szAgname);
						m_MemberList.AddString(i, Buff[0], Buff[1], Buff[2], pNode->bStatus);
					}
					pNode = pData->NextItem();
				}
			}
		}
		m_btCurrentPage = btPage;
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KPilgarlicItem* KUiTongFunUseSub::GetSelectedPlayer()
{
	KLinkStruct<KPilgarlicItem>* pData = NULL;
	KPilgarlicItem* pNode = NULL;

	pData = KUiTongManager::GetMemberData();
	if(pData)
	{	
		int nIndex = m_nSelectIndex;
		if(m_btCurrentPage > 1)
			nIndex += (int)((m_btCurrentPage - 1) * defMAX_LINE_IN_ONE_PAGE);
    	pNode = pData->GetItem(nIndex);
	}
	return pNode;
}

//=============================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongFunUse::KUiTongFunUse()
{

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUse::Initialize()
{	
	m_InternalPad.Initialize();
	AddChild(&m_InternalPad);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUse::Clear()
{
	m_InternalPad.Clear();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUse::ArrangeComposition(const char* pszPlayerName, int nIndex)
{
	m_InternalPad.ArrangeComposition(pszPlayerName, nIndex);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongFunUse::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_FUNUSE_INI);
	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongFunUse::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUse::InfoArrive(KTongInfo *pTongInfo, int nRelation)
{	
	m_InternalPad.InfoArrive(pTongInfo, nRelation);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongFunUse::UpdateListView(BYTE btPage /*= 1*/)
{	
	m_InternalPad.UpdateListView(btPage);
}

//=============================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongRightManage::KUiTongRightManage()
{
	memset(m_sRightBtn, 0, sizeof(RIGHT_BTN)*defMAX_RIGHT_BTN);
	m_szPromptInfo[0] = 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongRightManage::Initialize()
{	
	int i = 0;
	for(i = 0; i < 2; i++)
	{
		AddChild(&m_BtnSubPages[i]);
	}

	for(i = 0; i < defMAX_RIGHT_BTN; i++)
		AddChild(&m_BtnRightManage[i]);

	AddChild(&m_BtnSelectAll);
	AddChild(&m_BtnDistribute);

	AddChild(&m_TxtRank);
	AddChild(&m_TxtTitle);
	AddChild(&m_TxtType);
	
	AddChild(&m_MemberList);

	AddChild(&m_BtnOnlinePriority);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongRightManage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_RIGHTMANAGE_INI);
	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");
		for(int i = 0; i < 2; i++)
		{	
			sprintf(Buff, "BtnSubPage_%d", i);
			m_BtnSubPages[i].Init(&Ini, Buff);
		}
		
		Ini.GetString("Main", "PromptInfo", "", m_szPromptInfo, sizeof(m_szPromptInfo));

		for(i = 0; i < defMAX_RIGHT_BTN; i++)
		{	
			sprintf(Buff, "%d", i + 1);
			Ini.GetString("Main", Buff, "", m_sRightBtn[i].szSection, sizeof(m_sRightBtn[i].szSection));
			m_BtnRightManage[i].Init(&Ini, m_sRightBtn[i].szSection);
			Ini.GetShort(m_sRightBtn[i].szSection, "RightID", 0, &m_sRightBtn[i].nRightID);
			Ini.GetShort(m_sRightBtn[i].szSection, "SubPage", 0, (short*)&m_sRightBtn[i].btSubPage);
		}

		m_BtnSelectAll.Init(&Ini, "BtnSelectAll");
		m_BtnDistribute.Init(&Ini, "BtnDistribute");

		m_TxtRank.Init(&Ini, "TxtRank");
		m_TxtTitle.Init(&Ini, "TxtTitle");
		m_TxtType.Init(&Ini, "TxtType");
		m_MemberList.Init(&Ini, "MemberList");
		m_BtnOnlinePriority.Init(&Ini, "BtnOnlinePriority");
	}
	ShowSubPage(SUB_ONE);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongRightManage::Clear()
{
	m_MemberList.ResetContent();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongRightManage::UpdateListView()
{
	KLinkStruct<KPilgarlicItem> *pData = KUiTongManager::GetMemberData();
	if(pData)
	{	
		m_MemberList.ResetContent();
    	int nDataCount = pData->GetCount();
		int nViewCount = 0;
		if(nDataCount > nViewCount)
		{
			KPilgarlicItem *pNode = pData->GetItem(nViewCount);
			if(pNode)
			{
				char Buff[3][32];
				int nRank = nViewCount;
				for(int i = nViewCount; i < nDataCount; i++)
				{	
					if(i >= nViewCount + defMAX_LINE_IN_ONE_PAGE)
						break;

					if(pNode->Name[0])
					{	
						if(pNode->nData != enumTONG_FIGURE_DIRECTOR)
						{
							break;
						}
						nRank++;
						sprintf(Buff[0], "%d", nRank);
						sprintf(Buff[1], "%s", pNode->Name);
						sprintf(Buff[2], "%s", pNode->szAgname);
						m_MemberList.AddString(i, Buff[0], Buff[1], Buff[2], pNode->bStatus);
					}
					pNode = pData->NextItem();
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int KUiTongRightManage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnSubPages[SUB_ONE])
		{
			ShowSubPage(SUB_ONE);
		}
		else if(uParam == (unsigned int)&m_BtnSubPages[SUB_TWO])
		{
			ShowSubPage(SUB_TWO);
		}
		else if(uParam == (unsigned int)&m_BtnSelectAll)
		{
			OnSelectAll((BOOL)nParam);
		}
		break;
	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongRightManage::ShowSubPage(RIGHT_PAGE eSub)
{	
	int i = 0;
	for(i = 0; i < defMAX_RIGHT_BTN; i++)
	{
		if(m_sRightBtn[i].btSubPage == eSub)
			m_BtnRightManage[i].Show();
		else
			m_BtnRightManage[i].Hide();
	}
	for(i = 0; i < 2; i++)
	{	
		if(i == eSub)
			m_BtnSubPages[i].CheckButton(true);
		else
			m_BtnSubPages[i].CheckButton(false);
	}

	
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void KUiTongRightManage::OnSelectAll(BOOL bSel)
{	
	BYTE	btSubActive = 0;
	if(m_BtnSubPages[1].IsButtonChecked())
	{
		btSubActive = 1;
	}

	for(int i = 0; i < defMAX_RIGHT_BTN; i++)
	{
		if(m_sRightBtn[i].btSubPage == btSubActive)
			m_BtnRightManage[i].CheckButton(bSel);
		else
			m_BtnRightManage[i].CheckButton(false);
	}
}

//=============================================================================================================

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
KUiTongRecord::KUiTongRecord()
{

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongRecord::Initialize()
{	
	AddChild(&m_BtnWeekDaily);
	AddChild(&m_BtnAnnounce);
	AddChild(&m_BtnTongAffair);
	AddChild(&m_BtnTongHistory);

	AddChild(&m_RecordList);
	AddChild(&m_AnnounceEditor);

	AddChild(&m_EditorScroll);

	AddChild(&m_BtnLeaveWord);
	AddChild(&m_BtnEditAnnounce);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongRecord::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, TONG_RECORD_INI);
	if (Ini.Load(Buff))
	{	
		Init(&Ini, "Main");

		m_BtnWeekDaily.Init(&Ini, "BtnWeekDaily");
		m_BtnAnnounce.Init(&Ini, "BtnAnnounce");
		m_BtnTongAffair.Init(&Ini, "BtnTongAffair");
		m_BtnTongHistory.Init(&Ini, "BtnTongHistory");

		m_RecordList.Init(&Ini, "RecordList");

		m_AnnounceEditor.Init(&Ini, "AnnounceEditor");
		m_EditorScroll.Init(&Ini, "EditorScroll");

		m_BtnLeaveWord.Init(&Ini, "BtnLeaveWord");
		m_BtnEditAnnounce.Init(&Ini, "BtnEditAnnounce");
	}

	ShowList(LIST_WEEKDAILY);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int	KUiTongRecord::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnWeekDaily)
		{
			ShowList(LIST_WEEKDAILY);
		}
		else if(uParam == (unsigned int)&m_BtnAnnounce)
		{
			ShowList(LIST_ANNOUNCE);
		}
		else if(uParam == (unsigned int)&m_BtnTongAffair)
		{
			ShowList(LIST_AFFAIR);
		}
		else if(uParam == (unsigned int)&m_BtnTongHistory)
		{
			ShowList(LIST_HISTORY);
		}
		break;
	default:
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void	KUiTongRecord::ShowList(RECORD_LIST eList)
{
	m_RecordList.Hide();
	m_AnnounceEditor.Hide();
	m_EditorScroll.Hide();

	m_BtnWeekDaily.CheckButton(false);
	m_BtnAnnounce.CheckButton(false);
	m_BtnTongAffair.CheckButton(false);
	m_BtnTongHistory.CheckButton(false);

	switch(eList)
	{
	case LIST_WEEKDAILY:
		m_RecordList.Show();
		m_BtnWeekDaily.CheckButton(true);
		break;
	case LIST_ANNOUNCE:
		m_AnnounceEditor.Show();
		m_EditorScroll.Show();
		m_BtnAnnounce.CheckButton(true);
		break;
	case LIST_AFFAIR:
		m_RecordList.Show();
		m_BtnTongAffair.CheckButton(true);
		break;
	case LIST_HISTORY:
		m_RecordList.Show();
		m_BtnTongHistory.CheckButton(true);
		break;
	default:
		break;
	}
}
