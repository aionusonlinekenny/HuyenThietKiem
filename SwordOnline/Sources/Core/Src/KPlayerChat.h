
#ifndef KPLAYERCHAT_H
#define KPLAYERCHAT_H
#include	"GameDataDef.h"
#ifndef _SERVER
#include	"KNode.h"
#include	"KList.h"
#endif
#ifdef _SERVER
#define		MAX_FRIEND_TEAM				1
#define		CHAT_SPEED					20
#else
#define		MAX_FRIEND_TEAM				10
#endif
#define		CHAT_APPLY_ADD_FRIEND_LIST	10

//==============================
//
//==============================
enum
{
	CHAT_CUR_CHANNEL_NONE = 0,		
	CHAT_CUR_CHANNEL_ALL,		
	CHAT_CUR_CHANNEL_SCREEN,	
	CHAT_CUR_CHANNEL_SINGLE,
	CHAT_CUR_CHANNEL_TEAM,
	CHAT_CUR_CHANNEL_FACTION,
	CHAT_CUR_CHANNEL_TONG,
	CHAT_CUR_CHANNEL_SCREENSINGLE,
	CHAT_CUR_CHANNEL_SYSTEM,
	CHAT_CUR_CHANNEL_NUM,
};

extern	DWORD	g_dwChannelAnd[CHAT_CUR_CHANNEL_NUM];
extern	DWORD	g_dwChannelOr[CHAT_CUR_CHANNEL_NUM];

#ifndef _SERVER
class KCHAT_RECORDER
{
private:
	char	m_szDay[11];
	char	m_szTime[9];

private:
	void		SetTime();

public:
	void		SaveSentenceSingle(char *lpszSelfName, char *lpszTarget, char *lpszTalkerName, char *lpszSentence);
	void		SaveSentenceChannel(char *lpszSelfName, char *lpszTalkerName, char *lpszSentence);

};
#endif


class CChatFriend : public KNode
{
public:
	DWORD			m_dwID;
	int				m_nPlayerIdx;
	char			m_szName[32];

public:
	CChatFriend() {Release();};
	void			Release() {m_dwID = 0; memset(m_szName, 0, 32);};
};

class CChatFriendTeam
{
public:
	int				m_nFriendNo;	
	char			m_szTeamName[32];
	KList			m_cEveryOne;
public:
	CChatFriendTeam();
	void			Release();
#ifdef _SERVER
	void			DataBaseAddOne(int nSelfIdx, DWORD dwID, char *lpszName);
#endif
};

#ifndef _SERVER
class CChatApplyListNode : public KNode
{
public:
	int		m_nPlayerIdx;
	int		m_nAccessFlag;
	char	m_szName[32];
	char	m_szInfo[MAX_SENTENCE_LENGTH];

public:
	CChatApplyListNode() { m_nPlayerIdx = 0; m_nAccessFlag = 0; m_szName[0] = 0; m_szInfo[0] = 0; };
};
#endif

#ifdef _SERVER
#include	<list>
#endif

class KPlayerChat
{
public:

#ifdef _SERVER
	int				m_nAddFriendList[CHAT_APPLY_ADD_FRIEND_LIST];
	int				m_nListPos;
	int				m_nSyncState;
	CChatFriend		*m_pStateSendPos;
	int				m_nTimer;
#endif

#ifndef _SERVER
	KList			m_cApplyAddList;
	int				m_nCurChannel;
	BOOL			m_btIniLoadFlag;
	KIniFile		m_cTeamInfo;
	int				m_nLoginGetFirstOneFriendFlag;
	KCHAT_RECORDER	m_cRecorder;
#endif

	DWORD			m_dwTakeChannel;		
	DWORD			m_dwTargetID;					
	int				m_nTargetIdx;					
	CChatFriendTeam	m_cFriendTeam[MAX_FRIEND_TEAM];

public:
	void			Release();
	BOOL			CheckExist(DWORD dwID);
	BOOL			CheckExist(char *lpszName);
	BOOL			GetName(DWORD dwID, char *lpszName);
	CChatFriend*	GetFriendNode(DWORD dwID);
	int				GetTeamNo(DWORD dwID);
	void			SetTakeChannel(DWORD dwChannel);
	BOOL			CheckTalkChannel(int nChannel);

#ifdef _SERVER
	void			ServerSendChat(int nPlayerIdx, BYTE* pProtocol);
	void			DataBaseAddOne(int nSelfIdx, DWORD dwID, char *lpszName);
	void			AddFriendData(int nSelfIdx, int nDestIdx);	
	void			ResendOneFriendData(DWORD dwID, int nPlayerIdx);
	BOOL			SyncFriendData(int nPlayerIdx, unsigned int &nParam);
	void			StartSendFriendData();							
	void			DeleteFriend(DWORD dwID, DWORD dwSelfID);		
	void			DeleteFriendData(DWORD dwID, int nSelfIndex);
	void			RedeleteFriend(DWORD dwID, int nSelfIndex);	
	void			OffLine(DWORD dwSelfID);				
	void			GetMsgOffLine(DWORD dwID, int nSelfIdx);
	void			TimerAdd();							
	static	void	SendSystemInfo(int nType, int nTargetIdx, char *lpszSendName, char *lpszSentence, int nSentenceLength);
	static	void	SendGlobalSystemInfo(char *lpszSendName, char *lpszSentence, int nSentenceLength);
	typedef std::list<std::string>	STRINGLIST;
	static	void	MakeBrother(const STRINGLIST& brothers);
	static	void	MakeEnemy(char* szPlayer, char* szEnemy);
	static	void	SendInfoToGM(char *lpszAccName, char *lpszRoleName, char *lpszSentence, int nSentenceLength);
	static	void	SendInfoToIP(DWORD nIP, DWORD nID, char *lpszAccName, char *lpszRoleName, char *lpszSentence, int nSentenceLength);
	
	static	void	SendInfoToFaction(int nFactionID, char *lpszSendName, char *lpszSentence, int nSentenceLength);
	
#endif

#ifndef _SERVER
	void			SetCurChannel(int nChannelNo, DWORD dwID = 0, int nIdx = 0);
	void			AddChannel(int nChannelNo);					
	void			SubChannel(int nChannelNo);				
	void			SendTakeChannel();							

	void			SendSentence(KUiMsgParam *pMsg, char *lpszSentence);
	void			QQSendSentence(DWORD dwID, int nIdx, KUiMsgParam *pMsg, char *lpszSentence);

	void			GetChat(PLAYER_SEND_CHAT_SYNC *pChat);	

	void			ApplyAddFriend(DWORD dwNpcID, char *lpszSentence = NULL);
	void			ApplyDeleteFriend(DWORD dwID, int nTeamNo, char *lpszSelfName);

	BOOL			GetApplyOneInfo(int *lpnPlayerIdx, char *lpszName, char *lpszInfo);

	void			SaveTeamInfo(char *lpszName);				
	int				CheckTeamNo(char *lpszName, char *lpszGetName);	
	void			LoadTeamName(char *lpszPlayerName);			

	void			AddOne(DWORD dwID, int nPlayerIdx, char *lpszName, int nTeamNo);
	BOOL			CheckIsFriend(int nPlayerIdx);			
	int				AddTeam(char *lpszPlayerName, char *lpszTeamName);
	BOOL			RenameTeam(int nTeamNo, char *lpszTeamName, char *lpszPlayerName);
	BOOL			DeleteTeam(int nTeamNo, char *lpszPlayerName);	
	BOOL			OneFriendChangeTeam(DWORD dwID, int nSrcTeam, int nDestTeam);

	void			GetMsgOffLine(DWORD dwID);		

	BOOL			CheckTakeChannel(int nChannelNo);
	void			SaveTakeChannelInfo(char *lpszName);
	void			LoadTakeChannelInfo(char *lpszName);

	int				SendSelectChannelList(KUiChatChannel *pList, int nCount);
	int				SendTakeChannelList(KUiChatChannel *pList, int nCount);
	BOOL			GetCurChannel(KUiChatChannel *pList);	
	int				IGetTeamInfo(int nTeamNo, KUiChatGroupInfo *psTeamInfo);
	int				IGetTeamFriendInfo(int nTeamNo, KUiPlayerItem *pList);	
#endif

};

#endif

