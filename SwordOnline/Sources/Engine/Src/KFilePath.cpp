
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFilePath.h"

#ifndef _SERVER
#include "KPakList.h"
#endif

#ifndef WIN32
#include <unistd.h>
#else
#include <direct.h>
#endif
#include <string.h>
// --
#ifdef WIN32
	static char szRootPath[MAXPATH] = "C:";
	static char szCurrPath[MAXPATH] = "\\";
#else	// Linux
	static char szRootPath[MAXPATH] = "/";
	static char szCurrPath[MAXPATH] = "/";
#endif
// --

// --
//
// --
int RemoveTwoPointPath(LPTSTR szPath, int nLength)
{
	int nRemove = 0;
	KASSERT(szPath);
#ifdef WIN32
	LPCTSTR lpszOld = "\\..\\";
#else
	LPCTSTR lpszOld = "/../";
#endif
	LPTSTR lpszTarget = strstr(szPath, lpszOld);
	if (lpszTarget)
	{
		LPTSTR lpszAfter = lpszTarget + 3;
		while(lpszTarget > szPath)
		{
			lpszTarget--;
			if ((*lpszTarget) == '\\' ||(*lpszTarget) == '/')
				break;
		}
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (lpszAfter - lpszTarget);
		return RemoveTwoPointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

// --
//
// --
int RemoveOnePointPath(LPTSTR szPath, int nLength)
{
	int nRemove = 0;
	KASSERT(szPath);
#ifdef WIN32
	LPCTSTR lpszOld = "\\.\\";
#else
	LPCTSTR lpszOld = "/./";
#endif
	LPTSTR lpszTarget = strstr(szPath, lpszOld);
	if (lpszTarget)
	{
		LPTSTR lpszAfter = lpszTarget + 2;
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (lpszAfter - lpszTarget);
		return RemoveOnePointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

// --
//
// --
int RemoveAllPointPath(LPTSTR szPath, int nLength)
{
	return RemoveOnePointPath(szPath, RemoveTwoPointPath(szPath, nLength));
}

// --
//
// --
ENGINE_API void g_SetRootPath(LPSTR lpPathName)
{
	if(lpPathName)
	{

#ifdef _DEBUG
		//g_DebugLog("[ENGINE] SetRootPath %s\n", lpPathName);
#endif

		g_StrCpy(szRootPath, lpPathName);
	}
	else
	{

#ifdef _DEBUG
		//g_DebugLog("[ENGINE] SetRootPath NULL\n", lpPathName);
#endif

		GetCurrentDirectory(MAXPATH, szRootPath);
	}

	const int nLen = g_StrLen(szRootPath);
	if( (szRootPath[nLen - 1] == '\\') || (szRootPath[nLen - 1] == '/') )
	{
		szRootPath[nLen - 1] = 0;
	}

#ifdef _DEBUG
	//g_DebugLog("RootPath = %s", szRootPath);
#endif

}

// --
//
// --
ENGINE_API void g_GetRootPath(LPSTR lpPathName)
{
	g_StrCpy(lpPathName, szRootPath);
}

// --
//
// --
ENGINE_API void g_SetFilePath(LPSTR lpPathName)
{
	if(lpPathName[0] == '\\' ||lpPathName[0] == '/')
	{
		g_StrCpy(szCurrPath, lpPathName + 1);
	}
	else
	{
		g_StrCpy(szCurrPath, lpPathName);
	}

	int len = g_StrLen(szCurrPath);
	if (len > 0 && szCurrPath[len - 1] != '\\' && szCurrPath[len - 1] != '/')
	{
#ifdef WIN32
		szCurrPath[len] = '\\';
#else
		szCurrPath[len] = '/';
#endif
		szCurrPath[len + 1] = 0;
	}

	RemoveAllPointPath(szCurrPath, len + 1);

#ifndef WIN32
	for (len = 0; szCurrPath[len]; len++)
	{
		if (szCurrPath[len] == '\\')
			szCurrPath[len] = '/';
	}
#endif
}

// --
//
// --
ENGINE_API	void g_GetFilePath(LPSTR lpPathName)
{
	g_StrCpy(lpPathName, szCurrPath);
}

// --
//
// --
ENGINE_API void g_GetFullPath(LPSTR lpPathName, LPSTR lpFileName)
{
	if (lpFileName[1] == ':')
	{
		g_StrCpy(lpPathName, lpFileName);
		return;
	}

	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, szRootPath);
		g_StrCat(lpPathName, lpFileName);
		return;
	}
	
#ifdef WIN32
	if (szCurrPath[1] == ':')
	{
		g_StrCpy(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
		return;
	}
#endif
	
	g_StrCpy(lpPathName, szRootPath);
        if(szCurrPath[0] != '\\' && szCurrPath[0] != '/') {
#ifdef WIN32
	g_StrCat(lpPathName, "\\");
#else
	g_StrCat(lpPathName, "/");
#endif
      }
	g_StrCat(lpPathName, szCurrPath);

	if (lpFileName[0] == '.' && (lpFileName[1] == '\\'||lpFileName[1] == '/') )
		g_StrCat(lpPathName, lpFileName + 2);
	else
		g_StrCat(lpPathName, lpFileName);
}

// --
//
// --
ENGINE_API void g_GetHalfPath(LPSTR lpPathName, LPSTR lpFileName)
{
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, lpFileName);
	}
	else
	{
#ifdef WIN32
		g_StrCpy(lpPathName, "\\");
#else
		g_StrCpy(lpPathName, "/");
#endif
		g_StrCat(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
}

// --
//
// --
ENGINE_API void g_GetPackPath(LPSTR lpPathName, LPSTR lpFileName)
{
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, lpFileName + 1);
	}
	else
	{
		g_StrCpy(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
	int len = g_StrLen(lpPathName);
	RemoveAllPointPath(lpPathName, len + 1);
	g_StrLower(lpPathName);
}

// --
//
// --
ENGINE_API void g_GetDiskPath(LPSTR lpPathName, LPSTR lpFileName)
{
	g_StrCpy(lpPathName, "C:");
	/*
	for (int i = 0; i < 24; lpPathName[0]++, i++)
	{
//		if (GetDriveType(lpPathName) == DRIVE_CDROM)
//			break;
	}
	*/
	if (lpFileName[0] == '\\' || lpPathName[0] == '/')
	{
		g_StrCat(lpPathName, lpFileName);
	}
	else
	{
#ifdef WIN32
		g_StrCat(lpPathName, "\\");
#else
		g_StrCat(lpPathName, "/");
#endif
		g_StrCat(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
}

// --
//
// --
ENGINE_API	void	g_CreatePath(LPSTR lpPathName)
{
	if (!lpPathName || !lpPathName[0])
		return;

	char	szFullPath[MAXPATH];
	int		i;
	if (lpPathName[1] == ':')
	{
		if (g_StrLen(lpPathName) < 4)
			return;
		g_StrCpy(szFullPath, lpPathName);
		i = 4;
	}
	else if (lpPathName[0] == '\\' || lpPathName[0] == '/')
	{
		g_StrCpy(szFullPath, szRootPath);
		g_StrCat(szFullPath, lpPathName);
		i = g_StrLen(szRootPath) + 1;
	}
	else
	{
		g_StrCpy(szFullPath, szRootPath);
#ifdef WIN32
		g_StrCat(szFullPath, "\\");
#else
		g_StrCat(szFullPath, "/");
#endif
		g_StrCat(szFullPath, lpPathName);
                
		i = g_StrLen(szRootPath) + 1;
	}

	for (; i < g_StrLen(szFullPath); i++)
	{
#ifdef WIN32
		if (szFullPath[i] == '\\') {
			szFullPath[i] = 0;
			CreateDirectory(szFullPath, NULL);
			szFullPath[i] = '\\';
		}
#else
		if (szFullPath[i] == '/') {
			szFullPath[i] = 0;
			szFullPath[i] = '/';
		}
#endif
	}
#ifdef WIN32
	CreateDirectory(szFullPath, NULL);
#else
	// flying comment
	// 朱传靖实现这个调用
	//mkdir();
#endif
}

// --
//
// --
ENGINE_API	void	g_UnitePathAndName(char *lpPath, char *lpFile, char *lpGet)
{
	if (!lpPath || !lpFile || !lpGet)
		return;
	strcpy(lpGet, lpPath);
	int	nSize = strlen(lpGet);
	if (lpGet[nSize] - 1 != '\\')
	{
		lpGet[nSize] = '\\';
		lpGet[nSize + 1] = 0;
	}
	if (lpFile[0] != '\\')
	{
		strcat(lpGet, lpFile);
	}
	else
	{
		strcat(lpGet, &lpFile[1]);
	}
}

// --
//
// --
ENGINE_API BOOL g_FileExists(LPSTR FileName)
{
	BOOL	bExist = FALSE;
	char	szFullName[MAX_PATH];
	if(FileName && FileName[0])
	{
		if (g_pPakList)
		{
			XPackElemFileRef	PackRef;
			bExist = g_pPakList->FindElemFile(FileName, PackRef);
		}

		if (bExist == FALSE)
		{
			#ifdef	WIN32
				g_GetFullPath(szFullName, FileName);
				bExist = !(GetFileAttributes(szFullName) & FILE_ATTRIBUTE_DIRECTORY);// || dword == INVALID_FILE_ATTRIBUTES)
			#endif
		}
	}
	return bExist;
}

// --
//
// --
ENGINE_API DWORD g_FileName2Id(LPSTR lpFileName)
{
	DWORD Id = 0;
	char c = 0;
	for (int i = 0; lpFileName[i]; i++)
	{
		c = lpFileName[i];
#ifndef WIN32
		if ('/' == c)
			c = '\\';
#endif
		Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
	}
	return (Id ^ 0x12345678);
}

// --
//
// --
ENGINE_API void g_ChangeFileExt(LPSTR lpFileName, LPSTR lpNewExt)
{
	int  i;

	for (i = 0; lpFileName[i]; i++)
	{
		if (lpFileName[i] == '.')
			break;
	}
	
	if (lpFileName[i] == '.')
	{
		g_StrCpy(&lpFileName[i + 1], lpNewExt);
	}
	else
	{
		g_StrCat(lpFileName, ".");
		g_StrCat(lpFileName, lpNewExt);
	}
}

// --
//
// --
ENGINE_API void g_ExtractFileName(LPSTR lpFileName, LPSTR lpFilePath)
{
	int nLen = g_StrLen(lpFilePath);
	if (nLen < 5)
		return;
	int nPos = nLen;
	while (--nPos > 0)
	{
		if (lpFilePath[nPos] == '\\'||lpFilePath[nPos] == '/')
			break;
	}
	g_StrCpy(lpFileName, &lpFilePath[nPos + 1]);
}

// --
//
// --
ENGINE_API void g_ExtractFilePath(LPSTR lpPathName, LPSTR lpFilePath)
{
	int nLen = g_StrLen(lpFilePath);
	if (nLen < 5)
		return;
	int nPos = nLen;
	while (--nPos > 0)
	{
		if (lpFilePath[nPos] == '\\' ||lpFilePath[nPos] == '/')
			break;
	}
	g_StrCpyLen(lpPathName, lpFilePath, nPos + 1);
}

