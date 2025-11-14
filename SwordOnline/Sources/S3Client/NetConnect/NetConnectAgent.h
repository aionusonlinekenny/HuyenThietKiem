/*****************************************************************************************
//	�������ӣ��㼯��������Ϣ�������͵ִ���Ϣ���Ĵ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-6
// --
	Engine���KNetClientģ���װʵ�������������봫�Ͱ�����ģ��ΪKNetClient����Ӧ��ʱ�Ĵ�����
��Ҫ���ڻ㼯����Ӧ������Ҫ���͵���������Լ��ѵִ����������͵�����ش�������ģ�顣
*****************************************************************************************/
#pragma once
//#include "KNetClient.h"
#include "../../../Headers/KProtocol.h"
#include "../../../Headers/iClient.h"

struct iKNetMsgTargetObject;
typedef void (*fnNetMsgCallbackFunc)(void* pMsgData);

typedef HRESULT ( __stdcall * pfnCreateClientInterface )(
			REFIID	riid,
			void	**ppv
		);


//====Ĭ�ϵĳ�ʱʱ��====
#define	DEF_TIMEOUT_LIMIT	300000	//300sec = 5 minutes (increased from 60sec to prevent AUTO PLAY timeout)

class KNetConnectAgent
{
public:
	KNetConnectAgent();
	~KNetConnectAgent();
	//��ʼ��
	int		Initialize();
	//�˳�
	void	Exit();

	//��������
	int		ClientConnectByNumericIp(const unsigned char* pIpAddress, unsigned short pszPort);
	//�ر�����
	void	DisconnectClient();

	int		ConnectToGameSvr(const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid);
	void	DisconnectGameSvr();

	//������Ϣ
	int		SendMsg(const void *pBuffer, int nSize);
	//��������Ϊ
	void	Breathe();

	void	UpdateClientRequestTime(bool bCancel, unsigned int uTimeLimit = DEF_TIMEOUT_LIMIT);

	//ע��ִ���Ϣ��Ӧ����
	void	RegisterMsgTargetObject(PROTOCOL_MSG_TYPE Msg, iKNetMsgTargetObject* pObject);

	int		IsConnecting(int bGameServ);

	void	TobeDisconnect();

private:
	bool	ProcessSwitchGameSvrMsg(void* pMsgData);			//������Ϸ�����������������Ϣ

private:
	IClient*				m_pClient;
	IClient*				m_pGameSvrClient;

private:
#define	MAX_MSG_COUNT	1 << (PROTOCOL_MSG_SIZE * 8)
	iKNetMsgTargetObject*	m_MsgTargetObjs[MAX_MSG_COUNT];

	HMODULE					    m_hModule;
	pfnCreateClientInterface    m_pFactroyFun;
	IClientFactory             *m_pClientFactory;



	bool					m_bIsClientConnecting;
	bool					m_bIsGameServConnecting;
	bool					m_bTobeDisconnect;
	unsigned int			m_uClientRequestTime;		//���������ʱ��
	unsigned int			m_uClientTimeoutLimit;
};

extern KNetConnectAgent g_NetConnectAgent;