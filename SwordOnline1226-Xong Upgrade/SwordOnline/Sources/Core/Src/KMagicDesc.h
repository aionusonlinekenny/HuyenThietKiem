#ifndef KMagicDescH
#define	KMagicDescH

#include "KIniFile.h"

class KMagicDesc
{
private:
	KIniFile	m_IniFile;
	char		m_szDesc[512];
public:
	KMagicDesc();
	~KMagicDesc();
	BOOL		Init();
	const char*	GetDesc(void* pData);
	int	String2MagicID(char * szMagicAttribName); //AutoAI by kinnox;
};

extern KMagicDesc	g_MagicDesc;
#endif
