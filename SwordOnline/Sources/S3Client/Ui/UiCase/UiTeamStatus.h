#ifndef __UiTeamStatus_H__
#define __UiTeamStatus_H__

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndImage2.h"
#include "../Elem/WndText.h"
#include "../Elem/WndList.h"
#include "../../../Core/src/GameDataDef.h"

#define			TEAM_NUM_MEMBER		8
class KUiTeamStatus : protected KWndShowAnimate
{
public:
	static KUiTeamStatus*	GetIfVisible();
	static KUiTeamStatus*	OpenWindow();
	static void				CloseWindow(bool bDestroy = true);

private:
	KUiTeamStatus();
	~KUiTeamStatus() {}
	void	Initialize();					
	void	LoadScheme(const char* pScheme);
	void	Breathe();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	Clear(bool bData = false);
	void	UpdateData(KUiPlayerTeam* pInfo);
	void	UpdateNewState(int nCount);
	void	UpdateTime();
	void	UpdateToolTip(int nSlot, bool bBuff = true);
	void	ConvertInfo(char* pszRet, int nTime, bool bFull = false);
private:
	static KUiTeamStatus* m_pSelf;
private:

	KWndText32		m_LeaderAbility;
	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersListMem;
	KWndImage		m_TeamMem[TEAM_NUM_MEMBER];
	KWndList		m_TeamMember;
};

#endif // __UiTeamStatus_H__