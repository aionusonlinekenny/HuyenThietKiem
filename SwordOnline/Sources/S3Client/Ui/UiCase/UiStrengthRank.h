
#if !defined(AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_)
#define AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KIniFile.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
#include "../Elem/WndMessageListBox.h"
#include "../../../Engine/src/LinkStruct.h"
#include "../../Core/Src/GameDataDef.h"

#define MAX_RANK_MESSAGE_LENGTH 128

struct KRankIndexNode : public KRankIndex
{
	char szMsg[MAX_RANK_MESSAGE_LENGTH];
	int  nMsgLen;
	char szUnit[16];
	KRankIndexNode *next;
	KRankIndexNode *previous;
};

struct KRankNode : public KRankMessage
{
	KRankNode* next;
	KRankNode* previous;
};

class KUiStrengthRank : public KWndShowAnimate
{
public:
	KUiStrengthRank();
	virtual ~KUiStrengthRank();

	static        KUiStrengthRank* OpenWindow();    
	static        KUiStrengthRank* OpenDefaultWindow(int nId = -1);
	static        KUiStrengthRank* GetIfVisible();  
	static void   CloseWindow(bool bDestory = TRUE);
	static void   LoadScheme(const char* pScheme); 

	static void   NewRankArrive(unsigned int uParam, KRankMessage *pRankData);
	static void   NewIndexArrive(unsigned int uParam, struct KRankIndex *pIndex);

	static int    AttachTempFile();                            
	static void   RemoveTempFile();                            

private:
	static void   StoreTempData( );                            
	static void   StoreNewDataToTempFile(unsigned int uParam,  
		                                 KRankMessage *pInData);
	static void   StoreNewDataToTempFile(unsigned int uParam,  
		                                 KRankIndex *pInData);

private:
	int	   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void Breathe();                                    

private:
	static KUiStrengthRank* m_pSelf;

private:
	void   Initialize();
//	void   UpdateIndexData();                   
	void   UpdateRankData(int nID);             

	static void StoreTempIni();                 
	BOOL   LoadIndexIni(KIniFile *Ini);         
	BOOL   LoadRankIni(KIniFile *Ini, int nID); 
                                                
//	void   SendRobIndexToCore();                
//	void   SendGetIndexToCore();                
	void   SendGetRankToCore(int nID);          
                                                
	void   ConstructIndexView();              
	void   ConstructRankView();                 
	void   ReleasePic();                        
	void   ReleaseData();                       

	void   ArrangeDefaultData();                

	static void   AddRank(unsigned int uParam, KRankMessage *pRankData);
	static void   AddIndex(unsigned int uParam, struct KRankIndex *pIndex);

private:
	KWndButton				  m_BtnClose;
	KWndList				  m_IndexList;
	KWndScrollBar			  m_IndexScroll;
	KWndMessageListBox		  m_RankList;
	KWndScrollBar			  m_RankScroll;

	static KIniFile           m_IndexIDIni;
	static KLinkStruct<KRankIndexNode> m_IndexData;
	static KLinkStruct<KRankNode> m_RankData;       

	unsigned int              m_uColor;
	static BOOL               m_bIsIndexDataChange;
	static BOOL               m_bIsRankDataChange; 
	KRankIndexNode           *m_pCurrentIndexNode; 

	int                       m_nFontSize;          
	int                       m_nLineLen;           
	int                       m_nDefaultID;          
	unsigned char            *m_pIDontThinkSo;
    unsigned short            m_usUpPic;            
	unsigned short            m_usSteadyPic;        
	unsigned short            m_usDownPic;

	int						  m_nLastIndex;
	unsigned int             *m_pLastGettimes;
	unsigned int              m_uLastGetIndexTime;
	unsigned int              m_uLastGetRankTime;

	char                      m_szUpPicPath[128];
	char                      m_szDownPicPath[128];
	char                      m_szSteadyPicPath[128];
};

int TDrawOutString(char *szBuf, int nBufLen, int nLineLen, int nFontSize);
#endif // !defined(AFX_KUISTRENGTHORDER_H__7801F7C7_BAE8_4A49_BEA8_63B5F8FA0BB1__INCLUDED_)
