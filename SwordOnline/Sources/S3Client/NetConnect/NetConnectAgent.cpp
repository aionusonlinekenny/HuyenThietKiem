/*****************************************************************************************
//	�������ӣ��㼯�����Ͱ������͵ִ���Ĵ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-6
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include <objbase.h>
#include <initguid.h>
#include "KEngine.h"
#include "NetConnectAgent.h"
#include "NetMsgTargetObject.h"
#include "../Ui/Elem/Wnds.h"
#include "../Ui/UiCase/UiSysMsgCentre.h"
#include "../Ui/UiShell.h"
#include "../../Core/Src/CoreShell.h"
#include "../../../Headers/KProtocolDef.h"
#include "crtdbg.h"

#define	NETCONNECT_MODULE			"Rainbow.dll"

extern int			g_bDisconnect;


void __stdcall ClientCallBack(LPVOID lpParam, const unsigned long &ulnEventType);

extern iCoreShell*		g_pCoreShell;
KNetConnectAgent g_NetConnectAgent;
#ifdef _DEBUG
static int g_snBugLog;
#endif

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KNetConnectAgent::KNetConnectAgent()
{
	memset(&m_MsgTargetObjs, 0, sizeof(m_MsgTargetObjs));
	m_pClient = NULL;
	m_pGameSvrClient = NULL;
	m_hModule = NULL;
	m_bIsClientConnecting = false;
	m_bTobeDisconnect = false;
	m_uClientRequestTime = 0;

	m_pFactroyFun = NULL;
	m_pClientFactory = NULL;

}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
KNetConnectAgent::~KNetConnectAgent()
{
	Exit();
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KNetConnectAgent::Initialize()
{
	m_hModule = ::LoadLibrary(NETCONNECT_MODULE);

	if (m_hModule)
	{
		m_pFactroyFun = (pfnCreateClientInterface)GetProcAddress(m_hModule, "CreateInterface");
		if (m_pFactroyFun)
		{
			if (SUCCEEDED(m_pFactroyFun(IID_IClientFactory, reinterpret_cast< void ** >( &m_pClientFactory))))
			{
				m_pClientFactory->SetEnvironment(1024 * 512);
			}
		}
	}

	if (!m_pFactroyFun || !m_pClientFactory)
        return false;

	m_bIsClientConnecting = false;
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ��˳�
//--------------------------------------------------------------------------
void KNetConnectAgent::Exit()
{
	memset(&m_MsgTargetObjs, 0, sizeof(m_MsgTargetObjs));
	
    DisconnectClient();
	DisconnectGameSvr();

    if (m_pClientFactory)
    {
        m_pClientFactory->Release();
        m_pClientFactory = NULL;
    }

    m_pFactroyFun = NULL;

	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
int	KNetConnectAgent::ClientConnectByNumericIp(const unsigned char* pIpAddress, unsigned short nPort)
{	
	DisconnectClient();
	
    if (!pIpAddress || !nPort)
        return false;

    if (!m_pClientFactory)
        return false;

	m_pClientFactory->CreateClientInterface( IID_IESClient, reinterpret_cast< void ** >(&m_pClient));

    if (!m_pClient)
        return false;

	if (FAILED(m_pClient->Startup()))
		return false;

	m_pClient->RegisterMsgFilter((void*)false, ClientCallBack);

    m_bIsClientConnecting = false;

	char	Address[128];
	sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],
		pIpAddress[2], pIpAddress[3]);
	if (SUCCEEDED(m_pClient->ConnectTo(Address, nPort)))
	{
		//g_DebugLog("[Gateway] connectted.");
		m_bIsClientConnecting = true;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
//	���ܣ��ر�����
//--------------------------------------------------------------------------
void KNetConnectAgent::DisconnectClient()
{
	if (m_bIsGameServConnecting == false)
		m_uClientRequestTime = 0;

	if (m_pClient && m_bIsClientConnecting)
	{
		//g_DebugLog("[Gateway] connection closed.");
		m_bIsClientConnecting = false;
		m_pClient->Shutdown();
	}

	if (m_pClient)
	{
		m_pClient->Cleanup();
		m_pClient->Release();
		m_pClient = NULL;
	}

}

int KNetConnectAgent::ConnectToGameSvr(const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid)
{
	
	DisconnectGameSvr();
	if (pGuid == NULL)
        return false;

    if (!m_pClientFactory)
        return false;

    m_pClientFactory->CreateClientInterface( IID_IESClient, reinterpret_cast< void ** >(&m_pGameSvrClient));
    
    if (!m_pGameSvrClient)
        return false;

	if (FAILED(m_pGameSvrClient->Startup()))
		return false;

	m_pGameSvrClient->RegisterMsgFilter((void*)true, ClientCallBack);


	char	Address[128];
	sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],
		pIpAddress[2], pIpAddress[3]);
	
	if (FAILED(m_pGameSvrClient->ConnectTo(Address, uPort)))
		return false;

	m_bIsGameServConnecting = true;
	tagLogicLogin ll;
	ll.cProtocol = c2s_logiclogin;
	memcpy( &ll.guid, pGuid, sizeof(GUID));
	if (FAILED(m_pGameSvrClient->SendPackToServer(&ll, sizeof(tagLogicLogin))))
		return false;

	if (g_pCoreShell)
		g_pCoreShell->SetClient(m_pGameSvrClient);

	return true;
}

void KNetConnectAgent::DisconnectGameSvr()
{
	if (m_bIsClientConnecting == false)
		m_uClientRequestTime = 0;

	if (m_pGameSvrClient && m_bIsGameServConnecting)
	{
		if (g_pCoreShell)
			g_pCoreShell->SetClient(NULL);

		m_bIsGameServConnecting = false;
		m_pGameSvrClient->Shutdown();
	}    

	if (m_pGameSvrClient)
	{
		m_pGameSvrClient->Cleanup();
		m_pGameSvrClient->Release();
		m_pGameSvrClient = NULL;
	}

}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ
//--------------------------------------------------------------------------
int KNetConnectAgent::SendMsg(const void *pBuffer, int nSize)
{
	if (m_pClient)
	{
		m_pClient->SendPackToServer((BYTE*)pBuffer, nSize);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���������Ϊ
//--------------------------------------------------------------------------
void KNetConnectAgent::Breathe()
{
	//----��ʱ�ж�----
	if (m_uClientRequestTime &&
		GetTickCount() - m_uClientRequestTime >= m_uClientTimeoutLimit)
	{
		g_bDisconnect = true;
		m_bTobeDisconnect = true;
	}

	//----�����ж�----
	if (m_bTobeDisconnect)
	{
		m_bTobeDisconnect = false;
//		m_bIsClientConnecting = false;//to be check
//		m_bIsGameServConnecting = false;
		DisconnectClient();
		DisconnectGameSvr();
		return;
	}

	unsigned int nSize;
	const char* pBuffer = NULL;

	//----�����������ص�����----
	if (m_bIsClientConnecting)
	{
		while (true)
		{
            if (!m_pClient)
                break;

            pBuffer = (const char*)m_pClient->GetPackFromServer(nSize);
            if (!(pBuffer && nSize))
                break;

			PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			PROTOCOL_MSG_TYPE	Msg = *pMsg;			

			_ASSERT(Msg > s2c_multiserverbegin || Msg < s2c_end);
				
			if (m_MsgTargetObjs[Msg])
				(m_MsgTargetObjs[Msg])->AcceptNetMsg(pMsg);
		}
	}
	//----����������Ϸ������������----
	if (m_bIsGameServConnecting && m_pGameSvrClient)
	{
		while (true)
		{
			if (!m_pGameSvrClient)
                break;
            
            pBuffer = (const char*)m_pGameSvrClient->GetPackFromServer(nSize);

            if (!(pBuffer && nSize))
                break;

            
            PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			while(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
			{
				PROTOCOL_MSG_TYPE	Msg = *pMsg;			


				// ���������Э��
				if (Msg == s2c_notifyplayerexchange)
				{
					ProcessSwitchGameSvrMsg(pMsg);
					break;
				}
				else if (g_pCoreShell)
				{
					_ASSERT(Msg > s2c_clientbegin && Msg < s2c_end);
					_ASSERT(g_pCoreShell->GetProtocolSize(Msg) != 0);
					g_pCoreShell->NetMsgCallbackFunc(pMsg);
					if (g_pCoreShell->GetProtocolSize(Msg) > 0)
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
					else
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE + (*(unsigned short*) (((char*)pMsg) + PROTOCOL_MSG_SIZE)));
				}

			}
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ�ע��ִ���Ϣ��Ӧ����
//--------------------------------------------------------------------------
void KNetConnectAgent::RegisterMsgTargetObject(PROTOCOL_MSG_TYPE Msg, iKNetMsgTargetObject* pObject)
{
	if (Msg >= 0 && Msg < MAX_MSG_COUNT)
		m_MsgTargetObjs[Msg] = pObject;
}

void KNetConnectAgent::TobeDisconnect()
{
	m_bTobeDisconnect = true;
}

void KNetConnectAgent::UpdateClientRequestTime(bool bCancel, unsigned int uTimeLimit)
{
	if (m_bIsClientConnecting || m_bIsGameServConnecting)
	{
		if (bCancel == false)
		{
			m_uClientRequestTime = GetTickCount();
			if (m_uClientRequestTime == 0)
				m_uClientRequestTime = 1;
			m_uClientTimeoutLimit = uTimeLimit;
		}
		else
			m_uClientRequestTime = 0;
	}
}

int	KNetConnectAgent::IsConnecting(int bGameServ)
{
	if (bGameServ)
		return m_bIsGameServConnecting;
	else
		return m_bIsClientConnecting;
}

void __stdcall ClientCallBack(LPVOID lpParam, const unsigned long &ulnEventType)
{
	switch(ulnEventType)
	{
	case enumServerConnectCreate:
		break;
	case enumServerConnectClose:
		{
			int	bGameServ = (int)lpParam;
			if (g_NetConnectAgent.IsConnecting(bGameServ))
			{
				g_bDisconnect = true;
				g_NetConnectAgent.TobeDisconnect();
			}
		}
		break;
	}
}

//������Ϸ�����������������Ϣ
bool KNetConnectAgent::ProcessSwitchGameSvrMsg(void* pMsgData)
{
	tagNotifyPlayerExchange* pInfo = (tagNotifyPlayerExchange*)pMsgData;
	_ASSERT(pInfo && pInfo->cProtocol == s2c_notifyplayerexchange);
	
	DWORD	dwIp = pInfo->nIPAddr;
	int	nPort = pInfo->nPort;
	GUID guid;
	guid = pInfo->guid;

	if (dwIp && nPort)
	{
		// ��ʼ��GameSvr��������
		g_pCoreShell->OperationRequest(GOI_EXIT_GAME, 0, 0);
		if (ConnectToGameSvr((const unsigned char*)&dwIp, nPort, &guid))
		{
			Wnd_GameSpaceHandleInput(false);
			UpdateClientRequestTime(false);
			if (g_pCoreShell)
            g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, 0, 0);
			UiOnGameServerConnected();
			//Kenny Fix Skill



			return true;
		}
	}
	else
	{
		// �������ʧ��
//		MessageBox(NULL, "", "", MB_OK);

#define	MSG_EXCHANGE_FAIL	"ǰ��·;��̫ͨ�������ǹ��������ɡ�"
		KSystemMessage	Msg;
		Msg.byConfirmType = SMCT_NONE;
		Msg.byParamSize = 0;
		Msg.byPriority = 0;
		Msg.eType = SMT_SYSTEM;
		
		strcpy(Msg.szMessage, MSG_EXCHANGE_FAIL);
		KUiSysMsgCentre::AMessageArrival(&Msg, 0);
		g_bDisconnect = false;
		return false;
		
	}
	// FIX: Don't force disconnect on connection failure
	// Allow player to retry or manually reconnect
	g_bDisconnect = false;  // Changed from true
	// g_NetConnectAgent.TobeDisconnect();  // Commented out - don't force disconnect
	return false;	
}