#ifndef S3ClientH
#define	S3ClientH

#include "KEngine.h"
#include "KMp3Music.h"
#include "../../Core/Src/CoreShell.h"

class KInlinePicSink;
class KMyApp : public KWin32App
{
private:
	KMp3Music			m_Music;
	KDirectSound		m_Sound;
	KTimer				m_Timer;
	DWORD				m_GameCounter;
	KInlinePicSink*		m_pInlinePicSink;
	
public:
	KMyApp();
	BOOL				GameInit();
	BOOL				GameLoop();
	BOOL				GameExit();

protected:
	int					HandleInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

struct KClientCallback : public IClientCallback
{
	void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam);
	void ChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc);
	void MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc);
	void NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost);
	void FriendInvite(char* roleName);
	void AddFriend(char* roleName, BYTE answer);
	void FriendStatus(char* roleName, BYTE state);
	void FriendInfo(char* roleName, char* unitName, char* groupName, BYTE state);
	void AddPeople(char* unitName, char* roleName);
};

#endif