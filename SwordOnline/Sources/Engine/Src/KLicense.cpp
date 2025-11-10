#include "KWin32.h"
#include "KEngine.h"
#include "KLicense.h"
#include "KSG_MD5_String.h"
#include "KProtocolDef.h"
#include <stdlib.h>
#include "KWin32Wnd.h"
#include "KIniFile.h"
#include "KStrBase.h"

#define	MAX_LEN_STRING		32

//-
//
//-
ENGINE_API KLicense* g_pLicense = NULL;

//-
//
//-
KLicense::KLicense()
{
	g_pLicense		= this;
	m_btIndexID		= 65539;
}

//-
//
//-
KLicense::~KLicense()
{
	g_pLicense = this;
}

//-
//
//-
void	KLicense::GetMacAddress(char* pszLicense, bool bServer /*= true*/)
{
	IP_ADAPTER_INFO AdapterInfo[16];      
                                         
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(      
	AdapterInfo,                 
	&dwBufLen);                  
	assert(dwStatus == ERROR_SUCCESS);  
                                      

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; 
                                               
	do
	{	
		if(bServer)
		{
			if(pAdapterInfo->Index == unsigned(m_btIndexID))
			{
				sprintf(pszLicense, "%02X%02X%02X%02X%02X%02X", 
								pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2], pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
				break;
			}
		}
		else
		{
			sprintf(pszLicense, "%02X%02X%02X%02X%02X%02X", 
								pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2], pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			break;
		}
		pAdapterInfo = pAdapterInfo->Next;
	}
	while(pAdapterInfo);
	
}

//
//
//
void	KLicense::GetName(char* lpszName)
{	
	char* pValue = NULL;

	TCHAR	szBuffer[16];
	DWORD	dwSize = sizeof(szBuffer)/sizeof(TCHAR);
	
	GetComputerName(szBuffer, &dwSize);

	g_StrCpyLen(lpszName, szBuffer, 16);
	if(!lpszName[0])
	{	
		DestroyWindow(g_GetMainHWnd());
	}
		
}

//
//
//
void	KLicense::GetLicense(char* pszBuffer, bool bServer /*= true*/)
{	
	char szMacAddress[16];
	GetMacAddress(szMacAddress, bServer);
	if(!szMacAddress[0])
	{	
		DestroyWindow(g_GetMainHWnd());
	}

	char szComputerName[16];
	GetName(szComputerName);

	int nLen = strlen(szMacAddress) + strlen(szComputerName);
	if(nLen > MAX_LEN_STRING)
	{
		DestroyWindow(g_GetMainHWnd());
	}
	char szTemp[MAX_LEN_STRING];
	memcpy(szTemp, szMacAddress, sizeof(szMacAddress));
	strcat(szTemp, szComputerName);

	memcpy(pszBuffer, szTemp, sizeof(szTemp));
}


//// --
//
//// --
void g_pClientLic(char* pszBuffer)
{	
	KLicense	g_pLic;
	g_pLic.GetLicense(pszBuffer, false);
}