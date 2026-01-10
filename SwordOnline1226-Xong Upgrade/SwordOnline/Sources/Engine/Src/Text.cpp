/*****************************************************************************************
//	????????? 
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-31
// --
*****************************************************************************************/
#include "KWin32.h"
#include <string.h>
#ifndef __linux
#include <crtdbg.h>
#else
#define HRESULT	long
#define S_OK 0
#endif

#include "Text.h"
#define		MAX_ENCODED_CTRL_LEN	4


IInlinePicEngineSink* g_pIInlinePicSink = NULL;
extern "C" ENGINE_API HRESULT 
AdviseEngine(IInlinePicEngineSink* pSink)
{
	_ASSERT(NULL == g_pIInlinePicSink);
	g_pIInlinePicSink = pSink;
	return S_OK;
}

extern "C" ENGINE_API HRESULT 
UnAdviseEngine(IInlinePicEngineSink* pSink)
{
	if (pSink == g_pIInlinePicSink)
		g_pIInlinePicSink = NULL;
	return S_OK;
}

#define	NUM_CHARACTER_IN_A1	11
unsigned char	s_NotAllowAtLineHeadA1Characters[NUM_CHARACTER_IN_A1] = 
{
	0xa2, 0xa3, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf
};
#define	NUM_CHARACTER_IN_A3	10
unsigned char	s_NotAllowAtLineHeadA3Characters[NUM_CHARACTER_IN_A3] = 
{
	0xa1, 0xa9, 0xac, 0xae, 0xba, 0xbb, 0xbe, 0xbf, 0xdd, 0xfd
};
#define	NUM_CHARACTER_IN_00	7
unsigned char	s_NotAllowAtLineHead00Characters[NUM_CHARACTER_IN_00] =
{
	0x21, 0x29, 0x2c, /*0x2e,*/ 0x3a, 0x3b, 0x3e, 0x3f
};

extern "C" ENGINE_API
int TIsCharacterNotAlowAtLineHead(const char* pCharacter)
{
	int				i;
	unsigned char	cChar;
	cChar = (unsigned char)(*pCharacter);
	if (cChar == 0xa3)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa1 && cChar <= 0xfd)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A3; i++)
				if (s_NotAllowAtLineHeadA3Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar == 0xa1)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa2 && cChar <= 0xbf)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A1; i++)
				if (s_NotAllowAtLineHeadA1Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar >= 0x21 && cChar <= 0x3f)
	{
		for (i = 0; i < NUM_CHARACTER_IN_00; i++)
			if (s_NotAllowAtLineHead00Characters[i] == cChar)
				return 1;
	}
	return false;
}

extern "C" ENGINE_API
const char* TGetSecondVisibleCharacterThisLine(const char* pCharacter, int nPos, int nLen)
{
	if (pCharacter && nLen > 0)
	{
		bool bFoundFirst = false;
		while(nPos < nLen)
		{
			unsigned char cChar = (unsigned char)(pCharacter[nPos]);
			if (cChar >= 0x20)
			{
				if (bFoundFirst)
					return (pCharacter + nPos);
				bFoundFirst = true;
				if (cChar > 0x80)
					nPos += 2;
				else
					nPos ++;
				continue;
			}
			if (cChar == KTC_COLOR || cChar == KTC_BORDER_COLOR)
				nPos += 4;
			else if (cChar == KTC_INLINE_PIC)
				nPos += 1 + sizeof(WORD);
			else if (cChar == KTC_COLOR_RESTORE || cChar == KTC_BORDER_RESTORE)
				nPos++;
			break;
		};
	}
	return NULL;
}



extern "C" ENGINE_API
int TSplitString(const char* pString, int nDesirePos, int bLess)
{
	register int	nPos = 0;
	if (pString)
	{
		nDesirePos -= 2;
		while(nPos < nDesirePos)
		{
			/*
			if ((unsigned char)pString[nPos] > 0x80)
				nPos += 2;
			else
			*/
			if (pString[nPos])
				nPos++;
			else
				break;
		};
		nDesirePos += 2;
		while(nPos < nDesirePos)
		{	
			/*
			if ((unsigned char)pString[nPos] > 0x80)
			{
				if (bLess && (nPos + 2 > nDesirePos))
					break;
				if (pString[nPos + 1] == 0)
				{
					nPos ++;
					break;
				}				
				nPos += 2;
			}
			else
			*/
			if (pString[nPos])
				nPos ++;
			else
				break;
		}
	}
	return nPos;
}


extern "C" ENGINE_API
int	TSplitEncodedString(const char* pString, int nCount, int nDesirePos, int bLess)
{
	int	nPos = 0;
	if (pString)
	{
		if (nDesirePos <= nCount)
		{
			register unsigned char cCharacter;
			nDesirePos -= MAX_ENCODED_CTRL_LEN;
			while (nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				/*
				if (cCharacter > 0x80)
					nPos += 2;
				else 
				*/
				if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
					nPos += 4;
				else if (cCharacter == KTC_INLINE_PIC)
					nPos += 3;
				else
					nPos ++;
			}
			nPos += MAX_ENCODED_CTRL_LEN;
			while(nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				/*
				if (cCharacter > 0x80)
				{
					if (bLess && (nPos + 2 > nDesirePos))
						break;
					if (nPos + 1 == nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 2;
				}				
				else 
				*/
				if (cCharacter == KTC_INLINE_PIC)
				{
					if (bLess && nPos + 3 > nDesirePos)
						break;
					if (nPos + 3 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 3;
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					if (bLess && (nPos + 4 > nDesirePos))
						break;
					if (nPos + 4 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 4;
				}
				else
					nPos ++;
			}

		}
		else
		{
			nPos = nCount;
		}
	}
	return nPos;
	
}

#define	KTC_CTRL_CODE_MAX_LEN	7
typedef struct _KCtrlTable
{
	char    szCtrl[KTC_CTRL_CODE_MAX_LEN + 1];
	short	nCtrlLen;						
	short   nCtrl;								
}KCtrlTable;

static	const KCtrlTable	s_CtrlTable[] =
{	
	{ "enter",	5, KTC_ENTER		},
	{ "color",	5, KTC_COLOR		},
	{ "bclr",	4, KTC_BORDER_COLOR	},
	{ "pic",	3, KTC_INLINE_PIC	},
};

static	const int	s_nCtrlCount = sizeof(s_CtrlTable)/sizeof(KCtrlTable);

typedef struct _KColorTable
{
	char			Token[16];		//		
	unsigned char	Red;			
	unsigned char	Green;			
	unsigned char	Blue;			
}KColorTable;

static	const KColorTable	s_ColorTable[] =
{
	{ "Black",	0,		0,		0	},
	{ "White",	255,	255,	255	},
	{ "Red",	255,	0,		0	},
	{ "Green",	0,		255,	0	},
	{ "DGreen",	0x00,	0x64,	0x00 },
	{ "Blue",	100,	100,	255 },
	{ "DBlue",	120,	120,	120 },
	{ "Yellow",	255,	255,	0	},
	{ "DYellow", 78,	90,		40	},
	{ "Purple",	188,	64,		255	},
	{ "DPurple",	102,	51,		102 },
	{ "Gold",	234,	189,	11	},
	{ "DGold",	101,	80,		36	},
	{ "Orange",	227,	186,	0	},
	{ "DOrange",115,	77,		0	},
	{ "Cyan",	0,		255,	255	},
	{ "Metal",	246,	255,	117	},
	{ "DMetal",	128,	132,	76	},
	{ "Wood",	0,		255,	120	},
	{ "Water",	78,		124,	255	},
	{ "Fire",	255,	90,		0	},
	{ "Earth",	254,	207,	179	},
	{ "SGreen",	85,		255,	45	},
	{ "Pink",	255,	0,	204	},
	{ "OrangeRed",	255,	69,	0	},
	{ "LightSalmon", 255, 160, 122 },
	{ "Violet", 238, 130, 238 },
		
};

static	const int	s_nColorCount = sizeof(s_ColorTable)/sizeof(KColorTable);

#define MAXPICTOKENLEN	16

// Helper function to parse RGB color format: "R,G,B" or "R, G, B"
// Returns true if successfully parsed, false otherwise
static bool ParseRGBColor(const char* pColorStr, int nLen, unsigned char& r, unsigned char& g, unsigned char& b)
{
	if (!pColorStr || nLen <= 0 || nLen >= 16)
		return false;

	char temp[16];
	memcpy(temp, pColorStr, nLen);
	temp[nLen] = 0;

	// Check if it contains a comma (RGB format)
	if (strchr(temp, ',') == NULL)
		return false;

	int red = 0, green = 0, blue = 0;
	int count = sscanf(temp, "%d,%d,%d", &red, &green, &blue);

	if (count == 3 && red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
	{
		r = (unsigned char)red;
		g = (unsigned char)green;
		b = (unsigned char)blue;
		return true;
	}

	return false;
}

static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount);
static int  TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer);


extern "C" ENGINE_API
int	TEncodeText(char* pBuffer, int nCount)
{
	int nShortCount = 0;
	int nPrefixSize = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int		nReadPos = 0;		
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				if (nReadPos + 1 < nCount)
				{
					pBuffer[nShortCount++] = cCharacter;
					pBuffer[nShortCount++] = pBuffer[nReadPos+ 1];
					nReadPos += 2;
				}
				else
					break;
			}
			else if (cCharacter == 0x0d)
			{
				if (nReadPos + 1 < nCount && pBuffer[nReadPos + 1] == 0x0a)
					nReadPos += 2;
				else
					nReadPos ++;
				pBuffer[nShortCount++] = 0x0a;
			}
			else if (pBuffer[nReadPos] == '<')
				//TEncodeCtrl(pBuffer, nCount, nReadPos, nShortCount);
			{
				TEncodeCtrl(pBuffer, nCount, nReadPos, nShortCount);
			}
			else if((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				memmove((pBuffer + nShortCount), (pBuffer + nReadPos), 3);
				nShortCount += 3;
				nReadPos += 3;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
				int nTemp = *(int*)(pBuffer + nReadPos);
				*(int*)(pBuffer + nShortCount) = nTemp;
				nShortCount += 4;
				nReadPos += 4;
			}
			else
				nReadPos++;
		}
		if (nShortCount <nCount)
			pBuffer[nShortCount] = 0;
	}
	return nShortCount;

}

extern "C" ENGINE_API
int TFilterEncodedText(char* pBuffer, int nCount)
{
	int nShortCount = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int nReadPos = 0;
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				if (nReadPos + 1 < nCount)
				{
					pBuffer[nShortCount++] = cCharacter;
					pBuffer[nShortCount++] = pBuffer[nReadPos+ 1];
					nReadPos += 2;
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else if ((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
				if (nReadPos + 4 < nCount)
				{
					*(int*)(pBuffer + nShortCount) = *(int*)(pBuffer + nReadPos);
					nShortCount += 4;
					nReadPos += 4;
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				if ((int)(nReadPos + 1 + sizeof(WORD)) < nCount)
				{
					memcpy(pBuffer + nShortCount, pBuffer + nReadPos, 1 + sizeof(WORD));;
					nShortCount += 1 + sizeof(WORD);
					nReadPos += 1 + sizeof(WORD);
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else
				nReadPos ++;
		}
		if (nShortCount < nCount)
			pBuffer[nShortCount] = 0;
	}

	return nShortCount;
}

static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount)
{

	_ASSERT(pBuffer != NULL && nReadPos < nCount && nShortCount < nCount && nShortCount <= nReadPos);
	
	int nCtrlCodeSize, nEndPos, nCtrl;

	int nEqualPos = nReadPos + 1;
	for (; nEqualPos < nCount && nEqualPos <= nReadPos + KTC_CTRL_CODE_MAX_LEN; nEqualPos++)
		if (pBuffer[nEqualPos] == '>' || pBuffer[nEqualPos] == '=')
			break;	

	if(nEqualPos >= nCount || nEqualPos > nReadPos + KTC_CTRL_CODE_MAX_LEN)
		goto NO_MATCHING_CTRL;

	nCtrlCodeSize = nEqualPos - nReadPos - 1;

	for (nCtrl = 0; nCtrl < s_nCtrlCount; nCtrl++)
	{
		if (nCtrlCodeSize ==  s_CtrlTable[nCtrl].nCtrlLen &&
			memcmp(pBuffer + nReadPos + 1, s_CtrlTable[nCtrl].szCtrl, nCtrlCodeSize) == 0)
			break;
	}
	if (nCtrl >= s_nCtrlCount)
		goto NO_MATCHING_CTRL;
	nCtrl = s_CtrlTable[nCtrl].nCtrl;

	nEndPos = nEqualPos;
	if (pBuffer[nEqualPos] != '>')
	{
		for(nEndPos++; nEndPos < nCount; nEndPos++)
			if (pBuffer[nEndPos] == '>')
				break;
		if (nEndPos >= nCount)
			goto NO_MATCHING_CTRL;
		nShortCount += TEncodeCtrl(nCtrl, pBuffer + nEqualPos + 1,
			nEndPos - nEqualPos - 1, pBuffer + nShortCount);
	}
	else
		nShortCount += TEncodeCtrl(nCtrl, NULL, 0, pBuffer + nShortCount);
	nReadPos = nEndPos + 1;
	return true;

NO_MATCHING_CTRL:
	pBuffer[nShortCount++] = '<';
	nReadPos++;
	return false;
}

static int TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer)
{
	_ASSERT(pEncodedBuffer && (nParamLen == 0 || pParamBuffer != NULL));

	int nEncodedSize = 0;
	static char	Color[16];		//
	static char	szPic[MAXPICTOKENLEN];

	switch(nCtrl)
	{
	case KTC_ENTER:
		pEncodedBuffer[nEncodedSize ++] = nCtrl;
		break;
	case KTC_INLINE_PIC:	//[wxb 2003-6-19]
		if (nParamLen == 0 && nParamLen >= MAXPICTOKENLEN)
			break;
		{
			memcpy(szPic, pParamBuffer, nParamLen);
			szPic[nParamLen] = 0;
			pEncodedBuffer[nEncodedSize] = KTC_INLINE_PIC;
			*((WORD*)(pEncodedBuffer + nEncodedSize + 1)) = atoi(szPic);
			nEncodedSize += 1 + sizeof(WORD);
		}		
		break;
	case KTC_COLOR:
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_COLOR_RESTORE;
		}
		else if (nParamLen < 16)		//
		{
			unsigned char r, g, b;
			// First, try to parse as RGB format (e.g., "0,255,255")
			if (ParseRGBColor(pParamBuffer, nParamLen, r, g, b))
			{
				pEncodedBuffer[nEncodedSize] = KTC_COLOR;
				pEncodedBuffer[nEncodedSize + 1] = r;
				pEncodedBuffer[nEncodedSize + 2] = g;
				pEncodedBuffer[nEncodedSize + 3] = b;
				nEncodedSize += 4;
			}
			else
			{
				// If not RGB format, try to match named color
				memcpy(Color, pParamBuffer, nParamLen);
				Color[nParamLen] = 0;
				for (int i = 0; i < s_nColorCount; i++)
				{
#ifndef __linux
					if (_stricmp(Color,s_ColorTable[i].Token) == 0)
#else
					if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
					{
						pEncodedBuffer[nEncodedSize] = KTC_COLOR;
						pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
						pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
						pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
						nEncodedSize += 4;
						break;
					}
				}
			}
		}
		break;
	case KTC_BORDER_COLOR:
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_BORDER_RESTORE;
		}
		else if (nParamLen < 16)		//
		{
			unsigned char r, g, b;
			// First, try to parse as RGB format (e.g., "0,255,255")
			if (ParseRGBColor(pParamBuffer, nParamLen, r, g, b))
			{
				pEncodedBuffer[nEncodedSize] = KTC_BORDER_COLOR;
				pEncodedBuffer[nEncodedSize + 1] = r;
				pEncodedBuffer[nEncodedSize + 2] = g;
				pEncodedBuffer[nEncodedSize + 3] = b;
				nEncodedSize += 4;
			}
			else
			{
				// If not RGB format, try to match named color
				memcpy(Color, pParamBuffer, nParamLen);
				Color[nParamLen] = 0;
				for (int i = 0; i < s_nColorCount; i++)
				{
#ifndef __linux
					if (_stricmp(Color,s_ColorTable[i].Token) == 0)
#else
					if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
					{
						pEncodedBuffer[nEncodedSize] = KTC_BORDER_COLOR;
						pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
						pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
						pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
						nEncodedSize += 4;
						break;
					}
				}
			}
		}
		break;
	}
	return nEncodedSize;
}

extern "C" ENGINE_API
int	TRemoveCtrlInEncodedText(char* pBuffer, int nCount)
{
	int nLen = 0;
	nCount = TFilterEncodedText(pBuffer, nCount);
	for (int nPos = 0; nPos < nCount; nPos++)
	{
		char cCharacter = pBuffer[nPos];
		if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			nPos += 3;
		else if (cCharacter == KTC_INLINE_PIC)
			nPos += sizeof(WORD);
		else if (cCharacter != KTC_COLOR_RESTORE && cCharacter != KTC_BORDER_RESTORE)
		{
			pBuffer[nLen] = cCharacter;
			nLen ++;
		}
	}
	return nLen;
}

//????????i???????????????
//??????pBuffer			?i???????
//		nCount			?i?????????
//		nWrapCharaNum	????ÿ?????????????????
//		nMaxLineLen		???????i??????????????????????
//		nFontSize		????????J?? [wxb 2003-6-19]
//		nSkipLine		????j????????????
//		nNumLineLimit	?????i????????????????????????????????????????????????0??????????o?
//??????i???????
//extern "C" ENGINE_API
//int	TGetEncodedTextLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize, int nSkipLine = 0, int nNumLineLimit = 0)
extern "C" ENGINE_API
int	TGetEncodedTextLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize, int nSkipLine, int nNumLineLimit,
							 BOOL bPicSingleLine/* = FALSE*/)
{
	//??h???????ó??? [wxb 2003-6-20]
	_ASSERT(nFontSize >= 4 && nFontSize < 64);
	nFontSize = max(4, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	nMaxLineLen = 0;
	if (pBuffer == 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int nNumLine = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;
	if (nSkipLine < 0)
		nSkipLine = 0;
	if (nNumLineLimit <= 0)
		nNumLineLimit = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	int  nExtraLineForInlinePic = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//?????????????
		{
			nPos += 2;
			if (fNumChars + 2 < nWrapCharaNum)
				fNumChars += 2;
			else if (fNumChars + 2 == nWrapCharaNum || fNumChars == 0)
			{
				bNextLine = true;
				fNumChars += 2;
			}
			else
			{
				bNextLine = true;
				fNumNextLineChar = 2;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//???????
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//?????????? [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
			if (nSkipLine > 0)
			{
				nSkipLine -= 1 + nExtraLineForInlinePic;

				//??????????????? [wxb 2003-6-19]
				if (nSkipLine < 0)
				{
					if (fMaxLineLen < fNumChars)
						fMaxLineLen = fNumChars;
					nNumLine += (-nSkipLine);
					if (nNumLine >= nNumLineLimit)
						break;
				}
			}
			else
			{
				if (fMaxLineLen < fNumChars)
					fMaxLineLen = fNumChars;
				nNumLine += 1 + nExtraLineForInlinePic;
				if (nNumLine >= nNumLineLimit)
					break;
			}
			nExtraLineForInlinePic = 0;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}
	if (nNumLine < nNumLineLimit && fNumChars && nSkipLine == 0)
	{
		if (fMaxLineLen < fNumChars)
			fMaxLineLen = fNumChars;
		nNumLine += 1 + nExtraLineForInlinePic;
	}

	nMaxLineLen = (int)(fMaxLineLen + (float)0.9999);	//??1
	return nNumLine;
}

//??????????'???
int TGetEncodeStringLineHeadPos(const char* pBuffer, int nCount, int nLine, int nWrapCharaNum, int nFontSize, BOOL bPicSingleLine)
{
	//??h???????ó??? [wxb 2003-6-20]
	_ASSERT(nFontSize > 1 && nFontSize < 64);
	nFontSize = max(1, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	if (pBuffer == 0 || nLine <= 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int  nExtraLineForInlinePic = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//?????????????
		{
			nPos += 2;
			if (fNumChars + 2 < nWrapCharaNum)
				fNumChars += 2;
			else if (fNumChars + 2 == nWrapCharaNum || fNumChars == 0)
			{
				bNextLine = true;
				fNumChars += 2;
			}
			else
			{
				bNextLine = true;
				fNumNextLineChar = 2;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//???????
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//?????????? [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
//			if (nSkipLine > 0)
//			{
//				nSkipLine -= 1 + nExtraLineForInlinePic;
//
//				//??????????????? [wxb 2003-6-19]
//				if (nSkipLine < 0)
//				{
//					if (fMaxLineLen < fNumChars)
//						fMaxLineLen = fNumChars;
//					nNumLine += (-nSkipLine);
//					if (nNumLine >= nNumLineLimit)
//						break;
//				}
//			}
			if ((--nLine) == 0)
				break;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}

	return nPos;
}

extern "C" ENGINE_API
//????(?????????)????????????????ß?????????????????????????????..???
const char* TGetLimitLenEncodedString(const char* pOrigString, int nOrigLen, int nFontSize,
	int nWrapCharaNum, char* pLimitLenString, int& nShortLen, int nLineLimit, int bPicPackInSingleLine/*=false*/)
{
	if (pOrigString == NULL || pLimitLenString == NULL ||
		nOrigLen <= 0 || nShortLen < 2 || nLineLimit < 1 || nWrapCharaNum < 2)
	{
		nShortLen = 0;
		return NULL;
	}

	int nPreLineEndPos = 0, nFinalLineEndPos;
	if (nLineLimit > 1)	//????j????
	{
		nPreLineEndPos = TGetEncodeStringLineHeadPos(pOrigString, nOrigLen, nLineLimit - 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine);
		if (nPreLineEndPos > nShortLen)
		{
			nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen - 2, true);
			memcpy(pLimitLenString, pOrigString, nShortLen);
			pLimitLenString[nShortLen] = '.';
			pLimitLenString[nShortLen + 1] = '.';
			nShortLen += 2;
			return pLimitLenString;
		}
	}

	if (nPreLineEndPos < nOrigLen)
	{
		nFinalLineEndPos = TGetEncodeStringLineHeadPos(pOrigString + nPreLineEndPos,
			nOrigLen - nPreLineEndPos, 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine) + nPreLineEndPos;
	}
	else
		nFinalLineEndPos = nOrigLen;

	if (nFinalLineEndPos >= nOrigLen)
	{
		nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen, true);
		memcpy(pLimitLenString, pOrigString, nShortLen);
		return pLimitLenString;
	}

	int nDesireLen = (nFinalLineEndPos <= nShortLen) ? nFinalLineEndPos - 2 : nShortLen - 2;

	const char* pFinalLineHead = pOrigString + nPreLineEndPos;
	int nRemainCount = nOrigLen - nPreLineEndPos;
	nDesireLen -= nPreLineEndPos;
	while(true)
	{
		nShortLen = TSplitEncodedString(pFinalLineHead, nRemainCount, nDesireLen, true);
		int nMaxLineLen;
		TGetEncodedTextLineCount(pFinalLineHead, nShortLen, 0, nMaxLineLen, nFontSize, 0, 1, FALSE);
		if (nMaxLineLen <= nWrapCharaNum - 2)
			break;
		nDesireLen--;
	}
	nShortLen += nPreLineEndPos;

   	memcpy(pLimitLenString, pOrigString, nShortLen);
	pLimitLenString[nShortLen] = '.';
	pLimitLenString[nShortLen + 1] = '.';
	nShortLen += 2;
	return pLimitLenString;
}


//	?????????????????????????ß?????????????????????????????..???
//	??????pOrigString     // --> ?????????????????
//		??nOrigLen		  // --> ???????????????????ß????
//		  pLimitLenString // --> ???????????????????????????????????L????????????????
//		  nLimitLen		  // --> ????????????????????3
//	?????????????????????????????????????????????????????????????L??????????
//	????Chinese GBK??????????????????????????????????????????????

extern "C" ENGINE_API
const char* TGetLimitLenString(const char* pOrigString, int nOrigLen, char* pLimitLenString, int nLimitLen)
{
	if (pOrigString && pLimitLenString && nLimitLen > 0)
	{
		if (nOrigLen < 0)
			nOrigLen = strlen(pOrigString);
		if (nOrigLen < nLimitLen)
			return pOrigString;
		if (nLimitLen > 2)
		{
			nOrigLen = TSplitString(pOrigString, nLimitLen - 3, true);
			memcpy(pLimitLenString, pOrigString, nOrigLen);
			pLimitLenString[nOrigLen] = '.';
			pLimitLenString[nOrigLen + 1] = '.';
			pLimitLenString[nOrigLen + 2] = 0;
		}
		else if (nLimitLen == 2)
		{
			pLimitLenString[0] = '.';
			pLimitLenString[1] = 0;
		}
		return ((nLimitLen >= 2) ? pLimitLenString : NULL);
	}
	return NULL;
}


//???????????i??????????ÿ?'??????????????w???ã?????-1???d???
extern "C" ENGINE_API
int	TFindSpecialCtrlInEncodedText(const char* pBuffer, int nCount, int nStartPos, char cControl)
{
	int nFindPos = -1;
	if (pBuffer)
	{
		while(nStartPos < nCount)
		{
			unsigned char cCharacter = pBuffer[nStartPos];
			if ((unsigned char)cControl == cCharacter)
			{
				nFindPos = nStartPos;
				break;
			}
			if (cCharacter > 0x80)	//??????????????
				nStartPos += 2;
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				nStartPos += 4;
			else if (cCharacter == KTC_INLINE_PIC)
				nStartPos += 3;
			else
				nStartPos ++;
		}
	}
	return nFindPos;
}

//???????????i??????????????????
extern "C" ENGINE_API
int	TClearSpecialCtrlInEncodedText(char* pBuffer, int nCount, char cControl)
{
	int nFinalLen = 0;
	int nReadPos = 0;
	if (pBuffer)
	{
		if ((unsigned char)(cControl) <= 0x80)
		{
			int nMatchLen = 1;
			if (cControl == KTC_COLOR || cControl == KTC_BORDER_COLOR)
				nMatchLen = 4;
			else if (cControl == KTC_INLINE_PIC)
				nMatchLen = 3;
			while(nReadPos < nCount)
			{
				unsigned char cCharacter = pBuffer[nReadPos];
				if ((unsigned char)cControl == cCharacter)
				{
					nReadPos += nMatchLen;
				}
				else if (cCharacter > 0x80)
				{
					short sTemp = *(short*)(pBuffer + nReadPos);
					*(short*)(pBuffer + nFinalLen) = sTemp;
					nReadPos += 2;
					nFinalLen += 2;
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					int nTemp = *(int*)(pBuffer + nReadPos);
					*(int*)(pBuffer + nFinalLen) = nTemp;
					nFinalLen += 4;
					nReadPos += 4;
				}
				else if (cCharacter == KTC_INLINE_PIC)
				{
					memmove((pBuffer + nFinalLen), (pBuffer + nReadPos), 3);
					nFinalLen += 3;
					nReadPos += 3;
				}
				else
				{
					pBuffer[nFinalLen++] = pBuffer[nReadPos++];
				}
			}
		}
	}
	return nFinalLen;
}

//???????????i??????????????????????????
extern "C" ENGINE_API
int TGetEncodedTextOutputLenPos(const char* pBuffer, int nCount, int& nLen, bool bLess, int nFontSize)
{
	int nIndex = 0, nLenTemp = 0;

	_ASSERT(nFontSize >= 4);
	nFontSize = max(4, nFontSize);

    if (pBuffer)
	{
		int nWidth, nHeight;
		int nByteCount = 0, nCurCharLen = 0;
	    unsigned char cCharacter        = 0;

		while(nLenTemp < nLen)
		{
			cCharacter = pBuffer[nIndex];
			//???????j????????????nByteCount????????????????????nCurCharLen
			if (cCharacter > 0x80)	//??????????????
			{
                nByteCount  = 2;
				nCurCharLen = 2;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
			    nByteCount  = 4;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_COLOR_RESTORE && cCharacter == KTC_BORDER_RESTORE)
			{
				nByteCount  = 1;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				nByteCount  = 3;
				if(SUCCEEDED(g_pIInlinePicSink->GetPicSize(
					*((unsigned short *)(pBuffer + nIndex + 1)), nWidth, nHeight)))
				{
					nCurCharLen = ((nWidth * 2 + nFontSize - 1) /  nFontSize);
				}
				else
					nCurCharLen = 0;
			}
			else
			{
				nByteCount  = 1;
				nCurCharLen = 1;
			}

			//?????????????????????
			if(nIndex + nByteCount > nCount)
				break;
			//????????û??????????
			if(nLenTemp + nCurCharLen < nLen)
			{
				nLenTemp += nCurCharLen;
		        nIndex   += nByteCount;
			}
			//??????????????????
			else if(nLenTemp + nCurCharLen == nLen)
			{
				nLenTemp += nCurCharLen;
				nIndex   += nByteCount;
				break;
			}
			//????????????
			else
			{
				nLenTemp = bLess ? nLenTemp : (nLenTemp + nCurCharLen);
				nIndex   = bLess ? nIndex   : (nIndex + nByteCount);
				break;
			}
		}
	}
	nLen = nLenTemp;
	return nIndex;
}

//???????????i????????j??????????????????????????????????
extern "C" ENGINE_API
int TGetEncodedTextEffectCtrls(const char* pBuffer, int nSkipCount, KTP_CTRL& Ctrl0, KTP_CTRL& Ctrl1)
{
	int nIndex = 0;
	Ctrl0.cCtrl = Ctrl1.cCtrl = KTC_INVALID;
	if (pBuffer)
	{
		KTP_CTRL PreCtrl0, PreCtrl1;
		PreCtrl0.cCtrl = PreCtrl1.cCtrl = KTC_INVALID;

		while(nIndex < nSkipCount)
		{
			unsigned char cCharacter = pBuffer[nIndex];
			if (cCharacter == KTC_COLOR)
			{
				PreCtrl0  =  Ctrl0;
				*(int*)(&Ctrl0) = *(int*)(pBuffer + nIndex);
				nIndex += 4;				
			}
			else if (cCharacter == KTC_BORDER_COLOR)
			{
				PreCtrl1  =  Ctrl1;
				*(int*)(&Ctrl1) = *(int*)(pBuffer + nIndex);
				nIndex += 4;
			}
			else if(cCharacter == KTC_COLOR_RESTORE)
			{
				Ctrl0 = PreCtrl0;
				nIndex ++;
			}
			else if(cCharacter == KTC_BORDER_RESTORE)
			{
				Ctrl1 = PreCtrl1;
				nIndex ++;
			}
			else
				nIndex ++;
		}
	}
	return nIndex;
}

extern "C" ENGINE_API
int	TGetChatItemLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize,
	int& nFace, int& nLastPos, int& nTotalLen, int nSkipLine, int nNumLineLimit, bool bPicSingleLine/* = false*/ )
{
	_ASSERT(nFontSize >= 4 && nFontSize < 64);
	nFontSize = max(4, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	nMaxLineLen = 0;
	if (pBuffer == 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);
	nFace = 0;
	nLastPos = 0;
	nTotalLen = 0;
	float fNumChars = 0;
	int nNumLine = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;
	if (nSkipLine < 0)
		nSkipLine = 0;
	if (nNumLineLimit <= 0)
		nNumLineLimit = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	int  nExtraLineForInlinePic = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)
		{
			nPos += 2;
			nLastPos += 2;
			nTotalLen += 2;
			if (fNumChars + 2 < nWrapCharaNum)
				fNumChars += 2;
			else if (fNumChars + 2 == nWrapCharaNum || fNumChars == 0)
			{
				bNextLine = true;
				fNumChars += 2;
			}
			else
			{
				bNextLine = true;
				fNumNextLineChar = 2;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			nFace++;
			nTotalLen+=3;
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			nLastPos++;//
			nTotalLen++;//
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
			//nLastPos++;//
			//nLastPos++;//
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
			if (nSkipLine > 0)
			{
				nSkipLine -= 1 + nExtraLineForInlinePic;

				if (nSkipLine < 0)
				{
					if (fMaxLineLen < fNumChars)
						fMaxLineLen = fNumChars;
					nNumLine += (-nSkipLine);
					nLastPos = 0;//
					if (cCode != KTC_INLINE_PIC)
					nFace = 0;//
					else nFace = 1;
					if (nNumLine >= nNumLineLimit)
						break;
				}
			}
			else
			{
				if (fMaxLineLen < fNumChars)
					fMaxLineLen = fNumChars;
				nNumLine += 1 + nExtraLineForInlinePic;
				nLastPos = 0;//
				if (cCode != KTC_INLINE_PIC)
				nFace = 0;//
				else nFace = 1;
				if (nNumLine >= nNumLineLimit)
					break;
			}
			nExtraLineForInlinePic = 0;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}
	if (nNumLine < nNumLineLimit && fNumChars && nSkipLine == 0)
	{
		if (fMaxLineLen < fNumChars)
			fMaxLineLen = fNumChars;
		nNumLine += 1 + nExtraLineForInlinePic;
	}

	nMaxLineLen = (int)(fMaxLineLen + (float)0.9999);
	return nNumLine;
}