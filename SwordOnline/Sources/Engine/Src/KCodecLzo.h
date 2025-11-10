
#ifndef KCodecLzo_H
#define KCodecLzo_H
#include "KMemClass.h"
#include "KCodec.h"
class ENGINE_API KCodecLzo : public KCodec
{
private:
	KMemClass	m_WorkMem;
	DWORD		Compress(PBYTE pIn, DWORD dwInLen, PBYTE pOut, PDWORD pOutLen);
public:
	BOOL		Encode(TCodeInfo* pCodeInfo);
	BOOL		Decode(TCodeInfo* pCodeInfo);
};
#endif
