
#ifndef	ZLoadFileH
#define	ZLoadFileH

#include "KTabFile.h"
#include "KIniFile.h"

#define			MAX_TAB_FILE		10
#define			MAX_INI_FILE		5
class ZLoadFile
{
private:
	KTabFile	m_cTabFile[MAX_TAB_FILE];
	KIniFile	m_cIniFile[MAX_INI_FILE];

private:
	int			FindTabFree();
	int			FindIniFree();

public:
	ZLoadFile::ZLoadFile();
	ZLoadFile::~ZLoadFile();
	BOOL		Init();
	//
	int			SearchTabFile(DWORD dwFileID);
	BOOL		ReadTabFile(const char* pFileName);
	BOOL		ClearTabFile(const char* pFileName);
	int			GetTabFileRow(const char* pFileName);
	int			GetTabFileColum(const char* pFileName);
	void		GetTabFileCell(const char* pFileName, int nRow, int nColum, char* pResult);
	//
	int			SearchIniFile(DWORD dwFileID);
	BOOL		ReadIniFile(const char* pFileName);
	BOOL		ClearIniFile(const char* pFileName);
	BOOL		GetIniFileData(const char* pFileName, const char* pSec, const char* pKey, char* pResult);
};

extern ZLoadFile g_LoadFile;

#endif
