#ifndef _CFileH_
#define _CFileH_
#define SEEK_ERROR		0xFFFFFFFF
class ENGINE_API KFile
{
private:
	FILE*		m_hFile;	// File Handle
	DWORD		m_dwLen;	// File Size
	DWORD		m_dwPos;	// File Pointer

public:
	KFile();
	~KFile();
	BOOL		Open(LPSTR FileName);
	BOOL		Create(LPSTR FileName);
	BOOL		Append(LPSTR FileName);
	void		Close();
	DWORD		Read(LPVOID lpBuffer, DWORD dwReadBytes);
	DWORD		Write(LPVOID lpBuffer, DWORD dwWriteBytes);
	DWORD		Seek(LONG lDistance, DWORD dwMoveMethod);
	DWORD		Tell();
	DWORD		Size();
};
#endif
