#pragma once

enum UISYS_STATUS
{
	UIS_S_IDLE,
	UIS_S_TRADE_PLAYER,
	UIS_S_TRADE_NPC,	
	UIS_S_TRADE_SALE,
	UIS_S_TRADE_BUY,
	UIS_S_TRADE_REPAIR,
	UIS_S_BIND_ITEM,
	UIS_S_UNBIND_ITEM,
	UIS_S_MARKPRICE_ITEM,
	UIS_S_PLAYER_SHOP,
};

enum UISYS_OPERATION
{
	UIS_O_MOVE_ITEM,
	UIS_O_TRADE_ITEM,
	UIS_O_USE_ITEM,
};

#define APP_START	"Start()"
#define APP_EXIT	"Exit()"
#define APP_STARTGAME	"StartGame(\"%s\")"
#define APP_EXITGAME	"ExitGame()"
#define APP_SAY		"Say(\"%s\", \"%s\")"
#define APP_CHAT	"Chat(\"%s\", \"%s\")"
#define APP_CREATEUNIT	"CreateUnit(\"%s\")"
#define APP_DELETEUNIT	"DeleteUnit(\"%s\")"
#define APP_CREATEUNITGROUP	"CreateUnitGroup(\"%s\", \"%s\")"
#define APP_RENAMEUNITROUP	"RenameUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITROUP	"DeleteUnitGroup(\"%s\", \"%s\")"
#define APP_MOVEUNITROUP	"MoveUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_CREATEUNITMEMBER	"CreateUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITMEMBER	"DeleteUnitMember(\"%s\", \"%s\")"
#define APP_MOVEUNITMEMBER	"MoveUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_SETUNITMEMBERSTATUS	"SetUnitMemberStatus(\"%s\", \"%s\", \"%s\")"

class KIniFile;

extern const char*	$Main;

#pragma warning(disable:4786)
#include "map"
#include "string"

class KUiBase
{
public:
	KUiBase();
	~KUiBase();
	int		Init();											
	void	Exit();											

	void	SetUserAccount(char* pszId, char* pszRole);
	int		GetUserPrivateDataFolder(char* pBuffer, int nSize);	
	const char*	GetUserTempDataFolder();
	void	CleanTempDataFolder();
	void	CleanPrivateDataFolder();
	void	DeletePrivateDataFile(const char* pszFileName);

	int		LoadScheme(const char* pSchemeName);
	int		SchemeCount();
	int		GetScheme(int nIndex, char* pName, char* pPath);
	int		GetCurSchemePath(char* pBuffer, int nSize);
	
	KIniFile*	GetCommSettingFile();
	void		CloseCommSettingFile(bool bSave);
	KIniFile*	GetPrivateSettingFile();
	void		ClosePrivateSettingFile(bool bSave);
	KIniFile*	GetCommConfigFile();
	void		CloseCommConfigFile();
	//
	KIniFile*	GetAutoSettingFile();
	void 		CloseAutoSettingFile(bool bSave);
	//
	UISYS_STATUS GetStatus() { return m_Status; }
	void		 SetStatus(UISYS_STATUS eStatus);
	void		 SetCurrentCursor();
	int			 IsOperationEnable(UISYS_OPERATION uOper);

	void		LoadPrivateConfig();
	int			SavePrivateConfig();

	void		RegisterEvent(const char* szName, HANDLE hWnd);
	void		UnregisterEvent(const char* szName, HANDLE hWnd);
	int			NotifyEvent(const char* szEvent);
	int			NotifyOneEvent(const char* szName, const char* szEvent);

private:
	void	LoadSchemeForEachWnd();
	int		GetSchemePath(const char* pScheme);
	void	ClearFolder(const char* pszFolder, bool bDeleteFolder);

private:
	UISYS_STATUS	m_Status;

	char		m_CurScheme[32];
	char		m_CurSchemePath[40];
	char		m_UserAccountId[32];

	KIniFile*	m_pUiCommSettingFile;
	KIniFile*	m_pUiPrivateSettingFile;
	KIniFile*	m_pUiCommConfigFile;
	KIniFile*	m_pUiAutoSettingFile;
	typedef std::map<std::string, HANDLE> EVENTWNDLIST;
	EVENTWNDLIST m_EventWndList;
};

extern KUiBase	g_UiBase;