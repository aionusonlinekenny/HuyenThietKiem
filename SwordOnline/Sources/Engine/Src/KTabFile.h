
#ifndef KTabFile_H
#define KTabFile_H
#include "KMemClass.h"
#include "KITabFile.h"
typedef struct tagTabOffset
{
	DWORD		dwOffset;
	DWORD		dwLength;
} TABOFFSET;

class ENGINE_API KTabFile:public KITabFile
{
private:
	int			m_Width;
	int			m_Height;
	KMemClass	m_Memory;
	KMemClass	m_OffsetTable;
	
	DWORD		m_FileNameId;
	
private:
	void		CreateTabOffset();
	BOOL		GetValue(int nRow, int nColumn, LPSTR lpRString, DWORD dwSize);
	int			Str2Col(LPSTR szColumn);
public:
	KTabFile();
	~KTabFile();
	BOOL		Load(LPSTR FileName);
	BOOL		Save(LPSTR FileName){return FALSE;}; 
	BOOL		LoadPack(LPSTR FileName);
	int			FindRow(LPSTR szRow, int nColum = 0);		//
	int			FindColumn(LPSTR szColumn);
	void		Col2Str(int nCol, LPSTR szColumn);
	int			GetWidth() { return m_Width;};
	int			GetHeight() { return m_Height;};
	//DWORD		GetID() const { return m_FileNameId; };
	BOOL		GetString(int nRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize, BOOL bColumnLab = TRUE);
	BOOL		GetString(int nRow, int nColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize);
	BOOL		GetString(LPSTR szRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize);
	BOOL		GetInteger(int nRow, LPSTR szColumn, int nDefault, int *pnValue, BOOL bColumnLab = TRUE);
	BOOL		GetInteger(int nRow, int nColumn, int nDefault, int *pnValue);
	BOOL		GetInteger(LPSTR szRow, LPSTR szColumn, int nDefault, int *pnValue);
	BOOL		GetFloat(int nRow, LPSTR szColumn, float fDefault, float *pfValue, BOOL bColumnLab = TRUE);
	BOOL		GetFloat(int nRow, int nColumn, float fDefault, float *pfValue);
	BOOL		GetFloat(LPSTR szRow, LPSTR szColumn, float fDefault, float *pfValue);
	void		Clear();
	
	DWORD		GetFileNameId() const { return m_FileNameId; };
	
};
#endif
