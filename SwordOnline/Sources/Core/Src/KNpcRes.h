
#pragma once

#ifndef _SERVER

#include "KWavSound.h"
#include "KList.h"
#include "KNpcResNode.h"
#include "KNpcResList.h"
#include "KSprControl.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"

class KCacheNode;

class KStateSpr
{
public:
	int				m_nID;						
	int				m_nType;				
	int				m_nPlayType;				
	int				m_nBackStart;				
	int				m_nBackEnd;					
	KSprControl		m_SprContrul;				
public:
	KStateSpr();
	void			Release();
};

#define		MAX_BLUR_FRAME		7
#define		START_BLUR_ALPHA	128
#define		BLUR_ALPHA_CHANGE	16

class KNpcBlur
{
public:
	int				m_nActive;
	int				m_nCurNo;
	DWORD			m_dwTimer;
	DWORD			m_dwInterval;
	int				m_nMapXpos[MAX_BLUR_FRAME];
	int				m_nMapYpos[MAX_BLUR_FRAME];
	int				m_nMapZpos[MAX_BLUR_FRAME];
	unsigned int	m_SceneIDNpcIdx[MAX_BLUR_FRAME];
	unsigned int	m_SceneID[MAX_BLUR_FRAME];
	KRUImage		m_Blur[MAX_BLUR_FRAME][MAX_PART];
public:
	KNpcBlur();
	~KNpcBlur();
	BOOL			Init();
	void			Remove();
	void			SetNextNo();
	void			SetMapPos(int x, int y, int z, int nNpcIdx);
	void			ChangeAlpha();
	void			ClearCurNo();
	void			SetFile(int nNo, char *lpszFileName, int nSprID, int nFrameNo, int nXpos, int nYpos, int nZpos);
	void			Draw(int nIdx);
	BOOL			NowGetBlur();
	void			AddObj();
	void			RemoveObj();
	void			Remove(int i);
};

#ifdef TOOLVERSION
class CORE_API  KNpcRes
#else
class KNpcRes
#endif
{

	enum
	{
		SHADOW_BEGIN	= 0,
		STATE_BEGIN		= 1,
		PART_BEGIN		= 1 + 6,
		SPEC_BEGIN		= 1 + MAX_PART + 6,
		MENUSTATE_BEGIN = 1 + MAX_PART + 6 + 1,
	};
private:
	int				m_nDoing;							
	int				m_nAction;							
	int				m_nNpcKind;							
	int				m_nXpos;							 
	int				m_nYpos;							
	int				m_nZpos;							
	unsigned int 	m_SceneID_NPCIdx;                 
	unsigned int	m_SceneID;							
	int				m_nHelmType;						
	int				m_nArmorType;						
	int				m_nWeaponType;					
	int				m_nHorseType;						
	BOOL			m_bRideHorse;
	
	int				m_nMantleType;
	
	int				m_nBlurState;
	char			m_szSoundName[80];					// µ±Ç°ÒôÐ§ÎÄ¼þÃû
	KCacheNode		*m_pSoundNode;						// ÉùÐ§Ö¸Õë
	KWavSound		*m_pWave;							// ÉùÐ§wavÖ¸Õ
	
public:
	enum
	{
			adjustcolor_physics = 0,		
			adjustcolor_poison, 
			adjustcolor_freeze,			
			adjustcolor_burn,			
			adjustcolor_confuse,		
			adjustcolor_stun,
	};
	KSprControl		m_cNpcImage[MAX_PART];				
	KSprControl		m_cNpcShadow;						
	KStateSpr		m_cStateSpr[MAX_SKILL_STATE];		//
	KSprControl		m_cSpecialSpr;						
	unsigned int	m_ulAdjustColorId;

	KSprControl		m_cMenuStateSpr;
	int				m_nMenuState;
	int				m_nBackMenuState;
	int				m_nSleepState;
	//char			m_szSentence[MAX_SENTENCE_LENGTH];
	//char			m_szBackSentence[MAX_SENTENCE_LENGTH];
	
	int				m_nSortTable[MAX_PART];

	KRUImage		m_cDrawFile[MAX_NPC_IMAGE_NUM];
	KNpcBlur		m_cNpcBlur;

	KNpcResNode		*m_pcResNode;

private:
	void			GetSoundName();						
	void			PlaySound(int nX, int nY);
	void			SetMenuStateSpr(int nMenuState);
public:
	KNpcRes();
	~KNpcRes();
	BOOL			Init(char *lpszNpcName, KNpcResList *pNpcResList);	
	void			Remove(int nNpcIdx);								
	void			Draw(int nNpcIdx, int nDir, int nAllFrame, int nCurFrame, BOOL bInMenu = FALSE);		
	void			DrawBorder();
	int				DrawMenuState(int nHeightOffset);
	BOOL			SetHelm(int nHelmType);								
	BOOL			SetArmor(int nArmorType);							
	BOOL			SetWeapon(int nWeaponType);							
	BOOL			SetHorse(int nHorseType);
	
	BOOL			SetMantle(short nMantleType);
	
	BOOL			SetAction(int nDoing);								
	int				GetHelm(){return m_nHelmType;};
	int				GetArmor(){return m_nArmorType;	};
	int				GetWeapon(){return m_nWeaponType;};
	BOOL			SetRideHorse(BOOL bRideHorse);						// Éè¶¨ÊÇ·ñÆïÂí
	void			SetPos(int nNpcIdx, int x, int y, int z = 0, BOOL bFocus = FALSE, BOOL bMenu = FALSE);// Éè¶¨ npc Î»ÖÃ
	void			SetState(KList *pNpcStateList, KNpcResList *pNpcResList);	// Éè¶¨×´Ì¬ÌØÐ§
	void			SetSpecialSpr(char *lpszSprName);					// Éè¶¨ÌØÊâµÄÖ»²¥·ÅÒ»±éµÄËæÉísprÎÄ¼þ
	void			SetBlur(BOOL bBlur);								// ²ÐÓ°´ò¿ª¹Ø±Õ
	void			SetAdjustColorId(unsigned long ulColorId){m_ulAdjustColorId = ulColorId;};			// ÉèÖÃÆ«É«Çé¿ö£¬Èç¹ûÎª0±íÊ¾²»Æ«É«.
	int				GetAction(){return m_nAction;};
	void			SetMenuState(int nState, char *lpszSentence = NULL, int nSentenceLength = 0);	// Éè¶¨Í·¶¥×´Ì¬
	int				GetMenuState();						// »ñµÃÍ·¶¥×´Ì¬
	void			SetSleepState(BOOL bFlag);			// Éè¶¨Ë¯Ãß×´Ì¬
	BOOL			GetSleepState();						// »ñµÃË¯Ãß×´Ì¬
	void			StopSound();
	int				GetSndVolume(int nVol);
	static void		GetShadowName(char *lpszShadow, char *lpszSprName);
	int				GetNormalNpcStandDir(int nFrame);
	
	int				GetSprNumFrames(const char* lpzsImage, int nImageType);
	void			SetBlurSecMove(int nIndex, int nX, int nY, int nZ);
	void			RemoveBlurSecMove(int nCurNo);
	
};
#endif
