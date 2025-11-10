
#ifndef KPLAYERPK_H
#define KPLAYERPK_H

#define		PK_ANMITY_TIME					200	
#define		PK_ANMITY_CLOSE_TIME			10800

enum
{
	enumPK_ENMITY_STATE_CLOSE = 0,
	enumPK_ENMITY_STATE_TIME,
	enumPK_ENMITY_STATE_PKING,
	enumPK_ENMITY_STATE_NUM,
};

#ifdef _SERVER

class KPlayerPK
{
	friend class KPlayer;
private:
	int		m_nPlayerIndex;					
	BYTE	m_btNormalPKFlag;
	int		m_nEnmityPKState;
	int		m_nEnmityPKAim;
	int		m_nEnmityPKTime;
	int		m_nEnmityPKLaunchFlag;
	int		m_nExercisePKFlag;
	int		m_nExercisePKAim;
	char	m_cPKValue;
	
	bool	m_bForbidChangePK;
	
public:
	KPlayerPK();							
	void	Init(int nPlayerIdx);			
	void	SetNormalPKState(BYTE btFlag);		//
	BYTE	GetNormalPKState();				
	void	EnmityPKClose();				
	BOOL	EnmityPKOpen(int nAim);			
	int		GetEnmityPKState();				
	int		GetEnmityPKAim();				
	BOOL	IsEnmityPKLauncher() {return m_nEnmityPKLaunchFlag;};
	void	ExercisePKClose();				
	BOOL	ExercisePKOpen(int nAim);		
	int		GetExercisePKAim();				
	int		GetExercisePKState() {return m_nExercisePKFlag;};
	void	SetPKValue(char cValue);			
	char	GetPKValue() const ;					
	void	AddPKValue(char cAdd);
	void	CloseAll();
	void	EnmityPKCountDown();
	void	Active();
	
	void	SetForbidChangePK(bool bFlag) { m_bForbidChangePK = bFlag; }
	
};

#endif

#ifndef _SERVER
class KPlayerPK
{
	friend class KPlayer;
private:
	BYTE	m_btNormalPKFlag;
	int		m_nEnmityPKState;
	int		m_nEnmityPKAim;
	int		m_nEnmityPKTime;
	char	m_szEnmityAimName[32];			
	int		m_nExercisePKFlag;
	int		m_nExercisePKAim;
	char	m_szExerciseAimName[32];
	char	m_cPKValue;
	DWORD	m_PKChangeTime;

public:
	void	Init();							
	void	ApplySetNormalPKState(BYTE btFlag);
	void	SetNormalPKState(BYTE btFlag, BOOL bShowMsg = TRUE);
	BYTE	GetNormalPKState();				
	void	ApplyEnmityPK(char *lpszName);	
	void	ApplyEnmityPK(int nNpcID);		
	void	SetEnmityPKState(int nState, int nNpcID = 0, char *lpszName = NULL);	
	int		GetEnmityPKState()	{return m_nEnmityPKState;}		
	int		GetEnmityPKAimNpcID()	{return m_nEnmityPKAim;}	
	int		GetEnmityPKTime()	{return m_nEnmityPKTime;}		
	char*	GetEnmityPKAimName()	{return m_szEnmityAimName;}	
	void	EnmityPKCountDown();			
	void	SetExercisePKState(int nState, int nNpcID = 0, char *lpszName = NULL);	
	int		GetExercisePKState()	{return m_nExercisePKFlag;}	
	int		GetExercisePKAim()	{return m_nExercisePKAim;}		
	char*	GetExercisePKName()	{return m_szExerciseAimName;}	
	void	SetPKValue(char cValue);			
	char	GetPKValue() const { return m_cPKValue; }	
	void	Active();
	
	void	ChangePKState(BYTE btFlag);
	
};
#endif

#endif
