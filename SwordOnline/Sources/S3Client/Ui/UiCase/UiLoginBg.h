

#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndText.h"

class KUiLoginBackGround : protected KWndImage
{
public:
	static KUiLoginBackGround*	OpenWindow(const char* pszConfig);	
	static void					CloseWindow(bool bDestroy);
	static void					SetConfig(const char* pszConfig);
private:
	KUiLoginBackGround() {}
	~KUiLoginBackGround() {}
	void	Initialize();					
	void	LoadScheme(KIniFile* pIni);		
	void	Breathe();
private:
	static 	KUiLoginBackGround* m_pSelf;
#define	MAX_NUM_BUTTERFLY	3
	KWndImage		m_Butterflys[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMin[MAX_NUM_BUTTERFLY];
	unsigned int	m_uAdviceIntervalMax[MAX_NUM_BUTTERFLY];
	unsigned int	m_uInterval[MAX_NUM_BUTTERFLY];
	unsigned int	m_uLastSleepTime[MAX_NUM_BUTTERFLY];
	char			m_szConfig[32];
};
