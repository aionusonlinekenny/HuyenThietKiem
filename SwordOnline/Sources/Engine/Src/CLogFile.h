
#ifndef	_CLogFile_H_
#define	_CLogFile_H_
// --
#include "KDebug.h"
// --

class ENGINE_API CLogFile
{
private:
	int				m_nLevel;
	int				m_nFileSize;
	int				m_nDayOfYear;
	BOOL			m_bFileNameAutoChangeWithDate;
	FILE*			m_pLog;
	char			m_szSuffixName[20];
	char			m_szFileNameKey[260];
	char			m_szFile[MAX_PATH];	

public:
	CLogFile();
	~CLogFile();

	BOOL	Init(LPCSTR lpszFileName);
	BOOL	InitWithDate(LPCSTR lpszKeyName, LPCSTR lpszSuffixName, BOOL bEveryDayChangeFile);

	void	write_time();
	void	write_date_time();
	void	write_date_time_in(time_t tmCurrent);
	void	write_date();
	void	puts(const char* lpszString);
	void	puts_t(const char* lpszString);

	void	WriteLog(LPCSTR pszMsg, size_t nLen);
	BOOL	OpenNewFile();

	void	Clear();

};

extern ENGINE_API CLogFile g_LogFile;

#endif
