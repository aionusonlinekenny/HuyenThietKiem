#include "KCore.h"
#include "MyAssert.H"
#include "KTabFile.h"
#include "KNpc.h"
#include "KItem.h"
#include "KItemSet.h"
#include <time.h>
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif

#ifndef _SERVER
#include "ImgRef.h"
#include "KPlayer.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KMagicDesc.h"
#endif

KItem	Item[MAX_ITEM];
int GetRandomNumber(int nMin, int nMax);

// --
//
// --
KItem::KItem()
{
	Release();
}
// --
//
// --
KItem::~KItem()
{
	Release();
}
// --
//
// --
void KItem::Release()
{
	::memset(&m_CommonAttrib,    0, sizeof(m_CommonAttrib));
	::memset(m_aryBaseAttrib,    0, sizeof(m_aryBaseAttrib));
	::memset(m_aryRequireAttrib, 0, sizeof(m_aryRequireAttrib));
	::memset(m_aryMagicAttrib,   0, sizeof(m_aryMagicAttrib));
	::memset(&m_GeneratorParam,	 0, sizeof(m_GeneratorParam));
	m_dwID				= 0;
	m_nCurrentDur		= -1;
	//
	m_wRecord			= 0;
	//
	m_dwBindState		= 0;
	//
	m_dwExpiredTime		= 0;
	//
	m_btPriceType		= 0;
	m_btDisCountPrice	= 0;
	m_btDaySell			= 0;
	m_nShopPrice		= 0;
	//
	m_nParam1			= 0;
	//
	m_dwScriptID		= 0;

#ifndef _SERVER
	::memset(&m_Image,   0, sizeof(KRUImage));
	m_bHotSell			= FALSE;
#endif
}
// --
//
// --
void KItem::Remove()
{
	Release();
}
// --
//
// --
void KItem::SetGeneratorLevel(const int* pnaryMALevel)
{
	if(pnaryMALevel)
		memcpy(m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	else
		ZeroMemory(m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);
}

// --
//
// --
void* KItem::GetRequirement(IN int nReq)
{
	int i = sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]);
	if (nReq >= i)
		return NULL;

	return &m_aryRequireAttrib[nReq];
}

// --
//
// --
void KItem::ApplyMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int i;

	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
		}
	}

	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			if (i & 1)						
			{
				if (nCount > 0)
				{
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
					nCount--;
				}
			}
			else
			{
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
			}
		}
	}
	for (i = 0; i < sizeof(m_aryExtraMagicAttrib)/sizeof(m_aryExtraMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryExtraMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
		}
	}
}

// --
//
// --
void KItem::RemoveMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int	i;
	
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}

	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)	
		{
			KItemNormalAttrib RemoveAttrib;
			if (i & 1)						
			{
				if (nCount > 0)
				{
					RemoveAttrib.nAttribType = pAttrib->nAttribType;
					RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
					RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
					RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
					nCount--;
				}
			}
			else
			{
				RemoveAttrib.nAttribType = pAttrib->nAttribType;
				RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
				RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
				RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
			}
		}
	}
	for (i = 0; i < sizeof(m_aryExtraMagicAttrib)/sizeof(m_aryExtraMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryExtraMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)	
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}
}

// --
//
// --
void KItem::ApplyHiddenMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	
	if (-1 != pAttrib->nAttribType)
	{
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
	}
}

// --
//
// --
void KItem::RemoveHiddenMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);
	if (-1 != pAttrib->nAttribType)
	{
		KItemNormalAttrib RemoveAttrib;
		RemoveAttrib.nAttribType = pAttrib->nAttribType;
		RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
		RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
		RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
	}
}

// --
//
// --
BOOL KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT* pData)
{
	_ASSERT(pData != NULL);
	
	BOOL bEC = FALSE;
	if (pData)
	{
		*this = *pData;		
		SetAttrib_Base(pData->m_aryPropBasic);
		SetAttrib_Req(pData->m_aryPropReq);
		bEC = TRUE;
	}
	return bEC;
}
// --
//
// --
BOOL KItem::SetAttrib_CBR(IN const KBASICPROP_GOLD_EQUIPMENT* pData)
{
	_ASSERT(pData != NULL);
	
	BOOL bEC = FALSE;
	if (pData)
	{
		*this = *pData;		
		SetAttrib_Base(pData->m_aryPropBasic);
		SetAttrib_Req(pData->m_aryPropReq);
		bEC = TRUE;
	}
	return bEC;
}
// --
//
// --
BOOL KItem::SetAttrib_Base(const KEQCP_BASIC* pBasic)
{	
	KItemNormalAttrib* pDst = NULL;
	const KEQCP_BASIC* pSrc = NULL;

	const int nArySize = sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		
		pDst = &(m_aryBaseAttrib[i]);
		pSrc = &(pBasic[i]);
		pDst->nAttribType = pSrc->nType;
		pDst->nValue[0] = ::GetRandomNumber(pSrc->sRange.nMin, pSrc->sRange.nMax);
		pDst->nValue[1] = 0;	
		pDst->nValue[2] = 0;	
		if (pDst->nAttribType == magic_durability_v)
			SetDurability(pDst->nValue[0]);
	}
	if (m_nCurrentDur == 0)	 
		m_nCurrentDur = -1;

	return TRUE;
}

// --
//
// --
BOOL KItem::SetAttrib_Req(const KEQCP_REQ* pReq)
{	
	KItemNormalAttrib* pDst = NULL;
	const int nArySize = sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		pDst = &(m_aryRequireAttrib[i]);
		pDst->nAttribType = pReq[i].nType;
		pDst->nValue[0] = pReq[i].nPara;
		pDst->nValue[1] = 0;	
		pDst->nValue[2] = 0;
	}
	return TRUE;
}

// --
//
// --
BOOL KItem::SetAttrib_MA(IN const KItemNormalAttrib* pMA)
{
	if(NULL == pMA)
	{ 
		_ASSERT(FALSE); 
		return FALSE;
	}
	
	const int nArySize = sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		m_aryMagicAttrib[i] = pMA[i];
		
		if (m_aryMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}
	}
	return TRUE;
}

// --
//
// --
BOOL KItem::SetAttrib_ExtraMA(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
		{ _ASSERT(FALSE); return FALSE; }

	for (int i = 0; i < sizeof(m_aryExtraMagicAttrib) / sizeof(m_aryExtraMagicAttrib[0]); i++)
	{
		m_aryExtraMagicAttrib[i] = pMA[i];

		if (m_aryExtraMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}
	}
	return TRUE;
}
// --
//
// --
BOOL KItem::SetAttrib_MA(IN const KMACP* pMA)
{
	if(NULL == pMA)
	{ 
		_ASSERT(FALSE); 
		return FALSE; 
	}
	const KMACP* pSrc = NULL;
	KItemNormalAttrib* pDst = NULL;
	//
	const int nArySize = sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		pSrc = &(pMA[i]);
		pDst = &(m_aryRequireAttrib[i]);

		pDst->nAttribType = pSrc->nPropKind;
		pDst->nValue[0] =  ::GetRandomNumber(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
		pDst->nValue[1] =  ::GetRandomNumber(pSrc->aryRange[1].nMin, pSrc->aryRange[1].nMax);
		pDst->nValue[2] =  ::GetRandomNumber(pSrc->aryRange[2].nMin, pSrc->aryRange[2].nMax);
	}

	return TRUE;
}
// --
//
// --
void KItem::operator = (const KBASICPROP_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->cGenre		 = sData.m_nGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->wObjID			 = sData.m_nObjID;
	pCA->bWidth			 = sData.m_nWidth;
	pCA->bHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->bLevel			 = sData.m_nLevel;
	pCA->cSeries		 = sData.m_nSeries;
	
	::strcpy(pCA->szItemName,  sData.m_szName);
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	//
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    int nOffSet = 0, nCheckLen = 0, nS = 0, nLen = strlen(szIntro);  
    while(szIntro[nOffSet] != 0) 
    { 
        if(szIntro[nOffSet] == '<') 
        { 
			if(szIntro[nOffSet+1] == 'e' && 
				szIntro[nOffSet+2] == 'n' && 
				szIntro[nOffSet+3] == 't' && 
				szIntro[nOffSet+4] == 'e' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen = -6;
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen -= 6; 
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '=') 
				{ 
					for(int k = 0; k<10; k++) 
					{ 
						if(szIntro[nOffSet+6+k] == '>') 
							break; 
					} 
					nCheckLen -= 6+k; 
				} 
        } 
        if(nCheckLen == 40)
        { 
			while(szIntro[nOffSet+nS] < 32 || szIntro[nOffSet+nS] > 126) 
            { 
				nS++; 
            } 
            memmove(&szIntro[nOffSet+nS+8], &szIntro[nOffSet+nS], nLen-(nOffSet+nS)+1); 
            memcpy(&szIntro[nOffSet+nS], " <enter>", 8); 
            nOffSet += nS; 
            nLen += 8; 
            nS = 0; 
            nCheckLen = 0; 
        } 
        nOffSet++; 
        nCheckLen++; 
    } 
    if(strlen(szIntro) > 255) 
		szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
void KItem::operator = (const KBASICPROP_GOLD_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->cGenre			 = sData.m_nGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->wObjID			 = sData.m_nObjID;
	pCA->bWidth			 = sData.m_nWidth;
	pCA->bHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->bLevel			 = sData.m_nLevel;
	pCA->cSeries		 = sData.m_nSeries;
	
	::strcpy(pCA->szItemName,  sData.m_szName);
	//
	char szBuffer[16];
	sprintf(szBuffer,"1%03d%02d%02d", sData.m_nSet, sData.m_nSetId, sData.m_nSetNum);
	m_nParam1 = atoi(szBuffer);
	//

#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	//
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    //sua lai cho nay
    int nOffSet = 0, nCheckLen = 0, nS = 0, nLen = strlen(szIntro);  
    while(szIntro[nOffSet] != 0) 
    { 
        if(szIntro[nOffSet] == '<') 
        { 
			if(szIntro[nOffSet+1] == 'e' && 
				szIntro[nOffSet+2] == 'n' && 
				szIntro[nOffSet+3] == 't' && 
				szIntro[nOffSet+4] == 'e' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen = -6;
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen -= 6; 
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '=') 
				{ 
					for(int k = 0; k<10; k++) 
					{ 
						if(szIntro[nOffSet+6+k] == '>') 
							break; 
					} 
					nCheckLen -= 6+k; 
				} 
        } 
        if(nCheckLen == 40)
        { 
			while(szIntro[nOffSet+nS] < 32 || szIntro[nOffSet+nS] > 126) 
            { 
				nS++; 
            } 
            memmove(&szIntro[nOffSet+nS+8], &szIntro[nOffSet+nS], nLen-(nOffSet+nS)+1); 
            memcpy(&szIntro[nOffSet+nS], " <enter>", 8); 
            nOffSet += nS; 
            nLen += 8; 
            nS = 0; 
            nCheckLen = 0; 
        } 
        nOffSet++; 
        nCheckLen++; 
    } 
    if(strlen(szIntro) > 255) 
		szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
void KItem::operator = (const KBASICPROP_BROKEN_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->cGenre				= sData.m_nGenre;
	pCA->nDetailType		= -1;
	pCA->nParticularType	= -1;
	pCA->wObjID				= sData.m_nObjID;
	pCA->bWidth				= sData.m_nWidth;
	pCA->bHeight			= sData.m_nHeight;
	pCA->nPrice				= 0;
	pCA->bLevel				= 0;
	pCA->cSeries			= -1;
	
	::strcpy(pCA->szItemName,  sData.m_szName);
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
void KItem::operator = (const KBASICPROP_MEDICINE& sData)
{
	KItemCommonAttrib* pCA	= &m_CommonAttrib;
	pCA->cGenre				= (char)sData.m_nGenre;
	pCA->nDetailType		= (short)sData.m_nDetailType;
	pCA->nParticularType	= (short)sData.m_nParticularType;
	pCA->wObjID				= (WORD)sData.m_nObjID;
	pCA->bWidth				= (BYTE)sData.m_nWidth;
	pCA->bHeight			= (BYTE)sData.m_nHeight;
	pCA->nPrice				= sData.m_nPrice;
	pCA->bLevel				= (BYTE)sData.m_nLevel;
	pCA->cSeries			= -1;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//
	KItemNormalAttrib* pBA = (KItemNormalAttrib*)&m_aryBaseAttrib;

	pBA[0].nAttribType = sData.m_aryAttrib[0].nAttrib;
	pBA[0].nValue[0]   = sData.m_aryAttrib[0].nValue;
	pBA[0].nValue[1]   = sData.m_aryAttrib[0].nTime;
	pBA[1].nAttribType = sData.m_aryAttrib[1].nAttrib;
	pBA[1].nValue[0]   = sData.m_aryAttrib[1].nValue;
	pBA[1].nValue[1]   = sData.m_aryAttrib[1].nTime;
	//them by kinnox 24/03/2024;
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
	//
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    int nOffSet = 0, nCheckLen = 0, nS = 0, nLen = strlen(szIntro);  
    while(szIntro[nOffSet] != 0) 
    { 
        if(szIntro[nOffSet] == '<') 
        { 
			if(szIntro[nOffSet+1] == 'e' && 
				szIntro[nOffSet+2] == 'n' && 
				szIntro[nOffSet+3] == 't' && 
				szIntro[nOffSet+4] == 'e' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen = -6;
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen -= 6; 
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '=') 
				{ 
					for(int k = 0; k<10; k++) 
					{ 
						if(szIntro[nOffSet+6+k] == '>') 
							break; 
					} 
					nCheckLen -= 6+k; 
				} 
        } 
        if(nCheckLen == 40)
        { 
			while(szIntro[nOffSet+nS] < 32 || szIntro[nOffSet+nS] > 126) 
            { 
				nS++; 
            } 
            memmove(&szIntro[nOffSet+nS+8], &szIntro[nOffSet+nS], nLen-(nOffSet+nS)+1); 
            memcpy(&szIntro[nOffSet+nS], " <enter>", 8); 
            nOffSet += nS; 
            nLen += 8; 
            nS = 0; 
            nCheckLen = 0; 
        } 
        nOffSet++; 
        nCheckLen++; 
    } 
    if(strlen(szIntro) > 255) 
		szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif

}
// --
//
// --
void KItem::operator = (const KBASICPROP_QUEST& sData)
{
	KItemCommonAttrib* pCA	= &m_CommonAttrib;
	pCA->cGenre				= (char)sData.m_nGenre;
	pCA->nDetailType		= (short)sData.m_nDetailType;
	pCA->nParticularType	= (short)sData.m_nParticularType;
	pCA->wObjID				= (WORD)sData.m_nObjID;
	pCA->bWidth				= (BYTE)sData.m_nWidth;
	pCA->bHeight			= (BYTE)sData.m_nHeight;
	pCA->nPrice				= 0;
	pCA->bLevel				= 1;
	pCA->cSeries			= -1;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//
	if(sData.m_nMaxStack > 0)
	{
		m_aryBaseAttrib[0].nAttribType	= magic_durability_v;
		m_aryBaseAttrib[0].nValue[0]	= sData.m_nMaxStack;
		m_nCurrentDur = 1;
	}
	//them by kinnox 24/03/2024;
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
	//
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    int nOffSet = 0, nCheckLen = 0, nS = 0, nLen = strlen(szIntro);  
    while(szIntro[nOffSet] != 0) 
    { 
        if(szIntro[nOffSet] == '<') 
        { 
			if(szIntro[nOffSet+1] == 'e' && 
				szIntro[nOffSet+2] == 'n' && 
				szIntro[nOffSet+3] == 't' && 
				szIntro[nOffSet+4] == 'e' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen = -6;
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '>') 
			{
				nCheckLen -= 6; 
			}
			if(szIntro[nOffSet+1] == 'c' && 
				szIntro[nOffSet+2] == 'o' && 
				szIntro[nOffSet+3] == 'l' && 
				szIntro[nOffSet+4] == 'o' && 
				szIntro[nOffSet+5] == 'r' && 
				szIntro[nOffSet+6] == '=') 
				{ 
					for(int k = 0; k<10; k++) 
					{ 
						if(szIntro[nOffSet+6+k] == '>') 
							break; 
					} 
					nCheckLen -= 6+k; 
				} 
        } 
        if(nCheckLen == 40)
        { 
			while(szIntro[nOffSet+nS] < 32 || szIntro[nOffSet+nS] > 126) 
            { 
				nS++; 
            } 
            memmove(&szIntro[nOffSet+nS+8], &szIntro[nOffSet+nS], nLen-(nOffSet+nS)+1); 
            memcpy(&szIntro[nOffSet+nS], " <enter>", 8); 
            nOffSet += nS; 
            nLen += 8; 
            nS = 0; 
            nCheckLen = 0; 
        } 
        nOffSet++; 
        nCheckLen++; 
    } 
    if(strlen(szIntro) > 255) 
		szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
void KItem::operator = (const KBASICPROP_SCRIPT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->cGenre			 = (char)sData.m_nGenre;
	pCA->nDetailType	 = (short)sData.m_nDetailType;
	pCA->nParticularType = (short)sData.m_nParticularType;
	pCA->wObjID			 = (WORD)sData.m_nObjID;
	pCA->bWidth			 = (BYTE)sData.m_nWidth;
	pCA->bHeight		 = (BYTE)sData.m_nHeight;
	pCA->nPrice			 = 0;
	pCA->bLevel			 = 0;
	pCA->cSeries		 = -1;
	pCA->nTaskIdScript	 = (short)sData.m_nTaskIdScript;//nhim
	pCA->nTaskIdScriptMax = (short)sData.m_nTaskIdScriptMax;//nhim
	::strcpy(pCA->szItemName,  sData.m_szName);
	// 
	if(sData.m_nMaxStack > 0)
	{
		m_aryBaseAttrib[0].nAttribType	= magic_durability_v;
		m_aryBaseAttrib[0].nValue[0]	= sData.m_nMaxStack;
		m_nCurrentDur = 1;
	}
	//
	m_dwScriptID		= g_FileName2Id((char*)sData.m_szScriptFile);
	//them by kinnox 24/03/2024;
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
	//
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    int offset = 0, nL = 0, nS = 0; 
    int nStrL = strlen(szIntro); 
    while(szIntro[offset] != 0) 
    { 
        if(szIntro[offset] == '<') 
        { 
        if(szIntro[offset+1] == 'e' && 
        szIntro[offset+2] == 'n' && 
        szIntro[offset+3] == 't' && 
        szIntro[offset+4] == 'e' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL = -6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL -= 6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '=') 
            { 
            int k; 
            for(k = 0; k<10; k++) 
            { 
            if(szIntro[offset+6+k] == '>') break; 
            } 
            nL -= 6+k; 
            } 
        } 
        if(nL == 40) //doan. dai` hay ngan' tuy` vao` so' nay` 
        { 
        while(szIntro[offset+nS] < 32 || szIntro[offset+nS] > 126) 
            { 
            nS++; 
            } 
            memmove(&szIntro[offset+nS+8], &szIntro[offset+nS], nStrL-(offset+nS)+1); 
            memcpy(&szIntro[offset+nS]," <enter>",8); 
            offset += nS; 
            nStrL += 8; 
            nS = 0; 
            nL = 0; 
        } 
        offset++; 
        nL++; 
    } 
    if(strlen(szIntro) > 255) 
    szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
void KItem::operator = (const KBASICPROP_MINE& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->cGenre			 = (char)sData.m_nGenre;
	pCA->nDetailType	 = (short)sData.m_nDetailType;
	pCA->nParticularType = (short)sData.m_nParticularType;
	pCA->wObjID			 = (WORD)sData.m_nObjID;
	pCA->bWidth			 = (BYTE)sData.m_nWidth;
	pCA->bHeight		 = (BYTE)sData.m_nHeight;
	pCA->nPrice			 = 0;
	pCA->bLevel			 = 0;
	pCA->cSeries		 = -1;
	::strcpy(pCA->szItemName,  sData.m_szName);
	// 
	if(sData.m_nMaxStack > 0)
	{
		m_aryBaseAttrib[0].nAttribType	= magic_durability_v;
		m_aryBaseAttrib[0].nValue[0]	= sData.m_nMaxStack;
		m_nCurrentDur = 1;
	}
	//them by kinnox 24/03/2024;
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
	//
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szIcon);
	::strcpy(pCA->szIntro,	   sData.m_szIntro); //fix by kinnox;
	/*char szIntro[516]; 
    sprintf(szIntro, "%s", sData.m_szIntro); 
    int offset = 0, nL = 0, nS = 0; 
    int nStrL = strlen(szIntro); 
    while(szIntro[offset] != 0) 
    { 
        if(szIntro[offset] == '<') 
        { 
        if(szIntro[offset+1] == 'e' && 
        szIntro[offset+2] == 'n' && 
        szIntro[offset+3] == 't' && 
        szIntro[offset+4] == 'e' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL = -6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL -= 6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '=') 
            { 
            int k; 
            for(k = 0; k<10; k++) 
            { 
            if(szIntro[offset+6+k] == '>') break; 
            } 
            nL -= 6+k; 
            } 
        } 
        if(nL == 40) //doan. dai` hay ngan' tuy` vao` so' nay` 
        { 
        while(szIntro[offset+nS] < 32 || szIntro[offset+nS] > 126) 
            { 
            nS++; 
            } 
            memmove(&szIntro[offset+nS+8], &szIntro[offset+nS], nStrL-(offset+nS)+1); 
            memcpy(&szIntro[offset+nS]," <enter>",8); 
            offset += nS; 
            nStrL += 8; 
            nS = 0; 
            nL = 0; 
        } 
        offset++; 
        nL++; 
    } 
    if(strlen(szIntro) > 255) 
    szIntro[255] = 0; 
    ::memcpy(pCA->szIntro, szIntro, 256);*/
	//
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}
// --
//
// --
BOOL KItem::SetBaseAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	const int nArySize = sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		m_aryBaseAttrib[i] = pAttrib[i];
	}
	return TRUE;
}
// --
//
// --
BOOL KItem::SetRequireAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;
	
	const int nArySize = sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]);
	int i;
	for(i = 0; i < nArySize; i++)
	{
		m_aryRequireAttrib[i] = pAttrib[i];
	}
	return TRUE;
}
// --
//
// --
BOOL KItem::SetMagicAttrib(IN const KItemNormalAttrib* pAttrib)
{
	return SetAttrib_MA(pAttrib);
}
//// --
//	
//// --
int KItem::Abrade(IN const int nRandRange)
{
	if (m_nCurrentDur == -1 || nRandRange == 0)	
		return -1;

	if (g_Random(nRandRange) == 0)
	{
		m_nCurrentDur--;
		if (m_nCurrentDur == 0)
		{
			return 0;
		}
	}
	return m_nCurrentDur;
}

#ifndef _SERVER
// --
//
// --
void KItem::Paint(int nX, int nY, BOOL bStack)
{
	m_Image.oPosition.nX = nX;
	m_Image.oPosition.nY = nY;
	m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, TRUE);

	if( CanStack() && bStack)
	{
		char szBuffer[8];
		sprintf(szBuffer, "%d", m_nCurrentDur);
		g_pRepresent->OutputText(12, szBuffer, KRF_ZERO_END,
		nX + (m_CommonAttrib.bWidth * 27) - g_StrLen(szBuffer) * 12 / 2,
		nY + (m_CommonAttrib.bHeight) + 12 + 1, 0xffffff00, 0, TEXT_IN_SINGLE_PLANE_COORD, 0xff000000);
	}

}

// --
//
// --
void KItem::GetDesc(char* pszMsg, bool bShowPrice, int nPriceScale, int nActiveAttrib)
{
	char	szColor[item_number][32] =
	{
		"<color=White>",
		"<color=Purple>",
		"<color=Gold>",
		"<color=White>",
		"<color=Red>",
		"",
		"<color=White>",
		"<color=White>",
		"<color=White>",
	};
	if (m_CommonAttrib.cGenre == item_equip)
	{
		int nAttribIdx;
		for(nAttribIdx = 0; nAttribIdx < 6; nAttribIdx++)
		{
			if(m_aryMagicAttrib[nAttribIdx].nAttribType > 0)
			{
				strcpy(szColor[item_equip], "<color=Blue>");
				break;
			}
		}
	}
	strcpy(pszMsg, szColor[m_CommonAttrib.cGenre]);
	strcat(pszMsg, m_CommonAttrib.szItemName);
	//
	if(m_CommonAttrib.cGenre == item_equip ||
		m_CommonAttrib.cGenre == item_purpleequip ||
		m_CommonAttrib.cGenre == item_goldequip ||
		m_CommonAttrib.cGenre == item_brokenequip)
	{	
		char szTmp[16];
		sprintf(szTmp, " (CÊp %d)", m_CommonAttrib.bLevel);
		strcat(pszMsg, szTmp);
	}

	//
	if(m_CommonAttrib.cGenre == item_task ||
		m_CommonAttrib.cGenre == item_script)
	{
		if(m_CommonAttrib.nParticularType == 0)
		{
			strcat(pszMsg, "\n");
			strcat(pszMsg, "<color=red>VËt phÈm kh«ng thÓ b¸n, giao dÞch <color>");
		}
	}
	//gioi han
	if(m_CommonAttrib.cGenre == item_script)
	{
		if(m_CommonAttrib.nTaskIdScript > 0)
		{
			char szTmp[16];
			int j = Player[CLIENT_PLAYER_INDEX].TaskGetSaveVal(m_CommonAttrib.nTaskIdScript);
			strcat(pszMsg, "\n");
			if(j >= m_CommonAttrib.nTaskIdScriptMax)
			{
				sprintf(szTmp, "<color=Green>§· dïng hÕt %d vËt phÈm h«m nay<color>",m_CommonAttrib.nTaskIdScriptMax);
			}else{
				sprintf(szTmp, "<color=Green>Giíi h¹n trong ngµy: %d/%d <color>", j,m_CommonAttrib.nTaskIdScriptMax);
			}
			strcat(pszMsg, szTmp);
			strcat(pszMsg, "\n");
		}
	}
	//
	if(m_dwBindState == 1)
	{
		strcat(pszMsg, "\n");
		strcat(pszMsg, "<color=green>VËt phÈm ®· khãa b¶o hiÓm<color>");
	}
	else if(m_dwBindState == 2)
	{
		strcat(pszMsg, "\n");
		strcat(pszMsg, "<color=green>VËt phÈm ®Ýnh kÌm nh©n vËt<color>");
	}
	else if(m_dwBindState >= 2000000000)
	{
		char szTemp[64];
		this->GetTimeDesc(szTemp, TRUE);
		strcat(pszMsg, "\n");
		strcat(pszMsg, szTemp);
	}
	//
	strcat(pszMsg, "\n");
	if (bShowPrice/* && nPriceScale > 0*/)
	{	
		char szPrice[64];
		switch(m_btPriceType)
		{
			case 1:
				GetPriceDesc(-1, nPriceScale, "Xu", "<color=green>", szPrice);
				break;
			case 3:
				GetPriceDesc(-1, nPriceScale, "®iÓm", "<color=violet>", szPrice);
				break;
			default:
				GetPriceDesc(-1, nPriceScale, "l­îng", "<color=yellow>", szPrice);
				break;
		}
		strcat(pszMsg, szPrice);
		strcat(pszMsg, "\n");
	}
	if(m_CommonAttrib.cGenre == item_equip ||
		m_CommonAttrib.cGenre == item_purpleequip ||
		m_CommonAttrib.cGenre == item_goldequip ||
		m_CommonAttrib.cGenre == item_brokenequip ||
		m_CommonAttrib.cGenre == item_mine)
	{	
		switch(m_CommonAttrib.cSeries)
		{
			case series_metal:
				strcat(pszMsg, "\n<color=White>Thuéc tÝnh ngò hµnh: <color=Metal>Kim");
				break;
			case series_wood:
				strcat(pszMsg, "\n<color=White>Thuéc tÝnh ngò hµnh: <color=Wood>Méc");
				break;
			case series_water:
				strcat(pszMsg, "\n<color=White>Thuéc tÝnh ngò hµnh: <color=Water>Thñy");
				break;
			case series_fire:
				strcat(pszMsg, "\n<color=White>Thuéc tÝnh ngò hµnh: <color=Fire>Háa");
				break;
			case series_earth:
				strcat(pszMsg, "\n<color=White>Thuéc tÝnh ngò hµnh: <color=Earth>Thæ ");
				break;
		}
	}

	strcat(pszMsg, "\n");
	strcat(pszMsg, "<color=White>");
	//strcat(pszMsg, m_CommonAttrib.szIntro);
	char szIntro[SZBUFLEN_1]; 
    sprintf(szIntro, "%s", m_CommonAttrib.szIntro); 
	int offset = 0, nL = 0, nS = 0; 
    int nStrL = strlen(szIntro); 
    while(szIntro[offset] != 0) 
    { 
         if(szIntro[offset] == '<' && offset + 6 < nStrL)  // BOUNDS CHECK ADDED
        { 
        if(szIntro[offset+1] == 'e' && 
        szIntro[offset+2] == 'n' && 
        szIntro[offset+3] == 't' && 
        szIntro[offset+4] == 'e' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL = -6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '>') 
        nL -= 6; 
        if(szIntro[offset+1] == 'c' && 
        szIntro[offset+2] == 'o' && 
        szIntro[offset+3] == 'l' && 
        szIntro[offset+4] == 'o' && 
        szIntro[offset+5] == 'r' && 
        szIntro[offset+6] == '=') 
            { 
            int k; 
            for(k = 0; k<10; k++) 
             {
				if(offset+6+k >= nStrL || szIntro[offset+6+k] == '>') break;  // BOUNDS CHECK ADDED
            } 
				nL -= 6+k; 
            } 
        } 
        if(nL == 40) //doan. dai` hay ngan' tuy` vao` so' nay`
        {
        while(offset+nS < nStrL && (szIntro[offset+nS] < 32 || szIntro[offset+nS] > 126))  // BOUNDS CHECK ADDED
            {
            nS++;
            }
            if(offset+nS+8 < SZBUFLEN_1 && nStrL-(offset+nS)+1+8 < SZBUFLEN_1)  // BOUNDS CHECK ADDED
            {
                memmove(&szIntro[offset+nS+8], &szIntro[offset+nS], nStrL-(offset+nS)+1);
                memcpy(&szIntro[offset+nS]," <enter>",8);
                offset += nS;
                nStrL += 8;
            }
            nS = 0;
            nL = 0;
        }
        offset++;
        nL++;
    } 
    if(strlen(szIntro) > SZBUFLEN_1-1)
    szIntro[SZBUFLEN_1-1] = 0;
	strcat(pszMsg, szIntro);
	strcat(pszMsg, "\n");

	if(m_CommonAttrib.cGenre == item_mine)
	{
		if(m_CommonAttrib.bLevel > 0)
		{
			char szTmp[16];
			strcat(pszMsg, "\n");
			sprintf(szTmp, "<color=purple>PhÈm chÊt: %d <color>", m_CommonAttrib.bLevel);
			strcat(pszMsg, szTmp);
			strcat(pszMsg, "\n");
		}
	}
	//
	char* pszInfo = NULL;
	int i;
	if(m_CommonAttrib.cGenre == item_equip ||
	   m_CommonAttrib.cGenre == item_purpleequip ||
	   m_CommonAttrib.cGenre == item_goldequip ||
	   m_CommonAttrib.cGenre == item_brokenequip
	  )
	{
		for(i = 0; i < 7; i++)
		{
			if (!m_aryBaseAttrib[i].nAttribType)
			{
				continue;
			}
			if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
			{
				if(m_CommonAttrib.cGenre == 4)
				{
					char	szDurInfo[32];
					sprintf(szDurInfo, "<color=red>Trang bÞ tæn h¹i<color>");
					strcat(pszMsg, szDurInfo);
				}
				else
				{
					char	szDurInfo[32];
					if (m_nCurrentDur == -1)
						sprintf(szDurInfo, "<color=Yellow>Kh«ng thÓ ph¸ hñy<color=White>");
					else
						sprintf(szDurInfo, "§é bÒn: %3d / %3d", GetDurability(), GetMaxDurability());
					strcat(pszMsg, szDurInfo);
				}
			}
			else
			{
				pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryBaseAttrib[i]);
				if (!pszInfo || !pszInfo[0])
					continue;
				strcat(pszMsg, pszInfo);
			}
			strcat(pszMsg, "\n");
		}
		for (i = 0; i < 6; i++)
		{
			if (!m_aryRequireAttrib[i].nAttribType)
			{
				continue;
			}
			pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryRequireAttrib[i]);
			if (!pszInfo || !pszInfo[0])
				continue;
			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&m_aryRequireAttrib[i]))
			{
				strcat(pszMsg, "<color=White>");
			}
			else
			{
				strcat(pszMsg, "<color=Red>");
			}
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "\n");
		}
	}
	//
	for (i = 0; i < 6; i++)
	{
		if (!m_aryMagicAttrib[i].nAttribType)
		{
			continue;
		}
		pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryMagicAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		if ((i & 1) == 0)
		{	
			if(m_GeneratorParam.nLuck >= 1000000000)
			{
				if(m_aryMagicAttrib[i].nAttribType == magic_item_reserve10)
					strcat(pszMsg, "<color=Metal>");
				else
					strcat(pszMsg, "<color=Purple>");
			}
			else if(m_GeneratorParam.nLuck == 7531)
			{
				strcat(pszMsg, "<color=Yellow>");
			} 
			else
			{
				strcat(pszMsg, "<color=Blue>");
			}
			
		}
		else
		{
			if ((i>>1) < nActiveAttrib)
			{	
				if(m_GeneratorParam.nLuck >= 1000000000)
				{
					if(m_aryMagicAttrib[i].nAttribType == magic_item_reserve10)
						strcat(pszMsg, "<color=DMetal>");
					else
						strcat(pszMsg, "<color=Purple>");
				}
				else if(m_GeneratorParam.nLuck == 7531)
				{
					strcat(pszMsg, "<color=Yellow>");
				}
				else
				{
					strcat(pszMsg, "<color=Blue>");
				}
			}
			else
			{	
				if(m_GeneratorParam.nLuck >= 1000000000)
				{
					if(m_aryMagicAttrib[i].nAttribType == magic_item_reserve10)
						strcat(pszMsg, "<color=DMetal>");
					else
						strcat(pszMsg, "<color=DPurple>");
				}
				else if(m_GeneratorParam.nLuck == 7531)
				{	
					if(m_CommonAttrib.nDetailType == itempart_horse)
					{
						strcat(pszMsg, "<color=Yellow>");
					}
					else
					{
						strcat(pszMsg, "<color=DYellow>");
					}
				}
				else
				{
					strcat(pszMsg, "<color=DBlue>");
				}
			}
		}
		strcat(pszMsg, pszInfo);

	// Display [min-max] or [MAX] indicator (TamLTM custom)
	if (m_aryMagicAttrib[i].nMax > 0)
	{
		if (m_aryMagicAttrib[i].nValue[0] == m_aryMagicAttrib[i].nMax)
		{
			// If attribute value reaches maximum, show [MAX]
			strcat(pszMsg, " <color=red>[MAX]<color>");
		}
		else
		{
			// Otherwise, show [min-max] range
			char szRange[64];
			if (m_aryMagicAttrib[i].nMin < 0)
			{
				// Negative min value - display in green
				sprintf(szRange, " <color=0,255,255>[<color=0,255,0>%d<color=0,255,255>-%d]<color>",
						m_aryMagicAttrib[i].nMin, m_aryMagicAttrib[i].nMax);
			}
			else
			{
				// Positive min value - display normally
				sprintf(szRange, " <color=0,255,255>[%d-%d]<color>",
						m_aryMagicAttrib[i].nMin, m_aryMagicAttrib[i].nMax);
			}
			strcat(pszMsg, szRange);
		}
	}
		strcat(pszMsg, "\n");
	}

	if(m_CommonAttrib.cGenre == item_script)
	{
		if( (this->GetDurability() > 0) && (this->GetMaxStackCount() <= 0) )
		{	
			char szInfo[64];
			sprintf(szInfo, "Cã thÓ sö dông:<color=green> %d  <color>lÇn.", this->GetDurability());
			strcat(pszMsg, "\n");
			strcat(pszMsg, szInfo);
			strcat(pszMsg, "\n");
		}
	}

	if (m_aryExtraMagicAttrib[0].nAttribType)
		strcat(pszMsg, "\n");
	for (i = 0; i < 2; i++)
	{
		if (!m_aryExtraMagicAttrib[i].nAttribType)
		{
			continue;
		}
		pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryExtraMagicAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		
		if ((i>>1) < nActiveAttrib)
		{
			strcat(pszMsg, "<color=Violet>");
		}
		else
		{
			strcat(pszMsg, "<color=DYellow>");
		}
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}
	
	// Hien thi Desc cac trang bi trong 1 bo trang bi Hoang Kim
	if(m_CommonAttrib.cGenre == item_goldequip)
	{
		char szInfo[512];
		GetSuiteDesc(szInfo);
		if(szInfo[0])
			strcat(pszMsg, szInfo);
	}
	//
	if(m_CommonAttrib.cGenre == item_equip && m_aryMagicAttrib[0].nAttribType)
	{
		int nIndex = 0;
		strcat(pszMsg, "\n");
		switch(m_CommonAttrib.nDetailType)
		{
			case equip_meleeweapon:
			case equip_rangeweapon:
			case equip_helm:
				if(m_CommonAttrib.nDetailType == equip_helm)
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_head);
				}
				else
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_weapon);
				}
				if(Item[nIndex].GetID() == this->m_dwID && this->m_dwID != 0)
				{
					strcat(pszMsg, GetRequireSeries(m_CommonAttrib.cSeries));
					strcat(pszMsg, "cña <color=green>D©y ChuyÒn<color> vµ <color=green>Y Phôc<color> ®Ó kÝch thuéc tÝnh Èn<color>\n");
					strcat(pszMsg, "\n");
				}
				break;
			case equip_armor:
			case equip_amulet:	
				if(m_CommonAttrib.nDetailType == equip_armor)
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_body);
				}
				else
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_amulet);
				}
				if(Item[nIndex].GetID() == this->m_dwID && this->m_dwID != 0)
				{
					strcat(pszMsg, GetRequireSeries(m_CommonAttrib.cSeries));
					strcat(pszMsg, "cña <color=green>NhÉn D­íi<color> vµ <color=green>Th¾t L­ng<color> ®Ó kÝch thuéc tÝnh Èn<color>\n");
					strcat(pszMsg, "\n");
				}
				break;
			case equip_ring:
			case equip_boots:
			case equip_belt:
				{	
					if(m_CommonAttrib.nDetailType == equip_ring)
					{
						nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_ring1);
					}
					else if(m_CommonAttrib.nDetailType == equip_boots)
					{
						nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_foot);
					}
					
					if(Item[nIndex].GetID() == this->m_dwID && this->m_dwID != 0)
					{	
						strcat(pszMsg, GetRequireSeries(m_CommonAttrib.cSeries));
						strcat(pszMsg, "cña <color=green>Mò <color>vµ <color=green>Vò KhÝ <color>®Ó kÝch thuéc tÝnh Èn<color>\n");
						strcat(pszMsg, "\n");

						if(m_CommonAttrib.nDetailType == equip_boots)
							break;
					}

					if(m_CommonAttrib.nDetailType == equip_ring)
					{
						nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_ring2);
					}
					else if(m_CommonAttrib.nDetailType == equip_belt)
					{
						nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_belt);
					}

					if(Item[nIndex].GetID() == this->m_dwID && this->m_dwID != 0)
					{	
						strcat(pszMsg, GetRequireSeries(m_CommonAttrib.cSeries));
						strcat(pszMsg, "cña <color=green>Ngäc Béi<color> vµ <color=green>Bao Tay<color> ®Ó kÝch ho¹t thuéc tÝnh Èn\n");
						strcat(pszMsg, "\n");
						if(m_CommonAttrib.nDetailType == itempart_belt)
							break;		
					}
					break;
				}
			case equip_cuff:
			case equip_pendant:	
				if(m_CommonAttrib.nDetailType == equip_cuff)
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_cuff);
				}
				else
				{
					nIndex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_pendant);
				}
				if(Item[nIndex].GetID() == this->m_dwID && this->m_dwID != 0)
				{	
					strcat(pszMsg, GetRequireSeries(m_CommonAttrib.cSeries));
					strcat(pszMsg, "cña <color=green>Giµy<color> vµ <color=green>NhÉn Trªn<color> ®Ó kÝch thuéc tÝnh Èn<color>\n");
					strcat(pszMsg, "\n");
				}
				break;
			default:
				break;
		}
	}

	if (bShowPrice/* && nPriceScale > 0 */&& m_btDisCountPrice > 0)
	{	
		char szPrice[64];
		strcat(pszMsg, "\n");
		switch(m_btPriceType)
		{
			case 1:
				GetPriceDesc(0, nPriceScale, "xu", "<color=green>", szPrice);
				break;
			default:
				GetPriceDesc(0, nPriceScale, "l­îng", "<color=yellow>", szPrice);
				break;
		}
		strcat(pszMsg, szPrice);
	}
	//
	if(m_dwExpiredTime > 0)
	{
		char szBuffer[32];
		GetTimeDesc(szBuffer);
		strcat(pszMsg, "\n\n");
		strcat(pszMsg, szBuffer);
		strcat(pszMsg, "\n");
	}
	//

	if(m_btDaySell > 0)
	{
		char szBuffer[32];
		sprintf(szBuffer, "<color=fire>Thêi gian b¸n: %02d ngµy 00 giê 00 phót", 
				m_btDaySell);
		strcat(pszMsg, "\n\n");
		strcat(pszMsg, szBuffer);
		strcat(pszMsg, "\n");
	}

	if(m_nShopPrice > 0)
	{	
		char szInfo[64];
		if(m_nShopPrice < 10000)
		{	
			sprintf(szInfo, "<color=yellow>Gi¸ niªm yÕt: %d l­îng<color>", m_nShopPrice);
		}
		else
		{
			int nDivisor = m_nShopPrice / 10000;
			short nMod = m_nShopPrice % 10000;

			if(!nMod)
			{	
				sprintf(szInfo, "<color=yellow>Gi¸ niªm yÕt: %d v¹n l­îng<color>", nDivisor);
			}
			else
			{	
				sprintf(szInfo, "<color=yellow>Gi¸ niªm yÕt: %d v¹n %d l­îng<color>", nDivisor, nMod);
			}
		}
		strcat(pszMsg, "\n");
		strcat(pszMsg, szInfo);
		strcat(pszMsg, "\n");
	}
	//


#ifdef SWORDONLINE_SHOW_DBUG_INFO
	if(Player[CLIENT_PLAYER_INDEX].m_bDebugMode)
	{
		char szTemp[64];
		sprintf(szTemp, "<color=green>ItemID: [%d]<color>", m_dwID);
		strcat(pszMsg, "\n");
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "\n");
	}
#endif

	}

#endif

// --
//
// --
int KItem::GetMaxDurability()
{
	int i;
	for(i = 0; i < 7; i++)
	{
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			return m_aryBaseAttrib[i].nValue[0];
		}
	}
	return -1;
}

// --
//
// --
int KItem::GetTotalMagicLevel()
{
	int nRet = 0;
	for (int i = 0; i < 6; i++)
	{
		nRet += m_GeneratorParam.nGeneratorLevel[i];
	}
	return nRet;
}

// --
//
// --
int KItem::GetRepairPrice()
{
	if (ItemSet.m_sRepairParam.nMagicScale == 0)
		return 0;

	if (GetGenre() < item_equip ||
		GetGenre() > item_platinaequip)
		return 0;
	
	if(GetDetailType() == equip_ring || GetDetailType() == equip_amulet || 
		GetDetailType() == equip_pendant ||	GetDetailType() == equip_horse ||
		GetDetailType() == equip_mask || GetDetailType() == equip_mantle)
		return 0;

	if (m_nCurrentDur == -1)
		return 0;

	int nMaxDur = GetMaxDurability();
	int nSumMagic = GetTotalMagicLevel();

	if (nMaxDur <= 0)
		return 0;


	return m_CommonAttrib.nPrice * ItemSet.m_sRepairParam.nPriceScale / 100 * (nMaxDur - m_nCurrentDur) / nMaxDur * (ItemSet.m_sRepairParam.nMagicScale + nSumMagic) / ItemSet.m_sRepairParam.nMagicScale;
}

// --
//
// --
BOOL KItem::CanBeRepaired()
{
	if (GetGenre() < item_equip ||
		GetGenre() > item_platinaequip)
		return FALSE;
	
	if(GetDetailType() == equip_ring || GetDetailType() == equip_amulet || 
		GetDetailType() == equip_pendant ||	GetDetailType() == equip_horse ||
		GetDetailType() == equip_mask || GetDetailType() == equip_mantle)
		return FALSE;

	if (m_nCurrentDur == -1)
		return FALSE;

	int nMaxDur = GetMaxDurability();
	if (m_nCurrentDur == nMaxDur)
		return FALSE;

	return TRUE;
}

#ifndef _SERVER
// --
//
// --
char* KItem::GetRequireSeries(char cSeries) const
{
	switch(cSeries)
	{
		case series_metal:
			return "<color=Yellow>CÇn hÖ <color=earth>(Thæ)<color> ";
			break;
		case series_wood:
			return "<color=Yellow>CÇn hÖ <color=water>(Thñy)<color> ";
			break;
		case series_water:
			return "<color=Yellow>CÇn hÖ <color=metal>(Kim)<color> ";
			break;
		case series_fire:
			return "<color=Yellow>CÇn hÖ <color=wood>(Méc)<color> ";
			break;
		case series_earth:
			return "<color=Yellow>CÇn hÖ <color=fire>(Háa)<color> ";
			break;
		default:
			return "";
			break;
	}
}
#endif

// --
//
// --
void KItem::SetPrice(int nPrice, BYTE btType)
{
	m_CommonAttrib.nPrice = nPrice;
	m_btPriceType = btType;
}

// --
//
// --
int KItem::GetPrice(BOOL bRuled /*= FALSE*/, const char* lpszTongName /*= NULL*/, int nSubWorldId /*= 0*/) const
{	
	int nRet = m_CommonAttrib.nPrice - m_btDisCountPrice;

	if(m_btPriceType == 1)
		return nRet;

	if(bRuled)
	{
		
		int nTemp = nRet * (100.0);
		
	
		nRet = nRet + nTemp;
	}
	
	return nRet; 
}

#ifndef _SERVER
// --
//
// --
void KItem::GetPriceDesc(char cDisCount, int nPriceScale, const char* pszUnit, const char* pszColor, char* lpszPrice) const
{	
	char* szPrice = "";
	int nTotalPrice = 0;
	char szTongName[defTONG_NAME_MAX_LENGTH + 1];
	Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);

	if(nPriceScale == BUY_SELL_SCALE)
		nTotalPrice = this->GetPrice()/BUY_SELL_SCALE;
	else
		nTotalPrice = this->GetPrice();
	//else
	//	nTotalPrice = this->GetPrice(TRUE, szTongName);  

	if(cDisCount == -1)
	{	
		if(nTotalPrice/* / nPriceScale */< 10000)
		{	
			sprintf(lpszPrice, "%sGi¸ c¶: %d %s<color>", pszColor, nTotalPrice/* / nPriceScale*/, pszUnit);
		}
		else
		{
			int nDivisor = (nTotalPrice/* / nPriceScale*/) / 10000;
			short nMod = (nTotalPrice/* / nPriceScale*/) % 10000;

			if(!nMod)
			{	
				sprintf(lpszPrice, "%sGi¸ c¶: %d v¹n %s<color>", pszColor, nDivisor, pszUnit);
			}
			else
			{	
				sprintf(lpszPrice, "%sGi¸ c¶: %d v¹n %d %s<color>", pszColor, nDivisor, nMod, pszUnit);
			}
		}			
	}
	else if(cDisCount == 0)
	{
		if(m_CommonAttrib.nPrice/* / nPriceScale */< 10000)
		{	
			sprintf(lpszPrice, "<color=red>Gi¸ cò: %d %s<color>\n", m_CommonAttrib.nPrice/* / nPriceScale*/, pszUnit);
		}
		else
		{
			int nDivisor = (m_CommonAttrib.nPrice/* / nPriceScale*/) / 10000;
			short nMod = (m_CommonAttrib.nPrice/* / nPriceScale*/) % 10000;

			if(!nMod)
			{	
				sprintf(lpszPrice, "<color=red>Gi¸ cò: %d v¹n %s<color>", nDivisor, pszUnit);
			}
			else
			{	
				sprintf(lpszPrice, "<color=red>Gi¸ cò: %d v¹n %d %s<color>", nDivisor, nMod, pszUnit);
			}
		}
	}
}
#endif
// --
//
// --
void KItem::SetPlayerShopPrice(int nPrice)
{
	this->m_nShopPrice = nPrice;
}

// --
//
// --
int KItem::GetMagicAttrib(BYTE btNo, BYTE btType) const
{
	if(btNo < 0 || btNo >= 6)
		return 0;
	if(btType < 0 || btType >= 4)
		return 0;
	int nRet = 0;
	switch(btType)
	{
	case 0:
		nRet = m_aryMagicAttrib[btNo].nAttribType;
		break;
	case 1:
		nRet = m_aryMagicAttrib[btNo].nValue[0];
		break;
	case 2:
		nRet = m_aryMagicAttrib[btNo].nValue[1];
		break;
	case 3:
		nRet = m_aryMagicAttrib[btNo].nValue[2];
		break;
	default:
		break;
	}
	return nRet;
}


void KItem::SetBrokenEquip(const KBASICPROP_BROKEN_EQUIPMENT *pBroken)
{
	if(pBroken == NULL)
		return;
	
	m_CommonAttrib.cGenre		= (char)pBroken->m_nGenre;
	m_CommonAttrib.wObjID		= (WORD)pBroken->m_nObjID;
	m_CommonAttrib.bWidth		= (BYTE)pBroken->m_nWidth; 
	m_CommonAttrib.bHeight		= (BYTE)pBroken->m_nHeight;

#ifndef _SERVER
	memcpy(m_CommonAttrib.szImageName, pBroken->m_szIcon, sizeof(m_CommonAttrib.szImageName));
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, m_CommonAttrib.szImageName);
	m_Image.uImage = 0;
#endif
}
#ifdef _SERVER
// --
//
// --
void KItem::GetItemBackupInfo(OUT char* pszInfo)
{
	/*if(!pszInfo)
		return;
	// bo doan nay xem the nao co the gay loi dis
	sprintf(pszInfo, "%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d|"
					"%d",
					m_CommonAttrib.cGenre,
					m_CommonAttrib.nDetailType,
					m_CommonAttrib.bLevel,
					m_CommonAttrib.cSeries,
					GetGeneratorParam()->nVersion,
					GetGeneratorParam()->dwRandomSeed,
					GetGeneratorParam()->nGeneratorLevel[0],
					GetGeneratorParam()->nGeneratorLevel[1],
					GetGeneratorParam()->nGeneratorLevel[2],
					GetGeneratorParam()->nGeneratorLevel[3],
					GetGeneratorParam()->nGeneratorLevel[4],
					GetGeneratorParam()->nGeneratorLevel[5],
					GetGeneratorParam()->nLuck,
					GetDurability(),
					GetRecord(),
					GetBindState(),
					GetTime(),
					GetPlayerShopPrice());
					*/
}
// --
//
// --
void KItem::RestoreItemInfo(IN char* pszInfo)
{

}
#endif
// --
//
// --
BOOL KItem::CanStack()
{	
	if(m_CommonAttrib.cGenre < item_task ||
		m_CommonAttrib.cGenre > item_mine)
		return FALSE;

	if (m_aryBaseAttrib[0].nAttribType != magic_durability_v)
		return FALSE;

	return TRUE;
}
// --
//
// --
void KItem::SetBindState(const DWORD dwState, BOOL bSetTime /*= FALSE*/)
{
	if(!bSetTime)
	{
		m_dwBindState = dwState;
		return;
	}

	SetTime(dwState, TRUE);
}

// --
//
// --
void KItem::SetTime(const DWORD dwTime, BOOL bBind /*= FALSE*/)
{	
	DWORD dwTempTime = dwTime;
#ifdef _SERVER
	if( (dwTempTime > 0) && (dwTempTime <= 200000000) )
	{
		time_t rawtime;
		struct tm* timeinfo;
		//
		time (&rawtime);
		timeinfo = localtime(&rawtime);
		//
		timeinfo->tm_mday += dwTime;
		//
		time_t later = mktime(timeinfo);
		timeinfo = localtime ( &later );
		//time chuan by kinnox;
		dwTempTime = ( (timeinfo->tm_year + 1900) * 1000000 ) + ( (timeinfo->tm_mon + 1) * 10000) +  (timeinfo->tm_mday * 100) + timeinfo->tm_hour;//2021 05 29 19:30
		dwTempTime = (dwTempTime % 100000000); //2021 05 29 19
		dwTempTime = ((dwTempTime*100)+timeinfo->tm_min);//21 05 29 19:30
		//end code;
	}
#endif

	if(bBind == TRUE)
	{	
		m_dwBindState = dwTempTime;
		return;
	}

	if(dwTempTime <= 200000000)
		return;
	
	m_dwExpiredTime = dwTempTime;
}

// --
//
// --
DWORD KItem::GetTime(BOOL bBind /*= FALSE*/) const
{
	//if(bBind == TRUE)
	//	return m_dwBindTime;

	return m_dwExpiredTime;
}

// --
//time chuan by kinnox;
// --
BOOL KItem::DecodeTime(int* pnHour,int* pnMin, int* pnDate, int* pnMonth, int* pnYear, BOOL bBind /*= FALSE*/)
{	
	DWORD dwTime = m_dwExpiredTime;
	if(bBind == TRUE)
		dwTime = m_dwBindState;
	
	*pnHour = 0;
	*pnMin = 0;
	*pnDate = 0;
	*pnMonth = 0;
	*pnYear = 0;
	if( dwTime <= 200000000 )
		return FALSE;
	// 2105291930
	// 21 = Year ; 05 = month;  29 = Date ; 19 = Hour ; 30 = Min
	int dwTimeYear = 0;
	dwTimeYear = dwTime / 100000000; //2105291930
	*pnYear = dwTimeYear ; //21
	//
	dwTime = dwTime % 100000000; // 05291930
	*pnMonth = dwTime / 1000000; // 05
	//
	dwTime = dwTime % 1000000; //291930
	*pnDate =  dwTime / 10000; // 29
	//
	dwTime = dwTime % 10000; //1930
	*pnHour = dwTime / 100; //19
	
	*pnMin = dwTime % 100; //30
	return TRUE;
}

#ifndef _SERVER
// --
//time chuan by kinnox;
// --
void KItem::GetTimeDesc(char* pszTime, BOOL bBind /*= FALSE*/)
{	
	if(!pszTime)
		return;

	int nHour, nMin, nDate, nMonth, nYear;
	if( DecodeTime(&nHour, &nMin, &nDate, &nMonth, &nYear, bBind) )
	{	
		if(bBind == TRUE)
		{
			sprintf(pszTime, "<color=green>Thêi gian më khãa: %02d:%02d %02d-%02d-%02d<color>\n", nHour, nMin, nDate, nMonth, nYear + 2000);
		}
		else
		{
			sprintf(pszTime, "<color=fire>Thêi h¹n sö dông:<color> <color=green>%02d:%02d %02d-%02d-%02d \n", nHour, nMin, nDate, nMonth, nYear + 2000);
			
		}		
	}

}
#endif

// --
//
//	Code by Z (03/11/2015)
// --
int KItem::GetSuiteNo() const
{
	int nTemp = m_nParam1;
	if( nTemp <= 10000000 )
		return 0;
	nTemp = (nTemp % 1000000) / 10000;
	return nTemp;
}

// --
//
//	Code by Z (03/11/2015)
// --
int KItem::GetSuiteId() const
{
	int nTemp = m_nParam1;
	if( nTemp <= 10000000 )
		return 0;
	nTemp = ((nTemp % 1000000) % 10000) / 100;
	return nTemp;
}

// --
//
//	Code by Z (03/11/2015)
// --
int KItem::GetSuiteCount() const
{
	int nTemp = m_nParam1;
	if( nTemp <= 10000000 )
		return 0;
	nTemp = ((nTemp % 1000000) % 10000) % 100;
	return nTemp;
}

#ifndef _SERVER
// --
//
// --
void KItem::GetSuiteDesc(char* pszSuite)
{	
	int i, nRow, nColor;
	char szName[80];

	sprintf(pszSuite, "\n");
	for(i = 0; i < this->GetSuiteCount(); i++)
	{
		nRow = m_wRecord - GetSuiteId() + i + 2;
		g_cGoldEquip.GetString(nRow, "ItemName", "", szName, sizeof(szName));
		if(GetSuiteId() == i + 1)
		{
			strcat(pszSuite, "<color=Gold>");
		}
		else
		{
			nColor = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetSuiteDescColor(nRow - 1, i + 1);
			if(nColor == 1)
			{
				strcat(pszSuite, "<color=Green>");
			}
			else
			{
				strcat(pszSuite, "<color=DGreen>");
			}
		}
		
		strcat(pszSuite, szName);
		strcat(pszSuite, "\n");
	}
}
#endif

#ifdef _SERVER
// --
//time chuan by kinnox;
// --
BOOL KItem::CheckVaildTime(BOOL bBind /* =FALSE*/)
{	
	int nHour, nMin, nDate, nMonth, nYear;
	
	if( DecodeTime(&nHour, &nMin, &nDate, &nMonth, &nYear, bBind) == FALSE )
		return TRUE;

	time_t rawtime;
	struct tm* timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	
	nYear = nYear + 100;

#ifdef _DEBUG
	//g_DebugLog("[Check Expired Item] CurTime[%02d:%02d %02d-%02d-%02d] - ExpiredTime[%02d:%02d %02d-%02d-%02d]", 
				timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, 
				nHour, nMin, nDate, nMonth, nYear + 1900);
#endif
	
	if (nYear < timeinfo->tm_year || 
	   (nYear == timeinfo->tm_year && nMonth < timeinfo->tm_mon + 1) || 
	   (nYear == timeinfo->tm_year && nMonth == timeinfo->tm_mon + 1 && nDate < timeinfo->tm_mday) || 
	   (nYear == timeinfo->tm_year && nMonth == timeinfo->tm_mon + 1 && nDate == timeinfo->tm_mday && nHour < timeinfo->tm_hour) || 
	   (nYear == timeinfo->tm_year && nMonth == timeinfo->tm_mon + 1 && nDate == timeinfo->tm_mday && nHour == timeinfo->tm_hour && nMin <= timeinfo->tm_min))
	{
		return FALSE;
	}

	return TRUE;
}

#endif