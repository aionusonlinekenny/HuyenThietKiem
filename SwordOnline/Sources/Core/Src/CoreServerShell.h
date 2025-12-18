#ifndef CORESERVERSHELL_H
#define CORESERVERSHELL_H

#include "CoreServerDataDef.h"


//=========================================================
// 
//=========================================================
enum SERVER_SHELL_OPERATION_INDEX
{
	SSOI_LAUNCH = 1,
	SSOI_SHUTDOWN,
	SSOI_BROADCASTING,
	SSOI_RELOAD_WELCOME_MSG,
	SSOI_TONG_CREATE,
	SSOI_TONG_REFUSE_ADD,
	SSOI_TONG_ADD,
};


enum GAMEDATA_INDEX
{
	SGDI_CHARACTER_NAME,

	SGDI_LOADEDMAP_ID,

	SGDI_CHARACTER_ACCOUNT,

	SGDI_CHARACTER_ID,

	SGDI_CHARACTER_NETID,

	SGDI_CHARACTER_EXTPOINT,

	SGDI_CHARACTER_EXTPOINTCHANGED,

	SGDI_TONG_APPLY_CREATE,

	SGDI_TONG_APPLY_ADD,

	SGDI_TONG_CHECK_ADD_CONDITION,

	SGDI_TONG_GET_INFO,

	SGDI_TONG_INSTATE_POWER,

	SGDI_TONG_BE_INSTATED,

	SGDI_TONG_KICK_POWER,

	SGDI_TONG_BE_KICKED,

	SGDI_TONG_LEAVE_POWER,

	SGDI_TONG_LEAVE,

	SGDI_TONG_CHANGE_MASTER_POWER,

	SGDI_TONG_GET_MASTER_POWER,

	SGDI_TONG_CHANGE_AS,

	SGDI_TONG_CHANGE_MASTER,

	SGDI_TONG_GET_TONG_NAMEID,

	SGDI_TONG_LOGIN,

	SGDI_TONG_SEND_SELF_INFO,

	SGDI_CHARACTER_STATUS,
	
	SGDI_TONG_CHANGE_CAMP_POWER,

	SGDI_TONG_CHANGE_CAMP,

	SGDI_TONG_CHANGE_MEMBER_TITLE_POWER,

	SGDI_TONG_CHANGE_MEMBER_TITLE,

	SGDI_TONG_CHANGE_ONE_TITLE_POWER,

	SGDI_TONG_CHANGE_ONE_TITLE,

	SGDI_TONG_SAVE_MONEY_POWER,

	SGDI_TONG_SAVE_MONEY_FAIL,

	SGDI_TONG_SAVE_MONEY,

	SGDI_TONG_GET_MONEY_POWER,

	SGDI_TONG_GET_MONEY,
};

#ifdef _STANDALONE
class IClient;
#else
struct IClient;
#endif

#ifndef _STANDALONE
struct _declspec (novtable) iCoreServerShell
#else
struct iCoreServerShell
#endif
{
	virtual int  GetLoopRate() = 0;
	virtual void GetGuid(int nIndex, void* pGuid) = 0;
	virtual DWORD GetExchangeMap(int nIndex) = 0;
	virtual bool IsPlayerLoginTimeOut(int nIndex) = 0;
	virtual void RemovePlayerLoginTimeOut(int nIndex) = 0;
	virtual bool IsPlayerExchangingServer(int nIndex) = 0;
	virtual void ProcessClientMessage(int nIndex, const char* pChar, int nSize) = 0;
	virtual void ProcessNewClientMessage(IClient* pTransfer, DWORD dwFromIP, DWORD dwFromRelayID, int nPlayerIndex, const char* pChar, int nSize) = 0;
	virtual void SendNetMsgToTransfer(IClient* pClient) = 0;
	virtual void SendNetMsgToChat(IClient* pClient) = 0;
	virtual void SendNetMsgToTong(IClient* pClient) = 0;
	virtual void ProcessBroadcastMessage(const char* pChar, int nSize) = 0;
	virtual void ProcessExecuteMessage(const char* pChar, int nSize) = 0;
	virtual void ClientDisconnect(int nIndex) = 0;
	virtual void RemoveQuitingPlayer(int nIndex) = 0;
	virtual void* SavePlayerDataAtOnce(int nIndex) = 0;
	virtual bool IsCharacterQuiting(int nIndex) = 0;

	virtual bool CheckProtocolSize(const char* pChar, int nSize, BOOL& bShutDown) = 0;
	virtual bool PlayerDbLoading(int nPlayerIndex, int bSyncEnd, int& nStep, unsigned int& nParam) = 0;
	virtual int  AttachPlayer(const unsigned long lnID, GUID* pGuid) = 0;
	virtual void GetPlayerIndexByGuid(GUID* pGuid, int* pnIndex, int* plnID) = 0;
	virtual void AddPlayerToWorld(int nIndex) = 0;
	virtual void* PreparePlayerForExchange(int nIndex) = 0;
	virtual void PreparePlayerForLoginFailed(int nIndex) = 0;
	virtual void RemovePlayerForExchange(int nIndex) = 0;
	virtual void RecoverPlayerExchange(int nIndex) = 0;
	virtual int  AddCharacter(void* pBuffer, GUID* pGuid) = 0;

	virtual int	 AddTempTaskValue(int nIndex, const char* pData) = 0;

	virtual int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam) = 0;

	virtual int	 GetConnectInfo(KCoreConnectInfo* pInfo) = 0;

	virtual int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam) = 0;

	virtual int  Breathe() = 0;
	virtual void Release() = 0;
	virtual void SetSaveStatus(int nIndex, UINT uStatus) = 0;
	virtual UINT GetSaveStatus(int nIndex) = 0;
	virtual BOOL GroupChat(IClient* pClient, DWORD FromIP, unsigned long FromRelayID, DWORD channid, BYTE tgtcls, DWORD tgtid, const void* pData, size_t size) = 0;
	virtual	void SetLadder(void* pData, size_t uSize) = 0;
	virtual BOOL PayForSpeech(int nIndex, int nType) = 0;
	virtual int LixianStatus(int nIndex) = 0; //Lixian by kinnox;
	virtual void SetStatusLixian(int nIndex, int nStatus); //Lixian by kinnox;	
};

#ifndef CORE_EXPORTS

#ifndef __linxu
	extern "C" 
#endif
	iCoreServerShell* CoreGetServerShell(int nProtocolVersion);

#endif
#endif
