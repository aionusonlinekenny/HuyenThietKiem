
#pragma once

#include "../Elem/WndGameSpace.h"
#include "../Elem/WndWindow.h"

class KUiGameSpace : public KWndWindow
{
public:
	int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void PaintWindow();
	void OnMouseMoveCursor(int x,int y);
	KUiPlayerItem m_SelectPlayer;
};

extern KUiGameSpace	g_WndGameSpace;

enum
{
	ACTION_CHAT = 0,					
	ACTION_MAKEFRIEND,					
	ACTION_TRADE,	
	ACTION_JOINTEAM,
	ACTION_INVITETEAM,
	ACTION_FOLLOW,
	ACTION_REVENGE,
	ACTION_VIEWITEM,
	ACTION_BLACKLIST,
	ACTION_TONG,
	ACTION_RQTONG,
	ACTION_OTT,
};

void ProcessPeople(KUiPlayerItem* pDest, int nAction);
void ProcessEmote(char* szDest, char *szDestChannel = "", int nEmoteIndex = 0);

void PopUpContextPeopleMenu(const KUiPlayerItem& SelectPlayer, int x, int y);
void PopUpContextEmoteMenu(char* szDest, int x, int y);