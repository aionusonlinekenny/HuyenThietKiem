
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KPakFile.h"
#include "KCanvas.h"
#include "KBitmap.h"

KBitmap::KBitmap()
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nColors = 0;
}

BOOL KBitmap::Init(int nWidth, int nHeight, int nColors)
{
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	m_nColors = nColors;
	m_Bitmap.Alloc(m_nWidth * m_nHeight);
	return (m_Bitmap.GetMemPtr() != NULL);
}

void KBitmap::Draw(int nX, int nY)
{
	LPVOID lpBitmap = m_Bitmap.GetMemPtr();
	if (lpBitmap)
	{
		g_pCanvas->DrawBitmap(nX, nY, m_nWidth, m_nHeight, lpBitmap, m_Pal16);
	}
}

void KBitmap::Clear(BYTE byColor)
{
	m_Bitmap.Fill(byColor);
}

void KBitmap::MakePalette()
{
	g_Pal32ToPal16(m_Pal32, m_Pal16, m_nColors);
}

void KBitmap::PutPixel(int nX, int nY, BYTE byColor)
{
//	if ((nX < 0) || (nY < 0) || (nX >= m_nWidth) || (nY >= m_nHeight))
//		return;
	PBYTE pPixel = (PBYTE)m_Bitmap.GetMemPtr();
	pPixel[nY * m_nWidth + nX] = byColor;
}

BYTE KBitmap::GetPixel(int nX, int nY)
{
//	if ((nX < 0) || (nY < 0) || (nX >= m_nWidth) || (nY >= m_nHeight))
//		return 0;
	PBYTE pPixel = (PBYTE)m_Bitmap.GetMemPtr();
	return pPixel[nY * m_nWidth + nX];
}

BOOL KBitmap::Load(LPSTR lpFileName)
{
	KPakFile	File;
	KBMPHEADER	Header;

	File.Open(lpFileName);
	File.Read(&Header, sizeof(Header));

	if (!g_MemComp(Header.Id, "BM08", 4))
		return FALSE;
	
	if (!Init(Header.Width, Header.Height, Header.Colors))
		return FALSE;
	
	File.Read(m_Pal32, m_nColors * sizeof(KPAL32));
	File.Read(m_Bitmap.GetMemPtr(), m_nWidth * m_nHeight);

	return TRUE;
}

BOOL KBitmap::Save(LPSTR lpFileName)
{
	KFile		File;
	KBMPHEADER	Header;

	File.Create(lpFileName);
	
	g_MemZero(&Header, sizeof(Header));
	g_MemCopy(Header.Id, "BM08", 4);
	Header.Width  = m_nWidth;
	Header.Height = m_nHeight;
	Header.Colors = m_nColors;

	File.Write(&Header, sizeof(Header));
	File.Write(m_Pal32, m_nColors * sizeof(KPAL32));
	File.Write(m_Bitmap.GetMemPtr(), m_nWidth * m_nHeight);

	return TRUE;
}

