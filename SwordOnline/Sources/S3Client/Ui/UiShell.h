#pragma once
int			UiGetFrameRate();
void		UiPostQuitMsg();						
bool		UiIsAlreadyQuit();						
int			UiInit();								
int			UiStart();								
void		UiExit();								
void		UiPaint(int nGameLoop);					
void		UiSetScreenSize(int nWidth, int nHeight);
int			UiHeartBeat();							
void		UiProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
void		UiSetGraphicDevice(void* pDevice);

void		UiStartGame();							
void		UiOnGameServerConnected();				
void		UiOnGameServerStartSyncEnd();			
void		UiResumeGame();							
void		UiEndGame();							
void		UiSetSwitchSceneStatus(int bSwitching);

#include "Elem/ComWindow.h"
#include "Elem/WndButton.h"

class Player_Life : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Life)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

class Player_Mana : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Mana)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

class Player_Stamina : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Stamina)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

class Player_Exp : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Exp)
	void UpdateData();
	void OnButtonClick();
	int  GetToolTipInfo(char* szTip, int nMax);
	static bool m_bText;
};

class Player_Level : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Level)
	void UpdateData();
};

class Player_WorldSort : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_WorldSort)
	void OnButtonClick();
	void UpdateData();
};

class Player_Status : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Status)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_Items : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Items)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_ItemEx : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_ItemEx)
	void OnButtonClick();
};

class Player_Skills : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Skills)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_Team : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Team)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_Faction : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Faction)
	void OnButtonClick();
	const char*	GetShortKey();
};


class Player_Sit : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Sit)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_Run : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Run)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_Horse : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Horse)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_Exchange : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Exchange)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_PK : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_PK)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_Task : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Task)
	void OnButtonClick();
};

class Player_AutoPlay : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_AutoPlay)
	void OnButtonClick();
};

class Player_Lock : public KWndButton //LockPlayer by kinnox;
{
public:
	DECLARE_COMCLASS(Player_Lock)
	void OnButtonClick();
};