
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndValueImage.h"
#include "../Elem/WndObjContainer.h"
#include "../Elem/WndLabeledButton.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Core/src/GameDataDef.h"
#include "UiMsgCentrePad.h"
#include "../Elem/ComWindow.h"
#include "../Elem/WndToolBar.h"
#include "../Elem/WndImage.h"

#define		defHIDE_CHATITEM_BUTTON		0,0
#define		defMAX_CHATITEM_BUTTON		120

//===========================================================================

class GameWorld_DateTime : public KWndLabeledButton
{
public:
	DECLARE_COMCLASS(GameWorld_DateTime)
	GameWorld_DateTime();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	void            UpdateData();

private:
	int             m_nSmoothPing, m_nCrowdPing, m_nBlockPing;
	bool			m_bTrueWorld;
	bool            m_bNetStatus;
	bool			m_bLogo;
	char            m_szLogo[32], m_szSmoothMsg[32], m_szCrowdMsg[32], m_szBlockMsg[32];
	unsigned int	m_uLastSwitchTime, m_uDefaultColor, m_uSmoothColor, m_uCrowdColor, m_uBlockColor;

private:
	void OnButtonClick();
	void UpdateNetStatus();
};

//===========================================================================

class KImmediaItem : public KWndObjectBox
{
public:
	int m_nIndex;
	DWORD m_TextColor;
	KImmediaItem() :m_nIndex(-1), m_TextColor(0xFFFFFF00){}
	void PaintWindow();
	int	Init(KIniFile* pIniFile, const char* pSection);
};

//===========================================================================

struct KUiDraggedObject;
struct KPopupMenuData;
class KIniFile;

//===========================================================================

class KUiPlayerBar : protected KWndImage
{
public:
	static KUiPlayerBar* OpenWindow();					
	static KUiPlayerBar* GetIfVisible();				
	static void			CloseWindow(bool bDestroy);		
	static void			LoadScheme(const char* pScheme);

	static void			InputNameMsg(char bChannel, const char* szName, bool bFocus);
	static int			FindRecentPlayer(const char* szName);
	static int          AddRecentPlayer(const char* szName);
	static char*        GetRecentPlayerName(int nIndex);

	static const char*	SelfName()
	{
		if (m_pSelf)
			return m_pSelf->m_szSelfName;
		return NULL;
	}
	static BOOL	IsSelfName(char* szName)
	{
		if (m_pSelf && szName)
		{
			return (_strcmpi(m_pSelf->m_szSelfName, szName) == 0);
		}
		return FALSE;
	}
	static void	OnUseItem(int nIndex);				
	static BOOL	IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID);				
	static void	OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen);				
	static void	OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen);				

	void	UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId);	
	void	UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId);	

	static	bool		GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp);

	static BOOL			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);

	static void         SetCurrentChannel(int nIndex);
	static void         SwitchChannel(BOOL bUp = FALSE);
	                                                                    
    static int          GetCurChannel();  
   	
	static void			SetChatItemInfo(KUiChatItemParam cItem, unsigned int uId);
	static void			SetChatItemInfo(short nBtnNo, char* pszName, char* pszInfo);
	static void			SetChatItemPosition(short nBtnNo, short nLeft, short nTop, bool bShow);
	static void			ClearChatItem();
	static void			EncodeChatItemInfo(const char* pMsgBuff, unsigned int uMsgLengh, char* pszName, char* pszInfo);

	static	void		PopupPKMenu(short nX, short nY);
	static	void		MyMessage(char* strMessage);//AutoAI by kinnox;
	
private:
	KUiPlayerBar();
	~KUiPlayerBar();
	void	Breathe();									
	void	Initialize();								
	void	PaintWindow();
	void	StatusCity();

	void	UpdateData();								
	void	UpdateXXXNumber(int& nMana, int& nFullMana);							
	void	UpdateRuntimeAttribute(int& nMoney, int& nLevel);

	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	BOOL	CanDragObj(unsigned int uId);
	void	LoadScheme(KIniFile* pIni, const char* pScheme);

	void	OnSend(BOOL bDirectSend);
	void	OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen);				
	void	PopupChannelMenu(int x, int y);
	void	PopupPhraseMenu(int x, int y, bool bFirstItem);
	void	InputRecentMsg(bool bPrior);

	int GetChannelIndex(const char* pTitle);
	void InputCurrentChannel(int nIndex, bool bFocus);
	void ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc);
	int IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana);
	void	OnChoosePKMenu(BYTE btChoose);

private:
	static KUiPlayerBar* m_pSelf;
private:
	KWndButton		m_Face, m_Friend, m_Options;
	KWndToolBar		m_ToolBox;						
	KWndButton		m_Market;											
//	KWndText32		m_LevelText;
	KWndImage		m_ImmediaExtend;

	GameWorld_DateTime		m_DateTime;		
#define	UPB_IMMEDIA_ITEM_COUNT	9
	KImmediaItem	m_ImmediaItem[UPB_IMMEDIA_ITEM_COUNT];
	KWndObjectBox	m_ImmediaSkill[2];
	
	KWndButton	m_SendBtn;
	KWndEdit80	m_InputEdit;	
	KWndPureTextBtn	m_ChannelSwitchBtn;	
	KWndButton	m_ChannelOpenBtn;
	
	KWndPureTextBtn	m_ChatItemBtn[defMAX_CHATITEM_BUTTON];
	KWndButton	m_InputBack;
	

	char		m_cPreMsgCounter;
	char		m_cLatestMsgIndex;
#define	MAX_RECENT_MSG_COUNT	8
	char		m_RecentMsg[MAX_RECENT_MSG_COUNT][512];
	int m_nCurChannel;
#define	MAX_RECENTPLAYER_COUNT	10
	char m_RecentPlayerName[MAX_RECENTPLAYER_COUNT][32];
	int m_nRecentPlayerName;

	char			m_szSelfName[32];

	int				m_nExperienceFull;
	int				m_nCurLevelExperience;
	int				m_nExperience;
	
	char			m_szChatItemName[80];
	char			m_szChatItemInfo[MAX_CHATITEM_INFO_LENGTH];
	KWndButton		m_HideWindow; // an hien windows by kinnox;
	KWndButton 		m_HideMiniBuff;// MiniBuff by kinnox;
	KWndButton 		m_ShowMiniBuff;// MiniBuff by kinnox;
	//
	#define MAX_NUM_BUTTERFLY 3
	KWndButton		m_NoteQuest;
	unsigned int	m_uAdviceIntervalMin[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMax[MAX_NUM_BUTTERFLY];
	unsigned int	m_uInterval[MAX_NUM_BUTTERFLY];
	unsigned int	m_uLastSleepTime[MAX_NUM_BUTTERFLY];

	KWndButton		m_HelpMission;
	unsigned int	m_uAdviceIntervalMin_1[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMax_1[MAX_NUM_BUTTERFLY];
	unsigned int	m_uInterval_1[MAX_NUM_BUTTERFLY];
	unsigned int	m_uLastSleepTime_1[MAX_NUM_BUTTERFLY];
	//
	
};
