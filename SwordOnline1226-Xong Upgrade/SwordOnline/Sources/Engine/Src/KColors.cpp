
#include "KWin32.h"
#include "KDDraw.h"
#include "KColors.h"

ENGINE_API WORD (*g_RGB)(int nRed, int nGreen, int nBlue) = g_RGB565;

ENGINE_API BYTE g_Red(WORD wColor)
{
	BYTE Red;
	__asm
	{
		mov		ax, wColor
		and		ax, 0x0031
		mov		Red, al
	}
	return Red;
}

ENGINE_API BYTE g_Green(WORD wColor)
{
	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
	BYTE Green;
	__asm
	{
		mov		ax, wColor
		shr		ax, 5
		mov		edx, Mask16
		cmp		edx, 0xffff
		je		loc_Green_0001
		and		ax, 0x0031

loc_Green_0001:

		and		ax, 0x0071
		mov		Green, al
	}
	return Green;
}

ENGINE_API BYTE g_Blue(WORD wColor)
{
	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
	BYTE Blue;
	__asm
	{
		mov		ax, wColor
		shr		ax, 10
		mov		edx, Mask16
		cmp		edx, 0xffff
		jne		loc_Blue_0001
		shr		ax, 1

loc_Blue_0001:

		and		ax, 0x0031
		mov		Blue, al
	}
	return Blue;
}

ENGINE_API WORD g_RGB555(int nRed, int nGreen, int nBlue)
{
	WORD wColor;
	__asm
	{
		xor		ecx, ecx
		mov		ebx, 0xff
		mov		eax, nRed
		and		eax, ebx
		shr		eax, 3
		shl		eax, 10
		or		ecx, eax
		mov		eax, nGreen
		and		eax, ebx
		shr		eax, 3
		shl		eax, 5
		or		ecx, eax
		mov		eax, nBlue
		and		eax, ebx
		shr		eax, 3
		or		ecx, eax
		mov		wColor, cx
	}
	return wColor;
}

ENGINE_API WORD g_RGB565(int nRed, int nGreen, int nBlue)
{
	WORD wColor;
	__asm
	{
		xor		ecx, ecx
		mov		ebx, 0xff
		mov		eax, nRed
		and		eax, ebx
		shr		eax, 3
		shl		eax, 11
		or		ecx, eax
		mov		eax, nGreen
		and		eax, ebx
		shr		eax, 2
		shl		eax, 5
		or		ecx, eax
		mov		eax, nBlue
		and		eax, ebx
		shr		eax, 3
		or		ecx, eax
		mov		wColor, cx
	}
	return wColor;
}

void g_555To565(int nWidth, int nHeight, void* lpBitmap)
{
	__asm
	{
		mov		esi, lpBitmap
		mov		edx, nHeight

loc_555to565_loop1:
		mov		ecx, nWidth

loc_555to565_loop2:
		mov		ax, [esi]
		mov		bx, ax
		shr		ax, 5
		shl		ax, 6
		and		bx, 0x001f
		or		ax, bx
		mov		[esi], ax
		add		esi, 2
		dec		ecx
		jnz		loc_555to565_loop2
		dec		edx
		jnz		loc_555to565_loop1
	}
}

void g_565To555(int nWidth, int nHeight, void* lpBitmap)
{
	__asm
	{
		mov		esi, lpBitmap
		mov		edx, nHeight

loc_565to555_loop1:
		mov		ecx, nWidth

loc_565to555_loop2:
		mov		ax, [esi]
		mov		bx, ax
		shr		ax, 6
		shl		ax, 5
		and		bx, 0x001f
		or		ax, bx
		mov		[esi], ax
		add		esi, 2
		dec		ecx
		jnz		loc_565to555_loop2
		dec		edx
		jnz		loc_565to555_loop1
	}
}


