#include "StdAfx.h"
#include "KSOServer.h"
#include "iostream.h"
// moi them
#include <windows.h> 
#include <winsock2.h>
#include <ws2tcpip.h>
// end
#include <stdio.h>
#include <string>
#include <conio.h>

#include "tstring.h"
#include "Exception.h"
#include <sstream>
#include "../Engine/Src/KSG_MD5_String.h"
#include "KLicense.h"
using namespace std;
using OnlineGameLib::Win32::_tstring;
using OnlineGameLib::Win32::CException;

extern KSwordOnLineSever g_SOServer;
int g_nPort = 0;

_tstring GetCurrentDirectory()
{
   DWORD size = ::GetCurrentDirectory( 0, 0 );

   auto_ptr<TCHAR> spBuf(new TCHAR[size]);

   if ( 0 == ::GetCurrentDirectory( size, spBuf.get() ) )
   {
      throw CException( _T("GetCurrentDirectory()"), _T("Failed to get current directory") );
   }

   return _tstring( spBuf.get() );
}

int KinnoxHx(char* szMD5)
{
	DWORD HexCode;
	if (szMD5)
	{
		DWORD Id = 0;
		for (int i = 0; i<6; i++)
		{
			Id = (Id + (i + 1) * szMD5[i]) % 0x8000000b * 0xffffffef;
		}
		HexCode = Id ^ 0x987654321;
	}
	memset(szMD5,0,sizeof(szMD5));
	return HexCode;
}

string GetMachineID()
{
    // LPCTSTR szHD = "C:\\";  // ERROR
    string ss;
    ss = "Err_StringIsNull";
    UCHAR szFileSys[255],
        szVolNameBuff[255];
    DWORD dwSerial;
    DWORD dwMFL;
    DWORD dwSysFlags;
    int error = 0;

    int success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)szVolNameBuff,
        255, &dwSerial,
        &dwMFL, &dwSysFlags,
        (LPTSTR)szFileSys,
        255);
    if (!success) {
        ss = "Err_Not_Elevated";
    }
    std::stringstream errorStream;
    errorStream << dwSerial;
    return string(errorStream.str().c_str());
}

int main(int argc, char* argv[])
{
	HANDLE hInput;
    DWORD prev_mode;
    const ENABLE_EXTENDED_FLAGS = 0x0080;
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput, &prev_mode); 
    SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);

	system("color B");
	char szMD5[256];
	char char_directHW[256];
	string drirectHW = GetCurrentDirectory();
	strcpy(char_directHW, drirectHW.c_str());
	KSG_StringToMD5String(szMD5, char_directHW);

	char szMD5HW[256];
	char char_HWare[256];
	HW_PROFILE_INFO hwProfileInfo;
	string szHware;
	if(GetCurrentHwProfile(&hwProfileInfo))
	{
		szHware = hwProfileInfo.szHwProfileGuid;
	}
	else
	{
		return 0; 
	}

	strcpy(char_HWare, szHware.c_str());
	KSG_StringToMD5String(szMD5HW,char_HWare);

	char char_GetMachineID[256];
	char char_MachineID[256];
	string drirectGetMachineID = GetMachineID();
	strcpy(char_MachineID, drirectGetMachineID.c_str());
	KSG_StringToMD5String(char_GetMachineID,char_MachineID);

	char szLicense[128];
	char md5_szLicense[128];
	g_pClientLic(szLicense);
	KSG_StringToMD5String(md5_szLicense,szLicense);
	
/*	if (KinnoxHx(szMD5) != LINE_GS)
		return 0;

	if (KinnoxHx(szMD5HW) != LINE_HW)
		return 0;

	if (KinnoxHx(char_GetMachineID) != LINE_MCID)
		return 0;

	if (KinnoxHx(md5_szLicense) != LINE_MAC)
		return 0;*/
#ifdef _DEBUG
	cout<<KinnoxHx(szMD5)<<endl;
	cout<<KinnoxHx(szMD5HW)<<endl;
	cout<<KinnoxHx(char_GetMachineID)<<endl;
	cout<<KinnoxHx(md5_szLicense)<<endl;
#endif 
	memset(szMD5,0,sizeof(szMD5));
	memset(char_directHW,0,sizeof(char_directHW));
	memset(szMD5HW,0,sizeof(szMD5HW));
	memset(char_HWare,0,sizeof(char_HWare));

	/*if(CHECK_PASSWORD)
	{
		unsigned int NumLic;
		cin>>NumLic;
		NumLic += (123^0x12345678);
		system("cls");
		if (NumLic != (CHECK_PASSWORD/2)+(123^0x12345678))
		{
			system("date /t");
			system("time /t");
			//system("color 0c");
			return 0;
		}
	}*/
	BOOL bRunning = TRUE;
	if (argc == 2)
	{
		g_nPort = atoi(argv[1]);
	}

	if (!g_SOServer.Init())
		return 0;

	while(bRunning)
	{
		bRunning = g_SOServer.Breathe();
	}
	g_SOServer.Release();

	return 1;
}

#ifdef _STANDALONE
extern "C"
{
void lua_outerrmsg(const char * szerrmsg)
{
	fprintf(stderr, szerrmsg);
}
};
#endif
