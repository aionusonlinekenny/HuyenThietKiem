
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KCodec.h"
#include "KCodecLzo.h"

DWORD KCodec::GetPackLen(DWORD dwDataLen)
{
	return (dwDataLen + dwDataLen / 10 + 1024);
}

BOOL KCodec::Encode(TCodeInfo* pCodeInfo)
{
	g_MemCopyMmx(pCodeInfo->lpPack, pCodeInfo->lpData, pCodeInfo->dwDataLen);
	pCodeInfo->dwPackLen = pCodeInfo->dwDataLen;
	return TRUE;
}

BOOL KCodec::Decode(TCodeInfo* pCodeInfo)
{
	g_MemCopyMmx(pCodeInfo->lpData, pCodeInfo->lpPack, pCodeInfo->dwPackLen);
	pCodeInfo->dwDataLen = pCodeInfo->dwPackLen;
	return TRUE;
}

ENGINE_API void g_InitCodec(KCodec** ppCodec, int nCompressMethod)
{
	*ppCodec = NULL;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		*ppCodec = new KCodec;
		break;
//
//	case CODEC_LHA:
//		*ppCodec = new KCodecLha;
//		break;

	case CODEC_LZO:
		*ppCodec = new KCodecLzo;
		break;
		
	}
}

ENGINE_API void g_FreeCodec(KCodec** ppCodec, int nCompressMethod)
{
	if (*ppCodec == NULL)
		return;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		delete (KCodec *)*ppCodec;
		break;
//
//	case CODEC_LHA:
//		delete (KCodecLha *)*ppCodec;
//		break;

	case CODEC_LZO:
		delete (KCodecLzo *)*ppCodec;
		break;

	}
	*ppCodec = NULL;
}


