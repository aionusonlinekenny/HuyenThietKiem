#include <windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")
	
class ENGINE_API KLicense
{
private:

	void	GetMacAddress(char* pszLicense, bool bServer = true);
	void	GetName(char* lpszName);

public:
	KLicense();
	~KLicense();
	void	GetLicense(char* pszBuffer, bool bServer = true);

private:
	int		m_btIndexID;
};

ENGINE_API void g_pClientLic(char* pszBuffer);