// Goddess.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include <objbase.h>
#include <initguid.h>
#include <winsock2.h>
#include <process.h>

#include "kprotocoldef.h"

#include "IDBRoleServer.h"

#include "console.h"
#include "Event.h"
#include "macro.h"
#include "Inifile.h"
#include "Utils.h"
#include "tstring.h"
#include "Library.h"
#include "Buffer.h"
#include "CriticalSection.h"

#include "IServer.h"
#include "HeavenInterface.h"

#include "ClientNode.h"

#include <iostream>
#include <strstream>
#include <string>
#include <list>

#include "FilterTextLib.h"
#include "RoleNameFilter.h"

using namespace std;

using OnlineGameLib::Win32::Console::clrscr;
using OnlineGameLib::Win32::Console::setcolor;
using OnlineGameLib::Win32::CEvent;
using OnlineGameLib::Win32::CIniFile;
using OnlineGameLib::Win32::GetAppFullPath;
using OnlineGameLib::Win32::_tstring;
using OnlineGameLib::Win32::CLibrary;
using OnlineGameLib::Win32::ToString;
using OnlineGameLib::Win32::CPackager;
using OnlineGameLib::Win32::CBuffer;
using OnlineGameLib::Win32::CCriticalSection;

/*
 * User data
 */

static CLibrary g_theHeavenLibrary( "heaven.dll" );

static CEvent g_hQuitEvent( NULL, true, false );
static CEvent g_hStartWorkEvent( NULL, true, false );

static size_t g_nMaxRoleCount = 3;

static unsigned short g_nServerPort = 5001;
static IServer *g_pServer = NULL;

static HANDLE g_hThread = NULL;

static CCriticalSection                 g_csPlayer;

typedef map< size_t, CClientNode * >    stdPlayerMap;
static  stdPlayerMap                    g_thePlayer;

static CPackager                        g_theRecv;
static CPackager                        g_theSend;

UINT g_nDBEngineLoop = 0;
UINT g_nServiceLoop  = 0;

extern HWND hListOutput;

#define WM_CREATE_ENGINE    WM_USER + 0x100
#define WM_ADD_CLIENT       WM_USER + 0x200
#define WM_DEL_CLIENT       WM_USER + 0x300

CFilterTextLib g_libFilterText;
CRoleNameFilter g_fltRoleName;

static size_t g_BackupSleepTime = 1;   // thoi gian ngu backup thread noi bo (khong dung lich noi bo cu)
static bool   g_IsBackupSuspend = false;  // co dang suspend backup thread khong
static bool g_bAutoBackupEnabled = false;
static UINT g_nAutoBackupMinutes = 30;
static const UINT g_nIDAutoTimer = 0x200; // timer rieng cho auto backup

/*
 * Helper function
 */
BOOL CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

bool CreateDatabaseEngine( HWND hwnd );
void DestroyDatabaseEngine();

void LoadSetting();
void SaveSetting();

DWORD WINAPI MonitorProc(LPVOID lpParameter) {
    while(true) {
        Sleep(60 * 1000);
    }
    return 0;
}

/*
 * Callback function
 */
typedef HRESULT ( __stdcall * pfnCreateServerInterface )(
            REFIID  riid,
            void    **ppv
        );

void __stdcall ServerEventNotify(
            LPVOID lpParam,
            const unsigned long &ulnID,
            const unsigned long &ulnEventType )
{
    HWND hwnd = ( HWND )lpParam;

    switch( ulnEventType )
    {
    case enumClientConnectCreate:
        {
            CClientNode *pNode = CClientNode::AddNode( g_pServer, ulnID );
            
            if ( pNode )
            {
                CCriticalSection::Owner lock( g_csPlayer );

                g_thePlayer.insert( stdPlayerMap::value_type( ulnID, pNode ) );

                ::PostMessage( hwnd, WM_ADD_CLIENT, ulnID, 0 );
            }
        }
        break;

    case enumClientConnectClose:
        {
            {
                CCriticalSection::Owner lock( g_csPlayer );

                stdPlayerMap::iterator it;
                if ( g_thePlayer.end() != ( it = g_thePlayer.find( ulnID ) ) )
                {
                    g_thePlayer.erase( ulnID );

                    ::PostMessage( hwnd, WM_DEL_CLIENT, ulnID, 0 );
                }
            }
            
            CClientNode::DelNode( ulnID );
        }
        break;
    }
}

DWORD WINAPI ThreadProcess( void *pParam )
{
    IServer *pServer = ( IServer * )pParam;

    ASSERT( pServer );

    g_hStartWorkEvent.Wait();

    while ( !g_hQuitEvent.Wait( 0 ) )
    {
        {
            CCriticalSection::Owner lock( g_csPlayer );
            
            stdPlayerMap::iterator it;
            for ( it = g_thePlayer.begin(); it != g_thePlayer.end(); it ++ )
            {
                UINT index = ( *it ).first;
                CClientNode *pClientNode = ( *it ).second;
                
                size_t datalength = 0;
                const char *pData = ( const char * )pServer->GetPackFromClient( index, datalength );
                
                while ( pClientNode && pData && datalength )
                {
                    pClientNode->AppendData( pData, datalength );
                    
                    pData = ( const char * )pServer->GetPackFromClient( index, datalength );
                }
            }
        }
    
        if ( ++ g_nServiceLoop & 0x80000000 )
        {
            g_nServiceLoop = 0;
        }

        if ( g_nServiceLoop & 0x1 )
        {
            ::Sleep( 1 );
        }
    }

    return 0L;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    //role name filter
    if (!g_libFilterText.Initialize()
        || !g_fltRoleName.Initialize())
    {
        MessageBox(NULL, "text filter's initing has failed", "error", MB_OK|MB_ICONERROR);
        return -1;
    }

    HWND hwndMain = ::CreateDialog( hInstance, 
        MAKEINTRESOURCE( IDD_DLG_GODDESS ),
        NULL, 
        (DLGPROC)MainWndProc );

    ASSERT( hwndMain );

    ::ShowWindow( hwndMain, SW_SHOWNORMAL );
    ::UpdateWindow( hwndMain );
    
    hListOutput = ::GetDlgItem( hwndMain, IDC_OUTPUT );//output list

    BOOL bRet;
    MSG msg;
    
    while ( ( bRet = ::GetMessage( &msg, NULL, 0, 0 ) ) != 0 )
    {
        if ( !IsWindow( hwndMain ) || !IsDialogMessage( hwndMain, &msg ) )
        {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
        }
    }
    
    //rolename filter
    g_fltRoleName.Uninitialize();
    g_libFilterText.Uninitialize();
    
    return msg.wParam;
}

/*
 *
 * MainWndProc() - Main window callback procedure.
 *  
 */

BOOL CALLBACK MainWndProc( HWND hwnd, 
                       UINT msg,
                       WPARAM wParam,
                       LPARAM lParam )
{
    static const UINT g_nIDEvent = 0x100;     // timer UI status
    static const UINT g_nElapse  = 500;       // 500ms
    static const char g_szBaseInfo[] = "..Running...........";
    static const int  g_nDots = sizeof( g_szBaseInfo ) - 1;
    static UINT g_nLastServiceLoop   = g_nServiceLoop;
    static int  g_nServiceStep       = g_nDots;
    static UINT g_nLastDBEngineLoop  = g_nDBEngineLoop;
    static int  g_nDBEngineStep      = g_nDots;
    static bool  s_fastUITick   = false;
    static bool  s_lastWasAuto  = false;
    static DWORD s_lastStartTick = 0;
    static DWORD s_lastEndTick   = 0;
    static bool  s_prevRunning   = false;

    BOOL bTranslated = TRUE;

    switch ( msg )
    {
    case WM_TIMER:
	{

		if (wParam == g_nIDAutoTimer)
		{
			// ton trong trang thai suspend: khong chay auto khi dang suspend
			if (IsBackupThreadWorking() && !g_IsBackupSuspend)
			{
				s_lastWasAuto   = true;
				s_lastStartTick = GetTickCount();

				// log ra list IDC_OUTPUT
				AddOutputString(hListOutput, "[Auto - Backup]: Den chu ky, bat dau !!");

				// cap nhat label + disable nut trong luc chay
				::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Auto backup: Running");
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), FALSE);

				// tang tan so cap nhat UI de thay doi trang thai kip thoi
				::SetTimer(hwnd, g_nIDEvent, 100, NULL);
				s_fastUITick = true;

				if (!DoManualBackup())
				{
					// neu khong bat dau duoc (dang busy), log ra UI
					AddOutputString(hListOutput, "[Auto - Backup]: Khong the bat dau (busy?)");
				}
			}
			return TRUE;
		}

		if (wParam == g_nIDEvent)
		{
			// Dong chay Service
			if ( g_nLastServiceLoop != g_nServiceLoop )
			{
				g_nServiceStep = (-- g_nServiceStep > 0 ) ? g_nServiceStep : g_nDots;
				::SetDlgItemText( hwnd, IDC_STATIC_SERVICE, ( const char * )g_szBaseInfo + g_nServiceStep );
				g_nLastServiceLoop = g_nServiceLoop;
			}

			// Dong chay DBEngine
			if ( g_nLastDBEngineLoop != g_nDBEngineLoop )
			{
				g_nDBEngineStep = (-- g_nDBEngineStep > 0 ) ? g_nDBEngineStep : g_nDots;
				::SetDlgItemText( hwnd, IDC_STATIC_DATABASE, ( const char * )g_szBaseInfo + g_nDBEngineStep );
				g_nLastDBEngineLoop = g_nDBEngineLoop;
			}

			// Trang thai backup
			BOOL isThread = IsBackupThreadWorking();
			BOOL isRun    = IsBackupWorking();

			// Phat hien chuyen tu running -> not running de log ket thuc
			if (s_prevRunning && !isRun)
			{
				s_lastEndTick = GetTickCount();

				if (s_lastEndTick && s_lastStartTick && (s_lastEndTick >= s_lastStartTick))
				{
					char buf[128];
					DWORD ms = s_lastEndTick - s_lastStartTick;
					sprintf(buf, "[Backup] Ket thuc (%s), thoi gian: %u ms",
							s_lastWasAuto ? "Auto" : "Manual", (unsigned)ms);
					AddOutputString(hListOutput, buf);
					// reset de tranh log lap
					s_lastEndTick = 0;
				}
			}
			s_prevRunning = (isRun != 0);

			// Cap nhat label va nut UI
			char statusBuf[96] = {0};
			if (!isThread)
			{
				lstrcpy(statusBuf, "Backup thread status: Stop");
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), FALSE );
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_MANUAL ), FALSE );
			}
			else if (g_IsBackupSuspend)
			{
				lstrcpy(statusBuf, "Backup thread status: Suspended");
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), TRUE );
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_MANUAL ), TRUE );
			}
			else if (isRun)
			{
				lstrcpy(statusBuf, s_lastWasAuto ? "Auto backup: Running" : "Manual backup: Running");
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), FALSE );
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_MANUAL ), FALSE );
			}
			else
			{
				lstrcpy(statusBuf, "Backup thread status: Sleeping");
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), TRUE );
				::EnableWindow( GetDlgItem( hwnd, IDC_BTN_BACKUP_MANUAL ), TRUE );
			}
			::SetWindowText(GetDlgItem( hwnd, IDC_LAB_BACKUP_STATUS ), statusBuf);

			// Dieu chinh tan so cap nhat UI
			if (isRun)
			{
				if (!s_fastUITick) {
					::SetTimer(hwnd, g_nIDEvent, 100, NULL);
					s_fastUITick = true;
				}
			}
			else
			{
				if (s_fastUITick) {
					::SetTimer(hwnd, g_nIDEvent, g_nElapse, NULL);
					s_fastUITick = false;
					// mo lai nut khi vua chay xong (neu co thread va khong suspend)
					if (IsBackupThreadWorking() && !g_IsBackupSuspend) {
						::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
						::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), TRUE);
					}
				}
			}

			return TRUE;
		}

		return FALSE;
	}
    case WM_INITDIALOG:
    {
        LoadSetting();

        // ====== Fill các field s?n có ======
        ::SetDlgItemInt(hwnd, IDC_EDIT_PORT,              g_nServerPort,    FALSE);
        ::SetDlgItemInt(hwnd, IDC_EDIT_MAXNUM_ROLE,       g_nMaxRoleCount,  FALSE);
        ::SetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, g_BackupSleepTime,FALSE);

        // ====== Auto backup UI ======
        ::CheckDlgButton(hwnd, IDC_CHK_AUTO_BACKUP, g_bAutoBackupEnabled ? BST_CHECKED : BST_UNCHECKED);
        ::SetDlgItemInt(hwnd, IDC_EDIT_AUTO_MINUTES, g_nAutoBackupMinutes, FALSE);
        ::SendMessage(GetDlgItem(hwnd, IDC_EDIT_AUTO_MINUTES), EM_SETREADONLY, FALSE, 0);
        ::SendMessage(GetDlgItem(hwnd, IDC_EDIT_AUTO_MINUTES), EM_LIMITTEXT, 5, 0);
        ::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_AUTO_MINUTES), g_bAutoBackupEnabled); // hoac TRUE neu muon nhap luon

        // ====== Timer UI status ======
        ::SetTimer(hwnd, g_nIDEvent, g_nElapse, NULL);

        // ====== B?t timer Auto n?u dang enable ======
        ::KillTimer(hwnd, g_nIDAutoTimer);
        if (g_bAutoBackupEnabled)
        {
            UINT minutes = g_nAutoBackupMinutes ? g_nAutoBackupMinutes : 1; // tránh 0
            ::SetTimer(hwnd, g_nIDAutoTimer, minutes * 60 * 1000, NULL);
        }

        // Hi?n tr?ng thái kh?i t?o cho label
        {
            char statusBuf[64];
            if (!IsBackupThreadWorking())
                lstrcpy(statusBuf, "Backup thread status: Stop");
            else if (g_IsBackupSuspend)
                lstrcpy(statusBuf, "Backup thread status: Suspended");
            else if (IsBackupWorking())
                lstrcpy(statusBuf, "Backup thread status: Running");
            else
                lstrcpy(statusBuf, "Backup thread status: Sleeping");

            HWND hLbl = GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS);
            if (hLbl) {
                SetWindowText(hLbl, statusBuf);
                InvalidateRect(hLbl, NULL, TRUE);
            }
        }

        return TRUE;
    }
    break;

case WM_CLOSE:
{
    DestroyDatabaseEngine();

    ::KillTimer( hwnd, g_nIDAutoTimer );
    ::KillTimer( hwnd, g_nIDEvent );

    g_nServerPort      = ::GetDlgItemInt( hwnd, IDC_EDIT_PORT, &bTranslated, FALSE );
    g_nMaxRoleCount    = ::GetDlgItemInt( hwnd, IDC_EDIT_MAXNUM_ROLE, &bTranslated, FALSE );
    g_BackupSleepTime  = ::GetDlgItemInt( hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, &bTranslated, FALSE );

    SaveSetting();

    ::DestroyWindow( hwnd );
    ::PostQuitMessage( 0 );
    return TRUE;
}
break;
    
    case WM_COMMAND:
    {
        // Checkbox Auto Backup
        if (LOWORD(wParam) == IDC_CHK_AUTO_BACKUP && HIWORD(wParam) == BN_CLICKED)
        {
            g_bAutoBackupEnabled = ( ::IsDlgButtonChecked(hwnd, IDC_CHK_AUTO_BACKUP) == BST_CHECKED );
            ::EnableWindow( GetDlgItem( hwnd, IDC_EDIT_AUTO_MINUTES ), g_bAutoBackupEnabled );

            ::KillTimer( hwnd, g_nIDAutoTimer );
            if ( g_bAutoBackupEnabled )
            {
                BOOL ok = FALSE;
                UINT minutes = ::GetDlgItemInt( hwnd, IDC_EDIT_AUTO_MINUTES, &ok, FALSE );
                if (!ok || minutes == 0) minutes = 1;
                g_nAutoBackupMinutes = minutes;

                ::SetTimer( hwnd, g_nIDAutoTimer, g_nAutoBackupMinutes * 60 * 1000, NULL );
            }
            return TRUE;
        }

        // Thay d?i minutes
        if (LOWORD(wParam) == IDC_EDIT_AUTO_MINUTES && HIWORD(wParam) == EN_CHANGE)
        {
            BOOL ok = FALSE;
            UINT minutes = ::GetDlgItemInt( hwnd, IDC_EDIT_AUTO_MINUTES, &ok, FALSE );
            if (ok)
            {
                if (minutes == 0) minutes = 1;
                g_nAutoBackupMinutes = minutes;

                if (g_bAutoBackupEnabled)
                {
                    ::KillTimer(hwnd, g_nIDAutoTimer);
                    ::SetTimer(hwnd, g_nIDAutoTimer, g_nAutoBackupMinutes * 60 * 1000, NULL);
                }
            }
            return TRUE;
        }

        switch (LOWORD(wParam))
        {
        case IDOK:
            ::EnableWindow( GetDlgItem( hwnd, IDOK ), FALSE );
            ::EnableWindow( GetDlgItem( hwnd, IDC_EDIT_PORT ), FALSE );
            ::EnableWindow( GetDlgItem( hwnd, IDC_EDIT_MAXNUM_ROLE ), FALSE );
            ::EnableWindow( GetDlgItem( hwnd, IDC_EDIT_BACKUP_SLEEP_TIME ), FALSE );

            ::SetWindowText( hwnd, "Goddess - [Start up...]" );
            ::PostMessage( hwnd, WM_CREATE_ENGINE, 0L, 0L );
            break;

        case IDCANCEL:
            ::PostMessage( hwnd, WM_CLOSE, 0L, 0L );
            break;

        case IDC_BTN_BACKUP_MANUAL:
			{
				if (!IsBackupThreadWorking())
				{
					MessageBox(hwnd, "Backup thread is not started yet.",
							   "Information", MB_OK | MB_ICONEXCLAMATION);
					break;
				}

				//goi ManualBackup
				if (DoManualBackup())
				{
					// danh dau thong tin lan chay
					s_lastWasAuto   = false;
					s_lastStartTick = GetTickCount();

					// log ra listbox IDC_OUTPUT
					AddOutputString(hListOutput, (char*)"[Manual-Backup]: Bat dau");

					// cap nhat UI ngay lap tuc
					::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Manual backup: Running");
					::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);
					::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), FALSE);

					// tang tan so cap nhat UI trong luc chay
					::SetTimer(hwnd, g_nIDEvent, 100, NULL);
					s_fastUITick = true;
				}
				else
				{
					AddOutputString(hListOutput, (char*)"[Manual-Backup]: Khong the bat dau (busy?)");
					MessageBox(hwnd, "Unable to start manual backup.\nPlease try later.",
							   "Information", MB_OK | MB_ICONEXCLAMATION);
				}
			}
			break;


        case IDC_RESTART:
        {
            CClientNode::End();
            ReleaseDBInterface();

            if (!InitDBInterface(g_nMaxRoleCount)) {
                MessageBox(hwnd, "Init database failed after restart.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            // B?t l?i backup thread theo thi?t l?p hi?n t?i
            if (!StartBackupTimer(g_BackupSleepTime)) {
                // optional: MessageBox c?nh báo
            }

            // C?p nh?t UI tr?ng thái backup
            if (IsBackupThreadWorking()) {
                ::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup thread status: Sleeping");
                ::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
                ::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), TRUE);
            } else {
                ::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup thread status: Stop");
                ::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);
                ::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), FALSE);
            }

            CClientNode::Start( g_pServer );
        }
        break;

        case IDC_BTN_BACKUP_SUS_RES:
            if(!IsBackupThreadWorking())
            {
                MessageBox(hwnd, "Backup thread is not started yet.",
                           "Information",MB_OK | MB_ICONEXCLAMATION);
            }
            else if(g_IsBackupSuspend)
            {
                if(ResumeBackupTimer())
                {
                    g_IsBackupSuspend = false;
                    ::SetWindowText(GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), "Suspend");
                }
                else
                {
                    MessageBox(hwnd, "Unable to resume backup thread.\nPlease try later.",
                               "Information",MB_OK | MB_ICONEXCLAMATION);
                }
            }
            else
            {
                if(SuspendBackupTimer())
                {
                    g_IsBackupSuspend = true;
                    ::SetWindowText(GetDlgItem( hwnd, IDC_BTN_BACKUP_SUS_RES ), "Resume");
                }
                else
                {
                    MessageBox(hwnd, "Unable to suspend backup thread.\nPlease try later.",
                               "Information",MB_OK | MB_ICONEXCLAMATION);
                }
            }
            break;
        }
    }
    break;
    
    case WM_CREATE_ENGINE:
    {
        g_nServerPort       = ::GetDlgItemInt( hwnd, IDC_EDIT_PORT, &bTranslated, FALSE );
        g_nMaxRoleCount     = ::GetDlgItemInt( hwnd, IDC_EDIT_MAXNUM_ROLE, &bTranslated, FALSE );
        g_BackupSleepTime   = ::GetDlgItemInt( hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, &bTranslated, FALSE );

        g_bAutoBackupEnabled = ( ::IsDlgButtonChecked(hwnd, IDC_CHK_AUTO_BACKUP) == BST_CHECKED );
        g_nAutoBackupMinutes = ::GetDlgItemInt( hwnd, IDC_EDIT_AUTO_MINUTES, &bTranslated, FALSE );
        if (g_nAutoBackupMinutes == 0) g_nAutoBackupMinutes = 1;

        // C?p nh?t timer auto theo UI
        ::KillTimer( hwnd, g_nIDAutoTimer );
        if ( g_bAutoBackupEnabled )
        {
            ::SetTimer( hwnd, g_nIDAutoTimer, g_nAutoBackupMinutes * 60 * 1000, NULL );
        }

        CreateDatabaseEngine( hwnd );

        ::SetWindowText( hwnd, "Goddess - [Enable]" );
    }
    break;

    case WM_ADD_CLIENT:
    {
        const char *pText = g_pServer->GetClientInfo( wParam );

        if ( pText && pText[0] )
        {
            HWND hCtrl = ::GetDlgItem( hwnd, IDC_LIST_CLIENT );
            
            if ( hCtrl && ::IsWindow( hCtrl ) )
            {
                int nIndex = ::SendMessage( hCtrl, LB_ADDSTRING, 0, ( LPARAM )pText );
                
                if ( LB_ERR != nIndex )
                {
                    ::SendMessage( hCtrl, LB_SETITEMDATA, nIndex, wParam );
                }
            }
        }
    }       
    break;

    case WM_DEL_CLIENT:
    {
        HWND hCtrl = ::GetDlgItem( hwnd, IDC_LIST_CLIENT );
        
        if ( hCtrl && ::IsWindow( hCtrl ) )
        {
            int nCount = ::SendMessage( hCtrl, LB_GETCOUNT, 0, 0 );
            
            for ( int i=0; i<nCount && LB_ERR != nCount; i++ )
            {
                UINT nSearchID = (UINT)::SendMessage( hCtrl, LB_GETITEMDATA, i, 0 );
                if ( wParam == nSearchID )
                {
                    ::SendMessage( hCtrl, LB_DELETESTRING, i, 0 );
                    return TRUE;
                }
            }
        }
    }
    break;
	case WM_CTLCOLORSTATIC:
	{
		HDC  hdc   = (HDC)wParam;
		HWND hCtrl = (HWND)lParam;

		// Chi to mau cho label status
		if (hCtrl == GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS))
		{
			char text[128] = {0};
			GetWindowText(hCtrl, text, sizeof(text)-1);

			// Mac dinh: den
			COLORREF clr = RGB(0,0,0);

			// Map mau theo noi dung
			if (strstr(text, "Sleeping"))     clr = RGB(0,128,0);    // xanh la
			else if (strstr(text, "Stop"))    clr = RGB(220,0,0);    // do
			else if (strstr(text, "Suspended")) clr = RGB(255,128,0); // cam
			else if (strstr(text, "Running")) clr = RGB(0,0,200);    // xanh duong (tuy chon)

			SetTextColor(hdc, clr);
			SetBkMode(hdc, TRANSPARENT);

			// Nen su dung brush he thong -> khong can delete
			return (BOOL)GetSysColorBrush(COLOR_3DFACE);
		}

		// De mac dinh cho static khac
		return (BOOL)DefWindowProc(hwnd, msg, wParam, lParam);
	}

    default:
        break;      
    }

    return FALSE;
}

bool CreateDatabaseEngine( HWND hwnd )
{
    if ( !InitDBInterface( g_nMaxRoleCount ) )
    {
        ::MessageBox( NULL, "Setup dbserver is failed!", "Warning", MB_OK | MB_ICONSTOP );
        return false;
    }
    
    StartBackupTimer(g_BackupSleepTime); // bat backup thread
    ::SetWindowText(GetDlgItem( hwnd, IDC_LAB_BACKUP_STATUS ), "Backup thread status: Sleeping");

    // Open this server to client
    pfnCreateServerInterface pFactroyFun = ( pfnCreateServerInterface )( g_theHeavenLibrary.GetProcAddress( "CreateInterface" ) );

    IServerFactory *pServerFactory = NULL;

    if ( pFactroyFun && SUCCEEDED( pFactroyFun( IID_IServerFactory, reinterpret_cast< void ** >( &pServerFactory ) ) ) )
    {
        pServerFactory->SetEnvironment( 10, 10, 20, 4 * 1024 * 1024 );
        
        pServerFactory->CreateServerInterface( IID_IIOCPServer, reinterpret_cast< void ** >( &g_pServer ) );
        
        pServerFactory->Release();
    }

    if ( !g_pServer )
    {
        ::MessageBox( NULL, "Initialization failed! Don't find a correct heaven.dll", "Warning", MB_OK | MB_ICONSTOP );
        return false;
    }

    g_pServer->Startup();

    g_pServer->RegisterMsgFilter( ( void * )hwnd, ServerEventNotify );

    if ( FAILED( g_pServer->OpenService( INADDR_ANY, g_nServerPort ) ) )
    {
        return false;
    }

    DWORD dwThreadID = 0L;

    IServer *pCloneServer = NULL;
    g_pServer->QueryInterface( IID_IIOCPServer, ( void ** )&pCloneServer );

    g_hThread = ::CreateThread( NULL, 0, ThreadProcess, ( void * )pCloneServer, 0, &dwThreadID );

    if ( !g_hThread )
    {
        return false;
    }

    CClientNode::Start( g_pServer );

    g_hStartWorkEvent.Set();
    
    return true;
}

void DestroyDatabaseEngine()
{
    CClientNode::End();

    g_hQuitEvent.Set();
    
    DWORD result = ::WaitForSingleObject( g_hThread, 50000 );

    if ( result == WAIT_TIMEOUT )
    {
        ::TerminateThread( g_hThread, ( DWORD )( -1 ) );
    }

    SAFE_CLOSEHANDLE( g_hThread );

    if ( g_pServer )
    {
        g_pServer->CloseService();
        g_pServer = NULL;
    }

    SAFE_RELEASE( g_pServer );

    ReleaseDBInterface();
}

void LoadSetting()
{
    CIniFile theConfigFile;

    _tstring sAppPath = GetAppFullPath( NULL );

    _tstring sConfigFile;

    sConfigFile = sAppPath + "Goddess.cfg";

    theConfigFile.SetFile( sConfigFile.c_str() );

    g_nServerPort     = theConfigFile.ReadInteger( "Setting", "Port", 5001 );
    g_nMaxRoleCount   = theConfigFile.ReadInteger( "Setting", "MaxRoleCount", 3 );
    g_BackupSleepTime = theConfigFile.ReadInteger( "Setting", "BackupSleepTime", 3 );

    // auto backup config
    g_bAutoBackupEnabled = theConfigFile.ReadInteger( "Setting", "AutoBackupEnabled", 0 ) != 0;
    g_nAutoBackupMinutes = theConfigFile.ReadInteger( "Setting", "AutoBackupMinutes", 30 );
    if (g_nAutoBackupMinutes == 0) g_nAutoBackupMinutes = 1;
}

void SaveSetting()
{
    CIniFile theConfigFile;

    _tstring sAppPath = GetAppFullPath( NULL );

    _tstring sConfigFile;

    sConfigFile = sAppPath + "Goddess.cfg";

    theConfigFile.SetFile( sConfigFile.c_str() );

    theConfigFile.WriteInteger( "Setting", "Port", g_nServerPort );
    theConfigFile.WriteInteger( "Setting", "MaxRoleCount", g_nMaxRoleCount );
    theConfigFile.WriteInteger( "Setting", "BackupSleepTime", g_BackupSleepTime );

    // auto backup config
    theConfigFile.WriteInteger( "Setting", "AutoBackupEnabled", g_bAutoBackupEnabled ? 1 : 0 );
    theConfigFile.WriteInteger( "Setting", "AutoBackupMinutes",  g_nAutoBackupMinutes );
}
