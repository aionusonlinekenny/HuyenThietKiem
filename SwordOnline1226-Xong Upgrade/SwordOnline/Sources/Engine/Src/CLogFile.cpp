
#include "KWin32.h"
#include "KEngine.h"

#include <sys/stat.h>
#include <time.h>

#include "CLogFile.h"

// --
//
// --
CLogFile::CLogFile()
{
	m_nLevel = 3;
	m_nFileSize = 0;
	m_nDayOfYear = 0;
	m_bFileNameAutoChangeWithDate = 0;
	m_pLog = 0;
	m_szFile[0] = 0;
	
	//if( pszFileName )
		//Init(pszFileName);
}

// --
//
// --
CLogFile::~CLogFile()
{
	if( m_pLog )
	{
		fclose(m_pLog);
		m_pLog = NULL;
	}
}

// --
//
// --
BOOL CLogFile::Init(LPCSTR lpszFileName)
{
	if(!lpszFileName)
		return FALSE;

	if(m_pLog)
	{
		fclose(m_pLog);
		m_pLog = NULL;
	}

	char szFile[MAX_PATH];
	g_GetFullPath(szFile, (char*)lpszFileName);

	m_pLog = fopen(szFile, "ab+");
	if(m_pLog)
	{
		struct stat sInf;
		memset(&sInf, 0, sizeof(sInf));
		
		if( stat(szFile, &sInf) )
			m_nFileSize = 0;
		else
			m_nFileSize = sInf.st_size;

		strcpy(m_szFile, szFile);
		return TRUE;
	}

	return FALSE;
}

// --
//
// --
BOOL CLogFile::InitWithDate(LPCSTR lpszKeyName, LPCSTR lpszSuffixName, BOOL bEveryDayChangeFile)
{
	if(!lpszKeyName)
		return FALSE;
	
	char szFileName[1024];
	time_t tmCurrent;
	struct tm* timeinfo; 

	tmCurrent = time(0);
	timeinfo = localtime(&tmCurrent);
	
	if(timeinfo)
	{
		sprintf(szFileName,	"%s_%04d%02d%02d.%s", lpszKeyName, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, lpszSuffixName);
		m_nDayOfYear = timeinfo->tm_yday;
	}
	else
	{
		sprintf(szFileName, "%s.%s", lpszKeyName, lpszSuffixName);
	}
	szFileName[1023] = 0;
	strcpy(m_szFileNameKey, lpszKeyName);
    m_szFileNameKey[259] = 0;

    strcpy(m_szSuffixName, lpszSuffixName);
    m_szSuffixName[19] = 0;

    m_bFileNameAutoChangeWithDate = bEveryDayChangeFile;
	
    return Init(szFileName);

}
// --
//
// --
void CLogFile::Clear()
{
	if( m_pLog )
		fclose(m_pLog);	
	m_pLog = NULL;
	//
	m_nLevel = 3;
	m_nFileSize = 0;
	m_nDayOfYear = 0;
	m_bFileNameAutoChangeWithDate = 0;
	m_pLog = 0;
	m_szFile[0] = 0;
}

// --
//
// --
void CLogFile::write_time()
{
	if(!m_pLog)
		return;

	char szTime[256];
	time_t tmCurrent;
	struct tm* timeinfo; 

	tmCurrent = time(0);
	timeinfo = localtime(&tmCurrent);
	if(timeinfo)
	{
		sprintf(szTime, "Time(hh-mm-ss): %02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		if( m_bFileNameAutoChangeWithDate )
		{
			if( timeinfo->tm_yday != m_nDayOfYear )
				InitWithDate(m_szFileNameKey, m_szSuffixName, 1);
		}
		WriteLog(szTime, strlen(szTime));
	}
}

// --
//
// --
void CLogFile::write_date_time()
{
	if(!m_pLog)
		return;

	char szDateTime[256];
	time_t tmCurrent;
	struct tm* timeinfo; 

	tmCurrent = time(0);
	timeinfo = localtime(&tmCurrent);
	if(timeinfo)
	{
		sprintf(szDateTime,  
		"%04d-%02d-%02d %02d:%02d:%02d\t", 
		timeinfo->tm_year + 1900,
        timeinfo->tm_mon + 1,
        timeinfo->tm_mday, 
		timeinfo->tm_hour, 
		timeinfo->tm_min, 
		timeinfo->tm_sec);

		if( m_bFileNameAutoChangeWithDate )
		{
			if( timeinfo->tm_yday != m_nDayOfYear )
				InitWithDate(m_szFileNameKey, m_szSuffixName, 1);
		}
		WriteLog(szDateTime, strlen(szDateTime));
	}
}

// --
//
// --
void CLogFile::write_date_time_in(time_t tmCurrent)
{
	if(!m_pLog)
		return;

	char szDateTime[256];
	struct tm* timeinfo; 

	timeinfo = localtime(&tmCurrent);
	if(timeinfo)
	{
		sprintf(szDateTime,  
		"%04d-%02d-%02d %02d:%02d:%02d\t", 
		timeinfo->tm_year + 1900,
        timeinfo->tm_mon + 1,
        timeinfo->tm_mday, 
		timeinfo->tm_hour, 
		timeinfo->tm_min, 
		timeinfo->tm_sec);

		if( m_bFileNameAutoChangeWithDate )
		{
			if( timeinfo->tm_yday != m_nDayOfYear )
				InitWithDate(m_szFileNameKey, m_szSuffixName, 1);
		}
		WriteLog(szDateTime, strlen(szDateTime));
	}
}

// --
//
// --
void CLogFile::write_date()
{
	if(!m_pLog)
		return;

	char szDate[256];
	time_t tmCurrent;
	struct tm* timeinfo; 

	tmCurrent = time(0);
	timeinfo = localtime(&tmCurrent);
	if(timeinfo)
	{
		sprintf(szDate, "%04d-%02d-%02d ", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);

		if( m_bFileNameAutoChangeWithDate )
		{
			if( timeinfo->tm_yday != m_nDayOfYear )
				InitWithDate(m_szFileNameKey, m_szSuffixName, 1);
		}
		WriteLog(szDate, strlen(szDate));
	}
}

// --
//
// --
void CLogFile::puts(const char *pcszString)
{
	if(!pcszString)
		return;

	time_t tmCurrent;
	struct tm* timeinfo; 

	tmCurrent = time(0);
	timeinfo = localtime(&tmCurrent);
	if(timeinfo)
	{
		if( m_bFileNameAutoChangeWithDate )
		{
			if( timeinfo->tm_yday != m_nDayOfYear )
				InitWithDate(m_szFileNameKey, m_szSuffixName, 1);
		}
		WriteLog(pcszString, strlen(pcszString));
	}
}

// --
//
// --
void CLogFile::puts_t(const char *pcszString)
{
	write_date_time();
	puts(pcszString);
}

// --
//
// --
void CLogFile::WriteLog(LPCSTR pszMsg, size_t nLen)
{
	if(!m_pLog)
		return;

	int nFileSize;
	nFileSize = fwrite(pszMsg, sizeof(char), nLen, m_pLog);

	if( nFileSize > 0 )
	{
		m_nFileSize += nFileSize;
		fflush(m_pLog);
		if( m_nFileSize > 0x3FFFFFFF )
			OpenNewFile();
	}

}

// --
//
// --
BOOL CLogFile::OpenNewFile()
{	
	char* pSymbol;
	LPCSTR pszSuffix;

	char szFile[260];

	strcpy(szFile, m_szFile);
	szFile[259] = 0;

	pszSuffix = "(INF)\tmod:%s\tact:%s\t" + 20;
	
	pSymbol = strrchr(szFile, 46);
	if( szFile < pSymbol )
	{
		*pSymbol = 0;
		pszSuffix = pSymbol + 1;
	}
	int i = 1;
	struct stat sInf;

	char szPath[520];
	FILE* fFile;
	while(1)
	{
		szPath[sprintf(szPath, "%s_%d.%s", szFile, i, pszSuffix)] = 0;
		memset(&sInf, 0, sizeof(sInf));

		if( stat(szPath, &sInf) || sInf.st_size <= 0x3FFFFFFF )
		{
		  fFile = fopen(szPath, "ab+");
		  if( fFile )
			break;
		}
		i++;
		if(i >= 1000)
			return FALSE;
	}

	if(m_pLog)
		fclose(this->m_pLog);

	m_pLog = fFile;
	m_nFileSize = sInf.st_size;
	return TRUE;
}