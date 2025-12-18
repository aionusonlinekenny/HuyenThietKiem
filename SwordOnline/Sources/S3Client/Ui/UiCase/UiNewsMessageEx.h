#ifndef _UiNewsMessageEx_H
#define _UiNewsMessageEx_H    1

#include "KIniFile.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../../../Core/Src/GameDataDef.h"
#include "UiNewsMessage.h"

class KUiNewsMessageEx : protected KWndShadow
{
public:
	static KUiNewsMessageEx*	OpenWindow();						
	static KUiNewsMessageEx*  GetIfVisible();                     
	static void				CloseWindow(BOOL bDestory = FALSE);	
	static void				LoadScheme(const char* pszScheme);	
	static void				MessageArrival(KNewsMessage* pMsg, SYSTEMTIME* pTime);
	static void				EnableIdleMsg(BOOL bEnable);

private:
    static KUiNewsMessageEx *m_pSelf;
	static BOOL				ms_bEnableIdleMsg;
private:
	KUiNewsMessageEx();
	~KUiNewsMessageEx() {}
	void	Initialize();
	void	LoadScheme(KIniFile* pIni);			
	virtual void	Breathe();				
	virtual int		PtInWindow(int x, int y);	
	virtual void	PaintWindow();

	bool	AddMessage(KNewsMessage* pMsg, unsigned int uTime, unsigned int uTimeParam = 0);
												
	KNewsMessageNode* 	SeparateMsg();			
	void	AddToTail(KNewsMessageNode* pNode);	
	bool	PickAMessage();						
	void	Reset();							
	bool	ScrollMsg();						
	void	PickFromIni();						
	void	Clear();							
	void	ConvertMsg();						
	bool	MakeCountingMsg();					

private:

	KNewsMessageNode*	m_pHead;			
	KNewsMessageNode*	m_pHandling;	

	KIniFile		m_IniFile;				

	int				m_nIndentH;				
	int				m_nIndentV;				
	int				m_nFontSize;		
	unsigned int	m_uTextColor;			
	unsigned int	m_uTextBorderColor;		
	int				m_nVisionWidth;			
	int				m_nCharasVisibleLimitNum;
	int				m_nFontHalfWidth[2];	

	unsigned int	m_uMaxIdleTime;			
	unsigned int	m_uShowInterval;		
	unsigned int	m_uScrollInterval;		

	unsigned int	m_uLastShowTime;		
	KNewsMessage	m_CurrentMsg;			
	int				m_nLineLen;				

	unsigned int	m_uLastScrollTime;		
	bool	m_bJustIncoming;				
	int		m_nCharIndex;					
	int		m_nTextPosX;				
	int		m_nHalfIndex;					
	int		m_nInsertPlace;					
};

#endif
