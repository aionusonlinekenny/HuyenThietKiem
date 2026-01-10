
#include "KWin32.h"
#include "KFilePath.h"
#include "KPakFile.h"
#include "KPakList.h"

#ifdef _DEBUG
#include "KDebug.h"
#endif

static int m_nPakFileMode = 0;

// --
//
// --
void g_SetPakFileMode(int nFileMode)
{
	m_nPakFileMode = nFileMode;
}

#define	PAK_INDEX_STORE_IN_RESERVED	0

#ifndef _SERVER
// --
//
// --
SPRHEAD* SprGetHeader(const char* pszFileName, SPROFFS*& pOffsetTable)
{
	pOffsetTable = NULL;

	if(pszFileName == NULL || pszFileName[0] == 0)
		return NULL;

	KPakFile	File;
	if (!File.Open(pszFileName))
		return NULL;

	SPRHEAD*		pSpr = NULL;
	if (File.IsFileInPak())
	{
		XPackElemFileRef	PakRef;
		if (g_pPakList->FindElemFile(pszFileName, PakRef))
		{
			pSpr = g_pPakList->GetSprHeader(PakRef, pOffsetTable);
			if (pSpr)
				pSpr->Reserved[PAK_INDEX_STORE_IN_RESERVED] = (WORD)(short)PakRef.nPackIndex;
		}
	}
	else
	{
		bool			bOk = false;
		SPRHEAD			Header;
		while(File.Read(&Header, sizeof(SPRHEAD)) == sizeof(SPRHEAD))
		{
			if (*(int*)&Header.Comment[0] != SPR_COMMENT_FLAG || Header.Colors > 256)
				break;

			unsigned int uEntireSize = File.Size();
			pSpr = (SPRHEAD*)malloc(uEntireSize);
			if (pSpr == NULL)
				break;

			uEntireSize -= sizeof(SPRHEAD);
			if (File.Read(&pSpr[1], uEntireSize) == uEntireSize)
			{
				pOffsetTable = (SPROFFS*)(((char*)(pSpr)) + sizeof(SPRHEAD) + Header.Colors * 3);
				Header.Reserved[PAK_INDEX_STORE_IN_RESERVED] = (WORD)(-1);
				memcpy(pSpr, &Header, sizeof(SPRHEAD));
				bOk = true;
			}
			break;
		};

		if (bOk == false && pSpr)
		{
			free (pSpr);
			pSpr = NULL;
		}
	}
	File.Close();
	return pSpr;
}

// --
//
// --
void SprReleaseHeader(SPRHEAD* pSprHeader)
{
    if (pSprHeader)
		free(pSprHeader);
}

// --
//
// --
SPRFRAME* SprGetFrame(SPRHEAD* pSprHeader, int nFrame)
{
	SPRFRAME*	pFrame = NULL;
	if (pSprHeader && g_pPakList)
	{
		int nPakIndex = (short)pSprHeader->Reserved[PAK_INDEX_STORE_IN_RESERVED];
		if (nPakIndex >= 0)
			pFrame = g_pPakList->GetSprFrame(nPakIndex, pSprHeader, nFrame);
	}
	return pFrame;
}

// --
//
// --
void SprReleaseFrame(SPRFRAME* pFrame)
{
    if (pFrame)
		free(pFrame);
}

#include "JpgLib.h"
#include "KDDraw.h"

// --
//
// --
KSGImageContent*	get_jpg_image(const char cszName[], unsigned uRGBMask16)
{
	KPakFile	File;
	unsigned char *pbyFileData = NULL;

	if (File.Open(cszName))
	{
		unsigned int uSize = File.Size();
		pbyFileData = (unsigned char *)malloc(uSize);
		if (pbyFileData)
		{
			if (File.Read(pbyFileData, uSize) != uSize)
			{
				free (pbyFileData);
				pbyFileData = NULL;
			}
		}
	}

	if (!pbyFileData)
        return NULL;

	int nResult = false;
    int nRetCode = false;
    KSGImageContent *pImageResult = NULL;

	BOOL		bRGB555;
	JPEG_INFO	JpegInfo;

    if (uRGBMask16 == ((unsigned)-1))
    {
    	bRGB555 = (g_pDirectDraw->GetRGBBitMask16() == RGB_555) ? TRUE : FALSE;
    }
    else
    {
        bRGB555 = (uRGBMask16 == RGB_555) ? TRUE : FALSE;
    }

    nRetCode = jpeg_decode_init(bRGB555, TRUE);
	if(!nRetCode)
        goto Exit0;
         
	nRetCode = jpeg_decode_info(pbyFileData, &JpegInfo);
    if (!nRetCode)
        goto Exit0;

	pImageResult = (KSGImageContent *)malloc(KSG_IMAGE_CONTENT_SIZE(JpegInfo.width, JpegInfo.height));
    if (!pImageResult)
        goto Exit0;

    pImageResult->nWidth = JpegInfo.width;
    pImageResult->nHeight = JpegInfo.height;

	nRetCode = jpeg_decode_data(pImageResult->Data, &JpegInfo);
    if (!nRetCode)
        goto Exit0;

    nResult = true;

Exit0:
	free (pbyFileData);
    if (!nResult && pImageResult)
	{
		free (pImageResult);
		pImageResult = NULL;
    }

	return pImageResult;
}


void release_image(KSGImageContent *pImage)
{
    if (pImage)
        free (pImage);
}

#endif

// --
// 
// --
KPakFile::KPakFile()
{
	m_PackRef.nPackIndex = -1;
	m_PackRef.uId = 0;
}

// --
//
// --
KPakFile::~KPakFile()
{
	Close();
}

// --
//
// --
bool KPakFile::IsFileInPak()
{
	return (m_PackRef.nPackIndex >= 0 && m_PackRef.uId);
}

// --
//
// --
BOOL KPakFile::Open(const char* pszFileName)
{
	if (pszFileName == NULL || pszFileName[0] == 0)
		return false;

	bool bOk = false;
	Close();
	if (m_nPakFileMode == 0)
	{
		bOk = (m_File.Open((char*)pszFileName) != FALSE);
		if (bOk == false && g_pPakList)
		{
			bOk = g_pPakList->FindElemFile(pszFileName, m_PackRef);
		}
	}
	else
	{
		if (g_pPakList)
			bOk = g_pPakList->FindElemFile(pszFileName, m_PackRef);
		if (bOk == false)
			bOk = (m_File.Open((char*)pszFileName) != FALSE);
	}
	return bOk;
}
#ifdef _DEBUG
// --

// --
BOOL KPakFile::Open4Unpack(const char* pszFileName, int nIndex)
{
	bool bOk = false;
#ifndef _SERVER
	if (pszFileName == NULL || pszFileName[0] == 0)
		return false;

	Close();
	if (g_pPakList)
	{
		bOk = g_pPakList->FindElemFile4Unpack(pszFileName, m_PackRef, nIndex);
	}
	//g_DebugLog("[KPakFile] Open Is: [%d]",bOk);

#endif
	return bOk;
}
#endif
// --
//
// --
DWORD KPakFile::Read(void* pBuffer, unsigned int uSize)
{
	if(m_PackRef.nPackIndex >= 0)
	{
		if (g_pPakList->ElemFileRead(m_PackRef, pBuffer, uSize) == false)
			uSize = 0;
	}
	else
	{
		uSize = m_File.Read(pBuffer, uSize);
	}
	return uSize;
}

// --
//
// --
DWORD KPakFile::Seek(int nOffset, unsigned int uMethod)
{
	if(m_PackRef.nPackIndex >= 0)
	{
		if (uMethod == FILE_BEGIN)
			m_PackRef.nOffset = nOffset;
		else if (uMethod == FILE_END)
			m_PackRef.nOffset = m_PackRef.nSize + nOffset;
		else
			m_PackRef.nOffset += nOffset;
		if (m_PackRef.nOffset > m_PackRef.nSize)
			m_PackRef.nOffset =  m_PackRef.nSize;
		else if (m_PackRef.nOffset < 0)
			m_PackRef.nOffset = 0;
		nOffset = m_PackRef.nOffset;
	}
	else
	{
		nOffset = m_File.Seek(nOffset, uMethod);
	}
	return nOffset;
}

// --
//
// --
DWORD KPakFile::Tell()
{
	int nOffset;
	if (m_PackRef.nPackIndex >= 0)
		nOffset = m_PackRef.nOffset;
	else
		nOffset = m_File.Tell();
	return nOffset;
}

// --
//
// --
DWORD KPakFile::Size()
{
	unsigned int uSize;
	if (m_PackRef.nPackIndex >= 0)
		uSize = m_PackRef.nSize;
	else
		uSize = m_File.Size();
	return uSize;
}
// --
//
// --
void KPakFile::Close()
{
	if (m_PackRef.nPackIndex >= 0)
	{
		m_PackRef.nPackIndex = -1;
		m_PackRef.uId = 0;
	}
	else
	{
		m_File.Close();
	}
	/*
	m_PackRef.nPackIndex = -1;
	m_PackRef.uId = 0;
	m_File.Close();
	*/
	
	
}


#ifdef _DEBUG
// --
//
// --
BOOL KPakFile::Save(const char* pszFileName)
{
	KFile File;
	if (!File.Create((char*)pszFileName))
		return FALSE;

	DWORD dwSize = Size();
	//g_DebugLog("[KPakFile] Save Size1: [%d]",dwSize);

	void* lpBuffer = new BYTE[dwSize];
	if(!lpBuffer) 
		return FALSE;

	DWORD dwSize2 = Read(lpBuffer,dwSize);
	//g_DebugLog("[KPakFile] Save Size2: [%d]",dwSize2);

	File.Write(lpBuffer, dwSize);
	File.Close();
	if(lpBuffer) 
		delete [] lpBuffer;
	return TRUE;
}
#endif

