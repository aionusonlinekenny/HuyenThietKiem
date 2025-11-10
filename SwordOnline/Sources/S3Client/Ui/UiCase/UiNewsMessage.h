#ifndef _UINEWSMESSAGE_H
#define _UINEWSMESSAGE_H    1

#include "KEngine.h"
#include "KIniFile.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../../../Core/Src/GameDataDef.h"

//======================
//
//======================
struct KNewsMessageNode : public KNewsMessage
{
	unsigned int		uTime;
		
	union
	{
		unsigned int	uShowTimes;		
		unsigned int	uStartTime;		
		unsigned int	uLastShowTime;
	};
	KNewsMessageNode*	pNext;
};

class KUiNewsMessage : protected KWndShadow
{
public:
	static KUiNewsMessage*	OpenWindow();						
	static KUiNewsMessage*  GetIfVisible();                     
	static void				CloseWindow(BOOL bDestory = FALSE);	
	static void				LoadScheme(const char* pszScheme);	
	static void				MessageArrival(KNewsMessage* pMsg, SYSTEMTIME* pTime);
	static void				EnableIdleMsg(BOOL bEnable);

private:
    static KUiNewsMessage *m_pSelf;
	static BOOL				ms_bEnableIdleMsg;
private:
	KUiNewsMessage();
	~KUiNewsMessage() {}
	void	Initialize();
	void	LoadScheme(KIniFile* pIni);			
	virtual void	Breathe();
	virtual int		PtInWindow(short x, short y);
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
