
#include "KCore.h"
#include "ZLoadFile.h"

ZLoadFile g_LoadFile;
// --
//
// --
ZLoadFile::ZLoadFile()
{
	
}

// --
//
// --
ZLoadFile::~ZLoadFile()
{
}

// --
//
// --
BOOL ZLoadFile::Init()
{
	WORD i;
	for(i = 0; i < MAX_TAB_FILE; i++)
	{
		m_cTabFile[i].Clear();
	}
	for(i = 0; i < MAX_INI_FILE; i++)
	{
		m_cIniFile[i].Clear();
	}
	return TRUE;
}

// --
//
// --
int ZLoadFile::FindTabFree()
{
	WORD i;
	for(i = 0; i < MAX_TAB_FILE; i++)
	{
		if(m_cTabFile[i].GetFileNameId() == 0)
			return i;
	}
	return -1;
}

// --
//
// --
int ZLoadFile::SearchTabFile(DWORD dwFileID)
{
	WORD i;
	for(i = 0; i < MAX_TAB_FILE; i++)
	{
		if(m_cTabFile[i].GetFileNameId() == dwFileID)
			return i;
	}
	return -1;
}

// --
//
// --
BOOL ZLoadFile::ReadTabFile(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	
	if( SearchTabFile(dwID) >= 0 )
		return TRUE;
	
	const int i = FindTabFree();
	if(i < 0)
		return FALSE;
	
	
	return m_cTabFile[i].Load((char*)pFileName);	
}

// --
//
// --
BOOL ZLoadFile::ClearTabFile(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchTabFile(dwID);
	if(i < 0)
		return FALSE;

	m_cTabFile[i].Clear();

	return TRUE;
}

// --
//
// --
int ZLoadFile::GetTabFileRow(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchTabFile(dwID);
	if(i < 0)
		return FALSE;

	return m_cTabFile[i].GetHeight();
}

// --
//
// --
int ZLoadFile::GetTabFileColum(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchTabFile(dwID);
	if(i < 0)
		return FALSE;

	return m_cTabFile[i].GetWidth();
}

// --
//
// --
void ZLoadFile::GetTabFileCell(const char* pFileName, int nRow, int nColum, char* pResult)
{
	if(!pResult)
		return;

	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchTabFile(dwID);
	if(i < 0)
		return;
	
	char szResult[256];
	m_cTabFile[i].GetString(nRow, nColum, "", szResult, sizeof(szResult));

	memcpy(pResult, szResult, sizeof(szResult));
}

// --
//
// --
int ZLoadFile::FindIniFree()
{
	WORD i;
	for(i = 0; i < MAX_INI_FILE; i++)
	{
		if(m_cIniFile[i].GetFileNameId() == 0)
			return i;
	}
	return -1;
}

// --
//
// --
int ZLoadFile::SearchIniFile(DWORD dwFileID)
{
	WORD i;
	for(i = 0; i < MAX_INI_FILE; i++)
	{
		if(m_cIniFile[i].GetFileNameId() == dwFileID)
			return i;
	}
	return -1;
}

// --
//
// --
BOOL ZLoadFile::ReadIniFile(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	
	if( SearchIniFile(dwID) >= 0 )
		return TRUE;
	
	const int i = FindIniFree();
	if(i < 0)
		return FALSE;
	
	return m_cIniFile[i].Load((char*)pFileName);
	
}

// --
//
// --
BOOL ZLoadFile::ClearIniFile(const char* pFileName)
{
	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchIniFile(dwID);
	if(i < 0)
		return FALSE;

	m_cIniFile[i].Clear();

	return TRUE;
}

// --
//
// --
BOOL ZLoadFile::GetIniFileData(const char* pFileName, const char* pSec, const char* pKey, char* pResult)
{
	if(!pResult)
		return FALSE;

	DWORD dwID = g_FileName2Id((char*)pFileName);
	const int i = SearchIniFile(dwID);
	if(i < 0)
		return FALSE;
	
	char szResult[80];
	m_cIniFile[i].GetString(pSec, pKey, "", szResult, sizeof(szResult));

	memcpy(pResult, szResult, sizeof(szResult));

	return TRUE;
}