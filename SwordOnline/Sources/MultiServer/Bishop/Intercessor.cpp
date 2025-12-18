#include "stdafx.h"
#include "Intercessor.h"

#include "AccountLoginDef.h"
#include "LoginDef.h"

#include "KProtocolDef.h"
#include "KProtocol.h"

#include "Macro.h"
#include "Buffer.h"

#include "SmartClient.h"

using OnlineGameLib::Win32::CEvent;
using OnlineGameLib::Win32::CCriticalSection;
using OnlineGameLib::Win32::CPackager;

// ============================================
// Static Members
// ============================================
OnlineGameLib::Win32::CLibrary CIntercessor::m_theHeavenLib("heaven.dll");
OnlineGameLib::Win32::CLibrary CIntercessor::m_theRainbowLib("rainbow.dll");

// ============================================
// FIX: Ping/Pong Tracking
// ============================================
struct PingInfo {
    DWORD dwLastPingSent;
    DWORD dwLastPongReceived;
    int nMissedPings;
    bool bConnected;
};
static PingInfo g_pingInfo = {0, 0, 0, false};
static OnlineGameLib::Win32::CCriticalSection g_csPingInfo;
static const DWORD PING_INTERVAL = 15000;      // 30 seconds (increased from 10s for DDoS protection)
static const DWORD PING_TIMEOUT = 90000;       // 90 seconds (increased from 30s for DDoS protection)
static const int MAX_MISSED_PINGS = 5;         // Increased from 3 for better tolerance

// ============================================
// Constructor/Destructor
// ============================================
CIntercessor::CIntercessor(unsigned long lnMaxPlayerCount, CNetwork &theNetworkConfig)
    : m_hwndViewer(NULL)
    , m_pDBRoleClient(NULL)
    , m_pGameSvrServer(NULL)
    , m_pPlayerServer(NULL)
    , m_hWorkingThread(INVALID_HANDLE_VALUE)
    , m_hHelperThread(INVALID_HANDLE_VALUE)
    , m_hQuitEvent(NULL, true, false, NULL)
    , m_hStartupWoringThreadEvent(NULL, false, false, NULL)
    , m_hStartupHelperThreadEvent(NULL, false, false, NULL)
    , m_theNetworkConfig(theNetworkConfig)
    , m_lnMaxPlayerCount(lnMaxPlayerCount)
{
}

CIntercessor::~CIntercessor()
{
}

// ============================================
// Create/Destroy
// ============================================
bool CIntercessor::Create(HWND hwndViewer)
{
    m_hwndViewer = hwndViewer;
    
    // FIX: Initialize ping info
    {
        OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
        g_pingInfo.dwLastPingSent = 0;
        g_pingInfo.dwLastPongReceived = 0;
        g_pingInfo.nMissedPings = 0;
        g_pingInfo.bConnected = false;
    }
    
    // Start working thread
    DWORD dwThreadID = 0;
    m_hWorkingThread = ::CreateThread(NULL, 0, WorkingThreadFunction, (void *)this, 0, &dwThreadID);
    if (m_hWorkingThread == INVALID_HANDLE_VALUE)
        return false;
    
    // Start helper thread
    dwThreadID = 0;
    m_hHelperThread = ::CreateThread(NULL, 0, HelperThreadFunction, (void *)this, 0, &dwThreadID);
    if (m_hHelperThread == INVALID_HANDLE_VALUE)
        return false;
    
    return StartupNetwork();
}

void CIntercessor::Destroy()
{
    m_hQuitEvent.Set();
    m_hStartupWoringThreadEvent.Set();
    m_hStartupHelperThreadEvent.Set();
    
    SAFE_CLOSEHANDLE(m_hWorkingThread);
    SAFE_CLOSEHANDLE(m_hHelperThread);
    
    CleanNetwork();
}

// ============================================
// Event Notifications
// ============================================
void __stdcall CIntercessor::DBRoleEventNotify(LPVOID lpParam, const unsigned long &ulnEventType)
{
    CIntercessor *pPlayerManager = reinterpret_cast<CIntercessor *>(lpParam);
    ASSERT(pPlayerManager);
    
    try
    {
        pPlayerManager->_DBRoleEventNotify(ulnEventType);
    }
    catch(...)
    {
        TRACE("CIntercessor::DBRoleEventNotify exception!");
    }
}

void CIntercessor::_DBRoleEventNotify(const unsigned long &ulnEventType)
{
    switch(ulnEventType)
    {
    case enumServerConnectCreate:
        ::PostMessage(m_hwndViewer, WM_SERVER_STATUS, DBROLESERVER_NOTIFY, CONNECTED);
        break;
    case enumServerConnectClose:
        ::PostMessage(m_hwndViewer, WM_SERVER_STATUS, DBROLESERVER_NOTIFY, DICONNECTED);
        break;
    }
}

void __stdcall CIntercessor::PlayerEventNotify(LPVOID lpParam,
                                               const unsigned long &ulnID,
                                               const unsigned long &ulnEventType)
{
    CIntercessor *pPlayerManager = reinterpret_cast<CIntercessor *>(lpParam);
    ASSERT(pPlayerManager);
    
    try
    {
        pPlayerManager->_PlayerEventNotify(ulnID, ulnEventType);
    }
    catch(...)
    {
        TRACE("CIntercessor::PlayerEventNotify exception!");
    }
}

void CIntercessor::_PlayerEventNotify(const unsigned long &ulnID, const unsigned long &ulnEventType)
{
    switch (ulnEventType)
    {
    case enumClientConnectCreate:
        {
            IPlayer *pPlayer = m_thePlayers[ulnID];
            ASSERT(pPlayer);
            pPlayer->Active();
        }
        break;
    case enumClientConnectClose:
        {
            PLAYER_MAP::iterator it;
            if (m_thePlayers.end() != (it = m_thePlayers.find(ulnID)))
            {
                IPlayer *pPlayer = m_thePlayers[ulnID];
                ASSERT(pPlayer);
                pPlayer->Inactive();
            }
        }
        break;
    }
}

// ============================================
// Network Startup
// ============================================
bool CIntercessor::StartupNetwork()
{
    bool bPlayerServerSucceed = false;
    bool bDBRoleServerSucceed = false;
    bool bGameSvrServerSucceed = false;
    
    // For the db-role server
    pfnCreateClientInterface pClientFactroyFun = 
        (pfnCreateClientInterface)(m_theRainbowLib.GetProcAddress(_T("CreateInterface")));
    
    IClientFactory *pClientFactory = NULL;
    if (pClientFactroyFun && SUCCEEDED(pClientFactroyFun(IID_IClientFactory, reinterpret_cast<void **>(&pClientFactory))))
    {
        pClientFactory->SetEnvironment(1024 * 1024);
        pClientFactory->CreateClientInterface(IID_IESClient, reinterpret_cast<void **>(&m_pDBRoleClient));
        SAFE_RELEASE(pClientFactory);
    }
    
    if (m_pDBRoleClient)
    {
        m_pDBRoleClient->Startup();
        m_pDBRoleClient->RegisterMsgFilter(reinterpret_cast<void *>(this), DBRoleEventNotify);
        if (SUCCEEDED(m_pDBRoleClient->ConnectTo(m_theNetworkConfig.GetRoleSvrIP(), m_theNetworkConfig.GetRoleSvrPort())))
        {
            bDBRoleServerSucceed = true;
        }
    }
    
    // We open the heaven to wait for the rainbow
    pfnCreateServerInterface pServerFactroyFun = 
        (pfnCreateServerInterface)(m_theHeavenLib.GetProcAddress(_T("CreateInterface")));
    
    IServerFactory *pServerFactory = NULL;
    if (pServerFactroyFun && 
        SUCCEEDED(pServerFactroyFun(IID_IServerFactory, reinterpret_cast<void **>(&pServerFactory))))
    {
        pServerFactory->SetEnvironment(m_lnMaxPlayerCount, 10, 1000, 1024 * 8);
        pServerFactory->CreateServerInterface(IID_IIOCPServer, reinterpret_cast<void **>(&m_pPlayerServer));
        pServerFactory->CreateServerInterface(IID_IIOCPServer, reinterpret_cast<void **>(&m_pGameSvrServer));
        pServerFactory->Release();
    }
    
    if (m_pGameSvrServer)
    {
        m_pGameSvrServer->Startup();
        static CGameServer::NI ni;
        ni.pServer = m_pGameSvrServer;
        ni.pClient = NULL;
        ni.hwndContainer = m_hwndViewer;
        m_pGameSvrServer->RegisterMsgFilter(reinterpret_cast<void *>(&ni), CGameServer::GameSvrEventNotify);
        if (SUCCEEDED(m_pGameSvrServer->OpenService(INADDR_ANY, m_theNetworkConfig.GetGameSvrOpenPort())))
        {
            bGameSvrServerSucceed = true;
        }
    }
    
    if (m_pPlayerServer)
    {
        m_pPlayerServer->Startup();
        m_pPlayerServer->RegisterMsgFilter(reinterpret_cast<void *>(this), PlayerEventNotify);
        if (SUCCEEDED(m_pPlayerServer->OpenService(INADDR_ANY, m_theNetworkConfig.GetClientOpenPort())))
        {
            bPlayerServerSucceed = true;
        }
    }
    
    if (bPlayerServerSucceed && bDBRoleServerSucceed && bGameSvrServerSucceed)
    {
        IServer *pClonePlayer = NULL;
        m_pPlayerServer->QueryInterface(IID_IIOCPServer, reinterpret_cast<void **>(&pClonePlayer));
        
        IClient *pCloneDBRole = NULL;
        m_pDBRoleClient->QueryInterface(IID_IESClient, reinterpret_cast<void **>(&pCloneDBRole));
        
        ASSERT(pClonePlayer && pCloneDBRole);
        CGamePlayer::ATTACH_NETWORK(NULL, pClonePlayer, pCloneDBRole);
        
        // Create players
        {
            CCriticalSection::Owner lock(m_csPlayerAction);
            for (size_t index = 0; index < m_lnMaxPlayerCount; index++)
            {
                IPlayer *pPlayer = new CGamePlayer(index);
                ASSERT(pPlayer);
                m_thePlayers[index] = pPlayer;
            }
        }
        
        IServer *pCloneGameServer = NULL;
        m_pGameSvrServer->QueryInterface(IID_IIOCPServer, reinterpret_cast<void **>(&pCloneGameServer));
        CGameServer::Begin(pCloneGameServer);
        
        // FIX: Mark as connected
        {
            OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
            g_pingInfo.bConnected = true;
            g_pingInfo.dwLastPongReceived = ::GetTickCount();
        }
        
        m_hStartupWoringThreadEvent.Set();
        m_hStartupHelperThreadEvent.Set();
    }
    
    return (bPlayerServerSucceed && bDBRoleServerSucceed && bGameSvrServerSucceed);
}

// ============================================
// Network Cleanup
// ============================================
void CIntercessor::CleanNetwork()
{
    // Clear player info
    {
        CCriticalSection::Owner locker(m_csPlayerAction);
        PLAYER_MAP::iterator it;
        for (it = m_thePlayers.begin(); it != m_thePlayers.end(); it++)
        {
            IPlayer *pPlayer = (IPlayer *)((*it).second);
            ASSERT(pPlayer);
            SAFE_DELETE(pPlayer);
        }
        m_thePlayers.erase(m_thePlayers.begin(), m_thePlayers.end());
    }
    
    CGameServer::End();
    CGamePlayer::DETACH_NETWORK();
    
    if (m_pPlayerServer)
    {
        m_pPlayerServer->CloseService();
        m_pPlayerServer->Cleanup();
        SAFE_RELEASE(m_pPlayerServer);
    }
    
    if (m_pDBRoleClient)
    {
        m_pDBRoleClient->Cleanup();
        SAFE_RELEASE(m_pDBRoleClient);
    }
    
    if (m_pGameSvrServer)
    {
        m_pGameSvrServer->CloseService();
        m_pGameSvrServer->Cleanup();
        SAFE_RELEASE(m_pGameSvrServer);
    }
}
// ============================================
// Working Thread
// ============================================
DWORD WINAPI CIntercessor::WorkingThreadFunction(void *pV)
{
    CIntercessor *pThis = reinterpret_cast<CIntercessor *>(pV);
    ASSERT(pThis);
    
    try
    {
        pThis->Working();
    }
    catch(...)
    {
        ::MessageBox(NULL, "Run a working thread is failed!", "CIntercessor class", MB_OK);
    }
    return 0;
}

int CIntercessor::Working()
{
    m_hStartupWoringThreadEvent.Wait();
    
    while (!m_hQuitEvent.Wait(0))
    {
        // For some alive player
        {
            CCriticalSection::Owner locker(m_csPlayerAction);
            for (size_t index = 0; index < m_lnMaxPlayerCount; index++)
            {
                IPlayer *pPlayer = m_thePlayers[index];
                ASSERT(pPlayer);
                
                if (pPlayer && pPlayer->IsActive())
                {
                    // Get data from player
                    size_t datalength = 0;
                    const void *pData = m_pPlayerServer->GetPackFromClient(index, datalength);
                    if (pData && datalength)
                    {
                        AnalyzePlayerRequire(index, pData, datalength);
                    }
                    
                    // Execute work
                    pPlayer->Run();
                }
            }
        }
        // CRITICAL: Sleep to prevent system call flood
        // Working thread processes player packets - needs to be responsive
        // But Sleep(1ms) = 1000 loops/sec is excessive, causes VPS firewall issues
        // Sleep(10ms) = 100 loops/sec is sufficient for player packet processing
        ::Sleep(7);  // Increased from 5ms to reduce system calls by 50%
    }
    return 0;
}

// ============================================
// Helper Thread (FIXED với ping/pong tracking)
// ============================================
DWORD WINAPI CIntercessor::HelperThreadFunction(void *pV)
{
    CIntercessor *pThis = reinterpret_cast<CIntercessor *>(pV);
    ASSERT(pThis);
    
    try
    {
        pThis->Helper();
    }
    catch(...)
    {
        ::MessageBox(NULL, "Startup a helper thread is failed!", "CIntercessor class", MB_OK);
    }
    return 0;
}

int CIntercessor::Helper()
{
    m_hStartupHelperThreadEvent.Wait();
    
    DWORD dwStart = ::GetTickCount();
    DWORD dwEnd = dwStart;
    DWORD dwLastHealthCheck = dwStart;
    const DWORD HEALTH_CHECK_INTERVAL = 5000; // 5 seconds
    
    while (!m_hQuitEvent.Wait(0))
    {
        dwEnd = ::GetTickCount();
        
        // FIX: Send ping periodically
        bool shouldSendPing = false;
        {
            OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
            
            DWORD elapsed = (dwEnd >= g_pingInfo.dwLastPingSent) ? 
                (dwEnd - g_pingInfo.dwLastPingSent) : 
                (0xFFFFFFFF - g_pingInfo.dwLastPingSent + dwEnd);
            
            if (g_pingInfo.bConnected && (elapsed > PING_INTERVAL || g_pingInfo.dwLastPingSent == 0))
            {
                shouldSendPing = true;
                g_pingInfo.dwLastPingSent = dwEnd;
            }
        }
        
        if (shouldSendPing)
        {
            PING_COMMAND pc;
            pc.ProtocolType = c2s_ping;
            pc.m_dwTime = dwEnd;
            g_theSmartClient.Send(&pc, sizeof(PING_COMMAND));
            printf("[BISHOP] Ping sent at %u\n", dwEnd);
        }
        
        // FIX: Check ping timeout
        if ((dwEnd - dwLastHealthCheck) > HEALTH_CHECK_INTERVAL || dwEnd < dwLastHealthCheck)
        {
            dwLastHealthCheck = dwEnd;
            
            OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
            if (g_pingInfo.bConnected)
            {
                DWORD timeSinceLastPong = (dwEnd >= g_pingInfo.dwLastPongReceived) ?
                    (dwEnd - g_pingInfo.dwLastPongReceived) :
                    (0xFFFFFFFF - g_pingInfo.dwLastPongReceived + dwEnd);
                
                if (timeSinceLastPong > PING_TIMEOUT)
                {
                    g_pingInfo.nMissedPings++;
                    printf("[BISHOP] [WARNING] Ping timeout! Missed pings: %d, Time since last pong: %u ms\n",
                           g_pingInfo.nMissedPings, timeSinceLastPong);
                    
                    if (g_pingInfo.nMissedPings >= MAX_MISSED_PINGS)
                    {
                        printf("[BISHOP] [ERROR] Connection lost! Reconnecting...\n");
                        // TODO: Implement reconnection logic here if needed
                        g_pingInfo.bConnected = false;
                    }
                }
            }
        }
        
        // Get data from account-server
        size_t dataLength = 0;
        const void *pData = g_theSmartClient.Recv(dataLength);
        
        while(pData && dataLength)
        {
            BYTE cProtocol = CPackager::Peek(pData);
            ASSERT(cProtocol > g_nGlobalProtocolType);
            
            // FIX: Handle pong response
            if (s2c_ping == cProtocol)
            {
                OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
                g_pingInfo.dwLastPongReceived = ::GetTickCount();
                g_pingInfo.nMissedPings = 0;
                printf("[BISHOP] Pong received at %u\n", g_pingInfo.dwLastPongReceived);
            }
            else if (s2c_accountlogin == cProtocol)
            {
                const KAccountHead *pAUR = (KAccountHead *)((const char *)pData + 1);
                UINT uID = pAUR->Operate;
                if (uID < m_lnMaxPlayerCount)
                {
                    IPlayer *pPlayer = m_thePlayers[uID];
                    ASSERT(pPlayer);
                    pPlayer->AppendData(CGamePlayer::enumOwnerAccSvr, pData, dataLength);
                }
            }
            else if (s2c_gamelogin == cProtocol)
            {
                const KAccountHead *pAUR = (KAccountHead *)((const char *)pData + 1);
                UINT uID = pAUR->Operate;
                if (uID < m_lnMaxPlayerCount)
                {
                    IPlayer *pPlayer = m_thePlayers[uID];
                    ASSERT(pPlayer);
                    pPlayer->AppendData(CGamePlayer::enumOwnerAccSvr, pData, dataLength);
                }
            }
            
            // Check if need to send ping again
            dwEnd = ::GetTickCount();
            {
                OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
                DWORD elapsed = (dwEnd >= g_pingInfo.dwLastPingSent) ? 
                    (dwEnd - g_pingInfo.dwLastPingSent) : 
                    (0xFFFFFFFF - g_pingInfo.dwLastPingSent + dwEnd);
                
                if (g_pingInfo.bConnected && elapsed > PING_INTERVAL)
                {
                    g_pingInfo.dwLastPingSent = dwEnd;
                    PING_COMMAND pc;
                    pc.ProtocolType = c2s_ping;
                    pc.m_dwTime = dwEnd;
                    g_theSmartClient.Send(&pc, sizeof(PING_COMMAND));
                }
            }
            
            pData = g_theSmartClient.Recv(dataLength);
        }
        
        // Get data from role-database
        dataLength = 0;
        pData = m_pDBRoleClient->GetPackFromServer(dataLength);
        
        while(pData && dataLength)
        {
            BYTE cProtocol = CPackager::Peek(pData);
            UINT uID = (UINT)(-1);
            
            if (cProtocol < g_nGlobalProtocolType)
            {
                uID = *(const unsigned long *)((const char *)pData + 2);
            }
            else // cProtocol > g_nGlobalProtocolType
            {
                TProcessData *pPD = (TProcessData *)(pData);
                uID = pPD->ulIdentity;
            }
            
            if (uID < m_lnMaxPlayerCount)
            {
                IPlayer *pPlayer = m_thePlayers[uID];
                ASSERT(pPlayer);
                pPlayer->AppendData(CGamePlayer::enumOwnerRoleSvr, pData, dataLength);
            }
            
            // Check if need to send ping
            dwEnd = ::GetTickCount();
            {
                OnlineGameLib::Win32::CCriticalSection::Owner lock(g_csPingInfo);
                DWORD elapsed = (dwEnd >= g_pingInfo.dwLastPingSent) ? 
                    (dwEnd - g_pingInfo.dwLastPingSent) : 
                    (0xFFFFFFFF - g_pingInfo.dwLastPingSent + dwEnd);
                
                if (g_pingInfo.bConnected && elapsed > PING_INTERVAL)
                {
                    g_pingInfo.dwLastPingSent = dwEnd;
                    PING_COMMAND pc;
                    pc.ProtocolType = c2s_ping;
                    pc.m_dwTime = dwEnd;
                    g_theSmartClient.Send(&pc, sizeof(PING_COMMAND));
                }
            }

            pData = m_pDBRoleClient->GetPackFromServer(dataLength);
        }

        // CRITICAL: Sleep longer to prevent system call flood
        // Helper thread only needs to check ping every 30s and health every 5s
        // No need to loop 1000 times/second - causes VPS firewall to block IP!
        // Sleep(1ms) = 1000 loops/sec → Sleep(100ms) = 10 loops/sec (reduce 99% load)
        ::Sleep(30);
    }

    return 0;
}

// ============================================
// Analyze Player Require
// ============================================
bool CIntercessor::AnalyzePlayerRequire(size_t index, const void *pData, size_t datalength)
{
    ASSERT(pData && datalength);
    BYTE cProtocol = *(const BYTE *)pData;
    
    switch (cProtocol)
    {
    case c2s_login:
        {
            const BYTE *pBuffer = (const BYTE *)pData + 1;
            KLoginAccountInfo *pLAI = (KLoginAccountInfo *)pBuffer;
            ASSERT(pLAI);
            
            IPlayer *pPlayer = m_thePlayers[index];
            if (pLAI && pPlayer)
            {
                pPlayer->AppendData(CGamePlayer::enumOwnerPlayer, pData, datalength);
                pPlayer->DispatchTask(CGamePlayer::enumLogin);
            }
        }
        break;
        
    case c2s_dbplayerselect:
    case c2s_newplayer:
    case c2s_roleserver_deleteplayer:
        {
            if (index < m_lnMaxPlayerCount)
            {
                IPlayer *pPlayer = m_thePlayers[index];
                ASSERT(pPlayer);
                pPlayer->AppendData(CGamePlayer::enumOwnerPlayer, pData, datalength);
            }
        }
        break;
        
    default:
        break;
    }
    
    return true;
}

// ============================================
// Game Server Info
// ============================================
size_t CIntercessor::GetGameServerCount()
{
    if (m_pGameSvrServer)
    {
        return m_pGameSvrServer->GetClientCount();
    }
    return 0;
}

const char *CIntercessor::GetGameServerInfo(const unsigned long &ulnID)
{
    if (m_pGameSvrServer)
    {
        return m_pGameSvrServer->GetClientInfo(ulnID);
    }
    return NULL;
}