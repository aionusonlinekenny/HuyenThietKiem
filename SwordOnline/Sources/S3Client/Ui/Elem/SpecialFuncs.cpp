#include "KWin32.h"
#include "KEngine.h"
#include "Shlobj.h"
#include <time.h>
#include "../../../Engine/Src/Text.h"
#include "SpecialFuncs.h"
#include "..\UiCase\UiMsgCentrePad.h"
extern iRepresentShell*	g_pRepresentShell;

#define	SCREEN_PIC_SAVE_PATH	"\\UserData\\Screenshot\\"
static char szScrPicPath[MAX_PATH] = "";

// --
//
// --
int OutputTabSplitText(const char* pBuffer, int nCount,
			int nLineWidth, int nFontId,
			KOutputTextParam* pParam)
{
	if (pBuffer == NULL || nCount < 0 || g_pRepresentShell == NULL || nFontId <= 0)
		return 0;

	int	nOldX = pParam->nX;
	int nOldY = pParam->nY;
	int nOutputLineCount = 0;
	int	nCurrentPos = 0;

	pParam->nSkipLine = 0;
	int nMaxLineCount = pParam->nNumLine;
	pParam->nNumLine = 1;

	while(nCurrentPos < nCount && nOutputLineCount < nMaxLineCount)
	{
		int	nLineEndPos = TFindSpecialCtrlInEncodedText(pBuffer, nCount, nCurrentPos, KTC_ENTER);
		int	nTabPos = TFindSpecialCtrlInEncodedText(pBuffer, nCount, nCurrentPos, KTC_TAB);
		
		int nLen;
		if (nTabPos >= 0)
			nLen = nTabPos - nCurrentPos;
		else if (nLineEndPos >= 0)
			nLen = nLineEndPos - nCurrentPos;
		else
			nLen = nCount - nCurrentPos;

		g_pRepresentShell->OutputRichText(nFontId, pParam, pBuffer + nCurrentPos, nLen);

		nCurrentPos += nLen;
		if (nTabPos >= 0)
		{
			nCurrentPos ++;
			if (nLineEndPos >= 0)
				nLen = nLineEndPos - nCurrentPos;
			else
				nLen = nCount - nCurrentPos;
			if (nLen > 0)
			{
				int nOutputLen;
				TGetEncodedTextLineCount(pBuffer + nCurrentPos, nLen, 0, nOutputLen, nFontId);
				pParam->nX += nLineWidth - nOutputLen * nFontId / 2;
				g_pRepresentShell->OutputRichText(nFontId, pParam, pBuffer + nCurrentPos, nLen);
				pParam->nX = nOldX;
			}
			nCurrentPos += nLen;
		}
		pParam->nY += nFontId + 1;
		nOutputLineCount++;
	}

	pParam->nY = nOldY;
	return nOutputLineCount;
}



// --
//
// --
bool SetScrPicPath(const char* szPath)
{
	if(!szPath || !szPath[0] || strlen(szPath) >= MAX_PATH)
	{
		if(!SHGetSpecialFolderPath(NULL, szScrPicPath, CSIDL_DESKTOPDIRECTORY , false))
			return false;
		strcat(szScrPicPath,"\\Screenshot\\");
	}
	else
	{
		int nLen = strlen(szPath);
		memcpy(szScrPicPath, szPath, nLen);
		if (szPath[nLen - 1] == '\\')
			szScrPicPath[nLen] = 0;
		else
		{
			szScrPicPath[nLen] = '\\';
			szScrPicPath[nLen + 1] = 0;
		}
	}

	return true;
}
// --
//
// --
char* GetSrcPicPath()
{
	if(!szScrPicPath[0])
		SetScrPicPath(NULL);

	return szScrPicPath;
}
// --
//
// --
bool GetAPrintScreenFileName(char *buf, int nBufLen)
{
	if(nBufLen < 128)
		return false;

	time_t ltime;
    struct tm *today;
	time( &ltime );
	today = localtime( &ltime );
	strftime( buf, 128, "%Y-%m-%d_%H-%M-%S", today );
	return true;
}

// --
//
// --
void SaveScreenToFile(ScreenFileType eType, unsigned int nQuality)
{
	if(!szScrPicPath[0] && !SetScrPicPath(NULL))
		return;

	char	szFile[256];
	int		nLen;
	nLen = sprintf(szFile, "%s", szScrPicPath);	
	if(GetAPrintScreenFileName(&szFile[nLen], sizeof(szFile) - nLen))
	{	
		
		g_CreatePath(szScrPicPath);

		if(eType == SCRFILETYPE_BMP)
			strcat(szFile, ".bmp");
		else
			strcat(szFile, ".jpg");
		KUiMsgCentrePad::NewChannelMessageArrival(0, "Screenshot", szFile, strlen(szFile));
		g_pRepresentShell->SaveScreenToFile(szFile, eType, nQuality);
	}
}
