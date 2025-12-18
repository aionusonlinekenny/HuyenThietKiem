#include "Stdafx.h"
#include "GameServer.h"
#include "IPlayer.h"
#include "GamePlayer.h"
#include "msg_define.h"
#include <process.h>
#include <set>
#include <string>
#include <map>
#include <vector>
#include "AccountLoginDef.h"
#include "Macro.h"
#include "SmartClient.h"

using OnlineGameLib::Win32::CCriticalSection;
using OnlineGameLib::Win32::CEvent;
using OnlineGameLib::Win32::CBuffer;
using OnlineGameLib::Win32::CPackager;
using OnlineGameLib::Win32::ToString;
using OnlineGameLib::Win32::_tstring;

// ============================================
// Static Members Initialization
// ============================================
CBuffer::Allocator CGameServer::m_theGlobalAllocator(1024 * 96, 200);
CCriticalSection CGameServer::m_csMapIDAction;
CGameServer::stdMapIDConvert CGameServer::m_theMapIDConvert;
CEvent CGameServer::m_shQuitEvent(NULL, true, false, NULL);
CEvent CGameServer::m_shStartupManagerThreadEvent(NULL, false, false, NULL);
HANDLE CGameServer::m_shManagerThread = NULL;
CGameServer::stdGameSvr CGameServer::m_theGameServers;
CCriticalSection CGameServer::m_csGameSvrAction;
LONG CGameServer::m_slnIdentityCounts = 0L;
OnlineGameLib::Win32::CCriticalSection CGameServer::m_csTransfer;
CGameServer::TransferSet CGameServer::m_transferAccs;

// ============================================
// Transfer Timeout Tracking
// ============================================
struct TransferInfo {
    DWORD dwStartTime;
    int nFromGS;
    int nToGS;
    std::string account;
};
static std::map<std::string, TransferInfo> g_transferTimeouts;
static OnlineGameLib::Win32::CCriticalSection g_csTransferTimeout;
static const DWORD TRANSFER_TIMEOUT = 30000; // 30 seconds

// ============================================
// Global Functions
// ============================================
bool CGameServer::SetupGlobalAllocator(size_t bufferSize, size_t maxFreeBuffers)
{
    return CGameServer::m_theGlobalAllocator.ReSet(bufferSize, maxFreeBuffers);
}

void CGameServer::BeginTransfer(const char *acc)
{
    if (!acc || !acc[0]) return;
    
    OnlineGameLib::Win32::CCriticalSection::Owner lock1(m_csTransfer);
    OnlineGameLib::Win32::CCriticalSection::Owner lock2(g_csTransferTimeout);
    
    m_transferAccs.insert(std::string(acc));
    
    TransferInfo info;
    info.dwStartTime = ::GetTickCount();
    info.nFromGS = FindServerByAccount(acc);
    info.nToGS = -1;
    info.account = acc;
    g_transferTimeouts[acc] = info;
    
    printf("[BISHOP] BeginTransfer: %s at GS%d, time=%u\n", acc, info.nFromGS, info.dwStartTime);
}

void CGameServer::EndTransfer(const char *acc)
{
    if (!acc || !acc[0]) return;
    
    OnlineGameLib::Win32::CCriticalSection::Owner lock1(m_csTransfer);
    OnlineGameLib::Win32::CCriticalSection::Owner lock2(g_csTransferTimeout);
    
    TransferSet::iterator it = m_transferAccs.find(std::string(acc));
    if (it != m_transferAccs.end())
    {
        m_transferAccs.erase(it);
        printf("[BISHOP] EndTransfer: %s\n", acc);
    }
    g_transferTimeouts.erase(acc);
}

bool CGameServer::IsInTransfer(const char *acc)
{
    if (!acc || !acc[0]) return false;
    OnlineGameLib::Win32::CCriticalSection::Owner lock(m_csTransfer);
    return (m_transferAccs.find(std::string(acc)) != m_transferAccs.end());
}

void CGameServer::CleanupStaleTransfers()
{
    OnlineGameLib::Win32::CCriticalSection::Owner lock1(m_csTransfer);
    OnlineGameLib::Win32::CCriticalSection::Owner lock2(g_csTransferTimeout);
    
    DWORD dwNow = ::GetTickCount();
    std::vector<std::string> toRemove;
    
    for (std::map<std::string, TransferInfo>::iterator it = g_transferTimeouts.begin(); 
         it != g_transferTimeouts.end(); ++it)
    {
        DWORD elapsed = dwNow - it->second.dwStartTime;
        if (dwNow < it->second.dwStartTime)
            elapsed = (0xFFFFFFFF - it->second.dwStartTime) + dwNow;
        
        if (elapsed > TRANSFER_TIMEOUT)
        {
            printf("[BISHOP] [WARNING] Transfer timeout for %s (elapsed=%u ms)\n", 
                   it->first.c_str(), elapsed);
            toRemove.push_back(it->first);
        }
    }
    
    for (size_t i = 0; i < toRemove.size(); ++i)
    {
        m_transferAccs.erase(toRemove[i]);
        g_transferTimeouts.erase(toRemove[i]);
        
        int nGS = FindServerByAccount(toRemove[i].c_str());
        if (nGS != -1)
        {
            IGServer *pGS = GetServer(nGS);
            if (pGS)
                ((CGameServer*)pGS)->PopAccount(toRemove[i].c_str(), true);
        }
    }
}

static void GenGuid(GUID *guid)
{
    ASSERT(guid);
    static char szData[9] = { 0 };
    static DWORD dwBase = 0;
    
    if (FAILED(::CoCreateGuid(guid)))
    {
        if ((dwBase >> 31) & 0x1)
            dwBase = 0;
        
        guid->Data1 = dwBase++;
        guid->Data2 = (DWORD)rand() & 0xFFFF;
        guid->Data3 = ((DWORD)rand() >> 16) & 0xFFFF;
        sprintf((char *)szData, "%d%d", ((DWORD)rand() + dwBase++), (DWORD)rand());
        memcpy(guid->Data4, szData, 8);
    }
}

// ============================================
// Constructor/Destructor
// ============================================
CGameServer::CGameServer(IServer *pGameSvrServer, IClient *pAccountClient, UINT nIdentityID)
    : m_lnIdentityID(nIdentityID)
    , m_pGameSvrServer(pGameSvrServer)
    , m_nServerIP_Internet(0)
    , m_nServerIP_Intraner(0)
    , m_nServerPort(0)
    , m_dwCapability(-1)
{
    LONG lnID = ::InterlockedExchangeAdd(&m_slnIdentityCounts, 1);
    m_lnIdentityID = ((UINT)(-1) == m_lnIdentityID) ? lnID : m_lnIdentityID;
}

CGameServer::~CGameServer()
{
    m_thePackager.Empty();
    ::InterlockedExchangeAdd(&m_slnIdentityCounts, -1);
}

// ============================================
// Create/Destroy
// ============================================
bool CGameServer::Create()
{
    ZeroMemory(m_theProcessProtocolFun, sizeof(m_theProcessProtocolFun));
    m_theProcessProtocolFun[c2s_registeraccount] = &CGameServer::_RegisterAccount;
    m_theProcessProtocolFun[c2s_entergame] = &CGameServer::_NotifyEnterGame;
    m_theProcessProtocolFun[c2s_leavegame] = &CGameServer::_NotifyLeaveGame;
    m_theProcessProtocolFun[c2s_permitplayerlogin] = &CGameServer::_NotifyPlayerLogin;
    m_theProcessProtocolFun[c2s_updatemapinfo] = &CGameServer::_UpdateMapID;
    m_theProcessProtocolFun[c2s_updategameserverinfo] = &CGameServer::_UpdateGameSvrInfo;
    m_theProcessProtocolFun[c2s_requestsvrip] = &CGameServer::_RequestSvrIP;
    _QueryGameSvrInfo();
    return _QueryMapID();
}

bool CGameServer::Destroy()
{
    {
        CCriticalSection::Owner lock(m_csAITS);
        stdAccountAttachIn::iterator it;
        for (it = m_theAccountInThisServer.begin(); it != m_theAccountInThisServer.end(); it++)
        {
            std::string sAccountName = (*it).first;
            FreezeMoney(sAccountName.c_str());
        }
    }
    
    m_dwCapability = 0;
    m_nServerPort = 0;
    m_nServerIP_Intraner = 0;
    m_nServerIP_Internet = 0;
    m_sServerIPAddr_Internet = "";
    m_sServerIPAddr_Intraner = "";
    
    {
        CCriticalSection::Owner locker(m_csMapIDAction);
        stdMapIDConvert::iterator itM2C;
        for (itM2C = m_theMapIDConvert.begin(); itM2C != m_theMapIDConvert.end(); itM2C++)
        {
            stdServerList &sl = (*itM2C).second;
            if (!sl.empty())
                sl.remove(this);
        }
    }
    return true;
}

// ============================================
// Event Notification
// ============================================
void __stdcall CGameServer::GameSvrEventNotify(LPVOID lpParam, const unsigned long &ulnID, const unsigned long &ulnEventType)
{
    CGameServer::LPNI pNI = reinterpret_cast<CGameServer::NI *>(lpParam);
    ASSERT(pNI);
    CCriticalSection::Owner locker(CGameServer::m_csGameSvrAction);
    
    switch (ulnEventType)
    {
    case enumClientConnectCreate:
        {
            IGServer *pGServer = new CGameServer(pNI->pServer, pNI->pClient, ulnID);
            ASSERT(pGServer);
            pGServer->Create();
            std::pair<CGameServer::stdGameSvr::iterator, bool> result = 
                CGameServer::m_theGameServers.insert(CGameServer::stdGameSvr::value_type(ulnID, pGServer));
            if (result.second && pNI->hwndContainer && ::IsWindow(pNI->hwndContainer))
                ::PostMessage(pNI->hwndContainer, WM_GAMESERVER_EXCHANGE, ADD_GAMESERVER_ACTION, ulnID);
        }
        break;
    case enumClientConnectClose:
        {
            stdGameSvr::iterator it;
            if (CGameServer::m_theGameServers.end() != (it = CGameServer::m_theGameServers.find(ulnID)))
            {
                if (pNI->hwndContainer && ::IsWindow(pNI->hwndContainer))
                    ::PostMessage(pNI->hwndContainer, WM_GAMESERVER_EXCHANGE, DEL_GAMESERVER_ACTION, ulnID);
                IGServer *pGServer = (*it).second;
                ASSERT(pGServer);
                CGameServer::m_theGameServers.erase(it);
                pGServer->Destroy();
                SAFE_DELETE(pGServer);
            }
        }
        break;
    }
}

// ============================================
// Begin/End
// ============================================
bool CGameServer::Begin(IServer *pGameSvrServer)
{
    DWORD dwThreadID = 0;
    m_shManagerThread = ::CreateThread(NULL, 0, ManagerThreadFunction, (void *)pGameSvrServer, 0, &dwThreadID);
    if (m_shManagerThread == INVALID_HANDLE_VALUE)
        return false;
    m_shStartupManagerThreadEvent.Set();
    return true;
}

void CGameServer::End()
{
    m_shQuitEvent.Set();
    m_shStartupManagerThreadEvent.Set();
    if (WAIT_TIMEOUT == ::WaitForSingleObject(m_shManagerThread, 5000))
        ::TerminateThread(m_shManagerThread, 0);
    SAFE_CLOSEHANDLE(m_shManagerThread);
    
    {
        CCriticalSection::Owner locker(m_csMapIDAction);
        stdMapIDConvert::iterator itM2C;
        for (itM2C = m_theMapIDConvert.begin(); itM2C != m_theMapIDConvert.end(); itM2C++)
        {
            stdServerList &SL = (*itM2C).second;
            SL.clear();
        }
        m_theMapIDConvert.erase(m_theMapIDConvert.begin(), m_theMapIDConvert.end());
    }
    
    {
        CCriticalSection::Owner locker(m_csGameSvrAction);
        m_theGameServers.erase(m_theGameServers.begin(), m_theGameServers.end());
    }
}

// ============================================
// Manager Thread (FIXED v2 - eliminate lock contention)
// FIX: Don't hold m_csGameSvrAction while processing packets
// This was causing 4-6s lock contention when many packets need processing
// ============================================
DWORD WINAPI CGameServer::ManagerThreadFunction(void *pParam)
{
    IServer *pGameSvrServer = (IServer *)pParam;
    ASSERT(pGameSvrServer);
    m_shStartupManagerThreadEvent.Wait();

    stdGameSvr::iterator it;
    DWORD dwLastCleanup = ::GetTickCount();
    const DWORD CLEANUP_INTERVAL = 5000; // 5 seconds

    // Pre-allocate arrays for server info
    struct ServerInfo {
        UINT nlnID;
        IGServer *pServer;
    };
    ServerInfo servers[10];  // Support up to 10 GameServers

    while (!m_shQuitEvent.Wait(0))
    {
        // Step 1: Get server info with lock held (FAST - just copying pointers)
        int serverCount = 0;
        {
            CCriticalSection::Owner locker(CGameServer::m_csGameSvrAction);
            for (it = CGameServer::m_theGameServers.begin();
                 it != CGameServer::m_theGameServers.end() && serverCount < 10;
                 it++)
            {
                servers[serverCount].nlnID = (*it).first;
                servers[serverCount].pServer = (*it).second;
                serverCount++;
            }
        }  // Lock released here!

        // Step 2: Process packets WITHOUT holding m_csGameSvrAction (NO contention!)
        for (int i = 0; i < serverCount; i++)
        {
            size_t datalength = 0;
            const void *pData = pGameSvrServer->GetPackFromClient(servers[i].nlnID, datalength);
            if (datalength > 0 && pData && servers[i].pServer)
            {
											  
						 
                servers[i].pServer->AnalyzeRequire(pData, datalength);
            }
        }

        // FIX: Periodic cleanup of stale transfers
        DWORD dwNow = ::GetTickCount();
        if ((dwNow - dwLastCleanup) > CLEANUP_INTERVAL || dwNow < dwLastCleanup)
        {
            dwLastCleanup = dwNow;
            CleanupStaleTransfers();
        }
        ::Sleep(3);
    }
    SAFE_RELEASE(pGameSvrServer);
    return 0;
}

// ============================================
// Protocol Processing
// ============================================
bool CGameServer::AnalyzeRequire(const void *pData, size_t datalength)
{
    BYTE cProtocol = CPackager::Peek(pData);
    if (cProtocol < g_nGlobalProtocolType)
        return LargePackProcess(cProtocol, pData, datalength);
    else if (cProtocol > g_nGlobalProtocolType)
        return SmallPackProcess(cProtocol, pData, datalength);
    return false;
}

bool CGameServer::LargePackProcess(BYTE cProtocol, const void *pData, size_t datalength)
{
    return true;
}

bool CGameServer::SmallPackProcess(BYTE cProtocol, const void *pData, size_t datalength)
{
    if (cProtocol < c2s_end && m_theProcessProtocolFun[cProtocol])
        return (this->*m_theProcessProtocolFun[cProtocol])((const void *)pData, datalength);
    return false;
}

bool CGameServer::DispatchTask(UINT nTask, const void *pData, size_t datalength)
{
    bool ok = true;
    switch (nTask)
    {
    case enumSyncRoleInfo:
        ok = _SyncRoleInfo(pData, datalength);
        break;
    case enumPlayerLogicLogout:
        ok = _PlayerLogicLogout(pData, datalength);
        break;
    case enumTaskProtocol:
    default:
        break;
    }
    return ok;
}

// ============================================
// Protocol Handlers
// ============================================
bool CGameServer::_QueryMapID()
{
    tagQueryMapInfo qmi;
    qmi.cProtocol = s2c_querymapinfo;
    m_pGameSvrServer->SendData(m_lnIdentityID, (const void *)&qmi, sizeof(tagQueryMapInfo));
    return true;
}

bool CGameServer::_QueryGameSvrInfo()
{
    tagQueryGameSvrInfo qgsi;
    qgsi.cProtocol = s2c_querygameserverinfo;
    m_pGameSvrServer->SendData(m_lnIdentityID, (const void *)&qgsi, sizeof(tagQueryGameSvrInfo));
    return true;
}

bool CGameServer::_UpdateMapID(const void *pData, size_t datalength)
{
    tagUpdateMapID *pUMI = (tagUpdateMapID *)pData;
    int nMapCount = pUMI->cMapCount;
    short *pMapID = pUMI->szMapID;
    while (--nMapCount >= 0)
        RegisterServer(pMapID[nMapCount], (IGServer *)this);
    return true;
}

bool CGameServer::_UpdateGameSvrInfo(const void *pData, size_t datalength)
{
    ASSERT(sizeof(tagGameSvrInfo) == datalength);
    tagGameSvrInfo *pGSI = (tagGameSvrInfo *)pData;
    m_dwCapability = pGSI->wCapability;
    m_nServerPort = pGSI->nPort;
    m_nServerIP_Internet = pGSI->nIPAddr_Internet;
    m_nServerIP_Intraner = pGSI->nIPAddr_Intraner;
    m_sServerIPAddr_Intraner = OnlineGameLib::Win32::net_ntoa(m_nServerIP_Intraner);
    m_sServerIPAddr_Internet = OnlineGameLib::Win32::net_ntoa(m_nServerIP_Internet);
    return true;
}

bool CGameServer::_RequestSvrIP(const void *pData, size_t datalength)
{
    tagRequestSvrIp *pRSI = (tagRequestSvrIp *)pData;
    ASSERT(sizeof(tagRequestSvrIp) == datalength);
    DWORD dwIP = 0;
    IGServer *pGServer = CGameServer::QueryServer(pRSI->dwMapID);
    if (pGServer)
        dwIP = pGServer->GetIP(pRSI->cIPType);
    tagNotifySvrIp nsi;
    nsi.cProtocol = s2c_notifysvrip;
    nsi.pckgID = pRSI->pckgID;
    nsi.dwMapID = pRSI->dwMapID;
    nsi.cIPType = pRSI->cIPType;
    nsi.dwSvrIP = dwIP;
    m_pGameSvrServer->SendData(m_lnIdentityID, (const void *)&nsi, sizeof(tagNotifySvrIp));
    return true;
}

bool CGameServer::_NotifyPlayerLogin(const void *pData, size_t datalength)
{
    tagPermitPlayerLogin *pPPL = (tagPermitPlayerLogin *)pData;
    ASSERT(sizeof(tagPermitPlayerLogin) == datalength);
    bool ok = false;
    IPlayer *pPlayer = CGamePlayer::Get((const char *)(pPPL->szRoleName));
    if (pPlayer)
    {
        tagNotifyPlayerLogin npl;
        npl.cProtocol = s2c_notifyplayerlogin;
        int nMinLen = strlen((const char *)pPPL->szRoleName);
        nMinLen = nMinLen > sizeof(npl.szRoleName) ? sizeof(npl.szRoleName) : nMinLen;
        memcpy((char *)npl.szRoleName, (const char *)pPPL->szRoleName, nMinLen);
        npl.szRoleName[nMinLen] = '\0';
        memcpy(&(npl.guid), &(pPPL->guid), sizeof(GUID));
        npl.bPermit = pPPL->bPermit;
        npl.nIPAddr = m_nServerIP_Internet;
        npl.nPort = m_nServerPort;
        ok = pPlayer->AppendData(CGamePlayer::enumOwnerPlayer, (const void *)&npl, sizeof(tagNotifyPlayerLogin));
    }
    return ok;
}

bool CGameServer::_RegisterAccount(const void *pData, size_t datalength)
{
    ASSERT(pData && datalength);
    tagRegisterAccount *pRA = (tagRegisterAccount *)pData;
    if (sizeof(tagRegisterAccount) != datalength)
        return false;
    AttatchAccountToGameServer((const char *)pRA->szAccountName);
    return true;
}

// ============================================
// _NotifyEnterGame (FIXED)
// ============================================
bool CGameServer::_NotifyEnterGame(const void *pData, size_t datalength)
{
    ASSERT(pData && datalength);
    tagEnterGame *pEG = (tagEnterGame *)pData;
    if (sizeof(tagEnterGame) != datalength)
        return false;
    
    const char *szAcc = (const char *)pEG->szAccountName;
    bool isTransferring = IsInTransfer(szAcc);
    int nOldGS = FindServerByAccount(szAcc);
    int nThisGS = (int)GetID();
    
    printf("[BISHOP] EnterGame: %s, OldGS=%d, ThisGS=%d, Transfer=%d\n", szAcc, nOldGS, nThisGS, isTransferring);
    
    // Case 1: Normal login (not in any GS)
    if (nOldGS == -1)
        return PushAccount(szAcc);
    
    // Case 2: Already in this GS (shouldn't happen)
    if (nOldGS == nThisGS)
    {
        printf("[BISHOP] [WARNING] Duplicate EnterGame for %s in GS%d\n", szAcc, nThisGS);
        return true;
    }
    
    // Case 3: Transfer from another GS
    IGServer *pOld = GetServer(nOldGS);
    if (pOld)
    {
        if (isTransferring)
        {
            printf("[BISHOP] Transfer confirmed: %s from GS%d to GS%d\n", szAcc, nOldGS, nThisGS);
            ((CGameServer *)pOld)->DetachAccountFromGameServer(szAcc);
            EndTransfer(szAcc);
        }
        else
        {
            printf("[BISHOP] [WARNING] Unexpected move: %s from GS%d to GS%d\n", szAcc, nOldGS, nThisGS);
            pOld->DispatchTask(CGameServer::enumPlayerLogicLogout, szAcc, (int)strlen(szAcc) + 1);
        }
    }
    return PushAccount(szAcc);
}

// ============================================
// _NotifyLeaveGame (FIXED)
// ============================================
bool CGameServer::_NotifyLeaveGame(const void *pData, size_t datalength)
{
    ASSERT(pData && datalength);
    tagLeaveGame *pLG = (tagLeaveGame *)pData;
    ASSERT(sizeof(tagLeaveGame) == datalength);
    const char *pAccountName = (const char *)(pLG->szAccountName);

    bool isTransferring = IsInTransfer(pAccountName);
    bool hold = (pLG->cCmdType == HOLDACC_LEAVEGAME);
    bool inGS = HaveAccountInGameServer(pAccountName);

    printf("[BISHOP] LeaveGame: %s, GS=%d, InGS=%d, Transfer=%d, Hold=%d\n",
           pAccountName, (int)GetID(), inGS ? 1 : 0, isTransferring ? 1 : 0, hold ? 1 : 0);

    // FIX: Check if this account is actually in this GS
    if (!inGS)
    {
        printf("[BISHOP] [WARNING] LeaveGame for %s but NOT in GS%d - ignoring\n",
               pAccountName, (int)GetID());
        return false;
    }

    // CRITICAL: Do NOT release g_ActiveAccounts here!
    // LeaveGame message is unreliable - called both on enter and leave
    // Only Inactive() should manage g_ActiveAccounts lifecycle
    // This function only handles GS-level detachment

    if (isTransferring)
    {
        BeginTransfer(pAccountName);
        bool result = PopAccount(pAccountName, false);
        printf("[BISHOP] LeaveGame (already transferring): %s detached=%d\n", pAccountName, result ? 1 : 0);
        return result;
    }

    // FIX: If HOLDACC_LEAVEGAME received, this is a cross-GS transfer
    // Must call BeginTransfer() to track account in transfer state
    // This allows EndTransfer() to work correctly when player enters new GS
    if (hold)
    {
        BeginTransfer(pAccountName);
        bool result = PopAccount(pAccountName, false);
        printf("[BISHOP] LeaveGame (hold for transfer): %s detached=%d, tracking transfer\n",
               pAccountName, result ? 1 : 0);
        return result;
    }

    // Normal logout - unlock account
    bool result = PopAccount(pAccountName, true);
    printf("[BISHOP] LeaveGame (normal logout): %s detached=%d, unlocked=%d\n",
           pAccountName, result ? 1 : 0, 1);
    return result;
}

// ============================================
// Account Management
// ============================================
bool CGameServer::PushAccount(const char *pAccountName)
{
    ASSERT(pAccountName);
    return ConsumeMoney(pAccountName);
}

bool CGameServer::PopAccount(const char *pAccountName, bool bUnlockAccount)
{
    ASSERT(pAccountName);
    if (HaveAccountInGameServer(pAccountName))
    {
        if (bUnlockAccount)
            FreezeMoney(pAccountName);
        DetachAccountFromGameServer(pAccountName);
        return true;
    }
    return false;
}

// ============================================
// Attach (FIXED)
// ============================================
bool CGameServer::Attach(const char *pAccountName, bool bCheck)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    
    // Check if already in this GS
    if (HaveAccountInGameServer(pAccountName))
    {
        printf("[BISHOP] [WARNING] Account %s already in GS%d\n", pAccountName, (int)GetID());
        return true;
    }
    
    int nExistingGS = FindServerByAccount(pAccountName);
    int nThisGS = (int)GetID();

    if (nExistingGS != -1 && nExistingGS != nThisGS)
    {
        bool isTransferring = IsInTransfer(pAccountName);
        printf("[BISHOP] Attach: %s from GS%d to GS%d, Transfer=%d\n", pAccountName, nExistingGS, nThisGS, isTransferring);
        if (!isTransferring)
            BeginTransfer(pAccountName);

        // FIX: Do NOT call DispatchTask(enumPlayerLogicLogout) to old GS
        // This is a BLOCKING synchronous call that can take 20+ seconds if old GS is busy!
        // Old GS will detect player disconnect and cleanup automatically.
        // Just detach account from old GS locally without waiting for response.
        IGServer *pOld = GetServer(nExistingGS);
        if (pOld)
        {
            CGameServer *pOldGS = static_cast<CGameServer*>(pOld);
            if (pOldGS)
            {
                pOldGS->DetachAccountFromGameServer(pAccountName);
                printf("[BISHOP] Attach: Detached \"%s\" from old GS%d (no logout signal)\n",
                       pAccountName, nExistingGS);
            }
        }
    }
    return AttatchAccountToGameServer(pAccountName, bCheck);
}

bool CGameServer::AttatchAccountToGameServer(const char *pAccountName, bool bCheck)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    if (_NAME_LEN <= strlen(pAccountName))
        return false;
    OnlineGameLib::Win32::CCriticalSection::Owner lock(m_csAITS);
    if (m_theAccountInThisServer.find(pAccountName) != m_theAccountInThisServer.end())
        return false;

    // CRITICAL: Only insert if NOT in check mode
    // bCheck=true means "check only, don't attach"
    // bCheck=false means "actually attach the account"
    if (!bCheck)
    {
        m_theAccountInThisServer.insert(stdAccountAttachIn::value_type(pAccountName, reinterpret_cast<void *>(this)));
        printf("[BISHOP] AttatchAccount: \"%s\" attached to GS%d\n", pAccountName, (int)GetID());
    }
    return true;
}

bool CGameServer::HaveAccountInGameServer(const char *pAccountName)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    if (_NAME_LEN > strlen(pAccountName))
    {
        CCriticalSection::Owner lock(m_csAITS);
        stdAccountAttachIn::iterator it;
        if (m_theAccountInThisServer.end() != (it = m_theAccountInThisServer.find(pAccountName)))
            return true;
    }
    return false;
}

bool CGameServer::DetachAccountFromGameServer(const char *pAccountName)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    if (_NAME_LEN > strlen(pAccountName))
    {
        CCriticalSection::Owner lock(m_csAITS);
        stdAccountAttachIn::iterator it;
        if (m_theAccountInThisServer.end() != (it = m_theAccountInThisServer.find(pAccountName)))
        {
            CGameServer *pThis = reinterpret_cast<CGameServer *>((*it).second);
            ASSERT(pThis && pThis == this);
            m_theAccountInThisServer.erase(it);
            return true;
        }
    }
    return false;
}

// ============================================
// Payment System Integration
// ============================================
bool CGameServer::ConsumeMoney(const char *pAccountName)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    if (_NAME_LEN <= strlen(pAccountName))
        return false;
    CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
    BYTE *pContext = const_cast<BYTE *>(pBuffer->GetBuffer());
    const size_t len = sizeof(KAccountUser) + 1;
    KAccountUser user;
    user.Size = sizeof(KAccountUser);
    user.Type = AccountUser;
    user.Version = ACCOUNT_CURRENT_VERSION;
    size_t length = strlen(pAccountName);
    length = (length > LOGIN_USER_ACCOUNT_MAX_LEN) ? LOGIN_USER_ACCOUNT_MAX_LEN : length;
    memcpy(user.Account, pAccountName, length);
    user.Account[length] = '\0';
    *pContext = c2s_gamelogin;
    memcpy(pContext + 1, &user, sizeof(KAccountUser));
    g_theSmartClient.Send(pContext, len);
    SAFE_RELEASE(pBuffer);
    return true;
}

bool CGameServer::FreezeMoney(const char *pAccountName)
{
    if (!pAccountName || !pAccountName[0])
        return false;
    if (_NAME_LEN <= strlen(pAccountName))
        return false;
    CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
    BYTE *pData = const_cast<BYTE *>(pBuffer->GetBuffer());
    const size_t datalength = sizeof(KAccountUserLogout) + 1;
    KAccountUserLogout user;
    user.Size = sizeof(KAccountUserLogout);
    user.Type = AccountUserLogout;
    user.Version = ACCOUNT_CURRENT_VERSION;
    size_t length = strlen(pAccountName);
    length = (length > LOGIN_USER_ACCOUNT_MAX_LEN) ? LOGIN_USER_ACCOUNT_MAX_LEN : length;
    memcpy(user.Account, pAccountName, length);
    user.Account[length] = '\0';
    *pData = c2s_accountlogout;
    memcpy(pData + 1, &user, sizeof(KAccountUserLogout));
    g_theSmartClient.Send((const void *)pData, datalength);
    SAFE_RELEASE(pBuffer);
    return true;
}

// ============================================
// Internal Message Handlers
// ============================================
bool CGameServer::_PlayerLogicLogout(const void *pData, size_t datalength)
{
    tagLogicLogout ll;
    if (sizeof(ll.szRoleName) < datalength)
        return false;
    ll.cProtocol = s2c_logiclogout;
    memcpy(ll.szRoleName, pData, datalength);
    m_pGameSvrServer->SendData(m_lnIdentityID, (const void *)&ll, sizeof(tagLogicLogout));
    return true;
}

bool CGameServer::_SyncRoleInfo(const void *pData, size_t datalength)
{
    tagGuidableInfo ginfo;
    ginfo.cProtocol = s2c_syncgamesvr_roleinfo_cipher;
    GenGuid(&(ginfo.guid));
    ginfo.bCheckOff = 0;
    ginfo.datalength = datalength;
    m_thePackager.AddData(s2c_syncgamesvr_roleinfo_cipher, (const char *)&ginfo, sizeof(tagGuidableInfo));
    m_thePackager.AddData(s2c_syncgamesvr_roleinfo_cipher, (const char *)pData, datalength);
    
    CBuffer *pBuffer = m_thePackager.GetHeadPack(s2c_syncgamesvr_roleinfo_cipher);
    while (pBuffer)
    {
        m_pGameSvrServer->SendData(m_lnIdentityID, pBuffer->GetBuffer(), pBuffer->GetUsed());
        SAFE_RELEASE(pBuffer);
        pBuffer = m_thePackager.GetNextPack(s2c_syncgamesvr_roleinfo_cipher);
    }
    SAFE_RELEASE(pBuffer);
    m_thePackager.DelData(s2c_syncgamesvr_roleinfo_cipher);
    return true;
}

// ============================================
// Server Registration and Query
// ============================================
bool CGameServer::RegisterServer(UINT nID, IGServer *pGServer)
{
    ASSERT(pGServer);
    CCriticalSection::Owner locker(m_csMapIDAction);
    stdMapIDConvert::iterator it;
    
    if (m_theMapIDConvert.end() != (it = m_theMapIDConvert.find(nID)))
    {
        stdServerList& sl = (*it).second;
        sl.push_back(pGServer);
    }
    else
    {
        stdServerList sl;
        sl.push_back(pGServer);
        std::pair<stdMapIDConvert::iterator, bool> result =
            m_theMapIDConvert.insert(stdMapIDConvert::value_type(nID, sl));
        return result.second;
    }
    return true;
}

IGServer *CGameServer::QueryServer(UINT nMapID)
{
    CCriticalSection::Owner locker(m_csMapIDAction);
    stdMapIDConvert::iterator it;
    
    if (m_theMapIDConvert.end() != (it = m_theMapIDConvert.find(nMapID)))
    {
        stdServerList& sl = (*it).second;
        if (!sl.empty())
        {
            IGServer *pGServer = NULL;
            stdServerList::iterator it;
            for (it = sl.begin(); it != sl.end(); it++)
            {
                pGServer = (IGServer *)(*it);
                ASSERT(pGServer);
                if (NULL == pGServer)
                    continue;
                if (pGServer->GetContent() < pGServer->GetCapability())
                    return pGServer;
            }
        }
    }
    return NULL;
}

// ============================================
// FindServerByAccount (FIXED v2)
// FIX: Don't hold m_csGameSvrAction while calling HaveAccountInGameServer()
// This was causing 8+ second lock contention during concurrent login/logout
// ============================================
int CGameServer::FindServerByAccount(const char* szAcc)
{
    if (!szAcc || !szAcc[0])
        return -1;

    // Step 1: Get all server pointers with lock held (FAST)
    CGameServer *servers[10];  // Support up to 10 GameServers
    int serverCount = 0;
    {
        OnlineGameLib::Win32::CCriticalSection::Owner locker(m_csGameSvrAction);
        stdGameSvr::iterator it = m_theGameServers.begin();
        for (; it != m_theGameServers.end() && serverCount < 10; ++it)
        {
            CGameServer* pSrv = (CGameServer*)(it->second);
            if (pSrv)
                servers[serverCount++] = pSrv;
        }
    }  // Lock released here!

    // Step 2: Check each server WITHOUT holding m_csGameSvrAction (NO contention!)
    for (int i = 0; i < serverCount; i++)
    {
        if (servers[i]->HaveAccountInGameServer(szAcc))
            return (int)servers[i]->GetID();
    }
    return -1;
}

IGServer *CGameServer::GetServer(size_t nID)
{
    CCriticalSection::Owner locker(CGameServer::m_csGameSvrAction);
    stdGameSvr::iterator it;
    if (CGameServer::m_theGameServers.end() != (it = CGameServer::m_theGameServers.find(nID)))
    {
        IGServer *pGServer = (*it).second;
        ASSERT(pGServer);
        return pGServer;
    }
    return NULL;
}

// ============================================
// GetAllServers - Retrieve all GameServers with single lock acquisition
// ============================================
void CGameServer::GetAllServers(CGameServer **pOutServers, int nMaxServers)
{
    if (!pOutServers || nMaxServers <= 0)
        return;

    // Initialize output array
    for (int i = 0; i < nMaxServers; i++)
        pOutServers[i] = NULL;

    // Acquire lock ONCE and get all servers
    CCriticalSection::Owner locker(CGameServer::m_csGameSvrAction);
    stdGameSvr::iterator it;
    for (it = CGameServer::m_theGameServers.begin(); it != CGameServer::m_theGameServers.end(); ++it)
    {
        CGameServer *pGS = static_cast<CGameServer*>(it->second);
        if (pGS)
        {
            size_t nID = pGS->GetID();
            if (nID >= 1 && nID <= (size_t)nMaxServers)
                pOutServers[nID - 1] = pGS;  // Store in 0-indexed array
        }
    }
    // Lock automatically released here
}

// ============================================											   
// Content Management
// ============================================
size_t CGameServer::GetContent()
{
    CCriticalSection::Owner lock(m_csAITS);
    return m_theAccountInThisServer.size();
}

// ============================================
// Broadcasting
// ============================================
void CGameServer::SendToAll(const char *pText, int nLength, UINT uOption)
{
    stdGameSvr::iterator it;
    CCriticalSection::Owner locker(CGameServer::m_csGameSvrAction);
    for (it = CGameServer::m_theGameServers.begin(); it != CGameServer::m_theGameServers.end(); it++)
    {
        IGServer *pGServer = (*it).second;
        if (pGServer)
            pGServer->SendText(pText, nLength, uOption);
    }
}

void CGameServer::SendText(const char *pText, int nLength, UINT uOption)
{
    if (!pText || 0 == nLength || !m_pGameSvrServer)
        return;
    tagGatewayBroadCast gbc;
    gbc.cProtocol = s2c_gateway_broadcast;
    gbc.uCmdType = uOption;
    int nLen = 0;
    if (sizeof(gbc.szData) > nLength)
    {
        strcpy(gbc.szData, pText);
        nLen = nLength;
    }
    gbc.szData[nLen] = '\0';
    m_pGameSvrServer->SendData(m_lnIdentityID, (const void *)&gbc, sizeof(tagGatewayBroadCast));
}