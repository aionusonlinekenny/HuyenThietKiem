
#ifndef _CPakFileH_
#define _CPakFileH_
#include "KFile.h"
#include "XPackFile.h"

ENGINE_API void g_SetPakFileMode(int nFileMode);

class ENGINE_API KPakFile
{
public:
	KPakFile();
	~KPakFile();
	BOOL		Open(const char* pszFileName);
	void		Close();
	bool		IsFileInPak();
	DWORD		Read(void* pBuffer, unsigned int uSize);
	DWORD		Seek(int nOffset, unsigned int uMethod);
	DWORD		Tell();
	DWORD		Size();
#ifdef _DEBUG
	BOOL		Open4Unpack(const char* pszFileName, int nIndex);
#endif
#ifdef _DEBUG
	BOOL		Save(const char* pszFileName);
#endif

private:
	KFile				m_File;
	XPackElemFileRef	m_PackRef;
};
#ifndef _SERVER
	struct KSGImageContent
	{
		int				nWidth;
		int				nHeight;
		unsigned short	Data[1];
	};
	#define	KSG_IMAGE_CONTENT_SIZE(w, h)    ((unsigned)((&((KSGImageContent *)0)->Data[0])) + w * h * 2)

	ENGINE_API SPRHEAD*		SprGetHeader(const char* pszFileName, SPROFFS*& pOffsetTable);
	ENGINE_API void			SprReleaseHeader(SPRHEAD* pSprHeader);
	ENGINE_API SPRFRAME*	SprGetFrame(SPRHEAD* pSprHeader, int nFrame);
	ENGINE_API void			SprReleaseFrame(SPRFRAME* pFrame);
	ENGINE_API KSGImageContent *get_jpg_image(const char cszName[], unsigned uRGBMask16 = ((unsigned)-1));
	ENGINE_API void release_image(KSGImageContent *pImage);
#endif

#endif
