#include "KWin32.h"
#include <windows.h>
#include "ErrorCode.h"
#include "KEngine.h"
#include "KiniFile.h"

#define ERROR_STRING		"\\UI\\ErrorCode.ini"
#define	NEXT_LINE_CHARA		'~'

static unsigned int s_uErrorCode = 0;
static char			s_szErrorString[32] = "";

// --
//
// --
void Error_SetErrorCode(unsigned int uCode)
{
	s_uErrorCode = uCode;
}

// --
//
// --
void Error_SetErrorString(const char* pcszString)
{
	strncpy(s_szErrorString, pcszString, sizeof(s_szErrorString));
	s_szErrorString[sizeof(s_szErrorString) - 1] = 0;
}

// --
//
// --
void Error_Box()
{
	if(s_uErrorCode)
	{
    	KIniFile Ini;
    	char szFormatString[512], szOutputInfo[512], szBuf[16];

	    Ini.Load(ERROR_STRING);

        Ini.GetString("Strings", _itoa(s_uErrorCode,szBuf,10), "", szFormatString, sizeof(szFormatString));
		if(szFormatString[0] == 0)
		{
			Ini.GetString("Strings", "Unknown", "", szFormatString, sizeof(szFormatString));
			if(szFormatString[0] == 0)
			    strcpy(szFormatString, "Î´ÖªµÄ´íÎó(´íÎóºÅ: %d)");
		}
		while(true)
		{
			char* pszNextLine = strchr(szFormatString, NEXT_LINE_CHARA);
			if (pszNextLine == NULL)
				break;
			*pszNextLine = '\n';
		}
		const char* pszCodePos = strstr(szFormatString, "%d");
		const char* pszStringPos = strstr(szFormatString, "%s");
		if (pszCodePos)
		{
			if (pszStringPos)
			{
				if (pszCodePos < pszStringPos)
					sprintf(szOutputInfo, szFormatString, s_uErrorCode, s_szErrorString);
				else
					sprintf(szOutputInfo, szFormatString, s_szErrorString, s_uErrorCode);
			}
			else
			{
				sprintf(szOutputInfo, szFormatString, s_uErrorCode);
			}
		}
		else if (pszStringPos)
		{
			sprintf(szOutputInfo, szFormatString, s_szErrorString);
		}
		else
			strcpy(szOutputInfo, szFormatString);

	    MessageBox(NULL, szOutputInfo, "SwordMan", MB_OK | MB_ICONERROR);

	    s_uErrorCode = 0;
	}
	s_szErrorString[0] = 0;
}
