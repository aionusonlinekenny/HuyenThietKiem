#include "Stdafx.h"
#include "GamePlayer.h"

#include "AccountLoginDef.h"
#include "LoginDef.h"
#include "S3DBInterface.h"

#include "KProtocol.h"
#include "KProtocolDef.h"

#include "Utils.h"
#include "Macro.h"
#include "Exception.h"
#include "Buffer.h"

#include "SmartClient.h"
#include "GameServer.h"
#include <map>
#include <set>
#include <stdarg.h>
static OnlineGameLib::Win32::CCriticalSection g_csActiveAccounts;
static std::set<std::string> g_ActiveAccounts;
using OnlineGameLib::Win32::Output;
using OnlineGameLib::Win32::CException;
using OnlineGameLib::Win32::CCriticalSection;
using OnlineGameLib::Win32::ToString;
using OnlineGameLib::Win32::Trace;
using OnlineGameLib::Win32::_tstring;
using OnlineGameLib::Win32::CBuffer;
using OnlineGameLib::Win32::CPackager;
using OnlineGameLib::Win32::net_aton;

CBuffer::Allocator	CGamePlayer::m_theGlobalAllocator( 1024 * 64, 1000 );

LONG				CGamePlayer::m_slnIdentityCounts = 0;
LONG				CGamePlayer::m_lnWorkingCounts = 0;

const int			CGamePlayer::s_nRoleListCount	 = 3;
const int			CGamePlayer::s_nLoginTimeoutTimer	 = 30 * 1000;
const int			CGamePlayer::s_nProcessTimeoutTimer	 = 400 * 1000;

CCriticalSection		CGamePlayer::m_csMapSP;
CGamePlayer::stdMapSP	CGamePlayer::m_sthePlayerTable;

CPlayerCreator						CGamePlayer::m_thePlayerCreator;

//IClient * CGamePlayer::m_pAccSvrClient = NULL;
IServer * CGamePlayer::m_pPlayerServer = NULL;
IClient * CGamePlayer::m_pDBRoleClient = NULL;
static OnlineGameLib::Win32::CCriticalSection g_csLoginLog;

static std::set<std::string> g_TempAccounts;
static OnlineGameLib::Win32::CCriticalSection g_csTempAccounts;

static bool AcquireTempLock(const std::string &acc)
{
    if (acc.empty()) return false;

    OnlineGameLib::Win32::CCriticalSection::Owner lk1(g_csActiveAccounts);
    OnlineGameLib::Win32::CCriticalSection::Owner lk2(g_csTempAccounts);

    // N?u dã ACTIVE và không ph?i dang transfer -> không cho TEMP m?i
    bool alreadyOnline = (g_ActiveAccounts.find(acc) != g_ActiveAccounts.end());
    bool inTransfer    = CGameServer::IsInTransfer(acc.c_str());

    if (alreadyOnline && !inTransfer)
        return false;

    // N?u dã có TEMP pending (login song song) -> t? ch?i
    if (g_TempAccounts.find(acc) != g_TempAccounts.end())
        return false;

    g_TempAccounts.insert(acc);
    return true;
}

static void ReleaseTempLock(const std::string &acc)
{
    if (acc.empty()) return;
    OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csTempAccounts);
    std::set<std::string>::iterator it = g_TempAccounts.find(acc);
    if (it != g_TempAccounts.end()) g_TempAccounts.erase(it);
}

static void LoginLog(const char* fmt, ...)
{
    OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csLoginLog);

    FILE* f = fopen("Bishop_Log\\Bishop_Login.log", "a+");
    if (!f) return;

    SYSTEMTIME st; GetLocalTime(&st);
    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    va_list ap; va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);

    fputc('\n', f);
    fclose(f);
}

static void ReleaseLoginLockSafe(const std::string &acc)
{
    if (acc.empty()) return;
    OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csActiveAccounts);
    g_ActiveAccounts.erase(acc);
    LoginLog("[SafeUnlock] ACTIVE lock REMOVED for \"%s\"", acc.c_str());
}

// SendData wrapper for server?client packets
// CRITICAL: NO rate limiting for outgoing packets!
// Rate limiting should only apply to incoming packets (client?server) for DDoS protection.
// Server?client responses MUST be sent immediately or client will lag/timeout.
static bool SendDataToPlayer(IServer *pServer, UINT playerID, const void *pData, size_t dataLength)
{
    if (!pServer || !pData || dataLength == 0)
        return false;

    // Send packet immediately without throttling
    // CRITICAL FIX: SendData() returns HRESULT (COM interface)
    // S_OK (0x00000000) = success, non-zero = error
    // Previous logic was inverted: checking != 0 treated success as failure!
    HRESULT hr = pServer->SendData(playerID, pData, dataLength);
    return SUCCEEDED(hr);
}

/*
 * CGamePlayer Global Function
 */
bool CGamePlayer::SetupGlobalAllocator( size_t bufferSize, size_t maxFreeBuffers )
{
	return CGamePlayer::m_theGlobalAllocator.ReSet( bufferSize, maxFreeBuffers );
}

/*
 * CGamePlayer::CTask
 */
CGamePlayer::CTask::CTask()
{
	ASSERT( FALSE );
}

CGamePlayer::CTask::~CTask()
{
	//CCriticalSection::Owner	lock( m_csTask );

	stdVector::iterator theIterator;

	for ( theIterator = m_stdCommand.begin(); theIterator != m_stdCommand.end(); theIterator ++ )
	{
		ICommand *pCmd = reinterpret_cast< ICommand * >( *theIterator );

		SAFE_DELETE( pCmd );
	}

	m_stdCommand.clear();
}

CGamePlayer::CTask::CTask( CGamePlayer *pReceiver, UINT nTaskID )
			: m_pReceiver( pReceiver )
			, m_indexCmd( 0 )
			, m_nTaskProgID( nTaskID )
{
}

size_t CGamePlayer::CTask::AddCmd( Action pFun )
{
	//CCriticalSection::Owner	lock( m_csTask );

	/*
	 * Convert a status to the other status
	 */

	/*
	 * Generate a command and push it into the task queue
	 */
	ICommand *pCmd = new CTaskCommand< CGamePlayer >( m_pReceiver, pFun );

	m_stdCommand.push_back( pCmd );
	
	size_t id = m_stdCommand.size();

	return id;
}

UINT CGamePlayer::CTask::Execute()
{
	//CCriticalSection::Owner	lock( m_csTask );

	if ( m_indexCmd < m_stdCommand.size() )
	{
		ICommand *pCmd = m_stdCommand[ m_indexCmd ];

		ASSERT( pCmd );

		UINT nResult = pCmd->Execute();

		switch ( nResult )
		{
		case enumSelAddDelRole:
		case enumLoginCreateRole:
		case enumLoginDeleteRole:
		case enumLoginSelectRole:
		case enumCompleted:
			Reset();

		case enumError:
		case enumNone:

			return nResult;
			break;

		case enumRepeat:

			return m_nTaskProgID;
			break;

		case enumToNextTask:
		default:
			break;
		}

		m_indexCmd ++;
		return m_nTaskProgID;
	}	

	Reset();	
	return enumCompleted;
}

/*
 * CGamePlayer class
 */
CGamePlayer::CDataQueue::CDataQueue( size_t bufferSize /*= 1024 * 64*/, size_t maxFreeBuffers /*= 1*/ )
				: m_theDQAllocator( bufferSize, maxFreeBuffers )
{

}

CGamePlayer::CDataQueue::~CDataQueue()
{
	Empty();
}

void CGamePlayer::CDataQueue::Empty()
{
	CCriticalSection::Owner locker( m_csQueue );
	
	stdDataMap::iterator it;
	for ( it = m_theData.begin(); it != m_theData.end(); it ++ )
	{
		LONG id = ( *it ).first;
		
		CBuffer *pBuffer = (*it).second;

		SAFE_RELEASE( pBuffer );
	}

	m_theData.erase( m_theData.begin(), m_theData.end() );
}

bool CGamePlayer::CDataQueue::AddData( LONG lnID, const BYTE *pData, size_t datalength )
{
	bool ok = false;

	ASSERT( pData && datalength );

	CBuffer *pBuffer = m_theDQAllocator.Allocate();

	ASSERT( pBuffer );

	pBuffer->AddData( pData, datalength );

	{
		CCriticalSection::Owner locker( m_csQueue );

		pBuffer->AddRef();

		stdDataMap::iterator it;
		if ( m_theData.end() != ( it = m_theData.find( lnID ) ) )
		{
			CBuffer *pTemp = ( *it ).second;
			
			SAFE_RELEASE( pTemp );
			
			m_theData.erase( it );
		}

		std::pair< stdDataMap::iterator, bool > result = 
			m_theData.insert( stdDataMap::value_type( lnID, pBuffer ) );

		if ( !( ok = result.second ) )
		{
			SAFE_RELEASE( pBuffer );
		}
	}

	SAFE_RELEASE( pBuffer );

	return ok;
}

CBuffer *CGamePlayer::CDataQueue::Attach( LONG lnID )
{
	CCriticalSection::Owner locker( m_csQueue );

	stdDataMap::iterator it;

	if ( m_theData.end() != ( it = m_theData.find( lnID ) ) )
	{
		CBuffer *pBuffer = ( *it ).second;

		m_theData.erase( it );

		return pBuffer;
	}

	return NULL;
}

void CGamePlayer::CDataQueue::Detach( LONG lnID )
{
//	Clear( lnID );
}

void CGamePlayer::CDataQueue::Clear( LONG lnID )
{
	CCriticalSection::Owner locker( m_csQueue );
	
	stdDataMap::iterator it;
	if ( m_theData.end() != ( it = m_theData.find( lnID ) ) )
	{
		CBuffer *pBuffer = ( *it ).second;

		SAFE_RELEASE( pBuffer );

		m_theData.erase( it );
	}
}

/*
 * CGamePlayer class
 */
CGamePlayer::CGamePlayer( UINT nIdentityID /*  = ( UINT )( -1 ) */ )
				: m_lnIdentityID( nIdentityID )
				, m_theLoginTask( this, enumLogin )
				, m_theSelAddDelTask( this, enumSelAddDelRole )
				, m_theLoginCreateRoleTask( this, enumLoginCreateRole )
				, m_theLoginSelectRoleTask( this, enumLoginSelectRole )
				, m_theLoginDeleteRoleTask( this, enumLoginDeleteRole )
				, m_theSafeCloseTask( this, enumSafeClose )
				, m_nCurrentTaskID( 0 )
				, m_nAttachServerID( -1 )
				, m_bActiveStatus( false )
				, m_dwTaskBeginTimer( 0 )
				, m_dwTaskTotalTimer( 0 )
				, m_bAutoUnlockAccount( false )
                , m_bUseSuperPassword( false )
				, m_bOwnsActiveLock( false ) 
				, m_bHasTempLock( false )
{
	SetCurrentTask( enumNone );

	LONG lnID = ::InterlockedExchangeAdd( &m_slnIdentityCounts, 1 );

	m_lnIdentityID = ( ( UINT )( -1 ) == m_lnIdentityID ) ? lnID : m_lnIdentityID;

	InitTaskProcessor();
}

CGamePlayer::~CGamePlayer()
{
	{
		CCriticalSection::Owner locker( CGamePlayer::m_csMapSP );

		m_sthePlayerTable.erase( m_sthePlayerTable.begin(), m_sthePlayerTable.end() );
	}

	::InterlockedExchangeAdd( &m_slnIdentityCounts, -1 );
}

bool CGamePlayer::Active()
{
	SetCurrentTask( enumNone );

	m_nAttachServerID = -1;

	m_bActiveStatus = true;

	m_dwTaskBeginTimer = ::GetTickCount();
	m_dwTaskTotalTimer = s_nLoginTimeoutTimer;

	::InterlockedExchangeAdd( &m_lnWorkingCounts, 1 );

	return true;
}
bool CGamePlayer::Inactive()
{
    m_bActiveStatus = false;
    SetCurrentTask( enumNone );

    // CRITICAL: Check if player already entered GameServer
    const bool playerOnGameServer = (m_nAttachServerID != -1 && !m_sRoleName.empty());

    IGServer *pGServer = CGameServer::GetServer( m_nAttachServerID );

    if (playerOnGameServer && pGServer)
    {
        // Player on GameServer - notify GS that Bishop connection closed
        // GS will handle logout and send LeaveGame back to Bishop for cleanup
        LoginLog("[Inactive][ID=%ld] Player \"%s\" on GS%d - notifying GameServer",
                 m_lnIdentityID, m_sAccountName.c_str(), m_nAttachServerID);

        if ( !m_sRoleName.empty() )
        {
            DWORD tStart = ::GetTickCount();
            LoginLog("[Inactive][ID=%ld] Sending logic logout to GS for role \"%s\"",
                     m_lnIdentityID, m_sRoleName.c_str());

            pGServer->DispatchTask( CGameServer::enumPlayerLogicLogout,
                                    m_sRoleName.c_str(),
                                    (int)(m_sRoleName.size() + 1) );

            DWORD tDispatch = ::GetTickCount();
            if (tDispatch - tStart > 1000)
            {
                LoginLog("[Inactive][ID=%ld] WARNING: DispatchTask took %dms",
                         m_lnIdentityID, tDispatch - tStart);
            }
        }

        // Detach from GameServer (GS will receive logout message)
        if (!m_sAccountName.empty())
        {
            int nDetachCount = 0;
            DWORD tBeforeDetach = ::GetTickCount();

            // Method 1: Detach from primary GameServer
            CGameServer *pGS = static_cast<CGameServer*>(pGServer);
            if (pGS)
            {
                DWORD tPrimaryStart = ::GetTickCount();
                bool bPrimaryDetached = pGS->DetachAccountFromGameServer(m_sAccountName.c_str());
                DWORD tPrimaryEnd = ::GetTickCount();

                if (bPrimaryDetached)
                {
                    nDetachCount++;
																									   
                    LoginLog("[Inactive][ID=%ld] Detached \"%s\" from GS%d (primary) in %dms",
                             m_lnIdentityID, m_sAccountName.c_str(), m_nAttachServerID,
                             tPrimaryEnd - tPrimaryStart);
                }
                else
                {
                    // Not found in primary - this is suspicious if we were attached
                    LoginLog("[Inactive][ID=%ld] Account \"%s\" NOT FOUND in GS%d (primary) after %dms",
                             m_lnIdentityID, m_sAccountName.c_str(), m_nAttachServerID,
                             tPrimaryEnd - tPrimaryStart);

                    if (tPrimaryEnd - tPrimaryStart > 1000)
                    {
                        LoginLog("[Inactive][ID=%ld] WARNING: Primary detach took %dms but found nothing!",
                                 m_lnIdentityID, tPrimaryEnd - tPrimaryStart);
                    }
                }
            }

            // Method 2: Detach from ALL other GameServers
            // FIX: Use GetAllServers() to get all server pointers with SINGLE lock acquisition
            // This avoids lock contention with FindServerByAccount() which was causing 2-18s delays
            DWORD tLoopStart = ::GetTickCount();

            // Get all GameServer pointers with SINGLE lock acquisition (NO lock contention!)
            CGameServer *allServers[5] = {NULL};
            DWORD tCacheStart = ::GetTickCount();
            CGameServer::GetAllServers(allServers, 5);
            DWORD tCacheEnd = ::GetTickCount();

            if (tCacheEnd - tCacheStart > 500)
            {
                LoginLog("[Inactive][ID=%ld] WARNING: GetAllServers took %dms (unexpected delay!)",
                         m_lnIdentityID, tCacheEnd - tCacheStart);
            }

            // Now detach from each GameServer using cached pointers (no lock contention)
            for (int i = 0; i < 5; i++)
            {
                if (allServers[i] == NULL)
                    continue;
										   

                int gsId = i + 1;
                if (gsId == m_nAttachServerID)
                    continue;  // Skip primary (already handled)
					 

                DWORD tGSStart = ::GetTickCount();
                bool bDetached = allServers[i]->DetachAccountFromGameServer(m_sAccountName.c_str());
                DWORD tGSEnd = ::GetTickCount();
                DWORD duration = tGSEnd - tGSStart;

                if (bDetached)
                {
                    nDetachCount++;
                    LoginLog("[Inactive][ID=%ld] Detached \"%s\" from GS%d (stuck cleanup) in %dms",
                             m_lnIdentityID, m_sAccountName.c_str(), gsId, duration);
                }
                else if (duration > 100)
                {
                    // Took >100ms but found nothing - suspicious!
                    LoginLog("[Inactive][ID=%ld] GS%d detach took %dms but account \"%s\" not found",
                             m_lnIdentityID, gsId, duration, m_sAccountName.c_str());
                }

                // Warn about SLOW individual GameServer calls
                if (duration > 1000)
                {
                    LoginLog("[Inactive][ID=%ld] WARNING: GS%d DetachAccount() BLOCKED for %dms!",
                             m_lnIdentityID, gsId, duration);
					 
                }
            }

            DWORD tLoopEnd = ::GetTickCount();
            DWORD totalLoopTime = tLoopEnd - tLoopStart;

	
			
            if (totalLoopTime > 1000)
            {
                LoginLog("[Inactive][ID=%ld] WARNING: Total loop detach took %dms (cache=%dms, detach=%dms)",
                         m_lnIdentityID, totalLoopTime, tCacheEnd - tCacheStart, tLoopEnd - tCacheEnd);
            }

            LoginLog("[Inactive][ID=%ld] Detach summary: \"%s\" removed from %d GameServer(s) in %dms total",
                     m_lnIdentityID, m_sAccountName.c_str(), nDetachCount,
                     ::GetTickCount() - tBeforeDetach);
        }

        // Cleanup local Bishop state
        Del( m_sRoleName.c_str() );
        m_dwTaskBeginTimer = 0;
        m_dwTaskTotalTimer = s_nLoginTimeoutTimer;
        _ClearTaskQueue();
        m_nAttachServerID  = -1;
        m_sPassword        = "";
        m_sDelRoleName     = "";
        m_bUseSuperPassword= false;
        m_sRoleName        = "";
        m_sLicense         = "";
        ::InterlockedExchangeAdd( &m_lnWorkingCounts, -1 );

        // Release g_ActiveAccounts lock (Bishop connection closed = player disconnected)
        {
            OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csActiveAccounts);
            if (m_bOwnsActiveLock)
            {
                g_ActiveAccounts.erase(m_sAccountName);
                m_bOwnsActiveLock = false;
                LoginLog("[Inactive][ID=%ld] ACTIVE lock REMOVED for \"%s\" (player on GS)",
                         m_lnIdentityID, m_sAccountName.c_str());
            }
   
        }

        // Release TEMP lock if exists
        if (m_bHasTempLock)
        {
            ReleaseTempLock(m_sAccountName);
            m_bHasTempLock = false;
            LoginLog("[Inactive][ID=%ld] TEMP lock REMOVED for \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
        }

        // CRITICAL FIX: Send AccountLogout to AccServer when player disconnects from GameServer
        // Without this, AccServer still thinks account is online and rejects next login with "already online"
        if (!m_sAccountName.empty())
        {
            _UnlockAccount();  // Send c2s_accountlogout to AccServer
            m_bAutoUnlockAccount = false;
            LoginLog("[Inactive][ID=%ld] Sent AccountLogout for \"%s\"",
                     m_lnIdentityID, m_sAccountName.c_str());
        }

        CGameServer::EndTransfer(m_sAccountName.c_str());
        LoginLog("[Inactive][ID=%ld] Completed cleanup for \"%s\" (player was on GS)",
                 m_lnIdentityID, m_sAccountName.c_str());
        return true;
    }

    // Player NOT on GameServer yet (disconnected during login) - do full cleanup
    if ( pGServer )
    {
        if ( !m_sRoleName.empty() )
        {
            LoginLog("[Inactive][ID=%ld] Logic logout for role \"%s\"", m_lnIdentityID, m_sRoleName.c_str());

            pGServer->DispatchTask( CGameServer::enumPlayerLogicLogout,
                                    m_sRoleName.c_str(),
                                    (int)(m_sRoleName.size() + 1) );
        }

        // CRITICAL FIX: Detach account from GameServer on logout
        // Without this, account remains in m_theAccountInThisServer forever
        // causing "account already online" error on next login
        if (!m_sAccountName.empty())
        {
            int nDetachCount = 0;

            // Method 1: Detach from primary GameServer
            CGameServer *pGS = static_cast<CGameServer*>(pGServer);
            if (pGS)
            {
                if (pGS->DetachAccountFromGameServer(m_sAccountName.c_str()))
                {
                    nDetachCount++;
                    LoginLog("[Inactive][ID=%ld] Detached \"%s\" from GS%d (primary)",
                             m_lnIdentityID, m_sAccountName.c_str(), m_nAttachServerID);
			 
                }
            }

            // Method 2: Detach from ALL GameServers using GetAllServers() (single lock)
            // FIX: Use GetAllServers() instead of multiple GetServer() calls to avoid lock contention
            CGameServer *allServers[5] = {NULL};
            CGameServer::GetAllServers(allServers, 5);

            for (int i = 0; i < 5; i++)
            {
                if (allServers[i] == NULL)
                    continue;

                int gsId = i + 1;
                if (gsId == m_nAttachServerID)
                    continue;  // Skip primary (already handled above)

                if (allServers[i]->DetachAccountFromGameServer(m_sAccountName.c_str()))
								 
                {
																					
																								  
					 
                    nDetachCount++;
                    LoginLog("[Inactive][ID=%ld] Detached \"%s\" from GS%d (stuck account cleanup)",
                             m_lnIdentityID, m_sAccountName.c_str(), gsId);
					 
                }
            }

            LoginLog("[Inactive][ID=%ld] Total detached \"%s\" from %d GameServer(s) (not on GS path)",
                     m_lnIdentityID, m_sAccountName.c_str(), nDetachCount);
        }
    }
    if (!m_sAccountName.empty())
    {
        _UnlockAccount();                    // g?i c2s_accountlogout
       m_bAutoUnlockAccount = false;        // kh?ng ?? Run/timeout g?i tr?ng
        LoginLog("[Inactive][ID=%ld] Sent AccountLogout for \"%s\"",
                 m_lnIdentityID, m_sAccountName.c_str());
    }

    Del( m_sRoleName.c_str() );

    m_dwTaskBeginTimer = 0;
    m_dwTaskTotalTimer = s_nLoginTimeoutTimer;

    _ClearTaskQueue();

    m_nAttachServerID  = -1;

    // Clear info nh?
    m_sPassword        = "";
    m_sDelRoleName     = "";
    m_bUseSuperPassword= false;
    m_sRoleName        = "";
    m_sLicense         = "";

    ::InterlockedExchangeAdd( &m_lnWorkingCounts, -1 );

    // ACTIVE-LOCK: ch? owner m?i du?c xo?
    {
        OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csActiveAccounts);
        if (m_bOwnsActiveLock)
        {
            g_ActiveAccounts.erase(m_sAccountName);
            m_bOwnsActiveLock = false;
            LoginLog("[Inactive][ID=%ld] ACTIVE lock REMOVED for \"%s\" (owner)", m_lnIdentityID, m_sAccountName.c_str());
        }
        else
        {
            LoginLog("[Inactive][ID=%ld] Skip ACTIVE unlock for \"%s\" (not owner)", m_lnIdentityID, m_sAccountName.c_str());
        }
    }

    // TEMP-LOCK: n?u c?n (client t?t gi?a handshake)
    if (m_bHasTempLock)
    {
        ReleaseTempLock(m_sAccountName);
        m_bHasTempLock = false;
        LoginLog("[Inactive][ID=%ld] TEMP lock REMOVED for \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
    }

    // NOTE: Do NOT call ReleaseLoginLockSafe() here!
    // g_ActiveAccounts was already properly cleaned up above (line 482) if m_bOwnsActiveLock
    // Calling it again would remove account owned by another session!

	CGameServer::EndTransfer(m_sAccountName.c_str());
    LoginLog("[Inactive][ID=%ld] Completed cleanup for \"%s\"", m_lnIdentityID, m_sAccountName.c_str());

    return true;
}
UINT CGamePlayer::SafeClose()
{
	ASSERT( FALSE );
	
	return enumToNextTask;
}

bool CGamePlayer::_UnlockAccount()
{
    const char *pAccountName = m_sAccountName.c_str();

    if ( !pAccountName || !pAccountName[0] )
        return false;

    CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
    BYTE *pData = const_cast< BYTE * >( pBuffer->GetBuffer() );

    const size_t datalength = sizeof( KAccountUserLogout ) + 1;

    KAccountUserLogout user;
    user.Size      = sizeof( KAccountUserLogout );
    user.Type      = AccountUserLogout;
    user.Version   = ACCOUNT_CURRENT_VERSION;
    user.nExtPoint = 0;
    user.nExtPoint2= 0;

    size_t length = strlen( pAccountName );
    length = ( length > LOGIN_USER_ACCOUNT_MAX_LEN ) ? LOGIN_USER_ACCOUNT_MAX_LEN : length;
    memcpy( user.Account, pAccountName, length );
    user.Account[length] = '\0';

    *pData = c2s_accountlogout;
    memcpy( pData + 1, &user, sizeof( KAccountUserLogout ) );

    g_theSmartClient.Send( ( const void * )pData, datalength );

    SAFE_RELEASE( pBuffer );
    return true;
}

void CGamePlayer::ATTACH_NETWORK( IClient *pAccSvrClient, 
				IServer *pPlayerServer, 
				IClient	*pDBRoleClient )
{
	ASSERT( m_slnIdentityCounts == 0 );

//	m_pAccSvrClient = pAccSvrClient;
	m_pPlayerServer = pPlayerServer;
	m_pDBRoleClient = pDBRoleClient;
}

void CGamePlayer::DETACH_NETWORK()
{
	ASSERT( m_slnIdentityCounts == 0 );

//	SAFE_RELEASE( m_pAccSvrClient );
	SAFE_RELEASE( m_pPlayerServer );
	SAFE_RELEASE( m_pDBRoleClient );
}

bool CGamePlayer::DispatchTask( UINT nTaskID )
{
	/*
	 * This player is processing a special tasks
	 */	
/*
	if ( IsWorking() )
	{
		return false;
	}
*/
	m_theLoginTask.Reset();

	m_theSelAddDelTask.Reset();

	m_theLoginCreateRoleTask.Reset();
	m_theLoginSelectRoleTask.Reset();
	m_theLoginDeleteRoleTask.Reset();

	SetCurrentTask( nTaskID );

	return true;	
}

bool CGamePlayer::IsWorking()
{
	return ( GetCurrentTask() != enumNone );
}
bool CGamePlayer::Run()
{
    LONG lnNextTask = enumNone;

    if ( m_bActiveStatus && m_dwTaskBeginTimer )
    {
        DWORD dwCurTimer = ::GetTickCount();
        if ( dwCurTimer - m_dwTaskBeginTimer > m_dwTaskTotalTimer )
        {
            LoginLog("[Run-Timeout][ID=%ld] Task timeout for acc=\"%s\"", m_lnIdentityID, m_sAccountName.c_str());

            m_pPlayerServer->ShutdownClient( m_lnIdentityID );

            // FIX: Only send logout ONCE - check flag before calling _UnlockAccount()
            if ( !m_sAccountName.empty() && m_bAutoUnlockAccount )
            {
                _UnlockAccount();
                m_bAutoUnlockAccount = false;
				LoginLog("[Run-Timeout][ID=%ld] Sent AccountLogout for \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            }
			// FIX: Reset timer to prevent spam - this timeout block should only run ONCE
            m_dwTaskBeginTimer = 0;
            // ACTIVE-LOCK:
            {
                OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csActiveAccounts);
                if (m_bOwnsActiveLock)
                {
                    g_ActiveAccounts.erase(m_sAccountName);
                    m_bOwnsActiveLock = false;
                    LoginLog("[Run-Timeout][ID=%ld] ACTIVE lock REMOVED for \"%s\" (owner)", m_lnIdentityID, m_sAccountName.c_str());
                }
                else
                {
                    LoginLog("[Run-Timeout][ID=%ld] Skip ACTIVE unlock for \"%s\" (not owner)", m_lnIdentityID, m_sAccountName.c_str());
                }
            }

            // TEMP-LOCK: n?u còn thì x?
            if (m_bHasTempLock)
            {
                ReleaseTempLock(m_sAccountName);
                m_bHasTempLock = false;
                LoginLog("[Run-Timeout][ID=%ld] TEMP lock REMOVED for \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            }

            // Safe-lock v?i AccServer
            ReleaseLoginLockSafe(m_sAccountName);
			CGameServer::EndTransfer(m_sAccountName.c_str());
            return false;
        }
    }

    switch ( GetCurrentTask() )
    {
    case enumNone: return true;

    case enumLogin:
        lnNextTask = m_theLoginTask.Execute();           SetCurrentTask( lnNextTask ); break;
    case enumSelAddDelRole:
        lnNextTask = m_theSelAddDelTask.Execute();       SetCurrentTask( lnNextTask ); break;
    case enumLoginCreateRole:
        lnNextTask = m_theLoginCreateRoleTask.Execute(); SetCurrentTask( lnNextTask ); break;
    case enumLoginSelectRole:
        lnNextTask = m_theLoginSelectRoleTask.Execute(); SetCurrentTask( lnNextTask ); break;
    case enumLoginDeleteRole:
        lnNextTask = m_theLoginDeleteRoleTask.Execute(); SetCurrentTask( lnNextTask ); break;
    case enumSafeClose:
        lnNextTask = m_theSafeCloseTask.Execute();       SetCurrentTask( lnNextTask ); break;
    case enumCompleted:
        lnNextTask = TaskCompleted() ? enumNone : enumCompleted; SetCurrentTask( lnNextTask ); break;
    case enumError:
        SetCurrentTask( enumNone ); break;
    default: break;
    }
    return true;
}


bool CGamePlayer::AppendData( UINT nOwner, const void *pData, size_t dataLength )
{
	if ( nOwner >= enumOwnerTotal || !m_bActiveStatus )
	{
		return false;
	}

	BYTE cProtocol = CPackager::Peek( pData );
	
	if ( cProtocol < g_nGlobalProtocolType )
	{
		return LargePackProcess( nOwner, pData, dataLength );
	}
	else if ( cProtocol > g_nGlobalProtocolType )
	{
		return SmallPackProcess( nOwner, pData, dataLength );
	}

	return true;
}

bool CGamePlayer::SmallPackProcess( UINT nOwner, const void *pData, size_t dataLength )
{
	switch ( nOwner )
	{
	case enumOwnerAccSvr:

		return DispatchTaskForAccount( pData, dataLength );
		break;
		
	case enumOwnerRoleSvr:
		
		return DispatchTaskForDBRole( pData, dataLength );
		break;
		
	case enumOwnerPlayer:
		
		return DispatchTaskForPlayer( pData, dataLength );
		break;

	default:
		break;
	}

	return false;
}

bool CGamePlayer::LargePackProcess( UINT nOwner, const void *pData, size_t dataLength )
{
	switch ( nOwner )
	{
	case enumOwnerAccSvr:

		ASSERT( FALSE );
		
		break;

	case enumOwnerRoleSvr:
		{	
			bool ok = true;
			CBuffer *pBuffer = m_thePackager.PackUp( pData, dataLength );
			
			if ( pBuffer )
			{
				ok = DispatchTaskForDBRole( pBuffer->GetBuffer(), pBuffer->GetUsed() );
				
				SAFE_RELEASE( pBuffer );
			}
			
			return ok;
		}
		break;

	case enumOwnerPlayer:
		
		ASSERT( FALSE );
		break;

	default:
		break;
	}

	return false;
}

bool CGamePlayer::DispatchTaskForAccount( const void *pData, size_t dataLength )
{
	if ( NULL == pData || 0 == dataLength )
	{
		return false;
	}

	BYTE cProtocol = CPackager::Peek( pData );

	m_theDataQueue[enumOwnerAccSvr].AddData( cProtocol, ( const BYTE * )pData, dataLength );

	return true;
}

bool CGamePlayer::DispatchTaskForDBRole( const void *pData, size_t dataLength )
{
	if ( NULL == pData || 0 == dataLength )
	{
		return false;
	}

	BYTE cProtocol = CPackager::Peek( pData );

	m_theDataQueue[enumOwnerRoleSvr].AddData( cProtocol, ( const BYTE * )pData, dataLength );

	return true;
}

bool CGamePlayer::DispatchTaskForPlayer( const void *pData, size_t dataLength )
{
	if ( NULL == pData || 0 == dataLength )
	{
		return false;
	}

	BYTE cProtocol = CPackager::Peek( pData );

	m_theDataQueue[enumOwnerPlayer].AddData( cProtocol, ( const BYTE * )pData, dataLength );

	return true;
}

void CGamePlayer::_ClearTaskQueue()
{
	for ( int i=0; i<enumOwnerTotal; i++ )
	{
		m_theDataQueue[i].Empty();
	}
	
	m_thePackager.Empty();
}

void CGamePlayer::InitTaskProcessor()
{
	/*
	 * Login main task
	 */
	m_theLoginTask.AddCmd( &CGamePlayer::WaitForAccPwd );
	m_theLoginTask.AddCmd( &CGamePlayer::QueryAccPwd );
	m_theLoginTask.AddCmd( &CGamePlayer::VerifyAccount );
	m_theLoginTask.AddCmd( &CGamePlayer::QueryRoleList );
	m_theLoginTask.AddCmd( &CGamePlayer::ProcessRoleList );

	/*
	 * Login branch task
	 */
	{
		/*
		 * m_theSelAddDelTask::SelAddDelRole
		 *
		 * switch( result )
		 * case m_theLoginCreateRoleTask
		 * case m_theLoginDeleteRoleTask
		 * case m_theLoginSelectRoleTask
		 */
		m_theSelAddDelTask.AddCmd( &CGamePlayer::SelAddDelRole );
		m_theSelAddDelTask.AddCmd( &CGamePlayer::QueryAccPwd );
		m_theSelAddDelTask.AddCmd( &CGamePlayer::DelRole_WaitForVerify );

		/*
		 * m_theLoginCreateRoleTask::WaitForCreateResult
		 *
		 * successed : m_theLoginCreateRoleTask::ProcessRoleInfo
		 * failed	 : m_theSelAddDelTask::SelAddDelRole
		 */
		m_theLoginCreateRoleTask.AddCmd( &CGamePlayer::WaitForCreateResult );

		m_theLoginCreateRoleTask.AddCmd( &CGamePlayer::ProcessRoleInfo );
		m_theLoginCreateRoleTask.AddCmd( &CGamePlayer::WaitForGameSvrPermit );

		m_theLoginDeleteRoleTask.AddCmd( &CGamePlayer::WaitForDeleteResult );

		m_theLoginSelectRoleTask.AddCmd( &CGamePlayer::ProcessRoleInfo );
		m_theLoginSelectRoleTask.AddCmd( &CGamePlayer::WaitForGameSvrPermit );
	}

	m_theSafeCloseTask.AddCmd( &CGamePlayer::SafeClose );

	/*
	 * Logout
	 */
}

bool CGamePlayer::TaskCompleted()
{
	Trace( ToString( m_lnIdentityID ), "CGamePlayer::TaskCompleted" );

	/*
	 * Clear some data
	 */
	
	return true;
}
UINT CGamePlayer::WaitForAccPwd()
{
#ifdef  CONSOLE_DEBUG
    cprintf("CGamePlayer::WaitForAccPwd...\n");
#endif

    CBuffer* pRetBuffer = m_theDataQueue[enumOwnerPlayer].Attach(c2s_login);
    if (!pRetBuffer)
        return enumRepeat;

    // B? byte protocol ??u
    KLoginAccountInfo* pLAI = (KLoginAccountInfo*)(pRetBuffer->GetBuffer() + 1);

    UINT nQueryResult = LOGIN_A_LOGIN;
    UINT nNextTask    = enumError;

    if (!pLAI)
    {
        // Không có LOGIN_R_PACKET_ERROR -> dùng LOGIN_R_TIMEOUT
        LoginLog("[WaitForAccPwd][ID=%ld] Invalid login packet (pLAI=null)", m_lnIdentityID);
        _VerifyAccount_ToPlayer(nQueryResult | LOGIN_R_TIMEOUT, 0);

        if (m_pPlayerServer)
        {
            m_pPlayerServer->ShutdownClient(m_lnIdentityID);
            LoginLog("[WaitForAccPwd][ID=%ld] Client shutdown - invalid packet", m_lnIdentityID);
        }

        SAFE_RELEASE(pRetBuffer);
        m_theDataQueue[enumOwnerPlayer].Detach(c2s_login);
        return enumError;
    }

    char accBuf[64] = {0};
    char licBuf[64] = {0};

    // ---- L?Y ACCOUNT: pLAI->Account ----
    {
        const char* pAcc = (const char*)pLAI->Account;
        if (pAcc)
        {
            int n = (int)strlen(pAcc);
            if (n > (int)sizeof(accBuf) - 1) n = (int)sizeof(accBuf) - 1;
            memcpy(accBuf, pAcc, n);
            accBuf[n] = '\0';
        }
    }

    // ---- L?Y LICENSE: pLAI->License ----
    {
        const char* pLic = (const char*)pLAI->License;
        if (pLic)
        {
            int n = (int)strlen(pLic);
            if (n > (int)sizeof(licBuf) - 1) n = (int)sizeof(licBuf) - 1;
            memcpy(licBuf, pLic, n);
            licBuf[n] = '\0';
        }
    }

    // ---- L?Y PASSWORD: KSG_PASSWORD -> szPassword ----
    // L?u vào m_sPassword (d? án c?a b?n ?ang dùng string này ? b??c VerifyAccount ?? build payload)
    m_sPassword = (const char*)pLAI->Password.szPassword;

    LoginLog("[WaitForAccPwd][ID=%ld] Received login: account=\"%s\"", m_lnIdentityID, accBuf);

#ifdef  CONSOLE_DEBUG
    cprintf("CGamePlayer::WaitForAccPwd [Account:%s]\n", accBuf);
#endif

    // Ki?m tra version (n?u b?t macro)
    int nCheckProtocolVersion = 1;
#ifdef USE_KPROTOCOL_VERSION
    nCheckProtocolVersion = (pLAI->ProtocolVersion == KPROTOCOL_VERSION);
#endif

    if (nCheckProtocolVersion)
    {
        if (accBuf[0] != '\0')
        {
            // L?U STATE TR??C ?? n?u client r?t gi?a ch?ng, Inactive() còn d?n ???c
            m_sAccountName = accBuf;
            m_sLicense     = licBuf;
            m_bUseSuperPassword = false;

            // Acquire TEMP-LOCK theo account
            std::string _acc(accBuf);
            if (!AcquireTempLock(_acc))
            {
                // TEMP-LOCK failed - có th? là leaked lock t? session cu
                // Th? cleanup và retry m?t l?n
                LoginLog("[WaitForAccPwd][ID=%ld] TEMP-LOCK FAILED for \"%s\" - attempting FULL recovery",
                         m_lnIdentityID, accBuf);

                // Force cleanup potential leaked TEMP lock
                ReleaseTempLock(_acc);
ReleaseLoginLockSafe(_acc);  // Force cleanup ACTIVE lock

                // Also detach from all GameServers (account might be stuck there)
                for (int gsId = 1; gsId <= 5; gsId++)
                {
                    IGServer *pGS = CGameServer::GetServer(gsId);
                    if (pGS)
                    {
                        CGameServer *pGameServer = static_cast<CGameServer*>(pGS);
                        if (pGameServer && pGameServer->DetachAccountFromGameServer(_acc.c_str()))
                        {
                            LoginLog("[WaitForAccPwd][ID=%ld] Recovery: Detached \"%s\" from GS%d",
                                     m_lnIdentityID, accBuf, gsId);
                        }
                    }
                }

                // Clear transfer flag if exists
                CGameServer::EndTransfer(_acc.c_str());
                // Retry acquire TEMP lock
                if (AcquireTempLock(_acc))
                {
                    // Recovery thành công - leak dã du?c cleanup!
                    m_bHasTempLock = true;
                    LoginLog("[WaitForAccPwd][ID=%ld] TEMP-LOCK recovered for \"%s\" - continuing login",
                             m_lnIdentityID, accBuf);
                    // Ti?p t?c login flow bình thu?ng (không return enumError)
                }
                else
                {
                    // V?n fail sau recovery ? th?c s? duplicate (account dang ACTIVE ? session khác)
                    LoginLog("[WaitForAccPwd][ID=%ld] TEMP-LOCK FAILED for \"%s\" -> duplicate/pending (after recovery)",
                             m_lnIdentityID, accBuf);

                    _VerifyAccount_ToPlayer(LOGIN_A_LOGIN | LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR, 0);
										 
																  
								 
			 

                    if (m_pPlayerServer)
                    {
                        m_pPlayerServer->ShutdownClient(m_lnIdentityID);
                        LoginLog("[WaitForAccPwd][ID=%ld] Client shutdown - duplicate login attempt", m_lnIdentityID);
                    }

                    // KHÔNG xóa m_sAccountName d? Inactive() có th? cleanup n?u c?n
                    SAFE_RELEASE(pRetBuffer);
                    m_theDataQueue[enumOwnerPlayer].Detach(c2s_login);
                    return enumError;
                }
            }
            else
            {
                // TEMP-LOCK thành công ngay l?n d?u
                m_bHasTempLock = true;
                LoginLog("[WaitForAccPwd][ID=%ld] TEMP-LOCK OK for \"%s\"", m_lnIdentityID, accBuf);
            }

            // Sang b??c k? ti?p: g?i AccServer ?? VerifyAccount
            nNextTask = enumToNextTask;

            LoginLog("[WaitForAccPwd][ID=%ld] Ready to QueryAccPwd for \"%s\"",
                     m_lnIdentityID, m_sAccountName.c_str());
        }
        else
        {
            // Không có LOGIN_R_ACCOUNT_ERROR -> dùng LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR
            nQueryResult |= LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR;
            _VerifyAccount_ToPlayer(nQueryResult, 0);
            LoginLog("[WaitForAccPwd][ID=%ld] Empty account name", m_lnIdentityID);
        }
    }
    else
    {
        if (accBuf[0] != '\0')
        {
            m_sAccountName = accBuf;
            m_sLicense     = licBuf;
            m_bUseSuperPassword = false;
        }

        nQueryResult |= LOGIN_R_INVALID_PROTOCOLVERSION;
        _VerifyAccount_ToPlayer(nQueryResult, 0);
        LoginLog("[WaitForAccPwd][ID=%ld] Invalid protocol for \"%s\"",
                 m_lnIdentityID, m_sAccountName.c_str());
    }

    // Shutdown client immediately if login validation failed
    if (nNextTask == enumError && m_pPlayerServer)
    {
        m_pPlayerServer->ShutdownClient(m_lnIdentityID);
        LoginLog("[WaitForAccPwd][ID=%ld] Client shutdown due to login validation error",
                 m_lnIdentityID);
    }

    SAFE_RELEASE(pRetBuffer);
    m_theDataQueue[enumOwnerPlayer].Detach(c2s_login);
    return nNextTask;
}

UINT CGamePlayer::QueryAccPwd()
{
#ifdef  CONSOLE_DEBUG
    cprintf( "CGamePlayer::QueryAccPwd...\n" );
#endif

    m_dwTaskTotalTimer = s_nProcessTimeoutTimer;

    CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
    BYTE *pData      = const_cast< BYTE * >( pBuffer->GetBuffer() );
    const size_t datalength = sizeof( KAccountUserLoginInfo ) + 1;

    KAccountUserLoginInfo userlogin;
    memset(&userlogin, 0, sizeof(userlogin));

    userlogin.Size    = sizeof( KAccountUserLoginInfo );
    userlogin.Type    = m_bUseSuperPassword ? AccountUserVerify : AccountUserLoginInfo;
    userlogin.Version = ACCOUNT_CURRENT_VERSION;
    userlogin.Operate = m_lnIdentityID; // flow chu?n d? án

    // copy an toàn (VC6)
    int nMinLen;

    nMinLen = sizeof( userlogin.Account ) - 1;
    nMinLen = (int)m_sAccountName.size() > nMinLen ? nMinLen : (int)m_sAccountName.size();
    if (nMinLen > 0) memcpy( userlogin.Account, m_sAccountName.c_str(), nMinLen );
    userlogin.Account[nMinLen] = '\0';

    if (m_bUseSuperPassword)
    {
        nMinLen = sizeof( userlogin.Password ) - 1;
        nMinLen = (int)m_sSuperPassword.size() > nMinLen ? nMinLen : (int)m_sSuperPassword.size();
        if (nMinLen > 0) memcpy( userlogin.Password, m_sSuperPassword.c_str(), nMinLen );
        userlogin.Password[nMinLen] = '\0';
    }
    else
    {
        nMinLen = sizeof( userlogin.Password ) - 1;
        nMinLen = (int)m_sPassword.size() > nMinLen ? nMinLen : (int)m_sPassword.size();
        if (nMinLen > 0) memcpy( userlogin.Password, m_sPassword.c_str(), nMinLen );
        userlogin.Password[nMinLen] = '\0';
    }

    nMinLen = sizeof( userlogin.License ) - 1;
    nMinLen = (int)m_sLicense.size() > nMinLen ? nMinLen : (int)m_sLicense.size();
    if (nMinLen > 0) memcpy( userlogin.License, m_sLicense.c_str(), nMinLen );
    userlogin.License[nMinLen] = '\0';

    // Log gói g?i (không log password)
    LoginLog("[QueryAccPwd][ID=%ld] Send to AccServer: Type=%d Ver=%d Operate=%lu Acc=\"%s\" passLen=%d licLen=%d",
             m_lnIdentityID, (int)userlogin.Type, (int)userlogin.Version, (unsigned long)userlogin.Operate,
             userlogin.Account, (int)strlen(userlogin.Password), (int)strlen(userlogin.License));

    *pData = c2s_accountlogin;
    memcpy( pData + 1, &userlogin, sizeof( KAccountUserLoginInfo ) );

    m_bAutoUnlockAccount = true;
    m_theDataQueue[enumOwnerAccSvr].Empty();

    g_theSmartClient.Send( ( const void * )pData, datalength );

    SAFE_RELEASE( pBuffer );

    return enumToNextTask;
}
UINT CGamePlayer::VerifyAccount()
{
#ifdef  CONSOLE_DEBUG
    cprintf( "CGamePlayer::VerifyAccount...\n" );
#endif

    CBuffer *pRetBuffer = m_theDataQueue[enumOwnerAccSvr].Attach( s2c_accountlogin );

    if ( pRetBuffer )
    {
        KAccountUserReturnExt* pReturn = ( KAccountUserReturnExt * )( pRetBuffer->GetBuffer() + 1 );

        int  nResult      = pReturn ? pReturn->nReturn : 0;
        UINT nQueryResult = LOGIN_A_LOGIN;
        UINT nNextTask    = enumError;

        LoginLog("[VerifyAccount][ID=%ld] Acc=\"%s\" result=%d",
                 m_lnIdentityID, m_sAccountName.c_str(), nResult);

        switch ( nResult )
        {
        case ACTION_SUCCESS:
			{
				OnlineGameLib::Win32::CCriticalSection::Owner lk(g_csActiveAccounts);

				const bool inTransfer    = CGameServer::IsInTransfer(m_sAccountName.c_str());
				const bool inActiveSet   = (g_ActiveAccounts.find(m_sAccountName) != g_ActiveAccounts.end());

				// CRITICAL: Also check if account is playing on any GameServer
				int nExistingGS = CGameServer::FindServerByAccount(m_sAccountName.c_str());
				const bool onGameServer  = (nExistingGS != -1);
				const bool alreadyOnline = inActiveSet || onGameServer;

				LoginLog("[VerifyAccount][ID=%ld] ACTION_SUCCESS for \"%s\" (inActiveSet=%d onGameServer=%d[GS=%d] inTransfer=%d)",
						m_lnIdentityID, m_sAccountName.c_str(), inActiveSet ? 1 : 0, onGameServer ? 1 : 0, nExistingGS, inTransfer ? 1 : 0);

				if (alreadyOnline && !inTransfer)
				{
					nQueryResult |= LOGIN_R_ACCOUNT_EXIST;
					_VerifyAccount_ToPlayer(nQueryResult, 0);

					// Shutdown client immediately - account already online
					if (m_pPlayerServer)
					{
						m_pPlayerServer->ShutdownClient(m_lnIdentityID);
						LoginLog("[VerifyAccount][ID=%ld] Client shutdown - account already online \"%s\"",
								 m_lnIdentityID, m_sAccountName.c_str());
					}

					if (m_bHasTempLock)
					{
						ReleaseTempLock(m_sAccountName);
						m_bHasTempLock = false;
					}

					SAFE_RELEASE(pRetBuffer);
					m_theDataQueue[enumOwnerAccSvr].Detach(s2c_accountlogin);
					SetCurrentTask(enumError);
					return enumError;
				}

				if (m_bHasTempLock)
				{
					ReleaseTempLock(m_sAccountName);
					m_bHasTempLock = false;
				}

				g_ActiveAccounts.insert(m_sAccountName);
				m_bOwnsActiveLock = true;

				LoginLog("[VerifyAccount][ID=%ld] \"%s\" LOCKED ACTIVE (owner=1)",
						m_lnIdentityID, m_sAccountName.c_str());

				nQueryResult |= LOGIN_R_SUCCESS;
				nNextTask     = enumToNextTask;
			}
			break;

        case E_ACCOUNT_OR_PASSWORD: // 3
            nQueryResult |= LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR;
            LoginLog("[VerifyAccount][ID=%ld] Failed: name/password error for \"%s\"",
                     m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_ACCOUNT_EXIST: // 4
            nQueryResult |= LOGIN_R_ACCOUNT_EXIST;
            LoginLog("[VerifyAccount][ID=%ld] Failed: account already exists/online \"%s\"",
                     m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_ACCOUNT_NODEPOSIT: // 5
            nQueryResult |= LOGIN_R_TIMEOUT;
            LoginLog("[VerifyAccount][ID=%ld] Failed: nodeposit \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_ACCOUNT_ACCESSDENIED: // 6
            nQueryResult |= LOGIN_R_LOCKED;
            LoginLog("[VerifyAccount][ID=%ld] Failed: access denied \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_ADDRESS_OR_PORT: // 7
            nQueryResult |= LOGIN_R_TIMEOUT;
            LoginLog("[VerifyAccount][ID=%ld] Failed: address/port \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_ACCOUNT_FREEZE: // 8
            nQueryResult |= LOGIN_R_FREEZE;
            LoginLog("[VerifyAccount][ID=%ld] Failed: account freeze \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_CONNECT_LOCKED: // 9
            nQueryResult |= LOGIN_R_LOCKED;
            LoginLog("[VerifyAccount][ID=%ld] Failed: connect locked \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        case E_CONNECT_BANED: // 0xA
            nQueryResult |= LOGIN_R_BANED;
            LoginLog("[VerifyAccount][ID=%ld] Failed: banned \"%s\"", m_lnIdentityID, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;

        default:
            nQueryResult |= LOGIN_R_TIMEOUT;
            LoginLog("[VerifyAccount][ID=%ld] Failed: unknown(%d) \"%s\"",
                     m_lnIdentityID, nResult, m_sAccountName.c_str());
            m_bAutoUnlockAccount = false;
            ReleaseLoginLockSafe(m_sAccountName);
            if (m_bHasTempLock) { ReleaseTempLock(m_sAccountName); m_bHasTempLock = false; }
            break;
        }

        _VerifyAccount_ToPlayer( nQueryResult, pReturn ? pReturn->nLeftTime : 0 );

        // Shutdown client immediately if login failed (prevent showing character selection screen)
        // BUT do not shutdown if account is in transfer state (server switching)
        if (nNextTask == enumError && m_pPlayerServer)
        {
            bool inTransfer = CGameServer::IsInTransfer(m_sAccountName.c_str());
            if (!inTransfer)
            {
                m_pPlayerServer->ShutdownClient(m_lnIdentityID);
                LoginLog("[VerifyAccount][ID=%ld] Client shutdown due to login error for \"%s\"",
                         m_lnIdentityID, m_sAccountName.c_str());
            }
            else
            {
                LoginLog("[VerifyAccount][ID=%ld] Skip shutdown - account in transfer for \"%s\"",
                         m_lnIdentityID, m_sAccountName.c_str());
            }
        }

        SAFE_RELEASE( pRetBuffer );
        m_theDataQueue[enumOwnerAccSvr].Detach( s2c_accountlogin );

        return nNextTask;
    }

    return enumRepeat;
}


bool CGamePlayer::_VerifyAccount_ToPlayer( UINT nQueryResult, unsigned long nLeftTime)
{
	KLoginAccountInfo lai;

	ZeroMemory( &lai, sizeof( KLoginAccountInfo ) );

	/*
	 * Account
	 */
	size_t used = sizeof( lai.Account );
	used = ( used > m_sAccountName.length() ) ? m_sAccountName.length() : ( used - 1 );

	memcpy( lai.Account, m_sAccountName.c_str(), used );
	lai.Account[used] = '\0';

	/*
	 * Password
	 */
	used = sizeof( lai.Password.szPassword );//ganlei
	used = ( used > m_sPassword.length() ) ? m_sPassword.length() : ( used - 1 );

	memcpy( lai.Password.szPassword, m_sPassword.c_str(), used );//ganlei
	lai.Password.szPassword[used] = '\0';//ganlei
	
	
	used = sizeof( lai.License );
	used = ( used > m_sLicense.length() ) ? m_sLicense.length() : ( used - 1 );

	memcpy( lai.License, m_sLicense.c_str(), used );
	lai.License[used] = '\0';
	

	lai.nLeftTime = nLeftTime;

	/*
	 * The other info
	 */
	lai.Param = nQueryResult;

	lai.Size = sizeof( lai );	

	CBuffer *pBuffer = m_theGlobalAllocator.Allocate();

	BYTE *pData = const_cast< BYTE * >( pBuffer->GetBuffer() );

	BYTE bProtocol = s2c_login;
	size_t sizeProtocol = sizeof( BYTE );
	memcpy( pData, &bProtocol, sizeProtocol );
	memcpy( pData + sizeProtocol, ( BYTE * )( &lai ), sizeof( lai ) );

	bool sent = SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, pData, ( sizeof( KLoginAccountInfo ) + sizeof( BYTE )/* protocol id */ ) );

	LoginLog("[VerifyAccount_ToPlayer][ID=%ld] Login response sent to client (result=0x%X, sent=%d)",
	         m_lnIdentityID, nQueryResult, sent ? 1 : 0);

	pBuffer->Release();

	return true;
}

UINT CGamePlayer::QueryRoleList()
{
#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::QueryRoleList...\n" );
#endif

	LoginLog("[QueryRoleList][ID=%ld] Requesting character list for \"%s\"",
	         m_lnIdentityID, m_sAccountName.c_str());

	const size_t lenAN = strlen( m_sAccountName.c_str() );
	const size_t lenPL = lenAN + sizeof( TProcessData );

	CBuffer *pBuffer = m_theGlobalAllocator.Allocate();

	TProcessData *pPlayerList = reinterpret_cast< TProcessData * >( const_cast< BYTE * >( pBuffer->GetBuffer() ) );

	ASSERT( pPlayerList );

	pPlayerList->nProtoId = c2s_roleserver_getrolelist;
	pPlayerList->nDataLen = lenAN + 1;
	pPlayerList->ulIdentity = m_lnIdentityID;

	pPlayerList->pDataBuffer[0] = s_nRoleListCount;

	memcpy( &pPlayerList->pDataBuffer[1], m_sAccountName.c_str(), lenAN );
	pPlayerList->pDataBuffer[lenAN + 1] = '\0';

	m_theDataQueue[enumOwnerRoleSvr].Empty();

	m_pDBRoleClient->SendPackToServer( ( const void * )pPlayerList, lenPL );

	LoginLog("[QueryRoleList][ID=%ld] Request sent to RoleServer", m_lnIdentityID);

	SAFE_RELEASE( pBuffer );

	return enumToNextTask;
}

UINT CGamePlayer::ProcessRoleList()
{
#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::ProcessRoleList...\n" );
#endif

	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerRoleSvr].Attach( s2c_roleserver_getrolelist_result );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		const TProcessData *pPD = ( const TProcessData * )pRetBuffer->GetBuffer();

		int nRoleCount = CPackager::Peek( ( const char * )pPD->pDataBuffer );

		LoginLog("[ProcessRoleList][ID=%ld] Received character list: %d characters for \"%s\"",
		         m_lnIdentityID, nRoleCount, m_sAccountName.c_str());

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::ProcessRoleList find %d role in list\n", nRoleCount );
#endif

		if ( nRoleCount >= 0 )
		{
			m_theDataQueue[enumOwnerPlayer].Empty();

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, pRetBuffer->GetBuffer(), pRetBuffer->GetUsed() );

			LoginLog("[ProcessRoleList][ID=%ld] Character list sent to client", m_lnIdentityID);

			nNextTask = enumSelAddDelRole;
		}
		else
		{
			LoginLog("[ProcessRoleList][ID=%ld] ERROR: Invalid character count %d",
			         m_lnIdentityID, nRoleCount);
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerRoleSvr].Detach( s2c_roleserver_getrolelist_result );

		return nNextTask;
	}

	// Still waiting for RoleServer response
	return enumRepeat;
}

UINT CGamePlayer::SelAddDelRole()
{
	CBuffer *pRetBuffer = NULL;

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::SelAddDelRole...\n" );
#endif

	/*
	 * Select a role
	 */
	pRetBuffer = m_theDataQueue[enumOwnerPlayer].Attach( c2s_dbplayerselect );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		const tagDBSelPlayer *pDSPC = ( const tagDBSelPlayer * )pRetBuffer->GetBuffer();

		LoginLog("[SelAddDelRole][ID=%ld] Client selected character: \"%s\"",
		         m_lnIdentityID, pDSPC->szRoleName);

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::SelAddDelRole Select a role from list\n" );
#endif

		if( _QueryRoleInfo_ToDBRole( pDSPC->szRoleName, true ) )
		{
			LoginLog("[SelAddDelRole][ID=%ld] Requesting role data for \"%s\" from RoleServer",
			         m_lnIdentityID, pDSPC->szRoleName);
			nNextTask = enumLoginSelectRole;
		}
		else
		{
			LoginLog("[SelAddDelRole][ID=%ld] ERROR: Failed to query role info for \"%s\"",
			         m_lnIdentityID, pDSPC->szRoleName);
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerPlayer].Detach( c2s_dbplayerselect );

		return nNextTask;
	}

	/*
	 * Create a role
	 */
	pRetBuffer = m_theDataQueue[enumOwnerPlayer].Attach( c2s_newplayer );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		const TProcessData *pCRPD = ( const TProcessData * )pRetBuffer->GetBuffer();

		const NEW_PLAYER_COMMAND *pNPC = ( const NEW_PLAYER_COMMAND * )( pCRPD->pDataBuffer );

		ASSERT( pNPC );
		
		char szRoleName[NAME_LEN];
		size_t namelen = pRetBuffer->GetUsed() - sizeof( TProcessData );
		
		namelen = ( namelen < 31 ) ? namelen : 31;

		memcpy( szRoleName, pNPC->m_szName, namelen );
		szRoleName[namelen] = '\0';

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::SelAddDelRole Create a role [Name : %s]\n", szRoleName );
#endif
		m_theDataQueue[enumOwnerRoleSvr].Empty();

		if ( _CreateNewPlayer_ToDBRole( ( const char * )szRoleName, 
					pNPC->m_btRoleNo, 
					pNPC->m_btSeries,
					pNPC->m_NativePlaceId ) )
		{
			nNextTask = enumLoginCreateRole;
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerPlayer].Detach( c2s_newplayer );

		return nNextTask;
	}

	/*
	 * Delete a role
	 */
	pRetBuffer = m_theDataQueue[enumOwnerPlayer].Attach( c2s_roleserver_deleteplayer );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::SelAddDelRole Del a role in list\n" );
#endif
		m_theDataQueue[enumOwnerRoleSvr].Empty();

		if ( _DeleteRole_ToDBRole( pRetBuffer->GetBuffer(), pRetBuffer->GetUsed() ) )
		{
            nNextTask = enumToNextTask;
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerPlayer].Detach( c2s_roleserver_deleteplayer );

		if ( enumToNextTask != nNextTask )
		{
			tagNewDelRoleResponse ndrr;
			ndrr.cProtocol = s2c_rolenewdelresponse;
			ndrr.szRoleName[0] = '\0';
			ndrr.bSucceeded = false;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
		}

		return nNextTask;
	}

	return enumRepeat;
}

UINT CGamePlayer::DelRole_WaitForVerify()
{
#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::DelRole_WaitForVerify...\n" );
#endif
	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerAccSvr].Attach( s2c_accountlogin );
    
    if (pRetBuffer)
    {
		UINT nNextTask = enumError;

		KAccountUserReturnExt* pReturn = ( KAccountUserReturnExt * )( pRetBuffer->GetBuffer() + 1/* size of a protocol byte */ );

		int nResult = pReturn->nReturn;

		if (nResult == ACTION_SUCCESS)
        {	
			const char *pAccountName = m_sAccountName.c_str();		
            const char *pRoleName = m_sDelRoleName.c_str();
			
			const size_t lenAN = strlen( pAccountName );	
			const size_t lenRN = 16;		//
	        const size_t lenRI = sizeof( TProcessData ) - 1 + lenAN + lenRN;
	        
	        CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
	        
	        TProcessData *pRoleInfo = reinterpret_cast< TProcessData * >( const_cast< BYTE * >( pBuffer->GetBuffer() ) );
	        
	        ASSERT( pRoleInfo );
	        
	        pRoleInfo->nProtoId = c2s_roleserver_deleteplayer;
	        pRoleInfo->nDataLen = lenRI;
	        pRoleInfo->ulIdentity = m_lnIdentityID;
	        
			//
	        memcpy( &pRoleInfo->pDataBuffer[0], pAccountName, lenAN );
	        pRoleInfo->pDataBuffer[lenAN] = '\0';
			
			memcpy( &pRoleInfo->pDataBuffer[lenAN], pRoleName, lenRN );
			pRoleInfo->pDataBuffer[lenAN+lenRN] = '\0';
			
			
	        
	        m_pDBRoleClient->SendPackToServer( ( const void * )pRoleInfo, lenRI );
	        
	        SAFE_RELEASE( pBuffer );
        
            nNextTask = enumLoginDeleteRole;
        }
        else
        {
            tagNewDelRoleResponse ndrr;
            ndrr.cProtocol = s2c_rolenewdelresponse;
            ndrr.szRoleName[0] = '\0';
            ndrr.bSucceeded = false;

            SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

            nNextTask = enumSelAddDelRole;  // ±íÊ¾Õâ´ÎµÄ¶¯×÷Íê³É£¬½øÈëÏÂÒ»¸öµÈ´ýÁ÷³Ì
        }

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerAccSvr].Detach( s2c_accountlogin );

        return nNextTask;
    }
	return enumRepeat;


}


UINT CGamePlayer::WaitForCreateResult()
{
#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::WaitForCreateResult...\n" );
#endif

	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerRoleSvr].Attach( s2c_roleserver_createrole_result );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		TProcessData *pPD = ( TProcessData * )pRetBuffer->GetBuffer();

		ASSERT( pPD );

		char cResult = pPD->pDataBuffer[0];

		tagNewDelRoleResponse ndrr;
		ndrr.cProtocol = s2c_rolenewdelresponse;
		ndrr.szRoleName[0] = '\0';
		ndrr.bSucceeded = false;

#ifdef	CONSOLE_DEBUG
		cprintf( "CGamePlayer::WaitForCreateResult %s\n", ( 1 == cResult ) ? "Successful" : "Failed" );
#endif

		switch ( cResult )
		{
		case 1:	// Successed

			ndrr.bSucceeded = true;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumToNextTask;
			break;

		//case -1:	// Failed
		//case -2:	// Have exist
		default:

			ndrr.bSucceeded = false;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );
			
			nNextTask = enumSelAddDelRole;
			break;
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerRoleSvr].Detach( s2c_roleserver_createrole_result );

		return nNextTask;
	}

	return enumRepeat;
}

UINT CGamePlayer::WaitForDeleteResult()
{
#ifdef	CONSOLE_DEBUG
		cprintf( "CGamePlayer::WaitForDeleteResult\n" );
#endif

	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerRoleSvr].Attach( s2c_roleserver_deleterole_result );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		TProcessData *pPD = ( TProcessData * )pRetBuffer->GetBuffer();

		ASSERT( pPD );

		char cResult = pPD->pDataBuffer[0];

		tagNewDelRoleResponse ndrr;
		ndrr.cProtocol = s2c_rolenewdelresponse;
		ndrr.szRoleName[0] = '\0';
		ndrr.bSucceeded = false;	

#ifdef	CONSOLE_DEBUG
		cprintf( "CGamePlayer::WaitForDeleteResult %s\n", ( 1 == cResult ) ? "Successful" : "Failed" );
#endif

		switch ( cResult )
		{
		case 1:	// Successed

			ndrr.bSucceeded = 1;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
			break;
		
		case -1:
			ndrr.bSucceeded = -1;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
			break;
		case -2:
			ndrr.bSucceeded = -2;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
			break;
		case -3:
			ndrr.bSucceeded = -3;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
			break;
		default:

			ndrr.bSucceeded = -4;

			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&ndrr, sizeof( tagNewDelRoleResponse ) );

			nNextTask = enumSelAddDelRole;
			break;
		
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerRoleSvr].Detach( s2c_roleserver_deleterole_result );

		return nNextTask;
	}

	return enumRepeat;
}

bool CGamePlayer::_QueryRoleInfo_ToDBRole( const char *pRoleName, bool bCheck )
{
	if( (NULL == pRoleName) || ('\0' == pRoleName[0]) )
	{
		return false;
	}
	
	const size_t lenRN = strlen( pRoleName );
	const size_t lenRI = sizeof( TProcessData ) + lenRN;

	CBuffer *pBuffer = m_theGlobalAllocator.Allocate();
			
	TProcessData *pRoleInfo = reinterpret_cast< TProcessData * >( const_cast< BYTE * >( pBuffer->GetBuffer() ) );
	
	ASSERT( pRoleInfo );
	
	pRoleInfo->nProtoId = c2s_roleserver_getroleinfo;
	pRoleInfo->nDataLen = lenRN;
	pRoleInfo->ulIdentity = m_lnIdentityID;

	memcpy( &pRoleInfo->pDataBuffer[0], pRoleName, lenRN );
	//pRoleInfo->pDataBuffer[lenRN] = '\0';

	if(bCheck == true)
		pRoleInfo->pDataBuffer[lenRN] = 1;
	else
		pRoleInfo->pDataBuffer[lenRN] = 0;

	pRoleInfo->pDataBuffer[lenRN + 1] = '\0';
	
	m_pDBRoleClient->SendPackToServer( ( const void * )pRoleInfo, lenRI );
	
	SAFE_RELEASE( pBuffer );

	return true;
}

bool CGamePlayer::_CreateNewPlayer_ToDBRole( const char *pRoleName, 
						int nRoleSex /* male or female */, 
						int nRoleClass,
						unsigned short nMapID )
{
	if ( NULL == pRoleName || '\0' == pRoleName[0] )
	{
		return false;
	}

	size_t datalength = 0;

	CPlayerCreator::ROLEPARAM	RP;

	int nMinLen = strlen( pRoleName );
	nMinLen = nMinLen > sizeof( RP.szName ) ? sizeof( RP.szName ) : nMinLen;
	memcpy( RP.szName, pRoleName, nMinLen );
	RP.szName[nMinLen] = '\0';

	nMinLen = m_sAccountName.size();
	nMinLen = nMinLen > sizeof( RP.szAccName ) ? sizeof( RP.szAccName ) : nMinLen;
	memcpy( RP.szAccName, m_sAccountName.c_str(), nMinLen );
	RP.szAccName[nMinLen] = '\0';

	RP.nSex = nRoleSex;
	RP.nSeries = nRoleClass;
	RP.nMapID = nMapID;

	const TRoleData *pRoleData = m_thePlayerCreator.GetRoleData( datalength, &RP );

	if ( pRoleData && datalength )
	{
		CBuffer *pData = m_theGlobalAllocator.Allocate();
		
		TProcessData *pPD = reinterpret_cast< TProcessData * >( const_cast< BYTE * >( pData->GetBuffer() ) );

		pPD->nProtoId = c2s_roleserver_createroleinfo;
		pPD->ulIdentity = m_lnIdentityID;

		pPD->pDataBuffer[0] = 1; // Create successed
		pPD->nDataLen = datalength + 1 /* sizeof( pPD->pDataBuffer[0] ) */;
		memcpy( &pPD->pDataBuffer[1], pRoleData, datalength );

		/*
		 * Send a role-info to role dbserver
		 */
		size_t nRoleInfoLength = datalength + sizeof( TProcessData );

		m_thePackager.AddData( c2s_roleserver_createroleinfo,
			( const BYTE * )pPD,
			nRoleInfoLength );

		/*
		 * For create a role
		 */
		m_theDataQueue[enumOwnerRoleSvr].AddData( s2c_roleserver_getroleinfo_result, 
			( const BYTE * )pPD,
			nRoleInfoLength );

		SAFE_RELEASE( pData );

		CBuffer *pPack = m_thePackager.GetHeadPack( c2s_roleserver_createroleinfo );

		while ( pPack )
		{
			m_pDBRoleClient->SendPackToServer( pPack->GetBuffer(), pPack->GetUsed() );

			SAFE_RELEASE( pPack );

			pPack = m_thePackager.GetNextPack( c2s_roleserver_createroleinfo );
		}

		SAFE_RELEASE( pPack );
		
		m_thePackager.DelData( c2s_roleserver_createroleinfo );

		return true;
	}

	return false;
}

UINT CGamePlayer::_DeleteRole_ToDBRole( const void *pData, size_t dataLength )
{
	tagDBDelPlayer *pDBDP = ( tagDBDelPlayer * )pData;

	if ( NULL == pData || 0 == dataLength )
	{
		return false;
	}

	const char *pAccountName = pDBDP->szAccountName;
	const char *pPassword = pDBDP->Password.szPassword;
	const char *pRoleName = pDBDP->szRoleName;

	if ( !pAccountName || !pPassword || !pRoleName )
	{
		return false;
	}

    
	if (m_sAccountName.compare( pAccountName ) != 0)
    {
        return false;
    }

    m_sSuperPassword = pPassword ? pPassword : "";
    m_sDelRoleName   = pRoleName ? pRoleName : ""; 
    m_bUseSuperPassword = true;
    
	return true;
}


//

UINT CGamePlayer::QueryAccExt()
{
#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::QueryAccExt...\n" );
#endif

	m_dwTaskTotalTimer = s_nProcessTimeoutTimer;

	CBuffer *pBuffer = m_theGlobalAllocator.Allocate();

	BYTE *pData = const_cast< BYTE * >( pBuffer->GetBuffer() );

	const size_t datalength = sizeof( KAccountUser ) + 1;

	KAccountUser user;
	user.Size = sizeof( KAccountUser );
	user.Type = AccountUserLoginInfo;
	user.Version = ACCOUNT_CURRENT_VERSION;
	user.Operate = m_lnIdentityID;

	int nMinLen = sizeof( user.Account );
	nMinLen = m_sAccountName.size() > nMinLen ? nMinLen: m_sAccountName.size();
	memcpy( user.Account, m_sAccountName.c_str(), nMinLen );
	user.Account[nMinLen] = '\0';

	*pData = c2s_gamelogin;
	memcpy( pData + 1, &user, sizeof( KAccountUser ) );

	m_theDataQueue[enumOwnerAccSvr].Empty();

	g_theSmartClient.Send( ( const void * )pData, datalength );

	SAFE_RELEASE( pBuffer );

	return enumToNextTask;
}


//

UINT CGamePlayer::VerifyAccExt()
{
	#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::VerifyAccExt...\n" );
#endif

	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerAccSvr].Attach( s2c_gamelogin );

	if ( pRetBuffer )
	{
		KAccountUserReturnExt* pReturn = ( KAccountUserReturnExt * )( pRetBuffer->GetBuffer() + 1/* size of a protocol byte */ );

		int nResult = pReturn->nReturn;

		UINT nQueryResult = LOGIN_A_LOGIN;
		UINT nNextTask = enumError;

		switch ( nResult )
		{
		case ACTION_SUCCESS:

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::VerifyAccount Successful!\n" );
#endif
			nQueryResult |= LOGIN_R_SUCCESS;

			nNextTask = enumToNextTask;

			break;

		case E_ACCOUNT_NODEPOSIT:

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::VerifyAccount Failed! [No Money]\n" );
#endif

			nQueryResult |= LOGIN_R_TIMEOUT;

			break;
		case E_ACCOUNT_ACCESSDENIED:
			nQueryResult |= LOGIN_R_DENIED;
			break;

		default:

#ifdef	CONSOLE_DEBUG
	cprintf( "CGamePlayer::VerifyAccount Failed!\n" );
#endif

			nQueryResult |= LOGIN_R_FAILED;

			break;

		}
		
		/*
		 * Notify the result to player
		 */
		_VerifyAccount_ToPlayer( nQueryResult, pReturn->nLeftTime );

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerAccSvr].Detach( s2c_gamelogin );

		return nNextTask;
	}

	return enumRepeat;
}



UINT CGamePlayer::ProcessRoleInfo()
{
#ifdef	CONSOLE_DEBUG
		cprintf( "CGamePlayer::ProcessRoleInfo...\n" );
#endif

	CBuffer *pRetBuffer = m_theDataQueue[enumOwnerRoleSvr].Attach( s2c_roleserver_getroleinfo_result );

	if ( pRetBuffer )
	{
		UINT nNextTask = enumError;

		TProcessData *pPA = ( TProcessData * )pRetBuffer->GetBuffer();

		ASSERT( m_lnIdentityID == pPA->ulIdentity );

		char cResult = pPA->pDataBuffer[0];

		LoginLog("[ProcessRoleInfo][ID=%ld] Received role data from RoleServer (result=%d, dataLen=%d)",
		         m_lnIdentityID, (int)cResult, (int)pPA->nDataLen);

#ifdef	CONSOLE_DEBUG
		cprintf( "CGamePlayer::ProcessRoleInfo Send info to gameserver\n" );
#endif
		m_nAttachServerID = -1;

		if ( 1 == cResult && _SyncRoleInfo_ToGameServer( ( const void * )( &pPA->pDataBuffer[1] ), pPA->nDataLen-1 ) )
		{
			LoginLog("[ProcessRoleInfo][ID=%ld] Successfully synced role to GameServer",
			         m_lnIdentityID);
			nNextTask = enumToNextTask;
		}
		else
		{
			if (cResult != 1)
			{
				LoginLog("[ProcessRoleInfo][ID=%ld] ERROR: Invalid result from RoleServer (result=%d)",
				         m_lnIdentityID, (int)cResult);
			}
			else
			{
				LoginLog("[ProcessRoleInfo][ID=%ld] ERROR: Failed to sync role to GameServer",
				         m_lnIdentityID);
			}

			tagNotifyPlayerLogin npl;

			memset( &npl, 0, sizeof( tagNotifyPlayerLogin ) );

			npl.cProtocol = s2c_notifyplayerlogin;
			CGameServer::EndTransfer(m_sAccountName.c_str());
			SendDataToPlayer( m_pPlayerServer, m_lnIdentityID, ( const void * )&npl, sizeof( tagNotifyPlayerLogin ) );
		}

		SAFE_RELEASE( pRetBuffer );
		m_theDataQueue[enumOwnerRoleSvr].Detach( s2c_roleserver_getroleinfo_result );

		return nNextTask;
	}

	// Still waiting for RoleServer response
	return enumRepeat;
}
bool CGamePlayer::_SyncRoleInfo_ToGameServer(const void *pData, size_t dataLength)
{
    bool ok = false;
    bool beganTransfer = false;

    ASSERT(pData);
    const TRoleData *pRoleData = (const TRoleData *)(pData);
    const int oldServerId = m_nAttachServerID;
    const std::string oldRoleName = m_sRoleName;

    LoginLog("[_SyncRoleInfo][ID=%ld] Starting sync for \"%s\" (UseRevive=%d, revivalID=%d, enterID=%d)",
             m_lnIdentityID, pRoleData->BaseInfo.caccname,
             pRoleData->BaseInfo.cUseRevive, pRoleData->BaseInfo.irevivalid, pRoleData->BaseInfo.ientergameid);

    IGServer *pGServer = NULL;
    if (pRoleData->BaseInfo.cUseRevive)
        pGServer = CGameServer::QueryServer(pRoleData->BaseInfo.irevivalid);
    else
        pGServer = CGameServer::QueryServer(pRoleData->BaseInfo.ientergameid);

    if (!pGServer)
    {
        LoginLog("[_SyncRoleInfo][ID=%ld] ERROR: GameServer not found (UseRevive=%d, ID=%d)",
                 m_lnIdentityID, pRoleData->BaseInfo.cUseRevive,
                 pRoleData->BaseInfo.cUseRevive ? pRoleData->BaseInfo.irevivalid : pRoleData->BaseInfo.ientergameid);
        return false;
    }

    LoginLog("[_SyncRoleInfo][ID=%ld] Target GameServer found: GS%d",
             m_lnIdentityID, (int)pGServer->GetID());

    if (strcmp(m_sAccountName.c_str(), pRoleData->BaseInfo.caccname) != 0)
    {
        LoginLog("[_SyncRoleInfo][ID=%ld] ERROR: Account mismatch (expected=\"%s\", got=\"%s\")",
                 m_lnIdentityID, m_sAccountName.c_str(), pRoleData->BaseInfo.caccname);
        return false;
    }

    // --- N?u dang ? GS khác và sang GS m?i: dánh d?u transfer & b?o GS cu logic-logout ---
    if (oldServerId != -1 && oldServerId != (int)pGServer->GetID())
    {
		beganTransfer = true;
        // Ðánh d?u transfer d? _NotifyLeaveGame không FreezeMoney
        CGameServer::BeginTransfer(m_sAccountName.c_str());

        IGServer *pOld = CGameServer::GetServer(oldServerId);
        if (pOld)
        {
            const char *szRole = oldRoleName.empty()
                               ? (const char *)pRoleData->BaseInfo.szName
                               : oldRoleName.c_str();

            if (szRole && szRole[0])
            {
                // B?o GS cu ?logic logout? (g? entity), KHÔNG ph?i logout PaySys
                pOld->DispatchTask(CGameServer::enumPlayerLogicLogout,
                                   szRole, (int)strlen(szRole) + 1);

                // CRITICAL FIX: Detach account from old GS to prevent account stuck in server list
                // Without this, FindServerByAccount() will still find account on old server,
                // causing transfer back to fail with "account already online" error
                // IGServer* is always CGameServer* in this context, use static_cast (no RTTI needed)
                CGameServer *pOldGS = static_cast<CGameServer*>(pOld);
                if (pOldGS)
                {
                    pOldGS->DetachAccountFromGameServer(m_sAccountName.c_str());
                    LoginLog("[Transfer][ID=%ld] Detached \"%s\" from old GS%d",
                             m_lnIdentityID, m_sAccountName.c_str(), oldServerId);
                }
            }
        }
    }

    // --- Ch?n login trùng trong cùng GS ---
    {
        const char *szAcc = pRoleData->BaseInfo.caccname;
        int nExistingGS = CGameServer::FindServerByAccount(szAcc);

        LoginLog("[_SyncRoleInfo][ID=%ld] Checking for duplicates: account \"%s\" currently on GS%d (target GS%d)",
                 m_lnIdentityID, szAcc, nExistingGS, (int)pGServer->GetID());

        if (nExistingGS != -1 && nExistingGS == (int)pGServer->GetID())
        {
            // Duplicate session trên cùng GS ? ch?n
            LoginLog("[_SyncRoleInfo][ID=%ld] ERROR: Duplicate session detected - \"%s\" already on GS%d",
                     m_lnIdentityID, szAcc, nExistingGS);
			if (beganTransfer)
                CGameServer::EndTransfer(m_sAccountName.c_str());
            return false;
        }
        // nExistingGS == -1  ho?c dang ? GS khác (dang transfer) ? cho phép
    }

    // Ki?m tra attach h?p l? (bCheck = true ch? ki?m tra, không ghi danh)
    LoginLog("[_SyncRoleInfo][ID=%ld] Checking if can attach \"%s\" to GS%d...",
             m_lnIdentityID, pRoleData->BaseInfo.caccname, (int)pGServer->GetID());

    if (pGServer->Attach(pRoleData->BaseInfo.caccname, true) == false)
	{
        LoginLog("[_SyncRoleInfo][ID=%ld] ERROR: Attach check FAILED - \"%s\" cannot attach to GS%d (already attached?)",
                 m_lnIdentityID, pRoleData->BaseInfo.caccname, (int)pGServer->GetID());
        if (beganTransfer)
            CGameServer::EndTransfer(m_sAccountName.c_str());
        return false;
 	}

    LoginLog("[_SyncRoleInfo][ID=%ld] Attach check PASSED - ready to sync role data",
             m_lnIdentityID);
    ASSERT(pRoleData->BaseInfo.szName[0] != '\0');

    // C?p nh?t danh b? role -> player
    CGamePlayer::Del((const char *)pRoleData->BaseInfo.szName);
    CGamePlayer::Add((const char *)pRoleData->BaseInfo.szName, (IPlayer *)this);

    // C?p nh?t GS hi?n t?i
    m_nAttachServerID = (int)pGServer->GetID();
    m_theDataQueue[enumOwnerPlayer].Empty();

    // G?i role data sang GS m?i
    LoginLog("[_SyncRoleInfo][ID=%ld] Sending role data to GS%d (dataLen=%d)...",
             m_lnIdentityID, (int)pGServer->GetID(), (int)dataLength);

    ok = pGServer->DispatchTask(CGameServer::enumSyncRoleInfo, pData, dataLength);

    if (!ok)
    {
        LoginLog("[_SyncRoleInfo][ID=%ld] ERROR: DispatchTask FAILED - GS%d did not accept role data",
                 m_lnIdentityID, (int)pGServer->GetID());
        if (beganTransfer)
            CGameServer::EndTransfer(m_sAccountName.c_str());
        return false;
    }

    LoginLog("[_SyncRoleInfo][ID=%ld] DispatchTask SUCCESS - GS%d received role data",
             m_lnIdentityID, (int)pGServer->GetID());

    // GS m?i dã nh?n role ? b? c? transfer
    // CRITICAL FIX: Actually attach account to new GS
    // Earlier Attach(..., true) only checked, didn't insert
    // Now we need to actually attach with bCheck=false
    bool attached = pGServer->Attach(pRoleData->BaseInfo.caccname, false);
    if (!attached)
    {
        LoginLog("[SyncRoleInfo][ID=%ld] WARNING: Failed to attach \"%s\" to GS%d after sync",
                 m_lnIdentityID, pRoleData->BaseInfo.caccname, (int)pGServer->GetID());
    }
    else
    {
        LoginLog("[SyncRoleInfo][ID=%ld] Successfully attached \"%s\" to GS%d",
                 m_lnIdentityID, pRoleData->BaseInfo.caccname, (int)pGServer->GetID());
    }

    CGameServer::EndTransfer(m_sAccountName.c_str());
    return ok;
}

UINT CGamePlayer::WaitForGameSvrPermit()
{
    CBuffer *pRetBuffer = m_theDataQueue[enumOwnerPlayer].Attach( s2c_notifyplayerlogin );
    if ( pRetBuffer && m_pPlayerServer )
    {
        UINT nNextTask = enumError;
        tagNotifyPlayerLogin *pNPL = ( tagNotifyPlayerLogin * )pRetBuffer->GetBuffer();

        LoginLog("[WaitForGameSvrPermit][ID=%ld] Received GameServer response (bPermit=%d, IP=%s, Port=%d)",
                 m_lnIdentityID, pNPL->bPermit ? 1 : 0,
                 inet_ntoa(*(struct in_addr*)&pNPL->nIPAddr), pNPL->nPort);

        if ( pNPL->bPermit )
        {
            LoginLog("[WaitForGameSvrPermit][ID=%ld] GameServer ACCEPTED login for \"%s\"",
                     m_lnIdentityID, m_sAccountName.c_str());

            // GS cho phép -> chuy?n b??c ti?p theo
            m_bAutoUnlockAccount = false;
            m_dwTaskBeginTimer   = 0;
            m_dwTaskTotalTimer   = s_nProcessTimeoutTimer;
            nNextTask            = enumToNextTask;
        }
        else
        {
            // GS t? ch?i (server full/deny) -> PH?I tháo lock ngay
            LoginLog("[WaitForGameSvrPermit][ID=%ld] GameServer DENIED login for \"%s\" (server full/deny)",
                     m_lnIdentityID, m_sAccountName.c_str());
			CGameServer::EndTransfer(m_sAccountName.c_str());
            // G?i tr? gói notify cho client nh? c? (?ang send bên d??i)
            // ??ng th?i confirm k?t qu? login cho client (tu? client có ??c notify hay code login)
            _VerifyAccount_ToPlayer(LOGIN_A_LOGIN | LOGIN_R_DENIED, 0);

            // Tháo ACTIVE lock local n?u mình ?ang gi?
            if (m_bOwnsActiveLock)
            {
                ReleaseLoginLockSafe(m_sAccountName);
                m_bOwnsActiveLock = false;
                LoginLog("[PermitLogin][ID=%ld] ACTIVE lock REMOVED for \"%s\" after deny",
                         m_lnIdentityID, m_sAccountName.c_str());
            }

            // Temp-lock th??ng ?ã release ? ACTION_SUCCESS; nh?ng n?u còn thì release n?t
            if (m_bHasTempLock)
            {
                ReleaseTempLock(m_sAccountName);
                m_bHasTempLock = false;
                LoginLog("[PermitLogin][ID=%ld] TEMP lock REMOVED for \"%s\" after deny",
                         m_lnIdentityID, m_sAccountName.c_str());
            }

            // Báo AccServer logout ?? g? lock phía AccServer ngay
            if (!m_sAccountName.empty())
            {
                _UnlockAccount();
                m_bAutoUnlockAccount = false;
                LoginLog("[PermitLogin][ID=%ld] Sent AccountLogout for \"%s\" after deny",
                         m_lnIdentityID, m_sAccountName.c_str());
            }

            // K?t thúc flow login l?n này
            nNextTask = enumError;
        }

        // V?N forward gói notify cho client (gi? nguyên hành vi c?)
        SendDataToPlayer( m_pPlayerServer, m_lnIdentityID,
                         pRetBuffer->GetBuffer(),
                         pRetBuffer->GetUsed() );

        SAFE_RELEASE( pRetBuffer );
        m_theDataQueue[enumOwnerPlayer].Detach( s2c_notifyplayerlogin );

        return nNextTask;
    }
    return enumRepeat;
}


bool CGamePlayer::Attach( const char *pRoleName )
{
	if ( pRoleName && pRoleName[0] )
	{
		m_sRoleName = pRoleName;

		return true;
	}

	return false;
}

bool CGamePlayer::Add( const char *pRoleName, IPlayer *pPlayer )
{
	if ( NULL == pRoleName || NULL == pPlayer || !pRoleName[0] )
	{
		ASSERT( FALSE );

		return false;
	}

	if ( pPlayer )
	{
		CCriticalSection::Owner locker( CGamePlayer::m_csMapSP );

		std::pair< stdMapSP::iterator, bool > result = 
			m_sthePlayerTable.insert( stdMapSP::value_type( pRoleName, pPlayer ) );

		if ( result.second )
		{
			return pPlayer->Attach( pRoleName );
		}
	}

	return false;
}

bool CGamePlayer::Del( const char *pRoleName )
{
	if ( !pRoleName || !pRoleName[0] )
	{
		return false;
	}

	{
		CCriticalSection::Owner locker( CGamePlayer::m_csMapSP );
		
		stdMapSP::iterator it;
		
		if ( m_sthePlayerTable.end() != ( it = m_sthePlayerTable.find( pRoleName ) ) )
		{
			IPlayer *pPlayer = ( IPlayer * )( ( *it ).second );
			
			ASSERT( pPlayer );
			
			m_sthePlayerTable.erase( it );
			
			return true;
		}
	}

	return false;
}

IPlayer *CGamePlayer::Get( const char *pRoleName )
{
	if ( !pRoleName )
	{
		return NULL;
	}

	CCriticalSection::Owner locker( CGamePlayer::m_csMapSP );

	stdMapSP::iterator it;

	if ( m_sthePlayerTable.end() != ( it = m_sthePlayerTable.find( pRoleName ) ) )
	{
		return ( IPlayer * )( ( *it ).second );
	}

	return NULL;
}