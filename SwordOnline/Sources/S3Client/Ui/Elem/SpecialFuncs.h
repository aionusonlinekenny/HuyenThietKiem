#pragma once

#include "../../../Represent/iRepresent/iRepresentShell.h"

struct KOutputTextParam;
int OutputTabSplitText(const char* pBuffer, int nCount, int nLineWidth,
					   int nFontId, KOutputTextParam* pParam);
void SaveScreenToFile(ScreenFileType eType = SCRFILETYPE_JPG, unsigned int nQuality = 100);
bool SetScrPicPath(const char* szPath);
char* GetSrcPicPath();