
#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndText.h"
#include "../elem/wndbutton.h"
#include "../elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../../../core/src/gamedatadef.h"
#include "../elem/wndlabeledbutton.h"



class KUiTeamManage : public KWndShowAnimate
{
public:
	static KUiTeamManage* OpenWindow();	
	static KUiTeamManage* GetIfVisible();	
	static void			  CloseWindow();	
	static void			  LoadScheme(const char* pScheme);		
//	static void			  UpdateNearbyTeams(KUiTeamItem* pList, int nCount); 
	void				  UpdateData(KUiPlayerTeam* pInfo);

private:
	KUiTeamManage();
	~KUiTeamManage() {}
	void	Initialize();							
	void	LoadScheme(class KIniFile* pSetting);	
protected:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	//void	OnApplyJoin();					
	void	OnKick();						
	void	OnAppoint();					
	void	OnInvite();						
	void	OnLeave();						
	void	OnDismiss();					
	void	OnRefresh();					
	void	OnSelTeamMember(int nItem);
	void	OnSelNearby(int nItem);
	void	Clear();
	//void	SwitchNearbyMode(bool bNearbyPlayer);
private:
	static KUiTeamManage* m_pSelf;

	KWndText32		m_LeaderAbility;			
	KWndList		m_TeamMember;			
	KWndList		m_NearbyList;			
	KWndScrollBar	m_MemberScroll;			
	KWndScrollBar	m_NearbyScroll;			

//	KWndButton		m_NearbyPlayerTab;		
//	KWndButton		m_NearbyTeamTab;		

	KWndLabeledButton		m_btnInvite;			
	KWndLabeledButton		m_btnKick;				
//	KWndButton		m_btnApplyJoin;			

	KWndLabeledButton		m_btnAppoint;			
	KWndLabeledButton		m_btnLeave, m_btnDismiss;
	KWndLabeledButton		m_btnCloseTeam;			
	KWndLabeledButton		m_btnRefresh;			
	KWndButton		m_btnCancel;		

	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
//	KUiTeamItem*	m_pNearbyTeamsList;
};

//----------------------------------------------------------------------------------------------
//------------------------------ Ui Team Manage Bar --------------------------------------------
//----------------------------------------------------------------------------------------------

class KUiTeamManageBar : protected KWndShowAnimate {
public:
    static KUiTeamManageBar *OpenWindow();
    static KUiTeamManageBar *GetIfVisible();
    static void CloseWindow(BOOL bDestroy);
    static void LoadScheme(const char *pScheme);
    void UpdateData(KUiPlayerTeam *pInfo);

private:
    KUiTeamManageBar();

    ~KUiTeamManageBar() {}
    void Breathe(); 
    void Initialize();
    void LoadScheme(class KIniFile *pSetting);
    void DisplayInfo(int index);
    void OnHideShow();
    void HideInfo(int index);
    void ShowInfo(int index);

    void PopupTeamBarPlayer(int index);
    void ActionPopupTeamBarPlayer(int indexPlayer, int indexMenu);

protected:
    int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
    
    void ClearTeamManageBar();

public:
    bool m_bShowUI;

private:
    static KUiTeamManageBar *m_pSelf;
    int m_nGameLoop;
    KUiPlayerTeam m_Info;
    KUiPlayerItem *m_pPlayersList;

    KWndImage m_CaptainFlag;
    KWndButton m_ButtonShowHide;

        KWndButton m_Box1;
    KWndText32 m_RoleName1;
    KWndButton m_IconFaction1;
    KWndImageTextButton m_Life1;
    KWndText32 m_Level1;
    KWndText32 m_Map1;

        KWndButton m_Box2;
    KWndText32 m_RoleName2;
    KWndButton m_IconFaction2;
    KWndImageTextButton m_Life2;
    KWndText32 m_Level2;
    KWndText32 m_Map2;

        KWndButton m_Box3;
    KWndText32 m_RoleName3;
    KWndButton m_IconFaction3;
    KWndImageTextButton m_Life3;
    KWndText32 m_Level3;
    KWndText32 m_Map3;

        KWndButton m_Box4;
    KWndText32 m_RoleName4;
    KWndButton m_IconFaction4;
    KWndImageTextButton m_Life4;
    KWndText32 m_Level4;
    KWndText32 m_Map4;

        KWndButton m_Box5;
    KWndText32 m_RoleName5;
    KWndButton m_IconFaction5;
    KWndImageTextButton m_Life5;
    KWndText32 m_Level5;
    KWndText32 m_Map5;

        KWndButton m_Box6;
    KWndText32 m_RoleName6;
    KWndButton m_IconFaction6;
    KWndImageTextButton m_Life6;
    KWndText32 m_Level6;
    KWndText32 m_Map6;
    
        KWndButton m_Box7;
    KWndText32 m_RoleName7;
    KWndButton m_IconFaction7;
    KWndImageTextButton m_Life7;
    KWndText32 m_Level7;
    KWndText32 m_Map7;

};
