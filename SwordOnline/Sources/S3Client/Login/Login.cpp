#include "KWin32.h"
#include "KEngine.h"
#include "LoginDef.h"
#include "Login.h"
#include "../NetConnect/NetConnectAgent.h"
#include "KProtocol.h"
#include "crtdbg.h"
#include "../Ui/UiBase.h"
#include "../Ui/UiShell.h"
#include "../../Engine/Src/Cryptography/EDOneTimePad.h"
#include "time.h"
#include "KLicense.h"

#define	SERVER_LIST_FILE				"\\Settings\\ServerList.ini"

KLogin		g_LoginLogic;

// ==== [BEGIN] Helpers for ENC:<hex> address decoding ====
static int HexToNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static int HexToBytes(const char* src, unsigned char* dst, int maxDst) {
    int n = 0, i = 0;
    while (src[i]) {
        char c1 = src[i++]; if (c1==' '||c1=='\t'||c1=='\r'||c1=='\n') continue;
        if (!src[i]) return -1;
        char c2 = src[i++]; while (c2==' '||c2=='\t'||c2=='\r'||c2=='\n') { if (!src[i]) return -1; c2 = src[i++]; }
        int hi = HexToNibble(c1), lo = HexToNibble(c2);
        if (hi < 0 || lo < 0) return -1;
        if (n >= maxDst) return -1;
        dst[n++] = (unsigned char)((hi << 4) | lo);
    }
    return n;
}

// Trả về 1 nếu decode OK, copy ra plain IPv4; nếu không phải ENC: thì copy nguyên văn (giữ tương thích)
static int DecodeAddressField(const char* szField, char* szPlainOut, int outSize) {
    if (!szField || !szPlainOut || outSize <= 0) return 0;

    // Bỏ khoảng trắng đầu/cuối (chống lỗi khi ghi ini)
    while (*szField==' '||*szField=='\t') ++szField;

    if (szField[0]=='E' && szField[1]=='N' && szField[2]=='C' && szField[3]==':') {
        const char* pHex = szField + 4;
        unsigned char buf[128];
        int n = HexToBytes(pHex, buf, sizeof(buf));
        if (n <= 0) return 0;
        EDOneTimePad_Decipher((char*)buf, n);
        if (n >= (int)sizeof(buf)) return 0;
        buf[n] = '\0';
        lstrcpynA(szPlainOut, (const char*)buf, outSize);
        return 1;
    } else {
        // Dạng cũ: IPv4 “x.x.x.x”
        lstrcpynA(szPlainOut, szField, outSize);
        return 1;
    }
}
// ==== [END] Helpers ====



bool GetIpAddress(const char* szAddress, unsigned char* pcAddress)
{
	_ASSERT(pcAddress);
	int nValue[4];
	int nRet = sscanf(szAddress, "%d.%d.%d.%d", &nValue[0], &nValue[1], &nValue[2], &nValue[3]);
	if (nRet == 4 &&
		nValue[0] >= 0 && nValue[0] < 256 &&
		nValue[1] >= 0 && nValue[1] < 256 &&
		nValue[2] >= 0 && nValue[2] < 256 &&
		nValue[3] >= 0 && nValue[3] < 256)
	{
		pcAddress[0] = nValue[0];
		pcAddress[1] = nValue[1];
		pcAddress[2] = nValue[2];
		pcAddress[3] = nValue[3];
		return true;
	}
	return false;
}

static unsigned int gs_holdrand = (unsigned int)time(NULL);


static inline unsigned _Rand()
{
    gs_holdrand = gs_holdrand * 244213L + 1541021L;
     
    return gs_holdrand;
}


void RandMemSet(int nSize, unsigned char *pbyBuffer)
{
    _ASSERT(nSize);
    _ASSERT(pbyBuffer);

    while (nSize--)
    {
        *pbyBuffer++ = (unsigned char)_Rand();
    }
}


KLogin::KLogin()
{
	m_Status = LL_S_IDLE;
	m_Result = LL_R_NOTHING;
	m_bInAutoProgress = false;
	m_nNumRole = 0;
	memset(&m_Choices, 0, sizeof(LOGIN_CHOICE));
	ClearAccountPassword(true, true);
	m_LeftTime = 0;
}


void KLogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
	if (bAccount)
		memset(m_Choices.Account, 0xff, sizeof(m_Choices.Account));
	if (bPassword)
		memset(&m_Choices.Password, 0xff, sizeof(m_Choices.Password));
}


KLogin::~KLogin()
{
	_ASSERT(m_Status == LL_S_IDLE);
}

int KLogin::CreateConnection(const unsigned char* pAddress)
{
	int nRet;
	if (m_Status == LL_S_IDLE && pAddress &&
		ConnectAccountServer(pAddress))
	{
		RegistNetAgent();
		m_Status = LL_S_WAIT_INPUT_ACCOUNT;
		m_Result = LL_R_NOTHING;

		if (m_bInAutoProgress)
		{
			char	szAccount[32];
            KSG_PASSWORD Password;
			GetAccountPassword(szAccount, &Password);
			AccountLogin(szAccount, Password, false);
			memset(szAccount, 0, sizeof(szAccount));
			memset(&Password, 0, sizeof(Password));
		}
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}


int	KLogin::AccountLogin(const char* pszAccount, const KSG_PASSWORD& crPassword, bool bOrignPassword)
{
	int nRet;
	if (m_Status == LL_S_WAIT_INPUT_ACCOUNT &&
		pszAccount && 
		Request(pszAccount, &crPassword, LOGIN_A_LOGIN))
	{
        if (bOrignPassword)
        {
    		SetAccountPassword(pszAccount, &crPassword);
        }
		m_Status = LL_S_ACCOUNT_CONFIRMING;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}

int	KLogin::SelectRole(int nIndex)
{
	int nRet;
	if (m_Status == LL_S_ROLE_LIST_READY && nIndex >= 0 && nIndex < m_nNumRole)
	{
		tagDBSelPlayer	NetCommand;
		NetCommand.cProtocol = c2s_dbplayerselect;
		strcpy(NetCommand.szRoleName, m_RoleList[nIndex].Name);
		g_NetConnectAgent.SendMsg(&NetCommand, sizeof(tagDBSelPlayer));
		g_NetConnectAgent.UpdateClientRequestTime(false);
		strcpy(m_Choices.szProcessingRoleName, NetCommand.szRoleName);
		m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}


int	KLogin::CreateRole(KRoleChiefInfo* pCreateInfo)
{
	int nRet = false;
	m_Result = LL_R_CONNECT_FAILED;

	if (m_Status == LL_S_ROLE_LIST_READY && pCreateInfo && m_nNumRole < MAX_PLAYER_PER_ACCOUNT &&
		pCreateInfo->Attribute >= 0 && pCreateInfo->Attribute < series_num)
	{
		int nNameLen = strlen(pCreateInfo->Name);
		if (nNameLen >= 1 && nNameLen < sizeof(pCreateInfo->Name))
		{
			char	Data[sizeof(TProcessData) + sizeof(NEW_PLAYER_COMMAND)];
			TProcessData*	pNetCommand = (TProcessData*)&Data;
			NEW_PLAYER_COMMAND* pInfo = (NEW_PLAYER_COMMAND*)pNetCommand->pDataBuffer;
			pInfo->m_btRoleNo = pCreateInfo->Gender;
			pInfo->m_btSeries = pCreateInfo->Attribute;
			pInfo->m_NativePlaceId = pCreateInfo->NativePlaceId;
			memcpy(pInfo->m_szName, pCreateInfo->Name, nNameLen);
			pInfo->m_szName[nNameLen] = '\0';

			pNetCommand->nProtoId = c2s_newplayer;
			pNetCommand->nDataLen = sizeof(NEW_PLAYER_COMMAND) - sizeof(pInfo->m_szName) + nNameLen + 1/* sizeof( '\0' ) */;
			pNetCommand->ulIdentity = 0;

			g_NetConnectAgent.SendMsg(&Data, sizeof(TProcessData) - sizeof(pNetCommand->pDataBuffer) + pNetCommand->nDataLen);
			g_NetConnectAgent.UpdateClientRequestTime(false);

			memcpy(m_Choices.szProcessingRoleName, pCreateInfo->Name, nNameLen);
			m_Choices.szProcessingRoleName[nNameLen] = 0;

			m_Status = LL_S_CREATING_ROLE;
			m_Result = LL_R_NOTHING;
			nRet = true;
		}
	}
	return nRet;
}

int	KLogin::DeleteRole(int nIndex, const KSG_PASSWORD &crSupperPassword)
{
	int nRet;

	if (m_Status == LL_S_ROLE_LIST_READY && nIndex >= 0 && nIndex < m_nNumRole)
	{
		tagDBDelPlayer	NetCommand;
		RandMemSet(sizeof(tagDBDelPlayer), (BYTE*)&NetCommand);

		NetCommand.cProtocol = c2s_roleserver_deleteplayer;
		GetAccountPassword(NetCommand.szAccountName, NULL);
        NetCommand.Password = crSupperPassword;
		strncpy(NetCommand.szRoleName, m_RoleList[nIndex].Name, sizeof(NetCommand.szRoleName));
        NetCommand.szRoleName[sizeof(NetCommand.szRoleName) - 1] = '\0';

		g_NetConnectAgent.SendMsg(&NetCommand, sizeof(tagDBDelPlayer));
		memset(&NetCommand.Password, 0, sizeof(NetCommand.Password));
		g_NetConnectAgent.UpdateClientRequestTime(false);

		strcpy(m_Choices.szProcessingRoleName, m_RoleList[nIndex].Name);

		m_Status = LL_S_DELETING_ROLE;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		nRet = false;
		m_Result = LL_R_CONNECT_FAILED;
	}
	return nRet;
}

void KLogin::NotifyTimeout()
{
	if (m_Status != LL_S_IDLE)
	{
		ReturnToIdle();
		m_Result = LL_R_CONNECT_TIMEOUT;
	}
}

void KLogin::NotifyDisconnect()
{
	if (m_Status != LL_S_IDLE)
	{
		ReturnToIdle();
		m_Result = LL_R_CONNECT_FAILED;
	}
}
void KLogin::NotifyToStartGame()
{
	if (m_Status == LL_S_ENTERING_GAME)
	{
		g_NetConnectAgent.UpdateClientRequestTime(true);

		char	szAccount[32];
		GetAccountPassword(szAccount, NULL);
		g_UiBase.SetUserAccount(szAccount, m_Choices.szProcessingRoleName);

		m_Status = LL_S_IN_GAME;
		m_Result = LL_R_NOTHING;
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		UiOnGameServerStartSyncEnd();
	}
}

void KLogin::ReturnToIdle()
{
	if (m_Status != LL_S_IDLE)
	{
		UnRegistNetAgent();
		g_NetConnectAgent.DisconnectGameSvr();
		g_NetConnectAgent.DisconnectClient();
		m_Status = LL_S_IDLE;
	}
	m_Choices.bIsRoleNewCreated = false;
	m_Result = LL_R_NOTHING;
	m_bInAutoProgress = false;
}

void KLogin::ReturnToIdleLixian()//Lixian by kinnox;
{
	if (m_Status != LL_S_IDLE)
	{
		UnRegistNetAgent();
		g_NetConnectAgent.DisconnectGameSvr();
		g_NetConnectAgent.DisconnectClient(); 
		m_Status = LL_S_IDLE;
	}
	m_Choices.bIsRoleNewCreated = false;
	m_Result = LL_R_NOTHING;
	m_bInAutoProgress = false;
}

void KLogin::AutoLogin()
{
	ReturnToIdle();
	if (IsAutoLoginEnable())
	{
		m_bInAutoProgress = true;
		if (m_Choices.AccountServer.Address[0] == 0 &&
			m_Choices.AccountServer.Address[1] == 0 &&
			m_Choices.AccountServer.Address[2] == 0 &&
			m_Choices.AccountServer.Address[3] == 0)
		{
			int nCount, nSel;
			KLoginServer* pList = GetServerList(-1, nCount, nSel);
			if (pList)
			{
				free(pList);
				pList = NULL;
			}
		}
		CreateConnection(m_Choices.AccountServer.Address);
	}
}
	
int	KLogin::IsAutoLoginEnable()
{
	return ((~m_Choices.Account[0]) &&
		(~m_Choices.Password.szPassword[0]) &&
		m_Choices.szProcessingRoleName[0] &&
		m_Choices.AccountServer.Title[0]);
}

void KLogin::SetRememberAccountFlag(bool bEnable)
{
	m_Choices.bRememberAccount = bEnable;
	if (bEnable == false)
		m_Choices.bRememberAll = false;		
}

void KLogin::SetRememberAllFlag(bool bEnable)
{
	m_Choices.bRememberAll = bEnable;
	if (bEnable)
		m_Choices.bRememberAccount = true;
}

void KLogin::SetVirtualKeyboardFlag(bool bEnable)
{
	m_Choices.bVirtualKeyboard = bEnable;	
}
int	KLogin::GetRoleInfo(int nIndex, KRoleChiefInfo* pInfo)
{
	if (nIndex >= 0 && nIndex < m_nNumRole)
	{
		if (pInfo)
			*pInfo = m_RoleList[nIndex];
		return true;
	}
	return false;
}

void KLogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{
	if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&
		pResponse->Size >= sizeof(KLoginAccountInfo))				
	{
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
		char	szAccount[32];
        KSG_PASSWORD Password;
		GetAccountPassword(szAccount, &Password);
		if (strcmp(pInfo->Account,  szAccount)  == 0 &&
			strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
		{			
			int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
			if (nResult == LOGIN_R_SUCCESS)
			{
				g_NetConnectAgent.UpdateClientRequestTime(false);
				m_Status = LL_S_WAIT_ROLE_LIST;
				m_Result = LL_R_ACCOUNT_CONFIRM_SUCCESS;
				m_LeftTime = pInfo->nLeftTime;
			}
			else
			{
				LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
				switch(nResult)
				{
				case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:
					eResult = LL_R_ACCOUNT_PWD_ERROR;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_ACCOUNT_EXIST:
					eResult = LL_R_ACCOUNT_LOCKED;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_FREEZE:
					eResult = LL_R_ACCOUNT_FREEZE;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_INVALID_PROTOCOLVERSION:
					eResult = LL_R_INVALID_PROTOCOLVERSION;
					break;
				case LOGIN_R_FAILED:
					eResult = LL_R_CONNECT_SERV_BUSY;
					break;
				case LOGIN_R_TIMEOUT:
					eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_LOCKED:
					eResult = LL_R_CONNECT_SERV_LOCKED;
					break;
				case LOGIN_R_DENIED:
					eResult = LL_R_CONNECT_SERV_DENIED;
					break;
				case LOGIN_R_BANED:
					eResult = LL_R_CONNECT_SERV_BANED;
					break;
				default:
					eResult = LL_R_CONNECT_FAILED;
					break;
				}
				if (m_Status != LL_S_WAIT_INPUT_ACCOUNT ||
					m_bInAutoProgress)
				{
					ReturnToIdle();
				}
				m_Result = eResult;
			}
		}
		memset(szAccount, 0, sizeof(szAccount));
		memset(&Password, 0, sizeof(Password));
	}
}

void KLogin::ProcessRoleListResponse(TProcessData* pResponse)
{
	if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
	{
		m_nNumRole = pResponse->pDataBuffer[0];
		if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
			m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
		else if (m_nNumRole < 0)
			m_nNumRole = 0;

		RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
		for (int i = 0; i < m_nNumRole; i++)
		{
			if (pList->szName[0])
			{
				strcpy(m_RoleList[i].Name, pList->szName);
				m_RoleList[i].Attribute = pList->Series;
				m_RoleList[i].Gender = pList->Sex;
				m_RoleList[i].nLevel = pList->Level;
				pList ++;
			}
			else
			{
				m_nNumRole = i;
				break;
			}
		}

		g_NetConnectAgent.UpdateClientRequestTime(true);
		m_Status = LL_S_ROLE_LIST_READY;
		m_Result = LL_R_NOTHING;

		if (m_bInAutoProgress)
		{
			int nAdviceChoice;
			GetRoleCount(nAdviceChoice);
			SelectRole(nAdviceChoice);
		}
	}
}

void KLogin::ProcessCreateRoleResponse(tagNewDelRoleResponse* pResponse)
{
	if (pResponse->cProtocol == s2c_rolenewdelresponse)
	{
		if (pResponse->bSucceeded)
		{
			g_NetConnectAgent.UpdateClientRequestTime(false);
			m_Choices.bIsRoleNewCreated = true;
			m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;
			m_Result = LL_R_CREATE_ROLE_SUCCESS;
		}
		else
		{
			g_NetConnectAgent.UpdateClientRequestTime(true);
			m_Status = LL_S_ROLE_LIST_READY;
			m_Result = LL_R_INVALID_ROLENAME;
		}
	}
}

void KLogin::ProcessDeleteRoleResponse(tagNewDelRoleResponse* pResponse)
{
	if (pResponse->cProtocol == s2c_rolenewdelresponse)
	{

		g_NetConnectAgent.UpdateClientRequestTime(true);
		m_Status = LL_S_ROLE_LIST_READY;
		if (pResponse->bSucceeded == 1)
		{
			char	szAccount[32];
			GetAccountPassword(szAccount, NULL);
			g_UiBase.SetUserAccount(szAccount, m_Choices.szProcessingRoleName);
			g_UiBase.CleanPrivateDataFolder();

			for (int i = 0; i < m_nNumRole; i++)
			{
				if (strcmp(m_RoleList[i].Name, m_Choices.szProcessingRoleName) == 0)
				{
					m_nNumRole--;
					for (; i < m_nNumRole; i++)
						m_RoleList[i] = m_RoleList[i + 1];
					break;
				}
			}
			m_Result = LL_R_NOTHING;
		}
		else if (pResponse->bSucceeded == -1)
		{
			m_Result = LL_R_INVALID_PASSWORD;
		}
		else if (pResponse->bSucceeded == -2 || pResponse->bSucceeded == -4)
		{
			m_Result = LL_R_CONNECT_FAILED;
		}
		else
		{
			m_Result = LL_R_DEL_ROLE_HAVE_TONG;
		}
	}
}

void KLogin::ProcessAccountExtPointResponse(KLoginStructHead* pResponse)
{
	if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&
		pResponse->Size >= sizeof(KLoginAccountInfo))
	{
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
		char	szAccount[32];
        KSG_PASSWORD Password;
		GetAccountPassword(szAccount, &Password);
		if (strcmp(pInfo->Account,  szAccount)  == 0 &&
			strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
		{			
			int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
			if (nResult == LOGIN_R_SUCCESS)
			{
				g_NetConnectAgent.UpdateClientRequestTime(false);
				m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;
				m_Result = LL_R_NOTHING;
			}
			else
			{
				LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
				switch(nResult)
				{
				case LOGIN_R_FAILED:
					eResult = LL_R_CONNECT_SERV_BUSY;
					break;
				case LOGIN_R_TIMEOUT:
					eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_DENIED:
					eResult = LL_R_CONNECT_SERV_DENIED;
					break;
				default:
					eResult = LL_R_CONNECT_FAILED;
					break;
				}
				if (m_Status != LL_S_WAIT_INPUT_ACCOUNT ||
					m_bInAutoProgress)
				{
					ReturnToIdle();
				}
				m_Result = eResult;
			}
		}
		memset(szAccount, 0, sizeof(szAccount));
		memset(&Password, 0, sizeof(Password));
	}
}


void KLogin::ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse)
{
	if (pResponse->cProtocol == s2c_notifyplayerlogin)
	{
		if (strcmp((const char*)pResponse->szRoleName, m_Choices.szProcessingRoleName) == 0)
		{
			g_NetConnectAgent.UpdateClientRequestTime(true);

			if (g_NetConnectAgent.ConnectToGameSvr(
				(const unsigned char*)&pResponse->nIPAddr,
				pResponse->nPort, &pResponse->guid))
			{
				m_Status = LL_S_ENTERING_GAME;
				m_Result = LL_R_NOTHING;

				// FIX: Delay disconnect from Bishop to allow GameServer connection to establish
				// Previous bug: Disconnected immediately causing Bishop to cleanup before GS connected
				// Now: Keep Bishop connection alive briefly while connecting to GameServer
				// Bishop will detect disconnect naturally when client fully transitions to GS
				// g_NetConnectAgent.DisconnectClient();  // ❌ REMOVED - Don't disconnect yet
			}
			else
			{
				ReturnToIdle();
				m_Result = LL_R_CONNECT_FAILED;
				// FIX: Only disconnect Bishop if GameServer connection FAILED
				g_NetConnectAgent.DisconnectClient();
			}
		}
		else
		{
			ReturnToIdle();
			m_Result = LL_R_SERVER_SHUTDOWN;
			g_NetConnectAgent.DisconnectClient();
		}
	}
}


void KLogin::AcceptNetMsg(void* pMsgData)
{
	if (pMsgData == NULL)
		return;

	switch(m_Status)
	{
	case LL_S_ACCOUNT_CONFIRMING:
		ProcessAccountLoginResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_ROLE_LIST:
		ProcessRoleListResponse((TProcessData*)pMsgData);
		break;
	case LL_S_CREATING_ROLE:
		ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_DELETING_ROLE:
		ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_WAIT_EXTPOINT:
		ProcessAccountExtPointResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:
		ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData);
		break;
	}
}

LOGIN_LOGIC_RESULT_INFO KLogin::GetResult()
{
	LOGIN_LOGIC_RESULT_INFO eReturn = m_Result;
	m_Result = LL_R_NOTHING;
	return eReturn;
}


int KLogin::GetRoleCount(int& nAdviceChoice)
{
	nAdviceChoice = 0;
	if (m_Choices.szProcessingRoleName[0])
	{
		for (int i = 0; i < m_nNumRole; i++)
		{
			if (strcmp(m_Choices.szProcessingRoleName, m_RoleList[i].Name) == 0)
			{
				nAdviceChoice = i;
				break;
			}
		}
	}
	return m_nNumRole;
}

bool KLogin::GetLoginAccount(char* pszAccount)
{
	if (pszAccount)
		GetAccountPassword(pszAccount, NULL);
	return m_Choices.bRememberAccount;
}

#define	$LOGIN			"Login"
#define	$LAST_ACCOUNT	"LastAccount"
#define	$LAST_PASSWORD	"LastPassword"

void KLogin::LoadLoginChoice()
{
	if (m_Choices.bLoaded)
		return;
	memset(&m_Choices, 0, sizeof(m_Choices));
	ClearAccountPassword(true, true);

	m_Choices.bLoaded = true;

	KIniFile* pSetting = g_UiBase.GetCommSettingFile();
	char	szAccount[32];
    KSG_PASSWORD Password;
	if (pSetting)
	{
		pSetting->GetInteger($LOGIN, "SelServerRegion", 0, &m_Choices.nServerRegionIndex);
		pSetting->GetString($LOGIN, "LastGameServer", "", m_Choices.AccountServer.Title, sizeof(m_Choices.AccountServer.Title));
		
		szAccount[0] = 0;
		pSetting->GetStruct($LOGIN, $LAST_ACCOUNT, szAccount, sizeof(szAccount));
		if (szAccount[0])
		{
			EDOneTimePad_Decipher(szAccount, strlen(szAccount));
			m_Choices.bRememberAccount = true;
			SetAccountPassword(szAccount, NULL);

			Password.szPassword[0] = '\0';
			pSetting->GetStruct($LOGIN, $LAST_PASSWORD, Password.szPassword, sizeof(Password.szPassword));
			if (Password.szPassword[0])
			{
				EDOneTimePad_Decipher(Password.szPassword, strlen(Password.szPassword));
				m_Choices.bRememberAll = true;
				SetAccountPassword(NULL, &Password);
				memset(&Password, 0, sizeof(Password));
			}
		}

		pSetting->GetShort($LOGIN, "VirtualKeyboard", 0, (short*)&m_Choices.bVirtualKeyboard);
	
		if (szAccount[0])
		{
			KIniFile* pPrivate = g_UiBase.GetPrivateSettingFile();
			if (pPrivate)
			{
				if (pPrivate->GetString("Main", "LastSelCharacter", "",
					m_Choices.szProcessingRoleName, sizeof(m_Choices.szProcessingRoleName)))
				{
					EDOneTimePad_Decipher(m_Choices.szProcessingRoleName, strlen(m_Choices.szProcessingRoleName));
				}
			}
			g_UiBase.ClosePrivateSettingFile(false);
		}

		g_UiBase.CloseCommSettingFile(false);
	}
}

void KLogin::SaveLoginChoice()
{

	KIniFile*	pSetting = g_UiBase.GetCommSettingFile();
	int	i;
	if (pSetting)
	{
		pSetting->WriteInteger($LOGIN, "SelServerRegion", m_Choices.nServerRegionIndex);
		if (m_Choices.AccountServer.Title[0])
		{
			pSetting->WriteString($LOGIN, "LastGameServer", m_Choices.AccountServer.Title);
		}

		pSetting->WriteInteger($LOGIN, "VirtualKeyboard", m_Choices.bVirtualKeyboard);

		char	szBuffer[32];
		pSetting->EraseKey($LOGIN, $LAST_ACCOUNT);

		if (m_Choices.bRememberAccount)
		{

			GetAccountPassword(szBuffer, NULL);
			i = strlen(szBuffer);
			EDOneTimePad_Encipher(szBuffer, i);
			pSetting->WriteStruct($LOGIN, $LAST_ACCOUNT, szBuffer, sizeof(szBuffer));
			int j = 0, k = 0;
			char szKeyName[32], szAccountName[32], szSaveBuffer[$MAX_ACCOUNT_LIST][32];
			szAccountName[0] = 0;
			memset(szSaveBuffer, 0, sizeof(szSaveBuffer));
			pSetting->GetStruct($ACCOUNT_LIST, $RECENT_FIRST_ACCOUNT, szAccountName, sizeof(szAccountName));
			if (strcmp(szAccountName, szBuffer) != 0)
			{
				for (j = 1; j < $MAX_ACCOUNT_LIST; j ++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					pSetting->GetStruct($ACCOUNT_LIST, szKeyName, szAccountName, sizeof(szAccountName));
					if (strcmp(szAccountName, szBuffer) == 0)
					{
						memset(szAccountName, 0, sizeof(szAccountName));
						pSetting->WriteStruct($ACCOUNT_LIST, szKeyName, szAccountName, sizeof(szAccountName));
					}
				}
				for (j = 0; j < $MAX_ACCOUNT_LIST; j ++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					pSetting->GetStruct($ACCOUNT_LIST, szKeyName, szSaveBuffer[j], sizeof(szSaveBuffer[j]));
				}
				pSetting->WriteStruct($ACCOUNT_LIST, $RECENT_FIRST_ACCOUNT, szBuffer, sizeof(szBuffer));
				for (j = 1; j < $MAX_ACCOUNT_LIST; j++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					while (k < $MAX_ACCOUNT_LIST)
					{
						if (szSaveBuffer[k])
						{
							pSetting->WriteStruct($ACCOUNT_LIST, szKeyName, szSaveBuffer[k], sizeof(szSaveBuffer[k]));
							k++;
							break;
						}
						k++;
					}
				}
			}
		}

		g_UiBase.CloseCommSettingFile(true);
	}
}

KLoginServer*	KLogin::GetServerRegionList(int& nCount, int& nAdviceChoice)
{
	KLoginServer* pServers = NULL;
	nCount = 0;
	nAdviceChoice = 0;

	KIniFile	File;
	int			i;
	if (File.Load(SERVER_LIST_FILE))
	{
		int		nReadCount = 0;
		char	szKey[32];
		File.GetInteger("List", "RegionCount", 0, &nReadCount);
		if (nReadCount > 0)
		{
			pServers = (KLoginServer*)malloc(sizeof(KLoginServer) * nReadCount);
			if (pServers)
			{
				for (i = 0; i < nReadCount; i++)
				{
					sprintf(szKey, "Region_%d", i);
					if (File.GetString("List", szKey, "", pServers[nCount].Title,
						sizeof(pServers[nCount].Title)) &&
						pServers[nCount].Title[0])
					{
						nCount ++;
					}
				}
				if (nCount == 0)
				{
					free(pServers);
					pServers = NULL;
				}
			}
		}
	}

	if (m_Choices.nServerRegionIndex < 0 || m_Choices.nServerRegionIndex >= nCount)
		m_Choices.nServerRegionIndex = 0;
	nAdviceChoice = m_Choices.nServerRegionIndex;
	return pServers;
}

// ==== [BEGIN] KLogin::GetServerList (đã cập nhật hỗ trợ ENC:<hex>) ====
KLoginServer* KLogin::GetServerList(int nRegion, int& nCount, int& nAdviceChoice)
{
    KLoginServer* pServers = NULL;
    nCount = 0;
    nAdviceChoice = 0;

    // Lấy region mặc định từ setting nếu caller truyền nRegion < 0
    if (nRegion < 0)
    {
        KIniFile* pSetting = g_UiBase.GetCommSettingFile();
        if (pSetting)
        {
            pSetting->GetInteger($LOGIN, "SelServerRegion", 0, &nRegion);
            g_UiBase.CloseCommSettingFile(false);
        }
    }

    KIniFile File;
    if (File.Load(SERVER_LIST_FILE))
    {
        int regionCount = 0;
        File.GetInteger("List", "RegionCount", 0, &regionCount);

        // Ràng buộc nRegion hợp lệ
        if (regionCount > 0)
        {
            if (nRegion < 0 || nRegion >= regionCount)
                nRegion = 0; // fallback an toàn

            m_Choices.nServerRegionIndex = nRegion;

            char szSection[32];
            wsprintfA(szSection, "Region_%d", nRegion);

            int nReadCount = 0;
            File.GetInteger(szSection, "Count", 0, &nReadCount);

            if (nReadCount > 0)
            {
                pServers = (KLoginServer*)malloc(sizeof(KLoginServer) * nReadCount);
                if (pServers)
                {
                    int i;
                    for (i = 0; i < nReadCount; i++)
                    {
                        // --- Đọc Address: dùng buffer RỘNG để không cắt cụt ENC:<hex> ---
                        char szKey[32];
                        char szAddrRaw[256];   // raw từ file (ENC:<hex> dài)
                        char szAddrPlain[64];  // sau decode (IPv4 x.x.x.x)

                        wsprintfA(szKey, "%d_Address", i);
                        if (!File.GetString(szSection, szKey, "", szAddrRaw, sizeof(szAddrRaw)))
                            continue;

                        // Giải mã nếu có ENC:, hoặc giữ nguyên nếu là IPv4 thường
                        if (!DecodeAddressField(szAddrRaw, szAddrPlain, sizeof(szAddrPlain)))
                            continue;

                        // Parse IPv4 (x.x.x.x) thành 4 byte Address
                        if (!GetIpAddress(szAddrPlain, pServers[nCount].Address))
                            continue;

                        // --- Đọc Title như cũ ---
                        wsprintfA(szKey, "%d_Title", i);
                        if (File.GetString(szSection, szKey, "", pServers[nCount].Title,
                                           sizeof(pServers[nCount].Title)) &&
                            pServers[nCount].Title[0])
                        {
                            nCount++;
                        }
                    }

                    if (nCount == 0)
                    {
                        free(pServers);
                        pServers = NULL;
                    }
                }
            }
        }
    }

    // Giữ logic chọn nAdviceChoice + m_Choices.AccountServer như cũ
    if (nCount)
    {
        int i;
        for (i = 0; i < nCount; i++)
        {
            if (strcmp(pServers[i].Title, m_Choices.AccountServer.Title) == 0)
            {
                nAdviceChoice = i;
                break;
            }
        }
        if (i >= nCount)
            lstrcpyA(m_Choices.AccountServer.Title, pServers[nAdviceChoice].Title);

        m_Choices.AccountServer.Address[0] = pServers[nAdviceChoice].Address[0];
        m_Choices.AccountServer.Address[1] = pServers[nAdviceChoice].Address[1];
        m_Choices.AccountServer.Address[2] = pServers[nAdviceChoice].Address[2];
        m_Choices.AccountServer.Address[3] = pServers[nAdviceChoice].Address[3];
    }
    return pServers;
}
// ==== [END] KLogin::GetServerList ====

int KLogin::SetAccountServer(const KLoginServer &rcSelectServer)
{
    m_Choices.AccountServer = rcSelectServer;
    
    return true;
}

bool KLogin::GetVirtualKeyboard()
{
	return m_Choices.bVirtualKeyboard;
}


extern void RandMemSet(int nSize, unsigned char *pbyBuffer);
int KLogin::Request(const char* pszAccount, const KSG_PASSWORD* pcPassword, int nAction)
{
	BYTE		Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];

	RandMemSet(sizeof(Buff), (BYTE*)Buff);

	if (pszAccount && pcPassword)
	{
		(*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login;
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];
		pInfo->Size  = sizeof(KLoginAccountInfo);
		pInfo->Param = nAction | LOGIN_R_REQUEST;
		strncpy(pInfo->Account,  pszAccount, sizeof(pInfo->Account));
        pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';
		pInfo->Password = *pcPassword;
		char szLicense[32];
		g_pClientLic(szLicense);
		strncpy(pInfo->License, szLicense, sizeof(pInfo->License));
        #ifdef USE_KPROTOCOL_VERSION
        pInfo->ProtocolVersion = KPROTOCOL_VERSION;
        #endif

		if (g_NetConnectAgent.SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))
		{
			g_NetConnectAgent.UpdateClientRequestTime(false);
			return true;
		}
	}
	return false;
}

int KLogin::ConnectAccountServer(const unsigned char* pIpAddress)
{
	/*KIniFile	IniFile;
	if (pIpAddress && IniFile.Load("\\Config.ini"))
	{			
		int nPort;
		IniFile.GetInteger("Server", "GameServPort", 5622, &nPort);
		return g_NetConnectAgent.ClientConnectByNumericIp(pIpAddress, nPort); 
	}
	return false;*/
	return g_NetConnectAgent.ClientConnectByNumericIp(pIpAddress, 5622); 

}

void KLogin::RegistNetAgent()
{
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_login, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_roleserver_getrolelist_result, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_notifyplayerlogin, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_rolenewdelresponse, this);
}

void KLogin::UnRegistNetAgent()
{
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_login, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_roleserver_getrolelist_result, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_notifyplayerlogin, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_rolenewdelresponse, NULL);
}

void KLogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
	int i;
	if (pszAccount)
	{
		strncpy(m_Choices.Account, pszAccount, sizeof(m_Choices.Account));
		for (i = 0; i < 32; i++)
			m_Choices.Account[i] = ~m_Choices.Account[i];
	}
	if (pcPassword)
	{
		m_Choices.Password = *pcPassword;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; i++)
			m_Choices.Password.szPassword[i] = ~m_Choices.Password.szPassword[i];
	}
}

void KLogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
	int i;
	if (pszAccount)
	{
		memcpy(pszAccount, m_Choices.Account, sizeof(m_Choices.Account));
		for (i = 0; i < 32; i++)
			pszAccount[i] = ~pszAccount[i];
	}
	if (pPassword)
	{
        *pPassword = m_Choices.Password;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; i++)
			pPassword->szPassword[i] = ~pPassword->szPassword[i];
	}
}