#pragma once
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndLabeledButton.h"

int VerticalSplitTwoWindow(KWndWindow* pLeft, KWndWindow* pRight, int nAt);

struct KUiMsgParam;

#define MAX_CHANNELRESOURCE 10

struct KChannelResourceInfo
{
	char cTitle[32];
	char cShortTitle[MAX_CHANNELRESOURCE][32];
	KRColor uTextColor;
	KRColor uTextBorderColor;
	KRColor uTextColorOrther;
	KRColor uTextBorderColorOrther;
	WORD nMenuPicIndex;
	WORD nMenuPicHeight;
	char cMenuText[32];
	WORD nMenuDeactivatePicIndex;
	WORD nMenuDeactivatePicHeight;
	char cMenuDeactivateText[32];
	KRColor uMenuBkColor;
	WORD nTextPicIndex;
	char cFormatName[32];
	int nNeverClose;
	char szSoundFileName[80];
	unsigned int uSendMsgInterval;
	unsigned int uSendMsgNum;
};

struct KChannelActivateInfo
{
	DWORD nChannelID;
	BYTE cost;
	bool bSubscribe;
	int ResourceIndex;	
	unsigned int uLastSendMsgTime;	
	unsigned int uLeftSendMsgNum;	
	unsigned int uBufferOffset;	
	char Buffer[1024 * 10];
};

struct KMSN_ChannelInfo
{
	KRColor uTextColorSelf;
	KRColor uTextBorderColorSelf;
	WORD nSelfTextPicIndex;
	KRColor uTextBKColorSelf;
	
	KRColor uTextFriendColor;
	KRColor uTextBorderFriendColor;
	WORD nFriendMenuPicIndex;
	WORD nFriendMenuPicHeight;
	KRColor uFriendMenuBkColor;
	WORD nFriendTextPicIndex;
	char szFriendSoundFileName[80];

	KRColor uTextColorUnknown;
	KRColor uTextBorderColorUnknown;
	WORD nStrangerMenuPicIndex;
	WORD nStrangerMenuPicHeight;
	KRColor uStrangerMenuBkColor;
	WORD nStrangerTextPicIndex;
};

class KSysMsgCentrePad : public KWndWindow
{
public:
	KWndButton				m_OpenSysButton;
	KWndButton				m_UpButton;
	KWndButton				m_DownButton;
	KWndMessageListBox		m_SysRoom;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void ScrollBottom();
};

class KUiMsgCentrePad : public KWndWindow
{
public:

	static KUiMsgCentrePad* OpenWindow();					
	static void				CloseWindow(bool bDestroy);		
	static void				Clear();
	static void				LoadScheme(const char* pScheme);
	static void				DefaultScheme(const char* pScheme);
	static void				HideAllMessage();
	static void				ShowAllMessage();
	
	static void				SystemMessageArrival(const char* pMsgBuff, unsigned short nMsgLength);

	static int				NewChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, 
														const char* pChatItemName = NULL, const char* pChatItemInfo = NULL);		// Modify by lzlsy301
	static void				NewMSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, 
													const char* pChatItemName = NULL, const char* pChatItemInfo = NULL);		//
	static void				OpenChannel(char* channelName, DWORD nChannelID, BYTE cost);
	static void				ShowSomeoneMessage(char* szSourceName, const char* pMsgBuff, unsigned short nMsgLength);
	enum  SelfChannel
	{
		ch_Team = 0,
		ch_Faction,
		ch_Tong,
		ch_Screen,
		ch_GM,
		ch_Song,
		ch_Jin,
	};
	static void				CloseSelfChannel(SelfChannel type);

	static void				QueryAllChannel();
	static void				ReplaceChannelName(char* szDest, size_t nDestSize, char* szSrc);
	static bool				GetChannelMenuinfo(int nChannelIndex, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL, char* pszMenuText = NULL, short* pnCheckPicIndex = NULL);
	static bool				GetPeopleMenuinfo(char* szDest, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL);
	static bool				ReplaceSpecialField(char* szDest, char* szSrc);
	static DWORD			GetChannelID(int nChannelIndex);
	static char*			GetChannelTitle(int nChannelIndex);
	static DWORD			GetChannelCount();
	static int				GetChannelIndex(char* channelName);
	static int				GetChannelIndex(DWORD dwID);
	static int				ReleaseActivateChannelAll();
	static bool				GetChannelSubscribe(int nChannelIndex);
	static bool				IsChannelType(int nChannelIndex, SelfChannel type);
	static int				CheckChannel(int nChannelIndex, bool b);
	static KUiMsgCentrePad* GetSelf()	{return m_pSelf;}
	static void SetFontSize(int nFontSize);
	static int GetFontSize();
	static int SetChannelTextColor(char* cTitle, DWORD uTextColor, DWORD uTextBorderColor);
	static char* GetChannelTextColor(int nIndex, DWORD& uTextColor, DWORD& uTextBorderColor);
	static void SetMSNTextColor(int nType, DWORD uTextColor, DWORD uTextBorderColor);
	static void GetMSNTextColor(int nType, DWORD& uTextColor, DWORD& uTextBorderColor);
	static BYTE	GetChannelCost(DWORD dwID);
	static int	PushChannelData(DWORD dwID, const char* Buffer, int nLen);
	static int	GetChannelData(DWORD& dwID, BYTE& cost, char*& Buffer, int& nLen);
	static int	PopChannelData(DWORD dwID);
	static void	SetReply(const char* Buff, int nLen);
	static void ClearReply();
	
	void	Breathe();	

	static void AddAutoReplyMsg(char* lpszMsg);
	
private:

	KUiMsgCentrePad();
	~KUiMsgCentrePad()
	{
		ReleaseActivateChannelAll();
	}
	void	Initialize();							
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	int		ChanageHeight(int nOffsetY, bool bAtTop);
	void	LoadScheme(KIniFile* pIni);
	int		PtInWindow(int x, int y);
	void	SetAutoDelMsgInterval(unsigned int uInterval = 0);	
	void	DragWndSize(int nMoveOffset);
	void	DragWndPos(int nMoveOffset);
	void	PaintWindow();							

	int		FindActivateChannelResourceIndex(char* cTitle);
	int		FindActivateChannelIndex(char* cTitle);
	int		FindActivateChannelIndex(DWORD nChannelID);
	BYTE	FindActivateChannelCost(DWORD nChannelID);
	int		FindChannelResourceIndex(char* cTitle);
	int		FindActivateChannelIndexByKey(char* cKey);
	int		IsNeverCloseActivateChannel(int nChannelIndex);
	void	ChannelMessageArrival(int nChannelIndex, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, KWndMessageListBox* pM, bool bName,
									const char* pChatItemName = NULL, const char* pChatItemInfo = NULL);		//
	void	MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, 
								const char* pChatItemName = NULL, const char* pChatItemInfo = NULL);		//
	void	ShowMSNMessage(char* szName, const char* pMsgBuff, unsigned short nMsgLength, KRColor uColor, KRColor uBKColor, KRColor uBorderColor, 
							WORD nPic, const char* pChatItemName = NULL, const char* pChatItemInfo = NULL);		//
	void	SendQueryChannelID(int nChannelResourceIndex);
	void	SendChannelSubscribe(int nChannelIndex, bool b);
	int		AddActivateChannel(const KChannelActivateInfo& Item);	
	int		ReleaseActivateChannel(int nChannelIndex);	
	void	CloseActivateChannel(int nChannelIndex);	

	void	PopupChannelMenu(int x, int y);
	
	int		FilterTextColor(char* pMsgBuff, unsigned short nMsgLength, const KRColor& uColor);
	
	int		GetMessageSendName(KWndWindow* pWnd, int nIndex, char* szCurText);
	int		GetMessageSendName(KWndWindow* pWnd, int x, int y, char* szCurText);

	void	OnSendAutoReplyMsg(char* lpDestName);

private:
	static KUiMsgCentrePad*	m_pSelf;
	enum CHAT_ROOM 
	{
		ROOM_ALL = 0,
		ROOM_PRIVACY,
		ROOM_TEAM,
		ROOM_TONG,
		ROOM_FACTION,
		ROOM_ORTHER,
		ROOM_NUM,
	};
	KWndImage			m_BorderImg;
	KWndImage			m_SizeBtn;
	KWndImage			m_MoveImg;
	KWndButton          m_BgShadowBtn;
	KSysMsgCentrePad	m_Sys;
	KWndLabeledButton	m_TabBtn[ROOM_NUM];
	short				m_nMinTopPos, m_nMaxBottomPos;
	bool				m_bSizingWnd;
	bool				m_bDockLeftSide;

	short				m_nSizeBtnDragOffsetY;
	
	bool				m_bSizeUp;
	char				m_szAutoReplyMsg[512];

#define SECOND_AUTODELMSG 20000
	unsigned int	m_uAutoDelMsgInterval;	
	unsigned int	m_uLastDelMsgTime;		
	KScrollMessageListBox m_ChatRoom[ROOM_NUM];


	KChannelResourceInfo m_ChannelsResource[MAX_CHANNELRESOURCE];
	int m_nChannelsResource;
	char m_DefaultChannelSendName[32];
	int m_nDefaultChannelResource;
	KChannelActivateInfo* m_pActivateChannel;
	int m_nActivateChannels;
	KMSN_ChannelInfo m_MSNInfo;
	short m_nCheckOnPicIndex;
	short m_nCheckOffPicIndex;

	KRColor m_NameColor;
	KRColor m_BorderNameColor;
	char m_szReply[128];

	static int ms_DefaultHeight, ms_DefaultWidth;
};
