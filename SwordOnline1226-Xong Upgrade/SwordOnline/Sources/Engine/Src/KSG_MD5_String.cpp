//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_MD5_String.cpp
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2003-7-29 17:53:17
//  Comment     :   Conver String to MD5 String
//
//////////////////////////////////////////////////////////////////////////////////////

#include "KWin32.h"

#include "md5.h"
#include "KSG_MD5_String.h"
#include <string.h>

int KSG_StringToMD5String(char szDestMD5String[64], const char cszSrcString[])
{
    int nResult = false;
    md5_state_t md5_state;
    unsigned char MD5Value[16];
    int nSrcStringLen = 0;

    if (!szDestMD5String)
        goto Exit0;

    szDestMD5String[0] = '\0';

    if  (!cszSrcString)
       goto Exit0;

    nSrcStringLen = strlen(cszSrcString);

    if (!nSrcStringLen)
        goto Exit0;

    md5_init(&md5_state);

    md5_append(&md5_state, (unsigned char *)cszSrcString, nSrcStringLen);

    md5_finish(&md5_state, MD5Value);


    sprintf(
        szDestMD5String,
        "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        MD5Value[ 0], MD5Value[ 1], MD5Value[ 2], MD5Value[ 3],
        MD5Value[ 4], MD5Value[ 5], MD5Value[ 6], MD5Value[ 7],
        MD5Value[ 8], MD5Value[ 9], MD5Value[10], MD5Value[11],
        MD5Value[12], MD5Value[13], MD5Value[14], MD5Value[15]
    );

    nResult = true;
Exit0:
    return nResult;
}

// --
//
// --
bool KSG_StringToMD5String2(char szDestMD5String[64], const char cszSrcString[])
{	
	bool nResult = false;
    md5_state_t md5_state;
	char szTempBuffer[64];
    unsigned char MD5Value[16];
    int nSrcStringLen = 0;

	if (!szDestMD5String)
        goto Exit0;

    if  (!cszSrcString)
       goto Exit0;

    nSrcStringLen = strlen(cszSrcString);

    if (!nSrcStringLen)
        goto Exit0;

    md5_init(&md5_state);

    md5_append(&md5_state, (unsigned char *)cszSrcString, nSrcStringLen);

    md5_finish(&md5_state, MD5Value);

    sprintf(
        szDestMD5String,
        "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        MD5Value[ 0], MD5Value[ 1], MD5Value[ 2], MD5Value[ 3],
        MD5Value[ 4], MD5Value[ 5], MD5Value[ 6], MD5Value[ 7],
        MD5Value[ 8], MD5Value[ 9], MD5Value[10], MD5Value[11],
        MD5Value[12], MD5Value[13], MD5Value[14], MD5Value[15]
    );
	
	ZeroMemory(szTempBuffer, sizeof(szTempBuffer));

	if  (!szTempBuffer)
       goto Exit0;

    nSrcStringLen = strlen(szDestMD5String);

    if (!nSrcStringLen)
        goto Exit0;

    md5_init(&md5_state);

    md5_append(&md5_state, (unsigned char *)szDestMD5String, nSrcStringLen);

    md5_finish(&md5_state, MD5Value);
	
	sprintf(
        szTempBuffer,
        "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        MD5Value[ 0], MD5Value[ 1], MD5Value[ 2], MD5Value[ 3],
        MD5Value[ 4], MD5Value[ 5], MD5Value[ 6], MD5Value[ 7],
        MD5Value[ 8], MD5Value[ 9], MD5Value[10], MD5Value[11],
        MD5Value[12], MD5Value[13], MD5Value[14], MD5Value[15]
    );

	ZeroMemory(szDestMD5String, sizeof(szDestMD5String));

	if  (!szDestMD5String)
       goto Exit0;

    nSrcStringLen = strlen(szTempBuffer);

    if (!nSrcStringLen)
        goto Exit0;

    md5_init(&md5_state);

    md5_append(&md5_state, (unsigned char *)szTempBuffer, nSrcStringLen);

    md5_finish(&md5_state, MD5Value);
	
	sprintf(
        szDestMD5String,
        "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        MD5Value[ 0], MD5Value[ 1], MD5Value[ 2], MD5Value[ 3],
        MD5Value[ 4], MD5Value[ 5], MD5Value[ 6], MD5Value[ 7],
        MD5Value[ 8], MD5Value[ 9], MD5Value[10], MD5Value[11],
        MD5Value[12], MD5Value[13], MD5Value[14], MD5Value[15]
    );

	nResult = true;

Exit0:
    return nResult;
}
