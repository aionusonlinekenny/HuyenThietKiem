
#include "KWin32.h"
#include "KFilePath.h"
#include "KFile.h"

// --
//
// --
KFile::KFile()
{
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}

// --
//
// --
KFile::~KFile()
{
	Close();
}

// --
//
// --
BOOL KFile::Open(LPSTR FileName)
{
	char PathName[MAXPATH];
	

	if (m_hFile != NULL)
		Close();

	g_GetFullPath(PathName, FileName);

#ifndef WIN32
        char *ptr = PathName;
        while(*ptr) {
          if(*ptr == '\\') *ptr = '/';
          ptr++;
        }

		char lcasePathName[MAXPATH];
		char szRootPath[MAXPATH];
		g_GetRootPath(szRootPath);
		strcpy(lcasePathName, PathName);
		if (memcmp(lcasePathName, szRootPath, strlen(szRootPath)) == 0)
			strlwr(lcasePathName + strlen(szRootPath));
		else
			strlwr(lcasePathName);
		if (NULL == (m_hFile = fopen(lcasePathName, "rb")))
#endif
	m_hFile = fopen(PathName, "rb");

	if (m_hFile == NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}


// --
//
// --
BOOL KFile::Create(LPSTR FileName)
{
	char PathName[MAXPATH];
	
	if (m_hFile != NULL)
		Close();
	
	g_GetFullPath(PathName, FileName);
	
	m_hFile = fopen(PathName, "wb+");
	
	if (m_hFile == NULL)
		return FALSE;

	return TRUE;
}

// --
//
// --
BOOL KFile::Append(LPSTR FileName)
{
	char PathName[MAXPATH];
	
	if (m_hFile != NULL)
		Close();
	
	g_GetFullPath(PathName, FileName);

	m_hFile = fopen(PathName, "ab");
	if (m_hFile == NULL)
		return FALSE;

	Seek(0, FILE_END);
	return TRUE;
}

// --
//
// --
void KFile::Close()
{
	if (m_hFile)
		fclose((FILE*)m_hFile);
	
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}
// --
//
// --
DWORD KFile::Read(LPVOID lpBuffer, DWORD dwReadBytes)
{
	DWORD dwBytesRead;
	
	if (m_hFile == NULL)
		return 0;
	
	dwBytesRead = fread(lpBuffer, 1, dwReadBytes, m_hFile);	
	m_dwPos += dwBytesRead;
	
	return dwBytesRead;
}
// --
//
// --
DWORD KFile::Write(LPVOID lpBuffer, DWORD dwWriteBytes)
{
	DWORD dwBytesWrite;
	
	if (m_hFile == NULL)
		return 0;

	dwBytesWrite = fwrite(lpBuffer, 1, dwWriteBytes, m_hFile);	
	m_dwPos += dwBytesWrite;
	
	return dwBytesWrite;
}
// --
//
// --
DWORD KFile::Seek(LONG lDistance, DWORD dwMoveMethod)
{
	if (m_hFile == NULL)
		return SEEK_ERROR;

	fseek(m_hFile, lDistance, dwMoveMethod);
	m_dwPos = ftell(m_hFile);
	return m_dwPos;
}

// --
//
// --
DWORD KFile::Tell()
{
	if (m_hFile == NULL)
		return SEEK_ERROR;

	return m_dwPos;
}
// --
//
// --
DWORD KFile::Size()
{
	if (m_hFile == NULL)
		return 0;

	if (m_dwLen == 0) 
	{
		DWORD temp = m_dwPos;
		m_dwLen = Seek(0, FILE_END);
		Seek(temp, FILE_BEGIN);
	}

	return m_dwLen;
}
// --
